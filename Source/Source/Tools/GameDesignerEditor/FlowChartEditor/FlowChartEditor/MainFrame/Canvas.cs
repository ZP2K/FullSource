using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;
using DevComponents.DotNetBar;

namespace FlowChartEditor
{
    public class Canvas:Panel
    {
        private List<GraphElement> graphElementList = new List<GraphElement>(); // ͼԪ����        
        private Background background; // ����

        private GraphElement abbreviatGraphElement; // ���Ե�ͼԪ
        private ConnectorContainer abbreviateLine; // ����ͼԪ��������
        private AbbreviateCanvas abbreviateCanvas; // ����ͼ��ͼ��
        
        private Rectangle visibleRectangle = Rectangle.Empty; // �����������
        private bool paintCanvas = true; // �Ƿ�ˢ�»�ͼ��

        private Rectangle tempRectangle = Rectangle.Empty; // ��ʱ����    
        private Size tempSize = Size.Empty; // ��ʱ�ߴ�
        private Point tempPoint = Point.Empty; // ��ʱ��
        private Point currentPoint; // ��ǰ������ڵ�        

        private Tooltip tooltipGraphElement = new Tooltip(Point.Empty); // ��ʾ�����
        private Rodman rodman = new Rodman(Point.Empty, new Size(10, 25)); // �α����
        private MultiSelectMark multiSelectMark = new MultiSelectMark(Point.Empty, Size.Empty); // ��ѡ���
        private GuideLine guideLine = new GuideLine(Point.Empty); // �����߶���

        protected GraphSetting graphSetting = GraphSetting.GetGraphSetting(); // ��ͼ�������ö���              

        /// <summary>
        /// ���캯��
        /// </summary>
        public Canvas()
        {
            // ����˫����Ȼ��Ʋ���
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.ResizeRedraw, true);                 
        }        

        /// <summary>
        /// �����߶���
        /// </summary>
        public GuideLine CurrentGuideLine
        {
            get
            {
                return guideLine;
            }
        }

        /// <summary>
        /// ��ʾ�����
        /// </summary>
        public Tooltip TooltipGraphElement
        {
            get
            {
                return tooltipGraphElement;
            }
        }

        /// <summary>
        /// ��ѡ���
        /// </summary>
        public MultiSelectMark CurrentMultiSelectMark
        {
            get
            {
                return multiSelectMark;
            }
        }

        /// <summary>
        /// �α����
        /// </summary>
        public Rodman CurrentRodman
        {
            set
            {
                rodman = value;
            }

            get
            {
                return rodman;
            }
        }

        /// <summary>
        /// �����������
        /// </summary>
        public Rectangle VisibleRectangle
        {
            get
            {
                return visibleRectangle;
            }
        }

        /// <summary>
        /// �Ƿ�ˢ�»�ͼ��
        /// </summary>
        public bool PaintCanvas
        {
            set
            {
                paintCanvas = value;

                if (abbreviateCanvas != null)
                {
                    abbreviateCanvas.PaintCanvas = value;
                }
            }

            get
            {
                return paintCanvas;
            }
        }

        /// <summary>
        /// ����ͼ��ͼ��
        /// </summary>
        public AbbreviateCanvas CurrentAbbreviateCanvas
        {
            set
            {
                abbreviateCanvas = value;
            }

            get
            {
                return abbreviateCanvas;
            }
        }

