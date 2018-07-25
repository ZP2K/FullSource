using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Data;
using System.IO;
using System.Windows.Forms;

namespace AIDiagramPlugin
{
    public class AITableExport
    {
        /// <summary>
        /// �����Զ��嶯�����ļ�
        /// </summary>
        /// <param name="strClientPath">���Ŀ¼</param>
        /// <param name="conn">sql����</param>
        public void UserActionToFile(string strClientPath, SqlConnection conn)
        {
            string sqlString = "SELECT actionid, ename, name, rettype FROM AI_Action_Define";
            DataTable actionTable = GetDataTable(sqlString, conn);
            StringBuilder content = new StringBuilder();
            content.AppendLine("ActionID\tEnglishName\tChineseName\tReturnValue");

            foreach(DataRow dataRow in actionTable.Rows)
            {
                content.AppendLine(string.Format("{0}\t{1}\t{2}\t{3}", dataRow["actionid"].ToString(), dataRow["ename"].ToString(),
                                                 dataRow["name"].ToString(), dataRow["rettype"].ToString()));
            }

            StringToFile(content.ToString(), Path.Combine(strClientPath, @"scripts\ai\AIActionDefine.tab"));

            // old code
            // string strContext = DBManager.DataBaseManager.GetDataBaseManager().GetUserActionContext(conn);
            // StringToFile(strContext, Path.Combine(strClientPath, "scripts\\ai\\customFuctions.lua"));
        }
        
        /// <summary>
        /// �����ı����ļ�
        /// </summary>
        /// <param name="strContent">�ı�����</param>
        /// <param name="strFileName">�ļ���</param>
        static public void StringToFile(string strContent, string strFileName)
        {
            StringToFile(strContent, strFileName, Encoding.GetEncoding("gb2312")); // �������ؼ�����Encoding
        }

        /// <summary>
        /// �����ı����ļ�
        /// </summary>
        /// <param name="strContent">�ı�����</param>
        /// <param name="strFileName">�ļ���</param>
        static public void WriteStringToFile(string strContent, string strFileName)
        {
            StringToFile(strContent, strFileName); // �������ؼ�����Encoding
        }

        /// <summary>
        /// �����ı����ļ�
        /// </summary>
        /// <param name="strContent">�ı�����</param>
        /// <param name="strFileName">�ļ���</param>
        /// <param name="encoding">����</param>
        static public void StringToFile(string strContent, string strFileName, Encoding encoding)
        {
            try
            {
                if (!File.Exists(strFileName)) // �ļ��������򴴽��ļ�
                {
                    CreatePath(strFileName);
                    File.Create(strFileName).Close();
                }

                StreamWriter writer = new StreamWriter(strFileName, false, encoding);
                writer.Write(strContent);
                writer.Close();
            }
            catch (IOException ex)
            {
                MessageBox.Show(string.Format("�����ļ�{0}ʧ�ܣ�", strFileName), "�����ļ�",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
            }            
        }

        /// <summary>
        /// ���Ŀ¼�Ƿ���ڣ��������򴴽���Ŀ¼
        /// </summary>
        /// <param name="fileName">�����ļ�·��</param>
        public static void CreatePath(string fileName)
        {
            string path = Path.GetDirectoryName(fileName); // ��ȡĿ¼
            
            if (!Directory.Exists(path)) // Ŀ¼������
            {
                try
                {
                    Directory.CreateDirectory(path);
                }
                catch (IOException ex)
                {
                    MessageBox.Show(string.Format("����Ŀ¼{0}ʧ�ܣ�", path), "����Ŀ¼",
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }                       
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString, SqlConnection conn)
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
            catch (SqlException ex)
            {
                MessageBox.Show("SQL���ݿ����" + ex.Message);
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
