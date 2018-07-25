using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace GameDesingerTools
{
    public partial class IconConfig : Form
    {
        int m_nSelectIndex = -1;
        int m_nModid = -1;
        TreeNode m_Node = null;
        ModelEdit m_ParentForm = null;
        public IconConfig(TreeNode node, ModelEdit parentForm)
        {
            InitializeComponent();
            IconListView.Items.Clear();

            this.IconListView.SmallImageList = ((MainForm)Program.MainForm).imageList2;

            // ǰ8��ͼ����ģ���µĽڵ�ͼ�꣬��������
            for (int i = 8; i < ((MainForm)Program.MainForm).imageList2.Images.Count; ++i)
            {
                ListViewItem listViewItem = new ListViewItem("",��i);
                IconListView.Items.Add(listViewItem);
            }
            m_nModid = (int)node.Tag;
            m_Node = node;
            m_ParentForm = parentForm;
        }

        private void IconListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            m_nSelectIndex = e.ItemIndex;
        }

        private void Confirm_Click(object sender, EventArgs e)
        {
            bool bOpenCnnt = false;
            if (m_nSelectIndex == -1)
                goto Exit0;
            m_nSelectIndex += 8;
            
            try
            {
                MainForm.conn.Open();
                bOpenCnnt = true;
                SqlCommand sqlcmd = MainForm.conn.CreateCommand();

                //sqlcmd.CommandText = string.Format("update sys_modl_def set iconid = {0} WHERE id = {1}", m_nSelectIndex, m_nModid);
                //sqlcmd.ExecuteScalar();

                string sql = string.Format("update sys_modl_def set iconid = {0} WHERE id = {1}", m_nSelectIndex, m_nModid);
                Helper.ExecuteCommand("sys_modl_def", sql, MainForm.conn);
                m_Node.ImageIndex = m_nSelectIndex;
                m_Node.SelectedImageIndex = m_nSelectIndex;
                m_ParentForm.LoadModelDefTable();
                Program.MainForm.RefreshIcons(m_nModid.ToString(), Program.MainForm.imageList2.Images[m_nSelectIndex]);
            }
            catch (Exception exp)
            {
                MessageBox.Show(exp.Message);
            }

        Exit0:
            if (bOpenCnnt)
            {
                MainForm.conn.Close();
            }
            Close();
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// �Զ���ͼ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void other_Click(object sender, EventArgs e)
        {
            string fileName; // �ļ���
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Environment.SpecialFolder.MyComputer.ToString();
            openFileDialog.Filter = "ͼƬ�ļ�(*.jpg, *.jpeg, *.bmp, *.emf, *.exif, *.gif, *.ico, *.icon, *.png, *.tiff, *.wmf)|" +
                "*.jpg;*.jpeg;*.bmp;*.emf;*.exif;*.gif;*.ico;*.icon;*.png;*.tiff;*.wmf|�����ļ�|*.*"; // �����Զ������ĸ�ʽ����
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 0;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    fileName = openFileDialog.FileName;
                    DBTools.AddImageForm imageForm = new DBTools.AddImageForm(fileName, m_Node, m_ParentForm);
                    DialogResult result = imageForm.ShowDialog();
                    if(result == DialogResult.OK) // �رմ���
                    {
                        this.Close();
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("���ļ�IOʱ�����쳣: " + ex.ToString());
                }
            }
        }
    }
}