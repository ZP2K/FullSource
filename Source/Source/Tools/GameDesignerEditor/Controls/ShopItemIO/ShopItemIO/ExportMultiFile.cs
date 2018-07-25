using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.IO;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Data;

namespace ShopItemIO
{
    class ExportMultiFile
    {
        private ArrayList fields; // ��������
        private string[] values; // ����������
        private List<string> excludeFields = new List<string>(); // ������������
        private List<int> excludeIndexList = new List<int>(); // ���������������
        private Hashtable fileTable = new Hashtable(); // ��¼�����ļ����ݵ�hash��
        private string path = null; // �ļ�����·��
        private SqlConnection conn; // sql����
        private List<string> exportedFiles = new List<string>(); // �Ѿ��������ļ����б�
        private string exportPath; // �����ļ���·��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="fields">��������</param>
        /// <param name="values">����������</param>
        public ExportMultiFile(SqlConnection conn, ArrayList fields, string[] values)
        {
            this.conn = conn;
            this.fields = fields;
            this.values = values;
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
            bool exportSuccess = false;
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

                // �����ļ�
                foreach (string s in fileTable.Keys)
                {
                    if (fileNameList.Contains(s))
                    {
                        List<string> lines = fileTable[s] as List<string>;
                        string tempValue;

                        // �������ݽ�������
                        bool finished = true;

                        if (lines.Count > 100)
                        {
                            MessageBox.Show(string.Format("�̵�{0}�ĵ����Ѿ���������100��\r\n\r\n���ļ������ᱻ������", s), "���µ�����Ϣ",
                                                          MessageBoxButtons.OK, MessageBoxIcon.Information);
                            continue;
                        }

                        for (int i = 0; i < lines.Count - 1; i++)
                        {
                            finished = true;

                            for (int j = 1; j < lines.Count - i - 1; j++)
                            {
                                if (IsLargeThan(lines[j], lines[j + 1]))
                                {
                                    tempValue = lines[j + 1];
                                    lines[j + 1] = lines[j];
                                    lines[j] = tempValue;
                                    finished = false;
                                }
                            }

                            if (finished) // �Ѿ�����Ҫ������
                            {
                                break;
                            }
                        }

                        SaveData(s, lines);
                    }
                }

                // ��ʾ�����ļ��б�
                string message = "�ļ��Ѿ��ɹ�������";
                MessageBox.Show(message);
                exportSuccess = true;
            }

            return exportSuccess;
        }

