using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CoordinateEditor
{
    public partial class CoordinateForm : Form
    {
        string cordinate; // ����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="cordinate">��ǰ����</param>
        public CoordinateForm(string cordinate)
        {
            this.cordinate = cordinate;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ǰ����
        /// </summary>
        public string Cordinate
        {
            set
            {
                cordinate = value;
            }

            get
            {
                return cordinate;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            if(cordinate != null && cordinate != "") // ���벻Ϊ��
            {
                string[] data = cordinate.Split(new char[] { ',' });
                integerInput1.Value = int.Parse(data[0]);
                integerInput2.Value = int.Parse(data[1]);
                integerInput3.Value = int.Parse(data[2]);
            }
        }

        /// <summary>
        /// ȷ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            int x = integerInput1.Value;
            int y = integerInput2.Value;
            int z = integerInput3.Value;            
            cordinate = string.Format("{0},{1},{2}", x, y, z);
            DialogResult = DialogResult.OK;
        }

        /// <summary>
        /// ȡ������
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