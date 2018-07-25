using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Drawing;

using FlowChartEditor.BaseGraphElement;
using FlowChartEditor.ButtonGraphElement;
using FlowChartEditor.Command.FlowChartCommand;
using GraphElementLibrary;

namespace FlowChartEditor
{
    public class GraphManager
    {
        // �ؼ�
        private Canvas canvas; // ��ͼ���
        private Background background; // ��ͼ����

        // ������
        private DocumentManager documentManager; // ��������
        private Helper helper; // ������
        private FlowChartManager flowChartManager; // ����ͼ������
        private RegionManager regionManager = new RegionManager(); // ���������
        private GraphSetting graphSetting = GraphSetting.GetGraphSetting(); // ��ͼ�������ö���
        private EventManager eventManager; // �¼�������

        // ����
        private GraphElement selectedGraphElement; // ��ǰѡ�е�ͼԪ
        private Point lastSelectedPoint; // ��һ��ѡ��ĵ�           
        private Point multiSelectStartPoint; // ��ѡͼԪ����ʼ��
        private GraphElement lastActivatedGraphElement; // ��һ��ѡ�е�ͼԪ
        private GraphElement lastSelectedGraphElement; // ��һ�������ͼԪ
        private SlotContainer lastResizingGraphElement; // ��һ�����ŵ�ͼԪ
        private GraphElement lastConnectGraphElement; // ��һ�����ӵ�ͼԪ

        private bool controlMode = false; // �Ƿ���Control������״̬
        private bool shiftMode = false; // �Ƿ���Shift������״̬
        private bool altMode = false; // �Ƿ���Alt������״̬         
        private bool autoConnect = false; // �Ƿ��Զ�����ͼԪ

        private List<SlotContainer> slotContainerList = new List<SlotContainer>(); // �����������
        private List<ConnectorContainer> connectorContainerList = new List<ConnectorContainer>(); // �����߿��Ƶ���������
        private List<GraphElement> selectedGraphElementList = new List<GraphElement>(); // ѡ�е�ͼԪ����
        private List<GraphElement> moveGraphElementList = new List<GraphElement>(); // �ƶ���ͼԪ����
                
        private Size moveOffset = Size.Empty; // �ƶ�ͼԪ��ƫ��
        private Rectangle multiMoveRegion; // �ƶ�ͼԪ������
        private int scrollOffset = 25; // ��ͼ�������ƫ��
        
        private long lastMoveTime; // ��һ���ƶ�ͼԪ��ʱ��
        private int moveLagTime = 200; // ͼԪ�ƶ��ӳ�ʱ��
        private bool moveLagged = false; // �Ƿ��Ѿ��ӳ��ƶ� 
        private Direction lastLagDirection = Direction.None; // ��һ���ƶ��ӳٵķ���
        private int lastLagPosition = -1; // ��һ���ƶ��ӳٵ�λ��
        private int moveAdjustWidth = 5; // ͼԪ�ƶ������ļ��    
        private int moveLagOffset = 3; // ͼԪ�ӳ��ƶ���ƫ��
        private bool lockMoveDrag = false; // �Ƿ���������϶�ͼԪ  
      
        private int idIndex = 0; // �Զ�����id�����

        // ״̬
        private enum MoveType { SingleElement, MultiElement, AbbreviateElement }; // �ƶ�������       
        private enum UserOperation { None, Create, SingleSelect, MultiSelect, RegionSelect, ScrollCanvas, MoveRodman }; // �û�����ö��
        private UserOperation userOperation = UserOperation.None; // ��ǰ�û�����
        
        // ����֧��
        private List<FlowChartBaseCommand> commandList = new List<FlowChartBaseCommand>(); // ��������
        private int commandIndex = -1; // ��ǰ�������ڵ����
        private FlowChartMoveCommand moveCommand; // �ƶ�����

        /// <summary>
        /// ����ģʽ�Ĺ��캯��
        /// </summary>
        /// <param name="mainForm">������</param>
        /// <param name="canvas">��ͼ���</param>
        /// <param name="propertyGrid">�������</param>
        public GraphManager(FlowChartManager flowChartManager, Canvas canvas)
        {
            this.flowChartManager = flowChartManager;
            this.canvas = canvas;
            helper = Helper.GetHelper();
            documentManager = DocumentManager.GetDocumentManager();

            // ��ʼ��������ͼ����
            InitBackground(); 
      
            // ��ʼ�����������
            regionManager.Init(slotContainerList, connectorContainerList, background.BaseSize, background.GridSize.Width);
        }               

        /// <summary>
        /// �¼�������
        /// </summary>
        public EventManager CurrentEventManager
        {
            set
            {
                eventManager = value;
            }

            get
            {
                return eventManager;
            }
        }

