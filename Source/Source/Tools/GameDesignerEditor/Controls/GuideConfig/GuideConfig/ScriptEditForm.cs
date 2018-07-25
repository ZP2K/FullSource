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
    public partial class ScriptEditForm : Office2007Form
    {
        private string scriptText; // �ű�����

        /// <summary>
        /// ���캯�� 
        /// </summary>
        /// <param name="scriptText">�ű�����</param>
        public ScriptEditForm(string scriptText)
        {
            this.scriptText = scriptText;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            luaEditorBox.Text = scriptText;
        }

        /// <summary>
        /// �ű�����
        /// </summary>
        public string ScriptText
        {
            get
            {
                return scriptText;
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bOK_Click(object sender, EventArgs e)
        {
            scriptText = luaEditorBox.Text;

            DialogResult = DialogResult.OK;
            this.Close();
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