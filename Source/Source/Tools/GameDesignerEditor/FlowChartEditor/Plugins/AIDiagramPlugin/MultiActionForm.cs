using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DevComponents.DotNetBar;
using DevComponents.AdvTree;

namespace AIDiagramPlugin
{
    public partial class MultiActionForm : Office2007Form
    {
        private AI_MultiAction m_maction; // ������ 
        private int groupIndex = 0; // �Զ����ɶ������
        private int graphElementID; // ��ǰ�༭��ͼԪID

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="maction">��������</param>
        public MultiActionForm(AI_MultiAction maction)
        {
            InitializeComponent();
            this.DialogResult = DialogResult.Cancel;

            if (maction != null)
            {
                this.m_maction = maction;

                if (maction.ActionList == null)
                {
                    maction.ActionList = new List<AI_SingleAction>();
                }

                foreach (AI_SingleAction action in maction.ActionList)
                {
                    listBox1.Items.Add(action.Clone());
                }
            }                       
        }

        /// <summary>
        /// ��ǰ�༭ͼԪ��ID
        /// </summary>
        public int GraphElementID
        {
            set
            {
                graphElementID = value;
            }
        }        

        /// <summary>
        /// ��ʼ���Զ��嶯���Զ�����������
        /// </summary>
        /// <param name="aiSingleAction">ai����</param>
        private void InitActionIndex(AI_SingleAction aiSingleAction)
        {
            int maxID = 0;
            
            if (aiSingleAction != null)
            {
                string actionName = aiSingleAction.Action.Ename;
                int index = actionName.LastIndexOf("_");

                if (index > 0)
                {
                    int tempID = -1;

                    if (int.TryParse(actionName.Substring(index + 1, actionName.Length - index - 1), out tempID))
                    {
                        maxID = tempID + 1;
                    }
                }
            }
            else
            {
                foreach (object o in listBox1.Items)
                {
                    AI_SingleAction singleAction = o as AI_SingleAction;

                    if (singleAction != null)
                    {
                        AI_Action aiAction = singleAction.Action;

                        if (int.Parse(aiAction.ActionID) > 2000)
                        {
                            string actionName = aiAction.Ename;
                            int index = actionName.LastIndexOf("_");

                            if (index > 0)
                            {
                                int tempID = -1;

                                if (int.TryParse(actionName.Substring(index + 1, actionName.Length - index - 1), out tempID))
                                {
                                    maxID = tempID + 1;
                                }
                            }
                        }
                    }
                }
            }            

            groupIndex = maxID;
        }
        
        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.m_maction.ActionList.Clear();
            foreach (object o in listBox1.Items)
            {
                this.m_maction.ActionList.Add(o as AI_SingleAction);
            }

            this.Close();
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// ��Ӷ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void btnAdd_Click(object sender, EventArgs e)
        {
            // ��ʼ���Զ�����������
            InitActionIndex(null);

            AI_SingleAction singleAction = new AI_SingleAction();
            SingleActionForm singleActionForm = new SingleActionForm();

            singleActionForm.GraphElementID = graphElementID;
            singleActionForm.InActionGroup = true;
            singleActionForm.GroupIndex = groupIndex;
            singleActionForm.Reload(singleAction);
            singleActionForm.ShowDialog();

            if (singleActionForm.EditSuccess)
            {
                listBox1.Items.Add(singleAction);
            }         
        }

        /// <summary>
        /// ɾ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void btnDel_Click(object sender, EventArgs e)
        {
            if (this.listBox1.SelectedItem != null)
            {
                if (MessageBox.Show("ȷ��Ҫɾ����ǰ������", "ɾ������", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                {
                    this.listBox1.Items.Remove(this.listBox1.SelectedItem);
                }
            }
        }

        /// <summary>
        /// �����ƶ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
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

        /// <summary>
        /// �����ƶ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
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

        /// <summary>
        /// ���ƶ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonCopy_Click(object sender, EventArgs e)
        {
            ICloneable copyFromExp = this.listBox1.SelectedItem as ICloneable;

            if (copyFromExp != null)
            {
                object newExp = copyFromExp.Clone();
                this.listBox1.Items.Add(newExp);
                this.listBox1.SelectedItem = newExp;
            }
        }

        /// <summary>
        /// �༭����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void listBox1_DoubleClick(object sender, EventArgs e)
        {
            AI_SingleAction singleAction = this.listBox1.SelectedItem as AI_SingleAction;

            if (singleAction != null)
            {
                // ��ʼ���Զ�����������
                InitActionIndex(singleAction);

                SingleActionForm saForm = new SingleActionForm();
                saForm.GraphElementID = graphElementID;
                saForm.InActionGroup = true;
                saForm.GroupIndex = groupIndex;
                saForm.Reload(singleAction);
                saForm.ShowDialog();

                if (saForm.EditSuccess)
                {
                    int ind = listBox1.SelectedIndex;
                    listBox1.Items.RemoveAt(ind);
                    listBox1.Items.Insert(ind, singleAction);
                }
            }                
        }
    }
}