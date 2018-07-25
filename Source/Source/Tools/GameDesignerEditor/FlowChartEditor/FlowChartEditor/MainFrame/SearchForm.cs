using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

using DevComponents.DotNetBar;

using GraphElementLibrary;
using DataElementLibrary;
using FlowChartEditor.BaseGraphElement;

namespace FlowChartEditor
{
    public partial class SearchForm : Office2007Form
    {
        private string searchType = "Node"; // ��������
        private string contentType = "Text"; // �������� 
        private int searchID = 0; // Ҫ������ID
        private string searchText = ""; // Ҫ�������ı�
        private FlowChartManager flowChartManager; // ����ͼ�������

        private List<string> searchTextList = new List<string>(); // �����ı��ļ�¼
        private List<string> searchScriptList = new List<string>(); // ���ҽű��ļ�¼

        /// <summary>
        /// ���캯��
        /// </summary>
        public SearchForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// ��ǰ����ͼ�������
        /// </summary>
        public FlowChartManager CurrentFlowChartManager
        {
            set
            {
                flowChartManager = value;
            }
        }        

        /// <summary>
        /// ��ʼ���ı��������б�
        /// </summary>
        /// <param name="searchText">���ҵ��ı�</param>
        private void InitTextList(string searchText)
        {
            if (!searchTextList.Contains(searchText))
            {
                searchTextList.Add(searchText);
                textBox.Items.Add(searchText);
            }
        }

        /// <summary>
        /// ��ʼ���ű��������б�
        /// </summary>
        /// <param name="searchScript">���ҵĽű�</param>
        private void InitScriptList(string searchScript)
        {
            if (!searchScriptList.Contains(searchScript))
            {
                searchScriptList.Add(searchScript);
                scriptBox.Items.Add(searchScript);
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (radioButton1.Checked) // ����������
            {
                searchType = "Node";
            }
            else if (radioButton2.Checked) // �������
            {
                searchType = "Line";
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ���ҵ����ͣ�", "����",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (radioButton3.Checked) // ����ID
            {
                contentType = "ID";
                searchID = idBox.Value;

                if (searchID == 0)
                {
                    MessageBox.Show("Ҫ���ҵ�ID����Ϊ�գ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }                
            }
            else if (radioButton4.Checked) // �����ı�
            {
                contentType = "Text";
                searchText = textBox.Text;
                InitTextList(searchText);

                if (searchText == "")
                {
                    MessageBox.Show("Ҫ���ҵ��ı�����Ϊ�գ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }
            }
            else if (radioButton5.Checked) // �����ű�
            {
                contentType = "Script";
                searchText = scriptBox.Text;
                InitScriptList(searchText);

                if (searchText == "")
                {
                    MessageBox.Show("Ҫ���ҵĽű�����Ϊ�գ�", "����", 
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }
            }
            else 
            {
                MessageBox.Show("����ѡ��Ҫ���ҵ����ݣ�", "����",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            List<GraphElement> graphElementList = new List<GraphElement>();

            switch (searchType)
            {
                case "Node":
                    {
                        switch(contentType)
                        {
                            case "ID":
                                {
                                    foreach (GraphElement graphElement in flowChartManager.CurrentGraphManager.SlotContainerList)
                                    {
                                        if (graphElement.ID == searchID)
                                        {
                                            graphElementList.Add(graphElement);
                                        }
                                    }

                                    break;
                                }
                            case "Text":
                                {
                                    foreach(GraphElement graphElement in flowChartManager.CurrentGraphManager.SlotContainerList)
                                    {
                                        if (graphElement.Text.Contains(searchText))
                                        {
                                            graphElementList.Add(graphElement);
                                        }
                                    }                                    

                                    break;
                                }
                            case "Script":
                                {
                                    foreach (GraphElement graphElement in flowChartManager.CurrentGraphManager.SlotContainerList)
                                    {
                                        DataElement dataElement = flowChartManager.CurrentDataManager.GetDataElement(graphElement);

                                        if (dataElement != null)
                                        {
                                            switch (dataElement.DataType)
                                            {
                                                case "AIActionNode":
                                                    {
                                                        Hashtable infoTable = dataElement.GetOutputArgs();
                                                        string scriptContent = infoTable["Script"] as string;
                                                        
                                                        if (scriptContent != null)
                                                        {
                                                            if (scriptContent.Contains(searchText))
                                                            {
                                                                graphElementList.Add(graphElement);
                                                            }
                                                        }
                                                        
                                                        break;
                                                    }
                                                case "AIActionsNode":
                                                    {
                                                        Hashtable infoTable = dataElement.GetOutputArgs();
                                                        List<string> scriptContentList = infoTable["ScriptList"] as List<string>;

                                                        if (scriptContentList != null)
                                                        {
                                                            foreach (string s in scriptContentList)
                                                            {
                                                                if (s.Contains(searchText))
                                                                {
                                                                    graphElementList.Add(graphElement);
                                                                    break;
                                                                }
                                                            }
                                                        }                                                        

                                                        break;
                                                    }
                                            }
                                        }
                                    }

                                    break;
                                }
                        }

                        break;
                    }
                case "Line":
                    {
                        switch(contentType)
                        {
                            case "ID":
                                {
                                    foreach(GraphElement graphElement in flowChartManager.CurrentGraphManager.ConnectorContainerList)
                                    {
                                        if (graphElement.ID == searchID)
                                        {
                                            graphElementList.Add(graphElement);
                                        }
                                    }

                                    break;
                                }
                            case "Text":
                                {
                                    foreach(GraphElement graphElement in flowChartManager.CurrentGraphManager.ConnectorContainerList)
                                    {
                                        if (graphElement.Text.Contains(searchText))
                                        {
                                            graphElementList.Add(graphElement);
                                        }
                                    }                                    

                                    break;
                                }
                        }

                        break;
                    }
            }                

            SearchResultForm sForm = new SearchResultForm(flowChartManager.CurrentGraphManager, graphElementList);
            sForm.Show();            
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Visible = false;
        }

        /// <summary>
        /// ����ر��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void SearchForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Visible = false;
        }
    }
}