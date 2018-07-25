using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.IO;
using System.Net;
using System.Collections;
using LuaInterface;

namespace AutoExport
{
    class ExportManager
    {
        private SqlConnection conn; // sql����
        private StringBuilder logText = new StringBuilder(); // log�ı�
        private string rootPath; // ������Ŀ¼
        private List<string> autoTableList = new List<string>(); // �Զ������������
        private List<string> customTableList = new List<string>(); // �Զ��嵼���������
        private List<string> fileList = new List<string>(); // ��Ҫ���ļ�
        DataTable configTable = null; // �������ñ�
        Lua exportLua; // lua�����
        Lua postExportLua; // lua�����


        // -------------------------  ��Ҫ --------------------------------
        // -------------------------  ��Ҫ --------------------------------
        // -------------------------  ��Ҫ --------------------------------
        // -------------------------  ��Ҫ --------------------------------

        // �����Ѿ��Ƶ� Sword3\Source\Tools\AutoExport\src ��Ҫ�ڴ��޸��ˡ�

        // -------------------------  ��Ҫ --------------------------------
        // -------------------------  ��Ҫ --------------------------------
        // -------------------------  ��Ҫ --------------------------------
        // -------------------------  ��Ҫ --------------------------------


        /// <summary>
        /// ���캯��
        /// </summary>
        public ExportManager()
        {
            Init();
        }        

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            try
            {
                // ��ʼ��sql����
                string fileName = Path.Combine(Application.StartupPath, "AutoExport.ini");
                string content = FileFolderHelper.FileToString(fileName);

                IniStructure m_inis = new IniStructure();
                m_inis = IniStructure.ReadIniWithContent(content);
                string connectString = m_inis.GetValue("General", "ConnString");
                conn = new SqlConnection(connectString);

                // ��ȡ��Ŀ¼·��
                rootPath = m_inis.GetValue("General", "RootDir");

                // ��ȡ�������б�            
                string[] autoTableArray = m_inis.GetKeys("AutoExport");
                autoTableList.AddRange(autoTableArray);

                string[] customTableArray = m_inis.GetKeys("CustomExport");
                customTableList.AddRange(customTableArray);

                // ��ȡ��Դ�ļ��б�
                string[] fileArray = m_inis.GetKeys("Resource");
                fileList.AddRange(fileArray);

                // ��ȡ�Զ��������������Ϣ
                string sqlString = string.Format("SELECT * FROM sys_export_table_cfg");
                configTable = GetDataTable(sqlString);

                // ������Դ�ļ�
                DownLoadResource();

                // ��ʼ��lua�����
                exportLua = new Lua();
                exportLua["Conn"] = conn;
                exportLua["RootDir"] = rootPath;
                string luaFile = Path.Combine(Application.StartupPath, "export.lua");
                exportLua.DoFile(luaFile);

                postExportLua = new Lua();
                postExportLua["Conn"] = conn;
                postExportLua["RootDir"] = rootPath;
                postExportLua["___GIsServer"] = true;
                luaFile = Path.Combine(Application.StartupPath, "post_export.lua");
                postExportLua.DoFile(luaFile);
            }
            catch (Exception ex)
            {
                logText.Append(string.Format("{0} ���� �����쳣��{1}\r\n", DateTime.Now, ex.Message));
            }            
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString)
        {
            DataTable tb = null;

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                tb = ds.Tables[0];
            }
            catch (SqlException ex)
            {
                
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }            

