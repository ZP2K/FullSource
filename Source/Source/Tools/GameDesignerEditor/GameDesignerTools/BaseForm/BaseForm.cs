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
using DataTableRemotingClient;
using System.Net;


namespace GameDesingerTools
{
    public partial class BaseForm : Form, IKDocument
    {
        public static PropertyGrid CurPg;

        // ������, ��ʼ��
        public BaseForm(string modelname)
        {
            log.Log(TimeLog.enumLogType.ltstart, modelname, "ģ������", null);
            this.SuspendLayout();
            try
            {
                InitializeComponent();
             
                AddExtraMenuItem();

                //((Control)this).MouseWheel += new MouseEventHandler(BaseForm_MouseWheel);
                tabControl = this.tabControl1;
                InitModel(modelname);
                ProducePreviewWindows();
                //InitPreivew();
            }
            finally
            {
                
                this.ResumeLayout();
                ReadLayOut(modelname);
               
            }
        }


        /// <summary>
        /// ��ȡָ��ģ��Ĳ���
        /// add by sunyong
        /// </summary>
        /// <param name="modelname">ģ����</param>
        protected void ReadLayOut(string modelname)
        {
            string _filePath = string.Format("{0}/����/{1}_layout.xml", Application.StartupPath, modelname);
            string filePath = string.Format("{0}/����/{1}layout.xml", Application.StartupPath, modelname);
            FileInfo _fi = new FileInfo(_filePath);
            FileInfo fi = new FileInfo(filePath);
            if (_fi.Exists && _fi.Length > 0)
            {
                dotNetBarManager1.LoadLayout(_filePath); // ��ȡ��������
            }
            else if (fi.Exists && fi.Length > 0)
            {
                dotNetBarManager1.LoadLayout(filePath); // ��ȡ��������
            }
        }

        /// <summary>
        /// ����ָ��ģ��Ĳ���
        /// add by sunyong
        /// </summary>
        /// <param name="modelname">ģ����</param>
        protected void SaveLayOut(string modelname)
        {

            
            string directorypath = string.Format("{0}\\����", Application.StartupPath);
            if (!Directory.Exists(directorypath))
            {
                Directory.CreateDirectory(directorypath);
            }
            string _filePath = string.Format("{0}/{1}_layout.xml",directorypath, modelname);
            dotNetBarManager1.SaveLayout(_filePath);
        }

        
        private void AddExtraMenuItem()
        {
            m_ExtraMenuItem = new ButtonItem[20];
            for (int i = 0; i < 20; i++)
            {
                m_ExtraMenuItem[i] = new ButtonItem();
                m_ExtraMenuItem[i].Name = "extra_menu_item" + Convert.ToString(i + 1);
                m_ExtraMenuItem[i].Text = m_ExtraMenuItem[i].Name;
                m_ExtraMenuItem[i].Click += new EventHandler(this.buttonExtraMenuItem_Click);
                m_ExtraMenuItem[i].Visible = false;
            }
            this.cmNodeRightClick.SubItems.AddRange(m_ExtraMenuItem);
        }

        private void UpdateStartIDs()
        {
            StartIDs = new object[TblMain.PrimaryKey.Length];
            for (int i = 0; i < StartIDs.Length; i++)
            {
                string field = TblMain.PrimaryKey[i].ColumnName;
                /* remoting
                string sQl = string.Format("SELECT * FROM sys_id_pool WHERE tablename='{0}' AND fieldname='{1}'", MainTableName, field);
                DataTable Tbl = Helper.GetDataTable(sQl, Conn);
                */

                string filter = string.Format("tablename = '{0}' and fieldname = '{1}'", MainTableName, field);
                DataTable Tbl = Helper.GetDataTableProxy("sys_id_pool", filter, null, Conn);

                DataRow rOw = null;
                if (Tbl.Rows.Count > 0)
                {
                    rOw = Tbl.Rows[0];
                    StartIDs[i] = (int)rOw["lastid"];
                }
                else
                {
                    StartIDs[i] = 1;
                }
            }
        }

        private void AddTab(ref PageDesc tab, bool firstTab)
        {
            TabItem tabitem = new TabItem();
            TabControlPanel panel = new TabControlPanel();

            if (!tab.bCustomTab)
            {
                tab.pg.SelectedObject = null;
                tab.pg.Name = "propertyGrid" + tab.id.ToString();
                tab.pg.BackColor = this.BasePg.BackColor;
                tab.pg.CommandsDisabledLinkColor = this.BasePg.CommandsDisabledLinkColor;
                tab.pg.Dock = this.BasePg.Dock;
                tab.pg.LineColor = this.BasePg.LineColor;
                tab.pg.Location = this.BasePg.Location;
                tab.pg.PropertySort = this.BasePg.PropertySort;
                tab.pg.Size = this.BasePg.Size;
                tab.pg.TabIndex = this.BasePg.TabIndex;
                tab.pg.SelectedGridItemChanged += new SelectedGridItemChangedEventHandler(BasePg_SelectedGridItemChanged);
                tab.pg.SelectedObjectsChanged += new EventHandler(BasePg_SelectedObjectsChanged);
                tab.pg.PropertyValueChanged += new PropertyValueChangedEventHandler(this.BasePg_PropertyValueChanged);
                tab.pg.Tag = tab.id;
            }

            panel.SuspendLayout();
            this.tabControl1.Controls.Add(panel);
            if (!tab.bCustomTab)
                panel.Controls.Add(tab.pg);
            else
                this.contextMenuBar1.SetContextMenuEx(/*panel*/tabControl1, this.cmScriptOp);
            panel.Dock = BasePanel.Dock;
            panel.Location = BasePanel.Location;
            panel.Name = "tabControlPanel" + tab.id.ToString();
            panel.Padding = BasePanel.Padding;
            panel.Size = BasePanel.Size;
            panel.Style.BackColor1.Color = BasePanel.Style.BackColor1.Color;
            panel.Style.BackColor2.Color = BasePanel.Style.BackColor2.Color;
            panel.Style.Border = BasePanel.Style.Border;
            panel.Style.BorderColor.Color = BasePanel.Style.BorderColor.Color;
            panel.Style.BorderSide = BasePanel.Style.BorderSide;
            panel.Style.GradientAngle = BasePanel.Style.GradientAngle;
            panel.TabItem = tabitem;
            panel.ResumeLayout(false);

            tabitem.AttachedControl = panel;
            tabitem.Name = "tabItem" + tab.id.ToString();
            tabitem.Text = tab.name_cn;
            if (!tab.bCustomTab)
                tabitem.Tag = tab.pg;
            else
                tabitem.Tag = tab.id; // ModTabID
            this.tabControl1.Tabs.Add(tabitem);

            if (firstTab)
            {
                this.tabControl1.SelectedTab = tabitem;
            }
        }


        ////////////////////////////
        // lua ��ע�ắ��
        ////////////////////////////
        public void BeginEdit(TreeNode node)
        {
            node.BeginEdit();
        }
        public DataRow GetPropDefaultValue(string tblname)
        {
            return CustomClassCache.GetPropDefaultValue(tblname);
        }

        private void SetLastID(string table, string field, int id)
        {
            
            string sql = string.Format("SELECT * FROM sys_id_pool WHERE tablename='{0}' AND fieldname='{1}'", table, field);
            /* remoting
             DataTable tbl = Helper.GetDataTable(sql, Conn);
            */

            string filter = string.Format("tablename = '{0}' and fieldname = '{1}'", table, field);
            DataTable tbl = Helper.GetDataTableProxy("sys_id_pool", filter, null, Conn);

            DataRow row = null;
            try
            {
                //remoting SqlDataAdapter adp = new SqlDataAdapter(sql, Conn);
                if (tbl.Rows.Count <= 0)
                {
                    row = tbl.NewRow();
                    row.BeginEdit();
                    row["tablename"] = table;
                    row["fieldname"] = field;
                    row["lastid"] = id;
                    row.EndEdit();
                    tbl.Rows.Add(row);
                    /*remoting
                    SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                    adp.InsertCommand = cmdBuilder.GetInsertCommand();
                    */ 
                }
                else
                {
                    row = tbl.Rows[0];
                    row["lastid"] = id;
                    /* remoting
                    SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                    adp.UpdateCommand = cmdBuilder.GetUpdateCommand();
                    */ 
                }
                /* remoting
                int val = adp.Update(tbl);
                tbl.AcceptChanges();
                */
                int val = Helper.SaveTable(tbl, sql, Conn);
            }
            catch //(Exception ex)
            {
                tbl.RejectChanges();
            }
            tbl.Dispose();
        }


        public void ClearTreeNodes(ref TreeNodeCollection nodes)
        {
            nodes.Clear();
        }

        public TreeNode FindAncestorNode(TreeNode node)
        {
            TreeNode tra_node = node;
            while (tra_node.Parent != null)
            {
                tra_node = tra_node.Parent;
            }
            return tra_node;
        }

        public void InitPgAndPghelper()
        {
            InitPgHelpers();
            foreach (PageDesc pd in m_Pages)
            {
                if (pd.pg != null)
                    pd.pg.Refresh();
            }
        }


        ////////////////////////////
        // ���ݵ��뵼��
        ////////////////////////////

        // ����m_lsTblUpdateDefault���������ݿ�,�����ֵ��ȵı���->��ֵ
        public void UpdateTableUsingDefault(ArrayList lsTblUpdateDefault)
        {
            foreach (TblUpdateDefault tud in lsTblUpdateDefault)
            {
                //DataTable tbl = Helper.GetDataTable("SELECT * FROM " + tud.tblName, Conn);
                DataTable tbl = Helper.GetDataTableProxy(tud.tblName, Conn);
                for (int i = 0; i < tbl.Columns.Count; i++)
                {
                    if (tud.newValue[i].ToString() != tud.oldValue[i].ToString())
                    {
                        foreach (DataRow row in tbl.Rows)
                        {
                            if (row[i].ToString() == tud.oldValue[i].ToString())
                            {
                                row[i] = tud.newValue[i];
                            }
                        }
                    }
                }
                // �ύ
                /* remoting
                SqlDataAdapter adp = new SqlDataAdapter("SELECT * FROM " + tud.tblName, Conn);
                SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                adp.UpdateCommand = cmdBuilder.GetUpdateCommand();
                int val = adp.Update(tbl);
                tbl.AcceptChanges();
                 */

                string sql = string.Format("select * from {0}", tud.tblName);
                Helper.SaveTable(tbl, sql, Conn);
            }
        }
        private bool UnlockRecord()
        {
            //������¼
            foreach (PageDesc pd in m_Pages)
            {
                if (!pd.dbClass.OnLockRecord(false, null, pd.dbClass.FindKeys))
                {
                    Helper.AddLog(string.Format("{0}ҳ����ʧ�ܡ�", pd.name_cn));
                    return false;
                }
            }
            //ˢ�°汾��
            foreach (PageDesc pd in m_Pages)
            {
                pd.dbClass.RefreshLastVersion();
            }
            return true;
        }

        /// <summary>
        /// ǿ�Ʒ�ҳ�ڱ���ʱˢ��
        /// </summary>
        /// <param name="pageIndex">��ҳ���</param>
        public void ForceReloadOnSave(int pageIndex)
        {
            if (pageIndex < m_Pages.Count && pageIndex >= 0)
            {
                PageDesc pageDesc = Pages[pageIndex] as PageDesc;
                DBCustomClass dbCustomClass = pageDesc.dbClass;

                if (dbCustomClass != null)
                {
                    ForceReloadOnSave(dbCustomClass);
                }
            }
        }

        /// <summary>
        /// ǿ��DBCustomClass�ڱ���ʱˢ��
        /// </summary>
        /// <param name="dbCustomClass">��ǰDBCustomClass</param>
        private void ForceReloadOnSave(DBCustomClass dbCustomClass)
        {
            dbCustomClass.ReloadDataOnSave = true;

            foreach (CustomProperty customProperty in dbCustomClass)
            {
                if (customProperty.ValueType == enumValueType.vtExpandNode)
                {
                    DBCustomClass childDBCustomClass = customProperty.Value as DBCustomClass;

                    if (childDBCustomClass != null)
                    {
                        ForceReloadOnSave(childDBCustomClass);
                    }
                }
            }
        }

        private void SaveDocument(string strRemarks)
        {
            this.Cursor = Cursors.WaitCursor;
            
            SaveCommand savecmd = new SaveCommand();
            savecmd.Execute(); //��ʱֻ���ڼ��Ȩ��

            // ֪ͨcustomtab����
            CustomTabSave();

            foreach (PageDesc pd in m_Pages)
            {
                if (!pd.bCustomTab && pd.dbClass.ValueChanged) // ֵ�ı��ʱ�����Ҫ����
                {
                    // ���ݱ������ܶ�ʱ������ΪDataAdapter������û�м�ʱˢ�£������ڳ����ʱ����ˢ���ش�һ�Σ�
                    // ����û����顣 modify by suntao
                    try
                    {
                        if (!pd.dbClass.Save(ref m_lsTblUpdateDefault, strRemarks))
                        {
                            Helper.AddLog(string.Format("{0}ҳ����ʧ�ܡ�", pd.name_cn));
                        }
                    }
                    catch (Exception ex)
                    {
                        throw (ex);
                    }                    

                    //add by cuiwei 07.9.24
                    if (pd.qc != null)
                    {
                        if ( pd.qc.syncList.Count > 0)
                            pd.qc.syncList.Add(new unBoldItem());
                    }
                    //add end
                }
            }

            // Ĭ��ֵ���
            if (MainForm.m_bModifyDbTableDefault)
                UpdateTableUsingDefault(m_lsTblUpdateDefault);

            // ֻ��Ҫˢ�µ�ǰҳ�漴��
            PageDesc currentPageDesc = m_Pages[tabControl1.SelectedTabIndex - 1] as PageDesc;
            if (!currentPageDesc.bCustomTab && currentPageDesc.pg != null) 
            {
                currentPageDesc.pg.Refresh();
            }

            m_bRecordChanged = false;

            if (m_ChangedProperty_tree != null)
            {
                RefreshTreeFindKey(m_ChangedProperty_tree as CustomProperty);
                m_ChangedProperty_tree = null;
            }
            
            this.Cursor = Cursors.Default;
        }

