using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;

using Fireball.CodeEditor.SyntaxFiles;
using Fireball.Windows.Forms;
using Fireball.Syntax;
using Fireball.Windows.Forms.CodeEditor;

namespace luaEditor
{
    public partial class luaEditorControl : UserControl
    {
        public string ForWho = "";
        private string m_curword = ""; // keywordǰ�Ĵ���
        private string m_list_context = "";

        private Color normalWordColor = Color.Gray; // ��ͨ������ɫ
        private Color commentWordColor = Color.Green; // ע�͵�����ɫ
        private Color fieldWordColor = Color.Purple; // �ֶε�����ɫ
        private Color stringWordColor = Color.Magenta; // �ַ���������ɫ
        private Color functionWordColor = Color.Brown; // ����������ɫ
        private Color gameObjectWordColor = Color.DeepSkyBlue; // ��Ϸ���󵥴���ɫ
        private TextStyle fieldWordStyle = new TextStyle(); // �ֶε�������
        private TextStyle functionWordStyle = new TextStyle(); // ������������
        private TextStyle gameObjectWordStyle = new TextStyle(); // ��Ϸ���󵥴�����
        private string gameObjectDescription = "npc|doodad|player|scene|item|skill"; // ��Ϸ��������  
        private int markWordDelay = 1000; // ��ǵ����ӳ�
        private Timer markWordTimer = new Timer(); // ��ǵ����õļ�ʱ��
        private bool markingWord = false; // �Ƿ����ڱ�ǵ���
        private char[] invalidChartArray = new char[] { ' ', '\t' }; // ��Ч�ַ�����
        private string[] switchRowKeyWordArray = new string[] { "function ", "local function ", "if ", "for " }; // ���йؼ�������
        private string[] queryObjectStringArray = new string[] { "+" }; // ���ܸ�֪�ַ����ָ�������

        // �ϵ�����¼�
        public event BreakPointAdded OnBreakPointAdded;
        public event BreakPointRemoved OnBreakPointRemoved;

        // ȡ���¼�
        public event WordMouseHover OnWordMouseHover;

        // ��ѯEvents
        public event QueryEventsHandle queryEvents;

        // ��ѯobjects
        public event QueryObjectHandle queryObjects;

        // ��ѯparms
        public event QueryParmsHandle queryParms;
        public event OnChangeHandle OnChange;

        public string strNs = "";

        // ����״̬
        private InputState inputState = InputState.NORMAL;
        private bool m_changefunc = false;

        // �Զ�����Ϣ
        private const int WM_CW_SCANCODE_FALSE = 0x500 + 3000;

        [DllImport("user32.dll")]
        public static extern IntPtr SendMessage(IntPtr hwnd, int wMsg, IntPtr wParam, string lParam);

        /// <summary>
        /// ���캯��
        /// </summary>
        public luaEditorControl()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ����������ɫ
        /// </summary>
        public Color FunctionWordColor
        {
            set
            {
                functionWordColor = value;
            }

            get
            {
                return functionWordColor;
            }
        }

        /// <summary>
        /// ��Ϸ���󵥴���ɫ
        /// </summary>
        public Color GameObjectWordColor
        {
            set
            {
                gameObjectWordColor = value;
            }

            get
            {
                return gameObjectWordColor;
            }
        }

        /// <summary>
        /// ��Ϸ��������
        /// </summary>
        public string GameObjectDescription
        {
            set
            {
                gameObjectDescription = value;
            }

            get
            {
                return gameObjectDescription;
            }
        }

        /// <summary>
        /// ѡ�е��ı�
        /// </summary>
        public string SelectedText
        {
            get 
            { 
                return codeEditorControl.Selection.Text; 
            }
        }

        /// <summary>
        /// ѡ�еĵ���
        /// </summary>
        public string SelectedWord
        {
            get 
            {
                if (codeEditorControl.Caret.CurrentWord != null)
                {
                    return codeEditorControl.Caret.CurrentWord.Text;
                }
                else
                {
                    return "";
                }
            }
        }

        /// <summary>
        /// ѡ�е��к�
        /// </summary>
        public int SelectedLineNumber
        {
            get 
            { 
                return codeEditorControl.Caret.CurrentRow.Index; 
            }
        }

        /// <summary>
        /// ѡ�е����ı�
        /// </summary>
        public string SelectedLineText
        {
            get 
            { 
                return codeEditorControl.Caret.CurrentRow.Text; 
            }
        }

        /// <summary>
        /// �ű�������
        /// </summary>
        public Font ScriptFont
        {
            set
            {
                codeEditorControl.FontName = value.Name;
                codeEditorControl.FontSize = value.Size;
            }

            get
            {
                return codeEditorControl.Font;
            }
        }

        /// <summary>
        /// �ű���������ɫ
        /// </summary>
        public Color ScriptForeColor
        {
            set
            {
                codeEditorControl.ForeColor = value;
            }

            get
            {
                return codeEditorControl.ForeColor;
            }
        }

        /// <summary>
        /// ȡ���ϵ�
        /// </summary>
        public List<int> BreakPointIndexList
        {
            get
            {
                List<int> list = new List<int>();                               

                for (int i = 0; i < codeEditorControl.Document.VisibleRows.Count; i++)
                {
                    if (codeEditorControl.Document.VisibleRows[i].Breakpoint)
                    {
                        list.Add(i + 1); // lua�кŴ�1��ʼ
                    }
                }

                return list;
            }
        }      
        
        /// <summary>
        /// �����к�
        /// </summary>
        public int DebugRowIndex
        {
            set
            {
                codeEditorControl.ActiveViewControl.DebugRowIndex = value;
            }

            get
            {
                return codeEditorControl.ActiveViewControl.DebugRowIndex;
            }
        }

        /// <summary>
        /// �Ƿ�ֻ��
        /// </summary>
        public bool ReadOnly
        {
            set
            {
                codeEditorControl.ReadOnly = value;
            }

            get
            {
                return codeEditorControl.ReadOnly;
            }
        }

