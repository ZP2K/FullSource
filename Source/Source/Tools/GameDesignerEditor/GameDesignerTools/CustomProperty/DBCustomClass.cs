using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Collections;
using System.Data.SqlClient;
using LuaInterface;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using DataTableRemotingClient;

namespace GameDesingerTools
{
    public class DBCustomClass : CustomClass, ICustomTypeDescriptor
    {
        System.Data.DataTable m_tbl_MainData, m_tbl_FieldName_CN;
        SqlConnection m_conn;
        public SqlConnection Conn { get { return m_conn; } }
        SqlDataAdapter m_adp;
        //CustomClass m_PropertiesContainer = new CustomClass();
        Hashtable m_colName = new Hashtable(); //��������
        Hashtable m_combox_Keys = new Hashtable(); //���е������б�ļ���,��ʾֵ��Ϊkey
        Hashtable m_combox_Values = new Hashtable();//��ʾʱ����idֵ��ʾ����,idֵ��Ϊkey
        Hashtable m_combox_ExValues = new Hashtable();//���ڻ�ȡ���ӵ��ֶ����ݣ�����ű�����
        Hashtable m_luacodes = new Hashtable();//��¼��ǰ�����еĽű�����
        Hashtable m_scriptExecState = new Hashtable();
        string m_strMainTableName;
        string m_TabMainTableName; // ��ǰTab��ҳ��������
        string[] m_strCats;
        //DataRow m_currRow;
        CustomProperty m_property = null;
        int m_nSortType = 0;

        //
        string m_strSql, m_strMetaSql, m_strScriptSQL;
        public object[] m_findkeys;
        object[] m_findkeyvalues; 

        string[] m_strDBPrimaryKeys;
        string[] m_strPrimaryKeys;
        int m_nModlId, m_nModlTabId, m_nParentFieldId;
        object m_parent;
        bool m_bPrimaryKeysSameAsDB = false;//m_strPrimaryKeys�Ƿ������ݱ��е�����������ͬ
        bool m_bDBVirtualField = false;//�Ƿ�����Ϊ�����ֶ�,ֻ�ڱ�Ԫ������Ϊ�����ֶεĲŻ�Ϊ��
        LuaEx m_lua;
        int m_nParentFieldType = 0;
        Hashtable m_events = null;
        CustomProperty m_ReplaceSubKeyProperty = null;
        int m_nRecordCound = 0;
        int m_nRecordModifyCount = 0; //���л���¼���κ�һ���޸Ļ����һ�����֣�����������ʹ��
        RecordLockEx m_reclock;
        int m_nLastServerVersion = -1;
        bool m_bReloadDataOnSave = false; //��Ҫ��ÿ�α���ʱ�����ݿ����¶�ȡ���ݣ����ⷢ�������ͻ

        public delegate void AsynNotifyHandler(int nNotifyID, object param);
        public event AsynNotifyHandler OnAsynNotify;

        private List<string> m_listSortedCol = null;

        private bool m_valueChanged = false; // ֵ�Ƿ��Ѿ��ı� add by suntao
        private bool m_initFinished = false; // �Ƿ��Ѿ���ɳ�ʼ�� add by suntao
        
        public string StrSql = string.Empty;

        private List<string[]> mRecordList = new List<string[]>();
        private List<DBCustomProperty> mRecordProList = new List<DBCustomProperty>();

        //�����ڷ������ֶ��������ֶ�ʱ���ã��ڱ�����ڲ�ʹ��
        public DBCustomClass(object parent, LuaEx lua, int nTabId, int nParentFieldId, int nParentFieldType)
        {
            m_nParentFieldType = nParentFieldType;
            InitMetaInfo(parent, lua, nTabId, nParentFieldId);
        }
        public DBCustomClass(object parent, LuaEx lua, int nTabId, int nParentFieldId)
        {
            InitMetaInfo(parent, lua, nTabId, nParentFieldId);
        }

        /// <summary>
        /// �Ƿ��Ѿ���ɳ�ʼ��
        /// </summary>
        public bool InitFinished
        {
            set
            {
                m_initFinished = value;
            }

            get
            {
                return m_initFinished;
            }
        }

        /// <summary>
        /// ֵ�Ƿ��Ѿ��ı� add by suntao
        /// </summary>
        public bool ValueChanged
        {
            set
            {
                m_valueChanged = value;
            }

            get
            {
                return m_valueChanged;
            }
        }

        /// <summary>
        /// ���ֵ�ı� add by suntao
        /// </summary>
        /// <param name="prop">CustomProperty</param>
        private void MarkValueChanged(CustomProperty prop)
        {
            DBCustomClass dbCustomClass = prop.Parent as DBCustomClass;

            if (dbCustomClass != null)
            {
                dbCustomClass.MarkValueChanged();
            }
        }

        /// <summary>
        /// ���ֵ�ı� add by suntao
        /// </summary>
        public void MarkValueChanged()
        {
            m_valueChanged = true;
            
            DBCustomClass parentDBCustomClass = m_parent as DBCustomClass;            

            if (parentDBCustomClass != null)
            {
                parentDBCustomClass.MarkValueChanged();                
            }
        }

        public void OnAsynNotifyEvent(int nNotifyID, object param)
        {
            DBCustomClass cls = this;
            while (cls.Parent != null)
            {
                if (cls.Parent is DBCustomClass)
                {
                    cls = (DBCustomClass)(cls.Parent);
                }
            }
            if (cls.OnAsynNotify != null)
            {
                cls.OnAsynNotify(nNotifyID, param);
            }            
        }

        private List<string> GetSplitField(string splitString, bool virtualField)
        {
            List<string> list = new List<string>();

            string[] tempArray = splitString.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
            
            foreach (string s in tempArray)
            {
                int index = s.IndexOf(",");

                if (virtualField) // �����ֶζ��ұߵ�ֵ
                {
                    list.Add(s.Substring(index + 1, s.Length - index - 1));
                }
                else // �������ֶζ���ߵ�ֵ
                {
                    list.Add(s.Substring(0, index).ToLower());
                }
                
            }

            return list;
        }

        private void InitMetaInfo(object parent, LuaEx lua, int nTabId, int nParentFieldId)
        {
            m_findkeyvalues = null;            
            m_lua = lua;

            // ����ParentFieldid��ͬ����ȡ�����ȫ��ͬ = -1��ʾ��һ��
            string strParentField = nParentFieldId >= 0 ? " AND parentfield=" + nParentFieldId : "AND (parentfield IS NULL OR parentfield = '0')";
            
            // ����TabId ��ȡ��ģ��id
            DataTable tbl = CustomClassCache.GetModTabDefProxy("sys_modl_tab_def", string.Format("SELECT modid FROM sys_modl_tab_def WHERE modtabid = {0}", nTabId));
            int nModalId = int.Parse(tbl.Rows[0][0].ToString());
            
            // ��ȡ��Tabҳ������
            tbl = CustomClassCache.GetModTabDefProxy("sys_modl_tab_def", string.Format("SELECT tablename, catfields FROM sys_modl_tab_def WHERE modid = {0} AND modtabid = {1}", nModalId, nTabId));
            m_TabMainTableName = tbl.Rows[0]["tablename"].ToString().Trim(); // no lower here.            
            
            if (nParentFieldId < 0)
            {
                m_strCats = GetFieldStr(tbl.Rows[0], "catfields").Split(new char[] { ',' });
            }
            
            // ��ȡ����������
            string currentTableName = null;
            tbl = CustomClassCache.GetModTabDefProxy("sys_meta_info", string.Format("SELECT tablename, fieldname FROM sys_meta_info WHERE fieldid = {0}", nParentFieldId));                            
            
            if (tbl.Rows.Count > 0)
            {
                currentTableName = tbl.Rows[0]["tablename"].ToString().Trim(); // no lower here.
            }            

            if (nParentFieldId >= 0)
            {
                m_strMainTableName = currentTableName;
            }
            else
            {
                m_strMainTableName = m_TabMainTableName;
            }

            // ���� metainfo SQL���, ֻѡ��ǰ�ɼ����ֶ�
            m_strMetaSql = string.Format("SELECT * FROM sys_meta_info WHERE modtabid = {0} {1} ORDER BY {2}", nTabId, strParentField, "orderno");
            DataTable metatbl = CustomClassCache.GetDbMetaInfoProxy("sys_meta_info", m_strMetaSql);

            // visible���ò����ã�����֮����Ҳ������ֶ�
            StringBuilder visibleFields = new StringBuilder();

            if (m_strMainTableName != m_TabMainTableName) // ��ģ��������ͬ��ֱ�Ӵ�Cache��ȡ��¼����
            {
                List<string> visibleFieldList = new List<string>();
                List<string> primaryKeyList = CustomClassCache.GetPrimaryKeys(m_strMainTableName);

                foreach (DataRow row in metatbl.Rows)
                {
                    string fieldType = row["fieldtype"].ToString();

                    if (fieldType != "1")
                    {
                        visibleFieldList.Add(GetFieldStr(row, "fieldname"));
                    }
                    else
                    {                        
                        // �����ֶ���Ҫ��ȡ������������ֶε���Ϣ
                        List<string> splitFiledList = GetSplitField(row["subtablefilter"].ToString(), false);

                        foreach (string s in splitFiledList)
                        {
                            if (!visibleFieldList.Contains(s))
                            {
                                visibleFieldList.Add(s);
                            }
                        }                        
                    }
                }
               
                // ��Ҫ�������Ĺ�����Ϣ
                if (parent != null && (parent as DBCustomClass).m_nParentFieldType == 1)
                {
                    tbl = CustomClassCache.GetDbMetaInfoProxy("sys_meta_info", string.Format("SELECT subtablefilter FROM sys_meta_info WHERE fieldid = '{0}'", nParentFieldId.ToString()));

                    List<string> splitFiledList = GetSplitField(tbl.Rows[0]["subtablefilter"].ToString(), true);

                    foreach (string s in splitFiledList)
                    {
                        if (!visibleFieldList.Contains(s))
                        {
                            visibleFieldList.Add(s);
                        }
                    }
                }                

                // ��������
                foreach (string s in primaryKeyList)
                {
                    string keyField = s.ToLower();

                    if (!visibleFieldList.Contains(keyField))
                    {
                        visibleFieldList.Add(keyField);
                    }
                }
 
                foreach (string s in visibleFieldList)
                {
                    visibleFields.Append(string.Format("[{0}],", s));
                }

                if (visibleFields.Length > 0)
                {
                    visibleFields.Remove(visibleFields.Length - 1, 1);
                }
            }
                        
            string strVisibleFeilds = visibleFields.ToString();          
            //m_strSql = string.Format("SELECT {1} FROM {0}", m_strMainTableName, strVisibleFeilds.Length > 0 ? strVisibleFeilds : "*");
            m_strSql = string.Format("SELECT {1} FROM {0}", m_strMainTableName, "*");
            m_nModlId = nModalId;
            m_nModlTabId = nTabId;
            m_nParentFieldId = nParentFieldId;
            m_parent = parent;
            
            // ��������
            m_strPrimaryKeys = GetPrimaryKeys();
            InitMetaInfo(m_strSql);

            // ��ʼ������
            m_reclock = new RecordLockEx(Conn, m_strMainTableName, m_strDBPrimaryKeys, m_nModlId);            
        }
     
        public bool ReloadDataOnSave
        {
            get { return m_bReloadDataOnSave; }
            set { m_bReloadDataOnSave = value; }
        }
        public object[] FindKeys
        {
            get { return m_findkeyvalues; }
        }
        public int SortType
        {
            get { return m_nSortType; }
            set { m_nSortType = value; }
        }

        public SqlDataAdapter DataAdapter
        {
            get { return m_adp; }
        }
        
        public CustomProperty Property
        {
            get { return m_property; }
            set { m_property = value; }
        }
        public int RecordCount
        {
            get { return m_nRecordCound; }
        }
        public int ParentFieldType
        {
            get { return m_nParentFieldType; }
        }
        public object Parent
        {
            get {return m_parent;}
        }
        public string MainTable
        {
            get {return m_strMainTableName;}    
        }
        public int ModalTabId
        {
            get { return m_nModlTabId; }
        }
        //add by cuiwei 07.10.16
        public int TabID { get { return this.ModalTabId; } }
        //add end

        //public int ModId
        //{
        //   get { return m_nModID; }
        //}
        /*
        public int ModalId
        {
            get {return m_nModalId;}
        }
         */
        public int ParentFieldID
        {
            get { return m_nParentFieldId; }
        }
        public bool IsVirtualField
        {
            get { return m_bDBVirtualField; }
            set { m_bDBVirtualField = value; }
        }

        public object[] DBPrimaryKey
        {
            get { return m_strDBPrimaryKeys; }
        }

        public object[] PrimaryKey
        {
            get { return m_strPrimaryKeys; }
        }        

        public bool IsPrimaryKeysSameAsDB
        {
            get { return m_bPrimaryKeysSameAsDB; }
        }
        public void ReInitMetaInfo()
        {
            InitMetaInfo(m_strSql);//, m_strMainTableName);//, m_strPrimaryKeys);
        }

        //��ȡ��ǰʹ�õ����ݱ�
        public System.Data.DataTable DataTable
        {
            get { return m_tbl_MainData; }
        }

        //ע�⣬��ΪOBJPOOLʹ��
        public void ReInit(object Parent, int nParentFieldType)
        {
            m_parent = Parent;
            m_nParentFieldType = nParentFieldType;
            ClearDBDataCache();
        }

        public void ClearDBDataCache()
        {
            CustomClassCache.DelDataTableCache(m_strSql);

            foreach (DBCustomProperty subprop in this)
            {
                DBCustomClass dbCustomClass = subprop.Value as DBCustomClass;

                if (dbCustomClass != null)
                {                    
                    dbCustomClass.ClearDBDataCache();   
                }                
            }
        }        