        public void ExecCommand(string strCmd, object data)
        {
            try
            {
                switch (strCmd.Trim().ToLower())
                {
                case "savedocument":
                    if ((!Program.m_bUseNewAcl || this.CanSaveDB) && !Program.m_bLockDBForCompetition)
                    {
                        if (m_bRecordChanged || CustomTabHasChanged())
                        {
                            //���ü�¼��ע
                            string strRemarks = "δ��ӱ�ע";
                            if (Program.m_bCommentOnSave)
                            {
                                InputBoxForm ibf = new InputBoxForm();
                                if (ibf.ShowDialog() == DialogResult.OK)
                                {
                                    strRemarks = ibf.Remarks;
                                }
                                else
                                {
                                    UnlockRecord();
                                    break;
                                }
                            }
                            Helper.AddLog(string.Format("{0}:���ڴ浵...", DateTime.Now.ToLongTimeString()));
                            SaveDocument(strRemarks);
                            Helper.AddLog(string.Format("{0}:�浵���", DateTime.Now.ToLongTimeString()));
                        }
                        
                    }
                    else
                    {
                        MessageBox.Show("�����ڵ�ǰ�༭��û��дȨ�ޣ������뿪ͨ��", "Ȩ�޲���", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                    break;
                case "findrecord":
                        {
                            if (m_bPluginTree || m_FindFrm != null)                    
                                return;
                            ArrayList arrExtraFindColumnField = new ArrayList();
                            DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;
                            if (dbClass.ExistLuaFunction("AddExtraFindColumnFields"))
                            {
                                object[] args = new object[1];
                                args[0] = arrExtraFindColumnField;
                                dbClass.CallLuaFunction("AddExtraFindColumnFields", args);
                            }

                            FindForm1 findfrm = new FindForm1(this, baseTree, TblMain, DisplayField, arrExtraFindColumnField);
                            m_FindFrm = findfrm;
                            findfrm.Show();

                        }                   
                    break;
                case "cmdexport":
                    break;
                case "cmdimport":
                    break;
                default:
                    break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        public void RefreshPropertyGrid()
        {
            foreach (PageDesc pd in m_Pages)
            {
                if (!pd.bCustomTab && pd.pg != null)
                    pd.pg.Refresh();
            }
        }
        private void RefreshAllDataInfo()
        {
            foreach (PageDesc pd in m_Pages)
            {
                pd.dbClass.ReloadCurrentInfo();
            }
        }
        private void OnAsynNotify(int nNotifyID, object param)
        {
            switch(nNotifyID)
            {
                case 1:
                    Helper.PostMessage(this.Handle, WM_REFRESHPG, IntPtr.Zero, "");
                    break;
                case 2:
                    //m_ChangedProperty = param;
                    m_lstChangedProperty.Add(param as CustomProperty);
                    Helper.PostMessage(this.Handle, WM_REFREPROP, IntPtr.Zero, "");
                    break;
                case 3:
                    m_ChangedProperty_tree = param;
                    //Helper.PostMessage(this.Handle, WM_REFREPRTREE, IntPtr.Zero, "");
                    break;
                case 4: //���¶�ȡ���е����ݣ�����Ԫ����
                    RefreshAllDataInfo();
                    goto case 1;
            }
        }
        public void ReloadProperties()
        {
            OnAsynNotify(1, null);
        }
       

        public void RegComDll(string strFullPathFileName)
        {
            //System.Diagnostics.Process.Start("regsvr32.exe", "-s " + strFullPathFileName);
            Helper.RegComDll(strFullPathFileName);
        }

        public void LoadTree()
        {
            m_lua["LuaCallBack"] = new LuaCallBack(baseTree);
            LoadTreeWithLua = true;
            DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;

            bool bUseOptimizedLoadTree = (this.MdiParent == Program.MainForm);
            //if (dbClass.ExistLuaFunction("UseOptimizedLoadTree"))
            //{
            //    object[] ret = dbClass.CallLuaFunction("UseOptimizedLoadTree");
            //    bUseOptimizedLoadTree = (bool)ret[0];
            //}
            if (dbClass.ExistLuaFunction("LoadTreePlugin"))
            {
                this.BaseTree.Nodes.Clear();
                this.BaseTree.Visible = false;
                this.BaseTree = null;

                object[] args = new object[7];
                args[0] = this.panelDockContainer1;
                TreeViewEventHandler afterSelect = new TreeViewEventHandler(this.baseTree_AfterSelect);
                args[1] = afterSelect;
                TreeViewCancelEventHandler beforeSelect = new TreeViewCancelEventHandler(this.baseTree_BeforeSelect);
                args[2] = beforeSelect;
                NodeLabelEditEventHandler beforeLabelEdit = new NodeLabelEditEventHandler(this.baseTree_BeforeLabelEdit);
                args[3] = beforeLabelEdit;
                NodeLabelEditEventHandler afterLabelEdit = new NodeLabelEditEventHandler(this.Tree_AfterLabelEdit);
                args[4] = afterLabelEdit;
                EventHandler extraMenuItemClick = new EventHandler(this.buttonExtraMenuItem_Click);
                args[5] = extraMenuItemClick;

                
                args[6] = Program.EnableRemoting;

                Helper.AddTimeLog("load tree plugin begin");

                object[] ret = dbClass.CallLuaFunction("LoadTreePlugin", args);

                Helper.AddTimeLog("load tree plugin end");

                LoadTreeWithLua = false;
                m_bPluginTree = true;

                // ��Щ�����ѷŵ�plugin��dll�
                //this.panelDockContainer1.Controls.Add((Control)(ret[0]));
                //PluginTree.Dock = DockStyle.Fill;
                //PluginTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.baseTree_AfterSelect);
                //PluginTree.Update();

                // ���صĵ�һ����treeview
                // ��Ҫ�õ�treeview�����ã���ʵʱ��panelDockContainer1.Controls�����ң���Ϊ��ʱ�����tree��
                TreeView PluginTree = (TreeView)(ret[0]);
                this.BaseTree = PluginTree;
            }
            else if (dbClass.ExistLuaFunction("LoadTreeAll"))
            {
                dbClass.CallLuaFunction("LoadTreeAll");
            }
            else
            {
                try
                {
                    if (dbClass.ExistLuaFunction("LoadTreePost"))
                    {
                        LoadTreeNoLua(false, bUseOptimizedLoadTree);
                        dbClass.CallLuaFunction("LoadTreePost");
                    }
                    else
                    {
                        if (dbClass.ExistLuaFunction("DisplayTreeNode")) // �ýű�������������ʾ(ֻ��һ��)
                        {   
                            object[] result = dbClass.CallLuaFunction("DisplayTreeNode");
                            if(result != null)
                            {
                                m_displayString = result[0] as string;
                                LoadTreeNoLua(m_displayString, true, bUseOptimizedLoadTree);
                                m_DisplayNode = true; // �ýű���ʾ������ı�(ֻ��һ��)
                            }
                            else
                            {
                                MessageBox.Show("�ű���DisplayTreeNode�����ķ���ֵ����Ϊ��!");
                            }
                        }
                        else if(dbClass.ExistLuaFunction("DisplayTreeNodePlus")) // �ýű�������������ʾ(ÿ�������仯����Ҫ��)
                        {
                            m_lua["DisplayField"] = null;
                            lua["DisplayRow"] = null; //���� DisplayTreeNodePlus �¼��ĵ�ǰ��ʾ������
                            object[] result = dbClass.CallLuaFunction("DisplayTreeNodePlus");
                            if(result != null)
                            {
                                m_displayField = result[0] as string;
                                m_displayString = "";
                                m_DisplayVarNode = true; // �ýű���ʾ������ı�(ÿ�������仯����Ҫ��)
                                LoadTreeNoLua(m_displayString, true, bUseOptimizedLoadTree);
                                m_DisplayNode = true;
                            }
                            else
                            {
                                MessageBox.Show("�ű���DisplayTreeNodePlus�����ķ���ֵ����Ϊ��!");
                            }
                        }
                        else
                        {
                            LoadTreeNoLua(true, bUseOptimizedLoadTree);
                            m_DisplayNode = false; // ���ýű���ʾ������ı�
                        }

                        LoadTreeWithLua = false;
                    }
                }
                catch (Exception ex)
                {
                    string err = string.Format("LoadTreeNoLuaʱ��������\r\n\r\n������ϢΪ��{0}", ex.Message);
                    MessageBox.Show(err);
                }
            }
           
            try
            {
                ModifyTreeCommand cmd = new ModifyTreeCommand(BaseTree);
                cmd.Execute();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }           
        }

        public void GetAll(TreeNodeCollection nodes, ref ArrayList ls, int iLevel)
        {
            foreach (TreeNode node in nodes)
            {
                if (node.Level == iLevel)
                {
                    ls.Add(node);
                }
                if (node.Nodes.Count > 0)
                {
                    GetAll(node.Nodes, ref ls, iLevel);
                }
            }
        }

        private void AddAllCatNodes(DataTable table)
        {
            int i = 0;
            if (CatHashes == null)
                return;
            
            foreach (Hashtable ht in CatHashes)
            {  
                ArrayList al_NodesInLevel = FindAllTreeNodesByLevel(i - 1);
                foreach (TreeNode node in al_NodesInLevel)
                {
                    if (node.Tag != null)
                    {
                        try
                        {
                            if(!(node.Tag is string))
                            {
                                object[] tags = (object[])node.Tag;
                                if (tags[0].ToString() == "-1")
                                    continue;
                            }
                        }
                        catch
                        {}
                    }

                    int newnodelevel = 0;
                    TreeNodeCollection tnc;
                    if (node.Name == "root")
                    {
                        tnc = this.baseTree.Nodes;
                        newnodelevel = 0;
                    }
                    else
                    {
                        tnc = node.Nodes;
                        newnodelevel = node.Level + 1;
                    }

                    ArrayList al_ht = new ArrayList(ht);
                    StringComparer comboComp = new StringComparer();
                    al_ht.Sort(comboComp);                   
                    foreach (DictionaryEntry de in al_ht)
                    {
                        bool fAlreadyExists = false;
                        TreeNode nFound = null;
                        
                        foreach (TreeNode n in tnc)
                        {
                            if (n.Tag.ToString() == de.Key.ToString())
                            {
                                fAlreadyExists = true;
                                nFound = n;
                                break;
                            }
                        }

                        if (!fAlreadyExists)
                        {
                            object[] ret = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("CustomCategory", new object[] { newnodelevel, de.Key.ToString(), de.Value.ToString(), table });
                            TreeNode newnode = (ret == null) ? tnc.Add(de.Value.ToString()) : tnc.Add(ret[0].ToString());
                            newnode.Tag = de.Key.ToString();
                        }
                        else
                        {
                            if (nFound.Text != de.Value.ToString())
                                nFound.Text = de.Value.ToString();
                        }
                    }

                }
                i++;          
                 
            }
            
        }

        private void LoadCatHashes()
        {            
            if (CatFields.Length == 1 && CatFields[0] == "")
                return;

            CatHashes = new Hashtable[CatFields.Length];

            int i = 0;
            foreach (string strField in CatFields)
            {
                string sql = string.Format("SELECT * FROM sys_meta_info WHERE tablename='{0}' AND fieldname='{1}' AND modtabid={2}", MainTableName, strField.Trim(), ((PageDesc)m_Pages[0]).id);
                //DataTable dt = Helper.GetDataTable(sql, Conn);

                string filter = string.Format("tablename='{0}' AND fieldname='{1}' AND modtabid={2}", 
                    MainTableName, strField.Trim(), ((PageDesc)m_Pages[0]).id);

                DataTable dt = Helper.GetDataTableProxy("sys_meta_info", filter, null, Conn);

                if (dt.Rows.Count < 1)
                {
                    string hint = string.Format("��Ԫ�༭��û�и÷����ֶ�: {0} �ļ�¼,�������Ľ�㽫ֱ����ʾ��ֵ.", strField);
                    //MessageBox.Show(hint);
                    CatHashes[i] = new Hashtable();
                    sql = string.Format("SELECT [{0}] FROM {1} GROUP BY [{2}]", strField, MainTableName, strField);
                    DataTable tbl = null;
                    try
                    {
                        //tbl = Helper.GetDataTable(sql, Conn);
                        tbl = Helper.GetDataTableWithSqlProxy(MainTableName, sql, Conn);
                    }
                    catch (Exception e)
                    {
                        string err = string.Format("���Խ��з���ʱ�������󡣷����ֶ�Ϊ��{0}��\r\n\r\n������ϢΪ��{1}", strField, e.Message);
                        MessageBox.Show(err);
                        return;
                    }
                    foreach (DataRow row in tbl.Rows)
                    {
                        if (row[0] != DBNull.Value)
                        {
                            string str = row[0].ToString();
                            CatHashes[i][str] = str;
                        }
                    }
                    i++;
                    continue;
                }

                DataRow r = dt.Rows[0];
                string editortype = r["editortype"].ToString().Trim();
                CatHashes[i] = new Hashtable(); 
               
                
                switch (editortype)
                {
                    case "customeditor":
                        string keyfield = r["keyfield"].ToString().Trim();
                        string listfield = r["listfield"].ToString().Trim();

                        string tblname = r["listtable"].ToString().Trim();
                        string strCond = "";
                        filter = string.Empty;

                        if (r["listcondition"].ToString().Trim() != "")
                        {
                            strCond = " WHERE " + r["listcondition"].ToString();
                            filter = r["listcondition"].ToString();
                        }

                        sql = string.Format("SELECT [{0}], [{1}] FROM {2}", keyfield, listfield, tblname) + strCond;
                        //DataTable tbl = Helper.GetDataTable(sql, Conn);                      
                        DataTable tbl = Helper.GetDataTableProxy(tblname, filter, null, Conn);

                        foreach (DataRow row in tbl.Rows)
                        {
                            CatHashes[i][row[keyfield].ToString()] = row[listfield].ToString();
                        }
                        break;

                    case "lookupcombo":
                        
                        tblname = r["listtable"].ToString().Trim();

                        keyfield = r["keyfield"].ToString().Trim();
                        listfield = r["listfield"].ToString().Trim(); 
                        strCond = "";
                        filter = string.Empty;

                        if (r["listcondition"].ToString().Trim() != "")
                        {
                            strCond = " WHERE " + r["listcondition"].ToString();
                            filter = r["listcondition"].ToString(); ;
                        }
                        sql = string.Format("SELECT [{0}], [{1}] FROM {2}", keyfield, listfield, tblname) + strCond;
                        
                        //tbl = Helper.GetDataTable(sql, Conn);
                        tbl = Helper.GetDataTableProxy(tblname, filter, null, Conn);
                       
                                               
                        foreach (DataRow row in tbl.Rows)
                        {

                            CatHashes[i][row[keyfield].ToString()] = row[listfield].ToString();
                        }
                        
                        break;

                    case "textcombo":
                        string listvalue = r["listvalues"].ToString().Trim();
                        string[] texts = listvalue.Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
                        foreach (string text in texts)
                        {
                            string[] s = text.Split(new char[] { ',' });
                            if (s != null && s.Length == 2)
                            {
                                CatHashes[i][s[0]] = s[1];
                            }
                            else
                            {
                                MessageBox.Show("�����listvalues�����޸ı�Ԫ���ݡ�");
                                throw new Exception("����� listvalues ��ʽ!");
                            }
                        }
                        break;

                    default:
                        sql = string.Format("SELECT [{0}] FROM {1} GROUP BY [{2}]", strField, MainTableName, strField);
                        //DataTable tbl_f = Helper.GetDataTable(sql, Conn);
                        DataTable tbl_f = Helper.GetDataTableWithSqlProxy(MainTableName, sql, Conn);

                        CatHashes[i] = new Hashtable();
                        foreach (DataRow row in tbl_f.Rows)
                        {
                            string sVal = row[0].ToString();
                            CatHashes[i][sVal] = sVal;
                        }
                        break;
                }
                
                i++;
            }
           
        }

        /// <summary>
        /// ���صķ�������������滻��������ʾֵ
        /// </summary>
        /// <param name="displayString">��ʾ���ַ���</param>
        /// <param name="bGenerateLeafNode">�Ƿ�����Ҷ���</param>
        private void LoadTreeNoLua(string displayString, bool bGenerateLeafNode, bool bUseOptimizedLoadTree)
        {
            // ������׼����������� [tag, name] �� Hashtable
            LoadCatHashes();
            
            // ������н��
            this.baseTree.Nodes.Clear();

            // ���hash��
            m_nodeTable.Clear();
            m_nodeDisplayValueTable.Clear();

            // ����Ĭ�Ͻ��
            bool bDefaultRecordExist = true;
            string sql = string.Format("SELECT [hasdefaultrow] FROM sys_table_cfg WHERE tablename='{0}'", MainTableName);

            string filter = string.Format("tablename='{0}'", MainTableName);

            //DataTable tbl_tbl_cfg = Helper.GetDataTable(sql, Conn);

            DataTable tbl_tbl_cfg = Helper.GetDataTableProxy("sys_table_cfg", filter, null, Conn);
            if (tbl_tbl_cfg.Rows.Count > 0)
            {
                DataRow row = tbl_tbl_cfg.Rows[0];
                if (row["hasdefaultrow"] != DBNull.Value)
                {
                    bDefaultRecordExist = Convert.ToBoolean(row["hasdefaultrow"]);
                }
            }

            if (bDefaultRecordExist)
            {
                TreeNode def_node = this.baseTree.Nodes.Add("<Ĭ��ֵ>");
                object[] keys = new object[TblMain.PrimaryKey.Length];
                for (int i = 0; i < keys.Length; i++)
                    keys[i] = "-1";
                def_node.Tag = keys;
            }
            //DataTable __table = Helper.GetDataTable("SELECT * FROM " + MainTableName, Conn);

            DataTable __table = Helper.GetDataTableProxy(MainTableName, Conn);
            
            if (!bUseOptimizedLoadTree)//��ʹ���Ż�
            {
                // ��������Ľ��
                AddAllCatNodes(__table);
                __table.Dispose();


                // ��Ҫ�����¼�Ľ����
                if (bGenerateLeafNode)
                {
                    string strSort = "";
                    if (!(CatFields.Length == 1 && CatFields[0] == ""))
                    {
                        strSort = " ORDER BY ";
                        foreach (string cat in CatFields)
                        {
                            strSort += cat;
                            strSort += ",";
                        }
                        strSort = strSort.Remove(strSort.Length - 1);
                    }
                    else
                    {
                        string strSordField = GetSordFields();
                        if (strSordField.Length > 0)
                        {
                            strSort = " ORDER BY ";
                            strSort += strSordField;
                        }
                    }

                    if (strSort == " ORDER BY ")
                        strSort = string.Empty;

                    sql = "SELECT * FROM " + MainTableName + strSort;
                    
                    if (strSort.Length == 0)
                    {
                        sql += " ORDER BY " + DisplayField;
                        strSort += " ORDER BY " + DisplayField;
                    }
                    else
                    {
                        string strSortDisplay = "," + DisplayField;
                        strSortDisplay = sql.IndexOf(strSortDisplay) == -1 ? strSortDisplay : string.Empty;
                        sql += strSortDisplay;
                        strSort += strSortDisplay;
                    }

                    string order = string.Empty;
                    if (strSort.Length > 0)
                    {
                        order = strSort.Substring(10);
                    }
                    //DataTable table = Helper.GetDataTable(sql, Conn);
                    DataTable table = Helper.GetDataTableProxy(MainTableName, null, order, Conn);

                    object[] primaryKeys = new object[table.PrimaryKey.Length]; // ����
                    for (int i = 0; i < primaryKeys.Length; i++)
                    {
                        primaryKeys[i] = "0";
                    }
                    
                    if (HasDefaultRow)
                        m_defaultRow = table.Rows.Find(primaryKeys); // Ĭ����

                    bool bNoMoreError = false;

                    foreach (DataRow row in table.Rows)
                    {
                        string nodeText = displayString; // ��������ʾֵ

                        // ����Ĭ��ֵ��ǰ����ӹ��ˣ����ﲻ�á�
                        bool isDefaultRow = true; // �����Ƿ���Ĭ����
                        string[] pKeys = new string[table.PrimaryKey.Length];
                        for (int i = 0; i < pKeys.Length; i++)
                        {
                            pKeys[i] = row[table.PrimaryKey[i]].ToString();
                            if (pKeys[i] != "0")
                            {
                                isDefaultRow = false;
                                break;
                            }
                        }

                        if (isDefaultRow) // ������Ĭ����
                        {
                            continue;
                        }

                        TreeNodeCollection nodes = this.baseTree.Nodes;
                        if (CatFields.Length == 1 && CatFields[0] == "")
                        {
                            if (m_DisplayVarNode) // �ýű�����ÿ�������Ĳ�ͬ��ʾ
                            {
                                if (table.Columns.Contains(m_displayField))
                                {
                                    lua["DisplayField"] = row[m_displayField].ToString();
                                    lua["DisplayRow"] = row;
                                    object[] result = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("DisplayTreeNodePlus");
                                    if (result != null)
                                    {
                                        nodeText = result[0] as string;
                                    }
                                    else
                                    {
                                        MessageBox.Show("�ű���DisplayTreeNodePlus�����ķ���ֵ����Ϊ��!");
                                    }
                                }
                            }
                            // ת����ʾ�Ľ���ı�
                            string regexPattern = @"\{(?<value>\w*)\}";
                            Regex r = new Regex(regexPattern);
                            MatchCollection matches = r.Matches(nodeText);
                            string[] results = new string[matches.Count];
                            for (int j = 0; j < matches.Count; j++)
                            {
                                results[j] = matches[j].Result("${value}");
                                if (row[results[j]] != null)
                                {
                                    string value = row[results[j]].ToString();
                                    if (bDefaultRecordExist && m_defaultRow != null) // ��Ĭ����
                                    {
                                        if (value == "")
                                        {
                                            value = m_defaultRow[results[j]].ToString();
                                        }
                                    }
                                    nodeText = nodeText.Replace("{" + results[j] + "}",
                                        value);
                                }
                            }

                            TreeNode newnode;
                            if (matches.Count > 0 || m_DisplayVarNode) // ��Ч����ʾ�ַ���
                            {
                                newnode = nodes.Add(nodeText);
                                m_nodeDisplayValueTable[newnode] = nodeText;
                            }
                            else
                            {
                                newnode = nodes.Add(row[DisplayField].ToString());
                            }
                            object[] keys = new object[TblMain.PrimaryKey.Length];
                            for (int i = 0; i < TblMain.PrimaryKey.Length; i++)
                                keys[i] = row[TblMain.PrimaryKey[i].ColumnName];
                            newnode.Tag = keys;
                            m_nodeTable[newnode] = row[DisplayField].ToString();
                        }
                        else
                        {
                            int i = 0;
                            foreach (string cat in CatFields)
                            {
                                string catvalue = string.Empty;
                                if (row[cat] == DBNull.Value)
                                {
                                    DataRow default_row = CustomClassCache.GetPropDefaultValue(MainTableName);
                                    if (default_row != null)
                                        catvalue = default_row[cat].ToString();
                                }
                                else
                                {
                                    catvalue = row[cat].ToString();
                                }

                                TreeNode node = null;
                                foreach (TreeNode eachnode in nodes)
                                {

                                    if (eachnode.Tag.ToString() == catvalue)
                                    {
                                        node = eachnode;
                                        break;
                                    }
                                }

                                if (node == null)
                                {
                                    if (!bNoMoreError)
                                    {
                                        string err = string.Format("��Ϊ�� {0} ����������ʱ��������û���ҵ�������Ӧ�ķ�����, ���ܸ�ģ�����ģ������������, �������ݱ���ĳ����¼�ķ����ֶε�ֵ����ȷ(����Ϊ��)��\r\n\r\nĳЩ��¼��������ʾ��\r\n\r\nҪ������ʾ����ʾ, ѡ�� < ��(Y) >.", row[TblMain.PrimaryKey[0].ColumnName]);
                                        DialogResult result = MessageBox.Show(err, "Warning", MessageBoxButtons.YesNo);
                                        if (result == DialogResult.Yes)
                                        {
                                            bNoMoreError = true;
                                        }
                                    }

                                    try
                                    {
                                        Hashtable ht = CatHashes[i];
                                        object field = row[cat];
                                        string name = string.Empty;
                                        if (ht[field] != null)
                                        {
                                            name = ht[field].ToString();
                                            TreeNode newnode = nodes.Add(name);
                                            newnode.Tag = row[cat].ToString();
                                            node = newnode;
                                        }
                                        else
                                        {
                                            continue;
                                        }
                                    }
                                    catch
                                    {
                                        continue;
                                    }
                                }
                                nodes = node.Nodes;
                                i++;

                                if (i == CatFields.Length)
                                {
                                    if (m_DisplayVarNode) // �ýű�����ÿ�������Ĳ�ͬ��ʾ
                                    {
                                        if (table.Columns.Contains(m_displayField))
                                        {
                                            lua["DisplayField"] = row[m_displayField].ToString();
                                            lua["DisplayRow"] = row;
                                            object[] result = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("DisplayTreeNodePlus");
                                            if (result != null)
                                            {
                                                nodeText = result[0] as string;
                                            }
                                            else
                                            {
                                                MessageBox.Show("�ű���DisplayTreeNodePlus�����ķ���ֵ����Ϊ��!");
                                            }
                                        }
                                    }

                                    // ת����ʾ�Ľ���ı�
                                    string regexPattern = @"\{(?<value>\w*)\}";
                                    Regex r = new Regex(regexPattern);
                                    MatchCollection matches = r.Matches(nodeText);
                                    string[] results = new string[matches.Count];
                                    for (int j = 0; j < matches.Count; j++)
                                    {
                                        results[j] = matches[j].Result("${value}");
                                        if (row[results[j]] != null)
                                        {
                                            string value = row[results[j]].ToString();
                                            if (bDefaultRecordExist && m_defaultRow != null) // ��Ĭ����
                                            {
                                                if (value == "")
                                                {
                                                    value = m_defaultRow[results[j]].ToString();
                                                }
                                            }
                                            nodeText = nodeText.Replace("{" + results[j] + "}",
                                                value);
                                        }
                                    }

                                    TreeNode newnode;
                                    if (matches.Count > 0) // ��ʾ�ַ�����Ч
                                    {
                                        newnode = nodes.Add(nodeText);
                                        m_nodeDisplayValueTable[newnode] = nodeText;
                                    }
                                    else
                                    {
                                        newnode = nodes.Add(row[DisplayField].ToString());
                                    }
                                    object[] keys = new object[TblMain.PrimaryKey.Length];
                                    for (int j = 0; j < TblMain.PrimaryKey.Length; j++)
                                        keys[j] = row[TblMain.PrimaryKey[j].ColumnName];
                                    newnode.Tag = keys;
                                    m_nodeTable[newnode] = row[DisplayField].ToString();
                                }

                            }
                        }
                    }

                    // ɾ����������¼���ķ�����
                    bool bNeedDeleteCat = true;
                    object[] resultCat = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("NeedDeleteCat");
                    if (resultCat != null)
                    {
                        bNeedDeleteCat = (bool)resultCat[0];
                    }
                    if (bNeedDeleteCat)
                    {
                        // ɾ����������¼���ķ�����
                        DeleteEmptyCat();
                    }
                }
            }//if (!bUseOptimizedLoadTree)
            else
            {
                if (bGenerateLeafNode)
                {
                    InitCatDisplayText(__table);
                    AddChildNodes(baseTree.Nodes, 0, null);

                }
                else
                {
                    AddAllCatNodes(__table);
                }
                __table.Dispose();
            }
            
                           
        }

        private string GetSordFields()
        {
            string strSort = "";
            //���û��ģ��ֲ��ֶζ��壬�Ϳ����Զ�����ʾ˳�������ֶ�
            bool bExits = ((PageDesc)m_Pages[0]).dbClass.ExistLuaFunction("OnGetSortField");
            if (bExits)
            {
                object[] results = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("OnGetSortField");
                strSort = results[0].ToString();
            }
            return strSort;
        }

        // c# �淶��loadtree������ʹ�����������ģ�������дloadtree��lua�ű����������޷�����
        private void LoadTreeNoLua(bool bGenerateLeafNode, bool bUseOptimizedLoadTree)
        {
            // ������׼����������� [tag, name] �� Hashtable
            LoadCatHashes();           
            // ������н��
            this.baseTree.Nodes.Clear();

            // ����Ĭ�Ͻ��
            bool bDefaultRecordExist = true;
           
            
            string sql = string.Format("SELECT * FROM sys_table_cfg WHERE tablename='{0}'", MainTableName);
            /* remoting
            DataTable tbl_tbl_cfg = Helper.GetDataTable(sql, Conn);
            */

            string filter = string.Format("tablename = '{0}'", MainTableName);
            DataTable tbl_tbl_cfg = Helper.GetDataTableProxy("sys_table_cfg", filter, null, Conn);

          
            if (tbl_tbl_cfg.Rows.Count > 0)
            {
                DataRow row = tbl_tbl_cfg.Rows[0];
                if (row["hasdefaultrow"] != DBNull.Value)
                {
                    bDefaultRecordExist = Convert.ToBoolean(row["hasdefaultrow"]);
                }
            }
            
            if (bDefaultRecordExist)
            {
                TreeNode def_node = this.baseTree.Nodes.Add("<Ĭ��ֵ>");
                object[] keys = new object[TblMain.PrimaryKey.Length];
                for (int i = 0; i < keys.Length; i++)
                    keys[i] = "-1";
                def_node.Tag = keys;
            }
            if (!bUseOptimizedLoadTree)//��ʹ���Ż�
            {

                // ��������Ľ��
                AddAllCatNodes(null);

                // ��Ҫ�����¼�Ľ����
                if (bGenerateLeafNode)
                {
                    string strSort = "";
                    if (!(CatFields.Length == 1 && CatFields[0] == ""))
                    {
                        strSort = " ORDER BY ";
                        foreach (string cat in CatFields)
                        {
                            strSort += "[" + cat + "]";
                            strSort += ",";
                        }
                        strSort = strSort.Remove(strSort.Length - 1);
                    }
                    else
                    {
                        string strSordField = GetSordFields();
                        if (strSordField.Length > 0)
                        {
                            strSort = " ORDER BY ";
                            strSort += strSordField;
                        }
                    }
                    
                    sql = "SELECT * FROM " + MainTableName + strSort;
                    
                    if (strSort.Length == 0)
                    {
                        
                        sql += " ORDER BY [" + DisplayField + "]";
                       
                        strSort = string.Format("[{0}]", DisplayField);
                    }
                    else
                    {
                        bool DisplayFieldIsCat = false;
                        foreach (string cat in CatFields)
                        {
                            if (DisplayField == cat)
                            {
                                DisplayFieldIsCat = true;
                                break;
                            }
                        }

                        bool bExtraSortField = ((PageDesc)m_Pages[0]).dbClass.ExistLuaFunction("LoadTreeSortField");
                        if (bExtraSortField)
                        {
                            object[] results = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("LoadTreeSortField");
                            string extraField = results[0].ToString();

                            string strSortDisplay = ",[" + extraField + "]";
                            strSortDisplay = sql.IndexOf(strSortDisplay) == -1 ? strSortDisplay : string.Empty;
                            
                            sql += strSortDisplay;
                            strSort += strSortDisplay;
                        }
                        else if (!DisplayFieldIsCat)
                        {
                            string strSortDisplay = ",[" + DisplayField + "]";
                            strSortDisplay = sql.IndexOf(strSortDisplay) == -1 ? strSortDisplay : string.Empty;
                            sql += strSortDisplay;
                            strSort += strSortDisplay;
                        }
                    }
                    
                    strSort = strSort.ToLower().Trim();
                    if (strSort.StartsWith("order"))
                    {
                        strSort = strSort.Substring(8);
                    }
                    //DataTable table = Helper.GetDataTable(sql, Conn);
                    DataTable table = Helper.GetDataTableProxy(MainTableName, null, strSort, Conn);

                    bool bNoMoreError = false;


                    foreach (DataRow row in table.Rows)
                    {

                        // ����Ĭ��ֵ��ǰ����ӹ��ˣ����ﲻ�á�
                        //if (Convert.ToInt32(row[TblMain.PrimaryKey[0].ColumnName]) == 0)
                        if (row[TblMain.PrimaryKey[0].ColumnName].ToString() == "0")
                            if (!(CatFieldsContainsKey && TblMain.PrimaryKey.Length > 1)) // ������������ҷ����ֶ��а�����������<Ĭ��ֵ>��㱻�����ˣ�������ζ�Ҫ������еĽ�㡣
                                continue;

                        TreeNodeCollection nodes = this.baseTree.Nodes;
                        if (CatFields.Length == 1 && CatFields[0] == "")
                        {
                            TreeNode newnode = nodes.Add(row[DisplayField].ToString());

                            object[] keys = new object[TblMain.PrimaryKey.Length];
                            for (int i = 0; i < TblMain.PrimaryKey.Length; i++)
                                keys[i] = row[TblMain.PrimaryKey[i].ColumnName];
                            newnode.Tag = keys;
                        }
                        else
                        {
                            int i = 0;
                            foreach (string cat in CatFields)
                            {
                                string catvalue = string.Empty;
                                if (row[cat] == DBNull.Value)
                                {
                                    DataRow default_row = CustomClassCache.GetPropDefaultValue(MainTableName);
                                    if (default_row != null)
                                        catvalue = default_row[cat].ToString();
                                }
                                else
                                {
                                    catvalue = row[cat].ToString();
                                }

                                TreeNode node = null;
                                foreach (TreeNode eachnode in nodes)
                                {

                                    if (eachnode.Tag.ToString() == catvalue)
                                    {
                                        node = eachnode;
                                        break;
                                    }
                                }

                                if (node == null)
                                {
                                    if (!bNoMoreError)
                                    {
                                        string err = string.Format("��Ϊ�� {0} ����������ʱ��������û���ҵ�������Ӧ�ķ�����, ���ܸ�ģ�����ģ������������, �������ݱ���ĳ����¼�ķ����ֶε�ֵ����ȷ(����Ϊ��)��\r\n\r\nĳЩ��¼��������ʾ��\r\n\r\nҪ������ʾ����ʾ, ѡ�� < ��(Y) >.", row[TblMain.PrimaryKey[0].ColumnName]);
                                        DialogResult result = MessageBox.Show(err, "Warning", MessageBoxButtons.YesNo);
                                        if (result == DialogResult.Yes)
                                        {
                                            bNoMoreError = true;
                                        }
                                    }

                                    try
                                    {
                                        Hashtable ht = CatHashes[i];
                                        object field = row[cat];
                                        string name = ht[field].ToString();
                                        TreeNode newnode = nodes.Add(name);
                                        newnode.Tag = row[cat].ToString();
                                        node = newnode;
                                    }
                                    catch
                                    {
                                        continue;
                                    }
                                }
                                nodes = node.Nodes;
                                i++;

                                if (i == CatFields.Length)
                                {
                                    TreeNode newnode = nodes.Add(row[DisplayField].ToString());

                                    object[] keys = new object[TblMain.PrimaryKey.Length];
                                    for (int j = 0; j < TblMain.PrimaryKey.Length; j++)
                                        keys[j] = row[TblMain.PrimaryKey[j].ColumnName];
                                    newnode.Tag = keys;
                                }
                            }
                        }

                    }

                    // ɾ����������¼���ķ�����
                    DeleteEmptyCat();
                }
            }
            else//ʹ���Ż��������ڵ�
            {
                if (bGenerateLeafNode)
                {
                    InitCatDisplayText(null);
                    AddChildNodes(baseTree.Nodes, 0, null);
                }
                else
                {
                    AddAllCatNodes(null);
                }

            }
        }

        private void DeleteEmptyCat()
        {
            if (CatHashes == null)
                return;

            int RealNodeLevel = CatHashes.Length;

            ArrayList toDelNodes = new ArrayList();
            foreach (TreeNode node in BaseTree.Nodes)
            {
                if (node.Text != "<Ĭ��ֵ>")
                    DeleteEmptyCat(node, RealNodeLevel, ref toDelNodes);
            }

            foreach (TreeNode node in toDelNodes)
            {
                node.Remove();
            }
        }

        private void DeleteEmptyCat(TreeNode father, int RealNodeLevel, ref ArrayList toDelNodes)
        {
            if (father.Level == RealNodeLevel - 1)
            {
                if (father.Nodes.Count == 0)
                    toDelNodes.Add(father);
            }
            else
            {
                foreach (TreeNode n in father.Nodes)
                {
                    DeleteEmptyCat(n, RealNodeLevel, ref toDelNodes);
                }
            }

            bool allSonToDelete = true;
            foreach (TreeNode n in father.Nodes)
            {
                if (toDelNodes.IndexOf(n) == -1)
                {
                    allSonToDelete = false;
                    break;
                }
            }
            if (allSonToDelete)
            {
                toDelNodes.Add(father);
            }
        }

        private void Form_Load(object sender, EventArgs e)
        {
            Helper.AddTimeLog("form load begin");

            if (TblMain.PrimaryKey.Length == 0)
            {
                string err = string.Format("��ģ�������û����������, ��ʹ�����ݱ�������.");
                MessageBox.Show(err);
                this.Close();
                return;
            }

            InitLua();
            CustomClassCache.DelAllScriptStore(); //ģ������ʱ,������еĽű�����

            foreach (PageDesc page in m_Pages)
                page.dbClass.DoScript(null);

            LoadTree();

            CustomTabInitial();

            BaseForm.CurPg = ((PageDesc)Pages[0]).pg;
        }

        private void InitPgHelpers()
        {
            foreach (PageDesc page in m_Pages)
            {
                page.dbClass = new DBCustomClass(null, m_lua, page.id, -1);
                page.Properties = page.dbClass;
                page.dbClass.OnAsynNotify += new DBCustomClass.AsynNotifyHandler(OnAsynNotify);
            }
        }

        private object selectObject(DBCustomClass dbClass, object[] tags)
        {
            CustomClass selectedObj = dbClass.ReloadDBData(tags);

            return selectedObj;
        }

        private void applyRestriction() // ��ӱ༭�����ڱ���֮�������
        {
            ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("onaftertreenodeclick");
        }

        private bool CustomTabHasChanged()
        {
            bool CustamTabContentsChanged = false;
            foreach (PageDesc pd in m_Pages)
            {
                if (pd.bCustomTab)
                {
                    object[] ret = pd.dbClass.CallLuaFunction("CustomTabHasChanged");
                    bool bHasChanged = false;
                    if (ret != null)
                    {
                        bHasChanged = Convert.ToBoolean(ret[0]);
                    }
                    if (bHasChanged)
                    {
                        CustamTabContentsChanged = true;
                        break;
                    }
                }
            }
            return CustamTabContentsChanged;
        }
        private void CustomTabSave()
        {
            foreach (PageDesc pd in m_Pages)
            {
                // if (pd.bCustomTab && pd.dbClass.ValueChanged) // ֵ�ı��ʱ�����Ҫ���ýű� add by suntao
                //if (pd.bCustomTab)���ȥ��������Tab�ڱ���ʱ������
                //{
                    object[] ret = pd.dbClass.CallLuaFunction("CustomTabSave");                 
                //}
            }
        }

        private void CustomTabReload(TreeNode node)
        {
            foreach (PageDesc pd in m_Pages)
            {
                if (pd.bCustomTab)
                {
                    object[] ret = pd.dbClass.CallLuaFunction("CustomTabReload", new object[] { node.Tag });
                }
            }
        }

        private void CustomTabInitial()
        {
            Helper.AddTimeLog("CustomTabInitial begin");
            int pd_index = 0;
            foreach (PageDesc pd in m_Pages)
            {
                pd_index++;
                if (pd.bCustomTab)
                {
                    
                    object[] ret = pd.dbClass.CallLuaFunction("CustomTabInit", new object[] { Conn, MainTableName });
                    this.tabControl1.Tabs[pd_index].AttachedControl.Controls.Clear();
                    if (ret != null)
                        this.tabControl1.Tabs[pd_index].AttachedControl.Controls.Add((Control)(ret[0]));
                }
            }
            Helper.AddTimeLog("CustomTabInitial end");
        }

        public void SelectTreeNode(TreeNode node, bool apply_restrict)
        {
            foreach (PageDesc pd in m_Pages)
            {
                if (pd.pg != null)
                {
                    pd.pg.SelectedObject = null;
                }
            }

            // �Ƿ���default���
            bool isDefaultNode = false;
            object[] keys = node.Tag as object[];

            if (keys != null)
            {
                isDefaultNode = (keys[0].ToString() == "-1");
            }

            if (isDefaultNode)
            {
                object[] sks = new object[TblMain.PrimaryKey.Length];

                for (int i = 0; i < TblMain.PrimaryKey.Length; i++)
                {
                    sks[i] = 0;
                }
                
                foreach (PageDesc pd in m_Pages)
                {
                    pd.qc = new queryCommand(pd.dbClass, sks);
                    pd.qc.m_syncList.Clear();
                }                
            }
            else
            {
                DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;

                if (dbClass.ExistLuaFunction("LoadTreePlugin")) // ���ɿؼ�����, 
                {
                    //if (dbClass.ExistLuaFunction("onselectedtreenode"))
                    //    dbClass.CallLuaFunction("onselectedtreenode");

                    foreach (PageDesc pd in m_Pages)
                    {
                        if (pd.pg != null)
                        {
                            pd.qc = new queryCommand(pd.dbClass, node.Tag as object[]);
                        }                        
                    }
                }
                else if (dbClass.ExistLuaFunction("TreeNodeClick"))
                {
                    m_lua["ClickNode"] = node;
                    object[] tags = dbClass.CallLuaFunction("TreeNodeClick");

                    if (tags != null)
                    {
                        foreach (PageDesc pd in m_Pages)
                        {
                            if (pd.pg == null)
                            {
                                continue;
                            }
                                
                            pd.qc = new queryCommand(pd.dbClass, (object[])tags[0]);                                

                            if (apply_restrict && pd.pg.SelectedObject != null)
                            {                                
                                dbClass.CallLuaFunction("onaftertreenodeclick"); // ����ʱ�Ѿ����пռ����                                
                            }
                        }
                    }
                }
                else // ��luaָ��,�Լ��淶���Ұ�.
                {
                    // ����Ĭ�Ͻ��
                    // �з����ֶ�
                    // �Ƿ�����
                    if (!isDefaultNode && !(CatFields.Length == 1 && CatFields[0] == "") && (node.Level != CatFields.Length)) 
                    {
                        return;
                    }

                    if (node.Level == CatFields.Length || (CatFields.Length == 1 && CatFields[0] == "" && node.Level == CatFields.Length - 1) || isDefaultNode)
                    {
                        foreach (PageDesc pd in m_Pages)
                        {
                            if (pd.pg != null)
                            {
                                pd.qc = new queryCommand(pd.dbClass, node.Tag as object[]);
                            }                                                                
                        }
                    }
                }
            }

            //add by cuiwei 07.8.23
            TabControlPanel tp = tabControl1.SelectedTab.AttachedControl as TabControlPanel;
            m_lua["TreeNodeTag"] = BaseTree.SelectedNode.Tag;

            if (tabControl1.SelectedTab != null && tabControl1.SelectedTab.Tag != null)
            {
                PageDesc pageDesc = m_Pages[tabControl1.SelectedTabIndex - 1] as PageDesc;
                if (pageDesc.pg != null && pageDesc.pg.SelectedObject == null)
                {
                    queryCommand qc = pageDesc.qc;
                    if (qc != null)
                        pageDesc.pg.SelectedObject = selectObject(qc.dbClass, qc.Args);
                }
                
            }
            
            CustomTabReload(node);            
            //add end
        }


        private void TreeNodeClick(object sender, TreeNodeMouseClickEventArgs e)
        {
           
            if (e.Button == MouseButtons.Right && e.Node != null)
            {
                this.baseTree.SelectedNode = e.Node;

                // �ű����Դ������Ŀ���ճ��
                bool bCanExtraCopyPaste = false;
                DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;
                if (dbClass.ExistLuaFunction("HandleExtraCopyPaste"))
                {
                    object[] results = dbClass.CallLuaFunction("HandleExtraCopyPaste", new object[] { e.Node.Level });
                    bCanExtraCopyPaste = Convert.ToBoolean(results[0]);
                }

                // �Ƿ���Ը���
                bool bNoRecordSelected = true;
                foreach (PageDesc pd in m_Pages)
                {
                    if (pd.pg != null && pd.pg.SelectedObject != null)
                    {
                        bNoRecordSelected = false;
                        break;
                    }
                }

                if (bNoRecordSelected) // û�е��ڼ�¼�Ľ����
                {
                    buttonCopy.Enabled = bCanExtraCopyPaste;
                }
                else
                {
                    buttonCopy.Enabled = true;
                }

                // �Ƿ����ճ��
                bool bCanPaste = bCanExtraCopyPaste;
                if (CopyFromNode != null && (e.Node.Level == CopyFromNode.Level - 1 || e.Node.Level == CopyFromNode.Level))
                    bCanPaste = true;
                buttonPaste.Enabled = bCanPaste;

                for (int i = 0; i < m_ExtraMenuItem.Length; i++)
                    m_ExtraMenuItem[i].Enabled = true;

                // ��û�и�ģ���дȨ�ޣ����δ󲿷���������
                if (Program.m_bUseNewAcl && !this.CanSaveDB)
                {
                    buttonAddSameLevel.Enabled = false;
                    buttonAddSub.Enabled = false;
                    buttonRename.Enabled = false;
                    buttonCopy.Enabled = false;
                    buttonPaste.Enabled = false;
                    buttonDelete.Enabled = false;

                    for (int i = 0; i < m_ExtraMenuItem.Length; i++)
                        m_ExtraMenuItem[i].Enabled = false;
                }

                // �����Ҽ��˵�
                m_poupupNode = e.Node;
                cmNodeRightClick.Popup(Control.MousePosition);
            }
        }

        private bool Updatedata(string sql,DataTable table,UpdateType type)
        {
            try
            {
                SqlDataAdapter adapter = new SqlDataAdapter(sql, MainForm.conn);
                //ʹ��SqlCommandBuilder  �������SqlDataAdapter ��InsertCommand��DeleteCommand��UpdateCommand����
                SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adapter);
                switch(type)
                {
                    case UpdateType.ADD: adapter.InsertCommand = cmdBuilder.GetInsertCommand(); break;
                    case UpdateType.MOD: adapter.UpdateCommand = cmdBuilder.GetUpdateCommand(); break;
                    case UpdateType.DEL: adapter.DeleteCommand = cmdBuilder.GetDeleteCommand(); break;
                }
                
                int val = adapter.Update(table);
                table.AcceptChanges();
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return false;
            }
        }

        private void cmLoadTreeScript_Click(object sender, EventArgs e)
        {
            string strFileFullName = Application.StartupPath + "/LoadTree.lua";
            ScriptCodeForm frm = ((MainForm)ParentForm).CodeForm;
            //frm.CurCode = FileFolderHelper.FileToString(strFileFullName);//frm.OutputBox.Text = FileFolderHelper.FileToString(strFileFullName);
            //modify by cuiwei 07.9.12
            //frm.ShowDialog();
            //showScriptCodeForm(frm,0,0);
            frm.Show(strFileFullName, strFileFullName);
            //modify end
            //if (frm.DialogResult == DialogResult.OK)
            //    FileFolderHelper.StringToFile(frm.lua_OutputBox.Text, strFileFullName);
        }

        private void doEditScript()
        {
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
            GridItem item = (page.pg == null) ? null : page.pg.SelectedGridItem;
            CustomPropertyDescriptor desc = item == null ? null : (CustomPropertyDescriptor)item.PropertyDescriptor;
            string strcode = page.dbClass.GetScriptCode(desc);
            ScriptCodeForm frm = Program.MainForm.CodeForm;
            //frm.CurCode = strcode; // frm.OutputBox.Text = strcode;
            int nFieldId = 0;
            if (desc != null)
                nFieldId = Convert.ToInt32(desc.Property.Key);
            int nModTabID = 0;
            if (tabControl1.SelectedTab != null && tabControl1.SelectedTab.Tag != null)
            {
                if (!page.bCustomTab && tabControl1.SelectedTab.Tag is PropertyGrid)
                {
                    PropertyGrid pg = (PropertyGrid)(tabControl1.SelectedTab.Tag);
                    if (pg != null && pg.Tag != null)
                        nModTabID = (int)pg.Tag;
                }
                else if (page.bCustomTab && tabControl1.SelectedTab.Tag is int)
                {
                    nModTabID = (int)(tabControl1.SelectedTab.Tag);
                }
                //                 PropertyGrid pg = (tabControl1.SelectedTab.Tag is PropertyGrid) ? (PropertyGrid)(tabControl1.SelectedTab.Tag) : null;
                //                 if (pg != null && pg.Tag != null)
                //                     nModTabID = (int)pg.Tag;
            }
            else
                return;

            string logcomment = string.Format("PropertyName={0}, FieldID={1}", desc == null ? "#" : desc.Property.Name, nFieldId);
            logscript.NewLine();
            logscript.Log(TimeLog.enumLogType.ltstart, this.ModelName, "�����Խű�����", logcomment);
            try
            {
                Hashtable tag = new Hashtable();
                tag["logscript"] = logscript;
//                 tag["open_1"] = TimeLog.enumLogType.ltstart;
//                 tag["open_2"] = this.ModelName;
//                 tag["open_3"] = "�����Խű�����";
//                 tag["open_4"] = logcomment;
                tag["modify_1"] = TimeLog.enumLogType.ltend;
                tag["modify_2"] = null;
                tag["modify_3"] = null;
                tag["modify_4"] = logcomment + " �ű�������!";
                showScriptCodeForm(frm, nFieldId, nModTabID, tag);

#if false
                frm.LoadHisCode(nModTabID, nFieldId);//m_dbclass.MainTable, strfnID);
                //modify by cuiwei 07.9.12
                //frm.ShowDialog();
                showScriptCodeForm(frm);
                //modify end
                if (frm.DialogResult == DialogResult.OK)
                {
                    //m_scriptExecState[strfnID] = false;
                    bool bSameCode = true;
                    if (!(strcode == null && frm.lua_OutputBox.Text.Length == 0)
                        && (strcode == null
                            || strcode.ToString() != frm.lua_OutputBox.Text)
                        )
                    {
                        bSameCode = false;
                        page.dbClass.SetScriptCode(desc == null ? null : desc.Property, frm.lua_OutputBox.Text);
                    }
                    page.dbClass.DoScript(desc == null ? null : desc.Property);
                    //�ϴ������ݿ�
                    if (!bSameCode)
                    {
                        page.dbClass.SaveScript(desc == null ? null : desc.Property);
                        CustomClassCache.DelAllScriptStore();
                        logscript.Log(TimeLog.enumLogType.ltend, null, null, logcomment + " �ű�������!");
                    }
                }
#endif
            }
            catch/*(Exception ex)*/
            {
                logscript.Log(TimeLog.enumLogType.ltend, null, null, null);
            }
        }

        //add by cuiwei 07.9.12
        private void showScriptCodeForm(ScriptCodeForm frm, int fieldid, int modtabid, object tag)
        {
            foreach (Control t in this.Controls)
                t.Enabled = !t.Enabled;
            frm.m_lua = this.m_lua;
            try
            {
                frm.Show(fieldid, modtabid, tag);
            }
            catch (Exception ex)
            {
                Helper.AddLog(ex.Message);
            }
            foreach (Control t in this.Controls)
                t.Enabled = !t.Enabled;
        }
        //add end

        private void buttonShowLuaCode_Click(object sender, EventArgs e)
        {
            try
            {
                ModifyScriptCommand cmd = new ModifyScriptCommand();
                cmd.Execute();
                doEditScript();
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        public void UpdateCatMetaInfo(int addinglevel, ref TreeNode newNode)
        {
            DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;

            // ����metainfo����
            string sql = string.Format("SELECT * FROM sys_meta_info WHERE tablename='{0}' AND fieldname='{1}'", MainTableName, CatFields[addinglevel]);
            /* remoting
            DataTable tblMetaInfo = Helper.GetDataTable(sql, Conn);
            */
            string filter = string.Format("tablename='{0}' AND fieldname='{1}'", MainTableName, CatFields[addinglevel]);
            DataTable tblMetaInfo = Helper.GetDataTableProxy("sys_meta_info", filter, null, Conn);

            DataRow row = tblMetaInfo.Rows[0];
            int newTag = -1;
            switch (row["editortype"].ToString().Trim())
            {
                case "lookupcombo":
                    {
                        string sql_lookup = "SELECT * FROM " + row["listtable"].ToString().Trim();
                        /* remoting
                        DataTable tbl_lookup = Helper.GetDataTable("SELECT * FROM " + row["listtable"].ToString().Trim(), Conn);
                        */

                        DataTable tbl_lookup = Helper.GetDataTableProxy(row["listtable"].ToString().Trim(), Conn);

                        object[] zero_keys = new object[tbl_lookup.PrimaryKey.Length];
                        for (int i = 0; i < zero_keys.Length; i++)
                        {
                            zero_keys[i] = 0;
                        }
                        DataRow defaultRow = tbl_lookup.Rows.Find(zero_keys);
                        if (defaultRow == null)
                            defaultRow = tbl_lookup.Rows[0];
                        DataRow newRow = tbl_lookup.NewRow();
                        newRow.BeginEdit();
                        foreach (DataColumn col in tbl_lookup.Columns)
                            newRow[col] = defaultRow[col];
                        newRow[tbl_lookup.PrimaryKey[0]] = tbl_lookup.Rows.Count;
                        newRow[row["listfield"].ToString().Trim()] = "�½�";
                        newRow.EndEdit();
                        tbl_lookup.Rows.Add(newRow);
                        /* remoting
                        SqlDataAdapter adp = new SqlDataAdapter("SELECT * FROM " + row["listtable"].ToString().Trim(), m_conn);
                        SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                        adp.InsertCommand = cmdBuilder.GetInsertCommand();
                        int val = adp.Update(tbl_lookup);
                         
                        tbl_lookup.AcceptChanges();
                         */
                        int val = Helper.SaveTable(tbl_lookup, sql_lookup, Conn);


                        newTag = Convert.ToInt32(newRow[tbl_lookup.PrimaryKey[0]]);
                    }
                    break;

                case "textcombo":
                    {
                        string listvalues = row["listvalues"].ToString().Trim();
                        string[] lines = listvalues.Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
                        // �ҵ���δʹ�õ�listvalues��key
                        int iTag = 0;
                        foreach (string str in lines)
                        {
                            string[] s = str.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                            if (s.Length != 2)
                            {
                                MessageBox.Show("listvalues����, �����Ԫ����.");
                                return;
                            }
                            if (s[0].Trim() != iTag.ToString())
                            {
                                break;
                            }
                            iTag++;
                        }

                        row["listvalues"] = listvalues + "\r\n" + iTag.ToString() + ",�½�";

                        /* remoting
                        SqlDataAdapter adp = new SqlDataAdapter(sql, m_conn);

                        SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                        adp.UpdateCommand = cmdBuilder.GetUpdateCommand();
                        int val = adp.Update(tblMetaInfo);
                        tblMetaInfo.AcceptChanges();
                        */

                        int val = Helper.SaveTable(tblMetaInfo, sql, m_conn);

                        newTag = iTag;
                    }
                    break;

                default:
                    break;
            }

            // ��ӱ����
            TreeNode nodeWhereToAdd;
            //if (this.baseTree.SelectedNode.Level == addinglevel)
            if (m_poupupNode.Level == addinglevel)
                nodeWhereToAdd = m_poupupNode.Parent;//this.baseTree.SelectedNode.Parent;
            else
                nodeWhereToAdd = m_poupupNode;//this.baseTree.SelectedNode;
            if (nodeWhereToAdd == null)
                newNode = this.baseTree.Nodes.Add("�½�");
            else
            {
                newNode = nodeWhereToAdd.Nodes.Add("�½�");
            }
            if (newTag == -1)
                newNode.Tag = newNode.Text;
            else
                newNode.Tag = newTag;

            if (dbClass.ExistLuaFunction("AdjustCatTag"))
            {
                object[] results = dbClass.CallLuaFunction("AdjustCatTag", new object[] { addinglevel });
                newNode.Tag = results[0];
                newNode.Text = results[1].ToString().Trim();
            }

            // ����ҪΪ�¼ӽ����ӷ�����ӽڵ���?
            if (!CatFieldsContainsKey)
            {
                bool bCleanCat = false;
                if (dbClass.ExistLuaFunction("IsCleanCat")) // ������Ҫ��ģ���ͨ������������Ʒ������Ƿ����ɡ�
                {
                    object[] results = dbClass.CallLuaFunction("IsCleanCat");
                    bCleanCat = (bool)results[0];
                }

                if (!bCleanCat)
                {
                    LoadCatHashes();
                    AddAllCatNodes(null);
                }
            }
        }

        ////////////////////////////
        // ���˵�����Ӧ����
        ////////////////////////////
        private void addNode_Click(int addinglevel)
        {
            TreeNode newNode = new TreeNode();
            DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;

            bool bNoCatActually = false;
            if (CatFields.Length == 1 && CatFields[0] == "")
                bNoCatActually = true;

            bool bNeedToRunPostScript = false;
            object[] keys = null;

            // �ű�����
            if (dbClass.ExistLuaFunction("AddNode"))
            {
                //ma
                //TblMain = Helper.GetDataTableProxy(MainTableName, Conn);
                HasDefaultRow = Helper.HasTableDefaultRow(MainTableName, Conn);

                object[] results = dbClass.CallLuaFunction("AddNode", new object[] { m_poupupNode, addinglevel });
                if (results != null)
                    // newNode = (TreeNode)results[0];
                {
                    newNode = (TreeNode)results[0];
                    if ((addinglevel == CatFields.Length || (addinglevel == 0 && bNoCatActually))) // ��ӵ���Ҷ�ӽ��
                    {
                        if (m_DisplayNode && newNode != null) // �ýű���ʾ������ı� 
                        {
                            // ת����ʾ�Ľ���ı�
                            string nodeText = m_displayString;
                            DataTable table = Helper.GetDataTableProxy(MainTableName, Conn);
                            DataRow row = table.Rows.Find(newNode.Tag as object[]); // �ҵ�������������
                            if (row != null) // ��������������
                            {
                                if (m_DisplayVarNode) // �ýű�����ÿ�������Ĳ�ͬ��ʾ
                                {
                                    if (table.Columns.Contains(m_displayField))
                                    {

                                        lua["DisplayField"] = row[m_displayField].ToString();
                                        lua["DisplayRow"] = row;
                                        object[] luaResult = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("DisplayTreeNodePlus");
                                        if(luaResult != null)
                                        {
                                            nodeText = luaResult[0] as string;
                                        }
                                        else
                                        {
                                            MessageBox.Show("�ű���DisplayTreeNodePlus�����ķ���ֵ����Ϊ��!");
                                        }
                                    }
                                }

                                string regexPattern = @"\{(?<value>\w*)\}";
                                Regex r = new Regex(regexPattern);
                                MatchCollection matches = r.Matches(nodeText);
                                string[] expressionResult = new string[matches.Count];
                                for (int j = 0; j < matches.Count; j++)
                                {
                                    expressionResult[j] = matches[j].Result("${value}");
                                    if (row[expressionResult[j]] != null)
                                    {
                                        string value = row[expressionResult[j]].ToString();
                                        if (m_defaultRow != null) // ��Ĭ����
                                        {
                                            if (value == "")
                                            {
                                                value = m_defaultRow[expressionResult[j]].ToString();
                                            }
                                        }
                                        nodeText = nodeText.Replace("{" + expressionResult[j] + "}",
                                            value);
                                    }
                                }

                                m_nodeTable[newNode] = newNode.Text; // ���±��������ԭ�ı�ֵ��hash��
                                m_nodeDisplayValueTable[newNode] = nodeText; // ���±������������ʾֵ��hash��
                                newNode.Text = nodeText;
                            }
                        }
                    }
                }
            }

            // ���Բ��ýű����, �������ͨ�ķ���
            else if (!bNoCatActually && addinglevel < CatFields.Length)
            {
                UpdateCatMetaInfo(addinglevel, ref newNode);
            }

            // ��lua,�������Ҷ�ӽ��
            else if (!LoadTreeWithLua && (addinglevel == CatFields.Length || (addinglevel == 0 && bNoCatActually)))
            {
                bool bUniqueCheck = false;
                string[] UniqueFields = null;
                if (dbClass.ExistLuaFunction("GetMainTblDBUniqueCheckFields")) // ĳЩ�ֶ��䲻����������ҪΨһ
                {
                    bUniqueCheck = true;
                    object[] results = dbClass.CallLuaFunction("GetMainTblDBUniqueCheckFields");
                    UniqueFields = new string[results.Length];
                    for (int i = 0; i < UniqueFields.Length; i++)
                    {
                        UniqueFields[i] = results[i].ToString();
                    }
                }

                //object[] keys = GetUnusedID();
                //create new id
                if (dbClass.ExistLuaFunction("CustomGenUnusedID")) // �ű�ָ���¼�¼id����
                {
                    object[] results = dbClass.CallLuaFunction("CustomGenUnusedID");
                    if (results[0] == null)
                        return;
                    keys = (object[])(((object[])results[0]).Clone());
                }
                else
                {
                    keys = (object[])(GetUnusedID().Clone());
                }

                DataRow newRow;
                if (bUniqueCheck)
                {
                    newRow = SqlInsertRecordWithUniqueField(MainTableName, keys, UniqueFields);
                }
                else
                {
                    newRow = SqlInsertRecord(MainTableName, keys);
                    for (int i = 0; i < TblMain.PrimaryKey.Length; i++)
                    {
                        keys[i] = newRow[TblMain.PrimaryKey[i].ColumnName];
                    }
                }

                if (!bUniqueCheck && !CatfieldsContainsDisplayfield /*&& !bNoCatActually*/ && !KeysContainsDisplayfileld)
                    SqlUpdateRecord(MainTableName, keys, DisplayField, "�½�");

                // ��� to add
                TreeNode nodeWhereToAdd;
                //if (this.baseTree.SelectedNode.Level == addinglevel)
                if (m_poupupNode.Level == addinglevel)
                    nodeWhereToAdd = m_poupupNode.Parent;//this.baseTree.SelectedNode.Parent;
                else
                    nodeWhereToAdd = m_poupupNode;//this.baseTree.SelectedNode;

                TreeNode _node = nodeWhereToAdd;

                DataRow row = null;

                if (nodeWhereToAdd != null) // �����null����Ȼ��û�з����ֶε������
                {
                    object[] values = new object[CatFields.Length];
                    for (int i = CatFields.Length - 1; i >= 0; i--)
                    {
                        values[i] = _node.Tag;
                        _node = _node.Parent;
                    }

                    try
                    {
                        row = SqlUpdateRecord_Native(MainTableName, keys, CatFields, values);
                        if (KeysContainsDisplayfileld)
                        {
                            // ����startID
                            for (int i = 0; i < TblMain.PrimaryKey.Length; i++)
                            {
                                SetLastID(MainTableName, TblMain.PrimaryKey[i].ColumnName, Convert.ToInt32(row[TblMain.PrimaryKey[i].ColumnName]) + 1);
                            }
                            UpdateStartIDs();
                        }
                    }
                    catch (System.Exception e)
                    {
                        string err = string.Format("�����ڴ����.\r\n\r\n������ϢΪ: \r\n{0}", e.Message);
                        MessageBox.Show(err);
                        SqlRemoveRecord(MainTableName, keys);
                        return;
                    }
                }
                else
                {
                    row = newRow;
                }


                // ���������
                object[] newKeys = new object[TblMain.PrimaryKey.Length];
                for (int i = 0; i < newKeys.Length; i++)
                {
                    newKeys[i] = row[TblMain.PrimaryKey[i].ColumnName];
                }

                if (bNoCatActually)
                {
                    if (nodeWhereToAdd == null)
                    {
                        newNode = this.baseTree.Nodes.Add("�½�");
                    }
                }
                else if (KeysContainsDisplayfileld)
                {
                    newNode = nodeWhereToAdd.Nodes.Add(row[DisplayField].ToString());
                }
                else if (CatfieldsContainsDisplayfield) // ֻ�е������ֶ���Ϊ�����ֶ�ʱ,�п���idҪ��GetUnusedID�ķ��ز�һ��,����Ҫ��������õõĽ��tag.
                {
                    newNode = nodeWhereToAdd.Nodes.Add(row[DisplayField].ToString());
                }
                else
                {
                    newNode = nodeWhereToAdd.Nodes.Add("�½�");
                }
                //newNode.ImageIndex = Convert.ToInt32(key);
                newNode.Tag = newKeys;


                // �ýű���ʾ������ı� 
                if(m_DisplayNode) 
                {
                    // ת����ʾ�Ľ���ı�
                    string nodeText = m_displayString;

                    if (m_DisplayVarNode) // �ýű�����ÿ�������Ĳ�ͬ��ʾ
                    {
                        try // �п��ܽű��ﶨ�����ʾ�ֶ��Ǳ��ﲻ���ڵģ����׳��Ƿ��������쳣��Ҫ����
                        {
                            lua["DisplayField"] = row[m_displayField].ToString();
                            lua["DisplayRow"] = row;
                            object[] luaResult = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("DisplayTreeNodePlus");
                            if(luaResult != null)
                            {
                                nodeText = luaResult[0] as string;
                            }
                            else
                            {
                                MessageBox.Show("�ű���DisplayTreeNodePlus�����ķ���ֵ����Ϊ��!");
                            }
                        }
                        catch(Exception ex) 
                        {
                            MessageBox.Show("��Ч����ʾ�ֶ�����! " + ex.Message);
                        }
                    }

                    string regexPattern = @"\{(?<value>\w*)\}";
                    Regex r = new Regex(regexPattern);
                    MatchCollection matches = r.Matches(nodeText);
                    string[] results = new string[matches.Count];
                    for (int j = 0; j < matches.Count; j++)
                    {
                        results[j] = matches[j].Result("${value}");
                        if (row[results[j]] != null)
                        {
                            string value = row[results[j]].ToString();
                            if (m_defaultRow != null) // ��Ĭ����
                            {
                                if (value == "")
                                {
                                    value = m_defaultRow[results[j]].ToString();
                                }
                            }
                            nodeText = nodeText.Replace("{" + results[j] + "}",
                                value);
                        }
                    }

                    m_nodeTable[newNode] = newNode.Text; // ���±��������ԭ�ı�ֵ��hash��
                    m_nodeDisplayValueTable[newNode] = nodeText; // ���±������������ʾֵ��hash��
                    newNode.Text = nodeText;
                   
                }
                // ��������ܶ���ű�
                if (dbClass.ExistLuaFunction("PostAddRecord"))
                {
                    bNeedToRunPostScript = true;
                }
            }

            // �����ұ�pghelper, pg
            CustomClassCache.DelDataTableCache("SELECT * FROM " + MainTableName);
            InitPgHelpers();
            foreach (PageDesc pd in m_Pages)
            {
                if (pd.pg != null)
                    pd.pg.Refresh();
            }


            // �½��rename
            bool bRenameNewCat = false;
            if (dbClass.ExistLuaFunction("IsRenameNewCat")) // ������Ҫ��ģ���ͨ������������Ʒ������Ƿ���������
            {
                object[] results = dbClass.CallLuaFunction("IsRenameNewCat");
                bRenameNewCat = (bool)results[0];
            }
            if (newNode == null)
            {
                MessageBox.Show("û����ӽ�㡣");
            }
            else if (bRenameNewCat)
            {
                this.baseTree.SelectedNode = newNode;
                buttonRename_Click(null, null);
            }
            else if (!newNode.IsEditing && !CatfieldsContainsDisplayfield)
            {
                this.baseTree.SelectedNode = newNode;
                buttonRename_Click(null, null);
            }

            if (bNeedToRunPostScript)
                dbClass.CallLuaFunction("PostAddRecord", new object[] { keys });
        }

        private void buttonAddSubLevel_Click(object sender, EventArgs e) // ����Ӳ�
        {
            int addinglevel = m_poupupNode.Level + 1;//this.baseTree.SelectedNode.Level + 1;
            addNode_Click(addinglevel);
        }

        private void buttonAddSameLevel_Click(object sender, EventArgs e) // ��ӱ���
        {
            int addinglevel = m_poupupNode.Level;//this.baseTree.SelectedNode.Level;
            addNode_Click(addinglevel);
        }

        private void baseTree_BeforeLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            TreeNode selected = e.Node;

            if (m_bRecordChanged || CustomTabHasChanged())
            {
                string question = "��ǰ��¼�����Ѿ����޸ģ������ȱ��������������\r\n\r\n��Ҫ������";
                DialogResult dr = MessageBox.Show(question, "��ʾ", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (dr == DialogResult.Cancel)
                {
                    e.CancelEdit = true;
                    selected.EndEdit(true);
                    return;
                }
                else if (dr == DialogResult.Yes)
                {
                    ExecCommand("savedocument", null);
                }
                else if (dr == DialogResult.No)
                {
                    m_bRecordChanged = false;
                }
            }

            if (m_recLock != null && selected != null && selected.Tag is object[])//selected.Level == CatFields.Length)
            {
                object[] keys = (object[])selected.Tag;
                string lockhost = string.Empty;
                if (!m_recLock.IsMyLock(keys, ref lockhost)) //�����ģ���Ѿ�����,���ܼ���,��������Ľ���
                {
                    if (!m_recLock.Lock(keys))
                    {
                        string message = string.Format("�ü�¼�ѱ�[{0}]����", lockhost);
                        MessageBox.Show(message);
                        e.CancelEdit = true;
                    }
                }
            }
        }

        private void Tree_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            string newText = ""; // ����������ʾֵ

            string sNewLabel = "";
            if (e.Label != null)
                sNewLabel = e.Label;
            else
                sNewLabel = e.Node.Text;// this.baseTree.SelectedNode.Text;

            TreeNode selected = e.Node;

            if (sNewLabel == selected.Text) // ������ǰ���ֵ��ͬ
            {
                if(m_nodeDisplayValueTable[selected] != null) // �ָ��ű����Ƶ���ʾֵ
                {
                    selected.Text = m_nodeDisplayValueTable[selected].ToString();
                }
                e.CancelEdit = true;
                return;
            }          

            if (m_recLock != null && selected != null && m_recLock.Locked)
            {
                object[] keys = selected.Tag as object[];
                if (keys != null && m_recLock.UnLock(keys) > 0)
                {
                    foreach (PageDesc pd in m_Pages)
                    {
                        pd.dbClass.ReloadDataOnSave = true; //�����Ѿ��޸��˱��е�����,��Ӱ���ұߵı���,��Ҫȫ�����ñ���ʱ����
                    }
                }
            }

            DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;

            bool bNoCatActually = false;
            if (CatFields.Length == 1 && CatFields[0] == "")
                bNoCatActually = true;

            if (!m_bPluginTree)
            {
                // �ű�����
                if (dbClass.ExistLuaFunction("RenameNode"))
                {
                    object[] tags = dbClass.CallLuaFunction("RenameNode", new object[] { baseTree.SelectedNode, sNewLabel });
                    // �ֶ���ѯ������ʾ�ı�
                    object[] keys = selected.Tag as object[];
                    DataRow row = null;
                    DataTable table = null;
                    if (keys != null)
                    {
                        table = Helper.GetDataTableProxy(MainTableName, m_conn);
                        row = table.Rows.Find(keys);
                    }
                    if(row != null) // ��ѯ�����Ϊ��
                    {
                        if (m_DisplayNode) // �ýű���ʾ������ı� 
                        {
                            // ת����ʾ�Ľ���ı�
                            string nodeText = m_displayString;

                            if (m_DisplayVarNode) // �ýű�����ÿ�������Ĳ�ͬ��ʾ
                            {
                                if (table.Columns.Contains(m_displayField))
                                {
                                    lua["DisplayField"] = row[m_displayField].ToString();
                                    lua["DisplayRow"] = row;
                                    object[] result = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("DisplayTreeNodePlus");
                                    if(result != null)
                                    {
                                        nodeText = result[0] as string;
                                    }
                                    else
                                    {
                                        MessageBox.Show("�ű���DisplayTreeNodePlus�����ķ���ֵ����Ϊ��!");
                                    }
                                }
                            }

                            string regexPattern = @"\{(?<value>\w*)\}";
                            Regex r = new Regex(regexPattern);
                            MatchCollection matches = r.Matches(nodeText);
                            string[] results = new string[matches.Count];
                            for (int j = 0; j < matches.Count; j++)
                            {
                                results[j] = matches[j].Result("${value}");
                                if (row[results[j]] != null)
                                {
                                    string value = row[results[j]].ToString();
                                    if (m_defaultRow != null) // ��Ĭ����
                                    {
                                        if (value == "")
                                        {
                                            value = m_defaultRow[results[j]].ToString();
                                        }
                                    }
                                    nodeText = nodeText.Replace("{" + results[j] + "}",
                                        value);
                                }
                            }
                            newText = nodeText;
                        }
                    }
                }

                else if (!bNoCatActually && selected.Level < CatFields.Length)
                {
                    // ��metainfo����صı�
                    string sql = string.Format("SELECT * FROM sys_meta_info WHERE tablename='{0}' AND fieldname='{1}'", MainTableName, CatFields[selected.Level]);
                    /* remoting
                    DataTable tblMetaInfo = Helper.GetDataTable(sql, Conn);
                    */
                    string filter = string.Format("tablename='{0}' AND fieldname='{1}'", MainTableName, CatFields[selected.Level]);
                    DataTable tblMetaInfo = Helper.GetDataTableProxy("sys_meta_info", filter, null, Conn);


                    if (tblMetaInfo.Rows.Count > 0)
                    {
                        DataRow row = tblMetaInfo.Rows[0];
                        switch (row["editortype"].ToString().Trim())
                        {
                            case "lookupcombo":
                                {
                                    string sql_lookup = "SELECT * FROM " + row["listtable"].ToString().Trim();

                                    DataTable tbl_lookup = Helper.GetDataTableProxy(row["listtable"].ToString().Trim(), Conn);

                                    DataRow rowchange = tbl_lookup.Rows.Find(selected.Tag);
                                    rowchange[row["listfield"].ToString().Trim()] = sNewLabel;

                                    /* remoting
                                    SqlDataAdapter adp = new SqlDataAdapter("SELECT * FROM " + row["listtable"].ToString().Trim(), m_conn);
                                    SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                                    adp.UpdateCommand = cmdBuilder.GetUpdateCommand();
                                    int val = adp.Update(tbl_lookup);
                                    tbl_lookup.AcceptChanges();*/

                                    Helper.SaveTable(tbl_lookup, sql_lookup, Conn);
                                }
                                break;

                            case "textcombo":
                                {
                                    if (selected.Tag == null)
                                        break;

                                    string listvalues = row["listvalues"].ToString().Trim();
                                    row["listvalues"] = listvalues.Replace(selected.Tag.ToString().Trim() + "," + selected.Text,
                                        selected.Tag.ToString().Trim() + "," + sNewLabel);

                                    /* remoting
                                    SqlDataAdapter adp = new SqlDataAdapter(sql, Conn);
                                    SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                                    adp.UpdateCommand = cmdBuilder.GetUpdateCommand();
                                    int val = adp.Update(tblMetaInfo);
                                    tblMetaInfo.AcceptChanges();
                                    */

                                    Helper.SaveTable(tblMetaInfo, sql, Conn);
                                }
                                break;

                            default:
                                break;
                        }
                    }

                    if (!(selected.Tag is object[]) && !CatFieldsContainsKey)
                        selected.Tag = sNewLabel;

                    // ��������ؽ�������
                    bool bCleanCat = false;
                    if (dbClass.ExistLuaFunction("IsCleanCat")) // ������Ҫ��ģ���ͨ������������Ʒ������Ƿ����ɡ�
                    {
                        object[] results = dbClass.CallLuaFunction("IsCleanCat");
                        bCleanCat = (bool)results[0];
                    }
                    if (!CatFieldsContainsKey && !bCleanCat)
                    {
                        LoadCatHashes();
                        AddAllCatNodes(null);
                    }
                }

                else if (!LoadTreeWithLua && !KeysContainsDisplayfileld && (selected.Level == CatFields.Length || (bNoCatActually && selected.Level == 0))) // ��lua,�Ҹ������
                {
                    DataRow row = SqlUpdateRecord(MainTableName, (object[])selected.Tag, DisplayField, sNewLabel);
                    CustomClassCache.DelDataTableCache("SELECT * FROM " + MainTableName);
                    
                    if (m_DisplayNode) // �ýű���ʾ������ı� 
                    {
                        // ת����ʾ�Ľ���ı�
                        string nodeText = m_displayString;

                        if (m_DisplayVarNode) // �ýű�����ÿ�������Ĳ�ͬ��ʾ
                        {
                            //DataTable table = Helper.GetDataTable("SELECT TOP 1 * FROM " + MainTableName, Conn);

                            DataTable table = Helper.GetDataTableTopRowProxy(MainTableName, Conn);


                            if (table.Columns.Contains(m_displayField))
                            {
                                lua["DisplayField"] = row[m_displayField].ToString();
                                lua["DisplayRow"] = row;
                                object[] result = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("DisplayTreeNodePlus");
                                if (result != null)
                                {
                                    nodeText = result[0] as string;
                                }
                                else
                                {
                                    MessageBox.Show("�ű���DisplayTreeNodePlus�����ķ���ֵ����Ϊ��!");
                                }
                            }
                        }

                        string regexPattern = @"\{(?<value>\w*)\}";
                        Regex r = new Regex(regexPattern);
                        MatchCollection matches = r.Matches(nodeText);
                        string[] results = new string[matches.Count];
                        for (int j = 0; j < matches.Count; j++)
                        {
                            results[j] = matches[j].Result("${value}");
                            if (row[results[j]] != null)
                            {
                                string value = row[results[j]].ToString();
                                if (m_defaultRow != null) // ��Ĭ����
                                {
                                    if (value == "")
                                    {
                                        value = m_defaultRow[results[j]].ToString();
                                    }
                                }
                                nodeText = nodeText.Replace("{" + results[j] + "}",
                                    value);
                            }
                        }
                        newText = nodeText;
                    }
                }
            }

            // �����ұ�pghelper, pg
            InitPgHelpers();
            foreach (PageDesc pd in m_Pages)
            {
                if (pd.pg != null)
                    pd.pg.Refresh();
            }

            // ��ѡ���Ը�������
            SelectTreeNode(this.baseTree.SelectedNode, true);

            // ���±��������ԭֵ��hash��           
            m_nodeTable[selected] = sNewLabel;
        
            if(newText != "") // ���±����������ʾֵ��hash��
            {
                selected.Text = newText;
                m_nodeDisplayValueTable[selected] = newText;
            }
            else
            {
                selected.Text = sNewLabel;
            }
            e.CancelEdit = true;

            TreeNode node = this.baseTree.SelectedNode;
            this.baseTree.SelectedNode = null;
            baseTree.SelectedNode = node;

            // PostRename
            if (dbClass.ExistLuaFunction("PostRenameNode"))
            {
                dbClass.CallLuaFunction("PostRenameNode", new object[] { sNewLabel });
            }
        }

        private void buttonRename_Click(object sender, EventArgs e) // ������
        {
            TreeNode node = this.baseTree.SelectedNode;
            this.baseTree.LabelEdit = true;
            if (!node.IsEditing)
            {
                if(m_nodeTable[node] != null) // ������ʾֵ�м�¼
                {
                    node.Text = m_nodeTable[node].ToString();
                }
                node.BeginEdit();
            }
        }

        private void buttonCopy_Click(object sender, EventArgs e) // ����
        {
            CopyFromNode = m_poupupNode;
        }

        private void buttonExtraMenuItem_Click(object sender, EventArgs e)
        {
            ButtonItem item = sender as ButtonItem;
            if (item == null)
                return;

            if (item.Tag is string && ((string)item.Tag).Length > 0)
            {
                DialogResult r = MessageBox.Show(item.Tag.ToString(), "��ʾ", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (r != DialogResult.Yes)
                    return;
            }

            DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;
            object[] results = dbClass.CallLuaFunction(item.Name);
        }

        private void buttonPaste_Click(object sender, EventArgs e) // ճ��
        {
            // ������ճ����ʽ:
            // 1> �Բ��뷽ʽճ��
            // 2> �Ը��Ǿ��м�¼��ʽճ��, id����
            // 3> �¼��������ճ�����ű��㶨

            // �ű����Դ������Ŀ���ճ��
            DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;
            if (dbClass.ExistLuaFunction("FuncExtraCopyPaste"))
            {
                object[] results = dbClass.CallLuaFunction("FuncExtraCopyPaste", new object[] { CopyFromNode.Level, CopyFromNode, m_poupupNode });
                if (results != null && results.Length > 0)
                {
                    bool bHandled = Convert.ToBoolean(results[0]);
                    if (bHandled)
                        return;
                }
            }

            if (m_poupupNode.Level == CopyFromNode.Level - 1) // ��ʽ 1
            {
                // �������ݿ��в�������
                object[] keys = (object[])(GetUnusedID().Clone());
                SqlInsertRecordAsCertainRow(MainTableName, keys, (object[])CopyFromNode.Tag);

                TreeNode newNode = null;
                TreeNode nodeWhereToAdd = m_poupupNode;
                DataRow row = null;

                if (dbClass.ExistLuaFunction("UpdatePasteNode"))
                {
                    m_lua["NodeToPaste"] = m_poupupNode;
                    m_lua["PasteKeys"] = keys;
                    object[] results = dbClass.CallLuaFunction("UpdatePasteNode");
                    row = (DataRow)results[0];
                }
                else
                {
                    // ��� to add
                    TreeNode _node = nodeWhereToAdd;
                    object[] values = new object[CatFields.Length];
                    for (int i = CatFields.Length - 1; i >= 0; i--)
                    {
                        values[i] = _node.Tag;
                        _node = _node.Parent;
                    }
                    
                    try
                    {
                        row = SqlUpdateRecord_Native(MainTableName, keys, CatFields, values);
                    }
                    catch (System.Exception ex)
                    {
                        string err = string.Format("�����ڴ����.\r\n\r\n������ϢΪ: \r\n{0}", ex.Message);
                        MessageBox.Show(err);
                        SqlRemoveRecord(MainTableName, keys);
                        return;
                    }
                }
                
                if (CatfieldsContainsDisplayfield) // ֻ�е������ֶ���Ϊ�����ֶ�ʱ,�п���idҪ��GetUnusedID�ķ��ز�һ��,����Ҫ��������õõĽ��tag.
                {
                    object[] newKeys = new object[CatFields.Length];
                    for (int i = 0; i < CatFields.Length; i++)
                    {
                        newKeys[i] = row[CatFields[i]];
                    }

                    if (m_DisplayNode) // �ýű�����������ı�����ʾ
                    {
                        // ת����ʾ�Ľ���ı�
                        string nodeText = m_displayString;
                        string regexPattern = @"\{(?<value>\w*)\}";
                        Regex r = new Regex(regexPattern);
                        MatchCollection matches = r.Matches(nodeText);
                        string[] results = new string[matches.Count];
                        for (int j = 0; j < matches.Count; j++)
                        {
                            results[j] = matches[j].Result("${value}");
                            if (row[results[j]] != null)
                            {
                                string value = row[results[j]].ToString();
                                if (m_defaultRow != null) // ��Ĭ����
                                {
                                    if (value == "")
                                    {
                                        value = m_defaultRow[results[j]].ToString();
                                    }
                                }
                                nodeText = nodeText.Replace("{" + results[j] + "}",
                                    value);
                            }
                        }

                        if (matches.Count > 0) // ��Ч����ʾ�ַ���
                        {
                            newNode = nodeWhereToAdd.Nodes.Add(nodeText);
                            m_nodeDisplayValueTable[newNode] = nodeText;
                        }
                        else
                        {
                    newNode = nodeWhereToAdd.Nodes.Add(row[DisplayField].ToString());
                        }
                    }
                    else
                    {
                        newNode = nodeWhereToAdd.Nodes.Add(row[DisplayField].ToString());
                    }
                    newNode.Tag = newKeys;
                    m_nodeTable[newNode] = row[DisplayField].ToString(); // ���±��������ԭ�ı���hash��
                }
                else
                {
                    if (m_DisplayNode) // �ýű�����������ı�����ʾ
                    {
                        // ת����ʾ�Ľ���ı�
                        string nodeText = m_displayString;
                        string regexPattern = @"\{(?<value>\w*)\}";
                        Regex r = new Regex(regexPattern);
                        MatchCollection matches = r.Matches(nodeText);
                        string[] results = new string[matches.Count];
                        for (int j = 0; j < matches.Count; j++)
                        {
                            results[j] = matches[j].Result("${value}");
                            if (row[results[j]] != null)
                            {
                                string value = row[results[j]].ToString();
                                if (m_defaultRow != null) // ��Ĭ����
                                {
                                    if (value == "")
                                    {
                                        value = m_defaultRow[results[j]].ToString();
                                    }
                                }
                                nodeText = nodeText.Replace("{" + results[j] + "}",
                                    value);
                            }
                        }

                        if (matches.Count > 0) // ��Ч����ʾ�ַ���
                        {
                            newNode = nodeWhereToAdd.Nodes.Add(nodeText);
                            m_nodeDisplayValueTable[newNode] = nodeText;
                        }
                        else
                        {
                            newNode = nodeWhereToAdd.Nodes.Add(row[DisplayField].ToString());
                        }
                    }
                    else
                    {
                        newNode = nodeWhereToAdd.Nodes.Add(row[DisplayField].ToString());
                    }
                    newNode.Tag = keys;
                    m_nodeTable[newNode] = row[DisplayField].ToString(); // ���±��������ԭ�ı���hash��
                }

                // �����ұ�pghelper, pg
                CustomClassCache.DelDataTableCache("SELECT * FROM " + MainTableName);

                InitPgHelpers();

                foreach (PageDesc pd in m_Pages)
                {
                    if (pd.pg != null)
                        pd.pg.Refresh();
                }

                // �½��rename
                if (!newNode.IsEditing && !CatfieldsContainsDisplayfield)
                {
                    this.baseTree.SelectedNode = newNode;
                    buttonRename_Click(null, null);
                    //this.baseTree.LabelEdit = true;
                    //newNode.BeginEdit();
                }
            }

            else if (m_poupupNode.Level == CopyFromNode.Level) // ��ʽ 2
            {
                // ������ʾ
                string src_key = string.Empty;
                foreach (object key in (object[])CopyFromNode.Tag)
                {
                    src_key += key.ToString();
                    src_key += ",";
                }
                src_key = src_key.Substring(0, src_key.Length - 1);

                string des_key = string.Empty;
                foreach (object key in (object[])m_poupupNode.Tag)
                {
                    des_key += key.ToString();
                    des_key += ",";
                }
                des_key = des_key.Substring(0, des_key.Length - 1);

                string strConfirm = string.Format("��ѡ���˴� [{0}] �Ÿ��Ƶ� [{1}] �š�\r\n\r\n[{2}] �ż�¼�У����������������ֶΡ���ʾ�ֶ�֮��������ֶζ�������д��\r\n\r\nȷ����", src_key, des_key, des_key);
                DialogResult result = MessageBox.Show(strConfirm, "��ʾ", MessageBoxButtons.YesNo);
                if (result == DialogResult.No)
                    return;

                // �������ݿ�, ���� ��ʾ�ֶ� �����ֶ� ����
                SqlUpdateRecordAsCertainRow(MainTableName, (object[])m_poupupNode.Tag, (object[])CopyFromNode.Tag);
                
                // ����ű����������Զ���
                if (dbClass.ExistLuaFunction("PostPasteRecord"))
                {
                    object[] results = dbClass.CallLuaFunction("PostPasteRecord", new object[] { CopyFromNode.Tag, m_poupupNode.Tag });
                }

                // ���������
                if (!CatfieldsContainsDisplayfield)
                {
                    //m_poupupNode.Text = CopyFromNode.Text;
                    m_nodeDisplayValueTable[m_poupupNode] = m_poupupNode.Text; // ���±����������ʾ�ı���hash��
                    m_nodeTable[m_poupupNode] = m_nodeTable[CopyFromNode]; // ���±��������ԭ�ı���hash��
                }

                // �����ұ�pghelper, pg
                CustomClassCache.DelDataTableCache("SELECT * FROM " + MainTableName);
                InitPgHelpers();
                foreach (PageDesc pd in m_Pages)
                {
                    if (pd.pg != null)
                        pd.pg.Refresh();
                }

                // ��ѡ���Ը��� pg ����
                SelectTreeNode(m_poupupNode, true);
            }
        }

        private void buttonDelete_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("�Ƿ�ȷ��ɾ���ý�㼰���ӽ��?", "ɾ��ȷ��", MessageBoxButtons.YesNo);
            if (result != DialogResult.Yes)
            {
                return;
            }

            string strRemarks = "δ��ӱ�ע";

            InputBoxForm ibf = new InputBoxForm();
            if (ibf.ShowDialog() == DialogResult.OK)
            {
                strRemarks = ibf.Remarks;
            }
            else
            {
                return;
            }
            

            bool bNoCatActually = false;
            if (CatFields.Length == 1 && CatFields[0] == "")
                bNoCatActually = true;

            TreeNode selected = this.baseTree.SelectedNode;
            int deleteCount = 0;
            DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;

            // �ű����
            if (dbClass.ExistLuaFunction("DeleteNode"))
            {
                //m_lua["SelectedNode"] = this.baseTree.SelectedNode;
                object[] results = dbClass.CallLuaFunction("DeleteNode", new object[] { baseTree.SelectedNode });
                if (results != null)
                    deleteCount = Convert.ToInt32(results[0]);

                baseTree.SelectedNode.Remove();
            }

            // ��C#���, ɾ�����Ƿ�����
            else if (!bNoCatActually && selected.Level < CatFields.Length)
            {
//                 // ���±�Ԫ����
//                 string sql = string.Format("SELECT * FROM sys_meta_info WHERE tablename='{0}' AND fieldname='{1}'", MainTableName, CatFields[selected.Level]);
//                 DataTable tblMetaInfo = Helper.GetDataTable(sql, Conn);
//                 if (tblMetaInfo.Rows.Count > 0)
//                 {
//                     DataRow row = tblMetaInfo.Rows[0];
//                     switch (row["editortype"].ToString().Trim())
//                     {
//                         case "lookupcombo":
//                             {
//                                 DataTable tbl_lookup = Helper.GetDataTable("SELECT * FROM " + row["listtable"].ToString().Trim(), Conn);
//                                 DataRow rowdel = tbl_lookup.Rows.Find(selected.Tag);
//                                 rowdel.Delete();
//                                 SqlDataAdapter adp = new SqlDataAdapter("SELECT * FROM " + row["listtable"].ToString().Trim(), m_conn);
//                                 SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
//                                 adp.DeleteCommand = cmdBuilder.GetDeleteCommand();
//                                 int val = adp.Update(tbl_lookup);
//                                 tbl_lookup.AcceptChanges();
//                             }
//                             break;
//                         case "textcombo":
//                             {
//                                 string listvalues = row["listvalues"].ToString().Trim();
//                                 row["listvalues"] = listvalues.Replace(selected.Tag.ToString().Trim() + "," + selected.Text, "");
//                                 SqlDataAdapter adp = new SqlDataAdapter(sql, Conn);
//                                 SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
//                                 adp.UpdateCommand = cmdBuilder.GetUpdateCommand();
//                                 int val = adp.Update(tblMetaInfo);
//                                 tblMetaInfo.AcceptChanges();
//                             }
//                             break;
//                         default:
//                             break;
//                     }
//                 }

                // ���������¼
//                 ArrayList ls = new ArrayList();
//                 FindAllOffspringNodesByLevel(selected, ref ls, CatFields.Length);

                string strQuotation = string.Empty;
                string strTypeName = Helper.GetFieldDataType(MainTableName, CatFields[selected.Level], Conn);
                if (strTypeName == "System.String")
                {
                    strQuotation = "'";
                }

                string sQl = string.Format("SELECT * FROM {0} WHERE [{1}]={2}{3}{4}", MainTableName, CatFields[selected.Level], strQuotation, selected.Tag is object[] ? (selected.Tag as object[])[0] : selected.Tag, strQuotation);
                string filter = string.Format("[{0}]={1}{2}{3}", CatFields[selected.Level], strQuotation, selected.Tag is object[] ? (selected.Tag as object[])[0] : selected.Tag, strQuotation);
                TreeNode parent_node = selected.Parent;
                while (parent_node != null)
                {
                    strQuotation = string.Empty;
                    strTypeName = Helper.GetFieldDataType(MainTableName, CatFields[parent_node.Level], Conn);
                    if (strTypeName == "System.String")
                    {
                        strQuotation = "'";
                    }

                    sQl += string.Format(" AND [{0}]={1}{2}{3}", CatFields[parent_node.Level], strQuotation, parent_node.Tag, strQuotation);
                    filter += string.Format(" AND [{0}]={1}{2}{3}", CatFields[parent_node.Level], strQuotation, parent_node.Tag, strQuotation);
                    parent_node = parent_node.Parent;
                }
                //DataTable tblrecord = Helper.GetDataTable(sQl, Conn);
                DataTable tblrecord = Helper.GetDataTableProxy(MainTableName, filter, null, Conn);
                foreach (DataRow r in tblrecord.Rows)
                {
                    r.Delete();
                }

                

                /* remoting
                SqlDataAdapter Adp = new SqlDataAdapter(sQl, Conn);
                SqlCommandBuilder CmdBuilder = new SqlCommandBuilder(Adp);
                Adp.DeleteCommand = CmdBuilder.GetDeleteCommand();
                int Val = Adp.Update(tblrecord);
                tblrecord.AcceptChanges();
                */

                int Val = Helper.SaveTable(tblrecord, sQl, Conn);

                deleteCount = Val;

                // ɾ��������ؽ��
                RecordDeleteNodes(selected, "ɾ���ڵ�", strRemarks);
                selected.Remove();
                
//                 foreach (TreeNode node in selected.Nodes)
//                     node.Remove();
            }

            else if (!LoadTreeWithLua && selected.Level == CatFields.Length || bNoCatActually) // ��lua,ɾ�����
            {
                deleteCount = SqlRemoveRecord(MainTableName, selected.Tag);
                RecordDeleteNodes(selected, "ɾ���ڵ�", strRemarks);
                selected.Remove();
                
            }

            MessageBox.Show("ɾ�����!\r\n\r\nɾ����¼����: " + deleteCount.ToString());

            if (dbClass.ExistLuaFunction("PostDeleteNode"))
                dbClass.CallLuaFunction("PostDeleteNode", new object[] { selected.Tag });
        }

        private void RecordDeleteNodes(TreeNode node, string action, string remark)
        {
            if (node.Level == CatFields.Length)
            {

                DataRecord dataRecord = DataRecord.GetDataRecord();
                dataRecord.Conn = m_conn; // sql����
                dataRecord.ModelID = m_modl_id; // ģ��id
                dataRecord.CurrentNode = node; // �����
                dataRecord.Time = DateTime.Now; // ��ǰ�޸�ʱ��
                dataRecord.UserName = Helper.GetHostName(); // ��ǰ�û���
                dataRecord.Action = action;
                dataRecord.Remarks = remark;
                dataRecord.Save();
                dataRecord.Action = "�޸�ֵ";
            }
            else
            {
                foreach (TreeNode child in node.Nodes)
                {
                    RecordDeleteNodes(child, action, remark);
                }
            }
            
        }

        private void baseTree_ItemDrag(object sender, ItemDragEventArgs e)
        {
            DoDragDrop(e.Item, DragDropEffects.Move);
        }

        private void baseTree_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Move;
        }

        private void baseTree_DragDrop(object sender, DragEventArgs e)
        {
            TreeNode OldNode;

            if (e.Data.GetDataPresent("System.Windows.Forms.TreeNode", false))
            {
                Point pt = ((TreeView)sender).PointToClient(new Point(e.X, e.Y));
                TreeNode DestinationNode = ((TreeView)sender).GetNodeAt(pt);
                OldNode = (TreeNode)e.Data.GetData("System.Windows.Forms.TreeNode");

                m_lua["SrcNode"] = OldNode;
                m_lua["DesNode"] = DestinationNode;
                object[] retobjs = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("TreeNodeDrag");
                CustomClassCache.DelDataTableCache("SELECT * FROM " + MainTableName);
                InitPgHelpers();
                foreach (PageDesc pd in m_Pages)
                {
                    if (pd.pg != null)
                        pd.pg.Refresh();
                }
            }
        }

        private void BaseForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (m_lua != null)
                m_lua.Dispose();

            if (m_3DEngie != null)
            {              
                for (int i = 0; i < m_Pages.Count; i++)
                {
                    m_3DEngie.RemoveScene(ModelName + "[" + Convert.ToString(i + 1) + "]");
                }
                // m_3DEngie.UnInit();
                // m_3DEngie = null;
            }

            //add by cuiwei 07.8.13
            if (ContainerPanel.Controls.Count > 0)
            {
                ContainerPanel.Controls[0].Dispose();
            }
            //add end

            CustomClassCache.DelMetaCache();
            SaveLayOut(ModelName);
            log.Log(TimeLog.enumLogType.ltend, null, null, null);
            log.Clear();
            logscript.Clear();
            loglocalscript.Clear();
        }
        
        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WM_REFRESHPG://����Զ�����Ϣ   
                    RefreshPropertyGrid();
                    break;
                case WM_REFREPROP:
                    //CustomProperty prop = m_ChangedProperty as CustomProperty;
                    //m_ChangedProperty = null;
                    //m_bRecordChanged = true;
                    //RefreshAllTabSameFieldValue(prop);
                    foreach (CustomProperty prop in m_lstChangedProperty)
                    {
                        RefreshAllTabSameFieldValue(prop);
                    }
                    m_lstChangedProperty.Clear();
                    m_bRecordChanged = true;
                    break;
                //case WM_REFREPRTREE:
                    //CustomProperty proptree = m_ChangedProperty_tree as CustomProperty;
                    //m_ChangedProperty_tree = null;
                    //RefreshAllTabSameFieldValue(proptree);
                    //RefreshTreeFindKey(proptree);                    
                //    break;
            }
            base.WndProc(ref m);
        }
        //��ǰѡ�еļ�¼����ֵ���޸ģ���Ҫͬ���޸����е�����
        private void RefreshTreeFindKey(CustomProperty prop)
        {
            //m_bDisableQuestion = true;
            try
            {
                //�����������ֵ������Ҫ���⴦��
                if (prop.Parent != null && prop.Parent is DBCustomClass)
                {
                    DBCustomClass parentClass = (DBCustomClass)prop.Parent;
                    
                    TreeNode selected = baseTree.SelectedNode;
                    int level = selected == null ? 0 : selected.Level;
                    object[] keys = parentClass.FindKeys;
                    LoadTree();
                    baseTree.SelectedNode = FindTreeNode(keys, level);
                }
            }
            finally
            {
                //m_bDisableQuestion = false;
            }
        }
        private void tmrRender_Tick(object sender, EventArgs e)
        {
            if (m_3DEngie != null)
            {
                m_3DEngie.FrameMove();
                this.Invalidate();
                //m_3DEngie.Render();
            }
        }
        protected override void OnPaintBackground(System.Windows.Forms.PaintEventArgs e)
        {
            //e.Dispose();
        }
        protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            if (m_3DEngie != null)
            {
                //m_3DEngie.FrameMove();
                m_3DEngie.Render();
            }
        }

