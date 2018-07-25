using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using DBManager;

namespace FlowChartEditor.Command.TreeCommand
{
    class TreeDeleteNodeCommand : TreeBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public TreeDeleteNodeCommand(object data, string description):
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
            TreeNode currentNode = tree.SelectedNode;

            if (currentNode != null)
            {
                DialogResult result = MessageBox.Show("ȷ��ɾ��������㼰�����е��ӽ����", "ɾ��ȷ��",
                                MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (result == DialogResult.Yes)
                {
                    // ��������ִ��ǰ��״̬
                    description = "ɾ������� " + currentNode.Text;
                                      
                    // �����ݿ���в���
                    DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
                    if(currentNode.ImageIndex == 0) // ɾ������
                    {
                        dataBaseManager.DeleteMultiDiagram(documentManager.GetNodePath(currentNode));
                    }
                    else // ɾ������ͼ
                    {
                        dataBaseManager.DeleteDiagram(currentNode.Tag.ToString());
                    }

                    currentNode.Remove();
                    documentManager.CurrentTreeMode = EditMode.Normal; 

                    success = true;
                }
            }

            return success;
        }
    }
}
