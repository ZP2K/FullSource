using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor
{
    public class Tooltip : GraphElement
    {
        private GraphSetting graphSetting = GraphSetting.GetGraphSetting();        
        private enum TipFormat { Normal, Bold, UnderLine, Split };

        private const string normalStyleString = "<normal>";
        private const string boldStyleString = "<bold>";
        private const string italicStyleString = "<underline>";
        private const string splitStyleString = "<split>";

        private const int charWidth = 16;        
        private const int tipInterval = 5;
        private const int textInterval = 5;
        private Size graphElementInterval = new Size(10, 10);

        private string[] lineSpliter = new string[] { "\r\n" };                
        private List<string> lineList = new List<string>();
        private List<TipFormat> formatList = new List<TipFormat>();
        
        private Rectangle visibleRectangle;
        private Rectangle graphElementRectangle = Rectangle.Empty;
        private Point textLocation = Point.Empty;
        private Size textSize = Size.Empty;

        private GraphElement activatedGraphElement = null; 

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="location">ͼԪ��λ��</param>
        public Tooltip(Point location) : base(location)
        {
            
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="text">ͼԪ���ı�</param>
        /// <param name="graphElementRectangle">ͼԪ����</param>
        /// <param name="visibleRectable">��ǰ��������</param>
        public void Adjust(string text, GraphElement graphElement, Rectangle visibleRectangle, Point mouseLocation)
        {
            this.text = text;            
            this.visibleRectangle = visibleRectangle;

            if (this.activatedGraphElement != graphElement)
            {
                this.activatedGraphElement = graphElement;
                Init(graphElement, mouseLocation);
            }            
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init(GraphElement graphElement, Point mouseLocation)
        {
            // ����ͼԪ����
            if (graphElement is SlotContainer)
            {
                SlotContainer slotContainer = graphElement as SlotContainer;
                graphElementRectangle = slotContainer.TextRectangle;
            }
            else if (graphElement is ConnectorContainer)
            {
                graphElementRectangle.Location = mouseLocation;
                graphElementRectangle.Size = graphElementInterval;
            }
            else
            {
                graphElementRectangle.Location = graphElement.Location;
                graphElementRectangle.Size = graphElement.ElementSize + graphElementInterval;
            }

            // �������ִ�С
            lineList.Clear();
            formatList.Clear();

            elementSize = Size.Empty;
            string[] lines = text.Split(lineSpliter, StringSplitOptions.RemoveEmptyEntries);
            Graphics g = Helper.GetHelper().GraphicsObject;
            
            textSize.Height = lines.Length * charWidth;
            int currentWidth = 0;            
            SizeF textSizeF;
            string displayLine;

            foreach (string line in lines)
            {                
                if (line.StartsWith(boldStyleString))
                {
                    displayLine = line.Replace(boldStyleString, "");
                    textSizeF = g.MeasureString(displayLine, graphSetting.GraphElementTextBoldFont);
                    currentWidth = (int)textSizeF.Width + 1;
                    
                    lineList.Add(displayLine);
                    formatList.Add(TipFormat.Bold);
                }
                else if (line.StartsWith(italicStyleString))
                {
                    displayLine = line.Replace(italicStyleString, "");
                    textSizeF = g.MeasureString(displayLine, graphSetting.GraphElementTextItalicFont);
                    currentWidth = (int)textSizeF.Width + 1;
                    
                    lineList.Add(displayLine);
                    formatList.Add(TipFormat.UnderLine);
                }
                else if (line.StartsWith(splitStyleString))
                {
                    currentWidth = 0;

                    displayLine = line.Replace(splitStyleString, "");
                    lineList.Add(displayLine);
                    formatList.Add(TipFormat.Split);
                }
                else
                {
                    displayLine = line;
                    textSizeF = g.MeasureString(displayLine, graphSetting.GraphElementTextFont);
                    currentWidth = (int)textSizeF.Width + 1;
                    
                    lineList.Add(displayLine);
                    formatList.Add(TipFormat.Normal);
                }

                if (currentWidth > textSize.Width)
                {
                    textSize.Width = currentWidth;
                }
            }

            elementSize.Width = textSize.Width + textInterval * 2;
            elementSize.Height = textSize.Height + textInterval * 2;

            // ��������λ��
            if (graphElementRectangle.Right + elementSize.Width > visibleRectangle.Right)
            {
                location.X = graphElementRectangle.Left - elementSize.Width - tipInterval;
                location.Y = graphElementRectangle.Top;
            }
            else
            {
                location.X = graphElementRectangle.Right + tipInterval;
                location.Y = graphElementRectangle.Top;
            }

            textLocation.X = location.X + textInterval;
            textLocation.Y = location.Y + textInterval;
        }

        /// <summary>
        /// �ж�ĳ�����Ƿ��ڸ�ͼԪ��������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�õ��Ƿ���ͼԪ��������</returns>
        public override bool IsInRegion(Point p)
        {
            return false;
        }

        /// <summary>
        /// �ж�ͼԪ�Ƿ���ĳ��������
        /// </summary>
        /// <param name="r">��ǰ����</param>
        /// <returns>ͼԪ�Ƿ��ڵ�ǰ������</returns>
        public override bool IsInRegion(Rectangle r)
        {
            return false;
        }

        /// <summary>
        /// ����ͼԪ����
        /// </summary>
        /// <param name="g">ͼ�ζ���</param>
        public override void Paint(Graphics g)
        {
            if (visible)
            {
                // g.ScaleTransform(1 / graphSetting.CanvasScale, 1 / graphSetting.CanvasScale); // �ָ�����

                g.FillRectangle(graphSetting.TooltipBackgroundBrush, location.X, location.Y, elementSize.Width, elementSize.Height);
                g.DrawRectangle(graphSetting.GraphElementNormalPen, location.X, location.Y, elementSize.Width, elementSize.Height);

                for (int i = 0; i < lineList.Count; i++)
                {
                    string line = lineList[i];
                    TipFormat lineFormat = formatList[i];                    

                    switch (lineFormat)
                    {
                        case TipFormat.Normal:
                            {
                                g.DrawString(line, graphSetting.GraphElementTextFont, graphSetting.LineTextBrush, textLocation.X, textLocation.Y + i * charWidth);
                                break;
                            }
                        case TipFormat.Bold:
                            {
                                g.DrawString(line, graphSetting.GraphElementTextBoldFont, graphSetting.LineTextBrush, textLocation.X, textLocation.Y + i * charWidth);
                                break;
                            }
                        case TipFormat.UnderLine:
                            {
                                g.DrawString(line, graphSetting.GraphElementTextUnderLineFont, graphSetting.LineTextBrush, textLocation.X, textLocation.Y + i * charWidth);
                                break;
                            }
                        case TipFormat.Split:
                            {
                                g.DrawLine(graphSetting.GraphElementNormalPen, location.X, textLocation.Y + i * charWidth, location.X + elementSize.Width, textLocation.Y + i * charWidth);
                                break;
                            }
                    }
                }                                
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

        }

        /// <summary>
        /// ˢ��ͼԪ���Ʋ���
        /// </summary>
        public override void Refresh()
        {

        }

        /// <summary>
        /// �ͷ�ͼԪ�Ļ�ͼ��Դ
        /// </summary>
        public override void Release()
        {

        }
    }
}
