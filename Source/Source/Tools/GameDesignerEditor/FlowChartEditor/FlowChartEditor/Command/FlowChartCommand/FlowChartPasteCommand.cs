using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Drawing;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartPasteCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartPasteCommand(object data, string description):base(data, description)
        {

        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">����Ĳ���</param>
        /// <returns>�Ƿ�ִ�гɹ�</returns>
        public override bool Execute(object o)
        {
            bool success = true;            
            Point currentPoint = (Point)o;
            Size moveSize = Size.Empty;

            // ��������ִ��ǰ������
            if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
            {                
                SaveBeforeExecute(flowChartManager.GetArguments());
            }

            description = "ճ��ͼԪ";
            Hashtable table = CopyGraphElement(documentManager.CopyTable);

            // ����ͼԪճ��λ��ƫ��
            List<GraphElement> graphElementList = new List<GraphElement>();
            foreach(object key in table.Keys)
            {
                graphElementList.Add(key as GraphElement);
            }
            moveSize = InitMoveSize(graphElementList, currentPoint);

            // ���뵽��ͼ������
            foreach (object key in table.Keys)
            {
                if (key is SlotContainer) // �������
                {
                    SlotContainer slotContainer = key as SlotContainer;                    
                    slotContainer.Move(moveSize);
                    graphManager.SlotContainerList.Add(slotContainer);

                    // �����µ�ͼԪID
                    slotContainer.ID = graphManager.AllocateGraphElementID();

                    // ��������
                    dataManager.AddDataElement(slotContainer);
                    dataManager.SetData(slotContainer, table[key]);
                    dataManager.ReloadData(slotContainer);
                }
                else if (key is ConnectorContainer) // ������
                {
                    ConnectorContainer line = key as ConnectorContainer;                    

                    foreach (ConnectorGraphElement connector in line.GetConnectorList()) // �ƶ������߿��Ƶ�
                    {
                        if (!connector.Binded) // �����߿��Ƶ㲻�������ߵ�ͷ����β���
                        {
                            connector.Move(moveSize);
                        }
                    }

                    graphManager.ConnectorContainerList.Add(line);                                

                    // �����µ�ͼԪID
                    line.ID = graphManager.AllocateGraphElementID();

                    // ��������                    
                    dataManager.AddDataElement(line);
                    dataManager.SetData(line, table[key]);
                    dataManager.ReloadData(line);
                }
            }

            // ����¼�����Ƿ���Ч
            foreach (object key in table.Keys)
            {
                if (key is SlotContainer) // �������
                {
                    SlotContainer slotContainer = key as SlotContainer;
                    slotContainer.Enable = dataManager.IsConnectEventNode(slotContainer);
                }
                else if(key is ConnectorContainer) // ������
                {
                    ConnectorContainer line = key as ConnectorContainer;
                    line.Enable = dataManager.IsConnectEventNode(line);
                }
            }

            flowChartManager.PasteTable = table; 
            
            if(success) // ��������ִ�к������
            {
                flowChartManager.ContentChanged = true;
                SaveAfterExecute(flowChartManager.GetArguments());                
            }
            
            return success;
        }

        /// <summary>
        /// ����ͼԪ�ƶ���ƫ��
        /// </summary>
        /// <param name="graphElementList">ͼԪ����</param>
        /// <param name="currentPoint">��ǰ������λ��</param>
        /// <returns>ͼԪ�ƶ���ƫ��</returns>
        private Size InitMoveSize(List<GraphElement> graphElementList, Point currentPoint)
        {
            Size moveSize = Size.Empty;
            Point newPosition = Point.Empty;

            foreach (GraphElement graphElement in graphElementList)
            {
                if (moveSize == Size.Empty)
                {
                    moveSize.Width = currentPoint.X - graphElement.Location.X;
                    moveSize.Height = currentPoint.Y - graphElement.Location.Y;                    
                }

                newPosition = graphElement.Location + moveSize;

                // ����ͼԪճ�����λ�õ�����
                if (newPosition.X < 50)
                {
                    moveSize.Width = 50 - graphElement.Location.X;
                }

                if (newPosition.Y < 50)
                {
                    moveSize.Height = 50 - graphElement.Location.Y;
                }
            }

            return moveSize;
        }
    }
}
