using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using System.Data.SqlClient;

namespace XMLEditControl
{
    public class SSControl
    {
        //�༭�ű�������ֵΪ��ֵ�����߾�ֵ
        public string XMLDataEdit(string strCode, SqlConnection conn)
        {
            /*
            DataTable dt = new DataTable();
                        dt.Columns.Add("id");
                        dt.Columns.Add("name");
                        dt.Rows.Add(new string[] { "1", "����" });
                        dt.Rows.Add(new string[] { "2", "����" });
                        dt.Rows.Add(new string[] { "7", "���" });*/
            
            DataTable dt = GetDataTable(@"select id, description as 'name', isenum, isduplicate from changan.s3diagram_rtm.dbo.typedefine", conn);

            FrmXMLEdit fse = new FrmXMLEdit(strCode, dt);

            if( fse.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                return fse.GetScriptCode();
            }
            else
            {
                return strCode;
            }
        }

        private DataTable GetDataTable(string sqlString, SqlConnection conn)
        {
            DataTable tb = null;
            ConnectionState state = conn.State;

            try
            {
                conn.Open();
                SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                tb = ds.Tables[0];
            }
            catch (SqlException ex)
            {
                MessageBox.Show("SQL���ݿ����" + ex.Message);
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return tb;
        }
    }
}
