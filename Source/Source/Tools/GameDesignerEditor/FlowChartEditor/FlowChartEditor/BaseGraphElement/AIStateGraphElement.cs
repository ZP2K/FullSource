using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.ComponentModel;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class AIStateGraphElement:AISlotContainer
    {        
        private bool isInitialState = false; // �Ƿ��ǳ�̬

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="elementSize">ͼԪ�Ĵ�С</param>
        public AIStateGraphElement(Point location, Size elementSize):base(location, elementSize)
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
                return "AIStateNode";
            }
        }

        /// <summary>
        /// �Ƿ��ǳ�̬
        /// </summary>
        [Browsable(false)]
        public bool IsInitialState
        {
            set
            {
                isInitialState = value;
                isEntrance = value;                
            }

            get
            {
                return isInitialState;
            }
        }

        /// <summary>
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void Paint(Graphics g)
        {            
            base.Paint(g);

            // ���Ƴ�̬���
            if (visible && isInitialState)
            {
                g.FillEllipse(Brushes.Black, location.X + elementSize.Width + 10, location.Y, 10, 10);
            }
        }

        /// <summary>
        /// ����ͼԪ������Ч��
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void AbbreviativePaint(Graphics g)
        {
            // ����������
            g.DrawImage(abbreviateImage, textRectangle);
        }

        /// <summary>
        /// ˢ��ͼԪ��ͼƬ
        /// </summary>
        protected override void RefreshImages()
        {
            if (normalImage == null)
            {
                normalImage = graphSetting.AIStateNodeNormalImage;
            }

            if (selectedImage == null)
            {
                selectedImage = graphSetting.AIStateNodeSelectedImage;
            }

            if (activatedImage == null)
            {
                activatedImage = graphSetting.AIStateNodeActivatedImage;
            }

            if (shadowImage == null)
            {
                shadowImage = graphSetting.AIStateNodeShadowImage;
            }

            if (abbreviateImage == null)
            {
                abbreviateImage = graphSetting.AIStateNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public override SlotContainer Clone()
        {            
            AIStateGraphElement aiStateGraphElement = new AIStateGraphElement(location, elementSize);
            CopyData(this, aiStateGraphElement);
            aiStateGraphElement.IsInitialState = isInitialState;

            return aiStateGraphElement;
        }
    }
}
