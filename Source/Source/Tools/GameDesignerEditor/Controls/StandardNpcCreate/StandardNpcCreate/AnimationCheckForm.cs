using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections;

namespace StandardNpcCreate
{
    public partial class AnimationCheckForm : Form
    {
        private DataTable boneToActionTable; // ���ݱ�
        private DataTable itemForInsertTable; // ���ݱ�
        private DataTable npcTable; // ���ݱ�
        private DataTable representTable; // ���ݱ�
        private DataTable npcAnimationTable; // ���ݱ�
        private string rootPath; // �ͻ���Ŀ¼
        private string folderName; // �ϼ��ļ���ȫĿ¼
        private string relevatePath; // ���·��
        private char[] commaCharArray = new char[] { ',' }; // ���ŷָ�������
        private char[] semicolonCharArray = new char[] { ';' }; // �ֺŷָ�������
        private char[] slashCharArray = new char[] { '\\' }; // б�ָܷ�������
        private Hashtable animationTable = new Hashtable(); // BoneNumber�Ͷ����Ķ�Ӧ��        
        private Hashtable commonStandbyAnimationTable = new Hashtable(); // BoneNumber����ͨ���������Ķ�Ӧ��
        private enum SlotType { LeftHand, RightHand }; // �������        
        private StringBuilder logText = new StringBuilder(); // ��־�ı�

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="rootPath">�ͻ���Ŀ¼</param>
        /// <param name="boneToActionTable">���ݱ�</param>
        /// <param name="itemForInsertTable">���ݱ�</param>
        /// <param name="npcTable">���ݱ�</param>
        /// <param name="representTable">���ݱ�</param>
        /// <param name="npcAnimationTable">���ݱ�</param>
        public AnimationCheckForm(string rootPath, DataTable boneToActionTable, DataTable itemForInsertTable, DataTable npcTable, DataTable representTable, DataTable npcAnimationTable)
        {
            this.rootPath = rootPath;
            this.boneToActionTable = boneToActionTable;
            this.itemForInsertTable = itemForInsertTable;
            this.npcTable = npcTable;
            this.representTable = representTable;
            this.npcAnimationTable = npcAnimationTable;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ���������
            DataGridViewTextBoxColumn boneNumberColumn = new DataGridViewTextBoxColumn();
            boneNumberColumn.HeaderText = "BoneNumber";
            boneNumberColumn.Name = "BoneNumber";

            DataGridViewTextBoxColumn detailTypeColumn = new DataGridViewTextBoxColumn();
            detailTypeColumn.HeaderText = "DetailType";
            detailTypeColumn.Name = "DetailType";

            DataGridViewTextBoxColumn animationTypeColumn = new DataGridViewTextBoxColumn();
            animationTypeColumn.HeaderText = "��������";
            animationTypeColumn.Name = "AnimationType";

            DataGridViewTextBoxColumn animationResourceLackNameColumn = new DataGridViewTextBoxColumn();
            animationResourceLackNameColumn.HeaderText = "��Դ����";
            animationResourceLackNameColumn.Name = "AnimationResourceLackName";

            DataGridViewTextBoxColumn animationResourceLackDescriptionColumn = new DataGridViewTextBoxColumn();
            animationResourceLackDescriptionColumn.HeaderText = "����";
            animationResourceLackDescriptionColumn.Name = "AnimationResourceLackDescription";

            animationResourceLackView.Columns.Add(boneNumberColumn);
            animationResourceLackView.Columns.Add(detailTypeColumn);
            animationResourceLackView.Columns.Add(animationTypeColumn);
            animationResourceLackView.Columns.Add(animationResourceLackNameColumn);
            animationResourceLackView.Columns.Add(animationResourceLackDescriptionColumn);

            DataGridViewTextBoxColumn boneNumberPlusColumn = new DataGridViewTextBoxColumn();
            boneNumberPlusColumn.HeaderText = "BoneNumber";
            boneNumberPlusColumn.Name = "BoneNumberPlus";

            DataGridViewTextBoxColumn animationTableLackNameColumn = new DataGridViewTextBoxColumn();
            animationTableLackNameColumn.HeaderText = "��Դ����";
            animationTableLackNameColumn.Name = "AnimationTableLackName";

            DataGridViewTextBoxColumn animationTableLackDescriptionColumn = new DataGridViewTextBoxColumn();
            animationTableLackDescriptionColumn.HeaderText = "����";
            animationTableLackDescriptionColumn.Name = "AnimationTableLackDescription";

            animationTableLackView.Columns.Add(boneNumberPlusColumn);
            animationTableLackView.Columns.Add(animationTableLackNameColumn);
            animationTableLackView.Columns.Add(animationTableLackDescriptionColumn);

            DataGridViewTextBoxColumn npcInfoColumn = new DataGridViewTextBoxColumn();
            npcInfoColumn.HeaderText = "NPC��Ϣ";
            npcInfoColumn.Name = "NpcInfo";

            DataGridViewTextBoxColumn slotTypeColumn = new DataGridViewTextBoxColumn();
            slotTypeColumn.HeaderText = "�������";
            slotTypeColumn.Name = "SlotType";

            DataGridViewTextBoxColumn itemResourceLackNameColumn = new DataGridViewTextBoxColumn();
            itemResourceLackNameColumn.HeaderText = "��Դ����";
            itemResourceLackNameColumn.Name = "ItemResourceLackName";

            DataGridViewTextBoxColumn itemResourceLackDescriptionColumn = new DataGridViewTextBoxColumn();
            itemResourceLackDescriptionColumn.HeaderText = "����";
            itemResourceLackDescriptionColumn.Name = "ItemResourceLackDescription";

            itemResourceLackView.Columns.Add(npcInfoColumn);
            itemResourceLackView.Columns.Add(slotTypeColumn);
            itemResourceLackView.Columns.Add(itemResourceLackNameColumn);
            itemResourceLackView.Columns.Add(itemResourceLackDescriptionColumn);

            DataGridViewTextBoxColumn fullPathColumn = new DataGridViewTextBoxColumn();
            fullPathColumn.HeaderText = "ȫ·��";
            fullPathColumn.Name = "FullPath";

            DataGridViewTextBoxColumn itemTableLackNameColumn = new DataGridViewTextBoxColumn();
            itemTableLackNameColumn.HeaderText = "��Դ����";
            itemTableLackNameColumn.Name = "ItemTableLackName";

            DataGridViewTextBoxColumn itemTableLackDescriptionColumn = new DataGridViewTextBoxColumn();
            itemTableLackDescriptionColumn.HeaderText = "����";
            itemTableLackDescriptionColumn.Name = "ItemTableLackDescription";

            itemTableLackView.Columns.Add(fullPathColumn);
            itemTableLackView.Columns.Add(itemTableLackNameColumn);
            itemTableLackView.Columns.Add(itemTableLackDescriptionColumn);

            DataGridViewTextBoxColumn npcInfoPlusColumn = new DataGridViewTextBoxColumn();
            npcInfoPlusColumn.HeaderText = "NPC��Ϣ";
            npcInfoPlusColumn.Name = "NpcInfoPlus";

            DataGridViewTextBoxColumn slotTypePlusColumn = new DataGridViewTextBoxColumn();
            slotTypePlusColumn.HeaderText = "�������";
            slotTypePlusColumn.Name = "SlotTypePlus";

            DataGridViewTextBoxColumn slotErrorNameColumn = new DataGridViewTextBoxColumn();
            slotErrorNameColumn.HeaderText = "��Դ����";
            slotErrorNameColumn.Name = "SlotErrorName";

            DataGridViewTextBoxColumn slotErrorDescriptionColumn = new DataGridViewTextBoxColumn();
            slotErrorDescriptionColumn.HeaderText = "����";
            slotErrorDescriptionColumn.Name = "SlotErrorDescription";

            slotErrorView.Columns.Add(npcInfoPlusColumn);
            slotErrorView.Columns.Add(slotTypePlusColumn);
            slotErrorView.Columns.Add(slotErrorNameColumn);
            slotErrorView.Columns.Add(slotErrorDescriptionColumn);

            DataGridViewTextBoxColumn npcInfoProColumn = new DataGridViewTextBoxColumn();
            npcInfoProColumn.HeaderText = "NPC��Ϣ";
            npcInfoProColumn.Name = "NpcInfoPro";

            DataGridViewTextBoxColumn slotTypeProColumn = new DataGridViewTextBoxColumn();
            slotTypeProColumn.HeaderText = "�������";
            slotTypeProColumn.Name = "SlotTypePro";

            DataGridViewTextBoxColumn npcTableLackNameColumn = new DataGridViewTextBoxColumn();
            npcTableLackNameColumn.HeaderText = "��Դ����";
            npcTableLackNameColumn.Name = "NpcTableLackName";

            DataGridViewTextBoxColumn npcLackDescriptionColumn = new DataGridViewTextBoxColumn();
            npcLackDescriptionColumn.HeaderText = "����";
            npcLackDescriptionColumn.Name = "NpcLackDescription";

            npcTableLackView.Columns.Add(npcInfoProColumn);
            npcTableLackView.Columns.Add(slotTypeProColumn);
            npcTableLackView.Columns.Add(npcTableLackNameColumn);
            npcTableLackView.Columns.Add(npcLackDescriptionColumn);

            DataGridViewTextBoxColumn npcInfoExColumn = new DataGridViewTextBoxColumn();
            npcInfoExColumn.HeaderText = "NPC��Ϣ";
            npcInfoExColumn.Name = "NpcInfoEx";

            DataGridViewTextBoxColumn representIDColumn = new DataGridViewTextBoxColumn();
            representIDColumn.HeaderText = "RepresentID";
            representIDColumn.Name = "RepresentID";

            DataGridViewTextBoxColumn commonStandbyAnimationNameColumn = new DataGridViewTextBoxColumn();
            commonStandbyAnimationNameColumn.HeaderText = "��ͨ��������";
            commonStandbyAnimationNameColumn.Name = "CommonStandbyAnimationName";
            
            DataGridViewTextBoxColumn commonStandbyAnimationErrorDescriptionColumn = new DataGridViewTextBoxColumn();
            commonStandbyAnimationErrorDescriptionColumn.HeaderText = "����";
            commonStandbyAnimationErrorDescriptionColumn.Name = "CommonStandbyAnimationErrorDesctiption";

            commonStandbyAnimationCheckView.Columns.Add(npcInfoExColumn);
            commonStandbyAnimationCheckView.Columns.Add(representIDColumn);
            commonStandbyAnimationCheckView.Columns.Add(commonStandbyAnimationNameColumn);
            commonStandbyAnimationCheckView.Columns.Add(commonStandbyAnimationErrorDescriptionColumn);

            DataGridViewTextBoxColumn npcInfoUltiColumn = new DataGridViewTextBoxColumn();
            npcInfoUltiColumn.HeaderText = "NPC��Ϣ";
            npcInfoUltiColumn.Name = "NpcInfoUlti";

            DataGridViewTextBoxColumn representIDProColumn = new DataGridViewTextBoxColumn();
            representIDProColumn.HeaderText = "RepresentID";
            representIDProColumn.Name = "RepresentIDPro";

            DataGridViewTextBoxColumn idleAniRatioFieldColumn = new DataGridViewTextBoxColumn();
            idleAniRatioFieldColumn.HeaderText = "�����������ż����ֶ�";
            idleAniRatioFieldColumn.Name = "IdleAniRatioFiled";

            DataGridViewTextBoxColumn descriptionUltiColumn = new DataGridViewTextBoxColumn();
            descriptionUltiColumn.HeaderText = "����";
            descriptionUltiColumn.Name = "DescriptionUlti";

            commonStandbyAnimationRateCheckView.Columns.Add(npcInfoUltiColumn);
            commonStandbyAnimationRateCheckView.Columns.Add(representIDProColumn);
            commonStandbyAnimationRateCheckView.Columns.Add(idleAniRatioFieldColumn);
            commonStandbyAnimationRateCheckView.Columns.Add(descriptionUltiColumn);

            DataGridViewTextBoxColumn checkTypeColumn = new DataGridViewTextBoxColumn();
            checkTypeColumn.HeaderText = "�������";
            checkTypeColumn.Name = "CheckType";

            DataGridViewTextBoxColumn errorCountColumn = new DataGridViewTextBoxColumn();
            errorCountColumn.HeaderText = "�������";
            errorCountColumn.Name = "ErrorCount";

            totalView.Columns.Add(checkTypeColumn);
            totalView.Columns.Add(errorCountColumn);

            // ���������
            CheckAnimationResourceLack();
            CheckAnimationTableLack();
            CheckItemResourceLack();
            CheckItemTableLack();
            CheckSlotError();
            CheckNpcTableLack();
            CheckCommonStandbyAnimationError();
            CheckCommonStandbyAnimationRateError();
            CountTotal();

            // �����־
            logTextBox.Text = logText.ToString();
        }

