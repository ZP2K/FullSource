using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using DevComponents.DotNetBar;

using GraphElementLibrary;

namespace FlowChartEditor
{
    public partial class SearchResultForm : Office2007Form
    {
        GraphManager graphManager; // ��ͼ������
        List<GraphElement> graphElementList; // ͼԪ����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="graphManager">��ͼ������</param>
        /// <param name="graphElementList">ͼԪ����</param>
        public SearchResultForm(GraphManager graphManager, List<GraphElement> graphElementList)
        {
            this.graphManager = graphManager;
            this.graphElementList = graphElementList;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ��
        /// </summary>
        private void Init()
        {
            DataGridViewTextBoxColumn idColumn = new DataGridViewTextBoxColumn();
            idColumn.Name = "ID";
            idColumn.HeaderText = "ID";            

            DataGridViewTextBoxColumn textColumn = new DataGridViewTextBoxColumn();
            textColumn.Name = "Text";
            textColumn.HeaderText = "Text";            

            DataGridViewTextBoxColumn locationColumn = new DataGridViewTextBoxColumn();
            locationColumn.Name = "Location";
            locationColumn.HeaderText = "Location";            

            dataGridViewX1.Columns.Add(idColumn);
            dataGridViewX1.Columns.Add(textColumn);
            dataGridViewX1.Columns.Add(locationColumn);

            for (int i = 0; i < graphElementList.Count; i++)
            {
                GraphElement graphElement = graphElementList[i];
                dataGridViewX1.Rows.Add(1);
                DataGridViewRow newRow = dataGridViewX1.Rows[i];

                newRow.Cells["ID"].Value = graphElement.ID.ToString();
                newRow.Cells["Text"].Value = graphElement.Text.ToString();
                newRow.Cells["Location"].Value = string.Format("{0}, {1}", graphElement.Location.X, graphElement.Location.Y);
            }
        }

        /// <summary>
        /// ѡ��ĵ�Ԫ�ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void dataGridViewX1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            int index = e.RowIndex;

            if (index >= 0 && index < graphElementList.Count)
            {
                GraphElement graphElement = graphElementList[index];
                graphManager.SelectGraphElement(graphElement, true);
                this.Focus();
            }
        }
    }
}