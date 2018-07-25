using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Collections;

namespace CustomLoadTree
{
    /// <summary>
    /// �������Ŵ�����
    /// </summary>
    class TreeData
    {
        private SqlConnection conn; // sql����
        private Hashtable table = new Hashtable(); // ���������ŵĹ�ϣ��

        /// <summary>
        /// ���캯��
        /// </summary>
        public TreeData()
        {
        }

        /// <summary>
        /// ��ʼ����һ��������˳��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="tree">��</param>
        public void InitTreeOrder(SqlConnection conn, TreeView tree)
        {
            int index = 0; // ������
            this.conn = conn; // ��ʼ��sql����
            if (conn.State != System.Data.ConnectionState.Open) // ������
            {
                conn.Open();
            }

            foreach (TreeNode node in tree.Nodes)
            {
                InitTreeOrder(node, "", index);
                index++;
            }
            if (conn.State != System.Data.ConnectionState.Closed) // �ر�����
            {
                conn.Close();
            }
        }

        /// <summary>
        /// ��ʼ���ǵ�һ��������˳��
        /// </summary>
        /// <param name="node">��ǰ���</param>
        /// <param name="fatherIndex">���������</param>
        /// <param name="index">���������</param>
        private void InitTreeOrder(TreeNode node, string fatherIndex, int index)
        {
            string questID = ((object[])node.Tag)[0].ToString().Trim(); // ����IDֵ
            string questIndex = index.ToString(); // �������

            for (int i = 0; i < 3 - index.ToString().Length; i++)
            {
                questIndex = "0" + questIndex;
            }
            questIndex = fatherIndex + questIndex;

            try
            {
                //SqlCommand cmd = conn.CreateCommand();
                //cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE QuestID = {3}", "sys_quests_order", "QuestIndex", questIndex, questID);
                //cmd.ExecuteNonQuery();
                table.Add(node, questIndex); // ����Ϣ�����hashtable
            }
            catch (Exception ex)
            {
                MessageBox.Show("��ʼ����������ʱ�����쳣: " + ex.ToString());
            }

            int index1 = 0; // �ӽ������
            foreach (TreeNode node1 in node.Nodes)
            {
                InitTreeOrder(node1, questIndex, index1);
                index1++;
            }
        }

