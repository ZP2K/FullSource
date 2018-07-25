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
    public partial class ChooseForm : Form
    {
        string tableName; // ��������
        SqlConnection conn; // sql����

        /// <summary>
        /// ���캯��
        /// </summary>
        public ChooseForm(SqlConnection conn)
        {
            this.conn = conn;
            InitializeComponent();
            InitData();
        }

        /// <summary>
        /// ������������
        /// </summary>
        public string TableName
        {
            get
            {
                return tableName;
            }
        }

        /// <summary>
        /// ����б�����
        /// </summary>
        private void InitData()
        {
            /* remoting
            string sql = "SELECT name FROM sysobjects WHERE xtype = 'u' ORDER BY name";
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                while(reader.Read())
                {
                    string tableName = reader[0].ToString().Trim();
                    if (tableName != "" && !tableName.StartsWith("sys"))
                    {
                        comboBoxEx1.Items.Add(tableName);
                    }
                }
            }
            */

            try
            {
                DataTable table = Helper.GetDataTableProxy("sysobjects", "xtype = 'u'", "name", conn);
                foreach (DataRow row in table.Rows)
                {
                    string tableName = row["name"] as string;
                    if (tableName != "" && !tableName.StartsWith("sys"))
                    {
                        comboBoxEx1.Items.Add(tableName);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("��ȡ������Ϣʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                /* remoting
                reader.Close();
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
                 * */
            }
        }

        /// <summary>
        ///  �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string text = comboBoxEx1.Text;
            if(text == "")
            {
                MessageBox.Show("����������Ϊ��!");
                return;
            }
            tableName = text;
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// �û�ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }
    }
}