        private DBCustomClass GetClass(int iTabIndex) // 0-based index
        {
            DBCustomClass dbClass = ((PageDesc)m_Pages[iTabIndex]).dbClass;
            return dbClass;
        }
        private void BasePg_SelectedGridItemChanged(object sender, SelectedGridItemChangedEventArgs e)
        {
            PropertyGrid pg = (PropertyGrid)sender;
            if (pg == this.tabControl1.SelectedTab.Tag)
            {
                DBCustomClass dbClass = (DBCustomClass)pg.SelectedObject;

                if (dbClass.ExistLuaFunction("beforeselectpropertychanged")) // ���ӻ�ȡѡ��ǰ��GridItem�ķ���
                {
                    GridItem item = e.OldSelection;
                    CustomPropertyDescriptor desc = item == null ? null : (CustomPropertyDescriptor)item.PropertyDescriptor;
                    if (desc != null)
                    {
                        dbClass.CallLuaFunction(desc.Property, "beforeselectpropertychanged", new object[] { desc.Property });
                    }
                }

                if (dbClass.ExistLuaFunction("onselectpropertychanged"))
                {
                    GridItem item = e.NewSelection;
                    CustomPropertyDescriptor desc = item == null ? null : (CustomPropertyDescriptor)item.PropertyDescriptor;
                    if (desc != null)
                    {
                        dbClass.CallLuaFunction(desc.Property, "onselectpropertychanged", new object[] { desc.Property });
                    }
                }
				//������ʱonselectrecordchanged�¼���ʱ�����յ�������
                //if (pg.SelectedObject != null && e.OldSelection == null)// && pg == tabControl1.SelectedTab.Tag)
                //{
                //    dbClass = (DBCustomClass)pg.SelectedObject;

                //    if (dbClass != null)// && dbClass.ExistLuaFunction("onselectrecordchanged"))
                //    {
                //        dbClass.CallLuaFunction("onselectrecordchanged", new object[] { dbClass, tabControl1.SelectedTabIndex });
                //    }
                //}
            }
        }

