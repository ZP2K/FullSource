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
    public partial class PreviewForm : Form
    {
        SqlConnection conn; // ���ݿ�����
        string m_TableName;
        DataTable m_tbl;

        public PreviewForm(SqlConnection conn, string tableName)
        {
            this.conn = conn;
            InitializeComponent();
            InitData(tableName);
            UpdateNullRowColor();
        }

        private void UpdateNullRowColor()
        {
//             foreach (DataGridViewRow gridrow in this.dataGridView1.Rows)
//             {
//                 DataRow datarow = m_tbl.Rows[gridrow.Index];
//                 if (datarow != null)
//                 {
//                     gridrow.DefaultCellStyle.BackColor = Color.Red;
//                     
//                 }
//             }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData(string tableName)
        {
            m_TableName = tableName;
            m_tbl = null;
            try
            {
                string sql = string.Format("SELECT * FROM {0}", tableName);
                //m_tbl = Helper.GetDataTable(sql, conn);
                m_tbl = Helper.GetDataTableProxy(tableName, conn);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڳ�ʼ����Ԥ������ʱ����sql�쳣: " + ex.ToString());
                return;
            }
            dataGridView1.DataSource = m_tbl;
            AutoEnlargeSize();
        }

        /// <summary>
        /// �����������ٵ�ʱ���Զ������п�
        /// </summary>
        private void AutoEnlargeSize()
        {
            int columnNumber = dataGridView1.Columns.Count;
            int columnWidth = dataGridView1.Columns[0].Width;
            int width = dataGridView1.Width;
            if(columnWidth * columnNumber < width)
            {
                dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            }
        }

        private void buttonX1_Click(object sender, EventArgs e) // ɾ����
        {
            try
            {
                DataGridViewRow row = dataGridView1.CurrentRow;
                if (row != null)
                {
                    // �ҳ����е�key
                    string strKey = string.Empty;
                    object[] keys = new object[m_tbl.PrimaryKey.Length];
                    int i = 0;
                    foreach (DataColumn col in m_tbl.PrimaryKey)
                    {
                        keys[i] = row.Cells[col.ColumnName].Value;
                        strKey += keys[i].ToString();
                        strKey += ",";
                        i++;
                    }
                    strKey = strKey.Substring(0, strKey.Length - 1);
                    string strQ = string.Format("��ȷ��Ҫɾ����ֵΪ {0} ������", strKey);
                    DialogResult dr = MessageBox.Show(strQ, "ȷ��ɾ��", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                    if (dr == DialogResult.Yes)
                    {
                        // ɾ��֮
                        DataRow rowToDel = m_tbl.Rows.Find(keys);
                        if (rowToDel != null)
                        {
                            rowToDel.Delete();
                            string sql = string.Format("SELECT * FROM {0}", m_TableName);
                            /*
                            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
                            SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                            adp.DeleteCommand = cmdBuilder.GetDeleteCommand();
                            int val = adp.Update(m_tbl);*/

                            int val = Helper.SaveTable(m_tbl, sql, conn);

                            m_tbl.AcceptChanges();
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                string err = string.Format("ɾ��ʧ�ܡ�������ϢΪ��{0}", ex.Message);
                MessageBox.Show(err);
            }
        }

        private void buttonX2_Click(object sender, EventArgs e) // �ر�
        {
            this.Dispose();
        }
    }
}