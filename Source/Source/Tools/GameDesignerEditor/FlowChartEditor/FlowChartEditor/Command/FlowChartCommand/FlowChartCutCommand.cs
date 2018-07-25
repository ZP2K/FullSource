using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartCutCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartCutCommand(object data, string description):base(data, description)
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
            List<GraphElement> list = new List<GraphElement>();

            // ��������ִ��ǰ������
            if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
            {
                SaveBeforeExecute(flowChartManager.GetArguments());                
            }            

            if (graphManager.SelectedGraphElementList.Count > 0) // ���ж��ͼԪ
            {                
                List<GraphElement> graphElementList = graphManager.SelectedGraphElementList;
                description = "����ͼԪ��";

                foreach(GraphElement graphElement in graphElementList)
                {
                    if (graphElement is SlotContainer) // ͼԪ�ǲ������
                    {
                        SlotContainer slotContainer = graphElement as SlotContainer;
                        
                        foreach (SlotGraphElement slot in slotContainer.GetSlotList())
                        {
                            if(slot.Binded && !graphElementList.Contains(slot.BindingConnector.Line))
                            {
                                slot.UnBind();
                            }
                        }

                        graphManager.SlotContainerList.Remove(slotContainer);
                    }
                    else if (graphElement is ConnectorContainer) // ͼԪ��������
                    {
                        ConnectorContainer line = graphElement as ConnectorContainer;
                        List<SlotGraphElement> deleteSlotList = new List<SlotGraphElement>();

                        if(line.InSlotContainer != null && !graphElementList.Contains(line.InSlotContainer))
                        {
                            line.UnbindInSlotContainer();
                        }

                        graphManager.ConnectorContainerList.Remove(line);
                    }

                    list.Add(graphElement);
                }
            }
            else // ���е���ͼԪ
            {                
                GraphElement graphElement = graphManager.SelectedGraphElement;
                description = "����ͼԪ " + graphElement.Name;

                if(graphElement is SlotContainer) // ͼԪ�ǲ������
                {
                    SlotContainer slotContainer = graphElement as SlotContainer;                    

                    slotContainer.UnBind();
                    graphManager.SlotContainerList.Remove(slotContainer);
                }
                else if(graphElement is ConnectorContainer) // ͼԪ��������
                {
                    ConnectorContainer line = graphElement as ConnectorContainer;

                    if(line.InSlotContainer != null)
                    {
                        line.UnbindInSlotContainer();
                    }

                    if(line.OutSlotContainer != null)
                    {
                        line.UnbindOutSlotContainer();
                    }

                    graphManager.ConnectorContainerList.Remove(line);
                }

                list.Add(graphElement);
            }

            Hashtable cutTable = new Hashtable();
            foreach(GraphElement g in list)
            {
                cutTable[g] = dataManager.GetData(g);
            }
            documentManager.CopyTable = cutTable;
            
            if(success) // ��������ִ�к������
            {
                flowChartManager.ContentChanged = true;
                SaveAfterExecute(flowChartManager.GetArguments());                
            }            

            return success;
        }
    }
}
