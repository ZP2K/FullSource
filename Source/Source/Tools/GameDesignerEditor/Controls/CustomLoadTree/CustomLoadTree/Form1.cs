using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DevComponents.DotNetBar;
using System.Data.SqlClient;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;


namespace CustomLoadTree
{
    public partial class Form1 : Form
    {
        [DllImport("user32.dll")]
        static extern void LockWindowUpdate(IntPtr handle);

        private Hashtable htQuestIdTreeNode;

        private Color ColorNodeCat = Color.SeaGreen;
        private Color ColorNodeNormal = Color.Black;
        private Color ColorNodeClone = Color.Olive;
        private Color ColorNodeAll = Color.Red;
        private Color ColorNodeOne = Color.Red;
        private Color ColorNodeSurf = Color.Red;
        private Color ColorNodeDragOld = Color.BlueViolet;
        private Color ColorNodeAreaQuestOnly = Color.DarkOrange; // �������û��ǰ����ֻΪ��ʾͬ������
        //private Color markedColor = Color.DeepSkyBlue; // ����������ɫ
        private string markedString = "�� "; // ��ǵķ���

        private const int ImageCat = 9;
        private const int ImageAll = 8;
        private const int ImageOne = 8;
        private const int ImageSurf = 10;

        private const int ImageSelectedCat = 9;
        private const int ImageSelectedAll = 8;
        private const int ImageSelectedOne = 8;
        private const int ImageSelectedSurf = 10;

        public TreeView theTree { get { return treeView1; } set { treeView1 = value; } }

        public SplitContainer Spliter { get { return splitContainer1; } set { splitContainer1 = value; } }

        public TreeNode PopupNode;

        public string TblName = string.Empty;
        public string CatField = string.Empty;

        public SqlConnection Conn = null;

        public DataTable Tbl = null;
        public DataRow defaultRow = null;

        public object[] StartIDs = null;

        public TreeNode CopySrcNode = null;

        private const string SubsequenceMark = "��"; // ���������ޡ�

        private ArrayList AllFoundNodes = new ArrayList();

        private ArrayList FindingIDList = null; // ���ڼ���������ѭ��

        public bool DisplayLevel = false;
        public bool DisplayID = true;

        private int LastVer = -1;
        private int LastOrderVer = -1;
        private TableLock TblLock = null;
        private TableLock OrderLock = null;

        private TreeNode OldDragOver = null;

        private Class1 MyParentClass = null;
        public FindFrm FindingForm = null;

        private NodeData nodeData = new NodeData(); // �������
        private TreeData treeData = new TreeData(); // ������
        private Progress proForm = new Progress(); // ����������
        private int nodeCount = 0; // ��ǰ�����Ľ����
        private string orderTable = "sys_quests_order"; // ��¼���˳��ı�
        private bool showError = true; // �Ƿ��ڳ�ʼ����ʱ��ʾ������Ϣ
        private ComputeForm cForm; // ������������
        private MultiTaskEditManager multiManager; // �����������ֶ��Զ���༭����
        private List<string> fieldList; // ������༭��ʾ�ֶ�����
        private Hashtable nameTable; // ����id���������Ƶ�������ϣ��
        private Hashtable levelTable; // ����id������ȼ���������ϣ��       
        private bool m_isReadOnly = false;
        public ButtonItem[] m_ExtraMenuItem = null;

        public Form1(string tblname, string catfield, Class1 cls, SqlConnection conn, bool isReadOnly) 
        {
            InitializeComponent();

            TblName = tblname;
            CatField = catfield;
            MyParentClass = cls;
            Conn = conn;
            buttonItemID.Checked = DisplayID;
            buttonItemLevel.Checked = DisplayLevel;
            TblLock = new TableLock(Conn, TblName);
            TblLock.UnLock();
            OrderLock = new TableLock(Conn, orderTable);
            OrderLock.UnLock();
            LastVer = TblLock.GetServerLastVersion();
            LastOrderVer = OrderLock.GetServerLastVersion();
            this.m_isReadOnly = isReadOnly;
        }

        private string SelectSql
        {
            get
            {
                string selectStr = string.Empty;
                string strRet = string.Empty;

                foreach (string colName in Logic.tableColumns)
                {
                    selectStr += string.Format("{0}.{1},", TblName, colName);
                }
                if (selectStr != string.Empty)
                {
                    selectStr = selectStr.Substring(0, selectStr.Length - 1);

                    strRet = string.Format("select {0}, {2}.Marked from {1} left join {2} on {1}.QuestId = {2}.QuestId order by {2}.QuestIndex",
                        selectStr, TblName, "sys_quests_order");
                }
                
                return strRet;
            }
               
        }
       
        public void AddExtraMenuItems(EventHandler extramenuItemClick)
        {
            m_ExtraMenuItem = new ButtonItem[20];
            for (int i = 0; i < 20; i++)
            {
                m_ExtraMenuItem[i] = new ButtonItem();
                m_ExtraMenuItem[i].Name = "extra_menu_item" + Convert.ToString(i + 1);
                m_ExtraMenuItem[i].Text = m_ExtraMenuItem[i].Name;
                m_ExtraMenuItem[i].Click += extramenuItemClick;
                m_ExtraMenuItem[i].Visible = false;
            }
            this.cmNodeRightClick.SubItems.AddRange(m_ExtraMenuItem);
        }

        /// <summary>
        /// ������༭��ʾ�ֶ�����
        /// </summary>
        public List<string> FieldList
        {
            set
            {
                fieldList = value;
            }
        }

        // ��������
        public TreeView ReloadTree()
        {
            htQuestIdTreeNode = new Hashtable();
            htQuestIdTreeNode.Clear();
            theTree.Nodes.Clear();
            TblLock = new TableLock(Conn, TblName);
            OrderLock = new TableLock(Conn, orderTable);

            if (!Logic.ReInit(SelectSql, Conn))
            {
                return null;                
            }
            Tbl = Logic.QuestTable;

            // Ĭ��ֵ���
            TreeNode newNode = theTree.Nodes.Add("<Ĭ��ֵ>");
            object[] keys = new object[1];
            keys[0] = "-1";
            newNode.Tag = keys;

            // ����tree
            // ������
            string sql = string.Format("SELECT {0} FROM {1} GROUP BY {2}", CatField, TblName, CatField);
            DataTable tblCats = Helper.GetDataTable(sql, Conn);
            foreach (DataRow row in tblCats.Rows)
            {
                string catname = row[0].ToString().Trim();
                TreeNode catnode = theTree.Nodes.Add(catname);
                object[] catkeys = new object[1];
                catkeys[0] = "-2";
                catnode.Tag = catkeys;
                catnode.ForeColor = ColorNodeCat;
                catnode.ImageIndex = ImageCat;
                catnode.SelectedImageIndex = ImageSelectedCat;
            }

            treeData.CheckIDCompare(Conn); // �Զ����ƥ��tbl_quests��sys_quests_order���questID�ֶ�            
          
            // Ĭ����
            defaultRow = Tbl.Rows[0];

            // StartID ��ʼ��
            StartIDs = new object[Tbl.PrimaryKey.Length];
            for (int i = 0; i < StartIDs.Length; i++)
            {
                string field = Tbl.PrimaryKey[i].ColumnName;
                string sQl = string.Format("SELECT * FROM sys_id_pool WHERE tablename='{0}' AND fieldname='{1}'", TblName, field);
                DataTable t = Helper.GetDataTable(sQl, Conn);
                if (t.Rows.Count > 0)
                    StartIDs[i] = Convert.ToInt32(t.Rows[0]["lastid"]);
                else
                    StartIDs[i] = 1;
            }

            // ��¼���
            foreach (DataRow row in Tbl.Rows)
            {
                string quest_name = row["QuestName"].ToString().Trim();
                int quest_id = Convert.ToInt32(row["QuestID"]);

                // ����Ĭ��ֵ
                if (quest_id == 0)
                    continue;

                FindingIDList = new ArrayList();
                FindingIDList.Add(quest_id);
                try
                { 
                    newNode = AddQuestNode(quest_id);
                }
                catch (Exception ex)
                {
                    if (showError) // �Ƿ���ʾ������Ϣ
                    {
                        string err = string.Format("δ����� {0} ������" +
                            "\r\n\r\n������ϢΪ��{1}��\n\n�Ƿ�����γ�ʼ�������ʱ���Դ�����Ϣ?", quest_id, ex.Message);
                        DialogResult result = MessageBox.Show(err, "��ӽ��ʱ��������", MessageBoxButtons.YesNo);
                        if (result == DialogResult.Yes)
                        {
                            showError = false;
                        }
                    }
                }       
                FindingIDList.Clear();
            }

            treeData.InitTreeOrder(Conn, theTree); // ��ʼ����������

            htQuestIdTreeNode.Clear();
            htQuestIdTreeNode = null;            
            return theTree;
        }

