using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Data.SqlClient;

namespace GameDesingerTools
{
    public partial class ExportEditor : Form
    {
        private SqlConnection m_conn;
        private DataTable m_ExportModlCfg;
        private DataTable m_ExportTableCfg;
        private int m_nModelID = 0;
        private Dictionary<string, int> tableIDDictionary = new Dictionary<string, int>();

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="nModelID">��ǰģ��ID</param>
        /// <param name="conn">sql����</param>
        public ExportEditor(int nModelID, SqlConnection conn)
        {
            this.m_conn = conn;
            this.m_nModelID = nModelID;

            InitializeComponent();
            Init();                        
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ʼ�����ݱ�
            
            /* remoting
            m_ExportModlCfg = Helper.GetDataTable("SELECT modid, tableid FROM sys_export_modl_cfg", m_conn);
            m_ExportTableCfg = Helper.GetDataTable("SELECT id, tablename FROM sys_export_table_cfg ORDER BY tablename", m_conn);
            */


            
            m_ExportModlCfg = Helper.GetDataTableProxy("sys_export_modl_cfg", m_conn);
            m_ExportTableCfg = Helper.GetDataTableProxy("sys_export_table_cfg", null, "tablename", m_conn);
            


            foreach (DataRow dataRow in m_ExportTableCfg.Rows)
            {
                string tableName = dataRow["tablename"].ToString();
                string tableID = dataRow["id"].ToString();

                tableIDDictionary.Add(tableName, int.Parse(tableID));
            }

            // ��ʼ�����ݱ�������
            tableNameBox.Items.Clear();

            foreach (string tableName in tableIDDictionary.Keys)
            {                
                tableNameBox.Items.Add(tableName);                
            }

            // ��ʼ���������б�
            exportTableBox.Items.Clear();

            DataRow[] rows = m_ExportModlCfg.Select(string.Format("modid = {0}", m_nModelID));

            foreach (DataRow dataRow in rows)
            {
                string tableID = dataRow["tableid"].ToString();                
                DataRow[] tableRows = m_ExportTableCfg.Select(string.Format("id = {0}", tableID));

                foreach (DataRow row in tableRows)
                {
                    string tableName = row["tablename"].ToString();

                    exportTableBox.Items.Add(tableName);
                }
            }
        }

        /// <summary>
        /// ���浼��������Ϣ 
        /// </summary>
        private void SaveExportInfo()
        {
            try
            {
                if (m_conn.State == ConnectionState.Closed)
                {
                    m_conn.Open();
                }

                

                // ��ɾ����������Ϣ
                string sqlString = string.Format("DELETE FROM sys_export_modl_cfg WHERE modid = {0}", m_nModelID); 
               
                /* remoting
                SqlCommand cmd = m_conn.CreateCommand();
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
                */

                Helper.ExecuteCommand("sys_export_modl_cfg", sqlString, m_conn);





                // ������������Ϣ
                foreach (string tableName in exportTableBox.Items)
                {
                    int tableID = tableIDDictionary[tableName];
                    sqlString = string.Format("INSERT INTO sys_export_modl_cfg (modid, tableid) VALUES ({0}, {1})",
                                              m_nModelID, tableID);
                    Helper.ExecuteCommand("sys_export_modl_cfg", sqlString, m_conn);
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڱ���������Ϣʱ����sql�쳣��" + ex.Message, "����������Ϣ",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                if (m_conn.State == ConnectionState.Open)
                {
                    m_conn.Close();
                }
            }
        }

        /// <summary>
        /// ������ݱ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddTable_Click(object sender, EventArgs e)
        {
            string tableName = tableNameBox.Text;

            if (!string.IsNullOrEmpty(tableName))
            {
                bool tableNameExist = false;

                foreach (string exportTableName in exportTableBox.Items)
                {
                    if (tableName == exportTableName)
                    {
                        tableNameExist = true;
                        break;
                    }
                }

                if (!tableNameExist)
                {
                    exportTableBox.Items.Add(tableName);
                }
                else
                {
                    MessageBox.Show("�����ݱ��Ѿ��ڵ��������д��ڣ�", "������ݱ�",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ��ӵ����������е����ݱ�", "������ݱ�",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ɾ�����ݱ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRemoveTable_Click(object sender, EventArgs e)
        {
            string tableName = exportTableBox.SelectedItem as string;

            if (tableName != null)
            {
                exportTableBox.Items.Remove(tableName);
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ�ӵ���������ɾ�������ݱ�", "ɾ�����ݱ�",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bOK_Click(object sender, EventArgs e)
        {
            SaveExportInfo();

            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}