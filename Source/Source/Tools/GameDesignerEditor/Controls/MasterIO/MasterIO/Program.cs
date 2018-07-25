using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;
using System.Collections;
using System.Data.SqlClient;
using System.Data;
using System.Text;

namespace MasterIO
{
    public class Program
    {
        private string path; // �ļ�·��
        private string[] fields; // ����������
        private string[] values; // ����������
        private SqlConnection conn; // sql����
        private List<string> valuesList = new List<string>(); // ����������
        private bool loadedFields = false; // �Ƿ��Ѿ���ӹ��������� 

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="path">�ļ�·��</param>
        public Program(SqlConnection conn, string path)
        {
            this.conn = conn;
            this.path = path;
        }

        /// <summary>
        /// ������������
        /// </summary>
        public string[] Fields
        {
            get
            {
                return fields;
            }
        }

        /// <summary>
        /// ��������������
        /// </summary>
        public string[] Values
        {
            get
            {
                return values;
            }
        }

        /// <summary>
        /// ����ѧϰʦ��Ϣ
        /// </summary>
        public bool LoadMasterSkillFromPath()
        {
            bool loadSucess = true; // �Ƿ���ɹ�
            string rootFolder = GetLoadPath(); // ��Ŀ¼
            string appendFolder; // ��Ŀ¼
            string baseFolder; // ��ײ�Ŀ¼
            string filePath; // �ļ�Ŀ¼
            string id; // ѧϰʦID

            if (rootFolder == null) // �û�δѡ��ѧϰʦ�������ڵ�Ŀ¼
            {
                MessageBox.Show("û��ѡ��ѧϰʦ�������ڵ�Ŀ¼!");
                return false;
            }

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0}", "tbl_master_list");
                DataTable table = GetDataTable(sqlString);
                foreach(DataRow r in table.Rows)
                {
                    appendFolder = r["TabFile"].ToString().Replace('/', '\\'); // ת��·���ַ���
                    appendFolder = appendFolder.Substring(7, appendFolder.Length - 7); // ȥ��"master\"�ַ���
                    id = r["ID"].ToString();
                    filePath = Path.Combine(rootFolder, appendFolder);
                    baseFolder = Path.GetDirectoryName(appendFolder); 
                    LoadCraftSkill(filePath, baseFolder, id);
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡѧϰʦ����·��ʱ����sql�쳣: " + ex.ToString());
                loadSucess = false;
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            values = new string[valuesList.Count + 1];
            values[0] = "empty line";
            for(int i = 0; i < valuesList.Count; i++)
            {
                values[i + 1] = valuesList[i];
            }

            return loadSucess;
        }

        /// <summary>
        /// ��ȡѧϰʦ�������ڵ�Ŀ¼
        /// </summary>
        /// <returns>ѧϰʦ�������ڵ�Ŀ¼</returns>
        private string GetLoadPath()
        {
            string resultFolder = null;
            FolderBrowserDialog openFolderDialog1 = new FolderBrowserDialog();
            openFolderDialog1.Description = "��ѡ��ѧϰʦ���ܱ����ڵ�Ŀ¼";
            openFolderDialog1.SelectedPath = path;
            if(openFolderDialog1.ShowDialog() == DialogResult.OK)
            {
                resultFolder = openFolderDialog1.SelectedPath;
            }

            return resultFolder;
        }

        /// <summary>
        /// ��ȡ�䷽����
        /// </summary>
        /// <param name="id">�䷽id</param>
        /// <returns>�䷽����</returns>
        private string GetRecipeName(string recipeID, string professionID)
        {
            string recipeName = null;
            string professionName = null;

            switch(professionID)
            {
                case "4":
                    {
                        professionName = "���";
                        break;
                    }
                case "5":
                    {
                        professionName = "����";
                        break;
                    }
                case "6":
                    {
                        professionName = "����";
                        break;
                    }
                case "7":
                    {
                        professionName = "ҽ��";
                        break;
                    }
            }

            if(professionName == null) // û���ҵ���Ӧ��profession
            {
                return null;
            }

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT Name FROM {0} WHERE ID = '{1}' AND filename = '{2}'", "tbl_Recipe", recipeID, professionName);
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                object o = cmd.ExecuteScalar();
                if((o != null) && !(o is DBNull))
                {
                    recipeName = o.ToString();
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ�䷽��Ϣʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return recipeName;
        }

        /// <summary>
        /// ���ļ��ж�ȡ����
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <param name="folder">�ļ���</param>
        /// <param name="masterID">ѧϰʦID</param>
        private void LoadCraftSkill(string fileName, string folder, string masterID)
        {
            FileInfo fi = new FileInfo(fileName);
            if(fi.Exists)
            {
                try
                {
                    StreamReader sr = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                    string text = sr.ReadLine();
                    string craftName;
                    string[] data;
                    int index = 0;
                    string appendText;

                    while(text != null)
                    {
                        data = text.Split(new char[] {'\t'});

                        if(index == 0) // �����һ�У���������
                        {
                            if(!loadedFields) // ��û��д������
                            {             
                                fields = new string[4 + data.Length];
                                fields[0] = "foldername";
                                fields[1] = "filename";
                                fields[2] = "Name";
                                fields[3] = "MasterID";

                                for (int i = 0; i < data.Length; i++)
                                {
                                    fields[4 + i] = data[i];
                                }

                                loadedFields = true;
                            }
                        }
                        else // ����������
                        {
                            appendText = folder + "\t";
                            appendText += Path.GetFileName(fileName) + "\t";
                            craftName = GetRecipeName(data[1], data[2]);
                            if(craftName != null)
                            {
                                appendText += craftName + "\t";
                            }
                            else
                            {
                                appendText += "\t";
                            }
                            appendText += masterID + "\t";
                            valuesList.Add(appendText + text);
                        }

                        index++;
                        text = sr.ReadLine();
                    }
                }
                catch (IOException ex)
                {
                    MessageBox.Show("�ڶ�ȡ����ѧϰʦ���ñ��ļ�ʱ����IO�쳣: " + ex.ToString());
                }
            }
            else
            {
                MessageBox.Show(string.Format("�ļ�{0}������!", fileName));
            }
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