using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.IO;

namespace AIEnumEditor
{
    class CodeWriter
    {
        private SqlConnection conn; // sql����
        DataTable diagramTable = null;
        DataTable enumTable = null;
        DataTable parameterTable = null;

        /// <summary>
        /// ���캯��
        /// </summary>
        public CodeWriter()
        {
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        public void Init()
        {
            string connectString = "Server = 10.20.86.11; DataBase = s3design_rtm; Uid = s3design; Password = davidbowie;"; // Ĭ�������ַ���           
            string filename = Path.Combine(Application.StartupPath, "GameDesingerTools_Public.ini");
            FileInfo fi = new FileInfo(filename);

            if (fi.Exists)
            {
                string content = FileFolderHelper.FileToString(filename);
                IniStructure m_inis = new IniStructure();
                m_inis = IniStructure.ReadIniWithContent(content);
                connectString = m_inis.GetValue("General", "ConnString");
            }
            else
            {
                filename = Path.Combine(Application.StartupPath, "GameDesingerTools_Public.ini");
                fi = new FileInfo(filename);
                if (fi.Exists) // ��ȡAutoExport.ini�ļ�
                {
                    string content = FileFolderHelper.FileToString(filename);
                    SymmetricMethod sm = new SymmetricMethod();
                    content = sm.Decrypto(content);

                    IniStructure m_inis = new IniStructure();
                    m_inis = IniStructure.ReadIniWithContent(content);
                    connectString = m_inis.GetValue("General", "ConnString"); 
                }
            }

            conn = new SqlConnection(connectString);

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = "SELECT * FROM AI_Enum_Define";
                enumTable = GetDataTable(sqlString);
                sqlString = "SELECT * FROM AI_TemplateParamTable";
                parameterTable = GetDataTable(sqlString);
                sqlString = "SELECT ID, FullPath FROM changan.s3diagram_rtm.dbo.diagram WHERE Class = 'AIͼ'";
                diagramTable = GetDataTable(sqlString);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ���ݱ���Ϣʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
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

            SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            tb = ds.Tables[0];

            return tb;
        }

        /// <summary>
        /// ��ȡ����ֵ
        /// </summary>
        /// <param name="diagramPath">����ͼ·��</param>
        /// <param name="value">����ԭֵ</param>
        /// <returns>����ֵ</returns>
        public string GetArgumentsValue(string diagramPath, string value)
        {
            string newValue = "";
            string relevatePath = diagramPath.Replace("scripts\\ai\\", "");
            relevatePath = relevatePath.Replace(".lua", "");

            DataRow[] rows = diagramTable.Select(string.Format("FullPath = '{0}'", relevatePath));
            if(rows.Length > 0)
            {
                string diagramID = rows[0]["ID"].ToString();
                newValue = GetArgumentString(value, diagramID, parameterTable, enumTable);
            }

            return newValue;
        }

        /// <summary>
        /// ���AI�����Ƿ���Ч
        /// </summary>
        /// <param name="aiTypeTable">AI�������ݱ�</param>
        /// <param name="parameterTable">AI�������ݱ�</param>
        /// <returns>AI�����Ƿ���Ч</returns>
        private bool CheckArgumentAvail(DataTable aiTypeTable, DataTable parameterTable)
        {
            bool avail = true;

            foreach(DataRow row in aiTypeTable.Rows)
            {
                string aiType = row["AIType"].ToString();
                if(row["AIDiagramID"] is DBNull || row["ArgValues"] is DBNull)
                {
                    MessageBox.Show(string.Format("IDΪ{0}��AIģ��û�����ò�����", aiType), "�����Լ��",
                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return false;
                }

                string diagramID = row["AIDiagramID"].ToString();
                string argValues = row["ArgValues"].ToString();
                string[] datas = argValues.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                DataRow[] rows = parameterTable.Select(string.Format("DiagramID = '{0}'", diagramID), "SortID");

                if(rows.Length != datas.Length)
                {
                    MessageBox.Show(string.Format("IDΪ{0}��AIģ��Ĳ����Ѿ��������ı䣬��Ҫ�������ã�", aiType), "�����Լ��",
                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return false;
                }

                for(int i = 0; i < rows.Length; i++)
                {
                    DataRow dataRow = rows[i];
                    string[] values = datas[i].Split(new char[]{','});

                    if (dataRow["KeyID"].ToString() != values[0])
                    {
                        MessageBox.Show(string.Format("IDΪ{0}��AIģ��Ĳ����Ѿ��������ı䣬��Ҫ�������ã�", aiType), "�����Լ��",
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return false;
                    }
                }
            }

            return avail;
        }

        /// <summary>
        /// ��ȡ���Ͷ�Ӧ��ֵ
        /// </summary>
        /// <param name="type">����</param>
        /// <param name="value">����ֵ</param>
        /// <param name="enumTable">ö�ٱ�</param>
        /// <returns>���Ͷ�Ӧ��ֵ</returns>
        private string GetTypeValue(string type, string value, DataTable enumTable)
        {
            string typeValue = value;

            switch(type)
            {
                case "-1": // ����
                    {
                        break;
                    }
                case "-2": // �ַ���
                    {
                        typeValue = string.Format("\"{0}\"", value);
                        break;
                    }
                default: // ö��
                    {
                        //DataRow[] rows = enumTable.Select(string.Format("id = '{0}'", type));
                        //if(rows.Length > 0)
                        //{
                        //    DataRow row = rows[0];
                        //    string englishName = row["ename"].ToString();
                        //    string[] datas = row["values"].ToString().Split(new char[] { '\n' });
                        //    datas = datas[int.Parse(value)].TrimEnd(new char[] { '\r' }).Split(new char[] { ',' });
                        //    typeValue = string.Format("{0}.{1}", englishName, datas[1]);
                        //}

                        break;
                    }
            }

            return typeValue;
        }

        /// <summary>
        /// ��ȡAI�����ַ���
        /// </summary>
        /// <param name="value">�����ַ���</param>
        /// <param name="diagramID">����ͼID</param>
        /// <param name="parameterTable">�������ݱ�</param>
        /// <param name="enumTable">ö�����ݱ�</param>
        /// <returns>AI�����ַ���</returns>
        private string GetArgumentString(string value, string diagramID, DataTable parameterTable, DataTable enumTable)
        {
            string[] datas = value.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
            string argumentString = "";

            foreach(string data in datas)
            {
                string[] values = data.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                DataRow[] rows = parameterTable.Select(string.Format("KeyID = '{0}' AND DiagramID = '{1}'", values[0], diagramID), "SortID");
                if(rows.Length > 0)
                {
                    DataRow row = rows[0];
                    string type = row["type"].ToString();                    
                    string englishName = row["ename"].ToString();
                    argumentString += string.Format("{0} = {1}, ", englishName, GetTypeValue(type, values[1], enumTable));
                }
            }
            argumentString = argumentString.TrimEnd(new char[] { ',', ' ' });

            return argumentString;
        }

        /// <summary>
        /// ����Ĭ��������
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
                    FileStream fs = fi.Create();
                    fs.Close();
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }
                else
                {
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));                        
                }

                sw.Write(content);
                sw.Close();
                MessageBox.Show(string.Format("�ļ�{0}�����ɹ���", fileName), "AI��������",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("���ļ�{0}����IO����ʱ�����쳣: " + ex.ToString(), fileName));
            }
        }
    }
}
