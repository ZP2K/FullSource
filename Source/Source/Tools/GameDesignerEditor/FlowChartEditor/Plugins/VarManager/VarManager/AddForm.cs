using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;
using System.Net;
using TableSelector;

namespace VarManager
{
    public partial class AddForm : Form
    {
        private SqlConnection conn; // sql����
        private Hashtable newVarData = new Hashtable(); // �������������� 
         
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        public AddForm(SqlConnection conn)
        {
            this.conn = conn;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��������������
        /// </summary>
        public Hashtable NewVarData
        {
            get
            {
                return newVarData;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sql">sql���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sql)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            System.Data.DataTable tbl = ds.Tables[0];
            return tbl;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string bindType = comboBoxEx2.Text;
            string bindObject = textBoxX3.Text;
            string varType = comboBoxEx1.Text.Replace(" ", "");
            string name = textBoxX1.Text;
            string description = textBoxX2.Text;
            string bindID = textBoxX3.Tag as string;
            string stringLength = integerInput1.Text; 

            // �Զ����bindIDֵ
            switch(bindType)
            {
                case "���":
                    {
                        bindID = "0";
                        break;
                    }
            }

            if(bindType == "" || (bindType == "����" && bindObject == "") ||
                varType == "" || name == "" || (varType == "�ַ���" && integerInput1.Text == ""))
            {
                MessageBox.Show("������Ϣ��������", "��ӱ���",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                newVarData["name"] = name;
                newVarData["userName"] = Dns.GetHostName();
                newVarData["type"] = varType;
                newVarData["description"] = description;
                newVarData["bindType"] = bindType;
                newVarData["bindObject"] = bindObject;
                newVarData["bindID"] = bindID;
                newVarData["stringLength"] = stringLength;

                DialogResult = DialogResult.OK;
                this.Close();
            }
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// ѡ��󶨶���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">ʱ�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            string bindType = comboBoxEx2.Text;
            string id = null;
            string value = null;

            switch(bindType)
            {
                case "����":
                    {
                        TableSelector.Program tableSelector = new TableSelector.Program();
                        bool selectFinished = tableSelector.Show(conn, "����ѡ����", "Select Name, ID FROM MapList", "Name", "ID", textBoxX3.Text);
                        if(selectFinished)
                        {
                            id = tableSelector.SelectedValue;
                            value = string.Format("{0} ({1})", tableSelector.SelectedValues["Name"], tableSelector.SelectedValues["ID"]);
                        }

                        break;
                    }
                case "���":
                    {
                        MessageBox.Show("Ĭ��Ϊ��ǰ��ң�����Ҫѡ��", "��ӱ���",
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                        break;
                    }
                case "":
                    {
                        MessageBox.Show("����ѡ������ͣ�", "��ӱ���",
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                        break;
                    }
            }

            textBoxX3.Text = value;
            textBoxX3.Tag = id;
        }

        /// <summary>
        /// �����͸ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxEx2_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch(comboBoxEx2.Text)
            {
                case "���":
                    {
                        textBoxX3.Text = "��ǰ���";
                        break;
                    }
                default:
                    {
                        textBoxX3.Text = "";
                        break;
                    }
            }
        }

        /// <summary>
        /// �������͸ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxEx1_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch(comboBoxEx1.Text)
            {
                case "�ַ���":
                    {
                        integerInput1.Enabled = true;
                        break;
                    }
                default:
                    {
                        integerInput1.Enabled = false;
                        break;
                    }
            }
        }
    }
}