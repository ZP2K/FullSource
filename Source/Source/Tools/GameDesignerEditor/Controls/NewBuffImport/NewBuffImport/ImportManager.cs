using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.IO;

namespace NewBuffImport
{
    public class ImportManager
    {        
        public ImportManager()
        {

        }

        public void ImportNewBuff(SqlConnection conn)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "�����ļ�|*.tab";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string fileName = openFileDialog.FileName;
                string content = GetFileContent(fileName);

                if (content != null)
                {
                    DataTable dataTable = CreateDataTable(content);
                    SqlTransaction transaction = null;

                    try
                    {
                        if (conn.State == ConnectionState.Closed)
                        {
                            conn.Open();
                        }

                        string sqlString = "SELECT * FROM tbl_NewBuff WHERE ID = 101 AND Level = 1";
                        DataTable defaultTable = GetDataTable(conn, sqlString);
                        DataRow defaultRow = defaultTable.Rows[0];

                        SqlCommand cmd = conn.CreateCommand();
                        transaction = conn.BeginTransaction();
                        cmd.Transaction = transaction;

                        int maxID = 0;                        
                        sqlString = "SELECT MAX(ID) FROM tbl_NewBuff";
                        cmd.CommandText = sqlString;
                        object executeResult = cmd.ExecuteScalar();

                        if (executeResult != null)
                        {
                            maxID = (int)executeResult + 1;
                        }                                                

                        foreach (DataRow dataRow in dataTable.Rows)
                        {                                                        
                            string level = dataRow["Level"].ToString();

                            if (level == "")
                            {
                                level = "1";
                            }

                            sqlString = string.Format("INSERT INTO tbl_NewBuff ([ID], [Level]) VALUES ({0}, {1})", 
                                                      maxID.ToString(), level);
                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();

                            foreach (DataColumn dataColumn in dataTable.Columns)
                            {
                                string value = dataRow[dataColumn.ColumnName].ToString();                                

                                if (dataColumn.ColumnName != "Level")
                                {
                                    if (value != "")
                                    {
                                        sqlString = string.Format("UPDATE tbl_NewBuff SET [{0}] = '{1}' WHERE ID = {2} AND Level = {3}",
                                                                  dataColumn.ColumnName, value, maxID.ToString(), level);
                                        cmd.CommandText = sqlString;
                                        cmd.ExecuteNonQuery();
                                    }
                                    else
                                    {
                                        string defaultValue = defaultRow[dataColumn.ColumnName].ToString();

                                        if (defaultValue != "")
                                        {
                                            // ת������ֵ
                                            if (defaultValue == "True")
                                            {
                                                defaultValue = "1";
                                            }
                                            else if (defaultValue == "False")
                                            {
                                                defaultValue = "0";
                                            }

                                            sqlString = string.Format("UPDATE tbl_NewBuff SET [{0}] = '{1}' WHERE ID = {2} AND Level = {3}",
                                                                      dataColumn.ColumnName, defaultValue, maxID.ToString(), level);
                                            cmd.CommandText = sqlString;
                                            cmd.ExecuteNonQuery();
                                        }
                                    }
                                }
                            }

                            maxID++;
                        }

                        transaction.Commit();
                    }
                    catch (SqlException ex)
                    {
                        MessageBox.Show("�ڸ���Buff����ʱ����sql�쳣��" + ex.Message, "����Buff����",
                                        MessageBoxButtons.OK, MessageBoxIcon.Information);

                        if (transaction != null)
                        {
                            transaction.Rollback();
                        }
                    }
                    finally
                    {
                        if (conn.State == ConnectionState.Open)
                        {
                            conn.Close();
                        }
                    }

                    MessageBox.Show("Buff������ɣ�", "����Buff", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        /// <summary>
        /// ��ȡ�ļ��ı�����
        /// </summary>
        /// <param name="filePath">�ļ�·��</param>
        /// <returns>�ı�����</returns>
        private string GetFileContent(string filePath)
        {
            string content = null;

            if (File.Exists(filePath))
            {
                StreamReader reader = new StreamReader(filePath, Encoding.GetEncoding("gb2312"));
                content = reader.ReadToEnd();
                reader.Close();
            }
            else
            {
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", filePath), "��ȡ�����ļ�",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return content;
        }

        /// <summary>
        /// �������ݱ�
        /// </summary>
        /// <param name="content">����</param>
        /// <returns>���ݱ�</returns>
        private DataTable CreateDataTable(string content)
        {
            DataTable table = new DataTable();
            List<string> fieldList = new List<string>();

            bool firstLine = true;
            string[] lines = content.Split(new char[] { '\n' });

            foreach (string s in lines)
            {
                string line = s.TrimEnd(new char[] { '\r' });

                if (line != "") // �����յ�������
                {
                    string[] data = line.Split(new char[] { '\t' });

                    if (firstLine)
                    {
                        firstLine = false;

                        for (int i = 0; i < data.Length; i++)
                        {
                            DataColumn dataColumn = new DataColumn(data[i]);
                            table.Columns.Add(dataColumn);
                            fieldList.Add(data[i]);
                        }
                    }
                    else
                    {
                        DataRow dataRow = table.NewRow();

                        for (int i = 0; i < data.Length; i++)
                        {
                            dataRow[i] = data[i];
                        }

                        table.Rows.Add(dataRow);
                    }
                }

            }

            return table;
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="sql">sql���</param>
        /// <returns>���ݱ�</returns>
        public DataTable GetDataTable(SqlConnection conn, string sql)
        {
            DataTable table = null;

            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            table = ds.Tables[0];

            return table;
        }
    }
}
