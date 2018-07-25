using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

using DevComponents.DotNetBar;

namespace DataTableCoordinate
{
    public partial class MainForm : Office2007Form
    {
        private string sourceDatabaseName = "s3design_rtm";
        private string destinationDatabaseName = "s3design_test";
        private List<string> databaseNameList = new List<string>();
        private const string connectString = "Server = jx3web; DataBase = {0}; Uid = s3design; Password = davidbowie;";

        private SqlConnection sourceConnection;
        private SqlConnection destinationConnection;        

        private Dictionary<string, TableInfo> sourceTableInfoDictionary = new Dictionary<string, TableInfo>();
        private Dictionary<string, TableInfo> destinationTableInfoDictionary = new Dictionary<string, TableInfo>();

        public MainForm()
        {
            InitializeComponent();
            Init();
        }

        class TableInfo
        {
            private readonly string tableName;
            private readonly DataTable columnTable;

            public TableInfo(string tableName, DataTable columnTable)
            {
                this.tableName = tableName;
                this.columnTable = columnTable;
            }

            public string TableName
            {
                get
                {
                    return tableName;
                }
            }

            public DataTable ColumnTable
            {
                get
                {
                    return columnTable;
                }
            }
        }

        private void Init()
        {
            // ��ʼ�������б�
            databaseNameList.Add("s3design_rtm");
            databaseNameList.Add("s3design_debug");
            databaseNameList.Add("s3design_test");
            databaseNameList.Add("s3design_skill_branch");
            databaseNameList.Add("s3design_branch_liqing");

            foreach (string databaseName in databaseNameList)
            {
                sourceDatabaseNameBox.Items.Add(databaseName);
                destinationDatabaseNameBox.Items.Add(databaseName);
            }

            sourceDatabaseNameBox.Text = sourceDatabaseName;
            destinationDatabaseNameBox.Text = destinationDatabaseName;
            
            // ��ʼ��������
            DataGridViewTextBoxColumn tableNameColumn = new DataGridViewTextBoxColumn();
            tableNameColumn.Name = "TableName";
            tableNameColumn.HeaderText = "���ݱ���";

            DataGridViewTextBoxColumn columnNameColumn = new DataGridViewTextBoxColumn();
            columnNameColumn.Name = "ColumnName";
            columnNameColumn.HeaderText = "����";

            DataGridViewTextBoxColumn descriptionColumn = new DataGridViewTextBoxColumn();
            descriptionColumn.Name = "Description";
            descriptionColumn.HeaderText = "����";

            resultView.Columns.Add(tableNameColumn);
            resultView.Columns.Add(columnNameColumn);
            resultView.Columns.Add(descriptionColumn);            
        }

        private DataTable GetDataTableWithoutOpen(string sqlString, SqlConnection conn)
        {
            DataTable table = null;            

            SqlDataAdapter adapter = new SqlDataAdapter(sqlString, conn);
            adapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet dataSet = new DataSet();
            adapter.Fill(dataSet);
            table = dataSet.Tables[0];            

            return table;
        }

        private DataTable GetDataTable(string sqlString, SqlConnection conn)
        {
            DataTable table = null;

            if (conn.State == ConnectionState.Closed)
            {
                conn.Open();
            }

            SqlDataAdapter adapter = new SqlDataAdapter(sqlString, conn);
            adapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet dataSet = new DataSet();
            adapter.Fill(dataSet);
            table = dataSet.Tables[0];
            
            if (conn.State == ConnectionState.Open)
            {
                conn.Close();
            }

            return table;
        }

        private void AddNewDataGridViewRow(string tableName, string columnName, string description)
        {
            resultView.Rows.Add(1);
            DataGridViewRow newRow = resultView.Rows[resultView.Rows.Count - 1];
            newRow.Cells["TableName"].Value = tableName;
            newRow.Cells["ColumnName"].Value = columnName;
            newRow.Cells["Description"].Value = description;
        }

        private void AddNewDataRow(SqlCommand cmd, string tableName, DataColumn dataColumn)
        {
            string typeString = "";

            switch (dataColumn.DataType.Name)
            {
                case "String":
                    {
                        typeString = string.Format("varchar ({0})", dataColumn.MaxLength);

                        break;
                    }
                case "Decimal":
                    {
                        string selectString = string.Format("SELECT prec, scale FROM syscolumns WHERE name = '{0}' AND id = object_id('{1}')",
                                                            dataColumn.ColumnName, tableName);
                        DataTable dataTable = GetDataTable(selectString, destinationConnection);

                        int prec = int.Parse(dataTable.Rows[0]["prec"].ToString());
                        int scale = int.Parse(dataTable.Rows[0]["scale"].ToString());

                        typeString = string.Format("decimal ({0}, {1})", prec, scale);

                        break;
                    }
                case "Boolean":
                    {
                        typeString = "bit";

                        break;
                    }
                case "Int32":
                    {
                        typeString = "int";

                        break;
                    }
            }


            string sqlString = string.Format("ALTER TABLE {0} ADD [{1}] {2} NULL",
                                             tableName, dataColumn.ColumnName, typeString);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();
        }

        /// <summary>
        /// ��ʼ��������
        /// </summary>
        /// <param name="minValue">��Сֵ</param>
        /// <param name="maxValue">���ֵ</param>
        private void InitProgressBar(int minValue, int maxValue)
        {
            progressBar.Minimum = minValue;
            progressBar.Maximum = maxValue;
            progressBar.Value = minValue;
            lProgressInfo.Text = "��ǰ����";
        }

