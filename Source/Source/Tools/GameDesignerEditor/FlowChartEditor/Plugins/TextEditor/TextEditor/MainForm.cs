using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using DevComponents.DotNetBar;

namespace TextEditor
{
    public partial class MainForm : Office2007Form
    {
        private string content; // �ı�����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="content">�ı�����</param>
        public MainForm(string content)
        {
            this.content = content;
            InitializeComponent();
            textBoxX1.Text = content;
            textBoxX1.Select(0, content.Length);
        }

        /// <summary>
        /// �ı�����
        /// </summary>
        public string Content
        {
            get
            {
                return content;
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            content = textBoxX1.Text;

            if(content != "")
            {                
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("��������ݲ���Ϊ�գ�", "������", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
        /// ������ı���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void textBoxX1_MouseClick(object sender, MouseEventArgs e)
        {
            if (textBoxX1.Text == "������༭����") // �Զ�����ı�����
            {
                textBoxX1.Text = "";
            }
        }
    }
}