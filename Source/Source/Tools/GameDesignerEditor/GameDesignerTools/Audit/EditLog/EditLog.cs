using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;


namespace GameDesingerTools
{
    public partial class EditLog : Form
    {
        private SqlConnection conn; // sql����
        private DataTable dataTable; // ���ݱ�
        private DataTable modelTable; // ģ���
        private DataTable tabTable; // ����ҳ��
        private TreeNode node; // ��ǰѡ�е������
        private GridItem item; // ��ǰѡ�е�GridItem
        private string fieldID; // �ֶ������ַ���
        private object[] primaryKeys; // �������������
        private bool allUserName = true; // �Ƿ�ѡ��ȫ���û�����
        private bool allNodeName = true; // �Ƿ�ѡ��ȫ�����������
        private bool allCatName = true;  // �Ƿ�ѡ��ȫ����������
        private bool allModelName = true; // �Ƿ�ѡ��ȫ��ģ������
        private bool allTabName = true; // �Ƿ�ѡ��ȫ������ҳ����
        private bool allFieldName = true; // �Ƿ�ѡ��ȫ���ֶ�����
        private bool beginSearch = false; // �Ƿ��Ѿ���ʼ����
        private CatForm cForm = new CatForm(); // ����ѡ����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="fieldID">�ֶ������ַ���</param>
        /// <param name="primaryKeys">�������������</param>
        /// <param name="searchLevel">��������</param>
        public EditLog(SqlConnection conn, string fieldID, TreeNode node, GridItem item)
        {
            this.conn = conn;
            this.fieldID = fieldID;
            this.node = node;
            this.primaryKeys = node.Tag as object[];
            this.item = item;
            InitializeComponent();
            ResetTime();
            InitTable();
            InitData();
        }

        /// <summary>
        /// ��ʼ�����
        /// </summary>
        private void InitTable()
        {
            DataGridViewTextBoxColumn hostNameColumn = new DataGridViewTextBoxColumn();
            hostNameColumn.HeaderText = "�û���";
            hostNameColumn.Name = "�û���";

            DataGridViewTextBoxColumn editTimeColumn = new DataGridViewTextBoxColumn();
            editTimeColumn.HeaderText = "�޸�ʱ��";
            editTimeColumn.Name = "�޸�ʱ��";

            DataGridViewTextBoxColumn actionColumn = new DataGridViewTextBoxColumn();
            actionColumn.HeaderText = "����";
            actionColumn.Name = "����";

            DataGridViewTextBoxColumn modelNameColumn = new DataGridViewTextBoxColumn();
            modelNameColumn.HeaderText = "ģ����";
            modelNameColumn.Name = "ģ����";

            DataGridViewTextBoxColumn tabNameColumn = new DataGridViewTextBoxColumn();
            tabNameColumn.HeaderText = "����ҳ��";
            tabNameColumn.Name = "����ҳ��";

            DataGridViewTextBoxColumn catNameColumn = new DataGridViewTextBoxColumn();
            catNameColumn.HeaderText = "��������";
            catNameColumn.Name = "��������";

            DataGridViewTextBoxColumn recordNameColumn = new DataGridViewTextBoxColumn();
            recordNameColumn.HeaderText = "�������";
            recordNameColumn.Name = "�������";

            DataGridViewTextBoxColumn fieldDisplayNameColumn = new DataGridViewTextBoxColumn();
            fieldDisplayNameColumn.HeaderText = "�ֶ���";
            fieldDisplayNameColumn.Name = "�ֶ���";

            DataGridViewTextBoxColumn oldValueColumn = new DataGridViewTextBoxColumn();
            oldValueColumn.HeaderText = "��ֵ";
            oldValueColumn.Name = "��ֵ";

            DataGridViewTextBoxColumn newValueColumn = new DataGridViewTextBoxColumn();
            newValueColumn.HeaderText = "��ֵ";
            newValueColumn.Name = "��ֵ";

            DataGridViewTextBoxColumn remarksColumn = new DataGridViewTextBoxColumn();
            remarksColumn.HeaderText = "��ע";
            remarksColumn.Name = "��ע";

            dataGridView1.Columns.Add(hostNameColumn);
            dataGridView1.Columns.Add(editTimeColumn);

            dataGridView1.Columns.Add(actionColumn);

            dataGridView1.Columns.Add(modelNameColumn);
            dataGridView1.Columns.Add(tabNameColumn);
            dataGridView1.Columns.Add(catNameColumn);
            dataGridView1.Columns.Add(recordNameColumn);
            dataGridView1.Columns.Add(fieldDisplayNameColumn);
            dataGridView1.Columns.Add(oldValueColumn);
            dataGridView1.Columns.Add(newValueColumn);
            dataGridView1.Columns.Add(remarksColumn);
        }

        /// <summary>
        /// �޸ĳ�ʼ�Ŀ�ѡʱ��
        /// </summary>
        private void ResetTime()
        {
            DateTime dateTime = dateTimePicker1.Value;
            switch(dateTime.DayOfWeek) // Ĭ���޸�Ϊ����һ
            {
                case DayOfWeek.Monday:
                    {
                        dateTimePicker1.Value = dateTime.AddDays(0);
                        break;
                    }
                case DayOfWeek.Tuesday:
                    {
                        dateTimePicker1.Value = dateTime.AddDays(-1);
                        break;
                    }
                case DayOfWeek.Wednesday:
                    {
                        dateTimePicker1.Value = dateTime.AddDays(-2);
                        break;
                    }
                case DayOfWeek.Thursday:
                    {
                        dateTimePicker1.Value = dateTime.AddDays(-3);
                        break;
                    }
                case DayOfWeek.Friday:
                    {
                        dateTimePicker1.Value = dateTime.AddDays(-4);
                        break;
                    }
                case DayOfWeek.Saturday:
                    {
                        dateTimePicker1.Value = dateTime.AddDays(-5);
                        break;
                    }
                case DayOfWeek.Sunday:
                    {
                        dateTimePicker1.Value = dateTime.AddDays(-6);
                        break;
                    }
            }
        }

