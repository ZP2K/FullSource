using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Collections;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartConnectCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartConnectCommand(object data, string description):
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
            SlotGraphElement  slot = args[0] as SlotGraphElement;
            ConnectorGraphElement connector = args[1] as ConnectorGraphElement;

            // ��������ִ��ǰ������
            if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
            {
                SaveBeforeExecute(flowChartManager.GetArguments());                
            }            

            description = "����ͼԪ " + slot.SlotContainer.Name;
            slot.Bind(connector);                        

            // ִ���߼�����
            LogicBaseManager logicManager = helper.GetLogicManager(flowChartManager.CurrentChartMode);
            success = logicManager.DoLogicOperation(flowChartManager, LogicType.Connect, o);

            if (success) // ��������ִ�к������
            {
                flowChartManager.ContentChanged = true;
                SaveAfterExecute(flowChartManager.GetArguments());                
            }

            return success;
        }
    }
}
