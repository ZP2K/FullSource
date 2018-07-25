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
    public partial class EnumValueSelector : Form
    {
        private SqlConnection conn; // sql����
        private string enumType; // ö������
        private string oldValue; // ��ֵ
        private int enumValueIndex; // ö��ֵ�����
        private string enumValueString; // ö��ֵ���ַ���

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="aiChartID">AIͼID</param>
        /// <param name="oldValue">��ֵ</param>
        public EnumValueSelector(SqlConnection conn, string enumType, string oldValue)
        {
            this.conn = conn;
            this.enumType = enumType;
            this.oldValue = oldValue;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ö��ֵ�����
        /// </summary>
        public int EnumValueIndex
        {
            get
            {
                return enumValueIndex;
            }
        }

        /// <summary>
        /// ö��ֵ���ַ���
        /// </summary>
        public string EnumValueString
        {
            get
            {
                return enumValueString;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {            
            // ��ʼ��������
            DataTable table = null;

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM AI_Enum_Define WHERE id = '{0}'", enumType);
                table = GetDataTable(sqlString);               
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡAIģ�����ʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            if(table.Rows.Count > 0)
            {
                DataRow row = table.Rows[0];
                string chineseName = row["name"].ToString();
                string englishName = row["ename"].ToString();
                string enumValues = row["values"].ToString();

                string[] datas = enumValues.Split(new char[] { '\n' });
                
                for(int i = 0; i < datas.Length; i++)
                {
                    string s = datas[i];
                    string[] values = s.TrimEnd(new char[] { ',' }).Split(new char[] { ',' });
                    comboBoxEx1.Items.Add(string.Format("{0} ({1})", values[0], values[1]));
                }              

                if(oldValue != null && oldValue != "")
                {
                    comboBoxEx1.SelectedIndex = int.Parse(oldValue);
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
            enumValueString = comboBoxEx1.Text;
            enumValueIndex = comboBoxEx1.SelectedIndex;

            if(enumValueIndex == -1)
            {
                MessageBox.Show("��������Ϊ�գ�", "ѡ��ö��ֵ", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }
            else
            {
                DialogResult = DialogResult.OK;
                this.Close();
            }
        }
    }
}