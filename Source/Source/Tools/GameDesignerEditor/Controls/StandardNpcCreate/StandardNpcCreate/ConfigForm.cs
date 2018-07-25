using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace StandardNpcCreate
{
    public partial class ConfigForm : Form
    {
        public ConfigForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// �Ƿ���²��
        /// </summary>
        public bool UpdateSlot
        {
            get
            {
                return checkBoxX3.Checked;
            }
        }

        /// <summary>
        /// �Ƿ���¶���
        /// </summary>
        public bool UpdateAnimation
        {
            get
            {
                return checkBoxX1.Checked;
            }
        }

        /// <summary>
        /// �Ƿ���¼���
        /// </summary>
        public bool UpdateSkill
        {
            get
            {
                return checkBoxX2.Checked;
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

        /// <summary>
        /// ��ѡ���¶���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void checkBoxX1_CheckedChanged(object sender, EventArgs e)
        {
            if (!checkBoxX1.Checked)
            {
                checkBoxX2.Checked = false;
                checkBoxX2.Enabled = false;
            }
            else
            {
                checkBoxX2.Enabled = true;
            }
        }
    }
}