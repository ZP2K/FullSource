using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace GameDesingerTools
{
    public partial class FiltrateForm : Form
    {
        TreeView baseTree; // ������
        List<TreeNode> treeNodeList = new List<TreeNode>(); // Ҷ�����������
        List<TreeNode> baseNodeList = new List<TreeNode>(); // BaseFormҶ�����������

        public FiltrateForm(TreeView baseTree)
        {
            this.baseTree = baseTree;
            InitializeComponent();
            ReconstructTree(baseTree);
        }

        /// <summary>
        /// ���¹�����
        /// </summary>
        /// <param name="tree">�ɵ���</param>
        private void ReconstructTree(TreeView tree)
        {
            treeView1.Nodes.Clear();
            treeNodeList.Clear();
            foreach(TreeNode node in tree.Nodes)
            {
                TreeNode newNode = new TreeNode(node.Text);
                treeView1.Nodes.Add(newNode);
                ReconstructTreeNode(node, newNode);
            }
        }

        /// <summary>
        /// ���¹��������
        /// </summary>
        /// <param name="oldNode">�ɵ������</param>
        /// <param name="newNode">�µ������</param>
        private void ReconstructTreeNode(TreeNode oldNode, TreeNode newNode)
        {
            foreach(TreeNode node in oldNode.Nodes)
            {
                TreeNode freshNode = new TreeNode(node.Text);
                newNode.Nodes.Add(freshNode);
                ReconstructTreeNode(node, freshNode);
            }

            if(oldNode.Nodes.Count == 0) // �����Ҷ�ӽ��Ļ���¼������������
            {
                treeNodeList.Add(newNode);
                baseNodeList.Add(oldNode);
            }
        }

        /// <summary>
        /// ���˳����������������
        /// </summary>
        /// <param name="text">�ؼ���</param>
        private void FiltrateTreeNode(string text)
        {
            int count = 0;

            if(text == "") // �ؼ���Ϊ�յĻ�����ʾȫ�������
            {
                ReconstructTree(baseTree);       
            }
            else
            {
                List<TreeNode> nodeList = new List<TreeNode>();
                TreeNode parentNode;
                TreeNode tempNode;
                TreeNodeCollection currentNodes;

                treeView1.Nodes.Clear();

                foreach(TreeNode node in treeNodeList)
                {
                    if(node.Text.Contains(text))
                    {
                        nodeList.Clear();
                        parentNode = node;
                        nodeList.Add(node);

                        while(parentNode.Parent != null)
                        {
                            parentNode = parentNode.Parent;
                            nodeList.Add(parentNode);
                        }

                        currentNodes = treeView1.Nodes;
                        for(int i = nodeList.Count - 1; i >= 0; i--)
                        {
                            tempNode = nodeList[i];
                            bool find = false;

                            foreach(TreeNode currentNode in currentNodes)
                            {
                                if(currentNode.Text == tempNode.Text)
                                {
                                    currentNodes = currentNode.Nodes;
                                    find = true;
                                    break;
                                }
                            }

                            if(!find)
                            {
                                TreeNode newNode = new TreeNode(tempNode.Text);
                                currentNodes.Add(newNode);
                                currentNodes = newNode.Nodes;
                            }
                        }

                        count++;
                    }
                }

                this.Text = string.Format("�ؼ��ֹ��� �����ҵ�{0}����¼��", count.ToString());
            }   

            treeView1.ExpandAll();
        }

        /// <summary>
        /// ѡ�������
        /// </summary>
        /// <return>�Ƿ�ѡ��ɹ�</return>
        private bool SelectTreeNode()
        {
            bool result = false;
            TreeNode selectedNode = treeView1.SelectedNode;

            if(selectedNode != null)
            {
                bool find = true;
                TreeNode parentNode1;
                TreeNode parentNode2;

                foreach(TreeNode node in baseNodeList)
                {
                    find = true;

                    if(node.Text == selectedNode.Text)
                    {
                        parentNode1 = node.Parent;
                        parentNode2 = selectedNode.Parent;

                        while(parentNode1 != null && parentNode2 != null)
                        {
                            if(parentNode1.Text != parentNode2.Text)
                            {
                                find = false;
                                break;
                            }

                            parentNode1 = parentNode1.Parent;
                            parentNode2 = parentNode2.Parent;
                        }

                        if(find)
                        {
                            baseTree.SelectedNode = node;
                            result = true;
                            break;
                        }
                    }
                }
            }
            else
            {
                MessageBox.Show("δѡ�������!", "�ؼ��ֹ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return result;
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            bool result = SelectTreeNode();
            if(result)
            {
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("��Ч��ѡ��!", "�ؼ��ֹ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �û�ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }

        /// <summary>
        /// �ؼ��ָı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void textBoxX1_TextChanged(object sender, EventArgs e)
        {
            FiltrateTreeNode(textBoxX1.Text);
        }

        /// <summary>
        /// ��������˿�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void textBoxX1_MouseClick(object sender, MouseEventArgs e)
        {
            if(textBoxX1.Text == "����ؼ���...")
            {
                textBoxX1.Text = "";
            }
        }
    }
}