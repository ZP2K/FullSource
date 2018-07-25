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
    public partial class LogForm : Office2007Form
    {
        private DataTable logTable; // log���ݱ�

        /// <summary>
        /// ���캯��
        /// </summary>
        public LogForm()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ���������
            DataGridViewTextBoxColumn hostNameColumn = new DataGridViewTextBoxColumn();
            hostNameColumn.Name = "HostName";
            hostNameColumn.HeaderText = "�û���";
            hostNameColumn.ReadOnly = true;

            DataGridViewTextBoxColumn operationColumn = new DataGridViewTextBoxColumn();
            operationColumn.Name = "Operation";
            operationColumn.HeaderText = "����";
            operationColumn.ReadOnly = true;

            DataGridViewTextBoxColumn operateTimeColumn = new DataGridViewTextBoxColumn();
            operateTimeColumn.Name = "OperateTime";
            operateTimeColumn.HeaderText = "ʱ��";
            operateTimeColumn.ReadOnly = true;

            DataGridViewTextBoxColumn successColumn = new DataGridViewTextBoxColumn();
            successColumn.Name = "Success";
            successColumn.HeaderText = "�Ƿ�ɹ�";
            successColumn.ReadOnly = true;

            DataGridViewTextBoxColumn descriptionColumn = new DataGridViewTextBoxColumn();
            descriptionColumn.Name = "Description";
            descriptionColumn.HeaderText = "����";
            descriptionColumn.ReadOnly = true;

            dataGridViewX1.Columns.Add(hostNameColumn);
            dataGridViewX1.Columns.Add(operationColumn);
            dataGridViewX1.Columns.Add(operateTimeColumn);
            dataGridViewX1.Columns.Add(successColumn);
            dataGridViewX1.Columns.Add(descriptionColumn);
        }

        /// <summary>
        /// ��������
        /// </summary>
        public void Reload()
        {
            string sqlString = string.Format("SELECT Top 25 * FROM DiagramLog ORDER BY OperateTime DESC");
            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            logTable = dataBaseManager.GetDataTable(sqlString);           

            // ����������
            dataGridViewX1.Rows.Clear();
            int index = 0;
            List<string> userList = new List<string>();
            userList.Add("�����û�");
            List<string> operationList = new List<string>();
            operationList.Add("���в���");

            foreach(DataRow row in logTable.Rows)
            {
                dataGridViewX1.Rows.Add(1);
                DataGridViewRow newRow = dataGridViewX1.Rows[index];
                string hostName = row["HostName"].ToString();
                string operation = row["Operation"].ToString();

                newRow.Cells["HostName"].Value = hostName;
                newRow.Cells["Operation"].Value = operation;
                newRow.Cells["OperateTime"].Value = row["OperateTime"].ToString();
                newRow.Cells["Success"].Value = row["Success"].ToString();
                newRow.Cells["Description"].Value = row["Description"].ToString();

                if(!userList.Contains(hostName))
                {
                    userList.Add(hostName);
                }

                if(!operationList.Contains(operation))
                {
                    operationList.Add(operation);
                }

                index++;
            }

            // �������������
            comboBoxEx1.Items.Clear();
            comboBoxEx1.Items.AddRange(userList.ToArray());
            comboBoxEx1.SelectedIndex = 0;
            comboBoxEx2.Items.Clear();
            comboBoxEx2.Items.AddRange(operationList.ToArray());
            comboBoxEx2.SelectedIndex = 0;
            dateTimeInput1.Value = DateTime.Now.AddDays(-1);
            dateTimeInput2.Value = DateTime.Now;
        }

        /// <summary>
        /// ������־
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string hostName = comboBoxEx1.Text;
            string selectHostString = string.Format("HostName = '{0}'", hostName);

            if (hostName == "�����û�")
            {
                selectHostString = "";
            }

            string operation = comboBoxEx2.Text;
            string selectOperationString = string.Format("Operation = '{0}'", operation);

            if (operation == "���в���")
            {
                selectOperationString = "";
            }

            DateTime startTime = dateTimeInput1.Value;
            startTime = startTime.AddHours(-dateTimeInput1.Value.Hour);
            startTime = startTime.AddMinutes(-dateTimeInput1.Value.Minute);
            startTime = startTime.AddSeconds(-dateTimeInput1.Value.Second);

            DateTime endTime = dateTimeInput2.Value;
            endTime = endTime.AddHours(23 - dateTimeInput2.Value.Hour);
            endTime = endTime.AddMinutes(59 - dateTimeInput2.Value.Minute);
            endTime = endTime.AddSeconds(59 - dateTimeInput2.Value.Second);

            string selectString = string.Format("SELECT * FROM DiagramLog WHERE OperateTime >= '{0}' AND OperateTime <= '{1}'",
                                                startTime, endTime);
            if (selectHostString != "")
            {
                selectString += string.Format(" AND {0}", selectHostString);
            }

            if (selectOperationString != "")
            {
                selectString += string.Format(" AND {0}", selectOperationString);
            }

            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            logTable = dataBaseManager.GetDataTable(selectString);

            // ����������
            dataGridViewX1.Rows.Clear();
            int index = 0;

            foreach (DataRow row in logTable.Rows)
            {
                dataGridViewX1.Rows.Add(1);
                DataGridViewRow newRow = dataGridViewX1.Rows[index];

                newRow.Cells["HostName"].Value = row["HostName"].ToString();
                newRow.Cells["Operation"].Value = row["Operation"].ToString();
                newRow.Cells["OperateTime"].Value = row["OperateTime"].ToString();
                newRow.Cells["Success"].Value = row["Success"].ToString();
                newRow.Cells["Description"].Value = row["Description"].ToString();

                index++;
            }
        }

        /// <summary>
        /// �رմ���
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