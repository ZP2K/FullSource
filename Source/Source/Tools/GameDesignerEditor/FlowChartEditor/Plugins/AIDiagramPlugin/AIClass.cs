using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Data;
using DataInteractor;

namespace AIDiagramPlugin
{
    /// <summary>
    /// AI�¼�
    /// </summary>
    [Serializable]
    public class AI_Event : IReloadable, ICloneable
	{
        internal string DBID = ""; // ���ݿ� [AI_Event_Define].id
        public string eventid = ""; // �¼���ID
        public string Name = ""; // �¼���������
        public string Ename = ""; // �¼���Ӣ����
        public string Info = ""; // �¼�������

        /// <summary>
        /// ˢ������
        /// </summary>
        public void Reload()
        {
            if (!string.IsNullOrEmpty(this.Name))
            {
                AI_Event aiEvent = EventTable.FindItemByDBID(this.DBID);

                if (aiEvent != null)
                {
                    this.Ename = aiEvent.Ename;
                    this.Name = aiEvent.Name;
                    this.Info = aiEvent.Info;
                    this.eventid = aiEvent.eventid;
                }
            }
        }

        /// <summary>
        /// ǿ��ˢ������
        /// </summary>
        public void ForceReload()
        {
            Reload();
        }

        /// <summary>
        /// ����Ƿ���Ҫ���±༭
        /// </summary>
        /// <returns>�Ƿ���Ҫ���±༭</returns>
        public bool RequestEdit()
        {
            return false;
        }

        /// <summary>
        /// ��ȡ�ַ������
        /// </summary>
        /// <returns>�ַ������</returns>
        public override string ToString()
        {
            if (int.Parse(this.eventid) <= 1000)
            {
                return Name;
            }
            else
            {
                return string.Format("{0} [{1}]", Name, this.eventid);                
            }
        }

