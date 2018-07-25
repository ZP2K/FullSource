using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Data;

namespace MasterIO
{
    class ImportFile
    {
        private SqlConnection conn; // sql����
        private ArrayList fields; // ��������
        private string[] values; // ����������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="fields">��������</param>
        /// <param name="values">����������</param>
        public ImportFile(SqlConnection conn, ArrayList fields, string[] values)
        {
            this.conn = conn;
            this.fields = fields;
            this.values = values;
        }

        /// <summary>
        /// ����ѧϰ�䷽�б�
        /// </summary>
        /// <returns>�Ƿ���ɹ�</returns>
        public bool ImportLearnRecipeList()
        {
            bool success = true; // �Ƿ���ɹ�
            string fieldName; // �ֶ�����
            string[] data; // ������
            int craftIDIndex = 0; // ����ID����
            int recipeIDIndex = 0; // �䷽ID����
            int nPriceIndex = 0; // �۸�����
            string craftID; // ����id
            string recipeID; // �䷽id
            string nPrice; // �۸�

            for (int i = 0; i < fields.Count; i++)
            {
                fieldName = fields[i].ToString();
                switch(fieldName)
                {
                    case "CraftID":
                        {
                            craftIDIndex = i;
                            break;
                        }
                    case "RecipeID":
                        {
                            recipeIDIndex = i;
                            break;
                        }
                    case "nPrice":
                        {
                            nPriceIndex = i;
                            break;
                        }
                }
            }

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0}", "tbl_master_skills");
                DataTable skillTable = GetDataTable(sqlString);
                DataRow[] rows;
                SqlCommand cmd = conn.CreateCommand();
                bool firstLine = true;

                foreach(string s in values)
                {
                    if(s != "") // ȥ���յ�������
                    {
                        if(firstLine) // ������һ��(������)
                        {
                            firstLine = false;
                            continue;
                        }

                        data = s.Split(new char[]{'\t'});
                        craftID = data[craftIDIndex];
                        recipeID = data[recipeIDIndex];
                        nPrice = data[nPriceIndex];
 
                        rows = skillTable.Select(string.Format("CraftID = '{0}' AND RecipeID = '{1}'", int.Parse(craftID), int.Parse(recipeID)));
                        if(rows.Length > 0) // �ҵ���Ӧ��������
                        {
                            if(rows[0]["nPrice"].ToString() != nPrice) // ��Ҫ���¼۸�
                            {
                                sqlString = string.Format("UPDATE {0} SET nPrice = {1} WHERE CraftID = {2} AND RecipeID = {3}",
                                    "tbl_master_skills", nPrice, craftID, recipeID);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();
                            }
                        }
                    }
                }

                MessageBox.Show("ѧϰʦ�����䷽�۸���³ɹ�!");
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڵ���ѧϰ�䷽�б�ʱ����sql�쳣: " + ex.ToString());
                success = false;
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return success;
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
    }
}
