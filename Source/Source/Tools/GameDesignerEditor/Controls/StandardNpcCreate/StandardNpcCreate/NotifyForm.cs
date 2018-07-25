using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace StandardNpcCreate
{
    public partial class NotifyForm : Form
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        public NotifyForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// �Ƿ�������ж���
        /// </summary>
        public bool UpdateExistAnimation
        {
            get
            {
                return checkBoxX2.Checked;
            }
        }

        /// <summary>
        /// �Ƿ��¼��ǰѡ��
        /// </summary>
        public bool RecordSelection
        {
            get
            {
                return checkBoxX1.Checked;
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {            
            DialogResult = DialogResult.OK;
            this.Close();
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