        /// <summary>
        /// ����ͬ������˳��
        /// </summary>
        /// <param name="node">��ǰ���</param>
        public void ResetOrder(TreeNode node)
        {
            TreeNode fatherNode = node.Parent; // ��ǰ���ĸ��׽��
            string fatherIndex = table[fatherNode].ToString(); // ���׽������
            int index = 0; // �������
            SqlCommand cmd;

            try
            {
                if (conn.State != System.Data.ConnectionState.Open) // ������
                {
                    conn.Open();
                }
                foreach (TreeNode node1 in fatherNode.Nodes)
                {
                    string questIndex = index.ToString(); // �������
                    string questID = ((object[])node1.Tag)[0].ToString().Trim(); // ����IDֵ
                    for (int i = 0; i < 3 - index.ToString().Length; i++)
                    {
                        questIndex = "0" + questIndex;
                    }
                    questIndex = fatherIndex + questIndex;
                    cmd = conn.CreateCommand();
                    cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE QuestID = {3}", "sys_quests_order", "QuestIndex", questIndex, questID);
                    cmd.ExecuteNonQuery();
                    table[node1] = questIndex;
                    ResetOrder(node1, questIndex, index);
                    index++;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("����ͬ�����˳��ʱ�����쳣: " + ex.ToString());
            }
            if (conn.State != System.Data.ConnectionState.Closed) // �ر�����
            {
                conn.Close();
            }
        }

        /// <summary>
        /// �����ӽ���˳��
        /// </summary>
        /// <param name="node">��ʼ���</param>
        /// <param name="fatherIndex">���׽������</param>
        /// <param name="index">���������</param>
        private void ResetOrder(TreeNode node, string fatherIndex, int index)
        {
            string questID = ((object[])node.Tag)[0].ToString().Trim(); // ����IDֵ
            string questIndex = index.ToString(); // �������

            for (int i = 0; i < 3 - index.ToString().Length; i++)
            {
                questIndex = "0" + questIndex;
            }
            questIndex = fatherIndex + questIndex;

            try
            {
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE {0} SET {1} = \'{2}\' WHERE QuestID = {3}", "sys_quests_order", "QuestIndex", questIndex, questID);
                cmd.ExecuteNonQuery();
                table[node] = questIndex;
            }
            catch (Exception ex)
            {
                MessageBox.Show("��ʼ����������ʱ�����쳣: " + ex.ToString());
            }

            int index1 = 0; // �ӽڵ�����
            foreach (TreeNode node1 in node.Nodes)
            {
                ResetOrder(node1, questIndex, index1);
                index1++;
            }
        }

        /// <summary>
        /// ��ȡ�������
        /// </summary>
        /// <param name="node">��Ҫ�����ŵĽ��</param>
        /// <returns></returns>
        public string GetQuestIndex(TreeNode node)
        {
            if (table[node] == null)
            {
                return null;
            }
            return table[node].ToString();
        }

        /// <summary>
        /// ����sql����
        /// </summary>
        /// <param name="conn">sql����</param>
        public void ResetConnection(SqlConnection conn)
        {
            this.conn = conn;
        }

        /// <summary>
        /// �������
        /// </summary>
        /// <param name="id">Ҫ��Ӽ�¼��ID��</param>
        public void AddData(string id)
        {
            try
            {
                if (conn.State != System.Data.ConnectionState.Open) // ������
                {
                    conn.Open();
                }

                if (!CheckIDExist(id)) // idδ����
                {
                    SqlCommand cmd = conn.CreateCommand();
                    cmd.CommandText = string.Format("INSERT INTO {0} ({1}) VALUES ({2})", "sys_quests_order", "QuestID", id);
                    cmd.ExecuteNonQuery();
                }

                if (conn.State != System.Data.ConnectionState.Closed) // �ر�����
                {
                    conn.Close();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("����ӽ���������ʱ�����쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// ��ӷ���
        /// </summary>
        /// <param name="node">�������</param>
        public void AddSection(TreeNode node)
        {
            TreeNode preNode = node.PrevNode; // ǰһ���������
            if(preNode != null)
            {
                string indexString = GetQuestIndex(preNode);
                if(indexString != null)
                {
                    int index = int.Parse(indexString);
                    index++;
                    table[node] = index.ToString();
                }
            }
            else
            {
                table[node] = "0";
            }
        }

        /// <summary>
        /// ���id�Ƿ��Ѿ�����
        /// </summary>
        /// <param name="id">����ID��</param>
        /// <returns></returns>
        private bool CheckIDExist(string id)
        {
            string sql = string.Format("SELECT QuestID FROM sys_quests_order WHERE QuestID = {0}", id);
            SqlCommand cmd;
            SqlDataReader reader = null;
            bool result = true;

            try
            {
                cmd = new SqlCommand(sql, conn);
                reader = cmd.ExecuteReader();
                reader.Read();
                if (!reader.HasRows) // ��ѯ���Ϊ��
                {
                    result = false;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("���ID�Ƿ����ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if (!reader.IsClosed) // reader��δ�ر�
                {
                    reader.Close();
                }
            }
            return result;
        }

        /// <summary>
        /// ����������е�ID�Ƿ�ƥ��
        /// </summary>
        /// <param name="conn">sql����</param>
        public void CheckIDCompare(SqlConnection conn)
        {
            try
            {
                if (conn.State != System.Data.ConnectionState.Open) // ������
                {
                    conn.Open();
                }

                string sql = "insert into sys_quests_order (questid) select questid from tbl_quests where questid not in (select questid from sys_quests_order)";
                SqlCommand cmd = new SqlCommand(sql, conn);
                cmd.ExecuteNonQuery(); 
             
            }
            catch (Exception ex)
            {
                MessageBox.Show("�����������QuestID�Ƿ�ƥ��ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State != System.Data.ConnectionState.Closed) // �ر�����
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// ɾ��ID��ż�¼
        /// </summary>
        /// <param name="id">Ҫɾ����¼��ID��</param>
        public void RemoveRecord(string id)
        {
            try
            {
                if (conn.State != System.Data.ConnectionState.Open) // ������
                {
                    conn.Open();
                }
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = string.Format("DELETE FROM {0} WHERE {1} = {2}", "sys_quests_order", "QuestID", id);
                cmd.ExecuteNonQuery();
            }
            catch (Exception ex)
            {
                MessageBox.Show("��ɾ��ID��ż�¼ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State != System.Data.ConnectionState.Closed) // �ر�����
                {
                    conn.Close();
                }
            }
        }
    }
}