        /// <summary>
        /// �Ҽ��˵�
        /// </summary>
        public ContextMenuStrip ContentMenu
        {
            get 
            { 
                return codeEditorControl.ContextMenuStrip; 
            }

            set 
            { 
                codeEditorControl.ContextMenuStrip = value; 
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ʼ����������
            fieldWordStyle.BackColor = Color.Transparent;
            fieldWordStyle.ForeColor = fieldWordColor;
            fieldWordStyle.Bold = false;
            fieldWordStyle.Italic = false;
            fieldWordStyle.Underline = false;

            functionWordStyle.BackColor = Color.Transparent;
            functionWordStyle.ForeColor = functionWordColor;
            functionWordStyle.Bold = false;
            functionWordStyle.Italic = false;
            functionWordStyle.Underline = false;

            gameObjectWordStyle.BackColor = Color.Transparent;
            gameObjectWordStyle.ForeColor = gameObjectWordColor;
            gameObjectWordStyle.Bold = false;
            gameObjectWordStyle.Italic = false;
            gameObjectWordStyle.Underline = false;

            // ��ʼ����ǵ��ʼ�ʱ��
            markWordTimer.Interval = markWordDelay;
            markWordTimer.Tick += MarkWord;

            // ת���¼��ص�
            codeEditorControl.Document.BreakPointAdded += new RowEventHandler(luaEditorBreakPointAdded);
            codeEditorControl.Document.BreakPointRemoved += new RowEventHandler(luaEditorBreakPointRemoved);
            codeEditorControl.WordMouseHover += new WordMouseHandler(luaEditorWordMouseHover);
        }

        /// <summary>
        /// ������Ӷϵ��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void luaEditorBreakPointAdded(object sender, RowEventArgs e)
        {
            if (OnBreakPointAdded != null)
            {
                OnBreakPointAdded(sender, e);
            }            
        }

        /// <summary>
        /// ѡ���ı�
        /// </summary>
        /// <param name="RowIndex">�к�</param>
        /// <param name="Column">�к�</param>
        /// <param name="Length">�ı�����</param>
		public void SelectText(int RowIndex, int Column, int Length)
        {
            codeEditorControl.SelectText(RowIndex, Column, Length);
        }

        public void SetupFindText()
        {
            codeEditorControl.SetupFindText();
        }

        public void FindNextSelectText()
        {
            codeEditorControl.FindNextSelectText();
        }

        public void FindPreviousSelectText()
        {
            codeEditorControl.FindPreviousSelectText();
        }

        /// <summary>
        /// ����ɾ���ϵ��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void luaEditorBreakPointRemoved(object sender, RowEventArgs e)
        {
            if (OnBreakPointRemoved != null)
            {
                OnBreakPointRemoved(sender, e);
            }
        }

        /// <summary>
        /// ����ȡ���¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void luaEditorWordMouseHover(object sender, ref WordMouseEventArgs e)
        {
            if (OnWordMouseHover != null)
            {
                OnWordMouseHover(sender, ref e);
            }
        }

        /// <summary>
        /// ����debug���
        /// </summary>
        public void ClearDebugMark()
        {
            codeEditorControl.ActiveViewControl.ClearDebugMark();
        }

        /// <summary>
        /// ����/�رնϵ�
        /// </summary>
        /// <param name="lineIndex">�ϵ������к�</param>
        /// <param name="enable">�Ƿ���</param>
        public void SetBreakPoint(int lineIndex, bool enable)
        {
            codeEditorControl.Document.VisibleRows[lineIndex].Breakpoint = enable;
        }

        /// <summary>
        /// ������ʾ�ı�
        /// </summary>
        /// <param name="tipText">�ı�����</param>
        /// <param name="location">λ��</param>
        public void SetTipText(string tipText, TextPoint location)
        {
            codeEditorControl.InfoTipPosition = location;
            codeEditorControl.InfoTipCount = 1;
            codeEditorControl.InfoTipSelectedIndex = 0;
            codeEditorControl.InfoTipText = tipText;
            codeEditorControl.InfoTipVisible = true;
        }

        /// <summary>
        /// �ر���ʾ�ı�
        /// </summary>
        public void CloseTipText()
        {
            codeEditorControl.InfoTipVisible = false;
        }

        /// <summary>
        /// �л���ִ����
        /// </summary>
        /// <param name="nLine">�к�</param>
        public void MoveToLine(int nLine)
        {
            this.codeEditorControl.GotoLine(nLine - 1);
        }

        /// <summary>
        /// �༭����л���λ��
        /// </summary>
        /// <param name="nX">x����</param>
        /// <param name="nY">y����</param>
        public void MoveToPos(int nX, int nY)
        {
            this.codeEditorControl.Caret.SetPos(new TextPoint(nX, nY));
            this.codeEditorControl.Caret.CurrentRow.EnsureVisible();
        }


        /// <summary>
        /// �ı�����
        /// </summary>
        public override string Text
        {
            get
            {
                return codeEditorControl.Document.Text;
            }

            set
            {
                cmbFunctionList.Items.Clear();
                codeEditorControl.Document.Text = value;
                QueryEventsArgs qe_arg = new QueryEventsArgs();
                qe_arg.leec = new leEventConnection();

                if (queryEvents != null)
                {
                    queryEvents(this, qe_arg); // ��ʼ�����Ȼص���ȡEvent�б�
                }

                foreach (LuaEditorEvent le in qe_arg.leec.m_list)
                {
                    foreach (LuaEditorParameter lp in le.parms)
                    {
                        ParameterCache.leParm_Cache.Add(lp);
                    }
                }
                
                CodeScan();

                // ��ʼ��ǵ���
                BeginMarkWord();
            }
        }

        /// <summary>
        /// ��ʼ��ǵ���
        /// </summary>
        private void BeginMarkWord()
        {
            if (!markingWord)
            {
                markWordTimer.Start();
                markingWord = true;
            }            
        }

        /// <summary>
        /// ��ǵ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void MarkWord(object sender, EventArgs e)
        {
            Timer timer = sender as Timer;

            MarkWord();
            timer.Stop();
            markingWord = false;
        }

        /// <summary>
        /// ��ǵ���
        /// </summary>
        private void MarkWord()
        {
            codeEditorControl.Document.ParseSome(codeEditorControl.Document.Count);

            foreach (Row row in codeEditorControl.Document.VisibleRows)
            {
                MarkWord(row);
            }
        }

        /// <summary>
        /// ����Ƿ�����ͨ����
        /// </summary>
        /// <param name="word">����</param>
        /// <returns>�Ƿ�����ͨ����</returns>
        private bool IsNormalWord(Word word)
        {
            bool isNormal = false;

            if (word.Style != null)
            {
                if (word.Style.ForeColor == normalWordColor)
                {
                    isNormal = true;
                }
            }

            return isNormal;
        }

        /// <summary>
        /// ��ǵ���
        /// </summary>
        /// <param name="row">������</param>
        private void MarkWord(Row row)
        {            
            string content = row.Text;
            content = content.Trim(new char[] { '\t', ' ' });

            // ����ֶ�
            string regexPattern = @"(\.(?<MatchWord>\w+))";
            List<string> fieldWordList = GetWordListInPattern(content, regexPattern);

            // ���Function
            regexPattern = @"((\.|\s)*(?<MatchWord>\w+)\()";            
            List<string> functionWordList  = GetWordListInPattern(content, regexPattern);

            // �����Ϸ����
            regexPattern = string.Format(@"((?<MatchWord>{0}))", gameObjectDescription);
            List<string> gameObjectWordList = GetWordListInPattern(content, regexPattern);

            foreach (Word word in row.FormattedWords)
            {
                if (gameObjectWordList.Contains(word.Text) && IsNormalWord(word))
                {
                    word.Style = gameObjectWordStyle;
                }
                else if (functionWordList.Contains(word.Text) && IsNormalWord(word))
                {
                    word.Style = functionWordStyle;
                }
                else if (fieldWordList.Contains(word.Text) && IsNormalWord(word))
                {
                    word.Style = fieldWordStyle;
                }
            }          
        }        

        /// <summary>
        /// ��ȡƥ��������ʽ�ĵ���
        /// </summary>
        /// <param name="content">�ı�����</param>
        /// <param name="pattern">������ʽ</param>
        /// <returns>����</returns>
        private List<string> GetWordListInPattern(string content, string pattern)
        {
            List<string> wordList = new List<string>();            
            Regex r = new Regex(pattern, RegexOptions.Singleline);

            if (r.IsMatch(content))
            {
                foreach (Match match in r.Matches(content))
                {
                    wordList.Add(match.Result("${MatchWord}"));
                }              
            }

            return wordList;
        }

        public void regObject(string name, Type type, System.Reflection.MethodInfo mi, string strNs)
        {
            LuaEditorParameter lp = new LuaEditorParameter(name, type, mi, strNs);
            ParameterCache.leParm_Cache.Add(lp);
        }

        public void clearObject()
        {
            ParameterCache.leParm_Cache.Clear();
            ParameterCache.asmCache.Clear();
        }

        /// <summary>
        /// ɨ��һ����䣬ģ�⸳ֵ����δ֪������֪���������
        /// </summary>
        /// <param name="sLine"></param>
        /// <param name="strNs"></param>
        private void CodeLineScan(string sline, string strNs)
        {
            //����ע��
            if (sline.IndexOf("--") != -1)
            {
                string[] asp = { "--" };
                sline = sline.Split(asp, StringSplitOptions.None)[0];
            }
            sline = sline.Trim();
            if (sline == "") return;

            //��������luanet.load_assembly()
            if(GetObjectFullName(sline) == "luanet.load_assembly()")
            {
                char[] asp = { '\"', '\''};
                if (sline.Trim().Split(asp).Length < 2) return;
                string dllname = sline.Trim().Split(asp)[1];
                ParameterCache.loadDll(dllname);
            }

            //Ѱ�Ҹ�ֵ���ͺ���ִ����䣨��ʱ�Ĺ����ǣ�������һ��=�ţ�����ƥ�䣬��˫����ƥ�䣩
            if (sline.Length > 2 &&
                (!(sline.StartsWith("="))) &&
                sline.Length - sline.Replace("=", "").Length == 1 &&
                sline.Replace("(", "").Length == sline.Replace(")", "").Length &&
                sline.Replace("{", "").Length == sline.Replace("}", "").Length &&
                sline.Replace("[", "").Length == sline.Replace("]", "").Length &&
                (sline.Replace("'", "").Length - sline.Length) % 2 == 0 &&
                (sline.Replace("\"", "").Length - sline.Length) % 2 == 0
                )
            {
                string sleft = "", sright = "";
                if (sline.IndexOf('=') == -1)
                {
                    sright = sline.Trim();
                }
                else
                {
                    sleft = sline.Substring(0, sline.IndexOf('=') - 1).Trim();
                    sright = sline.Substring(sline.IndexOf('=') + 1).Trim();
                }

                //���Ҳ���ʽ���з��� �� abc.e(ert.s(),2).value
                for (int i = 0; i < sright.Length; i++)
                {
                    string m = sright.Substring(i, 1);
                    if (m == "." || m == ":")
                    {
                        //ȡ����������ȫ�ֻ��棬����ȥ����ȥ��
                        string s = GetObjectFullName(sright.Substring(0, i));
                        ParameterCache.findMethodInfoByFullName(s, strNs);
                    }
                }

                if (sleft != "")
                {
                    //�������� [local] var = luanet.import_type('VisualEditor.Doit')���
                    if(GetObjectFullName(sright) == "luanet.import_type()")
                    {
                        string leftns = strNs;
                        if (sleft.IndexOf("local ") == -1)  leftns = "";
                        sleft = sleft.Replace("local ", "");
                        char[] asp = { '\"', '\'' };
                        string classname = sright.Trim().Split(asp)[1];
                        if (classname != "" && classname != ")")
                        {
                            //֮�������ڲ������typeֱ�Ӹ��ˣ��ǿ��ǵ�����һ�û�д����dll���֣�����������
                            //�������Ժ������㡰.����ʱ�򣬲�load�Ǹ�dll��һ��load��������ÿ��.����load��
                            ParameterCache.leParm_Cache.Add(new LuaEditorParameter(sleft.Trim() + "()", classname, leftns));  //�������Ϊ���õõ�����ֵ������
                            ParameterCache.leParm_Cache.Add(new LuaEditorParameter(sleft.Trim(), classname, leftns));         //�������Ϊ���õõ����캯���Ĳ���
                        }
                    }
                    //�����������
                    else
                    {
                        //�����ҵø���ֵһ���� :)
                        if (sleft.IndexOf("local ") != -1)
                        {
                            //˽�б���
                            sleft = sleft.Replace("local ", "");
                            ParameterCache.bindTwoObjects(GetObjectFullName(sleft), strNs, GetObjectFullName(sright), strNs);
                        }
                        else
                        {
                            //ȫ�ֱ���
                            ParameterCache.bindTwoObjects(GetObjectFullName(sleft), "", GetObjectFullName(sright), strNs);
                        }
                    }
                }
            }

        }


        /// <summary>
        /// ����ɨ�裬����ɨ�裬�³����ֶ�������ͣ����ֺ����ķ���ֵ�ȵȡ�Ĭ��Ϊ��ɨ�衣
        /// </summary>
        private void CodeScan()
        {
            CodeScan(true);
        }

        /// <summary>
        /// ����ɨ�裬����ɨ�裬�³����ֶ�������ͣ����ֺ����ķ���ֵ�ȵȡ�
        /// </summary>
        /// <param name="scanSyn">�Ƿ����ɨ��,��ɨ����������ͣ�ǳɨ���ɨ�赽��������</param>
        public void CodeScan(bool scanSyn)
        {
            int curLine = -1;
            string strNs = "", lastNs = "";

            //���¾�ֵ
            string oldfunc = cmbFunctionList.Text;
            cmbFunctionList.Items.Clear();

            foreach(string sline1 in codeEditorControl.Document.Lines)
            {
                curLine++;
                string sline = sline1.Replace("\r", "");
                Segment seg = codeEditorControl.Document.GetSegmentFromPos(new TextPoint(0, curLine));
                strNs = GetNameSpace(seg);
                lastNs = strNs;

                if (scanSyn)
                {
                    CodeLineScan(sline, strNs);
                }

                //���뵽functionlist
                if (sline.IndexOf("function ") != -1)
                {
                    AddToFunctionList(sline, curLine);
                }
            }

            // ɨ�����ˣ�����У��򻻻ؾ�ֵ
            foreach (FunctionList fl in cmbFunctionList.Items)
            {
                if (fl.ToString() == oldfunc)
                {
                    cmbFunctionList.SelectedItem = fl;
                    break;
                }
            }
        }
        
        /// <summary>
        /// ��Ϣ����
        /// </summary>
        /// <param name="m">��Ϣ����</param>
        protected override void WndProc(ref Message message)
        {
            switch(message.Msg)
            {
                case WM_CW_SCANCODE_FALSE:
                    {
                        CodeScan(false);

                        break;
                    }
                default:
                    {
                        break;
                    }
            }

            base.WndProc(ref message);
        }

        /// <summary>
        /// ���뵽�����б��ظ��ĸ��£�û�еļ�
        /// </summary>
        /// <param name="strLine"></param>
        /// <param name="nLineNumber"></param>
        private void AddToFunctionList(string strLine, int nLineNumber)
        {
            ++nLineNumber;
            string new_func_name = strLine.Replace("local function ", "").Trim();
            new_func_name = new_func_name.Replace("function ", "").Trim();
            if (new_func_name.IndexOf('(') == -1) return;
            new_func_name = new_func_name.Substring(0, new_func_name.IndexOf('(')).Trim();

            for( int i = 0; i <cmbFunctionList.Items.Count; i++ )
            {
                FunctionList fl = (FunctionList)cmbFunctionList.Items[i];

                if (fl.FunctionName == new_func_name)
                {
                    if (fl.LineNumber != nLineNumber || fl.FullText != strLine)
                    {
                        cmbFunctionList.Items[i] = new FunctionList(new_func_name, strLine, nLineNumber);
                    }

                    return;
                }
            }

            cmbFunctionList.Items.Add(new FunctionList(new_func_name, strLine, nLineNumber));
        }

        /// <summary>
        /// ȡ�������������� obj.func(obj1:method(xx.a(), x).value ��� obj1:method().value
        /// </summary>
        /// <param name="sexp">���������</param>
        /// <returns></returns>
        private string GetObjectFullName(string sexp)
        {
            string keyword = "";
            try
            {
                string keyW = sexp;

                //ȥ���������� �� obj.func(obj1:method(xx.a(), x).value ��� obj1:method().value
                int nleft = 0, nRight = 0;
                string keyW2 = "";
                for (int i = keyW.Length; i > 0; i--)
                {
                    string m = keyW.Substring(i - 1, 1);
                    if (m == "(") nleft++;
                    if (m == ")") nRight++;
                    if (nleft > nRight) break;
                    if ((m == ")") && nRight - nleft < 2) keyW2 = m + keyW2;
                    else if (nleft == nRight)
                    {
                        keyW2 = m + keyW2;
                    }
                }
                keyW = keyW2;

                keyW2 = "";
                nleft = 0;
                nRight = 0;
                for (int i = keyW.Length; i > 0; i--)
                {
                    string m = keyW.Substring(i - 1, 1);
                    if (m == "[") nleft++;
                    if (m == "]") nRight++;
                    if (nleft > nRight) break;
                    if ((m == "]") && nRight - nleft < 2) keyW2 = m + keyW2;
                    else if (nleft == nRight)
                    {
                        keyW2 = m + keyW2;
                    }
                }
                keyW = keyW2;


                //ȥǰ��Ĳ��֣�ֻ������������
                char[] ac_sp = { ' ', '=', '\t', '!', '@', '#', '$', '%', '^', '&', '*', '+', '-', '/', '\\', ',' };
                string[] ss = keyW.Split(ac_sp);
                keyword = ss[ss.Length - 1];

                string[] ac_sp1 = { ".." };
                string[] ss1 = keyword.Split(ac_sp1, StringSplitOptions.None);
                keyword = ss1[ss1.Length - 1];                
            }
            catch
            {
                keyword = "";
            }
            return (keyword);
        }

        /// <summary>
        /// �����������������Լ�������ڲ�����λ��
        /// �� obj.func(obj1:method(xx.a(b()), ��� obj1:method##2
        /// </summary>
        /// <param name="sexp">�����й������������ַ�</param>
        /// <returns></returns>
        private string GetFunctionFullNameAndTurns(string sexp)
        {
            string keyword = "";
            try
            {
                string keyW = sexp;

                //ȥ�������������� �� obj.func(obj1:method(xx.a(b()), ��� obj1:method##2
                int nleft = 0, nRight = 0, nRet = 1;
                string keyW2 = "";
                for (int i = keyW.Length; i > 0; i--)
                {
                    string m = keyW.Substring(i - 1, 1);
                    if (nleft > nRight) 
                        keyW2 = m + keyW2;
                    if (m == "(") 
                        nleft++;
                    if (m == ")") 
                        nRight++;
                    if (nleft == nRight && m == ",") 
                        nRet++;
                }
                keyW = GetObjectFullName(keyW2);
                keyword = keyW + "##" + nRet.ToString();
            }
            catch
            {
                keyword = "";
            }
            return (keyword);
        }


        /// <summary>
        /// ���ݶ��䣬��ȡns����Ҳ���Ǻ�����
        /// </summary>
        /// <param name="seg"></param>
        /// <returns>������</returns>
        private string GetNameSpace(Segment seg)
        {
            string strNs = "";
            
            if (seg != null)
            {
                if (seg.Parent != null)
                {
                    while (seg.Parent.Scope != null)
                    {
                        seg = seg.Parent;
                    }
                }

                strNs = seg.StartRow.Text;
                
                if (seg.EndRow != null)
                {
                    int index = strNs.LastIndexOf('(');

                    if (index == -1)
                    {
                        return "";
                    }

                    strNs = strNs.Substring(0, strNs.LastIndexOf('('));

                    index = strNs.LastIndexOf(' ');

                    if (index == -1)
                    {
                        return "";
                    }

                    strNs = strNs.Substring(index).Trim();
                }
                else
                {
                    strNs = "";
                }                
            }

            return strNs;
        }        

        /// <summary>
        /// �����¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CodeEditor_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (this.OnChange != null)
            {
                this.OnChange(this, new EventArgs());
            }

            if (this.codeEditorControl.Caret.CurrentWord != null)
            {
                string s = this.codeEditorControl.Caret.CurrentWord.Text;
                m_curword = s == null ? "" : s;
            }
            else
            {
                m_curword = "";
            }

            if (e.KeyChar > 128)
            {
                return;
            }

            if (this.ForWho == "GameLuaEditor")
            {
                ShowAutoList4GameLuaEditor(e, this.codeEditorControl);
            }
            else
            {
                ShowAutoList(e, this.codeEditorControl);
            }            
        }

