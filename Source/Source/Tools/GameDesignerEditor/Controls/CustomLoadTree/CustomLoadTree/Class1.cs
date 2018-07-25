
using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using DevComponents.DotNetBar;


namespace CustomLoadTree
{
    public class Class1
    {
        private Form1 OldForm;
        private string TblName;
        private string CatField;
        private PanelDockContainer PDC;
        private TreeViewEventHandler afterSelect;
        private TreeViewCancelEventHandler beforeSelect;
        private NodeLabelEditEventHandler beforeLabelEdit;
        private NodeLabelEditEventHandler afterLabelEdit;
        private SqlConnection Conn;
        private Helper helper;
        private List<string> fieldList = new List<string>(); // ������༭�ֶ�����
        private bool m_isReadOnly = false;
        private static bool m_bCommentOnSave = true;

        public static bool CommentOnSave
        {
            get { return m_bCommentOnSave; }
            set { m_bCommentOnSave = value; }
        }

        //ֻ��ģʽ
        public Class1(string tblname, string catfield, PanelDockContainer pdc, SqlConnection conn)
        {
            OldForm = null;
            TblName = tblname;
            CatField = catfield;
            PDC = pdc;
            Conn = conn;
            helper = new Helper();
            Helper.Conn = Conn;
            Helper.adp_hash = new Hashtable();
            m_isReadOnly = true;
        }

        public Class1(string tblname, string catfield, PanelDockContainer pdc, TreeViewEventHandler afterselect, TreeViewCancelEventHandler beforeselect, NodeLabelEditEventHandler beforelabeledit, NodeLabelEditEventHandler afterlabeledit, SqlConnection conn)
        {
            OldForm = null;
            TblName = tblname;
            CatField = catfield;
            PDC = pdc;
            this.afterSelect = afterselect;
            this.beforeSelect = beforeselect;
            this.beforeLabelEdit = beforelabeledit;
            this.afterLabelEdit = afterlabeledit;
            Conn = conn;
            helper = new Helper();
            Helper.Conn = Conn;
            Helper.adp_hash = new Hashtable();
        }

        public void OpenFindForm()
        {
            OldForm.OpenFindForm();
        }

        public TreeView LoadTreePlugin(int toSelectID)
        {
            if (OldForm != null)
            {
                PDC.Controls.Remove(OldForm.Spliter);
                OldForm.Dispose();
            }

            Form1 newFrm = new Form1(TblName, CatField, this, Conn, this.m_isReadOnly);
            newFrm.FieldList = fieldList;
            OldForm = newFrm;
            TreeView tree = newFrm.ReloadTree();
            PDC.Controls.Add(newFrm.Spliter);
            newFrm.Spliter.Dock = DockStyle.Fill;

            tree.AfterSelect += afterSelect;
            tree.BeforeSelect += beforeSelect;
            tree.BeforeLabelEdit += beforeLabelEdit;
            tree.AfterLabelEdit += afterLabelEdit;

            if (toSelectID > 0)
            {
                ArrayList AllFoundNodes = Helper.FindAllActualNodeByID(tree, toSelectID);
                if (AllFoundNodes.Count > 0)
                {
                    tree.SelectedNode = (TreeNode)(AllFoundNodes[0]);
                    tree.Focus();
                }
            }

            return tree;
        }

        public void SetupExtraMenuItemClick(EventHandler itemClick)
        {
            OldForm.AddExtraMenuItems(itemClick);
        }

        public ButtonItem[] GetExtraMenuItems()
        {
            return OldForm.m_ExtraMenuItem;
        }

        /// <summary>
        /// ���ö�����༭��ʾ���ֶ�
        /// </summary>
        /// <param name="s">��Ҫ��ʾ���ֶ�</param>
        public void AddMultiEditFields(string s)
        {
            fieldList.Add(s);
        }
    }
}
