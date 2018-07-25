using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace StandardNpcCreate
{
    public partial class NpcTypeSelectForm : Form
    {
        private List<string> detailTypeList; // npc��������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="detailTypeList">npc��������</param>
        /// <param name="titleText">�������</param>
        public NpcTypeSelectForm(List<string> detailTypeList, string titleText)
        {
            this.detailTypeList = detailTypeList;
            InitializeComponent();
            Init();
            this.Text = titleText;
        }

        /// <summary>
        /// ѡ����
        /// </summary>
        public int SelectionIndex
        {
            get
            {
                return comboBoxEx1.SelectedIndex;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            foreach(string s in detailTypeList)
            {
                comboBoxEx1.Items.Add(s);
            }

            comboBoxEx1.SelectedIndex = 0;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            this.Close();
        }
    }
}