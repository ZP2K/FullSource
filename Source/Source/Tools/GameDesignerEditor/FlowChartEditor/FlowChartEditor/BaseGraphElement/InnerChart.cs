using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class InnerChart : AISlotContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪλ��</param>
        /// <param name="size">ͼԪ��С</param>
        public InnerChart (Point location, Size size): base(location, size)
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
                return "InnerChart";
            }
        }

        /// <summary>
        /// ��ȡ�нӿ�ѡ���������
        /// </summary>
        /// <param name="infoString">�ӿ�ѡ��</param>
        /// <returns>������</returns>
        public ConnectorContainer GetConnectedLine(string infoString)
        {
            ConnectorContainer line = null;

            foreach (SlotGraphElement slot in outSlotList)
            {
                if (slot.Binded)
                {
                    ConnectorContainer connectorContainer = slot.BindingConnector.Line;

                    if (connectorContainer.Text == infoString)
                    {
                        line = connectorContainer;
                        break;
                    }
                }
            }

            return line;
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
        /// ��ȡ��ڲ��
        /// </summary>
        /// <returns>��ڲ��</returns>
        public override SlotGraphElement GetInSlot()
        {
            int priority = -1;
            SlotGraphElement newSlot = null;

            foreach (SlotGraphElement slot in inSlotList)
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
        /// ������ڲ��
        /// </summary>
        /// <returns>�����Ĳ��</returns>
        public override SlotGraphElement AddInSlot()
        {
            return null;
        }

        /// <summary>
        /// ˢ��ͼԪ��ͼƬ
        /// </summary>
        protected override void RefreshImages()
        {
            if (normalImage == null)
            {
                normalImage = graphSetting.AIActionNodeNormalImage;
            }

            if (selectedImage == null)
            {
                selectedImage = graphSetting.AIActionNodeSelectedImage;
            }

            if (activatedImage == null)
            {
                activatedImage = graphSetting.AIActionNodeActivatedImage;
            }

            if (shadowImage == null)
            {
                shadowImage = graphSetting.AIActionNodeShadowImage;
            }

            if (abbreviateImage == null)
            {
                abbreviateImage = graphSetting.AIActionNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public override SlotContainer Clone()
        {
            InnerChart innerChart = new InnerChart(location, elementSize);
            CopyData(this, innerChart);

            return innerChart;
        }
    }
}
