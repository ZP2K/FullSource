using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Windows.Forms;
using System.Reflection;
using System.IO;

using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;
using DataElementLibrary;

namespace FlowChartEditor
{
    public class DataManager
    {
        private Hashtable graphTable = new Hashtable(); // ͼԪ��ϣ��
        private Hashtable compileTable = new Hashtable(); // �������ݱ�
        private FlowChartManager flowChartManager; // ����ͼ������
        private Hashtable globeArgs = new Hashtable(); // ȫ�ֱ���
        private DataElement initializedDataElement; // ��ʼ��������Ԫ
        private string requestReloadID = ""; // ��Ҫ���±༭��ͼԪID

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="flowChartManager">����ͼ������</param>
        public DataManager(FlowChartManager flowChartManager)
        {
            this.flowChartManager = flowChartManager;
        }

        /// <summary>
        /// ��Ҫ���±༭��ͼԪID
        /// </summary>
        public string RequestReloadID
        {
            set
            {
                requestReloadID = value;
            }

            get
            {
                return requestReloadID;
            }
        }

        /// <summary>
        /// ��ʼ��������Ԫ
        /// </summary>
        public DataElement InitializedDataElement
        {
            set
            {
                initializedDataElement = value;
            }
        }

        /// <summary>
        /// ȫ�ֱ���
        /// </summary>
        public Hashtable GlobeArgs
        {
            get
            {
                return globeArgs;
            }
        }

        /// <summary>
        /// �߼�ͼԪ��ϣ��
        /// </summary>
        public Hashtable LogicGraphTable
        {
            get
            {
                return graphTable;
            }
        }

        /// <summary>
        /// ��ǰ����ͼ������
        /// </summary>
        public FlowChartManager CurrentFlowChartManager
        {
            set
            {
                flowChartManager = value;
            }

            get
            {
                return flowChartManager;
            }
        }

        /// <summary>
        /// ���·�������Ԫ��ID
        /// </summary>
        public void ResetDataElementID()
        {
            foreach (GraphElement graphElement in graphTable.Keys)
            {
                DataElement dataElement = graphTable[graphElement] as DataElement;
                dataElement.ID = graphElement.ID;
            }
        }

        /// <summary>
        /// ��ʼ���������ݱ�
        /// </summary>
        public void InitCompileTable()
        {
            compileTable = new Hashtable();

            foreach (GraphElement graphElement in graphTable.Keys)
            {
                compileTable[graphElement] = graphTable[graphElement];
            }
        }
        
        /// <summary>
        /// ��ӱ�������
        /// </summary>
        /// <param name="graphElement">ͼԪ</param>
        /// <param name="dataElement">����Ԫ</param>
        public void AddCompileData(GraphElement graphElement, DataElement dataElement)
        {
            compileTable[graphElement] = dataElement;
        }

        /// <summary>
        /// ��ȡ��������Ԫ
        /// </summary>
        /// <param name="graphElement">������ͼԪ</param>
        /// <returns>��ȡ������Ԫ</returns>
        public DataElement GetCompileDataElement(GraphElement graphElement)
        {
            DataElement dataElement = null;

            if (graphElement != null)
            {
                dataElement = compileTable[graphElement] as DataElement;
            }

            return dataElement;
        }

        /// <summary>
        /// ��ȡ��������
        /// </summary>
        /// <param name="graphElement">������ͼԪ</param>
        /// <returns>��ȡ������</returns>
        public object GetCompileData(GraphElement graphElement)
        {
            object data = null;
            DataElement dataElement = compileTable[graphElement] as DataElement;

            if (dataElement != null)
            {
                data = dataElement.Data;
            }

            return data;
        }

        /// <summary>
        /// ��ʼ������Ԫ
        /// </summary>
        /// <param name="graphElement">������ͼԪ</param>
        /// <returns>����Ԫ</returns>
        public DataElement InitDataElement(GraphElement graphElement)
        {
            // ִ���߼�����            
            LogicBaseManager logicManager = Helper.GetHelper().GetLogicManager(flowChartManager.CurrentChartMode);
            logicManager.DoLogicOperation(flowChartManager, LogicType.InitializeDataElement, graphElement);

            return initializedDataElement;
        }

