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

namespace DBManager
{
    public class DataBaseManager
    {
        private static DataBaseManager dataBaseManager = null; // DataBaseManager��ʵ��

        private const string diagramTable = "Diagram"; // ��ͼ���ݱ�
        private const string diagramBackupTable = "DiagramBackup"; // ��ͼ�������ݱ�

        private const string designerToolsConfigFileName = "GameDesingerTools_Public.ini";
        private const string flowChartEditorConfigFileName = "Diagram_Public.ini";

        private SqlConnection conn; // ���ݿ�����
        private static string connectString = ""; // �������ݿ���ַ���
        private SqlConnection conn_jx3web; // jx3web���ݿ�����        

        private int maxBackupCount = 20; // ��ͼ���ݼ�¼����

        /// <summary>
        /// ˽�й��캯�������������ⲿ��ʼ��
        /// </summary>
        private DataBaseManager() 
        {
 
        }

        /// <summary>
        /// ��ȡDataBaseManager��ʵ��
        /// </summary>
        /// <returns>DataBaseManager��ʵ��</returns>
        public static DataBaseManager GetDataBaseManager()
        {
            if (dataBaseManager == null) // ����δ��ʼ��
            {
                dataBaseManager = new DataBaseManager();
                dataBaseManager.Init();

                string fileName = Path.Combine(Application.StartupPath, designerToolsConfigFileName);
                string connectString = GetConnectString(fileName, "General", "ConnString");

                if (!string.IsNullOrEmpty(connectString))
                {
                    dataBaseManager.conn_jx3web = new SqlConnection(connectString);
                }                                
            }

            return dataBaseManager;
        }

        /// <summary>
        /// ��ͼ���ݱ�����
        /// </summary>
        public string DiagramTableName
        {
            get
            {
                return diagramTable;
            }
        }

        /// <summary>
        /// sql����
        /// </summary>
        public SqlConnection Connection
        {
            get 
            { 
                return conn; 
            }
        }

        /// <summary>
        /// sql����
        /// </summary>
        public SqlConnection Connection_Jx3web
        {
            get 
            { 
                return conn_jx3web; 
            }
        }        

        /// <summary>
        /// ��ȡ�����ַ���
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="sectionName">�ֶ���</param>
        /// <param name="keyName">����</param>
        /// <returns>�����ַ���</returns>
        private static string GetConnectString(string fileName, string sectionName, string keyName)
        {            
            string content = FileFolderHelper.FileToString(fileName);
            SymmetricMethod symmetricMethod = new SymmetricMethod();
            content = symmetricMethod.Decrypto(content);

            IniStructure iniStructure = new IniStructure();
            iniStructure = IniStructure.ReadIniWithContent(content);
            connectString = iniStructure.GetValue(sectionName, keyName);

            return connectString;
        }

        /// <summary>
        /// ��ʼ��DataBaseManager����
        /// </summary>        
        private void Init()
        {
            try
            {
                string fileName = Path.Combine(Application.StartupPath, flowChartEditorConfigFileName);
                string connectString = GetConnectString(fileName, "General", "ConnString");

                if (!string.IsNullOrEmpty(connectString))
                {
                    conn = new SqlConnection(connectString);
                }           
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);                
            }
        }
     
        /// <summary>
        /// ��ͼ����
        /// </summary>
        /// <param name="diagramID">��ͼID</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        public bool Diagram_Lock(int diagramID)
        {
            bool lockSuccess = true;
            string hostName = System.Net.Dns.GetHostName();
            
            // ���Ƿ񱻱�������
            string sqlString = string.Format("SELECT WhoLocked FROM {0} WHERE ID = {1}", diagramTable, diagramID);
            DataTable infoTable = GetDataTable(sqlString);

            if (infoTable.Rows.Count > 0)
            {
                string strWhoLocked = infoTable.Rows[0]["WhoLocked"].ToString();

                // �Ѿ����˼����ˣ�ͨ���쳣���ؼ����˵���Ϣ�����Ƽ�������д����
                if (strWhoLocked != hostName && strWhoLocked != "")
                {
                    throw new Exception(strWhoLocked);               
                }

                sqlString = string.Format("UPDATE {0} SET WhoLocked = '{1}' WHERE ID = {2}", diagramTable, hostName, diagramID);
                lockSuccess = ExecuteSqlCommand(sqlString);
            }
            else 
            {
                lockSuccess = false;
            }            

            return lockSuccess;
        }
        
        /// <summary>
        /// ��ͼ����
        /// </summary>
        /// <param name="diagramID">��ͼID</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        public bool Diagram_Unlock(int diagramID)
        {
            bool unlockSuccess = true;
            string hostName = System.Net.Dns.GetHostName();
            string sqlString = string.Format("SELECT WhoLocked FROM {0} WHERE ID = {1}", diagramTable, diagramID);
            DataTable infoTable = GetDataTable(sqlString);

            if (infoTable.Rows.Count > 0)
            {
                string lockHostName = infoTable.Rows[0]["WhoLocked"].ToString();

                if (lockHostName == hostName)
                {
                    sqlString = string.Format("UPDATE {0} SET WhoLocked = NULL WHERE ID = {1}", diagramTable, diagramID);
                    ExecuteSqlCommand(sqlString);
                }                
            }
            else
            {
                unlockSuccess = false;
            }            

            return unlockSuccess;
        }

        /// <summary>
        /// ����Ƿ�����������ͼ
        /// </summary>
        /// <param name="diagramID">��ͼID</param>
        /// <returns>�����˵Ļ�����</returns>
        public string Diagram_WhoLocked(int diagramID)
        {
            string lockHostName = null;
            string sqlString = string.Format("SELECT WhoLocked FROM {0} WHERE ID = {1}", diagramTable, diagramID);
            DataTable infoTable = GetDataTable(sqlString);

            if (infoTable.Rows.Count > 0)
            {
                if (!string.IsNullOrEmpty(infoTable.Rows[0]["WhoLocked"].ToString()))
                {
                    lockHostName = infoTable.Rows[0]["WhoLocked"].ToString();
                }
            }            

            return lockHostName;
        }

