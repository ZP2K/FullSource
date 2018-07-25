using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Windows.Forms;

using DataElementLibrary;
using GraphElementLibrary;
using FlowChartEditor.BaseGraphElement;
using DBManager;

namespace FlowChartEditor
{
    [Serializable]
    public class InnerChartEditor : DataElement
    {
        private string innerChartPath = "";

        [NonSerialized]
        private List<GraphElement> graphElementList; // ͼԪ����

        [NonSerialized]
        private Hashtable interfaceNodeTable; // �ӿڽ�����ݱ�

        [NonSerialized]
        private Hashtable graphTable; // ͼԪ���ݱ�

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
        /// ��ȡ�ӿڽ��
        /// </summary>
        /// <param name="interfaceID">���ID</param>
        /// <returns>�ӿڽ��</returns>
        public InterfaceGraphElement GetInterfaceNode(string interfaceID)
        {
            InterfaceGraphElement interfaceGraphElement = null;

            if (interfaceNodeTable != null)
            {
                interfaceGraphElement = interfaceNodeTable[interfaceID] as InterfaceGraphElement;
            }

            return interfaceGraphElement;
        }

        /// <summary>
        /// ��ȡ����Ԫ
        /// </summary>
        /// <param name="graphElement">ͼԪ</param>
        /// <returns>����Ԫ</returns>
        public DataElement GetDataElement(GraphElement graphElement)
        {
            DataElement dataElement = null;

            if (graphTable != null)
            {
                dataElement = graphTable[graphElement] as DataElement;
            }

            return dataElement;
        }

