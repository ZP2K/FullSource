using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Reflection;
using System.Collections;
using LuaInterface;
using System.IO;
using System.Threading;


namespace GameDesingerTools
{
    public class ScriptCodeForm //: Form
    {
        //bool m_bReEnter = false;

        private LuaEx m_lua_p = null;

        public LuaEx m_lua
        {
            get
            {
                return m_lua_p;
            }
            set
            {
                m_lua_p = value;
                g_lua["p_lua"] = value;
            }
        }

        private LuaEx g_lua = new LuaEx();
        private MainForm m_mainform = null;

        //���ݿ�ű��༭�������
        public void Show(int fieldid, int modtabid, object tag)
        {
            g_lua["Conn"] = m_lua_p["Conn"];
            g_lua["tag"] = tag;
            g_lua.DoString(string.Format("OpenWindow({0},{1})", fieldid, modtabid));
        }

        //�ļ��ű��༭�������
        public void Show(string fileName, object tag)
        {
            g_lua["Conn"] = m_lua_p["Conn"];
            g_lua["tag"] = tag;
            g_lua.DoString(string.Format("OpenFileWindow('{0}')", fileName.Replace("\\","\\\\")));
        }

        public ScriptCodeForm(MainForm mainform)
        {
            this.m_mainform = mainform;
            g_lua.RegisterFunction("lt2ArrayList", this, typeof(ScriptCodeForm).GetMethod("lt2ArrayList"));
            g_lua.RegisterFunction("lt2ArrayList_parms", this, typeof(ScriptCodeForm).GetMethod("lt2ArrayList_parms"));
            g_lua.RegisterFunction("lt_GetType", this, typeof(ScriptCodeForm).GetMethod("lt_GetType"));
            g_lua.RegisterFunction("SaveScriptIntoDatabase", this, typeof(ScriptCodeForm).GetMethod("SaveScriptIntoDatabase"));

            string strInitFileName = Path.Combine(Application.StartupPath, @"Plugins\LuaEditor\scriptcode.lua");
            if (!File.Exists(strInitFileName))
            {
                MessageBox.Show("�Ҳ���" + strInitFileName, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            g_lua.DoFile(strInitFileName);
        }

        public void loopbody()
        {
            Thread.Sleep(10);
            Application.DoEvents();
        }

        /// <summary>
        /// ����ű������ݿ���ڴ棬����ڴ���û��tabҳ����ֻ�����ݿ�.�˺���Ŀǰֻ���ű�����
        /// </summary>
        /// <param name="fieldid"></param>
        /// <param name="tabid"></param>
        /// <param name="strCode"></param>
        /// <returns></returns>
        public string SaveScriptIntoDatabase(string fieldid, string tabid, string strCode)
        {
            DBCustomClass dbclass = FindDbClassByTabid(tabid);
            DBCustomProperty prop = FindPropByID(fieldid, tabid);
            if(fieldid == "0")
            {
                //��Tabҳ�ű�
                //DBCustomClass dbclass = FindDbClassByTabid(tabid);
                if(dbclass != null)
                {
                    dbclass.SetScriptCode(null, strCode);
                    dbclass.SaveScript(null);
                    string logcomment = string.Format("PropertyName={0}, FieldID={1}", "#", fieldid);
                    //logscript.Log(TimeLog.enumLogType.ltend, null, null, logcomment + " �ű�������!");
                    dbclass.DoScript(null);
                    return "OK";
                }
            }
            else
            {
                //�����Խű�
                //DBCustomClass dbclass = FindDbClassByTabid(tabid);
                //DBCustomProperty prop = FindPropByID(fieldid, tabid);
                if(prop != null && dbclass != null)
                {
                    //�ڴ����нű�
                    dbclass.SetScriptCode(prop,strCode);
                    dbclass.SaveScript(prop);
                    string logcomment = string.Format("PropertyName={0}, FieldID={1}", prop.Name, prop.Key);
                    //logscript.Log(TimeLog.enumLogType.ltend, null, null, logcomment + " �ű�������!");
                    dbclass.DoScript(prop);
                    return "OK";
                }
            }

            //�ڴ���û�нű���ֱ�ӽ����ݿ�
            string strMainTable = prop == null ? "" : (prop.Parent as DBCustomClass).MainTable;
            return DBProcess.SaveScriptByID(strMainTable, fieldid, tabid, strCode) ? "OK" : "NG";
        }

        //����tabid�ҵ�DBCustomClass
        public DBCustomClass FindDbClassByTabid(string tabid)
        {
            if(this.m_mainform == null) return null; 
            //ÿ��ģ��
            foreach (Form frm in this.m_mainform.MdiChildren)
            {
                if (frm != null && frm is BaseForm)
                {
                    BaseForm bf = frm as BaseForm;
                    //ÿ��Tabҳ
                    foreach (PageDesc pd in bf.PagesCS)
                    {
                        if(pd.dbClass.TabID.ToString() == tabid)
                        {
                            return (pd.dbClass);
                        }
                    }
                }
            }
            return null;
        }

        //����fieldid, tabid���ҳ�prop��ǰ����fieldid����
        public DBCustomProperty FindPropByID(string fieldid, string tabid)
        {
            DBCustomClass dbClass = FindDbClassByTabid(tabid);
            if (dbClass == null) return null;

            //ÿ���ֶ�
            foreach (DBCustomProperty prop in dbClass)
            {
                if (prop != null && prop.Key == fieldid)
                    return prop;
                if (prop.Value != null && prop.Value is DBCustomClass)
                {
                    DBCustomProperty ret = FindPropInPropByFieldID(prop, fieldid);
                    if (ret != null) return ret;
                }
            }
            return null;
        }

        //�ݹ���
        private DBCustomProperty FindPropInPropByFieldID(DBCustomProperty prop , string fieldid)
        {
            if (prop.Value == null ) return null; //�����ڴӱ��ֶ��в��ܱ���ű�������

                DBCustomClass cls = prop.Value as DBCustomClass;
                
                //����������
                foreach(DBCustomProperty iprop in cls)
                {
                    //�ҵ���
                    if (iprop.Key == fieldid) return iprop;
                }

                //�������ֶ�֮���
                foreach(DBCustomProperty iprop in cls)
                {

                    if (iprop.Value != null && iprop.Value is DBCustomClass)
                    {
                        DBCustomProperty ret = FindPropInPropByFieldID(iprop, fieldid);
                        if (ret != null) return ret;
                    }

                }

                return null;
        }

        /*
        protected override void WndProc(ref Message m)
        {
            try
            {
                base.WndProc(ref m);
            }
            catch (System.Exception e)
            {
                Helper.AddLog(e.Message);
            }
        }
         * */

        #region �����ӿں���

        public Type lt_GetType(string name)
        {
            if (name == "TreeNode")
            {
                TreeNode tn = new TreeNode();
                return tn.GetType();
            }
            if (name == "GridItem")
            {
                PropertyGrid pg = new PropertyGrid();
                pg.SelectedObject = 0;
                return pg.SelectedGridItem.GetType();
            }
            return (Type.GetType(name));
        }

        public ArrayList lt2ArrayList(LuaEx lua, string keyword)
        {
            try
            {
                LuaInterface.LuaTable lt = lua.GetTable(keyword);
                ArrayList ret = new ArrayList();
                if (lt != null)
                    foreach (object k in lt.Keys)
                    {
                        object[] astr = { k.ToString(), lt[k].ToString() };
                        ret.Add(astr);
                    }
                return ret;
            }
            catch
            {
                return new ArrayList();
            }
        }

        public string lt2ArrayList_parms(LuaEx lua, string keyword)
        {
            string ret = keyword;
            try
            {
                LuaInterface.LuaTable lt = (LuaInterface.LuaTable)(lua.DoString("return debug.getinfo(" + keyword + ")")[0]);
                if (lt["what"].ToString() == "Lua")
                {
                    ret += "(";
                    ret += lt["strparms"].ToString();
                    if (lt["has3dot"].ToString() == "1")
                        ret += ",...";
                    ret += ")";
                }
            }
            catch
            {
                ret = "";
            }
            return (ret);
        }
        #endregion

#if false

        #region "��ʷ��¼���"
        //�������е���ʷ��¼
        public void LoadHisCode(int nModTabId, int nFieldId)
        {
            //nFieldId == 0 ��ʾ #
            //m_tbname = string.Empty;
            //m_fname = string.Empty;

            CMBHistory.Items.Clear();

            string strSql;
            if (nFieldId == 0)
                strSql = string.Format("SELECT * FROM sys_scriptstore_log WHERE modtabid={0} AND fieldname='{1}' ORDER BY logtime", nModTabId, "#");
            else
                strSql = string.Format("SELECT * FROM sys_scriptstore_log WHERE modtabid={0} AND fieldid={1} ORDER BY logtime", nModTabId, nFieldId);

            SqlDataAdapter adp = new SqlDataAdapter(strSql, MainForm.conn);
            m_HistCode = new System.Data.DataSet();
            System.Data.DataSet HistCode = m_HistCode;
            adp.Fill(HistCode);

            //string strfname;
            int nNewIdx = -1;
            foreach (System.Data.DataRow row in HistCode.Tables[0].Rows)
            {
                nNewIdx = CMBHistory.Items.Add(row["logtime"]);
            }
            //���ڣӣѣ̣ӣţң֣ţҴ��������ܼ�¼updateǰ��ֵ�������ʷ��¼�е�����һ���뵱ǰֵһ��������Ҫ����
            if (nNewIdx >= 0)
                CMBHistory.Items.RemoveAt(nNewIdx);
            //
            CMBHistory.Items.Add("���ڴ���汾");

            sliderHistory.Minimum = 0;
            sliderHistory.Maximum = CMBHistory.Items.Count - 1;// HistCode.Tables[0].Rows.Count;
            sliderHistory.Value = sliderHistory.Maximum;
            //m_tbname = strTableName;
            //m_fname = strFieldName;
            CMBHistory.Visible = true;
            sliderHistory.Visible = true;
        }

        private void ShowCode(int nIndex)
        {
            if (m_bReEnter)
                return;
            m_bReEnter = true;
            try
            {
                //��ת�����ʰ汾
                lua_OutputBox.Text = "";
                if (nIndex + 1 >= CMBHistory.Items.Count) //����ʷ�汾
                    lua_OutputBox.Text = m_strCurCode;
                else
                    lua_OutputBox.Text = Helper.GetStringFromByteArray(m_HistCode.Tables[0].Rows[nIndex]["scriptcode"]);

                sliderHistory.Value = nIndex;
                CMBHistory.SelectedIndex = nIndex;
            }
            finally
            {
                m_bReEnter = false;
                //OutputBox.Refresh();
            }
        }

        private void CMBHistory_SelectedIndexChanged(object sender, EventArgs e)
        {
            ShowCode(CMBHistory.SelectedIndex);
        }

        private void sliderHistory_ValueChanged(object sender, EventArgs e)
        {
            ShowCode(sliderHistory.Value);
        }

        #endregion

#endif 

    }

}