            return tb;
        }

        /// <summary>
        /// �Ƚϲ�������Դ�ļ�
        /// </summary>
        private void DownLoadResource()
        {
            foreach(string s in fileList)
            {                
                FtpWebRequest ftp = FtpWebRequest.Create(new Uri(s)) as FtpWebRequest;
                ftp.UseBinary = true;
                ftp.KeepAlive = false;
                string localFileName = s.Replace("ftp://changan/newtools/", "");
                localFileName = Path.Combine(Application.StartupPath, localFileName);

                FileInfo fi = new FileInfo(localFileName);

                if(fi.Exists) // �����ļ��Ѿ�����
                {
                    ftp.Method = WebRequestMethods.Ftp.GetDateTimestamp;
                    FtpWebResponse response = ftp.GetResponse() as FtpWebResponse;
                    DateTime newEditTime = response.LastModified;
                    response.Close();

                    DateTime localEditTime = fi.LastWriteTime;

                    if(newEditTime > localEditTime) // ��Ҫ�����ļ�
                    {
                        fi.Delete();

                        ftp.Method = WebRequestMethods.Ftp.DownloadFile;
                        response = (FtpWebResponse)ftp.GetResponse();
                        Stream ftpStream = response.GetResponseStream();
                        long contentLength = response.ContentLength;
                        int bufferSize = 2048;
                        int readCount;
                        byte[] buffer = new byte[bufferSize];
                        readCount = ftpStream.Read(buffer, 0, bufferSize);

                        FileStream fileStream = new FileStream(localFileName, FileMode.Create);

                        while (readCount > 0)
                        {
                            fileStream.Write(buffer, 0, readCount);
                            readCount = ftpStream.Read(buffer, 0, bufferSize);
                        }

                        ftpStream.Close();
                        fileStream.Close();
                        response.Close();
                    }
                }
                else
                {
                    ftp.Method = WebRequestMethods.Ftp.DownloadFile;
                    FtpWebResponse response = (FtpWebResponse)ftp.GetResponse();
                    Stream ftpStream = response.GetResponseStream();
                    long contentLength = response.ContentLength;
                    int bufferSize = 2048;
                    int readCount;
                    byte[] buffer = new byte[bufferSize];
                    readCount = ftpStream.Read(buffer, 0, bufferSize);

                    string folderName = Path.GetDirectoryName(localFileName); // ��ȡĿ¼
                    if (!Directory.Exists(folderName)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(folderName); // �ȴ���Ŀ¼���ٴ����ļ�
                    }  
                    FileStream fileStream = new FileStream(localFileName, FileMode.Create);
                    while(readCount > 0)
                    {
                        fileStream.Write(buffer, 0, readCount);
                        readCount = ftpStream.Read(buffer, 0, bufferSize);
                    }
                    
                    ftpStream.Close();
                    fileStream.Close();
                    response.Close();
                }
            }
        }

        /// <summary>
        /// �����ļ�
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="content">��������</param>
        private void SaveData(string fileName, string content)
        {
            try
            {
                FileInfo fi = new FileInfo(fileName);
                StreamWriter sw;

                if (!fi.Exists) // �ļ�������
                {
                    string path = Path.GetDirectoryName(fileName); // ��ȡĿ¼
                    if (!Directory.Exists(path)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(path); // �ȴ���Ŀ¼���ٴ����ļ�
                    }                    
                    // sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("GB18030"));
                }
                else
                {
                    // sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("GB18030"));
                }

                sw.Write(content);
                sw.Close();
            }
            catch (IOException ex)
            {
                logText.Append(string.Format("{0} ���� ���ļ�{1}����IO����ʱ�����쳣��" + ex.Message + "\r\n", DateTime.Now, fileName));
            }
        }

        /// <summary>
        /// �����ݱ�ת��Ϊ�ַ�������
        /// </summary>
        /// <param name="tableName">���ݱ���</param>
        /// <returns>�ַ�������</returns>
        private string GenerateContentString(string tableName)
        {
            string sqlString = string.Format("SELECT * FROM {0}", tableName);
            DataTable table = GetDataTable(sqlString);
            StringBuilder content = new StringBuilder();
            StringBuilder line = new StringBuilder();

            // ��ȡ�Զ���ĵ���˳�����Ҫ�ų����ֶ�
            string customOrderString = "";
            List<string> excludeFieldList = new List<string>();

            DataRow[] dataRows = configTable.Select(string.Format("tablename = '{0}'", tableName));
            if (dataRows.Length > 0)
            {
                customOrderString = dataRows[0]["export_order"].ToString();
                string excludeFieldString = dataRows[0]["export_ignore"].ToString();

                if (excludeFieldString != "")
                {
                    string[] excludeFieldArray = excludeFieldString.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                    excludeFieldList.AddRange(excludeFieldArray);
                }
            }

            if (table.PrimaryKey.Length > 0) // ����������Ҫ����������
            {
                string orderString = "";
                foreach (DataColumn c in table.PrimaryKey)
                {
                    orderString += string.Format("{0}, ", c.ColumnName);
                }
                orderString = orderString.TrimEnd(new char[] { ',', ' ' });

                // �Զ���ĵ���˳������
                if (customOrderString != "")
                {
                    orderString = customOrderString;
                }

                sqlString = string.Format("SELECT * FROM {0} ORDER BY {1}", tableName, orderString);
                table = GetDataTable(sqlString);
            }

            foreach (DataColumn c in table.Columns)
            {
                if (!excludeFieldList.Contains(c.ColumnName))
                {
                    line.Append(string.Format("{0}\t", c.ColumnName));
                }
            }

            line.Remove(line.Length - 1, 1);
            line.Append("\r\n");
            content.Append(line);

            foreach (DataRow row in table.Rows)
            {
                line = new StringBuilder();

                foreach (DataColumn column in table.Columns)
                {
                    if (!excludeFieldList.Contains(column.ColumnName))
                    {
                        object fieldValue = row[column.ColumnName];

                        if (fieldValue is Boolean) // ����ֵת��Ϊ0,1
                        {
                            if ((bool)fieldValue)
                            {
                                line.Append("1\t");
                            }
                            else
                            {
                                line.Append("0\t");
                            }
                        }
                        else
                        {
                            line.Append(string.Format("{0}\t", fieldValue.ToString()));
                        }
                    }
                }

                line.Remove(line.Length - 1, 1);
                line.Append("\r\n");
                content.Append(line);
            }

            return content.ToString();
        }

        /// <summary>
        /// �����Զ�������
        /// </summary>
        /// <param name="tableName">����</param>
        private void ExportAutoTable(string tableName)
        {
            logText.Append(string.Format("{0} ���� ׼���������ݱ�{1}...\r\n", DateTime.Now, tableName));

            string content = GenerateContentString(tableName);

            DataRow[] dataRows = configTable.Select(string.Format("tablename = '{0}'", tableName));
            if (dataRows.Length > 0)
            {
                string path = dataRows[0]["filename"].ToString().TrimStart(new char[] { '\\' });

                if (path != "")
                {
                    string fullPath = Path.Combine(rootPath, path);
                    SaveData(fullPath, content);
                    logText.Append(string.Format("{0} ���� ���ݱ�{1}�����ɹ���\r\n", DateTime.Now, tableName));
                }
                else
                {
                    logText.Append(string.Format("{0} ���� ���ݱ�{1}����ʧ�ܣ�\r\n", DateTime.Now, tableName));
                }  
            }                                 
        }

        /// <summary>
        /// �����Զ������ݱ�
        /// </summary>
        /// <param name="tableName">���ݱ���</param>
        private void ExportCustomTable(string tableName)
        {
            logText.Append(string.Format("{0} ���� ׼���������ݱ�{1}...\r\n", DateTime.Now, tableName));           

            string content = GenerateContentString(tableName);
            string[] lines = content.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
            string[] headLineArray = lines[0].TrimEnd(new char[] { '\r' }).Split(new char[] { '\t' }, StringSplitOptions.RemoveEmptyEntries);
            ArrayList fields = new ArrayList();
            fields.AddRange(headLineArray);
            
            string[] values = new string[lines.Length - 1];
            for (int i = 1; i < lines.Length; i++)
            {
                values[i - 1] = lines[i].TrimEnd(new char[] { '\r' });
            }

            ImportInfoHelper helper = new ImportInfoHelper();
            helper.Fields = fields;
            helper.Values = values;

            bool exportSuccess = true;

            try
            {
                LuaFunction function = exportLua.GetFunction("onexport");
                object[] results = function.Call("", tableName, rootPath, helper);
                if (results != null && results.Length > 0 && !(bool)results[0])
                {
                    ExportAutoTable(tableName);
                }
            }
            catch (Exception ex)
            {
                logText.Append(string.Format("{0} ���� ���ݱ�{1}����ʧ�ܣ�{2}\r\n", DateTime.Now, tableName, ex.Message));
                exportSuccess = false;
            }
            
            if (exportSuccess)
            {
                logText.Append(string.Format("{0} ���� ���ݱ�{1}�����ɹ���\r\n", DateTime.Now, tableName));
            }            
        }

        /// <summary>
        /// �����Զ������ݱ�
        /// </summary>
        /// <param name="tableName">���ݱ���</param>
        private void ExportCustomPostTable(string tableName)
        {
            logText.Append(string.Format("{0} ���� ׼���������ݱ�{1}������...\r\n", DateTime.Now, tableName));

            DataRow[] rows = configTable.Select(string.Format("tablename = '{0}'", tableName));

            if (rows.Length > 0)
            {
                string path = rows[0]["filename"].ToString().TrimStart(new char[] { '\\' });
                string fullPath = Path.Combine(rootPath, path);
                bool exportSuccess = true;

                try
                {
                    LuaFunction function = postExportLua.GetFunction("onpostexport");

                    object[] results = function.Call(tableName, fullPath);
                    if (results != null && results.Length > 0 && !(bool)results[0])
                    {
                        // ExportAutoTable(tableName);
                    }
                }
                catch (System.Exception ex)
                {
                    logText.Append(string.Format("{0} ���� ���ݱ�{1}���ݴ���ʧ�ܣ�{2}\r\n", DateTime.Now, tableName, ex.Message));
                    exportSuccess = false;
                }
                
                if (exportSuccess)
                {
                    logText.Append(string.Format("{0} ���� ���ݱ�{1}�����ݴ���ɹ���\r\n", DateTime.Now, tableName));
                }                
            }
            else
            {
                logText.Append(string.Format("{0} ���� ���ݱ�{1}�����ݴ���ʧ�ܣ�\r\n", DateTime.Now, tableName));
            }
        }

        /// <summary>
        /// �������е����ݱ�
        /// </summary>
        public void Export()
        {
            logText.Append(string.Format("{0} ���� ׼���������ݱ�...\r\n", DateTime.Now));
               
            // �����Զ����ݱ�
            foreach(string s in autoTableList)
            {
                ExportAutoTable(s);
            }            

            // �����Զ������ݱ�
            foreach(string s in customTableList)
            {
                ExportCustomTable(s);
                ExportCustomPostTable(s);
            }                      

            logText.Append(string.Format("{0} ���� ���ݱ�����ɣ�", DateTime.Now));

            // ������־�ļ�
            string logPath = Path.Combine(Application.StartupPath, "ExportTableLog.txt");
            SaveData(logPath, logText.ToString());
            string[] consoleLines = logText.ToString().Split(new char[] { '\n' });
            foreach(string s in consoleLines)
            {
                Console.WriteLine(s);
            }            
            logText = new StringBuilder();
        }

        /// <summary>
        /// ����������
        /// </summary>
        class ImportInfoHelper
        {
            public ArrayList Fields;
            public string[] Values;
        }
    }
}