        /// <summary>
        /// ��GridItem��ֵ��ΪDBNull
        /// </summary>
        /// <param name="o">GridItem</param>
        public void ClearProperty(object o)
        {
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
            CustomProperty prop = (CustomProperty)o;
            prop.Value = DBNull.Value;
            page.pg.Refresh();
        }

        /// <summary>
        /// �ж�GridItem��ֵ�Ƿ���DBNull
        /// </summary>
        /// <param name="o"></param>
        /// <returns></returns>
        public bool IsValueDBNull(object o)
        {
            return (o is DBNull);
        }

        /// <summary>
        /// ˢ������
        /// </summary>
        public void RefreshProperty()
        {
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
            page.pg.Refresh();
        }

        private void BasePg_SelectedObjectsChanged(object sender, EventArgs e)
        {
            PropertyGrid pg = (PropertyGrid)sender;

            for (int i = 0; i < this.tabControl1.Tabs.Count; i++)
            {
                TabItem tabitem = this.tabControl1.Tabs[i];
                PropertyGrid each_pg = (tabitem.Tag is PropertyGrid) ? (PropertyGrid)(tabitem.Tag) : null;
                if (pg == each_pg)
                {
                    init_preview_index = i; // ע�����ﲻ��i + 1����Ϊ����ʹ���һ��tabitem�����ɼ��ģ��������ʱ����
                    break;
                }
            }

            if (pg.SelectedObject != null)// && pg == tabControl1.SelectedTab.Tag)
            {
                DBCustomClass dbClass = (DBCustomClass)pg.SelectedObject;

                if (dbClass != null)// && dbClass.ExistLuaFunction("onselectrecordchanged"))
                {
                    dbClass.CallLuaFunction("onselectrecordchanged", new object[] { dbClass, tabControl1.SelectedTabIndex });
                }
            }
        }

