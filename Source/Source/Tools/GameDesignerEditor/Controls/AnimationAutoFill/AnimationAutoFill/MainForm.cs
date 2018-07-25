using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections;
using System.Text.RegularExpressions;

namespace AnimationAutoFill
{
    public partial class MainForm : Form
    {
        private string clientFolder; // �ͻ����ļ���
        private string dataFilePath; // �����ļ�·��
        private string relevatePath; // ���·��
        private string absolutePath; // ����·��
        private string lastBoneNumber; // ��һ��ģ�ͷ���

        private string voltAnimation;
        private string wardoffAnimation;
        private string vertigoAnimation;
        private string beatbackAnimation;
        private string glissadeAnimation;
        private string knockdownStandupAnimation;
        private string knockdownAnimation;
        private string standupAnimation;
        private string onGroundAnimation;
        private string onGroundUnderAttackAnimation;
        private string time;
        private string deathAnimation;
        private string animationGroup;
        private string knowndownAnimationName;

        private bool requestUpdate = false; // �Ƿ���Ҫ����
        private int lineIndex = 0; // ��ǰ�к�
        private int groupIndex = 0; // ������ 
        private StringBuilder logText = new StringBuilder(); // ��־�ı�
        private DataTable boneToActionTable; // ���ݱ�
        private UpdateResultForm updateResultForm = new UpdateResultForm(); // ���½������

        /// <summary>
        /// ���캯��
        /// </summary>
        public MainForm()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            StreamReader reader;
            string filePath = Path.Combine(Application.StartupPath, "AutoFillConfig.ini");
            string content = null;

