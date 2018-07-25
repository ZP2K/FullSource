using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using DBManager;

namespace FlowChartEditor.Command.TreeCommand
{
    class TreePasteNodeCommand : TreeBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public TreePasteNodeCommand(object data, string description):
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
            TreeNode nodeToAdd;
            TreeNode nodeToClone = null;

            if (currentNode != null)
            {
                if (currentNode.ImageIndex == 0) // ��ǰ����Ƿ�����
                {
                    nodeToAdd = currentNode;
                }
                else // ��ǰ���������ͼ���
                {
                    TreeNode parentNode = currentNode.Parent;
                    nodeToAdd = parentNode;
                }

                switch (documentManager.CurrentTreeMode)
                {
                    case EditMode.Cut:
                        {                            
                            nodeToClone = documentManager.CutTreeNode;                            

                            break;
                        }
                    case EditMode.Copy:
                        {                            
                            nodeToClone = documentManager.CopyTreeNode;                            

                            break;
                        }
                }

                if (nodeToClone != null)
                {
                    string nodeText = nodeToClone.Text;
                    bool nodeExist = false;

                    foreach(TreeNode node in nodeToAdd.Nodes)
                    {
                        if(node.Text == nodeText)
                        {
                            nodeExist = true;
                            break;
                        }
                    }

                    if (nodeExist) // ��ͬ���ֵ�
                    {
                        nodeText += " ����";
                    }

                    // ��������ִ��ǰ��״̬
                    description = "ճ������� " + nodeText;

                    TreeNode newNode = nodeToAdd.Nodes.Add(nodeText);
                    newNode.Name = nodeText;
                    newNode.Tag = nodeToClone.Tag;
                    newNode.ImageIndex = nodeToClone.ImageIndex;
                    newNode.SelectedImageIndex = nodeToClone.SelectedImageIndex;
                    tree.SelectedNode = newNode;

                    // �����ݿ���в���
                    DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
                    string path = documentManager.GetNodePath(newNode);

                    if (documentManager.CurrentTreeMode == EditMode.Cut) // ���������
                    {
                        dataBaseManager.UpdateDiagramPath(newNode.Tag.ToString(), path);
                    }
                    else // ճ�������
                    {
                        string id = nodeToClone.Tag.ToString();
                        string chartClassString = documentManager.CurrentChartMode;
                        object flowChartData = dataBaseManager.GetDiagramData(id);
                        id = dataBaseManager.CreateNewDiagram(path, false, chartClassString);
                        newNode.Tag = id;

                        if (flowChartData != null) // ԭͼΪ�յ�ʱ��Ͳ����ٴ���
                        {
                            dataBaseManager.SaveDiagramData(id, flowChartData);
                        }                        
                    }                    

                    success = true;
                }
            }  

            return success;
        }
    }
}