        // ����Ƿ�����ѭ��
        private void DetectRecurrentPrequest(int quest_id)
        {
            if (FindingIDList.IndexOf(quest_id) != -1)
            {
                string strids = "[";
                for (int i = 0; i < FindingIDList.Count; i++)
                {
                    strids += FindingIDList[i].ToString();
                    strids += ",";
                }
                strids = strids.Remove(strids.Length - 1);
                strids += "]";
                if (showError) // �Ƿ���ʾ������Ϣ
                {
                    string err = string.Format("����ѭ��������������ͨ�����ɴ����ǰ������ID����ġ�" +
                        "\r\n\r\n��ص�����IDΪ {0}��\n\n�Ƿ�����γ�ʼ�������ʱ���Դ�����Ϣ?", strids);
                    DialogResult result = MessageBox.Show(err, "��ӽ��ʱ��������", MessageBoxButtons.YesNo);
                    if (result == DialogResult.Yes)
                    {
                        showError = false;
                    }
                }
                throw new Exception("ѭ����������");
            }
        }

      
        private void PushNode(TreeNode node, int questid)
        {
            htQuestIdTreeNode[questid] = node;
        }
        // ���������
        private TreeNode AddQuestNode(int QuestID)
        {
            TreeNode newNode = null;
            List<string> list = new List<string>(); // ��¼�����Ϣ������

            if (QuestID == 0)   // �ܶ������prequestid, ����Ĭ��ֵ������, ������0.
                return newNode; // ����Ҫ����.

            DataRow row = Tbl.Rows.Find(QuestID);
            if (row == null)
                return newNode;

            string level = row["QuestLevel"].ToString().Trim();
            string strLevel = string.Format("[Lv{0}]", level);
            string quest_name = row["QuestName"].ToString().Trim();
            int quest_id = Convert.ToInt32(row["QuestID"]);
            string strID = string.Format("[{0}]", quest_id);
            string quest_cat = row[CatField].ToString().Trim();
            bool prequest_logic = true;
            if (defaultRow["PrequestLogic"] != DBNull.Value)
                prequest_logic = Convert.ToBoolean(defaultRow["PrequestLogic"]);
            if (row["PrequestLogic"] != DBNull.Value)
                prequest_logic = Convert.ToBoolean(row["PrequestLogic"]);
            int prequest_count = 0;
            int[] prequest_arr = new int[4];

            // ��ӽ����Ϣ������
            list.Add(QuestID.ToString()); // ����ID
            list.Add(quest_name); // ��������
            // ����ȼ�
            if (level == "") // �ȼ�Ϊ��
            {
                list.Add("0");
            }
            else
            {
                list.Add(level);
            }

            list.Add(Logic.GetQuestType(QuestID).ToString()); // ��������

            for (int i = 0; i < 4; i++)
            {
                string colnameprequestid = "PrequestID" + Convert.ToString(i + 1);
                if (row[colnameprequestid] != DBNull.Value && Convert.ToInt32(row[colnameprequestid]) != 0)
                {
                    prequest_arr[prequest_count++] = Convert.ToInt32(row[colnameprequestid]);
                }
            }

            //TreeNode QuestNode = Helper.FindQuestNode(QuestID, theTree);
            TreeNode QuestNode = htQuestIdTreeNode[QuestID] as TreeNode;
            if (QuestNode != null)
            {
                return QuestNode;
            }

            if (prequest_count == 0)
            {
                // ��ǰ�������񣬷����������
                // 1�������
                // 2����Ϊͬ�ڽ��λ��ĳ����֮��
                int areaQuest_father = -100;
                if (row["_AreaQuest"] != DBNull.Value)
                    areaQuest_father = Convert.ToInt32(row["_AreaQuest"]);

                if (areaQuest_father > 0) // ��Ϊͬ�ڽ��
                {
                    //TreeNode  = Helper.FindQuestNode(prequest_arr[0], theTree);                    
                    TreeNode father_node = htQuestIdTreeNode[prequest_arr[0]] as TreeNode;
                    if (father_node == null)
                    {
                        father_node = AddQuestNode(areaQuest_father);
                    }
                    newNode = father_node.Nodes.Add(quest_name);
                    object[] keys = new object[1];
                    keys[0] = quest_id;
                    newNode.Tag = keys;
                    newNode.ForeColor = ColorNodeAreaQuestOnly;
                    //CheckAndMarkNode(newNode, quest_id.ToString());
                    if (!row.IsNull("Marked") && row["Marked"].ToString() == "1") // ��ǲ�Ϊ��
                    {
                        newNode.Text = markedString + newNode.Text;                       
                    }
                    PushNode(newNode, quest_id);
                }
                else // ��������
                {
                    TreeNode catnode = Helper.FindCatNode(quest_cat, theTree);
                    if (catnode != null)
                    {
                        newNode = catnode.Nodes.Add(quest_name);
                        object[] keys = new object[1];
                        keys[0] = quest_id;
                        newNode.Tag = keys;
                        newNode.ForeColor = ColorNodeNormal;
                        // CheckAndMarkNode(newNode, quest_id.ToString());
                        if (!row.IsNull("Marked") && row["Marked"].ToString() == "1") // ��ǲ�Ϊ��
                        {
                            newNode.Text = markedString + newNode.Text;
                        }
                        PushNode(newNode, quest_id);

                    }
                }
            }
            else if (prequest_count == 1)
            {
               // TreeNode father_node = Helper.FindQuestNode(prequest_arr[0], theTree);
                TreeNode father_node = htQuestIdTreeNode[prequest_arr[0]] as TreeNode;

                if (father_node == null)
                {
                    DetectRecurrentPrequest(prequest_arr[0]);
                    FindingIDList.Add(prequest_arr[0]);

                    father_node = AddQuestNode(prequest_arr[0]);

                    if (father_node == null)
                    {
                        if (showError) // �Ƿ���ʾ������Ϣ
                        {
                            string err = string.Format("����� {0} ������ʱ��������\r\n\r\n����ǰ����" +
                                "{1} �����񲻴��ڻ����޷���ӡ�\n\n�Ƿ�����γ�ʼ�������ʱ���Դ�����Ϣ?", quest_id, prequest_arr[0]);
                            DialogResult result = MessageBox.Show(err, "��ӽ��ʱ��������", MessageBoxButtons.YesNo);
                            if (result == DialogResult.Yes)
                            {
                                showError = false;
                            }
                        }
                        return null;
                    }
                }

                newNode = father_node.Nodes.Add(quest_name);
                object[] keys = new object[1];
                keys[0] = quest_id;
                newNode.Tag = keys;
                newNode.ForeColor = ColorNodeNormal;
                // CheckAndMarkNode(newNode, quest_id.ToString());
                if (!row.IsNull("Marked") && row["Marked"].ToString() == "1") // ��ǲ�Ϊ��
                {
                    newNode.Text = markedString + newNode.Text;
                }
                PushNode(newNode, quest_id);
            }
            else
            {
                // ���ǰ��������
                TreeNode[] pre_nodes = new TreeNode[prequest_count];
                for (int i = 0; i < prequest_count; i++)
                {
                    //TreeNode prequest_node = Helper.FindQuestNode(prequest_arr[i], theTree);
                    TreeNode prequest_node = htQuestIdTreeNode[prequest_arr[i]] as TreeNode;
                    if (prequest_node == null)
                    {
                        DetectRecurrentPrequest(prequest_arr[i]);
                        FindingIDList.Add(prequest_arr[i]);
                        prequest_node = AddQuestNode(prequest_arr[i]);
                    }
                    if (prequest_node == null)
                    {
                        if(showError) // �Ƿ���ʾ������Ϣ
                        {
                            string err = string.Format("����� {0} ������ʱ��������\r\n\r\n��������һ��ǰ����" +
                                "{1} �����񲻴��ڻ����޷���ӡ�\n\n�Ƿ�����γ�ʼ�������ʱ���Դ�����Ϣ?", quest_id, prequest_arr[i]);
                            DialogResult result = MessageBox.Show(err, "��ӽ��ʱ��������", MessageBoxButtons.YesNo);
                            if (result == DialogResult.Yes)
                            {
                                showError = false;
                            }
                        }
                        return null;
                    }

                    TreeNode toadd = (TreeNode)prequest_node.Clone();
                    toadd.Nodes.Clear();
                    pre_nodes[i] = toadd;
                    pre_nodes[i].ForeColor = ColorNodeClone;
                    if (pre_nodes[i].Text.Contains(SubsequenceMark))
                    {
                        pre_nodes[i].Text = pre_nodes[i].Text.Remove(pre_nodes[i].Text.Length - SubsequenceMark.Length);
                    }
                }

                // �ڵ�һ��ǰ������ĸ�����, ��� �����ͽ��.
                // �����ҿ����Ƿ��Ѿ��������ļ����ͽ��
               // TreeNode first_prequest = Helper.FindQuestNode(prequest_arr[0], theTree);
                TreeNode first_prequest = htQuestIdTreeNode[prequest_arr[0]] as TreeNode;
                TreeNode set_node = Helper.FindSetNode(first_prequest.Parent, prequest_logic, prequest_count, prequest_arr);
                TreeNode nodeSurfix = null;
                if (set_node == null) // �ж�֮ǰ�Ƿ�ӹ�ͬ���ļ����ͽ��
                {
                    string set_string = prequest_logic ? "ȫ�����" : "�����һ";

                    if (first_prequest.Parent != null)
                        set_node = first_prequest.Parent.Nodes.Add(set_string);
                    else
                        set_node = theTree.Nodes.Add(set_string);

                    object[] keys_set = new object[1];
                    keys_set[0] = prequest_logic ? -3 : -4;
                    set_node.Tag = keys_set;
                    set_node.ForeColor = prequest_logic ? ColorNodeAll : ColorNodeOne;
                    set_node.ImageIndex = prequest_logic ? ImageAll : ImageOne;
                    set_node.SelectedImageIndex = prequest_logic ? ImageSelectedAll : ImageSelectedOne;

                    // ��Ӽ����µ�ǰ��������
                    foreach (TreeNode n in pre_nodes)
                    {
                        set_node.Nodes.Add(n);
                    }

                    // ������㣺<��������>
                    nodeSurfix = set_node.Nodes.Add("��������");
                    object[] keys_surf = new object[1];
                    keys_surf[0] = -5;
                    nodeSurfix.Tag = keys_surf;
                    nodeSurfix.ForeColor = ColorNodeSurf;
                    nodeSurfix.ImageIndex = ImageSurf;
                    nodeSurfix.SelectedImageIndex = ImageSelectedSurf;
                }
                else
                {
                    foreach (TreeNode node in set_node.Nodes)
                        if (node.Text == "��������")
                        {
                            nodeSurfix = node;
                            break;
                        }
                }

                newNode = nodeSurfix.Nodes.Add(quest_name);
                object[] keys = new object[1];
                keys[0] = quest_id;
                newNode.Tag = keys;
                newNode.ForeColor = ColorNodeNormal;
                // CheckAndMarkNode(newNode, quest_id.ToString());
                if (!row.IsNull("Marked") && row["Marked"].ToString() == "1") // ��ǲ�Ϊ��
                {
                    newNode.Text = markedString + newNode.Text;
                }
                PushNode(newNode, quest_id);
            }

            // �Բ�ͬ����������ò�ͬ��ͼ��
            newNode.ImageIndex = Logic.GetQuestImageIndex(quest_id);
            newNode.SelectedImageIndex = newNode.ImageIndex;

            // �ж��Ƿ���Ҫ��ʾΪֱ�Ӻ���
            bool need_subsequence = false;
            if (prequest_count == 1)
            {
                int prequest = prequest_arr[0];
                DataRow row_prequest = Tbl.Rows.Find(prequest);
                if (row_prequest != null)
                {
                    if (row_prequest["SubsequenceID"] != DBNull.Value)
                    {
                        if (Convert.ToInt32(row_prequest["SubsequenceID"]) == QuestID)
                            need_subsequence = true;
                    }
                }
            }
            if (need_subsequence)
            {
                newNode.Text += SubsequenceMark;
            }

            if (DisplayLevel)
            {
                if(newNode.Text.StartsWith(markedString)) // ������ı��Ѿ�����ǹ�
                {
                    newNode.Text = markedString + strLevel + newNode.Text.Substring(2);
                }
                else
                {
                    newNode.Text = strLevel + newNode.Text;
                }                
            }
            if (DisplayID)
            {
                if (newNode.Text.StartsWith(markedString)) // ������ı��Ѿ�����ǹ�
                {
                    newNode.Text = markedString + strID + newNode.Text.Substring(2);
                }
                else
                {
                    newNode.Text = strID + newNode.Text;
                }                
            }

            nodeData.AddData(newNode, list); // ��¼�����Ϣ
            return newNode;
        }

