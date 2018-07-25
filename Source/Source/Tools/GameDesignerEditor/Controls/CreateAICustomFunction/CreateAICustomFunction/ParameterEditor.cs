using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;

namespace CreateAICustomFunction
{
    public partial class ParameterEditor : Form
    {
        private SqlConnection conn; // sql����
        private string oldValue; // ��ֵ
        private string newValue; // ��ֵ
        private Hashtable typeTable = new Hashtable(); // ֵ���͹�ϣ��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="oldValue">��ֵ</param>
        public ParameterEditor(SqlConnection conn, string oldValue)
        {
            this.conn = conn;
            this.oldValue = oldValue;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ֵ
        /// </summary>
        public string OldValue
        {
            set
            {
                oldValue = value;
            }
        }

        /// <summary>
        /// ��ֵ
        /// </summary>
        public string NewValue
        {
            get
            {
                return newValue;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ���������
            DataGridViewTextBoxColumn indexColumn = new DataGridViewTextBoxColumn();
            indexColumn.Name = "Index";
            indexColumn.HeaderText = "�������";
            indexColumn.ReadOnly = true;
            DataGridViewTextBoxColumn nameColumn = new DataGridViewTextBoxColumn();
            nameColumn.Name = "Name";
            nameColumn.HeaderText = "������";
            DataGridViewComboBoxColumn typeColumn = new DataGridViewComboBoxColumn();
            typeColumn.Name = "Type";
            typeColumn.HeaderText = "��������";
            DataGridViewTextBoxColumn defaultValueColumn = new DataGridViewTextBoxColumn();
            defaultValueColumn.Name = "DefaultValue";
            defaultValueColumn.HeaderText = "Ĭ��ֵ";

            dataGridViewX1.Columns.Add(indexColumn);
            dataGridViewX1.Columns.Add(nameColumn);
            dataGridViewX1.Columns.Add(typeColumn);
            dataGridViewX1.Columns.Add(defaultValueColumn);

            // ��ʼ��������
            DataTable enumTable = null;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM AI_Enum_Define");
                enumTable = GetDataTable(sqlString);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡAIö�����ݱ�ʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            // ���ֵ����
            typeColumn.Items.Add("��");
            typeColumn.Items.Add("����");
            typeColumn.Items.Add("�ַ���");
            typeTable["����"] = "-1";
            typeTable["�ַ���"] = "-2";

            foreach(DataRow row in enumTable.Rows)
            {
                string typeName = row["name"].ToString();
                string enumID = row["id"].ToString();
                typeColumn.Items.Add(typeName);
                typeTable[typeName] = enumID;
            }

            if (oldValue == null)
            {
                oldValue = "";
            }

            // ���������
            string[] valueArray = oldValue.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);

            for (int i = 0; i < 5; i++)
            {
                dataGridViewX1.Rows.Add(1);
                DataGridViewRow newRow = dataGridViewX1.Rows[i];
                int valueIndex = i + 1;
                newRow.Cells["Index"].Value = valueIndex.ToString();

                if (i < valueArray.Length)
                {
                    string[] enumData = valueArray[i].TrimEnd(new char[] { '\r' }).Split(new char[] { ',' });                    
                    newRow.Cells["Name"].Value = enumData[0];
                    string enumType = enumData[1];
                    DataGridViewComboBoxCell typeCell = newRow.Cells["Type"] as DataGridViewComboBoxCell;

                    switch (enumType)
                    {
                        case "-1": // ����
                            {
                                typeCell.Value = "����";
                                break;
                            }
                        case "-2": // �ַ���
                            {
                                typeCell.Value = "�ַ���";
                                break;
                            }
                        default:
                            {
                                typeCell.Value = enumType;
                                break;
                            }
                    }

                    if (enumData.Length > 2) // ��Ĭ��ֵ
                    {
                        newRow.Cells["DefaultValue"].Value = enumData[2];
                    }
                    else
                    {
                        newRow.Cells["DefaultValue"].Value = "0";
                    }
                }
                else
                {
                    newRow.Cells["Type"].Value = "��";
                }
            }                         
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
            string enumValue = "";
            bool editSuccess = true;

            for (int i = 0; i < 5; i++)
            {
                DataGridViewRow row = dataGridViewX1.Rows[i];
                if(row.Cells["Type"].Value.ToString() != "��")
                {
                    string argumentName = ""; 
                    string argumentType = "";
                    string argumentDefaultValue = "";

                    if (row.Cells["Name"].EditedFormattedValue != null)
                    {
                        argumentName = row.Cells["Name"].Value.ToString();
                    }
                    
                    if (row.Cells["Type"].EditedFormattedValue != null)
                    {
                        argumentType = row.Cells["Type"].Value.ToString();
                    }

                    if (row.Cells["DefaultValue"].EditedFormattedValue != null)
                    {
                        argumentDefaultValue = row.Cells["DefaultValue"].Value.ToString();
                    }

                    if (argumentName == "" || argumentType == "" || argumentDefaultValue == "")
                    {
                        MessageBox.Show("����û��������ȫ��", "���ò���", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        editSuccess = false;
                        break;
                    }

                    enumValue += string.Format("{0},{1},{2}\r\n", argumentName, typeTable[argumentType] as string,
                        argumentDefaultValue);
                }
                else
                {
                    break;
                }
            }

            if (editSuccess)
            {
                newValue = enumValue;
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
    }
}