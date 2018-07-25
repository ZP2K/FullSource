using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using DevComponents.DotNetBar;
using GraphElementLibrary;

namespace FlowChartEditor
{
    public class AbbreviateCanvas:Panel
    {
        private Point visibleRegionLocation; // ���������λ��
        private Color frameColor = Color.RoyalBlue; // ����������ͨ״̬����ɫ
        private Color selectedColor = Color.Red; // ��������ѡ��״̬����ɫ
        private Color activatedColor = Color.DarkOrange; // �������򱻼���״̬����ɫ
        private bool selected = false; // ���������Ƿ�ѡ��
        private bool activated = false; // ���������Ƿ񱻼���
        private bool tracking = false; // �Ƿ������϶���������
        private bool paintCanvas = false; // �Ƿ���ƻ�ͼ��
        private Point lastMovePoint; // ��һ���ƶ���λ��
        private Point regionLocation = Point.Empty; // ���Ƶĵ�ǰ�����λ��
        private Size regionSize = Size.Empty; // ���Ƶĵ�ǰ����Ĵ�С
        private Canvas currentCanvas; // ��ǰ��Ӧ�Ļ�ͼ��
        private Point tooltipLocation; // ��ʾ���λ��
        private StringFormat stringFormat = new StringFormat(); // �����ʽ����
        private float horizontalScale; // ��ͼ��ˮƽ���Ų���
        private float verticalScale; // ��ͼ�崹ֱ���Ų���
        private float wlRate; // ��ͼ���߱�
        private Size validOffset = new Size(0, 0); // ��Ч����ƫ��
        private Point validRegionLocaiton = new Point(0, 0); // ��Ч����λ��

        Brush frameBrush = Brushes.Black;
        Brush backgroundBrush = Brushes.LightYellow;
        Brush textBrush = Brushes.Black;
        Pen framePen = new Pen(Color.RoyalBlue); // �߿򻭱�

        private Rectangle validRegion = Rectangle.Empty; // ��Ч��ͼ����

