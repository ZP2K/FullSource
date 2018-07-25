using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class AISlotContainer:SlotContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="elementSize">ͼԪ�Ĵ�С</param>
        public AISlotContainer(Point location, Size elementSize):
            base(location, elementSize)
        {
            enable = true;
        }

        /// <summary>
        /// ˢ��ͼԪ������
        /// </summary>
        public override void Refresh()
        {
            base.Refresh();
            requireEvent = false;
        }
    }
}
