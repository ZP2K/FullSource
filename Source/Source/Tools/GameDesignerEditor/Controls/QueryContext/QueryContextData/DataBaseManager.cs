using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Data;
using System.Collections;
using System.Text;
using System.IO;

namespace DataBaseIO
{
    public class DataBaseManager
    {
        private static DataBaseManager manager = null; // DataBaseManager��ʵ��
        private DataTable lsTable = null; // ls���ݱ�
        private Hashtable lsHashtable = null; // ls hash��
        public SqlConnection conn; // ���ݿ�����


        /// <summary>
        /// ��ȡDataBaseManager��ʵ��
        /// </summary>
        /// <returns>DataBaseManager��ʵ��</returns>
        public static DataBaseManager GetDataBaseManager()
        {
            if (manager == null) // ����δ��ʼ��
            {
                manager = new DataBaseManager();
            }

            return manager;
        }

        /// <summary>
        /// sql��������
        /// </summary>
        public SqlConnection Connection
        {
            get
            {
                return conn;
            }
        }

        /// <summary>
        /// ��ʼ��DataBaseManager����
        /// </summary>
        /// <param name="connectString">�����ַ���</param>
        public void Init(string connectString)
        {
            InitConnection(connectString);
            InitLsTable();
            InitLsHashTable();
        }

        /// <summary>
        /// ��ȡ�û���
        /// </summary>
        /// <returns>�û���</returns>
        public string GetHostName()
        {
            return System.Net.Dns.GetHostName();
        }

        /// <summary>
        /// ��ȡ���ݿ���
        /// </summary>
        /// <returns>���ݿ���</returns>
        public string GetDataBaseName()
        {
            string dataBaseName = "����δ��ʼ��";

            if(conn != null)
            {
                dataBaseName = conn.Database.ToString();
            }

            return dataBaseName; 
        }

        /// <summary>
        /// ��ȡ�ű���Ϣ���ݱ�
        /// </summary>
        /// <returns>�ű���Ϣ���ݱ�</returns>
        public DataTable GetScriptInformation()
        {
            DataTable scriptTable = null;

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT {0}, {1} FROM {2}", "id", "path",
                    "sys_script_script");
                scriptTable = GetDataTable(sqlString);
                if (scriptTable.Rows.Count < 0) // �����ڽű���¼
                {
                    scriptTable = null;
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڻ�ȡ�ű���Ϣ���ݱ�ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return scriptTable;
        }

        /// <summary>
        /// ��ȡls��Ϣ���ݱ�
        /// </summary>
        /// <returns>ls��Ϣ���ݱ�</returns>
        public DataTable GetLSInformation()
        {
            DataTable lsTable = null;

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0}", "sys_script_lsfile");
                lsTable = GetDataTable(sqlString);
                if(lsTable.Rows.Count < 0) // ������ls��¼
                {
                    lsTable = null;
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڻ�ȡls���ݱ�ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return lsTable;
        }

        /// <summary>
        /// ��ȡls�ķ�����Ϣ
        /// </summary>
        /// <returns>ls��������</returns>
        public List<string> GetLSClassification()
        {
            List<string> classificationList = new List<string>();

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT {0} FROM {1} GROUP BY {2}",
                    "classification", "sys_script_lsfile", "classification");
                DataTable lsTable = GetDataTable(sqlString);
                foreach(DataRow r in lsTable.Rows)
                {
                    classificationList.Add(r[0].ToString());
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڻ�ȡls������Ϣʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return classificationList;
        }

        /// <summary>
        /// �����ݿⴴ���µĽű�
        /// </summary>
        /// <param name="path">�ű�</param> 
        /// <returns>�ű���id</returns>
        public string CreateScriptData(string path)
        {
            string id = null; // �ű���id
            SqlTransaction transaction = null; // ����
            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                string sqlString = string.Format("INSERT INTO {0} ({1}, {2}, {3}) VALUES ('{4}', '{5}', '{6}')",
                    "sys_script_script", "path", "host", "edittime", path, GetHostName(), DateTime.Now);
                cmd.CommandText = sqlString;
                cmd.Transaction = transaction;
                int iret = cmd.ExecuteNonQuery();

                sqlString = string.Format("SELECT MAX({0}) FROM {1}", "id", "sys_script_script");
                cmd.CommandText = sqlString;
                id = cmd.ExecuteScalar().ToString();
                transaction.Commit(); // �ύ����
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڴ����µĽű�ʱ�����쳣: " + ex.ToString());
                if (transaction != null) // �ع�����
                {
                    transaction.Rollback();
                }
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql������
                {
                    conn.Close();
                }
            }

