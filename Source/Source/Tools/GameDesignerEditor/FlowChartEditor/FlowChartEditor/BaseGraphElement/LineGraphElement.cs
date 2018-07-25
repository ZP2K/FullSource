using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.ComponentModel;
using GraphElementLibrary;

namespace FlowChartEditor.BaseGraphElement
{
    [Serializable]
    public class LineGraphElement: ConnectorContainer
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="tail">������β����λ��</param>
        /// <param name="head">������ͷ����λ��</param>
        /// <param name="id">�����ߵ�id</param>
        public LineGraphElement(Point tail, Point head)
            : base(tail, head)
        {
            connectorCount = 2;
        }

        /// <summary>
        /// �����ַ���
        /// </summary>        
        public override string TypeString
        {
            get
            {
                return "ConditionLine";
            }
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <returns>���Ƶ�ͼԪ</returns>
        public override ConnectorContainer Clone()
        {
            LineGraphElement newLine = new LineGraphElement(tailConnector.Location, headConnector.Location);
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