        /// <summary>
        /// �Ƚ϶����Ƿ����
        /// </summary>
        /// <param name="obj">�Ƚ϶���</param>
        /// <returns>�Ƿ����</returns>
        public override bool Equals(object obj)
        {
            AI_Event aiEvent = obj as AI_Event;

            if (aiEvent != null)
            {
                return (this.DBID == aiEvent.DBID);
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// ��ȡ��ϣ��
        /// </summary>
        /// <returns>��ϣ��</returns>
        public override int GetHashCode()
        {
            return int.Parse(this.DBID);
        }

        /// <summary>
        /// ���ظ���
        /// </summary>
        /// <returns>���ƵĶ���</returns>
        public object Clone()
        {
            AI_Event newEvent = new AI_Event();
            newEvent.DBID = this.DBID;
            newEvent.Name = this.Name;
            newEvent.Ename = this.Ename;
            newEvent.Info = this.Info;
            newEvent.eventid = this.eventid;

            return newEvent;
        }
	}

    /// <summary>
    /// AIAction
    /// </summary>
    [Serializable]
	public class AI_Action : ICloneable, IReloadable
	{
		internal string DBID; // ���ݿ�ID
        public string ActionID; // ������ID
        public string Name; // ������������
        public string Ename; // ������Ӣ����
        public string Info; // ������������Ϣ
        public string ActionType; // ����������
		internal string rettype; // ����ֵ����ĺ��壬���ַ������棬���Ÿ���
        public List<AI_Parm> Args = new List<AI_Parm>(); //���涯������Ĳ�����ֵ

        // ����AI����
        public object Clone()
        {
            AI_Action action = new AI_Action();
            action.DBID = this.DBID;
            action.ActionID = this.ActionID;
            action.Name = this.Name;
            action.Ename = this.Ename;
            action.Info = this.Info;
            action.rettype = this.rettype;
            action.ActionType = this.ActionType;

            foreach (AI_Parm parm in this.Args)
            {
                action.Args.Add(parm.Clone() as AI_Parm);
            }

            return action;
        }

        /// <summary>
        /// �Ƚ϶���
        /// </summary>
        /// <param name="obj">����</param>
        /// <returns>�Ƿ����</returns>
        public override bool Equals(object obj)
        {
            bool result = false;

            AI_Action action = obj as AI_Action;

            if (action != null && this.DBID == action.DBID)
            {
                result = true;
            }

            return result;
        }

        /// <summary>
        /// ���ɹ�ϣ��
        /// </summary>
        /// <returns>��ϣ��</returns>
        public override int GetHashCode()
        {
            int hashCode = int.Parse(this.DBID);

            return hashCode;
        }

        /// <summary>
        /// �����ķ���ֵ����
        /// </summary>
        public List<string> RetList
        {
            get
            {
                string rettype_add = rettype + ",,";
                string[] strRetList = rettype_add.Split(new string[] { ","}, StringSplitOptions.None);
                List<string> list = new List<string>();

                for(int i = 1; i <= 3; i++) // ��������
                {
                    if (strRetList[i-1] != "")
                    {
                        list.Add(i.ToString() + ":" + strRetList[i - 1]);
                    }
                }

                return list;
            }
        }

        /// <summary>
        /// �����ַ����������
        /// </summary>
        /// <returns>�ַ������</returns>
        public override string ToString()
        {
            return string.Format("{0} ���� {1}", ActionType, Name);
        }

        /// <summary>
        /// ˢ�¶�������
        /// </summary>
        public void Reload()
        {
            AI_Action action = ActionTable.FindItemByDBID(this.DBID);

            if (action != null)
            {
                this.ActionID = action.ActionID;
                this.Name = action.Name;
                this.Ename = action.Ename;
                this.Info = action.Info;
                this.ActionType = action.ActionType;
                this.rettype = action.rettype;
            }
        }

        /// <summary>
        /// ǿ��ˢ������
        /// </summary>
        public void ForceReload()
        {
            AI_Action action = ActionTable.FindItemByDBID(this.DBID);

            if (action != null)
            {
                this.ActionID = action.ActionID;
                this.Name = action.Name;
                this.Ename = action.Ename;
                this.Info = action.Info;
                this.ActionType = action.ActionType;
                this.Args = action.Args;                
                this.rettype = action.rettype;
            }
        }

        /// <summary>
        /// ����Ƿ���Ҫ���±༭
        /// </summary>
        /// <returns>�Ƿ���Ҫ���±༭</returns>
        public bool RequestEdit()
        {
            bool result = false;
            AI_Action action = ActionTable.FindItemByDBID(this.DBID);

            if (action != null)
            {                           
                if (this.Args.Count == action.Args.Count)
                {
                    for (int i = 0; i < this.Args.Count; i++)
                    {
                        AI_Parm parameter1 = this.Args[i];
                        AI_Parm parameter2 = action.Args[i];

                        if (parameter1.m_type != parameter2.m_type)
                        {
                            result = true;
                            break;
                        }
                    }
                }
                else
                {
                    result = true;
                }                                                                           
            }
            
            return result;
        }
	}
    
    /// <summary>
    /// AI����
    /// </summary>
    [Serializable]
    public class AI_Parm : ICloneable
    {
        public string Name; // ������
        internal string m_Value; // �����������ֵ   ģ������Ļ��������ģ�������keyID�������ַ����Ļ�����ֵ��ö�ٵĻ�������ĳ��ö�ٵĵڼ���ֵ��0��ʼ��
        internal int m_type; // ��¼���������ʲô��ǿ�ƹ涨-2 Ϊ �ַ���,-1Ϊ���֣�����Ϊö�١�
        internal bool m_IsTemplateParm = false; // �ǲ��Ǵ���ģ�����
        private bool isVar = false; // �Ƿ��Ǳ�������
        
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="Name">������</param>
        /// <param name="mValue">����ֵ</param>
        /// <param name="mType">��������</param>
        /// <param name="istp">�Ƿ���ģ�����</param>
        public AI_Parm(string Name, string mValue, int mType, bool istp)
        {
            this.Name = Name;
            this.m_Value = mValue;
            this.m_type = mType;
            this.m_IsTemplateParm = istp;
        }

        /// <summary>
        /// ���ò�����ֵΪģ�����
        /// </summary>
        /// <param name="keyID">ģ�����ID</param>
        public void SetTemplateValue(string keyID)
        {
            this.m_Value = keyID;
            this.m_IsTemplateParm = true;
        }

        /// <summary>
        /// �Ƿ��Ǳ�������
        /// </summary>
        public bool IsVar
        {
            set
            {
                isVar = value;
            }

            get
            {
                return isVar;
            }
        }

        /// <summary>
        /// ����ֵ
        /// </summary>
        public IAI_Arg Value
        {
            get
            {
                IAI_Arg arg = null;

                if (m_IsTemplateParm) // ����ģ�����
                {
                    // �ڱ�����ͼ��������ȡ��������
                    string[] infoArray = AI_TemplateParmTable.GetAIParamTemplateTable().AIParamInfoList[int.Parse(m_Value) - 1]; // ��ͼ�ı���Ǵ�1��ʼ��
                    arg = new AI_TemplateParmItem(infoArray[0], infoArray[1]);                    
                }
                else // �����������
                {
                    if (m_type == -1 || m_type == -2)
                    {
                        arg = new AI_ArgValue();
                        (arg as AI_ArgValue).Value = m_Value == null ? "" : m_Value;
                        (arg as AI_ArgValue).type = m_type;
                    }
                    else
                    {
                        EnumItem ei = null;

                        foreach (EnumType et in EnumTable.EnumTypes)
                        {
                            if (et.DBID == this.m_type.ToString())
                            {
                                ei = et.Items[Int32.Parse(m_Value.ToString())];
                            }
                        }

                        arg = ei;
                    }                    
                }

                return arg;
            }
        }

        /// <summary>
        /// ���Ʋ���
        /// </summary>
        /// <returns>��������</returns>
        public object Clone()
        {
            AI_Parm newParm = new AI_Parm(this.Name, this.m_Value, this.m_type, this.m_IsTemplateParm);
            newParm.IsVar = isVar;

            return newParm;
        }
    }    

    /// <summary>
    /// ��ֵ/�ַ���
    /// </summary>
    [Serializable]
	public class AI_ArgValue : IAI_Arg
	{
		internal string Value; // �������֡��ַ������ַ�����+���ţ���
        internal int type;

        public string GetCode()
        {
            if (type == -1)
            {
                return Value == null ? "" : Value;
            }
            else
            {
                // ��ӵ�ls��������ID
                string argValue = DBManager.DataBaseManager.GetDataBaseManager().InsertIntoLS(Value);

                return argValue;
            }
        }

        public override string ToString()
        {
            return Value;
        }        
	}

    /// <summary>
    /// ģ�����
    /// </summary>
    [Serializable]
	public class AI_TemplateParmItem : IAI_Arg
	{
        private string name; // Ӣ������
        private string chineseName; // ��������
        
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="name">Ӣ������</param>
        /// <param name="chineseName">��������</param>
        public AI_TemplateParmItem(string name, string chineseName)
        {
            this.name = name;
            this.chineseName = chineseName;
        }

        /// <summary>
        /// ����
        /// </summary>
        public string Name
        {
            get
            {
                return name;
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        public string ChineseName
        {
            get
            {
                return chineseName;
            }
        }

        /// <summary>
        /// ��ȡ�����ַ���
        /// </summary>
        /// <returns>�����ַ���</returns>
        public string GetCode()
        {
            return string.Format("param['{0}']", name);
        }

        /// <summary>
        /// ��ȡ�ַ������
        /// </summary>
        /// <returns>�ַ������</returns>
        public override string ToString()
        {
            return  string.Format("<{0}>", chineseName);
        }
	}

    /// <summary>
    /// ö���ͬʱҲ�����Ƕ����Ĳ���
    /// </summary>
    [Serializable]
    public class EnumItem : IAI_Arg, IReloadable
    {
        internal string m_parentDBID; // ö�����͵�ID
        internal int m_asParentsPos; // �Լ���ö�����͵ĵڼ�����������0��ʼ

        [NonSerialized]
        public string Name;

        [NonSerialized]
        public string Ename;

        [NonSerialized]
        public string Parent_Ename;

        internal string Value;  // ��ֵ�����Ǻ����Ҫ����
        
        /// <summary>
        /// ��ȡö������
        /// </summary>
        public EnumType Parent
        {
            get 
            { 
                return null; 
            }
        }

        /// <summary>
        /// ��ȡ����ֵ
        /// </summary>
        /// <returns>����ֵ</returns>
        public string GetCode()
        {
            return Value;
        }

        /// <summary>
        /// ˢ������
        /// </summary>
        public void Reload()
        {
            EnumType etype = EnumTable.GetEnumTypeByID(m_parentDBID);
            EnumItem item = etype.Items[m_asParentsPos];
            this.Name = item.Name;
            this.Ename = item.Ename;
            this.Parent_Ename = item.Parent_Ename;
        }

        /// <summary>
        /// ǿ��ˢ������
        /// </summary>
        public void ForceReload()
        {
            Reload();
        }

        /// <summary>
        /// ����Ƿ���Ҫ���±༭
        /// </summary>
        /// <returns>�Ƿ���Ҫ���±༭</returns>
        public bool RequestEdit()
        {
            return false;
        }

        /// <summary>
        /// ��ȡ�ַ������
        /// </summary>
        /// <returns>�ַ������</returns>
        public override string ToString()
        {
            return Name;
        }
    }

    /// <summary>
    /// ģ�������
    /// </summary>
    [Serializable]
    public class AI_TemplateParmTable
    {
        private List<string[]> aiParamInfoList = new List<string[]>(); // ģ�������Ϣ����
        private static AI_TemplateParmTable aiTemplateParamTable;
        private List<AI_TemplateParmItem> aiParamItemList; // AI��������

        /// <summary>
        /// ���캯��
        /// </summary>
        public AI_TemplateParmTable()
        {
            // ��ʼ��ģ�������Ϣ����
            aiParamInfoList.Add(new string[] { "IsPositive", "�Ƿ�������" });
            aiParamInfoList.Add(new string[] { "AlertRange", "���䷶Χ" });
            aiParamInfoList.Add(new string[] { "AlertOverTime", "����״̬��ʱ" });
            aiParamInfoList.Add(new string[] { "AttackRange", "ս����Χ" });
            aiParamInfoList.Add(new string[] { "EscapeTime", "����ʱ��" });
            aiParamInfoList.Add(new string[] { "EscapeRange", "���ܷ�Χ" });
            aiParamInfoList.Add(new string[] { "EscapeInterval", "���ܵ������" });
            aiParamInfoList.Add(new string[] { "WanderRange", "�ε���Χ" });
            aiParamInfoList.Add(new string[] { "WanderDistance", "�ε�����" });
            aiParamInfoList.Add(new string[] { "WanderInterval", "�ε�����" });
            aiParamInfoList.Add(new string[] { "PatrolPathID", "Ѳ��·��ID" });
            aiParamInfoList.Add(new string[] { "EscapeLife", "����Ѫ��" });
            aiParamInfoList.Add(new string[] { "PatrolOrderIndex", "Ѳ������λ��" });
            aiParamInfoList.Add(new string[] { "ReviveTeammate", "����ʱ�������" });
            aiParamInfoList.Add(new string[] { "IsAutoTurn", "�Ի�ʱ�������" });
            aiParamInfoList.Add(new string[] { "IsSnarlInFight", "����ս���Ƿ���" });
            aiParamInfoList.Add(new string[] { "NpcSceneType", "NPC��������" });
        }

        /// <summary>
        /// ��ȡAIģ�������Ϣ��
        /// </summary>
        /// <returns>AIģ�������Ϣ��</returns>
        public static AI_TemplateParmTable GetAIParamTemplateTable()
        {
            if (aiTemplateParamTable == null)
            {
                aiTemplateParamTable = new AI_TemplateParmTable();
            }

            return aiTemplateParamTable;
        }
        
        /// <summary>
        /// AI��������
        /// </summary>
        public List<AI_TemplateParmItem> AIParamItemList
        {
            get
            {
                if (aiParamItemList == null)
                {
                    aiParamItemList = new List<AI_TemplateParmItem>();

                    foreach (string[] infoArray in aiParamInfoList)
                    {
                        AI_TemplateParmItem aiParamItem = new AI_TemplateParmItem(infoArray[0], infoArray[1]);
                        aiParamItemList.Add(aiParamItem);
                    }
                }

                return aiParamItemList;
            }            
        }

        /// <summary>
        /// ģ�������Ϣ����
        /// </summary>
        public List<string[]> AIParamInfoList
        {
            get
            {
                return aiParamInfoList;
            }
        }

        /// <summary>
        /// ��ȡAIģ������ı��
        /// </summary>
        /// <param name="paramName">��������</param>
        /// <returns>���</returns>
        public int GetAIParamIndex(string paramName)
        {
            int index = -1;

            for (int i = 0; i < aiParamInfoList.Count; i++)
            {
                if (aiParamInfoList[i][0] == paramName)
                {
                    index = i + 1; // ���ݾ�ͼ����Ŵ�1��ʼ
                    break;
                }
            }

            return index;
        }
    }

    //  ���²��������л�

    /// <summary>
    /// Action�Ĳ���ֵ
    /// </summary>
	public interface IAI_Arg
	{
        string GetCode();
	}

    /// <summary>
    /// ��ˢ�½ӿ�
    /// </summary>
    public interface IReloadable
    {
        void Reload();
        void ForceReload();
        bool RequestEdit();
    }

    /// <summary>
    /// ö�����ͣ��������������е�ö����
    /// </summary>
    public class EnumType
    {
        internal string DBID;
        public string Name;
        public string Ename;
        public string Info;
        internal string[] m_values;
        private List<EnumItem> m_valueList = null;

        public List<EnumItem> Items
        {
            get
            {
                //�Զ���ʼ��m_valueList�ٷ���
                if (m_valueList == null)
                {
                    m_valueList = new List<EnumItem>();
                    int pos = 0;
                    foreach (string line in this.m_values)
                    {
                        string[] strList = line.Split(new string[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                        EnumItem enu = new EnumItem();
                        enu.m_parentDBID = DBID;
                        enu.m_asParentsPos = pos;
                        enu.Name = strList[0].ToString();
                        enu.Ename = strList[1].ToString();
                        enu.Value = strList[2].ToString();
                        enu.Parent_Ename = this.Ename;
                        m_valueList.Add(enu);
                        pos++;
                    }
                }
                return m_valueList;
            }
        }

        /// <summary>
        /// ��ȡ�ַ������
        /// </summary>
        /// <returns>�ַ������</returns>
        public override string ToString()
        {
            return Name;
        }
    }

    /// <summary>
    /// ö���б��������е�ö������
    /// </summary>
    class EnumTable 
    {
        private static bool inited = false;
        private static string[] parameterSpliter = new string[] { "\r\n" };
        private static List<EnumType> enumTypeList = new List<EnumType>();
        private static Dictionary<string, EnumType> enumTypeDictionary = new Dictionary<string, EnumType>();

        public static List<EnumType> EnumTypes
        {
            get
            {
                Init();
                return enumTypeList;
            }
        }

        private static void Init()
        {
            if (!inited)
            {
                DataTable infoTable = DBManager.DataBaseManager.GetDataBaseManager().GetTable_AI_Enum_Define();

                foreach (DataRow dataRow in infoTable.Rows)
                {
                    EnumType enumType = new EnumType();
                    enumType.DBID = dataRow["id"].ToString();
                    enumType.Name = dataRow["name"].ToString();
                    enumType.Ename = dataRow["ename"].ToString();
                    enumType.Info = dataRow["info"].ToString();
                    enumType.m_values = dataRow["values"].ToString().Split(parameterSpliter, StringSplitOptions.RemoveEmptyEntries);

                    enumTypeList.Add(enumType);
                    enumTypeDictionary[enumType.DBID] = enumType;
                }

                inited = true;
            }
        }

        /// <summary>
        /// �������ݿ�ID����ö������
        /// </summary>
        /// <param name="DBID">���ݿ�ID</param>
        /// <returns>ö������</returns>
        public static EnumType GetEnumTypeByID(string databaseID)
        {            
            EnumType enumType = null;

            if (!string.IsNullOrEmpty(databaseID))
            {
                Init();

                if (!enumTypeDictionary.TryGetValue(databaseID, out enumType))
                {
                    enumType = null;
                }                
            }

            return enumType;
        }
    }

    //
    /// <summary>
    /// �¼��б�
    /// </summary>
    public class EventTable
    {
        private static bool inited = false;
        private static List<AI_Event> eventList = new List<AI_Event>();
        private static Dictionary<string, AI_Event> eventDictionary = new Dictionary<string, AI_Event>();

        private static void Init()
        {
            if (!inited)
            {
                DataTable infoTable = DBManager.DataBaseManager.GetDataBaseManager().GetTable_AI_Event_Define();

                foreach (DataRow dataRow in infoTable.Rows)
                {
                    AI_Event aiEvent = new AI_Event();
                    aiEvent.DBID = dataRow["id"].ToString();
                    aiEvent.Name = dataRow["name"].ToString();
                    aiEvent.Ename = dataRow["ename"].ToString();
                    aiEvent.Info = dataRow["info"].ToString();
                    aiEvent.eventid = dataRow["eventid"].ToString();

                    if (string.IsNullOrEmpty(aiEvent.eventid))
                    {
                        aiEvent.eventid = "0";
                    }

                    eventList.Add(aiEvent);
                    eventDictionary[aiEvent.DBID] = aiEvent;
                }

                eventList.Sort(new AIEventComparer());

                inited = true;
            }
        }

        public static List<AI_Event> EventItems
        {
            get
            {
                Init();                    
                return eventList;
            }
        }

        /// <summary>
        /// �����¼�����
        /// </summary>
        /// <param name="databaseID">KeyID</param>
        /// <returns>�¼�����</returns>
        public static AI_Event FindItemByDBID(string databaseID)
        {
            AI_Event aiEvent = null;

            if (!string.IsNullOrEmpty(databaseID))
            {
                Init();

                if (!eventDictionary.TryGetValue(databaseID, out aiEvent))
                {
                    aiEvent = null;
                }                
            }

            return aiEvent;
        }
    }

    /// <summary>
    /// �����б�
    /// </summary>
    class ActionTable
    {        
        private static bool inited = false;
        private static string[] parameterSpliter = new string[] { "\r\n" };
        private static char[] parameterInfoSpliter = new char[] { ',' };
        private static List<AI_Action> actionList = new List<AI_Action>();
        private static Dictionary<string, AI_Action> actionDictionary = new Dictionary<string, AI_Action>();       

        /// <summary>
        /// AI��������
        /// </summary>
        public static List<AI_Action> ActionItems
        {
            get
            {
                Init();
                return actionList;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private static void Init()
        {
            if (!inited)
            {
                DataTable actionInfo = DBManager.DataBaseManager.GetDataBaseManager().GetTable_AI_Action_Define();

                foreach (DataRow dataRow in actionInfo.Rows)
                {
                    AI_Action aiAction = new AI_Action();
                    aiAction.DBID = dataRow["id"].ToString();
                    aiAction.ActionID = dataRow["actionid"].ToString();
                    aiAction.Name = dataRow["name"].ToString();
                    aiAction.Ename = dataRow["ename"].ToString();
                    aiAction.Info = dataRow["info"].ToString();
                    aiAction.ActionType = dataRow["class"].ToString();
                    aiAction.rettype = dataRow["rettype"].ToString();

                    string parms = dataRow["parms"].ToString();

                    foreach (string line in parms.Split(parameterSpliter, StringSplitOptions.RemoveEmptyEntries))
                    {
                        // line: ����,-2,"����"
                        string sline = line + ",";
                        string[] strlist = sline.Split(parameterInfoSpliter);
                        aiAction.Args.Add(new AI_Parm(strlist[0], strlist[2] == "" ? null : strlist[2], int.Parse(strlist[1]), false));
                    }

                    actionList.Add(aiAction);
                    actionDictionary[aiAction.DBID] = aiAction;
                }

                inited = true;
            }
        }

        /// <summary>
        /// ������������
        /// </summary>
        /// <param name="DBID">KeyID</param>
        /// <returns>��������</returns>
        public static AI_Action FindItemByDBID(string databaseID)
        {
            AI_Action aiAction = null;
            
            if (!string.IsNullOrEmpty(databaseID))
            {
                Init();

                if (!actionDictionary.TryGetValue(databaseID, out aiAction))
                {
                    aiAction = null;
                }                
            }

            return aiAction;
        }
    }

}
