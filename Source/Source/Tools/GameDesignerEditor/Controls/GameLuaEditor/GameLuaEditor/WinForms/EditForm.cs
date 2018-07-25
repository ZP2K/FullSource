using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Reflection;

using Fireball.Syntax;
using Fireball.Windows.Forms.CodeEditor;

namespace GameLuaEditor
{
    public partial class EditForm : Form
    {
        private string m_id = ""; // ���ݿ��Ψһ��ʶ
        private string m_code = ""; // �����еĴ���
        private string m_filename = ""; // �ļ�ȫ���������·����
        private bool isLocked = false; // ��¼��ǰ�༭�����Ƿ񱻲����߱�������
        private Word lastHoveredWord = null; // ��һ�β�ѯ�ĵ���
        private IntelligentSenseManager intelligentSenseManager = IntelligentSenseManager.GetIntelligentSenseManager(); // ���ܸ�֪���ݹ�����

        // ����ϵ���صĻص�
        public delegate void BreakPointAdded(EditForm editForm, Row row);
        public delegate void BreakPointRemoved(EditForm editForm, Row row);
        private BreakPointAdded handleBreakPointAdded;
        private BreakPointRemoved handleBreakPointRemoved;

        // ����ȡ�ʵĻص�
        public delegate void WordMouseHover(EditForm editForm, ref WordMouseEventArgs e);
        private WordMouseHover handleWordMouseHover;

        // �ű�������
        private readonly string scriptType; // �ű�����

        /// <summary>
        /// ȡ�ʵĻص�
        /// </summary>
        public WordMouseHover HandleWordMouseHover
        {
            set
            {
                handleWordMouseHover = value;
            }

            get
            {
                return handleWordMouseHover;
            }
        }

        /// <summary>
        /// ��Ӷϵ�Ļص�
        /// </summary>
        public BreakPointAdded HandleBreakPointAdded
        {
            set
            {
                handleBreakPointAdded = value;
            }

            get
            {
                return handleBreakPointAdded;
            }
        }

        /// <summary>
        /// ɾ���ϵ�Ļص�
        /// </summary>
        public BreakPointRemoved HandleBreakPointRemoved
        {
            set
            {
                handleBreakPointRemoved = value;
            }

            get
            {
                return handleBreakPointRemoved;
            }
        }

        /// <summary>
        /// �ϵ��к�����
        /// </summary>
        public List<int> BreakPointIndexList
        {
            get
            {
                return luaEditBox.BreakPointIndexList;
            }
        }

        /// <summary>
        /// �ű������Ƿ�ı�
        /// </summary>
        public bool IsChanged
        {
            get 
            { 
                return this.ScriptCode != this.luaEditBox.Text; 
            }
        }

        /// <summary>
        /// �ű��ļ�·��
        /// </summary>
        public string FileName
        {
            get 
            { 
                return m_filename; 
            }

            set 
            { 
                this.m_filename = value;
                string strText = m_filename;

                if (strText.IndexOf('\\') != -1)
                {
                    strText = strText.Substring(strText.LastIndexOf('\\') + 1);
                }

                this.Text = strText;
            }
        }

        /// <summary>
        /// ��ǰѡ�е��кţ���0��ʼ
        /// </summary>
        public int SelectedLineIndex
        {
            get
            {
                return luaEditBox.SelectedLineNumber;
            }
        }

        /// <summary>
        /// ��ǰ�ű��ļ���ID
        /// </summary>
        public string ID
        {
            get
            {
                return m_id;
            }

            set 
            { 
                m_id = value; 
            }
        }

        /// <summary>
        /// ��ֵ������both�༭����and���棬 ��ȡ��ֻȡ����
        /// </summary>
        public string ScriptCode
        {
            get 
            { 
                return m_code; 
            }

            set 
            { 
                m_code = value; 
                this.luaEditBox.Text = value; 
            }
        }

        /// <summary>
        /// ��ȡ�༭��������
        /// </summary>
        public string NewScriptCode
        {
            get 
            { 
                return this.luaEditBox.Text; 
            }
        }

        /// <summary>
        /// �Ҽ��˵�
        /// </summary>
        public ContextMenuStrip EditorContextMenu
        {
            get 
            { 
                return luaEditBox.ContextMenuStrip; 
            }

            set 
            { 
                luaEditBox.ContextMenuStrip = value; 
            }
        }

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="scriptType">�ű�����</param>
        public EditForm(string scriptType)
        {
            this.scriptType = scriptType;
            InitializeComponent();
            Init();   
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            luaEditBox.ForWho = "GameLuaEditor";
            LuaRuntime.LuaInitEx();

            // ��ʼ�������
            string strLuaFile = Path.Combine(Application.StartupPath, @"Plugins\LuaCheck\IntelliSense.lua");            

            if (File.Exists(strLuaFile))
            {
                StreamReader sr = new StreamReader(strLuaFile, Encoding.GetEncoding("gb2312"));
                string strOut = "";
                bool bret = LuaRuntime.LuaInit(sr.ReadToEnd(), ref strOut);
                sr.Close();
            }                       
        }