        /// <summary>
        /// ���̰����¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CodeEditor_KeyDown(object sender, KeyEventArgs e)
        {
            // ���������¼�
            this.OnKeyDown(e);

            if (this.ForWho == "GameLuaEditor")
            {
                if ((e.KeyCode== Keys.Left || e.KeyCode == Keys.Right) && e.Alt == false)
                {
                    //������ƶ���꣬�Ǿ͹ر��б�
                    codeEditorControl.AutoListVisible = false;
                    codeEditorControl.InfoTipVisible = false;
                }

                Keys[] fkey = new Keys[] { Keys.F1, Keys.F2, Keys.F3, Keys.F4, Keys.F5, Keys.F6, Keys.F7, Keys.F8, Keys.F9, Keys.F10, Keys.F11, Keys.F12};

                for (int i = 0; i < fkey.Length; i++)
                {
                    if (e.KeyCode == fkey[i])
                    {
                        // ǿ�ƹ涨�����ascii��>130����ô00131ΪF1��00132ΪF2����������
                        // XY131 X����Ctrl��Y����Shift
                        int nadd = 0;
                        if (e.Control) nadd += 1000;
                        if (e.Shift) nadd += 100;
                        ShowAutoList4GameLuaEditor(new KeyPressEventArgs((char)(nadd + 131 + i)), this.codeEditorControl);
                        break;
                    }
                }
            }

            // ��ʼ��ǵ���
            BeginMarkWord();
        }

