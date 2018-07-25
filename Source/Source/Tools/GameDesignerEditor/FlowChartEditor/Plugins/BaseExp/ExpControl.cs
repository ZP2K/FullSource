using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Diagnostics;

namespace BaseExp
{
    public partial class ExpControl : FlowLayoutPanel
    {
        public VarExp VExp = null;              //��������

        private GameEvent m_eventDefine;
        private ActionExp m_actionExp;
        private bool isReady = false;
        private bool m_showThisEvent = true;

        //��ȡ�Ƿ�ȫ��
        public bool GetReady
        {
            get { return isReady; }
        }

        public ActionExp ActionExp
        {
            get { return this.m_actionExp; }
        }

        public ExpControl()
        {
            InitializeComponent();
        }

        //�����¼���ʾ��ǩ
        public void SetComboText(GameEvent gameEvent)
        {
            Trace.Assert(gameEvent != null);

            //��һ���ٵ�ActionExp
            ActionExp aexp = new ActionExp();
            aexp.API = gameEvent.GetEventAPI();

            //������ʾ
            SetComboText(aexp, gameEvent);
            this.m_showThisEvent = false;
        }

        //���ݱ��ʽ��ʾ��ǩ
        public void SetComboText(ActionExp expAction, GameEvent gameEvent)
        {
            Trace.Assert(expAction != null);
            Trace.Assert(gameEvent != null);
            this.m_eventDefine = gameEvent;
            this.m_actionExp = expAction;
            GameAPI api = expAction.API;
            this.Controls.Clear();
            isReady = true;
            m_actionExp.strText = "";
            m_actionExp.strFullText = "";

            string strComboText = "}" + api.combText;
            string[] tmp = strComboText.Split(new string[] { "{" }, StringSplitOptions.None);
            for (int i = -1; i < tmp.Length - 1; i++)
            {
                string[] strcs = tmp[i + 1].Split(new string[] { "}" }, StringSplitOptions.None);

                //������ı�
                if (strcs[0] != "")
                {
                    LinkLabel link = new LinkLabel();
                    link.Margin = new System.Windows.Forms.Padding(0);
                    link.AutoSize = true;
                    link.Tag = new Hashtable();
                    int nReali = Convert.ToInt32(strcs[0].ToString()) - 1;
                    
                    TypeDefine typed = api.ArgList[nReali].ArgType;
                    if(typed.isEnum || typed.DBID == FixedValueProvider.TYPE_STRING_ID)
                    {
                        //�Ҽ��˵�
                        ConstExp[] exps = ExpManager.ScanConstantFromHistory(typed);
                        if (exps.Length > 0)
                        {
                            link.ContextMenu = new ContextMenu();
                            for(int it = 0; it < exps.Length; it++)
                            {
                                MenuItem mi = new MenuItem(exps[it].ToString());
                                mi.Tag = new object[] { nReali, exps[it] };
                                mi.Click += new EventHandler(mi_Click);
                                link.ContextMenu.MenuItems.Add(mi);
                            }
                        }
                    }
                    
                    if (/*api.ArgValues.Count <= i || */api.ArgValues[nReali] == null)
                    {

                        link.Text = "<" + api.ArgList[nReali].ArgName + ">";
                        link.LinkColor = Color.Red;
                        (link.Tag as Hashtable)["exp"] = ExpManager.CreateNewExp(api.ArgList[nReali].ArgType);
                        (link.Tag as Hashtable)["pos"] = nReali;
                        isReady = false;
                    }
                    else
                    {
                        link.Text = api.ArgValues[nReali].ToString();
                        link.LinkColor = Color.Blue;
                        (link.Tag as Hashtable)["exp"] = api.ArgValues[nReali];
                        (link.Tag as Hashtable)["pos"] = nReali;
                    }

                    link.MouseClick += new MouseEventHandler(link_MouseClick);
                    link.PreviewKeyDown += new PreviewKeyDownEventHandler(link_PreviewKeyDown);
                    this.Controls.Add(link);

                    //׷�ӵ���ʾ��������
                    //if(link.Text.Length > 25)
                    //    m_actionExp.strText += link.Text.Substring(0,23) + "...";
                    //else
                    //    m_actionExp.strText += link.Text + "";
                    m_actionExp.strText += link.Text;
                    m_actionExp.strFullText += link.Text;
                }

                //�����ı�
                if (strcs[1] != "")
                {
                    Label l = new Label();
                    l.Margin = new System.Windows.Forms.Padding(0);
                    l.AutoSize = true;
                    l.Text = strcs[1];
                    this.Controls.Add(l);
                    m_actionExp.strText += l.Text + "";
                    m_actionExp.strFullText += l.Text;
                    l.Text = l.Text.Replace("\r\n", "");
                    
                }
            }

            //��ӻ���
            Control br = new Label();
            br.Width = this.Width - 23;
            br.Height = 0;
            this.Controls.Add(br);

            //���Tipע��
            Control ltip = new Label();
            ltip.Text = "\r\n" + expAction.API.strTip;
            ltip.ForeColor = Color.Gray;
            ltip.AutoSize = true;
            this.Controls.Add(ltip);

            //ר�Ŵ���Ƚ�API
            if(expAction.API.DBID == FixedValueProvider.COMPARE_API_ID)
            {
                LinkLabel left = this.Controls[0] as LinkLabel;
                LinkLabel right = this.Controls[2] as LinkLabel;
                Hashtable htleft = left.Tag as Hashtable;
                Hashtable htright = right.Tag as Hashtable;
                if (left.LinkColor == Color.Red)
                {
                    //��ֵ�������
                    htleft["exp"] = ExpManager.CreateUnknownExp();

                    //�Ҳ���
                    right.LinkColor = Color.Gray;
                }
                else
                {
                    //�Ҳ�����͸������
                    (htright["exp"] as Exp).ReturnType = (htleft["exp"] as Exp).ReturnType;

                    //�Ҳ���
                    if ((htright["exp"] as Exp).strText == null)
                    {
                        right.LinkColor = Color.Red;
                    }
                }
            }

        }