        protected GraphSetting graphSetting = GraphSetting.GetGraphSetting(); // ��ͼ�������ö���

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
            stringFormat.Alignment = StringAlignment.Center;
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
        /// ���Ʒ���
        /// </summary>
        /// <param name="e">���Ʋ���</param>
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (paintCanvas && Visible) // ����Ƿ���Ի��ƻ�ͼ��
            {               
                // ��ȡ��ͼ����
                Graphics g = e.Graphics;            
                // ���û�ͼ����
                g.PixelOffsetMode = PixelOffsetMode.HighSpeed;                

                Helper helper = Helper.GetHelper();                

                if (graphSetting.CanvasScale != 1.0f) // ��ͼ��������״̬
                {
                    SizeF textSize = g.MeasureString("������״̬���޷��鿴����ͼ", graphSetting.GraphElementTextFont);
                    int textX = (int)((this.Size.Width - textSize.Width) / 2);
                    int textY = (int)((this.Size.Height - textSize.Height) / 2);
                    g.DrawString("������״̬���޷��鿴����ͼ", graphSetting.GraphElementTextFont, Brushes.Black, textX, textY);                    
                }
                else
                {
                    // �������ǰ��Ч����                          
                    horizontalScale = (float)this.Size.Width / currentCanvas.AutoScrollMinSize.Width;
                    verticalScale = (float)this.Size.Height / currentCanvas.AutoScrollMinSize.Height;
                    wlRate = (float)currentCanvas.AutoScrollMinSize.Width / currentCanvas.AutoScrollMinSize.Height;                    

                    if (horizontalScale < verticalScale) // ˮƽ����
                    {
                        validRegion.Width = this.Size.Width;
                        validRegion.Height = (int)(this.Size.Width / wlRate);
                        validRegionLocaiton.X = 0;
                        validRegionLocaiton.Y = (int)((this.Size.Height - validRegion.Height) / 2);
                        validRegion.Location = validRegionLocaiton;
                    }
                    else // ��ֱ����
                    {
                        validRegion.Height = this.Size.Height;
                        validRegion.Width = (int)(this.Size.Height * wlRate);
                        validRegionLocaiton.X = (int)((this.Size.Width - validRegion.Width) / 2);
                        validRegionLocaiton.Y = 0;
                        validRegion.Location = validRegionLocaiton;
                    }

                    horizontalScale = (float)validRegion.Width / currentCanvas.AutoScrollMinSize.Width;
                    verticalScale = (float)validRegion.Height / currentCanvas.AutoScrollMinSize.Height;

                    validOffset.Width = (int)((this.Size.Width - validRegion.Width) / (2 * horizontalScale));
                    validOffset.Height = (int)((this.Size.Height - validRegion.Height) / (2 * verticalScale));                    

                    regionLocation.X = -currentCanvas.AutoScrollPosition.X;
                    regionLocation.Y = -currentCanvas.AutoScrollPosition.Y;
                    regionSize.Width = currentCanvas.ClientSize.Width;
                    regionSize.Height = currentCanvas.ClientSize.Height;

                    // ����ת��
                    g.ScaleTransform(horizontalScale, verticalScale);

                    // ����ƽ��
                    g.TranslateTransform(validOffset.Width, validOffset.Height);

                    // ����ͼԪ                    
                    foreach (GraphElement graphElement in currentCanvas.GraphElementList)
                    {
                        graphElement.AbbreviativePaint(g);
                    }                    
                                                            
                    // ���ƿ�������
                    Color penColor = frameColor;
                    
                    if(activated)
                    {
                        penColor = activatedColor;
                    }
                    else if(selected)
                    {
                        penColor = selectedColor;
                    }

                    framePen.Color = penColor;                                                           
                    Rectangle regionRectangle = new Rectangle(regionLocation, currentCanvas.ClientSize);
                    g.DrawRectangle(framePen, regionRectangle);

                    // ������ʾ�ı�
                    if (activated)
                    {   
                        // �ָ�ת���������
                        g.TranslateTransform(-validOffset.Width, -validOffset.Height);
                        g.ScaleTransform(1 / horizontalScale, 1 / verticalScale);

                        SizeF tooltipSize = g.MeasureString("����϶��ƶ���ǰ��ͼ����", graphSetting.GraphElementTextFont);
                        int textX = tooltipLocation.X + 5;
                        int textY = tooltipLocation.Y + 5;
                        int width = (int)(tooltipSize.Width + 10);
                        int height = (int)(tooltipSize.Height + 10);
                        Rectangle toolTipRectangle = new Rectangle(tooltipLocation, new Size(width, height));                        

                        g.FillRectangle(backgroundBrush, toolTipRectangle);
                        g.DrawRectangle(new Pen(Color.Black), toolTipRectangle);
                        g.DrawString("����϶��ƶ���ǰ��ͼ����", graphSetting.GraphElementTextFont, textBrush, new PointF(textX, textY));
                    } 
                }                                                               
            }            
        }

        /// <summary>
        /// ���Ʊ���
        /// </summary>
        /// <param name="e">���Ʋ���</param>
        protected override void OnPaintBackground(PaintEventArgs e)
        {
            base.OnPaintBackground(e);

            // ��ȡ��ͼ����
            Graphics g = e.Graphics;
            g.Clear(Color.Gray);
            g.FillRectangle(Brushes.White, validRegion);
        }

        /// <summary>
        /// ��ǰ���Ƿ��ڿ���������
        /// </summary>
        /// <param name="p">��ǰ��λ��</param>
        /// <returns>�Ƿ��ڿ���������</returns>
        private bool IsInRegion(Point p)
        {                       
            Rectangle regionRectangle = new Rectangle(new Point(-currentCanvas.AutoScrollPosition.X, -currentCanvas.AutoScrollPosition.Y), 
                currentCanvas.ClientSize);
            // ��ƽ���ٷ���
            Point point = new Point(p.X - validOffset.Width, p.Y - validOffset.Height);            
            point.X = (int)(p.X / horizontalScale);
            point.Y = (int)(p.Y / verticalScale);
            bool result = regionRectangle.Contains(point);

            return result;
        }

