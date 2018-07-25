using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using DevComponents.DotNetBar;

namespace GuideConfig
{
    public partial class AddNodeForm : Office2007Form
    {
        private string nodeTitle;
        private string nodeName;
        private string nodeDescription;

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="nodeTitle">����</param>
        /// <param name="nodeName">����</param>
        /// <param name="nodeDescription">����</param>
        public AddNodeForm(string nodeTitle, string nodeName, string nodeDescription)
        {
            this.nodeTitle = nodeTitle;
            this.nodeName = nodeName;
            this.nodeDescription = nodeDescription;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ����
        /// </summary>
        public string NodeName
        {
            get
            {
                return nodeName;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string NodeDescription
        {
            get
            {
                return nodeDescription;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            this.Text = nodeTitle;
            nameBox.Text = nodeName;
            descriptionBox.Text = nodeDescription;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bOK_Click(object sender, EventArgs e)
        {
            nodeName = nameBox.Text;
            nodeDescription = descriptionBox.Text;

            if (nodeName != "")
            {
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("���Ʋ���Ϊ�գ�", nodeTitle,
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}