        //�������
        void link_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left) return;
            LinkLabel link = sender as LinkLabel;
            if (link.LinkColor == Color.Gray) return;           //��ɫ����Ӧ
            Hashtable ht = link.Tag as Hashtable;
            Exp expEdit = ht["exp"] as Exp;
            int nAsPos = Convert.ToInt32(ht["pos"].ToString());
            TypeDefine require_retType = null;
            if (expEdit != null) require_retType = expEdit.ReturnType;
            ExpForm expForm = new ExpForm(expEdit, this.m_eventDefine, m_showThisEvent, require_retType, this.VExp);
            //У׼����
            expForm.Location = PointToScreen(new Point(link.Location.X, link.Location.Y + link.Height));
            int tx = expForm.Location.X;
            int ty = expForm.Location.Y;
            if (tx + expForm.Width > Screen.PrimaryScreen.Bounds.Width)
                tx = tx - expForm.Width + link.Width;
            if (ty + expForm.Height > Screen.PrimaryScreen.Bounds.Height)
                ty = ty - expForm.Height - link.Height;
            expForm.Location = new Point(tx, ty);
            //��������
            DialogResult dr = expForm.ShowDialog();
            if(dr == DialogResult.OK)
            {
                ExpManager.ScanConstantIntoHistory(expForm.RusultExp);          //�ѳ����������ʷ
                ModifyExp(nAsPos, expForm.RusultExp);
            }            
        }

        //ѡ�в˵�
        void mi_Click(object sender, EventArgs e)
        {
            MenuItem mi = sender as MenuItem;
            object[] objs = mi.Tag as object[];
            int nAsPos = Convert.ToInt32(objs[0].ToString());
            ConstExp exp = objs[1] as ConstExp;
            ModifyExp(nAsPos, exp);
        }

        void link_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                link_MouseClick(sender, new MouseEventArgs(MouseButtons.Left, 0, 0, 0, 0));
        }

        private void ModifyExp(int nAsPos, Exp exp)
        {
            this.m_actionExp.API.ArgValues[nAsPos] = exp;
            this.SetComboText(this.m_actionExp,this.m_eventDefine);
            //�Զ����ý���
            int n = 0;
            foreach(Control c in this.Controls)
            {
                if (c is LinkLabel)
                {
                    n++;
                    if (n > nAsPos)
                    {
                        (c as LinkLabel).Focus();
                        break;
                    }
                }
            }  
        }
    }
}
