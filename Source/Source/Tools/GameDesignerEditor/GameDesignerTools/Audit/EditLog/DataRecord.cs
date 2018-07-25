using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Data;

namespace GameDesingerTools
{
    class DataRecord
    {
        private SqlConnection conn; // sql����
        private static DataRecord dataRecord; // Ψһ��һ��ʵ��
        private DateTime time; // �޸�ʱ��
        private string userName; // �û���
        private string[] primaryKeys; // ��������
        private string catfields; // ����
        private string oldValue; // ��ֵ
        private string newValue; // ��ֵ
        private TreeNode currentNode; // ��ǰ�����
        private string nodeName; // ��ǰ����������
        private int modelID; // ģ��id
        private int tabID; // tabҳid
        private int fieldID; // �ֶ�id
        private string fieldDisplayName; // �ֶε���ʾֵ
        private string remarks;//��ע

        private string action = "�޸�ֵ";

        /// <summary>
        /// ˽�й��캯�������������µ�ʵ��
        /// </summary>
        private DataRecord()
        {

        }

        /// <summary>
        /// sql��������
        /// </summary>
        public SqlConnection Conn
        {
            set
            {
                conn = value;
            }
        }

        /// <summary>
        /// �ֶε�id����
        /// </summary>
        public int FieldID
        {
            set
            {
                fieldID = value;
            }
        }

        /// <summary>
        /// �ֶε���ʾֵ����
        /// </summary>
        public string FieldDisplayName
        {
            set
            {
                fieldDisplayName = value;
            }
        }

        /// <summary>
        /// ��ǰ���������
        /// </summary>
        public TreeNode CurrentNode
        {
            set
            {
                currentNode = value;
            }     
        }

        /// <summary>
        /// �޸�ʱ������
        /// </summary>
        public DateTime Time 
        {
            set
            {
                time = value;
            }
        }

        /// <summary>
        /// �û�������
        /// </summary>
        public string UserName
        {
            set
            {
                userName = value;
            }
        }

        public string Action
        {
            set 
            { 
                action = value; 
            }
        }
        /// <summary>
        /// ������������
        /// </summary>
        public string[] PrimaryKeys
        {
            set
            {
                primaryKeys = value;
            }
        }

        /// <summary>
        /// ��ֵ����
        /// </summary>
        public string OldValue
        {
            set
            {
                oldValue = value;
            }
        }

        /// <summary>
        /// ��ֵ����
        /// </summary>
        public string NewValue
        {
            set
            {
                newValue = value;
            }
        }

        /// <summary>
        /// ģ��ID����
        /// </summary>
        public int ModelID
        {
            set
            {
                modelID = value;
            }
        }

        /// <summary>
        /// tabҳID����
        /// </summary>
        public int TabID
        {
            set
            {
                tabID = value;
            }
        }
        public string Remarks
        {
            set
            {
                remarks = value;
            }
        }

        /// <summary>
        /// ��ȡʵ��
        /// </summary>
        /// <returns>ʵ��</returns>
        public static DataRecord GetDataRecord()
        {
            if(dataRecord == null) // ʵ��δ��ʼ��
            {
                dataRecord = new DataRecord();
            }
            return dataRecord;
        }
        
        public static string TranslateValue(string value)
        {
            // ����ֵ����ֵͳһת��Ϊtrue��false
            string newValue = value.ToString(); // ��ֵ
            switch (value) // �滻��ֵ����ʾ
            {
                case "True":
                    {
                        newValue = "��";
                        break;
                    }
                case "False":
                    {
                        newValue = "��";
                        break;
                    }
                case "1":
                    {
                        newValue = "��";
                        break;
                    }
                case "0":
                    {
                        newValue = "��";
                        break;
                    }
                default:
                    {
                        newValue = "��";
                        break;
                    }
            }

            return newValue;
        }


