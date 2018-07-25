using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.Drawing;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartMoveCommand:FlowChartBaseCommand
    {
        private Point location; // ��ǰͼԪ��λ��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartMoveCommand(object data, string description):
            base(data, description)
        {

        }

        /// <summary>
        /// ���浱ǰ��״̬
        /// </summary>
        public void SaveCurrentState(object o)
        {            
            location = (Point)o;

            // ��������ִ��ǰ������
            SaveBeforeExecute(flowChartManager.GetArguments());            
        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        /// <returns>�Ƿ�ִ�гɹ�</returns>
        public override bool Execute(object o)
        {
            bool success = false;            
            object[] args = o as object[];
            GraphElement graphElement = args[0] as GraphElement;
            Point newLocation = (Point)args[1];

            if(location != newLocation) // ͼԪ���ƶ���
            {
                description = "�ƶ�ͼԪ " + graphElement.Name;
                if(graphElement is ResizeControler)
                {
                    description = "����ͼԪ " + (graphElement as ResizeControler).Owner.Name;
                }
                // ��������ִ�к������
                flowChartManager.ContentChanged = true;
                SaveAfterExecute(flowChartManager.GetArguments());                
                success = true;
            }            

            return success;
        } 
    }
}