        /// <summary>
        /// ��ǰ����������
        /// </summary>
        public int CommandIndex
        {
            get
            {
                return commandIndex;
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        public int CommandCount
        {
            get
            {
                return commandList.Count;
            }
        }

        /// <summary>
        /// �������������
        /// </summary>
        public List<string> CommandDescriptionList
        {
            get
            {
                List<string> descriptionList = new List<string>();

                foreach (FlowChartBaseCommand command in commandList)
                {
                    descriptionList.Add(string.Format("���� ({0})", command.Description));
                }

                return descriptionList;
            }
        }

        /// <summary>
        /// �Զ������ͼԪ����ʼID
        /// </summary>
        public int IDIndex
        {            
            get
            {
                return idIndex;
            }
        }

        /// <summary>
        /// ����ͼ��ͼ��
        /// </summary>
        public AbbreviateCanvas CurrentAbbreviateCanvas
        {
            set
            {
                canvas.CurrentAbbreviateCanvas = value;
                value.CurrentCanvas = canvas;
            }                        
        }

        /// <summary>
        /// ��ǰ��ͼ��
        /// </summary>
        public Canvas CurrentCanvas
        {
            get
            {
                return canvas;
            }
        }

        /// <summary>
        ///  ��ǰ��ͼ��������
        /// </summary>
        public Background CurrentBackground
        {
            get
            {
                return background;
            }
        }

        /// <summary>
        /// ����ͼ����������
        /// </summary>
        public FlowChartManager CurrentFlowChartManager
        {
            get
            {
                return flowChartManager;
            }
        }        

        /// <summary>
        /// ��ǰѡ�е�ͼԪ
        /// </summary>
        public GraphElement SelectedGraphElement
        {
            get
            {
                return selectedGraphElement;
            }
        }

        /// <summary>
        /// ѡ�е�ͼԪ����
        /// </summary>
        public List<GraphElement> SelectedGraphElementList
        {
            get
            {
                return selectedGraphElementList;
            }
        }

        /// <summary>
        /// �����߿��Ƶ�����
        /// </summary>
        public List<ConnectorContainer> ConnectorContainerList
        {
            get
            {
                return connectorContainerList;
            }
        }

        /// <summary>
        /// �����������
        /// </summary>
        public List<SlotContainer> SlotContainerList
        {
            get
            {
                return slotContainerList;
            }
        }

        /// <summary>
        /// �Ƿ���Control������״̬
        /// </summary>
        public bool ControlMode
        {
            set
            {
                controlMode = value;

                if (!value)
                {
                    canvas.AutoScroll = true;
                }
            }

            get
            {
                return controlMode;
            }
        }

        /// <summary>
        /// �Ƿ���Shift������״̬
        /// </summary>
        public bool ShiftMode
        {
            set
            {
                shiftMode = value;                
            }

            get
            {
                return shiftMode;
            }
        }

        /// <summary>
        /// �Ƿ���Alt������״̬
        /// </summary>
        public bool AltMode
        {
            set
            {
                altMode = value;
            }

            get
            {
                return altMode;
            }
        }

        /// <summary>
        /// ��ʼ����ͼ�屳��
        /// </summary>
        private void InitBackground()
        {
            background = new Background();
            canvas.CanvasBackgound = background;
            canvas.AutoScroll = true;
            canvas.AutoScrollMinSize = background.CanvasSize;

            background.Refresh();
        }

        /// <summary>
        /// ��ȡ����ͼ�Ļ�ͼ����
        /// </summary>
        /// <returns>����ͼ�Ļ�ͼ������ϣ��</returns>
        public Hashtable GetArguments()
        {
            Hashtable args = new Hashtable();
            args["slotContainerList"] = slotContainerList;
            args["connectorContainerList"] = connectorContainerList;
            args["background"] = background;            
            args["idIndex"] = idIndex;

            return args;
        }

        /// <summary>
        /// ��������ͼ�Ļ�ͼ����
        /// </summary>
        /// <param name="args">����ͼ�Ļ�ͼ��ϣ��</param>
        public void SetArguments(Hashtable args)
        {
            slotContainerList = args["slotContainerList"] as List<SlotContainer>;
            connectorContainerList = args["connectorContainerList"] as List<ConnectorContainer>;
            background = args["background"] as Background;
            idIndex = (int)args["idIndex"];

            selectedGraphElementList = new List<GraphElement>();

            // ˢ�»������
            background.Refresh();

            // �����ͼ����
            canvas.CanvasBackgound = background;  
            canvas.AutoScrollMinSize = background.CanvasSize;
            canvas.BackColor = graphSetting.BackgroundColor;
            background.BaseSize = background.CanvasSize;
            graphSetting.CanvasScale = 1.0f;

            // ˢ��ͼԪ����
            foreach (SlotContainer slotContainer in slotContainerList)
            {
                slotContainer.Refresh();               
            }

            foreach (ConnectorContainer connectorContainer in connectorContainerList)
            {
                connectorContainer.Refresh();
            }            

            // ��ʼ�����������
            RefreshRegion();

            // ˢ�»�ͼ��
            ReconstructCanvasGraphElementList();
        }  

        /// <summary>
        /// ˢ�»�ͼ����
        /// </summary>
        public void RefreshRegion()
        {
            regionManager.Init(slotContainerList, connectorContainerList, background.BaseSize, background.GridSize.Width);
        }

        /// <summary>
        /// ���·���ͼԪ��ID
        /// </summary>
        public void ResetGraphElementID()
        {
            idIndex = 0;

            foreach (SlotContainer node in slotContainerList)
            {
                node.ID = AllocateGraphElementID();
            }

            foreach (ConnectorContainer line in connectorContainerList)
            {
                line.ID = AllocateGraphElementID();
            }
        }

        /// <summary>
        /// ����ͼԪID
        /// </summary>
        /// <returns>ͼԪ��ID</returns>
        public int AllocateGraphElementID()
        {
            idIndex++;

            return idIndex;
        }

        /// <summary>
        /// �ڱ༭ǰʹ����ͼԪ������Ч״̬
        /// </summary>
        public void ValidateBeforeSave()
        {
            // ��������ߣ�ȥ���ƶ�������״̬
            foreach (ConnectorContainer line in connectorContainerList)
            {
                line.Activated = false;
                line.Selected = false;
                line.Moving = false;
                line.Visible = true;                
            }
            
            // �������������������в����������������������
            foreach (SlotContainer slotContainer in slotContainerList)
            {
                slotContainer.Activated = false;
                slotContainer.Selected = false;
                slotContainer.Moving = false;

                foreach (SlotGraphElement slot in slotContainer.GetSlotList())
                {
                    if (slot.Binded)
                    {
                        if (!connectorContainerList.Contains(slot.BindingConnector.Line))
                        {
                            slot.UnBind();
                        }
                    }
                }
            }

            // �ָ�ѡ��״̬
            if (selectedGraphElement != null)
            {
                selectedGraphElement.Selected = true;
            }
            else 
            {
                foreach (GraphElement graphElemnt in selectedGraphElementList)
                {
                    graphElemnt.Selected = true;
                }
            }
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="slotContainer">�������</param>
        private void AdjustLine(SlotContainer slotContainer)
        {
            List<ConnectorContainer> lineList = regionManager.GetConnectorContainerList(slotContainer.InvalidRectangle);

            foreach (ConnectorContainer line in lineList)
            {
                if (line.LineIntersect(slotContainer))
                {
                    AdjustLine(line, slotContainer);
                }
            }
        }   
        
        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="line">��ǰ������</param>
        /// <param name="slotContainer">��صĲ������</param>
        private void AdjustLine(ConnectorContainer line , SlotContainer slotContainer)
        {
            Rectangle adjustRectangle = line.GetAdjustRectangle(background.CanvasSize, background.GridSize.Width);
            List<SlotContainer> totalSlotContainerList = regionManager.GetSlotContainerList(adjustRectangle);
            List<ConnectorContainer> totalLineList = regionManager.GetConnectorContainerList(adjustRectangle);

            line.AdjustLine(slotContainer, totalSlotContainerList, totalLineList, background.GridSize.Width, background.BaseSize, regionManager.CanvasGridBoard);
            line.AdjustRectangle();  
        }

        /// <summary>
        /// �����ͼ��Ĵ�С
        /// </summary>
        /// <param name="plusSize">�����Ĵ�С</param>
        private void ResetCanvasSize(Size plusSize)
        {
            if (plusSize.Width != 0 || plusSize.Height != 0) // �ȼ���Ƿ��б�Ҫ����
            {
                background.CanvasSize += plusSize;
                background.BaseSize += plusSize;
                canvas.AutoScrollMinSize = background.CanvasSize;

                // ������������
                regionManager.InitCanvasGridBoard(slotContainerList, background.BaseSize, background.GridSize.Width);
            }            
        }

        /// <summary>
        /// ��󻯻�ͼ��
        /// </summary>
        public void MaximumCanvasSize()
        {
            background.CanvasSize = canvas.ClientSize;
            background.BaseSize = canvas.ClientSize;
            canvas.AutoScrollMinSize = canvas.ClientSize;

            // ������������
            regionManager.InitCanvasGridBoard(slotContainerList, background.BaseSize, background.GridSize.Width);
        }

        /// <summary>
        /// ������ͼ��Ĵ�С
        /// </summary>
        public void AdjustCanvasSize()
        {
            int right = -1;
            int bottom = -1;

            foreach(GraphElement graphElement in canvas.GraphElementList)
            {
                if(graphElement.InvalidRectangle.Right > right)
                {
                    right = graphElement.InvalidRectangle.Right;
                }

                if(graphElement.InvalidRectangle.Bottom > bottom)
                {
                    bottom = graphElement.InvalidRectangle.Bottom;
                }
            }

            if (right < background.MinSize.Width)
            {
                right = background.MinSize.Width;
            }

            if (bottom < background.MinSize.Height)
            {
                bottom = background.MinSize.Height;
            }

            if ((right != background.CanvasSize.Width && right != -1) || 
                (bottom != background.CanvasSize.Height && bottom != -1))
            {
                Size newSize = new Size(right, bottom);

                background.CanvasSize = newSize;
                background.BaseSize = newSize;
                canvas.AutoScrollMinSize = newSize;

                // ������������
                regionManager.InitCanvasGridBoard(slotContainerList, background.BaseSize, background.GridSize.Width);
            }
        }

        /// <summary>
        /// ��ʼ����һ������
        /// </summary>
        /// <param name="command">��ǰ����</param>
        private void InitFirstCommand(FlowChartBaseCommand command)
        {
            if (commandIndex == -1) // ����Ƿ��ǵ�һ������
            {
                command.FirstCommand = true;
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        public void CreateGraphElement()
        {            
            Point p = canvas.AbbreviatGraphElement.Location;
            FlowChartCreateCommand cmd = new FlowChartCreateCommand(this, "�½�ͼԪ");
            InitFirstCommand(cmd);            

            if (cmd.Execute(new object[] { p, canvas.AbbreviatGraphElement.TypeString })) // ����ִ�гɹ�
            {
                AdjustCommandList(cmd);                                

                // �Զ�����������
                AdjustLine(selectedGraphElement as SlotContainer);

                GraphElement graphElement = selectedGraphElement; 
               
                if (autoConnect)
                {
                    ConnectGraphElement(lastConnectGraphElement as SlotContainer, selectedGraphElement as SlotContainer);                   
                }

                if (lastSelectedGraphElement != null) // ����һ����ѡ�е�ͼԪ��Ϊ��ѡ��״̬
                {
                    if (!selectedGraphElementList.Contains(lastSelectedGraphElement)) // �ڶ�ѡ״̬��Ҫ�����һ����ѡ�е�ͼԪ�Ƿ��ڶ�ѡ������
                    {
                        lastSelectedGraphElement.Selected = false;
                    }
                }

                SelectGraphElement(graphElement, p); // ѡ���½���ͼԪ

                // �༭�½���ͼԪ
                FlowChartEditCommand editCommand = new FlowChartEditCommand(this, "�༭ͼԪ");                
                InitFirstCommand(editCommand);

                if (editCommand.Execute(graphElement)) // ����ִ�гɹ�
                {
                    AdjustCommandList(editCommand);
                } 

                regionManager.AddToRegion(graphElement); // ����ͼԪ��������
            }                                       
        }

        /// <summary>
        /// �Զ�����ͼԪ
        /// </summary>
        /// <param name="previousContainer">�����Ĳ������</param>
        /// <param name="currentContainer">����Ĳ������</param>
        private void ConnectGraphElement(SlotContainer previousContainer, SlotContainer currentContainer)
        {
            SlotGraphElement outSlot = previousContainer.GetOutSlot();
            SlotGraphElement inSlot = currentContainer.GetInSlot();            

            if(outSlot != null && inSlot != null) // ������Щ��㲻�ܷ�����²��
            {
                FlowChartCreateAndConnectLineCommand cmd = new FlowChartCreateAndConnectLineCommand(this, "����������ͼԪ");
                InitFirstCommand(cmd);

                if (cmd.Execute(new object[] { outSlot, inSlot })) // ����ִ�гɹ�
                {
                    AdjustCommandList(cmd);

                    ConnectorContainer line = selectedGraphElement as ConnectorContainer;
                    regionManager.AddToRegion(line); // ����ͼԪ��������                    
                    
                    // �Զ�����������                    
                    AdjustLine(line, currentContainer);

                    if (previousContainer is ConditionGraphElement) // �༭�������������������
                    {
                        FlowChartEditCommand editCommand = new FlowChartEditCommand(this, "�༭ͼԪ");

                        if (editCommand.Execute(line))
                        {
                            AdjustCommandList(editCommand);
                        }
                    }
                }
            }                       

            autoConnect = false;
        }

        /// <summary>
        /// ������ʾ���Ե�ͼԪ��λ��
        /// </summary>
        /// <param name="p">����λ��</param>
        private void AdjustAbbreviatGraphElement(Point p)
        {
            canvas.AbbreviatGraphElement.Location = p - moveOffset;
            RecordLastSelectedPoint(p);
        }

        /// <summary>
        /// ���¹�����ͼ���ͼԪ����
        /// </summary>
        public void ReconstructCanvasGraphElementList()
        {
            canvas.PaintCanvas = false;
            canvas.GraphElementList.Clear();

            foreach (SlotContainer slotContainer in slotContainerList)
            {
                canvas.GraphElementList.Add(slotContainer);
            }

            foreach (ConnectorContainer connectorContainer in connectorContainerList)
            {
                canvas.GraphElementList.Add(connectorContainer);
            }

            canvas.PaintCanvas = true;

            // ˢ�»�ͼ��
            RefreshCanvas();
        }

        /// <summary>
        /// �ͷ�����ͼ���еĻ�ͼ��Դ
        /// </summary>
        public void Release()
        {
            foreach(SlotContainer slotContainer in slotContainerList)
            {
                slotContainer.Release();
            }

            foreach(ConnectorContainer connectorContainer in connectorContainerList)
            {
                connectorContainer.Release();
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        public void CutGraphElement()
        {
            // ����ǰͼԪ�������������
            if (selectedGraphElementList.Count > 0) // ���ж��ͼԪ
            {
                foreach (GraphElement graphElement in selectedGraphElementList)
                {                    
                    regionManager.DeleteFromRegion(graphElement);
                }                
            }
            else if (selectedGraphElement != null) // ���е���ͼԪ
            {
                regionManager.DeleteFromRegion(selectedGraphElement);
            }

            FlowChartCutCommand cmd = new FlowChartCutCommand(this, "����ͼԪ");
            InitFirstCommand(cmd);

            if (cmd.Execute(null)) // ����ִ�гɹ�
            {
                AdjustCommandList(cmd);
                
                if (selectedGraphElementList.Count > 0) // ���ж��ͼԪ
                {
                    selectedGraphElementList.Clear();
                    canvas.CurrentMultiSelectMark.Visible = false;
                }
                else if (selectedGraphElement != null) // ���е���ͼԪ
                {
                    selectedGraphElement = null;
                }

                ReconstructCanvasGraphElementList();
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        public void CopyGraphElement()
        {
            FlowChartCopyCommand cmd = new FlowChartCopyCommand(this, "����ͼԪ");
            InitFirstCommand(cmd);

            if (cmd.Execute(null)) // ����ִ�гɹ�
            {
                AdjustCommandList(cmd);                
            }
        }

        /// <summary>
        /// �Զ������϶���Ļ�ͼ���С
        /// </summary>
        private void AdjustOutOfBorder()
        {
            int right = background.BaseSize.Width;
            int bottom = background.BaseSize.Height;
            Rectangle multiSelectRegion = canvas.CurrentMultiSelectMark.RegionRectangle;

            switch (userOperation)
            {
                case UserOperation.SingleSelect: // �϶�����ͼԪ
                    {
                        if (selectedGraphElement.InvalidRectangle.Right > right)
                        {
                            right = selectedGraphElement.InvalidRectangle.Right;
                        }

                        if (selectedGraphElement.InvalidRectangle.Bottom > bottom)
                        {
                            bottom = selectedGraphElement.InvalidRectangle.Bottom;
                        }

                        break;
                    }
                case UserOperation.MultiSelect: // �϶�ͼԪ��
                    {
                        if (multiSelectRegion.Right > right)
                        {
                            right = multiSelectRegion.Right;
                        }

                        if (multiSelectRegion.Bottom > bottom)
                        {
                            bottom = multiSelectRegion.Bottom;
                        }

                        break;
                    }
                case UserOperation.MoveRodman: // �϶��α�
                    {
                        if (multiMoveRegion.Right > right)
                        {
                            right = multiMoveRegion.Right;
                        }

                        if (multiMoveRegion.Bottom > bottom)
                        {
                            bottom = multiMoveRegion.Bottom;
                        }

                        break;
                    }
                default:
                    {                        
                        if (selectedGraphElementList.Count > 0)
                        {
                            if (multiSelectRegion.Right > right)
                            {
                                right = multiSelectRegion.Right;
                            }

                            if (multiSelectRegion.Bottom > bottom)
                            {
                                bottom = multiSelectRegion.Bottom;
                            }
                        }
                        else if (selectedGraphElement != null)
                        {
                            if (selectedGraphElement.InvalidRectangle.Right > right)
                            {
                                right = selectedGraphElement.InvalidRectangle.Right;
                            }

                            if (selectedGraphElement.InvalidRectangle.Bottom > bottom)
                            {
                                bottom = selectedGraphElement.InvalidRectangle.Bottom;
                            }
                        }

                        break;
                    }
            }

            if (right != background.BaseSize.Width || bottom != background.BaseSize.Height)
            {
                background.CanvasSize = new Size((int)Math.Ceiling(right * graphSetting.CanvasScale), 
                                                 (int)Math.Ceiling(bottom * graphSetting.CanvasScale));
                background.BaseSize = new Size(right, bottom);
                canvas.AutoScrollMinSize = background.CanvasSize;
                RefreshCanvas();

                // ������������
                regionManager.InitCanvasGridBoard(slotContainerList, background.BaseSize, background.GridSize.Width);                
            }
        }

        /// <summary>
        /// ���Ż�ͼ��
        /// </summary>
        /// <param name="scale">���Ų���</param>
        public void SetCanvasScale(float scale)
        {
            graphSetting.CanvasScale = scale;
            background.CanvasSize = new Size((int)Math.Ceiling(background.BaseSize.Width * scale),
                                             (int)Math.Ceiling(background.BaseSize.Height * scale));
            canvas.AutoScrollMinSize = background.CanvasSize;

            RefreshCanvas();            
        }

        /// <summary>
        /// ���Ż�ͼ��
        /// </summary>
        /// <param name="scale">���Ų���</param>
        public void AdjustCanvasScale(float scale)
        {
            graphSetting.CanvasScale *= scale;            
            background.CanvasSize = new Size((int)Math.Ceiling(background.CanvasSize.Width * scale),
                                             (int)Math.Ceiling(background.CanvasSize.Height * scale));
            canvas.AutoScrollMinSize = background.CanvasSize;

            RefreshCanvas();
        }

        /// <summary>
        /// ճ��ͼԪ
        /// </summary>
        public void PasteGraphElement()
        {
            Point pastePoint = canvas.PointToClient(Cursor.Position) - (Size)canvas.AutoScrollPosition; // ճ��λ��
            FlowChartPasteCommand cmd = new FlowChartPasteCommand(this, "ճ��ͼԪ");
            InitFirstCommand(cmd);

            if (cmd.Execute(pastePoint)) // ����ִ�гɹ�
            {
                AdjustCommandList(cmd);

                bool multiPaste = false;

                if(flowChartManager.PasteTable.Keys.Count > 1) // ���ƶ��ͼԪ
                {
                    multiPaste = true;

                    // ��ն�ѡ����
                    ClearSelectedGraphElementList();
                }

                // ��ճ����ͼԪ���뵽������
                foreach(object o in flowChartManager.PasteTable.Keys)
                {
                    GraphElement graphElement = o as GraphElement;

                    if (graphElement is ConnectorContainer) // ������
                    {
                        ConnectorContainer line = graphElement as ConnectorContainer;
                        line.AdjustRectangle();
                    }

                    regionManager.AddToRegion(graphElement);

                    if (multiPaste)
                    {                        
                        selectedGraphElementList.Add(graphElement);
                    }
                    else
                    {
                        selectedGraphElement = graphElement;
                    }

                    graphElement.Selected = true;
                }

                // ѡ��ճ���Ķ��ͼԪ
                if (multiPaste)
                {                    
                    CreateMultiSelectRegion();
                }

                // �Զ�������ͼ���С
                AdjustOutOfBorder();
               
                ReconstructCanvasGraphElementList();

                // ִ���߼�����                
                List<GraphElement> graphElementList = new List<GraphElement>();

                if (multiPaste)
                {
                    graphElementList.AddRange(selectedGraphElementList);
                }
                else
                {
                    graphElementList.Add(selectedGraphElement);
                }

                LogicBaseManager logicManager = helper.GetLogicManager(flowChartManager.CurrentChartMode);
                logicManager.DoLogicOperation(flowChartManager, LogicType.Paste, graphElementList);                
            }
        }

        /// <summary>
        /// ������ͼ����Ҽ��˵�
        /// </summary>
        /// <param name="p">����λ��</param>
        public void ShowCanvasContextMenu(Point point)
        {
            Point p = point + new Size(canvas.RectangleToScreen(canvas.ClientRectangle).Location); // ����ת��            

            // ִ���߼�����
            LogicBaseManager logicManager = helper.GetLogicManager(flowChartManager.CurrentChartMode);
            logicManager.DoLogicOperation(flowChartManager, LogicType.PopUpContextMenu, p);                      
        }

        /// <summary>
        /// �༭ͼԪ
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        public void EditDataElement(GraphElement graphElement)
        {
            FlowChartEditCommand cmd = new FlowChartEditCommand(this, "�༭����Ԫ");
            InitFirstCommand(cmd);

            if (cmd.Execute(graphElement)) // ����ִ�гɹ�
            {
                AdjustCommandList(cmd);
                regionManager.ChangeRegion(selectedGraphElement);
            }
        }

        /// <summary>
        /// �༭ͼԪ
        /// </summary>
        public void EditDataElement()
        {
            if (selectedGraphElement != null) // ��ѡ�е�ͼԪ
            {
                if (selectedGraphElement.Enable) // ѡ�е�ͼԪ���Ա༭
                {
                    if (selectedGraphElement is RemarkGraphElement) // �༭ע��
                    {
                        FlowChartEditRemarkCommand cmd = new FlowChartEditRemarkCommand(this, "�༭Ԫע��");
                        InitFirstCommand(cmd);

                        if (cmd.Execute((selectedGraphElement as RemarkGraphElement).Owner)) // ����ִ�гɹ�
                        {
                            AdjustCommandList(cmd);
                        }
                    }
                    else // �༭ͼԪ
                    {
                        FlowChartEditCommand cmd = new FlowChartEditCommand(this, "�༭ͼԪ");
                        InitFirstCommand(cmd);

                        if (cmd.Execute(selectedGraphElement)) // ����ִ�гɹ�
                        {
                            AdjustCommandList(cmd);
                            regionManager.ChangeRegion(selectedGraphElement);                         
                        }                        
                    }                    
                }
                else
                {
                    MessageBox.Show("��ǰͼԪ������Ч�������Ƿ�������ȷ��", "ͼԪ�༭", MessageBoxButtons.OK,
                                    MessageBoxIcon.Information);
                }                
            }
        }

        /// <summary>
        /// �������Ե�ͼԪ
        /// </summary>
        /// <param name="graphType">ͼԪ����</param>
        /// <param name="centerLocation">ͼԪ����λ��</param>
        /// <param name="elementSize">ͼԪ��С</param>
        /// <param name="moveSize">ͼԪ���ƶ���С</param>
        /// <returns>ͼԪ����</returns>
        private GraphElement CreateAbbreviateGraphElement(GraphType graphType, Point centerLocation)
        {
            GraphElement abbreviateGraphElement = null;

            switch (graphType)
            {
                case GraphType.ConditionNode: // �������
                    {
                        abbreviateGraphElement = new ConditionGraphElement(centerLocation - graphSetting.ConditionNodeMoveOffset, graphSetting.ConditionNodeElementSize);
                        moveOffset = graphSetting.ConditionNodeMoveOffset;

                        break;
                    }
                case GraphType.ActionNode: // �������
                    {
                        abbreviateGraphElement = new ActionGraphElement(centerLocation - graphSetting.ActionNodeMoveOffset, graphSetting.ActionNodeElementSize);
                        moveOffset = graphSetting.ActionNodeMoveOffset;

                        break;
                    }
                case GraphType.EventNode: // �¼����
                    {
                        abbreviateGraphElement = new EventGraphElement(centerLocation - graphSetting.EventNodeMoveOffset, graphSetting.EventNodeElementSize);
                        moveOffset = graphSetting.EventNodeMoveOffset;

                        break;
                    }
                case GraphType.AIStateNode: // AI״̬���
                    {
                        abbreviateGraphElement = new AIStateGraphElement(centerLocation - graphSetting.AIStateNodeMoveOffset, graphSetting.AIStateNodeElementSize);
                        moveOffset = graphSetting.AIStateNodeMoveOffset;

                        break;
                    }
                case GraphType.AIActionNode: // AI�������
                    {
                        abbreviateGraphElement = new AIActionGraphElement(centerLocation - graphSetting.AIActionNodeMoveOffset, graphSetting.AIActionsNodeElementSize);
                        moveOffset = graphSetting.AIActionNodeMoveOffset;

                        break;
                    }
                case GraphType.AIActionsNode: // AI��������
                    {
                        abbreviateGraphElement = new AIActionsGraphElement(centerLocation - graphSetting.AIActionsNodeMoveOffset, graphSetting.AIActionsNodeElementSize);
                        moveOffset = graphSetting.AIActionsNodeMoveOffset;

                        break;
                    }
                case GraphType.InnerChart: // �ӻ�ͼ���
                    {
                        abbreviateGraphElement = new InnerChart(centerLocation - graphSetting.InnerChartMoveOffset, graphSetting.InnerChartElementSize);
                        moveOffset = graphSetting.InnerChartMoveOffset;

                        break;
                    }
                case GraphType.InterfaceNode: // �ӿڽ��
                    {
                        abbreviateGraphElement = new InterfaceGraphElement(centerLocation - graphSetting.InterfaceNodeMoveOffset, graphSetting.InterfaceNodeElementSize);
                        moveOffset = graphSetting.InterfaceNodeMoveOffset;

                        break;
                    }
            }

            if (abbreviateGraphElement != null)
            {
                abbreviateGraphElement.Refresh();
            }

            return abbreviateGraphElement;
        }

        /// <summary>
        /// ������ʾ���Ե�ͼԪ
        /// </summary>
        /// <param name="graphType">ͼԪ������</param>
        /// <param name="point">ͼԪ��λ��</param>
        /// <param name="autoConnect">�Ƿ��Զ�����</param>
        /// <param name="jumpConnect">�Ƿ���ת����</param>
        public void CreateAbbreviateGraphElement(GraphType graphType, Point p, bool autoConnect)
        {
            Point point = Point.Empty;

            if (!p.IsEmpty)
            {
                point = p - new Size(canvas.AutoScrollPosition);
            }

            canvas.AbbreviatGraphElement = CreateAbbreviateGraphElement(graphType, point);
            userOperation = UserOperation.Create;
            this.autoConnect = autoConnect;

            if (autoConnect) // ��¼��ǰѡ�еĲ������
            {
                lastConnectGraphElement = selectedGraphElement as SlotContainer;

                // ��������ͼԪ��������
                int tailX = (int)(lastConnectGraphElement.Location.X + lastConnectGraphElement.ElementSize.Width / 2);
                int tailY = (int)(lastConnectGraphElement.Location.Y + lastConnectGraphElement.ElementSize.Height);
                int headX = (int)(canvas.AbbreviatGraphElement.Location.X + canvas.AbbreviatGraphElement.ElementSize.Width / 2);
                int headY = (int)(canvas.AbbreviatGraphElement.Location.Y);

                canvas.AbbreviateLine = new ConnectorContainer(new Point(tailX, tailY), new Point(headX, headY));
                canvas.AbbreviateLine.Init();
            }
            else // ���������
            {
                canvas.AbbreviateLine = null;
            }

            if (!p.IsEmpty) // ��ʾ��ʾ��Ϣ
            {
                InitTooltipText(canvas.AbbreviatGraphElement, "<underline>����϶��ƶ�ͼԪ\r\n<underline>���������ͼԪ\r\n<underline>��Esc��ȡ������ͼԪ", point);
            }

            RefreshCanvas();
        }

        /// <summary>
        /// ��ǲ��������ص�������
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        /// <param name="mark">�Ƿ���</param>
        private void MarkRelavateLines(SlotContainer slotContainer, bool mark)
        {            
            foreach (ConnectorContainer line in slotContainer.GetConnectedLine())
            {
                line.Marked = mark;
            }
        }

        /// <summary>
        /// ����༭״̬
        /// </summary>
        public void ResetUserOperation()
        {
            userOperation = UserOperation.None;
        }

        /// <summary>
        /// ɾ����ʾ���Ե�ͼԪ
        /// </summary>
        public void DeleteAbbreviateGraphElement()
        {
            userOperation = UserOperation.None;
            canvas.AbbreviatGraphElement = null;
            canvas.AbbreviateLine = null;
            RefreshCanvas();
        }               

        /// <summary>
        /// ��ʼ���ı���ʾ
        /// </summary>
        /// <param name="graphElement">��Ҫ�ı���ʾ��ͼԪ</param>
        /// <param name="location">�ı���ʾ��λ��</param>
        private void InitTooltipText(GraphElement graphElement, Point location)
        {
            string tooltipText = "";

            if (!string.IsNullOrEmpty(graphElement.TooltipText) &&
                graphElement.Text != graphElement.TooltipText) // ��ʾͼԪ����ʾ�ı�
            {
                tooltipText = graphElement.TooltipText;
            }

            if (selectedGraphElementList.Contains(graphElement)) // ��ѡͼԪ״̬
            {
                tooltipText = "<underline>����϶��ƶ���ѡ��ͼԪ��";
            }
            else
            {
                if (graphElement is ConnectorGraphElement) // �����ͼԪ�������߿��Ƶ�
                {
                    ConnectorGraphElement connector = graphElement as ConnectorGraphElement;

                    if (connector.IsHeadPoint || connector.IsTailPoint) // �����߿��Ƶ���ͷ����β���
                    {
                        tooltipText = "<underline>����϶�����ͼԪ���������������״";
                    }
                    else
                    {
                        tooltipText = "<underline>����϶�������������״";
                    }
                }
                else if (graphElement is ConnectorContainer) // �����ͼԪ��������
                {
                    ConnectorContainer line = graphElement as ConnectorContainer;

                    if (tooltipText == "")
                    {
                        tooltipText = line.Text;
                    }
                    
                    if (tooltipText != "")
                    {
                        if (line.InSlotContainer != null && line.OutSlotContainer != null) // ��ʾ�������˵�����
                        {
                            tooltipText = string.Format("<bold>��֧����\r\n{0}\r\n<split>\r\n<bold>������Ϣ\r\n{1} --> {2}\r\n<split>\r\n<underline>��<>���л������������˽��\r\n<underline>���˫����Enter�༭ͼԪ",
                                                        tooltipText, line.InSlotContainer.Text, line.OutSlotContainer.Text);
                        }
                        else
                        {
                            tooltipText = string.Format("<bold>��֧����\r\n{0}\r\n<split>\r\n<underline>���˫����Enter���༭ͼԪ",
                                                        tooltipText);
                        }
                    }
                    else
                    {
                        if (line.InSlotContainer != null && line.OutSlotContainer != null) // ��ʾ�������˵�����
                        {
                            tooltipText = string.Format("<bold>������Ϣ\r\n{0} --> {1}\r\n<split>\r\n<underline>��<>���л������������˽��\r\n<underline>���˫����Enter�༭ͼԪ",
                                                        line.InSlotContainer.Text, line.OutSlotContainer.Text);
                        }
                        else
                        {
                            tooltipText = "<underline>���˫����Enter���༭ͼԪ";
                        }
                    }
                    
                }
                else if (graphElement is ResizeControler) // �����ͼԪ�����ſ��Ƶ�
                {
                    tooltipText = "<underline>����϶�����ͼԪ��״";
                }
                else if (graphElement is AddButton) // �����ͼԪ����Ӱ�ť
                {
                    tooltipText = "<underline>���������ͼԪ���";
                }
                else if (graphElement is ConnectButton) // �����ͼԪ�����Ӱ�ť
                {
                    tooltipText = "<underline>����϶�����������";
                }
                else if (graphElement is SlotGraphElement) // �����ͼԪ�ǲ��
                {
                    if (string.IsNullOrEmpty(graphElement.TooltipText))
                    {
                        tooltipText = "<underline>���������ϳ������߿��Ƶ����ͼԪ����";
                    }
                    else
                    {
                        tooltipText = string.Format("<bold>�������\r\n{0}\r\n<split>\r\n<underline>���������ϳ������߿��Ƶ����ͼԪ����",
                                                    graphElement.TooltipText);
                    }
                }
                else if (graphElement is RemarkGraphElement) // �����ͼԪ��ע��
                {
                    tooltipText = "<underline>���˫����Enter���༭ͼԪע��";
                }
                else if (graphElement is SlotContainer) // �����ͼԪ�ǲ������
                {
                    if (graphElement.Enable) // �����ͼԪ����
                    {
                        if (tooltipText != "")
                        {
                            tooltipText = string.Format("{0}\r\n<split>\r\n<underline>���˫����Enter���༭ͼԪ", tooltipText);
                        }
                        else
                        {
                            tooltipText = "<underline>���˫����Enter���༭ͼԪ";
                        }
                    }
                    else // �����ͼԪ������
                    {
                        if (tooltipText != "")
                        {
                            tooltipText = string.Format("{0}\r\n<split>\r\n<underline>��ǰͼԪ���ɱ༭����ȷ�������Ƿ���ȷ", tooltipText);
                        }
                        else
                        {
                            tooltipText = "<underline>��ǰͼԪ���ɱ༭����ȷ�������Ƿ���ȷ";
                        }
                    }
                }
                else if (graphElement is Rodman)
                {
                    tooltipText = "<underline>����϶������ƶ��α��ұ߻��±ߵ�ͼԪ";
                }
            }

            InitTooltipText(graphElement, tooltipText, location);
        }

        /// <summary>
        /// ��ʼ���ı���ʾ
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="text">�ı�����</param>
        /// <param name="mouseLocation">���λ��</param>
        private void InitTooltipText(GraphElement graphElement, string text, Point mouseLocation)
        {
            canvas.TooltipGraphElement.Adjust(text, graphElement, canvas.VisibleRectangle, mouseLocation);
            canvas.TooltipGraphElement.Visible = true;
        }        

        /// <summary>
        /// ������������ͼ���С
        /// </summary>
        /// <param name="moveType">�ƶ�������</param>
        /// <param name="direction">�ƶ���λ��</param>
        private void AutoScrollAndResizeCanvas(MoveType moveType, Direction direction)
        {        
            int leftX = -1;
            int rightX = -1;
            int topY = -1;
            int bottomY = -1;

            switch (moveType)
            {
                case MoveType.SingleElement:
                    {
                        leftX = selectedGraphElement.InvalidRectangle.Left;
                        rightX = selectedGraphElement.InvalidRectangle.Right;
                        topY = selectedGraphElement.InvalidRectangle.Top;
                        bottomY = selectedGraphElement.InvalidRectangle.Bottom;

                        break;
                    }
                case MoveType.MultiElement:
                    {
                        Rectangle multiSelectRegion = canvas.CurrentMultiSelectMark.RegionRectangle;
                        leftX = multiSelectRegion.Left;
                        rightX = multiSelectRegion.Right;
                        topY = multiSelectRegion.Top;
                        bottomY = multiSelectRegion.Bottom;

                        break;
                    }
            }

            AutoScrollAndResizeCanvas(leftX, rightX, topY, bottomY, direction);
        }

        /// <summary>
        /// ������������ͼ���С
        /// </summary>
        /// <param name="leftX">��߿�ֵ</param>
        /// <param name="rightX">�ұ߿�ֵ</param>
        /// <param name="topY">�ϱ߿�ֵ</param>
        /// <param name="bottomY">�±߿�ֵ</param>
        /// <param name="moveDirection">�ƶ�����</param>
        private void AutoScrollAndResizeCanvas(int leftX, int rightX, int topY, int bottomY, Direction moveDirection)
        {
            Size moveSize = new Size(0, 0);
            Size canvasPlusSize = new Size(0, 0);
            bool requireScroll = false;
            bool requireResize = false;

            switch (moveDirection)
            {
                case Direction.Right:
                    {
                        if ((rightX + scrollOffset) * graphSetting.CanvasScale > canvas.ClientRectangle.Right - canvas.AutoScrollPosition.X) // �ڻ�ͼ����ұ�Ե
                        {
                            moveSize.Width = scrollOffset;
                            requireScroll = true;

                            if ((rightX - canvas.AutoScrollPosition.X + scrollOffset) * graphSetting.CanvasScale > background.CanvasSize.Width) // ��Ҫ�����ͼ��Ŀ��
                            {
                                canvasPlusSize.Width = scrollOffset;
                                requireResize = true;
                            }
                        }

                        break;
                    }
                case Direction.Left:
                    {
                        if ((leftX - scrollOffset) * graphSetting.CanvasScale < -canvas.AutoScrollPosition.X) // �ڻ�ͼ������Ե
                        {
                            if (canvas.AutoScrollPosition.X < 0) // ��������δ�ߵ���ͷ
                            {
                                moveSize.Width = -scrollOffset;
                                requireScroll = true;
                            }
                        }

                        break;
                    }
                case Direction.Up:
                    {
                        if ((topY - scrollOffset) * graphSetting.CanvasScale < -canvas.AutoScrollPosition.Y) // �ڻ�ͼ����ϱ�Ե
                        {
                            if (canvas.AutoScrollPosition.Y < 0) // ��������δ�ߵ���ͷ
                            {
                                moveSize.Height = -scrollOffset;
                                requireScroll = true;
                            }
                        }

                        break;
                    }
                case Direction.Down:
                    {
                        if ((bottomY + scrollOffset) * graphSetting.CanvasScale > canvas.ClientRectangle.Bottom - canvas.AutoScrollPosition.Y) // �ڻ�ͼ����±�Ե
                        {
                            moveSize.Height = scrollOffset;
                            requireScroll = true;

                            if ((bottomY - canvas.AutoScrollPosition.Y + scrollOffset) * graphSetting.CanvasScale > background.CanvasSize.Height) // ��Ҫ�����ͼ��ĸ߶�
                            {
                                canvasPlusSize.Height = scrollOffset;
                                requireResize = true;
                            }
                        }

                        break;
                    }
            }

            if (requireScroll)
            {
                ScrollCanvas(moveSize, true);
            }

            if (requireResize)
            {
                ResetCanvasSize(canvasPlusSize);
            }
        }

        /// <summary>
        /// ������������ͼ���С
        /// </summary>
        /// <param name="p">��ǰ������ڵ�</param>
        private void AutoScrollAndResizeCanvas(Point p)
        {            
            Direction moveDirection = Direction.None;

            // �����ͼ�廭�涶����ֻ�����ĸ�����
            if (Math.Abs(p.X - lastSelectedPoint.X) > Math.Abs(p.Y - lastSelectedPoint.Y))
            {
                if (p.X > lastSelectedPoint.X)
                {
                    moveDirection = Direction.Right;
                }
                else if (p.X < lastSelectedPoint.X)
                {
                    moveDirection = Direction.Left;
                }
            }
            else if (Math.Abs(p.Y - lastSelectedPoint.Y) > Math.Abs(p.X - lastSelectedPoint.X))
            {
                if (p.Y > lastSelectedPoint.Y)
                {
                    moveDirection = Direction.Down;
                }
                else if (p.Y < lastSelectedPoint.Y)
                {
                    moveDirection = Direction.Up;
                }
            }

            int leftX = -1;
            int rightX = -1;
            int topY = -1;
            int bottomY = -1;            

            switch (userOperation)
            {
                case UserOperation.RegionSelect: // ��ѡͼԪ
                    {
                        leftX = p.X;
                        rightX = p.X;
                        topY = p.Y;
                        bottomY = p.Y;                        

                        break;
                    }
                case UserOperation.ScrollCanvas: // ������ͼ��
                    {                        
                        leftX = p.X - lastSelectedPoint.X - canvas.AutoScrollPosition.X;
                        rightX = p.X - lastSelectedPoint.X - canvas.AutoScrollPosition.X + canvas.ClientRectangle.Right;
                        topY = p.Y - lastSelectedPoint.Y - canvas.AutoScrollPosition.Y;
                        bottomY = p.Y - lastSelectedPoint.Y - canvas.AutoScrollPosition.Y + canvas.ClientRectangle.Bottom;

                        break;
                    }
                case UserOperation.SingleSelect: // �ƶ�����ͼԪ
                    {
                        leftX = selectedGraphElement.InvalidRectangle.Left;
                        rightX = selectedGraphElement.InvalidRectangle.Right;
                        topY = selectedGraphElement.InvalidRectangle.Top;
                        bottomY = selectedGraphElement.InvalidRectangle.Bottom;

                        break;
                    }
                case UserOperation.MultiSelect: // �ƶ����ͼԪ
                    {
                        Rectangle multiSelectRegion = canvas.CurrentMultiSelectMark.RegionRectangle;
                        leftX = multiSelectRegion.Left;
                        rightX = multiSelectRegion.Right;
                        topY = multiSelectRegion.Top;
                        bottomY = multiSelectRegion.Bottom;

                        break;
                    }
                case UserOperation.Create: // �ƶ�����ͼԪ
                    {
                        GraphElement abbreviateGraphElement = canvas.AbbreviatGraphElement;
                        leftX = abbreviateGraphElement.InvalidRectangle.Left;
                        rightX = abbreviateGraphElement.InvalidRectangle.Right;
                        topY = abbreviateGraphElement.InvalidRectangle.Top;
                        bottomY = abbreviateGraphElement.InvalidRectangle.Bottom;

                        break;
                    }
                case UserOperation.MoveRodman: // �ƶ��α�
                    {
                        break;
                    }
            }

            AutoScrollAndResizeCanvas(leftX, rightX, topY, bottomY, moveDirection);
            RecordLastSelectedPoint(p);
        }

        /// <summary>
        /// �ƶ�ѡ�е�ͼԪ
        /// </summary>
        /// <param name="moveSize">�ƶ���λ��</param>
        public void MoveSelectedGraphElement(Size moveSize)
        {
            if (selectedGraphElementList.Count > 0) // ��ѡͼԪ
            {
                foreach (GraphElement graphElement in selectedGraphElementList)
                {
                    if (graphElement is ConnectorContainer) // ��ǰͼԪ�������߿��Ƶ�����
                    {
                        ConnectorContainer line = graphElement as ConnectorContainer;

                        foreach (ConnectorGraphElement connector in line.GetConnectorList()) // �ƶ������߿��Ƶ�
                        {
                            if (!connector.Binded) // �����߿��Ƶ㲻�������ߵ�ͷ����β���
                            {
                                connector.Move(moveSize);
                            }
                            else
                            {
                                SlotGraphElement slot = connector.GetBindingSlot();

                                if (slot != null)
                                {
                                    SlotContainer slotContainer = slot.SlotContainer;

                                    if (!selectedGraphElementList.Contains(slotContainer))
                                    {
                                        FlowChartDisconnectCommand cmd = new FlowChartDisconnectCommand(this, "�������ͼԪ");
                                        InitFirstCommand(cmd);

                                        if (cmd.Execute(slot)) // ����ִ�гɹ�
                                        {
                                            AdjustCommandList(cmd);
                                        }

                                        slot.Move(moveSize);
                                    }
                                }
                            }
                        }
                    }
                    else // �������߿��Ƶ�������ͼԪֱ���ƶ�����
                    {
                        graphElement.Move(moveSize);
                    }

                    // ˢ�»�ͼ��������
                    regionManager.ChangeRegion(graphElement);
                }

                canvas.CurrentMultiSelectMark.Move(moveSize);

                Direction direction = Direction.None;
                if (moveSize.Width > 0)
                {
                    direction = Direction.Right;
                }
                else if (moveSize.Width < 0)
                {
                    direction = Direction.Left;
                }
                else if (moveSize.Height > 0)
                {
                    direction = Direction.Down;
                }
                else if (moveSize.Height < 0)
                {
                    direction = Direction.Up;
                }

                AutoScrollAndResizeCanvas(MoveType.MultiElement, direction); // �Զ�����������
                RefreshCanvas();
            }
            else if (selectedGraphElement != null) // ��ѡͼԪ
            {
                selectedGraphElement.Move(moveSize);
                Direction direction = Direction.None;

                if (moveSize.Width > 0)
                {
                    direction = Direction.Right;
                }
                else if (moveSize.Width < 0)
                {
                    direction = Direction.Left;
                }
                else if(moveSize.Height > 0)
                {
                    direction = Direction.Down;
                }
                else if(moveSize.Height < 0)
                {
                    direction = Direction.Up;
                }

                // ��ʾ���������
                if (selectedGraphElement is SlotContainer)
                {
                    CreateAdjustLine(selectedGraphElement as SlotContainer, direction, false);
                }        
        
                AutoScrollAndResizeCanvas(MoveType.SingleElement, direction); // �Զ�����������

                // ˢ�»�ͼ��������
                regionManager.ChangeRegion(selectedGraphElement);

                RefreshCanvas();
            }
            else // ����������
            {
                ScrollCanvas(moveSize, true);
            }
        }

        /// <summary>
        /// ��������ڵ�����ת��Ϊ��ͼ����
        /// </summary>
        /// <param name="mouseLocation">������ڵ�����</param>
        /// <returns>��ͼ����</returns>
        private Point AdjustMouseLocation(Point mouseLocation)
        {
            // ����ת����������������������  
            Point newLocation = mouseLocation - new Size(canvas.AutoScrollPosition);          
            
            // ���Ż�ͼ�����
            newLocation.X = (int)(newLocation.X / graphSetting.CanvasScale);
            newLocation.Y = (int)(newLocation.Y / graphSetting.CanvasScale);

            return newLocation;
        }

        /// <summary>
        /// �ƶ�ͼԪ
        /// </summary>
        /// <param name="p">��굱ǰ���ڵ�</param>
        public void MoveGraphElement(Point point)
        {
            Point p = AdjustMouseLocation(point);            

            if (lastActivatedGraphElement != null) // ����һ���������ͼԪ�ۻ�
            {
                lastActivatedGraphElement.Activated = false;

                if (lastActivatedGraphElement is SlotContainer) // ȡ����ǲ��������Χ��������
                {
                    MarkRelavateLines(lastActivatedGraphElement as SlotContainer, false);
                }
            }            

            switch (userOperation)
            {
                case UserOperation.None:
                    {                                                                    
                        ActivateGraphElement(p); // ����ͼԪ                  

                        break;
                    }
                case UserOperation.ScrollCanvas: // ������ͼ��
                    {                                                        
                        AutoScrollAndResizeCanvas(p);

                        break;
                    }
                case UserOperation.RegionSelect:
                    {
                        Rectangle multiSelectRectangle = GetMultiSelectRectangle(p, multiSelectStartPoint);
                        canvas.CurrentMultiSelectMark.Adjust(multiSelectRectangle.Location, multiSelectRectangle.Size);
                        canvas.CurrentMultiSelectMark.ShowBackground = true;
                        canvas.CurrentMultiSelectMark.Visible = true;
                        AutoScrollAndResizeCanvas(p);

                        break;
                    }
                case UserOperation.Create:
                    {
                        GraphElement abbreviateGraphElement = canvas.AbbreviatGraphElement;

                        if (abbreviateGraphElement != null) // ��ʾ���Ե�ͼԪ
                        {                            
                            canvas.TooltipGraphElement.Visible = false;
                            Size moveSize = new Size(p.X - lastSelectedPoint.X, p.Y - lastSelectedPoint.Y);
                            abbreviateGraphElement.Move(moveSize);

                            if (!abbreviateGraphElement.IsInRegion(p)) // ����Ƿ���Ҫ��������ͼԪ
                            {
                                AdjustAbbreviatGraphElement(p);
                            }

                            if (autoConnect) // ��Ҫ�޸�����ͼԪ�����ߵ�λ��
                            {
                                int headX = (int)(canvas.AbbreviatGraphElement.Location.X + canvas.AbbreviatGraphElement.ElementSize.Width / 2);
                                int headY = (int)(canvas.AbbreviatGraphElement.Location.Y);
                                canvas.AbbreviateLine.HeadConnector.Location = new Point(headX, headY);
                            }

                            AutoScrollAndResizeCanvas(p); // ��������������ͼ���С                            
                        }

                        break;
                    }
                case UserOperation.SingleSelect:
                    {
                        if (p != lastSelectedPoint)
                        {
                            List<GraphElement> graphElementList = new List<GraphElement>();
                            graphElementList.Add(selectedGraphElement);

                            DragGraphElement(graphElementList, p, MoveType.SingleElement);
                            AutoScrollAndResizeCanvas(p); // ��������������ͼ���С
                            canvas.TooltipGraphElement.Visible = false;
                        }

                        break;
                    }
                case UserOperation.MultiSelect:
                    {
                        if (p != lastSelectedPoint)
                        {
                            DragGraphElement(selectedGraphElementList, p, MoveType.MultiElement);
                            AutoScrollAndResizeCanvas(p); // ��������������ͼ���С
                            canvas.TooltipGraphElement.Visible = false;
                        }

                        break;
                    }
                case UserOperation.MoveRodman:
                    {
                        if (p != lastSelectedPoint)
                        {
                            canvas.CurrentRodman.Init(p, canvas.VisibleRectangle);
                            MoveMultiGraphElement(moveGraphElementList, p);
                            AutoScrollAndResizeCanvas(p); // ��������������ͼ���С
                            canvas.TooltipGraphElement.Visible = false;
                        }
                        

                        break;
                    }
            }            
            
            RefreshCanvas();
        }

        /// <summary>
        /// �ƶ����ͼԪ
        /// </summary>
        /// <param name="graphElementList">ͼԪ����</param>
        /// <param name="p">��굱ǰ���ڵ�</param>
        private void MoveMultiGraphElement(List<GraphElement> graphElementList, Point p)
        {
            Size moveSize = new Size(0, 0);

            switch (canvas.CurrentRodman.CurrentRodmanType)
            {
                case RodmanType.None:
                    {
                        break;
                    }
                case RodmanType.Horizonal:
                    {
                        moveSize.Width = p.X - lastSelectedPoint.X;
                        break;
                    }
                case RodmanType.Vertical:
                    {
                        moveSize.Height = p.Y - lastSelectedPoint.Y;
                        break;
                    }
            }
            
            foreach (GraphElement graphElement in graphElementList)
            {
                if (graphElement is ConnectorContainer) // ��ǰͼԪ�������߿��Ƶ�����
                {
                    ConnectorContainer line = graphElement as ConnectorContainer;
                    line.Visible = false;

                    foreach (ConnectorGraphElement connector in line.GetConnectorList()) // �ƶ������߿��Ƶ�
                    {
                        if (!connector.Binded) // �����߿��Ƶ㲻�������ߵ�ͷ����β���
                        {
                            connector.Move(moveSize);
                        }
                    }
                }
                else if (graphElement is SlotContainer) // ��ǰͼԪ�ǲ������
                {
                    SlotContainer slotContainer = graphElement as SlotContainer;

                    slotContainer.RefreshRelevateLine(false);
                    graphElement.Move(moveSize);
                }
                else // �������߿��Ƶ�������ͼԪֱ���ƶ�����
                {
                    graphElement.Move(moveSize);
                }

                graphElement.Moving = true;
            }

            multiMoveRegion.Location += moveSize;       
        }

        /// <summary>
        /// �ӳ��ƶ�ͼԪ
        /// </summary>
        /// <param name="direction">�ӳ��ƶ�����</param>
        /// <param name="lagTime">�ӳ�ʱ��</param>     
        /// <returns>�Ƿ��ƶ��ɹ�</returns>
        private bool LagMove(Direction direction, int lagPosition)
        {
            if (direction == lastLagDirection && lagPosition == lastLagPosition)
            {
                moveLagged = false;
            }
            else
            {                
                this.lastLagDirection = direction;
                this.lastLagPosition = lagPosition;
                lastMoveTime = DateTime.Now.Ticks;
                moveLagged = true;    
            }

            return moveLagged;
        }

        /// <summary>
        /// ����ƶ��ӳ�
        /// </summary>
        /// <param name="direction">�ƶ��ķ���</param>
        /// <returns>�Ƿ������ƶ�</returns>
        private bool CheckMoveLag(Direction direction)
        {
            bool enableMove = true;
            lockMoveDrag = false;

            if (DateTime.Now.Ticks - lastMoveTime < moveLagTime * 10000)
            {
                if (moveLagged && direction == lastLagDirection)
                {
                    enableMove = false;
                    lockMoveDrag = true;
                }                
            }
            else // ��ʱ����
            {
                lastLagDirection = Direction.None;
                lastLagPosition = -1;
            }

            return enableMove;
        }

        /// <summary>
        /// ��¼��һ��ѡ�еĵ�
        /// </summary>
        /// <param name="currentLocation">��ǰ������</param>
        private void RecordLastSelectedPoint(Point currentLocation)
        {
            if (!lockMoveDrag)
            {
                lastSelectedPoint = currentLocation;
            }
        }        

        /// <summary>
        /// �϶�ͼԪ
        /// </summary>
        /// <param name="p">��굱ǰ���ڵ�</param>
        /// <param name="moveType">�ƶ�����</param>
        private void DragGraphElement(List<GraphElement> graphElementList, Point p, MoveType moveType)
        {            
            Size moveSize = new Size(p.X - lastSelectedPoint.X, p.Y - lastSelectedPoint.Y);

            // �����ƶ�����
            Direction direction = Direction.None;

            if (moveSize.Width > 0)
            {
                direction = Direction.Right;
            }
            else if (moveSize.Width < 0)
            {
                direction = Direction.Left;
            }
            else if (moveSize.Height > 0)
            {
                direction = Direction.Down;
            }
            else if (moveSize.Height < 0)
            {
                direction = Direction.Up;
            }

            if (!CheckMoveLag(direction))
            {
                // �������λ��
                Cursor.Position = canvas.PointToScreen(lastSelectedPoint) + new Size(canvas.AutoScrollPosition);
                return;
            }

            switch (moveType)
            {
                case MoveType.SingleElement:
                    {
                        GraphElement graphElement = graphElementList[0];

                        graphElement.Move(moveSize); // �ƶ�ѡ�е�ͼԪ

                        if (graphElement is ConnectorGraphElement) // ��ǰ�϶���ͼԪ�������߿��Ƶ�
                        {
                            ConnectorGraphElement connector = graphElement as ConnectorGraphElement;
                            bool activated = false; // �Ƿ��в�۱�����

                            if (connector.IsHeadPoint || connector.IsTailPoint) // ��ǰ�϶��������߿��Ƶ���ͷ������β���
                            {
                                List<SlotContainer> regionSlotContainerList = regionManager.GetSlotContainerList(connector.Location);

                                foreach (SlotContainer slotContainer in regionSlotContainerList)
                                {
                                    if (activated) // �Ѿ��в�ۻ������������ֱ������
                                    {
                                        break;
                                    }

                                    SlotGraphElement slot = slotContainer.GetSlotInRegion(p);

                                    if (slot != null) // ���ǰ���
                                    {
                                        slot.Activated = true;
                                        activated = true;
                                        lastActivatedGraphElement = slot;
                                        break;
                                    }

                                    if (slotContainer.IsInRegion(p)) // ���ǰ�������
                                    {
                                        slotContainer.Activated = true;
                                        activated = true;
                                        lastActivatedGraphElement = slotContainer;
                                        break;
                                    }
                                }
                            }

                            connector.Line.Moving = true;

                            // �Զ�����������                                
                            connector.Line.AdjustLine(connector);
                        }
                        else if (graphElement is SlotContainer) // ��ǰ�϶���ͼԪ�ǲ������
                        {
                            SlotContainer slotContainer = graphElement as SlotContainer;
                            
                            slotContainer.RefreshRelevateLine(false);
                            CreateAdjustLine(slotContainer, direction, true);
                        }
                        else if (graphElement is ResizeControler) // ��ǰ�϶��������ſ��Ƶ�
                        {
                            SlotContainer slotContainer = (graphElement as ResizeControler).Owner;
                            CreateAdjustLine(slotContainer, direction, false);
                        }

                        graphElement.Moving = true;

                        break;
                    }
                case MoveType.MultiElement:
                    {                        
                        foreach (GraphElement graphElement in graphElementList)
                        {
                            if (graphElement is ConnectorContainer) // ��ǰͼԪ�������߿��Ƶ�����
                            {
                                ConnectorContainer line = graphElement as ConnectorContainer;
                                line.Visible = false;

                                foreach (ConnectorGraphElement connector in line.GetConnectorList()) // �ƶ������߿��Ƶ�
                                {
                                    if (!connector.Binded) // �����߿��Ƶ㲻�������ߵ�ͷ����β���
                                    {
                                        connector.Move(moveSize);
                                    }
                                    else
                                    {
                                        SlotGraphElement slot = connector.GetBindingSlot();

                                        if (slot != null)
                                        {
                                            SlotContainer slotContainer = slot.SlotContainer;

                                            if (!graphElementList.Contains(slotContainer))
                                            {
                                                FlowChartDisconnectCommand cmd = new FlowChartDisconnectCommand(this, "�������ͼԪ");
                                                InitFirstCommand(cmd);

                                                if (cmd.Execute(slot)) // ����ִ�гɹ�
                                                {
                                                    AdjustCommandList(cmd);
                                                }

                                                slot.Move(moveSize);
                                            }
                                        }                                        
                                    }
                                }
                            }
                            else if (graphElement is SlotContainer) // ��ǰͼԪ�ǲ������
                            {
                                SlotContainer slotContainer = graphElement as SlotContainer;
                                
                                slotContainer.RefreshRelevateLine(false);
                                graphElement.Move(moveSize);
                            }
                            else // �������߿��Ƶ�������ͼԪֱ���ƶ�����
                            {
                                graphElement.Move(moveSize);
                            }

                            graphElement.Moving = true;
                        }

                        canvas.CurrentMultiSelectMark.Move(moveSize);

                        break;
                    }
            }
        }       

        /// <summary>
        /// ��������������
        /// </summary>
        /// <param name="slotContaienr">��ǰ�������</param>
        private void CreateAdjustLine(SlotContainer slotContainer, Direction direction, bool autoAdjust)
        {
            canvas.CurrentGuideLine.ClearGuideLineList();

            Rectangle rectangle = Rectangle.Empty;
            rectangle.Location = canvas.ClientRectangle.Location - (Size)canvas.AutoScrollPosition;
            rectangle.Size = canvas.ClientSize;
            List<SlotContainer> list = regionManager.GetSlotContainerList(rectangle);            
            int leftX = -1;
            int rightX = -1;
            int topY = -1;
            int bottomY = -1;

            canvas.PaintCanvas = false;

            foreach(SlotContainer s in list)
            {
                if (s != slotContainer)
                {
                    // ��۵�����
                    if (s.TextRectangle.Left < slotContainer.TextRectangle.Right &&
                        s.TextRectangle.Right > slotContainer.TextRectangle.Left)
                    {
                        if (s.TextRectangle.Bottom < slotContainer.TextRectangle.Top)
                        {
                            foreach (SlotGraphElement slot1 in s.GetOutSlotList())
                            {
                                foreach (SlotGraphElement slot2 in slotContainer.GetInSlotList())
                                {
                                    if (slot1.Location.X == slot2.Location.X)
                                    {
                                        canvas.CurrentGuideLine.AddGuideLine(new Point[] { slot1.Location, slot2.Location });
                                    }
                                }
                            }
                        }

                        if (s.TextRectangle.Top > slotContainer.TextRectangle.Bottom)
                        {
                            foreach (SlotGraphElement slot1 in s.GetInSlotList())
                            {
                                foreach (SlotGraphElement slot2 in slotContainer.GetOutSlotList())
                                {
                                    if (slot1.Location.X == slot2.Location.X)
                                    {
                                        canvas.CurrentGuideLine.AddGuideLine(new Point[] { slot1.Location, slot2.Location });
                                    }
                                }
                            }
                        }
                    }                   

                    // ��ʾ�߿������
                    switch (direction)
                    {
                        case Direction.Left:
                            {                                
                                if (slotContainer.TextRectangle.Left - s.TextRectangle.Left >= 0 &&
                                    slotContainer.TextRectangle.Left - s.TextRectangle.Left < moveAdjustWidth)
                                {
                                    canvas.CurrentGuideLine.AddGuideLine(new Point[]{ new Point(s.TextRectangle.Left, canvas.VisibleRectangle.Top), 
                                                                                      new Point(s.TextRectangle.Left, canvas.VisibleRectangle.Bottom)});   
                                    
                                    if (s.TextRectangle.Left < leftX || leftX == -1)
                                    {
                                        leftX = s.TextRectangle.Left;
                                    }
                                }

                                break;
                            }
                        case Direction.Right:
                            {
                                if (s.TextRectangle.Right - slotContainer.TextRectangle.Right >= 0 &&
                                    s.TextRectangle.Right - slotContainer.TextRectangle.Right < moveAdjustWidth)
                                {
                                    canvas.CurrentGuideLine.AddGuideLine(new Point[]{ new Point(s.TextRectangle.Right, canvas.VisibleRectangle.Top), 
                                                                                      new Point(s.TextRectangle.Right, canvas.VisibleRectangle.Bottom)});

                                    if (s.TextRectangle.Right > rightX || rightX == -1)
                                    {
                                        rightX = s.TextRectangle.Right;
                                    }
                                }

                                break;
                            }
                        case Direction.Up:
                            {
                                if (slotContainer.TextRectangle.Top - s.TextRectangle.Top >= 0 &&
                                    slotContainer.TextRectangle.Top - s.TextRectangle.Top < moveAdjustWidth)
                                {
                                    canvas.CurrentGuideLine.AddGuideLine(new Point[]{ new Point(canvas.VisibleRectangle.Left, s.TextRectangle.Top), 
                                                                                      new Point(canvas.VisibleRectangle.Right, s.TextRectangle.Top)});

                                    if (s.TextRectangle.Top < topY || topY == -1)
                                    {
                                        topY = s.TextRectangle.Top;
                                    }
                                }

                                break;
                            }
                        case Direction.Down:
                            {
                                if (s.TextRectangle.Bottom - slotContainer.TextRectangle.Bottom >= 0 &&
                                    s.TextRectangle.Bottom - slotContainer.TextRectangle.Bottom < moveAdjustWidth)
                                {
                                    canvas.CurrentGuideLine.AddGuideLine(new Point[]{ new Point(canvas.VisibleRectangle.Left, s.TextRectangle.Bottom), 
                                                                                      new Point(canvas.VisibleRectangle.Right, s.TextRectangle.Bottom)});

                                    if (s.TextRectangle.Bottom > bottomY || bottomY == -1)
                                    {
                                        bottomY = s.TextRectangle.Bottom;
                                    }
                                }

                                break;
                            }
                    }                                      
                }                
            }               
            
            if (autoAdjust) // �Զ�����λ��
            {
                bool requireAdjust = false;
                Size moveSize = Size.Empty;
                int lagPosition = -1;

                switch (direction)
                {
                    case Direction.Left:
                        {
                            if (leftX != -1)
                            {
                                moveSize.Width = leftX - slotContainer.TextRectangle.Left;
                                lagPosition = leftX;

                                if (moveSize.Width < moveLagOffset)
                                {
                                    requireAdjust = true;
                                }                                
                            }

                            break;
                        }
                    case Direction.Right:
                        {
                            if (rightX != -1)
                            {
                                moveSize.Width = rightX - slotContainer.TextRectangle.Right;
                                lagPosition = rightX;

                                if (moveSize.Width < moveLagOffset)
                                {
                                    requireAdjust = true;
                                }   
                            }

                            break;
                        }
                    case Direction.Up:
                        {
                            if (topY != -1)
                            {
                                moveSize.Height = topY - slotContainer.TextRectangle.Top;
                                lagPosition = topY;

                                if (moveSize.Height < moveLagOffset)
                                {
                                    requireAdjust = true;
                                }
                            }

                            break;
                        }
                    case Direction.Down:
                        {
                            if (bottomY != -1)
                            {
                                moveSize.Height = bottomY - slotContainer.TextRectangle.Bottom;
                                lagPosition = bottomY;

                                if (moveSize.Height < moveLagOffset)
                                {
                                    requireAdjust = true;
                                }   
                            }

                            break;
                        }
                    default:
                        {
                            break;
                        }
                }

                if (requireAdjust)
                {
                    slotContainer.Move(moveSize);

                    if (LagMove(direction, lagPosition)) // �ƶ��ӳ� 
                    {
                        Cursor.Position += moveSize;     
                    }
                }
            }            

            canvas.PaintCanvas = true;
        }      

        /// <summary>
        /// �ı����ָ�����״
        /// </summary>
        /// <param name="direction">��ǰ�ķ���</param>
        private void ChangeMouseCursur(Direction direction)
        {
            switch (direction) // �ı����ָ��
            {
                case Direction.Up:
                    {
                        canvas.Cursor = Cursors.SizeNS;
                        break;
                    }
                case Direction.Up_Left:
                    {
                        canvas.Cursor = Cursors.SizeNWSE;
                        break;
                    }
                case Direction.Left:
                    {
                        canvas.Cursor = Cursors.SizeWE;
                        break;
                    }
                case Direction.Down_Left:
                    {
                        canvas.Cursor = Cursors.SizeNESW;
                        break;
                    }
                case Direction.Down:
                    {
                        canvas.Cursor = Cursors.SizeNS;
                        break;
                    }
                case Direction.Down_Right:
                    {
                        canvas.Cursor = Cursors.SizeNWSE;
                        break;
                    }
                case Direction.Right:
                    {
                        canvas.Cursor = Cursors.SizeWE;
                        break;
                    }
                case Direction.Up_Right:
                    {
                        canvas.Cursor = Cursors.SizeNESW;
                        break;
                    }
            }
        }

        /// <summary>
        /// ѡ�е�ǰ���������ߵ���ڲ������
        /// </summary>
        public void SelectLineInSlotContainer()
        {
            ConnectorContainer line = lastSelectedGraphElement as ConnectorContainer;

            if (line != null) // �ȼ�鱻ѡ�е�ͼԪ
            {
                if (line.InSlotContainer != null)
                {
                    SelectGraphElement(line.InSlotContainer, true);
                }
            }
            else 
            {
                line = lastActivatedGraphElement as ConnectorContainer;

                if (line != null) // �ټ�鱻�����ͼԪ
                {
                    if (line.InSlotContainer != null)
                    {
                        SelectGraphElement(line.InSlotContainer, true);
                    }
                }
            }
        }

        /// <summary>
        /// ѡ�е�ǰ���������ߵĳ��ڲ������
        /// </summary>
        public void SelectLineOutSlotContainer()
        {
            ConnectorContainer line = lastSelectedGraphElement as ConnectorContainer;

            if (line != null) // �ȼ�鱻ѡ�е�ͼԪ
            {
                if (line.OutSlotContainer != null)
                {
                    SelectGraphElement(line.OutSlotContainer, true);
                }
            }
            else 
            {
                line = lastActivatedGraphElement as ConnectorContainer;

                if (line != null) // �ټ�鱻�����ͼԪ
                {
                    if (line.OutSlotContainer != null)
                    {
                        SelectGraphElement(line.OutSlotContainer, true);
                    }
                }
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="p">��굱ǰ���ڵ�</param>
        private void ActivateGraphElement(Point p)
        {
            bool activated = false; // �Ƿ���ͼԪ������
            canvas.TooltipGraphElement.Visible = false; // �����ı���ʾ                  
            
            // ����һ�ζ�·���
            if (lastActivatedGraphElement != null)
            {
                if (lastActivatedGraphElement is ConnectorContainer)
                {
                    ConnectorContainer line = lastActivatedGraphElement as ConnectorContainer;
                    ConnectorGraphElement connector = line.GetConnectorInRegion(p);

                    if (connector != null)
                    {
                        lastActivatedGraphElement = connector;
                        activated = true;
                    }
                    else
                    {
                        if (line.IsInRegion(p))
                        {
                            activated = true;
                        }                        
                    }
                }
                else if (lastActivatedGraphElement is SlotContainer)
                {
                    SlotContainer node = lastActivatedGraphElement as SlotContainer;
                    SlotGraphElement slot = node.GetSlotInRegion(p);

                    if (slot != null)
                    {
                        lastActivatedGraphElement = slot;
                        activated = true;
                    }
                    else
                    {
                        if (node.IsInRegion(p))
                        {
                            activated = true;
                        }
                    }
                }
                else if (lastActivatedGraphElement is Rodman)
                {
                    ; // �α���Ҫ���⴦��
                }
                else
                {
                    if (lastActivatedGraphElement.IsInRegion(p))
                    {
                        activated = true;
                    }
                }                
            }

            if (!activated)
            {
                // ����Ƿ񼤻��α�
                canvas.CurrentRodman.Init(p, canvas.VisibleRectangle);

                if (canvas.CurrentRodman.IsInRegion(p))
                {
                    lastActivatedGraphElement = canvas.CurrentRodman;
                    activated = true;
                }
            }            

            // ����Ƿ��������߿��Ƶ㱻����
            foreach (ConnectorContainer connectorContainer in regionManager.GetConnectorContainerList(p))
            {
                if (activated) // ����Ѿ���ͼԪ�������ֱ������ѭ��
                {
                    break;
                }

                ConnectorGraphElement connector = connectorContainer.GetConnectorInRegion(p);

                if (connector != null)
                {
                    lastActivatedGraphElement = connector;
                    activated = true;
                    break;
                }
                
                if (connectorContainer.IsInRegion(p)) // ����������Ƿ���Լ���
                {
                    lastActivatedGraphElement = connectorContainer;
                    activated = true;
                    break;
                }                        
            }

            bool resizeControlerActivated = false; // �Ƿ������ſ��Ƶ㱻����

            foreach (SlotContainer slotContainer in regionManager.GetSlotContainerList(p)) // ����Ƿ��в������������
            {
                if (activated) // ����Ѿ���ͼԪ�������ֱ������ѭ��
                {
                    break;
                }

                ResizeControler resizeControler = slotContainer.GetResizeControlerInRegion(p);
                
                if (resizeControler != null)
                {
                    resizeControlerActivated = true;
                    activated = true;
                    lastActivatedGraphElement = resizeControler;
                    ChangeMouseCursur(resizeControler.CurrentDirection); // �ı����ָ����״
                    break;
                }                

                // ������������İ�ť�Ƿ񱻼���
                BaseButton baseButton = slotContainer.GetButtonInRegion(p);

                if (baseButton != null)
                {
                    activated = true;
                    lastActivatedGraphElement = baseButton;
                    break;
                }                

                // ����Ƿ���ע��ͼԪ��ѡ��
                if (slotContainer.RemarkNode != null)
                {
                    if (slotContainer.RemarkNode.IsInRegion(p))
                    {
                        lastActivatedGraphElement = slotContainer.RemarkNode;
                        activated = true;
                        break;
                    }
                }

                if (slotContainer.IsInRegion(p)) // ��鵱ǰ���Ƿ���ͼԪ������
                {
                    SlotGraphElement slot = slotContainer.GetSlotInRegion(p);

                    if (slot != null)
                    {
                        activated = true;
                        lastActivatedGraphElement = slot;
                        break;
                    }
                    
                    activated = true;
                    lastActivatedGraphElement = slotContainer;
                    break;                    
                }
            }

            if (activated) // ��ͼԪ������
            {
                lastActivatedGraphElement.Activated = true; // ����ͼԪ                

                if (lastActivatedGraphElement is SlotContainer) // ȡ����ǲ��������Χ��������
                {
                    MarkRelavateLines(lastActivatedGraphElement as SlotContainer, true);
                }

                // ��ʼ���ı���ʾ
                InitTooltipText(lastActivatedGraphElement, p);
            }

            if (!resizeControlerActivated) // �ָ���ͼ���Ĭ��ָ����ʽ
            {
                canvas.Cursor = Cursors.Default;
            }
        }

        /// <summary>
        /// ���ɶ�ѡͼԪ�ı�ʶ����
        /// </summary>
        /// <param name="p1">���εĶԽǵ�</param>
        /// <param name="p2">���εĶԽǵ�</param>
        /// <returns>��ʶ����</returns>
        private Rectangle GetMultiSelectRectangle(Point p1, Point p2)
        {
            int x1 = p1.X;
            int y1 = p1.Y;
            int x2 = p2.X;
            int y2 = p2.Y;
            int sizeWidth;
            int sizeHeight;
            int pointX;
            int pointY;

            if(x1 > x2)
            {
                pointX = x2;
                sizeWidth = x1 - x2;
            }
            else
            {
                pointX = x1;
                sizeWidth = x2 - x1;
            }
            
            if(y1 > y2)
            {
                pointY = y2;
                sizeHeight = y1 - y2;
            }
            else
            {
                pointY = y1;
                sizeHeight = y2 - y1;
            }

            Point multiSelectPoint = new Point(pointX, pointY);
            Size multiSelectSize = new Size(sizeWidth, sizeHeight);
            Rectangle multiSelectRectangle = new Rectangle(multiSelectPoint, multiSelectSize);

            return multiSelectRectangle;
        }

        /// <summary>
        /// ѡ�в���¼ͼԪ
        /// </summary>
        /// <param name="graphElement">ͼԪ</param>
        /// <param name="p">��굱ǰ���ڵ�</param>
        private void SelectGraphElement(GraphElement graphElement, Point p)
        {
            graphElement.Selected = true;
            selectedGraphElement = graphElement;
            lastSelectedGraphElement = graphElement;
            moveOffset.Width = p.X - graphElement.Location.X;
            moveOffset.Height = p.Y - graphElement.Location.Y;            
            documentManager.ShowObjectProperty(graphElement);

            // ����Move������浱ǰ״̬
            moveCommand = new FlowChartMoveCommand(this, "�ƶ�ͼԪ");

            if (commandIndex == -1) // ����Ƿ��ǵ�һ������
            {
                moveCommand.FirstCommand = true;
                moveCommand.SaveCurrentState(p);
            }            
        }

        /// <summary>
        /// ѡ��ͼԪ
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        /// <param name="showMultiSelectRegion">�Ƿ���ʾ��ѡ����</param>
        public void SelectGraphElement(GraphElement graphElement, bool showMultiSelectRegion)
        {
            if (lastSelectedGraphElement != null) // ����һ����ѡ�е�ͼԪ��Ϊ��ѡ��״̬
            {
                if (!selectedGraphElementList.Contains(lastSelectedGraphElement)) // �ڶ�ѡ״̬��Ҫ�����һ����ѡ�е�ͼԪ�Ƿ��ڶ�ѡ������
                {
                    lastSelectedGraphElement.Selected = false;
                }
            }

            if (lastResizingGraphElement != null) // ����һ�����ŵĲ��������Ϊ������״̬
            {
                lastResizingGraphElement.Resizing = false;
            }

            // ���ѡ�е�ͼԪ����
            ClearSelectedGraphElementList();

            // ����֣�Control���кܴ��ʻᱻ��Ϊ�ǰ��µģ�����������һ��
            controlMode = false;

            graphElement.Selected = true;
            selectedGraphElement = graphElement;
            lastSelectedGraphElement = graphElement;            
            documentManager.ShowObjectProperty(graphElement);

            // ������ѡ������ʾͻ����ʾ
            if (showMultiSelectRegion)
            {
                canvas.CurrentMultiSelectMark.Adjust(graphElement.Location + new Size(-5, -5), graphElement.ElementSize + new Size(10, 10));
                canvas.CurrentMultiSelectMark.ShowBackground = false;
                canvas.CurrentMultiSelectMark.Visible = true;

                // ��Ļ�Զ���������
                AutoScrollToPoint(graphElement.Location - new Size((canvas.ClientSize.Width - graphElement.ElementSize.Width) / 2,
                                  (canvas.ClientSize.Height - graphElement.ElementSize.Height) / 2));
            }                        

            // ˢ�»�ͼ            
            RefreshCanvas(graphElement.InvalidRectangle);
        }

        /// <summary>
        /// ������ѡ����
        /// </summary>
        private void CreateMultiSelectRegion()
        {
            int left = -1;
            int right = -1;
            int top = -1;
            int buttom = -1;

            foreach (GraphElement graphElement in selectedGraphElementList)
            {
                if(graphElement.Location.X < left || left == -1)
                {
                    left = graphElement.Location.X;
                }

                if(graphElement.Location.X + graphElement.ElementSize.Width > right || right == -1)
                {
                    right = graphElement.Location.X + graphElement.ElementSize.Width;
                }

                if(graphElement.Location.Y < top || top == -1)
                {
                    top = graphElement.Location.Y;
                }

                if(graphElement.Location.Y + graphElement.ElementSize.Height > buttom || buttom == -1)
                {
                    buttom = graphElement.Location.Y + graphElement.ElementSize.Height;
                }
            }

            canvas.CurrentMultiSelectMark.Adjust(new Point(left - 5, top - 5), new Size(right - left + 10, buttom - top + 10));
            canvas.CurrentMultiSelectMark.ShowBackground = false;
            canvas.CurrentMultiSelectMark.Visible = true;
        }       

        /// <summary>
        /// ����ͼԪ�ƶ�����
        /// </summary>
        private void CreateMultiMoveRegion()
        {
            int left = -1;
            int right = -1;
            int top = -1;
            int buttom = -1;

            foreach (GraphElement graphElement in moveGraphElementList)
            {
                if (graphElement.Location.X < left || left == -1)
                {
                    left = graphElement.Location.X;
                }

                if (graphElement.Location.X + graphElement.ElementSize.Width > right || right == -1)
                {
                    right = graphElement.Location.X + graphElement.ElementSize.Width;
                }

                if (graphElement.Location.Y < top || top == -1)
                {
                    top = graphElement.Location.Y;
                }

                if (graphElement.Location.Y + graphElement.ElementSize.Height > buttom || buttom == -1)
                {
                    buttom = graphElement.Location.Y + graphElement.ElementSize.Height;
                }
            }

            multiMoveRegion = new Rectangle(new Point(left - 5, top - 5), new Size(right - left + 10, buttom - top + 10));
        }

        /// <summary>
        /// ѡ�����е�ͼԪ
        /// </summary>
        public void SelectAllGraphElement()
        {
            selectedGraphElementList.Clear();

            foreach(GraphElement graphElement in slotContainerList)
            {
                graphElement.Selected = true;
                selectedGraphElementList.Add(graphElement);
            }

            foreach(GraphElement graphElement in connectorContainerList)
            {
                graphElement.Selected = true;
                selectedGraphElementList.Add(graphElement);
            }

            CreateMultiSelectRegion();
            RefreshCanvas();
        }                          
        
        /// <summary>
        /// ѡ�������е�����ͼԪ
        /// </summary>
        /// <param name="list">����ͼԪ������</param>
        public void SelectGraphElements(List<GraphElement> list)
        {
            ClearSelectedGraphElementList();

            foreach (GraphElement node in list)
            {
                node.Selected = true;
                selectedGraphElementList.Add(node);
            }

            CreateMultiSelectRegion();

            Rectangle multiSelectRegion = canvas.CurrentMultiSelectMark.RegionRectangle;
            AutoScrollToPoint(multiSelectRegion.Location - new Size((canvas.ClientSize.Width - multiSelectRegion.Width) / 2,
                              (canvas.ClientSize.Height - multiSelectRegion.Height) / 2));
            RefreshCanvas();
        }

        /// <summary>
        /// �Զ���������������ĳ������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        private void AutoScrollToPoint(Point p)
        {
            int horizontalValue = p.X + canvas.AutoScrollPosition.X;
            int verticalValue = p.Y + canvas.AutoScrollPosition.Y;
            canvas.Visible = false; // ǿ��ˢ�¹�����

            if (canvas.HorizontalScroll.Value + horizontalValue >= canvas.HorizontalScroll.Minimum &&
                            canvas.HorizontalScroll.Value + horizontalValue <= canvas.HorizontalScroll.Maximum)
            {
                canvas.HorizontalScroll.Value += horizontalValue;
            }
            else
            {
                if (horizontalValue > 0) // ���ҹ���
                {
                    canvas.HorizontalScroll.Value = canvas.HorizontalScroll.Maximum;
                }
                else // �������
                {
                    canvas.HorizontalScroll.Value = canvas.HorizontalScroll.Minimum;
                }
            }

            if (canvas.VerticalScroll.Value + verticalValue >= canvas.VerticalScroll.Minimum &&
                            canvas.VerticalScroll.Value + verticalValue <= canvas.VerticalScroll.Maximum)
            {
                canvas.VerticalScroll.Value += verticalValue;
            }
            else
            {
                if (verticalValue > 0) // ���¹���
                {
                    canvas.VerticalScroll.Value = canvas.VerticalScroll.Maximum;
                }
                else // ���Ϲ���
                {
                    canvas.VerticalScroll.Value = canvas.VerticalScroll.Minimum;
                }
            }

            canvas.Visible = true;
            canvas.Focus();
        }

        /// <summary>
        /// ��ʼ������ͼ��
        /// </summary>
        /// <param name="point">��ǰ���λ��</param>
        public void BeginScrollCavas(Point point)
        {
            RecordLastSelectedPoint(AdjustMouseLocation(point));
            canvas.Cursor = Cursors.SizeAll;
            userOperation = UserOperation.ScrollCanvas;
        }

        /// <summary>
        /// ѡ�������ڵ�����ͼԪ
        /// </summary>
        /// <param name="rectangle">����</param>
        private void SelectGraphElementInRegion(Rectangle rectangle)
        {
            foreach (GraphElement graphElement in canvas.GraphElementList)
            {
                if (graphElement.IsInRegion(rectangle)) // ��ͼԪ�ڶ�ѡ������
                {
                    graphElement.Selected = true;
                    selectedGraphElementList.Add(graphElement);
                }
            }

            if (selectedGraphElementList.Count > 0) // ������ѡͼԪ��ѡ������
            {
                CreateMultiSelectRegion();
            }
            else
            {
                canvas.CurrentMultiSelectMark.Visible = false; // ���ض�ѡ����
            }
        }

        /// <summary>
        /// ȷ���ƶ�����
        /// </summary>
        /// <param name="p">��굱ǰ���ڵ�</param>
        private void ConfirmLocation(Point point)
        {
            Point p = AdjustMouseLocation(point);
            Rectangle multiSelectRegion = canvas.CurrentMultiSelectMark.RegionRectangle;

            switch (userOperation) // ����û���ǰ����״̬
            {                        
                case UserOperation.RegionSelect:
                    {
                        SelectGraphElementInRegion(multiSelectRegion);

                        break;
                    }
                case UserOperation.SingleSelect:
                    {
                        bool bind = false; // �Ƿ��Ѿ���ͼԪ
                        bool unbind = false; // �Ƿ��Ѿ������ͼԪ
                        selectedGraphElement.Moving = false;

                        // �Զ�������ͼ���С
                        AdjustOutOfBorder();

                        if (selectedGraphElement is ConnectorGraphElement) // �϶����������߿��Ƶ�
                        {
                            ConnectorGraphElement connector = selectedGraphElement as ConnectorGraphElement;
                            SlotContainer slotContainer;
                            SlotGraphElement slot = null;

                            // �Ƚ���������߿��Ƶ�
                            if (connector.Binded)
                            {
                                slot = connector.GetBindingSlot();                                

                                if (slot != null)
                                {
                                    FlowChartDisconnectCommand cmd = new FlowChartDisconnectCommand(this, "�������ͼԪ");
                                    InitFirstCommand(cmd);

                                    if (cmd.Execute(slot)) // ����ִ�гɹ�
                                    {
                                        AdjustCommandList(cmd);
                                        unbind = true;
                                    }
                                }                                
                            }

                            if (lastActivatedGraphElement is SlotGraphElement) // ����Ƿ񼤻��˲��
                            {
                                slot = lastActivatedGraphElement as SlotGraphElement;

                                if (slot.IsInRegion(p)) // �������߿��Ƶ�
                                {
                                    FlowChartConnectCommand cmd = new FlowChartConnectCommand(this, "����ͼԪ");
                                    InitFirstCommand(cmd);

                                    if (cmd.Execute(new object[] { slot, connector })) // ����ִ�гɹ�
                                    {
                                        AdjustCommandList(cmd);
                                        bind = true;
                                    }
                                }
                            }
                            else if (lastActivatedGraphElement is SlotContainer) // ����Ƿ񼤻��˲������
                            {
                                slotContainer = lastActivatedGraphElement as SlotContainer;

                                if (slotContainer.IsInRegion(p)) // �������߿��Ƶ�
                                {
                                    if (connector.IsHeadPoint) // ͷ���ӵ�
                                    {
                                        slot = slotContainer.GetInSlot();
                                    }
                                    else if (connector.IsTailPoint) // β���ӵ�
                                    {
                                        slot = slotContainer.GetOutSlot();
                                    }

                                    if (slot != null)
                                    {
                                        FlowChartConnectCommand cmd = new FlowChartConnectCommand(this, "����ͼԪ");
                                        InitFirstCommand(cmd);

                                        if (cmd.Execute(new object[] { slot, connector })) // ����ִ�гɹ�
                                        {
                                            AdjustCommandList(cmd);
                                            bind = true;
                                        }
                                        else
                                        {
                                            slotContainer.RemoveSlot(slot);                                            
                                        }
                                    }
                                }
                            }

                            connector.Line.Moving = false;
                            connector.Line.AdjustRectangle();
                            regionManager.ChangeRegion((selectedGraphElement as ConnectorGraphElement).Line);
                        }
                        else if (selectedGraphElement is SlotContainer) // �϶����ǲ������
                        {
                            SlotContainer slotContainer = selectedGraphElement as SlotContainer;

                            foreach (ConnectorContainer line in slotContainer.GetConnectedLine())
                            {
                                AdjustLine(line, slotContainer);
                            }

                            slotContainer.RefreshRelevateLine(true);
                            AdjustLine(slotContainer);
                        }
                        else if (selectedGraphElement is ResizeControler) // �϶��������ſ��Ƶ�
                        {
                            regionManager.ChangeRegion((selectedGraphElement as ResizeControler).Owner);
                        }

                        regionManager.ChangeRegion(selectedGraphElement);

                        if (!bind && !unbind) // û�а󶨺ͽӴ���ͼԪ���������ƶ�ͼԪ
                        {
                            if (moveCommand.Execute(new object[] { selectedGraphElement, p })) // ����ִ�гɹ�
                            {
                                AdjustCommandList(moveCommand);
                            }
                        }
                        else // �Ѿ��󶨻��߽����ͼԪ
                        {
                            ConnectorContainer line = (selectedGraphElement as ConnectorGraphElement).Line;
                            
                            // �Զ�����������
                            ConnectorGraphElement connector = selectedGraphElement as ConnectorGraphElement;
                            SlotContainer currentSlotContainer = null;
                            if (connector.IsHeadPoint)
                            {
                                currentSlotContainer = connector.Line.OutSlotContainer;
                            }
                            else
                            {
                                currentSlotContainer = connector.Line.InSlotContainer;
                            }
                            
                            AdjustLine(line, currentSlotContainer);
                        } 

                        break;
                    }
                case UserOperation.MultiSelect:
                    {                        
                        // �Զ�������ͼ���С
                        AdjustOutOfBorder();

                        foreach (GraphElement graphElement in selectedGraphElementList)
                        {
                            if (graphElement is ConnectorContainer) // ������
                            {
                                graphElement.Visible = true;                               
                            }
                            else if (graphElement is SlotContainer) // �������
                            {
                                SlotContainer slotContainer = graphElement as SlotContainer;                                
                                slotContainer.RefreshRelevateLine(true);
                            }

                            regionManager.ChangeRegion(graphElement);
                            graphElement.Moving = false;
                        }
                       
                        // �Զ�����������
                        List<SlotContainer> affectedSlotContainerList = regionManager.GetSlotContainerList(multiSelectRegion);
                        List<ConnectorContainer> affectedLineList = regionManager.GetConnectorContainerList(multiSelectRegion);

                        foreach (ConnectorContainer line in affectedLineList)
                        {
                            foreach (SlotContainer slotContainer in affectedSlotContainerList)
                            {
                                if (line.LineIntersect(slotContainer))
                                {                                    
                                    AdjustLine(line, null);
                                    break;
                                }
                            }
                        }                        

                        if (moveCommand.Execute(new object[] { selectedGraphElement, p })) // ����ִ�гɹ�
                        {
                            AdjustCommandList(moveCommand);
                        }

                        break;
                    }
                case UserOperation.MoveRodman:
                    {
                        // �Զ�������ͼ���С
                        AdjustOutOfBorder();

                        foreach (GraphElement graphElement in moveGraphElementList)
                        {
                            if (graphElement is ConnectorContainer) // ������
                            {
                                ConnectorContainer line = graphElement as ConnectorContainer;
                                line.Visible = true;
                                line.AdjustRectangle();
                            }
                            else if (graphElement is SlotContainer) // �������
                            {
                                SlotContainer slotContainer = graphElement as SlotContainer;                                
                                slotContainer.RefreshRelevateLine(true);
                            }

                            regionManager.ChangeRegion(graphElement);
                            graphElement.Moving = false;
                        }

                        // �Զ�����������
                        List<SlotContainer> affectedSlotContainerList = regionManager.GetSlotContainerList(multiMoveRegion);
                        List<ConnectorContainer> affectedLineList = regionManager.GetConnectorContainerList(multiMoveRegion);

                        foreach (ConnectorContainer line in affectedLineList)
                        {
                            foreach (SlotContainer slotContainer in affectedSlotContainerList)
                            {
                                if (line.LineIntersect(slotContainer))
                                {
                                    AdjustLine(line, null);
                                    break;
                                }
                            }
                        } 

                        if (moveCommand.Execute(new object[] { selectedGraphElement, p })) // ����ִ�гɹ�
                        {
                            AdjustCommandList(moveCommand);
                        }

                        break;
                    }
            }

            lockMoveDrag = false; // �϶�ͼԪ����
            userOperation = UserOperation.None;         
            canvas.Cursor = Cursors.Default; // �ָ���ͼ��������״
            canvas.CurrentGuideLine.ClearGuideLineList();

            RefreshCanvas();
        }

        /// <summary>
        /// ˢ�»�ͼ��
        /// </summary>
        public void RefreshCanvas()
        {
            canvas.Invalidate(canvas.ClientRectangle);            
        }

        /// <summary>
        /// ˢ���ض������ڵĻ�ͼ��
        /// </summary>
        /// <param name="rectangle">ˢ���ض������ڵĻ�ͼ��</param>
        private void RefreshCanvas(Rectangle rectangle)
        {
            canvas.Invalidate(rectangle);
        }

        /// <summary>
        /// ��ʼ���ƶ�ͼԪ������
        /// </summary>
        private void InitMoveGraphElementList()
        {
            moveGraphElementList.Clear();

            switch (canvas.CurrentRodman.CurrentRodmanType)
            {
                case RodmanType.None:
                    {
                        break;
                    }
                case RodmanType.Horizonal:
                    {
                        foreach (SlotContainer slotContainer in slotContainerList)
                        {
                            if (slotContainer.Location.X > canvas.CurrentRodman.Location.X)
                            {
                                moveGraphElementList.Add(slotContainer);
                            }
                        }

                        foreach (ConnectorContainer line in connectorContainerList)
                        {
                            if (line.Location.X > canvas.CurrentRodman.Location.X)
                            {
                                moveGraphElementList.Add(line);
                            }
                        }

                        break;
                    }
                case RodmanType.Vertical:
                    {
                        foreach (SlotContainer slotContainer in slotContainerList)
                        {
                            if (slotContainer.Location.Y > canvas.CurrentRodman.Location.Y)
                            {
                                moveGraphElementList.Add(slotContainer);
                            }
                        }

                        foreach (ConnectorContainer line in connectorContainerList)
                        {
                            if (line.Location.Y > canvas.CurrentRodman.Location.Y)
                            {
                                moveGraphElementList.Add(line);
                            }
                        }

                        break;
                    }
            }            
        }

        /// <summary>
        /// ���ѡ�е�ͼԪ����
        /// </summary>
        private void ClearSelectedGraphElementList()
        {
            foreach (GraphElement graphElement in selectedGraphElementList)
            {
                graphElement.Selected = false;
            }

            selectedGraphElementList.Clear();
            canvas.CurrentMultiSelectMark.Visible = false;
        }

        /// <summary>
        /// ��ͼ����갴��
        /// </summary>
        /// <param name="mouseLocation">��ǰ���λ��</param>
        public void CanvasMouseDown(Point mouseLocation)
        {
            switch (userOperation)
            {
                case UserOperation.Create:
                    {
                        CreateGraphElement();
                        DeleteAbbreviateGraphElement();
                        ReconstructCanvasGraphElementList();
                        break;
                    }
                default:
                    {
                        CheckGraphElementSelected(mouseLocation);
                        break;
                    }
            }            
        }

        /// <summary>
        /// ��ͼ����굯��
        /// </summary>
        /// <param name="mouseLocation">��ǰ���λ��</param>
        public void CanvasMouseUp(Point mouseLocation)
        {
            ConfirmLocation(mouseLocation);
        }

        /// <summary>
        /// ����Ƿ�ͼԪ��ѡ��
        /// </summary>
        /// <param name="p">��굱ǰ���ڵ�</param>
        public void CheckGraphElementSelected(Point point)
        {
            Point p = AdjustMouseLocation(point);
            RecordLastSelectedPoint(p);          
  
            selectedGraphElement = null;
            ConnectorContainer newLine = null; // �Զ��½���������
            bool selected = false; // �Ƿ��Ѿ���ͼԪ��ѡ��
            GraphElement graphElement = null; // ��ǰͼԪ

            if (lastSelectedGraphElement != null) // ����һ����ѡ�е�ͼԪ��Ϊ��ѡ��״̬
            {
                if (!selectedGraphElementList.Contains(lastSelectedGraphElement)) // �ڶ�ѡ״̬��Ҫ�����һ����ѡ�е�ͼԪ�Ƿ��ڶ�ѡ������
                {
                    lastSelectedGraphElement.Selected = false;
                }
            }

            if (lastResizingGraphElement != null) // ����һ�����ŵĲ��������Ϊ������״̬
            {
                lastResizingGraphElement.Resizing = false;
            }

            // ����α��Ƿ�ѡ��
            if (canvas.CurrentRodman.IsInRegion(p))
            {
                InitMoveGraphElementList();
                graphElement = canvas.CurrentRodman;
                selected = true;
            }

            // ��������Ƿ��������߿��Ƶ�������ѡ��
            foreach (ConnectorContainer connectorContainer in regionManager.GetConnectorContainerList(p))
            {
                if (selected) // �Ѿ���ͼԪ��ѡ��ʱ�Զ�����ѭ��
                {
                    break;
                }

                // �ȱ�������Ƿ��������߿��Ƶ㱻ѡ��
                ConnectorGraphElement connector = connectorContainer.GetConnectorInRegion(p);

                if (connector != null)
                {
                    graphElement = connector;
                    selected = true;
                    break;
                }

                if (connectorContainer.IsInRegion(p)) // �����߱�ѡ��
                {
                    graphElement = connectorContainer;
                    selected = true;
                    break;
                }
            }

            foreach (SlotContainer slotContainer in regionManager.GetSlotContainerList(p)) // ��������Ƿ��в��������ѡ��
            {
                if (selected) // �����ͼԪ��ѡ�еĻ����Զ�����ѭ��
                {
                    break;
                }

                // ����Ƿ������ſ��Ƶ㱻ѡ��
                ResizeControler resizeControler = slotContainer.GetResizeControlerInRegion(p);
                
                if (resizeControler != null)
                {
                    graphElement = resizeControler;
                    selected = true;
                    slotContainer.Resizing = true;
                    lastResizingGraphElement = slotContainer;
                    ChangeMouseCursur(resizeControler.CurrentDirection); // �ı����ָ����״
                    break;
                }
                
                // ����Ƿ��а�ť��ѡ��
                BaseButton baseButton = slotContainer.GetButtonInRegion(p);

                if (baseButton != null)
                {
                    if (baseButton is ConnectButton) // �����Ӱ�ť
                    {
                        SlotGraphElement slot = (baseButton as ConnectButton).BindingSlot;

                        if (!slot.Binded && slot.IsOutSlot) // ���û�а������߿��Ƶ㣬���½������߲��󶨵���ǰ�����
                        {
                            FlowChartCreateLineCommand cmd = new FlowChartCreateLineCommand(this, "����ͼԪ");
                            InitFirstCommand(cmd);

                            if (cmd.Execute(new object[] { slot, p }))
                            {
                                AdjustCommandList(cmd);
                                newLine = selectedGraphElement as ConnectorContainer;
                                ConnectorGraphElement connector = newLine.GetConnectorList()[1];
                                graphElement = connector;
                                selected = true;
                            }

                            break;
                        }
                    }
                    else if (baseButton is AddButton) // ����Ӱ�ť
                    {
                        SlotGraphElement slot = slotContainer.AddOutSlot();
                        graphElement = slotContainer;
                        selected = true;
                        break;
                    }
                }

                // ����Ƿ���ע��ͼԪ��ѡ��
                if (slotContainer.RemarkNode != null)
                {
                    if (slotContainer.RemarkNode.IsInRegion(p))
                    {
                        graphElement = slotContainer.RemarkNode;
                        selected = true;
                        break;
                    }
                }
                
                if (slotContainer.IsInRegion(p))
                {
                    SlotGraphElement slot = slotContainer.GetSlotInRegion(p);

                    if (slot != null)
                    {
                        graphElement = slot;
                        selected = true;
                        break;
                    }

                    slotContainer.Resizing = true;
                    lastResizingGraphElement = slotContainer;
                    graphElement = slotContainer;
                    selected = true;
                    break;
                }
            }

            if (selected) // �Ѿ���ͼԪ��ѡ��
            {
                SelectGraphElement(graphElement, p);
                userOperation = UserOperation.SingleSelect;

                if (selectedGraphElementList.Contains(selectedGraphElement)) // ���ж�ͼԪ����
                {
                    if (controlMode) // ��controlģʽ����Ҫ��ѡͼԪ
                    {
                        selectedGraphElementList.Remove(selectedGraphElement);
                        selectedGraphElement.Selected = false;
                        selectedGraphElement = null;
                        lastSelectedGraphElement = null;
                        userOperation = UserOperation.None;

                        if (selectedGraphElementList.Count == 0)
                        {
                            canvas.CurrentMultiSelectMark.Visible = false;
                        }
                        else
                        {
                            CreateMultiSelectRegion();
                        }
                    }
                    else
                    {
                        userOperation = UserOperation.MultiSelect;
                    }
                }
                else
                {
                    if (controlMode) // ��controlģʽ����Ҫ����ͼԪ
                    {
                        selectedGraphElementList.Add(selectedGraphElement);
                        CreateMultiSelectRegion();

                        userOperation = UserOperation.MultiSelect;
                    }
                    else
                    {
                        if (graphElement is Rodman)
                        {
                            userOperation = UserOperation.MoveRodman;
                            CreateMultiMoveRegion();
                        }

                        // ��ն�ѡͼԪ����
                        ClearSelectedGraphElementList();
                    }
                }
            }
            else // û��ͼԪ��ѡ��
            {
                userOperation = UserOperation.RegionSelect;

                // ��ʾ��������                
                documentManager.ShowObjectProperty(background);
                multiSelectStartPoint = p;

                // ��ն�ѡͼԪ����
                ClearSelectedGraphElementList();
            }            

            RefreshCanvas();
        }

        /// <summary>
        /// ɾ��ѡ�е�ͼԪ
        /// </summary>
        public void DeleteSelectedGraphElement()
        {
            if (selectedGraphElementList.Count > 0) // ɾ������ѡ�е�ͼԪ
            {
                // ����ͼԪ����
                foreach (GraphElement graphElement in selectedGraphElementList)
                {                    
                    regionManager.DeleteFromRegion(graphElement);
                }

                FlowChartMultiDeleteCommand cmd = new FlowChartMultiDeleteCommand(this, "ɾ��ͼԪ");
                InitFirstCommand(cmd);

                if (cmd.Execute(selectedGraphElementList)) // ����ִ�гɹ�
                {
                    AdjustCommandList(cmd);
                    canvas.CurrentMultiSelectMark.Visible = false;
                    selectedGraphElement = null;
                    lastSelectedGraphElement = null;
                    lastActivatedGraphElement = null;
                    ReconstructCanvasGraphElementList();
                }                
            }

            if (selectedGraphElement != null) // ɾ������ѡ�е�ͼԪ
            {
                List<ConnectorContainer> list = new List<ConnectorContainer>();
                DataManager dataManager = flowChartManager.CurrentDataManager;
                
                if (selectedGraphElement is SlotContainer) // Ҫɾ���������
                {
                    SlotContainer slotContainer = selectedGraphElement as SlotContainer;

                    foreach (GraphElement graphElement in dataManager.GetPreviousGraphElements(slotContainer))
                    {
                        ConnectorContainer line = graphElement as ConnectorContainer;

                        if (dataManager.GetData(graphElement) == null && !list.Contains(line))
                        {
                            list.Add(line);
                        }
                    }

                    foreach (GraphElement graphElement in dataManager.GetNextGraphElements(slotContainer, false))
                    {
                        ConnectorContainer line = graphElement as ConnectorContainer;

                        if (dataManager.GetData(graphElement) == null && !list.Contains(line))
                        {
                            list.Add(line);
                        }
                    }
                }       
         
                regionManager.DeleteFromRegion(selectedGraphElement);

                FlowChartDeleteCommand cmd = new FlowChartDeleteCommand(this, "ɾ��ͼԪ");
                InitFirstCommand(cmd);

                if (cmd.Execute(selectedGraphElement)) // ����ִ�гɹ�
                {
                    AdjustCommandList(cmd);
                    
                    if (list.Count > 0) // ɾ�������ݵ�������
                    {
                        foreach (ConnectorContainer line in list)
                        {
                            SlotContainer inSlotContainer = line.InSlotContainer;
                            List<GraphElement> graphElementList = new List<GraphElement>();

                            // ִ���߼�����
                            LogicBaseManager logicManager = helper.GetLogicManager(flowChartManager.CurrentChartMode);
                            logicManager.DoLogicOperation(flowChartManager, LogicType.BeforeDelete, line);

                            if (line.InSlotContainer != null)
                            {
                                line.UnbindInSlotContainer();
                            }

                            if (line.OutSlotContainer != null)
                            {
                                graphElementList.Add(line.OutSlotContainer);
                                line.UnbindOutSlotContainer();
                            }

                            // ִ���߼�����
                            logicManager.DoLogicOperation(flowChartManager, LogicType.AfterDelete, graphElementList);

                            connectorContainerList.Remove(line); // �������߿��Ƶ�����������ɾ��
                        }
                    }
                    
                    selectedGraphElement = null;
                    lastSelectedGraphElement = null;
                    lastActivatedGraphElement = null;
                    ReconstructCanvasGraphElementList();                    
                }                                               
            }

            canvas.PaintCanvas = true; // �������ʱʹ���Ѿ��ͷŵ���Դ
            RefreshCanvas();
        }

        /// <summary>
        /// ������ͼ���
        /// </summary>
        /// <param name="moveSize">������ֵ</param>
        /// <param name="autoRefresh">�Ƿ��Զ�ˢ��</param>
        public void ScrollCanvas(Size moveSize, bool autoRefresh)
        {
            if (autoRefresh)
            {
                canvas.Visible = false; // �����ͼ����˸������ 
            }

            if (moveSize.Width != 0)
            {
                moveSize.Width = (int)((double)moveSize.Width * canvas.ClientSize.Width * graphSetting.CanvasScale / background.CanvasSize.Width); // �������ĵ�λ���ͼ�嵥λ��Ҫת��

                if (canvas.HorizontalScroll.Value + moveSize.Width >= canvas.HorizontalScroll.Minimum &&
                            canvas.HorizontalScroll.Value + moveSize.Width <= canvas.HorizontalScroll.Maximum)
                {
                    canvas.HorizontalScroll.Value += moveSize.Width;
                }
                else
                {
                    if (moveSize.Width > 0) // ���ҹ���
                    {
                        canvas.HorizontalScroll.Value = canvas.HorizontalScroll.Maximum;
                    }
                    else // �������
                    {
                        canvas.HorizontalScroll.Value = canvas.HorizontalScroll.Minimum;
                    }
                }
            }

            if (moveSize.Height != 0)
            {
                moveSize.Height = (int)((double)moveSize.Height * canvas.ClientSize.Height * graphSetting.CanvasScale / background.CanvasSize.Height); // �������ĵ�λ���ͼ�嵥λ��Ҫת��

                if (canvas.VerticalScroll.Value + moveSize.Height >= canvas.VerticalScroll.Minimum &&
                            canvas.VerticalScroll.Value + moveSize.Height <= canvas.VerticalScroll.Maximum)
                {
                    canvas.VerticalScroll.Value += moveSize.Height;
                }
                else
                {
                    if (moveSize.Height > 0) // ���¹���
                    {
                        canvas.VerticalScroll.Value = canvas.VerticalScroll.Maximum;
                    }
                    else // ���Ϲ���
                    {
                        canvas.VerticalScroll.Value = canvas.VerticalScroll.Minimum;                        
                    }
                }
            }

            if (autoRefresh)
            {
                canvas.Visible = true; // �����ͼ����˸������            
                canvas.Refresh();
                canvas.Focus();
            }
        }

        /// <summary>
        /// ������������
        /// </summary>
        public void ClearCommandList()
        {
            // ������������
            commandList.Clear();
            commandIndex = -1;            
        }

        /// <summary>
        /// ������������
        /// </summary>
        /// <param name="cmd">��ǰ����</param>
        private void AdjustCommandList(FlowChartBaseCommand cmd)
        {
            if (flowChartManager.SupportCommand)
            {
                List<FlowChartBaseCommand> deleteCommandList = new List<FlowChartBaseCommand>(); // Ҫɾ�������������

                for (int i = commandIndex + 1; i < commandList.Count; i++)
                {
                    deleteCommandList.Add(commandList[i]);
                }

                // ɾ�������ǵ�����
                foreach (FlowChartBaseCommand command in deleteCommandList)
                {
                    commandList.Remove(command);
                }

                documentManager.AdjustCommandList(commandIndex, string.Format("���� ({0} {1})", DateTime.Now, cmd.Description), 
                                                  commandList.Count);

                // ���������
                commandList.Add(cmd);
                commandIndex++;              
            }            
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="index">������</param>
        public void Undo(int index)
        {
            commandIndex = index;
            Undo();
        }

        /// <summary>
        /// ��������
        /// </summary>
        public void Undo()
        {
            FlowChartBaseCommand cmd = commandList[commandIndex];

            if (cmd.FirstCommand) // ֻ�е�һ�������ִ��ǰ��ִ�к������
            {
                cmd.Unexecute();
            }
            else // ��������ֻ����ִ�к������
            {
                cmd = commandList[commandIndex - 1];
                cmd.Execute();
            }
            
            commandIndex--;
            documentManager.SetRedoState(true);

            if (commandIndex < 0)
            {
                documentManager.SetUndoState(false);                
                flowChartManager.ContentChanged = false;
            }
            else
            {                
                documentManager.SetUndoState(true);
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        public void Redo()
        {
            FlowChartBaseCommand cmd = commandList[commandIndex + 1];
            cmd.Execute();
            commandIndex++;            
            documentManager.SetUndoState(true);

            if (commandIndex > commandList.Count - 2)
            {                
                documentManager.SetRedoState(false);
            }
            else
            {
                documentManager.SetRedoState(true);                
            }
        }
    }
}
