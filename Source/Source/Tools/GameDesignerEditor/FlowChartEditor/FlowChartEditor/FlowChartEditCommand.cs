using System;
using System.Collections.Generic;
using System.Text;
using GraphElementLibrary;

namespace FlowChartEditor
{
    class FlowChartEditCommand:FlowChartCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartEditCommand(object data, string description):
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
            bool success = false;
            GraphManager graphManager = data as GraphManager;
            DataManager dataManager = graphManager.CurrentFlowChartManager.CurrentDataManager;
            GraphElement graphElement = o as GraphElement;

            // ����ִ��ǰ������
            description = "�༭ͼԪ " + graphElement.Name;
            dataBeforeExecute.SaveData(dataManager.GetArguments());

            success = graphManager.CurrentFlowChartManager.EditDataElement(graphElement);

            // ����ִ�к������
            dataAfterExecute.SaveData(dataManager.GetArguments());

            return success;
        }
    }
}
