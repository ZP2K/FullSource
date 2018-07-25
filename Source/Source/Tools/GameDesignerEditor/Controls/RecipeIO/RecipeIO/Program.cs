using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Data;
using System.Collections;
using System.Windows.Forms;
using System.IO;

namespace RecipeIO
{
    public class Program
    {
        private ArrayList fields; // ��������
        private string[] values; // ����������
        private SqlConnection conn; // sql����
        private string filePath; // �ļ�·��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">���ݿ�����</param>
        /// <param name="filePath">�ļ�·��</param>
        /// <param name="fields">��������</param>
        /// <param name="values">����������</param>
        public Program(SqlConnection conn, string filePath, ArrayList fields, string[] values)
        {
            this.conn = conn;
            this.filePath = filePath;
            this.fields = fields;
            this.values = values;
        }

        /// <summary>
        /// ��������ַ���
        /// </summary>
        /// <param name="s">�����ַ���</param>
        /// <returns>������ַ���</returns>
        private string FullFillNumber(string s)
        {
            string newString = s;

            for(int i = s.Length; i < 3; i++)
            {
                newString = "0" + newString;
            }

            return newString;
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <returns>�Ƿ���ɹ�</returns>
        public bool Import()
        {
            bool success = true;

            DataTable table = null;
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0}", "CraftBelongName");
                table = GetDataTable(sqlString);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("��ȡ�䷽���ñ�����ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            // ����������
            int belongIndex = 0;
            for(int i = 0; i < fields.Count; i++)
            {
                if(fields[i].ToString() == "Belong")
                {
                    belongIndex = i;
                    break;
                }
            }

            // ��������
            fields.Add("BelongName");           
            fields.Add("filename"); 
            fields.Add("filerealname");
            fields.Add("fileindex");

            // ����������
            string fileName = Path.GetFileName(filePath);
            string[] data;
            string className = "";
            string fileIndex = "";
            string belongID = "";
            bool firstLine = true;

            for (int i = 0; i < values.Length; i++)
            {
                string s = values[i];
                if(firstLine) // �Զ�������һ��
                {
                    s += "\tBelongName\tfilename\tfilerealname\tfileindex";
                    values[i] = s;
                    firstLine = false;
                    continue;
                }

                
                if (s != "") // �����յ�������
                {
                    switch (fileName)
                    {
                        case "cooking.tab":
                            {
                                className = "���";
                                fileIndex = "4";
                                break;
                            }
                        case "tailoring.tab":
                            {
                                className = "����";
                                fileIndex = "5";
                                break;
                            }
                        case "founding.tab":
                            {
                                className = "����";
                                fileIndex = "6";
                                break;
                            }
                        case "medicine.tab":
                            {
                                className = "ҽ��";
                                fileIndex = "7";
                                break;
                            }
                        default:
                            {
                                MessageBox.Show(string.Format("���ݴ��󣬵�ǰ�ļ���{0}��Ч��", fileName));
                                success = false;
                                break;
                            }
                    }

                    if (success)
                    {
                        data = s.Split(new char[] { '\t' });
                        belongID = data[belongIndex];

                        DataRow[] rows = table.Select(string.Format("ProfessionID = {0} AND BelongID = {1}", fileIndex, belongID));
                        if (rows.Length > 0)
                        {
                            s += "\t";
                            s += FullFillNumber(belongID) + "_" + rows[0]["BelongName"].ToString() + "\t";
                            s += className + "\t";
                            s += fileName + "\t";
                            s += fileIndex;
                        }
                        else
                        {
                            if(belongID == "0") // Ĭ����
                            {
                                s += "\t";
                                s += FullFillNumber(belongID) + "_�޷���\t";
                                s += className + "\t";
                                s += fileName + "\t";
                                s += fileIndex;
                            }
                            else
                            {
                                MessageBox.Show(string.Format("���ݴ���û���ҵ�ProfessionID = {0}��BelongID = {1}��BelongName��",
                                                                fileIndex, belongID));
                                success = false;
                            }                            
                        }

                        values[i] = s;
                    }
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
