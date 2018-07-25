using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

using FlowChartEditor.ButtonGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class InterfaceGraphElement : AISlotContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪλ��</param>
        /// <param name="elementSize">ͼԪ��С</param>
        public InterfaceGraphElement(Point location, Size elementSize) : base(location, elementSize)
        {

        }

        /// <summary>
        /// �����ַ���
        /// </summary>
        public override string TypeString
        {
            get
            {
                return "InterfaceNode";
            }
        }

        /// <summary>
        /// ��ȡ���ӵ�ͼԪ
        /// </summary>
        /// <returns>���ӵ�ͼԪ</returns>
        public GraphElement GetConnectedOutGraphElement()
        {
            GraphElement graphElement = null;

            foreach (SlotGraphElement slot in outSlotList)
            {
                if (slot.Binded)
                {
                    ConnectorContainer line = slot.BindingConnector.Line;
                    graphElement = line.OutSlotContainer;
                }
            }

            return graphElement;
        }

        /// <summary>
        /// ˢ��ͼԪ��ͼƬ
        /// </summary>
        protected override void RefreshImages()
        {
            if (normalImage == null)
            {
                normalImage = graphSetting.InterfaceNodeNormalImage;
            }

            if (selectedImage == null)
            {
                selectedImage = graphSetting.InterfaceNodeSelectedImage;
            }

            if (activatedImage == null)
            {
                activatedImage = graphSetting.InterfaceNodeActivatedImage;
            }

            if (shadowImage == null)
            {
                shadowImage = graphSetting.InterfaceNodeShadowImage;
            }

            if (abbreviateImage == null)
            {
                abbreviateImage = graphSetting.InterfaceNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ��ʼ����ť
        /// </summary>
        protected override void InitButton()
        {

        }

        /// <summary>
        /// ������ť����
        /// </summary>
        protected override void AdjustButtonList()
        {

        }

        /// <summary>
        /// ��ȡ���ڲ��
        /// </summary>
        /// <returns>���ڲ��</returns>
        public override SlotGraphElement GetOutSlot()
        {
            int priority = -1;
            SlotGraphElement newSlot = null;

            foreach (SlotGraphElement slot in outSlotList)
            {
                if (!slot.Binded)
                {
                    if (slot.Priority < priority || priority == -1)
                    {
                        newSlot = slot;
                        priority = slot.Priority;
                    }
                }
            }

            return newSlot;
        }

        /// <summary>
        /// ���ӳ��ڲ��
        /// </summary>
        /// <returns>�����Ĳ��</returns>
        public override SlotGraphElement AddOutSlot()
        {
            return null;
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public override SlotContainer Clone()
        {
            InterfaceGraphElement interfaceGraphElement = new InterfaceGraphElement(location, elementSize);
            CopyData(this, interfaceGraphElement);

            return interfaceGraphElement;
        }
    }
}