        /// <summary>
        /// ��������Ԫ
        /// </summary>
        /// <param name="graphElement">������ͼԪ</param>
        /// <returns>�Ƿ����ӳɹ�</returns>
        public bool AddDataElement(GraphElement graphElement)
        {
            bool success = false;

            DataElement dataElement = InitDataElement(graphElement);

            if (dataElement != null)
            {
                dataElement.ID = graphElement.ID;
                graphTable[graphElement] = dataElement;
                success = true;
            }            

            return success;
        }

        /// <summary>
        /// ��ȡ����Ԫ
        /// </summary>
        /// <param name="graphElement">������ͼԪ</param>
        /// <returns>��ȡ������Ԫ</returns>
        public DataElement GetDataElement(GraphElement graphElement)
        {
            DataElement dataElement = null;

            if(graphElement != null)
            {
                dataElement = graphTable[graphElement] as DataElement;
            }            

            return dataElement;
        }

        /// <summary>
        /// ��ȡ����
        /// </summary>
        /// <param name="graphElement">������ͼԪ</param>
        /// <returns>��ȡ������</returns>
        public object GetData(GraphElement graphElement)
        {            
            object data = null;
            DataElement dataElement = graphTable[graphElement] as DataElement;
            
            if (dataElement != null)
            {
                data = dataElement.Data;
            }

            return data;
        }

        /// <summary>
        /// ����ͼԪ��Ӧ������
        /// </summary>
        /// <param name="graphElement">������ͼԪ</param>
        /// <param name="data">ͼԪ������</param>
        public void SetData(GraphElement graphElement, object data)
        {
            DataElement dataElement = graphTable[graphElement] as DataElement;

            if (dataElement != null)
            {
                dataElement.Text = graphElement.Text;
                dataElement.TooltipText = graphElement.TooltipText;
                dataElement.Data = data;
            }            
        }     
        
        /// <summary>
        /// ˢ��ͼԪ������
        /// </summary>
        /// <param name="graphElement">������ͼԪ</param>
        public void ReloadData(GraphElement graphElement)
        {
            Hashtable table = new Hashtable();
            table["globe_args"] = globeArgs;
            DataElement dataElement = graphTable[graphElement] as DataElement;

            try
            {
                dataElement.ReloadData(table);
            }
            catch (Exception ex)
            {
                DocumentManager.GetDocumentManager().PrintText(string.Format("ˢ��IDΪ{0}��ͼԪ����ʱ�����쳣��{1}",
                                                                             dataElement.ID, ex.Message));
            }
        }

        /// <summary>
        /// ��ȡָ��ID��ͼԪ
        /// </summary>
        /// <param name="id">ͼԪ��id</param>
        /// <returns>ͼԪ</returns>
        public GraphElement FindGraphElementByID(int id)
        {
            GraphElement graphElement = null;

            foreach (GraphElement g in graphTable.Keys)
            {
                if (g.ID == id)
                {
                    graphElement = g;
                    break;
                }
            }

            return graphElement;
        }

        /// <summary>
        /// ��ȡָ��ID������Ԫ
        /// </summary>
        /// <param name="id">����Ԫ��id</param>
        /// <returns>����Ԫ</returns>
        public DataElement FindDataElementByID(int id)
        {
            DataElement dataElement = null;

            foreach(DataElement dElement in graphTable.Values)
            {
                if (dElement.ID == id)
                {
                    dataElement = dElement;
                    break;
                }
            }

            return dataElement;
        }

