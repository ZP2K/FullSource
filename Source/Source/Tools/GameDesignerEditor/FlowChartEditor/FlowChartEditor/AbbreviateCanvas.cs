using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using DevComponents.DotNetBar;

namespace FlowChartEditor
{
    public class AbbreviateCanvas:Panel
    {
        private Image abbreviateImage; // ����ͼ
        private Size visibleRegionSize; // ��������Ĵ�С
        private Size entireRegionSize; // ��������Ĵ�С
        private Point visibleRegionLocation; // ���������λ��
        private Color frameColor = Color.RoyalBlue; // ����������ͨ״̬����ɫ
        private Color selectedColor = Color.Red; // ��������ѡ��״̬����ɫ
        private Color activatedColor = Color.DarkOrange; // �������򱻼���״̬����ɫ
        private bool selected = false; // ���������Ƿ�ѡ��
        private bool activated = false; // ���������Ƿ񱻼���
        private bool tracking = false; // �Ƿ������϶���������
        private bool paintCanvas = false; // �Ƿ���ƻ�ͼ��
        private Point lastMovePoint; // ��һ���ƶ���λ��
        private Point regionLocation; // ���Ƶĵ�ǰ�����λ��
        private Size regionSize; // ���Ƶĵ�ǰ����Ĵ�С
        private Canvas currentCanvas; // ��ǰ��Ӧ�Ļ�ͼ��
        private Point tooltipLocation; // ��ʾ���λ��

        /// <summary>
        /// ���캯��
        /// </summary>
        public AbbreviateCanvas()
        {
            // ����˫����Ȼ��Ʋ���
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.ResizeRedraw, true);
        }

        /// <summary>
        /// �Ƿ���ƻ�ͼ��
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
        /// ��ǰ��Ӧ�Ļ�ͼ������
        /// </summary>
        public Canvas CurrentCanvas
        {
            set
            {
                currentCanvas = value;
            }
        }
        
        /// <summary>
        /// ��������Ĵ�С����
        /// </summary>
        public Size EntireRegionSize
        {
            set
            {
                entireRegionSize = value;
            }
        }

        /// <summary>
        /// ���������λ������
        /// </summary>
        public Point VisibleRegionLocation
        {
            set
            {
                visibleRegionLocation = value;                
            }
        }

        /// <summary>
        /// ��������Ĵ�С����
        /// </summary>
        public Size VisibleRegionSize
        {
            set
            {
                visibleRegionSize = value;
            }
        }

        /// <summary>
        /// ����ͼ����
        /// </summary>
        public Image AbbreciateImage
        {
            set
            {
                abbreviateImage = value;
            }
        }