        /// <summary>
        /// ͼԪ��������
        /// </summary>
        public List<GraphElement> GraphElementList
        {
            set
            {
                graphElementList = value;
            }

            get
            {
                return graphElementList;
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        public Background CanvasBackgound
        {
            set
            {
                background = value;
            }

            get
            {
                return background;
            }
        }

        /// <summary>
        /// ���Ե�ͼԪ����
        /// </summary>
        public GraphElement AbbreviatGraphElement
        {
            set
            {
                abbreviatGraphElement = value;
            }

            get
            {
                return abbreviatGraphElement;
            }
        }

        /// <summary>
        /// ���Ե�����������
        /// </summary>
        public ConnectorContainer AbbreviateLine
        {
            set
            {
                abbreviateLine = value;
            }

            get
            {
                return abbreviateLine;
            }
        }

        /// <summary>
        /// ���Ƶķ���
        /// </summary>
        /// <param name="e">�����¼�����</param>
        protected override void  OnPaint(PaintEventArgs e)
        {
 	        base.OnPaint(e);

            if (paintCanvas) // ���ƻ�ͼ��
            {                
                // ��ȡ��ͼ����
                Graphics g = e.Graphics;                              

                // ���û�ͼ����
                g.SmoothingMode = SmoothingMode.HighQuality;
                g.PixelOffsetMode = PixelOffsetMode.HighSpeed;
                // g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit; // ����ı���������
                g.TranslateTransform(this.AutoScrollPosition.X, this.AutoScrollPosition.Y);

                // ���Ż�ͼ��
                g.ScaleTransform(graphSetting.CanvasScale, graphSetting.CanvasScale);                                        

                // ����ͼԪ
                PaintGraghElement(g);   
                
                // �������Ե�ͼԪ
                PaintAbbreviateGraphElement(g);

                // ���ƶ�ѡ��
                multiSelectMark.Paint(g);                                           

                // ���Ƶ�����
                guideLine.Paint(g);

                // �����α�
                rodman.Paint(g);                

                // ���Ƶ�����Ϣ
                PaintDebugMessage(g);

                // ������ʾ��
                tooltipGraphElement.Paint(g);
            }            
        }

        /// <summary>
        /// �������Ե�ͼԪ
        /// </summary>
        /// <param name="g">��ͼ����</param>
        private void PaintAbbreviateGraphElement(Graphics g)
        {
            // �������Ե�ͼԪ
            if (abbreviatGraphElement != null)
            {
                abbreviatGraphElement.Paint(g);
            }

            // ��������ͼԪ��������
            if (abbreviateLine != null && (abbreviateLine.Location.X > 0 && abbreviateLine.Location.Y > 0))
            {
                abbreviateLine.AbbreviativePaint(g);
            }        
        }

        /// <summary>
        /// ���Ƶ�����Ϣ
        /// </summary>
        /// <param name="g">��ͼ����</param>
        private void PaintDebugMessage(Graphics g)
        {
            if (background != null && background.DebugMode)
            {
                g.DrawString(currentPoint.ToString(), graphSetting.GraphElementTextFont, graphSetting.LineTextBrush, currentPoint.X - 100, currentPoint.Y);
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="g">��ͼ����</param>
        private void PaintGraghElement(Graphics g)
        {
            List<ConnectorContainer> textLineList = new List<ConnectorContainer>(); // �����ı�������������

            foreach (GraphElement graphElement in graphElementList)
            {
                tempPoint.X = (int)(graphElement.Location.X * graphSetting.CanvasScale);
                tempPoint.Y = (int)(graphElement.Location.Y * graphSetting.CanvasScale);
                tempSize.Width = (int)(graphElement.ElementSize.Width * graphSetting.CanvasScale);
                tempSize.Height = (int)(graphElement.ElementSize.Height * graphSetting.CanvasScale);
                tempRectangle.Location = tempPoint;
                tempRectangle.Size = tempSize;

                if ((visibleRectangle.Contains(tempRectangle) ||
                    visibleRectangle.IntersectsWith(tempRectangle))) // ��ͼԪ�ڿ����������ٽ��л���
                {
                    graphElement.ShowID = background.DebugMode; // ��ʾ������Ϣ
                    graphElement.Paint(g);

                    if (graphElement is ConnectorContainer && graphElement.Visible)
                    {
                        textLineList.Add(graphElement as ConnectorContainer);
                    }
                }
            }

            // �����ߵ��ı���������
            foreach (ConnectorContainer line in textLineList)
            {
                if (line.ShowText)
                {
                    line.PaintText(g);
                }
            }
        }

        /// <summary>
        /// ���Ʊ���
        /// </summary>
        /// <param name="e">�����¼�����</param>
        protected override void OnPaintBackground(PaintEventArgs e)
        {
            base.OnPaintBackground(e);

            // ��ȡ��ͼ����
            Graphics g = e.Graphics;

            // ���û�ͼ����
            g.SmoothingMode = SmoothingMode.HighSpeed;
            g.PixelOffsetMode = PixelOffsetMode.HighSpeed;
            g.TranslateTransform(this.AutoScrollPosition.X, this.AutoScrollPosition.Y);

            if (background != null)
            {                                
                int gridWidth = background.GridSize.Width;
                int gridHeight = background.GridSize.Height;
                int canvasWidth = this.background.CanvasSize.Width;
                int canvasHeight = this.background.CanvasSize.Height;

                // ��ǰ������ڵ�
                currentPoint = PointToClient(Cursor.Position) - (Size)AutoScrollPosition;

                // ���㵱ǰ��������    
                visibleRectangle.Location = ClientRectangle.Location - (Size)AutoScrollPosition;
                visibleRectangle.Size = ClientSize;                

                int leftX = (int)Math.Floor((float)visibleRectangle.Left / gridWidth) * gridWidth;
                int topY = (int)Math.Floor((float)visibleRectangle.Top / gridHeight) * gridHeight;

                if (canvasWidth < visibleRectangle.Size.Width
                    || canvasHeight < visibleRectangle.Size.Height)
                {
                    for (int i = 0; i < canvasWidth; i += gridWidth)
                    {
                        g.DrawLine(graphSetting.GridLinePen, i, 0, i, canvasHeight);
                    }

                    for (int i = 0; i < canvasHeight; i += gridHeight)
                    {
                        g.DrawLine(graphSetting.GridLinePen, 0, i, canvasWidth, i);
                    } 

                    // ���Ʊ߿�
                    g.DrawLine(graphSetting.LineNormalPen, 0, 0, canvasWidth, 0);
                    g.DrawLine(graphSetting.LineNormalPen, 0, 0, 0, canvasHeight);
                    g.DrawLine(graphSetting.LineNormalPen, canvasWidth, 0, canvasWidth, canvasHeight);
                    g.DrawLine(graphSetting.LineNormalPen, 0, canvasHeight, canvasWidth, canvasHeight);
                }
                else
                {
                    for (int i = leftX; i < visibleRectangle.Right; i += gridWidth)
                    {
                        g.DrawLine(graphSetting.GridLinePen, i, visibleRectangle.Top, i, visibleRectangle.Bottom);
                    }

                    for (int i = topY; i < visibleRectangle.Bottom; i += gridHeight)
                    {
                        g.DrawLine(graphSetting.GridLinePen, visibleRectangle.Left, i, visibleRectangle.Right, i);
                    } 
                }            
            }
        }
    }
}
