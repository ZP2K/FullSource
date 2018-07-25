using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace NpcClassify
{
    public partial class Form1 : Form
    {
        private int mode = 0; // ��ǰ���ģʽ
        private string result; // �û�ѡ��ķ��ؽ��
        private SqlConnection Conn; // sql����
        private string tblname; // �������
        private string catfieldname1; // ��һ�����ԵĲ���
        private string catfieldname2; // �ڶ������ԵĲ���
        private string oldValue; // ��ֵ����
        private string mapIndex; // ��ͼ���
        private string mapName; // ��ͼ����
        private string doodadKind; // С���������
        private string defaultValue; // Ĭ��ֵ
        private DataRow defaultRow; // Ĭ����
        private bool firstSelect = false; // �Ƿ��ǵ�һ��ѡ�������
        private List<string[]> rows = new List<string[]>(); // ��¼��ѯ���������
        private List<KeyValuePair<string, string>> list1 = new List<KeyValuePair<string, string>>(); // ���ɼ�ֵ������
        private List<KeyValuePair<string, string>> list2 = new List<KeyValuePair<string, string>>(); // С�����ֵ������
        private List<KeyValuePair<string, string>> list3 = new List<KeyValuePair<string, string>>(); // С���ע�ͼ�ֵ������
        private List<KeyValuePair<string, string>> list4 = new List<KeyValuePair<string, string>>(); // ��Ʒ��ֵ������1
        private List<KeyValuePair<string, string>> list5 = new List<KeyValuePair<string, string>>(); // ��Ʒ��ֵ������2

        /// <summary>
        /// ���캯��
        /// </summary>
        public Form1()
        {
            InitializeComponent(); // ��ʼ�����
        }

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="Conn">sql����</param>
        /// <param name="catfieldname1">����1</param>
        /// <param name="catfieldname2">����2</param>
        /// <param name="mode">ģʽ���</param>
        /// <param name="mapIndex">��ͼ���</param>
        /// <param name="oldValue">��ֵ</param>
        public Form1(SqlConnection Conn, string catfieldname1, string catfieldname2, int mode, string mapIndex, string oldValue)
        {
            this.Conn = Conn; // ��ȡsql����
            this.mode = mode; // ��ȡ��ǰ��ģʽ
            this.catfieldname1 = catfieldname1; // ��ȡ��һ�����ԵĲ���
            this.catfieldname2 = catfieldname2; // ��ȡ�ڶ������ԵĲ���
            this.mapIndex = mapIndex; // ��ȡ��ͼ���
            this.oldValue = oldValue; // ��ȡ��ֵ����
            InitializeComponent(); // ��ʼ�����
            switch (mode) // ��ʼ��Ĭ����
            {
                case 1: // ��ȡnpc��
                    {
                        string sqlString = string.Format("SELECT * FROM {0} WHERE ID = 0", "NpcTemplate");
                        DataTable table = GetDataTable(sqlString);
                        if (table.Rows.Count > 0)
                        {
                            defaultRow = table.Rows[0];
                        }

                        break;
                    }
                case 2: // ��ȡdoodad��
                    {
                        string sqlString = string.Format("SELECT * FROM {0} WHERE ID = 0", "tbl_doodad");
                        DataTable table = GetDataTable(sqlString);
                        if (table.Rows.Count > 0)
                        {
                            defaultRow = table.Rows[0];
                        }

                        break;
                    }
            }
            ChangeTabText(catfieldname1, catfieldname2); // �޸�Tab��ʾ���ı�ֵ
            FillMap(); // ���������ݼ�ֵ��
            FillTree(); // ��������
            ShowResult(); // ��ʾ�Ծ�ֵ�����Ĳ��ҽ��
        }

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="Conn">sql����</param>
        /// <param name="tblname">����</param>
        /// <param name="catfieldname1">����1</param>
        /// <param name="catfieldname2">����2</param>
        /// <param name="mode">ģʽ���</param>
        /// <param name="oldValue">��ֵ</param>
        public Form1(SqlConnection Conn, string tblname, string catfieldname1, string catfieldname2, int mode, string oldValue)
        {
            this.Conn = Conn; // ��ȡsql����
            this.tblname = tblname; // ��ȡ�������
            this.catfieldname1 = catfieldname1; // ��ȡ��һ�����ԵĲ���
            this.catfieldname2 = catfieldname2; // ��ȡ�ڶ������ԵĲ���
            this.mode = mode; // ��ȡ��ǰ��ģʽ
            this.oldValue = oldValue; // ��ȡ��ֵ����
            InitializeComponent(); // ��ʼ�����
            switch (mode) // ��ʼ��Ĭ����
            {
                case 1: // ��ȡnpc��
                    {
                        string sqlString = string.Format("SELECT * FROM {0} WHERE ID = 0", "NpcTemplate");
                        DataTable table = GetDataTable(sqlString);
                        if (table.Rows.Count > 0)
                        {
                            defaultRow = table.Rows[0];
                        }

                        break;
                    }
                case 2: // ��ȡdoodad��
                    {
                        string sqlString = string.Format("SELECT * FROM {0} WHERE ID = 0", "tbl_doodad");
                        DataTable table = GetDataTable(sqlString);
                        if (table.Rows.Count > 0)
                        {
                            defaultRow = table.Rows[0];
                        }

                        break;
                    }
            }
            ChangeTabText(catfieldname1, catfieldname2); // �޸�Tab��ʾ���ı�ֵ
            FillMap(); // ���������ݼ�ֵ��
            FillTree(tblname, catfieldname1, catfieldname2, treeView1.Nodes); // ��������
            FillTree(tblname, catfieldname2, catfieldname1, treeView2.Nodes);
            ShowResult(); // ��ʾ�Ծ�ֵ�����Ĳ��ҽ��
        }

        /// <summary>
        /// ��ȡ���ݱ�Ĭ��ֵ
        /// </summary>
        private void SetDefaultValue()
        {
            try
            {
                DataTable _tbl_ = GetDataTable("SELECT TOP 1 * FROM " + tblname);
                result = _tbl_.Rows[0][0].ToString().Trim();
                DialogResult = DialogResult.OK;
            }
            catch (Exception ex)
            {
                MessageBox.Show("��ȡ���ݱ�ʱ�����쳣: " + ex.ToString());
                DialogResult = DialogResult.No;
            }
            finally
            {
                this.Close(); // �رմ���
            }
        }

        /// <summary>
        /// ��ʾ�û�ѡ����ֶεĲ�ѯ���
        /// </summary>
        private void ShowResult()
        {
            switch (mode) // ѡ��ͬ�Ĳ�ѯģʽ
            {
                case 1:
                    {
                        // ��ѯ����
                        string sql = string.Format("SELECT MapName, ForceID, Name FROM {0} WHERE ID = \'{1}\'", tblname, oldValue);
                        SqlCommand cmd = null;
                        SqlDataReader reader = null;

                        string text1 = ""; // MapName
                        string text2 = ""; // ForceID
                        string text3 = ""; // Name

                        try
                        {
                            cmd = new SqlCommand(sql, Conn);
                            reader = cmd.ExecuteReader();
                          
                            if(!reader.HasRows) // δ��������
                            {
                                firstSelect = true;
                                return;
                            }

                            reader.Read(); // ��ȡ��ѯ���
                            text1 = reader[0].ToString().Trim(); // MapName
                            text2 = reader[1].ToString().Trim(); // ForceID
                            text3 = reader[2].ToString().Trim(); // Name
                        }
                        catch(SqlException ex)
                        {
                            MessageBox.Show("�ڶ�ȡ��ʾ��ֵʱ����sql�쳣: " + ex.ToString());
                            firstSelect = true;
                        }
                        finally
                        {
                            if (reader != null) // �ͷ�ϵͳ��Դ
                            {
                                reader.Close();
                            }
                        }

                        sql = string.Format("SELECT n.Name, n.ID, n.Title, m.middlemap FROM {0} n, middlemap_npc m WHERE n.id=m.id and n.MapName = \'{1}\' AND n.ForceID = \'{2}\' AND n.id=m.id AND n.MapName=m.map union all select n.name, n.id, n.title, null as middlemap from npctemplate n where n.MapName = \'{1}\' AND n.ForceID = \'{2}\' and n.id not in (select id from middlemap_npc)", tblname, text1, text2);
                        try
                        {
                            cmd = new SqlCommand(sql, Conn);
                            reader = cmd.ExecuteReader();

                            if (!reader.HasRows) // δ��������
                            {
                                firstSelect = true;
                                return;
                            }

                            while (reader.Read()) // ������ѯ���
                            {
                                rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim(), reader[2].ToString().Trim() });
                            }
                        }
                        catch(SqlException ex)
                        {
                            MessageBox.Show("�ڶ�ȡ��ʾ��ֵʱ����sql�쳣: " + ex.ToString());
                            firstSelect = true;
                        }
                        finally
                        {
                            if (reader != null) // �ͷ�ϵͳ��Դ
                            {
                                reader.Close();
                            }
                        }
                            
                        foreach (KeyValuePair<string, string> k in list1) // �������ɵļ�ֵ��
                        {
                            if (text2 == k.Key)
                            {
                                text2 = k.Value;
                                break;
                            }
                        }

                        dataGridView1.ColumnCount = 4;
                        dataGridView1.Columns[0].Name = "Name";
                        dataGridView1.Columns[1].Name = "ID";
                        dataGridView1.Columns[2].Name = "Title";
                        dataGridView1.Columns[3].Name = "middlemap";
                        dataGridView1.Rows.Clear(); // ���ÿ������

                        foreach (string[] rowArray in rows) // ����ѯ��������������б���
                        {
                            dataGridView1.Rows.Add(rowArray);
                        }

                        foreach (TreeNode node1 in treeView1.Nodes) // ������һ�������
                        {
                            if (text1 == node1.Text)
                            {
                                node1.Expand();
                                foreach (TreeNode node2 in node1.Nodes) // �����ڶ��������
                                {
                                    if (text2 == node2.Text)
                                    {
                                        treeView1.SelectedNode = node2;
                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        foreach (DataGridViewRow r in dataGridView1.Rows)
                        {
                            foreach (DataGridViewCell c in r.Cells)
                            {
                                if (c.Value != null && c.Value.ToString() == text3)
                                {
                                    dataGridView1.CurrentCell = c; // ѡ�в�ѯ���
                                    break;
                                }
                            }
                        }

                        if (rows.Count == 0) // ����ѯ����Ƿ�Ϊ��
                        {
                            firstSelect = true;
                        }
                        textBoxX2.Text = string.Format("MapName = {0}, ForceID = {1}", text1, text2); // ��ʾע��           
                        break;
                    }
                case 2:
                    {
                        // ��ѯ����
                        string sql = string.Format("SELECT MapName, Kind, Name FROM {0} WHERE ID = \'{1}\'", tblname, oldValue);
                        SqlCommand cmd = null;
                        SqlDataReader reader = null;

                        string text1 = ""; // MapName
                        string text2 = ""; // Kind
                        string text3 = ""; // Name

                        try
                        {
                            cmd = new SqlCommand(sql, Conn);
                            reader = cmd.ExecuteReader();                         

                            if (!reader.HasRows) // δ��������
                            {
                                firstSelect = true;
                                return;
                            }

                            reader.Read(); // ��ȡ��ѯ���
                            text1 = reader[0].ToString().Trim(); // MapName
                            text2 = reader[1].ToString().Trim(); // Kind
                            text3 = reader[2].ToString().Trim(); // Name
                        }
                        catch(SqlException ex)
                        {
                            MessageBox.Show("�ڶ�ȡ��ʾ��ֵ��Ϣʱ����sql�쳣: " + ex.ToString());
                            firstSelect = true;
                        }
                        finally
                        {
                            if (reader != null) // �ͷ�ϵͳ��Դ
                            {
                                reader.Close();
                            }
                        }
                            
                        try
                        {
                            // ��ѯ����
                            sql = string.Format("SELECT Name, ID FROM {0} WHERE MapName = \'{1}\' AND Kind = \'{2}\'", tblname, text1, text2);
                            cmd = new SqlCommand(sql, Conn);
                            reader = cmd.ExecuteReader();

                            if (!reader.HasRows) // δ��������
                            {
                                firstSelect = true;
                                return;
                            }

                            while (reader.Read()) // ������ѯ���
                            {
                                rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim() });
                            }
                        }
                        catch(SqlException ex)
                        {
                            MessageBox.Show("�ڶ�ȡ��ʾ��ֵ��Ϣʱ����sql�쳣: " + ex.ToString());
                            firstSelect = true;
                        }
                        finally
                        {
                            if (reader != null) // �ͷ�ϵͳ��Դ
                            {
                                reader.Close();
                            }
                        }
                            
                        foreach (KeyValuePair<string, string> k in list2) // ����С����ļ�ֵ��
                        {
                            if (text2 == k.Key)
                            {
                                text2 = k.Value;
                                break;
                            }
                        }

                        dataGridView1.ColumnCount = 2;
                        dataGridView1.Columns[0].Name = "Name";
                        dataGridView1.Columns[1].Name = "ID";
                        dataGridView1.Rows.Clear(); // ���ÿ������

                        foreach (string[] rowArray in rows) // ����ѯ��������������б���
                        {
                            dataGridView1.Rows.Add(rowArray);
                        }

                        foreach (TreeNode node1 in treeView1.Nodes) // ������һ�������
                        {
                            if (text1 == node1.Text)
                            {
                                node1.Expand();
                                foreach (TreeNode node2 in node1.Nodes) // �����ڶ��������
                                {
                                    if (text2 == node2.Text)
                                    {
                                        treeView1.SelectedNode = node2;
                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        foreach (DataGridViewRow r in dataGridView1.Rows)
                        {
                            foreach (DataGridViewCell c in r.Cells)
                            {
                                if (c.Value.ToString() == text3)
                                {
                                    dataGridView1.CurrentCell = c; // ѡ�в�ѯ���
                                    break;
                                }
                            }
                        }

                        if (rows.Count == 0) // ����ѯ����Ƿ�Ϊ��
                        {
                            firstSelect = true;
                        }
                        textBoxX2.Text = string.Format("MapName = {0}, Kind = {1}", text1, text2);
                        break;
                    }
                case 3:
                    {
                        string text1 = ""; // Type
                        string text2 = ""; // Genre
                        string text3 = ""; // _CATEGORY
                        string text4 = ""; // Name

                        int index = int.Parse(mapIndex); // ��ͼ�������
                        if ((index < 5) || (index > 8)) // �жϵ�ͼ��������Ƿ���Ч
                        {
                            mapIndex = "5"; // �Զ���Ϊ5
                                // MessageBox.Show("��ͼ���Ͳ�����Ч��\n����ΪOther.tab��");
                        }

                        foreach (KeyValuePair<string, string> k in list4) // ������Ʒ�ļ�ֵ��1
                        {
                            if (mapIndex == k.Key)
                            {
                                text1 = k.Value;
                                mapName = k.Value;
                                tblname = k.Value;
                                break;
                            }
                        }

                        // ��ѯ����
                        string sql = string.Format("SELECT Genre, _CATEGORY, Name FROM {0} WHERE ID = \'{1}\'", text1, oldValue);
                        SqlCommand cmd = null;
                        SqlDataReader reader = null;

                        try
                        {
                            cmd = new SqlCommand(sql, Conn);
                            reader = cmd.ExecuteReader();

                            if (!reader.HasRows) // δ��������
                            {
                                firstSelect = true;
                                return;
                            }

                            reader.Read(); // ��ȡ��ѯ���
                            text2 = reader[0].ToString().Trim(); // Genre
                            text3 = reader[1].ToString().Trim(); // _CATEGORY
                            text4 = reader[2].ToString().Trim(); // Name
                        }
                        catch(SqlException ex)
                        {
                            MessageBox.Show("�ڶ�ȡ��ʾ��ֵʱ����sql�쳣: " + ex.ToString());
                            firstSelect = true;
                        }
                        finally
                        {
                            if (reader != null) // �ͷ�ϵͳ��Դ
                            {
                                reader.Close();
                            }
                        }
                            
                        try
                        {
                            // ��ѯ����
                            sql = string.Format("SELECT Name, ID FROM {0} WHERE Genre = \'{1}\'", text1, text2);
                            cmd = new SqlCommand(sql, Conn);
                            reader = cmd.ExecuteReader();

                            if (!reader.HasRows) // δ��������
                            {
                                firstSelect = true;
                                return;
                            }

                            while (reader.Read()) // ������ѯ���
                            {
                                rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim(), mapIndex });
                            }
                        }
                        catch(SqlException ex)
                        {
                            MessageBox.Show("�ڶ�ȡ��ʾ��ֵʱ����sql�쳣: " + ex.ToString());
                            firstSelect = true;
                        }
                        finally
                        {
                            if (reader != null) // �ͷ�ϵͳ��Դ
                            {
                                reader.Close();
                            }
                        }      

                        foreach (KeyValuePair<string, string> k in list5) // ������Ʒ�ļ�ֵ��2
                        {
                            if (text2 == k.Key)
                            {
                                text2 = k.Value;
                                break;
                            }
                        }

                        dataGridView1.ColumnCount = 3;
                        dataGridView1.Columns[0].Name = "Name";
                        dataGridView1.Columns[1].Name = "ID";
                        dataGridView1.Columns[2].Name = "Type";
                        dataGridView1.Rows.Clear(); // ���ÿ������

                        foreach (string[] rowArray in rows) // ����ѯ��������������б���
                        {
                            dataGridView1.Rows.Add(rowArray);
                        }

                        foreach (TreeNode node1 in treeView1.Nodes) // ������һ�������
                        {
                            if (text1 == node1.Text)
                            {
                                node1.Expand();
                                foreach (TreeNode node2 in node1.Nodes) // �����ڶ��������
                                {
                                    if (text2 == node2.Text)
                                    {
                                        node2.Expand();
                                        foreach (TreeNode node3 in node2.Nodes) // ���������������
                                        {
                                            if (text3 == node3.Text)
                                            {
                                                treeView1.SelectedNode = node3;
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        foreach (DataGridViewRow r in dataGridView1.Rows)
                        {
                            foreach (DataGridViewCell c in r.Cells)
                            {
                                if (c.Value.ToString() == text4)
                                {
                                    dataGridView1.CurrentCell = c; // ѡ�в�ѯ���
                                    break;
                                }
                            }
                        }

                        if (rows.Count == 0) // ����ѯ����Ƿ�Ϊ��
                        {
                            firstSelect = true;
                        }
                        textBoxX2.Text = string.Format("Type = {0}, Genre = {1}, _CATEGORY = {2}", text1, text2, text3);
                        break;
                    }
                
            }
        }

        /// <summary>
        /// �޸�Tab��ʾ���ı�ֵ
        /// </summary>
        /// <param name="arg1">����1</param>
        /// <param name="arg2">����2</param>
        private void ChangeTabText(string arg1, string arg2)
        {
            tabItem1.Text = arg1;
            tabItem2.Text = arg2;
        }

        /// <summary>
        /// ���������ݼ�ֵ��
        /// </summary>
        private void FillMap()
        {
            switch (mode)
            {
                case 1: // ���RelationForce�ļ�ֵ��
                    {
                        FillRelationForceMap(); // ������ID�����Ƶļ�ֵ��
                        break;
                    }
                case 2: // ���DoodadTemplate�ļ�ֵ��
                    {
                        FillDoodadTemplateMap(); // ���С���Kind�����Ƶļ�ֵ��
                        FillDoodadTemplateExplation(); // ���С���ע�͵ļ�ֵ��
                        break;
                    }
                case 3: // ���Item�ļ�ֵ��
                    {
                        FillItemMap1(); // �����Ʒ��ֵ��1
                        FillItemMap2(); // �����Ʒ��ֵ��2
                        break;
                    }
            }
        }

        /// <summary>
        /// ������ID�����Ƶļ�ֵ��
        /// </summary>                             
        private void FillRelationForceMap()
        {
            string sql = string.Format("SELECT Name, ID FROM {0}", "dic_npc_relation_force");
            SqlCommand cmd;
            SqlDataReader reader = null;

            try // ���ܻ��׳�sql�쳣
            {
                cmd = new SqlCommand(sql, Conn);
                reader = cmd.ExecuteReader();
                while (reader.Read()) // ������ѯ���������ֵ��
                {
                    string text1 = reader[0].ToString().Trim(); // Name
                    string text2 = reader[1].ToString().Trim(); // ID
                    if (!(text1 == "") && !(text2 == ""))
                    {
                        list1.Add(new KeyValuePair<string, string>(text2, text1));
                    }
                }
            }
            catch (SqlException ex) // ��׽sql�쳣
            {
                MessageBox.Show("�����ɼ�ֵ��ʱ����sql�쳣: " + ex.ToString()); // ��ʾ�쳣����
            }
            finally
            {
                if (reader != null) // �ͷ�ϵͳ��Դ
                {
                    reader.Close();
                }
            }
        }

        /// <summary>
        /// ���С���Kind�����Ƶļ�ֵ��
        /// </summary>
        private void FillDoodadTemplateMap()
        {
            list2.Add(new KeyValuePair<string, string>("0", "dkInvalid"));
            list2.Add(new KeyValuePair<string, string>("1", "dkNormal"));
            list2.Add(new KeyValuePair<string, string>("2", "dkCorpse"));
            list2.Add(new KeyValuePair<string, string>("3", "dkQuest"));
            list2.Add(new KeyValuePair<string, string>("4", "dkRead"));
            list2.Add(new KeyValuePair<string, string>("5", "dkDialog"));
            list2.Add(new KeyValuePair<string, string>("6", "dkAcceptQuest"));
            list2.Add(new KeyValuePair<string, string>("7", "dkTreasure"));
            list2.Add(new KeyValuePair<string, string>("8", "dkOrnament"));
            list2.Add(new KeyValuePair<string, string>("9", "dkCraftTarget"));
            list2.Add(new KeyValuePair<string, string>("10", "dkClientOnly"));
        }

        /// <summary>
        /// ���С���ע�ͼ�ֵ������
        /// </summary>
        private void FillDoodadTemplateExplation()
        {
            list3.Add(new KeyValuePair<string, string>("dkInvalid", "��ҳ�ʼ"));
            list3.Add(new KeyValuePair<string, string>("dkNormal", "��ͨ, ���ɲ���"));
            list3.Add(new KeyValuePair<string, string>("dkCorpse", "ʬ��, ������"));
            list3.Add(new KeyValuePair<string, string>("dkQuest", "����, ֻ�н���ֱ����ص������ſɲ���"));
            list3.Add(new KeyValuePair<string, string>("dkRead", "�Ķ�, �ɲ���, �ɴ��ű�, �����ʾΪ\"�Ķ�\""));
            list3.Add(new KeyValuePair<string, string>("dkDialog", "�Ի�, �ɲ���, �ɴ��ű�, �����ʾΪ\"�Ի�\""));
            list3.Add(new KeyValuePair<string, string>("dkAcceptQuest", "�ӽ�����, �ɲ���, �ɴ��ű�, �����ʾΪ\"����\""));
            list3.Add(new KeyValuePair<string, string>("dkTreasure", "����, �ɲ���, �ɴ��ű�"));
            list3.Add(new KeyValuePair<string, string>("dkOrnament", "װ��, ���ɲ���"));
            list3.Add(new KeyValuePair<string, string>("dkCraftTarget", "�ɼ�, ����ܲɼ���, �ɲ���"));
            list3.Add(new KeyValuePair<string, string>("dkClientOnly", "�ͻ���, �ͻ���DOODAD, ���ɲ���"));
        }

        /// <summary>
        /// �����Ʒ��ֵ��1
        /// </summary>
        private void FillItemMap1()
        {
            list4.Add(new KeyValuePair<string, string>("5", "Other"));
            list4.Add(new KeyValuePair<string, string>("6", "item_Custom_Weapon"));
            list4.Add(new KeyValuePair<string, string>("7", "item_Custom_Armor"));
            list4.Add(new KeyValuePair<string, string>("8", "Custom_Trinket"));
        }

        /// <summary>
        /// �����Ʒ��ֵ��2
        /// </summary>
        private void FillItemMap2()
        {
            list5.Add(new KeyValuePair<string, string>("0", "δ֪"));
            list5.Add(new KeyValuePair<string, string>("1", "ҩƷ"));
            list5.Add(new KeyValuePair<string, string>("2", "����"));
            list5.Add(new KeyValuePair<string, string>("3", "����"));
        }

        /// <summary>
        /// ������ڵ�
        /// </summary>
        private void FillTree()
        {            
            foreach (KeyValuePair<string, string> k1 in list4) // ������Ʒ��ֵ��1
            {
                // �����һ�������
                TreeNode node1 = treeView1.Nodes.Add(k1.Value);
                TreeNode node2 = treeView2.Nodes.Add(k1.Value);

                // ����������������ķ���
                FillTree(k1.Value, catfieldname1, catfieldname2, node1.Nodes);
                FillTree(k1.Value, catfieldname2, catfieldname1, node2.Nodes);
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="tblname">����</param>
        /// <param name="catfieldname1">����1</param>
        /// <param name="catfieldname2">����2</param>
        /// <param name="tree">����㼯��</param>                                  
        private void FillTree(string tblname, string catfieldname1, string catfieldname2, TreeNodeCollection tree)
        {
            string sql = string.Format("SELECT {0}, {1} FROM {2}", catfieldname1, catfieldname2, tblname);
            SqlCommand cmd;
            SqlDataReader reader = null;
            string text1; // ��һ����ֵ
            string text2; // �ڶ�����ֵ
            bool exist1 = false; // ��һ���Ӧ��������Ƿ��Ѵ���
            bool exist2 = false; // �ڶ����Ӧ��������Ƿ��Ѵ���

            try
            {
                cmd = new SqlCommand(sql, Conn);
                reader = cmd.ExecuteReader();

                while (reader.Read()) // ��ȡ���ݿ��ѯ�������
                {
                    text1 = reader[0].ToString().Trim();
                    text2 = reader[1].ToString().Trim();

                    switch (mode) // ѡ������ļ�ֵ������
                    {
                        case 1:
                            {
                                if (catfieldname2 == "ForceID")
                                {
                                    foreach (KeyValuePair<string, string> k in list1) // ����ƥ���ֵ��
                                    {
                                        if (text2 == k.Key) // ���keyֵƥ�䣬����valueֵ���и�ֵ
                                        {
                                            text2 = k.Value;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    foreach (KeyValuePair<string, string> k in list1) // ����ƥ���ֵ��
                                    {
                                        if (text1 == k.Key) // ���keyֵƥ�䣬����valueֵ���и�ֵ
                                        {
                                            text1 = k.Value;
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                        case 2:
                            {
                                if (catfieldname2 == "Kind")
                                {
                                    foreach (KeyValuePair<string, string> k in list2) // ����ƥ���ֵ��
                                    {
                                        if (text2 == k.Key) // ���keyֵƥ�䣬����valueֵ���и�ֵ
                                        {
                                            text2 = k.Value;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    foreach (KeyValuePair<string, string> k in list2) // ����ƥ���ֵ��
                                    {
                                        if (text1 == k.Key) // ���keyֵƥ�䣬����valueֵ���и�ֵ
                                        {
                                            text1 = k.Value;
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                        case 3:
                            {
                                if (catfieldname1 == "Genre")
                                {
                                    foreach (KeyValuePair<string, string> k in list5) // ����ƥ���ֵ��
                                    {
                                        if (text1 == k.Key) // ���keyֵƥ�䣬����valueֵ���и�ֵ
                                        {
                                            text1 = k.Value;
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    foreach (KeyValuePair<string, string> k in list5) // ����ƥ���ֵ��
                                    {
                                        if (text2 == k.Key) // ���keyֵƥ�䣬����valueֵ���и�ֵ
                                        {
                                            text2 = k.Value;
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                    }

                    if (text1 != "") // ��ֵ��Ϊ��
                    {
                        foreach (TreeNode node1 in tree) // ����һ���Ӧ��������Ƿ��Ѵ���
                        {
                            if (node1.Text.Trim() == text1) // ��һ���Ӧ��������Ѿ�����
                            {
                                exist1 = true;
                                foreach (TreeNode node2 in node1.Nodes) // ���ڶ����Ӧ��������Ƿ��Ѵ���
                                {
                                    if (node2.Text.Trim() == text2) // �ڶ����Ӧ��������Ѿ�����
                                    {
                                        exist2 = true;
                                        break;
                                    }
                                }

                                if (!exist2 && (text2 != "")) // �ڶ����Ӧ������㲻���ڣ�����½��
                                {
                                    TreeNode newNode = node1.Nodes.Add(text2);
                                }

                                exist2 = false; // ����ֵ
                                break;
                            }
                        }

                        if (!exist1) // ��һ���Ӧ������㲻���ڣ�����½��
                        {
                            TreeNode newNode1 = tree.Add(text1);

                            // ��ӵڶ���Ľ��
                            if (text2 != "") // ��Ϊ�ս��
                            {
                                TreeNode newNode2 = newNode1.Nodes.Add(text2);
                            }
                        }

                        exist1 = false; // ����ֵ
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("���������ʱ��������sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (reader != null) // �ͷ�ϵͳ��Դ
                {
                    reader.Close();
                }
            }
        }

        /// <summary>
        /// ��ʾ�û�ѡ������������������Ϣ�������в�����Ϣ�İ汾
        /// </summary>
        /// <param name="tblname">����</param>
        /// <param name="arg1">����1</param>
        /// <param name="arg2">����2</param>
        /// <param name="arg3">����1��ֵ</param>
        /// <param name="arg4">����2��ֵ</param>   
        private void ShowData(string tblname, string arg1, string arg2, string arg3, string arg4)
        {
            dataGridView1.ColumnCount = 2;
            dataGridView1.Columns[0].Name = "Name";
            dataGridView1.Columns[1].Name = "ID";
            if (mode == 1) // ����npc����
            {
                dataGridView1.ColumnCount = 4;
                dataGridView1.Columns[0].Name = "Name";
                dataGridView1.Columns[1].Name = "ID";
                dataGridView1.Columns[2].Name = "Title";
                dataGridView1.Columns[3].Name = "middlemap";
            }
            dataGridView1.Rows.Clear(); // ���ÿ������

            // ��ѯ����
            string sql = string.Format("SELECT Name, ID FROM {0} WHERE {1} = '{3}' AND {2} = '{4}' ORDER BY Name",
                tblname, arg3, arg4, arg1, arg2);

            if(defaultRow != null && defaultRow[arg3].ToString() == arg1) // ��ȡĬ��������
            {
                sql = string.Format("SELECT Name, ID FROM {0} WHERE ({1} = '{3}' OR {1} IS NULL) AND {2} = '{4}' ORDER BY Name",
                    tblname, arg3, arg4, arg1, arg2);
            }

            if(defaultRow != null && defaultRow[arg4].ToString() == arg2) // ��ȡĬ��������
            {
                sql = string.Format("SELECT Name, ID FROM {0} WHERE {1} = '{3}' AND ({2} = '{4}' OR {2} IS NULL) ORDER BY Name",
                    tblname, arg3, arg4, arg1, arg2);
            }

            if (mode == 1) // ����npc����
            {
                sql = string.Format("SELECT n.Name, n.ID, n.Title, m.middlemap FROM {0} n, middlemap_npc m WHERE n.id=m.id and n.{1} = '{3}' AND n.{2} = '{4}' union all select n.name, n.id, n.title, null as middlemap from npctemplate n where n.{1} = '{3}' AND n.{2} = '{4}' and n.id not in (select id from middlemap_npc) ORDER BY n.Name",
                    tblname, arg3, arg4, arg1, arg2);
            }
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {
                cmd = new SqlCommand(sql, Conn);
                reader = cmd.ExecuteReader();
                rows.Clear(); // ��ղ�ѯ���������
                while (reader.Read()) // ��ȡ��ѯ������
                {
                    if (mode == 1) // ����npc����
                    {
                        rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim(), reader[2].ToString().Trim(), reader[3].ToString().Trim() });
                    }
                    else // ����С�������
                    {
                        rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim() });
                    }

                }
            }
            catch (SqlException ex) // ����sql�쳣
            {
                MessageBox.Show("��ѯѡ����Ϣʱ��������sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (reader != null) // �ͷ�ϵͳ��Դ
                {
                    reader.Close();
                }
            }

            foreach (string[] rowArray in rows) // ����ѯ��������������б���
            {
                dataGridView1.Rows.Add(rowArray);
            }
        }

        /// <summary>
        /// ��ʾ�û�ѡ������������������Ϣ��һ���в�����Ϣ�İ汾
        /// </summary>
        /// <param name="tblname">����</param>
        /// <param name="arg1">����1��ֵ</param>
        /// <param name="arg2">����1</param>
        private void ShowData(string tblname, string arg1, string arg2)
        {
            dataGridView1.ColumnCount = 2;
            dataGridView1.Columns[0].Name = "Name";
            dataGridView1.Columns[1].Name = "ID";
            if (mode == 1) // ����npc����
            {
                dataGridView1.ColumnCount = 4;
                dataGridView1.Columns[0].Name = "Name";
                dataGridView1.Columns[1].Name = "ID";
                dataGridView1.Columns[2].Name = "Title";
                dataGridView1.Columns[3].Name = "middlemap";
            }
            dataGridView1.Rows.Clear(); // ���ÿ������

            // ��ѯ����
            string sql = string.Format("SELECT Name, ID FROM {0} WHERE {2} = \'{1}\' ORDER BY Name",
                tblname, arg1, arg2);
            if (mode == 1) // ����npc����
            {
                sql = string.Format("SELECT n.Name, n.ID, n.Title, m.middlemap FROM {0} n, middlemap_npc m WHERE n.id=m.id and {2} = \'{1}\' union all select n.name, n.id, n.title, null as middlemap from npctemplate n WHERE {2} = \'{1}\' and n.id not in (select id from middlemap_npc) ORDER BY n.Name",
                    tblname, arg1, arg2);
            }
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {
                cmd = new SqlCommand(sql, Conn);
                reader = cmd.ExecuteReader();
                rows.Clear(); // ��ղ�ѯ���������
                while (reader.Read()) // ��ȡ��ѯ������
                {
                    if (mode == 1) // ����npc����
                    {
                        rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim(), reader[2].ToString().Trim(), reader[3].ToString().Trim() });
                    }
                    else // ����С�������
                    {
                        rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim() });
                    }
                }
            }
            catch (SqlException ex) // ����sql�쳣
            {
                MessageBox.Show("��ѯѡ����Ϣʱ��������sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (reader != null) // �ͷ�ϵͳ��Դ
                {
                    reader.Close();
                }
            }

            foreach (string[] rowArray in rows) // ����ѯ��������������б���
            {
                dataGridView1.Rows.Add(rowArray);
            }
        }

        /// <summary>
        /// ��ʾ�û�ѡ������������������Ϣ�����в�����Ϣ�İ汾
        /// </summary>
        /// <param name="tblname">����</param>
        /// <param name="arg1">��ͼ����</param>
        private void ShowSpecialData(string tblname, string arg1)
        {
            dataGridView1.ColumnCount = 3;
            dataGridView1.Columns[0].Name = "Name";
            dataGridView1.Columns[1].Name = "ID";
            dataGridView1.Columns[2].Name = "Type";
            dataGridView1.Rows.Clear(); // ���ÿ������

            // ��ѯ����
            string sql = string.Format("SELECT TOP 10000 Name, ID FROM {0} ORDER BY Name", tblname);
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {
                cmd = new SqlCommand(sql, Conn);
                reader = cmd.ExecuteReader();
                rows.Clear(); // ��ղ�ѯ���������
                //dataGridView1.Rows.Clear();
                while (reader.Read()) // ��ȡ��ѯ������
                {
                    rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim(), arg1 });
                }
            }
            catch (SqlException ex) // ����sql�쳣
            {
                MessageBox.Show("��ѯѡ����Ϣʱ��������sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (reader != null) // �ͷ�ϵͳ��Դ
                {
                    reader.Close();
                }
            }

            bool bShow = true;
            if (rows.Count > 1000 && checkBox.Checked)
            { 
                bShow = false;
            }
            if (bShow)
            {
                foreach (string[] rowArray in rows) // ����ѯ��������������б���
                {
                    dataGridView1.Rows.Add(rowArray);
                }
            }
        }

        /// <summary>
        /// ��ʾ�û�ѡ������������������Ϣ��һ��������Ϣ�İ汾
        /// </summary>
        /// <param name="tblname">����</param>
        /// <param name="arg1">����1</param>
        /// <param name="arg2">����1��ֵ</param>
        /// <param name="arg3">��ͼ����</param>
        private void ShowSpecialData(string tblname, string arg1, string arg2, string arg3)
        {
            dataGridView1.ColumnCount = 3;
            dataGridView1.Columns[0].Name = "Name";
            dataGridView1.Columns[1].Name = "ID";
            dataGridView1.Columns[2].Name = "Type";
            dataGridView1.Rows.Clear(); // ���ÿ������

            string sql = string.Format("SELECT TOP 10000 Name, ID FROM {0} WHERE {1} = '{2}' ORDER BY Name",
                                tblname, arg1, arg2);
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {
                cmd = new SqlCommand(sql, Conn);
                reader = cmd.ExecuteReader();
                rows.Clear(); // ��ղ�ѯ���������
                while (reader.Read()) // ��ȡ��ѯ������
                {
                    rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim(), arg3 });
                }
            }
            catch (SqlException ex) // ����sql�쳣
            {
                MessageBox.Show("��ѯѡ����Ϣʱ��������sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(reader != null) // �ͷ�ϵͳ��Դ
                {
                    reader.Close(); 
                }
            }

            foreach (string[] rowArray in rows) // ����ѯ��������������б���
            {
                dataGridView1.Rows.Add(rowArray);
            }
        }

        /// <summary>
        /// ��ʾ�û�ѡ������������������Ϣ������������Ϣ�İ汾
        /// </summary>
        /// <param name="tblname">����</param>
        /// <param name="arg1">�ֶ�1</param>
        /// <param name="arg2">�ֶ�1��ֵ</param>
        /// <param name="arg3">�ֶ�2</param>
        /// <param name="arg4">�ֶ�2��ֵ</param>
        /// <param name="arg5">��ͼ����</param>
        private void ShowSpecialData(string tblname, string arg1, string arg2, string arg3, string arg4, string arg5)
        {
            dataGridView1.ColumnCount = 3;
            dataGridView1.Columns[0].Name = "Name";
            dataGridView1.Columns[1].Name = "ID";
            dataGridView1.Columns[2].Name = "Type";
            dataGridView1.Rows.Clear(); // ���ÿ������

            string sql = string.Format("SELECT TOP 10000 Name, ID FROM {0} WHERE {1} = '{2}' AND {3} = '{4}' ORDER BY Name",
                                       tblname, arg1, arg2, arg3, arg4);
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {
                cmd = new SqlCommand(sql, Conn);
                reader = cmd.ExecuteReader();
                rows.Clear(); // ��ղ�ѯ���������
                while (reader.Read()) // ��ȡ��ѯ������
                {
                    rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim(), arg5 });
                }
            }
            catch (SqlException ex) // ����sql�쳣
            {
                MessageBox.Show("��ѯѡ����Ϣʱ��������sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (reader != null) // �ͷ�ϵͳ��Դ
                {
                    reader.Close();
                }
            }

            foreach (string[] rowArray in rows) // ����ѯ��������������б���
            {
                dataGridView1.Rows.Add(rowArray);
            }
        }

        /// <summary>
        /// ��ʾ�����ı�ע��
        /// </summary>
        /// <param name="text"></param>
        private void ShowExplaination(string text)
        {
            textBoxX2.Text = "�޸�����ע��"; // ��ʾԤ����Ϣ

            switch (mode)
            {
                case 1: // ��ʾNpcTemplate��ע��
                    {
                        break;
                    }
                case 2: // ��ʾDoodadTemplate��ע��
                    {
                        foreach (KeyValuePair<string, string> k in list3)
                        {
                            if (text == k.Key)
                            {
                                textBoxX2.Text = k.Value; // ��ʾע��ֵ
                                break;
                            }
                        }
                        break;
                    }
                case 3: // ��ʾItem��ע��
                    {
                        break;
                    }
            }
        }

        /// <summary>
        /// ����id�Ż�ȡ�������Ϣ
        /// </summary>
        /// <param name="tblname">����</param>
        /// <param name="catfieldname1">�ֶ���</param>
        /// <param name="id">id��</param>
        /// <returns>������Ϣ</returns>
        private string GetExtraData(string tblname, string catfieldname1, string id)
        {
            string data = "0";
            string sql = string.Format("SELECT {0} FROM {1} WHERE ID = {2}",
                             catfieldname1, tblname, id);
            SqlCommand cmd;
            SqlDataReader reader = null;
            try
            {
                cmd = new SqlCommand(sql, Conn);
                reader = cmd.ExecuteReader();
                reader.Read();
                data = reader[0].ToString().Trim();
            }
            catch (SqlException ex) // ����sql�쳣
            {
                MessageBox.Show("��ѯ������Ϣʱ��������sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (reader != null) // �ͷ�ϵͳ��Դ
                {
                    reader.Close();
                }
            }
            return data;
        }

        /// <summary>
        /// �û�ѡ��ֵ������
        /// </summary>
        public string Result
        {
            get
            {
                return result;
            }
        }

        /// <summary>
        /// ��ͼ��ŵ�����
        /// </summary>
        public string MapIndex
        {
            get
            {
                return mapIndex;
            }
        }

        /// <summary>
        /// ��ͼ���Ƶ�����
        /// </summary>
        public string MapName
        {
            get
            {
                return mapName;
            }
        }

        /// <summary>
        /// С������͵�����
        /// </summary>
        public string DoodadKind
        {
            get
            {
                return doodadKind;
            }
        }

        /// <summary>
        /// �û�ѡ��treeView1�ϵĽ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            try
            {
                if (firstSelect) // �Ƿ��ǵ�һ�ν���ѡ��
                {
                    TreeNode currentNode = (sender as TreeView).SelectedNode; // ��ǰѡ�еĽ��

                    switch (currentNode.Level)
                    {
                        case 0: // ѡ�е�һ�������
                            {
                                string text1 = currentNode.Text.Trim(); // ��һ������Ϣ

                                if (mode == 3) // �������ʾģʽ
                                {
                                    string text2 = "0"; // ��ͼ��Ӧ��Typeֵ

                                    foreach (KeyValuePair<string, string> k in list4) // ������Ʒ��ֵ��1
                                    {
                                        if (text1 == k.Value)
                                        {
                                            text2 = k.Key;
                                            break;
                                        }
                                    }
                                    ShowSpecialData(text1, text2);
                                }
                                else
                                {
                                    List<KeyValuePair<string, string>> l;

                                    if (mode == 1) // ����RelationForce�������
                                    {
                                        l = list1;
                                    }
                                    else // ����DoodadTemplate�������
                                    {
                                        l = list2;
                                    }

                                    ShowData(tblname, text1, catfieldname1); // ��ʾ�û�ѡ������������������Ϣ                               
                                }
                                break;
                            }
                        case 1: // ѡ�еڶ��������
                            {
                                string text1 = currentNode.Parent.Text.Trim(); // ��һ��ڵ���Ϣ
                                string text2 = currentNode.Text.Trim(); // �ڶ���ڵ���Ϣ

                                if (mode == 3) // �������ʾģʽ
                                {
                                    string text3 = "0"; // ��ͼ��Ӧ��Typeֵ

                                    foreach (KeyValuePair<string, string> k in list4) // ������Ʒ��ֵ��1
                                    {
                                        if (text1 == k.Value)
                                        {
                                            text3 = k.Key;
                                            break;
                                        }
                                    }

                                    foreach (KeyValuePair<string, string> k in list5) // ������Ʒ��ֵ��2
                                    {
                                        if (text2 == k.Value)
                                        {
                                            text2 = k.Key;
                                        }
                                    }

                                    ShowSpecialData(text1, catfieldname1, text2, text3); // ��ʾ�û�ѡ������������������Ϣ                             
                                }
                                else
                                {
                                    List<KeyValuePair<string, string>> l;

                                    if (mode == 1) // ����RelationForce�������
                                    {
                                        l = list1;
                                    }
                                    else // ����DoodadTemplate�������
                                    {
                                        l = list2;
                                    }

                                    foreach (KeyValuePair<string, string> k in l) // ����ƥ���ֵ��
                                    {
                                        if (text2 == k.Value) // ���valueֵƥ�䣬����keyֵ���и�ֵ
                                        {
                                            text2 = k.Key;
                                            break;
                                        }
                                    }
                                    ShowData(tblname, text1, text2, catfieldname1, catfieldname2); // ��ʾ�û�ѡ������������������Ϣ
                                }
                                break;
                            }
                        case 2: // ѡ�е����������
                            {
                                string text1 = currentNode.Parent.Parent.Text.Trim(); // ��һ��ڵ���Ϣ
                                string text2 = currentNode.Parent.Text.Trim(); // �ڶ���ڵ���Ϣ
                                string text3 = currentNode.Text.Trim(); // ����������Ϣ

                                if (mode == 3) // �������ʾģʽ
                                {
                                    string text4 = "0"; // ��ͼ��Ӧ��Typeֵ

                                    foreach (KeyValuePair<string, string> k in list4) // ������Ʒ��ֵ��1
                                    {
                                        if (text1 == k.Value)
                                        {
                                            text4 = k.Key;
                                            break;
                                        }
                                    }

                                    foreach (KeyValuePair<string, string> k in list5) // ������Ʒ��ֵ��2
                                    {
                                        if (text2 == k.Value)
                                        {
                                            text2 = k.Key;
                                        }
                                    }

                                    ShowSpecialData(text1, catfieldname1, text2, catfieldname2, text3, text4); // ��ʾ�û�ѡ������������������Ϣ
                                }
                                else
                                {
                                    List<KeyValuePair<string, string>> l;

                                    if (mode == 1) // ����RelationForce�������
                                    {
                                        l = list1;
                                    }
                                    else // ����DoodadTemplate�������
                                    {
                                        l = list2;
                                    }

                                    foreach (KeyValuePair<string, string> k in l) // ����ƥ���ֵ��
                                    {
                                        if (text2 == k.Value) // ���valueֵƥ�䣬����keyֵ���и�ֵ
                                        {
                                            text2 = k.Key;
                                            break;
                                        }
                                    }
                                    ShowData(tblname, text1, text2, catfieldname1, catfieldname2); // ��ʾ�û�ѡ������������������Ϣ
                                }
                                break;
                            }
                    }

                    ShowExplaination(currentNode.Text); // ��ʾѡ������ע��
                    textBoxX1.Text = "�ؼ��ֹ���..."; // �ָ��ؼ��ֲ�ѯ����������
                }
                else
                {
                    firstSelect = true;
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("��ȡ��ʾ�������Ϣʱ�����쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            DataGridViewCell cell = null; // �û�ѡ��ĵ�Ԫ

            if (dataGridView1.SelectedCells == null) // �û�δѡ��Ԫ
            {
                MessageBox.Show("���κ���Ч����!\n���򽫲������κ��޸���Ϣ!");
                DialogResult = DialogResult.No; // ����ȡ����Ϣ
                this.Close(); // �رմ���
                return;
            }

            try // ����û��Ƿ����ѡ��
            {
                cell = dataGridView1.SelectedCells[0]; // ��ȡ�û�ѡ��ĵ�Ԫ
            }
            catch (Exception ex) // �����û�δ��ѡ����쳣
            {
                MessageBox.Show("��Ч��ѡ��!\n���򽫲������κ��޸���Ϣ!");
                DialogResult = DialogResult.No;
                this.Close();
                return;
            }

            DataGridViewCell resultCell = dataGridView1[1, cell.RowIndex]; // �����ѯ�����Ϣ�ĵ�Ԫ
            result = resultCell.Value.ToString().Trim(); // ���÷���ֵ

            switch (mode)
            {
                case 1:
                    {
                        break;
                    }
                case 2: // ����С�������
                    {
                        doodadKind = GetExtraData(tblname, "Kind", result);
                        if((doodadKind == "") && (defaultRow != null) && !(defaultRow["Kind"] is DBNull))
                        {
                            doodadKind = defaultRow["Kind"].ToString();
                        }

                        break;
                    }
                case 3: // ���õ�ͼ����
                    {
                        mapIndex = dataGridView1[2, cell.RowIndex].Value.ToString().Trim(); // ���õ�ͼ����
                        foreach (KeyValuePair<string, string> k in list4)
                        {
                            if (mapIndex == k.Key)
                            {
                                mapName = k.Value; // ���õ�ͼ����
                                break;
                            }
                        }
                        break;
                    }
            }
            if (mode == 3) // �Ƿ���Ҫ���õ�ͼ����
            {

            }

            DialogResult = DialogResult.OK; // ����ȷ����Ϣ    
            this.Close(); // �رմ���
        }

        /// <summary>
        /// �û�ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No; // ����ȡ����Ϣ
            this.Close();
        }

        /// <summary>
        /// �û�ѡ�йؼ��ֲ�ѯ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void textBoxX1_MouseClick(object sender, MouseEventArgs e)
        {
            if ((sender as TextBox).Text == "�ؼ��ֹ���...")
            {
                textBoxX1.Text = ""; // ��������
            }
        }

        /// <summary>
        /// �ؼ��ֲ�ѯ���е����ݷ����仯
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void textBoxX1_TextChanged(object sender, EventArgs e)
        {
            string text = (sender as TextBox).Text; // ��ȡ�ı����е�����
            if (text != "" && text != "�ؼ��ֹ���...") // �ؼ��ֲ�Ϊ��
            {
                dataGridView1.Rows.Clear(); // ���ÿ������
                foreach (string[] s in rows) // ������ѯ���
                {
                    if (s[0].Contains(text) || s[1].Contains(text))
                    {
                        dataGridView1.Rows.Add(s);
                    }
                }
            }
            else // �ؼ���Ϊ�գ���ʾȫ����Ϣ
            {
                dataGridView1.Rows.Clear(); // ���ÿ������

                bool bShow = true;
                if (rows.Count > 1000 && checkBox.Checked)
                {
                    bShow = false;
                }
                if (bShow)
                {
                    foreach (string[] rowArray in rows) // ����ѯ��������������б���
                    {
                        dataGridView1.Rows.Add(rowArray);
                    }
                }

                //foreach (string[] rowArray in rows) // ����ѯ��������������б���
                //{
                //    //dataGridView1.DataSource = DataTable
                //    dataGridView1.Rows.Add(rowArray);
                //}
            }
        }

        /// <summary>
        /// �û�ѡ��ָ�Ĭ��ֵ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            SetDefaultValue(); // ��ȡ���ݱ�Ĭ��ֵ
        }

        /// <summary>
        /// �û�ѡ��treeView2�ϵĽ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void treeView2_AfterSelect(object sender, TreeViewEventArgs e)
        {
            TreeNode currentNode = (sender as TreeView).SelectedNode; // ��ǰѡ�еĽ��
            try
            {
                switch (currentNode.Level)
                {
                    case 0: // ѡ�е�һ�������
                        {
                            string text1 = currentNode.Text.Trim(); // ��һ������Ϣ

                            if (mode == 3) // �������ʾģʽ
                            {
                                string text2 = "0"; // ��ͼ��Ӧ��Typeֵ

                                foreach (KeyValuePair<string, string> k in list4) // ������Ʒ��ֵ��1
                                {
                                    if (text1 == k.Value)
                                    {
                                        text2 = k.Key;
                                        break;
                                    }
                                }
                                ShowSpecialData(text1, text2);
                            }
                            else
                            {
                                List<KeyValuePair<string, string>> l;

                                if (mode == 1) // ����RelationForce�������
                                {
                                    l = list1;
                                }
                                else // ����DoodadTemplate�������
                                {
                                    l = list2;
                                }

                                text1 = currentNode.Text.Trim();
                                foreach (KeyValuePair<string, string> k in l) // ����ƥ���ֵ��
                                {
                                    if (text1 == k.Value) // ���valueֵƥ�䣬����keyֵ���и�ֵ
                                    {
                                        text1 = k.Key;
                                        break;
                                    }
                                }

                                ShowData(tblname, text1, catfieldname2); // ��ʾ�û�ѡ������������������Ϣ
                            }
                            break;
                        }
                    case 1: // ѡ�еڶ��������
                        {
                            string text1 = currentNode.Parent.Text.Trim(); // ��һ��ڵ���Ϣ
                            string text2 = currentNode.Text.Trim(); // �ڶ���ڵ���Ϣ

                            if (mode == 3) // �������ʾģʽ
                            {
                                string text3 = "0"; // ��ͼ��Ӧ��Typeֵ

                                foreach (KeyValuePair<string, string> k in list4) // ������Ʒ��ֵ��1
                                {
                                    if (text1 == k.Value)
                                    {
                                        text3 = k.Key;
                                        break;
                                    }
                                }

                                ShowSpecialData(text1, catfieldname2, text2, text3); // ��ʾ�û�ѡ������������������Ϣ
                            }
                            else
                            {
                                List<KeyValuePair<string, string>> l;

                                if (mode == 1) // ����RelationForce�������
                                {
                                    l = list1;
                                }
                                else // ����DoodadTemplate�������
                                {
                                    l = list2;
                                }

                                text1 = currentNode.Text.Trim(); // ��ͼ��Ϣ
                                text2 = currentNode.Parent.Text.Trim(); // ������Ϣ

                                foreach (KeyValuePair<string, string> k in l) // ����ƥ���ֵ��
                                {
                                    if (text2 == k.Value) // ���valueֵƥ�䣬����keyֵ���и�ֵ
                                    {
                                        text2 = k.Key;
                                        break;
                                    }
                                }
                                ShowData(tblname, text1, text2, catfieldname1, catfieldname2); // ��ʾ�û�ѡ������������������Ϣ
                            }
                            break;
                        }
                    case 2: // ѡ�е����������
                        {
                            string text1 = currentNode.Parent.Parent.Text.Trim(); // ��һ��ڵ���Ϣ
                            string text2 = currentNode.Parent.Text.Trim(); // �ڶ���ڵ���Ϣ
                            string text3 = currentNode.Text.Trim(); // ����������Ϣ

                            if (mode == 3) // �������ʾģʽ
                            {
                                string text4 = "0"; // ��ͼ��Ӧ��Typeֵ

                                foreach (KeyValuePair<string, string> k in list4) // ������Ʒ��ֵ��1
                                {
                                    if (text1 == k.Value)
                                    {
                                        text4 = k.Key;
                                        break;
                                    }
                                }

                                text2 = currentNode.Text.Trim();
                                text3 = currentNode.Parent.Text.Trim();

                                foreach (KeyValuePair<string, string> k in list5) // ������Ʒ��ֵ��2
                                {
                                    if (text2 == k.Value)
                                    {
                                        text2 = k.Key;
                                    }
                                }

                                ShowSpecialData(text1, catfieldname1, text2, catfieldname2, text3, text4); // ��ʾ�û�ѡ������������������Ϣ
                            }
                            else
                            {
                                List<KeyValuePair<string, string>> l;

                                if (mode == 1) // ����RelationForce�������
                                {
                                    l = list1;
                                }
                                else // ����DoodadTemplate�������
                                {
                                    l = list2;
                                }

                                text1 = currentNode.Text.Trim(); // ��ͼ��Ϣ
                                text2 = currentNode.Parent.Text.Trim(); // ������Ϣ

                                foreach (KeyValuePair<string, string> k in l) // ����ƥ���ֵ��
                                {
                                    if (text2 == k.Value) // ���valueֵƥ�䣬����keyֵ���и�ֵ
                                    {
                                        text2 = k.Key;
                                        break;
                                    }
                                }

                                ShowData(tblname, text1, text2, catfieldname1, catfieldname2); // ��ʾ�û�ѡ������������������Ϣ
                            }
                            break;
                        }
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show("��ȡ��ʾ�������Ϣʱ�����쳣: " + ex.ToString());
            }

            ShowExplaination(currentNode.Text); // ��ʾѡ������ע��
            textBoxX1.Text = "�ؼ��ֹ���..."; // �ָ��ؼ��ֲ�ѯ����������
            textBoxX3.Text = "�ؼ��ֹ���...";
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sql">sql���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sql)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, Conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            System.Data.DataTable tbl = ds.Tables[0];
            return tbl;
        }

        /// <summary>
        /// �û�ѡ�йؼ��ֲ�ѯ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void textBoxX3_MouseClick(object sender, MouseEventArgs e)
        {
            if ((sender as TextBox).Text == "�ؼ��ֹ���...")
            {
                textBoxX3.Text = ""; // ��������
            }
        }

        /// <summary>
        /// �ؼ��ֲ�ѯ���е����ݷ����仯
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void textBoxX3_TextChanged(object sender, EventArgs e)
        {
            string text = (sender as TextBox).Text; // ��ȡ�ı����е�����
            if (text != "" && text != "�ؼ��ֹ���...") // �ؼ��ֲ�Ϊ��
            {
                if (dataGridView1.ColumnCount == 0)
                {
                    dataGridView1.ColumnCount = 4;
                    dataGridView1.Columns[0].Name = "Name";
                    dataGridView1.Columns[1].Name = "ID";
                    dataGridView1.Columns[2].Name = "Title";
                    dataGridView1.Columns[3].Name = "middlemap";
                }
                dataGridView1.Rows.Clear(); // ���ÿ������

                string sql = string.Empty;
                string mapcomparestring = string.Empty;
                if (treeView1.SelectedNode != null)
                {
                    TreeNode snode = treeView1.SelectedNode.Parent != null ? treeView1.SelectedNode.Parent : treeView1.SelectedNode;
                    if (snode != null)
                        mapcomparestring = string.Format("and n.mapname = \'{0}\' ", snode.Text);
                }

                sql = string.Format("SELECT n.Name, n.ID, n.Title, m.middlemap FROM {0} n,middlemap_npc m WHERE n.id = m.id and n.Title like \'%{1}%\' {2} union all select n.name, n.id, n.title, null as middlemap from npctemplate n where n.Title like \'%{1}%\' {2} and n.id not in (select id from middlemap_npc)", tblname, text, mapcomparestring);

                SqlCommand cmd = null;
                SqlDataReader reader = null;
                try
                {
                    cmd = new SqlCommand(sql, Conn);
                    reader = cmd.ExecuteReader();

                    if (!reader.HasRows) // δ��������
                    {
                        firstSelect = true;
                        return;
                    }

                    while (reader.Read()) // ������ѯ���
                    {
                        dataGridView1.Rows.Add(new string[] { reader[0].ToString().Trim(), reader[1].ToString().Trim(), reader[2].ToString().Trim(), reader[3].ToString().Trim() });
                    }
                }
                catch (SqlException ex)
                {
                    MessageBox.Show("�ڶ�ȡ��ʾ��ֵʱ����sql�쳣: " + ex.ToString());
                    firstSelect = true;
                }
                finally
                {
                    if (reader != null) // �ͷ�ϵͳ��Դ
                    {
                        reader.Close();
                    }
                }
            }
            else // �ؼ���Ϊ�գ���ʾȫ����Ϣ
            {
                dataGridView1.Rows.Clear(); // ���ÿ������
                foreach (string[] rowArray in rows) // ����ѯ��������������б���
                {
                    dataGridView1.Rows.Add(rowArray);
                }
            }
        }
    }
}