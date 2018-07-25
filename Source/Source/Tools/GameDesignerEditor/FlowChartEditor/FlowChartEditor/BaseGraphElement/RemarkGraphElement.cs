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
    public class RemarkGraphElement:GraphElement
    {
        protected GraphElement owner; // ע��������ͼԪ
        protected Point headPoint = Point.Empty; // ������ͷ���λ��
        protected Point tailPoint = Point.Empty; // ������β���λ��
        protected Point textLocation = Point.Empty; // �����ı���λ��

        [NonSerialized]
        Rectangle textRectangle = Rectangle.Empty;

        [NonSerialized]
        protected GraphSetting graphSetting; // ��ͼ�������ö���

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="owner">ע��������ͼԪ</param>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="elementSize">ͼԪ�Ĵ�С</param>
        public RemarkGraphElement(GraphElement owner, Point location, Size elementSize):
            base(location)
        {
            this.owner = owner;
            this.location = location;
            this.elementSize = elementSize;
        }

        /// <summary>
        /// ͼԪ��ˢ����������
        /// </summary>
        [Browsable(false)]
        public override Rectangle InvalidRectangle
        {
            get
            {
                invalidRectangle = new Rectangle(location, elementSize);

                return invalidRectangle;
            }
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
                textRectangle.Location = value;
                AdjustLine(); // ����������
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
        /// ͼԪ�Ƿ񼤻�����
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
        /// ע��������ͼԪ����
        /// </summary>
        [Browsable(false)]
        public virtual GraphElement Owner
        {
            set
            {
                owner = value;
            }

            get
            {
                return owner;
            }
        }

        /// <summary>
        /// ˢ��ͼԪ���Ʋ���
        /// </summary>
        public override void Refresh()
        {
            // ��ʼ����ͼ���ݹ������
            graphSetting = GraphSetting.GetGraphSetting();

            // ˢ���߼�����
            textRectangle.Location = location;
            textRectangle.Size = elementSize;    
        }

        /// <summary>
        /// �ͷ�ͼԪ�Ļ�ͼ��Դ
        /// </summary>
        public override void Release()
        {

        }

        /// <summary>
        /// �Զ�����ͼԪ�Ĵ�С
        /// </summary>
        public virtual void AdjustElementSize()
        {
            Graphics g = Helper.GetHelper().GraphicsObject;
            SizeF textSize = g.MeasureString(text, graphSetting.GraphElementTextFont);
            float textWidth = textSize.Width;
            float textHeight = textSize.Height;
            int newWidth = (int)(textWidth + 10);
            int newHeight = (int)(textHeight + 10);
            textLocation = location + new Size(5, 5);

            // ����ͼԪ��С
            if (newWidth != elementSize.Width || newHeight != elementSize.Height)
            {
                elementSize = new Size(newWidth, newHeight);
                textRectangle.Size = elementSize;
            }
        }

        /// <summary>
        /// �ж�ĳ�����Ƿ��ڸ�ͼԪ��������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�õ��Ƿ���ͼԪ��������</returns>
        public override bool IsInRegion(Point p)
        {
            bool result = textRectangle.Contains(p);

            return result;
        }

        /// <summary>
        /// �ж�ͼԪ�Ƿ���ĳ��������
        /// </summary>
        /// <param name="r">��ǰ����</param>
        /// <returns>ͼԪ�Ƿ��ڵ�ǰ������</returns>
        public override bool IsInRegion(Rectangle r)
        {
            bool result = r.Contains(textRectangle);

            return result;
        }

        /// <summary>
        /// ����������
        /// </summary>
        public void AdjustLine()
        {
            int ownerX = (int)(owner.Location.X + owner.ElementSize.Width / 2);
            int ownerY = (int)(owner.Location.Y + owner.ElementSize.Height / 2);
            int x = location.X;
            int y = location.Y;
            int lineX1;
            int lineY1;
            int lineX2;
            int lineY2;

            if(ownerX >= x)
            {
                if(ownerY >= y) // ע��������ͼԪ�����Ϸ�
                {
                    lineX1 = location.X + elementSize.Width;
                    lineY1 = location.Y + elementSize.Height;
                    lineX2 = owner.Location.X;
                    lineY2 = owner.Location.Y;
                }
                else // ע��������ͼԪ�����·�
                {
                    lineX1 = location.X + elementSize.Width;
                    lineY1 = location.Y;
                    lineX2 = owner.Location.X;
                    lineY2 = owner.Location.Y + owner.ElementSize.Height;
                }
            }
            else
            {
                if(ownerY >= y) // ע��������ͼԪ�����Ϸ�
                {
                    lineX1 = location.X;
                    lineY1 = location.Y + elementSize.Height;
                    lineX2 = owner.Location.X + owner.ElementSize.Width;
                    lineY2 = owner.Location.Y;
                }
                else // ע��������ͼԪ�����·�
                {
                    lineX1 = location.X;
                    lineY1 = location.Y;
                    lineX2 = owner.Location.X + owner.ElementSize.Width;
                    lineY2 = owner.Location.Y + owner.ElementSize.Height;
                }
            }

            headPoint.X = lineX1;
            headPoint.Y = lineY1;
            tailPoint.X = lineX2;
            tailPoint.Y = lineY2;
        }

        /// <summary>
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void Paint(Graphics g)
        {
            if (visible)
            {
                // ���Ʊ���
                g.FillRectangle(graphSetting.RemarkBackgroundBrush, textRectangle);          

                // ���Ʊ߿�
                g.DrawRectangle(graphSetting.GraphElementBorderPen, textRectangle);

                // ����������           
                g.DrawLine(graphSetting.LineNormalPen, headPoint, tailPoint);
                
                //�����ı�
                g.DrawString(text, graphSetting.GraphElementTextFont, graphSetting.GraphElementTextBrush, textLocation);
            }            
        }

        /// <summary>
        /// ����ͼԪ������Ч��
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void AbbreviativePaint(Graphics g)
        {

        }

        /// <summary>
        /// �ƶ�ͼԪ
        /// </summary>
        /// <param name="s">�ƶ������λ��</param>
        public override void Move(Size s)
        {
            this.location += s;
            textRectangle.Location += s;
            textLocation += s;
            AdjustLine();
        }
    }
}
