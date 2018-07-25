using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DevComponents.DotNetBar;
using DevComponents.AdvTree;
using System.Threading;
using DataElementLibrary;
using System.Collections;
using DataInteractor;

namespace AIDiagramPlugin
{
    public partial class StateNodeForm : Office2007Form
    {
        private AI_State m_state = null; // ��ǰ״̬
        private FlowChartInteractor flowChartInteractor; // ��ͼ�м����
        private bool editSuccess = false; // �Ƿ�༭�ɹ�
        private int graphElementID; // ��ǰ�༭ͼԪ��ID

        /// <summary>
        /// ���캯��
        /// </summary>
        public StateNodeForm()
        {
            InitializeComponent();            
        }

        /// <summary>
        /// ��ǰ�༭ͼԪ��ID
        /// </summary>
        public int GraphElementID
        {
            set
            {
                graphElementID = value;
            }
        }

        /// <summary>
        /// �Ƿ�༭�ɹ�
        /// </summary>
        public bool EditSuccess
        {
            get
            {
                return editSuccess;
            }
        }

        /// <summary>
        /// ˢ������
        /// </summary>
        public void Reload(AI_State state)
        {
            editSuccess = false;
            allEventTree.Nodes.Clear();
            registedEventTree.Nodes.Clear();

            this.m_state = state;
            flowChartInteractor = Interactor.GetInteractor().CurrentFlowChartInteractor;

            // ��ʼ������
            if (string.IsNullOrEmpty(state.Ename))
            {
                state.Ename = string.Format("State{0}", graphElementID.ToString());
            }

            txtName.Text = state.Name;

            // ��ʼ�������¼�
            foreach (AI_Event ai_event in EventTable.EventItems)
            {
                // ȡ����ע����¼�
                if (!state.EventList.Contains(ai_event))
                {
                    Node node = new Node();
                    node.Text = ai_event.ToString();
                    node.Tag = ai_event;
                    if (int.Parse(ai_event.eventid) < 1000) // ϵͳ�¼�
                    {
                        node.Image = imageList1.Images[0];
                    }
                    else
                    {
                        node.Image = imageList1.Images[2];
                    }
                    allEventTree.Nodes.Add(node);
                }
            }

            // ��ʼ���Զ����¼�
            List<AI_Event> customEventList = flowChartInteractor.CustomTable["CustomEventList"] as List<AI_Event>;

            if (customEventList != null)
            {
                foreach (AI_Event aiEvent in customEventList)
                {
                    // ȥ����ע����¼�
                    if (!state.EventList.Contains(aiEvent))
                    {
                        Node node = new Node();
                        node.Text = aiEvent.ToString();
                        node.Tag = aiEvent;
                        node.Image = imageList1.Images[2];
                        allEventTree.Nodes.Add(node);
                    }
                }
            }

            // ��ʼ��ѡ�е��¼�         
            foreach (AI_Event aiEvent in state.EventList)
            {
                Node node = new Node();
                node.Text = aiEvent.ToString();
                node.Tag = aiEvent;

                if (int.Parse(aiEvent.eventid) < 1000) // ϵͳ�¼�
                {
                    node.Image = imageList1.Images[0];
                }
                else
                {
                    node.Image = imageList1.Images[2];
                }

                registedEventTree.Nodes.Add(node);
            }

            // �л�����
            txtName.Focus();
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            // �����Լ��
            if (txtName.Text != "")
            {
                // �޸�����
                this.m_state.Name = txtName.Text;

                // ���¿����¼�
                this.m_state.EventList.Clear();
                foreach (Node node in registedEventTree.Nodes)
                {
                    this.m_state.EventList.Add(node.Tag as AI_Event);
                }

                editSuccess = true;
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("״̬������Ϊ�գ�", "�༭״̬���", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }                                   
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// ����¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void btnAdd_Click(object sender, EventArgs e)
        {
            Node node = allEventTree.SelectedNode;

            if (node != null)
            {
                allEventTree.Nodes.Remove(node);
                registedEventTree.Nodes.Add(node);
            }
        }

        /// <summary>
        /// ɾ���¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void btnDel_Click(object sender, EventArgs e)
        {
            Node node = registedEventTree.SelectedNode;

            if (node != null)
            {
                registedEventTree.Nodes.Remove(node);
                allEventTree.Nodes.Add(node);
            }
        }        

        /// <summary>
        /// �½��Զ����¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            string eventName = textBoxX1.Text;

            if (eventName != "")
            {
                List<AI_Event> customEventList = flowChartInteractor.CustomTable["CustomEventList"] as List<AI_Event>;

                if (customEventList == null)
                {
                    customEventList = new List<AI_Event>();
                }

                bool nameExist = false;

                foreach (AI_Event aiEvent in customEventList)
                {
                    if (aiEvent.Name == eventName)
                    {
                        nameExist = true;
                        break;
                    }
                }

                if (nameExist) // �¼����Ѿ�����
                {
                    MessageBox.Show("�����Ƶ��¼��Ѿ����ڣ�", "�½��Զ����¼�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    string customEventIndexString = flowChartInteractor.CustomTable["CustomEventIndex"] as string;
                    int customEventIndex = 1;
                    if(customEventIndexString != null)
                    {
                        customEventIndex = int.Parse(customEventIndexString);
                    }
                    int eventID = customEventIndex + 2000; // ID2000���ϵ�Ϊ�ű����Զ����¼�

                    AI_Event aiEvent = new AI_Event();
                    aiEvent.Name = eventName;
                    aiEvent.Ename = "CustomEvent";
                    aiEvent.DBID = "0";
                    aiEvent.eventid = eventID.ToString();
                    customEventList.Add(aiEvent);
                    customEventIndex++;

                    Node node = new Node();
                    node.Text = aiEvent.ToString();
                    node.Tag = aiEvent;
                    node.Image = imageList1.Images[0];
                    allEventTree.Nodes.Add(node);
                    allEventTree.SelectedNode = node;

                    flowChartInteractor.CustomTable["CustomEventList"] = customEventList;
                    flowChartInteractor.CustomTable["CustomEventIndex"] = customEventIndex.ToString(); 
                }
            }
            else
            {
                MessageBox.Show("���������Զ����¼������ƣ�", "�½��Զ����¼�", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ɾ���Զ����¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            Node currentNode = allEventTree.SelectedNode;

            if (currentNode != null)
            {
                AI_Event aiEvent = currentNode.Tag as AI_Event;

                if (int.Parse(aiEvent.eventid) > 2000) // ����Ƿ��ǻ�ͼ�����Զ����¼�
                {
                    List<AI_Event> customEventList = flowChartInteractor.CustomTable["CustomEventList"] as List<AI_Event>;
                    customEventList.Remove(aiEvent);
                    allEventTree.Nodes.Remove(currentNode);
                }
                else
                {
                    MessageBox.Show("ֻ��ɾ����ͼ�ڴ������Զ����¼���", "ɾ���Զ����¼�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫɾ�����Զ����¼���", "ɾ���Զ����¼�", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ����ر��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void StateNodeForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Visible = false;
        }

        /// <summary>
        /// �����¼��л�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void advTree1_AfterNodeSelect(object sender, AdvTreeNodeEventArgs e)
        {
            Node currentNode = allEventTree.SelectedNode;

            if (currentNode != null)
            {
                AI_Event aiEvent = currentNode.Tag as AI_Event;
                string description = aiEvent.Info;

                if (string.IsNullOrEmpty(description))
                {
                    description = aiEvent.Name;
                }

                informationBox.Text = description;
            }
        }

        /// <summary>
        /// ע���¼��л�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void advTree2_AfterNodeSelect(object sender, AdvTreeNodeEventArgs e)
        {
            Node currentNode = registedEventTree.SelectedNode;

            if (currentNode != null)
            {
                AI_Event aiEvent = currentNode.Tag as AI_Event;
                string description = aiEvent.Info;

                if (string.IsNullOrEmpty(description))
                {
                    description = aiEvent.Name;
                }

                informationBox.Text = description;
            }
        }

        /// <summary>
        /// �������Զ����¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX5_Click(object sender, EventArgs e)
        {
            Node eventNode = allEventTree.SelectedNode;
            
            if (eventNode != null)
            {
                string eventName = textBoxX1.Text;  
 
                if (eventName != "")
                {
                    AI_Event aiEvent = eventNode.Tag as AI_Event;

                    if (aiEvent != null && int.Parse(aiEvent.eventid) > 2000)
                    {
                        aiEvent.Name = eventName;
                        eventNode.Text = aiEvent.ToString();
                    }
                    else
                    {
                        MessageBox.Show("ֻ����������ͼ�ڴ������Զ����¼���", "�������Զ����¼�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
                else
                {
                    MessageBox.Show("���������Զ����¼������ƣ�", "�������Զ����¼�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }                                
            }
            else
            {                
                MessageBox.Show("����ѡ��Ҫ���������Զ����¼���", "�������Զ����¼�", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}