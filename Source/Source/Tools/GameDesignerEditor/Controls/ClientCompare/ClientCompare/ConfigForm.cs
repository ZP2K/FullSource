using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using DevComponents.DotNetBar;

namespace ClientCompare
{
    public partial class ConfigForm : Office2007Form
    {
        public ConfigForm(string filePath)
        {
            InitializeComponent();
            textBoxX1.Text = filePath;
        }

        public string FilePath
        {
            get
            {
                return textBoxX1.Text;
            }
        }

        public bool CompareTabFile
        {
            get
            {
                return tabCheckBox.Checked;
            }
        }

        public bool CompareTxtFile
        {
            get
            {
                return txtCheckBox.Checked;
            }
        }

        public bool CompareIniFile
        {
            get
            {
                return iniCheckBox.Checked;
            }
        }

        public bool CompareLogicalFile
        {
            get
            {
                return logicalCheckBox.Checked;
            }
        }

        public bool CompareOtherFile
        {
            get
            {
                return otherCheckBox.Checked;
            }
        }

        public bool CompareLuaFile
        {
            get
            {
                return luaCheckBox.Checked;
            }
        }

        public bool CompareLSFile
        {
            get
            {
                return lsCheckBox.Checked;
            }
        }

        public bool CompareLHFile
        {
            get
            {
                return lhCheckBox.Checked;
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {      
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

        /// <summary>
        /// ����ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Title = "��ѡ��ȶԳ����ļ�";
            
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                textBoxX1.Text = openFileDialog.FileName;
            }
        }
    }
}