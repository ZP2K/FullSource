using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using GraphElementLibrary;
using DevComponents.DotNetBar;

namespace FlowChartEditor
{
    public class Canvas:Panel
    {
        private List<GraphElement> graphElementList = new List<GraphElement>(); // ͼԪ����
        private Background background; // ����

        private bool showAbbreviatGraphElement = false; // �Ƿ���ʾ���Ե�ͼԪ
        private GraphElement abbreviatGraphElement; // ���Ե�ͼԪ
        private GraphElement abbreviateLine; // ����ͼԪ��������

        private bool autoConnect = false; // �Ƿ��Զ�����ͼԪ

        private bool multiSelect = false; // �Ƿ��ڶ�ѡͼԪ�׶�
        private bool showMultiSelectRegion = false; // �Ƿ���ʾ��ѡͼԪ��ѡ������
        private Rectangle multiSelectRectangle; // ��ѡ���ƵĶ����
        private Rectangle multiSelectRegion; // ��ѡͼԪ��ѡ������

        private Bitmap canvasAbbreviateBitmap; // ��ͼ�������ͼ
        private bool refreshAbbreviateBitmap = true; // �Ƿ�ˢ�»�ͼ�������ͼ
        private AbbreviateCanvas abbreviateCanvas; // ����ͼ��ͼ��

        private Point tooltipImageLocation; // ��ʾͼƬ��λ��
        private Size tooltipImageSize; // ��ʾͼƬ�Ĵ�С
        private Image tooltipImage; // ��ʾͼƬ
        private bool showTooltipImage; // ��ʾ��ʾͼƬ

        private Rectangle tooltipTextRectangle; // ��ʾ�ı��ı�������
        private Point tooltipTextLocation; // ��ʾ�ı����ı�λ��

        private Rectangle visibleRectangle = Rectangle.Empty; // �����������
        private Rectangle tooltipImageRectangle = Rectangle.Empty; // ��ʾͼƬ�Ļ��ƾ���

        private bool firstShow = true; // �Ƿ��ǵ�һ����ʾ
        private bool paintCanvas = true; // �Ƿ�ˢ�»�ͼ��

        private bool showConflictRectangle = false; // �Ƿ���ʾ��ͻ����
        private Rectangle conflictRectangle; // ��ͻ����

        private Brush frameBrush = Brushes.Black;
        private Brush backgroundBrush;
        private Brush textBrush;
        private Pen multiSelectRegionPen;
        private Pen multiSelectPen;
        private Color multiSelectColor;
        private Brush multiSelectBrush;
        private Pen gridPen;

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
        /// �ͷŻ�ͼ��Ļ�ͼ��Դ
        /// </summary>
        public void Release()
        {
            backgroundBrush.Dispose();
            textBrush.Dispose();
            multiSelectRegionPen.Dispose();
            multiSelectPen.Dispose();
            multiSelectBrush.Dispose();
        }

        /// <summary>
        /// �����ͼ����
        /// </summary>
        public void ResetArgs()
        {
            backgroundBrush = new SolidBrush(background.TooltipBackgroundColor);
            textBrush = new SolidBrush(background.ToolTipColor);
            multiSelectRegionPen = new Pen(background.MultiSelectRegionColor);
            multiSelectRegionPen.DashStyle = DashStyle.Dash;
            multiSelectPen = new Pen(background.MultiSelectColor);
            multiSelectColor = Color.FromArgb(127, background.MultiSelectBackgroundColor);
            multiSelectBrush = new SolidBrush(multiSelectColor);
            gridPen = new Pen(background.GridColor);            
        }

        /// <summary>
        /// ˢ������ͼ��ͼ��
        /// </summary>
        public void RefreshAbbreviateCanvas()
        {
            if(abbreviateCanvas != null && abbreviateCanvas.Visible && refreshAbbreviateBitmap) // ��Ҫˢ������ͼ
            {
                abbreviateCanvas.PaintCanvas = false; // ����ʹ���ͷŵ�����ͼƬ��Դ������
                AbbreviateCanvas tempAbbreviateCanvas = abbreviateCanvas;
                abbreviateCanvas = null;

                if (canvasAbbreviateBitmap != null)
                {
                    canvasAbbreviateBitmap.Dispose(); // �ͷ�ͼƬ��Դ
                }
                canvasAbbreviateBitmap = new Bitmap(background.CanvasSize.Width, background.CanvasSize.Height);
                Graphics g = Graphics.FromImage(canvasAbbreviateBitmap);
                g.Clear(Color.White);

                // ����ͼԪ
                foreach (GraphElement graphElement in graphElementList)
                {
                    graphElement.AbbreviativePaint(g);
                }

                refreshAbbreviateBitmap = false;
                g.Dispose(); // �ͷŻ�ͼ��Դ
                tempAbbreviateCanvas.AbbreciateImage = canvasAbbreviateBitmap;
                tempAbbreviateCanvas.Invalidate();
                abbreviateCanvas = tempAbbreviateCanvas;
                abbreviateCanvas.PaintCanvas = true; // ����ʹ���ͷŵ�����ͼƬ��Դ������
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
            }

            get
            {
                return paintCanvas;
            }
        }

