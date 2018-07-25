using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace AddNpcActionNode
{
    class Helper
    {
        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sql">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        static public DataTable GetDataTable(string sql, SqlConnection conn)
        {
            DataTable tbl = null;

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                tbl = ds.Tables[0];
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڲ�ѯ���ݱ�����ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return tbl;
        }
    }
}
