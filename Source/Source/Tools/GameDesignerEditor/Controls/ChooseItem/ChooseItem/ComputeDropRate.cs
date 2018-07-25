using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ChooseItem
{
    public partial class ComputeDropRate : Form
    {
        private float finalDropRate = 0; // ���յ�����
        private int luckyRate; // ����Ȩֵ
        private int dropValue; // ����ֵ

        public ComputeDropRate()
        {
            InitializeComponent();
        }

        /// <summary>
        /// ���յ���������
        /// </summary>
        public float FinalDropRate
        {
            get
            {
                return finalDropRate;
            }
        }

        /// <summary>
        /// ����Ȩֵ����
        /// </summary>
        public int LuckyRate
        {
            get
            {
                return luckyRate;
            }

            set
            {
                luckyRate = value;
            }
        }

        /// <summary>
        /// ����ֵ����
        /// </summary>
        public int DropValue
        {
            get 
            {
                return dropValue;
            }

            set
            {
                dropValue = value;
            }
        }

        /// <summary>
        /// �������յ�����
        /// </summary>
        private void ComputeFinalDropRate()
        {
            int luckyValue = 0; // ����ֵ

            if(textBoxX1.Text == "")
            {
                MessageBox.Show("����ֵ����Ϊ��!");
                return;
            }

            try
            {
                luckyValue = int.Parse(textBoxX1.Text);
                // ���յ����� ��(����ֵ + (����Ȩֵ �� ����ֵ)) / ������ֵ
                finalDropRate = (dropValue + ((float)luckyRate * luckyValue)) / 1000000;
            }
            catch(Exception ex)
            {
                MessageBox.Show("�������յ�����ʱ�����쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// �������յ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            ComputeFinalDropRate();
            textBoxX2.Text = finalDropRate.ToString();
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