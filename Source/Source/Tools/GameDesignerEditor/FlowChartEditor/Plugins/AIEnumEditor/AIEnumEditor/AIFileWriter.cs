using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Data;
using System.IO;

namespace AIEnumEditor
{
    class AIFileWriter
    {
        private string rootDir; // �ͻ��˸�Ŀ¼
        private SqlConnection conn; // sql����
        private SqlConnection headConn; // head���sql����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="rootDir">�ͻ��˸�Ŀ¼</param>
        /// <param name="conn">sql����</param>
        public AIFileWriter(string rootDir, SqlConnection conn)
        {
            this.rootDir = rootDir;
            this.conn = conn;
            InitHeadConnection();
        }

        private void InitHeadConnection()
        {
            string connectString = "Server = 10.20.86.11; DataBase = s3design_rtm; Uid = s3design; Password = davidbowie;";
            headConn = new SqlConnection(connectString);
        }

        /// <summary>
        /// ����ls�ļ�
        /// </summary>
        public void ExportLSFile()
        {
            string filePath = Path.Combine(rootDir, @"scripts\ai\argumentStrings.ls");
            string content = ConstructLSFile();
            SaveData(filePath, content);
        }

        /// <summary>
        /// �����Զ��嶯���ļ�
        /// </summary>
        public void ExportCustomFunctionsFile()
        {
            string filePath = Path.Combine(rootDir, @"scripts\ai\customFuctions.lua");
            string content = GetUserActionContext();
            SaveData(filePath, content);
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <param name="connection">sql����</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString, SqlConnection connection)
        {
            DataTable tb = null;

            try
            {
                if (connection.State == ConnectionState.Closed)
                {
                    connection.Open();
                }

                SqlDataAdapter adp = new SqlDataAdapter(sqlString, connection);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                tb = ds.Tables[0];
            }
            finally
            {
                if (connection.State == ConnectionState.Open)
                {
                    connection.Close();
                }
            }
                       
            return tb;
        }

        /// <summary>
        /// ��ȡAI�Զ��嶯��������
        /// </summary>
        /// <returns>�Զ��嶯��������</returns>
        private string GetUserActionContext()
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

                for (int i = 1; i <= strPList.Length; i++)
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
        /// ����LS��
        /// </summary>
        /// <returns>LS������</returns>
        private string ConstructLSFile()
        {
            string sqlString = "SELECT * FROM AI_LS";
            DataTable lsTable = GetDataTable(sqlString, headConn);
            StringBuilder code = new StringBuilder();

            code.AppendLine("ARGUMENT_STRING_TABLE = {");

            for (int i = 0; i < lsTable.Rows.Count; i++)
            {
                DataRow row = lsTable.Rows[i];
                string id = row["ID"].ToString();
                string word = row["Content"].ToString();

                if (i == lsTable.Rows.Count - 1) // ���һ������
                {
                    if ((word.StartsWith("'") && word.EndsWith("'")) ||
                        (word.StartsWith("\"") && word.EndsWith("\"") ||
                        (word.StartsWith("[[") && word.EndsWith("]]")))) // ����Ƿ��Ѿ����ַ������
                    {
                        code.AppendLine(string.Format("    [{0}] = {1}", id, word));
                    }
                    else
                    {
                        code.AppendLine(string.Format("    [{0}] = '{1}'", id, word));
                    }
                }
                else
                {
                    if ((word.StartsWith("'") && word.EndsWith("'")) ||
                        (word.StartsWith("\"") && word.EndsWith("\"") ||
                        (word.StartsWith("[[") && word.EndsWith("]]")))) // ����Ƿ��Ѿ����ַ������
                    {
                        code.AppendLine(string.Format("    [{0}] = {1},", id, word));
                    }
                    else
                    {
                        code.AppendLine(string.Format("    [{0}] = '{1}',", id, word));
                    }
                }
            }

            code.Append("}");

            return code.ToString();
        }

        /// <summary>
        /// ����Ĭ��������
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="content">��������</param>
        private void SaveData(string fileName, string content)
        {
            // ��Ϊ���ܱ��Զ��������ã�������IO�쳣�����ϲ㴦�����
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
        }
    }
}
