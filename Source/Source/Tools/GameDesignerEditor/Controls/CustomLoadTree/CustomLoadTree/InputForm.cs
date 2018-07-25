using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CustomLoadTree
{
    public partial class InputForm : Form
    {
        private string titleText; // �����ı�
        private string inputText; // �����ı�

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="titleText">�����ı�</param>
        /// <param name="inputText">�����ı�</param>
        public InputForm(string titleText, string inputText)
        {
            this.titleText = titleText;
            this.inputText = inputText;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ������ı�
        /// </summary>
        public string InputText
        {
            get
            {
                return inputText;
            }
        }

        /// <summary>
        /// ��ʼ��
        /// </summary>
        private void Init()
        {
            this.Text = titleText;
            textBoxX1.Text = inputText;
            textBoxX1.SelectionStart = 0;
            textBoxX1.SelectionLength = inputText.Length;
            textBoxX1.Focus();
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            inputText = textBoxX1.Text;
            if (inputText != "")
            {
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("���벻��Ϊ�գ�", "�ı�����", MessageBoxButtons.OK, MessageBoxIcon.Information);
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