        /// <summary>
        /// ��յ�ǰ�û���������
        /// </summary>
        public void ClearRecordLocks()
        {
            string hostName = System.Net.Dns.GetHostName();
            string sqlString = string.Format("UPDATE {0} SET WhoLocked = NULL WHERE WhoLocked = '{1}'", diagramTable, hostName);

            ExecuteSqlCommand(sqlString);
        }

        /// <summary>
        /// ����û���Ȩ
        /// </summary>
        /// <param name="userName">�û���</param>
        /// <param name="ipAddress">�û�IP</param>
        /// <returns>�Ƿ�ӵ����Ȩ</returns>
        public bool CheckAuthority(string userName, string ipAddress)
        {
            bool hasAuthority = false;

            try
            {
                if (conn_jx3web.State == ConnectionState.Closed)
                {
                    conn_jx3web.Open();
                }

                SqlCommand cmd = conn_jx3web.CreateCommand();
                string sqlString = string.Format("SELECT _index FROM sys_useracl WHERE userid = '{0}' AND ip = '{1}'",
                                                 userName, ipAddress);
                cmd.CommandText = sqlString;
                object executeResult = cmd.ExecuteScalar();

                if (executeResult != null && !(executeResult is DBNull))
                {
                    hasAuthority = true;
                }
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);
            }
            finally
            {
                if (conn_jx3web.State == ConnectionState.Open)
                {
                    conn_jx3web.Close();
                }
            }