        /// <summary>
        /// �༭����Ԫ
        /// </summary>
        /// <param name="graphElement">������ͼԪ</param>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public bool EditData(GraphElement graphElement)
        {
            bool success = false;

            DataElement dataElement = graphTable[graphElement] as DataElement;
            if(dataElement != null) // �ҵ���ͼԪ�󶨵�����Ԫ
            {
                // ִ���߼�����
                LogicBaseManager logicManager = Helper.GetHelper().GetLogicManager(flowChartManager.CurrentChartMode);
                success = logicManager.DoLogicOperation(flowChartManager, LogicType.Edit, graphElement);
            }
            else
            {
                MessageBox.Show("��ͼԪû�а�����Ԫ��", "����Ԫ�༭", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }            

            return success;
        }                   

        /// <summary>
        /// ɾ������Ԫ
        /// </summary>
        /// <param name="graphElement">������ͼԪ</param>
        /// <returns>�Ƿ�ɾ���ɹ�</returns>
        public bool DeleteData(GraphElement graphElement)
        {
            if (graphElement != null)
            {
                graphTable.Remove(graphElement);
            }

            return true;
        }

        /// <summary>
        /// ��ȡ����ͼ�����ݲ���
        /// </summary>
        /// <returns>����ͼ�����ݲ�����ϣ��</returns>
        public Hashtable GetArguments()
        {
            Hashtable args = new Hashtable();
            args["graphTable"] = graphTable;

            return args;
        }

        /// <summary>
        /// ����ǰ��֤��ͼ����Ч��
        /// </summary>
        public void ValidateBeforeSave()
        {
            List<GraphElement> deleteGraphElementList = new List<GraphElement>();
            List<GraphElement> validGraphElementList = flowChartManager.CurrentGraphManager.CurrentCanvas.GraphElementList;

            foreach (GraphElement graphElement in graphTable.Keys)
            {
                if (!validGraphElementList.Contains(graphElement))
                {
                    deleteGraphElementList.Add(graphElement);
                }
            }

            foreach (GraphElement graphElement in deleteGraphElementList)
            {
                DeleteData(graphElement);
            }
        }

        /// <summary>
        /// ��������ͼ�����ݲ���
        /// </summary>
        /// <param name="args">����ͼ�����ݲ�����ϣ��</param>
        public void SetArguments(Hashtable args)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            graphTable = args["graphTable"] as Hashtable;
           
            Hashtable table = new Hashtable();
            table["globe_args"] = globeArgs;
            table["requestReloadID"] = requestReloadID;
            table["forceReload"] = "0";

            foreach (GraphElement graphElement in graphTable.Keys)
            {
                DataElement dataElement = graphTable[graphElement] as DataElement;

                if (dataElement != null)
                {
                    dataElement.ReloadData(table);
                    graphElement.TooltipText = dataElement.TooltipText;
                         
                    if (graphElement.Text != dataElement.Text && !string.IsNullOrEmpty(dataElement.Text))
                    {
                        graphElement.Text = dataElement.Text;

                        // �����ı�
                        if (graphElement is SlotContainer)
                        {
                            (graphElement as SlotContainer).AdjustText();
                        }
                        else if (graphElement is ConnectorContainer)
                        {
                            (graphElement as ConnectorContainer).AdjustText();
                        }
                    }
                }
            }

            requestReloadID = table["requestReloadID"] as string;
        }        

        /// <summary>
        /// ��ȡ�����ͼԪ������
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <returns>�����ͼԪ������</returns>
        public Hashtable GetPreviousData(GraphElement graphElement)
        {
            Hashtable table = new Hashtable();

            foreach (GraphElement g in GetPreviousGraphElements(graphElement))
            {
                table[g.ID.ToString()] = GetDataElement(g);
            }

            return table;
        }

        /// <summary>
        /// ��ȡ�ֵ������ߵ�����
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <returns>�ֵ������ߵ�����</returns>
        public Hashtable GetNeighborData(GraphElement graphElement)
        {
            Hashtable table = new Hashtable();

            if(graphElement is ConnectorContainer)
            {
                ConnectorContainer line = graphElement as ConnectorContainer;
                if(line.InSlotContainer != null)
                {
                    List<GraphElement> list = GetNextGraphElements(line.InSlotContainer, false);
                    foreach (GraphElement g in list)
                    {
                        if(g != graphElement)
                        {
                            table[g.ID.ToString()] = GetDataElement(g);
                        }
                    }
                }
            }

            return table;
        }

