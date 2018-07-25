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
        /// ��ʼ��·������
        /// </summary>
        private void InitPatrol()
        {
            RefreshPatrol();
        }

        /// <summary>
        /// ��ʼ��Ѳ��·������
        /// </summary>
        private void InitPatrolPathTable()
        {
            int patrolCount = 0;
            m_doc.DocLogical.GetObjCount("WayPointSet", 0, ref patrolCount);

            _AtlObjInfo patrolInfo = new _AtlObjInfo();
            patrolPathTable.Rows.Clear();

            for (int i = 0; i < patrolCount; i++)
            {
                int patroPathIndex = i + 1;
                m_doc.DocLogical.GetObjDisplayInfo("WayPointSet", i, 0, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                m_doc.DocLogical.GetSetObjInfo("WayPointSet", ref patrolInfo, logicObj, 1);
                Hashtable patrolInfoTable = Helper.GetInfoTable(patrolInfo);

                DataRow dataRow = patrolPathTable.NewRow();
                dataRow["ID"] = patroPathIndex.ToString();
                dataRow["Name"] = patrolInfoTable["szName"];
                patrolPathTable.Rows.Add(dataRow);
            }
        }

        /// <summary>
        /// ��ʼ��npc��Ӫ���ݱ�
        /// </summary>
        private void InitNpcOrderTable()
        {
            string filePath = string.Format("{0}\\settings\\NpcOrder\\OrderList.tab", Helper.RootDir);
            FileInfo fi = new FileInfo(filePath);

            if (fi.Exists)
            {
                string content = FileFolderHelper.FileToString(filePath);
                string[] lines = content.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
                bool firstLine = true;

                foreach (string s in lines)
                {
                    if (firstLine)
                    {
                        firstLine = false;
                    }
                    else
                    {
                        string line = s.TrimEnd(new char[] { '\r', ' ' });
                        string[] data = line.Split(new char[] { '\t' }, StringSplitOptions.RemoveEmptyEntries);
                        string orderID = data[0];
                        string orderName = data[3];
                        string orderFile = data[1];
                        string orderFilePath = string.Format("{0}\\settings\\NpcOrder\\{1}", Helper.RootDir, orderFile);

                        int orderCount = 0;
                        FileInfo orderFileInfo = new FileInfo(orderFilePath);

                        if (orderFileInfo.Exists)
                        {
                            content = FileFolderHelper.FileToString(orderFilePath);
                            string[] orderLines = content.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
                            orderCount = orderLines.Length - 1;
                            if (orderCount < 0)
                            {
                                orderCount = 0;
                            }

                            DataRow dataRow = npcOrderTable.NewRow();
                            dataRow["ID"] = orderID;
                            dataRow["Name"] = orderName;
                            dataRow["Count"] = orderCount;
                            npcOrderTable.Rows.Add(dataRow);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// ���Ѳ����������
        /// </summary>
        private void FillPatrolPathOrderList()
        {
            if (patrolPathOrderBox.Items.Count == 0)
            {
                patrolPathOrderBox.Items.Clear();

                foreach (DataRow dataRow in BaseForm.npcOrderTable.Rows)
                {
                    patrolPathOrderBox.Items.Add(string.Format("{0} ({1})", dataRow["Name"].ToString(), dataRow["ID"].ToString()));
                }
            }            
        }

        /// <summary>
        /// ˢ��·������
        /// </summary>
        private void RefreshPatrol()
        {
            patrolTree.Nodes.Clear();

            int patrolCount = 0;
            m_doc.DocLogical.GetObjCount("WayPointSet", 0, ref patrolCount);

            if (patrolCount > 0)
            {
                _AtlObjInfo patrolInfo = new _AtlObjInfo();

                for (int i = 0; i < patrolCount; i++)
                {
                    int logicObj = 0;
                    int representObj = 0;
                    int hasScript = 0;
                    string name = "";
                    string nickName = "";

                    m_doc.DocLogical.GetObjDisplayInfo("WayPointSet", i, 0, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                    m_doc.DocLogical.GetSetObjInfo("WayPointSet", ref patrolInfo, logicObj, 1);
                    Hashtable patrolInfoTable = Helper.GetInfoTable(patrolInfo);

                    // ���·�������
                    Node patrolNode = new Node();
                    patrolNode.Text = string.Format("{0} ({1})", patrolInfoTable["szName"], i.ToString());
                    patrolNode.Tag = patrolInfo;
                    patrolTree.Nodes.Add(patrolNode);

                    int patrolIndex = int.Parse(patrolInfoTable["nIndex"] as string);
                    int patrolPointCount = 0;
                    m_doc.DocLogical.GetObjCount("WayPoint", i, ref patrolPointCount);

                    for (int j = 0; j < patrolPointCount; j++)
                    {
                        _AtlObjInfo patrolPointInfo = new _AtlObjInfo();
                        m_doc.DocLogical.GetObjDisplayInfo("WayPoint", j, i, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                        m_doc.DocLogical.GetSetObjInfo("WayPoint", ref patrolPointInfo, logicObj, 1);
                        Hashtable patrolPointInfoTable = Helper.GetInfoTable(patrolPointInfo);

                        // ˢ��·��������
                        int wayPointType = 1;

                        if (j == 0)
                        {
                            wayPointType = 0;
                        }
                        else if (j == patrolPointCount - 1)
                        {
                            wayPointType = 2;
                        }

                        m_doc.DocLogical.RefreshWayPoint(representObj, wayPointType);

                        // ���·���������
                        Node patrolPointNode = new Node();
                        patrolPointNode.Text = j.ToString();
                        patrolPointNode.Tag = patrolPointInfo;
                        patrolNode.Nodes.Add(patrolPointNode);
                    }
                }

                m_doc.DocLogical.RefreshWay();
            }
        }

        /// <summary>
        /// ����·����Ϣ
        /// </summary>        
        /// <param name="fieldName">������</param>
        /// <param name="newValue">����ֵ</param>
        /// <return>�Ƿ���³ɹ�</return>
        private bool UpdatePatrolInfo(string fieldName, string newValue)
        {
            bool updateSuccess = false;
            Node currentNode = patrolTree.SelectedNode;

            if (beginEdit && currentNode != null)
            {                
                int parentIndex = 0;
                int index = currentNode.Index;

                if (currentNode.Parent != null)
                {
                    parentIndex = currentNode.Parent.Index;
                }

                _AtlObjInfo objectInfo = (_AtlObjInfo)currentNode.Tag;

                UpdateObjectInfo(objectInfo, fieldName, newValue, index, parentIndex);
                updateSuccess = true;
            }

            return updateSuccess;
        }

        /// <summary>
        /// ·������������ѡ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void advTree1_AfterNodeSelect(object sender, AdvTreeNodeEventArgs e)
        {
            Node currentNode = patrolTree.SelectedNode;
            beginEdit = false;

            if (currentNode != null)
            {
                _AtlObjInfo objectInfo = (_AtlObjInfo)currentNode.Tag;
                Hashtable infoTable = Helper.GetInfoTable(objectInfo);
                Node cameraNode = null;

                if (currentNode.Level == 1) // ˢ��·������Ϣ
                {
                    wayPointSetParameterPanel.Enabled = false;
                    wayPointParameterPanel.Enabled = true;

                    if (infoTable["nStayFrame"] != null)
                    {
                        wayPointStayFrameBox.Text = infoTable["nStayFrame"] as string;
                    }
                    else
                    {
                        wayPointStayFrameBox.Text = "";
                    }

                    if (infoTable["nStayDirection"] != null)
                    {
                        wayPointStayDirectionBox.Text = infoTable["nStayDirection"] as string;
                    }
                    else
                    {
                        wayPointStayDirectionBox.Text = "";
                    }

                    if (infoTable["nStayAnimation"] != null)
                    {
                        wayPointStayAnimationBox.Text = infoTable["nStayAnimation"] as string;
                    }
                    else
                    {
                        wayPointStayAnimationBox.Text = "";
                    }

                    if (infoTable["bStayAniLoop"] as string == "1")
                    {
                        cWayPointAnimationLoop.Checked = true;
                    }
                    else
                    {
                        cWayPointAnimationLoop.Checked = false;
                    }

                    if (infoTable["bIsRun"] as string == "1")
                    {
                        cWayPointRun.Checked = true;
                    }
                    else
                    {
                        cWayPointRun.Checked = false;
                    }

                    if (infoTable["szScriptName"] != null)
                    {
                        wayPointScriptBox.Text = infoTable["szScriptName"] as string;
                    }
                    else
                    {
                        wayPointScriptBox.Text = "";
                    }

                    cameraNode = currentNode;
                }
                else // ˢ��·����Ϣ
                {
                    wayPointSetParameterPanel.Enabled = true;
                    wayPointParameterPanel.Enabled = false;

                    FillPatrolPathOrderList();

                    if (infoTable["szName"] != null)
                    {
                        patrolPathNameBox.Text = infoTable["szName"] as string;
                    }
                    else
                    {
                        patrolPathNameBox.Text = "";
                    }

                    if (infoTable["nPatrolPathOrderID"] != null)
                    {
                        patrolPathOrderBox.SelectedIndex = int.Parse(infoTable["nPatrolPathOrderID"].ToString()) - 1;
                    }
                    else
                    {
                        patrolPathOrderBox.SelectedIndex = -1;
                    }

                    if (infoTable["nPatrolPathWalkSpeed"] != null)
                    {
                        patrolPathWalkSpeedBox.Text = infoTable["nPatrolPathWalkSpeed"] as string;
                    }
                    else
                    {
                        patrolPathWalkSpeedBox.Text = "";
                    }

                    if (currentNode.Nodes.Count > 0)
                    {
                        cameraNode = currentNode.Nodes[0];
                    }
                }

                // �ƶ������λ��
                if (cameraNode != null)
                {
                    m_doc.DocLogical.GetObjDisplayInfo("WayPoint", cameraNode.Index, cameraNode.Parent.Index, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                    MoveCameraToRepresentObj(representObj);
                    SelectOnlyRepresentObj(representObj);
                }
            }

            beginEdit = true;
        }

        /// <summary>
        /// ���·��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            InputForm iForm = new InputForm("�½�·��", "������·������");

            if (iForm.ShowDialog() == DialogResult.OK)
            {
                int newWayPointSet = 0;
                m_doc.DocLogical.DecoratedAddOneWayPointSet(ref newWayPointSet, iForm.InputText);

                int index = patrolTree.Nodes.Count;
                _AtlObjInfo patrolInfo = new _AtlObjInfo();
                m_doc.DocLogical.GetObjDisplayInfo("WayPointSet", index, 0, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                m_doc.DocLogical.GetSetObjInfo("WayPointSet", ref patrolInfo, logicObj, 1);

                Node patrolNode = new Node();
                patrolNode.Text = string.Format("{0} ({1})", iForm.InputText, index + 1);
                patrolNode.Tag = patrolInfo;
                patrolTree.Nodes.Add(patrolNode);
                patrolTree.SelectedNode = patrolNode;

                // ˢ��·������
                InitPatrolPathTable();
            }
        }

        /// <summary>
        /// ɾ��·��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            Node currentNode = patrolTree.SelectedNode;

            if (currentNode != null && currentNode.Level == 0)
            {
                DialogResult dialogResult = MessageBox.Show("ȷ��Ҫɾ��ѡ�е�·����", "ɾ��·��",
                                                            MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                if (dialogResult == DialogResult.OK)
                {
                    int index = currentNode.Index;
                    m_doc.DocLogical.GetObjDisplayInfo("WayPointSet", index, 0, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                    m_doc.DocLogical.DecoratedDeleteWayPointSet(logicObj);
                    m_doc.DocLogical.RefreshWay();
                    patrolTree.Nodes.Remove(currentNode);

                    // ˢ��·������
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫɾ����·����", "ɾ��·��",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ˢ��·��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            RefreshPatrol();
        }

        /// <summary>
        /// ���·����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            Node currentNode = patrolTree.SelectedNode;

            if (currentNode != null)
            {
                if (currentNode.Level == 1)
                {
                    currentNode = currentNode.Parent;
                }

                _AtlObjInfo objectInfo = (_AtlObjInfo)currentNode.Tag;
                Hashtable infoTable = Helper.GetInfoTable(objectInfo);
                int parentIndex = currentNode.Index;
                int index = currentNode.Nodes.Count;
                int wayPointSetID = int.Parse(infoTable["dwSetID"] as string);

                m_doc.DocLogical.AddOneWayPoint(wayPointSetID, index.ToString());
                editMode = EditMode.AddWayPoint;
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ���·�����·����", "���·����",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ���·����Ľű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX17_Click(object sender, EventArgs e)
        {            
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Helper.RootDir;
            openFileDialog.Filter = "�ű��ļ�|*.lua";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string scriptFileName = openFileDialog.FileName;
                scriptFileName = scriptFileName.Replace(Helper.RootDir, "");
                scriptFileName = scriptFileName.TrimStart(new char[] { '\\' });
                wayPointScriptBox.Text = scriptFileName;
            }
        }

        /// <summary>
        /// ���·����Ľű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX18_Click(object sender, EventArgs e)
        {            
            wayPointScriptBox.Text = "";                
        }

        /// <summary>
        /// �Զ�Ϊ������NPC�ж�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAutoAttackPathNpc_Click(object sender, EventArgs e)
        {
            Node currentNode = patrolTree.SelectedNode;

            if (currentNode != null && currentNode.Level == 0)
            {
                // ·�����Index������ + 1
                m_doc.DocLogical.ResortNpcOrderInPatrolPath(currentNode.Index + 1);

                MessageBox.Show("�Զ�Ϊ������NPC�ж���ɣ�", "�Զ�Ϊ������NPC�ж�", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }            
        }

        /// <summary>
        /// �޸�·������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void patrolPathNameBox_TextChanged(object sender, EventArgs e)
        {
            if (UpdatePatrolInfo("szName", patrolPathNameBox.Text))
            {
                // ��Ӧ�������ˢ��
                Node currentNode = patrolTree.SelectedNode;
                int patrolPathID = currentNode.Index;

                currentNode.Text = string.Format("{0} ({1})", patrolPathNameBox.Text, patrolPathID.ToString());                
            }
        }

        /// <summary>
        /// �޸�·�������ٶ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void patrolPathWalkSpeedBox_TextChanged(object sender, EventArgs e)
        {
            UpdatePatrolInfo("nPatrolPathWalkSpeed", patrolPathWalkSpeedBox.Text);
        }

        /// <summary>
        /// �޸�·������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void patrolPathOrderBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            int patrolOrderID = patrolPathOrderBox.SelectedIndex + 1;
            UpdatePatrolInfo("nPatrolPathOrderID", patrolOrderID.ToString());
        }

        /// <summary>
        /// �޸�·����ͣ��֡��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void wayPointStayFrameBox_TextChanged(object sender, EventArgs e)
        {
            UpdatePatrolInfo("nStayFrame", wayPointStayFrameBox.Text);
        }

        /// <summary>
        /// �޸�·����ͣ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void wayPointStayDirectionBox_TextChanged(object sender, EventArgs e)
        {
            UpdatePatrolInfo("nStayDirection", wayPointStayDirectionBox.Text);
        }

        /// <summary>
        /// �޸�·����ͣ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void wayPointStayAnimationBox_TextChanged(object sender, EventArgs e)
        {
            UpdatePatrolInfo("nStayAnimation", wayPointStayAnimationBox.Text);
        }

        /// <summary>
        /// �޸�·����ű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void wayPointScriptBox_TextChanged(object sender, EventArgs e)
        {
            UpdatePatrolInfo("szScriptName", wayPointScriptBox.Text);
        }

        /// <summary>
        /// �޸��Ƿ�ѭ������ͣ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cWayPointAnimationLoop_CheckedChanged(object sender, EventArgs e)
        {
            string boolString = "0";

            if (cWayPointAnimationLoop.Checked)
            {
                boolString = "1";
            }

            UpdatePatrolInfo("bStayAniLoop", boolString);
        }

        /// <summary>
        /// �޸��Ƿ��ܲ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cWayPointRun_CheckedChanged(object sender, EventArgs e)
        {
            string boolString = "0";

            if (cWayPointRun.Checked)
            {
                boolString = "1";
            }

            UpdatePatrolInfo("bIsRun", boolString);
        }
	}
}