        /// <summary>
        /// ȥ���ϵ�
        /// </summary>
        /// <param name="lineIndex">�ϵ����ڵ��к�</param>
        public void RemoveBreakPoint(int lineIndex)
        {
            luaEditBox.SetBreakPoint(lineIndex, false);
        }

        /// <summary>
        /// �ű����ݸı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void luaEdit1_OnChange(object sender, EventArgs e)
        {
            string scriptName = m_filename;

            if (scriptName.IndexOf('\\') != -1)
            {
                scriptName = scriptName.Substring(scriptName.LastIndexOf('\\') + 1);
            }

            if (scriptType == "localFile") // �����ļ�������������
            {
                this.Text = scriptName + "*";
                return;
            }

            if (isLocked) // �Ѿ����ˣ�û�˻��ٸĵ����������ˣ����Լ���
            {
                return;
            }

            if (IsChanged) // ��ĸ���
            {                
                DataBaseManager databaseManager = DataBaseManager.GetDataBaseManager();
                string strServerContent = databaseManager.GetScriptData(this.m_id);

                if (strServerContent == null)
                {
                    MessageBox.Show("У����������ļ�ʧ�ܣ����ܷ��������ļ���ɾ����", "�����ű�", 
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                    this.luaEditBox.Text = "";
                    this.luaEditBox.Text = this.ScriptCode;

                    return;
                }

                if (strServerContent != this.ScriptCode)
                {
                    MessageBox.Show("�������˵��ļ��ѱ��޸Ĺ���Ҫ���¼����������ݣ�", "�����ű�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    this.ScriptCode = strServerContent;
                }

                // ����ǰ�Ƚ��� ������Լ��������������ˣ��ٴ������༭���Խ��Լ���ǰ�ӵ�����ǰ�����Լ�����unlock���˵���������֤��
                databaseManager.UnLockScript(this.ID);

                // ��ʽ����
                bool bret = databaseManager.LockScript(this.m_id);

                if (bret)
                {
                    // �����ɹ�����ô���޸ı�־���ļ�����                    
                    this.Text = scriptName + "*";
                    this.isLocked = true;
                }
                else
                {
                    //����ʧ�ܣ�˵������������(����LockScript�а���������棬���ҾͲ��ظ���������)
                    this.luaEditBox.Text = this.ScriptCode;
                }
            }
        }

        public void Reload()
        {
            if(IsChanged)
            {
                throw (new Exception("�ļ��ı��reload��Ч")); 
            }
            DataBaseManager dbm = DataBaseManager.GetDataBaseManager();
            string strServerContent = dbm.GetScriptData(this.m_id);
            if(strServerContent != null)
            {
                this.ScriptCode = strServerContent;
            }
        }

        /// <summary>
        /// �༭���ر�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void frmEdit_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (IsChanged)
            {
                if (MessageBox.Show(string.Format(" �����Ѹı��Ƿ񱣴棿", m_filename), "����ű�", 
                                    MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question) == DialogResult.Yes)
                {
                    // ����ǰ���
                    MainForm mainForm = this.MdiParent as MainForm;
                    string strOut = mainForm.ScriptCheck(luaEditBox.Text);

                    if (strOut != null)
                    {
                        strOut = strOut.TrimEnd('\n');
                    }                        

                    if (!string.IsNullOrEmpty(strOut))
                    {
                        MessageBox.Show(string.Format("{0} �ű��а�������\r\n{1}", m_filename, strOut), "�ű����",
                                        MessageBoxButtons.OK, MessageBoxIcon.Question);
                        TreePhOpera.SetNodeColorByID(mainForm.treePh, this.ID, Color.Red);
                    }
                    else
                    {
                        TreePhOpera.SetNodeColorByID(mainForm.treePh, this.ID, Color.Black);
                    }

                    // ����
                    DoSave();
                }
                else
                {
                    // ������
                    DataBaseManager databaseManager = DataBaseManager.GetDataBaseManager();
                    databaseManager.UnLockScript(this.ID);

                    this.isLocked = false;
                }
            }
        }

        /// <summary>
        /// ����ű�
        /// </summary>
        /// <returns>�Ƿ񱣴�ɹ�</returns>
        public bool DoSave()
        {
            bool saveSuccess = false;

            if (IsChanged) // ��Ҫ����
            {
                string scriptName = m_filename;

                if (scriptName.IndexOf('\\') != -1)
                {
                    scriptName = scriptName.Substring(scriptName.LastIndexOf('\\') + 1);
                }

                switch (scriptType)
                {
                    case "databaseFile": // ���ݿ�ű��ļ�
                        {
                            DataBaseManager databaseManager = DataBaseManager.GetDataBaseManager();
                            string strView = "";
                            bool bret = databaseManager.SaveScriptData(this.FileName, this.NewScriptCode, out strView);

                            if (bret)
                            {
                                if (strView != this.NewScriptCode)
                                {
                                    this.ScriptCode = databaseManager.GetBeforeOpen(strView, FileName);
                                }

                                this.ScriptCode = this.NewScriptCode;
                                

                                this.Text = scriptName;
                                databaseManager.UnLockScript(this.ID);
                                bret = true;
                                this.isLocked = false;
                            }

                            saveSuccess = bret;

                            break;
                        }
                    case "localFile": // ���ؽű��ļ�
                        {
                            Helper.WriteStringToFile(NewScriptCode, m_filename);
                            this.ScriptCode = this.NewScriptCode;
                            this.Text = scriptName;
                            saveSuccess = true;

                            break;
                        }
                    default:
                        {
                            break;
                        }
                }                
            }

            return saveSuccess;
        }

        /// <summary>
        /// ���ܸ�֪
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void luaEdit1_queryObjects(object sender, luaEditor.QueryEventsArgs e)
        {
            string strOut = "";
            IntPtr ls_out = IntPtr.Zero;

            // �����涨��
            // szCode:   ���еĴ���
            // nPos:     ���λ�ã����ְ�2���ַ��㣩
            // nIsDelete 0Ϊ�������룬1Ϊɾ����
            // 11-22ΪF1-F12,
            // 1111����Ctrl+Shift+F1,
            // 1011����Ctrl+F1
            // 111����Shift+F1

            int nIsDelete = e.nPos > 0 ? 0 : 1;

            if (e.keyword != "")
            {
                nIsDelete = 10 + Int32.Parse(e.keyword);
            }            
                        
            bool bret = LuaRuntime.LuaRun("OnCodeSense", e.szCode, Math.Abs(e.nPos), nIsDelete, Helper.GameLuaEditorSetting, ref ls_out);
            strOut = Marshal.PtrToStringAnsi(ls_out);            

            if (strOut == null)
            {
                return;
            }

            string[] as_data = strOut.Split(new char[] { '|' });

            if(as_data.Length < 3)
            {
                MessageBox.Show("����ֵ����ȷ������ֵΪ��" + strOut, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // ��ȡoverlen
            e.nOverLen = Int32.Parse((as_data[0]));

            // ��ʼ���α�
            int nPosition = 3;

            // ��ȡlist
            int nListCount = Int32.Parse((as_data[1]));

            if (as_data.Length < 3 + nListCount)
            {
                MessageBox.Show("List����ֵ��������ȷ������ֵΪ��" + strOut, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }      

            for(int i = 0; i < nListCount; i++ )
            {
                string s_name = as_data[nPosition++] as string;
                string s_word = as_data[nPosition++] as string;
                string s_type = as_data[nPosition++] as string;
                string s_info = as_data[nPosition++] as string;
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
        /// ��Ӷϵ��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void luaEditBox_OnBreakPointAdded(object sender, Fireball.Syntax.RowEventArgs e)
        {
            if (handleBreakPointAdded != null)
            {
                handleBreakPointAdded(this, e.Row);
            }
        }

        /// <summary>
        /// ɾ���ϵ��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void luaEditBox_OnBreakPointRemoved(object sender, Fireball.Syntax.RowEventArgs e)
        {
            if (handleBreakPointRemoved != null)
            {
                handleBreakPointRemoved(this, e.Row);
            }
        }

        /// <summary>
        /// ���ȡ���¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void luaEditBox_OnWordMouseHover(object sender, ref Fireball.Windows.Forms.CodeEditor.WordMouseEventArgs e)
        {
            Word hoveredWord = e.Word;

            if (e.Word.Index < e.Word.Row.Count - 1) // ��ĩβ�ĵ��ʶ�λ��׼����ʱȡ����ȡ��
            {
                if (hoveredWord != lastHoveredWord) // ͬһ������ֻ��ʾһ��Tip�͹���
                {                   
                    if (handleWordMouseHover != null)
                    {
                        handleWordMouseHover(this, ref e);
                    }

                    lastHoveredWord = hoveredWord;                   
                }
            }
        }
    }
    
    /// <summary>
    /// Lua�����
    /// </summary>
    public static class LuaRuntime
    {
        public static void LuaInitEx()
        {
            
        }

        [DllImport("Plugins\\Luacheck\\LuaRuntime.dll", EntryPoint = "LuaInit", CharSet = CharSet.Ansi)]
        public static extern bool LuaInit(string strCode, ref string szOutput);

        [DllImport("Plugins\\Luacheck\\LuaRuntime.dll", EntryPoint = "LuaRun", CharSet = CharSet.Ansi)]
        public static extern bool LuaRun(string szFuncName, string szCode, int nPos, int nIsDelete, string szSetting, ref IntPtr szOutput);
    };
}