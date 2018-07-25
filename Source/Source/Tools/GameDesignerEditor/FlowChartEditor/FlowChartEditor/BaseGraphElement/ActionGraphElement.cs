using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing.Imaging;
using GraphElementLibrary;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class ActionGraphElement:SlotContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        /// <param name="id">ͼԪ�Ĵ�С</param>
        public ActionGraphElement(Point location, Size size)
            : base(location, size)
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
                return "ActionNode";
            }
        }

        /// <summary>
        /// ˢ��ͼԪ��ͼƬ
        /// </summary>
        protected override void RefreshImages()
        {
            if (normalImage == null)
            {
                normalImage = graphSetting.ActionNodeNormalImage;
            }

            if (selectedImage == null)
            {
                selectedImage = graphSetting.ActionNodeSelectedImage;
            }

            if (activatedImage == null)
            {
                activatedImage = graphSetting.ActionNodeActivatedImage;
            }

            if (shadowImage == null)
            {
                shadowImage = graphSetting.ActionNodeShadowImage;
            }

            if (abbreviateImage == null)
            {
                abbreviateImage = graphSetting.ActionNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public override SlotContainer Clone()
        {
            ActionGraphElement actionGraphElement = new ActionGraphElement(location, elementSize);
            CopyData(this, actionGraphElement);                          

            return actionGraphElement;
        }
    }
}
