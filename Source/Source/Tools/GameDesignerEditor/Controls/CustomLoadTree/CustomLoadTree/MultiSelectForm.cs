using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace CustomLoadTree
{
    public partial class MultiSelectForm : Form
    {
        private List<string> itemList; // ��ʾ����������
        private List<string> selectItemList; // ѡ�е���������
        private bool selectAll = true; // �Ƿ�ȫ��ѡ��

        /// <summary>
        /// ���캯��
        /// <param name="title">����ı���</param>
        /// </summary>
        public MultiSelectForm(string title)
        {
            InitializeComponent();
            this.Text = title;
        }

        /// <summary>
        /// ��ʾ��������������
        /// </summary>
        public List<string> ItemList
        {
            set
            {
                itemList = value;
            }

            get
            {
                return itemList;
            }
        }

        /// <summary>
        /// ѡ�е�������������
        /// </summary>
        public List<string> SelectItemList
        {
            set
            {
                selectItemList = value;
            }
        }

        /// <summary>
        /// ��ʾ�ɹ�ѡ�������
        /// </summary>
        public void ShowItems()
        {
            if((selectItemList != null) && (selectItemList.Count != 0)) // �Զ���ѡ����ʾ�е�����
            {
                int index = 0;
                foreach(string s in itemList)
                {
                    checkedListBox1.Items.Add(s);
                    if(selectItemList.Contains(s))
                    {
                        checkedListBox1.SetItemChecked(index, true);
                    }
                    index++;
                }
            }
            else
            {
                foreach (string s in itemList)
                {
                    checkedListBox1.Items.Add(s);
                }
            }
            
        }

        /// <summary>
        /// ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (checkedListBox1.CheckedItems.Count > 0)
            {
                itemList.Clear();
                for (int i = 0; i < checkedListBox1.Items.Count; i++)
                {
                    if (checkedListBox1.GetItemChecked(i))
                    {
                        itemList.Add(checkedListBox1.Items[i].ToString());
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
        /// ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }

        /// <summary>
        /// ȫ��ѡ�л�ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
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
    }
}