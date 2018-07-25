using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Xml;

using DevComponents.DotNetBar;
using DevComponents.AdvTree;

namespace GuideConfig
{
    public partial class MainForm : Office2007Form
    {
        private SqlConnection conn; // sql����
        private DataTable classTable;
        private DataTable tabTable;
        private DataTable fieldTable;

        private bool classInfoChanged = false;
        private bool tabInfoChanged = false;
        private bool fieldInfoChanged = false;

        private const string classTableName = "sys_guide_class_config";
        private const string tabTableName = "sys_guide_tab_config";
        private const string fieldTableName = "sys_guide_field_config";

        private Node lastSelectedNode = null; // ��һ��ѡ�е�����㣬����ʱ��

        private enum FieldType { Text, Number, Boolean, List, File, Custom }; // ѡ������

        public MainForm()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// sql����
        /// </summary>
        public SqlConnection Connection
        {
            set
            {
                conn = value;
            }
        }

        /// <summary>
        /// ���¼��������ݱ�
        /// </summary>
        private void ReloadClassTable()
        {
            string sqlString = string.Format("SELECT ID, Name FROM {0}", classTableName);
            classTable = Helper.GetDataTable(sqlString, conn);
        }

        /// <summary>
        /// ���¼��ط�ҳ���ݱ�
        /// </summary>
        private void ReloadTabTable()
        {
            string sqlString = string.Format("SELECT ID, Name, ClassID, OrderIndex FROM {0}", tabTableName);
            tabTable = Helper.GetDataTable(sqlString, conn);
        }

        /// <summary>
        /// ���¼���ѡ�����ݱ�
        /// </summary>
        private void ReloadFieldTable()
        {
            string sqlString = string.Format("SELECT ID, Name, TabID, OrderIndex FROM {0}", fieldTableName);
            fieldTable = Helper.GetDataTable(sqlString, conn);
        }

        /// <summary>
        /// ת��ѡ������
        /// </summary>
        /// <param name="typeInfo">�����ַ���</param>
        /// <returns>ѡ������</returns>
        private FieldType ConvertFieldType(string typeInfo)
        {
            FieldType fieldType = FieldType.Text;

            switch (typeInfo)
            {
                case "text":
                    {
                        fieldType = FieldType.Text;
                        break;
                    }
                case "number":
                    {
                        fieldType = FieldType.Number;
                        break;
                    }
                case "boolean":
                    {
                        fieldType = FieldType.Boolean;
                        break;
                    }
                case "list":
                    {
                        fieldType = FieldType.List;
                        break;
                    }
                case "file":
                    {
                        fieldType = FieldType.File;
                        break;
                    }
                case "custom":
                    {
                        fieldType = FieldType.Custom;
                        break;
                    }
            }

            return fieldType;
        }

        /// <summary>
        /// ת��ѡ������
        /// </summary>
        /// <param name="fieldType">ѡ������</param>
        /// <returns>�����ַ���</returns>
        private string ConvertFieldType(FieldType fieldType)
        {
            string typeInfo = "";

            switch (fieldType)
            {
                case FieldType.Text:
                    {
                        typeInfo = "text";
                        break;
                    }
                case FieldType.Number:
                    {
                        typeInfo = "number";
                        break;
                    }
                case FieldType.Boolean:
                    {
                        typeInfo = "boolean";
                        break;
                    }
                case FieldType.List:
                    {
                        typeInfo = "list";
                        break;
                    }
                case FieldType.File:
                    {
                        typeInfo = "file";
                        break;
                    }
                case FieldType.Custom:
                    {
                        typeInfo = "custom";
                        break;
                    }
            }

            return typeInfo;
        }

