using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace LogManager
{        
    [Flags]
    public enum LogLevel { Trace, Warning, Error }; // ��־�ȼ�

    public class LogManager
    {
        private static LogManager logManager = null;  // ��־����������
        private List<LogInfo> logInfoList = new List<LogInfo>(); // ��־��Ϣ����
        private string logEncode = "gb2312"; // ��־����
        private string logFolder = @"log\FlowChartEditor"; // ��־�ļ���

        /// <summary>
        /// ˽�й��캯��
        /// </summary>
        private LogManager()
        {
            
        }

        /// <summary>
        /// ��־����
        /// </summary>
        public string LogEncode
        {
            set
            {
                logEncode = value;
            }

            get
            {
                return logEncode;
            }
        }

        /// <summary>
        /// ��־�ļ���
        /// </summary>
        public string LogFolder
        {
            set
            {
                logFolder = value;
            }

            get
            {
                return logFolder;
            }
        }

        /// <summary>
        /// ��ȡ��־����������ʵ��
        /// </summary>
        /// <returns>��־����������</returns>
        public static LogManager GetLogManager()
        {
            if (logManager == null)
            {
                logManager = new LogManager();
            }

            return logManager;
        }


        /// <summary>
        /// ��¼��־
        /// </summary>
        /// <param name="logType">��־����</param>
        /// <param name="message">��־��Ϣ</param>
        /// <param name="logLevel">��־�ȼ�</param>
        public void RecordLog(string logType, string message, LogLevel logLevel)
        {
            LogInfo logInfo = new LogInfo(logType, message, logLevel);
            logInfoList.Add(logInfo);
        }        

        /// <summary>
        /// ת������
        /// </summary>
        /// <param name="dateTime">��ǰ����</param>
        /// <returns>�����ַ���</returns>
        public static string ConvertDateTime(DateTime dateTime)
        {
            return dateTime.ToString().Replace(" ", "-").Replace(":", "-");
        }

        /// <summary>
        /// ��ȡ�ļ�����
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <returns>�ļ�����</returns>
        public string ReadFile(string fileName)
        {
            string content = null;

            if (File.Exists(fileName))
            {
                try
                {
                    StreamReader streamReader = new StreamReader(fileName, Encoding.GetEncoding(logEncode));
                    content = streamReader.ReadToEnd();
                    streamReader.Close();
                }
                catch (IOException ex)
                {
                    ;
                }                
            }

            return content;
        }

        /// <summary>
        /// ���ı����浽�ļ�
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="content">�ı�����</param>
        public bool SaveFile(string fileName, string content)
        {
            bool success = true;

            try
            {
                FileInfo fi = new FileInfo(fileName);

                if (!fi.Exists) // �ļ�������
                {
                    string folderName = fi.DirectoryName; // ��ȡĿ¼

                    if (!string.IsNullOrEmpty(folderName) && !Directory.Exists(folderName)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(folderName); // �ȴ���Ŀ¼���ٴ����ļ�
                    }

                    fi.Create().Close();
                }

                StreamWriter streamWriter = new StreamWriter(fileName, false, Encoding.GetEncoding(logEncode));
                streamWriter.Write(content);
                streamWriter.Close();
            }
            catch (IOException ex)
            {                
                success = false;
            }

            return success;
        }

        /// <summary>
        /// ��¼��־
        /// </summary>
        public void WriteLog()
        {
            LogLevel logLevel = LogLevel.Trace | LogLevel.Warning | LogLevel.Error;
            WriteLog("", logLevel, DateTime.MinValue, DateTime.MaxValue);
        }

        /// <summary>
        /// д��־
        /// </summary>
        /// <param name="logType">��־����</param>
        /// <param name="logLevel">��־�ȼ�</param>
        /// <param name="minTime">��Сʱ��</param>
        /// <param name="maxTime">���ʱ��</param>
        public void WriteLog(string logType, LogLevel logLevel, DateTime minTime, DateTime maxTime)
        {
            StringBuilder logText = new StringBuilder();

            foreach (LogInfo logInfo in logInfoList)
            {
                if (((logInfo.RecordLogLevel & logLevel) == logInfo.RecordLogLevel)
                    && (logInfo.RecordTime >= minTime && logInfo.RecordTime <= maxTime)
                    && (logInfo.LogType == logType || string.IsNullOrEmpty(logType)))
                {
                    logText.AppendLine(logInfo.ToString());
                }
            }

            string fileName = Path.Combine(logFolder, string.Format("{0}.txt", ConvertDateTime(DateTime.Now)));
            SaveFile(fileName, logText.ToString());
        }

        /// <summary>
        /// ��־��Ϣ
        /// </summary>
        class LogInfo
        {
            private readonly string logType;
            private readonly string message;
            private readonly LogLevel logLevel;
            private DateTime recordTime;

            public LogInfo(string logType, string message, LogLevel logLevel)
            {
                this.logType = logType;
                this.message = message;
                this.logLevel = logLevel;
                recordTime = DateTime.Now;
            }

            public string LogType
            {
                get
                {
                    return logType;
                }
            }

            public string Message
            {
                get
                {
                    return message;
                }
            }

            public LogLevel RecordLogLevel
            {
                get
                {
                    return logLevel;
                }
            }

            public DateTime RecordTime
            {
                get
                {
                    return recordTime;
                }
            }

            public override string ToString()
            {
                string levelDescription = "Record";

                switch (logLevel)
                {
                    case LogLevel.Trace:
                        {
                            levelDescription = "Trace";
                            break;
                        }
                    case LogLevel.Warning:
                        {
                            levelDescription = "Warning";
                            break;
                        }
                    case LogLevel.Error:
                        {
                            levelDescription = "Error";
                            break;
                        }
                }

                string infoString = string.Format("{0} [{1}]: <{2}> {3}", LogManager.ConvertDateTime(DateTime.Now),
                                                  levelDescription, logType, message);

                return infoString;
            }
        }
    }
}
