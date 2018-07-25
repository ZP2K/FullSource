using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.ComponentModel;

namespace GraphElementLibrary
{
    [Serializable]
    public abstract class GraphElement
    {
        protected Point location; // ͼԪλ��
        protected string name = ""; // ͼԪ������
        protected int id = 0; // ͼԪ��id  
        protected Size elementSize; // ͼԪ��С����
        protected Size minSize; // ͼԪ����С��С

        [NonSerialized]
        protected Rectangle invalidRectangle; // ͼԪ��ˢ������      

        [NonSerialized]
        protected object tag; // ��������              

        protected string remark = ""; // ͼԪ��ע��
        protected string text = ""; // ͼԪ���ı�

        [NonSerialized]
        protected string displayText = ""; // ͼԪ����ʾ�ı�

        [NonSerialized]
        protected string tooltipText = ""; // ͼԪ��ע���ı�

        protected bool selected = false; // �Ƿ��ѱ�ѡ��
        protected bool activated = false; // �Ƿ��ѱ�����
        protected bool enable = false; // �Ƿ���Ч
        protected bool showText = false; // �Ƿ���ʾ�ı�
        protected bool showRemark = false; // �Ƿ���ʾע��
        protected bool moving = false; // ͼԪ�Ƿ������ƶ�
        protected bool showID = false; // �Ƿ���ʾͼԪ��id
        protected bool visible = true; // �Ƿ���ʾͼԪ        

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪλ��</param>
        /// <param name="id">ͼԪ��id</param>
        protected GraphElement(Point location)
        {
            this.location = location;
        }

        /// <summary>
        /// �����ַ���
        /// </summary>
        [Browsable(false)]
        public virtual string TypeString
        {
            get
            {
                return "";
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        [Browsable(false)]
        public virtual object Tag
        {
            set
            {
                tag = value;
            }

            get
            {
                return tag;
            }
        }

        /// <summary>
        /// ͼԪ�Ƿ�ɼ�
        /// </summary>
        [Browsable(false)]
        public virtual bool Visible
        {
            set
            {
                visible = value;
            }

            get
            {
                return visible;
            }
        }

        /// <summary>
        /// ͼԪ����ʾ�ı�
        /// </summary>
        [Browsable(false)]
        public virtual string TooltipText
        {
            set
            {
                tooltipText = value;
            }

            get
            {
                return tooltipText;
            }
        }

        /// <summary>
        /// �Ƿ���ʾID����
        /// </summary>
        [Browsable(false)]
        public virtual bool ShowID
        {
            set
            {
                showID = value;
            }

            get
            {
                return showID;
            }
        }

        /// <summary>
        /// ͼԪ�Ƿ������ƶ�����
        /// </summary>
        [Browsable(false)]
        public virtual bool Moving
        {
            set
            {
                moving = value;
            }

            get
            {
                return moving;
            }
        }

        /// <summary>
        /// ͼԪ��ˢ����������
        /// </summary>
        [Browsable(false)]
        public virtual Rectangle InvalidRectangle
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
        public virtual string DisplayText
        {
            set
            {
                displayText = value;
            }

            get
            {
                return displayText;
            }
        }

        /// <summary>
        /// ͼԪ��ע������
        /// </summary>
        [Browsable(false)]
        public virtual string Remark
        {
            set
            {
                remark = value;
            }

            get
            {
                return remark;
            }
        }

        /// <summary>
        /// �Ƿ���ʾע������
        /// </summary>
        [Browsable(false)]
        public virtual bool ShowRemark
        {
            set
            {
                showRemark = value;
            }

            get
            {
                return showRemark;
            }
        }

        

        /// <summary>
        /// ͼԪ�Ƿ���Ч����
        /// </summary>
        [Browsable(false)]
        public virtual bool Enable
        {
            set
            {
                enable = value;
            }

            get
            {
                return enable;
            }
        }

        /// <summary>
        /// ͼԪ����������
        /// </summary>
        [Browsable(false)]
        public virtual string Name
        {
            set
            {
                name = value;
            }

            get
            {
                return name;
            }
        }

        /// <summary>
        /// ͼԪ���ı�����
        /// </summary>
        [Browsable(false)]
        public virtual string Text
        {
            set
            {
                text = value;
            }

            get
            {
                return text;
            }
        }

        /// <summary>
        /// �Ƿ���ʾ�ı�����
        /// </summary>
        [Browsable(false)]
        public virtual bool ShowText
        {
            set
            {
                showText = value;
            }

            get
            {
                return showText;
            }
        }

        /// <summary>
        /// ͼԪ����С��С����
        /// </summary>
        [Browsable(false)]
        public virtual Size MinSize
        {
            set
            {
                minSize = value;
            }

            get
            {
                return minSize;
            }
        }        

        /// <summary>
        /// ͼԪλ������
        /// </summary>
        [Browsable(false)]
        public virtual Point Location
        {
            set
            {
                location = value;
            }

            get
            {
                return location;
            }
        }

        /// <summary>
        /// ͼԪid����
        /// </summary>
        [Browsable(false)]
        public virtual int ID
        {
            set
            {
                id = value;
            }

            get
            {
                return id;
            }
        }        

        /// <summary>
        /// ͼԪ�Ƿ�ѡ������
        /// </summary>
        [Browsable(false)]
        public virtual bool Selected
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
        /// ͼԪ�Ƿ񼤻�����
        /// </summary>
        [Browsable(false)]
        public virtual bool Activated
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
        /// ͼԪ��С����
        /// </summary>
        [Browsable(false)]
        public virtual Size ElementSize
        {
            set
            {
                elementSize = value;
            }

            get
            {
                return elementSize;
            }
        }

        /// <summary>
        /// �ж�ĳ�����Ƿ��ڸ�ͼԪ��������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�õ��Ƿ���ͼԪ��������</returns>
        public abstract bool IsInRegion(Point p);

        /// <summary>
        /// �ж�ͼԪ�Ƿ���ĳ��������
        /// </summary>
        /// <param name="r">��ǰ����</param>
        /// <returns>ͼԪ�Ƿ��ڵ�ǰ������</returns>
        public abstract bool IsInRegion(Rectangle r);

        /// <summary>
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public abstract void Paint(Graphics g);

        /// <summary>
        /// ����ͼԪ������Ч��
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public abstract void AbbreviativePaint(Graphics g);

        /// <summary>
        /// �ƶ�ͼԪ
        /// </summary>
        /// <param name="s">�ƶ������λ��</param>
        public abstract void Move(Size s);

        /// <summary>
        /// ˢ��ͼԪ���Ʋ���
        /// </summary>
        public abstract void Refresh();

        /// <summary>
        /// �ͷ�ͼԪ�Ļ�ͼ��Դ
        /// </summary>
        public abstract void Release();
    }
}
