using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using DBManager;

namespace FlowChartScriptConvert
{
    public partial class MainForm : Form
    {
        private DataBaseManager dataBaseManager;
        private enum ConvertState { Normal, BlockRemark };
        private ConvertState convertState = ConvertState.Normal;
        private string scriptFilePath; // �ű�·��
        private string mapName; // ��ͼ����

        public MainForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// ��ȡ�ļ��ı�����
        /// </summary>
        /// <param name="filePath">�ļ�·��</param>
        /// <returns>�ı�����</returns>
        private string GetFileContent(string filePath)
        {
            string content = null;

            if (File.Exists(filePath))
            {
                StreamReader reader = new StreamReader(filePath, Encoding.GetEncoding("gb2312"));
                content = reader.ReadToEnd();
                reader.Close();
            }
            else
            {
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", filePath), "��ȡ�����ļ�",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return content;
        }

        /// <summary>
        /// ������д���ļ�
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <param name="content">�ı�����</param>
        private void SaveDataToFile(string fileName, string content)
        {
            try
            {
                string filePath = Path.GetDirectoryName(fileName);
                FileInfo fi = new FileInfo(fileName);
                StreamWriter sw;

                if (!fi.Exists) // �ļ�������
                {
                    if (!Directory.Exists(filePath)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(filePath); // �ȴ���Ŀ¼���ٴ����ļ�
                    }

                    FileStream fs = fi.Create();
                    fs.Close();
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }
                else // �ļ��Ѿ�����
                {
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }

                sw.Write(content);
                sw.Close();
            }
            catch (IOException ex)
            {
                MessageBox.Show(string.Format("�����ļ�{0}ʱ����IO�쳣��" + ex.Message, fileName), "���������ļ�",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ת�����ı�
        /// </summary>
        /// <param name="line">���ı�</param>
        /// <returns>���ı�</returns>
        private string ConvertLine(string line)
        {
            string newLine = line;

            CodeConvert codeConvert = CodeConvert.GetCodeConvert();
            codeConvert.ConvertTableName = "stringTable";
            List<string> wordList = codeConvert.GetStringList(line);

            if (wordList.Count > 0) // ��Ҫ�ַ����滻
            {
                DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();

                foreach (string s in wordList)
                {
                    if (!s.Contains(",") && !line.Contains(string.Format("context, {0}", s))) // �������Ͳ����滻
                    {                        
                        string id = dataBaseManager.InsertIntoFlowChartLS(s.Replace("'", "''"), mapName);
                        newLine = newLine.Replace(s, string.Format("{0}[{1}]", codeConvert.ConvertTableName, id));
                    }                    
                }
            }

            return newLine;
        }

        /// <summary>
        /// ����ls�ļ�
        /// </summary>
        /// <param name="filePath">�ļ�·��</param>
        private void SaveLSFile(string filePath)
        {
            string sqlString = string.Format("SELECT * FROM sys_script_lsfile WHERE classification = '{0}'", mapName);
            DataTable lsTable = dataBaseManager.GetDataTable(sqlString, dataBaseManager.Connection_Jx3web);
            string lsContent = CodeConvert.ConvertLsFileContent(lsTable.Select(string.Format("classification = '{0}'", mapName)));
            SaveDataToFile(filePath, lsContent);                        
        }

        /// <summary>
        /// ��ʼת��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            scriptFilePath = textBoxX1.Text;
            mapName = textBoxX2.Text;

            if (scriptFilePath != "" && mapName != "")
            {
                string content = GetFileContent(scriptFilePath);
                
                if (content != null)
                {
                    dataBaseManager = DataBaseManager.GetDataBaseManager();

                    StringBuilder newContent = new StringBuilder();
                    string[] lines = content.Split(new string[] { "\r\n" }, StringSplitOptions.None);

                    // ���Ӽ����ַ���table�ĵ���
                    string includeLsFile = string.Format("Include('scripts/Map/{0}/include/scenestrings.ls')", mapName);

                    if (lines[0] != includeLsFile)
                    {
                        newContent.AppendLine(includeLsFile);
                    }

                    int insertIndex = -1;

                    for (int i = 0; i < lines.Length; i++)
                    {
                        string s = lines[i];
                        string line = s.Trim(new char[] { '\t', ' ' });
                        string newLine = null;

                        if (lines[i] == "using('Event')")
                        {
                            if (lines[i + 2] != "local stringTable = SCENE_STRING_TABLE")
                            {
                                insertIndex = i + 2;
                            }
                        }

                        // ������ַ����������
                        if (i == insertIndex)
                        {
                            newContent.AppendLine("local stringTable = SCENE_STRING_TABLE");
                            newContent.AppendLine();
                        }

                        switch (convertState)
                        {
                            case ConvertState.Normal:
                                {
                                    if (line.StartsWith("--[["))
                                    {
                                        convertState = ConvertState.BlockRemark;
                                        newContent.AppendLine(s);
                                    }
                                    else if (line.StartsWith("--"))
                                    {
                                        newContent.AppendLine(s);
                                    }
                                    else
                                    {                                        
                                        newLine = ConvertLine(s);
                                        newContent.AppendLine(newLine);
                                    }

                                    break;
                                }
                            case ConvertState.BlockRemark:
                                {
                                    if (line.StartsWith("--]]"))
                                    {
                                        convertState = ConvertState.Normal;
                                        newContent.AppendLine(s);
                                    }
                                    else
                                    {
                                        newContent.AppendLine(s);
                                    }

                                    break;
                                }
                        }
                    }

                    SaveDataToFile(scriptFilePath, newContent.ToString());

                    string lsFileFolder = Path.GetDirectoryName(scriptFilePath);
                    lsFileFolder = Path.Combine(lsFileFolder, "include");
                    string lsFilePath = Path.Combine(lsFileFolder, "scenestrings.ls");

                    SaveLSFile(lsFilePath);

                    MessageBox.Show(string.Format("�ļ���\r\n{0}\r\n{1}\r\n\r\nת���ɹ���", scriptFilePath, lsFilePath), "�ű�ת��", 
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("��������Ϊ�գ�", "�ű�ת��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �˳�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// ����ű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Title = "��ѡ��Ҫת���Ľű��ļ�";
            openFileDialog.Filter = "�ű��ļ�|*.lua";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                textBoxX1.Text = openFileDialog.FileName;
                string[] dataArray = openFileDialog.FileName.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);

                if (dataArray.Length > 3)
                {
                    textBoxX2.Text = dataArray[dataArray.Length - 2];                    
                }
            }
        }
    }
}