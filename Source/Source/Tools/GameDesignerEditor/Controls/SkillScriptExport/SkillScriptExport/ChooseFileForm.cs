using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DevComponents.DotNetBar.Controls;

namespace SkillScriptExport
{
    public partial class ChooseFileForm : Form
    {
        private bool selectAll = true; // �Ƿ�ȫ��ѡ��
        private List<string> fileNameList; // ��ǰ��ѡ�ļ�������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="fileNameList">�ļ�������</param>
        public ChooseFileForm(List<string> fileNameList)
        {
            this.fileNameList = fileNameList;
            InitializeComponent();
            InitData();
        }

        /// <summary>
        /// �ļ�������
        /// </summary>
        public List<string> FileNameList
        {
            set
            {
                fileNameList = value;
            }

            get
            {
                return fileNameList;
            }
        }

        /// <summary>
        /// ��ʼ����ѡ���ļ���
        /// </summary>
        private void InitData()
        {
            foreach (string s in fileNameList)
            {
                checkedListBox1.Items.Add(s);
            }
        }

        /// <summary>
        /// �û�ѡ��ȫ��ѡ��
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            if (selectAll) // ȫ��ѡ��
            {
                for (int i = 0; i < checkedListBox1.Items.Count; i++)
                {
                    checkedListBox1.SetItemChecked(i, true);
                }
                selectAll = false;
                buttonX3.Text = "ȫ��ȡ��";
            }
            else // ȫ��ȡ��
            {
                for (int i = 0; i < checkedListBox1.Items.Count; i++)
                {
                    checkedListBox1.SetItemChecked(i, false);
                }

                selectAll = true;
                buttonX3.Text = "ȫ��ѡ��";
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (checkedListBox1.CheckedItems.Count > 0)
            {
                fileNameList.Clear();
                for (int i = 0; i < checkedListBox1.Items.Count; i++)
                {
                    if (checkedListBox1.GetItemChecked(i))
                    {
                        fileNameList.Add(checkedListBox1.Items[i].ToString());
                    }
                }

                DialogResult = DialogResult.OK;
            }
            else
            {
                DialogResult = DialogResult.No;
            }
            this.Close();
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
    }
}