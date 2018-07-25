using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;
using System.IO;
using DevComponents.DotNetBar;

namespace GameDesingerTools.DBTools
{
    public partial class ModelConfigForm : Form
    {
        private TreeNode OldDragOver = null; // �ɵ��϶������
        private MainForm mainForm; // ���������
        private ImageList imageList = new ImageList(); // ͼ������
        private Hashtable modelTable = new Hashtable(); // ģ�����������hash��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="mainForm">���������</param>
        public ModelConfigForm(MainForm mainForm)
        {
            this.mainForm = mainForm;
            InitializeComponent();
            InitData();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData()
        {
            InitImageList();
            InitNode();
        }

        /// <summary>
        /// ��ȡ�û����Զ���ģ����ʾ�ַ���
        /// </summary>
        /// <param name="hostname">�û���</param>
        /// <returns>�û��Զ���ģ����ʾ�ַ���</returns>
        private string GetConfigString(string hostname)
        {
            SqlConnection conn = new SqlConnection(Program.ConnetionString); // sql����
            string configString = null; // �û����Զ���ģ����ʾ�ַ���

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }
                /* remoting
                string sqlString = string.Format("SELECT data FROM {0} WHERE host = '{1}'", "sys_modl_display_cfg", hostname);
                DataTable table = Helper.GetDataTable(sqlString, conn);
                */

                string filter = string.Format("host = '{0}'", hostname);
                DataTable table = Helper.GetDataTableProxy("sys_modl_display_cfg", filter, null, conn);

                if(table.Rows.Count > 0)
                {
                    if (!(table.Rows[0]["data"] is DBNull)) // �ַ������ݷǿ�
                    {
                        configString = table.Rows[0]["data"].ToString();
                    }
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show(string.Format("�ڻ�ȡ�û���Ϊ{0}���û��Զ���ģ����ʾ����ʱ����sql�쳣: ", hostname) + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            return configString;
        }

        /// <summary>
        /// ��ʼ�������
        /// </summary>
        private void InitNode()
        {
            int index = 1; // key���
            TreeNode newKindNode; // ���������
            TreeNode newValueNode; // ֵ���
            List<TreeNode> nodeList = new List<TreeNode>(); // �Ѿ�����˵�����������

            bool readHideKind = false; // �Ƿ��Ѷ��Զ����ط��� 
            
            string configString = GetConfigString(System.Net.Dns.GetHostName());
            if(configString != null) // �û��Զ���ģ����ʾ�ַ�����Ϊ��
            {
                string[] kinds = configString.Split(new char[] { ';' });
                foreach(string s in kinds)
                {
                    if (readHideKind) // �Ѿ������Զ����ط���
                    {
                        newKindNode = treeView1.Nodes.Add("����" + index);
                        index++;
                    }
                    else // û�ж����Զ����ط���
                    {
                        readHideKind = true;
                        newKindNode = treeView1.Nodes.Add("�Զ�����");
                    }

                    newKindNode.ImageIndex = 6;
                    newKindNode.SelectedImageIndex = 6;
                    if (s == string.Empty)
                    {
                        continue;
                    }

                    string[] modelIDs = s.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                    foreach (string t in modelIDs)
                    {
                        if (modelTable[t] != null)
                        {
                            newValueNode = modelTable[t] as TreeNode;
                            newKindNode.Nodes.Add(newValueNode);
                            nodeList.Add(newValueNode);
                        }
                    }
                }
            }

            switch(treeView1.Nodes.Count)
            {
                case 0: // �����ļ���û�е�ǰ���ģ����ʾ�Զ�����Ϣ
                    {
                        // ����Զ����ط���
                        newKindNode = treeView1.Nodes.Add("�Զ�����");
                        newKindNode.ImageIndex = 6;
                        newKindNode.SelectedImageIndex = 6;
                        // ����·���
                        newKindNode = treeView1.Nodes.Add("����1");
                        newKindNode.ImageIndex = 6;
                        newKindNode.SelectedImageIndex = 6;
                        foreach (object o in modelTable.Values)
                        {
                            newValueNode = o as TreeNode;
                            newKindNode.Nodes.Add(newValueNode);
                        }
                        break;
                    }
                case 1: // �����ļ���û��ģ����ʾ�Զ�����Ϣ
                    {
                        // ����·���
                        newKindNode = treeView1.Nodes.Add("����1");
                        newKindNode.ImageIndex = 6;
                        newKindNode.SelectedImageIndex = 6;
                        foreach(object o in modelTable.Values)
                        {
                            newValueNode = o as TreeNode;
                            newKindNode.Nodes.Add(newValueNode);
                        }
                        break;
                    }
                default: // �����ļ�������ģ����ʾ�Զ�����Ϣ���Ѿ���ȷ��ȡ              
                    {
                        newKindNode = treeView1.Nodes[treeView1.Nodes.Count - 1];
                        foreach (object o in modelTable.Values) // ����¼ӵ�ģ����û�зŵ���ʾ�б���
                        {
                            newValueNode = o as TreeNode;
                            if (!nodeList.Contains(newValueNode))
                            {
                                newKindNode.Nodes.Add(newValueNode);
                            }
                        }
                                               
                        break;
                    }
            }
        }

        /// <summary>
        /// ��ʼ��ͼ������
        /// </summary>
        private void InitImageList()
        {
            SqlConnection conn = new SqlConnection(Program.ConnetionString); // sql����
            List<TreeNode> nodeList = new List<TreeNode>(); // ���������
            treeView1.ImageList = imageList; // ͼ������
            Image newImage; // ��ʱͼ��

            foreach(Image image in mainForm.imageList2.Images)
            {
                imageList.Images.Add(image);
            }

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                // ��sys_modl_def��ȡ����ģ����Ϣ
                //DataTable tbl = Helper.GetDataTable("SELECT * FROM sys_modl_def", conn);

                DataTable tbl = Helper.GetDataTableProxy("sys_modl_def", conn);

                foreach (DataRow row in tbl.Rows)
                {
                    int iconid = Convert.ToInt32(row["iconid"]);
                    string id = row["id"].ToString();

                    if (iconid < imageList.Images.Count)
                    {
                        if (iconid == -1) // �Զ���ͼ��
                        {
                            TreeNode newNode = new TreeNode(row["modelname_en"].ToString());
                            newImage = GetImageFromDataBase(row);
                            imageList.Images.Add(newImage);
                            newNode.ImageIndex = imageList.Images.Count - 1;
                            newNode.SelectedImageIndex = imageList.Images.Count - 1;
                            newNode.Tag = id;
                            nodeList.Add(newNode);
                            modelTable[id] = newNode;
                        }
                        else
                        {
                            TreeNode newNode = new TreeNode(row["modelname_en"].ToString()); 
                            newNode.ImageIndex = iconid;
                            newNode.SelectedImageIndex = iconid;
                            newNode.Tag = id;
                            nodeList.Add(newNode);
                            modelTable[id] = newNode;
                        }
                    }
                }

            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڳ�ʼ��ͼ������ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }      
        }

        /// <summary>
        /// �����ݿ��ж�ȡͼ����Ϣ
        /// </summary>
        /// <param name="r">���ݿ���</param>
        /// <returns>ͼ��</returns>
        private Image GetImageFromDataBase(DataRow r)
        {
            Image image = null;
            byte[] imageData;
            try
            {
                imageData = (byte[])r["imagedata"];
                MemoryStream ms = new MemoryStream();
                ms.Write(imageData, 0, imageData.Length);
                image = Image.FromStream(ms);
                ms.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڶ�ȡ���ݿ�ͼ����Ϣʱ�����쳣: " + ex.ToString());
            }
            return image;
        }

        /// <summary>
        /// ��������
        /// </summary>
        private void SaveData()
        {
            List<ButtonItem> buttonItemList = new List<ButtonItem>(); // ����ButtonItem������
            ButtonItem buttonItem; // ��ʱButtonItem����
            string values = string.Empty; // �������Զ���ģ����ʾ����

            foreach(TreeNode kindNode in treeView1.Nodes)
            {
                if(kindNode.Text == "�Զ�����") // ���Զ����ؽ��
                {
                    string value = string.Empty;
                    foreach (TreeNode node in kindNode.Nodes)
                    {
                        value += string.Format("{0},", node.Tag);
                    }
                    values = values + value.TrimEnd(',') + ";";
                }
                else // �����Զ����ؽ��
                {
                    if(kindNode.Nodes.Count == 0) // û���ӽ��
                    {
                        continue;
                    }

                    string value = string.Empty;
                    foreach (TreeNode node in kindNode.Nodes)
                    {
                        value += string.Format("{0},", node.Tag);
                        if(mainForm.imageTable[node.Tag] != null)
                        {
                            buttonItem = mainForm.imageTable[node.Tag] as ButtonItem;
                            if(node == kindNode.Nodes[0]) // �Ǹ÷���ĵ�һ��ButtonItem��Ҫ�ӷ�����
                            {
                                buttonItem.BeginGroup = true;
                            }
                            else
                            {
                                buttonItem.BeginGroup = false;
                            }
                            buttonItemList.Add(buttonItem);
                        }
                    }
                    values = values + value.TrimEnd(',') + ";";             
                }
            }

            values = values.TrimEnd(';'); 

            SqlConnection conn = new SqlConnection(Program.ConnetionString); // sql����
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string hostName = System.Net.Dns.GetHostName();

                
                string sqlString = string.Format("SELECT host FROM {0} WHERE host = '{1}'", "sys_modl_display_cfg", hostName);
                /* remoting
                * DataTable table = Helper.GetDataTable(sqlString, conn);
                */

                string filter = string.Format("host = '{0}'", hostName);
                DataTable table = Helper.GetDataTableProxy("sys_modl_display_cfg", filter, null, conn);

                if(table.Rows.Count > 0) // ���û���¼�Ѿ����ڣ��޸�
                {
                    sqlString = string.Format("UPDATE {0} SET data = '{1}' WHERE host = '{2}'", "sys_modl_display_cfg", values, hostName);
                }
                else // ���û���¼δ���ڣ����
                {
                    sqlString = string.Format("INSERT INTO {0} (host, data) VALUES ('{1}', '{2}')", "sys_modl_display_cfg", hostName,
                        values);
                }
                /* remoting
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
                 */

                Helper.ExecuteCommand("sys_modl_display_cfg", sqlString, conn);

            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڱ����û��Զ���ģ����ʾ��Ϣʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            mainForm.ReloadButtonItems(buttonItemList); // ���¼���ButtonItem
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
            if (srcnode.Level == 0) // ��֧�ַ����϶�
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
                    foreach (TreeNode node in list1)
                    {
                        parentNode.Nodes.Add(node);
                    }

                    // ���·�������������Խ��
                    list1.Clear();
                    parentNode = desnode;
                    foreach (TreeNode node in parentNode.Nodes)
                    {
                        list1.Add(node);
                    }
                    parentNode.Nodes.Clear();
                    parentNode.Nodes.Add(srcnode);
                    foreach (TreeNode node in list1)
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
        /// �½�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            TreeNode newKindNode = treeView1.Nodes.Add("����" + treeView1.Nodes.Count);
            newKindNode.ImageIndex = 6;
            newKindNode.SelectedImageIndex = 6;
            treeView1.SelectedNode = newKindNode;
        }

        /// <summary>
        /// ȷ���Զ���ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            SaveData();
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// ȡ���Զ���ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
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
                if (Des_Node == null)
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

                if(OldDragOver != null)
                {
                    OldDragOver.BackColor = Color.Empty;
                }
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
                if(OldDragOver != null)
                {
                    OldDragOver.BackColor = Color.BlueViolet;
                }
            }

            if (tvhti.Location == TreeViewHitTestLocations.PlusMinus)
            {
                tvhti.Node.Expand();
            }
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
        /// �������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("�Ƿ�ȷ���������з���", "�������з���", 
                MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if(result == DialogResult.Yes) // �û�ѡ��ȷ��
            {
                TreeNode kindNode = null;
                treeView1.Nodes.Clear();
                kindNode = treeView1.Nodes.Add("�Զ�����");
                kindNode.ImageIndex = 6;
                kindNode.SelectedImageIndex = 6;
                kindNode = treeView1.Nodes.Add("����1");
                kindNode.ImageIndex = 6;
                kindNode.SelectedImageIndex = 6;
                foreach (object o in modelTable.Values)
                {
                    kindNode.Nodes.Add(o as TreeNode);
                }
            }
            
        }
    }
}