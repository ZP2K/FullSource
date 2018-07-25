using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.IO;
using System.Collections;
using System.Drawing;

using DevComponents.DotNetBar;

using CodeCompiler;
using DBManager;
using FlowChartDataLibrary;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;
using DataElementLibrary;

namespace FlowChartEditor
{
    public class LogicBaseManager
    {
        protected const string pluginPath = @"Plugins\FlowChart"; // ���·��
        protected Compiler compiler;
        protected Dictionary<string, DataElementInfo> dataElementInfoDirectory = new Dictionary<string, DataElementInfo>();
        protected string displayName = "��ͼ"; // ��ʾ����

        protected ContextMenuBar contextMenyBar = new ContextMenuBar(); // �Ҽ��˵�
        protected ButtonItem editMenu = new ButtonItem("editMenu", "editMenu"); // �༭�˵�
        protected ButtonItem canvasMenu = new ButtonItem("canvasMenu", "canvasMenu"); // ��ͼ��˵�

        /// <summary>
        /// ���캯��
        /// </summary>
        public LogicBaseManager()
        {
            // ��ʼ����ͼ����
            InitGraphSetting();

            // ��ʼ���Ҽ��˵�
            InitMenuStrip();
        }

        /// <summary>
        /// ��ʾ����
        /// </summary>
        public string DisplayName
        {
            set
            {
                displayName = value;
            }

            get
            {
                return displayName;
            }
        }

        /// <summary>
        /// ��ʼ����ͼ����
        /// </summary>
        protected virtual void InitGraphSetting()
        {
            GraphSetting graphSetting = GraphSetting.GetGraphSetting();
            graphSetting.GraphElementTextColor = Color.Black;
        }

        /// <summary>
        /// ��ʼ���Ҽ��˵�
        /// </summary>
        protected virtual void InitMenuStrip()
        {
            // ��ʼ���˵�����                        
            contextMenyBar.Items.Add(editMenu);
            contextMenyBar.Items.Add(canvasMenu);
            contextMenyBar.Style = eDotNetBarStyle.Office2003;

            // ��ʼ���༭�˵�
            ButtonItem bEditRemark = new ButtonItem("bEditRemark", "�༭ע��");
            bEditRemark.Click += new EventHandler(EditRemark);
            editMenu.SubItems.Add(bEditRemark);

            ButtonItem bCutGraphElement = new ButtonItem("bCutGraphElement", "���� (Ctrl + P)");
            bCutGraphElement.Click += new EventHandler(CutGraphElement);
            editMenu.SubItems.Add(bCutGraphElement);

            ButtonItem bCopyGraphElement = new ButtonItem("bCopyGraphElement", "���� (Ctrl + C)");
            bCopyGraphElement.Click += new EventHandler(CopyGraphElement);
            editMenu.SubItems.Add(bCopyGraphElement);

            ButtonItem bCAndCConditionNode = new ButtonItem("bCAndCConditionNode", "���������� ������� (2)");
            bCAndCConditionNode.Click += new EventHandler(CAndCConditionNode);
            editMenu.SubItems.Add(bCAndCConditionNode);

            ButtonItem bCAndCActionNode = new ButtonItem("bCAndCActionNode", "���������� ������� (3)");
            bCAndCActionNode.Click += new EventHandler(CAndCActionNode);
            editMenu.SubItems.Add(bCAndCActionNode);

            // ��ʼ����ͼ��˵�
            ButtonItem bRefreshCanvas = new ButtonItem("bRefreshCancas", "ˢ��(F5)");
            bRefreshCanvas.Click += new EventHandler(RefreshCanvas);
            canvasMenu.SubItems.Add(bRefreshCanvas);

            ButtonItem bResetGraphElementID = new ButtonItem("bResetGraphElementID", "���·���ͼԪID");
            bResetGraphElementID.Click += new EventHandler(ResetGraphElementID);
            canvasMenu.SubItems.Add(bResetGraphElementID);

            ButtonItem bPasteGraphElement = new ButtonItem("bPasteGraphElement", "ճ�� (Ctrl + V)");
            bPasteGraphElement.Click += new EventHandler(PasteGraphElement);
            canvasMenu.SubItems.Add(bPasteGraphElement);

            ButtonItem bCreateEventNode = new ButtonItem("bCreateEventNode", "�����¼���� (1)");
            bCreateEventNode.BeginGroup = true;
            bCreateEventNode.Click += new EventHandler(CreateEventNode);
            canvasMenu.SubItems.Add(bCreateEventNode);
        }