        /// <summary>
        /// �༭����
        /// </summary>
        /// <param name="o">����Ԫ������</param>
        /// <return>�Ƿ�༭�ɹ�</return>
        public override bool EditData(Hashtable table)
        {
            bool editSuccess = false;

            InnerChartForm innerChartForm = new InnerChartForm(innerChartPath);
            
            if (innerChartForm.ShowDialog() == DialogResult.OK)
            {                
                innerChartPath = innerChartForm.InnerChartPath;
                graphElementList = innerChartForm.GraphElementList;
                graphTable = innerChartForm.GraphTable;
                interfaceNodeTable = innerChartForm.InterfaceNodeTable;
                this.text = innerChartPath;      
          
                // ��������
                UpdateData(innerChartForm.InSlotInfo, innerChartForm.OutSlotInfo);
                
                InnerChart innerChart = DocumentManager.GetDocumentManager().CurrentFlowChartManager.CurrentDataManager.FindGraphElementByID(id) as InnerChart;

                // ���²������
                UpdateSlotCount(innerChart, innerChartForm.InSlotCount, innerChartForm.OutSlotCount, false);

                List<int> inIDList = new List<int>();
                List<int> outIDList = new List<int>();

                foreach (string nodeID in innerChartForm.InSlotInfo.Keys)
                {                  
                    inIDList.Add(int.Parse(nodeID));
                }            

                foreach (string nodeID in innerChartForm.OutSlotInfo.Keys)
                {                    
                    outIDList.Add(int.Parse(nodeID));                    
                }            
    
                foreach (InterfaceGraphElement interfaceNode in interfaceNodeTable.Values)
                {
                    interfaceNode.Tag = id.ToString();
                }

                // ˢ�²����Ϣ
                RefreshSlotInfo(innerChart, inIDList, outIDList, innerChartForm.InSlotInfo, innerChartForm.OutSlotInfo);                

                editSuccess = true;
            }

            return editSuccess;
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="inSlotInfo">��ڲ������</param>
        /// <param name="outSlotInfo">���ڲ������</param>
        private void UpdateData(Hashtable inSlotInfo, Hashtable outSlotInfo)
        {
            Hashtable infoTable = new Hashtable();
            infoTable["inSlotInfo"] = inSlotInfo;
            infoTable["outSlotInfo"] = outSlotInfo;
            infoTable["innerChartPath"] = innerChartPath;
            data = infoTable;
        }

        /// <summary>
        /// ���²������
        /// </summary>
        /// <param name="innerChart">�ӻ�ͼ���</param>
        /// <param name="inSlotCount">��ڲ������</param>
        /// <param name="outSlotCount">���ڲ������</param>
        /// <param name="showMessage">�Ƿ���ʾ��Ϣ</param>
        private void UpdateSlotCount(InnerChart innerChart, int inSlotCount, int outSlotCount, bool showMessage)
        {
            // ���²����Ϣ
            if (innerChart.InSlotCount != inSlotCount || innerChart.OutSlotCount != outSlotCount)
            {
                innerChart.InSlotCount = inSlotCount;
                innerChart.OutSlotCount = outSlotCount;
                innerChart.LockInSlot();
                innerChart.LockOutSlot();

                if (showMessage)
                {
                    MessageBox.Show(string.Format("�ӻ�ͼ���{0} (ID:{1})�Ĳ�۲������£���Ҫ�������ӣ�", innerChart.Text, innerChart.ID), 
                                    "�ӻ�ͼ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        /// <summary>
        /// ˢ�²����Ϣ
        /// </summary>
        /// <param name="innerChart">�ӻ�ͼ���</param>
        /// <param name="inIDList">��ڲ��ID����</param>
        /// <param name="outIDList">���ڲ��ID����</param>
        /// <param name="inSlotInfo">��ڲ����Ϣ</param>
        /// <param name="outSlotInfo">���ڲ����Ϣ</param>
        private void RefreshSlotInfo(InnerChart innerChart, List<int> inIDList, List<int> outIDList, Hashtable inSlotInfo, Hashtable outSlotInfo)
        {
            StringBuilder descriptionInfo = new StringBuilder();

            descriptionInfo.AppendLine("<bold>��ڲ��");            

            // ˢ�²����Ϣ
            inIDList.Sort();
            List<SlotGraphElement> slotList = innerChart.GetInSlotList();

            for (int i = 0; i < inIDList.Count; i++)
            {
                string nodeID = inIDList[i].ToString();
                string nodeName = inSlotInfo[nodeID] as string;
                slotList[i].Tag = nodeID;
                slotList[i].TooltipText = nodeName;

                descriptionInfo.AppendLine(string.Format("{0}��{1}", i, nodeName));                
            }

            outIDList.Sort();
            slotList = innerChart.GetOutSlotList();

            descriptionInfo.AppendLine("<split>");
            descriptionInfo.AppendLine("<bold>���ڲ��");            

            for (int i = 0; i < outIDList.Count; i++)
            {
                string nodeID = outIDList[i].ToString();
                string nodeName = outSlotInfo[nodeID] as string;
                slotList[i].Tag = nodeID;

                descriptionInfo.AppendLine(string.Format("{0}��{1}", i, nodeName));                
            }

            // ˢ����ʾ��Ϣ
            tooltipText = descriptionInfo.ToString();
        }

        /// <summary>
        /// ��ʼ���ӻ�ͼ���ݱ�
        /// </summary>        
        private void InitGraphTable()
        {
            if (!string.IsNullOrEmpty(innerChartPath))
            {
                DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
                string fullPath = string.Format(@"�ӻ�ͼ\{0}", innerChartPath);
                Hashtable infoTable = dataBaseManager.GetDiagramDataByPath(fullPath, "AIͼ") as Hashtable;

                if (infoTable != null)
                {
                    Hashtable logicData = infoTable["logicData"] as Hashtable;
                    graphTable = logicData["graphTable"] as Hashtable;
                }     
            }                   
        }

        /// <summary>
        /// �ض�����
        /// </summary>
        /// <param name="table">��ǰ����</param>
        /// <return>�Ƿ��ض��ɹ�</return>
        public override bool ReloadData(Hashtable table)
        {            
            Hashtable dataInfo = data as Hashtable;

            if (dataInfo != null)
            {
                innerChartPath = dataInfo["innerChartPath"] as string;

                // ��ʼ����ͼ���ݱ�
                InitGraphTable();

                if (graphTable != null)
                {
                    int inSlotCount = 0;
                    int outSlotCount = 0;
                    Hashtable inSlotInfo = new Hashtable();
                    Hashtable outSlotInfo = new Hashtable();
                    graphElementList = new List<GraphElement>();
                    interfaceNodeTable = new Hashtable();

                    List<int> inIDList = new List<int>();
                    List<int> outIDList = new List<int>();

                    foreach (GraphElement graphElement in graphTable.Keys)
                    {
                        if (graphElement is InterfaceGraphElement)
                        {
                            InterfaceGraphElement interfaceGraphElement = graphElement as InterfaceGraphElement;
                            InterfaceNodeEditor interfaceEditor = graphTable[graphElement] as InterfaceNodeEditor;

                            if (interfaceEditor.IsInput)
                            {                                
                                inSlotInfo[interfaceEditor.ID.ToString()] = interfaceEditor.InterfaceName;
                                inIDList.Add(interfaceGraphElement.ID);
                                inSlotCount++;
                            }
                            else
                            {
                                outSlotInfo[interfaceEditor.ID.ToString()] = interfaceEditor.InterfaceName;
                                outIDList.Add(interfaceGraphElement.ID);
                                outSlotCount++;
                            }

                            interfaceGraphElement.Tag = id.ToString();
                            interfaceNodeTable[interfaceEditor.ID.ToString()] = graphElement;
                        }
                        else
                        {
                            graphElementList.Add(graphElement);
                        }
                    }                    

                    InnerChart innerChart = DocumentManager.GetDocumentManager().CurrentFlowChartManager.CurrentDataManager.FindGraphElementByID(id) as InnerChart;
                    
                    // ��������
                    UpdateData(inSlotInfo, outSlotInfo);

                    // ���²������
                    UpdateSlotCount(innerChart, inSlotCount, outSlotCount, true);

                    // ���²����Ϣ
                    RefreshSlotInfo(innerChart, inIDList, outIDList, inSlotInfo, outSlotInfo);
                }
            }                                       

            return true;
        }

        /// <summary>
        /// ��ȡ����Ĳ���
        /// </summary>
        /// <returns>����Ĳ���</returns>
        public override Hashtable GetOutputArgs()
        {
            return null;
        }
    }
}
