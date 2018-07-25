using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.ComponentModel;
using FlowChartEditor.BaseGraphElement;

namespace FlowChartEditor.ButtonGraphElement
{
    [Serializable]
    public class ConnectButton : BaseButton
    {
        private SlotGraphElement bindingSlot; // �󶨵Ĳ��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="bindingSlot">�󶨵Ĳ��</param>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="elementSize">ͼԪ�Ĵ�С</param>
        public ConnectButton(SlotGraphElement bindingSlot, Point location, Size elementSize)
            :base(location, elementSize)
        {
            this.bindingSlot = bindingSlot;
        }

        /// <summary>
        /// �󶨵Ĳ������
        /// </summary>
        public SlotGraphElement BindingSlot
        {
            get
            {
                return bindingSlot;
            }
        }

        /// <summary>
        /// ˢ��ͼԪ����
        /// </summary>
        public override void Refresh()
        {
            base.Refresh();

            normalImage = graphSetting.ConnectButtonNormalImage;
            activatedImage = graphSetting.ConnectButtonActivatedImage;
        }
    }
}
