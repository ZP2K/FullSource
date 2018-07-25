using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Collections;
using System.Windows.Forms;
using DevComponents.AdvTree;
using AtlKG3DEngineProxyLib;

namespace DesignerSceneEditor
{
	public partial class GroupManageForm: Form
	{
        private BaseDoc baseDoc; // BaseDoc
        private List<_AtlObjInfo> npcReviveGroupInfoList = new List<_AtlObjInfo>(); // npc��������Ϣ����
        private List<_AtlObjInfo> doodadReviveGroupInfoList = new List<_AtlObjInfo>(); // doodad��������Ϣ����
        private List<_AtlObjInfo> aiGroupInfoList = new List<_AtlObjInfo>(); // �������Ϣ����
        private List<_AtlObjInfo> randomGroupInfoList = new List<_AtlObjInfo>(); // �������Ϣ����
        private List<string[]> aiParameterGroupInfoList = new List<string[]>(); // AI��������Ϣ����

        private List<Hashtable> npcInfoList = new List<Hashtable>(); // npc��������
        private List<Hashtable> doodadInfoList = new List<Hashtable>(); // doodad��������

        // ��GetObjDisplayInfo�õ���ʱ������
        int logicObj = 0;
        int representObj = 0;
        int hasScript = 0;
        string name = string.Empty;
        string nickName = string.Empty;
        int templateID = -1;


        /// <summary>
        /// ���캯��
        /// </summary>
		public GroupManageForm()
		{
			InitializeComponent();
		}