        /// <summary>
        /// ��ȡ������Ϣ
        /// </summary>
        /// <param name="tableName">���߱���</param>
        /// <param name="id">����id</param>
        /// <param name="fieldName">�����ֶ�</param>
        /// <returns>�����ֶ�</returns>
        private string GetInfo(string tableName, string id, string fieldName)
        {
            string value = null;

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                SqlCommand cmd = conn.CreateCommand();
                string sqlString = string.Format("SELECT {0} FROM {1} WHERE ID = '{2}'", fieldName, tableName, id);
                cmd.CommandText = sqlString;
                object o = cmd.ExecuteScalar();
                if (o != null)
                {
                    value = o.ToString();
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ������Ϣʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            if (value == "")
            {
                value = null;
            }

            return value;
        }

        /// <summary>
        /// �����ֵ�ַ���a��Ȩֵ�Ƿ��b��
        /// </summary>
        /// <param name="a">�����ַ���</param>
        /// <param name="b">�����ַ���</param>
        /// <returns>a��Ȩֵ�Ƿ��b��</returns>
        private bool IsLargeThan(string a, string b)
        {
            bool result = false;

            string[] arrayA = a.Split(new char[] { '\t' });
            string[] arrayB = b.Split(new char[] { '\t' });

            // �ȱȽ�ReputeLevel
            int reputeLevelA = 0;
            int reputeLevelB = 0;

            if(arrayA[5] != "")
            {
                reputeLevelA = int.Parse(arrayA[5]);
            }

            if(arrayB[5] != "")
            {
                reputeLevelB = int.Parse(arrayB[5]);
            }

            if(reputeLevelA > reputeLevelB)
            {
                result = true;
            }
            else if(reputeLevelA < reputeLevelB)
            {
                result = false;
            }
            else
            {
                // �Ƚ�TabType
                int tabTypeNumberA = 0;
                int tabTypeNumberB = 0;

                string tabTypeA = arrayA[0];
                switch (tabTypeA)
                {
                    case "Other":
                        {
                            tabTypeNumberA = 1;
                            break;
                        }
                    case "CustomWeapon":
                        {
                            tabTypeNumberA = 2;
                            break;
                        }
                    case "CustomArmor":
                        {
                            tabTypeNumberA = 3;
                            break;
                        }
                    case "CustomTrinket":
                        {
                            tabTypeNumberA = 4;
                            break;
                        }
                }

                string tabTypeB = arrayB[0];
                switch (tabTypeB)
                {
                    case "Other":
                        {
                            tabTypeNumberB = 1;
                            break;
                        }
                    case "CustomWeapon":
                        {
                            tabTypeNumberB = 2;
                            break;
                        }
                    case "CustomArmor":
                        {
                            tabTypeNumberB = 3;
                            break;
                        }
                    case "CustomTrinket":
                        {
                            tabTypeNumberB = 4;
                            break;
                        }
                }

                if (tabTypeNumberA > tabTypeNumberB)
                {
                    result = true;
                }
                else if (tabTypeNumberA < tabTypeNumberB)
                {
                    result = false;
                }
                else
                {
                    if (tabTypeNumberA == 1) // ���߱��ڵıȽ�
                    {
                        // �Ƚ�_CATEGORY
                        string itemIndexA = arrayA[1];
                        string itemIndexB = arrayB[1];
                        string catA = GetInfo("Other", itemIndexA, "_CATEGORY");
                        string catB = GetInfo("Other", itemIndexB, "_CATEGORY");
                        string _strlevelA = GetInfo("Other", itemIndexA, "RequireLevel");
                        string _strlevelB = GetInfo("Other", itemIndexB, "RequireLevel");
                        int levelA = _strlevelA != null ? Convert.ToInt32(_strlevelA) : 0;
                        int levelB = _strlevelB != null ? Convert.ToInt32(_strlevelB) : 0;

                        if (catA == null)
                        {
                            result = false;
                        }
                        else if (catB == null)
                        {
                            result = true;
                        }
                        else if (catA.CompareTo(catB) > 0)
                        {
                            result = true;
                        }
                        else if (catA.CompareTo(catB) < 0)
                        {
                            result = false;
                        }
                        else if (levelA != levelB)
                        {
                            result = levelA > levelB;
                        }
                        else
                        {
                            // �Ƚ�Price
                            int priceA = int.Parse(arrayA[6]);
                            int priceB = int.Parse(arrayB[6]);
                            result = priceA > priceB;
                        }
                    }
                    else // ������ڵıȽ�
                    {
                        string tableName = null;
                        string itemIndexA = arrayA[1];
                        string itemIndexB = arrayB[1];

                        switch (tabTypeNumberA)
                        {
                            case 2:
                                {
                                    tableName = "item_Custom_Weapon";
                                    break;
                                }
                            case 3:
                                {
                                    tableName = "item_Custom_Armor";
                                    break;
                                }
                            case 4:
                                {
                                    tableName = "Custom_Trinket";
                                    break;
                                }
                        }

                        if (tableName != null)
                        {
                            // �ɾ͵���
                            int achieveA = 0;
                            int achieveB = 0;
                            if (arrayA[7].Length != 0)
                                achieveA = int.Parse(arrayA[7]);
                            if (arrayB[7].Length != 0)
                                achieveB = int.Parse(arrayB[7]);

                            // �Ƚ�SubType
                            string subTpyeStringA = GetInfo(tableName, itemIndexA, "SubType");
                            string subTypeStringB = GetInfo(tableName, itemIndexB, "SubType");

                            if (achieveA != achieveB)
                            {
                                result = achieveA > achieveB;
                            }
                            else if (subTpyeStringA == null)
                            {
                                result = false;
                            }
                            else if (subTypeStringB == null)
                            {
                                result = true;
                            }
                            else
                            {
                                int subTypeA = int.Parse(subTpyeStringA);
                                int subTypeB = int.Parse(subTypeStringB);

                                if (subTypeA > subTypeB)
                                {
                                    result = true;
                                }
                                else if (subTypeA < subTypeB)
                                {
                                    result = false;
                                }
                                else
                                {
                                    // �Ƚ�DetailType
                                    string detailTypeStringA = GetInfo(tableName, itemIndexA, "DetailType");
                                    string detailTypeStringB = GetInfo(tableName, itemIndexB, "DetailType");

                                    if (detailTypeStringA == null)
                                    {
                                        result = false;
                                    }
                                    else if (detailTypeStringB == null)
                                    {
                                        result = true;
                                    }
                                    else
                                    {
                                        int detailTypeA = int.Parse(detailTypeStringA);
                                        int detailTypeB = int.Parse(detailTypeStringB);

                                        if (detailTypeA > detailTypeB)
                                        {
                                            result = true;
                                        }
                                        else if (detailTypeA < detailTypeB)
                                        {
                                            result = false;
                                        }
                                        else
                                        {
                                            // �Ƚ�Price
                                            int priceA = int.Parse(arrayA[6]);
                                            int priceB = int.Parse(arrayB[6]);

                                            result = priceA > priceB;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            

            return result;
        }

        /// <summary>
        /// �����ļ�
        /// </summary>
        /// <param name="fileName">�����ļ�������</param>
        /// <returns>�Ƿ񵼳��ɹ�</returns>
        public bool ExportFiles(string fileName)
        {
            bool exportSuccess = false;
            string resultFolder = "";
            FolderBrowserDialog openFolderDialog1 = new FolderBrowserDialog();
            openFolderDialog1.Description = "��ѡ�񵼳��ļ��ı���Ŀ¼";
            openFolderDialog1.ShowNewFolderButton = true;
            if(path == null)
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
                }

                if (fileNameIndex == -1) // û���ҵ������ļ���������
                {
                    MessageBox.Show("û���ҵ������ļ���������!");
                    return false;
                }

                headline = headline.TrimEnd(new char[] { '\r', '\n', '\t' }); // ȥ��������Ϣ

                foreach (string s in values) // ������������е�hash����
                {
                    if(s == "") // ���Կ���
                    {
                        continue;
                    }
                    string[] data = s.Split('\t');
                    if (fileTable[Path.Combine(resultFolder, data[fileNameIndex])] != null)
                    {
                        List<string> lines = fileTable[Path.Combine(resultFolder, data[fileNameIndex])] as List<string>;
                        string t = string.Empty;
                        for (int i = 0; i < data.Length; i++)
                        {
                            if(!excludeIndexList.Contains(i))
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
                        fileTable[Path.Combine(resultFolder, data[fileNameIndex])] = lines;
                    }
                }

                List<string> fileNameList = new List<string>();
                foreach (string s in fileTable.Keys)
                {
                    fileNameList.Add(s);
                }

                // �����ļ�
                foreach (string s in fileTable.Keys)
                {
                    if (fileNameList.Contains(s))
                    {
                        List<string> lines = fileTable[s] as List<string>;

                        string tempValue;

                        // �������ݽ�������
                        bool finished = true;
                        for (int i = 0; i < lines.Count - 1; i++)
                        {
                            finished = true;

                            for (int j = 0; j < lines.Count - i - 1; j++)
                            {
                                if (IsLargeThan(lines[j], lines[j + 1]))
                                {
                                    tempValue = lines[j + 1];
                                    lines[j + 1] = lines[j];
                                    lines[j] = tempValue;
                                    finished = false;
                                }
                            }

                            if(finished) // �Ѿ�����Ҫ������
                            {
                                break;
                            }
                        }
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

                exportSuccess = true;
            }

            return exportSuccess;
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
                    MessageBox.Show(string.Format("�ļ�{0}������,���½����ļ�!", fileName));
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
