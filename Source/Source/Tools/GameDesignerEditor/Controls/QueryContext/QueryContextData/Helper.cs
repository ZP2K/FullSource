using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using System.Collections;
using System.Data.SqlClient;
using System.Data;
using System.Drawing;

namespace QueryContextData
{
    public static class Helper
    {
        public static string ConnectionString = "";
        public static string ClientPath = "";
        public static string GameLuaEditorSetting = "";
        public static string ServerPath = ""; // ���������Ŀ¼
        public static string ServerIP = "127.0.0.1"; // ����˵�IP��ַ
        public static int ReceiveMessageDelay = 64; // ������Ϣ�ӳ�
        public static int ReceiveMessageRetryTime = 3; // ������Ϣ��ʱ���Դ���
        public static int BreakPointWaitCircle = 256; // ���Զϵ㴥�������
        public static Hashtable apiTable = new Hashtable(); // �ű�API���ݱ�

        // ����string���ļ�
        private static void StringToFile(string strContent, string strFileName)
        {
            StringToFile(strContent, strFileName, Encoding.GetEncoding("gb2312")); // �������ؼ�����Encoding
        }

        public static void WriteStringToFile(string strContent, string strFileName)
        {
            StringToFile(strContent, strFileName); // �������ؼ�����Encoding
        }

        private static void StringToFile(string strContent, string strFileName, Encoding encoding)
        {
            if (!File.Exists(strFileName))
            {
                if (strFileName.IndexOf("\\") == -1) return;
                Directory.CreateDirectory(strFileName.Substring(0, strFileName.LastIndexOf("\\")));
                File.Create(strFileName).Close();
            }

            StreamWriter writer = new StreamWriter(strFileName, false, encoding);
            writer.Write(strContent);
            writer.Close();
        }

        // ��ȡ�ļ���String
        public static string FileToString(string strFileName)
        {
            if (!File.Exists(strFileName))
            {
                return "";
            }

            StreamReader reader = new StreamReader(strFileName, Encoding.GetEncoding("gb2312"));
            string strRet = reader.ReadToEnd();
            reader.Close();

            return strRet;
        }

        // ִ���ⲿ����
        public static void Execute(string cmd, string arg, string sync)
        {
            Process p = new Process();
            p.StartInfo.FileName = cmd;
            p.StartInfo.Arguments = arg;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardInput = true;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.CreateNoWindow = true;
            p.Start();
            if ("false" == sync)
            {
                p.WaitForExit();
                p.Close();
            }
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <param name="connection">sql����</param>
        /// <returns>���ݱ�</returns>
        public static DataTable GetDataTable(string sqlString, SqlConnection conn)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            DataTable tb = ds.Tables[0];
            return tb;
        }

        /// <summary>
        /// �������Ƿ����
        /// </summary>
        /// <param name="processName">������</param>
        /// <param name="checkMultiProcess">�Ƿ�����ͬ������</param>
        /// <returns>�����Ƿ����</returns>
        public static bool CheckProcessExist(string processName, bool checkMultiProcess)
        {
            bool exist = false;
            int count = 0;

            foreach (System.Diagnostics.Process p in System.Diagnostics.Process.GetProcesses())
            {
                if (p.ProcessName == processName)
                {                                        
                    if (checkMultiProcess)
                    {
                        count++;
                    }
                    else
                    {
                        exist = true;
                        break;
                    }                    
                }
            }

            if (checkMultiProcess)
            {
                if (count > 1)
                {
                    exist = true;
                }
            }

            return exist;
        }
    }
}
