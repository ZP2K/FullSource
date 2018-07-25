using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace AITypeAutoCreate
{
    class Helper
    {
        private static Helper helper;
        private StringBuilder logText = new StringBuilder();

        private Helper()
        {

        }

        public static Helper GetHelper()
        {
            if (helper == null)
            {
                helper = new Helper();
            }

            return helper;
        }

        public string LogText
        {
            get
            {
                return logText.ToString();
            }
        }

        public void RecordLogText(string log)
        {
            string recordText = string.Format("{0} ���� {1}", DateTime.Now, log);
            logText.AppendLine(recordText);
            Console.WriteLine(recordText);
        }

        public string GetFileContent(string fileName)
        {
            string content = null;

            try
            {
                StreamReader streamReader = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                content = streamReader.ReadToEnd();
                streamReader.Close();
            }
            catch (IOException ex)
            {
                RecordLogText(string.Format("���ļ�{0}����IO����ʱ�����쳣��" + ex.Message, fileName));   
            }

            return content;
        }

        /// <summary>
        /// �����ļ�
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="content">��������</param>
        public void SaveData(string fileName, string content)
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
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }
                else
                {
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }

                sw.Write(content);
                sw.Close();
            }
            catch (IOException ex)
            {
                RecordLogText(string.Format("���ļ�{0}����IO����ʱ�����쳣��" + ex.Message, fileName));                
            }
        }
    }
}
