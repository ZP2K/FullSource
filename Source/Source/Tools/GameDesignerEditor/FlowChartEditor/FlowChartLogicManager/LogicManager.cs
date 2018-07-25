using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.IO;
using System.Data;
using System.Collections;

using DevComponents.DotNetBar;
using DevComponents.DotNetBar.Controls;

using FlowChartEditor;
using Compiler_Sword3;
using DataInteractor;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;
using DataElementLibrary;
using DBManager;
using FlowChartDataLibrary;

namespace FlowChartLogicManager
{
    public class LogicManager:LogicBaseManager
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        public LogicManager()
        {
            
        }

        /// <summary>
        /// ��ʼ����ͼ����
        /// </summary>
        protected override void InitGraphSetting()
        {
            GraphSetting graphSetting = GraphSetting.GetGraphSetting();
            graphSetting.GraphElementTextColor = Color.Yellow;
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicConnect(FlowChartManager flowChartManager, object logicData)
        {
            bool executeSuccess = true;
            DataManager dataManager = flowChartManager.CurrentDataManager;

            object[] data = logicData as object[];
            SlotGraphElement slot = data[0] as SlotGraphElement;
            ConnectorGraphElement connector = data[1] as ConnectorGraphElement;
            bool enableConnect = true;
            List<GraphElement> list = new List<GraphElement>(); // ��¼��������ͼԪ������

            if (connector.IsHeadPoint) // ����������
            {
                SlotContainer inSlotContainer = connector.Line.InSlotContainer;

                if (inSlotContainer != null)
                {
                    List<DataElement> eventList = dataManager.GetEventList(connector.Line.OutSlotContainer);

                    // ����¼���㻥��
                    enableConnect = LogicCheck.CheckEventExclusion(eventList);

                    if (enableConnect) // ���������¼����
                    {
                        bool enable = dataManager.IsConnectEventNode(inSlotContainer);
                        ReBindEventNode(dataManager, list, slot.SlotContainer, enable); // ���°󶨵�ǰͼԪ��������ͼԪ���¼����
                    }
                    else
                    {
                        slot.UnBind();
                        executeSuccess = false;
                    }
                }
            }
            else // �����������
            {
                SlotContainer outSlotContainer = connector.Line.OutSlotContainer;

                if (outSlotContainer != null)
                {
                    List<DataElement> eventList = dataManager.GetEventList(outSlotContainer);

                    if (enableConnect) // ���������¼����
                    {
                        bool enable = dataManager.IsConnectEventNode(slot.SlotContainer);
                        ReBindEventNode(dataManager, list, connector.Line, enable); // ���°󶨵�ǰͼԪ��������ͼԪ���¼���� 
                    }
                    else
                    {
                        slot.UnBind();
                        executeSuccess = false;
                    }
                }
            }

            return executeSuccess;
        }
        
        /// <summary>
        /// �������ͼԪ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicDisconnect(FlowChartManager flowChartManager, object logicData)
        {
            DataManager dataManager = flowChartManager.CurrentDataManager;

            object[] args = logicData as object[];
            SlotGraphElement slot = args[0] as SlotGraphElement;
            ConnectorGraphElement connector = args[1] as ConnectorGraphElement;
            ConnectorContainer line = connector.Line;
            SlotContainer slotContainer = slot.SlotContainer;

            List<GraphElement> list = new List<GraphElement>(); // ��¼������ͼԪ������

            if (connector.IsHeadPoint) // �Ƴ������ߵ�ͷ���
            {
                ReBindEventNode(dataManager, list, slotContainer, false); // ���°󶨵�ǰͼԪ��������ͼԪ���¼����
            }
            else // �Ƴ������ߵ�β���
            {
                SlotContainer outSlotContainer = connector.Line.OutSlotContainer;

                if (outSlotContainer != null)
                {
                    ReBindEventNode(dataManager, list, connector.Line, false); // ���°󶨵�ǰͼԪ��������ͼԪ���¼����
                }
            }

            return true;
        }

        /// <summary>
        /// ɾ��ͼԪ֮��Ĳ���
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicAfterDelete(FlowChartManager flowChartManager, object logicData)
        {
            DataManager dataManager = flowChartManager.CurrentDataManager;
            List<GraphElement> graphElementList = logicData as List<GraphElement>;

            foreach (GraphElement graphElement in graphElementList)
            {
                List<GraphElement> list = new List<GraphElement>(); // ��¼������ͼԪ������
                ReBindEventNode(dataManager, list, graphElement, false); // �����趨�¼���㼰ͼԪ�Ƿ����

                ConnectorContainer line = graphElement as ConnectorContainer;

                if (line != null)
                {
                    // �����߻ָ��ɼ�״̬
                    line.Moving = false;
                    line.Visible = true;
                }
            }

            return true;
        }

        /// <summary>
        /// �༭ͼԪ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicEdit(FlowChartManager flowChartManager, object logicData)
        {
            bool executeResult = true;

            GraphManager graphManager = flowChartManager.CurrentGraphManager;
            DataManager dataManager = flowChartManager.CurrentDataManager;
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            GraphElement graphElement = logicData as GraphElement;
            DataElement dataElement = dataManager.GetDataElement(graphElement);

            if (CheckGraphElementEditable(graphElement)) // ���ͼԪ�Ƿ��ܹ��༭
            {
                object data = dataManager.GetData(graphElement);
                Hashtable information = new Hashtable();

                information["data"] = data;
                information["prev_data"] = dataManager.GetPreviousData(graphElement);
                information["next_data"] = dataManager.GetNextData(graphElement);
                information["neighbor_data"] = dataManager.GetNeighborData(graphElement);
                information["globe_args"] = dataManager.GlobeArgs;
                information["flowchart_name"] = flowChartManager.Name;
                information["map_name"] = flowChartManager.MapName;
                information["client_dir"] = Helper.GetHelper().OutputDir;

                if (graphElement is SlotContainer)
                {
                    SlotContainer slotContainer = graphElement as SlotContainer;

                    // ˢ�µ�ǰ�¼����
                    information["event_data"] = dataManager.GetEventData(slotContainer);
                }

                dataElement.PrintInformation = new DataElement.PrintInfo(documentManager.PrintText);

                try
                {
                    executeResult = dataElement.EditData(information);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("��ǰͼԪ��������ԭ���޷��༭��\n\n" + ex.Message, "ͼԪ�༭",
                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                }

                if (executeResult) // ����ͼԪ����
                {
                    Hashtable previousDataTable = information["prev_data"] as Hashtable;
                    Hashtable nextDataTable = information["next_data"] as Hashtable;
                    GraphElement currentGraphElement;
                    DataElement editDataElement;

                    // ����Ƿ���Ҫ����ͼԪ������Ԫ������
                    foreach (string id in previousDataTable.Keys)
                    {
                        editDataElement = previousDataTable[id] as DataElement;

                        if (editDataElement != null && editDataElement.Data == null)
                        {
                            currentGraphElement = dataManager.FindGraphElementByID(int.Parse(id));
                            currentGraphElement.Text = editDataElement.Text;
                            currentGraphElement.TooltipText = editDataElement.TooltipText;
                            currentGraphElement.ShowText = false;
                        }
                    }

                    foreach (string id in nextDataTable.Keys)
                    {
                        editDataElement = nextDataTable[id] as DataElement;

                        if (editDataElement != null && editDataElement.Data == null)
                        {
                            currentGraphElement = dataManager.FindGraphElementByID(int.Parse(id));
                            currentGraphElement.Text = editDataElement.Text;
                            currentGraphElement.TooltipText = editDataElement.TooltipText;
                            currentGraphElement.ShowText = false;
                        }
                    }

                    graphElement.TooltipText = dataElement.TooltipText;
                    graphElement.Text = dataElement.Text;
                    if (dataElement.Text == "")
                    {
                        graphElement.ShowText = false;
                    }
                    else
                    {
                        graphElement.ShowText = true;
                    }

                    // �����ı�
                    if (graphElement is SlotContainer)
                    {
                        SlotContainer slotContainer = graphElement as SlotContainer;
                        slotContainer.AdjustText();

                        // �����ı����ݵ�����������Ĵ�С
                        slotContainer.AdjustElementSize();
                    }
                    else if (graphElement is ConnectorContainer)
                    {
                        ConnectorContainer line = graphElement as ConnectorContainer;
                        line.AdjustText();
                    }
                }
            }

            return executeResult;
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicCreateLine(FlowChartManager flowChartManager, object logicData)
        {
            GraphManager graphManager = flowChartManager.CurrentGraphManager;
            DataManager dataManager = flowChartManager.CurrentDataManager;

            object[] args = logicData as object[];
            int id = graphManager.AllocateGraphElementID();
            SlotGraphElement slot = args[0] as SlotGraphElement;
            Point p = (Point)args[1];

            // ����������
            base.LogicCreateLine(flowChartManager, logicData);

            // �������ߵ��¼�
            List<GraphElement> list = new List<GraphElement>();
            bool enable = dataManager.IsConnectEventNode(slot.SlotContainer);
            ReBindEventNode(dataManager, list, slot.SlotContainer, enable); 

            return true;
        }

        /// <summary>
        /// ��ָ���㴴�����
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicCreateNodeAtPoint(FlowChartManager flowChartManager, object logicData)
        {
            object[] args = logicData as object[];
            int graphType = (int)args[0];
            Point location = (Point)args[1];

            switch (graphType)
            {
                case 1: // �����¼����
                    {
                        CreateNode(GraphType.EventNode, location, false);
                        break;
                    }
                case 2: // �����������
                    {
                        if (flowChartManager.CurrentGraphManager.SelectedGraphElement is SlotContainer) // �������п����ӵĽ��Ļ����ϴ���
                        {
                            CreateNode(GraphType.ConditionNode, location, true);                           
                        }

                        break;
                    }
                case 3: // �����������
                    {
                        if (flowChartManager.CurrentGraphManager.SelectedGraphElement is SlotContainer) // �������п������ӵĽ��Ļ����ϴ���
                        {
                            CreateNode(GraphType.ActionNode, location, true);                            
                        }

                        break;
                    }
            }

            return true;
        }

        /// <summary>
        /// �򿪽ű�
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�����Ƿ�ɹ�</returns>
        protected override bool LogicViewCode(FlowChartManager flowChartManager, object logicData)
        {
            Helper helper = Helper.GetHelper();
            string relevatePath = logicData as string;
            string path = Path.Combine(helper.OutputDir, "scripts\\Map");
            path = Path.Combine(path, string.Format("{0}.lua", relevatePath));            

            if (File.Exists(path)) // �ļ�����
            {
                try
                {
                    System.Diagnostics.Process.Start(path);
                }
                catch (IOException ex)
                {
                    MessageBox.Show("�ڴ򿪴����ļ�ʱ����IO�쳣��" + ex.Message, "�鿴�ű�",
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", path), "�鿴�ű�",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return true;
        }

        /// <summary>
        /// �鿴�ļ���
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicViewFolder(FlowChartManager flowChartManager, object logicData)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            Helper helper = Helper.GetHelper();
            TreeNode currentNode = logicData as TreeNode;

            string path = Path.Combine(helper.OutputDir, "scripts\\Map");
            path = Path.Combine(path, documentManager.GetNodePath(currentNode));
            
            if (Directory.Exists(path)) // ��ǰĿ¼����
            {
                System.Diagnostics.Process.Start("explorer.exe", path);
            }
            else // ��ǰĿ¼������ 
            {
                MessageBox.Show("��Ŀ¼�����ڣ�", "���ļ���", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return true;
        }

        /// <summary>
        /// ����߼�
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicCheckLogic(FlowChartManager flowChartManager, object logicData)
        {
            bool executeResult = CheckFlowChartAvail(flowChartManager);

            return executeResult;
        }

        /// <summary>
        /// �������
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicManageArguments(FlowChartManager flowChartManager, object logicData)
        {
            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            VarManager.Program varManager = new VarManager.Program();
            varManager.Show(dataBaseManager.GetJx3webConnection());

            return true;
        }

        /// <summary>
        /// ���ͼԪ�Ƿ��ܹ��༭
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <returns>ͼԪ�Ƿ��ܹ��༭</returns>
        private bool CheckGraphElementEditable(GraphElement graphElement)
        {
            bool result = true;

            if (graphElement is ConnectorContainer)
            {
                ConnectorContainer line = graphElement as ConnectorContainer;
                if (line.InSlotContainer != null && !(line.InSlotContainer is ConditionGraphElement))
                {
                    result = false;
                }
            }

            return result;
        }

        /// <summary>
        /// ��ȡ����ͼԪ����
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="slotContainerList">����ͼ�������������</param>
        /// <returns>����ͼԪ���ݵ�����</returns>
        protected override List<FlowChartMetaData> GetMetaData(DataManager dataManager, List<SlotContainer> slotContainerList)
        {
            List<FlowChartMetaData> metaDataList = new List<FlowChartMetaData>();
            List<GraphElement> graphElementList = new List<GraphElement>();
            Hashtable graphElementTable = new Hashtable();

            foreach (SlotContainer slotContainer in slotContainerList)
            {
                if (slotContainer is EventGraphElement)
                {
                    object data = dataManager.GetData(slotContainer);
                    DataElement dataElement = dataManager.GetDataElement(slotContainer);
                    FlowChartMetaData metaData = new FlowChartMetaData(slotContainer.ID, data, "EventNode");
                    metaData.Code = CodeProvider.CodeProviderClass.ConvertToCode(data, dataElement.DataType);
                    metaData.DisplayText = slotContainer.TooltipText;
                    FindNextMetaData(dataManager, slotContainer, metaData, graphElementList, graphElementTable);
                    metaDataList.Add(metaData);
                }
            }

            return metaDataList;
        }

        /// <summary>
        /// ��ȡ��һ������ͼԪ����
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="metaData">����ͼԪ����</param>
        /// <param name="graphElementList">��������ͼԪ����</param>
        /// <param name="graphElementTable">����ͼ������ϣ��</param>
        private void FindNextMetaData(DataManager dataManager, GraphElement graphElement, FlowChartMetaData metaData, List<GraphElement> graphElementList, Hashtable graphElementTable)
        {
            graphElementList.Add(graphElement);
            graphElementTable[graphElement] = metaData;
            List<GraphElement> list = dataManager.GetNextGraphElements(graphElement, false);

            foreach (GraphElement g in list)
            {
                object data = dataManager.GetData(g);                
                
                if(!graphElementList.Contains(g))
                {
                    DataElement dataElement = dataManager.GetDataElement(g);
                    FlowChartMetaData newMetaData = new FlowChartMetaData(g.ID, data, dataElement.DataType);
                    newMetaData.Code = CodeProvider.CodeProviderClass.ConvertToCode(data, dataElement.DataType);
                    newMetaData.DisplayText = g.TooltipText;
                    newMetaData.AddPreviousMetaData(metaData);
                    metaData.AddNextMetaData(newMetaData);
                    FindNextMetaData(dataManager, g, newMetaData, graphElementList, graphElementTable);
                }
                else
                {
                    FlowChartMetaData newMetaData = graphElementTable[g] as FlowChartMetaData;
                    newMetaData.AddPreviousMetaData(metaData);
                    metaData.AddNextMetaData(newMetaData);
                }
            }

            if (graphElement is ConditionGraphElement) // �������
            {
                bool avail = true; // �Ƿ���Ҫ����else������

                foreach(GraphElement g in list)
                {
                    if(dataManager.GetData(g) == null) // �ҵ�else������
                    {
                        avail = false;
                        break;
                    }
                }

                if(avail) // ��Ҫ����else������
                {
                    FlowChartMetaData lineMetaData = new FlowChartMetaData(0, null, "Line");
                    lineMetaData.Code = "";
                    lineMetaData.AddPreviousMetaData(metaData);
                    metaData.AddNextMetaData(lineMetaData);
                    FlowChartMetaData endMetaData = new FlowChartMetaData(0, null, "EndNode");
                    endMetaData.AddPreviousMetaData(lineMetaData);
                    lineMetaData.AddNextMetaData(endMetaData);
                }
                else // ��������Ҫ������
                {
                    List<GraphElement> lineList = new List<GraphElement>();
                    GraphElement elseLine = null;
                    FlowChartMetaData newMetaData;

                    foreach(GraphElement g in list)
                    {
                        if(dataManager.GetData(g) == null) // else������
                        {
                            elseLine = g;
                        }
                        else // ��ͨ������
                        {
                            lineList.Add(g);
                        }
                    }

                    metaData.ClearNextMetaDataList();
                    foreach(GraphElement g in lineList)
                    {
                        newMetaData = graphElementTable[g] as FlowChartMetaData;
                        metaData.AddNextMetaData(newMetaData);
                    }
                    newMetaData = graphElementTable[elseLine] as FlowChartMetaData;
                    metaData.AddNextMetaData(newMetaData);
                }
            }
            else
            {
                if(list.Count == 0) // ��ǰͼԪû�к���ͼԪ
                {
                    FlowChartMetaData lineMetaData = new FlowChartMetaData(0, null, "Line");
                    lineMetaData.AddPreviousMetaData(metaData);
                    metaData.AddNextMetaData(lineMetaData);
                    FlowChartMetaData endMetaData = new FlowChartMetaData(0, null, "EndNode");
                    endMetaData.AddPreviousMetaData(lineMetaData);
                    lineMetaData.AddNextMetaData(endMetaData);
                }
            }                        
        }

        /// <summary>
        /// ����ͼԪ�Ƿ����
        /// </summary>
        /// <param name="dataManager">�¼�������</param>
        /// <param name="list">��������ͼԪ����</param>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="enable">ͼԪ�Ƿ����</param>
        public void ReBindEventNode(DataManager dataManager, List<GraphElement> list, GraphElement graphElement, bool enable)
        {
            if (!list.Contains(graphElement)) // ��û�б�������ǰͼԪ
            {
                list.Add(graphElement);

                if(enable)
                {
                    graphElement.Enable = enable;
                }
                else
                {
                    if(graphElement is SlotContainer) // �������
                    {
                        if(!dataManager.IsConnectEventNode(graphElement as SlotContainer))
                        {
                            graphElement.Enable = enable;
                        }
                    }
                    else if(graphElement is ConnectorContainer) // ������
                    {
                        if (!dataManager.IsConnectEventNode(graphElement as ConnectorContainer))
                        {
                            graphElement.Enable = enable;
                        }
                    }                    
                }

                foreach (GraphElement g in dataManager.GetNextGraphElements(graphElement, false))
                {
                    ReBindEventNode(dataManager, list, g, enable);
                }
            }
        }

        /// <summary>
        /// �������ͼ����Ч��
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>        
        /// <returns>����ͼ�Ƿ���Ч</returns>
        private bool CheckFlowChartAvail(FlowChartManager flowChartManager)
        {
            bool avail = true;

            // ��������ߵ���Ч��
            avail = CheckLineAvail(flowChartManager);

            // ���ͼԪ����Ч��
            if (avail)
            {
                avail = CheckGraphElementAvail(flowChartManager);
            }

            return avail;
        }        

        /// <summary>
        /// ���ͼԪ�Ƿ���Ч
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <returns>ͼԪ�Ƿ���Ч</returns>
        private bool CheckGraphElementAvail(FlowChartManager flowChartManager)
        {
            bool avail = true;
            bool unfinished = false;
            GraphElement invalidGraphElement = null;
            GraphElement unfinishedGraphElement = null;
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            DataManager dataManager = flowChartManager.CurrentDataManager;

            foreach (GraphElement graphElement in flowChartManager.CurrentGraphManager.CurrentCanvas.GraphElementList)
            {
                if (!graphElement.Enable)
                {
                    avail = false;
                    invalidGraphElement = graphElement;
                    documentManager.PrintText(string.Format("IDΪ{0}��ͼԪû����ȷ�����¼���㣡", graphElement.ID));                    
                    break;
                }
            }

            if (!avail) // ����ͼ��Ч 
            {
                MessageBox.Show("��ͼ���в���ȷ���ӵ�ͼԪ����ȷ�����е�ͼԪ���������¼���㣡", "����ͼ��Ч�Լ��", 
                                MessageBoxButtons.OK, MessageBoxIcon.Information);

                // ��ʾϸ��
                List<GraphElement> list = new List<GraphElement>();
                list.Add(invalidGraphElement);
                dataManager.CurrentFlowChartManager.CurrentGraphManager.SelectGraphElements(list);
            }

            if(unfinished && avail) // ��δ�༭��ɵ�ͼԪ
            {
                avail = false;
                MessageBox.Show("��ͼ����û����ɱ༭��ͼԪ����ȷ�����е�ͼԪ�������ò�����", "����ͼ��Ч�Լ��", 
                                MessageBoxButtons.OK, MessageBoxIcon.Information);

                // ��ʾϸ��
                List<GraphElement> list = new List<GraphElement>();
                list.Add(unfinishedGraphElement);
                dataManager.CurrentFlowChartManager.CurrentGraphManager.SelectGraphElements(list);
            }

            return avail;
        }

        /// <summary>
        /// ����������Ƿ���Ч
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>        
        /// <returns>�������Ƿ���Ч</returns>
        private bool CheckLineAvail(FlowChartManager flowChartManager)
        {
            bool avail = true;
            ConnectorContainer invalidLine = null;
            DocumentManager documentManager = DocumentManager.GetDocumentManager();

            foreach (ConnectorContainer line in flowChartManager.CurrentGraphManager.ConnectorContainerList)
            {
                if (line.InSlotContainer == null || line.OutSlotContainer == null) // ���������˱��������Ͻ��
                {
                    avail = false;
                    invalidLine = line;
                    documentManager.PrintText(string.Format("IDΪ{0}��������û����ȷ���ӣ�", line.ID));
                    break;
                }
            }

            if (!avail) // ����ͼ��Ч 
            {
                MessageBox.Show("��ͼ���в����������ӣ���ȷ�����е������Ƿ���ȷ��", "����ͼ��Ч�Լ��", 
                                MessageBoxButtons.OK, MessageBoxIcon.Information);

                // ��ʾϸ��
                List<GraphElement> list = new List<GraphElement>();
                list.Add(invalidLine);
                flowChartManager.CurrentGraphManager.SelectGraphElements(list);
            }

            return avail;
        }
    }
}