        public CustomClass ReloadDBData(object[] keyvalues) // ���������Ԫ����֮�������
        {
            CustomClass customClass = null;                                
            
            if (keyvalues != null) 
            {
                customClass = GetDynamicClass(keyvalues); // GetDynamicClass()�Ѿ����ù�InitDBInfo()��ClearProperties()��
            }

            return customClass;
        }

        public void ReloadCurrentInfo()
        {
            CustomClassCache.DelDataTableCache(m_strSql);
            ReInitMetaInfo();
            if (m_findkeys != null)
            {
                GetDynamicClass(m_findkeys);
            }
        }
        
        //��������,������и��ֶε����,��Ҫ�Ӹ��ֶε�SubTableFilter(������ϵ)�ֶ��ж�ȡ��������
        public string[] GetKeyFromParentField(int nFieldId, bool bReadMainField)
        {
            string strMetaSql = string.Format("SELECT subtablefilter FROM sys_meta_info WHERE fieldid = {0}", nFieldId);//todo:���Ӧ����initmetadata�г�ʼ�����
            DataRow row = CustomClassCache.GetDbMetaInfoProxy("sys_meta_info", strMetaSql).Rows[0];
            string strFilter = GetFieldStr(row, "subtablefilter");
            string[] strkeys = GetSplitField(strFilter, !bReadMainField).ToArray();
           
            return strkeys;
        }

        private bool CompareKeys(string[] keys1, string[] keys2)
        {
            if (keys1.Length != keys2.Length)
            {
                return false;
            }

            for (int i = 0; i < keys1.Length; i++)
            {
                if (keys1[i].ToLower() != keys2[i].ToLower())
                {
                    return false;
                }
            }

            return true;
        }

        private string[] GetPrimaryKeys()
        {
            string[] strKeys;
            m_strDBPrimaryKeys = CustomClassCache.GetPrimaryKeys(m_strMainTableName).ToArray();
            // m_strDBPrimaryKeys = CustomClassCache.GetKeyFromDB(m_strSql);

            if (m_nParentFieldId < 0 || m_nParentFieldType == 2) // �������ֶλ����Ǵ����ֶεķ������ֶ�
            {
                strKeys = m_strDBPrimaryKeys;
                m_bPrimaryKeysSameAsDB = true;
            }
            else
            {                
                strKeys = GetKeyFromParentField(m_nParentFieldId, false);
                // m_bPrimaryKeysSameAsDB = Helper.CompareStringArray(m_strDBPrimaryKeys, strKeys, true);  
                m_bPrimaryKeysSameAsDB = CompareKeys(m_strDBPrimaryKeys, strKeys);               
            }

            return strKeys;
        }

        private void SetDBPrimaryKeys(string[] strkeys)
        {
            DataColumn[] keys = new DataColumn[strkeys.Length];
            int nKeyCount = 0;
            for (int i = 0; i < strkeys.Length; i++)
            {
                keys[i] = m_tbl_MainData.Columns[strkeys[i]];
                nKeyCount++;
            }
            m_tbl_MainData.PrimaryKey = keys;
        }
        private void ResetPrimaryKey()
        {
            //������������,���ͬʱƥ�䵽������¼����Ҫ��ֳɲ�ͬ��CustomClass��ֱ��ִ��PrimaryKey=xxx���쳣
            if (m_bPrimaryKeysSameAsDB && m_strPrimaryKeys != null && m_tbl_MainData.PrimaryKey.Length == 0)
            {
                SetDBPrimaryKeys(m_strPrimaryKeys);
            }
        }

        // ���¶����ݿ�����
        private void InitDBInfo(string strmaindatasql, string[] keys, object[] keyvalues)
        {
            string strsql = string.Empty;
            string sql;
            if (keyvalues.Length > 0)
            {
                sql = GetFilter(keys, keyvalues);
                strsql = string.Format("{0} where {1}", strmaindatasql, sql);
                StrSql = strsql;

                m_tbl_MainData = CustomClassCache.GetDataTableWithoutKeyProxy(m_strMainTableName, sql, null, ref m_adp);
            }
            else
            {
                Debug.Assert(true);
                strsql = strmaindatasql;
                StrSql = strsql;
                m_tbl_MainData = CustomClassCache.GetDataTableWithoutKeyProxy(m_strMainTableName, ref m_adp);
            }

            /*m_tbl_MainData = CustomClassCache.GetDataTableWithoutKey(strsql, ref m_adp);*/

            ResetPrimaryKey();
        }

        //��ʼ�����ݿ����ӣ�����ص�Ԫ���ݶ�ȡ
        private void InitMetaInfo(string strsql)
        {
            m_conn = MainForm.conn;
            m_listSortedCol = new List<string>();
            m_tbl_FieldName_CN = CustomClassCache.GetDbMetaInfoProxy("", m_strMetaSql);
            m_colName.Clear();

            // ��¼������ر�Ԫ��¼������
            foreach (DataRow row in m_tbl_FieldName_CN.Rows)
            {                
                string fieldname = GetFieldStr(row, "fieldname");
                string strEditorType = GetFieldStr(row, "editortype");

                object[] ao = new object[] { 
                    GetFieldStr(row, "fieldcnname"), 
                    GetFieldStr(row, "description"), 
                    GetFieldStr(row, "catname"), 
                    strEditorType,
                    GetFieldStr(row, "readonly") ,
                    GetFieldStr(row, "relativepath") ,
                    GetFieldStr(row, "modtabid") ,
                    GetFieldStr(row, "tablename") ,
                    fieldname,
                    GetFieldStr(row, "fieldid") ,
                    GetFieldStr(row, "fieldtype") ,
                    GetFieldStr(row, "sorttype") ,
                    GetFieldStr(row, "cutrelativepath") ,
                    GetFieldStr(row, "cutpreslash") ,
                    GetFieldStr(row, "visible") ,
                    };
               
                m_colName[fieldname] = ao; // �涨ͬһ��ȫ������ͬһ�ű�����ֶ���������ͬ
                m_listSortedCol.Add(fieldname);

                if (strEditorType == "lookupcombo") // �������Ӳ�ѯ�ֶΣ���Ҫ�����ݿ��ȡ���������ʾ����
                {
                    FillComboList(fieldname, row, m_conn);
                }
                else if (strEditorType == "textcombo")
                {
                    FillTextComboList(fieldname, row);
                }
            }

            // ��ʼ��������صĽű� #�������ĳ���ֶεĽű�            
            // luacodeֻ�浱ǰ��Ľű�            
            if (m_nParentFieldId >= 0)
            {
                m_strScriptSQL = string.Format("SELECT s.fieldname, s.scriptcode FROM sys_scriptstore s LEFT OUTER JOIN sys_meta_info m ON s.fieldid = m.fieldid WHERE (s.modtabid = '{0}') AND (s.tablename = '{1}') AND (m.parentfield={2})"
                                               , m_nModlTabId, m_strMainTableName, m_nParentFieldId);
            }
            else
            {
                m_strScriptSQL = string.Format("SELECT s.fieldname, s.scriptcode FROM sys_scriptstore s LEFT OUTER JOIN sys_meta_info m ON s.fieldid = m.fieldid WHERE (s.fieldid != -1) AND (s.modtabid = '{0}') AND (m.parentfield is null OR m.parentfield = 0)"
                                               , m_nModlTabId);
            }

            DataTable tblMainCode = CustomClassCache.GetScriptStoreProxy(new string[]{"sys_scriptstore","sys_meta_info"}, m_strScriptSQL);

            foreach (DataRow row in tblMainCode.Rows)
            {
                string strfname = GetFieldStr(row, "fieldname"); 
                string scriptText = Helper.GetStringFromByteArray(row["scriptcode"]).Trim();
                m_luacodes[strfname] = scriptText;
                
                if (strfname == "" || strfname == "#")
                {
                    m_luacodes["#"] = scriptText; // ����Ϊtabҳ����Ĵ����
                }
            }

            string[] names = new string[] { "sys_modl_tab_def", "sys_modl_tab_def" };

            // ## �Ĵ���
            string strsql_ = string.Format("SELECT TOP 1 modtabid FROM sys_modl_tab_def WHERE (modid = (SELECT modid FROM sys_modl_tab_def WHERE (modtabid = {0})))", m_nModlTabId);
            DataTable tbl_ = CustomClassCache.GetModTabDefProxy("sys_modl_tab_def", strsql_);
            string first_modtabid = tbl_.Rows[0][0].ToString();
            string strsql__ = string.Format("SELECT scriptcode FROM sys_scriptstore WHERE tablename='{0}' AND fieldid=-1 AND modtabid={1}", m_strMainTableName, first_modtabid);
            DataTable tbl__ = CustomClassCache.GetScriptStoreProxy(names, strsql__);
            
            /* ����Ĵ���ù��죬ֻȡ��һ�����ݵĻ���ȫû��Ҫ����д suntao
            foreach (System.Data.DataRow row in tbl__.Rows)
            {
                m_luacodes["##"] = Helper.GetStringFromByteArray(row["scriptcode"]).Trim();
                break;
            }
            */

            if (tbl__.Rows.Count > 0)
            {
                m_luacodes["##"] = Helper.GetStringFromByteArray(tbl__.Rows[0][0]).Trim();
            }            
        }

        private DBCustomProperty GetProperty(DataRow CurrRow, string strColId)
        {
            long beginTick = DateTime.Now.Ticks;

            object[] args = (object[])(m_colName[strColId]);
            bool bVirtualFeild = m_tbl_MainData.Columns.IndexOf(strColId) < 0;            
            string strcoldbid = strColId;                        
            string strcolname = (string)args[0]; // ��������
            string strdes = (string)args[1]; // ����������            
            string strtype = (string)args[3]; // �����������ƣ���Ӱ��ֵ�ı༭��ʽ
            int nSortType = (string)args[11] == "" ? 0 : Convert.ToInt32(args[11]);//��Ϊ�����ֶ�ʱ���Դӱ��¼������ʽ��Ӱ��
            string strrelativepath = (string)args[5]; // ���·����
            string strcutrelativepath = (string)args[12];
            string strmodtabid = (string)args[6]; // ģ��tabid
            bool bCutPreSlash = ((string)args[13] == "") ? true : Convert.ToBoolean(args[13]);
            bool bVisible = (string)args[14] == "false" ? false : true;

            bool breadonly = false;

            if (args[4].ToString() != "")
            {
                breadonly = Convert.ToBoolean(args[4]);    //�Ƿ�ֻ��
            }
            else
            {
                breadonly = true;
            }

            string strcat = (string)args[2]; // �����

            if (strcat == "")
            {
                strcat = "δ����";
            }

            string strFieldType = args[10] == DBNull.Value ? "0" : (string)args[10]; // == "1" Ϊ�����ֶε������ֶ�

            if (strFieldType == "1" || strFieldType == "2") // ==1Ϊ�����ֶΣ�==2Ϊ��ͨ�ֶ����������ֶ�
            {
                strtype = "tree"; // ǿ����Ϊ�����
            }

            object value = null;
            int nFieldID = args[9] == DBNull.Value ? 0 : Convert.ToInt32(args[9]);

            if (bVirtualFeild) // m_colName.ContainsKey(strcolname))                
            {                
                strtype = "tree"; // ǿ����Ϊ�����
                strcoldbid = strColId.ToLower();
            }
            else
            {
                value = CurrRow[strcoldbid];
                
                if (value == null || value == DBNull.Value)
                {
                    DataRow def_row = CustomClassCache.GetPropDefaultValue(m_strMainTableName);

                    if (def_row != null)
                    {
                        value = def_row[strcoldbid];
                    }
                }

                DataColumn col = m_tbl_MainData.Columns[strColId];
                strcoldbid = col.ColumnName.ToLower();
            }            

            enumValueType vtType = enumValueType.vtUnknown;
            DBCustomClass subNode = null;
            
            switch (strtype)
            {
                case "text":
                    vtType = enumValueType.vtText;
                    break;
                case "bool":
                    vtType = enumValueType.vtBool;
                    break;
                case "filename":
                    vtType = enumValueType.vtFileName;
                    break;
                case "filenamescript":
                    vtType = enumValueType.vtFileNameScript;
                    break;
                case "filenamesfx":
                    vtType = enumValueType.vtFileNameSfx;
                    break;
                case "pathname":
                    vtType = enumValueType.vtPath;
                    break;
                case "tree":
                    // ���m_strPrimaryKeys������ǰ����ʵ�ʵ�����ֵ�Ļ�����ȷ��Ϊһ�Զ�Ĺ�ϵ, //��Ҫ��ʼ�ֲ�ͬ��CustomClass
                    vtType = enumValueType.vtExpandNode;
                    subNode = ObjPool.GetDBClass(this, m_lua, m_nModlTabId, nFieldID, int.Parse(strFieldType)); // ע�⣺ֻ�������ֶβŻ��л����µ�������
                    subNode.ID = strcoldbid; // ֻ�ܼٶ�����ʵ�ֶ�������ͬ,��Ȼ�����
                    subNode.Description = strdes + "...";
                    subNode.Value = "...";
                    subNode.SortType = nSortType;
                    object[] findkeyvalues;

                    if (strFieldType == "2") // �������ͨ�ֶ��µ������ֶΣ���Ҫ���Լ���ֵ��Ϊ�����ֶε�keyֵ���ݽ�ȥ
                    {
                        findkeyvalues = GetChildKeyValues(CurrRow, m_strDBPrimaryKeys); // �������ӱ�ʱ,�Ҵ��ӱ��¼������
                        
                        if (m_ReplaceSubKeyProperty == null)
                        {
                            subNode.Value = CurrRow[strcoldbid];

                            if (subNode.Value == null || subNode.Value == DBNull.Value)
                            {
                                subNode.Value = value;                                
                            }
                        }
                        else
                        {
                            subNode.Value = Value;
                        }

                        subNode.DefaultValue = subNode.Value;
                    }
                    else
                    {
                        subNode.Value = strdes + "...";
                        
                        // bubfix:ֻҪ�й�����ϵ�ģ�һ������������ֶ�ֵ���ݽ�ȥ���Ҵӱ��¼
                        findkeyvalues = GetChildKeyValues(CurrRow, GetKeyFromParentField(nFieldID, true));                        
                    }

                    // ȡ��CLASS��KEYֵ�ֶ�,���Ҷ�ȡ��ǰ��¼��������Ϊfindkeyvalues������
                    value = subNode.GetDynamicClass(findkeyvalues);
                    subNode.SetEvents(subNode.AllEvents);
                    
                    if (strFieldType == "1")
                    {
                        subNode.IsVirtualField = true;
                    }

                    break;
                case "customeditor":
                    vtType = enumValueType.vtCustomEditor;
                    break;
                default:
                    break;
            }

            if (m_luacodes[strcoldbid] != null && (m_luacodes[strcoldbid] as string).Trim().Length > 0)
            {
                strcolname += " [��]";
            }

            DBCustomProperty prop = ObjPool.GetProperty(nFieldID, this, strcoldbid, strcolname, value, breadonly, bVisible, vtType);            
            prop.Category = strcat;
            prop.Description = strdes;
            prop.Key = nFieldID.ToString();

            // ����������������ص�hashtable����������
            if (m_combox_Keys.ContainsKey(strcoldbid))
            {
                prop.ValueType = enumValueType.vtCombo;                
                prop.ComboValues = m_combox_Keys[strcoldbid] as Hashtable;
                Hashtable htValues = (Hashtable)m_combox_Values[strcoldbid];
                prop.ComboKeys = htValues;
                prop.ComboValuesEx = (Hashtable)m_combox_ExValues[strcoldbid];
                
                if (value != null)
                {
                    prop.Value = htValues[value.ToString()];

                    if (prop.Value == null)
                    {
                        prop.Value = value;
                    }
                }
                else
                {
                    prop.Value = "";
                }
            }

            switch (vtType)
            {
                case enumValueType.vtBool:
                    if (prop.Value != DBNull.Value)
                    {
                        if (m_tbl_MainData.Columns[prop.ID].DataType == typeof(System.Boolean))
                            prop.Value = Convert.ToBoolean(prop.Value) == true ? "��" : "��";
                        else
                            prop.Value = Convert.ToString(prop.Value) == "1" ? "��" : "��";
                    }
                    else
                        prop.Value = "��";
                    break;
                case enumValueType.vtText:
                    if (prop.Value == DBNull.Value)
                        prop.Value = "";
                    break;
                case enumValueType.vtCustomEditor:
                    if (prop.Value == DBNull.Value)
                        prop.Value = "";
                    break;
                case enumValueType.vtFileName:
                    strrelativepath = strrelativepath.TrimStart(new char[] { '\\' });
                    strcutrelativepath = strcutrelativepath.TrimStart(new char[] { '\\' });
                    prop.Prefix = Helper.CombinePaths(Program.RootDir, strrelativepath); 
                    prop.CutPrefix = Helper.CombinePaths(Program.RootDir, strcutrelativepath); 
                    prop.CutPreSlash = bCutPreSlash;

                    if (prop.Value == DBNull.Value)
                        prop.Value = "";
                    break;
                case enumValueType.vtExpandNode:
                    subNode.Property = prop;
                    break;
                default:
                    break;
            }

            long endTick = DateTime.Now.Ticks;
            string timeString = string.Format("GetProperty\t{0}\t{1}", strColId, endTick - beginTick);
            Debug.WriteLine(timeString);

            return prop;
        }