        /// <summary>
        /// BaseDoc
        /// </summary>
        public BaseDoc CurrentBaseDoc
        {
            set
            {
                baseDoc = value;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        public void Init()
        {
            InitReviveGroup();
            FillReviveGroup();
            
            InitHatredGroup();
            FillHatredGroup();
            
            InitRandomGroup();
            FillRandomGroup();

            InitAIParameterGroup();
            FillAIParameterGroup();

            InitMemberInfoList("NPC", npcInfoList);
            InitMemberInfoList("Doodad", doodadInfoList);

            // ��ʼ��������
            DataGridViewTextBoxColumn groupInfoNameColumn = new DataGridViewTextBoxColumn();
            groupInfoNameColumn.HeaderText = "�������";
            groupInfoNameColumn.Name = "GroupInfoName";
            groupInfoNameColumn.ReadOnly = true;

            DataGridViewTextBoxColumn groupInfoValueColumn = new DataGridViewTextBoxColumn();
            groupInfoValueColumn.HeaderText = "����ֵ";
            groupInfoValueColumn.Name = "GroupInfoValue";

            groupView.Columns.Add(groupInfoNameColumn);
            groupView.Columns.Add(groupInfoValueColumn);

            DataGridViewTextBoxColumn npcNameColumn = new DataGridViewTextBoxColumn();
            npcNameColumn.HeaderText = "����";
            npcNameColumn.Name = "NpcName";

            DataGridViewTextBoxColumn npcTemplateIDColumn = new DataGridViewTextBoxColumn();
            npcTemplateIDColumn.HeaderText = "ģ��ID";
            npcTemplateIDColumn.Name = "NpcTemplateID";

            DataGridViewTextBoxColumn npcIndexColumn = new DataGridViewTextBoxColumn();
            npcIndexColumn.HeaderText = "���";
            npcIndexColumn.Name = "NpcIndex";

            npcView.Columns.Add(npcNameColumn);
            npcView.Columns.Add(npcTemplateIDColumn);
            npcView.Columns.Add(npcIndexColumn);
        }

        /// <summary>
        /// ��ʼ����Ա��Ϣ����
        /// </summary>
        /// <param name="objectType">��������</param>
        /// <param name="memberInfoList">��Ա��Ϣ����</param>
        private void InitMemberInfoList(string objectType, List<Hashtable> memberInfoList)
        {
            int logicObj = 0;
            int representObj = 0;
            int hasScript = 0;
            string name = "";
            string nickName = "";
            int memberCount = 0;

            memberInfoList.Clear();
            baseDoc.DocLogical.GetObjCount(objectType, 0, ref memberCount);

            for (int i = 0; i < memberCount; i++)
            {
                _AtlObjInfo objectInfo = new _AtlObjInfo();
                baseDoc.DocLogical.GetObjDisplayInfo(objectType, i, 0, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);

                if (logicObj != 0) // ò����Ч�ĳ�ԱҲ�����
                {
                    baseDoc.DocLogical.GetSetObjInfo(objectType, ref objectInfo, logicObj, 1);
                    Hashtable infoTable = Helper.GetInfoTable(objectInfo);
                    memberInfoList.Add(infoTable);
                }
            }
        }

        /// <summary>
        /// ��ʼ����������
        /// </summary>
        /// <param name="groupName">��������</param>
        /// <param name="groupInfoList">��������</param>
        private void InitGroupInfoList(string groupName, List<_AtlObjInfo> groupInfoList)
        {
            int groupCount = 0;            
            _AtlObjInfo objectInfo = new _AtlObjInfo();

            groupInfoList.Clear();
            baseDoc.DocLogical.GetObjCount(groupName, 0, ref groupCount);            

            for (int i = 0; i < groupCount; i++)
            {
                baseDoc.DocLogical.GetObjDisplayInfo(groupName, i, 0, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                baseDoc.DocLogical.GetSetObjInfo(groupName, ref objectInfo, logicObj, 1);
                groupInfoList.Add(objectInfo);
            }
        }

        /// <summary>
        /// ��ʼ�������
        /// </summary>
        private void InitHatredGroup()
        {
            InitGroupInfoList("AIGroup", aiGroupInfoList);
        }

        /// <summary>
        /// ��ʼ�������
        /// </summary>
        private void InitRandomGroup()
        {
            InitGroupInfoList("NpcRandomGroup", randomGroupInfoList);
        }

        /// <summary>
        /// ��ʼ��AI������
        /// </summary>
        private void InitAIParameterGroup()
        {
            string info = "";

            aiParameterGroupInfoList.Clear();
            baseDoc.DocLogical.GetAIParameterGroupInfo(ref info);

            string[] infoArray = info.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);

            foreach (string s in infoArray)
            {
                aiParameterGroupInfoList.Add(s.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries));
            }
        }

        /// <summary>
        /// ��ʼ��������
        /// </summary>
        private void InitReviveGroup()
        {
            InitGroupInfoList("NpcReviveGroup", npcReviveGroupInfoList);
            InitGroupInfoList("DoodadReviveGroup", doodadReviveGroupInfoList);
        }

        /// <summary>
        /// �����������
        /// </summary>
        /// <param name="idField">����ID�ֶ���</param>
        /// <param name="nameField">���������ֶ���</param>
        /// <param name="groupInfoList">������Ϣ����</param>
        /// <param name="groupTree">������</param>
        private void FillGroupTree(string idField, string nameField, List<_AtlObjInfo> groupInfoList, AdvTree groupTree)
        {
            groupTree.Nodes.Clear();

            foreach (_AtlObjInfo objectInfo in groupInfoList)
            {
                Hashtable infoTable = Helper.GetInfoTable(objectInfo);
                string groupID = infoTable[idField] as string;
                string groupName = infoTable[nameField] as string;
                Node newNode = new Node();
                newNode.Text = string.Format("{0} ({1})", groupName, groupID);
                newNode.Tag = infoTable;

                groupTree.Nodes.Add(newNode);
            }
        }

        /// <summary>
        /// �������
        /// </summary>
        private void FillHatredGroup()
        {
            FillGroupTree("dwSetID", "szName", aiGroupInfoList, aiTree);
        }

        /// <summary>
        /// ��������
        /// </summary>
        private void FillRandomGroup()
        {
            FillGroupTree("dwGroupID", "szName", randomGroupInfoList, randomTree);
        }

        /// <summary>
        /// ���AI������
        /// </summary>
        private void FillAIParameterGroup()
        {
            aiParameterTree.Nodes.Clear();

            for (int i = 0; i < aiParameterGroupInfoList.Count; i++)
            {
                string[] infoArray = aiParameterGroupInfoList[i];

                Node newNode = new Node();

                string[] nodeInfo = infoArray[0].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                newNode.Text = string.Format("{0} ({1})", nodeInfo[0], nodeInfo[1]);

                List<string[]> npcInfo = new List<string[]>();

                for (int j = 1; j < infoArray.Length; j++)
                {
                    npcInfo.Add(infoArray[j].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries));
                }

                newNode.Tag = npcInfo;
                aiParameterTree.Nodes.Add(newNode);
            }
        }

