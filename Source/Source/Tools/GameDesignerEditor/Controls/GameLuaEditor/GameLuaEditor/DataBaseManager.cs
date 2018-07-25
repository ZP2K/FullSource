using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Data;
using System.Collections;
using System.Text;
using System.IO;
using LuaInterface;

namespace GameLuaEditor
{
    public class DataBaseManager
    {
        private static DataBaseManager manager = null; // DataBaseManager��ʵ��
        private DataTable lsTable = null; // ls���ݱ�
        private Hashtable lsHashtable = null; // ls hash��
        public SqlConnection conn; // ���ݿ�����
        private Lua m_lua;

        /// <summary>
        /// ˽�й��캯�������������ⲿ��ʼ��
        /// </summary>
        private DataBaseManager()
        {
            InitLua();
        }

        private void InitLua()
        {
            m_lua = new Lua();
            m_lua.RegisterFunction("GetDataTableRow", this, typeof(DataBaseManager).GetMethod("GetDataTableRow"));
        }

        public bool GetDataTableRow(string tablename, LuaTable keys, ref LuaTable ltReturn)
        {
            string sql = string.Format("SELECT * FROM {0}", tablename);
            DataTable tbl = GetDataTable(sql);
            if (tbl.PrimaryKey.Length != keys.Keys.Count)
                MessageBox.Show(string.Format("{0} ���� ����GetDataTableRow���ԣ�{1}\r\n", DateTime.Now, "�������keys���ȴ���"));

            object[] KEYS = new object[tbl.PrimaryKey.Length];
            for (int i = 0; i < KEYS.Length; i++)
            {
                KEYS[i] = ((LuaTable)keys)[i];
            }
            DataRow row = tbl.Rows.Find(KEYS);
            if (row == null)
                return false;

            foreach (DataColumn col in tbl.Columns)
            {
                ltReturn[col.ColumnName] = (row[col.ColumnName] == DBNull.Value ? null : row[col.ColumnName]);
            }
            return true;
        }


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
        /// �����ű�
        /// </summary>
        /// <param name="path">�ű�·��</param>
        /// <param name="content">�ű�����</param>
        /// <param name="coverScript">�Ƿ񸲸�ԭ�нű�</param>
        /// <returns>�ű�ID</returns>
        public string CreateScriptData(string path, string content, bool coverScript)
        {
            string id = null;
            SqlTransaction transaction = null;

            string newContent = EncodeScriptContent(path, content);
            byte[] data = EncodeScriptData(newContent);

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;

                string sqlString = string.Format("SELECT id FROM sys_script_script WHERE path = '{0}'", path);
                cmd.CommandText = sqlString;
                object executeResult = cmd.ExecuteScalar();

                SqlParameter sqlParameter = new SqlParameter("@data", data);
                cmd.Parameters.Add(sqlParameter);

                if (executeResult != null)
                {
                    if (coverScript)
                    {
                        id = executeResult.ToString();
                        sqlString = string.Format("UPDATE sys_script_script SET data = @data, edittime = '{0}', host = '{1}' WHERE id = '{2}'",
                                                  DateTime.Now, GetHostName(), id);
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                    }                    
                }
                else
                {
                    sqlString = string.Format("INSERT INTO sys_script_script (path, data, host, edittime) VALUES ('{0}', @data, '{1}', '{2}')",
                                              path, GetHostName(), DateTime.Now);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("SELECT id FROM sys_script_script WHERE path = '{0}'", path);
                    cmd.CommandText = sqlString;
                    executeResult = cmd.ExecuteScalar();

                    if (executeResult != null)
                    {
                        id = executeResult.ToString();
                    }
                }
                
                transaction.Commit();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("�ڴ����ű�ʱ����sql�쳣��{0}", ex.Message), "�����ű�",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);

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

            return id;
        }

        /// <summary>
        /// �����ű�
        /// </summary>
        /// <param name="path">�ű�</param> 
        /// <returns>�ű�ID</returns>
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

