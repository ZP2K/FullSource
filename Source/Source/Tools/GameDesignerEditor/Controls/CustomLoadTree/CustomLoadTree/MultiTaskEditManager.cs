using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Drawing;
using System.Collections;

namespace CustomLoadTree
{
    class MultiTaskEditManager
    {
        private SqlConnection conn; // sql����
        private TreeNode node; // ��ǰ�����
        private DataTable taskTable; // �������ݱ�
        private List<string> displayFieldList; // ��ʾ���ֶ�����
        private Hashtable idTable = new Hashtable(); // �������������id�Ĺ�ϣ��
        private Hashtable iconIndexTable = new Hashtable();


        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="node">��ǰ�����</param>
        /// <param name="displayFieldList">��ʾ���ֶ�����</param>
        public MultiTaskEditManager(SqlConnection conn, TreeNode node, List<string> displayFieldList)
        {
            this.conn = conn;
            this.node = node;
            this.displayFieldList = displayFieldList;
            iconIndexTable.Add(0, "ǰ������");
            iconIndexTable.Add(1, "�ű�����");
            iconIndexTable.Add(2, "�ɼ�����");
            iconIndexTable.Add(3, "�ռ�����");
            iconIndexTable.Add(4, "ɱ������");
            iconIndexTable.Add(5, "�͵�������");
            iconIndexTable.Add(6, "�Ի�����");
            iconIndexTable.Add(7, "��ʱ����");
        }

        /// <summary>
        /// ����ѡ�����
        /// </summary>
        public void Operate()
        {
            // ��ȡ��������������
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0}", "tbl_quests");
                taskTable = Helper.GetDataTable(sqlString, conn);
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ������Ϣʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            // ��ʾ����ѡ����
            List<string> taskStringList = new List<string>(); // �����ַ�������
            List<string> taskIDList = new List<string>(); // ѡ�е�����id����
            string taskID; // �����id

            FillTaskItems(node, taskStringList);
            MultiSelectForm mForm = new MultiSelectForm("��ѡ����Ҫ�༭������");
            mForm.ItemList = taskStringList;
            mForm.ShowItems();
            DialogResult result = mForm.ShowDialog();
            if(result != DialogResult.OK) // �û�û��ȷ��ѡ��ֱ�ӷ���
            {
                return;
            }
            if(mForm.ItemList.Count == 0) // �û�û��ѡ���κ�����ֱ�ӷ���
            {
                MessageBox.Show("��û��ѡ����Ҫ�༭������!");
                return;
            }

            foreach(string s in mForm.ItemList)
            {
                taskID = GetTaskID(s);
                if(taskID != null)
                {
                    taskIDList.Add(taskID);
                }
            }

            // ��ʾ�ֶ�ѡ����
            List<string> fieldList = new List<string>(); // ѡ�е��ֶ�����
            foreach(DataColumn c in taskTable.Columns)
            {
                fieldList.Add(c.ColumnName);
            }
            mForm = new MultiSelectForm("��ѡ����Ҫ�༭���ֶ�");
            mForm.ItemList = fieldList;
            mForm.SelectItemList = displayFieldList;
            mForm.ShowItems();
            result = mForm.ShowDialog();
            if (result != DialogResult.OK) // �û�û��ȷ��ѡ��ֱ�ӷ���
            {
                return;
            }
            if (mForm.ItemList.Count == 0) // �û�û��ѡ���κ�����ֱ�ӷ���
            {
                MessageBox.Show("��û��ѡ����Ҫ�༭���ֶ�!");
                return;
            }

            MultiTaskEditForm eForm = new MultiTaskEditForm(conn, taskTable, taskIDList, fieldList);
            eForm.ShowDialog();
        }

        /// <summary>
        /// ��ȡ�����id
        /// </summary>
        /// <param name="taskString">����Ļ���ַ���</param>
        /// <returns>�����id</returns>
        private string GetTaskID(string taskString)
        {
            string id = idTable[taskString] as string;

            /*int firstIndex = taskString.IndexOf('[');
            int lastIndex = taskString.IndexOf(']');

            if((firstIndex >= 0) && (lastIndex >= 0))
            {
                id = taskString.Substring(firstIndex + 1, lastIndex - firstIndex - 1);
            }*/

            return id;
        }

        /// <summary>
        /// ��������б�����
        /// </summary>
        /// <param name="currentNode">��ǰ�������</param>
        /// <param name="list">ѡ����������</param>
        private void FillTaskItems(TreeNode currentNode, List<string> list)
        {
            object[] keys = currentNode.Tag as object[];
            if (keys != null)
            {
                int id = int.Parse(keys[0].ToString());
                if((id > 0) && (currentNode.ForeColor != Color.Olive)) // ���˵������һ��ȫ����ɺ;�����
                {
                    string strNodeFullName = string.Format("[{0}]\t{1}",
                        iconIndexTable[currentNode.ImageIndex].ToString(), currentNode.Text);
                    list.Add(strNodeFullName);
                    idTable[strNodeFullName] = id.ToString(); // ����id
                }            
            }

            foreach(TreeNode childNode in currentNode.Nodes)
            {
                FillTaskItems(childNode, list);
            }
        }
    }
}
