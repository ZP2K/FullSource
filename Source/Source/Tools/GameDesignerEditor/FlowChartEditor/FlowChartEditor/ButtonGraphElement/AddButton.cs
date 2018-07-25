using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using FlowChartEditor.BaseGraphElement;
using System.ComponentModel;

namespace FlowChartEditor.ButtonGraphElement
{
    [Serializable]
    public class AddButton:BaseButton
    {
        private SlotContainer slotContainer; // ��Ӱ�ť��Ӧ�Ĳ��        

        public AddButton(SlotContainer slotContainer, Point location, Size elementSize)
            :base(location, elementSize)
        {
            this.slotContainer = slotContainer;
        }

        /// <summary>
        /// �󶨵Ĳ������
        /// </summary>
        [Browsable(false)]
        public SlotContainer BindingSlotContainer
        {
            get
            {
                return slotContainer;
            }
        }

        /// <summary>
        /// ˢ��ͼԪ����
        /// </summary>
        public override void Refresh()
        {
            base.Refresh();

            normalImage = graphSetting.AddButtonNormalImage;
            activatedImage = graphSetting.AddButtonActivatedImage;
        }
    }
}