        // ��ĳ��ҳ��Ĭ��ֵ����Ϊ��ǰ��ʾ��ֵ
        private void SetPageValueIntoDefault(DBCustomClass dbClass)
        {
            foreach (DBCustomProperty prop in dbClass)
            {
                prop.UpdateDefaultValue();
                if (prop.ValueType == enumValueType.vtExpandNode && prop.Value is DBCustomClass)
                {
                    SetPageValueIntoDefault(prop.Value as DBCustomClass);
                }
            }
        }

        public void LoadPageDBClass(PropertyGrid pg)
        {
            queryCommand qc = null;
            PageDesc mypd = null;
            foreach (PageDesc pd in m_Pages)
            {
                if (pd.pg == pg)
                {
                    qc = pd.qc;
                    mypd = pd;
                }
            }
            if (qc == null)
            {
                return;
            }
            if (pg.SelectedObject == null)
            {
                pg.SelectedObject = selectObject(qc.dbClass, qc.Args);
            }
            int nRet = 0;
            for (int i = 0; i < qc.syncList.Count; i++)
            {
                syncItem si = qc.syncList[i] as syncItem;

                //Helper.AddLog(si.FieldName + ":" + si.FieldValue + "(" + si.ViewAsBold.ToString() + ")");

                if (qc.syncList[i] is unBoldItem)
                {
                    if (nRet > 0)
                    {
                        SetPageValueIntoDefault(mypd.dbClass);
                        nRet = 0;
                    }
                }
                else
                {
                    nRet += qc.dbClass.ModifyValue(si.MainTableName, si.FieldName, si.FieldValue, si.FindKeys);
                }
            }


            if(nRet>0)
            {
                qc.cls.ReloadDataOnSave = true;
            }
            qc.syncList.Clear();
        }
        private void tabControl1_SelectedTabChanged(object sender, TabStripTabChangedEventArgs e)
        {
            //add by cuiwei 07.8.23
            try
            {
                if (e.NewTab != null && e.NewTab.Visible)
                {
                    if (e.NewTab.Tag != null && e.NewTab.Tag is PropertyGrid)
                    {
                        LoadPageDBClass(((PropertyGrid)e.NewTab.Tag));                        
                    }
                }                
            }
            catch
            {

            }
            //add end

            // �л�TABҳ
            if (e.NewTab != null && e.NewTab.Visible)
            {
                if (e.NewTab.Tag != null && e.NewTab.Tag is PropertyGrid)
                {
                    if (((PropertyGrid)e.NewTab.Tag).SelectedObject != null)
                    {
                        DBCustomClass dbClass = (DBCustomClass)(((PropertyGrid)e.NewTab.Tag).SelectedObject);
                        //���ݲ��� NewRecord
                        if (dbClass.ExistLuaFunction("onselecttabchanged"))
                        {
                            dbClass.CallLuaFunction("onselecttabchanged", new object[] { dbClass });
                        }
                    }
                }
            }

            // Ԥ�����
            // ���ûscene
            int preview_index = this.tabControl1.SelectedTabIndex; // 1-based
            if (preview_index > 0 && m_3DEngie != null)
            {
                m_3DEngie.ActivateScene(ModelName + "[" + preview_index.ToString() + "]");
            }
            // ����Ԥ����preview_panels�Ŀɼ��ԡ�
            if (preview_panels != null)
            {
                foreach (MyPanelEx pvp in preview_panels)
                {
                    pvp.Visible = false;
                }
                preview_panels[preview_index - 1].Visible = true;
            }
        }

