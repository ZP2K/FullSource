using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Collections;

namespace FlowChartVarialbe
{
    public partial class VariableForm : DevComponents.DotNetBar.Office2007Form
    {
        public VariableForm()
        {
            InitializeComponent();
        }
        public VariableForm(string typeid, XmlDataDocument xmlDoc, ArrayList UsedVariablesList, string strOldValue, string strTypeName)
        {
            InitializeComponent();
            m_XmlDoc = new XmlDataDocument();
            m_strTypeId = typeid;
            m_XmlDoc = xmlDoc;
            m_arylstUsedVariables = UsedVariablesList;
            lbTypeName.Text = strTypeName;
            InitListBox();
            if (strOldValue != "" && lboxVarialbes.FindString(strOldValue) == ListBox.NoMatches)
            {
                lboxVarialbes.Items.Add(strOldValue);
                AddNewNode(strOldValue);
            }
            lboxVarialbes.SelectedItem = strOldValue;
        }

        /// <summary>
        /// ���ر�ѡ�еı�����
        /// </summary>
        /// <returns></returns>
        public string RtnChoosedName()
        {
            string strChoosedName = lboxVarialbes.SelectedItem.ToString();

            return strChoosedName;
        }
        public XmlDataDocument RtnResultXml()
        {
            return m_XmlDoc;
        }

        XmlDataDocument m_XmlDoc;
        string m_strTypeId;
        ArrayList m_arylstUsedVariables;

        /// <summary>
        /// ��ʼ��Lisbox������
        /// </summary>
        protected void InitListBox()
        {
            foreach (object obj in m_arylstUsedVariables)
            {
                lboxVarialbes.Items.Add(obj);
            }
            string strPath = string.Format("//variable[typeid='{0}']/name", m_strTypeId);
            XmlNodeList nodelist = m_XmlDoc.DocumentElement.SelectNodes(strPath);
            foreach (XmlNode node in nodelist)
            {
                lboxVarialbes.Items.Add(node.FirstChild.Value);
            }
        }

        private void lboxVarialbes_SelectedIndexChanged(object sender, EventArgs e)
        {
            int nSelectIndex = lboxVarialbes.SelectedIndex;
            if (nSelectIndex == -1)
            {
                return;
            }
            string strname = lboxVarialbes.SelectedItem.ToString();
            bool bCanEdit = false;

            bCanEdit = HasNode(strname);
            btnReName.Enabled = bCanEdit;
            btnDelete.Enabled = bCanEdit;
            lbChoosedName.Text = strname;           

        }

