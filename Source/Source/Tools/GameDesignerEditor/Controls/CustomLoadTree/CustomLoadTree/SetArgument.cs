using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CustomLoadTree
{
    public partial class SetArgument : Form
    {
        private float maxFlow; // ʵ����������
        private float minFlow; // ʵ����������

        public SetArgument()
        {
            InitializeComponent();
        }

        /// <summary>
        /// ������������
        /// </summary>
        public float MaxFlow
        {
            get
            {
                return maxFlow;
            }
        }

        /// <summary>
        /// ������������
        /// </summary>
        public float MinFlow
        {
            get
            {
                return minFlow;
            }
        }

        /// <summary>
        /// ��������仯����
        /// </summary>
        public bool FontChangable
        {
            get
            {
                return checkBoxX1.Checked;
            }
        }

        /// <summary>
        /// ����������Чֵ
        /// </summary>
        /// <returns>�����Ƿ���Ч</returns>
        private bool CheckAvail()
        {
            string maxF = textBoxX1.Text;
            string minF = textBoxX2.Text;

            if(maxF == "") // ��������Ϊ��
            {
                MessageBox.Show("�������޲���Ϊ��!");
                return false;
            }

            if(minF == "") //��������Ϊ��
            {
                MessageBox.Show("�������޲���Ϊ��!");
                return false;
            }

            try
            {
                float maxFlow = float.Parse(maxF);
                float minFlow = float.Parse(minF);

                if(maxFlow < 0) // ��������Ϊ��
                {
                    MessageBox.Show("�������޲���Ϊ��!");
                    return false;
                }

                if(minFlow < 0) // ��������Ϊ��
                {
                    MessageBox.Show("�������޲���Ϊ��!");
                    return false;
                }

                if(maxFlow < minFlow) // ��������С������
                {
                    MessageBox.Show("�������޲���С������!");
                    return false;
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("���������������Чֵʱ�����쳣: " + ex.ToString());
                return false;
            }

            return true;
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if(CheckAvail()) // ����û��������Ч��
            {
                maxFlow = float.Parse(textBoxX1.Text);
                minFlow = float.Parse(textBoxX2.Text);
                DialogResult = DialogResult.OK;
                this.Close();
            }
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

        /// <summary>
        /// �û�ѡ����������仯
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void checkBoxX1_CheckedChanged(object sender, EventArgs e)
        {
            if(checkBoxX1.Checked)
            {
                MessageBox.Show("ע�⣬�Ե�Ԫ����������ÿ��ܻ����Ľ϶�Ĵ���ʱ�䣡");
            }
        }
    }
}