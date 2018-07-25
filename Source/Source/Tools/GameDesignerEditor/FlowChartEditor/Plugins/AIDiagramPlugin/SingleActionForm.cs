using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Collections;
using System.Windows.Forms;
using System.Data.SqlClient;

using DevComponents.DotNetBar;
using DevComponents.AdvTree;
using DevComponents.DotNetBar.Controls;

using DataInteractor;
using DBManager;

namespace AIDiagramPlugin
{
    public partial class SingleActionForm : Office2007Form
    {
        private bool editSuccess = false; // �Ƿ�༭�ɹ�
        private bool beginEdit = false; // �Ƿ��Ѿ���ʼ�༭

        private LabelX[] labelArray; 
        private ComboBoxEx[] comboBoxArray;
        private TextBox[] returnValueBoxArray;
        private CheckBoxX[] checkBoxArray;
        private AI_SingleAction singleAction;  // ��ǰ����

        private int graphElementID; // ��ǰ�༭ͼԪ��ID
        private bool inActionGroup; // �Ƿ��ڶ�������
        private int groupIndex; // ��������
        private Hashtable actionTable = new Hashtable(); // �������ݱ�

        private char[] splitLineArray = new char[] { '\n' };
        private char[] splitReturnValueArray = new char[] { ',' };                

        /// <summary>
        /// ���캯��
        /// </summary>
        public SingleActionForm()
        {
            InitializeComponent();            
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ʼ���ؼ�           
            codeEditBox.ForWho = "GameLuaEditor";
            labelArray = new LabelX[] { labelX2, labelX3, labelX4, labelX5, labelX6 };
            comboBoxArray = new ComboBoxEx[] { comboBoxEx1, comboBoxEx2, comboBoxEx3, comboBoxEx4, comboBoxEx5 };
            checkBoxArray = new CheckBoxX[] { checkBoxX1, checkBoxX2, checkBoxX3, checkBoxX4, checkBoxX5 };
            returnValueBoxArray = new TextBox[] { returnValue1Box, returnValue2Box, returnValue3Box };   

            foreach (AI_Action action in ActionTable.ActionItems)
            {
                Node classNode = null;

                foreach (Node node in actionTree.Nodes)
                {
                    if (node.Text == action.ActionType)
                    {
                        classNode = node;
                        break;
                    }
                }

                if (classNode == null)
                {
                    classNode = new Node();
                    classNode.Text = action.ActionType;
                    actionTree.Nodes.Add(classNode);
                }

                Node newNode = new Node();
                newNode.Text = action.Name;
                newNode.Tag = action.DBID;
                classNode.Nodes.Add(newNode);

                AI_Action newAction = action.Clone() as AI_Action;
                actionTable[action.DBID] = newAction;
                actionComboBox.Items_Add(newAction);
            }

            // չ�����еķ�����
            foreach (Node classNode in actionTree.Nodes)
            {
                classNode.Expand();
            }

            beginEdit = true;
        }

