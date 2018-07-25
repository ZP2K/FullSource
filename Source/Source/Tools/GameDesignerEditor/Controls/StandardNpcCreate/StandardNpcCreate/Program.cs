using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.IO;
using System.Data;
using System.Collections;
using System.Reflection;

namespace StandardNpcCreate
{
    public class Program
    {
        private SqlConnection conn; // sql����
        private string npcModel; // npcģ��
        private string leftHandMesh; // ���ֹ���
        private string rightHandMesh; // ���ֹ���
        private string faceMesh; // ����
        private string hatMesh; // ����
        private string lhMesh; // ����
        private string lpMesh; // ����
        private string lcMesh; // ����
        private string rhMesh; // ����
        private string rpMesh; // ����
        private string rcMesh; // ����
        private string longMesh; // ����
        private string spineMesh; // ����
        private string spine2Mesh; // ����
        private string leftHandMaterial; // ������������
        private string rightHandMaterial; // ������������
        private string faceMaterial; // ����
        private string hatMaterial; // ����
        private string lhMaterial; // ����
        private string lpMaterial; // ����
        private string lcMaterial; // ����
        private string rhMaterial; // ����
        private string rpMaterial; // ����
        private string rcMaterial; // ����
        private string longMaterial; // ����
        private string spineMaterial; // ����
        private string spine2Material; // ����
        private int npcTemplateID = 0; // npcģ��ID
        private int npcRepresentID = 0; // npc����ID
        private string mapName; // npc����������
        private string longRangeWeaponType = "����"; // Զ����������
        private string modelForce; // ģ����������
        
        private DataTable boneToActionTable; // ���ݱ�
        private DataTable itemForInsertTable; // ���ݱ�
        private DataTable modelTable; // ���ݱ�     
        private DataTable npcGlobeTable; // npcȫ�ֱ�
        private bool updateExistAnimation; // �Ƿ�������ж���
        private string relevatePath; // ���·��
        private Random random = new Random(); // ���������
        private StringBuilder logText = new StringBuilder(); // ��־�ı�
        private string rootDir; // ��Ŀ¼
        private float modelScale = 1; // ģ�����ű���
        private List<int> updateSuccessIDList = new List<int>(); // ���³ɹ�ID����

        private string npcDisplayName; // npc��������
        private Type boolType;
        Helper helper = Helper.GetHelper();

        enum UserSection {None, AutoCreate, NotCreate}; // �û�ѡ��ö��
        UserSection userSection = UserSection.None; // ��ǰ�û�ѡ�� 

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        public Program(SqlConnection conn)
        {
            this.conn = conn;
        }        

