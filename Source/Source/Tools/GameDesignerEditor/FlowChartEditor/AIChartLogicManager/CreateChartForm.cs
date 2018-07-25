using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

using DevComponents.DotNetBar;

using DBManager;

namespace AIChartLogicManager
{
    public partial class CreateChartForm : Office2007Form
    {
        private Hashtable modelTable = new Hashtable(); // ģ�����ݹ�ϣ��
        private string currentPath; // ��ǰĿ¼
        private string chartID; // �½���ͼ��ID
        private string chartName; // �½���ͼ������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="currentPath">��ǰĿ¼</param>
        public CreateChartForm(string currentPath)
        {
            this.currentPath = currentPath;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// �½���ͼ��ID
        /// </summary>
        public string ChartID
        {
            get
            {
                return chartID;
            }
        }

        /// <summary>
        /// �½���ͼ������
        /// </summary>
        public string ChartName
        {
            get
            {
                return chartName;
            }
        }

        /// <summary>
        /// ��ʼ��
        /// </summary>
        private void Init()
        {
            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            DataTable infoTable = dataBaseManager.GetDiagramInformation("AIͼ");

            foreach (DataRow row in infoTable.Rows)
            {
                string fullPath = row["FullPath"].ToString();

                if (fullPath.StartsWith("�ο�ģ��") && !(bool)row["IsFolder"])
                {
                    string modelName = fullPath.Replace("�ο�ģ��", "");
                    modelName = modelName.Replace("\\", "");
                    string modelID = row["ID"].ToString();

                    comboBoxEx1.Items.Add(modelName);                    
                    modelTable[modelName] = modelID;
                }
            }
        }

        /// <summary>
        /// ������AIͼ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                radioButton2.Checked = false;
                comboBoxEx1.Enabled = false;
            }
        }

        /// <summary>
        /// ��ģ�帴��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton2.Checked)
            {
                radioButton1.Checked = false;
                comboBoxEx1.Enabled = true;
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            bool editSuccess = false;
            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            
            chartName = textBoxX1.Text;
            if (chartName != "")
            {                
                string fullPath = string.Format("{0}\\{1}", currentPath, chartName);

                if (radioButton1.Checked) // ������AIͼ
                {
                    string originID = dataBaseManager.CreateNewDiagram(fullPath, false, "AIͼ");
                    chartID = originID;
                    string destinID = "311";

                    if (originID != null)
                    {
                        editSuccess = dataBaseManager.CopyDiagramData(originID, destinID);
                    }                
                }
                else // ��ģ�帴��
                {
                    string modelName = comboBoxEx1.Text;

                    if (modelName != "")
                    {
                        string originID = dataBaseManager.CreateNewDiagram(fullPath, false, "AIͼ");
                        chartID = originID;
                        string destinID = modelTable[modelName] as string;

                        if (originID != null)
                        {
                            editSuccess = dataBaseManager.CopyDiagramData(originID, destinID);
                        } 
                    }
                    else
                    {
                        MessageBox.Show("����ģ�岻��Ϊ�գ�", "�½���ͼ", MessageBoxButtons.OK,
                            MessageBoxIcon.Information);
                    }
                }
            }
            else
            {
                MessageBox.Show("��ͼ���Ʋ���Ϊ�գ�", "�½���ͼ", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }            

            if (editSuccess)
            {
                DialogResult = DialogResult.OK;
                this.Close();
            }
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}