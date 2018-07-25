using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using DevComponents.DotNetBar;

using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor
{
    public partial class CheckCircleForm : Office2007Form
    {
        private GraphManager graphManager; // ��ͼ������
        private List<List<SlotContainer>> circleList; // ��ǰѭ����Ȧ


        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="graphManager">��ͼ������</param>
        /// <param name="circleList">��ǰѭ����Ȧ</param>
        public CheckCircleForm(GraphManager graphManager, List<List<SlotContainer>> circleList)
        {
            this.graphManager = graphManager;
            this.circleList = circleList;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ��
        /// </summary>
        private void Init()
        {
            this.Text = string.Format("ѭ����Ч�Լ�� ��{0}��ѭ�������޷���������", circleList.Count);
            string circleString = string.Empty;
            foreach(List<SlotContainer> list in circleList)
            {
                circleString = string.Empty;
                foreach(SlotContainer slotContainer in list)
                {
                    circleString += slotContainer.ID + " -> ";
                }
                circleString = circleString + list[0].ID;
                listBox1.Items.Add(circleString);
            }
        }

        /// <summary>
        /// �رմ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }

        /// <summary>
        /// ����ر��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void CheckCircleForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            foreach(List<SlotContainer> list in circleList)
            {
                foreach(SlotContainer slotContainer in list)
                {
                    slotContainer.ShowID = false;
                }
            }
        }

        /// <summary>
        /// ѡ��鿴ѭ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int index = listBox1.SelectedIndex;
            if (index != -1)
            {
                List<SlotContainer> currentList = circleList[index];
                List<GraphElement> list = new List<GraphElement>();
                foreach (SlotContainer slotContainer in currentList)
                {
                    slotContainer.ShowID = true;
                    list.Add(slotContainer);
                }

                graphManager.SelectGraphElements(list);
            }
        }
    }
}