using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using FlowChartEditor.ButtonGraphElement;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class AIActionGraphElement:AISlotContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="elementSize">ͼԪ�Ĵ�С</param>
        public AIActionGraphElement(Point location, Size elementSize):base(location, elementSize)
        {
            inSlotCount = 1;
            outSlotCount = 3;
        }

        /// <summary>
        /// �����ַ���
        /// </summary>
        public override string TypeString
        {
            get
            {
                return "AIActionNode";
            }
        }

        /// <summary>
        /// ��ʼ�����Ĳ��
        /// </summary>
        /// <param name="Size">��ǰͼԪ�Ĵ�С</param>
        protected override void InitSlot(Size size)
        {
            int x1 = location.X + (int)(size.Width / 2);
            int y1 = location.Y;
            int x2 = location.X + (int)(size.Width * 1 / 4);
            int y2 = location.Y + size.Height;
            int x3 = location.X + (int)(size.Width * 2 / 4);
            int y3 = y2;
            int x4 = location.X + (int)(size.Width * 3 / 4);
            int y4 = y2;

            SlotGraphElement slot1 = new SlotGraphElement(this, new Point(x1, y1), new Size(6, 6));
            SlotGraphElement slot2 = new SlotGraphElement(this, new Point(x2, y2), new Size(6, 6));
            SlotGraphElement slot3 = new SlotGraphElement(this, new Point(x3, y3), new Size(6, 6));
            SlotGraphElement slot4 = new SlotGraphElement(this, new Point(x4, y4), new Size(6, 6));

            slot1.Name = "���Ӳ��";
            slot1.IsInSlot = true;
            slot1.CanDelete = false;

            slot2.Name = "���Ӳ��";
            slot2.IsOutSlot = true;
            slot2.CanDelete = false;

            slot3.Name = "���Ӳ��";
            slot3.IsOutSlot = true;
            slot3.CanDelete = false;

            slot4.Name = "���Ӳ��";
            slot4.IsOutSlot = true;
            slot4.CanDelete = false;

            ConnectButton button1 = new ConnectButton(slot2, new Point(x2 - 6, y2 + 25), new Size(12, 8));
            button1.Name = "���Ӱ�ť";
            slot2.BindingConnectButton = button1;

            ConnectButton button2 = new ConnectButton(slot3, new Point(x3 - 6, y3 + 25), new Size(12, 8));
            button2.Name = "���Ӱ�ť";
            slot3.BindingConnectButton = button2;

            ConnectButton button3 = new ConnectButton(slot4, new Point(x4 - 6, y4 + 25), new Size(12, 8));
            button3.Name = "���Ӱ�ť";
            slot4.BindingConnectButton = button3;

            inSlotList.Add(slot1);
            outSlotList.Add(slot2);
            outSlotList.Add(slot3);
            outSlotList.Add(slot4);
            buttonList.Add(button1);
            buttonList.Add(button2);
            buttonList.Add(button3);
            slotList.Add(slot1);
            slotList.Add(slot2);
            slotList.Add(slot3);
            slotList.Add(slot4);

            // ���������ȼ�
            ResetSlotProperty(inSlotList);
            ResetSlotProperty(outSlotList);
        }

        /// <summary>
        /// ��ʼ����ť
        /// </summary>
        protected override void InitButton()
        {

        }

        /// <summary>
        /// ������ť����
        /// </summary>
        protected override void AdjustButtonList()
        {
            
        }

        /// <summary>
        /// ��ȡ���ڲ��
        /// </summary>
        /// <returns>���ڲ��</returns>
        public override SlotGraphElement GetOutSlot()
        {
            int priority = -1;
            SlotGraphElement newSlot = null;

            foreach (SlotGraphElement slot in outSlotList)
            {
                if (!slot.Binded)
                {
                    if (slot.Priority < priority || priority == -1)
                    {
                        newSlot = slot;
                        priority = slot.Priority;
                    }
                }
            }

            return newSlot;
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
        /// ˢ��ͼԪ��ͼƬ
        /// </summary>
        protected override void RefreshImages()
        {
            if (normalImage == null)
            {
                normalImage = graphSetting.AIActionNodeNormalImage;
            }

            if (selectedImage == null)
            {
                selectedImage = graphSetting.AIActionNodeSelectedImage;
            }

            if (activatedImage == null)
            {
                activatedImage = graphSetting.AIActionNodeActivatedImage;
            }

            if (shadowImage == null)
            {
                shadowImage = graphSetting.AIActionNodeShadowImage;
            }

            if (abbreviateImage == null)
            {
                abbreviateImage = graphSetting.AIActionNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public override SlotContainer Clone()
        {
            AIActionGraphElement aiActionGraphElement = new AIActionGraphElement(location, elementSize);
            CopyData(this, aiActionGraphElement);

            return aiActionGraphElement;
        }
    }
}