        /// <summary>
        /// ��ʾ��ʾѡ��
        /// </summary>
        /// <param name="eKey">������Ϣ</param>
        /// <param name="codeEditor">��ǰ�༭�ؼ�</param>
        private void ShowAutoList4GameLuaEditor(KeyPressEventArgs eKey, CodeEditorControl codeEditor)
        {
            // ǿ�ƹ涨�����ascii��>130����ô131ΪF1��132ΪF2����������
            // �������߶�ʮһ���ȹر��б���˵
            codeEditor.AutoListVisible = false;
            codeEditor.InfoTipVisible = false;

            //���㵱ǰ����Ķ����Լ�λ��
            int nOffset = 0;
            QueryEventsArgs queryEventsArgs = new QueryEventsArgs();

            if (eKey.KeyChar != '\b' && eKey.KeyChar <= 128)
            {
                queryEventsArgs.szCode = string.Format("{0}{1}{2}", 
                                                       codeEditor.Document.Text.Substring(0, codeEditor.Selection.LogicalSelStart),
                                                       eKey.KeyChar,
                                                       codeEditor.Document.Text.Substring(codeEditor.Selection.LogicalSelStart));
                queryEventsArgs.nPos = codeEditor.Selection.LogicalSelStart + 1;

                nOffset++;
            }
            else
            {
                queryEventsArgs.szCode = codeEditor.Document.Text;
                queryEventsArgs.nPos = codeEditor.Selection.LogicalSelStart;
            }

            // ���ʲôҲû���룬ֻ�ǰ������ȼ�
            if (eKey.KeyChar > 128)
            {
                int ikey = (int)eKey.KeyChar - 130;
                queryEventsArgs.keyword = ikey.ToString();
            }

            // ����Lua�ܱ�̬����Ϊ���ֵĳ���Ϊ2�������ҵ��ֹ������ж������ģ�Ȼ����ڳ�����
            int nAdd = 0;

            for (int i = 0; i < queryEventsArgs.nPos; i++ )
            {
                if (queryEventsArgs.szCode[i] > 128)
                {
                    nAdd++;
                }
            }
            queryEventsArgs.nPos += nAdd;

            if (eKey.KeyChar == '\b')
            {
                queryEventsArgs.nPos *= -1;
            }

            // ��ȡ���б�
            if (queryObjects != null && !codeEditorControl.ReadOnly) // ����ģʽ�²��������Զ���ɹ���
            {
                queryObjects(this, queryEventsArgs);
                bool querySuccess = false;

                if (queryEventsArgs.parms_list.Count > 0)
                {
                    codeEditor.InfoTipPosition = new TextPoint(codeEditor.Caret.Position.X + 1, codeEditor.Caret.Position.Y);
                    codeEditor.InfoTipCount = queryEventsArgs.parms_list.Count;

                    for (int i = 0; i < queryEventsArgs.parms_list.Count; i++)
                    {
                        codeEditor.InfoTipSelectedIndex = i;
                        string argIn = queryEventsArgs.parms_list[i];
                        codeEditor.InfoTipText = argIn;
                    }

                    codeEditor.InfoTipVisible = true;
                    querySuccess = true;
                }

                if (queryEventsArgs.leTable.Count > 0)
                {
                    codeEditor.AutoListBeginLoad();
                    codeEditor.AutoListClear();

                    foreach (LuaEditorTableItem tableItem in queryEventsArgs.leTable.Values)
                    {
                        codeEditor.AutoListAdd(tableItem.Name, tableItem.ItemValue.ToString(), tableItem.Info, int.Parse(tableItem.ItemType));                        
                    }

                    codeEditor.AutoListEndLoad();
                    codeEditor.AutoListPosition = new TextPoint(codeEditor.Caret.Position.X + nOffset, codeEditor.Caret.Position.Y);
                    codeEditor.AutoListPosition.X -= queryEventsArgs.nOverLen;

                    if (codeEditor.AutoListPosition.X < 0)
                    {
                        codeEditor.AutoListPosition.X = 0;
                    }

                    codeEditor.AutoListVisible = true;
                    querySuccess = true;
                }

                switch (eKey.KeyChar)
                {
                    case '\r':
                        {
                            IndentText(codeEditor);                           

                            break;
                        }
                        
                    default:
                        {
                            break;
                        }                    
                }

                if (!querySuccess)
                {
                    // ShowAutoList(eKey, codeEditor);
                }
            }            
        }

