using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Data.SqlClient;
using System.Text.RegularExpressions;

namespace CustomLoadTree
{
    public partial class ComputeForm : Form
    {
        private TreeNode node; // ��ʼ����ĸ����
        private Hashtable table; // ��¼�����Ϣ���ݵ�hash��
        private SqlConnection conn; // sql����
        private List<KeyValuePair<string, string>> kindList = new List<KeyValuePair<string, string>>(); // �������ͼ�ֵ������
        private List<KeyValuePair<string, Logic.QuestType>> questTypeList = new List<KeyValuePair<string, Logic.QuestType>>(); // �������ͼ�ֵ�Ա�(ת����)
        private List<KeyValuePair<string, string>> mapList = new List<KeyValuePair<string, string>>(); // ��Ʒ��ͼ��ֵ������
        private List<float[]> virtualFlowSectionList = new List<float[]>(); // ʵ��������������
        private List<float[]> additionalFlowSectionList = new List<float[]>(); // ����������������
        private List<List<string>> coTaskList = new List<List<string>>(); // ͬ����������
        private List<string> resourceList = new List<string>(); // ��Դ����
        private List<string> publicResourceList = new List<string>(); // ������Դ����
        private List<int> fillableColumnList = new List<int>(); // ���Զ�������������
        private List<int> defaultColumnList = new List<int>(); // �ɻָ�Ĭ��ֵ�����������
        private List<int> simpleModeList = new List<int>(); // ����ģʽ�������ص����������
        private List<int> savableColumnList = new List<int>(); // �ɴ�����������
        private int npcKind = 0; // npc������
        private int npcReviveTime = 0; // npc��ˢ��ʱ��
        private int doodadReviveDelay = 0; // doodad��ˢ��ʱ��
        private int index = 0; // �����
        private Progress pForm = new Progress(); //����������
        private int nodeTotal = 0; // ���������
        private int nodeCount = 0; // ��ǰ�������
        private DataGridViewCell computeCell; // ���ڴ���ĵ�Ԫ��
        private string plusTable = "sys_QuestPlus"; // ���ӱ��
        private bool selectAll = true; // �Ƿ�ȫ��ѡ��
        private bool startEdit = false; // �Ƿ��ѿ�ʼ�༭
        private bool simpleMode = false; // �Ƿ��Ǿ���ģʽ
        private Color readOnlyColor = Color.LavenderBlush; // ֻ����Ԫ�����ɫ
        private Color savableColor = Color.LightYellow; // �ɴ浥Ԫ�����ɫ
        private SetArgument sa = new SetArgument(); // ���ò�����ֵ����
        private TaskData taskData = new TaskData(); // ������ֵ��
        private Hashtable finalMoneyTable = new Hashtable(); // ��Ǯ��ֵ��
        private Hashtable finalExpTable = new Hashtable(); // ������ֵ��
        private Hashtable npcIDTable = new Hashtable(); // NPC��ID��
        private Hashtable doodadIDTable = new Hashtable(); // Doodad��ID��
        private Hashtable publicResourceTable = new Hashtable(); // ������Դ��hash��
        private Hashtable coTaskTable = new Hashtable(); // ͬ�������hash��
        private Hashtable idTable = new Hashtable(); // ͬһid��hash��
        private Hashtable coTaskTime = new Hashtable(); // ͬ������ʱ���hash��
        private DataTable dataTable; // ���ݱ�
        private DataRow npcDefaultRow; // npc��Ĭ����
        private DataRow doodadDefaultRow; // doodad��Ĭ����
        private int totalNormalExp = 0; // ��׼�����ܺ�
        private int totalActualExp = 0; // ʵ�ʾ����ܺ�
        private Font boldFont; // ����
        private Font regularFont; // ����
        private bool fontChangable = false; // �Ƿ�ı�����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="node">��ʼ����ĸ����</param>
        /// <param name="list">��¼�����Ϣ���ݵ�����</param>
        public ComputeForm(SqlConnection conn, TreeNode node, Hashtable table)
        {
            this.conn = conn;
            this.node = node;
            this.table = table;
            InitializeComponent();
            InitTable();
            InitDataTable();
            FillKindList();
            FillMapList();
            FillFillableColumnList();
            FillDefaultColumnList();
            FillSimpleModeList();
            FillSavableColumnList();
            boldFont = new Font(dataGridView1.DefaultCellStyle.Font, FontStyle.Bold);
            regularFont = new Font(dataGridView1.DefaultCellStyle.Font, FontStyle.Regular);
            CountNodeTotal(node); // ����������
            nodeCount = 0;
            pForm.SetMaxValue(nodeTotal);
            pForm.Visible = true;
            InitData(node);           
            buttonItem1_Click(null, null);
            startEdit = true;
        }