        /// <summary>
        /// ģ����������
        /// </summary>
        public string ModelForce
        {
            set
            {
                modelForce = value;
            }

            get
            {
                return modelForce;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string Spine2Material
        {
            set
            {
                spine2Material = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string SpineMaterial
        {
            set
            {
                spineMaterial = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string LongMaterial
        {
            set
            {
                longMaterial = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string RCMaterial
        {
            set
            {
                rcMaterial = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string RPMaterial
        {
            set
            {
                rpMaterial = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string RHMaterial
        {
            set
            {
                rhMaterial = value;
                rightHandMaterial = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string LCMaterial
        {
            set
            {
                lcMaterial = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string LPMaterial
        {
            set
            {
                lpMaterial = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string LHMaterial
        {
            set
            {
                lhMaterial = value;
                leftHandMaterial = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string HatMaterial
        {
            set
            {
                hatMaterial = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string FaceMaterial
        {
            set
            {
                faceMaterial = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string Spine2Mesh
        {
            set
            {
                spine2Mesh = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string SpineMesh
        {
            set
            {
                spineMesh = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string LongMesh
        {
            set
            {
                longMesh = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string RCMesh
        {
            set
            {
                rcMesh = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string RPMesh
        {
            set
            {
                rpMesh = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string RHMesh
        {
            set
            {
                rhMesh = value;
                rightHandMesh = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string LCMesh
        {
            set
            {
                lcMesh = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string LPMesh
        {
            set
            {
                lpMesh = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string LHMesh
        {
            set
            {
                lhMesh = value;
                leftHandMesh = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string HatMesh
        {
            set
            {
                hatMesh = value;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string FaceMesh
        {
            set
            {
                faceMesh = value;
            }
        }

        /// <summary>
        /// Զ����������
        /// </summary>
        public string LongRangeWeaponType
        {
            set
            {
                longRangeWeaponType = value;
            }
        }

        /// <summary>
        /// npc����������
        /// </summary>
        public string MapName
        {
            set
            {
                mapName = value;
            }
        }

        /// <summary>
        /// npcģ��ID
        /// </summary>
        public int NpcTemplateID
        {
            set
            {
                npcTemplateID = value;
            }
        }

        /// <summary>
        /// npc����ID
        /// </summary>
        public int NpcRepresentID
        {
            set
            {
                npcRepresentID = value;
            }
        }

        /// <summary>
        /// npcģ��
        /// </summary>
        public string NpcModel
        {
            set
            {
                npcModel = value;
            }
        }

        /// <summary>
        /// ��Ŀ¼
        /// </summary>
        public string RootDir
        {
            set
            {
                rootDir = value;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {            
            // ��ʼ������
            bool tempBool = true;
            boolType = tempBool.GetType();

            // ��ȡ�ļ�����
            string filePath = Path.Combine(Application.StartupPath, @"Design\BoneToAction.tab");
            string content = helper.GetFileContent(filePath);   
         
            if (content != null)
            {
                boneToActionTable = helper.CreateDataTable(content);
            }

            filePath = Path.Combine(Application.StartupPath, @"Design\ItemForInsert.tab");
            content = helper.GetFileContent(filePath);

            if (content != null)
            {
                itemForInsertTable = helper.CreateDataTable(content);
            }

            filePath = Path.Combine(Application.StartupPath, @"Design\Mdl.tab");
            content = helper.GetFileContent(filePath);

            if (content != null)
            {
                modelTable = helper.CreateDataTable(content);
            }            

            // ��ʼ�����ݱ�
            ReloadDataTable();
        }

        /// <summary>
        /// ��������NPC
        /// </summary>
        /// <param name="baseTree">��ǰ������</param>
        public void MultiCopyNPC(TreeView baseTree)
        {            
            TreeNode treeNode = baseTree.SelectedNode;

            if (treeNode != null && treeNode.Level == 1)
            {
                treeNode = treeNode.Parent;
            }

            if (treeNode != null)
            {
                string sourceMapName = treeNode.Text;
                string destinationMapName = string.Format("{0}����", treeNode.Text);
                List<string> idList = new List<string>();
                List<string> displayTextList = new List<string>();
                string sqlString = string.Format("SELECT * FROM NpcTemplate WHERE MapName = '{0}'", sourceMapName);
                DataTable npcTable = helper.GetDataTable(conn, sqlString);
                SqlTransaction transaction = null;

                try
                {
                    if (conn.State == ConnectionState.Closed)
                    {
                        conn.Open();
                    }

                    transaction = conn.BeginTransaction();
                    SqlCommand cmd = conn.CreateCommand();
                    cmd.Transaction = transaction;

                    sqlString = "SELECT MAX(ID) FROM NpcTemplate";
                    cmd.CommandText = sqlString;
                    int maxID = (int)cmd.ExecuteScalar() + 1;

                    foreach (DataRow row in npcTable.Rows)
                    {
                        string npcName = row["Name"].ToString();

                        // �Ȳ���������
                        sqlString = string.Format("INSERT INTO NpcTemplate (ID) VALUES ('{0}')", maxID.ToString());
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();

                        foreach (DataColumn column in npcTable.Columns)
                        {
                            string data = row[column.ColumnName].ToString();

                            switch (column.ColumnName)
                            {
                                case "ID":
                                    {
                                        break;
                                    }
                                case "MapName":
                                    {
                                        sqlString = string.Format("UPDATE NpcTemplate SET MapName = '{0}' WHERE ID = '{1}'",
                                                                  destinationMapName, maxID.ToString());
                                        cmd.CommandText = sqlString;
                                        cmd.ExecuteNonQuery();

                                        break;
                                    }
                                default:
                                    {
                                        if (data != "")
                                        {
                                            if (column.DataType == boolType) // ����������Ҫ���⴦����
                                            {
                                                switch (data)
                                                {
                                                    case "True":
                                                        {
                                                            data = "1";
                                                            break;
                                                        }
                                                    case "False":
                                                        {
                                                            data = "0";
                                                            break;
                                                        }
                                                }
                                            }

                                            sqlString = string.Format("UPDATE NpcTemplate SET [{0}] = '{1}' WHERE ID = '{2}'", 
                                                                      column.ColumnName, data, maxID.ToString());
                                            cmd.CommandText = sqlString;
                                            cmd.ExecuteNonQuery();
                                        }

                                        break;
                                    }
                            }                            
                        }

                        idList.Add(maxID.ToString());
                        displayTextList.Add(string.Format("{0}({1})", npcName, maxID.ToString()));
                        maxID++;
                    }

                    transaction.Commit();

                    // ������Ӧ�������
                    TreeNode parentNode = new TreeNode(destinationMapName);
                    parentNode.Tag = destinationMapName;
                    baseTree.Nodes.Add(parentNode);

                    for (int i = 0; i < idList.Count; i++)
                    {
                        TreeNode newNode = new TreeNode(displayTextList[i]);
                        newNode.Tag = new object[] { int.Parse(idList[i]) };
                        parentNode.Nodes.Add(newNode);
                    }

                    baseTree.SelectedNode = parentNode;
                }
                catch (SqlException ex)
                {
                    MessageBox.Show("�ڸ���NPC���ݵ�ʱ�����sql�쳣��" + ex.Message, "��������NPC", MessageBoxButtons.OK,
                                    MessageBoxIcon.Information);

            	    if (transaction != null)
                    {
                        transaction.Rollback();
                    }
                }
                finally
                {
                    if (conn.State == ConnectionState.Open)
                    {
                        conn.Close();
                    }
                }
            }                       
        }

        /// <summary>
        /// ����ˢ��һ�����ݱ�
        /// </summary>
        public void ReloadDataTable()
        {
            string sqlString = "SELECT ID, Name, ForceID, RepresentID1, SkillID1, SkillID2, SkillID3, SkillID4, MapName FROM NpcTemplate";
            npcGlobeTable = helper.GetDataTable(conn, sqlString);
        }

        /// <summary>
        /// �༭npc���ִ���
        /// </summary>
        /// <returns>�Ƿ���ɹ�</returns>
        public bool EditWithoutMessage()
        {
            bool editSuccess = false;

            InitModelScale();
            editSuccess = AutoCreateActions(false, updateExistAnimation, true);

            return editSuccess;
        }

        /// <summary>
        /// �༭npc���ִ���
        /// </summary>
        /// <returns>�Ƿ���ɹ�</returns>
        public bool Edit()
        {
            bool editSuccess = false;
            logText = new StringBuilder();            

            switch (userSection)
            {
                case UserSection.None: // δ����ѡ��
                    {
                        NotifyForm nForm = new NotifyForm();
                        DialogResult dialogResult = nForm.ShowDialog();

                        switch (dialogResult)
                        {
                            case DialogResult.OK:
                                {
                                    InitModelScale();
                                    editSuccess = AutoCreateActions(true, nForm.UpdateExistAnimation, true);                                    

                                    if (nForm.RecordSelection)
                                    {
                                        userSection = UserSection.AutoCreate;
                                        updateExistAnimation = nForm.UpdateExistAnimation;
                                    }

                                    break;
                                }
                            case DialogResult.Cancel:
                                {
                                    if (nForm.RecordSelection)
                                    {
                                        userSection = UserSection.NotCreate;
                                    }

                                    break;
                                }
                        }

                        break;
                    }
                case UserSection.AutoCreate:
                    {
                        InitModelScale();
                        editSuccess = AutoCreateActions(true, updateExistAnimation, true);                        

                        break;
                    }
                case UserSection.NotCreate:
                    {
                        break;
                    }
            }           

            return editSuccess;
        }

        /// <summary>
        /// ���¶�����Ϣ����
        /// </summary>
        /// <param name="cmd">sql����</param>
        private void UpdateAnimationPlusInfo(SqlCommand cmd, string animationID)
        {
            // AttackMove��IsAnimationShockWave��IsSFXShockWave�������ֶ����Ϊ�գ�����Ϊ0
            string sqlString = string.Format("UPDATE animation_npc SET AttackMove = '0' WHERE RepresentID = '{0}' AND AnimationID = '{1}'",
                                             npcRepresentID.ToString(), animationID);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();

            sqlString = string.Format("UPDATE animation_npc SET IsAnimationShockWave = '0' WHERE RepresentID = '{0}' AND AnimationID = '{1}'",
                                      npcRepresentID.ToString(), animationID);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();

            sqlString = string.Format("UPDATE animation_npc SET IsSFXShockWave = '0' WHERE RepresentID = '{0}' AND AnimationID = '{1}'",
                                      npcRepresentID.ToString(), animationID);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();

            // SFXScale��SFXSpeed��SFXType��AnimationSpeed���ĸ��ֶ�����ǿպ�0����ȫ���ĳ�1
            sqlString = string.Format("UPDATE animation_npc SET SFXScale = '1' WHERE RepresentID = '{0}' AND AnimationID = '{1}'",
                                      npcRepresentID.ToString(), animationID);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();

            sqlString = string.Format("UPDATE animation_npc SET SFXSpeed = '1' WHERE RepresentID = '{0}' AND AnimationID = '{1}'",
                                      npcRepresentID.ToString(), animationID);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();

            sqlString = string.Format("UPDATE animation_npc SET SFXType = '1' WHERE RepresentID = '{0}' AND AnimationID = '{1}'",
                                      npcRepresentID.ToString(), animationID);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();

            sqlString = string.Format("UPDATE animation_npc SET AnimationSpeed = '1' WHERE RepresentID = '{0}' AND AnimationID = '{1}'",
                                      npcRepresentID.ToString(), animationID);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();

            // IsLoadDefaultSocket�� ʼ����Ϊ1
            sqlString = string.Format("UPDATE animation_npc SET IsLoadDefaultSocket = '1' WHERE RepresentID = '{0}' AND AnimationID = '{1}'",
                                      npcRepresentID.ToString(), animationID);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();
        }

        /// <summary>
        /// ��ʼ��ģ�����ű���
        /// </summary>        
        private void InitModelScale()
        {
            string sqlString = string.Format("SELECT ModelScale FROM npc WHERE RepresentID = '{0}'",
                                             npcRepresentID.ToString());
            DataTable dataTable = helper.GetDataTable(conn, sqlString);

            if (dataTable.Rows.Count > 0)
            {
                DataRow dataRow = dataTable.Rows[0];
                string modelScaleString = dataRow["ModelScale"].ToString();

                if (modelScaleString != "" && modelScaleString != "0")
                {
                    modelScale = float.Parse(modelScaleString);
                }
                else
                {
                    modelScale = 1;
                }
            }
        }

        /// <summary>
        /// ��NPC�����ַ�����ȡ��NPC��ID
        /// </summary>
        /// <param name="npcString">NPC�����ַ���</param>
        /// <return>NPC��ID</return>
        private int GetNpcID(string npcString)
        {
            int npcID = -1;

            int firstIndex = npcString.LastIndexOf("(");
            int lastIndex = npcString.LastIndexOf(")");

            if (firstIndex >= 0 && lastIndex >= 0)
            {
                npcID = int.Parse(npcString.Substring(firstIndex + 1, lastIndex - firstIndex - 1));
            }

            return npcID;
        }

        /// <summary>
        /// ���¶��NPC����
        /// </summary>
        /// <param name="mapName">��ͼ��</param>        
        /// <param name="templateID">ģ��ID</param>
        private void MultiUpdate(string mapName, int templateID)
        {
            List<string> selectedNpcList = new List<string>();            
            logText = new StringBuilder();
            this.mapName = mapName;
            updateSuccessIDList.Clear();

            string sqlString = "SELECT * FROM npc";
            DataTable representTable = helper.GetDataTable(conn, sqlString);

            DataTable npcTable = null;
            if (templateID == 0) // ���¶��npc
            {
                sqlString = string.Format("SELECT ID, Name, RepresentID1 FROM NpcTemplate WHERE MapName = '{0}'", mapName);
                npcTable = helper.GetDataTable(conn, sqlString);
            }
            else // ���µ���npc
            {
                sqlString = string.Format("SELECT ID, Name, RepresentID1 FROM NpcTemplate WHERE ID = '{0}'", templateID.ToString());
                npcTable = helper.GetDataTable(conn, sqlString);
            }            

            sqlString = string.Format("SELECT * FROM npc_animation_record WHERE MapName = '{0}'", mapName);
            DataTable npcRecordTable = helper.GetDataTable(conn, sqlString);

            Hashtable representID1Table = new Hashtable();

            foreach(DataRow dataRow in npcTable.Rows)
            {
                string representID1 = dataRow["RepresentID1"].ToString();
                string npcName = dataRow["Name"].ToString();
                string npcID = dataRow["ID"].ToString();
                representID1Table[npcID] = representID1;

                if (representID1 != "" && representID1 != "0")
                {
                    DataRow[] rows = npcRecordTable.Select(string.Format("ID = '{0}'", npcID));

                    if (rows.Length > 0)
                    {
                        DataRow row = rows[0];

                        if (row["IsSpecific"].ToString() == "True") // ������NPC
                        {
                            selectedNpcList.Add(string.Format("{0} ({1})    [����]", npcName, npcID));
                        }
                        else
                        {
                            if (CheckAnimationIntegrity(representID1)) // ��������
                            {
                                selectedNpcList.Add(string.Format("{0} ({1})", npcName, npcID));
                            }
                            else
                            {
                                selectedNpcList.Add(string.Format("{0} ({1})    [����������]", npcName, npcID));
                            } 
                        }
                    }
                    else
                    {
                        if (CheckAnimationIntegrity(representID1)) // ��������
                        {
                            selectedNpcList.Add(string.Format("{0} ({1})", npcName, npcID));
                        }
                        else
                        {
                            selectedNpcList.Add(string.Format("{0} ({1})    [����������]", npcName, npcID));
                        }                        
                    }                   
                }                
            }

            MultiUpdateForm mForm = new MultiUpdateForm(selectedNpcList, conn, mapName);

            if (mForm.ShowDialog() == DialogResult.OK)
            {
                DataRow[] rows;
                DataRow row;   
                bool updateExistAnimation = mForm.UpdateExistAnimation;
                bool updateCommonStandbyAnimation = mForm.UpdateCommonStandbyAnimation;
                
                // ���¶���
                List<string> successNpcList = new List<string>();
                List<string> failNpcList = new List<string>();
                this.mapName = mapName;                

                foreach (string s in selectedNpcList)
                {
                    npcDisplayName = s;
                    int npcID = GetNpcID(npcDisplayName);
                    string representID1 = representID1Table[npcID.ToString()] as string;

                    if (!string.IsNullOrEmpty(representID1) && representID1 != "0")
                    {
                        rows = representTable.Select(string.Format("RepresentID = '{0}'", representID1));                    

                        if (rows.Length > 0)
                        {
                            row = rows[0];
                            this.npcModel = row["MainModelFile"].ToString();
                            this.leftHandMesh = row["S_LH_MeshFile"].ToString();
                            this.rightHandMesh = row["S_RH_MeshFile"].ToString();
                            this.faceMesh = row["S_Face_MeshFile"].ToString();
                            this.hatMesh = row["S_Hat_MeshFile"].ToString();
                            this.lhMesh = row["S_LH_MeshFile"].ToString();
                            this.lpMesh = row["S_LP_MeshFile"].ToString();
                            this.lcMesh = row["S_LC_MeshFile"].ToString();
                            this.rhMesh = row["S_RH_MeshFile"].ToString();
                            this.rpMesh = row["S_RP_MeshFile"].ToString();
                            this.rcMesh = row["S_RC_MeshFile"].ToString();
                            this.longMesh = row["S_Long_MeshFile"].ToString();
                            this.spineMesh = row["S_Spine_MeshFile"].ToString();
                            this.spine2Mesh = row["S_Spine2_MeshFile"].ToString();
                            this.leftHandMaterial = row["S_LH_MaterialFile"].ToString();
                            this.rightHandMaterial = row["S_RH_MaterialFile"].ToString();
                            this.faceMaterial = row["S_Face_MaterialFile"].ToString();
                            this.hatMaterial = row["S_Hat_MaterialFile"].ToString();
                            this.lhMaterial = row["S_LH_MaterialFile"].ToString();
                            this.lpMaterial = row["S_LP_MaterialFile"].ToString();
                            this.lcMaterial = row["S_LC_MaterialFile"].ToString();
                            this.rhMaterial = row["S_RH_MaterialFile"].ToString();
                            this.rpMaterial = row["S_RP_MaterialFile"].ToString();
                            this.rcMaterial = row["S_RC_MaterialFile"].ToString();
                            this.longMaterial = row["S_Long_MaterialFile"].ToString();
                            this.spineMaterial = row["S_Spine_MaterialFile"].ToString();
                            this.spine2Material = row["S_Spine2_MaterialFile"].ToString();
                            this.longRangeWeaponType = row["LongRangeWeaponType"].ToString();
                            this.npcRepresentID = int.Parse(representID1);                        
                            this.npcTemplateID = npcID;

                            // �滻����
                            if (this.leftHandMesh == "")
                            {
                                this.leftHandMesh = row["LeftHandMesh"].ToString();

                                if (this.leftHandMaterial == "")
                                {
                                    this.leftHandMaterial = row["LeftHandMaterial"].ToString();
                                }
                            }

                            if (this.rightHandMesh == "")
                            {
                                this.rightHandMesh = row["RightHandMesh"].ToString();

                                if (this.rightHandMaterial == "")
                                {
                                    this.rightHandMaterial = row["RightHandMaterial"].ToString();
                                }
                            }

                            string modelScaleString = row["ModelScale"].ToString();
                            if (modelScaleString != "" && modelScaleString != "0")
                            {
                                modelScale = float.Parse(modelScaleString);
                            }
                            else
                            {
                                modelScale = 1;
                            }

                            bool success = AutoCreateActions(false, updateExistAnimation, updateCommonStandbyAnimation);

                            // ��¼�ɹ�ʧ����־                                                                              
                            if (success)
                            {
                                successNpcList.Add(npcDisplayName);
                            }
                            else
                            {
                                failNpcList.Add(npcDisplayName);
                            }                                                
                        }
                    }                    
                }

                UpdateResultForm rForm = new UpdateResultForm(successNpcList, failNpcList);
                rForm.LogText = logText.ToString();
                rForm.Init();
                rForm.ShowDialog();
            }
        }

        /// <summary>
        /// ��鶯����������
        /// </summary>
        /// <param name="representID">����ID</param>
        /// <returns>�����Ƿ�����</returns>
        private bool CheckAnimationIntegrity(string representID)
        {
            DataRow[] rows;            
            string sqlString = string.Format("SELECT AnimationID FROM animation_npc WHERE RepresentID = '{0}'", representID);
            DataTable animationTable = helper.GetDataTable(conn, sqlString);

            List<string> conditionList = new List<string>(); // �����������
            conditionList.Add("AnimationID = 30 OR AnimationID = 31 OR AnimationID = 32 OR AnimationID = 33 OR AnimationID = 34 OR AnimationID = 35"); // �����ͨ��������
            conditionList.Add("AnimationID = 9"); // ������߶���
            conditionList.Add("AnimationID = 16"); // ����ܲ�����
            conditionList.Add("AnimationID = 47"); // ���ս����������
            conditionList.Add("AnimationID = 73"); // ���ս����������
            conditionList.Add("AnimationID = 54 OR AnimationID = 55 OR AnimationID = 56 OR AnimationID = 57"); // ���ս����������
            conditionList.Add("AnimationID = 64"); // ��鼼��BUFF����
            conditionList.Add("AnimationID = 65"); // ��鼼��AOE����
            conditionList.Add("AnimationID = 66"); // ���ս���ػ�����
            conditionList.Add("AnimationID = 67"); // ��鼼��Զ�̶���
            conditionList.Add("AnimationID = 1"); // �����������
            conditionList.Add("AnimationID = 2"); // ��鱻��������
            conditionList.Add("AnimationID = 5"); // ���������
            conditionList.Add("AnimationID = 287"); // ��黬������
            conditionList.Add("AnimationID = 250"); // ������ܶ���
            conditionList.Add("AnimationID = 69"); // ����������
            conditionList.Add("AnimationID = 68"); // �������ͷŶ���
            conditionList.Add("AnimationID = 3"); // �����˶���
            conditionList.Add("AnimationID = 291"); // ��鵹���ж���
            conditionList.Add("AnimationID = 292"); // ��鵹�ر�������
            conditionList.Add("AnimationID = 5"); // ���������
            conditionList.Add("AnimationID = 80"); // ���ѣ�ζ���
            conditionList.Add("AnimationID = 84"); // ���񵲶���
            conditionList.Add("AnimationID = 288"); // ��黬��ֹͣ����

            foreach (string condition in conditionList)
            {
                rows = animationTable.Select(condition);

                if (rows.Length == 0)
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// ��鶯���Ƿ���Ч
        /// </summary>
        /// <param name="mapName">��ͼ��</param>
        /// <param name="templateID">ģ����</param>
        public void CheckAnimationValid(string mapName, int templateID)
        {
            string sqlString = "SELECT * FROM npc";
            DataTable representTable = helper.GetDataTable(conn, sqlString);

            sqlString = "SELECT * FROM animation_npc";
            DataTable animationTable = helper.GetDataTable(conn, sqlString);

            DataTable npcTable = null;
            
            if (templateID == 0) // �������npc
            {
                sqlString = string.Format("SELECT ID, Name, RepresentID1 FROM NpcTemplate WHERE MapName = '{0}'", mapName);
                npcTable = helper.GetDataTable(conn, sqlString);
            }
            else // ��鵥��npc
            {
                sqlString = string.Format("SELECT ID, Name, RepresentID1 FROM NpcTemplate WHERE ID = '{0}'", templateID.ToString());
                npcTable = helper.GetDataTable(conn, sqlString);
            }                                    

            AnimationCheckForm aForm = new AnimationCheckForm(rootDir, boneToActionTable, itemForInsertTable, npcTable, representTable, animationTable);
            aForm.ShowDialog();
        }

        /// <summary>
        /// ��ȡ���Ч��ID
        /// </summary>
        /// <param name="itemNumber">ItemNumber</param>
        /// <returns>���Ч��ID</returns>
        private string GetHitEffectResultID(string itemNumber)
        {
            string hitEffectResultID = "5000";

            DataRow[] rows = itemForInsertTable.Select(string.Format("ItemNumber = '{0}'", itemNumber));
            if (rows.Length > 0)
            {
                DataRow row = rows[0];
                string tempValue = row["EffectType"] as string;

                if (!string.IsNullOrEmpty(tempValue))
                {
                    switch(tempValue)
                    {
                        case "1": // �����������ʣ�ذ�ף��ֵ���
                            {
                                hitEffectResultID = "5010";
                                break;
                            }
                        case "2": // ���������������⹳����ͷ��
                            {
                                hitEffectResultID = "5015";
                                break;
                            }
                        case "3": // �����������󵶣��ؽ���ǹ���󸫣�
                            {
                                hitEffectResultID = "5020";
                                break;
                            }
                        case "4": // ����������ľ����ǯ��С����
                            {
                                hitEffectResultID = "5025";
                                break;
                            }
                        case "5": // ���Ͷ��������ӣ��������ê���󴸣�
                            {
                                hitEffectResultID = "5030";
                                break;
                            }
                        case "6": // צ��ȭ�ף�
                            {
                                hitEffectResultID = "5005";
                                break;
                            }
                    }
                }
            }

            return hitEffectResultID;
        }

        /// <summary>
        /// ��ȡItemForInsert.tab��Keyֵ
        /// </summary>
        /// <param name="meshFile">mesh·��</param>
        /// <returns>ItemForInsert.tab��Keyֵ</returns>
        private string GetItemNumber(string meshFile)
        {
            string itemNumber = null;

            string[] data = meshFile.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
            if (data.Length > 0)
            {
                string meshName = data[data.Length - 1];
                itemNumber = meshName.Replace(".Mesh", "").Replace(".mesh", "");                
            }

            return itemNumber;
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
 
                switch(modelClass)
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
        /// �������ȼ���䶯��
        /// </summary>
        /// <param name="dataRow">������</param>
        /// <returns>���ݱ�</returns>
        private Hashtable FillAnimationWithPriorty(DataRow dataRow)
        {
            Hashtable animationTable = new Hashtable();
            string[] tempArray;

            string walkAnimation = dataRow["Walk"] as string; // ����
            string runAnimation = dataRow["Run"] as string; // �ܲ�
            string battleStandbyAnimation = dataRow["BattleStandby"] as string; // ս������
            string commonStandbyAnimation = dataRow["CommonStandbySeries1"] as string; // ��ͨ����
            string battleUnderAttackAnimation = dataRow["BattleUnderAttack"] as string; // ս������
            string attackSeriesAnimation = dataRow["AttackSeries"] as string; // ս������
            string attackBuffAnimation = dataRow["AttackBuff"] as string; // ����buff           
            string attackAoeAnimation = dataRow["AttackAoe"] as string; // ����aoe
            string attackBangAnimation = dataRow["AttackBang"] as string; // ս���ػ�
            string attackLongRangeAnimation = dataRow["AttackLongRange"] as string; // Զ�̹���
            string deathAnimation = dataRow["Death"] as string; // ����
            string knockDownStandUpAnimation = dataRow["KnockDown_StandUp"] as string; // ��������
            string onGroundAnimation = dataRow["OnGround"] as string; // ������
            string onGroundUnderAttackAnimation = dataRow["OnGroundUnderAttack"] as string; // ���ر���
            string beatBackAnimation = dataRow["BeatBack"] as string; // ������
            string glissadeAnimation = dataRow["Glissade"] as string; // ����
            string voltAnimation = dataRow["Volt"] as string; // ����
            string intonateReleaseAnimation = dataRow["Intonate_Release"] as string; // �����ͷ�
            string vertigoAnimation = dataRow["Vertigo"] as string; // ѣ��
            string wardOffAnimation = dataRow["WardOff"] as string; // ��
            string stopGlissadeAnimation = dataRow["StopGlissade"] as string; // ����ֹͣ
            string animationGroup = dataRow["Knockdown_OnGround_OnGroundUnderAttack_Standup_death_Time"] as string; // ������          

            // ���ȡ������������
            int commonStandbyAnimationIndex = 0;
            List<string> validCommonStandbyAnimationList = new List<string>();
            string commonStandbyAnimation1 = dataRow["CommonStandbySeries1"] as string;
            string commonStandbyAnimation2 = dataRow["CommonStandbySeries2"] as string;
            string commonStandbyAnimation3 = dataRow["CommonStandbySeries3"] as string;
            string commonStandbyAnimation4 = dataRow["CommonStandbySeries4"] as string;
            string commonStandbyAnimation5 = dataRow["CommonStandbySeries5"] as string;
            string commonStandbyAnimation6 = dataRow["CommonStandbySeries6"] as string;

            if (!string.IsNullOrEmpty(commonStandbyAnimation1))
            {
                validCommonStandbyAnimationList.Add(commonStandbyAnimation1);                
            }

            if (!string.IsNullOrEmpty(commonStandbyAnimation2))
            {
                validCommonStandbyAnimationList.Add(commonStandbyAnimation2);                
            }

            if (!string.IsNullOrEmpty(commonStandbyAnimation3))
            {
                validCommonStandbyAnimationList.Add(commonStandbyAnimation3);                
            }

            if (!string.IsNullOrEmpty(commonStandbyAnimation4))
            {
                validCommonStandbyAnimationList.Add(commonStandbyAnimation4);                
            }

            if (!string.IsNullOrEmpty(commonStandbyAnimation5))
            {
                validCommonStandbyAnimationList.Add(commonStandbyAnimation5);                
            }

            if (!string.IsNullOrEmpty(commonStandbyAnimation6))
            {
                validCommonStandbyAnimationList.Add(commonStandbyAnimation6);                
            }

            if (validCommonStandbyAnimationList.Count > 1)
            {
                commonStandbyAnimationIndex = random.Next(validCommonStandbyAnimationList.Count);
                commonStandbyAnimation = validCommonStandbyAnimationList[commonStandbyAnimationIndex];
            }                        

            if (string.IsNullOrEmpty(commonStandbyAnimation)) // �����ͨ���������Ƿ�Ϊ��
            {
                // ���滻����
            }

            if (string.IsNullOrEmpty(battleStandbyAnimation)) // ���ս�����������Ƿ�Ϊ��
            {
                // �滻Ϊ��ͨ��������
                battleStandbyAnimation = commonStandbyAnimation;
            }

            if (string.IsNullOrEmpty(battleUnderAttackAnimation)) // ���ս�����������Ƿ�Ϊ��
            {
                // �滻Ϊս����������
                battleUnderAttackAnimation = battleStandbyAnimation;
            }

            if (string.IsNullOrEmpty(attackSeriesAnimation)) // ���ս�����������Ƿ�Ϊ��
            {
                // �滻Ϊս����������
                attackSeriesAnimation = battleStandbyAnimation;
            }                        

            if (string.IsNullOrEmpty(runAnimation)) // ����ܲ������Ƿ�Ϊ��
            {
                // �滻Ϊ��ͨ��������
                runAnimation = commonStandbyAnimation;
            }

            if (string.IsNullOrEmpty(walkAnimation)) // ������߶����Ƿ�Ϊ��
            {
                // �滻Ϊ�ܲ�����
                walkAnimation = runAnimation;
            }

            if (string.IsNullOrEmpty(voltAnimation)) // ������ܶ����Ƿ�Ϊ��
            {
                // �滻Ϊ��������
                voltAnimation = battleUnderAttackAnimation;
            }

            if (string.IsNullOrEmpty(vertigoAnimation)) // ���ѣ�ζ����Ƿ�Ϊ��
            {
                // �滻Ϊ��������
                vertigoAnimation = battleUnderAttackAnimation;
            }

            if (string.IsNullOrEmpty(wardOffAnimation)) // ���񵲶����Ƿ�Ϊ��
            {
                // �滻Ϊ��������
                wardOffAnimation = battleUnderAttackAnimation;
            }

            if (string.IsNullOrEmpty(beatBackAnimation)) // ��鱻���˶����Ƿ�Ϊ��
            {
                // �滻Ϊս����������
                beatBackAnimation = battleStandbyAnimation;
            }

            if (string.IsNullOrEmpty(attackBuffAnimation)) // ��鼼��buff�����Ƿ�Ϊ��
            {
                // �滻Ϊս����������                
                attackBuffAnimation = attackSeriesAnimation;                
            }

            if (string.IsNullOrEmpty(attackAoeAnimation)) // ��鼼��aoe�����Ƿ�Ϊ��
            {
                // �滻Ϊս����������                
                attackAoeAnimation = attackSeriesAnimation;                
            }

            if (string.IsNullOrEmpty(attackBangAnimation)) // ���ս���ػ������Ƿ�Ϊ��
            {
                // �滻Ϊս����������
                attackBangAnimation = attackSeriesAnimation;                
            }

            if (string.IsNullOrEmpty(attackLongRangeAnimation)) // ���Զ�̹��������Ƿ�Ϊ��
            {
                // �滻Ϊս����������
                attackLongRangeAnimation = attackSeriesAnimation;                
            }

            if (string.IsNullOrEmpty(deathAnimation)) // ������������Ƿ�Ϊ��
            {
                // �滻Ϊ��ͨ��������
                deathAnimation = commonStandbyAnimation;
            }

            if (string.IsNullOrEmpty(onGroundAnimation)) // ��鵹���ж����Ƿ�Ϊ��
            {
                // �滻Ϊ��������
                onGroundAnimation = deathAnimation;
            }

            if (string.IsNullOrEmpty(onGroundUnderAttackAnimation)) // ��鵹�ر��������Ƿ�Ϊ��
            {
                // �滻Ϊ�����ж���
                onGroundUnderAttackAnimation = onGroundAnimation;
            }
           
            if (string.IsNullOrEmpty(glissadeAnimation)) // ��黬�������Ƿ�Ϊ��
            {
                // �滻Ϊ��������
                glissadeAnimation = battleUnderAttackAnimation;
            }           

            if (string.IsNullOrEmpty(knockDownStandUpAnimation)) // �������������Ƿ�Ϊ��
            {
                if (!string.IsNullOrEmpty(battleUnderAttackAnimation)) // �滻Ϊ��������
                {
                    tempArray = battleUnderAttackAnimation.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries); // ֻȡ1������                   
                    knockDownStandUpAnimation = string.Format("{0},{1}", tempArray[0], tempArray[0]);
                }
            }

            if (string.IsNullOrEmpty(intonateReleaseAnimation)) // �������ͷŶ����Ƿ�Ϊ��
            {
                if (!string.IsNullOrEmpty(attackSeriesAnimation) && !string.IsNullOrEmpty(battleStandbyAnimation)) // �滻Ϊս������������ս����������
                {
                    tempArray = battleStandbyAnimation.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                    string battleStandbyAnimationString = tempArray[0]; // ֻȡ1������
                    tempArray = attackSeriesAnimation.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                    string attackSeriesAnimationString = tempArray[0]; // ֻȡ1������

                    intonateReleaseAnimation = string.Format("{0},{1}", battleStandbyAnimationString, attackSeriesAnimationString);
                }                
            }

            if (string.IsNullOrEmpty(stopGlissadeAnimation)) // ��黬��ֹͣ�����Ƿ�Ϊ��
            {
                // �滻Ϊս����������
                stopGlissadeAnimation = battleStandbyAnimation;
            }

            if (!string.IsNullOrEmpty(animationGroup)) // ��鶯�����Ƿ�Ϊ��
            {
                // ���Ƚ��������ڶ����滻����Ӧ�ĵ�������
                tempArray = animationGroup.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);

                if (tempArray.Length > 0)
                {
                    tempArray = tempArray[0].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                    if (tempArray.Length == 6)
                    {
                        knockDownStandUpAnimation = string.Format("{0},{1}", tempArray[0], tempArray[3]);
                        onGroundAnimation = tempArray[1];
                        onGroundUnderAttackAnimation = tempArray[2];
                        deathAnimation = tempArray[4];
                    }
                    else
                    {
                        logText.Append(string.Format("���ݸ�ʽ����{0}\r\n", animationGroup));
                    }
                }
            }

            // ��������
            animationTable["Walk"] = walkAnimation;
            animationTable["Run"] = runAnimation;
            animationTable["BattleStandby"] = battleStandbyAnimation;
            animationTable["CommonStandby"] = commonStandbyAnimation;
            animationTable["BattleUnderAttack"] = battleUnderAttackAnimation;
            animationTable["AttackSeries"] = attackSeriesAnimation;
            animationTable["AttackBuff"] = attackBuffAnimation;
            animationTable["AttackAoe"] = attackAoeAnimation;
            animationTable["AttackBang"] = attackBangAnimation;
            animationTable["AttackLongRange"] = attackLongRangeAnimation;
            animationTable["Death"] = deathAnimation;
            animationTable["KnockDownStandUp"] = knockDownStandUpAnimation;
            animationTable["OnGround"] = onGroundAnimation;
            animationTable["OnGroundUnderAttack"] = onGroundUnderAttackAnimation;
            animationTable["BeatBack"] = beatBackAnimation;
            animationTable["Glissade"] = glissadeAnimation;
            animationTable["Volt"] = voltAnimation;
            animationTable["IntonateRelease"] = intonateReleaseAnimation;
            animationTable["Vertigo"] = vertigoAnimation;
            animationTable["WardOff"] = wardOffAnimation;
            animationTable["StopGlissade"] = stopGlissadeAnimation;
            animationTable["BoardHeight"] = dataRow["BoneToHeight"] as string;
            animationTable["SelectionScale"] = dataRow["BoneToSelectionScale"] as string;
            animationTable["BuffInsertPosition"] = dataRow["BuffInsertPosition"] as string;
            animationTable["AOEInsertPosition"] = dataRow["AOEInsertPosition"] as string;
            animationTable["BangInsertPosition"] = dataRow["BangInsertPosition"] as string;            
            animationTable["LongRangeInsertPosition"] = dataRow["LongRangeInsertPosition"] as string;
            // ����ʱ���ε� animationTable["SocketScale"] = dataRow["SocketScale"] as string;

            return animationTable;
        }

        /// <summary>
        /// ��ȡ��������
        /// </summary>
        /// <param name="meshFile">����ļ�</param>
        /// <returns>��������</returns>
        private string GetWeaponType(string meshFile)
        {
            string weaponType = null;
            string itemType = "";

            string[] data = meshFile.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
            if (data.Length > 0)
            {
                string meshName = data[data.Length - 1];
                string itemNumber = meshName.Replace(".Mesh", "").Replace(".mesh", "");
                DataRow[] rows = itemForInsertTable.Select(string.Format("ItemNumber = '{0}'", itemNumber));

                if (rows.Length > 0)
                {
                    itemType = rows[0]["ItemType"].ToString();
                }
            }

            switch (itemType)
            {
                case "1":
                    {
                        weaponType = "����";
                        break;
                    }
                case "2":
                    {
                        weaponType = "����";
                        break;
                    }
                case "3":
                    {
                        weaponType = "����";
                        break;
                    }
                case "4":
                    {
                        weaponType = "ȭ��";
                        break;
                    }
                case "5":
                    {
                        weaponType = "��";
                        break;
                    }
                case "6":
                    {
                        weaponType = "����";
                        break;
                    }
                case "7":
                    {
                        weaponType = "����";
                        break;
                    }
                default:
                    {
                        weaponType = itemType;
                        break;
                    }
            }

            return weaponType;
        }

        /// <summary>
        /// ѡ��DetailType
        /// </summary>
        /// <param name="rows">����������</param>
        /// <param name="showMessage">�Ƿ���ʾ��Ϣ</param>
        /// <returns>ѡ��ı��</returns>
        private int SelectDetailType(DataRow[] rows, bool showMessage)
        {
            int index = 0;
            NpcTypeSelectForm nForm = null;

            if (rows.Length > 1) // �ж��ƥ�����������Ҫ�û�ѡ��
            {
                List<string> detailTypeList = new List<string>();

                foreach (DataRow dataRow in rows)
                {
                    // string forceString = dataRow["BoneForce"].ToString(); ��ʱ���ε�
                    string forceString = "";

                    if (forceString == modelForce || forceString == "" || modelForce == "")
                    {
                        if (forceString == "")
                        {
                            forceString = "������";
                        }

                        detailTypeList.Add(string.Format("{0} ({1})", dataRow["DetailType"].ToString(), forceString));
                    }                                       
                }

                if (showMessage)
                {
                    nForm = new NpcTypeSelectForm(detailTypeList, "NPC����ѡ��");
                }
                else
                {
                    nForm = new NpcTypeSelectForm(detailTypeList, string.Format("NPC����ѡ�� {0}", npcDisplayName));
                }

                if (nForm.ShowDialog() == DialogResult.OK)
                {
                    index = nForm.SelectionIndex;
                }                
            }

            return index;
        }

        /// <summary>
        /// ����npc����
        /// </summary>
        /// <param name="modelType">ģ������</param>
        /// <param name="showMessage">�Ƿ���ʾ��Ϣ</param>
        /// <param name="updateExistAnimation">�Ƿ�������ж���</param>
        /// <param name="updateCommonStandbyAnimation">�Ƿ������ͨ��������</param>
        /// <return>�Ƿ���³ɹ�</return>
        private bool UpdateAnimation(string modelType, bool showMessage, bool updateExistAnimation, bool updateCommonStandbyAnimation)
        {
            bool success = false;
            Hashtable infoTable = null;
            DataRow[] rows;
            DataRow row;
            string leftHandPriorty = null; // ���ֲ�����ȼ�
            string rightHandPriorty = null; // ���ֲ�����ȼ�
            string leftHandWeaponType = GetWeaponType(leftHandMesh); // ����������Դ
            string rightHandWeaponType = GetWeaponType(rightHandMesh); // ����������Դ
            string leftHandItemNumber = GetItemNumber(leftHandMesh); // ���ֲ��keyֵ
            string rightHandItemNumber = GetItemNumber(rightHandMesh); // ���ֲ��keyֵ
            string leftHandHitEffectResultID = GetHitEffectResultID(leftHandItemNumber); // ���ִ��Ч��ID
            string rightHandHitEffectResultID = GetHitEffectResultID(rightHandItemNumber);  // ���ִ��Ч��ID
            string hitEffectResultID = null;

            // ��ʼ�����ֲ�����ȼ�
            if (leftHandItemNumber != null)
            {
                rows = itemForInsertTable.Select(string.Format("ItemNumber = '{0}'", leftHandItemNumber));

                if (rows.Length > 0)
                {
                    row = rows[0];
                    leftHandPriorty = row["S_LH_MeshFile"] as string;
                }
            }

            // ��ʼ�����ֲ�����ȼ�
            if (rightHandItemNumber != null)
            {
                rows = itemForInsertTable.Select(string.Format("ItemNumber = '{0}'", rightHandItemNumber));

                if (rows.Length > 0)
                {
                    row = rows[0];
                    rightHandPriorty = row["S_RH_MeshFile"] as string;
                }
            }
            
            if (!string.IsNullOrEmpty(rightHandMesh)) // �������������дģ����Դ
            {
                if (!string.IsNullOrEmpty(rightHandPriorty) && rightHandPriorty != "0") // ���ģ����Դ�Ƿ���ȷ�������ֲ��
                {
                    if (!string.IsNullOrEmpty(leftHandMesh)) // �������������дģ����Դ 
                    {
                        if (!string.IsNullOrEmpty(leftHandPriorty) && leftHandPriorty != "0") // ���ģ����Դ�Ƿ���ȷ�������ֲ��
                        {
                            if (leftHandWeaponType == rightHandWeaponType) // �Ƚ����ֵ�ģ�ͷ����Ƿ���ͬ
                            {
                                if (leftHandWeaponType == "ȭ��") // ������ȭ������
                                {
                                    // ����Ϊ����������д������
                                    rows = boneToActionTable.Select(string.Format("BoneNumber = '{0}' AND WeaponType = '{1}'",
                                                                    modelType, "����"));
                                    if (rows.Length > 0)
                                    {
                                        row = rows[SelectDetailType(rows, showMessage)];

                                        if (modelType.StartsWith("A")) // ����ģ����Ҫ���˺ҧ
                                        {
                                            hitEffectResultID = "5070"; // ������˺ҧ���͵Ĵ��Ч��
                                        }
                                        else
                                        {
                                            hitEffectResultID = "5000"; // ������ȭ�����͵Ĵ��Ч��
                                        }                                            
                                        infoTable = FillAnimationWithPriorty(row);
                                    }
                                }
                                else if (leftHandWeaponType == "����") // �����ڵ�������
                                {
                                    // �����ڵ���������д������
                                    rows = boneToActionTable.Select(string.Format("BoneNumber = '{0}' AND WeaponType = '{1}'",
                                                                    modelType, "����"));
                                    if (rows.Length > 0)
                                    {
                                        row = rows[SelectDetailType(rows, showMessage)];

                                        hitEffectResultID = rightHandHitEffectResultID; // �����ֵĴ��Ч��
                                        infoTable = FillAnimationWithPriorty(row);
                                    }
                                }
                                else
                                {
                                    // ����Ϊ˫��������д������
                                    rows = boneToActionTable.Select(string.Format("BoneNumber = '{0}' AND WeaponType = '{1}'",
                                                                    modelType, "˫��"));
                                    if (rows.Length > 0)
                                    {
                                        row = rows[SelectDetailType(rows, showMessage)];

                                        hitEffectResultID = rightHandHitEffectResultID; // �����ֵĴ��Ч��
                                        infoTable = FillAnimationWithPriorty(row);
                                    }
                                }
                            }
                            else
                            {
                                // ������ģ������������д������
                                rows = boneToActionTable.Select(string.Format("BoneNumber = '{0}' AND WeaponType = '{1}'",
                                                                modelType, rightHandWeaponType));

                                if (rows.Length > 0)
                                {
                                    row = rows[SelectDetailType(rows, showMessage)];

                                    hitEffectResultID = rightHandHitEffectResultID; // �����ֵĴ��Ч��
                                    infoTable = FillAnimationWithPriorty(row);
                                }
                            }
                        }
                        else
                        {
                            if (showMessage)
                            {
                                MessageBox.Show("ģ����Դ���������ֲ�ۣ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            }
                        }
                    }
                    else
                    {
                        // ��������ģ����Դ������������
                        rows = boneToActionTable.Select(string.Format("BoneNumber = '{0}' AND WeaponType = '{1}'",
                                                        modelType, rightHandWeaponType));

                        if (rows.Length > 0)
                        {
                            row = rows[SelectDetailType(rows, showMessage)];

                            hitEffectResultID = rightHandHitEffectResultID; // �����ֵĴ��Ч��
                            infoTable = FillAnimationWithPriorty(row);
                        }
                    }
                }
                else
                {
                    if (showMessage)
                    {
                        MessageBox.Show("ģ����Դ���������ֲ�ۣ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                }
            }
            else // ������������д������
            {
                rows = boneToActionTable.Select(string.Format("BoneNumber = '{0}' AND WeaponType = '{1}'",
                                                modelType, "����"));
                if (rows.Length > 0)
                {
                    row = rows[SelectDetailType(rows, showMessage)];

                    if (modelType.StartsWith("A")) // ����ģ����Ҫ���˺ҧ
                    {
                        hitEffectResultID = "5070"; // ������˺ҧ���͵Ĵ��Ч��
                    }
                    else
                    {
                        hitEffectResultID = "5000"; // ������ȭ�����͵Ĵ��Ч��
                    }     
                    infoTable = FillAnimationWithPriorty(row);
                }
            }            

            if (infoTable != null)
            {
                success = UpdateAnimation(infoTable, hitEffectResultID, showMessage, updateExistAnimation, updateCommonStandbyAnimation);
            }
            else
            {
                if (showMessage)
                {
                    MessageBox.Show("û���ҵ�ƥ��Ķ�����Դ��", "���¶���",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);                    
                }

                logText.Append("Error��û���ҵ�ƥ��Ķ�����Դ��\r\n");
            }

            return success;
        }

        /// <summary>
        /// ��鶯���Ƿ��Ѿ�����
        /// </summary>
        /// <param name="animationTable">������</param>
        /// <param name="animationID">����ID</param>
        /// <returns>�����Ƿ��Ѿ�����</returns>
        private bool AnimationExist(DataTable animationTable, string animationID)
        {
            DataRow[] rows = animationTable.Select(string.Format("AnimationID = '{0}'", animationID));
            bool result = rows.Length > 0;

            return result;
        }

        /// <summary>
        /// ��ȡ�����ļ�·��
        /// </summary>
        /// <param name="animationTable">������</param>
        /// <param name="animationID">����ID</param>
        /// <returns>�����ļ�·��</returns>
        private string GetAnimationFile(DataTable animationTable, string animationID)
        {
            string animationFile = null;

            DataRow[] rows = animationTable.Select(string.Format("AnimationID = '{0}'", animationID));
            if (rows.Length > 0)
            {
                animationFile = rows[0]["AnimationFile"].ToString();
            }

            return animationFile;
        }

        /// <summary>
        /// �����������Ƿ��Ѿ�����
        /// </summary>
        /// <param name="animationTable">������</param>
        /// <returns>���������Ƿ��Ѿ�����</returns>
        private bool CommonStandbyAnimationExist(DataTable animationTable)
        {
            DataRow[] rows = animationTable.Select("AnimationID = 30 OR AnimationID = 31 OR AnimationID = 32 OR AnimationID = 33 OR AnimationID = 34 OR AnimationID = 35");
            bool result = rows.Length > 0;

            return result;
        }

        /// <summary>
        /// �������
        /// </summary>
        /// <param name="cmd">sql�������</param>
        /// <param name="animationID">����ID</param>
        private void ClearAnimation(SqlCommand cmd, string animationID)
        {
            string sqlString = string.Format("DELETE FROM animation_npc WHERE RepresentID = '{0}' AND AnimationID = '{1}'",
                                             npcRepresentID.ToString(), animationID);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();
        }

        /// <summary>
        /// ���¶���
        /// </summary>
        /// <param name="cmd">SqlCommand</param>
        /// <param name="animationTable">�������ݱ�</param>
        /// <param name="animationFile">�����ļ���</param>
        /// <param name="animationID">����ID</param>
        /// <param name="formatString">��ѯ��ʽ�ַ���</param>
        /// <param name="updateExistAnimation">�Ƿ�������ж���</param>
        private void UpdateAnimation(SqlCommand cmd, DataTable animationTable, string animationFile, string animationID, string formatString, bool updateExistAnimation)
        {
            if (!string.IsNullOrEmpty(animationFile))
            {
                if (updateExistAnimation) // ���嶯��
                {
                    ClearAnimation(cmd, animationID);
                }

                if (updateExistAnimation || !AnimationExist(animationTable, animationID)) // ǿ�Ƹ��¶������߶���������
                {
                    // ����Դ�����ѡ��һ�������                        
                    string animationString = GetAnimationString(animationFile);
                    string[] tempArray = animationFile.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);

                    if (tempArray.Length > 1)
                    {
                        int randomIndex = random.Next(tempArray.Length);
                        animationString = GetAnimationString(tempArray[randomIndex]);
                    }

                    string sqlString = string.Format(formatString, animationString, npcRepresentID.ToString(), mapName);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    // ��������
                    UpdateAnimationPlusInfo(cmd, animationID);
                }

                // ���Ʋ�����ݣ�ǿ�Ƹ���                               
                UpdateSlotInfo(cmd, animationID);
            }
            else
            {
                if (AnimationExist(animationTable, animationID))
                {
                    // ���Ʋ������                             
                    UpdateSlotInfo(cmd, animationID);
                }
            }
        }

        /// <summary>
        /// ����npc����
        /// </summary>
        /// <param name="infoTable">��������</param>
        /// <param name="hitEffectResultID">���Ч��ID</param>
        /// <param name="showMessage">�Ƿ���ʾ��ʾ��Ϣ</param>
        /// <param name="updateExistAnimation">�Ƿ�������ж���</param>
        /// <param name="updateCommonStandbyAnimation">�Ƿ������ͨ��������</param>
        /// <return>�Ƿ���³ɹ�</return>
        private bool UpdateAnimation(Hashtable infoTable, string hitEffectResultID, bool showMessage, bool updateExistAnimation, bool updateCommonStandbyAnimation)
        {
            bool success = true;
            string walkAnimation = infoTable["Walk"] as string; // ����
            string runAnimation = infoTable["Run"] as string; // �ܲ�
            string battleStandbyAnimation = infoTable["BattleStandby"] as string; // ս������
            string commonStandbyAnimation = infoTable["CommonStandby"] as string; // ��ͨ����
            string battleUnderAttackAnimation = infoTable["BattleUnderAttack"] as string; // ս������            
            string attackSeriesAnimation = infoTable["AttackSeries"] as string; // ս������
            string attackBuffAnimation = infoTable["AttackBuff"] as string; // ����buff           
            string attackAoeAnimation = infoTable["AttackAoe"] as string; // ����aoe
            string attackBangAnimation = infoTable["AttackBang"] as string; // ս���ػ�
            string attackLongRangeAnimation = infoTable["AttackLongRange"] as string; // Զ�̹���
            string deathAnimation = infoTable["Death"] as string; // ����
            string knockDownStandUpAnimation = infoTable["KnockDownStandUp"] as string; // ����
            string onGroundAnimation = infoTable["OnGround"] as string; // ����
            string onGroundUnderAttackAnimation = infoTable["OnGroundUnderAttack"] as string; // ���䱻��
            string beatBackAnimation = infoTable["BeatBack"] as string; // ������
            string glissadeAnimation = infoTable["Glissade"] as string; // ����
            string voltAnimation = infoTable["Volt"] as string; // ����
            string intonateReleaseAnimation = infoTable["IntonateRelease"] as string; // �����ͷ�
            string vertigoAnimation = infoTable["Vertigo"] as string; // ѣ��
            string wardOffAnimation = infoTable["WardOff"] as string; // ��
            string stopGlissadeAnimation = infoTable["StopGlissade"] as string; // ����ֹͣ
            string boardHeight = infoTable["BoardHeight"] as string; // �����߶�
            string selectionScale = infoTable["SelectionScale"] as string; // ѡ������ű���
            string buffInsertPosition = infoTable["BuffInsertPosition"] as string; // ����buff���λ��
            string aoeInsertPosition = infoTable["AOEInsertPosition"] as string; // ����aoe���λ��
            string bangInsertPosition = infoTable["BangInsertPosition"] as string; // ս���ػ����λ��
            string longRangeInsertPosition = infoTable["LongRangeInsertPosition"] as string; // Զ�̹������λ��
            // ����ʱ���ε� string socketScale = infoTable["SocketScale"] as string; // ������ű�

            // ִ�����ݿ��д����
            string sqlString = string.Format("SELECT AnimationID, AnimationFile FROM animation_npc WHERE RepresentID = '{0}'",
                                             npcRepresentID);
            DataTable animationTable = helper.GetDataTable(conn, sqlString);

            string[] tempArray;
            string animationString;
            int randomIndex;
            SqlTransaction transaction = null;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;              

                // �Զ���ȫ���ܱ�
                sqlString = string.Format("SELECT RepresentID FROM dic_npc_skill_caster_npc WHERE RepresentID = '{0}' AND SkillID = 28 AND SkillLevel = 1",
                                          npcRepresentID.ToString());
                cmd.CommandText = sqlString;
                if (cmd.ExecuteScalar() == null) // ��Ҫ�Զ���ȫ
                {
                    sqlString = string.Format("INSERT INTO dic_npc_skill_caster_npc (RepresentID, SkillID, SkillLevel) VALUES ('{0}', 28, 1)",
                                              npcRepresentID.ToString());
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                    logText.Append("û���ҵ����ܶ������Զ���ȫ��\r\n");
                }

                // ��������¶���
                // ������߶���
                string formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (9, 2, '{0}', '1', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, walkAnimation, "9", formatString, updateExistAnimation);

                // ����ܲ�����
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (16, 3, '{0}', '1', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, runAnimation, "16", formatString, updateExistAnimation);                

                // �����ͨ��������
                if (!string.IsNullOrEmpty(commonStandbyAnimation))
                {
                    if (updateExistAnimation && updateCommonStandbyAnimation) // ���嶯��
                    {
                        ClearAnimation(cmd, "30");
                        ClearAnimation(cmd, "31");
                        ClearAnimation(cmd, "32");
                        ClearAnimation(cmd, "33");
                        ClearAnimation(cmd, "34");
                        ClearAnimation(cmd, "35");
                    }

                    // ѡ�������ҲҪ��������Ӧ�Ĳ����
                    tempArray = commonStandbyAnimation.Split(new char[] { ';' });
                    string[] commonStandbySeriousArray = new string[] { "30", "31", "32", "33", "34", "35" };
                    string[] fieldArray = new string[] { "IdleAniRatio0", "IdleAniRatio1", "IdleAniRatio2", "IdleAniRatio3", "IdleAniRatio4", "IdleAniRatio5" };
                    string[] aniRatioArray = new string[] { "10000", "500", "200", "200", "200", "200" };

                    if ((updateExistAnimation && updateCommonStandbyAnimation) || !CommonStandbyAnimationExist(animationTable)) // �Ѿ�����һ���Ͳ���Ҫ����
                    {
                        for (int i = 0; i < 6; i++)
                        {
                            if (i < tempArray.Length)
                            {                                         
                                // ���¶���
                                animationString = GetAnimationString(tempArray[i]);

                                if (tempArray.Length > 1) // ��ͨ������������ > 1ʱ��ѭ������Ϊ0
                                {
                                    sqlString = string.Format("INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES ('{0}', 1, '{1}', '0', '{2}', '{3}')",
                                                          commonStandbySeriousArray[i], animationString, npcRepresentID.ToString(), mapName);
                                }
                                else // ��ͨ������������ == 1ʱ��ѭ��������1
                                {
                                    sqlString = string.Format("INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES ('{0}', 1, '{1}', '1', '{2}', '{3}')",
                                                          commonStandbySeriousArray[i], animationString, npcRepresentID.ToString(), mapName);
                                }
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();                                                                                                  

                                // ���²��ż���
                                sqlString = string.Format("UPDATE npc SET [{0}] = '{1}' WHERE RepresentID = '{2}'",
                                                          fieldArray[i], aniRatioArray[i], npcRepresentID.ToString());
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();

                                // ��������
                                UpdateAnimationPlusInfo(cmd, commonStandbySeriousArray[i]);
                                
                                // ���Ʋ�����ݣ�ǿ�Ƹ���
                                UpdateSlotInfo(cmd, commonStandbySeriousArray[i]);
                            }
                            else // ǿ��ˢ���ż���
                            {
                                sqlString = string.Format("UPDATE npc SET [{0}] = '0' WHERE RepresentID = '{1}'",
                                                          fieldArray[i], npcRepresentID.ToString());
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();
                            }
                        } 
                    }
                    else
                    {
                        for (int i = 0; i < 6; i++)
                        {
                            if (AnimationExist(animationTable, commonStandbySeriousArray[i]))
                            {
                                // ���Ʋ������
                                UpdateSlotInfo(cmd, commonStandbySeriousArray[i]);
                            }
                            else // ǿ��ˢ���ż���
                            {
                                sqlString = string.Format("UPDATE npc SET [{0}] = '0' WHERE RepresentID = '{1}'",
                                                          fieldArray[i], npcRepresentID.ToString());
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();
                            }
                        }
                    }                                                           
                }

                // ���ս����������
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (47, 4, '{0}', '1', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, battleStandbyAnimation, "47", formatString, updateExistAnimation);

                // ��ӱ�������
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (73, 6, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, battleUnderAttackAnimation, "73", formatString, updateExistAnimation);
                

                // ���ս����������
                if (!string.IsNullOrEmpty(attackSeriesAnimation))
                {
                    if (updateExistAnimation) // ���嶯��
                    {
                        ClearAnimation(cmd, "54");
                        ClearAnimation(cmd, "55");
                        ClearAnimation(cmd, "56");
                        ClearAnimation(cmd, "57");
                    }

                    string[] attackSeriesIDArray = new string[] { "54", "55", "56", "57" };
                    tempArray = attackSeriesAnimation.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                    int maxAttackTime = 0;
                    int currentAttackTime = 0;

                    // ���¶�����
                    if (tempArray.Length >= 4) // ���4�����CastSkillAnimationID0 -- CastSkillAnimationID3�ֶ���
                    {
                        List<string> attackAnimationList = new List<string>();
                        List<string> attackRandomList = new List<string>();

                        attackRandomList.AddRange(tempArray);
                        
                        // ��һ��������������
                        attackAnimationList.Add(tempArray[0]);
                        attackRandomList.Remove(tempArray[0]);                        

                        // �������ȡ������������
                        for (int i = 0; i < 3; i++)
                        {
                            randomIndex = random.Next(attackRandomList.Count);
                            attackAnimationList.Add(attackRandomList[randomIndex]);
                            attackRandomList.RemoveAt(randomIndex);
                        }

                        for (int i = 0; i < 4; i++)
                        {
                            if (updateExistAnimation || !AnimationExist(animationTable, attackSeriesIDArray[i])) // ǿ�Ƹ��¶������߶���������
                            {                                
                                animationString = GetAnimationString(attackAnimationList[i]);
                                sqlString = string.Format("INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES ('{0}', 5, '{1}', '0', '{2}', '{3}')",
                                                          attackSeriesIDArray[i], animationString, npcRepresentID.ToString(), mapName);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();

                                currentAttackTime = GetAnimationTime(animationString);
                                if (currentAttackTime > maxAttackTime) // ȡ�������ʱ��
                                {
                                    maxAttackTime = currentAttackTime;
                                }

                                // ��������
                                UpdateAnimationPlusInfo(cmd, attackSeriesIDArray[i]);
                            }
                            else
                            {
                                animationString = GetAnimationFile(animationTable, attackSeriesIDArray[i]);

                                if (!string.IsNullOrEmpty(animationString))
                                {
                                    currentAttackTime = GetAnimationTime(animationString);
                                    if (currentAttackTime > maxAttackTime) // ȡ�������ʱ��
                                    {
                                        maxAttackTime = currentAttackTime;
                                    }
                                }
                            }
                        }                                               
                    }
                    else // �����ظ�����֤4���ֶζ��ж���
                    {                        
                        for (int i = 0; i < 4; i++)
                        {
                            if (i < tempArray.Length)
                            {
                                if (updateExistAnimation || !AnimationExist(animationTable, attackSeriesIDArray[i])) // ǿ�Ƹ��¶������߶���������
                                {                                    
                                    animationString = GetAnimationString(tempArray[i]);
                                    sqlString = string.Format("INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES ('{0}', 5, '{1}', '0', '{2}', '{3}')",
                                                              attackSeriesIDArray[i], animationString, npcRepresentID.ToString(), mapName);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();

                                    currentAttackTime = GetAnimationTime(animationString);
                                    if (currentAttackTime > maxAttackTime) // ȡ�������ʱ��
                                    {
                                        maxAttackTime = currentAttackTime;
                                    }

                                    // ��������
                                    UpdateAnimationPlusInfo(cmd, attackSeriesIDArray[i]);
                                }
                                else
                                {
                                    animationString = GetAnimationFile(animationTable, attackSeriesIDArray[i]);

                                    if (!string.IsNullOrEmpty(animationString))
                                    {
                                        currentAttackTime = GetAnimationTime(animationString);
                                        if (currentAttackTime > maxAttackTime) // ȡ�������ʱ��
                                        {
                                            maxAttackTime = currentAttackTime;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (updateExistAnimation || !AnimationExist(animationTable, attackSeriesIDArray[i])) // ǿ�Ƹ��¶������߶���������
                                {                                    
                                    animationString = GetAnimationString(tempArray[tempArray.Length - 1]);
                                    sqlString = string.Format("INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES ('{0}', 5, '{1}', '0', '{2}', '{3}')",
                                                              attackSeriesIDArray[i], animationString, npcRepresentID.ToString(), mapName);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();

                                    currentAttackTime = GetAnimationTime(animationString);
                                    if (currentAttackTime > maxAttackTime) // ȡ�������ʱ��
                                    {
                                        maxAttackTime = currentAttackTime;
                                    }

                                    // ��������
                                    UpdateAnimationPlusInfo(cmd, attackSeriesIDArray[i]);
                                }
                                else
                                {
                                    animationString = GetAnimationFile(animationTable, attackSeriesIDArray[i]);

                                    if (!string.IsNullOrEmpty(animationString))
                                    {
                                        currentAttackTime = GetAnimationTime(animationString);
                                        if (currentAttackTime > maxAttackTime) // ȡ�������ʱ��
                                        {
                                            maxAttackTime = currentAttackTime;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // ���¼��ܱ�
                    sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET CastSkillAnimationID0 = 54 WHERE RepresentID = '{0}' AND SkillID = 28 AND SkillLevel = 1", 
                                              npcRepresentID.ToString());
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET CastSkillAnimationID1 = 55 WHERE RepresentID = '{0}' AND SkillID = 28 AND SkillLevel = 1", 
                                              npcRepresentID.ToString());
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET CastSkillAnimationID2 = 56 WHERE RepresentID = '{0}' AND SkillID = 28 AND SkillLevel = 1", 
                                              npcRepresentID.ToString());
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET CastSkillAnimationID3 = 57 WHERE RepresentID = '{0}' AND SkillID = 28 AND SkillLevel = 1", 
                                              npcRepresentID.ToString());
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    // ����AI����
                    UpdateAISkillAniFrame(cmd, maxAttackTime);

                    // ���Ʋ������                               
                    UpdateSlotInfo(cmd, "54");                                       
                    UpdateSlotInfo(cmd, "55");                                       
                    UpdateSlotInfo(cmd, "56");                                             
                    UpdateSlotInfo(cmd, "57");
                }

                // ��Ӽ���buff����
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (64, 5, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, attackBuffAnimation, "64", formatString, updateExistAnimation);

                // ��Ӽ���aoe����
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (65, 5, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, attackAoeAnimation, "65", formatString, updateExistAnimation);

                // ���ս���ػ�����
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (66, 5, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, attackBangAnimation, "66", formatString, updateExistAnimation);

                // ���Զ�̹�������                
                if (!string.IsNullOrEmpty(attackLongRangeAnimation))
                {
                    if (updateExistAnimation) // ���嶯��
                    {
                        ClearAnimation(cmd, "67");
                    }

                    if (updateExistAnimation || !AnimationExist(animationTable, "67")) // ǿ�Ƹ��¶������߶���������
                    {
                        // ����Զ���������ͣ�����ѡ�����������Դ�ļ����ֵ�����������֣� ����������Զ�̶������ٺ����Ͷ�Ӧ��ѡ��һ�����붯����
                        tempArray = attackLongRangeAnimation.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);

                        // ѡ���ʺϵ�Զ�̶���
                        if (string.IsNullOrEmpty(longRangeWeaponType)) // Ĭ����Ϊ��������
                        {
                            longRangeWeaponType = "����";
                        }

                        int tempIndex = 0;
                        for (int i = 0; i < tempArray.Length; i++)
                        {
                            if (tempArray[i].EndsWith(longRangeWeaponType))
                            {
                                tempIndex = i;
                                break;
                            }
                        }
                        
                        animationString = GetAnimationString(tempArray[tempIndex]);
                        sqlString = string.Format("INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (67, 5, '{0}', '0', '{1}', '{2}')",
                                                  animationString, npcRepresentID.ToString(), mapName);
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();

                        // ��������
                        UpdateAnimationPlusInfo(cmd, "67");
                    }  
                  
                    // ���Ʋ�����ݣ�ǿ�Ƹ���                               
                    UpdateSlotInfo(cmd, "67");
                }
                else
                {
                    if (AnimationExist(animationTable, "67"))
                    {
                        // ���Ʋ������                             
                        UpdateSlotInfo(cmd, "67");
                    }
                }

                // �����������
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (1, 8, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, deathAnimation, "1", formatString, updateExistAnimation);

                // ��ӱ�������������
                if (!string.IsNullOrEmpty(knockDownStandUpAnimation))
                {
                    // ����Դ�����ѡ��һ�������
                    tempArray = knockDownStandUpAnimation.Split(new char[]{ ';' }, StringSplitOptions.RemoveEmptyEntries);
                    // �ֽ��������������������

                    if (tempArray.Length > 1)
                    {
                        randomIndex = random.Next(tempArray.Length);
                        tempArray = tempArray[randomIndex].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                    }
                    else
                    {
                        tempArray = tempArray[0].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                    }
                    
                    if (tempArray.Length == 2)
                    {
                        if (updateExistAnimation) // ���嶯��
                        {
                            ClearAnimation(cmd, "2");
                            ClearAnimation(cmd, "5");
                        }

                        // ��ӱ���������
                        if (updateExistAnimation || !AnimationExist(animationTable, "2")) // ǿ�Ƹ��¶������߶���������
                        {                        
                            animationString = GetAnimationString(tempArray[0]);
                            sqlString = string.Format("INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (2, 11, '{0}', '0', '{1}', '{2}')",
                                                      animationString, npcRepresentID.ToString(), mapName);
                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();

                            // ��������
                            UpdateAnimationPlusInfo(cmd, "2");
                        }
                    
                        // ���Ʋ�����ݣ�ǿ�Ƹ���                               
                        UpdateSlotInfo(cmd, "2");

                        // ���������
                        if (updateExistAnimation || !AnimationExist(animationTable, "5")) // ǿ�Ƹ��¶������߶���������
                        {                        
                            animationString = GetAnimationString(tempArray[1]);
                            sqlString = string.Format("INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (5, 12, '{0}', '0', '{1}', '{2}')",
                                                      animationString, npcRepresentID.ToString(), mapName);
                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();

                            // ��������
                            UpdateAnimationPlusInfo(cmd, "5");
                        } 

                        // ���Ʋ�����ݣ�ǿ�Ƹ���                              
                        UpdateSlotInfo(cmd, "5");
                    }
                    else
                    {
                        if (AnimationExist(animationTable, "2"))
                        {
                            // ���Ʋ������                              
                            UpdateSlotInfo(cmd, "2");
                        }

                        if (AnimationExist(animationTable, "5"))
                        {
                            // ���Ʋ������                            
                            UpdateSlotInfo(cmd, "5");
                        }

                        logText.Append(string.Format("���ݸ�ʽ����{0}\r\n", knockDownStandUpAnimation));
                    }
                }
                else
                {
                    if (AnimationExist(animationTable, "2"))
                    {
                        // ���Ʋ������                              
                        UpdateSlotInfo(cmd, "2");
                    }

                    if (AnimationExist(animationTable, "5"))
                    {
                        // ���Ʋ������                            
                        UpdateSlotInfo(cmd, "5");
                    }
                }

                // ��ӵ����ж���
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (291, 4, '{0}', '1', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, onGroundAnimation, "291", formatString, updateExistAnimation);

                // ��ӵ��ر�������
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (292, 6, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, onGroundUnderAttackAnimation, "292", formatString, updateExistAnimation);

                // ��ӱ����˶���
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (3, 11, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, beatBackAnimation, "3", formatString, updateExistAnimation);

                // ��ӻ�������
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (287, 11, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, glissadeAnimation, "287", formatString, updateExistAnimation);

                // ������ܶ���
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (250, 13, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, voltAnimation, "250", formatString, updateExistAnimation);

                // ��Ӿ����ͷŶ���
                if (!string.IsNullOrEmpty(intonateReleaseAnimation))
                {
                    // ����Դ�����ѡ��һ�������
                    tempArray = intonateReleaseAnimation.Split(new char[]{ ';' }, StringSplitOptions.RemoveEmptyEntries);
                    // �ֽ�������������ͷŶ���
                    if (tempArray.Length > 1)
                    {
                        randomIndex = random.Next(tempArray.Length);
                        tempArray = tempArray[randomIndex].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                    }
                    else
                    {
                        tempArray = tempArray[0].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                    }                    

                    if (tempArray.Length == 2)
                    {
                        if (updateExistAnimation) // ���嶯��
                        {
                            ClearAnimation(cmd, "68");
                            ClearAnimation(cmd, "69");
                        }

                        // ��Ӿ�������
                        if (updateExistAnimation || !AnimationExist(animationTable, "69")) // ǿ�Ƹ��¶������߶���������
                        {                        
                            animationString = GetAnimationString(tempArray[0]);
                            sqlString = string.Format("INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (69, 25, '{0}', '1', '{1}', '{2}')",
                                                      animationString, npcRepresentID.ToString(), mapName);
                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();

                            // ��������
                            UpdateAnimationPlusInfo(cmd, "69");
                        }
                    
                        // ���Ʋ�����ݣ�ǿ�Ƹ���                             
                        UpdateSlotInfo(cmd, "69");

                        // ����ͷŶ���
                        if (updateExistAnimation || !AnimationExist(animationTable, "68")) // ǿ�Ƹ��¶������߶���������
                        {
                            animationString = GetAnimationString(tempArray[1]);
                            sqlString = string.Format("INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (68, 5, '{0}', '0', '{1}', '{2}')",
                                                      animationString, npcRepresentID.ToString(), mapName);
                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();

                            // ��������
                            UpdateAnimationPlusInfo(cmd, "68");
                        }              
      
                        // ���Ʋ�����ݣ�ǿ�Ƹ���                               
                        UpdateSlotInfo(cmd, "68");

                        // ���¼��ܱ�
                        sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET PrepareCastSkillAnimationID = 69 WHERE RepresentID = '{0}' AND SkillID = 28 AND SkillLevel = 1", 
                                                  npcRepresentID.ToString());
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                    }
                    else
                    {
                        if (AnimationExist(animationTable, "69"))
                        {
                            // ���Ʋ������                            
                            UpdateSlotInfo(cmd, "69");
                        }

                        if (AnimationExist(animationTable, "68"))
                        {
                            // ���Ʋ������                               
                            UpdateSlotInfo(cmd, "68");
                        }

                        logText.Append(string.Format("���ݸ�ʽ����{0}\r\n", intonateReleaseAnimation));
                    }
                }
                else
                {
                    if (AnimationExist(animationTable, "69"))
                    {
                        // ���Ʋ������                            
                        UpdateSlotInfo(cmd, "69");
                    }

                    if (AnimationExist(animationTable, "68"))
                    {
                        // ���Ʋ������                               
                        UpdateSlotInfo(cmd, "68");
                    }
                }

                // ���ѣ�ζ���
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (80, 15, '{0}', '1', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, vertigoAnimation, "80", formatString, updateExistAnimation);

                // ��Ӹ񵲶���
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (84, 14, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, wardOffAnimation, "84", formatString, updateExistAnimation);

                // ��ӻ���ֹͣ����
                formatString = "INSERT INTO animation_npc (AnimationID, KindID, AnimationFile, AnimationType, RepresentID, Map) VALUES (288, 11, '{0}', '0', '{1}', '{2}')";
                UpdateAnimation(cmd, animationTable, stopGlissadeAnimation, "288", formatString, updateExistAnimation);               

                // ���´��Ч��
                if (!string.IsNullOrEmpty(hitEffectResultID))
                {
                    sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET HitEffectResultID = '{0}' WHERE RepresentID = '{1}' AND SkillID = 28 AND SkillLevel = 1",
                                              hitEffectResultID, npcRepresentID.ToString());
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }

                // ���¹����߶�
                if (!string.IsNullOrEmpty(boardHeight))
                {
                    float boardHeightF = float.Parse(boardHeight);
                    boardHeightF = boardHeightF * modelScale;
                    sqlString = string.Format("UPDATE npc SET Height = '{0}' WHERE RepresentID = '{1}'", boardHeightF.ToString(), npcRepresentID.ToString());
                }
                else
                {
                    sqlString = string.Format("UPDATE npc SET Height = '180' WHERE RepresentID = '{0}'", npcRepresentID.ToString());
                }

                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();                

                // ����ѡ������ű���
                if (!string.IsNullOrEmpty(selectionScale))
                {
                    float selectionScaleF = float.Parse(selectionScale);
                    selectionScaleF = selectionScaleF * modelScale;
                    sqlString = string.Format("UPDATE npc SET SelectionScale = '{0}' WHERE RepresentID = '{1}'", selectionScaleF.ToString(), npcRepresentID.ToString());
                }
                else
                {
                    sqlString = string.Format("UPDATE npc SET SelectionScale = '1' WHERE RepresentID = '{0}'", npcRepresentID.ToString());
                }

                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();               

                // ���²�����ű���
                /* ����ʱ���ε�
                if (!string.IsNullOrEmpty(socketScale))
                {                    
                    sqlString = string.Format("UPDATE npc SET SocketScale = '{0}' WHERE RepresentID = '{1}'", socketScale, npcRepresentID.ToString());
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery(); 
                }
                */

                // ���ԭ�в����Ϣ     
                ClearSlotInfo(cmd);     
                
                // ���µ�ͼ����
                sqlString = string.Format("UPDATE npc SET map = '{0}' WHERE RepresentID = '{1}'", mapName, npcRepresentID);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();

                sqlString = string.Format("UPDATE animation_npc SET Map = '{0}' WHERE RepresentID = '{1}'", mapName, npcRepresentID);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();

                transaction.Commit();

                if (showMessage)
                {
                    MessageBox.Show("NPC�������³ɹ���", "�Զ����¶���", 
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                }

                logText.Append("NPC�������³ɹ���\r\n");
            }
            catch (SqlException ex)
            {
                if (showMessage)
                {
                    MessageBox.Show("���ݿ��дʧ�ܣ�" + ex.Message, "���ݿ��д",
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);                    
                }                

                if (transaction != null)
                {
                    transaction.Rollback();
                }

                logText.Append(string.Format("Error���������ݱ�ʧ�ܣ�������ϢΪ{0}\r\n", ex.Message));
                success = false;
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            return success;
        }

        /// <summary>
        /// ��ղ����Ϣ
        /// </summary>
        /// <param name="cmd">sql����</param>
        private void ClearSlotInfo(SqlCommand cmd)
        {
            string sqlString = null;
     
            if (!string.IsNullOrEmpty(leftHandMesh))
            {
                sqlString = string.Format("UPDATE npc SET LeftHandMesh = '{0}' WHERE RepresentID = '{1}'",
                                          leftHandMesh, npcRepresentID);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }

            if (!string.IsNullOrEmpty(leftHandMaterial))
            {
                sqlString = string.Format("UPDATE npc SET LeftHandMaterial = '{0}' WHERE RepresentID = '{1}'",
                                          leftHandMaterial, npcRepresentID);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }

            if (!string.IsNullOrEmpty(rightHandMesh))
            {
                sqlString = string.Format("UPDATE npc SET RightHandMesh = '{0}' WHERE RepresentID = '{1}'",
                                          rightHandMesh, npcRepresentID);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }

            if (!string.IsNullOrEmpty(rightHandMaterial))
            {
                sqlString = string.Format("UPDATE npc SET RightHandMaterial = '{0}' WHERE RepresentID = '{1}'",
                                          rightHandMaterial, npcRepresentID);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }

            sqlString = string.Format("UPDATE npc SET S_LH_MeshFile = NULL, S_LH_MaterialFile = NULL, S_RH_MeshFile = NULL, S_RH_MaterialFile = NULL WHERE RepresentID = '{0}'",
                                             npcRepresentID);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();
        }

        /// <summary>
        /// ���²����Ϣ
        /// </summary>
        /// <param name="cmd">SqlCommand</param>
        /// <param name="animationID">����ID</param>
        /// <param name="meshField">Mesh�ֶ���</param>
        /// <param name="meshValue">Meshֵ</param>
        /// <param name="materialField">Material�ֶ���</param>
        /// <param name="materialValue">Materialֵ</param>
        private void UpdateSlotInfo(SqlCommand cmd, string animationID, string meshField, string meshValue, string materialField, string materialValue)
        {
            if (!string.IsNullOrEmpty(meshValue))
            {
                string sqlString = string.Format("UPDATE animation_npc SET [{0}] = '{1}' WHERE RepresentID = '{2}' AND AnimationID = '{3}'",
                                                 meshField, meshValue, npcRepresentID.ToString(), animationID);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();

                if (!string.IsNullOrEmpty(materialValue))
                {
                    sqlString = string.Format("UPDATE animation_npc SET [{0}] = '{1}' WHERE RepresentID = '{2}' AND AnimationID = '{3}'",
                                              materialField, materialValue, npcRepresentID.ToString(), animationID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }
            }            
        }

        /// <summary>
        /// ���²����Ϣ
        /// </summary>
        /// <param name="cmd">sql����</param>
        /// <param name="animationID">����ID</param>
        private void UpdateSlotInfo(SqlCommand cmd, string animationID)
        {
            UpdateSlotInfo(cmd, animationID, ""); // �����ֲ��㼰����
            UpdateSlotInfo(cmd, animationID, "S_Face_MeshFile");
            UpdateSlotInfo(cmd, animationID, "S_Hat_MeshFile");
            UpdateSlotInfo(cmd, animationID, "S_LP_MeshFile");
            UpdateSlotInfo(cmd, animationID, "S_LC_MeshFile");                       
            UpdateSlotInfo(cmd, animationID, "S_RP_MeshFile");
            UpdateSlotInfo(cmd, animationID, "S_RP_MeshFile");
            UpdateSlotInfo(cmd, animationID, "S_Long_MeshFile");
            UpdateSlotInfo(cmd, animationID, "S_Spine_MeshFile");
            UpdateSlotInfo(cmd, animationID, "S_Spine2_MeshFile");
        }

        /// <summary>
        /// ���²����Ϣ
        /// </summary>
        /// <param name="cmd">sql����</param>
        /// <param name="animationID">����ID</param>
        /// <param name="slotString">��������ַ���</param>
        private void UpdateSlotInfo(SqlCommand cmd, string animationID, string slotString)
        {
            string sqlString = null;

            switch(slotString)
            {
                case "0": // ԭ�������������κβ����
                    {
                        break;
                    }
                case "1": // ��۲���
                    {
                        UpdateSlotInfo(cmd, animationID, "S_LH_MeshFile", leftHandMesh, "S_LH_MaterialFile", leftHandMaterial);
                        UpdateSlotInfo(cmd, animationID, "S_RH_MeshFile", rightHandMesh, "S_RH_MaterialFile", rightHandMaterial);

                        break;
                    }
                case "": // ��۲���
                    {
                        UpdateSlotInfo(cmd, animationID, "S_LH_MeshFile", leftHandMesh, "S_LH_MaterialFile", leftHandMaterial);
                        UpdateSlotInfo(cmd, animationID, "S_RH_MeshFile", rightHandMesh, "S_RH_MaterialFile", rightHandMaterial);

                        break;
                    }
                case "S_Face_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_Face_MeshFile", faceMesh, "S_Face_MaterialFile", faceMaterial);

                        break;
                    }
                case "S_Hat_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_Hat_MeshFile", hatMesh, "S_Hat_MaterialFile", hatMaterial);

                        break;
                    }
                case "S_LH_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_LH_MeshFile", lhMesh, "S_LH_MaterialFile", lhMaterial);

                        break;
                    }
                case "S_LP_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_LP_MeshFile", lpMesh, "S_LP_MaterialFile", lpMaterial);

                        break;
                    }
                case "S_LC_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_LC_MeshFile", lcMesh, "S_LC_MaterialFile", lcMaterial);

                        break;
                    }
                case "S_RH_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_RH_MeshFile", rhMesh, "S_RH_MaterialFile", rhMaterial);                        

                        break;
                    }
                case "S_RP_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_RP_MeshFile", rpMesh, "S_RP_MaterialFile", rpMaterial);

                        break;
                    }
                case "S_RC_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_RP_MeshFile", rcMesh, "S_RP_MaterialFile", rcMaterial);

                        break;
                    }
                case "S_Long_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_Long_MeshFile", longMesh, "S_Long_MaterialFile", longMaterial);

                        break;
                    }
                case "S_Spine_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_Spine_MeshFile", spineMesh, "S_Spine_MaterialFile", spineMaterial);

                        break;
                    }
                case "S_Spine2_MeshFile":
                    {
                        UpdateSlotInfo(cmd, animationID, "S_Spine2_MeshFile", spine2Mesh, "S_Spine2_MaterialFile", spine2Material);

                        break;
                    }
                default:
                    {
                        logText.Append(string.Format("Error������{0}�������ڷŲ��������Ч {1}\r\n", animationID, slotString));
                        break;
                    }
            }
        }

        /// <summary>
        /// ѡ������
        /// </summary>
        /// <param name="isLeftHand">�Ƿ������ֲ��</param>
        /// <param name="modelFileString">MainModelFile</param>
        /// <returns>ѡ�������·��</returns>
        private string SelectWeapon(bool isLeftHand, string modelFileString)
        {
            string weaponFilePath = null;
            WeaponSelectForm.WeaponType weaponType = WeaponSelectForm.WeaponType.RightHand;

            if (isLeftHand)
            {
                weaponType = WeaponSelectForm.WeaponType.LeftHand;
            }

            string editorID = null;

            if (!string.IsNullOrEmpty(modelFileString))
            {
                string[] tempArray = modelFileString.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
                editorID = tempArray[3];
            }

            WeaponSelectForm wForm = new WeaponSelectForm(conn, boneToActionTable, itemForInsertTable, modelTable, weaponType, editorID, modelFileString);

            if (wForm.ShowDialog() == DialogResult.OK)
            {
                weaponFilePath = wForm.WeaponFilePath;
            }

            return weaponFilePath;
        }

        /// <summary>
        /// ����npc��AI����ʱ��
        /// </summary>
        /// <param name="cmd">sql����</param>
        /// <param name="maxAttackTime">�����ʱ��</param>
        private void UpdateAISkillAniFrame(SqlCommand cmd, int maxAttackTime)
        {
            string sqlString = null;
            DataRow[] rows = npcGlobeTable.Select(string.Format("ID = '{0}'", npcTemplateID));            

            if (rows.Length > 0)
            {
                DataRow row = rows[0];
                string skillID1 = row["SkillID1"].ToString();

                if (skillID1 == "" || skillID1 == "0") // û�����������������һ��
                {
                    sqlString = string.Format("UPDATE NpcTemplate SET SkillID1 = 28, SkillLevel1 = 1, SkillType1 = 1, SkillInterval1 = 1024, SkillRate1 = 1024, SkillAniFrame1 = '{0}', SkillRestFrame1 = 10 WHERE ID = '{1}'",
                                              maxAttackTime.ToString(), npcTemplateID.ToString());
                    skillID1 = "28";
                }                
                else 
                {
                    sqlString = string.Format("UPDATE NpcTemplate SET SkillAniFrame1 = '{0}', SkillRestFrame1 = 10 WHERE ID = '{1}'",
                                              maxAttackTime.ToString(), npcTemplateID.ToString());
                }

                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();

                if (skillID1 == "28") // ���ܼ���ֶ�SkillInterval1����ֵΪSkillAniFrame1 + SkillRestFrame1
                {
                    int skillInterval = maxAttackTime + 10;
                    sqlString = string.Format("UPDATE NpcTemplate SET SkillInterval1 = '{0}' WHERE ID = '{1}'",
                                              skillInterval.ToString(), npcTemplateID.ToString());
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }
            }
        }

        /// <summary>
        /// ����ȱʧmesh��׺�Ĳ���
        /// </summary>
        /// <param name="mapName">������</param>
        public void WeaponMeshFilePatch(string mapName)
        {
            DataRow[] rows = npcGlobeTable.Select(string.Format("MapName = '{0}'", mapName));

            string sqlString = "SELECT RepresentID, LeftHandMesh, RightHandMesh FROM npc";
            DataTable representTable = helper.GetDataTable(conn, sqlString);
            List<string> npcDisplayNameList = new List<string>();

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                SqlCommand cmd = conn.CreateCommand();

                foreach (DataRow row in rows)
                {
                    string representID = row["RepresentID1"].ToString();
                    string npcID = row["ID"].ToString();
                    string npcName = row["Name"].ToString();
                    string npcDisplayName = string.Format("{0} ({1})", npcName, npcID);

                    if (representID != "" && representID != "0")
                    {
                        DataRow[] dataRows = representTable.Select(string.Format("RepresentID = '{0}'", representID));

                        if (dataRows.Length > 0)
                        {
                            DataRow dataRow = dataRows[0];
                            string leftHandMesh = dataRow["LeftHandMesh"].ToString();
                            string rightHandMesh = dataRow["RightHandMesh"].ToString();
                            string newFileName = null;
                            bool updated = false;

                            if (leftHandMesh != "" && !leftHandMesh.EndsWith("Mesh"))
                            {
                                newFileName = string.Format("{0}.Mesh", leftHandMesh);
                                sqlString = string.Format("UPDATE npc SET LeftHandMesh = '{0}' WHERE RepresentID = '{1}'",
                                                          newFileName, representID);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();

                                sqlString = string.Format("UPDATE animation_npc SET S_LH_MeshFile = '{0}' WHERE RepresentID = '{1}'",
                                                          newFileName, representID);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();

                                updated = true;
                            }

                            if (rightHandMesh != "" && !rightHandMesh.EndsWith("Mesh"))
                            {
                                newFileName = string.Format("{0}.Mesh", rightHandMesh);
                                sqlString = string.Format("UPDATE npc SET RightHandMesh = '{0}' WHERE RepresentID = '{1}'",
                                                          newFileName, representID);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();

                                sqlString = string.Format("UPDATE animation_npc SET S_RH_MeshFile = '{0}' WHERE RepresentID = '{1}'",
                                                          newFileName, representID);
                                cmd.CommandText = sqlString;
                                cmd.ExecuteNonQuery();

                                updated = true;
                            }

                            if (updated && !npcDisplayNameList.Contains(npcDisplayName))
                            {
                                npcDisplayNameList.Add(npcDisplayName);
                            }
                        }
                    }
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڴ�����ȱʧ.Mesh��׺����ʱ����sql�쳣��" + ex.Message, "�򲹶�", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            StringBuilder updateInfo = new StringBuilder();
            updateInfo.AppendLine(string.Format("������{0}��npc��", npcDisplayNameList.Count.ToString()));
            updateInfo.AppendLine();
            foreach (string s in npcDisplayNameList)
            {
                updateInfo.AppendLine(s);
            }

            MessageBox.Show(updateInfo.ToString(), "���½��", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        /// <summary>
        /// ��ö����Ĳ���ʱ��
        /// </summary>
        /// <returns>�����Ĳ���ʱ��</returns>
        private int GetAnimationTime(string animationString)
        {
            int animationTime = 0;

            AnimationHelper animationHelper = AnimationHelper.GetAnimationHelper();
            animationHelper.RootPath = rootDir;
            animationTime = animationHelper.GetAniFrameCount(animationString);

            return animationTime;
        }        

        /// <summary>
        /// �Զ����ɶ����ַ���
        /// </summary>
        /// <param name="animationFileName">�����ļ���</param>
        /// <returns>�����ַ���</returns>
        private string GetAnimationString(string animationFileName)
        {
            string animationString = string.Format("{0}{1}", relevatePath, animationFileName);

            if (animationFileName.EndsWith(".ani") || animationFileName.EndsWith(".tani")) // �Ѿ��к�׺��
            {
                // ����Ҫ����
            }
            else
            {
                string filePath = Path.Combine(rootDir, animationString);
                filePath = filePath + ".tani";

                if (File.Exists(filePath))
                {
                    animationString = animationString + ".tani";
                }
                else
                {
                    animationString = animationString + ".ani";
                }
            }            

            return animationString;
        }        

        /// <summary>
        /// �Զ�����npc����
        /// </summary>
        /// <param name="showMessage">�Ƿ���ʾ��ʾ��Ϣ</param>
        /// <param name="updateExistAnimation">�Ƿ�������ж���</param>
        /// <param name="updateCommonStandbyAnimation">�Ƿ������ͨ��������</param>
        /// <return>�Ƿ񴴽��ɹ�</return>
        private bool AutoCreateActions(bool showMessage, bool updateExistAnimation, bool updateCommonStandbyAnimation)
        {
            bool success = true;
            logText.Append(string.Format("��ʼ��ʼ����npc����IDΪ{0}��\r\n", npcRepresentID.ToString()));

            if (npcRepresentID == 0) // ������ID�Ƿ���Ч
            {
                if (showMessage)
                {
                    MessageBox.Show("����Ϊ��ǰNPC������RepresentID��", "���¶���",
                                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                }

                logText.Append("Error������IDΪ�գ�\r\n");
                success = false;
            }
            else
            {
                string modelType = GetModelType(npcModel);

                if (modelType != null) // ������Ч
                {
                    string firstLetter = modelType.Substring(0, 1);
                    switch(firstLetter)
                    {
                        case "F": // player����
                            {
                                relevatePath = string.Format(@"data\source\player\{0}\����\", modelType);

                                break;
                            }
                        case "M": // player����
                            {
                                relevatePath = string.Format(@"data\source\player\{0}\����\", modelType);

                                break;
                            }
                        default: // npc����
                            {
                                relevatePath = string.Format(@"data\source\NPC_source\{0}\����\", modelType);

                                break;
                            }
                    }

                    success = UpdateAnimation(modelType, showMessage, updateExistAnimation, updateCommonStandbyAnimation);
                }
                else
                {
                    if (showMessage)
                    {
                        MessageBox.Show("ģ���ļ�·������Ϊ�գ�", "���¶���",
                                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }

                    logText.Append("Error��ģ��·��Ϊ�գ�\r\n");
                    success = false;
                }
            }

            return success;
        }

        /// <summary>
        /// �������ݱ�
        /// </summary>
        /// <param name="rootPath">�ͻ���Ŀ¼</param>
        /// <param name="fields">������</param>
        /// <param name="values">������</param>
        public void ExportNpcFile(string rootPath, ArrayList fields, string[] values)
        {            
            StringBuilder content = new StringBuilder();
            StringBuilder headLine = new StringBuilder();
            List<int> excludeFiledIndexList = new List<int>();

            for (int i = 0; i < fields.Count; i++)
            {
                string fieldString = fields[i].ToString().TrimEnd(new char[] { '\r', '\n' });

                switch(fieldString)
                {
                    case "LongRangeWeaponType":
                        {                            
                            excludeFiledIndexList.Add(i);
                            break;
                        }
                    case "LeftHandMesh":
                        {
                            excludeFiledIndexList.Add(i);
                            break;
                        }
                    case "LeftHandMaterial":
                        {
                            excludeFiledIndexList.Add(i);
                            break;
                        }
                    case "RightHandMesh":
                        {
                            excludeFiledIndexList.Add(i);
                            break;
                        }
                    case "RightHandMaterial":
                        {
                            excludeFiledIndexList.Add(i);
                            break;
                        }
                    case "EditorID":
                        {
                            excludeFiledIndexList.Add(i);
                            break;
                        }
                    case "Force":
                        {
                            excludeFiledIndexList.Add(i);
                            break;
                        }
                    case "ForceDetail":
                        {
                            excludeFiledIndexList.Add(i);
                            break;
                        }
                    default:
                        {
                            headLine.Append(string.Format("{0}\t", fieldString));
                            break;
                        }
                }     
            }

            content.AppendLine(string.Format("{0}", headLine.ToString().Substring(0, headLine.Length - 1)));

            for (int i = 0; i < values.Length; i++)
            {
                string[] data = values[i].Split(new char[]{'\t'});
                StringBuilder line = new StringBuilder();

                for (int j = 0; j < data.Length; j++)
                {
                    if (!excludeFiledIndexList.Contains(j))
                    {
                        line.Append(string.Format("{0}\t", data[j]));
                    }                    
                }

                content.Append(string.Format("{0}\r\n", line.ToString().Substring(0, line.Length - 1)));
            }
                        
            string fileName = Path.Combine(rootPath, @"represent\npc\npc.txt");
            helper.SaveDataToFile(fileName, content.ToString().TrimEnd(new char[] { '\r', '\n' }));
        }
    }
}
