using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using DevComponents.DotNetBar;

namespace CustomLoadTree_CareerEvent
{
    public class Class1
    {
        private Form m_OldForm;
        private string m_TblName;
        private string m_CatField;
        private PanelDockContainer m_PDC;
        private TreeViewEventHandler afterSelect;
        private TreeViewCancelEventHandler beforeSelect;
        private NodeLabelEditEventHandler beforeLabelEdit;
        private NodeLabelEditEventHandler afterLabelEdit;
        private SqlConnection m_Conn;
        private VisualEditLibEx m_EditLibEx;
  
       // private Helper helper;
       // private List<string> m_fieldList = new List<string>(); // ������༭�ֶ�����
        private bool m_isReadOnly = false;

        public Form TreeForm
        {
            get { return m_OldForm; }
        }

        //ֻ��ģʽ
        public Class1(string tblname, string catfield, PanelDockContainer pdc, SqlConnection conn)
        {
            m_OldForm = null;
            m_TblName = tblname;
            m_CatField = catfield;
            m_PDC = pdc;
            m_Conn = conn;
           // helper = new Helper();
            Helper.Conn = m_Conn;
            Helper.adp_hash = new Hashtable();
            m_isReadOnly = true;
        }

        public Class1(string tblname, string catfield, PanelDockContainer pdc, TreeViewEventHandler afterselect, TreeViewCancelEventHandler beforeselect, NodeLabelEditEventHandler beforelabeledit, NodeLabelEditEventHandler afterlabeledit, SqlConnection conn)
        {
            m_OldForm = null;
            m_TblName = tblname;
            m_CatField = catfield;
            m_PDC = pdc;
            this.afterSelect = afterselect;
            this.beforeSelect = beforeselect;
            this.beforeLabelEdit = beforelabeledit;
            this.afterLabelEdit = afterlabeledit;
            m_Conn = conn;
            // helper = new Helper();
            Helper.Conn = m_Conn;
            Helper.adp_hash = new Hashtable();
        }

        public string DescriptionDialog(string strOld, Form parentForm, string RootDir, bool bImageSupport, int nDefFont)
        {
            StringBuilder sb = new StringBuilder();
            VisualEditEx EditBox = new VisualEditEx(strOld, sb, m_Conn, parentForm, RootDir, bImageSupport, nDefFont);
            if (EditBox.ShowDialog() != DialogResult.OK)
            {
                sb.Remove(0, sb.Length);
                sb.Append(strOld);
            }
            return sb.ToString().TrimEnd(new char[] { ' ' }) ;
        }

        public TreeView LoadTreePlugin(int toSelectID)
        {
            if (m_OldForm != null)
            {
                m_PDC.Controls.Remove((m_OldForm as Form1).Spliter);
                m_OldForm.Dispose();
            }

            Form1 newFrm = new Form1(m_TblName, m_CatField, this, m_Conn, this.m_isReadOnly);
          //  newFrm.FieldList = m_fieldList;
            m_OldForm = newFrm;
            TreeView tree = newFrm.ReloadTree();
            m_PDC.Controls.Add(newFrm.Spliter);
            newFrm.Spliter.Dock = DockStyle.Fill;

            tree.AfterSelect += afterSelect;
            tree.BeforeSelect += beforeSelect;
            tree.BeforeLabelEdit += beforeLabelEdit;
            tree.AfterLabelEdit += afterLabelEdit;

            return tree;
        }
        
        public void ReloadTree()
        {
            (m_OldForm as Form1).ReloadTree();
        }

        public TreeView LoadTreePlugin_JX3lib(int toSelectID)
        {
            if (m_OldForm != null)
            {
                m_PDC.Controls.Remove((m_OldForm as Form1).Spliter);
                m_OldForm.Dispose();
            }

            Form_JX3lib newFrm = new Form_JX3lib(m_TblName, m_CatField, this, m_Conn, this.m_isReadOnly);
            //  newFrm.FieldList = m_fieldList;
            m_OldForm = newFrm;
            TreeView tree = newFrm.ReloadTree();
            m_PDC.Controls.Add(newFrm.Spliter);
            newFrm.Spliter.Dock = DockStyle.Fill;

            tree.AfterSelect += afterSelect;
            tree.BeforeSelect += beforeSelect;
            tree.BeforeLabelEdit += beforeLabelEdit;
            tree.AfterLabelEdit += afterLabelEdit;

            return tree;
        }

        public Control GetTabControl(string RootDir, string tabName, int nFont)
        {
            m_EditLibEx = new VisualEditLibEx(m_Conn, this, tabName, RootDir, true, nFont);
            return m_EditLibEx.Table;
        }

        /// <summary>
        /// �Ƿ��û��ı�
        /// </summary>
        /// <returns>����һ���Ƿ��޸ĵı�־</returns>
        public Boolean hasChanged()
        {
            if (m_EditLibEx != null)
                return m_EditLibEx.hasChanged();
            else
                return false;
        }


        /// <summary>
        /// ��������ҳ
        /// </summary>
        public void saveAll()
        {
            if (m_EditLibEx != null)   
               m_EditLibEx.saveAll();
        }

        /// <summary>
        /// ˢ������ҳ��
        /// </summary>
        public void reloadAll()
        {
            if (m_EditLibEx != null) 
                m_EditLibEx.reloadAll();
        }

        /// <summary>
        /// �����¹ؼ�����������
        /// </summary>
        /// <param name="keyword">�¹ؼ���</param>
        public void reloadAll(string keyword)
        {
            if (m_EditLibEx != null) 
                m_EditLibEx.reloadAll(keyword);
        }
    }
}
