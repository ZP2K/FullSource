using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;

namespace PatrolIO
{
    public partial class PatrolEditor : Form
    {
        private SqlConnection conn; // sql����
        private string mapName; // ��ͼ��
        private string patrolIndex; // ·���ı��
        private string patrolPointIndex; // ·����ı�� 
        private string patrolName; // ·��������
        private DataTable table; // ���ݱ�
        private bool beginEdit = false; // �Ƿ��Ѿ���ʼ�༭
        private Hashtable patrolTable = new Hashtable(); // ����·��ID�Ĺ�ϣ��
        private Hashtable patrolPointTable = new Hashtable(); // ����·����ID�Ĺ�ϣ��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="mapName">��ͼ��</param>
        public PatrolEditor(SqlConnection conn, string mapName)
        {
            this.conn = conn;
            this.mapName = mapName;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM PatrolInfo WHERE MapName = '{0}' ORDER BY PatrolID", mapName);
                table = GetDataTable(sqlString);

                List<string> list = new List<string>();
                foreach (DataRow row in table.Rows)
                {
                    if (!list.Contains(row["PatrolID"].ToString()))
                    {
                        string displayText = string.Format("{0} {1}", row["PatrolID"], row["PatrolName"]);
                        patrolTable[displayText] = row["PatrolID"].ToString();
                        comboBoxEx1.Items.Add(displayText);
                        list.Add(row["PatrolID"].ToString());
                    }                    
                }

                beginEdit = true;
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ·����Ϣʱ����sql�쳣��" + ex.ToString());
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
        /// ·���ı��
        /// </summary>
        public string PatrolIndex
        {
            get
            {
                return patrolIndex;
            }
        }

        /// <summary>
        /// ·��������
        /// </summary>
        public string PatrolName
        {
            get
            {
                return patrolName;
            }
        }

        /// <summary>
        /// ·����ı��
        /// </summary>
        public string PatrolPointIndex
        {
            get
            {
                return patrolPointIndex;
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if(comboBoxEx1.Text != "" && comboBoxEx2.Text != "")
            {
                patrolIndex = patrolTable[comboBoxEx1.Text].ToString();
                patrolPointIndex = patrolPointTable[comboBoxEx2.Text].ToString();
                patrolName = comboBoxEx1.Text.Split(new char[] { ' ' })[1];
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("ѡ�������ݲ���Ϊ�գ�", "��Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �û�ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// �л�·��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxEx1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(beginEdit)
            {
                comboBoxEx2.Items.Clear();
                string patrolID = patrolTable[comboBoxEx1.Text].ToString();
                DataRow[] rows = table.Select(string.Format("PatrolID = '{0}'", patrolID), "PatrolPointID");
                foreach (DataRow row in rows)
                {
                    if(row["PatrolPointID"].ToString() != "-1") // ���Ե�����Ϊ"0, 0"��������
                    {
                        string displayText = string.Format("{0} ({1} ,{2})", row["PatrolPointID"], row["X"], row["Y"]);
                        patrolPointTable[displayText] = row["PatrolPointID"];
                        comboBoxEx2.Items.Add(displayText);
                    }                    
                }
            }            
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sql">sql���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sql)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            System.Data.DataTable tbl = ds.Tables[0];
            return tbl;
        }
    }
}