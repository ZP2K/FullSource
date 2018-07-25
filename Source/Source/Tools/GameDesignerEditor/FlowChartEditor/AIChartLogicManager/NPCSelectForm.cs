using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

using DevComponents.DotNetBar;
using DevComponents.AdvTree;

namespace AIChartLogicManager
{
    public partial class NPCSelectForm : Office2007Form
    {
        private SqlConnection conn; // sql����
        private string npcTemplateID; // npcģ��ID
        private Node lastFindNode = null; // ��һ���ҵ��������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        public NPCSelectForm(SqlConnection conn)
        {
            this.conn = conn;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// �Ƿ��¼���عҽӵ�NPC
        /// </summary>
        public bool RecordAttachedNpc
        {
            get
            {
                return checkBoxX1.Checked;
            }
        }

        /// <summary>
        /// npcģ��ID
        /// </summary>
        public string NpcTemplateID
        {
            get
            {
                return npcTemplateID;
            }
        }

        /// <summary>
        /// ��ʼ��
        /// </summary>
        private void Init()
        {
            string sqlString = "SELECT ID, Name, MapName FROM NpcTemplate";
            DataTable npcTable = GetDataTable(sqlString);

            foreach (DataRow dataRow in npcTable.Rows)
            {
                string npcID = dataRow["ID"].ToString();
                string npcName = dataRow["Name"].ToString();
                string mapName = dataRow["MapName"].ToString();

                Node mapNode = null;

                foreach (Node node in npcTree.Nodes)
                {
                    if (node.Text == mapName)
                    {
                        mapNode = node;
                        break;
                    }
                }

                if (mapNode == null)
                {
                    mapNode = new Node();
                    mapNode.Text = mapName;
                    npcTree.Nodes.Add(mapNode);
                }

                Node newNode = new Node();
                newNode.Text = string.Format("{0} ({1})", npcName, npcID);
                newNode.TagString = npcID;
                mapNode.Nodes.Add(newNode);
            }
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        public DataTable GetDataTable(string sqlString)
        {
            DataTable tb = null;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }
                
                SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                tb = ds.Tables[0];
            }
            catch (SqlException ex)
            {
                MessageBox.Show("SQL���ݿ����" + ex.Message);
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return tb;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            Node currentNode = npcTree.SelectedNode;

            if (currentNode != null && currentNode.Level == 1)
            {
                npcTemplateID = currentNode.TagString;
                DialogResult = DialogResult.OK;
                this.Close();
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

        /// <summary>
        /// ѡ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            string searchString = null;

            if (textBoxX1.Text != "" && textBoxX2.Text != "")
            {
                searchString = string.Format("{0} ({1})", textBoxX2.Text, textBoxX1.Text);
            }
            else if (textBoxX1.Text != "")
            {
                searchString = textBoxX1.Text;
            }
            else if (textBoxX2.Text != "")
            {
                searchString = textBoxX2.Text;
            }

            if (searchString != null)
            {
                Node findNode = null;
                bool bLabel = false;

                foreach (Node mapNode in npcTree.Nodes)
                {
                    foreach (Node npcNode in mapNode.Nodes)
                    {
                        if (lastFindNode == null)
                        {
                            bLabel = true;
                        }

                        if (npcNode == lastFindNode)
                        {
                            bLabel = true;
                            continue;
                        }                        

                        if (npcNode.Text.Contains(searchString) && bLabel)
                        {                            
                            findNode = npcNode;
                            lastFindNode = npcNode;
                            break;
                        }
                    }

                    if (findNode != null)
                    {
                        break;
                    }
                }

                if (findNode != null)
                {
                    npcTree.SelectedNode = findNode;
                }

                if (!bLabel)
                {
                    lastFindNode = null;
                }
            }
            else
            {
                MessageBox.Show("�����������������", "����", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}