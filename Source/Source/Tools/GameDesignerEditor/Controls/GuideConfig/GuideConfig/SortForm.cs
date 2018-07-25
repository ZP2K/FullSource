using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using DevComponents.DotNetBar;

namespace GuideConfig
{
    public partial class SortForm : Office2007Form
    {
        private List<string> recordList; // ��¼����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="recordList">��¼����</param>
        public SortForm(List<string> recordList)
        {
            this.recordList = recordList;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��¼����
        /// </summary>
        public List<string> RecordList
        {
            get
            {
                return recordList;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            foreach (string recordName in recordList)
            {
                recordListBox.Items.Add(recordName);
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bOK_Click(object sender, EventArgs e)
        {
            recordList = new List<string>();

            foreach (string recordName in recordListBox.Items)
            {
                recordList.Add(recordName);
            }

            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// ���Ƽ�¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bMoveUp_Click(object sender, EventArgs e)
        {
            int index = recordListBox.SelectedIndex;

            if (index != -1)
            {
                if (index > 0)
                {
                    string itemInfo = recordListBox.Items[index] as string;
                    recordListBox.Items.RemoveAt(index);
                    recordListBox.Items.Insert(index - 1, itemInfo);
                    recordListBox.SelectedIndex = index - 1;
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ���Ƶļ�¼��", "���Ƽ�¼",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ���Ƽ�¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bMoveDown_Click(object sender, EventArgs e)
        {
            int index = recordListBox.SelectedIndex;

            if (index != -1)
            {
                if (index < recordListBox.Items.Count - 1)
                {
                    string itemInfo = recordListBox.Items[index] as string;
                    recordListBox.Items.RemoveAt(index);
                    recordListBox.Items.Insert(index + 1, itemInfo);
                    recordListBox.SelectedIndex = index + 1;
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ���Ƶļ�¼��", "���Ƽ�¼",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}