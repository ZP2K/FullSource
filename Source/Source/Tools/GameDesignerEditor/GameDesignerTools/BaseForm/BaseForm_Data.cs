using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using LuaInterface;
using System.Collections;
using System.Text.RegularExpressions;
using DevComponents.DotNetBar;
using atlaxLib;


namespace GameDesingerTools
{
    public partial class BaseForm
    {
        int m_oldX = 0;
        int m_oldY = 0;
        enum MdlType { MT_PLAYER, MT_NPC, MT_DODEAD, MT_OTHER, MT_COUNT };
        enum UpdateType { ADD, MOD, DEL };

        static string[] strMdlDefaultPath = new string[] {
            "data\\source\\player\\",
            "data\\source\\npc\\",
            "data\\source\\doodad\\",
            ""};
        static string[] strMdlDefaultAnimationPathSuffix = new string[] {
            "����", "", "", ""};
        static string[] strMdlDefaultFacePathSuffix = new string[] {
            "����", "", "", ""};

        // OpenModelInDialogStatic���
        static Hashtable staticDialogs = new Hashtable();

        public TreeView BaseTree { get { return baseTree; } set { baseTree = value; } }
        public DevComponents.DotNetBar.TabControl tabControl;

        private bool m_DisplayNode = false; // �Ƿ��ýű�����Ҷ������ʾ 
        private bool m_DisplayVarNode = false; // �Ƿ�ÿ��������õĿ�����ʾ���ַ������б仯
        private DataRow m_defaultRow; // ��ǰĬ����
        private string m_displayString; // �ű�������ʾ���ַ���
        private string m_displayField; // �ű�������ʾ���ֶ���
        private Hashtable m_nodeTable = new Hashtable(); // ������ԭ���Ƶ�hash��
        private Hashtable m_nodeDisplayValueTable = new Hashtable(); // ��������ʾ���Ƶ�hash��

        ArrayList m_Pages;

        //�ű���
        public ArrayList Pages { get { return m_Pages; } }

        //C#��
        public ArrayList PagesCS { get { return m_Pages; } }

        SqlConnection m_conn = MainForm.conn;
        public SqlConnection Conn { get { return m_conn; } }

        LuaEx m_lua = new LuaEx();
        public Lua lua { get { return m_lua; } }

        bool m_canSaveDB = false;
        public bool CanSaveDB { get { return m_canSaveDB; } set { m_canSaveDB = value; } }

        bool m_canCfg = false;
        public bool CanCfg { get { return m_canCfg; } set { m_canCfg = value; } }

        int m_modl_id;
        public int ModID { get { return m_modl_id; } set { m_modl_id = value; } }

        bool m_bLoadTreeWithLua;
        public bool LoadTreeWithLua { get { return m_bLoadTreeWithLua; } set { m_bLoadTreeWithLua = value; } }

        string m_strModelName;
        public string ModelName { get { return m_strModelName; } set { m_strModelName = value; } }

        string m_strMainTableName;
        public string MainTableName { get { return m_strMainTableName; } set { m_strMainTableName = value; } }

        string[] m_strCatFields;
        public string[] CatFields { get { return m_strCatFields; } set { m_strCatFields = value; } }

        Hashtable[] m_htCatHashes;
        public Hashtable[] CatHashes { get { return m_htCatHashes; } set { m_htCatHashes = value; } }

        // ����
        DataTable m_tblMain;
        public DataTable TblMain { get { return m_tblMain; } set { m_tblMain = value; } }
        bool m_btblMainHasDefaultRow;
        public bool HasDefaultRow { get { return m_btblMainHasDefaultRow; } set { m_btblMainHasDefaultRow = value; } }

        object[] m_iStartIDs;
        public object[] StartIDs { get { return m_iStartIDs; } set { m_iStartIDs = value; } }

        // ���������������ĸ��ֶ���ʾ
        string m_strDisplayField;
        public string DisplayField { get { return m_strDisplayField; } set { m_strDisplayField = value; } }

        bool m_bCatFieldsContainsDisplayField;
        public bool CatfieldsContainsDisplayfield { get { return m_bCatFieldsContainsDisplayField; } set { m_bCatFieldsContainsDisplayField = value; } }

        public bool KeysContainsDisplayfileld = false;

        public bool CatFieldsContainsKey = false;
        public int ToChangeIndex = -1;

        // ����
        public FindForm1 m_FindFrm = null;

        // ���ڸ���,ճ��
        TreeNode m_tnCopyFromNode = null;
        public TreeNode CopyFromNode { get { return m_tnCopyFromNode; } set { m_tnCopyFromNode = value; } }


        ArrayList m_lsTblUpdateDefault = new ArrayList();
        public ArrayList TableUpdateDefault { get { return m_lsTblUpdateDefault; } set { m_lsTblUpdateDefault = value; } }

        const int WM_REFRESHPG = 0x400 + 2000; // WM_USER = ?
        const int WM_REFREPROP = WM_REFRESHPG + 1;
        //const int WM_REFREPRTREE = WM_REFRESHPG + 2;

        // ����Ԥ��
        private int init_preview_index = -1; // 1-based, ��tab˳��һ����
        private MyPanelEx[] preview_panels;
        private static Test3DEngine m_3DEngie;
        private static bool m_bInit3DEngineFaield = false;
        public static Test3DEngine K3DEngine { get { return m_3DEngie; } }
        private static bool m_bWarning3dEngine = false;

        private object m_ChangedProperty_tree;
        private List<CustomProperty> m_lstChangedProperty = new List<CustomProperty>();
        private bool m_bRecordChanged;
        private TreeNode m_poupupNode;
        private TimeLog log = new TimeLog();
        private TimeLog logscript = new TimeLog();
        private TimeLog loglocalscript = new TimeLog();
        private RecordLockEx m_recLock;
        private bool m_bPluginTree;

        private ButtonItem[] m_ExtraMenuItem = null;
        public BaseDialog m_Dialog = null;



        class ArrayListEx : ArrayList
        {
            private BaseForm m_baseForm;
            public ArrayListEx(BaseForm baseform)
            {
                m_baseForm = baseform;
            }
            public override object this[int idx]
            {
                get
                {
                    //���У���ᱻ foreach�����õ����ֻ���뵱ǰҳ�Ĳ��Ի�ʧЧ
                    /*PageDesc pgdes = base[idx] as PageDesc;
                    if (pgdes.pg != null && pgdes.pg.SelectedObject == null && pgdes.pg.Parent.Tag != null)
                    {
                        m_baseForm.LoadPageDBClass(pgdes.pg);
                    }*/
                    return base[idx];
                }
            }
        };
    };

    public class TblUpdateDefault
    {
        public string tblName;
        public DataRow oldValue;
        public DataRow newValue;
    };
    public class PageDesc
    {
        public string name_cn;
        public int id;
        public PropertyGrid pg;
        public DBCustomClass dbClass;
        public DBCustomClass Properties;
        public bool bCustomTab;
        //add by cuiwei 07.9.20
        public queryCommand qc;
        //add end
    };
}
