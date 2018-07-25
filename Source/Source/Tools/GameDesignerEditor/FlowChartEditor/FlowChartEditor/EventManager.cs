using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

using DevComponents.DotNetBar;

using FlowChartEditor.BaseGraphElement;

namespace FlowChartEditor
{
    public class EventManager
    {
        GraphManager graphManager; // ��ͼ������
        
        // �¼����
        public delegate void OnConnectDelegate(object sender, ConnectEventArgs e);
        public delegate void OnDisconnectDelegate(object sender, ConnectEventArgs e);


        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="graphManager">��ͼ����������</param>
        public EventManager(GraphManager graphManager)
        {
            this.graphManager = graphManager;
        }

        /// <summary>
        /// ע���ͼ����¼�
        /// </summary>
        /// <param name="canvas">��ͼ�����</param>
        public void RegistCanvasEvent(Canvas canvas)
        {
            canvas.MouseDoubleClick += canvas_MouseDoubleClick;
            canvas.MouseDown += canvas_MouseDown;
            canvas.MouseEnter += canvas_MouseEnter;
            canvas.MouseLeave += canvas_MouseLeave;
            canvas.MouseMove += canvas_MouseMove;
            canvas.MouseUp += canvas_MouseUp;
            canvas.MouseWheel += canvas_MouseWheel;
            canvas.KeyDown += canvas_KeyDown;
            canvas.KeyUp += canvas_KeyUp;
        }

        /// <summary>
        /// ע�����ť���¼�
        /// </summary>
        /// <param name="buttonItem">��ť����</param>
        public void RegistCommandButtonEvent(ButtonItem buttonItem)
        {
            buttonItem.Click += commandButton_Click;            
        }

        /// <summary>
        /// ���ť�¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void commandButton_Click(object sender, EventArgs e)
        {
            ButtonItem buttonItem = sender as ButtonItem;

            if (buttonItem != null)
            {
                int commandIndex = int.Parse(buttonItem.Tag.ToString());
                graphManager.Undo(commandIndex);
            }
        }