        /// <summary>
        /// �����ı�
        /// </summary>
        /// <param name="codeEditor">��ǰ�༭�ؼ�</param>
        private void IndentText(CodeEditorControl codeEditor)
        {
            Row currentRow = codeEditor.Document[codeEditor.Caret.Position.Y - 1];

            if (currentRow != null)
            {
                Row nextRow = codeEditor.Document[codeEditor.Caret.Position.Y + 1];

                if (nextRow == null || nextRow.Text.TrimStart(invalidChartArray) == "")
                {
                    string rowText = currentRow.Text.TrimStart(invalidChartArray);

                    foreach (string keyWord in switchRowKeyWordArray)
                    {
                        if (rowText.StartsWith(keyWord))
                        {
                            SendMessage(this.Handle, WM_CW_SCANCODE_FALSE, IntPtr.Zero, "");
                            break;
                        }
                    }
                }
            }  
        }

        /// <summary>
        /// ��ʾ��ʾѡ��
        /// </summary>
        /// <param name="eKey">������Ϣ</param>
        /// <param name="codeEditor">��ǰ�༭�ؼ�</param>
        private void ShowAutoList(KeyPressEventArgs eKey, CodeEditorControl codeEditor)
        {
            if (eKey.KeyChar > 128)
            {
                return;
            }

            //��ȡ����������                     
            string keyWord = codeEditor.Caret.CurrentRow.Text.Substring(0, codeEditor.Caret.Position.X);
            string objectFullName = GetObjectFullName(keyWord);

            string functionName = "";
            int cursorPosition = 0;

            if (!((eKey.KeyChar >= 'a' && eKey.KeyChar <= 'z') || (eKey.KeyChar >= 'A' && eKey.KeyChar <= 'Z') || (eKey.KeyChar >= '0' && eKey.KeyChar <= '9') || eKey.KeyChar == '_'))
            {
                // ����֮ǰ�Ĳ�������ȡ���������Լ����������ڵ�λ��
                if (eKey.KeyChar == '\b')
                {
                    if (keyWord.Length > 0)
                    {
                        keyWord = keyWord.Substring(0, keyWord.Length - 1);
                    }
                }
                else
                {
                    keyWord += eKey.KeyChar.ToString(); // ��','Ҳ׷�Ӻ���ȥ��
                }

                functionName = GetFunctionFullNameAndTurns(keyWord);
                int index = functionName.IndexOf("##");
                cursorPosition = int.Parse(functionName.Substring(index + 2));
                functionName = functionName.Substring(0, index);                
                
                // ��ȡ�����ռ䣨˵���˾��Ǻ�������
                Segment segment = codeEditor.Caret.CurrentSegment();
                
                // ���������ע���ڣ������������ڣ�������
                segment = codeEditor.Caret.CurrentSegment();

                if (segment != null)
                {
                    if (segment.BlockType.Style.Name == "Lua Comment")
                    {
                        return;
                    }

                    if (segment.BlockType.Style.Name == "Lua String")
                    {
                        return;
                    }
                    
                    string currentText = codeEditor.Caret.CurrentRow.Text;
                    currentText = currentText.Substring(0, codeEditor.Caret.Position.X);
                    currentText = currentText.Trim(invalidChartArray);

                    if (currentText.StartsWith("--"))
                    {
                        return;
                    }

                    if ((currentText.Replace("'", "").Length - currentText.Length) % 2 == 1)
                    {
                        return;
                    }

                    if ((currentText.Replace("\"", "").Length - currentText.Length) % 2 == 1)
                    {
                        return;
                    }                    
                }
            }     
               
            switch (eKey.KeyChar)
            {
                case ':':
                    {
                        goto case '.';
                    }
                case '.':
                    {
                        switch (inputState)
                        {
                            case InputState.FUNCTIONLIST_OPEN:
                                {
                                    codeEditor.AutoListVisible = false;
                                    inputState = InputState.NORMAL;

                                    break;
                                }
                            case InputState.OBJECTLIST_OPEN:
                                {
                                    goto case InputState.FUNCTIONLIST_OPEN;
                                }
                            case InputState.INTELLISENCE_OPEN:
                                {
                                    goto case InputState.FUNCTIONLIST_OPEN;
                                }
                            default:
                                {
                                    break;
                                }
                        }

                        if (objectFullName != "")
                        {
                            // �����¼�
                            QueryEventsArgs queryEventsArg = new QueryEventsArgs();
                            queryEventsArg.keyword = objectFullName;
                            List<string> objectList = StandardLib.GetObjectList(objectFullName);

                            if (objectList.Count == 0)
                            {
                                objectList = ParameterCache.findMethodInfoByFullName(objectFullName, strNs);
                            }
        
                            foreach (string objectInfo in objectList)
                            {
                                string[] infoArray = objectInfo.Split(queryObjectStringArray, StringSplitOptions.RemoveEmptyEntries);
                                string type = infoArray[1];
                                string value = infoArray[0];

                                if (!queryEventsArg.leTable.ContainsKey(objectInfo))
                                {
                                    queryEventsArg.leTable.Add(objectInfo, new LuaEditorTableItem(value, type, value, ""));
                                }
                            }

                            objectList.Clear();

                            if (queryEventsArg.leTable.Count > 0)
                            {
                                codeEditor.AutoListBeginLoad();
                                codeEditor.AutoListClear();

                                m_list_context = "";

                                foreach (LuaEditorTableItem tableItem in queryEventsArg.leTable.Values)
                                {
                                    switch (tableItem.ItemType)
                                    {
                                        case "method":
                                            {
                                                if (eKey.KeyChar == ':')
                                                {
                                                    codeEditor.AutoListAdd(tableItem.Name, tableItem.ItemValue.ToString(), tableItem.Info, 1);
                                                }

                                                break;
                                            }                                            
                                        case "function":
                                            {
                                                if (eKey.KeyChar == '.')
                                                {
                                                    codeEditor.AutoListAdd(tableItem.Name, tableItem.ItemValue.ToString(), tableItem.Info, 6);
                                                }

                                                break;
                                            }                                            
                                        case "table":
                                            {
                                                if (eKey.KeyChar == '.')
                                                {
                                                    codeEditor.AutoListAdd(tableItem.Name, tableItem.ItemValue.ToString(), tableItem.Info, 3);
                                                }

                                                break;
                                            }
                                            
                                        case "var":
                                            {
                                                if (eKey.KeyChar == '.')
                                                {
                                                    codeEditor.AutoListAdd(tableItem.Name, tableItem.ItemValue.ToString(), tableItem.Info, 4);
                                                }

                                                break;
                                            }          
                                        default:
                                            {
                                                break;
                                            }
                                    }

                                    m_list_context += "|" + tableItem.Name;
                                }

                                codeEditor.AutoListEndLoad();
                                codeEditor.AutoListPosition = new TextPoint(codeEditor.Caret.Position.X + 1, codeEditor.Caret.Position.Y);
                                codeEditor.AutoListVisible = true;
                                inputState = InputState.OBJECTLIST_OPEN;
                            }
                        }

                        break;
                    }                                      
                case ' ':
                    {
                        switch (inputState)
                        {
                            case InputState.FUNCTIONLIST_OPEN:
                                {
                                    codeEditor.AutoListVisible = false;
                                    inputState = InputState.NORMAL;

                                    break;
                                }
                            case InputState.OBJECTLIST_OPEN:
                                {
                                    goto case InputState.OBJECTLIST_OPEN;
                                }
                            case InputState.INTELLISENCE_OPEN:
                                {
                                    codeEditor.AutoListVisible = false;
                                    inputState = InputState.NORMAL;

                                    goto case InputState.NORMAL;
                                }
                            case InputState.NORMAL:
                                {
                                    break;
                                }
                            default:
                                {
                                    break;
                                }
                        }
                       
                        break;
                    }
                case ')':
                    {
                        codeEditor.InfoTipVisible = false;
                        codeEditor.AutoListVisible = false;
                        inputState = InputState.NORMAL;

                        break;
                    }
                    
                case ',':
                    {
                        switch (inputState)
                        {
                            case InputState.INTELLISENCE_OPEN:
                                {
                                    codeEditor.AutoListVisible = false;
                                    codeEditor.InfoTipVisible = false;
                                    inputState = InputState.NORMAL;

                                    break;
                                }
                            case InputState.FUNCTIONLIST_OPEN:
                                {
                                    goto case InputState.INTELLISENCE_OPEN;
                                }
                            default:
                                {
                                    break;
                                }
                        }

                        if (functionName != "")
                        {
                            PopInfo(functionName, cursorPosition, strNs, codeEditor);
                        }

                        break;
                    }                    
                case '(' :
                    {
                        switch (inputState)
                        {
                            case InputState.FUNCTIONLIST_OPEN:
                                {
                                    codeEditor.AutoListVisible = false;
                                    inputState = InputState.NORMAL;

                                    break;
                                }
                            case InputState.OBJECTLIST_OPEN:
                                {
                                    goto case InputState.OBJECTLIST_OPEN;

                                    break;
                                }
                            default:
                                {
                                    break;
                                }
                        }                        

                        if (objectFullName != "")
                        {
                            PopInfo(objectFullName, 1, strNs, codeEditor);
                        }

                        break;
                    }
                    
                case '\r':
                    {
                        switch (inputState)
                        {
                            case InputState.FUNCTIONLIST_OPEN:
                                {
                                    codeEditor.AutoListVisible = false;
                                    inputState = InputState.NORMAL;

                                    break;
                                }
                            case InputState.OBJECTLIST_OPEN:
                                {
                                    goto case InputState.FUNCTIONLIST_OPEN;
                                }
                            case InputState.INTELLISENCE_OPEN:
                                {
                                    goto case InputState.FUNCTIONLIST_OPEN;
                                }
                            default:
                                {
                                    // ����ģ��ִ�е�ǰ�����
                                    Row currentRow = codeEditor.Document[codeEditor.Caret.Position.Y - 1];
                                    CodeLineScan(currentRow.Text, strNs);

                                    // �����Զ�����
                                    IndentText(codeEditor);

                                    break;
                                }
                        }

                        break;
                    }
                case '\b':
                    {
                        if (inputState == InputState.INTELLISENCE_OPEN)
                        {
                            if (objectFullName == "")
                            {
                                codeEditor.AutoListVisible = false;
                                inputState = InputState.NORMAL;                                
                            }
                        }

                        break;
                    }
                case '=':
                    {
                        if (inputState != InputState.NORMAL)
                        {
                            codeEditor.AutoListVisible = false;
                            inputState = InputState.NORMAL;
                        }

                        break;
                    }
                case '+':
                    {
                        goto case '=';
                    }
                case '-':
                    {
                        goto case '=';
                    }
                case '*':
                    {
                        goto case '=';
                    }
                case '/':
                    {
                        goto case '=';
                    }
                case '?':
                    {
                        goto case '=';
                    }
                default:
                    {
                        if (inputState == InputState.NORMAL)
                        {
                            if ((eKey.KeyChar >= 'a' && eKey.KeyChar <= 'z') || (eKey.KeyChar >= 'A' && eKey.KeyChar <= 'Z') || (eKey.KeyChar >= '0' && eKey.KeyChar <= '9') || eKey.KeyChar == '_')
                            {
                                // ���ܸ�֪>
                                List<string> wordList;
                                wordList = ParameterCache.GetIntelliSenceWordList(objectFullName + eKey.KeyChar.ToString(), strNs);

                                if (wordList.Count > 0)
                                {
                                    codeEditor.AutoListBeginLoad();
                                    codeEditor.AutoListClear();

                                    m_list_context = "";

                                    foreach (string s in wordList)
                                    {
                                        codeEditor.AutoListAdd(s, s, s, 1);
                                        m_list_context += "|" + s;
                                    }

                                    codeEditor.AutoListEndLoad();
                                    
                                    codeEditor.AutoListPosition = new TextPoint(codeEditor.Caret.Position.X/* + 1*/, codeEditor.Caret.Position.Y);
                                    codeEditor.AutoListVisible = true;
                                    inputState = InputState.INTELLISENCE_OPEN;
                                }

                                if (m_list_context.ToLower().IndexOf(string.Format("|{0}{1}", objectFullName, eKey.KeyChar).ToLower()) == -1) // ����ǰ������ĸ��ƥ�䣬��ɵ��б�
                                {
                                    codeEditor.AutoListVisible = false;
                                    inputState = InputState.NORMAL;
                                }

                                if (string.Format("{0}|", m_list_context).IndexOf(string.Format("|{0}{1}|", objectFullName, eKey.KeyChar)) != -1)  // ��ȫƥ��Ҳ�ɵ��б�
                                {
                                    codeEditor.AutoListVisible = false;
                                    inputState = InputState.NORMAL;
                                }
                            }
                        }

                        break;
                    }
            }
        }

