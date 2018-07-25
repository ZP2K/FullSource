using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;
using System.Threading;
using System.Diagnostics;
using LuaInterface;
using DevComponents.AdvTree;
using DevComponents.DotNetBar;
using AtlKG3DEngineProxyLib;

namespace DesignerSceneEditor
{
    unsafe public partial class BaseForm
	{
        /// <summary>
        /// ��ʼ����ͨ����
        /// </summary>
        private void InitTraffic()
        {
            DataGridViewTextBoxColumn argumentValueColumn = new DataGridViewTextBoxColumn();
            argumentValueColumn.Name = "ArgumentValue";
            argumentValueColumn.HeaderText = "����ֵ";
            dataGridViewX2.Columns.Add(argumentValueColumn);

            RefreshTraffic();
        }

        /// <summary>
        /// ˢ�½�ͨ����
        /// </summary>
        private void RefreshTraffic()
        {
            trafficTree.Nodes.Clear();

            int trafficCount = 0;
            m_doc.DocLogical.GetObjCount("TrafficLittlePointSet", 0, ref trafficCount);
            if (trafficCount > 0)
            {
                _AtlObjInfo trafficInfo = new _AtlObjInfo();

                for (int i = 0; i < trafficCount; i++)
                {
                    m_doc.DocLogical.GetObjDisplayInfo("TrafficLittlePointSet", i, 0, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                    m_doc.DocLogical.GetSetObjInfo("TrafficLittlePointSet", ref trafficInfo, logicObj, 1);
                    Hashtable trafficInfoTable = Helper.GetInfoTable(trafficInfo);

                    int trafficPointCount = 0;
                    m_doc.DocLogical.GetObjCount("TrafficLittlePoint", i, ref trafficPointCount);

                    Node trafficNode = new Node();
                    trafficNode.Text = string.Format("{0} ({1})", trafficInfoTable["szName"], trafficPointCount.ToString());
                    trafficNode.Tag = trafficInfo;
                    trafficTree.Nodes.Add(trafficNode);

                    for (int j = 0; j < trafficPointCount; j++)
                    {
                        _AtlObjInfo trafficPointInfo = new _AtlObjInfo();
                        m_doc.DocLogical.GetObjDisplayInfo("TrafficLittlePoint", j, i, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                        m_doc.DocLogical.GetSetObjInfo("TrafficLittlePoint", ref trafficPointInfo, logicObj, 1);
                        Hashtable patrolPointInfoTable = Helper.GetInfoTable(trafficPointInfo);

                        Node trafficPointNode = new Node();
                        trafficPointNode.Text = j.ToString();
                        trafficPointNode.Tag = trafficPointInfo;
                        trafficNode.Nodes.Add(trafficPointNode);
                    }
                }

                m_doc.DocLogical.RefreshTraffic();
            }
        }

        /// <summary>
        /// �������ͨ·���ͽ�ͨ�㵥Ԫ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void dataGridViewX2_CellMouseDown(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.ColumnIndex >= 0 && e.RowIndex >= 0)
            {
                DataGridViewCell currentCell = dataGridViewX2[e.ColumnIndex, e.RowIndex];

                string propertyName = currentCell.OwningRow.Tag as string;
                string oldValue = currentCell.EditedFormattedValue as string;
                string typeValue = "��ͨ·��";
                if (trafficTree.SelectedNode.Level == 1)
                {
                    typeValue = "��ͨ��";
                }
                LogicManager logicManager = LogicManager.GetLogicManager();

                bool editSuccess = logicManager.ConfigEdit(string.Format("�༭{0}����", typeValue), propertyName, oldValue);
                if (editSuccess)
                {
                    currentCell.Value = logicManager.NewValue;
                }
            }
        }

        /// <summary>
        /// ��ͨ·���ͽ�ͨ��ֵ�ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void dataGridViewX2_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (beginEdit && e.RowIndex >= 0)
            {
                DataGridViewCell currentCell = dataGridViewX2[e.ColumnIndex, e.RowIndex];
                _AtlObjInfo objectInfo = (_AtlObjInfo)trafficTree.SelectedNode.Tag;
                int parentIndex = 0;
                int index = trafficTree.SelectedNode.Index;
                int fieldIndex = GetFieldIndex(objectInfo, currentCell.OwningRow.Tag as string);

                if (trafficTree.SelectedNode.Level == 1) // �༭·����
                {
                    parentIndex = trafficTree.SelectedNode.Parent.Index;
                }

                SaveObjectData(currentCell, objectInfo, index, parentIndex);
            }
        }

