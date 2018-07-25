using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace CustomLoadTree
{
    public partial class Form2 : Form
    {
        private SqlConnection conn; // ���ݿ�����
        private string missionName; // ��������
        private int missionLevel; // ����ȼ�
        private string missionClass; // ����Class
        private string missionCat; // ����_Cat
        private string missionObject; // ����Ŀ��
        private string startMap; // ��ʼ��ͼ
        private string endMap; // ������ͼ
        private bool tongqiMission; // �Ƿ���ͬ������
        private string fatherid; // ������id
        private string fatherLevel; // �����ĵȼ�
        private string fatherStartMap; // �����Ŀ�ʼ��ͼ
        private string fatherEndMap; // �����Ľ�����ͼ
        private List<KeyValuePair<string, string>> list1 = new List<KeyValuePair<string, string>>(); // ��ֵ������

        public Form2(SqlConnection conn, string missionCat)
        {
            this.conn = conn;
            conn.Open();
            InitializeComponent();
            textBoxX4.Text = missionCat;
            comboBoxEx3.Text = missionCat;
            FillList();
            FillClass();
            comboBoxEx1.Text = missionCat;
            comboBoxEx2.Text = missionCat;
            conn.Close();
        }

        public Form2(SqlConnection conn, int fatherid, string missionCat)
        {
            this.conn = conn;
            this.fatherid = fatherid.ToString();
            conn.Open();
            InitializeComponent();
            textBoxX4.Text = missionCat;
            comboBoxEx3.Text = missionCat;
            FillList();
            FillClass();
            GetFatherInfo();
            textBoxX2.Text = fatherLevel;
            comboBoxEx1.Text = fatherStartMap;
            comboBoxEx2.Text = fatherEndMap;
            conn.Close();
        }

        // ������������
        public string MissionName
        {
            get
            {
                return missionName;
            }
        }

        // ����ȼ�����
        public int MissionLevel
        {
            get
            {
                return missionLevel;
            }
        }

        // ������С�ȼ�����
        public int MissionMinLevel
        {
            get
            {
                if (missionLevel - 2 < 0)
                {
                    return 0;
                }
                else
                {
                    return missionLevel - 2;
                }

            }
        }

        // ����Class����
        public string MissionClass
        {
            get
            {
                return missionClass;
            }
        }

        // ����_Cat����
        public string MissionCat
        {
            get
            {
                return missionCat;
            }
        }

        // ����Object����
        public string MissionObject
        {
            get
            {
                return missionObject;
            }
        }

        // ��ʼ��ͼ����
        public string StartMap
        {
            get
            {
                return startMap;
            }
        }

        // ������ͼ����
        public string EndMap
        {
            get
            {
                return endMap;
            }
        }

        // ͬ����������
        public bool TongqiMission
        {
            get
            {
                return tongqiMission;
            }
        }

        // ��ȡ��������Ϣ
        private void GetFatherInfo()
        {
            string sql = string.Format("SELECT QuestLevel, StartMapID, EndMapID FROM tbl_quests WHERE QuestID = {0}", fatherid);
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                reader.Read();
                fatherLevel = reader[0].ToString().Trim();
                fatherStartMap = reader[1].ToString().Trim();
                fatherEndMap = reader[2].ToString().Trim();
                foreach (KeyValuePair<string, string> k in list1)
                {
                    if (fatherStartMap == k.Key)
                    {
                        fatherStartMap = k.Value;
                    }
                    if (fatherEndMap == k.Key)
                    {
                        fatherEndMap = k.Value;
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("��ȡ�������Ϣʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                reader.Close();
            }
        }

        // ���Mapѡ������������
        private void FillList()
        {
            string sql = string.Format("SELECT ID, Name FROM MapList");
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    if (reader[0].ToString().Trim() != "")
                    {
                        list1.Add(new KeyValuePair<string, string>(reader[0].ToString().Trim(), reader[1].ToString().Trim()));
                        comboBoxEx1.Items.Add(reader[1].ToString().Trim());
                        comboBoxEx2.Items.Add(reader[1].ToString().Trim());
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("���Mapѡ������������ʱ����Sql�쳣: " + ex.ToString());
            }
            finally
            {
                reader.Close();
            }
        }

        // ���Classѡ������������
        private void FillClass()
        {
            string sql = string.Format("SELECT DISTINCT Class FROM tbl_quests");
            SqlCommand cmd;
            SqlDataReader reader = null;

            try
            {
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                while (reader.Read())
                {
                    if (reader[0].ToString().Trim() != "")
                    {
                        comboBoxEx3.Items.Add(reader[0].ToString().Trim());
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("���Classѡ������������ʱ����Sql�쳣: " + ex.ToString());
            }
            finally
            {
                reader.Close();
            }
        }

        // ����û��������Ч��
        private bool CheckInput()
        {
            int level;

            if (textBoxX1.Text == "") // ��������Ϊ��
            {
                MessageBox.Show("����������Ϊ��!");
                return false;
            }

            if (textBoxX1.Text.Length > 12)
            {
                MessageBox.Show("���������������!");
                return false;
            }

            if (textBoxX2.Text == "") // ����ȼ�Ϊ��
            {
                MessageBox.Show("����ȼ�����Ϊ��!");
                return false;
            }

            if (comboBoxEx3.Text == "") // ����ClassΪ��
            {
                MessageBox.Show("����Class����Ϊ��!");
                return false;
            }

            if (textBoxX5.Text == "") // ����Ŀ��Ϊ��
            {
                MessageBox.Show("����Ŀ�겻��Ϊ��!");
                return false;
            }

            if (comboBoxEx1.Text == "")
            {
                MessageBox.Show("��ʼ��ͼ����Ϊ��!");
                return false;
            }

            if (comboBoxEx2.Text == "")
            {
                MessageBox.Show("������ͼ����Ϊ��");
                return false;
            }

            try // �������ȼ��Ƿ�Ϊ����
            {
                level = int.Parse(textBoxX2.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show("����ȼ�������Ч��ֵ: " + ex.ToString());
                return false;
            }

            if (level < 0) // �������ȼ������Ƿ�����Ч��Χ��
            {
                return false;
            }

            return true;
        }

        // �û�ѡ��ȷ��
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (CheckInput()) // �û�������Ч
            {
                missionName = textBoxX1.Text;
                missionLevel = int.Parse(textBoxX2.Text);
                missionClass = comboBoxEx3.Text;
                missionCat = textBoxX4.Text;
                missionObject = textBoxX5.Text;
                if (!missionObject.StartsWith("<G>")) // ����Ŀ��ǰ����<G>
                {
                    missionObject = "<G>" + missionObject;
                }
                startMap = comboBoxEx1.Text;
                endMap = comboBoxEx2.Text;
                tongqiMission = checkBoxX1.Checked;
                bool findStartMapIndex = false;
                bool findEndMapIndex = false;

                foreach (KeyValuePair<string, string> k in list1) // ���ƥ���ͼ�ļ�ֵ��
                {
                    if (startMap == k.Value)
                    {
                        startMap = k.Key;
                        findStartMapIndex = true;
                    }
                    if (endMap == k.Value)
                    {
                        endMap = k.Key;
                        findEndMapIndex = true;
                    }
                }

                if(findStartMapIndex && findEndMapIndex)
                {
                    DialogResult = DialogResult.OK;
                    this.Close();
                }
                else
                {
                    if(!findStartMapIndex) // �Ҳ���������ʼ��ͼ��Ӧ�����
                    {
                        MessageBox.Show("�Ҳ���������ʼ��ͼ��Ӧ����ţ������������Ч��");
                    }

                    if(!findEndMapIndex) // �Ҳ������������ͼ��Ӧ�����
                    {
                        MessageBox.Show("�Ҳ������������ͼ��Ӧ����ţ������������Ч�ԣ�");
                    }                    
                }
            }
        }

        // �û�ѡ��ȡ��
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }
    }
}