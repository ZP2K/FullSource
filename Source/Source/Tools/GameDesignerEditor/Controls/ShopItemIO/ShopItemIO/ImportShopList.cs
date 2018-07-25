using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Collections;
using System.Data;
using System.Windows.Forms;

namespace ShopItemIO
{
    class ImportShopList
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
        public ImportShopList(SqlConnection conn, ArrayList fields, string[] values)
        {
            this.conn = conn;
            this.fields = fields;
            this.values = values;
        }

        /// <summary>
        /// ������Ʒ�ļ۸�
        /// </summary>
        /// <returns>�Ƿ���³ɹ�</returns>
        public bool UpdateItemPrice()
        {
            bool success = true; // �����Ƿ�ɹ�
            Hashtable indexTable = new Hashtable();
            SqlTransaction transaction = null;
            List<string> updateFieldList = new List<string>();

            updateFieldList.Add("Price");
            updateFieldList.Add("IsValid");
            updateFieldList.Add("CoinType1");
            updateFieldList.Add("CoinAmount1");
            updateFieldList.Add("Contribution");
            updateFieldList.Add("Prestige");
            updateFieldList.Add("AchievementPoint");

            for (int i = 0; i < fields.Count; i++)
            {                                
                indexTable[fields[i].ToString()] = i;
            }

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = "SELECT * FROM tbl_npc_shopitem";
                DataTable itemTable = GetDataTable(sqlString);
                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;
                bool firstLine = true;

                foreach (string s in values)
                {
                    if (s != "")
                    {
                        if(firstLine) // ������һ��(������)
                        {
                            firstLine = false;
                            continue;
                        }

                        string[] data = s.Split('\t');
                        string type = data[(int)indexTable["TabType"]];
                        string id = data[(int)indexTable["ItemIndex"]];
                        string durability = data[(int)indexTable["Durability"]];                        

                        DataRow[] rows = itemTable.Select(string.Format("TabType = '{0}' AND ItemIndex = '{1}' AND Durability = {2}",
                                                                        type, id, durability));
                        foreach(DataRow row in rows)
                        {
                            foreach (string fieldName in updateFieldList)
                            {
                                string fieldValue = data[(int)indexTable[fieldName]];

                                if (row[fieldName].ToString() != fieldValue)
                                {
                                    if (fieldValue == "")
                                    {
                                        sqlString = string.Format("UPDATE tbl_npc_shopitem SET [{0}] = NULL WHERE TabType = '{1}' AND ItemIndex = '{2}' AND Durability = {3}",
                                                                  fieldName, type, id, durability);
                                    }
                                    else
                                    {
                                        sqlString = string.Format("UPDATE tbl_npc_shopitem SET [{0}] = '{1}' WHERE TabType = '{2}' AND ItemIndex = '{3}' AND Durability = {4}",
                                                                  fieldName, fieldValue, type, id, durability);
                                    }

                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();
                                }
                            }
                        }
                    }
                }

                transaction.Commit();
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڸ����̵���ߵ���Ϣʱ����sql�쳣: " + ex.ToString());
                success = false;

                if (transaction != null)
                {
                    transaction.Rollback();
                }                
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            MessageBox.Show("�̵������Ϣ���³ɹ�!");

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