        //�Զ���ת��Ԫ��Ϣ��ʾ
        private void bShowMetainfo_Click(object sender, EventArgs e)
        {
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
            GridItem item = page.pg.SelectedGridItem;
            CustomPropertyDescriptor desc = item == null ? null : (CustomPropertyDescriptor)item.PropertyDescriptor;
            if (desc != null)
            {
                ((MainForm)ParentForm).ShowMetainfoEditor();
                if (((MainForm)ParentForm).MetainfoForm != null)
                    ((MainForm)ParentForm).MetainfoForm.EditField(Convert.ToInt32(desc.Property.Key));
            }
        }

        private DBCustomClass GetCanDeleteRecord()
        {
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
            GridItem item = (page.pg == null) ? null : page.pg.SelectedGridItem;
            CustomPropertyDescriptor desc = item == null ? null : (CustomPropertyDescriptor)item.PropertyDescriptor;
            if (desc != null && desc.Property.Parent != null)
            {
                CustomProperty prop = desc.Property;
                if (!(prop.Value is DBCustomClass))
                {
                    DBCustomClass cls = (DBCustomClass)prop.Parent;
                    if (cls.Parent != null)
                    {
                        return cls;
                    }
                }
            }
            return null;
        }
        private void bDeleteRecord_Click(object sender, EventArgs e)
        {
            try
            {
                DelSubTalbeRecordCommand cmd = new DelSubTalbeRecordCommand();
                cmd.Execute();

                //ɾ���ӱ��¼
                DBCustomClass cls = GetCanDeleteRecord();
                if (cls != null)
                {
                    string msg = string.Format("ȷ��Ҫɾ��[{0}]���м�¼��.", cls.MainTable);
                    DialogResult result = MessageBox.Show(msg, "ע��", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2);
                    if (result == DialogResult.Yes)
                    {
                        PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
                        GridItem item = page.pg.SelectedGridItem;
                        CustomPropertyDescriptor desc = item == null ? null : (CustomPropertyDescriptor)item.PropertyDescriptor;
                        cls.DeleteRecord(desc.Property);
                    }
                }
                else
                    MessageBox.Show("��Ҫ���дӱ��е����Բ���ɾ��", "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);            
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void cmScriptOp_PopupOpen(object sender, PopupOpenEventArgs e)
        {
            DBCustomClass cls = GetCanDeleteRecord();
            bDeleteRecord.Enabled = cls != null;
            // ����Ƿ�����½���¼
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
            if (page.pg == null)
                return;
            GridItem item = page.pg.SelectedGridItem;
            if (item == null)
            {
                bAddRecord.Enabled = false;
            }
            else
            {
                DBCustomClass dbClass = item.Value as DBCustomClass;
                if (dbClass == null || dbClass.ParentFieldType == 0)
                {
                    bAddRecord.Enabled = false;
                }
                else
                {

                    bAddRecord.Enabled = true;
                }
            }

            // Ȩ�����
            if (Program.m_bUseNewAcl)
            {
                if (!this.CanSaveDB)
                {
                    bEditFile.Enabled = false;
                    bDeleteRecord.Enabled = false;
                    bAddRecord.Enabled = false;
                    bClear.Enabled = false;
                    bClearContent.Enabled = false;
                }
                if (!this.CanCfg)
                {
                    bShowLuaCode.Enabled = false;
                    bShowMetainfo.Enabled = false;
                }
            }
        }

        public void RefreshAllTabSameFieldValue(CustomProperty prop)
        {
            DBCustomClass cls;
            object value;
            if (prop.Value is DBCustomClass)
            {
                cls = prop.Value as DBCustomClass;
                value = cls.Value;
            }
            else
            {
                cls = (DBCustomClass)prop.Parent;
                value = prop.Value;
            }
            if (prop.ValueType == enumValueType.vtCombo)
            {
                value = prop.ComboValues[prop.Value];
            }

            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
            //page.dbClass.ReloadDataOnSave = true;

            string strFindKeys = Helper.StringArrayToString(cls.FindKeys);

            int nSameFieldCount = 0;
            foreach (PageDesc pd in m_Pages)
            {
                //add by cuiwei 07.9.20
                //��ֱ���޸ģ�����֪ͨ��
                if (pd.pg == null)
                {
                    continue;
                }
                else
                {
                    if (page == pd)
                        continue;
                    queryCommand qc = pd.qc;
                    qc.syncList.Add(new syncItem(cls.MainTable, prop.ID, value, strFindKeys));
                    if (qc.cls == null) qc.cls = cls;
                }
                //add end
            }
            ReloadProperties();
            
            if (nSameFieldCount > 0)
            {
                cls.ReloadDataOnSave = true;
            }
        }

        private void BasePg_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            m_bRecordChanged = true;
            if (e.ChangedItem != null && e.ChangedItem.PropertyDescriptor is CustomPropertyDescriptor)
            {//��������ҳ��ĳ���Ա��޸�ʱ����֪ͨ��ÿ��TABҳ����Ҫ������ͬ����ͬ�ֶε�����ֵ
                CustomPropertyDescriptor desc = (CustomPropertyDescriptor)e.ChangedItem.PropertyDescriptor;
                RefreshAllTabSameFieldValue(desc.Property);
            }
        }

        private void baseTree_BeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            if (m_bRecordChanged || CustomTabHasChanged()) // && !m_bDisableQuestion)
            {
                DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;
                if (dbClass.ExistLuaFunction("onbeforeselect")) // �ű������Ƿ�� 
                {
                    object[] results = dbClass.CallLuaFunction("onbeforeselect");
                    {
                        string result = results[0].ToString();

                        switch(result)
                        {
                            case "save_always":
                                ExecCommand("savedocument", null);
                                e.Cancel = false;
                                break;
                            case "ask_always":
                                UserCfmSave(e);
                                break;
                            case "do_not_save_always":
                                e.Cancel = false;
                                UnlockRecord();
                                break;
                            default:
                                break;
                        }
                    }
                }
                else
                {
                    if ((!Program.m_bUseNewAcl || this.CanSaveDB) && !Program.m_bLockDBForCompetition)
                        UserCfmSave(e);
                }
            }
        }

        private void UserCfmSave(TreeViewCancelEventArgs e)
        {
            string msg = string.Format("��ǰ��¼�Ѿ����޸ģ���Ҫ������");
            DialogResult result = MessageBox.Show(msg, "ע��", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button3);
            switch (result)
            {
                case DialogResult.Cancel:
                    e.Cancel = true;
                    break;
                case DialogResult.Yes:
                    ExecCommand("savedocument", null);
                    e.Cancel = false;
                    break;
                case DialogResult.No:
                    e.Cancel = false;
                    UnlockRecord();
                    break;
            }
        }

        private void baseTree_AfterSelect(object sender, TreeViewEventArgs e)
        {
            m_bRecordChanged = false;            
            m_lua["SelectedNode"] = e.Node;
            SelectTreeNode(e.Node, true);            
        }

        private void SolveBuffComboValueLowerProblem()
        {
            /* remoting
            DataTable tbl_att = Helper.GetDataTable("select Attribute from skill_attribute", Conn);
            DataTable tbl_buff = Helper.GetDataTable("select * from tbl_buff", Conn);
            */

            DataTable tbl_att = Helper.GetDataTableProxy("skill_attribute", Conn);
            DataTable tbl_buff = Helper.GetDataTableProxy("tbl_buff", Conn);

            Conn.Open();
            foreach (DataRow row in tbl_att.Rows)
            {
                string newValue = row["Attribute"].ToString().Trim();
                string oldValue = newValue.ToLower();

                try
                {
                    /* remoting
                    SqlCommand cmd = Conn.CreateCommand();
                    cmd.CommandText = string.Format("UPDATE tbl_buff SET Attrib1='{0}' WHERE Attrib1='{1}'",
                        newValue, oldValue);
                    cmd.ExecuteNonQuery();
                    */
                    string cmd = string.Format("UPDATE tbl_buff SET Attrib1='{0}' WHERE Attrib1='{1}'", 
                        newValue, oldValue);
                    Helper.ExecuteCommand("tbl_buff", cmd, Conn);
                }
                catch (Exception ex)
                {
                    string err = string.Format("�ڸ��±� tbl_buff ʱ��������\r\n\r\n������Ϣ��{0}", ex.Message);
                    MessageBox.Show(err);
                }
            }
            Conn.Close();
        }

        private void bClear_Click(object sender, EventArgs e) // ���ֵ
        {
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
            GridItem item = page.pg.SelectedGridItem;
            CustomPropertyDescriptor desc = item == null ? null : (CustomPropertyDescriptor)item.PropertyDescriptor;
            //desc.Property
            if (desc == null)
                return;
            CustomProperty prop = desc.Property;

            if (((PageDesc)m_Pages[0]).dbClass.ExistLuaFunction("SetPropValueToDBNull"))
            {
                ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("SetPropValueToDBNull", new object[] { prop });
            }
            else
            {
                prop.Value = DBNull.Value;
            }

//             if (prop.ValueType == enumValueType.vtCombo || prop.ValueType == enumValueType.vtCustomEditor)
//             {
            m_bRecordChanged = true;
            page.pg.Refresh();
//             }
//             else
//             {
//                 MessageBox.Show("��ʱֻ����� \"������\" ���� \"�Զ���༭\" ���͵ı���");
//             }
        }

        private void bClearContent_Click(object sender, EventArgs e) // ���������
        {
//             string strFile = @"D:\Kingsoft\Game\sword3-products\trunk\client\data\source\doodad\�����\WJ_������001.Mesh";
//             string strOut = @"d:/�����";
//             m_3DEngie.FileToImage(strFile, strOut);
//             strFile = @"D:\Kingsoft\Game\sword3-products\trunk\client\data\source\doodad\�ܵ�\WJ_��ʧ����001.Mesh";
//             strOut = @"d:/��ʧ����001";
//             m_3DEngie.FileToImage(strFile, strOut);
//             strFile = @"D:\Kingsoft\Game\sword3-products\trunk\client\data\source\doodad\�ܵ�\WJ_��ʧ����002.Mesh";
//             strOut = @"d:/��ʧ����002";
//             m_3DEngie.FileToImage(strFile, strOut);

//             string strSfx = @"D:\Kingsoft\Game\sword3-products\trunk\client\data\source\other\��Ч\ϵͳ\SFX\ѡ��\ѡ����ЧA002.Sfx";
//             m_3DEngie.LoadPlayerModel(strSfx);

            DBCustomClass dbClass = ((PageDesc)m_Pages[0]).dbClass;
            dbClass.CallLuaFunction("_test");
            return;

            //string strFile = @"D:\Kingsoft\Game\sword3-products\trunk\client\data\source\maps\��\��.Map.Logical";
            //m_3DEngie.SaveLogicScene(strFile);
            //string strFile = @"D:\Kingsoft\Game\sword3-products\trunk\client\data\source\NPC_source\P006\ģ��\P006004.mdl";
//             string strFile = @"D:\Kingsoft\Game\sword3-products\trunk\client\data\source\NPC_source\P006\����\P006_bat01�ز�����.ani";
//             string strOut = @"d:/JELL";
//             if (m_3DEngie != null)
//                 m_3DEngie.FileToImage(strFile, strOut);
            //SolveBuffComboValueLowerProblem();
            //AdjustPortraitRecordsCountToNpc();
        }

        private void buttonReloadTree_Click(object sender, EventArgs e)
        {
            ReloadTree();
        }
        private TreeNode FindNodeByTagAndText(TreeNode fromFind, TreeNode toFind)
        {
            if (ObjectsEqual(fromFind.Tag, toFind.Tag) && fromFind.Text == toFind.Text)
                return fromFind;

            foreach (TreeNode node in fromFind.Nodes)
            {
                TreeNode nodeReturn = FindNodeByTagAndText(node, toFind);
                if (nodeReturn != null)
                    return nodeReturn;
            }

            return null;
        }

        private void buttonItem1_Click(object sender, EventArgs e) // �༭( Ctrl + E )
        {
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
            GridItem item = page.pg.SelectedGridItem;
            CustomPropertyDescriptor desc = item == null ? null : (CustomPropertyDescriptor)item.PropertyDescriptor;
            if (desc == null)
                return;
            CustomProperty prop = desc.Property;

            object editor = null;
            object newValue = null;
            switch (prop.ValueType)
            {
                case enumValueType.vtCustomEditor:
                    editor = (CustomEditor)desc.GetEditor(null);
                    newValue = ((CustomEditor)editor).DoEditValue(page.dbClass, desc, prop.Value);
                    break;
                case enumValueType.vtFileName:
                    editor = (FileNameEditorEx)desc.GetEditor(null);
                    newValue = ((FileNameEditorEx)editor).DoEditValue(desc, prop.Value);
                    break;
                default:
                    MessageBox.Show("�˿�ݼ���ʱֻ֧�� \"�Զ���༭\" ���� \"�ļ������\" ���͵ı��");
                    break;
            }
            if (editor != null)
            {
                if (newValue != null)
                {
                    prop.SetValue(newValue);
                }
                page.pg.Refresh();
                PropertyValueChangedEventArgs pvcea = new PropertyValueChangedEventArgs(item, prop.Value);
                BasePg_PropertyValueChanged(page.pg, pvcea);
            }
        }

        private void BaseForm_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.F1:
                    buttonShowLuaCode_Click(this.bShowLuaCode, null);
                    break;

                case Keys.F2:
                    bShowMetainfo_Click(bShowMetainfo, null);
                    break;

                case Keys.F3:
                    DBCustomClass dbClass = ((PageDesc)m_Pages[1]).dbClass;
                    PropertyGrid pg = ((PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1]).pg;
                    object[] ret = dbClass.CallLuaFunction("_F3", new object[] { pg.SelectedGridItem });
                    break;

                default:
                    break;
            }
        }

