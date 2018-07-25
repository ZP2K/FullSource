using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using System.Data.SqlClient;

using DevComponents.DotNetBar;

namespace UIQuestTableConvert
{
    public partial class MainForm : Office2007Form
    {
        private string connectString = "Server = jx3web; DataBase = s3design_rtm; Uid = s3design; Password = davidbowie;";
        private SqlConnection conn;

        public MainForm()
        {
            InitializeComponent();
            Init();
        }

        private void Init()
        {
            conn = new SqlConnection(connectString);
            folderNameBox.Text = Application.StartupPath;
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
        /// �������ݱ�
        /// </summary>
        /// <param name="content">����</param>
        /// <returns>���ݱ�</returns>
        private DataTable CreateDataTable(string content)
        {
            DataTable table = new DataTable();
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

            return table;
        }

        /// <summary>
        /// ѡ���ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bBrowse_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Title = "��ѡ���������ݱ�����λ��";
            openFileDialog.Filter = "�������ݱ�|*.tab";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                fileNameBox.Text = openFileDialog.FileName;
            }
        }

        /// <summary>
        /// ��ʼ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bConvert_Click(object sender, EventArgs e)
        {
            string fileName = fileNameBox.Text;
            string folderName = folderNameBox.Text;

            if (!string.IsNullOrEmpty(fileName) && !string.IsNullOrEmpty(folderName))
            {
                string content = GetFileContent(fileName);

                if (content != null)
                {
                    DataTable questTable = CreateDataTable(content);

                    ExportManager exportManager = new ExportManager(conn, folderName, questTable);
                    exportManager.Export();
                }
            }
            else
            {
                MessageBox.Show("����ȷ��������ݱ�Ĳ�����", "����������ݱ�",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �˳�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// ������Ŀ¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bBrowseFolder_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog();
            folderBrowserDialog.SelectedPath = Application.StartupPath;
            folderBrowserDialog.Description = "��ѡ���ļ����Ŀ¼";

            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                folderNameBox.Text = folderBrowserDialog.SelectedPath;
            }
        }
    }
}