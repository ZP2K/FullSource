using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using FlowChartEditor.ButtonGraphElement;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class AIActionsGraphElement:AISlotContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="locaiton">ͼԪ��λ��</param>
        /// <param name="elementSize">ͼԪ�Ĵ�С</param>
        public AIActionsGraphElement(Point locaiton, Size elementSize):base(locaiton, elementSize)
        {
            inSlotCount = 1;
            outSlotCount = 1;
        }

        /// <summary>
        /// �����ַ���
        /// </summary>
        public override string TypeString
        {
            get
            {
                return "AIActionsNode";
            }
        }

        /// <summary>
        /// ��ʼ�����Ĳ��
        /// </summary>
        /// <param name="Size">��ǰͼԪ�Ĵ�С</param>
        protected override void InitSlot(Size size)
        {
            Helper helper = Helper.GetHelper();

            int x1 = location.X + (int)(size.Width / 2);
            int y1 = location.Y;
            int x2 = x1;
            int y2 = location.Y + size.Height;

            SlotGraphElement slot1 = new SlotGraphElement(this, new Point(x1, y1), new Size(6, 6));
            SlotGraphElement slot2 = new SlotGraphElement(this, new Point(x2, y2), new Size(6, 6));

            slot1.Name = "���Ӳ��";
            slot1.IsInSlot = true;
            slot1.CanDelete = false;

            slot2.Name = "���Ӳ��";
            slot2.IsOutSlot = true;
            slot2.CanDelete = false;

            ConnectButton button = new ConnectButton(slot2, new Point(x2 - 6, y2 + 25), new Size(12, 8));
            button.Name = "���Ӱ�ť";
            slot2.BindingConnectButton = button;

            inSlotList.Add(slot1);
            outSlotList.Add(slot2);
            buttonList.Add(button);
            slotList.Add(slot1);
            slotList.Add(slot2);

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
        /// ����״̬����
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
                normalImage = graphSetting.AIActionsNodeNormalImage;
            }

            if (selectedImage == null)
            {
                selectedImage = graphSetting.AIActionsNodeSelectedImage;
            }

            if (activatedImage == null)
            {
                activatedImage = graphSetting.AIActionsNodeActivatedImage;
            }

            if (shadowImage == null)
            {
                shadowImage = graphSetting.AIActionsNodeShadowImage;
            }

            if(abbreviateImage == null)
            {
                abbreviateImage = graphSetting.AIActionsNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public override SlotContainer Clone()
        {
            AIActionsGraphElement aiActionsGraphElement = new AIActionsGraphElement(location, elementSize);
            CopyData(this, aiActionsGraphElement);

            return aiActionsGraphElement;
        }
    }
}
