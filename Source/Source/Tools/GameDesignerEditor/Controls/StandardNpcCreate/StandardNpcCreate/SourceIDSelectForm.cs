using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Data.SqlClient;
using System.IO;

using DevComponents.AdvTree;

namespace StandardNpcCreate
{
    public partial class SourceIDSelectForm : Form
    {
        private SqlConnection conn; // sql����
        private DataTable modelTable; // ģ�ͱ�
        private DataTable skillTable; // ���ܱ�
        private string editorID; // EditorID
        private string sourceID; // SourceID
        private string npcForce; // npc����
        private string forceDetail; // ����ϸ��     
        private string colorChannel; // ��ɫͨ��

        private int lastSearchForceIndex = 0; // �ϴβ��ҵ�Force�������
        private int lastSearchForceDetailIndex = 0; // �ϴβ��ҵ�ForceDetail���ڵ�ı��
        private int lastRowIndex = 0; // �ϴβ��ҵ������еı��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="modelTable">ģ�ͱ�</param>
        /// <param name="skillTable">���ܱ�</param>
        public SourceIDSelectForm(SqlConnection conn, DataTable modelTable, DataTable skillTable)
        {
            InitializeComponent();
            this.conn = conn;
            this.modelTable = modelTable;
            this.skillTable = skillTable;            
            Init();
        }

        /// <summary>
        /// ��ɫͨ��
        /// </summary>
        public string ColorChannel
        {
            get
            {
                return colorChannel;
            }
        }

        /// <summary>
        /// SourceID
        /// </summary>
        public string SourceID
        {
            get
            {
                return sourceID;
            }
        }

        /// <summary>
        /// EditorID
        /// </summary>
        public string EditorID
        {
            get
            {
                return editorID;
            }
        }

        /// <summary>
        /// npc����
        /// </summary>
        public string NpcForce
        {
            get
            {
                return npcForce;
            }
        }

