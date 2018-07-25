using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace GameDesingerTools
{
    public partial class FieldSearchForm : Form
    {
        TreeView treeView = null;
        TreeNode modelNode = null;
        TreeNode findNode = null;
        bool bLabel = false;
        TreeNode lastFindNode = null;

        public FieldSearchForm(TreeView treeView)
        {
            this.treeView = treeView;
            this.modelNode = treeView.SelectedNode;
            InitializeComponent();
        }

        /// <summary>
        /// ����ƥ��������
        /// </summary>
        /// <param name="currentNode">��ǰ�����</param>
        /// <param name="searchString">�����ַ���</param>
        private void SearchFiledNode(TreeNode currentNode, string searchString)
        {
            if (lastFindNode == null)
            {
                bLabel = true;
            }

            if (currentNode == lastFindNode)
            {
                bLabel = true;                
            }
            else
            {
                if (currentNode.Text.ToLower().Contains(searchString) && bLabel)
                {
                    findNode = currentNode;
                    lastFindNode = currentNode;
                }
                else
                {
                    foreach (TreeNode childNode in currentNode.Nodes)
                    {
                        SearchFiledNode(childNode, searchString);  
                      
                        if (findNode != null)
                        {
                            break;
                        }
                    }
                }
            }            
        }

        /// <summary>
        /// ������һ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string searchString = textBoxX1.Text.ToLower();

            if (searchString != "")
            {
                findNode = null;
                bLabel = false;

                foreach (TreeNode tabNode in modelNode.Nodes)
                {
                    foreach (TreeNode fieldNode in tabNode.Nodes)
                    {
                        SearchFiledNode(fieldNode, searchString);

                        if (findNode != null)
                        {
                            break;
                        }
                    }

                    if (findNode != null)
                    {
                        break;
                    }
                }

                if (findNode != null)
                {
                    treeView.SelectedNode = findNode;
                }
                else                
                {
                    MessageBox.Show("û���ҵ�ƥ�������㣬���ߵ�ǰ�����������һ��ƥ�������㣡", "�����ֶ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    lastFindNode = null;
                }
            }
            else
            {
                MessageBox.Show("�����������������", "�����ֶ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}