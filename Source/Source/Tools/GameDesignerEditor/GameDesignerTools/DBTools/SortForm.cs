using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace GameDesingerTools.DBTools
{
    public partial class SortForm : Form
    {
        private TreeNode node; // �����
        private SqlConnection conn; // sql����
        private ImageList imageList;
        List<string> cat = new List<string>(); // ��������
        private TreeNode OldDragOver = null;

        public SortForm(SqlConnection conn, TreeNode node, ImageList imageList)
        {
            this.conn = conn;
            this.node = node;
            this.imageList = imageList;
            InitializeComponent();
            treeView1.ImageList = imageList;
            InitData();
        }

        /// <summary>
        /// ��ʼ�����������
        /// </summary>
        private void InitData()
        {
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }
                foreach(TreeNode node1 in node.Nodes)
                {
                    InitNode(node1);
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�������ֶ����������ʱ����Sql�쳣: " + ex.ToString());
            }
            finally
            {            
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
            if(node.ImageIndex == 7) // �����ֶ��޷��½���
            {
                buttonX3.Enabled = false;
            }
        }

        /// <summary>
        /// ��ʼ�����
        /// </summary>
        /// <param name="node">�����</param>
        private void InitNode(TreeNode node)
        {
            string fieldid = node.Tag.ToString().Trim();
            string sql = string.Format("SELECT {0}, {1} FROM {2} WHERE {3} = {4}", "fieldname", "catname", "sys_meta_info", 
                "fieldid", fieldid);
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {              
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    string fieldName = node.Text;
                    string catName = reader[1].ToString().Trim();
                    if(catName == "") // ����δ����
                    {
                        catName = "δ����";
                    }
                    if (!cat.Contains(catName)) // ��������δ������������
                    {
                        TreeNode node1 = treeView1.Nodes.Add(catName);
                        node1.ImageIndex = 0;
                        node1.SelectedImageIndex = 0;
                        TreeNode node2 = node1.Nodes.Add(fieldName);
                        node2.Tag = fieldid;
                        node2.ImageIndex = node.ImageIndex;
                        node2.SelectedImageIndex = node.SelectedImageIndex;
                        cat.Add(catName);
                    }
                    else // ���������Ѵ���
                    {
                        foreach (TreeNode node1 in treeView1.Nodes)
                        {
                            if (node1.Text == catName)
                            {
                                TreeNode node2 = node1.Nodes.Add(fieldName);
                                node2.Tag = fieldid;
                                node2.ImageIndex = node.ImageIndex;
                                node2.SelectedImageIndex = node.SelectedImageIndex;
                                break;
                            }
                        }
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�������ֶ����������ʱ����Sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (reader != null) // �ر�reader
                {
                    reader.Close();
                }
            }
        }

        /// <summary>
        /// ������˳����Ϣ
        /// </summary>
        /// <param name="fieldid">����id</param>
        /// <param name="orderNo">���</param>
        private void SaveData(string fieldid, string orderNo)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE {3} = {4}", "sys_meta_info", "orderno", orderNo, 
                    "fieldid", fieldid);
                //cmd.ExecuteNonQuery();
                Helper.ExecuteCommand("sys_meta_info", cmd.CommandText, conn);
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�����Զ����ֶ�˳����Ϣʱ����Sql�쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// �����������
        /// </summary>
        /// <param name="fieldid">����id</param>
        /// <param name="catName">����</param>
        private void SaveType(string fieldid, string catName)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE {3} = {4}", "sys_meta_info", "catname", catName,
                    "fieldid", fieldid);
                //cmd.ExecuteNonQuery();
                Helper.ExecuteCommand("sys_meta_info", cmd.CommandText, conn);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�����Զ����ֶ�������Ϣʱ����Sql�쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// ��ȡ�½��
        /// </summary>
        /// <param name="desnode">Ŀ����</param>
        /// <param name="srcnode">Դ���</param>
        /// <param name="type">�϶�����</param>
        /// <returns>�½��</returns>
        private TreeNode DragNode(TreeNode desnode, TreeNode srcnode, DragDropEffects type)
        {
            TreeNode newAddNode = null;
            if(srcnode.Level == 0) // ��֧�ַ����϶�
            {
                MessageBox.Show("Ŀǰ��֧�ַ����϶�!");
                return null;
            }

            if (srcnode.Parent == desnode.Parent || srcnode.Parent == desnode) // ͬһ���Խ���϶�
            {
                if (desnode.Level == 0) // �϶�����������
                {
                    List<TreeNode> list1 = new List<TreeNode>();
                    foreach (TreeNode node in desnode.Nodes)
                    {
                        if (node != srcnode)
                        {
                            list1.Add(node);
                        }
                    }
                    desnode.Nodes.Clear();
                    desnode.Nodes.Add(srcnode);
                    foreach (TreeNode node in list1)
                    {
                        desnode.Nodes.Add(node);
                    }
                }
                else // �ϵ����Խ����
                {
                    TreeNodeCollection nodes = srcnode.Parent.Nodes;
                    List<TreeNode> list1 = new List<TreeNode>();
                    List<TreeNode> list2 = new List<TreeNode>();
                    bool findDesNode = false;
                    foreach (TreeNode node in nodes)
                    {
                        if (node != srcnode) // �Ƿ�Ϊ��ʼ���
                        {
                            if (node == desnode)
                            {
                                findDesNode = true;
                                list1.Add(node);
                                continue;
                            }

                            if (!findDesNode)
                            {
                                list1.Add(node);
                            }
                            else
                            {
                                list2.Add(node);
                            }
                        }
                    }

                    nodes.Clear();
                    foreach (TreeNode node in list1)
                    {
                        nodes.Add(node);
                    }
                    nodes.Add(srcnode);
                    foreach (TreeNode node in list2)
                    {
                        nodes.Add(node);
                    }
                }
            }
            else // ��ͬ���ԵĽ���϶�
            {
                if (desnode.Level == 0) // �϶�����������
                {
                    // ��ԭ��������ɾ�����Խ��
                    List<TreeNode> list1 = new List<TreeNode>();
                    TreeNode parentNode = srcnode.Parent;
                    foreach (TreeNode node in parentNode.Nodes)
                    {
                        if (node != srcnode)
                        {
                            list1.Add(node);
                        }
                    }
                    parentNode.Nodes.Clear();
                    foreach(TreeNode node in list1)
                    {
                        parentNode.Nodes.Add(node);
                    }

                    // ���·�������������Խ��
                    list1.Clear();
                    parentNode = desnode;
                    foreach(TreeNode node in parentNode.Nodes)
                    {
                        list1.Add(node);
                    }
                    parentNode.Nodes.Clear();
                    parentNode.Nodes.Add(srcnode);
                    foreach(TreeNode node in list1)
                    {
                        parentNode.Nodes.Add(node);
                    }
                }
                else // �ϵ����Խ����
                {
                    // ��ԭ��������ɾ�����Խ��
                    List<TreeNode> list1 = new List<TreeNode>();
                    TreeNode parentNode = srcnode.Parent;
                    foreach (TreeNode node in parentNode.Nodes)
                    {
                        if (node != srcnode)
                        {
                            list1.Add(node);
                        }
                    }
                    parentNode.Nodes.Clear();
                    foreach (TreeNode node in list1)
                    {
                        parentNode.Nodes.Add(node);
                    }

                    // ���·�������������Խ��
                    TreeNodeCollection nodes = desnode.Parent.Nodes;
                    list1.Clear();
                    List<TreeNode> list2 = new List<TreeNode>();
                    bool findDesNode = false;
                    foreach (TreeNode node in nodes)
                    {            
                        if (node == desnode)
                        {
                            findDesNode = true;
                            list1.Add(node);
                            continue;
                        }

                        if (!findDesNode)
                        {
                            list1.Add(node);
                        }
                        else
                        {
                            list2.Add(node);
                        }
                    }

                    nodes.Clear();
                    foreach (TreeNode node in list1)
                    {
                        nodes.Add(node);
                    }
                    nodes.Add(srcnode);
                    foreach (TreeNode node in list2)
                    {
                        nodes.Add(node);
                    }
                }
            }

            newAddNode = srcnode;           
            return newAddNode;
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            int index = 0;
            List<string> idList = new List<string>();
            if(conn.State == ConnectionState.Closed) // ��sql����
            {
                conn.Open();
            }
            foreach(TreeNode node1 in treeView1.Nodes)
            {
                foreach(TreeNode node2 in node1.Nodes)
                {
                    string fieldid = node2.Tag.ToString().Trim();
                    idList.Add(node2.Tag.ToString());
                    SaveData(fieldid, index.ToString());
                    if(node1.Text != "δ����")
                    {
                        SaveType(fieldid, node1.Text);
                    }
                    else
                    {
                        SaveType(fieldid, "");
                    }
                    index++;
                }
                index++;
            }
            if(conn.State == ConnectionState.Open) // �ر�sql����
            {
                conn.Close();
            }

            // �ı�ԭ����˳��
            List<TreeNode> nodeList = new List<TreeNode>();
            foreach(TreeNode node1 in node.Nodes)
            {
                nodeList.Add(node1);
            }
            node.Nodes.Clear();
            foreach(string s in idList)
            {
                foreach(TreeNode node1 in nodeList)
                {
                    if(s == node1.Tag.ToString())
                    {
                        node.Nodes.Add(node1);
                    }
                }
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
        /// �϶����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_ItemDrag(object sender, ItemDragEventArgs e)
        {
            DoDragDrop(e.Item, DragDropEffects.Move);
        }

        /// <summary>
        /// �϶�����ƶ�������Ϸ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_DragOver(object sender, DragEventArgs e)
        {
            Point targetPoint = treeView1.PointToClient(new Point(e.X, e.Y));
            TreeViewHitTestInfo tvhti = treeView1.HitTest(targetPoint);

            if (OldDragOver != tvhti.Node)
            {
                if (OldDragOver != null)
                    OldDragOver.BackColor = Color.Empty;

                OldDragOver = tvhti.Node;
                OldDragOver.BackColor = Color.BlueViolet;
            }

            if (tvhti.Location == TreeViewHitTestLocations.PlusMinus)
            {
                tvhti.Node.Expand();
            }
        }

        /// <summary>
        /// ��ɽ���϶�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent("System.Windows.Forms.TreeNode", false))
            {
                Point pt = ((TreeView)sender).PointToClient(new Point(e.X, e.Y));
                TreeNode Des_Node = ((TreeView)sender).GetNodeAt(pt);
                if(Des_Node == null)
                {
                    MessageBox.Show("��Ч���϶�!");
                    return;
                }
                TreeNode Src_Node = (TreeNode)e.Data.GetData("System.Windows.Forms.TreeNode");
                TreeNode newAddNode = null;
                try
                {
                    newAddNode = DragNode(Des_Node, Src_Node, e.Effect);
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show("���϶���� (DragNode) ʱ��������\r\n\r\n������ϢΪ��" + ex.Message);
                }

                if (newAddNode != null) // �ɹ��ģ���Ч�����϶�
                {
                    treeView1.SelectedNode = newAddNode;
                    if (e.Effect != DragDropEffects.Copy) // ����ͬ������϶�
                    {
                        Des_Node.Expand();
                    }
                }
                OldDragOver.BackColor = Color.Empty;
            }
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = e.AllowedEffect;
        }

        /// <summary>
        /// �����µķ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            TypeForm tf = new TypeForm();
            DialogResult result = tf.ShowDialog();
            if(result == DialogResult.OK)
            {
                foreach(TreeNode node in treeView1.Nodes)
                {
                    if(node.Text == tf.TypeName)
                    {
                        MessageBox.Show("�÷����Ѿ�����!");
                        return;
                    }
                }
                TreeNode node1 = treeView1.Nodes.Add(tf.TypeName);
                node1.ImageIndex = 0;
                node1.SelectedImageIndex = 0;
            }
        }
    }
}