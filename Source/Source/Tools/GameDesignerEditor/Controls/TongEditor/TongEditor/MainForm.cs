using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using DevComponents.DotNetBar;
using DevComponents.AdvTree;

namespace ToneEditor
{
    public partial class MainForm : Office2007Form
    {
        private int fileVersion; // �ļ��汾��
        private DataTable tongOperationTable; // ���������ݱ�
        private DataTable tongOperationGroupTable; // �������������ݱ�
        private List<TongTemplate> tongTemplateList = new List<TongTemplate>(); // ���ģ������

        private bool beginEdit = false; // �Ƿ��Ѿ���ʼ�༭
        private bool groupContentChanged = false; // �����Ϣ�Ƿ�ı�
        private int lastTongIndex = -1; // ��һ��ѡ���TongIndex;

        private enum DisplayMode { HideCheckedItem, HideUncheckedItem, ShowAllItem };
        private DisplayMode displayMode = DisplayMode.ShowAllItem;

        /// <summary>
        /// ���캯��
        /// </summary>
        public MainForm()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// �����
        /// </summary>
        class TongGroup
        {
            private string groupName = ""; // ��������
            private bool enable = false; // �Ƿ���

            private bool[] baseOperationMask = new bool[8]; // ��������Ȩ��
            private bool[,] advanceOperationMask = new bool[16, 8]; // �߼�����Ȩ��,���뱻������������Group��صĲ���Ȩ��
            private bool[,] baseGrantMask = new bool[16, 8]; // ������Ȩ
            private bool[, ,] advanceGrantMask = new bool[16, 16, 8]; // // �߼���Ȩ,���뱻������������Group��صĲ�����Ȩ

            /// <summary>
            /// ��������
            /// </summary>
            [Browsable(true)]
            [Category("����Ա����")]
            [DisplayName("��Ա����")]
            [Description("����Ա������")]
            public string GroupName
            {
                set
                {
                    groupName = value;
                }

                get
                {
                    return groupName;
                }
            }

            /// <summary>
            /// �Ƿ���
            /// </summary>
            [Browsable(true)]
            [Category("����Ա����")]
            [DisplayName("�Ƿ���")]
            [Description("�Ƿ�����ǰ��Ա")]
            public bool Enable
            {
                set
                {
                    enable = value;
                }

                get
                {
                    return enable;
                }
            }

            /// <summary>
            /// ��������Ȩ��
            /// </summary>
            [Browsable(false)]
            public bool[] BaseOperationMask
            {
                get
                {
                    return baseOperationMask;
                }
            }

            /// <summary>
            /// �߼�����Ȩ��
            /// </summary>
            [Browsable(false)]
            public bool[,] AdvanceOperationMask
            {
                get
                {
                    return advanceOperationMask;
                }
            }

            /// <summary>
            /// ������Ȩ
            /// </summary>
            [Browsable(false)]
            public bool[,] BaseGrantmask
            {
                get
                {
                    return baseGrantMask;
                }
            }

            /// <summary>
            /// �߼���Ȩ
            /// </summary>
            [Browsable(false)]
            public bool[ , , ] AdvanceGrantMask
            {
                get
                {
                    return advanceGrantMask;
                }
            }

