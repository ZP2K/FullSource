using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing.Imaging;
using GraphElementLibrary;

namespace FlowChartEditor.BaseGraphElement
{
    class CustomGraphElement: SlotContainer 
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="id">ͼԪ�Ĵ�С</param>
        public CustomGraphElement(Point location, Size size)
            : base(location, size)
        {
            inSlotCount = 1;
            outSlotCount = 1;
        }        
    }
}