        /// <summary>
        /// ��ǰ�����Ƿ��ڶ�������
        /// </summary>
        public bool InActionGroup
        {
            set
            {
                inActionGroup = value;
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        public int GroupIndex
        {
            set
            {
                groupIndex = value;
            }
        }

        /// <summary>
        /// ��ǰ�༭ͼԪ��ID
        /// </summary>
        public int GraphElementID
        {
            set
            {
                graphElementID = value;
            }
        }

        /// <summary>
        /// �Ƿ�༭�ɹ�
        /// </summary>
        public bool EditSuccess
        {
            get
            {
                return editSuccess;
            }
        }

        /// <summary>
        /// ˢ������
        /// </summary>
        /// <param name="sigaction">AId����</param>
        public void Reload(AI_SingleAction sigaction)
        {
            actionTree.SelectedNode = null;
            actionComboBox.SelectedItem = null;
            editSuccess = false;

            // ���洫�����Ĳ���
            this.singleAction = sigaction;
            AI_Action action = sigaction.Action;                      

            // ���ļ��ڵ��Զ���ű�����
            string actionNameID = graphElementID.ToString();

            if (inActionGroup)
            {
                actionNameID = string.Format("{0}_{1}", graphElementID.ToString(), groupIndex.ToString());
            }

            if (action.ActionID != null)
            {
                int actionID = int.Parse(action.ActionID);
                string[] returnValueArray;

                if (actionID > 2000) // ����ͼ���Զ��嶯��
                {
                    actionNameBox.Text = action.Name;
                    codeEditBox.Text = action.Info;

                    returnValueArray = action.rettype.Split(splitReturnValueArray, StringSplitOptions.RemoveEmptyEntries);

                    for (int i = 0; i < 3; i++)
                    {
                        if (i < returnValueArray.Length)
                        {
                            returnValueBoxArray[i].Text = returnValueArray[i];
                        }
                        else
                        {
                            returnValueBoxArray[i].Text = "";
                        }
                    }                        
                    
                    tabControl1.SelectedTabIndex = 1;
                }
                else // ϵͳ���Զ��嶯��
                {
                    tabControl1.SelectedTabIndex = 0;

                    // ѡ�е�ǰ����
                    foreach (Node classNode in actionTree.Nodes)
                    {
                        if (classNode.Text == action.ActionType)
                        {
                            foreach (Node childNode in classNode.Nodes)
                            {
                                if (childNode.Tag.ToString() == action.DBID)
                                {
                                    actionTree.SelectedNode = childNode;
                                    break;
                                }
                            }

                            break;
                        }
                    }                  

                    // �Զ��嶯�����ϳ�ʼֵ
                    actionNameBox.Text = string.Format("�Զ��嶯��{0}", actionNameID);
                    returnValue1Box.Text = "�ɹ�";
                    returnValue2Box.Text = "";
                    returnValue3Box.Text = "";

                    StringBuilder code = new StringBuilder();
                    code.Append(string.Format("function CustomAction{0}(npc, actionData)\r\n", actionNameID));
                    code.Append("    -- �����Զ��嶯��ʵ�ֽű�\r\n\r\n");
                    code.Append("    return 1\r\n");
                    code.Append("end");
                    codeEditBox.Text = code.ToString();
                }
            }
            else
            {
                tabControl1.SelectedTabIndex = 0;               

                // �Զ��嶯�����ϳ�ʼֵ
                actionNameBox.Text = string.Format("�Զ��嶯��{0}", actionNameID);
                returnValue1Box.Text = "�ɹ�";
                returnValue2Box.Text = "";
                returnValue3Box.Text = "";

                StringBuilder code = new StringBuilder();
                code.Append(string.Format("function CustomAction{0}(npc, actionData)\r\n", actionNameID));
                code.Append("    -- �����Զ��嶯��ʵ�ֽű�\r\n\r\n");
                code.Append("    return 1\r\n");
                code.Append("end");
                codeEditBox.Text = code.ToString();
            }
        }

        /// <summary>
        /// ���ű��Ƿ���Ч
        /// </summary>
        /// <param name="content">�ű�����</param>
        /// <returns>�Ƿ���Ч</returns>
        private bool CheckScriptValid(string content)
        {
            bool valid = false;
            bool endSearch = false;
            bool findEnd = false;
            string[] lines = content.Split(splitLineArray, StringSplitOptions.RemoveEmptyEntries);

            for (int i = lines.Length - 1; i >= 0; i--)
            {
                string line = lines[i].TrimStart(new char[] { ' ', '\t' } ).TrimEnd(new char[] { '\r', ';', ' ', '\t' });

                switch (line)
                {
                    case "":
                        {
                            break;
                        }
                    case "end":
                        {
                            findEnd = true;
                            break;
                        }
                    case "return 1":
                        {
                            if (findEnd)
                            {
                                valid = true;
                            }
                            
                            break;
                        }
                    case "return 2":
                        {
                            if (findEnd)
                            {
                                valid = true;
                            }
                            
                            break;
                        }
                    case "return 3":
                        {
                            if (findEnd)
                            {
                                valid = true;
                            }
                            
                            break;
                        }
                    default:
                        {
                            endSearch = true;
                            break;
                        }
                }

                if (endSearch)
                {
                    break;
                }
            }

            return valid;
        }

        /// <summary>
        /// ��������Ч��
        /// </summary>
        /// <param name="parameterType">��������</param>
        /// <param name="parameterValue">����ֵ</param>
        /// <param name="varType">�Ƿ��Ǳ�������</param>
        /// <returns>�����Ƿ���Ч</returns>
        private bool CheckParameterValid(int parameterType, string parameterValue, bool varType)
        {
            bool valid = true;            

            switch (parameterType)
            {
                case -1: // ����
                    {
                        if (string.IsNullOrEmpty(parameterValue))
                        {
                            valid = false;
                        }
                        else
                        {
                            if (!varType && !(parameterValue.StartsWith("<") && parameterValue.EndsWith(">")))
                            {
                                int testValue = 0;

                                if (!int.TryParse(parameterValue, out testValue))
                                {
                                    valid = false;
                                }
                            }
                        }

                        break;
                    }
                case -2: // �ַ���
                    {
                        // �ַ��������ֵ
                        break;
                    }
                default: // ö��ֵ
                    {
                        if (string.IsNullOrEmpty(parameterValue))
                        {
                            valid = false;
                        }

                        break;
                    }
            }

            return valid;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            AI_Action newAction = null;

            if (tabControl1.SelectedTabIndex == 0) // ѡ��ϵͳAction
            {
                Node currentNode = actionTree.SelectedNode;

                if (currentNode != null && currentNode.Level == 1)
                {
                    AI_Action selectedAction = actionTable[currentNode.Tag.ToString()] as AI_Action;                

                    if (selectedAction != null)
                    {                   
                        // ��һ�����������������ȷ��
                        for (int i = 0; i < selectedAction.Args.Count; i++)
                        {
                            if (!CheckParameterValid(selectedAction.Args[i].m_type, comboBoxArray[i].Text, checkBoxArray[i].Checked))
                            {
                                MessageBox.Show(string.Format("{0}����ֵ��Ч��", labelArray[i].Text), "�������",
                                                MessageBoxButtons.OK, MessageBoxIcon.Error);
                                comboBoxArray[i].Focus();

                                return;
                            }
                        }

                        // �ڶ��������������ݴ浽Action��
                        newAction = selectedAction.Clone() as AI_Action;

                        for (int i = 0; i < newAction.Args.Count; i++)
                        {
                            AI_Parm parm = newAction.Args[i];
                            object objSelect = comboBoxArray[i].SelectedItem;

                            if (comboBoxArray[i].Text.StartsWith("<") && comboBoxArray[i].Text.EndsWith(">")) // ģ�����
                            {
                                if (checkBoxArray[i].Checked)
                                {
                                    MessageBox.Show("ģ���������Ϊ�������ͣ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    comboBoxArray[i].Focus();

                                    return;
                                }

                                parm.m_IsTemplateParm = true;
                                AI_TemplateParmItem tpitem = objSelect as AI_TemplateParmItem;

                                if (objSelect == null)
                                {
                                    MessageBox.Show("��������", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    comboBoxArray[i].Focus();

                                    return;
                                }

                                int index = AI_TemplateParmTable.GetAIParamTemplateTable().GetAIParamIndex(tpitem.Name);
                                parm.SetTemplateValue(index.ToString());
                            }
                            else // ��ģ�����
                            {
                                parm.m_IsTemplateParm = false;

                                if (objSelect == null) // ����ģ�����ѡ�ģ��Ǿ����ַ�������������
                                {
                                    parm.m_Value = comboBoxArray[i].Text;
                                }
                                else if (objSelect is EnumItem) // ��ö��
                                {
                                    EnumItem ei = objSelect as EnumItem;
                                    parm.m_Value = ei.m_asParentsPos.ToString();
                                }
                            }

                            // ��¼���Ƿ��Ǳ�������
                            parm.IsVar = checkBoxArray[i].Checked;
                        }                                                
                    }
                }
                else
                {
                    MessageBox.Show("����ѡ������", "�������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }                
            }
            else // ѡ���Զ���Action
            {
                string functionName = actionNameBox.Text;
                string functionText = codeEditBox.Text;
                string returnValue = "";

                if (functionName == "")
                {
                    MessageBox.Show("����������Ϊ�գ�", "�������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                if (functionText == "")
                {
                    MessageBox.Show("�Զ���ű�����Ϊ�գ�", "�������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                for (int i = 0; i < 3; i++)
                {
                    if (returnValueBoxArray[i].Text.Trim() != "")
                    {
                        returnValue += string.Format("{0},", returnValueBoxArray[i].Text.Trim());
                    }
                    else
                    {
                        break;
                    }
                }
                returnValue = returnValue.TrimEnd(new char[] { ',' });

                if (returnValue == "")
                {
                    MessageBox.Show("������Ҫ��һ������ֵ��", "�������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }               
                
                int actionID = graphElementID + 2000;

                if (inActionGroup) // �������ڵ��Զ��嶯����Ҫ���·���ID
                {
                    actionID = (graphElementID + 2000) * 10 + groupIndex;
                }

                string englishName = GetFunctionName(functionText);

                if (!CheckScriptValid(functionText)) // ���ű���Ч��
                {
                    MessageBox.Show("�ű������������ȷ����ִ�н�����磺return 1����", "�ű����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }

                if (!AIDiagramHelper.CheckScript(codeEditBox, functionText)) // û��ͨ���ű����
                {
                    if (MessageBox.Show("û��ͨ���ű���飬�Ƿ����±༭�ű���", "�ű����", 
                                        MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
                    {
                        return;
                    }
                }

                newAction = new AI_Action();
                newAction.ActionID = actionID.ToString();
                newAction.Name = functionName;
                newAction.Info = functionText;
                newAction.Ename = englishName;
                newAction.Args = new List<AI_Parm>();
                newAction.rettype = returnValue;
            }

            singleAction.Action = newAction;
            editSuccess = true;
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// ��ȡ������
        /// </summary>
        /// <param name="content">ȫ���ַ���</param>
        /// <returns>������</returns>
        private string GetFunctionName(string content)
        {
            string functionName = "";

            string[] lines = content.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < lines.Length; i++)
            {
                string line = lines[i].Trim(new char[] { '\r', ' ' });
                if(line.StartsWith("function"))
                {
                    line = line.Replace("function", "");
                    int index = line.IndexOf("(");
                    if(index > 0)
                    {
                        functionName = line.Substring(0, index).Trim();
                    }

                    break;
                }
            }

            return functionName;
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// ��ʾAI����������
        /// </summary>
        /// <param name="action">AI����</param>
        private void ShowActionData(AI_Action action)
        {            
            this.textBoxX1.Text = action.Info;

            for (int i = 0 ; i < 5; i++)
            {
                // ����Ĳ�������ֹ��
                if (i >= action.Args.Count)
                {
                    labelArray[i].Text = "-";
                    comboBoxArray[i].Text = "";
                    comboBoxArray[i].Enabled = false;
                    checkBoxArray[i].Checked = false;
                    checkBoxArray[i].Enabled = false;
                }
                else
                {
                    AI_Parm parm = action.Args[i];

                    // ����1����ʼ��ö�٣��������б� �����ö�٣������ö���������
                    comboBoxArray[i].Enabled = true;
                    comboBoxArray[i].Items.Clear();

                    if (parm.m_type > 0)
                    {
                        comboBoxArray[i].Items.AddRange(EnumTable.GetEnumTypeByID(parm.m_type.ToString()).Items.ToArray());
                    }

                    // �ı�ǩ
                    string strLab = null ;

                    switch (parm.m_type)
                    {
                        case -1:
                            {
                                strLab = "����";

                                // Ŀǰֻ֧�����ֵı�������
                                checkBoxArray[i].Enabled = true;

                                break;
                            }
                        case -2:
                            {
                                strLab = "�ַ���";

                                // ��ʱ��֧���ַ����ı�������
                                checkBoxArray[i].Enabled = false;

                                break;
                            }
                        default: 
                            {
                                strLab = "ö��";

                                // ��ʱ��֧��ö�ٵı�������
                                checkBoxArray[i].Enabled = false;

                                break;
                            }
                    }

                    if (parm.Name.Length > 10)
                    {
                        labelArray[i].Text = string.Format("{0}... ({1})", parm.Name.Substring(0, 8), strLab);
                    }
                    else
                    {
                        labelArray[i].Text = string.Format("{0} ({1})", parm.Name, strLab);
                    }

                    // ����2����ʼ�� ģ�����������������������͵�ģ��������ͼӽ�����    
                    comboBoxArray[i].Items.AddRange(AI_TemplateParmTable.GetAIParamTemplateTable().AIParamItemList.ToArray());

                    // ����3����ֵ
                    if (parm.Value != null)
                    {
                        comboBoxArray[i].Text = parm.Value.ToString();
                    }
                    else
                    {
                        comboBoxArray[i].Text = "";
                    }

                    // ����4����ȡ��������
                    checkBoxArray[i].Checked = parm.IsVar;
                }                
            } 
        }

        /// <summary>
        /// ѡ����ĳ��Action
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void RefreshActionInfo(object sender, AdvTreeNodeEventArgs e)
        {
            Node currentNode = actionTree.SelectedNode;

            if (currentNode != null && currentNode.Level == 1)
            {
                AI_Action act = actionTable[currentNode.Tag.ToString()] as AI_Action;

                if (act != null)
                {
                    if (act.Equals(singleAction.Action))
                    {
                        ShowActionData(singleAction.Action);
                    }
                    else
                    {
                        ShowActionData(act);                        
                    }                
                }
            }
            else
            {
                for (int i = 0; i < 5; i++)
                {
                    labelArray[i].Text = "-";
                    comboBoxArray[i].Text = "";
                    comboBoxArray[i].Enabled = false;     
                }
            }                               
        }

        /// <summary>
        /// ��ѯ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void codeEditBox_queryObjects(object sender, luaEditor.QueryEventsArgs e)
        {
            AIDiagramHelper.QueryObjects(sender, e);            
        }

        /// <summary>
        /// ����ر��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void SingleActionForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // e.Cancel = true;
            // this.Visible = false;
        }

        /// <summary>
        /// ѡ��ѡ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void actionComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            AI_Action currentAction = actionComboBox.Items_SelectedItem as AI_Action;

            if (beginEdit && currentAction != null)
            {
                string actionID = currentAction.DBID;
                Node selectedNode = null;

                foreach (Node classNode in actionTree.Nodes)
                {
                    foreach (Node actionNode in classNode.Nodes)
                    {
                        if (actionNode.Tag.ToString() == actionID)
                        {
                            selectedNode = actionNode;
                            break;
                        }
                    }

                    if (selectedNode != null)
                    {
                        break;
                    }
                }

                actionTree.SelectedNode = selectedNode;
            }                       
        }
    }
}