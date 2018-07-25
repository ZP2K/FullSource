using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;
using FlowChartEditor.ButtonGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class SlotGraphElement:GraphElement
    {
        private SlotContainer slotContainer; // �����������
        private ConnectorGraphElement bindingConnector; // �󶨵������߿��Ƶ�
        private ButtonGraphElement.BaseButton bindingConnectButton; // �󶨵����Ӱ�ť
        private Color bindedColor = Color.Lime; // �󶨺���ʾ����ɫ
        private bool binded = false; // ����Ƿ��Ѿ��������߿��Ƶ�
        private bool isInSlot = false; // �Ƿ�����ڲ��
        private bool isOutSlot = false; // �Ƿ��ǳ��ڲ��
        private bool canDelete = true; // �Ƿ����ɾ��
        private int priority; // ��۵����ȼ�

        [NonSerialized]
        Rectangle textRectangle = Rectangle.Empty;
        [NonSerialized]
        Rectangle activateRectangle = Rectangle.Empty;

        [NonSerialized]
        protected GraphSetting graphSetting; // ��ͼ�������ö���

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="slotContainner">�����������</param>
        /// <param name="location">��۵�λ��</param>
        /// <param name="size">��۵Ĵ�С</param>
        /// <param name="id">��۵�id</param>
        public SlotGraphElement(SlotContainer slotContainer, Point location, Size size)
            : base(location)
        {
            this.slotContainer = slotContainer;
            this.elementSize = size;
        }

        /// <summary>
        /// ��۵����ȼ�����
        /// </summary>
        [Browsable(false)]
        public int Priority
        {
            set
            {
                priority = value;
            }

            get
            {
                return priority;
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
            }

            get
            {
                return location;
            }
        }

        /// <summary>
        /// �Ƿ����ɾ������
        /// </summary>
        [Browsable(false)]
        public bool CanDelete
        {
            set
            {
                canDelete = value;
            }

            get
            {
                return canDelete;
            }
        }

        /// <summary>
        /// �󶨵����Ӱ�ť����
        /// </summary>
        [Browsable(false)]
        public ButtonGraphElement.BaseButton BindingConnectButton
        {
            set
            {
                bindingConnectButton = value;
            }

            get
            {
                return bindingConnectButton;
            }
        }

        /// <summary>
        /// �Ƿ��ǳ��ڲ������
        /// </summary>
        [Browsable(false)]
        public bool IsInSlot
        {
            set
            {
                isInSlot = value;
            }

            get
            {
                return isInSlot;
            }
        }

        /// <summary>
        /// �Ƿ�����ڲ������
        /// </summary>
        [Browsable(false)]
        public bool IsOutSlot
        {
            set
            {
                isOutSlot = value;
            }

            get
            {
                return isOutSlot;
            }
        }

        /// <summary>
        /// �󶨺���ʾ����ɫ����
        /// </summary>
        [Browsable(true), DisplayName("��״̬��ɫ"), Description("�󶨺���ʾ����ɫ"), Category("��ͼ����")]
        public Color BindedColor
        {
            set
            {
                bindedColor = value;
            }

            get
            {
                return bindedColor;
            }
        }

        /// <summary>
        /// ���������������
        /// </summary>
        [Browsable(false)]
        public SlotContainer SlotContainer
        {
            get
            {
                return slotContainer;
            }
        }

        /// <summary>
        /// ��۴�С����
        /// </summary>
        [Browsable(false)]
        public override Size ElementSize
        {
            get
            {
                return elementSize;
            }
        }

        /// <summary>
        /// ����Ƿ��Ѿ��������߿��Ƶ�����
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
        /// �󶨵������߿��Ƶ�����
        /// </summary>
        [Browsable(false)]
        public ConnectorGraphElement BindingConnector
        {
            get
            {
                return bindingConnector;
            }
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
                if(slotContainer.Selected || slotContainer.Resizing || selected || activated) // ����������Լ���ѡ�кͼ���ʱ����ʾ
                {
                    if (activated) // ��ǰͼԪ������
                    {
                        g.FillRectangle(graphSetting.BackgroundBrush, activateRectangle);
                        g.DrawRectangle(graphSetting.GraphElementActivatedPen, activateRectangle);
                    }
                    else if(selected) // ��ǰͼԪ��ѡ��
                    {
                        g.FillRectangle(graphSetting.BackgroundBrush, activateRectangle);
                        g.DrawRectangle(graphSetting.GraphElementSelectedPen, activateRectangle);
                    }
                    else if(binded) // ��ǰͼԪ����
                    {
                        // �������߿��Ƶ�������
                    }
                    else // ��ǰͼԪû�б�ѡ�У�����Ͱ�
                    {
                        g.FillRectangle(graphSetting.BackgroundBrush, textRectangle);
                        g.DrawRectangle(graphSetting.GraphElementNormalPen, textRectangle);
                    }

                    // ���ư󶨵������߿��Ƶ�
                    if (binded)
                    {
                        bindingConnector.Paint(g);
                    }
                }

                // �������Ӱ�ť
                if (isOutSlot && !binded)
                {
                    if (slotContainer.Resizing && bindingConnectButton != null)
                    {
                        bindingConnectButton.Paint(g);
                    }                
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
            if(!selected) // ��۱�ѡ��ʱ�����ƶ�
            {
                this.location += s;
                textRectangle.Location += s;
                activateRectangle.Location += s;

                if(bindingConnector != null) // �ƶ������߿��Ƶ�
                {
                    bindingConnector.Move(s);
                }

                if(bindingConnectButton != null) // �ƶ����Ӱ�ť
                {
                    bindingConnectButton.Move(s);
                }
            }
        }

        /// <summary>
        /// �������߿��Ƶ�
        /// </summary>
        /// <param name="connector">�����߿��Ƶ�</param>
        public void Bind(ConnectorGraphElement connector)
        {
            if(binded && connector != bindingConnector) // �Ѿ����������߿��Ƶ�
            {
                MessageBox.Show("�ò���Ѿ����������߿��Ƶ㣡", "���Ӱ󶨼��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else if(isInSlot && connector.IsTailPoint)
            {
                MessageBox.Show("��ڲ�۲��ܰ󶨳��������߿��Ƶ㣡", "���Ӱ󶨼��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else if(isOutSlot && connector.IsHeadPoint)
            {
                MessageBox.Show("���ڲ�۲��ܰ�����������߿��Ƶ㣡", "���Ӱ󶨼��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                bindingConnector = connector;
                bindingConnector.Binded = true;
                bindingConnector.Location = location;
                Binded = true;

                // �����߼�������Ϣ
                if (connector.IsHeadPoint) // �����߿��Ƶ���ͷ���Ƶ�
                {
                    connector.Line.OutSlotContainer = slotContainer;
                }
                else // �����߿��Ƶ���β���Ƶ�
                {
                    connector.Line.InSlotContainer = slotContainer;
                }
            }
        }

        /// <summary>
        /// ȡ���������߿��Ƶ�
        /// </summary>
        public void UnBind()
        {
            // �����߼�������Ϣ
            if (bindingConnector.IsHeadPoint) // �����߿��Ƶ���ͷ���Ƶ�
            {
                bindingConnector.Line.OutSlotContainer = null;
            }
            else // �����߿��Ƶ���β���Ƶ�
            {
                bindingConnector.Line.InSlotContainer = null;
            }

            bindingConnector.Binded = false;
            bindingConnector = null;
            Binded = false;            
        }
    }
}
