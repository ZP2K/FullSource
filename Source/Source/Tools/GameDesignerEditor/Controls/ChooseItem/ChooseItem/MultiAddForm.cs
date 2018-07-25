using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace ChooseItem
{
    public partial class MultiAddForm : Form
    {
        private TreeNode node; // �����
        private SqlConnection conn; // sql����
        private List<string> existAttributes = new List<string>(); // �Ѿ����ڵ�����
        private List<string> totalAttributes = new List<string>(); // �ܵ�����
        private List<string> addedAttributes = new List<string>(); // �Ѿ���ӵ�����
        private string fileName; // ini�ļ�����
        private DataTable dt; // ���ݱ�
        private TreeNode findNode; // �ҵ��������
        private Hashtable table = new Hashtable(); // �����������Ϣ��hash��
        private Hashtable detailTable = new Hashtable(); // detail�������Ϣ��hash��
        private string itemTable; // ��Ʒ����
        private string dropTable; // �����ʱ��� 
        private int catLength = 0; // ���೤��
        private bool autoResetItemName; // �Ƿ��Զ������������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="node">��ǰ�����</param>
        /// <param name="itemTable">��Ʒ����</param>
        /// <param name="dropTable">�����ʱ���</param>
        /// <param name="autoResetItemName">�Ƿ��Զ������������</param>
        public MultiAddForm(SqlConnection conn, TreeNode node, string itemTable, string dropTable, bool autoResetItemName)
        {
            this.conn = conn;
            while(node != null && node.Level != 0)
            {
                node = node.Parent;
            }
            this.node = node;
            this.autoResetItemName = autoResetItemName;
            this.itemTable = itemTable;
            this.dropTable = dropTable;
            fileName = node.Text;
            InitializeComponent();
            FillDetailTable();
            FillTotalAttributes();
            FillExistAttributes();
            FillLists();
        }

        /// <summary>
        /// ���detail�������Ϣ��hash��
        /// </summary>
        private void FillDetailTable()
        {
            detailTable["1"] = "��������";
            detailTable["2"] = "Զ������";
            detailTable["3"] = "�·�";
            detailTable["4"] = "��ָ";
            detailTable["5"] = "����";
            detailTable["6"] = "�����";
            detailTable["7"] = "Ь��";
            detailTable["8"] = "����";
            detailTable["9"] = "ñ��";
            detailTable["10"] = "��������";
            detailTable["11"] = "��������";
        }

        /// <summary>
        /// ����б�
        /// </summary>
        public void FillLists()
        {
            foreach(string s in totalAttributes)
            {
                if(existAttributes.Contains(s)) // �����ѡ�������б�
                {
                    listBox2.Items.Add(s);
                    addedAttributes.Add(s);

                    // �޸�������ѡ��״̬
                    foreach (TreeNode node1 in treeView1.Nodes)
                    {
                        GetNode(node1, s);
                    }  

                    if (findNode != null)
                    {
                        findNode.Checked = true;
                        findNode = null;
                    }  
                }
            }
        }

        /// <summary>
        /// �����������
        /// </summary>
        private void FillTotalAttributes()
        {
            try
            {
                string sql = string.Format("SELECT * FROM {0}", itemTable);
                SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                DataTable tbl = ds.Tables[0];
                dt = tbl;

                // ��¼���Ժ͸�����Ϣ
                foreach(DataRow r in tbl.Rows)
                {
                    object o = r["Name"].ToString();
                    string[] information = new string[4];
                    information[0] = r["Genre"].ToString();
                    information[1] = r["DetailType"].ToString();
                    information[2] = r["ParticularType"].ToString();
                    information[3] = r["Level"].ToString();
                    totalAttributes.Add(r["Name"].ToString());
                    table[o] = information;
                }

                // �µ���ӷ���
                string[] catFields = GetCatfields("dropitem");
                foreach (DataRow r in tbl.Rows)
                {
                    TreeNodeCollection currentNodes = treeView1.Nodes; // ��ǰ����㼯��
                    string name = r["Name"].ToString();

                    foreach (string c in catFields)
                    {
                        bool find = false;
                        TreeNode findNode = null;
                        string catName = r[c].ToString().Trim();
                        if(c == "DetailType") // ��DetailType�����滻Ϊ������
                        {
                            catName = detailTable[catName].ToString();
                        }

                        foreach (TreeNode node2 in currentNodes)
                        {
                            if (node2.Text == catName)
                            {
                                find = true;
                                findNode = node2;
                                break;
                            }
                        }

                        if (!find)
                        {
                            
                            findNode = currentNodes.Add(catName);
                        }

                        currentNodes = findNode.Nodes;
                    }

                    TreeNode addNode = currentNodes.Add(name);
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("��������ɾ����Ʒ��Ϣʱ����sql�쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// ��ȡ�����ķ�����Ϣ
        /// </summary>
        /// <param name="itemTable">����</param>
        /// <returns>������Ϣ</returns>
        private string[] GetCatfields(string itemTable)
        {
            string sql = string.Format("SELECT {0} FROM {1} WHERE {2} = \'{3}\'", "catfields", "sys_modl_tab_def",
                "tablename", itemTable);
            SqlCommand cmd;
            SqlDataReader reader = null;
            string[] classifyNames = null;

            try
            {
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    if (reader[0].ToString().Trim() != "")
                    {
                        classifyNames = reader[0].ToString().Trim().Split(',');
                        catLength = classifyNames.Length; // ��¼����
                        break;
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("��ȡ����������Ϣʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(reader != null)
                {
                    reader.Close();
                }
            }
            return classifyNames;
        }

        /// <summary>
        /// �����������
        /// </summary>
        private void FillExistAttributes()
        {
            try
            {
                List<string[]> informations = new List<string[]>();
                string sql = string.Format("SELECT * FROM {0}", dropTable);
                SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                DataTable tbl = ds.Tables[0];

                foreach (TreeNode node1 in node.Nodes)
                {
                    string id = (node1.Tag as object[])[0].ToString();
                    if (node1.Text == "-1" || node1.Text == "��Ч��Ʒ")
                    {
                        continue;
                    }
                    DataRow r = tbl.Rows.Find(id);
                    string[] information = new string[6];
                    information[0] = r["Genre"].ToString();
                    information[1] = r["Detail"].ToString();
                    information[2] = r["Particular"].ToString();
                    information[3] = r["Level"].ToString();
                    information[4] = r["RandRate"].ToString();
                    information[5] = r["LuckyRate"].ToString();
                    informations.Add(information);
                }

                foreach (string[] s in informations)
                {
                    string[] t = new string[4];
                    for(int i = 0; i < 4; i++)
                    {
                        t[i] = s[i];
                    }
                    DataRow r = dt.Rows.Find(t);
                    if(r != null)
                    {
                        existAttributes.Add(r["Name"].ToString());
                        table[r["Name"].ToString()] = s; // ���¼�¼��Ϣ
                    }
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("��������ɾ����Ʒ��Ϣʱ����sql�쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// ɾ������
        /// </summary>
        /// <param name="id">id��</param>
        private void DeleteData(string id)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("DELETE FROM {0} WHERE {1} = {2}", dropTable, "id", id);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("ɾ��idΪ{0}�ļ�¼ʱ����sql�쳣: " + ex.ToString(), id));
            }
        }

        /// <summary>
        /// �������
        /// </summary>
        /// <param name="name">��Ʒ����</param>
        /// <param name="label">���������</param>
        private void InsertData(string name, int label)
        {
            try
            {
                string[] information = table[name] as string[];
                if(information == null)
                {
                    return;
                }
                string randRate = "0";
                string luckyRate = "0";
                if(information.Length == 6)
                {
                    randRate = information[4];
                    luckyRate = information[5];
                }

                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("INSERT INTO {0} ({1}) VALUES ({2})", dropTable,
                    "filename, label, Genre, Detail, Particular, Level, RandRate, LuckyRate", string.Format("\'{0}\'", 
                    fileName) + ", " + string.Format("\'{0}\'", label.ToString()) + ", " +
                    string.Format("\'{0}\'", information[0]) + ", " + string.Format("\'{0}\'", information[1]) + 
                    ", " + string.Format("\'{0}\'", information[2]) + ", " + string.Format("\'{0}\'", information[3]) +
                    string.Format(", \'{0}\', \'{1}\'", randRate, luckyRate));
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("�����Ʒ����Ϊ{0}�ļ�¼ʱ����sql�쳣: " + ex.ToString(), name));
            }      
        }

        /// <summary>
        /// �������(�Զ�������Ʒ����)
        /// </summary>
        /// <param name="name">��Ʒ����</param>
        private void InsertData(string name)
        {
            try
            {
                string[] information = table[name] as string[];
                if (information == null)
                {
                    return;
                }
                string randRate = "0";
                string luckyRate = "0";
                if (information.Length == 6)
                {
                    randRate = information[4];
                    luckyRate = information[5];
                }

                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("INSERT INTO {0} (filename, Genre, Detail, Particular, Level, RandRate, LuckyRate, Name) VALUES ('{1}', '{2}', '{3}', '{4}', '{5}', '{6}', '{7}', '{8}')", 
                    dropTable, fileName, information[0], information[1], information[2], information[3], randRate, luckyRate, name);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("�����Ʒ����Ϊ{0}�ļ�¼ʱ����sql�쳣: " + ex.ToString(), name));
            }     
        }

        /// <summary>
        /// ��ȡid��
        /// </summary>
        /// <param name="information">������Ʒ��Ϣ</param>
        /// <returns>id��</returns>
        private string GetID(string[] information)
        {
            string id = null;

            if(information == null) // ������Ʒ��ϢΪ��
            {
                return null;
            }

            try
            {
                
                string sqlString = string.Format("SELECT id FROM {0} WHERE filename = '{1}' AND Genre = '{2}' AND Detail = '{3}' AND Particular = '{4}' AND Level = '{5}'",
                    dropTable, fileName, information[0], information[1], information[2], information[3]);
                DataTable table = GetDataTable(sqlString);
                if(table.Rows.Count > 0)
                {
                    id = table.Rows[0][0].ToString();
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڻ�ȡ������Ʒ��Ӧidʱ����sql�쳣: " + ex.ToString());
            }

            return id;
        }

        /// <summary>
        /// ��ȡid��
        /// </summary>
        /// <param name="label">���������</param>
        /// <returns>id��</returns>
        private string GetID(int label)
        {
            string id = null;
            string sql = string.Format("SELECT {0} FROM {1} WHERE {2} = \'{3}\' AND {4} = \'{5}\'", "id",
                dropTable, "filename", fileName, "label", label.ToString());
            SqlCommand cmd;
            SqlDataReader reader = null;
            try
            {
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                reader.Read();
                if (!reader.HasRows) // ��ѯ���Ϊ��
                {
                    reader.Close();
                    return null;
                }
                else
                {
                    id = reader[0].ToString();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("���ID�Ƿ����ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if (reader != null) // reader��δ�ر�
                {
                    reader.Close();
                }
            }
            return id;
        }

        /// <summary>
        /// ���浱ǰ����(�Զ������������)
        /// </summary>
        private void SaveDataResetItemName()
        {
            List<string> deleteIdList = new List<string>(); // Ҫɾ����id����
            List<string[]> addNodeList = new List<string[]>(); // Ҫ��ӵ����������
            List<TreeNode> deleteNodeList = new List<TreeNode>(); // Ҫɾ������������� 

            foreach (TreeNode node1 in node.Nodes) // ���������
            {
                if (node1.Text != "-1" && node1.Text != "��Ч��Ʒ") // �������¼
                {
                    deleteIdList.Add((node1.Tag as object[])[0].ToString());
                    deleteNodeList.Add(node1);
                }
            }

            foreach (string s in deleteIdList) // ɾ����¼
            {
                DeleteData(s);
            }

            foreach (object o in listBox2.Items) // ������ѡ����
            {
                string name = o as string;
                InsertData(name);
                string id = GetID(table[name] as string[]);
                if (id != null)
                {
                    //addNodeList.Add(new string[2]{id, label.ToString()});
                    addNodeList.Add(new string[2] { id, name });
                }
            }

            foreach (TreeNode node1 in deleteNodeList) // ɾ�������
            {
                node1.Remove();
            }

            foreach (string[] s in addNodeList) // ��������
            {
                TreeNode node1 = node.Nodes.Add(s[1]);
                node1.Tag = new object[] { s[0] };
            }
        }

        /// <summary>
        /// ���浱ǰ����
        /// </summary>
        private void SaveData()
        {
            List<string> deleteIdList = new List<string>(); // Ҫɾ����id����
            List<string[]> addNodeList = new List<string[]>(); // Ҫ��ӵ����������
            List<TreeNode> deleteNodeList = new List<TreeNode>(); // Ҫɾ������������� 
            int label = 1; // �µ����

            foreach(TreeNode node1 in node.Nodes) // ���������
            {
                if(node1.Text != "-1" && node1.Text != "��Ч��Ʒ") // �������¼
                {
                    deleteIdList.Add((node1.Tag as object[])[0].ToString());
                    deleteNodeList.Add(node1);
                }
            }

            foreach(string s in deleteIdList) // ɾ����¼
            {
                DeleteData(s);
            }

            foreach(object o in listBox2.Items) // ������ѡ����
            {
                string name = o as string;
                InsertData(name, label);
                string id = GetID(label);
                if(id != null)
                {
                    //addNodeList.Add(new string[2]{id, label.ToString()});
                    addNodeList.Add(new string[2] { id, name });
                }
                label++;
            }

            foreach(TreeNode node1 in deleteNodeList) // ɾ�������
            {
                node1.Remove();
            }

            foreach(string[] s in addNodeList) // ��������
            {
                TreeNode node1 = node.Nodes.Add(s[1]);
                node1.Tag = new object[] {s[0]};
            }
        }

        /// <summary>
        /// ���������������ѡ�е������б���
        /// </summary>
        /// <param name="node">ѡ��������</param>
        private void FillSellectedAttributes(TreeNode node)
        {
            if(node.Level == catLength)
            {
                if(node.Checked)
                {
                    string text = node.Text;
                    if(!addedAttributes.Contains(text))
                    {
                        listBox2.Items.Add(text);
                        listBox2.SelectedItem = text;
                        addedAttributes.Add(text);
                    }
                }
            }
            else
            {
                foreach(TreeNode node1 in node.Nodes)
                {
                    FillSellectedAttributes(node1);
                }
            }
        }

        /// <summary>
        /// �Զ�������ѡ���
        /// </summary>
        /// <param name="node"></param>
        private void AutoFill(TreeNode node, bool isChecked)
        {
            if(node.Checked != isChecked)
            {
                node.Checked = isChecked;
            }
        }

        /// <summary>
        /// ��ȡ�ı�Ϊ����ֵ�������
        /// </summary>
        /// <param name="node">�����</param>
        /// <param name="text">�ı�����</param>
        private void GetNode(TreeNode node, string text)
        {
            if(node.Text == text)
            {
                findNode = node;
                return;
            }
            else
            {
                foreach(TreeNode node2 in node.Nodes)
                {
                    GetNode(node2, text);
                }
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if(autoResetItemName) // �Զ�������Ʒ����
            {
                SaveDataResetItemName();
            }
            else
            {
                SaveData();
            }
            
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// �û�ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }

        /// <summary>
        /// ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            foreach(TreeNode node in treeView1.Nodes)
            {
                FillSellectedAttributes(node);
            }
        }

        /// <summary>
        /// ȫ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            foreach(TreeNode node1 in treeView1.Nodes) // ȫ��ѡ�е�һ����
            {
                if(!node1.Checked)
                {
                    node1.Checked = true;
                }
            }
            buttonX3_Click(null, null); // ������
        }

        /// <summary>
        /// ɾ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX5_Click(object sender, EventArgs e)
        {
            object o = listBox2.SelectedItem;
            string name = o as string;
            foreach(TreeNode node1 in treeView1.Nodes)
            {
                GetNode(node1, name);
            }
            addedAttributes.Remove(name);
            listBox2.Items.Remove(name);

            if(findNode != null)
            {
                treeView1.SelectedNode = findNode;
                findNode.Checked = false;
                findNode = null;
            }
        }

        /// <summary>
        /// ȫ��ɾ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX6_Click(object sender, EventArgs e)
        {
            List<string> deleteAttributes = new List<string>();
            foreach(object o in listBox2.Items) // ���������Ѿ�ѡ�������
            {
                deleteAttributes.Add(o as string);
            }
            foreach(string s in deleteAttributes)
            {
                listBox2.SelectedItem = s;
                buttonX5_Click(null, null);
            }
        }

        /// <summary>
        /// �û������б��е�ѡ��ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void listBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            object o = listBox2.SelectedItem;
            if (o == null)
            {
                return;
            }
            string[] information = table[o] as string[];
            if (information != null)
            {
                textBoxX1.Text = information[0];
                textBoxX2.Text = information[1];
                textBoxX3.Text = information[2];
                textBoxX4.Text = information[3];
            }  
        }

        /// <summary>
        /// �����ѡ��ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            TreeNode currentNode = treeView1.SelectedNode;
            if(currentNode != null)
            {
                if(currentNode.Level == catLength) // ��ײ�Ľ��
                {
                    string text = currentNode.Text;
                    string[] information = table[text] as string[];
                    if (information != null)
                    {
                        textBoxX1.Text = information[0];
                        textBoxX2.Text = information[1];
                        textBoxX3.Text = information[2];
                        textBoxX4.Text = information[3];
                    }
                }
            }
        }

        /// <summary>
        /// ������ѡ���ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_AfterCheck(object sender, TreeViewEventArgs e)
        {
            TreeNode currentNode = e.Node;
            if (currentNode != null)
            {
                foreach (TreeNode node1 in currentNode.Nodes)
                {
                    AutoFill(node1, currentNode.Checked);
                }
            }
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            DataTable table = ds.Tables[0];
            return table;
        }
    }
}