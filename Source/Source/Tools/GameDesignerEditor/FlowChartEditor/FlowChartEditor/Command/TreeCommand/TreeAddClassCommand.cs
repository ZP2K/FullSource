using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using DBManager;

namespace FlowChartEditor.Command.TreeCommand
{
    class TreeAddClassCommand:TreeBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">��������</param>
        public TreeAddClassCommand(object data, string description):
            base(data, description)
        {

        }        

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">�������</param>
        /// <returns>�Ƿ�ִ�гɹ�</returns>
        public override bool Execute(object o)
        {
            bool success = false;
            List<string> mapList = o as List<string>;
            TreeNode currentNode = tree.SelectedNode;
            TreeNode newNode;
            DialogResult result;
            string inputText;

            if (currentNode.Level > 0) // �ڵ�ǰѡ����������½�����
            {
                InputForm iForm = new InputForm("�������½����������", "�½�����");
                result = iForm.ShowDialog();
                inputText = iForm.InputText;
            }
            else // �ڸ�������½�����
            {
                ListForm lForm = new ListForm("��ѡ���½�����ĵ�ͼ��", mapList);
                result = lForm.ShowDialog();
                inputText = lForm.InputText;
            }

            if (result == DialogResult.OK)
            {
                if (CheckPathValid(currentNode, inputText)) // ��ǰ���ظ�·��
                {
                    // ��������ִ��ǰ��״̬
                    description = ("�½����� " + inputText);                    

                    if (currentNode != null) 
                    {
                        newNode = currentNode.Nodes.Add(inputText);
                    }
                    else 
                    {
                        newNode = tree.Nodes.Add(inputText);
                    }

                    newNode.Name = inputText;
                    newNode.ImageIndex = 0;
                    newNode.SelectedImageIndex = 0;
                    tree.SelectedNode = newNode;
                    documentManager.CurrentTreeMode = EditMode.Normal;

                    // �����ݿ���в���
                    DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
                    string path = documentManager.GetNodePath(newNode);
                    string id = dataBaseManager.CreateNewDiagram(path, true, documentManager.CurrentChartMode);
                    newNode.Tag = id;

                    // ��������ִ�к��״̬                    
                    success = true;
                }
                else
                {
                    MessageBox.Show("��·���Ѿ����ڣ�", "·����Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }

            return success;
        }
    }
}
