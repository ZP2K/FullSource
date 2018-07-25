using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.IO;
using FlowChartEditor.BaseGraphElement;
using DevComponents.DotNetBar;
using DevComponents.DotNetBar.Controls;
using GraphElementLibrary;
using Compiler_Sword3;
using DBManager;
using FlowChartDataLibrary;

namespace FlowChartEditor
{
    class LogicManager
    {
        private static LogicManager logicManager; // �߼�������

        /// <summary>
        /// ˽�й��캯��������ģʽ
        /// </summary>
        private LogicManager()
        {

        }

        /// <summary>
        /// ��ȡ�߼�������ʵ��
        /// </summary>
        /// <returns>�߼�������ʵ��</returns>
        public static LogicManager GetLogicManager()
        {
            if(logicManager == null)
            {
                logicManager = new LogicManager();
            }

            return logicManager;
        }

        /// <summary>
        /// ����Ƿ�����¼����
        /// </summary>
        /// <param name="graphManager">ͼԪ������</param>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="eventNode">�¼����</param>
        /// <returns>�Ƿ���Ҫ����</returns>
        private bool CheckCanBindEventNode(GraphManager graphManager, GraphElement graphElement, EventGraphElement eventNode)
        {
            bool avail = true;            
            DataManager dataManager = graphManager.CurrentFlowChartManager.CurrentDataManager;
            IComparable com1;
            IComparable com2;

            if (graphElement is SlotContainer) // �������
            {
                SlotContainer slotContainer = graphElement as SlotContainer;
                if (slotContainer.EventNode != null && slotContainer.EventNode != eventNode)
                {
                    com1 = dataManager.GetData(slotContainer.EventNode) as IComparable;
                    com2 = dataManager.GetData(eventNode) as IComparable;
                    avail = (com1.CompareTo(com2) == 0);
                }
            }
            else if (graphElement is ConnectorContainer) // ������
            {
                ConnectorContainer line = graphElement as ConnectorContainer;
                if (line.EventNode != null && line.EventNode != eventNode)
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
        /// <param name="graphManager">ͼԪ������</param>
        /// <param name="list">��������ͼԪ����</param>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="eventNode">�¼����</param>
        /// <param name="enable">ͼԪ�Ƿ����</param>
        public void ReBindEventNode(GraphManager graphManager, List<GraphElement> list, GraphElement graphElement, EventGraphElement eventNode, bool enable)
        {
            if (!list.Contains(graphElement)) // ��û�б�������ǰͼԪ
            {
                DataManager dataManager = graphManager.CurrentFlowChartManager.CurrentDataManager;

                if (graphElement is SlotContainer) // ��ͼԪ�ǲ������
                {
                    SlotContainer slotContainer = graphElement as SlotContainer;

                    if (!enable && slotContainer.EventNode != null && dataManager.IsConnect(slotContainer.EventNode, slotContainer)) // ����Ҫ���µ�ǰͼԪ���¼����
                    {

                    }
                    else
                    {
                        list.Add(slotContainer);
                        slotContainer.Enable = enable;
                        slotContainer.EventNode = eventNode;

                        foreach (GraphElement g in dataManager.GetNextGraphElements(graphElement))
                        {
                            ReBindEventNode(graphManager, list, g, eventNode, enable);
                        }
                    }
                }
                else if (graphElement is ConnectorContainer) // ��ͼԪ�������߿��Ƶ�����
                {
                    ConnectorContainer line = graphElement as ConnectorContainer;

                    if (!enable && line.EventNode != null && dataManager.IsConnect(line.EventNode, line)) // ����Ҫ���µ�ǰͼԪ���¼����
                    {

                    }
                    else
                    {
                        list.Add(line);
                        line.Enable = enable;
                        line.EventNode = eventNode;

                        foreach (GraphElement g in dataManager.GetNextGraphElements(graphElement))
                        {
                            ReBindEventNode(graphManager, list, g, eventNode, enable);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// ������������������������Ƿ���Ч
        /// </summary>
        /// <param name="slotContainer">�������</param>
        private void CheckLineValid(DataManager dataManager, SlotContainer slotContainer)
        {
            int invalidLineCount = 0;
            List<ConnectorContainer> list = new List<ConnectorContainer>();

            foreach (SlotGraphElement slot in slotContainer.GetOutSlotList())
            {
                if (slot.Binded)
                {
                    object data = dataManager.GetData(slot.BindingConnector.Line);
                    if (data == null)
                    {
                        list.Add(slot.BindingConnector.Line);
                        invalidLineCount++;
                    }
                    else
                    {
                        slot.BindingConnector.Line.Invalid = false;
                    }
                }
            }

            if (invalidLineCount == 1) // ��Ч������������1
            {
                foreach (ConnectorContainer line in list)
                {
                    line.Invalid = false;
                    line.Text = "else";
                    line.ShowText = true;
                    line.AdjustText();
                }
            }
            else if (invalidLineCount > 1) // ��Ч������������1
            {
                foreach (ConnectorContainer line in list)
                {
                    line.Invalid = true;
                    line.Text = "��";
                    line.ShowText = true;
                    line.AdjustText();
                }
            }
            else
            {
                foreach (ConnectorContainer line in list)
                {
                    line.Invalid = false;
                    line.ShowText = false;
                }
            }
        }

        /// <summary>
        /// �������ͼ����Ч��
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="name">����ͼ������</param>
        /// <param name="slotContainerList">�����������</param>
        /// <param name="connectorContainerList">����������</param>
        /// <param name="logBox">������־����</param>
        /// <param name="showDetail">�Ƿ���ʾϸ��</param>
        /// <returns>����ͼ�Ƿ���Ч</returns>
        private bool CheckFlowChartAvail(DataManager dataManager, string name, List<SlotContainer> slotContainerList, List<ConnectorContainer> connectorContainerList,
            TextBoxX logBox, bool showDetail)
        {
            bool avail = true;

            // ��������ߵ���Ч��
            avail = CheckLineAvail(dataManager, name, connectorContainerList, logBox, showDetail);

            // ���ͼԪ����Ч��
            if (avail)
            {
                List<GraphElement> list = new List<GraphElement>();
                foreach (SlotContainer slotContainer in slotContainerList)
                {
                    list.Add(slotContainer);
                }

                foreach (ConnectorContainer line in connectorContainerList)
                {
                    list.Add(line);
                }

                avail = CheckGraphElementAvail(dataManager, name, list, logBox, showDetail);
            }

            // ���ѭ������Ч��
            if (avail)
            {
                avail = CheckCircleAvail(dataManager, name, slotContainerList, logBox, showDetail);
            }

            return avail;
        }

        /// <summary>
        /// ���ͼԪ�Ƿ���Ч
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="name">����ͼ������</param>
        /// <param name="graphElementList">ͼԪ����</param>
        /// <param name="showDetail">�Ƿ���ʾϸ��</param>
        /// <param name="logBox">�����־����</param>
        /// <returns>ͼԪ�Ƿ���Ч</returns>
        private bool CheckGraphElementAvail(DataManager dataManager, string name, List<GraphElement> graphElementList, TextBoxX logBox, bool showDetail)
        {
            bool avail = true;
            GraphElement invalidGraphElement = null;

            foreach (GraphElement graphElement in graphElementList)
            {
                if (!graphElement.Enable)
                {
                    avail = false;
                    invalidGraphElement = graphElement;
                    logBox.AppendText(string.Format("IDΪ{0}��ͼԪû����ȷ�����¼���㣡", graphElement.ID));
                    break;
                }
            }

            if (!avail) // ����ͼ��Ч 
            {
                MessageBox.Show(string.Format("����ͼ {0} ���в���ȷ���ӵ�ͼԪ����ȷ�����е�ͼԪ���������¼���㣡", name),
                        "����ͼ��Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);

                if (showDetail) // ��ʾϸ��
                {
                    List<GraphElement> list = new List<GraphElement>();
                    list.Add(invalidGraphElement);
                    dataManager.CurrentFlowChartManager.CurrentGraphManager.SelectGraphElements(list);
                }
            }

            return avail;
        }

        /// <summary>
        /// ����������Ƿ���Ч
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="name">����ͼ������</param>
        /// <param name="connectorContainerList">����������</param>
        /// <param name="showDetail">�Ƿ���ʾϸ��</param>
        /// <param name="logBox">�����־����</param>
        /// <returns>�������Ƿ���Ч</returns>
        private bool CheckLineAvail(DataManager dataManager, string name, List<ConnectorContainer> connectorContainerList, TextBoxX logBox, bool showDetail)
        {
            bool avail = true;
            ConnectorContainer invalidLine = null;

            foreach (ConnectorContainer line in connectorContainerList)
            {
                if (line.InSlotContainer == null || line.OutSlotContainer == null) // ���������˱��������Ͻ��
                {
                    avail = false;
                    invalidLine = line;
                    logBox.AppendText(string.Format("IDΪ{0}��������û����ȷ���ӣ�\r\n", line.ID));
                    break;
                }
                else if (line.Invalid) // �����߱�����Ч
                {
                    avail = false;
                    invalidLine = line;
                    logBox.AppendText(string.Format("IDΪ{0}��������û������������\r\n", line.ID));
                    break;
                }
            }

            if (!avail) // ����ͼ��Ч 
            {
                MessageBox.Show(string.Format("����ͼ {0} ���в����������ӣ���ȷ�����е������Ƿ���ȷ��", name),
                    "����ͼ��Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);

                if (showDetail) // ��ʾϸ��
                {
                    List<GraphElement> list = new List<GraphElement>();
                    list.Add(invalidLine);
                    dataManager.CurrentFlowChartManager.CurrentGraphManager.SelectGraphElements(list);
                }
            }

            return avail;
        }

        /// <summary>
        /// ���ѭ��Ȧ�Ƿ���Ч
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="name">����ͼ������</param>
        /// <param name="slotContainerList">�����������</param>
        /// <param name="showDetail">�Ƿ���ʾϸ��</param>
        /// <param name="logBox">�����־����</param>
        /// <return>ѭ��Ȧ�Ƿ���Ч</return>
        private bool CheckCircleAvail(DataManager dataManager, string name, List<SlotContainer> slotContainerList, TextBoxX logBox, bool showDetail)
        {
            bool avail = true;
            List<SlotContainer> list = new List<SlotContainer>();
            List<List<SlotContainer>> circleLists = new List<List<SlotContainer>>();
            List<SlotContainer> circleNodeList = new List<SlotContainer>();
            List<List<SlotContainer>> markList = new List<List<SlotContainer>>();

            foreach (SlotContainer slotContainer in slotContainerList)
            {
                if (slotContainer is EventGraphElement)
                {
                    dataManager.FindNextNode(slotContainer, list, circleLists, circleNodeList);
                }
            }

            if (circleLists.Count > 0)
            {
                foreach (List<SlotContainer> circleList in circleLists)
                {
                    avail = false;

                    foreach (SlotContainer node in circleList)
                    {
                        if (node is ConditionGraphElement) // �ҵ����е��������
                        {
                            // ��鵱ǰȦ�е�������������������������������
                            foreach (GraphElement graphElement in dataManager.GetNextGraphElements(node))
                            {
                                ConnectorContainer line = graphElement as ConnectorContainer;
                                if (dataManager.GetData(line) != null && circleList.Contains(line.OutSlotContainer))
                                {
                                    avail = true;
                                    break;
                                }
                            }

                            if (avail)
                            {
                                break;
                            }

                            // ����Ƿ���ͨ�������������Ȧ�Ľ��
                            foreach (SlotContainer nextNode in dataManager.GetSpecifyNextSlotContainers(node, circleList))
                            {
                                if (!circleNodeList.Contains(nextNode)) // ������Ȧ�Ľ��
                                {
                                    avail = true;
                                    break;
                                }
                            }

                            if (avail)
                            {
                                break;
                            }
                        }
                    }

                    if (!avail)
                    {
                        markList.Add(circleList);
                        string circleString = "";
                        for (int i = 0; i < circleList.Count; i++)
                        {
                            circleString += circleList[i].ID + " -> ";
                        }
                        circleString += circleList[0].ID;
                        logBox.AppendText(string.Format("����ѭ��Ȧ{0}��\r\n", circleString));
                    }
                }

                if (markList.Count > 0) // ��⵽��Ч��ѭ��
                {
                    avail = false;
                    MessageBox.Show(string.Format("����ͼ {0} ���п�����Ч��ѭ������ȷ�����е�ѭ���Ƿ���������Ч���ڣ�", name),
                        "����ͼ��Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);

                    if (showDetail) // ��ʾϸ��
                    {
                        CheckCircleForm cForm = new CheckCircleForm(dataManager.CurrentFlowChartManager.CurrentGraphManager, markList);
                        cForm.ShowDialog();
                    }
                }
            }

            return avail;
        }

        /// <summary>
        /// ִ���߼�����
        /// </summary>
        /// <param name="flowChartManager">����ͼ������</param>
        /// <param name="logicType">�߼�����</param>
        /// <param name="logicData">�߼�����</param>
        /// <return>ִ�н��</return>
        public bool DoLogicFunction(FlowChartManager flowChartManager, LogicType logicType, object logicData)
        {
            bool result = true;
            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            GraphManager graphManager = null;
            DataManager dataManager = null;
            if(flowChartManager != null)
            {
                graphManager = flowChartManager.CurrentGraphManager;
                dataManager = flowChartManager.CurrentDataManager;
            }

            switch(logicType)
            {
                case LogicType.Connect: // ���Ӳ���
                    {                        
                        object[] data = logicData as object[];
                        SlotGraphElement slot = data[0] as SlotGraphElement;
                        ConnectorGraphElement connector = data[1] as ConnectorGraphElement;                        

                        switch (flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {
                                    List<GraphElement> list = new List<GraphElement>(); // ��¼��������ͼԪ������

                                    if (connector.IsHeadPoint) // ����������
                                    {
                                        SlotContainer inSlotContainer = connector.Line.InSlotContainer;
                                        if (inSlotContainer != null)
                                        {
                                            slot.SlotContainer.EventNode = inSlotContainer.EventNode;
                                            bool enable = (inSlotContainer.EventNode != null && dataManager.IsConnect(inSlotContainer.EventNode, inSlotContainer));

                                            // ����Ƿ���Ը��°��¼�
                                            if (CheckCanBindEventNode(graphManager, slot.SlotContainer, inSlotContainer.EventNode))
                                            {
                                                ReBindEventNode(graphManager, list, slot.SlotContainer, inSlotContainer.EventNode, enable); // ���°󶨵�ǰͼԪ��������ͼԪ���¼����
                                            }
                                            else
                                            {
                                                slot.UnBind();
                                                MessageBox.Show("���ʽ�������¼����Ͳ�ƥ�䣡\r\n\r\n���������¼����������Ƿ���ȷ",
                                                    "���Ӽ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                                result = false;
                                            }
                                        }
                                    }
                                    else // �����������
                                    {
                                        SlotContainer outSlotContainer = connector.Line.OutSlotContainer;
                                        if (outSlotContainer != null)
                                        {
                                            bool enable = (slot.SlotContainer.EventNode != null && dataManager.IsConnect(slot.SlotContainer.EventNode, slot.SlotContainer));

                                            // ����Ƿ���Ը��°��¼�
                                            if (CheckCanBindEventNode(graphManager, connector.Line, slot.SlotContainer.EventNode))
                                            {
                                                ReBindEventNode(graphManager, list, connector.Line, slot.SlotContainer.EventNode, enable); // ���°󶨵�ǰͼԪ��������ͼԪ���¼����
                                            }
                                            else
                                            {
                                                slot.UnBind();
                                                MessageBox.Show("���ʽ�������¼����Ͳ�ƥ�䣡\r\n\r\n���������¼����������Ƿ���ȷ",
                                                    "���Ӽ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                                result = false;
                                            }
                                        }
                                    }

                                    if (slot.IsOutSlot && slot.SlotContainer is ConditionGraphElement) //  �����������������Ҫ������Ч�Լ��
                                    {
                                        CheckLineValid(dataManager, slot.SlotContainer);
                                    }

                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.Disconnect: // ������Ӳ���
                    {
                        object[] args = logicData as object[];
                        SlotGraphElement slot = args[0] as SlotGraphElement;                        
                        ConnectorGraphElement connector = args[1] as ConnectorGraphElement;
                        ConnectorContainer line = connector.Line;
                        SlotContainer slotContainer = slot.SlotContainer;

                        switch (flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {
                                    List<GraphElement> list = new List<GraphElement>(); // ��¼������ͼԪ������
                                    if (connector.IsHeadPoint) // �Ƴ������ߵ�ͷ���
                                    {
                                        ReBindEventNode(graphManager, list, slotContainer, null, false); // ���°󶨵�ǰͼԪ��������ͼԪ���¼����
                                    }
                                    else // �Ƴ������ߵ�β���
                                    {
                                        SlotContainer outSlotContainer = connector.Line.OutSlotContainer;
                                        if (outSlotContainer != null)
                                        {
                                            ReBindEventNode(graphManager, list, connector.Line, null, false); // ���°󶨵�ǰͼԪ��������ͼԪ���¼����
                                        }
                                    }

                                    if (slot.IsOutSlot && slotContainer is ConditionGraphElement) //  �����������������Ҫ������Ч�Լ��
                                    {
                                        CheckLineValid(dataManager, slotContainer);
                                        if (line.Text == "��") // ��������Ч
                                        {
                                            line.Invalid = false;
                                            line.ShowText = false;
                                        }
                                    }

                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    break;
                                }
                        } 

                        break;
                    }
                case LogicType.BeforeDelete: // ɾ��֮ǰ�Ĳ���
                    {
                        GraphElement graphElement = logicData as GraphElement;

                        switch(flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {
                                    if(graphElement is SlotContainer) // ��ǰͼԪ�ǲ������
                                    {
                                        SlotContainer slotContainer = graphElement as SlotContainer;                                                                                

                                        if(slotContainer is ConditionGraphElement) // �����������������Ҫ������Ч�Լ��
                                        {
                                            foreach(SlotGraphElement slot in slotContainer.GetOutSlotList())
                                            {
                                                if(slot.Binded)
                                                {
                                                    ConnectorContainer line = slot.BindingConnector.Line;
                                                    if (line.Invalid)
                                                    {
                                                        line.Invalid = false;
                                                        line.ShowText = false;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else if(graphElement is ConnectorContainer) // ��ǰͼԪ��������
                                    {
                                        ConnectorContainer line = graphElement as ConnectorContainer;

                                        if (line.InSlotContainer is ConditionGraphElement) // �����������������Ҫ������Ч�Լ��
                                        {
                                            CheckLineValid(dataManager, line.InSlotContainer);
                                        }
                                    }

                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.AfterDelete: // ɾ��֮��Ĳ���
                    {
                        List<GraphElement> graphElementList = logicData as List<GraphElement>;

                        foreach(GraphElement graphElement in graphElementList)
                        {
                            List<GraphElement> list = new List<GraphElement>(); // ��¼������ͼԪ������
                            ReBindEventNode(graphManager, list, graphElement, null, false); // �����趨�¼���㼰ͼԪ�Ƿ����
                        }

                        break;
                    }
                case LogicType.Edit: // �༭����
                    {
                        switch(flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // ����ͼ�༭ģʽ
                                {
                                    GraphElement graphElement = logicData as GraphElement;

                                    if (graphElement is SlotContainer)
                                    {
                                        SlotContainer slotContainer = graphElement as SlotContainer;
                                        if (graphElement is ConditionGraphElement)
                                        {
                                            CheckLineValid(dataManager, slotContainer);
                                        }

                                        // �����ı����ݵ�����������Ĵ�С
                                        slotContainer.AdjustElementSize();
                                    }
                                    else if (graphElement is ConnectorContainer)
                                    {
                                        ConnectorContainer line = graphElement as ConnectorContainer;

                                        if (line.InSlotContainer != null && line.InSlotContainer is ConditionGraphElement) // ����������������Ҫ������
                                        {
                                            CheckLineValid(dataManager, line.InSlotContainer);
                                        }
                                    }

                                    break;
                                }
                            case ChartMode.AIChart: // AIͼ�༭ģʽ
                                {
                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.CheckEditable:
                    {
                        GraphElement graphElement = logicData as GraphElement;

                        switch(flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {
                                    if (graphElement is ConnectorContainer)
                                    {
                                        ConnectorContainer line = graphElement as ConnectorContainer;
                                        if (line.InSlotContainer != null && !(line.InSlotContainer is ConditionGraphElement))
                                        {
                                            result = false;
                                        }
                                    }

                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.CreateLine: // ���������߲���
                    {
                        object[] args = logicData as object[];
                        int id = (int)args[0];
                        SlotGraphElement slot = args[1] as SlotGraphElement;
                        Point p = (Point)args[2];
                        Helper helper = Helper.GetHelper();

                        switch(flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart:
                                {
                                    LineGraphElement lineGraphElement = new LineGraphElement(slot.Location, p);
                                    lineGraphElement.ID = id;
                                    lineGraphElement.Name = "������";
                                    lineGraphElement.TextFont = helper.TextFont;
                                    lineGraphElement.InvalidColor = Color.Brown;
                                    lineGraphElement.Init();
                                    slot.Bind(lineGraphElement.GetConnectorList()[0]);
                                    lineGraphElement.Refresh();
                                    graphManager.SelectedGraphElement = lineGraphElement;

                                    break;
                                }
                            case ChartMode.AIChart:
                                {
                                    AILineGraphElement aiLineGraphElement = new AILineGraphElement(slot.Location, p);
                                    aiLineGraphElement.ID = id;
                                    aiLineGraphElement.Name = "AI������";
                                    aiLineGraphElement.TextFont = helper.TextFont;
                                    aiLineGraphElement.InvalidColor = Color.Brown;
                                    aiLineGraphElement.Init();
                                    slot.Bind(aiLineGraphElement.GetConnectorList()[0]);
                                    aiLineGraphElement.Refresh();
                                    graphManager.SelectedGraphElement = aiLineGraphElement;

                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.CreateAndConnectLine: // ����������������
                    {
                        Helper helper = Helper.GetHelper();
                        object[] args = logicData as object[];
                        int id = (int)args[0];
                        SlotGraphElement outSlot = args[1] as SlotGraphElement;
                        SlotGraphElement inSlot = args[2] as SlotGraphElement;

                        switch(flowChartManager.CurrentChartMode) 
                        {
                            case ChartMode.FlowChart: // ����ͼ�༭ģʽ
                                {
                                    LineGraphElement lineGraphElement = new LineGraphElement(outSlot.Location, inSlot.Location);
                                    lineGraphElement.ID = id;
                                    lineGraphElement.Name = "������";
                                    lineGraphElement.TextFont = helper.TextFont;
                                    lineGraphElement.InvalidColor = Color.Brown;
                                    lineGraphElement.Init();
                                    lineGraphElement.Refresh();
                                    graphManager.SelectedGraphElement = lineGraphElement;

                                    break;
                                }
                            case ChartMode.AIChart: // AIͼ�༭ģʽ
                                {
                                    AILineGraphElement aiLineGraphElement = new AILineGraphElement(outSlot.Location, inSlot.Location);
                                    aiLineGraphElement.ID = id;
                                    aiLineGraphElement.Name = "AI������";
                                    aiLineGraphElement.TextFont = helper.TextFont;
                                    aiLineGraphElement.InvalidColor = Color.Brown;
                                    aiLineGraphElement.Init();
                                    aiLineGraphElement.Refresh();
                                    graphManager.SelectedGraphElement = aiLineGraphElement;

                                    break;
                                }
                        }                        

                        break;
                    }
                case LogicType.PopUpContextMenu: // �����Ҽ��˵�
                    {
                        ButtonItem contextMenu = logicData as ButtonItem;

                        switch(flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {
                                    if (graphManager.SelectedGraphElement != null) // ��ͼԪ��ѡ��
                                    {
                                        contextMenu.SubItems["bAddConditionFlowChart"].Text = "���������� ������� (<u>2</u>)";
                                        contextMenu.SubItems["bAddActionFlowChart"].Text = "���������� ������� (<u>3</u>)";
                                    }
                                    else
                                    {
                                        contextMenu.SubItems["bCreateEventFlowChart"].Text = "�����¼���� (<u>1</u>)";
                                    }

                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    if (graphManager.SelectedGraphElement != null) // ��ͼԪ��ѡ��
                                    {
                                        contextMenu.SubItems["bAddConditionFlowChart"].Text = "���������� AI��֧��� (<u>2</u>)";
                                        contextMenu.SubItems["bAddActionFlowChart"].Text = "���������� AI������� (<u>3</u>)";
                                    }
                                    else
                                    {
                                        contextMenu.SubItems["bCreateEventFlowChart"].Text = "����AI״̬��� (<u>1</u>)";
                                    }

                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.Create: // �Ҽ��˵��д���ͼԪ
                    {
                        GraphType graphType = (GraphType)logicData;
                        DocumentManager documentManager = DocumentManager.GetDocumentManager();

                        switch(flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {
                                    switch(graphType)
                                    {
                                        case GraphType.EventNode: // �����¼����
                                            {
                                                flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.EventNode, false);
                                                documentManager.AddGraphType = GraphType.EventNode;
                                                documentManager.AddGraphElement = true;

                                                break;
                                            }
                                        case GraphType.ConditionNode: // �����������
                                            {
                                                flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.ConditionNode, true);
                                                documentManager.AddGraphType = GraphType.ConditionNode;
                                                documentManager.AddGraphElement = true;

                                                break;
                                            }
                                        case GraphType.ActionNode: // �����������
                                            {
                                                flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.ActionNode, true);
                                                documentManager.AddGraphType = GraphType.ActionNode;
                                                documentManager.AddGraphElement = true;

                                                break;
                                            }
                                    }

                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    switch (graphType)
                                    {
                                        case GraphType.EventNode: // ����AI״̬���
                                            {
                                                if(flowChartManager.CurrentGraphManager.SelectedGraphElement != null)
                                                {
                                                    flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.AIStateNode, true);
                                                }
                                                else
                                                {
                                                    flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.AIStateNode, false);
                                                }
                                                
                                                documentManager.AddGraphType = GraphType.AIStateNode;
                                                documentManager.AddGraphElement = true;

                                                break;
                                            }
                                        case GraphType.ConditionNode: // ����AI�������
                                            {
                                                flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.AIActionNode, true);
                                                documentManager.AddGraphType = GraphType.AIActionNode;
                                                documentManager.AddGraphElement = true;

                                                break;
                                            }
                                        case GraphType.ActionNode: // ����AI��������
                                            {
                                                flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.AIActionsNode, true);
                                                documentManager.AddGraphType = GraphType.AIActionsNode;
                                                documentManager.AddGraphElement = true;

                                                break;
                                            }
                                    }

                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.CreateAtPoint: // ��ָ���㴴��
                    {
                        DocumentManager documentManager = DocumentManager.GetDocumentManager();
                        object[] args = logicData as object[];
                        int graphType = (int)args[0];
                        Point location = (Point)args[1];

                        switch(flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {
                                    switch(graphType)
                                    {
                                        case 1: // �����¼����
                                            {
                                                flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.EventNode, location, false);
                                                documentManager.AddGraphType = GraphType.EventNode;
                                                documentManager.AddGraphElement = true;

                                                break;
                                            }
                                        case 2: // �����������
                                            {
                                                flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.ConditionNode, location, true);
                                                documentManager.AddGraphType = GraphType.ConditionNode;
                                                documentManager.AddGraphElement = true;

                                                break;
                                            }
                                        case 3: // �����������
                                            {
                                                flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.ActionNode, location, true);
                                                documentManager.AddGraphType = GraphType.ActionNode;
                                                documentManager.AddGraphElement = true;

                                                break;
                                            }
                                    }

                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    switch(graphType)
                                    {
                                        case 1: // ����AI״̬���
                                            {
                                                if(flowChartManager.CurrentGraphManager.SelectedGraphElement != null)
                                                {
                                                    flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.AIStateNode, location, true);
                                                }
                                                else
                                                {
                                                    flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.AIStateNode, location, false);
                                                }
                                                
                                                documentManager.AddGraphType = GraphType.AIStateNode;
                                                documentManager.AddGraphElement = true;

                                                break;
                                            }
                                        case 2: // ����AI�������
                                            {
                                                if(graphManager.SelectedGraphElement is SlotContainer)
                                                {
                                                    flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.AIActionNode, location, true);
                                                    documentManager.AddGraphType = GraphType.AIActionNode;
                                                    documentManager.AddGraphElement = true;
                                                }                                                

                                                break;
                                            }
                                        case 3: // ����AI��������
                                            {
                                                if(graphManager.SelectedGraphElement is SlotContainer)
                                                {
                                                    flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.AIActionsNode, location, true);
                                                    documentManager.AddGraphType = GraphType.AIActionsNode;
                                                    documentManager.AddGraphElement = true;
                                                }                                                

                                                break;
                                            }
                                    }                                    

                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.ViewCode: // �鿴����
                    {
                        DocumentManager documentManager = DocumentManager.GetDocumentManager();
                        Helper helper = Helper.GetHelper();
                        string relevatePath = logicData as string;

                        switch(documentManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {
                                    string path = Path.Combine(helper.OutputDir, "scripts\\Map");
                                    path = Path.Combine(path, string.Format("{0}.lua", relevatePath));
                                    FileInfo fi = new FileInfo(path);
                                    if(fi.Exists) // �ļ�����
                                    {
                                        try
                                        {
                                            System.Diagnostics.Process.Start(path);
                                        }
                                        catch (Exception ex)
                                        {
                                            MessageBox.Show("�ڲ鿴�����ļ�ʱ����IO�쳣��" + ex.ToString());	
                                        }
                                    }
                                    else
                                    {
                                        MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", path), "�鿴����",
                                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                                    }

                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.CompileCode: // ���ɴ���
                    {                       

                        break;
                    }
                case LogicType.CompileCodes: // ���ɴ���
                    {
                        break;
                    }
                case LogicType.CheckLogic: // ����߼�
                    {
                        object[] args = logicData as object[];
                        dataManager = args[0] as DataManager;
                        string name = args[1] as string;
                        List<SlotContainer> slotContainerList = args[2] as List<SlotContainer>;
                        List<ConnectorContainer> connectorContainerList = args[3] as List<ConnectorContainer>;
                        TextBoxX logBox = args[4] as TextBoxX;
                        bool showDetail = (bool)args[5];

                        switch(flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {
                                    result = CheckFlowChartAvail(dataManager, name, slotContainerList, connectorContainerList, logBox,
                                        showDetail);
                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.ManageArguments: // �������
                    {
                        DocumentManager documentManager = DocumentManager.GetDocumentManager();

                        switch(documentManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {
                                    VarManager.Program varManager = new VarManager.Program();
                                    varManager.Show(dataBaseManager.GetJx3webConnection());

                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    flowChartManager = documentManager.CurrentFlowChartManager;

                                    if(flowChartManager == null)
                                    {
                                        MessageBox.Show("��ǰû������ͼ�����", "��������", MessageBoxButtons.OK,
                                            MessageBoxIcon.Information);
                                        result = false;
                                    }
                                    else
                                    {

                                    }

                                    break;
                                }
                        }

                        break;
                    }
                case LogicType.SetCompileArgs: // ���ô������ɵĲ���
                    {
                        FlowChartMetaDataManager flowChartMetaDataManager = logicData as FlowChartMetaDataManager;
                        Helper helper = Helper.GetHelper();

                        switch(flowChartManager.CurrentChartMode)
                        {
                            case ChartMode.FlowChart: // �༭����ͼģʽ
                                {                                    
                                    string fullPath = Path.Combine(helper.OutputDir, "scripts\\Map");
                                    fullPath = Path.Combine(fullPath, flowChartManager.CurrentPath);
                                    string[] data = flowChartManager.CurrentPath.Split(new char[] { '\\' });

                                    flowChartMetaDataManager.RootPath = helper.OutputDir;
                                    flowChartMetaDataManager.FlowChartFullPath = fullPath;
                                    flowChartMetaDataManager.FlowChartShortPath = flowChartManager.CurrentPath;
                                    flowChartMetaDataManager.FlowChartName = data[data.Length - 1];
                                    flowChartMetaDataManager.MapName = data[0];

                                    break;
                                }
                            case ChartMode.AIChart: // �༭AIͼģʽ
                                {
                                    string fullPath = Path.Combine(helper.OutputDir, "scripts\\ai");
                                    fullPath = Path.Combine(fullPath, flowChartManager.CurrentPath);
                                    string[] data = flowChartManager.CurrentPath.Split(new char[] { '\\' });

                                    flowChartMetaDataManager.RootPath = helper.OutputDir;
                                    flowChartMetaDataManager.FlowChartFullPath = fullPath;
                                    flowChartMetaDataManager.FlowChartShortPath = flowChartManager.CurrentPath;
                                    flowChartMetaDataManager.FlowChartName = data[data.Length - 1];
                                    flowChartMetaDataManager.MapName = data[0];

                                    break;
                                }
                        }

                        break;
                    }
            }

            return result;
        }
    }
}
