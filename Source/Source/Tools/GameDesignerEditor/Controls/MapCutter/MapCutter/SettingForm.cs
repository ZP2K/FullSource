using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace MapCutter
{
    public partial class SettingForm : Form
    {
        private List<string> itemList; // ��������������
        private string areaName; // ��������
        private string backgroundMusic; // ��������
        private string rootPath; // ��·��

        public SettingForm(string id, List<string> itemList):this(id, "", itemList)
        {

        }

        public SettingForm(string id, string name, List<string> itemList):this(id, name, "", itemList)
        {

        }

        public SettingForm(string id, string name, string music, List<string> itemList)
        {
            this.itemList = itemList;
            InitializeComponent();
            InitData();
            this.Text = "�Ҷ�ͼѡ�� ID:" + id;
            textBoxX1.Text = name;
            comboBoxEx1.Text = music;
        }

        /// <summary>
        /// ��·������
        /// </summary>
        public string RootPath
        {
            set
            {
                rootPath = value;
            }
        }

        /// <summary>
        /// ������������������
        /// </summary>
        public List<string> ItemList
        {
            set
            {
                itemList = value;
            }
        }

        /// <summary>
        /// ������������
        /// </summary>
        public string AreaName
        {
            get
            {
                return areaName;
            }
        }

        /// <summary>
        /// ������������
        /// </summary>
        public string BackgroundMusic
        {
            get
            {
                return backgroundMusic;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData()
        {
            foreach(string s in itemList)
            {
                comboBoxEx1.Items.Add(s);
            }
        }

        /// <summary>
        /// ����ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            string fileName; // �ļ���
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Environment.SpecialFolder.MyComputer.ToString();
            openFileDialog.Filter = "�����ļ�(*.mp3)|*.mp3|�����ļ�(*.*)|*.*"; // �����ļ�����
            openFileDialog.InitialDirectory = rootPath;
            openFileDialog.FilterIndex = 0;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                fileName = openFileDialog.FileName;

                if (fileName.Contains(rootPath))
                {
                    int index = fileName.IndexOf(rootPath);
                    int startIndex = index + rootPath.Length;
                    fileName = fileName.Substring(startIndex).TrimStart(new char[] {'\\'});

                    if ((fileName != null) && (fileName != ""))
                    {
                        if(!itemList.Contains(fileName)) // �µ������ļ������������������
                        {
                            comboBoxEx1.Items.Add(fileName);
                            itemList.Add(fileName);    
                        }
                        comboBoxEx1.Text = fileName;
                    }
                }
                else
                {
                    MessageBox.Show("�����ļ������ڿͻ���Ŀ¼��!");
                }
            }
        }

        /// <summary>
        /// ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            areaName = textBoxX1.Text;
            backgroundMusic = comboBoxEx1.Text;

            if(areaName == "")
            {
                MessageBox.Show("��������û������!", "����������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if(backgroundMusic == "")
            {
                MessageBox.Show("��������û������!", "����������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            DialogResult = DialogResult.OK;
            this.Close();
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
    }
}