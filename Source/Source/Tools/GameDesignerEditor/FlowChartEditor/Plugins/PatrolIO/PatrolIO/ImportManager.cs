using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Data.SqlClient;
using System.Data;

namespace PatrolIO
{
    class ImportManager
    {
        private string[] fields; // ��������
        private string[] values; // ����������
        private List<string> valuesList = new List<string>(); // ����������
        private bool loadedFields = false; // �Ƿ��Ѿ���ӹ��������� 
        private string path; // �ļ�·��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="path">�ļ�·��</param
        public ImportManager(string path)
        {
            this.path = path;
        }

        /// <summary>
        /// ��������
        /// </summary>
        public string[] Fields
        {
            get
            {
                return fields;
            }
        }

        /// <summary>
        /// ����������
        /// </summary>
        public string[] Values
        {
            get
            {
                return values;
            }
        }

        /// <summary>
        /// ����·����Ϣ
        /// </summary>
        public bool LoadPatrolFromPath()
        {
            bool loadSucess = true; // �Ƿ���ɹ�
            string indexFilePath; // �����ļ�Ŀ¼
            string relevateFileName; // ���·����
            string tempFolder; // ��ʱ�ļ���
            string patrolID; // ·����ID

            try
            {
                string patrolInfoPath = Path.Combine(path, "settings\\map");
                DirectoryInfo di = new DirectoryInfo(patrolInfoPath);
                foreach(DirectoryInfo map in di.GetDirectories())
                {
                    tempFolder = Path.Combine(patrolInfoPath, map.Name);
                    tempFolder = Path.Combine(tempFolder, "PatrolPath");
                    relevateFileName = "settings\\map\\" + map.Name;
                    relevateFileName += "\\PatrolPath\\";
                    indexFilePath = Path.Combine(tempFolder, "PatrolPathList.tab");

                    FileInfo fi = new FileInfo(indexFilePath);
                    bool firstLine = true; // �Ƿ��Ѿ�������һ��

                    if(fi.Exists)
                    {
                        StreamReader sr = new StreamReader(indexFilePath, Encoding.GetEncoding("gb2312"));
                        string text = sr.ReadLine();
                        string[] data;

                        while(text != null)
                        {                        
                            if(!firstLine)
                            {
                                data = text.Split(new char[] { '\t' }, StringSplitOptions.RemoveEmptyEntries);
                                patrolID = data[0];
                                if(data.Length == 4) // �����¸�ʽѲ��·�������ļ�
                                {
                                    data = data[3].Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);  
                                }
                                else // ����ɸ�ʽѲ��·�������ļ�
                                {
                                    data = data[1].Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
                                }
                                string fileName = Path.Combine(tempFolder, data[3]);
                                LoadPatrolInfo(fileName, relevateFileName + data[3], map.Name, patrolID);
                            }
                            else
                            {
                                firstLine = false;
                            }
                            text = sr.ReadLine();
                        }

                        sr.Close();
                    }
                    else
                    {
                        MessageBox.Show(string.Format("��ͼ {0} ��·�������ļ������ڣ�", map.Name));
                    }
                }                               
            }
            catch (IOException ex)
            {
                MessageBox.Show("�ڶ�ȡ·���ļ���Ϣʱ����IO�쳣: " + ex.ToString());
                loadSucess = false;
            }

            values = new string[valuesList.Count + 1];
            values[0] = "empty line";
            for (int i = 0; i < valuesList.Count; i++)
            {
                values[i + 1] = valuesList[i];
            }

            return loadSucess;
        }