        /// <summary>
        /// ���̰����¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_KeyDown(object sender, KeyEventArgs e)
        {
            DocumentManager documentManager = DocumentManager.GetDocumentManager();
            FlowChartManager flowChartManager = documentManager.CurrentFlowChartManager;
            graphManager.ControlMode = e.Control;
            graphManager.ShiftMode = e.Shift;
            graphManager.AltMode = e.Alt;

            switch (e.KeyCode)
            {
                case Keys.Enter: // ��Ӧenter����
                    {
                        graphManager.EditDataElement();

                        break;
                    }
                case Keys.Escape: // ��Ӧesc����
                    {
                        graphManager.DeleteAbbreviateGraphElement();

                        break;
                    }
                case Keys.Space: // ��Ӧ�ո��
                    {
                        graphManager.CurrentBackground.DebugMode = !graphManager.CurrentBackground.DebugMode;

                        break;
                    }
                case Keys.Left: // ��Ӧ�����
                    {
                        graphManager.MoveSelectedGraphElement(new Size(-1, 0));

                        break;
                    }
                case Keys.Up: // ��Ӧ�Ϸ����
                    {
                        graphManager.MoveSelectedGraphElement(new Size(0, -1));

                        break;
                    }
                case Keys.Right: // ��Ӧ�ҷ����
                    {
                        graphManager.MoveSelectedGraphElement(new Size(1, 0));

                        break;
                    }
                case Keys.Down: // ��Ӧ�·����
                    {
                        graphManager.MoveSelectedGraphElement(new Size(0, 1));

                        break;
                    }
                case Keys.Delete:// ��Ӧdelete����
                    {
                        graphManager.DeleteSelectedGraphElement();

                        break;
                    }
                case Keys.D1: // ��Ӧ1����
                    {
                        // ִ���߼�����                
                        LogicBaseManager logicManager = Helper.GetHelper().GetLogicManager(flowChartManager.CurrentChartMode);
                        object[] logicData = new object[2];
                        logicData[0] = 1;
                        logicData[1] = (sender as Canvas).PointToClient(Cursor.Position);
                        logicManager.DoLogicOperation(flowChartManager, LogicType.CreateAtPoint, logicData);

                        break;
                    }
                case Keys.NumPad1: // ��Ӧ1����
                    {
                        goto case Keys.D1;
                    }
                case Keys.D2: // ��Ӧ2����
                    {
                        // ִ���߼����� 
                        LogicBaseManager logicManager = Helper.GetHelper().GetLogicManager(flowChartManager.CurrentChartMode);
                        object[] logicData = new object[2];
                        logicData[0] = 2;
                        logicData[1] = (sender as Canvas).PointToClient(Cursor.Position);
                        logicManager.DoLogicOperation(flowChartManager, LogicType.CreateAtPoint, logicData);

                        break;
                    }
                case Keys.NumPad2: // ��Ӧ2����
                    {
                        goto case Keys.D2;
                    }
                case Keys.D3: // ��Ӧ3����
                    {
                        // ִ���߼����� 
                        LogicBaseManager logicManager = Helper.GetHelper().GetLogicManager(flowChartManager.CurrentChartMode);
                        object[] logicData = new object[2];
                        logicData[0] = 3;
                        logicData[1] = (sender as Canvas).PointToClient(Cursor.Position);
                        logicManager.DoLogicOperation(flowChartManager, LogicType.CreateAtPoint, logicData);

                        break;
                    }
                case Keys.NumPad3: // ��Ӧ3����
                    {
                        goto case Keys.D3;
                    }
                case Keys.D4: // ��Ӧ4����
                    {
                        // ִ���߼�����
                        LogicBaseManager logicManager = Helper.GetHelper().GetLogicManager(flowChartManager.CurrentChartMode);
                        object[] logicData = new object[2];
                        logicData[0] = 4;
                        logicData[1] = (sender as Canvas).PointToClient(Cursor.Position);
                        logicManager.DoLogicOperation(flowChartManager, LogicType.CreateAtPoint, logicData);

                        break;
                    }
                case Keys.NumPad4: // ��Ӧ4����
                    {
                        goto case Keys.D4;
                    }
                case Keys.D5: // ��Ӧ5����
                    {
                        // ִ���߼�����
                        LogicBaseManager logicManager = Helper.GetHelper().GetLogicManager(flowChartManager.CurrentChartMode);
                        object[] logicData = new object[2];
                        logicData[0] = 5;
                        logicData[1] = (sender as Canvas).PointToClient(Cursor.Position);
                        logicManager.DoLogicOperation(flowChartManager, LogicType.CreateAtPoint, logicData);

                        break;
                    }
                case Keys.NumPad5: // ��Ӧ5����
                    {
                        goto case Keys.D5;
                    }
                case Keys.Oemplus: // ��Ӧ+����
                    {
                        graphManager.CurrentCanvas.AutoScroll = false;
                        graphManager.AdjustCanvasScale(1.1f);
                        documentManager.RefreshCanvasScaleDisplay();                        

                        break;
                    }
                case Keys.OemMinus: // ��Ӧ-����
                    {                        
                        graphManager.CurrentCanvas.AutoScroll = false;
                        graphManager.AdjustCanvasScale(0.9f);
                        documentManager.RefreshCanvasScaleDisplay();                        

                        break;
                    }
                case Keys.Oemcomma: // ����
                    {
                        graphManager.SelectLineInSlotContainer();

                        break;
                    }
                case Keys.OemPeriod: // ���
                    {
                        graphManager.SelectLineOutSlotContainer();

                        break;
                    }
                case Keys.F5: // ˢ�»�ͼ��
                    {
                        graphManager.RefreshRegion();

                        break;
                    }
            }            
        }

