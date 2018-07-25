using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ShopItemIO
{
    public partial class ProgressForm : Form
    {
        private int minValue; // ��������Сֵ
        private int maxValue; // ���������ֵ

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="minValue">��������Сֵ</param>
        /// <param name="maxValue">���������ֵ</param>
        public ProgressForm(int minValue, int maxValue)
        {
            this.minValue = minValue;
            this.maxValue = maxValue;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            progressBarX1.Minimum = minValue;
            progressBarX1.Maximum = maxValue;
        }

        /// <summary>
        /// ��ʾ����
        /// </summary>
        /// <param name="progressValue">����ֵ</param>
        /// <param name="progressText">�����ı�</param>
        public void ShowProgress(int progressValue, string progressText)
        {
            progressBarX1.Value = progressValue;
            labelX1.Text = progressText;
            Application.DoEvents();

            if (progressValue >= maxValue)
            {
                this.Close();
            }
        }
    }
}