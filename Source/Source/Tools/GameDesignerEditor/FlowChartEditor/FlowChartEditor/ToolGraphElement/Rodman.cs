using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

using GraphElementLibrary;

namespace FlowChartEditor
{
    public class Rodman : GraphElement
    {
        private Rectangle borderRectangle; // �߿����
        private RodmanType rodmanType = RodmanType.None; // ��ǰ�α�����

        private GraphSetting graphSetting = GraphSetting.GetGraphSetting();

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪλ��</param>
        /// <param name="elementSize">ͼԪ��С</param>
        public Rodman(Point location, Size elementSize) : base(location)
        {
            this.elementSize = elementSize;
        }

        /// <summary>
        /// ��ǰ�α�����
        /// </summary>
        public RodmanType CurrentRodmanType
        {
            set
            {
                rodmanType = value;
            }

            get
            {
                return rodmanType;
            }
        }

        /// <summary>
        /// �߿����
        /// </summary>
        public Rectangle BorderRectangle
        {
            set
            {
                borderRectangle = value;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        /// <param name="location">λ��</param>
        /// <param name="borderSize">�߿����</param>
        public void Init(Point location, Rectangle borderRectangle)
        {
            this.location = location;
            this.borderRectangle = borderRectangle;
        }        

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="g">��ͼ����</param>
        public override void Paint(Graphics g)
        {
            // ����״̬�Ͳ�Ҫ��ʾ��
            if (visible && graphSetting.CanvasScale == 1.0f)
            {
                switch (rodmanType)
                {
                    case RodmanType.None:
                        {
                            break;
                        }
                    case RodmanType.Horizonal:
                        {
                            g.DrawLine(graphSetting.AdjustLinePen, location.X, borderRectangle.Top, location.X, borderRectangle.Bottom);
                            g.FillRectangle(graphSetting.RodmanBackgroundBrush, borderRectangle.Location.X, borderRectangle.Location.Y, borderRectangle.Width, (int)(elementSize.Width / 2));

                            if (activated)
                            {
                                g.FillRectangle(graphSetting.RodmanActivatedBrush, location.X - (int)(elementSize.Width / 2), borderRectangle.Top, elementSize.Width, elementSize.Height);
                            }
                            else
                            {
                                g.FillRectangle(graphSetting.RodmanSelectedBrush, location.X - (int)(elementSize.Width / 2), borderRectangle.Top, elementSize.Width, elementSize.Height);
                            }

                            break;
                        }
                    case RodmanType.Vertical:
                        {
                            g.DrawLine(graphSetting.AdjustLinePen, borderRectangle.Left, location.Y, borderRectangle.Right, location.Y);
                            g.FillRectangle(graphSetting.RodmanBackgroundBrush, borderRectangle.Location.X, borderRectangle.Location.Y, (int)(elementSize.Width / 2), borderRectangle.Height);

                            if (activated)
                            {
                                g.FillRectangle(graphSetting.RodmanActivatedBrush, borderRectangle.Left, location.Y - (int)(elementSize.Width / 2), elementSize.Height, elementSize.Width);
                            }
                            else
                            {
                                g.FillRectangle(graphSetting.RodmanSelectedBrush, borderRectangle.Left, location.Y - (int)(elementSize.Width / 2), elementSize.Height, elementSize.Width);
                            }

                            break;
                        }
                }
            }            
        }

        /// <summary>
        /// ��������ͼԪ
        /// </summary>
        /// <param name="g">��ͼ����</param>
        public override void AbbreviativePaint(Graphics g)
        {
            
        }

        /// <summary>
        /// �ж�ĳ�����Ƿ��ڸ�ͼԪ��������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�õ��Ƿ���ͼԪ��������</returns>
        public override bool IsInRegion(Point p)
        {
            bool inRegion = false;

            if (p.Y - borderRectangle.Top < elementSize.Height)
            {
                this.rodmanType = RodmanType.Horizonal;
                inRegion = true;
            }
            else if (p.X - borderRectangle.Left < elementSize.Height)
            {
                this.rodmanType = RodmanType.Vertical;
                inRegion = true;
            }
            else
            {
                this.rodmanType = RodmanType.None;
            }

            return inRegion;
        }

        /// <summary>
        /// �ж�ͼԪ�Ƿ���ĳ��������
        /// </summary>
        /// <param name="r">��ǰ����</param>
        /// <returns>ͼԪ�Ƿ��ڵ�ǰ������</returns>
        public override bool IsInRegion(Rectangle r)
        {
            return false;
        }

        /// <summary>
        /// �ƶ�ͼԪ
        /// </summary>
        /// <param name="s">�ƶ������λ��</param>
        public override void Move(Size s)
        {
            this.location += s;
        }

        /// <summary>
        /// ˢ��ͼԪ���Ʋ���
        /// </summary>
        public override void Refresh()
        {

        }

        /// <summary>
        /// �ͷ�ͼԪ�Ļ�ͼ��Դ
        /// </summary>
        public override void Release()
        {

        }
    }
}