        /// <summary>
        /// ���̵����¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_KeyUp(object sender, KeyEventArgs e)
        {
            graphManager.ControlMode = e.Control;
            graphManager.ShiftMode = e.Shift;
            graphManager.AltMode = e.Alt;
        }

        /// <summary>
        /// ��갴�������¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_MouseDown(object sender, MouseEventArgs e)
        {
            (sender as Canvas).Focus(); // ��ȡ����

            switch (e.Button)
            {
                case MouseButtons.Left: // ���������
                    {
                        graphManager.CanvasMouseDown(e.Location);

                        break;
                    }
                case MouseButtons.Right: // ����Ҽ����
                    {
                        // ��ѡ�е�ǰͼԪ���ٵ����Ҽ��˵�
                        graphManager.CheckGraphElementSelected(e.Location);
                        graphManager.ShowCanvasContextMenu(e.Location);

                        break;
                    }
                case MouseButtons.Middle: // ����м����
                    {
                        graphManager.BeginScrollCavas(e.Location);

                        break;
                    }
            }
        }

        /// <summary>
        /// ��ͼ���������¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_MouseWheel(object sender, MouseEventArgs e)
        {
            if (graphManager.ControlMode) // ��ͼ������
            {
                return; // Control������Ӧ�����⣬��ʱ���ε�
                graphManager.CurrentCanvas.AutoScroll = false;

                if (e.Delta > 0)
                {
                    graphManager.AdjustCanvasScale(1.1f);
                }
                else
                {
                    graphManager.AdjustCanvasScale(0.9f);
                }

                DocumentManager.GetDocumentManager().RefreshCanvasScaleDisplay();                
            }
            else
            {
                if (graphManager.ShiftMode) // ��shift��ʱ�������
                {
                    graphManager.CurrentCanvas.Visible = false;
                    Size moveSize = new Size(-e.Delta, e.Delta); // ��ֱ���������һ��
                    moveSize.Width = (int)((double)moveSize.Width * graphManager.CurrentBackground.CanvasSize.Width / graphManager.CurrentCanvas.ClientSize.Width * GraphSetting.GetGraphSetting().CanvasScale); // �������ĵ�λ���ͼ�嵥λ��Ҫת��
                    moveSize.Height = (int)((double)moveSize.Height * graphManager.CurrentBackground.CanvasSize.Height / graphManager.CurrentCanvas.ClientSize.Height * GraphSetting.GetGraphSetting().CanvasScale); // �������ĵ�λ���ͼ�嵥λ��Ҫת��
                    graphManager.ScrollCanvas(moveSize, false);
                    graphManager.CurrentCanvas.Visible = true;
                    graphManager.CurrentCanvas.Focus();
                }

                graphManager.CurrentCanvas.Refresh();
            }
        }

        /// <summary>
        /// ��ͼ����갴�������¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_MouseUp(object sender, MouseEventArgs e)
        {            
            graphManager.CanvasMouseUp(e.Location);
        }

        /// <summary>
        /// ��ͼ������ƶ��¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Right) // ��ֹ�Ҽ��ƶ�
            {
                graphManager.MoveGraphElement(e.Location);
            }
        }

        /// <summary>
        /// ��ͼ���������¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_MouseEnter(object sender, EventArgs e)
        {
            
        }

        /// <summary>
        /// ��ͼ������뿪�¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_MouseLeave(object sender, EventArgs e)
        {
            graphManager.RefreshCanvas();
        }

        /// <summary>
        /// ��ͼ�����˫���¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            graphManager.EditDataElement();
        }        
    }

    public class ConnectEventArgs : EventArgs
    {
        private readonly SlotGraphElement slot;
        private readonly ConnectorGraphElement connector;

        public ConnectEventArgs(SlotGraphElement slot, ConnectorGraphElement connector)
        {
            this.slot = slot;
            this.connector = connector;
        }

        public SlotGraphElement Slot
        {
            get
            {
                return slot;
            }
        }

        public ConnectorGraphElement Connector
        {
            get
            {
                return connector;
            }
        }
    }
}
