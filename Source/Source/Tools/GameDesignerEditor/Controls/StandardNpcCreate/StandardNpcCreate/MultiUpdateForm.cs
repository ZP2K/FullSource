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
    public partial class MultiUpdateForm : Form
    {
        private List<string> selectedNpcList; // ѡ�е�npc����
        private bool selectAll = true; // �Ƿ�ȫ��ѡ��
        private SqlConnection conn; // sql����
        private string mapName; // ��ͼ��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="selectedNpcList">ѡ�е�npc����</param>
        /// <param name="conn">sql����</param>
        /// <param name="mapName">��ͼ��</param>
        public MultiUpdateForm(List<string> selectedNpcList, SqlConnection conn, string mapName)
        {
            this.selectedNpcList = selectedNpcList;
            this.conn = conn;
            this.mapName = mapName;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// �Ƿ�������ж���
        /// </summary>
        public bool UpdateExistAnimation
        {
            get
            {
                return checkBoxX1.Checked;
            }
        }

        /// <summary>
        /// �Ƿ������ͨ��������
        /// </summary>
        public bool UpdateCommonStandbyAnimation
        {
            get
            {
                return !checkBoxX2.Checked;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {            
            foreach(string s in selectedNpcList)
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
        /// <param name="npcName">npc������</param>
        /// <param name="mark">�Ƿ���</param>
        private void SetNpcSpecific(string npcID, string npcName, bool mark)
        {           
            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT Name FROM npc_animation_record WHERE ID = '{0}'", npcID);
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;

                object executeResult = cmd.ExecuteScalar();

                if (executeResult == null) // ��Ҫ����
                {                                
                    sqlString = string.Format("INSERT INTO npc_animation_record (ID, MapName, Name) VALUES ('{0}', '{1}', '{2}')",
                                                            npcID, mapName, npcName);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }

                if (mark)
                {
                    sqlString = string.Format("UPDATE npc_animation_record SET IsSpecific = 1 WHERE ID = '{0}'", npcID);
                }
                else
                {
                    sqlString = string.Format("UPDATE npc_animation_record SET IsSpecific = NULL WHERE ID = '{0}'", npcID);
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
            selectedNpcList.Clear();

            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {                
                if (checkedListBox1.GetItemChecked(i))
                {
                    string npcName = checkedListBox1.Items[i] as string;

                    if (!npcName.EndsWith("[����]")) // ����npc�������ɶ���
                    {
                        selectedNpcList.Add(npcName.Replace("[����������]", "").Trim());
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
        /// ȫ��ѡ��/ȫ��ȡ��
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
        /// ѡ������������NPC
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                string itemName = checkedListBox1.Items[i] as string;

                if (itemName.EndsWith("[����������]"))
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
        private void bMarkSpecialNpc_Click(object sender, EventArgs e)
        {
            int selectedIndex = checkedListBox1.SelectedIndex;

            if (selectedIndex >= 0 && selectedIndex < checkedListBox1.Items.Count)
            {
                string oldValue = checkedListBox1.Items[selectedIndex].ToString();
                string newString = string.Format("{0} [����]", oldValue.Replace("[����������]", "").Trim()); ;
                checkedListBox1.Items[selectedIndex] = newString;
                checkedListBox1.SetItemChecked(selectedIndex, false);

                string npcID = GetNpcID(newString).ToString();
                string npcName = GetNpcName(newString);

                SetNpcSpecific(npcID, npcName, true);
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
        private void bUnmarkSpecialNpc_Click(object sender, EventArgs e)
        {
            int selectedIndex = checkedListBox1.SelectedIndex;

            if (selectedIndex >= 0 && selectedIndex < checkedListBox1.Items.Count)
            {
                string oldValue = checkedListBox1.Items[selectedIndex].ToString();
                string newString = oldValue.Replace("[����]", "").Trim();
                checkedListBox1.Items[selectedIndex] = newString;
                checkedListBox1.SetItemChecked(selectedIndex, true);

                string npcID = GetNpcID(newString).ToString();
                string npcName = GetNpcName(newString);

                SetNpcSpecific(npcID, npcName, false);
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫȡ����ǵ�NPC", "ȡ���������NPC", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ��ѡ/ȡ���������ж���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void checkBoxX1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxX1.Checked)
            {
                checkBoxX2.Enabled = true;
            }
            else
            {
                checkBoxX2.Enabled = false;
            }
        }      
    }
}