            if (File.Exists(filePath))
            {
                try
                {                                        
                    reader = new StreamReader(filePath, Encoding.GetEncoding("gb2312"));
                    content = reader.ReadToEnd();
                    reader.Close();
                }
                catch (IOException ex)
                {
                    MessageBox.Show(string.Format("�ڶ�ȡ�����ļ�ʱ�����쳣��{0}", ex.ToString()), "��ȡ�����ļ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            } 
            
            if (content != null)
            {
                IniHandler.IniStructure m_inis = new IniHandler.IniStructure();
                m_inis = IniHandler.IniStructure.ReadIniWithContent(content);
                clientFolder = m_inis.GetValue("General", "ClientFolder");
                dataFilePath = m_inis.GetValue("General", "DataFilePath");
                textBoxX1.Text = clientFolder;
                textBoxX2.Text = dataFilePath;
            }
        }

        /// <summary>
        /// �������ݱ�
        /// </summary>
        /// <param name="content">����</param>
        /// <returns>���ݱ�</returns>
        private DataTable CreateDataTable(string content)
        {
            DataTable table = new DataTable();
            List<string> fieldList = new List<string>();

            bool firstLine = true;
            string[] lines = content.Split(new char[] { '\n' });

            foreach (string s in lines)
            {
                string line = s.TrimEnd(new char[] { '\r' });

                if (line != "") // �����յ�������
                {
                    string[] data = line.Split(new char[] { '\t' });

                    if (firstLine)
                    {
                        firstLine = false;

                        for (int i = 0; i < data.Length; i++)
                        {
                            DataColumn dataColumn = new DataColumn(data[i]);
                            table.Columns.Add(dataColumn);
                            fieldList.Add(data[i]);
                        }
                    }
                    else
                    {
                        DataRow dataRow = table.NewRow();

                        for (int i = 0; i < data.Length; i++)
                        {
                            dataRow[i] = data[i];
                        }

                        table.Rows.Add(dataRow);
                    }
                }
            }

            return table;
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

            absolutePath = Path.Combine(clientFolder, relevatePath);
        }

        /// <summary>
        /// ��ȡ������Ϣ��ϣ��
        /// </summary>
        /// <param name="boneNumber">ģ�ͷ���</param>
        /// <returns>������Ϣ��ϣ��</returns>
        private Hashtable GetAnimationInfoTable(string boneNumber)
        {
            Hashtable infoTable = new Hashtable();

            InitRelevatePath(boneNumber);
            DirectoryInfo di = new DirectoryInfo(absolutePath);

            if (di.Exists)
            {
                FileInfo[] files = di.GetFiles();
                string animationClass;
                string fileName;

                foreach(FileInfo fi in files)
                {
                    if (fi.Extension == ".ani" || fi.Extension == ".tani")
                    {
                        animationClass = GetAnimationClass(fi.Name, boneNumber);
                        fileName = fi.Name.Replace(".ani", "").Replace(".tani", "");

                        if (animationClass != null)
                        {
                            List<string> fileList;

                            if (infoTable[animationClass] == null)
                            {
                                fileList = new List<string>();
                                fileList.Add(fileName);
                                infoTable[animationClass] = fileList;
                            }
                            else
                            {
                                fileList = infoTable[animationClass] as List<string>;
                                fileList.Add(fileName);
                            }
                        }                        
                    }                    
                }
            }
            else
            {
                logText.Append(string.Format("Error��·��{0}�����ڣ�\r\n", absolutePath));
            }

            return infoTable;
        }

        /// <summary>
        /// ��ȡ��������
        /// </summary>
        /// <param name="animationString">�����ַ���</param>
        /// <param name="boneNumber">ģ�ͷ���</param>
        /// <returns>��������</returns>
        private string GetAnimationClass(string animationString, string boneNumber)
        {
            string animationClass = null;

            string regexPattern = string.Format(@"{0}_(?<SectionName>([a-z]|[A-Z])*)0", boneNumber);
            Regex r = new Regex(regexPattern); 
            MatchCollection matches = r.Matches(animationString);

            if (matches.Count > 0)
            {
                animationClass = matches[0].Result("${SectionName}");
            }            

            return animationClass;
        }

        /// <summary>
        /// ��ȡ�������
        /// </summary>
        /// <param name="animationString">�����ַ���</param>
        /// <param name="animationType">��������</param>
        /// <returns>�������</returns>
        private int GetAnimationIndex(string animationString, string animationType)
        {
            int animationIndex = 0;
            string animationIndexString = null;

            string regexPattern = string.Format(@"_{0}0(?<SectionName>\d*)", animationType);
            Regex r = new Regex(regexPattern);
            MatchCollection matches = r.Matches(animationString);

            if (matches.Count > 0)
            {
                animationIndexString = matches[0].Result("${SectionName}");
                animationIndex = int.Parse(animationIndexString);
            }

            return animationIndex;
        }

        /// <summary>
        /// ��ʼ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(clientFolder) && !string.IsNullOrEmpty(dataFilePath))
            {
                StreamReader reader;
                string filePath = dataFilePath;
                string content;
                string[] animationArray;

                // ��ʼ���ļ�
                if (File.Exists(filePath))
                {
                    reader = new StreamReader(filePath, Encoding.GetEncoding("gb2312"));
                    content = reader.ReadToEnd();
                    reader.Close();

                    boneToActionTable = CreateDataTable(content);
                    updateResultForm.Reset();

                    lineIndex = 0;
                    for (int i = 0; i < boneToActionTable.Rows.Count; i++)
                    {
                        lineIndex = i + 2;
                        DataRow dataRow = boneToActionTable.Rows[i];
                        string boneNumber = dataRow["BoneNumber"] as string;

                        if (string.IsNullOrEmpty(boneNumber))
                        {
                            logText.Append(string.Format("Error����{0}�����е�BoneNumberΪ�գ�", lineIndex.ToString()));
                        }
                        else
                        {
                            logText.Append(string.Format("��ʼ����{0}������...", lineIndex.ToString()));

                            Hashtable infoTable = GetAnimationInfoTable(boneNumber);

                            voltAnimation = dataRow["Volt"] as string;
                            if (string.IsNullOrEmpty(voltAnimation))
                            {
                                FillAnimation(infoTable, dataRow, "sb");
                            }

                            wardoffAnimation = dataRow["Wardoff"] as string;
                            if (string.IsNullOrEmpty(wardoffAnimation))
                            {
                                FillAnimation(infoTable, dataRow, "gd");
                            }

                            vertigoAnimation = dataRow["Vertigo"] as string;
                            if (string.IsNullOrEmpty(vertigoAnimation))
                            {
                                FillAnimation(infoTable, dataRow, "xy");
                            }

                            beatbackAnimation = dataRow["Beatback"] as string;
                            if (string.IsNullOrEmpty(beatbackAnimation))
                            {
                                FillAnimation(infoTable, dataRow, "jt");
                            }
                            
                            glissadeAnimation = dataRow["Glissade"] as string;
                            if (string.IsNullOrEmpty(glissadeAnimation))
                            {
                                FillAnimation(infoTable, dataRow, "th");
                            }

                            knockdownStandupAnimation = dataRow["Knockdown_Standup"] as string;
                            knockdownAnimation = null;
                            standupAnimation = null;

                            if (string.IsNullOrEmpty(knockdownStandupAnimation))
                            {
                                animationArray = knockdownStandupAnimation.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                                if (animationArray.Length > 0)
                                {
                                    animationArray = animationArray[0].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                                    if (animationArray.Length == 2)
                                    {
                                        knockdownAnimation = animationArray[0];
                                        standupAnimation = animationArray[1];

                                        FillAnimation(infoTable, dataRow, "jd");
                                        FillAnimation(infoTable, dataRow, "pq");
                                    }
                                }
                            }                            

                            onGroundAnimation = dataRow["OnGround"] as string;
                            if (string.IsNullOrEmpty(onGroundAnimation))
                            {
                                FillAnimation(infoTable, dataRow, "jdz");
                            }

                            onGroundUnderAttackAnimation = dataRow["OnGroundUnderAttack"] as string;
                            if (string.IsNullOrEmpty(onGroundUnderAttackAnimation))
                            {
                                FillAnimation(infoTable, dataRow, "jdzbat");
                            }                           

                            time = dataRow["Time"] as string;
                            if (string.IsNullOrEmpty(time))
                            {
                                dataRow["Time"] = "24";
                                updateResultForm.AppendLog(lineIndex.ToString(), "Time", "24");
                                requestUpdate = true;
                            }

                            knockdownStandupAnimation = dataRow["Knockdown_Standup"] as string;                            
                            onGroundAnimation = dataRow["OnGround"] as string;
                            onGroundUnderAttackAnimation = dataRow["OnGroundUnderAttack"] as string;                           
                            time = dataRow["Time"] as string;
                            deathAnimation = dataRow["Death"] as string;
                            animationGroup = dataRow["Knockdown_OnGround_OnGroundUnderAttack_Standup_Death_Time"] as string;

                            if (string.IsNullOrEmpty(animationGroup) && !string.IsNullOrEmpty(knockdownStandupAnimation) && !string.IsNullOrEmpty(onGroundAnimation) &&
                                !string.IsNullOrEmpty(onGroundUnderAttackAnimation) &&!string.IsNullOrEmpty(time) && 
                                !string.IsNullOrEmpty(deathAnimation))
                            {
                                animationArray = knockdownStandupAnimation.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                                if (animationArray.Length > 0)
                                {
                                    animationArray = animationArray[0].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                                    if (animationArray.Length == 2)
                                    {
                                        knockdownAnimation = animationArray[0];
                                        standupAnimation = animationArray[1];

                                        animationArray = deathAnimation.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                                        deathAnimation = animationArray[0];
                                        animationGroup = string.Format("{0},{1},{2},{3},{4},{5}", knockdownAnimation, onGroundAnimation, onGroundUnderAttackAnimation,
                                                                       standupAnimation, deathAnimation, time);
                                        dataRow["Knockdown_OnGround_OnGroundUnderAttack_Standup_Death_Time"] = animationGroup;
                                        updateResultForm.AppendLog(lineIndex.ToString(), "Knockdown_OnGround_OnGroundUnderAttack_Standup_Death_Time", animationGroup);
                                        requestUpdate = true;
                                    }
                                }                                
                            }

                            if (lastBoneNumber == boneNumber || lastBoneNumber == null)
                            {
                                groupIndex++;
                            }
                            else
                            {
                                lastBoneNumber = boneNumber;
                                groupIndex = 0;                                                                
                            }
                        }
                    }

                    if (requestUpdate) // ��Ҫ����
                    {
                        SaveDataTableToFile(boneToActionTable);
                    }
                    else
                    {
                        MessageBox.Show("�������Ѿ������£�", "�����Զ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }                    
                }
                else
                {
                    MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", filePath), "���������ļ�",
                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("������Ϣ��������", "�����Զ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �������ݱ�����
        /// </summary>
        /// <param name="dataTable">���ݱ�</param>
        private void SaveDataTableToFile(DataTable dataTable)
        {
            StringBuilder content = new StringBuilder();
            StringBuilder firstLine = new StringBuilder();            

            foreach(DataColumn dataColumn in dataTable.Columns)
            {
                firstLine.Append(string.Format("{0}\t", dataColumn.ColumnName));
            }            

            content.Append(string.Format("{0}\r\n", firstLine.Remove(firstLine.Length - 1, 1).ToString()));

            foreach(DataRow dataRow in dataTable.Rows)
            {
                StringBuilder dataLine = new StringBuilder();

                for (int i = 0; i < dataTable.Columns.Count; i++)
                {
                    dataLine.Append(string.Format("{0}\t", dataRow[i].ToString()));
                }

                content.Append(string.Format("{0}\r\n", dataLine.Remove(dataLine.Length - 1, 1).ToString()));
            }

            bool saveSuccess = SaveDataToFile(dataFilePath, content.ToString());
            if (saveSuccess)
            {
                MessageBox.Show("�ļ�����ɹ���", "�����ļ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                updateResultForm.ShowDialog();
            }
        }

        /// <summary>
        /// ������д���ļ�
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <param name="content">�ı�����</param>
        /// <return>�Ƿ񱣴�ɹ�</return>
        private bool SaveDataToFile(string fileName, string content)
        {
            bool success = false;

            try
            {
                string filePath = Path.GetDirectoryName(fileName);
                FileInfo fi = new FileInfo(fileName);                
                StreamWriter sw;

                if (!fi.Exists) // �ļ�������
                {
                    if (!Directory.Exists(filePath)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(filePath); // �ȴ���Ŀ¼���ٴ����ļ�
                    }

                    FileStream fs = fi.Create();
                    fs.Close();
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }
                else // �ļ��Ѿ�����
                {                    
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }

                sw.Write(content);
                sw.Close();
                success = true;
            }
            catch (IOException ex)
            {
                MessageBox.Show(string.Format("д���ļ�{0}ʱ�����쳣��{1}", fileName, ex.Message));
            }

            return success;
        }

        /// <summary>
        /// ��䶯��
        /// </summary>
        private void FillAnimation(Hashtable infoTable, DataRow dataRow, string animationType)
        {
            string fieldName = null;

            switch(animationType)
            {
                case "sb": // ����
                    {
                        fieldName = "Volt";
                        break;
                    }
                case "gd": // ��
                    {
                        fieldName = "Wardoff";
                        break;
                    }
                case "xy": // ѣ��
                    {
                        fieldName = "Vertigo";
                        break;
                    }
                case "jt": // ����
                    {
                        fieldName = "Beatback";
                        break;
                    }
                case "th": // ͣ��
                    {
                        fieldName = "Glissade";
                        break;
                    }
                case "jd": // ����
                    {
                        fieldName = "Knockdown";
                        break;
                    }
                case "jdz": // ������
                    {
                        fieldName = "OnGround";
                        break;
                    }
                case "jdbat": // �����б���
                    {
                        fieldName = "OnGroundUnderAttack";
                        break;
                    }
                case "pq": // ����
                    {
                        fieldName = "Standup";
                        break;
                    }
            }

            if (fieldName != null)
            {
                List<string> fileList = infoTable[animationType] as List<string>;

                if (fileList != null)
                {
                    fileList.Sort();
                    string animationName = null;

                    if (groupIndex < fileList.Count)
                    {
                        animationName = fileList[groupIndex];
                    }
                    else
                    {
                        animationName = fileList[0];
                    }

                    switch(fieldName)
                    {
                        case "jd":
                            {
                                knowndownAnimationName = animationName;
                                break;
                            }
                        case "pq":
                            {
                                fieldName = "Knockdown_Standup";
                                dataRow[fieldName] = string.Format("{0},{1}", knowndownAnimationName, animationName);
                                break;
                            }
                        default:
                            {
                                dataRow[fieldName] = animationName;
                                break;
                            }
                    }
                    
                    updateResultForm.AppendLog(lineIndex.ToString(), fieldName, animationName);
                    requestUpdate = true;
                }
            }
        }

        /// <summary>
        /// �˳�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// ����ļ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBroserDialog = new FolderBrowserDialog();
            if (clientFolder != null)
            {
                folderBroserDialog.SelectedPath = clientFolder;                
            }
            folderBroserDialog.Description = "��ѡ��ͻ������ڵ�Ŀ¼";

            if (folderBroserDialog.ShowDialog() == DialogResult.OK)
            {
                clientFolder = folderBroserDialog.SelectedPath;
                textBoxX1.Text = clientFolder;
            }
        }

        /// <summary>
        /// ����ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX4_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            if (dataFilePath != null)
            {
                openFileDialog.InitialDirectory = dataFilePath;
            }
            openFileDialog.Title = "��ѡ��Ҫ�򿪵������ļ�";
            openFileDialog.Filter="TAB�ļ�|*.tab";
            openFileDialog.RestoreDirectory = true;

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                dataFilePath = openFileDialog.FileName;
                textBoxX2.Text = dataFilePath;
            }
        }

        /// <summary>
        /// ����ر���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!string.IsNullOrEmpty(clientFolder) && !string.IsNullOrEmpty(dataFilePath))
            {
                string fileName = Path.Combine(Application.StartupPath, "AutoFillConfig.ini");
                string content = string.Format("[General]\r\nClientFolder={0}\r\nDataFilePath={1}\r\n", clientFolder, dataFilePath);
                SaveDataToFile(fileName, content);
            }            
        }
    }
}