        //�޸Ĵӱ��е�findkeys
        public void ReplaceSubFindKey(CustomProperty prop, object value)
        {
            //����ֱ��ʹ��value,��������޸ĵķ������ֶ�,Ҫ��ȡ�������������
            //Value = value;
            //GetDynamicClass(value);
            //��Ҫ����valueֵ���������keyֵ
            //��ʵKEYû�б�,ֻ����������������ֶ��������ڴ��иı���(��represent1)
            object oldvalue = Value;
            prop.Tag = m_findkeyvalues;
            try
            {
                //�Ƚ���֮ǰ�����Ŀ����޸Ĺ��Ĵӱ��¼
                OnLockRecord(false, null, null);

                Value = value;
                m_ReplaceSubKeyProperty = prop; //��ס��ǰ�޸ĵ��ֶ�ID���Ա���ʱ�Ķ��ӽ���е�����
                GetDynamicClass(m_findkeyvalues);
            }
            finally
            {
                m_ReplaceSubKeyProperty = null;
            }
        }
        //�޸ı��������findkeys
        private bool ModifyFindKeys(string strkeyfield, object value)
        {
            //����Ƕ����DBCLASS������ؼ�ֵ���޸ģ�����Ҫ�޸�findkeys
            //���ͬʱ���Ƕ����DBCLASS(parentfield=-1),����Ҫ�ڱ����ͬʱ֪ͨģ������FINDKEYS�����˱仯
            int nFindKey = -1;
            for(int i = 0; i < m_strDBPrimaryKeys.Length; i ++)
            {
                string strkey = m_strDBPrimaryKeys[i].ToLower();
                if (strkey == strkeyfield.ToLower())
                {
                    nFindKey = i;
                    break;
                }
            }
            if (nFindKey >= 0)
            {
                m_findkeyvalues[nFindKey] = value.ToString();
                m_findkeys[nFindKey] = value.ToString();
                return true;
            }
            else
                return false;
        }
        public void ModifyFindKeys(CustomProperty prop, object value)
        {
            //����Ƕ����DBCLASS������ؼ�ֵ���޸ģ�����Ҫ�޸�findkeys
            //���ͬʱ���Ƕ����DBCLASS(parentfield=-1),����Ҫ�ڱ����ͬʱ֪ͨģ������FINDKEYS�����˱仯            
            if (ModifyFindKeys(prop.ID, value))
            {
                DBCustomClass cls = this;
                while (cls.ParentFieldType != 2 && cls.Parent != null)
                {
                    if (cls.Parent is DBCustomClass)
                    {
                        cls = (DBCustomClass)(cls.Parent);
                    }
                }
                if (Parent == null || this.MainTable == cls.MainTable) //����������޸���ģ�������е�����ֵ
                {
                    OnAsynNotifyEvent(3, prop);//֪ͨFORM������ʱҪˢ��ģ����
                }
            }
        }

        public void ClearProperties()
        {
            foreach (DBCustomProperty subprop in this)
            {
                DBCustomClass dbCustomClass = subprop.Value as DBCustomClass;

                if (dbCustomClass is DBCustomClass)
                {
                    dbCustomClass.ClearProperties();
                    ObjPool.DelDBClass(dbCustomClass);
                }

                ObjPool.DelProperty(subprop);
            }

            this.Clear();
        }

        private void GetClassContent(CustomClass clsParent, DataRow CurrRow, object[] keyvalues)
        {
            if (CurrRow == null) // ���û���ҵ���ؼ�¼��������������е����ԡ�
            {
                ClearProperties();
                return;
            }           

            m_findkeys = keyvalues;
            ClearProperties();            

            for (int i = 0; i < m_listSortedCol.Count; i ++ )
            {
                this.Add(GetProperty(CurrRow, m_listSortedCol[i]));                
            }
        }

        private string GetFilter(string[] strkeys, object[] keyvalues)
        {
            string strfilter = "1 = 1"; //����������һ������
            if (strkeys.Length != keyvalues.Length)
            {
                System.Windows.Forms.MessageBox.Show("����Ĳ�ѯ��ֵ���ʼ��ʱʹ�õļ�ֵ�����С���£���ȷ�� GetDynmamicClass(keys)�е�keys����ȷ�ġ�");
                return strfilter;
            }
            for(int i = 0; i < strkeys.Length; i ++)
            {
                strfilter += string.Format(" AND {0} = '{1}'", strkeys[i], keyvalues[i]);
            }
            return strfilter;
        }

        //���ݼ�ֵ��������ȫ�Ŀ�Ψһ��λ�ӱ�һ����¼�� findkeyvalues
        private object[] GetChildKeyValues(DataRow row, string[] primaryKeys)
        {
            object[] KeyValues = new string[primaryKeys.Length];
            DataRow def_row = null;
            bool readDefaultRow = false;

            for (int i =  0;i < primaryKeys.Length; i++)
            {
                string primaryKeyString = primaryKeys[i].ToLower();

                // add by cuiwei 07.11.15 ���Զ�ȡ�޸��˵���û�����ֵ����Ϊdatarow������̫���ˣ��޷�Ӧ�Զ����������
                DBCustomClass dbc = this.Parent as DBCustomClass;
                if (dbc != null)
                {
                    CustomProperty prop = dbc[primaryKeyString];                    

                    if (prop != null)
                    {
                        string keyValue = prop.Value as string;

                        if (keyValue != null)
                        {
                            KeyValues[i] = keyValue;
                            continue;
                        }                        
                    }
                }
                // add end

                if (m_ReplaceSubKeyProperty != null && m_ReplaceSubKeyProperty.ID.ToLower() == primaryKeyString)
                {
                    KeyValues[i] = Value.ToString();
                }
                else
                {
                    object ovalue = row[primaryKeys[i]];

                    if ((ovalue == null || ovalue == DBNull.Value)) // Ϊ��Ҫ��ȡĬ���е�ֵ
                    {
                        if (def_row == null && !readDefaultRow) // �Ѿ�����һ��Ĭ���о�û��Ҫ��������
                        {
                            def_row = CustomClassCache.GetPropDefaultValue(m_strMainTableName);
                            readDefaultRow = true;
                        }
                        
                        if (def_row != null)
                        {
                            ovalue = def_row[primaryKeys[i]]; 
                        }                                                                       
                    }

                    KeyValues[i] = ovalue.ToString();
                }
            }

            return KeyValues;
        }

        // ���ֶ����������ֶ���Ϊ����㣬��¼��ΪҶ�����֯��ʾ
        // һ�Զ�ʱ����Ҫ���⴦��ͨ�����⸸���(1,2,3Ϊ�����������ԣ�һ�����Դ���һ��Ψһ�ļ�¼)
        // �������ӱ�����Ͳ��Ҷ�ӽ��
        private void Load1MultipleRecord(object[] keyvalues)
        {
            bool lockRecord = false; // Ĭ���е��ӱ��¼�ǲ��ܱ��޸ĵģ����Լ���û���壬�Ż�����

            foreach (object key in keyvalues)
            {
                if (key.ToString() != "0")
                {
                    lockRecord = true;
                    break;
                }
            }

            foreach (string strcolname in m_listSortedCol)
            {
                // �����ֶΣ����ӱ�һ���ֶ���Ϊ����
                DBCustomClass ChildNodes = new DBCustomClass(this, m_lua, m_nModlTabId, m_nParentFieldId); // �����ֶ��µڶ�����չ���ֶ�                
                ChildNodes.Load1MultipleRecordLine(strcolname, m_strPrimaryKeys, keyvalues, m_strDBPrimaryKeys, lockRecord); // ����һ�Զ���������

                object[] args = (object[])(m_colName[strcolname]);
                int nFieldID = args[9] == DBNull.Value ? 0 : Convert.ToInt32(args[9]);                
                DBCustomProperty p = ObjPool.GetProperty(nFieldID, this, strcolname, (string)args[0], ChildNodes, false, true, enumValueType.vtExpandNode);
                p.Name = (string)args[0]; // ������
                p.Description = (string)args[1];
                p.Category = (string)args[2]; // �����
                p.Value = ChildNodes;
                p.Key = (string)args[9];
                p.IsCanHideNode = (Parent != null); // ������Ƕ����㣬���ǿ��Ա����صĽ��
                p.ReadOnly = true;

                this.Add(p);

                //�����Ӽ�¼����ϸkeyֵ                
                ChildNodes.Value = "...";
                ChildNodes.SetEvents(ChildNodes.AllEvents);                
            }
        }

        //���ֶ����������ֶ���Ϊ����㣬��¼��ΪҶ�����֯��ʾ
        public void Load1MultipleRecordLine(string colname, string[] parentKeys, object[] parentKeyValues, string[] primaryKeys, bool lockRecord)
        {
            m_strPrimaryKeys = primaryKeys;
            m_bPrimaryKeysSameAsDB = true;

            // ��Ҫ��ʼ������ı����ݣ����ʱ��û�е��ù� getdynamicclass,��û�г�ʼ�����ݱ�
            InitDBInfo(m_strSql, parentKeys, parentKeyValues);
            SetDBPrimaryKeys(m_strPrimaryKeys);

            object[] childKeys = null;
            DataRow[] rows = m_tbl_MainData.Select(GetFilter(parentKeys, parentKeyValues)); // "skillid = '28' and skilllevel = '1'");

            // ��ʾ�������ʾ�ֶ�����
            string strPrompField = null;                    

            for (int i = 0; i < rows.Length; i++) // N����¼,��Ҫ�����з��ϵļ�¼��������
            {
                DataRow row = rows[i];
                childKeys = GetChildKeyValues(row, m_strPrimaryKeys);
                DBCustomProperty prop = GetProperty(row, colname);

                if (strPrompField != null)
                {
                    prop.Name = rows.Length == 1 ? prop.Name : string.Format("{0}# : {1}", i + 1, row[strPrompField].ToString());
                }
                else
                {
                    prop.Name = rows.Length == 1 ? prop.Name : string.Format("{0}#", i + 1);
                }

                prop.Tag = childKeys; // �����ڱ���ʱʹ�ã�Ҳ��������ɾ��
                prop.Lock = new RecordLockEx(Conn, MainTable, DBPrimaryKey, m_nModlId);

                if (lockRecord) // ֻ�����޸ķ�Ĭ���е��ӱ��¼ʱ�ż�����
                {
                    prop.UpdateLastVersion();
                }
                
                this.Add(prop); // ֻ����ָ����������(��¼��)��������, colcnname, colid
            }

            m_nRecordCound = rows.Length;
        }

