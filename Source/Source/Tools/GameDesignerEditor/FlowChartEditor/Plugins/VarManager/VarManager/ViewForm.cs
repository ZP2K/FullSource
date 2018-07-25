using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace VarManager
{
    public partial class ViewForm : Form
    {
        private DataTable table; // �������ݱ�
        private Hashtable bindTypeTable = new Hashtable(); // �����͵Ĺ�ϣ��
        private bool beginView = false; // �Ƿ��Ѿ���ʼ�鿴
        private Hashtable viewData = new Hashtable(); // ����������ϣ��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="table">�������ݱ�</param>
        public ViewForm(DataTable table)
        {
            this.table = table;
            InitializeComponent();
            InitData();
        }

        /// <summary>
        /// ����������ϣ��
        /// </summary>
        public Hashtable ViewData
        {
            get
            {
                return viewData;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData()
        {
            List<string> userNameList = new List<string>();
            List<string> varTypeList = new List<string>();
            List<string> bindTypeList = new List<string>();
            
            userNameList.Add("�����û�");
            varTypeList.Add("���б�������");
            bindTypeList.Add("���а�����");

            List<string> list = new List<string>();
            list.Add("���а󶨶���");
            bindTypeTable["���а�����"] = list;
            
            string userName;
            string varType;
            string bindType;
            string bindObject;

            foreach(DataRow row in table.Rows)
            {
                userName = row["UserName"] as string;
                varType = row["Type"] as string;
                bindType = row["bindType"] as string;
                bindObject = row["bindObject"] as string;

                if(!userNameList.Contains(userName))
                {
                    userNameList.Add(userName);
                }

                if(!varTypeList.Contains(varType))
                {
                    varTypeList.Add(varType);
                }

                if(!bindTypeList.Contains(bindType))
                {
                    bindTypeList.Add(bindType);
                    List<string> bindObjectList = new List<string>();
                    bindObjectList.Add("���а󶨶���");
                    bindObjectList.Add(bindObject);
                    bindTypeTable[bindType] = bindObjectList;
                }
                else
                {
                    List<string> bindObjectList = bindTypeTable[bindType] as List<string>;
                    if(!bindObjectList.Contains(bindObject))
                    {
                        bindObjectList.Add(bindObject);
                    }
                }                
            }

            foreach(string s in userNameList)
            {
                comboBoxEx1.Items.Add(s);
            }

            foreach(string s in varTypeList)
            {
                comboBoxEx2.Items.Add(s);
            }

            foreach(string s in bindTypeList)
            {
                comboBoxEx3.Items.Add(s);
            }

            comboBoxEx1.Text = "�����û�";
            comboBoxEx2.Text = "���б�������";
            comboBoxEx3.Text = "���а�����";
            comboBoxEx4.Text = "���а󶨶���";

            beginView = true;
        }


        /// <summary>
        /// �ı������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxEx3_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(beginView)
            {
                List<string> bindObjectList = bindTypeTable[comboBoxEx3.Text] as List<string>;
                comboBoxEx4.Items.Clear();

                foreach(string s in bindObjectList)
                {
                    comboBoxEx4.Items.Add(s);
                }

                comboBoxEx4.Text = "���а󶨶���";
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string userName = comboBoxEx1.Text;
            string varType = comboBoxEx2.Text;
            string bindType = comboBoxEx3.Text;
            string bindObject = comboBoxEx4.Text;

            if(userName == "" || varType == "" || bindType == "" || bindObject == "")
            {
                MessageBox.Show("������������Ϊ�գ�", "�������",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                viewData["userName"] = userName;
                viewData["type"] = varType;
                viewData["bindType"] = bindType;
                viewData["bindObject"] = bindObject;

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
    }
}