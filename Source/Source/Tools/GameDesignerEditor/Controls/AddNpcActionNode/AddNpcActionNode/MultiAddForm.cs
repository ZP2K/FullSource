using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace AddNpcActionNode
{
    public partial class MultiAddForm : Form
    {
        private SqlConnection conn; // ���ݿ�����
        private string[] actionIndexArray = new string[12]; // ���������������
        //private string[] propertyArray = new string[12]; // �������ȼ�����
        private string[] kindIdArray = new string[12]; // ����������������
        private string[] loopArray = new string[12]; // �Ƿ�ѭ������ 
        private string[] dataArray = new string[12]; // Ԥ����������

        /// <summary>
        /// ���캯��
        /// </summary>
        public MultiAddForm(SqlConnection conn)
        {
            this.conn = conn;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// �������������������
        /// </summary>
        public string[] ActionIndexArray
        {
            get
            {
                return actionIndexArray;
            }
        }

//         /// <summary>
//         /// ���ȼ���������
//         /// </summary>
//         public string[] PropertyArray
//         {
//             get
//             {
//                 return propertyArray;
//             }
//         }

        /// <summary>
        /// ����������������
        /// </summary>
        public string[] KindIDArray
        {
            get
            {
                return kindIdArray;
            }
        }

        /// <summary>
        /// �Ƿ�ѭ������
        /// </summary>
        public string[] LoopArray
        {
            get
            {
                return loopArray;
            }
        }

        /// <summary>
        /// ��ʼ��
        /// </summary>
        private void Init()
        {
            checkedListBox1.Items.Add("���� (1)", true);
            checkedListBox1.Items.Add("���� (9)", true);
            checkedListBox1.Items.Add("�ܲ� (16)", true);
            checkedListBox1.Items.Add("��ͨ����1 (30)", true);
            checkedListBox1.Items.Add("��ͨ����2 (31)", false); 
            checkedListBox1.Items.Add("��ͨ����3 (32)", false);
            checkedListBox1.Items.Add("��ͨ����4 (33)", false);
            checkedListBox1.Items.Add("ս������ (47)", false);
            checkedListBox1.Items.Add("ս������1 (54)", false);
            checkedListBox1.Items.Add("ս������2 (55)", false);
            checkedListBox1.Items.Add("ս������3 (56)", false);
            checkedListBox1.Items.Add("ս������ (73)", false);

            for(int i = 0; i < actionIndexArray.Length; i++)
            {
                actionIndexArray[i] = "0";
            }

            dataArray[0] = "1";
            dataArray[1] = "9";
            dataArray[2] = "16";
            dataArray[3] = "30";
            dataArray[4] = "31";
            dataArray[5] = "32";
            dataArray[6] = "33";
            dataArray[7] = "47";
            dataArray[8] = "54";
            dataArray[9] = "55";
            dataArray[10] = "56";
            dataArray[11] = "73";

            kindIdArray[0] = "8";
            kindIdArray[1] = "2";
            kindIdArray[2] = "3";
            kindIdArray[3] = "1";
            kindIdArray[4] = "1";
            kindIdArray[5] = "1";
            kindIdArray[6] = "1";
            kindIdArray[7] = "4";
            kindIdArray[8] = "5";
            kindIdArray[9] = "5";
            kindIdArray[10] = "5";
            kindIdArray[11] = "6";

            loopArray[0] = "0";
            loopArray[1] = "1";
            loopArray[2] = "1";
            loopArray[3] = "1";
            loopArray[4] = "1";
            loopArray[5] = "1";
            loopArray[6] = "1";
            loopArray[7] = "1";
            loopArray[8] = "0";
            loopArray[9] = "0";
            loopArray[10] = "0";
            loopArray[11] = "0";

//             propertyArray[0] = "1013";
//             propertyArray[1] = "1000";
//             propertyArray[2] = "1000";
//             propertyArray[3] = "999";
//             propertyArray[4] = "999";
//             propertyArray[5] = "999";
//             propertyArray[6] = "999";
//             propertyArray[7] = "1001";
//             propertyArray[8] = "1002";
//             propertyArray[9] = "1002";
//             propertyArray[10] = "1002";
//             propertyArray[11] = "1002";

            // ��ȡ�û��Զ�����Ϣ
            string sqlString = string.Format("SELECT data FROM {0} WHERE hostname = '{1}'", 
                "npc_action_config", System.Net.Dns.GetHostName());
            DataTable configTable = GetDataTable(sqlString);
            if(configTable != null && configTable.Rows.Count > 0)
            {
                string data = configTable.Rows[0][0].ToString();
                string[] datas = data.Split(new char[] { ',' });
                for(int i = 0; i < datas.Length; i++)
                {
                    if(datas[i] == "1")
                    {
                        checkedListBox1.SetItemChecked(i, true);
                    }
                    else
                    {
                        checkedListBox1.SetItemChecked(i, false);
                    }
                }
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        private void SaveData()
        {
            string data = "";

            for(int i = 0; i < 12; i++)
            {
                if(checkedListBox1.GetItemChecked(i))
                {
                    actionIndexArray[i] = dataArray[i];
                    data += "1,";
                }
                else
                {
                    data += "0,";
                }
            }

            data = data.TrimEnd(new char[] { ',' });
            bool dataExist = false;

            string hostName = System.Net.Dns.GetHostName();
            string sqlString = string.Format("SELECT data FROM {0} WHERE hostname = '{1}'", 
                "npc_action_config", hostName);
            DataTable table = GetDataTable(sqlString);
            dataExist = table.Rows.Count > 0 ;

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                if(dataExist) // �û����������Ѿ�����
                {
                    sqlString = string.Format("UPDATE {0} SET data = '{1}' WHERE hostname = '{2}'",
                        "npc_action_config", data, hostName);
                }
                else
                {
                    sqlString = string.Format("INSERT INTO {0} (hostname, data) VALUES ('{1}', '{2}')",
                        "npc_action_config", hostName, data);
                }
                SqlCommand cmd = new SqlCommand(sqlString, conn);
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڱ����û�������Ϣʱ����sql�쳣: " + ex.ToString());
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
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sql">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        public DataTable GetDataTable(string sql)
        {
            DataTable tbl = null;

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                tbl = ds.Tables[0];
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڲ�ѯ���ݱ�����ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }       
            
            return tbl;
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            SaveData();
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// �û�ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}