        /// <summary>
        /// ���������
        /// </summary>
        private void FillReviveGroup()
        {
            reviveTree.Nodes.Clear();

            Node npcReviveNode = new Node();
            npcReviveNode.Text = "NPC";
            reviveTree.Nodes.Add(npcReviveNode);

            foreach(_AtlObjInfo objectInfo in npcReviveGroupInfoList)
            {
                Hashtable infoTable = Helper.GetInfoTable(objectInfo);
                string groupID = infoTable["dwGroupID"] as string;
                string groupName = infoTable["szName"] as string;
                Node newNode = new Node();
                newNode.Text = string.Format("{0} ({1})", groupName, groupID);
                newNode.Tag = infoTable;

                npcReviveNode.Nodes.Add(newNode);
            }

            Node doodadReviveNode = new Node();
            doodadReviveNode.Text = "Doodad";
            reviveTree.Nodes.Add(doodadReviveNode);

            foreach (_AtlObjInfo objectInfo in doodadReviveGroupInfoList)
            {
                Hashtable infoTable = Helper.GetInfoTable(objectInfo);
                string groupID = infoTable["dwGroupID"] as string;
                string groupName = infoTable["szName"] as string;
                Node newNode = new Node();
                newNode.Text = string.Format("{0} ({1})", groupName, groupID);
                newNode.Tag = infoTable;

                doodadReviveNode.Nodes.Add(newNode);
            }
        }

        /// <summary>
        /// ���³����
        /// </summary>
        /// <return>�Ƿ���³ɹ�</return>
        private bool UpdateHatredGroup()
        {
            bool updateSuccess = false;
            Node currentNode = aiTree.SelectedNode;

            if (currentNode != null)
            {
                Hashtable infoTable = currentNode.Tag as Hashtable;
                int groupID = int.Parse(infoTable["dwSetID"] as string);
                string groupName = groupView.Rows[0].Cells["GroupInfoValue"].Value as string;

                if (!string.IsNullOrEmpty(groupName))
                {
                    baseDoc.DocLogical.DecoratedSetAIGroup(groupID, groupName);

                    infoTable["szName"] = groupName;
                    currentNode.Text = string.Format("{0} ({1})", groupName, groupID.ToString());
                    updateSuccess = true;
                }
                else
                {
                    MessageBox.Show("�������ݲ�������Ϊ�գ�", "���·�������", MessageBoxButtons.OK,
                                    MessageBoxIcon.Information);
                }
            }

            return updateSuccess;
        }

        /// <summary>
        /// ���������
        /// </summary>
        /// <return>�Ƿ���³ɹ�</return>
        private bool UpdateRandomGroup()
        {
            bool updateSuccess = false;
            Node currentNode = randomTree.SelectedNode;

            if (currentNode != null)
            {
                Hashtable infoTable = currentNode.Tag as Hashtable;
                int groupID = int.Parse(infoTable["dwGroupID"] as string);
                string groupName = groupView.Rows[0].Cells["GroupInfoValue"].Value as string;
                string groupInfo = groupView.Rows[1].Cells["GroupInfoValue"].Value as string;

                if (!string.IsNullOrEmpty(groupName) && !string.IsNullOrEmpty(groupInfo))
                {
                    baseDoc.DocLogical.DecoratedSetRandomGroup(groupID, groupName, groupInfo);

                    infoTable["szName"] = groupName;
                    currentNode.Text = string.Format("{0} ({1})", groupName, groupID.ToString());
                    updateSuccess = true;
                }
                else
                {
                    MessageBox.Show("�������ݲ�������Ϊ�գ�", "���·�������", MessageBoxButtons.OK,
                                    MessageBoxIcon.Information);
                }
            }

            return updateSuccess;
        }

