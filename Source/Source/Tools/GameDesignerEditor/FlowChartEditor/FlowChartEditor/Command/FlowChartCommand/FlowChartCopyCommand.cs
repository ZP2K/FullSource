using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartCopyCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartCopyCommand(object data, string description):base(data, description)
        {

        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">����Ĳ���</param>
        /// <returns>�Ƿ�ִ�гɹ�</returns>
        public override bool Execute(object o)
        {
            bool success = true;

            // ��������ִ��ǰ������
            if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
            {
                SaveBeforeExecute(flowChartManager.GetArguments());                
            }

            Hashtable dataTable = new Hashtable();

            if (graphManager.SelectedGraphElementList.Count > 0) // ���ƶ��ͼԪ
            {
                foreach (GraphElement graphElement in graphManager.SelectedGraphElementList)
                {
                    dataTable[graphElement] = dataManager.GetData(graphElement);
                }
            }
            else // ���Ƶ���ͼԪ
            {
                GraphElement graphElement = graphManager.SelectedGraphElement;
                dataTable[graphElement] = dataManager.GetData(graphElement);
            }
            
            documentManager.CopyTable = CopyGraphElement(dataTable);

            if (success) // ��������ִ�к������
            {
                SaveAfterExecute(flowChartManager.GetArguments());
            }

            return success;
        }
    }
}
