using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace FlowChartEditor
{
    public partial class TextForm : Form
    {
        string folder; // �����ļ���Ŀ¼

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="folder">�����ļ���Ŀ¼</param>
        /// <param name="title">�������</param>
        /// <param name="content">�������</param>
        public TextForm(string folder, string title, string content)
        {
            this.folder = folder;
            InitializeComponent();
            this.Text = title;
            textBoxX1.Text = content;
        }

        /// <summary>
        /// �رմ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// �����ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            string fileName = string.Format("{0}\\{1}\\{2}.txt", System.Environment.CurrentDirectory, folder,
                string.Format("{0}-{1}-{2} {3}-{4}-{5}", DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day,
                DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second));
            bool success = Helper.GetHelper().SaveTextToFile(fileName, textBoxX1.Text);
            if(success)
            {
                MessageBox.Show(string.Format("�ļ�{0}����ɹ���", fileName), "�ļ�����", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }
        }
    }
}