        /// <summary>
        /// ��ȡ��ǰ�ֶε�������ʾ����
        /// </summary>
        /// <returns>��ǰ�ֶε�������ʾ����</returns>
        private string GetFullDisplayName()
        {
            string fullDisplayName = item.Label; // ��������ʾ����
            GridItem parent = item.Parent; // ��GridItem

            while(parent != null) // ��GridItem��Ϊ��
            {
                DBCustomClass dbClass = parent.Value as DBCustomClass;
                if(dbClass != null && dbClass.ParentFieldID > 0) // ��Ч���ֶβ��Ҳ��Ƿ���
                {
                    fullDisplayName = string.Format("{0}.", parent.Label) + fullDisplayName;
                }
                parent = parent.Parent;
            }

            return fullDisplayName;
        }

        /// <summary>
        /// ��������ѯ��Χ������
        /// </summary>
        /// <param name="fieldid">�ֶ�id</param>
        private void FillSelectionNames(string fieldid)
        {
            //DataTable table = Helper.GetDataTable(string.Format("SELECT modtabid, fieldcnname FROM sys_meta_info WHERE fieldid = {0}", fieldid), conn);
            string filter = string.Format("fieldid = {0}", fieldid);
            DataTable table = Helper.GetDataTableProxy("sys_meta_info", filter, null, conn);

            if(table.Rows.Count > 0)
            {
                string tabID = table.Rows[0]["modtabid"].ToString();
                string fieldName = GetFullDisplayName();
                DataRow[] modelRows = tabTable.Select(string.Format("modtabid = {0}", tabID));
                if(modelRows.Length > 0)
                {
                    string modelID = modelRows[0]["modid"].ToString();
                    string nodeName = node.Text;
                    comboBoxEx5.Items[0] = string.Format("��ǰ�ֶ� [{0}]", fieldName);
                    comboBoxEx5.Items[1] = string.Format("��ǰ��¼ [{0}]", nodeName);
                    comboBoxEx5.Items[2] = string.Format("��ǰ����ҳ [{0}]", GetTabName(tabID));
                    comboBoxEx5.Items[3] = string.Format("��ǰģ�� [{0}]", GetModelName(modelID));
                }
            }
        }