            return id;
        }

        /// <summary>
        /// ��ȡ�ű�����
        /// </summary>
        /// <param name="path">�ű�·��</param>
        /// <returns>�ű�����</returns>
        private string GetClassification(string path)
        {            
            string[] as_Path = path.Split(new string[] { "\\" }, StringSplitOptions.RemoveEmptyEntries);
            string classification = "δ����";

            if (as_Path.Length >= 3 && as_Path[0] == "scripts" && as_Path[1] != "Map")
            {
                classification = as_Path[1];
            }
            else if (as_Path.Length < 3 || as_Path[0] != "scripts" || as_Path[1] != "Map")
            {
                ;
            }
            else
            {
                classification = as_Path[2];
            }

            return classification;
        }

        /// <summary>
        /// ��ǰ�ű����Ƿ����ڳ�������
        /// </summary>
        /// <param name="scriptPath">�ű�·��</param>
        /// <returns>�Ƿ����ڳ�������</returns>
        private bool IsScriptInMap(string scriptPath)
        {
            bool scriptInMap = scriptPath.StartsWith(@"scripts\Map");

            return scriptInMap;
        }        

        /// <summary>
        /// ����ls�ļ��ַ��������ݿ�
        /// </summary>
        /// <param name="content">ls�ļ��ַ�������</param>
        /// <param name="classification">ls�ļ�����</param>
        /// <returns>id��</returns>
        private string SaveLsFileData(string content, string classification)
        {
            string id = null; // ���ص�id
            SqlTransaction transaction = null; // ����  

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                content = content.Replace("'", "''"); // �������ݿ�ǰ�����滻
                transaction = conn.BeginTransaction(); // ��������
                SqlCommand cmd = conn.CreateCommand();
                string sqlString = string.Format("INSERT INTO {0} ({1}, {2}) VALUES (\'{3}\', \'{4}\')",
                    "sys_script_lsfile", "content", "classification", content, classification);
                cmd.CommandText = sqlString;
                cmd.Transaction = transaction;
                cmd.ExecuteNonQuery();

                sqlString = string.Format("SELECT MAX({0}) FROM {1}", "id", "sys_script_lsfile");
                cmd.CommandText = sqlString;
                id = cmd.ExecuteScalar().ToString();
                transaction.Commit(); // �ύ����
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڱ���ls�ļ���Ϣʱ�����쳣: " + ex.ToString());
                if (transaction != null) // �ع�����
                {
                    transaction.Rollback();
                }
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return id;
        }

        /// <summary>
        /// ��յ�ǰ�û���������
        /// </summary>
        public void ClearRecordLocks()
        {
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                SqlCommand cmd = conn.CreateCommand();
                string sqlString = "DELETE FROM sys_lock_record WHERE lockhostname='" + System.Net.Dns.GetHostName() +"'";
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                string err = string.Format("���������ʱ��������\r\n\r\n������Ϣ��{0}", ex.Message);
                MessageBox.Show(err);
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڴ�����ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }


   
        /// <summary>
        /// ɾ����Ӧid��ls�ַ���
        /// </summary>
        /// <param name="id">ls�ַ�����id</param>
        /// <returns>ִ�н��</returns>
        public bool DeleteLsData(string id)
        {
            bool result = true; // ִ�н��

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("DELETE FROM {0} WHERE {1} = \'{2}\'", "sys_script_lsfile", "id", id);
                SqlCommand cmd = new SqlCommand(sqlString, conn);
                result = (cmd.ExecuteNonQuery() > 0);
            }
            catch(SqlException ex)
            {
                MessageBox.Show(string.Format("��ɾ��idΪ{0}���ַ���ʱ����sql�쳣: ", id) + ex.ToString());
                result = false;
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return result;
        }

        /// <summary>
        /// ɾ�����ݿ��еĽű�
        /// </summary>
        /// <param name="id">�ű���id</param>
        /// <returns>�Ƿ�ɾ���ɹ�</returns>
        public bool DeleteScriptData(string id)
        {
            bool result = true;
            SqlTransaction transaction = null;

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;
                string sqlString = string.Format("UPDATE {0} SET {1} = '{2}', {3} = '{4}' WHERE {5} = '{6}'", "sys_script_script",
                    "host", GetHostName(), "edittime", DateTime.Now, "id", id);
                cmd.CommandText = sqlString;

                //result = (result && (cmd.ExecuteNonQuery() == 2));
                // AffectedNum should be 2, but if not, it's not yet an error. so ...
                int AffectedNum = cmd.ExecuteNonQuery();

                sqlString = string.Format("DELETE FROM {0} WHERE {1} = {2}", "sys_script_script", "id", id);
                cmd.CommandText = sqlString;
                result = (result && (cmd.ExecuteNonQuery() == 1));
                transaction.Commit();
            }
            catch(SqlException ex)
            {
                MessageBox.Show(string.Format("��ɾ��idΪ{0}�Ľű��ļ�ʱ����sql�쳣: ", id) + ex.ToString());
                transaction.Rollback();
                result = false;
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return result;
        }