        /// <summary>
        /// ���½�����
        /// </summary>
        /// <param name="newValue">��ֵ</param>
        /// <param name="displayText">��ʾ�ı�</param>
        private void UpdateProgressBar(int newValue, string displayText)
        {
            progressBar.Value = newValue;
            lProgressInfo.Text = displayText;
        }        

        /// <summary>
        /// ��ʼ��sql����
        /// </summary>
        private void InitConnection()
        {
            sourceDatabaseName = sourceDatabaseNameBox.Text;
            destinationDatabaseName = destinationDatabaseNameBox.Text;
            string sourceConnectString = string.Format(connectString, sourceDatabaseName);
            string destinationConnectString = string.Format(connectString, destinationDatabaseName);

            if (sourceConnection == null)
            {
                sourceConnection = new SqlConnection(sourceConnectString);                
            }
            else if (sourceConnection.ConnectionString != sourceConnectString)
            {
                sourceConnection = new SqlConnection(sourceConnectString);
            }

            if (destinationConnection == null)
            {
                destinationConnection = new SqlConnection(destinationConnectString);
            }
            else if (destinationConnection.ConnectionString != destinationConnectString)
            {
                destinationConnection = new SqlConnection(destinationConnectString);
            }
        }

        /// <summary>
        /// ��ʼ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bStartSearch_Click(object sender, EventArgs e)
        {
            resultView.Rows.Clear();

            InitConnection();

            string sqlString = "SELECT name FROM sysobjects WHERE xtype = 'u' AND name NOT LIKE 'sys_' ORDER BY name";
            DataTable dataTable = GetDataTable(sqlString, sourceConnection);

            foreach (DataRow dataRow in dataTable.Rows)
            {
                string tableName = dataRow["name"].ToString();
                sqlString = string.Format("SELECT TOP 1 * FROM {0}", tableName);
                DataTable rowInfo = GetDataTable(sqlString, sourceConnection);
                sourceTableInfoDictionary[tableName] = new TableInfo(tableName, rowInfo);
            }

            sqlString = "SELECT name FROM sysobjects WHERE xtype = 'u' AND name NOT LIKE 'sys_' ORDER BY name";
            dataTable = GetDataTable(sqlString, destinationConnection);

            foreach (DataRow dataRow in dataTable.Rows)
            {
                string tableName = dataRow["name"].ToString();
                sqlString = string.Format("SELECT TOP 1 * FROM {0}", tableName);
                DataTable rowInfo = GetDataTable(sqlString, destinationConnection);
                destinationTableInfoDictionary[tableName] = new TableInfo(tableName, rowInfo);
            }

            int index = 0;
            InitProgressBar(0, sourceTableInfoDictionary.Keys.Count + 1);            

            foreach (string tableName in sourceTableInfoDictionary.Keys)
            {
                index++;
                UpdateProgressBar(index, string.Format("��ʼ�ȶ�{0}��������...", tableName));

                TableInfo sourceTableInfo = sourceTableInfoDictionary[tableName];

                if (destinationTableInfoDictionary.ContainsKey(tableName))
                {
                    TableInfo destinationTableInfo = destinationTableInfoDictionary[tableName];
                    List<string> columnNameList = new List<string>();

                    foreach (DataColumn dataColumn in destinationTableInfo.ColumnTable.Columns)
                    {
                        columnNameList.Add(dataColumn.ColumnName);
                    }

                    foreach (DataColumn sourceTableColumn in sourceTableInfo.ColumnTable.Columns)
                    {
                        if (!columnNameList.Contains(sourceTableColumn.ColumnName))
                        {
                            AddNewDataGridViewRow(tableName, sourceTableColumn.ColumnName, "ȱʧ");
                        }                        
                    }                    
                }
            }            

            lProgressInfo.Text = string.Format("�ȶ���ɣ���ȱ��{0}��", resultView.Rows.Count);
        }

        /// <summary>
        /// ��ʼͬ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            InitConnection();

            int index = 0;
            InitProgressBar(0, sourceTableInfoDictionary.Keys.Count + 1);

            if (destinationConnection.State == ConnectionState.Closed)
            {
                destinationConnection.Open();
            }

            SqlCommand cmd = destinationConnection.CreateCommand();
            int addColumnCount = 0;

            foreach (string tableName in sourceTableInfoDictionary.Keys)
            {
                index++;
                UpdateProgressBar(index, string.Format("��ʼͬ��{0}��������...", tableName));

                TableInfo sourceTableInfo = sourceTableInfoDictionary[tableName];

                if (destinationTableInfoDictionary.ContainsKey(tableName))
                {
                    TableInfo destinationTableInfo = destinationTableInfoDictionary[tableName];
                    List<string> columnNameList = new List<string>();

                    foreach (DataColumn dataColumn in destinationTableInfo.ColumnTable.Columns)
                    {
                        columnNameList.Add(dataColumn.ColumnName);
                    }

                    foreach (DataColumn sourceTableColumn in sourceTableInfo.ColumnTable.Columns)
                    {
                        if (!columnNameList.Contains(sourceTableColumn.ColumnName))
                        {
                            AddNewDataRow(cmd, tableName, sourceTableColumn);
                            addColumnCount++;
                        }
                    }  
                }
            }

            if (destinationConnection.State == ConnectionState.Open)
            {
                destinationConnection.Close();
            }

            lProgressInfo.Text = string.Format("ͬ����ɣ���ͬ��{0}������", addColumnCount);
        }
    }
}