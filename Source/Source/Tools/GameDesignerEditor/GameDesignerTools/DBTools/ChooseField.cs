using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace GameDesingerTools.DBTools
{
    public partial class ChooseField : Form
    {
        SqlConnection conn; // sql����
        List<string> columnNames = new List<string>(); // ѡ����������
        List<string> idList; // ���ڵ�ID����
        List<string> existList = new List<string>(); // ���ڵ���������
        List<string> list; // ������������ 

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="list">������������</param>
        /// <param name="existList">���ڵ���������</param>
        public ChooseField(SqlConnection conn, List<string> list, List<string> idList)
        {
            this.conn = conn;
            this.list = list;
            this.idList = idList;
            InitializeComponent();
            FillExistList();
            InitData();
        }

        /// <summary>
        /// ѡ��������������
        /// </summary>
        public List<string> ColumnNames
        {
            get
            {
                return columnNames;
            }
        }

        /// <summary>
        /// ��������������
        /// </summary>
        private void FillExistList()
        {
            //SqlCommand cmd;
            SqlDataReader reader = null;

            if(conn.State == ConnectionState.Closed) // ��sql����
            {
                conn.Open();
            }
            foreach (string s in idList)
            {
                try
                {
                    string sql = string.Format("SELECT {0} FROM {1} WHERE {2} = {3}", "fieldname", "sys_meta_info", "fieldid",
                        s);
                    string filter = string.Format("{0} = {1}", "fieldid", s);
                    /* remoting
                    cmd = new SqlCommand(sql, conn);
                    reader = cmd.ExecuteReader();
                    reader.Read();
                    existList.Add(reader[0].ToString().Trim());
                    */

                    DataTable table = Helper.GetDataTableProxy("sys_meta_info", filter, null, conn);
                    existList.Add(table.Rows[0]["fieldname"].ToString().Trim());
                }
                catch (SqlException ex)
                {
                    MessageBox.Show("��ȡ��ǰ����Ѵ�����ʱ����Sql�쳣: " + ex.ToString());
                }
                finally
                {
                    if (reader != null) // reader�Ƿ��ѹر�
                    {
                        reader.Close();
                    }
                }
            }
            if(conn.State == ConnectionState.Open) // �ر�sql����
            {
                conn.Close();
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData()
        {
            foreach(string s in list)
            {
                if(existList.Contains(s)) // �������Ѵ��ڽ�����������
                {
                    listBox2.Items.Add(s);
                }
                else 
                {
                    listBox1.Items.Add(s);
                }
            }
        }

        /// <summary>
        /// �������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            List<object> list = new List<object>(); 
            ListBox.SelectedObjectCollection items = listBox1.SelectedItems;

            if (items != null && items.Count != 0)
            {
                foreach(object o in items)
                {
                    list.Add(o);
                }
            }

            foreach(object o in list)
            {
                listBox2.Items.Add(o);
                listBox2.SelectedItem = o;
                listBox1.Items.Remove(o);
            }
        }

        /// <summary>
        /// ɾ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            List<object> list = new List<object>();
            ListBox.SelectedObjectCollection items = listBox2.SelectedItems;

            if (items != null && items.Count != 0)
            {
                foreach (object o in items)
                {
                    list.Add(o);
                }
            }

            foreach (object o in list)
            {
                listBox1.Items.Add(o);
                listBox1.SelectedItem = o;
                listBox2.Items.Remove(o);
            }
        }

        /// <summary>
        /// �����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            foreach(object o in listBox1.Items)
            {
                listBox2.Items.Add(o);
            }
            listBox1.Items.Clear();
        }

        /// <summary>
        /// ɾ����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            foreach (object o in listBox2.Items)
            {
                listBox1.Items.Add(o);
            }
            listBox2.Items.Clear();
        }

        /// <summary>
        /// ȷ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX5_Click(object sender, EventArgs e)
        {
            foreach(object o in listBox2.Items)
            {
                columnNames.Add(o.ToString());
            }
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// ȡ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX6_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }
    }
}