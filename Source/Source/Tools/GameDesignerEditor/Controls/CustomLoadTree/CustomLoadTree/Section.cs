using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CustomLoadTree
{
    public partial class Section : Form
    {
        float[] data; // ��������������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">���ص���������������</param>
        public Section(float[] data)
        {
            this.data = data;
            InitializeComponent();
        }

        /// <summary>
        /// ����û��������Ч��
        /// </summary>
        /// <returns>�û������Ƿ���Ч</returns>
        private bool ChechAvail()
        {
            string text1 = textBoxX1.Text;
            string text2 = textBoxX2.Text;
            float upValue = 0.00f; // �Ͻ�ֵ
            float downValue = 0.00f; // �½�ֵ

            if(text1 == "" || text2 == "") // ����Ϊ��
            {
                MessageBox.Show("���벻��Ϊ��!");
                return false;
            }

            try // �����ֵ�Ƿ���Ч
            {
                upValue = float.Parse(text1);
                downValue = float.Parse(text2);
            }
            catch(Exception ex)
            {
                MessageBox.Show("�������ֵ��Ч: " + ex.ToString());
                return false;
            }

            if(upValue < downValue) // �Ͻ�С���½�
            {
                MessageBox.Show("�Ͻ�ֵ����С���½�ֵ!");
                return false;
            }

            data[0] = upValue;
            data[1] = downValue;
            return true;
        }

        /// <summary>
        /// ȷ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if(ChechAvail()) // ����������Ч��
            {
                DialogResult = DialogResult.OK;
                this.Close();
            }      
        }

        /// <summary>
        /// ȡ������
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