        //����¼�������Լ�¼��Ϊ����㣬������ΪҶ�����֯��ʾ
        //1�Զ���һ���������ɷ�ʽ���Լ�¼Ϊ�����ֶΣ�������Ϊ�˼�¼��ص�����
        private void Load1MultipleRecordRecordMode(string[] parentKeys, object[] parentKeyValues, string[] primaryKeys)
        {
            object[] childKeys = null;

            // ����ÿ����¼����ÿ����¼����һ��CustomClass,�Լ�¼��Ϊ��,����������ݰ�װ����
            DataRow[] rows = m_tbl_MainData.Select(GetFilter(parentKeys, parentKeyValues)); // "skillid = '28' and skilllevel = '1'");
            m_nRecordCound = rows.Length;

            for(int i = 0; i < rows.Length; i ++)
            {
                DataRow row = rows[i];
                DBCustomClass ChildNodes = new DBCustomClass(this, m_lua, m_nModlTabId, m_nParentFieldId); // �����ֶ��µڶ�����չ���ֶ�
                childKeys = GetChildKeyValues(row, m_strDBPrimaryKeys);
                ChildNodes.Load1MultipleRecordRecordModeLine(m_strDBPrimaryKeys, childKeys); // ����һ�Զ������������Լ�¼��Ϊ�����

                string strName = string.Format("{0}#", i + 1);
                DBCustomProperty p = new DBCustomProperty(this, strName, strName, ChildNodes, false, true, enumValueType.vtExpandNode); // �������ݲ�����                

                bool lockRecord = false; // Ĭ���е��ӱ��¼�ǲ��ܱ��޸ĵģ����Լ���û���壬�Ż�����

                foreach (object key in childKeys)
                {
                    if (key.ToString() != "0")
                    {
                        lockRecord = true;
                        break;
                    }
                }

                p.Lock = new RecordLockEx(Conn, MainTable, DBPrimaryKey, m_nModlId);

                if (lockRecord) // ֻ�����޸ķ�Ĭ���е��ӱ��¼ʱ�ż�����
                {
                    p.UpdateLastVersion();
                }                

                p.Name = strName; // ������
                p.Description = "�ӱ��еļ�¼";
                p.Value = ChildNodes;
                p.Key = "-1";
                p.Tag = childKeys;
                p.IsCanHideNode = Parent != null; // ������Ƕ����㣬���ǿ��Ա����صĽ��
                p.ReadOnly = true;
                this.Add(p);

                ChildNodes.Value = "...";
                ChildNodes.SetEvents(ChildNodes.AllEvents);                
            }
        }

        //����¼������
        public void Load1MultipleRecordRecordModeLine(string[] primaryKeys, object[] childKeyValues)
        {
            m_strPrimaryKeys = primaryKeys;
            m_bPrimaryKeysSameAsDB = true;

            // ��Ҫ��ʼ������ı����ݣ����ʱ��û�е��ù� getdynamicclass,��û�г�ʼ�����ݱ�
            InitDBInfo(m_strSql, m_strPrimaryKeys, childKeyValues);
            SetDBPrimaryKeys(m_strPrimaryKeys);
            GetDynamicClass(childKeyValues);

            foreach (CustomProperty prop in this)
            {
                if ((m_parent != null) && (m_parent as DBCustomClass).SortType == 1) // ����ǰ���¼��������Ҫ�� # ��Ų��������Ȼÿ����¼����һ��
                {
                    StringBuilder strkeys = new StringBuilder();

                    for (int i = 0; i < childKeyValues.Length; i ++)
                    {
                        if (i > 0)
                        {
                            strkeys.Append(", ");
                        }

                        strkeys.Append(childKeyValues[i]);
                    }

                    prop.Name = string.Format("{0} [{1}]", prop.Name, strkeys.ToString());
                }                
            }
        }

        public Hashtable AllEvents
        {
            get
            {
                if (m_events == null)
                {
                    m_events = new Hashtable();
                    m_events["PropertyChangeEventHandler"] = new CustomClass.PropertyChangeEventHandler(BeforePropertyChangedEvent);
                    m_events["PropertyAfterChangeEventHandler"] = new CustomClass.PropertyAfterChangeEventHandler(AfterPropertyChangedEvent);
                    m_events["CustomEditValueEventHandler"] = new CustomClass.CustomEditValueEventHandler(OnCustomEditorEditValueEvent);
                    m_events["TreeValueEventHandler"] = new CustomClass.TreeValueEventHandler(OnGetTreeValueEvent);
                }

                return m_events;
            }
        }

        //����в���ֵ�п��ַ���������תΪNULL�������FIND����
        object[] PreProcessKeys(object[] keys)
        {
            for(int i = 0; i < keys.Length; i ++)
            {
                object obj = keys[i];
                if (obj.ToString().Length <= 0)
                {
                    keys[i] = null;
                }
            }
            return keys;
        }
        //��þ��ж�̬Ԫ��Ϣ����ʵ��
        //ע�⣬������ӱ�Ļ������ܲ���Ψһ���ؼ�¼����Ҫ�Զ�Ϊÿ����¼����һ��CustomClass
        public CustomClass GetDynamicClass(object[] keyvalues)
        {
            m_initFinished = false;

            if (keyvalues == null)
            {
                return null;
            }

            // �ڿ�ʼ��ʾ����ʱ�������ݿ��ʼ����ֻ��ȡ��Ҫ����������
            InitDBInfo(m_strSql, m_strPrimaryKeys, keyvalues);
            m_findkeyvalues = keyvalues;   
            
            bool lockRecord = false;

            if (m_strMainTableName != m_TabMainTableName) // �����ֶ������Ĭ�����ӱ�Ļ��ǲ��ܱ��޸ĵģ����Բ��ü���
            {                
                foreach (object o in keyvalues)
                {
                    if (o.ToString() != "0")
                    {
                        lockRecord = true;
                        break;
                    }
                }         
            }
            else
            {
                lockRecord = true;
            }

            if (lockRecord)
            {
                m_reclock.UpdateLocalVersion(m_findkeyvalues);
            }

            if (m_bPrimaryKeysSameAsDB)
            {
                DataRow row = m_tbl_MainData.Rows.Find(PreProcessKeys(keyvalues));
                m_nRecordCound = (row == null ? 0 : 1);
                GetClassContent(this, row, keyvalues);
            }
            else
            {
                this.IsVirtualField = true;
                ClearProperties();

                if (SortType == 0) // ���ֶ���Ϊ�������֯1�Զ��в�ͬ��¼������
                {
                    Load1MultipleRecord(keyvalues);
                }
                else // �Լ�¼��Ϊ�������֯��ʽ�����Զ಻ͬ��¼
                {
                    Load1MultipleRecordRecordMode(m_strPrimaryKeys, keyvalues, m_strDBPrimaryKeys);
                }
            }

            SetEvents(AllEvents);
            m_initFinished = true;

            return this;
        }

        // ���ɱ��޸ĵ�Ĭ��ֵ��list����Ϊbaseform�ĸ������ݡ�
        public void GenerateModifiedDefaultList(CustomProperty prop, ref ArrayList lsTblNeedUpdateDefault)
        {
            // �ж� m_findkeys, �Ƿ����޸�Ĭ��ֵ
            // ���е�tablename, ��ֵ, ��ֵ, 
            bool bThisPropInDefaultRow = true;

            if (m_findkeys != null)
            {
                foreach (object key in m_findkeys)
                {
                    if (key.ToString().Trim() != "0")
                    {
                        bThisPropInDefaultRow = false;
                        break;
                    }
                }
            }
            else
            {
                if (prop.Tag is object[])
                {
                    foreach (object obj in (object[])(prop.Tag))
                    {
                        if (obj.ToString().Trim() != "0")
                        {
                            bThisPropInDefaultRow = false;
                            break;
                        }
                    }
                }
            }

            if (bThisPropInDefaultRow)
            {
                bool bAlreadyAdd = false;
                foreach (TblUpdateDefault tud in lsTblNeedUpdateDefault)
                {
                    if (tud.tblName == m_strMainTableName)
                    {
                        bAlreadyAdd = true;
                        break;
                    }
                }

                if (!bAlreadyAdd)
                {
                    TblUpdateDefault tud = new TblUpdateDefault();
                    tud.tblName = m_strMainTableName;
                    //DataTable tbl = Helper.GetDataTable("SELECT * FROM " + m_strMainTableName, Conn);
                    DataTable tbl = Helper.GetDataTableProxy(m_strMainTableName, Conn);
                    DataRow row = tbl.Rows.Find(PreProcessKeys(m_findkeys));
                    tud.oldValue = row;
                    DataRow newrow = m_tbl_MainData.Rows.Find(PreProcessKeys(m_findkeys));
                    tud.newValue = newrow;
                    lsTblNeedUpdateDefault.Add(tud);
                }
            }
        }

