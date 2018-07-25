using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using DevComponents.DotNetBar;

namespace CustomLoadTree_Dungeon
{
    public class Class1
    {
        private Form m_OldForm;
        private PanelDockContainer m_PDC;
        private TreeViewEventHandler afterSelect;
        private TreeViewCancelEventHandler beforeSelect;
        private NodeLabelEditEventHandler beforeLabelEdit;
        private NodeLabelEditEventHandler afterLabelEdit;
        private SqlConnection m_Conn;
        private VisualEditDungeonEx m_EditDungeonEx;
  
       // private Helper helper;
       // private List<string> m_fieldList = new List<string>(); // ������༭�ֶ�����
        private bool m_isReadOnly = false;

        public Form TreeForm
        {
            get { return m_OldForm; }
        }

        public Class1(PanelDockContainer pdc, TreeViewEventHandler afterselect, TreeViewCancelEventHandler beforeselect, NodeLabelEditEventHandler beforelabeledit, NodeLabelEditEventHandler afterlabeledit, SqlConnection conn)
        {
            m_OldForm = null;

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

        public string DescriptionDialog(string strOld, Form parentForm, string RootDir, bool bImageSupport, int nFont)
        {
            StringBuilder sb = new StringBuilder();
            VisualEditEx EditBox = new VisualEditEx(strOld, sb, m_Conn, parentForm, RootDir, bImageSupport, nFont);
            if (EditBox.ShowDialog() != DialogResult.OK)
            {
                sb.Remove(0, sb.Length);
                sb.Append(strOld);
            }
            return sb.ToString().TrimEnd(new char[] { ' ' }) ;
        }

        //public void ReloadTree()
        //{
        //    (m_OldForm as Form1).ReloadTree();
        //}

        public TreeView LoadTreePlugin_Dungeon(int toSelectID)
        {
            if (m_OldForm != null)
            {
                m_PDC.Controls.Remove((m_OldForm as Form_Dungeon).Spliter);
                m_OldForm.Dispose();
            }

            Form_Dungeon newFrm = new Form_Dungeon(this, m_Conn, this.m_isReadOnly);
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

        public Control GetTabControl(string RootDir, int nFont)
        {
            m_EditDungeonEx = new VisualEditDungeonEx(m_Conn, this, RootDir, true, nFont);
            return m_EditDungeonEx.Table;
        }

        /// <summary>
        /// �Ƿ��û��ı�
        /// </summary>
        /// <returns>����һ���Ƿ��޸ĵı�־</returns>
        public Boolean hasChanged()
        {
            if (m_EditDungeonEx != null)
                return m_EditDungeonEx.hasChanged();
            else
                return false;
        }


        /// <summary>
        /// ��������ҳ
        /// </summary>
        public void saveAll()
        {
            if (m_EditDungeonEx != null)   
               m_EditDungeonEx.saveAll();
        }

        /// <summary>
        /// ˢ������ҳ��
        /// </summary>
        public void reloadAll()
        {
            if (m_EditDungeonEx != null) 
                m_EditDungeonEx.reloadAll();
        }

        /// <summary>
        /// �����¹ؼ�����������
        /// </summary>
        /// <param name="keyword">�¹ؼ���</param>
        public void reloadAll(string keyword)
        {
            if (m_EditDungeonEx != null) 
                m_EditDungeonEx.reloadAll(keyword);
        }
    }
}