        /// <summary>
        /// ����keyword����������ʾ
        /// </summary>
        /// <param name="keyword">����ȫ��</param>
        /// <param name="index">�����λ��</param>
        /// <param name="codeEditor">luaEditorControl����</param>
        private void PopInfo(string keyword, int index, string strNs, CodeEditorControl codeEditor)
        {
            string content = "";
            QueryEventsArgs queryEventArgs = new QueryEventsArgs();

            // �ȿ����ĵ�����û��
            foreach (FunctionList functionList in cmbFunctionList.Items)
            {
                if (keyword == functionList.FunctionName)
                {
                    content = functionList.FullText;
                    break;
                }
            }

            // û�а�����ôȥ���׼��
            if (content == "")
            {
                queryEventArgs.keyword = keyword;
                content = StandardLib.GetFunctionInfo(keyword);
            }

            // ��׼����鲻�����Ǿ�ȥ���仺������
            if (content == "")
            {
                content = ParameterCache.FindMethodParameterByFullName(keyword, strNs);
            }

            //������Ҳû�У��Ǿ�ֻ�ܻص���
            if (content == "" && queryParms != null)
            {
                queryParms(this, queryEventArgs);         /*�ص�����ȡParms�б�*/
            }


            if (content != "")
            {
                string[] spec = { "<%newline%>" };
                string[] aspp = content.Split(spec, StringSplitOptions.RemoveEmptyEntries);
                foreach (string sitem in aspp)
                {
                    queryEventArgs.parms_list.Add(sitem.Replace("\r\n", "<BR>"));
                }
            }

            //�������˵Ĳ����Ž�ȥ
            if (queryEventArgs.parms_list.Count > 0)
            {
                codeEditor.InfoTipPosition = new TextPoint(codeEditor.Caret.Position.X + 1, codeEditor.Caret.Position.Y);
                codeEditor.InfoTipCount = queryEventArgs.parms_list.Count;

                for (int i = 0; i < queryEventArgs.parms_list.Count; i++)
                {
                    codeEditor.InfoTipSelectedIndex = i;
                    string argIn = queryEventArgs.parms_list[i];
                    if(argIn.IndexOf("<BR>") == -1)
                    {
                        argIn += "<BR>";
                    }

                    try 
                    {
                        string sc1 = argIn.Substring(0, argIn.IndexOf("<BR>"));
                        if(sc1.IndexOf('(') != -1)
                        {
                            string s1 = sc1.Substring(0, sc1.LastIndexOf('('));
                            string s2 = sc1.Substring(sc1.LastIndexOf('(') + 1);
                            s2 = s2.Substring(0, s2.Length - 1);
                            string s3 = argIn.Substring(argIn.IndexOf("<BR>"));

                            string[] asp = { "," };
                            string[] argIns = s2.Split(asp, StringSplitOptions.RemoveEmptyEntries);
                            if (argIns.Length > index - 1)
                            {
                                string t = argIns[index - 1].Trim();
                                if (t.Substring(t.Length - 1) == "[")
                                    argIns[index - 1] = "<b>" + t.Substring(0, t.Length - 1) + "</b>[";
                                else if (t.Substring(t.Length - 1) == "]")
                                    argIns[index - 1] = "<b>" + t.Substring(0, t.Length - 1) + "</b>]";
                                else
                                    argIns[index - 1] = "<b>" + t + "</b>";
                            }
                            s2 = string.Join(",", argIns);
                            codeEditor.InfoTipText = s1 + "(" + s2 + ")" + s3;         
                        }
                    }
                    catch
                    {
                        codeEditor.InfoTipText = argIn;
                        throw;
                    }

                }
                codeEditor.InfoTipVisible = true;
                inputState = InputState.PARMSLIST_OPEN;
            }
        }