                sqlString = string.Format("SELECT id FROM sys_script_script WHERE path = '{0}'", path);
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
        /// �ӱ����ļ��ж�ȡ�ű�
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <returns>�ű�����</returns>
        public string GetScriptDataFromFile(string fileName)
        {
            string content = null;

            if (File.Exists(fileName))
            {
                StreamReader reader = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                content = reader.ReadToEnd();
                reader.Close();
            }

            return content;
        }

        /// <summary>
        /// ��ȡ�ű���ԭʼ�ַ���
        /// </summary>
        /// <param name="id">�ű�ID</param>
        /// <returns>�ű�ԭʼ�ַ���</returns>
        public string GetScriptMetaData(string id)
        {
            string content = null;

            if (conn.State == ConnectionState.Closed)
            {
                conn.Open();
            }

            string sqlString = string.Format("SELECT data, path FROM sys_script_script WHERE id = '{0}'", id);
            DataTable scriptTable = GetDataTable(sqlString);

            if (scriptTable.Rows.Count > 0)
            {                
                if (!(scriptTable.Rows[0]["data"] is DBNull))
                {
                    content = DecodeScriptData(((byte[])scriptTable.Rows[0]["data"]));
                }
            }

            if (conn.State == ConnectionState.Open)
            {
                conn.Close();
            }

            return content;
        }

        /// <summary>
        /// ��ȡ�ű������ַ���
        /// </summary>
        /// <param name="id">�ű�id</param>
        /// <returns>�ű������ַ���</returns>
        public string GetScriptData(string id)
        {
            string content = null; // �ű������ַ���
            string path = null;

            if (conn.State == ConnectionState.Closed) // ��sql����
            {
                conn.Open();
            }

            string sqlString = string.Format("SELECT data, path FROM sys_script_script WHERE id = '{0}'", id);
            DataTable scriptTable = GetDataTable(sqlString);
            
            if (scriptTable.Rows.Count > 0) // ���ڶ�Ӧ�Ľű���¼
            {
                content = scriptTable.Rows[0]["data"].ToString();                    
                
                if (content != "")
                {
                    content = DecodeScriptData(((byte[])scriptTable.Rows[0]["data"]));
                    path = scriptTable.Rows[0]["path"].ToString(); 
                    content = DecodeScriptContent(path, content);
                }
            }

            if (conn.State == ConnectionState.Open) // �ر�sql����
            {
                conn.Close();
            }
         
            content = GetBeforeOpen(content, path);

            return content;
        }