        private void treeView1_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Button == MouseButtons.Left && e.Node != null)
            {
                AllFoundNodes = new ArrayList();
            }
            else if (e.Button == MouseButtons.Right && e.Node != null)
            {
                if (this.m_isReadOnly) return;
                int node_key = Convert.ToInt32(((object[])e.Node.Tag)[0]);

                // �Ƿ������ӷ���
                bool CanAddCat = false;
                if (node_key == -2)
                    CanAddCat = true;
                // �Ƿ�����������
                bool CanAddQuest = true;
                if (node_key == -1 || node_key == -3 || node_key == -4)
                    CanAddQuest = false;

                // �Ƿ���Ը���
                bool CanCopy = true;
                if (node_key == -2 || node_key == -3 || node_key == -4 || node_key == -5)
                    CanCopy = false;

                // �Ƿ����ճ��
                bool CanPaste = false;
                if (CopySrcNode != null && node_key > 0)
                    CanPaste = true;

                // �Ƿ����ɾ��
                bool CanDelete = true;

                // �Ƿ������Ϊֱ�Ӻ���
                bool CanSetSubsequence = false;
                buttonItem10.Text = "��Ϊֱ�Ӻ���";
                int father_key = -100;
                if (e.Node.Parent != null)
                    father_key = Convert.ToInt32(((object[])e.Node.Parent.Tag)[0]);
                if (father_key > 0 && node_key > 0)
                    CanSetSubsequence = true;
                if (e.Node.Text.Contains(SubsequenceMark))
                    buttonItem10.Text = "ȡ��ֱ�Ӻ���";

                // �Ƿ��������ͬ������
                bool CanSetAreaQuest = false;
                buttonItem13.Text = "��Ϊͬ������";
                if (father_key > 0 && node_key > 0)
                    CanSetAreaQuest = true;
                if (e.Node.ForeColor == ColorNodeAreaQuestOnly)
                    buttonItem13.Text = "ȡ��ͬ������";

                // ���¸��˵�
                buttonItem1.Enabled = CanAddCat;
                buttonItem2.Enabled = CanAddQuest;
                buttonItem6.Enabled = CanCopy;
                buttonItem7.Enabled = CanPaste;
                buttonItem8.Enabled = CanDelete;
                buttonItem10.Enabled = CanSetSubsequence;
                buttonItem13.Enabled = CanSetAreaQuest;

                // �����Ҽ��˵�
                PopupNode = e.Node;
                cmNodeRightClick.Popup(Control.MousePosition);
            }
        }

        // ��ӷ���
        private void buttonItem1_Click(object sender, EventArgs e)
        {
            if (this.m_isReadOnly) return;
            TreeNode node = treeView1.Nodes.Add("�½�����");
            object[] catkeys = new object[1];
            catkeys[0] = "-2";
            node.Tag = catkeys;
            node.ForeColor = ColorNodeCat;
            node.ImageIndex = ImageCat;
            node.SelectedImageIndex = ImageSelectedCat;

            // ����������״̬
            if (!node.IsEditing)
            {
                treeView1.SelectedNode = node;
                treeView1.LabelEdit = true;
                node.BeginEdit();
            }

            treeData.AddSection(node); // ��ӷ�����˳����Ϣ
        }

        // �������
        private void buttonItem2_Click(object sender, EventArgs e)
        {
            if (this.m_isReadOnly) return;
            TreeNode father_node = PopupNode;
            LockWindowUpdate(theTree.Handle);
            AddNode(father_node);
            LockWindowUpdate(IntPtr.Zero);   
        }

        // ��������
        private void AddNode(TreeNode father_node)
        {
            int father_tag = Convert.ToInt32(((object[])father_node.Tag)[0]);
            string father_text = father_node.Text;
            TreeNode catnode = Helper.FindAncestorNode(father_node);

            // ��Ӽ�¼��
            object[] keys = (object[])(Helper.GetUnusedID(TblName, ref StartIDs, Conn).Clone());
            if (Convert.ToInt32(keys[0]) > 8190)
            {
                MessageBox.Show("����ID�ѳ�������8192������ʧ�ܣ�");
                return;
            }
            DataTable tbl = Helper.GetDataTable("SELECT * FROM " + TblName, Conn);
            Helper.SqlInsertRecord(ref tbl, TblName, keys, Conn);

            string[] Fields;
            object[] Values;
            string nodeName = "�½�����"; // ����ı�����
            List<string> list = new List<string>(); // ���������Ϣ

            // ���±�Ҫ���ֶ�ֵ
            if (father_tag > 0)         // ����ͨ��������Ӻ�������
            {
                Form2 form2 = new Form2(Conn, father_tag, catnode.Text);
                form2.StartPosition = FormStartPosition.CenterScreen; // ����������Ϊ��Ļ����λ��
                DialogResult result = form2.ShowDialog();

                if (result == DialogResult.OK) // �û�ȷ���½�����
                {            
                    Fields = new string[9];
                    Fields[0] = "PrequestID1";
                    Fields[1] = "QuestName";
                    Fields[2] = "Class";
                    Fields[3] = CatField;
                    Fields[4] = "QuestLevel";
                    Fields[5] = "Objective";
                    Fields[6] = "StartMapID";
                    Fields[7] = "EndMapID";
                    Fields[8] = "MinLevel";
                    Values = new object[9];
                    Values[0] = father_tag;
                    Values[1] = form2.MissionName;
                    Values[2] = form2.MissionClass;
                    Values[3] = form2.MissionCat;
                    Values[4] = form2.MissionLevel;
                    Values[5] = form2.MissionObject;
                    Values[6] = form2.StartMap;
                    Values[7] = form2.EndMap;
                    Values[8] = form2.MissionMinLevel;

                    if(form2.TongqiMission) // �Ƿ���ͬ������
                    {
                        Fields[0] = "_AreaQuest";
                    }

                    nodeName = form2.MissionName;
                }
                else
                {
                    return; // ����
                }
                Helper.SqlUpdateRecord(TblName, keys, Fields, Values, Conn);
                list.Add(keys[0].ToString());
                list.Add(form2.MissionName);
                list.Add(form2.MissionLevel.ToString());
                list.Add(Logic.GetQuestType(int.Parse(keys[0].ToString())).ToString());
            }
            else if (father_tag == -2)  // �ڷ���������������
            {
                Form2 form2 = new Form2(Conn, father_text);
                form2.StartPosition = FormStartPosition.CenterScreen; // ����������Ϊ��Ļ����λ��
                DialogResult result = form2.ShowDialog();

                if (result == DialogResult.OK) // �û�ȷ���½�����
                {
                    Fields = new string[8];
                    Fields[0] = "QuestName";
                    Fields[1] = "Class";
                    Fields[2] = CatField;
                    Fields[3] = "QuestLevel";
                    Fields[4] = "Objective";
                    Fields[5] = "StartMapID";
                    Fields[6] = "EndMapID";
                    Fields[7] = "MinLevel";
                    Values = new object[8];
                    Values[0] = form2.MissionName;
                    Values[1] = form2.MissionClass;
                    Values[2] = form2.MissionCat;
                    Values[3] = form2.MissionLevel;
                    Values[4] = form2.MissionObject;
                    Values[5] = form2.StartMap;
                    Values[6] = form2.EndMap;
                    Values[7] = form2.MissionMinLevel;

                    nodeName = form2.MissionName;
                }
                else
                {
                    return; // ����
                }
                Helper.SqlUpdateRecord(TblName, keys, Fields, Values, Conn);
                list.Add(keys[0].ToString());
                list.Add(form2.MissionName);
                list.Add(form2.MissionLevel.ToString());
                list.Add(Logic.GetQuestType(int.Parse(keys[0].ToString())).ToString());
            }
            else if (father_tag == -5)  // �ں�������Ľ������Ӻ�������
            {
                Fields = new string[7];
                Values = new object[7];
                int count = 0;
                foreach (TreeNode node in father_node.Parent.Nodes)
                {
                    int node_tag = Convert.ToInt32(((object[])node.Tag)[0]);
                    if (node_tag > 0) // ������Ҫ��Ϊ��������ǰ��
                    {
                        Fields[count] = "PrequestID" + Convert.ToInt32(count + 1);
                        Values[count] = node_tag;
                        count++;
                    }
                }
                for (int i = 0; i < 4 - count; i++)
                {
                    Fields[count + i] = "PrequestID" + Convert.ToInt32(count + i + 1);
                    Values[count + i] = DBNull.Value;
                }
                bool prequestLogic = Convert.ToInt32(((object[])father_node.Parent.Tag)[0]) == -3;

                Form2 form2 = new Form2(Conn, catnode.Text);
                form2.StartPosition = FormStartPosition.CenterScreen; // ����������Ϊ��Ļ����λ��
                DialogResult result = form2.ShowDialog();

                if (result == DialogResult.OK) // �û�ȷ���½�����
                {
                    string[] newFields;
                    object[] newValues;

                    if(form2.TongqiMission) // �Ƿ���ͬ������
                    {
                        newFields = new string[9];
                        newValues = new object[9];
                        newFields[0] = "PrequestLogic";
                        newFields[1] = CatField;
                        newFields[2] = "QuestName";
                        newFields[3] = "Class";
                        newFields[4] = "QuestLevel";
                        newFields[5] = "Objective";
                        newFields[6] = "StartMapID";
                        newFields[7] = "EndMapID";
                        newFields[8] = "MinLevel";
                        newValues[0] = prequestLogic;
                        newValues[1] = form2.MissionCat;
                        newValues[2] = form2.MissionName;
                        newValues[3] = form2.MissionClass;
                        newValues[4] = form2.MissionLevel;
                        newValues[5] = form2.MissionObject;
                        newValues[6] = form2.StartMap;
                        newValues[7] = form2.EndMap;
                        newValues[8] = form2.MissionMinLevel;
                    }
                    else
                    {
                        newFields = new string[13];
                        newValues = new object[13];
                        for (int i = 0; i < 4; i++)
                        {
                            newFields[i] = Fields[i];
                            newValues[i] = Values[i];
                        }
                        newFields[4] = "PrequestLogic";
                        newFields[5] = CatField;
                        newFields[6] = "QuestName";
                        newFields[7] = "Class";
                        newFields[8] = "QuestLevel";
                        newFields[9] = "Objective";
                        newFields[10] = "StartMapID";
                        newFields[11] = "EndMapID";
                        newFields[12] = "MinLevel";
                        newValues[4] = prequestLogic;
                        newValues[5] = form2.MissionCat;
                        newValues[6] = form2.MissionName;
                        newValues[7] = form2.MissionClass;
                        newValues[8] = form2.MissionLevel;
                        newValues[9] = form2.MissionObject;
                        newValues[10] = form2.StartMap;
                        newValues[11] = form2.EndMap;
                        newValues[12] = form2.MissionMinLevel;
                    }

                    nodeName = form2.MissionName;
                    Helper.SqlUpdateRecord(TblName, keys, newFields, newValues, Conn);
                    list.Add(keys[0].ToString());
                    list.Add(form2.MissionName);
                    list.Add(form2.MissionLevel.ToString());
                    list.Add(Logic.GetQuestType(int.Parse(keys[0].ToString())).ToString());
                }
                else
                {
                    return; // ����
                }
            }
            else
            {
                throw new Exception("�����������");
            }

            // ����logic
            Logic.tbl = Helper.GetDataTable("SELECT * FROM " + TblName, Conn);

            // ������
            TreeNode newNode = father_node.Nodes.Add(nodeName);
            newNode.Tag = keys;
            newNode.ForeColor = ColorNodeNormal;
            int quest_id = Convert.ToInt32(keys[0]);
            newNode.ImageIndex = Logic.GetQuestImageIndex(quest_id);
            newNode.SelectedImageIndex = newNode.ImageIndex;
            nodeData.AddData(newNode, list); // �������������
            treeData.AddData(quest_id.ToString()); // �����sys_quests_order��
            treeData.ResetOrder(newNode); // ������˳��

            ///////////////////////////
            //ע�⣬�������½��������������������״̬���������BUG
            // ����������״̬
            /*
            if (!newNode.IsEditing)
            {
                treeView1.SelectedNode = newNode;
                treeView1.LabelEdit = true;
                newNode.BeginEdit();
            }*/
            //////////////////////////
            treeView1.SelectedNode = newNode;
        }

        // ��� �����һ
        private void buttonItem4_Click(object sender, EventArgs e)
        {
            if (this.m_isReadOnly) return;
            TreeNode newNode = PopupNode.Nodes.Add("�����һ");
            object[] keys = new object[1];
            keys[0] = -4;
            newNode.Tag = keys;
            newNode.ForeColor = ColorNodeOne;

            TreeNode node_surf = newNode.Nodes.Add("��������");
            object[] keys_surf = new object[1];
            keys_surf[0] = -5;
            node_surf.Tag = keys_surf;
            node_surf.ForeColor = ColorNodeSurf;

            treeData.ResetOrder(newNode);
            treeView1.SelectedNode = newNode;
        }

        // ��� ȫ�����
        private void buttonItem5_Click(object sender, EventArgs e)
        {
            if (this.m_isReadOnly) return;
            TreeNode newNode = PopupNode.Nodes.Add("ȫ�����");
            object[] keys = new object[1];
            keys[0] = -3;
            newNode.Tag = keys;
            newNode.ForeColor = ColorNodeAll;

            TreeNode node_surf = newNode.Nodes.Add("��������");
            object[] keys_surf = new object[1];
            keys_surf[0] = -5;
            node_surf.Tag = keys_surf;
            node_surf.ForeColor = ColorNodeSurf;

            treeData.ResetOrder(newNode);
            treeView1.SelectedNode = newNode;
        }

        // ����
        private void buttonItem6_Click(object sender, EventArgs e)
        {
            CopySrcNode = PopupNode;
        }

        // ճ��
        private void buttonItem7_Click(object sender, EventArgs e)
        {
            if (this.m_isReadOnly) return;
            string[] ColNamesRemain = new string[] { "QuestID", "PrequestLogic", "PrequestID1", "PrequestID2", "PrequestID3", "PrequestID4", "SubsequenceID", CatField };
            DataTable tbl = Helper.GetDataTable("SELECT * FROM " + TblName, Conn);
            Helper.SqlUpdateRecordAs(ref tbl, TblName, CopySrcNode.Tag, PopupNode.Tag, ColNamesRemain, Conn);
            
            bool desMark = PopupNode.Text.Contains(SubsequenceMark);
            bool srcMark = CopySrcNode.Text.Contains(SubsequenceMark);

            if (srcMark)
                PopupNode.Text = CopySrcNode.Text.Remove(CopySrcNode.Text.Length - SubsequenceMark.Length);
            else
                PopupNode.Text = CopySrcNode.Text;

            if (desMark)
                PopupNode.Text += SubsequenceMark;
        }

        // ɾ��
        private void buttonItem8_Click(object sender, EventArgs e)
        {
            if (this.m_isReadOnly) return;
            int DeleteKey = Convert.ToInt32(((object[])PopupNode.Tag)[0]);
            int DeleteFatherKey = -100;
            if (PopupNode.Parent != null)
                DeleteFatherKey = Convert.ToInt32(((object[])PopupNode.Parent.Tag)[0]);
            bool DeleteIsClone = DeleteKey > 0 && (DeleteFatherKey == -3 || DeleteFatherKey == -4);

            // ɾ�����Ǿ���
            if (DeleteIsClone)
            {
                // ����ɾ���˾������µ�ǰ�� id ���ϡ�
                int[] newPrequestIDs = new int[4];
                int newPrequestCount = 0;
                foreach (TreeNode node in PopupNode.Parent.Nodes)
                {
                    if (node == PopupNode)
                        continue;

                    int key = Convert.ToInt32(((object[])node.Tag)[0]);
                    if (key > 0)
                    {
                        newPrequestIDs[newPrequestCount++] = key;
                    }
                }

                // ���º����������һ���ǰ���ֶ��ǡ�
                foreach (TreeNode node in PopupNode.Parent.Nodes)
                {
                    int key = Convert.ToInt32(((object[])node.Tag)[0]);
                    if (key < 0)
                    {
                        foreach (TreeNode n in node.Nodes)
                        {
                            string[] Fields = new string[4];
                            object[] Values = new object[4];
                            for (int i = 0; i < newPrequestCount; i++)
                            {
                                Fields[i] = "PrequestID" + Convert.ToString(i + 1);
                                Values[i] = newPrequestIDs[i];
                            }
                            for (int i = 0; i < 4 - newPrequestCount; i++)
                            {
                                Fields[4 - i - 1] = "PrequestID" + Convert.ToString(4 - i);
                                Values[4 - i - 1] = DBNull.Value;
                            }
                            Helper.SqlUpdateRecord(TblName, n.Tag, Fields, Values, Conn);
                        }

                        break;
                    }
                }

                // ɾ�������
                PopupNode.Remove();
            }

            // ɾ������������㣬����Ӱ�쵽��¼��������
            else
            {
                if(PopupNode.Nodes.Count > 0) // ���ӽ�㣬������ɾ��
                {
                    if (DeleteKey > 0) // ������
                    {
                        MessageBox.Show("�������к�����񣬲�����ɾ��!", "ɾ��������", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                    else // �Ƿ���
                    {
                        MessageBox.Show("�÷���ǿգ�������ɾ��!", "ɾ��������", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                }
                else // ��ʾȷ�Ϻ�ɾ��
                {
                    DialogResult result;
                    if(DeleteKey > 0) // ������
                    {
                        result = MessageBox.Show("ȷ��ɾ����������?", "ɾ��������", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                    }
                    else // �Ƿ���
                    {
                        result = MessageBox.Show("ȷ��ɾ���÷�����?", "ɾ��������", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                    }
                    if(result == DialogResult.Yes)
                    {
                        // ɾ�����ݿ�
                        int deleted_count = DeleteAllQuest(PopupNode);
                        // ɾ�������
                        AllFoundNodes = Helper.FindAllNodesByID(treeView1, DeleteKey);
                        foreach (TreeNode nodeToDel in AllFoundNodes)
                        {
                            nodeToDel.Remove();
                        }
                        if (DeleteKey < 0)
                            PopupNode.Remove();

                        string rpt = string.Format("ɾ����ɣ�\r\n\r\nɾ����¼������{0}", deleted_count);
                        MessageBox.Show(rpt, "�ɹ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }      
            }
        }

        // ��������
        private int DeleteAllQuest(TreeNode node)
        {
            ArrayList IDsToDel = new ArrayList();
            AddToDelID(node, ref IDsToDel);
            Helper.SqlRemoveRecords(TblName, IDsToDel, Conn);
            return IDsToDel.Count;
        }

        // ��������
        private void AddToDelID(TreeNode node, ref ArrayList IDsToDel)
        {
            foreach (TreeNode n in node.Nodes)
            {
                AddToDelID(n, ref IDsToDel);
            }

            int tag = Convert.ToInt32(((object[])node.Tag)[0]);
            int father_tag = -100;
            if (node.Parent != null)
                father_tag = Convert.ToInt32(((object[])node.Parent.Tag)[0]);
            if (tag > 0 && father_tag != -3 && father_tag != -4) // ���Ǿ����㣬��������
            {
                IDsToDel.Add(node.Tag);
                treeData.RemoveRecord(tag.ToString()); // ɾ��ID��ż�¼
            }
        }


        // drag ���
        private void treeView1_DragDrop(object sender, DragEventArgs e)
        {
            if (this.m_isReadOnly) return;
            DialogResult result = MessageBox.Show("�Ƿ�ȷ���ƶ���������", "��������", MessageBoxButtons.YesNo);

            if (result == DialogResult.Yes)
            {
                if (e.Data.GetDataPresent("System.Windows.Forms.TreeNode", false))
                {
                    Point pt = ((TreeView)sender).PointToClient(new Point(e.X, e.Y));
                    TreeNode Des_Node = ((TreeView)sender).GetNodeAt(pt);
                    TreeNode Src_Node = (TreeNode)e.Data.GetData("System.Windows.Forms.TreeNode");

                    LockWindowUpdate(theTree.Handle);

                    TreeNode newAddNode = null;
                    try
                    {
                        newAddNode = DragNode(Des_Node, Src_Node, e.Effect);
                    }
                    catch (System.Exception ex)
                    {
                        MessageBox.Show("���϶���� (DragNode) ʱ��������\r\n\r\n������ϢΪ��" + ex.Message);
                    }

                    if (newAddNode != null) // �ɹ��ģ���Ч�����϶�
                    {
                        theTree.SelectedNode = newAddNode;
                        if (e.Effect != DragDropEffects.Copy) // ����ͬ������϶�
                        {
                            Des_Node.Expand();
                        }      
                    }
                    OldDragOver.BackColor = Color.Empty;

                    LockWindowUpdate(IntPtr.Zero);
                }
            }
            else
            {
                OldDragOver.BackColor = Color.Empty;
                OldDragOver = null;
            }

            // ����
            TblLock.UnLock();
            LastVer = TblLock.GetServerLastVersion();
            OrderLock.UnLock();
            LastOrderVer = OrderLock.GetServerLastVersion();
        }

        /// <summary>
        /// �������ϲ������Ƿ��Ѿ������ض���IDֵ
        /// </summary>
        /// <param name="node">�����Ľ��</param>
        /// <param name="id">����IDֵ</param>
        /// <returns>�ý���Ƿ������ϲ����д���</returns>
        private bool CheckExist(TreeNode node, string id)
        {
            TreeNode fatherNode = node.Parent;
            while(fatherNode != null)
            {
                foreach(TreeNode node1 in fatherNode.Nodes)
                {
                    if(((object[])node1.Tag)[0].ToString().Trim() == id)
                    {
                        return true;
                    }
                }
                fatherNode = fatherNode.Parent;
            }
            return false;
        }

        private TreeNode DragNode(TreeNode desnode, TreeNode srcnode, DragDropEffects type)
        {
            if (this.m_isReadOnly) return null;
            TreeNode newAddNode = null;

            int des_key = Convert.ToInt32(((object[])desnode.Tag)[0]);
            int des_parent_key = -100;
            if (desnode.Parent != null)
            {
                des_parent_key = Convert.ToInt32(((object[])desnode.Parent.Tag)[0]);
            }
            int src_key = Convert.ToInt32(((object[])srcnode.Tag)[0]);

            object[] des_key_s = new object[1];
            des_key_s[0] = des_key;
            object[] src_key_s = new object[1];
            src_key_s[0] = src_key;

            // Դ��㸸�׵ĺ������
            if (srcnode.Text.Contains(SubsequenceMark))
            {
                string[] Fields = new string[1];
                Fields[0] = "SubsequenceID";
                object[] Values = new object[1];
                Values[0] = DBNull.Value;
                Helper.SqlUpdateRecord(TblName, srcnode.Parent.Tag, Fields, Values, Conn);
                srcnode.Text = srcnode.Text.Remove(srcnode.Text.Length - SubsequenceMark.Length);
            }

            if (type == DragDropEffects.Move) // �����, �ƶ����
            {
                // �ѽ���ϵ��Լ��������ϣ��ǲ�����ġ�
                if (Helper.IsAncestorOffspring(srcnode, desnode))
                {
                    MessageBox.Show("�����϶����Լ����������ϡ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return null;
                }
                // �ѽ���϶������׻����Լ��ϣ����ò�����
                if (srcnode.Parent == desnode || srcnode == desnode)
                    return null;
                // �϶����������ϣ��ǲ�����ġ�
                if (des_key > 0 && (des_parent_key == -3 || des_parent_key == -4))
                {
                    MessageBox.Show("�����϶����������ϡ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return null;
                }

                // �ϵ�ʵ�����
                if (des_key > 0)
                {
                    string[] Fields = new string[6];
                    Fields[0] = "PrequestLogic";
                    Fields[1] = "PrequestID1";
                    Fields[2] = "PrequestID2";
                    Fields[3] = "PrequestID3";
                    Fields[4] = "PrequestID4";
                    Fields[5] = "_AreaQuest";
                    object[] Values = new object[6];
                    Values[0] = DBNull.Value;
                    Values[1] = des_key;
                    Values[2] = DBNull.Value;
                    Values[3] = DBNull.Value;
                    Values[4] = DBNull.Value;
                    Values[5] = DBNull.Value;
                    Helper.SqlUpdateRecord(TblName, src_key_s, Fields, Values, Conn);

                    newAddNode = (TreeNode)(srcnode.Clone());
                    desnode.Nodes.Add(newAddNode);
                    newAddNode.ForeColor = ColorNodeNormal;
                    
                    srcnode.Remove();
                }

                // �϶��� ��ȫ����ɡ��������һ�� ��
                else if (des_key == -3 || des_key == -4)
                {
                    if (desnode.Nodes.Count >= 5)
                    {
                        MessageBox.Show("�������������Ӹ����ǰ������", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return null;
                    }

                    // ���������, ���������Ž��Ķ�����
                    TreeNode[] node_store = new TreeNode[4];
                    for (int i = 0; i < desnode.Nodes.Count; i++)
                    {
                        node_store[i] = (TreeNode)(desnode.Nodes[i].Clone());
                    }
                    desnode.Nodes.Clear();
                    TreeNode newNode = (TreeNode)(srcnode.Clone());
                    newNode.Nodes.Clear(); // ���϶���ǰ�����񼯺���Ľ�㣬����Ҫ��ʾ�ӽ�㡣
                    newNode.Text = newNode.Text.Contains(SubsequenceMark) ? newNode.Text.Remove(newNode.Text.Length - SubsequenceMark.Length) : newNode.Text;
                    newNode.ForeColor = ColorNodeClone;
                    newAddNode = newNode;
                    desnode.Nodes.Add(newNode);
                    for (int i = 0; i < 4; i++)
                    {
                        if (node_store[i] != null)
                        {
                            desnode.Nodes.Add(node_store[i]);
                        }
                    }

                    // �����µ�ǰ������
                    int[] newPrequestIDs = new int[4];
                    int newPrequestCount = 0;
                    foreach (TreeNode node in desnode.Nodes)
                    {
                        int key = Convert.ToInt32(((object[])node.Tag)[0]);
                        if (key > 0)
                        {
                            newPrequestIDs[newPrequestCount++] = key;
                        }
                    }

                    // ���º����������һ���ǰ���ֶ��ǡ�
                    foreach (TreeNode node in desnode.Nodes)
                    {
                        int key = Convert.ToInt32(((object[])node.Tag)[0]);
                        if (key < 0) // �ҵ��˺�������Ľ��
                        {
                            foreach (TreeNode n in node.Nodes)
                            {
                                string[] Fields = new string[4];
                                object[] Values = new object[4];
                                for (int i = 0; i < newPrequestCount; i++)
                                {
                                    Fields[i] = "PrequestID" + Convert.ToString(i + 1);
                                    Values[i] = newPrequestIDs[i];
                                }
                                for (int i = 0; i < 4 - newPrequestCount; i++)
                                {
                                    Fields[4 - i - 1] = "PrequestID" + Convert.ToString(4 - i);
                                    Values[4 - i - 1] = DBNull.Value;
                                }
                                Helper.SqlUpdateRecord(TblName, n.Tag, Fields, Values, Conn);
                            }

                            break;
                        }
                    }
                }

                // �϶�������������
                else if (des_key == -5)
                {
                    if (CheckExist(desnode, src_key.ToString())) // ������ѭ��
                    {
                        MessageBox.Show("������Ӹý�㣬���Ѿ����丸����д���!");
                        return null;
                    }

                    // �����µ�ǰ������
                    int[] newPrequestIDs = new int[4];
                    int newPrequestCount = 0;
                    foreach (TreeNode node in desnode.Parent.Nodes)
                    {
                        int key = Convert.ToInt32(((object[])node.Tag)[0]);
                        if (key > 0)
                        {
                            newPrequestIDs[newPrequestCount++] = key;
                        }
                    }

                    // ���±��������ǰ���ֶΣ���ǰ��logic
                    string[] Fields = new string[6];
                    object[] Values = new object[6];
                    for (int i = 0; i < newPrequestCount; i++)
                    {
                        Fields[i] = "PrequestID" + Convert.ToString(i + 1);
                        Values[i] = newPrequestIDs[i];
                    }
                    for (int i = 0; i < 4 - newPrequestCount; i++)
                    {
                        Fields[4 - i - 1] = "PrequestID" + Convert.ToString(4 - i);
                        Values[4 - i - 1] = DBNull.Value;
                    }
                    Fields[4] = "PrequestLogic";
                    Fields[5] = "_AreaQuest";
                    bool prequestlogic = (des_parent_key == -3) ? true : false;
                    Values[4] = prequestlogic;
                    Values[5] = DBNull.Value;
                    Helper.SqlUpdateRecord(TblName, srcnode.Tag, Fields, Values, Conn);

                    // ������
                    newAddNode = (TreeNode)(srcnode.Clone());
                    desnode.Nodes.Add(newAddNode);
                    newAddNode.ForeColor = ColorNodeNormal;
                    srcnode.Remove();
                }

                // �϶�����������
                else if (des_key == -2)
                {
                    string[] Fields = new string[6];
                    Fields[0] = "PrequestLogic";
                    Fields[1] = "PrequestID1";
                    Fields[2] = "PrequestID2";
                    Fields[3] = "PrequestID3";
                    Fields[4] = "PrequestID4";
                    Fields[5] = CatField;
                    object[] Values = new object[6];
                    Values[0] = DBNull.Value;
                    Values[1] = DBNull.Value;
                    Values[2] = DBNull.Value;
                    Values[3] = DBNull.Value;
                    Values[4] = DBNull.Value;
                    Values[5] = desnode.Text;
                    Helper.SqlUpdateRecord(TblName, srcnode.Tag, Fields, Values, Conn);

                    // ������
                    newAddNode = (TreeNode)(srcnode.Clone());
                    desnode.Nodes.Add(newAddNode);
                    newAddNode.ForeColor = ColorNodeNormal;
                    srcnode.Remove();
                }

                // δ֪���
                else
                {
                    MessageBox.Show("��֪����ô�϶���", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                treeData.ResetOrder(newAddNode);
            }
            else if (type == DragDropEffects.Copy) // �Ҽ���, �ƶ�
            {
                if (srcnode.Parent == desnode) // Ŀ�������϶����ĸ����
                {
                    List<TreeNode> list1 = new List<TreeNode>();
                    foreach (TreeNode node in desnode.Nodes)
                    {
                        if (node != srcnode)
                        {
                            list1.Add(node);
                        }
                    }
                    desnode.Nodes.Clear();
                    desnode.Nodes.Add(srcnode);
                    foreach (TreeNode node in list1)
                    {
                        desnode.Nodes.Add(node);
                    }
                }
                else
                {
                    if ((srcnode.Level != desnode.Level) || (srcnode.Parent != desnode.Parent)) // ����Ƿ��϶���ͬ�������
                    {
                        MessageBox.Show("�����϶���ͬ�������!");
                        return null;
                    }

                    TreeNodeCollection nodes;
                    if (srcnode.Level == 0) // ��һ�������
                    {
                        nodes = treeView1.Nodes;
                    }
                    else // �ǵ�һ�������
                    {
                        nodes = srcnode.Parent.Nodes;
                    }

                    List<TreeNode> list1 = new List<TreeNode>();
                    List<TreeNode> list2 = new List<TreeNode>();
                    bool findDesNode = false;
                    foreach (TreeNode node in nodes)
                    {
                        if (node != srcnode) // �Ƿ�Ϊ��ʼ���
                        {
                            if (node == desnode)
                            {
                                findDesNode = true;
                                list1.Add(node);
                                continue;
                            }

                            if (!findDesNode)
                            {
                                list1.Add(node);
                            }
                            else
                            {
                                list2.Add(node);
                            }
                        }
                    }

                    nodes.Clear();
                    foreach (TreeNode node in list1)
                    {
                        nodes.Add(node);
                    }
                    nodes.Add(srcnode);
                    foreach (TreeNode node in list2)
                    {
                        nodes.Add(node);
                    }
                }
             
                newAddNode = srcnode;
                treeData.ResetOrder(newAddNode);
            }
            return newAddNode;
        }

        private void baseTree_ItemDrag(object sender, ItemDragEventArgs e)
        {
            if (this.m_isReadOnly) return;

            // �����϶�����
            TreeNode src_node = ((TreeNode)e.Item);
            int src_key = Convert.ToInt32(((object[])src_node.Tag)[0]);
            if (src_key <= 0) 
            {
                return;
            }
            // �����϶������������
            int src_parent_key = Convert.ToInt32(((object[])src_node.Parent.Tag)[0]);
            if (src_parent_key == -3 || src_parent_key == -4)
            {
                return;
            }

            if (LastVer != TblLock.GetServerLastVersion())
            {
                int selected_key = Convert.ToInt32(((object[])src_node.Tag)[0]);
                MyParentClass.LoadTreePlugin(selected_key);
                LastVer = TblLock.GetServerLastVersion();
                return;
            }

            if (LastOrderVer != OrderLock.GetServerLastVersion())
            {
                int selected_key = Convert.ToInt32(((object[])src_node.Tag)[0]);
                MyParentClass.LoadTreePlugin(selected_key);
                LastOrderVer = OrderLock.GetServerLastVersion();
                return;
            }

            // ����
            if (!TblLock.Lock())
            {
                //MessageBox.Show("�����������޸��������ṹ�����ݲ����϶���");
                //treeView1.SelectedNode = null;
                return;
            }
            if(!OrderLock.Lock())
            {
                return;
            }

            if (e.Button == MouseButtons.Left)
            {
                DoDragDrop(e.Item, DragDropEffects.Move);
            }
            else if (e.Button == MouseButtons.Right)
            {
                DoDragDrop(e.Item, DragDropEffects.Copy);
            }
        }

        private void treeView1_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = e.AllowedEffect;
        }

        private void treeView1_DragOver(object sender, DragEventArgs e)
        {
            if (this.m_isReadOnly) return;
            Point targetPoint = treeView1.PointToClient(new Point(e.X, e.Y));

            TreeViewHitTestInfo tvhti = treeView1.HitTest(targetPoint);

            if (OldDragOver != tvhti.Node)
            {
                if (OldDragOver != null)
                    OldDragOver.BackColor = Color.Empty;

                OldDragOver = tvhti.Node;
                OldDragOver.BackColor = ColorNodeDragOld;
            }

            if (tvhti.Location == TreeViewHitTestLocations.PlusMinus)
            {
                tvhti.Node.Expand();
            }
        }

        private void buttonItem3_Click(object sender, EventArgs e) // ������
        {
            if (this.m_isReadOnly) return;
            this.treeView1.LabelEdit = true;
            if (!PopupNode.IsEditing)
            {
                PopupNode.BeginEdit();

                if (!PopupNode.IsEditing) // ������ת��༭״̬����ֱ�ӵ����ı������
                {
                    InputForm iForm = new InputForm("�������µ�����", PopupNode.Text);
                    if (iForm.ShowDialog() == DialogResult.OK)
                    {
                        PopupNode.Text = iForm.InputText;

                        // ����ԭ�ȴ���༭�¼��ķ���
                        NodeLabelEditEventArgs arg = new NodeLabelEditEventArgs(PopupNode, iForm.InputText);
                        treeView1_AfterLabelEdit(this.treeView1, arg);
                    }
                }
            }
        }

        private void buttonItem10_Click(object sender, EventArgs e) // ��Ϊ(ȡ��)ֱ�Ӻ���
        {
            if (this.m_isReadOnly) return;
            int node_key = Convert.ToInt32(((object[])PopupNode.Tag)[0]);

            string[] Fields = new string[1];
            Fields[0] = "SubsequenceID";
            object[] Values = new object[1];
            Values[0] = node_key;
            object[] NullValues = new object[1];
            NullValues[0] = DBNull.Value;

            string sql = string.Format("SELECT * FROM {0}", TblName);
            DataTable tbl = Helper.GetDataTable(sql, Conn);
            DataRow row = tbl.Rows.Find((object[])(PopupNode.Parent.Tag));

            if (PopupNode.Text.Contains(SubsequenceMark)) // ȡ��ֱ�Ӻ���
            {
                Helper.SqlUpdateRecord(TblName, PopupNode.Parent.Tag, Fields, NullValues, Conn);
                PopupNode.Text = PopupNode.Text.Remove(PopupNode.Text.Length - SubsequenceMark.Length);
            }
            else
            {
                if (row != null && row["SubsequenceID"] != DBNull.Value && row["SubsequenceID"] != null && row["SubsequenceID"].ToString().Trim() != "0")
                {
                    DialogResult result = MessageBox.Show("���������ֱ�Ӻ�����\r\n\r\n�����Ҫ���Ǹ����ĺ�����", "ȷ��", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2);
                    if (result == DialogResult.Yes)
                    {
                        Helper.SqlUpdateRecord(TblName, PopupNode.Parent.Tag, Fields, Values, Conn);
                        foreach (TreeNode node in PopupNode.Parent.Nodes)
                        {
                            if (node.Text.Contains(SubsequenceMark))
                            {
                                node.Text = node.Text.Remove(node.Text.Length - SubsequenceMark.Length);
                                break;
                            }
                        }
                        PopupNode.Text += SubsequenceMark;

                    }
                }
                else
                {
                    Helper.SqlUpdateRecord(TblName, PopupNode.Parent.Tag, Fields, Values, Conn);
                    PopupNode.Text += SubsequenceMark;
                }
            }
        }

        private void buttonItem13_Click(object sender, EventArgs e) // ͬ������
        {
            if (this.m_isReadOnly) return;
            int father_key = Convert.ToInt32(((object[])PopupNode.Parent.Tag)[0]);
            if (PopupNode.ForeColor == ColorNodeNormal) // ��Ϊͬ������
            {
                string[] Fields = new string[1];
                Fields[0] = "_AreaQuest";
                object[] Values = new object[1];
                Values[0] = father_key;
                string[] FieldsPrequest = new string[4];
                FieldsPrequest[0] = "PrequestID1";
                FieldsPrequest[1] = "PrequestID2";
                FieldsPrequest[2] = "PrequestID3";
                FieldsPrequest[3] = "PrequestID4";
                object[] NullValues = new object[4];
                NullValues[0] = DBNull.Value;
                NullValues[1] = DBNull.Value;
                NullValues[2] = DBNull.Value;
                NullValues[3] = DBNull.Value;

                Helper.SqlUpdateRecord(TblName, PopupNode.Tag, Fields, Values, Conn);
                Helper.SqlUpdateRecord(TblName, PopupNode.Tag, FieldsPrequest, NullValues, Conn);
                PopupNode.ForeColor = ColorNodeAreaQuestOnly;
            }
            else // ȡ��ͬ������
            {
                string[] Fields = new string[1];
                Fields[0] = "_AreaQuest";
                object[] Values = new object[1];
                Values[0] = DBNull.Value;
                string[] FieldsPrequest = new string[1];
                FieldsPrequest[0] = "PrequestID1";
                object[] ValuesPrequest = new object[4];
                ValuesPrequest[0] = father_key;

                Helper.SqlUpdateRecord(TblName, PopupNode.Tag, Fields, Values, Conn);
                Helper.SqlUpdateRecord(TblName, PopupNode.Tag, FieldsPrequest, ValuesPrequest, Conn);
                PopupNode.ForeColor = ColorNodeNormal;
            }

        }

        /// <summary>
        /// ��������㴦��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            if (this.m_isReadOnly) 
            {
                e.CancelEdit = true;
                return;
            }
            string oldName = e.Node.Text.Trim();
            string newName = string.Empty;
            if (e.Label != null)
                newName = e.Label.Trim();
            else
                newName = e.Node.Text.Trim();

            if (oldName == newName) // ʵ��û�и���������
                return;

            int key = Convert.ToInt32(((object[])(e.Node.Tag))[0]);
            if (key > 0) // ����������
            {
                int start = newName.IndexOf("[");
                int end = newName.LastIndexOf("]");
                if (start != -1 && end != -1)
                    newName = newName.Remove(start, end - start + 1);
                if (newName.Contains(SubsequenceMark))
                    newName = newName.Replace(SubsequenceMark, "");

                string[] Fields = new string[1];
                Fields[0] = "QuestName";
                object[] Values = new object[1];
                Values[0] = newName;
                Helper.SqlUpdateRecord(TblName, e.Node.Tag, Fields, Values, Conn);
            }
            else if (key == -2) // �������������
            {
                if(Conn.State == ConnectionState.Closed)
                {
                    Conn.Open();
                }
                SqlCommand cmd = Conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1}='{2}' WHERE {3}='{4}'", TblName, CatField, newName, CatField, oldName);
                cmd.ExecuteNonQuery();
                if(Conn.State == ConnectionState.Open)
                {
                    Conn.Close();
                }
            }
        }

        /// <summary>
        /// �ҵ���һ�����
        /// </summary>
        /// <param name="shift"></param>
        private void NextNodeWithSameID(bool shift)
        {
            if (treeView1.SelectedNode == null)
                return;
            int select_id = Convert.ToInt32(((object[])(treeView1.SelectedNode.Tag))[0]);
            if (select_id <= 0)
                return;

            if (AllFoundNodes.Count == 0)
            {
                AllFoundNodes = Helper.FindAllNodesByID(treeView1, select_id);
            }
            int new_selected_index = AllFoundNodes.IndexOf(treeView1.SelectedNode) + (shift ? -1 : 1);
            if (new_selected_index >= AllFoundNodes.Count)
                new_selected_index = 0;
            if (new_selected_index < 0)
                new_selected_index = AllFoundNodes.Count - 1;
            treeView1.SelectedNode = (TreeNode)(AllFoundNodes[new_selected_index]);
        }

        /// <summary>
        /// �򿪲��Ҵ���
        /// </summary>
        public void OpenFindForm()
        {
            if (FindingForm != null)
            {
                return;
            }
            FindFrm findfrm = new FindFrm(this);
            FindingForm = findfrm;
            findfrm.Show();
        }

        /// <summary>
        /// ����㰴���¼�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_KeyDown(object sender, KeyEventArgs e)
        {
            switch(e.KeyCode)
            {
                case Keys.F: // ���ҹ���
                    {
                        if(e.Control)
                        {
                            OpenFindForm();
                        }
                        break;
                    }
                case Keys.F4: // ʵ��/���� ����ת
                    {
                        NextNodeWithSameID(e.Shift);
                        break;
                    }
                case Keys.Delete: // ɾ�����
                    {
                        if (this.m_isReadOnly) return;
                        PopupNode = treeView1.SelectedNode;
                        buttonItem8_Click(null, null);
                        break;
                    }
                case Keys.F2: // ���������
                    {
                        PopupNode = treeView1.SelectedNode;
                        buttonItem3_Click(null, null);
                        break;
                    }
                case Keys.F3: // չ�����
                    {
                        if (treeView1.SelectedNode.IsExpanded)
                        {
                            treeView1.SelectedNode.Collapse(false);
                        }
                        else
                        {
                            treeView1.SelectedNode.ExpandAll();
                        }
                        break;
                    }
            }
        }

        /// <summary>
        /// ��ʾ������Ϣ���ȼ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonItemLevel_Click(object sender, EventArgs e) 
        {
            DisplayLevel = !DisplayLevel;
            DisplayID = !DisplayID;
            buttonItemID.Checked = DisplayID;
            buttonItemLevel.Checked = DisplayLevel;
            // UpdateDisplayInfo();
            // ������������ʾ������
            nodeCount = 0;
            proForm.SetMaxValue(nodeData.NodeCount);
            proForm.Visible = true;
            UpdateDisplayInfomation();
            proForm.Visible = false;
        }

        /// <summary>
        /// ��ʾ������Ϣ��ID
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonItemID_Click(object sender, EventArgs e) 
        {
            DisplayLevel = !DisplayLevel;
            DisplayID = !DisplayID;
            buttonItemID.Checked = DisplayID;
            buttonItemLevel.Checked = DisplayLevel;
            // UpdateDisplayInfo();
            // ������������ʾ������
            nodeCount = 0;
            proForm.SetMaxValue(nodeData.NodeCount);
            proForm.Visible = true;
            UpdateDisplayInfomation();
            proForm.Visible = false;
        }

        /// <summary>
        /// ��ʾ���ĵ�һ�������Ϣ
        /// </summary>
        private void UpdateDisplayInfomation()
        {
            nameTable = nodeData.GetNameTable();
            levelTable = nodeData.GetLevelTable();

            foreach(TreeNode node in treeView1.Nodes)
            {
                UpdateDisplayInfomation(node);
            }
        }

        /// <summary>
        /// ��ʾ��㼰���ӽ�����Ϣ
        /// </summary>
        /// <param name="node">��ǰ���</param>
        private void UpdateDisplayInfomation(TreeNode node)
        {       
            string id = ((object[])node.Tag)[0].ToString();

            object name = nameTable[id];
            object level = levelTable[id];

            if ((name != null) && (level != null))
            {
                if (DisplayID) // ��ʾID
                {
                    node.Text = string.Format("[ID {1}]{0}", name.ToString(), id);
                }
                else // ��ʾLevel
                {                    
                    node.Text = string.Format("[Level {1}]{0}", name.ToString(), level.ToString());
                }
                nodeCount++;
                proForm.SetValue(nodeCount);
            }
            
            foreach (TreeNode node1 in node.Nodes)
            {
                UpdateDisplayInfomation(node1);
            }
        }

        /// <summary>
        /// ˢ�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonItem12_Click(object sender, EventArgs e)
        {
            int selected_key = Convert.ToInt32(((object[])PopupNode.Tag)[0]);
            MyParentClass.LoadTreePlugin(selected_key);
        }

        /// <summary>
        /// ���ҽ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e) 
        {
            OpenFindForm();
        }

        /// <summary>
        /// ��ת���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            NextNodeWithSameID(false);
            treeView1.Focus();
        }

        /// <summary>
        /// չ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode == null)
                return;

            if (treeView1.SelectedNode.IsExpanded)
            {
                treeView1.SelectedNode.Collapse(false);
            }
            else
            {
                treeView1.SelectedNode.ExpandAll();
            }
            treeView1.Focus();
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonItem14_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treeView1.SelectedNode;
            if (currentNode != null)
            {
                cForm = new ComputeForm(Conn, currentNode, nodeData.GetData(currentNode));
                cForm.Visible = true;
            }
        }

        /// <summary>
        /// �����༭
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bMultiEdit_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treeView1.SelectedNode;
            if(currentNode != null)
            {
                multiManager = new MultiTaskEditManager(Conn, currentNode, fieldList);
                multiManager.Operate();
            }
        }

        /// <summary>
        /// ���������
        /// </summary>
        /// <param name="node">Ҫ��ǵ������</param>
        private void MarkQuest(TreeNode node)
        {
            string id = (node.Tag as object[])[0].ToString();

            try
            {
                if(Conn.State == ConnectionState.Closed) // ��sql����
                {
                    Conn.Open();
                }

                string sqlString = string.Format("UPDATE {0} SET Marked = 1 WHERE QuestID = '{1}'",
                    "sys_quests_order", id);
                SqlCommand cmd = new SqlCommand(sqlString, Conn);
                cmd.ExecuteNonQuery();
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڱ��������ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(Conn.State == ConnectionState.Open) // �ر�sql����
                {
                    Conn.Close();
                }
            }

            node.Text = markedString + node.Text;            
        }

        /// <summary>
        /// ȡ�����������
        /// </summary>
        /// <param name="node">Ҫȡ����ǵ������</param>
        private void UnmarkQuest(TreeNode node)
        {
            string id = (node.Tag as object[])[0].ToString();
            
            try
            {
                if (Conn.State == ConnectionState.Closed) // ��sql����
                {
                    Conn.Open();
                }

                string sqlString = string.Format("UPDATE {0} SET Marked = 0 WHERE QuestID = '{1}'",
                    "sys_quests_order", id);
                SqlCommand cmd = new SqlCommand(sqlString, Conn);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڱ��������ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (Conn.State == ConnectionState.Open) // �ر�sql����
                {
                    Conn.Close();
                }
            }

            node.Text = node.Text.Substring(2);            
        }

        /// <summary>
        /// �������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bMarkQuest_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treeView1.SelectedNode;
            if(currentNode.Level > 0 && !currentNode.Text.StartsWith(markedString) &&
                ((currentNode.ForeColor == ColorNodeNormal) ||
                (currentNode.ForeColor == ColorNodeAreaQuestOnly))) // ��ɫ��������
            {                
                MarkQuest(currentNode);
            }
        }

        /// <summary>
        /// ȡ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bCancelMark_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treeView1.SelectedNode;
            if(currentNode.Level > 0 && currentNode.Text.StartsWith(markedString)) // ��ǹ���������
            {
                UnmarkQuest(currentNode);
            }
        }

        private void treeView1_NodeMouseHover(object sender, TreeNodeMouseHoverEventArgs e)
        {

            int[] qtCnt = new int[11];//�������ͼ�������
            string strtip = "";
            if (e.Node.ImageIndex == 9)//����ڵ�����Ϊ����
            {
                StatQuestType(e.Node, qtCnt);
                strtip = e.Node.Text + StatDisplay(qtCnt);

                e.Node.ToolTipText = strtip;
            }
            if (e.Node.Parent == null && e.Node.ImageIndex != 9)
            {               
                foreach (TreeNode node1 in treeView1.Nodes)
                {
                    StatQuestType(node1, qtCnt);
                }
                strtip = StatDisplay(qtCnt);
                
                e.Node.ToolTipText = strtip;
            }
        }              

        protected void StatQuestType(TreeNode node, int []na)
        {
            foreach(TreeNode node1 in node.Nodes)
            {
                na[node1.ImageIndex]++;
                StatQuestType(node1,na);
            }
        }
        protected string StatDisplay(int []na)
        {
            string []QuestTypeNames = 
            {
                "ǰ�ò��ɽ�����",
                "�ű�����",
                "�ɼ�����",
                "�ռ�����",
                "ɱ������",
                "�͵�������",
                "�Ի�����",
                "X_��ʱ����",
                "ǰ�����񼯺�",
                "��������",
                "��������"
            };
            string relstr = "";
            int i = 0;
            int sum = 0;//����������
            for (i = 0; i < 11; i++)
            {
                if (na[i] != 0)
                {
                    relstr += string.Format("{0}:{1}\n",QuestTypeNames[i],na[i]);
                    sum += na[i];
                }
            }
            relstr = string.Format("������������{0}\n", sum) + relstr;
            return relstr;
        }

        private void QuestStatbtn_Click(object sender, EventArgs e)//��������ͳ����ʾ
        {   
            

            QuestStatForm qsform = new QuestStatForm(treeView1);
           
            qsform.Show();  


        }

      

        private void Form1_Load(object sender, EventArgs e)
        {

        }     

        

    }
}