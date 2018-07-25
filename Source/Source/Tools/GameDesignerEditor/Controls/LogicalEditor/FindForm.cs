using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace LogicalEditor
{
    public partial class FindForm : Form
    {
        MWControlSuite.MWTreeView m_tree;
        ArrayList m_AllFoundNodes = new ArrayList();
        int m_CurIndex = -1;
        const int excludeFindTreeNodeLevel = 3; // ���Ҳ��������level�������

        public FindForm(MWControlSuite.MWTreeView tree)
        {
            InitializeComponent();
            m_tree = tree;
        }

        private void button2_Click(object sender, EventArgs e) // ����
        {
            string whatToFind = textBox1.Text;
            if (whatToFind == string.Empty) return;

            if (m_AllFoundNodes.Count == 0)
            {
                GenerateFindResult(whatToFind);
            }
            if (m_AllFoundNodes.Count == 0)
            {
                MessageBox.Show("û�ҵ����ϵļ�¼��");
                return;
            }

            m_CurIndex++;
            if (m_CurIndex >= m_AllFoundNodes.Count)
                m_CurIndex = 0;

            m_tree.SelectNode((TreeNode)(m_AllFoundNodes[m_CurIndex]), true);
            m_tree.Focus();
        }

        private void GenerateFindResult(string whatToFind)
        {
            foreach (TreeNode node in m_tree.Nodes)
            {
                GenerateFindResult_NodeText(node, whatToFind);
            }
        }

        private void GenerateFindResult_NodeText(TreeNode node, string whatToFind)
        {
            if (node.Level != excludeFindTreeNodeLevel && node.Text.Contains(whatToFind))
            {
                m_AllFoundNodes.Add(node);
            }
            foreach (TreeNode n in node.Nodes)
            {
                GenerateFindResult_NodeText(n, whatToFind);
            }
        }

        private void button1_Click(object sender, EventArgs e) // �ر�
        {
            this.Close();
        }

        private void textBox1_TextChanged(object sender, EventArgs e) // �ؼ������������ˡ�
        {
            m_AllFoundNodes.Clear();
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            //button2_Click(this.button2, null);
        }
    }
}