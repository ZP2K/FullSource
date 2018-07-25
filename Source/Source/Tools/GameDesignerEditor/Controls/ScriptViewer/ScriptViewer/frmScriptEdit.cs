using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.IO;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ScriptViewer
{
    public delegate void OnSaveEventsHandle(object sender, OnSaveEventArgs e);

    public partial class frmScriptEdit : Form
    {
        private LuaRuntime luaRuntime = LuaRuntime.GetLuaRunTime();
        private ScriptLock m_slLock;
        private bool m_bLocked = false;//�Ƿ��м�����¼
        private bool m_bLocalLocked = false;//��������
        private string m_strCode = "";
        private frmScriptView m_fsv;
        public string fieldid = "";
        public string tabid = "";
        public string filename = "";
        public object tag = null;

        public event OnSaveEventsHandle OnSave = null;

        public frmScriptEdit()
        {
            InitializeComponent();
        }

        //�ı���ʾ�ı�����ʵ�ı�
        public string strCode
        {
            get { return luaEditorControl1.Text; }
            set
            {
                m_strCode = value;
                luaEditorControl1.Text = m_strCode;
            }
        }

        //�����ı���ʾ�ı�
        public void setCode(string strcode)
        {
            luaEditorControl1.Text = strcode;
        }

        /// <summary>
        /// �Ƿ񱻸ı�
        /// </summary>
        public bool isChanged
        {
            get { return luaEditorControl1.Text != m_strCode; }
        }

        private void luaEditorControl1_OnChange(object sender, EventArgs e)
        {
            if (this.fieldid == null || this.tabid == null)
                return;

            m_fsv = this.MdiParent as frmScriptView;
            m_slLock = new ScriptLock(m_fsv.Conn, Convert.ToInt32(this.fieldid), Convert.ToInt32(this.tabid), "", "");
            if (!m_bLocalLocked && !m_bLocked)
            {
                //����δ����,û�м�����¼  ���Լ���
                if (m_slLock.Lock())
                {
                    //�����Ѽ����������ɹ�
                    m_bLocalLocked = true;
                    m_bLocked = false;//��ǰ�޼�����¼
                }
                else
                {
                    //���м�����¼������ʧ��
                    m_bLocked = true;
                    m_bLocalLocked = false;
                    MessageBox.Show("�������ѱ�" + m_slLock.GetServerLastLockUser() + "��������ǰ���޸Ĳ����Ա����棡");
                }
            }
            if (isChanged && !this.Text.EndsWith("*"))
            {
                this.Text += "*";
            }
        }

        private void frmScriptEdit_FormClosing(object sender, FormClosingEventArgs e)
        {
            //�ر�ǰ��ȷ�ϴ��
            if(isChanged && m_bLocalLocked)
            {
                if (MessageBox.Show("�����Ѹı��Ƿ񱣴棿", "�Ƿ񱣴�", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                {
                    //����
                    bool r = this.Save();
                    if (!r)
                    {
                        MessageBox.Show("����ʧ��");
                        e.Cancel = true;
                        return;
                    }
                }
                else
                {
                    //�����棬ҲҪ����
                    if (m_bLocalLocked)
                    {
                        m_slLock.UnLock();
                        m_bLocked = false;
                        m_bLocalLocked = false;
                    }
                }
            }
        }

        /// <summary>
        /// �����ĵ������ݿ�
        /// </summary>
        /// <returns></returns>
        public bool Save()
        {
            m_fsv = this.MdiParent as frmScriptView;
            if(this.filename != "")
            {
                //�����ļ�������
                StringToFile(luaEditorControl1.Text, filename);
            }
            else
            {
                //����ű���dbclass
                if(this.OnSave != null && m_bLocalLocked)
                {
                    OnSaveEventArgs osea = new OnSaveEventArgs(this.fieldid, this.tabid, luaEditorControl1.Text, this.tag);
                    this.OnSave(this, osea);
                    //m_slLock = new ScriptLock(m_fsv.Conn, Convert.ToInt32(this.fieldid), Convert.ToInt32(this.tabid), "", "");
                    m_slLock.UnLock();
                    m_bLocalLocked = false;
                    m_bLocked = false;

                    if( osea.fieldid == "NG")
                    {
                        return false;
                    }
                }
                else if (!m_bLocalLocked && m_bLocked)
                {
                    MessageBox.Show("���ݱ��������޷����浽���ݿ⣡");
                }
            }
            
            this.strCode = luaEditorControl1.Text;
            if (this.Text.EndsWith("*")) this.Text = this.Text.Substring(0, this.Text.Length - 1);

            return true;
        }

        // ����string���ļ�
        static public void StringToFile(string strContent, string strFileName)
        {
            StringToFile(strContent, strFileName, Encoding.GetEncoding("gb2312")); // �������ؼ�����Encoding
        }
        static public void WriteStringToFile(string strContent, string strFileName)
        {
            StringToFile(strContent, strFileName); // �������ؼ�����Encoding
        }
        static public void StringToFile(string strContent, string strFileName, Encoding encoding)
        {
            if (!File.Exists(strFileName))
            {
                Directory.CreateDirectory(Path.GetFullPath(strFileName));
                File.Create(strFileName).Close();
            }
            StreamWriter writer = new StreamWriter(strFileName, false, encoding);
            writer.Write(strContent);
            writer.Close();
        }

        private void luaEditorControl1_queryObjects(object sender, luaEditor.QueryEventsArgs e)
        {
            string strOut = "";
            IntPtr ls_out = IntPtr.Zero;

            //�����涨��
            //szCode:   ���еĴ���
            //nPos:     ���λ�ã����ְ�2���ַ��㣩
            //nIsDelete 0Ϊ�������룬1Ϊɾ����
            //  11-22ΪF1-F12,
            //  1111����Ctrl+Shift+F1,
            //  1011����Ctrl+F1
            //   111����Shift+F1
            int nIsDelete = e.nPos > 0 ? 0 : 1;

            if (e.keyword != "")
            {
                nIsDelete = 10 + Int32.Parse(e.keyword);
            }

            bool bret = LuaRuntime.LuaRun("OnCodeSense", e.szCode, Math.Abs(e.nPos), nIsDelete, "true,1024,��ʾ����,1,false,�﷨���", ref ls_out);
            strOut = Marshal.PtrToStringAnsi(ls_out);

            if (bret == false)
            {
                MessageBox.Show(strOut, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (strOut == null)
            {
                return;
            }

            string[] as_data = strOut.Split(new char[] { '|' });
            if (as_data.Length < 3)
            {
                MessageBox.Show("����ֵ����ȷ������ֵΪ��" + strOut, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // ��ȡoverlen
            e.nOverLen = Int32.Parse((as_data[0]));

            // ��ʼ���α�
            int nPosition = 3;

            // ��ȡlist
            int nListCount = Int32.Parse((as_data[1]));
            if (as_data.Length < 3 + nListCount)
            {
                MessageBox.Show("List����ֵ��������ȷ������ֵΪ��" + strOut, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            for (int i = 0; i < nListCount; i++)
            {
                string s_name = as_data[nPosition++] as string;
                string s_word = as_data[nPosition++] as string;
                string s_type = as_data[nPosition++] as string;
                string s_info = as_data[nPosition++] as string;
                e.leTable.Add(s_name, new luaEditor.leTableItem(s_name, s_type, s_word, s_info));

            }

            // ��ȡinfo
            int nInfoCount = Int32.Parse((as_data[2]));
            if (as_data.Length < 3 + nListCount + nInfoCount)
            {
                MessageBox.Show("Info����ֵ��������ȷ������ֵΪ��" + strOut, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            for (int i = 0; i < nInfoCount; i++)
            {
                string s_text = as_data[nPosition++] as string;
                e.parms_list.Add(s_text.Replace("\n", "<BR>"));
            }
        }

        public void GoToPosition(FindResultPosition position)
        {
            if (position == null)
            {
                return;
            }
            int LineNumber = position.LineNumber;
            int ColNumber = position.ColNumber;
            int Length = position.Text.Length;
            //this.luaEditorControl1.MoveToLine(LineNumber);
            //add by wangying1  2009-7-23
            this.luaEditorControl1.SelectText(LineNumber - 1, ColNumber, Length);
          
            //end of add 
        }

        private void luaEditorControl1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.F3 && e.Control)
            {
                this.luaEditorControl1.SetupFindText();
                this.luaEditorControl1.FindNextSelectText();
            }
            else if (e.KeyCode == Keys.F3 && e.Shift)
            {
                this.luaEditorControl1.FindPreviousSelectText();
            }
            else if (e.KeyCode == Keys.F3)
            {
                this.luaEditorControl1.FindNextSelectText();
            }

        }

    }

    public class OnSaveEventArgs : EventArgs
    {
        public OnSaveEventArgs(string fieldid, string tabid, string strcode, object tag)
        {
            this.strcode = strcode;
            this.fieldid = fieldid;
            this.tabid = tabid;
            this.tag = tag;
        }
        public string strcode = null;
        public string fieldid = "";
        public string tabid = "";
        public object tag = null;
    };


}