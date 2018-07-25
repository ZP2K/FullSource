using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using DevComponents.DotNetBar;

namespace FlowChartEditor
{
    public partial class ListForm : Office2007Form
    {
        private List<string> inputList; // �����ַ���������
        private string inputText; // ������ַ���

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="title">����ı���</param>
        /// <param name="inputList">�����ַ���������</param>
        public ListForm(string title, List<string> inputList)
        {
            this.inputList = inputList;
            InitializeComponent();
            this.Text = title;
            Init();
        }

        /// <summary>
        /// ������ַ�������
        /// </summary>
        public string InputText
        {
            set
            {
                inputText = value;
            }

            get
            {
                return inputText;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            foreach(string s in inputList)
            {
                comboBoxEx1.Items.Add(s);
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            inputText = comboBoxEx1.Text;
            if(inputText != "")
            {
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("���벻��Ϊ�գ�", "������Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �û�ѡ��ȡ��
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