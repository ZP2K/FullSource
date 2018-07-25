using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Data;
using System.Collections;
using System.Text;
using System.IO;
using System.Windows.Forms;

using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

namespace FlowChartEditor
{
    public class DataBaseManager
    {
        private static DataBaseManager manager = null;  // DataBaseManager��ʵ��
        private SqlConnection conn;                     // ���ݿ�����
        private static string connectString = ""; // �������ݿ���ַ���

        //˽�й��캯�������������ⲿ��ʼ��
        private DataBaseManager() { }

        //��ȡDatabaseManager��ʵ��
        public static DataBaseManager GetDataBaseManager()
        {
            if (manager == null) // ����δ��ʼ��
            {
                manager = new DataBaseManager();
                manager.Init(connectString);
            }
            return manager;
        }

        //sql��������
        public SqlConnection Connection
        {
            get { return conn; }
        }

        //��ʼ��DataBaseManager����
        public void Init(string connectString)
        {
            try
            {
                connectString = "Server = cuiwei; DataBase = s3diagram_debug1; Uid = sa; Password = ddd12345678;";
                if(conn != null && conn.ConnectionString == connectString) // �����ַ���û�иı䣬����Ҫ�½�����
                {

                }
                else
                {
                    conn = new SqlConnection(connectString);
                }
                
            }
            catch (SqlException ex)
            {
                throw (new Exception("�ڴ�sql����ʱ�����쳣: " + ex.ToString()));
            }
        }

    #region ����أ�����������������������
        //����
        public bool Diagram_Lock(int nDiagramID)
        {
            string strHostName = System.Net.Dns.GetHostName();
            //string strIp = System.Net.Dns.GetHostEntry(System.Net.Dns.GetHostName()).AddressList[0].ToString();
            //���Ƿ񱻱�������
            DataTable dt = GetDataTable("select * from diagram where id=" + nDiagramID.ToString());
            if(dt.Rows.Count <= 0)
                throw(new Exception("����ʧ�ܣ��޴˼�¼"));
            string strWhoLocked = dt.Rows[0]["WhoLocked"].ToString();
            if (strWhoLocked != strHostName && strWhoLocked != "")
            {
                MessageBox.Show("����ʧ�ܣ��Ѿ���" + strWhoLocked + "������");
                return false;
            }
            //��ʼ����
            conn.Open();
            int nRet = 0;
            try
            {
                SqlCommand sqlcmd = new SqlCommand("update diagram set wholocked=@hostname where id=" + nDiagramID.ToString(), conn);
                sqlcmd.Parameters.Add(new SqlParameter("@hostname", strHostName));
                nRet = sqlcmd.ExecuteNonQuery();
            }
            catch(Exception ex)
            {
                throw(ex);
            }
            finally
            {
                if (conn.State == ConnectionState.Open) conn.Close();
            }
            return(nRet == 1);
        }

        //����
        public bool Diagram_Unlock(int nDiagramID)
        {
            string strHostName = System.Net.Dns.GetHostName();
            //���ǲ����Լ�����
            DataTable dt = GetDataTable("select * from diagram where id=" + nDiagramID.ToString());
            if (dt.Rows.Count <= 0)
                throw (new Exception("����ʧ�ܣ��޴˼�¼"));
            string strWhoLocked = dt.Rows[0]["WhoLocked"].ToString();
            if (strWhoLocked == "")
                throw (new Exception("����ʧ�ܣ���¼��δ����"));
            if (strWhoLocked != strHostName)
                throw (new Exception("����ʧ�ܣ���¼�Ǳ�" + strWhoLocked + "������"));
            //��ʼ����
            conn.Open();
            int nRet = 0;
            try
            {
                SqlCommand sqlcmd = new SqlCommand("update diagram set wholocked='' where id=" + nDiagramID.ToString(),conn);
                nRet = sqlcmd.ExecuteNonQuery();
            }
            catch (Exception ex)
            {
                throw (ex);
            }
            finally
            {
                if (conn.State == ConnectionState.Open) conn.Close();
            }
            return(nRet == 1);
        }

