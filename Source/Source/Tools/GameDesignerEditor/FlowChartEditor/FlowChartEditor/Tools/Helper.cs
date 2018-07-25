using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Drawing;
using System.Windows.Forms;
using System.Net;
using System.Reflection;
using System.Collections;
using System.Diagnostics;
using System.Data;

using LuaInterface;
using DevComponents.DotNetBar.Controls;

namespace FlowChartEditor
{
    public class Helper
    {
        private static Helper helper; // Ψһ��˽��ʵ��             

        private Graphics graphicsObject; // ��ͼ����
        private TextBoxX logBox; // ��־����ı���

        private string outputDir = ""; // ����ͼ�����·��
        private List<int> latestDiagramList = new List<int>(); // ����򿪵Ļ�ͼ
        private string encodeInfo = "gb2312"; // ����

        private string defaultChart = "AIͼ"; // Ĭ�ϻ�ͼ
        private const string configFileName = "config.lua"; // �����ļ�����
        private List<string> diagramNameList = new List<string>(); // ��ͼ��������        
        private const int maxDiagramCount = 10; // ����ͼ��������
        private Dictionary<string, LogicBaseManager> logicManagerDictionary = new Dictionary<string, LogicBaseManager>(); // �߼����������� 
        private Dictionary<string, string> logicManagerDisplayNameDictionary = new Dictionary<string, string>();
        private List<string> updateFileList = new List<string>(); // ��Ҫ���µ��ļ�����

        private LogManager.LogManager logManager = LogManager.LogManager.GetLogManager(); // ��־������

        /// <summary>
        /// ���캯��
        /// </summary>
        private Helper()
        {

        }

        /// <summary>
        /// ��ȡ�������ʵ��
        /// </summary>
        public static Helper GetHelper()
        {
            if(helper == null)
            {
                helper = new Helper();
            }

            return helper;
        }

        /// <summary>
        /// ��������ͼ����
        /// </summary>
        /// <param name="args">������ϣ��</param>
        public void SetArguments(Hashtable args)
        {           
            outputDir = args["OutPutDir"] as string;
            latestDiagramList = args["LatestDiagramList"] as List<int>;

            if (latestDiagramList == null)
            {
                latestDiagramList = new List<int>();
            }
        }

        /// <summary>
        /// ��ȡ����ͼ����
        /// </summary>
        /// <returns>������ϣ��</returns>
        public Hashtable GetArguments()
        {
            Hashtable args = new Hashtable();
            args["OutPutDir"] = outputDir;            
            args["LatestDiagramList"] = latestDiagramList;

            return args;
        }

        /// <summary>
        /// Ĭ�ϻ�ͼ����
        /// </summary>
        public string DefaultChart
        {
            get
            {
                return defaultChart;              
            }
        }

        /// <summary>
        /// ����򿪵Ļ�ͼ����s
        /// </summary>
        public List<int> LatestDiagramList
        {
            get
            {
                return latestDiagramList;
            }
        }

