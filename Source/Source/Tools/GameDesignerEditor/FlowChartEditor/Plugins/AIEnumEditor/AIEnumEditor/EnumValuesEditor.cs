using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace AIEnumEditor
{
    public partial class EnumValuesEditor : Form
    {
        private string oldValue; // ��ֵ
        private string newValue; // ��ֵ

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="oldValue">��ֵ</param>
        public EnumValuesEditor(string oldValue)
        {
            this.oldValue = oldValue;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ֵ
        /// </summary>
        public string NewValue
        {
            get
            {
                return newValue;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ʼ��������
            DataGridViewTextBoxColumn chineseNameColumn = new DataGridViewTextBoxColumn();
            chineseNameColumn.Name = "ChineseName";
            chineseNameColumn.HeaderText = "������";
            DataGridViewTextBoxColumn englishNameColumn = new DataGridViewTextBoxColumn();
            englishNameColumn.Name = "EnglishName";
            englishNameColumn.HeaderText = "Ӣ����";
            DataGridViewTextBoxColumn valueColumn = new DataGridViewTextBoxColumn();
            valueColumn.Name = "Value";
            valueColumn.HeaderText = "��ֵ";

            dataGridViewX1.Columns.Add(chineseNameColumn);
            dataGridViewX1.Columns.Add(englishNameColumn);
            dataGridViewX1.Columns.Add(valueColumn);

            // ��ʼ��������
            if(oldValue != null && oldValue != "")
            {
                string[] datas = oldValue.Split(new char[] { '\n' });
                int index = 0;

                foreach(string s in datas)
                {
                    string value = s.TrimEnd(new char[] { '\r' });
                    string[] values = value.Split(new char[] { ',' });
                    dataGridViewX1.Rows.Add(1);
                    DataGridViewRow newRow = dataGridViewX1.Rows[index];
                    newRow.Cells["ChineseName"].Value = values[0];
                    newRow.Cells["EnglishName"].Value = values[1];
                    newRow.Cells["Value"].Value = values[2];
                    index++;
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
            string value = "";
            bool valid = true;

            foreach(DataGridViewRow row in dataGridViewX1.Rows)
            {
                if(!row.IsNewRow) // �����½���������
                {
                    string chineseName = row.Cells["ChineseName"].EditedFormattedValue as string;
                    string englishName = row.Cells["EnglishName"].EditedFormattedValue as string;
                    string values = row.Cells["Value"].EditedFormattedValue as string;

                    if (chineseName == null || chineseName == "" || englishName == null || englishName == "" ||
                        values == null || values == "")
                    {
                        MessageBox.Show("�����������Ĳ�����", "ö��ֵ�б�༭",
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                        valid = false;
                        break;
                    }

                    value += string.Format("{0},{1},{2}\r\n", chineseName, englishName, values);
                }
                
            }

            if(valid) // ������Ч
            {
                newValue = value.TrimEnd(new char[] { '\r', '\n' });
                DialogResult = DialogResult.OK;
                this.Close();
            }                        
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