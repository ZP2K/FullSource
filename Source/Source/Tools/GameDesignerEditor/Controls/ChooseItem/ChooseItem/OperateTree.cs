using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using DevComponents.DotNetBar;

namespace ChooseItem
{
    public partial class OperateTree : Form
    {
        private PanelDockContainer PDC;
        private TreeViewEventHandler afterSelect;
        private TreeViewCancelEventHandler beforeSelect;
        private NodeLabelEditEventHandler beforeLabelEdit;
        private NodeLabelEditEventHandler afterLabelEdit;

        private SqlConnection conn; // sql����
        private string dropTable = "droprate";
        private string itemTable = "dropitem";
        private bool autoResetItemName = false; // �Ƿ��Զ�������ߵ�����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="tree">��</param>
        public OperateTree(SqlConnection conn)
        {
            this.conn = conn;
            InitializeComponent();
        }

        /// <summary>
        /// �����ʱ�����
        /// </summary>
        public string DropTable
        {
            set
            {
                dropTable = value;
            }
        }

        /// <summary>
        /// ������Ʒ������
        /// </summary>
        public string ItemTable
        {
            set
            {
                itemTable = value;
            }
        }

        /// <summary>
        /// �Ƿ��Զ�������ߵ���������
        /// </summary>
        public bool AutoResetItemName
        {
            set
            {
                autoResetItemName = value;
            }
        }

        /// <summary>
        /// �������в���
        /// </summary>
        public void Operate(PanelDockContainer pdc, TreeViewEventHandler afterselect, TreeViewCancelEventHandler beforeselect, NodeLabelEditEventHandler beforelabeledit, NodeLabelEditEventHandler afterlabeledit)
        {
            //tree.ContextMenuStrip = this.contextMenuStrip1;
            PDC = pdc;
            this.afterSelect = afterselect;
            this.beforeSelect = beforeselect;
            this.beforeLabelEdit = beforelabeledit;
            this.afterLabelEdit = afterlabeledit;
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <returns>����</returns>
        private TreeView ReloadTree()
        {
            string checkFileName = ""; // ��������ļ���
            string checkID = ""; // �������������id 

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0}", dropTable);
                DataTable dTable = GetDataTable(sqlString);

                sqlString = string.Format("SELECT * FROM {0}", itemTable);
                DataTable iTable = GetDataTable(sqlString);

                foreach(DataRow r in dTable.Rows)
                {
                    string fileName = r["filename"].ToString();
                    string id = r["id"].ToString();
                    string nodeName = "��Ч��Ʒ";
                    bool found = false;
                    TreeNode currentNode = null;
                    checkFileName = fileName;
                    checkID = id;

                    foreach(TreeNode node1 in treeView1.Nodes)
                    {
                        if(node1.Text == fileName)
                        {
                            currentNode = node1;
                            found = true;
                            break;
                        }
                    }

                    if(!found)
                    {
                        currentNode = treeView1.Nodes.Add(fileName);
                        currentNode.Tag = new object[]{"-2"};
                    }
               
                    DataRow row;

                    if((r["Genre"] is DBNull) || (r["Genre"] == null) ||
                        (r["Detail"] is DBNull) || (r["Detail"] == null)||
                        (r["Particular"] is DBNull) || (r["Particular"] == null)||
                        (r["Level"] is DBNull) || (r["Level"] == null)) // ��Ч�ĵ�����Ʒ
                    {
                        row = null;
                    }
                    else
                    {
                        string[] keys = new string[4];
                        keys[0] = r["Genre"].ToString();
                        keys[1] = r["Detail"].ToString();
                        keys[2] = r["Particular"].ToString();
                        keys[3] = r["Level"].ToString();
                        row = iTable.Rows.Find(keys);
                    }
             
                    if(row != null) // �ҵ�ƥ�����Ʒ
                    {
                        nodeName = row["Name"].ToString();
                        if (autoResetItemName) // ��Ҫ������ߵ�����
                        {
                            if((r["Name"] is DBNull) || (r["Name"] == null) || (r["Name"].ToString() == "")) // ԭ��������Ϊ��
                            {
                                ReSetItemName(r["id"].ToString(), nodeName);
                            }
                        }
                    }
                    else // û���ҵ�ƥ�����Ʒ
                    {
                        if(autoResetItemName) // �Զ�������Ʒ����
                        {
                            if (!(r["Name"] is DBNull) && (r["Name"] != null) && (r["Name"].ToString() != "")) // ԭ�������Ʋ�Ϊ��
                            {
                                nodeName = r["Name"].ToString();
                            }
                        }
                    }

                    TreeNode addNode = currentNode.Nodes.Add(nodeName);
                    addNode.Tag = new object[]{id};
                }
            
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show(string.Format("�����¹����������ʱ�����쳣�������ļ�{0}�е�idΪ{1}������\n\n", checkFileName, checkID) + 
                    ex.ToString());
                return null;
            }
            return treeView1;
        }

        /// <summary>
        /// ������Ʒ����
        /// </summary>
        /// <param name="id">������Ʒ��Ӧ��id</param>
        private void ReSetItemName(string id, string itemName)
        {
            try
            {
                string sqlString = string.Format("UPDATE {0} SET Name = '{1}' WHERE id = '{2}'", dropTable, itemName, id);
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڸ��µ����ʱ����Ʒ������Ϣʱ����sql�쳣: " + ex.ToString());
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

        /// <summary>
        /// ��ȡ�����
        /// </summary>
        /// <returns>�����</returns>
        public TreeView LoadTreePlugin()
        {
            TreeView tree = ReloadTree();
            PDC.Controls.Add(tree);
            tree.Dock = DockStyle.Fill;

            tree.ContextMenuStrip = this.contextMenuStrip1;
            tree.AfterSelect += afterSelect;
            tree.BeforeSelect += beforeSelect;
            tree.BeforeLabelEdit += beforeLabelEdit;
            tree.AfterLabelEdit += afterLabelEdit;

            return tree;
        }

        /// <summary>
        /// �������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void multipleAddToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treeView1.SelectedNode;
            if(currentNode != null && currentNode.Level == 1)
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }
                MultiAddForm mForm = new MultiAddForm(conn, currentNode, itemTable, dropTable, autoResetItemName);
                DialogResult result = mForm.ShowDialog();
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// �����ض��ļ��ĵ�����ֵ��ֵ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void computeTotalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                TreeNode currentNode = treeView1.SelectedNode;
                if(currentNode != null)
                {
                    int sumRandRate = 0;

                    if(currentNode.Parent != null)
                    {
                        currentNode = currentNode.Parent;
                    }

                    string fileName = currentNode.Text;

                    string sql = string.Format("SELECT RandRate FROM {0} WHERE {1} = \'{2}\'",
                        dropTable, "filename", fileName);
                    SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
                    adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                    DataSet ds = new DataSet();
                    adp.Fill(ds);
                    DataTable dTable = ds.Tables[0];

                    foreach(DataRow r in dTable.Rows)
                    {
                        if(!(r["RandRate"] is DBNull))
                        {
                            int value = int.Parse(r["RandRate"].ToString());
                            if(value > 0)
                            {
                                sumRandRate += value;
                            }
                        }
                    }

                    Clipboard.SetDataObject(sumRandRate.ToString(), true);
                    MessageBox.Show(string.Format("�ļ�{0}�ĵ�����ֵΪ{1}!\n\n��ֵ�Ѿ������������!", fileName, sumRandRate.ToString()), 
                        "������ֵ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("�ڼ��������ֵʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }
    }
}