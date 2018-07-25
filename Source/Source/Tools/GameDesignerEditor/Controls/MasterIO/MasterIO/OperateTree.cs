using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Collections;
using System.Data;
using System.Windows.Forms;

namespace MasterIO
{
    class OperateTree
    {
        private SqlConnection conn; // sql����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        public OperateTree(SqlConnection conn)
        {
            this.conn = conn;
        }

        /// <summary>
        /// ����ճ��
        /// </summary>
        public void Paste(TreeNode sourceNode, TreeNode destinateNode)
        {
            if (sourceNode.Level == destinateNode.Level) // ����Ƿ���ͬ�����
            {
                switch (destinateNode.Level)
                {
                    case 0: // ճ���ļ���
                        {
                            PasteFolder(sourceNode, destinateNode);
                            break;
                        }
                    case 1: // ճ���ļ�
                        {
                            PasteFile(sourceNode, destinateNode);
                            break;
                        }
                    case 2: // ճ������
                        {
                            PasteItem(sourceNode, destinateNode.Parent);
                            break;
                        }
                }
            }
            else
            {
                MessageBox.Show("��ͬ����㲻������ճ��!");
            }
        }

        /// <summary>
        /// ճ��ѧϰʦ�ļ���
        /// </summary>
        /// <param name="sourceNode">Դ�����</param>
        /// <param name="destinateNode">Ŀ�������</param>
        private void PasteFolder(TreeNode sourceNode, TreeNode destinateNode)
        {
            TreeNode newNode;

            if (sourceNode.Nodes.Count > 0)
            {
                foreach (TreeNode fileNode in sourceNode.Nodes)
                {
                    if (fileNode.Nodes.Count > 0)
                    {
                        newNode = destinateNode.Nodes.Add(fileNode.Text);
                        newNode.Tag = fileNode.Tag;
                        PasteFile(fileNode, newNode);
                    }
                    else
                    {
                        MessageBox.Show(string.Format("�������ճ������: {0}�ļ�����û���κ�����!", fileNode.Text));
                    }
                }
            }
            else
            {
                MessageBox.Show(string.Format("�������ճ������: {0}�ļ�������û���κ�����!", sourceNode.Text));
            }
        }

        /// <summary>
        /// ճ��ѧϰʦ�ļ�
        /// </summary>
        /// <param name="sourceNode">Դ�����</param>
        /// <param name="destinationNode">Ŀ�������</param>
        private void PasteFile(TreeNode sourceNode, TreeNode destinateNode)
        {
            if (sourceNode.Nodes.Count > 0)
            {
                foreach (TreeNode childNode in sourceNode.Nodes)
                {
                    PasteItem(childNode, destinateNode);
                }
            }
            else
            {
                MessageBox.Show(string.Format("�������ճ������: {0}�ļ�����û���κ�����!", sourceNode.Text));
            }
        }

        /// <summary>
        /// ճ��ѧϰʦ����
        /// </summary>
        /// <param name="sourceNode">Դ�����</param>
        /// <param name="destinateNode">Ŀ�������</param>
        private void PasteItem(TreeNode sourceNode, TreeNode destinateNode)
        {
            TreeNode newNode;
            string folderName = destinateNode.Parent.Text;
            string fileName = destinateNode.Text;

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0}", "tbl_master_skills");
                SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
                DataSet ds = new DataSet();
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                adp.Fill(ds);
                DataTable shopTable = ds.Tables[0];

                string id = (sourceNode.Tag as object[])[0].ToString();
                DataRow row = shopTable.Rows.Find(id);

                if (row != null)
                {
                    DataRow newRow = shopTable.NewRow();
                    newRow.BeginEdit();
                    foreach (DataColumn c in shopTable.Columns)
                    {
                        switch (c.ColumnName)
                        {
                            case "id":
                                {
                                    break;
                                }
                            case "filename":
                                {
                                    newRow[c] = fileName;
                                    break;
                                }
                            case "foldername":
                                {
                                    newRow[c] = folderName;
                                    break;
                                }
                            case "MasterID": // ����Ϊ0������ʱ�Զ�ƥ�����
                                {
                                    newRow[c] = 0;
                                    break;
                                }
                            default:
                                {
                                    newRow[c] = row[c];
                                    break;
                                }
                        }
                    }
                    newRow.EndEdit();

                    shopTable.Rows.Add(newRow);
                    SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
                    adp.InsertCommand = cmdBuilder.GetInsertCommand();
                    adp.Update(shopTable);
                    shopTable.AcceptChanges();

                    string newID = newRow["id"].ToString();
                    newNode = destinateNode.Nodes.Add(sourceNode.Text);
                    newNode.Tag = new object[] { newID };
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("��ճ��ѧϰʦ����ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }
    }
}
