using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Data.SqlClient;
using DataBaseIO;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Text.RegularExpressions;

using LuaInterface;
using DevComponents.DotNetBar;
using DevComponents.AdvTree;
using Fireball.Syntax;
using Fireball.Windows.Forms.CodeEditor;

namespace GameLuaEditor
{
    public partial class MainForm : Form
    {
        private ExportForm exportForm = null;
        public static MainForm CurrentMainForm = null;

        private Lua m_lua = new Lua();
        public TreeView tree = null;
        private SqlConnection Conn;
        private bool startDebug = false; // �Ƿ��Ѿ���ʼdebug   
        private bool lockVarValue = false; // �Ƿ������޸ı���ֵ
        private bool enableFetchWord = true; // �Ƿ�����ȡ��
        private bool mouseFetchWord = true; // �Ƿ������ȡ��
        private bool enableServerDebugOutput = false; // �Ƿ������������������Ϣ
        private string currentDebugFile = null; // ��ǰ���Ե��ļ�
        private List<ButtonItem> debugButtonList = new List<ButtonItem>(); // debug��صİ�ť
        private NetworkManager networkManager = null; // ���������ʵ��
        private System.Windows.Forms.Timer debugWaitTimer = new System.Windows.Forms.Timer(); // ���Եȴ��õļ�ʱ��        

        private enum DebugState { None, WaitBreakPointActivated, WaitStartDebugActivated, WaitStepDebugActivated }; // ����״̬
        private DebugState debugState = DebugState.None; // ��ǰ�ĵ���״̬ 

        private enum DebugMode { None, Client, Server }; // ���Ե�ģʽ
        private DebugMode debugMode = DebugMode.None;

        private StringReplaceForm stringReplaceForm; // �ַ����滻����
        private List<string> localFileList; // �����ļ�����

        private TreeNode localFileRootNode; // ���ؽű����ڵ�

        /// <summary>
        /// ���캯��
        /// </summary>
        public MainForm()
        {
            InitializeComponent();            
            Init();                        
        }        

        /// <summary>
        /// ��ȡ������
        /// </summary>
        /// <param name="tablename">���ݱ���</param>
        /// <param name="keys">���ݱ�����</param>
        /// <param name="ltReturn">������ݱ�</param>
        /// <returns>�Ƿ��ȡ�ɹ�</returns>
        public bool GetDataTableRow(string tablename, LuaTable keys, ref LuaTable ltReturn)
        {
            string sql = string.Format("SELECT * FROM {0}", tablename);
            DataTable tbl = Helper.GetDataTable(sql, Conn);

            if (tbl.PrimaryKey.Length != keys.Keys.Count)
            {
                MessageBox.Show(string.Format("{0} ���� ����GetDataTableRow���ԣ�{1}\r\n", DateTime.Now, "�������keys���ȴ���"));
            }

            object[] KEYS = new object[tbl.PrimaryKey.Length];

            for (int i = 0; i < KEYS.Length; i++)
            {
                KEYS[i] = ((LuaTable)keys)[i];
            }

            DataRow row = tbl.Rows.Find(KEYS);

            if (row == null)
            {
                return false;
            }

            foreach (DataColumn col in tbl.Columns)
            {
                ltReturn[col.ColumnName] = (row[col.ColumnName] == DBNull.Value ? null : row[col.ColumnName]);
            }

            return true;
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void btnSetup_Click(object sender, EventArgs e)
        {
            SettingForm settingForm = new SettingForm();
            settingForm.CurrentUpdateScriptFont = new SettingForm.UpdateScriptFont(UpdateScriptFont);
            settingForm.CurrentUpdateScriptForeColor = new SettingForm.UpdateScriptForeColor(UpdateScriptForeColor);
            settingForm.ShowDialog();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            tree = treePh;
            this.m_lua["this"] = this;
            tabStrip1.MdiForm = this;            

            // ��ȡ�߻����ߵ�������Ϣ
            Helper.ClientPath = IniIO.IniReadValue("General", "RootDir", "./GameDesingerTools.ini");
            string filename = Application.StartupPath + "/GameDesingerTools_Public.ini";
            string content = FileFolderHelper.FileToString(filename);
            SymmetricMethod sm = new SymmetricMethod();
            content = sm.Decrypto(content);
            IniStructure m_inis = new IniStructure();
            m_inis = IniStructure.ReadIniWithContent(content);
            Helper.ConnectionString = m_inis.GetValue("General", "ConnString");

            string configFileName = Path.Combine(Application.StartupPath, "LuaEditor.ini");

            // ��ȡ����������Ϣ
            if (File.Exists(configFileName))
            {
                string serverPath = IniIO.IniReadValue("General", "ServerFolder", configFileName);

                if (!string.IsNullOrEmpty(serverPath))
                {
                    Helper.ServerPath = serverPath;
                }

                /* ������صĲ�����ʱ�������Զ���
                string receiveMessageDelay = IniIO.IniReadValue("General", "ReceiveMessageDelay", configFileName);
                
                if (!string.IsNullOrEmpty(receiveMessageDelay))
                {
                    Helper.ReceiveMessageDelay = int.Parse(receiveMessageDelay);
                }

                string receiveMessageRetryTime = IniIO.IniReadValue("General", "ReceiveMessageRetryTime", configFileName);

                if (!string.IsNullOrEmpty(receiveMessageRetryTime))
                {
                    Helper.ReceiveMessageRetryTime = int.Parse(receiveMessageRetryTime);
                }

                string breakPointWaitCircle = IniIO.IniReadValue("General", "BreakPointWaitCircle", configFileName);

                if (!string.IsNullOrEmpty(breakPointWaitCircle))
                {
                    Helper.BreakPointWaitCircle = int.Parse(breakPointWaitCircle);
                }
                */ 

                string serverIP = IniIO.IniReadValue("General", "ServerIP", configFileName);

                if (!string.IsNullOrEmpty(serverIP))
                {
                    Helper.ServerIP = serverIP;
                }

                string fontName = IniIO.IniReadValue("General", "ScriptFontName", configFileName);
                string fontSize = IniIO.IniReadValue("General", "ScriptFontSize", configFileName);

                if (!string.IsNullOrEmpty(fontName) && !string.IsNullOrEmpty(fontSize))
                {
                    Helper.ScriptFont = new Font(fontName, float.Parse(fontSize), FontStyle.Regular);
                }

                string fontColor = IniIO.IniReadValue("General", "ScriptForeColor", configFileName);

                if (!string.IsNullOrEmpty(fontColor))
                {
                    Helper.ScriptForeColor = Color.FromName(fontColor);
                }                
            }

            // ��ȡ_setting.cfg
            string strCfgFile = System.IO.Directory.GetCurrentDirectory().TrimEnd(new char[] { '\\' });
            strCfgFile += @"\Plugins\LuaCheck\_setting.cfg";
            Helper.GameLuaEditorSetting = Helper.FileToString(strCfgFile);

            // ���setting����������ȡĬ��setting
            if (Helper.GameLuaEditorSetting == "")
            {
                string strFolder = System.IO.Directory.GetCurrentDirectory().TrimEnd(new char[] { '\\' });
                string strCfg = Helper.FileToString(strFolder + @"\Plugins\LuaCheck\setting.cfg");
                // strCfg = "����,int,123,|�Ƿ�������,list,false;true,|";
                string[] as_item = strCfg.Split(new string[] { ",", "|" }, StringSplitOptions.None);
                string strUserCfg = "";
                for (int i = 0; i < as_item.Length - 1; i += 4)
                {
                    strUserCfg += as_item[i + 2].Split(new char[] { ';' })[0] + ",";
                }
                Helper.GameLuaEditorSetting = strUserCfg.TrimEnd(new char[]{','});
            }

            // ע��debug��صİ�ť
            debugButtonList.Add(bStepInto);
            debugButtonList.Add(bContinueDebug);
            debugButtonList.Add(bRefreshVar);
            debugButtonList.Add(bStepOver);

            // ������
            DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
            dbm.Init(Helper.ConnectionString);
            dbm.ClearRecordLocks();
            Conn = dbm.conn;

            bar4.Items[1].Text = dbm.GetDataBaseName();
            bar4.Items[2].Text = dbm.GetHostName();

            // �������ݿ�ű���
            LoadDataBaseScriptTree();

            // ���ɱ��ؽű���
            LoadLocalScriptTree();

            this.m_lua.RegisterFunction("GetDataTableRow", this, typeof(MainForm).GetMethod("GetDataTableRow"));
            CurrentMainForm = this;

            // ��ʼ�����Լ�ʱ��
            debugWaitTimer.Tick += new EventHandler(OnTimerExpired);
        }

        /// <summary>
        /// ��ӡ����ջ
        /// </summary>
        private void RefreshCallStack()
        {
            callStackView.Rows.Clear();

            string message = "print_call_stack";
            string receivedMessage = SendAndWaitMessage(message, "<ldb>print_call_stack:", true);
            
            if (receivedMessage == null)
            {
                callStackView.Rows.Add(1);
                DataGridViewRow newRow = callStackView.Rows[0];

                newRow.Cells["CallStackLevel"].Value = "1";
                newRow.Cells["CallStackFileName"].Value = "������Ϣ��ʱ";
                newRow.Cells["CallStackLineIndex"].Value = "������Ϣ��ʱ";
                newRow.Cells["CallStackFunctionName"].Value = "������Ϣ��ʱ";
            }
            else
            {
                receivedMessage = receivedMessage.Replace("<ldb>print_call_stack: ", "");

                string[] lines = receivedMessage.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);

                for (int i = 0; i < lines.Length; i++)
                {
                    string line = lines[i];
                    int callLevel = i + 1;
                    string[] info = line.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);

                    callStackView.Rows.Add(1);
                    DataGridViewRow newRow = callStackView.Rows[callStackView.Rows.Count - 1];

                    newRow.Cells["CallStackLevel"].Value = callLevel.ToString();
                    newRow.Cells["CallStackFileName"].Value = info[0];
                    newRow.Cells["CallStackLineIndex"].Value = info[1];
                    newRow.Cells["CallStackFunctionName"].Value = info[2];
                }
            }            
        }