        /// <summary>
        /// ���Ʒ���
        /// </summary>
        /// <param name="e">���Ʋ���</param>
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if(paintCanvas && abbreviateImage != null) // ����Ƿ���Ի��ƻ�ͼ��
            {               
                // ��ȡ��ͼ����
                Graphics g = e.Graphics;            
                // ���û�ͼ����
                g.PixelOffsetMode = PixelOffsetMode.HighSpeed;

                Rectangle imageRectangle = new Rectangle(new Point(0, 0), this.Size);
                g.DrawImage(abbreviateImage, imageRectangle);

                // ���ƿ�������
                Color penColor = frameColor;
                if(selected)
                {
                    penColor = selectedColor;
                }
                else if(activated)
                {
                    penColor = activatedColor;
                }
                Pen framePen = new Pen(penColor);
                int regionWidth = (int)(visibleRegionSize.Width * this.Size.Width / entireRegionSize.Width);
                int regionHeight = (int)(visibleRegionSize.Height * this.Size.Height / entireRegionSize.Height);
                int regionX = (int)(-visibleRegionLocation.X * this.Size.Width / entireRegionSize.Width);
                int regionY = (int)(-visibleRegionLocation.Y * this.Size.Height / entireRegionSize.Height);
                if(!tracking) // �϶�ʱ�Ѿ��Զ�����λ��
                {
                    regionLocation = new Point(regionX, regionY);
                }                                
                regionSize = new Size(regionWidth, regionHeight);
                Rectangle regionRectangle = new Rectangle(regionLocation, regionSize);
                g.DrawRectangle(framePen, regionRectangle);

                // ������ʾ�ı�
                if(activated)
                {
                    Helper helper = Helper.GetHelper();
                    SizeF tooltipSize = g.MeasureString("����϶��ƶ���ǰ��ͼ����", helper.TooltipFont);
                    int textX = tooltipLocation.X + 5;
                    int textY = tooltipLocation.Y + 5;
                    int width = (int)(tooltipSize.Width + 10);
                    int height = (int)(tooltipSize.Height + 10);
                    Rectangle toolTipRectangle = new Rectangle(tooltipLocation, new Size(width, height));

                    Brush frameBrush = Brushes.Black;
                    Brush backgroundBrush = Brushes.LightYellow;
                    Brush textBrush = Brushes.Black;

                    g.FillRectangle(backgroundBrush, toolTipRectangle);
                    g.DrawRectangle(new Pen(Color.Black), toolTipRectangle);
                    g.DrawString("����϶��ƶ���ǰ��ͼ����", helper.TooltipFont, textBrush, new PointF(textX, textY));
                }                                                
            }            
        }

        /// <summary>
        /// ��ǰ���Ƿ��ڿ���������
        /// </summary>
        /// <param name="p">��ǰ��λ��</param>
        /// <returns>�Ƿ��ڿ���������</returns>
        private bool IsInRegion(Point p)
        {
            int regionWidth = (int)(visibleRegionSize.Width * this.Size.Width / entireRegionSize.Width);
            int regionHeight = (int)(visibleRegionSize.Height * this.Size.Height / entireRegionSize.Height);
            int regionX = (int)(-visibleRegionLocation.X * this.Size.Width / entireRegionSize.Width);
            int regionY = (int)(-visibleRegionLocation.Y * this.Size.Height / entireRegionSize.Height);
            Rectangle regionRectangle = new Rectangle(new Point(regionX, regionY), new Size(regionWidth, regionHeight));
            bool result = regionRectangle.Contains(p);

            return result;
        }

        /// <summary>
        /// �ƶ���������
        /// </summary>
        /// <param name="p"></param>
        public void MoveRegion(Point p)
        {
            activated = false;

            if (currentCanvas != null)
            {
                if (tracking) // ���ӻ�ͼ����
                {

                    int horizontalValue = p.X - lastMovePoint.X;
                    int verticalValue = p.Y - lastMovePoint.Y;
                    int horizontalScrollValue = (int)(horizontalValue * entireRegionSize.Width / this.Size.Width);
                    int verticalScrollValue = (int)(verticalValue * entireRegionSize.Height / this.Size.Height);
                    bool changeLocation = false; // �Ƿ�ı��������λ��                               

                    currentCanvas.Visible = false; // ������˸

                    if (regionLocation.X + horizontalValue >= 0 &&
                        regionLocation.X + regionSize.Width + horizontalValue <= this.Size.Width &&
                        currentCanvas.HorizontalScroll.Value + horizontalScrollValue >= currentCanvas.HorizontalScroll.Minimum &&
                        currentCanvas.HorizontalScroll.Value + horizontalScrollValue <= currentCanvas.HorizontalScroll.Maximum)
                    {
                        currentCanvas.HorizontalScroll.Value += horizontalScrollValue;
                        changeLocation = true;
                    }
                    else
                    {
                        if (horizontalValue > 0) // �����ƶ���������
                        {
                            horizontalValue = this.Size.Width - regionSize.Width - regionLocation.X;
                            horizontalScrollValue = (int)(horizontalValue * entireRegionSize.Width / this.Size.Width);
                            currentCanvas.HorizontalScroll.Value = currentCanvas.HorizontalScroll.Maximum;
                        }
                        else // �����ƶ���������
                        {
                            horizontalValue = -regionLocation.X;
                            horizontalScrollValue = (int)(horizontalValue * entireRegionSize.Width / this.Size.Width);
                            currentCanvas.HorizontalScroll.Value = currentCanvas.HorizontalScroll.Minimum;
                        }
                    }

                    if (regionLocation.Y + verticalValue >= 0 &&
                        regionLocation.Y + regionSize.Height + verticalValue <= this.Size.Height &&
                        currentCanvas.VerticalScroll.Value + verticalScrollValue >= currentCanvas.VerticalScroll.Minimum &&
                        currentCanvas.VerticalScroll.Value + verticalScrollValue <= currentCanvas.VerticalScroll.Maximum)
                    {
                        currentCanvas.VerticalScroll.Value += verticalScrollValue;
                        changeLocation = true;
                    }
                    else
                    {
                        if (verticalValue > 0) // �����ƶ���������
                        {
                            verticalValue = this.Size.Height - regionSize.Height - regionLocation.Y;
                            verticalScrollValue = (int)(verticalValue * entireRegionSize.Height / this.Size.Height);
                            currentCanvas.VerticalScroll.Value = currentCanvas.VerticalScroll.Maximum;
                        }
                        else // �����ƶ���������
                        {
                            verticalValue = -regionLocation.Y;
                            verticalScrollValue = (int)(verticalValue * entireRegionSize.Height / this.Size.Height);
                            currentCanvas.VerticalScroll.Value = currentCanvas.VerticalScroll.Minimum;
                        }
                    }

                    if (changeLocation)
                    {
                        regionLocation += new Size(horizontalValue, verticalValue);
                    }

                    lastMovePoint = p;
                    currentCanvas.Visible = true;
                    this.Refresh();
                    currentCanvas.Refresh();
                }
                else
                {
                    if (IsInRegion(p)) // �����������
                    {
                        activated = true;
                        tooltipLocation = p;
                    }

                    Invalidate(ClientRectangle);
                }
            }    
        }

        /// <summary>
        /// �л����������λ��
        /// </summary>
        /// <param name="p">��ǰ��λ��</param>
        private void ChangeVisibleRegionLocation(Point p)
        {
            if (currentCanvas != null)
            {
                int horizontalValue = p.X - regionLocation.X - (int)(regionSize.Width / 2);
                int verticalValue = p.Y - regionLocation.Y - (int)(regionSize.Height / 2);
                int horizontalScrollValue = (int)(horizontalValue * entireRegionSize.Width / this.Size.Width);
                int verticalScrollValue = (int)(verticalValue * entireRegionSize.Height / this.Size.Height);

                currentCanvas.Visible = false; // ������˸

                if (regionLocation.X + horizontalValue >= 0 &&
                    regionLocation.X + regionSize.Width + horizontalValue <= this.Size.Width &&
                    currentCanvas.HorizontalScroll.Value + horizontalScrollValue >= currentCanvas.HorizontalScroll.Minimum &&
                    currentCanvas.HorizontalScroll.Value + horizontalScrollValue <= currentCanvas.HorizontalScroll.Maximum)
                {                    
                    currentCanvas.HorizontalScroll.Value += horizontalScrollValue;
                }
                else
                {
                    if (horizontalValue > 0) // �����ƶ���������
                    {
                        horizontalValue = this.Size.Width - regionSize.Width - regionLocation.X;
                        horizontalScrollValue = (int)(horizontalValue * entireRegionSize.Width / this.Size.Width);
                        currentCanvas.HorizontalScroll.Value = currentCanvas.HorizontalScroll.Maximum;
                    }
                    else // �����ƶ���������
                    {
                        horizontalValue = -regionLocation.X;
                        horizontalScrollValue = (int)(horizontalValue * entireRegionSize.Width / this.Size.Width);
                        currentCanvas.HorizontalScroll.Value = currentCanvas.HorizontalScroll.Minimum;
                    }
                }

                if (regionLocation.Y + verticalValue >= 0 &&
                    regionLocation.Y + regionSize.Height + verticalValue <= this.Size.Height &&
                    currentCanvas.VerticalScroll.Value + verticalScrollValue >= currentCanvas.VerticalScroll.Minimum &&
                    currentCanvas.VerticalScroll.Value + verticalScrollValue <= currentCanvas.VerticalScroll.Maximum)
                {
                    currentCanvas.VerticalScroll.Value += verticalScrollValue;
                }
                else
                {
                    if (verticalValue > 0) // �����ƶ���������
                    {
                        verticalValue = this.Size.Height - regionSize.Height - regionLocation.Y;
                        verticalScrollValue = (int)(verticalValue * entireRegionSize.Height / this.Size.Height);
                        currentCanvas.VerticalScroll.Value = currentCanvas.VerticalScroll.Maximum;
                    }
                    else // �����ƶ���������
                    {
                        verticalValue = -regionLocation.Y;
                        verticalScrollValue = (int)(verticalValue * entireRegionSize.Height / this.Size.Height);
                        currentCanvas.VerticalScroll.Value = currentCanvas.VerticalScroll.Minimum;
                    }
                }
                
                selected = true;
                regionLocation += new Size(horizontalValue, verticalValue);
                lastMovePoint = p;
                currentCanvas.Visible = true;
                this.Invalidate();
                currentCanvas.Invalidate();
            }
        }

        /// <summary>
        /// ѡ�п�������
        /// </summary>
        /// <param name="p">��ǰ��λ��</param>
        public void SelectRegion(Point p)
        {
            if(currentCanvas != null)
            {
                if(IsInRegion(p))
                {
                    selected = true;
                    tracking = true;
                    lastMovePoint = p;
                }
                else
                {
                    selected = false;

                    // �ƶ���������
                    ChangeVisibleRegionLocation(p);
                }

                Invalidate(new Rectangle(regionLocation, regionSize));
            }            
        }

        /// <summary>
        /// �����ƶ�
        /// </summary>
        /// <param name="p">��ǰ��λ��</param>
        public void EndMove(Point p)
        {
            tracking = false;
        }

        /// <summary>
        /// �����ƶ�
        /// </summary>
        public void EndMove()
        {
            tracking = false;
            activated = false;
            selected = false;
        }
    }
}