        public bool DeleteRecord(CustomProperty prop)
        {
            string[] findKeys;
            if (prop.Tag == null)
            {
                DBCustomClass cls = prop.Parent as DBCustomClass;
                findKeys = (string[])cls.FindKeys;
            }
            else
                findKeys = (string[])prop.Tag;

            if (findKeys != null)
            {
                //SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(m_adp);
                //m_adp.DeleteCommand = cmdBuilder.GetDeleteCommand();
                DataRow delRow = m_tbl_MainData.Rows.Find(PreProcessKeys(findKeys));
                if (delRow == null)
                {
                    MessageBox.Show(string.Format("��[{0}]��ɾ����¼ʧ�ܡ�", MainTable), "����",
                            MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                delRow.Delete();
                //int nRet = m_adp.Update(new DataRow[] { delRow });
                Helper.SaveTable(m_tbl_MainData, StrSql, MainForm.conn);
                m_tbl_MainData.AcceptChanges();
                DBCustomClass cls = this;
                while (cls.ParentFieldType != 2 && cls.Parent != null)
                {
                    if (cls.Parent is DBCustomClass)
                    {
                        cls = (DBCustomClass)(cls.Parent);
                    }
                }
                CustomClassCache.DelAllDataTableCache(); //�������
                if (cls.Property == null) //�����ֶ��ڶ���
                {
                    cls.GetDynamicClass(cls.FindKeys);
                    cls.OnAsynNotifyEvent(1, null);//todo:���ҳ����ͬһ��ͬһ�ֶ�ʱ���ᵼ���ظ�ˢ�¶��                    
                }
                else
                    cls.Property.SetValue(cls.Value);

                {
                    DBCustomClass parentcls = prop.Parent as DBCustomClass;
                    if (parentcls != null)
                        parentcls = parentcls.Parent as DBCustomClass;

                    if (parentcls.Property != null)
                    {
                        LuaFunction fun2 = m_lua.GetFunction(GetRealFunctionName(parentcls.Property.Key, "onafterdelrecord"));
                        if (fun2 != null)
                        {
                            object[] retobjs = fun2.Call();
                        }
                    }
                }

                return true;
            }
            return false;
        }               

        // �����ݱ��������ݱ���
        public bool Save(ref ArrayList lsTblNeedUpdateDefault, string strRemarks)
        {
            // ��Ԥ�������е�����㣬�����������ǰ�޸Ĺ�row�е����ݣ������𲢷�����
            int nModifiedCount = 0;
            List<bool> shouldSerializeValueList = new List<bool>();

            for (int i = 0; i < this.Count; i++)
            {
                CustomProperty prop = this[i];
                DBCustomClass dbCustomClass = prop.Value as DBCustomClass;

                if (prop.ValueType == enumValueType.vtExpandNode && dbCustomClass != null)
                {
                    if (dbCustomClass.ValueChanged) // ֵ�ı��ʱ�����Ҫ����
                    {
                        bool bok = dbCustomClass.Save(ref lsTblNeedUpdateDefault, strRemarks);

                        if (!bok)
                        {
                            Helper.AddLog(string.Format("{0}����������⡣", prop.Name));
                        }
                    }                    
                }

                if (prop.ShouldSerializeValue())
                {
                    shouldSerializeValueList.Add(true);
                    nModifiedCount++;
                }
                else
                {
                    shouldSerializeValueList.Add(false);
                }
            }

            if (nModifiedCount == 0) // ���û�иĹ�����¼����ֱ���˳�
                return true;

            // �û������ݱ�༭������Ƿ���Ĭ��ֵ�Ƚ�,����null������.
            int savetype = 0;
            string sql = string.Format("SELECT * FROM sys_table_cfg WHERE tablename='{0}'", m_strMainTableName);

            /*DataTable tbl = CustomClassCache.GetDataTableWithoutKey(sql);*/

            DataTable tbl = CustomClassCache.GetDataTableWithoutKeyProxy("sys_table_cfg", 
                string.Format("tablename ='{0}'", m_strMainTableName), null);
            
            if (tbl.Rows.Count > 0)
                savetype = Convert.ToInt32(tbl.Rows[0]["savetype"]);

            // �����������ݿ��е����ݣ���ֹ�����ͻ����
            if (ReloadDataOnSave)
            {
                CustomClassCache.DelDataTableCache(m_strSql);
               
                //m_tbl_MainData = CustomClassCache.GetDataTableWithoutKey(m_strSql);
                m_tbl_MainData = CustomClassCache.GetDataTableWithoutKeyProxy(m_strMainTableName);
                ResetPrimaryKey();
                ReloadDataOnSave = false;
            }
            
            List<DBCustomProperty> lstSavedProp = new List<DBCustomProperty>();
            DataRow MainSaveRow = null;
            
            for (int i = 0; i < this.Count; i++)
            {
                DBCustomProperty prop = this[i] as DBCustomProperty;

                if (shouldSerializeValueList[i])
                {
                    object value = null;

                    switch (prop.ValueType)
                    {
                        case enumValueType.vtCombo:
                            if (prop.Value == null || prop.Value.ToString().Trim() == "")
                            {
                                value = DBNull.Value;
                            }
                            else
                            {
                                value = prop.ComboValues[prop.Value];
                                if (value == null)
                                    value = prop.Value;
                            }
                            break;
                        case enumValueType.vtBool:
                            string sBool = prop.Value.ToString().Trim().ToLower();
                            if (m_tbl_MainData.Columns[prop.ID].DataType == typeof(System.Boolean))
                                value = sBool == "��" ? true : false;
                            else
                                value = sBool == "��" ? "1" : "0";
                            break;
                        case enumValueType.vtExpandNode:
                            if (prop.Value is DBCustomClass)
                            {
                                DBCustomClass cls = ((DBCustomClass)prop.Value);
                                //cls.Save();//Ϊ���Ⲣ�������ύ�ں���ͷ���б���
                                if (cls.ParentFieldType == 2)
                                    value = cls.Value;
                                else
                                    continue;
                            }
                            break;
                        default:
                            value = prop.Value;
                            break;
                    }

                    if (value == null)
                    {
                        value = DBNull.Value;
                    }
                    
                    DataRow CurSaveRow;
                    bool bMainSaveRow = true;
                    if (m_nParentFieldId < 0 || (m_bPrimaryKeysSameAsDB && m_bDBVirtualField) || prop.Tag == null) //���Լ�¼��Ϊ���������£�prop.tagֵΪ��
                    {
                        bMainSaveRow = true;
                        if (MainSaveRow == null)
                        {
                            MainSaveRow = m_tbl_MainData.Rows.Find(PreProcessKeys(m_findkeys)); //todo:��Ҫ���´����ݿ��ȡ���ݣ����ⷢ�������ͻ
                            DataTable tblrow = ReloadData(MainSaveRow, m_strMainTableName);
                            MainSaveRow = tblrow.Rows[0];
                        }
                        CurSaveRow = MainSaveRow;
                    }
                    else
                    {
                        bMainSaveRow = false;
                        CurSaveRow = m_tbl_MainData.Rows.Find(PreProcessKeys((string[])prop.Tag));//��ÿ�����Կ������Բ�ͬ��¼
                        DataTable tblrow = ReloadData(CurSaveRow, m_strMainTableName);
                        CurSaveRow = tblrow.Rows[0];
                    }
                    if (CurSaveRow == null)
                    {
                        //CurSaveRow.RejectChanges(); //����������ݿⲢ�����ʴ���
                        string message = "����ʱ�����ڴ����п������û�ֱ���޸�������ֵ���º����޸ĵ�ֵ�޷����ҵ���صļ�¼��";
                        MessageBox.Show(message);
                        return false; //�п������û�ֱ���޸�������ֵ�����º����޸ĵ�ֵ�޷����ҵ���صļ�¼
                    }



                    if (!SavePropertyData(CurSaveRow, prop, value, savetype, strRemarks, bMainSaveRow))
                    {
                        return false;
                    }
                    lstSavedProp.Add(prop);                    
                }
                
                // Ĭ��ֵ���
                if (MainForm.m_bModifyDbTableDefault)
                    GenerateModifiedDefaultList(prop, ref lsTblNeedUpdateDefault);
            }

            CommitSave(strRemarks, MainSaveRow);
            
            // �Ƚ���
            if (!this.OnLockRecord(false, null, this.FindKeys))
            {
                Helper.AddLog(string.Format("{0}�����ʧ�ܡ�", this.ToString()));
                return false;
            }

            // ˢ�°汾�� add by suntao
            this.RefreshLastVersion();            
            
            //PostSave ��Ҫ������ύ�������Լ��һ�Σ����������ֵ���Ķ�������Ҫ������ͬ��ͬ��¼������tag(findkey)��һ�θĶ���������������Ի����Ҳ�����¼����ʧ��
            RefreshFindKeys(ref lstSavedProp);
            lstSavedProp.Clear();
            m_valueChanged = false; // ���ñ���

            return true;
        }

        private DataTable ReloadData(DataRow row, string tblname)
        {
            DataColumn[] keyColumns = row.Table.PrimaryKey;
            string filter = string.Empty;
            foreach (DataColumn column in keyColumns)
            {
                filter += string.Format("{0} = {1} and ", column.ColumnName, row[column]);  
            }
            filter = filter.Substring(0, filter.Length - 4);
            string sql = string.Format("select * from {0} where {1}", tblname, filter);
            DataTable tbl = Helper.GetDataTableProxy(tblname, filter, null, MainForm.conn);
            tbl.ExtendedProperties["sql"] = sql;
            return tbl;
        }

        //��ȡstrfieldname�Ƿ����������������򷵻���m_strDBPrimaryKeys�����е�������
        private int GetKeyIndex(string strfieldname)
        {
            //���ͬʱ���Ƕ����DBCLASS(parentfield=-1),����Ҫ�ڱ����ͬʱ֪ͨģ������FINDKEYS�����˱仯
            string strkeyfield = strfieldname.ToLower();
            for (int i = 0; i < m_strDBPrimaryKeys.Length; i++)
            {
                string strkey = m_strDBPrimaryKeys[i].ToLower();
                if (strkey == strkeyfield)
                {
                    return i;
                }
            }
            return -1;
        }
        //�ж���������ֵ�����Ƿ���ͬ
        private bool IsSameKey(object[] keyvalue1, object[] keyvalue2)
        {
            if (keyvalue1.Length != keyvalue2.Length)
                return false;

            for (int i = 0; i < keyvalue1.Length; i++)
            {
                if (keyvalue1[i].ToString() != keyvalue2[i].ToString())
                    return false;
            }
            return true;
        }
        //ʵʩ�ݹ���Ҳ��滻prop.tag�б����FindKeyValues
        public void ReplacePropertyKey(string strMainTableName, object[] oldfindkeyvalues, object[] newfindkeyvalues)
        {
            foreach (DBCustomProperty subprop in this)
            {
                if (subprop.Value != null && subprop.Value is DBCustomClass)
                {
                    (subprop.Value as DBCustomClass).ReplacePropertyKey(strMainTableName, oldfindkeyvalues, newfindkeyvalues);
                }
                else if (strMainTableName == this.MainTable && subprop.Tag != null)
                {
                    object[] subpropfindkeyvalues = subprop.Tag as object[];
                    if (IsSameKey(oldfindkeyvalues, subpropfindkeyvalues))
                    {
                        subprop.Tag = newfindkeyvalues;
                    }
                }

            }
        }
        //ˢ�µ�ǰҳ������ͬ��ͬ��¼������prop.Tag�м�¼��FindKeyValues
        private void RefreshFindKeys(ref List<DBCustomProperty> lstSavedProp)
        {
            foreach (DBCustomProperty prop in lstSavedProp)
            {
                int nKeyIndex = GetKeyIndex(prop.ID);
                if (!(prop.Value is DBCustomClass) && nKeyIndex >= 0) //����������ֶ��޸ģ��������Ҫˢ��ͬһ�����������ͬһ��¼�� prop.tag
                {
                    if (prop.Tag != null && prop.Tag is object[])
                    {
                        object[] propfindkeyvalues = prop.Tag as object[];
                        object[] oldfindkeyvalues = propfindkeyvalues.Clone() as object[];

                        propfindkeyvalues[nKeyIndex] = prop.Value.ToString();

                        DBCustomClass cls = this;
                        while (cls.Parent != null)
                        {
                            cls = cls.Parent as DBCustomClass;
                        }
                        //ʹ�������� DBClass ���� tag �滻
                        cls.ReplacePropertyKey(this.MainTable, oldfindkeyvalues, propfindkeyvalues);
                    }
                }
            }
        }

        private void CommitSave(string strRemarks, DataRow saverow)
        {

            if (saverow != null)
            {
                DataTable tbl = saverow.Table;
                string sql = tbl.ExtendedProperties["sql"] as string;
                Helper.SaveTable(tbl, sql, m_conn);

                foreach (DBCustomProperty prop in mRecordProList)
                {
                    prop.UpdateDefaultValue();
                }

                foreach (string[] record in mRecordList)
                {
                    DataRecord dataRecord = DataRecord.GetDataRecord();
                    dataRecord.Conn = m_conn; // sql����
                    dataRecord.ModelID = m_nModlId; // ģ��id
                    dataRecord.TabID = m_nModlTabId; // tabҳid
                    dataRecord.CurrentNode = m_lua["SelectedNode"] as TreeNode; // �����
                    dataRecord.Time = DateTime.Now; // ��ǰ�޸�ʱ��
                    dataRecord.UserName = Helper.GetHostName(); // ��ǰ�û���
                    dataRecord.OldValue = record[0]; // �ֶεľ�ֵ        
                    dataRecord.NewValue = record[1]; // �ֶε���ֵ
                    dataRecord.FieldDisplayName = record[2]; // �ֶε�ȫ·��
                    dataRecord.FieldID = int.Parse(record[3]); // �ֶε�id
                    dataRecord.Remarks = strRemarks;
                    dataRecord.Save();
                }

                
            }

            mRecordList.Clear();
            mRecordProList.Clear();
            try
            {
                Helper.SaveTable(m_tbl_MainData, m_strSql, Conn);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
                Helper.AddLog(ex.Message);
                throw ex;             
            }
        }
        private bool SavePropertyData(DataRow SaveRow, DBCustomProperty prop, object value, int savetype, string strRemarks, bool bMainSaveRow)
        {
            string oldValue = SaveRow[prop.ID].ToString(); // ��ֵ
            if (oldValue == "") // Ϊ�գ�ȡĬ��ֵ
            {
                DataRow defaultRow = CustomClassCache.GetPropDefaultValue(m_strMainTableName);
                if (defaultRow != null) // Ĭ���зǿ�
                {
                    oldValue = defaultRow[prop.ID].ToString();
                }
            }

            //���浽���ݱ���
            if (prop.ValueType != enumValueType.vtUnknown)
            {
                try
                {
                    SaveRow[prop.ID] = value;

                    DataRow def_row = CustomClassCache.GetPropDefaultValue(m_strMainTableName);
                    if (def_row != null)
                    {
                        bool IsProcessingDefaultRow = true;

                        if (m_findkeys != null)
                        {
                            foreach (object obj in m_findkeys)
                            {
                                if (obj.ToString().Trim() != "0")
                                {
                                    IsProcessingDefaultRow = false;
                                    break;
                                }
                            }
                        }
                        else if (prop.Tag is object[])
                        {
                            foreach (object obj in (object[])(prop.Tag))
                            {
                                if (obj.ToString().Trim() != "0")
                                {
                                    IsProcessingDefaultRow = false;
                                    break;
                                }
                            }
                        }

                        if (!IsProcessingDefaultRow) // �������ı�������default row�����Բ��ô���
                        {
                            object def_val = def_row[prop.ID];

                            bool bPropIsCat = false;
                            if (m_strCats != null)
                            {
                                foreach (string cat in m_strCats)
                                {
                                    if (prop.ID == cat)
                                    {
                                        bPropIsCat = true;
                                        break;
                                    }
                                }
                            }

                            // ���������, Ҳ���غ�Ĭ��ֵ�Ƚ���, ����� DBNull
                            bool bPropIsPrimaryKeys = false;
                            foreach (DataColumn col in m_tbl_MainData.PrimaryKey)
                            {
                                if (col.ColumnName.ToLower() == prop.ID.ToLower())
                                {
                                    bPropIsPrimaryKeys = true;
                                    break;
                                }
                            }

                            if (def_val.ToString() == value.ToString() && !bPropIsCat && savetype == 0 && !bPropIsPrimaryKeys)
                                SaveRow[prop.ID] = DBNull.Value;
                        }
                    }
                }
                catch (Exception ex)
                {
                    SaveRow[prop.ID] = DBNull.Value;
                    Helper.AddLog(ex.Message);
                }
            }


            // ����ֵ����ֵͳһת��Ϊtrue��false
            string newValue = value.ToString(); // ��ֵ
            if (prop.ValueType == enumValueType.vtBool) // ����Ƿ������ֵ����ʾֵͳһ��Ϊ�Ǻͷ�
            {
                oldValue = DataRecord.TranslateValue(oldValue);
                newValue = DataRecord.TranslateValue(newValue);
            }
            oldValue = oldValue.Replace("'", "''");
            newValue = newValue.Replace("'", "''");

            if (!bMainSaveRow)
            {

                DataTable tbl = SaveRow.Table;
                string sqlrow = tbl.ExtendedProperties["sql"] as string;
                if (Helper.SaveTable(tbl, sqlrow, MainForm.conn) < 1)
                {
                    return false;
                }

                //�޸��ڴ���ʾ�õ�Ĭ��ֵ
                /*if (prop.Value is DBCustomClass)
                    prop.DefaultValue = ((DBCustomClass)prop.Value).Value;
                else
                    prop.DefaultValue = prop.Value;
                */
                prop.UpdateDefaultValue();



                // ��¼���ֵ
                DataRecord dataRecord = DataRecord.GetDataRecord();
                dataRecord.Conn = m_conn; // sql����
                dataRecord.ModelID = m_nModlId; // ģ��id
                dataRecord.TabID = m_nModlTabId; // tabҳid
                dataRecord.CurrentNode = m_lua["SelectedNode"] as TreeNode; // �����
                dataRecord.Time = DateTime.Now; // ��ǰ�޸�ʱ��
                dataRecord.UserName = Helper.GetHostName(); // ��ǰ�û���
                dataRecord.OldValue = oldValue; // �ֶεľ�ֵ        
                dataRecord.NewValue = newValue; // �ֶε���ֵ
                dataRecord.FieldDisplayName = GetFullDisplayName(prop); // �ֶε�ȫ·��
                dataRecord.FieldID = int.Parse(prop.Key); // �ֶε�id
                dataRecord.Remarks = strRemarks;
                dataRecord.Save();
            }
            else
            {
                mRecordProList.Add(prop);
                string[] record = new string[4];
                record[0] = oldValue;
                record[1] = newValue;
                record[2] = GetFullDisplayName(prop);
                record[3] = prop.Key;
                mRecordList.Add(record);
            }

            return true;
        }

        /// <summary>
        /// ��ȡ�ֶε�ȫ·����
        /// </summary>
        /// <param name="dbClass">��ǰDBClass</param>
        /// <returns>ȫ·����</returns>
        private string GetFullDisplayName(DBCustomProperty prop)
        {
            
            string fullDisplayName = prop.Name.Trim(); // ��������ʾ����
            DBCustomClass parent = prop.Parent as DBCustomClass;

            while (parent != null && parent.Property != null && parent.ParentFieldID > 0) // ���ڸ��ֶ��Ҳ�Ϊ����
            {
                fullDisplayName = string.Format("{0}.", parent.Property.Name.Trim()) + fullDisplayName;
                parent = parent.Parent as DBCustomClass;
            }

            return fullDisplayName;
        }

        public void SaveScript(CustomProperty prop)
        {
            //����Ǵӱ����������,��Ҫ�ҵ���ȷ�شӱ���
            if (prop != null && prop.Parent is DBCustomClass && prop.Parent != this)
            {
                //((DBCustomClass)(prop.Parent)).SaveScript(prop);
                DBCustomClass cls = GetTopParent(prop);
                if (this != cls)
                {
                    cls.SaveScript(prop);
                    return;
                }
            }
            string strFieldid = prop == null ? "0" : prop.Key;
            string strFieldName = prop == null ? "#" : prop.ID;
            string strMainTable = prop == null ? "" : (prop.Parent as DBCustomClass).MainTable;

            DBProcess.SaveScriptByID(strMainTable, strFieldid, m_nModlTabId.ToString(), m_luacodes[strFieldName].ToString());
            CustomClassCache.DelAllCaches();
#if false

            string ssql = string.Format("SELECT * FROM sys_scriptstore WHERE fieldid={0} AND modtabid = {1} AND tablename='{2}' AND fieldname='{3}'", strFieldid, m_nModlTabId, m_strMainTableName, strFieldName);
            SqlDataAdapter adp = new SqlDataAdapter(ssql, m_conn);
            System.Data.DataSet DS = new System.Data.DataSet();
            adp.Fill(DS);

            //���ж����ݿ����Ƿ���ڼ�¼
            //���ڣ������
            DataTable tbl = DS.Tables[0];
            if (tbl.Rows.Count > 0)
            {
                DataRow row = tbl.Rows[0];
                row["fieldid"] = strFieldid;
                row["modtabid"] = m_nModlTabId;
                row["tablename"] = m_strMainTableName;
                row["fieldname"] = strFieldName;
                row["scriptlanguage"] = "LUA";
                row["scriptcode"] = Helper.GetByteArrayFromString(m_luacodes[strFieldName].ToString());
                //ʹ��SqlCommandBuilder  �������SqlDataAdapter ��InsertCommand��DeleteCommand��UpdateCommand����
                SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                adp.UpdateCommand = cmdBuilder.GetUpdateCommand();

                int val = adp.Update(tbl);
                tbl.AcceptChanges();
            }
            //�����ڣ����½���¼
            else
            {
                DataRow row = tbl.NewRow();
                row["fieldid"] = strFieldid;
                row["modtabid"] = m_nModlTabId;
                row["tablename"] = m_strMainTableName;
                row["fieldname"] = strFieldName;
                row["scriptlanguage"] = "LUA";
                row["scriptcode"] = Helper.GetByteArrayFromString(m_luacodes[strFieldName].ToString());
                tbl.Rows.Add(row);
                SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                adp.InsertCommand = cmdBuilder.GetInsertCommand();

                int val = adp.Update(tbl);
                tbl.AcceptChanges();
            }
#endif
        }

        public string GetFieldStr(System.Data.DataRow row, string strFieldName)
        {
            return row[strFieldName].ToString().Trim().ToLower();
        }

        public string GetFieldStrNoLower(System.Data.DataRow row, string strFieldName)
        {
            return row[strFieldName].ToString().Trim();
        }

        public Hashtable GetListFieldExArray(string strfieldex)
        {
            if (strfieldex.Trim().Length <= 0)
                return null;
            Hashtable hs = new Hashtable();
            string[] strLines = strfieldex.Split(new Char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
            foreach (string strLine in strLines)
            {
                hs[strLine.Trim().ToLower()] = "";
            }
            return hs;
        }

        // ���������г��ֵ����ݽ���Ԥ����,�����ݿ��е��ֵ��ֵȫ��������Ԥ�浽hashtable��
        private void FillComboList(string fieldnamekey, System.Data.DataRow row, SqlConnection conn)
        {
            // listtable strliscondition strlistf strlistk strlistfex 
            string strlisttable = GetFieldStr(row, "listtable");
            string strliscondition = row["listcondition"].ToString().Trim();
            string strlistf = GetFieldStr(row, "listfield");
            string strlistk = GetFieldStr(row, "keyfield");
            string strlistfex = GetFieldStr(row, "listexfields");
            string strCacheKey = string.Format("{0},{1},{2},{3},{4}", strlisttable, strliscondition, strlistf,
                                               strlistk, strlistfex);

            Hashtable[] ComboCaches = CustomClassCache.GetComboCaches(strCacheKey, this);            
            m_combox_Keys[fieldnamekey] = ComboCaches[0];
            m_combox_Values[fieldnamekey] = ComboCaches[1];
            m_combox_ExValues[fieldnamekey] = ComboCaches[2];
        }

        // ���������г��ֵ����ݽ���Ԥ����,�����ݿ��е��ֹ�������ֵ��ֵȫ��������Ԥ�浽hashtable��
        private void FillTextComboList(string fieldnamekey, DataRow row)
        {
            string strlistvalues = row["listvalues"].ToString().Trim();
            string[] strLines =  strlistvalues.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            object[] strdescs = (object[])m_colName[fieldnamekey];
            string strfieldname = strdescs[8].ToString();

            Hashtable comhashkeys = new Hashtable();
            Hashtable comhashvalues = new Hashtable();

            if (strLines == null)
            {
                MessageBox.Show("���ֶ� " + fieldnamekey + " ѡ��Ϊ<�ı�������>, ����û����ȷ������.", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return; 
            }

            foreach (string line in strLines)
            {
                string[] strkv = line.Split(new Char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                if (strkv != null && strkv.Length > 1)
                {
                    string key = strkv[0];
                    string value = strkv[1];
                    comhashkeys[value] = key;
                    comhashvalues[key] = value;
                }
                else
                {
                    string s = string.Format("�ڶ�ȡ {0} �ֶ� ListValues ����ʱ���ִ������޸�Ԫ��Ϣ�������ݱ�", strfieldname);
                    System.Windows.Forms.MessageBox.Show(s);
                    throw new Exception("�����ListValues");
                }
            }

            m_combox_Keys[fieldnamekey] = comhashkeys;
            m_combox_Values[fieldnamekey] = comhashvalues;
            m_combox_ExValues[fieldnamekey] = null;
        }

        public int GetRealPropertySize()
        {
            int nCount = 0;
            CustomProperty prop = null;
            for (int i = 0; i < this.Count; i++)
            {
                prop = (CustomProperty)this[i];
                if (prop.Visible == true)
                {
                    if (prop.Value is DBCustomClass)
                    {
                        DBCustomClass cls = (DBCustomClass)prop.Value;
                        //if ((subcls.ParentFieldType == 1 || subcls.ParentFieldType == 0) && subcls.RecordCount == 1)
                        if (prop.IsCanHideNode && (Count == 1 || cls.Count == 1))
                        {
                            nCount += cls.GetRealPropertySize();
                            continue;
                        }
                        
                    }
                    nCount++;
                }
            }
            return nCount;
        }
        private Attribute[] ToAttribute(ArrayList attrs)
        {
            return (System.Attribute[])attrs.ToArray(typeof(Attribute));
        }
        public new PropertyDescriptorCollection GetProperties(Attribute[] attributes)
        {
            CustomProperty prop = null;
            CustomProperty subProp = null;
            int nPropCount = GetRealPropertySize();
            //PropertyDescriptor[] newProps = new PropertyDescriptor[this.Count];
            PropertyDescriptor[] newProps = new PropertyDescriptor[nPropCount];
            int nCount = 0;

            try
            {
                for (int i = 0; i < this.Count; i++)
                {
                    prop = (CustomProperty)this[i];
                    if (prop.Visible == true)
                    {
                        if (prop.Value is DBCustomClass)
                        {
                            DBCustomClass cls = (DBCustomClass)prop.Value;
                            //if ((cls.IsVirtualField && cls.IsPrimaryKeysSameAsDB && cls.RecordCount > 0) || cls.RecordCount == 1) //����ֻ����Ψһƥ��ļ�¼����������㣬�������µ�����ȫ��������һ��
                            //if ((cls.IsVirtualField && cls.IsPrimaryKeysSameAsDB) || cls.RecordCount == 1) //����ֻ����Ψһƥ��ļ�¼����������㣬�������µ�����ȫ��������һ��
                            //ParentFieldType == 0 ��ʾ�Զ����ɵ���ͨ�ֶ���һ�Զ�ĵļ�¼, 1��ʾ�����ֶΣ������Ǵӱ���ֶ�
                            //if ((cls.ParentFieldType == 1 || cls.ParentFieldType == 0) && cls.RecordCount == 1) //ֻ�������ֶμ����Զ�ſ��Խ���¼����һ��
                            if (prop.IsCanHideNode && (Count == 1 || cls.Count == 1)) //
                            {
                                for (int j = 0; j < cls.Count; j ++ )
                                {
                                    subProp = cls[j];
                                    if (subProp.Visible)
                                    {

                                        //if (this.SortType == 1)
                                        //    subProp.Name = string.Format("[{0}] {1}", prop.Name, subProp.Name);
                                        if (subProp.Value is DBCustomClass && (subProp.Value as DBCustomClass).ParentFieldType == 2)
                                        {
                                            ArrayList attrlist = new ArrayList();
                                            attrlist.AddRange(attributes);
                                            attrlist.Add(new EditorAttribute(typeof(CustomEditor), typeof(System.Drawing.Design.UITypeEditor))); //��ָ̬���༭��
                                            Attribute[] attrArray = ToAttribute(attrlist);// (System.Attribute[])attrs.ToArray(typeof(Attribute));
                                            if (subProp.DefaultValue == null)
                                                subProp.DefaultValue = subProp.Value;
                                            newProps[nCount] = new CustomPropertyDescriptor(ref subProp, attrArray);
                                        }
                                        else
                                        {
                                            if (subProp.DefaultValue == null)
                                                subProp.DefaultValue = subProp.Value;
                                            newProps[nCount] = new CustomPropertyDescriptor(ref subProp, attributes);
                                        }
                                        nCount++;
                                    }
                                }
                                continue;
                            }
                            else
                            {
                                ArrayList attrlist = new ArrayList();
                                attrlist.AddRange(attributes);
                                if ((cls.IsVirtualField || cls.ParentFieldType != 2) && Parent != null) //ֻ�зǶ��������ֶβŽ�ֹ�½�����
                                    attrlist.Add(new EditorAttribute(typeof(TextEditor), typeof(System.Drawing.Design.UITypeEditor)));
                                else
                                {
                                    attrlist.Add(new EditorAttribute(typeof(CustomEditor), typeof(System.Drawing.Design.UITypeEditor)));
                                    //attrlist.Add(new DefaultValueAttribute(typeof(int), prop.Value.ToString()));
                                }
                                Attribute[] attrArray = ToAttribute(attrlist);// (System.Attribute[])attrs.ToArray(typeof(Attribute));
                                if (prop.DefaultValue == null)
                                    prop.DefaultValue = prop.Value;
                                newProps[nCount] = new CustomPropertyDescriptor(ref prop, attrArray);
                                nCount++;
                                continue;
                            }
                        }
                        /*
                        else
                        {
                            ArrayList attrlist = new ArrayList();
                            attrlist.AddRange(attributes);
                            attrlist.Add(new DefaultValueAttribute(prop.Value));
                            Attribute[] attrArray = ToAttribute(attrlist);// (System.Attribute[])attrs.ToArray(typeof(Attribute));
                            newProps[nCount] = new CustomPropertyDescriptor(ref prop, attrArray);
                            nCount++;
                            continue;
                        }*/
                        if (prop.DefaultValue == null)
                            prop.DefaultValue = prop.Value;
                        newProps[nCount] = new CustomPropertyDescriptor(ref prop, attributes);
                        nCount++;
                    }
                    else
                    {
                        if (prop.DefaultValue == null)
                            prop.DefaultValue = prop.Value;
                    }
                }
            }
            catch (Exception ex)
            {
                string s = string.Format("�ڶ�ȡ {0} �ֶ�ʱ���ִ������޸�Ԫ��Ϣ�������ݱ�\r\n����ѶϢΪ:{1}", prop.ID, ex.Message);
                System.Windows.Forms.MessageBox.Show(s);
            }

            return new PropertyDescriptorCollection(newProps);
        }


        #region "�ű�ʵ�����"
        //��ȡ��ǰ������ģ̣գ�����
        public Hashtable ScriptCode
        {
            get { return m_luacodes; }
        }
        public Hashtable ScriptExecState
        {
            get { return m_scriptExecState; }
        }

        //�ҵ���Ч�ĸ����,����1�Զ�ʱ,ÿ����¼�Ľű��洢λ��Ӧ�������������ֶν��, ���������ֶ���Ϊ���Ľ����
        private DBCustomClass _GetTopParent(DBCustomClass cls)
        {
            if (cls.IsVirtualField) return cls;
            if (cls.Parent is DBCustomClass)
            {
                return _GetTopParent((DBCustomClass)cls.Parent);
            }
            else
            {
                return null;
            }
        }
        private DBCustomClass GetTopParent(object prop)
        {
            DBCustomClass retcls = _GetTopParent((DBCustomClass)((CustomProperty)(prop)).Parent);
            if (retcls == null)
                retcls = this;
            return retcls;
        }
        public void  SetScriptCode(CustomProperty prop, string strCode)
        {
            string strfnID;
            if (prop == null)
                strfnID = "#";
            else
                strfnID = prop.ID;

            DBCustomClass cls = null;
            if (strfnID == "#")
            {
                cls = this;
            }
            else if (prop.Parent is DBCustomClass)
            {
                cls = GetTopParent(prop);
                //if (prop.Parent != this)
                //    cls = ((DBCustomClass)(prop.Parent));
                //else
                //    cls = this;
            }

            cls.ScriptCode[strfnID] = strCode;
            cls.ScriptExecState[strfnID] = false;
        }
        public string GetScriptCode(CustomPropertyDescriptor desc)
        {
            string strfnID;
            if (desc == null)
                strfnID = "#";
            else
                strfnID = desc.Property.ID;

            DBCustomClass cls;
            if (strfnID == "#")
            {
                cls = this;
            }
            else if (desc.Property.Parent is DBCustomClass)
            {
                //cls = ((DBCustomClass)(desc.Property.Parent));
                cls = GetTopParent(desc.Property);
            }
            else
                return "";
            
            object ocode = cls.ScriptCode[strfnID];
            if (ocode == null)
                return "";
            else
                return ocode.ToString();
        }

        private object OnGetTreeValueEvent(CustomClass sender, TreeValueEventArgs e)
        {
            CustomClass parent = sender;
            CustomProperty prop = e.tree;
            object svalue = null;
            try
            {
                if (m_lua != null && prop.ID != null)
                {
                    this.DoScript(prop);
                    LuaFunction fun = m_lua.GetFunction(GetRealFunctionName(prop.Key, "ongettreevalue"));//"__fname__" + prop.Key + ".ongetreevalue");
                    if (fun != null)
                    {
                        CustomClass child = (CustomClass)e.tree.Value;
                        svalue = child.Value;//e.tree.Description;
                        object[] retobjs = fun.Call(prop, e);
                        if (retobjs != null && retobjs.GetLength(0) > 0)
                        {
                            if (retobjs[0] is bool)
                            {
                                if ((bool)retobjs[0] == true && retobjs.GetLength(0) > 1)
                                    return retobjs[1];
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                //ScriptDebugForm frm = Program.MainForm.DebugForm;
                //frm.OutputBox.Text += ex.Message + "\r\n";
                //frm.Show();
                //frm.BringToFront();
                Helper.AddLog(ex.Message);
            }
            return svalue;//ȡ���޸�ֵ������δ����ֵ
        }

        private object OnCustomEditorEditValueEvent(CustomProperty sender, CustomEditorEventArgs e)
        {
            CustomProperty prop = sender;

            string svalue;
            if (e.value is DBCustomClass)
                svalue = ((DBCustomClass)e.value).Value.ToString();
            else
                svalue = e.value.ToString();
            
            try
            {
                if (m_lua != null)
                {
                    DoScript(prop);
                    
                    //���������㣬��Ҫ����������¼�����֧���½��ӱ��¼
                    if (prop.Value is DBCustomClass)
                    {
                        LuaFunction fun = m_lua.GetFunction(GetRealFunctionName(prop.Key, "neednewrecord"));//"__fname__" + prop.Key + ".oneditvalue");
                        if (fun != null)
                        {
                            object[] retobjs = fun.Call(prop, e);
                            if (retobjs != null && retobjs.GetLength(0) > 0)
                            {
                                if (retobjs[0] is bool && (bool)retobjs[0])
                                {
                                    NewRecord(prop);
                                }
                            }
                        }
                    }
                    //else
                    {
                        LuaFunction fun = m_lua.GetFunction(GetRealFunctionName(prop.Key, "oneditvalue"));//"__fname__" + prop.Key + ".oneditvalue");
                        if (fun != null)
                        {
                            object[] retobjs = fun.Call(prop, e);
                            if (retobjs != null && retobjs.GetLength(0) > 0)
                            {
                                if (retobjs[0] is bool)
                                {
                                    if ((bool)retobjs[0] == true)
                                    {
                                        if (retobjs.GetLength(0) == 2)
                                        {
                                            //prop.DisplayValue = null;
                                            return retobjs[1];
                                        }
                                        else if (retobjs.GetLength(0) > 2)
                                        {
                                            //prop.DisplayValue = retobjs[2];
                                            //prop.IsClearDisplayValue = false;
                                            return retobjs[1];
                                        }
                                    }
                                }
                            }
                            return e.value;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                //ScriptDebugForm frm = Program.MainForm.DebugForm;
                //frm.OutputBox.Text += ex.Message + "\r\n";
                //frm.Show();
                //frm.BringToFront();
                Helper.AddLog(ex.Message);
            }
            return e.value;//ȡ���޸�ֵ������δ����ֵ
        }
        public void NewRecord(CustomProperty prop)
        {
            if (prop.Value is DBCustomClass)
            {
                string svalue = ((DBCustomClass)prop.Value).Value.ToString();
                NewRowHelper helper = new NewRowHelper(prop);
                if (MessageBox.Show(string.Format("��ȷ��Ҫ��[{0}]�����½���¼��", helper.NewRowTableName), "ȷ��",
                    MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2) == DialogResult.Yes)
                {
                    helper.GernateDefaultValue();
                    LuaFunction fun1 = m_lua.GetFunction(GetRealFunctionName(prop.Key, "onnewrecord"));//"__fname__" + prop.Key + ".oneditvalue");
                    DBCustomClass cls = prop.Value as DBCustomClass;
                    if (fun1 != null)
                    {
                        //object[] retobjs = fun1.Call(prop, svalue, helper.KeyValues, helper.SubProperty.ID); //��Ҫ����ӱ�����Hash, key=�ֶ���, value=�գ��ú�����hashtable����
                        object[] retobjs = fun1.Call(prop, svalue, helper.NewRowFieldValues, helper.SubProperty.ID); //��Ҫ����ӱ�����Hash, key=�ֶ���, value=�գ��ú�����hashtable����
                        
                        //��������棬�Ϳ����½��ӱ��¼��
                        if (retobjs != null && retobjs.GetLength(0) > 0)
                        {
                            if (retobjs[0] is bool)
                            {
                                if ((bool)retobjs[0] == true)
                                {
                                    helper.NewRow(); //�����¼�¼���ύ
                                    //prop.SetValue(helper.GetNewValue(prop)); //�˺���ǿ��ˢ���������
                                    //OnAsynNotifyEvent(2, prop);
                                    helper.SetNewValue(prop);
                                }
                            }
                        }
                        //return e.value;
                    }
                    else //ֱ���½���¼
                    {
                        helper.NewRow(); //�����¼�¼���ύ
                        //prop.SetValue(helper.GetNewValue(prop)); //�˺���ǿ��ˢ���������
                        //if (prop.Value is DBCustomClass && (prop.Value as DBCustomClass).ParentFieldType == 1) //Ϊ�����ֶ����½��ӱ��¼ʱ����Ҫ getDynamicclass
                        //OnAsynNotifyEvent(2, prop);
                        helper.SetNewValue(prop);
                    }

                    LuaFunction fun2 = m_lua.GetFunction(GetRealFunctionName(prop.Key, "afternewrecord"));
                    if (fun2 != null)
                    {
                        object[] retobjs = fun2.Call();
                    }
					else
					{
	                    LuaFunction fun3 = m_lua.GetFunction(GetRealFunctionName(prop.Key, "onafternewrecord"));
    	                if (fun3 != null)
        	            {
            	            object[] retobjs = fun3.Call();
                	    }					
					}
                }
            }
        }

        private void BeforePropertyChangedEvent(CustomProperty sender, PropertyChangeEventArgs e)
        {
            if (sender is CustomProperty)
            {
                DBCustomProperty prop = (DBCustomProperty)sender;
                bool bRet = false;
                string sErrMsg = "δ����Ĵ���";
                try
                {
                    if (m_lua != null)
                    {
                        DoScript(prop);//���ܻ����������ֶΣ���ͬ���ֶ����ڿ�����ͬһTABҳ�У���Ҫ����parentfield.
                        LuaFunction fun = m_lua.GetFunction(GetRealFunctionName(prop.Key, "onbeforevaluechang"));//"__fname__" + prop.Key + ".onbeforevaluechang");
                        if (fun != null)
                        {
                            object[] retobjs = fun.Call(prop, e);
                            if (retobjs != null && retobjs.GetLength(0) > 0)
                            {
                                if (retobjs[0] is bool)
                                {
                                    bRet = (bool)retobjs[0];
                                    if (!bRet)
                                        sErrMsg = (string)retobjs[1];
                                }
                            }
                        }
                        else
                            bRet = true; //�޺�����ʾû�нű�ָ��
                    }
                }
                catch (Exception ex)
                {
                    //ScriptDebugForm frm = Program.MainForm.DebugForm;
                    //frm.OutputBox.Text += ex.Message + "\r\n";
                    //frm.Show();
                    //frm.BringToFront();
                    Helper.AddLog(ex.Message);
                    sErrMsg = "LUA����ʱ����:\r\n" + ex.Message;
                }
                if (!bRet)
                {
                    sErrMsg = sErrMsg.Replace("\\n", "\n");
                    sErrMsg = sErrMsg.Replace("\\t", "\t");
                    throw new Exception(sErrMsg);
                }
                else
                {
                    //�������������ύ�޸�
                    object[] findkeys = FindKeys == null ? (object[])prop.Tag : FindKeys;

                    // �������ƣ�ֻ����<Ĭ��ֵ>������޸Ĺҽӵ��ӱ��Ĭ���е�����
                    bool bModifyingSubTableDefaults = true;
                    foreach (object _key in findkeys)
                    {
                        if (_key.ToString() != "0")
                        {
                            bModifyingSubTableDefaults = false;
                            break;
                        }
                    }
                    if (bModifyingSubTableDefaults)
                    {
                        if (!Program.MainForm.m_CurBaseForm.BaseTree.SelectedNode.Text.Contains("Ĭ��ֵ"))
                            throw new Exception("����������ֱ���޸��ӱ�Ĭ��ֵ������ѡ��<Ĭ��ֵ>��㣬�ٽ����޸ġ�");
                        else
                        { 
                            if (!CanModifyDefaultVal())
                                throw new Exception("�������޸�Ĭ��ֵ��");
                        }
                    }

                    if (!OnLockRecord(true, prop, findkeys))
                    {
                        throw new Exception(string.Format("������¼�Ѿ���[{0}]��ģ��[{1}]����������ȴ��������ٽ����޸Ĳ�����", 
                            m_reclock.GetServerLastLockUser(findkeys), m_reclock.GetServerLastLockModID(findkeys)));
                    }
                }
            }
        }
        private void AfterPropertyChangedEvent(CustomProperty sender, PropertyAfterChangeEventArgs e)
        {
            if (sender is CustomProperty)
            {
                CustomProperty prop = (CustomProperty)sender;

                string svalue = e.value.ToString();
                try
                {
                    if (m_lua != null)
                    {
                        DoScript(prop);
                        LuaFunction fun = m_lua.GetFunction(GetRealFunctionName(prop.Key, "onaftervaluechanged"));//"__fname__" + prop.Key + ".onaftervaluechanged");
                        if (fun != null)
                            fun.Call(prop, e);
                    }
                }
                catch (Exception ex)
                {
                    //ScriptDebugForm frm = Program.MainForm.DebugForm;
                    //frm.OutputBox.Text += ex.Message + "\r\n";
                    //frm.Show();
                    //frm.BringToFront();
                    Helper.AddLog(ex.Message);
                }
                //GridItem item = propertyGrid1.SelectedGridItem;
                //CustomPropertyDescriptor desc = (CustomPropertyDescriptor)item.PropertyDescriptor;
                //CustomProperty specprop = desc.Property;          
            }
        }
        private string GetRealFunctionName(string strkey, string strfunctionname)
        {
            string strRet;
            switch (strkey)
            {
                case null:
                    goto case "##";
                case "##": //ȫ�ֻ���,��δʵ��
                    strRet = strfunctionname;
                    break;
                case "#": //TAB����
                    strRet = string.Format("__fname__tab_global_{0}.{1}", ModalTabId, strfunctionname);
                    break;
                default:
                    strRet = string.Format("__fname__{0}.{1}", strkey, strfunctionname);
                    break;
            }
            return strRet;
        }
        private string GetPrefixCode(string strkey) //����л������Ĵ���
        {
            string strcodeprefix = "";
            //�л���˽�л���
            if (m_lua != null)
            {
                /*if (strkey == "##")
                    strcodeprefix = "_ChgEnv()\r\n"; //ȫ�ֻ���
                
                else
                    strcodeprefix = string.Format("_ChgEnv(\"{0}\")\r\n", strkey);
                */
                switch (strkey)
                {
                    case "##": //ȫ�ֻ���,��δʵ��
                        strcodeprefix = "_ChgEnv()\r\n";
                        strcodeprefix = string.Format("_ChgEnv('tab_global_{0}')\r\n", ModalTabId); // kuangsihaoʵ�ֵ�ģ��ȫ��
                        break;
                    case "#": //TAB����
                        strcodeprefix = string.Format("_ChgEnv('tab_global_{0}')\r\n", ModalTabId);
                        break;
                    default:
                        strcodeprefix = string.Format("_ChgEnv('{0}', 'tab_global_{1}')\r\n", strkey, ModalTabId);
                        break;
                }
            }
            return strcodeprefix;
        }
        //ע�Ȿ����ֻ���ҵ���ǰ����ֶ���ش���,�����ڱ���֮�����
        public void DoScript(string strkey, string strFieldName)
        {
            try
            {
                //string strcodeprefix = GetPrefixCode(strkey);
                m_lua.DoString(GetPrefixCode(strkey));//������,����Ҫ����Pacage�����������ĺ����Ƿ���ڵ��ж�
                //m_lua.GetFunction("_ChgEnv").Call(strFieldName == "#" ? "" : strFieldName); //����,����ֱ�ӵ���.call,�����ᵼ��setfenv(2,xx)ʧЧ

                object fn = m_scriptExecState[strFieldName];
                if (fn != null)
                {
                    bool bExecuted = (bool)fn;
                    if (bExecuted)
                        return;
                }
                else
                {
                    //return;//û�нű����ݣ�����ִ��
                    m_scriptExecState[strFieldName] = false;
                }
                if (ScriptCode[strFieldName] == null)
                    return;
                string strcode = ScriptCode[strFieldName].ToString();
                if (m_lua != null && strcode != null && strcode.Length > 0)
                {
                    string strcodeprefix = GetPrefixCode(strkey);
                    m_lua.DoString(strcodeprefix);
                    m_lua.DoString(strcodeprefix + strcode);
                    m_scriptExecState[strFieldName] = true;
                }
            }
            catch (Exception ex)
            {
                //ScriptDebugForm frm = Program.MainForm.DebugForm;
                //frm.OutputBox.Text += ex.Message + "\r\n";
                //frm.Show();
                //frm.BringToFront();
                Helper.AddLog(ex.Message);
            }
        }
        
        public void DoScript(CustomProperty prop)
        {   
            if (prop == null)
            {
                this.DoScript("##", "##");
                this.DoScript("#", "#");
            }
            else if (prop.Parent is DBCustomClass)
            {
                //((DBCustomClass)(prop.Parent)).DoScript(prop.Key, prop.ID);
                DBCustomClass cls = GetTopParent(prop);
                cls.DoScript(prop.Key, prop.ID);
            }
        }

        public bool ExistLuaFunction(string strfuncName)
        {
            bool bRet = false;
            if (m_lua != null)
            {
                m_lua.DoString(GetPrefixCode("#"));
                LuaFunction fun = m_lua.GetFunction(GetRealFunctionName("#", strfuncName));
                if (fun != null)
                    bRet = true;
            }
            return bRet;
        }
        //�鿴ĳ�������Ƿ��к���,û�о͵�ȫ������,�ҵ���ִ��
        public object[] CallLuaFunction(CustomProperty prop, string strfuncName, object[] args)
        {            
            if (m_lua != null)
            {
                m_lua.DoString(prop == null ? GetPrefixCode("#") : GetPrefixCode(prop.Key));
                LuaFunction fun = m_lua.GetFunction(GetRealFunctionName(prop.Key, strfuncName));//"__fname__" + prop.Key + "." + strfuncName);
                if (fun != null)
                {
                    object[] retobjs = fun.Call(args);
                    return retobjs;
                }
            }
            return null;
        }

        public object[] CallLuaFunction(string strfuncName)
        {
            object[] ret = null;
            try
            {
                if (m_lua != null)
                {
                    m_lua.DoString(GetPrefixCode("#"));
                    LuaFunction fun = m_lua.GetFunction(GetRealFunctionName("#", strfuncName));
                    if (fun != null)
                        ret = fun.Call(null);
                }
            }
            catch (Exception ex)
            {
                //ScriptDebugForm frm = Program.MainForm.DebugForm;
                //frm.OutputBox.Text += ex.Message + "\r\n";
                //frm.Show();
                //frm.BringToFront();
                Helper.AddLog(ex.Message);
            }
            return ret;
        }

        public object[] CallLuaFunction(string strfuncName, object[] args)
        {
            object[] ret = null;
            try
            {
                if (m_lua != null)
                {
                    m_lua.DoString(GetPrefixCode("#"));
                    LuaFunction fun = m_lua.GetFunction(GetRealFunctionName("#", strfuncName));
                    if (fun != null)
                        ret = fun.Call(args);
                }
            }
            catch (Exception ex)
            {
                //ScriptDebugForm frm = Program.MainForm.DebugForm;
                //frm.OutputBox.Text += ex.Message + "\r\n";
                //frm.Show();
                //frm.BringToFront();
                Helper.AddLog(ex.Message);
            }
            return ret;
        }

        #endregion
        public object GetFieldValue(string strFieldName)
        {
            DataRow row = m_tbl_MainData.Rows.Find(PreProcessKeys(m_findkeyvalues));
            return row[strFieldName];
        }

        //��������ҳ��ĳ���Ա��޸�ʱ����֪ͨ��ÿ��TABҳ����Ҫ������ͬ����ͬ�ֶε�����ֵ
        public int ModifyValue(string strTable, string strField, object newValue, string findkeyvalues)
        {
            int nSameFieldCount = 0;
            //���������������ֶΣ���Ҫˢ��findkeys
            //if (this.MainTable == strTable)
            //    ModifyFindKeys(strField, newValue);

            if (this.IsVirtualField == false &&
                this.Property != null && this.ID == strField && this.MainTable == strTable &&
                newValue.ToString() != this.Value.ToString())
            {
                string thisfindkeyvalues = Helper.StringArrayToString(this.FindKeys);
                if (thisfindkeyvalues == findkeyvalues)
                {
                    this.Property.SetValue(newValue);
                    ReloadDataOnSave = true;
                    nSameFieldCount ++;
                }
            }

            try
            {
                foreach (CustomProperty prop in this)
                {
                    if (prop.ValueType == enumValueType.vtExpandNode && prop.Value is DBCustomClass)
                    {
                        DBCustomClass cls = ((DBCustomClass)prop.Value);
                        nSameFieldCount += cls.ModifyValue(strTable, strField, newValue, findkeyvalues);
                        if (cls.IsVirtualField == false &&
                            cls.Property != null && cls.ID == strField && cls.MainTable == strTable && 
                            newValue.ToString() != cls.Value.ToString())
                        {
                            string thisfindkeyvalues = Helper.StringArrayToString(cls.FindKeys);
                            if (thisfindkeyvalues == findkeyvalues)
                            {
                                cls.Property.SetValue(newValue);
                                cls.ReloadDataOnSave = true;
                                nSameFieldCount++;
                            }
                        }
                    }
                    else if (prop.ID == strField && MainTable == strTable)
                    {
                        string thisfindkeyvalues = Helper.StringArrayToString(this.FindKeys);
                        if (thisfindkeyvalues == findkeyvalues)
                        {
                            if (prop.ValueType == enumValueType.vtCombo)
                            {
                                if (prop.ComboKeys[newValue.ToString()] == null)
                                {
                                    CustomClassCache.DelComboCache();
                                    ReInitMetaInfo();
                                    prop.ComboKeys = (Hashtable)(m_combox_Values[prop.ID]);
                                }
                                prop.SetValue(prop.ComboKeys[newValue.ToString()]);
                            }
                            else
                            {
                                prop.SetValue(newValue);
                            }
                            ReloadDataOnSave = true;
                            nSameFieldCount++;
                        }
                    }
                    //if (prop.ID == strField && MainTable == strTable)
                    //{
                    //    ReloadDataOnSave = true;
                    //}
                }
            }
            catch (Exception ex)
            {
                Helper.AddLog(ex.Message);
            }
            return nSameFieldCount;
        }
        public bool OnLockRecord(bool bLock, DBCustomProperty prop, object[] findkeyvalues)
        {   
            RecordLock Lock;
            object[] keyvalue;
            int nLastServerVersion;
            if (prop == null || prop.Lock == null)
            {
                Lock = m_reclock;
                keyvalue = findkeyvalues;
                nLastServerVersion = m_reclock.LocalVersion;// m_nLastServerVersion;
            }
            else if (prop.Lock != null && prop.Tag != null)
            {
                Lock = prop.Lock;
                keyvalue = prop.Tag as object[];
                m_nRecordModifyCount = 0;
                nLastServerVersion = prop.LastVersion;
            }
            else
            {
                throw new Exception("ϵͳ�ڲ�����OnLockRecord ʧ�ܣ�����ϵ�༭�������ͬ�¡�");
            }
      
            if (bLock)
            {
                if (m_nRecordModifyCount == 0 && (!Lock.Locked))// || Lock.UnLock(keyvalue)))
                {
                    if (Lock.Lock(keyvalue))
                    {
                        m_nRecordModifyCount++;
                        int serverver = Lock.GetServerLastVersion(keyvalue);
                        if (serverver != nLastServerVersion && serverver != 0)
                        {
                            MessageBox.Show("���ڷ���������Ѿ����£�����ǰ�޸ĵ����ݻᱻ����Ϊ����˵����ݡ�", "����",
                                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            //��Ҫˢ�µ�ǰ���ݲ��ܿ�ʼ�༭
                            //ReloadCurrentInfo();
                            OnAsynNotifyEvent(4, null);
                        }
                    }
                    else
                        return false;
                }
            }
            else //����
            {
                //�Ƚ��ӱ��е���ȫ����
                if (prop == null)
                {
                    foreach (DBCustomProperty subprop in this)
                    {
                        if (subprop.ValueType == enumValueType.vtExpandNode && subprop.Value is DBCustomClass)
                        {
                            DBCustomClass cls = ((DBCustomClass)subprop.Value);

                            if (!cls.ValueChanged) // ���ֵû��Ļ��ͱ�Ҫ�Ͻ����ظ��Ľ������� add by suntao
                            {
                                cls.OnLockRecord(bLock, null, cls.FindKeys);
                            }                            
                        }
                        else if (subprop.Tag != null && subprop.Tag is object[])
                        {
                            OnLockRecord(bLock, subprop, null);
                        }
                        //subprop.UpdateLastVersion();
                    }
                }
                if (Lock.Locked)
                {
                    if (Lock.UnLock(keyvalue) > 0)
                    {
                        m_nRecordModifyCount = 0;
                        if (prop != null && prop.Lock != null && prop.Tag != null)
                            prop.UpdateLastVersion();
                        //nLastServerVersion = Lock.GetServerLastVersion(keyvalue);//����ģ�鼶,һ��ȫ����,�����TAB��İ汾�᲻һ��
                    }
                    else
                        return false;
                }
            }
            /*if (m_nRecordModifyCount > 0)
            {
                //������¼
            }
            m_nRecordModifyCount = 0;
             */
            return true;
        }
        public void RefreshLastVersion()
        {
            if (m_findkeyvalues != null)
            {
                m_nLastServerVersion = m_reclock.GetServerLastVersion(m_findkeyvalues);
            }

            foreach (CustomProperty prop in this)
            {
                DBCustomClass dbCustomClass = prop.Value as DBCustomClass;

                if (prop.ValueType == enumValueType.vtExpandNode && dbCustomClass != null && dbCustomClass.ValueChanged)
                {
                    dbCustomClass.RefreshLastVersion();                    
                }
            }
        }
        private bool CanModifyDefaultVal()
        {
            bool bResult = false;
            string strUserID = Helper.GetHostName();
            string strIP = Helper.GetIPAddress();
            string strSQL = string.Format("select _index from sys_useracl where userid ='{0}' and ip='{1}'", strUserID, strIP);

            //DataTable dtab = Helper.GetDataTable(strSQL, Conn);
            DataTable dtab = Helper.GetDataTableWithSqlProxy("sys_useracl", strSQL, Conn);
            
            if (dtab.Rows.Count <= 0)
                return true;

            string strIndex = dtab.Rows[0][0].ToString();

            strSQL = string.Format("select canmodifydefaultval from sys_useracl_desc where userindex={0} and moduleid={1}", strIndex, Convert.ToString(m_nModlId));
            //dtab = Helper.GetDataTable(strSQL, Conn);
            dtab = Helper.GetDataTableWithSqlProxy("sys_useracl_desc", strSQL, Conn);
            string strCanModifyDefaultVal = "";
            if (dtab.Rows.Count != 0)
            {
                strCanModifyDefaultVal = dtab.Rows[0][0].ToString();
            }
            if (strCanModifyDefaultVal == "False" || strCanModifyDefaultVal == "" || strCanModifyDefaultVal == "0")
                return bResult;
            bResult = true;
            return bResult;
        }
    }
}
