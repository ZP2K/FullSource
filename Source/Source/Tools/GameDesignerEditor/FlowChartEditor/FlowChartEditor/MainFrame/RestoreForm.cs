using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

using DevComponents.DotNetBar;

using DBManager;

namespace FlowChartEditor
{
    public partial class RestoreForm : Office2007Form
    {
        string originID; // ԭ��ͼID
        DataTable backupTable; // ��ͼ������Ϣ���ݱ�

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="originID">ԭ��ͼID</param>
        /// <param name="backupTable">��ͼ������Ϣ���ݱ�</param>
        public RestoreForm(string originID, DataTable backupTable)
        {
            this.originID = originID;
            this.backupTable = backupTable;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ���������
            DataGridViewTextBoxColumn hostColumn = new DataGridViewTextBoxColumn();
            hostColumn.Name = "Host";
            hostColumn.HeaderText = "�༭��";

            DataGridViewTextBoxColumn editTimeColumn = new DataGridViewTextBoxColumn();
            editTimeColumn.Name = "EditTime";
            editTimeColumn.HeaderText = "�༭ʱ��";

            DataGridViewTextBoxColumn versionColumn = new DataGridViewTextBoxColumn();
            versionColumn.Name = "Version";
            versionColumn.HeaderText = "�汾��";

            dataGridViewX1.Columns.Add(hostColumn);
            dataGridViewX1.Columns.Add(editTimeColumn);
            dataGridViewX1.Columns.Add(versionColumn);

            // ���������            
            for (int i = 0; i < backupTable.Rows.Count; i++)
            {
                DataRow row = backupTable.Rows[i];
                dataGridViewX1.Rows.Add(1);
                DataGridViewRow newRow = dataGridViewX1.Rows[i];
                newRow.Cells["Host"].Value = row["Host"].ToString();
                newRow.Cells["EditTime"].Value = row["EditTime"].ToString();
                string diagramVersion = row["Version"].ToString();
                if (diagramVersion == "")
                {
                    diagramVersion = "0";
                }
                newRow.Cells["Version"].Value = diagramVersion;
                newRow.Tag = row["ID"].ToString();
            }
        }

        /// <summary>
        /// ѡ��ָ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {            
            if (dataGridViewX1.SelectedCells.Count >= 0)
            {
                string id = dataGridViewX1.SelectedCells[0].OwningRow.Tag.ToString();
                DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
                string lockUser = dataBaseManager.Diagram_WhoLocked(int.Parse(originID));

                if (lockUser == null)
                {
                    bool success = dataBaseManager.RestoreDiagramData(originID, id);

                    if (success)
                    {
                        MessageBox.Show("��ͼ�ָ��ɹ���", "�ָ���ͼ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        DialogResult = DialogResult.OK;
                        this.Close();
                    }
                }
                else
                {
                    MessageBox.Show(string.Format("�û�ͼ��ǰ��{0}������"), "�ָ���ͼ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ�ָ��Ļ�ͼ�汾��", "�ָ���ͼ", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
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
        /// �鿴��ͼ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {            
            if (dataGridViewX1.SelectedCells.Count >= 0)
            {
                string id = dataGridViewX1.SelectedCells[0].OwningRow.Tag.ToString();
                string version = dataGridViewX1.SelectedCells[0].OwningRow.Cells["Version"].Value.ToString();
                DocumentManager.GetDocumentManager().OpenDiaramAtNewPanel(id, version);
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ�ָ��Ļ�ͼ�汾��", "�ָ���ͼ", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            this.Close();
        }
    }
}