using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Collections;
using System.Data;
using System.Windows.Forms;
using System.IO;

namespace ShopItemIO
{
    class ExportShopList
    {
        private SqlConnection conn; // sql����
        private ArrayList fields; // ����������
        private string[] values; // ����������
        private string path; // ����·��
        private List<string> excludeColumnNameList = new List<string>(); // ������������������
        private List<int> excludeColumnIndexList = new List<int>(); // �����������������
        private Hashtable itemTable = new Hashtable(); // ������Ʒ���ݵ�hash��
        private string fileName; // �����ļ����ļ���

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="fields">����������</param>
        /// <param name="values">����������</param>
        /// <param name="path">����·��</param>
        public ExportShopList(SqlConnection conn, ArrayList fields, string[] values, string path)
        {
            this.conn = conn;
            this.fields = fields;
            this.values = values;
            this.path = path;
        }

        /// <summary>
        /// ���岻��������
        /// </summary>
        /// <param name="columnName">�������е�����</param>
        public void AddExcludeColumn(string columnName)
        {
            excludeColumnNameList.Add(columnName);
        }

        /// <summary>
        /// ���嵼���ļ����ļ���
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        public void SetFileName(string fileName)
        {
            this.fileName =fileName;
        }

        /// <summary>
        /// ����̵���ߵ��̵�IDһ����
        /// </summary>
        private bool CheckCoordinate()
        {
            bool coordinate = true; // �Ƿ�һ��
            Hashtable pathTable = new Hashtable(); // �����̵�·���Ĺ�ϣ��
            List<string> errorList = new List<string>(); // ������Ϣ��ʾ������
            List<string> pathList = new List<string>(); // ����·��������

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string folderName;
                string fileName;
                string shopTemplateID;
                string path;

                string sqlString = string.Format("SELECT foldername, filename, ShopTemplateID FROM {0}", "tbl_npc_shopitem");
                DataTable itemTable = GetDataTable(sqlString);
                foreach(DataRow r in itemTable.Rows)
                {
                    folderName = r["foldername"].ToString();
                    fileName = r["filename"].ToString();
                    shopTemplateID = r["ShopTemplateID"].ToString();

                    path = folderName + "\\" + fileName;
                    if(pathTable[path] == null)
                    {
                        pathTable[path] = shopTemplateID;
                    }
                    else
                    {
                        if(pathTable[path].ToString() != shopTemplateID) // ͬ·�����в�ͬ���̵�ID
                        {
                            if(pathTable[path].ToString() == "0") // �̵�IDΪ0ʱ�Զ�����
                            {
                                pathTable[path] = shopTemplateID;
                            }
                            else
                            {
                                if(shopTemplateID != "0")
                                {
                                    if(!pathList.Contains(path))
                                    {
                                        pathList.Add(path);
                                        errorList.Add(string.Format("·��{0}���г�ͻ���̵�ID!", path));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڼ���̵���ߵ��̵�IDһ����ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            coordinate = (errorList.Count == 0);
            if(!coordinate) // �̵���ߵ��̵�ID���ֲ�һ�µ����
            {
                string message = "";
                foreach(string s in errorList)
                {
                    message = message + s + "\n";
                }
                MessageBox.Show(message, "�̵����ShopTemplateIDһ���Լ��", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return coordinate;
        }

        /// <summary>
        /// �����̵������ļ�
        /// </summary>
        public void ExportNpcShopList()
        {
            if(!CheckCoordinate()) // û��ͨ���̵����ShopTemplateIDһ���Լ��
            {
                return;
            }

            List<string> outputList = new List<string>(); // ��������������
            List<string> existShopList = new List<string>(); // �Ѿ����ڵ��̵�����
            int folderNameIndex = 0; // �̵�����ļ��е����
            int fileNameIndex = 0; // �̵�����ļ������
            int shopTemplateIDIndex = 0; // �̵�ID����� 
            int idIndex = 0; // ����ID�����
            string fieldName; // �ֶ���
            string[] data; // ������

            for (int i = 0; i < fields.Count; i++)
            {
                fieldName = fields[i].ToString().TrimEnd(new char[] {'\r', '\n'});
                switch(fieldName)
                {
                    case "ShopTemplateID":
                        {
                            shopTemplateIDIndex = i;
                            break;
                        }
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

                SqlCommand cmd = conn.CreateCommand();
                string sqlString = string.Format("SELECT * FROM {0} ORDER BY {1}", "tbl_npc_shoplist", "ShopTemplateID");
                DataTable shopListTable = GetDataTable(sqlString);
                DataRow defaultRow = null; // Ĭ����
                int maxShopIndex = 1; // ����̵�ID
                if(shopListTable.Rows.Count > 0)
                {
                    maxShopIndex = int.Parse(shopListTable.Rows[shopListTable.Rows.Count - 1]["ShopTemplateID"].ToString()) + 1;
                    defaultRow = shopListTable.Rows[0];
                }

                foreach(string s in values)
                {
                    if(s != "") // �������Ŀ���
                    {
                        data = s.Split('\t');
                        string fileName = data[fileNameIndex];
                        string folderName = data[folderNameIndex];
                        string path = Path.Combine(folderName, fileName);
                        string shopTemplateID = data[shopTemplateIDIndex];
                        string id = data[idIndex];

                        DataRow row = shopListTable.Rows.Find(shopTemplateID);
                        if((shopTemplateID == "0") && (row != null)) // shopTemplateIDΪ0���зǷ�
                        {
                            shopListTable.Rows.Remove(row);
                            row = null; // ������������
                        }

                        if(row == null) // û���ҵ���Ӧ��������
                        {
                            DataRow[] rows = shopListTable.Select(string.Format("ConfigFile = '{0}'", path));
                            if(rows.Length > 0) // ��·����Ӧ���̵��Ѿ�����
                            {
                                string shopID = rows[0]["ShopTemplateID"].ToString();
                                sqlString = string.Format("UPDATE {0} SET ShopTemplateID = '{1}' WHERE id = '{2}'", "tbl_npc_shopitem", shopID, id);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();
                            }
                            else
                            {
                                if (defaultRow != null) // ����Ĭ����
                                {
                                    string canRepair = "0"; // ������ֵת��Ϊ0��1
                                    if((bool)defaultRow["CanRepair"])
                                    {
                                        canRepair = "1";
                                    }

                                    sqlString = string.Format("INSERT INTO {0} (ShopTemplateID, ConfigFile, ShopName, CanRepair) VALUES ('{1}', '{2}', '{3}', '{4}')",
                                    "tbl_npc_shoplist", maxShopIndex.ToString(), path, defaultRow["ShopName"], canRepair);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();

                                    DataRow newRow = shopListTable.NewRow();
                                    newRow["ShopTemplateID"] = maxShopIndex.ToString();
                                    newRow["ConfigFile"] = path;
                                    newRow["ShopName"] = defaultRow["ShopName"];
                                    newRow["CanRepair"] = defaultRow["CanRepair"];
                                    shopListTable.Rows.Add(newRow);
                                    existShopList.Add(maxShopIndex.ToString()); // ��¼��ǰ���ڵ��̵�ID

                                }
                                else // ������Ĭ����
                                {
                                    sqlString = string.Format("INSERT INTO {0} (ShopTemplateID, ConfigFile) VALUES ('{1}', '{2}')",
                                    "tbl_npc_shoplist", maxShopIndex.ToString(), path);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();

                                    DataRow newRow = shopListTable.NewRow();
                                    newRow["ShopTemplateID"] = maxShopIndex.ToString();
                                    newRow["ConfigFile"] = path;
                                    shopListTable.Rows.Add(newRow);
                                    existShopList.Add(maxShopIndex.ToString()); // ��¼��ǰ���ڵ��̵�ID
                                }

                                if(shopTemplateID == "0") // ������ճ���������½��ĵ��ߣ���������Ӧ���̵껹�����ڣ���Ҫ���µ��߱�
                                {
                                    sqlString = string.Format("UPDATE {0} SET ShopTemplateID = '{1}' WHERE id = '{2}'",
                                        "tbl_npc_shopitem", maxShopIndex.ToString(), id);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();
                                }

                                maxShopIndex++;
                            }
                        }
                        else
                        {
                            if((row["ConfigFile"].ToString() != path)) // ��Ҫ���¶�Ӧ�̵����Ϣ
                            {
                                sqlString = string.Format("UPDATE {0} SET ConfigFile = '{1}' WHERE ShopTemplateID = '{2}'",
                                    "tbl_npc_shoplist", path, shopTemplateID);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();

                                row["ConfigFile"] = path;
                            }
                            existShopList.Add(shopTemplateID); // ��¼��ǰ���ڵ��̵�ID
                        }
                    }
                }

                // ����û�б����õ����̵�����
                List<DataRow> deleteRowList = new List<DataRow>(); // ��Ҫɾ��������������
                foreach(DataRow r in shopListTable.Rows)
                {
                    string shopTemplateID = r["ShopTemplateID"].ToString();
                    if(!existShopList.Contains(shopTemplateID))
                    {
                        sqlString = string.Format("DELETE FROM {0} WHERE ShopTemplateID = '{1}'", "tbl_npc_shoplist",
                            shopTemplateID);
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                        deleteRowList.Add(r);
                    }
                }

                foreach(DataRow r in deleteRowList)
                {
                    shopListTable.Rows.Remove(r);
                }

                outputList.Add("ShopTemplateID\tShopName\tConfigFile\tCanRepair");
                foreach(DataRow r in shopListTable.Rows)
                {
                    object[] itemArray = r.ItemArray;
                    string item;
                    string value = "";
                    foreach(object o in itemArray)
                    {
                        item = o.ToString();
                        switch(item)
                        {
                            case "True":
                                {
                                    item = "1";
                                    break;
                                }
                            case "False":
                                {
                                    item = "0";
                                    break;
                                }
                        }
                        value += string.Format("{0}\t", item);
                    }
                    outputList.Add(value.TrimEnd('\t'));
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڸ����̵�����ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            SaveDataToFile(outputList);
        }

        /// <summary>
        /// �����̵������Ϣ
        /// </summary>
        /// <param name="itemID">�¼�������</param>
        /// <param name="itemType">�¼�����</param>
        /// <param name="infoList">��Ϣ����</param>
        private void UpdateShopItemInfo(string itemID, string itemType, List<string> infoList)
        {
            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                SqlCommand cmd = conn.CreateCommand();
                string sqlString = string.Format("UPDATE tbl_npc_shopitem SET Name = '{0}', Genre = '{1}', SubType = '{2}', Level = '{3}', Quality = '{4}' WHERE ItemIndex = '{5}' AND TabType = '{6}'",
                                                 infoList[0], infoList[1], infoList[2], infoList[3], infoList[4], itemID, itemType);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڸ����̵������Ϣʱ����sql�쳣��" + ex.Message, "�����̵������Ϣ",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }            
        }

        /// <summary>
        /// �����̵�����б��ļ�
        /// </summary>
        public void ExportSupplyList()
        {
            List<string> dataList = new List<string>(); // �������ݵ�����

            // ���������
            string headLine = string.Empty; // ��������
            string fieldName; // ����
            int countIndex = 0; // ���ü����е����
            int itemIDIndex = 0; // ��ƷID�е����
            int itemTypeIndex = 0; // ��Ʒ�����е����
            int nameIndex = 0; // Name�����
            int genreIndex = 0; // Genre�����
            int subTypeIndex = 0; // SubType�����
            int levelIndex = 0; // Level�����
            int qualityIndex = 0; // Quality�����
            int durabilityIndex = 0; // Qurability�����

            for (int i = 0, j = 0; i < fields.Count; i++)
            {
                fieldName = fields[i].ToString().Trim(new char[] {'\r', '\n'}); // ȥ�����ܴ��ڵĻ����ַ�
                if (!excludeColumnNameList.Contains(fieldName)) // ����û�б�����Ϊ������
                {
                    switch(fieldName)
                    {
                        case "UsedCount":
                            {
                                countIndex = j;
                                break;
                            }
                        case "ItemIndex":
                            {
                                itemIDIndex = j;
                                break;
                            }
                        case "TabType":
                            {
                                itemTypeIndex = j;
                                break;
                            }
                        case "Name":
                            {
                                nameIndex = j;
                                break;
                            }
                        case "Genre":
                            {
                                genreIndex = j;
                                break;
                            }
                        case "SubType":
                            {
                                subTypeIndex = j;
                                break;
                            }
                        case "Level":
                            {
                                levelIndex = j;
                                break;
                            }
                        case "Quality":
                            {
                                qualityIndex = j;
                                break;
                            }
                        case "Durability":
                            {
                                durabilityIndex = j;
                                break;
                            }
                    }
                 
                    headLine += fieldName + "\t";
                    j++;
                }
                else
                {
                    excludeColumnIndexList.Add(i);
                }
            }

            dataList.Add(headLine.TrimEnd('\t'));

            // ���������
            string key;
            string[] data;
            string[] newData;
            List<string> itemInfo;
            ItemData itemData;

            foreach(string s in values)
            {
                if(s != "")
                {
                    data = s.Split(new char[] { '\t' });
                    newData = new string[data.Length - excludeColumnIndexList.Count];
                    for (int i = 0, j = 0; i < data.Length; i++)
                    {
                        if (!excludeColumnIndexList.Contains(i)) // ����û�б�����Ϊ������
                        {
                            newData[j] = data[i];
                            j++;
                        }
                    }
                    key = newData[itemIDIndex] + "," + newData[itemTypeIndex] + "," + newData[durabilityIndex];
                    itemInfo = GetItemInfo(newData[itemIDIndex], newData[itemTypeIndex]);
                    newData[nameIndex] = itemInfo[0];
                    newData[genreIndex] = itemInfo[1];
                    newData[subTypeIndex] = itemInfo[2];
                    newData[levelIndex] = itemInfo[3];
                    newData[qualityIndex] = itemInfo[4];

                    // �����̵������Ϣ
                    UpdateShopItemInfo(newData[itemIDIndex], newData[itemTypeIndex], itemInfo);

                    if (itemTable[key] != null)
                    {
                        itemData = itemTable[key] as ItemData;
                        itemData.AddItem();
                    }
                    else
                    {
                        itemData = new ItemData(newData, countIndex);
                        itemTable[key] = itemData;
                    }
                }
            }

            foreach(string s in itemTable.Keys)
            {
                itemData = itemTable[s] as ItemData;
                dataList.Add(itemData.GetData());
            }

            SaveDataToFile(dataList);
        }

        /// <summary>
        /// ��ȡ������Ϣ����
        /// </summary>
        /// <param name="id">����id</param>
        /// <param name="type">��������</param>
        /// <returns>������Ϣ����</returns>
        private List<string> GetItemInfo(string id, string type)
        {
            List<string> infoList = new List<string>();
            string tableName = "Other";

            if (id == "" || type == "")
            {
                return null;
            }

            switch (type)
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

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0} WHERE ID = '{1}'", tableName, id);
                DataTable table = GetDataTable(sqlString);

                if (table.Rows.Count > 0)
                {
                    DataRow row = table.Rows[0];
                    if (tableName == "Other") // Other����û��Level�ֶ�
                    {
                        infoList.Add(row["Name"].ToString());
                        infoList.Add(row["Genre"].ToString());
                        infoList.Add(row["SubType"].ToString());
                        infoList.Add("");
                        infoList.Add(row["Quality"].ToString());
                    }
                    else
                    {
                        infoList.Add(row["Name"].ToString());
                        infoList.Add(row["Genre"].ToString());
                        infoList.Add(row["SubType"].ToString());
                        infoList.Add(row["Level"].ToString());
                        infoList.Add(row["Quality"].ToString());
                    }
                }
                else
                {
                    for (int i = 0; i < 5; i++)
                    {
                        infoList.Add("");
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ��Ʒ��Ϣʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return infoList;
        }

        /// <summary>
        /// ������д���ļ�
        /// </summary>
        private void SaveDataToFile(List<string> dataList)
        {
            try
            {
                string filePath = Path.Combine(path, fileName);
                FileInfo fi = new FileInfo(filePath);
                StreamWriter sw;

                if (!fi.Exists) // �ļ�������
                {
                    if(!Directory.Exists(path)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(path); // �ȴ���Ŀ¼���ٴ����ļ�
                    }
                    FileStream fs = fi.Create();
                    fs.Close();
                    sw = new StreamWriter(filePath, false, Encoding.GetEncoding("gb2312"));
                }
                else // �ļ��Ѿ�����
                {
                    sw = new StreamWriter(filePath, false, Encoding.GetEncoding("gb2312"));
                }

                foreach(string s in dataList)
                {
                    sw.WriteLine(s);
                }
                sw.Close();
                MessageBox.Show(string.Format("�ļ� {0} �Ѿ��ɹ�����!", filePath));
            }
            catch(IOException ex)
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

        /// <summary>
        /// ������Ʒ���ݵ��ڲ���
        /// </summary>
        private class ItemData
        {
            private string[] data; // ��Ʒ��������
            private int count = 1; // ���ü���
            private int countIndex; // ���ü����е����

            /// <summary>
            /// ���캯��
            /// </summary>
            /// <param name="data">��Ʒ��������</param>
            /// <param name="countIndex">���ü����е����</param>
            public ItemData(string[] data, int countIndex)
            {
                this.data = data;
                this.countIndex = countIndex;
            }
            
            /// <summary>
            /// ���ü�����һ
            /// </summary>
            public void AddItem()
            {
                count++;
            }

            /// <summary>
            /// ��ȡ��Ʒ������
            /// </summary>
            /// <returns>��Ʒ������</returns>
            public string GetData()
            {                
                StringBuilder value = new StringBuilder();

                for(int i = 0; i < data.Length; i++)
                {
                    if (i == countIndex)
                    {
                        value.Append(string.Format("{0}\t", count.ToString()));                        
                    }
                    else
                    {
                        value.Append(string.Format("{0}\t", data[i]));
                    }     
                }

                value.Remove(value.Length - 1, 1);

                return value.ToString();
            }
        }
    }
}
