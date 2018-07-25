using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace FlowChartEditor
{
    [Serializable]
    class ConnectJumpButton:ButtonGraphElement
    {
        private SlotGraphElement bindingSlot; // �󶨵Ĳ��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="bindingSlot">�󶨵Ĳ��</param>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="elementSize">ͼԪ�Ĵ�С</param>
        public ConnectJumpButton(SlotGraphElement bindingSlot, Point location, Size elementSize)
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
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void Paint(Graphics g)
        {
            RefreshImages();

            Rectangle rectangle = new Rectangle(location, elementSize);
            Rectangle hoverRectangle = new Rectangle(new Point(location.X, location.Y - 1),
                new Size(elementSize.Width + 3, elementSize.Height + 2));
            Image image;

            if (selected) // ͼԪ��ѡ��
            {
                image = selectedImage;
                g.DrawImage(image, hoverRectangle);
            }
            else if (activated) // ͼԪ������
            {
                image = activatedImage;
                g.DrawImage(image, hoverRectangle);
            }
            else // ͼԪû�б�ѡ�кͼ���
            {
                image = normalImage;
                g.DrawImage(image, rectangle);
            }
        }
    }
}