        //���� Ϊnull˵��û�����������򷵻������˵Ļ�����
        public string Diagram_WhoLocked(int nDiagramID)
        {
            DataTable dt = GetDataTable("select * from diagram where (not wholocked is NULL) and wholocked<>'' and id=" + nDiagramID.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
                return dt.Rows[0]["WhoLocked"].ToString();
        }

        //��յ�ǰ�û���������
        public void ClearRecordLocks()
        {
            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                SqlCommand cmd = conn.CreateCommand();
                string sqlString = "Update diagram set wholocked='' where wholocked=" + System.Net.Dns.GetHostName() + "'";
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
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }
    #endregion


    #region ����ͼ������أ���д����ͼ�����ӡ�ɾ�����޸�����ͼ
        //��������ͼ���ݶ���
        public object GetDiagramData(string id)
        {
            object content = null; // ����ͼ���ݶ���

            try
            {
                string sqlString = string.Format("SELECT data FROM diagram WHERE id = '{0}'", id);
                DataTable scriptTable = GetDataTable(sqlString);
                if (scriptTable.Rows.Count > 0) // ���ڶ�Ӧ�Ľű���¼
                {
                    if (scriptTable.Rows[0]["data"] is DBNull)
                    {
                        content = null;
                    }
                    else
                    {
                        IFormatter formater = new BinaryFormatter();
                        content = formater.Deserialize(new MemoryStream((byte[])scriptTable.Rows[0]["data"]));
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڻ�ȡ����ͼ���ݶ���ʱ�����쳣: " + ex.ToString());
            }

            return content;
        }

        //��������ͼ���ݶ���
        public bool SaveDiagramData(string id, object content)
        {
            bool result = true;

            //���л�����
            byte[] data = null;
            try
            {
                IFormatter formatter = new BinaryFormatter();
                Stream s = new MemoryStream();
                formatter.Serialize(s, content);
                data = new byte[s.Length];
                s.Position = 0;
                s.Read(data, 0, (int)s.Length);
                s.Close();
            }
            catch(Exception ex)
            {
                throw(ex);
            }

            //д���ݿ�
            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }
                string sqlString = string.Format("UPDATE {0} SET {1} = @data, {2} = \'{3}\', {4} = \'{5}\' WHERE {6} = \'{7}\'",
                    "diagram", "data", "edittime", DateTime.Now, "host", System.Net.Dns.GetHostName(), "id", id);
                SqlCommand cmd = new SqlCommand(sqlString, conn);
                SqlParameter spcode = new SqlParameter("@data", data);
                cmd.Parameters.Add(spcode);
                int iret = cmd.ExecuteNonQuery();
                if (iret != 1) result = false;
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڱ�������ͼʱ����sql�쳣: " + ex.ToString());
                result = false;
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
            return result;
        }

        //�����ݿⴴ���µ�����ͼ
        public string CreateNewDiagram(string path, bool isFolder)
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
                cmd.Transaction = transaction;

                //���ظ�
                cmd.CommandText = "select id from Diagram where fullpath='" + path.Replace("'", "''") + "'";
                if (cmd.ExecuteScalar() != null)
                    throw (new Exception("��������ͼʱ�����쳣�����ظ�������ͼ��"));
                
                //�����¼
                string sqlString = string.Format("INSERT INTO Diagram(FullPath,Host,EditTime,IsFolder) VALUES ('{0}','{1}','{2}','{3}')",
                    path, System.Net.Dns.GetHostName(), DateTime.Now, isFolder);
                cmd.CommandText = sqlString;
                int iret = cmd.ExecuteNonQuery();

                //��ID
                sqlString = string.Format("SELECT MAX(id) FROM Diagram");
                cmd.CommandText = sqlString;
                id = cmd.ExecuteScalar().ToString();
                transaction.Commit(); // �ύ����
            }
            catch (Exception ex)
            {
                if (transaction != null) // �ع�����
                {
                    transaction.Rollback();
                }
                throw (new Exception("�ڴ����µĽű�ʱ�����쳣: " + ex.ToString()));
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

        //��ȡ����ͼTable��������id, path, ClassifyNode
        public DataTable GetDiagramInformation()
        {
            DataTable scriptTable = null;

            try
            {
                string sqlString = "SELECT id, fullpath, isFolder as ClassifyNode from diagram";
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

            return scriptTable;
        }

        //������ͼ��path
        public bool UpdateDiagramPath(string id, string newPath)
        {
            bool result = true; // �Ƿ���ɹ�
            SqlTransaction transaction = null; // ����

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;

                cmd.CommandText = "select count(id) from diagram where id='" + id + "'";
                int iret = (int)cmd.ExecuteScalar();
                if (iret == 0) throw (new Exception("�޴�����ͼ"));

                cmd.CommandText = "select count(id) from diagram where fullpath='" + newPath + "'";
                iret = (int)cmd.ExecuteScalar();
                if (iret > 0) throw (new Exception("������ͼ·�������ƣ��ظ�"));

                cmd.CommandText = "update diagram set fullpath='" + newPath + "' where id='" + id + "'";
                iret = cmd.ExecuteNonQuery();
                transaction.Commit();
            }
            catch (SqlException ex)
            {
                MessageBox.Show("��������ͼ��·����Ϣʱ����sql�쳣: " + ex.ToString());
                if (transaction != null) // ����ع�
                {
                    transaction.Rollback();
                }
                result = false;
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return result;
        }

        /// <summary>
        /// ɾ����·���µ���������ͼ
        /// </summary>
        /// <param name="path">��ǰ·��</param>
        /// <returns>�Ƿ�ɾ���ɹ�</returns>
        public bool DeleteMultiDiagram(string path)
        {
            bool result = true; // �Ƿ�ִ�гɹ�
            SqlTransaction transaction = null; // ���ݿ�����

            try
            {
                string sqlString = string.Format("SELECT ID FROM {0} WHERE FullPath LIKE '{1}%'", "diagram", path);
                DataTable table = GetDataTable(sqlString);

                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;
                foreach(DataRow row in table.Rows)
                {
                    string id = row["ID"].ToString();
                    sqlString = string.Format("DELETE FROM {0} WHERE ID = '{1}';", "diagram", id);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }                

                transaction.Commit();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("��ɾ��·��{0}�µ�����ͼʱ����sql�쳣: ", path) + ex.ToString());
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

        //ɾ����ͼ
        public bool DeleteDiagram(string id)
        {
            bool result = true;
            SqlTransaction transaction = null;

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                //transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                //cmd.Transaction = transaction;
                string sqlString = string.Format("UPDATE {0} SET {1} = '{2}', {3} = '{4}' WHERE {5} = '{6}';", "diagram",
                    "host", System.Net.Dns.GetHostName(), "edittime", DateTime.Now, "id", id);
                sqlString += string.Format("DELETE FROM {0} WHERE {1} = '{2}';", "diagram", "id", id);
                cmd.CommandText = sqlString;
                result = (cmd.ExecuteNonQuery() == 1);
                //transaction.Commit();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("��ɾ��idΪ{0}�Ľű��ļ�ʱ����sql�쳣: ", id) + ex.ToString());
                //transaction.Rollback();
                result = false;
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return result;
        }
    #endregion

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString)
        {                
            DataTable tb = null;
            ConnectionState state = conn.State;

            try
            {
                conn.Open();
                SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                tb = ds.Tables[0];
            }
            catch(SqlException ex)
            {
                MessageBox.Show("SQL���ݿ����"+ ex.Message);
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return tb;
        }
    }
}
