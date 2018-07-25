using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace GameDesingerTools
{
    public partial class ExportFieldConfigForm : Form
    {
        private List<string> fieldList;
        private string excludeFieldString;
        private bool selectAll = true;

        public ExportFieldConfigForm(List<string> fieldList, string excludeFieldString)
        {
            this.fieldList = fieldList;
            this.excludeFieldString = excludeFieldString;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ���������ֶε������ַ���
        /// </summary>
        public string ExcludeFieldString
        {
            get
            {
                return excludeFieldString;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            List<string> excludeFieldList = new List<string>();

            if (excludeFieldString != "")
            {
                string[] tempArray = excludeFieldString.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);            
                excludeFieldList.AddRange(tempArray);
            }
            

            for (int i = 0; i < fieldList.Count; i++)
            {
                checkedListBox1.Items.Add(fieldList[i]);

                if (excludeFieldList.Contains(fieldList[i]))
                {
                    checkedListBox1.SetItemChecked(i, true);
                }
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            StringBuilder configString = new StringBuilder();

            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                if (checkedListBox1.GetItemChecked(i))
                {
                    configString.Append(string.Format("{0},", checkedListBox1.Items[i].ToString()));
                }
            }

            excludeFieldString = configString.ToString().TrimEnd(new char[] { ',' });

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
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// ȫ��ѡ��/ȫ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {            
            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                checkedListBox1.SetItemChecked(i, selectAll);
            }

            selectAll = !selectAll;

            if (selectAll)
            {               
                buttonX3.Text = "ȫ��ѡ��";
            }
            else
            {
                buttonX3.Text = "ȫ��ȡ��";
            }
        }
    }
}