using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace GameDesingerTools
{
    public partial class CatForm : Form
    {
        private List<string> catList = new List<string>(); // ������Ϣ����
        private string catNames; // �����ַ���
        private bool beginSelect = false; // �Ƿ��Ѿ���ʼѡ��
        private string oldValue; // �ɵķ����ַ���
        private EditLog editLog; // ��ѯ�������

        public CatForm()
        {         
            InitializeComponent();           
        }

        /// <summary>
        /// ���ò���
        /// </summary>
        /// <param name="startPoint">��ʼλ��</param>
        /// <param name="oldValue">������ֵ</param>
        /// <param name="editLog">��ѯ�������</param>
        public void SetParameters(Point startPoint, string oldValue, EditLog editLog)
        {
            this.oldValue = oldValue;
            this.Location = startPoint;  
            this.editLog = editLog;
            catList.Clear();
        }

        /// <summary>
        /// ��ӷ����ַ���
        /// </summary>
        /// <param name="s">�����ַ���</param>
        public void AddData(string s)
        {
            if(!catList.Contains(s))
            {
                catList.Add(s);
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        public void InitData()
        {           
            treeView1.Nodes.Clear();
            treeView1.Nodes.Add("���з���");

            foreach(string s in catList)
            {
                string[] catString = s.Split(',');
                TreeNodeCollection currentNodes = treeView1.Nodes; // ��ǰ����㼯��
                TreeNode currentNode; // ��ǰ�����
                bool foundNode = false; // �Ƿ��Ѿ��ҵ������

                for (int i = catString.Length - 1; i >= 0; i--) // �������
                {
                    foundNode = false;
                    foreach (TreeNode node1 in currentNodes)
                    {
                        if (node1.Text == catString[i])
                        {
                            currentNode = node1;
                            currentNodes = node1.Nodes;
                            foundNode = true;
                            break;
                        }
                    }

                    if (!foundNode) // �������δ����
                    {
                        currentNode = currentNodes.Add(catString[i]);
                        currentNodes = currentNode.Nodes;
                    }
                }
            }

            ShowData();
        }

        /// <summary>
        /// ��ʾ��ǰ�ķ���
        /// </summary>
        private void ShowData()
        {
            string[] catString = oldValue.Split(',');
            TreeNodeCollection currentNodes = treeView1.Nodes; // ��ǰ����㼯��
            TreeNode currentNode = null; // ��ǰ�����
            bool foundNode = false; // �Ƿ��Ѿ��ҵ������

            for (int i = catString.Length - 1; i >= 0; i--) // �������
            {
                foundNode = false;
                foreach (TreeNode node1 in currentNodes)
                {
                    if (node1.Text == catString[i])
                    {
                        currentNode = node1;
                        currentNodes = node1.Nodes;
                        foundNode = true;
                        break;
                    }
                }

                if (!foundNode) // �������δ����
                {
                    break;
                }
            }

            if(currentNode != null) // ѡ�е�ǰ�����
            {
                treeView1.SelectedNode = currentNode;
            }
        }

        /// <summary>
        /// �û�ѡ�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if(beginSelect) // ���Ե�һ��ѡ��
            {
                TreeNode currentNode = treeView1.SelectedNode;

                if(currentNode != null) // �Ѿ�ѡ������� 
                {
                    string tempCatNames = currentNode.Text + ",";
                    TreeNode parentNode = currentNode.Parent;
                    while(parentNode != null)
                    {
                        tempCatNames += string.Format("{0},", parentNode.Text);
                        parentNode = parentNode.Parent;
                    }
                    catNames = tempCatNames.TrimEnd(',');
                    editLog.ResetCatName(catNames);
                    this.Visible = false;
                }  
            }
            else
            {
                beginSelect = true;
            }
        }

        /// <summary>
        /// �û�����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            TreeNode currentNode = treeView1.SelectedNode;
            if(currentNode == e.Node) // ��ǰ��ѡ��������޷��ٴ�ѡ��
            {
                // treeView1_AfterSelect(null, null); // ǿ��ѡ���Ѿ�ѡ�е������
            }
        }

        /// <summary>
        /// ���岻�ٻ�Ծ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void CatForm_Deactivate(object sender, EventArgs e)
        {
            this.Visible = false;
        }
    }
}