        /// <summary>
        /// ���¼��ط�����
        /// </summary>
        private void ReloadTree()
        {
            guideTree.Nodes.Clear();

            // ��ȡ�����
            Node rootNode = new Node();
            rootNode.Text = "�򵼷���";
            rootNode.ContextMenu = bRootMenu;
            guideTree.Nodes.Add(rootNode);

            foreach (DataRow classRow in classTable.Rows)
            {
                string className = classRow["Name"].ToString();
                string classID = classRow["ID"].ToString();

                Node classNode = new Node();
                classNode.Text = className;
                classNode.Tag = classID;
                classNode.ContextMenu = bClassMenu;

                DataRow[] tabRows = tabTable.Select(string.Format("ClassID = {0}", classID), "OrderIndex");

                foreach (DataRow tabRow in tabRows)
                {
                    string tabName = tabRow["Name"].ToString();
                    string tabID = tabRow["ID"].ToString();

                    Node tabNode = new Node();
                    tabNode.Text = tabName;
                    tabNode.Tag = tabID;
                    tabNode.ContextMenu = bTabMenu;

                    DataRow[] fieldRows = fieldTable.Select(string.Format("TabID = {0}", tabID), "OrderIndex");

                    foreach (DataRow fieldRow in fieldRows)
                    {
                        string fieldName = fieldRow["Name"].ToString();
                        string fieldID = fieldRow["ID"].ToString();

                        Node fieldNode = new Node();
                        fieldNode.Text = fieldName;
                        fieldNode.Tag = fieldID;
                        fieldNode.ContextMenu = bFieldMenu;

                        tabNode.Nodes.Add(fieldNode);
                    }

                    classNode.Nodes.Add(tabNode);
                }

                rootNode.Nodes.Add(classNode);
            }

            rootNode.Expand();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ʼ��sql����
            string connectString = "Server = jx3web; DataBase = s3design_rtm; Uid = s3design; Password = davidbowie;";
            conn = new SqlConnection(connectString);

            // �������ݱ�
            ReloadClassTable();
            ReloadTabTable();
            ReloadFieldTable();

            // ���ط�����
            ReloadTree();            
        }

