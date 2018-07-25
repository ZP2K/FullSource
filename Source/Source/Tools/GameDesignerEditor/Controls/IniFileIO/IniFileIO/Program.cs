using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Collections;
using System.Reflection;

namespace IniFileIO
{
    public class Program
    {
        private List<string> fields = new List<string>(); // ��������
        private List<string> values = new List<string>(); // ����������
        private List<string> files = new List<string>(); // �ļ�������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="fields">��������</param>
        /// <param name="values">����������</param>
        public Program(ArrayList fields, string[] values)
        {
            int idIndex = 0;
            // ��ʼ����������
            for (int i = 0; i < fields.Count; i++)
            {
                if(fields[i] == null || fields[i].ToString() == "")
                {
                    continue;
                }
                if(fields[i].ToString() != "id\r\n")
                {
                    this.fields.Add(fields[i].ToString());
                }
                else
                {
                    idIndex = i; 
                }
            }
            // ��ʼ������������
            foreach(string s in values)
            {
                if(s == "")
                {
                    continue;
                }
                string[] data = s.Split('\t');
                string value = "";
                for(int i = 0; i < data.Length; i++)
                {
                    if(i != idIndex)
                    {
                        value = value + data[i] + '\t';
                    }
                }
                value = value.TrimEnd('\t');
                this.values.Add(value);
            }
        }

        /// <summary>
        /// �ع�Ҫ������ļ�
        /// </summary>
        /// <param name="path">�ļ�·��</param>
        public void RestructFiles(string path)
        {
            string resultFolder = "";
            FolderBrowserDialog openFolderDialog1 = new FolderBrowserDialog();
            openFolderDialog1.Description = "��ѡ�񵼳��ļ��ı���Ŀ¼";
            openFolderDialog1.ShowNewFolderButton = true;
            openFolderDialog1.RootFolder = Environment.SpecialFolder.MyComputer;
            if (openFolderDialog1.ShowDialog() == DialogResult.OK)
            {
                resultFolder = openFolderDialog1.SelectedPath + "\\";

                // ������Ч���Եĸ���
                int attributeNumber = 0; // ��Ч���Ը���
                string[] datas = values[0].Split('\t');
                foreach (string s in datas)
                {
                    if (s == "-1")
                    {
                        attributeNumber++;
                    }
                }
                attributeNumber--;

                List<string> fileNameList = new List<string>(); // �ļ�������
                foreach(string s in values)
                {
                    string[] tempData = s.Split('\t');
                    string tempFileName = tempData[0];
                    if(!fileNameList.Contains(tempFileName))
                    {
                        fileNameList.Add(tempFileName);
                    }
                }
                ChooseFileForm cForm = new ChooseFileForm("��ѡ��Ҫ������ļ�", fileNameList);
                DialogResult result = cForm.ShowDialog();
                if(result != DialogResult.OK)
                {
                    return;
                }

                foreach (string s in values)
                {
                    if (s.Contains("-1\t")) // ��ȡĬ��������
                    {
                        string[] data = s.Split('\t');
                        List<string> list = new List<string>();
                        list.Add("[Main]");
                        if(!fileNameList.Contains(data[0]))
                        {
                            continue;
                        }
                        string fileName = resultFolder + data[0];
                        files.Add(fileName);
                        for (int i = 2; i < fields.Count - attributeNumber; i++)
                        {
                            list.Add(fields[i].ToString() + "=" + data[i]);
                        }
                        list.Add(""); // �������
                        SaveSpecialData(fileName, list); // ����Ĭ��������
                    }
                    else // ��ȡ��ͨ������
                    {
                        string[] data = s.Split('\t');
                        List<string> list = new List<string>();
                        list.Add("[" + data[1] + "]");
                        if (!fileNameList.Contains(data[0]))
                        {
                            continue;
                        }
                        string fileName = resultFolder + data[0];
                        for (int i = fields.Count - attributeNumber; i < fields.Count; i++)
                        {
                            list.Add(fields[i].ToString() + "=" + data[i]);
                        }
                        list.Add(""); // �������
                        SaveNormalData(fileName, list); // ������ͨ������
                    }
                }
                string message = "�����ļ�:\n\n";
                foreach (string s in files)
                {
                    message = message + string.Format("{0}\n", s);
                }
                message = message + "\n\n";
                message = message + "�Ѿ��ɹ�����!";
                MessageBox.Show(message);
            }
        }

        /// <summary>
        /// ����Ĭ��������
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="values">��������</param>
        private void SaveSpecialData(string fileName, List<string> values)
        {
            try
            {
                FileInfo fi = new FileInfo(fileName);
                if(!fi.Exists) // �ļ��Ѿ�����
                {
                    MessageBox.Show(string.Format("�ļ�{0}������,���½����ļ�!", fileName));
                    FileStream fs = fi.Create();
                    fs.Close();
                }
                StreamWriter sw = fi.CreateText();
                foreach(string s in values)
                {
                    sw.WriteLine(s);
                }
                sw.Close();
            }
            catch(Exception ex)
            {
                MessageBox.Show(string.Format("���ļ�{0}����IO����ʱ�����쳣: " + ex.ToString(), fileName));
            }
        }

        /// <summary>
        /// ������ͨ������
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="values">��������</param>
        private void SaveNormalData(string fileName, List<string> values)
        {
            try
            {
                FileInfo fi = new FileInfo(fileName);
                if (!fi.Exists) // �ļ��Ѿ�����
                {
                    MessageBox.Show(string.Format("�ļ�{0}������!", fileName));
                    return;
                }
                StreamWriter sw = fi.AppendText();
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
