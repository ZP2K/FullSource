using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.ComponentModel;
using GraphElementLibrary;

namespace FlowChartEditor
{
    [Serializable]
    public class ButtonGraphElement:GraphElement
    {
        [NonSerialized]
        protected Rectangle textRectangle = Rectangle.Empty;

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="elementSize">ͼԪ�Ĵ�С</param>
        /// <param name="buttonType">��ť������</param>
        public ButtonGraphElement(Point location, Size elementSize):base(location)
        {
            this.elementSize = elementSize;
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
                invalidRectangle = new Rectangle(location, elementSize);
                return invalidRectangle;
            }
        }

        /// <summary>
        /// ˢ��ͼԪ���Ʋ���
        /// </summary>
        public override void Refresh()
        {
            textRectangle.Location = location;
            textRectangle.Size = elementSize;
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
            bool result = false;

            result = textRectangle.Contains(p);

            return result;
        }

        /// <summary>
        /// �ж�ͼԪ�Ƿ���ĳ��������
        /// </summary>
        /// <param name="r">��ǰ����</param>
        /// <returns>ͼԪ�Ƿ��ڵ�ǰ������</returns>
        public override bool IsInRegion(Rectangle r)
        {
            bool result = false;

            result = r.Contains(textRectangle);

            return result;
        }

        /// <summary>
        /// ˢ��ͼԪ��ͼƬ
        /// </summary>
        public virtual void RefreshImages()
        {
            if (this is AddButton) // ��ǰ����Ϊ��Ӱ�ť
            {
                if (normalImage == null)
                {
                    normalImage = Helper.GetHelper().AddButtonNormalImage;
                }

                if (activatedImage == null)
                {
                    activatedImage = Helper.GetHelper().AddButtonActivatedImage;
                }
            }
            else if (this is ConnectButton) // ��ǰ����Ϊ���Ӱ�ť
            {
                if (normalImage == null)
                {
                    normalImage = Helper.GetHelper().ConnectButtonNormalImage;
                }

                if (activatedImage == null)
                {
                    activatedImage = Helper.GetHelper().ConnectButtonActivatedImage;
                }               
            }            
        }

        /// <summary>
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void Paint(Graphics g)
        {
            Image image;

            if(selected) // ͼԪ��ѡ��
            {
                image = activatedImage;
                g.DrawImage(image, textRectangle);
            }
            else if(activated) // ͼԪ������
            {
                image = activatedImage;
                g.DrawImage(image, textRectangle);
            }
            else // ͼԪû�б�ѡ�кͼ���
            {
                image = normalImage;
                g.DrawImage(image, textRectangle);
            }
        }

        /// <summary>
        /// ����ͼԪ������Ч��
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void AbbreviativePaint(Graphics g)
        {

        }

        /// <summary>
        /// �ƶ�ͼԪ
        /// </summary>
        /// <param name="s">�ƶ������λ��</param>
        public override void Move(Size s)
        {
            this.location += s;
            textRectangle.Location += s;
        }
    }
}
