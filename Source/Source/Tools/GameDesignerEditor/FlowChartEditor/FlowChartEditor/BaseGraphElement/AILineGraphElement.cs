using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class AILineGraphElement:ConnectorContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="tail">������β����λ��</param>
        /// <param name="head">������ͷ����λ��</param>
        public AILineGraphElement(Point tail, Point head):base(tail, head)
        {
            connectorCount = 2;
            enable = true;
        }

        /// <summary>
        /// �����ַ���
        /// </summary>
        public override string TypeString
        {
            get
            {
                return "AILine";
            }
        }

        /// <summary>
        /// ˢ��ͼԪ������
        /// </summary>
        public override void Refresh()
        {
            base.Refresh();
            requireEvent = false;
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public override ConnectorContainer Clone()
        {
            AILineGraphElement newLine = new AILineGraphElement(tailConnector.Location, headConnector.Location);
            newLine.Text = text;
            newLine.DisplayText = displayText;
            newLine.TooltipText = tooltipText;
            newLine.ShowText = showText;
            newLine.ShowID = showID;

            newLine.Init();
            newLine.Refresh();

            newLine.ConnectorCount = connectorCount;
            List<ConnectorGraphElement> list = newLine.GetConnectorList();

            for (int i = 0; i < connectorList.Count; i++)
            {
                list[i].Location = connectorList[i].Location;
            }

            return newLine;
        }
    }
}
