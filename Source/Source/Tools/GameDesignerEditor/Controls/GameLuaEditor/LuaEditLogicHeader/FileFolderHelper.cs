using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Runtime.InteropServices; // import


namespace LuaEditLogicHeader
{
    public class FileFolderHelper 
    {
        // import
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);


        // ����string���ļ�
        static public void StringToFile(string strContent, string strFileName)
        {
            StringToFile(strContent, strFileName, Encoding.GetEncoding("gb2312")); // �������ؼ�����Encoding
        }
        static public void WriteStringToFile(string strContent, string strFileName)
        {
            StringToFile(strContent, strFileName); // �������ؼ�����Encoding
        }
        static public void StringToFile(string strContent, string strFileName, Encoding encoding)
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

        /// <summary>
        /// ���Ŀ¼�Ƿ���ڣ��������򴴽���Ŀ¼
        /// </summary>
        /// <param name="fileName">�����ļ�·��</param>
        public static void CreatePath(string fileName)
        {
            string path = Path.GetDirectoryName(fileName); // ��ȡĿ¼
            try
            { 
                if(!Directory.Exists(path)) // Ŀ¼������
                {
                    Directory.CreateDirectory(path);
                }
            }
            catch(IOException ex)
            {
                MessageBox.Show("�ڴ�����Ŀ¼ʱ����IO�쳣: " + ex.ToString());
            } 
        }

        // ��ȡ�ļ���string
        static public string FileToString(string strFileName)
        {
            return FileToString(strFileName, Encoding.GetEncoding("gb2312"));
        }
        static public string FileToString(string strFileName, Encoding encoding)
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

            //StreamReader reader = new StreamReader(strFileName, encoding);
            string strContent = reader.ReadToEnd();
            reader.Close();
            return strContent;
        }
        // ini�ļ�����
        static public int ReadIniFile(string section, string key, string def, StringBuilder retVal, int size, string filePath)
        {
            return GetPrivateProfileString(section, key, def, retVal, size, filePath);
        }
        static public long WriteIniFile(string section, string key, string val, string filePath)
        {
            return WritePrivateProfileString(section, key, val, filePath);
        }
        // �ļ�����·���� -> ���ļ���
        static public string FullPathToFileName(string strFullPath)
        {
            int lio1 = strFullPath.LastIndexOf('/');
            int lio2 = strFullPath.LastIndexOf('\\');
            return strFullPath.Substring(lio1 > lio2 ? lio1 + 1 : lio2 + 1);
        }


        // not recommended
        
        public FileFolderHelper()
        {
            
        }
        ~FileFolderHelper()
        {
            
        }
    }
}