        private void bEditFile_Click(object sender, EventArgs e)
        {
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
            GridItem item = page.pg.SelectedGridItem;
            CustomPropertyDescriptor desc = item == null ? null : (CustomPropertyDescriptor)item.PropertyDescriptor;
            if (desc == null)
                return;
            CustomProperty prop = desc.Property;

            object prefixOutput = (prop.CutPrefix != string.Empty && prop.CutPrefix != null) ? prop.CutPrefix : prop.Prefix;
            if (prefixOutput == null)
                return;
            string filename = System.IO.Path.Combine(prefixOutput.ToString(), prop.Value.ToString());
            string content = FileFolderHelper.FileToString(filename);
            if (content != "")
            {
                //loglocalscript.NewLine();
                //loglocalscript.Log(TimeLog.enumLogType.ltstart, this.ModelName, "open local script file", filename);
                try
                {
                    ScriptCodeForm frm = Program.MainForm.CodeForm;
                    frm.m_lua = this.m_lua;
                    //modify by cuiwei 07.9.12
                    frm.Show(filename, null);
                    //modify end
                    //if (frm.DialogResult == DialogResult.OK)
                    //{
                    //    FileFolderHelper.StringToFile(frm.lua_OutputBox.Text, filename);
                    //    loglocalscript.Log(TimeLog.enumLogType.ltend, null, null, filename + " saved local file");
                    //}            
                }
                finally
                {
                    //�༭���Ǳ����ļ������ô��������־�ɣ�
                    //loglocalscript.Log(TimeLog.enumLogType.ltend, null, null, null);
                }
            }
        }

