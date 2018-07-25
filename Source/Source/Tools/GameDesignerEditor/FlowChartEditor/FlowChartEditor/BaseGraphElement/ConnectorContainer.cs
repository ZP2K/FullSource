using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using GraphElementLibrary;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class ConnectorContainer:GraphElement
    {
        protected List<ConnectorGraphElement> connectorList = new List<ConnectorGraphElement>(); // ���ӵ�����
        protected Point head; // ������ͷ����λ��
        protected Point tail; // ������β����λ��
        protected int connectorCount; // �����߿��Ƶ�����
        protected SlotContainer inSlotContainer; // ����Ĳ������
        protected SlotContainer outSlotContainer; // �����Ĳ������
        protected ConnectorGraphElement headConnector; // �����ߵ�ͷ���
        protected ConnectorGraphElement tailConnector; // �����ߵ�β���
        protected bool requireEvent = false; // �Ƿ���Ҫ����¼�

        protected List<Rectangle> textRectangleList = new List<Rectangle>(); // �ı�λ������

        // ��ͼ������ʱ�õ�
        [NonSerialized]
        private Point connector1Location;
        [NonSerialized]
        private Point connector2Location;
        [NonSerialized]
        private Point connector3Location;
        [NonSerialized]
        private Point connector4Location;
        [NonSerialized]
        private Point connector5Location;
        [NonSerialized]
        private Point connector6Location;

        [NonSerialized]
        private int textBorderWidth = 20; // �ı��߽��� 

        [NonSerialized]
        private int textMargin = 16; // �ַ����

        [NonSerialized]
        protected bool marked = false; // �Ƿ񱻱��

        [NonSerialized]
        protected GraphSetting graphSetting; // ��ͼ�������ö���

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="tail">������β����λ��</param>
        /// <param name="head">������ͷ����λ��</param>
        /// <param name="id">�����ߵ�id</param>
        public ConnectorContainer(Point tail, Point head)
            : base(tail)
        {
            this.tail = tail;
            this.head = head;
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        public void Init()
        {
            InitConnector(tail, head);
            Refresh();
        }

        /// <summary>
        /// �����ַ���
        /// </summary>
        public override string TypeString
        {
            get
            {
                return "Line";
            }
        }

        /// <summary>
        /// �Ƿ񱻱��
        /// </summary>
        [Browsable(false)]
        public virtual bool Marked
        {
            set
            {
                marked = value;                
            }

            get
            {
                return marked;
            }
        }

        /// <summary>
        /// �Ƿ���Ҫ����¼�
        /// </summary>
        [Browsable(false)]
        public virtual bool RequireEvent
        {
            set
            {
                requireEvent = value;
            }

            get
            {
                return requireEvent;
            }
        }    

        /// <summary>
        /// ͼԪ��ʾ���ı�����
        /// </summary>
        [Browsable(false)]
        public override string Text
        {
            set
            {
                text = value;
                displayText = value;
            }

            get
            {
                return text;
            }
        }

        /// <summary>
        /// �����߿��Ƶ���������
        /// </summary>
        [Browsable(false)]
        public virtual int ConnectorCount
        {
            set
            {                
                AdjustConnector(value);
                connectorCount = value;
            }

            get
            {
                return connectorCount;
            }
        }

        /// <summary>
        /// ����Ĳ����������
        /// </summary>
        [Browsable(false)]
        public virtual SlotContainer InSlotContainer
        {
            set
            {
                inSlotContainer = value;
            }

            get
            {
                return inSlotContainer;
            }
        }

        /// <summary>
        /// �����Ĳ������
        /// </summary>
        [Browsable(false)]
        public virtual SlotContainer OutSlotContainer
        {
            set
            {
                outSlotContainer = value;
            }

            get
            {
                return outSlotContainer;
            }
        }

        /// <summary>
        /// ������ͷ�������
        /// </summary>
        [Browsable(false)]
        public virtual ConnectorGraphElement HeadConnector
        {
            set
            {
                headConnector = value;
            }

            get
            {
                return headConnector;
            }
        }

        /// <summary>
        /// ������β�������
        /// </summary>
        [Browsable(false)]
        public virtual ConnectorGraphElement TailConnector
        {
            set
            {
                tailConnector = value;
            }

            get
            {
                return tailConnector;
            }
        }

        /// <summary>
        /// ˢ��ͼԪ���Ʋ���
        /// </summary>
        public override void Refresh()
        {
            // ��ʼ����ͼ���ݹ������
            graphSetting = GraphSetting.GetGraphSetting();

            // ˢ�±����ı�
            displayText = text;

            // ˢ���߼�����
            textBorderWidth = 20;
            textMargin = 16;

            AdjustRectangle();            
            AdjustText();                      

            // ˢ�������߿��Ƶ�
            foreach (ConnectorGraphElement connector in connectorList)
            {
                connector.Refresh();
            }
        }

        /// <summary>
        /// �ͷ�ͼԪ�Ļ�ͼ��Դ
        /// </summary>
        public override void Release()
        {

        }        

        /// <summary>
        /// ��ȡ�����������
        /// </summary>
        /// <param name="canvasSize">��ͼ���С</param>
        /// <param name="gridWidth">������</param>
        /// <returns>�����������</returns>
        public Rectangle GetAdjustRectangle(Size canvasSize, int gridWidth)
        {
            Rectangle adjustRectangle = Rectangle.Empty;

            if (inSlotContainer != null && outSlotContainer != null)
            {
                int leftX;
                int rightX;
                int topY;
                int bottomY;

                if (inSlotContainer.Location.X < outSlotContainer.Location.X)
                {
                    leftX = inSlotContainer.Location.X;
                }
                else
                {
                    leftX = outSlotContainer.Location.X;
                }

                if (inSlotContainer.TextRectangle.Right > outSlotContainer.TextRectangle.Right)
                {
                    rightX = inSlotContainer.TextRectangle.Right;
                }
                else
                {
                    rightX = outSlotContainer.TextRectangle.Right;
                }

                if (inSlotContainer.Location.Y < outSlotContainer.Location.Y)
                {
                    topY = inSlotContainer.Location.Y;
                }
                else
                {
                    topY = outSlotContainer.Location.Y;
                }

                if (inSlotContainer.TextRectangle.Bottom >
                    outSlotContainer.TextRectangle.Bottom)
                {
                    bottomY = inSlotContainer.TextRectangle.Bottom;
                }
                else
                {
                    bottomY = outSlotContainer.TextRectangle.Bottom;
                }

                int leftLineX = (int)Math.Floor(((float)leftX - 10 * gridWidth) / gridWidth) * gridWidth;
                if (leftLineX < 0)
                {
                    leftLineX = gridWidth;
                }

                int rightLineX = (int)Math.Ceiling(((float)rightX + 10 * gridWidth) / gridWidth) * gridWidth;
                if (rightLineX > canvasSize.Width)
                {
                    rightLineX = canvasSize.Width;
                }

                int topLineY = (int)Math.Floor(((float)topY - 10 * gridWidth) / gridWidth) * gridWidth;
                if (topLineY < 0)
                {
                    topLineY = gridWidth;
                }

                int bottomLineY = (int)Math.Ceiling(((float)bottomY + 10 * gridWidth) / gridWidth) * gridWidth;
                if (bottomLineY > canvasSize.Height)
                {
                    bottomLineY = canvasSize.Height;
                }

                adjustRectangle.Location = new Point(leftLineX, topLineY);
                adjustRectangle.Size = new Size(rightLineX - leftLineX, bottomLineY - topLineY);
            }

            return adjustRectangle;                
        }

        /// <summary>
        /// �����Ƿ���������
        /// </summary>
        /// <param name="connector2Location">���ӵ�2������</param>
        /// <param name="connector3Location">���ӵ�3������</param>
        /// <param name="gridBoard">��������</param>
        /// <param name="gridWidth">�����С</param>
        /// <returns>�Ƿ����������</returns>
        private bool CheckPointInGrid(Point connector2Location, Point connector3Location, bool[,] gridBoard, int gridWidth)
        {
            bool inGrid = false;

            int boardWidth = gridBoard.GetLength(0);
            int boardHeight = gridBoard.GetLength(1);
            int tempX = (int)(connector2Location.X / gridWidth);
            int tempY = (int)(connector2Location.Y / gridWidth);

            if ((tempX >= 0 && tempX < boardWidth) && (tempY >= 0 && tempY < boardHeight)) // ��Ҫ���߽����
            {
                inGrid = gridBoard[tempX, tempY];
            }

            tempX = (int)(connector3Location.X / gridWidth);
            tempY = (int)(connector3Location.Y / gridWidth);

            if ((tempX >= 0 && tempX < boardWidth) && (tempY >= 0 && tempY < boardHeight)) // ��Ҫ���߽����
            {
                inGrid = gridBoard[tempX, tempY];

            }

            return inGrid;
        }

        /// <summary>
        /// �����Ƿ���������
        /// </summary>
        /// <param name="connector2Location">���ӵ�2������</param>
        /// <param name="connector3Location">���ӵ�3������</param>
        /// <param name="connector4Location">���ӵ�4������</param>
        /// <param name="connector5Location">���ӵ�5������</param>
        /// <param name="gridBoard">��������</param>
        /// <param name="gridWidth">�����С</param>
        /// <returns>�Ƿ����������</returns>
        private bool CheckPointInGrid(Point connector2Location, Point connector3Location, Point connector4Location, Point connector5Location, bool[,] gridBoard, int gridWidth)
        {
            bool inGrid = false;

            int boardWidth = gridBoard.GetLength(0);
            int boardHeight = gridBoard.GetLength(1);
            int tempX = (int)(connector2Location.X / gridWidth);
            int tempY = (int)(connector2Location.Y / gridWidth);

            if ((tempX >= 0 && tempX < boardWidth) && (tempY >= 0 && tempY < boardHeight)) // ��Ҫ���߽����
            {
                inGrid = gridBoard[tempX, tempY];
            }

            tempX = (int)(connector3Location.X / gridWidth);
            tempY = (int)(connector3Location.Y / gridWidth);

            if ((tempX >= 0 && tempX < boardWidth) && (tempY >= 0 && tempY < boardHeight)) // ��Ҫ���߽����
            {
                inGrid = gridBoard[tempX, tempY];
                
            }

            tempX = (int)(connector4Location.X / gridWidth);
            tempY = (int)(connector4Location.Y / gridWidth);

            if ((tempX >= 0 && tempX < boardWidth) && (tempY >= 0 && tempY < boardHeight)) // ��Ҫ���߽����
            {
                inGrid = gridBoard[tempX, tempY];                
            }

            tempX = (int)(connector5Location.X / gridWidth);
            tempY = (int)(connector5Location.Y / gridWidth);

            if ((tempX >= 0 && tempX < boardWidth) && (tempY >= 0 && tempY < boardHeight)) // ��Ҫ���߽����
            {
                inGrid = gridBoard[tempX, tempY];                
            }

            return inGrid;
        }

        /// <summary>
        /// ����Ƿ���Ҫ����������
        /// </summary>
        /// <param name="slotContainerList">�����������</param>
        /// <param name="canvasSize">��ͼ���С</param>
        /// <returns>�Ƿ���Ҫ����������</returns>
        private bool CheckRequireAdjust(List<SlotContainer> slotContainerList, Size canvasSize)
        {
            bool requireAdjust = false;
            int headX = headConnector.Location.X;
            int headY = headConnector.Location.Y;
            int tailX = tailConnector.Location.X;
            int tailY = tailConnector.Location.Y;

            if (headY > tailY) // ͷ�����β���֮��
            {
                Rectangle tempRectangle = InvalidRectangle;

                // �ȼ���Ƿ������������ཻ
                foreach (SlotContainer slotContainer in slotContainerList)
                {
                    Rectangle rectangle = slotContainer.TextRectangle;

                    if (LineIntersect(slotContainer, headConnector.Location, tailConnector.Location))
                    {
                        requireAdjust = true;
                        break;
                    }
                }

                // �Զ�������������
                if (!requireAdjust)
                {
                    ConnectorCount = 2;
                }
            }
            else // ͷ�����β���֮��
            {
                requireAdjust = true;
            }

            if (headX < 0 || headX > canvasSize.Width || headY < 0 || headY > canvasSize.Height ||
                tailX < 0 || tailX > canvasSize.Width || tailY < 0 || tailY > canvasSize.Height) // �������ڻ�ͼ��֮��������߲��õ�������ֹ������Ƿ�����
            {
                requireAdjust = false;
            }

            return requireAdjust;
        }

        /// <summary>
        /// ��ȡ�����ڵĲ������
        /// </summary>
        /// <param name="regionRectangle">�������</param>
        /// <param name="slotContainerList">�����������</param>
        /// <returns>�����ڵĲ������</returns>
        private List<SlotContainer> GetSlotContainerInRegion(Rectangle regionRectangle, List<SlotContainer> slotContainerList)
        {
            List<SlotContainer> nodeList = new List<SlotContainer>();

            foreach (SlotContainer slotContainer in slotContainerList)
            {
                Rectangle rectangle = slotContainer.InvalidRectangle;

                if (regionRectangle.Contains(rectangle) || regionRectangle.IntersectsWith(rectangle))
                {
                    nodeList.Add(slotContainer);
                }
            }

            // ����������Ĳ������Ҫ�������
            if (!nodeList.Contains(inSlotContainer))
            {
                nodeList.Add(inSlotContainer);
            }

            if (!nodeList.Contains(outSlotContainer))
            {
                nodeList.Add(outSlotContainer);
            }

            return nodeList;
        }

        /// <summary>
        /// ��ȡ�����ڵ�������
        /// </summary>
        /// <param name="regionRectangle">�������</param>
        /// <param name="connectorContainerList">����������</param>
        /// <returns>�����ڵ�������</returns>
        private List<ConnectorContainer> GetLineInRegion(Rectangle regionRectangle, List<ConnectorContainer> connectorContainerList)
        {
            List<ConnectorContainer> lineList = new List<ConnectorContainer>();

            foreach (ConnectorContainer line in connectorContainerList)
            {
                Rectangle rectangle = line.InvalidRectangle;

                if (regionRectangle.Contains(rectangle) || regionRectangle.IntersectsWith(rectangle))
                {
                    lineList.Add(line);
                }
            }

            return lineList;
        }

        /// <summary>
        /// ����������Ƿ����������ཻ
        /// </summary>
        /// <param name="slotContainer">�������</param>
        /// <param name="pointList">�����ߵ����ӵ�����</param>
        /// <returns>�Ƿ��ཻ</returns>
        private bool CheckSlotContainerIntersect4(SlotContainer slotContainer, List<Point> pointList)
        {
            bool intersect = false;
            Rectangle rectangle = slotContainer.TextRectangle;

            if (LineIntersect(rectangle, pointList[1], pointList[2]))
            {
                intersect = true;
            }
            else
            {
                if (slotContainer == inSlotContainer)
                {
                    if (pointList[0].Y > pointList[1].Y)
                    {
                        intersect = true;
                    }
                }
                else if (slotContainer == outSlotContainer)
                {
                    if (pointList[2].Y > pointList[3].Y)
                    {
                        intersect = true;
                    }
                }
                else
                {
                    if (LineIntersect(rectangle, pointList[0], pointList[1])
                        || LineIntersect(rectangle, pointList[2], pointList[3]))
                    {
                        intersect = true;
                    }
                }
            }

            return intersect;
        }

        /// <summary>
        /// ����������Ƿ����������ཻ
        /// </summary>
        /// <param name="slotContainer">�������</param>
        /// <param name="pointList">�����ߵ����ӵ�����</param>
        /// <returns>�Ƿ��ཻ</returns>
        private bool CheckSlotContainerIntersect6(SlotContainer slotContainer, List<Point> pointList)
        {
            bool intersect = false;
            Rectangle rectangle = slotContainer.TextRectangle;

            if (LineIntersect(rectangle, pointList[1], pointList[2])
                || LineIntersect(rectangle, pointList[2], pointList[3])
                || LineIntersect(rectangle, pointList[3], pointList[4]))
            {
                intersect = true;                
            }
            else
            {
                if (slotContainer == inSlotContainer)
                {
                    if (pointList[0].Y > pointList[1].Y)
                    {
                        intersect = true;                        
                    }
                }
                else if (slotContainer == outSlotContainer)
                {
                    if (pointList[4].Y > pointList[5].Y)
                    {
                        intersect = true;                                                
                    }
                }
                else                
                {
                    if (LineIntersect(rectangle, pointList[0], pointList[1])
                        || LineIntersect(rectangle, pointList[4], pointList[5]))
                    {
                        intersect = true;
                    }
                }
            }

            return intersect;
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="currentSlotContainer">��ǰ�ƶ��Ĳ������</param>
        /// <param name="slotContainerList">�����������</param>
        /// <param name="connectorContainerList">����������</param>
        /// <param name="gridWidth">����ı߳�</param>
        /// <param name="canvasSize">��ͼ��Ĵ�С</param>
        /// <param name="gridBoard">�������ݻ�ͼ��</param>
        public virtual void AdjustLine(SlotContainer currentSlotContainer, List<SlotContainer> slotContainerList, List<ConnectorContainer> connectorContainerList, int gridWidth, Size canvasSize, bool[,] gridBoard)
        {
            if (inSlotContainer != null && outSlotContainer != null) // ���������˾�������ͼԪ
            {
                int headX = headConnector.Location.X;
                int headY = headConnector.Location.Y;
                int tailX = tailConnector.Location.X;
                int tailY = tailConnector.Location.Y;

                ConnectorGraphElement connector1 = null;
                ConnectorGraphElement connector2 = null;
                ConnectorGraphElement connector3 = null;
                ConnectorGraphElement connector4 = null;
                ConnectorGraphElement connector5 = null;
                ConnectorGraphElement connector6 = null;                

                if (CheckRequireAdjust(slotContainerList, canvasSize)) // ��ҪѰ·����
                {
                    // �ȳ�����Ѱ·�㷨
                    bool findLine = false;
                    Rectangle regionRectangle = Rectangle.Empty;
                    List<SlotContainer> nodeList;
                    List<ConnectorContainer> lineList;

                    // ������·�������������غ�
                    bool findBackup = false;
                    Point[] backupPoints = new Point[4];
                    backupPoints[0] = Point.Empty;
                    backupPoints[1] = Point.Empty;
                    backupPoints[2] = Point.Empty;
                    backupPoints[3] = Point.Empty;                     
                   
                    List<Point> pointList = new List<Point>();
                    List<Point[]> validPoints = new List<Point[]>(); // ��Ч��ļ���
                    bool preCheck = false; // �Ƿ���Ԥ���
                    bool valid = true;

                    // ��ס��һ�α������ͼԪ
                    SlotContainer lastBlockSlotContainer = null;
                    ConnectorContainer lastBlockLine = null;    
                    
                    // ��ʼ������
                    int leftX;
                    int rightX;
                    int topY;
                    int bottomY;

                    if (inSlotContainer.Location.X < outSlotContainer.Location.X)
                    {
                        leftX = inSlotContainer.Location.X;
                    }
                    else
                    {
                        leftX = outSlotContainer.Location.X;
                    }

                    if (inSlotContainer.TextRectangle.Right > outSlotContainer.TextRectangle.Right)
                    {
                        rightX = inSlotContainer.TextRectangle.Right;
                    }
                    else
                    {
                        rightX = outSlotContainer.TextRectangle.Right;
                    }

                    if (inSlotContainer.Location.Y < outSlotContainer.Location.Y)
                    {
                        topY = inSlotContainer.Location.Y;
                    }
                    else
                    {
                        topY = outSlotContainer.Location.Y;
                    }

                    if (inSlotContainer.TextRectangle.Bottom >
                        outSlotContainer.TextRectangle.Bottom)
                    {
                        bottomY = inSlotContainer.TextRectangle.Bottom;
                    }
                    else
                    {
                        bottomY = outSlotContainer.TextRectangle.Bottom;
                    }

                    // �ȳ������ӿ��Ƶ�������Ϊ4�����
                    if (head.Y > tail.Y)
                    {
                        bool requireRestore = false; // �Ƿ���Ҫ���浱ǰ���ӿ��Ƶ�����
                        Point[] restorePointArray = new Point[6]; // ���浱ǰ�����߿��Ƶ����������

                        if (connectorCount == 6)
                        {
                            requireRestore = true;
                            restorePointArray[0] = connectorList[0].Location;
                            restorePointArray[1] = connectorList[1].Location;
                            restorePointArray[2] = connectorList[2].Location;
                            restorePointArray[3] = connectorList[3].Location;
                            restorePointArray[4] = connectorList[4].Location;
                            restorePointArray[5] = connectorList[5].Location;
                        }

                        ConnectorCount = 4;
                        connector1 = connectorList[0];
                        connector2 = connectorList[1];
                        connector3 = connectorList[2];
                        connector4 = connectorList[3];                        

                        // ɸѡ��Ҫ���õĽ���������                        
                        regionRectangle.Location = new Point(leftX, topY);
                        regionRectangle.Size = new Size(rightX - leftX, bottomY - topY);

                        nodeList = GetSlotContainerInRegion(regionRectangle, slotContainerList);
                        lineList = GetLineInRegion(regionRectangle, connectorContainerList);

                        // Ԥ���һ��                                               
                        if (currentSlotContainer == inSlotContainer)
                        {
                            connector1Location.X = connector1.Location.X;
                            connector1Location.Y = connector1.Location.Y;
                            connector2Location.X = connector1.Location.X;
                            connector2Location.Y = connector2.Location.Y;
                            connector3Location.X = connector4.Location.X;
                            connector3Location.Y = connector2.Location.Y;
                            connector4Location.X = connector4.Location.X;
                            connector4Location.Y = connector4.Location.Y;

                            preCheck = true;
                        }
                        else if (currentSlotContainer == outSlotContainer)
                        {
                            connector1Location.X = connector1.Location.X;
                            connector1Location.Y = connector1.Location.Y;
                            connector2Location.X = connector1.Location.X;
                            connector2Location.Y = connector3.Location.Y;
                            connector3Location.X = connector4.Location.X;
                            connector3Location.Y = connector3.Location.Y;
                            connector4Location.X = connector4.Location.X;
                            connector4Location.Y = connector4.Location.Y;

                            preCheck = true;
                        }

                        // У�������ߵ���״
                        if (connector2Location.X != connector1Location.X)
                        {
                            connector2Location.X = connector1Location.X;
                        }

                        if (connector3Location.Y != connector2Location.Y)
                        {
                            connector3Location.Y = connector2Location.Y;
                        }

                        if (connector3Location.X != connector4Location.X)
                        {
                            connector3Location.Y = connector5Location.X;
                        }

                        if (preCheck)
                        {
                            if (!CheckPointInGrid(connector2Location, connector3Location, gridBoard, gridWidth))
                            {
                                List<Point> checkPointList = new List<Point>();
                                checkPointList.Add(connector1Location);
                                checkPointList.Add(connector2Location);
                                checkPointList.Add(connector3Location);
                                checkPointList.Add(connector4Location);

                                foreach (SlotContainer slotContainer in nodeList)
                                {                                    
                                    if (CheckSlotContainerIntersect4(slotContainer, checkPointList))
                                    {
                                        valid = false;
                                        lastBlockSlotContainer = slotContainer;
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            valid = false;
                        }

                        if (valid) // Ԥ���ѡ�����·�ɹ���ֱ�ӵ���������
                        {
                            validPoints.Add(new Point[] { connector2Location, connector3Location });
                            findLine = true;
                        }
                        else
                        {
                            int tempY1 = (int)Math.Ceiling((float)inSlotContainer.TextRectangle.Bottom / gridWidth) * gridWidth;
                            int tempY2 = (int)Math.Floor((float)outSlotContainer.TextRectangle.Top / gridWidth) * gridWidth;

                            for (int i = tempY1; i < tempY2; i += gridWidth)
                            {
                                valid = true;

                                connector2Location.X = connector1Location.X;
                                connector2Location.Y = i;
                                connector3Location.X = connector4Location.X;
                                connector3Location.Y = i;

                                List<Point> checkPointList = new List<Point>();
                                checkPointList.Add(connector1Location);
                                checkPointList.Add(connector2Location);
                                checkPointList.Add(connector3Location);
                                checkPointList.Add(connector4Location);

                                if (CheckPointInGrid(connector2Location, connector3Location, gridBoard, gridWidth))
                                {
                                    valid = false;
                                    continue;
                                }

                                if (lastBlockSlotContainer != null)
                                {                                    
                                    if (CheckSlotContainerIntersect4(lastBlockSlotContainer, checkPointList))
                                    {
                                        valid = false;
                                        continue;
                                    }                                    
                                }

                                foreach (SlotContainer slotContainer in nodeList)
                                {
                                    if (CheckSlotContainerIntersect4(slotContainer, checkPointList))
                                    {
                                        valid = false;
                                        lastBlockSlotContainer = slotContainer;
                                        break;
                                    }
                                }

                                if (valid)
                                {
                                    // ��¼������·
                                    if (!findBackup)
                                    {
                                        findBackup = true;
                                        backupPoints[0].X = connector1Location.X;
                                        backupPoints[0].Y = i;
                                        backupPoints[1].X = connector4Location.Y;
                                        backupPoints[1].Y = i;
                                    }

                                    pointList.Clear();
                                    pointList.Add(connector1Location);
                                    pointList.Add(connector2Location);
                                    pointList.Add(connector3Location);
                                    pointList.Add(connector4Location);

                                    if (lastBlockLine != null)
                                    {
                                        List<ConnectorGraphElement> blockList = lastBlockLine.GetConnectorList();

                                        switch (blockList.Count)
                                        {
                                            case 4:
                                                {
                                                    if (PointInLine(blockList[1].Location, pointList) || PointInLine(blockList[2].Location, pointList))
                                                    {
                                                        valid = false;
                                                        continue;
                                                    }

                                                    break;
                                                }
                                            case 6:
                                                {
                                                    if (PointInLine(blockList[1].Location, pointList) || PointInLine(blockList[2].Location, pointList)
                                                        || (PointInLine(blockList[3].Location, pointList) || (PointInLine(blockList[4].Location, pointList))))
                                                    {
                                                        valid = false;
                                                        continue;
                                                    }

                                                    break;
                                                }
                                        }

                                    }
                                }

                                if (valid)
                                {
                                    validPoints.Add(new Point[] { connector2Location, connector3Location });
                                    findLine = true;
                                }
                            }                            
                        }
                        
                        if (!findLine)
                        {
                            if (requireRestore) // �ָ������߿��Ƶ�����
                            {
                                ConnectorCount = 6;
                                connectorList[0].Location = restorePointArray[0];
                                connectorList[1].Location = restorePointArray[1];
                                connectorList[2].Location = restorePointArray[2];
                                connectorList[3].Location = restorePointArray[3];
                                connectorList[4].Location = restorePointArray[4];
                                connectorList[5].Location = restorePointArray[5];
                            }
                        }
                    }

                    if (!findLine) // �ٳ��������߿��Ƶ�����Ϊ6�����
                    {
                        // ��ʼ������
                        valid = true;
                        preCheck = false;
                        findBackup = false;

                        pointList.Clear();
                        validPoints.Clear();

                        int leftLineX = (int)Math.Floor(((float)leftX - 10 * gridWidth) / gridWidth) * gridWidth;
                        if (leftLineX < 0)
                        {
                            leftLineX = gridWidth;
                        }

                        int rightLineX = (int)Math.Ceiling(((float)rightX + 10 * gridWidth) / gridWidth) * gridWidth;
                        if (rightLineX > canvasSize.Width)
                        {
                            rightLineX = canvasSize.Width;
                        }

                        int topLineY = (int)Math.Floor(((float)topY - 10 * gridWidth) / gridWidth) * gridWidth;
                        if (topLineY < 0)
                        {
                            topLineY = gridWidth;
                        }

                        int bottomLineY = (int)Math.Ceiling(((float)bottomY + 10 * gridWidth) / gridWidth) * gridWidth;
                        if (bottomLineY > canvasSize.Height)
                        {
                            bottomLineY = canvasSize.Height;
                        }

                        // ɸѡ��Ҫ���õĽ���������
                        regionRectangle.Location = new Point(leftLineX, topLineY);
                        regionRectangle.Size = new Size(rightLineX - leftLineX, bottomLineY - topLineY);

                        nodeList = GetSlotContainerInRegion(regionRectangle, slotContainerList);
                        lineList = GetLineInRegion(regionRectangle, connectorContainerList);            

                        List<int> verticalLineList = new List<int>();
                        List<int> horizontalLineList = new List<int>();                        

                        for (int i = leftLineX; i < rightLineX; i += gridWidth)
                        {
                            verticalLineList.Add(i);
                        }

                        for (int i = topLineY; i < bottomLineY; i += gridWidth)
                        {
                            horizontalLineList.Add(i);
                        }

                        ConnectorCount = 6;
                        connector1 = connectorList[0];
                        connector2 = connectorList[1];
                        connector3 = connectorList[2];
                        connector4 = connectorList[3];
                        connector5 = connectorList[4];
                        connector6 = connectorList[5];

                        // ��ʼ�������ҷ���������·��
                        connector1Location = connector1.Location;                   
                        connector6Location = connector6.Location;

                        // Ԥ���һ��
                        if (currentSlotContainer == inSlotContainer)
                        {
                            connector2Location.X = connector1.Location.X;
                            connector2Location.Y = connector2.Location.Y;
                            connector3Location.X = connector3.Location.X;
                            connector3Location.Y = connector3.Location.Y;
                            connector4Location.X = connector4.Location.X;
                            connector4Location.Y = connector4.Location.Y;
                            connector5Location.X = connector5.Location.X;
                            connector5Location.Y = connector5.Location.Y;
                            preCheck = true;
                        }
                        else if (currentSlotContainer == outSlotContainer)
                        {
                            connector2Location.X = connector1.Location.X;
                            connector2Location.Y = connector2.Location.Y;
                            connector3Location.X = connector3.Location.X;
                            connector3Location.Y = connector3.Location.Y;
                            connector4Location.X = connector4.Location.X;
                            connector4Location.Y = connector4.Location.Y;
                            connector5Location.X = connector6.Location.X;
                            connector5Location.Y = connector5.Location.Y;
                            preCheck = true;
                        }

                        // У�������ߵ���״
                        if (connector2Location.X != connector1Location.X)
                        {
                            connector2Location.X = connector1Location.X;
                        }

                        if (connector5Location.X != connector6Location.X)
                        {
                            connector5Location.X = connector6Location.X;
                        }

                        if (connector3Location.Y != connector2Location.Y)
                        {
                            connector3Location.Y = connector2Location.Y;
                        }

                        if (connector4Location.Y != connector5Location.Y)
                        {
                            connector4Location.Y = connector5Location.Y;
                        }

                        if (preCheck)
                        {
                            if (!CheckPointInGrid(connector2Location, connector3Location, connector4Location, connector5Location, gridBoard, gridWidth))
                            {
                                List<Point> checkPointList = new List<Point>();
                                checkPointList.Add(connector1Location);
                                checkPointList.Add(connector2Location);
                                checkPointList.Add(connector3Location);
                                checkPointList.Add(connector4Location);
                                checkPointList.Add(connector5Location);
                                checkPointList.Add(connector6Location);

                                foreach (SlotContainer slotContainer in nodeList)
                                {
                                    if (CheckSlotContainerIntersect6(slotContainer, checkPointList))
                                    {
                                        valid = false;
                                        lastBlockSlotContainer = slotContainer;
                                        break;
                                    }                                    
                                }
                            }
                        }
                        else
                        {
                            valid = false;
                        }

                        if (valid) // Ԥ���ѡ�����·�ɹ���ֱ�ӵ���������
                        {
                            validPoints.Add(new Point[] { connector2Location, connector3Location, connector4Location, connector5Location });
                            findLine = true;
                        }
                        else
                        {
                            for (int i = 0; i < verticalLineList.Count; i++)
                            {
                                for (int j = 0; j < horizontalLineList.Count; j++)
                                {
                                    for (int k = 0; k < horizontalLineList.Count; k++)
                                    {
                                        valid = true;
                                        connector2Location.X = connector1Location.X;
                                        connector2Location.Y = horizontalLineList[k];
                                        connector3Location.X = verticalLineList[i];
                                        connector3Location.Y = horizontalLineList[k];
                                        connector4Location.X = verticalLineList[i];
                                        connector4Location.Y = horizontalLineList[j];
                                        connector5Location.X = connector6Location.X;
                                        connector5Location.Y = horizontalLineList[j];

                                        List<Point> checkPointList = new List<Point>();
                                        checkPointList.Add(connector1Location);
                                        checkPointList.Add(connector2Location);
                                        checkPointList.Add(connector3Location); 
                                        checkPointList.Add(connector4Location);
                                        checkPointList.Add(connector5Location);
                                        checkPointList.Add(connector6Location);

                                        if (CheckPointInGrid(connector2Location, connector3Location, connector4Location, connector5Location, gridBoard, gridWidth))
                                        {
                                            valid = false;
                                        }

                                        if (lastBlockSlotContainer != null)
                                        {
                                            if (CheckSlotContainerIntersect6(lastBlockSlotContainer, checkPointList))
                                            {
                                                valid = false;
                                                continue;
                                            }                                            
                                        }

                                        foreach (SlotContainer slotContainer in nodeList)
                                        {
                                            if (CheckSlotContainerIntersect6(slotContainer, checkPointList))
                                            {
                                                valid = false;
                                                lastBlockSlotContainer = slotContainer;
                                                break;
                                            }
                                        }

                                        if (valid)
                                        {
                                            // ��¼������·
                                            if (!findBackup)
                                            {
                                                findBackup = true;
                                                backupPoints[0].X = connector1.Location.X;
                                                backupPoints[0].Y = horizontalLineList[k];
                                                backupPoints[1].X = verticalLineList[i];
                                                backupPoints[1].Y = horizontalLineList[k];
                                                backupPoints[2].X = verticalLineList[i];
                                                backupPoints[2].Y = horizontalLineList[j];
                                                backupPoints[3].X = connector6.Location.X;
                                                backupPoints[3].Y = horizontalLineList[j];
                                            }

                                            pointList.Clear();
                                            pointList.Add(connector1Location);
                                            pointList.Add(connector2Location);
                                            pointList.Add(connector3Location);
                                            pointList.Add(connector4Location);
                                            pointList.Add(connector5Location);
                                            pointList.Add(connector6Location);

                                            if (lastBlockLine != null)
                                            {
                                                List<ConnectorGraphElement> blockList = lastBlockLine.GetConnectorList();

                                                if (PointInLine(blockList[1].Location, pointList) || PointInLine(blockList[2].Location, pointList)
                                                    || (PointInLine(blockList[3].Location, pointList) || (PointInLine(blockList[4].Location, pointList))))
                                                {
                                                    valid = false;
                                                    continue;
                                                }
                                            }

                                            foreach (ConnectorContainer line in lineList)
                                            {
                                                if (line != this && line.ConnectorCount == 6)
                                                {
                                                    List<ConnectorGraphElement> list = line.GetConnectorList();

                                                    if (PointInLine(list[1].Location, pointList) || PointInLine(list[2].Location, pointList)
                                                        || (PointInLine(list[3].Location, pointList) || (PointInLine(list[4].Location, pointList))))
                                                    {
                                                        valid = false;
                                                        lastBlockLine = line;
                                                        break;
                                                    }
                                                }
                                            }
                                        }

                                        if (valid)
                                        {
                                            validPoints.Add(new Point[] { connector2Location, connector3Location, connector4Location, connector5Location });
                                            findLine = true;
                                        }
                                    }
                                }
                            }
                        }
                    }                                                                                                                                                                                                                                                                   

                    if (findLine) // �ҵ�����������·��
                    {
                        // ��ѡ�����·
                        int index = 0;
                        int score = -1;

                        Point[] points;
                        int tempLength1;
                        int tempLength2;
                        int tempLength3;
                        int tempLength4;
                        int tempLength5;

                        switch (connectorCount)
                        {
                            case 4:
                                {
                                    for (int i = 0; i < validPoints.Count; i++)
                                    {
                                        points = validPoints[i];
                                        tempLength1 = points[0].Y - connector1Location.Y;
                                        tempLength2 = Math.Abs(points[0].X - points[1].X);
                                        tempLength3 = connector4Location.Y - points[1].Y;

                                        if (tempLength1 < gridWidth ||
                                            tempLength2 < gridWidth ||
                                            tempLength3 < gridWidth
                                            )
                                        {
                                            continue;
                                        }
                                        else
                                        {
                                            int currentScore = (Math.Abs(connector1Location.X - points[1].X) + Math.Abs(connector4Location.X - points[1].X)) * 10
                                                                                            + tempLength1 + tempLength3;
                                            if (currentScore < score || score == -1)
                                            {
                                                score = currentScore;
                                                index = i;
                                            }
                                        }
                                    }

                                    points = validPoints[index];
                                    connector2.Location = points[0];
                                    connector3.Location = points[1];

                                    break;
                                }
                            case 6:
                                {
                                    for (int i = 0; i < validPoints.Count; i++)
                                    {
                                        points = validPoints[i];
                                        tempLength1 = points[0].Y - connector1Location.Y;
                                        tempLength2 = Math.Abs(points[0].X - points[1].X);
                                        tempLength3 = Math.Abs(points[1].Y - points[2].Y);
                                        tempLength4 = Math.Abs(points[2].X - points[3].X);
                                        tempLength5 = connector6Location.Y - points[3].Y;

                                        if (tempLength1 < gridWidth ||
                                            tempLength2 < gridWidth ||
                                            tempLength3 < gridWidth ||
                                            tempLength4 < gridWidth ||
                                            tempLength5 < gridWidth
                                            )
                                        {
                                            continue;
                                        }
                                        else
                                        {
                                            int currentScore = (Math.Abs(connector1Location.X - points[1].X) + Math.Abs(connector6Location.X - points[1].X)) * 10
                                                                                            + tempLength1 + tempLength5;
                                            if (currentScore < score || score == -1)
                                            {
                                                score = currentScore;
                                                index = i;
                                            }
                                        }
                                    }

                                    points = validPoints[index];
                                    connector2.Location = points[0];
                                    connector3.Location = points[1];
                                    connector4.Location = points[2];
                                    connector5.Location = points[3];

                                    break;
                                }
                        }
                    }
                    else // Ѱ·�㷨ʧЧ֮��Ĵ���
                    {
                        if (findBackup) // ���ñ�����·
                        {
                            switch (connectorCount)
                            {
                                case 4:
                                    {
                                        connector2.Location = backupPoints[0];
                                        connector3.Location = backupPoints[1];

                                        break;
                                    }
                                case 6:
                                    {
                                        connector2.Location = backupPoints[0];
                                        connector3.Location = backupPoints[1];
                                        connector4.Location = backupPoints[2];
                                        connector5.Location = backupPoints[3];

                                        break;
                                    }
                            }                            
                        }
                        else
                        {
                            if (headY > tailY) // ͷ�����β���֮��
                            {
                                ConnectorCount = 2;                                
                            }
                            else // ͷ�����β���֮��
                            {
                                switch(connectorCount)
                                {
                                    case 4:
                                        {
                                            int middleY = (int)((connector1.Location.Y + connector4.Location.Y) / 2);
                                            connector2.Location = new Point(connector1Location.X + 50, middleY);
                                            connector3.Location = new Point(connector4Location.X - 50, middleY);

                                            break;
                                        }
                                    case 6:
                                        {
                                            int middleX = (int)((connector1.Location.X + connector6.Location.X) / 2);
                                            connector2.Location = connector1Location + new Size(0, 50);
                                            connector5.Location = connector6Location + new Size(0, -50);
                                            connector3.Location = new Point(middleX, connector2Location.Y);
                                            connector4.Location = new Point(middleX, connector5Location.Y);

                                            break;
                                        }
                                }                                
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// ����������Ƿ����������ཻ
        /// </summary>
        /// <param name="slotContainer">�������</param>
        /// <param name="headPoint">ͷ���</param>
        /// <param name="tailPoint">β���</param>
        /// <returns>�Ƿ��ཻ</returns>
        private bool LineIntersect(SlotContainer slotContainer, Point headPoint, Point tailPoint)
        {
            if (slotContainer != inSlotContainer && slotContainer != outSlotContainer)
            {
                Rectangle rectangle = slotContainer.TextRectangle;
                
                int x1 = headPoint.X;
                int y1 = headPoint.Y;
                int x2 = tailPoint.X;
                int y2 = tailPoint.Y;
                int x, y;

                int topY = y1;
                int bottomY = y2;
                if (y1 > y2)
                {
                    topY = y2;
                    bottomY = y1;
                }

                int leftX = x1;
                int rightX = x2;
                if (x1 > x2)
                {
                    leftX = x2;
                    rightX = x1;
                }

                if (x1 == x2) // ��ֱ��
                {
                    if (x1 >= rectangle.Left && x1 <= rectangle.Right && topY <= rectangle.Top
                        && bottomY >= rectangle.Bottom)
                    {
                        return true;
                    }
                }
                else if (y1 == y2) // ˮƽ��
                {
                    if (y1 >= rectangle.Top && y1 <= rectangle.Bottom && leftX < rectangle.Left
                        && rightX >= rectangle.Right)
                    {
                        return true;
                    }
                }
                else
                {
                    // ����ϱ�
                    x = (x2 - x1) * (rectangle.Top - y1) / (y2 - y1) + x1;
                    if (x > rectangle.Left && x < rectangle.Right
                        && topY < rectangle.Top && bottomY > rectangle.Top)
                    {
                        return true;
                    }

                    // ����±�
                    x = (x2 - x1) * (rectangle.Bottom - y1) / (y2 - y1) + x1;
                    if (x > rectangle.Left && x < rectangle.Right
                        && topY < rectangle.Bottom && bottomY > rectangle.Bottom)
                    {
                        return true;
                    }

                    // ������
                    y = (y2 - y1) * (rectangle.Left - x1) / (x2 - x1) + y1;
                    if (y > rectangle.Top && y < rectangle.Bottom
                        && leftX < rectangle.Left && rightX > rectangle.Left)
                    {
                        return true;
                    }

                    // ����ұ�
                    y = (y2 - y1) * (rectangle.Right - x1) / (x2 - x1) + y1;
                    if (y > rectangle.Top && y < rectangle.Bottom
                        && leftX < rectangle.Right && rightX > rectangle.Right)
                    {
                        return true;
                    }
                }                
            }

            return false;
        }

        /// <summary>
        /// �������Ƿ��뵱ǰ�������ཻ
        /// </summary>
        /// <param name="slotContainer">�������</param>
        /// <return>�Ƿ��ཻ</return>
        public bool LineIntersect(SlotContainer slotContainer)
        {
            if (slotContainer != inSlotContainer && slotContainer != outSlotContainer)
            {
                Rectangle rectangle = slotContainer.TextRectangle;

                if(connectorList.Count == 6)
                {
                    if (LineIntersect(rectangle, connectorList[0].Location, connectorList[1].Location))
                    {
                        return true;
                    }

                    if (LineIntersect(rectangle, connectorList[1].Location, connectorList[2].Location))
                    {
                        return true;
                    }

                    if (LineIntersect(rectangle, connectorList[2].Location, connectorList[3].Location))
                    {
                        return true;
                    }

                    if (LineIntersect(rectangle, connectorList[3].Location, connectorList[4].Location))
                    {
                        return true;
                    }

                    if (LineIntersect(rectangle, connectorList[4].Location, connectorList[5].Location))
                    {
                        return true;
                    }
                }
                else
                {
                    int x1 = connectorList[0].Location.X;
                    int y1 = connectorList[0].Location.Y;
                    int x2 = connectorList[1].Location.X;
                    int y2 = connectorList[1].Location.Y;
                    int x, y;

                    int topY = y1;
                    int bottomY = y2;
                    if (y1 > y2)
                    {
                        topY = y2;
                        bottomY = y1;
                    }

                    int leftX = x1;
                    int rightX = x2;
                    if (x1 > x2)
                    {
                        leftX = x2;
                        rightX = x1;
                    }

                    if(x1 == x2) // ��ֱ��
                    {
                        if (x1 >= rectangle.Left && x1 <= rectangle.Right && topY <= rectangle.Top
                            && bottomY >= rectangle.Bottom)
                        {
                            return true;
                        }
                    }
                    else if (y1 == y2) // ˮƽ��
                    {
                        if (y1 >= rectangle.Top && y1 <= rectangle.Bottom && leftX < rectangle.Left
                            && rightX >= rectangle.Right)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        // ����ϱ�
                        x = (x2 - x1) * (rectangle.Top - y1) / (y2 - y1) + x1;
                        if (x > rectangle.Left && x < rectangle.Right
                            && topY < rectangle.Top && bottomY > rectangle.Top)
                        {
                            return true;
                        }                        

                        // ����±�
                        x = (x2 - x1) * (rectangle.Bottom - y1) / (y2 - y1) + x1;
                        if (x > rectangle.Left && x < rectangle.Right
                            && topY < rectangle.Bottom && bottomY > rectangle.Bottom)
                        {
                            return true;
                        }

                        // ������
                        y = (y2 - y1) * (rectangle.Left - x1) / (x2 - x1) + y1;
                        if (y > rectangle.Top && y < rectangle.Bottom
                            && leftX < rectangle.Left && rightX > rectangle.Left)
                        {
                            return true;
                        }

                        // ����ұ�
                        y = (y2 - y1) * (rectangle.Right - x1) / (x2 - x1) + y1;
                        if (y > rectangle.Top && y < rectangle.Bottom
                            && leftX < rectangle.Right && rightX > rectangle.Right)
                        {
                            return true;
                        }
                    }
                }
            }                  

            return false;
        }

        /// <summary>
        /// ��������������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <param name="pointList">������������</param>
        /// <returns>�Ƿ��е��ڵ�ǰ��������</returns>
        protected bool PointInLine(Point p, List<Point> pointList)
        {
            switch (pointList.Count)
            {
                case 4:
                    {
                        if ((p.X == pointList[0].X) && (p.Y >= pointList[0].Y && p.Y <= pointList[1].Y))
                        {
                            return true;
                        }

                        if ((p.Y == pointList[1].Y) && ((p.X >= pointList[1].X && p.X <= pointList[2].X)
                            || (p.X >= pointList[2].X && p.X <= pointList[1].X)))
                        {
                            return true;
                        }                        

                        if ((p.X == pointList[3].X) && (p.Y >= pointList[2].Y && p.Y <= pointList[3].Y))
                        {
                            return true;
                        }

                        break;
                    }
                case 6:
                    {
                        if ((p.X == pointList[0].X) && (p.Y >= pointList[0].Y && p.Y <= pointList[1].Y))
                        {
                            return true;
                        }

                        if ((p.Y == pointList[1].Y) && ((p.X >= pointList[1].X && p.X <= pointList[2].X)
                            || (p.X >= pointList[2].X && p.X <= pointList[1].X)))
                        {
                            return true;
                        }

                        if ((p.Y == pointList[3].Y) && ((p.X >= pointList[3].X && p.X <= pointList[4].X)
                            || (p.X >= pointList[4].X && p.X <= pointList[3].X)))
                        {
                            return true;
                        }

                        if ((p.X == pointList[2].X) && ((p.Y >= pointList[3].Y && p.Y <= pointList[2].Y)
                            || (p.Y >= pointList[2].Y && p.Y <= pointList[3].Y)))
                        {
                            return true;
                        }

                        if ((p.X == pointList[4].X) && (p.Y >= pointList[4].Y && p.Y <= pointList[5].Y))
                        {
                            return true;
                        }

                        break;
                    }
            }            

            return false;
        }

        /// <summary>
        /// �������Ƿ����������ཻ
        /// </summary>
        /// <param name="rectangle">����</param>
        /// <param name="point1">������ͷ���</param>
        /// <param name="point2">������β���</param>
        /// <returns>�Ƿ��ཻ</returns>
        protected bool LineIntersect(Rectangle rectangle, Point point1, Point point2)
        {
            bool result = false;

            Point startPoint = point1;
            Point endPoint = point2;
            if(startPoint.X > endPoint.X || startPoint.Y > endPoint.Y)
            {
                startPoint = point2;
                endPoint = point1;
            }
            
            if(startPoint.X == endPoint.X) // ����
            {
                result = (rectangle.Left <= startPoint.X && rectangle.Right >= startPoint.X)
                            && ((rectangle.Top > startPoint.Y && rectangle.Top < endPoint.Y)
                                || (rectangle.Bottom > startPoint.Y && rectangle.Bottom < endPoint.Y)
                                || (rectangle.Top < startPoint.Y && rectangle.Bottom > endPoint.Y));
            }
            else // ����
            {
                result = (rectangle.Top <= startPoint.Y && rectangle.Bottom >= startPoint.Y)
                            && ((rectangle.Left > startPoint.X && rectangle.Left < endPoint.X)
                                || (rectangle.Right > startPoint.X && rectangle.Right < endPoint.X)
                                || (rectangle.Left < startPoint.X && rectangle.Right > endPoint.X));
            }
                                            

            return result;
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="currentConnector">��ǰ�������߿��Ƶ�</param>
        public virtual void AdjustLine(ConnectorGraphElement currentConnector)
        {
            if (inSlotContainer != null && outSlotContainer != null) // ���������˾�������ͼԪ
            {
                int headX = headConnector.Location.X;
                int headY = headConnector.Location.Y;
                int tailX = tailConnector.Location.X;
                int tailY = tailConnector.Location.Y;

                switch (connectorCount)
                {
                    case 2:
                        {
                            break;
                        }
                    case 4:
                        {
                            ConnectorGraphElement connector1 = connectorList[0];
                            ConnectorGraphElement connector2 = connectorList[1];
                            ConnectorGraphElement connector3 = connectorList[2];
                            ConnectorGraphElement connector4 = connectorList[3];

                            if (currentConnector == connector1)
                            {
                                connector2.Location = new Point(connector1.Location.X, connector2.Location.Y);
                            }
                            else if (currentConnector == connector2)
                            {
                                connector2.Location = new Point(connector1.Location.X, connector2.Location.Y);
                                connector3.Location = new Point(connector4.Location.X, connector2.Location.Y);
                            }
                            else if (currentConnector == connector3)
                            {                                
                                connector3.Location = new Point(connector4.Location.X, connector3.Location.Y);
                                connector2.Location = new Point(connector1.Location.X, connector3.Location.Y);                                
                            }
                            else if (currentConnector == connector4)
                            {
                                connector3.Location = new Point(connector4.Location.X, connector3.Location.Y);                                
                            }                            

                            break;
                        }
                    case 6:
                        {
                            ConnectorGraphElement connector1 = connectorList[0];
                            ConnectorGraphElement connector2 = connectorList[1];
                            ConnectorGraphElement connector3 = connectorList[2];
                            ConnectorGraphElement connector4 = connectorList[3];
                            ConnectorGraphElement connector5 = connectorList[4];
                            ConnectorGraphElement connector6 = connectorList[5];

                            if (currentConnector == connector1)
                            {
                                connector2.Location = new Point(connector1.Location.X, connector2.Location.Y);
                            }
                            else if (currentConnector == connector2)
                            {
                                connector2.Location = new Point(connector1.Location.X, connector2.Location.Y);
                                connector3.Location = new Point(connector3.Location.X, connector2.Location.Y);
                            }
                            else if (currentConnector == connector3)
                            {
                                connector2.Location = new Point(connector1.Location.X, connector3.Location.Y);
                                connector4.Location = new Point(connector3.Location.X, connector5.Location.Y);
                            }
                            else if (currentConnector == connector4)
                            {
                                connector3.Location = new Point(connector4.Location.X, connector3.Location.Y);
                                connector5.Location = new Point(connector6.Location.X, connector4.Location.Y);
                            }
                            else if (currentConnector == connector5)
                            {
                                connector4.Location = new Point(connector4.Location.X, connector5.Location.Y);
                                connector5.Location = new Point(connector6.Location.X, connector5.Location.Y);
                            }
                            else if (currentConnector == connector6)
                            {
                                connector5.Location = new Point(connector6.Location.X, connector5.Location.Y);
                            }

                            break;
                        }
                }
            }   
        }

        /// <summary>
        /// ȡ��������Ĳ������
        /// </summary>
        public virtual void UnbindInSlotContainer()
        {
            if(inSlotContainer != null)
            {
                inSlotContainer.UnBindLine(this);
            }
        }

        /// <summary>
        /// ȡ���������Ĳ������
        /// </summary>
        public virtual void UnbindOutSlotContainer()
        {
            if(outSlotContainer != null)
            {
                outSlotContainer.UnBindLine(this);
            }
        }

        /// <summary>
        /// ����1��2�����ӵ�
        /// </summary>
        /// <param name="g">��ͼ����</param>
        /// <param name="framePen">��ͼ��</param>
        /// <param name="direction">��������</param>
        private void PaintConnector1And2(Graphics g, Pen framePen,Direction direction)
        {
            switch (direction)
            {
                case Direction.Left:
                    {
                        if (connector1Location.Y > connector2Location.Y)
                        {
                            g.DrawArc(framePen, connector2Location.X, connector2Location.Y, 16, 16, 180, 90);
                            g.DrawLine(framePen, connector2Location.X, connector2Location.Y + 8, connector1Location.X, connector1Location.Y);
                        }
                        else
                        {
                            g.DrawArc(framePen, connector2Location.X, connector2Location.Y - 16, 16, 16, 90, 90);
                            g.DrawLine(framePen, connector1Location.X, connector1Location.Y, connector2Location.X, connector2Location.Y - 8);
                        }                        

                        break;
                    }
                case Direction.Right:
                    {
                        if (connector1Location.Y > connector2Location.Y)
                        {
                            g.DrawArc(framePen, connector2Location.X - 16, connector2Location.Y, 16, 16, 270, 90);
                            g.DrawLine(framePen, connector2Location.X, connector2Location.Y + 8, connector1Location.X, connector1Location.Y);
                        }
                        else
                        {
                            g.DrawArc(framePen, connector2Location.X - 16, connector2Location.Y - 16, 16, 16, 0, 90);
                            g.DrawLine(framePen, connector1Location.X, connector1Location.Y, connector2Location.X, connector2Location.Y - 8);
                        }

                        break;
                    }
            }
        }

        /// <summary>
        /// ����2�ź�3�����ӵ�
        /// </summary>
        /// <param name="g">��ͼ����</param>
        /// <param name="framePen">��ͼ��</param>
        /// <param name="direction">��������</param>
        private void PaintConnector2And3(Graphics g, Pen framePen, Direction direction)
        {
            switch (direction)
            {
                case Direction.Left:
                    {
                        g.DrawLine(framePen, connector2Location.X + 8, connector2Location.Y, connector3Location.X - 8, connector3Location.Y);

                        break;
                    }
                case Direction.Right:
                    {
                        g.DrawLine(framePen, connector2Location.X - 8, connector2Location.Y, connector3Location.X + 8, connector3Location.Y);

                        break;
                    }
            }
        }

        /// <summary>
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void Paint(Graphics g)
        {
            if (visible)
            {
                Pen framePen;
                Pen frameArrawPen;

                if (activated)
                {
                    framePen = graphSetting.LineActivatedPen;
                    frameArrawPen = graphSetting.LineActivatedArrawPen;
                }
                else if (selected)
                {
                    framePen = graphSetting.LineSelectedPen;
                    frameArrawPen = graphSetting.LineSelectedArrawPen;
                }
                else if (marked)
                {
                    framePen = graphSetting.LineMarkedPen;
                    frameArrawPen = graphSetting.LineMarkedArrawPen;
                }
                else
                {
                    framePen = graphSetting.LineNormalPen;
                    frameArrawPen = graphSetting.LineNormalArrawPen;
                }

                switch (connectorCount)
                {
                    case 2: // 2�������߿��Ƶ��ʱ��ֱ�ӻ�ֱ�߼���
                        {
                            g.DrawLine(frameArrawPen, connectorList[0].Location, connectorList[1].Location);

                            break;
                        }
                    case 4: // 4�������߿��Ƶ�
                        {
                            connector1Location = connectorList[0].Location;
                            connector2Location = connectorList[1].Location;
                            connector3Location = connectorList[2].Location;
                            connector4Location = connectorList[3].Location;
                            
                            if (connector2Location.X > connector3Location.X)
                            {
                                PaintConnector1And2(g, framePen, Direction.Right);                                

                                if (connector3Location.Y > connector4Location.Y)
                                {
                                    g.DrawArc(framePen, connector3Location.X, connector3Location.Y - 16, 16, 16, 90, 90);
                                    g.DrawLine(frameArrawPen, connector3Location.X, connector3Location.Y - 8, connector4Location.X, connector4Location.Y);
                                }
                                else
                                {
                                    g.DrawArc(framePen, connector3Location.X, connector3Location.Y, 16, 16, 180, 90);
                                    g.DrawLine(frameArrawPen, connector3Location.X, connector3Location.Y + 8, connector4Location.X, connector4Location.Y);
                                }

                                PaintConnector2And3(g, framePen, Direction.Right);                                
                            }
                            else
                            {
                                PaintConnector1And2(g, framePen, Direction.Left);

                                if (connector3Location.Y > connector4Location.Y)
                                {
                                    g.DrawArc(framePen, connector3Location.X - 16, connector3Location.Y - 16, 16, 16, 0, 90);
                                    g.DrawLine(frameArrawPen, connector3Location.X, connector3Location.Y - 8, connector4Location.X, connector4Location.Y);
                                }
                                else
                                {
                                    g.DrawArc(framePen, connector3Location.X - 16, connector3Location.Y, 16, 16, 270, 90);
                                    g.DrawLine(frameArrawPen, connector3Location.X, connector3Location.Y + 8, connector4Location.X, connector4Location.Y);
                                }

                                PaintConnector2And3(g, framePen, Direction.Left);                                
                            }                            

                            break;
                        }
                    case 6: // 6�������߿��Ƶ�
                        {
                            connector1Location = connectorList[0].Location;
                            connector2Location = connectorList[1].Location;
                            connector3Location = connectorList[2].Location;
                            connector4Location = connectorList[3].Location;
                            connector5Location = connectorList[4].Location;
                            connector6Location = connectorList[5].Location;                         

                            if (connector2Location.X > connector3Location.X)
                            {
                                PaintConnector1And2(g, framePen, Direction.Right);
                                PaintConnector2And3(g, framePen, Direction.Right);
                            }
                            else
                            {
                                PaintConnector1And2(g, framePen, Direction.Left);
                                PaintConnector2And3(g, framePen, Direction.Left);
                            }

                            if (connector3Location.Y > connector4Location.Y)
                            {
                                g.DrawLine(framePen, connector3Location.X, connector3Location.Y - 8, connector4Location.X, connector4Location.Y + 8);

                                if (connector2Location.X > connector3Location.X)
                                {
                                    g.DrawArc(framePen, connector3Location.X, connector3Location.Y - 16, 16, 16, 90, 90);
                                }
                                else
                                {
                                    g.DrawArc(framePen, connector3Location.X - 16, connector3Location.Y - 16, 16, 16, 0, 90);
                                }
                            }
                            else
                            {
                                g.DrawLine(framePen, connector3Location.X, connector3Location.Y + 8, connector4Location.X, connector4Location.Y - 8);

                                if (connector2Location.X > connector3Location.X)
                                {
                                    g.DrawArc(framePen, connector3Location.X, connector3Location.Y, 16, 16, 180, 90);
                                }
                                else
                                {
                                    g.DrawArc(framePen, connector3Location.X - 16, connector3Location.Y, 16, 16, 270, 90);
                                }
                            }

                            if (connector4Location.X > connector5Location.X)
                            {
                                g.DrawLine(framePen, connector4Location.X - 8, connector4Location.Y, connector5Location.X + 8, connector5Location.Y);

                                if (connector3Location.Y > connector4Location.Y)
                                {
                                    g.DrawArc(framePen, connector4Location.X - 16, connector4Location.Y, 16, 16, 270, 90);
                                }
                                else
                                {
                                    g.DrawArc(framePen, connector4Location.X - 16, connector4Location.Y - 16, 16, 16, 0, 90);
                                }
                            }
                            else
                            {
                                g.DrawLine(framePen, connector4Location.X + 8, connector4Location.Y, connector5Location.X - 8, connector5Location.Y);

                                if (connector3Location.Y > connector4Location.Y)
                                {
                                    g.DrawArc(framePen, connector4Location.X, connector4Location.Y, 16, 16, 180, 90);
                                }
                                else
                                {
                                    g.DrawArc(framePen, connector4Location.X, connector4Location.Y - 16, 16, 16, 90, 90);
                                }
                            }


                            if (connector4Location.X > connector5Location.X)
                            {
                                if (connector5Location.Y > connector6Location.Y)
                                {
                                    g.DrawArc(framePen, connector5Location.X, connector5Location.Y - 16, 16, 16, 90, 90);
                                    g.DrawLine(frameArrawPen, connector5Location.X, connector5Location.Y - 8, connector6Location.X, connector6Location.Y);
                                }
                                else
                                {
                                    g.DrawArc(framePen, connector5Location.X, connector5Location.Y, 16, 16, 180, 90);
                                    g.DrawLine(frameArrawPen, connector5Location.X, connector5Location.Y + 8, connector6Location.X, connector6Location.Y);
                                }                                
                            }
                            else
                            {
                                if (connector5Location.Y > connector6Location.Y)
                                {
                                    g.DrawArc(framePen, connector5Location.X - 16, connector5Location.Y - 16, 16, 16, 0, 90);
                                    g.DrawLine(frameArrawPen, connector5Location.X, connector5Location.Y - 8, connector6Location.X, connector6Location.Y);
                                }
                                else
                                {
                                    g.DrawArc(framePen, connector5Location.X - 16, connector5Location.Y, 16, 16, 270, 90);
                                    g.DrawLine(frameArrawPen, connector5Location.X, connector5Location.Y + 8, connector6Location.X, connector6Location.Y);
                                }                                
                            }

                            break;
                        }
                }

                if (selected || activated) // ֻ�б�ѡ�л򼤻�ʱ�Ż��������߿��Ƶ�
                {
                    // ���������߿��Ƶ�
                    foreach (ConnectorGraphElement connector in connectorList)
                    {
                        connector.Paint(g);
                    }
                }
                else
                {
                    foreach (ConnectorGraphElement connector in connectorList)
                    {
                        if (connector.Activated || connector.Selected) // �����߿��Ƶ㱻ѡ�л򼤻�
                        {
                            connector.Paint(g);
                        }
                    }
                }

                // ����ID
                if (showID)
                {
                    g.DrawString(id.ToString(), graphSetting.GraphElementTextBoldFont, graphSetting.LineTextBrush,
                                 tailConnector.Location.X - 50, tailConnector.Location.Y);
                }
            }
        }

        /// <summary>
        /// �����ı�
        /// </summary>
        /// <param name="g">��ͼ����</param>
        public virtual void PaintText(Graphics g)
        {
            // ���ɼ����ƶ�ʱ�����û����ı�
            if (visible && !moving) 
            {      
                if (displayText == "...")
                {
                    g.FillRectangle(graphSetting.BackgroundBrush, textRectangleList[0]);

                    if (selected)
                    {
                        g.DrawString(displayText, graphSetting.GraphElementTextBoldFont, graphSetting.GraphElementTextSelectedBrush, textRectangleList[0].Location);
                    }
                    else
                    {
                        g.DrawString(displayText, graphSetting.GraphElementTextFont, graphSetting.LineTextBrush, textRectangleList[0].Location);
                    }
                }
                else
                {
                    for (int i = 0; i < displayText.Length; i++)
                    {
                        g.FillRectangle(graphSetting.BackgroundBrush, textRectangleList[i]);

                        if (selected)
                        {
                            g.DrawString(displayText.Substring(i, 1), graphSetting.GraphElementTextBoldFont, graphSetting.GraphElementTextSelectedBrush, textRectangleList[i].Location);
                        }
                        else
                        {
                            g.DrawString(displayText.Substring(i, 1), graphSetting.GraphElementTextFont, graphSetting.LineTextBrush, textRectangleList[i].Location);
                        }
                    }
                }                
            }            
        }

        /// <summary>
        /// ���������ı�������
        /// </summary>
        public virtual void AdjustText()
        {            
            Graphics g = Helper.GetHelper().GraphicsObject;

            if (textRectangleList == null) // �������л��汾��ԭ����Ҫ��ʼ��һ��
            {
                textRectangleList = new List<Rectangle>();
            }

            textRectangleList.Clear();
            Point headLocation = connectorList[1].Location;
            Point tailLocation = connectorList[0].Location;
           
            double length = Math.Pow(tailLocation.X - headLocation.X, 2) + Math.Pow(tailLocation.Y - headLocation.Y, 2);            
            
            if (Math.Pow((text.Length - 1) * textMargin + textBorderWidth * 2, 2) > length)
            {
                displayText = "...";
            }
            else
            {
                displayText = text;
            }
            
            if (!string.IsNullOrEmpty(displayText))
            {                
                float rate = 0;

                if (headLocation.X != tailLocation.X)
                {
                    rate = Math.Abs((float)(headLocation.Y - tailLocation.Y) / (headLocation.X - tailLocation.X));
                }

                float widthOffset = (float)Math.Sqrt(Math.Pow(textBorderWidth, 2) / (Math.Pow(rate, 2) + 1));
                float heightOffset = textBorderWidth;

                if (displayText == "...")
                {
                    SizeF stringSize = g.MeasureString(displayText, graphSetting.GraphElementTextFont);
                    float charX = 0;
                    float charY = 0;

                    if (rate != 0) // б��
                    {
                        if (tailLocation.X < headLocation.X)
                        {
                            charX = tailLocation.X + widthOffset;
                        }
                        else
                        {
                            charX = tailLocation.X - widthOffset;
                        }

                        charY = tailLocation.Y + Math.Abs(charX - tailLocation.X) * rate;
                    }
                    else // ��ֱ��
                    {
                        charX = tailLocation.X;
                        charY = tailLocation.Y + heightOffset;
                    }

                    textRectangleList.Add(new Rectangle(new Point((int)(charX - stringSize.Width / 2), (int)(charY - stringSize.Height / 2)), 
                                          new Size((int)stringSize.Width, (int)stringSize.Height)));
                }
                else
                {
                    float charIncreasement = (float)Math.Sqrt(Math.Pow(textMargin, 2) / (Math.Pow(rate, 2) + 1)); // �ַ��������

                    for (int i = 0; i < displayText.Length; i++)
                    {
                        SizeF charSize = g.MeasureString(displayText.Substring(i, 1), graphSetting.GraphElementTextFont);
                        float charX = 0;
                        float charY = 0;

                        if (rate != 0) // б��
                        {
                            if (tailLocation.X < headLocation.X)
                            {
                                charX = tailLocation.X + widthOffset;
                            }
                            else
                            {
                                charX = tailLocation.X - widthOffset;
                            }

                            charY = tailLocation.Y + Math.Abs(charX - tailLocation.X) * rate;
                         }
                        else // ��ֱ��
                        {
                            charX = tailLocation.X;
                            charY = tailLocation.Y + heightOffset;
                        }

                        textRectangleList.Add(new Rectangle(new Point((int)(charX - charSize.Width / 2), (int)(charY - charSize.Height / 2)),
                                              new Size((int)charSize.Width, (int)charSize.Height)));
                        widthOffset += charIncreasement;
                        heightOffset += textMargin;                    
                    }
                }                
            }
        }

        /// <summary>
        /// �������Ե�ͼԪ
        /// </summary>
        /// <param name="g">��ͼ����</param>
        public override void AbbreviativePaint(Graphics g)
        {
            // ���Ʊ߿�
            int index = 0;
            ConnectorGraphElement head;
            ConnectorGraphElement tail;

            while (index + 1 < connectorList.Count)
            {
                head = connectorList[index];
                tail = connectorList[index + 1];
                                
                g.DrawLine(graphSetting.LineNormalPen, head.Location, tail.Location);
                index++;
            }
        }

        /// <summary>
        /// �ж�ĳ�����Ƿ��ڸ�ͼԪ��������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�õ��Ƿ���ͼԪ��������</returns>
        public override bool IsInRegion(Point p)
        {
            bool result = false;

            int index = 0;
            ConnectorGraphElement head;
            ConnectorGraphElement tail;

            while (index + 1 < connectorList.Count)
            {
                head = connectorList[index];
                tail = connectorList[index + 1];
                result = CheckHitLine(head.Location, tail.Location, p);
                if (result)
                {
                    break;
                }
                index++;
            }

            return result;
        }

        /// <summary>
        /// �ж�ͼԪ�Ƿ���ĳ��������
        /// </summary>
        /// <param name="r">��ǰ����</param>
        /// <returns>ͼԪ�Ƿ��ڵ�ǰ������</returns>
        public override bool IsInRegion(Rectangle r)
        {
            bool result = false;

            AdjustRectangle(); // ���������ߵķ�Χ����
            Rectangle rectangle = new Rectangle(location, elementSize);
            result = r.Contains(rectangle);

            return result;
        }

        /// <summary>
        /// �ƶ�ͼԪ
        /// </summary>
        /// <param name="p">�ƶ������λ��</param>
        public override void Move(Size s)
        {
            if (inSlotContainer == null && outSlotContainer == null) // ֻ��������û�а�ͼԪʱ�����ƶ�
            {
                foreach (ConnectorGraphElement connector in connectorList)
                {
                    connector.Move(s);                    
                }

                location += s;
            }
        }

        /// <summary>
        /// ��ʼ�������߿��Ƶ�
        /// </summary>
        /// <param name="tail">β�����ӿ��Ƶ�</param>
        /// <param name="head">ͷ�����ӿ��Ƶ�</param>
        protected virtual void InitConnector(Point tail, Point head)
        {
            int x1 = tail.X - 3;
            int y1 = tail.Y - 3;
            int x2 = head.X - 3;
            int y2 = head.Y - 3;

            connectorList.Clear();
            ConnectorGraphElement connector1 = new ConnectorGraphElement(this, new Point(x1, y1), new Size(6, 6));
            ConnectorGraphElement connector2 = new ConnectorGraphElement(this, new Point(x2, y2), new Size(6, 6));

            connector1.Name = "�����߿��Ƶ�";
            connector1.IsTailPoint = true;

            connector2.Name = "�����߿��Ƶ�";
            connector2.IsHeadPoint = true;

            tailConnector = connector1;
            headConnector = connector2;

            connectorList.Add(connector1);
            connectorList.Add(connector2);
        }

        /// <summary>
        /// ���ֱ���Ƿ񱻵�ǰ��ѡ��
        /// </summary>
        /// <param name="p1">ֱ�����</param>
        /// <param name="p2">ֱ���յ�</param>
        /// <returns>��ǰ������ڵ�</returns>
        protected virtual bool CheckHitLine(Point p1, Point p2, Point p)
        {
            Point s;
            RectangleF r1, r2;
            float o, u;

            // p1 must be the leftmost point.
            if (p1.X > p2.X)
            {
                s = p2;
                p2 = p1;
                p1 = s;
            }

            r1 = new RectangleF(p1.X, p1.Y, 0, 0);
            r2 = new RectangleF(p2.X, p2.Y, 0, 0);
            r1.Inflate(3, 3);
            r2.Inflate(3, 3);

            // this is like a topological neighborhood
            // the connection is shifted left and right
            // and the point under consideration has to be in between.						
            if (RectangleF.Union(r1, r2).Contains(p))
            {
                if (p1.Y < p2.Y) // SWNE
                {
                    o = r1.Left + (((r2.Left - r1.Left) * (p.Y - r1.Bottom)) / (r2.Bottom - r1.Bottom));
                    u = r1.Right + (((r2.Right - r1.Right) * (p.Y - r1.Top)) / (r2.Top - r1.Top));
                    return ((p.X > o) && (p.X < u));
                }
                else if(p1.Y == p2.Y)
                {
                    return (p.X >= p1.X && p.X <= p2.X);
                }
                else // NWSE
                {
                    o = r1.Left + (((r2.Left - r1.Left) * (p.Y - r1.Top)) / (r2.Top - r1.Top));
                    u = r1.Right + (((r2.Right - r1.Right) * (p.Y - r1.Bottom)) / (r2.Bottom - r1.Bottom));
                    return ((p.X > o) && (p.X < u));
                }
            }

            return false;
        }

        /// <summary>
        /// ���������߿��Ƶ�����
        /// </summary>
        /// <param name="n">�����߿��Ƶ�����</param>
        protected virtual void AdjustConnector(int n)
        {
            if (n == connectorList.Count) // �����߿��Ƶ�����û�б仯
            {
                return;
            }

            if (n < connectorList.Count) // �����߿��Ƶ���������
            {
                List<ConnectorGraphElement> deleteConnectorList = new List<ConnectorGraphElement>(); // Ҫɾ���������߿��Ƶ�����
                int deleteCount = connectorList.Count - n;
                for (int i = 0; i < deleteCount; i++)
                {
                    deleteConnectorList.Add(connectorList[connectorList.Count - 2 - i]);
                }

                foreach (ConnectorGraphElement connector in deleteConnectorList)
                {
                    connectorList.Remove(connector);
                }
            }
            else // �����߿��Ƶ���������
            {
                int addCount = n - connectorList.Count;
                Point point1 = connectorList[connectorList.Count - 2].Location;
                Point point2 = connectorList[connectorList.Count - 1].Location;
                Point newPoint;
                ConnectorGraphElement newConnector;
                ConnectorGraphElement headConnector;
                int newX;
                int newY;

                // ���Ƴ������ߵ�ͷ���ӿ��Ƶ㣬�������ӻ�ȥ����֤�¼ӵĵ㶼�������ڶ���������һ����֮��
                headConnector = connectorList[connectorList.Count - 1];
                connectorList.Remove(headConnector);

                for (int i = 1; i <= addCount; i++)
                {
                    newX = (int)(point1.X + (point2.X - point1.X) * i / (addCount + 1) - 3);
                    newY = (int)(point1.Y + (point2.Y - point1.Y) * i / (addCount + 1) - 3);
                    newPoint = new Point(newX, newY);
                    newConnector = new ConnectorGraphElement(this, newPoint, new Size(6, 6));
                    newConnector.Name = "�����߿��Ƶ�";
                    newConnector.Refresh();
                    connectorList.Add(newConnector);
                }
                connectorList.Add(headConnector);
            }
        }

        /// <summary>
        /// ���������߶�Ӧ���������
        /// </summary>
        public virtual void AdjustRectangle()
        {
            int leftX = -1;
            int rightX = -1;
            int topY = -1;
            int buttomY = -1;
            int sizeWidth;
            int sizeHeight;

            foreach (ConnectorGraphElement connector in connectorList)
            {
                if ((connector.Location.X < leftX) || (leftX == -1))
                {
                    leftX = connector.Location.X;
                }

                if ((connector.Location.X > rightX) || (rightX == -1))
                {
                    rightX = connector.Location.X;
                }

                if ((connector.Location.Y < topY) || (topY == -1))
                {
                    topY = connector.Location.Y;
                }

                if ((connector.Location.Y > buttomY) || (buttomY == -1))
                {
                    buttomY = connector.Location.Y;
                }
            }

            location = new Point(leftX, topY);
            sizeWidth = rightX - leftX;
            sizeHeight = buttomY - topY;
            elementSize = new Size(sizeWidth, sizeHeight);
            invalidRectangle = new Rectangle(location + new Size(-6, -6), elementSize + new Size(12, 12));
        }

        /// <summary>
        /// ��ȡָ�������ڵ������߿��Ƶ�
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�����߿��Ƶ�</returns>
        public virtual ConnectorGraphElement GetConnectorInRegion(Point p)
        {
            ConnectorGraphElement connector = null;

            foreach (ConnectorGraphElement connectorGraphElement in connectorList)
            {
                if (connectorGraphElement.IsInRegion(p))
                {
                    connector = connectorGraphElement;
                    break;
                }
            }

            return connector;
        }

        /// <summary>
        /// ��ȡ���ӿ��Ƶ�����
        /// </summary>
        /// <returns>���ӿ��Ƶ�����</returns>
        public virtual List<ConnectorGraphElement> GetConnectorList()
        {
            return connectorList;
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public virtual ConnectorContainer Clone()
        {
            ConnectorContainer newLine = new ConnectorContainer(tailConnector.Location, headConnector.Location);
            newLine.Text = text;
            newLine.DisplayText = displayText;
            newLine.TooltipText = tooltipText;
            newLine.ShowText = showText;
            newLine.ShowID = showID;

            newLine.Init();
            newLine.Refresh();

            newLine.ConnectorCount = connectorCount;
            List<ConnectorGraphElement> list = newLine.GetConnectorList();

            for(int i = 0; i < connectorList.Count; i++)
            {
                list[i].Location = connectorList[i].Location;
            }

            return newLine;
        }
    }
}
