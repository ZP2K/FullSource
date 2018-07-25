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
    public partial class AIArgumentsEditor : Form
    {
        private SqlConnection conn; // sql����
        private string diagramPath; // ����ͼ·��
        private string oldValue; // ��ֵ
        private string aiArguments; // AI����
        private string filtField; // ���˵��ֶ�

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="diagramPath">����ͼ·��</param>
        /// <param name="oldValue">��ֵ</param>
        /// <param name="filtField">���˵��ֶ�</param>
        public AIArgumentsEditor(SqlConnection conn, string diagramPath, string oldValue, string filtField)
        {
            this.conn = conn;
            this.diagramPath = diagramPath;
            this.oldValue = oldValue;
            this.filtField = filtField;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// AI����
        /// </summary>
        public string AIArguments
        {
            get
            {
                return aiArguments;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {           
            // ���������
            DataGridViewTextBoxColumn sortIDColumn = new DataGridViewTextBoxColumn();
            sortIDColumn.Name = "SortID";
            sortIDColumn.HeaderText = "�������";
            sortIDColumn.ReadOnly = true;
            DataGridViewTextBoxColumn chineseNameColumn = new DataGridViewTextBoxColumn();
            chineseNameColumn.Name = "ChineseName";
            chineseNameColumn.HeaderText = "������";
            chineseNameColumn.ReadOnly = true;
            DataGridViewTextBoxColumn englishNameColumn = new DataGridViewTextBoxColumn();
            englishNameColumn.Name = "EnglishName";
            englishNameColumn.HeaderText = "Ӣ����";
            englishNameColumn.ReadOnly = true;
            DataGridViewTextBoxColumn typeColumn = new DataGridViewTextBoxColumn();
            typeColumn.Name = "Type";
            typeColumn.HeaderText = "����";
            typeColumn.ReadOnly = true;
            DataGridViewTextBoxColumn textValueColumn = new DataGridViewTextBoxColumn();
            textValueColumn.Name = "TextValue";
            textValueColumn.HeaderText = "����ֵ";
            DataGridViewComboBoxColumn enumValueColumn = new DataGridViewComboBoxColumn();
            enumValueColumn.Name = "EnumValue";
            enumValueColumn.HeaderText = "����ֵ";

            dataGridViewX1.Columns.Add(sortIDColumn);
            dataGridViewX1.Columns.Add(chineseNameColumn);
            dataGridViewX1.Columns.Add(englishNameColumn);
            dataGridViewX1.Columns.Add(typeColumn);
            dataGridViewX1.Columns.Add(textValueColumn);
            dataGridViewX1.Columns.Add(enumValueColumn);

            // ��ʼ�������ֶ�
            List<string> filtFieldList = new List<string>();
            string[] filtData = filtField.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
            filtFieldList.AddRange(filtData);

            // ���������
            DataTable table = null;
            DataTable enumTable = null;
            DataTable diagramTable = null;
            string sqlString = "";
            string relevatePath = diagramPath.Replace("scripts\\ai\\", "");
            relevatePath = relevatePath.Replace(".lua", "");            

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                sqlString = string.Format("SELECT ID, FullPath FROM changan.s3diagram_rtm.dbo.diagram WHERE Class = 'AIͼ' AND FullPath = '{0}'", relevatePath);
                diagramTable = GetDataTable(sqlString);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡAIͼ����ʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            if(diagramTable.Rows.Count > 0) // �ҵ���Ӧ��AI�ű�
            {
                string diagramID = diagramTable.Rows[0]["ID"].ToString();

                try
                {
                    if (conn.State == ConnectionState.Closed)
                    {
                        conn.Open();
                    }

                    sqlString = string.Format("SELECT * FROM AI_TemplateParamTable WHERE DiagramID = '{0}' ORDER BY SortID",
                        diagramID);
                    table = GetDataTable(sqlString);
                    sqlString = "SELECT * FROM AI_Enum_Define";
                    enumTable = GetDataTable(sqlString);
                }
                catch (SqlException ex)
                {
                    MessageBox.Show("�ڶ�ȡAI��������ʱ����sql�쳣��" + ex.ToString());
                }
                finally
                {
                    if (conn.State == ConnectionState.Open)
                    {
                        conn.Close();
                    }
                }

                List<string[]> valueList = GetValueList();
                int index = 0;
                bool argumentsChanged = CheckArgumentsChanged(valueList, table);

                foreach(DataRow row in table.Rows)
                {
                    dataGridViewX1.Rows.Add(1);
                    DataGridViewRow newRow = dataGridViewX1.Rows[index];

                    string id = row["KeyID"].ToString();
                    string sortID = row["SortID"].ToString();
                    string chineseName = row["name"].ToString();
                    string englishName = row["ename"].ToString();
                    string type = row["type"].ToString();         
                    string defaultValue = row["defaultvalue"].ToString();

                    newRow.Cells["SortID"].Value = sortID;
                    newRow.Cells["SortID"].Tag = id;
                    newRow.Cells["ChineseName"].Value = chineseName;
                    newRow.Cells["EnglishName"].Value = englishName;                

                    string typeName = "";

                    switch(type)
                    {
                        case "-2": // �ַ���
                            {
                                typeName = "�ַ���";
                                break;
                            }
                        case "-1": // ����
                            {
                                typeName = "����";
                                break;
                            }
                        default:
                            {
                                typeName = GetTypeName(type, enumTable);
                                break;
                            }
                    }

                    newRow.Cells["Type"].Value = typeName;
                    newRow.Cells["Type"].Tag = type;                

                    switch(type)
                    {
                        case "-1": // ����
                            {
                                newRow.Cells["EnumValue"].ReadOnly = true;
                                newRow.Cells["EnumValue"].Style.BackColor = Color.LightGray;

                                break;
                            }
                        case "-2": // �ַ���
                            {
                                newRow.Cells["EnumValue"].ReadOnly = true;
                                newRow.Cells["EnumValue"].Style.BackColor = Color.LightGray;

                                break;
                            }
                        default: // ö��
                            {
                                DataGridViewComboBoxCell comboCell = (DataGridViewComboBoxCell)newRow.Cells["EnumValue"];
                                List<string> enumValues = GetEnumValues(type, enumTable);
                                foreach (string s in enumValues)
                                {
                                    comboCell.Items.Add(s);
                                }
                                newRow.Cells["TextValue"].ReadOnly = true;
                                newRow.Cells["TextValue"].Style.BackColor = Color.LightGray;

                                break;
                            }
                    }

                    if(valueList.Count > 0) // ������ֵ
                    {
                        if(argumentsChanged) // �����Ѿ��ı�
                        {
                            foreach(string[] datas in valueList)
                            {
                                if(datas[0] == row["KeyID"].ToString()) // �ҵ���Ӧ�Ĳ���
                                {
                                    switch (type)
                                    {
                                        case "-2": // �ַ���
                                            {
                                                newRow.Cells["TextValue"].Value = datas[1];

                                                break;
                                            }
                                        case "-1": // ����
                                            {
                                                newRow.Cells["TextValue"].Value = datas[1];

                                                break;
                                            }
                                        default:
                                            {
                                                DataGridViewComboBoxCell comboCell = (DataGridViewComboBoxCell)newRow.Cells["EnumValue"];
                                                newRow.Cells["EnumValue"].Value = comboCell.Items[int.Parse(datas[1])].ToString();

                                                break;
                                            }
                                    }

                                    break;
                                }
                                else // û�ҵ���Ӧ�Ĳ���
                                {
                                    switch (type)
                                    {
                                        case "-2": // �ַ���
                                            {
                                                newRow.Cells["TextValue"].Value = defaultValue;

                                                break;
                                            }
                                        case "-1": // ����
                                            {
                                                newRow.Cells["TextValue"].Value = defaultValue;

                                                break;
                                            }
                                        default:
                                            {
                                                DataGridViewComboBoxCell comboCell = (DataGridViewComboBoxCell)newRow.Cells["EnumValue"];
                                                newRow.Cells["EnumValue"].Value = comboCell.Items[0].ToString();

                                                break;
                                            }
                                    }
                                }
                            }
                        }
                        else
                        {
                            string[] datas = valueList[index];

                            switch (type)
                            {
                                case "-2": // �ַ���
                                    {
                                        newRow.Cells["TextValue"].Value = datas[1];

                                        break;
                                    }
                                case "-1": // ����
                                    {
                                        newRow.Cells["TextValue"].Value = datas[1];

                                        break;
                                    }
                                default:
                                    {
                                        DataGridViewComboBoxCell comboCell = (DataGridViewComboBoxCell)newRow.Cells["EnumValue"];
                                        newRow.Cells["EnumValue"].Value = comboCell.Items[int.Parse(datas[1])].ToString();

                                        break;
                                    }
                            }
                        }                    
                    }
                    else // ��ֵΪ��
                    {
                        switch(type)
                        {
                            case "-2": // �ַ���
                                {
                                    newRow.Cells["TextValue"].Value = defaultValue;

                                    break;
                                }
                            case "-1": // ����
                                {
                                    newRow.Cells["TextValue"].Value = defaultValue;

                                    break;
                                }
                            default:
                                {
                                    DataGridViewComboBoxCell comboCell = (DataGridViewComboBoxCell)newRow.Cells["EnumValue"];
                                    newRow.Cells["EnumValue"].Value = comboCell.Items[0].ToString();

                                    break;
                                }
                        }
                    }

                    if(filtFieldList.Contains(englishName))
                    {
                        newRow.Visible = false;
                    }
                    index++;
                }

                if(argumentsChanged) // ���������仯
                {
                    MessageBox.Show("AIģ��Ĳ��������仯����Ҫ�������ã�", "�����༭", MessageBoxButtons.OK,
                        MessageBoxIcon.Information);
                }
            }                        

            
        }

        /// <summary>
        /// �������Ƿ����仯
        /// </summary>
        /// <param name="valueList">��ֵ����</param>
        /// <param name="table">�������ݱ�</param>
        /// <returns>�Ƿ����仯</returns>
        private bool CheckArgumentsChanged(List<string[]> valueList, DataTable table)
        {
            bool changed = false;

            if(valueList.Count != table.Rows.Count) // ������������
            {
                return true;
            }

            for (int i = 0; i < table.Rows.Count; i++)
            {
                DataRow row = table.Rows[i];
                string[] value = valueList[i]; 
                
                if(row["KeyID"].ToString() != value[0])
                {
                    changed = true;
                    break;
                }                
            }

            return changed;
        }

        /// <summary>
        /// ��ȡö�ٵ�����
        /// </summary>
        /// <param name="type">ö������</param>
        /// <param name="enumTable">ö�����ݱ�</param>
        /// <returns>ö�ٵ�����</returns>
        private string GetTypeName(string type, DataTable enumTable)
        {
            string typeName = "";

            DataRow[] rows = enumTable.Select(string.Format("id = '{0}'", type));
            if(rows.Length > 0)
            {
                DataRow row = rows[0];
                string chineseName = row["name"].ToString();
                string englishName = row["ename"].ToString();
                typeName = string.Format("{0} ({1})", chineseName, englishName);
            }

            return typeName;
        }

        /// <summary>
        /// ��ȡ����ֵ����
        /// </summary>
        /// <returns>����ֵ����</returns>
        private List<string[]> GetValueList()
        {
            List<string[]> valueList = new List<string[]>();

            string[] values = oldValue.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
            foreach(string s in values)
            {
                string[] datas = s.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                valueList.Add(datas);
            }

            return valueList;
        }

        /// <summary>
        /// ��ȡö��ֵ�б�
        /// </summary>
        /// <param name="type">ö������</param>
        /// <param name="enumTable">ö�����ݱ�</param>
        /// <returns>ö��ֵ�б�</returns>
        private List<string> GetEnumValues(string type, DataTable enumTable)
        {
            List<string> enumValues = new List<string>();

            DataRow[] rows = enumTable.Select(string.Format("id = '{0}'", type));
            if(rows.Length > 0)
            {
                DataRow row = rows[0];                
                string[] values = row["values"].ToString().Split(new char[]{'\n'});
                foreach(string s in values)
                {
                    string value = s.TrimEnd(new char[]{'\r'});
                    string[] datas = value.Split(new char[]{','});
                    enumValues.Add(string.Format("{0} ({1})", datas[0], datas[1]));
                }
            }

            return enumValues;
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
        /// ��ȡö��������ı��
        /// </summary>
        /// <param name="comboCell">������Ԫ</param>
        /// <param name="enumValue">ö��ֵ</param>
        /// <returns>ö��������ı��</returns>
        private int GetEnumIndex(DataGridViewComboBoxCell comboCell, string enumValue)
        {
            int index = -1;

            for (int i = 0; i < comboCell.Items.Count; i++)
            {
                string value = comboCell.Items[i].ToString();
                if(value == enumValue)
                {
                    index = i;
                    break;
                }
            }

            return index;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string values = "";
            foreach(DataGridViewRow row in dataGridViewX1.Rows)
            {
                string id = row.Cells["SortID"].Tag.ToString();
                string type = row.Cells["Type"].Tag.ToString();
                string textValue = ""; 
                string enumValue = ""; 

                if(row.Cells["TextValue"].EditedFormattedValue != null)
                {
                    textValue = row.Cells["TextValue"].EditedFormattedValue.ToString();
                }

                if(row.Cells["EnumValue"].EditedFormattedValue != null)
                {
                    enumValue = row.Cells["EnumValue"].EditedFormattedValue.ToString();
                }

                switch(type)
                {
                    case "-2": // �ַ���
                        {
                            values += string.Format("{0},{1};", id, textValue);
                            break;
                        }
                    case "-1": // ����
                        {
                            values += string.Format("{0},{1};", id, textValue);
                            break;
                        }
                    default:
                        {
                            values += string.Format("{0},{1};", id, 
                                GetEnumIndex(row.Cells["EnumValue"] as DataGridViewComboBoxCell, enumValue));
                            break;
                        }
                }                
            }

            aiArguments = values.TrimEnd(new char[] { ';' });

            DialogResult = DialogResult.OK;
            this.Close();
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