        /// <summary>
        /// �����߼�������
        /// </summary>
        /// <param name="filePath">�ļ�·��</param>
        /// <returns>�߼�������</returns>
        private LogicBaseManager LoadLogicManager(string filePath, string className)
        {
            LogicBaseManager logicBaseManager = null;

            try
            {
                string fullPath = Path.Combine(Application.StartupPath, @"Plugins\FlowChart");
                fullPath = Path.Combine(fullPath, filePath);
                Assembly assembly = Assembly.LoadFrom(fullPath);
                logicBaseManager = assembly.CreateInstance(className) as LogicBaseManager;
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("�����߼��ļ� {0} ʱ�����쳣��\r\n{1}", filePath, ex.Message), "��ʼ���߼�",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return logicBaseManager;
        }



        /// <summary>
        /// ��ʼ���߼�������
        /// </summary>
        public void InitLogicManager()
        {
            string filePath = Path.Combine(Application.StartupPath, configFileName);
            
            if (File.Exists(filePath))
            {
                Lua lua = new Lua();

                lua.DoFile(filePath);
                LuaTable configTable = lua.GetTable("Config") as LuaTable;

                string[] diagramNameArray = new string[maxDiagramCount];

                foreach (object key in configTable.Keys)
                {
                    LuaTable logicManagerTable = configTable[key] as LuaTable;

                    if (logicManagerTable != null)
                    {
                        // ��ȡ�߼���������Ϣ
                        string logicManagerFilePath = logicManagerTable["LogicManagerFilePath"] as string;
                        string logicManagerClassName = logicManagerTable["LogicManagerClassName"] as string;
                        LogicBaseManager logicManager = null;

                        if (!string.IsNullOrEmpty(logicManagerFilePath) && !string.IsNullOrEmpty(logicManagerClassName))
                        {
                            logicManager = LoadLogicManager(logicManagerFilePath, logicManagerClassName);
                        }
                        else
                        {
                            logicManager = new LogicBaseManager();
                        }

                        // ��ȡ��ʾ����
                        string logicManagerDisplayName = logicManagerTable["DisplayName"] as string;

                        if (!string.IsNullOrEmpty(logicManagerDisplayName))
                        {
                            logicManager.DisplayName = logicManagerDisplayName;
                        }                        

                        // ��ȡ�ű����ɲ����Ϣ
                        string compilerFilePath = logicManagerTable["CompilerFilePath"] as string;
                        string compilerClassName = logicManagerTable["CompilerClassName"] as string;

                        if (!string.IsNullOrEmpty(compilerFilePath) && !string.IsNullOrEmpty(compilerClassName))
                        {
                            logicManager.InitCompiler(compilerFilePath, compilerClassName);
                        }

                        // ����Ƿ���Ĭ�Ϸ���
                        int index = (int)(double)logicManagerTable["Index"];
                        diagramNameArray[index - 1] = logicManagerDisplayName;                        

                        // ��ȡ����Ԫ��Ϣ
                        LuaTable dataElementGroupTable = logicManagerTable["DataElement"] as LuaTable;

                        if (dataElementGroupTable != null)
                        {
                            foreach (object graphType in dataElementGroupTable.Keys)
                            {
                                LuaTable dataElementConfigTable = dataElementGroupTable[graphType] as LuaTable;

                                if (dataElementConfigTable != null)
                                {
                                    string dataElementFilePath = dataElementConfigTable["FilePath"] as string;
                                    string dataElementClassName = dataElementConfigTable["ClassName"] as string;

                                    logicManager.RegistDataElementInfo(graphType as string, dataElementFilePath, dataElementClassName);
                                }                                
                            }
                        }

                        logicManagerDictionary[logicManager.DisplayName] = logicManager;
                    }                    
                }

                foreach (string displayName in diagramNameArray)
                {
                    if (displayName != null)
                    {
                        diagramNameList.Add(displayName);
                    }
                }                
            }            
        }

        /// <summary>
        /// ��־����ı���
        /// </summary>
        public TextBoxX LogBox
        {
            set
            {
                logBox = value;
            }
        }

        /// <summary>
        /// ����ͼ������·������
        /// </summary>
        public string OutputDir
        {
            set
            {
                outputDir = value;
            }

            get
            {
                return outputDir;
            }
        }

        /// <summary>
        /// ��ͼ��������
        /// </summary>
        public List<string> DiagramNameList
        {
            get
            {
                return diagramNameList;
            }
        }

        /// <summary>
        /// ��ͼ����
        /// </summary>
        public Graphics GraphicsObject
        {
            get
            {
                if(graphicsObject == null)
                {
                    Bitmap bmp = new Bitmap(1, 1);
                    graphicsObject = Graphics.FromImage(bmp);
                }

                return graphicsObject;
            }
        }
        
        /// <summary>
        /// ��ȡͼƬ�ļ�
        /// </summary>
        /// <param name="resourceName">��Դ����</param>
        /// <returns>ͼƬ</returns>
        public Image LoadImageFromResource(string resourceName)
        {
            Image image = null;
            Assembly assembly = Assembly.GetExecutingAssembly();
            byte[] resourceBuffer = FlowChartEditor.Properties.Resources.ResourceManager.GetObject(resourceName) as byte[];

            if (resourceBuffer != null)
            {
                Stream imageStream = new MemoryStream(resourceBuffer);
                image = Image.FromStream(imageStream);
            }                     

            return image;
        }

        /// <summary>
        /// ��ȡ��ǰ�û���
        /// </summary>
        /// <returns>��ǰ�û���</returns>
        public string GetHostName()
        {
            string hostName = Dns.GetHostName();

            return hostName;
        }

        /// <summary>
        /// ��ȡ��ǰ�û������ַ
        /// </summary>
        /// <returns>��ǰ�û������ַ</returns>
        public string GetHostAddress()
        {            
            string ip = Dns.GetHostEntry(Dns.GetHostName()).AddressList[0].ToString();;

            return ip;
        }

        /// <summary>
        /// ��ȡ��ǰ�û����������ַ
        /// </summary>
        /// <returns>��ǰ�û����������ַ</returns>
        public List<string> GetHostAddresses()
        {
            List<string> addressList = new List<string>();

            foreach (IPAddress ipAddress in Dns.GetHostEntry(Dns.GetHostName()).AddressList)
            {
                addressList.Add(ipAddress.ToString());
            }

            return addressList;
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
                    StreamReader streamReader = new StreamReader(fileName, Encoding.GetEncoding(encodeInfo));
                    content = streamReader.ReadToEnd();
                    streamReader.Close();
                }
                catch (IOException ex)
                {
                    logManager.RecordLog("д�ļ�", ex.Message, LogManager.LogLevel.Warning);
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

                StreamWriter streamWriter = new StreamWriter(fileName, false, Encoding.GetEncoding(encodeInfo));
                streamWriter.Write(content);
                streamWriter.Close();
            }
            catch (IOException ex)
            {
                MessageBox.Show(string.Format("���ļ�{0}����IO����ʱ�����쳣: " + ex.Message, fileName));
                success = false;
            }

            return success;
        }        

