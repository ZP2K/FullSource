using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using DBManager;

namespace FlowChartEditor.Command.TreeCommand
{
    class TreeRenameCommand : TreeBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public TreeRenameCommand(object data, string description)
            : base(data, description)
        {
            
        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        /// <return>�Ƿ�ִ�гɹ�</return>
        public override bool Execute(object o)
        {
            bool success = false;
            TreeNode currentNode = tree.SelectedNode;

            if (currentNode != null)
            {
                // ��ʾ���������
                InputForm iForm = new InputForm("��������������������", currentNode.Text);
                DialogResult result = iForm.ShowDialog();

                if (result == DialogResult.OK)
                {
                    if (CheckPathValid(currentNode, iForm.InputText)) // ��ǰ���ظ�·��
                    {
                        description = "����������� " + iForm.InputText;
                        currentNode.Text = iForm.InputText;
                        currentNode.Name = iForm.InputText;
                        documentManager.CurrentTreeMode = EditMode.Normal;
                        
                        // �����ݿ���в���
                        DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();                                             

                        // �������»�ͼ·��
                        List<TreeNode> nodeList = GetChildNodeList(currentNode);
                        foreach (TreeNode node in nodeList)
                        {
                            string newPath = documentManager.GetNodePath(node);
                            dataBaseManager.UpdateDiagramPath(node.Tag.ToString(), newPath);
                        }               

                        success = true;
                    }
                    else
                    {
                        MessageBox.Show("��·���Ѿ����ڣ�", "·����Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }                                      
                }
            }

            return success;
        }
    }
}
