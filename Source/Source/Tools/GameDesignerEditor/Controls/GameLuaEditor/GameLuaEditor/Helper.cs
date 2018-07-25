using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using System.Collections;
using System.Data.SqlClient;
using System.Data;
using System.Drawing;
using System.Text.RegularExpressions;

namespace GameLuaEditor
{
    public static class Helper
    {
        public static string ConnectionString = "";
        public static string ClientPath = "";
        public static string GameLuaEditorSetting = "";
        public static string ServerPath = ""; // ���������Ŀ¼
        public static string ServerIP = "127.0.0.1"; // ����˵�IP��ַ
        public static int ReceiveMessageDelay = 64; // ������Ϣ�ӳ�
        public static int ReceiveMessageRetryTime = 3; // ������Ϣ��ʱ���Դ���
        public static int BreakPointWaitCircle = 256; // ���Զϵ㴥�������
        public static Hashtable apiTable = new Hashtable(); // �ű�API���ݱ�
        public static Font ScriptFont = new Font("Sim Sun", 8, FontStyle.Regular); // �ű�������
        public static Color ScriptForeColor = Color.Black; // �ű�����ɫ

    #region InputBox
        static Form f;
        public static string InputBox(string strDefault, string strTitle)
        {
            f = new Form();
            RichTextBox text1 = new RichTextBox();
            Button buttonOK = new Button();
            Button buttonCancel = new Button();
            

            text1.Text = strDefault;
            if(strDefault.IndexOf(".") != -1)
            {
                text1.SelectionStart = 0;
                text1.SelectionLength = strDefault.LastIndexOf(".");
            }
            text1.Left = 10;
            text1.Top = 10;
            text1.Size = new System.Drawing.Size(410, 22);

            buttonOK.Text = "ȷ��";
            buttonOK.Top = 40;
            buttonOK.Left = 250;

            buttonCancel.Text = "ȡ��";
            buttonCancel.Top = 40;
            buttonCancel.Left = 340;

            f.Controls.Add(text1);
            f.Controls.Add(buttonOK);
            f.Controls.Add(buttonCancel);
            f.AcceptButton = buttonOK;
            f.CancelButton = buttonCancel;
            f.MaximizeBox = false;
            f.MinimizeBox = false;
            f.ShowIcon = false;
            f.ShowInTaskbar = false;
            f.StartPosition = FormStartPosition.CenterScreen;
            buttonOK.Click += new EventHandler(buttonOK_Click);
            buttonCancel.Click += new EventHandler(buttonCancel_Click);
            f.Text = strTitle;
            f.Size = new System.Drawing.Size(430, 100);
            DialogResult dr = f.ShowDialog();
            if(dr == DialogResult.OK )
            {
                return text1.Text;
            }
            else
            {
                return null;
            }
        }

        static void buttonCancel_Click(object sender, EventArgs e)
        {
            f.DialogResult = DialogResult.Cancel;
            f.Visible = false;
        }

        static void buttonOK_Click(object sender, EventArgs e)
        {
            f.DialogResult = DialogResult.OK;
            f.Visible = false;
        }
    #endregion

        /// <summary>
        /// ��ȡ�ű����ַ���ID����
        /// </summary>
        /// <param name="content">�ű�����</param>
        /// <param name="tableName">�ַ���������</param>
        /// <returns>�ַ���ID����</returns>
        public static List<string> GetScriptID(string content, string tableName)
        {
            List<string> idList = new List<string>();

            if (content != null)
            {
                string regexPattern = string.Format(@"({0}\[(?<MatchResult>\d*)\])", tableName);
                Regex r = new Regex(regexPattern);
                MatchCollection matches = r.Matches(content);

                foreach (Match match in matches)
                {
                    idList.Add(match.Result("${MatchResult}"));
                }
            }

            return idList;
        }

        /// <summary>
        /// �����ı����ļ�
        /// </summary>
        /// <param name="content">�ı�����</param>
        /// <param name="fileName">�ļ���</param>
        private static void StringToFile(string content, string fileName)
        {
            StringToFile(content, fileName, Encoding.GetEncoding("gb2312")); // �������ؼ�����Encoding
        }

        /// <summary>
        /// �����ı����ļ�
        /// </summary>
        /// <param name="content">�ı�����</param>
        /// <param name="fileName">�ļ���</param>
        public static void WriteStringToFile(string content, string fileName)
        {
            StringToFile(content, fileName); // �������ؼ�����Encoding
        }

        /// <summary>
        /// �����ı��ļ�
        /// </summary>
        /// <param name="content">�ı�����</param>
        /// <param name="fileName">�ļ���</param>
        /// <param name="encoding">����</param>
        private static void StringToFile(string content, string fileName, Encoding encoding)
        {
            if (!File.Exists(fileName))
            {
                if (fileName.IndexOf("\\") != -1)
                {
                    Directory.CreateDirectory(fileName.Substring(0, fileName.LastIndexOf("\\")));
                    File.Create(fileName).Close();
                }                
            }

            StreamWriter writer = new StreamWriter(fileName, false, encoding);
            writer.Write(content);
            writer.Close();
        }
        
        /// <summary>
        /// ��ȡ�ļ�����
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <returns>�ļ�����</returns>
        public static string FileToString(string fileName)
        {
            string content = "";

            if (File.Exists(fileName))
            {
                StreamReader reader = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                content = reader.ReadToEnd();
                reader.Close();
            }            

            return content;
        }
        
        /// <summary>
        /// ִ���ⲿ����
        /// </summary>
        /// <param name="cmd">��������</param>
        /// <param name="arg">�������</param>
        /// <param name="sync">������Ϣ</param>
        public static void Execute(string cmd, string arg, string sync)
        {
            Process p = new Process();
            p.StartInfo.FileName = cmd;
            p.StartInfo.Arguments = arg;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardInput = true;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.CreateNoWindow = true;
            p.Start();

            if ("false" == sync)
            {
                p.WaitForExit();
                p.Close();
            }
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <param name="connection">sql����</param>
        /// <returns>���ݱ�</returns>
        public static DataTable GetDataTable(string sqlString, SqlConnection conn)
        {
            SqlDataAdapter adapter = new SqlDataAdapter(sqlString, conn);
            adapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet dataSet = new DataSet();
            adapter.Fill(dataSet);
            DataTable table = dataSet.Tables[0];
            return table;
        }

        /// <summary>
        /// �������Ƿ����
        /// </summary>
        /// <param name="processName">������</param>
        /// <param name="checkMultiProcess">�Ƿ�����ͬ������</param>
        /// <returns>�����Ƿ����</returns>
        public static bool CheckProcessExist(string processName, bool checkMultiProcess)
        {
            bool exist = false;
            int count = 0;

            foreach (System.Diagnostics.Process p in System.Diagnostics.Process.GetProcesses())
            {
                if (p.ProcessName == processName)
                {                                        
                    if (checkMultiProcess)
                    {
                        count++;
                    }
                    else
                    {
                        exist = true;
                        break;
                    }                    
                }
            }

            if (checkMultiProcess)
            {
                if (count > 1)
                {
                    exist = true;
                }
            }

            return exist;
        }
    }
}
