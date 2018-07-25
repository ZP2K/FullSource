using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CustomLoadTree
{
    public partial class Selector : Form
    {
        float[] data; // ��������
        private List<string> selectionList = new List<string>(); // ѡ���б�
        private List<KeyValuePair<string, string>> valueList = new List<KeyValuePair<string,string>>(); // ѡ���ֵ���б�

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��������</param>
        public Selector(float[] data)
        {
            this.data = data;
            InitializeComponent();
            FillList();
        }

        /// <summary>
        /// �������
        /// </summary>
        private void FillList()
        {
            selectionList.Add("��ͨ");
            selectionList.Add("�߼�");
            selectionList.Add("��Ӣ");
            valueList.Add(new KeyValuePair<string, string>("��ͨ", "1.00"));
            valueList.Add(new KeyValuePair<string, string>("�߼�", "1.30"));
            valueList.Add(new KeyValuePair<string, string>("��Ӣ", "1.50"));            
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string value = comboBoxEx1.Text;

            if(value == "") // ����Ϊ��
            {
                MessageBox.Show("���벻��Ϊ��!");
                return;
            }

            if(selectionList.Contains(value)) // ������ѡ���е�����
            {
                foreach(KeyValuePair<string, string> k in valueList)
                {
                    if(value == k.Key)
                    {
                        value = k.Value;
                    }
                }
            }

            try
            {
                float result = float.Parse(value);
                data[0] = result;
                DialogResult = DialogResult.OK;
                this.Close();
            }
            catch(Exception ex)
            {
                MessageBox.Show("����ֵ��Ч: " + ex.ToString());
            }
        }
    }
}