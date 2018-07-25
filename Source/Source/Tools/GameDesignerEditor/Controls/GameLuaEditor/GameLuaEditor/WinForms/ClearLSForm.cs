using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using System.Text.RegularExpressions;

namespace GameLuaEditor
{
    public partial class ClearLSForm : Form
    {
        private bool beginEdit = false; // �Ƿ��Ѿ���ʼ�༭
        private bool selectAll = true; // ȫ��ѡ��
        private DataTable lsTable; // ls���ݱ�
        private DataTable scriptTable; // �ű����ݱ�
        private Hashtable lsHashtable = new Hashtable(); // ls�Ĺ�ϣ��
        private DataBaseManager dataBaseManager; // ���ݿ⽻����

        /// <summary>
        /// ���캯��
        /// </summary>
        public ClearLSForm()
        {
            InitializeComponent();
            InitData();
            beginEdit = true;
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData()
        {
            dataBaseManager = DataBaseManager.GetDataBaseManager();
            lsTable = dataBaseManager.GetLSInformation();
            scriptTable = dataBaseManager.GetScriptInformation();

            if((lsTable != null) && (scriptTable != null)) // ���ݱ�ǿ�
            {
                // �������������
                List<string> lsClassification = dataBaseManager.GetLSClassification();
                foreach(string s in lsClassification)
                {
                    comboBoxEx1.Items.Add(s);
                }
            }
        }
       
        /// <summary>
        /// ��ȡָ����������ʹ�ù���LS�ַ���
        /// </summary>
        /// <param name="classification">����</param>
        /// <param name="sceneScript">�Ƿ��ǳ����µĽű�</param>
        /// <returns>LS�ַ���</returns>
        private DataRow[] GetValidStringDataRow(string classification, bool sceneScript)
        {
            List<DataRow> validStringDataRowList = new List<DataRow>();            
            List<string> idList = new List<string>(); // �ű�����

            string folderName = string.Format(@"scripts\Map\{0}", classification);
            string condition = string.Format(@"path LIKE 'scripts\Map\{0}\%'", classification);

            if (!sceneScript) // �ǳ��������µ��ַ���
            {
                folderName = string.Format(@"scripts\{0}", classification);
                condition = string.Format(@"path LIKE 'scripts\{0}%'", classification);
            }

            folderName = Path.Combine(Helper.ClientPath, folderName);
            DirectoryInfo di = new DirectoryInfo(folderName);

            if (di.Exists)
            {
                GetScriptID(di, idList);
            }

            DataRow[] lsRows = lsTable.Select(string.Format("classification = '{0}'", classification));

            foreach (DataRow r in lsRows)
            {                
                string lsID = r["id"].ToString();

                if (idList.Contains(lsID))
                {
                    validStringDataRowList.Add(r);
                }
            }

            return validStringDataRowList.ToArray();
        }

        /// <summary>
        /// ��ȡ�ű��ַ���ID����
        /// </summary>
        /// <param name="directoryInfo">��ǰĿ¼</param>
        /// <param name="idList">�ַ���ID����</param>
        private void GetScriptID(DirectoryInfo directoryInfo, List<string> idList)
        {
            foreach (DirectoryInfo di in directoryInfo.GetDirectories())
            {
                if (!di.Attributes.ToString().Contains("Hidden"))
                {
                    GetScriptID(di, idList);
                }
            }

            foreach (FileInfo fi in directoryInfo.GetFiles())
            {
                if (fi.Extension == ".lua")
                {
                    string fileName = fi.FullName;
                    string content = dataBaseManager.GetScriptDataFromFile(fileName);
                    List<string> list = Helper.GetScriptID(content, "SCENE_STRING_TABLE");

                    foreach (string lsID in list)
                    {
                        if (!idList.Contains(lsID))
                        {
                            idList.Add(lsID);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// ��ȡָ������������û��ʹ�ù���LS�ַ���
        /// </summary>
        /// <param name="classification">����</param>
        /// <returns>LS�ַ���</returns>
        private List<string> GetUnuseStringList(string classification)
        {
            List<string> unuseStringList = new List<string>();            
            string content;            
            List<string> scriptList = new List<string>(); // �ű�����

            string condition = string.Format(@"path LIKE 'scripts\Map\{0}\%'", classification);

            if (!checkBoxX1.Checked) // �ǳ��������µ��ַ���
            {
                condition = string.Format(@"path LIKE 'scripts\{0}%'", classification);
            }

            DataRow[] scriptRows = scriptTable.Select(condition);
            
            foreach(DataRow r in scriptRows)
            {
                string id = r["id"].ToString();
                content = dataBaseManager.GetScriptData(id);
                List<string> list = CodeConvert.GetStringList(content);

                foreach (string s in list)
                {
                    if(!scriptList.Contains(s))
                    {
                        scriptList.Add(s);
                    }
                }
            }

            DataRow[] lsRows = lsTable.Select(string.Format("classification = '{0}'", classification));
            
            foreach(DataRow r in lsRows)
            {
                content = r["content"].ToString();

                if (!scriptList.Contains(content))
                {
                    unuseStringList.Add(content);
                }
            }

            return unuseStringList;
        }

        /// <summary>
        /// ����ls��
        /// </summary>
        private void ClearLSTable()
        {
            List<string> lsList = new List<string>();
            string classification = comboBoxEx1.Text;
            string id;
            DataRow[] lsRows;
            int count = 0;

            if(classification == "") // �ų��շ�������
            {
                return;
            }

            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                if (checkedListBox1.GetItemChecked(i))
                {
                    lsList.Add(checkedListBox1.Items[i].ToString());
                }
            }

            foreach(string s in lsList)
            {
                lsRows = lsTable.Select(string.Format("classification = '{0}' AND content = '{1}'", classification, s.Replace("'", "''")));
                if(lsRows.Length > 0)
                {
                    foreach(DataRow r in lsRows)
                    {
                        id = r["id"].ToString();
                        dataBaseManager.DeleteLsData(id);
                        count++;
                    }           
                }
            }

            // ��������
            lsHashtable[classification] = new List<string>();
            checkedListBox1.Items.Clear();
            MessageBox.Show(string.Format("{0}�����{1}��ls�����ַ����Ѿ�����!", classification, count.ToString()));
        }

        /// <summary>
        /// ��ʼ����ls��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {            
            if (MessageBox.Show("����ȷ�����˶Կ��нű����б༭ʱ��ִ������LS�ļ�����������LS�ļ��󣬲�����ɾ���ű������޷��ָ����Ƿ������", 
                                "����LS�ļ�ȷ��", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
            {
                ClearLSTable();
            }
        }

        /// <summary>
        /// ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }

        /// <summary>
        /// �б�ѡ��ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxEx1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (beginEdit) // ��ʼ�༭
            {
                int index = comboBoxEx1.SelectedIndex;
                if(index >= 0) // ��Ч��ѡ��
                {
                    checkedListBox1.Items.Clear();
                    string value = comboBoxEx1.Text;
                    List<string> lsList = lsHashtable[value] as List<string>;
                    if(lsList == null) // ��ϣ����û�л�������ݣ���Ҫ���¶�ȡ
                    {
                        lsList = GetUnuseStringList(value);
                    }
                    
                    foreach(string s in lsList)
                    {
                        checkedListBox1.Items.Add(s);
                    }

                    this.Text = string.Format("LS���ַ������� ���ҵ�{0}���������õ��ַ���", lsList.Count.ToString());
                }
            }
        }

        /// <summary>
        /// ȫ��ѡ��/ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < checkedListBox1.Items.Count; i++)
            {
                checkedListBox1.SetItemChecked(i, selectAll);
            }

            selectAll = !selectAll;

            if (selectAll)
            {                
                buttonX3.Text = "ȫ��ѡ��";
            }
            else
            {
                buttonX3.Text = "ȫ��ȡ��";
            }
        }

        /// <summary>
        /// �������������ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bClearAndExport_Click(object sender, EventArgs e)
        {
            List<TreeNode> nodeList = new List<TreeNode>();

            foreach (TreeNode node in MainForm.CurrentMainForm.tree.Nodes["scripts"].Nodes)
            {
                if (node.Text == "Map")
                {
                    foreach (TreeNode sceneNode in node.Nodes)
                    {
                        nodeList.Add(sceneNode);
                    }
                }
                else
                {
                    nodeList.Add(node);
                }
            }

            ProgressForm pForm = new ProgressForm(1, nodeList.Count);
            pForm.Show();
            int index = 1;

            foreach (TreeNode node in nodeList)
            {
                string classification = node.Text;
                bool sceneScript = (node.Parent.Text == "Map");

                pForm.ShowProgress(index, string.Format("����{0}�����µ�LS���ݱ�...", classification));
                index++;

                string tableName = "SCENE_STRING_TABLE";
                string fileName = string.Format(@"scripts\Map\{0}\include\scenestrings.ls", classification);

                if (!sceneScript)
                {
                    tableName = string.Format("{0}_STRING_TABLE", classification.ToUpper());
                    fileName = string.Format(@"scripts\{0}\include\strings.ls", classification);
                }                                

                DataRow[] lsRows = GetValidStringDataRow(classification, sceneScript);
                string content = CodeConvert.ConvertLsFileContent(lsRows, tableName);

                fileName = Path.Combine(Helper.ClientPath, fileName);
                Helper.WriteStringToFile(content, fileName);
            }

            MessageBox.Show("LS�ļ������ɹ���", "��������LS�ļ�",
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }
}