        /// <summary>
        /// ��������ı�
        /// </summary>
        /// <param name="text">�����ı�</param>
        public void PrintText(string text)
        {
            logBox.AppendText(string.Format("{0}\r\n", text));
        }

        /// <summary>
        /// ��¼����򿪵Ļ�ͼID
        /// </summary>
        /// <param name="id">����򿪵Ļ�ͼID</param>
        public void RecordLatestDiagram(int id)
        {
            if (latestDiagramList.Count < 5)
            {
                if (!latestDiagramList.Contains(id))
                {
                    latestDiagramList.Add(id);
                }
            }
            else
            {
                if (!latestDiagramList.Contains(id))
                {
                    latestDiagramList.RemoveAt(0);
                    latestDiagramList.Insert(0, id);
                }                                
            }
        }

        /// <summary>
        /// ��ȡ�߼�������ʵ��
        /// </summary>
        /// <param name="chartMode">��ǰ�༭ģʽ</param>
        /// <returns>�߼�������ʵ��</returns>
        public LogicBaseManager GetLogicManager(string chartMode)
        {
            LogicBaseManager logicManager = logicManagerDictionary[chartMode];            

            return logicManager;
        }

        /// <summary>
        /// ת�����ݱ���ֵ
        /// </summary>
        /// <param name="value">����ֵ</param>
        /// <returns>����ֵ</returns>
        private string ConvertValue(string value)
        {
            string newValue = value;

            switch (value)
            {
                case "True":
                    {
                        newValue = "1";
                        break;
                    }
                case "False":
                    {
                        newValue = "0";
                        break;
                    }
                default:
                    {
                        break;
                    }
            }

            return newValue;
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <returns>���ݱ�</returns>
        public DataTable ReadDataTable(string fileName)
        {
            DataTable table = null;
            string content = ReadFile(fileName);

            if (content != null)
            {
                table = new DataTable();
                List<string> fieldList = new List<string>();

                bool firstLine = true;
                string[] lines = content.Split(new char[] { '\n' });

                foreach (string s in lines)
                {
                    string line = s.TrimEnd(new char[] { '\r' });

                    if (line != "") // �����յ�������
                    {
                        string[] data = line.Split(new char[] { '\t' });

                        if (firstLine)
                        {
                            firstLine = false;

                            for (int i = 0; i < data.Length; i++)
                            {
                                DataColumn dataColumn = new DataColumn(data[i]);
                                table.Columns.Add(dataColumn);
                                fieldList.Add(data[i]);
                            }
                        }
                        else
                        {
                            DataRow dataRow = table.NewRow();

                            for (int i = 0; i < data.Length; i++)
                            {
                                dataRow[i] = data[i];
                            }

                            table.Rows.Add(dataRow);
                        }
                    }
                }
            }            

            return table;
        }

        /// <summary>
        /// �������ݱ�����
        /// </summary>
        /// <param name="infoTable">���ݱ�</param>
        /// <param name="fileName">�ļ���</param>
        public void SaveDataTable(DataTable infoTable, string fileName)
        {
            StringBuilder content = new StringBuilder();
            StringBuilder headLine = new StringBuilder();

            foreach (DataColumn dataColumn in infoTable.Columns)
            {
                headLine.Append(string.Format("{0}\t", dataColumn.ColumnName));
            }

            headLine.Remove(headLine.Length - 1, 1);
            content.AppendLine(headLine.ToString());

            foreach (DataRow dataRow in infoTable.Rows)
            {
                StringBuilder line = new StringBuilder();

                foreach (DataColumn dataColumn in infoTable.Columns)
                {
                    line.Append(string.Format("{0}\t", ConvertValue(dataRow[dataColumn].ToString())));
                }

                line.Remove(line.Length - 1, 1);
                content.AppendLine(line.ToString());
            }

            SaveFile(fileName, content.ToString());
        }

        /// <summary>
        /// ����Ƿ���Ҫ����
        /// </summary>
        /// <returns>�Ƿ���Ҫ����</returns>
        public bool CheckRequireUpdate()
        {
            return false;

            bool requireUpdate = false;
            string ftpFolder = "ftp://changan/newtools/";
            string toolFolder = Application.StartupPath;

            List<string> fileList = new List<string>();
            fileList.Add("FlowChartEditor.exe");
            fileList.Add(@"Plugins/FlowChart/ActionNode.dll");
            fileList.Add(@"Plugins/FlowChart/AIChartLogicManager.dll");
            fileList.Add(@"Plugins/FlowChart/AIDiagramPlugin.dll");
            fileList.Add(@"Plugins/FlowChart/BaseExp.dll");
            fileList.Add(@"Plugins/FlowChart/CodeCompiler.dll");
            fileList.Add(@"Plugins/FlowChart/CodeProvider.dll");
            fileList.Add(@"Plugins/FlowChart/Compiler_Sword3.dll");
            fileList.Add(@"Plugins/FlowChart/Compiler_Sword3_AI.dll");
            fileList.Add(@"Plugins/FlowChart/ConditionLine.dll");
            fileList.Add(@"Plugins/FlowChart/ConditionNode.dll");
            fileList.Add(@"Plugins/FlowChart/ConstSelector.dll");
            fileList.Add(@"Plugins/FlowChart/CoordinateEditor.dll");
            fileList.Add(@"Plugins/FlowChart/DataElementLibrary.dll");
            fileList.Add(@"Plugins/FlowChart/DataInteractor.dll");
            fileList.Add(@"Plugins/FlowChart/DBManager.dll");
            fileList.Add(@"Plugins/FlowChart/EventNode.dll");
            fileList.Add(@"Plugins/FlowChart/FlowChartDataLibrary.dll");
            fileList.Add(@"Plugins/FlowChart/FlowChartLogicManager.dll");
            fileList.Add(@"Plugins/FlowChart/FlowChartVariable.dll");
            fileList.Add(@"Plugins/FlowChart/GraphElementLibrary.dll");
            fileList.Add(@"Plugins/FlowChart/PatrolIO.dll");
            fileList.Add(@"Plugins/FlowChart/QuestSelector.dll");
            fileList.Add(@"Plugins/FlowChart/TextEditor.dll");
            fileList.Add(@"Plugins/FlowChart/VarManager.dll");            

            try
            {
                foreach (string fileName in fileList)
                {
                    string ftpFileName = string.Format("{0}{1}", ftpFolder, fileName);
                    string toolFileName = Path.Combine(toolFolder, fileName);

                    FtpWebRequest ftp = FtpWebRequest.Create(new Uri(ftpFileName)) as FtpWebRequest;
                    ftp.UseBinary = true;
                    ftp.KeepAlive = false;                

                    FileInfo fi = new FileInfo(toolFileName);

                    if (fi.Exists) // �����ļ��Ѿ�����
                    {
                        ftp.Method = WebRequestMethods.Ftp.GetDateTimestamp;
                        FtpWebResponse response = ftp.GetResponse() as FtpWebResponse;
                        DateTime newEditTime = response.LastModified;
                        response.Close();

                        DateTime localEditTime = fi.LastWriteTime;

                        if (newEditTime > localEditTime) // ��Ҫ�����ļ�
                        {
                            requireUpdate = true;
                            break;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                logManager.RecordLog("������", ex.Message, LogManager.LogLevel.Warning);
            }            

            return requireUpdate;
        }
    }
}
