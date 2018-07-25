using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace GameLuaEditor
{
    public partial class RecordForm : Form
    {
        private SqlConnection conn; // sql����
        private ImageList imageList; // ��ͼ������
        private string id; // ѡ�е�id
        private string path; // ѡ�е������·��

        public RecordForm(SqlConnection conn, ImageList imageList)
        {
            this.conn = conn;
            this.imageList = imageList;
            InitializeComponent();
            InitData();
        }

        /// <summary>
        /// ѡ�е�id����
        /// </summary>
        public string ID
        {
            get
            {
                return id;
            }
        }

        /// <summary>
        /// ѡ�е�������·������
        /// </summary>
        public string Path
        {
            get
            {
                return path;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData()
        {
            treeView1.Nodes.Clear();
            treeView1.ImageList = imageList;

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }


                string sqlString = "SELECT DISTINCT id, path FROM sys_script_log WHERE id NOT IN " +
                    "(SELECT id FROM sys_script_script)";
                DataTable table = GetDataTable(sqlString);

                foreach (DataRow r in table.Rows)
                {
                    string path = r["path"].ToString();
                    string id = r["id"].ToString();

                    string[] paths = path.Split('\\');

                    TreeNodeCollection currentNodes = treeView1.Nodes;
                    TreeNode currentNode = null;
                    bool findNode = false;

                    foreach (string s in paths)
                    {
                        findNode = false;
                        if (s != "")
                        {
                            foreach (TreeNode node1 in currentNodes)
                            {
                                if (node1.Text == s)
                                {
                                    findNode = true;
                                    currentNodes = node1.Nodes;
                                    break;
                                }
                            }

                            if (!findNode)
                            {
                                currentNode = currentNodes.Add(s);
                                currentNode.Tag = id;
                                currentNodes = currentNode.Nodes;
                                if (s.EndsWith(".lua"))
                                {
                                    currentNode.ImageIndex = 1;
                                    currentNode.SelectedImageIndex = 1;
                                }
                                else
                                {
                                    currentNode.ImageIndex = 0;
                                    currentNode.SelectedImageIndex = 0;
                                }
                            }
                        }
                    }
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڳ�ʼ��ɾ����ʷ����ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            DataTable tb = ds.Tables[0];
            return tb;
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            TreeNode currentNode = treeView1.SelectedNode;
            if(currentNode != null)
            {
                id = currentNode.Tag as string;
                path = currentNode.FullPath;
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("������ѡ��Ҫ��ȡ����ʷ��¼!");
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
    }
}