        /// <summary>
        /// ��ȡ������ͼԪ������
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <returns>������ͼԪ������</returns>
        public Hashtable GetNextData(GraphElement graphElement)
        {
            Hashtable table = new Hashtable();

            foreach(GraphElement g in GetNextGraphElements(graphElement, false))
            {
                table[g.ID.ToString()] = GetDataElement(g);
            }

            return table;
        }

        /// <summary>
        /// ��ȡ�����ͼԪ����
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <returns>�����ͼԪ����</returns>
        public List<GraphElement> GetPreviousGraphElements(GraphElement graphElement)
        {
            List<GraphElement> list = new List<GraphElement>();

            if (graphElement is SlotContainer) // ��ǰͼԪ�ǲ������
            {
                SlotContainer slotContaienr = graphElement as SlotContainer;

                foreach (SlotGraphElement slot in slotContaienr.GetInSlotList())
                {
                    if (slot.Binded)
                    {
                        list.Add(slot.BindingConnector.Line);
                    }
                }
            }
            else if (graphElement is ConnectorContainer) // ��ǰͼԪ�������߿��Ƶ�����
            {                
                ConnectorContainer line = graphElement as ConnectorContainer;

                if (line.InSlotContainer != null)
                {                    
                    list.Add(line.InSlotContainer);                   
                }
            }

            return list;
        }

        /// <summary>
        /// �������ͼԪ�Ƿ�����
        /// </summary>
        /// <param name="fromGraphElement">��ʼͼԪ</param>
        /// <param name="toGraphElement">�ս�ͼԪ</param>
        /// <returns>����ͼԪ�Ƿ�����</returns>
        public bool IsConnect(GraphElement fromGraphElement, GraphElement toGraphElement)
        {
            List<GraphElement> list = new List<GraphElement>(); // ��¼��ǰ��������ͼԪ��������ѭ��
            IsConnect(fromGraphElement, toGraphElement, list);
            bool result = list.Contains(toGraphElement);

            return result;
        }

        /// <summary>
        /// �������ͼԪ�Ƿ����ӵĸ�������
        /// </summary>
        /// <param name="fromGraphElement">��ʼͼԪ</param>
        /// <param name="toGraphElement">�ս�ͼԪ</param>
        /// <param name="list">��ǰ��������ͼԪ����</param>
        private void IsConnect(GraphElement fromGraphElement, GraphElement toGraphElement, List<GraphElement> list)
        {
            if (fromGraphElement == toGraphElement)
            {
                list.Add(toGraphElement);
            }
            else
            {
                if (!list.Contains(fromGraphElement))
                {
                    list.Add(fromGraphElement);

                    foreach (GraphElement graphElement in GetNextGraphElements(fromGraphElement, false))
                    {
                        IsConnect(graphElement, toGraphElement, list);
                    }
                }
            }
        }

