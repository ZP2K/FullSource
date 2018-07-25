using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Data.SqlClient;
using System.Diagnostics;
using System.IO;
using LuaInterface;
using DataTableRemotingClient;

namespace GameDesingerTools
{
    public partial class DBTableEditor : Form
    {
        SqlConnection m_conn = MainForm.conn;
        public SqlConnection Conn { get { return m_conn; } }

        Hashtable m_htAllDbTables = null;
        public Hashtable AllTables { get { return m_htAllDbTables; } set { m_htAllDbTables = value; } }

        DataTable m_dtExportTbl = null;
        public DataTable ExportTbl { get { return m_dtExportTbl; } set { m_dtExportTbl = value; } }

        DataTable m_dtTblCfgTbl = null;
        public DataTable TblCfgTbl { get { return m_dtTblCfgTbl; } set { m_dtTblCfgTbl = value; } }

        static string TRASH_FIELD = "___delete_me";
        private TimeLog m_log = new TimeLog();

        private List<TreeNode> newNodes = new List<TreeNode>(); // �½���������

        public DBTableEditor()
        {
            InitializeComponent();
            m_log.Log(TimeLog.enumLogType.ltstart, "���ݱ�༭��", "����ģ������", null);
        }
        

        private void DBTableEditor_Load(object sender, EventArgs e)
        {
            AllTables = new Hashtable();
            /*
            string sql = "SELECT * FROM sys_export_table_cfg";
            ExportTbl = Helper.GetDataTable(sql, Conn);
             */
            ExportTbl = Helper.GetDataTableProxy("sys_export_table_cfg", Conn);
            /*
            sql = "SELECT * FROM sys_table_cfg";
            TblCfgTbl = Helper.GetDataTable(sql, Conn);
            */

            TblCfgTbl = Helper.GetDataTableProxy("sys_table_cfg", Conn);
            
            /*
            sql = "SELECT name FROM sysobjects WHERE (xtype = 'u') ORDER BY name";
            SqlDataAdapter adp = new SqlDataAdapter(sql, Conn);
            DataSet ds = new DataSet();
            adp.Fill(ds, "sysobjects");
            DataTable dt = ds.Tables[0];
            */

            DataTable dt = Helper.GetDataTableProxy("sysobjects", "xtype = 'u'", "name", Conn);
            foreach (DataRow row in dt.Rows)
            {
                string tblName = row[0].ToString().Trim();
                if(!tblName.StartsWith("sys"))
                {
                    TreeNode tbl_node = this.tree.Nodes.Add(tblName);
                    tbl_node.ImageIndex = 3;
                    tbl_node.SelectedImageIndex = 3;
                    tbl_node.Nodes.Add("��Ч����");
                }
            }

            // ��չ�����
            tree.TopNode.ExpandAll();
            tree.TopNode.Collapse();
        }

        private void ShowFieldInfo(TreeNode node)
        {
            this.groupField.Visible = true;
            this.groupTable.Visible = false;

            string tblName = node.Parent.Text;
            DataTable tbl = ((DataTable)AllTables[tblName]);
            string fieldName = node.Text;
            if (tbl.Columns[fieldName] == null)
                return;
            Type type = tbl.Columns[fieldName].DataType;
            int len = tbl.Columns[fieldName].MaxLength;

            if (type.Name == "String")
            {
                this.combo_data_type.SelectedIndex = 1;
                this.textBox_data_len.Text = Convert.ToString(len);
                this.textBox_data_len.Enabled = true;
                this.textBox_Zhengshu_len.Text = "";
                this.textBox_Zhengshu_len.Enabled = false;
                this.textBox_Xiaoshu_len.Text = "";
                this.textBox_Xiaoshu_len.Enabled = false;
            }
            else if (type.Name == "Decimal")
            {
                this.combo_data_type.SelectedIndex = 0;
                this.textBox_data_len.Text = "";
                this.textBox_data_len.Enabled = false;

                string sql = string.Format("SELECT * FROM syscolumns WHERE name='{0}' AND id=object_id('{1}')", fieldName, tblName);

               
                DataTable table = Helper.GetDataTableWithSqlProxy("syscolumns", sql, Conn);
                DataRow row = table.Rows[0];
                //DataRow row = Helper.GetDataTable(sql, Conn).Rows[0];
                int prec = Convert.ToInt32(row["prec"]);
                int scale = Convert.ToInt32(row["scale"]);

                this.textBox_Zhengshu_len.Text = Convert.ToString(prec);
                this.textBox_Zhengshu_len.Enabled = true;
                this.textBox_Xiaoshu_len.Text = Convert.ToString(scale);
                this.textBox_Xiaoshu_len.Enabled = true;
            }
            else if (type.Name == "Boolean")
            {
                this.combo_data_type.SelectedIndex = 2;
                this.textBox_data_len.Text = "";
                this.textBox_data_len.Enabled = false;
                this.textBox_Zhengshu_len.Text = "";
                this.textBox_Zhengshu_len.Enabled = false;
                this.textBox_Xiaoshu_len.Text = "";
                this.textBox_Xiaoshu_len.Enabled = false;
            }
            else if (type.Name == "Int32")
            {
                this.combo_data_type.SelectedIndex = 3;
                this.textBox_data_len.Text = "";
                this.textBox_data_len.Enabled = false;
                this.textBox_Zhengshu_len.Text = "";
                this.textBox_Zhengshu_len.Enabled = false;
                this.textBox_Xiaoshu_len.Text = "";
                this.textBox_Xiaoshu_len.Enabled = false;
            }
            else
            {
                this.combo_data_type.SelectedIndex = -1;
                this.textBox_data_len.Text = "";
                this.textBox_Zhengshu_len.Text = "";
                this.textBox_Xiaoshu_len.Text = "";
            }
        }

        private void ShowTableInfo(TreeNode node)
        {
            this.groupField.Visible = false;
            this.groupTable.Visible = true;

            this.textBox_rela_path.Text = "";
            this.checkHasHead.Checked = true;
            foreach (DataRow row in ExportTbl.Rows)
            {
                if (row["tablename"].ToString().Trim() == node.Text)
                {
                    this.textBox_rela_path.Text = row["filename"].ToString().Trim();

                    if (row["hastitle"] != DBNull.Value)
                    {
                        this.checkHasHead.Checked = Convert.ToBoolean(row["hastitle"]);
                    }

                    // ��ʾ���������ֶ�
                    this.textBox_Exclude_Field.Text = row["export_ignore"].ToString();

                    break;
                }
            }

            this.checkHasDefault.Checked = true;
            this.comboExportRule.SelectedIndex = 0;
            this.checkBrowseFolder.Checked = false;

            foreach (DataRow row in TblCfgTbl.Rows)
            {
                if (row["tablename"].ToString().Trim() == node.Text)
                {
                    if (row["hasdefaultrow"] != DBNull.Value)
                        this.checkHasDefault.Checked = Convert.ToBoolean(row["hasdefaultrow"]);
                    if (row["savetype"] != DBNull.Value)
                        this.comboExportRule.SelectedIndex = Convert.ToInt32(row["savetype"]);
                    if (row["bBrowseFolderWhileImport"] != DBNull.Value)
                        this.checkBrowseFolder.Checked = Convert.ToBoolean(row["bBrowseFolderWhileImport"]);
                    break;
                }
            }
        }

        private void TreeNodeClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Node != null && (e.Button == MouseButtons.Left || e.Button == MouseButtons.Right))
            {
                if (e.Node.Level == 0) // ��ѡ��
                {
                    if(e.Node.FirstNode != null && e.Node.FirstNode.Text == "��Ч����") // ��Ҫ�����ӽ��
                    {
                        ReloadAttributes(e.Node);
                    }
                    ShowTableInfo(e.Node);
                }
                else if (e.Node.Level == 1) // ��ѡ�ֶ�
                {
                    ShowFieldInfo(e.Node);
                }
            }