        /// <summary>
        /// ����������
        /// </summary>
        private bool UpdateReviveGroup()
        {
            bool updateSuccess = false;
            Node currentNode = reviveTree.SelectedNode;

            if (currentNode != null && currentNode.Level == 1)
            {
                Hashtable infoTable = currentNode.Tag as Hashtable;
                int groupID = int.Parse(infoTable["dwGroupID"] as string);
                string groupName = groupView.Rows[0].Cells["GroupInfoValue"].Value as string;
                string maxString = groupView.Rows[1].Cells["GroupInfoValue"].Value as string;
                string minString = groupView.Rows[2].Cells["GroupInfoValue"].Value as string;

                if (!string.IsNullOrEmpty(groupName) && !string.IsNullOrEmpty(maxString) && !string.IsNullOrEmpty(minString))
                {
                    int maxCount = int.Parse(maxString);
                    int minCount = int.Parse(minString);

                    switch (currentNode.Parent.Text)
                    {
                        case "NPC":
                            {
                                baseDoc.DocLogical.DecoratedSetNpcReviveGroup(groupID, groupName, minCount, maxCount);
                                break;
                            }
                        case "Doodad":
                            {
                                baseDoc.DocLogical.DecoratedSetDoodadReviveGroup(groupID, groupName, minCount, maxCount);
                                break;
                            }
                    }
                    
                    infoTable["szName"] = groupName;
                    infoTable["nMinNpcCount"] = minString;
                    infoTable["nMaxNpcCount"] = maxString;
                    currentNode.Text = string.Format("{0} ({1})", groupName, groupID.ToString());
                    updateSuccess = true;                    
                }
                else
                {
                    MessageBox.Show("�������ݲ�������Ϊ�գ�", "���·�������", MessageBoxButtons.OK,
                                    MessageBoxIcon.Information);
                }
            }

            return updateSuccess;
        }