        /// <summary>
        /// ��ȡ������ͼԪ����
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="hasInnerChart">�Ƿ����ӻ�ͼ</param>
        /// <returns>������ͼԪ����</returns>
        public List<GraphElement> GetNextGraphElements(GraphElement graphElement, bool dealInnerChart)
        {
            List<GraphElement> list = new List<GraphElement>();

            if (graphElement is SlotContainer) // ��ǰͼԪ�ǲ������
            {
                SlotContainer slotContainer = graphElement as SlotContainer;

                foreach (SlotGraphElement slot in slotContainer.GetOutSlotList())
                {
                    if (slot.Binded)
                    {
                        list.Add(slot.BindingConnector.Line);
                    }
                }
            }
            else if (graphElement is ConnectorContainer) // ��ǰͼԪ�������߿��Ƶ�����
            {
                ConnectorContainer line = graphElement as ConnectorContainer;

                if (line.OutSlotContainer != null)
                {
                    if (dealInnerChart)
                    {
                        SlotContainer slotContainer = line.OutSlotContainer;

                        if (slotContainer is InnerChart)
                        {
                            SlotGraphElement slot = slotContainer.GetBindedInSlot(line);
                            InnerChartEditor innerChartEditor = GetDataElement(slotContainer) as InnerChartEditor;

                            if (slot != null)
                            {
                                InterfaceGraphElement interfaceNode = innerChartEditor.GetInterfaceNode(slot.Tag.ToString());

                                if (interfaceNode != null)
                                {
                                    GraphElement connectedGraphElement = interfaceNode.GetConnectedOutGraphElement();

                                    if (connectedGraphElement != null)
                                    {
                                        list.Add(connectedGraphElement);
                                    }
                                }
                            }
                        }
                        else if (slotContainer is InterfaceGraphElement)
                        {
                            InnerChart innerChart = FindGraphElementByID(int.Parse(slotContainer.Tag as string)) as InnerChart;

                            foreach (SlotGraphElement slot in innerChart.GetOutSlotList())
                            {
                                if (slot.Binded)
                                {
                                    ConnectorContainer connectedLine = slot.BindingConnector.Line;
                                    string lineData = GetData(connectedLine) as string;

                                    if (!string.IsNullOrEmpty(lineData))
                                    {
                                        string[] dataArray = lineData.Split(new char[] { ':' }, StringSplitOptions.RemoveEmptyEntries);

                                        if (dataArray[0] == slotContainer.ID.ToString())
                                        {
                                            list.AddRange(GetNextGraphElements(slot.BindingConnector.Line, true));
                                            break;
                                        }
                                    }

                                    /*
                                    if (!string.IsNullOrEmpty(connectedLine.Text))
                                    {
                                        string[] dataArray = connectedLine.Text.Split(new char[] { ':' }, StringSplitOptions.RemoveEmptyEntries);

                                        if (dataArray[0] == slotContainer.ID.ToString())
                                        {
                                            list.AddRange(GetNextGraphElements(slot.BindingConnector.Line, true));
                                            break;
                                        }
                                    }
                                    */ 
                                }
                            }

                            /*
                            foreach (SlotGraphElement slot in innerChart.GetOutSlotList())
                            {
                                if (slot.Tag.ToString() == slotContainer.ID.ToString())
                                {
                                    slotGraphElement = slot;
                                    break;
                                }
                            }

                            if (slotGraphElement != null)
                            {
                                if (slotGraphElement.Binded)
                                {
                                    list.AddRange(GetNextGraphElements(slotGraphElement.BindingConnector.Line, true));
                                }
                            }
                            */
                        }
                        else
                        {
                            list.Add(slotContainer);
                        }
                    }
                    else
                    {
                        list.Add(line.OutSlotContainer);
                    }                    
                }
            }            

            return list;
        }

        /// <summary>
        /// ��ȡ���������в������������
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        /// <returns>���������в������������</returns>
        public List<SlotContainer> GetAllNextSlotContainers(SlotContainer slotContainer)
        {
            List<SlotContainer> list = new List<SlotContainer>();

            GetAllNextSlotContainers(slotContainer, list);

            return list;
        }

        /// <summary>
        /// ��ȡ��������в������������
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        /// <returns>��������в������������</returns>
        public List<SlotContainer> GetAllPreviousSlotContainers(SlotContainer slotContainer)
        {
            List<SlotContainer> list = new List<SlotContainer>();

            GetAllPreviousSlotContainers(slotContainer, list);

            return list;
        }

        /// <summary>
        /// ��ȡ���������в����������������������
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        /// <param name="list">��ǰ���������в������������</param>
        public void GetAllNextSlotContainers(SlotContainer slotContainer, List<SlotContainer> list)
        {
            if(!list.Contains(slotContainer)) // ���������ѭ��
            {
                list.Add(slotContainer);

                foreach (SlotGraphElement slot in slotContainer.GetOutSlotList())
                {
                    if (slot.Binded && slot.BindingConnector.Line.OutSlotContainer != null)
                    {
                        GetAllNextSlotContainers(slot.BindingConnector.Line.OutSlotContainer, list);
                    }
                }
            }
        }

