using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using BaseExp;
using System.Text.RegularExpressions;

namespace ConditionNode
{
    public partial class ConditionForm : DevComponents.DotNetBar.Office2007Form
    {
        private Exp m_actexp;
        private GameEvent m_gameEvent;
        private List<Exp> m_elist;
        private bool m_bChanged = false;

        private VarExp m_Vexp = new VarExp();

        //�����������ʽ
        public Exp ResultExp
        {
            get { return m_actexp; }
        }

        //�����߱��ʽ�б�
        public List<Exp> LineExpList
        {
            get { return m_elist; }
        }

        public ConditionForm(Exp actexp, GameEvent gameEvent, List<Exp> ConditionList)
        {
            InitializeComponent();

            this.m_actexp = actexp;
            this.m_elist = ConditionList;
            this.m_gameEvent = gameEvent;

            this.textBoxX1.Text = m_actexp.ToString();
            this.listBox1.Items.AddRange(ConditionList.ToArray());

        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (this.m_actexp == null || this.m_actexp.ReturnType == null || this.m_actexp.ReturnType.DBID == FixedValueProvider.TYPE_NIL_ID)
            {
                MessageBox.Show("�������������ʽ", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //ѡ�����APIĬ��ֵ
            ActionExp expa = ExpManager.CreateNewActionExp(new GameAPIDefine(FixedValueProvider.COMPARE_API_ID));

            expa.API.ArgValues[0] = m_Vexp;           //��ֵΪ�ɱ�����
            expa.API.ArgValues[1] = ExpManager.CreateUnknownExp();      //��ֵΪ������
            m_Vexp.ReturnType = expa.API.ArgValues[1].ReturnType = this.m_actexp.ReturnType;     //���Զ����������������ʽ�����ͺ�����һ��


            ExpForm expform = new ExpForm(expa, this.m_gameEvent, true, expa.ReturnType, m_Vexp);
            expform.StartPosition = FormStartPosition.CenterParent;
            if (expform.ShowDialog() == DialogResult.OK)
            {
                this.listBox1.Items.Add(expform.RusultExp);
                this.m_bChanged = true;
            }
        }

        private void btnExp_Click(object sender, EventArgs e)
        {
            ExpForm expform = new ExpForm(this.m_actexp, this.m_gameEvent, true, null, null);
            expform.StartPosition = FormStartPosition.CenterParent;
            if (expform.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (this.m_actexp.ReturnType != null && this.m_actexp.ReturnType.DBID != expform.RusultExp.ReturnType.DBID)
                {
                    //���������
                    this.listBox1.Items.Clear();
                }
                this.m_actexp = expform.RusultExp;
                this.m_Vexp.ReturnType = this.m_actexp.ReturnType;      //�ɱ����͵ķ���ֵ�ͱ��ʽһ��
                this.textBoxX1.Text = this.m_actexp.ToString();
                this.m_bChanged = true;
            }
            else
            {
                return;
            }

            //�Զ���ӷ�֧
            if (this.m_actexp != null && this.m_actexp.ReturnType != null)
            {
                //�������ö�ٵ���������
                if (this.m_actexp.ReturnType.isEnum)
                {
                    ConstExp[] cexpArray = ExpManager.GetConstExpByReturnType(this.m_actexp.ReturnType);
                    if (cexpArray != null)
                    {
                        foreach (ConstExp cexp in cexpArray)
                        {
                            ActionExp expa = ExpManager.CreateNewActionExp(new GameAPIDefine(FixedValueProvider.COMPARE_API_ID));
                            expa.API.ArgValues[0] = m_Vexp;           //��ֵΪ�ɱ�����
                            expa.API.ArgValues[1] = cexp;             //��ֵΪ����ֵ
                            expa.strText = m_Vexp.ToString() + "==" + cexp.ToString();
                            if (!listBox1.Items.Contains(expa))
                            {
                                this.listBox1.Items.Add(expa);
                                this.m_bChanged = true;
                            }
                        }
                    }
                }

                //�������OpenWindow��ѡ��
                if (this.m_actexp is ActionExp)
                {
                    ActionExp aexp = this.m_actexp as ActionExp;
                    if (aexp.API.DBID == FixedValueProvider.OPENWINDOW_ID)          //��ѡ���
                    {
                        Exp dialogExp = aexp.API.ArgValues[2];      //����������
                        if (dialogExp is ConstExp)                  //�����ʽ�ı�������ǳ����Ŵ���
                        {
                            string strDialog = (dialogExp as ConstExp).DBValue;
                            Regex reg = new Regex(@"(?!=<)\$[^>]*(?=>)");
                            foreach (Match mat in  reg.Matches(strDialog))
                            {
                                string strSelection = mat.Value.TrimStart(new char[] { '$', 'C', ' ' });
                                ActionExp expa = ExpManager.CreateNewActionExp(new GameAPIDefine(FixedValueProvider.COMPARE_API_ID));
                                expa.API.ArgValues[0] = m_Vexp;           //��ֵΪ�ɱ�����
                                ConstExp cexp = new ConstExp(strSelection, new TypeDefine(FixedValueProvider.TYPE_STRING_ID));
                                m_Vexp.ReturnType = cexp.ReturnType;
                                expa.API.ArgValues[1] = cexp;             //��ֵΪ�ַ���
                                expa.strText = m_Vexp.ToString() + "==" + cexp.ToString();
                                if (!listBox1.Items.Contains(expa))
                                {
                                    this.listBox1.Items.Add(expa);
                                    this.m_bChanged = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        private void btnDel_Click(object sender, EventArgs e)
        {
            if (this.listBox1.SelectedItem != null)
            {
                if (MessageBox.Show("Ҫɾ�����ʽ��ȷ��һ���Լ��϶���", "ȷ��", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                {
                    this.listBox1.Items.Remove(this.listBox1.SelectedItem);
                    this.m_bChanged = true;
                }
            }
        }

        private void btnMoveDown_Click(object sender, EventArgs e)
        {
            if (this.listBox1.SelectedIndex >= 0 && this.listBox1.SelectedIndex < this.listBox1.Items.Count)
            {
                int nIndex = this.listBox1.SelectedIndex;
                if (nIndex == this.listBox1.Items.Count - 1) return;
                object o = this.listBox1.SelectedItem;
                this.listBox1.Items.Remove(o);
                this.listBox1.Items.Insert(nIndex + 1, o);
                this.listBox1.SelectedItem = o;
            }  
        }

        private void btnMoveUp_Click(object sender, EventArgs e)
        {
            if (this.listBox1.SelectedItem != null && this.listBox1.SelectedIndex > 0)
            {
                int nIndex = this.listBox1.SelectedIndex;
                object o = this.listBox1.SelectedItem;
                this.listBox1.Items.Remove(o);
                this.listBox1.Items.Insert(nIndex - 1, o);
                this.listBox1.SelectedItem = o;
            }   
        }

        private void listBox1_DoubleClick(object sender, EventArgs e)
        {
            if (listBox1.SelectedItem != null)
            {
                ActionExp expa = listBox1.SelectedItem as ActionExp;
                ExpForm expform = new ExpForm(expa, this.m_gameEvent, true, expa.ReturnType, m_Vexp);
                expform.StartPosition = FormStartPosition.CenterParent;
                if (expform.ShowDialog() == DialogResult.OK)
                {
                    int ind = listBox1.SelectedIndex;
                    listBox1.Items.RemoveAt(ind);
                    listBox1.Items.Insert(ind, expform.RusultExp);
                    this.m_bChanged = true;
                }
            }
        }

        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (this.m_bChanged == false)
            {
                this.DialogResult = DialogResult.Cancel;
                this.Close();
            }
            else
            {
                this.m_elist.Clear();
                for (int i = 0; i < listBox1.Items.Count; i++)
                {
                    this.m_elist.Add(listBox1.Items[i] as Exp);
                }
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
        }

        private void buttonX2_Click(object sender, EventArgs e)
        {

        }
    }
}