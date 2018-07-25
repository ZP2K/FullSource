using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace StandardNpcCreate
{
    public partial class MultiUpdateSkillForm : Form
    {
        private SqlConnection conn; // sql����
        private List<string> npcInfoList; // npc��Ϣ����
        private List<string> selectedNpcInfoList = new List<string>(); // ѡ�е�npc��Ϣ����
        private bool selectAll = true;

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="npcInfoList">npc��Ϣ����</param>
        public MultiUpdateSkillForm(SqlConnection conn, List<string> npcInfoList)
        {
            this.conn = conn;
            this.npcInfoList = npcInfoList;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ѡ�е�npc��Ϣ����
        /// </summary>
        public List<string> SelectedNpcInfoList
        {
            get
            {
                return selectedNpcInfoList;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            foreach(string s in npcInfoList)
            {
                checkedListBox1.Items.Add(s);
            }
        }

        /// <summary>
        /// ��NPC�����ַ�����ȡ��NPC��ID
        /// </summary>
        /// <param name="npcString">NPC�����ַ���</param>
        /// <return>NPC��ID</return>
        private int GetNpcID(string npcString)
        {
            int npcID = -1;

            int firstIndex = npcString.LastIndexOf("(");
            int lastIndex = npcString.LastIndexOf(")");

            if (firstIndex >= 0 && lastIndex >= 0)
            {
                npcID = int.Parse(npcString.Substring(firstIndex + 1, lastIndex - firstIndex - 1));
            }

            return npcID;
        }

        /// <summary>
        /// ��NPC�����ַ�����ȡ��NPC������
        /// </summary>
        /// <param name="npcString">NPC�����ַ���</param>
        /// <returns>NPC������</returns>
        private string GetNpcName(string npcString)
        {
            string npcName = null;

            int index = npcString.LastIndexOf("(");

            if (index > 0)
            {
                npcName = npcString.Substring(0, index - 1);
            }

            return npcName;
        }

        /// <summary>
        /// ���/ȡ���������NPC
        /// </summary>
        /// <param name="npcID">npc��ģ��ID</param>
        /// <param name="mark">�Ƿ���</param>
        private void SetNpcSpecific(string npcID, bool mark)
        {
            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT ID FROM npc_skill_record WHERE ID = '{0}'", npcID);
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;

                object executeResult = cmd.ExecuteScalar();

                if (executeResult == null) // ��Ҫ����
                {
                    sqlString = string.Format("INSERT INTO npc_skill_record (ID) VALUES ('{0}')", npcID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }

                if (mark)
                {
                    sqlString = string.Format("UPDATE npc_skill_record SET IsSpecific = 1 WHERE ID = '{0}'", npcID);
                }
                else
                {
                    sqlString = string.Format("UPDATE npc_skill_record SET IsSpecific = 0 WHERE ID = '{0}'", npcID);
                }

                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڸ�������NPC��Ϣʱ����sql�쳣��" + ex.Message, "��������NPC��Ϣ",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                if (checkedListBox1.GetItemChecked(i))
                {
                    string npcInfo = checkedListBox1.Items[i].ToString();

                    if (!npcInfo.EndsWith("[����]")) // ����NPC���������
                    {
                        selectedNpcInfoList.Add(npcInfo);
                    }                    
                }
            }

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

        /// <summary>
        /// ȫ��ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                checkedListBox1.SetItemChecked(i, selectAll);
            }

            if (selectAll)
            {
                buttonX3.Text = "ȫ��ȡ��";                
            }
            else
            {
                buttonX3.Text = "ȫ��ѡ��";                
            }

            selectAll = !selectAll;
        }

        /// <summary>
        /// ѡ��δ��ڶ����ܵ�NPC
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < npcInfoList.Count; i++)
            {
                if (npcInfoList[i].EndsWith("[δ��ڶ�����]"))
                {
                    checkedListBox1.SetItemChecked(i, true);
                }
                else
                {
                    checkedListBox1.SetItemChecked(i, false);
                }
            }
        }

        /// <summary>
        /// ���Ϊ����NPC
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bMarkSpecificNpc_Click(object sender, EventArgs e)
        {
            int selectedIndex = checkedListBox1.SelectedIndex;

            if (selectedIndex >= 0 && selectedIndex < checkedListBox1.Items.Count)
            {
                string oldValue = checkedListBox1.Items[selectedIndex].ToString();
                string newString = string.Format("{0} [����]", oldValue.Replace("[δ��ڶ�����]", "").Trim()); ;
                checkedListBox1.Items[selectedIndex] = newString;
                checkedListBox1.SetItemChecked(selectedIndex, false);

                string npcID = GetNpcID(newString).ToString();
                SetNpcSpecific(npcID, true);
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ��ǵ�NPC", "�������NPC", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ȡ�����Ϊ����NPC
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bUnmarkSpecificNpc_Click(object sender, EventArgs e)
        {
            int selectedIndex = checkedListBox1.SelectedIndex;

            if (selectedIndex >= 0 && selectedIndex < checkedListBox1.Items.Count)
            {
                string oldValue = checkedListBox1.Items[selectedIndex].ToString();
                string newString = oldValue.Replace("[����]", "").Trim();
                checkedListBox1.Items[selectedIndex] = newString;
                checkedListBox1.SetItemChecked(selectedIndex, true);

                string npcID = GetNpcID(newString).ToString();
                SetNpcSpecific(npcID, false);
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫȡ����ǵ�NPC", "ȡ���������NPC", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }
        }
    }
}