            if (e.Node != null && e.Button == MouseButtons.Right)
            {
                this.tree.SelectedNode = e.Node;
                if (e.Node.Level == 0)
                    cTblNodeClick.Popup(Control.MousePosition);
                else if (e.Node.Level == 1)
                    cFieldNodeClick.Popup(Control.MousePosition);
            }
        }

        private void DBTableEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            //foreach (TreeNode node in this.tree.Nodes)
            foreach(TreeNode node in newNodes)
            {
                string tblName = node.Text.Trim();
                DataTable tbl = ((DataTable)AllTables[tblName]);
                if (tbl.Columns.Count == 1)
                {
                    if (tbl.Columns[0].ColumnName == TRASH_FIELD)
                    {
                        string warning = string.Format("���ڿձ�{0}, �ñ��ᱻ���浽���ݿ⡣", tblName);
                        MessageBox.Show(warning);

                        if (Helper.RemoveDbTable(tblName, Conn))
                        {
                            node.Remove();
                            AllTables[tblName] = null;
                        }
                    }
                }
            }

            newNodes.Clear();
            e.Cancel = true;
            Hide();

            m_log.Log(TimeLog.enumLogType.ltend, null, null, null);
        }

        private void OK_button_Click(object sender, EventArgs e) // �޸�
        {
            TreeNode node = this.tree.SelectedNode;

            if (node.Level == 0) // �޸ı���Ϣ
            {
                string tblName = node.Text.Trim();
                bool bSuc = false;
                try
                {
                    // ���� sys_export_table_cfg ��
                    //DataTable tbl_export_tbl_cfg = Helper.GetDataTable("SELECT * FROM sys_export_table_cfg", Conn);

                    DataTable tbl_export_tbl_cfg = Helper.GetDataTableProxy("sys_export_table_cfg", Conn);

                    bool bFindTbl = false;
                    foreach (DataRow row in tbl_export_tbl_cfg.Rows)
                    {
                        if (row["tablename"].ToString().Trim() == tblName)
                        {
                            row["filename"] = this.textBox_rela_path.Text;
                            row["hastitle"] = this.checkHasHead.Checked;
                            row["export_ignore"] = this.textBox_Exclude_Field.Text;

                            bFindTbl = true;
                            break;
                        }
                    }
                    if (!bFindTbl)
                    {
                        DataRow newRow = tbl_export_tbl_cfg.NewRow();
                        newRow.BeginEdit();
                        newRow["tablename"] = tblName;
                        newRow["filename"] = this.textBox_rela_path.Text;
                        newRow["hastitle"] = this.checkHasHead.Checked;
                        newRow.EndEdit();

                        tbl_export_tbl_cfg.Rows.Add(newRow);
                    }

                    /* remoting
                     * SqlDataAdapter adp = new SqlDataAdapter("SELECT * FROM sys_export_table_cfg", Conn);
                    SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                    if (bFindTbl)
                        adp.UpdateCommand = cmdBuilder.GetUpdateCommand();
                    else
                        adp.InsertCommand = cmdBuilder.GetInsertCommand();
                    int val = adp.Update(tbl_export_tbl_cfg);
                    tbl_export_tbl_cfg.AcceptChanges();
                    */

                    int val = Helper.SaveTable(tbl_export_tbl_cfg, "SELECT * FROM sys_export_table_cfg", Conn);

                    // ���� sys_table_cfg
                    //DataTable tbl_tbl_cfg = Helper.GetDataTable("SELECT * FROM sys_table_cfg", Conn);

                    DataTable tbl_tbl_cfg = Helper.GetDataTableProxy("sys_table_cfg", Conn);

                    bFindTbl = false;
                    foreach (DataRow row in tbl_tbl_cfg.Rows)
                    {
                        if (row["tablename"].ToString().Trim() == tblName)
                        {
                            row["hasdefaultrow"] = this.checkHasDefault.Checked;
                            row["savetype"] = this.comboExportRule.SelectedIndex;
                            bFindTbl = true;
                            break;
                        }
                    }
                    if (!bFindTbl)
                    {
                        DataRow newRow = tbl_tbl_cfg.NewRow();
                        newRow.BeginEdit();
                        newRow["tablename"] = tblName;
                        newRow["hasdefaultrow"] = this.checkHasDefault.Checked;
                        newRow["savetype"] = this.comboExportRule.SelectedIndex;
                        newRow.EndEdit();

                        tbl_tbl_cfg.Rows.Add(newRow);
                    }
                    /* remoting
                    SqlDataAdapter adp2 = new SqlDataAdapter("SELECT * FROM sys_table_cfg", Conn);
                    SqlCommandBuilder cmdBuilder2 = new SqlCommandBuilder(adp2);
                    if (bFindTbl)
                        adp2.UpdateCommand = cmdBuilder2.GetUpdateCommand();
                    else
                        adp2.InsertCommand = cmdBuilder2.GetInsertCommand();
                    val = adp2.Update(tbl_tbl_cfg);
                    tbl_tbl_cfg.AcceptChanges();
                    */

                    val = Helper.SaveTable(tbl_tbl_cfg, "SELECT * FROM sys_table_cfg", Conn);
                     
                    bSuc = true;
                }
                catch (Exception ex)
                {
                    string err = string.Format("���޸ı� {0} ����Ϣʱ��������\r\n\r\n������ϢΪ\r\n{1}", tblName, ex.Message);
                    MessageBox.Show(err);
                }

                if (bSuc)
                {
                    //ExportTbl = Helper.GetDataTable("SELECT * FROM sys_export_table_cfg", Conn);
                    //TblCfgTbl = Helper.GetDataTable("SELECT * FROM sys_table_cfg", Conn);
                    ExportTbl = Helper.GetDataTableProxy("sys_export_table_cfg", Conn);
                    TblCfgTbl = Helper.GetDataTableProxy("sys_table_cfg", Conn);
                }
            }

            else if (node.Level == 1) // �޸��ֶ���Ϣ
            {
                bool fieldIsKey = false;
                string tblName = node.Parent.Text.Trim();
                string fieldName = node.Text.Trim();

                string dataType = "";
                string len = "";
                int prec = -1;
                if (this.textBox_Zhengshu_len.Text.Trim() != "")
                    prec = Convert.ToInt32(this.textBox_Zhengshu_len.Text.Trim());
                else
                    prec = 0;
                int scale = -1;
                if (this.textBox_Xiaoshu_len.Text.Trim() != "")
                    scale = Convert.ToInt32(this.textBox_Xiaoshu_len.Text.Trim());
                else
                    scale = 0;
                switch (this.combo_data_type.SelectedIndex)
                {
                    case 0:
                        dataType = "decimal";
                        len = string.Format("({0},{1})", prec, scale); // ������, ���� "(18,0)" .
                        break;
                    case 1:
                        dataType = "varchar";
                        len = "(" + textBox_data_len.Text.Trim() + ")"; // ������
                        if (len == "()")
                        {
                            MessageBox.Show("��ָ���ַ����ĳ���.");
                            return;
                        }
                        break;
                    case 2:
                        dataType = "bit";
                        len = "";
                        break;
                    case 3:
                        dataType = "int";
                        len = "";
                        break;
                    default:
                        MessageBox.Show("��ѡ����������.");
                        return;
                }
                string can_be_null = "";
                if (Helper.FieldIsTableKey(fieldName, ((DataTable)AllTables[tblName])))
                {
                    fieldIsKey = true;
                    can_be_null = "NOT";
                }
                else
                {
                    fieldIsKey = false;
                    can_be_null = "";
                }

                string sql = string.Format("ALTER TABLE {0} ALTER COLUMN [{1}] {2}{3} {4} NULL",
                    tblName, fieldName, dataType, len, can_be_null);

                bool bSuc = false;
                Conn.Open();
                try
                {
                    /* remoting
                    SqlCommand cmd = Conn.CreateCommand();
                    cmd.CommandText = sql;
                    cmd.ExecuteNonQuery();
                    */

                    Helper.ExecuteCommand(tblName, sql, Conn);

                    bSuc = true;
                }
                catch (Exception ex)
                {
                    string err = string.Format("���޸��ֶ� {0} ʱ��������\r\n\r\n������Ϣ��{1}", fieldName, ex.Message);
                    MessageBox.Show(err);
                }
                Conn.Close();

                if (bSuc)
                {
                    if (fieldIsKey)
                        this.tree.SelectedNode.StateImageIndex = 0;
                    else if (dataType == "varchar" && len == "(3000)")
                        this.tree.SelectedNode.StateImageIndex = 7;
                    else
                        this.tree.SelectedNode.StateImageIndex = -1;

                    //DataTable tbl = Helper.GetDataTable("SELECT * FROM " + tblName, Conn);
                    DataTable tbl = Helper.GetDataTableProxy(tblName, Conn);
                    AllTables[tblName] = tbl;
                }
            }
        }

        private void Cancel_button_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void Tree_AfterLabelEdit(object sender, NodeLabelEditEventArgs e) // ������
        {
            string oldName = e.Node.Text.Trim();
            string newName = "";
            if (e.Label != null)
                newName = e.Label.Trim();
            else
                newName = e.Node.Text.Trim();

            if (oldName == newName) // ʵ��û�и���������
                return;

            if (e.Node.Level == 0) // ��������
            {
                bool bRenameSuc = false;
                Conn.Open();
                try
                {
                    bRenameSuc = Helper.RenameDbTable(oldName, newName, Conn);
                }
                catch (Exception ex)
                {
                    string str = string.Format("���������� {0} ʱ��������\r\n\r\n������Ϣ��{1}", oldName, ex.Message);
                    MessageBox.Show(str);
                }
                Conn.Close();

                if (bRenameSuc)
                {
                    DataTable tbl_ren = ((DataTable)AllTables[oldName]);
                    AllTables[oldName] = null;
                    AllTables[newName] = tbl_ren;
                }
            }

            else if (e.Node.Level == 1) // �������ֶ�
            {
                string tblname = e.Node.Parent.Text.Trim();
                bool bRenameSuc = false;
                Conn.Open();
                try
                {
                    bRenameSuc = Helper.RenameDbField(tblname, oldName, newName, Conn);
                }
                catch (Exception ex)
                {
                    string str = string.Format("���������ֶ� {0} ʱ��������\r\n\r\n������Ϣ��{1}", tblname + "." + oldName, ex.Message);
                    MessageBox.Show(str);
                }
                Conn.Close();

                if (bRenameSuc)
                {
                    //DataTable tbl_ren = Helper.GetDataTable("SELECT * FROM " + tblname, Conn);
                    DataTable tbl_ren = Helper.GetDataTableProxy(tblname, Conn);
                    AllTables[tblname] = tbl_ren;
                }
            }
        }

        private void bNewTbl_Click(object sender, EventArgs e) // �½���
        {
            string newTblName = "";
            NameInput frm = new NameInput();
            DialogResult result = frm.ShowDialog();
            if (result == DialogResult.OK)
            {
                newTblName = frm.StringInput;
            }
            if (newTblName == "")
            {
                return;
            }

            bool bSucCreate = false;
            Conn.Open();
            try
            {
                string strcmd = string.Format("CREATE TABLE {0} ({1} varchar(3000))", newTblName, TRASH_FIELD);
                /* remoting
                SqlCommand cmd = Conn.CreateCommand();
               
                cmd.CommandText = strcmd;
                cmd.ExecuteNonQuery();
                 */

                Helper.ExecuteCommand(newTblName, strcmd, Conn);
                bSucCreate = true;
            }
            catch (Exception ex)
            {
                string str = string.Format("����ӱ� {0} ʱ��������\r\n\r\n������Ϣ��{1}", newTblName, ex.Message);
                MessageBox.Show(str);
            }
            Conn.Close();

            if (bSucCreate)
            {
                //DataTable new_tbl = Helper.GetDataTable("SELECT * FROM " + newTblName, Conn);
                DataTable new_tbl = Helper.GetDataTableProxy(newTblName, Conn);

                AllTables[newTblName] = new_tbl;

                TreeNode new_node = this.tree.Nodes.Add(newTblName);
                new_node.ImageIndex = 3;
                new_node.SelectedImageIndex = 3;

                this.tree.SelectedNode = new_node;
                ShowTableInfo(new_node);
                newNodes.Add(new_node);
            }

            // ��ʾ�½���Ľ��
            tree.TopNode.ExpandAll();
            tree.TopNode.Collapse();
        }

        private void bDelTbl_Click(object sender, EventArgs e) // ɾ����
        {
            string tbl_del = this.tree.SelectedNode.Text;
            string confirm = string.Format("��ȷ��Ҫɾ�������� {0} ��?", tbl_del);
            DialogResult result = MessageBox.Show(confirm, "����", MessageBoxButtons.YesNo);
            if (result == DialogResult.No)
                return;

            // ɾ���������м�¼�������
            TreeNode currentNode = this.tree.SelectedNode;
            if(newNodes.Contains(currentNode))
            {
                newNodes.Remove(currentNode);
            }

            if (Helper.RemoveDbTable(tbl_del, Conn))
            {
                this.tree.SelectedNode.Remove();
                AllTables[tbl_del] = null;
            }
        }

        private void bRenameTbl_Click(object sender, EventArgs e) // ��������
        {
            TreeNode theNode = this.tree.SelectedNode;
            if (!theNode.IsEditing)
            {
                this.tree.LabelEdit = true;
                theNode.BeginEdit();
            }
        }

        private bool IsRowIDAcceptable(ref string[] strFields, ref string[] strColnames, string strKeyField, int iBeginNum, int iEndNum)
        {
            for (int i = 0; i < strColnames.Length; i++)
            {
                if (strColnames[i].Trim() == strKeyField)
                {
                    int id = Convert.ToInt32(strFields[i].Trim());
                    if (id >= iBeginNum && id <= iEndNum)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            MessageBox.Show("�޷��ж�ĳ���Ƿ�Ӧ�ñ����룬��ͨ������Ϊָ���˴�����������");
            return false;
        }

        /// <summary>
        /// ���¼��ر��е�����
        /// </summary>
        /// <param name="node">���Ӧ�������</param>
        private void ReloadAttributes(TreeNode node)
        {
            string tblName = node.Text;
            //string sql = "SELECT TOP 1 * FROM " + tblName;
            //DataTable tbl = Helper.GetDataTable(sql, Conn);

            DataTable tbl = Helper.GetDataTableTopRowProxy(tblName, Conn);

            AllTables[tblName] = tbl;

            node.Nodes.Clear();
            foreach (DataColumn col in tbl.Columns)
            {
                TreeNode field_node = node.Nodes.Add(col.ColumnName.Trim());
                field_node.ImageIndex = 4;
                field_node.SelectedImageIndex = 4;

                if (Helper.FieldIsTableKey(col, tbl))
                {
                    field_node.StateImageIndex = 0;
                }
                else if (col.DataType.Name.Trim() == "String" && col.MaxLength == 3000)
                {
                    field_node.StateImageIndex = 7;
                }
            }
        }


        private bool WriteRow(ref DataTable tbl, ref DataRow row, ref string[] strColNames, ref string[] strFields)
        {
            try
            {
                for (int i = 0; i < strColNames.Length; i++)
                {
                    if (i < strFields.Length)
                    {
                        if (strFields[i] == "")
                            row[strColNames[i]] = DBNull.Value;
                        else
                        {
                            if (tbl.Columns[strColNames[i]].DataType.Name == "Boolean")
                            {
                                if (strFields[i].Trim() == "0")
                                    row[strColNames[i]] = false;
                                else
                                    row[strColNames[i]] = true;
                            }
                            else
                            {
                                try
                                {
                                    row[strColNames[i]] = strFields[i];
                                }
                                catch (Exception ex)
                                {
                                    string err = string.Format("���޸ĵ� {0} �������� (�ֶ���: {1}; �ֶ�ֵ: {2}) ʱ��������, ���޸����ݱ�����µ��롣\r\n\r\n������Ϣ��{3}", i, strColNames[i], strFields[i], ex.Message);
                                    MessageBox.Show(err);
                                    return false;
                                }
                            }
                        }
                    }
                    else
                    {
                        row[strColNames[i]] = DBNull.Value;
                    }
                }
            }
            catch//Exception ex)
            {
                return false;
            }
            
            return true;
        }

        /// <summary>
        /// ����lua�ű�ͬʱ������ļ�
        /// </summary>
        /// <param name="lua">lua����</param>
        /// <param name="path">�����·��</param>
        /// <param name="tableName">���ݱ���</param>
        private object[] CallLuaLoadMultiFile(LuaEx lua, string path, string tableName)
        {
            //LuaEx lua = new LuaEx();
            String luafile = Path.GetDirectoryName(Process.GetCurrentProcess().MainModule.FileName) + @"\import.lua";
            if (!File.Exists(luafile))
                return null;
            try
            {
                lua.DoFile(luafile);
                LuaFunction fun = lua.GetFunction("loadmultifile");
                if (fun != null)
                {
                    object[] retobjs = fun.Call(path, tableName);

                    return retobjs;
                }
                return null;
            }
            catch (Exception ex)
            {
                Helper.AddLog(ex.Message);
                throw;
            }    
        }

        private bool CallLuaImport(LuaEx lua, string strTableName, string filename, ref string[] fields, ref string[] values)
        {
            //LuaEx lua = new LuaEx();
            lua["Conn"] = m_conn; // ע��sql����
            String luafile = Path.GetDirectoryName(Process.GetCurrentProcess().MainModule.FileName) + @"\import.lua";
            if (!File.Exists(luafile))
                return false;
            try
            {
                lua.DoFile(luafile);
                LuaFunction fun = lua.GetFunction("onimport");
                if (fun != null)
                {
                    ImportInfoHelper helper = new ImportInfoHelper();
                    ArrayList feildlist = new ArrayList();
                    if (fields != null)
                        feildlist.AddRange(fields);

                    helper.Fields = feildlist;
                    helper.Values = values;
                    object[] retobjs = fun.Call(strTableName, Path.GetDirectoryName(filename), filename, helper);

                    //fields = (string[])feildlist.ToArray(typeof(string));
                    fields = (string[])(helper.Fields).ToArray(typeof(string));
                    values = helper.Values;

                    if (retobjs != null && retobjs.GetLength(0) > 0)
                    {
                        bool ret = (bool)retobjs[0];
                        return ret;
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
                throw;
            }
            return false;
        }

        private void bImportTbl_Click(object sender, EventArgs e) // �����
        {
            if (Program.m_bLockDBForCompetition)
            {
                MessageBox.Show("���������ˣ��벻Ҫ����");
                return;
            }

            //////////////////////////////////////////////////////////////////////////
            // ׼������
            //////////////////////////////////////////////////////////////////////////
            string[] strLines = null;
            string[] strColNames = null;
            string tblName = this.tree.SelectedNode.Text.Trim();
            string rootPath = Program.RootDir; // ����ĸ�Ŀ¼
            string strFile = ""; // ������ļ�·��

            // �жϸ��û��ܲ��ܵ�����
            if (Program.m_bUseNewAcl)
            {
                try
                {
                    //DataTable tblExportTable = Helper.GetDataTable("SELECT id FROM sys_export_table_cfg WHERE tablename = '" + tblName + "' ", Conn);

                    DataTable tblExportTable = Helper.GetDataTableProxy("sys_export_table_cfg", "tablename = '" + tblName + "' ", null, Conn);

                    if (tblExportTable.Rows.Count > 0)
                    {
                        int tbl_id = Convert.ToInt32(tblExportTable.Rows[0]["id"]);
                        //DataTable tblExportModlCfg = Helper.GetDataTable("SELECT modid FROM sys_export_modl_cfg WHERE tableid = " + tbl_id.ToString() + " ", Conn);
                        DataTable tblExportModlCfg = Helper.GetDataTableProxy("sys_export_modl_cfg", "tableid = " + tbl_id.ToString(), null, Conn);
                        //if (tblExportModlCfg.Rows.Count > 0)
                        //{
                        //    int moduleid = Convert.ToInt32(tblExportModlCfg.Rows[0][0]);
                        //    string _sql = string.Format("SELECT * FROM sys_useracl_desc WHERE userindex = {0} AND moduleid = {1} AND canimport = 1", Program.m_rightmgr._userindex, moduleid);
                        //    bool can_import = Helper.GetDataTable(_sql, Conn).Rows.Count > 0;
                        //    if (Program.m_bUseNewAcl && !can_import)
                        //    {
                        //        MessageBox.Show("�����ڵ�ǰ���ݱ� " + tblName + " û�е���Ȩ�ޣ������뿪ͨ��", "Ȩ�޲���", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        //        return;
                        //    }
                        //}
                        if (Program.m_bUseNewAcl)
                        {
                            bool can_import = false;
                            foreach (DataRow rowex in tblExportModlCfg.Rows)
                            {
                                int moduleid = Convert.ToInt32(rowex["modid"]);
                                //string _sql = string.Format("SELECT * FROM sys_useracl_desc WHERE userindex = {0} AND moduleid = {1} AND canimport = 1", Program.m_rightmgr._userindex, moduleid);
                                string filter = string.Format("userindex = {0} AND moduleid = {1} AND canimport = 1", Program.m_rightmgr._userindex, moduleid);
                                DataTable table = Helper.GetDataTableProxy("sys_useracl_desc", filter, null, Conn);
                                bool _b = table.Rows.Count > 0;
                                //bool _b = Helper.GetDataTable(_sql, Conn).Rows.Count > 0;
                                if (_b)
                                {
                                    can_import = true;
                                    break;
                                }
                            }

                            // ���һ�ж�û�У�֤�������������κ�һ���༭ģ�����ô������
                            if (tblExportModlCfg.Rows.Count == 0)
                                can_import = true;

                            if (!can_import)
                            {
                                MessageBox.Show("�����ڵ�ǰ���ݱ� " + tblName + " û�е���Ȩ�ޣ������뿪ͨ��", "Ȩ�޲���", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                                return;
                            }
                        }
                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show("�ñ��޷����롣\r\n������ϢΪ��" + ex.Message, "�޷�����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }
            
            bool bBrowseFolder = false;
            bool jump = false; // �Ƿ�������tab��text�ļ����ַ�����
            LuaEx lua = new LuaEx();
            lua["Conn"] = Conn;
            lua.DoString("function trace(txt)\r\n    MainForm.LogEditBox.Visible = true\r\n    MainForm.LogEditBox.Text = MainForm.LogEditBox.Text ..txt..'\\r\\n' \r\n end");
            object[] results = null;
            try
            {
                results = CallLuaLoadMultiFile(lua, rootPath, tblName);
            }
            catch (Exception ex)
            {
            	MessageBox.Show("�ű����� CallLuaLoadMultiFile() �쳣������ import.lua �ļ���\r\n\r\n������Ϣ��" + ex.Message, "����ʧ��", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (results != null && results[0] is bool && (bool)results[0] && results.Length >= 3)
            { // �ű����óɹ�
                strColNames = results[1] as string[];
                strLines = results[2] as string[];
                jump = true;
            }
            else
            { // �ű�ʧ�ܻ򲻵���, ��ԭ�ȵ�������ѡ�񵥸��ļ� ���� �ļ���
                /*
                string s = string.Format("SELECT * FROM sys_table_cfg WHERE tablename='{0}'", tblName);
                DataTable tbl_cfg = Helper.GetDataTable(s, Conn);
                */

                string filter = string.Format("tablename='{0}'", tblName);
                DataTable tbl_cfg = Helper.GetDataTableProxy("sys_table_cfg", filter, null, Conn);

                if (tbl_cfg.Rows.Count == 1 && tbl_cfg.Rows[0]["bBrowseFolderWhileImport"] != DBNull.Value && Convert.ToBoolean(tbl_cfg.Rows[0]["bBrowseFolderWhileImport"]) == true)
                    bBrowseFolder = true;

                if (bBrowseFolder)
                    strFile = FileFolderHelper.BrowserFolder("��ѡ�� Client/data/source/maps Ŀ¼��");
                else
                    strFile = FileFolderHelper.BrowserFile(new string[] { "tab", "txt" });

                if (strFile == "") // �û��Ƿ�ѡ���ļ��ļ��ŵ���������
                    return;
            }
             
            //DataTable tbl = Helper.GetDataTable("SELECT * FROM " + tblName, Conn);
            DataTable tbl = Helper.GetDataTableProxy(tblName, Conn);

            List<string> newNodeList = new List<string>(); // �����ӵ��������������
            List<string> newColumnList = new List<string>(); // �����ӵ��е���������
            List<DataRow> overwriteRowList = new List<DataRow>(); // ����д�������е�����
            bool deleteMe = false; // �Ƿ���___delete_me�ֶ�
            int rowNumber = tbl.Rows.Count; // ���ݱ������
            int iOverwriteCount = 0; // ���ǵ�����
            int iAddCount = 0; // ����������
            
            //TimeLog log = new TimeLog();
            //log.Log(TimeLog.enumLogType.ltstart, "���ݱ���", "��������", string.Format("�����ļ���:{0}, ����Ŀ�����ݱ�:{1}", strFile, tblName));          

            bool bHasTitle = true;
            if(jump)
            {
                goto label0;
            }

            // split
            string strContent = FileFolderHelper.FileToString(strFile);
            if (!bBrowseFolder && strContent == string.Empty)
            {
                MessageBox.Show("���ܶ�ȡ�ļ����ݣ������ļ����ڱ���������򿪡�", "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            
            strLines = strContent.Split(new char[2] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

            // �Ƿ��б�ͷ
            /*
            string sql = string.Format("SELECT * FROM sys_export_table_cfg WHERE tablename='{0}'", tblName);
            DataTable tbl_exp = Helper.GetDataTable(sql, Conn);
             */

            string _filter = string.Format("tablename='{0}'", tblName);
            DataTable tbl_exp = Helper.GetDataTableProxy("sys_export_table_cfg", _filter, null, Conn);
            if (tbl_exp.Rows.Count > 0 && tbl_exp.Rows[0]["hastitle"] != DBNull.Value && Convert.ToBoolean(tbl_exp.Rows[0]["hastitle"]) == false)
            {
                bHasTitle = false;
            }

            if (!bBrowseFolder && bHasTitle && strLines.Length < 2)
                return;
            
            if (!bBrowseFolder)
                if (bHasTitle)
                {
                    strColNames = strLines[0].Split(new char[1] { '\t' }, StringSplitOptions.RemoveEmptyEntries);
                    for (int i = 0; i < strColNames.Length; i++)
                    {
                        strColNames[i] = strColNames[i].Trim();
                    }
                }
                else // û��ͷ(����), �Լ���
                {
                    int col_count = strLines[0].Split(new char[1] { '\t' }).Length;
                    strColNames = new string[col_count];
                    for (int i = 0; i < col_count; i++)
                    {
                        strColNames[i] = string.Format("Field_{0}", i);
                    }
                }

            //////////////////////////////////////////////////////////////////////////
            // ѯ���Ƿ�ȫ������
            //////////////////////////////////////////////////////////////////////////

            label0:

            bool bImportAllRow = true;
            string strKeyField = "";
            int iBeginNum = -1;
            int iEndNum = -1;

            ImportInfo imp_frm = new ImportInfo(strColNames);
            DialogResult imp_r = imp_frm.ShowDialog();

            if (imp_r == DialogResult.Cancel)
            {
                return;
            }

            if (imp_r == DialogResult.OK)
            {
                bImportAllRow = imp_frm.All;
                strKeyField = imp_frm.KeyField;
                iBeginNum = imp_frm.BeginNum;
                iEndNum = imp_frm.EndNum;
            }

            if(!jump) // �������ת�����ģ��Ͳ�������lua�ű�������
            {
                lua = new LuaEx();
                lua["Conn"] = m_conn;
                lua.DoString("function trace(txt)\r\n    MainForm.LogEditBox.Visible = true\r\n    MainForm.LogEditBox.Text = MainForm.LogEditBox.Text ..txt..'\\r\\n' \r\n end");
                try
                {
                    if (CallLuaImport(lua, tblName, strFile, ref strColNames, ref strLines))
                        return;
                }
                catch//(Exception ex)
                {
                    return;
                }
            }        

            // log & ftp backup
            TimeLog log = new TimeLog();
            string backupName = Helper.GetHostName() + "_" + Convert.ToString(DateTime.Now);
            backupName = backupName.Replace(':', '-');
            string importParams = string.Format("bImportAllRow:{0} strKeyField:{1} iBeginNum:{2} iEndNum:{3}", bImportAllRow, strKeyField, iBeginNum, iEndNum);
            log.Log(TimeLog.enumLogType.ltstart, "���ݱ���", "��������", string.Format("�����ļ���:{0}, ����Ŀ�����ݱ�:{1}, �������:{2}, ������:{3}", strFile, tblName, importParams, backupName));
            if (File.Exists(strFile))
                Program.m_ftp.Upload(strFile, Program.m_ftp_basedir + backupName);


            try // ��ʼ����ʱ����в�������ʵ��д�����ݿ⣬���Ժ������в������쳣
            {
                // ɾ��___delete_me�ֶ�
                if (tbl.Columns.Contains(TRASH_FIELD))
                {
                    deleteMe = true;
                    tbl.Columns.Remove(TRASH_FIELD);
                }

                // ����ֶ�
                foreach (string colname in strColNames)
                {
                    if (!tbl.Columns.Contains(colname)) // ����δ�ڱ���
                    {
                        DataColumn c = tbl.Columns.Add(colname);
                        c.MaxLength = 3000; // ������󳤶�
                        newColumnList.Add(colname);
                    }
                }

                // ���������
                int key_col_index = -1;
                if (!bImportAllRow)
                {
                    for (int i = 0; i < strColNames.Length; i++)
                    {
                        if (strColNames[i].Trim() == strKeyField)
                        {
                            key_col_index = i;
                            break;
                        }
                    }
                    if (key_col_index == -1)
                    {
                        MessageBox.Show("�޷��ж�ĳ���Ƿ�Ӧ�ñ����룬��ͨ������Ϊָ���˴�����������\r\n\r\n����ʧ�ܡ�",
                            "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                }

                // ��ȡ����, �����Ƿ����±�
                bool bFirstLine = true;
                bool bYesToAll = false;
                bool bNoToAll = false;
                bool bTblHasPrimKey = (tbl.PrimaryKey.Length != 0);

                foreach (string eachLine in strLines)
                {
                    // ��һ�����ֶ���; ���
                    if (bHasTitle)
                    {
                        if (bFirstLine)
                        {
                            bFirstLine = false;
                            continue;
                        }
                    }
                    string[] strFields = eachLine.Split(new char[1] { '\t' });
                    if (eachLine.Split(new char[1] {'\t'}, StringSplitOptions.RemoveEmptyEntries).Length == 0)
                    {
                        MessageBox.Show("�����ļ����п��У����顣");
                        return;
                    }

                    // �ж��к��Ƿ���Ҫ����ķ�Χ��
                    bool bRowAcceptable = true;
                    if (bImportAllRow)
                    {
                        bRowAcceptable = true;
                    }
                    else
                    {
                        try
                        {
                            int id = Convert.ToInt32(strFields[key_col_index].Trim());
                            bRowAcceptable = bImportAllRow || (id >= iBeginNum && id <= iEndNum);
                        }
                        catch (Exception ex)
                        {
                            string err = string.Format("�޷��ж�ĳ���Ƿ�Ӧ�ñ����룬��ͨ������Ϊָ���˴�����������\r\n\r\n������ϢΪ��{0}", ex.Message);
                            MessageBox.Show(err, "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        }
                    }

                    if (!bRowAcceptable)
                        continue;

                    // ��ʼ����
                    if (!bTblHasPrimKey) // û����key�����±������ȫ����
                    {
                        DataRow newRow = tbl.NewRow();
                        newRow.BeginEdit();

                        if (!WriteRow(ref tbl, ref newRow, ref strColNames, ref strFields))
                        {
                            MessageBox.Show("����ʧ�ܣ������������ݽ���������!",
                                "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        }

                        tbl.Rows.Add(newRow);
                        newRow.EndEdit();

                        iAddCount++;
                    }

                    else // �Ǿɱ���Ҫ����key�ж�ԭ���Ƿ���ڸ���
                    {
                        string strAsking = "�Ѿ����� ";

                        // format key values to find
                        object[] objKeyValues = new object[tbl.PrimaryKey.Length];
                        int iKeyIndex = 0;
                        foreach (DataColumn col in tbl.PrimaryKey)
                        {
                            string primKeyToFind = col.ColumnName;
                            for (int i = 0; i < strColNames.Length; i++)
                            {
                                if (strColNames[i].Trim().ToLower() == primKeyToFind.Trim().ToLower())
                                {
                                    if (strFields[i] == string.Empty)
                                    {
                                        string strError = string.Format("�ڵ���� {0} ʱ����ĳЩ����Ϊ�յ��С�\r\n����Դ�ļ� {1}��", tblName, strFile);
                                        MessageBox.Show(strError, "ʧ��", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                        return;
                                    }

                                    objKeyValues[iKeyIndex++] = strFields[i];
                                    strAsking += primKeyToFind;
                                    strAsking += "Ϊ";
                                    strAsking += strFields[i];
                                    strAsking += " ";
                                }
                            }
                        }
                        strAsking += "���С�";

                        // find it
                        DataRow row = tbl.Rows.Find(objKeyValues);
                        if (row != null) // ����ͬkey���У�ѯ��
                        {
                            if (bYesToAll)
                                goto label_overwrite_it;

                            if (bNoToAll)
                                continue;

                            // ѯ���û�
                            TabImportComfirming cfmForm = new TabImportComfirming(strAsking);
                            DialogResult result = cfmForm.ShowDialog();
                            switch (result)
                            {
                                case DialogResult.Yes:
                                    goto label_overwrite_it;
                                case DialogResult.OK:
                                    bYesToAll = true;
                                    goto label_overwrite_it;
                                case DialogResult.No:
                                    continue;
                                case DialogResult.Ignore:
                                    bNoToAll = true;
                                    continue;
                                case DialogResult.Cancel:
                                    return;
                                default:
                                    break;
                            }

                        label_overwrite_it: // ����������
                            if (!WriteRow(ref tbl, ref row, ref strColNames, ref strFields))
                            {
                                MessageBox.Show("����ʧ�ܣ������������ݽ���������!",
                                    "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }

                            overwriteRowList.Add(row); // �������д������
                            iOverwriteCount++;
                        }

                        // ��������
                        else
                        {
                            DataRow newRow = tbl.NewRow();
                            newRow.BeginEdit();

                            if (!WriteRow(ref tbl, ref newRow, ref strColNames, ref strFields))
                            {
                                MessageBox.Show("����ʧ�ܣ������������ݽ���������!",
                                    "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }

                            tbl.Rows.Add(newRow);
                            newRow.EndEdit();

                            iAddCount++;
                        }
                    }
                }

                // �������Ĭ��ֵ��
                if (MainForm.m_bModifyDbTableDefault)
                {
                    if (bTblHasPrimKey)
                    {
                        DialogResult result = MessageBox.Show("��ϣ���Ѹñ��еĿ���������Ӧ��Ĭ��ֵ��\r\n\r\n�Ƽ���ѡ < ��(Y) >��", "��ʾ", MessageBoxButtons.YesNo);
                        if (result == DialogResult.Yes)
                        {
                            try
                            {
                                Helper.FillDBNullWithDefault(ref tbl);
                            }
                            catch (SqlException ex)
                            {
                                MessageBox.Show("�����п�������Ĭ��ֵʱ�����쳣: " + ex.ToString() + "\r\n\r\n�������ݽ���������",
                                    "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                        }
                    }
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("��ȡ������ʱ�����쳣: " + ex.ToString() + "\r\n\r\n�������ݽ���������",
                                    "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // �ύ 
            if(Conn.State == ConnectionState.Closed) // ��sql����
            {
                Conn.Open();
            }

            // �������ݿ�ϵͳ�����
            /* remoting
            SqlCommand cfg_cmd = Conn.CreateCommand();
            cfg_cmd.CommandText = "sp_configure 'allow updates', '1'";
            cfg_cmd.ExecuteNonQuery();
            cfg_cmd.CommandText = "reconfigure with override";
            cfg_cmd.ExecuteNonQuery();
            */

            Helper.ExecuteCommand("sp_configure 'allow updates', '1'", Conn);
            Helper.ExecuteCommand("reconfigure with override", Conn);

            /*
            SqlTransaction transaction = Conn.BeginTransaction(); //** ��������
            SqlCommand cmd = new SqlCommand(); // �½�sql����
            cmd.Connection = Conn;
            cmd.Transaction = transaction; // ���������
            */

            DataTableProxy proxy = null;
            if (Program.EnableRemoting)
            {
                proxy = DataTableServerProxy.GetDataTableProxy();
                proxy.DeleteRemotingTable(tblName);
            }

            int nID = Helper.BeginTransaction(Conn);
            

            int col_index = 0; // �����
            int index = 0; // �м����α�

            List<string> strColNameList = new List<string>(); // ��¼��������������
            foreach(string s in strColNames)
            {
                strColNameList.Add(s);
            }

            try
            {
                // ����ֶ�
                foreach (string colname in newColumnList)
                {
                    string columnName = colname.Replace("]", "]]"); // �滻�����ַ�
                    /* remoting
                    cmd.CommandText = string.Format("ALTER TABLE {0} ADD [{1}] [varchar] (3000) NULL", tblName, columnName);
                    cmd.ExecuteNonQuery();  
                    */

                    string sql = string.Format("ALTER TABLE {0} ADD [{1}] [varchar] (3000) NULL", tblName, columnName);
                    Helper.ExecuteTransaction(tblName, sql, nID);
                     
                    newNodeList.Add(colname);            
                }

                // ɾ��___delete_me�ֶ�
                if(deleteMe)
                {
                    /* remoting
                    cmd.CommandText = string.Format("ALTER TABLE {0} DROP COLUMN {1}", tblName, TRASH_FIELD);
                    cmd.ExecuteNonQuery();
                     */
                    string sql = string.Format("ALTER TABLE {0} DROP COLUMN {1}", tblName, TRASH_FIELD);
                    Helper.ExecuteTransaction(tblName, sql, nID);

                }

                // ����Ƿ����е�˳�����˱仯
                bool sameOrder = true; // ��˳���Ƿ�仯�ı��
                for (int i = 0; i < strColNames.Length; i++)
                {
                    if (strColNames[i] != tbl.Columns[i].ColumnName)
                    {
                        sameOrder = false;
                        break;
                    }
                }

                if (!sameOrder) // �����е�˳�����˱仯����Ҫ������
                {
                    // ��Ϊϵͳ��������������ܻ�����������⣬������������������ݣ������������¼���
                    /*
                    cmd.CommandText = string.Format("DELETE FROM {0}", tblName);
                    cmd.ExecuteNonQuery();
                    */

                    string _sql = string.Format("DELETE FROM {0}", tblName);
                    Helper.ExecuteTransaction(tblName, _sql, nID);

                    foreach (DataColumn c in tbl.Columns) // ������colid��1000��Ϊ��������׼��
                    {
                        col_index++;
                        //cmd.CommandText = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", col_index + 1000, c.ColumnName, tblName);
                        //cmd.ExecuteNonQuery();
                        string sql = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", col_index + 1000, c.ColumnName, tblName);
                        Helper.ExecuteTransaction("syscolumns", sql, nID);
                    }

                    col_index = 0;

                    foreach (string colname in strColNames) // ����������
                    {
                        col_index++;
                        //cmd.CommandText = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", col_index, colname, tblName);
                        //cmd.ExecuteNonQuery();

                        string sql = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", col_index, colname, tblName);
                        Helper.ExecuteTransaction("syscolumns", sql, nID);
                    }

                    foreach (DataColumn c in tbl.Columns) // ����������
                    {
                        string colname = c.ColumnName;
                        if (!strColNameList.Contains(colname))
                        {
                            col_index++;
                            //cmd.CommandText = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", col_index, colname, tblName);
                            //cmd.ExecuteNonQuery();
                            string sql = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", col_index, colname, tblName);
                            Helper.ExecuteTransaction("syscolumns", sql, nID);

                        }
                    }

                    // �ָ�����Լ�� ����������ܸı���ԭ�ȵ�����Լ��
                    DataColumn[] primaryKeys = tbl.PrimaryKey;
                    if (primaryKeys.Length > 0) // ��������Լ��
                    {
                        string keyString = string.Empty;
                        foreach (DataColumn c in primaryKeys)
                        {
                            keyString = keyString + "[" + c.ColumnName + "]" + ",";
                        }

                        // ��ȡ����Լ��
                        
                        ////cmd.CommandText = string.Format("SELECT name FROM sysindexes WHERE id = OBJECT_ID('{0}') AND name LIKE 'PK%'", tblName);
                        //string filter = string.Format("id = OBJECT_ID('{0}') AND name LIKE 'PK%'", tblName);
                        string sql = string.Format("SELECT name FROM sysindexes WHERE id = OBJECT_ID('{0}') AND name LIKE 'PK%'", tblName);

                        object constraint = Helper.ExecuteTransactionScalar("sysindexes", sql, nID, false);


                        // ɾ������Լ��
                        /*cmd.CommandText = string.Format("ALTER TABLE {0} DROP CONSTRAINT {1}", tblName, constraint);
                        cmd.ExecuteNonQuery();
                        */
                         
                        sql = string.Format("ALTER TABLE {0} DROP CONSTRAINT {1}", tblName, constraint);
                        Helper.ExecuteTransaction(tblName, sql, nID);

                        // �����µ�����Լ��
                        /*
                        cmd.CommandText = string.Format("ALTER TABLE {0} ADD CONSTRAINT PK_{1} PRIMARY KEY({2})", tblName, tblName, keyString.TrimEnd(','));
                        cmd.ExecuteNonQuery();
                         */

                        sql = string.Format("ALTER TABLE {0} ADD CONSTRAINT PK_{1} PRIMARY KEY({2})", tblName, tblName, keyString.TrimEnd(','));
                        Helper.ExecuteTransaction(tblName, sql, nID);
                    }
                    /* remoting
                    cmd.CommandText = string.Format("SELECT OBJECTPROPERTY(OBJECT_ID('{0}'), 'TableHasIdentity')", tblName);
                    object executeResult = cmd.ExecuteScalar();
                    */

                    string sqlCmd = string.Format("SELECT OBJECTPROPERTY(OBJECT_ID('{0}'), 'TableHasIdentity')", tblName);
                    object executeResult = Helper.ExecuteTransactionScalar(tblName, sqlCmd, nID, false);

                    if (executeResult != null && executeResult.ToString() == "1") // ������ݱ��Ƿ����Զ������
                    {
                        // ����ֵ���뵽�Զ��������
                        /* remoting
                        cmd.CommandText = string.Format("SET IDENTITY_INSERT {0} ON", tblName);
                        cmd.ExecuteNonQuery();
                         */

                        _sql = string.Format("SET IDENTITY_INSERT {0} ON", tblName);
                        Helper.ExecuteTransaction(tblName, _sql, nID);
                    }

                    // ������ӱ�����
                    foreach (DataRow r in tbl.Rows)
                    {
                        string newValue = "";
                        string columns = "";

                        //for (int i = 0; i < strColNames.Length; i++)
                        for (int i = 0; i < tbl.Columns.Count; i++)
                        {
                            string raw_colname = tbl.Columns[i].ColumnName;

                            string value = r[raw_colname].ToString().Replace("'", "''"); // �滻�����ַ�
                            string columnName = raw_colname.Replace("]", "]]"); // �滻�����ַ�
                            if (r[raw_colname] is Boolean) // �ж��Ƿ��ǲ���ֵ
                            {
                                value = (bool)r[raw_colname] ? "1" : "0";
                            }

                            if (value == string.Empty) // �Զ����Կ�ֵ��������ֵĿ�ֵת��������
                            {
                                continue;
                            }
                            else
                            {
                                columns += string.Format("[{0}], ", columnName);
                                newValue += string.Format("'{0}', ", value);
                            }
                        }

                        newValue = newValue.Trim().TrimEnd(',');
                        columns = columns.Trim().TrimEnd(',');
                        /*cmd.CommandText = string.Format("INSERT INTO {0} ({1}) VALUES ({2})", tblName, columns, newValue);*/
                        try
                        {
                           // cmd.ExecuteNonQuery();
                            string sql = string.Format("INSERT INTO {0} ({1}) VALUES ({2})", tblName, columns, newValue);
                            Helper.ExecuteTransaction(tblName, sql, nID);
                        }
                        catch (SqlException ex)
                        {
                            MessageBox.Show(ex.Message + "\r\n\r\n" + string.Format("INSERT INTO {0} ({1}) VALUES ({2})", tblName, columns, newValue));
                            throw ex;
                        }
                    }
                }
                else // ��˳�򲻱䣬ֻ����������ݼ���
                {
                    // ���Ǿɵ�������
                    if (overwriteRowList.Count > 0)
                    {
                        DataColumn[] keys = tbl.PrimaryKey;
                        foreach (DataRow r in overwriteRowList)
                        {
                            string setValue = "";
                            string keyValue = "";

                            foreach (string s in strColNames)
                            {
                                string columnName = s.Replace("]", "]]"); // �滻�����ַ�
                                string value = r[s].ToString().Replace("'", "''"); // �滻�����ַ�
                                if (r[s] is Boolean) // �ж��Ƿ��ǲ���ֵ
                                {
                                    value = (bool)r[s] ? "1" : "0";
                                }

                                if (value == string.Empty) // ��ֵ��������������Ҫ���⴦��
                                {
                                    setValue += string.Format("[{0}] = NULL, ", columnName);
                                }
                                else
                                {
                                    setValue += string.Format("[{0}] = \'{1}\', ", columnName, value);
                                }
                            }
                            setValue = setValue.Trim().TrimEnd(',');

                            foreach (DataColumn c in keys)
                            {
                                string columnName = c.ColumnName.Replace("]", "]]"); // �滻�����ַ�
                                string value = r[c.ColumnName].ToString().Replace("'", "''"); // �滻�����ַ�
                                keyValue += string.Format("[{0}] = \'{1}\' AND ", columnName, value);
                            }
                            keyValue = keyValue.Substring(0, keyValue.Length - 5);

                            /*cmd.CommandText = string.Format("UPDATE {0} SET {1} WHERE {2}", tblName, setValue, keyValue);*/

                            try
                            {
                                //cmd.ExecuteNonQuery();
                                string sql = string.Format("UPDATE {0} SET {1} WHERE {2}", tblName, setValue, keyValue);
                                Helper.ExecuteTransaction(tblName, sql, nID);
                            }
                            catch (SqlException ex)
                            {
                                MessageBox.Show(ex.Message + "\r\n\r\n" + string.Format("UPDATE {0} SET {1} WHERE {2}", tblName, setValue, keyValue));
                                throw ex;
                            }
                        }
                    }

                    // ������������
                    foreach (DataRow r in tbl.Rows)
                    {
                        if (index < rowNumber) // ���Ծɵ�������
                        {
                            index++;
                            continue;
                        }

                        string newValue = "";
                        string columns = "";

                        for (int i = 0; i < strColNames.Length; i++)
                        {
                            string value = r[strColNames[i]].ToString().Replace("'", "''"); // �滻�����ַ�
                            string columnName = strColNames[i].Replace("]", "]]"); // �滻�����ַ�
                            if (r[strColNames[i]] is Boolean) // �ж��Ƿ��ǲ���ֵ
                            {
                                value = (bool)r[strColNames[i]] ? "1" : "0";
                            }

                            if (value == string.Empty) // �Զ����Կ�ֵ��������ֵĿ�ֵת��������
                            {
                                continue;
                            }
                            else
                            {
                                columns += string.Format("[{0}], ", columnName);
                                newValue += string.Format("'{0}', ", value);
                            }
                        }

                        newValue = newValue.Trim().TrimEnd(',');
                        columns = columns.Trim().TrimEnd(',');
                        /*cmd.CommandText = string.Format("INSERT INTO {0} ({1}) VALUES ({2})", tblName, columns, newValue);*/
                        try
                        {
                            //cmd.ExecuteNonQuery();
                            string sql = string.Format("INSERT INTO {0} ({1}) VALUES ({2})", tblName, columns, newValue);
                            Helper.ExecuteTransaction(tblName, sql, nID);
                        }
                        catch (SqlException ex)
                        {
                            MessageBox.Show(ex.Message + "\r\n\r\n" + string.Format("INSERT INTO {0} ({1}) VALUES ({2})", tblName, columns, newValue));
                            throw ex;
                        }
                    }
                }

                tbl.AcceptChanges();
                //transaction.Commit(); // �ύ����
                Helper.CommitTransaction(nID);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("���ݸ���ʱ�����쳣: " + ex.ToString() + "\r\n\r\n�������ݽ���������",
                     "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                //transaction.Rollback(); // ����ع�
                Helper.RollbackTransaction(nID);
                if (Program.EnableRemoting)
                {
                    proxy.AddRemotingTable(tblName);
                }
            }
            finally
            {
                // ��ֹ���ݿ�ϵͳ�����
                /* remoting
                cfg_cmd.CommandText = "sp_configure 'allow updates', '0'";
                cfg_cmd.ExecuteNonQuery();
                cfg_cmd.CommandText = "reconfigure with override";
                cfg_cmd.ExecuteNonQuery();
                */


                Helper.ExecuteCommand("sp_configure 'allow updates', '0'", Conn);
                Helper.ExecuteCommand("reconfigure with override", Conn);

                if (Conn.State == ConnectionState.Open) // �ر�sql����
                {
                    Conn.Close();
                }

                if (Program.EnableRemoting)
                {
                    proxy.AddRemotingTable(tblName);
                }
            }

            // ���������
            tree.SelectedNode.Collapse();
            tree.SelectedNode.Nodes.Clear();
            tree.SelectedNode.Nodes.Add("��Ч����");
            
            //////////////////////////////////////////////////////////////////////////
            // �����ڴ� ����
            //////////////////////////////////////////////////////////////////////////
            AllTables[tblName] = tbl;
            string rpt = string.Format("�ɹ���������,�����У�{0}  �����У�{1}", iOverwriteCount, iAddCount);
            log.Log(TimeLog.enumLogType.ltend, null, null, string.Format("�����ļ���:{0}, ����Ŀ�����ݱ�:{1}, �������:{2}, ������:{3}, ������:{4}", strFile, tblName, importParams, backupName, rpt));
            log.Clear();
            MessageBox.Show(rpt);
            lua.Dispose();
        }

        private void DeleteTrashField(string tblName)
        {
            // ������ɾ��___delete_me�ֶ�
            Conn.Open();
            try
            {
                //SqlCommand cmd = Conn.CreateCommand();
                //cmd.CommandText = string.Format("ALTER TABLE {0} DROP COLUMN {1}", tblName, TRASH_FIELD);
                //cmd.ExecuteNonQuery();
                string sql = string.Format("ALTER TABLE {0} DROP COLUMN {1}", tblName, TRASH_FIELD);
                Helper.ExecuteCommand(tblName, sql, Conn);
            }
            catch (Exception ex)
            {
                string err = string.Format("��ɾ���� {0} �е��ֶ� {1} ʱ��������\r\n\r\n������Ϣ��{2}", tblName, TRASH_FIELD, ex.Message);
                //MessageBox.Show(err);
            }
            Conn.Close();
        }

        private void bNewField_Click(object sender, EventArgs e) // �½��ֶ�
        {
            string newFieldName = "";
            NameInput frm = new NameInput();
            DialogResult result = frm.ShowDialog();
            if (result == DialogResult.OK)
            {
                newFieldName = frm.StringInput.Trim();
            }
            if (newFieldName == "")
            {
                return;
            }

            string tblName = this.tree.SelectedNode.Text.Trim();
            bool bSucNewField = false;
            Conn.Open();
            try
            {
                /* remoting
                SqlCommand cmd = Conn.CreateCommand();
                cmd.CommandText = string.Format("ALTER TABLE {0} ADD {1} varchar (3000) NULL", tblName, newFieldName);
                cmd.ExecuteNonQuery();
                */

                string sql = string.Format("ALTER TABLE {0} ADD [{1}] varchar (3000) NULL", tblName, newFieldName);
                Helper.ExecuteCommand(tblName, sql, Conn);
                 
                bSucNewField = true;
            }
            catch (Exception ex)
            {
                string str = string.Format("������ֶ�ʱ��������\r\n\r\n������Ϣ��{0}", ex.Message);
                MessageBox.Show(str);
            }
            Conn.Close();

            if (bSucNewField)
            {
                //DataTable tbl = Helper.GetDataTable("SELECT * FROM " + tblName, Conn);
                DataTable tbl = Helper.GetDataTableProxy(tblName, Conn);

                AllTables[tblName] = tbl;

                TreeNode new_node = this.tree.SelectedNode.Nodes.Add(newFieldName);
                new_node.Tag = newFieldName;
                new_node.ImageIndex = 4;
                new_node.SelectedImageIndex = 4;
                new_node.StateImageIndex = 7;

                this.tree.SelectedNode = new_node;
                ShowFieldInfo(new_node);
                DeleteTrashField(tblName); // ������ɾ��___delete_me�ֶ�
            }
        }

        private void bRenameField_Click(object sender, EventArgs e) // �������ֶ�
        {
            TreeNode theNode = this.tree.SelectedNode;
            if (!theNode.IsEditing)
            {
                this.tree.LabelEdit = true;
                theNode.BeginEdit();
            }
        }

        private void bDelField_Click(object sender, EventArgs e) // ɾ���ֶ�
        {
            string tbl_Name = this.tree.SelectedNode.Parent.Text;
            string field_Name = this.tree.SelectedNode.Text;
            bool bSucDelField = false;
            Conn.Open();
            try
            {
                /* remoting
                SqlCommand cmd = Conn.CreateCommand();
                cmd.CommandText = string.Format("ALTER TABLE {0} DROP COLUMN {1}", tbl_Name, field_Name);
                cmd.ExecuteNonQuery();
                 */
                string sql = string.Format("ALTER TABLE {0} DROP COLUMN {1}", tbl_Name, field_Name);

                Helper.ExecuteCommand(tbl_Name, sql, Conn);

                bSucDelField = true;
            }
            catch (Exception ex)
            {
                string err = string.Format("��ɾ���� {0} �е��ֶ� {1} ʱ��������\r\n\r\n������Ϣ��{2}", tbl_Name, field_Name, ex.Message);
                MessageBox.Show(err);
            }
            Conn.Close();

            if (bSucDelField)
            {

                //DataTable tbl = Helper.GetDataTable("SELECT * FROM " + tbl_Name, Conn);

                DataTable tbl = Helper.GetDataTableProxy(tbl_Name, Conn);

                AllTables[tbl_Name] = tbl;

                this.tree.SelectedNode.Remove();
            }
        }

        private void bSetKey_Click(object sender, EventArgs e) // ��Ϊ����
        {
            string tblName = this.tree.SelectedNode.Parent.Text.Trim();
            string fieldName = this.tree.SelectedNode.Text.Trim();
            string oldKeys = "";
            foreach (DataColumn col in ((DataTable)AllTables[tblName]).PrimaryKey)
            {
                oldKeys += col.ColumnName;
                oldKeys += ",";
            }

            if (oldKeys.IndexOf(fieldName) != -1)
            {
                MessageBox.Show("����Ѿ���������.");
                return;
            }

            bool bSucSetKey = false;
            Conn.Open();
            try
            {
                // ������ֶβ���Ϊ��
                string dataType = "";
                string len = "";
                switch (this.combo_data_type.SelectedIndex)
                {
                    case 0:
                        dataType = "decimal";
                        len = "(18)";
                        break;
                    case 1:
                        dataType = "varchar";
                        len = "(" + textBox_data_len.Text.Trim() + ")";
                        if (len == "()")
                        {
                            MessageBox.Show("����ָ���ַ����ĳ���.");
                            return;
                        }
                        break;
                    case 2:
                        dataType = "bit";
                        len = "";
                        break;
                    case 3:
                        dataType = "int";
                        len = "";
                        break;
                    default:
                        MessageBox.Show("����������������.");
                        return;
                }

                // �������������ܺ��Ƿ񳬹� 900
                int maxlength = 0;
                foreach (DataColumn col in ((DataTable)AllTables[tblName]).PrimaryKey)
                {
                    maxlength += col.MaxLength;
                }
                int length = 0;
                if (textBox_data_len.Text != null && textBox_data_len.Text != string.Empty)
                    length = Convert.ToInt32(textBox_data_len.Text);
                if (maxlength + length > 900)
                {
                    MessageBox.Show("��Ϊ�������ֶγ����ܺͲ��ܳ��� 900 �����޸��ֶε����ͻ��߳��Ⱥ����ԡ�", "ʧ��", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    Conn.Close();
                    return;
                }

                /* remoting
                SqlCommand cmd = Conn.CreateCommand();
                string sql = string.Format("ALTER TABLE {0} ALTER COLUMN {1} {2}{3} NOT NULL",
                    tblName, fieldName, dataType, len);
                cmd.CommandText = sql;
                cmd.ExecuteNonQuery();
                */

                string sql = string.Format("ALTER TABLE {0} ALTER COLUMN [{1}] {2}{3} NOT NULL",
                    tblName, fieldName, dataType, len);
                Helper.ExecuteCommand(tblName, sql, Conn);

                // ��������
               
                sql = string.Format("SELECT * INTO {0} FROM {1}", "tmp" + tblName, tblName);
                 /* remoting
                cmd.CommandText = sql;
                cmd.ExecuteNonQuery();
                */

                Helper.ExecuteCommand(tblName, sql, Conn);

                sql = string.Format("DROP TABLE {0}", tblName);
                /* remoting
                cmd.CommandText = sql;
                cmd.ExecuteNonQuery();
                */

                Helper.ExecuteCommand(tblName, sql, Conn);

                Helper.RenameDbTable("tmp" + tblName, tblName, Conn);

                sql = string.Format("ALTER TABLE {0} ADD PRIMARY KEY ({1})", tblName, oldKeys + "[" + fieldName + "]");
                /* remoting
                cmd.CommandText = sql;
                cmd.ExecuteNonQuery();
                */

                Helper.ExecuteCommand(tblName, sql, Conn);
                bSucSetKey = true;
            }
            catch (Exception ex)
            {
                string err = string.Format("�ڽ��� {0} �е��ֶ� {1} ����Ϊ����ʱ��������\r\n\r\n������Ϣ��{2}", tblName, fieldName, ex.Message);
                MessageBox.Show(err);
            }
            Conn.Close();

            if (bSucSetKey)
            {
                //DataTable tbl = Helper.GetDataTable("SELECT * FROM " + tblName, Conn);
                DataTable tbl = Helper.GetDataTableProxy(tblName, Conn);

                AllTables[tblName] = tbl;

                this.tree.SelectedNode.StateImageIndex = 0;

                // ����˳������table��dbnull�������Ĭ��ֵ
                if (MainForm.m_bModifyDbTableDefault)
                {
                    DialogResult dr = MessageBox.Show("�ѳɹ�����������\r\n\r\n��ϣ���Ѹñ��еĿ���������Ӧ��Ĭ��ֵ��\r\n\r\n�Ƽ���ѡ < ��(Y) >��", "��ʾ", MessageBoxButtons.YesNo);
                    if (dr == DialogResult.Yes)
                    {
                        try
                        {
                            Helper.FillDBNullWithDefault(ref tbl);
                            /*
                            SqlDataAdapter adp = new SqlDataAdapter("SELECT * FROM " + tblName, Conn);
                            SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                            adp.UpdateCommand = cmdBuilder.GetUpdateCommand();
                            int val = adp.Update(tbl);
                            tbl.AcceptChanges();
                             */

                            int val = Helper.SaveTable(tbl, "SELECT * FROM " + tblName, Conn);

                            MessageBox.Show(string.Format("�ɹ����Ĭ��ֵ��\r\n\r\n��Ӱ���������{0}��", val));
                        }
                        catch (Exception ex)
                        {
                            string err = string.Format("��Ϊ���ݱ� {0} ���Ĭ��ֵʱ��������\r\n\r\n������ϢΪ��{1}", tblName, ex.Message);
                            MessageBox.Show(err);
                        }
                    }
                }
            }
        }

        private void bUnsetKey_Click(object sender, EventArgs e) // ɾ������
        {
            string tblName = this.tree.SelectedNode.Parent.Text.Trim();
            string fieldName = this.tree.SelectedNode.Text.Trim();
            string oldKeys = "";
            foreach (DataColumn col in ((DataTable)AllTables[tblName]).PrimaryKey)
            {
                oldKeys += col.ColumnName;
                oldKeys += ",";
            }
            oldKeys = oldKeys.Substring(0, oldKeys.Length - 1);

            int iIndex = oldKeys.IndexOf(fieldName);
            if (iIndex < 0)
            {
                MessageBox.Show("����ֶβ���������.");
                return;
            }

            oldKeys = oldKeys.Remove(iIndex, fieldName.Length);
            if (oldKeys.Length > 0)
            {
                oldKeys = oldKeys.Replace(",,", ",");

                if (oldKeys[0] == ',')
                    oldKeys = oldKeys.Substring(1);
                if (oldKeys[oldKeys.Length - 1] == ',')
                    oldKeys = oldKeys.Substring(0, oldKeys.Length - 1);
            }

            string newKeys = oldKeys;

            bool bString8000 = false;
            bool bSucUnsetKey = false;
            Conn.Open();
            try
            {

                SqlCommand cmd = Conn.CreateCommand();
                string sql = string.Format("SELECT * INTO {0} FROM {1}", "tmp" + tblName, tblName);
                /* remoting
                cmd.CommandText = sql;
                cmd.ExecuteNonQuery();
                */
                Helper.ExecuteCommand(tblName, sql, Conn);

                sql = string.Format("DROP TABLE {0}", tblName);
                /*
                cmd.CommandText = sql;
                cmd.ExecuteNonQuery();
                */

                Helper.ExecuteCommand(tblName, sql, Conn);

                Helper.RenameDbTable("tmp" + tblName, tblName, Conn);

                if (newKeys.Length > 0)
                {
                    sql = string.Format("ALTER TABLE {0} ADD PRIMARY KEY ({1})", tblName, newKeys);
                    /* remoting
                    cmd.CommandText = sql;
                    cmd.ExecuteNonQuery();
                    */
                    Helper.ExecuteCommand(tblName, sql, Conn);
                }

                // ���ֶ������
                string dataType = "";
                string len = "";
                switch (this.combo_data_type.SelectedIndex)
                {
                    case 0:
                        dataType = "decimal";
                        len = "(18)";
                        break;
                    case 1:
                        dataType = "varchar";
                        len = "(" + textBox_data_len.Text.Trim() + ")";
                        if (len == "()")
                        {
                            MessageBox.Show("����ָ���ַ����ĳ���.");
                            return;
                        }
                        break;
                    case 2:
                        dataType = "bit";
                        len = "";
                        break;
                    case 3:
                        dataType = "int";
                        len = "";
                        break;
                    default:
                        MessageBox.Show("����������������.");
                        return;
                }
                if (dataType == "varchar" && len == "(3000)")
                    bString8000 = true;
                sql = string.Format("ALTER TABLE {0} ALTER COLUMN {1} {2}{3} NULL",
                    tblName, fieldName, dataType, len);
                /* remoting
                cmd.CommandText = sql;
                cmd.ExecuteNonQuery();
                */
                Helper.ExecuteCommand(tblName, sql, Conn);

                bSucUnsetKey = true;
            }
            catch (Exception ex)
            {
                string err = string.Format("�ڽ��� {0} �е��ֶ� {1} ɾ������ʱ��������\r\n\r\n������Ϣ��{2}", tblName, fieldName, ex.Message);
                MessageBox.Show(err);
            }
            Conn.Close();

            if (bSucUnsetKey)
            {
                //DataTable tbl = Helper.GetDataTable("SELECT * FROM " + tblName, Conn);

                DataTable tbl = Helper.GetDataTableProxy(tblName, Conn);

                AllTables[tblName] = tbl;

                if (bString8000)
                    this.tree.SelectedNode.StateImageIndex = 7;
                else
                    this.tree.SelectedNode.StateImageIndex = -1;
            }
        }

        private void combo_data_type_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.tree.SelectedNode.Level == 0)
            {
                return;
            }

            string tblName = this.tree.SelectedNode.Parent.Text;
            DataTable tbl = ((DataTable)AllTables[tblName]);
            string fieldName = this.tree.SelectedNode.Text;
            Type type = tbl.Columns[fieldName].DataType;
            int len = tbl.Columns[fieldName].MaxLength;
           
            string sql = string.Format("SELECT * FROM syscolumns WHERE name='{0}' AND id=object_id('{1}')", fieldName, tblName);
            /* DataRow row = Helper.GetDataTable(sql, Conn).Rows[0];
             */

            
            DataTable table = Helper.GetDataTableWithSqlProxy("syscolumns", sql, Conn);
            DataRow row = table.Rows[0];


            int prec = Convert.ToInt32(row["prec"]);
            int scale = -1;
            if (row["scale"] == DBNull.Value)
                scale = 0;
            else
                scale = Convert.ToInt32(row["scale"]);
            bool bMatch = ((type.Name == "Decimal" && this.combo_data_type.SelectedIndex == 0)
                        || (type.Name == "String"  && this.combo_data_type.SelectedIndex == 1)
                        || (type.Name == "Boolean" && this.combo_data_type.SelectedIndex == 2)
                        || (type.Name == "Int32"   && this.combo_data_type.SelectedIndex == 3));

            switch (this.combo_data_type.SelectedIndex)
            {
                case 0: // ��ֵ(������)
                    this.textBox_data_len.Text = "";
                    this.textBox_data_len.Enabled = false;
                    this.textBox_Xiaoshu_len.Enabled = true;
                    this.textBox_Zhengshu_len.Enabled = true;
                    if (bMatch)
                    {
                        this.textBox_Xiaoshu_len.Text = Convert.ToString(scale);
                        this.textBox_Zhengshu_len.Text = Convert.ToString(prec);
                    }
                    else
                    {
                        this.textBox_Xiaoshu_len.Text = "2";
                        this.textBox_Zhengshu_len.Text = "10";
                    }
                    break;
                case 1: // �ַ���
                    this.textBox_Xiaoshu_len.Text = "";
                    this.textBox_Xiaoshu_len.Enabled = false;
                    this.textBox_Zhengshu_len.Text = "";
                    this.textBox_Zhengshu_len.Enabled = false;
                    this.textBox_data_len.Enabled = true;
                    if (bMatch)
                    {
                        this.textBox_data_len.Text = Convert.ToString(len);
                    }
                    else
                    {
                        this.textBox_data_len.Text = "3000";
                    }
                    break;
                case 2: // ���ֵ
                    this.textBox_data_len.Text = "";
                    this.textBox_data_len.Enabled = false;
                    this.textBox_Xiaoshu_len.Text = "";
                    this.textBox_Xiaoshu_len.Enabled = false;
                    this.textBox_Zhengshu_len.Text = "";
                    this.textBox_Zhengshu_len.Enabled = false;
                    break;
                case 3: // ����
                    this.textBox_data_len.Text = "";
                    this.textBox_data_len.Enabled = false;
                    this.textBox_Xiaoshu_len.Text = "";
                    this.textBox_Xiaoshu_len.Enabled = false;
                    this.textBox_Zhengshu_len.Text = "";
                    this.textBox_Zhengshu_len.Enabled = false;
                    break;
                default:
                    break;
            }
        }

        private void bEmptyTbl_Click(object sender, EventArgs e)
        {
            string tblName = this.tree.SelectedNode.Text.Trim();
            string confirm = string.Format("��ȷ��Ҫ��������� {0} ��������?", tblName);
            DialogResult result = MessageBox.Show(confirm, "����", MessageBoxButtons.YesNo);
            if (result == DialogResult.No)
                return;

            Conn.Open();
            try
            {
                /* remoting
                SqlCommand cmd = Conn.CreateCommand();
                string sql = "DELETE FROM " + tblName;
                cmd.CommandText = sql;
                cmd.ExecuteNonQuery();
                 */

                Helper.ExecuteCommand(tblName, "DELETE FROM " + tblName, Conn);
            }
            catch (Exception ex)
            {
                string err = string.Format("�ڽ��� {0} �������ʱ��������\r\n\r\n������Ϣ��{1}", tblName, ex.Message);
                MessageBox.Show(err);
            }
            Conn.Close();
        }

        private void checkHasDefault_CheckedChanged(object sender, EventArgs e)
        {
            bool bChecked = ((DevComponents.DotNetBar.Controls.CheckBoxX)sender).Checked;
            if (!bChecked)
            {
                comboExportRule.SelectedIndex = 1;
            }
        }

        private void cFieldNodeClick_Click(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Ԥ�����ݱ���Ϣ
        /// </summary>
        /// <param name="sender">��Ϣ������</param>
        /// <param name="e">��Ϣ</param>
        private void buttonItem1_Click(object sender, EventArgs e)
        {
            TreeNode node = tree.SelectedNode;
            if (node == null)
            {
                return;
            }
            string tableName = node.Text;
            DBTools.PreviewForm pf = new DBTools.PreviewForm(m_conn, tableName);
            DialogResult result = pf.ShowDialog();
        }

        /// <summary>
        /// ����㼴��չ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void tree_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            TreeNode currentNode = e.Node;
            if(currentNode.FirstNode.Text == "��Ч����") // ��Ҫ��������
            {
                ReloadAttributes(currentNode);
            }
        }

        private void buttonItem2_Click(object sender, EventArgs e) // ����Ĭ��ֵ��ȣ����NULL��ȫ�����
        {
            Conn.Open();
            try
            {
                string tbl_Name = this.tree.SelectedNode.Text;

                //DataTable tbl = Helper.GetDataTable("SELECT * FROM " + tbl_Name, Conn);

                DataTable tbl = Helper.GetDataTableProxy(tbl_Name, Conn);

                string strKeyRestriction = "";
                foreach (DataColumn col in tbl.PrimaryKey)
                {
                    strKeyRestriction += col.ColumnName;
                    strKeyRestriction += " <> 0 AND";
                }
                strKeyRestriction = strKeyRestriction.Substring(0, strKeyRestriction.Length - 4);
                SqlCommand cmd = Conn.CreateCommand();
                foreach (DataColumn col in tbl.Columns)
                {
                    string sql = string.Format("UPDATE {0} SET {1} = NULL WHERE ({2} = (SELECT TOP 1 {3} FROM {4})) AND ({5})",
                        tbl_Name, col.ColumnName, col.ColumnName, col.ColumnName, tbl_Name, strKeyRestriction);
                    /* remoting
                    cmd.CommandText = sql;
                    cmd.ExecuteNonQuery();
                    */

                    Helper.ExecuteCommand(tbl_Name, sql, Conn);
                }
                   
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(string.Format("����{0}", ex.Message));
            }
            Conn.Close();
            MessageBox.Show("�ɹ�");
        }

        /// <summary>
        /// ���õ��������ֶ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bConfigExcludeField_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = tree.SelectedNode;

            if (currentNode != null)
            {
                List<string> fieldList = new List<string>();
                
                foreach (TreeNode fieldNode in currentNode.Nodes)
                {
                    fieldList.Add(fieldNode.Text);
                }

                ExportFieldConfigForm eForm = new ExportFieldConfigForm(fieldList, textBox_Exclude_Field.Text);

                if (eForm.ShowDialog() == DialogResult.OK)
                {
                    textBox_Exclude_Field.Text = eForm.ExcludeFieldString;
                }
            }
        }

        private void buttonItemModColOrder_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = tree.SelectedNode;
            string[] columns = new string[currentNode.Nodes.Count];
            int index = 0;
            foreach (TreeNode node in currentNode.Nodes)
            {
                columns[index] = node.Text;
                index++;
            }

            ColumnOrder frm = new ColumnOrder(columns, currentNode.Text);
            frm.ShowDialog(this);
            if (frm.Changed)
            {
                SetColumnsOrder(currentNode.Text, frm.Result, null);
                tree.Nodes.Clear();
                DBTableEditor_Load(null, null);
            }
        }

        private void SetColumnsOrder(string tablename, string[] columns)
        {
            DataTable table = Helper.GetDataTableTopRowProxy(tablename, m_conn);
            string sqlColumn = string.Empty;
            foreach (string column in columns)
            {
                sqlColumn += "[" + column + "],";
            }
            sqlColumn = sqlColumn.Substring(0, sqlColumn.Length - 1);

            string newtable = tablename + "_new";
            //1.����������
            string sql1 = string.Format("select {0} into {1} from {2}", sqlColumn, newtable, tablename);
            Helper.ExecuteCommand(tablename, sql1, m_conn);

            //2.��ȡ����������Լ��
            if (table.PrimaryKey.Length > 0)
            {
                string sqlKey = string.Empty;
                foreach (DataColumn column in table.PrimaryKey)
                {
                    sqlKey += column.ColumnName + ",";
                }
                sqlKey = sqlKey.Substring(0, sqlKey.Length - 1);
                string sql2 = string.Format("SELECT name FROM sysindexes WHERE id = OBJECT_ID('{0}') AND name LIKE 'PK%'", tablename);
                object constraint = Helper.ExecuteScalar("sysindexes", sql2, m_conn, false);

                sql2 = string.Format("ALTER TABLE {0} ADD CONSTRAINT PK_{1} PRIMARY KEY({2})", newtable, newtable, sqlKey);
                Helper.ExecuteCommand(newtable, sql2, m_conn);
            }

            //3.�Զ������
            string sqlCmd = string.Format("SELECT OBJECTPROPERTY(OBJECT_ID('{0}'), 'TableHasIdentity')", tablename);
            object executeResult = Helper.ExecuteScalar(tablename, sqlCmd, m_conn, false);
            if (executeResult != null && executeResult.ToString() == "1")
            {
                string sql3 = string.Format("SET IDENTITY_INSERT {0} ON", newtable);
                Helper.ExecuteCommand(tablename, sql3, m_conn);
            }
        }
        
        private void SetColumnsOrder(string tablename, string[] columns, object flag)
        {
            DataTable table = Helper.GetDataTableProxy(tablename, m_conn);

            Helper.ExecuteCommand("sp_configure 'allow updates', '1'", m_conn);
            Helper.ExecuteCommand("reconfigure with override", m_conn);
            try
            {
                Helper.ExecuteCommand(tablename, "delete from " + tablename, m_conn);

                //1.�޸�ʹ��syscolumns
                int index = 1;
                foreach (string column in columns)
                {
                    string sql1 = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", index + 1000, column, tablename);
                    Helper.ExecuteCommand("syscolumns", sql1, m_conn);
                    index++;
                }

                index = 1;
                foreach (string column in columns)
                {
                    string sql1 = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", index, column, tablename);
                    Helper.ExecuteCommand("syscolumns", sql1, m_conn);
                    index++;
                }

                //2.��ȡ����������Լ��
                if (table.PrimaryKey.Length > 0)
                {
                    string sqlKey = string.Empty;
                    foreach (DataColumn column in table.PrimaryKey)
                    {
                        sqlKey += column.ColumnName + ",";
                    }
                    sqlKey = sqlKey.Substring(0, sqlKey.Length - 1);
                    string sql2 = string.Format("SELECT name FROM sysindexes WHERE id = OBJECT_ID('{0}') AND name LIKE 'PK%'", tablename);
                    object constraint = Helper.ExecuteScalar("sysindexes", sql2, m_conn, false);

                    sql2 = string.Format("ALTER TABLE {0} DROP CONSTRAINT {1}", tablename, constraint);
                    Helper.ExecuteCommand(tablename, sql2, m_conn);

                    sql2 = string.Format("ALTER TABLE {0} ADD CONSTRAINT PK_{1} PRIMARY KEY({2})", tablename, tablename, sqlKey);
                    Helper.ExecuteCommand(tablename, sql2, m_conn);
                }

                //3.�Զ������
                string sqlCmd = string.Format("SELECT OBJECTPROPERTY(OBJECT_ID('{0}'), 'TableHasIdentity')", tablename);
                object executeResult = Helper.ExecuteScalar(tablename, sqlCmd, m_conn, false);
                if (executeResult != null && executeResult.ToString() == "1")
                {
                    string sql3 = string.Format("SET IDENTITY_INSERT {0} ON", tablename);
                    Helper.ExecuteCommand(tablename, sql3, m_conn);
                }

                //4.��������
                foreach (DataRow row in table.Rows)
                {
                    string newValue = "";
                    string _columns = "";

                    for (int i = 0; i < table.Columns.Count; i++)
                    {
                        string raw_colname = table.Columns[i].ColumnName;

                        string value = row[raw_colname].ToString().Replace("'", "''");
                        string columnName = raw_colname.Replace("]", "]]");
                        if (row[raw_colname] is Boolean)
                        {
                            value = (bool)row[raw_colname] ? "1" : "0";
                        }

                        if (value == string.Empty)
                        {
                            continue;
                        }
                        else
                        {
                            _columns += string.Format("[{0}], ", columnName);
                            newValue += string.Format("'{0}', ", value);
                        }
                    }

                    newValue = newValue.Trim().TrimEnd(',');
                    _columns = _columns.Trim().TrimEnd(',');

                    string sql = string.Format("INSERT INTO {0} ({1}) VALUES ({2})", tablename, _columns, newValue);
                    Helper.ExecuteCommand(tablename, sql, m_conn);

                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            finally
            {
                Helper.ExecuteCommand("sp_configure 'allow updates', '0'", m_conn);
                Helper.ExecuteCommand("reconfigure with override", m_conn);
            }
        }

    }
    public class ImportInfoHelper
    {
        public ArrayList Fields;
        public string[] Values;
        public string ResultInfo; //�����Ϣ
    };
}