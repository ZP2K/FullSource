using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using System.Data;
using System.Collections;

using DevComponents.DotNetBar;
using DevComponents.DotNetBar.Controls;

using FlowChartEditor;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;
using DataElementLibrary;
using AIDiagramPlugin;
using FlowChartDataLibrary;
using Compiler_Sword3_AI;

namespace AIChartLogicManager
{
    public class LogicManager:LogicBaseManager
    {
        private int maxID = -1; // �Զ�����ĵ�ID

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
        /// ��ʼ������
        /// </summary>
        protected override void InitMenuStrip()
        {
            // ��ʼ���˵�����
            contextMenyBar.Items.Add(editMenu);
            contextMenyBar.Items.Add(canvasMenu);
            contextMenyBar.Style = eDotNetBarStyle.Office2003;

            // ��ʼ���༭�˵�
            ButtonItem bSetInitialState = new ButtonItem("bSetInitialState", "����Ϊ��̬");   
            bSetInitialState.Click += new EventHandler(SetInitialState);
            editMenu.SubItems.Add(bSetInitialState);

            ButtonItem bEditRemark = new ButtonItem("bEditRemark", "�༭ע��");
            bEditRemark.Click += new EventHandler(EditRemark);
            editMenu.SubItems.Add(bEditRemark);

            ButtonItem bCutGraphElement = new ButtonItem("bCutGraphElement", "���� (Ctrl + P)");
            bCutGraphElement.Click += new EventHandler(CutGraphElement);
            editMenu.SubItems.Add(bCutGraphElement);

            ButtonItem bCopyGraphElement = new ButtonItem("bCopyGraphElement", "���� (Ctrl + C)");
            bCopyGraphElement.Click += new EventHandler(CopyGraphElement);
            editMenu.SubItems.Add(bCopyGraphElement);

            ButtonItem bCAndCStateNode = new ButtonItem("bCAndCStateNode", "���������� ״̬��� (1)");
            bCAndCStateNode.BeginGroup = true;
            bCAndCStateNode.Click += new EventHandler(CAndCStateNode);
            editMenu.SubItems.Add(bCAndCStateNode);

            ButtonItem bCAndCActionNode = new ButtonItem("bCAndCActionNode", "���������� ������� (2)");
            bCAndCActionNode.Click += new EventHandler(CAndCActionNode);
            editMenu.SubItems.Add(bCAndCActionNode);

            ButtonItem bCAndCActionsNode = new ButtonItem("bCAndCActionsNode", "���������� �������� (3)");
            bCAndCActionsNode.Click += new EventHandler(CAndCActionsNode);
            editMenu.SubItems.Add(bCAndCActionsNode);

            ButtonItem bCAndCInnerChartNode = new ButtonItem("bCAndCInnerChartNode", "���������� �ӻ�ͼ��� (4)");
            bCAndCInnerChartNode.Click += new EventHandler(CAndCInnerChartNode);
            editMenu.SubItems.Add(bCAndCInnerChartNode);

            ButtonItem bCAndCInterfaceNode = new ButtonItem("bCAndCInterfaceNode", "���������� �ӿڽ�� 5)");
            bCAndCInterfaceNode.Click += new EventHandler(CAndCInterfaceNode);
            editMenu.SubItems.Add(bCAndCInterfaceNode);

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

            ButtonItem bCreateStateNode = new ButtonItem("bCreateStateNode", "����״̬��� (1)");
            bCreateStateNode.BeginGroup = true;
            bCreateStateNode.Click += new EventHandler(CreateStateNode);
            canvasMenu.SubItems.Add(bCreateStateNode);

            ButtonItem bCreateActionNode = new ButtonItem("bCreateActionNode", "����������� (2)");
            bCreateActionNode.Click += new EventHandler(CreateActionNode);
            canvasMenu.SubItems.Add(bCreateActionNode);

            ButtonItem bCreateActionsNode = new ButtonItem("bCreateActionsNode", "������������ (3)");
            bCreateActionsNode.Click += new EventHandler(CreateActionsNode);
            canvasMenu.SubItems.Add(bCreateActionsNode);

            ButtonItem bCreateInnerChartNode = new ButtonItem("bCreateInnerChartNode", "�����ӻ�ͼ��� (4)");
            bCreateInnerChartNode.Click += new EventHandler(CreateInnerChartNode);
            canvasMenu.SubItems.Add(bCreateInnerChartNode);

            ButtonItem bCreateInterfaceNode = new ButtonItem("bCreateInterfaceNode", "�����ӿڽ�� (5)");
            bCreateInterfaceNode.Click += new EventHandler(CreateInterfaceNode);
            canvasMenu.SubItems.Add(bCreateInterfaceNode);
        }
        
        /// <summary>
        /// �����ӿڽ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CreateInterfaceNode(object sender, EventArgs e)
        {            
            CreateNode(GraphType.InterfaceNode, Point.Empty, false);
        }

        /// <summary>
        /// �����ӻ�ͼ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CreateInnerChartNode(object sender, EventArgs e)
        {            
            CreateNode(GraphType.InnerChart, Point.Empty, false);
        }

