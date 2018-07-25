using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.ComponentModel;
using System.Windows.Forms;
using FlowChartEditor.ButtonGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class ConditionGraphElement:SlotContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="size">ͼԪ�Ĵ�С</param>
        public ConditionGraphElement(Point location, Size size):base(location, size)
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
                return "ConditionNode";
            }
        }

        /// <summary>
        /// ��ʼ�����Ĳ��
        /// </summary>
        /// <param name="size">ͼԪ�Ĵ�С</param>
        protected override void InitSlot(Size size)
        {
            Helper helper = Helper.GetHelper();
            int x1 = location.X + (int)(size.Width / 2);
            int y1 = location.Y;            
            int x2 = location.X;
            int y2 = location.Y + (int)(size.Height / 2);
            int x3 = location.X + size.Width;
            int y3 = y2;
            int x4 = location.X + (int)(size.Width / 2);
            int y4 = location.Y + size.Height;

            SlotGraphElement slot1 = new SlotGraphElement(this, new Point(x1, y1), new Size(6, 6));
            SlotGraphElement slot2 = new SlotGraphElement(this, new Point(x2, y2), new Size(6, 6));
            SlotGraphElement slot3 = new SlotGraphElement(this, new Point(x3, y3), new Size(6, 6));
            SlotGraphElement slot4 = new SlotGraphElement(this, new Point(x4, y4), new Size(6, 6));

            slot1.Name = "���Ӳ��";         
            slot1.IsInSlot = true;

            slot2.Name = "���Ӳ��";
            slot2.IsOutSlot = true;
            slot2.CanDelete = false;
            slot2.Priority = 101;
            ConnectButton button1 = new ConnectButton(slot2, new Point(x2 - 6, y2 + 25), new Size(12, 8));
            button1.Name = "���Ӱ�ť";
            slot2.BindingConnectButton = button1;

            slot3.Name = "���Ӳ��";
            slot3.IsOutSlot = true;
            slot3.CanDelete = false;
            slot3.Priority = 100;
            ConnectButton button2 = new ConnectButton(slot3, new Point(x3 - 6, y3 + 25), new Size(12, 8));
            button2.Name = "���Ӱ�ť";
            slot3.BindingConnectButton = button2;

            slot4.Name = "���Ӳ��";
            slot4.IsOutSlot = true;
            slot4.CanDelete = false;

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
        /// �������ڲ������
        /// </summary>
        /// <param name="n">�������</param>
        protected override void AdjustOutSlotList(int n)
        {
            int newX;
            int newY;
            Point newPoint = Point.Empty;

            if (n == outSlotList.Count) // ����Ҫ�������
            {
                newX = location.X;
                newY = (int)(location.Y + elementSize.Height / 2);
                newPoint.X = newX;
                newPoint.Y = newY;
                outSlotList[0].Location = newPoint;
                outSlotList[0].BindingConnectButton.Location = new Point(newX - 6, newY + 25);
                if (outSlotList[0].Binded) // �����󶨵������߿��Ƶ�
                {
                    outSlotList[0].BindingConnector.Location = newPoint;
                }

                newX = location.X + elementSize.Width;
                newY = (int)(location.Y + elementSize.Height / 2);
                newPoint.X = newX;
                newPoint.Y = newY;
                outSlotList[1].Location = newPoint;
                outSlotList[1].BindingConnectButton.Location = new Point(newX - 6, newY + 25);
                if (outSlotList[1].Binded) // �����󶨵������߿��Ƶ�
                {
                    outSlotList[1].BindingConnector.Location = newPoint;
                }

                // �������������
                for (int i = 2; i < outSlotList.Count; i++)
                {
                    newX = (int)(location.X + elementSize.Width * (i - 2 + 1) / (n - 2 + 1));
                    newY = location.Y + elementSize.Height;
                    newPoint.X = newX;
                    newPoint.Y = newY;
                    outSlotList[i].Location = newPoint;
                    outSlotList[i].BindingConnectButton.Location = new Point(newX - 6, newY + 25);
                    if (outSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                    {
                        outSlotList[i].BindingConnector.Location = newPoint;
                    }
                }
            }
            else if (n < outSlotList.Count) // ��Ҫ���ٲ������
            {
                int deleteCount = outSlotList.Count - n;
                List<SlotGraphElement> deleteSlotList = new List<SlotGraphElement>();

                for (int i = 0; i < deleteCount; i++)
                {
                    deleteSlotList.Add(outSlotList[outSlotList.Count - 1 - i]);
                }

                foreach (SlotGraphElement slot in deleteSlotList)
                {
                    if (slot.Binded)
                    {
                        slot.UnBind();
                    }

                    buttonList.Remove(slot.BindingConnectButton);
                    slot.BindingConnectButton = null;
                    outSlotList.Remove(slot);
                    slotList.Remove(slot);
                }

                // �������������
                for (int i = 2; i < outSlotList.Count; i++)
                {
                    newX = (int)(location.X + elementSize.Width * (i - 2 + 1) / (n - 2 + 1));
                    newY = location.Y + elementSize.Height;
                    newPoint.X = newX;
                    newPoint.Y = newY;
                    outSlotList[i].Location = newPoint;
                    outSlotList[i].BindingConnectButton.Location = new Point(newX - 6, newY + 25);
                    if (outSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                    {
                        outSlotList[i].BindingConnector.Location = newPoint;
                    }
                }
            }
            else // ��Ҫ���Ӳ������
            {
                Helper helper = Helper.GetHelper();
                int oldCount = outSlotList.Count;
                SlotGraphElement newSlot;

                // ��Ӳ�۲�������۵�����
                for (int i = 2; i < n; i++)
                {
                    if (i < oldCount)
                    {
                        newX = (int)(location.X + elementSize.Width * (i - 2 + 1) / (n - 2 + 1));
                        newY = location.Y + elementSize.Height;
                        newPoint.X = newX;
                        newPoint.Y = newY;
                        outSlotList[i].Location = newPoint;
                        outSlotList[i].BindingConnectButton.Location = new Point(newX - 6, newY + 25);
                        if (outSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                        {
                            outSlotList[i].BindingConnector.Location = newPoint;
                        }
                    }
                    else
                    {
                        newX = (int)(location.X + elementSize.Width * (i - 2 + 1) / (n - 2 + 1));
                        newY = location.Y + elementSize.Height;
                        newPoint.X = newX;
                        newPoint.Y = newY;
                        newSlot = new SlotGraphElement(this, newPoint, new Size(6, 6));
                        newSlot.Name = "���Ӳ��";
                        newSlot.IsOutSlot = true;
                        newSlot.CanDelete = true;
                        newSlot.Refresh();

                        ConnectButton button = new ConnectButton(newSlot, new Point(newX - 6, location.Y + elementSize.Height + 25),
                            new Size(12, 8));
                        button.Name = "���Ӱ�ť";
                        button.Refresh();
                        newSlot.BindingConnectButton = button;

                        outSlotList.Add(newSlot);
                        buttonList.Add(button);
                        slotList.Add(newSlot);
                    }
                }
            }

            ResetSlotProperty(outSlotList);
        }

        /// <summary>
        /// �Զ�����ͼԪ�Ĵ�С
        /// </summary>
        public override void AdjustElementSize()
        {
            Graphics g = Helper.GetHelper().GraphicsObject;
            Rectangle textRectangle = new Rectangle(location, elementSize);
            SizeF textSize = g.MeasureString(text, graphSetting.GraphElementTextFont);
            float textWidth = textSize.Width;
            float textHeight = textSize.Height;
            int newWidth = elementSize.Width;
            int newHeight = elementSize.Height;

            // measurestring��׼��Ԥ���������ص�ƫ����
            if (textWidth * 2 < textRectangle.Width - 12)
            {
                if (textWidth * 2 < minSize.Width - 12)
                {
                    newWidth = minSize.Width;
                }
                else
                {
                    newWidth = (int)(textWidth * 2 + 12);
                }
            }
            else
            {
                newWidth = (int)(textWidth * 2 + 12);
            }

            if (textHeight * 2 < textRectangle.Height - 12)
            {
                if (textHeight * 2 < minSize.Height - 12)
                {
                    newHeight = minSize.Height;
                }
                else
                {
                    newHeight = (int)(textHeight * 2 + 12);
                }
            }
            else
            {
                newHeight = (int)(textHeight * 2 + 12);
            }

            // ����ͼԪ��С
            if (newWidth != elementSize.Width || newHeight != elementSize.Height)
            {
                if (newWidth > minSize.Width * 2)
                {
                    newWidth = (int)(minSize.Width * 1.5);
                    
                }

                if (newHeight > minSize.Height * 2)
                {
                    newHeight = (int)(minSize.Height * 1.5);
                }

                ElementSize = new Size(newWidth, newHeight);
            }
        }

        /// <summary>
        /// ����ͼԪ�ı�
        /// </summary>
        public override void AdjustText()
        {
            Graphics g = Helper.GetHelper().GraphicsObject;
            SizeF textSize = g.MeasureString(text, graphSetting.GraphElementTextFont);
            float textWidth = 0;
            float textHeight = textSize.Height;

            string[] data = text.Split(new char[] { '\n' });
            List<string> newData = new List<string>();
            displayText = "";
            float currentWidth;

            for (int i = 0; i < data.Length; i++)
            {
                string line = data[i];
                string newLine = line;
                textSize = g.MeasureString(line, graphSetting.GraphElementTextFont);
                currentWidth = textSize.Width * 2;
                if (currentWidth > textRectangle.Width - 10) // �ı�������Ч���
                {
                    int j = 2;

                    while (currentWidth > textRectangle.Width - 10)
                    {
                        j = 2;

                        while (currentWidth > textRectangle.Width - 10)
                        {
                            newLine = line.Substring(0, line.Length - j);
                            textSize = g.MeasureString(newLine, graphSetting.GraphElementTextFont);
                            currentWidth = textSize.Width * 2;
                            j++;
                        }

                        j--;
                        newData.Add(newLine);
                        line = "    " + line.Substring(line.Length - j, j);
                        textSize = g.MeasureString(line, graphSetting.GraphElementTextFont);
                        currentWidth = textSize.Width * 2;
                    }
                }

                newData.Add(line);
            }

            foreach (string s in newData)
            {
                displayText += s + "\n";
            }
            displayText = displayText.TrimEnd(new char[] { '\n' });
            textSize = g.MeasureString(displayText, graphSetting.GraphElementTextFont);
            textWidth = textSize.Width;
            textHeight = textSize.Height;

            if (textHeight * 2 + 10 > elementSize.Height) // �����߶�
            {
                elementSize.Height = (int)(textHeight * 2 + 10);
                textRectangle.Size = new Size(textRectangle.Width, elementSize.Height);
                shadowRectangle.Size = new Size(textRectangle.Width, elementSize.Height);
                invalidRectangle.Size = new Size(textRectangle.Width + 20, elementSize.Height + 45);
            }

            int textX = (int)(textRectangle.X + (textRectangle.Width - textWidth) / 2);
            int textY = (int)(textRectangle.Y + (textRectangle.Height - textHeight) / 2);
            textLocation.X = textX;
            textLocation.Y = textY;
        }

        /// <summary>
        /// ˢ��ͼԪ��ͼƬ
        /// </summary>
        protected override void RefreshImages()
        {
            if (normalImage == null)
            {
                normalImage = graphSetting.ConditionNodeNormalImage;
            }

            if (selectedImage == null)
            {
                selectedImage = graphSetting.ConditionNodeSelectedImage;
            }

            if (activatedImage == null)
            {
                activatedImage = graphSetting.ConditionNodeActivatedImage;
            }

            if (shadowImage == null)
            {
                shadowImage = graphSetting.ConditionNodeShadowImage;
            }

            if (abbreviateImage == null)
            {
                abbreviateImage = graphSetting.ConditionNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public override SlotContainer Clone()
        {
            ConditionGraphElement conditionGraphElement = new ConditionGraphElement(location, elementSize);
            CopyData(this, conditionGraphElement);             

            return conditionGraphElement;
        }
    }
}