        /// <summary>
        /// ����쳣��Ϣ
        /// </summary>
        /// <param name="ex">��ǰ�쳣</param>
        private void PrintExceptionMessage(Exception ex)
        {
            MessageBox.Show(string.Format("��ǰ���������쳣��{0}", ex.Message, "����ʧ��",
                                          MessageBoxButtons.OK, MessageBoxIcon.Error));
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bConfigList_Click(object sender, EventArgs e)
        {
            string listType = "text";

            if (rListDatabaseType.Checked)
            {
                listType = "database";
            }

            string listData = listDataBox.Text;

            ListConfigForm listConfigForm = new ListConfigForm(conn, listType, listData);

            if (listConfigForm.ShowDialog() == DialogResult.OK)
            {
                listDataBox.Text = listConfigForm.ConfigInfo;
            }
        }

        /// <summary>
        /// �л��ֶ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void ChangeFieldType(object sender, EventArgs e)
        {
            RadioButton radioButton = sender as RadioButton;

            if (radioButton.Checked)
            {
                switch (radioButton.Text)
                {
                    case "�ı�":
                        {
                            fieldConfigControl.SelectedTabIndex = 0;
                            break;
                        }
                    case "����":
                        {
                            fieldConfigControl.SelectedTabIndex = 1;
                            break;
                        }
                    case "������":
                        {
                            fieldConfigControl.SelectedTabIndex = 2;
                            break;
                        }
                    case "�ļ�":
                        {
                            fieldConfigControl.SelectedTabIndex = 3;
                            break;
                        }
                    default:
                        {
                            fieldConfigControl.SelectedTabIndex = 4;
                            break;
                        }
                }
            }
        }

        /// <summary>
        /// ��sql����
        /// </summary>
        private void OpenConnection()
        {
            if (conn.State == ConnectionState.Closed)
            {
                conn.Open();
            }
        }

        /// <summary>
        /// �ر�sql����
        /// </summary>
        private void CloseConnection()
        {
            if (conn.State == ConnectionState.Open)
            {
                conn.Close();
            }
        }

        /// <summary>
        /// ִ��sql����
        /// </summary>
        /// <param name="sqlString">sql����</param>
        private void ExecuteSqlCommand(string sqlString)
        {
            try
            {
                OpenConnection();

                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                PrintExceptionMessage(ex);
            }
            finally
            {
                CloseConnection();
            }
        }

        /// <summary>
        /// ���������Ϣ
        /// </summary>
        /// <param name="currentNode">��ǰ�����</param>
        private void SaveClassInfo(Node currentNode)
        {
            if (classInfoChanged)
            {
                string id = currentNode.Tag as string;
                string className = classNameBox.Text;
                string classDescription = classDescriptionBox.Text;
                string sqlString = null;

                if (className != "")
                {
                    sqlString = string.Format("UPDATE {0} SET Name = '{1}' WHERE ID = {2}",
                                              classTableName, className, id);
                    ExecuteSqlCommand(sqlString);

                    currentNode.Text = className;
                }

                if (classDescription != "")
                {
                    sqlString = string.Format("UPDATE {0} SET Description = '{1}' WHERE ID = {2}",
                                              classTableName, classDescription, id);
                    ExecuteSqlCommand(sqlString);
                }
            }            
        }

        /// <summary>
        /// �����ҳ��Ϣ
        /// </summary>
        /// <param name="currentNode">��ǰ�����</param>
        private void SaveTabInfo(Node currentNode)
        {
            if (tabInfoChanged)
            {
                string id = currentNode.Tag as string;
                string tabName = tabNameBox.Text;
                string tabDescription = tabDescriptionBox.Text;
                string sqlString = null;

                if (tabName != "")
                {
                    sqlString = string.Format("UPDATE {0} SET Name = '{1}' WHERE ID = {2}",
                                              tabTableName, tabName, id);
                    ExecuteSqlCommand(sqlString);

                    currentNode.Text = tabName;
                }

                if (tabDescription != "")
                {
                    sqlString = string.Format("UPDATE {0} SET Description = '{1}' WHERE ID = {2}",
                                              tabTableName, tabDescription, id);
                    ExecuteSqlCommand(sqlString);
                }
            }            
        }

        /// <summary>
        /// ����ѡ����Ϣ
        /// </summary>
        /// <param name="currentNode">��ǰ�����</param>
        private void SaveFieldInfo(Node currentNode)
        {
            if (fieldInfoChanged)
            {
	            string id = currentNode.Tag as string;
	            string fieldName = fieldNameBox.Text;
	            string fieldDescription = fieldDescriptionBox.Text;
	            string sqlString = null;
	
	            if (fieldName != "")
	            {
	                sqlString = string.Format("UPDATE {0} SET Name = '{1}' WHERE ID = {2}",
	                                          fieldTableName, fieldName, id);
	                ExecuteSqlCommand(sqlString);
	
	                currentNode.Text = fieldName;
	            }
	
	            if (fieldDescription != "")
	            {
	                sqlString = string.Format("UPDATE {0} SET Description = '{1}' WHERE ID = {2}",
	                                          fieldTableName, fieldDescription, id);
	                ExecuteSqlCommand(sqlString);
	            }

                FieldType fieldType = FieldType.Text;
                string typeData = "";

                if (rTextType.Checked)
                {
                    fieldType = FieldType.Text;
                    int maxLength = textLengthInput.Value;
                    typeData = string.Format("<typeData maxLength = \"{0}\"></typeData>", maxLength);
                }
                else if (rNumberType.Checked)
                {
                    fieldType = FieldType.Number;
                    string numberType = "integer";

                    if (rFloatType.Checked)
                    {
                        numberType = "float";
                    }

                    typeData = string.Format("<typeData numberType = \"{0}\"></typeData>", numberType);
                }
                else if (rBooleanType.Checked)
                {
                    fieldType = FieldType.Boolean;
                }
                else if (rListType.Checked)
                {
                    fieldType = FieldType.List;
                    typeData = string.Format("<typeData>{0}</typeData>", listDataBox.Text);
                }
                else if (rFileType.Checked)
                {
                    fieldType = FieldType.File;
                    string fileFolder = fileFolderBox.Text;
                    typeData = string.Format("<typeData fileFolder = \"{0}\"></typeData>", fileFolder);
                }
                else if (rCustomType.Checked)
                {
                    fieldType = FieldType.Custom;
                }
                
                string typeInfo = ConvertFieldType(fieldType);
                sqlString = string.Format("UPDATE {0} SET Type = '{1}', TypeData = '{2}' WHERE ID = {3}",
                                          fieldTableName, typeInfo, typeData, id);
                ExecuteSqlCommand(sqlString);
            }
        }

        /// <summary>
        /// �����¼
        /// </summary>
        private void SaveRecord()
        {
            Node currentNode = lastSelectedNode;

            if (currentNode != null)
            {
                switch (currentNode.Level)
                {
                    case 1: // ����
                        {
                            SaveClassInfo(currentNode);

                            break;
                        }
                    case 2: // ��ҳ
                        {
                            SaveTabInfo(currentNode);

                            break;
                        }
                    case 3: // ��¼
                        {
                            SaveFieldInfo(currentNode);

                            break;
                        }
                }
            }
        }

        /// <summary>
        /// ���ط�����Ϣ
        /// </summary>
        /// <param name="id">ID</param>
        private void LoadClassInfo(string id)
        {
            string sqlString = string.Format("SELECT Name, Description FROM {0} WHERE ID = {1}", 
                                             classTableName, id);
            DataTable infoTable = Helper.GetDataTable(sqlString, conn);

            string className = "";
            string classDescription = "";

            if (infoTable.Rows.Count > 0)
            {
                DataRow currentRow = infoTable.Rows[0];
                className = currentRow["Name"].ToString();
                classDescription = currentRow["Description"].ToString();
            }

            classNameBox.Text = className;
            classDescriptionBox.Text = classDescription;
            classInfoChanged = false;
        }

        /// <summary>
        /// ���ط�ҳ��Ϣ
        /// </summary>
        /// <param name="id">ID</param>
        private void LoadTabInfo(string id)
        {
            string sqlString = string.Format("SELECT Name, Description FROM {0} WHERE ID = {1}", 
                                             tabTableName, id);
            DataTable infoTable = Helper.GetDataTable(sqlString, conn);

            string tabName = "";
            string tabDescription = "";

            if (infoTable.Rows.Count > 0)
            {
                DataRow currentRow = infoTable.Rows[0];
                tabName = currentRow["Name"].ToString();
                tabDescription = currentRow["Description"].ToString();
            }

            tabNameBox.Text = tabName;
            tabDescriptionBox.Text = tabDescription;
            tabInfoChanged = false;
        }

        /// <summary>
        /// �����ı�ѡ�����Ϣ
        /// </summary>
        /// <param name="typeData">��Ϣ�ַ���</param>
        private void LoadTextFieldInfo(string typeData)
        {
            int maxLength = 100;

            if (!string.IsNullOrEmpty(typeData))
            {
                XmlDocument xmlDocument = new XmlDocument();
                xmlDocument.LoadXml(typeData);
                XmlNode xmlNode = xmlDocument["typeData"];
                string lengthInfo = xmlNode.Attributes["maxLength"].Value;
                maxLength = int.Parse(lengthInfo);
            }
            
            textLengthInput.Value = maxLength;
        }

        /// <summary>
        /// ��������ѡ�����Ϣ
        /// </summary>
        /// <param name="typeData">��Ϣ�ַ���</param>
        private void LoadNumberFieldInfo(string typeData)
        {
            string numberType = "number";

            if (!string.IsNullOrEmpty(typeData))
            {
                XmlDocument xmlDocument = new XmlDocument();
                xmlDocument.LoadXml(typeData);
                XmlNode xmlNode = xmlDocument["typeData"];
                numberType = xmlNode.Attributes["numberType"].Value;
            }            

            switch (numberType)
            {
                case "number":
                    {
                        rIntegerType.Checked = true;
                        break;
                    }
                case "float":
                    {
                        rFloatType.Checked = true;
                        break;
                    }
            }
        }

        /// <summary>
        /// ����������ѡ�����Ϣ
        /// </summary>
        /// <param name="typeData">��Ϣ�ַ���</param>
        private void LoadListFieldInfo(string typeData)
        {
            string listData = "";

            if (!string.IsNullOrEmpty(typeData))
            {
                XmlDocument xmlDocument = new XmlDocument();
                xmlDocument.LoadXml(typeData);
                XmlNode xmlNode = xmlDocument["typeData"];
                listData = xmlNode.InnerXml;                
            }
            
            listDataBox.Text = listData;
        }

        /// <summary>
        /// �����ļ�ѡ�����Ϣ
        /// </summary>
        /// <param name="typeData"></param>
        private void LoadFileFieldInfo(string typeData)
        {
            string fileFolder = "";

            if (!string.IsNullOrEmpty(typeData))
            {
                XmlDocument xmlDocument = new XmlDocument();
                xmlDocument.LoadXml(typeData);
                XmlNode xmlNode = xmlDocument["typeData"];
                fileFolder = xmlNode.Attributes["fileFolder"].Value;
            }
            
            fileFolderBox.Text = fileFolder;
        }

        /// <summary>
        /// ����ѡ����Ϣ
        /// </summary>
        /// <param name="id">ID</param>
        private void LoadFieldInfo(string id)
        {
            string sqlString = string.Format("SELECT * FROM {0} WHERE ID = {1}", 
                                             fieldTableName, id);
            DataTable infoTable = Helper.GetDataTable(sqlString, conn);

            string fieldName = "";
            string fieldDescription = "";

            if (infoTable.Rows.Count > 0)
            {
                DataRow currentRow = infoTable.Rows[0];
                fieldName = currentRow["Name"].ToString();
                fieldDescription = currentRow["Description"].ToString();
                string typeInfo = currentRow["Type"].ToString();
                FieldType fieldType = ConvertFieldType(typeInfo);
                string typeData = currentRow["TypeData"].ToString();

                switch (fieldType)
                {
                    case FieldType.Text:
                        {
                            rTextType.Checked = true;
                            LoadTextFieldInfo(typeData);

                            break;
                        }
                    case FieldType.Number:
                        {
                            rNumberType.Checked = true;
                            LoadNumberFieldInfo(typeData);

                            break;
                        }
                    case FieldType.Boolean:
                        {
                            rBooleanType.Checked = true;

                            break;
                        }
                    case FieldType.List:
                        {
                            rListType.Checked = true;
                            LoadListFieldInfo(typeData);

                            break;
                        }
                    case FieldType.File:
                        {
                            rFileType.Checked = true;
                            LoadFileFieldInfo(typeData);

                            break;
                        }
                    case FieldType.Custom:
                        {
                            rCustomType.Checked = true;
                            break;
                        }
                }
            }

            fieldNameBox.Text = fieldName;
            fieldDescriptionBox.Text = fieldDescription;
            fieldInfoChanged = true;
        }

        /// <summary>
        /// ���ؼ�¼
        /// </summary>
        private void LoadRecord()
        {
            Node currentNode = guideTree.SelectedNode;

            if (currentNode != null)
            {
                string id = currentNode.Tag as string;

                switch (currentNode.Level)
                {
                    case 0:
                        {
                            configControl.SelectedTabIndex = 3;
                            break;
                        }
                    case 1:
                        {
                            LoadClassInfo(id);
                            configControl.SelectedTabIndex = 0;

                            break;
                        }
                    case 2:
                        {
                            LoadTabInfo(id);
                            configControl.SelectedTabIndex = 1;

                            break;
                        }
                    case 3:
                        {
                            LoadFieldInfo(id);
                            configControl.SelectedTabIndex = 2;

                            break;
                        }
                }
            }
        }

        /// <summary>
        /// ɾ����¼
        /// </summary>        
        private void DeleteRecord()
        {
            Node currentNode = guideTree.SelectedNode;

            if (currentNode != null)
            {
                string id = currentNode.Tag as string;

                switch (currentNode.Level)
                {
                    case 1: // ��
                        {
                            List<Node> tabNodeList = new List<Node>();

                            foreach (Node tabNode in currentNode.Nodes)
                            {
                                tabNodeList.Add(tabNode);
                            }

                            foreach (Node tabNode in tabNodeList)
                            {
                                List<Node> fieldNodeList = new List<Node>();

                                foreach (Node fieldNode in tabNode.Nodes)
                                {
                                    fieldNodeList.Add(fieldNode);
                                }

                                foreach (Node fieldNode in fieldNodeList)
                                {
                                    DeleteRecord(fieldNode, fieldTableName);
                                }

                                DeleteRecord(tabNode, tabTableName);
                            }

                            DeleteRecord(currentNode, classTableName);

                            break;
                        }
                    case 2: // ��ҳ
                        {
                            List<Node> fieldNodeList = new List<Node>();
                            
                            foreach (Node fieldNode in currentNode.Nodes)
                            {
                                fieldNodeList.Add(fieldNode);
                            }
                            
                            foreach (Node fieldNode in fieldNodeList)
                            {
                                DeleteRecord(fieldNode, fieldTableName);
                            }

                            DeleteRecord(currentNode, tabTableName);

                            break;
                        }
                    case 3: // ѡ��
                        {
                            DeleteRecord(currentNode, fieldTableName);

                            break;
                        }
                }
            }
        }

        /// <summary>
        /// ɾ����¼
        /// </summary>
        /// <param name="node">��ǰ�����</param>
        /// <param name="tableName">���ݱ���</param>
        private void DeleteRecord(Node node, string tableName)
        {
            string id = node.Tag as string;
            string sqlString = string.Format("DELETE FROM {0} WHERE ID = {1}",
                                             tableName, id);

            ExecuteSqlCommand(sqlString);            
            node.Remove();
        }

        /// <summary>
        /// ���м�¼
        /// </summary>
        /// <param name="tableName">���ݱ���</param>
        private void SortRecord(string tableName)
        {
            Node currentNode = guideTree.SelectedNode;

            if (currentNode != null)
            {
                List<string> recordList = new List<string>();
                Dictionary<string, string> idDictionary = new Dictionary<string, string>();

                foreach (Node childNode in currentNode.Nodes)
                {
                    string nodeName = childNode.Text;
                    string nodeID = childNode.Tag as string;
                    idDictionary[nodeName] = nodeID;

                    recordList.Add(nodeName);
                }

                SortForm sortForm = new SortForm(recordList);

                if (sortForm.ShowDialog() == DialogResult.OK)
                {
                    // ���ݼ�¼����
                    List<string> idList = new List<string>();

                    foreach (string recordName in sortForm.RecordList)
                    {
                        idList.Add(idDictionary[recordName]);
                    }

                    SetRecordOrder(tableName, idList);

                    // ���������
                    Node[] nodeArray = new Node[idList.Count];

                    foreach (Node childNode in currentNode.Nodes)
                    {
                        string nodeID = childNode.Tag as string;
                        int index = idList.IndexOf(nodeID);
                        nodeArray[index] = childNode;
                    }

                    currentNode.Nodes.Clear();

                    foreach (Node node in nodeArray)
                    {
                        currentNode.Nodes.Add(node);
                    }
                }
            }
        }

        /// <summary>
        /// ���ü�¼��˳��
        /// </summary>
        /// <param name="tableName">���ݱ���</param>
        /// <param name="idList">ID����</param>
        private void SetRecordOrder(string tableName, List<string> idList)
        {
            try
            {
                OpenConnection();

                SqlCommand cmd = conn.CreateCommand();
                string sqlString = null;

                for (int i = 0; i < idList.Count; i++)
                {
                    string recordID = idList[i];
                    sqlString = string.Format("UPDATE {0} SET OrderIndex = {1} WHERE ID = {2}",
                                              tableName, i, recordID);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                }
            }
            catch (SqlException ex)
            {
                PrintExceptionMessage(ex);
            }
            finally
            {
                CloseConnection();
            }
        }

        /// <summary>
        /// �½���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddClass_Click(object sender, EventArgs e)
        {
            Node currentNode = guideTree.SelectedNode;

            if (currentNode != null)
            {
                string nodeName = string.Format("��{0}", currentNode.Nodes.Count + 1);
                AddNodeForm addNodeForm = new AddNodeForm("�½���", nodeName, "");

                if (addNodeForm.ShowDialog() == DialogResult.OK)
                {
                    nodeName = addNodeForm.NodeName;
                    string nodeDescription = addNodeForm.NodeDescription;
                    string id = null;

                    SqlTransaction transaction = null;                    

                    try
                    {
                        OpenConnection();

                        transaction = conn.BeginTransaction();
                        SqlCommand cmd = conn.CreateCommand();
                        cmd.Transaction = transaction;

                        string sqlString = string.Format("SELECT ID FROM {0} WHERE Name = '{1}'", 
                                                         classTableName, nodeName);
                        cmd.CommandText = sqlString;

                        object executeResult = cmd.ExecuteScalar();

                        if (executeResult == null)
                        {
                            sqlString = string.Format("INSERT INTO {0} (Name, Description) VALUES ('{1}', '{2}')",
                                                      classTableName, nodeName, nodeDescription);
                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();

                            sqlString = string.Format("SELECT ID FROM {0} WHERE Name = '{1}'",
                                                      classTableName, nodeName);
                            cmd.CommandText = sqlString;
                            executeResult = cmd.ExecuteScalar();
                            id = executeResult.ToString();
                        }

                        transaction.Commit();
                    }
                    catch (SqlException ex)
                    {
                        if (transaction != null)
                        {
                            transaction.Rollback();
                        }

                        PrintExceptionMessage(ex);
                    }
                    finally
                    {
                        CloseConnection();
                    }

                    if (id != null)
                    {
                        Node newNode = new Node();
                        newNode.Text = nodeName;
                        newNode.Tag = id;
                        newNode.ContextMenu = bClassMenu;
                        currentNode.Nodes.Add(newNode);
                        guideTree.SelectedNode = newNode;
                    }
                }                                
            }
        }

        /// <summary>
        /// �½���ҳ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddTab_Click(object sender, EventArgs e)
        {
            Node currentNode = guideTree.SelectedNode;

            if (currentNode != null)
            {
                int orderIndex = currentNode.Nodes.Count;
                string nodeName = string.Format("��ҳ{0}", orderIndex + 1);
                AddNodeForm addNodeForm = new AddNodeForm("�½���ҳ", nodeName, "");

                if (addNodeForm.ShowDialog() == DialogResult.OK)
                {
                    nodeName = addNodeForm.NodeName;
                    string nodeDescription = addNodeForm.NodeDescription;
                    string classID = currentNode.Tag as string;
                    string id = null;

                    SqlTransaction transaction = null;

                    try
                    {
                        OpenConnection();

                        transaction = conn.BeginTransaction();
                        SqlCommand cmd = conn.CreateCommand();
                        cmd.Transaction = transaction;

                        string sqlString = string.Format("SELECT ID FROM {0} WHERE Name = '{1}'", 
                                                         tabTableName, nodeName);
                        cmd.CommandText = sqlString;

                        object executeResult = cmd.ExecuteScalar();

                        if (executeResult == null)
                        {
                            sqlString = string.Format("INSERT INTO {0} (Name, Description, ClassID, OrderIndex) VALUES ('{1}', '{2}', {3}, {4})",
                                                      tabTableName, nodeName, nodeDescription, classID, orderIndex);
                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();

                            sqlString = string.Format("SELECT ID FROM {0} WHERE Name = '{1}'",
                                                      tabTableName, nodeName);
                            cmd.CommandText = sqlString;
                            executeResult = cmd.ExecuteScalar();
                            id = executeResult.ToString();
                        }

                        transaction.Commit();
                    }
                    catch (SqlException ex)
                    {
                        if (transaction != null)
                        {
                            transaction.Rollback();
                        }

                        PrintExceptionMessage(ex);
                    }
                    finally
                    {
                        CloseConnection();
                    }

                    if (id != null)
                    {
                        Node newNode = new Node();
                        newNode.Text = nodeName;
                        newNode.Tag = id;
                        newNode.ContextMenu = bTabMenu;
                        currentNode.Nodes.Add(newNode);
                        guideTree.SelectedNode = newNode;
                    }
                }
            }
        }

        /// <summary>
        /// �½�ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddField_Click(object sender, EventArgs e)
        {
            Node currentNode = guideTree.SelectedNode;

            if (currentNode != null)
            {
                int orderIndex = currentNode.Nodes.Count;
                string nodeName = string.Format("ѡ��{0}", orderIndex + 1);
                AddNodeForm addNodeForm = new AddNodeForm("�½�ѡ��", nodeName, "");

                if (addNodeForm.ShowDialog() == DialogResult.OK)
                {
                    nodeName = addNodeForm.NodeName;
                    string nodeDescription = addNodeForm.NodeDescription;
                    string tabID = currentNode.Tag as string;
                    string id = null;

                    SqlTransaction transaction = null;

                    try
                    {
                        OpenConnection();

                        transaction = conn.BeginTransaction();
                        SqlCommand cmd = conn.CreateCommand();
                        cmd.Transaction = transaction;

                        string sqlString = string.Format("SELECT ID FROM {0} WHERE Name = '{1}'", 
                                                         fieldTableName, nodeName);
                        cmd.CommandText = sqlString;

                        object executeResult = cmd.ExecuteScalar();

                        if (executeResult == null)
                        {
                            sqlString = string.Format("INSERT INTO {0} (Name, Description, TabID, OrderIndex) VALUES ('{1}', '{2}', {3}, {4})",
                                                      fieldTableName, nodeName, nodeDescription, tabID, orderIndex);
                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();

                            sqlString = string.Format("SELECT ID FROM {0} WHERE Name = '{1}'",
                                                      fieldTableName, nodeName);
                            cmd.CommandText = sqlString;
                            executeResult = cmd.ExecuteScalar();
                            id = executeResult.ToString();
                        }

                        transaction.Commit();
                    }
                    catch (SqlException ex)
                    {
                        if (transaction != null)
                        {
                            transaction.Rollback();
                        }

                        PrintExceptionMessage(ex);
                    }
                    finally
                    {
                        CloseConnection();
                    }

                    if (id != null)
                    {
                        Node newNode = new Node();
                        newNode.Text = nodeName;
                        newNode.Tag = id;
                        newNode.ContextMenu = bFieldMenu;
                        currentNode.Nodes.Add(newNode);
                        guideTree.SelectedNode = newNode;
                    }
                }
            }
        }

        /// <summary>
        /// ���з�ҳ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bSortTab_Click(object sender, EventArgs e)
        {
            SortRecord(tabTableName);
        }

        /// <summary>
        /// ����ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bSortField_Click(object sender, EventArgs e)
        {
            SortRecord(fieldTableName);
        }

        /// <summary>
        /// ˢ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRefresh_Click(object sender, EventArgs e)
        {
            ReloadClassTable();
            ReloadTabTable();
            ReloadFieldTable();
            ReloadTree();
        }

        /// <summary>
        /// ɾ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRemoveClass_Click(object sender, EventArgs e)
        {
            DeleteRecord();
        }

        /// <summary>
        /// ɾ����ҳ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRemoveTab_Click(object sender, EventArgs e)
        {
            DeleteRecord();
        }

        /// <summary>
        /// ɾ��ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDeleteField_Click(object sender, EventArgs e)
        {
            DeleteRecord();
        }

        /// <summary>
        /// ������Ϣ�ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void OnClassInfoChanged(object sender, EventArgs e)
        {
            classInfoChanged = true;
        }

        /// <summary>
        /// ��ҳ��Ϣ�ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void OnTabInfoChanged(object sender, EventArgs e)
        {
            tabInfoChanged = true;
        }

        /// <summary>
        /// ѡ����Ϣ�ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void OnFieldInfoChanged(object sender, EventArgs e)
        {
            fieldInfoChanged = true;
        }

        /// <summary>
        /// ѡ�������֮��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void guideTree_AfterNodeSelect(object sender, AdvTreeNodeEventArgs e)
        {
            LoadRecord();
            lastSelectedNode = guideTree.SelectedNode;
        }

        /// <summary>
        /// ѡ�������֮ǰ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void guideTree_BeforeNodeSelect(object sender, AdvTreeNodeCancelEventArgs e)
        {
            SaveRecord();
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bOK_Click(object sender, EventArgs e)
        {
            SaveRecord();
        }

        /// <summary>
        /// �ر���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// �༭��ҳ�ű�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bEditScript_Click(object sender, EventArgs e)
        {
            Node currentNode = guideTree.SelectedNode;
            string id = currentNode.Tag as string;
            string scriptText = "";

            SqlCommand cmd;
            string sqlString;
            byte[] buffer;

            try
            {
                OpenConnection();

                cmd = conn.CreateCommand();
                sqlString = string.Format("SELECT Script FROM {0} WHERE ID = {1}",
                                          tabTableName, id);                
                cmd.CommandText = sqlString;
                object executeResult = cmd.ExecuteScalar();

                if (executeResult != null && !(executeResult is DBNull))
                {
                    buffer = executeResult as byte[];
                    scriptText = Encoding.Unicode.GetString(buffer);
                }
            }
            catch (SqlException ex)
            {
                PrintExceptionMessage(ex);
            }
            finally
            {
                CloseConnection();
            }

            ScriptEditForm scriptEditForm = new ScriptEditForm(scriptText);

            if (scriptEditForm.ShowDialog() == DialogResult.OK)
            {
                scriptText = scriptEditForm.ScriptText;
                buffer = Encoding.Unicode.GetBytes(scriptText);

                try
                {
                    OpenConnection();

                    cmd = conn.CreateCommand();
                    SqlParameter sqlParameter = new SqlParameter("@data", buffer);
                    cmd.Parameters.Add(sqlParameter);
                    sqlString = string.Format("UPDATE {0} SET Script = @data WHERE ID = {1}",
                                              tabTableName, id);
                    cmd.CommandText = sqlString;
                    cmd.ExecuteScalar();
                }
                catch(SqlException ex)
                {
                    PrintExceptionMessage(ex);
                }
                finally
                {
                    CloseConnection();
                }
            }
        }        
    }
}