            /// <summary>
            /// ��Ϊ�᳤��ʼ��
            /// </summary>
            public void InitAsLeader()
            {
                enable = true;

                for (int i = 0; i < 8; i++)
                {
                    baseOperationMask[i] = true;
                }

                for (int i = 0; i < 16; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        advanceOperationMask[i, j] = true;
                        baseGrantMask[i, j] = true;
                    }
                    
                    for (int j = 0; j < 16; j++)
                    {
                        for (int k = 0; k < 8; k++)
                        {
                            advanceGrantMask[i, j, k] = true;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// ���ģ��
        /// </summary>
        class TongTemplate
        {
            private int masterGroup = 0; // ��ӱ��
            private int defaultGroup = 0; // Ĭ�ϳ�Ա���
            private TongGroup[] tongGroupArray = new TongGroup[16]; // ��Ա����

            /// <summary>
            /// ���캯��
            /// </summary>
            /// <param name="masterGroup">��ӱ��</param>
            /// <param name="defaultGroup">Ĭ�ϳ�Ա���</param>
            /// <param name="tongGroupArray">��Ա����</param>
            public TongTemplate(int masterGroup, int defaultGroup, TongGroup[] tongGroupArray)
            {
                this.masterGroup = masterGroup;
                this.defaultGroup = defaultGroup;
                this.tongGroupArray = tongGroupArray;
            }

            /// <summary>
            /// ��ӱ��
            /// </summary>
            [Browsable(true)]
            [Category("�������")]
            [DisplayName("��ӱ��")]
            [Description("��ӱ��")]
            public int MasterGroup
            {
                set
                {
                    if (value >= 0 && value < 16)
                    {
                        masterGroup = value;
                    }
                    else
                    {
                        MessageBox.Show("���ֵֻ����0��15֮�䣡", "��Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }

                get
                {
                    return masterGroup;
                }
            }

            // Ĭ�ϳ�Ա���
            [Browsable(true)]
            [Category("�������")]
            [DisplayName("Ĭ�ϱ��")]
            [Description("Ĭ�ϱ��")]
            public int DefaultGroup
            {
                set
                {
                    if (value >= 0 && value < 16)
                    {
                        defaultGroup = value;
                    }
                    else
                    {
                        MessageBox.Show("���ֵֻ����0��15֮�䣡", "��Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }

                get
                {
                    return defaultGroup;
                }
            }

            /// <summary>
            /// ����Ա����
            /// </summary>
            [Browsable(false)]
            public TongGroup[] TongGroupArray
            {
                get
                {
                    return tongGroupArray;
                }
            }
        }

        /// <summary>
        /// ˢ�°���Ա��
        /// </summary>
        private void RefreshTongTree()
        {
            tongTree.Nodes.Clear();

            // ��ʼ�������
            for (int i = 0; i < tongTemplateList.Count; i++)
            {
                TongGroup[] tArray = tongTemplateList[i].TongGroupArray;
                Node tongNode = new Node();
                int tongIndex = i + 1;
                tongNode.Text = string.Format("���{0}", tongIndex);
                tongNode.Tag = i.ToString();
                tongTree.Nodes.Add(tongNode);

                // ��ʼ����Ա��
                for (int j = 0; j < 16; j++)
                {
                    Node node = new Node();
                    node.Text = GetNodeDisplayText(i, j);
                    node.Tag = j.ToString();
                    tongNode.Nodes.Add(node);
                }

                tongNode.Expand();
            }                

            // �Զ�ѡ�е�һ����㣬ˢ������
            tongTree.SelectedNode = tongTree.Nodes[0].Nodes[0];
        }

        /// <summary>
        /// ��ȡ������������������
        /// </summary>
        /// <returns>������������������</returns>
        private byte[] GetTongGroupArrayListByteData()
        {
            List<byte> byteList = new List<byte>();
            byte[] buffer;

            // ��¼�汾��
            int fileVersion = 3;
            buffer = BitConverter.GetBytes(fileVersion);
            byteList.AddRange(buffer);

            // ��¼�������
            int groupCount = tongTemplateList.Count;
            buffer = BitConverter.GetBytes(groupCount);
            byteList.AddRange(buffer);

            // ��¼�������
            foreach (TongTemplate tongTemplate in tongTemplateList)
            {
                
                // ת���������
                buffer = BitConverter.GetBytes(tongTemplate.MasterGroup);
                byteList.AddRange(buffer);

                // ת��Ĭ�ϳ�Ա����
                buffer = BitConverter.GetBytes(tongTemplate.DefaultGroup);
                byteList.AddRange(buffer);

                TongGroup[] tArray = tongTemplate.TongGroupArray;
                List<byte> dataList = GetTongGroupByteList(tArray);
                byteList.AddRange(dataList);
            }

            return byteList.ToArray();
        }

        /// <summary>
        /// ��ʼ�������������
        /// </summary>
        /// <param name="byteArray">�ֽ�����</param>
        private void InitTongGroupArrayListByByte(byte[] byteArray, int tongDataLength)
        {            
            tongTemplateList.Clear();

            for (int i = 0; i < byteArray.Length; i += tongDataLength)
            {
                List<byte> byteList = new List<byte>();

                for (int j = 0; j < tongDataLength; j++)
                {
                    byteList.Add(byteArray[i + j]);
                }
                
                TongTemplate tongTemplate = GetTongTemplate(byteList);
                tongTemplateList.Add(tongTemplate);
            }
        }

        /// <summary>
        /// ��ȡ���ģ������
        /// </summary>
        /// <param name="byteList">�����ֽ�����</param>
        /// <returns>���ģ������</returns>
        private TongTemplate GetTongTemplate(List<byte> byteList)
        {
            int masterGroup = 0;
            int defaultGroup = 0;            
            TongGroup[] tArray = new TongGroup[16];

            byte[] buffer;
            int currentIndex = 0;

            // ��ȡ�������
            buffer = new byte[4];
            for (int i = 0; i < 4; i++)
            {
                buffer[i] = byteList[currentIndex + i];
            }
            masterGroup = BitConverter.ToInt32(buffer, 0);
            currentIndex += 4;

            // ��ȡĬ�ϳ�Ա����            
            buffer = new byte[4];
            for (int i = 0; i < 4; i++)
            {
                buffer[i] = byteList[currentIndex + i];
            }
            defaultGroup = BitConverter.ToInt32(buffer, 0);
            currentIndex += 4;

            // ��ȡ����
            for (int i = 0; i < 16; i++)
            {
                TongGroup tongGroup = new TongGroup();

                // ��ȡ������
                List<byte> tempByteList = new List<byte>();
                for (int j = 0; j < 32; j++)
                {
                    byte b = byteList[currentIndex + j];
                    char c = Convert.ToChar(b);

                    if (c == '\0')
                    {
                        break;
                    }
                    else
                    {
                        tempByteList.Add(b);
                    }
                }

                string groupName = Encoding.GetEncoding("gb2312").GetString(tempByteList.ToArray());
                tongGroup.GroupName = groupName;
                currentIndex += 32;

                // ��ȡ�Ƿ���
                bool enable = Convert.ToBoolean(byteList[currentIndex]);
                tongGroup.Enable = enable;
                currentIndex += 1;

                // ��ȡ���ʶ��
                if (fileVersion < 3)
                {
                    buffer = new byte[4];
                    for (int j = 0; j < 4; j++)
                    {
                        buffer[j] = byteList[currentIndex + j];
                    }
                    int wage = BitConverter.ToInt32(buffer, 0);
                    // tongGroup.Wage = wage;
                    currentIndex += 4;
                }
                
                // ��ȡ��������Ȩ��
                byte bMask = byteList[currentIndex];
                currentIndex += 1;
                bool[] boolArray = ConvertByteToBooleanArray(bMask);
                
                for (int j = 0; j < 8; j++)
                {
                    tongGroup.BaseOperationMask[j] = boolArray[j];
                }                

                // ��ȡ�߼�����Ȩ��
                for (int j = 0; j < 16; j++)
                {
                    bMask = byteList[currentIndex];
                    currentIndex += 1;
                    boolArray = ConvertByteToBooleanArray(bMask);

                    for (int k = 0; k < 8; k++)
                    {
                        tongGroup.AdvanceOperationMask[j, k] = boolArray[k];
                    }                                       
                }

                // ��ȡ������Ȩ
                for (int j = 0; j < 16; j++)
                {
                    bMask = byteList[currentIndex];
                    currentIndex += 1;
                    boolArray = ConvertByteToBooleanArray(bMask);

                    for (int k = 0; k < 8; k++)
                    {
                        tongGroup.BaseGrantmask[j, k] = boolArray[k];
                    }
                }

                // ��ȡ�߼���Ȩ
                for (int j = 0; j < 16; j++)
                {
                    for (int k = 0; k < 16; k++)
                    {
                        bMask = byteList[currentIndex];
                        currentIndex += 1;
                        boolArray = ConvertByteToBooleanArray(bMask);

                        for (int l = 0; l < 8; l++)
                        {
                            tongGroup.AdvanceGrantMask[j, k, l] = boolArray[l];
                        }
                    }                                           
                }

                tArray[i] = tongGroup;
            }

            TongTemplate tongTemplate = new TongTemplate(masterGroup, defaultGroup, tArray);
            return tongTemplate;
        }

        /// <summary>
        /// ��ȡ������������ֽ�����
        /// </summary>
        /// <param name="tongGroupArray">�������</param>
        /// <return>������������ֽ�����</return>
        private List<byte> GetTongGroupByteList(TongGroup[] tongGroupArray)
        {
            List<byte> byteList = new List<byte>();
            byte[] buffer;            

            // ת������
            foreach (TongGroup tongGroup in tongGroupArray)
            {
                // ��������Ҫ������32�ֽ�
                string groupName = tongGroup.GroupName;
                buffer = Encoding.GetEncoding("gb2312").GetBytes(groupName);                
                
                byte[] groupNameBuffer = new byte[32];
                for (int i = 0; i < buffer.Length; i++)
                {
                    groupNameBuffer[i] = buffer[i];
                }
                byteList.AddRange(groupNameBuffer);

                // ת���Ƿ���
                buffer = BitConverter.GetBytes(tongGroup.Enable);
                byteList.AddRange(buffer);

                // ת����������Ȩ��
                string tempString = ConvertBooleanArrayToString(tongGroup.BaseOperationMask);
                byte tempByte = byte.Parse(tempString);
                byteList.Add(tempByte);

                // ת���߼�����Ȩ��
                bool[] tempArray = new bool[8];
                for (int i = 0; i < 16; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        tempArray[j] = tongGroup.AdvanceOperationMask[i, j];
                    }

                    tempString = ConvertBooleanArrayToString(tempArray);
                    tempByte = byte.Parse(tempString);
                    byteList.Add(tempByte);
                }

                // ת��������Ȩ
                for (int i = 0; i < 16; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        tempArray[j] = tongGroup.BaseGrantmask[i, j];
                    }

                    tempString = ConvertBooleanArrayToString(tempArray);
                    tempByte = byte.Parse(tempString);
                    byteList.Add(tempByte);
                }

                // ת���߼���Ȩ
                for (int i = 0; i < 16; i++)
                {
                    for (int j = 0; j < 16; j++)
                    {
                        for (int k = 0; k < 8; k++)
                        {
                            tempArray[k] = tongGroup.AdvanceGrantMask[i, j, k];
                        }

                        tempString = ConvertBooleanArrayToString(tempArray);
                        tempByte = byte.Parse(tempString);
                        byteList.Add(tempByte);
                    }
                }
            }

            return byteList;
        }        

        /// <summary>
        /// ���ֽ�ת���ɲ�������
        /// </summary>
        /// <param name="b">�ֽ�</param>
        /// <returns>��������</returns>
        private bool[] ConvertByteToBooleanArray(byte b)
        {
            bool[] boolArray = new bool[8];
            int[] tempArray = new int[] { 1, 2, 4, 8, 16, 32, 64, 128 };

            for (int i = 0; i < 8; i++)
            {
                boolArray[i] = ((b & tempArray[i]) != 0);
            }

            return boolArray;
        }

        /// <summary>
        /// �Ѳ���ֵ����ת���������ַ���
        /// </summary>
        /// <param name="boolArray">����ֵ����</param>
        /// <returns>�����ַ���</returns>
        private string ConvertBooleanArrayToString(bool[] boolArray)
        {
            int number = 0;

            if (boolArray[0])
            {
                number += 1;
            }

            if (boolArray[1])
            {
                number += 2;
            }

            if (boolArray[2])
            {
                number += 4;
            }

            if (boolArray[3])
            {
                number += 8;
            }

            if (boolArray[4])
            {
                number += 16;
            }

            if (boolArray[5])
            {
                number += 32;
            }

            if (boolArray[6])
            {
                number += 64;
            }

            if (boolArray[7])
            {
                number += 128;
            }

            return number.ToString();
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
        /// ��ʼ������
        /// </summary>
        private void Init()
        {           
            // �������ݱ�
            string filePath = Path.Combine(Application.StartupPath, @"TongOperation.tab");
            string content = null;

            if (File.Exists(filePath))
            {
                StreamReader reader = new StreamReader(filePath, Encoding.GetEncoding("gb2312"));
                content = reader.ReadToEnd();
                reader.Close();

                tongOperationTable = CreateDataTable(content);
            }
            else
            {
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", filePath), "���������ļ�",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            filePath = Path.Combine(Application.StartupPath, @"TongOperationGroup.tab");

            if (File.Exists(filePath))
            {
                StreamReader reader = new StreamReader(filePath, Encoding.GetEncoding("gb2312"));
                content = reader.ReadToEnd();
                reader.Close();

                tongOperationGroupTable = CreateDataTable(content);
            }
            else
            {
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", filePath), "���������ļ�",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            // ��ʼ��������
            DataGridViewTextBoxColumn operationColumn = new DataGridViewTextBoxColumn();
            operationColumn.Name = "Operation";
            operationColumn.HeaderText = "Operation";

            DataGridViewTextBoxColumn indexColumn = new DataGridViewTextBoxColumn();
            indexColumn.Name = "Index";
            indexColumn.HeaderText = "Index";

            DataGridViewTextBoxColumn descriptionColumn = new DataGridViewTextBoxColumn();
            descriptionColumn.Name = "Description";
            descriptionColumn.HeaderText = "Description";

            DataGridViewComboBoxColumn typeColumn = new DataGridViewComboBoxColumn();
            typeColumn.Name = "Type";
            typeColumn.HeaderText = "Type";
            typeColumn.Items.Add("Common");
            typeColumn.Items.Add("Advance");

            tableOperationView.Columns.Add(operationColumn);
            tableOperationView.Columns.Add(indexColumn);
            tableOperationView.Columns.Add(descriptionColumn);
            tableOperationView.Columns.Add(typeColumn);

            // ��ʼ���������
            TongGroup[] tongGroupArray = CreateTongGroup();     
            tongTemplateList.Add(new TongTemplate(0, 0, tongGroupArray));

            // ˢ������
            RefreshData();

            // ˢ�·�����
            RefreshTongTree();

            // ��Ȼ�Ǵ��������������е�Ԫ����������...
            this.WindowState = FormWindowState.Maximized;
        }

        /// <summary>
        /// �����������
        /// </summary>
        /// <returns>�������</returns>
        private TongGroup[] CreateTongGroup()
        {
            TongGroup[] newTongGroupArray = new TongGroup[16];

            TongGroup tongLeader = new TongGroup();
            tongLeader.InitAsLeader();
            newTongGroupArray[0] = tongLeader;

            for (int i = 1; i < newTongGroupArray.Length; i++)
            {
                newTongGroupArray[i] = new TongGroup();
            }

            return newTongGroupArray;
        }

        /// <summary>
        /// ��ȡ����������ַ���
        /// </summary>
        /// <param name="tongIndex">�����</param>
        /// <param name="groupID">����ID</param>
        /// <returns>����������ַ���</returns>
        private string GetNodeDisplayText(int tongIndex, int groupID)
        {
            TongGroup[] tGroup = tongTemplateList[tongIndex].TongGroupArray;
            string displayText = groupID.ToString();

            string groupName = tGroup[groupID].GroupName;
            if (groupName != "")
            {
                displayText = string.Format("{0} {1}", groupID, groupName);
            }

            if (tGroup[groupID].Enable)
            {
                displayText += " ��";
            }

            return displayText;
        }

        /// <summary>
        /// ���ذ������
        /// </summary>
        /// <param name="tongIndex">�������</param>
        /// <param name="groupID">���ID</param>
        private void LoadTongData(int tongIndex, int groupID)
        {
            TongGroup tongGroup = tongTemplateList[tongIndex].TongGroupArray[groupID];

            // ���ػ�������Ȩ��
            foreach (Node node in commonOperationTree.Nodes)
            {
                int index = int.Parse(node.Cells[0].Tag.ToString());
                node.Checked = tongGroup.BaseOperationMask[index];
            }

            // ���ظ߼�����Ȩ��
            foreach (Node node in advanceOperationTree.Nodes)
            {
                int index = int.Parse(node.Cells[0].Tag.ToString());
                bool isNodeChecked = false;
                Node lastNode = node.LastNode;

                foreach (Node childNode in lastNode.Nodes)
                {
                    int memberID = int.Parse(childNode.Tag.ToString());
                    childNode.Checked = tongGroup.AdvanceOperationMask[memberID, index];

                    if (childNode.Checked)
                    {
                        isNodeChecked = true;
                    }
                }

                node.Checked = isNodeChecked;                
            }

            // ���ػ�����Ȩ
            foreach (Node memberNode in commonAuthorityTree.Nodes)
            {
                int memberID = int.Parse(memberNode.Tag.ToString());
                bool isNodeChecked = false;

                foreach (Node childNode in memberNode.Nodes)
                {
                    int index = int.Parse(childNode.Cells[1].Tag.ToString());
                    childNode.Checked = tongGroup.BaseGrantmask[memberID, index];

                    if (childNode.Checked)
                    {
                        isNodeChecked = true;
                    }
                }

                memberNode.Checked = isNodeChecked;
            }

            // ���ظ߼���Ȩ
            foreach (Node memberNode in advanceAuthorityTree.Nodes)
            {
                int memberID = int.Parse(memberNode.Tag.ToString());
                bool childChecked = false;

                foreach (Node childNode in memberNode.Nodes)
                {
                    int index = int.Parse(childNode.Cells[1].Tag.ToString());
                    bool objectChecked = false;
                    Node lastNode = childNode.LastNode;

                    foreach (Node objectNode in lastNode.Nodes)
                    {
                        int objectID = int.Parse(objectNode.Tag.ToString());
                        objectNode.Checked = tongGroup.AdvanceGrantMask[memberID, objectID, index];

                        if (objectNode.Checked)
                        {
                            objectChecked = true;
                        }
                    }

                    childNode.Checked = objectChecked;

                    if (childNode.Checked)
                    {
                        childChecked = true;
                    }
                }

                memberNode.Checked = childChecked;
            }
        }

        /// <summary>
        /// ���°������
        /// </summary>
        /// <param name="tongIndex">�����ID</param>
        /// <param name="groupID">���ID</param>
        private void UpdateTongData(int tongIndex,int groupID)
        {
            TongGroup tongGroup = tongTemplateList[tongIndex].TongGroupArray[groupID];

            // ���»�������Ȩ��
            foreach (Node node in commonOperationTree.Nodes)
            {
                int index = int.Parse(node.Cells[0].Tag.ToString());
                tongGroup.BaseOperationMask[index] = node.Checked;
            }

            // ���¸߼�����Ȩ��
            foreach (Node node in advanceOperationTree.Nodes)
            {
                int index = int.Parse(node.Cells[0].Tag.ToString());
                Node lastNode = node.LastNode;
                
                if (node.Checked)
                {
                    foreach (Node childNode in lastNode.Nodes)
                    {
                        int memberID = int.Parse(childNode.Tag.ToString());
                        tongGroup.AdvanceOperationMask[memberID, index] = childNode.Checked;
                    }
                }
                else
                {
                    for (int i = 0; i < 16; i++)
                    {
                        tongGroup.AdvanceOperationMask[i, index] = false;
                    }
                }                
            }

            // ������ͨ��Ȩ
            foreach (Node memberNode in commonAuthorityTree.Nodes)
            {
                int memberID = int.Parse(memberNode.Tag.ToString());

                if (memberNode.Checked)
                {
                    foreach (Node childNode in memberNode.Nodes)
                    {
                        int index = int.Parse(childNode.Cells[1].Tag.ToString());
                        tongGroup.BaseGrantmask[memberID, index] = childNode.Checked;                        
                    }
                }
                else
                {
                    for (int i = 0; i < 8; i++)
                    {
                        tongGroup.BaseGrantmask[memberID, i] = false;
                    }
                }
            }

            // ���¸߼���Ȩ
            foreach (Node memberNode in advanceAuthorityTree.Nodes)
            {
                int memberID = int.Parse(memberNode.Tag.ToString());

                if (memberNode.Checked)
                {
                    foreach (Node childNode in memberNode.Nodes)
                    {
                        int index = int.Parse(childNode.Cells[1].Tag.ToString());

                        if (childNode.Checked)
                        {
                            Node lastNode = childNode.LastNode;

                            foreach (Node objectNode in lastNode.Nodes)
                            {
                                int objectID = int.Parse(objectNode.Tag.ToString());
                                tongGroup.AdvanceGrantMask[memberID, objectID, index] = objectNode.Checked;
                            }
                        }
                        else
                        {
                            for (int i = 0; i < 16; i++)
                            {
                                tongGroup.AdvanceGrantMask[memberID, i, index] = false;
                            }
                        }                        
                    }
                }
                else
                {
                    for (int i = 0; i < 16; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            tongGroup.AdvanceGrantMask[memberID, i, j] = false;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// �������ݱ�
        /// </summary>
        private void UpdateTongTable()
        {
            DataTable tempTable = new DataTable();
            DataColumn operationColumn = new DataColumn("Operation");
            DataColumn indexColumn = new DataColumn("Index");
            DataColumn descColumn = new DataColumn("Desc");
            DataColumn typeColumn = new DataColumn("Type");

            tempTable.Columns.Add(operationColumn);
            tempTable.Columns.Add(indexColumn);
            tempTable.Columns.Add(descColumn);
            tempTable.Columns.Add(typeColumn);           

            foreach (DataGridViewRow dataGridViewRow in tableOperationView.Rows)
            {               
                string operationString = dataGridViewRow.Cells["Operation"].EditedFormattedValue as string;
                string indexString = dataGridViewRow.Cells["Index"].EditedFormattedValue as string;
                string descString = dataGridViewRow.Cells["Description"].EditedFormattedValue as string;
                string typeString = dataGridViewRow.Cells["Type"].EditedFormattedValue as string;

                if (!string.IsNullOrEmpty(operationString) && !string.IsNullOrEmpty(indexString) &&
                    !string.IsNullOrEmpty(descString) && !string.IsNullOrEmpty(typeString))
                {
                    DataRow newRow = tempTable.NewRow();
                    newRow["Operation"] = operationString;
                    newRow["Index"] = indexString;
                    newRow["Desc"] = descString;
                    newRow["Type"] = typeString;

                    tempTable.Rows.Add(newRow);
                }                
            }

            tongOperationTable.Rows.Clear();
            DataRow[] dataRows = tempTable.Select("1 = 1", "Type DESC, Index ASC");
            foreach (DataRow dataRow in dataRows)
            {
                DataRow newRow = tongOperationTable.NewRow();
                newRow["Operation"] = dataRow["Operation"];
                newRow["Index"] = dataRow["Index"];
                newRow["Desc"] = dataRow["Desc"];
                newRow["TYPE"] = dataRow["Type"];

                tongOperationTable.Rows.Add(newRow);
            }
        }

        /// <summary>
        /// �����ݱ�ת��Ϊ�ַ���
        /// </summary>
        /// <param name="dataTable">���ݱ�</param>
        /// <returns>�ַ���</returns>
        private string ConvertDataTableToString(DataTable dataTable)
        {
            StringBuilder content = new StringBuilder();

            StringBuilder titleLine = new StringBuilder();
            foreach(DataColumn dataColumn in dataTable.Columns)
            {
                titleLine.Append(string.Format("{0}\t", dataColumn.ColumnName));
            }
            titleLine.Remove(titleLine.Length - 1, 1);
            content.AppendLine(titleLine.ToString());
                        
            foreach(DataRow dataRow in dataTable.Rows)
            {
                StringBuilder line = new StringBuilder();

                for (int i = 0; i < dataTable.Columns.Count; i++)
                {
                    line.Append(string.Format("{0}\t", dataRow[i].ToString()));
                }
                
                line.Remove(line.Length - 1, 1);
                content.AppendLine(line.ToString());
            }            

            return content.ToString().TrimEnd(new char[] { '\r', '\n' });
        }

        /// <summary>
        /// ������д���ļ�
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <param name="content">�ı�����</param>
        private void SaveDataToFile(string fileName, string content)
        {
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
            }
            catch (IOException ex)
            {
                ;
            }
        }

        /// <summary>
        /// ˢ��������ı�
        /// </summary>
        /// <param name="tongIndex">�����</param>
        private void RefreshNodeText(int tongIndex)
        {
            DataRow[] rows;

            // ˢ�¸߼��������
            for (int i = 0; i < 8; i++)
            {
                rows = tongOperationTable.Select(string.Format("Index = '{0}' AND TYPE = 'Advance'", i.ToString()));

                if (rows.Length > 0)
                {
                    Node parentNode = advanceOperationTree.Nodes[i];
                    Node lastNode = parentNode.LastNode;

                    for (int j = 0; j < 16; j++)
                    {
                        Node memberNode = lastNode.Nodes[j];
                        memberNode.Cells[3].Text = GetNodeDisplayText(tongIndex, j);
                    }
                }
                else
                {
                    break;
                }
            }

            // ˢ�»�����Ȩ���
            for (int i = 0; i < 16; i++)
            {
                Node memberNode = commonAuthorityTree.Nodes[i];
                memberNode.Cells[0].Text = GetNodeDisplayText(tongIndex, i);
            }

            // ˢ�¸߼���Ȩ���
            for (int i = 0; i < 16; i++)
            {
                Node memberNode = advanceAuthorityTree.Nodes[i];
                memberNode.Cells[0].Text = GetNodeDisplayText(tongIndex, i);

                for (int j = 0; j < 8; j++)
                {
                    rows = tongOperationTable.Select(string.Format("Index = '{0}' AND TYPE = 'Advance'", j.ToString()));

                    if (rows.Length > 0)
                    {
                        Node parentNode = memberNode.Nodes[j];
                        Node lastNode = parentNode.LastNode;

                        for (int k = 0; k < 16; k++)
                        {
                            Node objectNode = lastNode.Nodes[k];
                            objectNode.Cells[0].Text = GetNodeDisplayText(tongIndex, k);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        
        /// <summary>
        /// ��ȡ��������ŵ�����
        /// </summary>
        /// <param name="index">���������</param>
        /// <param name="operationType">����������</param>
        /// <returns>��������ŵ�����</returns>
        private string GetTongOperationIndexDesplayText(string index, string operationType)
        {
            string displayText = index;

            DataRow[] rows = tongOperationGroupTable.Select(string.Format("Index = '{0}' AND TYPE = '{1}'", index, operationType));

            if (rows.Length > 0)
            {
                string descrption = rows[0]["Desc"].ToString();

                if (descrption != "")
                {
                    displayText = string.Format("{0} {1}", index, descrption);
                }
            }

            return displayText;
        }

        /// <summary>
        /// ˢ������
        /// </summary>
        private void RefreshData()
        {
            DataRow[] rows;                       

            // ��ʼ�������������
            commonOperationTree.Nodes.Clear();

            for (int i = 0; i < 8; i++)
            {
                rows = tongOperationTable.Select(string.Format("Index = '{0}' AND TYPE = 'Common'", i.ToString()));                

                if (rows.Length > 0)
                {
                    Node parentNode = new Node();                    
                    parentNode.Cells.Add(new Cell());
                    parentNode.Cells.Add(new Cell());
                    parentNode.Cells[0].Text = GetTongOperationIndexDesplayText(i.ToString(), "Common");
                    parentNode.Cells[0].Tag = i.ToString();
                    parentNode.CheckBoxVisible = true;

                    foreach (DataRow row in rows)
                    {
                        Node childNode = new Node();
                        childNode.Cells.Add(new Cell());
                        childNode.Cells.Add(new Cell());
                        childNode.Cells[0].Text = i.ToString();
                        childNode.Cells[1].Text = row["Operation"] as string;
                        childNode.Cells[2].Text = row["Desc"] as string;

                        parentNode.Nodes.Add(childNode);
                    }

                    parentNode.Expand();                    
                    commonOperationTree.Nodes.Add(parentNode);
                }
                else
                {
                    break;
                }
            }            

            // ��ʼ���߼��������
            advanceOperationTree.Nodes.Clear();

            for (int i = 0; i < 8; i++)
            {
                rows = tongOperationTable.Select(string.Format("Index = '{0}' AND TYPE = 'Advance'", i.ToString()));             

                if (rows.Length > 0)
                {
                    Node parentNode = new Node();
                    parentNode.Cells.Add(new Cell());
                    parentNode.Cells.Add(new Cell());
                    parentNode.Cells.Add(new Cell());
                    parentNode.Cells[0].Text = GetTongOperationIndexDesplayText(i.ToString(), "Advance");
                    parentNode.Cells[0].Tag = i.ToString();
                    parentNode.CheckBoxVisible = true;

                    foreach(DataRow row in rows)
                    {
                        Node childNode = new Node();
                        childNode.Cells.Add(new Cell());
                        childNode.Cells.Add(new Cell());
                        childNode.Cells.Add(new Cell());
                        childNode.Cells[0].Text = i.ToString();
                        childNode.Cells[1].Text = row["Operation"] as string;
                        childNode.Cells[2].Text = row["Desc"] as string;

                        parentNode.Nodes.Add(childNode);
                    }

                    Node lastNode = parentNode.LastNode;

                    for (int j = 0; j < 16; j++)
                    {
                        Node memberNode = new Node();
                        memberNode.Cells.Add(new Cell());
                        memberNode.Cells.Add(new Cell());
                        memberNode.Cells.Add(new Cell());
                        memberNode.Cells[3].Text = j.ToString();
                        memberNode.Tag = j.ToString();
                        memberNode.CheckBoxVisible = true;
                        lastNode.Nodes.Add(memberNode);                        
                    }

                    lastNode.Expand();
                    parentNode.Expand();
                    advanceOperationTree.Nodes.Add(parentNode);                    
                }
                else
                {
                    break;
                }
            }            

            // ��ʼ��������Ȩ���
            commonAuthorityTree.Nodes.Clear();

            for (int i = 0; i < 16; i++)
            {
                Node memberNode = new Node();                                
                memberNode.Cells.Add(new Cell());
                memberNode.Cells.Add(new Cell());
                memberNode.Cells.Add(new Cell());
                memberNode.Cells[0].Text = i.ToString();
                memberNode.Tag = i.ToString();
                memberNode.CheckBoxVisible = true;

                for (int j = 0; j < 8; j++)
                {
                    rows = tongOperationTable.Select(string.Format("Index = '{0}' AND TYPE = 'Common'", j.ToString()));

                    if (rows.Length > 0)
                    {
                        Node parentNode = new Node();
                        parentNode.Cells.Add(new Cell());
                        parentNode.Cells.Add(new Cell());
                        parentNode.Cells.Add(new Cell());
                        parentNode.Cells[1].Text = GetTongOperationIndexDesplayText(j.ToString(), "Common");
                        parentNode.Cells[1].Tag = j.ToString();
                        parentNode.CheckBoxVisible = true;

                        foreach (DataRow row in rows)
                        {
                            Node childNode = new Node();
                            childNode.Cells.Add(new Cell());
                            childNode.Cells.Add(new Cell());
                            childNode.Cells.Add(new Cell());
                            childNode.Cells[1].Text = j.ToString();
                            childNode.Cells[2].Text = row["Operation"] as string;
                            childNode.Cells[3].Text = row["Desc"] as string;

                            parentNode.Nodes.Add(childNode);
                        }

                        // parentNode.Expand();
                        memberNode.Nodes.Add(parentNode);
                    }
                    else
                    {
                        break;
                    }
                }

                // memberNode.Expand();
                commonAuthorityTree.Nodes.Add(memberNode);
            }

            // ��ʼ���߼���Ȩ���
            advanceAuthorityTree.Nodes.Clear();

            for (int i = 0; i < 16; i++)
            {
                Node memberNode = new Node();
                memberNode.Cells.Add(new Cell());
                memberNode.Cells.Add(new Cell());
                memberNode.Cells.Add(new Cell());
                memberNode.Cells[0].Text = i.ToString();
                memberNode.Tag = i.ToString();
                memberNode.CheckBoxVisible = true;

                for (int j = 0; j < 8; j++)
                {
                    rows = tongOperationTable.Select(string.Format("Index = '{0}' AND TYPE = 'Advance'", j.ToString()));

                    if (rows.Length > 0)
                    {
                        Node parentNode = new Node();
                        parentNode.Cells.Add(new Cell());
                        parentNode.Cells.Add(new Cell());
                        parentNode.Cells.Add(new Cell());
                        parentNode.Cells[1].Text = GetTongOperationIndexDesplayText(j.ToString(), "Advance");
                        parentNode.Cells[1].Tag = j.ToString();
                        parentNode.CheckBoxVisible = true;

                        foreach (DataRow row in rows)
                        {
                            Node childNode = new Node();
                            childNode.Cells.Add(new Cell());
                            childNode.Cells.Add(new Cell());
                            childNode.Cells.Add(new Cell());
                            childNode.Cells[1].Text = j.ToString();
                            childNode.Cells[2].Text = row["Operation"] as string;
                            childNode.Cells[3].Text = row["Desc"] as string;  
                          
                            parentNode.Nodes.Add(childNode);
                        }

                        Node lastNode = parentNode.LastNode;

                        for (int k = 0; k < 16; k++)
                        {
                            Node objectNode = new Node();
                            objectNode.Cells.Add(new Cell());
                            objectNode.Cells.Add(new Cell());
                            objectNode.Cells.Add(new Cell());
                            objectNode.Cells[0].Text = k.ToString();
                            objectNode.Tag = k.ToString();
                            objectNode.CheckBoxVisible = true;

                            lastNode.Nodes.Add(objectNode);
                        }

                        // lastNode.Expand();                        
                        // parentNode.Expand();
                        memberNode.Nodes.Add(parentNode);
                    }
                    else
                    {
                        break;
                    }                    
                }

                memberNode.Expand();
                advanceAuthorityTree.Nodes.Add(memberNode);
            }

            // ��ʼ���������ñ�
            tableOperationView.Rows.Clear();

            for (int i = 0; i < tongOperationTable.Rows.Count; i++)
            {
                DataRow dataRow = tongOperationTable.Rows[i];
                tableOperationView.Rows.Add(1);
                DataGridViewRow newRow = tableOperationView.Rows[i];

                newRow.Cells["Operation"].Value = dataRow["Operation"].ToString();
                newRow.Cells["Index"].Value = dataRow["Index"].ToString();
                newRow.Cells["Description"].Value = dataRow["Desc"].ToString();
                newRow.Cells["Type"].Value = dataRow["TYPE"].ToString();
            }
        }

        /// <summary>
        /// ��ѡ�߼�����ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void advanceOperationTree_AfterCheck(object sender, AdvTreeCellEventArgs e)
        {
            if (beginEdit)
            {
                Node currentNode = advanceOperationTree.SelectedNode;

                if (currentNode != null)
                {
                    if (currentNode.Level == 0)
                    {
                        Node lastNode = currentNode.LastNode;
                        foreach (Node node in lastNode.Nodes)
                        {
                            node.Checked = currentNode.Checked;
                        }
                    }

                    if (currentNode.CheckBoxVisible && currentNode.Checked) // �Զ���ѡ�����
                    {
                        beginEdit = false;
                        Node parentNode = currentNode.Parent;

                        while (parentNode != null)
                        {
                            if (parentNode.CheckBoxVisible)
                            {
                                parentNode.Checked = true;
                            }

                            parentNode = parentNode.Parent;
                        }

                        beginEdit = true;
                    } 
                }

                groupContentChanged = true;
            }            
        }

        /// <summary>
        /// ��ѡ�߼���Ȩѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void advanceAuthorityTree_AfterCheck(object sender, AdvTreeCellEventArgs e)
        {
            if (beginEdit)
            {
                Node currentNode = advanceAuthorityTree.SelectedNode;

                if (currentNode != null)
                {
                    switch (currentNode.Level)
                    {
                        case 0:
                            {
                                foreach (Node node in currentNode.Nodes)
                                {
                                    node.Checked = currentNode.Checked;

                                    Node lastNode = node.LastNode;

                                    foreach (Node childNode in lastNode.Nodes)
                                    {
                                        childNode.Checked = currentNode.Checked;
                                    }
                                }

                                break;
                            }
                        case 1:
                            {
                                Node lastNode = currentNode.LastNode;

                                foreach (Node childNode in lastNode.Nodes)
                                {
                                    childNode.Checked = currentNode.Checked;
                                }

                                break;
                            }
                    }

                    if (currentNode.CheckBoxVisible && currentNode.Checked) // �Զ���ѡ�����
                    {
                        beginEdit = false;
                        Node parentNode = currentNode.Parent;

                        while (parentNode != null)
                        {
                            if (parentNode.CheckBoxVisible)
                            {
                                parentNode.Checked = true;
                            }

                            parentNode = parentNode.Parent;
                        }

                        beginEdit = true;
                    }                    
                }

                groupContentChanged = true;
            }
            
        }

        /// <summary>
        /// ˢ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRefresh_Click(object sender, EventArgs e)
        {
            // ˢ�����ݱ�����
            UpdateTongTable();

            // ˢ�´�����ʾ����
            RefreshData();

            // ˢ����������ʾ
            lastTongIndex = -1;
            tongTree.SelectedNode = null;
            tongTree.SelectedNode = tongTree.Nodes[0].Nodes[0];
        }

        /// <summary>
        /// �رմ���
        /// </summary>
        /// <param name="sender">�¼�</param>
        /// <param name="e"></param>
        private void bExit_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("ȷ���رյ�ǰ���壿", "�رմ���", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
            {
                this.Close();
            }            
        }

        /// <summary>
        /// ��ѡ������Ȩѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void commonAuthorityTree_AfterCheck(object sender, AdvTreeCellEventArgs e)
        {
            if (beginEdit)
            {
                Node currentNode = commonAuthorityTree.SelectedNode;

                if (currentNode != null)
                {
                    if (currentNode.Level == 0)
                    {
                        foreach (Node node in currentNode.Nodes)
                        {
                            node.Checked = currentNode.Checked;
                        }
                    }                    

                    if (currentNode.CheckBoxVisible && currentNode.Checked) // �Զ���ѡ�����
                    {
                        beginEdit = false;
                        Node parentNode = currentNode.Parent;

                        while (parentNode != null)
                        {
                            if (parentNode.CheckBoxVisible)
                            {
                                parentNode.Checked = true;
                            }

                            parentNode = parentNode.Parent;
                        }

                        beginEdit = true;
                    } 
                }

                groupContentChanged = true;
            }                                    
        }

        /// <summary>
        /// ��ѡ��������ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void commonOperationTree_AfterCheck(object sender, AdvTreeCellEventArgs e)
        {
            if (beginEdit)
            {
                groupContentChanged = true;
            }            
        }

        /// <summary>
        /// �л�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void advTree1_AfterNodeSelect(object sender, AdvTreeNodeEventArgs e)
        {
            if (e.Node != null)
            {
                if (e.Node.Level == 1)
                {
                    int groupID = int.Parse(e.Node.Tag.ToString());
                    int tongIndex = int.Parse(e.Node.Parent.Tag.ToString());

                    if (lastTongIndex != tongIndex)
                    {
                        RefreshNodeText(tongIndex);
                        lastTongIndex = tongIndex;
                    }

                    beginEdit = false;
                    LoadTongData(tongIndex, groupID);
                    groupContentChanged = false;
                    beginEdit = true;

                    // ˢ���Ƿ�������
                    switch (displayMode)
                    {
                        case DisplayMode.ShowAllItem:
                            {
                                break;
                            }
                        case DisplayMode.HideCheckedItem:
                            {
                                HideItem(true);
                                break;
                            }
                        case DisplayMode.HideUncheckedItem:
                            {
                                HideItem(false);
                                break;
                            }
                    }

                    TongGroup tongGroup = tongTemplateList[tongIndex].TongGroupArray[groupID];
                    tongPropertyGrid.SelectedObject = tongGroup;
                }
                else if (e.Node.Level == 0)
                {
                    int tongIndex = int.Parse(e.Node.Tag.ToString());
                    TongTemplate tongTemplate = tongTemplateList[tongIndex];
                    tongPropertyGrid.SelectedObject = tongTemplate;
                }
            }            
        }

        /// <summary>
        /// ����㱻�л�֮��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void advTree1_AfterNodeDeselect(object sender, AdvTreeNodeEventArgs e)
        {
            if (e.Node != null && e.Node.Level == 1)
            {
                if (groupContentChanged)
                {
                    // ȥ��������Ϣ
                    // if (MessageBox.Show("�����Ϣ�ı䣬�Ƿ񱣴����ݣ�", "�༭�����Ϣ", MessageBoxButtons.OKCancel,
                    //                    MessageBoxIcon.Question) == DialogResult.OK)
                    {
                        int groupID = int.Parse(e.Node.Tag.ToString());
                        int tongIndex = int.Parse(e.Node.Parent.Tag.ToString());
                        UpdateTongData(tongIndex, groupID);
                    }
                } 
            }             
        }

        /// <summary>
        /// ��Ӱ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddTong_Click(object sender, EventArgs e)
        {
            // �ȱ��浱ǰ���������
            Node currentNode = tongTree.SelectedNode;
            if (currentNode != null && currentNode.Level == 1 && groupContentChanged)
            {
                // ȥ��������ʾ
                // if (MessageBox.Show("�����Ϣ�ı䣬�Ƿ񱣴����ݣ�", "�༭�����Ϣ", MessageBoxButtons.OKCancel,
                //                     MessageBoxIcon.Question) == DialogResult.OK)
                {
                    int groupID = int.Parse(currentNode.Tag.ToString());
                    int tongIndex = int.Parse(currentNode.Parent.Tag.ToString());
                    UpdateTongData(tongIndex, groupID);
                }
            }

            TongGroup[] newTongGroupArray = CreateTongGroup();         
            tongTemplateList.Add(new TongTemplate(0, 0, newTongGroupArray));

            // ˢ�·�����
            groupContentChanged = false;
            RefreshTongTree();
        }

        /// <summary>
        /// ɾ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDeleteTong_Click(object sender, EventArgs e)
        {
            Node currentNode = tongTree.SelectedNode;

            if (currentNode != null && currentNode.Level == 0)
            {
                if (tongTemplateList.Count > 1)
                {
                    if (MessageBox.Show("ȷ��ɾ����ǰ��᣿", "ɾ�����", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
                    {
                        int tongIndex = int.Parse(currentNode.Tag.ToString());   
                        tongTemplateList.RemoveAt(tongIndex);
                        groupContentChanged = false;
                        RefreshTongTree();
                    }                
                }
                else
                {
                    MessageBox.Show("��Ҫ���ٴ���һ����ᣡ", "ɾ�����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫɾ���İ��!��", "ɾ�����", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }            
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bSave_Click(object sender, EventArgs e)
        {
            SaveConfigForm sForm = new SaveConfigForm();

            if (sForm.ShowDialog() == DialogResult.OK)
            {
                string fileFolder;
                string fileName;

                // ����TongTemplate.dat�ļ�
                if (sForm.SaveTongTemplateFile)
                {
                    fileFolder = sForm.TongTemplateFolder;

                    // �ȱ�������
                    Node currentNode = tongTree.SelectedNode;
                    if (currentNode != null && currentNode.Level == 1 && groupContentChanged)
                    {
                        int groupID = int.Parse(currentNode.Tag.ToString());
                        int tongIndex = int.Parse(currentNode.Parent.Tag.ToString());
                        UpdateTongData(tongIndex, groupID);
                    }            

                    if (!Directory.Exists(fileFolder)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(fileFolder); // �ȴ���Ŀ¼���ٴ����ļ�
                    }

                    fileName = Path.Combine(fileFolder, "TongTemplate.dat");                    
                    FileStream fileStream = new FileStream(fileName, FileMode.OpenOrCreate);

                    byte[] buffer = GetTongGroupArrayListByteData();
                    fileStream.Write(buffer, 0, buffer.Length);
                    fileStream.Close();

                    MessageBox.Show(string.Format("�ļ�{0}����ɹ���", fileName), "�����ļ�", MessageBoxButtons.OK,
                                    MessageBoxIcon.Information);    
                }

                // ����TongOperation.tab�ļ�
                if (sForm.SaveTongOperationFile)
                {
                    fileFolder = sForm.TongOperationFolder;

                    // �ȱ�������
                    UpdateTongTable();

                    string content = ConvertDataTableToString(tongOperationTable);
                    fileName = Path.Combine(fileFolder, "TongOperation.tab");
                    SaveDataToFile(fileName, content);

                    MessageBox.Show(string.Format("�ļ�{0}����ɹ���", fileName), "�����ļ�", MessageBoxButtons.OK,
                                    MessageBoxIcon.Information); 
                }
            }                    
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bLoad_Click(object sender, EventArgs e)
        {
            bool loadSuccess = true;

            // ��ȡ���ݱ�
            string fileName = Path.Combine(Application.StartupPath, @"TongOperation.tab");
            string content = null;

            if (File.Exists(fileName))
            {
                StreamReader reader = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                content = reader.ReadToEnd();
                reader.Close();

                tongOperationTable = CreateDataTable(content);
            }
            else
            {
                loadSuccess = false;
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", fileName), "�����ļ�", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }

            fileName = Path.Combine(Application.StartupPath, @"TongOperationGroup.tab");

            if (File.Exists(fileName))
            {
                StreamReader reader = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                content = reader.ReadToEnd();
                reader.Close();

                tongOperationGroupTable = CreateDataTable(content);
            }
            else
            {
                loadSuccess = false;
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", fileName), "�����ļ�", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }

            // ��ȡ�������
            fileName = Path.Combine(Application.StartupPath, "TongTemplate.dat");            

            if (File.Exists(fileName))
            {
                FileStream fileStream = new FileStream(fileName, FileMode.Open);

                // ��ȡversion
                byte[] buffer = new byte[4];

                for (int i = 0; i < 4; i++)
                {
                    buffer[i] = (byte)fileStream.ReadByte();
                }

                fileVersion = BitConverter.ToInt32(buffer, 0);

                // ��ȡ�������
                buffer = new byte[4];

                for (int i = 0; i < 4; i++)
                {
                    buffer[i] = (byte)fileStream.ReadByte();
                }

                int tongCount = BitConverter.ToInt32(buffer, 0);

                // ��ȡ�����Ϣ
                int tongDataLength = 5220;
                switch (fileVersion)
                {
                    case 1:
                        {
                            break;
                        }
                    case 2:
                        {
                            tongDataLength = 5224;
                            break;
                        }
                    case 3:
                        {
                            tongDataLength = 5160;
                            break;
                        }
                }

                buffer = new byte[tongCount * tongDataLength];
                fileStream.Read(buffer, 0, buffer.Length);
                fileStream.Close();
                InitTongGroupArrayListByByte(buffer, tongDataLength);

                // ��ʼ�������
                lastTongIndex = -1; // ˢһ��������ı�
                groupContentChanged = false;
                RefreshTongTree();
            }
            else
            {
                loadSuccess = false;
                MessageBox.Show(string.Format("�ļ�{0}�����ڣ�", fileName), "�����ļ�", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);                
            }
            
            if (loadSuccess)
            {
                MessageBox.Show("�ļ����سɹ���", "�����ļ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ����Ա����ֵ�ı�
        /// </summary>
        /// <param name="s">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void tongPropertyGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            Node currentNode = tongTree.SelectedNode;

            if (currentNode != null && currentNode.Level == 1)
            {
                int groupID = int.Parse(currentNode.Tag.ToString());
                int tongIndex = int.Parse(currentNode.Parent.Tag.ToString());                
                currentNode.Text = GetNodeDisplayText(tongIndex, groupID);

                RefreshNodeText(tongIndex);
            }
        }

        /// <summary>
        /// �������갴��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void tongTree_NodeMouseDown(object sender, TreeNodeMouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right && e.Node != null && e.Node.Level == 0)
            {
                Point location = new Point(e.X, e.Y) + new Size(tongTree.RectangleToScreen(tongTree.ClientRectangle).Location)
                                 - new Size(tongTree.AutoScrollOffset); // ����ת����������������������
                bTong.Popup(location);
            }            
        }

        /// <summary>
        /// ������
        /// </summary>
        /// <param name="isChecked">�Ƿ�ѡ</param>
        private void HideItem(bool isChecked)
        {
            Node lastCheckedNode = null;
            bool allNodeInvisible = true;

            // ˢ�»����������            
            foreach (Node parentNode in commonOperationTree.Nodes)
            {
                if (parentNode.Checked == isChecked)
                {
                    lastCheckedNode = parentNode;
                    parentNode.Visible = false;
                }
                else
                {
                    parentNode.Visible = true;
                    allNodeInvisible = false;
                }
            }

            // ����Ҫ��һ�����ɼ�
            if (allNodeInvisible)
            {
                lastCheckedNode.Visible = true;
            }

            // ˢ�¸߼��������
            foreach (Node parentNode in advanceOperationTree.Nodes)
            {
                allNodeInvisible = true;

                foreach (Node childNode in parentNode.LastNode.Nodes)
                {
                    if (childNode.Checked == isChecked)
                    {
                        lastCheckedNode = childNode;
                        childNode.Visible = false;
                    }
                    else
                    {
                        childNode.Visible = true;
                        allNodeInvisible = false;
                    }
                }

                // ����Ҫ��һ�����ɼ�
                if (allNodeInvisible)
                {
                    lastCheckedNode.Visible = true;
                }
            }

            // ˢ�»�����Ȩ���
            foreach (Node parentNode in commonAuthorityTree.Nodes)
            {
                foreach (Node childNode in parentNode.Nodes)
                {
                    allNodeInvisible = true;

                    foreach (Node memberNode in childNode.Nodes)
                    {
                        if (memberNode.Checked == isChecked)
                        {
                            lastCheckedNode = memberNode;
                            memberNode.Visible = false;
                        }
                        else
                        {
                            memberNode.Visible = true;
                            allNodeInvisible = false;
                        }
                    }

                    // ����Ҫ��һ�����ɼ�
                    if (allNodeInvisible)
                    {
                        lastCheckedNode.Visible = true;
                    }
                }
            }

            // ˢ�¸߼���Ȩ���
            foreach (Node memberNode in advanceAuthorityTree.Nodes)
            {
                foreach (Node parentNode in memberNode.Nodes)
                {
                    allNodeInvisible = true;

                    foreach (Node objectNode in parentNode.LastNode.Nodes)
                    {
                        if (objectNode.Checked == isChecked)
                        {
                            lastCheckedNode = objectNode;
                            objectNode.Visible = false;
                        }
                        else
                        {
                            objectNode.Visible = true;
                            allNodeInvisible = false;
                        }
                    }

                    // ����Ҫ��һ�����ɼ�
                    if (allNodeInvisible)
                    {
                        lastCheckedNode.Visible = true;
                    }
                }
            } 
        }

        /// <summary>
        /// �����ѹ�ѡ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bHideCheckedItem_Click(object sender, EventArgs e)
        {
            displayMode = DisplayMode.HideCheckedItem;
            HideItem(true);
            bHideCheckedItem.Text = "�����ѹ�ѡ���� ��";
            bHideUncheckedItem.Text = "����δ��ѡ����";
            bShowAllItem.Text = "��ʾ������";
        }

        /// <summary>
        /// ��ʾ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bShowAllItem_Click(object sender, EventArgs e)
        {
            displayMode = DisplayMode.ShowAllItem;

            // ˢ�»����������
            foreach (Node parentNode in commonOperationTree.Nodes)
            {
                parentNode.Visible = true;
            }

            // ˢ�¸߼��������
            foreach (Node parentNode in advanceOperationTree.Nodes)
            {
                foreach (Node childNode in parentNode.LastNode.Nodes)
                {
                    childNode.Visible = true;
                }
            }

            // ˢ�»�����Ȩ���
            foreach (Node parentNode in commonAuthorityTree.Nodes)
            {
                foreach (Node childNode in parentNode.Nodes)
                {
                    foreach (Node memberNode in childNode.Nodes)
                    {
                        memberNode.Visible = true;
                    }
                }
            }

            // ˢ�¸߼���Ȩ���
            foreach (Node memberNode in advanceAuthorityTree.Nodes)
            {
                foreach (Node parentNode in memberNode.Nodes)
                {
                    foreach (Node objectNode in parentNode.LastNode.Nodes)
                    {
                        objectNode.Visible = true;
                    }
                }
            }

            bHideCheckedItem.Text = "�����ѹ�ѡ����";
            bHideUncheckedItem.Text = "����δ��ѡ����";
            bShowAllItem.Text = "��ʾ������ ��";
        }

        /// <summary>
        /// ����δ��ѡ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bHideUncheckedItem_Click(object sender, EventArgs e)
        {
            displayMode = DisplayMode.HideUncheckedItem;
            HideItem(false);
            bHideCheckedItem.Text = "�����ѹ�ѡ����";
            bHideUncheckedItem.Text = "����δ��ѡ���� ��";
            bShowAllItem.Text = "��ʾ������";
        }
    }
}