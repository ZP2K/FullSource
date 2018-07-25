using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Data.SqlClient;

namespace UploadImageToDatabase
{
    public partial class MainForm : Form
    {
        private string clientFolder; // �ͻ����ļ���
        private SqlConnection conn; // sql����

        public MainForm()
        {
            InitializeComponent();
        }       

        /// <summary>
        /// ��ʼ��sql����
        /// </summary>
        private void InitConnection()
        {
            string connectString = "Server = 192.168.27.31; DataBase = s3design_rtm; Uid = s3design; Password = davidbowie;";
            conn = new SqlConnection(connectString);
        }

        /// <summary>
        /// ����ͼƬ
        /// </summary>
        /// <param name="tableName">���ݱ���</param>
        /// <param name="keyField">�����ֶ�</param>
        private void UpdateImage(string tableName, string keyField)
        {
            if (conn == null)
            {
                InitConnection();
            }
            
            DirectoryInfo di = new DirectoryInfo(clientFolder);

            if (di.Exists)
            {
                FileInfo[] files = di.GetFiles();
                FileStream fileStream;
                int updateCount = 0;

                try
                {
                    if (conn.State == ConnectionState.Closed)
                    {
                        conn.Open();
                    }

                    string sqlString = null;
                    object executeResult = null;
                    SqlCommand cmd = conn.CreateCommand();

                    int totalCount = files.Length;
                    int currentCount = 0;

                    foreach (FileInfo fi in files)
                    {
                        if (fi.Extension == ".jpg")
                        {
                            fileStream = fi.OpenRead();
                            byte[] buffer = new byte[fileStream.Length];
                            fileStream.Read(buffer, 0, buffer.Length);
                            fileStream.Close();

                            string keyValue = fi.Name.Replace(".jpg", "");
                            sqlString = string.Format("SELECT EditTime FROM {0} WHERE [{1}] = '{2}'",
                                                      tableName, keyField, keyValue);
                            cmd.CommandText = sqlString;
                            executeResult = cmd.ExecuteScalar();

                            if (executeResult != null) // ͼƬ��¼�Ѿ�����
                            {
                                DateTime editTime = (DateTime)executeResult;                                

                                if (fi.LastWriteTime > editTime) // ��Ҫ����ͼƬ
                                {
                                    DateTime newTime = fi.LastWriteTime.AddSeconds(1); // ��������һ�룬��Ȼif�����������㣬�ܹ���

                                    sqlString = string.Format("UPDATE {0} SET EditTime = '{1}', Image = @image WHERE [{2}] = '{3}'",
                                                              tableName, newTime, keyField, keyValue);
                                    cmd.CommandText = sqlString;
                                    cmd.Parameters.Add("@image", SqlDbType.Image).Value = buffer;
                                    cmd.ExecuteNonQuery();
                                    cmd.Parameters.Clear();

                                    updateCount++;
                                }
                            }
                            else // ֱ�Ӳ��뼴��
                            {
                                sqlString = string.Format("INSERT INTO {0} ({1}, EditTime, Image) VALUES ('{2}', '{3}', @image)",
                                                          tableName, keyField, keyValue, fi.LastWriteTime);
                                cmd.CommandText = sqlString;
                                cmd.Parameters.Add("@image", SqlDbType.Image).Value = buffer;
                                cmd.ExecuteNonQuery();
                                cmd.Parameters.Clear();

                                updateCount++;
                            }
                        }

                        currentCount++;
                        this.Text = string.Format("Ԥ��ͼƬ�������� {0}/{1}", currentCount.ToString(), totalCount.ToString());
                    }

                    this.Text = "Ԥ��ͼƬ�������� ����ɣ�";
                }
                catch (Exception ex)
                {
                    MessageBox.Show("�ڸ���Ԥ��ͼƬʱ�����쳣��" + ex.Message, "����Ԥ��ͼƬ", MessageBoxButtons.OK,
                                    MessageBoxIcon.Information);
                }
                finally
                {
                    if (conn.State == ConnectionState.Open)
                    {
                        conn.Close();
                    }
                }

                MessageBox.Show(string.Format("������{0}����¼", updateCount.ToString()), "����Ԥ��ͼƬ", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show(string.Format("Ŀ¼{0}�����ڣ�", clientFolder), "������Ԥ��ͼƬ", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ѡ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            UpdateImage("WeaponImage", "ItemNumber");
        }

        /// <summary>
        /// �رմ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// ����ļ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBroserDialog = new FolderBrowserDialog();
            if (clientFolder != null)
            {
                folderBroserDialog.SelectedPath = clientFolder;
            }
            folderBroserDialog.Description = "��ѡ��ͼƬ�ļ������ڵ�Ŀ¼";

            if (folderBroserDialog.ShowDialog() == DialogResult.OK)
            {
                clientFolder = folderBroserDialog.SelectedPath;
                textBoxX1.Text = clientFolder;
            }
        }

        /// <summary>
        /// ����ģ��ͼƬ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            UpdateImage("ModelImage", "EditorID");
        }
    }
}