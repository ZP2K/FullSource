using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Windows.Forms;
using GraphElementLibrary;

namespace FlowChartEditor
{
    class FlowChartCommand:Command
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartCommand(object data, string description):
            base(data, description)
        {
            dataBeforeExecute = new SerialMemento();
            dataAfterExecute = new SerialMemento();
        }

        /// <summary>
        /// ɾ��ͼԪ
        /// </summary>
        /// <param name="graphElement">Ҫɾ����ͼԪ</param>
        /// <return>�Ƿ�ɾ���ɹ�</return>
        protected bool DeleteGraphElement(GraphElement graphElement)
        {
            GraphManager graphManager = data as GraphManager;
            DataManager dataManager = graphManager.CurrentFlowChartManager.CurrentDataManager;
            List<GraphElement> list = new List<GraphElement>(); // ��¼������ͼԪ������
            bool deleteSuccess = false; // �Ƿ�ɾ���ɹ�

            if (graphElement is SlotContainer) // Ҫɾ����ͼԪ�ǲ������
            {
                SlotContainer slotContainer = graphElement as SlotContainer;
                ReBindEventNode(list, slotContainer, null, false); // �����趨�¼���㼰ͼԪ�Ƿ����

                foreach (SlotGraphElement slot in slotContainer.GetSlotList())
                {
                    if (slot.Binded) // ����������߿��Ƶ�
                    {
                        if (slot.IsOutSlot && slotContainer is ConditionGraphElement) // �����������������Ҫ������Ч�Լ��
                        {
                            ConnectorContainer line = slot.BindingConnector.Line;
                            if (line.Text == "��")
                            {
                                line.Invalid = false;
                                line.ShowText = false;
                            }
                        }
                        slot.UnBind();
                    }
                }

                graphManager.SlotContainerList.Remove(slotContainer); // �Ӳ����������������ɾ��
                deleteSuccess = true;
            }
            else if (graphElement is ConnectorContainer) // Ҫɾ����ͼԪ�������߿��Ƶ�����
            {
                ConnectorContainer connectorContainer = graphElement as ConnectorContainer;

                SlotContainer inSlotContainer = connectorContainer.InSlotContainer;
                if (inSlotContainer != null)
                {
                    connectorContainer.UnbindInSlotContainer();
                }

                ReBindEventNode(list, connectorContainer, null, false); // �����趨�¼���㼰ͼԪ�Ƿ����

                SlotContainer outSlotContainer = connectorContainer.OutSlotContainer;
                if (outSlotContainer != null)
                {
                    connectorContainer.UnbindOutSlotContainer();
                }

                if (inSlotContainer is ConditionGraphElement) // �����������������Ҫ������Ч�Լ��
                {
                    dataManager.CheckLineValid(inSlotContainer);
                }

                graphManager.ConnectorContainerList.Remove(connectorContainer); // �������߿��Ƶ�����������ɾ��
                deleteSuccess = true;
            }
            else if (graphElement is ConnectorGraphElement) // Ҫɾ����ͼԪ�������߿��Ƶ�
            {
                ConnectorGraphElement connector = graphElement as ConnectorGraphElement;
                if (connector.IsHeadPoint || connector.IsTailPoint) // ������ɾ��ͷ����β���
                {
                    MessageBox.Show("����ɾ��ͷ�����߿��Ƶ��β�����߿��Ƶ㣡", "ͼԪɾ��", MessageBoxButtons.OK, MessageBoxIcon.Information);

                }
                else
                {
                    connector.Line.RemoveConnector(connector);
                    deleteSuccess = true;
                }
            }
            else if (graphElement is SlotGraphElement) // Ҫɾ����ͼԪ�ǲ��
            {
                SlotGraphElement slot = graphElement as SlotGraphElement;

                if (slot.CanDelete && (slot.IsInSlot || slot.SlotContainer.OutSlotCount > 1)) // ��ۿ���ɾ��
                {
                    SlotContainer slotContainer = slot.SlotContainer;
                    slotContainer.RemoveSlot(slot);                    
                    deleteSuccess = true;
                }
                else
                {
                    MessageBox.Show("�ò�۲���ɾ��", "ͼԪɾ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else if (graphElement is RemarkGraphElement) // Ҫɾ����ͼԪ��ע�ͽ��
            {
                RemarkGraphElement remarkGraphElement = graphElement as RemarkGraphElement;
                SlotContainer slotContainer = remarkGraphElement.Owner as SlotContainer;

                slotContainer.RemarkNode = null;
                remarkGraphElement.Owner = null;
                remarkGraphElement = null;
                deleteSuccess = true;
            }

            if (deleteSuccess) // �ͷ�ͼԪ��ռ����Դ
            {
                dataManager.DeleteData(graphElement);
                graphElement.Release();
            }

            return deleteSuccess;
        }

        /// <summary>
        /// ����ͼԪ������Ϣ
        /// </summary>
        /// <param name="dataTable">����ͼԪ��Ϣ�Ĺ�ϣ��</param>
        /// <returns>���Ƶ�����</returns>
        protected Hashtable CopyGraphElement(Hashtable dataTable)
        {
            GraphManager graphManager = data as GraphManager;
            FlowChartManager flowChartManager = graphManager.CurrentFlowChartManager;
            DataManager dataManager = flowChartManager.CurrentDataManager;
            Hashtable copyTable = new Hashtable();
            Hashtable graphTable = new Hashtable();
            List<ConnectorContainer> lineList = new List<ConnectorContainer>();
            List<GraphElement> graphElementList = new List<GraphElement>();

            if (dataTable.Keys.Count > 1) // ���ƶ��ͼԪ
            {
                SlotContainer slotContainer;
                SlotContainer newSlotContainer;
                ConnectorContainer line;
                ConnectorContainer newLine;
                SlotGraphElement slot;
                ICloneable cloneData;
                EventGraphElement newEventNode;
                List<EventGraphElement> eventList = new List<EventGraphElement>();

                foreach (object o in dataTable.Keys)
                {
                    GraphElement graphElement = o as GraphElement;

                    if (graphElement is SlotContainer) // �������
                    {
                        slotContainer = graphElement as SlotContainer;
                        newSlotContainer = slotContainer.Clone();
                        graphTable[slotContainer] = newSlotContainer;
                        cloneData = dataTable[slotContainer] as ICloneable;

                        if (slotContainer is EventGraphElement)
                        {
                            eventList.Add(slotContainer as EventGraphElement);
                        }

                        if (cloneData != null)
                        {
                            copyTable[newSlotContainer] = cloneData.Clone();
                        }
                        else
                        {
                            copyTable[newSlotContainer] = null;
                        }

                        graphElementList.Add(slotContainer);
                    }
                    else if (graphElement is ConnectorContainer) // ������
                    {
                        line = graphElement as ConnectorContainer;
                        newLine = line.Clone();
                        graphTable[line] = newLine;
                        lineList.Add(line);
                        cloneData = dataTable[line] as ICloneable;

                        if (cloneData != null)
                        {
                            copyTable[newLine] = cloneData.Clone();
                        }
                        else
                        {
                            copyTable[newLine] = null;
                        }

                        graphElementList.Add(line);
                    }
                }

                // ���¹�������ͼԪ�����ӹ�ϵ
                foreach (ConnectorContainer connectorContainer in lineList)
                {
                    if (connectorContainer.InSlotContainer != null
                        && graphElementList.Contains(connectorContainer.InSlotContainer))
                    {
                        newSlotContainer = graphTable[connectorContainer.InSlotContainer] as SlotContainer;
                        newLine = graphTable[connectorContainer] as ConnectorContainer;

                        slot = newSlotContainer.GetOutSlot();
                        slot.Bind(newLine.TailConnector);
                    }

                    if (connectorContainer.OutSlotContainer != null
                        && graphElementList.Contains(connectorContainer.OutSlotContainer))
                    {
                        newSlotContainer = graphTable[connectorContainer.OutSlotContainer] as SlotContainer;
                        newLine = graphTable[connectorContainer] as ConnectorContainer;

                        slot = newSlotContainer.GetInSlot();
                        slot.Bind(newLine.HeadConnector);
                    }
                }

                // ����ͼԪ��Ӧ���¼����
                if (eventList.Count > 0)
                {
                    foreach (object o in dataTable.Keys)
                    {
                        GraphElement graphElement = o as GraphElement;

                        if (graphElement is SlotContainer) // �������
                        {
                            slotContainer = graphElement as SlotContainer;
                            if (slotContainer.EventNode != null)
                            {
                                newSlotContainer = graphTable[slotContainer] as SlotContainer;

                                if (eventList.Contains(slotContainer.EventNode))
                                {
                                    newEventNode = graphTable[slotContainer.EventNode] as EventGraphElement;
                                    newSlotContainer.EventNode = newEventNode;
                                }
                                else
                                {
                                    newSlotContainer.EventNode = slotContainer.EventNode;
                                }
                            }
                        }
                        else if (graphElement is ConnectorContainer) // ������
                        {
                            line = graphElement as ConnectorContainer;
                            if (line.EventNode != null)
                            {
                                newLine = graphTable[line] as ConnectorContainer;

                                if (eventList.Contains(line.EventNode))
                                {
                                    newEventNode = graphTable[line.EventNode] as EventGraphElement;
                                    newLine.EventNode = newEventNode;
                                }
                                else
                                {
                                    newLine.EventNode = line.EventNode;
                                }
                            }
                        }
                    }
                }
            }
            else // ���Ƶ���ͼԪ
            {
                GraphElement graphElement = null;
                foreach(object o in dataTable.Keys)
                {
                    graphElement = o as GraphElement;
                }               
                ICloneable cloneData;

                if (graphElement is SlotContainer) // �������
                {
                    SlotContainer slotContainer = graphElement as SlotContainer;
                    SlotContainer newSlotContainer = slotContainer.Clone();
                    cloneData = dataTable[slotContainer] as ICloneable;

                    if (cloneData != null)
                    {
                        copyTable[newSlotContainer] = cloneData.Clone();
                    }
                    else
                    {
                        copyTable[newSlotContainer] = null;
                    }
                }
                else if (graphElement is ConnectorContainer) // ������
                {
                    ConnectorContainer line = graphElement as ConnectorContainer;
                    ConnectorContainer newLine = line.Clone();
                    cloneData = dataTable[line] as ICloneable;

                    if (cloneData != null)
                    {
                        copyTable[newLine] = cloneData.Clone();
                    }
                    else
                    {
                        copyTable[newLine] = null;
                    }
                }
            }

            return copyTable;
        }

        /// <summary>
        /// ����Ƿ�����¼����
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="eventNode">�¼����</param>
        /// <returns>�Ƿ���Ҫ����</returns>
        protected virtual bool CheckCanBindEventNode(GraphElement graphElement, EventGraphElement eventNode)
        {
            bool avail = true;
            GraphManager graphManager = data as GraphManager;
            DataManager dataManager = graphManager.CurrentFlowChartManager.CurrentDataManager;
            IComparable com1;
            IComparable com2;

            if(graphElement is SlotContainer) // �������
            {
                SlotContainer slotContainer = graphElement as SlotContainer;
                if(slotContainer.EventNode != null && slotContainer.EventNode != eventNode)
                {
                    com1 = dataManager.GetData(slotContainer.EventNode) as IComparable;
                    com2 = dataManager.GetData(eventNode) as IComparable;
                    avail = (com1.CompareTo(com2) == 0);
                }
            }
            else if(graphElement is ConnectorContainer) // ������
            {
                ConnectorContainer line = graphElement as ConnectorContainer;
                if(line.EventNode != null && line.EventNode != eventNode)
                {
                    com1 = dataManager.GetData(line.EventNode) as IComparable;
                    com2 = dataManager.GetData(eventNode) as IComparable;
                    avail = (com1.CompareTo(com2) == 0);
                }
            }

            return avail;
        }

        /// <summary>
        /// ����ͼԪ�Ƿ����
        /// </summary>
        /// <param name="list">��������ͼԪ����</param>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="eventNode">�¼����</param>
        /// <param name="enable">ͼԪ�Ƿ����</param>
        protected virtual void ReBindEventNode(List<GraphElement> list, GraphElement graphElement, EventGraphElement eventNode, bool enable)
        {
            if(!list.Contains(graphElement)) // ��û�б�������ǰͼԪ
            {
                GraphManager graphManager = data as GraphManager;
                DataManager dataManager = graphManager.CurrentFlowChartManager.CurrentDataManager;               

                if(graphElement is SlotContainer) // ��ͼԪ�ǲ������
                {
                    SlotContainer slotContainer = graphElement as SlotContainer;

                    if(!enable && slotContainer.EventNode != null && dataManager.IsConnect(slotContainer.EventNode, slotContainer)) // ����Ҫ���µ�ǰͼԪ���¼����
                    {

                    }
                    else
                    {
                        list.Add(slotContainer);
                        slotContainer.Enable = enable;
                        slotContainer.EventNode = eventNode;

                        foreach (GraphElement g in dataManager.GetNextGraphElements(graphElement))
                        {
                            ReBindEventNode(list, g, eventNode, enable);
                        }
                    }
                }
                else if(graphElement is ConnectorContainer) // ��ͼԪ�������߿��Ƶ�����
                {
                    ConnectorContainer line = graphElement as ConnectorContainer;

                    if(!enable && line.EventNode != null && dataManager.IsConnect(line.EventNode, line)) // ����Ҫ���µ�ǰͼԪ���¼����
                    {                            
                        
                    } 
                    else
                    {
                        list.Add(line);
                        line.Enable = enable;
                        line.EventNode = eventNode;

                        foreach (GraphElement g in dataManager.GetNextGraphElements(graphElement))
                        {
                            ReBindEventNode(list, g, eventNode, enable);
                        }
                    }
                }                
            }
        }

        /// <summary>
        /// ������ִ��ǰ�����������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        protected override void SaveBeforeExecute(object o)
        {
            dataBeforeExecute.SaveData(o);
        }

        /// <summary>
        /// ������ִ�к󱣴��������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        protected override void SaveAfterExecute(object o)
        {
            dataAfterExecute.SaveData(o);
        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        /// <returns>�Ƿ�ִ�гɹ�</returns>
        public override bool Execute(object o)
        {
            return false;
        }

        /// <summary>
        /// ִ������
        /// </summary>
        public override void Execute()
        {
            object o = dataAfterExecute.LoadData();
            GraphManager graphManager = data as GraphManager;
            FlowChartManager flowChartManager = graphManager.CurrentFlowChartManager;
            object[] args = o as object[];
            flowChartManager.SetArguments(args);
        }

        /// <summary>
        /// ��������
        /// </summary>
        public override void Unexecute()
        {
            object o = dataBeforeExecute.LoadData();
            GraphManager graphManager = data as GraphManager;
            FlowChartManager flowChartManager = graphManager.CurrentFlowChartManager;
            object[] args = o as object[];
            flowChartManager.SetArguments(args);
        }
    }
}
