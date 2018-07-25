using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel;

namespace FlowChartEditor
{
    [Serializable]
    class JumpGraphElement:SlotContainer
    {
        private List<SlotGraphElement> jumpSlotList = new List<SlotGraphElement>(); // ��ת�������
        private int jumpSlotCount; // ��ת�������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="id">ͼԪ�Ĵ�С</param>
        public JumpGraphElement(Point location, Size size)
            : base(location, size)
        {
            inSlotCount = 1;
            outSlotCount = 1;
            jumpSlotCount = 1;
        }        

        /// <summary>
        /// ��ת�����������
        /// </summary>
        public int JumpSlotCount
        {
            set
            {
                AdjustJumpSlotList(value);
                jumpSlotCount = value;
            }

            get
            {
                return jumpSlotCount;
            }
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
                AdjustInSlotList(inSlotCount); // ������ڲ������
                AdjustOutSlotList(outSlotCount); // �������ڲ������
                AdjustJumpSlotList(jumpSlotCount); // ������ת�������
                AdjustResizeControlerList(); // �������ſ��Ƶ�����
                AdjustButtonList(); // ������ť����
            }

            get
            {
                return elementSize;
            }
        }

        /// <summary>
        /// ��ʼ�����Ĳ��
        /// </summary>
        /// <param name="Size">��ǰͼԪ�Ĵ�С</param>
        protected override void InitSlot(Size size)
        {
            int x1 = location.X + (int)(size.Width / 2) - 3;
            int y1 = location.Y - 3;
            int x2 = x1;
            int y2 = location.Y + size.Height;
            int x3 = location.X + size.Width - 3;
            int y3 = location.Y + (int)((size.Height - 20) / 2) - 3 + 20;

            SlotGraphElement slot1 = new SlotGraphElement(this, new Point(x1, y1), new Size(6, 6));
            SlotGraphElement slot2 = new SlotGraphElement(this, new Point(x2, y2), new Size(6, 6));
            SlotGraphElement slot3 = new SlotGraphElement(this, new Point(x3, y3), new Size(6, 6));

            slot1.Name = "���Ӳ��";
            slot1.FrameColor = Color.RoyalBlue;
            slot1.SelectedColor = Color.Red;
            slot1.ActivatedColor = Color.DarkOrange;
            slot1.BindedColor = Color.Lime;
            slot1.IsInSlot = true;
            slot1.IsOutSlot = false;
            slot1.IsJumpSlot = false;
            slot1.CanDelete = true;
            slot2.Name = "���Ӳ��";
            slot2.FrameColor = Color.RoyalBlue;
            slot2.SelectedColor = Color.Red;
            slot2.ActivatedColor = Color.DarkOrange;
            slot2.BindedColor = Color.Lime;
            slot2.IsInSlot = false;
            slot2.IsOutSlot = true;
            slot2.IsJumpSlot = false;
            slot2.CanDelete = true;
            slot3.Name = "���Ӳ��";
            slot3.FrameColor = Color.RoyalBlue;
            slot3.SelectedColor = Color.Red;
            slot3.ActivatedColor = Color.DarkOrange;
            slot3.BindedColor = Color.Lime;
            slot3.IsInSlot = false;
            slot3.IsOutSlot = false;
            slot3.IsJumpSlot = true;
            slot3.CanDelete = false;

            ConnectButton button = new ConnectButton(slot2, new Point(x2 - 6, y2 + 5), new Size(12, 18));
            button.Name = "���Ӱ�ť";
            button.NormalImage = connectButtonNormalImage;
            button.SelectedImage = connectButtonSelectedImage;
            button.ActivatedImage = connectButtonActivatedImage;
            slot2.BindingConnectButton = button;

            ConnectJumpButton jumpButton = new ConnectJumpButton(slot3, new Point(x3 + 5, y3 - 6), new Size(18, 12));
            jumpButton.Name = "���Ӱ�ť";
            jumpButton.NormalImage = connectJumpButtonNormalImage;
            jumpButton.SelectedImage = connectJumpButtonSelectedImage;
            jumpButton.ActivatedImage = connectJumpButtonActivatedImage;
            slot3.BindingConnectButton = jumpButton;

            inSlotList.Add(slot1);
            outSlotList.Add(slot2);
            jumpSlotList.Add(slot3);
            buttonList.Add(button);
            buttonList.Add(jumpButton);
            slotList.Add(slot1);
            slotList.Add(slot2);
            slotList.Add(slot3);
        }