        /// <summary>
        /// ��ȡ��������в����������������������
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        /// <param name="list">��ǰ���������в������������</param>
        public void GetAllPreviousSlotContainers(SlotContainer slotContainer, List<SlotContainer> list)
        {
            if(!list.Contains(slotContainer)) // ���������ѭ��
            {
                list.Add(slotContainer);

                foreach(SlotGraphElement slot in slotContainer.GetInSlotList())
                {
                    if(slot.Binded && slot.BindingConnector.Line.InSlotContainer != null)
                    {
                        GetAllPreviousSlotContainers(slot.BindingConnector.Line.InSlotContainer, list);
                    }
                }
            }
        }

        /// <summary>
        /// ��ȡ�����Ĳ����������
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        /// <returns>�����Ĳ����������</returns>
        public List<SlotContainer> GetNextSlotContainers(SlotContainer slotContainer)
        {
            List<SlotContainer> list = new List<SlotContainer>();

            foreach(SlotGraphElement slot in slotContainer.GetOutSlotList())
            {
                if(slot.Binded && slot.BindingConnector.Line.OutSlotContainer != null
                    && !list.Contains(slot.BindingConnector.Line.OutSlotContainer))
                {
                    list.Add(slot.BindingConnector.Line.OutSlotContainer);
                }
            }

            return list;
        }

        /// <summary>
        /// ��ȡ���������Ӧ���¼���������
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        /// <returns>�¼���������</returns>
        public object GetEventData(SlotContainer slotContainer)
        {
            object data = null;

            List<DataElement> eventList = GetEventList(slotContainer);
            if(eventList.Count > 0)
            {
                data = eventList[0].Data;
            }

            return data;
        }

        /// <summary>
        /// ��ȡ���������Ӧ���¼������������
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        /// <returns>���������Ӧ���¼������������</returns>
        public List<DataElement> GetEventList(SlotContainer slotContainer)
        {
            List<DataElement> list = new List<DataElement>();
            List<SlotContainer> slotContainerList = GetAllPreviousSlotContainers(slotContainer);
            foreach(SlotContainer s in slotContainerList)
            {
                if(s is EventGraphElement)
                {
                    DataElement dataElement = graphTable[s] as DataElement;
                    if(dataElement != null)
                    {
                        list.Add(dataElement);
                    }
                }
            }

            return list;
        }

        /// <summary>
        /// ��ȡ�����߶�Ӧ���¼������������
        /// </summary>
        /// <param name="connectorContainer">��ǰ������</param>
        /// <returns>�����߶�Ӧ���¼������������</returns>
        public List<DataElement> GetEventList(ConnectorContainer line)
        {
            List<DataElement> list = new List<DataElement>();
            SlotContainer slotContainer = line.InSlotContainer;

            if(slotContainer != null)
            {
                List<SlotContainer> slotContainerList = GetAllPreviousSlotContainers(slotContainer);
                foreach (SlotContainer s in slotContainerList)
                {
                    if (s is EventGraphElement)
                    {
                        DataElement dataElement = graphTable[s] as DataElement;
                        if (dataElement != null)
                        {
                            list.Add(dataElement);
                        }
                    }
                }
            }

            return list;
        }

        /// <summary>
        /// ����Ƿ��������¼����
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        /// <returns>�Ƿ��������¼����</returns>
        public bool IsConnectEventNode(SlotContainer slotContainer)
        {
            bool result = false;
            
            if (slotContainer.RequireEvent) // ��Ҫ����¼�
            {
                if (GetEventList(slotContainer).Count > 0)
                {
                    result = true;
                }
            }
            else
            {
                result = true;
            }                                    

            return result;
        }

        /// <summary>
        /// ����Ƿ��������¼����
        /// </summary>
        /// <param name="line">��ǰ������</param>
        /// <returns>�Ƿ��������¼����</returns>
        public bool IsConnectEventNode(ConnectorContainer line)
        {
            bool result = false;

            if (line.RequireEvent) // ��Ҫ����¼�
            {
                if (GetEventList(line).Count > 0)
                {
                    result = true;
                }
            }
            else
            {
                result = true;
            }            

            return result;
        }
    }
}
