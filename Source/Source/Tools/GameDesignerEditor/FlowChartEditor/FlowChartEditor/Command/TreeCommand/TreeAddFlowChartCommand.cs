using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using DBManager;

namespace FlowChartEditor.Command.TreeCommand
{
    class TreeAddFlowChartCommand : TreeBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">��������</param>
        public TreeAddFlowChartCommand(object data, string description):
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

            // ִ���߼�����
            LogicBaseManager logicManager = Helper.GetHelper().GetLogicManager(documentManager.CurrentChartMode);
            logicManager.DoLogicOperation(documentManager.CurrentFlowChartManager, LogicType.CreateChart, o);            

            return success;
        }
    }
}