        /// <summary>
        /// ɾ��������
        /// </summary>        
        private void DeleteReviveGroup()
        {
            Node currentNode = reviveTree.SelectedNode;

            if (currentNode != null)
            {
                if (currentNode.Level == 1)
                {
                    if (MessageBox.Show("ȷ��Ҫɾ��ѡ�еķ�����", "ɾ������", MessageBoxButtons.OKCancel,
                                        MessageBoxIcon.Question) == DialogResult.OK)
                    {
                        Hashtable infoTable = currentNode.Tag as Hashtable;
                        int groupID = int.Parse(infoTable["dwGroupID"] as string);

                        switch (currentNode.Parent.Text)
                        {
                            case "NPC":
                                {
                                    baseDoc.DocLogical.DecoratedDeleteNpcReviveGroup(groupID);
                                    break;
                                }
                            case "Doodad":
                                {
                                    baseDoc.DocLogical.DecoratedDeleteDoodadReviveGroup(groupID);
                                    break;
                                }
                        }
                    }

                    Node parentNode = currentNode.Parent;
                    parentNode.Nodes.Remove(currentNode);                    
                }
                else
                {
                    MessageBox.Show("����ɾ�������㣡", "ɾ������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }                
            }
        }

        /// <summary>
        /// ɾ�������
        /// </summary>
        private void DeleteHatredGroup()
        {
            Node currentNode = aiTree.SelectedNode;

            if (currentNode != null)
            {
                Hashtable infoTable = currentNode.Tag as Hashtable;
                string groupID = infoTable["dwSetID"] as string;

                // �ȸ�������NPC�ķ�����Ϣ
                string groupField = "TeamID";
                int npcCount = 0;
                baseDoc.DocLogical.GetObjCount("NPC", 0, ref npcCount);

                for (int i = 0; i < npcCount; i++)
                {
                    _AtlObjInfo objectInfo = new _AtlObjInfo();
                    baseDoc.DocLogical.GetObjDisplayInfo("NPC", i, 0, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                    baseDoc.DocLogical.GetSetObjInfo("NPC", ref objectInfo, logicObj, 1);

                    for (int j = 0; j < objectInfo.strFields.Length; j++)
                    {
                        string fieldName = objectInfo.strFields[j];

                        if (fieldName == groupField)
                        {
                            if (objectInfo.strValues[j] == groupID)
                            {
                                objectInfo.strValues[j] = "0";
                                objectInfo.bChanged = 1;
                                baseDoc.DocLogical.GetSetObjInfo("NPC", ref objectInfo, logicObj, 0);
                            }

                            break;
                        }
                    }
                }

                // ��ɾ���÷���
                baseDoc.DocLogical.DecoratedDeleteAIGroup(int.Parse(groupID));
                aiTree.Nodes.Remove(currentNode);  
            }
        }

        /// <summary>
        /// ɾ�������
        /// </summary>
        private void DeleteRandomGroup()
        {
            Node currentNode = randomTree.SelectedNode;

            if (currentNode != null)
            {
                if (currentNode.Level == 1)
                {
                    if (MessageBox.Show("ȷ��Ҫɾ��ѡ�еķ�����", "ɾ������", MessageBoxButtons.OKCancel,
                                        MessageBoxIcon.Question) == DialogResult.OK)
                    {
                        Hashtable infoTable = currentNode.Tag as Hashtable;
                        int groupID = int.Parse(infoTable["dwGroupID"] as string);

                        baseDoc.DocLogical.DecoratedDeleteRandomGroup(groupID);
                    }

                    Node parentNode = currentNode.Parent;
                    parentNode.Nodes.Remove(currentNode);
                }
                else
                {
                    MessageBox.Show("����ɾ�������㣡", "ɾ������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        /// <summary>
        /// ��ȡNPC����Ϣ���ݱ�
        /// </summary>
        /// <param name="npcIndex">npc���</param>
        /// <returns>��Ϣ���ݱ�</returns>
        private Hashtable GetNpcInfoTable(string npcIndex)
        {
            Hashtable npcInfo = null;

            foreach (Hashtable infoTable in npcInfoList)
            {
                if (infoTable["nIndex"] as string == npcIndex)
                {
                    npcInfo = infoTable;
                    break;
                }
            }

            return npcInfo;
        }

        /// <summary>
        /// ��ʾ��Ա��Ϣ
        /// </summary>
        /// <param name="memberInfoList">��Ա��Ϣ����</param>
        /// <param name="fieldName">�ֶ���</param>
        /// <param name="fieldValue">�ֶ�ֵ</param>
        private void ShowMemberInfo(List<Hashtable> memberInfoList, string fieldName, string fieldValue)
        {
            for (int i = 0; i < memberInfoList.Count; i++)
            {
                Hashtable infoTable = memberInfoList[i];

                string memberName = infoTable["szName"] as string;
                string memberTemplateID = infoTable["nTempleteID"] as string;
                string memberIndex = infoTable["nIndex"] as string;
                string currentFieldValue = infoTable[fieldName] as string;

                if (currentFieldValue == fieldValue)
                {
                    npcView.Rows.Add(1);
                    DataGridViewRow newRow = npcView.Rows[npcView.Rows.Count - 1];
                    newRow.Cells["NpcName"].Value = memberName;
                    newRow.Cells["NpcTemplateID"].Value = memberTemplateID;
                    newRow.Cells["NpcIndex"].Value = memberIndex;
                }
            }
        }

        /// <summary>
        /// ��ȡAI�������ϵ�ID
        /// </summary>
        /// <param name="nodeText">�ı�</param>
        /// <returns>ID</returns>
        private int GetAIParameterSetID(string nodeText)
        {            
            int index = nodeText.LastIndexOf("(");
            string idString = nodeText.Substring(index + 1, nodeText.Length - index - 2);
            int aiSetID = int.Parse(idString);

            return aiSetID;
        }

        /// <summary>
        /// ��ʾ����ĳ�Ա��Ϣ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void ShowGroupMember(object sender, AdvTreeNodeEventArgs e)
        {
            Node currentNode;
            Hashtable infoTable;
            string groupID;
            string groupName;
            DataGridViewRow newRow;

            groupView.Rows.Clear();
            npcView.Rows.Clear();

            switch(tabControl1.SelectedTab.Text)
            {
                case "������":
                    {
                        currentNode = reviveTree.SelectedNode;

                        if (currentNode != null && currentNode.Level == 1)
                        {
                            infoTable = currentNode.Tag as Hashtable;
                            groupID = infoTable["dwGroupID"] as string;
                            groupName = infoTable["szName"] as string;
                            string minCount = infoTable["nMinNpcCount"] as string;
                            string maxCount = infoTable["nMaxNpcCount"] as string;
                            string randomSeed = infoTable["nRandom"] as string;

                            groupView.Rows.Add(1);
                            newRow = groupView.Rows[0];
                            newRow.Cells["GroupInfoName"].Value = "����";
                            newRow.Cells["GroupInfoValue"].Value = groupName;

                            groupView.Rows.Add(1);
                            newRow = groupView.Rows[1];
                            newRow.Cells["GroupInfoName"].Value = "MaxCount";
                            newRow.Cells["GroupInfoValue"].Value = maxCount;

                            groupView.Rows.Add(1);
                            newRow = groupView.Rows[2];
                            newRow.Cells["GroupInfoName"].Value = "MinCount";
                            newRow.Cells["GroupInfoValue"].Value = minCount;

                            groupView.Rows.Add(1);
                            newRow = groupView.Rows[3];
                            newRow.Cells["GroupInfoName"].Value = "Random";
                            newRow.Cells["GroupInfoValue"].Value = randomSeed;
                            newRow.ReadOnly = true; // Random���ܱ��༭

                            switch (currentNode.Parent.Text)
                            {
                                case "NPC":
                                    {
                                        ShowMemberInfo(npcInfoList, "ReliveID", groupID);
                                        break;
                                    }
                                case "Doodad":
                                    {
                                        ShowMemberInfo(doodadInfoList, "ReliveID", groupID);
                                        break;
                                    }
                                default:
                                    {
                                        break;
                                    }
                            }                            
                        }

                        break;
                    }
                case "�����":
                    {
                        currentNode = aiTree.SelectedNode;

                        if (currentNode != null)
                        {
                            infoTable = currentNode.Tag as Hashtable;
                            groupID = infoTable["dwSetID"] as string;
                            groupName = infoTable["szName"] as string;

                            string infoString = "";
                            baseDoc.DocLogical.GetAIGroupInfo(int.Parse(groupID), ref infoString);

                            string[] dataArray = infoString.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                            int currentIndex = 0;

                            foreach (string s in dataArray)
                            {
                                Hashtable npcInfoTable = GetNpcInfoTable(s);

                                if (npcInfoTable != null)
                                {
                                    string npcName = npcInfoTable["szName"] as string;
                                    string npcTemplateID = npcInfoTable["nTempleteID"] as string;
                                    string npcIndex = npcInfoTable["nIndex"] as string;
                                    
                                    npcView.Rows.Add(1);
                                    newRow = npcView.Rows[currentIndex];
                                    newRow.Cells["NpcName"].Value = npcName;
                                    newRow.Cells["NpcTemplateID"].Value = npcTemplateID;
                                    newRow.Cells["NpcIndex"].Value = npcIndex;
                                    currentIndex++;                                    
                                }
                            }

                            groupView.Rows.Add(1);
                            newRow = groupView.Rows[0];
                            newRow.Cells["GroupInfoName"].Value = "����";
                            newRow.Cells["GroupInfoValue"].Value = groupName;
                        }

                        break;
                    }
                case "�����":
                    {
                        currentNode = randomTree.SelectedNode;

                        if (currentNode != null)
                        {
                            infoTable = currentNode.Tag as Hashtable;
                            groupID = infoTable["dwGroupID"] as string;
                            groupName = infoTable["szName"] as string;

                            ShowMemberInfo(npcInfoList, "RandomID", groupID);

                            string infoString = "";
                            baseDoc.DocLogical.GetRandomGroupInfo(int.Parse(groupID), ref infoString);

                            groupView.Rows.Add(1);
                            newRow = groupView.Rows[0];
                            newRow.Cells["GroupInfoName"].Value = "����";
                            newRow.Cells["GroupInfoValue"].Value = groupName;

                            groupView.Rows.Add(1);
                            newRow = groupView.Rows[1];
                            newRow.Cells["GroupInfoName"].Value = "ģ��ID";
                            newRow.Cells["GroupInfoValue"].Value = infoString;
                        }                        

                        break;
                    }
                case "AI������":
                    {
                        currentNode = aiParameterTree.SelectedNode;

                        if (currentNode != null)
                        {
                            List<string[]> infoList = currentNode.Tag as List<string[]>;
                            int currentIndex = 0;

                            foreach (string[] infoArray in infoList)
                            {
                                string npcName = infoArray[0];
                                string npcTemplateID = infoArray[2];
                                string npcIndex = infoArray[1];

                                npcView.Rows.Add(1);
                                newRow = npcView.Rows[currentIndex];
                                newRow.Cells["NpcName"].Value = npcName;
                                newRow.Cells["NpcTemplateID"].Value = npcTemplateID;
                                newRow.Cells["NpcIndex"].Value = npcIndex;
                                currentIndex++;
                            }                            

                            int aiSetID = GetAIParameterSetID(currentNode.Text);

                            string infoString = "";
                            baseDoc.DocLogical.GetAIParameterSetInfo(aiSetID, ref infoString);
                            string[] tempArray = infoString.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);

                            foreach (string s in tempArray)
                            {
                                string[] dataArray = s.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);                                    

                                groupView.Rows.Add(1);
                                newRow = groupView.Rows[groupView.Rows.Count - 1];
                                newRow.Cells["GroupInfoName"].Value = dataArray[0];
                                newRow.Cells["GroupInfoValue"].Value = dataArray[1];
                                newRow.ReadOnly = true;
                            }                            
                        }

                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }

        /// <summary>
        ///  �л�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void tabControl1_SelectedTabChanged(object sender, DevComponents.DotNetBar.TabStripTabChangedEventArgs e)
        {
            groupView.Rows.Clear();
            npcView.Rows.Clear();
        }

        /// <summary>
        /// ɾ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDeleteGroup_Click(object sender, EventArgs e)
        {
            switch (tabControl1.SelectedTab.Text)
            {
                case "������":
                    {
                        DeleteReviveGroup();
                        break;
                    }
                case "�����":
                    {
                        DeleteHatredGroup();
                        break;
                    }
                case "�����":
                    {
                        DeleteRandomGroup();
                        break;
                    }
                case "AI������":
                    {                        
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }

        /// <summary>
        /// ���·�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bUpdateGroup_Click(object sender, EventArgs e)
        {
            bool updateSuccess = false;

            switch (tabControl1.SelectedTab.Text)
            {
                case "������": // ������
                    {                        
                        updateSuccess = UpdateReviveGroup();
                        break;
                    }
                case "�����": // �����
                    {
                        updateSuccess = UpdateHatredGroup();
                        break;
                    }
                case "�����": // �����
                    {
                        updateSuccess = UpdateRandomGroup();
                        break;
                    }
                case "AI������":
                    {
                        break;
                    }
                default:
                    {
                        break;
                    }
            }

            if (updateSuccess)
            {
                MessageBox.Show("�������ݸ��³ɹ���", "���·�������", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }
        }
	}
}