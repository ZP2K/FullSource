using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.IO;
using System.Windows.Forms;

namespace IniFileIO
{
    class ExportMultiFile
    {
        private ArrayList fields; // ��������
        private string[] values; // ����������
        private List<string> excludeFields = new List<string>(); // ������������
        private List<int> excludeIndexList = new List<int>(); // ���������������
        private List<string> includeFields = new List<string>(); // ����������
        private List<int> includeIndexList = new List<int>(); // �������������        

        private string path = null; // �ļ�����·��
        private List<string> exportedFiles = new List<string>(); // �Ѿ��������ļ����б�
        private string exportPath; // �����ļ���·��

        private bool autoExport = false; // �Ƿ��Զ����� 

        // ����������
        private enum ExportType { None, Exclude, Include };
        private ExportType exportType = ExportType.None;

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="fields">��������</param>
        /// <param name="values">����������</param>
        public ExportMultiFile(ArrayList fields, string[] values)
        {
            this.fields = fields;
            this.values = values;
        }

        /// <summary>
        /// �Ƿ��Զ�����
        /// </summary>
        public bool AutoExport
        {
            set
            {
                autoExport = value;
            }
        }

        /// <summary>
        /// �����ļ���·������
        /// </summary>
        public string ExportPath
        {
            get
            {
                return exportPath;
            }
        }

        /// <summary>
        /// ���ò���������
        /// </summary>
        /// <param name="s">������������</param>
        public void AddExcludeFields(string s)
        {
            excludeFields.Add(s);
            exportType = ExportType.Exclude;            
        }

        /// <summary>
        /// ���õ�������
        /// </summary>
        /// <param name="s">����������</param>
        public void AddIncludeFields(string s)
        {
            includeFields.Add(s);
            exportType = ExportType.Include;
        }

        /// <summary>
        /// �����ļ�����·��
        /// </summary>
        /// <param name="path">�ļ�����·��</param>
        public void SetPath(string path)
        {
            this.path = path;
            this.exportPath = path;
        }

        /// <summary>
        /// �����ļ�
        /// </summary>
        /// <param name="folderName">�����ļ���������</param>
        /// <param name="fileName">�����ļ�������</param>
        /// <returns>�Ƿ񵼳��ɹ�</returns>
        public bool ExportFiles(string folderName, string fileName)
        {
            Hashtable fileTable = new Hashtable();
            string resultFolder = "";

            FolderBrowserDialog openFolderDialog1 = new FolderBrowserDialog();
            openFolderDialog1.Description = "��ѡ�񵼳��ļ��ı���Ŀ¼";
            openFolderDialog1.ShowNewFolderButton = true;
            if (path == null)
            {
                openFolderDialog1.RootFolder = Environment.SpecialFolder.MyComputer;
            }
            else
            {
                openFolderDialog1.SelectedPath = path;
            }

            if (openFolderDialog1.ShowDialog() == DialogResult.OK)
            {
                resultFolder = openFolderDialog1.SelectedPath + "\\";
                exportPath = openFolderDialog1.SelectedPath;

                string headline = string.Empty;
                int fileNameIndex = -1;
                int folderNameIndex = -1;

                for (int i = 0; i < fields.Count; i++)
                {
                    string field = (fields[i] as string).TrimEnd(new char[] { '\r', '\n' });
                    if (excludeFields.Contains(field))
                    {
                        excludeIndexList.Add(i);
                    }
                    else
                    {
                        headline = headline + field + "\t";
                    }

                    if (field == fileName)
                    {
                        fileNameIndex = i;
                    }
                    else if(field == folderName)
                    {
                        folderNameIndex = i;
                    }
                }

                if (fileNameIndex == -1 || folderNameIndex == -1) // û���ҵ������ļ���������
                {
                    MessageBox.Show("û���ҵ������ļ��к��ļ���������!");
                    return false;
                }

                headline = headline.TrimEnd(new char[] { '\r', '\n', '\t' }); // ȥ��������Ϣ

                foreach (string s in values) // ������������е�hash����
                {
                    if (s == "") // ���Կ���
                    {
                        continue;
                    }
                    string[] data = s.Split('\t');
                    string fullPath = Path.Combine(resultFolder, Path.Combine(data[folderNameIndex], data[fileNameIndex]));

                    if (fileTable[fullPath] != null)
                    {
                        List<string> lines = fileTable[fullPath] as List<string>;
                        string t = string.Empty;
                        for (int i = 0; i < data.Length; i++)
                        {
                            if (!excludeIndexList.Contains(i))
                            {
                                t = t + data[i] + "\t";
                            }
                        }
                        //lines.Add(t.TrimEnd('\t'));
                        lines.Add(t.Substring(0, t.Length - 1));
                    }
                    else
                    {
                        List<string> lines = new List<string>();
                        lines.Add(headline);
                        string t = string.Empty;
                        for (int i = 0; i < data.Length; i++)
                        {
                            if (!excludeIndexList.Contains(i))
                            {
                                t = t + data[i] + "\t";
                            }
                        }
                        //lines.Add(t.TrimEnd('\t'));
                        lines.Add(t.Substring(0, t.Length - 1));
                        fileTable[fullPath] = lines;
                    }
                }

                List<string> fileNameList = new List<string>();
                foreach (string s in fileTable.Keys)
                {
                    fileNameList.Add(s);
                }

                // ѡ��Ҫ�������ļ�
                ChooseFileForm cForm = new ChooseFileForm("��ѡ��Ҫ�������ļ�", fileNameList);
                DialogResult result = cForm.ShowDialog();
                if (result != DialogResult.OK)
                {
                    return false;
                }

                // �����ļ�
                foreach (string s in fileTable.Keys)
                {
                    if (fileNameList.Contains(s))
                    {
                        List<string> lines = fileTable[s] as List<string>;
                        SaveData(s, lines);
                    }
                }

                // ��ʾ�����ļ��б�
                string message = "�����ļ�:\n\n";
                foreach (string s in fileNameList)
                {
                    message = message + string.Format("{0}.tab\n", s);
                }
                message = message + "\n\n";
                message = message + "�Ѿ��ɹ�����!";
                MessageBox.Show(message);
            }
 
            return true;             
        }

