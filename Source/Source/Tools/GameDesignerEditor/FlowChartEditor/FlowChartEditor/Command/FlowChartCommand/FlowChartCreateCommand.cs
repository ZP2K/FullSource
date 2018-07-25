using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;
using DataElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartCreateCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartCreateCommand(object data, string description):
            base(data, description)
        {

        }

        /// <summary>
        /// ��ʼ��ͼ���
        /// </summary>
        /// <param name="graphElement">������</param>
        /// <param name="id">���ID</param>
        /// <param name="name">�������</param>
        private void InitSlotContainer(SlotContainer slotContainer, int id, string name)
        {
            slotContainer.ID = id;
            slotContainer.Name = name;
            slotContainer.Init();
            graphManager.SlotContainerList.Add(slotContainer);

            description = "����ͼԪ " + slotContainer.Name;
            dataManager.AddDataElement(slotContainer);            
            graphManager.SelectGraphElement(slotContainer, false);
            graphManager.ReconstructCanvasGraphElementList(); 
        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        /// <returns>�Ƿ�ִ�гɹ�</returns>
        public override bool Execute(object o)
        {
            bool success = true;
            object[] args = o as object[];
            int id = graphManager.AllocateGraphElementID();
            Point p = (Point)args[0];
            string typeString = args[1] as string;
            GraphSetting graphSetting = GraphSetting.GetGraphSetting();

            // ��������ִ��ǰ������
            if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
            {
                SaveBeforeExecute(flowChartManager.GetArguments());
            }   

            switch (typeString)
            {
                case "ConditionNode": // �������
                    {
                        ConditionGraphElement conditionGraphElement = new ConditionGraphElement(p, graphSetting.ConditionNodeElementSize);                        
                        InitSlotContainer(conditionGraphElement, id, "�������");

                        break;
                    }
                case "ActionNode": // �������
                    {
                        ActionGraphElement actionGraphElement = new ActionGraphElement(p, graphSetting.ActionNodeElementSize);
                        InitSlotContainer(actionGraphElement, id, "�������");

                        break;
                    }
                case "EventNode": // �¼����
                    {
                        EventGraphElement eventGraphElement = new EventGraphElement(p, graphSetting.EventNodeElementSize);                        
                        InitSlotContainer(eventGraphElement, id, "�¼����");

                        break;
                    }
                case "AIStateNode": // ai״̬���
                    {
                        AIStateGraphElement aiStateGraphElement = new AIStateGraphElement(p, graphSetting.AIStateNodeElementSize);                        
                        InitSlotContainer(aiStateGraphElement, id, "AI״̬���");

                        break;
                    }
                case "AIActionNode": // ai�������
                    {
                        AIActionGraphElement aiActionGraphElement = new AIActionGraphElement(p, graphSetting.AIActionNodeElementSize);                        
                        InitSlotContainer(aiActionGraphElement, id, "AI�������");

                        break;
                    }
                case "AIActionsNode": // ai��������
                    {
                        AIActionsGraphElement aiActionsGraphElement = new AIActionsGraphElement(p, graphSetting.AIActionsNodeElementSize);                        
                        InitSlotContainer(aiActionsGraphElement, id, "AI��������");

                        break;
                    }
                case "InnerChart": // �ӻ�ͼ���
                    {
                        InnerChart innerChart = new InnerChart(p, graphSetting.InnerChartElementSize);                        
                        InitSlotContainer(innerChart, id, "�ӻ�ͼ���");

                        break;
                    }
                case "InterfaceNode": // �ӿڽ��
                    {
                        InterfaceGraphElement interfaceGraphElement = new InterfaceGraphElement(p, graphSetting.InterfaceNodeElementSize);                        
                        InitSlotContainer(interfaceGraphElement, id, "�ӿڽ��");

                        break;
                    }
            }
            
            if (success) // ��������ִ�к������
            {
                flowChartManager.ContentChanged = true;                
                SaveAfterExecute(flowChartManager.GetArguments());
            }

            return success;
        }
    }
}
