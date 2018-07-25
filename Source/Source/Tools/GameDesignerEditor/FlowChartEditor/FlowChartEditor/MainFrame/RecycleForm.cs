using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

using DevComponents.DotNetBar;

using DBManager;

namespace FlowChartEditor
{
    public partial class BackupForm : Office2007Form
    {        
        public BackupForm()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ʼ��������
            

            // ��ʼ��������
            DataGridViewTextBoxColumn editorColumn = new DataGridViewTextBoxColumn();
            editorColumn.Name = "Editor";
            editorColumn.HeaderText = "�༭��";

            DataGridViewTextBoxColumn lastEditTimeColumn = new DataGridViewTextBoxColumn();
            lastEditTimeColumn.Name = "LastEditTime";
            lastEditTimeColumn.HeaderText = "���༭ʱ��";

            infoView.Columns.Add(editorColumn);
            infoView.Columns.Add(lastEditTimeColumn);
        }

        /// <summary>
        /// ѡ��ָ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bOK_Click(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCancel_Click(object sender, EventArgs e)
        {
            
        }

        /// <summary>
        /// Ԥ����ͼ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bView_Click(object sender, EventArgs e)
        {

        }
    }
}