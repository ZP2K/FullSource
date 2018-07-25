using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;

namespace CustomLoadTree
{
    public partial class MultiTaskEditForm : Form
    {
        private SqlConnection conn; // sql����
        private List<string> taskIDList; // ����id����
        private List<string> fieldList; // �ֶ�����
        private DataTable taskTable; // �������ݱ�
        private bool selectAll = true; // �Ƿ�ȫ��ѡ��
        private bool beginEdit = false; // �Ƿ��Ѿ���ʼ�༭
        private Font boldFont; // ����
        private Font regularFont; // ����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="taskTable">�������ݱ�</param>
        /// <param name="taskIDList">����id����</param>
        /// <param name="fieldList">�ֶ�����</param>
        public MultiTaskEditForm(SqlConnection conn, DataTable taskTable, List<string> taskIDList, List<string> fieldList)
        {
            this.conn = conn;
            this.taskTable = taskTable;
            this.taskIDList = taskIDList;
            this.fieldList = fieldList;
            InitializeComponent();
            InitData();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData()
        {
            // ���������
            DataGridViewColumn selectAllColumn = new DataGridViewCheckBoxColumn();
            selectAllColumn.HeaderText = "�Ƿ񱣴�";
            selectAllColumn.Name = "SelectAllColumn";
            dataGridView1.Columns.Add(selectAllColumn);

            DataGridViewColumn tempColumn;

            int index = 1;
            foreach(string s in fieldList)
            {
                dataGridView1.Columns.Add(s, s);
                tempColumn = dataGridView1.Columns[index];
                tempColumn.ValueType = taskTable.Columns[s].DataType;
                index++;
            }

            // ���������
            DataRow row; // ��ǰ������
            DataGridViewRow gridViewRow; // gridView������
            int rowIndex = 0; // ���������
            foreach(string s in taskIDList)
            {
                row = taskTable.Rows.Find(s);
                if(row != null)
                {
                    dataGridView1.Rows.Add();
                    gridViewRow = dataGridView1.Rows[rowIndex];
                    foreach(string field in fieldList)
                    {
                        gridViewRow.Cells[field].Value = row[field];
                    }
                    gridViewRow.HeaderCell.Value = string.Format("{0} ({1})", row["QuestName"], row["QuestID"]);
                    gridViewRow.HeaderCell.Tag = row["QuestID"];
                    rowIndex++;
                }
            }

            // ��ʼ������
            boldFont = new Font(dataGridView1.DefaultCellStyle.Font, FontStyle.Bold);
            regularFont = new Font(dataGridView1.DefaultCellStyle.Font, FontStyle.Regular);

            // ��ʼ�༭
            beginEdit = true;
        }

        /// <summary>
        /// ��ȡ�ֶζ�Ӧ��tabҳid
        /// </summary>
        /// <param name="field">�ֶ���</param>
        /// <returns>�����ֶζ�Ӧ����Ϣ������������</returns>
        private DataRow[] GetFieldInfo(string field)
        {
            DataRow[] rows = null;

            try
            {
                string sqlString = string.Format("SELECT fieldid, fieldcnname, modtabid FROM {0} WHERE fieldname = '{1}'",
                    "sys_meta_info", field);
                DataTable table = Helper.GetDataTable(sqlString, conn);
                if(table.Rows.Count > 0)
                {
                    rows = new DataRow[table.Rows.Count];
                    for (int i = 0; i < table.Rows.Count; i++)
                    {
                        rows[i] = table.Rows[i];
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡtabҳ��idֵʱ����sql�쳣: " + ex.ToString());
            }

            return rows;
        }

        /// <summary>
        /// ����ֵ�仯д�����ݿ�
        /// </summary>
        /// <param name="id">����id</param>
        /// <param name="field">�����ֶ�</param>
        /// <param name="value">������ֵ</param>
        private void UpdateData(string id, string field, string newValue, string oldValue, string strRemarks)
        {
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                // �޸��ֶ���Ϣ
                string sqlString = string.Format("UPDATE {0} SET {1} = '{2}' WHERE QuestID = '{3}'", "tbl_quests", field, newValue, id);
                if(newValue == "") // �������ֿ�ֵ���������
                {
                    sqlString = string.Format("UPDATE {0} SET {1} = NULL WHERE QuestID = '{2}'", "tbl_quests", field, id);
                }
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();

                // ��¼�ֶ��޸���ʷ
                DataRow[] fieldInfo = GetFieldInfo(field);
                if(fieldInfo != null) // �ҵ���Ӧ���ֶ���Ϣ������
                {
                    DataRow taskRow = taskTable.Rows.Find(id);
                    
                    int modelID = 63;                
                    string userName = Helper.GetHostName();
                    string key = id;
                    string nodeName = taskRow["QuestName"].ToString();
                    string catFields = taskRow["_Cat"].ToString();
                    DateTime time = DateTime.Now;
                     
                    foreach(DataRow r in fieldInfo) // ��Щ�ֶο����ڶ��tabҳ�г��֣�����Ҫ�ౣ��һ���ֶ��޸���ʷ��
                    {
                        int tabID = (int)r["modtabid"];
                        int fieldID = (int)r["fieldid"];
                        string fieldName = r["fieldcnname"].ToString();
                        sqlString = string.Format("INSERT INTO {0} (hostname, edittime, keys, oldvalue, newvalue, modelid, tabid, nodename, " +
                                                "catfields, fielddisplayname, fieldid, remarks) VALUES ('{1}', '{2}', '{3}', '{4}', '{5}', '{6}', '{7}', '{8}', '{9}', '{10}', '{11}', '{12}')",
                                                "sys_dataRecord", userName, time, key, oldValue, newValue, modelID, tabID, nodeName, catFields, fieldName, fieldID, strRemarks);
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                    }
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show(string.Format("�ڸ���idΪ{0}�������{1}�ֶ���ֵʱ����sql�쳣: " + ex.ToString(), id, field));
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
        /// ��������
        /// </summary>
        private void SaveData(string strRemarks)
        {
            string id; // �����id
            DataRow row; // ������
            string newValue; // ��ֵ
            int editTaskCount = 0; // ���µ�������
            bool valueChanged = false; // �����е�ֵ�Ƿ��Ѹı�

            foreach(DataGridViewRow r in dataGridView1.Rows)
            {
                if((bool)r.Cells["selectAllColumn"].EditedFormattedValue) // ���б�ѡ��
                {
                    valueChanged = false;
                    id = r.HeaderCell.Tag.ToString();
                    row = taskTable.Rows.Find(id);
                    
                    if (row != null)
                    {
                        foreach (string s in fieldList)
                        {
                            newValue = r.Cells[s].EditedFormattedValue.ToString();
                            if (newValue != row[s].ToString())
                            {
                                UpdateData(id, s, newValue, row[s].ToString(), strRemarks);
                                r.Cells[s].Style.Font = regularFont; // ��������
                                // ������ֵ
                                if (newValue == "") // ��ֵ��Ҫ���⴦��
                                {
                                    row[s] = DBNull.Value;
                                } 
                                else
                                {
                                    row[s] = r.Cells[s].EditedFormattedValue;
                                }
                                 
                                valueChanged = true;
                            }
                        }

                        if(valueChanged) // �������е�ֵ�Ѿ��ı�
                        {
                            editTaskCount++;
                        }
                    }
                }
                
            }

            MessageBox.Show(string.Format("����{0}����������ݱ�����!", editTaskCount.ToString()), "���ݸ��³ɹ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        /// <summary>
        /// ȫ��ѡ�л�ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bSelectAll_Click(object sender, EventArgs e)
        {
            if(selectAll)
            {
                foreach(DataGridViewRow r in dataGridView1.Rows)
                {
                    r.Cells["SelectAllColumn"].Value = selectAll;
                }
                bSelectAll.Text = "ȫ��ȡ��";
                selectAll = false;
            }
            else
            {
                foreach (DataGridViewRow r in dataGridView1.Rows)
                {
                    r.Cells["SelectAllColumn"].Value = selectAll;
                }
                bSelectAll.Text = "ȫ��ѡ��";
                selectAll = true;
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bSave_Click(object sender, EventArgs e)
        {
            if (HasChanged())
            {
                string strRemarks = "δ��ӱ�ע";
                if (Class1.CommentOnSave)
                {
                    GameDesingerTools.InputBoxForm ibf = new GameDesingerTools.InputBoxForm();
                    if (ibf.ShowDialog() == DialogResult.OK)
                    {
                        strRemarks = ibf.Remarks;
                    }
                    else
                    {
                        return;
                    }                   
                }
                SaveData(strRemarks);
            }
            else
            {
                MessageBox.Show("ѡ�е���û�����ݱ��ı䡣");
            }
            
        }

        /// <summary>
        /// ��Ԫ��ֵ�ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (beginEdit)
            {
                dataGridView1[e.ColumnIndex, e.RowIndex].Style.Font = boldFont;
                if (dataGridView1.SelectedCells.Count > 1)
                {
                    foreach (DataGridViewCell cell in dataGridView1.SelectedCells)
                    {
                        cell.Value = dataGridView1[e.ColumnIndex, e.RowIndex].Value;
                    }
                }
            }
            
        }

        private bool HasChanged()
        {
            string id; // �����id
            DataRow row; // ������
            string newValue; // ��ֵ
            
            foreach (DataGridViewRow r in dataGridView1.Rows)
            {
                if ((bool)r.Cells["selectAllColumn"].EditedFormattedValue) // ���б�ѡ��
                {   
                    id = r.HeaderCell.Tag.ToString();
                    row = taskTable.Rows.Find(id);

                    if (row != null)
                    {
                        foreach (string s in fieldList)
                        {
                            newValue = r.Cells[s].EditedFormattedValue.ToString();
                            if (newValue != row[s].ToString())
                            {
                                return true;                                
                            }
                        }
                    }
                }
            }

            return false;
        }

    }
}