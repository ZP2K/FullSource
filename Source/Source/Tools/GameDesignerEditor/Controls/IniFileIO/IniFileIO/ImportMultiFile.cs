using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace IniFileIO
{
    class ImportMultiFile
    {
        private string rootPath; // �����Ŀ¼
        private string[] fields; // ����������
        private string[] values; // ����������
        private string importPath; // ��ǰ�����ļ���Ŀ¼
        private int beginLine = 1; // �ǵ�һ���ļ���ʼ��ȡ�������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="rootPath">�����Ŀ¼</param>
        public ImportMultiFile(string rootPath)
        {
            this.rootPath = rootPath;
        }

        /// <summary>
        /// ��������������
        /// </summary>
        public string[] Fields
        {
            get
            {
                return fields;
            }
        }

        /// <summary>
        /// ��������������
        /// </summary>
        public string[] Values
        {
            get
            {
                return values;
            }
        }

        /// <summary>
        /// ��ǰ�����ļ���Ŀ¼
        /// </summary>
        public string ImportPath
        {
            get
            {
                return importPath;
            }
        }

        /// <summary>
        /// �ǵ�һ���ļ���ʼ��ȡ�������
        /// </summary>
        public int BeginLine
        {
            set
            {
                beginLine = value;
            }
        }

        /// <summary>
        /// �����ļ�
        /// </summary>
        /// <param name="fileName">��¼�ļ���������</param>
        /// <returns>�Ƿ���ɹ�</returns>
        public bool ImportFiles(string fileName)
        {
            bool success = true; // �Ƿ���ɹ�
            bool firstFile = true; // �Ƿ����ڶ�ȡ��һ���ļ�

            FolderBrowserDialog openFolderDialog1 = new FolderBrowserDialog();
            openFolderDialog1.Description = "��ѡ�����ļ����ڵ�Ŀ¼";
            openFolderDialog1.ShowNewFolderButton = false;

            if (rootPath == null)
            {
                openFolderDialog1.RootFolder = Environment.SpecialFolder.MyComputer;
            }
            else
            {
                openFolderDialog1.SelectedPath = rootPath;
            }

            if (openFolderDialog1.ShowDialog() == DialogResult.OK)
            {
                importPath = openFolderDialog1.SelectedPath;
                DirectoryInfo di = new DirectoryInfo(importPath);
                FileInfo[] files = di.GetFiles();
                List<string> data; // ������
                string[] headLine; // ���������ݵĵ�һ��
                List<string> valueList = new List<string>(); // ���������ݵ�����
                List<string> fileNameList = new List<string>(); // ׼��������ļ�����

                foreach(FileInfo fi in files)
                {
                    fileNameList.Add(fi.FullName);          
                }

                
                ChooseFileForm cForm = new ChooseFileForm("��ѡ��Ҫ������ļ�", fileNameList);
                DialogResult result = cForm.ShowDialog();

                if(result == DialogResult.OK) // �û�ȷ��Ҫ������ļ�
                {
                    fileNameList = cForm.FileNameList;
                    foreach(string s in fileNameList)
                    {
                        if (firstFile) // �ж��Ƿ����ڶ�ȡ��һ���ļ�
                        {
                            data = ReadFile(s);
                            headLine = data[0].Split(new char[] { '\t' });

                            fields = new string[headLine.Length + 1];
                            for (int i = 0; i < headLine.Length; i++)
                            {
                                fields[i] = headLine[i];
                            }
                            fields[headLine.Length] = fileName;

                            valueList.Add(data[0] + "\t" + fileName);
                            for (int i = 1; i < data.Count; i++)
                            {
                                valueList.Add(data[i] + "\t" + Path.GetFileName(s));
                            }

                            firstFile = false;
                        }
                        else
                        {
                            data = ReadFile(s);

                            for (int i = beginLine; i < data.Count; i++)
                            {
                                valueList.Add(data[i] + "\t" + Path.GetFileName(s));
                            }
                        }
                    }

                    values = new string[valueList.Count];
                    for (int i = 0; i < valueList.Count; i++)
                    {
                        values[i] = valueList[i];
                    }
                }
                else
                {
                    success = false;
                } 
            }
            else
            {
                success = false;
            }

            return success;
        }

        /// <summary>
        /// ��ȡ���ļ�����
        /// </summary>
        /// <param name="fileName">�ļ�ȫ·����</param>
        /// <returns>���ļ�����</returns>
        private string GetFolderName(string fileName)
        {
            string[] data = fileName.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
            if(data.Length > 1)
            {
                return data[data.Length - 2];
            }
            else
            {
                return "";
            }
        }

        /// <summary>
        /// �����ļ�
        /// </summary>
        /// <param name="folderName">��¼�ļ�����������</param>
        /// <param name="fileName">��¼�ļ���������</param>
        /// <returns>�Ƿ���ɹ�</returns>
        public bool ImportFiles(string folderName, string fileName)
        {
            bool success = true; // �Ƿ���ɹ�
            bool firstFile = true; // �Ƿ����ڶ�ȡ��һ���ļ�

            FolderBrowserDialog openFolderDialog1 = new FolderBrowserDialog();
            openFolderDialog1.Description = "��ѡ�����ļ����ڵ�Ŀ¼";
            openFolderDialog1.ShowNewFolderButton = false;

            if (rootPath == null)
            {
                openFolderDialog1.RootFolder = Environment.SpecialFolder.MyComputer;
            }
            else
            {
                openFolderDialog1.SelectedPath = rootPath;
            }

            if (openFolderDialog1.ShowDialog() == DialogResult.OK)
            {
                importPath = openFolderDialog1.SelectedPath;
                DirectoryInfo directory = new DirectoryInfo(importPath);
                DirectoryInfo[] folders = directory.GetDirectories();
                List<string> data; // ������
                string[] headLine; // ���������ݵĵ�һ��
                List<string> valueList = new List<string>(); // ���������ݵ�����
                List<string> fileNameList = new List<string>(); // ׼��������ļ�����

                foreach (DirectoryInfo di in folders)
                {
                    FileInfo[] files = di.GetFiles();
                    foreach(FileInfo fi in files)
                    {
                        fileNameList.Add(fi.FullName);
                    }  
                }

                ChooseFileForm cForm = new ChooseFileForm("��ѡ��Ҫ������ļ�", fileNameList);
                DialogResult result = cForm.ShowDialog();

                if (result == DialogResult.OK) // �û�ȷ��Ҫ������ļ�
                {
                    fileNameList = cForm.FileNameList;
                    foreach (string s in fileNameList)
                    {
                        if (firstFile) // �ж��Ƿ����ڶ�ȡ��һ���ļ�
                        {
                            data = ReadFile(s);
                            headLine = data[0].Split(new char[] { '\t' });

                            fields = new string[headLine.Length + 2];
                            for (int i = 0; i < headLine.Length; i++)
                            {
                                fields[i] = headLine[i];
                            }
                            fields[headLine.Length] = folderName;
                            fields[headLine.Length + 1] = fileName;

                            valueList.Add(data[0] + "\t" + folderName + "\t" + fileName);
                            for (int i = 1; i < data.Count; i++)
                            {
                                valueList.Add(data[i] + "\t" + GetFolderName(s) + "\t" + Path.GetFileName(s));
                            }

                            firstFile = false;
                        }
                        else
                        {
                            data = ReadFile(s);

                            for (int i = beginLine; i < data.Count; i++)
                            {
                                valueList.Add(data[i] + "\t" + GetFolderName(s) + "\t" + Path.GetFileName(s));
                            }
                        }
                    }

                    values = new string[valueList.Count];
                    for (int i = 0; i < valueList.Count; i++)
                    {
                        values[i] = valueList[i];
                    }
                }
            }
            else
            {
                success = false;
            }

            return success;
        }

        /// <summary>
        /// ��ȡ�ļ��е�����
        /// </summary>
        /// <param name="fileName">��ȫ·�����ļ���</param>
        /// <returns>�ļ��е���������</returns>
        public List<string> ReadFile(string fileName)
        {
            List<string> dataList = new List<string>(); // ��ȡ����������

            try
            {
                StreamReader sr = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                string text = sr.ReadLine();

                while(text != null)
                {                   
                    dataList.Add(text);
                    text = sr.ReadLine();
                }
            }
            catch(IOException ex)
            {
                MessageBox.Show(string.Format("�ڶ��ļ�{0}���ж�ȡ�����ǲ���IO�쳣: ", fileName) + ex.ToString());
            }

            return dataList;
        }
    }
}
