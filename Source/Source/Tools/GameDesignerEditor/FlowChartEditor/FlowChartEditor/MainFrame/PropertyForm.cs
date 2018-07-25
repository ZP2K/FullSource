using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

using DevComponents.DotNetBar;

using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor
{
    public partial class PropertyForm : Office2007Form
    {
        List<GraphElement> graphElementList; // ͼԪ����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="graphElementList">ͼԪ����</param>
        public PropertyForm(List<GraphElement> graphElementList)
        {
            this.graphElementList = graphElementList;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        public void Init()
        {
            int tempInt = 0;
            string tempString = "0";
            Type intType = tempInt.GetType();
            Type stringType = tempString.GetType();

            // ���������
            DataGridViewTextBoxColumn totalEntityColumn = new DataGridViewTextBoxColumn();
            totalEntityColumn.Name = "TotalEntity";
            totalEntityColumn.HeaderText = "ͳ����";
            totalEntityColumn.ValueType = stringType;

            DataGridViewTextBoxColumn totalCountColumn = new DataGridViewTextBoxColumn();
            totalCountColumn.Name = "TotalCount";
            totalCountColumn.HeaderText = "����";
            totalCountColumn.ValueType = intType;

            totalView.Columns.Add(totalEntityColumn);
            totalView.Columns.Add(totalCountColumn);            

            DataGridViewTextBoxColumn nodeEntityColumn = new DataGridViewTextBoxColumn();
            nodeEntityColumn.Name = "NodeEntity";
            nodeEntityColumn.HeaderText = "ͳ����";
            nodeEntityColumn.ValueType = stringType;

            DataGridViewTextBoxColumn nodeCountColumn = new DataGridViewTextBoxColumn();
            nodeCountColumn.Name = "NodeCount";
            nodeCountColumn.HeaderText = "����";
            nodeCountColumn.ValueType = intType;

            nodeView.Columns.Add(nodeEntityColumn);
            nodeView.Columns.Add(nodeCountColumn);

            DataGridViewTextBoxColumn lineEntityColumn = new DataGridViewTextBoxColumn();
            lineEntityColumn.Name = "LineEntity";
            lineEntityColumn.HeaderText = "ͳ����";
            lineEntityColumn.ValueType = stringType;

            DataGridViewTextBoxColumn lineCountColumn = new DataGridViewTextBoxColumn();
            lineCountColumn.Name = "LineCount";
            lineCountColumn.HeaderText = "����";
            lineCountColumn.ValueType = intType;

            lineView.Columns.Add(lineEntityColumn);
            lineView.Columns.Add(lineCountColumn);

            // ���������        
            int nodeCount = 0;
            int lineCount = 0;
            Hashtable nodeTable = new Hashtable();
            Hashtable lineTable = new Hashtable();

            foreach(GraphElement graphElement in graphElementList)
            {
                string graphElementName = graphElement.Text;

                if (graphElement is SlotContainer)
                {                   
                    if (!string.IsNullOrEmpty(graphElementName))
                    {
                        string countString = nodeTable[graphElementName] as string;

                        if (countString != null)
                        {
                            int count = int.Parse(countString);
                            count++;
                            nodeTable[graphElementName] = count.ToString();
                        }
                        else
                        {
                            nodeTable[graphElementName] = "1";
                        }
                    }

                    nodeCount++;
                }
                else if (graphElement is ConnectorContainer)
                {
                    if (!string.IsNullOrEmpty(graphElementName))
                    {
                        string countString = lineTable[graphElementName] as string;

                        if (countString != null)
                        {
                            int count = int.Parse(countString);
                            count++;
                            lineTable[graphElementName] = count.ToString();
                        }
                        else
                        {
                            lineTable[graphElementName] = "1";
                        }
                    }

                    lineCount++;
                }
            }

            int index = 0;            
            int nodeMaxCount = -1;
            int nodeMinCount = -1;
            int lineMaxCount = -1;
            int lineMinCount = -1;
            string nodeMaxName = null;
            string nodeMinName = null;
            string lineMaxName = null;
            string lineMinName = null;
            DataGridViewRow newRow;
            
            foreach(object key in nodeTable.Keys)
            {
                string nodeName = key as string;
                string countString = nodeTable[key] as string;
                int count = int.Parse(countString);

                nodeView.Rows.Add();
                newRow = nodeView.Rows[index];
                newRow.Cells["NodeEntity"].Value = nodeName;
                newRow.Cells["NodeCount"].Value = count;

                if (count > nodeMaxCount || nodeMaxCount == -1)
                {
                    nodeMaxCount = count;
                    nodeMaxName = nodeName;
                }

                if (count < nodeMinCount || nodeMinCount == -1)
                {
                    nodeMinCount = count;
                    nodeMinName = nodeName;
                }

                index++;
            }
            nodeView.Sort(nodeCountColumn, ListSortDirection.Descending);

            index = 0;
            foreach(object key in lineTable.Keys)
            {
                string lineName = key as string;
                string countString = lineTable[key] as string;
                int count = int.Parse(countString);

                lineView.Rows.Add();
                newRow = lineView.Rows[index];
                newRow.Cells["LineEntity"].Value = lineName;
                newRow.Cells["LineCount"].Value = count;

                if (count > lineMaxCount || lineMaxCount == -1)
                {
                    lineMaxCount = count;
                    lineMaxName = lineName;
                }

                if (count < lineMinCount || lineMinCount == -1)
                {
                    lineMinCount = count;
                    lineMinName = lineName;
                }

                index++;
            }
            lineView.Sort(lineCountColumn, ListSortDirection.Descending);

            index = 0;
            totalView.Rows.Add();
            newRow = totalView.Rows[index];
            newRow.Cells["TotalEntity"].Value = "���";
            newRow.Cells["TotalCount"].Value = nodeCount;
            index++;

            totalView.Rows.Add();
            newRow = totalView.Rows[index];
            newRow.Cells["TotalEntity"].Value = "������";
            newRow.Cells["TotalCount"].Value = lineCount;
            index++;

            if (nodeMaxName != null)
            {
                totalView.Rows.Add();
                newRow = totalView.Rows[index];
                newRow.Cells["TotalEntity"].Value = string.Format("��������㣺{0}", nodeMaxName);
                newRow.Cells["TotalCount"].Value = nodeMaxCount;
                index++;
            }

            if (nodeMinName != null)
            {
                totalView.Rows.Add();
                newRow = totalView.Rows[index];
                newRow.Cells["TotalEntity"].Value = string.Format("�������ٽ�㣺{0}", nodeMinName);
                newRow.Cells["TotalCount"].Value = nodeMinCount;
                index++;
            }

            if (lineMaxName != null)
            {
                totalView.Rows.Add();
                newRow = totalView.Rows[index];
                newRow.Cells["TotalEntity"].Value = string.Format("������������ߣ�{0}", lineMaxName);
                newRow.Cells["TotalCount"].Value = lineMaxCount;
                index++;
            }

            if (lineMinName != null)
            {
                totalView.Rows.Add();
                newRow = totalView.Rows[index];
                newRow.Cells["TotalEntity"].Value = string.Format("�������������ߣ�{0}", lineMinName);
                newRow.Cells["TotalCount"].Value = lineMinCount;
                index++;
            }

            tabControl1.SelectedTabIndex = 0;
        }
    }
}