        /// <summary>
        /// ������������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CreateActionsNode(object sender, EventArgs e)
        {            
            CreateNode(GraphType.AIActionsNode, Point.Empty, false);
        }

        /// <summary>
        /// �����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CreateActionNode(object sender, EventArgs e)
        {            
            CreateNode(GraphType.AIActionNode, Point.Empty, false);
        }

        /// <summary>
        /// ����״̬���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CreateStateNode(object sender, EventArgs e)
        {
            CreateNode(GraphType.AIStateNode, Point.Empty, false);
        }

        /// <summary>
        /// ���������ӽӿڽ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CAndCInterfaceNode(object sender, EventArgs e)
        {
            CreateNode(GraphType.InterfaceNode, Point.Empty, true);
        }

        /// <summary>
        ///  �����������ӻ�ͼ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CAndCInnerChartNode(object sender, EventArgs e)
        {
            CreateNode(GraphType.InnerChart, Point.Empty, true);
        }

        /// <summary>
        /// ���������Ӷ�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CAndCActionsNode(object sender, EventArgs e)
        {            
            CreateNode(GraphType.AIActionsNode, Point.Empty, true);
        }

        /// <summary>
        /// ���������Ӷ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CAndCActionNode(object sender, EventArgs e)
        {
            CreateNode(GraphType.AIActionNode, Point.Empty, true);
        }

        /// <summary>
        /// ����������״̬���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void CAndCStateNode(object sender, EventArgs e)
        {
            CreateNode(GraphType.AIStateNode, Point.Empty, true);
        }

        /// <summary>
        /// ����Ϊ��̬
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void SetInitialState(object sender, EventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;
            GraphManager graphManager = flowChartManager.CurrentGraphManager;
            AIStateGraphElement aiStateGraphElement = graphManager.SelectedGraphElement as AIStateGraphElement;

            foreach (SlotContainer slotContainer in graphManager.SlotContainerList)
            {
                if (slotContainer is AIStateGraphElement)
                {
                    AIStateGraphElement stateNode = slotContainer as AIStateGraphElement;
                    stateNode.IsInitialState = false;                    
                }
            }

            aiStateGraphElement.IsInitialState = true;
            graphManager.RefreshCanvas();
        }

        /// <summary>
        /// ���·���ͼԪ��ID
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        protected override void ResetGraphElementID(object sender, EventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;
            GraphManager graphManager = flowChartManager.CurrentGraphManager;
            DataManager dataManager = flowChartManager.CurrentDataManager;

            base.ResetGraphElementID(sender, e);                        

            foreach (SlotContainer node in graphManager.SlotContainerList)
            {
                RenameGraphElement(dataManager, node);
            }            
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

            foreach (GraphElement graphElement in graphElementList) // ����������ϵ�����
            {
                if (graphElement is ConnectorContainer)
                {
                    ConnectorContainer line = graphElement as ConnectorContainer;
                    DataElement dataElement = dataManager.GetDataElement(line);

                    if (dataElement != null)
                    {
                        dataElement.Data = null;
                    }

                    dataManager.SetData(line, null);

                    // �����߻ָ��ɼ�״̬
                    line.Moving = false;
                    line.Visible = true;
                    graphElement.ShowText = false;
                }
            }

            return true;
        }

        /// <summary>
        /// ճ��ͼԪ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicPaste(FlowChartManager flowChartManager, object logicData)
        {
            DataManager dataManager = flowChartManager.CurrentDataManager;

            List<GraphElement> graphElementList = logicData as List<GraphElement>;
            dataManager.InitCompileTable();

            foreach (GraphElement graphElement in graphElementList)
            {
                RenameGraphElement(dataManager, graphElement);
            }

            return true;
        }

        /// <summary>
        /// ������ͼ
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicCreateChart(FlowChartManager flowChartManager, object logicData)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            TreeView tree = logicData as TreeView;

            TreeNode currentNode = tree.SelectedNode;
            string path = documentManager.GetNodePath(currentNode);

            CreateChartForm cForm = new CreateChartForm(path);

            if (cForm.ShowDialog() == DialogResult.OK)
            {
                string chartID = cForm.ChartID;
                string chartName = cForm.ChartName;

                TreeNode newNode = currentNode.Nodes.Add(chartName);
                newNode.Name = chartName;
                newNode.ImageIndex = 1;
                newNode.SelectedImageIndex = 1;
                newNode.Tag = chartID;
                tree.SelectedNode = newNode;
                documentManager.CurrentTreeMode = EditMode.Normal;
            }

            return true;
        }

        /// <summary>
        /// ��ͼ����֮��Ĳ���
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicAfterLoad(FlowChartManager flowChartManager, object logicData)
        {
            return true;
        }

