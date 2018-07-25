using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace GameDesingerTools.DBTools
{
    public partial class TypeForm : Form
    {
        private string typeName; // ��������

        public TypeForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// ������������
        /// </summary>
        public string TypeName 
        {
            get
            {
                return typeName;
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if(textBoxX1.Text == "")
            {
                MessageBox.Show("�������Ʋ���Ϊ��!");
                return;
            }
            typeName = textBoxX1.Text;
            DialogResult = DialogResult.OK;
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