using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using DevComponents.DotNetBar;

namespace FlowChartEditor
{
    public partial class InterfaceNodeForm : Office2007Form
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="interfaceName">�ӿ�����</param>
        /// <param name="isInput">�Ƿ�����������</param>
        public InterfaceNodeForm(string interfaceName, bool isInput)
        {
            InitializeComponent();

            // ��ʼ���ؼ���ʾ
            nameBox.Text = interfaceName;
            bInput.Checked = isInput;
            bOutput.Checked = !isInput;
        }        

        /// <summary>
        /// �ӿ�����
        /// </summary>
        public string InterfaceName
        {
            get
            {
                return nameBox.Text;
            }
        }

        /// <summary>
        /// �Ƿ�����������
        /// </summary>
        public bool IsInput
        {
            get
            {
                return bInput.Checked;
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {            
            if (nameBox.Text != "" && (bInput.Checked || bOutput.Checked)) // ������Ч
            {
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("��������Ϊ�գ�", "�ӿڱ༭",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼����ͺ���</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}