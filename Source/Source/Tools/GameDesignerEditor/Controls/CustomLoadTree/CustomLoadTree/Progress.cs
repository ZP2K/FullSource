using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CustomLoadTree
{
    /// <summary>
    /// ��������ʾ����
    /// </summary>
    public partial class Progress : Form
    {
        private int maxValue = 0; // �����������ֵ
        private string titleText = "����������";

        /// <summary>
        /// ���캯��
        /// </summary>
        public Progress()
        {
            InitializeComponent();
            this.StartPosition = FormStartPosition.CenterScreen; // ����������Ϊ��Ļ����λ��
        }

        /// <summary>
        /// �����ı�����
        /// </summary>
        public string TitleText
        {
            get
            {
                return titleText;
            }

            set
            {
                titleText = value;
            }
        }

        /// <summary>
        /// ���ý����������ֵ
        /// </summary>
        /// <param name="n">���ֵ</param>
        public void SetMaxValue(int n)
        {
            maxValue = n;
            progressBarX1.Minimum = 0;
            progressBarX1.Maximum = n;
        }

        /// <summary>
        /// ���ý������ĵ�ǰֵ
        /// </summary>
        /// <param name="n">��ǰֵ</param>
        public void SetValue(int n)
        {
            this.Text = string.Format("{2} {0}/{1}", n, maxValue, titleText);
            progressBarX1.Value = n;
            if (maxValue == n)
            {
                this.Close();
            }
        }

        /// <summary>
        /// �رմ����¼�����
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Progress_FormClosing(object sender, FormClosingEventArgs e)
        {
            // ���ش���
            this.Visible = false;
            e.Cancel = true;
        }
    }
}