using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DesignerSceneEditor
{
	public partial class InputForm: Form
	{
        private string inputText;

		public InputForm(string inputText, string titleText)
		{
			InitializeComponent();
            textBoxX1.Text = inputText;
            textBoxX1.SelectAll();
            this.Text = titleText;
            textBoxX1.Focus();
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
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
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
                MessageBox.Show("���벻��Ϊ�գ�", "�ı�����",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
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