using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using DBManager;

namespace FlowChartEditor.Command.TreeCommand
{
    class TreeCutNodeCommand : TreeBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public TreeCutNodeCommand(object data, string description):
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
                TreeNode parentNode = currentNode.Parent;

                if (parentNode != null)
                {
                    documentManager.CutTreeNode = currentNode;
                    documentManager.CutTreeNodePath = documentManager.GetNodePath(currentNode);
                    description = "��������� " + currentNode.Text;
                    parentNode.Nodes.Remove(currentNode);
                    documentManager.CurrentTreeMode = EditMode.Cut;
                    success = true;
                }
            }

            return success;
        }
    }
}
