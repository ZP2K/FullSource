using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

using GraphElementLibrary;

namespace FlowChartEditor
{
    public class MultiSelectMark : GraphElement
    {
        private GraphSetting graphSetting = GraphSetting.GetGraphSetting();
        private bool showBackground = false; // �Ƿ���ʾ����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪλ��</param>
        /// <param name="elementSize">ͼԪ��С</param>
        public MultiSelectMark(Point location, Size elementSize) : base(location)
        {
            this.elementSize = elementSize;

            // Ĭ�ϲ���ʾ
            visible = false;
        }

        /// <summary>
        /// �Ƿ���ʾ����
        /// </summary>
        public bool ShowBackground
        {
            set
            {
                showBackground = value;
            }

            get
            {
                return showBackground;
            }
        }

        /// <summary>
        /// �������
        /// </summary>
        public Rectangle RegionRectangle
        {
            get
            {
                return new Rectangle(location, elementSize);
            }
        }

        /// <summary>
        /// ������ѡ���
        /// </summary>
        /// <param name="location">��ѡ��ǵ�λ��</param>
        /// <param name="elementSize">��ѡ��ǵĴ�С</param>
        public void Adjust(Point location, Size elementSize)
        {
            this.location = location;
            this.elementSize = elementSize;
        }

        /// <summary>
        /// �ж�ĳ�����Ƿ��ڸ�ͼԪ��������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�õ��Ƿ���ͼԪ��������</returns>
        public override bool IsInRegion(Point p)
        {
            return false;
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
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void Paint(Graphics g)
        {
            if (visible)
            {
                // ���Ʊ���
                if (showBackground)
                {
                    g.FillRectangle(graphSetting.MultiSelectBackgroudBrush, location.X, location.Y, elementSize.Width, elementSize.Height);
                }
                
                // ���Ʊ߿�
                g.DrawRectangle(graphSetting.MultiSelectMarkPen, location.X, location.Y, elementSize.Width, elementSize.Height);
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
