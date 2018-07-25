using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Windows.Forms;
using System.Data.OleDb;

namespace StandardNpcCreate
{
    class Helper
    {
        private static Helper helper;

        private Helper()
        {

        }

        /// <summary>
        /// ��ȡ������ʵ��
        /// </summary>
        /// <returns>������ʵ��</returns>
        public static Helper GetHelper()
        {
            if (helper == null)
            {
                helper = new Helper();
            }

            return helper;
        }

        /// <summary>
        /// ��ȡ�ļ��ı�����
        /// </summary>
        /// <param name="filePath">�ļ�·��</param>
        /// <returns>�ı�����</returns>
        public string GetFileContent(string filePath)
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
        public DataTable CreateDataTable(string content)
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

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                table = ds.Tables[0];
            }
            catch (SqlException ex)
            {
                ; // ��ʱ����Ҫ����������Ϣ
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            return table;
        }

        /// <summary>
        /// ����ַ����Ƿ���Ч
        /// </summary>
        /// <param name="inputString">�����ַ���</param>
        /// <returns>�Ƿ����</returns>
        public bool CheckStringValid(string inputString)
        {
            bool valid = false;

            if (!string.IsNullOrEmpty(inputString))
            {
                string newString = inputString.Trim(new char[] { ' ', '��' });

                if (!string.IsNullOrEmpty(newString))
                {
                    valid = true;
                }
            }

            return valid;
        }

        /// <summary>
        /// ������д���ļ�
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <param name="content">�ı�����</param>
        public void SaveDataToFile(string fileName, string content)
        {
            try
            {
                string filePath = Path.GetDirectoryName(fileName);
                FileInfo fi = new FileInfo(fileName);
                StreamWriter sw;

                if (!fi.Exists) // �ļ�������
                {
                    if (!Directory.Exists(filePath)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(filePath); // �ȴ���Ŀ¼���ٴ����ļ�
                    }

                    FileStream fs = fi.Create();
                    fs.Close();
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }
                else // �ļ��Ѿ�����
                {
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }

                sw.Write(content);
                sw.Close();
            }
            catch (IOException ex)
            {
                ;
            }
        }

        /// <summary>
        /// ��excel�ļ��д������ݱ�
        /// </summary>
        /// <param name="filePath">excel�ļ�·��</param>
        /// <returns>���ݱ�</returns>
        public DataTable CreateDataTableFromExcelFile(string filePath, string sheetName)
        {
            DataTable table = new DataTable();

            string connectString = string.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source={0};Extended Properties=\"Excel 8.0;HDR=YES;IMEX=1\"", filePath);
            OleDbConnection oledbConnection = new OleDbConnection(connectString);
            OleDbDataAdapter myCommand = null;
            DataSet ds = new DataSet();
            string sqlString = string.Format("SELECT * FROM [{0}$]", sheetName);

            try
            {
                oledbConnection.Open();
                myCommand = new OleDbDataAdapter(sqlString, oledbConnection);
                myCommand.Fill(ds);
            }
            catch (Exception ex)
            {
                MessageBox.Show("��ȡExcel��Ϣʱ�����쳣��" + ex.Message, "��ȡ����",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            finally
            {
                oledbConnection.Close();
                oledbConnection.Dispose();
            }

            table = ds.Tables[0];

            return table;
        }
    }
}
