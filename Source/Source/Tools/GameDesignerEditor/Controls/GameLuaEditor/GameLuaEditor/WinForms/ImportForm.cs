using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace GameLuaEditor
{
    public partial class ImportForm : Form
    {
        private List<string> scriptList;
        private string scriptFolder;
        private TreeNode rootNode;
        private string fileFolder;
        private bool selectAll = true;

        // ����������õĻص�
        public delegate TreeNode CreateNewNode(TreeNode parentNode, string scriptName, string scriptID);
        private CreateNewNode createNewNode;

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="rootNode">�����</param>
        /// <param name="scriptList">�ű�����</param>
        /// <param name="fileFolder">�ļ��ļ���</param>
        public ImportForm(TreeNode rootNode,List<string> scriptList, string fileFolder)
        {
            this.rootNode = rootNode;
            this.scriptList = scriptList;
            this.fileFolder = fileFolder;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ���������
        /// </summary>
        public CreateNewNode CreateTheNewNode
        {
            set
            {
                createNewNode = value;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            scriptFolder = rootNode.FullPath;

            scriptListBox.Items.Clear();

            for (int i = 0; i < scriptList.Count; i++)
            {
                scriptListBox.Items.Add(scriptList[i]);
                scriptListBox.SetItemChecked(i, true);
            }            
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bOK_Click(object sender, EventArgs e)
        {
            bool coverScript = coverScriptBox.Checked;

            if (coverScript)
            {
                if (MessageBox.Show("ȷ��Ҫ�������нű���", " ����ű�",
                                    MessageBoxButtons.OKCancel, MessageBoxIcon.Question)
                    == DialogResult.Cancel) 
                {
                    coverScript = false;
                }
            }

            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            int successCount = 0;

            scriptList.Clear();

            for (int i = 0; i < scriptListBox.Items.Count; i++)
            {
                if (scriptListBox.GetItemChecked(i))
                {
                    scriptList.Add(scriptListBox.Items[i] as string);
                }
            }

            ProgressForm progressForm = new ProgressForm(0, scriptList.Count);
            progressForm.Show();

            for (int i = 0; i < scriptList.Count; i++)
            {
                string scriptName = scriptList[i];
                string fileName = Path.Combine(fileFolder, scriptName);
                string content = Helper.FileToString(fileName);

                progressForm.ShowProgress(i, string.Format("����ű�{0}������...", scriptName));

                if (!string.IsNullOrEmpty(content))
                {
                    string dataBasePath = Path.Combine(scriptFolder, scriptName);
                    string id = dataBaseManager.CreateScriptData(dataBasePath, content, coverScript);

                    if (id != null)
                    {
                        createNewNode(rootNode, scriptName, id);
                        successCount++;
                    }
                }
            }

            progressForm.Close();

            MessageBox.Show(string.Format("������{0}���ű��ļ���", successCount), "����ű�",
                            MessageBoxButtons.OK, MessageBoxIcon.Information);

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
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// ȫ��ѡ��/ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void selectAllBox_CheckedChanged(object sender, EventArgs e)
        {
            selectAll = !selectAll;

            for (int i = 0; i < scriptListBox.Items.Count; i++)
            {
                scriptListBox.SetItemChecked(i, selectAll);
            }
        }
    }
}