        /// <summary>
        /// �����Ҽ��˵�
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicPopUpMenuStrip(FlowChartManager flowChartManager, object logicData)
        {
            GraphManager graphManager = flowChartManager.CurrentGraphManager;

            Point popUpLocation = (Point)logicData;
            DocumentManager documentManager = DocumentManager.GetDocumentManager();

            if (graphManager.SelectedGraphElement is SlotContainer) // ��AI���������ѡ��
            {
                if (graphManager.SelectedGraphElement is AIStateGraphElement)
                {
                    editMenu.SubItems["bCAndCStateNode"].Enabled = false;
                    editMenu.SubItems["bSetInitialState"].Enabled = true;
                }
                else
                {
                    editMenu.SubItems["bCAndCStateNode"].Enabled = true;
                    editMenu.SubItems["bSetInitialState"].Enabled = false;
                }

                if (flowChartManager.MapName == "�ӻ�ͼ")
                {
                    // �ӻ�ͼ�ڲ����ٴ����ӻ�ͼ���
                    editMenu.SubItems["bCAndCInnerChartNode"].Enabled = false;

                    // ֻ���ӻ�ͼ�ܴ����ӿڽ��
                    editMenu.SubItems["bCAndCInterfaceNode"].Enabled = true;
                }
                else
                {                    
                    editMenu.SubItems["bCAndCInnerChartNode"].Enabled = true;                    
                    editMenu.SubItems["bCAndCInterfaceNode"].Enabled = false;
                }

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

                if (flowChartManager.MapName == "�ӻ�ͼ") 
                {
                    // �ӻ�ͼ����������ͼԪID
                    canvasMenu.SubItems["bResetGraphElementID"].Enabled = false;
                    
                    // �ӻ�ͼ�ڲ����ٴ����ӻ�ͼ���
                    canvasMenu.SubItems["bCreateInnerChartNode"].Enabled = false;

                    // ֻ���ӻ�ͼ�ܴ����ӿڽ��
                    canvasMenu.SubItems["bCreateInterfaceNode"].Enabled = true;
                }
                else
                {
                    canvasMenu.SubItems["bResetGraphElementID"].Enabled = true;
                    canvasMenu.SubItems["bCreateInnerChartNode"].Enabled = true;
                    canvasMenu.SubItems["bCreateInterfaceNode"].Enabled = false;
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
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicCreateNodeAtPoint(FlowChartManager flowChartManager, object logicData)
        {
            GraphManager graphManager = flowChartManager.CurrentGraphManager;

            object[] args = logicData as object[];
            int graphType = (int)args[0];
            Point location = (Point)args[1];

            switch (graphType)
            {
                case 1: // ����AI״̬���
                    {
                        if ((graphManager.SelectedGraphElement is AISlotContainer) &&
                            !(graphManager.SelectedGraphElement is AIStateGraphElement))
                        {
                            CreateNode(GraphType.AIStateNode, location, true);
                        }
                        else
                        {
                            CreateNode(GraphType.AIStateNode, location, false);
                        }

                        break;
                    }
                case 2: // ����AI�������
                    {
                        if (graphManager.SelectedGraphElement is AISlotContainer)
                        {
                            CreateNode(GraphType.AIActionNode, location, true);
                        }
                        else
                        {
                            CreateNode(GraphType.AIActionNode, location, false);
                        }

                        break;
                    }
                case 3: // ����AI��������
                    {
                        if (graphManager.SelectedGraphElement is AISlotContainer)
                        {
                            CreateNode(GraphType.AIActionsNode, location, true);
                        }
                        else
                        {
                            CreateNode(GraphType.AIActionsNode, location, false);
                        }

                        break;
                    }
                case 4: // �����ӻ�ͼ���
                    {
                        if (graphManager.SelectedGraphElement is AISlotContainer)
                        {
                            CreateNode(GraphType.InnerChart, location, true);
                        }
                        else
                        {
                            CreateNode(GraphType.InnerChart, location, false);
                        }

                        break;
                    }
                case 5:// ���� �ӿڽ��
                    {
                        if (graphManager.SelectedGraphElement is AISlotContainer)
                        {
                            CreateNode(GraphType.InterfaceNode, location, true);
                        }
                        else
                        {
                            CreateNode(GraphType.InterfaceNode, location, false);
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
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicViewCode(FlowChartManager flowChartManager, object logicData)
        {
            Helper helper = Helper.GetHelper();
            string relevatePath = logicData as string;

            string[] data = relevatePath.Split(new char[] { '\\' });
            string shortPath = data[1];

            for (int i = 2; i < data.Length; i++)
            {
                shortPath += string.Format("\\{0}", data[i]);
            }

            string path = string.Format(@"{0}\scripts\Map\{1}\ai\{2}.lua", helper.OutputDir, data[0], shortPath);            

            if (File.Exists(path)) // �ļ�����
            {
                try
                {
                    System.Diagnostics.Process.Start(path);
                }
                catch (IOException ex)
                {
                    MessageBox.Show("�ڲ鿴�ű��ļ�ʱ����IO�쳣��" + ex.Message, "�򿪽ű�",
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", path), "�򿪽ű�",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
            }  

            return true;
        }

        /// <summary>
        /// ���ļ���
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicViewFolder(FlowChartManager flowChartManager, object logicData)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            Helper helper = Helper.GetHelper();
            TreeNode currentNode = logicData as TreeNode;

            string currentPath = documentManager.GetNodePath(currentNode);
            string[] data = currentPath.Split(new char[] { '\\' });
            string path = "";
            char[] pathSymbleArray = new char[] { '\\' };

            if (data.Length > 1) // �鿴������Ŀ¼
            {
                string shortPath = data[1];

                for (int i = 2; i < data.Length; i++)
                {
                    shortPath += string.Format("\\{0}", data[i]);
                }

                path = string.Format(@"{0}\scripts\Map\{1}\ai\{2}", helper.OutputDir.TrimEnd(pathSymbleArray), data[0], shortPath);
            }
            else
            {
                path = string.Format(@"{0}\scripts\Map\{1}\ai", helper.OutputDir.TrimEnd(pathSymbleArray), data[0]);
            }
            
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
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicSave(FlowChartManager flowChartManager, object logicData)
        {
            if (flowChartManager.CurrentPath.StartsWith("�ӻ�ͼ"))
            {
                string description = flowChartManager.CustomData["description"] as string;

                if (string.IsNullOrEmpty(description))
                {
                    description = flowChartManager.Name;
                }

                InputForm inputForm = new InputForm("�������ӻ�ͼ������", description);

                if (inputForm.ShowDialog() == DialogResult.OK)
                {
                    description = inputForm.InputText;
                    flowChartManager.CustomData["description"] = description;
                }
            }

            return true;
        }

        /// <summary>
        /// ���ɽű�
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicCompileCode(FlowChartManager flowChartManager, object logicData)
        {
            bool executeSuccess = GenerateCode();

            // ���ű�����
            string[] dataArray = flowChartManager.CurrentPath.Split(new char[] { '\\' });
            string shortPath = dataArray[1];

            for (int i = 2; i < dataArray.Length; i++)
            { 
                shortPath = shortPath + string.Format("\\{0}", dataArray[i]);
            }

            string fullPath = string.Format(@"{0}\scripts\Map\{1}\ai\{2}", Helper.GetHelper().OutputDir.TrimEnd(new char[] { '\\' }), dataArray[0], shortPath);
            string fileName = string.Format("{0}.lua", fullPath);
            string content = Helper.GetHelper().ReadFile(fileName);

            if (!AIDiagramHelper.CheckScript(null, content))
            {
                MessageBox.Show("û��ͨ���ű���飬�������ʾ��Ϣ���AI�ű��Ƿ���ȷ��", "�ű����",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return executeSuccess;
        }

        /// <summary>
        /// ����߼�
        /// </summary>
        /// <param name="flowChartManager">��ͼ����</param>
        /// <param name="logicData">�߼�����</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        protected override bool LogicCheckLogic(FlowChartManager flowChartManager, object logicData)
        {
            bool executeResult = CheckAIChartAvail(flowChartManager);

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
            bool executeResult = true;

            if (flowChartManager == null || flowChartManager.CurrentChartMode != displayName)
            {
                MessageBox.Show("��ǰû��AIͼ�����", "��������", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);

                executeResult = false;
            }
            else
            {
                TemplateParmForm tForm = new TemplateParmForm();
                DialogResult editResult = tForm.ShowDialog();

                if (editResult == DialogResult.OK)
                {
                    flowChartManager.ContentChanged = true;
                }
            }

            return executeResult;
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="headLocation">ͷ���λ��</param>
        /// <param name="tailLocation">β���λ��</param>
        /// <param name="id">������ID</param>
        /// <returns>������</returns>
        protected override ConnectorContainer CreateLine(Point headLocation, Point tailLocation, int id)
        {
            AILineGraphElement line = new AILineGraphElement(headLocation, tailLocation);
            line.ID = id;
            line.Name = "������";
            line.Init();

            return line;
        }

        /// <summary>
        /// ������ͼԪ
        /// </summary>
        /// <param name="graphElement">���ݹ�����</param>
        /// <param name="graphElement">ͼԪ</param>
        private void RenameGraphElement(DataManager dataManager, GraphElement graphElement)
        {
            object data = dataManager.GetCompileData(graphElement);

            if (graphElement is AIStateGraphElement) // �Զ�Ϊ״̬���������
            {
                AI_State aiState = data as AI_State;

                if (aiState != null)
                {
                    aiState.Ename = string.Format("State{0}", graphElement.ID.ToString());
                }
            }
            else if (graphElement is AIActionGraphElement) // �Զ�Ϊ�Զ���ű��Ķ������������
            {
                AI_SingleAction singleAction = data as AI_SingleAction;

                if (singleAction != null)
                {
                    string newName = string.Format("CustomAction{0}", graphElement.ID.ToString());
                    int actionID = 2000 + graphElement.ID;
                    RenameAIAction(singleAction.Action, newName, actionID);
                }
            }
            else if (graphElement is AIActionsGraphElement) // �Զ�Ϊ���������Զ���ű��Ķ������������
            {                
                AI_MultiAction multiAction = data as AI_MultiAction;

                if (multiAction != null)
                {
                    int actionIndex = 0;
                    List<AI_SingleAction> aiActionList = multiAction.ActionList;

                    foreach (AI_SingleAction singleAction in aiActionList)
                    {
                        string newName = string.Format("CustomAction{0}_{1}", graphElement.ID.ToString(),
                                                        actionIndex.ToString());
                        int actionID = (2000 + graphElement.ID) * 10 + actionIndex;
                        RenameAIAction(singleAction.Action, newName, actionID);
                        actionIndex++;
                    }
                }
            } 
        }

        /// <summary>
        /// ������AI����
        /// </summary>
        /// <param name="singleAction">AI����</param>
        /// <param name="newName">�µĶ�����</param>
        /// <param name="actionID">�µĶ���ID</param>
        private void RenameAIAction(AI_Action aiAction, string newName, int actionID)
        {
            if (int.Parse(aiAction.ActionID) > 2000) // �ű��Զ��嶯������Ҫ������
            {
                string script = aiAction.Info;
                string actionName = aiAction.Ename;
                aiAction.Info = script.Replace(aiAction.Ename, newName);
                aiAction.Ename = newName;
                aiAction.ActionID = actionID.ToString();
            }
        }

        /// <summary>
        /// ��ȡ�������Ԫ��������
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="aiActionsGraphElement">������ͼԪ</param>
        /// <returns>�������Ԫ��������</returns>
        private List<FlowChartMetaData> GetActionsMetaDataList(DataManager dataManager, AIActionsGraphElement aiActionsGraphElement)
        {
            object data = dataManager.GetCompileData(aiActionsGraphElement);

            List<object> actions = CodeProviderClass.SplitMultiActionIntoSingle(data);
            List<FlowChartMetaData> actionsMetaDataList = new List<FlowChartMetaData>();

            // ����ʾ�ı����
            List<string> displayTextList = new List<string>();
            string[] tempArray = aiActionsGraphElement.Text.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
            foreach (string s in tempArray)
            {
                displayTextList.Add(s.TrimEnd(new char[] { '\r' }).TrimStart(new char[] { '��', ' ' }));
            }

            for (int i = 0; i < actions.Count; i++)
            {
                FlowChartMetaData actionMetaData = new FlowChartMetaData(maxID, actions[i], "AIActionNode");
                actionMetaData.DisplayText = displayTextList[i];
                actionsMetaDataList.Add(actionMetaData);
                maxID++;
            }

            for (int i = 0; i < actions.Count - 1; i++)
            {
                FlowChartMetaData actionMetaData = actionsMetaDataList[i];
                FlowChartMetaData nextMetaData = actionsMetaDataList[i + 1];

                FlowChartMetaData lineMetaData;
                int returnValueCount = CodeProviderClass.GetActionRetCount(actions[i]);

                for (int j = 1; j <= returnValueCount; j++)
                {
                    lineMetaData = new FlowChartMetaData(maxID, j.ToString(), "AILine");
                    lineMetaData.DisplayText = j.ToString();
                    actionMetaData.AddNextMetaData(lineMetaData);
                    lineMetaData.AddPreviousMetaData(actionMetaData);
                    lineMetaData.AddNextMetaData(nextMetaData);
                    nextMetaData.AddPreviousMetaData(lineMetaData);

                    maxID++;
                }
            }

            return actionsMetaDataList;
        }

        /// <summary>
        /// ��ȡ����ͼԪ����
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="slotContainerList">����ͼ�������������</param>
        /// <returns>����ͼԪ���ݵ�����</returns>
        protected override List<FlowChartMetaData> GetMetaData(DataManager dataManager, List<SlotContainer> slotContainerList)
        {
            // ��ʼ��ID
            maxID = dataManager.CurrentFlowChartManager.CurrentGraphManager.IDIndex + 1;

            List<FlowChartMetaData> metaDataList = new List<FlowChartMetaData>();
            List<GraphElement> graphElementList = new List<GraphElement>();
            Hashtable graphElementTable = new Hashtable();
            dataManager.InitCompileTable();

            Hashtable actionsTable = new Hashtable(); // ���������ݹ�ϣ��

            // Ԥ����������
            foreach (SlotContainer slotContainer in slotContainerList)
            {
                if (slotContainer is AIActionsGraphElement)
                {
                    AIActionsGraphElement actionsGraphElement = slotContainer as AIActionsGraphElement;
                    List<FlowChartMetaData> actionsMetaDataList = GetActionsMetaDataList(dataManager, actionsGraphElement);
                    actionsTable[actionsGraphElement] = actionsMetaDataList;
                }
            }

            // Ԥ�����ӻ�ͼ���
            AIStateGraphElement stateNode = null;            

            foreach (SlotContainer slotContainer in slotContainerList)
            {
                // �����ӻ�ͼ��ͼԪ�����·���ID
                if (slotContainer is InnerChart)
                {
                    InnerChart innerChart = slotContainer as InnerChart;
                    InnerChartEditor innerChartEditor = dataManager.GetDataElement(innerChart) as InnerChartEditor;
                    List<GraphElement> list = innerChartEditor.GraphElementList;

                    foreach (GraphElement graphElement in list)
                    {
                        graphElement.ID = maxID;
                        graphElement.Tag = innerChart.ID;
                        maxID++;

                        dataManager.AddCompileData(graphElement, innerChartEditor.GetDataElement(graphElement));

                        // ����������                        
                        RenameGraphElement(dataManager, graphElement);

                        if (graphElement is AIStateGraphElement)
                        {
                            AIStateGraphElement aiStateGraphElement = graphElement as AIStateGraphElement;

                            if (aiStateGraphElement.IsInitialState)
                            {
                                stateNode = aiStateGraphElement;
                            }
                        }
                        else if (graphElement is AIActionsGraphElement)
                        {
                            AIActionsGraphElement actionsGraphElement = graphElement as AIActionsGraphElement;
                            List<FlowChartMetaData> actionsMetaDataList = GetActionsMetaDataList(dataManager, actionsGraphElement);
                            actionsTable[actionsGraphElement] = actionsMetaDataList;
                        }
                    }
                }
            }

            // �����ͼ���
            foreach (SlotContainer slotContainer in slotContainerList)
            {
                if (slotContainer is AIStateGraphElement)
                {
                    AIStateGraphElement stateGraphElement = slotContainer as AIStateGraphElement;

                    if (stateGraphElement.IsInitialState)
                    {
                        stateNode = stateGraphElement;
                        break;
                    }                    
                }
            }

            if (stateNode != null)
            {
                object data = dataManager.GetCompileData(stateNode);
                DataElement dataElement = dataManager.GetCompileDataElement(stateNode);
                FlowChartMetaData metaData = new FlowChartMetaData(stateNode.ID, data, "AIStateNode");
                string stateNodeName = CodeProviderClass.GetStateString(data);

                metaData.Code = stateNodeName;
                metaData.DisplayText = stateNode.Text;
                FindNextMetaData(dataManager, stateNode, metaData, graphElementList, graphElementTable, actionsTable);
                metaDataList.Add(metaData);
            }

            return metaDataList;
        }

        /// <summary>
        /// ��ȡ����״̬�Ļ�ͼԪ����
        /// </summary>
        /// <param name="previousMetaData">ǰһ��Ԫ����</param>
        /// <param name="stateMetaData">״̬Ԫ����</param>
        private void InitSetStateMetaData(FlowChartMetaData previousMetaData, FlowChartMetaData stateMetaData)
        {
            FlowChartMetaData setStateMetaData = new FlowChartMetaData(maxID, null, "AISetStateNode");
            setStateMetaData.DisplayText = "����״̬";
            setStateMetaData.AddPreviousMetaData(previousMetaData);
            previousMetaData.AddNextMetaData(setStateMetaData);
            maxID++;

            FlowChartMetaData lineMetaData = new FlowChartMetaData(maxID, "1", "AILine");
            lineMetaData.DisplayText = "1";
            setStateMetaData.AddNextMetaData(lineMetaData);
            lineMetaData.AddPreviousMetaData(setStateMetaData);
            maxID++;

            lineMetaData.AddNextMetaData(stateMetaData);
            stateMetaData.AddPreviousMetaData(lineMetaData);            
        }

        /// <summary>
        /// ��ȡ��һ����ͼԪ����
        /// </summary>
        /// <param name="dataManager">���ݹ�����</param>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="metaData">��ͼԪ����</param>
        /// <param name="graphElementList">��������ͼԪ����</param>
        /// <param name="graphElementTable">����ͼ������ϣ��</param>
        /// <param name="actionsTable">���������ݹ�ϣ��</param>
        private void FindNextMetaData(DataManager dataManager, GraphElement graphElement, FlowChartMetaData metaData, List<GraphElement> graphElementList, Hashtable graphElementTable, Hashtable actionsTable)
        {
            graphElementList.Add(graphElement);
            graphElementTable[graphElement] = metaData;
            List<GraphElement> list = dataManager.GetNextGraphElements(graphElement, true);

            foreach (GraphElement g in list)
            {
                object data = dataManager.GetCompileData(g);
                DataElement dataElement =  dataManager.GetCompileDataElement(g);

                if (!graphElementList.Contains(g))
                {                    
                    FlowChartMetaData newMetaData = new FlowChartMetaData(g.ID, data, dataElement.DataType);
                    newMetaData.DisplayText = g.Text;                    
                    
                    if (g is AIStateGraphElement) // ״̬
                    {
                        string stateNodeName = CodeProviderClass.GetStateString(data);
                        newMetaData.Code = stateNodeName;

                        InitSetStateMetaData(metaData, newMetaData);
                        FindNextMetaData(dataManager, g, newMetaData, graphElementList, graphElementTable, actionsTable);
                    }
                    else if (g is AILineGraphElement) // ������
                    {
                        newMetaData.Code = CodeProviderClass.GetEventString(data);

                        newMetaData.AddPreviousMetaData(metaData);
                        metaData.AddNextMetaData(newMetaData);
                        FindNextMetaData(dataManager, g, newMetaData, graphElementList, graphElementTable, actionsTable);                        
                    }
                    else if (g is AIActionGraphElement) // ����
                    {
                        newMetaData.AddPreviousMetaData(metaData);
                        metaData.AddNextMetaData(newMetaData);
                        FindNextMetaData(dataManager, g, newMetaData, graphElementList, graphElementTable, actionsTable);
                    }
                    else if (g is AIActionsGraphElement) // ������
                    {                    
                        List<FlowChartMetaData> metaDataList = actionsTable[g] as List<FlowChartMetaData>;

                        metaDataList[0].AddPreviousMetaData(metaData);
                        metaData.AddNextMetaData(metaDataList[0]);

                        FindNextMetaData(dataManager, g, metaDataList[metaDataList.Count - 1], graphElementList, graphElementTable, actionsTable);
                    }
                }
                else
                {
                    if (g is AIActionsGraphElement) // ������Ҫ���⴦��
                    {
                        List<FlowChartMetaData> metaDataList = actionsTable[g] as List<FlowChartMetaData>;
                        metaDataList[0].AddPreviousMetaData(metaData);
                        metaData.AddNextMetaData(metaDataList[0]);
                    }
                    else if (g is AIStateGraphElement) // ״̬���Ҫ���⴦��
                    {
                        FlowChartMetaData newMetaData = graphElementTable[g] as FlowChartMetaData;

                        InitSetStateMetaData(metaData, newMetaData);
                    }
                    else
                    {
                        FlowChartMetaData newMetaData = graphElementTable[g] as FlowChartMetaData;
                        newMetaData.AddPreviousMetaData(metaData);
                        metaData.AddNextMetaData(newMetaData);
                    }                    
                }
            }                      
        }

        /// <summary>
        /// ���AIͼ����Ч��
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="showDetail">�Ƿ���ʾϸ��</param>
        /// <returns>AIͼ�Ƿ���Ч</returns>
        private bool CheckAIChartAvail(FlowChartManager flowChartManager)
        {
            bool avail = true;

            // ��������ߵ���Ч��
            avail = CheckLineAvail(flowChartManager);

            // ���ͼԪ����Ч��
            if (avail)
            {                
                avail = CheckAIGraphElementAvail(flowChartManager);
            }

            return avail;
        }

        /// <summary>
        /// ���ͼԪ�Ƿ���Ч
        /// </summary>
        /// <param name="dataManager">��ͼ������</param>
        /// <returns>ͼԪ�Ƿ���Ч</returns>
        private bool CheckAIGraphElementAvail(FlowChartManager flowChartManager)
        {
            GraphElement initialStateNode = null;
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            DataManager dataManager = flowChartManager.CurrentDataManager;

            foreach (GraphElement graphElement in flowChartManager.CurrentGraphManager.CurrentCanvas.GraphElementList)
            {
                if (dataManager.GetData(graphElement) == null)
                {
                    documentManager.PrintText(string.Format("IDΪ{0}��ͼԪû����ɱ༭��", graphElement.ID));

                    MessageBox.Show("��ͼ����û����ɱ༭��ͼԪ����ȷ�����е�ͼԪ������ȷ�༭����", "AIͼ��Ч�Լ��", 
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);

                    // ��ʾϸ��
                    List<GraphElement> list = new List<GraphElement>();
                    list.Add(graphElement);
                    dataManager.CurrentFlowChartManager.CurrentGraphManager.SelectGraphElements(list);

                    return false;
                }

                if (graphElement is AIStateGraphElement) // AI״̬���
                {
                    AIStateGraphElement stateNode = graphElement as AIStateGraphElement;

                    if(stateNode.IsInitialState)
                    {
                        initialStateNode = graphElement;
                    }

                    int stateEventCount = CodeProviderClass.GetStateEvtCount(dataManager.GetData(stateNode));

                    if(stateEventCount > dataManager.GetNextGraphElements(stateNode, false).Count)
                    {
                        documentManager.PrintText(string.Format("IDΪ{0}��AI״̬���û�д������е��¼���", graphElement.ID));
                    }

                    List<SlotContainer> list = dataManager.GetNextSlotContainers(stateNode);

                    foreach (SlotContainer slotContainer in list)
                    {
                        if (slotContainer is AIStateGraphElement)
                        {
                            documentManager.PrintText(string.Format("IDΪ{0}��AI״̬���ֱ��������״̬��㣡", graphElement.ID));                            

                            MessageBox.Show("��ͼ����״̬���ֱ��������״̬��㣬״̬���ֻ�����Ӷ�����㣡", "AIͼ��Ч�Լ��", 
                                            MessageBoxButtons.OK, MessageBoxIcon.Information);

                            // ��ʾϸ��
                            List<GraphElement> detailList = new List<GraphElement>();
                            detailList.Add(graphElement);
                            dataManager.CurrentFlowChartManager.CurrentGraphManager.SelectGraphElements(detailList);

                            return false;
                        }
                    }
                }
                else if (graphElement is AIActionGraphElement) // AI�������
                {
                    int actionBranchCount = CodeProviderClass.GetActionRetCount(dataManager.GetData(graphElement));

                    if (actionBranchCount > dataManager.GetNextGraphElements(graphElement, false).Count)
                    {
                        documentManager.PrintText(string.Format("IDΪ{0}��AI�������û�д������еķ���ֵ��", graphElement.ID));                        
                        
                        MessageBox.Show("��ͼ���ж������û�д������еķ���ֵ����ȷ��������ʵ�����еķ�֧��", "AIͼ��Ч�Լ��", 
                                        MessageBoxButtons.OK, MessageBoxIcon.Information);

                        // ��ʾϸ��
                        List<GraphElement> list = new List<GraphElement>();
                        list.Add(graphElement);
                        dataManager.CurrentFlowChartManager.CurrentGraphManager.SelectGraphElements(list);

                        return false;
                    }                    
                }
                else if (graphElement is InnerChart) // �ӻ�ͼ���
                {
                    if (initialStateNode == null)
                    {
                        InnerChart innerChart = graphElement as InnerChart;
                        InnerChartEditor innerChartEditor = dataManager.GetDataElement(innerChart) as InnerChartEditor;

                        List<GraphElement> graphElementList = innerChartEditor.GraphElementList;

                        foreach (GraphElement g in graphElementList)
                        {
                            if (g is AIStateGraphElement)
                            {
                                AIStateGraphElement stateNode = g as AIStateGraphElement;

                                if (stateNode.IsInitialState)
                                {
                                    initialStateNode = innerChart;
                                    break;
                                }
                            }
                        }
                    }                    
                }
            }

            if (flowChartManager.CurrentPath.StartsWith("�ӻ�ͼ")) // �ӻ�ͼ����Ҫ�ͳ�̬�ļ��
            {                
                bool hasInterfaceGraphElement = false; // ����Ƿ��нӿڽ��
                bool interfaceNameConflict = false; // �ӿڽ�������Ƿ��ͻ
                List<string> interfaceNameList = new List<string>();

                foreach (SlotContainer slotContainer in flowChartManager.CurrentGraphManager.SlotContainerList)
                {
                    if (slotContainer is InterfaceGraphElement)
                    {
                        hasInterfaceGraphElement = true;

                        if (!interfaceNameList.Contains(slotContainer.Text))
                        {
                            interfaceNameList.Add(slotContainer.Text);
                        }
                        else
                        {
                            interfaceNameConflict = true;
                        }

                        break;
                    }
                }

                if (!hasInterfaceGraphElement)
                {
                    documentManager.PrintText("û�нӿڽ�㣡");

                    MessageBox.Show("�ӻ�ͼ��Ҫ������һ���ӿڽ�㣡", "AIͼ��Ч�Լ��",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);

                    return false;
                }

                if (interfaceNameConflict)
                {
                    documentManager.PrintText("�ӿڽ�����Ƴ�ͻ��");

                    MessageBox.Show("�ӿڽ�㲻������ͬ�����ƣ�", "AIͼ��Ч�Լ��",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);

                    return false;
                }
            }
            else
            {
                if (initialStateNode == null) // û���ҵ���̬���
                {
                    documentManager.PrintText("û�����ó�̬��");

                    MessageBox.Show("��ͼ��û�����ó�̬����ȷ��AIͼ�г�̬��Ϊ������ڣ�", "AIͼ��Ч�Լ��", 
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);

                    return false;
                }

                // ����Ƿ����е�ͼԪ�����̬�����ȷ����            
                foreach (GraphElement graphElement in flowChartManager.CurrentGraphManager.SlotContainerList)
                {
                    if (!dataManager.IsConnect(initialStateNode, graphElement))
                    {
                        MessageBox.Show("��ͼ���г�̬�޷������ͼԪ����ȷ�����е�ͼԪ�����ɳ�̬��ȷ�����ϣ�", "AIͼ��Ч�Լ��", 
                                        MessageBoxButtons.OK, MessageBoxIcon.Information);

                        // ��ʾϸ��
                        List<GraphElement> list = new List<GraphElement>();
                        list.Add(graphElement);
                        dataManager.CurrentFlowChartManager.CurrentGraphManager.SelectGraphElements(list);

                        return false;
                    }
                }  
            }                      

            return true;
        }

        /// <summary>
        /// ����������Ƿ���Ч
        /// </summary>
        /// <param name="flowChartManager">��ͼ������</param>
        /// <param name="showDetail">�Ƿ���ʾϸ��</param>
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

            if (!avail) // ��ͼ��Ч 
            {
                MessageBox.Show("��ͼ���в����������ӣ���ȷ�����е������Ƿ���ȷ��", "��ͼ��Ч�Լ��", 
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