        /// <summary>
        /// ��ʼ�����
        /// </summary>
        private void InitTable()
        {
            // ������
            DataGridViewCheckBoxColumn checkColumn = new DataGridViewCheckBoxColumn();
            checkColumn.HeaderText = "�Զ�����";
            checkColumn.Name = "�Զ�����";

            DataGridViewTextBoxColumn questNameColumn = new DataGridViewTextBoxColumn();
            questNameColumn.HeaderText = "��������";
            questNameColumn.Name = "��������";

            DataGridViewTextBoxColumn questTypeColumn = new DataGridViewTextBoxColumn();
            questTypeColumn.HeaderText = "��������";
            questTypeColumn.Name = "��������";

            DataGridViewTextBoxColumn virtualFlowColumn = new DataGridViewTextBoxColumn();
            virtualFlowColumn.HeaderText = "ʵ������";
            virtualFlowColumn.Name = "ʵ������";

            DataGridViewTextBoxColumn additionalFlowColumn = new DataGridViewTextBoxColumn();
            additionalFlowColumn.HeaderText = "��������";
            additionalFlowColumn.Name = "��������";

            DataGridViewTextBoxColumn singleExpColumn = new DataGridViewTextBoxColumn();
            singleExpColumn.HeaderText = "��λʱ�侭��(��)";
            singleExpColumn.Name = "��λʱ�侭��(��)";

            DataGridViewTextBoxColumn singleMoneyColumn = new DataGridViewTextBoxColumn();
            singleMoneyColumn.HeaderText = "��λʱ���Ǯ(��)";
            singleMoneyColumn.Name = "��λʱ���Ǯ(��)";

            DataGridViewTextBoxColumn levelColumn = new DataGridViewTextBoxColumn();
            levelColumn.HeaderText = "�ȼ�";
            levelColumn.Name = "�ȼ�";

            DataGridViewTextBoxColumn requireResourceColumn = new DataGridViewTextBoxColumn();
            requireResourceColumn.HeaderText = "������Դ";
            requireResourceColumn.Name = "������Դ";

            DataGridViewTextBoxColumn requireNumberColumn = new DataGridViewTextBoxColumn();
            requireNumberColumn.HeaderText = "��������";
            requireNumberColumn.Name = "��������";

            DataGridViewTextBoxColumn getRateColumn = new DataGridViewTextBoxColumn();
            getRateColumn.HeaderText = "��ü���";
            getRateColumn.Name = "��ü���";

            DataGridViewTextBoxColumn resourceNumberColumn = new DataGridViewTextBoxColumn();
            resourceNumberColumn.HeaderText = "��Դ����";
            resourceNumberColumn.Name = "��Դ����";

            DataGridViewTextBoxColumn refreshTimeColumn = new DataGridViewTextBoxColumn();
            refreshTimeColumn.HeaderText = "ˢ��ʱ��(��)";
            refreshTimeColumn.Name = "ˢ��ʱ��(��)";

            DataGridViewTextBoxColumn operateTimeColumn = new DataGridViewTextBoxColumn();
            operateTimeColumn.HeaderText = "����ʱ��(��)";
            operateTimeColumn.Name = "����ʱ��(��)";

            DataGridViewTextBoxColumn runTimeColumn = new DataGridViewTextBoxColumn();
            runTimeColumn.HeaderText = "��·ʱ��(��)";
            runTimeColumn.Name = "��·ʱ��(��)";

            DataGridViewTextBoxColumn publicResourcePlusColumn = new DataGridViewTextBoxColumn();
            publicResourcePlusColumn.HeaderText = "������Դ����";
            publicResourcePlusColumn.Name = "������Դ����";

            DataGridViewTextBoxColumn coTaskPlusColumn = new DataGridViewTextBoxColumn();
            coTaskPlusColumn.HeaderText = "ͬ���������";
            coTaskPlusColumn.Name = "ͬ���������";

            DataGridViewTextBoxColumn flowPlusColumn = new DataGridViewTextBoxColumn();
            flowPlusColumn.HeaderText = "��������";
            flowPlusColumn.Name = "��������";

            DataGridViewTextBoxColumn expPlusColumn = new DataGridViewTextBoxColumn();
            expPlusColumn.HeaderText = "�������";
            expPlusColumn.Name = "�������";

            DataGridViewTextBoxColumn singleTaskTimeColumn = new DataGridViewTextBoxColumn();
            singleTaskTimeColumn.HeaderText = "��������ʱ��(��)";
            singleTaskTimeColumn.Name = "��������ʱ��(��)";

            DataGridViewTextBoxColumn otherCoTaskTimeColumn = new DataGridViewTextBoxColumn();
            otherCoTaskTimeColumn.HeaderText = "ͬ����������ʱ��(��)";
            otherCoTaskTimeColumn.Name = "ͬ����������ʱ��(��)";

            DataGridViewTextBoxColumn allCoTaskTimeColumn = new DataGridViewTextBoxColumn();
            allCoTaskTimeColumn.HeaderText = "ͬ����������ʱ��(��)";
            allCoTaskTimeColumn.Name = "ͬ����������ʱ��(��)";

            DataGridViewTextBoxColumn pressRateColumn = new DataGridViewTextBoxColumn();
            pressRateColumn.HeaderText = "ѹ������";
            pressRateColumn.Name = "ѹ������";

            DataGridViewTextBoxColumn decPressRateColumn = new DataGridViewTextBoxColumn();
            decPressRateColumn.HeaderText = "��ѹ����";
            decPressRateColumn.Name = "��ѹ����";

            DataGridViewTextBoxColumn plusColumn = new DataGridViewTextBoxColumn();
            plusColumn.HeaderText = "�������";
            plusColumn.Name = "�������";

            DataGridViewTextBoxColumn normalTimeColumn = new DataGridViewTextBoxColumn();
            normalTimeColumn.HeaderText = "��׼ʱ��(��)";
            normalTimeColumn.Name = "��׼ʱ��(��)";

            DataGridViewTextBoxColumn normalExpColumn = new DataGridViewTextBoxColumn();
            normalExpColumn.HeaderText = "��׼����";
            normalExpColumn.Name = "��׼����";

            DataGridViewTextBoxColumn actualExpColumn = new DataGridViewTextBoxColumn();
            actualExpColumn.HeaderText = "ʵ�ʾ���";
            actualExpColumn.Name = "ʵ�ʾ���";

            DataGridViewTextBoxColumn reallocExpColumn = new DataGridViewTextBoxColumn();
            reallocExpColumn.HeaderText = "���·��侭��";
            reallocExpColumn.Name = "���·��侭��";

            DataGridViewTextBoxColumn finalExpColumn = new DataGridViewTextBoxColumn();
            finalExpColumn.HeaderText = "������ֵ";
            finalExpColumn.Name = "������ֵ";

            DataGridViewTextBoxColumn reallocMoneyColumn = new DataGridViewTextBoxColumn();
            reallocMoneyColumn.HeaderText = "���·����Ǯ";
            reallocMoneyColumn.Name = "���·����Ǯ";

            DataGridViewTextBoxColumn finalMoneyColumn = new DataGridViewTextBoxColumn();
            finalMoneyColumn.HeaderText = "��Ǯ��ֵ";
            finalMoneyColumn.Name = "��Ǯ��ֵ";

            // �����
            dataGridView1.Columns.Add(checkColumn);
            dataGridView1.Columns.Add(questNameColumn);
            dataGridView1.Columns.Add(questTypeColumn);
            dataGridView1.Columns.Add(virtualFlowColumn);
            dataGridView1.Columns.Add(additionalFlowColumn);
            dataGridView1.Columns.Add(singleExpColumn);
            dataGridView1.Columns.Add(singleMoneyColumn);
            dataGridView1.Columns.Add(levelColumn);
            dataGridView1.Columns.Add(requireResourceColumn);
            dataGridView1.Columns.Add(requireNumberColumn);
            dataGridView1.Columns.Add(getRateColumn);
            dataGridView1.Columns.Add(resourceNumberColumn);
            dataGridView1.Columns.Add(refreshTimeColumn);
            dataGridView1.Columns.Add(operateTimeColumn);
            dataGridView1.Columns.Add(runTimeColumn);
            dataGridView1.Columns.Add(publicResourcePlusColumn);
            dataGridView1.Columns.Add(coTaskPlusColumn);
            dataGridView1.Columns.Add(flowPlusColumn);
            dataGridView1.Columns.Add(expPlusColumn);
            dataGridView1.Columns.Add(singleTaskTimeColumn);
            dataGridView1.Columns.Add(otherCoTaskTimeColumn);
            dataGridView1.Columns.Add(allCoTaskTimeColumn);
            dataGridView1.Columns.Add(pressRateColumn);
            dataGridView1.Columns.Add(decPressRateColumn);
            dataGridView1.Columns.Add(plusColumn);
            dataGridView1.Columns.Add(normalTimeColumn);
            dataGridView1.Columns.Add(normalExpColumn);
            dataGridView1.Columns.Add(actualExpColumn);
            dataGridView1.Columns.Add(reallocExpColumn);
            dataGridView1.Columns.Add(finalExpColumn);
            dataGridView1.Columns.Add(reallocMoneyColumn);
            dataGridView1.Columns.Add(finalMoneyColumn);

            // �����е�ֻ������
            dataGridView1.Columns["��������"].ReadOnly = true; // ��������ֻ��
            dataGridView1.Columns["��������"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["��������"].ReadOnly = true; // ��������ֻ��
            dataGridView1.Columns["��������"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["ʵ������"].ReadOnly = true; // ʵ������ֻ��
            dataGridView1.Columns["ʵ������"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["��������"].ReadOnly = true; // ��������ֻ��
            dataGridView1.Columns["��������"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["��λʱ�侭��(��)"].ReadOnly = true; // ��λʱ�侭��ֻ��
            dataGridView1.Columns["��λʱ�侭��(��)"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["��λʱ���Ǯ(��)"].ReadOnly = true; // ��λʱ���Ǯֻ��
            dataGridView1.Columns["��λʱ���Ǯ(��)"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["������Դ"].ReadOnly = true; // ������Դֻ��
            dataGridView1.Columns["������Դ"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["������Դ����"].ReadOnly = true; // ������Դ����ֻ��
            dataGridView1.Columns["������Դ����"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["ͬ���������"].ReadOnly = true; // ͬ���������ֻ��
            dataGridView1.Columns["ͬ���������"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["��������ʱ��(��)"].ReadOnly = true; // ��������ʱ��ֻ��
            dataGridView1.Columns["��������ʱ��(��)"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["ͬ����������ʱ��(��)"].ReadOnly = true; // ͬ����������ʱ��ֻ��
            dataGridView1.Columns["ͬ����������ʱ��(��)"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["ͬ����������ʱ��(��)"].ReadOnly = true; // ͬ����������ʱ��ֻ��
            dataGridView1.Columns["ͬ����������ʱ��(��)"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["ѹ������"].ReadOnly = true; // ѹ������ֻ��
            dataGridView1.Columns["ѹ������"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["��ѹ����"].ReadOnly = true; // ��ѹ����ֻ��
            dataGridView1.Columns["��ѹ����"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["�������"].ReadOnly = true; // �������ֻ��  
            dataGridView1.Columns["�������"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["��׼ʱ��(��)"].ReadOnly = true; // ��׼ʱ��ֻ��
            dataGridView1.Columns["��׼ʱ��(��)"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["��׼����"].ReadOnly = true; // ��׼����ֻ��
            dataGridView1.Columns["��׼����"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["ʵ�ʾ���"].ReadOnly = true; // ʵ�ʾ���ֻ��
            dataGridView1.Columns["ʵ�ʾ���"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["���·��侭��"].ReadOnly = true; // ���·��侭��ֻ��
            dataGridView1.Columns["���·��侭��"].DefaultCellStyle.BackColor = readOnlyColor;
            dataGridView1.Columns["���·����Ǯ"].ReadOnly = true; // ���·����Ǯֻ��
            dataGridView1.Columns["���·����Ǯ"].DefaultCellStyle.BackColor = readOnlyColor;

            // �����е���ɫ
            dataGridView1.Columns["�ȼ�"].DefaultCellStyle.BackColor = savableColor; // �ȼ��ɴ�
            dataGridView1.Columns["��������"].DefaultCellStyle.BackColor = savableColor; // ���������ɴ�
            dataGridView1.Columns["��ü���"].DefaultCellStyle.BackColor = savableColor; // ��ü��ʿɴ�
            dataGridView1.Columns["��Դ����"].DefaultCellStyle.BackColor = savableColor; // ��Դ�����ɴ�
            dataGridView1.Columns["ˢ��ʱ��(��)"].DefaultCellStyle.BackColor = savableColor; // ˢ��ʱ��ɴ�
            dataGridView1.Columns["��·ʱ��(��)"].DefaultCellStyle.BackColor = savableColor; // ��·ʱ��ɴ�
            dataGridView1.Columns["��������"].DefaultCellStyle.BackColor = savableColor; // ��������ɴ�
            dataGridView1.Columns["�������"].DefaultCellStyle.BackColor = savableColor; // �������ɴ�
            dataGridView1.Columns["������ֵ"].DefaultCellStyle.BackColor = savableColor; // ������ֵ�ɴ�
            dataGridView1.Columns["��Ǯ��ֵ"].DefaultCellStyle.BackColor = savableColor; // ��Ǯ��ֵ�ɴ�
        }

        /// <summary>
        /// ��ʼ�����ݱ�
        /// </summary>
        private void InitDataTable()
        {
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }
                string sql = "SELECT * FROM tbl_quests";
                dataTable = Helper.GetDataTable(sql, conn);
                npcDefaultRow = GetNpcDefaultRow(); // ��ȡnpc��Ĭ����
                doodadDefaultRow = GetDoodadDefaultRow(); // ��ȡdoodad��Ĭ����
            }
            catch(SqlException ex)
            {
                MessageBox.Show("��ʼ�����ݱ�ʱ����sql�쳣: " + ex.ToString());
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
        /// ����������ͼ�ֵ������
        /// </summary>
        private void FillKindList()
        {
            kindList.Add(new KeyValuePair<string, string>("QUEST_NULL", "ǰ������"));
            kindList.Add(new KeyValuePair<string, string>("QUEST_SCRIPT", "�ű�����"));
            kindList.Add(new KeyValuePair<string, string>("QUEST_ITEM_MINE", "�ɼ�����"));
            kindList.Add(new KeyValuePair<string, string>("QUEST_ITEM_COLLECT", "�ռ�����"));
            kindList.Add(new KeyValuePair<string, string>("QUEST_KILL", "ɱ������"));
            kindList.Add(new KeyValuePair<string, string>("QUEST_ITEM_DELIVER", "��������"));
            kindList.Add(new KeyValuePair<string, string>("QUEST_DIALOG", "�Ի�����"));
        }

        /// <summary>
        /// �����Ʒ��ͼ��ֵ������
        /// </summary>
        private void FillMapList()
        {
            mapList.Add(new KeyValuePair<string, string>("5", "Other"));
            mapList.Add(new KeyValuePair<string, string>("6", "item_Custom_Weapon"));
            mapList.Add(new KeyValuePair<string, string>("7", "item_Custom_Armor"));
            mapList.Add(new KeyValuePair<string, string>("8", "Custom_Trinket"));
        }

        /// <summary>
        /// �����Զ�������������
        /// </summary>
        private void FillFillableColumnList()
        {
            fillableColumnList.Add(11); // ��Դ����
            fillableColumnList.Add(14); // ��·ʱ��
            fillableColumnList.Add(17); // ��������
            fillableColumnList.Add(18); // �������
        }

        /// <summary>
        /// �����Իָ�Ĭ��ֵ�����������
        /// </summary>
        private void FillDefaultColumnList()
        {
            defaultColumnList.Add(12); // ˢ��ʱ��
            defaultColumnList.Add(13); // ����ʱ��
            defaultColumnList.Add(17); // ��������
            defaultColumnList.Add(18); // �������
        }

        /// <summary>
        /// �������ھ���ģʽ�����ص����������
        /// </summary>
        private void FillSimpleModeList()
        {
            simpleModeList.Add(1);
            simpleModeList.Add(3);
            simpleModeList.Add(5);
            simpleModeList.Add(6);
            simpleModeList.Add(15);
            simpleModeList.Add(16);
            simpleModeList.Add(19);
            simpleModeList.Add(20);
            simpleModeList.Add(21);
            simpleModeList.Add(22);
            simpleModeList.Add(23);
            simpleModeList.Add(24);
            simpleModeList.Add(25);
            simpleModeList.Add(26);
            simpleModeList.Add(27);
            simpleModeList.Add(28);
            simpleModeList.Add(30);
        }

        /// <summary>
        /// �����Ա����е�����
        /// </summary>
        private void FillSavableColumnList()
        {
            savableColumnList.Add(7); // ����ȼ�
            savableColumnList.Add(9); // ��������
            savableColumnList.Add(10); // ��ü���
            savableColumnList.Add(12); // ˢ��ʱ��
            savableColumnList.Add(29); // ������ֵ
            savableColumnList.Add(31); // ��Ǯ��ֵ
        }

        /// <summary>
        /// �������������
        /// </summary>
        private void CountNodeTotal(TreeNode node)
        {
            nodeTotal++;
            foreach(TreeNode node1 in node.Nodes)
            {
                CountNodeTotal(node1);
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        /// <param name="node">����������</param>
        private void InitData(TreeNode node)
        {
            // ˢ�½�����
            nodeCount++;
            pForm.SetValue(nodeCount);
            if(conn.State == ConnectionState.Closed) // ��sql����
            {
                conn.Open();
            }
            AddData(node);
            if(node.Nodes.Count > 1) // �ӽ�����ͬ������
            {
                List<string> taskids = new List<string>();
                foreach(TreeNode node1 in node.Nodes)
                {
                    taskids.Add(((object[]) node1.Tag)[0].ToString());
                    coTaskTable[((object[]) node1.Tag)[0].ToString()] = taskids;
                }
                coTaskList.Add(taskids);
            }

            foreach (TreeNode node1 in node.Nodes)
            {                
                InitData(node1);
            }
            if (conn.State == ConnectionState.Open) // �ر�sql����
            {
                conn.Close();
            }
        }

        /// <summary>
        /// ��ȡnpc���Ĭ����
        /// </summary>
        /// <returns>npc���Ĭ����</returns>
        private DataRow GetNpcDefaultRow()
        {
            DataRow row = null;

            string sqlString = string.Format("SELECT * FROM {0} WHERE ID = 0", "NpcTemplate");
            DataTable table = Helper.GetDataTable(sqlString, conn);
            if(table.Rows.Count > 0) // �ҵ�Ĭ����
            {
                row = table.Rows[0];
            }

            return row;
        }

        /// <summary>
        /// ��ȡdoodad���Ĭ����
        /// </summary>
        /// <returns>doodad���Ĭ����</returns>
        private DataRow GetDoodadDefaultRow()
        {
            DataRow row = null;

            string sqlString = string.Format("SELECT * FROM {0} WHERE ID = 0", "tbl_doodad");
            DataTable table = Helper.GetDataTable(sqlString, conn);
            if (table.Rows.Count > 0) // �ҵ�Ĭ����
            {
                row = table.Rows[0];
            }

            return row;
        }

        /// <summary>
        /// ����������б���
        /// </summary>
        /// <param name="node"></param>
        private void AddData(TreeNode node)
        {
            try
            {
                int fatherid = 0;
                if(node.Parent != null)
                {
                    fatherid = int.Parse(((object[])node.Parent.Tag)[0].ToString());
                }
                int id = int.Parse(((object[])node.Tag)[0].ToString());
                if(id > 0 && (fatherid == -3 || fatherid == -4)) // �Ǿ�����
                {
                    return;
                }

                DataRow dr = dataTable.Rows.Find(id);
                if(dr == null)
                {
                    return;
                }

                if(id > 0) // ��ͨ���
                {
                    List<string> list= table[node] as List<string>;
                    dataGridView1.Rows.Add(1); // �������
                    virtualFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                    additionalFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                    // �������ID������
                    dataGridView1.Rows[index].Cells["��������"].Value = string.Format("({0}) {1}", list[0], list[1]);
                    dataGridView1.Rows[index].Cells["��������"].Tag = list[0];
                    // �����������
                    string kind = list[3];

                    string info = GetQuestPlusInfo(list[0]); // ������Ӹ�����Ϣ
                    List<List<string>> dataList = null; // ������Ϣ��
                    bool plus = false;
                    if(info != null && info != "")
                    {
                        dataList = GetPlusData(info);
                        plus = true;
                    }

                    foreach(KeyValuePair<string, string> k in kindList)
                    {
                        if(kind == k.Key)
                        {
                            kind = k.Value;
                            break;
                        }
                    }
                    dataGridView1.Rows[index].Cells["��������"].Style.BackColor = readOnlyColor;
                    dataGridView1.Rows[index].Cells["��������"].Value = kind;  
                    dataGridView1.Rows[index].HeaderCell.Value = string.Format("({0}) {1}, {2}", list[0], list[1], kind);
                    // ������ȼ�
                    dataGridView1.Rows[index].Cells["�ȼ�"].Style.BackColor = savableColor;
                    dataGridView1.Rows[index].Cells["�ȼ�"].Value = list[2];
                    
                    switch(kind)
                    {
                        case "ǰ������": // ǰ������
                            {
                                // ��ӻ�ü���
                                dataGridView1.Rows[index].Cells["��ü���"].Value = "����";
                                dataGridView1.Rows[index].Cells["��ü���"].ReadOnly = true; // ����Ϊֻ��
                                dataGridView1.Rows[index].Cells["��ü���"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ���ˢ��ʱ��
                                dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = "����";
                                dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].ReadOnly = true; // ����Ϊֻ��
                                dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ��Ӳ���ʱ��
                                dataGridView1.Rows[index].Cells["����ʱ��(��)"].Value = "����";
                                dataGridView1.Rows[index].Cells["����ʱ��(��)"].ReadOnly = true; // ����Ϊֻ��
                                dataGridView1.Rows[index].Cells["����ʱ��(��)"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ���������Դ
                                dataGridView1.Rows[index].Cells["������Դ"].Value = "����";
                                // �����������
                                dataGridView1.Rows[index].Cells["��������"].Value = "����";
                                dataGridView1.Rows[index].Cells["��������"].ReadOnly = true; // ����Ϊֻ��
                                dataGridView1.Rows[index].Cells["��������"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ��ӵ�λʱ�侭��
                                dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Value = "����";
                                dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ��ӵ�λʱ���Ǯ
                                dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Value = "����";
                                dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // �����Դ����
                                dataGridView1.Rows[index].Cells["��Դ����"].Value = "����";
                                dataGridView1.Rows[index].Cells["��Դ����"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // �����·ʱ��
                                dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Value = "����";
                                dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // �����������
                                dataGridView1.Rows[index].Cells["��������"].Value = "����";
                                dataGridView1.Rows[index].Cells["��������"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ��Ӿ������
                                dataGridView1.Rows[index].Cells["�������"].Value = "����";
                                dataGridView1.Rows[index].Cells["�������"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ��ӱ�׼ʱ��
                                dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Value = "����";
                                dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ

                                // ��¼��ID�������к�
                                List<int> indexList = new List<int>();
                                indexList.Add(index);
                                idTable[id.ToString()] = indexList;

                                break;
                            }
                        case "�ű�����": // �ű�����
                            {
                                int requireResourceCount = 0; // ������Դ����
                                for (int i = 1; i <= 8; i++) // ����������Դ
                                {
                                    if (dr["QuestValueStr" + i].ToString().Trim() != "" && dr["QuestValueStr" + i].ToString().Trim() != "0") 
                                    {
                                        if (requireResourceCount > 0)
                                        {
                                            dataGridView1.Rows.Add(1); // �������
                                            virtualFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            additionalFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            index++;
                                            // �������ID������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("({0}) {1}", list[0], list[1]);
                                            dataGridView1.Rows[index].Cells["��������"].Tag = list[0];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = kind;
                                            dataGridView1.Rows[index].HeaderCell.Value = string.Format("({0}) {1}, {2}", list[0], list[1], kind);
                                            // ������ȼ�
                                            dataGridView1.Rows[index].Cells["�ȼ�"].Value = list[2];
                                        }
                                        
                                        // ��ӻ�ü���
                                        dataGridView1.Rows[index].Cells["��ü���"].Style.BackColor = Color.White;
                                        dataGridView1.Rows[index].Cells["��ü���"].Value = "100%";
                                        dataGridView1.Rows[index].Cells["��ü���"].Tag = "100%";
                                        // ���ˢ��ʱ��
                                        dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = "1";
                                        dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "1";
                                        dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Style.BackColor = Color.White;
                                        // ��Ӳ���ʱ��
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Value = "2";
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Tag = "2";
                                        // ���������Դ
                                        string resource = dr["QuestValueStr" + i].ToString();
                                        dataGridView1.Rows[index].Cells["������Դ"].Value = resource;
                                        dataGridView1.Rows[index].Cells["������Դ"].Tag = resource;
                                        if(resourceList.Contains(resource)) // �Ƿ���δ��¼�Ĺ�����Դ
                                        {
                                            if(!publicResourceList.Contains(resource))
                                            {
                                                publicResourceList.Add(resource);
                                            }
                                            List<int> rowIndexList = publicResourceTable[resource] as List<int>;
                                            rowIndexList.Add(index);
                                        }
                                        else
                                        {
                                            resourceList.Add(resource);
                                            List<int> rowIndexList = new List<int>();
                                            rowIndexList.Add(index);
                                            publicResourceTable[resource] = rowIndexList;
                                        }
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = dr["QuestValue" + i].ToString();
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "QuestValue" + i;
                                        // ��ӵ�λʱ�侭��
                                        int level = int.Parse(list[2]);
                                        int exp = 0;
                                        if(level < taskData.ExpArray.Length)
                                        {
                                            exp = (int)(taskData.ExpArray[level] * 1.15);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Value = exp.ToString();
                                        // ��ӵ�λʱ���Ǯ
                                        int money = 0;
                                        if(level < taskData.MoneyArray.Length)
                                        {
                                            money = (int)(taskData.MoneyArray[level] * 1.15);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Value = money.ToString();
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Tag = money.ToString();
                                        // ��ӱ�׼ʱ��
                                        int normalTime = 0;
                                        if(level < taskData.NormalTimeArray3.Length)
                                        {
                                            normalTime = taskData.NormalTimeArray3[level];
                                        }
                                        dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Value = normalTime.ToString();
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "1";
                                        // ��Ӿ������
                                        dataGridView1.Rows[index].Cells["�������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["�������"].Tag = "1";
                                        
                                        if(plus) // ��Ӹ�����Ϣ
                                        {
                                            virtualFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][0]);
                                            virtualFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][1]);
                                            additionalFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][2]);
                                            additionalFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][3]);
                                            // ���ʵ����������
                                            dataGridView1.Rows[index].Cells["ʵ������"].Value = string.Format("[{0}, {1}]",
                                                virtualFlowSectionList[index][1].ToString(), virtualFlowSectionList[index][0].ToString());
                                            // ��Ӽ�����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("[{0}, {1}]",
                                                additionalFlowSectionList[index][1].ToString(), additionalFlowSectionList[index][0].ToString());
                                            // �����Դ����
                                            dataGridView1.Rows[index].Cells["��Դ����"].Value = dataList[requireResourceCount][4];
                                            // �����·ʱ��
                                            dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Value = dataList[requireResourceCount][5];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = dataList[requireResourceCount][6];
                                            // ��Ӿ������
                                            dataGridView1.Rows[index].Cells["�������"].Value = dataList[requireResourceCount][7];
                                        }

                                        // ��¼��ID�������к�
                                        if(idTable[id.ToString()] == null)
                                        {
                                            List<int> indexList = new List<int>();
                                            indexList.Add(index);
                                            idTable[id.ToString()] = indexList;
                                        }
                                        else
                                        {
                                            List<int> indexList = idTable[id.ToString()] as List<int>;
                                            indexList.Add(index);
                                        }
                                        
                                        requireResourceCount++;
                                    }
                                }
                                break;
                            }
                        case "�ɼ�����": // �ɼ�����
                            {
                                int requireResourceCount = 0; // ������Դ����
                                for (int i = 1; i <= 4; i++)
                                {
                                    if (dr["DropDoodadTemplateID" + i].ToString().Trim() != "" &&  
                                        dr["DropDoodadTemplateID" + i].ToString().Trim() != "0") // �ɼ������ж�
                                    {
                                        if(requireResourceCount > 0)
                                        {
                                            dataGridView1.Rows.Add(1); // �������
                                            virtualFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            additionalFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            index++;
                                            // �������ID������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("({0}) {1}", list[0], list[1]);
                                            dataGridView1.Rows[index].Cells["��������"].Tag = list[0];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = kind;
                                            dataGridView1.Rows[index].HeaderCell.Value = string.Format("({0}) {1}, {2}", list[0], list[1], kind);
                                            // ������ȼ�
                                            dataGridView1.Rows[index].Cells["�ȼ�"].Value = list[2];
                                        }

                                        // ��ӻ�ü���
                                        string tmpStr = dr["DropItemRate" + i].ToString();
                                        if (tmpStr == null || tmpStr == "")
                                        {
                                            dataGridView1.Rows[index].Cells["��ü���"].Value = "100%";
                                        }
                                        else
                                        {
                                            float number = float.Parse(tmpStr);
                                            float rate = number * 100 / 1048576;
                                            dataGridView1.Rows[index].Cells["��ü���"].Value = string.Format("{0:f1}%", rate);
                                        }
                                        dataGridView1.Rows[index].Cells["��ü���"].Tag = "DropItemRate" + i;
                                        // ���������Դ
                                        string itemid = dr["EndRequireItemIndex" + i].ToString();
                                        string mapid = dr["EndRequireItemType" + i].ToString();
                                        if(itemid == "" || itemid == "0" || mapid == "" || mapid == "0") // item���������Ч
                                        {
                                            MessageBox.Show(string.Format("��{0}��������Ʒ���ò�ƥ��!", list[0]));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index); 
                                            index--;
                                            continue;
                                        }
                                        string itemName = GetItemName(itemid, mapid);
                                        if(itemName == null)
                                        {
                                            MessageBox.Show(string.Format("��{0}�������IDΪ{1}����Ʒ������!", list[0], itemid));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string doodadName = GetDoodadName(dr["DropDoodadTemplateID" + i].ToString());
                                        if (doodadName == null)
                                        {
                                            MessageBox.Show(string.Format("��{0}�������IDΪ{1}��С���������!", list[0], dr["DropDoodadTemplateID" + i].ToString()));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string resource = string.Format("{0} ({1})", itemName, doodadName);
                                        dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Green;
                                        dataGridView1.Rows[index].Cells["������Դ"].Value = resource;
                                        dataGridView1.Rows[index].Cells["������Դ"].Tag = doodadName;
                                        doodadIDTable[index.ToString()] = dr["DropDoodadTemplateID" + i].ToString(); // ��¼doodad��id
                                        if (resourceList.Contains(doodadName)) // �Ƿ���δ��¼�Ĺ�����Դ
                                        {
                                            if (!publicResourceList.Contains(doodadName))
                                            {
                                                publicResourceList.Add(doodadName);
                                            }
                                            List<int> rowIndexList = publicResourceTable[doodadName] as List<int>;
                                            rowIndexList.Add(index);
                                        }
                                        else
                                        {
                                            resourceList.Add(doodadName);
                                            List<int> rowIndexList = new List<int>();
                                            rowIndexList.Add(index);
                                            publicResourceTable[doodadName] = rowIndexList;
                                        }
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = dr["EndRequireItemAmount" + i].ToString();
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "EndRequireItemAmount" + i;
                                        // ���ˢ��ʱ��
                                        dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = doodadReviveDelay.ToString();
                                        dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "1";
                                        // ��Ӳ���ʱ��
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Value = "5";
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Tag = "5";
                                        // ��ӵ�λʱ�侭��
                                        int level = int.Parse(list[2]);
                                        int exp = 0;
                                        if (level < taskData.ExpArray.Length)
                                        {
                                            exp = (int)(taskData.ExpArray[level] * 1.15);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Value = exp.ToString();
                                        // ��ӵ�λʱ���Ǯ
                                        int money = 0;
                                        if (level < taskData.MoneyArray.Length)
                                        {
                                            money = (int)(taskData.MoneyArray[level] * 1.15);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Value = money.ToString();
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Tag = money.ToString();
                                        // �����Ԫ����ɫ
                                        int requireItemAmount = 0;
                                        if (dr["EndRequireItemAmount" + i] != null && dr["EndRequireItemAmount" + i].ToString() != "")
                                        {
                                            requireItemAmount = int.Parse(dr["EndRequireItemAmount" + i].ToString());
                                        }
                                        if (requireItemAmount > 10)
                                        {
                                            dataGridView1.Rows[index].Cells["��������"].Style.ForeColor = Color.Red;
                                        }
                                        // ��ӱ�׼ʱ��
                                        int normalTime = 0;
                                        if (level < taskData.NormalTimeArray3.Length)
                                        {
                                            normalTime = taskData.NormalTimeArray3[level];
                                        }
                                        dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Value = normalTime.ToString();
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "1";
                                        // ��Ӿ������
                                        dataGridView1.Rows[index].Cells["�������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["�������"].Tag = "1";

                                        if (plus) // ��Ӹ�����Ϣ
                                        {
                                            virtualFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][0]);
                                            virtualFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][1]);
                                            additionalFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][2]);
                                            additionalFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][3]);
                                            // ���ʵ����������
                                            dataGridView1.Rows[index].Cells["ʵ������"].Value = string.Format("[{0}, {1}]",
                                                virtualFlowSectionList[index][1].ToString(), virtualFlowSectionList[index][0].ToString());
                                            // ��Ӽ�����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("[{0}, {1}]",
                                                additionalFlowSectionList[index][1].ToString(), additionalFlowSectionList[index][0].ToString());
                                            // �����Դ����
                                            dataGridView1.Rows[index].Cells["��Դ����"].Value = dataList[requireResourceCount][4];
                                            // �����·ʱ��
                                            dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Value = dataList[requireResourceCount][5];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = dataList[requireResourceCount][6];
                                            // ��Ӿ������
                                            dataGridView1.Rows[index].Cells["�������"].Value = dataList[requireResourceCount][7];
                                        }

                                        // ��¼��ID�������к�
                                        if (idTable[id.ToString()] == null)
                                        {
                                            List<int> indexList = new List<int>();
                                            indexList.Add(index);
                                            idTable[id.ToString()] = indexList;
                                        }
                                        else
                                        {
                                            List<int> indexList = idTable[id.ToString()] as List<int>;
                                            indexList.Add(index);
                                        }

                                        requireResourceCount++;
                                    }
                                    else if (dr["DropNpcTemplateID" + i].ToString().Trim() != "" && 
                                        dr["DropNpcTemplateID" + i].ToString().Trim() != "0") // �ռ������ж�
                                    {
                                        if (requireResourceCount > 0)
                                        {
                                            dataGridView1.Rows.Add(1); // �������
                                            virtualFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            additionalFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            index++;
                                            // �������ID������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("({0}) {1}", list[0], list[1]);
                                            dataGridView1.Rows[index].Cells["��������"].Tag = list[0];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = kind;
                                            dataGridView1.Rows[index].HeaderCell.Value = string.Format("({0}) {1}, {2}", list[0], list[1], kind);
                                            // ������ȼ�
                                            dataGridView1.Rows[index].Cells["�ȼ�"].Value = list[2];
                                        }

                                        // �޸���������
                                        dataGridView1.Rows[index].Cells["��������"].Value = "�ռ�����";
                                        dataGridView1.Rows[index].HeaderCell.Value = string.Format("({0}) {1}, {2}", list[0], list[1], "�ռ�����");
                                        // ��ӻ�ü���
                                        string tmpStr = dr["DropItemRate" + i].ToString();
                                        if (tmpStr == null || tmpStr == "")
                                        {
                                            dataGridView1.Rows[index].Cells["��ü���"].Value = "100%";
                                        }
                                        else
                                        {
                                            float number = float.Parse(tmpStr);
                                            float rate = number * 100 / 1048576;
                                            dataGridView1.Rows[index].Cells["��ü���"].Value = string.Format("{0:f1}%", rate);;
                                        }
                                        dataGridView1.Rows[index].Cells["��ü���"].Tag = "DropItemRate" + i;
                                        // ���������Դ
                                        string itemid = dr["EndRequireItemIndex" + i].ToString();
                                        string mapid = dr["EndRequireItemType" + i].ToString();
                                        if (itemid == "" || itemid == "0" || mapid == "" || mapid == "0") // item���������Ч
                                        {
                                            MessageBox.Show(string.Format("��{0}��������Ʒ���ò�ƥ��!", list[0]));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string itemName = GetItemName(itemid, mapid);
                                        if (itemName == null)
                                        {
                                            MessageBox.Show(string.Format("��{0}�������IDΪ{1}����Ʒ������!", list[0], itemid));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string npcName = GetNpcName(dr["DropNpcTemplateID" + i].ToString());
                                        if (npcName == null)
                                        {
                                            MessageBox.Show(string.Format("��{0}�������IDΪ{1}��NPC������!", list[0], dr["DropNpcTemplateID" + i].ToString()));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string resource = string.Format("{0} ({1})", itemName, npcName);
                                        dataGridView1.Rows[index].Cells["������Դ"].Value = resource;
                                        dataGridView1.Rows[index].Cells["������Դ"].Tag = npcName;
                                        npcIDTable[index.ToString()] = dr["DropNpcTemplateID" + i].ToString(); // ��¼NPC��ID
                                        if (resourceList.Contains(npcName)) // �Ƿ���δ��¼�Ĺ�����Դ
                                        {
                                            if (!publicResourceList.Contains(npcName))
                                            {
                                                publicResourceList.Add(npcName);
                                            }
                                            List<int> rowIndexList = publicResourceTable[npcName] as List<int>;
                                            rowIndexList.Add(index);
                                        }
                                        else
                                        {
                                            resourceList.Add(npcName);
                                            List<int> rowIndexList = new List<int>();
                                            rowIndexList.Add(index);
                                            publicResourceTable[npcName] = rowIndexList;
                                        }
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = dr["EndRequireItemAmount" + i].ToString();
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "EndRequireItemAmount" + i;
                                        // ��Ӳ���ʱ��
                                        int level = int.Parse(list[2]);
                                        int operateTime = 0;
                                        if(level < taskData.TimeArray.Length)
                                        {
                                            operateTime = taskData.TimeArray[level];
                                        }
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Value = operateTime.ToString();
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Tag = operateTime.ToString();
                                        // ��ӵ�λʱ�侭��                            
                                        int exp = 0;
                                        if (level < taskData.ExpArray.Length)
                                        {
                                            exp = (int)(taskData.ExpArray[level] * 1.2);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Value = exp.ToString();
                                        // ��ӵ�λʱ���Ǯ
                                        int money = 0;
                                        if (level < taskData.MoneyArray.Length)
                                        {
                                            money = (int)(taskData.MoneyArray[level] * 1.2);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Value = money.ToString();
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Tag = money.ToString();
                                        // ��ӱ�׼ʱ��
                                        int normalTime = 0;
                                        if (level < taskData.NormalTimeArray4.Length)
                                        {
                                            normalTime = taskData.NormalTimeArray4[level];
                                        }
                                        dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Value = normalTime.ToString();
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "1";
                                        // ��Ӿ������
                                        dataGridView1.Rows[index].Cells["�������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["�������"].Tag = "1";

                                        // �����Ԫ����ɫ
                                        switch (npcKind)
                                        {
                                            case 0: // ��ͨ��
                                                {
                                                    dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Black;
                                                    // ���ˢ��ʱ��
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = npcReviveTime.ToString();
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "1";
                                                    break;
                                                }
                                            case 1: // ������
                                                {
                                                    dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Red;
                                                    // ���ˢ��ʱ��
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = npcReviveTime.ToString();
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "120";
                                                    break;
                                                }
                                            case 2: // ������
                                                {
                                                    dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Orange;
                                                    // ���ˢ��ʱ��
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = npcReviveTime.ToString();
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "90";
                                                    break;
                                                }
                                        }
                                        int requireItemAmount = 0;
                                        if (dr["EndRequireItemAmount" + i] != null && dr["EndRequireItemAmount" + i].ToString() != "")
                                        {
                                            requireItemAmount = int.Parse(dr["EndRequireItemAmount" + i].ToString());
                                        }
                                        if (requireItemAmount > 10)
                                        {
                                            dataGridView1.Rows[index].Cells["��������"].Style.ForeColor = Color.Red;
                                        }
                                        npcKind = 0;

                                        if (plus) // ��Ӹ�����Ϣ
                                        {
                                            virtualFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][0]);
                                            virtualFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][1]);
                                            additionalFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][2]);
                                            additionalFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][3]);
                                            // ���ʵ����������
                                            dataGridView1.Rows[index].Cells["ʵ������"].Value = string.Format("[{0}, {1}]",
                                                virtualFlowSectionList[index][1].ToString(), virtualFlowSectionList[index][0].ToString());
                                            // ��Ӽ�����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("[{0}, {1}]",
                                                additionalFlowSectionList[index][1].ToString(), additionalFlowSectionList[index][0].ToString());
                                            // �����Դ����
                                            dataGridView1.Rows[index].Cells["��Դ����"].Value = dataList[requireResourceCount][4];
                                            // �����·ʱ��
                                            dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Value = dataList[requireResourceCount][5];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = dataList[requireResourceCount][6];
                                            // ��Ӿ������
                                            dataGridView1.Rows[index].Cells["�������"].Value = dataList[requireResourceCount][7];
                                        }

                                        // ��¼��ID�������к�
                                        if (idTable[id.ToString()] == null)
                                        {
                                            List<int> indexList = new List<int>();
                                            indexList.Add(index);
                                            idTable[id.ToString()] = indexList;
                                        }
                                        else
                                        {
                                            List<int> indexList = idTable[id.ToString()] as List<int>;
                                            indexList.Add(index);
                                        }

                                        requireResourceCount++;
                                    }
                                }
                                break;
                            }
                        case "�ռ�����": // �ռ�����
                            {
                                int requireResourceCount = 0; // ������Դ����
                                for (int i = 1; i <= 4; i++)
                                {
                                    if (dr["DropNpcTemplateID" + i].ToString().Trim() != "" 
                                        && dr["DropNpcTemplateID" + i].ToString().Trim() != "0") // �ռ������ж�
                                    {
                                        if (requireResourceCount > 0)
                                        {
                                            dataGridView1.Rows.Add(1); // �������
                                            virtualFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            additionalFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            index++;
                                            // �������ID������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("({0}) {1}", list[0], list[1]);
                                            dataGridView1.Rows[index].Cells["��������"].Tag = list[0];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = kind;
                                            dataGridView1.Rows[index].HeaderCell.Value = string.Format("({0}) {1}, {2}", list[0], list[1], kind);
                                            // ������ȼ�
                                            dataGridView1.Rows[index].Cells["�ȼ�"].Value = list[2];
                                                                                 
                                        }
                                        
                                        // ��ӻ�ü���
                                        string tmpStr = dr["DropItemRate" + i].ToString();
                                        if(tmpStr == null || tmpStr == "")
                                        {
                                            dataGridView1.Rows[index].Cells["��ü���"].Value = "100%";
                                        }
                                        else
                                        {
                                            float number = float.Parse(tmpStr);
                                            float rate = number * 100 / 1048576;
                                            dataGridView1.Rows[index].Cells["��ü���"].Value = string.Format("{0:f1}%", rate);;
                                        }
                                        dataGridView1.Rows[index].Cells["��ü���"].Tag = "DropItemRate" + i;
                                        // ���������Դ
                                        string itemid = dr["EndRequireItemIndex" + i].ToString();
                                        string mapid = dr["EndRequireItemType" + i].ToString();
                                        if (itemid == "" || itemid == "0" || mapid == "" || mapid == "0") // item���������Ч
                                        {
                                            MessageBox.Show(string.Format("��{0}��������Ʒ���ò�ƥ��!", list[0]));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index); 
                                            index--;
                                            continue;
                                        }
                                        string itemName = GetItemName(itemid, mapid);
                                        if (itemName == null)
                                        {
                                            MessageBox.Show(string.Format("��{0}�������IDΪ{1}����Ʒ������!", list[0], itemid));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string npcName = GetNpcName(dr["DropNpcTemplateID" + i].ToString());
                                        if (npcName == null)
                                        {
                                            MessageBox.Show(string.Format("��{0}�������IDΪ{1}��NPC������!", list[0], dr["DropNpcTemplateID" + i].ToString()));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string resource = string.Format("{0} ({1})", itemName, npcName);
                                        dataGridView1.Rows[index].Cells["������Դ"].Value = resource;
                                        dataGridView1.Rows[index].Cells["������Դ"].Tag = npcName;
                                        npcIDTable[index.ToString()] = dr["DropNpcTemplateID" + i].ToString(); // ��¼NPC��ID
                                        if (resourceList.Contains(npcName)) // �Ƿ���δ��¼�Ĺ�����Դ
                                        {
                                            if (!publicResourceList.Contains(npcName))
                                            {
                                                publicResourceList.Add(npcName);
                                            }
                                            List<int> rowIndexList = publicResourceTable[npcName] as List<int>;
                                            rowIndexList.Add(index);
                                        }
                                        else
                                        {
                                            resourceList.Add(npcName);
                                            List<int> rowIndexList = new List<int>();
                                            rowIndexList.Add(index);
                                            publicResourceTable[npcName] = rowIndexList;
                                        }
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = dr["EndRequireItemAmount" + i].ToString();
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "EndRequireItemAmount" + i;
                                        // ��Ӳ���ʱ��
                                        int level = int.Parse(list[2]);
                                        int operateTime = 0;
                                        if(level < taskData.TimeArray.Length)
                                        {
                                            operateTime = taskData.TimeArray[level];
                                        }
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Value = operateTime.ToString();
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Tag = operateTime.ToString();
                                        // ��ӵ�λʱ�侭��                                       
                                        int exp = 0;
                                        if (level < taskData.ExpArray.Length)
                                        {
                                            exp = (int)(taskData.ExpArray[level] * 1.2);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Value = exp.ToString();
                                        // ��ӵ�λʱ���Ǯ
                                        int money = 0;
                                        if (level < taskData.MoneyArray.Length)
                                        {
                                            money = (int)(taskData.MoneyArray[level] *1.2);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Value = money.ToString();
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Tag = money.ToString();
                                        // ��ӱ�׼ʱ��
                                        int normalTime = 0;
                                        if (level < taskData.NormalTimeArray4.Length)
                                        {
                                            normalTime = taskData.NormalTimeArray4[level];
                                        }
                                        dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Value = normalTime.ToString();
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "1";
                                        // ��Ӿ������
                                        dataGridView1.Rows[index].Cells["�������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["�������"].Tag = "1";

                                        // �����Ԫ����ɫ
                                        switch (npcKind)
                                        {
                                            case 0: // ��ͨ��
                                                {
                                                    dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Black;
                                                    // ���ˢ��ʱ��
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = npcReviveTime.ToString();
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "1";
                                                    break;
                                                }
                                            case 1: // ������
                                                {
                                                    dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Red;
                                                    // ���ˢ��ʱ��
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = npcReviveTime.ToString();
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "120";
                                                    break;
                                                }
                                            case 2: // ������
                                                {
                                                    dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Orange;
                                                    // ���ˢ��ʱ��
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = npcReviveTime.ToString();
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "90";
                                                    break;
                                                }
                                        }
                                        int requireItemAmount = 0;
                                        if (dr["EndRequireItemAmount" + i] != null && dr["EndRequireItemAmount" + i].ToString() != "")
                                        {
                                            requireItemAmount = int.Parse(dr["EndRequireItemAmount" + i].ToString());
                                        }
                                        if (requireItemAmount > 10)
                                        {
                                            dataGridView1.Rows[index].Cells["��������"].Style.ForeColor = Color.Red;
                                        }
                                        npcKind = 0;

                                        if (plus) // ��Ӹ�����Ϣ
                                        {
                                            virtualFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][0]);
                                            virtualFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][1]);
                                            additionalFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][2]);
                                            additionalFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][3]);
                                            // ���ʵ����������
                                            dataGridView1.Rows[index].Cells["ʵ������"].Value = string.Format("[{0}, {1}]",
                                                virtualFlowSectionList[index][1].ToString(), virtualFlowSectionList[index][0].ToString());
                                            // ��Ӽ�����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("[{0}, {1}]",
                                                additionalFlowSectionList[index][1].ToString(), additionalFlowSectionList[index][0].ToString());
                                            // �����Դ����
                                            dataGridView1.Rows[index].Cells["��Դ����"].Value = dataList[requireResourceCount][4];
                                            // �����·ʱ��
                                            dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Value = dataList[requireResourceCount][5];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = dataList[requireResourceCount][6];
                                            // ��Ӿ������
                                            dataGridView1.Rows[index].Cells["�������"].Value = dataList[requireResourceCount][7];
                                        }

                                        // ��¼��ID�������к�
                                        if (idTable[id.ToString()] == null)
                                        {
                                            List<int> indexList = new List<int>();
                                            indexList.Add(index);
                                            idTable[id.ToString()] = indexList;
                                        }
                                        else
                                        {
                                            List<int> indexList = idTable[id.ToString()] as List<int>;
                                            indexList.Add(index);
                                        }

                                        requireResourceCount++;
                                    }
                                    else if (dr["DropDoodadTemplateID" + i].ToString().Trim() != "" &&
                                        dr["DropDoodadTemplateID" + i].ToString().Trim() != "0") // �ɼ������ж�
                                    {
                                        if (requireResourceCount > 0)
                                        {
                                            dataGridView1.Rows.Add(1); // �������
                                            virtualFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            additionalFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            index++;
                                            // �������ID������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("({0}) {1}", list[0], list[1]);
                                            dataGridView1.Rows[index].Cells["��������"].Tag = list[0];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = kind;
                                            dataGridView1.Rows[index].HeaderCell.Value = string.Format("({0}) {1}, {2}", list[0], list[1], kind);
                                            // ������ȼ�
                                            dataGridView1.Rows[index].Cells["�ȼ�"].Value = list[2];
                                        }

                                        // �޸���������
                                        dataGridView1.Rows[index].Cells["��������"].Value = "�ɼ�����";
                                        dataGridView1.Rows[index].HeaderCell.Value = string.Format("({0}) {1}, {2}", list[0], list[1], "�ɼ�����");
                                        // ��ӻ�ü���
                                        string tmpStr = dr["DropItemRate" + i].ToString();
                                        if (tmpStr == null || tmpStr == "")
                                        {
                                            dataGridView1.Rows[index].Cells["��ü���"].Value = "100%";
                                        }
                                        else
                                        {
                                            float number = float.Parse(tmpStr);
                                            float rate = number * 100 / 1048576;
                                            dataGridView1.Rows[index].Cells["��ü���"].Value = string.Format("{0:f1}%", rate);;
                                        }
                                        dataGridView1.Rows[index].Cells["��ü���"].Tag = "DropItemRate" + i;
                                        // ���������Դ
                                        string itemid = dr["EndRequireItemIndex" + i].ToString();
                                        string mapid = dr["EndRequireItemType" + i].ToString();
                                        if (itemid == "" || itemid == "0" || mapid == "" || mapid == "0") // item���������Ч
                                        {
                                            MessageBox.Show(string.Format("��{0}��������Ʒ���ò�ƥ��!", list[0]));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string itemName = GetItemName(itemid, mapid);
                                        if (itemName == null)
                                        {
                                            MessageBox.Show(string.Format("��{0}�������IDΪ{1}����Ʒ������!", list[0], itemid));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string doodadName = GetDoodadName(dr["DropDoodadTemplateID" + i].ToString());
                                        if (doodadName == null)
                                        {
                                            MessageBox.Show(string.Format("��{0}�������IDΪ{1}��С���������!", list[0], dr["DropDoodadTemplateID" + i].ToString()));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string resource = string.Format("{0} ({1})", itemName, doodadName);
                                        dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Green;
                                        dataGridView1.Rows[index].Cells["������Դ"].Value = resource;
                                        dataGridView1.Rows[index].Cells["������Դ"].Tag = doodadName;
                                        doodadIDTable[index.ToString()] = dr["DropDoodadTemplateID" + i].ToString(); // ��¼doodad��id
                                        if (resourceList.Contains(doodadName)) // �Ƿ���δ��¼�Ĺ�����Դ
                                        {
                                            if (!publicResourceList.Contains(doodadName))
                                            {
                                                publicResourceList.Add(doodadName);
                                            }
                                            List<int> rowIndexList = publicResourceTable[doodadName] as List<int>;
                                            rowIndexList.Add(index);
                                        }
                                        else
                                        {
                                            resourceList.Add(doodadName);
                                            List<int> rowIndexList = new List<int>();
                                            rowIndexList.Add(index);
                                            publicResourceTable[doodadName] = rowIndexList;
                                        }
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = dr["EndRequireItemAmount" + i].ToString();
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "EndRequireItemAmount" + i;
                                        // ���ˢ��ʱ��
                                        dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = doodadReviveDelay.ToString();
                                        dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "1";
                                        // ��Ӳ���ʱ��
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Value = "5";
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Tag = "5";
                                        // ��ӵ�λʱ�侭��
                                        int level = int.Parse(list[2]);
                                        int exp = 0;
                                        if (level < taskData.ExpArray.Length)
                                        {
                                            exp = (int)(taskData.ExpArray[level] * 1.15);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Value = exp.ToString();
                                        // ��ӵ�λʱ���Ǯ
                                        int money = 0;
                                        if (level < taskData.MoneyArray.Length)
                                        {
                                            money = (int)(taskData.MoneyArray[level] * 1.15);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Value = money.ToString();
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Tag = money.ToString();
                                        // ��ӱ�׼ʱ��
                                        int normalTime = 0;
                                        if (level < taskData.NormalTimeArray3.Length)
                                        {
                                            normalTime = taskData.NormalTimeArray3[level];
                                        }
                                        dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Value = normalTime.ToString();
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "1";
                                        // ��Ӿ������
                                        dataGridView1.Rows[index].Cells["�������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["�������"].Tag = "1";

                                        // �����Ԫ����ɫ
                                        int requireItemAmount = 0;
                                        if (dr["EndRequireItemAmount" + i] != null && dr["EndRequireItemAmount" + i].ToString() != "")
                                        {
                                            requireItemAmount = int.Parse(dr["EndRequireItemAmount" + i].ToString());
                                        }
                                        if (requireItemAmount > 10)
                                        {
                                            dataGridView1.Rows[index].Cells["��������"].Style.ForeColor = Color.Red;
                                        }

                                        if (plus) // ��Ӹ�����Ϣ
                                        {
                                            virtualFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][0]);
                                            virtualFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][1]);
                                            additionalFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][2]);
                                            additionalFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][3]);
                                            // ���ʵ����������
                                            dataGridView1.Rows[index].Cells["ʵ������"].Value = string.Format("[{0}, {1}]",
                                                virtualFlowSectionList[index][1].ToString(), virtualFlowSectionList[index][0].ToString());
                                            // ��Ӽ�����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("[{0}, {1}]",
                                                additionalFlowSectionList[index][1].ToString(), additionalFlowSectionList[index][0].ToString());
                                            // �����Դ����
                                            dataGridView1.Rows[index].Cells["��Դ����"].Value = dataList[requireResourceCount][4];
                                            // �����·ʱ��
                                            dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Value = dataList[requireResourceCount][5];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = dataList[requireResourceCount][6];
                                            // ��Ӿ������
                                            dataGridView1.Rows[index].Cells["�������"].Value = dataList[requireResourceCount][7];
                                        }

                                        // ��¼��ID�������к�
                                        if (idTable[id.ToString()] == null)
                                        {
                                            List<int> indexList = new List<int>();
                                            indexList.Add(index);
                                            idTable[id.ToString()] = indexList;
                                        }
                                        else
                                        {
                                            List<int> indexList = idTable[id.ToString()] as List<int>;
                                            indexList.Add(index);
                                        }

                                        requireResourceCount++;
                                    }
                                    else if (dr["EndRequireItemIndex" + i].ToString().Trim() != "" && 
                                        dr["EndRequireItemIndex" + i].ToString().Trim() != "0") // �ռ������ж�
                                    {
                                        if (requireResourceCount > 0)
                                        {
                                            dataGridView1.Rows.Add(1); // �������
                                            virtualFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            additionalFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            index++;
                                            // �������ID������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("({0}) {1}", list[0], list[1]);
                                            dataGridView1.Rows[index].Cells["��������"].Tag = list[0];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = kind;
                                            dataGridView1.Rows[index].HeaderCell.Value = string.Format("({0}) {1}, {2}", list[0], list[1], kind);
                                            // ������ȼ�
                                            dataGridView1.Rows[index].Cells["�ȼ�"].Value = list[2];                                        
                                        }
                                        
                                        // ��ӻ�ü���
                                        string tmpStr = dr["DropItemRate" + i].ToString();
                                        if (tmpStr == null || tmpStr == "")
                                        {
                                            dataGridView1.Rows[index].Cells["��ü���"].Value = "100%";
                                        }
                                        else
                                        {
                                            float number = float.Parse(tmpStr);
                                            float rate = number * 100 / 1048576;
                                            dataGridView1.Rows[index].Cells["��ü���"].Value = string.Format("{0:f1}%", rate);;
                                        }
                                        dataGridView1.Rows[index].Cells["��ü���"].Tag = "DropItemRate" + i;
                                        // ���������Դ
                                        string itemid = dr["EndRequireItemIndex" + i].ToString();
                                        string mapid = dr["EndRequireItemType" + i].ToString();
                                        if (itemid == "" || itemid == "0" || mapid == "" || mapid == "0") // item���������Ч
                                        {
                                            MessageBox.Show(string.Format("��{0}��������Ʒ���ò�ƥ��!", list[0]));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index); 
                                            index--;
                                            continue;
                                        }
                                        string itemName = GetItemName(itemid, mapid);
                                        if (itemName == null)
                                        {
                                            MessageBox.Show(string.Format("��{0}�������IDΪ{1}����Ʒ������!", list[0], itemid));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        string resource = string.Format("{0} (other)", itemName);
                                        dataGridView1.Rows[index].Cells["������Դ"].Value = resource;
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = dr["EndRequireItemAmount" + i].ToString();
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "EndRequireItemAmount" + i;
                                        // ��Ӳ���ʱ��
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Value = "2";
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Tag = "2";
                                        // ��ӵ�λʱ�侭��
                                        int level = int.Parse(list[2]);
                                        int exp = 0;
                                        if (level < taskData.ExpArray.Length)
                                        {
                                            exp = (int)(taskData.ExpArray[level] * 1.2);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Value = exp.ToString();
                                        // ��ӵ�λʱ���Ǯ
                                        int money = 0;
                                        if (level < taskData.MoneyArray.Length)
                                        {
                                            money = (int)(taskData.MoneyArray[level] * 1.2);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Value = money.ToString();
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Tag = money.ToString();
                                        // ��ӱ�׼ʱ��
                                        int normalTime = 0;
                                        if (level < taskData.NormalTimeArray4.Length)
                                        {
                                            normalTime = taskData.NormalTimeArray4[level];
                                        }
                                        dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Value = normalTime.ToString();
                                        // ���ˢ��ʱ��
                                        dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "1";
                                        dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = "1";
                                        dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Style.BackColor = Color.White;
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "1";
                                        // ��Ӿ������
                                        dataGridView1.Rows[index].Cells["�������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["�������"].Tag = "1";

                                        // �����Ԫ����ɫ
                                        int requireItemAmount = 0;
                                        if (dr["EndRequireItemAmount" + i] != null && dr["EndRequireItemAmount" + i].ToString() != "")
                                        {
                                            requireItemAmount = int.Parse(dr["EndRequireItemAmount" + i].ToString());
                                        }
                                        if (requireItemAmount > 10)
                                        {
                                            dataGridView1.Rows[index].Cells["��������"].Style.ForeColor = Color.Red;
                                        }

                                        if (plus) // ��Ӹ�����Ϣ
                                        {
                                            virtualFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][0]);
                                            virtualFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][1]);
                                            additionalFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][2]);
                                            additionalFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][3]);
                                            // ���ʵ����������
                                            dataGridView1.Rows[index].Cells["ʵ������"].Value = string.Format("[{0}, {1}]",
                                                virtualFlowSectionList[index][1].ToString(), virtualFlowSectionList[index][0].ToString());
                                            // ��Ӽ�����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("[{0}, {1}]",
                                                additionalFlowSectionList[index][1].ToString(), additionalFlowSectionList[index][0].ToString());
                                            // �����Դ����
                                            dataGridView1.Rows[index].Cells["��Դ����"].Value = dataList[requireResourceCount][4];
                                            // �����·ʱ��
                                            dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Value = dataList[requireResourceCount][5];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = dataList[requireResourceCount][6];
                                            // ��Ӿ������
                                            dataGridView1.Rows[index].Cells["�������"].Value = dataList[requireResourceCount][7];
                                        }

                                        // ��¼��ID�������к�
                                        if (idTable[id.ToString()] == null)
                                        {
                                            List<int> indexList = new List<int>();
                                            indexList.Add(index);
                                            idTable[id.ToString()] = indexList;
                                        }
                                        else
                                        {
                                            List<int> indexList = idTable[id.ToString()] as List<int>;
                                            indexList.Add(index);
                                        }

                                        requireResourceCount++;
                                    }                                   
                                }

                                break;
                            }
                        case "ɱ������": // ɱ������
                            {
                                int requireResourceCount = 0; // ������Դ����
                                for (int i = 1; i <= 4; i++)
                                {
                                    if(dr["KillNpcTemplateID" + i].ToString() != "" && dr["KillNpcTemplateID" + i].ToString() != "0")
                                    {
                                        if(requireResourceCount > 0)
                                        {
                                            dataGridView1.Rows.Add(1); // �������
                                            virtualFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            additionalFlowSectionList.Add(new float[] { 0.00f, 0.00f });
                                            index++;
                                            // �������ID������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("({0}) {1}", list[0], list[1]);
                                            dataGridView1.Rows[index].Cells["��������"].Tag = list[0];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = kind;
                                            dataGridView1.Rows[index].HeaderCell.Value = string.Format("({0}) {1}, {2}", list[0], list[1], kind);
                                            // ������ȼ�
                                            dataGridView1.Rows[index].Cells["�ȼ�"].Value = list[2];
                                        }
                                       
                                        // ��ӻ�ü���
                                        dataGridView1.Rows[index].Cells["��ü���"].Style.BackColor = readOnlyColor;
                                        dataGridView1.Rows[index].Cells["��ü���"].Value = "100%";
                                        dataGridView1.Rows[index].Cells["��ü���"].Tag = "100%";
                                        dataGridView1.Rows[index].Cells["��ü���"].ReadOnly = true; // ����Ϊֻ��
                                        // ���������Դ
                                        string npcName = GetNpcName(dr["KillNpcTemplateID" + i].ToString());
                                        if (npcName == null)
                                        {
                                            MessageBox.Show(string.Format("��{0}�������IDΪ{1}��NPC������!", list[0], dr["KillNpcTemplateID" + i].ToString()));
                                            dataGridView1.Rows.RemoveAt(index); // ɾ����
                                            virtualFlowSectionList.RemoveAt(index);
                                            additionalFlowSectionList.RemoveAt(index);
                                            index--;
                                            continue;
                                        }
                                        dataGridView1.Rows[index].Cells["������Դ"].Value = npcName;
                                        dataGridView1.Rows[index].Cells["������Դ"].Tag = npcName;
                                        npcIDTable[index.ToString()] = dr["KillNpcTemplateID" + i].ToString(); // ��¼NPC��ID
                                        if (resourceList.Contains(npcName)) // �Ƿ���δ��¼�Ĺ�����Դ
                                        {
                                            if (!publicResourceList.Contains(npcName))
                                            {
                                                publicResourceList.Add(npcName);
                                            }
                                            List<int> rowIndexList = publicResourceTable[npcName] as List<int>;
                                            rowIndexList.Add(index);
                                        }
                                        else
                                        {
                                            resourceList.Add(npcName);
                                            List<int> rowIndexList = new List<int>();
                                            rowIndexList.Add(index);
                                            publicResourceTable[npcName] = rowIndexList;
                                        }
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = dr["KillNpcAmount" + i].ToString();
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "KillNpcAmount" + i;
                                        // ��Ӳ���ʱ��
                                        int level = int.Parse(list[2]);
                                        int operateTime = 0;
                                        if(level < taskData.TimeArray.Length)
                                        {
                                            operateTime = taskData.TimeArray[level];
                                        }
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Value = operateTime.ToString();
                                        dataGridView1.Rows[index].Cells["����ʱ��(��)"].Tag = operateTime.ToString();
                                        // ��ӵ�λʱ�侭��
                                        int exp = 0;
                                        if (level < taskData.ExpArray.Length)
                                        {
                                            exp = (int)(taskData.ExpArray[level] * 1.2);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Value = exp.ToString();
                                        // ��ӵ�λʱ���Ǯ
                                        int money = 0;
                                        if (level < taskData.MoneyArray.Length)
                                        {
                                            money = (int)(taskData.MoneyArray[level] * 1.2);
                                        }
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Value = money.ToString();
                                        dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Tag = money.ToString();
                                        // ��ӱ�׼ʱ��
                                        int normalTime = 0;
                                        if (level < taskData.NormalTimeArray4.Length)
                                        {
                                            normalTime = taskData.NormalTimeArray4[level];
                                        }
                                        dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Value = normalTime.ToString();
                                        // �����������
                                        dataGridView1.Rows[index].Cells["��������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["��������"].Tag = "1";
                                        // ��Ӿ������
                                        dataGridView1.Rows[index].Cells["�������"].Value = "1";
                                        dataGridView1.Rows[index].Cells["�������"].Tag = "1";

                                        // �����Ԫ����ɫ
                                        switch (npcKind)
                                        {
                                            case 0: // ��ͨ��
                                                {
                                                    dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Black;
                                                    // ���ˢ��ʱ��
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = npcReviveTime.ToString();
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "1";
                                                    break;
                                                }
                                            case 1: // ������
                                                {
                                                    dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Red;
                                                    // ���ˢ��ʱ��
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = npcReviveTime.ToString();
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "120";
                                                    break;
                                                }
                                            case 2: // ������
                                                {
                                                    dataGridView1.Rows[index].Cells["������Դ"].Style.ForeColor = Color.Orange;
                                                    // ���ˢ��ʱ��
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = npcReviveTime.ToString();
                                                    dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Tag = "90";
                                                    break;
                                                }
                                        }
                                        if (int.Parse(dr["KillNpcAmount" + i].ToString()) > 10)
                                        {
                                            dataGridView1.Rows[index].Cells["��������"].Style.ForeColor = Color.Red;
                                        }
                                        npcKind = 0;

                                        if (plus) // ��Ӹ�����Ϣ
                                        {
                                            virtualFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][0]);
                                            virtualFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][1]);
                                            additionalFlowSectionList[index][0] = float.Parse(dataList[requireResourceCount][2]);
                                            additionalFlowSectionList[index][1] = float.Parse(dataList[requireResourceCount][3]);
                                            // ���ʵ����������
                                            dataGridView1.Rows[index].Cells["ʵ������"].Value = string.Format("[{0}, {1}]",
                                                virtualFlowSectionList[index][1].ToString(), virtualFlowSectionList[index][0].ToString());
                                            // ��Ӽ�����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = string.Format("[{0}, {1}]",
                                                additionalFlowSectionList[index][1].ToString(), additionalFlowSectionList[index][0].ToString());
                                            // �����Դ����
                                            dataGridView1.Rows[index].Cells["��Դ����"].Value = dataList[requireResourceCount][4];
                                            // �����·ʱ��
                                            dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Value = dataList[requireResourceCount][5];
                                            // �����������
                                            dataGridView1.Rows[index].Cells["��������"].Value = dataList[requireResourceCount][6];
                                            // ��Ӿ������
                                            dataGridView1.Rows[index].Cells["�������"].Value = dataList[requireResourceCount][7];
                                        }

                                        // ��¼��ID�������к�
                                        if (idTable[id.ToString()] == null)
                                        {
                                            List<int> indexList = new List<int>();
                                            indexList.Add(index);
                                            idTable[id.ToString()] = indexList;
                                        }
                                        else
                                        {
                                            List<int> indexList = idTable[id.ToString()] as List<int>;
                                            indexList.Add(index);
                                        }

                                        requireResourceCount++;
                                    }
                                }
                                break;
                            }
                        case "��������": // ��������
                            {
                                // ��ӻ�ü���
                                dataGridView1.Rows[index].Cells["��ü���"].Value = "����";
                                dataGridView1.Rows[index].Cells["��ü���"].ReadOnly = true; // ����Ϊֻ��
                                dataGridView1.Rows[index].Cells["��ü���"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ���ˢ��ʱ��
                                dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = "����";
                                dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].ReadOnly = true; // ����Ϊֻ��
                                dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ��Ӳ���ʱ��
                                dataGridView1.Rows[index].Cells["����ʱ��(��)"].Value = "2";
                                dataGridView1.Rows[index].Cells["����ʱ��(��)"].Tag = "2";
                                // ���������Դ
                                dataGridView1.Rows[index].Cells["������Դ"].Value = "����";
                                // �����������
                                dataGridView1.Rows[index].Cells["��������"].Value = "����";
                                dataGridView1.Rows[index].Cells["��������"].ReadOnly = true; // ����Ϊֻ��
                                dataGridView1.Rows[index].Cells["��������"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ��ӵ�λʱ�侭��
                                int level = int.Parse(list[2]);
                                int exp = 0;
                                if (level < taskData.ExpArray.Length)
                                {
                                    exp = (int)(taskData.ExpArray[level] * 1.05);
                                }
                                dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Value = exp.ToString();
                                // ��ӵ�λʱ���Ǯ
                                int money = 0;
                                if (level < taskData.MoneyArray.Length)
                                {
                                    money = (int)(taskData.MoneyArray[level] * 1.05);
                                }
                                dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Value = money.ToString();
                                dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Tag = money.ToString();
                                // ��ӱ�׼ʱ��
                                int normalTime = 0;
                                if (level < taskData.NormalTimeArray2.Length)
                                {
                                    normalTime = taskData.NormalTimeArray2[level];
                                }
                                dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Value = normalTime.ToString();
                                // �����������
                                dataGridView1.Rows[index].Cells["��������"].Value = "1";
                                dataGridView1.Rows[index].Cells["��������"].Tag = "1";
                                // ��Ӿ������
                                dataGridView1.Rows[index].Cells["�������"].Value = "1";
                                dataGridView1.Rows[index].Cells["�������"].Tag = "1";
                                // �����Դ����
                                dataGridView1.Rows[index].Cells["��Դ����"].Value = "����";
                                dataGridView1.Rows[index].Cells["��Դ����"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ

                                if (plus) // ��Ӹ�����Ϣ
                                {
                                    virtualFlowSectionList[index][0] = float.Parse(dataList[0][0]);
                                    virtualFlowSectionList[index][1] = float.Parse(dataList[0][1]);
                                    additionalFlowSectionList[index][0] = float.Parse(dataList[0][2]);
                                    additionalFlowSectionList[index][1] = float.Parse(dataList[0][3]);
                                    // ���ʵ����������
                                    dataGridView1.Rows[index].Cells["ʵ������"].Value = string.Format("[{0}, {1}]",
                                        virtualFlowSectionList[index][1].ToString(), virtualFlowSectionList[index][0].ToString());
                                    // ��Ӽ�����������
                                    dataGridView1.Rows[index].Cells["��������"].Value = string.Format("[{0}, {1}]",
                                        additionalFlowSectionList[index][1].ToString(), additionalFlowSectionList[index][0].ToString());
                                    // �����Դ����
                                    dataGridView1.Rows[index].Cells["��Դ����"].Value = dataList[0][4];
                                    // �����·ʱ��
                                    dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Value = dataList[0][5];
                                    // �����������
                                    dataGridView1.Rows[index].Cells["��������"].Value = dataList[0][6];
                                    // ��Ӿ������
                                    dataGridView1.Rows[index].Cells["�������"].Value = dataList[0][7];
                                }

                                // ��¼��ID�������к�
                                List<int> indexList = new List<int>();
                                indexList.Add(index);
                                idTable[id.ToString()] = indexList;

                                break;
                            }
                        case "�Ի�����": // �Ի�����
                            {
                                // ��ӻ�ü���
                                dataGridView1.Rows[index].Cells["��ü���"].Value = "����";
                                dataGridView1.Rows[index].Cells["��ü���"].ReadOnly = true; // ����Ϊֻ��
                                dataGridView1.Rows[index].Cells["��ü���"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ���ˢ��ʱ��
                                dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Value = "����";
                                dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].ReadOnly = true; // ����Ϊֻ��
                                dataGridView1.Rows[index].Cells["ˢ��ʱ��(��)"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ��Ӳ���ʱ��
                                dataGridView1.Rows[index].Cells["����ʱ��(��)"].Value = "2";
                                dataGridView1.Rows[index].Cells["����ʱ��(��)"].Tag = "2";
                                // ���������Դ
                                dataGridView1.Rows[index].Cells["������Դ"].Value = "����";
                                // �����������
                                dataGridView1.Rows[index].Cells["��������"].Value = "����";
                                dataGridView1.Rows[index].Cells["��������"].ReadOnly = true; // ����Ϊֻ��
                                dataGridView1.Rows[index].Cells["��������"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ
                                // ��ӵ�λʱ�侭��
                                int level = int.Parse(list[2]);
                                int exp = 0;
                                if (level < taskData.ExpArray.Length)
                                {
                                    exp = taskData.ExpArray[level];
                                }
                                dataGridView1.Rows[index].Cells["��λʱ�侭��(��)"].Value = exp.ToString();
                                // ��ӵ�λʱ���Ǯ
                                int money = 0;
                                if (level < taskData.MoneyArray.Length)
                                {
                                    money = taskData.MoneyArray[level];
                                }
                                dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Value = money.ToString();
                                dataGridView1.Rows[index].Cells["��λʱ���Ǯ(��)"].Tag = money.ToString();
                                // ��ӱ�׼ʱ��
                                int normalTime = 0;
                                if (level < taskData.NormalTimeArray1.Length)
                                {
                                    normalTime = taskData.NormalTimeArray1[level];
                                }
                                dataGridView1.Rows[index].Cells["��׼ʱ��(��)"].Value = normalTime.ToString();
                                // �����������
                                dataGridView1.Rows[index].Cells["��������"].Value = "1";
                                dataGridView1.Rows[index].Cells["��������"].Tag = "1";
                                // ��Ӿ������
                                dataGridView1.Rows[index].Cells["�������"].Value = "1";
                                dataGridView1.Rows[index].Cells["�������"].Tag = "1";
                                // �����Դ����
                                dataGridView1.Rows[index].Cells["��Դ����"].Value = "����";
                                dataGridView1.Rows[index].Cells["��Դ����"].Style.BackColor = readOnlyColor; // ���õ�Ԫ����ɫ

                                if (plus) // ��Ӹ�����Ϣ
                                {
                                    virtualFlowSectionList[index][0] = float.Parse(dataList[0][0]);
                                    virtualFlowSectionList[index][1] = float.Parse(dataList[0][1]);
                                    additionalFlowSectionList[index][0] = float.Parse(dataList[0][2]);
                                    additionalFlowSectionList[index][1] = float.Parse(dataList[0][3]);
                                    // ���ʵ����������
                                    dataGridView1.Rows[index].Cells["ʵ������"].Value = string.Format("[{0}, {1}]",
                                        virtualFlowSectionList[index][1].ToString(), virtualFlowSectionList[index][0].ToString());
                                    // ��Ӽ�����������
                                    dataGridView1.Rows[index].Cells["��������"].Value = string.Format("[{0}, {1}]",
                                        additionalFlowSectionList[index][1].ToString(), additionalFlowSectionList[index][0].ToString());
                                    // �����Դ����
                                    dataGridView1.Rows[index].Cells["��Դ����"].Value = dataList[0][4];
                                    // �����·ʱ��
                                    dataGridView1.Rows[index].Cells["��·ʱ��(��)"].Value = dataList[0][5];
                                    // �����������
                                    dataGridView1.Rows[index].Cells["��������"].Value = dataList[0][6];
                                    // ��Ӿ������
                                    dataGridView1.Rows[index].Cells["�������"].Value = dataList[0][7];
                                }

                                // ��¼��ID�������к�
                                List<int> indexList = new List<int>();
                                indexList.Add(index);
                                idTable[id.ToString()] = indexList;

                                break;
                            }
                    }
                    index++;
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show(string.Format("�����������{0}�������������ʱ�����쳣: " + ex.ToString(), ((object[])node.Tag)[0].ToString()));
            }
        }

        /// <summary>
        /// ��ȡNPC������
        /// </summary>
        /// <param name="id">NPC��ID</param>
        /// <returns>NPC������</returns>
        private string GetNpcName(string id)
        {
            string sql = string.Format("SELECT Name, AIParamTemplateID, ReviveTime FROM NpcTemplate WHERE ID = {0}", id);
            SqlCommand cmd;
            SqlDataReader reader = null;
            string name = "δ�ҵ���NPC";
            int aiid = 0;

            try
            {
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                reader.Read();
                if (!reader.HasRows) // ��ѯ���Ϊ��
                {
                    return null;
                }
                else
                {
                    if(reader[0] != null)
                    {
                        name = reader[0].ToString().Trim();
                    }
                    if(reader[1] != null)
                    {   
                        if((reader[1] is DBNull) && (npcDefaultRow != null)) // ����ǿ�ֵ�Ļ��滻ΪĬ���е�����
                        {
                            aiid = int.Parse(npcDefaultRow["AIParamTemplateID"].ToString());
                        }
                        else
                        {
                            aiid = int.Parse(reader[1].ToString().Trim());
                        }
                    }
                    if(reader[2] != null)
                    {
                        if ((reader[2] is DBNull) && (npcDefaultRow != null)) // ����ǿ�ֵ�Ļ��滻ΪĬ���е�����
                        {
                            npcReviveTime = int.Parse(npcDefaultRow["ReviveTime"].ToString());
                        }
                        else
                        {
                            npcReviveTime = int.Parse(reader[2].ToString().Trim());
                        }
                    }
                    
                    if(aiid >= 11 && aiid <= 16) // ������
                    {
                        npcKind = 1;
                    }
                    else if(aiid >= 21 && aiid <= 26) // ������
                    {
                        npcKind = 2;
                    }
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("��ȡNPC������ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if(reader != null) // �ر�reader
                {
                    reader.Close();
                }
            }
            return name;
        }

        /// <summary>
        /// ��ȡDoodad������
        /// </summary>
        /// <param name="id">Doodad��ID</param>
        /// <returns>Doodad������</returns>
        private string GetDoodadName(string id)
        {
            string sql = string.Format("SELECT Name, ReviveDelay FROM tbl_doodad WHERE ID = {0}", id);
            SqlCommand cmd;
            SqlDataReader reader = null;
            string name = "δ�ҵ���С���";

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
                    name = reader[0].ToString().Trim();

                    if(reader[1] != null) // û�ж���ˢ��ʱ��
                    {
                        if ((reader[1] is DBNull) && (npcDefaultRow != null)) // ����ǿ�ֵ�Ļ��滻ΪĬ���е�����
                        {
                            doodadReviveDelay = int.Parse(doodadDefaultRow["ReviveDelay"].ToString());
                        }
                        else
                        {
                            doodadReviveDelay = int.Parse(reader[1].ToString().Trim());
                        }
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("��ȡС���������ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                reader.Close();
            }
            return name;
        }

        /// <summary>
        /// ��ȡItem������
        /// </summary>
        /// <param name="id">Item��ID</param>
        /// <param name="mapid">Map��ID</param>
        /// <returns>Item������</returns>
        private string GetItemName(string id, string mapid)
        {
            string map = "0";
            foreach(KeyValuePair<string, string> k in mapList) // ��map��IDת��ΪName
            {
                if(mapid == k.Key)
                {
                    map = k.Value;
                }
            }

            string sql = string.Format("SELECT Name FROM {0} WHERE ID = {1}", map, id);
            SqlCommand cmd;
            SqlDataReader reader = null;
            string name = "δ�ҵ�����Ʒ";
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
                    name = reader[0].ToString().Trim();
                }           
            }
            catch (SqlException ex)
            {
                MessageBox.Show("��ȡ��Ʒ������ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                reader.Close();
            }
            return name;
        }

        /// <summary>
        /// ��ȡ���񸽼���Ϣ
        /// </summary>
        /// <param name="id">����ID��</param>
        /// <returns>���񸽼���Ϣ�ַ���</returns>
        private string GetQuestPlusInfo(string id)
        {
            string sql = string.Format("SELECT PlusInfo FROM {0} WHERE QuestID = {1}", plusTable, id);
            string info = null;
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                reader.Read();
                if(!reader.HasRows) // ���в�����
                {
                    reader.Close();
                    cmd = conn.CreateCommand();
                    cmd.CommandText = string.Format("INSERT INTO {0} ({1}) VALUES ({2})", plusTable, "QuestID", id);
                    cmd.ExecuteNonQuery();
                    return null;
                }
                else
                {
                    info = reader[0].ToString().Trim();
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("��ȡ���񸽼���Ϣʱ����Sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(!reader.IsClosed) // reader�Ƿ��ѹر�
                {
                    reader.Close();
                }
            }
            return info;
        }

        /// <summary>
        /// �������񸽼���Ϣ
        /// </summary>
        /// <param name="id">����ID��</param>
        /// <param name="info">���񸽼���Ϣ</param>
        private void SetQuestPlusInfo(string id, string info)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE QuestID = {3}", plusTable, "PlusInfo", info, id);
                cmd.ExecuteNonQuery();
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�������񸽼���Ϣʱ����Sql�쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// ��������Ϣת��Ϊ������Ϣ��
        /// </summary>
        /// <param name="info">������Ϣ</param>
        /// <returns>������Ϣ��</returns>
        private List<List<string>> GetPlusData(string info)
        {
            List<List<string>> result = new List<List<string>>();
            string[] data1 = info.Split(';');
            
            foreach(string s1 in data1)
            {
                List<string> list1 = new List<string>();
                string[] data2 = s1.Split(',');
                foreach(string s2 in data2)
                {
                    list1.Add(s2);
                }
                result.Add(list1);
            }
            return result;
        }

        /// <summary>
        ///  ��������ĵȼ���Ϣ
        /// </summary>
        /// <param name="id">�����ID</param>
        /// <param name="level">����ĵȼ�</param>
        private void UpdateMapLevel(string id, string level)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE QuestID = {3}", "tbl_quests", "QuestLevel", level, id);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("���µ�{0}������ĵȼ���Ϣʱ����Sql�쳣: " + ex.ToString(), id));
            }
        }

        /// <summary>
        /// �����������������
        /// </summary>
        /// <param name="id">�����ID</param>
        /// <param name="attribute">��Ʒ����</param>
        /// <param name="number">��������</param>
        private void UpdateRequestNumber(string id, string attribute,string number)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE QuestID = {3}", "tbl_quests", attribute, number, id);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("���µ�{0}���������������ʱ����Sql�쳣: " + ex.ToString(), id));
            }
        }

        /// <summary>
        /// ��������Ļ�ü���
        /// </summary>
        /// <param name="id">�����ID</param>
        /// <param name="attribute">��������</param>
        /// <param name="rate">��ü���</param>
        private void UpdateGetRate(string id, string attribute, string rate)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE QuestID = {3}", "tbl_quests", attribute, rate, id);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("���µ�{0}������Ļ�ü���ʱ����Sql�쳣: " + ex.ToString(), id));
            }
        }

        /// <summary>
        /// ��������Ľ�������
        /// </summary>
        /// <param name="id">�����ID</param>
        /// <param name="value">������ֵ</param>
        private void UpdatePresentExp(string id, string value)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE QuestID = {3}", "tbl_quests", "PresentExp", value, id);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("���µ�{0}������Ľ�������ʱ����Sql�쳣: " + ex.ToString(), id));
            }
        }

        /// <summary>
        /// ��������Ľ�����Ǯ
        /// </summary>
        /// <param name="id">�����ID</param>
        /// <param name="value">��Ǯ��ֵ</param>
        private void UpdatePresentMoney(string id, string value)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE QuestID = {3}", "tbl_quests", "PresentMoney", value, id);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("���µ�{0}������Ľ�����Ǯʱ����Sql�쳣: " + ex.ToString(), id));
            }
        }

        /// <summary>
        /// ����NPCˢ��ʱ��
        /// </summary>
        /// <param name="id">NPC��ID</param>
        /// <param name="value">ˢ��ʱ��</param>
        private void UpdateNPCRefreshTime(string id, string value)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE ID = {3}", "NpcTemplate", "ReviveTime", value, id);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("���µ�{0}�������NPCˢ��ʱ��ʱ����Sql�쳣: " + ex.ToString(), id));
            }
        }

        /// <summary>
        /// ����Doodadˢ��ʱ��
        /// </summary>
        /// <param name="id">Doodad��ID</param>
        /// <param name="value">ˢ��ʱ��</param>
        private void UpdateDoodadRefreshTime(string id, string value)
        {
            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE ID = {3}", "tbl_doodad", "ReviveDelay", value, id);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show(string.Format("���µ�{0}�������Doodadˢ��ʱ��ʱ����Sql�쳣: " + ex.ToString(), id));
            }
        }

        /// <summary>
        /// ����ʵ������
        /// </summary>
        /// <param name="i">����������</param>
        private void ComputeVirtualFlow(int i)
        {
            float virtualFlow = 0.00f; // ʵ������
            string rC = dataGridView1.Rows[i].Cells["��Դ����"].EditedFormattedValue.ToString().Trim(); // ��Դ����
            string rT = dataGridView1.Rows[i].Cells["ˢ��ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ˢ��ʱ��
            string oT = dataGridView1.Rows[i].Cells["����ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ����ʱ��
            string gR = dataGridView1.Rows[i].Cells["��ü���"].EditedFormattedValue.ToString().TrimEnd('%'); // ��ü���
            string rN = dataGridView1.Rows[i].Cells["��������"].EditedFormattedValue.ToString(); // ��������
            if (rC == "" || rC == "����") // δ������������
            {
                MessageBox.Show(string.Format("δ���õ�{0}����Դ����!", i.ToString()));
                return;
            }
            if (rT == "" || rT == "����") // δ����ˢ��ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}��ˢ��ʱ��!", i.ToString()));
                return;
            }
            if (oT == "" || oT == "����") // δ���ò���ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�в���ʱ��!", i.ToString()));
                return;
            }
            if (gR == "" || gR == "����") // δ���û�ü���
            {
                MessageBox.Show(string.Format("δ���õ�{0}�л�ü���!", i.ToString()));
                return;
            }
            if(rN == "" || rN == "����") // δ������������
            {
                MessageBox.Show(string.Format("δ���õ�{0}����������!", i.ToString()));
                return;
            }
            try
            {
                int resourceCount = int.Parse(rC); // ��Դ����
                int refreshTime = int.Parse(rT); // ˢ��ʱ��
                int operateTime = int.Parse(oT); // ����ʱ��
                float getRate = float.Parse(gR); // ��ü���
                int requireNumber = int.Parse(rN); // ��������
                //ʵ������ = (((��Դ����/(ˢ��ʱ��+����ʱ��))*60)*��ü���)/��������
                virtualFlow = ((((float)resourceCount / (refreshTime + operateTime)) * 60) * getRate) / requireNumber / 100; // ����ʵ������
                float downNumber = virtualFlowSectionList[i][1]; // ��ֵ����
                float upNumber = virtualFlowSectionList[i][0]; // ��ֵ����
                if(downNumber == 0.00 && upNumber == 0.00) // δ����������
                {
                    MessageBox.Show(string.Format("δ���õ�{0}��ʵ���������ݵ�������!", i.ToString()));
                    return;
                }
                if(virtualFlow < downNumber)
                {
                    dataGridView1.Rows[i].Cells["ʵ������"].Style.ForeColor = Color.Red;
                }
                else if(virtualFlow > upNumber)
                {
                    dataGridView1.Rows[i].Cells["ʵ������"].Style.ForeColor = Color.Blue;
                    if(virtualFlow > upNumber * 2)
                    {
                        dataGridView1.Rows[i].Cells["ʵ������"].Value = "��";
                        return;
                    }
                }
                else
                {
                    dataGridView1.Rows[i].Cells["ʵ������"].Style.ForeColor = Color.Green;
                }
                dataGridView1.Rows[i].Cells["ʵ������"].Value = string.Format("{0:f1}", virtualFlow);
            }
            catch(Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}��ʵ������ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// ���㵥������ʱ��
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeSingleTaskTime(int i)
        {
            int singleTaskTime = 0; // ��������ʱ��
            string rT = dataGridView1.Rows[i].Cells["��·ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ��·ʱ��
            string oT = dataGridView1.Rows[i].Cells["����ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ����ʱ��
            string rN = dataGridView1.Rows[i].Cells["��Դ����"].EditedFormattedValue.ToString().Trim(); // ��Դ����
            string dR = dataGridView1.Rows[i].Cells["��ü���"].EditedFormattedValue.ToString().TrimEnd('%'); // ���伸��

            if (rT == "" || rT == "����") // δ������·ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}����·ʱ��!", i.ToString()));
                return;
            }
            if (oT == "" || oT == "����") // δ���ò���ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�в���ʱ��!", i.ToString()));
                return;
            }
            if (rN == "") // δ������Դ����
            {
                MessageBox.Show(string.Format("δ���õ�{0}����Դ����!", i.ToString()));
                return;
            }
            if (dR == "") // δ���õ��伸��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�е��伸��!", i.ToString()));
                return;
            }

            string kind = dataGridView1.Rows[i].Cells["��������"].Value.ToString();
            if(kind == "�Ի�����" || kind == "��������") // �Ի��������������
            {
                rN = "0";
                dR = "100";
            }

            try
            {
                int runTime = int.Parse(rT); // ��·ʱ��
                int operateTime = int.Parse(oT); // ����ʱ��
                int resourceNumber = int.Parse(rN); // ��Դ����
                float dropRate = float.Parse(dR); // ���伸��
                // ��������ʱ�� = ��·ʱ�� + ����ʱ�� * ��Դ���� / ���伸��
                singleTaskTime = (int)(runTime + operateTime * resourceNumber * 100 / dropRate);
                dataGridView1.Rows[i].Cells["��������ʱ��(��)"].Value = singleTaskTime.ToString();
            }
            catch(Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}�е�������ʱ��ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// ��������ͬ������ʱ��
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeCoTaskTime(int i)
        {
            string id = dataGridView1.Rows[i].Cells["��������"].Tag.ToString().Trim();
            int sum = 0;

            if(coTaskTable[id] != null) // ����ͬ������
            {
                List<string> idList = coTaskTable[id] as List<string>;
                foreach(string s in idList)
                {
                    List<int> indexList = idTable[s] as List<int>;
                    if(indexList != null)
                    {
                        foreach(int k in indexList)
                        {
                            DataGridViewRow r = dataGridView1.Rows[k];
                            string str = r.Cells["��������ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ��ȡ��������ʱ��
                            if (str == "" || str == "����") // ���Կ�ֵ
                            {
                                continue;
                            }
                            int value = int.Parse(str);
                            sum += value;
                        }
                    }
                }
                // ͬ����������ʱ�� = ��ͬ�ڵ�������ʱ��
                dataGridView1.Rows[i].Cells["ͬ����������ʱ��(��)"].Value = sum.ToString();
            }
            else // ������ͬ������
            {
                dataGridView1.Rows[i].Cells["ͬ����������ʱ��(��)"].Value = dataGridView1.Rows[i].Cells["��������ʱ��(��)"].EditedFormattedValue.ToString();
            }
        }

        /// <summary>
        /// ��������ͬ�������ʱ��
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeOtherCoTaskTime(int i)
        {
            string total = dataGridView1.Rows[i].Cells["ͬ����������ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ͬ����������ʱ��
            string single = dataGridView1.Rows[i].Cells["��������ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ��������ʱ��

            if (total == "" || total == "����") // δ����ͬ����������ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}��ͬ����������ʱ��!", i.ToString()));
                return;
            }
            if (single == "" || single == "����") // δ���õ�������ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�е�������ʱ��!", i.ToString()));
                return;
            }
          
            try
            {
                int totalNumber = int.Parse(total);
                int singleNumber = int.Parse(single);
                // ͬ����������ʱ�� = ͬ����������ʱ�� - ��������ʱ��
                dataGridView1.Rows[i].Cells["ͬ����������ʱ��(��)"].Value = (totalNumber - singleNumber).ToString();
            }
            catch(Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}������ͬ������ʱ��ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// ����ͬ���������
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeCoTaskPlus(int i)
        {
            float coTaskPlus = 0.00f;
            // ������� = (1/(ͬ����������ʱ��/ͬ����������ʱ��)) * 100%
            string str1 = dataGridView1.Rows[i].Cells["ͬ����������ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ͬ����������ʱ��
            string str2 = dataGridView1.Rows[i].Cells["ͬ����������ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ͬ����������ʱ��

            if (str1 == "" || str1 == "����") // δ����ͬ����������ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}��ͬ����������ʱ��!", i.ToString()));
                return;
            }
            if (str2 == "" || str2 == "����") // δ����ͬ����������ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}��ͬ����������ʱ��!", i.ToString()));
                return;
            }      
            
            try
            {
                int otherCoTaskTime = int.Parse(str1);
                int coTaskTime = int.Parse(str2);
                // ������� = (1/(ͬ����������ʱ��/ͬ����������ʱ��)) * 100%
                coTaskPlus = (float) coTaskTime / otherCoTaskTime;
                if(coTaskPlus > 1)
                {
                    dataGridView1.Rows[i].Cells["ͬ���������"].Style.ForeColor = Color.Green;
                }
                if(coTaskPlus > 1.5)
                {
                    coTaskPlus = 1.50f;
                }
                dataGridView1.Rows[i].Cells["ͬ���������"].Value = string.Format("{0:f1}", coTaskPlus);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}��ͬ������������ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// ���㹫����Դ����
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputePublicResourcePlus(int i)
        {
            float publicResourcePlus = 0.00f;
            if(dataGridView1.Rows[i].Cells["������Դ"].Tag == null) // û����Դ
            {
                dataGridView1.Rows[i].Cells["������Դ����"].Value = "1";
                return;
            }
            string requireResource = dataGridView1.Rows[i].Cells["������Դ"].Tag.ToString();
            
            if(publicResourceList.Contains(requireResource)) // �ǹ���������Դ
            {
                float maxValue = 0.00f;
                float totalValue = 0.00f;
                try
                {
                    List<int> resourceList = publicResourceTable[requireResource] as List<int>;
                    foreach (int j in resourceList)
                    {
                        DataGridViewRow r = dataGridView1.Rows[j];
                        string requireNumber = r.Cells["��������"].EditedFormattedValue.ToString();
                        string requireRate = r.Cells["��ü���"].EditedFormattedValue.ToString().TrimEnd('%');
                        int number = int.Parse(requireNumber);
                        float rate = float.Parse(requireRate);
                        //������� = MAX(��������/��ü���) / ��(��������/��ü���)
                        float value = (float)number / rate;
                        totalValue += value;
                        if (value > maxValue)
                        {
                            maxValue = value;
                        }
                    }

                    /*foreach (DataGridViewRow r in dataGridView1.Rows)
                    {
                        if(r.Cells["������Դ"].Tag == null) // û����Դ
                        {                         
                            continue;
                        }
                        string resource = r.Cells["������Դ"].Tag.ToString();
                        if (resource == requireResource)
                        {
                            string requireNumber = r.Cells["��������"].EditedFormattedValue.ToString();
                            string requireRate = r.Cells["��ü���"].EditedFormattedValue.ToString().TrimEnd('%');
                            int number = int.Parse(requireNumber);
                            float rate = float.Parse(requireRate);
                            //������� = MAX(��������/��ü���) / ��(��������/��ü���)
                            float value = (float)number / rate;
                            totalValue += value;
                            if (value > maxValue)
                            {
                                maxValue = value;
                            }
                        }
                    }*/
                    publicResourcePlus = maxValue / totalValue;
                    if(publicResourcePlus < 1)
                    {
                        dataGridView1.Rows[i].Cells["������Դ����"].Style.ForeColor = Color.Red;
                    }
                }
                catch(Exception ex)
                {
                    MessageBox.Show(string.Format("�ڼ����{0}�й�����Դ�������ʱ�����쳣: " + ex.ToString(), i.ToString()));
                }
            }
            else
            {
                publicResourcePlus = 1.00f;    
            }
            dataGridView1.Rows[i].Cells["������Դ����"].Value = publicResourcePlus.ToString();
        }

        /// <summary>
        /// �����������
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeAdditionFlow(int i)
        {
            float additionalFlow = 0.00f;
            string virtualFlow = dataGridView1.Rows[i].Cells["ʵ������"].EditedFormattedValue.ToString().Trim(); // ʵ������
            string publicResourcePlus = dataGridView1.Rows[i].Cells["������Դ����"].EditedFormattedValue.ToString().Trim(); // ������Դ����
            string coTaskPlus = dataGridView1.Rows[i].Cells["ͬ���������"].EditedFormattedValue.ToString().Trim(); // ͬ���������
            string flowPlus = dataGridView1.Rows[i].Cells["��������"].EditedFormattedValue.ToString().Trim(); // ��������

            if (virtualFlow == "" || virtualFlow == "����") // δ����ʵ������
            {
                MessageBox.Show(string.Format("δ���õ�{0}��ʵ������!", i.ToString()));
                return;
            }
            if (virtualFlow == "��") // ʵ������Ϊ��
            {
                dataGridView1.Rows[i].Cells["��������"].Style.ForeColor = Color.Blue;
                dataGridView1.Rows[i].Cells["��������"].Value = "��";
                return;
            }
            if (publicResourcePlus == "" || publicResourcePlus == "����") // δ���ù�����Դ����
            {
                MessageBox.Show(string.Format("δ���õ�{0}�й�����Դ����!", i.ToString()));
                return;
            }
            if (coTaskPlus == "" || coTaskPlus == "����") // δ����ͬ���������
            {
                MessageBox.Show(string.Format("δ���õ�{0}��ͬ���������!", i.ToString()));
                return;
            }
            if (flowPlus == "" || flowPlus == "����") // δ������������
            {
                MessageBox.Show(string.Format("δ���õ�{0}����������!", i.ToString()));
                return;
            } 
            
            try
            {
                float vFlow = float.Parse(virtualFlow);
                float pR = float.Parse(publicResourcePlus);
                float cP = float.Parse(coTaskPlus);
                float fP = float.Parse(flowPlus);
                // �������� = ʵ������*������Դ����*ͬ���������*��������
                additionalFlow = vFlow * pR * cP * fP;
                float downNumber = additionalFlowSectionList[i][1]; // ��ֵ����
                float upNumber = additionalFlowSectionList[i][0]; // ��ֵ����
                if (downNumber == 0.00 && upNumber == 0.00) // δ����������
                {
                    MessageBox.Show(string.Format("δ���õ�{0}�м����������ݵ�������!", i.ToString()));
                    return;
                }
                if (additionalFlow < downNumber)
                {
                    dataGridView1.Rows[i].Cells["��������"].Style.ForeColor = Color.Red;
                }
                else if (additionalFlow > upNumber)
                {
                    dataGridView1.Rows[i].Cells["��������"].Style.ForeColor = Color.Blue;
                    if(additionalFlow > upNumber * 2)
                    {
                        dataGridView1.Rows[i].Cells["��������"].Value = "��";
                        return;
                    }
                }
                else
                {
                    dataGridView1.Rows[i].Cells["��������"].Style.ForeColor = Color.Green;
                }
                dataGridView1.Rows[i].Cells["��������"].Value = string.Format("{0:f1}", additionalFlow);
            }
            catch(Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}�м�������ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }        
        }

        /// <summary>
        /// ������㾭��
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeAdditionalExp(int i)
        {
            float additionalExp = 0.00f;
            string vE = dataGridView1.Rows[i].Cells["��λʱ�侭��(��)"].EditedFormattedValue.ToString().Trim(); // ʵ�ʾ���
            string oT = dataGridView1.Rows[i].Cells["����ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ����ʱ��
            string rN = dataGridView1.Rows[i].Cells["��������"].EditedFormattedValue.ToString().Trim(); // ��������
            string rT = dataGridView1.Rows[i].Cells["��·ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ��·ʱ��
            string kP = dataGridView1.Rows[i].Cells["��������"].EditedFormattedValue.ToString().Trim(); // ��������
            string eP = dataGridView1.Rows[i].Cells["�������"].EditedFormattedValue.ToString().Trim(); // �������

            if (vE == "" || vE == "����") // δ����ʵ�ʾ���
            {
                MessageBox.Show(string.Format("δ���õ�{0}��ʵ�ʾ���!", i.ToString()));
                return;
            }
            if (oT == "" || oT == "����") // δ���ò���ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�в���ʱ��!", i.ToString()));
                return;
            }
            if (rN == "" || rN == "����") // δ������������
            {
                MessageBox.Show(string.Format("δ���õ�{0}����������!", i.ToString()));
                return;
            }
            if (rT == "" || rT == "����") // δ������·ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}����·ʱ��!", i.ToString()));
                return;
            }
            if (eP == "" || eP == "����") // δ���þ������
            {
                MessageBox.Show(string.Format("δ���õ�{0}�о������!", i.ToString()));
                return;
            }
            
            string kind = kP;
            switch(kind)
            {
                case "ǰ������":
                    {
                        kP = "1.00";
                        break;
                    }
                case "�ű�����":
                    {
                        kP = "1.15";
                        break;
                    }
                case "�ɼ�����":
                    {
                        kP = "1.20";
                        break;
                    }
                case "�ռ�����":
                    {
                        kP = "1.15";
                        break;
                    }
                case "ɱ������":
                    {
                        kP = "1.20";
                        break;
                    }
                case "��������":
                    {
                        kP = "1.05";
                        break;
                    }
                case "�Ի�����":
                    {
                        kP = "1.00";
                        break;
                    }
                default:
                    {
                        kP = "1.00";
                        break;
                    }
            }
   
            try
            {
                float virtualExp = float.Parse(vE); // ʵ�ʾ���
                float operateTime = float.Parse(oT); // ����ʱ��
                float requireNumber = float.Parse(rN); // ��������
                float runTime = float.Parse(rT); // ��·ʱ��
                float kindPlus = float.Parse(kP); // ���ͼ���
                float expPlus = float.Parse(eP); // �������
                // ������ = (�����ȼ�����*(����ʱ��*��������+��·ʱ��)/60)*���ͼ���*�������
                additionalExp = (virtualExp * (operateTime * requireNumber + runTime) / 60) * kindPlus * expPlus;
                dataGridView1.Rows[i].Cells["��λʱ���Ǯ(��)"].Value = additionalExp.ToString();
            }
            catch (Exception ex)
            {
            	MessageBox.Show(string.Format("�ڼ����{0}�м��㾭��ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// ����ѹ������
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputePressRate(int i)
        {
            float pressRate = 0.00f;
            string sT = dataGridView1.Rows[i].Cells["��������ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ��������ʱ��
            string cT = dataGridView1.Rows[i].Cells["ͬ����������ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ͬ����������ʱ��

            if (sT == "" || sT == "����") // δ���õ�������ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�е�������ʱ��!", i.ToString()));
                return;
            }
            if (cT == "" || cT == "����") // δ����ͬ����������ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}��ͬ����������ʱ��!", i.ToString()));
                return;
            }

            try
            {
                int singTastTime = int.Parse(sT); // ��������ʱ��
                int coTastTime = int.Parse(cT); // ͬ����������ʱ��
                pressRate = (float) singTastTime * 100 / coTastTime;
                dataGridView1.Rows[i].Cells["ѹ������"].Value = string.Format("{0:f1}%", pressRate);
            }
            catch(Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}��ѹ������ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        ///  ���㽵ѹ����
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeDepressRate(int i)
        {
            float depressRate = 0.00f;
            string oT = dataGridView1.Rows[i].Cells["ͬ����������ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ͬ����������ʱ��
            string cT = dataGridView1.Rows[i].Cells["ͬ����������ʱ��(��)"].EditedFormattedValue.ToString().Trim(); // ͬ����������ʱ��

            if (oT == "" || oT == "����") // δ����ͬ����������ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}��ͬ����������ʱ��!", i.ToString()));
                return;
            }
            if (cT == "" || cT == "����") // δ����ͬ����������ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}��ͬ����������ʱ��!", i.ToString()));
                return;
            }

            try
            {
                int otherCoTastTime = int.Parse(oT); // ͬ����������ʱ��
                int coTastTime = int.Parse(cT); // ͬ����������ʱ��
                depressRate = (float) otherCoTastTime * 100 / coTastTime;
                dataGridView1.Rows[i].Cells["��ѹ����"].Value = string.Format("{0:f1}%", depressRate);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}�н�ѹ����ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeAdditionalRate(int i)
        {
            float additionalRate = 0.00f;
            string dR = dataGridView1.Rows[i].Cells["��ѹ����"].EditedFormattedValue.ToString().TrimEnd('%'); // ��ѹ����

            if (dR == "" || dR == "����") // δ���ý�ѹ����
            {
                MessageBox.Show(string.Format("δ���õ�{0}�н�ѹ����!", i.ToString()));
                return;
            }
           
            try
            {
                float depressRate = float.Parse(dR); // ��ѹ����
                if(depressRate == 0.00) // ��ѹ����Ϊ��
                {
                    dataGridView1.Rows[i].Cells["�������"].Value = "��";
                }
                else
                {
                    additionalRate = 100 / depressRate;
                    dataGridView1.Rows[i].Cells["�������"].Value = string.Format("{0:f1}%", additionalRate);
                }
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}�м������ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// �����׼����
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeNormalExp(int i)
        {
            int normalExp = 0;
            string singleTaskTime = dataGridView1.Rows[i].Cells["��������ʱ��(��)"].Value.ToString(); // ��������ʱ��
            string normalTime = dataGridView1.Rows[i].Cells["��׼ʱ��(��)"].Value.ToString(); // ��׼ʱ��
            string singleTaskExp = dataGridView1.Rows[i].Cells["��λʱ�侭��(��)"].Value.ToString(); // ��λʱ�侭��

            if (singleTaskTime == "" || singleTaskTime == "����") // δ���õ�������ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�е�������ʱ��!", i.ToString()));
                return;
            }

            if (normalTime == "" || normalTime == "����") // δ���ñ�׼ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�б�׼ʱ��!", i.ToString()));
                return;
            }

            if (singleTaskExp == "" || singleTaskExp == "����") // δ���õ�λʱ�侭��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�е�λʱ�侭��!", i.ToString()));
                return;
            }

            try
            {
                int singleTT = int.Parse(singleTaskTime);
                int normalT = int.Parse(normalTime);
                int singleTE = int.Parse(singleTaskExp);

                // ��׼����=max(��׼ʱ��, ʵ��ʱ��) * ������λʱ�侭��
                if (singleTT > normalT)
                {
                    normalExp = (int)(singleTT * singleTE / 60);
                }
                else
                {
                    normalExp = (int)(normalT * singleTE / 60);
                }

                totalNormalExp += normalExp; // �����׼�����ܺ�
                dataGridView1.Rows[i].Cells["��׼����"].Value = normalExp.ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}�б�׼����ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// ����ʵ�ʾ���
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeActualExp(int i)
        {
            int actualExp = 0;
            string singleTaskTime = dataGridView1.Rows[i].Cells["��������ʱ��(��)"].Value.ToString(); // ��������ʱ��
            string singleTaskExp = dataGridView1.Rows[i].Cells["��λʱ�侭��(��)"].Value.ToString(); // ��λʱ�侭��

            if (singleTaskTime == "" || singleTaskTime == "����") // δ���õ�������ʱ��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�е�������ʱ��!", i.ToString()));
                return;
            }

            if (singleTaskExp == "" || singleTaskExp == "����") // δ���õ�λʱ�侭��
            {
                MessageBox.Show(string.Format("δ���õ�{0}�е�λʱ�侭��!", i.ToString()));
                return;
            }

            try
            {
                int singleTT = int.Parse(singleTaskTime);
                int singleTE = int.Parse(singleTaskExp);

                // ʵ�ʾ��� = ʵ��ʱ�� * ��λʱ�侭��
                actualExp = (int)(singleTT * singleTE / 60);
                totalActualExp += actualExp; // ����ʵ�ʾ����ܺ�
                dataGridView1.Rows[i].Cells["ʵ�ʾ���"].Value = actualExp.ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}��ʵ�ʾ���ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// ���㾭���ܺ�
        /// </summary>
        private void ComputeTotalExp()
        {
            int totalNormalExp = 0; // ��׼�����ܺ�
            int totalActualExp = 0; // ʵ�ʾ����ܺ�

            try
            {
                foreach (DataGridViewRow r in dataGridView1.Rows)
                {

                    if ((bool)r.Cells["�Զ�����"].EditedFormattedValue == true)
                    {
                        // �����׼�����
                        string normalExp = r.Cells["��׼����"].EditedFormattedValue.ToString();
                        if (normalExp == "" || normalExp == "����")
                        {
                            continue;
                        }
                        int normalE = int.Parse(normalExp);
                        totalNormalExp += normalE;

                        // ����ʵ�ʾ����
                        string actualExp = r.Cells["ʵ�ʾ���"].EditedFormattedValue.ToString();
                        if (actualExp == "" || actualExp == "����")
                        {
                            continue;
                        }
                        int actualE = int.Parse(actualExp);
                        totalActualExp += actualE;
                    }
                }

                this.totalNormalExp = totalNormalExp;
                this.totalActualExp = totalActualExp;
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڼ��㾭���ܺ�ʱ�����쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// �������·��侭��
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeReallocExp(int i)
        {
            int reallocExp = 0; // ���·��侭��

            try
            {
                string actualExperiment = dataGridView1.Rows[i].Cells["ʵ�ʾ���"].EditedFormattedValue.ToString(); // ʵ�ʾ���
                if (actualExperiment == "" || actualExperiment == "����")
                {
                    MessageBox.Show(string.Format("δ���õ�{0}��ʵ�ʾ���!", i.ToString()));
                    return;
                }
                int actualEx = int.Parse(actualExperiment);

                // ���·��侭�� = (���б�׼����� / ����ʵ�ʾ����) * ������ʵ�ʾ���
                reallocExp = (int)(((float)totalNormalExp / totalActualExp) * actualEx);
                dataGridView1.Rows[i].Cells["���·��侭��"].Value = reallocExp.ToString();
            }
            catch(Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}�����·��侭��ʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// �������·����Ǯ
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeReallocMoney(int i)
        {
            int reallocMoney = 0; // ���·����Ǯ
            int totalNormalMoney = 0; // ��׼��Ǯ�ܺ�
            float totalTaskRate = 0; // ����ȼ�������

            try
            {
                string level = dataGridView1.Rows[i].Cells["�ȼ�"].EditedFormattedValue.ToString().Trim();

                foreach (DataGridViewRow r in dataGridView1.Rows)
                {
                    if ((bool)r.Cells["�Զ�����"].EditedFormattedValue == true)
                    {
                        if(r.Cells["�ȼ�"].EditedFormattedValue.ToString().Trim() == level) // ͬ�ȼ�������
                        {
                            // ��������ȼ�������
                            string kind = r.Cells["��������"].Value.ToString(); // �������� 
                            float rewardRate = 1f;
                            switch(kind)
                            {
                                case "ǰ������":
                                    {
                                        break;
                                    }
                                case "�ű�����":
                                    {
                                        rewardRate = 1.15f;
                                        break;
                                    }
                                case "�ɼ�����":
                                    {
                                        rewardRate = 1.15f;
                                        break;
                                    }
                                case "�ռ�����":
                                    {
                                        rewardRate = 1.2f;
                                        break;
                                    }
                                case "ɱ������":
                                    {
                                        rewardRate = 1.2f;
                                        break;
                                    }
                                case "��������":
                                    {
                                        rewardRate = 1.05f;
                                        break;
                                    }
                                case "�Ի�����":
                                    {
                                        break;
                                    }
                                case "�߼�����":
                                    {
                                        rewardRate = 1.3f;
                                        break;
                                    }
                                case "��Ӣ����":
                                    {
                                        rewardRate = 1.5f;
                                        break;
                                    }
                            }

                            string singleTaskTime = r.Cells["��������ʱ��(��)"].EditedFormattedValue.ToString(); // ��������ʱ��
                            if(singleTaskTime == "" || singleTaskTime == "����")
                            {
                                continue;
                            }
                            int singleTT = int.Parse(singleTaskTime);

                            // ��������ȼ�����
                            // ����ȼ����� = �������ͼ��� * ��������ʱ��
                            float taskRate = rewardRate * singleTT;
                            totalTaskRate += taskRate;
                        }
                    }
                }

                string theKind = dataGridView1.Rows[i].Cells["��������"].Value.ToString(); // �������� 
                float theRewardRate = 1f;
                switch (theKind)
                {
                    case "ǰ������":
                        {
                            break;
                        }
                    case "�ű�����":
                        {
                            theRewardRate = 1.15f;
                            break;
                        }
                    case "�ɼ�����":
                        {
                            theRewardRate = 1.15f;
                            break;
                        }
                    case "�ռ�����":
                        {
                            theRewardRate = 1.2f;
                            break;
                        }
                    case "ɱ������":
                        {
                            theRewardRate = 1.2f;
                            break;
                        }
                    case "��������":
                        {
                            theRewardRate = 1.05f;
                            break;
                        }
                    case "�Ի�����":
                        {
                            break;
                        }
                    case "�߼�����":
                        {
                            theRewardRate = 1.3f;
                            break;
                        }
                    case "��Ӣ����":
                        {
                            theRewardRate = 1.5f;
                            break;
                        }
                }

                string theSingleTaskTime = dataGridView1.Rows[i].Cells["��������ʱ��(��)"].EditedFormattedValue.ToString(); // ��������ʱ��
                if (theSingleTaskTime == "" || theSingleTaskTime == "����")
                {
                    MessageBox.Show(string.Format("δ���õ�{0}�е�������ʱ��!", i.ToString()));
                    return;
                }
                int theSTT = int.Parse(theSingleTaskTime);

                // ��������ȼ�����
                // ����ȼ����� = �������ͼ��� * ��������ʱ��
                float theTaskRate = theRewardRate * theSTT;
                int l = int.Parse(level);
                if(l < taskData.TotalMoneyArray.Length)
                {
                    totalNormalMoney = taskData.TotalMoneyArray[l];
                }

                // ���·����Ǯ = (����ͬ�ȼ������׼��Ǯ�� / ����ͬ�ȼ����������) * ���������
                reallocMoney = (int)((totalNormalMoney / totalTaskRate) * theTaskRate);
                dataGridView1.Rows[i].Cells["���·����Ǯ"].Value = reallocMoney.ToString();
                dataGridView1.Rows[i].Cells["���·����Ǯ"].Tag = reallocMoney.ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}�����·����Ǯʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// ���㾭����ֵ
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeFinalExp(int i)
        {
            int finalExp = 0; // ������ֵ

            try
            {
                string normalExp = dataGridView1.Rows[i].Cells["��׼����"].EditedFormattedValue.ToString(); // ��׼����
                string reallocExp = dataGridView1.Rows[i].Cells["���·��侭��"].EditedFormattedValue.ToString(); // ���·��侭��
                string flowPlus = dataGridView1.Rows[i].Cells["��������"].EditedFormattedValue.ToString(); // ��������
                string expPlus = dataGridView1.Rows[i].Cells["�������"].EditedFormattedValue.ToString(); // �������
                string id = dataGridView1.Rows[i].Cells["��������"].Tag.ToString(); // ����id

                if (normalExp == "" || normalExp == "����")
                {
                    MessageBox.Show(string.Format("δ���õ�{0}�б�׼����!", i.ToString()));
                    return;
                }

                if (reallocExp == "" || reallocExp == "����")
                {
                    MessageBox.Show(string.Format("δ���õ�{0}�����·��侭��!", i.ToString()));
                    return;
                }

                if (flowPlus == "" || flowPlus == "����")
                {
                    MessageBox.Show(string.Format("δ���õ�{0}����������!", i.ToString()));
                    return;
                }

                if (expPlus == "" || expPlus == "����")
                {
                    MessageBox.Show(string.Format("δ���õ�{0}�о������!", i.ToString()));
                    return;
                }

                int normalE = int.Parse(normalExp);
                int reallocE = int.Parse(reallocExp);
                float flowP = float.Parse(flowPlus);
                float expP = float.Parse(expPlus);

                // ������ֵ = max(������, ���·��侭��) * �������� * �������
                if(normalE > reallocE)
                {
                    finalExp = (int)(normalE * flowP * expP);
                }
                else
                {
                    finalExp = (int)(reallocE * expP * expP);
                }
                finalExp = (int)((finalExp + 5) / 10) * 10; // ʮλ��������

                if(finalExpTable[id] == null)
                {
                    finalExpTable[id] = finalExp.ToString();
                }
                else
                {
                    int totalExp = int.Parse(finalExpTable[id].ToString());
                    totalExp += finalExp;
                    finalExpTable[id] = totalExp.ToString();
                    dataGridView1.Rows[i].Cells["������ֵ"].Style.ForeColor = Color.Gray;
                }
                dataGridView1.Rows[i].Cells["������ֵ"].Value = finalExp.ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}�о�����ֵʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// �����Ǯ��ֵ
        /// </summary>
        /// <param name="i">�����</param>
        private void ComputeFinalMoney(int i)
        {
            int finalMoney = 0; // ��Ǯ��ֵ

            try
            {
                string reallocMoney = dataGridView1.Rows[i].Cells["���·����Ǯ"].Tag.ToString(); // ���·����Ǯ
                string flowPlus = dataGridView1.Rows[i].Cells["��������"].EditedFormattedValue.ToString(); // ��������
                string expPlus = dataGridView1.Rows[i].Cells["�������"].EditedFormattedValue.ToString(); // �������
                string id = dataGridView1.Rows[i].Cells["��������"].Tag.ToString(); // ����id

                if (reallocMoney == "")
                {
                    MessageBox.Show(string.Format("δ���õ�{0}�����·����Ǯ!", i.ToString()));
                    return;
                }

                if (flowPlus == "" || flowPlus == "����")
                {
                    MessageBox.Show(string.Format("δ���õ�{0}����������!", i.ToString()));
                    return;
                }

                if (expPlus == "" || expPlus == "����")
                {
                    MessageBox.Show(string.Format("δ���õ�{0}�о������!", i.ToString()));
                    return;
                }

                int reallocM = int.Parse(reallocMoney);
                float flowP = float.Parse(flowPlus);
                float expP = float.Parse(expPlus);

                // ��Ǯ��ֵ = ���·����Ǯ * �������� * �������            
                finalMoney = (int)(reallocM * expP * expP);               

                if (finalMoneyTable[id] == null)
                {
                    finalMoneyTable[id] = finalMoney.ToString();
                }
                else
                {
                    int totalMoney = int.Parse(finalMoneyTable[id].ToString());
                    totalMoney += finalMoney;
                    finalMoneyTable[id] = totalMoney.ToString();
                    dataGridView1.Rows[i].Cells["��Ǯ��ֵ"].Style.ForeColor = Color.Gray;
                }
                dataGridView1.Rows[i].Cells["��Ǯ��ֵ"].Value = finalMoney.ToString();
                dataGridView1.Rows[i].Cells["��Ǯ��ֵ"].Tag = finalMoney.ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("�ڼ����{0}�н�Ǯ��ֵʱ�����쳣: " + ex.ToString(), i.ToString()));
            }
        }

        /// <summary>
        /// ������д����ͽ�Ǯ����
        /// </summary>
        /// <param name="i"></param>
        private void CoordinateTotal(int i)
        {
            string id = dataGridView1.Rows[i].Cells["��������"].Tag.ToString(); // ����id
            if(finalExpTable[id] == null || finalMoneyTable[id] == null) // ��ֵ
            {
                dataGridView1.Rows[i].Cells["������ֵ"].Value = "����";
                dataGridView1.Rows[i].Cells["��Ǯ��ֵ"].Value = "����";
                return;
            }
            string totalExp = finalExpTable[id].ToString();
            int totalMoney = int.Parse(finalMoneyTable[id].ToString());
            totalMoney = ((int)((totalMoney + 5) / 10)) * 10;

            dataGridView1.Rows[i].Cells["������ֵ"].Value = totalExp.ToString();
            dataGridView1.Rows[i].Cells["��Ǯ��ֵ"].Value = totalMoney.ToString();
            dataGridView1.Rows[i].Cells["��Ǯ��ֵ"].Tag = totalMoney.ToString();
        }

        /// <summary>
        /// ͬ������ͬ���������ֵ��Ϣ
        /// </summary>
        /// <param name="index">�����</param>
        /// <param name="taskName">��������</param>
        /// <param name="value">��ֵ��Ϣ</param>
        /// <param name="upward">�Ƿ����ϼ���</param>
        private void CoordinateData(int rowIndex, int columnIndex, string taskID, string value, bool upward)
        {
            if(rowIndex < 0 || rowIndex >= dataGridView1.Rows.Count) // ��Ч��
            {
                return;
            }

            if (dataGridView1.Rows[rowIndex].Cells["��������"].Tag.ToString() == taskID)
            {
                dataGridView1.Rows[rowIndex].Cells[columnIndex].Value = value;
                if(fontChangable) // ��������仯
                {
                    dataGridView1.Rows[rowIndex].Cells[columnIndex].Style.Font = boldFont;
                }
                if (upward) // ���ϼ���
                {             
                    CoordinateData(rowIndex - 1, columnIndex, taskID, value, upward);                    
                }
                else // ���¼���
                {                
                    CoordinateData(rowIndex + 1, columnIndex, taskID, value, upward);               
                }
            }
        }

        /// <summary>
        /// ͬ������ͬ����Դ��������Ϣ
        /// </summary>
        /// <param name="resourceName">��Դ����</param>
        /// <param name="value">������Ϣ</param>
        private void CoordinateResourceNumber(string resourceName, string value)
        {
            foreach(DataGridViewRow r in dataGridView1.Rows)
            {
                if(r.Cells["������Դ"].Tag != null)
                {
                    if (r.Cells["������Դ"].Tag.ToString() == resourceName)
                    {
                        if (r.Cells["��Դ����"].Value != null)
                        {
                            if (r.Cells["��Դ����"].Value.ToString() != value)
                            {
                                r.Cells["��Դ����"].Value = value;
                                if(fontChangable) // ��������仯
                                {
                                    r.Cells["��Դ����"].Style.Font = boldFont;
                                }
                            }
                        }
                        else
                        {
                            r.Cells["��Դ����"].Value = value;
                        }
                    }
                }          
            }
        }

        /// <summary>
        /// ͬ������ͬ����Դˢ��ʱ����Ϣ
        /// </summary>
        /// <param name="resourceName">��Դ����</param>
        /// <param name="value">ˢ��ʱ��</param>
        private void CoordinateRefreshTime(string resourceName, string value)
        {
            foreach (DataGridViewRow r in dataGridView1.Rows)
            {
                if (r.Cells["������Դ"].Tag != null)
                {
                    if (r.Cells["������Դ"].Tag.ToString() == resourceName)
                    {
                        if (r.Cells["ˢ��ʱ��(��)"].Value != null)
                        {
                            if (r.Cells["ˢ��ʱ��(��)"].Value.ToString() != value)
                            {
                                r.Cells["ˢ��ʱ��(��)"].Value = value;
                                if (fontChangable) // ��������仯
                                {
                                    r.Cells["ˢ��ʱ��(��)"].Style.Font = boldFont;
                                }
                            }
                        }
                        else
                        {
                            r.Cells["ˢ��ʱ��(��)"].Value = value;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// ���ʵ����������
        /// </summary>
        /// <param name="maxValue">ʵ����������</param>
        /// <param name="minValue">ʵ����������</param>
        private void FillVirtualFlowSection(float maxValue, float minValue)
        {
            foreach(float[] f in virtualFlowSectionList)
            {
                f[0] = maxValue;
                f[1] = minValue;
            }

            string value = string.Format("[{0}, {1}]", minValue, maxValue);
            foreach(DataGridViewRow r in dataGridView1.Rows)
            {
                string kind = r.Cells["��������"].Value.ToString();
                if (kind == "�ű�����" || kind == "�ռ�����" || kind == "�ɼ�����" || kind == "ɱ������") // �ж���������
                {
                    r.Cells["ʵ������"].Value = value;
                }
                else
                {
                    r.Cells["ʵ������"].Value = "����";
                }
            }
        }

        /// <summary>
        /// ��������������
        /// </summary>
        /// <param name="maxValue">������������</param>
        /// <param name="minValue">������������</param>
        private void FillAdditionalFlowSection(float maxValue, float minValue)
        {
            foreach (float[] f in additionalFlowSectionList)
            {
                f[0] = maxValue;
                f[1] = minValue;
            }

            string value = string.Format("[{0}, {1}]", minValue, maxValue);
            foreach (DataGridViewRow r in dataGridView1.Rows)
            {
                string kind = r.Cells["��������"].Value.ToString();
                if (kind == "�ű�����" || kind == "�ռ�����" || kind == "�ɼ�����" || kind == "ɱ������") // �ж���������
                {
                    r.Cells["��������"].Value = value;
                }
                else
                {
                    r.Cells["��������"].Value = "����";
                }
            }
        }

        /// <summary>
        /// ת����Ǯ�ı�ʾ��ʽ
        /// </summary>
        /// <param name="n">ͭ������</param>
        /// <returns>��Ǯ�ַ���</returns>
        private string ConvertMoney(int n)
        {
            string money = n.ToString();
            string result = "0��0��0ͭ";

            if(money.Length < 3)
            {
                result = string.Format("0��0��{0}ͭ", money);
            }
            else if(money.Length < 5)
            {
                result = string.Format("0��{0}��{1}ͭ", money.Substring(0, money.Length - 2),
                    money.Substring(money.Length - 2));
            }
            else
            {
                result = string.Format("{0}��{1}��{2}ͭ", money.Substring(0, money.Length - 4),
                    money.Substring(money.Length - 4, 2), money.Substring(money.Length - 2));
            }

            return result;
        }

        /// <summary>
        /// ת����Ǯ�ı�ʾ��ʽ
        /// </summary>
        /// <param name="n">��Ǯ�ַ���</param>
        /// <returns>ͭ������</returns>
        private int ConvertMoney(string n)
        {
            string gold = "0";
            string silver = "0";
            string copper = "0";
            int goldNumber = 0;
            int silverNumber = 0;
            int copperNumber = 0;

            string regexPattern = @"(?<gold>\d*)��(?<silver>\d*)��(?<copper>\d*)ͭ";
            Regex r = new Regex(regexPattern);

            if (r.IsMatch(n))
            {
                gold = r.Match(n).Result("${gold}");
                goldNumber = int.Parse(gold);
                silver = r.Match(n).Result("${silver}");
                silverNumber = int.Parse(silver);
                copper = r.Match(n).Result("${copper}");
                copperNumber = int.Parse(copper);
            }

            return goldNumber * 10000 + silverNumber * 100 + copperNumber;
        }

        /// <summary>
        /// ȫ��ѡ��ȫ�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonItem4_Click(object sender, EventArgs e)
        {
            startEdit = false;
            foreach(DataGridViewRow r in dataGridView1.Rows)
            {
                r.Cells["�Զ�����"].Value = selectAll;
                dataGridView1.CommitEdit(DataGridViewDataErrorContexts.Commit);
            }
            startEdit = true;
            if(selectAll) // ȫ��ȡ��
            {
                buttonItem4.Text = "ȫ��ȡ��";
                selectAll = false;
            }
            else // ȫ��ѡ��
            {
                buttonItem4.Text = "ȫ��ѡ��";
                selectAll = true;
            }
        }

        /// <summary>
        /// ��ʾ�Ҽ��˵�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void dataGridView1_CellMouseDown(object sender, DataGridViewCellMouseEventArgs e)
        {
            if(dataGridView1.SelectedCells == null)
            {
                return;
            }

            DataGridViewCell currentCell = dataGridView1.SelectedCells[0];
            string kind = dataGridView1.Rows[currentCell.RowIndex].Cells["��������"].Value.ToString().Trim();

            if (e.Button == MouseButtons.Right) // ����Ч���ϵ���Ҽ�
            {
                bool setDefault = defaultColumnList.Contains(currentCell.ColumnIndex);
                bool autoFill = fillableColumnList.Contains(currentCell.ColumnIndex);
                bool savable = savableColumnList.Contains(currentCell.ColumnIndex);

                setDefaultToolStripMenuItem.Enabled = setDefault;
                autoFillToolStripMenuItem.Enabled = autoFill;
                hideColumnToolStripMenuItem.Enabled = savable;
            
                computeCell = currentCell;
                currentCell.ContextMenuStrip = contextMenuStrip1;             
            }
        }

        /// <summary>
        /// ��ʼ���м���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonItem5_Click(object sender, EventArgs e)
        {
            finalMoneyTable.Clear(); // ��ս�Ǯ��ֵ��
            finalExpTable.Clear(); // ��վ�����ֵ��
            
            // ��ʼ��������
            pForm.TitleText = "��ֵ������(��һ�׶�)";
            nodeTotal = dataGridView1.Rows.Count;
            nodeCount = 0;
            pForm.SetMaxValue(nodeTotal);
            pForm.Visible = true;
            startEdit = false;
            foreach(DataGridViewRow r in dataGridView1.Rows) // ��һ�α���
            {
                nodeCount++;
                pForm.SetValue(nodeCount);
                string taskType = r.Cells["��������"].Value.ToString();
                
                if((bool)r.Cells["�Զ�����"].EditedFormattedValue == true) // �Ƿ���ѡ��
                {
                    if (taskType == "ǰ������") // ���ü�������
                    {
                        // ���ʵ������
                        r.Cells["ʵ������"].Value = "����";
                        // ��Ӽ�������
                        r.Cells["��������"].Value = "����";
                        // ��ӵ�λʱ�侭��
                        r.Cells["��λʱ�侭��(��)"].Value = "����";
                        // ��ӵ�λʱ���Ǯ
                        r.Cells["��λʱ���Ǯ(��)"].Value = "����";
                        // ���ͬ����������ʱ��
                        r.Cells["ͬ����������ʱ��(��)"].Value = "����";
                        // ���ͬ����������ʱ��
                        r.Cells["ͬ����������ʱ��(��)"].Value = "����"; 
                        // ���ѹ������
                        r.Cells["ѹ������"].Value = "����";
                        // ��ӽ�ѹ����
                        r.Cells["��ѹ����"].Value = "����";
                        // ��Ӽ������
                        r.Cells["�������"].Value = "����";
                        // ��ӹ�����Դ����
                        r.Cells["������Դ����"].Value = "����";
                        // ���ͬ���������
                        r.Cells["ͬ���������"].Value = "����";
                        // ��ӵ�������ʱ��
                        r.Cells["��������ʱ��(��)"].Value = "����";
                        // ��ӱ�׼����
                        r.Cells["��׼����"].Value = "����";
                        // ���ʵ�ʾ���
                        r.Cells["ʵ�ʾ���"].Value = "����";
                        // ������·��侭��
                        r.Cells["���·��侭��"].Value = "����";
                        // ��Ӿ�����ֵ
                        r.Cells["������ֵ"].Value = "����";
                        // ������·����Ǯ
                        r.Cells["���·����Ǯ"].Value = "����";
                        // ��ӽ�Ǯ��ֵ
                        r.Cells["��Ǯ��ֵ"].Value = "����";
                    }
                    else if (taskType == "��������" || taskType == "�Ի�����") // ��Ҫ���㾭�鼰��Ǯ
                    {
                        // ���ʵ������
                        r.Cells["ʵ������"].Value = "����";
                        // ��Ӽ�������
                        r.Cells["��������"].Value = "����";
                        // ��ӹ�����Դ����
                        r.Cells["������Դ����"].Value = "����";

                        // ���㵥��������·ʱ��
                        ComputeSingleTaskTime(r.Index);
                        // �����׼����
                        ComputeNormalExp(r.Index);
                        // ����ʵ�ʾ���
                        ComputeActualExp(r.Index);
                    }
                    else // ��Ҫ��������
                    {
                        // ��������
                        ComputeVirtualFlow(r.Index);
                        // ���㵥��������·ʱ��
                        ComputeSingleTaskTime(r.Index);
                        // ���㹫����Դ����
                        ComputePublicResourcePlus(r.Index);
                        // �����׼����
                        ComputeNormalExp(r.Index);
                        // ����ʵ�ʾ���
                        ComputeActualExp(r.Index);
                    }
                }
            }

            // ComputeTotalExp(); // ���㾭���ܺ�

            // ��ʼ��������
            pForm.TitleText = "��ֵ������(�ڶ��׶�)";
            nodeCount = 0;
            pForm.Visible = true;
            foreach(DataGridViewRow r in dataGridView1.Rows) // �ڶ��α���
            {
                nodeCount++;
                pForm.SetValue(nodeCount);
                string taskType = r.Cells["��������"].Value.ToString();

                if ((bool)r.Cells["�Զ�����"].EditedFormattedValue == true) // �Ƿ���ѡ��
                {
                    if (taskType == "ǰ������") // ���ü�������
                    {

                    }
                    else if(taskType == "��������" || taskType == "�Ի�����") // ��Ҫ�����Ǯ������
                    {
                        // ��������ͬ������ʱ��
                        ComputeCoTaskTime(r.Index);
                        // ��������ͬ������ʱ��
                        ComputeOtherCoTaskTime(r.Index);
                        // ����ͬ������������
                        ComputeCoTaskPlus(r.Index);
                        // ����ѹ������
                        ComputePressRate(r.Index);
                        // ���㽵ѹ����
                        ComputeDepressRate(r.Index);
                        // ����������
                        ComputeAdditionalRate(r.Index);
                        // �������·��侭��
                        ComputeReallocExp(r.Index);
                        // ���㾭����ֵ
                        ComputeFinalExp(r.Index);
                        // �������·����Ǯ
                        ComputeReallocMoney(r.Index);
                        // �����Ǯ��ֵ
                        ComputeFinalMoney(r.Index);
                    }
                    else // ��Ҫ��������
                    {
                        // ��������ͬ������ʱ��
                        ComputeCoTaskTime(r.Index);
                        // ��������ͬ������ʱ��
                        ComputeOtherCoTaskTime(r.Index);
                        // ����ͬ������������
                        ComputeCoTaskPlus(r.Index);
                        // �����������
                        ComputeAdditionFlow(r.Index);
                        // ����ѹ������
                        ComputePressRate(r.Index);
                        // ���㽵ѹ����
                        ComputeDepressRate(r.Index);
                        // ����������
                        ComputeAdditionalRate(r.Index);
                        // �������·��侭��
                        ComputeReallocExp(r.Index);
                        // ���㾭����ֵ
                        ComputeFinalExp(r.Index);
                        // �������·����Ǯ
                        ComputeReallocMoney(r.Index);
                        // �����Ǯ��ֵ
                        ComputeFinalMoney(r.Index);
                    }
                }
            }

            foreach(DataGridViewRow r in dataGridView1.Rows) // �����α���
            {
                if ((bool)r.Cells["�Զ�����"].EditedFormattedValue == true) // �Ƿ���ѡ��
                {
                    CoordinateTotal(r.Index);
                }
            }

            startEdit = true;
        }

        /// <summary>
        /// ����Ԫ��������¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if(dataGridView1.SelectedCells == null)
            {
                return;
            }
            DataGridViewCell currentCell = dataGridView1.SelectedCells[0];
        }

        /// <summary>
        /// �ظ���Ԫ��Ĭ��ֵ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void setDefaultToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(computeCell.Tag != null)
            {
                string defaultValue = computeCell.Tag.ToString(); // Ĭ��ֵ
                computeCell.Value = defaultValue;
            }
        }

        /// <summary>
        /// ��鵥Ԫ���������Ч��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if(!startEdit) // �û���δ��ʼ�༭
            {
                return;
            }

            DataGridViewCell c = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex]; // ��ǰ��Ԫ��
            bool fontBold = true; // �Ƿ������Ϊ����

            switch(e.ColumnIndex)
            {
                case 0: // ͬ��ͬһ�����ѡ����Ϣ
                    {
                        string taskName = dataGridView1.Rows[e.RowIndex].Cells["��������"].Tag.ToString();
                        string value = c.Value.ToString();
                        startEdit = false;
                        CoordinateData(e.RowIndex - 1, 0, taskName, value, true);
                        CoordinateData(e.RowIndex + 1, 0, taskName, value, false);
                        startEdit = true;
                        break;
                    }
                case 7: // ͬ�����еĵȼ���Ϣ�����޸ĺ͵ȼ��йص���Ϣ
                    {
                        string taskName = dataGridView1.Rows[e.RowIndex].Cells["��������"].Tag.ToString();
                        string value = c.Value.ToString();
                        startEdit = false;
                        CoordinateData(e.RowIndex - 1, 7, taskName, value, true);
                        CoordinateData(e.RowIndex + 1, 7, taskName, value, false);
                        startEdit = true;

                        int level = int.Parse(dataGridView1.Rows[e.RowIndex].Cells["�ȼ�"].Value.ToString().Trim()); // �ȼ�
                        // �޸ĵ�λʱ�侭��
                        string kind = dataGridView1.Rows[e.RowIndex].Cells["��������"].Value.ToString(); // ��������                                             
                        int exp = 0;

                        if(level < taskData.ExpArray.Length)
                        {
                            exp = taskData.ExpArray[level];                           
                            if (kind == "�Ի�����")
                            {
                            }
                            else if (kind == "��������")
                            {
                                exp = (int)(exp * 1.05);
                            }
                            else if (kind == "�ɼ�����" || kind == "�ű�����")
                            {
                                exp = (int)(exp * 1.15);
                            }
                            else if (kind == "ɱ������" || kind == "�ռ�����")
                            {
                                exp = (int)(exp * 1.2);
                            }        
                        }
                        dataGridView1.Rows[e.RowIndex].Cells["��λʱ�侭��(��)"].Value = exp.ToString();
                        // �޸ĵ�λʱ���Ǯ
                        int money = 0;
                        if (level < taskData.MoneyArray.Length)
                        {
                            money = taskData.MoneyArray[level];
                            if (kind == "�Ի�����")
                            {
                            }
                            else if (kind == "��������")
                            {
                                money = (int)(money * 1.05);
                            }
                            else if (kind == "�ɼ�����" || kind == "�ű�����")
                            {
                                money = (int)(money * 1.15);
                            }
                            else if (kind == "ɱ������" || kind == "�ռ�����")
                            {
                                money = (int)(money * 1.2);
                            }    
                        }
                        dataGridView1.Rows[e.RowIndex].Cells["��λʱ���Ǯ(��)"].Value = money.ToString();
                        dataGridView1.Rows[e.RowIndex].Cells["��λʱ���Ǯ(��)"].Tag = money.ToString();
                        // �޸ı�׼ʱ��
                        int normalTime = 0;
                        if (level < taskData.NormalTimeArray5.Length)
                        {
                            normalTime = taskData.NormalTimeArray5[level];
                        }
                        dataGridView1.Rows[e.RowIndex].Cells["��׼ʱ��(��)"].Value = normalTime.ToString();  
                        break;
                    }
                case 11: // ͬ�����е���Դ������Ϣ
                    {
                        if(dataGridView1.Rows[e.RowIndex].Cells["������Դ"].Tag != null) // ����Դ
                        {
                            string resourceName = dataGridView1.Rows[e.RowIndex].Cells["������Դ"].Tag.ToString();
                            if(publicResourceList.Contains(resourceName)) // �ǹ�����Դ
                            {
                                string value = dataGridView1.Rows[e.RowIndex].Cells["��Դ����"].Value.ToString();
                                startEdit = false;
                                CoordinateResourceNumber(resourceName, value);
                                startEdit = true;
                            }        
                        }       
                        break;
                    }
                case 12: // ���ˢ��ʱ�����Ч��
                    {
                        if(c.Tag != null) // Ĭ��ֵ�ǿ�
                        {
                            string defaultValue = c.Tag.ToString();
                            string value = c.Value.ToString();

                            if(defaultValue != "" && value != "") // ֵ��Ϊ��
                            {
                                try
                                {
                                    int dV = int.Parse(defaultValue); // Ĭ��ֵ
                                    int v = int.Parse(value); // ��ǰֵ
                                    if(v < dV)
                                    {
                                        if (dataGridView1.Rows[c.RowIndex].Cells["��������"].EditedFormattedValue.ToString() == "�ɼ�����")
                                        {
                                            if(v < 90)
                                            {
                                                if(v != 1)
                                                {
                                                    DialogResult result = MessageBox.Show("�Ƿ��������", "����ֵ����ҪôΪ1, Ҫô���ܵ���90!",
                                                        MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                                                    if(result == DialogResult.Yes)
                                                    {
                                                        startEdit = false;
                                                        c.Value = 1;
                                                        startEdit = true;
                                                        return;
                                                    }
                                                }
                                            }
                                        }
                                        else
                                        {
                                            DialogResult result = MessageBox.Show("�Ƿ��������", string.Format("����ֵ���ܵ���������ֵ{0}!", defaultValue),
                                                MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                                            if (result == DialogResult.Yes)
                                            {
                                                startEdit = false;
                                                c.Value = defaultValue;
                                                startEdit = true;
                                                return;
                                            }
                                        }
                                    }
                                }
                                catch(Exception ex)
                                {
                                    MessageBox.Show(string.Format("��ⵥԪ��[{0}, {1}]������Ч��ʱ�����쳣: " + ex.ToString(),
                                        e.RowIndex.ToString(), e.ColumnIndex.ToString()));
                                }

                                if(dataGridView1.Rows[e.RowIndex].Cells["������Դ"].Tag != null) // ��������Դ
                                {
                                    string requireResource = dataGridView1.Rows[e.RowIndex].Cells["������Դ"].Tag.ToString(); // ������Դ
                                    if(publicResourceList.Contains(requireResource)) // �ǹ�����Դ
                                    {
                                        startEdit = false;
                                        CoordinateRefreshTime(requireResource, value);
                                        startEdit = true;
                                    }
                                }
                            }                      
                        }
                        break;
                    }
                case 13: // ������ʱ�����Ч��
                    {
                        if (c.Tag != null) // Ĭ��ֵ�ǿ�
                        {
                            string defaultValue = c.Tag.ToString();
                            string value = c.Value.ToString();

                            if (defaultValue != "" && value != "") // ֵ��Ϊ��
                            {
                                try
                                {
                                    int dV = int.Parse(defaultValue); // Ĭ��ֵ
                                    int v = int.Parse(value); // ��ǰֵ
                                    if (v < dV)
                                    {
                                        if (dataGridView1.Rows[c.RowIndex].Cells["��������"].EditedFormattedValue.ToString() == "�ռ�����" || 
                                            dataGridView1.Rows[c.RowIndex].Cells["��������"].EditedFormattedValue.ToString() == "ɱ������" ||
                                            dataGridView1.Rows[c.RowIndex].Cells["��������"].EditedFormattedValue.ToString() == "�ɼ�����")
                                        {
                                            DialogResult result = MessageBox.Show("�Ƿ��������", string.Format("����ֵ���ܵ���������ֵ{0}!", defaultValue),
                                                MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                                            if (result == DialogResult.Yes)
                                            {
                                                startEdit = false;
                                                c.Value = defaultValue;
                                                startEdit = true;
                                                return;
                                            }
                                        }
                                    }
                                }
                                catch (Exception ex)
                                {
                                    MessageBox.Show(string.Format("��ⵥԪ��[{0}, {1}]������Ч��ʱ�����쳣: " + ex.ToString(),
                                        e.RowIndex.ToString(), e.ColumnIndex.ToString()));
                                }
                            }
                        }
                        break;
                    }
                case 29: // ͬ��������ֵ
                    {
                        string id = dataGridView1.Rows[e.RowIndex].Cells["��������"].Tag.ToString();
                        string value = c.Value.ToString();
                        startEdit = false;
                        CoordinateData(e.RowIndex - 1, 29, id, value, true);
                        CoordinateData(e.RowIndex + 1, 29, id, value, false);
                        startEdit = true;
                        break;
                    }
                case 31: // ͬ����Ǯ��ֵ
                    {
                        string id = dataGridView1.Rows[e.RowIndex].Cells["��������"].Tag.ToString();
                        string value = c.Value.ToString();
                        startEdit = false;
                        CoordinateData(e.RowIndex - 1, 31, id, value, true);
                        CoordinateData(e.RowIndex + 1, 31, id, value, false);
                        startEdit = true;
                        break;
                    }
            }

            if(fontBold) // �޸�ֵ�ô�����ʾ
            {
                if(fontChangable) // ��������仯
                {
                    c.Style.Font = boldFont;
                }
            }
        }

        /// <summary>
        /// �Զ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void autoFillToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(computeCell == null) // �����Ч��
            {
                return;
            }
            if(computeCell.Value.ToString().Trim() == "" || computeCell.Value.ToString().Trim() == "����") // ֵ����Ϊ��
            {
                MessageBox.Show("�Զ�������ֵ����Ϊ��!");
                return;
            }

            string data = computeCell.EditedFormattedValue.ToString();
            int rowIndex = computeCell.RowIndex; // �����
            int columnIndex = computeCell.ColumnIndex; // �����
        
            startEdit = false;
            switch(columnIndex)
            {
                case 11: // �����Դ����
                    {
                        foreach(DataGridViewRow r in dataGridView1.Rows)
                        {
                            string kind = r.Cells["��������"].Value.ToString(); // ��������
                            if(kind == "�ű�����" || kind == "�ɼ�����" || kind == "�ռ�����" || kind == "ɱ������") // �ж���������
                            {
                                r.Cells[columnIndex].Value = data;
                                if(fontChangable) // ��������仯
                                {
                                    r.Cells[columnIndex].Style.Font = boldFont; // �ı�����
                                }
                                
                            }
                            else
                            {
                                r.Cells[columnIndex].Style.BackColor = readOnlyColor;
                                r.Cells[columnIndex].Value = "����";
                            }
                        }
                        break;
                    }
                case 14: // �����·ʱ��
                    {
                        foreach (DataGridViewRow r in dataGridView1.Rows)
                        {
                            string kind = r.Cells["��������"].Value.ToString(); // ��������
                            if(kind != "ǰ������") // �ж���������
                            {
                                r.Cells[columnIndex].Value = data;
                                if (fontChangable) // ��������仯
                                {
                                    r.Cells[columnIndex].Style.Font = boldFont; // �ı�����
                                }
                            }
                            else
                            {
                                r.Cells[columnIndex].Style.BackColor = readOnlyColor;
                                r.Cells[columnIndex].Value = "����";
                            }
                        }
                        break;
                    }
                case 17: // �����������
                    {
                        foreach (DataGridViewRow r in dataGridView1.Rows)
                        {
                            string kind = r.Cells["��������"].Value.ToString(); // ��������
                            if(kind != "ǰ������") // �ж���������
                            {
                                r.Cells[columnIndex].Value = data;
                                if (fontChangable) // ��������仯
                                {
                                    r.Cells[columnIndex].Style.Font = boldFont; // �ı�����
                                }
                            }
                            else
                            {
                                r.Cells[columnIndex].Style.BackColor = readOnlyColor;
                                r.Cells[columnIndex].Value = "����";
                            }
                        }
                        break;
                    }
                case 18: // ��侭�����
                    {
                        foreach (DataGridViewRow r in dataGridView1.Rows)
                        {
                            string kind = r.Cells["��������"].Value.ToString(); // ��������
                            if(kind != "ǰ������") // �ж���������
                            {
                                r.Cells[columnIndex].Value = data;
                                if (fontChangable) // ��������仯
                                {
                                    r.Cells[columnIndex].Style.Font = boldFont; // �ı�����
                                }
                            }
                            else
                            {
                                r.Cells[columnIndex].Style.BackColor = readOnlyColor;
                                r.Cells[columnIndex].Value = "����";
                            }
                        }
                        break;
                    }
            }
            startEdit = true;
        }

        /// <summary>
        /// �����Զ�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonItem6_Click(object sender, EventArgs e)
        {
            string formerid = "";
            string formeroutput = "";
            bool exist = false;
            if(conn.State == ConnectionState.Closed) // ������
            {
                conn.Open();
            }
            foreach(DataGridViewRow r in dataGridView1.Rows)
            {
                int index = r.Index;
                if(((bool) r.Cells["�Զ�����"].EditedFormattedValue == true)) // ���б�ѡ��
                {
                    exist = true;
                    string id = r.Cells["��������"].Tag.ToString();
                    string output = virtualFlowSectionList[index][0] + "," + virtualFlowSectionList[index][1] +
                        "," + additionalFlowSectionList[index][0] + "," + additionalFlowSectionList[index][1] +
                        "," + r.Cells["��Դ����"].EditedFormattedValue.ToString() + "," + r.Cells["��·ʱ��(��)"].EditedFormattedValue.ToString() +
                        "," + r.Cells["��������"].EditedFormattedValue.ToString() + "," + r.Cells["�������"].EditedFormattedValue.ToString() +
                        ";";
                    if(id != formerid && formerid != "") // �µ�id
                    {
                        SetQuestPlusInfo(formerid, formeroutput.TrimEnd(';')); // ���¸�����Ϣ
                        formerid = id;
                        formeroutput = output;
                    }
                    else
                    {
                        formerid = id;
                        formeroutput = formeroutput + output;
                    }

                    // ����������Ϣ
                    string kind = r.Cells["��������"].Value.ToString(); // ��������
                    string attribute;
                    // ���µ�ͼ�ȼ���Ϣ
                    string level = r.Cells["�ȼ�"].EditedFormattedValue.ToString();
                    if(level == "") // �ȼ�Ϊ��
                    {
                        MessageBox.Show("��{0}������ĵȼ�δ����!", id);
                    }
                    else
                    {
                        UpdateMapLevel(id, level);
                    }
                    
                    // ������Ʒ����������
                    if (kind == "�ű�����" || kind == "�ɼ�����" || kind == "�ռ�����" || kind == "ɱ������") // �����������
                    {    
                        string requireNumber = r.Cells["��������"].EditedFormattedValue.ToString();
                        attribute = r.Cells["��������"].Tag.ToString();
                        if (requireNumber == "") // ��������Ϊ��
                        {
                            MessageBox.Show("��{0}���������������δ����!", id);
                        }
                        else
                        {
                            UpdateRequestNumber(id, attribute, requireNumber);
                        }
                    }
     
                    
                    // ������Ʒ�Ļ�ü���
                    if (kind == "�ɼ�����" || kind == "�ռ�����") // �����������
                    {
                        attribute = r.Cells["��ü���"].Tag.ToString();
                        string rate = r.Cells["��ü���"].EditedFormattedValue.ToString().TrimEnd('%');
                        if (rate == "") // ��ü���Ϊ��
                        {
                            MessageBox.Show("��{0}������Ļ�ü���δ����!", id);
                        }
                        else
                        {
                            int dropRate = (int)(float.Parse(rate) * 10485.76);
                            UpdateGetRate(id, attribute, dropRate.ToString());
                        }                  
                    }

                    // ����NPC��ˢ��ʱ��
                    if (kind == "�ռ�����" || kind == "ɱ������") // �����������
                    {
                        attribute = npcIDTable[index.ToString()] as string; // NPC��ID
                        if(attribute != null)
                        {
                            string refreshTime = r.Cells["ˢ��ʱ��(��)"].EditedFormattedValue.ToString();
                            if (refreshTime == "") // ˢ��ʱ��Ϊ��
                            {
                                MessageBox.Show("��{0}�������ˢ��ʱ��δ����!", id);
                            }
                            else
                            {
                                UpdateNPCRefreshTime(attribute, refreshTime);
                            }
                        }
                    }

                    // ����Doodad��ˢ��ʱ��
                    if (kind == "�ɼ�����") // �����������
                    {
                        attribute = doodadIDTable[index.ToString()] as string; // Doodad��ID
                        if (attribute != null)
                        {
                            string refreshTime = r.Cells["ˢ��ʱ��(��)"].EditedFormattedValue.ToString();
                            if (refreshTime == "") // ˢ��ʱ��Ϊ��
                            {
                                MessageBox.Show("��{0}�������ˢ��ʱ��δ����!", id);
                            }
                            else
                            {
                                UpdateDoodadRefreshTime(attribute, refreshTime);
                            }
                        }
                    }

                    // ��������Ľ�������
                    if(kind != "ǰ������") // �����������
                    {
                        string rewardExp = r.Cells["������ֵ"].EditedFormattedValue.ToString();
                        if (rewardExp == "") // ��������Ϊ��
                        {
                            MessageBox.Show("��{0}������Ľ�������δ����!", id);
                        }
                        else
                        {
                            UpdatePresentExp(id, rewardExp);
                        } 
                    }

                    // ��������Ľ�����Ǯ
                    if(kind != "ǰ������") // �����������
                    {
                        string rewardMoney = r.Cells["��Ǯ��ֵ"].EditedFormattedValue.ToString();
                        if (rewardMoney == "") // ������ǮΪ��
                        {
                            MessageBox.Show("��{0}������Ľ�����Ǯδ����!", id);
                        }
                        else
                        {
                            UpdatePresentMoney(id, rewardMoney);
                        } 
                    }

                    // �ı䵥Ԫ������
                    if(fontChangable) // ��������仯
                    {
                        foreach (DataGridViewCell c in r.Cells)
                        {
                            if (c.Style.Font != regularFont)
                            {
                                c.Style.Font = regularFont;
                            }
                        }
                    }           
                }
            }
            if(exist)
            {
                SetQuestPlusInfo(formerid, formeroutput.TrimEnd(';')); // ���¸�����Ϣ
            }
            if (conn.State == ConnectionState.Open) // �ر�����
            {
                conn.Close();
            }
            MessageBox.Show("�Զ����������ɹ�!");
        }

        /// <summary>
        /// ���ò�����ֵ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonItem1_Click(object sender, EventArgs e)
        {
            DialogResult result = sa.ShowDialog();
            if(result != DialogResult.OK)
            {
                return;
            }
            startEdit = false;
            FillVirtualFlowSection(sa.MaxFlow, sa.MinFlow); // ���ʵ����������
            FillAdditionalFlowSection(sa.MaxFlow, sa.MinFlow); // ��������������
            startEdit = true;
            fontChangable = sa.FontChangable;
        }

        /// <summary>
        /// �û�ѡ�񱣴���ֵ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void hideColumnToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(computeCell != null)
            {
                string id = dataGridView1.Rows[computeCell.RowIndex].Cells["��������"].Tag.ToString(); // ����id
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                switch(computeCell.ColumnIndex)
                {
                    case 7: // ����ȼ�
                        {
                            string level = computeCell.EditedFormattedValue.ToString();
                            UpdateMapLevel(id, level);
                            MessageBox.Show("��ֵ����ɹ�!");
                            if (fontChangable) // ��������仯
                            {
                                computeCell.Style.Font = regularFont; // �ı�����
                            }
                            break;
                        }
                    case 9: // ��������
                        {
                            string requireNumber = computeCell.EditedFormattedValue.ToString();
                            string attribute = computeCell.Tag.ToString();
                            if (requireNumber == "" || requireNumber == "����") // ��������Ϊ��
                            {
                                MessageBox.Show("��{0}���������������δ����!", id);
                            }
                            else
                            {
                                UpdateRequestNumber(id, attribute, requireNumber);
                                MessageBox.Show("��ֵ����ɹ�!");
                                if (fontChangable) // ��������仯
                                {
                                    computeCell.Style.Font = regularFont; // �ı�����
                                }
                            }
                            break;
                        }
                    case 10: // ��ü���
                        {
                            string attribute = computeCell.Tag.ToString();
                            string rate = computeCell.EditedFormattedValue.ToString().TrimEnd('%');
                            if (rate == "" || rate == "����") // ��ü���Ϊ��
                            {
                                MessageBox.Show("��{0}������Ļ�ü���δ����!", id);
                            }
                            else
                            {
                                int dropRate = (int)(float.Parse(rate) * 10485.76);
                                UpdateGetRate(id, attribute, dropRate.ToString());
                                MessageBox.Show("��ֵ����ɹ�!");
                                if (fontChangable) // ��������仯
                                {
                                    computeCell.Style.Font = regularFont; // �ı�����
                                }
                            }    
                            break;
                        }
                    case 12: // ˢ��ʱ��
                        {
                            string refreshTime = computeCell.EditedFormattedValue.ToString();
                            string kind = computeCell.OwningRow.Cells["��������"].Value.ToString();

                            if(kind == "�ռ�����" || kind == "ɱ������")
                            {
                                string attribute = npcIDTable[computeCell.RowIndex.ToString()] as string;
                                if (attribute != null)
                                {
                                    if (refreshTime == "") // ˢ��ʱ��Ϊ��
                                    {
                                        MessageBox.Show("��{0}�������ˢ��ʱ��δ����!", id);
                                    }
                                    else
                                    {
                                        UpdateNPCRefreshTime(attribute, refreshTime);
                                        MessageBox.Show("��ֵ����ɹ�!");
                                        if (fontChangable) // ��������仯
                                        {
                                            computeCell.Style.Font = regularFont; // �ı�����
                                        }
                                    }
                                }
                            }
                            else if (kind == "�ɼ�����")
                            {
                                string attribute = doodadIDTable[computeCell.RowIndex.ToString()] as string;
                                if (attribute != null)
                                {
                                    if (refreshTime == "") // ˢ��ʱ��Ϊ��
                                    {
                                        MessageBox.Show("��{0}�������ˢ��ʱ��δ����!", id);
                                    }
                                    else
                                    {
                                        UpdateDoodadRefreshTime(attribute, refreshTime);
                                        MessageBox.Show("��ֵ����ɹ�!");
                                        if (fontChangable) // ��������仯
                                        {
                                            computeCell.Style.Font = regularFont; // �ı�����
                                        }
                                    }
                                }
                            }
                            break;
                        }
                    case 29: // ������ֵ
                        {
                            string rewardExp = computeCell.EditedFormattedValue.ToString();
                            if (rewardExp == "" || rewardExp == "����") // ��������Ϊ��
                            {
                                MessageBox.Show("��{0}������Ľ�������δ����!", id);
                            }
                            else
                            {
                                UpdatePresentExp(id, rewardExp);
                                MessageBox.Show("��ֵ����ɹ�!");
                                if (fontChangable) // ��������仯
                                {
                                    computeCell.Style.Font = regularFont; // �ı�����
                                }
                            } 
                            break;
                        }
                    case 31: // ��Ǯ��ֵ
                        {
                            string rewardMoney = computeCell.EditedFormattedValue.ToString();
                            if (rewardMoney == "" || rewardMoney == "����") // ������ǮΪ��
                            {
                                MessageBox.Show("��{0}������Ľ�����Ǯδ����!", id);
                            }
                            else
                            {
                                UpdatePresentMoney(id, rewardMoney);
                                MessageBox.Show("��ֵ����ɹ�!");
                                if (fontChangable) // ��������仯
                                {
                                    computeCell.Style.Font = regularFont; // �ı�����
                                }
                            } 
                            break;
                        }
                }
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// ��ʾģʽ�л�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonItem2_Click(object sender, EventArgs e)
        {
            if(simpleMode) // ����ģʽ
            {
                foreach(int i in simpleModeList)
                {
                    dataGridView1.Columns[i].Visible = true;
                }
                buttonItem2.Text = "����ģʽ";
            }
            else // ��ȫģʽ
            {
                foreach(int i in simpleModeList)
                {
                    dataGridView1.Columns[i].Visible = false;
                }
                buttonItem2.Text = "��ȫģʽ";
            }
            simpleMode = !simpleMode;
        }
    }
}