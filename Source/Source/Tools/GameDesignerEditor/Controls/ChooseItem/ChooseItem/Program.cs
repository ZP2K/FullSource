using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;

namespace ChooseItem
{
    public class Program
    {
        private Form1 form1; // ѡ�񴰿�
        private List<string> tableNames = new List<string>(); // ��������
        private List<string> detailList = new List<string>(); // Detail��ֵ����
        private List<string> particularList = new List<string>(); // Particular��ֵ������

        /// <summary>
        /// genre����
        /// </summary>
        public string Genre
        {
            get
            {
                return form1.Genre;
            }
        }

        /// <summary>
        /// detail����
        /// </summary>
        public string Detail
        {
            get
            {
                return form1.Detail;
            }
        }

        /// <summary>
        /// particular����
        /// </summary>
        public string Particular
        {
            get
            {
                return form1.Particular;
            }
        }

        /// <summary>
        /// level����
        /// </summary>
        public string Level
        {
            get
            {
                return form1.Level;
            }
        }

        /// <summary>
        /// itemName����
        /// </summary>
        public string ItemName
        {
            get
            {
                return form1.ItemName;
            }
        }
        
        /// <summary>
        /// ���ӱ�
        /// </summary>
        /// <param name="tableName"></param>
        public void AddTable(string tableName)
        {
            tableNames.Add(tableName);
        }

        /// <summary>
        /// ��Detail���������ֵ
        /// </summary>
        /// <param name="value"></param>
        public void AddDetailList(string value)
        {
            detailList.Add(value);
        }

        /// <summary>
        /// ��Particual���������ֵ
        /// </summary>
        /// <param name="value"></param>
        public void AddParticularList(string value)
        {
            particularList.Add(value);
        }

        /// <summary>
        /// ��ʾ�������ɾ������
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="id">id��</param>
        /// <param name="genre">genre�ֶ�</param>
        /// <param name="detail">detail�ֶ�</param>
        /// <param name="particular">particular�ֶ�</param>
        /// <param name="level">level�ֶ�</param>
        /// <returns>�û��Ƿ�ȷ��ѡ��</returns>
        public int Show(SqlConnection conn, string id, object genre, object detail, object particular, object level)
        {
            if((genre is DBNull) || (detail is DBNull) || (particular is DBNull) || level is DBNull)
            {
                genre = "0";
                detail = "0";
                particular = "0";
                level = "0";
            }

            if(genre.ToString() == "-1" || detail.ToString() == "-1" || particular.ToString() == "-1" 
                || level.ToString() == "-1") // �Ƿ���Ĭ��������
            {
                return 0;
            }

            Hashtable particularTable = new Hashtable();
            Hashtable detailTable = new Hashtable();

            if(particularList.Count != 0) // particular����Ϊ��
            {
                int index = 1;
                foreach(string s in particularList)
                {
                    particularTable[index.ToString()] = s;
                    index++;
                }
            }

            if(detailList.Count != 0) // detail����Ϊ��
            {
                int index = 1;
                foreach(string s in detailList)
                {
                    detailTable[index.ToString()] = s;
                    index++;
                }
            }

            form1 = new Form1(conn, tableNames, detailTable, particularTable);
            form1.ID = id;
            form1.Genre = genre.ToString();
            form1.Detail = detail.ToString();
            form1.Particular = particular.ToString();
            form1.Level = level.ToString();
            form1.InitSelection(); // ��ʼ��ѡ��
            DialogResult result = form1.ShowDialog();
            if(result == DialogResult.OK)
            {
                return 1; // �û�ȷ��ѡ��
            }
            else
            {
                return 0; // �û�δȷ��ѡ��
            }
        }
    }
}