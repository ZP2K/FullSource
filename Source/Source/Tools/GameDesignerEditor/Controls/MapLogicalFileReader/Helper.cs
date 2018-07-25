using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Data;
using System.Data.SqlClient;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Reflection;
using System.Windows.Forms;

namespace MapLogicalFileReader
{
    public class Helper
    {
        // import
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

        public static IniStructure m_inis = new IniStructure();
        private static string m_filename = string.Empty;

        public static void CreatePath(string fileName)
        {
            string path = Path.GetDirectoryName(fileName); // ��ȡĿ¼
            try
            {
                if (!Directory.Exists(path)) // Ŀ¼������
                {
                    Directory.CreateDirectory(path);
                }
            }
            catch (IOException ex)
            {
                MessageBox.Show("�ڴ�����Ŀ¼ʱ����IO�쳣: " + ex.ToString());
            }
        }
        public static void StringToFile(string strContent, string strFileName, Encoding encoding)
        {
            if (!File.Exists(strFileName))
            {
                CreatePath(strFileName);
                File.Create(strFileName).Close();
            }
            StreamWriter writer = new StreamWriter(strFileName, false, encoding);
            writer.Write(strContent);
            writer.Close();
        }
        public static string FileToString(string strFileName)
        {
            return FileToString(strFileName, Encoding.GetEncoding("gb2312"));
        }
        public static string FileToString(string strFileName, Encoding encoding)
        {
            StreamReader reader;
            try
            {
                reader = new StreamReader(strFileName, encoding);
            }
            catch //(Exception e)
            {
                //MessageBox.Show(e.Message);
                return "";
            }
            string strContent = reader.ReadToEnd();
            reader.Close();
            return strContent;
        }

        // ϵͳini�ļ�����
        public static int ReadIniFile(string section, string key, string def, StringBuilder retVal, int size, string filePath)
        {
            return GetPrivateProfileString(section, key, def, retVal, size, filePath);
        }
        public static long WriteIniFile(string section, string key, string val, string filePath)
        {
            return WritePrivateProfileString(section, key, val, filePath);
        }
        // �Ż���ini�ļ�����
        public static bool OpenIniFile(string filename, string content)
        {
            //m_inis = IniStructure.ReadIni(filename);
            m_inis = IniStructure.ReadIniWithContent(content);
            m_filename = filename;
            return true;
        }
        public static void CloseIniFile()
        {
            m_inis = null;
            m_filename = string.Empty;
        }
        public static string GetIniString(string section, string key)
        {
            return m_inis.GetValue(section, key);
        }

        // db���
        private System.Data.DataTable GetDataTable(string sql, SqlConnection conn)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            System.Data.DataTable tbl = ds.Tables[0];
            return tbl;
        }

        //ִ��sql�ǲ�ѯ���
        public static int SqlNoneQureyExcute(SqlConnection conn, string sql)
        {
            int affectedRowCnt = 0;

            if (conn.State == ConnectionState.Closed)
            {
                conn.Open();
            }
            SqlCommand cmd = new SqlCommand(sql, conn);

            try
            {
                affectedRowCnt = cmd.ExecuteNonQuery();
            }
            catch (System.Exception e)
            {
                MessageBox.Show("execute sql cmd error:" + e.ToString());
            }
            finally
            {
                conn.Close();
            }

            return affectedRowCnt;
        }

        // ����Ѵ����ļ�
        public static string BrowserFile(string[] postfixes)
        {
            string filter = "";
            foreach (string postfix in postfixes)
            {
                if (postfix != "")
                    filter += string.Format("{0} �ļ�|*.{1}|", postfix, postfix);
            }

            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = filter + "ȫ���ļ�|*.*";
            if (ofd.ShowDialog() == DialogResult.OK)
                return ofd.FileName;
            else
                return "";
        }
        public static string BrowserFile()
        {
            return BrowserFile(new string[] { "" });
        }

        public static bool IsFileExists(string filePath)
        {
            return File.Exists(filePath);
        }
        public static bool IsDirectoryExists(string dir)
        {
            return Directory.Exists(dir);
        }
        //�ж��ַ����ܷ�ת��Ϊ���֣���������������
        public static bool CanConverToNumber(string ConvertingStr)
        {
            bool bRet = System.Text.RegularExpressions.Regex.IsMatch(ConvertingStr, @"^[+-]?\d+([\.]\d+)?$");
            return bRet;
        }
    }
}
