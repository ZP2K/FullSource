using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using FlowChartEditor.BaseGraphElement;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartCreateLineCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartCreateLineCommand(object data, string description):
            base(data, description)
        {

        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public override bool Execute(object o)
        {
            bool success = true;
            Helper helper = Helper.GetHelper();
            object[] args = o as object[];
            SlotGraphElement slot = args[0] as SlotGraphElement;

            // ��������ִ��ǰ������
            if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
            {                
                SaveBeforeExecute(flowChartManager.GetArguments());
            }   
            
            // ִ���߼�����
            LogicBaseManager logicManager = helper.GetLogicManager(flowChartManager.CurrentChartMode);
            logicManager.DoLogicOperation(flowChartManager, LogicType.CreateLine, o);

            ConnectorContainer line = graphManager.SelectedGraphElement as ConnectorContainer;
            graphManager.ConnectorContainerList.Add(line);
            description = "����ͼԪ " + line.Name;
            dataManager.AddDataElement(line);
            graphManager.ReconstructCanvasGraphElementList();

            // ִ���߼�����
            object[] logicData = new object[] { slot, slot.BindingConnector };
            logicManager.DoLogicOperation(flowChartManager, LogicType.Connect, logicData);

            if(success) // ��������ִ�к������
            {
                flowChartManager.ContentChanged = true;
                SaveAfterExecute(flowChartManager.GetArguments());                
            }                        

            return success;
        }
    }
}
