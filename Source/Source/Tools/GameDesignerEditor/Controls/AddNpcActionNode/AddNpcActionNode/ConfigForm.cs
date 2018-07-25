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
    public partial class ConfigForm : Form
    {
        private SqlConnection conn; // sql����
        private string animationID; // ����id
        //private string priority; // ���ȼ�
        private string kindid; // ������������
        private string loop; // �Ƿ�ѭ��
        private string filePath; // �ļ�·��
        private string representID; // ����id
        private string modelName; // ģ������
        private DataTable tbl_npc_ani_relation;

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        public ConfigForm(SqlConnection conn)
        {
            this.conn = conn;
            InitializeComponent();
            InitData();
        }

        public void InitFileInfo(string representID)
        {
            this.representID = representID;
            GetFilePath();
        }

        private void InitData()
        {
            string sql = "SELECT * FROM animation_npc_relation ORDER BY ��������";
            tbl_npc_ani_relation = Helper.GetDataTable(sql, conn);
            foreach (DataRow row in tbl_npc_ani_relation.Rows)
            {
                comboBoxEx1.Items.Add(row["״̬��"].ToString().Trim());
            }
        }

        /// <summary>
        /// ģ����������
        /// </summary>
        public string ModelName
        {
            get
            {
                return modelName;
            }
        }

        /// <summary>
        /// �ļ�·������
        /// </summary>
        public string FilePath
        {
            get
            {
                return filePath;
            }
        }

        /// <summary>
        /// ����id����
        /// </summary>
        public String AnimationID
        {
            get
            {
                return animationID;
            }
        }

//         /// <summary>
//         /// ���ȼ�����
//         /// </summary>
//         public String Priority
//         {
//             get
//             {
//                 return priority;
//             }
//         }

        /// <summary>
        /// ��������
        /// </summary>
        public String KindID
        {
            get
            {
                return kindid;
            }
        }

        /// <summary>
        /// �Ƿ�ѭ��
        /// </summary>
        public String Loop
        {
            get
            {
                return loop;
            }
        }

        /// <summary>
        /// �������ID
        /// </summary>
        /// <param name="index">�������</param>
        private void FillCatifyID(/*int index*/)
        {
            comboBoxEx2.Items.Clear();
            string stateName = comboBoxEx1.Text;
            if (stateName.Length != 0)
            {
                DataRow rrr = tbl_npc_ani_relation.Rows.Find(stateName);
                if (rrr == null)
                    rrr = tbl_npc_ani_relation.Rows.Find(stateName + "\r\n");

                string strID = rrr["ID"].ToString().Trim();
                string[] strIDs = strID.Split(new char[] { '-' });
                if (strIDs.Length == 2)
                {
                    int iStart = Convert.ToInt32(strIDs[0]);
                    int iEnd = Convert.ToInt32(strIDs[1]);
                    for (int i = iStart; i <= iEnd; i++)
                    {
                        comboBoxEx2.Items.Add(i.ToString());
                    }
                }
                else if (strIDs.Length == 1)
                {
                    comboBoxEx2.Items.Add(strIDs[0]);
                }
            }

            comboBoxEx2.SelectedIndex = 0; // Ĭ��ѡ�е�һ��ѡ��
        }

        /// <summary>
        /// ��ȡ�ļ�·��
        /// </summary>
        private void GetFilePath()
        {
            string sqlString = string.Format("SELECT MainModelFile FROM {0} WHERE RepresentID = '{1}'", "npc", representID);
            DataTable table = Helper.GetDataTable(sqlString, conn);
            if(table.Rows.Count > 0)
            {
                filePath = table.Rows[0][0].ToString();
            }
        }

        /// <summary>
        /// ������ȼ�
        /// </summary>
        /// <param name="index">�������</param>
        private void FillPropertyID(/*int index*/)
        {
            string stateName = comboBoxEx1.Text;
            if (stateName.Length != 0)
            {
                DataRow rrr = tbl_npc_ani_relation.Rows.Find(stateName);
                if (rrr == null)
                    rrr = tbl_npc_ani_relation.Rows.Find(stateName + "\r\n");

                textBoxX2.Text = rrr["��������"].ToString().Trim();
                loop = rrr["Ĭ��ѭ��"].ToString().Trim();
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            animationID = comboBoxEx2.Text;
            //priority = textBoxX1.Text;
            kindid = textBoxX2.Text;

            if(animationID == "")
            {
                MessageBox.Show("����id����Ϊ��!");
                return;
            }

//             if(priority == "")
//             {
//                 MessageBox.Show("�������ȼ�����Ϊ��!");
//                 return;
//             }

            if(kindid == "")
            {
                MessageBox.Show("�����������Ͳ���Ϊ��!");
                return;
            }

            string[] data = filePath.Split(new char[] { '\\' });
            filePath = "";
            modelName = data[3];
            for (int i = 0; i < 4; i++)
            {
                filePath += data[i] + "\\";
            }
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
            DialogResult = DialogResult.No;
            this.Close();
        }

        /// <summary>
        /// �û�ѡ���µĶ�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int index = comboBoxEx1.SelectedIndex; // ѡ�е����
            FillPropertyID(/*index*/);
            FillCatifyID(/*index*/);
        }
    }
}