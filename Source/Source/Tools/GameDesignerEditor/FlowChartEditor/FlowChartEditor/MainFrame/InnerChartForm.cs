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
using DBManager;
using FlowChartEditor.BaseGraphElement;

namespace FlowChartEditor
{
    public partial class InnerChartForm : Office2007Form
    {
        private DocumentManager documentManager;
        private Hashtable idTable = new Hashtable();
        private Hashtable diagramTable = new Hashtable();
        private string innerChartPath = ""; // �ӻ�ͼ·��
        private int inSlotCount = 0; // ��ڲ������
        private int outSlotCount = 0; // ���ڲ������
        private Hashtable inSlotInfo = new Hashtable(); // ��ڲ����Ϣ
        private Hashtable outSlotInfo = new Hashtable(); // ���ڲ����Ϣ
        private Hashtable interfaceNodeTable = new Hashtable(); // �ӿڽ�����ݱ�
        private List<GraphElement> graphElementList = new List<GraphElement>(); // ͼԪ����
        private Hashtable graphTable = new Hashtable(); // ͼԪ���ݱ�        

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="innerChartPath">�ӻ�ͼ·��</param>
        public InnerChartForm(string innerChartPath)
        {
            this.innerChartPath = innerChartPath;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ͼԪ���ݱ�
        /// </summary>
        public Hashtable GraphTable
        {
            get
            {
                return graphTable;
            }
        }

        /// <summary>
        /// ͼԪ����
        /// </summary>
        public List<GraphElement> GraphElementList
        {
            get
            {
                return graphElementList;
            }
        }

        /// <summary>
        /// �ӿڽ�����ݱ�
        /// </summary>
        public Hashtable InterfaceNodeTable
        {
            get
            {
                return interfaceNodeTable;
            }
        }

        /// <summary>
        /// ��ڲ����Ϣ
        /// </summary>
        public Hashtable InSlotInfo
        {
            get
            {
                return inSlotInfo;
            }
        }

        /// <summary>
        /// ���ڲ����Ϣ
        /// </summary>
        public Hashtable OutSlotInfo
        {
            get
            {
                return outSlotInfo;
            }
        }
        
        /// <summary>
        /// ��ڲ������
        /// </summary>
        public int InSlotCount
        {
            get
            {
                return inSlotCount;
            }
        }

        /// <summary>
        /// ���ڲ������
        /// </summary>
        public int OutSlotCount
        {
            get
            {
                return outSlotCount;
            }
        }

        /// <summary>
        /// �ӻ�ͼ·��
        /// </summary>
        public string InnerChartPath
        {
            get
            {
                return innerChartPath;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            documentManager = DocumentManager.GetDocumentManager();
            TreeView currentTree = documentManager.CurrentTree;
            TreeNode node = currentTree.Nodes[0].Nodes["�ӻ�ͼ"];

            innerChartBox.Items.Clear();

            if (node != null)
            {
                foreach (TreeNode innerChartNode in node.Nodes)
                {
                    innerChartBox.Items.Add(innerChartNode.Text);
                    idTable[innerChartNode.Text] = innerChartNode.Tag;
                }
            }

            innerChartBox.Text = innerChartPath;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bOK_Click(object sender, EventArgs e)
        {
            innerChartPath = innerChartBox.Text;

            if (innerChartPath != "")
            {
                string id = idTable[innerChartPath].ToString();
                Hashtable infoTable = diagramTable[id] as Hashtable;
                 
                if (infoTable != null)
                {
                    Hashtable logicData = infoTable["logicData"] as Hashtable;
                    graphTable = logicData["graphTable"] as Hashtable;

                    foreach (GraphElement graphElement in graphTable.Keys)
                    {
                        if (graphElement is InterfaceGraphElement)
                        {
                            InterfaceGraphElement interfaceGraphElement = graphElement as InterfaceGraphElement;
                            InterfaceNodeEditor interfaceEditor = graphTable[graphElement] as InterfaceNodeEditor;
                            
                            if (interfaceEditor.IsInput)
                            {
                                inSlotInfo[interfaceEditor.ID.ToString()] = interfaceEditor.InterfaceName;
                                inSlotCount++;
                            }
                            else
                            {
                                outSlotInfo[interfaceEditor.ID.ToString()] = interfaceEditor.InterfaceName;
                                outSlotCount++;
                            }

                            interfaceNodeTable[interfaceEditor.ID.ToString()] = graphElement;                            
                        }
                        else // ��¼���еķǽӿڽ��
                        {
                            graphElementList.Add(graphElement);
                        }
                    }                    
                }

                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("����ѡ���ӻ�ͼ��", "�ӻ�ͼ�༭",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// �鿴��ͼ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bView_Click(object sender, EventArgs e)
        {
            innerChartPath = innerChartBox.Text;

            if (innerChartPath != "")
            {
                string nodePath = string.Format(@"�ӻ�ͼ\{0}", innerChartPath);
                documentManager.OpenDiagramInPath(nodePath, "AIChart");
                this.Close();
            }            
        }

        /// <summary>
        /// ѡ���ͼ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void innerChartBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            innerChartPath = innerChartBox.Text;
            string id = idTable[innerChartPath].ToString();
            Hashtable infoTable = null;
            lDescription.Text = "�ӻ�ͼ����";

            if (diagramTable[id] == null)
            {
                DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
                infoTable = dataBaseManager.GetDiagramData(id) as Hashtable;
                diagramTable[id] = infoTable;
            }
            else
            {
                infoTable = diagramTable[id] as Hashtable;
            }
            
            if (infoTable != null)
            {
                Hashtable customData = infoTable["customData"] as Hashtable;

                if (customData != null)
                {
                    string description = customData["description"] as string;

                    if (description != null)
                    {
                        lDescription.Text = description;
                    }
                }                
            }
        }
    }
}