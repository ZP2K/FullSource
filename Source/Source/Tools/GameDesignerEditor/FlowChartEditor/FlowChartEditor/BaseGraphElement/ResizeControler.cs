using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.ComponentModel;
using GraphElementLibrary;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class ResizeControler:GraphElement
    {
        private Direction direction; // ���ſ��Ƶ�ķ���
        private SlotContainer slotContainer; // �����߿��Ƶ�������������

        [NonSerialized]
        private Rectangle textRectangle = Rectangle.Empty;
        [NonSerialized]
        private Rectangle activateRectangle = Rectangle.Empty;

        [NonSerialized]
        protected GraphSetting graphSetting; // ��ͼ�������ö���

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="line">���ſ��Ƶ������Ĳ������</param>
        /// <param name="location">���ſ��Ƶ��λ��</param>
        /// <param name="size">���ſ��Ƶ�Ĵ�С</param>
        public ResizeControler(SlotContainer slotContainer, Point location, Size size)
            : base(location)
        {
            this.slotContainer = slotContainer;
            this.location = location;
            this.elementSize = size;
        }

        /// <summary>
        /// ͼԪ��λ������
        /// </summary>
        [Browsable(false)]
        public override Point Location
        {
            set
            {
                location = value;
                textRectangle.Location = value + new Size(-(int)(elementSize.Width / 2), -(int)(elementSize.Height / 2));
                activateRectangle.Location = value + new Size(-elementSize.Width / 2, -elementSize.Height / 2);
            }

            get
            {
                return location;
            }
        }

        /// <summary>
        /// ͼԪ�Ƿ�ѡ������
        /// </summary>
        [Browsable(false)]
        public override bool Selected
        {
            set
            {
                selected = value;               
            }

            get
            {
                return selected;
            }
        }

        /// <summary>
        /// ͼԪ�Ƿ񱻼�������
        /// </summary>
        [Browsable(false)]
        public override bool Activated
        {
            set
            {
                activated = value;               
            }

            get
            {
                return activated;
            }
        }

        /// <summary>
        /// ͼԪ��ˢ����������
        /// </summary>
        [Browsable(false)]
        public override Rectangle InvalidRectangle
        {
            get
            {
                invalidRectangle = new Rectangle(location + new Size(-elementSize.Width, -elementSize.Height), 
                    new Size(elementSize.Width * 2, elementSize.Height * 2));

                return base.InvalidRectangle;
            }           
        }

        /// <summary>
        /// ���ſ��Ƶ�ķ�������
        /// </summary>
        [Browsable(false)]
        public Direction CurrentDirection
        {
            set
            {
                direction = value;
            }

            get
            {
                return direction;
            }
        }

        /// <summary>
        /// ���ſ��Ƶ������Ĳ����������
        /// </summary>
        [Browsable(false)]
        public SlotContainer Owner
        {
            get
            {
                return slotContainer;
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

            Rectangle rectangle = new Rectangle(new Point(location.X - elementSize.Width,
                                                location.Y - elementSize.Height), new Size(elementSize.Width * 2, elementSize.Height * 2));
            result = rectangle.Contains(p);

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

            Rectangle rectangle = new Rectangle(location, elementSize);
            result = r.Contains(rectangle);

            return result;
        }

        /// <summary>
        /// ˢ��ͼԪ���Ʋ���
        /// </summary>
        public override void Refresh()
        {
            // ��ʼ����ͼ���ݹ������
            graphSetting = GraphSetting.GetGraphSetting();

            // ˢ���߼�����
            textRectangle.Location = location + new Size(-(int)(elementSize.Width / 2), -(int)(elementSize.Height / 2));
            textRectangle.Size = elementSize;
            activateRectangle.Location = location + new Size(-elementSize.Width, -elementSize.Height);
            activateRectangle.Size = elementSize + elementSize;           
        }

        /// <summary>
        /// �ͷ�ͼԪ�Ļ�ͼ��Դ
        /// </summary>
        public override void Release()
        {
            
        }

        /// <summary>
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void Paint(Graphics g)
        {           
            if (visible)
            {
                // ���Ʊ߿�ɫ            
                if (activated) // ��ǰͼԪ������
                {
                    g.FillRectangle(graphSetting.GraphElementActivatedBrush, activateRectangle);
                }
                else if(selected) // ��ǰͼԪ��ѡ��
                {
                    g.FillRectangle(graphSetting.GraphElementSelectedBrush, activateRectangle);
                }
                else // ��ǰͼԪû�б�ѡ��
                {
                    g.FillRectangle(graphSetting.GraphElementNormalBrush, textRectangle);
                }
            }            
        }

        /// <summary>
        /// �������Ե�ͼԪ
        /// </summary>
        /// <param name="g">��ͼ����</param>
        public override void AbbreviativePaint(Graphics g)
        {
            
        }

        /// <summary>
        /// �ƶ�ͼԪ
        /// </summary>
        /// <param name="p">�ƶ������λ��</param>
        public override void Move(Size s)
        {
            if(selected) // ���ſ��Ƶ��ڱ�ѡ��״̬���ı�����Ӧ�Ĳ�������Ĵ�С
            {
                int newX;
                int newY;
                Size locationDistancement = new Size(0, 0);
                Size elementSizeDistancement = new Size(0, 0);
                bool resizeSlotContainer = false; // �Ƿ����Ų������

                switch (direction)
                {
                    case Direction.Up:
                        {
                            locationDistancement = new Size(0, s.Height);
                            elementSizeDistancement = new Size(0, -s.Height);  

                            // ��֤������ͼԪ�Ĵ�С��������С��С               
                            if((slotContainer.ElementSize.Width + elementSizeDistancement.Width >= slotContainer.MinSize.Width) &&
                                (slotContainer.ElementSize.Height + elementSizeDistancement.Height >= slotContainer.MinSize.Height))
                            {
                                newX = location.X;
                                newY = location.Y + s.Height;                            
                                location = new Point(newX, newY);
                                textRectangle.Location += new Size(0, s.Height);
                                activateRectangle.Location += new Size(0, s.Height);
                                resizeSlotContainer = true;
                            }                            

                            break;
                        }
                    case Direction.Up_Left:
                        {
                            locationDistancement = new Size(s.Width, s.Height);
                            elementSizeDistancement = new Size(-s.Width, -s.Height);

                            // ��֤������ͼԪ�Ĵ�С��������С��С               
                            if ((slotContainer.ElementSize.Width + elementSizeDistancement.Width >= slotContainer.MinSize.Width) &&
                                (slotContainer.ElementSize.Height + elementSizeDistancement.Height >= slotContainer.MinSize.Height))
                            {
                                newX = location.X + s.Width;
                                newY = location.Y + s.Height;                                
                                location = new Point(newX, newY);
                                textRectangle.Location += new Size(s.Width, s.Height);
                                activateRectangle.Location += new Size(s.Width, s.Height);
                                resizeSlotContainer = true;
                            }

                            break;
                        }
                    case Direction.Left:
                        {
                            locationDistancement = new Size(s.Width, 0);
                            elementSizeDistancement = new Size(-s.Width, 0);

                            // ��֤������ͼԪ�Ĵ�С��������С��С               
                            if ((slotContainer.ElementSize.Width + elementSizeDistancement.Width >= slotContainer.MinSize.Width) &&
                                (slotContainer.ElementSize.Height + elementSizeDistancement.Height >= slotContainer.MinSize.Height))
                            {
                                newX = location.X + s.Width;
                                newY = location.Y;                                
                                location = new Point(newX, newY);
                                textRectangle.Location += new Size(s.Width, 0);
                                activateRectangle.Location += new Size(s.Width, 0);
                                resizeSlotContainer = true;
                            }

                            break;
                        }
                    case Direction.Down_Left:
                        {
                            locationDistancement = new Size(s.Width, 0);
                            elementSizeDistancement = new Size(-s.Width, s.Height);

                            // ��֤������ͼԪ�Ĵ�С��������С��С               
                            if ((slotContainer.ElementSize.Width + elementSizeDistancement.Width >= slotContainer.MinSize.Width) &&
                                (slotContainer.ElementSize.Height + elementSizeDistancement.Height >= slotContainer.MinSize.Height))
                            {
                                newX = location.X + s.Width;
                                newY = location.Y + s.Height;                                
                                location = new Point(newX, newY);
                                textRectangle.Location += new Size(s.Width, s.Height);
                                activateRectangle.Location += new Size(s.Width, s.Height);
                                resizeSlotContainer = true;
                            }

                            break;
                        }
                    case Direction.Down:
                        {
                            elementSizeDistancement = new Size(0, s.Height);

                            // ��֤������ͼԪ�Ĵ�С��������С��С               
                            if ((slotContainer.ElementSize.Width + elementSizeDistancement.Width >= slotContainer.MinSize.Width) &&
                                (slotContainer.ElementSize.Height + elementSizeDistancement.Height >= slotContainer.MinSize.Height))
                            {
                                newX = location.X;
                                newY = location.Y + s.Height;                                
                                location = new Point(newX, newY);
                                textRectangle.Location += new Size(0, s.Height);
                                activateRectangle.Location += new Size(0, s.Height);
                                resizeSlotContainer = true;
                            }

                            break;
                        }
                    case Direction.Down_Right:
                        {
                            elementSizeDistancement = new Size(s.Width, s.Height);

                            // ��֤������ͼԪ�Ĵ�С��������С��С               
                            if ((slotContainer.ElementSize.Width + elementSizeDistancement.Width >= slotContainer.MinSize.Width) &&
                                (slotContainer.ElementSize.Height + elementSizeDistancement.Height >= slotContainer.MinSize.Height))
                            {
                                newX = location.X + s.Width;
                                newY = location.Y + s.Height;                                
                                location = new Point(newX, newY);
                                textRectangle.Location += new Size(s.Width, s.Height);
                                activateRectangle.Location += new Size(s.Width, s.Height);
                                resizeSlotContainer = true;
                            }

                            break;
                        }
                    case Direction.Right:
                        {
                            elementSizeDistancement = new Size(s.Width, 0);

                            // ��֤������ͼԪ�Ĵ�С��������С��С               
                            if ((slotContainer.ElementSize.Width + elementSizeDistancement.Width >= slotContainer.MinSize.Width) &&
                                (slotContainer.ElementSize.Height + elementSizeDistancement.Height >= slotContainer.MinSize.Height))
                            {
                                newX = location.X + s.Width;
                                newY = location.Y;                                
                                location = new Point(newX, newY);
                                textRectangle.Location += new Size(s.Width, 0);
                                activateRectangle.Location += new Size(s.Width, 0);
                                resizeSlotContainer = true;
                            }

                            break;
                        }
                    case Direction.Up_Right:
                        {
                            locationDistancement = new Size(0, s.Height);
                            elementSizeDistancement = new Size(s.Width, -s.Height);

                            // ��֤������ͼԪ�Ĵ�С��������С��С               
                            if ((slotContainer.ElementSize.Width + elementSizeDistancement.Width >= slotContainer.MinSize.Width) &&
                                (slotContainer.ElementSize.Height + elementSizeDistancement.Height >= slotContainer.MinSize.Height))
                            {
                                newX = location.X + s.Width;
                                newY = location.Y + s.Height;                                
                                location = new Point(newX, newY);
                                textRectangle.Location += new Size(s.Width, s.Height);
                                activateRectangle.Location += new Size(s.Width, s.Height);
                                resizeSlotContainer = true;
                            }

                            break;
                        }
                }

                // �������ſ��Ƶ��Ӧ�Ĳ�������Ĵ�С
                if(resizeSlotContainer)
                {
                    slotContainer.MoveBorder(locationDistancement, elementSizeDistancement);
                }                
            }
            else // �ڷ�ѡ��״̬�£���ֱ���ƶ����ſ��Ƶ㼴��
            {
                this.location += s;
                textRectangle.Location += s;
                activateRectangle.Location += s;
            }
        }
    }
}
