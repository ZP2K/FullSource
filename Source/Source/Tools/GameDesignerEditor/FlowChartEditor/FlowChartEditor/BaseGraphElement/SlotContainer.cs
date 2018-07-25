using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Windows.Forms;
using System.ComponentModel;

using FlowChartEditor.ButtonGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class SlotContainer:GraphElement
    {
        protected List<SlotGraphElement> inSlotList = new List<SlotGraphElement>(); // ��ڲ������
        protected List<SlotGraphElement> outSlotList = new List<SlotGraphElement>(); // ���ڲ������
        protected List<SlotGraphElement> slotList = new List<SlotGraphElement>(); // �������
        protected List<ButtonGraphElement.BaseButton> buttonList = new List<ButtonGraphElement.BaseButton>(); // ��ť����
        protected List<ResizeControler> resizeControlerList = new List<ResizeControler>(); // ���ſ��Ƶ�����
        protected AddButton addButton; // ��Ӱ�ť
        protected int inSlotCount; // ��ڲ�۵�����
        protected int outSlotCount; // ���ڲ�۵�����
        protected bool resizing = false; // �Ƿ���������״̬
        protected RemarkGraphElement remarkNode; // ���������Ӧ��ע�ͽ��        
        protected Point textLocation = Point.Empty; // �����ı���λ��   
        protected bool requireEvent = false; // �Ƿ���Ҫ����¼�
        protected bool isEntrance = false; // �Ƿ�����ڽ��

        [NonSerialized]
        protected Image normalImage; // ��ͨ״̬�µ�ͼƬ
        [NonSerialized]
        protected Image selectedImage; // ��ѡ��״̬�µ�ͼƬ
        [NonSerialized]
        protected Image activatedImage; // ������״̬�µ�ͼƬ
        [NonSerialized]
        protected Image shadowImage; // ��ӰͼƬ
        [NonSerialized]
        protected Image abbreviateImage; // ���Ե�ͼƬ

        [NonSerialized]
        protected Rectangle textRectangle = Rectangle.Empty; // ͼԪ��ǰ����
        [NonSerialized]
        protected Rectangle shadowRectangle = Rectangle.Empty; // ͼԪ��Ӱ����

        [NonSerialized]
        protected GraphSetting graphSetting; // ��ͼ�������ö���

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="elementSize">ͼԪ�Ĵ�С</param>
        public SlotContainer(Point location, Size elementSize)
                            : base(location)
        {
            this.elementSize = elementSize;
            this.minSize = elementSize;
        }

        /// <summary>
        /// �Ƿ�����ڽ��
        /// </summary>
        [Browsable(false)]
        public virtual bool IsEntrance
        {
            set
            {
                isEntrance = value;
            }

            get
            {
                return isEntrance;
            }
        }

        /// <summary>
        /// ������ӰЧ����ͼƬ����
        /// </summary>
        [Browsable(false)]
        public virtual Image ShadowImage
        {
            set
            {
                shadowImage = value;
            }
        }

        /// <summary>
        /// ��ͨ״̬�µ�ͼƬ����
        /// </summary>
        [Browsable(false)]
        public virtual Image NormalImage
        {
            set
            {
                normalImage = value;
            }
        }

        /// <summary>
        /// ��ѡ��״̬�µ�ͼƬ����
        /// </summary>
        [Browsable(false)]
        public virtual Image SelectedImage
        {
            set
            {
                selectedImage = value;
            }
        }

        /// <summary>
        /// ������״̬�µ�ͼƬ����
        /// </summary>
        [Browsable(false)]
        public virtual Image ActivatedImage
        {
            set
            {
                activatedImage = value;
            }
        }

        /// <summary>
        /// ͼԪ���������
        /// </summary>
        [Browsable(false)]
        public virtual Rectangle TextRectangle
        {
            get
            {
                return textRectangle;
            }
        }

        /// <summary>
        /// �Ƿ���Ҫ����¼�
        /// </summary>
        [Browsable(false)]
        public virtual bool RequireEvent
        {
            set
            {
                requireEvent = value;
            }

            get
            {
                return requireEvent;
            }
        }

        /// <summary>
        /// ͼԪ��ˢ����������
        /// </summary>
        [Browsable(false)]
        public override Rectangle InvalidRectangle
        {
            get
            {               
                return invalidRectangle;
            }
        }

        /// <summary>
        /// ͼԪ����ʾ�ı�����
        /// </summary>
        [Browsable(false)]
        public override string Text
        {            
            set
            {
                displayText = value;
                text = value;
            }

            get
            {
                return text;
            }
        }

        /// <summary>
        /// ͼԪ��ע������
        /// </summary>
        [Browsable(false)]
        public override string Remark
        {
            get
            {
                return remark;
            }

            set
            {
                remark = value;

                if (remarkNode != null) // �ı�ע�ͽ����ı�
                {
                    remarkNode.Text = value;
                }
            }
        }

        /// <summary>
        /// ���������Ӧ��ע�ͽ������
        /// </summary>
        [Browsable(false)]
        public virtual RemarkGraphElement RemarkNode
        {
            set
            {
                remarkNode = value;
            }

            get
            {
                return remarkNode;
            }
        }

        /// <summary>
        /// �Ƿ���������״̬����
        /// </summary>
        [Browsable(false)]
        public bool Resizing
        {
            set
            {
                resizing = value;
            }

            get
            {
                return resizing;
            }
        }

        /// <summary>
        /// ͼԪ��λ������
        /// </summary>
        [Browsable(false)]
        public override Point Location
        {
            set
            {
                location = value;
                textRectangle.Location = value;
                shadowRectangle.Location = value + new Size(4, 4);
                invalidRectangle.Location = value + new Size(-10, -10);
            }

            get
            {
                return location;
            }
        }

        /// <summary>
        /// ͼԪ��С����
        /// </summary>
        [Browsable(false)]
        public override Size ElementSize
        {
            set
            {
                elementSize = value;
                textRectangle.Size = value;
                shadowRectangle.Size = value;
                invalidRectangle.Size = value + new Size(20, 45);
                AdjustText(); // ������ʾ�ı�
                AdjustInSlotList(inSlotCount); // ������ڲ������
                AdjustOutSlotList(outSlotCount); // �������ڲ������
                AdjustResizeControlerList(); // �������ſ��Ƶ�����
                AdjustButtonList(); // ������ť����                
            }

            get
            {
                return elementSize;
            }
        }

        /// <summary>
        /// ��ڲ�����������
        /// </summary>
        [Browsable(false)]
        public virtual int InSlotCount
        {
            set
            {
                AdjustInSlotList(value);
                inSlotCount = value;                
            }

            get
            {
                return inSlotCount;
            }
        }

        /// <summary>
        /// ��ʼ��ͼԪ
        /// </summary>
        public void Init()
        {
            InitSlot(elementSize); // ��ʼ�����Ĳ��
            InitResizeControler(elementSize); // ��ʼ���������ſ��Ƶ�
            InitButton(); // ��ʼ����ť

            // ˢ������
            Refresh();
        }

        /// <summary>
        /// ��ʼ����ť
        /// </summary>
        protected virtual void InitButton()
        {
            Helper helper = Helper.GetHelper();
            int x = location.X + elementSize.Width - 12;
            int y = location.Y + elementSize.Height + 22;
            AddButton button = new AddButton(this, new Point(x, y), new Size(12, 12));
            button.Name = "��Ӳ�۰�ť";
            this.addButton = button;
            buttonList.Add(button);
        }

        /// <summary>
        /// ������ť����
        /// </summary>
        protected virtual void AdjustButtonList()
        {
            if (addButton != null)
            {
                int x = location.X + elementSize.Width - 12;
                int y = location.Y + elementSize.Height + 22;
                addButton.Location = new Point(x, y);
            }            
        }

        /// <summary>
        /// �������ſ��Ƶ�����
        /// </summary>
        protected virtual void AdjustResizeControlerList()
        {
            int index = 0;

            // �������Ͻǵ����ſ��Ƶ������
            int newX = location.X - 10;
            int newY = location.Y - 10;
            Point newPoint = new Point(newX, newY);
            ResizeControler resizeControler = resizeControlerList[index];
            resizeControler.Location = newPoint;
            index++;

            // ��ʼ����ߵ����ſ��Ƶ�
            newX = location.X - 10;
            newY = location.Y + (int)(elementSize.Height / 2);
            newPoint = new Point(newX, newY);
            resizeControler = resizeControlerList[index];
            resizeControler.Location = newPoint;
            index++;

            // ��ʼ�����½ǵ����ſ��Ƶ�
            newX = location.X - 10;
            newY = location.Y + elementSize.Height + 10;
            newPoint = new Point(newX, newY);
            resizeControler = resizeControlerList[index];
            resizeControler.Location = newPoint;
            index++;

            // ��ʼ���±ߵ����ſ��Ƶ�
            newX = location.X + (int)(elementSize.Width / 2);
            newY = location.Y + elementSize.Height + 10;
            newPoint = new Point(newX, newY);
            resizeControler = resizeControlerList[index];
            resizeControler.Location = newPoint;
            index++;

            // ��ʼ�����½ǵ����ſ��Ƶ�
            newX = location.X + elementSize.Width + 10;
            newY = location.Y + elementSize.Height + 10;
            newPoint = new Point(newX, newY);
            resizeControler = resizeControlerList[index];
            resizeControler.Location = newPoint;
            index++;

            // ��ʼ���ұߵ����ſ��Ƶ�
            newX = location.X + elementSize.Width + 10;
            newY = location.Y + (int)(elementSize.Height / 2);
            newPoint = new Point(newX, newY);
            resizeControler = resizeControlerList[index];
            resizeControler.Location = newPoint;
            index++;

            // ��ʼ�����Ͻǵ����ſ��Ƶ�
            newX = location.X + elementSize.Width + 10;
            newY = location.Y - 10;
            newPoint = new Point(newX, newY);
            resizeControler = resizeControlerList[index];            
            resizeControler.Location = newPoint;            
            index++;

            // ��ʼ���ϱߵ����ſ��Ƶ�
            newX = location.X + (int)(elementSize.Width / 2);
            newY = location.Y - 10;
            newPoint = new Point(newX, newY);
            resizeControler = resizeControlerList[index];            
            resizeControler.Location = newPoint;            
        }

        /// <summary>
        /// ���ڲ�����������
        /// </summary>
        [Browsable(false)]
        public virtual int OutSlotCount
        {
            set
            {
                AdjustOutSlotList(value);
                outSlotCount = value;                
            }

            get
            {
                return outSlotCount;
            }
        }

        /// <summary>
        /// ��ʼ���������ſ��Ƶ�
        /// </summary>
        /// <param name="size">��ǰͼԪ�Ĵ�С</param>
        protected virtual void InitResizeControler(Size size)
        {
            // ��ʼ�����Ͻǵ����ſ��Ƶ�
            int newX = location.X - 10;
            int newY = location.Y - 10;
            Point newPoint = new Point(newX, newY);
            ResizeControler newControler = new ResizeControler(this, newPoint, new Size(4, 4));
            newControler.CurrentDirection = Direction.Up_Left;
            resizeControlerList.Add(newControler);

            // ��ʼ����ߵ����ſ��Ƶ�
            newX = location.X - 10;
            newY = location.Y + (int)(elementSize.Height / 2);
            newPoint = new Point(newX, newY);
            newControler = new ResizeControler(this, newPoint, new Size(4, 4));
            newControler.CurrentDirection = Direction.Left;
            resizeControlerList.Add(newControler);

            // ��ʼ�����½ǵ����ſ��Ƶ�
            newX = location.X - 10;
            newY = location.Y + elementSize.Height + 10;
            newPoint = new Point(newX, newY);
            newControler = new ResizeControler(this, newPoint, new Size(4, 4));
            newControler.CurrentDirection = Direction.Down_Left;
            resizeControlerList.Add(newControler); 

            // ��ʼ���±ߵ����ſ��Ƶ�
            newX = location.X + (int)(elementSize.Width / 2);
            newY = location.Y + elementSize.Height + 10;
            newPoint = new Point(newX, newY);
            newControler = new ResizeControler(this, newPoint, new Size(4, 4));
            newControler.CurrentDirection = Direction.Down;
            resizeControlerList.Add(newControler); 

            // ��ʼ�����½ǵ����ſ��Ƶ�
            newX = location.X + elementSize.Width + 10;
            newY = location.Y + elementSize.Height + 10;
            newPoint = new Point(newX, newY);
            newControler = new ResizeControler(this, newPoint, new Size(4, 4));
            newControler.CurrentDirection = Direction.Down_Right;
            resizeControlerList.Add(newControler); 

            // ��ʼ���ұߵ����ſ��Ƶ�
            newX = location.X + elementSize.Width + 10;
            newY = location.Y + (int)(elementSize.Height / 2);
            newPoint = new Point(newX, newY);
            newControler = new ResizeControler(this, newPoint, new Size(4, 4));
            newControler.CurrentDirection = Direction.Right;
            resizeControlerList.Add(newControler); 

            // ��ʼ�����Ͻǵ����ſ��Ƶ�
            newX = location.X + elementSize.Width + 10;
            newY = location.Y - 10;
            newPoint = new Point(newX, newY);
            newControler = new ResizeControler(this, newPoint, new Size(4, 4));
            newControler.CurrentDirection = Direction.Up_Right;
            resizeControlerList.Add(newControler); 

            // ��ʼ���ϱߵ����ſ��Ƶ�
            newX = location.X + (int)(elementSize.Width / 2);
            newY = location.Y - 10;
            newPoint = new Point(newX, newY);
            newControler = new ResizeControler(this, newPoint, new Size(4, 4));
            newControler.CurrentDirection = Direction.Up;
            resizeControlerList.Add(newControler); 
        }

        /// <summary>
        /// ��ʼ�����Ĳ��
        /// </summary>
        /// <param name="Size">��ǰͼԪ�Ĵ�С</param>
        protected virtual void InitSlot(Size size)
        {
            int x1 = location.X + (int)(size.Width / 2);
            int y1 = location.Y;
            int x2 = x1;
            int y2 = location.Y + size.Height;

            SlotGraphElement slot1 = new SlotGraphElement(this, new Point(x1, y1), new Size(6, 6));
            SlotGraphElement slot2 = new SlotGraphElement(this, new Point(x2, y2), new Size(6, 6));

            slot1.Name = "���Ӳ��";        
            slot1.IsInSlot = true;

            slot2.Name = "���Ӳ��";
            slot2.IsOutSlot = true;

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
        /// ���������ȼ�
        /// </summary>
        protected virtual void ResetSlotProperty(List<SlotGraphElement> slotGraphElementList)
        {
            int property = 0;

            foreach (SlotGraphElement slot in slotGraphElementList)
            {
                slot.Priority = property;
                property++;
            }
        }

        /// <summary>
        /// ������ڲ������
        /// </summary>
        /// <param name="n">�������</param>
        protected virtual void AdjustInSlotList(int n)
        {
            if (n == inSlotList.Count) // ����Ҫ�������
            {
                int newX;
                Point newPoint;

                // �������������
                for (int i = 0; i < inSlotList.Count; i++)
                {
                    newX = (int)(location.X + elementSize.Width * (i + 1) / (n + 1));
                    newPoint = new Point(newX, location.Y);
                    inSlotList[i].Location = newPoint;

                    if (inSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                    {
                        inSlotList[i].BindingConnector.Location = newPoint;
                    }
                }
            }
            else if (n < inSlotList.Count) // ��Ҫ���ٲ������
            {
                int deleteCount = inSlotList.Count - n;
                int newX;
                Point newPoint;
                List<SlotGraphElement> deleteSlotList = new List<SlotGraphElement>();

                for (int i = 0; i < deleteCount; i++)
                {
                    deleteSlotList.Add(inSlotList[inSlotList.Count - 1 - i]);
                }

                foreach (SlotGraphElement slot in deleteSlotList)
                {
                    if (slot.Binded)
                    {
                        slot.UnBind();
                    }

                    inSlotList.Remove(slot);
                    slotList.Remove(slot);
                }

                // �������������
                for (int i = 0; i < inSlotList.Count; i++)
                {
                    newX = (int)(location.X + elementSize.Width * (i + 1) / (n + 1));
                    newPoint = new Point(newX, location.Y);
                    inSlotList[i].Location = newPoint;

                    if (inSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                    {
                        inSlotList[i].BindingConnector.Location = newPoint;
                    }
                }
            }
            else // ��Ҫ���Ӳ������
            {
                int oldCount = inSlotList.Count;
                SlotGraphElement newSlot;
                Point newPoint;
                int newX;

                // ��Ӳ�۲�������۵�����
                for (int i = 0; i < n; i++)
                {
                    if (i < oldCount)
                    {
                        newX = (int)(location.X + elementSize.Width * (i + 1) / (n + 1));
                        newPoint = new Point(newX, location.Y);
                        inSlotList[i].Location = newPoint;

                        if (inSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                        {
                            inSlotList[i].BindingConnector.Location = newPoint;
                        }
                    }
                    else
                    {
                        newX = (int)(location.X + elementSize.Width * (i + 1) / (n + 1));
                        newPoint = new Point(newX, location.Y);
                        newSlot = new SlotGraphElement(this, newPoint, new Size(6, 6));
                        newSlot.Name = "���Ӳ��";
                        newSlot.IsInSlot = true;
                        newSlot.CanDelete = true;
                        newSlot.Refresh();

                        inSlotList.Add(newSlot);
                        slotList.Add(newSlot);
                    }
                }
            }

            ResetSlotProperty(inSlotList);
        }

        /// <summary>
        /// �������ڲ������
        /// </summary>
        /// <param name="n">�������</param>
        protected virtual void AdjustOutSlotList(int n)
        {
            if (n == outSlotList.Count) // ����Ҫ�������
            {
                int newX;
                Point newPoint;

                // �������������
                for (int i = 0; i < outSlotList.Count; i++)
                {
                    newX = (int)(location.X + elementSize.Width * (i + 1) / (n + 1));
                    newPoint = new Point(newX, location.Y + elementSize.Height);
                    outSlotList[i].Location = newPoint;
                    outSlotList[i].BindingConnectButton.Location = new Point(newX - 6, location.Y + elementSize.Height + 25);

                    if (outSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                    {
                        outSlotList[i].BindingConnector.Location = newPoint;
                    }
                }
            }
            else if (n < outSlotList.Count) // ��Ҫ���ٲ������
            {
                int deleteCount = outSlotList.Count - n;
                int newX;
                Point newPoint;
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
                for (int i = 0; i < outSlotList.Count; i++)
                {
                    newX = (int)(location.X + elementSize.Width * (i + 1) / (n + 1));
                    newPoint = new Point(newX, location.Y + elementSize.Height);
                    outSlotList[i].Location = newPoint;
                    outSlotList[i].BindingConnectButton.Location = new Point(newX - 6, location.Y + elementSize.Height + 25);

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
                Point newPoint;
                int newX;

                // ��Ӳ�۲�������۵�����
                for (int i = 0; i < n; i++)
                {
                    if (i < oldCount)
                    {
                        newX = (int)(location.X + elementSize.Width * (i + 1) / (n + 1));
                        newPoint = new Point(newX, location.Y + elementSize.Height);
                        outSlotList[i].Location = newPoint;
                        outSlotList[i].BindingConnectButton.Location = new Point(newX - 6, location.Y + elementSize.Height + 25);

                        if (outSlotList[i].Binded) // �����󶨵������߿��Ƶ�
                        {
                            outSlotList[i].BindingConnector.Location = newPoint;
                        }
                    }
                    else
                    {
                        newX = (int)(location.X + elementSize.Width * (i + 1) / (n + 1));
                        newPoint = new Point(newX, location.Y + elementSize.Height);
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
        /// ��ȡ�������߰󶨵���ڲ��
        /// </summary>
        /// <param name="line">������</param>
        /// <returns>��ڲ��</returns>
        public SlotGraphElement GetBindedInSlot(ConnectorContainer line)
        {
            SlotGraphElement slotGraphElement = null;

            foreach (SlotGraphElement slot in inSlotList)
            {
                if (slot.Binded && slot.BindingConnector.Line == line)
                {
                    slotGraphElement = slot;
                    break;
                }
            }

            return slotGraphElement;
        }

        /// <summary>
        /// ��ȡ�������߰󶨵���ڲ�۵�����
        /// </summary>
        /// <param name="line">������</param>
        /// <returns>����</returns>
        public int GetBindedInSlotIndex(ConnectorContainer line)
        {
            int index = -1;
            SlotGraphElement slot = GetBindedInSlot(line);

            if (slot != null)
            {
                index = slot.Priority;
            }

            return index;
        }

        /// <summary>
        /// ��ȡ�������߰󶨵ĳ��ڲ��
        /// </summary>
        /// <param name="line">������</param>
        /// <returns>���ڲ��</returns>
        public SlotGraphElement GetBindedOutSlot(ConnectorContainer line)
        {
            SlotGraphElement slotGraphElement = null;

            foreach (SlotGraphElement slot in outSlotList)
            {
                if (slot.Binded && slot.BindingConnector.Line == line)
                {
                    slotGraphElement = slot;
                    break;
                }
            }

            return slotGraphElement;
        }

        /// <summary>
        /// ��ȡ�������߰󶨵ĳ��ڲ�۵�����
        /// </summary>
        /// <param name="line">������</param>
        /// <returns>����</returns>
        public int GetBindedOutSlotIndex(ConnectorContainer line)
        {
            int index = -1;
            SlotGraphElement slot = GetBindedOutSlot(line);

            if (slot != null)
            {
                index = slot.Priority;
            }

            return index;
        }

        /// <summary>
        /// �ж�ĳ�����Ƿ��ڸ�ͼԪ��������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�õ��Ƿ���ͼԪ��������</returns>
        public override bool IsInRegion(Point p)
        {
            bool result = textRectangle.Contains(p);
            return result;
        }

        /// <summary>
        /// �ж�ͼԪ�Ƿ���ĳ��������
        /// </summary>
        /// <param name="r">��ǰ����</param>
        /// <returns>ͼԪ�Ƿ��ڵ�ǰ������</returns>
        public override bool IsInRegion(Rectangle r)
        {
            bool result  = r.Contains(textRectangle);
            return result;            
        }

        /// <summary>
        /// ˢ��ͼԪ��ͼƬ
        /// </summary>
        protected virtual void RefreshImages()
        {
            
        }

        /// <summary>
        /// �Զ�����ͼԪ�Ĵ�С
        /// </summary>
        public virtual void AdjustElementSize()
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

            if (textHeight < textRectangle.Height - 12)
            {
                if (textHeight < minSize.Height - 12)
                {
                    newHeight = minSize.Height;
                }
                else
                {
                    newHeight = (int)(textHeight + 12);
                }
            }
            else
            {
                newHeight = (int)(textHeight + 12);
            }

            // ����ͼԪ��С
            if (newWidth != elementSize.Width || newHeight != elementSize.Height)
            {
                if(newWidth > minSize.Width * 2)
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
        public virtual void AdjustText()
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

            foreach(string s in newData)
            {
                displayText += s + "\n";
            }
            displayText = displayText.TrimEnd(new char[] { '\n' });
            textSize = g.MeasureString(displayText, graphSetting.GraphElementTextFont);
            textWidth = textSize.Width;
            textHeight = textSize.Height;

            if(textHeight + 10 > elementSize.Height) // �����߶�
            {
                elementSize.Height = (int)(textHeight + 10);
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
        /// ˢ��ͼԪ���Ʋ���
        /// </summary>
        public override void Refresh()
        {
            // ��ʼ����ͼ���ݹ������
            graphSetting = GraphSetting.GetGraphSetting();

            // ˢ�±����ı�
            displayText = text;

            // ˢ���߼�����
            textRectangle.Location = location;
            textRectangle.Size = elementSize;
            shadowRectangle.Location = location + new Size(4, 4);
            shadowRectangle.Size = elementSize;
            invalidRectangle = new Rectangle(location + new Size(-10, -10), elementSize + new Size(20, 45));
            inSlotCount = inSlotList.Count;
            outSlotCount = outSlotList.Count;
           
            requireEvent = true;

            // ˢ��ͼƬ
            RefreshImages();

            // ˢ�²��
            foreach(SlotGraphElement slot in slotList)
            {
                slot.Refresh();
            }

            // ���������ȼ�
            ResetSlotProperty(inSlotList);
            ResetSlotProperty(outSlotList);

            // ˢ�°�ť
            foreach(ButtonGraphElement.BaseButton button in buttonList)
            {
                button.Refresh();
            }

            // ˢ�����ſ��Ƶ�
            foreach(ResizeControler resizeControler in resizeControlerList)
            {
                resizeControler.Refresh();
            }

            // ˢ��ע�ͽ��
            if(remarkNode != null)
            {
                remarkNode.Refresh();
            }
        }

        /// <summary>
        /// �ͷ�ͼԪ�Ļ�ͼ��Դ
        /// </summary>
        public override void Release()
        {
            
        } 

        /// <summary>
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void Paint(Graphics g)
        {
            if (visible)
            {
                bool sizeChanged = (this.elementSize != graphSetting.AIActionNodeElementSize);

                if (moving) // �ƶ�ʱֻ���ƾ��ο�
                {
                    // ����ͼƬ
                    if (sizeChanged)
                    {
                        g.DrawImage(normalImage, textRectangle);
                    }
                    else
                    {
                        g.DrawImage(normalImage, location);
                    }
                }
                else
                {
                    // ����ͼƬ��Ӱ                    
                    if (sizeChanged)
                    {
                        g.DrawImage(shadowImage, shadowRectangle);
                    }
                    else
                    {
                        g.DrawImage(shadowImage, shadowRectangle.Location);
                    }                    

                    // ����ͼƬ                       
                    Image image;

                    if (activated) // ��ǰͼԪ������
                    {
                        image = activatedImage;                        
                    }
                    else if (selected) // ��ǰͼԪ��ѡ��
                    {
                        image = selectedImage;
                    }
                    else // ��ǰͼԪû�б�ѡ�кͼ���
                    {
                        image = normalImage;
                    }                    

                    // ����ͼƬ                    
                    if (sizeChanged)
                    {
                        g.DrawImage(image, textRectangle);
                    }
                    else
                    {
                        g.DrawImage(image, location);
                    }
                    

                    // �����ı�
                    if (showText)
                    {
                        if (selected)
                        {
                            g.DrawString(displayText, graphSetting.GraphElementTextBoldFont, graphSetting.GraphElementTextSelectedBrush, textLocation);
                        }
                        else
                        {
                            g.DrawString(displayText, graphSetting.GraphElementTextFont, graphSetting.GraphElementTextBrush, textLocation);
                        }
                        
                    }

                    // ����ID
                    if (showID)
                    {
                        g.DrawString(id.ToString(), graphSetting.GraphElementTextBoldFont, Brushes.Black, location.X - 50, location.Y);
                    }

                    // ���Ʋ���
                    foreach (SlotGraphElement slotGraphElement in slotList)
                    {
                        slotGraphElement.Paint(g);
                    }

                    // �������ſ��Ƶ�
                    if (resizing)
                    {
                        foreach (ResizeControler resizeControler in resizeControlerList)
                        {
                            resizeControler.Paint(g);
                        }

                        // ���ư�ť
                        if (addButton != null)
                        {
                            addButton.Paint(g);
                        }
                    }

                    // ����ע�ͽ��
                    if (showRemark && remarkNode != null)
                    {
                        remarkNode.Paint(g);
                    }
                }
            }
        }

        /// <summary>
        /// ����ͼԪ������Ч��
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void AbbreviativePaint(Graphics g)
        {
            g.DrawImage(abbreviateImage, textRectangle);
        }

        /// <summary>
        /// �ƶ�ͼԪ
        /// </summary>
        /// <param name="s">�ƶ������λ��</param>
        public override void Move(Size s)
        {
            location += s;
            textRectangle.Location += s;
            shadowRectangle.Location += s;
            invalidRectangle.Location += s;

            // �ƶ��ı�λ��
            textLocation += s;

            // �ƶ�����ϵĲ��
            foreach (SlotGraphElement slotGraphElement in slotList)
            {
                slotGraphElement.Move(s);
            }

            // �ƶ�����ϵ����ſ��Ƶ�
            foreach(ResizeControler resizeControler in resizeControlerList)
            {
                resizeControler.Move(s);        
            }

            // �ƶ�����ϵİ�ť
            if(addButton != null)
            {
                addButton.Move(s);
            }
         
            // �ƶ�����ע��ͼԪ
            if(remarkNode != null)
            {
                remarkNode.Move(s);
            }
        }

        /// <summary>
        /// �ƶ�ͼԪ�߿�
        /// </summary>
        /// <param name="locationDistancement">����λ��</param>
        /// <param name="elementSizeDistancement">��Сλ��</param>
        public virtual void MoveBorder(Size locationDistancement, Size elementSizeDistancement)
        {
            this.location += locationDistancement;
            textRectangle.Location += locationDistancement;
            shadowRectangle.Location += locationDistancement;
            invalidRectangle.Location += locationDistancement;
            this.ElementSize += elementSizeDistancement;
        }

        /// <summary>
        /// ��ȡ�������
        /// </summary>
        /// <returns>�������</returns>
        public virtual List<SlotGraphElement> GetSlotList()
        {
            return slotList;
        }

        /// <summary>
        /// ��ȡ��ڲ������
        /// </summary>
        /// <returns>��ڲ������</returns>
        public virtual List<SlotGraphElement> GetInSlotList()
        {
            return inSlotList;
        }

        /// <summary>
        /// ��ȡ���ڲ������
        /// </summary>
        /// <returns>���ڲ������ </returns>
        public virtual List<SlotGraphElement> GetOutSlotList()
        {
            return outSlotList;
        }

        /// <summary>
        /// ��ȡָ�������ڵ����ſ��Ƶ�
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>���ſ��Ƶ�</returns>
        public virtual ResizeControler GetResizeControlerInRegion(Point p)
        {
            ResizeControler resizeControler = null;

            foreach (ResizeControler controler in resizeControlerList)
            {
                if (controler.IsInRegion(p))
                {
                    resizeControler = controler;
                    break;
                }
            }

            return resizeControler;
        }

        /// <summary>
        /// ��ȡ���ſ��Ƶ�����
        /// </summary>
        /// <returns>���ſ��Ƶ�����</returns>
        public virtual List<ResizeControler> GetResizeControlerList()
        {
            return resizeControlerList;
        }

        /// <summary>
        /// ��ȡ�����ڵİ�ť
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>��ť</returns>
        public BaseButton GetButtonInRegion(Point p)
        {
            BaseButton button = null;

            foreach (BaseButton baseButton in buttonList)
            {
                if (baseButton.IsInRegion(p))
                {
                    button = baseButton;
                    break;
                }
            }

            return button;
        }

        /// <summary>
        /// ��ȡ��ť����
        /// </summary>
        /// <returns>��ť����</returns>
        public virtual List<ButtonGraphElement.BaseButton> GetButtonList()
        {
            return buttonList;
        }

        /// <summary>
        /// ȡ����������
        /// </summary>
        /// <param name="line">������</param>
        public virtual void UnBindLine(ConnectorContainer line)
        {
            SlotGraphElement deleteSlot = null;

            foreach(SlotGraphElement slot in slotList)
            {
                if(slot.Binded && ((slot.BindingConnector == line.HeadConnector) || slot.BindingConnector == line.TailConnector))
                {
                    slot.UnBind();
                    deleteSlot = slot;
                    break;
                }
            }

            if (deleteSlot != null && deleteSlot.CanDelete) // ��ۿ���ɾ��
            {
                if(deleteSlot.IsInSlot || outSlotCount > 1)
                {
                    RemoveSlot(deleteSlot);
                }                
            }
        }

        /// <summary>
        /// ������ڲ��
        /// </summary>
        /// <returns>�����Ĳ��</returns>
        public virtual SlotGraphElement AddInSlot()
        {            
            InSlotCount = inSlotCount + 1;
            SlotGraphElement newSlot = inSlotList[inSlotCount - 1];            

            return newSlot;
        }

        /// <summary>
        /// ��ȡ��ڲ��
        /// </summary>
        /// <returns>��ڲ��</returns>
        public virtual SlotGraphElement GetInSlot()
        {
            bool find = false; // �Ƿ��Ѿ��ҵ�û�а������߿��Ƶ�Ĳ��
            int priority = -1;
            SlotGraphElement newSlot = null;            

            foreach (SlotGraphElement slot in inSlotList)
            {
                if (!slot.Binded)
                {
                    if(slot.Priority < priority || priority == -1)
                    {
                        newSlot = slot;
                        priority = slot.Priority;
                    }                    
                    find = true;
                }
            }

            if (!find) // û���ҵ�û�а������߿��Ƶ�Ĳ�ۣ����½�һ����ڲ��
            {
                InSlotCount = inSlotCount + 1;
                newSlot = inSlotList[inSlotCount - 1];
            }

            return newSlot;
        }

        /// <summary>
        /// �������������
        /// </summary>
        /// <param name="line">������</param>
        public virtual void BindInLine(ConnectorContainer line)
        {
            SlotGraphElement slot = GetInSlot();

            if (slot != null)
            {
                slot.Bind(line.HeadConnector);
            }
        }

        /// <summary>
        /// �������������
        /// </summary>
        /// <param name="line">������</param>
        /// <param name="index">����</param>
        public virtual void BindInLine(ConnectorContainer line, int index)
        {            
            if (index >= 0 && index < inSlotCount)
            {
                SlotGraphElement slot = inSlotList[index];

                if (!slot.Binded)
                {
                    slot.Bind(line.HeadConnector);
                }
            }
        }

        /// <summary>
        /// ��������������
        /// </summary>
        /// <param name="line">������</param>
        public virtual void BindOutLine(ConnectorContainer line)
        {
            SlotGraphElement slot = GetOutSlot();

            if (slot != null)
            {
                slot.Bind(line.TailConnector);
            }
        }

        /// <summary>
        /// ��������������
        /// </summary>
        /// <param name="line">������</param>
        /// <param name="index">����</param>
        public virtual void BindOutLine(ConnectorContainer line, int index)
        {
            if (index >= 0 && index < outSlotCount)
            {
                SlotGraphElement slot = outSlotList[index];

                if (!slot.Binded)
                {
                    slot.Bind(line.TailConnector);
                }
            }
        }

        /// <summary>
        /// ˢ�����ӵ������ߵ���ʾ
        /// </summary>
        /// <param name="lineVisible">�Ƿ���ʾ������</param>
        public void RefreshRelevateLine(bool lineVisible)
        {
            foreach (SlotGraphElement slot in slotList)
            {
                if (slot.Binded)
                {
                    slot.BindingConnector.Line.Visible = lineVisible; // �ƶ�������������
                }
            }
        }

        /// <summary>
        /// ��ȡ��ָ��λ�õĲ��
        /// </summary>
        /// <param name="p">ָ��λ��</param>
        /// <returns>���</returns>
        public SlotGraphElement GetSlotInRegion(Point p)
        {
            SlotGraphElement slotGraphElement = null;

            foreach (SlotGraphElement slot in slotList) // �������������Ĳ���Ƿ�ɱ�����
            {
                if (slot.IsInRegion(p))
                {
                    slotGraphElement = slot;
                    break;
                }
            }

            return slotGraphElement;
        }

        /// <summary>
        /// ����������е�������
        /// </summary>
        public void UnBind()
        {
            foreach (SlotGraphElement slot in slotList)
            {
                if (slot.Binded)
                {
                    slot.UnBind();
                }
            }
        }

        /// <summary>
        /// ��ȡ���ӵ�������
        /// </summary>
        /// <returns>����������</returns>
        public List<ConnectorContainer> GetConnectedLine()
        {
            List<ConnectorContainer> lineList = new List<ConnectorContainer>();

            foreach (SlotGraphElement slot in slotList)
            {
                if (slot.Binded)
                {
                    ConnectorContainer line = slot.BindingConnector.Line;

                    if (!lineList.Contains(line))
                    {
                        lineList.Add(line);
                    }
                }
            }

            return lineList;
        }

        /// <summary>
        /// ���ӳ��ڲ��
        /// </summary>
        /// <returns>�����Ĳ��</returns>
        public virtual SlotGraphElement AddOutSlot()
        {
            OutSlotCount = outSlotCount + 1;
            SlotGraphElement newSlot = outSlotList[outSlotCount - 1];

            return newSlot;
        }

        /// <summary>
        /// ��ȡ���ڲ��
        /// </summary>
        /// <returns>���ڲ��</returns>
        public virtual SlotGraphElement GetOutSlot()
        {
            bool find = false; // �Ƿ��Ѿ��ҵ�û�а������߿��Ƶ�Ĳ��
            int priority = -1;
            SlotGraphElement newSlot = null;

            foreach (SlotGraphElement slot in outSlotList)
            {
                if (!slot.Binded)
                {
                    if(slot.Priority < priority || priority == -1)
                    {
                        newSlot = slot;
                        priority = slot.Priority;
                    }
                    find = true;
                }
            }

            if (!find) // û���ҵ�û�а������߿��Ƶ�Ĳ�ۣ����½�һ����ڲ��
            {
                OutSlotCount = outSlotCount + 1;
                newSlot = outSlotList[outSlotCount - 1];
            }

            return newSlot;
        }

        /// <summary>
        /// ɾ�����
        /// </summary>
        /// <param name="slot">Ҫɾ���Ĳ��</param>
        public virtual void RemoveSlot(SlotGraphElement slot)
        {
            if (slot.IsInSlot) // ��ڲ��
            {
                RemoveInSlot(slot);
            }
            else // ���ڲ��
            {
                RemoveOutSlot(slot);
            }
        }

        /// <summary>
        /// ������ڲ��
        /// </summary>
        public void LockInSlot()
        {
            foreach (SlotGraphElement slot in inSlotList)
            {
                slot.CanDelete = false;
            }
        }

        /// <summary>
        /// �������ڲ��
        /// </summary>
        public void LockOutSlot()
        {
            foreach (SlotGraphElement slot in outSlotList)
            {
                slot.CanDelete = false;
            }
        }        

        /// <summary>
        /// ɾ����ڲ��
        /// </summary>
        /// <param name="slot">Ҫɾ���Ĳ��</param>
        protected virtual void RemoveInSlot(SlotGraphElement slot)
        {
            if(slot.Binded) // �����
            {
                slot.UnBind();
            }
            inSlotList.Remove(slot);
            slotList.Remove(slot);
            inSlotCount = inSlotList.Count;
            AdjustInSlotList(inSlotList.Count);
        }

        /// <summary>
        /// ɾ�����ڲ��
        /// </summary>
        /// <param name="slot">Ҫɾ���Ĳ��</param>
        protected virtual void RemoveOutSlot(SlotGraphElement slot)
        {
            if (slot.Binded) // �����
            {
                slot.UnBind();
            }
            outSlotList.Remove(slot);
            slotList.Remove(slot);
            buttonList.Remove(slot.BindingConnectButton);
            outSlotCount = outSlotList.Count;
            AdjustOutSlotList(outSlotList.Count);
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="srcSlotContainer">Դ�������</param>
        /// <param name="destSlotContainer">Ŀ��������</param>
        protected virtual void CopyData(SlotContainer srcSlotContainer, SlotContainer destSlotContainer)
        {
            destSlotContainer.Name = srcSlotContainer.Name;
            destSlotContainer.Remark = srcSlotContainer.Remark;
            destSlotContainer.Text = srcSlotContainer.Text;
            destSlotContainer.DisplayText = srcSlotContainer.DisplayText;
            destSlotContainer.TooltipText = srcSlotContainer.TooltipText;
            destSlotContainer.ShowText = srcSlotContainer.ShowText;

            // ��ʼ����ͼ����
            destSlotContainer.Init();
            destSlotContainer.InSlotCount = srcSlotContainer.InSlotCount;
            destSlotContainer.OutSlotCount = srcSlotContainer.OutSlotCount;
            destSlotContainer.AdjustText();
            destSlotContainer.AdjustResizeControlerList();
            destSlotContainer.AdjustButtonList();
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public virtual SlotContainer Clone()
        {            
            return null;
        }
    }
}
