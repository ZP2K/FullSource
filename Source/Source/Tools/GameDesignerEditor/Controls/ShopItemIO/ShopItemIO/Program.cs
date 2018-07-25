using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;
using System.Data.SqlClient;
using System.Collections;
using System.Data;
using System.Text;

namespace ShopItemIO
{
    public class Program
    {
        private string path; // �ļ�·��
        private string rootFolder; // �̵���Ŀ¼
        private SqlConnection conn; // sql����
        private string[] fields; // ��������
        private string[] values; // ����������
        private List<string> valuesList = new List<string>(); // ����������
        private bool loadedFields = false; // �Ƿ��Ѿ���ӹ��������� 
        private DataTable shopListTable; // �̵�������
        private DataTable shopItemTable; // �̵���߱�

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="path">��·��</param>
        public Program(SqlConnection conn, string path)
        {
            this.conn = conn;
            this.path = path;
            Init();
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
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            if (conn.State == ConnectionState.Closed)
            {
                conn.Open();
            }

            string sqlString = "SELECT * FROM tbl_npc_shoplist";
            shopListTable = GetDataTable(sqlString);

            sqlString = "SELECT * FROM tbl_npc_shopitem";
            shopItemTable = GetDataTable(sqlString);

            if (conn.State == ConnectionState.Open)
            {
                conn.Close();
            }
        }