        private void cmbFunctionList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!m_changefunc) return;
            FunctionList fl = (FunctionList)cmbFunctionList.SelectedItem;
            if (fl == null) return;
            codeEditorControl.Caret.SetPos(new TextPoint(codeEditorControl.Caret.Position.X, fl.LineNumber));
            codeEditorControl.Focus();
            //ce1.Document.ResetVisibleRows();
            //ce1.Caret.CropPosition();
            codeEditorControl.Caret.MoveUp(false);
            codeEditorControl.Caret.MoveDown(false);
            m_changefunc = false;
        }

        private void cmbFunctionList_MouseEnter(object sender, EventArgs e)
        {
            CodeScan(false);
            m_changefunc = true;
        }

        private void cmbFunctionList_MouseLeave(object sender, EventArgs e)
        {
            m_changefunc = false;
        }

        private void luaEditorControl_Load(object sender, EventArgs e)
        {
            CodeEditorSyntaxLoader.SetSyntax(this.codeEditorControl, SyntaxLanguage.Lua);
            codeEditorControl.Caret.Change += new EventHandler(this.OnCarChange);
        }

        //�����л��е�ʱ�򣬹ر���ʾ�б�
        private int lastline = 0; 

        private void OnCarChange(object sender, EventArgs e)
        {
            if (lastline != codeEditorControl.Caret.Position.Y)
            {
                if(inputState != InputState.NORMAL)
                {
                    codeEditorControl.AutoListVisible = false;
                    codeEditorControl.InfoTipVisible = false;
                    inputState = InputState.NORMAL;
                }
            }
            lastline = codeEditorControl.Caret.Position.Y;


            //ˢ����ͷ��functionlist
            Fireball.Syntax.Segment seg = codeEditorControl.Caret.CurrentSegment();
            string strNs = GetNameSpace(seg);
            this.strNs = strNs;
            foreach (FunctionList fl in cmbFunctionList.Items)
            {
                if (fl.FunctionName == strNs)
                {
                    cmbFunctionList.SelectedItem = fl;
                    break;
                }
            }
        }        
    }

    public enum InputState
    {
        NORMAL, // ����
        FUNCTIONLIST_OPEN, // �¼��б���ʾ
        OBJECTLIST_OPEN, // �����б���ʾ
        PARMSLIST_OPEN, // �����б���ʾ
        INTELLISENCE_OPEN // ���ܸ�֪�б���ʾ
    }

    /************************************************************************/
    /* �¼����                                                             */
    /************************************************************************/
    public delegate void QueryEventsHandle(object sender, QueryEventsArgs e);   /*�¼�*/
    public delegate void QueryObjectHandle(object sender, QueryEventsArgs e);   /*����*/
    public delegate void QueryParmsHandle(object sender, QueryEventsArgs e);    /*����*/
    public delegate void OnChangeHandle(object sender, EventArgs e);
    public delegate void BreakPointAdded(object sender, RowEventArgs e);
    public delegate void BreakPointRemoved(object sender, RowEventArgs e);
    public delegate void WordMouseHover(object sender, ref WordMouseEventArgs e);

    public class QueryEventsArgs : EventArgs
    {
        // ����¼��б�
        public leEventConnection leec;

        // ��ų�Ա�б�Value���leTableItem
        public Hashtable leTable = new Hashtable();
        
        // ��Ų����б�ÿ��Item���һ���ַ����� "prop,e"
        public List<string> parms_list = new List<string>(); 

        // ���������·������ _G.string.len
        public string keyword = "";

        //������
        public string szCode = "";
        public int nPos = 0;
        public int nOverLen = 0;
    };

    /************************************************************************/
}
