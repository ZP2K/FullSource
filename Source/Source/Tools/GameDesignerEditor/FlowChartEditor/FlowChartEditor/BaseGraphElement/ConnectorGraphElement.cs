using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.ComponentModel;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor
{
    [Serializable]
    public class ConnectorGraphElement:GraphElement
    {
        private bool isHeadPoint = false; // �Ƿ��������ߵ�ͷ���
        private bool isTailPoint = false; // �Ƿ��������ߵ�β���
        private bool binded = false; // �Ƿ��Ѿ��󶨲��
        private ConnectorContainer line; // �����߿��Ƶ�������������

        [NonSerialized]
        Rectangle textRectangle = Rectangle.Empty;
        [NonSerialized]
        Rectangle activateRectangle = Rectangle.Empty;

        [NonSerialized]
        protected GraphSetting graphSetting; // ��ͼ�������ö���

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="line">�����߿��Ƶ�������������</param>
        /// <param name="location">���ӵ��λ��</param>
        /// <param name="size">�����ߵĴ�С</param>
        public ConnectorGraphElement(ConnectorContainer line, Point location, Size size)
            : base(location)
        {
            this.line = line;
            this.location = location;
            this.elementSize = size;
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
                textRectangle.Location = value + new Size(-(int)(elementSize.Width / 2), -(int)(elementSize.Height / 2));
                activateRectangle.Location = value + new Size(-elementSize.Width, -elementSize.Height);

                if(line.ShowText) // ���������ߵ��ı�λ��
                {
                    line.AdjustText(); 
                }
            }

            get
            {
                return location;
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
                invalidRectangle = new Rectangle(location + new Size(-elementSize.Width, -elementSize.Height), 
                                                 elementSize + new Size(elementSize.Width * 2, elementSize.Height * 2));

                return invalidRectangle;
            }
        }

        /// <summary>
        /// �Ƿ���������ͷ�������
        /// </summary>
        [Browsable(false)]
        public bool IsHeadPoint
        {
            set
            {
                isHeadPoint = value;
            }

            get
            {
                return isHeadPoint;
            }
        }

        /// <summary>
        /// �Ƿ���������β�������
        /// </summary>
        [Browsable(false)]
        public bool IsTailPoint
        {
            set
            {
                isTailPoint = value;
            }

            get
            {
                return isTailPoint;
            }
        }

        /// <summary>
        /// ͼԪ�Ƿ񱻼�������
        /// </summary>
        [Browsable(false)]
        public override bool Activated
        {
            set
            {
                activated = value;              
            }

            get
            {
                return activated;
            }
        }

        /// <summary>
        /// ͼԪ�Ƿ�ѡ������
        /// </summary>
        [Browsable(false)]
        public override bool Selected
        {
            set
            {
                selected = value;             
            }

            get
            {
                return selected;
            }
        }

        /// <summary>
        /// �Ƿ��Ѿ��󶨲������
        /// </summary>
        [Browsable(false)]
        public bool Binded
        {
            set
            {
                binded = value;              
            }

            get
            {
                return binded;
            }
        }

        /// <summary>
        /// �����߿��Ƶ�����������������
        /// </summary>
        [Browsable(false)]
        public ConnectorContainer Line
        {
            get
            {
                return line;
            }
        }

        /// <summary>
        /// ��ȡ�󶨵Ĳ��
        /// </summary>
        /// <returns>�󶨵Ĳ��</returns>
        public SlotGraphElement GetBindingSlot()
        {
            SlotGraphElement slotGraphElement = null;
            SlotContainer slotContainer;

            if (isHeadPoint)
            {
                slotContainer = line.OutSlotContainer;

                if (slotContainer != null)
                {
                    foreach(SlotGraphElement slot in slotContainer.GetInSlotList())
                    {
                        if (slot.BindingConnector == this)
                        {
                            slotGraphElement = slot;
                            break;
                        }
                    }
                }
            }
            else if (isTailPoint)
            {
                slotContainer = line.InSlotContainer;

                if (slotContainer != null)
                {
                    foreach(SlotGraphElement slot in slotContainer.GetOutSlotList())
                    {
                        if (slot.BindingConnector == this)
                        {
                            slotGraphElement = slot;
                            break;
                        }
                    }
                }
            }

            return slotGraphElement;
        }

        /// <summary>
        /// ˢ��ͼԪ���Ʋ���
        /// </summary>
        public override void Refresh()
        {
            // ��ʼ����ͼ���ݹ������
            graphSetting = GraphSetting.GetGraphSetting();

            // ˢ���߼�����
            textRectangle.Location = location + new Size(-(int)(elementSize.Width / 2), -(int)(elementSize.Height / 2));
            textRectangle.Size = elementSize;
            activateRectangle.Location = location + new Size(-elementSize.Width, -elementSize.Height);
            activateRectangle.Size = elementSize + elementSize;           
        }

        /// <summary>
        /// �ͷ�ͼԪ�Ļ�ͼ��Դ
        /// </summary>
        public override void Release()
        {
            
        }

        /// <summary>
        /// �ж�ĳ�����Ƿ��ڸ�ͼԪ��������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�õ��Ƿ���ͼԪ��������</returns>
        public override bool IsInRegion(Point p)
        {
            bool result = activateRectangle.Contains(p);

            return result;
        }

        /// <summary>
        /// �ж�ͼԪ�Ƿ���ĳ��������
        /// </summary>
        /// <param name="r">��ǰ����</param>
        /// <returns>ͼԪ�Ƿ��ڵ�ǰ������</returns>
        public override bool IsInRegion(Rectangle r)
        {
            bool result = r.Contains(textRectangle);

            return result;
        }

        /// <summary>
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void Paint(Graphics g)
        {   
            if (visible)
            {
                // ���Ʊ߿�ɫ           
                if (activated) // ��ǰͼԪ������
                {
                    g.FillRectangle(graphSetting.GraphElementActivatedBrush, textRectangle);
                }
                else if (selected) // ��ǰͼԪ��ѡ��
                {
                    g.FillRectangle(graphSetting.GraphElementSelectedBrush, textRectangle);
                }
                else if (binded) // ��ǰͼԪ����
                {
                    g.FillRectangle(graphSetting.GraphElementBindedBrush, textRectangle);
                }
                else // ��ǰͼԪû�б�ѡ��
                {
                    g.FillRectangle(graphSetting.GraphElementNormalBrush, textRectangle);
                }
            }            
        }

        /// <summary>
        /// �������Ե�ͼԪ
        /// </summary>
        /// <param name="g">��ͼ����</param>
        public override void AbbreviativePaint(Graphics g)
        {
            
        }

        /// <summary>
        /// �ƶ�ͼԪ
        /// </summary>
        /// <param name="p">�ƶ������λ��</param>
        public override void Move(Size s)
        {
            this.location += s;
            textRectangle.Location += s;
            activateRectangle.Location += s;

            if (line.ShowText) // ���������ߵ��ı�λ��
            {
                line.AdjustText();
            }
        }
    }
}
