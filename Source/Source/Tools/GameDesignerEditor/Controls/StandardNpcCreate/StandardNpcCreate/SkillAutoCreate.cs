using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Data;
using System.IO;
using System.Windows.Forms;
using System.Reflection;
using System.Collections;

namespace StandardNpcCreate
{
    class SkillAutoCreate
    {
        private SqlConnection conn; // sql����
        private string npcForce; // npc������
        private string forceDetail; // ����ϸ��
        private string currentSkillID2; // ����ID
        private DataTable npcGlobeTable; // npcȫ�ֱ�
        private DataTable npcRepresentTable; // npc���ֱ�
        private DataTable npcForceSkillTable; // npc���������ܱ�
        private DataTable npcAnimationTable; // npc������
        private DataTable modelTable; // ģ�ͱ�
        private StringBuilder logText = new StringBuilder(); // ��־�ı�
        private string rootDir; // ��Ŀ¼
        private string npcRepresentID; // npc����ID
        private string npcTemplateID; // npcģ��ID        
        private int skillLevel; // ���ܵȼ�
        private Random random = new Random(); // ���������
        private int randomIndex; // ������
        private Helper helper = Helper.GetHelper();

        private string updateModelFile; // ��Ҫ�ⲿ���µ��ֶ�
        private string updateSourceID; // ��Ҫ�ⲿ���µ��ֶ�
        private string updateEditorID; // ��Ҫ�ⲿ���µ��ֶ�
        private string updateNpcForce; // ��Ҫ�ⲿ���µ��ֶ�
        private string updateForceDetail; // ��Ҫ�ⲿ���µ��ֶ�
        private string updateColorChannel; // ��Ҫ�ⲿ���µ��ֶ�
        private string faceMeshFile; // ��Ҫ�ⲿ���µ��ֶ�
        private string faceMaterialFile; // ��Ҫ�ⲿ���µ��ֶ�

        private List<string[]> updateRepresentInfoList = new List<string[]>(); // ��Ҫ�ⲿ���µ���������

        enum UserSection { None, AutoCreate, NotCreate }; // �û�ѡ��ö��
        UserSection userSection = UserSection.None; // ��ǰ�û�ѡ�� 

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="rootDir">��Ŀ¼</param>
        public SkillAutoCreate(SqlConnection conn, string rootDir)
        {
            this.conn = conn;
            this.rootDir = rootDir;
        }        

        /// <summary>
        /// ��Ҫ�ⲿ���µ��ֶ�
        /// </summary>
        public string UpdateColorChannel
        {
            get
            {
                return updateColorChannel;
            }
        }

        /// <summary>
        /// ��Ҫ�ⲿ���µ��ֶ�
        /// </summary>
        public string FaceMaterialFile
        {
            get
            {
                return faceMaterialFile;
            }
        }

        /// <summary>
        /// ��Ҫ�ⲿ���µ��ֶ�
        /// </summary>
        public string FaceMeshFile
        {
            get
            {
                return faceMeshFile;
            }
        }

        /// <summary>
        /// ��Ҫ�ⲿ���µ��ֶ�
        /// </summary>
        public string UpdateModelFile
        {
            get
            {
                return updateModelFile;
            }
        }

        /// <summary>
        /// ��Ҫ�ⲿ���µ��ֶ�
        /// </summary>
        public string UpdateEditorID
        {
            get
            {
                return updateEditorID;
            }
        }

        /// <summary>
        /// ��Ҫ�ⲿ���µ��ֶ�
        /// </summary>
        public string UpdateSourceID
        {
            get
            {
                return updateSourceID;
            }
        }

        /// <summary>
        /// ��Ҫ�ⲿ���µ��ֶ�
        /// </summary>
        public string UpdateNpcForce
        {
            get
            {
                return updateNpcForce;
            }
        }

        /// <summary>
        /// ��Ҫ�ⲿ���µ��ֶ�
        /// </summary>
        public string UpdateForceDetail
        {
            get
            {
                return updateForceDetail;
            }
        }

        /// <summary>
        /// npc����ID
        /// </summary>
        public string NpcRepresentID
        {
            set
            {
                npcRepresentID = value;
            }
        }

        /// <summary>
        /// npcģ��ID
        /// </summary>
        public string NpcTemplateID
        {
            set
            {
                npcTemplateID = value;
            }
        }

        /// <summary>
        /// ����ϸ��
        /// </summary>
        public string ForceDetail
        {
            set
            {
                forceDetail = value;
            }
        }

