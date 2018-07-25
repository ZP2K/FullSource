using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Windows.Forms;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartBaseCommand:Command
    {
        protected DocumentManager documentManager; // �ĵ�������ʵ��
        protected GraphManager graphManager; // ��ͼ������ʵ��
        protected DataManager dataManager; // ���ݹ�����ʵ��
        protected FlowChartManager flowChartManager; // ����ͼ������ʵ��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartBaseCommand(object data, string description):
            base(data, description)
        {
            graphManager = data as GraphManager;
            flowChartManager = graphManager.CurrentFlowChartManager;
            dataManager = flowChartManager.CurrentDataManager;
            documentManager = DocumentManager.GetDocumentManager();

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
            Helper helper = Helper.GetHelper();
            GraphManager graphManager = data as GraphManager;
            FlowChartManager flowChartManager = graphManager.CurrentFlowChartManager;
            DataManager dataManager = flowChartManager.CurrentDataManager;            
            bool deleteSuccess = false; // �Ƿ�ɾ���ɹ�            

            // ִ���߼�����
            LogicBaseManager logicManager = helper.GetLogicManager(flowChartManager.CurrentChartMode);            
            logicManager.DoLogicOperation(flowChartManager, LogicType.BeforeDelete, graphElement); 

            if (graphElement is SlotContainer) // Ҫɾ����ͼԪ�ǲ������
            {
                SlotContainer slotContainer = graphElement as SlotContainer;
                List<GraphElement> graphElementList = new List<GraphElement>();

                // ����������߿��Ƶ�                
                foreach (ConnectorContainer line in slotContainer.GetConnectedLine())
                {
                    graphElementList.Add(line);
                }

                slotContainer.UnBind();

                // ִ���߼�����
                logicManager.DoLogicOperation(flowChartManager, LogicType.AfterDelete, graphElementList);

                graphManager.SlotContainerList.Remove(slotContainer); // �Ӳ����������������ɾ��
                deleteSuccess = true;
            }
            else if (graphElement is ConnectorContainer) // Ҫɾ����ͼԪ�������߿��Ƶ�����
            {
                ConnectorContainer connectorContainer = graphElement as ConnectorContainer;
                List<GraphElement> graphElementList = new List<GraphElement>();

                SlotContainer inSlotContainer = connectorContainer.InSlotContainer;
                if (inSlotContainer != null)
                {
                    connectorContainer.UnbindInSlotContainer();
                }              

                SlotContainer outSlotContainer = connectorContainer.OutSlotContainer;
                if (outSlotContainer != null)
                {
                    graphElementList.Add(outSlotContainer);
                    connectorContainer.UnbindOutSlotContainer();
                }

                // ִ���߼�����
                logicManager.DoLogicOperation(flowChartManager, LogicType.AfterDelete, graphElementList);

                graphManager.ConnectorContainerList.Remove(connectorContainer); // �������߿��Ƶ�����������ɾ��
                deleteSuccess = true;
            }
            else if (graphElement is ConnectorGraphElement) // Ҫɾ����ͼԪ�������߿��Ƶ�
            {
                ConnectorGraphElement connector = graphElement as ConnectorGraphElement;
                ConnectorContainer connectorContainer = connector.Line;
                List<GraphElement> graphElementList = new List<GraphElement>();

                SlotContainer inSlotContainer = connectorContainer.InSlotContainer;
                if (inSlotContainer != null)
                {
                    connectorContainer.UnbindInSlotContainer();
                }

                SlotContainer outSlotContainer = connectorContainer.OutSlotContainer;
                if (outSlotContainer != null)
                {
                    graphElementList.Add(outSlotContainer);
                    connectorContainer.UnbindOutSlotContainer();
                }

                // ִ���߼�����
                logicManager.DoLogicOperation(flowChartManager, LogicType.AfterDelete, graphElementList);

                graphManager.ConnectorContainerList.Remove(connectorContainer); // �������߿��Ƶ�����������ɾ��
                deleteSuccess = true;
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
            
            SlotContainer slotContainer;
            SlotContainer newSlotContainer;
            ConnectorContainer line;
            ConnectorContainer newLine;
            ICloneable cloneData;

            foreach (object o in dataTable.Keys)
            {
                GraphElement graphElement = o as GraphElement;

                if (graphElement is SlotContainer) // �������
                {
                    slotContainer = graphElement as SlotContainer;
                    newSlotContainer = slotContainer.Clone();
                    graphTable[slotContainer] = newSlotContainer;
                    cloneData = dataTable[slotContainer] as ICloneable;

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
                int index;
                slotContainer = connectorContainer.InSlotContainer;

                if (slotContainer != null
                    && graphElementList.Contains(slotContainer))
                {
                    index = slotContainer.GetBindedOutSlotIndex(connectorContainer);
                    newSlotContainer = graphTable[slotContainer] as SlotContainer;
                    newLine = graphTable[connectorContainer] as ConnectorContainer;

                    newSlotContainer.BindOutLine(newLine, index); 
                }

                slotContainer = connectorContainer.OutSlotContainer;

                if (slotContainer != null
                    && graphElementList.Contains(slotContainer))
                {
                    index = slotContainer.GetBindedInSlotIndex(connectorContainer);
                    newSlotContainer = graphTable[slotContainer] as SlotContainer;
                    newLine = graphTable[connectorContainer] as ConnectorContainer;

                    newSlotContainer.BindInLine(newLine, index);
                }
            }            

            return copyTable;
        }                

        /// <summary>
        /// ������ִ��ǰ�����������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        protected override void SaveBeforeExecute(object o)
        {
            if (flowChartManager.SupportCommand) // �Ż�����
            {
                dataBeforeExecute.SaveData(o);
            }            
        }

        /// <summary>
        /// ������ִ�к󱣴��������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        protected override void SaveAfterExecute(object o)
        {
            if (flowChartManager.SupportCommand) // �Ż�����
            {
                dataAfterExecute.SaveData(o);
            }            
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
            Hashtable args = o as Hashtable;
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
            Hashtable args = o as Hashtable;
            flowChartManager.SetArguments(args);
        }
    }
}
