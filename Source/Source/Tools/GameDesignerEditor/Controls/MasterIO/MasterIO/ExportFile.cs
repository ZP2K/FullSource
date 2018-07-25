using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Data;
using System.Data.SqlClient;
using System.Collections;
using System.Windows.Forms;

namespace MasterIO
{
    class ExportFile
    {
        private SqlConnection conn; // sql����
        private ArrayList fields; // ����������
        private string[] values; // ����������
        private string rootPath; // ������Ŀ¼

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="rootPath">������Ŀ¼</param>
        /// <param name="fields">����������</param>
        /// <param name="values">����������</param>
        public ExportFile(SqlConnection conn, string rootPath, ArrayList fields, string[] values)
        {
            this.conn = conn;
            this.rootPath = rootPath;
            this.fields = fields;
            this.values = values;
        }

        /// <summary>
        /// ����ѧϰʦ�����ļ�
        /// </summary>
        /// <returns>�Ƿ񵼳��ɹ�</returns>
        public bool ExportMasterList()
        {
            bool success = true; // �Ƿ񵼳��ɹ�
            List<string> dataList = new List<string>(); // �������ݵ�����
            List<string> existMasterList = new List<string>(); // �Ѿ����ڵ�ѧϰʦ����
            string[] data; // ��������
            int folderNameIndex = 0; // �ļ�������
            int fileNameIndex = 0; // �ļ���������
            int masterIDIndex = 0; // ����������
            int idIndex = 0; // id����
            string fileName; // �ļ���
            string folderName; // �ļ����� 
            string masterID; // ������
            string filePath; // �ļ�·��
            string id; // id

            for (int i = 0; i < fields.Count; i++)
            {
                string fieldName = fields[i].ToString().TrimEnd(new char[] {'\r', '\n'});
                switch(fieldName)
                {
                    case "foldername":
                        {
                            folderNameIndex = i;
                            break;
                        }
                    case "filename":
                        {
                            fileNameIndex = i;
                            break;
                        }
                    case "MasterID":
                        {
                            masterIDIndex = i;
                            break;
                        }
                    case "id":
                        {
                            idIndex = i;
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

                string sqlString = string.Format("SELECT * FROM {0} ORDER BY ID", "tbl_master_list");
                DataTable masterTable = GetDataTable(sqlString);
                SqlCommand cmd = conn.CreateCommand();
                DataRow newRow;
                int maxID = 1;
                if (masterTable.Rows.Count > 0)
                {
                    maxID = int.Parse(masterTable.Rows[masterTable.Rows.Count - 1]["ID"].ToString()) + 1;
                } 

                foreach(string s in values)
                {
                    if(s != "") // ȥ��Ϊ�յ�������
                    {
                        data = s.Split('\t');
                        fileName = data[fileNameIndex];
                        folderName = data[folderNameIndex];
                        masterID = data[masterIDIndex];
                        id = data[idIndex];
                        filePath = string.Format("Master/{0}/{1}", folderName, fileName);                    

                        DataRow row = masterTable.Rows.Find(masterID);
                        if((masterID == "0") && (row != null)) // ��Ч��ѧϰʦ����ID
                        {
                            masterTable.Rows.Remove(row);
                            row = null;
                        }

                        if(row != null) // �ҵ���Ӧѧϰʦ�������ڵ���
                        {
                            if(filePath != row["TabFile"].ToString()) // �ļ�·���Ѿ����޸Ĺ�
                            {
                                sqlString = string.Format("UPDATE {0} SET TabFile = '{1}' WHERE ID = '{2}'", "tbl_master_list", filePath, masterID);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();
                                row["TabFile"] = filePath;
                            }

                            existMasterList.Add(masterID);
                        }
                        else // û���ҵ���Ӧѧϰʦ�������ڵ���
                        {
                            DataRow[] rows = masterTable.Select(string.Format("TabFile = '{0}'", filePath));
                            if(rows.Length > 0) // ��·����Ӧ��ѧϰʦ�Ѿ�����
                            {
                                string newMasterID = rows[0]["ID"].ToString();
                                sqlString = string.Format("UPDATE {0} SET MasterID = '{1}' WHERE id = '{2}'", "tbl_master_skills", newMasterID, id);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();
                            }
                            else
                            {
                                sqlString = string.Format("INSERT INTO {0} (ID, TabFile) VALUES ('{1}', '{2}')",
                                                                "tbl_master_list", maxID.ToString(), filePath);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();
                                newRow = masterTable.NewRow();
                                newRow["ID"] = maxID;
                                newRow["TabFile"] = filePath;
                                masterTable.Rows.Add(newRow);
                                existMasterList.Add(maxID.ToString());

                                if (masterID == "0") // ������ճ���������½��ļ��ܣ���������Ӧ��ѧϰʦ�����������ڣ���Ҫ���¼��ܱ�
                                {
                                    sqlString = string.Format("UPDATE {0} SET MasterID = '{1}' WHERE id = '{2}'",
                                        "tbl_master_skills", maxID.ToString(), id);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();
                                }

                                maxID++;
                            }         
                        }
                    }
                }

                // ����û�б����õ���ѧϰʦ����
                List<DataRow> deleteRowList = new List<DataRow>(); // ��Ҫɾ��������������
                foreach (DataRow r in masterTable.Rows)
                {
                    masterID = r["ID"].ToString();
                    if (!existMasterList.Contains(masterID))
                    {
                        sqlString = string.Format("DELETE FROM {0} WHERE ID = '{1}'", "tbl_master_list",
                            masterID);
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                        deleteRowList.Add(r);
                    }
                }

                foreach (DataRow r in deleteRowList)
                {
                    masterTable.Rows.Remove(r);
                }

                // ����ѧϰʦ�����ļ�
                dataList.Add("ID\tTabFile");
                foreach(DataRow r in masterTable.Rows)
                {
                    masterID = r["ID"].ToString();
                    filePath = r["TabFile"].ToString();
                    dataList.Add(string.Format("{0}\t{1}", masterID, filePath));
                }

                DirectoryInfo di = new DirectoryInfo(rootPath);
                string topFolder = di.Parent.FullName;
                SaveDataToFile(Path.Combine(topFolder, "Master.tab"), dataList, true);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڻ�ȡѧϰʦ��Ϣʱ����sql�쳣: " + ex.ToString());
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
        /// �����䷽�����ļ�
        /// </summary>
        /// <returns>�Ƿ񵼳��ɹ�</returns>
        public bool ExportRecipeList()
        {
            bool success = true; // �Ƿ񵼳��ɹ�
            List<string> dataList = new List<string>(); // �������ݵ�����
            string[] data; // ��������
            string fieldName; // ����
            int craftIDIndex = 0; // craftID����
            int recipeIDIndex = 0; // recipeID����
            int professionIDIndex = 0; // professionID����
            int professionLevelIndex = 0; // professionLevel����
            int branchIDIndex = 0; // branchID����
            int reputationIDIndex = 0; // reputationID����
            int nReputationLevelIndex = 0; // nReputationLevel����
            int nPriceIndex = 0; // nPrice����

            DataTable recipeTable; // �䷽���ݱ�

            dataList.Add("CraftID\tCraft\tRecipeID\tName\tBelong\t_Quality\tProfessionLevel\tBranchID\tReputationID\tnReputationLevel\tnPrice"); // �����

            for (int i = 0; i < fields.Count; i++)
            {
                fieldName = fields[i].ToString().Trim(new char[] { '\r', '\n' }); // ȥ�����ܴ��ڵĻ����ַ�

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
                    case "ProfessionID":
                        {
                            professionIDIndex = i;
                            break;
                        }
                    case "ProfessionLevel":
                        {
                            professionLevelIndex = i;
                            break;
                        }
                    case "BranchID":
                        {
                            branchIDIndex = i;
                            break;
                        }
                    case "ReputationID":
                        {
                            reputationIDIndex = i;
                            break;
                        }
                    case "nReputationLevel":
                        {
                            nReputationLevelIndex = i;
                            break;
                        }
                    case "nPrice":
                        {
                            nPriceIndex = i;
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
            }

            string newLine = "";

            foreach(string s in values)
            {
                if(s != "") // ���Կ���
                {
                    data = s.Split('\t');
                    string craftID = data[craftIDIndex];
                    string recipeID = data[recipeIDIndex];
                    string professionID = data[professionIDIndex];
                    string professionLevel = data[professionLevelIndex];
                    string branchID = data[branchIDIndex];
                    string reputationID = data[reputationIDIndex];
                    string nReputationLevel = data[nReputationLevelIndex];
                    string nPrice = data[nPriceIndex];

                    recipeTable = GetRecipeTable(recipeID, professionID);

                    newLine = string.Format("{0}\t", craftID);
                    newLine += string.Format("{0}\t", GetCraftName(craftID));
                    newLine += string.Format("{0}\t", recipeID);
                    if(recipeTable != null) // �ҵ���Ӧ���䷽
                    {
                        newLine += string.Format("{0}\t", recipeTable.Rows[0]["Name"].ToString());
                        newLine += string.Format("{0}\t", recipeTable.Rows[0]["Belong"].ToString());
                        newLine += string.Format("{0}\t", recipeTable.Rows[0]["_Quality"].ToString());
                    }
                    else // û���ҵ���Ӧ���䷽
                    {
                        newLine += "\t\t\t";
                    }
                    newLine += string.Format("{0}\t", professionLevel);
                    newLine += string.Format("{0}\t", branchID);
                    newLine += string.Format("{0}\t", reputationID);
                    newLine += string.Format("{0}\t", nReputationLevel);
                    newLine += string.Format("{0}\t", nPrice);
                    dataList.Add(newLine.TrimEnd(new char[] { '\t' }));
                }
            }

            DirectoryInfo di = new DirectoryInfo(rootPath);
            string topFolder = di.Parent.FullName;
            topFolder = Path.Combine(topFolder, "ForEditor");
            string fileName = Path.Combine(topFolder, "LearnRecipeList.tab");
            SaveDataToFile(fileName, dataList, true);

            return success;
        }

        /// <summary>
        /// ��ȡ���ܵ�����
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        private string GetCraftName(string id)
        {
            string craftName = "";

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT Name FROM {0} WHERE ID = '{1}'", "tbl_master_craft", id);
                DataTable craftTable = GetDataTable(sqlString);
                if(craftTable.Rows.Count > 0)
                {
                    craftName = craftTable.Rows[0][0].ToString();
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡѧϰʦ��������ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return craftName;
        }

        /// <summary>
        /// ����ѧϰʦ�ļ�
        /// </summary>
        /// <returns>�Ƿ񵼳��ɹ�</returns>
        public bool ExportMasterFiles()
        {
            Hashtable fileTable = new Hashtable();
            List<string> dataList;
            DataTable masterTable = null;
            string headLine = "CraftID\tRecipeID\tProfessionID\tProfessionLevel\tBranchID\tReputationID\tnReputationLevel\tnPrice";
            string newLine;
            string fileName;
            string folderName;
            string filePath;

            // ��ȡ���ݿ�����
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0} ORDER BY {1}", "tbl_master_skills", "ProfessionLevel");
                masterTable = GetDataTable(sqlString);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("��ȡѧϰʦ��Ϣʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            foreach(DataRow r in masterTable.Rows)
            {
                newLine = r["CraftID"].ToString() + "\t" + r["RecipeID"].ToString() + "\t" + r["ProfessionID"].ToString() + "\t" +
                    r["ProfessionLevel"].ToString() + "\t" + r["BranchID"].ToString() + "\t" + r["ReputationID"].ToString() + "\t" +
                    r["nReputationLevel"].ToString() + "\t" + r["nPrice"].ToString();
                fileName = r["filename"].ToString();
                folderName = r["foldername"].ToString();
                filePath = Path.Combine(folderName, fileName);

                if(fileTable[filePath] != null)
                {
                    dataList = fileTable[filePath] as List<string>;
                    dataList.Add(newLine);
                }
                else
                {
                    dataList = new List<string>();
                    dataList.Add(headLine);
                    dataList.Add(newLine);
                    fileTable[filePath] = dataList;
                }
            }

            List<string> fileList = new List<string>();
            foreach(object o in fileTable.Keys)
            {
                fileList.Add(o.ToString());
            }

            // ѡ�񵼳���·��
            string resultFolder = "";
            FolderBrowserDialog openFolderDialog1 = new FolderBrowserDialog();
            openFolderDialog1.Description = "��ѡ�񵼳��ļ��ı���Ŀ¼";
            openFolderDialog1.ShowNewFolderButton = true;
            if(rootPath == null)
            {
                openFolderDialog1.RootFolder = Environment.SpecialFolder.MyComputer;
            }
            else
            {
                openFolderDialog1.SelectedPath = rootPath;
            }

            if (openFolderDialog1.ShowDialog() != DialogResult.OK)
            {
                return false;
            }
            resultFolder = openFolderDialog1.SelectedPath;
            rootPath = resultFolder;

            // ѡ��Ҫ�������ļ�
            ChooseFileForm cForm = new ChooseFileForm("��ѡ��Ҫ�������ļ�", fileList);
            DialogResult result = cForm.ShowDialog();
            if(result != DialogResult.OK)
            {
                return false;
            }

            List<string> fileNameList = new List<string>();
            foreach(string s in fileList)
            {
                dataList = fileTable[s] as List<string>;
                filePath = Path.Combine(resultFolder, s);
                fileNameList.Add(filePath);
                SaveDataToFile(filePath, dataList, false);
            }

            // ��ʾ�����ļ��б�
            string message = "�����ļ�:\n\n";
            foreach (string s in fileNameList)
            {
                message = message + string.Format("{0}\n", s);
            }
            message = message + "\n\n";
            message = message + "�Ѿ��ɹ�����!";
            MessageBox.Show(message);

            return true;
        }

        /// <summary>
        /// ��ȡ�䷽����Ϣ���ݱ�
        /// </summary>
        /// <param name="recipeID">�䷽��ID</param>
        /// <param name="professionID">profession��ID</param>
        /// <returns>�䷽����Ϣ���ݱ�</returns>
        private DataTable GetRecipeTable(string recipeID, string professionID)
        {
            DataTable recipeTable = null;
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

                string sqlString = string.Format("SELECT * FROM {0} WHERE ID = '{1}' AND filename = '{2}'", "tbl_Recipe", recipeID, professionName);
                recipeTable = GetDataTable(sqlString);
                if(recipeTable.Rows.Count == 0) // û���ҵ���Ӧ��������
                {
                    recipeTable = null;
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

            return recipeTable;
        }

        /// <summary>
        /// ������д���ļ�
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="dataList">��������</param>
        /// <param name="showMessage">�Ƿ���ʾ��ʾ��Ϣ</param>
        private void SaveDataToFile(string fileName, List<string> dataList, bool showMessage)
        {
            try
            {
                string path = Path.GetDirectoryName(fileName);
                FileInfo fi = new FileInfo(fileName);
                StreamWriter sw;

                if (!fi.Exists) // �ļ�������
                {
                    if (!Directory.Exists(path)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(path); // �ȴ���Ŀ¼���ٴ����ļ�
                    }
                    FileStream fs = fi.Create();
                    fs.Close();
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }
                else // �ļ��Ѿ�����
                {
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }

                foreach (string s in dataList)
                {
                    sw.WriteLine(s);
                }
                sw.Close();

                if(showMessage)
                {
                    MessageBox.Show(string.Format("�ļ� {0} �Ѿ��ɹ�����!", fileName));
                }            
            }
            catch (IOException ex)
            {
                MessageBox.Show("�ڵ��������ļ�ʱ����IO�쳣: " + ex.ToString());
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