        /// <summary>
        /// npc������
        /// </summary>
        public string NpcForce
        {
            set
            {
                npcForce = value;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {                       
            string filePath = Path.Combine(Application.StartupPath, @"Design\NPC���������ܱ�.xls");
            string content = helper.GetFileContent(filePath);

            if (content != null)
            {
                npcForceSkillTable = helper.CreateDataTableFromExcelFile(filePath, "����");
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
        /// ���¼������ݱ�
        /// </summary>
        public void ReloadDataTable()
        {
            string sqlString = "SELECT ID, Name, RepresentID1, Level, SkillID1, SkillLevel1, SkillID2, SkillLevel2, SkillID3, SkillLevel3, SkillID4, SkillLevel4, MapName FROM NpcTemplate";
            npcGlobeTable = helper.GetDataTable(conn, sqlString);

            sqlString = "SELECT RepresentID, AnimationID, AnimationFile FROM animation_npc";
            npcAnimationTable = helper.GetDataTable(conn, sqlString);

            sqlString = "SELECT * FROM npc";
            npcRepresentTable = helper.GetDataTable(conn, sqlString);
        }

        /// <summary>
        /// �����Ҫ���µı�����Ϣ
        /// </summary>
        public void ClearRepresentInfo()
        {
            updateRepresentInfoList.Clear();
        }

        /// <summary>
        /// ������Ҫ���µı�����Ϣ
        /// </summary>
        /// <param name="fieldName"></param>
        /// <param name="fieldValue"></param>
        public void AddRepresentInfo(string fieldName, string fieldValue)
        {
            updateRepresentInfoList.Add(new string[] { fieldName, fieldValue });
        }

        /// <summary>
        /// ǿ�Ʊ���NPC�������ID����Ϣ
        /// </summary>
        /// <param name="representID">����ID</param>
        /// <param name="templateID">ģ��ID</param>
        public void UpdateNpcTemplateRepresentInfo(string representID, string templateID)
        {
            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = null;
                SqlCommand cmd = conn.CreateCommand();
                sqlString = string.Format("UPDATE NpcTemplate SET RepresentID1 = '{0}' WHERE ID = '{1}'", representID,
                                          templateID);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                MessageBox.Show("����NPC���RepresentIDʱ����sql�쳣��" + ex.Message, "����NPC����",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }   
        }

        /// <summary>
        /// ˢ�±�����Ϣ
        /// </summary>
        /// <param name="representID">����ID</param>
        /// <param name="mapName">��ͼ��</param>
        /// <param name="npcName">NPC��</param>
        public void RefreshRepresentInfo(string representID)
        {
            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = null;
                SqlCommand cmd = conn.CreateCommand();

                foreach(string[] stringArray in updateRepresentInfoList)
                {
                    sqlString = string.Format("UPDATE npc SET [{0}] = '{1}' WHERE RepresentID = {2}", stringArray[0],
                                              stringArray[1], representID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("����NPC����ʱ����sql�쳣��" + ex.Message, "����NPC����",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }             
        }

        /// <summary>
        /// ��ȡnpc����Ϣ��
        /// </summary>
        /// <param name="templateID">ģ��ID</param>
        /// <returns>npc����Ϣ��</returns>
        public Hashtable GetNpcInfoTable(int templateID)
        {            
            Hashtable infoTable = new Hashtable();

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT Name FROM NpcTemplate WHERE ID = {0}", templateID.ToString());
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                object selectResult = cmd.ExecuteScalar();
                string npcName = selectResult.ToString();

                infoTable["npcName"] = npcName;
            }
            catch (SqlException ex)
            {
                MessageBox.Show("��ȡNPC��Ϣʱ����sql�쳣��" + ex.Message, "��ȡNPC��Ϣ",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }                                                                 

            return infoTable;
        }

        /// <summary>
        /// ��ȡ����ID
        /// </summary>
        /// <param name="skillName">��������</param>
        /// <returns>����ID</returns>
        private string GetAnimationID(string animationName)
        {
            string animationID = "0";

            switch (animationName.ToUpper()) // ��Сдת��һ��
            {
                case "BUFF":
                    {
                        animationID = "64";
                        break;
                    }
                case "��Χ":
                    {
                        animationID = "65";
                        break;
                    }
                case "�ػ�":
                    {
                        animationID = "66";
                        break;
                    }
                case "Զ��":
                    {
                        animationID = "67";
                        break;
                    }
                case "�ͷ�":
                    {
                        animationID = "68";
                        break;
                    }
                case "����":
                    {
                        animationID = "69";
                        break;
                    }
            }

            return animationID;
        }

        /// <summary>
        /// ȥ���ַ����е�����
        /// </summary>
        /// <param name="inString">Դ�ַ���</param>
        /// <returns>ȥ�����ŵ��ַ���</returns>
        private string RemoveBracket(string inString)
        {
            string outString = inString;
            int firstIndex = inString.IndexOf("��");

            if (firstIndex >= 0)
            {
                outString = inString.Substring(0, firstIndex);
            }

            return outString;
        }

        /// <summary>
        /// �Զ�����npc����
        /// </summary>
        /// <returns>�Ƿ񴴽��ɹ�</returns>
        private bool AutoCreateSkills(bool showMessage)
        {
            bool success = true;

            if (!string.IsNullOrEmpty(npcForce) && !string.IsNullOrEmpty(forceDetail))
            {
                // ȥ�������������
                npcForce = RemoveBracket(npcForce);
                forceDetail = RemoveBracket(forceDetail);

                DataRow[] rows = npcForceSkillTable.Select(string.Format("[����] = '{0}' AND [����] = '{1}'", npcForce, forceDetail));
                if (rows.Length > 0)
                {
                    DataRow row = rows[0];
                    DataRow[] npcRows = npcGlobeTable.Select(string.Format("ID = '{0}'", npcTemplateID));

                    if (npcRows.Length > 0)
                    {
                        DataRow npcRow = npcRows[0];
                        string npcLevelString = npcRow["Level"].ToString();
                        int npcLevel = 1;
                        if (npcLevelString != "")
                        {
                            npcLevel = int.Parse(npcLevelString);
                        }                        
                        skillLevel = GetSkillLevel(npcLevel);
                        success = UpdateSkill(row);
                    }                    
                }
                else
                {
                    if (!string.IsNullOrEmpty(currentSkillID2) && currentSkillID2 != "0")
                    {
                        rows = npcForceSkillTable.Select(string.Format("[skillid2] = '{0}'", currentSkillID2));

                        if (rows.Length > 0)
                        {
                            logText.AppendLine("����������ƥ�䲻�ϣ������õڶ�����ƥ��...");

                            DataRow row = rows[0];
                            DataRow[] npcRows = npcGlobeTable.Select(string.Format("ID = '{0}'", npcTemplateID));

                            if (npcRows.Length > 0)
                            {
                                DataRow npcRow = npcRows[0];
                                string npcLevelString = npcRow["Level"].ToString();
                                int npcLevel = 1;
                                if (npcLevelString != "")
                                {
                                    npcLevel = int.Parse(npcLevelString);
                                }
                                skillLevel = GetSkillLevel(npcLevel);
                                success = UpdateSkill(row);
                            } 
                        }
                        else
                        {
                            success = false;
                            logText.AppendLine("û���ҵ�ƥ��������У�");
                        }
                    }
                    else
                    {
                        success = false;
                        logText.AppendLine("û���ҵ�ƥ��������У�");
                    }                    
                }
            }
            else
            {
                success = false;

                if (showMessage)
                {
                    MessageBox.Show("NPC��Force��ForceDetail��Ϣ��������", "���¼���",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    logText.AppendLine("Error:NPC��Force��ForceDetail��Ϣ��������");
                }
            }

            if (success)
            {                
                if (showMessage)
                {
                    MessageBox.Show("���ܸ��³ɹ���", "���¼���",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    logText.AppendLine("���ܸ��³ɹ���");
                }
            }
            else
            {
                if (showMessage)
                {
                    MessageBox.Show(string.Format("���ܸ���ʧ�ܣ�ԭ�����£�\r\n\r\n{0}", logText.ToString()), "���¼���",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    logText.AppendLine("���ܸ���ʧ�ܣ�");
                }
                
            }

            return success;
        }

        /// <summary>
        /// �������¼���
        /// </summary>
        /// <param name="templateID">ģ��ID</param>
        /// <param name="mapName">��ͼ��</param>        
        public void MultiUpdateSkills(int templateID, string mapName)
        {            
            List<string> npcInfoList = new List<string>();
            List<string> updateSuccessNpcList = new List<string>();
            List<string> updateFailNpcList = new List<string>();
            logText = new StringBuilder();
            DataRow[] rows;
            DataRow row;
            string npcID;
            string npcName;
            string skillID2;

            string sqlString = string.Format("SELECT * FROM npc_skill_record");
            DataTable recordTable = helper.GetDataTable(conn, sqlString);

            sqlString = string.Format("SELECT RepresentID, SkillID FROM dic_npc_skill_caster_npc");
            DataTable skillTable = helper.GetDataTable(conn, sqlString);

            if (templateID == 0) // ���¶��npc
            {
                rows = npcGlobeTable.Select(string.Format("MapName = '{0}'", mapName));
            }
            else // ���µ���npc
            {
                rows = npcGlobeTable.Select(string.Format("ID = {0}", templateID.ToString()));
            }

            foreach(DataRow dataRow in rows)
            {
                npcID = dataRow["ID"].ToString();
                npcName = dataRow["Name"].ToString();
                skillID2 = dataRow["SkillID2"].ToString();

                DataRow[] dataRows = recordTable.Select(string.Format("ID = '{0}'", npcID));
                if (dataRows.Length > 0) // �и��¼�¼
                {
                    if (dataRows[0]["IsSpecific"].ToString() == "True") // ����npc����
                    {
                        npcInfoList.Add(string.Format("{0} ({1})    [����]", npcName, npcID));
                    }
                    else
                    {
                        if (skillID2 == "" || skillID2 == "0") // δ��ڶ�����
                        {
                            npcInfoList.Add(string.Format("{0} ({1})    [δ��ڶ�����]", npcName, npcID));
                        }
                        else
                        {
                            npcInfoList.Add(string.Format("{0} ({1})", npcName, npcID));                            
                        }
                    }
                }
                else
                {
                    if (skillID2 == "" || skillID2 == "0") // δ��ڶ�����
                    {                       
                        npcInfoList.Add(string.Format("{0} ({1})    [δ��ڶ�����]", npcName, npcID));
                    }
                    else
                    {
                        npcInfoList.Add(string.Format("{0} ({1})", npcName, npcID));
                    }
                }
            }

            MultiUpdateSkillForm mForm = new MultiUpdateSkillForm(conn, npcInfoList);
            if (mForm.ShowDialog() == DialogResult.OK)
            {
                List<string> selectedNpcInfoList = mForm.SelectedNpcInfoList;
                
                // ���¼���
                foreach(string s in selectedNpcInfoList)
                {
                    bool updateSuccess = false;
                    npcID = GetNpcID(s).ToString();
                    npcName = GetNpcName(s);
                    logText.AppendLine(string.Format("��ʼΪIDΪ{0}��NPC���¼���...", npcID));

                    rows = npcGlobeTable.Select(string.Format("ID = {0}", npcID));
                    if (rows.Length > 0)
                    {
                        row = rows[0];
                        npcTemplateID = npcID;
                        npcRepresentID = row["RepresentID1"].ToString();                        

                        if (npcRepresentID != "" && npcRepresentID != "0")
                        {
                            // ��ȡ�ڶ�����ID
                            DataRow[] dataRows = skillTable.Select(string.Format("RepresentID = '{0}' AND SkillID <> 28", npcRepresentID));
                            if (dataRows.Length > 0) 
                            {
                                currentSkillID2 = dataRows[0]["SkillID"].ToString();
                            }
                            else
                            {
                                currentSkillID2 = null;
                            }

                            string mainModelFile = GetMainModelFile(npcRepresentID);

                            if (!string.IsNullOrEmpty(mainModelFile))
                            {
                                string[] skillInfo = GetSkillInfo(mainModelFile);

                                if (skillInfo != null)
                                {
                                    npcForce = skillInfo[0];
                                    forceDetail = skillInfo[1];
                                }
                                else // ��գ����ڶ�����ID����
                                {
                                    npcForce = "��";
                                    forceDetail = "��";
                                }

                                if (npcRepresentID != "" && npcRepresentID != "0")
                                {
                                    updateSuccess = AutoCreateSkills(false);
                                }
                                else
                                {
                                    logText.AppendLine("Error��RepresentID1Ϊ�գ�");
                                }                                
                            }
                            else
                            {
                                logText.AppendLine("Error��MainModelFile�ֶ�Ϊ�գ�");
                            } 
                        }
                        else
                        {
                            logText.AppendLine("Error����Ч�ı���ID��");
                        }
                    }
                    
                    if (updateSuccess)
                    {
                        updateSuccessNpcList.Add(string.Format("{0} ({1})", npcName, npcID));
                    }
                    else
                    {
                        updateFailNpcList.Add(string.Format("{0} ({1})", npcName, npcID));
                    }
                }

                // ���¼�¼
                try
                {
                    if (conn.State == ConnectionState.Closed)
                    {
                        conn.Open();
                    }

                    SqlCommand cmd = conn.CreateCommand();

                    foreach(string s in updateSuccessNpcList)
                    {
                        npcID = GetNpcID(s).ToString();

                        sqlString = string.Format("SELECT ID FROM npc_skill_record WHERE ID = {0}", npcID);
                        cmd.CommandText = sqlString;

                        if (cmd.ExecuteScalar() == null) // ����
                        {
                            sqlString = string.Format("INSERT INTO npc_skill_record (ID, UpdateSuccess) VALUES ({0}, 1)", npcID);
                        }
                        else // ����
                        {
                            sqlString = string.Format("UPDATE npc_skill_record SET UpdateSuccess = 1 WHERE ID = {0}", npcID);
                        }
                        
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                    }
                }
                catch (SqlException ex)
                {
                    MessageBox.Show("����NPC���ܼ�¼ʱ����sql�쳣��" + ex.Message, "���¼��ܼ�¼",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                finally
                {
                    if (conn.State == ConnectionState.Open)
                    {
                        conn.Close();
                    }
                }

                // ��ʾ���½��
                UpdateResultForm uForm = new UpdateResultForm(updateSuccessNpcList, updateFailNpcList);
                uForm.LogText = logText.ToString();
                uForm.Init();
                uForm.ShowDialog();
            }
        }
       
        /// <summary>
        /// ��ȡ������Ϣ
        /// </summary>
        /// <param name="mainModelFile">ģ���ļ�·��</param>
        /// <returns>������Ϣ</returns>
        private string[] GetSkillInfo(string mainModelFile)
        {
            string[] skillInfo = null;

            DataRow[] rows = modelTable.Select(string.Format("SourceID = '{0}'", mainModelFile));
            if (rows.Length > 0)
            {
                DataRow row = rows[0];
                string force = row["Force"].ToString();
                string forceDetail = row["ForceDetail"].ToString();                

                if (force != "" && forceDetail != "")
                {
                    // ���forceDetail
                    string[] tempArray = forceDetail.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                    forceDetail = tempArray[0];

                    skillInfo = new string[] { force, forceDetail };
                }
            }

            return skillInfo;
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
        /// ��NPC�����ַ�����ȡ��NPC������
        /// </summary>
        /// <param name="npcString">NPC�����ַ���</param>
        /// <returns>NPC������</returns>
        private string GetNpcName(string npcString)
        {
            string npcName = null;

            int index = npcString.LastIndexOf("(");

            if (index > 0)
            {
                npcName = npcString.Substring(0, index - 1);
            }

            return npcName;
        }

        /// <summary>
        /// ���npc�ļ����Ƿ��Ѿ������¹�
        /// </summary>
        /// <param name="npcID">npc��ID</param>
        /// <param name="recordTable"></param>
        /// <returns></returns>
        private bool CheckNpcUpdated(string npcID, DataTable recordTable)
        {
            bool updated = false;

            DataRow[] rows = recordTable.Select(string.Format("ID = '{0}'", npcID.ToString()));
            if (rows.Length > 0)
            {
                if (rows[0]["UpdateSuccess"].ToString() == "True")
                {
                    updated = true;
                }
            }

            return updated;
        }    
        
        /// <summary>
        /// ���npc�ļ���
        /// </summary>
        /// <param name="cmd">sql����</param>
        /// <param name="skillID">����ID</param>
        /// <param name="skillLevel">���ܵȼ�</param>
        private void ClearNpcSkill(SqlCommand cmd, string skillID, string skillLevel)
        {
            string sqlString = string.Format("DELETE FROM dic_npc_skill_caster_npc WHERE RepresentID = '{0}' AND SkillID = '{1}' AND SkillLevel = '{2}'",
                                             npcRepresentID, skillID, skillLevel);
            cmd.CommandText = sqlString;
            cmd.ExecuteNonQuery();
        }

        /// <summary>
        /// ����npc����
        /// </summary>
        /// <param name="dataRow">������</param>
        /// <returns>�Ƿ���³ɹ�</returns>
        private bool UpdateSkill(DataRow dataRow)
        {
            bool success = false;

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

                // �������ǰ������м��ܣ�����28�ż���
                // string sqlString = string.Format("DELETE FROM dic_npc_skill_caster_npc WHERE RepresentID = '{0}' AND SkillID <> 28", npcRepresentID.ToString());
                // cmd.CommandText = sqlString;
                // cmd.ExecuteNonQuery();

                // ���NPC�����м���ID
                // sqlString = string.Format("UPDATE NpcTemplate SET SkillID2 = 0, SkillID3 = 0, SkillID4 = 0 WHERE ID = '{0}'", npcTemplateID);
                // cmd.CommandText = sqlString;
                // cmd.ExecuteNonQuery();                

                // ���¼���2
                string animationName = dataRow["��������2"].ToString();
                string skillID = dataRow["skillid2"].ToString();
                string skillType = dataRow["skilltype2"].ToString();
                string skillInterval = dataRow["skillinterval2"].ToString();
                string skillRate = dataRow["skillrate2"].ToString();
                string hitEffectID = dataRow["HitEffectResultID2"].ToString();
                string animationFile = null;                
                int animationTime = 0;
                string animationID = GetAnimationID(animationName);                
                DataRow[] rows = null;
                DataRow row = null;
                string sqlString = null;

                rows = npcGlobeTable.Select(string.Format("ID = '{0}'", npcTemplateID));
                row = rows[0];
                string npcSkillID2 = row["SkillID2"].ToString();
                string npcSkillLevel2 = row["SkillLevel2"].ToString();
                string npcSkillID3 = row["SkillID3"].ToString();
                string npcSkillLevel3 = row["SkillLevel3"].ToString();
                string npcSkillID4 = row["SkillID4"].ToString();
                string npcSkillLevel4 = row["SkillLevel4"].ToString();

                if (animationID != "0")
                {
                    // �Զ����npc�ĵ�ǰ��Ӧ����
                    if (npcSkillID2 != "" && npcSkillID2 != "0")
                    {
                        ClearNpcSkill(cmd, npcSkillID2, npcSkillLevel2.ToString());
                    }                    

                    // �Զ���ȫ���ܱ�
                    sqlString = string.Format("SELECT RepresentID FROM dic_npc_skill_caster_npc WHERE RepresentID = '{0}' AND SkillID = '{1}' AND SkillLevel = '{2}'",
                                              npcRepresentID.ToString(), skillID, skillLevel.ToString());
                    cmd.CommandText = sqlString;

                    if (cmd.ExecuteScalar() == null) // ��Ҫ�Զ���ȫ
                    {
                        sqlString = string.Format("INSERT INTO dic_npc_skill_caster_npc (RepresentID, SkillID, SkillLevel) VALUES ('{0}', '{1}', '{2}')",
                                                  npcRepresentID.ToString(), skillID, skillLevel.ToString());
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                        logText.AppendLine(string.Format("���IDΪ{0}�ļ��ܶ���", skillID));
                    }

                    if (animationName == "����") // ��������Ϊ����ʱ����69����PrepareCastSkillAnimationID�ֶΣ���68������CastSkillAnimationID0�ֶΡ�
                    {
                        sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET CastSkillAnimationID0 = '68', PrepareCastSkillAnimationID = '69' WHERE RepresentID = '{0}' AND SkillID = '{1}' AND SkillLevel = '{2}'",
                                                  npcRepresentID.ToString(), skillID, skillLevel.ToString());
                    }
                    else
                    {
                        sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET CastSkillAnimationID0 = {0} WHERE RepresentID = '{1}' AND SkillID = '{2}' AND SkillLevel = '{3}'",
                                                  animationID, npcRepresentID.ToString(), skillID, skillLevel.ToString());                        
                    }

                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    if (hitEffectID != "") // ���ϴ����Ч
                    {
                        sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET HitEffectResultID = '{0}' WHERE RepresentID = '{1}' AND SkillID = '{2}' AND SkillLevel = '{3}'",
                                                  hitEffectID, npcRepresentID.ToString(), skillID, skillLevel.ToString());
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                    }

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillID2 = '{0}' WHERE ID = '{1}'", skillID, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillType2 = '{0}' WHERE ID = '{1}'", skillType, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillInterval2 = '{0}' WHERE ID = '{1}'", skillInterval, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillRate2 = '{0}' WHERE ID = '{1}'", skillRate, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillLevel2 = '{0}' WHERE ID = '{1}'", skillLevel.ToString(), npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillRestFrame2 = 8 WHERE ID = '{0}'", npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    if (animationName == "����")
                    {
                        rows = npcAnimationTable.Select(string.Format("RepresentID = '{0}' AND AnimationID = 69", npcRepresentID));
                        if (rows.Length > 0)
                        {
                            row = rows[0];
                            animationFile = row["AnimationFile"].ToString();
                            if (!string.IsNullOrEmpty(animationFile))
                            {
                                animationTime = GetAnimationTime(animationFile);

                                if (animationTime > 0)
                                {
                                    sqlString = string.Format("UPDATE NpcTemplate SET SkillAniFrame2 = '{0}' WHERE ID = '{1}'", animationTime.ToString(), npcTemplateID);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();
                                }
                                else
                                {
                                    logText.AppendLine(string.Format("Error���޷���ȡIDΪ{0}�Ķ����Ĳ���ʱ�䣡", animationID));
                                }
                            }
                            else
                            {
                                logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ������ļ���", animationID));
                            }
                        }
                        else
                        {
                            logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ�����", animationID));
                        }
                    }
                    else
                    {
                        rows = npcAnimationTable.Select(string.Format("RepresentID = '{0}' AND AnimationID = '{1}'", npcRepresentID, animationID));
                        if (rows.Length > 0)
                        {
                            row = rows[0];
                            animationFile = row["AnimationFile"].ToString();
                            if (!string.IsNullOrEmpty(animationFile))
                            {
                                animationTime = GetAnimationTime(animationFile);

                                if (animationTime > 0)
                                {
                                    sqlString = string.Format("UPDATE NpcTemplate SET SkillAniFrame2 = '{0}' WHERE ID = '{1}'", animationTime.ToString(), npcTemplateID);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();
                                }
                                else
                                {
                                    logText.AppendLine(string.Format("Error���޷���ȡIDΪ{0}�Ķ����Ĳ���ʱ�䣡", animationID));
                                }
                            }
                            else
                            {
                                logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ������ļ���", animationID));
                            }
                        }
                        else
                        {
                            logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ�����", animationID));
                        }
                    }
                }
                else
                {
                    logText.AppendLine("��������2Ϊ��...");
                }

                // ���¼���3
                animationName = dataRow["��������3"].ToString();
                skillID = dataRow["skillid3"].ToString();
                skillType = dataRow["skilltype3"].ToString();
                skillInterval = dataRow["skillinterval3"].ToString();
                skillRate = dataRow["skillrate3"].ToString();
                hitEffectID = dataRow["HitEffectResultID3"].ToString();
                animationID = GetAnimationID(animationName);

                if (animationID != "0")
                {
                    // �Զ����npc�ĵ�ǰ��Ӧ����
                    if (npcSkillID3 != "" && npcSkillID3 != "0")
                    {
                        ClearNpcSkill(cmd, npcSkillID3, npcSkillLevel3.ToString());
                    } 

                    // �Զ���ȫ���ܱ�
                    sqlString = string.Format("SELECT RepresentID FROM dic_npc_skill_caster_npc WHERE RepresentID = '{0}' AND SkillID = '{1}' AND SkillLevel = '{2}'",
                                              npcRepresentID.ToString(), skillID, skillLevel.ToString());
                    cmd.CommandText = sqlString;

                    if (cmd.ExecuteScalar() == null) // ��Ҫ�Զ���ȫ
                    {
                        sqlString = string.Format("INSERT INTO dic_npc_skill_caster_npc (RepresentID, SkillID, SkillLevel) VALUES ('{0}', '{1}', '{2}')",
                                                  npcRepresentID.ToString(), skillID, skillLevel.ToString());
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                        logText.AppendLine(string.Format("���IDΪ{0}�ļ��ܶ���", skillID));
                    }

                    if (animationName == "����") // ��������Ϊ����ʱ����69����PrepareCastSkillAnimationID�ֶΣ���68������CastSkillAnimationID0�ֶΡ�
                    {
                        sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET CastSkillAnimationID0 = '68', PrepareCastSkillAnimationID = '69' WHERE RepresentID = '{0}' AND SkillID = '{1}' AND SkillLevel = '{2}'",
                                                  npcRepresentID.ToString(), skillID, skillLevel.ToString());
                    }
                    else
                    {
                        sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET CastSkillAnimationID0 = {0} WHERE RepresentID = '{1}' AND SkillID = '{2}' AND SkillLevel = '{3}'",
                                                  animationID, npcRepresentID.ToString(), skillID, skillLevel.ToString());
                    }                    

                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    if (hitEffectID != "") // ���ϴ����Ч
                    {
                        sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET HitEffectResultID = '{0}' WHERE RepresentID = '{1}' AND SkillID = '{2}' AND SkillLevel = '{3}'",
                                                  hitEffectID, npcRepresentID.ToString(), skillID, skillLevel.ToString());
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                    }

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillID3 = '{0}' WHERE ID = '{1}'", skillID, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillType3 = '{0}' WHERE ID = '{1}'", skillType, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillInterval3 = '{0}' WHERE ID = '{1}'", skillInterval, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillRate3 = '{0}' WHERE ID = '{1}'", skillRate, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillLevel3 = '{0}' WHERE ID = '{1}'", skillLevel.ToString(), npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillRestFrame3 = 8 WHERE ID = '{0}'", npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    if (animationName == "����")
                    {
                        rows = npcAnimationTable.Select(string.Format("RepresentID = '{0}' AND AnimationID = 69", npcRepresentID));
                        if (rows.Length > 0)
                        {
                            row = rows[0];
                            animationFile = row["AnimationFile"].ToString();
                            if (!string.IsNullOrEmpty(animationFile))
                            {
                                animationTime = GetAnimationTime(animationFile);

                                if (animationTime > 0)
                                {
                                    sqlString = string.Format("UPDATE NpcTemplate SET SkillAniFrame3 = '{0}' WHERE ID = '{1}'", animationTime.ToString(), npcTemplateID);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();
                                }
                                else
                                {
                                    logText.AppendLine(string.Format("Error���޷���ȡIDΪ{0}�Ķ����Ĳ���ʱ�䣡", animationID));
                                }
                            }
                            else
                            {
                                logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ������ļ���", animationID));
                            }
                        }
                        else
                        {
                            logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ�����", animationID));
                        }
                    }
                    else
                    {
                        rows = npcAnimationTable.Select(string.Format("RepresentID = '{0}' AND AnimationID = '{1}'", npcRepresentID, animationID));
                        if (rows.Length > 0)
                        {
                            row = rows[0];
                            animationFile = row["AnimationFile"].ToString();
                            if (!string.IsNullOrEmpty(animationFile))
                            {
                                animationTime = GetAnimationTime(animationFile);

                                if (animationTime > 0)
                                {
                                    sqlString = string.Format("UPDATE NpcTemplate SET SkillAniFrame3 = '{0}' WHERE ID = '{1}'", animationTime.ToString(), npcTemplateID);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();
                                }
                                else
                                {
                                    logText.AppendLine(string.Format("Error���޷���ȡIDΪ{0}�Ķ����Ĳ���ʱ�䣡", animationID));
                                }
                            }
                            else
                            {
                                logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ������ļ���", animationID));
                            }
                        }
                        else
                        {
                            logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ�����", animationID));
                        }
                    }
                }
                else
                {
                    logText.AppendLine("��������3Ϊ��...");
                }

                // ���¼���4
                animationName = dataRow["��������4"].ToString();
                skillID = dataRow["skillid4"].ToString();
                skillType = dataRow["skilltype4"].ToString();
                skillInterval = dataRow["skillinterval4"].ToString();
                skillRate = dataRow["skillrate4"].ToString();
                hitEffectID = dataRow["HitEffectResultID4"].ToString();
                animationID = GetAnimationID(animationName);

                if (animationID != "0")
                {
                    // �Զ����npc�ĵ�ǰ��Ӧ����
                    if (npcSkillID4 != "" && npcSkillID4 != "0")
                    {
                        ClearNpcSkill(cmd, npcSkillID4, npcSkillLevel4.ToString());
                    } 

                    // �Զ���ȫ���ܱ�
                    sqlString = string.Format("SELECT RepresentID FROM dic_npc_skill_caster_npc WHERE RepresentID = '{0}' AND SkillID = '{1}' AND SkillLevel = '{2}'",
                                              npcRepresentID.ToString(), skillID, skillLevel.ToString());
                    cmd.CommandText = sqlString;

                    if (cmd.ExecuteScalar() == null) // ��Ҫ�Զ���ȫ
                    {
                        sqlString = string.Format("INSERT INTO dic_npc_skill_caster_npc (RepresentID, SkillID, SkillLevel) VALUES ('{0}', '{1}', '{2}')",
                                                  npcRepresentID.ToString(), skillID, skillLevel.ToString());
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                        logText.AppendLine(string.Format("���IDΪ{0}�ļ��ܶ���", skillID));
                    }

                    if (animationName == "����") // ��������Ϊ����ʱ����69����PrepareCastSkillAnimationID�ֶΣ���68������CastSkillAnimationID0�ֶΡ�
                    {
                        sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET CastSkillAnimationID0 = '68', PrepareCastSkillAnimationID = '69' WHERE RepresentID = '{0}' AND SkillID = '{1}' AND SkillLevel = '{2}'",
                                                  npcRepresentID.ToString(), skillID, skillLevel.ToString());
                    }
                    else
                    {
                        sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET CastSkillAnimationID0 = {0} WHERE RepresentID = '{1}' AND SkillID = '{2}' AND SkillLevel = '{3}'",
                                                  animationID, npcRepresentID.ToString(), skillID, skillLevel.ToString());
                    }

                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    if (hitEffectID != "") // ���ϴ����Ч
                    {
                        sqlString = string.Format("UPDATE dic_npc_skill_caster_npc SET HitEffectResultID = '{0}' WHERE RepresentID = '{1}' AND SkillID = '{2}' AND SkillLevel = '{3}'",
                                                  hitEffectID, npcRepresentID.ToString(), skillID, skillLevel.ToString());
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                    }

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillID4 = '{0}' WHERE ID = '{1}'", skillID, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillType4 = '{0}' WHERE ID = '{1}'", skillType, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillInterval4 = '{0}' WHERE ID = '{1}'", skillInterval, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillRate4 = '{0}' WHERE ID = '{1}'", skillRate, npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillLevel4 = '{0}' WHERE ID = '{1}'", skillLevel.ToString(), npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    sqlString = string.Format("UPDATE NpcTemplate SET SkillRestFrame4 = 8 WHERE ID = '{0}'", npcTemplateID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();

                    if (animationName == "����")
                    {
                        rows = npcAnimationTable.Select(string.Format("RepresentID = '{0}' AND AnimationID = 69", npcRepresentID));
                        if (rows.Length > 0)
                        {
                            row = rows[0];
                            animationFile = row["AnimationFile"].ToString();
                            if (!string.IsNullOrEmpty(animationFile))
                            {
                                animationTime = GetAnimationTime(animationFile);

                                if (animationTime > 0)
                                {
                                    sqlString = string.Format("UPDATE NpcTemplate SET SkillAniFrame4 = '{0}' WHERE ID = '{1}'", animationTime.ToString(), npcTemplateID);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();
                                }
                                else
                                {
                                    logText.AppendLine(string.Format("Error���޷���ȡIDΪ{0}�Ķ����Ĳ���ʱ�䣡", animationID));
                                }
                            }
                            else
                            {
                                logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ������ļ���", animationID));
                            }
                        }
                        else
                        {
                            logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ�����", animationID));
                        }
                    }
                    else
                    {
                        rows = npcAnimationTable.Select(string.Format("RepresentID = '{0}' AND AnimationID = '{1}'", npcRepresentID, animationID));
                        if (rows.Length > 0)
                        {
                            row = rows[0];
                            animationFile = row["AnimationFile"].ToString();
                            if (!string.IsNullOrEmpty(animationFile))
                            {
                                animationTime = GetAnimationTime(animationFile);

                                if (animationTime > 0)
                                {
                                    sqlString = string.Format("UPDATE NpcTemplate SET SkillAniFrame4 = '{0}' WHERE ID = '{1}'", animationTime.ToString(), npcTemplateID);
                                    cmd.CommandText = sqlString;
                                    cmd.ExecuteNonQuery();
                                }
                                else
                                {
                                    logText.AppendLine(string.Format("Error���޷���ȡIDΪ{0}�Ķ����Ĳ���ʱ�䣡", animationID));
                                }
                            }
                            else
                            {
                                logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ������ļ���", animationID));
                            }
                        }
                        else
                        {
                            logText.AppendLine(string.Format("Error��û���ҵ�IDΪ{0}�Ķ�����", animationID));
                        }
                    }
                }
                else
                {
                    logText.AppendLine("��������4Ϊ��...");
                }

                transaction.Commit();
                success = true;
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڸ���NPC����ʱ����sql�쳣��" + ex.Message, "���¼���",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);

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

            return success;
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
        /// ��ȡMainModelFile�ֶ�ֵ
        /// </summary>
        /// <param name="representID">����ID</param>
        /// <returns>MainModelFile�ֶ�ֵ</returns>
        private string GetMainModelFile(string representID)
        {
            string mainModelFile = "";

            DataRow[] rows = npcRepresentTable.Select(string.Format("RepresentID = {0}", representID));
            if (rows.Length > 0)
            {
                mainModelFile = rows[0]["MainModelFile"].ToString();
            }

            return mainModelFile;
        }

        /// <summary>
        /// ��ȡEditorID�ֶ�ֵ
        /// </summary>
        /// <param name="representID">����ID</param>
        /// <returns>EditorID�ֶ�ֵ</returns>
        private string GetEditorID(string representID)
        {
            string editorID = "";

            DataRow[] rows = npcRepresentTable.Select(string.Format("RepresentID = {0}", representID));
            if (rows.Length > 0)
            {
                editorID = rows[0]["EditorID"].ToString();
            }

            return editorID;
        }

        /// <summary>
        /// ��ȡ���ܵȼ�
        /// </summary>
        /// <param name="npcLevel">npc�ȼ�</param>
        /// <returns>���ܵȼ�</returns>
        private int GetSkillLevel(int npcLevel)
        {
            int skillLevel = (int)((npcLevel + 5) / 5);

            return skillLevel;
        }

        /// <summary>
        /// ��SourceIDƴ������Mesh�����·��
        /// </summary>
        /// <param name="sourceID">SourceID</param>
        /// <param name="fieldValue">�ֶ�ֵ</param>
        /// <returns>����Mesh�����·��</returns>
        private string GetFaceMeshFile(string sourceID, string fieldValue)
        {
            StringBuilder meshFile = new StringBuilder();
            string[] tempArray = sourceID.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);

            if (sourceID.StartsWith(@"data\source\NPC_source")) // npc��ģ��
            {
                for (int i = 0; i < tempArray.Length - 1; i++)
                {
                    meshFile.Append(string.Format("{0}\\", tempArray[i]));
                }
                meshFile.Append(string.Format("{0}.mesh", fieldValue));
            }
            else // player��ģ��
            {
                for (int i = 0; i < tempArray.Length - 2; i++)
                {
                    meshFile.Append(string.Format("{0}\\", tempArray[i]));
                }
                meshFile.Append(string.Format("{0}.mesh", fieldValue));
            }

            return meshFile.ToString();
        }

        /// <summary>
        /// ��SourceIDƴ������Material�����·��
        /// </summary>
        /// <param name="sourceID">SourceID</param>
        /// <param name="fieldValue">�ֶ�ֵ</param>
        /// <returns>����Material�����·��</returns>
        private string GetFaceMaterialFile(string sourceID, string fieldValue)
        {
            StringBuilder materialFile = new StringBuilder();
            string[] tempArray = sourceID.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);

            if (sourceID.StartsWith(@"data\source\NPC_source")) // npc��ģ��
            {
                for (int i = 0; i < tempArray.Length - 1; i++)
                {
                    materialFile.Append(string.Format("{0}\\", tempArray[i]));
                }
                materialFile.Append(string.Format("{0}.mtl", fieldValue));
            }
            else // player��ģ��
            {
                for (int i = 0; i < tempArray.Length - 2; i++)
                {
                    materialFile.Append(string.Format("{0}\\", tempArray[i]));
                }
                materialFile.Append(string.Format("{0}.mtl", fieldValue));
            }            

            return materialFile.ToString();
        }

        /// <summary>
        /// ���²����Ϣ
        /// </summary>
        /// <param name="dataRow">������</param>
        /// <returns>�Ƿ���³ɹ�</returns>
        private bool UpdateSlot(DataRow dataRow)
        {
            bool editSuccess = false;
            SqlTransaction transaction = null;
            faceMeshFile = null;
            faceMaterialFile = null;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;
                string sqlString = null;

                string sourceID = dataRow["SourceID"].ToString();
                string face = dataRow["Face"].ToString();

                if (helper.CheckStringValid(face) && helper.CheckStringValid(sourceID))
                {
                    string[] tempArray = face.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                    if (tempArray.Length > 1)
                    {                        
                        randomIndex = random.Next(tempArray.Length);
                        faceMeshFile = GetFaceMeshFile(sourceID, tempArray[randomIndex]);                       
                    }
                    else
                    {
                        faceMeshFile = GetFaceMeshFile(sourceID, tempArray[0]);
                    }

                    sqlString = string.Format("UPDATE npc SET S_Face_MeshFile = '{0}' WHERE RepresentID = '{1}'", faceMeshFile, npcRepresentID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }
                else
                {
                    sqlString = string.Format("UPDATE npc SET S_Face_MeshFile = NULL WHERE RepresentID = '{0}'", npcRepresentID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }

                string faceMaterial = dataRow["FaceTexture"].ToString();

                if (helper.CheckStringValid(faceMaterial))
                {
                    faceMaterialFile = GetFaceMaterialFile(sourceID, faceMaterial);
                    sqlString = string.Format("UPDATE npc SET S_Face_MaterialFile = '{0}' WHERE RepresentID = '{1}'", faceMaterialFile, npcRepresentID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }
                else
                {
                    sqlString = string.Format("UPDATE npc SET S_Face_MaterialFile = NULL WHERE RepresentID = '{0}'", npcRepresentID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }

                string specialTrinket1 = dataRow["SpecialTrinket1"].ToString();
                string insertPosition1 = dataRow["InsertPosition1"].ToString();

                if (helper.CheckStringValid(insertPosition1))
                {
                    if (helper.CheckStringValid(specialTrinket1))
                    {
                        sqlString = string.Format("UPDATE npc SET {0}_MeshFile = '{1}' WHERE RepresentID = '{2}'", insertPosition1, specialTrinket1, npcRepresentID);
                    }
                    else
                    {
                        sqlString = string.Format("UPDATE npc SET {0}_MeshFile = NULL WHERE RepresentID = '{1}'", insertPosition1, npcRepresentID);                        
                    }

                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }                

                string specialTrinket2 = dataRow["SpecialTrinket2"].ToString();
                string insertPosition2 = dataRow["InsertPosition2"].ToString();

                if (helper.CheckStringValid(insertPosition2))
                {
                    if (helper.CheckStringValid(specialTrinket2))
                    {
                        sqlString = string.Format("UPDATE npc SET {0}_MeshFile = '{1}' WHERE RepresentID = '{2}'", insertPosition2, specialTrinket2, npcRepresentID);
                    }
                    else
                    {
                        sqlString = string.Format("UPDATE npc SET {0}_MeshFile = NULL WHERE RepresentID = '{1}'", insertPosition2, npcRepresentID);
                    }

                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }                

                // ����ģ������ֵ
                string modelScale = dataRow["ModelScale"].ToString();

                if (helper.CheckStringValid(modelScale))
                {
                    sqlString = string.Format("UPDATE npc SET ModelScale = '{0}' WHERE RepresentID = '{1}'", modelScale, npcRepresentID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }

                transaction.Commit();
                editSuccess = true;
            }
            catch (SqlException ex)
            {
                MessageBox.Show("���²������ʱ����sql�쳣��" + ex.Message, "���²������",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);

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

            return editSuccess;
        }

        /// <summary>
        /// �༭npc���
        /// </summary>
        /// <returns>�Ƿ���ɹ�</returns>
        public bool EditSeriesID()
        {
            bool editSuccess = false;
            SourceIDSelectForm sForm = new SourceIDSelectForm(conn, modelTable, npcForceSkillTable);

            if (sForm.ShowDialog() == DialogResult.OK)
            {
                updateEditorID = sForm.EditorID;
                updateNpcForce = sForm.NpcForce;
                updateForceDetail = sForm.ForceDetail;
                updateModelFile = sForm.SourceID;
                updateColorChannel = sForm.ColorChannel;
                editSuccess = true;
            }

            return editSuccess;
        }

        /// <summary>
        /// �༭npc���
        /// </summary>
        /// <param name="seriesID">npc����ID</param>
        /// <returns>�Ƿ���ɹ�</returns>
        public bool EditSeriesIDWithoutMessage(string seriesID)
        {
            bool editSuccess = false;

            if (!string.IsNullOrEmpty(seriesID))
            {
                DataRow[] rows = modelTable.Select(string.Format("EditorID = '{0}'", seriesID));
                if (rows.Length > 0)
                {
                    DataRow row = rows[0];
                    editSuccess = UpdateSlot(row);
                }
            }

            return editSuccess;
        }

        /// <summary>
        /// �༭npc���
        /// </summary>
        /// <param name="seriesID">npc����ID</param>
        /// <returns>�Ƿ���ɹ�</returns>
        public bool EditSeriesID(string seriesID)
        {
            bool editSuccess = false;
            DialogResult result = MessageBox.Show("�Ƿ����NPC������ۼ��Ҽ���Ϣ��", "���²��", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);

            if (result == DialogResult.OK)
            {                
                logText = new StringBuilder();

                if (!string.IsNullOrEmpty(seriesID))
                {
                    DataRow[] rows = modelTable.Select(string.Format("EditorID = '{0}'", seriesID));
                    if (rows.Length > 0)
                    {
                        DataRow row = rows[0];
                        editSuccess = UpdateSlot(row);
                    }
                    else
                    {
                        logText.AppendLine("Error��û���ҵ�ƥ������ݣ�");
                    }
                }
                else
                {
                    logText.AppendLine("Error��NPC��Ų���Ϊ�գ�");
                }

                if (editSuccess)
                {
                    MessageBox.Show("��۸��³ɹ���", "���²��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show(string.Format("��۸���ʧ�ܣ�ԭ�����£�\r\n\r\n{0}", logText.ToString()), "���²��",
                                                  MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                editSuccess = false;
            }            

            return editSuccess;
        }

        /// <summary>
        /// �༭npc���ִ���
        /// </summary>
        /// <returns>�Ƿ���ɹ�</returns>
        public bool EditWithoutMessage()
        {
            bool editSuccess = false;
            logText = new StringBuilder();

            string sqlString = string.Format("SELECT SkillID FROM dic_npc_skill_caster_npc WHERE RepresentID = '{0}'", npcRepresentID);
            DataTable skillTable = helper.GetDataTable(conn, sqlString);

            if (skillTable.Rows.Count > 0)
            {
                currentSkillID2 = skillTable.Rows[0]["SkillID"].ToString();
            }
            else
            {
                currentSkillID2 = null;
            }

            editSuccess = AutoCreateSkills(false);

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

            string sqlString = string.Format("SELECT SkillID FROM dic_npc_skill_caster_npc WHERE RepresentID = '{0}'", npcRepresentID);
            DataTable skillTable = helper.GetDataTable(conn, sqlString);

            if (skillTable.Rows.Count > 0)
            {
                currentSkillID2 = skillTable.Rows[0]["SkillID"].ToString();
            }
            else
            {
                currentSkillID2 = null;
            }

            switch (userSection)
            {
                case UserSection.None: // δ����ѡ��
                    {
                        SkillNotifyForm sForm = new SkillNotifyForm();
                        DialogResult dialogResult = sForm.ShowDialog();

                        switch (dialogResult)
                        {
                            case DialogResult.OK:
                                {
                                    editSuccess = AutoCreateSkills(true);

                                    if (sForm.RecordSelection)
                                    {
                                        userSection = UserSection.AutoCreate;       
                                    }

                                    break;
                                }
                            case DialogResult.Cancel:
                                {
                                    if (sForm.RecordSelection)
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
                        editSuccess = AutoCreateSkills(true);

                        break;
                    }
                case UserSection.NotCreate:
                    {
                        break;
                    }
            }

            return editSuccess;
        }
    }
}
