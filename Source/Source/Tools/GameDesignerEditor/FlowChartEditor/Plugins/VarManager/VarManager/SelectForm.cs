using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using DevComponents.DotNetBar.Controls;

namespace VarManager
{
    public partial class SelectForm : Form
    {
        private SqlConnection conn; // sql����
        private string conditionFields; // ����������
        private string conditionValues; // ��������ֵ
        private int varIndex; // ������ƫ�����
        private string varString; // ��������ʾ�ַ���

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="conditonFields">����������</param>
        /// <param name="conditonValues">��������ֵ</param>
        public SelectForm(SqlConnection conn, string conditionFields, string conditionValues)
        {
            this.conn = conn;
            this.conditionFields = conditionFields;
            this.conditionValues = conditionValues;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ������ƫ�����
        /// </summary>
        public int VarIndex
        {
            get
            {
                return varIndex;
            }
        }

        /// <summary>
        /// ��������ʾ�ַ���
        /// </summary>
        public string VarString
        {
            get
            {
                return varString;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ʼ��������
            DataGridViewTextBoxColumn idColumn = new DataGridViewTextBoxColumn();
            idColumn.Name = "idColumn";
            idColumn.HeaderText = "����ID";
            idColumn.ReadOnly = true;
            DataGridViewTextBoxColumn offsetColumn = new DataGridViewTextBoxColumn();
            offsetColumn.Name = "offsetColumn";
            offsetColumn.HeaderText = "����ƫ��";
            offsetColumn.ReadOnly = true;
            DataGridViewTextBoxColumn nameColumn = new DataGridViewTextBoxColumn();
            nameColumn.Name = "nameColumn";
            nameColumn.HeaderText = "��������";
            nameColumn.ReadOnly = true;
            DataGridViewTextBoxColumn descriptionColumn = new DataGridViewTextBoxColumn();
            descriptionColumn.Name = "descriptionColumn";
            descriptionColumn.HeaderText = "��������";
            descriptionColumn.ReadOnly = true;
            DataGridViewTextBoxColumn userColumn = new DataGridViewTextBoxColumn();
            userColumn.Name = "userColumn";
            userColumn.HeaderText = "������";
            userColumn.ReadOnly = true;

            dataGridViewX1.Columns.Add(idColumn);
            dataGridViewX1.Columns.Add(offsetColumn);
            dataGridViewX1.Columns.Add(nameColumn);
            dataGridViewX1.Columns.Add(descriptionColumn);
            dataGridViewX1.Columns.Add(userColumn);

            // ��ʼ��������
            DataTable table = null;
            string[] fieldsArray = conditionFields.Split(new char[]{','});
            string[] valuesArray = conditionValues.Split(new char[]{','});
            string conditonString = "";
            for(int i = 0; i < fieldsArray.Length; i++)
            {
                conditonString += string.Format("[{0}] = '{1}' AND ", fieldsArray[i], valuesArray[i]);
            }
            conditonString = conditonString.Substring(0, conditonString.Length - 5);

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = "SELECT * FROM VarData WHERE " + conditonString;
                table = GetDataTable(sqlString);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ������Ϣʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            int index = 0;
            foreach(DataRow row in table.Rows)
            {
                dataGridViewX1.Rows.Add(1);
                DataGridViewRow newRow = dataGridViewX1.Rows[index];
                newRow.Cells["idColumn"].Value = row["ID"];
                newRow.Cells["offsetColumn"].Value = row["StartIndex"];
                newRow.Cells["nameColumn"].Value = row["Name"];
                newRow.Cells["descriptionColumn"].Value = row["Description"];
                newRow.Cells["userColumn"].Value = row["UserName"];
                index++;
            }
        }

        /// <summary>
        /// ���¼�������
        /// </summary>
        private void ReloadData()
        {
            // ��ʼ��������
            DataTable table = null;
            string[] fieldsArray = conditionFields.Split(new char[] { ',' });
            string[] valuesArray = conditionValues.Split(new char[] { ',' });
            string conditonString = "";
            for (int i = 0; i < fieldsArray.Length; i++)
            {
                conditonString += string.Format("[{0}] = '{1}' AND ", fieldsArray[i], valuesArray[i]);
            }
            conditonString = conditonString.Substring(0, conditonString.Length - 5);

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = "SELECT * FROM VarData WHERE " + conditonString;
                table = GetDataTable(sqlString);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ������Ϣʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            int index = 0;
            foreach (DataRow row in table.Rows)
            {
                dataGridViewX1.Rows.Add(1);
                DataGridViewRow newRow = dataGridViewX1.Rows[index];
                newRow.Cells["idColumn"].Value = row["ID"];
                newRow.Cells["offsetColumn"].Value = row["StartIndex"];
                newRow.Cells["nameColumn"].Value = row["Name"];
                newRow.Cells["descriptionColumn"].Value = row["Description"];
                newRow.Cells["userColumn"].Value = row["UserName"];
                index++;
            }
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sql">sql���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sql)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            System.Data.DataTable tbl = ds.Tables[0];
            return tbl;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if(dataGridViewX1.SelectedCells.Count > 0)
            {
                DataGridViewRow selectedRow = dataGridViewX1.SelectedCells[0].OwningRow;
                varIndex = int.Parse(selectedRow.Cells["offsetColumn"].EditedFormattedValue.ToString());
                varString = string.Format("{0},{1}", selectedRow.Cells["nameColumn"].EditedFormattedValue,
                    selectedRow.Cells["offsetColumn"].EditedFormattedValue);
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("û��ѡ���Զ��������", "����ѡ��", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
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
        /// �������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            MainForm mForm = new MainForm(conn);
            DialogResult result = mForm.ShowDialog();            
            ReloadData();            
        }
    }
}