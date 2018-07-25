using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.ComponentModel;
using GraphElementLibrary;

namespace FlowChartEditor
{
    class TextGraphElement:SlotContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="id">ͼԪ�Ĵ�С</param>
        public TextGraphElement(Point location, Size size)
            : base(location, size)
        {
            inSlotCount = 0;
            outSlotCount = 0;
        }

        /// <summary>
        /// ͼԪ��С����
        /// </summary>
        [Browsable(true), Description("ͼԪ�Ĵ�С"), Category("��ͼ����")]
        public override Size ElementSize
        {
            set
            {
                elementSize = value;
                AdjustResizeControlerList(); // �������ſ��Ƶ�����
            }

            get
            {
                return elementSize;
            }            
        }


        /// <summary>
        /// ��ʼ�����
        /// </summary>
        /// <param name="size">ͼԪ�Ĵ�С</param>
        protected override void InitSlot(Size size)
        {
            
        }

        /// <summary>
        /// ������ڲ��
        /// </summary>
        /// <returns>�����Ĳ��</returns>
        public override SlotGraphElement AddInSlot()
        {
            return null;
        }

        /// <summary>
        /// ���ӳ��ڲ��
        /// </summary>
        /// <returns>�����Ĳ��</returns>
        public override SlotGraphElement AddOutSlot()
        {
            return null;
        }

        /// <summary>
        /// ɾ����ڲ��
        /// </summary>
        /// <param name="slot">Ҫɾ���Ĳ��</param>
        public override void RemoveInSlot(SlotGraphElement slot)
        {

        }

        /// <summary>
        /// ɾ�����ڲ��
        /// </summary>
        /// <param name="slot">Ҫɾ���Ĳ��</param>
        public override void RemoveOutSlot(SlotGraphElement slot)
        {

        }
    }
}