        /// <summary>
        /// ����ϸ��
        /// </summary>
        public string ForceDetail
        {
            get
            {
                return forceDetail;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ���������
            DataGridViewTextBoxColumn animationTypeColumn = new DataGridViewTextBoxColumn();
            animationTypeColumn.Name = "AnimationType";
            animationTypeColumn.HeaderText = "��������";

            DataGridViewTextBoxColumn editorIDColumn = new DataGridViewTextBoxColumn();
            editorIDColumn.Name = "EditorID";
            editorIDColumn.HeaderText = "NPC���";

            DataGridViewTextBoxColumn sourceIDColumn = new DataGridViewTextBoxColumn();
            sourceIDColumn.Name = "SourceID";
            sourceIDColumn.HeaderText = "ģ�ͱ��";

            DataGridViewTextBoxColumn colorChannelColumn = new DataGridViewTextBoxColumn();
            colorChannelColumn.Name = "ColorChannel";
            colorChannelColumn.HeaderText = "��ɫͨ��";

            editorView.Columns.Add(animationTypeColumn);
            editorView.Columns.Add(editorIDColumn);
            editorView.Columns.Add(sourceIDColumn);
            editorView.Columns.Add(colorChannelColumn);

            // ��ʼ��������
            string[] tempArray;
            string animationType = "�޷���";           
            char[] splitCharArray = new char[] { ';' };
            Hashtable forceTable = new Hashtable();
            Hashtable informationTable = new Hashtable();
            List<string[]> dataList;
            Node tempNode = null;
            Node leafNode = null;
            bool find = false;

            foreach(DataRow dataRow in modelTable.Rows)
            {                
                editorID = dataRow["EditorID"].ToString();
                sourceID = dataRow["SourceID"].ToString();
                npcForce = dataRow["Force"].ToString();
                forceDetail = dataRow["ForceDetail"].ToString();
                colorChannel = dataRow["ColorChannel"].ToString();                          

                // ����һ�����
                find = false;

                foreach(Node node in editorTree.Nodes)
                {
                    if (node.Text == npcForce)
                    {
                        tempNode = node;
                        find = true;
                        break;
                    }
                }

                if (!find)
                {
                    tempNode = new Node();
                    tempNode.Text = npcForce;
                    editorTree.Nodes.Add(tempNode);
                }
                
                // ���ڶ������                
                tempArray = forceDetail.Split(splitCharArray, StringSplitOptions.RemoveEmptyEntries);

                foreach(string s in tempArray)
                {
                    DataRow[] rows = skillTable.Select(string.Format("���� = '{0}' AND ���� = '{1}'", npcForce, s));
                    if (rows.Length > 0)
                    {
                        animationType = rows[0]["��������2"].ToString();
                    }
                    else
                    {
                        animationType = "�޷���";
                    }

                    find = false;

                    foreach(Node node in tempNode.Nodes)
                    {
                        if (node.Text == s)
                        {
                            leafNode = node;
                            find = true;
                            break;
                        }
                    }

                    if (!find)
                    {
                        leafNode = new Node();
                        leafNode.Text = s;
                        tempNode.Nodes.Add(leafNode);
                    }

                    if (leafNode.Tag == null)
                    {
                        dataList = new List<string[]>();
                        leafNode.Tag = dataList;
                    }
                    else
                    {
                        dataList = leafNode.Tag as List<string[]>;
                    }

                    dataList.Add(new string[] { animationType, editorID, sourceID, colorChannel });
                }
            }
        }

        /// <summary>
        /// ����ģ������
        /// </summary>
        /// <param name="currentEditorID">EditorID</param>
        /// <param name="currentColorChannel">��ɫͨ��</param>
        /// <param name="description">ģ������</param>
        private void UpdateModelDescription(string currentEditorID, string currentColorChannel, string description)
        {
            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                // �ȼ���Ƿ����и�����¼
                SqlCommand cmd = conn.CreateCommand();
                string sqlString = string.Format("SELECT Description FROM npc_model_record WHERE EditorID = '{0}' AND ColorChannel = '{1}'",
                                                 currentEditorID, currentColorChannel);
                cmd.CommandText = sqlString;
                object executeResult = cmd.ExecuteScalar();

                if (executeResult != null)
                {
                    sqlString = string.Format("UPDATE npc_model_record SET Description = '{0}' WHERE EditorID = '{1}' AND ColorChannel = '{2}'",
                                              description, currentEditorID, currentColorChannel);
                }
                else
                {
                    sqlString = string.Format("INSERT INTO npc_model_record (EditorID, ColorChannel, Description) VALUES ('{0}', '{1}', '{2}')",
                                              currentEditorID, currentColorChannel, description);
                }

                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڻ�ȡģ����������ʱ����sql�쳣��" + ex.Message, "��ȡģ������", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
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
        /// ��ȡģ������
        /// </summary>
        /// <param name="currentEditorID">EditorID</param>
        /// <param name="currentColorChannel">��ɫͨ��</param>
        /// <returns>ģ������</returns>
        private string GetModelDescription(string currentEditorID, string currentColorChannel)
        {
            string description = "";

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                SqlCommand cmd = conn.CreateCommand();
                string sqlString = string.Format("SELECT Description FROM npc_model_record WHERE EditorID = '{0}' AND ColorChannel = '{1}'",
                                                 currentEditorID, currentColorChannel);
                cmd.CommandText = sqlString;
                object executeResult = cmd.ExecuteScalar();

                if (executeResult != null)
                {
                    description = executeResult as string;
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڻ�ȡģ����������ʱ����sql�쳣��" + ex.Message, "��ȡģ������", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            return description;
        }

        /// <summary>
        /// ��ȡģ��ѡ��Ĵ���
        /// </summary>
        /// <param name="modelFile">ģ���ļ�</param>
        /// <param name="currentColorChannel">��ɫͨ��</param>
        /// <returns>ģ��ѡ��Ĵ���</returns>
        private int GetSelectedCount(string modelFile, string currentColorChannel)
        {
            int selectedCount = 0;

            if (!string.IsNullOrEmpty(modelFile))
            {
                try
                {
                    if (conn.State == ConnectionState.Closed)
                    {
                        conn.Open();
                    }

                    SqlCommand cmd = conn.CreateCommand();
                    string sqlString = null;
                    
                    if (currentColorChannel == "")
                    {
                        sqlString = string.Format("SELECT COUNT(RepresentID) FROM npc WHERE MainModelFile = '{0}' AND ColorChannel IS NULL",
                                                  modelFile);
                    }
                    else
                    {
                        sqlString = string.Format("SELECT COUNT(RepresentID) FROM npc WHERE MainModelFile = '{0}' AND ColorChannel = '{1}'",
                                                  modelFile, currentColorChannel);
                    }                                   
                    
                    cmd.CommandText = sqlString;
                    object executeResult = cmd.ExecuteScalar();

                    if (executeResult != null)
                    {
                        selectedCount = (int)executeResult;
                    }
                }
                catch (SqlException ex)
                {
                    MessageBox.Show("�ڻ�ȡģ��ʹ�ü�¼����ʱ����sql�쳣��" + ex.Message, "��ȡģ��ʹ�ü�¼", MessageBoxButtons.OK,
                                    MessageBoxIcon.Information);
                }
                finally
                {
                    if (conn.State == ConnectionState.Open)
                    {
                        conn.Close();
                    }
                }
            }            

            return selectedCount;
        }

        /// <summary>
        /// ��ȡģ�Ͷ�Ӧ��ͼƬ
        /// </summary>
        /// <param name="currentEditorID">EditorID</param>
        /// <returns>ģ�Ͷ�Ӧ��ͼƬ</returns>
        private Image GetModelImage(string currentEditorID)
        {
            Image image = null;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT Image FROM ModelImage WHERE EditorID = '{0}'", currentEditorID);
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                object executeResult = cmd.ExecuteScalar();

                if (executeResult != null)
                {
                    byte[] imageData = (byte[])executeResult;
                    MemoryStream ms = new MemoryStream();
                    ms.Write(imageData, 0, imageData.Length);
                    image = Image.FromStream(ms);
                    ms.Close();
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡģ��ͼƬʱ����sql�쳣��" + ex.Message, "ѡ��ģ��ͼƬ", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            // ͼƬ����
            if (image != null)
            {
                int desWidth = image.Width;
                int desHeight = image.Height;

                if (desWidth > previewLabel.Width)
                {
                    desWidth = previewLabel.Width;
                }

                if (desHeight > previewLabel.Height)
                {
                    desHeight = previewLabel.Height;
                }

                if (desWidth != image.Width || desHeight != image.Height)
                {
                    Bitmap bmp = new Bitmap(desWidth, desHeight);
                    Graphics g = Graphics.FromImage(bmp);
                    g.DrawImage(image, 0, 0, desWidth, desHeight);
                    image = bmp;
                }
            }

            return image;
        }

        /// <summary>
        /// ��ʾģ����Ϣ
        /// </summary>
        /// <param name="currentRow">��ǰ������</param>
        private void ShowModelInfo(DataGridViewRow currentRow)
        {
            string currentColorChannel = currentRow.Cells["ColorChannel"].Value.ToString();
            string currentSourceID = currentRow.Cells["SourceID"].Value.ToString();
            string currentEditorID = currentRow.Cells["EditorID"].Value.ToString();

            // ��ʾģ��ʹ�ô���
            int selectedCount = GetSelectedCount(currentSourceID, currentColorChannel);
            descriptionLabel.Text = string.Format("ģ��ʹ�ô�����{0}", selectedCount.ToString());

            // ��ʾģ��Ԥ��ͼƬ
            Image previewImage = GetModelImage(currentEditorID);
            previewLabel.Image = previewImage;

            // ��ʾģ������
            string modelDescription = GetModelDescription(currentEditorID, currentColorChannel);
            modelDescriptionBox.Text = modelDescription;

            if (modelDescription != "")
            {
                MessageBox.Show("��ģ����������;�����������ʹ�ã�", "ģ����Ϣ",
                                 MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
        
        /// <summary>
        /// ����EditorID
        /// </summary>
        /// <param name="searchString">��������</param>
        private void SearchEditorID(string searchString)
        {
            Node searchNode = null;
            bool canSearchAgain = false;
            int currentRowIndex = 0;

            if (lastSearchForceIndex != 0 || lastSearchForceDetailIndex != 0)
            {
                canSearchAgain = true;
            }

            for (int i = lastSearchForceIndex; i < editorTree.Nodes.Count; i++)
            {
                Node forceNode = editorTree.Nodes[i];

                for (int j = lastSearchForceDetailIndex; j < forceNode.Nodes.Count; j++)
                {
                    Node forceDetailNode = forceNode.Nodes[j];
                    List<string[]> dataList = forceDetailNode.Tag as List<string[]>;

                    for (int k = lastRowIndex; k < dataList.Count; k++)
                    {
                        string[] dataArray = dataList[k];

                        if (dataArray[1].Contains(searchString))
                        {
                            searchNode = forceDetailNode;
                            currentRowIndex = k;
                            lastRowIndex = k + 1;
                            lastSearchForceDetailIndex = j;
                            lastSearchForceIndex = i;

                            if (lastRowIndex == dataList.Count)
                            {
                                lastRowIndex = 0;
                                lastSearchForceDetailIndex++;

                                if (lastSearchForceDetailIndex == forceNode.Nodes.Count)
                                {
                                    lastSearchForceDetailIndex = 0;
                                    lastSearchForceIndex++;

                                    if (lastSearchForceIndex == editorTree.Nodes.Count)
                                    {
                                        lastSearchForceIndex = 0;
                                    }
                                }
                            }

                            break;
                        }
                    }

                    if (searchNode != null)
                    {
                        break;
                    }
                    else
                    {
                        lastRowIndex = 0;
                    }
                }

                if (searchNode != null)
                {
                    break;
                }
                else
                {
                    lastSearchForceDetailIndex = 0;
                }
            }

            if (searchNode != null)
            {
                editorTree.SelectedNode = searchNode;
                editorView.CurrentCell = editorView.Rows[currentRowIndex].Cells[0];
            }
            else
            {
                if (canSearchAgain)
                {
                    if (MessageBox.Show("û���ҵ�ƥ��ķ��࣡\r\n\r\n�Ѿ����ҵ�����ĩβ���Ƿ��ͷ��ʼ���ң�", "�������",
                                        MessageBoxButtons.OKCancel, MessageBoxIcon.Information) == DialogResult.OK)
                    {
                        lastRowIndex = 0;
                        lastSearchForceIndex = 0;
                        lastSearchForceDetailIndex = 0;
                        SearchForceDetail(searchString);
                    }
                }
                else
                {
                    MessageBox.Show("û���ҵ�ƥ��ķ��࣡", "�������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        /// <summary>
        /// ����ForceDetail
        /// </summary>
        /// <param name="searchString">��������</param>
        private void SearchForceDetail(string searchString)
        {
            Node searchNode = null;
            
            bool canSearchAgain = false;

            if (lastSearchForceIndex != 0 || lastSearchForceDetailIndex != 0 || lastRowIndex != 0)
            {
                canSearchAgain = true;
            }

            for (int i = lastSearchForceIndex; i < editorTree.Nodes.Count; i++)
            {
                Node forceNode = editorTree.Nodes[i];

                for (int j = lastSearchForceDetailIndex; j < forceNode.Nodes.Count; j++)
                {
                    Node forceDetailNode = forceNode.Nodes[j];

                    if (forceDetailNode.Text.Contains(searchString))
                    {
                        searchNode = forceDetailNode;
                        lastSearchForceDetailIndex = j + 1;
                        lastSearchForceIndex = i;

                        if (lastSearchForceDetailIndex == forceNode.Nodes.Count)
                        {
                            lastSearchForceDetailIndex = 0;
                            lastSearchForceIndex++;

                            if (lastSearchForceIndex == editorTree.Nodes.Count)
                            {
                                lastSearchForceIndex = 0;
                            }
                        }

                        break;
                    }                    
                }

                if (searchNode != null)
                {
                    break;
                }
                else
                {
                    lastSearchForceDetailIndex = 0;
                }
            }

            if (searchNode != null)
            {
                editorTree.SelectedNode = searchNode;                
            }
            else
            {
                if (canSearchAgain)
                {
                    if (MessageBox.Show("û���ҵ�ƥ��ķ��࣡\r\n\r\n�Ѿ����ҵ�����ĩβ���Ƿ��ͷ��ʼ���ң�", "�������", 
                                        MessageBoxButtons.OKCancel, MessageBoxIcon.Information) == DialogResult.OK)
                    {
                        lastSearchForceIndex = 0;
                        lastSearchForceDetailIndex = 0;
                        SearchForceDetail(searchString);
                    }
                }
                else
                {
                    MessageBox.Show("û���ҵ�ƥ��ķ��࣡", "�������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (editorView.SelectedCells.Count > 0)
            {
                DataGridViewCell currentCell = editorView.SelectedCells[0];

                if (currentCell.RowIndex >= 0)
                {
                    DataGridViewRow currentRow = currentCell.OwningRow;
                    npcForce = editorTree.SelectedNode.Parent.Text;
                    forceDetail = editorTree.SelectedNode.Text;
                    editorID = currentRow.Cells["EditorID"].Value.ToString();
                    sourceID = currentRow.Cells["SourceID"].Value.ToString();
                    colorChannel = currentRow.Cells["ColorChannel"].Value.ToString();                                

                    DialogResult = DialogResult.OK;
                    this.Close();
                }
                else
                {
                    MessageBox.Show("���Ƚ���ѡ��", "SourceIDѡ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("���Ƚ���ѡ��", "SourceIDѡ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }            
        }

        /// <summary>
        /// ѡ��ر�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// ѡ����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void advTree1_AfterNodeSelect(object sender, AdvTreeNodeEventArgs e)
        {
            Node currentNode = editorTree.SelectedNode;
            editorView.Rows.Clear();

            if (currentNode.Level == 1)
            {
                List<string[]> dataList = currentNode.Tag as List<string[]>;         
                DataGridViewRow firstRow = null;

                for (int i = 0; i < dataList.Count; i++)
                {
                    editorView.Rows.Add(1);
                    DataGridViewRow newRow = editorView.Rows[i];
                    newRow.Cells["AnimationType"].Value = dataList[i][0];
                    newRow.Cells["EditorID"].Value = dataList[i][1];
                    newRow.Cells["SourceID"].Value = dataList[i][2];
                    newRow.Cells["ColorChannel"].Value = dataList[i][3];

                    if (i == 0) // Ĭ����ʾ��һ�е���Ϣ
                    {
                        ShowModelInfo(newRow);
                    }
                }                                
            }
            else
            {
                // ���ģ��ͼƬ������
                previewLabel.Image = null;
                descriptionLabel.Text = "ģ��ʹ�ô���";
                modelDescriptionBox.Text = "";
            }
        }

        /// <summary>
        /// ѡ��SourceID
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void dataGridViewX1_CellMouseDown(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.RowIndex >= 0)
            {
                DataGridViewRow currentRow = editorView.Rows[e.RowIndex];

                ShowModelInfo(currentRow);
            }
        }

        /// <summary>
        /// ����ģ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            string description = modelDescriptionBox.Text;

            if (description != "")
            {
                if (editorView.SelectedCells.Count > 0)
                {
                    DataGridViewCell currentCell = editorView.SelectedCells[0];

                    if (currentCell.RowIndex >= 0)
                    {
                        DataGridViewRow currentRow = editorView.Rows[currentCell.RowIndex];
                        string currentColorChannel = currentRow.Cells["ColorChannel"].Value.ToString();
                        string currentEditorID = currentRow.Cells["EditorID"].Value.ToString();

                        UpdateModelDescription(currentEditorID, currentColorChannel, description);
                    }
                    else
                    {
                        MessageBox.Show("����ѡ��Ҫ����������ģ�����ͣ�", "����ģ������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
                else
                {
                    MessageBox.Show("����ѡ��Ҫ����������ģ�����ͣ�", "����ģ������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("ģ����������Ϊ�գ�", "����ģ������", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ���ҷ���ƥ�������������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            string searchString = searchConditionBox.Text;            

            if (searchConditionBox.Text != "")
            {
                if (forceDetailRadioButton.Checked) // ����ForceDetail
                {
                    SearchForceDetail(searchString);
                }
                else if (editorIDRadioButton.Checked) // ����EditorID
                {
                    SearchEditorID(searchString);
                }
                else
                {
                    MessageBox.Show("����ѡ��������ͣ�", "�������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("������������Ϊ�գ�", "�������", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ѡ��ForceDetail
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void forceDetailRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (forceDetailRadioButton.Checked)
            {
                editorIDRadioButton.Checked = false;
            }
        }

        /// <summary>
        /// ѡ��EditorID
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void editorIDRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            if (editorIDRadioButton.Checked)
            {
                forceDetailRadioButton.Checked = false;
            }
        }
    }
}