        /// <summary>
        /// �½������ֶεļ�¼
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void bAddRecord_Click(object sender, EventArgs e)
        {
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1]; // ��ǰҳ
            GridItem item = page.pg.SelectedGridItem; // ѡ�е�item
            DBCustomClass cls = (item.Value as DBCustomClass);
            cls.NewRecord(cls.Property);
            // ����ˢ��̫���ģ����пո�֮��
            //SelectTreeNode(this.baseTree.SelectedNode, true);
        }

        /// <summary>
        /// �û�ѡ��鿴�޸���־(��ǰ��¼)
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bViewLog_Click(object sender, EventArgs e)
        {
            PageDesc page = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1]; // ��ǰҳ
            GridItem item = page.pg.SelectedGridItem; // ѡ�е�item
            TreeNode currentNode = baseTree.SelectedNode;
            if (item != null && currentNode != null)
            {
                CustomPropertyDescriptor descriptor = item.PropertyDescriptor as CustomPropertyDescriptor;
                object[] primaryKeys = currentNode.Tag as object[];
                if (descriptor != null && primaryKeys != null)
                {
                    EditLog logForm = new EditLog(m_conn, descriptor.Property.Key, currentNode, item);
                    logForm.Visible = true;
                }
            }
        }

        private void BaseForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason != CloseReason.WindowsShutDown)
            {
                if ((!Program.m_bUseNewAcl || this.CanSaveDB) && (m_bRecordChanged || CustomTabHasChanged()) && !Program.m_bLockDBForCompetition)
                {
                    string msg = string.Format("��ǰ��¼�Ѿ����޸ģ���Ҫ������");
                    DialogResult result = MessageBox.Show(msg, "ע��", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button3);
                    switch (result)
                    {
                        case DialogResult.Cancel:
                            e.Cancel = true;
                            break;
                        case DialogResult.Yes:
                            ExecCommand("savedocument", null);
                            e.Cancel = false;
                            break;
                        case DialogResult.No:
                            e.Cancel = false;
                            UnlockRecord();
                            break;
                    }
                }
            }
        }

        private void buttonExpandAll_Click(object sender, EventArgs e)
        {
            TreeNode node = baseTree.SelectedNode;
            if (node != null)
            {
                if (node.IsExpanded)
                    node.Collapse(false);
                else
                    node.ExpandAll();
            }
        }

        /// <summary>
        /// ��ȡ��ǰ����ҳ����������ֵ
        /// </summary>
        /// <returns>������ǰ����ҳ����������ֵ��hash��</returns>
        public Hashtable GetPropertyGridData()
        {
            Hashtable dataTable = new Hashtable();

            if (this.tabControl1.SelectedTabIndex >= 0)
            {
                PageDesc pageDesc = (PageDesc)m_Pages[this.tabControl1.SelectedTabIndex - 1];
                PropertyGrid pg = pageDesc.pg;
                DBCustomClass dbClass = pageDesc.dbClass;
                for (int i = 0; i < dbClass.Count; i++)
                {
                    CustomProperty p = dbClass[i];
                    if (p != null)
                    {
                        dataTable[p.ID] = p.Value;
                    }
                }
            }

            return dataTable;
        }

        /// <summary>
        /// ��ȡBaseForm�Ի���
        /// </summary>
        /// <param name="modelName">ģ������</param>
        /// <returns>BaseForm�Ի���</returns>
        public BaseDialog ShowModelInDialog(string modelName)
        {
            m_Dialog = new BaseDialog(modelName);
            DialogResult result = m_Dialog.ShowDialog();
            if(result == DialogResult.OK)
            {
                return m_Dialog;
            }
            else
            {
                return null;
            }
        }

        public static BaseDialog ShowModelInDialog_Static(string modelName)
        {
            if (staticDialogs[modelName] == null)
            {
                staticDialogs[modelName] = new BaseDialog(modelName);
                (staticDialogs[modelName] as BaseDialog).useHideMode = true;
            }

            DialogResult result = (staticDialogs[modelName] as BaseDialog).ShowDialog();
            if (result == DialogResult.OK)
            {
                return staticDialogs[modelName] as BaseDialog;
            }
            else
            {
                return null;
            }
        }

   
        private void BaseForm_Activated(object sender, EventArgs e)
        {
            foreach (Bar mybar in dotNetBarManager1.Bars)
            {
                if (mybar.Visible == true && mybar.DockSide == eDockSide.None)
                {
                    mybar.Visible = false;                    
                    mybar.DockSide = eDockSide.Left;
                    mybar.Refresh();
                    mybar.DockSide = eDockSide.None;
                    mybar.Visible = true;
                    mybar.Refresh();    
                }
            }

            if (Program.EnableRemoting)
            {
                DataTableProxy proxy = DataTableServerProxy.GetDataTableProxy();
                proxy.HISetModel(ModelName);
            }
        }


        /* add  by SunYong begin
         * 1��InitCatDisplayText�����ı���CatHashes���valueֵ������ӽڵ�ʱ����ֱ�Ӵ�CatHashes����ֵ������ʾ
         * 2����������ڵ��Ϊ����ʾ��+���ţ������һ����ʱ�ڵ�Text = "ɾ������" tag = 0����չ��ʱ��ɾ���ýڵ�
        */
        /// <summary>
        /// ��ȡ��nDepth��������ֵ(��ȡ��nDepth��������нڵ�)
        /// </summary>
        /// <param name="nDepth">�ڼ���</param>
        /// <returns>û���ӽڵ㷵��flase</returns>
        private bool GetColumnValues(int nDepth, ref ArrayList al_ht, string[] strParentNodesValue)
        {
            if (al_ht == null)
            {
                return false;
            }
            string strColumnName = string.Empty;
            string strWhere = string.Empty;

            if (nDepth > 0)            
            {
                strWhere = ConstructSQL(nDepth, strParentNodesValue);
            }
            strColumnName = CatFields[nDepth];

            string strSql = string.Empty;
            if (ModID == 100 && nDepth == 1)//����ģ���������
            {
                strWhere += string.Format(" group by [{0}]", strColumnName);
                strSql = string.Format("Select [{0}] from {1} {2}", strColumnName, MainTableName, strWhere);
                strWhere = string.Format("Select [_index] from sys_useracl where ([_index] in ({0})) order by [userid]", strSql);
                strSql = strWhere;
            }
            else
            {
                strWhere += string.Format(" group by [{0}] order by [{0}]", strColumnName);
                strSql = string.Format("Select [{0}] from {1} {2}", strColumnName, MainTableName, strWhere);
            }

            DataTable dt = Helper.GetDataTableWithSqlProxy(MainTableName, strSql, Conn);
            if (dt.Rows.Count <= 0)
            {
                dt.Dispose();
                return false;
            }

            Hashtable displayRelation = CatHashes[nDepth];

            foreach (DataRow row in dt.Rows)
            {
                string str = string.Empty;
                if (row[0] != DBNull.Value)
                {
                    str = row[0].ToString();
                }
                else
                {
                    DataRow default_row = CustomClassCache.GetPropDefaultValue(MainTableName);
                    if (default_row != null)
                    {
                        str = default_row[strColumnName].ToString();
                    }
                }
                if (str != string.Empty && displayRelation[str] != null)
                {
                    DictionaryEntry de = new DictionaryEntry(str, displayRelation[str]);
                    al_ht.Add(de);
                }
                
            }
            dt.Dispose();
            if (ModID == 100 && nDepth == 1)
            {
            }
            else
            {
                // StringComparer sc = new StringComparer();
                // al_ht.Sort(sc);
                al_ht.Sort(new DictionaryEntryValueComparer());
            }
            return true;
        }

        /// <summary>
        /// ����where����
        /// </summary>
        /// <param name="nDepth"></param>
        /// <returns></returns>        
        private string ConstructSQL(int nDepth, string[] strParentsNodeValue)
        {
            System.Diagnostics.Debug.Assert(nDepth > 0);
            DataRow default_row = CustomClassCache.GetPropDefaultValue(MainTableName);
            string strSql = string.Empty; 

            strSql = "Where ";
            for (int i = 0; i < nDepth; i++)
            {
                string strOneCondition = string.Empty;
                string strColumnName = CatFields[i].ToString();
                strOneCondition = string.Format("[{0}] = '{1}'", strColumnName, strParentsNodeValue[i]);
                if (default_row != null && default_row[strColumnName].ToString() == strParentsNodeValue[i].ToString())//����ýڵ���Ĭ��ֵ 
                {
                    strOneCondition +=  string.Format(" or [{0}] is NULL", strColumnName);
                }
                else if (strParentsNodeValue[i] == string.Empty)//����ýڵ�Ϊ�յ���û��Ĭ��ֵ��Ĭ���в����ڵ��µĽ����
                {
                    strOneCondition += string.Format(" or [{0}] is NULL", strColumnName);
                }

                strSql += string.Format("({0})", strOneCondition);
                if (i + 1 < nDepth)
                {
                    strSql += " AND ";
                }
            }  
            return strSql;
        }
       
       /// <summary>
       /// ���nDepth��Ľڵ�
       /// </summary>
       /// <param name="tnc"></param>
       /// <param name="nDepth"></param>
       /// <param name="strParentsNodeValue">ǰ����ֵ�ļ�¼</param>
        private void AddChildNodes(TreeNodeCollection tnc, int nDepth, string[] strParentsNodeValue)
        {
            if (!(CatFields.Length == 1 && CatFields[0] == "") && nDepth < CatFields.Length)
            {
                ArrayList al_ht = new ArrayList(); 
                if (!GetColumnValues(nDepth, ref al_ht, strParentsNodeValue))
                {
                    return;
                }                                     

                foreach (DictionaryEntry de in al_ht)
                {
                    if (CatHashes[nDepth][de.Key] != null)
                    {
                        TreeNode newnode = tnc.Add(CatHashes[nDepth][de.Key].ToString());
                        newnode.Tag = de.Key.ToString();
                        newnode.Nodes.Add("ɾ������");
                        newnode.FirstNode.Tag = 0; 
                    }
                }
            }
            else
            {
                AddLeafNodes(nDepth, tnc, strParentsNodeValue);
            }
        
        }

        /// <summary>
        /// ���Ҷ�ڵ�
        /// </summary>
        /// <param name="nDepth"></param>
        /// <param name="tnc"></param>
        private void AddLeafNodes(int nDepth, TreeNodeCollection tnc, string[] strParentsNodeValue)
        {
            string strWhere = string.Empty;

            if (nDepth > 0)
            {
                strWhere = ConstructSQL(nDepth, strParentsNodeValue);
            }

            // ������¼��ʾ˳��
            string orderString = GetSordFields();

            if (string.IsNullOrEmpty(orderString))
            {
                orderString = string.Format("[{0}]", m_strDisplayField);
            }
            else
            {
                string[] dataArray = orderString.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                orderString = "";

                foreach (string fieldName in dataArray)
                {
                    orderString += string.Format("[{0}], ", fieldName.Trim());
                }

                if (orderString.Contains(m_strDisplayField))
                {
                    orderString = orderString.TrimEnd(new char[] { ',', ' ' });
                }
                else
                {
                    orderString += string.Format("[{0}]", m_strDisplayField);
                }                
            }

            string strSql = string.Format("Select * from {0} {1} order by [{2}]", MainTableName, strWhere, orderString);
            //DataTable dt = Helper.GetDataTable(strSql, Conn);
            string where = string.Empty;
            if (strWhere.Length > 1)
            {
                where = strWhere.Substring(6);
            }
            // string order = string.Format("[{0}]", orderString);

            DataTable dt = Helper.GetDataTableProxy(MainTableName, where, orderString, Conn);

            bool bDefaultRecordExist = true;
            string __sql = string.Format("SELECT [hasdefaultrow] FROM sys_table_cfg WHERE tablename='{0}'", MainTableName);
            //DataTable tbl_tbl_cfg = Helper.GetDataTable(__sql, Conn);
            string filter = string.Format("tablename='{0}'", MainTableName);

            DataTable tbl_tbl_cfg = Helper.GetDataTableProxy("sys_table_cfg", filter, null, Conn);

            if (tbl_tbl_cfg.Rows.Count > 0)
            {
                DataRow cfg_row = tbl_tbl_cfg.Rows[0];
                if (cfg_row["hasdefaultrow"] != DBNull.Value)
                {
                    bDefaultRecordExist = Convert.ToBoolean(cfg_row["hasdefaultrow"]);
                }
            }
            tbl_tbl_cfg.Dispose();

            object[] primaryKeys = new object[TblMain.PrimaryKey.Length]; // ����
            for (int i = 0; i < primaryKeys.Length; i++)
            {
                primaryKeys[i] = "0";
            }
            
            if (HasDefaultRow)
                m_defaultRow = TblMain.Rows.Find(primaryKeys); // Ĭ����
           
            foreach (DataRow row in dt.Rows)
            {
                
                TreeNode newnode;
                string nodeText = string.Empty;
               
                if (SetLeafDisplayText(row, ref nodeText, bDefaultRecordExist))
                {
                    newnode = tnc.Add(nodeText);
                    m_nodeDisplayValueTable[newnode] = nodeText;
                }
                else
                {
                    newnode = tnc.Add(row[DisplayField].ToString());
                } 
                object[] keys = new object[TblMain.PrimaryKey.Length];
                for (int j = 0; j < TblMain.PrimaryKey.Length; j++)
                    keys[j] = row[TblMain.PrimaryKey[j].ColumnName];
                newnode.Tag = keys;
                if (m_displayString != null)
                {
                    m_nodeTable[newnode] = row[DisplayField].ToString();
                }
            }
            dt.Dispose();            
        }

        /// <summary>
        /// ����Ҷ�ڵ����ʾֵ�������ǿ����ģ�
        /// ���m_displayString����Ч����ʾֵ�򷵻�true
        /// </summary>
        /// <param name="row"></param>
        /// <param name="nodeText"></param>
        /// <returns></returns>
        private bool SetLeafDisplayText(DataRow row, ref string nodeText, bool bDefaultRecordExist)
        {
            if (m_displayString == null)
            {
                return false;
            }
            nodeText = m_displayString;
           
            if (m_DisplayVarNode) // �ýű�����ÿ�������Ĳ�ͬ��ʾ
            {                
                if (TblMain.Columns.Contains(m_displayField))
                {
                    lua["DisplayField"] = row[m_displayField].ToString();
                    lua["DisplayRow"] = row;
                    object[] result = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("DisplayTreeNodePlus");
                    if (result != null)
                    {
                        nodeText = result[0] as string;
                    }
                    else
                    {
                        MessageBox.Show("�ű���DisplayTreeNodePlus�����ķ���ֵ����Ϊ��!");
                    }
                }               
            }
                       
            string regexPattern = @"\{(?<value>\w*)\}";
            Regex r = new Regex(regexPattern);
            MatchCollection matches = r.Matches(nodeText);
            string[] results = new string[matches.Count];
            for (int j = 0; j < matches.Count; j++)
            {
                results[j] = matches[j].Result("${value}");
                if (row[results[j]] != null)
                {
                    string value = row[results[j]].ToString();
                    if (bDefaultRecordExist && m_defaultRow != null) // ��Ĭ����
                    {
                        if (value == "")
                        {
                            value = m_defaultRow[results[j]].ToString();
                        }
                    }
                    nodeText = nodeText.Replace("{" + results[j] + "}", value);
                }
            }
            return matches.Count > 0; 
        }

        /// <summary>
        /// �ı�catHashes���valueΪҪ��ʾ��ֵ---�˺����ı���CatHashes
        /// </summary>
        /// <param name="nDepth"></param>
        /// <param name="strKey"></param>
        /// <param name="table"></param>
        /// <returns></returns>
        private void InitCatDisplayText(DataTable table)
        {
            if (CatHashes == null)
            {
                return;
            }
            int nDepth = -1;
            foreach (Hashtable ht in CatHashes)
            {
                nDepth++;
                ArrayList al_ht = new ArrayList(ht);
                StringComparer comboComp = new StringComparer();
                al_ht.Sort(comboComp);
                foreach (DictionaryEntry de in al_ht)
                {                    
                    object[] ret = ((PageDesc)m_Pages[0]).dbClass.CallLuaFunction("CustomCategory", new object[] { nDepth, de.Key.ToString(), de.Value.ToString(), table });
                    if (ret != null)
                    {
                        ht[de.Key] = ret[0].ToString();
                    }
                }
            }           
        }

        private void baseTree_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            if (e.Node.FirstNode.Tag.ToString() == "0" && e.Node.FirstNode.Text == "ɾ������")
            {
                int nDepth = e.Node.Level;
                string[] strParentsNodeValue = new string[nDepth + 1];
                TreeNode node = e.Node;
                for (int i = nDepth; i >= 0; i--)
                {
                    strParentsNodeValue[i] = node.Tag.ToString();
                    node = node.Parent;
                }             
                TreeNodeCollection tnc = e.Node.Nodes;
                tnc.Clear(); 
                AddChildNodes(tnc, nDepth + 1, strParentsNodeValue);
            }
            
        }
        private GridItem GetRoot(PropertyGrid pg)
        {
            GridItem root = pg.SelectedGridItem;
            while (root.Parent != null)
                root = root.Parent;
            return root;
        }
        /*private GridItem GetItem(GridItem root, string strLabel)
        {
            if (root.Label == strLabel)
                return root;
            else
            {
                foreach (GridItem gi in root.GridItems)
                {
                    GridItem result = GetItem(gi, strLabel);
                    if (result != null) return result;
                }
                return null;
            }
        }*/

        private GridItem GetItem(GridItem root, string strFieldid)
        {
            DBCustomProperty prop = GetProperty(root) as DBCustomProperty;
            if (prop != null && prop.ID == strFieldid)
                return root;
            else
            {
                foreach (GridItem gi in root.GridItems)
                {
                    GridItem result = GetItem(gi, strFieldid);
                    if (result != null) return result;
                }
                return null;
            }
        }
        private CustomProperty GetProperty(GridItem item)
        {
            CustomPropertyDescriptor desc = item == null ? null : (CustomPropertyDescriptor)item.PropertyDescriptor;
            if (desc == null)
                return null;
            CustomProperty prop = desc.Property;
            return prop;
        }
        private PropertyGrid GetPropertyGrid(DBCustomProperty prop)
        {
            DBCustomClass cls = prop.Parent as DBCustomClass;
            while (cls.Parent != null)
            {
                if (cls.Parent is DBCustomClass)
                {
                    cls = (DBCustomClass)(cls.Parent);
                }
            }
            PropertyGrid pg = null;
            foreach (PageDesc pd in m_Pages)
            {
                if (pd.dbClass == cls)
                {
                    pg = pd.pg;
                    break;
                }
            }
            return pg;
        }
        //����ָ�������ԣ�չ��,ע�⣺���ܶԴӱ�����¼�������ֻ���ҵ���1����¼������
        public bool ExpandProperty(DBCustomProperty prop)
        {            
            GridItem root = GetRoot(GetPropertyGrid(prop));
            GridItem item = GetItem(root, prop.ID);
            if (item != null)
            {
                item.Expanded = true;
                return item.Expanded;
            }
            return false;
        }
    }
    //the end of SunYong added
}
