using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.IO;

using DevComponents.DotNetBar;
using DevComponents.AdvTree;

using FieldInfoLib;

namespace GuideTool
{
    public partial class MainForm : Office2007Form
    {
        private SqlConnection conn;
        private const string guideTableName = "sys_guide_class_config";
        private Dictionary<string, string> descriptionDictionary = new Dictionary<string, string>();

        public MainForm()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// sql����
        /// </summary>
        public SqlConnection Connection
        {
            set
            {
                conn = value;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ʼ��sql����
            string connectString = "Server = jx3web; DataBase = s3design_rtm; Uid = s3design; Password = davidbowie;";
            conn = new SqlConnection(connectString);

            string sqlString = string.Format("SELECT * FROM {0}", guideTableName);
            DataTable guideTable = Helper.GetDataTable(sqlString, conn);

            foreach (DataRow dataRow in guideTable.Rows)
            {
                string id = dataRow["ID"].ToString();
                string guideName = dataRow["Name"].ToString();
                string guideDescription = dataRow["Description"].ToString();

                Node node = new Node();
                node.Text = guideName;
                node.Tag = id;

                guideTree.Nodes.Add(node);

                descriptionDictionary[id] = guideDescription;
            }
        }

        /// <summary>
        /// ��ʼ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bStart_Click(object sender, EventArgs e)
        {
            Node currentNode = guideTree.SelectedNode;

            if (currentNode != null)
            {
                string guideID = currentNode.Tag as string;
                string guideName = currentNode.Text;
                EditForm editForm = new EditForm(conn, guideID, guideName);
                editForm.ShowDialog();
            }
            else
            {
                MessageBox.Show("����ѡ���򵼣�", "��ʼ��",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �˳�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bExit_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// ѡ�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void guideTree_AfterNodeSelect(object sender, AdvTreeNodeEventArgs e)
        {
            Node currentNode = guideTree.SelectedNode;

            if (currentNode != null)
            {
                string id = currentNode.Tag as string;
                string description = descriptionDictionary[id];
                descriptionBox.Text = description;
            }
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bLoadGuideInfo_Click(object sender, EventArgs e)
        {
            Node currentNode = guideTree.SelectedNode;

            if (currentNode != null)
            {
                string guideName = currentNode.Text;
                string fileName = Path.Combine(Application.StartupPath, string.Format("{0}.GuideInfo", guideName));

                OpenFileDialog openFileDialog = new OpenFileDialog();
                openFileDialog.Title = "����������";
                openFileDialog.Filter = "������|*.GuideInfo";
                openFileDialog.InitialDirectory = Application.StartupPath;

                if (File.Exists(fileName))
                {
                    openFileDialog.FileName = fileName;

                    if (openFileDialog.ShowDialog() == DialogResult.OK)
                    {
                        fileName = openFileDialog.FileName;
                        string id = currentNode.Tag as string;
                        byte[] buffer = Helper.ReadFile(fileName);

                        if (buffer != null)
                        {
                            List<List<FieldInfo>> guideData = Helper.UnseralizeData(buffer) as List<List<FieldInfo>>;

                            EditForm editForm = new EditForm(conn, id, guideName);
                            editForm.GuideData = guideData;
                            editForm.ShowDialog();
                        }
                    }
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ�������ݵ��򵼣�", "����������",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}