        /// <summary>
        /// ������������ļ��Ƿ��Ѿ�����
        /// </summary>
        /// <returns>���������ļ��Ƿ��Ѿ�����</returns>
        private bool CheckDebugFileReady()
        {
            if (Helper.ClientPath == "")
            {
                MessageBox.Show("�������ÿͻ�������Ŀ¼��", "��ʼ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return false;
            }

            if (Helper.ServerPath == "")
            {
                MessageBox.Show("�������÷��������Ŀ¼��", "��ʼ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return false;
            }

            List<string> srcFileList = new List<string>();

            srcFileList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\Engine_Lua5.dll"));
            srcFileList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\Engine_Lua5D.dll"));
            srcFileList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\Lua5D.dll"));
            srcFileList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\udpsocket.dll"));
            srcFileList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\ldb.lua"));
            srcFileList.Add(Path.Combine(Application.StartupPath, @"Plugins\LuaDebug\udpsocket.dll"));

            List<string> destFileList = new List<string>();

            destFileList.Add(Path.Combine(Helper.ServerPath, "Engine_Lua5.dll"));
            destFileList.Add(Path.Combine(Helper.ServerPath, "Engine_Lua5D.dll"));
            destFileList.Add(Path.Combine(Helper.ServerPath, "Lua5D.dll"));
            destFileList.Add(Path.Combine(Helper.ServerPath, "udpsocket.dll"));
            destFileList.Add(Path.Combine(Helper.ClientPath, @"scripts\ldb.lua"));
            destFileList.Add(Path.Combine(Helper.ClientPath, "udpsocket.dll"));

            for (int i = 0; i < srcFileList.Count; i++)
            {
                string srcFileName = srcFileList[i];
                string destFileName = destFileList[i];

                if (!File.Exists(destFileName))
                {
                    MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", destFileName), "��ʼ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return false;
                }

                DateTime srcFileLastWriteTime = File.GetLastWriteTime(srcFileName);
                DateTime destFileLastWriteTime = File.GetLastWriteTime(destFileName);

                if (srcFileLastWriteTime > destFileLastWriteTime)
                {
                    MessageBox.Show(string.Format("�ļ�{0}�İ汾̫�ɣ�", destFileName), "��ʼ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// ��ʱ���¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void OnTimerExpired(object sender,  EventArgs e)
        {
            switch (debugState)
            {
                case DebugState.None:
                    {
                        debugWaitTimer.Stop();
                        break;
                    }
                case DebugState.WaitBreakPointActivated:
                    {
                        WaitBreakPointActivated(sender, e);
                        break;
                    }
                case DebugState.WaitStartDebugActivated:
                    {
                        WaitStartDebugActivated(sender, e);
                        break;
                    }
                case DebugState.WaitStepDebugActivated:
                    {
                        WaitStepDebugActivated(sender, e);
                        break;
                    }
            }
        }

        /// <summary>
        /// ��ӱ����ļ�
        /// </summary>
        /// <param name="folderName">�ļ�������</param>
        private void AddLocalFile(string folderName)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = folderName;
            openFileDialog.Title = "��ѡ��Ҫ���صĽű��ļ�";
            openFileDialog.Filter = "�ű��ļ�|*.lua";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string fileName = openFileDialog.FileName;

                AddLocalFileNode(fileName);
                OpenFileInEditor(fileName, false);                
            }
        }

        /// <summary>
        /// �ȴ�����������Ϣ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void WaitStepDebugActivated(object sender, EventArgs e)
        {
            // ��������״̬
            string receivedMessage = null;          

            if (networkManager.HasReceivedMessage)
            {
                // ����Ƿ��ǵ�����Ϣ
                receivedMessage = networkManager.GetMessageForHead("<ldb>step_at:");

                if (receivedMessage == null)
                {
                    // ����Ƿ��Ƕϵ���Ϣ
                    receivedMessage = networkManager.GetMessageForHead("<ldb>break_at:");
                }
            }            

            if (receivedMessage != null)
            {
                // ����ͣ��ǰ��ʱ��
                StopDebugWaitTimer();

                PrintOutputText(receivedMessage);
                object[] stepInfo = GetJumpInfoFromMessage(receivedMessage);

                if (stepInfo != null)
                {
                    string fileName = stepInfo[0] as string;
                    int lineIndex = (int)stepInfo[1];
                    EditForm editForm = tabStrip1.SelectedTab.AttachedControl as EditForm;

                    if (ProcessFileName(editForm.FileName) == fileName)
                    {
                        editForm.luaEditBox.DebugRowIndex = lineIndex - 1;
                        editForm.luaEditBox.MoveToLine(lineIndex);
                    }
                    else
                    {
                        editForm.luaEditBox.ClearDebugMark();

                        // �ڱ༭�����д���ת���Ľű��ļ�
                        string filePath = Path.Combine(Helper.ClientPath, fileName);
                        EditForm stepForm = OpenFileInEditor(filePath, true);

                        // ������ǰ�к�
                        if (stepForm != null)
                        {
                            stepForm.luaEditBox.DebugRowIndex = lineIndex - 1;
                            stepForm.luaEditBox.MoveToLine(lineIndex);
                            stepForm.luaEditBox.Refresh();
                        }
                    }

                    currentDebugFile = fileName;
                    editForm.luaEditBox.Refresh();
                }

                // ˢ��һ�α���ֵ
                RefreshVarValue();

                // ˢ�µ���ջ����Ϣ
                RefreshCallStack();

                // ������յ���������Ϣ
                networkManager.ClearReceivedMessageList();

                // ����ȡ�ʹ���
                enableFetchWord = true;

                // �����ʱ����
                debugWaitTimer.Tag = 0;
            }
            else
            {                
                int waitCount = 0;

                if (debugWaitTimer.Tag != null)
                {
                    waitCount = (int)debugWaitTimer.Tag;
                }

                waitCount++;

                if (waitCount > 10)
                {
                    waitCount = 0;
                    StopDebugWaitTimer();                    

                    MessageBox.Show("�ȴ�������Ϣ��ʱ�����ܵ�ǰ���������뿪��ǰ������", "��������", 
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);         
         
                    ContinueDebug();
                }
                
                debugWaitTimer.Tag = waitCount;
            }
        }

        /// <summary>
        /// �ȴ���ʼ����ȷ�ϵĵ�����Ϣ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void WaitStartDebugActivated(object sender, EventArgs e)
        {
            // ��������״̬
            string message = "start_debug";
            string receivedMessage = SendAndWaitMessage(message, "<ldb>start_debug:", true);

            if (receivedMessage == "<ldb>start_debug: success!") // ��ʼ���Գɹ�
            {
                // ͣ����ǰ��ʱ��
                StopDebugWaitTimer();

                PrintOutputText("��ʼ���Գɹ�...");

                // ���������Ϣ
                networkManager.ClearReceivedMessageList();

                // ע��ϵ�
                RegistBreakPoint();

                // �л�״̬
                startDebug = true;
                bStartDebug.Enabled = false;
                bStartDebugClient.Enabled = false;
                bStopDebug.Enabled = true;

                foreach (ButtonItem buttonItem in debugButtonList)
                {
                    buttonItem.Enabled = true;
                }

                // ���������Ѵ򿪵Ĵ���
                foreach (EditForm editForm in this.MdiChildren)
                {
                    editForm.luaEditBox.ReadOnly = true;
                    editForm.Text = string.Format("{0} [������]", editForm.Text);
                }                

                // �ȴ�����˴����ϵ�
                PrintOutputText("��ʼ�ȴ������ϵ�...");
                StartDebugWaitTimer(Helper.BreakPointWaitCircle, DebugState.WaitBreakPointActivated);
            }
            else if (receivedMessage == "<ldb>start_debug: failed!") // ��ʼ����ʧ��
            {
                PrintOutputText("��ʼ����ʧ��...");
                MessageBox.Show("ע�������Ϣʧ�ܣ��Ѿ��б���������Ӳ����нű����ԣ�", "��ʼ����",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �ȴ��ϵ㴥���ĵ�����Ϣ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void WaitBreakPointActivated(object sender, EventArgs e)
        {
            string receivedMessage = networkManager.GetMessageForHead("<ldb>break_at:");

            if (receivedMessage != null)
            {
                // ����ͣ��ǰ��ʱ��
                StopDebugWaitTimer();

                PrintOutputText(receivedMessage);

                object[] stepInfo = GetJumpInfoFromMessage(receivedMessage);

                if (stepInfo != null)
                {
                    string fileName = stepInfo[0] as string;
                    int lineIndex = (int)stepInfo[1];

                    // �Զ��е���ǰ�ű� 
                    foreach (EditForm editForm in this.MdiChildren)
                    {
                        if (ProcessFileName(editForm.FileName) == fileName)
                        {
                            SelectEditForm(editForm);

                            // ������ǰ�к�
                            editForm.luaEditBox.DebugRowIndex = lineIndex - 1;
                            editForm.luaEditBox.MoveToLine(lineIndex);
                            editForm.luaEditBox.Refresh();
                            break;
                        }
                    }

                    currentDebugFile = fileName;

                    // ˢ��һ�α���ֵ
                    RefreshVarValue();

                    // ˢ�µ���ջ��Ϣ
                    RefreshCallStack();

                    // ������н��յ���Ϣ
                    networkManager.ClearReceivedMessageList();
                }
            }
        }

        /// <summary>
        /// �༭�ؼ������¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void EditForm_KeyDown(object sender, KeyEventArgs e)
        {            
            switch (e.KeyCode)
            {
                case Keys.F10: // ��������
                    {
                        if (startDebug)
                        {
                            StepDebug("��������", "step_over");
                        }                       
                        
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }

        /// <summary>
        /// ���ر��ؽű���
        /// </summary>
        private void LoadLocalScriptTree()
        {
            // ��ȡ�����ļ�
            List<string> fileNameList = LoadLocalFileList();
            localFileList = fileNameList;

            // �������ݿ�ű�
            TreePhOpera tpo = new TreePhOpera();
            Hashtable infoTable = new Hashtable();

            DataTable treeTable = new DataTable();
            DataColumn idColumn = new DataColumn("id");
            DataColumn pathColumn = new DataColumn("path");

            treeTable.Columns.Add(idColumn);
            treeTable.Columns.Add(pathColumn);

            foreach (string s in fileNameList)
            {
                DataRow newRow = treeTable.NewRow();
                newRow["id"] = "0";
                newRow["path"] = s;
                treeTable.Rows.Add(newRow);
            }

            infoTable["dtable"] = treeTable;

            ArrayList icoList = new ArrayList();
            icoList.Add("folder");
            icoList.Add("file");

            infoTable["icolist"] = icoList;
            infoTable["pop_folder"] = popTreePh_LocalFolder;
            infoTable["pop_file"] = popTreePh_LocalFile;
            infoTable["scriptType"] = "localFile";

            foreach (TreeNode tn in tpo.LoadTreeNodeCollection(infoTable))
            {
                localFileRootNode.Nodes.Add(tn);
                tn.Expand();
            }         
        }

        /// <summary>
        /// �������ݿ�ű���
        /// </summary>
        private void LoadDataBaseScriptTree()
        {
            treePh.Nodes.Clear();

            // �������ݿ�ű�
            TreePhOpera tpo = new TreePhOpera();
            Hashtable htable = new Hashtable();
            DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
            
            treePh.Tag = dbm.GetScriptInformation();
            htable["dtable"] = dbm.GetScriptInformation();                      
            htable["pop_folder"] = popTreePh_Folder;
            htable["pop_file"] = popTreePh_File;
            htable["scriptType"] = "databaseFile";

            foreach (TreeNode tn in tpo.LoadTreeNodeCollection(htable))
            {
                treePh.Nodes.Add(tn);
                tn.Expand();
            }                 
     
            // ���ӱ����ļ����
            Hashtable infoTable = new Hashtable();
            infoTable["type"] = "folder";
            localFileRootNode = TreePhOpera.CreateNode("�����ļ�", "folder", popTreePh_LocalFileRoot, infoTable);

            treePh.Nodes.Add(localFileRootNode);  
        }

        /// <summary>
        /// ѡ�б༭�ؼ�
        /// </summary>
        /// <param name="editForm">�༭�ؼ�</param>
        private void SelectEditForm(EditForm editForm)
        {           
            foreach (DevComponents.DotNetBar.TabItem tabItem in this.tabStrip1.Tabs)
            {
                if (tabItem.AttachedControl == editForm)
                {
                    tabStrip1.SelectedTab = tabItem;
                    break;
                }
            }

            editForm.Activate();
            editForm.Focus();
        }

        /// <summary>
        /// �ڱ༭�����д򿪽ű��ļ�
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <param name="requireProcessFileName">�Ƿ���Ҫ�����ļ���</param>
        /// <returns>�Ƿ�򿪳ɹ�</returns>
        private EditForm OpenFileInEditor(string fileName, bool requireProcessFileName)
        {
            EditForm editForm = null;    
            string filePath = fileName.Replace(Helper.ClientPath, "").TrimStart(new char[] { '\\', '/' });

            foreach (EditForm eForm in this.MdiChildren)
            {
                string editorFileName = eForm.FileName;

                if (requireProcessFileName)
                {
                    editorFileName = ProcessFileName(editorFileName);
                }

                if (editorFileName == filePath)
                {
                    SelectEditForm(eForm);
                    return eForm;                    
                }
            }

            string content = Helper.FileToString(fileName);

            if (!string.IsNullOrEmpty(content))
            {
                editForm = new EditForm("localFile");
                editForm.ID = "0";

                // InitEditForm(editForm, content, filePath);                
                InitEditForm(editForm, content, fileName);
                editForm.Show();
                SelectEditForm(editForm);
            }

            return editForm;
        }

        /// <summary>
        /// �򿪶�Ӧ�ļ����Ľű�
        /// </summary>
        /// <param name="filename">�ļ���</param>
        /// <returns>��Ӧ�ű��Ƿ����</returns>
        public bool OpenFileByName(string filename)
        {
            if (SelectNodeByFileName(filename))
            {
                treePh_NodeMouseDoubleClick(this, new TreeNodeMouseClickEventArgs(treePh.SelectedNode, MouseButtons.Left, 2, 0, 0));
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// �򿪶�Ӧ�ļ����Ľű�
        /// </summary>
        /// <param name="filename">�ļ���</param>
        /// <returns>�Ƿ�򿪳ɹ�</returns>
        public bool SelectNodeByFileName(string filename)
        {
            return _SelectNodeByFileName(treePh.Nodes, filename);
        }

        /// <summary>
        /// �򿪶�Ӧ�ļ����Ľű�
        /// </summary>
        /// <param name="tnc">����㼯��</param>
        /// <param name="filename">�ļ���</param>
        /// <returns>�Ƿ�򿪳ɹ�</returns>
        private bool _SelectNodeByFileName(TreeNodeCollection tnc, string filename)
        {
            bool bret = false;

            foreach (TreeNode tn in tnc)
            {
                if(tn.Nodes.Count > 0)
                {
                    bret = _SelectNodeByFileName(tn.Nodes, filename);

                    if (bret)
                    {
                        return true;
                    }
                }
                else
                {
                    if (tn.FullPath == filename)
                    {
                        treePh.SelectedNode = tn;
                        return true;
                    }
                }
            }

            return bret;
        }

        // ahpho
        public void OpenTreeNodeScript(TreeNode node)
        {
            TreeNodeMouseClickEventArgs e = new TreeNodeMouseClickEventArgs(node, MouseButtons.Left, 2, 0, 0);
            treePh_NodeMouseDoubleClick(null, e);
        }

        public string GetTreeNodeScriptContent(TreeNode node)
        {
            if (node.Tag == null)
                return string.Empty;

            Hashtable ht = node.Tag as Hashtable;
            if (ht["type"].ToString() == "folder")
                return string.Empty;

            string id = ht["id"] as string;
            DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
            string strCode = dbm.GetScriptData(id);

            if (strCode != null)
            {
                return strCode;
            }
            return string.Empty;
        }

        /// <summary>
        /// ��鱾���ļ�����û�иı��
        /// </summary>
        private bool CheckLocalFileTreeChanged()
        {
            bool changed = false;
            List<string> fileNameList = new List<string>();

            InitLocalFileNameList(localFileRootNode, fileNameList);

            if (fileNameList.Count == localFileList.Count)
            {
                foreach (string s in localFileList)
                {
                    if (!fileNameList.Contains(s))
                    {
                        changed = true;
                        break;
                    }
                }
            }
            else
            {
                changed = true;
            }

            return changed;
        }

        /// <summary>
        /// ���汾�ص��ļ�����
        /// </summary>
        private void SaveLocalFileList()
        {
            List<string> fileNameList = new List<string>();

            InitLocalFileNameList(localFileRootNode, fileNameList);

            if (fileNameList.Count > 0)
            {
                string fileName = Path.Combine(Application.StartupPath, "LuaEditor.ini");

                IniIO.IniWriteValue("General", "LocalFileCount", fileNameList.Count.ToString(), fileName);

                for (int i = 0; i < fileNameList.Count; i++)
                {
                    IniIO.IniWriteValue("LocalFile", string.Format("File{0}", i), fileNameList[i], fileName);
                }                
            }
        }

        /// <summary>
        /// ���ر��ص��ļ�����
        /// </summary>
        /// <returns>�ļ�����</returns>
        private List<string> LoadLocalFileList()
        {
            List<string> fileNameList = new List<string>();
            string fileName = Path.Combine(Application.StartupPath, "LuaEditor.ini");

            string fileCountString = IniIO.IniReadValue("General", "LocalFileCount", fileName);

            if (!string.IsNullOrEmpty(fileCountString))
            {
                int fileCount = int.Parse(fileCountString);

                for (int i = 0; i < fileCount; i++)
                {
                    string localFileName = IniIO.IniReadValue("LocalFile", string.Format("File{0}", i), fileName);
                    fileNameList.Add(localFileName);
                }
            }

            return fileNameList;
        }

        /// <summary>
        /// ��ʼ�������ļ�������
        /// </summary>
        /// <param name="treeNode"></param>
        /// <param name="fileNameList"></param>
        private void InitLocalFileNameList(TreeNode treeNode, List<string> fileNameList)
        {
            Hashtable infoTable = treeNode.Tag as Hashtable;

            if (infoTable["type"] as string == "file")
            {
                fileNameList.Add(treeNode.FullPath.Replace("�����ļ�\\", ""));
            }
            else
            {
                foreach (TreeNode node in treeNode.Nodes)
                {
                    InitLocalFileNameList(node, fileNameList);
                }
            }            
        }

        /// <summary>
        /// ת���ļ���
        /// </summary>
        /// <param name="fileName">���ļ���</param>
        /// <returns>���ļ���</returns>
        private string ProcessFileName(string fileName)
        {
            string newFileName = fileName.ToLower().Replace("\\", "/");

            return newFileName;
        }

        /// <summary>
        /// ��ʼ���༭����
        /// </summary>
        /// <param name="editForm">�༭����</param>
        /// <param name="scriptCode">�ű�����</param>
        /// <param name="scriptPath">�ű�·��</param>
        private void InitEditForm(EditForm editForm, string scriptCode, string scriptPath)
        {
            editForm.ScriptCode = scriptCode;
            editForm.WindowState = FormWindowState.Maximized;
            editForm.MdiParent = this;
            editForm.FileName = scriptPath;
            editForm.EditorContextMenu = popRight;
            editForm.luaEditBox.ScriptFont = Helper.ScriptFont;
            editForm.luaEditBox.ScriptForeColor = Helper.ScriptForeColor;
            editForm.HandleBreakPointAdded += new EditForm.BreakPointAdded(EditFormBreakPointAdded);
            editForm.HandleBreakPointRemoved += new EditForm.BreakPointRemoved(EditFormBreakPointRemoved);
            editForm.HandleWordMouseHover += new EditForm.WordMouseHover(EditFormWordMouseHover);
            editForm.luaEditBox.KeyDown += new KeyEventHandler(EditForm_KeyDown);
            editForm.luaEditBox.ReadOnly = startDebug; // ����ģʽ�µĴ򿪴���ֻ��ֻ����

            if (startDebug)
            {
                editForm.Text = string.Format("{0} [������]", editForm.Text);
            }
        }

        /// <summary>
        /// ��ӱ����ļ��ű����
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        private void AddLocalFileNode(string fileName)
        {
            fileName = fileName.Replace(Helper.ClientPath, "").TrimStart(new char[] { '\\' });
            string[] dataArray = fileName.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);

            TreeNode rootNode = localFileRootNode;
            TreeNode currentNode = null;

            for (int i = 0; i < dataArray.Length; i++)
            {
                string s = dataArray[i];
                currentNode = null;
                
                foreach (TreeNode childNode in rootNode.Nodes)
                {
                    if (childNode.Text == s)
                    {
                        currentNode = childNode;
                        rootNode = childNode;
                        break;
                    }
                }

                if (currentNode == null)
                {
                    Hashtable infoTable = new Hashtable();
                    ContextMenuStrip contextMenuStrip = null;
                    string nodeType = null;

                    if (i == dataArray.Length - 1) // �ļ����
                    {
                        nodeType = "file";
                        infoTable["type"] = "file";
                        infoTable["scriptType"] = "localFile";
                        infoTable["id"] = "0";
                        infoTable["filePath"] = fileName;
                    }
                    else // �ļ��н��
                    {
                        nodeType = "folder";
                        infoTable["type"] = nodeType;
                        contextMenuStrip = popTreePh_LocalFolder;
                    }                                        
                    
                    currentNode = TreePhOpera.CreateNode(s, nodeType, contextMenuStrip, infoTable);
                    rootNode.Nodes.Add(currentNode);
                    rootNode = currentNode;
                }
            }

            treePh.SelectedNode = currentNode;
        }

        /// <summary>
        /// ˫�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void treePh_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            TreeNode currentNode = treePh.SelectedNode;

            if (currentNode.Tag == null)
            {
                return;
            }

            Hashtable infoTable = currentNode.Tag as Hashtable;

            if (infoTable["type"] as string == "folder")
            {
                return;
            }

            string scriptType = infoTable["scriptType"] as string;

            if (scriptType == "databaseFile") // �����ݿ���Ľű��ļ�
            {
                string id = infoTable["id"] as string;
                EditForm editForm = null;

                foreach (EditForm eForm in this.MdiChildren)
                {
                    if (eForm.ID == id)
                    {
                        editForm = eForm;
                    }
                }

                if (editForm == null)
                {
                    editForm = new EditForm("databaseFile");
                    editForm.ID = id;
                    DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
                    string strCode = dbm.GetScriptData(id);

                    if (strCode == null)
                    {
                        MessageBox.Show("���ؽű�ʧ�ܣ�", "���ؽű�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }

                    InitEditForm(editForm, strCode, currentNode.FullPath);
                    editForm.Show();
                }

                SelectEditForm(editForm);
            }
            else if (scriptType == "localFile") // �򿪱��صĽű��ļ�
            {
                string filePath = infoTable["filePath"] as string;
                filePath = Path.Combine(Helper.ClientPath, filePath);
                OpenFileInEditor(filePath, false);
            }
        }

        /// <summary>
        /// �ű���������굥���¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void treePh_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            // ˢ���Ҽ��˵�״̬
            if (e.Node != null)
            {
                // ��ѡ��һ�Σ������Ҽ��˵�����������ȷ������
                treePh.SelectedNode = e.Node;

                string scriptPath = e.Node.FullPath;                
                string[] data = scriptPath.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);

                cmdTreePhFolder_newFolder.Enabled = true;
                cmdTreePhFolder_newFile.Enabled = true;
                cmdTreePhFolder_rename.Enabled = true;
                cmdTreePhFolder_del.Enabled = true;

                if (data.Length > 1)
                {                    
                    if (data.Length == 2 && scriptPath == @"scripts\Map") // scripts\MapsĿ¼��ֻ���ǳ����ļ���
                    {
                        cmdTreePhFolder_newFile.Enabled = false;
                    }                    
                }
                else // ��Ŀ¼��������������ɾ��
                {
                    cmdTreePhFolder_rename.Enabled = false;
                    cmdTreePhFolder_del.Enabled = false;
                }                               
            }
        }
      
        /// <summary>
        /// �������ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cmdTreePhFile_reanme_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treePh.SelectedNode;

            if (currentNode != null)
            {
                currentNode.BeginEdit();
            }
        }

        /// <summary>
        /// ɾ���ű��ļ���
        /// </summary>
        /// <param name="folderNode">�ű��ļ��������</param>
        /// <return>�Ƿ�ɾ���ɹ�</return>
        private bool DeleteScriptFolder(TreeNode folderNode)
        {
            List<TreeNode> childNodeList = new List<TreeNode>();

            foreach (TreeNode childNode in folderNode.Nodes)
            {
                childNodeList.Add(childNode);
            }

            foreach (TreeNode childNode in childNodeList)
            {
                Hashtable infoTable = childNode.Tag as Hashtable;

                if (infoTable["type"] as string == "folder")
                {
                    DeleteScriptFolder(childNode);
                }
                else if (infoTable["type"] as string == "file")
                {
                    DeleteScriptFile(childNode);
                }
            }

            folderNode.Remove();

            return true;
        }

        /// <summary>
        /// ɾ���ű��ļ�
        /// </summary>
        /// <param name="scriptNode">�ű��ļ������</param>
        /// <returns>�Ƿ�ɾ���ɹ�</returns>
        private bool DeleteScriptFile(TreeNode fileNode)
        {
            bool deleteSuccess = false;
            
            Hashtable infoTable = fileNode.Tag as Hashtable;
            string id = infoTable["id"].ToString();
            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();

            if (!dataBaseManager.IsScriptLocked(id))
            {
                if (dataBaseManager.DeleteScriptData(id)) // �������ݿ���ɾ��
                {
                    fileNode.Remove();

                    // �޸ĳɹ�������������
                    treePh.Tag = dataBaseManager.GetScriptInformation();
                }
            }
            else
            {
                MessageBox.Show("�ýű��ѱ��������޷�ɾ����", "ɾ���ű�",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return deleteSuccess;
        }

        /// <summary>
        /// ɾ���ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cmdTreePhFile_del_Click(object sender, EventArgs e)
        {            
            if (MessageBox.Show("ȷ��Ҫɾ����ǰ�ű���", "ɾ���ű�", 
                                MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)                
            {
                TreeNode currentNode = treePh.SelectedNode;

                if (currentNode != null)
                {
                    DeleteScriptFile(currentNode);
                }            
            }            
        }
        
        /// <summary>
        /// �����ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cmdTreePhFile_export_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treePh.SelectedNode;

            if (currentNode != null)
            {
                Hashtable infoTable = currentNode.Tag as Hashtable;
                string id = infoTable["id"].ToString();
                List<string> scriptIDList = new List<string>();
                scriptIDList.Add(id);                

                exportForm = new ExportForm(scriptIDList, Helper.ClientPath);
                exportForm.ShowDialog();
            }            
        }

        #region Ŀ¼�Ҽ��˵�

        /// <summary>
        /// ���������Ƿ��Ѿ�����
        /// </summary>
        /// <param name="parentNode">�����</param>
        /// <param name="nodeName">�������</param>
        /// <returns></returns>
        private bool CheckNodeExist(TreeNode parentNode, string nodeName)
        {
            bool exist = false;

            foreach (TreeNode node in parentNode.Nodes)
            {
                if (node.Text == nodeName)
                {
                    exist = true;
                    break;
                }
            }

            return exist;
        }

        /// <summary>
        /// �½�Ŀ¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cmdTreePhFolder_newFolder_Click(object sender, EventArgs e)
        {
            if (treePh.SelectedNode != null)
            {
                string strName = Helper.InputBox("��Ŀ¼", "����Ŀ¼��");

                if (!string.IsNullOrEmpty(strName))
                {                    
                    if (CheckNodeExist(treePh.SelectedNode, strName))
                    {
                        MessageBox.Show("��ǰ������Ѿ����ڣ�", "�½�Ŀ¼", 
                                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return;
                    }

                    TreeNode newNode = treePh.SelectedNode.Nodes.Add(strName, strName);
                    Hashtable ht = new Hashtable();
                    ht["type"] = "folder";
                    newNode.Tag = ht;
                    newNode.ImageKey = newNode.SelectedImageKey = "folder";
                    newNode.ContextMenuStrip = this.popTreePh_Folder;
                    treePh.SelectedNode = newNode;
                }
            }                        
        }

        /// <summary>
        /// �½��ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cmdTreePhFolder_newFile_Click(object sender, EventArgs e)
        {
            if (treePh.SelectedNode != null)
            {
                // ��ʼ��lua����
                string strFullPath = treePh.SelectedNode.FullPath;
                Lua plua = new Lua();
                object[] oRet = null;
                
                plua.DoString(DataBaseManager.GetDataBaseManager().GetScriptData(this.getFileIDsFromPath("Template")[0].ToString()));
                LuaFunction fun = plua.GetFunction("LoadParms");

                if (fun != null)
                {
                    object[] args = { strFullPath };
                    oRet = fun.Call(args);

                    if (oRet == null)
                    {
                        return;
                    }
                }                            
                
                string strControls = oRet[0] as string;

                // ��躷��ص�����
                object[] strParms;

                // �ļ���
                string strName = "";
                string[] asp1 = { "|" };
                ModelForm mf = new ModelForm(strControls.Split(asp1, StringSplitOptions.RemoveEmptyEntries));

                if (mf.ShowDialog() == DialogResult.OK)
                {
                    List<string> list = mf.InputList;

                    if (list.Count >= 2)
                    {
                        strParms = list.ToArray();
                        strName = strParms[1] as string;

                        string strCode = "";
                        fun = plua.GetFunction("LoadScript");

                        if (fun != null)
                        {
                            oRet = fun.Call(strParms);
                            strCode = oRet[0] as string;
                        }
                        else
                        {
                            return;
                        }                

                        // �������
                        if (string.IsNullOrEmpty(strName))
                        {
                            return;
                        }

                        if (CheckNodeExist(treePh.SelectedNode, strName))
                        {
                            MessageBox.Show("��ǰ������Ѿ����ڣ�", "�½��ļ�",
                                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                            return;
                        }

                        Hashtable infoTable = new Hashtable();
                        infoTable["type"] = "file";

                        // �����ݿ�
                        DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
                        string strPath = treePh.SelectedNode.FullPath;
                        string[] asp = { "\\" };
                        string[] as_Path = strPath.Split(asp, StringSplitOptions.RemoveEmptyEntries);

                        if (as_Path.Length < 2 && as_Path[0] == "scripts")
                        {
                            MessageBox.Show("��ʱ��֧����������ͼ�����Ŀ¼���ļ�", "�½��ļ�", 
                                            MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        }

                        string id = dbm.CreateScriptData(string.Format("{0}\\{1}", strPath, strName));

                        if (id == null)
                        {
                            MessageBox.Show("�½��ļ�ʧ�ܣ�", "�½��ļ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        }
                        else
                        {
                            // �½��ɹ�������������
                            treePh.Tag = dbm.GetScriptInformation();
                        }

                        infoTable["id"] = id;
                        infoTable["scriptType"] = "databaseFile";
                        TreeNode newNode = treePh.SelectedNode.Nodes.Add(strName, strName);
                        newNode.ImageKey = "file";
                        newNode.SelectedImageKey = "file";
                        newNode.Tag = infoTable;
                        newNode.ContextMenuStrip = this.popTreePh_File;
                        treePh.SelectedNode.Expand();
                        treePh.SelectedNode = newNode;
                        
                        // ģ�����ڵ�
                        this.treePh_NodeMouseDoubleClick(this, new TreeNodeMouseClickEventArgs(newNode, MouseButtons.Left, 2, 1, 1));

                        EditForm fe = this.ActiveMdiChild as EditForm;
                        
                        // ģ������д��
                        if (fe != null)
                        {
                            fe.luaEditBox.Text = strCode;
                        } 
                    } 
                }                
            }        
        }

        /// <summary>
        /// �������ű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cmdTreePhFolder_rename_Click(object sender, EventArgs e)
        {
            if (treePh.SelectedNode != null)
            {
                treePh.SelectedNode.BeginEdit();
            }
        }

        /// <summary>
        /// ɾ��Ŀ¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cmdTreePhFolder_del_Click(object sender, EventArgs e)
        {
            TreeNode folderNode = treePh.SelectedNode;

            if (folderNode != null)
            {
                if (MessageBox.Show("ȷ��Ҫɾ����ǰĿ¼��", "ɾ��Ŀ¼",
                                    MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
                {
                    DeleteScriptFolder(folderNode);
                }
            }
        }

        /// <summary>
        /// ����Ŀ¼�µ����нű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cmdTreePhFolder_export_Click(object sender, EventArgs e)
        {
            TreeNode tn_folder = treePh.SelectedNode;            

            if (tn_folder != null)
            {
                List<string> scriptIDList = new List<string>();
                fillArrayListInIDFromNode(scriptIDList, tn_folder);
                exportForm = new ExportForm(scriptIDList, Helper.ClientPath);
                exportForm.ShowDialog();
            }            
        }

        //����Ŀ¼�ڵ�/�ļ��ڵ㣬�ݹ������ļ���id ��䵽al_list
        private void fillArrayListInIDFromNode(List<string> idList, TreeNode tnode)
        {
            Hashtable ht = tnode.Tag as Hashtable;
            if(ht["type"].ToString() == "file")
            {
                idList.Add(ht["id"].ToString());
                return;
            }
            foreach(TreeNode tn in tnode.Nodes)
            {
                    Hashtable _ht = tn.Tag as Hashtable;
                if(_ht["type"].ToString() == "file")
                {
                    idList.Add(_ht["id"].ToString());
                }
                else
                {
                    fillArrayListInIDFromNode(idList, tn);
                }
            }
        }

        //����Ŀ¼���������ļ�ID�б�
        public List<string> getFileIDsFromPath(string strPath)
        {
            List<string> idList = new List<string>();
            string [] asp = {"\\"};
            string [] as_path = strPath.Split(asp,  StringSplitOptions.RemoveEmptyEntries);
            foreach( TreeNode tn in treePh.Nodes.Find( as_path[as_path.Length - 1], true))
            {
                if(tn.FullPath == strPath)
                {
                    fillArrayListInIDFromNode(idList, tn);
                    break;
                }
            }
            return idList;
        }

        /// <summary>
        /// �鿴Ŀ¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cmdTreePhFolder_openlocal_Click(object sender, EventArgs e)
        {            
            string strPath = System.IO.Path.Combine(Helper.ClientPath, treePh.SelectedNode.FullPath);

            if (System.IO.Directory.Exists(strPath))
            {
                Helper.Execute("explorer.exe", string.Format("\"{0}\"", strPath), "true");
            }
            else
            {
                MessageBox.Show("�Ҳ���·�������ȵ�����");
            }
        }

        #endregion

        /// <summary>
        /// ȡ���ҽӿͻ��˵��Խű�
        /// </summary>
        private void DetachClientDebugFile()
        {
            string filePath = Path.Combine(Helper.ClientPath, @"ui\loginscript.ini");

            if (File.Exists(filePath))
            {
                string content = Helper.FileToString(filePath);

                if (content.Contains(@"\scripts\ldb.lua")) // ��Ҫ�ҽ�
                {
                    StringBuilder newContent = new StringBuilder();
                    string[] lines = content.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);                    

                    foreach (string s in lines)
                    {
                        if (!s.EndsWith(@"\scripts\ldb.lua"))
                        {
                            newContent.AppendLine(s);
                        }
                    }

                    Helper.WriteStringToFile(newContent.ToString(), filePath);
                }
            }
        }

        /// <summary>
        /// �ҽӿͻ��˵��Խű�
        /// </summary>
        /// <return>�Ƿ�ҽӳɹ�</return>
        private bool AttachClientDebugFile()
        {
            bool attachSuccess = false;
            string filePath = Path.Combine(Helper.ClientPath, @"ui\loginscript.ini");

            if (File.Exists(filePath))
            {
                string content = Helper.FileToString(filePath);

                if (!content.Contains(@"\scripts\ldb.lua")) // ��Ҫ�ҽ�
                {
                    StringBuilder newContent = new StringBuilder();
                    string[] lines = content.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
                    int fileCount = lines.Length - 2; // ������ؽű�������

                    foreach (string s in lines)
                    {
                        newContent.AppendLine(s);
                    }

                    newContent.AppendLine(string.Format(@"lua_{0}=\scripts\ldb.lua", fileCount));
                    Helper.WriteStringToFile(newContent.ToString(), filePath);
                }

                attachSuccess = true;
            }
            else
            {
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", filePath), "�ҽӵ��Խű�",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return attachSuccess;
        }

        /// <summary>
        /// ȡ���ҽӷ���˵��Խű�
        /// </summary>
        private void DetachServerDebugFile()
        {
            string filePath = Path.Combine(Helper.ClientPath, @"scripts\server_main.lua");

            if (File.Exists(filePath))
            {
                string content = Helper.FileToString(filePath);

                if (!string.IsNullOrEmpty(content))
                {
                    if (content.Contains("Include('scripts/ldb.lua')"))
                    {
                        StringBuilder newContent = new StringBuilder();
                        string[] lines = content.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);

                        foreach (string s in lines)
                        {
                            string line = s.Trim(new char[] { ' ', '\t' });

                            if (line != "Include('scripts/ldb.lua')")
                            {
                                newContent.AppendLine(s);
                            }
                        }

                        // ��Ҫд�ļ�
                        if (content != newContent.ToString())
                        {
                            Helper.WriteStringToFile(newContent.ToString(), filePath);
                        }
                    }                    
                }
            }
        }

        /// <summary>
        /// �ҽӷ���˵��Խű�
        /// </summary>
        /// <return>�Ƿ�ҽӳɹ�</return>
        private bool AttachServerDebugFile()
        {
            bool attachSuccess = false;
            string filePath = Path.Combine(Helper.ClientPath, @"scripts\server_main.lua");

            if (File.Exists(filePath))
            {
                string content = Helper.FileToString(filePath);

                if (!string.IsNullOrEmpty(content))
                {
                    StringBuilder newContent = new StringBuilder();
                    string[] lines = content.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);

                    foreach (string s in lines)
                    {
                        string line = s.Trim(new char[] { ' ', '\t' });

                        switch (line)
                        {
                            case "function OnGameServerConnected(nConnIndex)":
                                {
                                    newContent.AppendLine(s);
                                    newContent.AppendLine("\tInclude('scripts/ldb.lua')");
                                    break;
                                }
                            case "Include('scripts/ldb.lua')":
                                {
                                    break;
                                }
                            default:
                                {
                                    newContent.AppendLine(s);
                                    break;
                                }
                        }
                    }

                    // ��Ҫд�ļ�
                    if (content != newContent.ToString())
                    {
                        Helper.WriteStringToFile(newContent.ToString(), filePath);
                    }

                    attachSuccess = true;  
                }
            }
            else
            {
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", filePath), "�ҽӵ��Խű�",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return attachSuccess;
        }

        /// <summary>
        /// ����Ŀ¼��������{ [{�ļ�ID}: {�ļ������ļ�����}] ������}��Hashtable
        /// </summary>
        /// <param name="strPath">Ŀ¼��</param>
        /// <returns>��Ϣ���ݱ�</returns>
        public Hashtable getFileListFromPath(string strPath)
        {
            List<string> fileList = getFileIDsFromPath(strPath);
            ProgressForm pForm = null;

            if (fileList.Count > 0)
            {
                pForm = new ProgressForm(0, fileList.Count);
                pForm.Show(); 
            }                       
            
            Hashtable ht_ret = new Hashtable();
            DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
            DataTable dt = dbm.GetScriptInformation();
            int index = 1;

            foreach(string id in fileList)
            {                
                Hashtable ht_tmp = new Hashtable();
                DataRow[] dr = dt.Select("id = " + id);
                if(dr.Length == 0 ) return null;
                ht_tmp["filename"] = dr[0]["path"];
                ht_tmp["filecontent"] = dbm.GetScriptData(id);
                ht_ret.Add(id, ht_tmp);

                if (pForm != null)
                {
                    pForm.ShowProgress(index, string.Format("����{0}...", ht_tmp["filename"] as string));
                }
                
                index++;
            }

            return ht_ret;
        }

        public Hashtable getFileNameListFromPath(string strPath)
        {
            List<string> idList = getFileIDsFromPath(strPath);
            Hashtable ht_ret = new Hashtable();
            DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
            DataTable dt = dbm.GetScriptInformation();
            foreach(string id in idList)
            {
                DataRow[] dr = dt.Select("id=" + id);
                if(dr.Length == 0 ) return null;
                ht_ret.Add(id, dr[0]["path"]);
            }
            return ht_ret;
        }        

        /// <summary>
        /// ���������ķ�����
        /// </summary>
        /// <returns>�������Ƿ�ı�</returns>
        private bool CheckTreePhServerChanged()
        {
            DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
            DataTable dt = treePh.Tag as DataTable;

            if(DataTable2String(dbm.GetScriptInformation()) != DataTable2String(dt))
            {
                MessageBox.Show("Ŀ¼�ṹ�Ѿ��ڷ������˱����ģ����򽫻���������Ŀ¼��", "��ʾ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                LoadDataBaseScriptTree();
                return true;
            }

            return false;
        }

        private string DataTable2String(DataTable dt)
        {
            string strRet = "";
            foreach(DataRow dr in dt.Rows)
            {
                strRet += dr["id"] + "," + dr["path"] + ",";
            }
            return strRet;
        }

        //�ļ���Ŀ¼����������
        private void treePh_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            if (e.Label == null || e.Label == "" || e.Label == e.Node.Text)
            {
                e.CancelEdit = true;
                return;
            }

            //��黺�������Ƿ�ͷ�����ƥ��
            if(CheckTreePhServerChanged())
            {
                e.CancelEdit = true;
                return;
            }

            //ǿ�Ʋ����������Ŀ¼
            // if(e.Node.FullPath == @"scripts\Map" || e.Node.FullPath == @"scripts")
            // {
            //     MessageBox.Show("���ڸ���scriptsĿ¼��MapĿ¼�����������Ŀ¼�ļ��ĵ�ͼ��Ϣ��ʧ����ǿ�ƹ涨������Ŀ¼���ܸ�����", "��ʾ", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //     e.CancelEdit = true; 
            //     return;
            // }
            if(e.Node.FullPath.StartsWith(@"scripts\Map\"))
            {
                char [] asp = {'\\'};
                if(e.Node.FullPath.Split(asp).Length == 3)
                {
                    MessageBox.Show("��ͼ������ʱ�������޸�", "��ʾ", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    e.CancelEdit = true;
                    return;
                }
            }

            Hashtable ht = e.Node.Tag as Hashtable;
            string strType = ht["type"] as string;
            if (strType == "file")
            {
                DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
                bool bret = false;
                string id = ht["id"].ToString();
                if (dbm.IsScriptLocked(id))
                {
                    MessageBox.Show("�ļ����ڱ��˱༭�����ܸ���", "��ʾ", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    e.CancelEdit = true;
                    return;
                }
                string strParent = e.Node.FullPath.Substring(0, e.Node.FullPath.LastIndexOf("\\"));
                string strNewFilename = strParent + "\\" + e.Label;

                //�������
                DataTable dt = treePh.Tag as DataTable;
                if (dt.Select("path='" + strNewFilename + "'").Length > 0)
                {
                    MessageBox.Show("������ʧ�ܣ����ظ����ļ�����", "��ʾ", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    e.CancelEdit = true;
                    return;
                }

                bret = dbm.UpdateScriptPath(e.Node.FullPath, strNewFilename);
                if (!bret) 
                    e.CancelEdit = true;
                else
                {
                    //�޸ĳɹ�������������
                    treePh.Tag = dbm.GetScriptInformation();
                }
            }
            else if(strType == "folder")
            {
                string strParent = "";
                string strNewFolderRoot = "";
                if (e.Node.FullPath.IndexOf("\\") != -1)
                {
                    strParent = e.Node.FullPath.Substring(0, e.Node.FullPath.LastIndexOf("\\"));
                    strNewFolderRoot = strParent + "\\" + e.Label;
                }
                else
                {
                    strNewFolderRoot = e.Label;
                }
                if (e.Node.FullPath.ToLower() != strNewFolderRoot.ToLower())
                {
                    MessageBox.Show("Ŀ¼����ʱֻ����Ĵ�Сд��");
                    e.CancelEdit = true;
                    return;
                }

                DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
                bool bret = true;
                List<string> idList = getFileIDsFromPath(e.Node.FullPath);

                foreach(string tid in idList)
                {
                    if (dbm.IsScriptLocked(tid))
                    {
                        MessageBox.Show("Ŀ¼�����ļ����ڱ����˱༭���ʲ��ܸ�����", "��ʾ", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        e.CancelEdit = true;
                        return;
                    }
                }

                //�������
                DataTable dt = treePh.Tag as DataTable;
                if (e.Node.FullPath.ToLower() != strNewFolderRoot.ToLower())
                {
                    if (dt.Select("path like '" + strNewFolderRoot + "\\%'").Length > 0)
                    {
                        MessageBox.Show("������ʧ�ܣ����ظ���Ŀ¼����");
                        e.CancelEdit = true;
                        return;
                    }
                }

                bret = dbm.UpdateScriptPath(e.Node.FullPath, strNewFolderRoot);
                if (!bret)
                {
                    MessageBox.Show("�������ݿ�ʧ�ܣ��޷�������");
                    e.CancelEdit = true;
                }
                else
                {
                    //�޸ĳɹ�������������
                    treePh.Tag = dbm.GetScriptInformation();
                }
            }
        }

        /// <summary>
        /// ���浱ǰ�ű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void btnSave_Click(object sender, EventArgs e)
        {
            EditForm editForm = this.ActiveMdiChild as EditForm;

            string strOut = ScriptCheck();

            if (strOut != null)
            {                
                strOut = strOut.TrimEnd('\n');
            }

            if (!string.IsNullOrEmpty(strOut)) // �ű����������
            {
                if (MessageBox.Show(string.Format("{0} �ű��а�������\r\n{1}\r\n\r\nȷ�ϱ�����", editForm.FileName, strOut), "�ű����", 
                                    MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
                {
                    editForm.DoSave();
                    TreePhOpera.SetNodeColorByID(treePh, editForm.ID, Color.Red);
                }
            }
            else
            {
                TreePhOpera.SetNodeColorByID(treePh, editForm.ID, Color.Black);
                bool bret = editForm.DoSave();

                if (bret)
                {
                    // ������Զ�����
                    if (editForm.ID != "0")
                    {
                        List<string> scriptIDList = new List<string>();
                        scriptIDList.Add(editForm.ID);
                        exportForm = new ExportForm(scriptIDList, Helper.ClientPath);
                        exportForm.ShowDialog();
                    }                    
                }
            }            
        }

        /// <summary>
        /// �������нű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void btnSaveAll_Click(object sender, EventArgs e)
        {
            foreach (EditForm editForm in this.MdiChildren)
            {
                string strOut = ScriptCheck(editForm);

                if (strOut != null)
                {
                    strOut = strOut.TrimEnd('\n');
                }

                if (!string.IsNullOrEmpty(strOut))
                {
                    if (MessageBox.Show(string.Format("{0} �ű��а�������\r\n{1}\r\n\r\nȷ�ϱ�����", editForm.FileName, strOut), "�ű����",
                                        MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
                    {
                        editForm.DoSave();
                        TreePhOpera.SetNodeColorByID(treePh, editForm.ID, Color.Red);
                    }

                    continue;
                }
                else
                {
                    TreePhOpera.SetNodeColorByID(treePh, editForm.ID, Color.Black);
                    editForm.DoSave();
                }                
            }
        }

        /// <summary>
        /// �����滻�ַ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void btnStringReplace_Click(object sender, EventArgs e)
        {
            //����ǰ��ͼ��ѡ������
            string mapName = "";
            string findText = "";
            EditForm editForm = this.ActiveMdiChild as EditForm;

            if (editForm != null) 
            {
                if (editForm.luaEditBox.SelectedText != "")
                {
                    findText = editForm.luaEditBox.SelectedText;
                }
                else
                {
                    findText = editForm.luaEditBox.SelectedWord;
                }

                if (editForm.FileName.StartsWith(@"scripts\Map\"))
                {                    
                    string[] dataArray = editForm.FileName.Split(new char[] { '\\' }, StringSplitOptions.None);
                    mapName = dataArray[2];
                }
            }
            
            if (stringReplaceForm == null)
            {
                stringReplaceForm = new StringReplaceForm(this);
            }
            
            stringReplaceForm.Init(findText, mapName);            
        }

        /// <summary>
        /// �ָ���ɾ���Ľű�����������
        /// </summary>
        /// <param name="path">�¼�������</param>
        /// <param name="content">�¼�</param>
        private void InsertNewNode(string path, string content)
        {
            string[] paths = path.Split('\\');
            TreeNodeCollection currentNodes = treePh.Nodes;
            TreeNode currentNode = null;
            bool findNode = false;

            foreach (string s in paths)
            {
                findNode = false;
                if (s != "")
                {
                    foreach (TreeNode node1 in currentNodes)
                    {
                        if (node1.Text == s)
                        {
                            if(s.EndsWith(".lua")) // ��������Ѿ�����
                            {
                                DialogResult result = MessageBox.Show("�Ѿ�������ͬ·�����ļ����ļ�¼���Ƿ񸲸�����?", "��ʷ��¼�ָ�",
                                    MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                                if(result == DialogResult.Yes)
                                {
                                    string strView = "";
                                    bool coverSuccess = DataBaseManager.GetDataBaseManager().SaveScriptData(path, content, out strView);
                                    if(coverSuccess)
                                    {
                                        treePh.SelectedNode = node1;
                                        MessageBox.Show("��ɾ���Ľű��ָ��ɹ�!", "��ʷ�ű��ָ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    }
                                    else
                                    {
                                        MessageBox.Show("��ɾ���Ľű��ָ�ʧ��!", "��ʷ�ű��ָ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    }
                                }
                                findNode = true;
                                break;
                            }
                            else
                            {
                                findNode = true;
                                currentNodes = node1.Nodes;
                                break;
                            }
                            
                        }
                    }

                    if (!findNode)
                    {
                        currentNode = currentNodes.Add(s);
                        currentNodes = currentNode.Nodes;
                        if (s.EndsWith(".lua")) // �ļ�
                        {
                            currentNode.ImageIndex = 1;
                            currentNode.SelectedImageIndex = 1;

                            string id = DataBaseManager.GetDataBaseManager().CreateScriptData(path);
                            if(id != null)
                            {
                                string strView = "";
                                bool result = DataBaseManager.GetDataBaseManager().SaveScriptData(path, content, out strView);

                                if(result)
                                {
                                    Hashtable ht = new Hashtable();
                                    ht["type"] = "file";
                                    ht["id"] = id;
                                    currentNode.Tag = ht;
                                    MessageBox.Show("��ɾ���Ľű��ָ��ɹ�!", "��ʷ�ű��ָ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    treePh.SelectedNode = currentNode;
                                }
                                else
                                {
                                    MessageBox.Show("��ɾ���Ľű��ָ�ʧ��!", "��ʷ�ű��ָ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                }
                            }
                        }
                        else // �ļ���
                        {
                            currentNode.ImageIndex = 0;
                            currentNode.SelectedImageIndex = 0;
                            Hashtable ht = new Hashtable();
                            ht["type"] = "folder";
                            currentNode.Tag = ht;
                        }
                    }
                }
            }

            
        }

        /// <summary>
        /// �û�ѡ��鿴��ʷ��¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void btnHistory_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treePh.SelectedNode;

            if (currentNode != null && currentNode.Tag != null)
            {
                Hashtable infoTable = currentNode.Tag as Hashtable;

                if (infoTable["type"] as string == "file" && infoTable["scriptType"] as string == "databaseFile")
                {
                    string id = infoTable["id"] as string;          
                    HistoryForm hForm = new HistoryForm(DataBaseManager.GetDataBaseManager().Connection, id);
                    hForm.TreeImageList = imageList1;
                    hForm.Text = string.Format("�鿴��ʷ��¼  {0}", currentNode.FullPath);

                    if (hForm.ShowDialog() == DialogResult.Yes)
                    {
                        if (hForm.RecoverRecord) // �ָ���ʷ��¼
                        {
                            InsertNewNode(hForm.Path, hForm.ScriptData);
                        }
                        else // ��ȡ��ʷ��¼
                        {
                            EditForm feshow = null;

                            foreach (EditForm fe in this.MdiChildren)
                            {
                                if (fe.ID == id)
                                {
                                    feshow = fe;
                                    break;
                                }
                            }

                            if (feshow == null)
                            {
                                feshow = new EditForm("databaseFile");
                            }

                            feshow.ID = id;
                            feshow.ScriptCode = hForm.ScriptData;
                            feshow.WindowState = FormWindowState.Maximized;
                            feshow.MdiParent = this;
                            feshow.FileName = currentNode.FullPath;
                            feshow.Show();
                        }                  
                    }
                }
                else
                {
                    MessageBox.Show("��ѡ��Ҫ�鿴��ʷ��¼�Ľű��ļ�!", "�鿴��ʷ��¼", MessageBoxButtons.OK, MessageBoxIcon.Information);
                } 
            }
            else
            {
                MessageBox.Show("��ѡ��Ҫ�鿴��ʷ��¼�Ľű��ļ�!", "�鿴��ʷ��¼", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void popRight_Opening(object sender, CancelEventArgs e)
        {
            
        }

        private void treePh_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Return)
            {
                if (treePh.SelectedNode != null)
                    treePh_NodeMouseDoubleClick(this, new TreeNodeMouseClickEventArgs(treePh.SelectedNode, MouseButtons.Left, 2, 0, 0));
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        //�ű����
        private void btnCheck_Click(object sender, EventArgs e)
        {
            string strOut = "";
            try
            {
                strOut = ScriptCheck();
                if (strOut == null) return;
                strOut = strOut.TrimEnd('\n');
                if (strOut == "")
                {
                    MessageBox.Show("û�з��ִ���", "�����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show(strOut, "�����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //������ת��������
            string[] strLines = strOut.Split(new string[] { ":" }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < strLines.Length - 1; i++)
            {
                if (strLines[i].Trim() == "LuaExec.lua" && strLines[i + 1].Trim() != "")
                {
                    string strLineNumber = strLines[i + 1].Trim();
                    try
                    {
                        int nLineNumber = Int32.Parse(strLineNumber);
                        EditForm fe = this.ActiveMdiChild as EditForm;
                        fe.luaEditBox.MoveToLine(nLineNumber);
                        return;
                    }
                    catch (Exception ex)
                    {
                        ex.GetType();
                    }
                }
            }
        }

        //��齹�㴰�ڵĴ������
        public string ScriptCheck()
        {
            return ScriptCheck(this.ActiveMdiChild as EditForm);
        }

        //���ָ������Ĵ���
        public string ScriptCheck(string strCode)
        {
            string sCKDir = Path.Combine(Application.StartupPath,  @"Plugins\LuaCheck\");

            string sourceCode = strCode; // = DataBaseManager.GetDataBaseManager().GetContentAfterConvert(strCode);
            Helper.WriteStringToFile(sourceCode, Path.Combine(sCKDir, "tmp.lua"));
        
            if (File.Exists(Path.Combine(sCKDir, "tmp.lua")) == false)
            {
                //MessageBox.Show("������ʱ�ű�ʧ�ܣ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                throw (new Exception("������ʱ�ű�ʧ�ܣ�"));
            }

            //���������ļ�
            string strConfig = @"-- Settings
FILE_CLIENTPATH = '{0}'
FILE_LUAOUTPUT = false";
            string strClientPath = Helper.ClientPath.Replace("\\", "/");
            if (!strClientPath.EndsWith("/"))
            {
                strClientPath += "/";
            }
            strConfig = string.Format(strConfig, strClientPath);
            Helper.WriteStringToFile(strConfig, Path.Combine(sCKDir, "config.lh"));

            //��ȡ�û�������ȷ�������鹤��
            string strUserCfg = Helper.FileToString(Application.StartupPath + @"\Plugins\LuaCheck\_setting.cfg");
            string[] astrUserCfg = strUserCfg.Split(new string[] { "," }, StringSplitOptions.RemoveEmptyEntries);
            string strExeName = "";
            if (astrUserCfg.Length == 6 && astrUserCfg[5] == "�߼����")
            {
                strExeName = "ExecLua.exe";
            }
            else
            {
                strExeName = "luac5.1.exe";
            }

            //ִ�м�鹤��
            if (File.Exists(Path.Combine(sCKDir, strExeName)))
            {
                Process p = new Process();
                p.StartInfo.FileName = Path.Combine(sCKDir, strExeName);
                p.StartInfo.Arguments = "\"" + Path.Combine(sCKDir, @"tmp.lua") + "\"";
                p.StartInfo.WorkingDirectory = sCKDir;
                p.StartInfo.RedirectStandardOutput = true;
                p.StartInfo.RedirectStandardError = true;
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.CreateNoWindow = true;
                p.Start();

                StreamReader stream = p.StandardOutput;                
                StreamReader streamErr = p.StandardError;

                string strOut = "";
                while (!p.HasExited)
                {
                    strOut += stream.ReadLine() + "\n";
                    strOut += streamErr.ReadLine() + "\n";
                }
                strOut += stream.ReadToEnd();
                strOut += streamErr.ReadToEnd();
                p.WaitForExit();
                File.Delete(Path.Combine(sCKDir, @"tmp.lua"));
                return strOut;
            }
            else
            {
                //MessageBox.Show("��鹤�߲����ڣ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                throw (new Exception("��鹤�߲����ڣ�"));
            }
        }

        //���ָ�����ڵĴ������
        public string ScriptCheck(EditForm fe)
        {
            string strCode = "";
            if (fe != null)
            {
                if (fe.luaEditBox.Text != null)
                {
                    strCode = fe.luaEditBox.Text;
                }
            }
            if (strCode == "")
            {
                return null;
            }
            return ScriptCheck(strCode);
        }        

        private void bunToolCheck_Click(object sender, EventArgs e)
        {
            btnCheck_Click(this, new EventArgs());
        }

        private void cmdTreePhFolder_check_Click(object sender, EventArgs e)
        {
            TreeNode tn_folder = treePh.SelectedNode;

            if (tn_folder == null) return;
            List<string> idList = new List<string>();
            fillArrayListInIDFromNode(idList, tn_folder);
            int nErrorCount = 0;
            foreach(string strID in idList)
            {
                string strContent = DataBaseManager.GetDataBaseManager().GetScriptData(strID);
                string strOut = "";
                try
                {
                    strOut = ScriptCheck(strContent);
                    strOut = strOut.Trim().TrimEnd('\n');
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                if (strOut != "")             //���ִ���
                {
                    TreePhOpera.SetNodeColorByID(treePh, strID, Color.Red);
                    nErrorCount++;
                }
                else
                {
                    TreePhOpera.SetNodeColorByID(treePh, strID, Color.Black);
                }
            }
            MessageBox.Show("�����ϣ�" + ((nErrorCount > 0) ? ("����" + nErrorCount + "�������ļ�") : ("û�з��ִ���")),
                    "������",
                    MessageBoxButtons.OK,
                    ((nErrorCount > 0) ? MessageBoxIcon.Error : MessageBoxIcon.Information));
        }

        /// <summary>
        /// ����ls��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bClearLS_Click(object sender, EventArgs e)
        {
            return;
            ClearLSForm cForm = new ClearLSForm();
            cForm.ShowDialog();
        }

        private void btnToolFormat_Click(object sender, EventArgs e)
        {
            string strFileName = Path.Combine(Application.StartupPath, @"Plugins\LuaCheck\filter.lua");
            if (File.Exists(strFileName))
            {
                try
                {
                    this.m_lua.DoFile(strFileName);
                    LuaInterface.LuaFunction fun = this.m_lua.GetFunction("main");
                    if (fun != null)
                    {
                        fun.Call(sender, e);
                    }
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.ToString(), "����",
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void btnFormat_Click(object sender, EventArgs e)
        {
            btnToolFormat_Click(btnToolFormat, e);
        }

        private void buttonItem4_Click(object sender, EventArgs e)
        {
            NewsForm nf = new NewsForm();
            nf.ShowDialog();
        }

        private void buttonItem5_Click(object sender, EventArgs e)
        {
            HistoryTotalForm htf = new HistoryTotalForm(DataBaseManager.GetDataBaseManager().Connection);
            htf.Show();
        }

        /// <summary>
        /// ɾ���ϵ�
        /// </summary>
        /// <param name="fileName">�ϵ������ļ�</param>
        /// <param name="lineIndex">�ϵ������к�</param>
        private void RemoveBreakPoint(string fileName, int lineIndex)
        {
            foreach (EditForm editForm in this.MdiChildren)
            {
                if (editForm.FileName == fileName)
                {
                    editForm.RemoveBreakPoint(lineIndex);
                    break;
                }
            }
        }

        /// <summary>
        /// ����ı�
        /// </summary>
        /// <param name="text">�ı�����</param>
        private void PrintOutputText(string text)
        {
            outputBox.AppendText(string.Format("{0}\r\n", text));
        }

        /// <summary>
        /// ע��ϵ�
        /// </summary>
        private void RegistBreakPoint()
        {
            PrintOutputText("��ʼע��ϵ�...");

            foreach (DataGridViewRow currentRow in breakPointView.Rows)
            {
                bool enableBreakPoint = (bool)currentRow.Cells["EnableBreakPoint"].Value;
                string fileName = currentRow.Cells["BreakPointFileName"].Value as string;
                string lineIndex = currentRow.Cells["BreakPointLineIndex"].Value as string;
                string condition = currentRow.Cells["BreakPointCondition"].Value as string;

                if (enableBreakPoint)
                {
                    string message = null;
                    string returnMessage = null;

                    if (!string.IsNullOrEmpty(condition)) // ����������Ϣ�Ķϵ�
                    {
                        message = string.Format("add_break_point {0} {1} {2}", fileName, lineIndex.ToString(), condition);
                        returnMessage = string.Format("<ldb>add_break_point: {0},{1},{2}", fileName, lineIndex.ToString(), condition);
                        PrintOutputText(string.Format("ע��ϵ� ���� �ļ�����{0}���кţ�{1}��������{2}", 
                                                      fileName, lineIndex.ToString(), condition));

                    }
                    else
                    {
                        message = string.Format("add_break_point {0} {1}", fileName, lineIndex.ToString());
                        returnMessage = string.Format("<ldb>add_break_point: {0},{1}", fileName, lineIndex.ToString());
                        PrintOutputText(string.Format("ע��ϵ� ���� �ļ�����{0}���кţ�{1}", fileName, lineIndex.ToString()));
                    }

                    string receivedMessage = SendAndWaitMessage(message, returnMessage, false);

                    if (receivedMessage == null)
                    {
                        receivedMessage = "������Ϣ��ʱ...";
                        currentRow.Cells["BreakPointState"].Value = "������Ϣ��ʱ";
                        currentRow.Cells["BreakPointState"].Style.ForeColor = Color.Red;
                    }
                    else
                    {
                        currentRow.Cells["BreakPointState"].Value = "ע��ɹ�";
                        currentRow.Cells["BreakPointState"].Style.ForeColor = Color.Black;
                    }

                    PrintOutputText(receivedMessage);
                }
            }

            PrintOutputText("ע��ϵ����...");
        }

        /// <summary>
        /// ���Ͳ���ʼ������Ϣ
        /// </summary>
        /// <param name="message">���͵���Ϣ</param>
        /// <param name="returnMessage">�������ص���Ϣ</param>
        /// <param name="forHead">�Ƿ�������ָ��Ϣͷ</param>
        /// <returns>���յ�����Ϣ</returns>
        private string SendAndWaitMessage(string message, string returnMessage, bool forHead)
        {
            string receivedMessage = null;
            networkManager.SendUdpMessage(message);

            //���Լ��Σ������ʱ��Ϊ����ʧ��
            for (int i = 0; i < Helper.ReceiveMessageRetryTime; i++)
            {
                if (forHead)
                {
                    receivedMessage = networkManager.GetMessageForHead(returnMessage);
                }
                else
                {
                    receivedMessage = networkManager.GetMessage(returnMessage);
                }

                if (receivedMessage != null)
                {
                    break;
                }

                Thread.Sleep(Helper.ReceiveMessageDelay);
                // Application.DoEvents();
            }            

            return receivedMessage;
        }

        /// <summary>
        /// �ӵ�����Ϣ�л�ȡ��ת��Ϣ
        /// </summary>
        /// <param name="message">�ı���Ϣ</param>        
        /// <returns>������Ϣ</returns>
        private object[] GetJumpInfoFromMessage(string message)
        {
            object[] info = new object[3];
            int index = message.IndexOf(":");
            string tempString = message.Substring(index + 2);            
            string[] data = tempString.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
            info[0] = data[0];
            info[1] = int.Parse(data[1]);

            if (data.Length == 3)
            {
                info[2] = data[2];
            }
                      
            return info;
        }      
        
        /// <summary>
        /// �����������
        /// </summary>
        /// <returns>�������</returns>
        private Node CreateVarNode()
        {
            Node newNode = new Node();
            newNode.Cells.Add(new Cell());
            newNode.Cells.Add(new Cell());
            newNode.Cells.Add(new Cell());

            newNode.Cells[0].Tag = "varName";
            newNode.Cells[1].Tag = "varValue";
            newNode.Cells[2].Tag = "varFileName";
            newNode.Cells[3].Tag = "varState";            

            return newNode;
        }

        /// <summary>
        /// ˢ�±���ֵ
        /// </summary>
        private void RefreshVarValue()
        {
            PrintOutputText("��ʼˢ�±���ֵ...");

            lockVarValue = true; // ���ʱ�������޸ı���ֵ
            EditForm editForm = this.ActiveMdiChild as EditForm;

            foreach (Node node in watchTree.Nodes)
            {                
                string varName = node.Cells[0].Text;
                string fileName = node.Cells[2].Text;

                if (!string.IsNullOrEmpty(varName) && fileName == editForm.FileName)
                {
                    PrintOutputText(string.Format("��ȡ����ֵ ���� ��������{0}���ļ�����{1}", varName, fileName));
                    string message = null;

                    if (varName.Contains("(")) // ����Ƿ������������
                    {
                        message = string.Format("print_fun {0}", varName);
                    }
                    else
                    {
                        message = string.Format("print_var {0}", varName);
                    }

                    string receivedMessage = SendAndWaitMessage(message, string.Format("<ldb>print_var: {0}", varName), true);

                    if (receivedMessage == null)
                    {                        
                        node.Cells[3].Text = "������Ϣ��ʱ";
                    }
                    else if (receivedMessage == string.Format("<ldb>print_var: {0} failed!", varName))
                    {
                        node.Cells[1].Text = "��Ч����";
                        node.Cells[3].Text = "��ȡ�ɹ�";                        
                    }
                    else
                    {
                        string varNewValue = receivedMessage.Replace(string.Format("<ldb>print_var: {0} -> ", varName), "");

                        if (varNewValue.Contains("\n")) // ���зֽ����
                        {
                            node.Nodes.Clear();
                            string[] valueArray = varNewValue.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
                            node.Cells[1].Text = valueArray[0];

                            for (int i = 1; i < valueArray.Length; i++)
                            {
                                Node newNode = CreateVarNode();
                                string valueString = valueArray[i];
                                int index = valueString.IndexOf("=");

                                if (index >= 0)
                                {
                                    newNode.Cells[0].Text = valueString.Substring(0, index - 1);
                                    newNode.Cells[1].Text = valueString.Substring(index + 2);
                                }
                                else
                                {
                                    newNode.Cells[1].Text = valueString;
                                }

                                node.Nodes.Add(newNode);
                            }
                        }
                        else
                        {
                            node.Cells[1].Text = varNewValue;
                        }
                        
                        node.Cells[3].Text = "��ȡ�ɹ�";                        
                    }
                }    
            }

            // �����޸ı���ֵ
            lockVarValue = false; 
            PrintOutputText("ˢ�±���ֵ���...");
        }

        /// <summary>
        /// �༭����ȡ��
        /// </summary>
        /// <param name="editForm">�༭����</param>
        /// <param name="e">ȡ�ʲ���</param>
        private void EditFormWordMouseHover(EditForm editForm, ref WordMouseEventArgs e)
        {
            string fileName = editForm.FileName;
            string varName = e.Word.Text;
            editForm.luaEditBox.CloseTipText();

            if (startDebug && enableFetchWord && mouseFetchWord && !string.IsNullOrEmpty(varName))
            {
                // ֻ���պϷ������ĵ���
                Regex regex = new Regex(@"[A-Za-z0-9_]+");
               
                if (regex.IsMatch(varName))
                {
                    if (ProcessFileName(editForm.FileName) == currentDebugFile)
                    {
                        string message = string.Format("print_var {0}", varName);
                        string receivedMessage = SendAndWaitMessage(message, string.Format("<ldb>print_var: {0}", varName), true);
                        string tipText = null;

                        if (receivedMessage != null)
                        {
                            if (receivedMessage == string.Format("<ldb>print_var: {0} failed!", varName))
                            {
                                tipText = "��Ч����";
                            }
                            else
                            {
                                receivedMessage = receivedMessage.Replace(string.Format("<ldb>print_var: {0} -> ", varName), "");
                                receivedMessage = receivedMessage.Replace("<", "��").Replace(">", "��"); // ����<��>���ŵ�ʱ����bug������ʱ�滻��
                                receivedMessage = receivedMessage.Replace("\n", "<BR>"); // �滻���з�
                                tipText = string.Format("<b>{0}</b><BR>{1}", varName, receivedMessage);
                            }                           
                        }
                        else
                        {
                            tipText = "������Ϣ��ʱ";
                        }

                        TextPoint location = new TextPoint(e.Word.Column + e.Word.Text.Length, e.Word.Row.Index);
                        editForm.luaEditBox.SetTipText(tipText, location);
                    }
                }
            }
        }

        /// <summary>
        /// �༭������Ӷϵ�
        /// </summary>
        /// <param name="editForm">�༭����</param>
        /// <param name="row">�ϵ�������</param>
        private void EditFormBreakPointAdded(EditForm editForm, Fireball.Syntax.Row row)
        {
            bool exist = false;
            int currentLineIndex = -1;
            string fileName = null;

            foreach (DataGridViewRow breakPointRow in breakPointView.Rows)
            {
                fileName = breakPointRow.Cells["BreakPointFileName"].Value as string;
                string lineIndex = breakPointRow.Cells["BreakPointLineIndex"].Value as string;
                currentLineIndex = row.Index + 1;

                if (fileName == editForm.FileName && lineIndex == currentLineIndex.ToString())
                {
                    exist = true;
                    break;
                }
            }

            if (!exist)
            {
                fileName = editForm.FileName;
                currentLineIndex = row.Index + 1;                
                breakPointView.Rows.Add(1);
                DataGridViewRow newRow = breakPointView.Rows[breakPointView.Rows.Count - 1];
                newRow.Cells["EnableBreakPoint"].Value = true;
                newRow.Cells["BreakPointFileName"].Value = fileName;
                newRow.Cells["BreakPointLineIndex"].Value = currentLineIndex.ToString();

                // ��̬ע��ϵ�
                if (startDebug)
                {
                    string message = string.Format("add_break_point {0} {1}", fileName, currentLineIndex.ToString());
                    PrintOutputText(string.Format("ע��ϵ� ���� �ļ�����{0}���кţ�{1}", fileName, currentLineIndex.ToString()));

                    string receivedMessage = SendAndWaitMessage(message, string.Format("<ldb>add_break_point: {0},{1}", fileName, currentLineIndex), false);

                    if (receivedMessage != null)
                    {
                        newRow.Cells["BreakPointState"].Value = "ע��ɹ�";
                        newRow.Cells["BreakPointState"].Style.ForeColor = Color.Black;
                    }
                    else
                    {
                        receivedMessage = "������Ϣ��ʱ...";
                        newRow.Cells["BreakPointState"].Value = "������Ϣ��ʱ";
                        newRow.Cells["BreakPointState"].Style.ForeColor = Color.Red;
                    }

                    PrintOutputText(receivedMessage);
                }
            }
        }

        /// <summary>
        /// �༭����ɾ���ϵ�
        /// </summary>
        /// <param name="editForm">�༭����</param>
        /// <param name="row">�ϵ�������</param>
        private void EditFormBreakPointRemoved(EditForm editForm, Fireball.Syntax.Row row)
        {
            int removeRowIndex = -1;
            string fileName = null;
            int currentLineIndex = row.Index + 1;

            foreach (DataGridViewRow breakPointRow in breakPointView.Rows)
            {
                fileName = breakPointRow.Cells["BreakPointFileName"].Value as string;
                string lineIndex = breakPointRow.Cells["BreakPointLineIndex"].Value as string;

                if (fileName == editForm.FileName && lineIndex == currentLineIndex.ToString())
                {
                    removeRowIndex = breakPointRow.Index;
                    break;
                }
            }

            if (removeRowIndex != -1)
            {
                breakPointView.Rows.RemoveAt(removeRowIndex);
                
                // ��̬ȡ��ע��ϵ�
                if (startDebug)
                {
                    string message = string.Format("delete_break_point {0} {1}", fileName, currentLineIndex.ToString());
                    PrintOutputText(string.Format("ȡ��ע��ϵ� ���� �ļ�����{0}���кţ�{1}", fileName, currentLineIndex.ToString()));
                    string receivedMessage = SendAndWaitMessage(message, string.Format("<ldb>delete_break_point: {0},{1}", 
                                                                                       fileName, currentLineIndex.ToString()), false);

                    if (receivedMessage != null)
                    {
                        receivedMessage = "ȡ��ע��ϵ�ɹ�...";
                    }
                    else
                    {
                        receivedMessage = "������Ϣ��ʱ...";
                    }

                    PrintOutputText(receivedMessage);
                }
            }            
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="stepType">��������</param>
        /// <param name="stepMessage">������Ϣ</param>
        private void StepDebug(string stepType, string stepMessage)
        {
            PrintOutputText(string.Format("��ʼ{0}...", stepType));
            enableFetchWord = false; // �ر�ȡ�ʹ��ܣ���ֹ���յ�ȡ�ʵ���Ϣ

            // ��������״̬
            string message = stepMessage;
            networkManager.SendUdpMessage(message);
            
            PrintOutputText(string.Format("��ʼ�ȴ�{0}��Ϣ...", stepType));          
            StartDebugWaitTimer(Helper.BreakPointWaitCircle, DebugState.WaitStepDebugActivated);
        }

        /// <summary>
        /// ���е���һ���ϵ�
        /// </summary>
        private void ContinueDebug()
        {
            PrintOutputText("��ʼ���е���һ���ϵ�...");
            
            // �����ǰ���յ���������Ϣ
            networkManager.ClearReceivedMessageList();

            // ��������״̬
            string message = "continue_debug";
            networkManager.SendUdpMessage(message);

            // �������ջ��Ϣ
            callStackView.Rows.Clear();

            // ������ǰ�ĵ����б��
            EditForm editForm = tabStrip1.SelectedTab.AttachedControl as EditForm;
            editForm.luaEditBox.ClearDebugMark();
            editForm.luaEditBox.Refresh();

            // ȥ����ǰ�����ļ�����Ϣ
            currentDebugFile = null;

            // �ȴ�����˴����ϵ�
            PrintOutputText("��ʼ�ȴ������ϵ�...");
            StartDebugWaitTimer(Helper.BreakPointWaitCircle, DebugState.WaitBreakPointActivated);
        }

        /// <summary>
        /// ���½ű�������
        /// </summary>
        /// <param name="font">����</param>
        private void UpdateScriptFont(Font font)
        {
            foreach (EditForm editForm in this.MdiChildren)
            {
                editForm.luaEditBox.ScriptFont = Helper.ScriptFont;
            }
        }

        /// <summary>
        /// ���½ű��������ɫ
        /// </summary>
        /// <param name="color">��ɫ</param>
        private void UpdateScriptForeColor(Color color)
        {
            foreach (EditForm editForm in this.MdiChildren)
            {
                editForm.luaEditBox.ScriptForeColor = Helper.ScriptForeColor;
            }
        }        

        /// <summary>
        /// ȡ���ҽӵ����ļ�
        /// </summary>
        private void DetachDebugFile()
        {
            string filePath = Path.Combine(Helper.ClientPath, @"scripts\server_main.lua");
            string content = Helper.FileToString(filePath);

            if (!string.IsNullOrEmpty(content))
            {
                StringBuilder newContent = new StringBuilder();
                string[] lines = content.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
                bool findLine = false;
                bool ignoreLine = false;
                bool requireWriteFile = false;

                foreach (string s in lines)
                {
                    string line = s.Trim(new char[] { ' ', '\t' });

                    if (findLine)
                    {
                        if (line == "Include('scripts/ldb.lua')")
                        {
                            ignoreLine = true;
                            requireWriteFile = true;
                        }
                    }
                    else
                    {
                        if (line == "function OnGameServerConnected(nConnIndex)")
                        {
                            findLine = true;
                        }
                    }

                    if (!ignoreLine)
                    {
                        newContent.AppendLine(s);
                    }
                    else
                    {
                        ignoreLine = false;
                    }
                }

                if (requireWriteFile)
                {
                    Helper.WriteStringToFile(newContent.ToString(), filePath);
                }
            }
        }

        /// <summary>
        /// �������Եȴ���ʱ��
        /// </summary>
        /// <param name="interval">��ʱ�����</param>
        /// <param name="debugState">����״̬</param>
        private void StartDebugWaitTimer(int interval, DebugState debugState)
        {
            this.debugState = debugState;
            debugWaitTimer.Interval = interval;
            debugWaitTimer.Start();
        }

        /// <summary>
        /// �رյ��Եȴ���ʱ��
        /// </summary>
        private void StopDebugWaitTimer()
        {
            if (debugWaitTimer != null)
            {
                debugState = DebugState.None;
                debugWaitTimer.Stop();
            }
        }

        /// <summary>
        /// ��ʼ���Խű�
        /// </summary>
        private void StartDebug(DebugMode currentDebugMode)
        {
            // �������ļ��Ƿ��Ѿ�����
            if (!CheckDebugFileReady())
            {
                MessageBox.Show("����ȷ�������ļ����Ѿ���ȷ�ҽӣ�", "��ʼ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                SettingForm sForm = new SettingForm();
                sForm.CurrentUpdateScriptFont = new SettingForm.UpdateScriptFont(UpdateScriptFont);
                sForm.CurrentUpdateScriptForeColor = new SettingForm.UpdateScriptForeColor(UpdateScriptForeColor);
                sForm.ShowDialog();
                return;
            }

            // ������ģʽ�Ƿ�����
            if (debugMode != DebugMode.None && debugMode != currentDebugMode)
            {
                switch (currentDebugMode)
                {
                    case DebugMode.Client:
                        {
                            if (Helper.CheckProcessExist("JX3Client", false))
                            {
                                MessageBox.Show("�ӷ���˽ű��л����ͻ��˽ű�����ʱ����Ҫ�޸Ĺҽ��ļ������ȹر�JX3Client.exe��", "��ʼ����",
                                                MessageBoxButtons.OK, MessageBoxIcon.Information);
                                return;
                            }

                            break;
                        }
                    case DebugMode.Server:
                        {
                            if  (Helper.CheckProcessExist("SO3GameServer", false))
                            {
                                MessageBox.Show("�ӿͻ��˽ű��л�������˽ű�����ʱ����Ҫ�޸Ĺҽ��ļ������ȹر�JX3Client.exe��", "��ʼ����",
                                                MessageBoxButtons.OK, MessageBoxIcon.Information);
                                return;
                            }

                            break;
                        }
                }
            }

            // �ҽӵ����ļ�
            switch (currentDebugMode)
            {
                case DebugMode.Client:
                    {
                        DetachServerDebugFile();

                        if (!AttachClientDebugFile())
                        {
                            return;
                        }

                        break;
                    }
                case DebugMode.Server:
                    {
                        DetachClientDebugFile();

                        if (!AttachServerDebugFile())
                        {
                            return;
                        }

                        break;
                    }
            }

            debugMode = currentDebugMode;

            PrintOutputText("��ʼ���Խű�...");
            networkManager = NetworkManager.GetNetworkManager();
            networkManager.BeginReceiveUdpMessage();
            PrintOutputText("��ʼ�ȴ���Ӧ��������...");
            StartDebugWaitTimer(Helper.BreakPointWaitCircle, DebugState.WaitStartDebugActivated);
        }

        /// <summary>
        /// ��ʼ���Է���˽ű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bStartDebug_Click(object sender, EventArgs e)
        {            
            StartDebug(DebugMode.Server);            
        }

        /// <summary>
        /// ֹͣ���Խű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bStopDebug_Click(object sender, EventArgs e)
        {
            // �رյ���״̬
            PrintOutputText("ֹͣ���Խű�...");
            string message = "stop_debug";
            string receivedMessage = SendAndWaitMessage(message, "<ldb>stop_debug: success!", false);

            while (receivedMessage == null)
            {
                if (MessageBox.Show("����ȷ����Ϣ��ʱ���Ƿ����·��͵�����Ϣ��", "ֹͣ����",
                                    MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
                {
                    receivedMessage = SendAndWaitMessage(message, "<ldb>stop_debug: success!", false);
                }
                else
                {
                    break;
                }
            }

            if (receivedMessage != null)
            {
                PrintOutputText("ֹͣ���Խű��ɹ�...");

                // �����Ϣ����
                networkManager.ClearReceivedMessageList();
            }

            //  �л�״̬
            startDebug = false;
            bStartDebug.Enabled = true;
            bStartDebugClient.Enabled = true;
            bStopDebug.Enabled = false;
            currentDebugFile = null;

            foreach (ButtonItem buttonItem in debugButtonList)
            {
                buttonItem.Enabled = false;
            }

            // ���������Ϣ
            watchTree.Nodes.Clear();

            // �������ջ��Ϣ
            callStackView.Rows.Clear();

            // �������Ա��
            foreach (EditForm editForm in this.MdiChildren)
            {
                editForm.Text = editForm.Text.Replace("[������]", "").Trim();
                editForm.luaEditBox.ReadOnly = false;
                editForm.luaEditBox.ClearDebugMark();
                editForm.Refresh();
            }

            // �ص����Լ�ʱ��
            StopDebugWaitTimer();            
        }

        /// <summary>
        /// ɾ���ϵ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDeleteBreakPoint_Click(object sender, EventArgs e)
        {
            if (breakPointView.SelectedCells != null && breakPointView.SelectedCells.Count> 0)
            {
                DataGridViewCell currentCell = breakPointView.SelectedCells[0];

                if (currentCell.RowIndex >= 0) // ������ǲ���HeaderRow
                {
                    DataGridViewRow currentRow = currentCell.OwningRow;
                    string fileName = currentRow.Cells["BreakPointFileName"].Value as string;
                    int lineIndex = int.Parse(currentRow.Cells["BreakPointLineIndex"].Value as string);
                    RemoveBreakPoint(fileName, lineIndex - 1);
                }
                else
                {
                    MessageBox.Show("����ѡ��Ҫɾ���Ķϵ㣡", "ɾ���ϵ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫɾ���Ķϵ㣡", "ɾ���ϵ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ɾ�����жϵ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDeleteAllBreakPoint_Click(object sender, EventArgs e)
        {
            // ��Ϊ��ȥ���ϵ��ʱ��breakPointView��������Ҳ�Ƕ�̬�仯�ģ����Բ���ֱ����ѭ���з���
            List<string> fileNameList = new List<string>();
            List<int> lineIndexList = new List<int>();

            foreach (DataGridViewRow currentRow in breakPointView.Rows)
            {
                string fileName = currentRow.Cells["BreakPointFileName"].Value as string;
                int lineIndex = int.Parse(currentRow.Cells["BreakPointLineIndex"].Value as string);                
                fileNameList.Add(fileName);
                lineIndexList.Add(lineIndex - 1);
            }

            for (int i = 0; i < fileNameList.Count; i++)
            {
                RemoveBreakPoint(fileNameList[i], lineIndexList[i]);
            }
        }

        /// <summary>
        /// ˢ�±���ֵ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRefreshVar_Click(object sender, EventArgs e)
        {
            RefreshVarValue();
        }

        /// <summary>
        /// ɾ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDeleteVar_Click(object sender, EventArgs e)
        {
            Node currentNode = watchTree.SelectedNode;

            if (currentNode != null && currentNode.Level == 0)
            {
                currentNode.Remove();
            }
        }

        /// <summary>
        /// ɾ�����б���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDeleteAllVar_Click(object sender, EventArgs e)
        {
            watchTree.Nodes.Clear();            
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bStepDebug_Click(object sender, EventArgs e)
        {
            StepDebug("��������", "step_into");            
        }

        /// <summary>
        /// ���е���һ���ϵ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bContinueDebug_Click(object sender, EventArgs e)
        {
            ContinueDebug();
        }

        /// <summary>
        /// ��ӱ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddVar_Click(object sender, EventArgs e)
        {
            if (tabStrip1.SelectedTab != null)
            {
                EditForm editForm = tabStrip1.SelectedTab.AttachedControl as EditForm;
                string fileName = editForm.FileName;

                Node newNode = CreateVarNode();
                newNode.Cells[2].Text = fileName;
                watchTree.Nodes.Add(newNode);
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ��ӱ����Ľű���", "��ӱ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDeleteOutput_Click(object sender, EventArgs e)
        {
            outputBox.Clear();
        }

        /// <summary>
        /// ����/ȡ���ϵ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bSwitchBreakPoint_Click(object sender, EventArgs e)
        {
            EditForm editForm = this.ActiveMdiChild as EditForm;
 
            if (editForm != null)
            {
                int currentLineIndex = editForm.luaEditBox.SelectedLineNumber;

                if (currentLineIndex >= 0)
                {
                    List<int> breakPointIndexList = editForm.luaEditBox.BreakPointIndexList;
                    
                    if (breakPointIndexList.Contains(currentLineIndex + 1))
                    {
                        editForm.luaEditBox.SetBreakPoint(currentLineIndex, false);
                    }
                    else
                    {
                        editForm.luaEditBox.SetBreakPoint(currentLineIndex, true);
                    }                    
                }
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bStepOver_Click(object sender, EventArgs e)
        {
            StepDebug("��������", "step_over");
        }

        /// <summary>
        /// ������İ����¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void MainForm_KeyDown(object sender, KeyEventArgs e)
        {
            EditForm_KeyDown(sender, e);
        }

        /// <summary>
        /// ���˫������ջ��Ϣ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void callStackView_CellMouseDoubleClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.RowIndex >= 0)
            {
                DataGridViewRow currentRow = callStackView.Rows[e.RowIndex];

                string fileName = currentRow.Cells["CallStackFileName"].Value.ToString();
                string lineIndex = currentRow.Cells["CallStackLineIndex"].Value.ToString();

                EditForm editForm = OpenFileInEditor(fileName, true);

                if (editForm != null)
                {
                    editForm.luaEditBox.MoveToLine(int.Parse(lineIndex) - 1);
                }
            }
        }

        /// <summary>
        /// ����/�رշ���˵������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bEnableServerOutput_Click(object sender, EventArgs e)
        {
            enableServerDebugOutput = !enableServerDebugOutput;
            string message = null;

            if (enableServerDebugOutput)
            {
                message = "enable_debug_output";
                bEnableServerDebugOutput.Text = "�رշ���˵������";
                PrintOutputText("��������˵������...");
            }
            else
            {
                message = "disable_debug_output";
                bEnableServerDebugOutput.Text = "��������˵������";
                PrintOutputText("�رշ���˵������...");
            }

            networkManager.SendUdpMessage(message);
        }

        /// <summary>
        /// ��ʾ/����ͨ�ù�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bViewGeneralToolBar_Click(object sender, EventArgs e)
        {
            bViewGeneralToolBar.Checked = !bViewGeneralToolBar.Checked;
            barGeneral.Visible = bViewGeneralToolBar.Checked;
        }

        /// <summary>
        /// ��ʾ/���ص��Թ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bViewDebugToolBar_Click(object sender, EventArgs e)
        {
            bViewDebugToolBar.Checked = !bViewDebugToolBar.Checked;
            barDebug.Visible = bViewDebugToolBar.Checked;
        }

        /// <summary>
        /// ����/�ر����ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bMouseGetWord_Click(object sender, EventArgs e)
        {
            mouseFetchWord = !mouseFetchWord;

            if (mouseFetchWord)
            {
                bMouseGetWord.Text = "�ر����ȡ��";
            }
            else
            {
                bMouseGetWord.Text = "�������ȡ��";
                
                // �ر����ȡ�ʵ�Tip
                EditForm editForm = this.ActiveMdiChild as EditForm;

                if (editForm != null)
                {
                    editForm.luaEditBox.CloseTipText();
                }
            }
        }

        /// <summary>
        /// ����ر�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // ȡ���ű�����
            if (networkManager != null)
            {
                networkManager.Uninit();
            }
            
            // ���汾�ؽű�
            if (CheckLocalFileTreeChanged())
            {                
                if (MessageBox.Show("�Ƿ񱣴�Ա����ļ���������Ϣ��", "��������", 
                                    MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
                {
                    SaveLocalFileList();
                }
            }
        }

        /// <summary>
        /// ���ر����ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bLoadLocalFile_Click(object sender, EventArgs e)
        {            
            AddLocalFile(Helper.ClientPath);
        }

        /// <summary>
        /// �Ƴ�����Ŀ¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRemoveLocalFolder_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treePh.SelectedNode;

            if (currentNode != null)
            {
                if (MessageBox.Show("ȷ��Ҫ�ڱ༭�����Ƴ���ǰĿ¼��", "�Ƴ�Ŀ¼",
                                    MessageBoxButtons.OKCancel, MessageBoxIcon.Information) == DialogResult.OK)
                {
                    currentNode.Remove();
                }
            }
        }

        /// <summary>
        /// ����ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddLocalFile_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treePh.SelectedNode;

            if (currentNode != null)
            {
                string folderName = currentNode.FullPath.Replace("�����ļ�", "").TrimStart(new char[] { '\\' });
                folderName = Path.Combine(Helper.ClientPath, folderName);

                AddLocalFile(folderName);
            }            
        }

        /// <summary>
        /// �Ƴ������ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRemoveLocalFile_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treePh.SelectedNode;

            if (currentNode != null)
            {
                if (MessageBox.Show("ȷ��Ҫ�ڱ༭�����Ƴ���ǰ�ļ���", "�Ƴ��ļ�",
                                    MessageBoxButtons.OKCancel, MessageBoxIcon.Information) == DialogResult.OK)
                {
                    currentNode.Remove();
                }
            }
        }

        /// <summary>
        /// ���Կͻ��˽ű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bStartDebugClient_Click(object sender, EventArgs e)
        {
            StartDebug(DebugMode.Client);
        }

        /// <summary>
        /// ���Ŀ¼�µ������ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddAllLocalFile_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treePh.SelectedNode;

            if (currentNode != null)
            {
                string folderName = currentNode.FullPath.Replace("�����ļ�", "").TrimStart(new char[] { '\\' });
                folderName = Path.Combine(Helper.ClientPath, folderName);

                DirectoryInfo di = new DirectoryInfo(folderName);

                if (di.Exists)
                {
                    foreach (FileInfo fi in di.GetFiles())
                    {
                        if (fi.Extension == ".lua")
                        {
                            AddLocalFileNode(fi.FullName);
                        }
                    }
                }                
            } 
        }

        /// <summary>
        /// �����۲�����ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void watchTree_AfterCellEdit(object sender, DevComponents.AdvTree.CellEditEventArgs e)
        {
            Node currentNode = e.Cell.Parent;
            EditForm editForm = this.ActiveMdiChild as EditForm;
            string varName = currentNode.Cells[0].Text;
            string varValue = currentNode.Cells[1].Text;
            
            switch (e.Cell.Tag.ToString())
            {
                case "varName":
                    {
                        varName = e.NewText;
                        break;
                    }
                case "varValue":
                    {
                        varValue = e.NewText;
                        break;
                    }
                default:
                    {
                        break;
                    }
            }

            if (!string.IsNullOrEmpty(varName) && editForm != null && startDebug)
            {
                switch (e.Cell.Tag.ToString())
                {
                    case "varName": // �ı�����
                        {
                            RefreshVarValue();

                            break;
                        }
                    case "varValue": // �ı���ֵ
                        {
                            if (!lockVarValue) // ������ˢ�±���ֵ������ͻ
                            {
                                if (varName.Contains("."))
                                {
                                    MessageBox.Show("����ֱ���޸Ķ�������ԣ�", "�޸ı���", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                }
                                else
                                {
                                    PrintOutputText(string.Format("��ʼ�޸ı���ֵ ���� {0} = {1}", varName, varValue));
                                    string message = string.Format("set_var {0} {1}", varName, varValue);
                                    string receivedMessage = SendAndWaitMessage(message, string.Format("<ldb>set_var: {0}", varName), true);

                                    if (receivedMessage != null)
                                    {
                                        if (receivedMessage == string.Format("<ldb>set_var: {0} success!", varName))
                                        {
                                            currentNode.Cells[3].Text = "�޸ĳɹ�";                                            
                                            PrintOutputText("����ֵ�޸ĳɹ�...");
                                        }
                                        else if (receivedMessage == string.Format("<ldb>set_var: {0} failed!", varName))
                                        {
                                            currentNode.Cells[3].Text = "�޸�ʧ��";                                            
                                            PrintOutputText("����ֵ�޸�ʧ��...");
                                        }
                                    }
                                    else
                                    {
                                        currentNode.Cells[3].Text = "������Ϣ��ʱ";                                        
                                        PrintOutputText("������Ϣ��ʱ...");
                                    }
                                }
                            }

                            break;
                        }
                    default:
                        {
                            MessageBox.Show("ֻ�����޸ı������ͱ���ֵ��", "�޸ı���",
                                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                            e.Cancel = true;
                            
                            break;
                        }
                }
            }          
        }

        /// <summary>
        /// �ı�����۲����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void watchTree_DragDrop(object sender, DragEventArgs e)
        {
            string varName = e.Data.GetData(Type.GetType("System.String")) as string;

            EditForm editForm = this.ActiveMdiChild as EditForm;
            string fileName = editForm.FileName;

            Node newNode = CreateVarNode();

            newNode.Cells[0].Text = varName;
            newNode.Cells[2].Text = fileName;

            watchTree.Nodes.Add(newNode);

            // ˢ��һ�α�������
            if (startDebug)
            {
                RefreshVarValue();
            }
        }

        /// <summary>
        /// �ı�����۲����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void watchTree_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(Type.GetType("System.String")))
            {
                e.Effect = DragDropEffects.Move;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        /// <summary>
        /// ���ر���Ŀ¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bLoadLocalFolder_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog();
            folderBrowserDialog.Description = "��ѡ��Ҫ���ؽű����ڵ�Ŀ¼";
            folderBrowserDialog.SelectedPath = Helper.ClientPath;

            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                string folderName = folderBrowserDialog.SelectedPath;

                DirectoryInfo di = new DirectoryInfo(folderName);

                if (di.Exists)
                {
                    foreach (FileInfo fi in di.GetFiles())
                    {
                        if (fi.Extension == ".lua")
                        {
                            AddLocalFileNode(fi.FullName);
                        }
                    }
                }
            } 
        }

        /// <summary>
        /// ȡ���ҽӽű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDetachDebugFile_Click(object sender, EventArgs e)
        {
            DetachServerDebugFile();
            DetachClientDebugFile();

            MessageBox.Show("ȡ���ҽӵ��Խű���ɣ�", "ȡ���ҽӽű�",
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }

}