        /// <summary>
        /// �����������
        /// </summary>
        /// <param name="n">�������</param>
        private void AdjustJumpSlotList(int n)
        {
            if (n == jumpSlotList.Count) // ����Ҫ�������
            {
                int newY;
                Point newPoint;

                // �������������
                for (int i = 0; i < jumpSlotList.Count; i++)
                {
                    newY = (int)(location.Y + (elementSize.Height - 20) * (i + 1) / (n + 1) - 3 + 20);
                    newPoint = new Point(location.X + elementSize.Width - 3, newY);
                    jumpSlotList[i].Location = newPoint;
                    jumpSlotList[i].BindingConnectButton.Location = new Point(location.X + elementSize.Width + 5, newY - 6);
                    if (jumpSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                    {
                        jumpSlotList[i].BindingConnector.Location = newPoint;
                    }
                }
            }
            else if (n < jumpSlotList.Count) // ��Ҫ���ٲ������
            {
                int deleteCount = jumpSlotList.Count - n;
                int newY;
                Point newPoint;
                List<SlotGraphElement> deleteSlotList = new List<SlotGraphElement>();

                for (int i = 0; i < deleteCount; i++)
                {
                    deleteSlotList.Add(jumpSlotList[jumpSlotList.Count - 1 - i]);
                }

                foreach (SlotGraphElement slot in deleteSlotList)
                {
                    if (slot.Binded)
                    {
                        slot.UnBind();
                    }

                    jumpSlotList.Remove(slot);
                    slotList.Remove(slot);
                }

                // �������������
                for (int i = 0; i < jumpSlotList.Count; i++)
                {
                    newY = (int)(location.Y + (elementSize.Height - 20) * (i + 1) / (n + 1) - 3 + 20);
                    newPoint = new Point(location.X + elementSize.Width - 3, newY);
                    jumpSlotList[i].Location = newPoint;
                    jumpSlotList[i].BindingConnectButton.Location = new Point(location.X + elementSize.Width + 5, newY - 6);
                    if (jumpSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                    {
                        jumpSlotList[i].BindingConnector.Location = newPoint;
                    }
                }
            }
            else // ��Ҫ���Ӳ������
            {
                int oldCount = jumpSlotList.Count;
                SlotGraphElement newSlot;
                Point newPoint;
                int newY;

                // ��Ӳ�۲�������۵�����
                for (int i = 0; i < n; i++)
                {
                    if (i < oldCount)
                    {
                        newY = (int)(location.Y + (elementSize.Height - 20) * (i + 1) / (n + 1) - 3 + 20);
                        newPoint = new Point(location.X + elementSize.Width - 3, newY);
                        jumpSlotList[i].Location = newPoint;
                        jumpSlotList[i].BindingConnectButton.Location = new Point(location.X + elementSize.Width + 5, newY - 6);
                        if (jumpSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                        {
                            jumpSlotList[i].BindingConnector.Location = newPoint;
                        }
                    }
                    else
                    {
                        newY = (int)(location.Y + (elementSize.Height - 20) * (i + 1) / (n + 1) - 3 + 20);
                        newPoint = new Point(location.X + elementSize.Width - 3, newY);
                        newSlot = new SlotGraphElement(this, newPoint, new Size(6, 6));
                        newSlot.Name = "���Ӳ��";
                        newSlot.FrameColor = Color.RoyalBlue;
                        newSlot.SelectedColor = Color.Red;
                        newSlot.ActivatedColor = Color.DarkOrange;
                        newSlot.BindedColor = Color.Lime;
                        newSlot.IsInSlot = false;
                        newSlot.IsOutSlot = false;
                        newSlot.IsJumpSlot = true;
                        newSlot.CanDelete = false;

                        ConnectJumpButton button = new ConnectJumpButton(newSlot, new Point(location.X + elementSize.Width + 5, 
                            newY - 6), new Size(12, 18));
                        button.Name = "���Ӱ�ť";
                        button.NormalImage = connectJumpButtonNormalImage;
                        button.SelectedImage = connectJumpButtonSelectedImage;
                        button.ActivatedImage = connectJumpButtonActivatedImage;
                        newSlot.BindingConnectButton = button;

                        buttonList.Add(button);
                        jumpSlotList.Add(newSlot);
                        slotList.Add(newSlot);
                    }
                }
            }
        }

        /// <summary>
        /// ��ȡ��ת�������
        /// </summary>
        /// <returns>��ת������� </returns>
        public List<SlotGraphElement> GetJumpSlotList()
        {
            return jumpSlotList;
        }

        /// <summary>
        /// ������ת���
        /// </summary>
        /// <returns>�����Ĳ��</returns>
        public SlotGraphElement AddJumpSlot()
        {            
            JumpSlotCount = jumpSlotCount + 1;
            SlotGraphElement newSlot = jumpSlotList[jumpSlotCount - 1];            

            return newSlot;
        }

        /// <summary>
        /// ��ȡ��ת���
        /// </summary>
        /// <returns>��ת���</returns>
        public SlotGraphElement GetJumpSlot()
        {
            bool find = false; // �Ƿ��Ѿ��ҵ�û�а������߿��Ƶ�Ĳ��
            SlotGraphElement newSlot = null;

            foreach (SlotGraphElement slot in jumpSlotList)
            {
                if (!slot.Binded)
                {
                    newSlot = slot;
                    find = true;
                    break;
                }
            }

            if (!find) // û���ҵ�û�а������߿��Ƶ�Ĳ�ۣ����½�һ����ڲ��
            {
                JumpSlotCount = jumpSlotCount + 1;
                newSlot = jumpSlotList[jumpSlotCount - 1];
            }

            return newSlot;
        }
    }
}
