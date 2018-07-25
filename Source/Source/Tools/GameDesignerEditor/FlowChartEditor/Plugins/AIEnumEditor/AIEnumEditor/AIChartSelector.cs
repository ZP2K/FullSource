using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace AIEnumEditor
{
    public partial class AIChartSelector : Form
    {
        private SqlConnection conn; // sql����
        private string lastID; // ��ֵ
        private string aiChartID; // AIͼ��id
        private string aiChartPath; // AIͼ��·��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="lastID">��ֵ</param>
        public AIChartSelector(SqlConnection conn, string lastID)
        {
            this.conn = conn;
            this.lastID = lastID;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// AIͼ��ID
        /// </summary>
        public string AIChartID
        {
            get
            {
                return aiChartID;
            }
        }

        /// <summary>
        /// AIͼ��·��
        /// </summary>
        public string AIChartPath
        {
            get
            {
                return aiChartPath;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            DataTable table = null;

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = "SELECT ID, FullPath FROM Diagram WHERE [Class] = 'AIͼ' AND [IsFolder] = 'False'";
                table = GetDataTable(sqlString);
            }
            catch (SqlException ex)
            {
            	MessageBox.Show("�ڶ�ȡAIͼ����ʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            advTree1.Nodes.Clear();
            DevComponents.AdvTree.NodeCollection nodes;
            DevComponents.AdvTree.Node newNode;
            DevComponents.AdvTree.Node selectedNode = null;
            bool find = false;

            foreach(DataRow row in table.Rows)
            {
                string id = row["ID"].ToString();
                string fullPath = row["FullPath"].ToString();
                string[] datas = fullPath.Split(new char[] { '\\' });
                nodes = advTree1.Nodes;

                for(int i = 0; i < datas.Length; i++)
                {
                    string data = datas[i];
                    find = false;

                    foreach (DevComponents.AdvTree.Node node in nodes)
                    {
                        if (node.Text == data)
                        {
                            nodes = node.Nodes;
                            find = true;
                            break;
                        }
                    }

                    if (!find)
                    {
                        newNode = new DevComponents.AdvTree.Node();
                        newNode.Text = data;
                        nodes.Add(newNode);
                        nodes = newNode.Nodes;

                        if (i == datas.Length - 1)
                        {
                            newNode.Tag = id;
                            if (id == lastID)
                            {
                                selectedNode = newNode;
                            }
                        }
                    }
                }
            }

            // ѡ�о�ֵ����������
            advTree1.SelectedNode = selectedNode;
        }

        /// <summary>
        /// ��ȡ������·��
        /// </summary>
        /// <param name="node">��ǰ�����</param>
        /// <returns>������·��</returns>
        private string GetNodePath(DevComponents.AdvTree.Node node)
        {
            string path = node.Text;
            DevComponents.AdvTree.Node parentNode = node.Parent;
            while(parentNode != null)
            {
                path = string.Format("{0}/", parentNode.Text) + path;
                parentNode = parentNode.Parent;
            }

            return path;
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString)
        {
            DataTable tb = null;
            
            SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            tb = ds.Tables[0];

            return tb;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            DevComponents.AdvTree.Node selectedNode = advTree1.SelectedNode;
            if(selectedNode != null && selectedNode.Tag != null)
            {
                aiChartID = selectedNode.Tag.ToString();
                aiChartPath = GetNodePath(selectedNode);                
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("��Ч��ѡ��", "ѡ��AIͼ", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}