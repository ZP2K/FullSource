using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartCreateAndConnectLineCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartCreateAndConnectLineCommand(object data, string description):
            base(data, description)
        {

        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        /// <returns>�Ƿ�ִ�гɹ�</returns>
        public override bool Execute(object o)
        {
            bool success = true;
            Helper helper = Helper.GetHelper();
            object[] args = o as object[];
            SlotGraphElement outSlot = args[0] as SlotGraphElement;
            SlotGraphElement inSlot = args[1] as SlotGraphElement;

            // ��������ִ��ǰ������
            if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
            {                
                SaveBeforeExecute(flowChartManager.GetArguments());
            }

            // ִ���߼�����
            LogicBaseManager logicManager = helper.GetLogicManager(flowChartManager.CurrentChartMode);
            logicManager.DoLogicOperation(flowChartManager, LogicType.CreateAndConnectLine, o);

            ConnectorContainer line = graphManager.SelectedGraphElement as ConnectorContainer;
            outSlot.Bind(line.GetConnectorList()[0]);
            inSlot.Bind(line.GetConnectorList()[1]);
            
            graphManager.ConnectorContainerList.Add(line);            
            graphManager.SelectGraphElement(line, false);
            description = "����������ͼԪ " + outSlot.SlotContainer.Name + " - " + inSlot.SlotContainer.Name;
            dataManager.AddDataElement(line);
            graphManager.ReconstructCanvasGraphElementList();

            // ִ���߼�����
            object[] logicData = new object[]{ outSlot, outSlot.BindingConnector };
            logicManager.DoLogicOperation(flowChartManager, LogicType.Connect, logicData);

            if (success) // ��������ִ�к������
            {
                flowChartManager.ContentChanged = true;                
                SaveAfterExecute(flowChartManager.GetArguments());
            }            

            return success;
        }
    }
}
