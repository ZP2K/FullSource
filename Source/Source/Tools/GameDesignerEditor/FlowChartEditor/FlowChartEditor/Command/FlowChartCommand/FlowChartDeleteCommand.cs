using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using GraphElementLibrary;
using DataElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartDeleteCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartDeleteCommand(object data, string description):
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
            GraphElement graphElement = o as GraphElement;           

            DialogResult result = MessageBox.Show("�Ƿ�ȷ��ɾ��ѡ�е�ͼԪ��", "ɾ������ȷ��", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (result == DialogResult.Yes) // �û�ȷ��ɾ��
            {                
                graphManager.CurrentCanvas.PaintCanvas = false; // �������ʱʹ���Ѿ��ͷŵ���Դ

                // ��������ִ��ǰ������
                if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
                {                    
                    SaveBeforeExecute(flowChartManager.GetArguments());
                }   

                description = "ɾ��ͼԪ " + graphElement.Name;
                success = DeleteGraphElement(graphElement);                                                                
            }
            else
            {
                success = false;
            }

            // ��������ִ�к������
            if(success)
            {
                flowChartManager.ContentChanged = true;
                SaveAfterExecute(flowChartManager.GetArguments());                
            }

            return success;
        }
    }
}
