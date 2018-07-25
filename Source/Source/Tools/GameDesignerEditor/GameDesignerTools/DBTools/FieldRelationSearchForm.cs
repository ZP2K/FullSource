using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;

using DevComponents.AdvTree;

namespace GameDesingerTools
{
    public partial class FieldRelationSearchForm : Form
    {
        private SqlConnection conn;
        private string displayInfo;        
        private string sourceFieldValue;
        private Hashtable infoTable;

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="displayInfo">��ʾ��Ϣ</param>
        /// <param name="sourceFieldValue">�ֶ�ֵ</param>
        /// <param name="infoTable">���ݱ�</param>
        public FieldRelationSearchForm(SqlConnection conn, string displayInfo, string sourceFieldValue, Hashtable infoTable)
        {
            this.conn = conn;
            this.displayInfo = displayInfo;
            this.sourceFieldValue = sourceFieldValue;
            this.infoTable = infoTable;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            DataGridViewTextBoxColumn tableNameColumn = new DataGridViewTextBoxColumn();
            tableNameColumn.HeaderText = "���ݱ���";
            tableNameColumn.Name = "TableName";

            DataGridViewTextBoxColumn keyValueColumn = new DataGridViewTextBoxColumn();
            keyValueColumn.HeaderText = "����ֵ";
            keyValueColumn.Name = "KeyValue";

            DataGridViewTextBoxColumn fieldNameColumn = new DataGridViewTextBoxColumn();
            fieldNameColumn.HeaderText = "�ֶ���";
            fieldNameColumn.Name = "FieldName";

            resultView.Columns.Add(tableNameColumn);
            resultView.Columns.Add(keyValueColumn);
            resultView.Columns.Add(fieldNameColumn);

            fieldValueBox.Text = sourceFieldValue;
            ShowSearchResult(sourceFieldValue);
        }

        /// <summary>
        /// ��ʾ���ҽ��
        /// </summary>
        /// <param name="value">��ʾ���ҽ��</param>
        private void ShowSearchResult(string value)
        {
            resultView.Rows.Clear();

            if (conn.State == ConnectionState.Closed)
            {
                conn.Open();
            }

            foreach (string tableName in infoTable.Keys)
            {
                List<string> fieldList = infoTable[tableName] as List<string>;
                StringBuilder conditionString = new StringBuilder();

                foreach (string s in fieldList)
                {
                    conditionString.Append(string.Format("[{0}] = '{1}' OR ", s, value));
                }

                conditionString.Remove(conditionString.Length - 4, 4);

                string sqlString = string.Format("SELECT * FROM {0} WHERE {1}", tableName, conditionString.ToString());
               // DataTable dataTable = Helper.GetDataTable(sqlString, conn);
                DataTable dataTable = Helper.GetDataTableProxy(tableName, conditionString.ToString(), null, conn);
                foreach (DataRow dataRow in dataTable.Rows)
                {
                    StringBuilder fieldString = new StringBuilder();
                    StringBuilder keyString = new StringBuilder();

                    foreach (DataColumn dataColumn in dataTable.PrimaryKey)
                    {
                        keyString.Append(string.Format("{0} = {1}, ", dataColumn.ColumnName, dataRow[dataColumn].ToString()));
                    }

                    keyString.Remove(keyString.Length - 2, 2);

                    foreach (string s in fieldList)
                    {
                        if (dataRow[s].ToString() == value)
                        {
                            fieldString.Append(string.Format("{0}, ", s));
                        }
                    }

                    fieldString.Remove(fieldString.Length - 2, 2);

                    resultView.Rows.Add(1);
                    DataGridViewRow newRow = resultView.Rows[resultView.Rows.Count - 1];
                    newRow.Cells["TableName"].Value = tableName;
                    newRow.Cells["KeyValue"].Value = keyString.ToString();
                    newRow.Cells["FieldName"].Value = fieldString;
                }
            }           

            if (conn.State == ConnectionState.Open)
            {
                conn.Close();
            }

            this.Text = string.Format("������ϵ���� ����{0}��ƥ���¼", resultView.Rows.Count.ToString());
            labelX2.Text = displayInfo;
        }

        /// <summary>
        /// ��ʼ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bSearch_Click(object sender, EventArgs e)
        {
            string fieldValue = fieldValueBox.Text;

            if (fieldValue != "")
            {
                ShowSearchResult(fieldValue);
            }
            else
            {
                MessageBox.Show("�ֶ�ֵ����Ϊ�գ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            
        }
    }
}