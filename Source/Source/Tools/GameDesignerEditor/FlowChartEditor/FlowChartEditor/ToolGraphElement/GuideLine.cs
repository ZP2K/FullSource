using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

using GraphElementLibrary;

namespace FlowChartEditor
{
    public class GuideLine : GraphElement
    {
        private List<Point[]> guideLineList = new List<Point[]>(); // ���������� 
        private GraphSetting graphSetting = GraphSetting.GetGraphSetting();

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        public GuideLine(Point location) : base(location)
        {

        }

        /// <summary>
        /// ��ӵ�����
        /// </summary>
        /// <param name="pointArray">�����ߵ�����</param>
        public void AddGuideLine(Point[] pointArray)
        {
            guideLineList.Add(pointArray);
        }

        /// <summary>
        /// �������������
        /// </summary>        
        public void ClearGuideLineList()
        {
            guideLineList.Clear();
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
            if (visible && graphSetting.CanvasScale == 1.0f)
            {                
                foreach (Point[] points in guideLineList)
                {
                    g.DrawLine(graphSetting.AdjustLinePen, points[0], points[1]);
                }                
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