        /// <summary>
        /// �����ļ�
        /// </summary>
        /// <param name="fileName">�����ļ�������</param>
        /// <returns>�Ƿ񵼳��ɹ�</returns>
        public bool ExportFiles(string fileName)
        {
            Hashtable fileTable = new Hashtable();

            if (exportType == ExportType.None)
            {
                MessageBox.Show("û�����õ������ݵ����ͣ�", "��������",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            if (!autoExport)
            {
                FolderBrowserDialog openFolderDialog = new FolderBrowserDialog();
                openFolderDialog.Description = "��ѡ�񵼳��ļ��ı���Ŀ¼";
                openFolderDialog.ShowNewFolderButton = true;
                
                if (string.IsNullOrEmpty(path))
                {
                    openFolderDialog.RootFolder = Environment.SpecialFolder.MyComputer;
                }
                else
                {
                    openFolderDialog.SelectedPath = path;
                }

                if (openFolderDialog.ShowDialog() == DialogResult.OK)
                {
                    path = openFolderDialog.SelectedPath;
                }
                else
                {
                    return false;
                }
            }                                      

            StringBuilder headLine = new StringBuilder();
            int fileNameIndex = -1;

            for (int i = 0; i < fields.Count; i++)
            {
                string field = (fields[i] as string).TrimEnd(new char[] { '\r', '\n' });

                switch (exportType)
                {
                    case ExportType.Exclude:
                        {
                            if (excludeFields.Contains(field))
                            {
                                excludeIndexList.Add(i);
                            }
                            else
                            {                                    
                                headLine.Append(string.Format("{0}\t", field));
                            }

                            break;
                        }
                    case ExportType.Include:
                        {
                            if (includeFields.Contains(field))
                            {
                                includeIndexList.Add(i);
                                headLine.Append(string.Format("{0}\t", field));
                            }

                            break;
                        }
                }

                if (field == fileName)
                {
                    fileNameIndex = i;
                }
            }

            if (fileNameIndex == -1) // û���ҵ������ļ���������
            {
                MessageBox.Show("û���ҵ������ļ��������У�");
                return false;
            }

            headLine.Remove(headLine.Length - 1, 1);                

            foreach (string s in values) // ������������е�hash����
            {
                if (string.IsNullOrEmpty(s)) // ���Կ���
                {
                    continue;
                }

                string[] data = s.Split('\t');

                if (fileTable[Path.Combine(path, data[fileNameIndex])] != null)
                {
                    List<string> lines = fileTable[Path.Combine(path, data[fileNameIndex])] as List<string>;
                    StringBuilder line = new StringBuilder();

                    for (int i = 0; i < data.Length; i++)
                    {
                        switch (exportType)
                        {
                            case ExportType.Exclude:
                                {
                                    if (!excludeIndexList.Contains(i))
                                    {                                            
                                        line.Append(string.Format("{0}\t", data[i]));
                                    }

                                    break;
                                }
                            case ExportType.Include:
                                {
                                    if (includeIndexList.Contains(i))
                                    {
                                        line.Append(string.Format("{0}\t", data[i]));
                                    }

                                    break;
                                }
                        }                            
                    }

                    line.Remove(line.Length - 1, 1);
                    lines.Add(line.ToString());
                }
                else
                {
                    List<string> lines = new List<string>();
                    lines.Add(headLine.ToString());
                    StringBuilder line = new StringBuilder();

                    for (int i = 0; i < data.Length; i++)
                    {
                        switch (exportType)
                        {
                            case ExportType.Exclude:
                                {
                                    if (!excludeIndexList.Contains(i))
                                    {
                                        line.Append(string.Format("{0}\t", data[i]));
                                    }

                                    break;
                                }
                            case ExportType.Include:
                                {
                                    if (includeIndexList.Contains(i))
                                    {
                                        line.Append(string.Format("{0}\t", data[i]));
                                    }

                                    break;
                                }
                        }
                    }

                    line.Remove(line.Length - 1, 1);
                    lines.Add(line.ToString());
                    fileTable[Path.Combine(path, data[fileNameIndex])] = lines;
                }
            }

            List<string> fileNameList = new List<string>();

            foreach (string s in fileTable.Keys)
            {
                fileNameList.Add(s);
            }

            // ѡ��Ҫ�������ļ�
            if (!autoExport)
            {
                ChooseFileForm cForm = new ChooseFileForm("��ѡ��Ҫ�������ļ�", fileNameList);
                DialogResult result = cForm.ShowDialog();

                if (result != DialogResult.OK)
                {
                    return false;
                }
            }            

            // �����ļ�
            foreach (string s in fileTable.Keys)
            {
                if (fileNameList.Contains(s))
                {
                    List<string> lines = fileTable[s] as List<string>;
                    SaveData(s, lines);
                }
            }

            // ��ʾ�����ļ��б�
            string message = "�����ļ�:\n\n";

            foreach (string s in fileNameList)
            {
                message = message + string.Format("{0}\n", s);
            }

            message = message + "\n\n";
            message = message + "�Ѿ��ɹ�����!";
            MessageBox.Show(message);            

            return true;
        }

        /// <summary>
        /// ����Ĭ��������
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="values">��������</param>
        private void SaveData(string fileName, List<string> values)
        {
            try
            {
                FileInfo fi = new FileInfo(fileName);
                StreamWriter sw;

                if (!fi.Exists) // �ļ�������
                {                    
                    string path = Path.GetDirectoryName(fileName); // ��ȡĿ¼
                    if(!Directory.Exists(path)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(path); // �ȴ���Ŀ¼���ٴ����ļ�
                    }
                    FileStream fs = fi.Create();
                    fs.Close();
                    // sw = fi.CreateText(); // �½��ı�
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }
                else
                {
                    if(!exportedFiles.Contains(fileName)) // ��һ��д�ļ�
                    {
                        // sw = fi.CreateText();
                        sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                        exportedFiles.Add(fileName);
                    }
                    else // �������
                    {
                        // sw = fi.AppendText(); // ����ı�
                        sw = new StreamWriter(fileName, true, Encoding.GetEncoding("gb2312"));
                    }              
                }
                 
                foreach (string s in values)
                {
                    sw.WriteLine(s);
                }
                sw.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("���ļ�{0}����IO����ʱ�����쳣: " + ex.ToString(), fileName));
            }
        }
    }
}