        /// <summary>
        /// �����ֶ��޸ĵ���ʷ��¼
        /// </summary>
        public void Save()
        {
            try
            {
                if(conn.State == System.Data.ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }
                nodeName = currentNode.Text;
                catfields = GetCatifields();

                string sql = string.Format("INSERT INTO {0} (hostname, edittime, keys, oldvalue, newvalue, modelid, tabid, nodename, " +
                    "catfields, fielddisplayname, fieldid, remarks, [action]) VALUES (\'{1}\', \'{2}\', \'{3}\', \'{4}\', \'{5}\', \'{6}\', \'{7}\', \'{8}\', \'{9}\', \'{10}\', \'{11}\', \'{12}\', \'{13}\')",  
                    "sys_dataRecord", userName, time, GetPrimaryKeys(), oldValue, newValue, modelID, tabID, nodeName, catfields, fieldDisplayName, fieldID, remarks, action);
                //SqlCommand cmd = new SqlCommand(sql, conn);
                //cmd.ExecuteNonQuery();
                Helper.ExecuteCommand("sys_dataRecord", sql, conn);
            }
            catch(Exception ex)
            {
                MessageBox.Show("�ڸ����ֶ��޸���ʷ��¼ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == System.Data.ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// ��ȡ���������и��ֶ�����������ʾ����
        /// </summary>
        /// <returns>���������и��ֶ�����������ʾ����</returns>
        private string GetFullDisplayName()
        {
            string fullDisplayName = fieldDisplayName.Trim(); // ������ʾ����

            try
            {
                /* remoting
                string sql = string.Format("SELECT parentfield FROM {0} WHERE fieldid = {1}", "sys_meta_info", fieldID.ToString());
                DataTable table = Helper.GetDataTable(sql, conn); 
                 */

                string filter = string.Format("fieldid = {0}", fieldID.ToString());
                DataTable table = Helper.GetDataTableProxy("sys_meta_info", filter, null, conn);

                string id = table.Rows[0]["parentfield"].ToString();
                while((id != "0") || (id != ""))
                {
                    /* remoting
                    sql = string.Format("SELECT parentfield, fieldcnname FROM {0} WHERE fieldid = {1}", "sys_meta_info", id);
                    table = Helper.GetDataTable(sql, conn);
                     */

                    filter = string.Format("fieldid = {0}", id);
                    table = Helper.GetDataTableProxy("sys_meta_info", filter, null, conn);

                    if(table.Rows.Count > 0) // �ҵ������
                    {
                        fullDisplayName = string.Format("{0}.", table.Rows[0]["fieldcnname"].ToString().Trim()) + fullDisplayName;
                        id = table.Rows[0]["parentfield"].ToString();
                    }
                    else
                    {
                        break;
                    }
                }              
            }
            catch(Exception ex)
            {
                MessageBox.Show("�ڻ�ȡ��ǰ�ֶε�������ʾ����ʱ�����쳣: " + ex.ToString());
                fullDisplayName = fieldDisplayName; // �ָ�����
            }

            return fullDisplayName;
        }

        /// <summary>
        /// ��ȡ�����ķ����ַ���
        /// </summary>
        /// <returns>�����ķ����ַ���</returns>
        private string GetPrimaryKeys()
        {
            string primaryKeysString = ""; // �����ַ���

            object[] keys = currentNode.Tag as object[];
            if(keys != null) // �������ϲ�Ϊ��
            {
                foreach(object o in keys)
                {
                    primaryKeysString += string.Format("{0},", o);
                }
            }

            return primaryKeysString.TrimEnd(',');
        }

        /// <summary>
        /// ��ȡ�����ķ�����Ϣ
        /// </summary>
        /// <returns>�����ķ�����Ϣ�ַ���</returns>
        private string GetCatifields()
        {
            string catifieldString = ""; // �����ַ���
            TreeNode fatherNode = currentNode.Parent; // �����

            while(fatherNode != null)
            {
                if(fatherNode.Tag != null)
                {
                    if(fatherNode.Tag is string) // ��¼������Ϣ
                    {
                        catifieldString += string.Format("{0},", fatherNode.Text);
                    }
                    else if((fatherNode.Tag as object[]) != null) // ��¼������Ϣ
                    {
                        try
                        {
                            string tagValue = (fatherNode.Tag as object[])[0].ToString();
                            if(int.Parse(tagValue) < 0) // �Ƿ�����
                            {
                                catifieldString += string.Format("{0},", fatherNode.Text);
                            }
                        }
                        catch(Exception ex)
                        {
                            MessageBox.Show("�ڻ�ȡ�����ķ�����Ϣʱ�����쳣: " + ex.ToString());
                        }
                    }                    
                }
                fatherNode = fatherNode.Parent;
            }

            return catifieldString.TrimEnd(',');
        }
    }
}