        /// <summary>
        /// ѡ��ͨ·������ͨ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void advTree2_AfterNodeSelect(object sender, AdvTreeNodeEventArgs e)
        {
            Node currentNode = trafficTree.SelectedNode;
            beginEdit = false;
            dataGridViewX2.Rows.Clear();

            if (currentNode != null)
            {
                _AtlObjInfo objectInfo = (_AtlObjInfo)currentNode.Tag;
                Hashtable infoTable = Helper.GetInfoTable(objectInfo);
                List<string> fieldList = new List<string>();
                List<string> displayFieldList = new List<string>();
                Node cameraNode = currentNode;

                if (currentNode.Level == 1)
                {
                    fieldList.Add("vPosition");
                    displayFieldList.Add("λ��");

                    cameraNode = currentNode;
                }
                else
                {
                    fieldList.Add("szName");
                    displayFieldList.Add("·����");
                    fieldList.Add("m_dwCostMoney");
                    displayFieldList.Add("�ķѽ�Ǯ");
                    fieldList.Add("m_dwVelocity");
                    displayFieldList.Add("VeloCity");
                    fieldList.Add("m_dwStep");
                    displayFieldList.Add("С����");
                    fieldList.Add("m_dwFromID");
                    displayFieldList.Add("FromID");
                    fieldList.Add("m_dwToID");
                    displayFieldList.Add("ToID");

                    if (currentNode.Nodes.Count > 0)
                    {
                        cameraNode = currentNode.Nodes[0];
                    }
                }

                // ˢ�����Կ�
                for (int i = 0; i < fieldList.Count; i++)
                {
                    dataGridViewX2.Rows.Add(1);
                    DataGridViewRow newRow = dataGridViewX2.Rows[i];
                    newRow.HeaderCell.Value = displayFieldList[i];
                    newRow.Cells["ArgumentValue"].Value = infoTable[fieldList[i]];
                    newRow.Cells["ArgumentValue"].Tag = infoTable[fieldList[i]];
                    newRow.Tag = fieldList[i];
                }

                // �ƶ������λ��
                if (cameraNode != null)
                {
                    m_doc.DocLogical.GetObjDisplayInfo("TrafficLittlePoint", cameraNode.Index, cameraNode.Parent.Index, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                    MoveCameraToRepresentObj(representObj);
                    SelectOnlyRepresentObj(representObj);                   
                }
            }

            beginEdit = true;
        }

        /// <summary>
        /// ɾ����ͨ·������ͨ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX9_Click(object sender, EventArgs e)
        {
            Node currentNode = trafficTree.SelectedNode;
            NodeCollection parentNodes;

            if (currentNode != null)
            {
                DialogResult dialogResult = MessageBox.Show("ȷ��Ҫɾ��ѡ�еĽ�ͨ·���ͽ�ͨ����", "ɾ����ͨ·���ͽ�ͨ��",
                                                            MessageBoxButtons.OKCancel, MessageBoxIcon.Information);

                if (dialogResult == DialogResult.OK)
                {
                    _AtlObjInfo objectInfo = (_AtlObjInfo)currentNode.Tag;
                    int index = currentNode.Index;
                    int parentIndex = 0;

                    if (currentNode.Level == 0) // ɾ����ͨ·��
                    {
                        m_doc.DocLogical.GetObjDisplayInfo("TrafficLittlePointSet", index, parentIndex, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                        m_doc.DocLogical.GetSetObjInfo("TrafficLittlePointSet", ref objectInfo, logicObj, 1);
                        parentNodes = trafficTree.Nodes;
                    }
                    else // ɾ����ͨ��
                    {
                        parentIndex = currentNode.Parent.Index;
                        m_doc.DocLogical.GetObjDisplayInfo("TrafficLittlePoint", index, parentIndex, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                        m_doc.DocLogical.GetSetObjInfo("TrafficLittlePoint", ref objectInfo, logicObj, 1);
                        parentNodes = currentNode.Parent.Nodes;
                    }

                    m_doc.DocLogical.DeleteTrafficPointAndSet(logicObj, currentNode.Level);
                    parentNodes.Remove(currentNode);

                    RefreshTraffic();
                }
            }
            else
            {
                MessageBox.Show("��ѡѡ��Ҫɾ���Ľ�ͨ·���ͽ�ͨ�㣡", "ɾ����ͨ·���ͽ�ͨ��",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }


        /// <summary>
        /// ˢ�½�ͨ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX5_Click(object sender, EventArgs e)
        {
            RefreshTraffic();
        }

        /// <summary>
        /// ��Ӵ�ͨ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX6_Click(object sender, EventArgs e)
        {
            m_doc.DocLogical.AddOneBigTrafficPoint();
        }

        /// <summary>
        /// ��ĳ��ǰ���С��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX8_Click(object sender, EventArgs e)
        {
            m_doc.DocLogical.InsertTrafficPoint();
            RefreshTraffic();
        }

        /// <summary>
        /// ��ӽ�ͨ·����С��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX7_Click(object sender, EventArgs e)
        {
            Node currentNode = trafficTree.SelectedNode;
            int trafficID = 0;
            int addPoint = 0;

            if (currentNode != null && currentNode.Level == 0)
            {
                _AtlObjInfo objectInfo = (_AtlObjInfo)currentNode.Tag;
                Hashtable infoTable = Helper.GetInfoTable(objectInfo);
                trafficID = int.Parse(infoTable["m_dwID"] as string);
            }

            m_doc.DocLogical.DecoratedAddOneTrafficPointSet(trafficID, addPoint);
            editMode = EditMode.AddTrafficPoint;
        }

	}
}
