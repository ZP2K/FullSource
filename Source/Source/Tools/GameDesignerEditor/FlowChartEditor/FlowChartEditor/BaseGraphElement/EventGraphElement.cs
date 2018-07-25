using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using FlowChartEditor.ButtonGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class EventGraphElement:SlotContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="id">ͼԪ�Ĵ�С</param>
        public EventGraphElement(Point location, Size size)
            : base(location, size)
        {
            inSlotCount = 0;
            outSlotCount = 1;
            enable = true;
            isEntrance = true;
        }

        /// <summary>
        /// �����ַ���
        /// </summary>
        public override string TypeString
        {
            get
            {
                return "EventNode";
            }
        }

        /// <summary>
        /// ��ڲ�����������
        /// </summary>
        [Browsable(false)]
        public override int InSlotCount
        {
            get
            {
                return inSlotCount;
            }
        }

        /// <summary>
        /// ��ʼ�����Ĳ��
        /// </summary>
        /// <param name="size">��ǰͼԪ�Ĵ�С</param>
        protected override void InitSlot(Size size)
        {
            Helper helper = Helper.GetHelper();
            int x1 = location.X + (int)(size.Width / 2);
            int y1 = location.Y + size.Height;

            SlotGraphElement slot1 = new SlotGraphElement(this, new Point(x1, y1), new Size(6, 6));
            slot1.Name = "���Ӳ��";
            slot1.IsOutSlot = true;
            slot1.CanDelete = false;

            ConnectButton button = new ConnectButton(slot1, new Point(x1 - 6, y1 + 25), new Size(12, 8));
            button.Name = "���Ӱ�ť";
            slot1.BindingConnectButton = button;  

            outSlotList.Add(slot1);
            buttonList.Add(button);
            slotList.Add(slot1);

            // ���������ȼ�
            ResetSlotProperty(inSlotList);
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
            if (textWidth < textRectangle.Width - 12)
            {
                if (textWidth < minSize.Width - 12)
                {
                    newWidth = minSize.Width;
                }
                else
                {
                    newWidth = (int)(textWidth + 12);
                }
            }
            else
            {
                newWidth = (int)(textWidth + 12);
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

                if(newHeight > minSize.Height * 2)
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
                currentWidth = textSize.Width;
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
                            currentWidth = textSize.Width;
                            j++;
                        }

                        j--;
                        newData.Add(newLine);
                        line = "    " + line.Substring(line.Length - j, j);
                        textSize = g.MeasureString(line, graphSetting.GraphElementTextFont);
                        currentWidth = textSize.Width;
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
            int textY = (int)(textRectangle.Y + ((float)textRectangle.Height / 2 - textHeight) / 2);
            textLocation.X = textX;
            textLocation.Y = textY;
        }

        /// <summary>
        /// ������ڲ��
        /// </summary>
        /// <param name="n">�������</param>
        protected override void AdjustInSlotList(int n)
        {

        }

        /// <summary>
        /// ��ȡδ�󶨵���ڲ��
        /// </summary>
        /// <returns>��ڲ��</returns>
        public override SlotGraphElement GetInSlot()
        {
            return null;
        }

        /// <summary>
        /// ������ڲ��
        /// </summary>
        /// <returns>�����Ĳ��</returns>
        public override SlotGraphElement AddInSlot()
        {
            MessageBox.Show("�¼�����ϲ���������ڲ�ۣ�", "����ͼԪ", MessageBoxButtons.OK, MessageBoxIcon.Information);
            return null;
        }

        /// <summary>
        /// ˢ��ͼԪ��ͼƬ
        /// </summary>
        protected override void RefreshImages()
        {
            if (normalImage == null)
            {
                normalImage = graphSetting.EventNodeNormalImage;
            }

            if (selectedImage == null)
            {
                selectedImage = graphSetting.EventNodeSelectedImage;
            }

            if (activatedImage == null)
            {
                activatedImage = graphSetting.EventNodeActivatedImage;
            }

            if (shadowImage == null)
            {
                shadowImage = graphSetting.EventNodeShadowImage;
            }

            if (abbreviateImage == null)
            {
                abbreviateImage = graphSetting.EventNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public override SlotContainer Clone()
        {
            EventGraphElement eventGraphElement = new EventGraphElement(location, elementSize);
            CopyData(this, eventGraphElement);            

            return eventGraphElement;
        }
    }
}
