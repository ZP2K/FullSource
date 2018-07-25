using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace FlowChartEditor.Command.TreeCommand
{
    class TreeCopyCommand : TreeBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public TreeCopyCommand(object data, string description):
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
                description = "��������� " + currentNode.Text;              
                documentManager.CopyTreeNode = currentNode;
                documentManager.CurrentTreeMode = EditMode.Copy;               
                success = true;
            }

            return success;
        }
    }
}