            return hasAuthority;
        }

        /// <summary>
        /// �����л�����
        /// </summary>
        /// <param name="data">����</param>
        /// <returns>���</returns>
        private object DeserializeData(byte[] data)
        {
            object content = null;

            try
            {
                IFormatter formater = new BinaryFormatter();
                content = formater.Deserialize(new MemoryStream(data));
            }
            catch (System.Exception ex)
            {
                PrintExceptionInformation(ex);
            }            

            return content;
        }
        
        /// <summary>
        /// ��ȡ��ͼ����
        /// </summary>
        /// <param name="id">��ͼID</param>
        /// <returns>�Ƿ��ȡ�ɹ�</returns>
        public object GetDiagramData(string id)
        {
            string sqlString = string.Format("SELECT Data FROM {0} WHERE ID = {1}", diagramTable, id);
            object executeResult = ExecuteSqlSelectCommand(sqlString);
            object content = null;

            if (executeResult != null && !(executeResult is DBNull))
            {
                content = DeserializeData((byte[])executeResult);
            }

            return content;
        }

        /// <summary>
        /// ��ȡ��ͼ����
        /// </summary>
        /// <param name="path">��ͼ·��</param>
        /// <param name="diagramClass">��ͼ����</param>
        /// <returns>�Ƿ��ȡ�ɹ�</returns>
        public object GetDiagramDataByPath(string path, string diagramClass)
        {

            string sqlString = string.Format("SELECT Data FROM {0} WHERE FullPath = '{1}' AND Class = '{2}'",
                                             diagramTable, path, diagramClass);
            object executeResult = ExecuteSqlSelectCommand(sqlString);
            object content = null; // ��ͼ���ݶ���

            if (executeResult != null && !(executeResult is DBNull))
            {
                content = DeserializeData((byte[])executeResult);
            }            

            return content;
        }

        /// <summary>
        /// ���ػ�ͼ��������
        /// </summary>
        /// <param name="backupID">��ͼ����ID</param>
        /// <returns>�Ƿ���سɹ�</returns>
        public object GetBackupDiagramData(string backupID)
        {
            string sqlString = string.Format("SELECT Data FROM {0} WHERE ID = {1}", diagramBackupTable, backupID);
            object executeResult = ExecuteSqlSelectCommand(sqlString);
            object content = null; // ����ͼ���ݶ���

            if (executeResult != null && !(executeResult is DBNull))
            {
                content = DeserializeData((byte[])executeResult);
            }

            return content;
        }

        /// <summary>
        /// ���ƻ�ͼ������
        /// </summary>
        /// <param name="originID">Դ��ͼ��ID</param>
        /// <param name="destinID">Ŀ���ͼ��ID</param>
        /// <returns>�Ƿ��Ƴɹ�</returns>
        public bool CopyDiagramData(string originID, string destinID)
        {
            string sqlString = string.Format("UPDATE {0} SET Data = (SELECT Data FROM {0} WHERE ID = '{1}') WHERE ID = '{2}'",
                                             diagramTable, destinID, originID);
            bool executeSuccess = ExecuteSqlCommand(sqlString);

            return executeSuccess;
        }

        /// <summary>
        /// �ָ���ͼ������
        /// </summary>
        /// <param name="originID">ԭ��ͼ��ID</param>
        /// <param name="backupID">���ݻ�ͼ��ID</param>
        /// <returns>�Ƿ�ָ��ɹ�</returns>
        public bool RestoreDiagramData(string originID, string backupID)
        {
            bool result = true;
            SqlTransaction transaction = null;
            string hostName = System.Net.Dns.GetHostName();

            // д���ݿ�
            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;

                // ��ȡ�汾��
                string sqlString = string.Format("SELECT Version FROM {0} WHERE ID = {1}", diagramTable, originID);
                cmd.CommandText = sqlString;
                int currentVersion = 1;
                object version = cmd.ExecuteScalar();

                if (version != null && !(version is DBNull))
                {
                    currentVersion = (int)version + 1;
                }

                // ���»�ͼ��Ϣ
                sqlString = string.Format("UPDATE {0} SET Data = (SELECT Data FROM {1} WHERE ID = {2}), Version = {3}, Host = '{4}', EditTime = getdate() WHERE ID = {5}",
                                          diagramTable, diagramBackupTable, backupID, currentVersion, hostName, originID);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();

                transaction.Commit();
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);
                transaction.Rollback();
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
        /// ���л�����
        /// </summary>
        /// <param name="content">����</param>
        /// <returns>���</returns>
        private byte[] SerializeData(object content)
        {
            byte[] data = null;

            try
            {
                IFormatter formatter = new BinaryFormatter();
                Stream memoryStream = new MemoryStream();
                formatter.Serialize(memoryStream, content);
                data = new byte[memoryStream.Length];
                memoryStream.Position = 0;
                memoryStream.Read(data, 0, (int)memoryStream.Length);
                memoryStream.Close();
            }
            catch (SerializationException ex)
            {
                PrintExceptionInformation(ex);              
            }

            return data;
        }

        /// <summary>
        /// ����ͼ���ݱ��浽�ļ�
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <param name="content">��ͼ����</param>
        /// <returns>�Ƿ񱣴�ɹ�</returns>
        public bool SaveDiagramDataToFile(string fileName, object content)
        {
            bool saveSuccess = false;
            byte[] data = SerializeData(content);

            if (data != null)
            {
                string folderName = Path.GetDirectoryName(fileName); // ��ȡĿ¼

                if (!string.IsNullOrEmpty(folderName) && !Directory.Exists(folderName)) // Ŀ¼������
                {
                    Directory.CreateDirectory(folderName); // �ȴ���Ŀ¼���ٴ����ļ�
                }

                File.WriteAllBytes(fileName, data);
                saveSuccess = true;
            }

            return saveSuccess;
        }

        /// <summary>
        /// ���ļ��м��ػ�ͼ����
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <returns>��ͼ����</returns>
        public object LoadDiagramDataFromFile(string fileName)
        {
            object data = null;

            if (File.Exists(fileName))
            {
                try
                {
                    byte[] buffer = File.ReadAllBytes(fileName);
                    data = DeserializeData(buffer);
                }
                catch (Exception ex)
                {
                    PrintExceptionInformation(ex);
                }                
            }

            return data;
        }

        /// <summary>
        /// �����ͼ
        /// </summary>
        /// <param name="path">��ͼid</param>
        /// <param name="diagramClass">��ͼ����</param>
        /// <param name="content">��ͼ����</param>
        /// <returns>�Ƿ񱣴�ɹ�</returns>
        public bool SaveDiagramData(string path, string diagramClass, object content)
        {
            bool saveSuccess = true;
            byte[] data = SerializeData(content);

            if (data != null)
            {
                // д���ݿ�
                try
                {
                    if (conn.State == ConnectionState.Closed) // ��sql����
                    {
                        conn.Open();
                    }

                    // ���»�ͼ��Ϣ
                    string sqlString = string.Format("UPDATE {0} SET Data = @data WHERE FullPath = '{1}' AND Class = '{2}'",
                                                     diagramTable, path, diagramClass);
                    SqlCommand cmd = conn.CreateCommand();
                    cmd.CommandText = sqlString;
                    SqlParameter spcode = new SqlParameter("@data", data);
                    cmd.Parameters.Add(spcode);
                    cmd.ExecuteNonQuery();
                }
                catch (SqlException ex)
                {
                    PrintExceptionInformation(ex);
                    saveSuccess = false;
                }
                finally
                {
                    if (conn.State == ConnectionState.Open) // �ر�sql����
                    {
                        conn.Close();
                    }
                }
            }
            else
            {
                saveSuccess = false;
            }

            return saveSuccess;
        }

        /// <summary>
        /// �����ͼ
        /// </summary>
        /// <param name="id">��ͼid</param>
        /// <param name="content">��ͼ����</param>
        /// <returns>�Ƿ񱣴�ɹ�</returns>
        public bool SaveDiagramData(string id, object content)
        {
            bool saveSuccess = true;
            string hostName = System.Net.Dns.GetHostName(); 
            SqlTransaction transaction = null;
            byte[] data = SerializeData(content);
            
            if (data != null)
            {
                // д���ݿ�
                try
                {
                    if (conn.State == ConnectionState.Closed) // ��sql����
                    {
                        conn.Open();
                    }

                    transaction = conn.BeginTransaction();
                    SqlCommand cmd = conn.CreateCommand();
                    cmd.Transaction = transaction;

                    // ��ȡ�汾��
                    string sqlString = string.Format("SELECT Version FROM {0} WHERE ID = {1}", diagramTable, id);
                    cmd.CommandText = sqlString;
                    int currentVersion = 1;
                    object version = cmd.ExecuteScalar();

                    if(version != null && !(version is DBNull))
                    {
                        currentVersion = (int)version + 1;
                    }

                    sqlString = string.Format("SELECT FullPath FROM {0} WHERE ID = {1}", diagramTable, id);
                    cmd.CommandText = sqlString;
                    string fullPath = cmd.ExecuteScalar() as string;

                    sqlString = string.Format("SELECT Class FROM {0} WHERE ID = {1}", diagramTable, id);
                    cmd.CommandText = sqlString;
                    string diagramClass = cmd.ExecuteScalar() as string;

                    // ���»�ͼ��Ϣ
                    sqlString = string.Format("UPDATE {0} SET Data = @data, EditTime = getdate(), Host = '{1}', Version = {2} WHERE ID = {3}",
                                              diagramTable, hostName, currentVersion, id);
                    cmd.CommandText = sqlString;
                    SqlParameter spcode = new SqlParameter("@data", data);
                    cmd.Parameters.Add(spcode);
                    cmd.ExecuteNonQuery();

                    // ���ݻ�ͼ��Ϣ
                    sqlString = string.Format("INSERT INTO {0} (OriginID, EditTime, Data, Class, Host, Version, FullPath) VALUES ('{1}', getdate(), @data, '{2}', '{3}', '{4}', '{5}')",
                                              diagramBackupTable, id, diagramClass, hostName, currentVersion, fullPath);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    // ��鱸����Ϣ�Ƿ���Ҫ����
                    sqlString = string.Format("SELECT COUNT(ID) FROM {0} WHERE OriginID = {1}", diagramBackupTable, id);
                    cmd.CommandText = sqlString;
                    object executeResult = cmd.ExecuteScalar();

                    // ��Ҫ��������Ϣ
                    if ((int)executeResult > maxBackupCount * 2)
                    {
                        sqlString = string.Format("DELETE FROM {0} WHERE OriginID = {2} AND ID NOT IN (SELECT TOP {1} ID FROM {0} WHERE OriginID = {2} ORDER BY EditTime DESC)",
                                                  diagramBackupTable, maxBackupCount, id);
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                    }

                    transaction.Commit();
                }
                catch (SqlException ex)
                {
                    PrintExceptionInformation(ex);
                    transaction.Rollback();
                    saveSuccess = false;
                }
                finally
                {
                    if (conn.State == ConnectionState.Open) // �ر�sql����
                    {
                        conn.Close();
                    }
                }
            }
            else
            {
                saveSuccess = false;
            }            

            return saveSuccess;
        }

        /// <summary>
        /// �����µ�����ͼ
        /// </summary>
        /// <param name="path">����ͼ·��</param>
        /// <param name="isFolder">�Ƿ����ļ���</param>
        /// <param name="diagramClass">����ͼ����</param>
        /// <returns>����ͼ��id</returns>
        public string CreateNewDiagram(string path, bool isFolder, string diagramClass)
        {
            string id = null; // ����ͼ��id
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
                cmd.CommandText = string.Format("SELECT ID FROM {0} WHERE FullPath = '{1}' AND Class = '{2}' AND Hide <> 1",
                                                diagramTable, path.Replace("'", "''"), diagramClass);
                
                if (cmd.ExecuteScalar() != null)
                {
                    MessageBox.Show("�����ƵĻ�ͼ�Ѿ����ڣ�", "�½���ͼ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    string hostName = System.Net.Dns.GetHostName();

                    // �����¼
                    string sqlString = string.Format("INSERT INTO {0} (FullPath, Host, EditTime, IsFolder, Class) VALUES ('{1}', '{2}', getdate(), '{3}', '{4}')",
                                                     diagramTable, path, hostName, isFolder, diagramClass);
                    cmd.CommandText = sqlString;
                    int iret = cmd.ExecuteNonQuery();

                    // ��ID
                    sqlString = string.Format("SELECT MAX(ID) FROM {0}", diagramTable);
                    cmd.CommandText = sqlString;
                    id = cmd.ExecuteScalar().ToString();
                    transaction.Commit(); // �ύ����
                }                                
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);

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
        /// ��ȡ��ͼ��Ϣ
        /// </summary>
        /// <param name="chartClass">��ͼ����</param>
        /// <returns>��ͼ��Ϣ</returns>
        public DataTable GetDiagramInformation(string chartClass)
        {
            DataTable scriptTable = null;

            try
            {
                string sqlString = string.Format("SELECT ID, FullPath, IsFolder, Class, EditTime, Host, Version From {0} WHERE Class = '{1}' AND (Hide IS NULL OR Hide = 0)", 
                                                 diagramTable, chartClass);

                if (string.IsNullOrEmpty(chartClass))
                {
                    sqlString = string.Format("SELECT ID, FullPath, IsFolder, Class, EditTime, Host, Version From {0} WHERE Hide IS NULL OR Hide = 0",
                                              diagramTable);
                }

                scriptTable = GetDataTable(sqlString);

                if (scriptTable.Rows.Count == 0) // �����ڽű���¼
                {
                    scriptTable = null;
                }
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);
            }

            return scriptTable;
        }
        
        /// <summary>
        /// �޸Ļ�ͼ��·��
        /// </summary>
        /// <param name="id">��ͼ��ID</param>
        /// <param name="newPath">�µĻ�ͼ·��</param>
        /// <returns>�Ƿ��޸ĳɹ�</returns>
        public bool UpdateDiagramPath(string id, string newPath)
        {
            string sqlString = string.Format("UPDATE {0} SET FullPath = '{1}' WHERE ID = {2}",
                                             diagramTable, newPath, id);
            bool executeSuccess = ExecuteSqlCommand(sqlString);

            return executeSuccess;
        }

        /// <summary>
        /// ɾ����ͼ
        /// </summary>
        /// <param name="id">��ͼ��ID</param>
        /// <returns>�Ƿ�ɾ���ɹ�</returns>
        public bool DeleteDiagram(string id)
        {            
            string sqlString = string.Format("DELETE FROM {0} WHERE ID = {1}", diagramTable, id);
            // string sqlString = string.Format("UPDATE {0} SET Hide = 1 WHERE ID = {1}", diagramTable, id);

            bool executeSuccess = ExecuteSqlCommand(sqlString);

            return executeSuccess;
        }

        /// <summary>
        /// ɾ����·���µ����л�ͼ
        /// </summary>
        /// <param name="path">��ǰ·��</param>
        /// <returns>�Ƿ�ɾ���ɹ�</returns>
        public bool DeleteMultiDiagram(string path)
        {
            bool result = true; // �Ƿ�ִ�гɹ�
            SqlTransaction transaction = null; // ���ݿ�����

            try
            {
                string sqlString = string.Format("SELECT ID FROM {0} WHERE FullPath LIKE '{1}%'", diagramTable, path);
                DataTable table = GetDataTable(sqlString);

                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;

                foreach (DataRow row in table.Rows)
                {
                    string id = row["ID"].ToString();
                    sqlString = string.Format("DELETE FROM {0} WHERE ID = {1}", diagramTable, id);
                    // sqlString = string.Format("UPDATE {0} SET Hide = 1 WHERE ID = {1}", diagramTable, id);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }

                transaction.Commit();
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);
                transaction.Rollback();
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
        /// ��ȡ��ͼ�ı༭ʱ��
        /// </summary>
        /// <param name="id">��ͼ��ID</param>
        /// <returns>��ͼ�ı༭ʱ��</returns>
        public DateTime GetDiagramEditTime(string id)
        {
            DateTime editTime = DateTime.MinValue;
            string sqlString = string.Format("SELECT EditTime FROM {0} WHERE ID = {1}", diagramTable, id);
            object executeResult = ExecuteSqlSelectCommand(sqlString);

            if (executeResult != null && !(executeResult is DBNull))
            {
                editTime = (DateTime)executeResult;
            }

            return editTime;
        }

        /// <summary>
        /// ��ȡ��ͼ���ݵı༭ʱ��
        /// </summary>
        /// <param name="backupID">��ͼ���ݵ�ID</param>
        /// <returns>��ͼ���ݵı༭ʱ��</returns>
        public DateTime GetBackupDiagramEditTime(string backupID)
        {
            DateTime editTime = DateTime.MinValue;
            string sqlString = string.Format("SELECT EditTime FROM {0} WHERE ID = {1}", diagramBackupTable, backupID);
            object executeResult = ExecuteSqlSelectCommand(sqlString);

            if (executeResult != null && !(executeResult is DBNull))
            {
                editTime = (DateTime)executeResult;
            }

            return editTime;
        }

        /// <summary>
        /// ��ȡ��ͼ�ı༭�汾
        /// </summary>
        /// <param name="id">��ͼ��ID</param>
        /// <returns>��ͼ�ı༭�汾</returns>
        public int GetDiagramVersion(string id)
        {
            int version = 0;
            string sqlString = string.Format("SELECT Version FROM {0} WHERE ID = {1}", diagramTable, id);
            object executeResult = ExecuteSqlSelectCommand(sqlString);

            if (executeResult != null && !(executeResult is DBNull))
            {                
                version = (int)executeResult;               
            }

            return version;
        }

        /// <summary>
        /// ��ȡ��ͼ���ݵı༭�汾
        /// </summary>
        /// <param name="backupID">��ͼ��ID</param>
        /// <returns>��ͼ�ı༭�汾</returns>
        public int GetBackupDiagramVersion(string backupID)
        {
            int version = 0;
            string sqlString = string.Format("SELECT Version FROM {0} WHERE ID = {1}", diagramBackupTable, backupID);
            object executeResult = ExecuteSqlSelectCommand(sqlString);

            if (executeResult != null && !(executeResult is DBNull))
            {
                version = (int)executeResult;
            }

            return version;
        }

        /// <summary>
        /// ��ȡ����ͼ�ı༭��
        /// </summary>
        /// <param name="id">����ͼ��id</param>
        /// <returns>����ͼ�ı༭��</returns>
        public string GetDiagramEditor(string id)
        {
            string userName = "";
            string sqlString = string.Format("SELECT Host FROM {0} WHERE ID = {1}", diagramTable, id);
            object executeResult = ExecuteSqlSelectCommand(sqlString);

            if (executeResult != null && !(executeResult is DBNull))
            {
                userName = executeResult as string;
            }

            return userName;
        }

        /// <summary>
        /// ��ȡ���еĵ�ͼ����
        /// </summary>
        /// <returns>���еĵ�ͼ����</returns>
        public List<string> GetMapList()
        {
            List<string> list = new List<string>();

            SqlConnection j3web_conn = GetJx3webConnection();
            string sqlString = "SELECT Name FROM MapList";
            DataTable table = GetDataTable(sqlString, j3web_conn);

            foreach(DataRow dataRow in table.Rows)
            {
                list.Add(dataRow[0].ToString());
            }

            return list;
        }

        /// <summary>
        /// ��ȡ�û���������Ϣ
        /// </summary>
        /// <param name="ip">�û���IP</param>
        /// <returns>�û���������Ϣ</returns>
        public object GetUserConfigInfo(string ip)
        {
            object info = null;
            string sqlString = string.Format("SELECT Data FROM UserConfigInfo WHERE IP = '{0}'", ip);
            object executeResult = ExecuteSqlSelectCommand(sqlString);

            if (executeResult != null && !(executeResult is DBNull))
            {
                info = DeserializeData((byte[])executeResult);                
            }

            return info;
        }

        /// <summary>
        /// ����û���Ϣ�Ƿ����
        /// </summary>
        /// <returns>�û���IP</returns>
        public bool CheckUserInfoExist(string ip)
        {
            string sqlString = string.Format("SELECT HostName FROM UserConfigInfo WHERE IP = '{0}'", ip);
            DataTable table = GetDataTable(sqlString);
            bool exist = table.Rows.Count > 0;

            return exist;
        }

        /// <summary>
        /// ����û���Ϣ
        /// </summary>
        /// <param name="ip">�û���IP</param>
        /// <param name="hostName">�û�������</param>
        /// <param name="info">������Ϣ</param>
        /// <returns>�Ƿ���ӳɹ�</returns>
        public bool AddUserConfigInfo(string ip, string hostName, object info)
        {
            bool result = true;

            //���л�����
            byte[] data = null;
            try
            {
                IFormatter formatter = new BinaryFormatter();
                Stream s = new MemoryStream();
                formatter.Serialize(s, info);
                data = new byte[s.Length];
                s.Position = 0;
                s.Read(data, 0, (int)s.Length);
                s.Close();
            }
            catch (SerializationException ex)
            {
                MessageBox.Show("�����л��û�������Ϣʱ�����쳣: " + ex.ToString());
            }

            //д���ݿ�
            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("INSERT INTO UserConfigInfo (IP, HostName, Data) VALUES ('{0}', '{1}', @data)",
                                                 ip, hostName);
                SqlCommand cmd = new SqlCommand(sqlString, conn);
                SqlParameter spcode = new SqlParameter("@data", data);
                cmd.Parameters.Add(spcode);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);
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
        /// �����û���������Ϣ
        /// </summary>
        /// <param name="ip">�û���IP</param>
        /// <param name="info">�û���������Ϣ</param>
        /// <returns>�Ƿ����óɹ�</returns>
        public bool SetUserConfigInfo(string ip, object info)
        {
            bool result = true;
            byte[] data = SerializeData(info);

            //д���ݿ�
            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("UPDATE UserConfigInfo SET Data = @data WHERE IP = '{0}'",
                                                 ip);
                SqlCommand cmd = new SqlCommand(sqlString, conn);
                SqlParameter spcode = new SqlParameter("@data", data);
                cmd.Parameters.Add(spcode);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);
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

    #region ���ࡢ�¼���API�����ⳣ��
        //��ѯ����
        public object[] GetTypeDefine(int id)
        {
            DataTable dt = GetDataTable("select * from typedefine where id=" + id.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
                return new object[] 
                {
                    dt.Rows[0]["Description"],
                    dt.Rows[0]["IsDuplicate"],
                    dt.Rows[0]["IsEnum"],
                    dt.Rows[0]["QueryScript"],
                    dt.Rows[0]["GenAsString"]
                };
        }

        //��ѯ�¼���Ϣ �������֡�����
        public object[] GetEventDefine(int id)
        {
            DataTable dt = GetDataTable("select * from eventdefine where id=" + id.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
                return new object[] 
                {
                    dt.Rows[0]["Name"],
                    dt.Rows[0]["Description"],
                    dt.Rows[0]["MainClass"]
                };
        }

        //��ѯ�¼�����
        public DataTable GetEventDefineArg(int id)
        {
            string strSQL = @"
declare @xml xml
select @xml=arglist from eventdefine where id = {0}
select 
c.value('@id','nvarchar(MAX)') as 'id' ,
c.value('@Type','nvarchar(MAX)') as 'type',
c.value('@Text','nvarchar(MAX)') as 'name',
c.value('@Name','nvarchar(MAX)') as 'default'
from @xml.nodes('/ArgList/Arg') as n(c)
";
            strSQL = string.Format(strSQL, id.ToString());
            DataTable dt = GetDataTable(strSQL);
            if (dt.Rows.Count == 0)
                return null;
            else
                return dt;
        }

        //��ѯAPI��Ϣ �������֡�������
        public object[] GetAPIDefine(int id)      
        {
            DataTable dt = GetDataTable("select (class + '--' + text) as text,Description,rettype,name,isasyn,tip from ApiDefine where id <> 1 and id=" + id.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
                return new object[] 
                {
                    dt.Rows[0]["Text"],
                    dt.Rows[0]["Description"],
                    dt.Rows[0]["rettype"],
                    dt.Rows[0]["name"],
                    dt.Rows[0]["isasyn"],
                    dt.Rows[0]["tip"]
                };
        }

        //��ѯAPI����
        public DataTable GetAPIDefineArg(int id)
        {
            string strSQL = @"
declare @xml xml
select @xml=arglist from ApiDefine where id <> 1 and id = {0}
select 
c.value('@id','nvarchar(MAX)') as 'id' ,
c.value('@Type','nvarchar(MAX)') as 'type',
c.value('@Text','nvarchar(MAX)') as 'name',
c.value('@Name','nvarchar(MAX)') as 'default'
from @xml.nodes('/ArgList/Arg') as n(c)
";
            strSQL = string.Format(strSQL, id.ToString());
            DataTable dt = GetDataTable(strSQL);
            if (dt.Rows.Count == 0)
                return null;
            else
                return dt;
        }

        //���ݷ���ֵ��API�ǵ�ID
        public int[] GetAPIsByRetType(int typeid)
        {
            DataTable dt = GetDataTable("select id from ApiDefine where id <> 1 and rettype=" + typeid.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
            {
                int[] ret = new int[dt.Rows.Count];
                for(int i = 0; i < ret.Length; i++)
                {
                    ret[i] = Convert.ToInt32(dt.Rows[i]["id"].ToString());
                }
                return ret;
            }
        }

        //������API�ǵ�ID
        public int[] GetAllAPIs()
        {
            DataTable dt = GetDataTable("select id from ApiDefine where id <> 1");
            if (dt.Rows.Count == 0)
                return null;
            else
            {
                int[] ret = new int[dt.Rows.Count];
                for(int i = 0; i < ret.Length; i++)
                {
                    ret[i] = Convert.ToInt32(dt.Rows[i]["id"].ToString());
                }
                return ret;
            }
        }


        //��API�ǵ�ID������ֵ�����ƶ����͵ġ�
        public int[] GetAPIs_Except_RetType(int typeid)
        {
            DataTable dt = GetDataTable("select id from ApiDefine where id <> 1 and (not rettype='" + typeid.ToString() + "')");
            if (dt.Rows.Count == 0)
                return null;
            else
            {
                int[] ret = new int[dt.Rows.Count];
                for (int i = 0; i < ret.Length; i++)
                {
                    ret[i] = Convert.ToInt32(dt.Rows[i]["id"].ToString());
                }
                return ret;
            }
        }

        //�����¼��б�
        public int[] GetEvents()
        {
            DataTable dt = GetDataTable("select id from eventdefine");
            if (dt.Rows.Count == 0)
                return null;
            else
            {
                int[] ret = new int[dt.Rows.Count];
                for (int i = 0; i < ret.Length; i++)
                {
                    ret[i] = Convert.ToInt32(dt.Rows[i]["id"].ToString());
                }
                return ret;                
            }
        }

        //���ݷ���ֵ��Const�ǵ�DBValue
        public string[] GetConstDBValuesByRetType(int typeid)
        {
            DataTable dt = GetDataTable("select value from constant where attachedtype=" + typeid.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
            {
                string[] ret = new string[dt.Rows.Count];
                for(int i = 0; i < ret.Length; i++)
                {
                    ret[i] = dt.Rows[i]["Value"].ToString();
                }
                return ret;
            }
        }

        //�ӱ����ѯ����������
        public string GetConstText(int nTypeID, string dbValue)
        {
            DataTable dt = GetDataTable("select text from constant where attachedType='" + nTypeID.ToString() + "' and value='" + dbValue + "'");
            if (dt.Rows.Count == 0)
                return nTypeID.ToString() + "<�Ҳ�����������>";
            else
                return dt.Rows[0]["text"].ToString();
            
        }

    #endregion


    #region ��JX3������ʹ�ã�����ϵͳ�ļ�
        //��ȡAPI�ļ�����
        public string GetAPIFileContext()
        {
            string sql = @"declare @ret varchar(MAX)
set @ret = ''
select @ret=@ret+
case when script is NUll then ''
else
    script
end + '
'
from apidefine
select @ret";
            string strTemp = @"Include('scripts/flowlib/lib.lua')
Include('scripts/flowlib/event_filter.lua')
using('comgr')
declare('API', {})
";
            return strTemp + GetConstTextFromSQL(sql, this.conn);
        }

        //��ȡ�¼��ɷ����ļ�����
        public string GetEventDispatchFileContext()
        {
            string sql = @"declare @ret varchar(MAX)
set @ret = ''
declare cur CURSOR
STATIC
FOR select id, Name from baseevent
open cur
declare @rows int, @i int
declare @name varchar(max), @id int
set @i = 0
set @rows = @@cursor_rows
while @i<@rows
begin
	fetch next from cur into @id, @name
	set @ret=@ret + 'EventDispatch.A' + @name + ' = function(key, ...)
    local bret = false
'
	select @ret = @ret + w from
	(
	select '	bret = bret or EventDispatch.AOnAllEvent(' + cast(id as varchar(max)) + ',  key, ...)
'  w from eventdefine where attachedbaseevent = @id
	) t
	set @ret = @ret + '    return bret
end

'
	set @i = @i + 1
end
close cur
deallocate cur
select @ret";
            string strTemp = @"Include('scripts/flowlib/api.lua')
Include('scripts/flowlib/event_filter.lua')
Include('scripts/flowlib/regist_file.lua')
declare('EventDispatch', {})
using('Event')
EventDispatch.AOnAllEvent = function(eventid, key, ...)
    local bret = false
	env = Event[eventid]
    if not env then
        trace('not found event implement: ' .. eventid)
        return bret
    end
    if env.FunList[key] ~= nil then
        trace('got key:', key)
        for k, v in pairs(env.FunList[key]) do
            local ta = {}
            for i = 1, #arg do
                ta[i] = arg[i]
            end
            for i = 1, #v do
                ta[i +#arg] = v[i]
            end
            if env.Condition(unpack(ta)) then
                trace(' do event: ', eventid, key)
                bret = true
                event_dispatch(k, env.Args(unpack(ta)))
            else
                trace(' not do event: ', eventid, key)
            end
        end
    else
	    trace('not got key', key)
    end
    return bret
end

";
            return strTemp + GetConstTextFromSQL(sql, this.conn);
        }

        //��ȡ�¼��ɷ����ļ�����
        public string GetEventFilterFileContext()
        {
            string sql = @"declare @ret varchar(MAX)
set @ret = ''
select @ret=@ret+
case when script is NUll then ''
else
    script
end + '
'
from eventdefine
select @ret";
            string strTemp = @"Include('scripts/flowlib/lib.lua')
using('comgr')
declare('Event', {})

";
            return strTemp + GetConstTextFromSQL(sql, this.conn);
        }

        public SqlConnection GetJx3webConnection()
        {
            return this.conn_jx3web;
        }

        //Lua�ú���
        public SqlConnection NewCustomConnection(string strConnStr)
        {
            SqlConnection conn = new SqlConnection(strConnStr); 
            return conn;
        }

        //����ѯ����������
        public string GetConstTextFromSQL(string strSQL,SqlConnection conn)
        {
            DataTable dt = GetDataTable(strSQL, conn);
            if (dt.Rows.Count == 0)
                return null;
            else
                return dt.Rows[0][0].ToString();
        }

    #endregion


    #region AIͼ���
        /// <summary>
        /// ��ѯAI�¼�����ȫ��
        /// </summary>
        /// <returns>AI�¼�����ȫ��</returns>
        public DataTable GetTable_AI_Event_Define()
        {
            string sql = "select * from AI_Event_Define";
            DataTable dt = GetDataTable(sql, conn_jx3web);

            return dt;
        }

        /// <summary>
        /// ��ѯAI Action����ȫ��
        /// </summary>
        /// <returns>AI Action����ȫ��</returns>
        public DataTable GetTable_AI_Action_Define()
        {
            string sql = "SELECT id, actionid, name, ename, info, class, parms, rettype FROM AI_Action_Define ORDER BY class";
            DataTable dt = GetDataTable(sql, conn_jx3web);

            return dt;
        }

        /// <summary>
        /// ��ѯAIö�ٶ���ȫ��
        /// </summary>
        /// <returns>AIö�ٶ���ȫ��</returns>
        public DataTable GetTable_AI_Enum_Define()
        {
            string sql = "select * from AI_Enum_Define";
            DataTable dt = GetDataTable(sql, conn_jx3web);

            return dt;
        }        
       
        /// <summary>
        /// ��ȡAI�Զ��嶯��������
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <returns>�Զ��嶯��������</returns>
        public string GetUserActionContext(SqlConnection conn)
        {
            string sql = "SELECT script, ename, parms FROM ai_action_define WHERE actionid > 1000";
            StringBuilder strRet = new StringBuilder();
            strRet.AppendLine("-- �Զ���Action");
            strRet.AppendLine("Include('scripts/ai/argumentStrings.ls')");
            strRet.AppendLine();
            DataTable dt = GetDataTable(sql, conn);

            foreach (DataRow dr in dt.Rows)
            {
                string str = "function {0}(character, actionData)\r\n        local p1, p2, p3, p4, p5 = actionData.GetParam();\r\n";
                strRet.Append(string.Format(str, dr["ename"].ToString()));
                string[] strPList = dr["parms"].ToString().Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);

                for(int i = 1; i <= strPList.Length; i++)
                {
                    string l = strPList[i - 1] + ",";

                    if (l.IndexOf(",-2,") != -1)
                    {
                        strRet.AppendLine(string.Format("        p{0} = ARGUMENT_STRING_TABLE[p{0}];", i.ToString()));
                    }
                }
                
                strRet.AppendLine(string.Format("        return _{0}(character, p1, p2, p3, p4, p5);\r\nend", dr["ename"].ToString()));
                strRet.Append(dr[0].ToString());
                strRet.AppendLine();
            }

            return strRet.ToString();
        }

        /// <summary>
        /// ��ȡAI�Զ��嶯��������
        /// </summary>
        /// <returns>AI�Զ��嶯��������</returns>
        public string GetUserActionContext()
        {
            return GetUserActionContext(conn_jx3web);
        }       

        /// <summary>
        /// ��ȡ����ͼLS��׼��Ӧ�ַ���������
        /// </summary>
        /// <param name="content">�ַ���</param>
        /// <param name="mapName">������</param>
        /// <returns>����</returns>
        public string InsertIntoFlowChartLS(string content, string mapName)
        {
            string id = null; // ���ص�id
            SqlTransaction transaction = null; // ����  

            try
            {
                if (conn_jx3web.State == ConnectionState.Closed) // ��sql����
                {
                    conn_jx3web.Open();
                }

                content = content.Replace("'", "''"); // �������ݿ�ǰ�����滻
                transaction = conn_jx3web.BeginTransaction(); // ��������
                SqlCommand cmd = conn_jx3web.CreateCommand();
                cmd.Transaction = transaction;

                string sqlString = string.Format("SELECT id FROM sys_script_lsfile WHERE content = '{0}' AND classification = '{1}'",
                                                 content, mapName);
                cmd.CommandText = sqlString;
                object executeResult = cmd.ExecuteScalar();

                if (executeResult != null && !(executeResult is DBNull)) // �����Ѿ�����
                {
                    id = executeResult.ToString();
                }
                else // ���в����ڣ���Ҫ���
                {
                    sqlString = string.Format("INSERT INTO {0} ({1}, {2}) VALUES ('{3}', '{4}')",
                                              "sys_script_lsfile", "content", "classification", content, mapName);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("SELECT MAX({0}) FROM {1}", "id", "sys_script_lsfile");
                    cmd.CommandText = sqlString;
                    id = cmd.ExecuteScalar().ToString();
                }
                
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
                if (conn_jx3web.State == ConnectionState.Open) // �ر�sql����
                {
                    conn_jx3web.Close();
                }
            }

            return id;
        }

        /// <summary>
        /// �����ַ�����ls���ж�Ӧ��ID
        /// </summary>
        /// <param name="str">�ַ���</param>
        /// <returns>�ַ�����Ӧ��ID</returns>
        public string InsertIntoLS(string str)
        {
            string strSQL = @"
declare @ret int
declare @str varchar(1000)
set @str = '{0}'
set @ret = (select id from ai_ls where content = @str)
if @ret is not null
	select @ret
else
	begin
	insert into ai_ls(content) values (@str)
	select @@identity
	end";
            strSQL = string.Format(strSQL, str);
            DataTable dt = GetDataTable(strSQL, conn_jx3web);

            return dt.Rows[0][0].ToString();
        }
    #endregion

        /// <summary>
        /// ִ��sql����
        /// </summary>
        /// <param name="sqlString">sql����</param>
        /// <return>�Ƿ��ִ�гɹ�</return>
        public bool ExecuteSqlCommand(string sqlString)
        {
            bool executeSuccess = true;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);
                executeSuccess = false;
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            return executeSuccess;
        }

        /// <summary>
        /// ִ��sql��ѯ����
        /// </summary>
        /// <param name="sqlString">sql����</param>
        /// <returns>��ѯ���</returns>
        public object ExecuteSqlSelectCommand(string sqlString)
        {
            object executeResult = null;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                executeResult = cmd.ExecuteScalar();
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            return executeResult;
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        public DataTable GetDataTable(string sqlString)
        {
            return GetDataTable(sqlString, this.conn);
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        public DataTable GetDataTable(string sqlString, SqlConnection conn)
        {                
            DataTable table = null;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }                

                SqlDataAdapter adapter = new SqlDataAdapter(sqlString, conn);
                adapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet dataSet = new DataSet();
                adapter.Fill(dataSet);
                table = dataSet.Tables[0];
            }
            catch (SqlException ex)
            {
                PrintExceptionInformation(ex);
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return table;
        }

        /// <summary>
        /// ��ӡ��sql�쳣��Ϣ
        /// </summary>
        /// <param name="ex">sql�쳣</param>
        private void PrintExceptionInformation(SqlException ex)
        {
            MessageBox.Show(string.Format("ִ�е�ǰ��������sql�쳣��{0}", ex.Message), "���ݿ����",
                            MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void PrintExceptionInformation(Exception ex)
        {
            MessageBox.Show(string.Format("ִ�е�ǰ���������쳣��{0}", ex.Message), "���ݲ���",
                            MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}
