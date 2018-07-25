using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using DevComponents.DotNetBar;

namespace FlowChartEditor
{
    public partial class SettingForm : Office2007Form
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        public SettingForm()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            Helper helper = Helper.GetHelper();
            textBoxX1.Text = helper.OutputDir;
        }

        /// <summary>
        /// ����ļ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog openFolderDialog = new FolderBrowserDialog();
            openFolderDialog.Description = "��ѡ��ͻ������ڵ�Ŀ¼";
            openFolderDialog.ShowNewFolderButton = true;
            openFolderDialog.RootFolder = Environment.SpecialFolder.MyComputer;

            if (openFolderDialog.ShowDialog() == DialogResult.OK)
            {
                textBoxX1.Text = openFolderDialog.SelectedPath;
            }
        }

        /// <summary>
        /// ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            string outputDir = textBoxX1.Text;
            if (outputDir != "")
            {
                DirectoryInfo di = new DirectoryInfo(outputDir);

                if(di.Exists) // �ͻ���Ŀ¼����
                {
                    Helper helper = Helper.GetHelper();
                    helper.OutputDir = outputDir;

                    DialogResult = DialogResult.OK;
                    this.Close();
                }
                else
                {
                    MessageBox.Show("���·����Ч��", "��������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }                
            }
            else
            {
                MessageBox.Show("���·������Ϊ�գ�", "��������", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }            
        }

        /// <summary>
        /// ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}