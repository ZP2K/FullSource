using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using luaEditor;

namespace AIDiagramPlugin
{
    public class AIDiagramHelper
    {               
        /// <summary>
        /// ��ѯ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        public static void QueryObjects(object sender, luaEditor.QueryEventsArgs e)
        {            
            // �����涨��
            // szCode: ���еĴ���
            // nPos: ���λ�ã����ְ�2���ַ��㣩
            // nIsDelete: 0Ϊ�������룬1Ϊɾ����
            // 11-22ΪF1-F12,
            // 1111����Ctrl+Shift+F1,
            // 1011����Ctrl+F1
            // 111����Shift+F1
            int nIsDelete = e.nPos > 0 ? 0 : 1;

            if (e.keyword != "")
            {
                nIsDelete = 10 + int.Parse(e.keyword);
            }

            IntPtr ls_out = IntPtr.Zero;
            LuaRuntime.Init();
            bool bret = LuaRuntime.LuaRun("OnCodeSense", e.szCode, Math.Abs(e.nPos), nIsDelete, "true,1024,��ʾ����,1,false,�﷨���", ref ls_out);
            string strOut = Marshal.PtrToStringAnsi(ls_out);

            if (string.IsNullOrEmpty(strOut))
            {
                return;
            }

            string[] as_data = strOut.Split(new char[] { '|' });

            if (as_data.Length < 3)
            {
                MessageBox.Show("����ֵ����ȷ������ֵΪ��" + strOut, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // ��ȡoverlen
            e.nOverLen = int.Parse((as_data[0]));

            // ��ʼ���α�
            int nPosition = 3;

            // ��ȡlist
            int nListCount = int.Parse((as_data[1]));

            if (as_data.Length < 3 + nListCount)
            {
                MessageBox.Show("List����ֵ��������ȷ������ֵΪ��" + strOut, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            for (int i = 0; i < nListCount; i++)
            {
                string s_name = as_data[nPosition++];
                string s_word = as_data[nPosition++];
                string s_type = as_data[nPosition++];
                string s_info = as_data[nPosition++];
                e.leTable.Add(s_name, new luaEditor.LuaEditorTableItem(s_name, s_type, s_word, s_info));                
            }

            // ��ȡinfo
            int nInfoCount = Int32.Parse((as_data[2]));

            if (as_data.Length < 3 + nListCount + nInfoCount)
            {
                MessageBox.Show("Info����ֵ��������ȷ������ֵΪ��" + strOut, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            for (int i = 0; i < nInfoCount; i++)
            {
                string s_text = as_data[nPosition++] as string;
                e.parms_list.Add(s_text.Replace("\n", "<BR>"));
            }
        }

        /// <summary>
        /// ���ű�
        /// </summary>
        /// <param name="codeEditBox">�ű��༭�ؼ�</param>
        /// <param name="content">�ű�����</param>
        public static bool CheckScript(luaEditorControl codeEditBox, string content)
        {
            string folderName = Path.Combine(Application.StartupPath, @"Plugins\LuaCheck");
            string scriptFilePath = Path.Combine(folderName, "tmp.lua");
            bool passCheck = true;

            AITableExport.WriteStringToFile(content, scriptFilePath);
            // string checkFilePath = Path.Combine(folderName, "ExecLua.exe");
            string checkFilePath = Path.Combine(folderName, "luac5.1.exe");

            //ִ�м�鹤��
            if (File.Exists(checkFilePath))
            {
                Process p = new Process();
                p.StartInfo.FileName = checkFilePath;
                p.StartInfo.Arguments = string.Format("\"{0}\"", scriptFilePath);
                p.StartInfo.WorkingDirectory = folderName;
                p.StartInfo.RedirectStandardOutput = true;
                p.StartInfo.RedirectStandardError = true;
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.CreateNoWindow = true;
                p.Start();

                StreamReader stream = p.StandardOutput;
                StreamReader streamErr = p.StandardError;

                StringBuilder checkString = new StringBuilder();

                while (!p.HasExited)
                {
                    checkString.AppendLine(stream.ReadLine());
                    checkString.AppendLine(streamErr.ReadLine());
                }

                checkString.AppendLine(stream.ReadLine());
                checkString.AppendLine(streamErr.ReadLine());
                string checkResult = checkString.ToString().Trim(new char[] { '\r', '\n' });
                checkResult = checkResult.Replace(string.Format("{0}:", checkFilePath), "");

                p.WaitForExit();
                File.Delete(scriptFilePath);

                if (!string.IsNullOrEmpty(checkResult)) // �ű�ͨ�����
                {
                    StringBuilder checkInfo = new StringBuilder();
                    string[] dataArray = checkResult.Split(new char[] { ':' }, StringSplitOptions.RemoveEmptyEntries);

                    if (dataArray.Length > 1)
                    {
                        for (int i = 1; i < dataArray.Length; i++)
                        {
                            checkInfo.Append(string.Format("{0}:", dataArray[i]));
                        }

                        checkInfo.Remove(checkInfo.Length - 1, 1);
                        MessageBox.Show(checkInfo.ToString(), "lua�ű����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        passCheck = false;

                        //������ת��������
                        if (codeEditBox != null)
                        {
                            int lineIndex = -1;

                            if (int.TryParse(dataArray[1], out lineIndex))
                            {
                                codeEditBox.MoveToLine(lineIndex);
                            }
                        }                        
                    }
                }
            }

            return passCheck;
        }
    }
}