        /// <summary>
        /// ��ȡ�ű�����ʷ��¼��Ϣ
        /// </summary>
        /// <param name="label">�ű�����ʷ��¼���</param>
        /// <returns>�ű�������</returns>
        public string GetScriptHistoryData(string label)
        {
            string historyData = null; // �ű�������

            if(conn.State == ConnectionState.Closed) // ��sql����
            {
                conn.Open();
            }

            string sqlString = string.Format("SELECT data, path FROM sys_script_log WHERE label = '{0}'", label);
            DataTable logTable = GetDataTable(sqlString);

            if (logTable.Rows.Count > 0) // ���ڶ�Ӧ�Ľű���¼
            {
                historyData = logTable.Rows[0]["data"].ToString();

                if (historyData != "")
                {
                    historyData = DecodeScriptData(((byte[])logTable.Rows[0]["data"]));
                    string path = logTable.Rows[0]["path"].ToString();
                    historyData = DecodeScriptContent(path, historyData);
                }
            }

            if(conn.State == ConnectionState.Open) // �ر�sql����
            {
                conn.Close();
            }

            return historyData;
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
    
        public List<string> GetFileNameList(List<string> idList)
        {
            List<string> fileNameList = new List<string>();

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString;                

                foreach (string id in idList)
                {
                    sqlString = string.Format("SELECT id, path FROM {0} WHERE id = {1}", "sys_script_lsfile", id);
                    DataTable infoTable = GetDataTable(sqlString);

                    if (infoTable.Rows.Count > 0)
                    {
                        string scriptPath = infoTable.Rows[0]["path"].ToString();
                        fileNameList.Add(infoTable.Rows[0]["path"].ToString());
                    }


                }                                
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ�ű�����ʱ����sql�쳣��" + ex.Message, "��ȡ�ű�����",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            return fileNameList;
        }

        /// <summary>
        /// ����ű���ls�ļ�
        /// </summary>
        /// <param name="idList">�ű�id����</param>
        /// <returns>�ű���ls�ļ�hash��</returns>
        public Hashtable OutputScriptData(List<string> idList, bool exportCondenseString)
        {
            Hashtable fileTable = null;

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                if (idList.Count > 0) // �����ļ���������
                {
                    fileTable = new Hashtable(); // �ļ�
                    List<string> classificationList = new List<string>(); // ��������
                    string sqlString = string.Format("SELECT * FROM {0}", "sys_script_lsfile");
                    DataTable lsTable = GetDataTable(sqlString);

                    // �����ļ�
                    foreach (string id in idList) 
                    {
                        sqlString = string.Format("SELECT * FROM {0} WHERE id = {1}",
                                                  "sys_script_script", id);
                        DataTable scriptTable = GetDataTable(sqlString);

                        if (scriptTable.Rows.Count > 0)
                        {
                            string path = scriptTable.Rows[0]["path"].ToString();

                            if (scriptTable.Rows[0]["data"] is DBNull)
                            {
                                fileTable[path] = "";
                                continue;
                            }

                            string data = DecodeScriptData(((byte[])scriptTable.Rows[0]["data"]));
                            string classification = GetClassification(scriptTable.Rows[0]["path"].ToString());

                            // ����ls�ļ�
                            DataRow[] lsRows = lsTable.Select(string.Format("classification = '{0}'", classification));

                            if (lsRows.Length > 0 && !classificationList.Contains(classification))
                            {
                                string tableName = "SCENE_STRING_TABLE";

                                if (!path.StartsWith(@"scripts\Map"))
                                {
                                    tableName = string.Format("{0}_STRING_TABLE", classification.ToUpper());
                                    fileTable[string.Format(@"scripts\{0}\include\strings.ls", classification)] = CodeConvert.ConvertLsFileContent(lsRows, tableName);
                                }
                                else
                                {
                                    fileTable[string.Format(@"scripts\Map\{0}\include\scenestrings.ls", classification)] = CodeConvert.ConvertLsFileContent(lsRows, tableName);
                                }

                                classificationList.Add(classification);
                            }
                            
                            fileTable[path] = data;
                        }
                    }
                }             
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڵ����ű��ļ�ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return fileTable;
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

        public string GetBeforeOpen(string code, string path)
        {
            string content = code;
            string strFileName = Path.Combine(Application.StartupPath, @"Plugins\LuaCheck\filter.lua");

            if (File.Exists(strFileName))
            {
                try
                {
                    // ���û�������
                    Environment.CurrentDirectory = Application.StartupPath;
                    m_lua.DoFile(strFileName);
                    LuaFunction fun = m_lua.GetFunction("beforeOpen");

                    if (fun != null)
                    {
                        object[] ret = fun.Call(content, path);

                        if (ret.Length == 1 && ret[0] != null)
                        {
                            content = ret[0].ToString();
                        }
                        else
                        {
                            Helper.RecordLog("beforeOpen�ķ���ֵ����ȷ", Helper.LogLevel.Warning);                            
                        }
                    }
                }
                catch (System.Exception ex)
                {
                    Helper.RecordLog(ex.ToString(), Helper.LogLevel.Warning);
                    InitLua();
                }
            }

            return content;
        }

        public string GetBeforeSave(string code, string path)
        {
            string content = code;
            string strFileName = Path.Combine(Application.StartupPath, @"Plugins\LuaCheck\filter.lua");

            if (File.Exists(strFileName))
            {
                try
                {
                    // ���û�������
                    Environment.CurrentDirectory = Application.StartupPath;
                    m_lua.DoFile(strFileName);
                    LuaFunction fun = m_lua.GetFunction("beforeSave");

                    if (fun != null)
                    {
                        object[] ret = fun.Call(content, path);

                        if (ret.Length == 1 && ret[0] != null)
                        {
                            content = ret[0].ToString();
                        }
                        else
                        {
                            Helper.RecordLog("beforeSave�ķ���ֵ����ȷ", Helper.LogLevel.Warning);
                        }
                    }
                }
                catch (System.Exception ex)
                {
                    Helper.RecordLog(ex.ToString(), Helper.LogLevel.Warning);
                }
            }
            return content;
        }

        /// <summary>
        /// ת���ű�����
        /// </summary>
        /// <param name="scriptPath">�ű�·��</param>
        /// <param name="scriptContent">�ű�����</param>
        /// <returns>ת����Ľű�����</returns>
        private string DecodeScriptContent(string scriptPath, string scriptContent)
        {
            string classification = GetClassification(scriptPath);
            InitLsTable(classification);
            InitLsHashTable();

            string tableName = "SCENE_STRING_TABLE";

            if (!IsScriptInMap(scriptPath) && !scriptContent.Contains("SCENE_STRING_TABLE")) // ���ݾɸ�ʽ
            {
                tableName = string.Format("{0}_STRING_TABLE", classification.ToUpper());
            }

            string newContent = CodeConvert.Decode(scriptContent, lsHashtable, tableName);

            return newContent;
        }

        /// <summary>
        /// ת���ű�����
        /// </summary>
        /// <param name="scriptPath">�ű�·��</param>
        /// <param name="scriptContent">�ű�����</param>        
        /// <returns>ת����Ľű�����</returns>
        private string EncodeScriptContent(string scriptPath, string scriptContent)
        {
            Hashtable contentHashtable = new Hashtable(); // ��¼�ű����ݺͶ�Ӧid��hash��
            string classification = GetClassification(scriptPath);
            InitLsTable(classification);
            InitLsHashTable();

            foreach (object o in lsHashtable.Keys)
            {
                contentHashtable[lsHashtable[o]] = o;
            }

            // ���ű��е������ַ���ȡ������ls�ļ��Աȣ�������ls�ļ�
            List<string> varList = CodeConvert.GetStringList(scriptContent);
            List<string> contentList = new List<string>();

            foreach (DataRow dataRow in lsTable.Rows)
            {
                contentList.Add(dataRow["content"].ToString());
            }

            foreach (string s in varList)
            {
                if (!contentList.Contains(s))
                {
                    contentHashtable[s] = SaveLsFileData(s, classification);
                }
            }

            string tableName = "SCENE_STRING_TABLE";

            if (!IsScriptInMap(scriptPath))
            {
                tableName = string.Format("{0}_STRING_TABLE", classification.ToUpper());
            }

            string newContent = CodeConvert.Encode(scriptContent, contentHashtable, tableName);

            return newContent;
        }

        /// <summary>
        /// ����ű�����
        /// </summary>
        /// <param name="content">�ű�����</param>
        /// <returns>��������</returns>
        private byte[] EncodeScriptData(string content)
        {
            byte[] data = Encoding.UTF8.GetBytes(content);

            return data;
        }
        
        /// <summary>
        /// ����ű�����
        /// </summary>
        /// <param name="data">��������</param>
        /// <returns>�ű�����</returns>
        private string DecodeScriptData(byte[] data)
        {
            string content = Encoding.UTF8.GetString(data);

            return content;
        }

        /// <summary>
        /// ����ű�����
        /// </summary>
        /// <param name="id">�ű�id</param>
        /// <param name="content">�ű�����</param>
        /// <returns>�Ƿ񱣴�ɹ�</returns>
        public bool SaveScriptData(string path, string content, out string viewText)
        {
            bool result = true;

            content = GetBeforeSave(content, path);
            viewText = content;
            string newContent = EncodeScriptContent(path, content);

            if (conn.State == ConnectionState.Closed) // ��sql����
            {
                conn.Open();
            }
                        
            byte[] data = EncodeScriptData(newContent);
            string sqlString = string.Format("UPDATE sys_script_script SET data = @data, edittime = '{0}', host = '{1}' WHERE path = '{2}'",
                                             DateTime.Now, System.Net.Dns.GetHostName(), path);
            SqlCommand cmd = new SqlCommand(sqlString, conn);
            SqlParameter spcode = new SqlParameter("@data", data);
            cmd.Parameters.Add(spcode);
            int iret = cmd.ExecuteNonQuery();

            if (iret <= 0)
            {
                result = false;
            }
        
            if (conn.State == ConnectionState.Open) // �ر�sql����
            {
                conn.Close();
            }

            return result;
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
        /// �����ű���¼
        /// </summary>
        /// <param name="id">�ű�id</param>
        /// <returns>�Ƿ������ɹ�</returns>
        public bool LockScript(string id)
        {
            object[] keys = new object[] { id }; // ����
            bool result = true; // ����ֵ

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                RecordLockEx recordLock = new RecordLockEx(conn, "sys_script_script", keys, 0);
                recordLock.UpdateLocalVersion(keys);
                if (!recordLock.Lock(keys))
                {
                    MessageBox.Show(string.Format("�ýű��ѱ�{0}����!", recordLock.GetServerLastLockUser(keys)));
                    result = false;
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�������ű���¼ʱ����sql�쳣: " + ex.ToString());
                result = false;
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڴ�����ʱ�����쳣: " + ex.ToString());
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
        /// �����ű���¼
        /// </summary>
        /// <param name="id">�ű�id</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        public bool UnLockScript(string id)
        {
            object[] keys = new object[] { id }; // ����
            bool result = true; // ����ֵ

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                RecordLockEx recordLock = new RecordLockEx(conn, "sys_script_script", keys, 0);
                recordLock.UpdateLocalVersion(keys);
                if (recordLock.UnLock(keys) < 0)
                {
                    result = false;
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڽ����ű���¼ʱ����sql�쳣: " + ex.ToString());
                result = false;
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڴ�����ʱ�����쳣: " + ex.ToString());
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
        /// ����Ӧid�Ľű��Ƿ��Ѿ�����
        /// </summary>
        /// <param name="id">�ű�id</param>
        /// <returns>�ű��Ƿ��Ѿ�����</returns>
        public bool IsScriptLocked(string id)
        {
            bool result = false;

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT tablename FROM {0} WHERE {1} = '{2}' AND {3} = '{4},' AND lockhostname IS NOT NULL",
                    "sys_lock_record", "tablename", "sys_script_script", "fieldvalues", id);
                DataTable scriptTable = GetDataTable(sqlString);
                result = (scriptTable.Rows.Count > 0);
            }
            catch(SqlException ex)
            {
                MessageBox.Show(string.Format("���idΪ{0}�Ľű��Ƿ���ʱ����sql�쳣: ", id) + ex.ToString());
                result = false;
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڴ�����ʱ�����쳣: " + ex.ToString());
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
        /// ǿ�����������
        /// </summary>
        /// <returns>�Ƿ�����ɹ�</returns>
        public bool ClearLockScript(string id)
        {
            object[] keys = new object[] { id }; // ����

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                RecordLockEx recordLock = new RecordLockEx(conn, "sys_script_script", keys, 0);
                if (!recordLock.ForceClearLock(keys))
                {
                    return false;
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("������ű���¼ʱ����sql�쳣: " + ex.ToString());
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
            return true;
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
                foreach (DataRow dataRow in lsTable.Rows)
                {
                    lsHashtable[dataRow["id"].ToString()] = dataRow["content"].ToString();
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
                // string connectString = "Server = jx3web; DataBase = s3design; Uid = s3design; Password = ddd12345678;";
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

                string sqlString = string.Format("SELECT * FROM sys_script_lsfile WHERE classification = '{0}'", classification);
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
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString)
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
        private DataTable GetDataTable(string sqlString, SqlTransaction transaction)
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