        /// <summary>
        /// �Ƿ���ʾ��ͻ����
        /// </summary>
        public bool ShowConflictRectangle
        {
            set
            {
                showConflictRectangle = value;
            }
        }

        /// <summary>
        /// ��ͻ����
        /// </summary>
        public Rectangle ConflictRectangle
        {
            set
            {
                conflictRectangle = value;
            }
        }

        /// <summary>
        /// ��ʾ�ı��ı�����������
        /// </summary>
        public Rectangle TooltipTextRectangle
        {
            set
            {
                tooltipTextRectangle = value;
            }

            get
            {
                return tooltipTextRectangle;
            }
        }

        /// <summary>
        /// ��ʾ�ı�������λ��
        /// </summary>
        public Point TooltipTextLocation
        {
            set
            {
                tooltipTextLocation = value;
            }

            get
            {
                return tooltipTextLocation;
            }
        }

        /// <summary>
        /// �Ƿ���ʾ��ѡͼԪ��ѡ������
        /// </summary>
        public bool ShowMultiSelectRegion
        {
            set
            {
                showMultiSelectRegion = value;
            }
        }

        /// <summary>
        /// ��ѡͼԪ��ѡ����������
        /// </summary>
        public Rectangle MultiSelectRegion
        {
            set
            {
                multiSelectRegion = value;
            }

            get
            {
                return multiSelectRegion;
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
        /// �Ƿ�ˢ�»�ͼ�������ͼ����
        /// </summary>
        public bool RefreshAbbreviateBitmap
        {
            set
            {
                refreshAbbreviateBitmap = value;
            }

            get
            {
                return refreshAbbreviateBitmap;
            }
        }

        /// <summary>
        /// �Ƿ��ڶ�ѡͼԪ�׶�
        /// </summary>
        public bool MultiSelect
        {
            set
            {
                multiSelect = value;
            }
        }

        /// <summary>
        /// ��ѡ���ƵĶ����
        /// </summary>
        public Rectangle MultiSelectRectangle
        {
            set
            {
                multiSelectRectangle = value;
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
                if (firstShow)
                {
                    refreshAbbreviateBitmap = true;
                    abbreviateCanvas.AbbreciateImage = GetBitMap();
                    abbreviateCanvas.VisibleRegionLocation = this.AutoScrollPosition;
                    abbreviateCanvas.VisibleRegionSize = this.ClientSize;
                    abbreviateCanvas.EntireRegionSize = background.CanvasSize;
                    abbreviateCanvas.Invalidate();
                    abbreviateCanvas.PaintCanvas = true;
                    firstShow = false;
                }
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
                ResetArgs(); // �����ͼ����
            }
        }

        /// <summary>
        /// �Ƿ���ʾ���Ե�ͼԪ����
        /// </summary>
        public bool ShowAbbreviatGraphElement
        {
            set
            {
                showAbbreviatGraphElement = value;
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
        }

        /// <summary>
        /// �Ƿ��Զ���������ͼԪ����
        /// </summary>
        public bool AutoConnect
        {
            set
            {
                autoConnect = value;
            }
        }

        /// <summary>
        /// ���Ƶķ���
        /// </summary>
        /// <param name="e">�����¼�����</param>
        protected override void  OnPaint(PaintEventArgs e)
        {
 	        base.OnPaint(e);

            if(paintCanvas) // ���ƻ�ͼ��
            {
                // ��ȡ��ͼ����
                Graphics g = e.Graphics;
                visibleRectangle.Location = ClientRectangle.Location - new Size(AutoScrollPosition);
                visibleRectangle.Size = ClientSize;

                // ���û�ͼ����
                g.SmoothingMode = SmoothingMode.HighQuality;
                g.PixelOffsetMode = PixelOffsetMode.HighSpeed;
                g.TextRenderingHint = TextRenderingHint.ClearTypeGridFit; // ����ı���������
                g.TranslateTransform(this.AutoScrollPosition.X, this.AutoScrollPosition.Y);

                // ��������ͼԪ��������
                if(showAbbreviatGraphElement && autoConnect)
                {
                    abbreviateLine.AbbreviativePaint(g);
                }

                // ����ͼԪ
                foreach(GraphElement graphElement in graphElementList)
                {                
                    Rectangle rectangle = new Rectangle(graphElement.Location, graphElement.ElementSize);
                    if(graphElement.Visible && (visibleRectangle.Contains(rectangle) || visibleRectangle.IntersectsWith(rectangle))) // ��ͼԪ�ڿ����������ٽ��л���
                    {
                        graphElement.Paint(g); 
                    }  
                }

                // �������Ե�ͼԪ
                if(showAbbreviatGraphElement)
                {                
                    abbreviatGraphElement.Paint(g);
                }

                // ���ƶ�ѡ��
                if(multiSelect)
                {
                    g.FillRectangle(multiSelectBrush, multiSelectRectangle);
                    g.DrawRectangle(multiSelectPen, multiSelectRectangle);
                }

                // ���ƶ�ѡͼԪ��ѡ����
                if(showMultiSelectRegion)
                {
                    g.DrawRectangle(multiSelectRegionPen, multiSelectRegion);
                }
                
                // ��������ͼ�ؼ�
                if(abbreviateCanvas != null && abbreviateCanvas.Visible)
                {
                    abbreviateCanvas.AbbreciateImage = GetBitMap();
                    abbreviateCanvas.VisibleRegionLocation = this.AutoScrollPosition;
                    abbreviateCanvas.VisibleRegionSize = this.ClientSize;
                    abbreviateCanvas.EntireRegionSize = background.CanvasSize;
                    abbreviateCanvas.Invalidate();
                }

                // ���Ƴ�ͻ����
                if(showConflictRectangle)
                {
                    g.FillRectangle(multiSelectBrush, conflictRectangle);
                    showConflictRectangle = false;
                }

                // ������ʾ��ͼƬ
                if(showTooltipImage)
                {
                    g.DrawImage(tooltipImage, tooltipImageRectangle);
                    showTooltipImage = false; // ֻ��ʾһ��
                }

                // ������ʾ�ı�
                if(background.ShowTooltipText)
                {                
                    g.FillRectangle(backgroundBrush, tooltipTextRectangle);
                    g.DrawRectangle(new Pen(Color.Black), tooltipTextRectangle);
                    g.DrawString(background.TooltipText, background.TooltipFont, textBrush, tooltipTextLocation);
                }
            }            
        }

        /// <summary>
        /// ��ʾ��ʾͼƬ
        /// </summary>
        /// <param name="image">��ʾͼƬ</param>
        /// <param name="imageLocation">ͼƬ��λ��</param>
        /// <param name="imageSize">ͼƬ�Ĵ�С</param>
        public void SetTooltipImage(Image image, Point imageLocation, Size imageSize)
        {
            tooltipImage = image;
            tooltipImageLocation = imageLocation;
            tooltipImageSize = imageSize;
            tooltipImageRectangle.Location = imageLocation;
            tooltipImageRectangle.Size = imageSize;
            showTooltipImage = true;
        }

        /// <summary>
        /// ��ȡ��ǰ��ͼ�������λͼ
        /// </summary>
        /// <returns>��ǰ��ͼ�������λͼ</returns>
        private Bitmap GetBitMap()
        {
            Bitmap bmp;

            if(refreshAbbreviateBitmap && !Helper.GetHelper().AutoRefreshAbbreviateCanvas) // ��Ҫˢ�»�ͼ������ͼ
            {                
                bmp = new Bitmap(background.CanvasSize.Width, background.CanvasSize.Height);
                Graphics g = Graphics.FromImage(bmp);
                g.Clear(Color.White);

                // ����ͼԪ
                foreach (GraphElement graphElement in graphElementList)
                {
                    graphElement.AbbreviativePaint(g);
                }

                if (canvasAbbreviateBitmap != null)
                {
                    canvasAbbreviateBitmap.Dispose(); // �ͷ�ͼƬ��Դ
                }
                canvasAbbreviateBitmap = bmp;
                refreshAbbreviateBitmap = false;
                g.Dispose(); // �ͷŻ�ͼ��Դ
            }
            else
            {
                bmp = canvasAbbreviateBitmap;
            }

            return bmp;
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
            g.SmoothingMode = SmoothingMode.HighQuality;
            g.PixelOffsetMode = PixelOffsetMode.HighSpeed;
            g.TranslateTransform(this.AutoScrollPosition.X, this.AutoScrollPosition.Y);

            if(background != null)
            {
                int gridWidth = background.GridSize.Width;
                int gridHeight = background.GridSize.Height;
                int canvasWidth = this.background.CanvasSize.Width;
                int canvasHeight = this.background.CanvasSize.Height;

                for (int i = 0; i <= canvasWidth; i += gridWidth)
                {
                    g.DrawLine(gridPen, i, 0, i, canvasHeight);
                }

                for (int i = 0; i <= canvasHeight; i +=gridHeight)
                {
                    g.DrawLine(gridPen, 0, i, canvasWidth, i);
                }
            }
        }
    }
}