        /// <summary>
        /// �ƶ���������
        /// </summary>
        /// <param name="p"></param>
        public void MoveRegion(Point p)
        {
            activated = false;

            if (currentCanvas != null && (graphSetting.CanvasScale == 1.0f))
            {
                if (tracking) // ���ӻ�ͼ����
                {
                    int horizontalValue = (int)((p.X - lastMovePoint.X) / horizontalScale);
                    int verticalValue = (int)((p.Y - lastMovePoint.Y) / verticalScale);                                                   
                    currentCanvas.Visible = false; // ������˸

                    if (regionLocation.X + horizontalValue >= 0 &&
                        regionLocation.X + regionSize.Width + horizontalValue <= currentCanvas.AutoScrollMinSize.Width &&
                        currentCanvas.HorizontalScroll.Value + horizontalValue >= currentCanvas.HorizontalScroll.Minimum &&
                        currentCanvas.HorizontalScroll.Value + horizontalValue <= currentCanvas.HorizontalScroll.Maximum)
                    {
                        currentCanvas.HorizontalScroll.Value += horizontalValue;
                    }
                    else
                    {
                        if (horizontalValue > 0) // �����ƶ���������
                        {
                            currentCanvas.HorizontalScroll.Value = currentCanvas.HorizontalScroll.Maximum;
                        }
                        else // �����ƶ���������
                        {
                            currentCanvas.HorizontalScroll.Value = currentCanvas.HorizontalScroll.Minimum;
                        }
                    }

                    if (regionLocation.Y + verticalValue >= 0 &&
                        regionLocation.Y + regionSize.Height + verticalValue <= currentCanvas.AutoScrollMinSize.Height &&
                        currentCanvas.VerticalScroll.Value + verticalValue >= currentCanvas.VerticalScroll.Minimum &&
                        currentCanvas.VerticalScroll.Value + verticalValue <= currentCanvas.VerticalScroll.Maximum)
                    {
                        currentCanvas.VerticalScroll.Value += verticalValue;
                    }
                    else
                    {
                        if (verticalValue > 0) // �����ƶ���������
                        {
                            currentCanvas.VerticalScroll.Value = currentCanvas.VerticalScroll.Maximum;
                        }
                        else // �����ƶ���������
                        {
                            currentCanvas.VerticalScroll.Value = currentCanvas.VerticalScroll.Minimum;
                        }
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

                    Invalidate();
                }
            }    
        }

        /// <summary>
        /// �л����������λ��
        /// </summary>
        /// <param name="p">��ǰ��λ��</param>
        private void ChangeVisibleRegionLocation(Point p)
        {
            if (currentCanvas != null && (graphSetting.CanvasScale == 1.0f))
            {
                int horizontalValue = (int)((p.X - validRegion.X) / horizontalScale - regionLocation.X - regionSize.Width / 2);
                int verticalValue = (int)((p.Y - validRegion.Y) / verticalScale - regionLocation.Y - regionSize.Height / 2);
                currentCanvas.Visible = false; // ������˸

                if (regionLocation.X + horizontalValue >= 0 &&
                    regionLocation.X + regionSize.Width + horizontalValue <= currentCanvas.AutoScrollMinSize.Width &&
                    currentCanvas.HorizontalScroll.Value + horizontalValue >= currentCanvas.HorizontalScroll.Minimum &&
                    currentCanvas.HorizontalScroll.Value + horizontalValue <= currentCanvas.HorizontalScroll.Maximum)
                {
                    currentCanvas.HorizontalScroll.Value += horizontalValue;
                }
                else
                {
                    if (horizontalValue > 0) // �����ƶ���������
                    {
                        currentCanvas.HorizontalScroll.Value = currentCanvas.HorizontalScroll.Maximum;
                    }
                    else // �����ƶ���������
                    {
                        currentCanvas.HorizontalScroll.Value = currentCanvas.HorizontalScroll.Minimum;
                    }
                }

                if (regionLocation.Y + verticalValue >= 0 &&
                    regionLocation.Y + regionSize.Height + verticalValue <= currentCanvas.AutoScrollMinSize.Height &&
                    currentCanvas.VerticalScroll.Value + verticalValue >= currentCanvas.VerticalScroll.Minimum &&
                    currentCanvas.VerticalScroll.Value + verticalValue <= currentCanvas.VerticalScroll.Maximum)
                {
                    currentCanvas.VerticalScroll.Value += verticalValue;
                }
                else
                {
                    if (verticalValue > 0) // �����ƶ���������
                    {
                        currentCanvas.VerticalScroll.Value = currentCanvas.VerticalScroll.Maximum;
                    }
                    else // �����ƶ���������
                    {
                        currentCanvas.VerticalScroll.Value = currentCanvas.VerticalScroll.Minimum;
                    }
                }

                selected = true;
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
            if(currentCanvas != null && (graphSetting.CanvasScale == 1.0f))
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
                    if (validRegion.Contains(p))
                    {
                        ChangeVisibleRegionLocation(p);
                    }                    
                }

                Invalidate();
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
