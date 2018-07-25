using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace ArgumentsEditor
{
    public partial class FieldSelectForm : Form
    {              
        private SqlConnection conn; // sql����
        private bool beginEdit = false; // �Ƿ�ʼ�༭
        private string value; // ֵ

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        public FieldSelectForm(SqlConnection conn)
        {
            this.conn = conn;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// 
        /// </summary>
        public string Value
        {
            get
            {
                return value;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            string sqlString = string.Format("SELECT name FROM sysobjects WHERE xtype = 'U' ORDER BY name");
            DataTable table = GetDataTable(sqlString);
            foreach(DataRow row in table.Rows)
            {
                string tableName = row[0].ToString();
                if(!tableName.StartsWith("sys"))
                {
                    comboBoxEx1.Items.Add(tableName);
                }
            }

            beginEdit = true;
        }        

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString)
        {
            DataTable table = null;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                table = ds.Tables[0];
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ���ݱ�ʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }           

            return table;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string tableName = comboBoxEx1.Text;
            string fieldName = comboBoxEx2.Text;

            if(tableName == "")
            {
                MessageBox.Show("��������Ϊ�գ�", "���ֶ�ѡ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if(fieldName == "")
            {
                MessageBox.Show("ȡֵ�ֶβ���Ϊ�գ�", "���ֶ�ѡ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if(listBox2.Items.Count == 0)
            {
                MessageBox.Show("������Ҫһ�������ֶ���������������", "���ֶ�ѡ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            string fields = "";
            foreach(object o in listBox2.Items)
            {
                fields += string.Format("{0},", o);
            }
            fields += fieldName;

            value = string.Format("{0}[{1}]", tableName, fields);
            DialogResult = DialogResult.OK;
            this.Close();
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
        /// �����ݸı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxEx1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(beginEdit)
            {
                comboBoxEx2.Items.Clear();
                listBox1.Items.Clear();
                string tableName = comboBoxEx1.Text;
                string sqlString = string.Format("SELECT TOP 1 * FROM {0}", tableName);
                DataTable table = GetDataTable(sqlString);
                foreach(DataColumn c in table.Columns)
                {
                    comboBoxEx2.Items.Add(c.ColumnName);
                    listBox1.Items.Add(c.ColumnName);
                }
            }
        }

        /// <summary>
        /// ���ӷ����ֶ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            object field = listBox1.SelectedItem;
            if(field != null)
            {
                listBox1.Items.Remove(field);
                listBox2.Items.Add(field);
                listBox2.SelectedItem = field;
            }
        }

        /// <summary>
        /// ɾ�������ֶ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            object field = listBox2.SelectedItem;
            if (field != null)
            {
                listBox2.Items.Remove(field);
                listBox1.Items.Add(field);
                listBox1.SelectedItem = field;
            }
        }
    }
}