        /// <summary>
        /// ˢ���̵��������
        /// </summary>
        /// <param name="baseTree">������</param>
        public void RefreshShopItemName(TreeView baseTree)
        {
            if (conn.State == ConnectionState.Closed)
            {
                conn.Open();
            }

            int refreshCount = 0;
            SqlCommand cmd = conn.CreateCommand();

            foreach (DataRow dataRow in shopItemTable.Rows)
            {
                string id = dataRow["id"].ToString();
                string folderName = dataRow["foldername"].ToString();
                string fileName = dataRow["filename"].ToString();
                string itemName = dataRow["Name"].ToString();
                string tabType = dataRow["TabType"].ToString();
                string itemIndex = dataRow["ItemIndex"].ToString();

                if (tabType != "" && itemIndex != "")
                {
                    Hashtable itemInfo = GetItemInfo(cmd, tabType, itemIndex);
                    string newName = itemInfo["Name"] as string;

                    if (!string.IsNullOrEmpty(newName) && itemName != newName)
                    {
                        string sqlString = string.Format("UPDATE tbl_npc_shopitem SET Name = '{0}' WHERE TabType = '{1}' AND ItemIndex = '{2}'",
                                                         newName, tabType, itemIndex);
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();

                        RefreshNodeText(baseTree, folderName, fileName, id, newName);

                        refreshCount++;
                    }
                }                                
            }

            if (conn.State == ConnectionState.Open)
            {
                conn.Close();
            }

            MessageBox.Show(string.Format("ˢ�³ɹ�������{0}���̵���ߵ����Ʊ�ˢ�£�", refreshCount), "ˢ���̵��������",
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        /// <summary>
        /// ˢ���������ı�
        /// </summary>
        /// <param name="tree">������</param>
        /// <param name="folderName">�ļ�����</param>
        /// <param name="fileName">�ļ���</param>
        /// <param name="id">����ID</param>
        /// <param name="newName">������</param>
        private void RefreshNodeText(TreeView tree, string folderName, string fileName, string id, string newName)
        {
            foreach (TreeNode folderNode in tree.Nodes)
            {
                if (folderNode.Text == folderName)
                {
                    foreach (TreeNode fileNode in folderNode.Nodes)
                    {
                        if (fileNode.Text == fileName)
                        {
                            foreach (TreeNode itemNode in fileNode.Nodes)
                            {
                                if (((object[])itemNode.Tag)[0].ToString() == id)
                                {
                                    itemNode.Text = newName;
                                    break;
                                }
                            }

                            break;
                        }
                    }

                    break;
                }
            }
        }

        /// <summary>
        /// �����̵���Ʒ��Ϣ
        /// </summary>
        public bool LoadShopItemFromPath()
        {
            bool loadSucess = true; // �Ƿ���ɹ�
            rootFolder = GetLoadPath(); // ��Ŀ¼

            if (rootFolder == null) // �û�δѡ���̵���߱����ڵ�Ŀ¼
            {
                MessageBox.Show("û��ѡ���̵���߱����ڵ�Ŀ¼!");
                return false;
            }

            List<string> fileNameList = new List<string>();
            DirectoryInfo shopDirectoryInfo = new DirectoryInfo(path);
            
            foreach (DirectoryInfo di in shopDirectoryInfo.GetDirectories())
            {
                if (!di.Attributes.ToString().Contains("Hidden")) // ����ʾ�����ļ���
                {
                    string folderName = di.Name;

                    foreach (FileInfo fi in di.GetFiles())
                    {
                        if (!fi.Attributes.ToString().Contains("Hidden")) // ����ʾ�����ļ�
                        {
                            string fileName = fi.Name;

                            fileNameList.Add(string.Format("{0}\\{1}", folderName, fileName));
                        }                        
                    }
                }                
            }

            ChooseFileForm cForm = new ChooseFileForm("��ѡ��Ҫ������̵��ļ�", fileNameList);

            if (cForm.ShowDialog() == DialogResult.OK)
            {
                SqlTransaction transaction = null;
                ProgressForm pForm = new ProgressForm(0, fileNameList.Count);
                pForm.Show();

                try
                {
                    if (conn.State == ConnectionState.Closed)
                    {
                        conn.Open();
                    }

                    transaction = conn.BeginTransaction();
                    SqlCommand cmd = conn.CreateCommand();
                    cmd.Transaction = transaction;

                    int index = 1;

                    foreach (string s in fileNameList)
                    {
                        string[] data = s.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
                        string folderName = data[0];
                        string fileName = data[1];

                        pForm.ShowProgress(index, string.Format("�����ļ�{0}������...", s));
                        UpdateShopItem(cmd, fileName, folderName);
                        index++;
                    }

                    transaction.Commit();
                }
                catch (SqlException ex)
                {
                	MessageBox.Show("�ڸ����̵������Ϣʱ����sql�쳣��" + ex.Message, "�����̵������Ϣ", MessageBoxButtons.OK,
                                    MessageBoxIcon.Information);

                    if (transaction != null)
                    {
                        transaction.Rollback();
                    }

                    loadSucess = false;
                }
                finally
                {
                    if (conn.State == ConnectionState.Open)
                    {
                        conn.Close();
                    }
                }
            }
            else
            {
                loadSucess = false;
            }           

            if (loadSucess)
            {
                MessageBox.Show("�̵������Ϣ���³ɹ���\r\n\r\n��ǰ��������Ѿ���ɣ������Ļ�����bugŶ~", "�����̵������Ϣ",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("�̵������Ϣ����ʧ�ܣ�\r\n\r\n��ǰ��������Ѿ���ɣ������Ļ�����bugŶ~", "�����̵������Ϣ",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return loadSucess;
        }

        /// <summary>
        /// ��ȡ�̵�ID
        /// </summary>
        /// <param name="folderName">�ļ�����</param>
        /// <param name="fileName">�ļ���</param>
        /// <returns>�̵�ID</returns>
        private string GetShopTemplateID(string folderName, string fileName)
        {
            string shopID = "0";
            string configFile = string.Format("{0}\\{1}", folderName, fileName);

            DataRow[] rows = shopListTable.Select(string.Format("ConfigFile = '{0}'", configFile));

            if (rows.Length > 0)
            {
                shopID = rows[0]["ShopTemplateID"].ToString();
            }

            return shopID;
        }

        /// <summary>
        /// ��ȡ�̵���߱����ڵ�Ŀ¼
        /// </summary>
        /// <returns>�̵���߱����ڵ�Ŀ¼</returns>
        private string GetLoadPath()
        {
            string resultFolder = null;
            FolderBrowserDialog openFolderDialog1 = new FolderBrowserDialog();
            openFolderDialog1.Description = "��ѡ���̵���߱����ڵ�Ŀ¼";
            openFolderDialog1.SelectedPath = path;

            if(openFolderDialog1.ShowDialog() == DialogResult.OK)
            {
                resultFolder = openFolderDialog1.SelectedPath;
            }

            return resultFolder;
        }      

        /// <summary>
        /// ��ȡ������Ϣ
        /// </summary>
        /// <param name="cmd">sql����</param>
        /// <param name="tabType">���߱���</param>
        /// <param name="itemIndex">���߱��</param>
        /// <returns>������Ϣ</returns>
        private Hashtable GetItemInfo(SqlCommand cmd, string tabType, string itemIndex)
        {
            Hashtable infoTable = new Hashtable();
            List<string> fieldList = new List<string>();
            string tableName = "Other";

            if (tabType != "" && itemIndex != "")
            {
                switch (tabType)
                {
                    case "Other":
                        {
                            tableName = "Other";
                            break;
                        }
                    case "CustomWeapon":
                        {
                            tableName = "item_Custom_Weapon";
                            break;
                        }
                    case "CustomArmor":
                        {
                            tableName = "item_Custom_Armor";
                            break;
                        }
                    case "CustomTrinket":
                        {
                            tableName = "Custom_Trinket";
                            break;
                        }
                }

                if (tableName == "Other")
                {
                    fieldList.Add("Name");
                    fieldList.Add("Genre");
                    fieldList.Add("SubType");
                    fieldList.Add("Quality");
                }
                else
                {
                    fieldList.Add("Name");
                    fieldList.Add("Genre");
                    fieldList.Add("SubType");
                    fieldList.Add("Level");
                    fieldList.Add("Quality");
                }                

                foreach (string s in fieldList)
                {
                    string sqlString = string.Format("SELECT {0} FROM {1} WHERE ID = '{2}'", s, tableName, itemIndex);
                    cmd.CommandText = sqlString;
                    object executeResult = cmd.ExecuteScalar();
                    
                    if (executeResult != null)
                    {
                        infoTable[s] = executeResult.ToString();
                    }
                }
            }

            return infoTable;
        }

        /// <summary>
        /// ��ȡ�ļ��ı�����
        /// </summary>
        /// <param name="filePath">�ļ�·��</param>
        /// <returns>�ı�����</returns>
        private string GetFileContent(string filePath)
        {
            string content = null;

            if (File.Exists(filePath))
            {
                StreamReader reader = new StreamReader(filePath, Encoding.GetEncoding("gb2312"));
                content = reader.ReadToEnd();
                reader.Close();
            }
            else
            {
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", filePath), "��ȡ�����ļ�",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return content;
        }

        /// <summary>
        /// �������ݱ�
        /// </summary>
        /// <param name="content">����</param>
        /// <returns>���ݱ�</returns>
        private DataTable CreateDataTable(string content)
        {
            DataTable table = new DataTable();
            List<string> fieldList = new List<string>();

            bool firstLine = true;
            string[] lines = content.Split(new char[] { '\n' });

            foreach (string s in lines)
            {
                string line = s.TrimEnd(new char[] { '\r' });

                if (line != "") // �����յ�������
                {
                    string[] data = line.Split(new char[] { '\t' });

                    if (firstLine)
                    {
                        firstLine = false;

                        for (int i = 0; i < data.Length; i++)
                        {
                            DataColumn dataColumn = new DataColumn(data[i]);
                            table.Columns.Add(dataColumn);
                            fieldList.Add(data[i]);
                        }
                    }
                    else
                    {
                        DataRow dataRow = table.NewRow();

                        for (int i = 0; i < data.Length; i++)
                        {
                            dataRow[i] = data[i];
                        }

                        table.Rows.Add(dataRow);
                    }
                }

            }

            return table;
        }

        /// <summary>
        /// �����̵������Ϣ
        /// </summary>
        /// <param name="cmd">sql����</param>
        /// <param name="fileName">�ļ���</param>
        /// <param name="folderName">�ļ�����</param>        
        private void UpdateShopItem(SqlCommand cmd, string fileName, string folderName)
        {
            string filePath = Path.Combine(rootFolder, string.Format("{0}\\{1}", folderName, fileName));            
            string content = GetFileContent(filePath);
            DataTable itemTable = CreateDataTable(content);

            for (int i = 0; i < itemTable.Rows.Count; i++)
            {
                if (i >= 100)
                {
                    MessageBox.Show(string.Format("�̵�{0}�ĵ����Ѿ���������100��\r\n\r\n�������޵ĵ��߽�������¡�", filePath), "���µ�����Ϣ",
                                                  MessageBoxButtons.OK, MessageBoxIcon.Information);
                    break;
                }

                DataRow dataRow = itemTable.Rows[i];
                string tabType = dataRow["TabType"].ToString();
                string itemIndex = dataRow["ItemIndex"].ToString();
                string durability = dataRow["Durability"].ToString();
                string shopTemplateID = GetShopTemplateID(folderName, fileName);
                string sqlString = null;
                DataRow itemRow = GetShopItemRow(tabType, itemIndex, durability, folderName, fileName);

                if (itemRow == null) // ��Ҫ�������
                {
                    sqlString = string.Format("INSERT INTO tbl_npc_shopitem (TabType, ItemIndex, Durability, foldername, filename, ShopTemplateID) VALUES ('{0}', '{1}', '{2}', '{3}', '{4}', '{5}')",
                                              tabType, itemIndex, durability, folderName, fileName, shopTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    Hashtable itemInfo = GetItemInfo(cmd, tabType, itemIndex);

                    foreach (string key in itemInfo.Keys)
                    {
                        string itemValue = itemInfo[key] as string;

                        if (!string.IsNullOrEmpty(itemValue))
                        {
                            sqlString = string.Format("UPDATE tbl_npc_shopitem SET [{0}] = '{1}' WHERE TabType = '{2}' AND ItemIndex = '{3}' AND Durability = '{4}'",
                                                      key, itemValue, tabType, itemIndex, durability);
                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();
                        }
                    }
                }

                foreach (DataColumn dataColumn in itemTable.Columns)
                {
                    string columnName = dataColumn.ColumnName;
                    string newValue = dataRow[columnName].ToString();

                    if (columnName != "TabType" && columnName != "ItemIndex" && columnName != "Durability" && columnName != "foldername" && columnName != "filename")
                    {
                        if (itemRow == null)
                        {
                            if (newValue == "")
                            {
                                sqlString = string.Format("UPDATE tbl_npc_shopitem SET [{0}] = NULL WHERE TabType = '{1}' AND ItemIndex = '{2}' AND Durability = '{3}' AND foldername = '{4}' AND filename = '{5}'",
                                                          columnName, tabType, itemIndex, durability, folderName, fileName);
                            }
                            else
                            {
                                sqlString = string.Format("UPDATE tbl_npc_shopitem SET [{0}] = '{1}' WHERE TabType = '{2}' AND ItemIndex = '{3}' AND Durability = '{4}' AND foldername = '{5}' AND filename = '{6}'",
                                                          columnName, newValue, tabType, itemIndex, durability, folderName, fileName);
                            }

                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();
                        }
                        else
                        {
                            string oldValue = itemRow[columnName].ToString();

                            if (oldValue != newValue)
                            {
                                if (newValue == "")
                                {
                                    sqlString = string.Format("UPDATE tbl_npc_shopitem SET [{0}] = NULL WHERE TabType = '{1}' AND ItemIndex = '{2}' AND Durability = '{3}' AND foldername = '{4}' AND filename = '{5}'",
                                                              columnName, tabType, itemIndex, durability, folderName, fileName);
                                }
                                else
                                {
                                    sqlString = string.Format("UPDATE tbl_npc_shopitem SET [{0}] = '{1}' WHERE TabType = '{2}' AND ItemIndex = '{3}' AND Durability = '{4}' AND foldername = '{5}' AND filename = '{6}'",
                                                              columnName, newValue, tabType, itemIndex, durability, folderName, fileName);
                                }

                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// ��ȡ�̵����������
        /// </summary>
        /// <param name="tabType">���߱���</param>
        /// <param name="itemIndex">����ID</param>
        /// <param name="durability">�����;ö�</param>
        /// <param name="folderName">�ļ�����</param>
        /// <param name="fileName">�ļ���</param>
        /// <returns>�̵����������</returns>
        private DataRow GetShopItemRow(string tabType, string itemIndex, string durability, string folderName, string fileName)
        {
            DataRow dataRow = null;

            DataRow[] rows = shopItemTable.Select(string.Format("TabType = '{0}' AND ItemIndex = '{1}' AND Durability = '{2}' AND foldername = '{3}' AND filename = '{4}'",
                                                                tabType, itemIndex, durability, folderName, fileName));
            if (rows.Length > 0)
            {
                dataRow = rows[0];
            }

            return dataRow;
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