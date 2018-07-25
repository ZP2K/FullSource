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
    public partial class Form1 : Form
    {
        private SqlConnection conn; // sql����
        private string id; // id��
        private string genre; // genre�ֶ�
        private string detail; // detail�ֶ�
        private string particular; // particular�ֶ�
        private string level; // level�ֶ�
        private string itemName; // itemName�ֶ�
        private List<string> tableNames = new List<string>(); // ��Ʒ����������
        private Hashtable table = new Hashtable(); // �����ID��Ӧhash��
        private Hashtable detailTable = new Hashtable(); // detail�������Ϣ��hash��
        private Hashtable particularTable = new Hashtable(); // particular�������Ϣ��hash��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="tableNames">��������</param>
        public Form1(SqlConnection conn, List<string> tableNames, Hashtable detailTable, Hashtable particularTable)
        {
            this.conn = conn;
            this.tableNames = tableNames;
            this.detailTable = detailTable;
            this.particularTable = particularTable;
            if(conn.State == ConnectionState.Closed) // ��sql����
            {
                conn.Open();
            }
            InitializeComponent();
            InitTree();
            if(conn.State == ConnectionState.Open) // �ر�sql����
            {
                conn.Close();
            }
        }

        /// <summary>
        /// id����
        /// </summary>
        public string ID
        {
            get
            {
                return id;
            }

            set
            {
                id = value;
            }
        }

        /// <summary>
        /// genre����
        /// </summary>
        public string Genre 
        {
            get
            {
                return genre;
            }

            set
            {
                genre = value;
            }
        }

        /// <summary>
        /// detail����
        /// </summary>
        public string Detail
        {
            get
            {
                return detail;
            }

            set
            {
                detail = value;
            }
        }

        /// <summary>
        /// particular����
        /// </summary>
        public string Particular
        {
            get
            {
                return particular;
            }

            set
            {
                particular = value;
            }
        }

        /// <summary>
        /// level����
        /// </summary>
        public string Level
        {
            get
            {
                return level;
            }

            set
            {
                level = value;
            }
        }

        /// <summary>
        /// itemName����
        /// </summary>
        public string ItemName
        {
            get
            {
                return itemName;
            }

            set
            {
                itemName = value;
            }
        }

        /// <summary>
        /// ������������
        /// </summary>
        public List<string> TableNames
        {
            get 
            {
                return tableNames;
            }

            set
            {
                tableNames = value;
            }
        }

        /// <summary>
        /// ��ʼ����Ʒ��
        /// </summary>
        private void InitTree()
        {
            foreach(string s in tableNames)
            {
                TreeNode node1 = treeView1.Nodes.Add(s);
                string[] catFields = GetCatfields(s);
                List<string> existCatFields = new List<string>();

                string sql = string.Format("SELECT * FROM {0}", s);
                SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
                DataSet ds = new DataSet();
                adp.Fill(ds);
                DataTable tbl = ds.Tables[0];
                
                foreach(DataRow r in tbl.Rows)
                {
                    TreeNode currentNode = node1;
                    string name = r["Name"].ToString(); 

                    foreach(string c in catFields)
                    {
                        bool find = false;
                        TreeNode findNode = null;
                        string catName = r[c].ToString().Trim();
                        if(c == "DetailType" && detailTable.Count != 0) // ��Ӧtable�����滻
                        {
                            catName = detailTable[catName] as string; 
                        }
                        else if(c == "ParticularType" && particularTable.Count != 0) // ��Ӧtable�����滻
                        {
                            catName = particularTable[catName] as string;
                        }

                        foreach(TreeNode node2 in currentNode.Nodes)
                        {
                            if(node2.Text == catName)
                            {
                                find = true;
                                findNode = node2;
                                break;
                            }
                        }

                        if(!find)
                        {
                            findNode = currentNode.Nodes.Add(catName);
                        }

                        currentNode = findNode;
                    }

                    string[] information = new string[4];
                    information[0] = r["Genre"].ToString().Trim();
                    information[1] = r["DetailType"].ToString().Trim();
                    information[2] = r["ParticularType"].ToString().Trim();
                    information[3] = r["Level"].ToString().Trim();
                    TreeNode addNode = currentNode.Nodes.Add(name);
                    addNode.Tag = information;
                    table[addNode] = information;
                }
            }
        }

        /// <summary>
        /// ��ʼ��ѡ����
        /// </summary>
        public void InitSelection()
        {
            foreach(TreeNode node in table.Keys)
            {
                string[] information = table[node] as string[];
                if(information[0] == genre && information[1] == detail && information[2] == particular
                    && information[3] == level)
                {
                    treeView1.SelectedNode = node;
                    break;
                }
            }
        }

        /// <summary>
        /// ��ȡ�����ķ�����Ϣ
        /// </summary>
        /// <param name="tableName">����</param>
        /// <returns>������Ϣ</returns>
        private string[] GetCatfields(string tableName)
        {
            string sql = string.Format("SELECT {0} FROM {1} WHERE {2} = \'{3}\'", "catfields", "sys_modl_tab_def",
                "tablename", tableName);
            SqlCommand cmd;
            SqlDataReader reader = null;
            string[] classifyNames = null;

            try
            {
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                while(reader.Read())
                {
                    if(reader[0].ToString().Trim() != "")
                    {
                        classifyNames = reader[0].ToString().Trim().Split(',');
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
                reader.Close();
            }
            return classifyNames;
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treeView1.SelectedNode;
            if (currentNode != null && currentNode.Tag != null)
            {
                string[] information = currentNode.Tag as string[];
                if (information != null)
                {
                    Genre = information[0];
                    Detail = information[1];
                    Particular = information[2];
                    Level = information[3];
                    ItemName = currentNode.Text;
                    DialogResult = DialogResult.OK;
                    this.Close();
                }
            }
            else
            {
                MessageBox.Show("ѡ������Ч�������!");
            }
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
        /// �û�ѡ�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            TreeNode currentNode = treeView1.SelectedNode;
            if(currentNode != null && currentNode.Tag != null)
            {
                string[] information = currentNode.Tag as string[];
                if(information != null)
                {
                    textBoxX1.Text = information[0];
                    textBoxX2.Text = information[1];
                    textBoxX3.Text = information[2];
                    textBoxX4.Text = information[3];
                }
            }
        }
    }
}