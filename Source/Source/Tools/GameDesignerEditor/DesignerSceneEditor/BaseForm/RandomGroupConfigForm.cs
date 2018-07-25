using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DesignerSceneEditor
{
	public partial class RandomGroupConfigForm: Form
	{
        private List<string> npcInfoList;
        private List<string> npcTemplateIDList;

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="npcInfoList">NPC��Ϣ����</param>
		public RandomGroupConfigForm(List<string> npcInfoList)
		{
            this.npcInfoList = npcInfoList;
			InitializeComponent();
            Init();
		}

        /// <summary>
        /// NPCģ��ID����
        /// </summary>
        public List<string> NpcTemplateIDList
        {
            get
            {
                return npcTemplateIDList;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            npcTemplateIDList = new List<string>();

            listBox1.Items.Clear();

            foreach (string s in npcInfoList)
            {
                listBox1.Items.Add(s);
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedItems.Count == 0)
            {
                MessageBox.Show("�����������Ҫ����һ��ģ��ID��", "���������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            foreach (string s in listBox1.SelectedItems)
            {
                int index = s.IndexOf(" ");
                string npcTemplateID = s.Substring(index + 2);
                npcTemplateID = npcTemplateID.Substring(0, npcTemplateID.Length - 1);

                npcTemplateIDList.Add(npcTemplateID);
            }

            DialogResult = DialogResult.OK;
            this.Close();
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