        /// <summary>
        /// ���ļ��ж�ȡ����
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="relevateFileName">���·����</param>
        /// <param name="mapName">��ͼ��</param>
        /// <param name="patrolID">·��ID</param>
        private void LoadPatrolInfo(string fileName, string relevateFileName, string mapName, string patrolID)
        {
            string dataPath = Path.Combine(path, string.Format("data\\source\\maps\\{0}\\{0}.Map.Logical", mapName));
            FileInfo dataFile = new FileInfo(dataPath);

            if(dataFile.Exists)
            {
                ImportData importData = new ImportData(dataPath);
                
                FileInfo fi = new FileInfo(fileName);
                if (fi.Exists)
                {
                    try
                    {
                        StreamReader sr = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                        string text = sr.ReadLine();
                        List<string> list = new List<string>();
                        string[] data;
                        int index = -1;
                        string appendText;
                        string patrolName;

                        while (text != null)
                        {
                            data = text.Split(new char[] { '\t' }, StringSplitOptions.RemoveEmptyEntries);

                            if (index == -1) // �����һ�У���������
                            {
                                if (!loadedFields) // ��û��д������
                                {
                                    fields = new string[5 + data.Length];
                                    fields[0] = "FileName";
                                    fields[1] = "MapName";
                                    fields[2] = "PatrolID";                                    
                                    fields[3] = "PatrolName";
                                    fields[4] = "PatrolPointID";

                                    for (int i = 0; i < data.Length; i++)
                                    {
                                        fields[5 + i] = data[i];
                                    }

                                    loadedFields = true;
                                }
                            }
                            else // ����������
                            {
                                patrolName = importData.GetKeyString(string.Format("WayPointSet{0}", int.Parse(patrolID) - 1), "szName");
                                appendText = string.Format("{0}\t{1}\t{2}\t{3}\t{4}\t", relevateFileName, mapName, patrolID, patrolName, index - 1);                       
                                list.Add(appendText + text);
                            }

                            index++;
                            text = sr.ReadLine();
                        }

                        valuesList.AddRange(list);
                    }
                    catch (IOException ex)
                    {
                        MessageBox.Show("�ڶ�ȡ·���ļ�ʱ����IO�쳣: " + ex.ToString());
                    }
                }
            }
            else
            {
                FileInfo fi = new FileInfo(fileName);
                if (fi.Exists)
                {
                    try
                    {
                        StreamReader sr = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                        string text = sr.ReadLine();
                        List<string> list = new List<string>();
                        string[] data;
                        int index = -1;
                        string appendText;
                        string patrolName;

                        while (text != null)
                        {
                            data = text.Split(new char[] { '\t' }, StringSplitOptions.RemoveEmptyEntries);

                            if (index == -1) // �����һ�У���������
                            {
                                if (!loadedFields) // ��û��д������
                                {
                                    fields = new string[5 + data.Length];
                                    fields[0] = "FileName";
                                    fields[1] = "MapName";
                                    fields[2] = "PatrolID";
                                    fields[3] = "PatrolName";
                                    fields[4] = "PatrolPointID";

                                    for (int i = 0; i < data.Length; i++)
                                    {
                                        fields[5 + i] = data[i];
                                    }

                                    loadedFields = true;
                                }
                            }
                            else // ����������
                            {
                                patrolName = string.Format("·��{0}", index + 1);
                                appendText = string.Format("{0}\t{1}\t{2}\t{3}\t{4}\t", relevateFileName, mapName, patrolID, patrolName, index - 1);
                                list.Add(appendText + text);
                            }

                            index++;
                            text = sr.ReadLine();
                        }

                        valuesList.AddRange(list);
                    }
                    catch (IOException ex)
                    {
                        MessageBox.Show("�ڶ�ȡ·���ļ�ʱ����IO�쳣: " + ex.ToString());
                    }
                }
            }            
        } 

        /// <summary>
        /// ��ȡ�̵���߱����ڵ�Ŀ¼
        /// </summary>
        /// <returns>�̵���߱����ڵ�Ŀ¼</returns>
        private string GetLoadPath()
        {
            string resultFolder = null;
            FolderBrowserDialog openFolderDialog1 = new FolderBrowserDialog();
            openFolderDialog1.Description = "��ѡ��ͻ������ڵ�Ŀ¼";
            openFolderDialog1.SelectedPath = path;
            if (openFolderDialog1.ShowDialog() == DialogResult.OK)
            {
                resultFolder = openFolderDialog1.SelectedPath;
            }

            return resultFolder;
        } 
    }
}