        /// <summary>
        /// �ж�strname�Ƿ���û�б�ʹ��
        /// </summary>
        /// <param name="strname"></param>
        /// <returns></returns>
        protected bool HasNode(string strname)
        {
            string strPath = string.Format("//variable[name='{0}']", strname);
            XmlNode root = m_XmlDoc.DocumentElement.SelectSingleNode(strPath);

            return (root != null);
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (lboxVarialbes.SelectedIndex == -1)
            {
                MessageBox.Show("��û��ѡ���κα���", "����",
                     MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (HasNode(lboxVarialbes.SelectedItem.ToString()))
            {
                DeleteNode(lboxVarialbes.SelectedItem.ToString());
            }
            
            this.DialogResult = DialogResult.OK;
        }

        private void btnCancle_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void btnAddNew_Click(object sender, EventArgs e)
        {
            string strNewVariableName = "";
            string strTip = "";
            strNewVariableName = tbxNewVariable.Text;
            if (strNewVariableName == "")
            {
                MessageBox.Show("�������µı�������", "����",
                     MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else if (IsExisted(strNewVariableName))
            {
                strTip = "����" + strNewVariableName + "�Ѿ�����";
                MessageBox.Show(strTip, "����",
                     MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                AddNewNode(strNewVariableName);
                lboxVarialbes.Items.Add(strNewVariableName);
                lboxVarialbes.SelectedItem = strNewVariableName;
                lbChoosedName.Text = strNewVariableName;
            }

           
        }

        private void btnReName_Click(object sender, EventArgs e)
        {
            string strNewVariableName = "";
            string strTip = "";
            
            strNewVariableName = tbxNewVariable.Text;            

            if (strNewVariableName == "")
            {
                MessageBox.Show("�������µı�������", "����",
                     MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else if (lboxVarialbes.SelectedIndex == -1)
            {
                MessageBox.Show("����ѡ��Ҫ�������ı�����", "����",
                     MessageBoxButtons.OK, MessageBoxIcon.Error);

            }
            else if (IsExisted(strNewVariableName))
            {
                strTip = "����" + strNewVariableName + "�Ѿ�����";
                MessageBox.Show(strTip, "����",
                     MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                int nChoosedIndex = 0; 
                string strOldVariableName = "";                

                nChoosedIndex = lboxVarialbes.SelectedIndex;
                strOldVariableName = lboxVarialbes.Items[nChoosedIndex].ToString();
                strTip = "ȷ���ѱ���" + strOldVariableName + "�޸�Ϊ��" + strNewVariableName + "?";
                if (MessageBox.Show(strTip, "ȷ���޸�", 
                    MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
                {
                    ChangeName(strNewVariableName, strOldVariableName);
                    lboxVarialbes.Items.Clear();
                    InitListBox();
                    lboxVarialbes.SelectedItem = strNewVariableName;
                    lbChoosedName.Text = strNewVariableName;

                } 
            }

        }

        /// <summary>
        /// �ж�strVariableName�Ƿ��Ѿ�����
        /// </summary>
        /// <param name="strVarialbeName"></param>
        /// <returns></returns>
        protected bool IsExisted(string strVarialbeName)
        {
            int i = 0;
            int nCnt = 0;
            string strItem = "";
            nCnt = lboxVarialbes.Items.Count;
            for (i = 0; i < nCnt; i++)
            {
                strItem = lboxVarialbes.Items[i].ToString();
                if (strItem == strVarialbeName)
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// ���strNewVariableName��m_Xmldoc
        /// </summary>
        /// <param name="strNewVariableName"></param>
        protected void AddNewNode(string strNewVariableName)
        {
            XmlElement NewVariable = m_XmlDoc.CreateElement("variable");
            XmlElement NewName = m_XmlDoc.CreateElement("name");
            XmlElement NewTypeId = m_XmlDoc.CreateElement("typeid");

            XmlText name = m_XmlDoc.CreateTextNode(strNewVariableName);
            XmlText typeid = m_XmlDoc.CreateTextNode(m_strTypeId);

            NewVariable.AppendChild(NewName);
            NewVariable.AppendChild(NewTypeId);
            NewName.AppendChild(name);
            NewTypeId.AppendChild(typeid);

            m_XmlDoc.DocumentElement.AppendChild(NewVariable);
        }

        protected void ChangeName(string strNewVariableName, string strOldVariableName)
        {
            string strPath = string.Format("//variable[name='{0}']/name", strOldVariableName);
            try
            {
                XmlNode changeNode = m_XmlDoc.DocumentElement.SelectSingleNode(strPath);
                changeNode.FirstChild.Value = strNewVariableName;
            }
            catch (System.Exception e)
            {
                MessageBox.Show(e.ToString());
            }
            
        }

        protected void DeleteNode(string strVariableName)
        {
            string strPath = string.Format("//variable[name='{0}']", strVariableName);
            XmlNode variable = m_XmlDoc.DocumentElement.SelectSingleNode(strPath);
            if (variable != null)
            {
                m_XmlDoc.DocumentElement.RemoveChild(variable);
            }            
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            if (lboxVarialbes.SelectedIndex == -1)
            {
                MessageBox.Show("����ѡ��Ҫɾ���ı�����","����",
                     MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;

            }
            string strVariableName = lboxVarialbes.SelectedItem.ToString();
            string strTip = "ɾ������" +strVariableName + "?";
            if (MessageBox.Show(strTip, "ȷ��ɾ��",
                MessageBoxButtons.OKCancel,  MessageBoxIcon.Question) == DialogResult.OK)
            {                
                DeleteNode(strVariableName);
                lboxVarialbes.Items.Clear();
                InitListBox();
                if (lboxVarialbes.SelectedIndex == -1)
                {
                    lbChoosedName.Text = "δѡ���κα���";
                }
            }
        }

        /// <summary>
        /// �����ܲ��Ϸ��ַ�
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbxNewVariable_KeyPress(object sender, KeyPressEventArgs e)
        {
            char c = e.KeyChar;
            if (!IsLegal(c))
            {
                e.KeyChar = '\0';
            }
            
        }
        /// <summary>
        /// �ж��ַ��Ƿ�Ϸ�
        /// </summary>
        /// <param name="c"></param>
        /// <returns></returns>
        protected bool IsLegal(char c)
        {
            switch (c)
            {
                case '\'':
                case '<':
                case '\"':
                case '\\':
                case '>':
                    return false;                    
            }
            return true;
        }

        
    }
}