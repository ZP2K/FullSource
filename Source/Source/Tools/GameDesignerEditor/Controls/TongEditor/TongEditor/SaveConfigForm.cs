using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DevComponents.DotNetBar;

namespace ToneEditor
{
    public partial class SaveConfigForm : Office2007Form
    {
        private string tongTemplateFolder; // TongTemplate.dat�ļ������Ŀ¼
        private string tongOperationFolder; // TongOperation.tab�ļ������Ŀ¼        

        /// <summary>
        /// ���캯��
        /// </summary>
        public SaveConfigForm()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// TongTemplate.dat�ļ������Ŀ¼
        /// </summary>
        public string TongTemplateFolder
        {
            get
            {
                return tongTemplateFolder;
            }
        }

        /// <summary>
        /// TongOperation.tab�ļ������Ŀ¼
        /// </summary>
        public string TongOperationFolder
        {
            get
            {
                return tongOperationFolder;
            }
        }

        /// <summary>
        /// �Ƿ񱣴�TongTemplate.dat�ļ�
        /// </summary>
        public bool SaveTongTemplateFile
        {
            get
            {
                return checkBoxX1.Checked;
            }
        }

        /// <summary>
        /// �Ƿ񱣴�TongOperation.tab�ļ�
        /// </summary>
        public bool SaveTongOperationFile
        {
            get
            {
                return checkBoxX2.Checked;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            tongTemplateFolder = Application.StartupPath;
            tongOperationFolder = Application.StartupPath;

            textBoxX1.Text = tongTemplateFolder;
            textBoxX2.Text = tongOperationFolder;
        }

        /// <summary>
        /// ��ѡ/ȡ������TongTemplate.dat�ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void checkBoxX1_CheckedChanged(object sender, EventArgs e)
        {
            textBoxX1.Enabled = checkBoxX1.Checked;
            buttonX3.Enabled = checkBoxX1.Checked;
        }

        /// <summary>
        /// ��ѡ/ȡ������TongOperation.tab�ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void checkBoxX2_CheckedChanged(object sender, EventArgs e)
        {
            textBoxX2.Enabled = checkBoxX2.Checked;
            buttonX4.Enabled = checkBoxX2.Checked;
        }

        /// <summary>
        /// ����ļ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBroserDialog = new FolderBrowserDialog();                        
            folderBroserDialog.SelectedPath = tongTemplateFolder;            
            folderBroserDialog.Description = "��ѡ���ļ������Ŀ¼";

            if (folderBroserDialog.ShowDialog() == DialogResult.OK)
            {
                tongTemplateFolder = folderBroserDialog.SelectedPath;
                textBoxX1.Text = tongTemplateFolder;
            }
        }

        /// <summary>
        /// ����ļ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBroserDialog = new FolderBrowserDialog();
            folderBroserDialog.SelectedPath = tongOperationFolder;
            folderBroserDialog.Description = "��ѡ���ļ������Ŀ¼";

            if (folderBroserDialog.ShowDialog() == DialogResult.OK)
            {
                tongOperationFolder = folderBroserDialog.SelectedPath;
                textBoxX1.Text = tongOperationFolder;
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (checkBoxX1.Checked && textBoxX1.Text == "")
            {
                MessageBox.Show("TongTemplate.dat�ļ�����Ŀ¼����Ϊ�գ�", "�ļ���������", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
                return;
            }

            if (checkBoxX2.Checked && textBoxX2.Text == "")
            {
                MessageBox.Show("TongOperation.tab�ļ�����Ŀ¼����Ϊ�գ�", "�ļ���������", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
                return;
            }

            tongTemplateFolder = textBoxX1.Text;
            tongOperationFolder = textBoxX2.Text;
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}