        /// <summary>
        /// �����ͨ�����������ż��ʴ���
        /// </summary>
        private void CheckCommonStandbyAnimationRateError()
        {
            foreach (DataRow dataRow in npcTable.Rows)
            {
                string npcID = dataRow["ID"].ToString();
                string npcName = dataRow["Name"].ToString();
                string representID = dataRow["RepresentID1"].ToString();

                if (representID != "" && representID != "0")
                {
                    DataRow[] rows = representTable.Select(string.Format("RepresentID = '{0}'", representID));

                    if (rows.Length > 0)
                    {
                        DataRow row = rows[0];
                        string[] fieldArray = { "IdleAniRatio0", "IdleAniRatio1", "IdleAniRatio2", "IdleAniRatio3", "IdleAniRatio4", "IdleAniRatio5" };
                        string[] animationIDArray = { "30", "31", "32", "33", "34", "35" };

                        for (int i = 0; i < 6; i++)
                        {
                            string rateString = row[fieldArray[i]].ToString();

                            if (rateString != "" && rateString != "0")
                            {
                                DataRow[] dataRows = npcAnimationTable.Select(string.Format("AnimationID = '{0}' AND RepresentID = '{1}'",
                                                                                            animationIDArray[i], representID));
                                if (dataRows.Length == 0)
                                {
                                    commonStandbyAnimationRateCheckView.Rows.Add(1);
                                    DataGridViewRow newRow = commonStandbyAnimationRateCheckView.Rows[commonStandbyAnimationRateCheckView.Rows.Count - 1];
                                    newRow.Cells["NpcInfoUlti"].Value = string.Format("{0} ({1})", npcName, npcID);
                                    newRow.Cells["RepresentIDPro"].Value = representID;
                                    newRow.Cells["IdleAniRatioFiled"].Value = fieldArray[i];
                                    newRow.Cells["DescriptionUlti"].Value = "�޶�Ӧ����";
                                }
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// �����ͨ������������
        /// </summary>
        private void CheckCommonStandbyAnimationError()
        {
            foreach (DataRow dataRow in npcTable.Rows)
            {
                string npcID = dataRow["ID"].ToString();
                string npcName = dataRow["Name"].ToString();
                string representID = dataRow["RepresentID1"].ToString();

                if (representID != "" && representID != "0")
                {
                    DataRow[] rows = representTable.Select(string.Format("RepresentID = '{0}'", representID));

                    if (rows.Length > 0)
                    {
                        string mainModelFile = rows[0]["MainModelFile"].ToString();

                        if (mainModelFile != "")
                        {
                            string boneNumber = GetModelType(mainModelFile);

                            if (boneNumber != null && !(boneNumber.StartsWith("F") || boneNumber.StartsWith("M"))) // ����ģ���ù�
                            {
                                List<string> commonStandbyAnimationList = commonStandbyAnimationTable[boneNumber] as List<string>;

                                if (commonStandbyAnimationList != null)
                                {
                                    string infoString = CheckCommonStandbyAnimationError(representID, commonStandbyAnimationList);

                                    if (infoString != null)
                                    {
                                        commonStandbyAnimationCheckView.Rows.Add(1);
                                        DataGridViewRow newRow = commonStandbyAnimationCheckView.Rows[commonStandbyAnimationCheckView.Rows.Count - 1];
                                        newRow.Cells["NpcInfoEx"].Value = string.Format("{0} ({1})", npcName, npcID);
                                        newRow.Cells["RepresentID"].Value = representID;
                                        newRow.Cells["CommonStandbyAnimationName"].Value = infoString;
                                        newRow.Cells["CommonStandbyAnimationErrorDesctiption"].Value = "��ƥ��";
                                    }
                                }                                
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// ���Mesh����Դȱʧ
        /// </summary>
        /// <param name="meshFile">�ļ�·��</param>
        /// <param name="npcID">NPC��ID</param>
        /// <param name="npcName">NPC������</param>
        private void CheckItemDataExist(string meshFile, string npcID, string npcName)
        {
            if (!string.IsNullOrEmpty(meshFile))
            {
                string itemNumber = GetItemNumber(meshFile);

                if (!CheckItemDataExist(itemNumber))
                {
                    npcTableLackView.Rows.Add(1);
                    DataGridViewRow newRow = npcTableLackView.Rows[npcTableLackView.Rows.Count - 1];
                    newRow.Cells["NpcInfoPro"].Value = string.Format("{0} ({1})", npcName, npcID);
                    newRow.Cells["SlotTypePro"].Value = itemNumber;
                    newRow.Cells["NpcTableLackName"].Value = meshFile;
                    newRow.Cells["NpcLackDescription"].Value = "ȱʧ";
                }
            }
        }

        /// <summary>
        /// ���npc����Դȱʧ
        /// </summary>
        private void CheckNpcTableLack()
        {
            foreach (DataRow dataRow in npcTable.Rows)
            {
                string npcName = dataRow["Name"].ToString();
                string npcID = dataRow["ID"].ToString();
                string representID = dataRow["RepresentID1"].ToString();

                if (!string.IsNullOrEmpty(representID))
                {
                    DataRow[] rows = representTable.Select(string.Format("RepresentID = '{0}'", representID));
                    if (rows.Length > 0)
                    {
                        DataRow row = rows[0];
                        string hatMeshFile = row["S_Hat_MeshFile"].ToString();
                        string lhMeshFile = row["S_LH_MeshFile"].ToString();
                        string lpMeshFile = row["S_LP_MeshFile"].ToString();
                        string rhMeshFile = row["S_RH_MeshFile"].ToString();
                        string rpMeshFile = row["S_RP_MeshFile"].ToString();
                        string spine2MeshFile = row["S_Spine2_MeshFile"].ToString();
                        string rcMeshFile = row["S_RC_MeshFile"].ToString();
                        string lcMeshFile = row["S_LC_MeshFile"].ToString();
                        string longMeshFile = row["S_Long_MeshFile"].ToString();
                        string spineMeshFile = row["S_Spine_MeshFile"].ToString();
                        string leftHandMeshFile = row["LeftHandMesh"].ToString();
                        string rightHandMeshFile = row["RightHandMesh"].ToString();
                        string itemNumber = null;

                        CheckItemDataExist(hatMeshFile);
                        CheckItemDataExist(lhMeshFile);
                        CheckItemDataExist(lpMeshFile);
                        CheckItemDataExist(rhMeshFile);
                        CheckItemDataExist(rpMeshFile);
                        CheckItemDataExist(spine2MeshFile);
                        CheckItemDataExist(rcMeshFile);
                        CheckItemDataExist(lcMeshFile);
                        CheckItemDataExist(longMeshFile);
                        CheckItemDataExist(spineMeshFile);
                        CheckItemDataExist(leftHandMeshFile);
                        CheckItemDataExist(rightHandMeshFile);
                    }
                }                
            }
        }

        /// <summary>
        /// ��鶯�������Ƿ����
        /// </summary>
        /// <param name="fieldString">�ֶ���</param>
        /// <param name="dataRow">������</param>
        /// <param name="animationList">��������</param>
        /// <param name="commonStandbyAnimationList">������������</param>
        /// <param name="isStandbyAnimation">�Ƿ��Ǵ�������</param>
        private void CheckAnimationFileExist(string fieldString, DataRow dataRow, List<string> animationList, List<string> commonStandbyAnimationList, bool isStandbyAnimation)
        {
            string animationString = dataRow[fieldString] as string;

            if (!string.IsNullOrEmpty(animationString))
            {
                string[] tempArray = animationString.Split(semicolonCharArray, StringSplitOptions.RemoveEmptyEntries);

                foreach (string s in tempArray)
                {
                    if (!CheckAnimationFileExist(s))
                    {
                        animationResourceLackView.Rows.Add(1);
                        DataGridViewRow newRow = animationResourceLackView.Rows[animationResourceLackView.Rows.Count - 1];
                        newRow.Cells["BoneNumber"].Value = dataRow["BoneNumber"] as string;
                        newRow.Cells["DetailType"].Value = dataRow["DetailType"] as string;
                        newRow.Cells["AnimationType"].Value = "CommonStandbySeries1";
                        newRow.Cells["AnimationResourceLackName"].Value = s;
                        newRow.Cells["AnimationResourceLackDescription"].Value = "ȱʧ";
                    }

                    if (!animationList.Contains(s))
                    {
                        animationList.Add(s);
                    }
                }

                if (isStandbyAnimation && !commonStandbyAnimationList.Contains(animationString))
                {
                    commonStandbyAnimationList.Add(animationString);
                }
            }
        }

        /// <summary>
        /// ��鶯����Դȱʧ
        /// </summary>
        private void CheckAnimationResourceLack()
        {
            string[] tempArray;
            string[] pairArray;

            foreach (DataRow dataRow in boneToActionTable.Rows)
            {
                string boneNumber = dataRow["BoneNumber"] as string;

                List<string> animationList = animationTable[boneNumber] as List<string>;
                if (animationList == null)
                {
                    animationList = new List<string>();
                    animationTable[boneNumber] = animationList;
                }

                List<string> commonStandbyAnimationList = commonStandbyAnimationTable[boneNumber] as List<string>;
                if (commonStandbyAnimationList == null)
                {
                    commonStandbyAnimationList = new List<string>();
                    commonStandbyAnimationTable[boneNumber] = commonStandbyAnimationList;
                }

                InitRelevatePath(boneNumber);

                CheckAnimationFileExist("CommonStandbySeries1", dataRow, animationList, commonStandbyAnimationList, true);
                CheckAnimationFileExist("CommonStandbySeries2", dataRow, animationList, commonStandbyAnimationList, true);
                CheckAnimationFileExist("CommonStandbySeries3", dataRow, animationList, commonStandbyAnimationList, true);
                CheckAnimationFileExist("CommonStandbySeries4", dataRow, animationList, commonStandbyAnimationList, true);
                CheckAnimationFileExist("CommonStandbySeries5", dataRow, animationList, commonStandbyAnimationList, true);
                CheckAnimationFileExist("CommonStandbySeries6", dataRow, animationList, commonStandbyAnimationList, true);
                CheckAnimationFileExist("Walk", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("Run", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("BattleStandby", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("BattleUnderAttack", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("AttackSeries", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("AttackBuff", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("AttackAoe", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("AttackBang", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("AttackLongRange", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("Death", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("OnGround", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("OnGroundUnderAttack", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("Beatback", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("Glissade", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("Volt", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("Vertigo", dataRow, animationList, commonStandbyAnimationList, false);
                CheckAnimationFileExist("Wardoff", dataRow, animationList, commonStandbyAnimationList, false);

                // ϵ�����׶������鷳����Ҫ���������ٽ�����Ч�Լ��
                string knockDownStandUp = dataRow["Knockdown_Standup"] as string;

                if (!string.IsNullOrEmpty(knockDownStandUp))
                {
                    tempArray = knockDownStandUp.Split(semicolonCharArray, StringSplitOptions.RemoveEmptyEntries);

                    foreach (string s in tempArray)
                    {
                        pairArray = s.Split(commaCharArray, StringSplitOptions.RemoveEmptyEntries);

                        if (pairArray.Length == 2)
                        {
                            if (!CheckAnimationFileExist(pairArray[0]))
                            {
                                animationResourceLackView.Rows.Add(1);
                                DataGridViewRow newRow = animationResourceLackView.Rows[animationResourceLackView.Rows.Count - 1];
                                newRow.Cells["BoneNumber"].Value = dataRow["BoneNumber"] as string;
                                newRow.Cells["DetailType"].Value = dataRow["DetailType"] as string;
                                newRow.Cells["AnimationType"].Value = "Knockdown_Standup";
                                newRow.Cells["AnimationResourceLackName"].Value = pairArray[0];
                                newRow.Cells["AnimationResourceLackDescription"].Value = "ȱʧ";
                            }

                            if (!CheckAnimationFileExist(pairArray[1]))
                            {
                                animationResourceLackView.Rows.Add(1);
                                DataGridViewRow newRow = animationResourceLackView.Rows[animationResourceLackView.Rows.Count - 1];
                                newRow.Cells["BoneNumber"].Value = dataRow["BoneNumber"] as string;
                                newRow.Cells["DetailType"].Value = dataRow["DetailType"] as string;
                                newRow.Cells["AnimationType"].Value = "Knockdown_Standup";
                                newRow.Cells["AnimationResourceLackName"].Value = pairArray[1];
                                newRow.Cells["AnimationResourceLackDescription"].Value = "ȱʧ";
                            }

                            if (!animationList.Contains(pairArray[0]))
                            {
                                animationList.Add(pairArray[0]);
                            }

                            if (!animationList.Contains(pairArray[1]))
                            {
                                animationList.Add(pairArray[1]);
                            }
                        }
                        else
                        {
                            logText.Append(string.Format("���ݸ�ʽ����{0}\r\n", knockDownStandUp));
                        }
                    }
                }

                string intonateRelease = dataRow["Intonate_Release"] as string;

                if (!string.IsNullOrEmpty(intonateRelease))
                {
                    tempArray = intonateRelease.Split(semicolonCharArray, StringSplitOptions.RemoveEmptyEntries);

                    foreach (string s in tempArray)
                    {
                        pairArray = s.Split(commaCharArray, StringSplitOptions.RemoveEmptyEntries);

                        if (pairArray.Length == 2)
                        {
                            if (!CheckAnimationFileExist(pairArray[0]))
                            {
                                animationResourceLackView.Rows.Add(1);
                                DataGridViewRow newRow = animationResourceLackView.Rows[animationResourceLackView.Rows.Count - 1];
                                newRow.Cells["BoneNumber"].Value = dataRow["BoneNumber"] as string;
                                newRow.Cells["DetailType"].Value = dataRow["DetailType"] as string;
                                newRow.Cells["AnimationType"].Value = "Intonate_Release";
                                newRow.Cells["AnimationResourceLackName"].Value = pairArray[0];
                                newRow.Cells["AnimationResourceLackDescription"].Value = "ȱʧ";
                            }

                            if (!CheckAnimationFileExist(pairArray[1]))
                            {
                                animationResourceLackView.Rows.Add(1);
                                DataGridViewRow newRow = animationResourceLackView.Rows[animationResourceLackView.Rows.Count - 1];
                                newRow.Cells["BoneNumber"].Value = dataRow["BoneNumber"] as string;
                                newRow.Cells["DetailType"].Value = dataRow["DetailType"] as string;
                                newRow.Cells["AnimationType"].Value = "Intonate_Release";
                                newRow.Cells["AnimationResourceLackName"].Value = pairArray[1];
                                newRow.Cells["AnimationResourceLackDescription"].Value = "ȱʧ";
                            }

                            if (!animationList.Contains(pairArray[0]))
                            {
                                animationList.Add(pairArray[0]);
                            }

                            if (!animationList.Contains(pairArray[1]))
                            {
                                animationList.Add(pairArray[1]);
                            }
                        }
                        else
                        {
                            logText.Append(string.Format("���ݸ�ʽ����{0}\r\n", intonateRelease));
                        }
                    }
                }

                string knockdownOnGroundOnGroundUnderAttackStandupDeath = dataRow["Knockdown_OnGround_OnGroundUnderAttack_Standup_Death_Time"] as string;

                if (!string.IsNullOrEmpty(knockdownOnGroundOnGroundUnderAttackStandupDeath))
                {
                    tempArray = knockdownOnGroundOnGroundUnderAttackStandupDeath.Split(semicolonCharArray, StringSplitOptions.RemoveEmptyEntries);

                    foreach (string s in tempArray)
                    {
                        pairArray = s.Split(commaCharArray, StringSplitOptions.RemoveEmptyEntries);

                        if (pairArray.Length == 6)
                        {
                            if (!CheckAnimationFileExist(pairArray[0]))
                            {
                                animationResourceLackView.Rows.Add(1);
                                DataGridViewRow newRow = animationResourceLackView.Rows[animationResourceLackView.Rows.Count - 1];
                                newRow.Cells["BoneNumber"].Value = dataRow["BoneNumber"] as string;
                                newRow.Cells["DetailType"].Value = dataRow["DetailType"] as string;
                                newRow.Cells["AnimationType"].Value = "Knockdown_OnGround_OnGroundUnderAttack_Standup_Death_Time";
                                newRow.Cells["AnimationResourceLackName"].Value = pairArray[0];
                                newRow.Cells["AnimationResourceLackDescription"].Value = "ȱʧ";
                            }

                            if (!CheckAnimationFileExist(pairArray[1]))
                            {
                                animationResourceLackView.Rows.Add(1);
                                DataGridViewRow newRow = animationResourceLackView.Rows[animationResourceLackView.Rows.Count - 1];
                                newRow.Cells["BoneNumber"].Value = dataRow["BoneNumber"] as string;
                                newRow.Cells["DetailType"].Value = dataRow["DetailType"] as string;
                                newRow.Cells["AnimationType"].Value = "Knockdown_OnGround_OnGroundUnderAttack_Standup_Death_Time";
                                newRow.Cells["AnimationResourceLackName"].Value = pairArray[1];
                                newRow.Cells["AnimationResourceLackDescription"].Value = "ȱʧ";
                            }

                            if (!CheckAnimationFileExist(pairArray[2]))
                            {
                                animationResourceLackView.Rows.Add(1);
                                DataGridViewRow newRow = animationResourceLackView.Rows[animationResourceLackView.Rows.Count - 1];
                                newRow.Cells["BoneNumber"].Value = dataRow["BoneNumber"] as string;
                                newRow.Cells["DetailType"].Value = dataRow["DetailType"] as string;
                                newRow.Cells["AnimationType"].Value = "Knockdown_OnGround_OnGroundUnderAttack_Standup_Death_Time";
                                newRow.Cells["AnimationResourceLackName"].Value = pairArray[2];
                                newRow.Cells["AnimationResourceLackDescription"].Value = "ȱʧ";
                            }

                            if (!CheckAnimationFileExist(pairArray[3]))
                            {
                                animationResourceLackView.Rows.Add(1);
                                DataGridViewRow newRow = animationResourceLackView.Rows[animationResourceLackView.Rows.Count - 1];
                                newRow.Cells["BoneNumber"].Value = dataRow["BoneNumber"] as string;
                                newRow.Cells["DetailType"].Value = dataRow["DetailType"] as string;
                                newRow.Cells["AnimationType"].Value = "Knockdown_OnGround_OnGroundUnderAttack_Standup_Death_Time";
                                newRow.Cells["AnimationResourceLackName"].Value = pairArray[3];
                                newRow.Cells["AnimationResourceLackDescription"].Value = "ȱʧ";
                            }

                            if (!CheckAnimationFileExist(pairArray[4]))
                            {
                                animationResourceLackView.Rows.Add(1);
                                DataGridViewRow newRow = animationResourceLackView.Rows[animationResourceLackView.Rows.Count - 1];
                                newRow.Cells["BoneNumber"].Value = dataRow["BoneNumber"] as string;
                                newRow.Cells["DetailType"].Value = dataRow["DetailType"] as string;
                                newRow.Cells["AnimationType"].Value = "Knockdown_OnGround_OnGroundUnderAttack_Standup_Death_Time";
                                newRow.Cells["AnimationResourceLackName"].Value = pairArray[4];
                                newRow.Cells["AnimationResourceLackDescription"].Value = "ȱʧ";
                            }

                            if (!animationList.Contains(pairArray[0]))
                            {
                                animationList.Add(pairArray[0]);
                            }

                            if (!animationList.Contains(pairArray[1]))
                            {
                                animationList.Add(pairArray[1]);
                            }

                            if (!animationList.Contains(pairArray[2]))
                            {
                                animationList.Add(pairArray[2]);
                            }

                            if (!animationList.Contains(pairArray[3]))
                            {
                                animationList.Add(pairArray[3]);
                            }

                            if (!animationList.Contains(pairArray[4]))
                            {
                                animationList.Add(pairArray[4]);
                            }
                        }
                        else
                        {
                            logText.Append(string.Format("���ݸ�ʽ����{0}\r\n", knockdownOnGroundOnGroundUnderAttackStandupDeath));
                        }
                    }
                }
            }
        }

        /// <summary>
        /// ��鶯������ȱʧ
        /// </summary>
        private void CheckAnimationTableLack()
        {            
            foreach(object o in animationTable.Keys)
            {   
                string boneNumber = o as string;

                if (!boneNumber.StartsWith("F") && !boneNumber.StartsWith("M")) // ��������Ƕ���
                {
                    InitRelevatePath(boneNumber);
                    List<string> animationList = animationTable[o] as List<string>;

                    if (animationList != null)
                    {
                        List<string> lackAnimationList = CheckAnimationDataExist(animationList);

                        foreach (string s in lackAnimationList)
                        {
                            animationTableLackView.Rows.Add(1);
                            DataGridViewRow newRow = animationTableLackView.Rows[animationTableLackView.Rows.Count - 1];
                            newRow.Cells["BoneNumberPlus"].Value = boneNumber;
                            newRow.Cells["AnimationTableLackName"].Value = s;
                            newRow.Cells["AnimationTableLackDescription"].Value = "ȱʧ";
                        }
                    }
                }                                
            }
        }

        /// <summary>
        /// ��������Դȱʧ
        /// </summary>
        private void CheckItemResourceLack()
        {
            DataGridViewRow newRow;

            foreach(DataRow dataRow in npcTable.Rows)
            {
                string npcName = dataRow["Name"].ToString();
                string npcID = dataRow["ID"].ToString();
                string representID = dataRow["RepresentID1"].ToString();
                
                if (!string.IsNullOrEmpty(representID))
                {
                    DataRow[] rows = representTable.Select(string.Format("RepresentID = '{0}'", representID));
                    if (rows.Length > 0)
                    {
                        DataRow row = rows[0];
                        string mainModelFile = row["MainModelFile"].ToString();
                        string leftHandMeshFile = row["S_LH_MeshFile"].ToString();
                        string rightHandMeshFile = row["S_RH_MeshFile"].ToString();

                        if (!string.IsNullOrEmpty(mainModelFile) && !CheckItemFileExist(mainModelFile))
                        {
                            itemResourceLackView.Rows.Add(1);
                            newRow = itemResourceLackView.Rows[itemResourceLackView.Rows.Count - 1];
                            newRow.Cells["NpcInfo"].Value = string.Format("{0} ({1})", npcName, npcID);
                            newRow.Cells["SlotType"].Value = "MDL�ļ�";
                            newRow.Cells["ItemResourceLackName"].Value = mainModelFile;
                            newRow.Cells["ItemResourceLackDescription"].Value = "ȱʧ";
                        }

                        if (!string.IsNullOrEmpty(leftHandMeshFile) && !CheckItemFileExist(leftHandMeshFile))
                        {
                            itemResourceLackView.Rows.Add(1);
                            newRow = itemResourceLackView.Rows[itemResourceLackView.Rows.Count - 1];
                            newRow.Cells["NpcInfo"].Value = string.Format("{0} ({1})", npcName, npcID);
                            newRow.Cells["SlotType"].Value = "���ֲ��";
                            newRow.Cells["ItemResourceLackName"].Value = leftHandMeshFile;
                            newRow.Cells["ItemResourceLackDescription"].Value = "ȱʧ";
                        }

                        if (!string.IsNullOrEmpty(rightHandMeshFile) && !CheckItemFileExist(rightHandMeshFile))
                        {
                            itemResourceLackView.Rows.Add(1);
                            newRow = itemResourceLackView.Rows[itemResourceLackView.Rows.Count - 1];
                            newRow.Cells["NpcInfo"].Value = string.Format("{0} ({1})", npcName, npcID);
                            newRow.Cells["SlotType"].Value = "���ֲ��";
                            newRow.Cells["ItemResourceLackName"].Value = rightHandMeshFile;
                            newRow.Cells["ItemResourceLackDescription"].Value = "ȱʧ";
                        }
                    }
                }                                               
            }
        }

        /// <summary>
        /// ���������ݱ���Դ
        /// </summary>
        private void CheckItemTableLack()
        {
            string fullPath = Path.Combine(rootPath, @"data\source\item");
            DirectoryInfo di = new DirectoryInfo(fullPath);
            List<string> fullPathList = new List<string>();
            List<string> itemNumberList = new List<string>();

            if (di.Exists)
            {
                CheckItemDataExist(di, fullPathList, itemNumberList);

                for (int i = 0; i < itemNumberList.Count; i++)
                {
                    itemTableLackView.Rows.Add(1);
                    DataGridViewRow newRow = itemTableLackView.Rows[itemTableLackView.Rows.Count - 1];
                    newRow.Cells["FullPath"].Value = fullPathList[i];
                    newRow.Cells["ItemTableLackName"].Value = itemNumberList[i];
                    newRow.Cells["ItemTableLackDescription"].Value = "ȱʧ";
                }
            }            
        }

        /// <summary>
        /// ����۴���
        /// </summary>
        private void CheckSlotError()
        {
            foreach(DataRow dataRow in npcTable.Rows)
            {
                string npcName = dataRow["Name"].ToString();
                string npcID = dataRow["ID"].ToString();
                string representID = dataRow["RepresentID1"].ToString();
                DataGridViewRow newRow;

                if (!string.IsNullOrEmpty(representID))
                {
                    DataRow[] rows = representTable.Select(string.Format("RepresentID = '{0}'", representID));
                    if (rows.Length > 0)
                    {
                        DataRow row = rows[0];
                        string leftHandMeshFile = row["S_LH_MeshFile"].ToString();
                        string rightHandMeshFile = row["S_RH_MeshFile"].ToString();

                        if (CheckSlotError(leftHandMeshFile, SlotType.LeftHand))
                        {
                            slotErrorView.Rows.Add(1);
                            newRow = slotErrorView.Rows[slotErrorView.Rows.Count - 1];
                            newRow.Cells["NpcInfoPlus"].Value = string.Format("{0} ({1})", npcName, npcID);
                            newRow.Cells["SlotTypePlus"].Value = "����";
                            newRow.Cells["SlotErrorName"].Value = leftHandMeshFile;
                            newRow.Cells["SlotErrorDescription"].Value = "��ƥ��";
                        }

                        if (CheckSlotError(rightHandMeshFile, SlotType.RightHand))
                        {
                            slotErrorView.Rows.Add(1);
                            newRow = slotErrorView.Rows[slotErrorView.Rows.Count - 1];
                            newRow.Cells["NpcInfoPlus"].Value = string.Format("{0} ({1})", npcName, npcID);
                            newRow.Cells["SlotTypePlus"].Value = "����";
                            newRow.Cells["SlotErrorName"].Value = rightHandMeshFile;
                            newRow.Cells["SlotErrorDescription"].Value = "��ƥ��";
                        }
                    }
                }                
            }
        }

        /// <summary>
        /// �����ܼ�
        /// </summary>
        private void CountTotal()
        {
            DataGridViewRow newRow;

            totalView.Rows.Add(1);
            newRow = totalView.Rows[0];
            newRow.Cells["CheckType"].Value = "������Դȱʧ��ȫ����";
            newRow.Cells["ErrorCount"].Value = animationResourceLackView.Rows.Count.ToString();

            totalView.Rows.Add(1);
            newRow = totalView.Rows[1];
            newRow.Cells["CheckType"].Value = "������ȱʧ��ȫ����";
            newRow.Cells["ErrorCount"].Value = animationTableLackView.Rows.Count.ToString();

            totalView.Rows.Add(1);
            newRow = totalView.Rows[2];
            newRow.Cells["CheckType"].Value = "������Դȱʧ����ǰ���ࣩ";
            newRow.Cells["ErrorCount"].Value = itemResourceLackView.Rows.Count.ToString();

            totalView.Rows.Add(1);
            newRow = totalView.Rows[3];
            newRow.Cells["CheckType"].Value = "���߱�ȱʧ��ȫ����";
            newRow.Cells["ErrorCount"].Value = itemTableLackView.Rows.Count.ToString();

            totalView.Rows.Add(1);
            newRow = totalView.Rows[4];
            newRow.Cells["CheckType"].Value = "��۴��󣨵�ǰ���ࣩ";
            newRow.Cells["ErrorCount"].Value = slotErrorView.Rows.Count.ToString();

            totalView.Rows.Add(1);
            newRow = totalView.Rows[5];
            newRow.Cells["CheckType"].Value = "NPC��ȱʧ����ǰ���ࣩ";
            newRow.Cells["ErrorCount"].Value = npcTableLackView.Rows.Count.ToString();

            totalView.Rows.Add(1);
            newRow = totalView.Rows[6];
            newRow.Cells["CheckType"].Value = "��ͨ�����������в�ƥ�䣨��ǰ���ࣩ";
            newRow.Cells["ErrorCount"].Value = commonStandbyAnimationCheckView.Rows.Count.ToString();

            totalView.Rows.Add(1);
            newRow = totalView.Rows[7];
            newRow.Cells["CheckType"].Value = "��ͨ�����������ż��ʲ�ƥ��";
            newRow.Cells["ErrorCount"].Value = commonStandbyAnimationRateCheckView.Rows.Count.ToString();
        }

        /// <summary>
        /// �����ͨ������������
        /// </summary>
        /// <param name="representID">����ID</param>
        /// <param name="commonStandbyAnimationList">��ͨ������������</param>
        /// <returns>������Ϣ</returns>
        private string CheckCommonStandbyAnimationError(string representID, List<string> commonStandbyAnimationList)
        {
            string infoString = null;

            string[] animationIDArray = new string[] { "30", "31", "32", "33", "34", "35" };
            StringBuilder commonStandbyAnimationString = new StringBuilder();
            string tempString = null;
            string[] tempArray = null;
            
            for (int i = 0; i < 6; i++)
            {
                DataRow[] rows = npcAnimationTable.Select(string.Format("RepresentID = '{0}' AND AnimationID = '{1}'", representID, animationIDArray[i]));
                if (rows.Length > 0)
                {
                    tempString = rows[0]["AnimationFile"].ToString();
                    if (tempString != "")
                    {
                        tempArray = tempString.Split(slashCharArray, StringSplitOptions.RemoveEmptyEntries);

                        if (tempArray.Length > 0)
                        {
                            tempString = tempArray[tempArray.Length - 1].Replace(".tani", "").Replace(".ani", "");
                        }
                    }

                    commonStandbyAnimationString.Append(string.Format("{0};", tempString));
                }
            }

            if (commonStandbyAnimationString.Length > 0)
            {
                commonStandbyAnimationString.Remove(commonStandbyAnimationString.Length - 1, 1);
            }

            infoString = commonStandbyAnimationString.ToString().ToLower();

            foreach (string s in commonStandbyAnimationList)
            {
                if (s.ToLower().Contains(infoString))
                {
                    infoString = null;
                    break;
                }
            }

            return infoString;
        }

        /// <summary>
        /// ����۴���
        /// </summary>
        /// <param name="meshFile">�ļ�·��</param>
        /// <param name="slotType">�������</param>
        /// <returns>�Ƿ��д�</returns>
        private bool CheckSlotError(string meshFile, SlotType slotType)
        {
            bool slotError = false;
            string[] sections = meshFile.Split(new char[] { '\\' });
            string itemNumber = sections[sections.Length - 1].Replace(".Mesh", "").Replace(".mesh", "");
            DataRow[] rows = itemForInsertTable.Select(string.Format("ItemNumber = '{0}'", itemNumber));

            if (rows.Length > 0)
            {
                DataRow row = rows[0];
                string leftHandPriorty = row["S_LH_MeshFile"] as string;
                string rightHandPriorty = row["S_RH_MeshFile"] as string;

                switch (slotType)
                {
                    case SlotType.LeftHand:
                        {
                            if (string.IsNullOrEmpty(leftHandPriorty) || leftHandPriorty == "0")
                            {
                                slotError = true;
                            }

                            break;
                        }
                    case SlotType.RightHand:
                        {
                            if (string.IsNullOrEmpty(rightHandPriorty) || rightHandPriorty == "0")
                            {
                                slotError = true;
                            }

                            break;
                        }
                }
            }            

            return slotError;
        }

        /// <summary>
        /// ��ȡģ���ļ�������
        /// </summary>
        /// <param name="modelPath">ģ���ļ���·��</param>
        /// <returns>ģ���ļ�������</returns>
        private string GetModelType(string modelPath)
        {
            string modelType = null;

            string[] data = modelPath.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
            if (data.Length > 4)
            {
                string modelClass = data[2];

                switch (modelClass)
                {
                    case "player":
                        {
                            modelType = data[3];

                            break;
                        }
                    case "NPC_source":
                        {
                            modelType = data[3];

                            break;
                        }
                }
            }

            return modelType;
        }

        /// <summary>
        /// ��ȡItemForInsert.tab��Keyֵ
        /// </summary>
        /// <param name="meshFileName">mesh·��</param>
        /// <returns>ItemForInsert.tab��Keyֵ</returns>
        private string GetItemNumber(string meshFileName)
        {
            string[] tempArray = meshFileName.Split(slashCharArray, StringSplitOptions.RemoveEmptyEntries);
            string itemNumber = tempArray[tempArray.Length - 1].Replace(".Mesh", "").Replace(".mesh", "");

            return itemNumber;
        }

        /// <summary>
        /// �����߱������Ƿ����
        /// </summary>
        /// <param name="itemNumber">ItemForInsert.tab��Keyֵ</param>
        /// <returns>���߱������Ƿ����</returns>
        private bool CheckItemDataExist(string itemNumber)
        {
            bool exist = false;

            DataRow[] rows = itemForInsertTable.Select(string.Format("ItemNumber = '{0}'", itemNumber));

            if (rows.Length > 0)
            {
                exist = true;
            }

            return exist;
        }

        /// <summary>
        /// �����߱������Ƿ����
        /// </summary>
        /// <param name="directoryInfo">��ǰ�ļ���</param>
        /// <param name="fullPathList">·������</param>
        /// <param name="itemNumberList">ItemNumber����</param>
        private void CheckItemDataExist(DirectoryInfo directoryInfo, List<string> fullPathList, List<string> itemNumberList)
        {
            FileInfo[] files = directoryInfo.GetFiles();
            DirectoryInfo[] directories = directoryInfo.GetDirectories();

            foreach(DirectoryInfo di in directories)
            {
                CheckItemDataExist(di, fullPathList, itemNumberList);
            }

            foreach(FileInfo fi in files)
            {
                if (fi.Extension == ".Mesh" || fi.Extension == ".mesh")
                {
                    string fileName = fi.Name.Replace(".Mesh", "").Replace(".mesh", "");
                    DataRow[] rows = itemForInsertTable.Select(string.Format("ItemNumber = '{0}'", fileName));

                    if (rows.Length == 0)
                    {
                        if (!itemNumberList.Contains(fileName))
                        {
                            fullPathList.Add(fi.FullName.Replace(rootPath, ""));
                            itemNumberList.Add(fileName);
                        }                        
                    }
                }
            }
        }

        /// <summary>
        /// �������ļ��Ƿ����
        /// </summary>
        /// <param name="itemFile">�����ļ�·��</param>
        /// <returns>�Ƿ����</returns>
        private bool CheckItemFileExist(string itemFile)
        {
            bool exist = false;
            string fullPath = Path.Combine(rootPath, itemFile);

            if (File.Exists(fullPath))
            {
                exist = true;
            }

            return exist;
        }

        /// <summary>
        /// ��鶯�����������Ƿ����
        /// </summary>
        /// <param name="animationList">��������</param>
        /// <returns>ȱʧ������������</returns>
        private List<string> CheckAnimationDataExist(List<string> animationList)
        {
            List<string> lackAnimationList = new List<string>();
            DirectoryInfo di = new DirectoryInfo(folderName);

            if (di.Exists)
            {
                FileInfo[] files = di.GetFiles();

                foreach(FileInfo fi in files)
                {
                    bool exist = true;
                    string fileName = fi.Name;

                    if (!animationList.Contains(fileName))
                    {
                        exist = false;
                    }

                    if (!exist) // ȥ����׺������һ�ο���
                    {
                        fileName = fileName.Replace(".ani", "").Replace(".tani", "");
                        if (animationList.Contains(fileName))
                        {
                            exist = true;
                        }
                    }

                    if (!exist)
                    {
                        if (!lackAnimationList.Contains(fileName))
                        {
                            lackAnimationList.Add(fileName);
                        }                        
                    }
                }
            }

            return lackAnimationList;
        }

        /// <summary>
        /// ��鶯����Դ�Ƿ����
        /// </summary>
        /// <param name="animationString">������Դ��</param>
        /// <returns>�Ƿ����</returns>
        private bool CheckAnimationFileExist(string animationString)
        {
            bool exist = false;
            string fullPath;

            if (!animationString.EndsWith(".ani") && !animationString.EndsWith(".tani"))
            {
                fullPath = Path.Combine(folderName, string.Format("{0}.ani", animationString));

                if (File.Exists(fullPath))
                {
                    exist = true;
                }

                if (!exist)
                {
                    fullPath = Path.Combine(folderName, string.Format("{0}.tani", animationString));

                    if (File.Exists(fullPath))
                    {
                        exist = true;
                    }
                }
            }
            else
            {
                fullPath = Path.Combine(folderName, animationString);

                if (File.Exists(fullPath))
                {
                    exist = true;
                }
            }

            return exist;
        }

        /// <summary>
        /// ��ʼ�����·��
        /// </summary>
        /// <param name="boneNumber">��������</param>
        private void InitRelevatePath(string boneNumber)
        {
            string firstLetter = boneNumber.Substring(0, 1);

            switch (firstLetter)
            {
                case "F": // player����
                    {
                        relevatePath = string.Format(@"data\source\player\{0}\����\", boneNumber);

                        break;
                    }
                case "M": // player����
                    {
                        relevatePath = string.Format(@"data\source\player\{0}\����\", boneNumber);

                        break;
                    }
                default: // npc����
                    {
                        relevatePath = string.Format(@"data\source\NPC_source\{0}\����\", boneNumber);

                        break;
                    }
            }

            folderName = Path.Combine(rootPath, relevatePath);
        }

        /// <summary>
        /// ������־
        /// </summary>
        private void ExportLog()
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Title = "������־�ļ�";
            saveFileDialog.FileName = "NPC���������־";
            saveFileDialog.DefaultExt = ".txt";
            saveFileDialog.Filter = "�ı��ļ�|*.txt";
            
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                string fileName = saveFileDialog.FileName;
                StringBuilder logText = new StringBuilder();

                logText.AppendLine("������Դȱʧ��ȫ������");
                foreach (DataGridViewRow row in animationResourceLackView.Rows)
                {
                    logText.AppendLine(string.Format("{0}\t{1}\t{2}\t{3}", row.Cells["BoneNumber"].Value, row.Cells["DetailType"].Value, row.Cells["AnimationResourceLackName"].Value,
                                   row.Cells["AnimationResourceLackDescription"].Value));
                }
                logText.AppendLine();

                logText.AppendLine("������ȱʧ��ȫ������");
                foreach (DataGridViewRow row in animationTableLackView.Rows)
                {
                    logText.AppendLine(string.Format("{0}\t\t{1}\t{2}", row.Cells["BoneNumberPlus"].Value, row.Cells["AnimationTableLackName"].Value,
                                   row.Cells["AnimationTableLackDescription"].Value));
                }
                logText.AppendLine();

                logText.AppendLine("������Դȱʧ����ǰ���ࣩ��");
                foreach (DataGridViewRow row in itemResourceLackView.Rows)
                {
                    logText.AppendLine(string.Format("{0}\t{1}\t{2}\t{3}", row.Cells["NpcInfo"].Value, row.Cells["SlotType"].Value,
                                                 row.Cells["ItemResourceLackName"].Value, row.Cells["ItemResourceLackDescription"].Value));
                }
                logText.AppendLine();

                logText.AppendLine("���߱�ȱʧ��ȫ������");
                foreach (DataGridViewRow row in itemTableLackView.Rows)
                {
                    logText.AppendLine(string.Format("{0}\t\t{1}\t{2}", row.Cells["FullPath"].Value, row.Cells["ItemTableLackName"].Value,
                                   row.Cells["ItemTableLackDescription"].Value));
                }
                logText.AppendLine();

                logText.AppendLine("��۴��󣨵�ǰ���ࣩ��");
                foreach (DataGridViewRow row in slotErrorView.Rows)
                {
                    logText.AppendLine(string.Format("{0}\t{1}\t{2}\t{3}", row.Cells["NpcInfoPlus"].Value, row.Cells["SlotTypePlus"].Value, 
                                                 row.Cells["SlotErrorName"].Value, row.Cells["SlotErrorDescription"].Value));
                }
                logText.AppendLine();

                logText.AppendLine("NPC��ȱʧ����ǰ���ࣩ��");
                foreach (DataGridViewRow row in npcTableLackView.Rows)
                {
                    logText.AppendLine(string.Format("{0}\t{1}\t{2}\t{3}", row.Cells["NpcInfoPro"].Value, row.Cells["SlotTypePro"].Value,
                                                 row.Cells["NpcTableLackName"].Value, row.Cells["NpcLackDescription"].Value));
                }
                logText.AppendLine();

                logText.AppendLine("��ͨ�����������в�ƥ�䣨��ǰ���ࣩ��");
                foreach (DataGridViewRow row in commonStandbyAnimationCheckView.Rows)
                {
                    logText.AppendLine(string.Format("{0}\t{1}\t{2}\t{3}", row.Cells["NpcInfoEx"].Value, row.Cells["RepresentID"].Value,
                                                    row.Cells["CommonStandbyAnimationName"].Value, row.Cells["CommonStandbyAnimationErrorDesctiption"].Value));
                }
                logText.AppendLine();

                logText.AppendLine("��ͨ�����������ż��ʲ�ƥ�䣨��ǰ���ࣩ��");
                foreach (DataGridViewRow row in commonStandbyAnimationRateCheckView.Rows)
                {
                    logText.AppendLine(string.Format("{0}\t{1}\t{2}\t{3}", row.Cells["NpcInfoUlti"].Value, row.Cells["RepresentIDPro"].Value,
                                                    row.Cells["IdleAniRatioFiled"].Value, row.Cells["DescriptionUlti"].Value));
                }
                logText.AppendLine();

                Helper.GetHelper().SaveDataToFile(fileName, logText.ToString());
                MessageBox.Show(string.Format("�ļ�{0}����ɹ���", fileName), "������־",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }                                  
        }

        /// <summary>
        /// ������־
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            ExportLog();
        }

        /// <summary>
        /// �ر�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            this.Close();
            DialogResult = DialogResult.Cancel;
        }

        /// <summary>
        /// ѡ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void checkTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            int index = checkTypeComboBox.SelectedIndex;

            if (index >= 0)
            {
                tabControl1.SelectedTabIndex = index;
            }
        }
    }
}