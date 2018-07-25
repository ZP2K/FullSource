using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartDisconnectCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartDisconnectCommand(object data, string description):
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
            SlotGraphElement slot = o as SlotGraphElement;

            // ��������ִ��ǰ������
            if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
            {
                SaveBeforeExecute(flowChartManager.GetArguments());                
            }

            ConnectorGraphElement connector = slot.BindingConnector;
            ConnectorContainer line = connector.Line;
            SlotContainer slotContainer = slot.SlotContainer;
            description = "�������ͼԪ " + slotContainer.Name;

            slot.UnBind();
            if(slot.CanDelete && (slot.IsInSlot || slot.SlotContainer.OutSlotCount > 1)) // ��ۿ���ɾ��
            {
                slotContainer.RemoveSlot(slot);                
            }                                   

            // ִ���߼�����
            LogicBaseManager logicManager = helper.GetLogicManager(flowChartManager.CurrentChartMode);
            object[] logicData = new object[] { slot, connector };
            logicManager.DoLogicOperation(flowChartManager, LogicType.Disconnect, logicData);

            if(success) // ��������ִ�к������
            {
                flowChartManager.ContentChanged = true;
                SaveAfterExecute(flowChartManager.GetArguments());                
            }                        

            return success;
        }
    }
}
