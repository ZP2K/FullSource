using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace GameDesingerTools.DBTools
{
    public partial class ConfigModelForm : Form
    {
        private SqlConnection conn; // sql����
        private TreeNode node; // �����
        private ModelEdit modelEditor; // ģ��༭������
        private ImageList imageList; // ͼ������
        private string modelID; // ģ���id

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="node">ģ���Ӧ�������</param>
        /// <param name="modelEditor">ģ��༭������</param>
        public ConfigModelForm(SqlConnection conn, TreeNode node, ModelEdit modelEditor)
        {
            this.conn = conn;
            this.node = node;
            this.modelEditor = modelEditor;
            this.imageList = modelEditor.imglstIcons;
            this.modelID = node.Tag.ToString();
            InitializeComponent();
            InitData();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData()
        {            
            if (node.ImageIndex >= 0)
            {
                buttonItem1.Image = imageList.Images[node.ImageIndex];
            }

            buttonItem1.Text = node.Text;
            buttonItem1.Tooltip = "��굥��ѡ��ͼƬ";
            textBoxX1.Text = node.Text;

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }
                /* remoting
                string sql = string.Format("SELECT displaytext, displaystyle FROM {0} WHERE id = {1}",
                    "sys_modl_def", modelID);
                DataTable table = Helper.GetDataTable(sql, conn);
                */

                string filter = string.Format("id = {0}", modelID);
                DataTable table = Helper.GetDataTableProxy("sys_modl_def", filter, null, conn);

                if(table.Rows.Count > 0 && !(table.Rows[0]["displaystyle"] is DBNull)) // �ҵ��Զ�������
                {
                    buttonItem1.Text = table.Rows[0]["displaytext"].ToString();
                    textBoxX1.Text = table.Rows[0]["displaytext"].ToString();
                    comboBoxEx1.SelectedIndex = int.Parse(table.Rows[0]["displaystyle"].ToString());
                }
                else
                {
                    comboBoxEx1.SelectedIndex = 0;
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڳ�ʼ��ģ����ʾ��ʽ��Ϣʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }    
        }

        /// <summary>
        /// ��������
        /// </summary>
        private void SaveData()
        {
            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }
                string displayText = textBoxX1.Text; // ��ʾ���ı�
                int displayStyle = comboBoxEx1.SelectedIndex; // ��ʾ����ʽ
                string sql = string.Format("UPDATE {0} SET {1} = \'{2}\', {3} = {4} WHERE id = {5}",
                    "sys_modl_def", "displaytext", displayText, "displaystyle", displayStyle.ToString(), modelID);

                Helper.ExecuteCommand("sys_modl_def", sql, conn);
                /* remoting
                SqlCommand cmd = new SqlCommand(sql, conn);
                cmd.ExecuteNonQuery();
                 */

            }
            catch(Exception ex)
            {
                MessageBox.Show("�ڱ���ģ���Զ�����ʾ��ʽʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            SaveData();
            Program.MainForm.RefreshButtonStyle(modelID, textBoxX1.Text, comboBoxEx1.SelectedIndex); // ˢ��MainForm�������ʾ��ʽ
            this.Close();
        }

        /// <summary>
        /// �û�ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// �û�ѡ���µ���ʾ��ʽ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx1_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch(comboBoxEx1.SelectedIndex)
            {
                case 0: // Ĭ����ʽ
                    {
                        buttonItem1.ButtonStyle = DevComponents.DotNetBar.eButtonStyle.Default;
                        buttonItem1.Tooltip = node.Text;
                        break;
                    }
                case 1: // ��һ�ı�
                    {
                        buttonItem1.ButtonStyle = DevComponents.DotNetBar.eButtonStyle.TextOnlyAlways;
                        buttonItem1.Tooltip = node.Text;
                        break;
                    }
                case 2: // ͼ�Ļ��
                    {
                        buttonItem1.ButtonStyle = DevComponents.DotNetBar.eButtonStyle.ImageAndText;
                        buttonItem1.Tooltip = node.Text;
                        break;
                    }
            }
        }

        /// <summary>
        /// ��ʾ�ı������仯
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void textBoxX1_TextChanged(object sender, EventArgs e)
        {
            buttonItem1.Text = textBoxX1.Text;
        }

        /// <summary>
        /// ����ģ��ͼ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonItem1_Click(object sender, EventArgs e)
        {
            IconConfig iconForm = new IconConfig(node, modelEditor);
            iconForm.ShowDialog();
            buttonItem1.Image = imageList.Images[node.ImageIndex];
        }
    }
}