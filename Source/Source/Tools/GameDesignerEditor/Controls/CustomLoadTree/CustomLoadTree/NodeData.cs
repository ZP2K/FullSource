using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.Windows.Forms;

namespace CustomLoadTree
{
    /// <summary>
    /// �����Ϣ������
    /// </summary>
    class NodeData
    {
        private Hashtable table = new Hashtable(); // �������������Ϣ��hashtable
        private List<List<string>> list = new List<List<string>>(); // ���������Ϣ������

        /// <summary>
        /// �����������
        /// </summary>
        public int NodeCount
        {
            get
            {
                return table.Count;
            }
        }

        /// <summary>
        /// ���������
        /// </summary>
        /// <param name="node">��ӵ��½��</param>
        /// <param name="list">�½�����Ϣ</param>
        public void AddData(TreeNode node, List<string> list)
        {
            try
            {
                table.Add(node, list);
            }
            catch (Exception ex)
            {
                MessageBox.Show("�������Ϣ�����HashTableʱ�����쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// ��ȡid��������ȼ��Ĺ�ϣ��
        /// </summary>
        /// <returns>id��������ȼ��Ĺ�ϣ��</returns>
        public Hashtable GetLevelTable()
        {
            Hashtable levelTable = new Hashtable();

            foreach(object o in table.Keys)
            {
                TreeNode node = o as TreeNode;
                List<string> data = table[o] as List<string>;
                if(data != null)
                {
                    levelTable[data[0]] = data[2];
                }
            }

            return levelTable;
        }

        /// <summary>
        /// ��ȡid�����������Ĺ�ϣ��
        /// </summary>
        /// <returns>id�����������Ĺ�ϣ��</returns>
        public Hashtable GetNameTable()
        {
            Hashtable nameTable = new Hashtable();

            foreach(object o in table.Keys)
            {
                TreeNode node = o as TreeNode;
                List<string> data = table[o] as List<string>;
                if(data != null)
                {
                    nameTable[data[0]] = data[1];
                }
            }

            return nameTable;
        }

        /// <summary>
        /// ��ȡ����
        /// </summary>
        /// <param name="node">Ҫ��ȡ��Ϣ�Ľ��</param>
        /// <returns>�����Ϣ��ϣ��</returns>
        public Hashtable GetData(TreeNode node)
        {
            return table;
        }

        /// <summary>
        /// ��ȡ����ID
        /// </summary>
        /// <param name="node">Ҫ��ȡID�Ľ��</param>
        /// <returns>����ID</returns>
        public string GetID(TreeNode node)
        {
            List<string> data = table[node] as List<string>;
            if (data == null)
            {
                return null;
            }
            else
            {
                return data[0];
            }
        }

        /// <summary>
        /// ��ȡ��������
        /// </summary>
        /// <param name="node">Ҫ��ȡ���ƵĽ��</param>
        /// <returns>��������</returns>
        public string GetName(TreeNode node)
        {
            List<string> data = table[node] as List<string>;
            if (data == null)
            {
                return null;
            }
            else
            {
                return data[1];
            }
        }

        /// <summary>
        /// ��ȡ����Level
        /// </summary>
        /// <param name="node">Ҫ��ȡLevel�Ľ��</param>
        /// <returns>����Level</returns>
        public string GetLevel(TreeNode node)
        {
            List<string> data = table[node] as List<string>;
            if (data == null)
            {
                return null;
            }
            else
            {
                return data[2];
            }
        }

        /// <summary>
        /// �������
        /// </summary>
        public void ClearData()
        {
            table.Clear();
            list.Clear();
        }
    }
}
