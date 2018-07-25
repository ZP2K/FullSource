using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace StandardNpcCreate
{
    public partial class UpdateResultForm : Form
    {
        private List<string> updateSuccessNpcList; // ���¶����ɹ�NPC����
        private List<string> updateFailNpcList; // ���¶���ʧ��NPC����
        private string logText; // log�ı�

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="updateSuccessNpcList">���¶����ɹ�NPC����</param>
        /// <param name="updateFailNpcList">���¶���ʧ��NPC����</param>
        public UpdateResultForm(List<string> updateSuccessNpcList, List<string> updateFailNpcList)
        {
            this.updateSuccessNpcList = updateSuccessNpcList;
            this.updateFailNpcList = updateFailNpcList;
            InitializeComponent();
        }

        /// <summary>
        /// log�ı�
        /// </summary>
        public string LogText
        {
            set
            {
                logText = value;
            }
        }

        /// <summary>
        /// ���ݳ�ʼ��
        /// </summary>
        public void Init()
        {
            int successCount = updateSuccessNpcList.Count;
            int failCount = updateFailNpcList.Count;
            int totalCount = successCount + failCount;

            StringBuilder content = new StringBuilder();
            content.AppendLine(string.Format("������{0}��NPC...\r\n", totalCount.ToString()));
            content.AppendLine(string.Format("{0}��NPC���³ɹ���", successCount.ToString()));

            foreach(string s in updateSuccessNpcList)
            {
                content.AppendLine(s);
            }

            content.AppendLine();
            content.AppendLine(string.Format("{0}��NPC����ʧ�ܣ�", failCount.ToString()));

            foreach(string s in updateFailNpcList)
            {
                content.AppendLine(s);
            }

            textBoxX1.Text = string.Format("{0}\r\n��־��\r\n{1}", content.ToString(), logText.ToString());
        }
    }
}