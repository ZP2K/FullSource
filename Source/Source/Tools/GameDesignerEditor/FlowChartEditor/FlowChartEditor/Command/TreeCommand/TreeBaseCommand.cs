using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Data;
using FlowChartEditor.Command;

namespace FlowChartEditor.Command.TreeCommand
{
    class TreeBaseCommand:Command
    {
        protected DocumentManager documentManager; // ���������
        protected TreeView tree; // ������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public TreeBaseCommand(object data, string description):base(data, description)
        {
            documentManager = DocumentManager.GetDocumentManager();
            tree = data as TreeView;
        }

        /// <summary>
        /// ��鵱ǰ·���Ƿ���Ч
        /// </summary>
        /// <param name="node">��ǰ�����</param>
        /// <param name="text">��ǰ�ı�</param>
        /// <returns>��ǰ·���Ƿ���Ч</returns>
        protected bool CheckPathValid(TreeNode node, string text)
        {
            bool valid = true;

            if (node != null)
            {
                foreach (TreeNode childNode in node.Nodes)
                {
                    if (childNode.Text == text)
                    {
                        valid = false;
                        break;
                    }
                }
            }
            else // �ڸ�����ϼ��
            {
                TreeView tree = data as TreeView;
                foreach(TreeNode childNode in tree.Nodes)
                {
                    if (childNode.Text == text)
                    {
                        valid = false;
                        break;
                    }
                }
            }

            return valid;
        }

        /// <summary>
        /// ��ȡ�ӽ������
        /// </summary>
        /// <param name="node">��ǰ�����</param>
        /// <returns>�ӽ������</returns>
        protected List<TreeNode> GetChildNodeList(TreeNode node)
        {
            List<TreeNode> nodeList = new List<TreeNode>();

            FillChildNodeInList(nodeList, node);

            return nodeList;
        }

        /// <summary>
        /// ���ӽ�������������
        /// </summary>
        /// <param name="nodeList">����</param>
        /// <param name="node">��ǰ�����</param>
        protected void FillChildNodeInList(List<TreeNode> nodeList, TreeNode node)
        {
            nodeList.Add(node);

            foreach(TreeNode treeNode in node.Nodes)
            {
                FillChildNodeInList(nodeList, treeNode);
            }
        }

        /// <summary>
        /// ������ִ��ǰ�����������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        protected override void  SaveBeforeExecute(object o)
        {
            dataBeforeExecute.SaveData(o);
        }

        /// <summary>
        /// ������ִ�к󱣴��������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        protected override void  SaveAfterExecute(object o)
        {
            dataAfterExecute.SaveData(o);
        }        

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        /// <return>�Ƿ�ִ�гɹ�</return>
        public override bool Execute(object o)
        {
            return false;
        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        public override void Execute()
        {
            object o = dataAfterExecute.LoadData();            
        }

        /// <summary>
        /// ȡ��ִ������
        /// </summary>
        /// <returns>��ǰ����</returns>
        public override void Unexecute()
        {
            object o = dataBeforeExecute.LoadData();            
        }
    }
}
