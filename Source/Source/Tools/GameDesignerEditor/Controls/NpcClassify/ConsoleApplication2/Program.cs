using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Data;

namespace NpcClassify
{
    class Program
    {
        Form1 frm = null; // ��ʾ�Ĵ���
        private string lastnpcid = "0"; // ��һ��ѡ���npc��id
        private string lastdoodadid = "0"; // ��һ��ѡ���С�����id
        private string lastitemid = "0"; // ��һ��ѡ�����Ʒ��id

        /// <summary>
        /// ���캯��
        /// </summary>
        public Program()
        {
        }

        /// <summary>
        /// ��ʾ������沢�����û�ѡ��ֵ
        /// </summary>
        /// <param name="Conn">sql����</param>
        /// <param name="oldValue">��ֵ</param>
        /// <param name="mode">ģʽ���</param>
        /// <returns>ѡ��id</returns>
        public string Show(SqlConnection Conn, string oldValue, int mode)
        {
            try
            {
                if (Conn.State == ConnectionState.Closed) // ��sql����
                {
                    Conn.Open();
                }

                if(oldValue == string.Empty) // ��ֵת��
                {
                    oldValue = "0";
                }

                switch (mode) // ����modeֵѡ��ͬ�Ĵ���ģʽ
                {
                    case 1: // ����Npc����
                        {
                            if (oldValue == "0")
                            {
                                frm = new Form1(Conn, "NpcTemplate", "MapName", "ForceID", 1, lastnpcid);
                            }
                            else
                            {
                                frm = new Form1(Conn, "NpcTemplate", "MapName", "ForceID", 1, oldValue);
                            }
                            frm.Text = "NPC�������";
                            break;
                        }
                    case 2: // ����С�������
                        {
                            if (oldValue == "0")
                            {
                                frm = new Form1(Conn, "tbl_doodad", "MapName", "Kind", 2, lastdoodadid);
                            }
                            else
                            {
                                frm = new Form1(Conn, "tbl_doodad", "MapName", "Kind", 2, oldValue);
                            }
                            frm.Text = "Doodad�������";
                            break;
                        }
                }
                frm.StartPosition = FormStartPosition.CenterScreen; // ����������Ϊ��Ļ����λ��
                DialogResult result = frm.ShowDialog();
                if (result == DialogResult.OK) // ���ȷ��
                {
                    switch (mode)
                    {
                        case 1:
                            {
                                lastnpcid = frm.Result;
                                break;
                            }
                        case 2:
                            {
                                lastdoodadid = frm.Result;
                                break;
                            }
                    }
                    return frm.Result; // ���ز�ѯ����ֵ
                }
                else // ���ȡ��
                {
                    return oldValue; // ����ԭֵ
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Exception occured: " + ex.ToString());
                return oldValue.ToString();
            }
            finally
            {
                if (Conn.State == ConnectionState.Open) // �ر�sql����
                {
                    Conn.Close();
                }
            }
        }

        /// <summary>
        /// ��ʾ������沢�����û�ѡ��ֵ
        /// </summary>
        /// <param name="Conn">sql����</param>
        /// <param name="mapIndex">��ͼ���</param>
        /// <param name="oldValue">��ֵ</param>
        /// <param name="mode">ģʽ���</param>
        /// <returns>ѡ��id</returns>
        public string Show(SqlConnection Conn, string mapIndex, string oldValue, int mode)
        {
            try
            {
                if (Conn.State == ConnectionState.Closed) // ��sql����
                {
                    Conn.Open();
                }

                if (oldValue == string.Empty) // ��ֵת��
                {
                    oldValue = "0";
                }

                if (mapIndex == string.Empty) // ��ֵת��
                {
                    mapIndex = "0";
                }

                if (oldValue == "0")
                {
                    frm = new Form1(Conn, "Genre", "_CATEGORY", mode, mapIndex, lastitemid); // ��ʾ�Ĵ���
                }
                else
                {
                    frm = new Form1(Conn, "Genre", "_CATEGORY", mode, mapIndex, oldValue); // ��ʾ�Ĵ��� 
                }
                frm.Text = "Item�������";
                frm.StartPosition = FormStartPosition.CenterScreen; // ����������Ϊ��Ļ����λ��
                DialogResult result = frm.ShowDialog();
                if (result == DialogResult.OK) // ���ȷ��
                {
                    lastitemid = frm.Result;
                    return frm.Result; // ���ز�ѯ����ֵ
                }
                else // ���ȡ��
                {
                    return oldValue; // ����ԭֵ
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Exception occured: " + ex.ToString()); // ��ʾ�쳣��Ϣ
                return oldValue;
            }
            finally
            {
                if (Conn.State == ConnectionState.Open) // �ر�sql����
                {
                    Conn.Close();
                }
            }
        }

        /// <summary>
        /// ��ʾ�Ի�����
        /// </summary>
        /// <param name="message">�Ի�����</param>
        /// <param name="title">����</param>
        public void Show(string message, string title)
        {
            MessageBox.Show(message, title);
        }

        /// <summary>
        /// ���ص�ͼ����ֵ
        /// </summary>
        /// <returns>��ͼ����</returns>
        public string GetMapType()
        {
            return frm.MapIndex;
        }

        /// <summary>
        /// ���ص�ͼ����
        /// </summary>
        /// <returns>��ͼ����</returns>
        public string GetMapName()
        {
            return frm.MapName;
        }

        /// <summary>
        /// ��ȡС�������
        /// </summary>
        /// <returns>С����������</returns>
        public string GetDoodadKind()
        {
            return frm.DoodadKind;
        }
    }
}
