using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace GameLuaEditor
{
    public partial class SettingForm : Form
    {
        // ���½ű������õĻص�
        public delegate void UpdateScriptFont(Font font);
        private UpdateScriptFont updateScriptFont;

        // ���½ű�������ɫ�õĻص�
        public delegate void UpdateScriptForeColor(Color color);
        private UpdateScriptForeColor updateScriptForeColor;

        /// <summary>
        /// ���캯��
        /// </summary>
        public SettingForm()
        {
            InitializeComponent();            
            Init();
        }

        /// <summary>
        /// ���½ű������õĻص�
        /// </summary>
        public UpdateScriptFont CurrentUpdateScriptFont
        {
            set
            {
                updateScriptFont = value;
            }
        }

        /// <summary>
        /// ���½ű�������ɫ�õĻص�
        /// </summary>
        public UpdateScriptForeColor CurrentUpdateScriptForeColor
        {
            set
            {
                updateScriptForeColor = value;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            txtPath.Text = Helper.ClientPath;
            serverPathBox.Text = Helper.ServerPath;
            messageDelayBox.Text = Helper.ReceiveMessageDelay.ToString();
            messageRetryTimeBox.Text = Helper.ReceiveMessageRetryTime.ToString();
            breakPointWaitCircleBox.Text = Helper.BreakPointWaitCircle.ToString();
            serverIPBox.Text = Helper.ServerIP;
            fontPreviewLabel.Font = Helper.ScriptFont;
            fontPreviewLabel.ForeColor = Helper.ScriptForeColor;
        }

        /// <summary>
        /// ѡ���ļ���
        /// </summary>
        /// <param name="description">��������</param>
        /// <returns>�ļ���·��</returns>
        private string ViewFolder(string description)
        {
            string folderPath = "";

            FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog();
            folderBrowserDialog.Description = description;

            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                folderPath = folderBrowserDialog.SelectedPath;
            }

            return folderPath;
        }        

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void btnOK_Click(object sender, EventArgs e)
        {
            if (txtPath.Text == "")
            {
                MessageBox.Show("�ͻ���·������Ϊ�գ�", "��������", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            Helper.ClientPath = txtPath.Text;            
            
            // ����ű��༭���������ļ�
            string fileName = Path.Combine(Application.StartupPath, "LuaEditor.ini");
            if (!File.Exists(fileName))
            {
                File.Create(fileName).Close();
            }
            
            if (serverPathBox.Text != "")
            {
                Helper.ServerPath = serverPathBox.Text;
                IniIO.IniWriteValue("General", "ServerFolder", Helper.ServerPath, fileName);
            }

            if (messageDelayBox.Text != "")
            {
                Helper.ReceiveMessageDelay = int.Parse(messageDelayBox.Text);
                IniIO.IniWriteValue("General", "ReceiveMessageDelay", Helper.ReceiveMessageDelay.ToString(), fileName);
            }

            if (messageRetryTimeBox.Text != "")
            {
                Helper.ReceiveMessageRetryTime = int.Parse(messageRetryTimeBox.Text);
                IniIO.IniWriteValue("General", "ReceiveMessageRetryTime", Helper.ReceiveMessageRetryTime.ToString(), fileName);
            }

            if (breakPointWaitCircleBox.Text != "")
            {
                Helper.BreakPointWaitCircle = int.Parse(breakPointWaitCircleBox.Text);
                IniIO.IniWriteValue("General", "BreakPointWaitCircle", Helper.BreakPointWaitCircle.ToString(), fileName);
            }

            if (serverIPBox.Text != "")
            {
                Helper.ServerIP = serverIPBox.Text;
                IniIO.IniWriteValue("General", "ServerIP", Helper.ServerIP, fileName);
            }

            Helper.ScriptFont = fontPreviewLabel.Font;
            Helper.ScriptForeColor = fontPreviewLabel.ForeColor;
            string fontName = fontPreviewLabel.Font.Name;            
            string fontSize = fontPreviewLabel.Font.Size.ToString();
            string fontColor = fontPreviewLabel.ForeColor.Name;

            IniIO.IniWriteValue("General", "ScriptFontName", fontName, fileName);
            IniIO.IniWriteValue("General", "ScriptFontSize", fontSize, fileName);
            IniIO.IniWriteValue("General", "ScriptForeColor", fontColor, fileName);
            IniIO.IniWriteValue("General", "RootDir", Helper.ClientPath, "./GameDesingerTools.ini");

            // ��������
            updateScriptFont(Helper.ScriptFont);

            // ����������ɫ
            updateScriptForeColor(Helper.ScriptForeColor);

            this.DialogResult = DialogResult.OK;
            this.Visible = false;
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void btnEditor_Click(object sender, EventArgs e)
        {
            string strFolder    = Application.StartupPath.TrimEnd(new char[] { '\\' });
            string strCfg       = Helper.FileToString(strFolder + @"\Plugins\LuaCheck\setting.cfg");
            string strUserCfg   = Helper.FileToString(strFolder + @"\Plugins\LuaCheck\_setting.cfg");

            //strCfg = "����,int,123,|�Ƿ�������,list,false;true,|";
            //strUserCfg = "456,true";

            string[] astrUserCfg = strUserCfg.Split(new string[] { "," },  StringSplitOptions.RemoveEmptyEntries);
            string[] astrCfg = strCfg.Split(new string[] { "|" },  StringSplitOptions.RemoveEmptyEntries);

            //��ȡ�û�����,����дstrCfg
            if(astrUserCfg.Length == astrCfg.Length)
            {
                strCfg = "";
                for(int i = 0; i < astrCfg.Length; i++)
                {
                    string strItem = astrCfg[i];
                    string[] asItem = strItem.Split(new char[] { ',' });
                    if (asItem.Length != 4) return;
                    if(asItem[1] == "list")
                    {
                        string strList = asItem[2].Trim(new char[] { ';' });
                        strList = ";" + strList + ";";
                        strList = astrUserCfg[i] + strList.Replace(";" + astrUserCfg[i] + ";", ";");
                        asItem[2] = strList;
                    }
                    else
                    {
                        asItem[2] = astrUserCfg[i];
                    }
                    astrCfg[i] = string.Join(",", asItem) + ",";
                }
                strCfg = string.Join("|", astrCfg) + "|";
            }

            string[] asp1 = { "|" };
            ModelForm mf = new ModelForm(strCfg.Split(asp1, StringSplitOptions.RemoveEmptyEntries));
            if (mf.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            List<string> list = mf.InputList;
            Helper.GameLuaEditorSetting = string.Join(",", list.ToArray());
            Helper.WriteStringToFile(Helper.GameLuaEditorSetting, strFolder + @"\Plugins\LuaCheck\_setting.cfg");
        }

        private void btnCancel_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.Alt && e.Control && e.KeyCode == Keys.I)
            {
                ImportForm frmImp = new ImportForm();
                frmImp.ShowDialog();
            }            
        }

        /// <summary>
        /// ����ͻ���Ŀ¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string folderPath = ViewFolder("��ѡ��ͻ����ļ�������Ŀ¼");

            if (folderPath != "")
            {
                txtPath.Text = folderPath;
            }
        }

        /// <summary>
        /// ��������Ŀ¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            string folderPath = ViewFolder("��ѡ�������ļ�������Ŀ¼");

            if (folderPath != "")
            {
                serverPathBox.Text = folderPath;
            }
        }

        /// <summary>
        /// �ҽӵ����ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAttachDebugFile_Click(object sender, EventArgs e)
        {
            string serverFolder = serverPathBox.Text;
            string clientFolder = txtPath.Text;

            if (clientFolder == "")
            {
                MessageBox.Show("����ѡ��ͻ�������Ŀ¼��", "�ҽӵ����ļ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (serverFolder == "")
            {
                MessageBox.Show("����ѡ����������Ŀ¼��", "�ҽӵ����ļ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            List<string> srcFileNameList = new List<string>();
            List<string> destFileNameList = new List<string>();

            srcFileNameList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\Engine_Lua5.dll"));
            destFileNameList.Add(Path.Combine(serverFolder, "Engine_Lua5.dll"));

            srcFileNameList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\Engine_Lua5D.dll"));
            destFileNameList.Add(Path.Combine(serverFolder, "Engine_Lua5D.dll"));

            srcFileNameList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\Lua5D.dll"));
            destFileNameList.Add(Path.Combine(serverFolder, "Lua5D.dll"));

            srcFileNameList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\udpsocket.dll"));
            destFileNameList.Add(Path.Combine(serverFolder, "udpsocket.dll"));

            srcFileNameList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\ldb.lua"));
            destFileNameList.Add(Path.Combine(clientFolder, @"scripts\ldb.lua"));

            srcFileNameList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\udpsocket.dll"));
            destFileNameList.Add(Path.Combine(serverFolder, "udpsocket.dll"));

            srcFileNameList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\udpsocket.dll"));
            destFileNameList.Add(Path.Combine(clientFolder, "udpsocket.dll"));

            for (int i = 0; i < srcFileNameList.Count; i++)
            {
                if (File.Exists(srcFileNameList[i]))
                {                    
                    File.Copy(srcFileNameList[i], destFileNameList[i], true);                    
                }
                else
                {
                    MessageBox.Show(string.Format("�ļ�{0}�����ڣ���������UpdateProgram.exe��ȡ�����ļ���", srcFileNameList[i]), "�ҽӵ����ļ�",
                                                  MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }

            MessageBox.Show("�����ļ��ҽ���ɣ�", "�ҽӵ����ļ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        /// <summary>
        /// ѡ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bSelectFont_Click(object sender, EventArgs e)
        {
            FontDialog fontDialog = new FontDialog();
            fontDialog.Font = fontPreviewLabel.Font;

            if (fontDialog.ShowDialog() == DialogResult.OK)
            {
                fontPreviewLabel.Font = fontDialog.Font;
            }
        }

        /// <summary>
        /// ѡ����ɫ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            ColorDialog colorDialog = new ColorDialog();
            colorDialog.Color = fontPreviewLabel.ForeColor;

            if (colorDialog.ShowDialog() == DialogResult.OK)
            {
                fontPreviewLabel.ForeColor = colorDialog.Color;
            }
        }
    }
}