        /// <summary>
        /// ������ǰ�ֶε���ʷ�޸�����
        /// </summary>
        private void SearchFieldData()
        {
            beginSearch = false;
            DateTime startTime = dateTimePicker1.Value;
            DateTime endTime = dateTimePicker2.Value;
            DateTime newStartTime = new DateTime(startTime.Year, startTime.Month, startTime.Day, 0, 0, 0);
            DateTime newEndTime = new DateTime(endTime.Year, endTime.Month, endTime.Day, 23, 59, 59);

            /* remoting
            string sql = string.Format("SELECT * FROM {0} WHERE fieldid = \'{1}\' AND keys = \'{2}\' AND edittime >= \'{3}\' AND edittime <= \'{4}\'",
                    "sys_dataRecord", fieldID, GetPrimaryKeyString(primaryKeys), newStartTime, newEndTime);
            dataTable = Helper.GetDataTable(sql, conn);
            */

            string filter = string.Format("fieldid = \'{0}\' AND keys = \'{1}\' AND edittime >= \'{2}\' AND edittime <= \'{3}\'",
                    fieldID, GetPrimaryKeyString(primaryKeys), newStartTime, newEndTime);
            dataTable = Helper.GetDataTableProxy("sys_dataRecord", filter, null, conn);


            dataGridView1.Rows.Clear(); // ���ԭ��ʾ����
            int index = 0;
            foreach (DataRow r in dataTable.Rows)
            {
                dataGridView1.Rows.Add();
                dataGridView1.Rows[index].Cells["�û���"].Value = r["hostname"];
                dataGridView1.Rows[index].Cells["�޸�ʱ��"].Value = r["edittime"];
                dataGridView1.Rows[index].Cells["ģ����"].Value = GetModelName(r["modelid"]);
                if (r["action"].ToString() == "�޸�ֵ")
                {
                    dataGridView1.Rows[index].Cells["����ҳ��"].Value = GetTabName(r["tabid"]);
                }
                dataGridView1.Rows[index].Cells["��������"].Value = r["catfields"];
                dataGridView1.Rows[index].Cells["�������"].Value = r["nodename"];
                dataGridView1.Rows[index].Cells["�ֶ���"].Value = r["fielddisplayname"];
                dataGridView1.Rows[index].Cells["��ֵ"].Value = r["oldvalue"];
                dataGridView1.Rows[index].Cells["��ֵ"].Value = r["newvalue"];
                dataGridView1.Rows[index].Cells["��ע"].Value = r["remarks"];
                dataGridView1.Rows[index].Cells["����"].Value = r["action"];
                index++;
            }
    
            if (dataGridView1.Rows.Count != 0) // ��ѯ�����Ϊ��
            {
                FillHostNames();
                FillModelNames();
                FillTabNames("����ģ��");
                FillRecordNames();
                FillFieldNames();
                allUserName = true;
                checkBox1.Enabled = true;

                comboBoxEx2.Text = dataGridView1.Rows[0].Cells["ģ����"].Value.ToString();
                comboBoxEx3.Text = dataGridView1.Rows[0].Cells["����ҳ��"].Value.ToString();
                comboBoxEx4.Text = dataGridView1.Rows[0].Cells["�������"].Value.ToString();
                comboBoxEx8.Text = dataGridView1.Rows[0].Cells["��������"].Value.ToString();
                comboBoxEx6.Text = dataGridView1.Rows[0].Cells["�ֶ���"].Value.ToString();
                this.Text = string.Format("�����޸���־ [���ҵ�{0}�������Ϣ]", index);
            }
            else
            {
                checkBox1.Enabled = false;
                this.Text = string.Format("�����޸���־ [û���ҵ������Ϣ]", index);
                MessageBox.Show("û���ҵ����ֶε���ʷ�޸���Ϣ!", "���ݲ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            checkBox1.Checked = false;
            beginSearch = true;
        }

        /// <summary>
        /// ������ǰ��¼����ʷ�޸�����
        /// </summary>
        private void SearchRecordData()
        {
            beginSearch = false;
            DateTime startTime = dateTimePicker1.Value;
            DateTime endTime = dateTimePicker2.Value;
            DateTime newStartTime = new DateTime(startTime.Year, startTime.Month, startTime.Day, 0, 0, 0);
            DateTime newEndTime = new DateTime(endTime.Year, endTime.Month, endTime.Day, 23, 59, 59);

            /* remoting
            string sql = string.Format("SELECT modtabid FROM {0} WHERE fieldid = \'{1}\'", 
                "sys_meta_info", fieldID);
            DataTable modTabIDTable = Helper.GetDataTable(sql, conn);
             */

            string filter = string.Format("fieldid = \'{0}\'",  fieldID);
            DataTable modTabIDTable = Helper.GetDataTableProxy("sys_meta_info", filter, null, conn);

            if (modTabIDTable.Rows.Count != 0)
            {
                string modtabID = modTabIDTable.Rows[0]["modtabid"].ToString();
                DataRow[] tabRow = tabTable.Select(string.Format("modtabid = {0}", modtabID));
                //??
                string modelID = tabRow[0]["modid"].ToString();

                /* remoting
                sql = string.Format("SELECT * FROM {0} WHERE modelid = \'{1}\' AND keys = \'{2}\' AND edittime >= \'{3}\' AND edittime <= \'{4}\'",
                    "sys_dataRecord", modelID, GetPrimaryKeyString(primaryKeys), newStartTime, newEndTime);
                dataTable = Helper.GetDataTable(sql, conn);
                 */

                filter = string.Format("modelid = \'{0}\' AND keys = \'{1}\' AND edittime >= \'{2}\' AND edittime <= \'{3}\'",
                    modelID, GetPrimaryKeyString(primaryKeys), newStartTime, newEndTime);
                dataTable = Helper.GetDataTableProxy("sys_dataRecord", filter, null, conn); 

                if (dataTable.Rows.Count != 0)
                {
                    dataGridView1.Rows.Clear(); // ���ԭ��ʾ����
                    int index = 0;
                    foreach (DataRow r in dataTable.Rows)
                    {
                        dataGridView1.Rows.Add();
                        dataGridView1.Rows[index].Cells["�û���"].Value = r["hostname"];
                        dataGridView1.Rows[index].Cells["�޸�ʱ��"].Value = r["edittime"];
                        dataGridView1.Rows[index].Cells["ģ����"].Value = GetModelName(r["modelid"]);
                        if (r["action"].ToString() == "�޸�ֵ")
                        {
                            dataGridView1.Rows[index].Cells["����ҳ��"].Value = GetTabName(r["tabid"]);
                        }
                        dataGridView1.Rows[index].Cells["��������"].Value = r["catfields"];
                        dataGridView1.Rows[index].Cells["�������"].Value = r["nodename"];
                        dataGridView1.Rows[index].Cells["�ֶ���"].Value = r["fielddisplayname"];
                        dataGridView1.Rows[index].Cells["��ֵ"].Value = r["oldvalue"];
                        dataGridView1.Rows[index].Cells["��ֵ"].Value = r["newvalue"];
                        dataGridView1.Rows[index].Cells["��ע"].Value = r["remarks"];
                        dataGridView1.Rows[index].Cells["����"].Value = r["action"];
                        index++;
                    }

                    if(dataGridView1.Rows.Count != 0) // ��ѯ�����Ϊ��
                    {
                        FillHostNames();
                        FillModelNames();
                        FillTabNames("����ģ��");
                        FillRecordNames();
                        FillFieldNames();
                        allUserName = true;
                        allFieldName = true;
                        checkBox1.Enabled = true;

                        comboBoxEx2.Text = dataGridView1.Rows[0].Cells["ģ����"].Value.ToString();
                        comboBoxEx3.Text = dataGridView1.Rows[0].Cells["����ҳ��"].Value.ToString();
                        comboBoxEx4.Text = dataGridView1.Rows[0].Cells["�������"].Value.ToString();
                        comboBoxEx8.Text = dataGridView1.Rows[0].Cells["��������"].Value.ToString();
                        this.Text = string.Format("�����޸���־ [���ҵ�{0}�������Ϣ]", index);
                    }                 
                }
                else
                {
                    checkBox1.Enabled = false;
                    this.Text = "�����޸���־ [û���ҵ������Ϣ]";
                    MessageBox.Show("û���ҵ��ü�¼����ʷ�޸���Ϣ!", "���ݲ���", MessageBoxButtons.OK, MessageBoxIcon.Information);                   
                }
            }
            else
            {
                checkBox1.Enabled = false;
                this.Text = "�����޸���־ [û���ҵ������Ϣ]";
                MessageBox.Show("û���ҵ��ü�¼����ʷ�޸���Ϣ!", "���ݲ���", MessageBoxButtons.OK, MessageBoxIcon.Information);              
            }
         
            checkBox1.Checked = false;           
            beginSearch = true;
        }

        /// <summary>
        /// ������ǰ����ҳ����ʷ�޸�����
        /// </summary>
        private void SearchTabData()
        {
            beginSearch = false;
            DateTime startTime = dateTimePicker1.Value;
            DateTime endTime = dateTimePicker2.Value;
            DateTime newStartTime = new DateTime(startTime.Year, startTime.Month, startTime.Day, 0, 0, 0);
            DateTime newEndTime = new DateTime(endTime.Year, endTime.Month, endTime.Day, 23, 59, 59);

            /* remoting
            string sql = string.Format(string.Format("SELECT modtabid FROM {0} WHERE fieldid = \'{1}\'", 
                "sys_meta_info", fieldID));
            DataTable modTabIDTable = Helper.GetDataTable(sql, conn);
            */

            string filter = string.Format("fieldid = \'{0}\'", fieldID);
            DataTable modTabIDTable = Helper.GetDataTableProxy("sys_meta_info", filter, null, conn);

            if (modTabIDTable.Rows.Count != 0)
            {
                string modtabID = modTabIDTable.Rows[0]["modtabid"].ToString();

                /* remoting
                string sql = string.Format("SELECT * FROM {0} WHERE tabid = \'{1}\' AND edittime >= \'{2}\' AND edittime <= \'{3}\'", 
                    "sys_dataRecord", modtabID, newStartTime, newEndTime);
                dataTable = Helper.GetDataTable(sql, conn);
                */

                filter = string.Format("tabid = \'{0}\' AND edittime >= \'{1}\' AND edittime <= \'{2}\'",
                    modtabID, newStartTime, newEndTime);
                dataTable = Helper.GetDataTableProxy("sys_dataRecord", filter, null, conn);

                if (dataTable.Rows.Count != 0)
                {
                    dataGridView1.Rows.Clear(); // ���ԭ��ʾ����
                    int index = 0;
                    foreach (DataRow r in dataTable.Rows)
                    {
                        dataGridView1.Rows.Add();
                        dataGridView1.Rows[index].Cells["�û���"].Value = r["hostname"];
                        dataGridView1.Rows[index].Cells["�޸�ʱ��"].Value = r["edittime"];
                        dataGridView1.Rows[index].Cells["ģ����"].Value = GetModelName(r["modelid"]);
                        if (r["action"].ToString() == "�޸�ֵ")
                        {
                            dataGridView1.Rows[index].Cells["����ҳ��"].Value = GetTabName(r["tabid"]);
                        }
                        dataGridView1.Rows[index].Cells["��������"].Value = r["catfields"];
                        dataGridView1.Rows[index].Cells["�������"].Value = r["nodename"];
                        dataGridView1.Rows[index].Cells["�ֶ���"].Value = r["fielddisplayname"];
                        dataGridView1.Rows[index].Cells["��ֵ"].Value = r["oldvalue"];
                        dataGridView1.Rows[index].Cells["��ֵ"].Value = r["newvalue"];
                        dataGridView1.Rows[index].Cells["��ע"].Value = r["remarks"];
                        dataGridView1.Rows[index].Cells["����"].Value = r["action"];
                        index++;
                    }

                    if (dataGridView1.Rows.Count != 0) // ��ѯ�����Ϊ��
                    {
                        FillHostNames();
                        FillModelNames();
                        FillTabNames("����ģ��");
                        FillRecordNames();
                        FillFieldNames();
                        comboBoxEx8.Text = "���з���";
                        allUserName = true;
                        allNodeName = true;
                        allCatName = true;
                        allFieldName = true;
                        checkBox1.Enabled = true;

                        comboBoxEx2.Text = dataGridView1.Rows[0].Cells["ģ����"].Value.ToString();
                        comboBoxEx3.Text = dataGridView1.Rows[0].Cells["����ҳ��"].Value.ToString();
                        this.Text = string.Format("�����޸���־ [���ҵ�{0}�������Ϣ]", index);
                    }
                }
                else
                {
                    checkBox1.Enabled = false;
                    this.Text = "�����޸���־ [û���ҵ������Ϣ]";
                    MessageBox.Show("û���ҵ�������ҳ����ʷ�޸���Ϣ!", "���ݲ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                checkBox1.Enabled = false;
                this.Text = "�����޸���־ [û���ҵ������Ϣ]";
                MessageBox.Show("û���ҵ�������ҳ����ʷ�޸���Ϣ!", "���ݲ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            checkBox1.Checked = false;
            beginSearch = true;
        }

        /// <summary>
        /// ������ǰģ�����ʷ�޸�����
        /// </summary>
        private void SearchModelData()
        {
            beginSearch = false;
            DateTime startTime = dateTimePicker1.Value;
            DateTime endTime = dateTimePicker2.Value;
            DateTime newStartTime = new DateTime(startTime.Year, startTime.Month, startTime.Day, 0, 0, 0);
            DateTime newEndTime = new DateTime(endTime.Year, endTime.Month, endTime.Day, 23, 59, 59);

            /* remoting
            string sql = string.Format(string.Format("SELECT modtabid FROM {0} WHERE fieldid = \'{1}\'", 
                "sys_meta_info", fieldID));
            DataTable modTabIDTable = Helper.GetDataTable(sql, conn);
            */

            string filter = string.Format("fieldid = \'{0}\'", fieldID);
            DataTable modTabIDTable = Helper.GetDataTableProxy("sys_meta_info", filter, null, conn);

            if (modTabIDTable.Rows.Count != 0)
            {
                string modtabID = modTabIDTable.Rows[0]["modtabid"].ToString();
                DataRow[] rows = tabTable.Select(string.Format("modtabid = {0}", modtabID));

                if(rows.Length != 0)
                {
                    string modelID = rows[0]["modid"].ToString();

                    /* remoting
                    sql = string.Format("SELECT * FROM {0} WHERE modelid = \'{1}\' AND edittime >= \'{2}\' AND edittime <= \'{3}\'", 
                        "sys_dataRecord", modelID, newStartTime, newEndTime);
                    dataTable = Helper.GetDataTable(sql, conn);
                    */

                    filter = string.Format("modelid = \'{0}\' AND edittime >= \'{1}\' AND edittime <= \'{2}\'",
                       modelID, newStartTime, newEndTime);
                     dataTable = Helper.GetDataTableProxy("sys_dataRecord",  filter, null, conn);
                    if (dataTable.Rows.Count != 0)
                    {
                        dataGridView1.Rows.Clear(); // ���ԭ��ʾ����
                        int index = 0;
                        foreach (DataRow r in dataTable.Rows)
                        {
                            dataGridView1.Rows.Add();
                            dataGridView1.Rows[index].Cells["�û���"].Value = r["hostname"];
                            dataGridView1.Rows[index].Cells["�޸�ʱ��"].Value = r["edittime"];
                            dataGridView1.Rows[index].Cells["ģ����"].Value = GetModelName(r["modelid"]);
                            if (r["action"].ToString() == "�޸�ֵ")
                            {
                                dataGridView1.Rows[index].Cells["����ҳ��"].Value = GetTabName(r["tabid"]);
                            }
                            dataGridView1.Rows[index].Cells["��������"].Value = r["catfields"];
                            dataGridView1.Rows[index].Cells["�������"].Value = r["nodename"];
                            dataGridView1.Rows[index].Cells["�ֶ���"].Value = r["fielddisplayname"];
                            dataGridView1.Rows[index].Cells["��ֵ"].Value = r["oldvalue"];
                            dataGridView1.Rows[index].Cells["��ֵ"].Value = r["newvalue"];
                            dataGridView1.Rows[index].Cells["��ע"].Value = r["remarks"];
                            dataGridView1.Rows[index].Cells["����"].Value = r["action"];
                            index++;
                        }

                        if (dataGridView1.Rows.Count != 0) // ��ѯ�����Ϊ��
                        {
                            FillHostNames();
                            FillModelNames();
                            FillTabNames("����ģ��");
                            FillRecordNames();
                            FillFieldNames();
                            comboBoxEx8.Text = "���з���";
                            allUserName = true;
                            allTabName = true;
                            allNodeName = true;
                            allCatName = true;
                            allFieldName = true;
                            checkBox1.Enabled = true;

                            comboBoxEx2.Text = dataGridView1.Rows[0].Cells["ģ����"].Value.ToString();
                            this.Text = string.Format("�����޸���־ [���ҵ�{0}�������Ϣ]", index);
                        }
                    }
                    else
                    {
                        checkBox1.Enabled = false;
                        this.Text = "�����޸���־ [û���ҵ������Ϣ]";
                        MessageBox.Show("û���ҵ���ģ�����ʷ�޸���Ϣ!", "���ݲ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
                else
                {
                    checkBox1.Enabled = false;
                    this.Text = "�����޸���־ [û���ҵ������Ϣ]";
                    MessageBox.Show("û���ҵ���ģ�����ʷ�޸���Ϣ!", "���ݲ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }          
            }
            else
            {
                checkBox1.Enabled = false;
                this.Text = "�����޸���־ [û���ҵ������Ϣ]";
                MessageBox.Show("û���ҵ���ģ�����ʷ�޸���Ϣ!", "���ݲ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            checkBox1.Checked = false;
            beginSearch = true;
        }

        /// <summary>
        /// ����ȫ����ʷ��¼��Ϣ
        /// </summary>
        private void SearchAllData()
        {
            beginSearch = false;
            DateTime startTime = dateTimePicker1.Value;
            DateTime endTime = dateTimePicker2.Value;
            DateTime newStartTime = new DateTime(startTime.Year, startTime.Month, startTime.Day, 0, 0, 0);
            DateTime newEndTime = new DateTime(endTime.Year, endTime.Month, endTime.Day, 23, 59, 59);

            /* remoting
            string sql = string.Format("SELECT * FROM {0} WHERE edittime >= \'{1}\' AND edittime <= \'{2}\'", 
                "sys_dataRecord", newStartTime, newEndTime);
            dataTable = Helper.GetDataTable(sql, conn);
            */

            string filter = string.Format("edittime >= \'{0}\' AND edittime <= \'{1}\'", newStartTime, newEndTime);
            dataTable = Helper.GetDataTableProxy("sys_dataRecord", filter, null, conn);

            dataGridView1.Rows.Clear(); // ���ԭ��ʾ����
            int index = 0;
            foreach (DataRow r in dataTable.Rows)
            {
                dataGridView1.Rows.Add();
                dataGridView1.Rows[index].Cells["�û���"].Value = r["hostname"];
                dataGridView1.Rows[index].Cells["�޸�ʱ��"].Value = r["edittime"];
                dataGridView1.Rows[index].Cells["ģ����"].Value = GetModelName(r["modelid"]);
                if (r["action"].ToString() == "�޸�ֵ")
                {
                    dataGridView1.Rows[index].Cells["����ҳ��"].Value = GetTabName(r["tabid"]);
                }
                dataGridView1.Rows[index].Cells["��������"].Value = r["catfields"];
                dataGridView1.Rows[index].Cells["�������"].Value = r["nodename"];
                dataGridView1.Rows[index].Cells["�ֶ���"].Value = r["fielddisplayname"];
                dataGridView1.Rows[index].Cells["��ֵ"].Value = r["oldvalue"];
                dataGridView1.Rows[index].Cells["��ֵ"].Value = r["newvalue"];
                dataGridView1.Rows[index].Cells["��ע"].Value = r["remarks"];
                dataGridView1.Rows[index].Cells["����"].Value = r["action"];
                index++;
            }

            if (dataGridView1.Rows.Count > 0) // ��ѯ�����Ϊ��
            {
                FillHostNames();
                FillModelNames();
                FillTabNames("����ģ��");
                FillRecordNames();
                FillFieldNames();
                comboBoxEx8.Text = "���з���";
                allUserName = true;
                allModelName = true;
                allTabName = true;
                allNodeName = true;
                allCatName = true;
                allFieldName = true;
                checkBox1.Enabled = true;

                this.Text = string.Format("�����޸���־ [���ҵ�{0}�������Ϣ]", index);
            }
            else
            {
                checkBox1.Enabled = false;
                this.Text = "�����޸���־ [û���ҵ������Ϣ]";
                MessageBox.Show("û���ҵ���ʷ�޸���Ϣ!", "���ݲ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            checkBox1.Checked = false;
            beginSearch = true;
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData()
        {
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }           
                /* remoting
                modelTable = Helper.GetDataTable("SELECT id, modelname_cn FROM sys_modl_def", conn);
                tabTable = Helper.GetDataTable("SELECT modtabid, modid, name FROM sys_modl_tab_def", conn);
                 */

                modelTable = Helper.GetDataTableProxy("sys_modl_def", conn);
                tabTable = Helper.GetDataTableProxy("sys_modl_tab_def", conn);


                FillSelectionNames(fieldID);
                SearchFieldData(); // ������ǰ�ֶε���ʷ�޸�����
                comboBoxEx5.SelectedIndex = 0;
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ��¼�޸���Ϣʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// ��ȡ�����������ַ���
        /// </summary>
        /// <param name="keys">��������������</param>
        /// <returns>�����������ַ���</returns>
        private string GetPrimaryKeyString(object[] keys)
        {
            string keyString = "";
            foreach(object o in keys)
            {
                keyString += string.Format("{0},", o);
            }
            return keyString.TrimEnd(',');
        }

        /// <summary>
        /// ����������Ƿ������ѯ����
        /// </summary>
        /// <param name="r">������</param>
        /// <returns>�Ƿ������ѯ����</returns>
        private bool CheckAvail(DataRow r)
        {
            if(!allUserName) // ����û�������
            {
                string userName = comboBoxEx1.Text;
                if(r["hostname"].ToString() != userName)
                {
                    return false;
                }
            }

            if(!allModelName) // ���ģ��������
            {
                string modelName = comboBoxEx2.Text;
                if (r["modelid"].ToString() != GetModelID(modelName))
                {
                    return false;
                }
            }

            if(!allTabName) // �������ҳ������
            {
                string tabName = comboBoxEx3.Text;
                if(r["tabid"].ToString() != GetTabID(tabName))
                {
                    return false;
                }
            }

            if(!allCatName) // ����������
            {
                string catName = comboBoxEx8.Text;
                if(r["catfields"].ToString() != catName)
                {
                    return false;
                }
            }

            if(!allNodeName) // ��������������
            {
                string nodeName = comboBoxEx4.Text;
                if(r["nodename"].ToString() != nodeName)
                {
                    return false;
                }
            }

            if(!allFieldName) // ����ֶ�������
            {
                string fieldName = comboBoxEx6.Text;
                if(r["fielddisplayname"].ToString() != fieldName)
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// ����������Ƿ�����ģ����ѯ����
        /// </summary>
        /// <param name="r">������</param>
        /// <returns>�Ƿ������ѯ����</returns>
        private bool CheckRawAvail(DataRow r)
        {
            if (!allUserName) // ����û�������
            {
                string userName = comboBoxEx1.Text;
                if (!r["hostname"].ToString().Contains(userName))
                {
                    return false;
                }
            }

            if (!allModelName) // ���ģ��������
            {
                string modelName = comboBoxEx2.Text;
                if (!GetModelName(r["modelid"].ToString()).Contains(modelName))
                {
                    return false;
                }
            }

            if (!allTabName) // �������ҳ������
            {
                string tabName = comboBoxEx3.Text;
                if (!GetTabName(r["tabid"].ToString()).Contains(tabName))
                {
                    return false;
                }
            }

            if (!allCatName) // ����������
            {
                string catName = comboBoxEx8.Text;
                if (!r["catfields"].ToString().Contains(catName))
                {
                    return false;
                }
            }

            if (!allNodeName) // ��������������
            {
                string nodeName = comboBoxEx4.Text;
                if (!r["nodename"].ToString().Contains(nodeName))
                {
                    return false;
                }
            }

            if (!allFieldName) // ����ֶ�������
            {
                string fieldName = comboBoxEx6.Text;
                if (!r["fielddisplayname"].ToString().Contains(fieldName))
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// ���¼�����ʾ������
        /// </summary>
        /// <param name="exactSearch">�Ƿ�ȷ����</param>
        private void ReLoadData(bool exactSearch)
        {
            if (!beginSearch) // ��û�п�ʼ��ʽ����
            {
                return;
            }

            dataGridView1.Rows.Clear(); // ���������
            int index = 0;
            foreach(DataRow r in dataTable.Rows)
            {
                bool condition;
                if(exactSearch) // ��ȷ����
                {
                    condition = CheckAvail(r);
                }
                else // ģ������
                {
                    condition = CheckRawAvail(r);
                }
                if(condition)
                {
                    dataGridView1.Rows.Add();
                    dataGridView1.Rows[index].Cells["�û���"].Value = r["hostname"];
                    dataGridView1.Rows[index].Cells["�޸�ʱ��"].Value = r["edittime"];
                    dataGridView1.Rows[index].Cells["ģ����"].Value = GetModelName(r["modelid"]);
                    if (r["action"].ToString() == "�޸�ֵ")
                    {
                        dataGridView1.Rows[index].Cells["����ҳ��"].Value = GetTabName(r["tabid"]);
                    }
                    dataGridView1.Rows[index].Cells["��������"].Value = r["catfields"];
                    dataGridView1.Rows[index].Cells["�������"].Value = r["nodename"];
                    dataGridView1.Rows[index].Cells["�ֶ���"].Value = r["fielddisplayname"];
                    dataGridView1.Rows[index].Cells["��ֵ"].Value = r["oldvalue"];
                    dataGridView1.Rows[index].Cells["��ֵ"].Value = r["newvalue"];
                    dataGridView1.Rows[index].Cells["��ע"].Value = r["remarks"];
                    dataGridView1.Rows[index].Cells["����"].Value = r["action"];
                    index++;
                }
            }
        }

        /// <summary>
        /// ��ȡģ������
        /// </summary>
        /// <param name="modeid">ģ��id</param>
        /// <returns>ģ������</returns>
        private string GetModelName(object modeid)
        {
            DataRow r = modelTable.Rows.Find(modeid);
            if(r != null)
            {
                return r["modelname_cn"].ToString();
            }
            else
            {
                return "�Ҳ�����ģ��";
            }
        }

        /// <summary>
        /// ��ȡ����ҳ����
        /// </summary>
        /// <param name="tabid">����ҳid</param>
        /// <returns>����ҳ����</returns>
        private string GetTabName(object tabid)
        {
            DataRow r = tabTable.Rows.Find(tabid);
            if(r != null)
            {
                return r["name"].ToString();
            }
            else
            {
                return "�Ҳ���������ҳ";
            }
        }

        /// <summary>
        /// ����û���
        /// </summary>
        private void FillHostNames()
        {
            comboBoxEx1.Items.Clear();
            comboBoxEx1.Items.Add("�����û�");
            foreach(DataRow r in dataTable.Rows)
            {
                object userName = r["hostname"];
                if(!comboBoxEx1.Items.Contains(userName))
                {
                    comboBoxEx1.Items.Add(userName);
                }
            }
            comboBoxEx1.SelectedIndex = 0;
        }

        /// <summary>
        /// ���ģ����
        /// </summary>
        private void FillModelNames()
        {
            comboBoxEx2.Items.Clear();
            comboBoxEx2.Items.Add("����ģ��");
            foreach(DataRow r in dataTable.Rows)
            {
                object modelName = GetModelName(r["modelid"]);
                if(!comboBoxEx2.Items.Contains(modelName))
                {
                    comboBoxEx2.Items.Add(modelName);
                }
            }
            comboBoxEx2.SelectedIndex = 0;
        }

        /// <summary>
        /// ����ֶ���
        /// </summary>
        private void FillFieldNames()
        {
            comboBoxEx6.Items.Clear();
            comboBoxEx6.Items.Add("�����ֶ�");
            foreach (DataRow r in dataTable.Rows)
            {
                object fieldName = r["fielddisplayname"];
                if (!comboBoxEx6.Items.Contains(fieldName))
                {
                    comboBoxEx6.Items.Add(fieldName);
                }
            }
            comboBoxEx6.SelectedIndex = 0;
        }

        /// <summary>
        /// ��ȡģ���id
        /// </summary>
        /// <param name="modelName">ģ�������</param>
        /// <returns>ģ���id</returns>
        private string GetModelID(string modelName)
        {
            DataRow[] modelRows = modelTable.Select(string.Format("modelname_cn = \'{0}\'", modelName));
            if (modelRows.Length > 0) // �ҵ�ģ���id
            {
                return modelRows[0]["id"].ToString();
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// ��ȡ����ҳ��id
        /// </summary>
        /// <param name="tabName">����ҳ������</param>
        /// <returns>����ҳ��id</returns>
        private string GetTabID(string tabName)
        {
            DataRow[] tabRows = tabTable.Select(string.Format("name = \'{0}\'", tabName));
            if (tabRows.Length > 0) // �ҵ�ģ���id
            {
                return tabRows[0]["modtabid"].ToString();
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// �������ҳ��
        /// </summary>
        private void FillTabNames(string modelName)
        {
            comboBoxEx3.Items.Clear();
            comboBoxEx3.Items.Add("��������ҳ");

            foreach (DataRow r in dataTable.Rows)
            {
                object tabName = GetTabName(r["tabid"]);
                if (!comboBoxEx3.Items.Contains(tabName))
                {
                    comboBoxEx3.Items.Add(tabName);
                }
            }
            comboBoxEx3.SelectedIndex = 0;
        }

        /// <summary>
        /// ����������
        /// </summary>
        private void FillRecordNames()
        {
            comboBoxEx4.Items.Clear();
            comboBoxEx4.Items.Add("���������");
            foreach(DataRow r in dataTable.Rows)
            {
                object recordName = r["nodename"];
                if(!comboBoxEx4.Items.Contains(recordName))
                {
                    comboBoxEx4.Items.Add(recordName);
                }
            }
            comboBoxEx4.SelectedIndex = 0;
        }

        /// <summary>
        /// �����������
        /// </summary>
        public void ResetCatName(string catName)
        {
            comboBoxEx8.Text = catName;
            allCatName = (comboBoxEx8.Text == "���з���");
            ReLoadData(true);
        }

        /// <summary>
        /// �û�ѡ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx8_MouseClick(object sender, MouseEventArgs e)
        {
            Point location = this.Location + (Size)groupPanel1.Location + (new Size(0, 30)) + (Size)groupPanel2.Location + 
                (Size)comboBoxEx8.Location + comboBoxEx8.Size;
            if(location.Y + 220 > Screen.PrimaryScreen.Bounds.Height) // ������Ļ��С
            {
                location.Y = location.Y - 200;
            }
            
            cForm.SetParameters(location, comboBoxEx8.Text, this);
            foreach(DataRow r in dataTable.Rows)
            {
                cForm.AddData(r["catfields"].ToString());
            }
            cForm.InitData();
            cForm.Visible = true;
        }

        /// <summary>
        /// �û�ѡ���µ�ģ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx2_SelectedIndexChanged(object sender, EventArgs e)
        {
            allModelName = (comboBoxEx2.SelectedIndex == 0);
            ReLoadData(true);
            FillTabNames(comboBoxEx2.Text);
        }

        /// <summary>
        /// �û�ѡ���ѯ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                switch(comboBoxEx5.SelectedIndex)
                {
                    case 0: // ��ѯ��ǰ�ֶ���ʷ�޸���Ϣ
                        {
                            SearchFieldData();
                            break;
                        }
                    case 1: // ��ѯ��ǰ��¼��ʷ�޸���Ϣ
                        {
                            SearchRecordData();
                            break;
                        }
                    case 2: // ��ѯ��ǰ����ҳ��ʷ�޸���Ϣ
                        {
                            SearchTabData();
                            break;
                        }
                    case 3: // ��ѯ��ǰģ����ʷ�޸���Ϣ
                        {
                            SearchModelData();
                            break;
                        }
                    case 4: // ��ѯ������ʷ�޸���Ϣ
                        {
                            SearchAllData();
                            break;
                        }
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڲ�ѯ��ʷ��¼ʱ�����쳣: " + ex.ToString());
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
        /// �û�ѡ���µ��û���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx1_SelectedIndexChanged(object sender, EventArgs e)
        {
            allUserName = (comboBoxEx1.SelectedIndex == 0);
            ReLoadData(true);
        }

        /// <summary>
        /// �û�ѡ���µ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx4_SelectedIndexChanged(object sender, EventArgs e)
        {
            allNodeName = (comboBoxEx4.SelectedIndex == 0);
            ReLoadData(true);
        }

        /// <summary>
        /// �û�ѡ��ر�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// �û�ѡ���µ�����ҳ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx3_SelectedIndexChanged(object sender, EventArgs e)
        {
            allTabName = (comboBoxEx3.SelectedIndex == 0);
            ReLoadData(true);
        }

        /// <summary>
        /// �û�ѡ��ģ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            ReLoadData(false);
        }

        /// <summary>
        /// �û����������ı��ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx1_TextChanged(object sender, EventArgs e)
        {
            allUserName = (comboBoxEx1.Text == "�����û�");
        }

        /// <summary>
        /// ģ�����������ı��ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx2_TextChanged(object sender, EventArgs e)
        {
            allModelName = (comboBoxEx2.Text == "����ģ��");
        }

        /// <summary>
        /// ����ҳ���������ı��ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx3_TextChanged(object sender, EventArgs e)
        {
            allTabName = (comboBoxEx3.Text == "��������ҳ");
        }

        /// <summary>
        /// �����������ı��ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx8_TextChanged(object sender, EventArgs e)
        {
            allCatName = (comboBoxEx8.Text == "���з���");
        }

        /// <summary>
        /// ��������������ı��ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx4_TextChanged(object sender, EventArgs e)
        {
            allNodeName = (comboBoxEx4.Text == "���������");
        }

        /// <summary>
        /// �û�ѡ���µ��ֶ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx6_SelectedIndexChanged(object sender, EventArgs e)
        {
            allFieldName = (comboBoxEx6.SelectedIndex == 0);
            ReLoadData(true);
        }

        /// <summary>
        /// �ֶ������ı��ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx6_TextChanged(object sender, EventArgs e)
        {
            allFieldName = (comboBoxEx6.Text == "�����ֶ�");
        }

        /// <summary>
        /// �û�ѡ����˲�ѯ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if(checkBox1.Checked == true)
            {
                groupPanel2.Enabled = true;
            }
            else
            {
                groupPanel2.Enabled = false;
            }
        }

        /// <summary>
        ///  �û�ѡ���ƾ�ֵ
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            try // ���û�δѡ����Ч��Ԫ��ʱ��SelectedCellsҲ�п��ܲ�Ϊ�գ����������쳣
            {
                if (dataGridView1.SelectedCells != null)
                {
                    DataGridViewRow currentRow = dataGridView1.SelectedCells[0].OwningRow;
                    object data = currentRow.Cells["��ֵ"].Value;
                    object fieldDisplayName = currentRow.Cells["�ֶ���"].Value;
                    Clipboard.SetDataObject(data, true);
                    MessageBox.Show(string.Format("{0} �ľ�ֵ {1} �Ѿ����������!", fieldDisplayName, data), "��������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show("δѡ��Ҫ���Ƶ������������!", "��������", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
            catch (Exception ex)
            {
                ex.ToString();
                MessageBox.Show("ѡ���������Ч!", "��������", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        /// <summary>
        /// �û�ѡ������ֵ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX5_Click(object sender, EventArgs e)
        {
            try // ���û�δѡ����Ч��Ԫ��ʱ��SelectedCellsҲ�п��ܲ�Ϊ�գ����������쳣
            {
                if (dataGridView1.SelectedCells != null)
                {
                    DataGridViewRow currentRow = dataGridView1.SelectedCells[0].OwningRow;
                    object data = currentRow.Cells["��ֵ"].Value;
                    object fieldDisplayName = currentRow.Cells["�ֶ���"].Value;
                    Clipboard.SetDataObject(data, true);
                    MessageBox.Show(string.Format("{0} ����ֵ {1} �Ѿ����������!", fieldDisplayName, data), "��������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show("δѡ��Ҫ���Ƶ������������!", "��������", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
            catch (Exception ex)
            {
                ex.ToString();
                MessageBox.Show("ѡ���������Ч!", "��������", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            } 
        }

        private void btnExport_Click(object sender, EventArgs e)
        {
            if (dataGridView1.Rows.Count == 0)
                return;
            SaveFileDialog dlgSaveFile = new SaveFileDialog();
            dlgSaveFile.Filter = "Tab Documents (*.tab) | *.tab | All Files | *.*";
            dlgSaveFile.DefaultExt = "tab";
            if (dlgSaveFile.ShowDialog() == DialogResult.OK)
            {
                string FileFullPath = dlgSaveFile.FileName;
                datagridviewExport(dataGridView1, FileFullPath);
            }
            
        }

        /// <summary>
        /// ��datagridview�����ݵ���
        /// �м�ӵ���\tÿ�к��\r\n
        /// </summary>
        /// <param name="dgv"></param>
        /// <param name="FileFullPath">�ļ�ȫ·����</param>
        /// <returns></returns>
        private bool datagridviewExport(DataGridView dgv, string FileFullPath)
        {
            string strExamPath = FileFullPath.Substring(0, FileFullPath.LastIndexOf('\\'));
            if (!System.IO.Directory.Exists(strExamPath))
            {
                MessageBox.Show(FileFullPath, "Ŀ¼����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            StringBuilder FileContent = new StringBuilder();

            foreach (DataGridViewColumn dgvc in dataGridView1.Columns)
            {
                FileContent.Append(dgvc.HeaderText);
                FileContent.Append('\t');
            }
            FileContent.Remove(FileContent.Length - 1, 1);
            FileContent.Append("\r\n");

            foreach (DataGridViewRow dgvr in dataGridView1.Rows)
            {
                foreach (DataGridViewCell dgvcell in dgvr.Cells)
                {                   
                    FileContent.Append(dgvcell.Value.ToString());
                    FileContent.Append('\t');
                }
                FileContent.Remove(FileContent.Length - 1, 1);
                FileContent.Append("\r\n");
            }
            try
            {
                FileFolderHelper.StringToFile(FileContent.ToString(), FileFullPath);
                return true;
            }
            catch (Exception ex)
            {
                string s = string.Format("���� tab �ļ�:\r\n    {0}\r\nʱ���ִ��������ļ��������̿ռ�ȡ�\r\n\r\n����ѶϢΪ:{1}", FileFullPath, ex.Message);
                System.Windows.Forms.MessageBox.Show(s, "Error");
                return false;
            } 
        }
    }
}