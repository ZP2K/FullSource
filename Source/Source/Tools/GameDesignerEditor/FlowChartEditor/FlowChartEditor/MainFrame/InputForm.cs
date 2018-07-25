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
    public partial class InputForm : Office2007Form
    {
        private string inputText; // ������ı�

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="title">����ı���</param>
        /// <param name="inputText">������ı�</param>
        public InputForm(string title, string inputText)
        {
            this.inputText = inputText;
            InitializeComponent();
            this.Text = title;
            textBoxX1.Text = inputText;
            textBoxX1.Select(0, inputText.Length);
        }

        /// <summary>
        /// ������ı�����
        /// </summary>
        public string InputText
        {
            get
            {
                return inputText;
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            inputText = textBoxX1.Text;

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
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}