        /// <summary>
        /// �����¼����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        protected virtual void CreateEventNode(object sender, EventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;
            flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.EventNode, Point.Empty, false);
        }

        /// <summary>
        /// ˢ�»�ͼ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        protected virtual void RefreshCanvas(object sender, EventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;
            GraphManager graphManager = flowChartManager.CurrentGraphManager;

            // ˢ������ͼԪ
            graphManager.CurrentCanvas.AbbreviatGraphElement = null;
            graphManager.CurrentCanvas.AbbreviateLine = null;
            
            // �ָ�Ĭ�ϰ���״̬
            graphManager.ControlMode = false;
            graphManager.ShiftMode = false;
            graphManager.AltMode = false;

            // ˢ�»�ͼ����
            graphManager.RefreshRegion();
            graphManager.RefreshCanvas();
        }

        /// <summary>
        /// ճ��ͼԪ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        protected virtual void PasteGraphElement(object sender, EventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            documentManager.PasteGraphElement();
        }

        /// <summary>
        /// �����������������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        protected virtual void CAndCConditionNode(object sender, EventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;
            flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.ConditionNode, Point.Empty, true);
        }

        /// <summary>
        /// ���������Ӷ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        protected virtual void CAndCActionNode(object sender, EventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;
            flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(GraphType.ActionNode, Point.Empty, true);
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        protected virtual void CopyGraphElement(object sender, EventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            documentManager.CopyGraphElement();
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        protected virtual void CutGraphElement(object sender, EventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            documentManager.CutGraphElement();
        }

        /// <summary>
        /// �༭ע��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        protected virtual void EditRemark(object sender, EventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            documentManager.EditRemark();
        }

        /// <summary>
        /// ���·���ͼԪ��ID
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        protected virtual void ResetGraphElementID(object sender, EventArgs e)
        {
            if (MessageBox.Show("ȷ�����·���ͼԪID��", "���·���ͼԪID", 
                                MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
            {
                DocumentManager documentManager = DocumentManager.GetDocumentManager();
                FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;
                GraphManager graphManager = flowChartManager.CurrentGraphManager;
                DataManager dataManager = flowChartManager.CurrentDataManager;

                graphManager.ResetGraphElementID();
                dataManager.ResetDataElementID();
                graphManager.RefreshCanvas();
            }            
        }

        /// <summary>
        /// ˢ���ӻ�ͼ�����ߵ�����
        /// </summary>
        /// <param name="innerChart">�ӻ�ͼ</param>
        protected virtual void RefreshInnerChartLine(InnerChart innerChart)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;
            GraphManager graphManager = flowChartManager.CurrentGraphManager;
            DataManager dataManager = flowChartManager.CurrentDataManager;
        }
        
        /// <summary>
        /// ����ͼԪ���
        /// </summary>
        /// <param name="graphType">ͼԪ����</param>
        /// <param name="location">ͼԪλ��</param>
        /// <param name="autoConnect">�Ƿ��Զ�����</param>
        protected void CreateNode(GraphType graphType, Point location, bool autoConnect)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;
            flowChartManager.CurrentGraphManager.CreateAbbreviateGraphElement(graphType, location, autoConnect);
        }

        /// <summary>
        /// ��ʼ���ű����ɲ��
        /// </summary>
        /// <param name="filePath">�ļ�·��</param>
        /// <param name="className">����</param>
        public void InitCompiler(string filePath, string className)
        {
            try
            {
                string fullPath = Path.Combine(Application.StartupPath, pluginPath);
                fullPath = Path.Combine(fullPath, filePath);
                Assembly assembly = Assembly.LoadFrom(fullPath);
                compiler = assembly.CreateInstance(className) as Compiler;
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("���ؽű������ļ� {0} ʱ�����쳣��\r\n{1}", filePath, ex.Message), "��ʼ���ű������ļ�",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// ע������Ԫ��Ϣ
        /// </summary>
        public void RegistDataElementInfo(string graphType, string filePath, string className)
        {
            string fullPath = Path.Combine(Application.StartupPath, pluginPath);
            fullPath = Path.Combine(fullPath, filePath);
            DataElementInfo dataElementInfo = new DataElementInfo(graphType, fullPath, className);
            dataElementInfoDirectory[graphType] = dataElementInfo;
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicConnect(FlowChartManager flowChartManager, object logicData)
        {
            object[] data = logicData as object[];
            SlotGraphElement slot = data[0] as SlotGraphElement;
            ConnectorGraphElement connector = data[1] as ConnectorGraphElement;
            ConnectorContainer line = connector.Line;

            if ((line.InSlotContainer != null && line.OutSlotContainer != null) || connector.Selected) // �����߿��Ƶ㴦�ڱ�ѡ��״̬
            {
                flowChartManager.CurrentGraphManager.EditDataElement(line);
            }

            return true;
        }

        /// <summary>
        /// �������ͼԪ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicDisconnect(FlowChartManager flowChartManager, object logicData)
        {
            return true;
        }

        /// <summary>
        /// ɾ��ͼԪ֮ǰ�Ĵ���
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicBeforeDelete(FlowChartManager flowChartManager, object logicData)
        {
            return true;
        }

        /// <summary>
        /// ɾ��ͼԪ֮��Ĵ���
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicAfterDelete(FlowChartManager flowChartManager, object logicData)
        {
            return true;
        }

        /// <summary>
        /// �༭ͼԪ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicEdit(FlowChartManager flowChartManager, object logicData)
        {
            bool executeResult = true;

            GraphManager graphManager = flowChartManager.CurrentGraphManager;
            DataManager dataManager = flowChartManager.CurrentDataManager;
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            GraphElement graphElement = logicData as GraphElement;
            DataElement dataElement = dataManager.GetDataElement(graphElement);

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

            dataElement.PrintInformation = new DataElement.PrintInfo(documentManager.PrintText);

            try
            {
                executeResult = dataElement.EditData(information);
            }
            catch (Exception ex)
            {
                executeResult = false;

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

                if (string.IsNullOrEmpty(dataElement.Text))
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

            return executeResult;
        }

        /// <summary>
        /// ճ��ͼԪ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicPaste(FlowChartManager flowChartManager, object logicData)
        {
            return true;
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="headLocation">ͷ���λ��</param>
        /// <param name="tailLocation">β���λ��</param>
        /// <param name="id">������ID</param>
        /// <returns>������</returns>
        protected virtual ConnectorContainer CreateLine(Point headLocation, Point tailLocation, int id)
        {            
            LineGraphElement line = new LineGraphElement(headLocation, tailLocation);
            line.ID = id;
            line.Name = "������";
            line.Init();            

            return line;
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicCreateLine(FlowChartManager flowChartManager, object logicData)
        {            
            GraphManager graphManager = flowChartManager.CurrentGraphManager;

            object[] args = logicData as object[];
            int id = graphManager.AllocateGraphElementID();
            SlotGraphElement slot = args[0] as SlotGraphElement;
            Point p = (Point)args[1];            

            ConnectorContainer line = CreateLine(slot.Location, p, id);
            slot.Bind(line.TailConnector);            
            flowChartManager.CurrentGraphManager.SelectGraphElement(line, false);

            return true;
        }

        /// <summary>
        /// ����������������
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicCreateAndConnectLine(FlowChartManager flowChartManager, object logicData)
        {
            GraphManager graphManager = flowChartManager.CurrentGraphManager;

            object[] args = logicData as object[];
            int id = graphManager.AllocateGraphElementID();
            SlotGraphElement outSlot = args[0] as SlotGraphElement;
            SlotGraphElement inSlot = args[1] as SlotGraphElement;

            ConnectorContainer line = CreateLine(outSlot.Location, inSlot.Location, id);                        
            flowChartManager.CurrentGraphManager.SelectGraphElement(line, false);

            return true;
        }

        /// <summary>
        /// ������ͼ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicCreateChart(FlowChartManager flowChartManager, object logicData)
        {
            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            TreeView tree = logicData as TreeView;
            TreeNode currentNode = tree.SelectedNode;

            InputForm iForm = new InputForm("�������½���ͼ������", "�½���ͼ");

            if (iForm.ShowDialog() == DialogResult.OK)
            {
                TreeNode newNode = currentNode.Nodes.Add(iForm.InputText);
                newNode.ImageIndex = 1;
                newNode.SelectedImageIndex = 1;
                tree.SelectedNode = newNode;
                documentManager.CurrentTreeMode = EditMode.Normal;

                // �����ݿ���в���
                string path = documentManager.GetNodePath(newNode);
                string id = dataBaseManager.CreateNewDiagram(path, false, documentManager.CurrentChartMode);
                newNode.Tag = id;
            }

            return true;
        }

        /// <summary>
        /// ��ͼ����֮��Ĵ���
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicAfterLoad(FlowChartManager flowChartManager, object logicData)
        {
            return true;
        }

        /// <summary>
        /// ��ʼ������Ԫ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicInitializeDataElement(FlowChartManager flowChartManager, object logicData)
        {
            bool executeSuccess = true;
            GraphElement graphElement = logicData as GraphElement;
            DataElement dataElement = null;

            try
            {
                switch (graphElement.TypeString)
                {
                    case "InnerChart": // ����ͼԪ
                        {
                            dataElement = new InnerChartEditor();

                            break;
                        }
                    case "InterfaceNode": // ����ͼԪ
                        {
                            dataElement = new InterfaceNodeEditor();

                            break;
                        }
                    default: // �Զ���ͼԪ
                        {
                            DataElementInfo dataElementInfo = dataElementInfoDirectory[graphElement.TypeString];

                            if (dataElementInfo != null)
                            {
                                dataElement = dataElementInfo.GetDataElement();                                                                    
                            }

                            break;
                        }
                }

                if (dataElement != null)
                {
                    dataElement.ID = graphElement.ID;
                    dataElement.DataType = graphElement.TypeString;
                }
            }
            catch (Exception ex)
            {
                executeSuccess = false;
                MessageBox.Show("�ڳ�ʼ������Ԫʱ�����쳣: " + ex.ToString(), "����Ԫ��ʼ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            flowChartManager.CurrentDataManager.InitializedDataElement = dataElement;

            return executeSuccess;
        }

        /// <summary>
        /// �����Ҽ��˵�
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicPopUpMenuStrip(FlowChartManager flowChartManager, object logicData)
        {
            Point popUpLocation = (Point)logicData;
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            GraphManager graphManager = flowChartManager.CurrentGraphManager;

            if (graphManager.SelectedGraphElement is SlotContainer) // ��AI���������ѡ��
            {                
                editMenu.Popup(popUpLocation);
            }
            else if (graphManager.SelectedGraphElement == null) // û��ѡ���κ�ͼԪ
            {
                if (documentManager.CopyTable != null)
                {
                    canvasMenu.SubItems["bPasteGraphElement"].Enabled = true;
                }
                else
                {
                    canvasMenu.SubItems["bPasteGraphElement"].Enabled = false;
                }

                if (flowChartManager.MapName == "�ӻ�ͼ") // �ӻ�ͼ����������ͼԪID
                {
                    canvasMenu.SubItems["bResetGraphElementID"].Enabled = false;
                }
                else
                {
                    canvasMenu.SubItems["bResetGraphElementID"].Enabled = true;
                }

                canvasMenu.Popup(popUpLocation);
            }

            return true;
        }

        /// <summary>
        /// ��ָ���㴴�����
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <return>�Ƿ�����ɹ�</return>
        protected virtual bool LogicCreateNodeAtPoint(FlowChartManager flowChartManager, object logicData)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
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
                        else
                        {
                            CreateNode(GraphType.ConditionNode, location, false);
                        }

                        break;
                    }
                case 3: // �����������
                    {
                        if (flowChartManager.CurrentGraphManager.SelectedGraphElement is SlotContainer) // �������п������ӵĽ��Ļ����ϴ���
                        {
                            CreateNode(GraphType.ActionNode, location, true);
                        }
                        else 
                        {
                            CreateNode(GraphType.ActionNode, location, false);
                        }

                        break;
                    }
                default:
                    {
                        break;
                    }                    
            }

            return true;
        }

        /// <summary>
        /// �򿪽ű�
        /// </summary>
        /// <param name="flowChartManager">�߼�������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicViewCode(FlowChartManager flowChartManager, object logicData)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            Helper helper = Helper.GetHelper();
            string relevatePath = logicData as string;
            string path = Path.Combine(helper.OutputDir, string.Format("{0}.lua", relevatePath));
            
            if (File.Exists(path)) // �ļ�����
            {
                try
                {
                    System.Diagnostics.Process.Start(path);
                }
                catch (IOException ex)
                {
                    MessageBox.Show("�ڲ鿴�ű�ʱ����IO�쳣��" + ex.Message, "�򿪽ű�",
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show(string.Format("�ű��ļ�{0}�����ڣ�", path), "�򿪽ű�",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return true;
        }

        /// <summary>
        /// ���ļ���
        /// </summary>
        /// <param name="flowChartManager">�߼�������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicViewFolder(FlowChartManager flowChartManager, object logicData)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            Helper helper = Helper.GetHelper();
            TreeNode currentNode = logicData as TreeNode;

            string path = Path.Combine(helper.OutputDir, documentManager.GetNodePath(currentNode));
                        
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
        /// �����ͼ
        /// </summary>
        /// <param name="flowChartManager">�߼�������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicSave(FlowChartManager flowChartManager, object logicData)
        {
            return true;
        }

        /// <summary>
        /// ���ɽű�
        /// </summary>
        /// <param name="flowChartManager">�߼�������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicCompileCode(FlowChartManager flowChartManager, object logicData)
        {
            bool executeSuccess = GenerateCode();

            return executeSuccess;
        }

        /// <summary>
        /// ����߼�
        /// </summary>
        /// <param name="flowChartManager">�߼�������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicCheckLogic(FlowChartManager flowChartManager, object logicData)
        {
            return true;
        }

        /// <summary>
        /// �������
        /// </summary>
        /// <param name="flowChartManager">�߼�������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected virtual bool LogicManageArguments(FlowChartManager flowChartManager, object logicData)
        {
            return true;
        }

        /// <summary>
        /// ִ���߼�����
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicType">�߼�����</param>
        /// <param name="logicData">�߼�����</param>
        /// <return>ִ�н��</return>
        public virtual bool DoLogicOperation(FlowChartManager flowChartManager, LogicType logicType, object logicData)
        {
            bool executeSuccess = true;

            switch (logicType)
            {
                case LogicType.Connect: // ����ͼԪ
                    {
                        executeSuccess = LogicConnect(flowChartManager, logicData);
                        break;
                    }
                case LogicType.Disconnect: // �������ͼԪ
                    {
                        executeSuccess = LogicDisconnect(flowChartManager, logicData);
                        break;
                    }
                case LogicType.BeforeDelete: // ɾ��ͼԪ֮ǰ
                    {
                        executeSuccess = LogicBeforeDelete(flowChartManager, logicData);
                        break;
                    }
                case LogicType.AfterDelete: // ɾ��ͼԪ֮��
                    {
                        executeSuccess = LogicAfterDelete(flowChartManager, logicData);
                        break;
                    }
                case LogicType.Edit: // �༭ͼԪ
                    {
                        executeSuccess = LogicEdit(flowChartManager, logicData);
                        break;
                    }
                case LogicType.Paste: // ճ��ͼԪ
                    {
                        executeSuccess = LogicPaste(flowChartManager, logicData);
                        break;
                    }
                case LogicType.CreateLine: // ����������
                    {
                        executeSuccess = LogicCreateLine(flowChartManager, logicData);
                        break;
                    }
                case LogicType.CreateAndConnectLine: // ����������������
                    {
                        executeSuccess = LogicCreateAndConnectLine(flowChartManager, logicData);
                        break;
                    }
                case LogicType.CreateChart: // ������ͼ
                    {
                        executeSuccess = LogicCreateChart(flowChartManager, logicData);
                        break;
                    }
                case LogicType.AfterLoad: // ���ػ�ͼ֮��Ĵ���
                    {
                        executeSuccess = LogicAfterLoad(flowChartManager, logicData);
                        break;
                    }
                case LogicType.InitializeDataElement: // ��ʼ������Ԫ
                    {
                        executeSuccess = LogicInitializeDataElement(flowChartManager, logicData);
                        break;
                    }
                case LogicType.PopUpContextMenu: // �����Ҽ��˵�
                    {
                        executeSuccess = LogicPopUpMenuStrip(flowChartManager, logicData);
                        break;
                    }
                case LogicType.CreateAtPoint: // ��ָ���㴴��ͼԪ
                    {
                        executeSuccess = LogicCreateNodeAtPoint(flowChartManager, logicData);
                        break;
                    }
                case LogicType.ViewCode: // �򿪽ű�
                    {
                        executeSuccess = LogicViewCode(flowChartManager, logicData);
                        break;
                    }
                case LogicType.ViewFolder: // ���ļ���
                    {
                        executeSuccess = LogicViewFolder(flowChartManager, logicData);
                        break;
                    }
                case LogicType.Save: // �����ͼ
                    {
                        executeSuccess = LogicSave(flowChartManager, logicData);
                        break;
                    }
                case LogicType.CompileCode: // ���ɽű�
                    {
                        executeSuccess = LogicCompileCode(flowChartManager, logicData);
                        break;
                    }
                case LogicType.CheckLogic: // ����߼�
                    {
                        executeSuccess = LogicCheckLogic(flowChartManager, logicData);
                        break;
                    }
                case LogicType.ManageArguments: // �������
                    {
                        executeSuccess = LogicManageArguments(flowChartManager, logicData);
                        break;
                    }             
                default:
                    {
                        executeSuccess = false;
                        break;
                    }
            }

            return executeSuccess;
        }

        /// <summary>
        /// ��ȡͼԪ��������
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="slotContainerList">�����������</param>
        /// <returns>ͼԪ��������</returns>
        protected virtual List<FlowChartMetaData> GetMetaData(DataManager dataManager, List<SlotContainer> slotContainerList)
        {
            List<FlowChartMetaData> metaDataList = new List<FlowChartMetaData>();
            List<GraphElement> graphElementList = new List<GraphElement>();
            Hashtable graphElementTable = new Hashtable();

            foreach (SlotContainer slotContainer in slotContainerList)
            {
                if (slotContainer.IsEntrance)
                {
                    object data = dataManager.GetData(slotContainer);
                    DataElement dataElement = dataManager.GetDataElement(slotContainer);
                    FlowChartMetaData metaData = new FlowChartMetaData(slotContainer.ID, data, slotContainer.TypeString);
                    metaData.DisplayText = slotContainer.TooltipText;
                    FindNextMetaData(dataManager, slotContainer, metaData, graphElementList, graphElementTable);
                    metaDataList.Add(metaData);
                }
            }

            return metaDataList;
        }

        /// <summary>
        /// ��ȡ��һ��ͼԪ����
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="metaData">ͼԪ����</param>
        /// <param name="graphElementList">��������ͼԪ����</param>
        /// <param name="graphElementTable">��ͼ������ϣ��</param>
        private void FindNextMetaData(DataManager dataManager, GraphElement graphElement, FlowChartMetaData metaData, List<GraphElement> graphElementList, Hashtable graphElementTable)
        {
            graphElementList.Add(graphElement);
            graphElementTable[graphElement] = metaData;
            List<GraphElement> list = dataManager.GetNextGraphElements(graphElement, false);

            foreach (GraphElement g in list)
            {
                object data = dataManager.GetData(g);

                if (!graphElementList.Contains(g))
                {
                    DataElement dataElement = dataManager.GetDataElement(g);
                    FlowChartMetaData newMetaData = new FlowChartMetaData(g.ID, data, dataElement.DataType);
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
        }

        /// <summary>
        /// ���ɽű�
        /// </summary>
        /// <returns>�Ƿ����ɳɹ�</returns>
        protected virtual bool GenerateCode()
        {
            bool success = true;
            Helper helper = Helper.GetHelper();
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            Hashtable fileTable = documentManager.FileTable;
            FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;

            if (flowChartManager == null)
            {
                return false;
            }

            string id = flowChartManager.ID;
            GraphManager graphManager = flowChartManager.CurrentGraphManager;
            DataManager dataManager = flowChartManager.CurrentDataManager;

            List<SlotContainer> slotContainerList = graphManager.SlotContainerList;
            List<FlowChartMetaData> metaDataList = GetMetaData(dataManager, slotContainerList);

            if (compiler  == null)
            {
                return false;
            }

            compiler.PrintDebugInformation = documentManager.PrintText;
            compiler.SaveTheFile = documentManager.SaveTextToFile;
            FlowChartMetaDataManager flowChartMetaDataManager = new FlowChartMetaDataManager(int.Parse(id), metaDataList);
            fileTable.Clear();
            documentManager.PrintText("------ ��ʼ��ʼ���ű� ------");

            // �������ɽű�����
            compiler.RootDirectory = helper.OutputDir;
            compiler.ScriptPath = flowChartManager.CurrentPath;
            flowChartMetaDataManager.MapName = flowChartManager.MapName;
            flowChartMetaDataManager.Version = flowChartMetaDataManager.Version;

            try
            {
                success = compiler.GenerateCode(int.Parse(id), metaDataList, flowChartMetaDataManager);
            }
            catch (Exception ex) // �׳��쳣�����������Ϣ
            {
                documentManager.PrintText(string.Format("�쳣��Ϣ��{0}", ex.Message));                
                success = false;
            }

            if (!success)
            {
                documentManager.PrintText("��ʼ���ű�ʧ�ܣ�");
                return false;
            }            

            documentManager.PrintText("��ʼ���ű��ɹ���");

            // д�ļ�
            documentManager.PrintText("------ ��ʼ���ɽű� ------");
                    
            foreach (string fileName in fileTable.Keys)
            {
                success = helper.SaveFile(fileName, fileTable[fileName] as string);

                if (success) // д�ļ��ɹ�
                {
                    documentManager.PrintText(string.Format("���ɽű��ļ� {0} �ɹ���", fileName));
                }
                else
                {
                    documentManager.PrintText(string.Format("���ɽű��ļ� {0} ʧ�ܣ�", fileName));
                    break;
                }
            }

            if (!success)
            {
                return false;
            }

            documentManager.PrintText(string.Format("====== �����ļ����ɹ����ɣ�������{0}���ļ� ======", fileTable.Keys.Count));                              
                                       
            return success;
        }

        /// <summary>
        /// ����Ԫ��Ϣ
        /// </summary>
        protected class DataElementInfo
        {
            private static Dictionary<string, Assembly> assemblyDictionary = new Dictionary<string, Assembly>();
            private readonly string graphType;
            private readonly string filePath;
            private readonly string className;

            public DataElementInfo(string graphType, string filePath, string className)
            {
                this.graphType = graphType;
                this.filePath = filePath;
                this.className = className;
            }

            public string GraphType
            {
                get
                {
                    return graphType;
                }
            }

            public string FilePath
            {
                get
                {
                    return filePath;
                }
            }

            public string ClassName
            {
                get
                {
                    return className;
                }
            }

            /// <summary>
            /// ��ȡ����Ԫ
            /// </summary>
            /// <returns>����Ԫʵ��</returns>
            public DataElement GetDataElement()
            {
                DataElement dataElement = null;

                if (assemblyDictionary.ContainsKey(graphType))
                {
                    Assembly assembly = assemblyDictionary[graphType];
                    
                    if (assembly != null)
                    {
                        dataElement = assembly.CreateInstance(className) as DataElement;
                    }
                }
                else
                {
                    if (File.Exists(filePath))
                    {
                        Assembly assembly = Assembly.LoadFrom(filePath);

                        if (assembly != null)
                        {
                            dataElement = assembly.CreateInstance(className) as DataElement;
                            assemblyDictionary[graphType] = assembly;
                        }                        
                    }
                }

                return dataElement;
            }
        }
    }
}