       /// <summary>
        /// ���¶�Ӧid�Ľű���·����Ϣ
        /// </summary>
        /// <param name="id">�ű���ԭ·��</param>
        /// <param name ="path">�ű�����·��</param>
        /// <returns>�Ƿ���³ɹ�</returns>
        public bool UpdateScriptPath(string oldPath, string newPath)
        {
            bool result = true; // �Ƿ���ɹ�
            SqlTransaction transaction = null; // ����

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;

                string sqlString = string.Format("SELECT id, path FROM {0}", "sys_script_script");
                DataTable scriptTable = GetDataTable(sqlString, transaction);
                foreach(DataRow r in scriptTable.Rows)
                {
                    if(!(r["path"] is DBNull))
                    {
                        string tempPath = r["path"].ToString();
                        string id = r["id"].ToString();
                        if(tempPath.StartsWith(oldPath))
                        {
                            tempPath = tempPath.Remove(0, oldPath.Length);
                            tempPath = newPath + tempPath;
                            sqlString = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE {3} = \'{4}\'", 
                                "sys_script_script", "path", tempPath, "id", id);
                            cmd.CommandText = sqlString;
                            int iAffected = cmd.ExecuteNonQuery();
                            //result = (result && (iAffected == 1));
                        }
                    }
                }

                transaction.Commit();
            }
            catch(SqlException ex)
            {
                MessageBox.Show("���½ű���·����Ϣʱ����sql�쳣: " + ex.ToString());
                if(transaction != null) // ����ع�
                {
                    transaction.Rollback();
                }
                result = false;
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return result;
        }

        /// <summary>
        /// ��ʼ��ls�ļ�hash��
        /// </summary>
        private void InitLsHashTable()
        {
            lsHashtable = new Hashtable();
            if (lsTable.Rows.Count > 0) // �����ַ�����¼
            {
                foreach (DataRow r in lsTable.Rows)
                {
                    lsHashtable[r["id"].ToString()] = r["content"].ToString();
                }
            }
        }

        /// <summary>
        /// ��ʼ��sql����
        /// </summary>
        /// <param name="connectString">�����ַ���</param>
        private void InitConnection(string connectString)
        {
            try
            {
                //connectString = "Server = jx3web; DataBase = s3design; Uid = s3design; Password = ddd12345678;";
                conn = new SqlConnection(connectString);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڴ�sql����ʱ�����쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// ��ʼ��ls���ݱ�
        /// </summary>
        private void InitLsTable()
        {
            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0}", "sys_script_lsfile");
                lsTable = GetDataTable(sqlString);
                if (lsTable.Rows.Count < 0) // �������ַ�����¼
                {
                    lsTable = null;
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڻ�ȡls�ļ�����ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// ��ʼ��ls���ݱ�
        /// </summary>
        /// <param name="classification">����</param>
        private void InitLsTable(string classification)
        {
            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0} WHERE {1} = \'{2}\'", 
                    "sys_script_lsfile", "classification", classification);
                lsTable = GetDataTable(sqlString);
                if (lsTable.Rows.Count < 0) // �������ַ�����¼
                {
                    lsTable = null;
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڻ�ȡls�ļ�����ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// �ر�sql����
        /// </summary>
        private void CloseConnection()
        {
            try
            {
                if (conn != null) // sql���ӷǿ�
                {
                    if (conn.State == ConnectionState.Open) // sql�����Ѿ���
                    {
                        conn.Close();
                    }
                }
                else
                {
                    MessageBox.Show("sql������δ��ʼ��!");
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڹر�sql����ʱ�����쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ���</param>
        /// <returns>���ݱ�</returns>
        public DataTable GetDataTable(string sqlString)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            DataTable tb = ds.Tables[0];
            return tb;
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ���</param>
        /// <param name="transaction">�������</param>
        /// <returns>���ݱ�</returns>
        public DataTable GetDataTable(string sqlString, SqlTransaction transaction)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            adp.SelectCommand.Transaction = transaction;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            DataTable tb = ds.Tables[0];
            return tb;
        }
    }
}