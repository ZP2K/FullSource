using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using GraphElementLibrary;
using DataElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartMultiDeleteCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartMultiDeleteCommand(object data, string description):
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
            List<GraphElement> graphElementList = o as List<GraphElement>;

            DialogResult result = MessageBox.Show("�Ƿ�ȷ��ɾ��ѡ�е�ͼԪ��", "ɾ������ȷ��", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (result == DialogResult.Yes) // �û�ȷ��ɾ��
            {
                graphManager.CurrentCanvas.PaintCanvas = false; // �������ʱʹ���Ѿ��ͷŵ���Դ

                // ��������ִ��ǰ������
                if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
                {
                    SaveBeforeExecute(flowChartManager.GetArguments());                    
                }   

                description = "����ɾ��ͼԪ";
                foreach(GraphElement graphElement in graphElementList)
                {
                    success = DeleteGraphElement(graphElement);
                    if(!success)
                    {
                        MessageBox.Show(string.Format("IDΪ{0}��ͼԪɾ��ʧ�ܣ�", graphElement.ID), "ɾ��ͼԪ", 
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                        break;
                    }
                }

                graphElementList.Clear();                

                // ��������ִ�к������
                if(success)
                {
                    flowChartManager.ContentChanged = true;
                    SaveAfterExecute(flowChartManager.GetArguments());                    
                }                
            }

            return success;
        }
    }
}
