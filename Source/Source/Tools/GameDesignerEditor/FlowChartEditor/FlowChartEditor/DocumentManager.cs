using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Data;
using System.Collections;
using System.IO;
using System.ComponentModel;

using DevComponents.DotNetBar;
using DevComponents.DotNetBar.Controls;

using FlowChartEditor.BaseGraphElement;
using FlowChartEditor.Command.TreeCommand;
using DBManager;
using GraphElementLibrary;
using DataElementLibrary;
using FlowChartDataLibrary;
using DataInteractor;

namespace FlowChartEditor
{
    public class DocumentManager
    {
        // ������
        private static DocumentManager documentManager; // Ψһ�ķ���������ʵ��
        private DataBaseManager dataBaseManager; // ���ݿ������ʵ��            
        private Helper helper = Helper.GetHelper(); // ϵͳ��������   
        private LogRecorder logRecorder = LogRecorder.GetLogRecorder(); // ��־����������  
        private Interactor interactor = Interactor.GetInteractor(); // ����������ʵ��
        private FlowChartManager flowChartManager; // ��ͼ������ 
        
        // ����
        private TreeNode cutNode; // ��ǰ���е������
        private TreeNode copyNode; // ��ǰ���Ƶ������
        private string cutTreeNodePath; // ���е�������·��        
        private TreeNode selectedNode; // ��ǰѡ�е������        
        private Hashtable flowChartManagerTable = new Hashtable(); // ͼԪ��������ϣ��                       
        private List<string> mapList; // ��ͼ����    
        private Hashtable fileTable = new Hashtable(); // ���浼���ļ����ݵĹ�ϣ��
        private List<string> sqlCommandList = new List<string>(); // ����sql���������
        private Hashtable copyTable; // �������ݵĹ�ϣ��
        private Hashtable diagramInfoTable = new Hashtable(); // ��ͼ��Ϣ��ϣ��  
        private int backupIDOffset = 10000; // ��ͼ����IDƫ��
        
        // ����
        private SearchForm searchForm = new SearchForm(); // ���Ҵ���
        private InnerChartReferenceForm innerChartReferenceForm = new InnerChartReferenceForm(); // �ӻ�ͼ���ù�ϵ����

        // ״̬
        private EditMode treeMode; // ��ǰ�����Ĵ���ģʽ
        private string chartMode; // ��ǰ����ı༭ģʽ

        // �ؼ�
        private AbbreviateCanvas abbreviateCanvas; // ����ͼ��ͼ��
        private DevComponents.DotNetBar.TabControl tabControl; // ��ͼ�������
        private DevComponents.DotNetBar.TabControl treeTabControl; // �������������    
        private PropertyGrid propertyGrid; // ���Կؼ�
        private TextBoxX logBox; // ��־�ؼ�
        private ButtonItem redoButton; // ������ť
        private ButtonItem undoButton; // ������ť
        private ButtonItem commandSwitchButton; // ����״̬�л���ť
        private ButtonItem treeMenu; // �Ҽ��˵�
        private ExpandablePanel treePanel; // ���������
        private ImageList treeImageList; // ��������ͼƬ����     
        private ComboBoxItem scaleComboBox; // �������������
        private List<ButtonItem> diagramRecordButtonList = new List<ButtonItem>(); // ��¼�򿪻�ͼ��ť������

        /// <summary>
        /// ˽�й��캯������������ģʽ
        /// </summary>
        private DocumentManager()
        {

        }

        /// <summary>
        /// �������������
        /// </summary>
        public ComboBoxItem ScaleComboBox
        {
            set
            {
                scaleComboBox = value;
            }
        }

        /// <summary>
        /// �������ݵĹ�ϣ��
        /// </summary>
        public Hashtable CopyTable
        {
            set
            {
                copyTable = value;
            }

            get
            {
                return copyTable;
            }
        }

        /// <summary>
        /// ��ǰ������
        /// </summary>
        public TreeView CurrentTree
        {
            get
            {
                return treeTabControl.SelectedTab.Tag as TreeView;
            }
        }

        /// <summary>
        /// ���浼���ļ����ݵĹ�ϣ��
        /// </summary>
        public Hashtable FileTable
        {
            get
            {
                return fileTable;
            }
        }

        /// <summary>
        /// ��ǰ��ͼ������
        /// </summary>
        public FlowChartManager CurrentFlowChartManager
        {
            get
            {
                return flowChartManager;
            }
        }

        /// <summary>
        /// ���е�������·��
        /// </summary>
        public string CutTreeNodePath
        {
            set
            {
                cutTreeNodePath = value;
            }
        }

        /// <summary>
        /// �������������
        /// </summary>
        public DevComponents.DotNetBar.TabControl TreeTabControl
        {
            set
            {
                treeTabControl = value;
            }
        }

        /// <summary>
        /// ��������ͼƬ����
        /// </summary>
        public ImageList TreeImageList
        {
            set
            {
                treeImageList = value;
            }
        }
        
        /// <summary>
        /// ��ǰ����ı༭ģʽ
        /// </summary>
        public string CurrentChartMode 
        {
            get
            {
                return chartMode;
            }
        }

        /// <summary>
        /// ���������
        /// </summary>
        public ExpandablePanel TreePanel
        {
            set
            {
                treePanel = value;
            }
        }

        /// <summary>
        /// �������˵�
        /// </summary>
        public ButtonItem TreeMenu
        {
            set
            {
                treeMenu = value;
            }
        }

        /// <summary>
        /// ��־�ؼ�
        /// </summary>
        public TextBoxX LogBox
        {
            set
            {
                logBox = value;
            }
        }

        /// <summary>
        /// ���Կؼ�
        /// </summary>
        public PropertyGrid CurrentPropertyGrid
        {
            set
            {
                propertyGrid = value;
            }
        }

        /// <summary>
        /// ����ͼ��ͼ��
        /// </summary>
        public AbbreviateCanvas CurrentAbbreviateCanvas
        {
            set
            {
                abbreviateCanvas = value;
            }
        }

        /// <summary>
        /// ��ͼ�������
        /// </summary>
        public DevComponents.DotNetBar.TabControl CurrentTabControl
        {
            set
            {
                tabControl = value;
            }
        }

        /// <summary>
        /// ��ǰ���Ƶ������
        /// </summary>
        public TreeNode CopyTreeNode
        {
            set
            {
                copyNode = value;
            }

            get
            {
                return copyNode;
            }
        }

        /// <summary>
        /// ��ǰ���е������
        /// </summary>
        public TreeNode CutTreeNode
        {
            set
            {
                cutNode = value;
            }

            get
            {
                return cutNode;
            }
        }

        /// <summary>
        /// ��ǰ��������״̬
        /// </summary>
        public EditMode CurrentTreeMode
        {
            set
            {
                treeMode = value;
            }

            get
            {
                return treeMode;
            }
        }

        /// <summary>
        /// ������ť
        /// </summary>
        public ButtonItem RedoButton
        {
            set
            {
                redoButton = value;
            }
        }

        /// <summary>
        /// ������ť
        /// </summary>
        public ButtonItem UndoButton
        {
            set
            {
                undoButton = value;
            }
        }

        /// <summary>
        /// �����л���ť
        /// </summary>
        public ButtonItem CommandSwitchButton
        {
            set
            {
                commandSwitchButton = value;
            }
        }

        /// <summary>
        /// ��ӻ�ͼ��¼��ť
        /// </summary>
        /// <param name="buttonItem">��ͼ��¼��ť</param>
        public void AddDiagramRecordButton(ButtonItem buttonItem)
        {
            diagramRecordButtonList.Add(buttonItem);
            buttonItem.Tag = diagramRecordButtonList.Count.ToString();
        }
    
        /// <summary>
        /// ��ȡ�����������ʵ��
        /// </summary>
        /// <returns>�����������ʵ��</returns>
        public static DocumentManager GetDocumentManager()
        {
            if(documentManager == null)
            {
                documentManager = new DocumentManager();
            }

            return documentManager;
        }

        /// <summary>
        /// ��ʼ����
        /// </summary>
        public void BeginSearch()
        {
            if (flowChartManager != null)
            {
                searchForm.CurrentFlowChartManager = flowChartManager;
                searchForm.Show();
                searchForm.Focus();
            }
            else
            {
                MessageBox.Show("���ȴ�Ҫ���ҵĻ�ͼ��", "����", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �л�����״̬
        /// </summary>
        public void SwitchCommandState()
        {
            if (flowChartManager != null)
            {
                flowChartManager.SupportCommand = !flowChartManager.SupportCommand;

                if (flowChartManager.SupportCommand)
                {
                    commandSwitchButton.Text = "�ر�����֧��";
                    RefreshCommandButton();
                }
                else
                {
                    commandSwitchButton.Text = "��������֧��";
                    flowChartManager.CurrentGraphManager.ClearCommandList();
                }
            }
            else
            {
                MessageBox.Show("���ȴ�Ҫ�л�����֧��״̬�Ļ�ͼ��", "�л�����֧��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }          

        /// <summary>
        /// �鿴�ӻ�ͼ���ù�ϵ
        /// </summary>
        public void ViewInnerChartReference()
        {
            innerChartReferenceForm.RefreshData();
            innerChartReferenceForm.Show();
            innerChartReferenceForm.Focus();
        }

        /// <summary>
        /// ��������
        /// </summary>
        public void EditVar()
        {
            // ִ���߼�����
            LogicBaseManager logicManager = helper.GetLogicManager(chartMode);
            logicManager.DoLogicOperation(flowChartManager, LogicType.ManageArguments, null);                                
        }

        /// <summary>
        /// ѡ�����е�ͼԪ
        /// </summary>
        public void SelectAllGraphElement()
        {
            if (flowChartManager != null)
            {
                flowChartManager.CurrentGraphManager.SelectAllGraphElement();
            }
        }

        /// <summary>
        /// ����ڷ����������¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void tree_MouseDown(object sender, MouseEventArgs e)
        {
            switch (e.Button)
            {
                case MouseButtons.Left: // ���������
                    {
                        TreeView tree = sender as TreeView;                                          

                        // �ȼ�鵱ǰ�Ƿ�������㱻ѡ��
                        foreach (TreeNode node in tree.Nodes)
                        {
                            CheckNodeSelected(node, e.Location);
                        }

                        if (selectedNode != null)
                        {
                            tree.SelectedNode = selectedNode;
                            ShowDiagramProperty(selectedNode);
                        }
                        
                        break;
                    }
                case MouseButtons.Right: // ����Ҽ����
                    {
                        PopTreeNodeMenu(e.Location);

                        break;
                    }
            }
        }

        /// <summary>
        /// ��ʾ��ͼ������
        /// </summary>        
        /// <param name="node">��ǰ�����</param>
        private void ShowDiagramProperty(TreeNode node)
        {
            string id = node.Tag.ToString();
            DiagramInfo info = diagramInfoTable[id] as DiagramInfo;
            propertyGrid.SelectedObject = info;
        }

        /// <summary>
        /// ˫�����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void tree_NodeMouseDoubleClick(object sender, MouseEventArgs e)
        {
            AddNewPanel();            
        }

        /// <summary>
        /// ˢ��δ��ʼ������Ԫ��ͼԪ������
        /// </summary>
        /// <param name="dataManager">���ݹ�����ʵ��</param>
        /// <param name="graphElement">ͼԪ����</param>
        /// <param name="data">����</param>
        private void ReloadData(DataManager dataManager, GraphElement graphElement, object data)
        {
            DataElement dataElement = dataManager.InitDataElement(graphElement);
            Hashtable table = new Hashtable();
            table["globe_args"] = dataManager.GlobeArgs;

            if (dataElement != null)
            {
                dataElement.Data = data;
                dataElement.ReloadData(table);
            }
        }
        
        /// <summary>
        /// ��ȡ��ͼ�������ַ���
        /// </summary>
        /// <param name="id">��ͼID</param>
        /// <param name="diagramTable">��ͼ��Ϣ���ݱ�</param>
        /// <returns>��ͼ�������ַ���</returns>
        private string GetDiagramDescription(string id, DataTable diagramTable)
        {
            string description = null;

            DataRow[] rows = diagramTable.Select(string.Format("ID = '{0}'", id));
            if (rows.Length > 0)
            {
                string diagramPath = rows[0]["FullPath"].ToString();
                string diagramClass = rows[0]["Class"].ToString();

                description = string.Format("{0} {1}", diagramClass, diagramPath);
            }

            return description;
        }

        /// <summary>
        /// �����ͼ���ݵ������ļ�
        /// </summary>
        public void SaveDiagramDataToFile()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;

            if (tree != null)
            {
                TreeNode currentNode = tree.SelectedNode;

                if (currentNode != null)
                {
                    SaveFileDialog saveFileDialog = new SaveFileDialog();
                    saveFileDialog.Title = "�����ͼ����";
                    saveFileDialog.Filter = "��ͼ����|*.Diagram";
                    saveFileDialog.InitialDirectory = Application.StartupPath;
                    saveFileDialog.FileName = string.Format("{0}.Diagram", currentNode.Text);
                    
                    if (saveFileDialog.ShowDialog() == DialogResult.OK)
                    {
                        string fileName = saveFileDialog.FileName;
                        string id = currentNode.Tag.ToString();
                        object data = dataBaseManager.GetDiagramData(id);
                        bool saveSuccess = false;

                        if (data != null)
                        {
                            saveSuccess = dataBaseManager.SaveDiagramDataToFile(fileName, data);
                        }                        

                        if (saveSuccess)
                        {
                            MessageBox.Show("��ͼ���ݱ���ɹ���", "�����ͼ����",
                                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                        }
                    }                                                           
                }
                else
                {
                    MessageBox.Show("����ѡ��Ҫ���浽�ļ��Ļ�ͼ��", "�����ͼ���� ",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        /// <summary>
        /// �ӱ����ļ����ػ�ͼ����
        /// </summary>
        public void LoadDiagramDataFromFile()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;

            if (tree != null)
            {
                TreeNode currentNode = tree.SelectedNode;

                if (currentNode != null)
                {
                    OpenFileDialog openFileDialog = new OpenFileDialog();
                    openFileDialog.Title = "���ػ�ͼ����";
                    openFileDialog.Filter = "��ͼ����|*.Diagram";
                    openFileDialog.InitialDirectory = Application.StartupPath;
                    openFileDialog.FileName = string.Format("{0}.Diagram", currentNode.Text);

                    if (openFileDialog.ShowDialog() == DialogResult.OK)
                    {
                        string fileName = openFileDialog.FileName;
                        string id = currentNode.Tag.ToString();
                        object data = dataBaseManager.LoadDiagramDataFromFile(fileName);

                        if (data != null)
                        {                            
                            string panelName = currentNode.Text;
                            string path = GetNodePath(currentNode);

                            OpenDiagram(panelName, id, path, false, data); 
                        }
                    }
                }
                else
                {
                    MessageBox.Show("����ѡ��Ҫ���ļ������ļ��Ļ�ͼ��", "���ػ�ͼ���� ",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        /// <summary>
        /// ��ʼ�����������
        /// </summary>
        public void Init()
        {
            // ����ļ�����
            if (helper.CheckRequireUpdate())
            {
                MessageBox.Show("�༭���ļ��и��£�������UpdateProgram.exe��ȡ�����ļ���", "������", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            // ��ʼ�����ݿ������
            dataBaseManager = DataBaseManager.GetDataBaseManager();

            // ��յ�ǰ�û���������
            dataBaseManager.ClearRecordLocks();

            // ��ȡ��ͼ����
            mapList = dataBaseManager.GetMapList();

            // ��ȡ�û�������Ϣ
            string ip = helper.GetHostAddress();
            object userInfo = dataBaseManager.GetUserConfigInfo(ip);
            Hashtable info = null;
            bool infoExist = false;

            if (userInfo is Hashtable)
            {
                info = userInfo as Hashtable;                                 
                infoExist = true;                
            }
            else if (userInfo is object[]) // ���ݾ�����
            {
                object[] oldInfo = userInfo as object[];
                info = new Hashtable();
                info["OutPutDir"] = oldInfo[0];
                info["ShowRemark"] = oldInfo[1];
                info["AutoAdjustLine"] = oldInfo[2];
                info["AutoAdjustGrid"] = oldInfo[3];
                infoExist = true;
            }            

            if (infoExist)
            {
                helper.SetArguments(info);

                // ��������򿪻�ͼ����
                DataTable diagramTable = dataBaseManager.GetDiagramInformation(null);
                List<int> latestDiagramList = info["LatestDiagramList"] as List<int>;                

                if (latestDiagramList != null)
                {
                    for (int i = 0; i < diagramRecordButtonList.Count; i++)
                    {
                        if (i < latestDiagramList.Count)
                        {
                            string diagramID = helper.LatestDiagramList[i].ToString();
                            string diagramInformation = GetDiagramDescription(diagramID, diagramTable);

                            if (diagramInformation != null)
                            {
                                diagramRecordButtonList[i].Text = diagramInformation;
                                diagramRecordButtonList[i].Tag = diagramID;
                            }
                            else
                            {
                                diagramRecordButtonList[i].Visible = false;
                            }
                        }
                        else
                        {
                            diagramRecordButtonList[i].Visible = false;
                        }
                    }                        
                }
                else
                {
                    foreach (ButtonItem buttonItem in diagramRecordButtonList)
                    {
                        buttonItem.Visible = false;
                    }
                }
            }
            else
            {
                SettingForm sForm = new SettingForm();
                DialogResult result = sForm.ShowDialog();

                if(result != DialogResult.OK)
                {
                    MessageBox.Show("�ͻ���Ŀ¼û�����ã������в��ֹ����޷�ʹ�ã�������������˵��д��������������ã�",
                                    "��������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }

            helper.LogBox = logBox;

            // ��ʼ���߼�������
            helper.InitLogicManager();

            // ��ʼ��Ĭ�ϻ�ͼ����
            chartMode = helper.DiagramNameList[0];

            // ˢ�½�����������ֵ
            interactor.RootDir = helper.OutputDir;
            
            // �����������
            ConstructClassPanel();

            // д��־
            logRecorder.SaveLog("��ʼ������", "�����û����ã���ͼ������Ϣ���Զ���ģ��", true);
        }

        /// <summary>
        /// �����������
        /// </summary>
        public void ConstructClassPanel()
        {
            List<string> list = helper.DiagramNameList;

            foreach (string s in list)
            {                
                TabItem tabItem = new TabItem();
                TabControlPanel tabControlPanel = new TabControlPanel();

                tabControlPanel.SuspendLayout();
                treeTabControl.Controls.Add(tabControlPanel);

                tabControlPanel.Dock = DockStyle.Fill;
                tabControlPanel.Name = "classPanel" + s;
                tabControlPanel.TabItem = tabItem;                
                tabControlPanel.ResumeLayout(false);

                TreeView tree = new TreeView();

                tree.Tag = s;
                tabItem.ImageIndex = 0;

                tree.ImageList = treeImageList;
                ConstructTree(tree, s);
                tree.MouseDown += new MouseEventHandler(tree_MouseDown);
                tree.MouseDoubleClick += new MouseEventHandler(tree_NodeMouseDoubleClick);
                tabControlPanel.Controls.Add(tree);
                tree.Dock = DockStyle.Fill;

                tabItem.AttachedControl = tabControlPanel;
                tabItem.Name = "classTab" + s;
                tabItem.Text = s;
                tabItem.Tag = tree;
                treeTabControl.Tabs.Add(tabItem);                
            }
        }

        /// <summary>
        /// ��󻯻�ͼ��
        /// </summary>
        public void MaximizeCanvas()
        {
            if (flowChartManager != null)
            {
                flowChartManager.CurrentGraphManager.MaximumCanvasSize();
            }
        }

        /// <summary>
        /// �ı��ͼ��Ĵ�С
        /// </summary>
        /// <param name="scaleArgument">���Ų���</param>
        public void ChangeCanvasScale(float scaleArgument)
        {
            if(flowChartManager != null)
            {
                flowChartManager.CurrentGraphManager.SetCanvasScale(scaleArgument);
            }
        }

        /// <summary>
        /// ��ʾ���������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        public void ShowObjectProperty(object o)
        {
            propertyGrid.SelectedObject = o;
        }

        /// <summary>
        /// ��¼��־
        /// </summary>
        public void RecordLog()
        {
            LogManager.LogManager.GetLogManager().WriteLog();
        }

        /// <summary>
        /// �����û���������Ϣ
        /// </summary>
        public void SaveUserConfigInfo()
        {
            bool success = true;
            string ip = helper.GetHostAddress();
            string hostName = helper.GetHostName();
            Hashtable info = helper.GetArguments();
            bool exist = dataBaseManager.CheckUserInfoExist(ip);            

            if(exist) // �û�������Ϣ�Ѿ�����
            {
                success = dataBaseManager.SetUserConfigInfo(ip, info);
            }
            else // ��Ҫ����û�������Ϣ
            {
                success = dataBaseManager.AddUserConfigInfo(ip, hostName, info);
            }

            // д��־
            logRecorder.SaveLog("�����û�������Ϣ", "���浱ǰ�û���������Ϣ", true);

            // д��־
            logRecorder.SaveLog("�˳�����", "�˳�����", true);
        }

        /// <summary>
        /// ����ͼ�Ƿ���Ч
        /// </summary>
        public void CheckFlowChartAvail()
        {
            if(flowChartManager != null)
            {                
                // ִ���߼�����
                LogicBaseManager logicManager = helper.GetLogicManager(flowChartManager.CurrentChartMode);
                GraphManager graphManager = flowChartManager.CurrentGraphManager;
                object[] logicData = new object[]{flowChartManager.CurrentDataManager, flowChartManager.CurrentPath,
                    graphManager.SlotContainerList, graphManager.ConnectorContainerList, logBox, true};
                bool avail = logicManager.DoLogicOperation(flowChartManager, LogicType.CheckLogic, logicData);

                if(avail)
                {
                    MessageBox.Show("��ǰ��ͼ�߼���Ϣ��Ч��", "��ͼ��Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("��ǰû���Ѿ��򿪵Ļ�ͼ��", "��ͼ��Ч�Լ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �л���ͼ���
        /// </summary>
        public void SwitchPanel()
        {
            TabItem currentTab = tabControl.SelectedTab;

            if (currentTab == null) // ��ǰû�м���Ļ�ͼ�༭����
            {
                flowChartManager = null;
                abbreviateCanvas.PaintCanvas = false;
                abbreviateCanvas.CurrentCanvas = null;
                abbreviateCanvas.Invalidate();
                undoButton.SubItems.Clear();
                undoButton.Enabled = false;
                redoButton.Enabled = false;
                commandSwitchButton.Enabled = false;
            }
            else
            {
                if (flowChartManagerTable[currentTab.Tag] != null)
                {
                    // �л���ǰ����Ļ�ͼ
                    FlowChartManager currentFlowChartManager = flowChartManagerTable[currentTab.Tag] as FlowChartManager;
                    flowChartManager = currentFlowChartManager;
                    flowChartManager.CurrentGraphManager.CurrentCanvas.CurrentAbbreviateCanvas = abbreviateCanvas;
                    abbreviateCanvas.CurrentCanvas = flowChartManager.CurrentGraphManager.CurrentCanvas;
                    RefreshCommandButton(); // ˢ�����ť
                    flowChartManager.CurrentGraphManager.CurrentCanvas.Focus();

                    interactor.CurrentFlowChartInteractor = flowChartManager.CurrentFlowChartInteractor;
                    scaleComboBox.WatermarkText = string.Format("{0}%", (int)(GraphSetting.GetGraphSetting().CanvasScale * 100));
                }
                else
                {
                    commandSwitchButton.Enabled = true;                
                    commandSwitchButton.Text = "��������֧��";                
                }                
            }
        }

        /// <summary>
        /// ������ť
        /// </summary>
        public void ClearCommandButton()
        {
            // �������ť
            undoButton.SubItems.Clear();
            SetUndoState(false);
            SetRedoState(false);            
        }

        /// <summary>
        /// ˢ�����ť
        /// </summary>
        private void RefreshCommandButton()
        {
            if (flowChartManager.SupportCommand)
            {
                GraphManager graphManager = flowChartManager.CurrentGraphManager;
                List<string> commandDescriptionList = graphManager.CommandDescriptionList;
                int commandIndex = graphManager.CommandIndex;
                int commandCount = graphManager.CommandCount;

                // ɾ��������ť
                undoButton.SubItems.Clear();                

                // ���ӳ�����ť
                for (int i = commandIndex; i >= 0; i--)
                {                    
                    ButtonItem newButton = new ButtonItem("undoButton" + commandIndex, commandDescriptionList[i]);
                    newButton.Tag = commandIndex.ToString();
                    graphManager.CurrentEventManager.RegistCommandButtonEvent(newButton);                    
                    undoButton.SubItems.Add(newButton);
                }

                // ���ð�ť�Ƿ���Ч
                if ((commandIndex == commandCount - 1) && (commandIndex != -1))
                {
                    SetUndoState(true);                    
                }

                if (commandIndex < commandCount - 1)
                {
                    SetRedoState(true);                    
                }

                commandSwitchButton.Enabled = true;
                commandSwitchButton.Text = "�ر�����֧��";
            }
            else
            {
                if (undoButton.SubItems.Count > 0)
                {
                    undoButton.SubItems.Clear();
                }
                
                SetUndoState(false);
                SetRedoState(false);
                commandSwitchButton.Enabled = true;
                commandSwitchButton.Text = "������֧��";
            }
        }

        /// <summary>
        /// ��ȡ������·��
        /// </summary>
        /// <param name="node">��ǰ�����</param>
        /// <returns>������·��</returns>
        public string GetNodePath(TreeNode node)
        {
            string path = node.Text;
            TreeNode parentNode = node.Parent;

            while (parentNode != null && parentNode.Level > 0)
            {
                path = parentNode.Text + "\\" + path;
                parentNode = parentNode.Parent;
            }

            return path;
        }

        /// <summary>
        /// ��ָ��·���Ļ�ͼ
        /// </summary>
        /// <param name="path">��ͼ·��</param>
        /// <param name="chartMode">��ͼ����</param>
        public void OpenDiagramInPath(string path, string chartMode)
        {            
            TreeNode currentNode = null;            

            foreach (TabItem tabItem in treeTabControl.Tabs)
            {
                if (tabItem.Text == chartMode)
                {
                    TreeView tree = tabItem.Tag as TreeView;
                    List<TreeNode> nodeList = new List<TreeNode>();

                    foreach (TreeNode node in tree.Nodes)
                    {
                        GetAllChildNodes(node, nodeList);
                    }

                    foreach (TreeNode node in nodeList)
                    {
                        if (GetNodePath(node) == path)
                        {
                            currentNode = node;
                            break;
                        }
                    }

                    if (currentNode != null)
                    {
                        break;
                    }
                }                
            }

            if (currentNode != null)
            {
                string panelName = currentNode.Text;
                string nodePath = GetNodePath(currentNode);
                string id = currentNode.Tag.ToString();

                OpenDiagram(panelName, id, nodePath, false, null);
            }
        }

        /// <summary>
        /// �򿪻�ͼ
        /// </summary>
        /// <param name="id">��ͼID</param>        
        public void OpenDiagram(string id)
        {
            TreeNode currentNode = null;

            // ��Ҫ���赱ǰ��ͼ�ı༭״̬
            if (flowChartManager != null)
            {
                flowChartManager.CurrentGraphManager.ResetUserOperation();
            }

            foreach (TabItem tabItem in treeTabControl.Tabs)
            {                
                TreeView tree = tabItem.Tag as TreeView;
                List<TreeNode> nodeList = new List<TreeNode>();

                foreach(TreeNode node in tree.Nodes)
                {
                    GetAllChildNodes(node, nodeList);
                }

                foreach(TreeNode node in nodeList)
                {
                    if (node.Tag.ToString() == id)
                    {
                        currentNode = node;
                        treeTabControl.SelectedTab = tabItem;
                        break;
                    }
                }

                if (currentNode != null)
                {
                    break;
                }
            }

            if (currentNode != null)
            {
                string panelName = currentNode.Text;
                string path = GetNodePath(currentNode);

                OpenDiagram(panelName, id, path, false, null);
            }
        }

        /// <summary>
        /// �鿴�����ĵ�
        /// </summary>
        public void ViewHelpDoc()
        {
            string filePath = Path.Combine(Application.StartupPath, @"Plugins\FlowChart\����ͼ(AIͼ)�ű�����ʹ���ĵ�.doc");
            FileInfo fi = new FileInfo(filePath);
            
            if (fi.Exists)
            {
                try
                {
                    System.Diagnostics.Process.Start(filePath);
                }
                catch (IOException ex)
                {
                    MessageBox.Show("�ڲ鿴�����ĵ�ʱ����IO�쳣��" + ex.ToString());
                }
            }
            else
            {
                MessageBox.Show("�����ĵ������ڣ��볢���Զ������ļ���", "�鿴�����ĵ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
        
        /// <summary>
        /// �򿪻�ͼ
        /// </summary>
        /// <param name="panelName">�������</param>
        /// <param name="id">��ͼID</param>
        /// <param name="path">��ͼ·��</param>        
        /// <param name="backupDiagram">�Ƿ��ǻ�ͼ����</param>
        /// <param name="diagramData">��ͼ����</param>
        private void OpenDiagram(string panelName, string id, string path, bool backupDiagram, object diagramData)
        {
            // ����Ӧid�����û�б���
            foreach (TabItem t in tabControl.Tabs)
            {
                if (t.Tag.ToString() == id) // �����ͼ�༭����
                {
                    tabControl.SelectedTab = t;
                    return;
                }
            }

            // ��ʼ����ͼ�ؼ�
            TabControlPanel tabControlPanel = new TabControlPanel();
            TabItem tabItem = CreateTabItemInstance(id, panelName, tabControlPanel, path);

            // ��ʼ����ͼ��岢���¼�����Ӧ����
            Canvas canvas = CreateCanvasInstance();
            tabControlPanel.Controls.Add(canvas);
            canvas.Dock = DockStyle.Fill;

            // ��¼������򿪵Ļ�ͼ
            if (!backupDiagram)
            {
                helper.RecordLatestDiagram(int.Parse(id));
            }            

            // ��ʼ����ͼ������
            FlowChartManager newFlowChartManager = CreateFlowChartManagerInstance(canvas, tabItem, id, panelName, path);
            flowChartManagerTable[tabItem.Tag] = newFlowChartManager;
            this.tabControl.SelectedTab = tabItem;
            flowChartManager = newFlowChartManager;

            // ˢ�µ�ǰ����Ļ�ͼ
            interactor.CurrentFlowChartInteractor = flowChartManager.CurrentFlowChartInteractor;

            // ���¼�
            EventManager eventManager = new EventManager(flowChartManager.CurrentGraphManager);
            eventManager.RegistCanvasEvent(canvas);
            flowChartManager.CurrentGraphManager.CurrentEventManager = eventManager;

            // ���ػ�ͼ������        
            if (!backupDiagram)
            {
                LoadFlowChart(id, diagramData);
            }
            else
            {
                LoadBackupFlowChart(id);
            }            

            // ��ȡ���� 
            canvas.Focus();

            // ����ͼ���ݵ���Ч��
            flowChartManager.CheckDataValid();
        }

        /// <summary>
        /// ���µ�����д򿪻�ͼĳ����ʷ��¼������
        /// </summary>
        /// <param name="backupID">��ͼ�ļ�¼ID</param>
        /// <param name="diagramVersion">��ͼ�İ汾��</param>
        public void OpenDiaramAtNewPanel(string backupID, string diagramVersion)
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;
            TreeNode currentNode = tree.SelectedNode;

            if (currentNode != null && currentNode.ImageIndex != 0) // ����Ƿ�����Ч�Ļ�ͼ���
            {
                string panelName = string.Format("{0} [{1}]", currentNode.Text, diagramVersion);
                int newID = int.Parse(backupID) + backupIDOffset; // ��������ظ�ID�Ļ�ͼ
                string id = newID.ToString();
                string path = GetNodePath(currentNode);

                OpenDiagram(panelName, id, path, true, null);
            }
        }

        /// <summary>
        /// ����µĻ�ͼ���
        /// </summary>        
        public void AddNewPanel()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;
            TreeNode currentNode = tree.SelectedNode;

            if (currentNode != null && currentNode.ImageIndex != 0) // ����Ƿ�����Ч�Ļ�ͼ���
            {
                string panelName = currentNode.Text;
                string id = currentNode.Tag.ToString();
                string path = GetNodePath(currentNode);

                OpenDiagram(panelName, id, path, false, null);
            }            
        }

        /// <summary>
        /// ������ҳ�ؼ�����
        /// </summary>
        /// <param name="id">��ͼID</param>
        /// <param name="panelName">��ͼ����</param>
        /// <param name="tabControlPanel">��ҳ���ؼ�����</param>
        /// <param name="path">��ͼ·��</param>
        /// <returns>��ҳ�ؼ�����</returns>        
        private TabItem CreateTabItemInstance(string id, string panelName, TabControlPanel tabControlPanel, string path)
        {
            TabItem tabItem = new TabItem();

            tabControlPanel.SuspendLayout();
            tabControl.Controls.Add(tabControlPanel);

            tabControlPanel.Dock = DockStyle.Fill;
            tabControlPanel.Name = "tabControlPanel" + id.ToString();
            tabControlPanel.TabItem = tabItem;
            tabControlPanel.ResumeLayout(false);

            tabItem.AttachedControl = tabControlPanel;
            tabItem.Name = "tabItem" + id.ToString();
            tabItem.Text = panelName;
            tabItem.Tag = id;
            tabItem.Tooltip = path;
            tabItem.ImageIndex = 0;

            this.tabControl.Tabs.Add(tabItem);

            return tabItem;
        }

        /// <summary>
        /// ������ͼ����������
        /// </summary>
        /// <param name="canvas">��ͼ�����</param>
        /// <param name="tabItem">��ҳ�ؼ�����</param>
        /// <param name="id">��ͼID</param>
        /// <param name="panelName">��ͼ����</param>
        /// <param name="path">��ͼ·��</param>
        /// <returns>��ͼ����������</returns>
        private FlowChartManager CreateFlowChartManagerInstance(Canvas canvas, TabItem tabItem, string id, string panelName, string path)
        {
            FlowChartManager newFlowChartManager = new FlowChartManager(chartMode, tabItem, id, panelName, path);
            newFlowChartManager.Init(canvas);

            return newFlowChartManager;
        }

        /// <summary>
        /// ������ͼ�����
        /// </summary>
        /// <return>��ͼ�����</return>
        private Canvas CreateCanvasInstance()
        {
            Canvas canvas = new Canvas();            
            canvas.BackColor = GraphSetting.GetGraphSetting().BackgroundColor;
            canvas.CurrentAbbreviateCanvas = abbreviateCanvas;
            abbreviateCanvas.CurrentCanvas = canvas;

            return canvas;
        }

        /// <summary>
        /// ����Ƿ��е�ǰ����㼰���ӽ������Ӧ�Ļ�ͼ����
        /// </summary>
        /// <param name="node">��ǰ�����</param>
        /// <returns>�Ƿ��л�ͼ����</returns>
        public bool CheckFlowChartOpened(TreeNode node)
        {
            bool opened = false;
            List<TreeNode> nodeList = new List<TreeNode>();

            GetAllChildNodes(node, nodeList);

            foreach(TreeNode treeNode in nodeList)
            {
                foreach (TabItem tabItem in tabControl.Tabs)
                {
                    if (tabItem.Tag.ToString() == treeNode.Tag.ToString())
                    {
                        opened = true;
                        break;
                    }
                }

                if (opened)
                {
                    break;
                }
            }

            return opened;
        }

        /// <summary>
        /// �鿴����վ
        /// </summary>
        public void ViewRecycle()
        {
            
        }

        /// <summary>
        /// ��鵱ǰ����û�з����仯
        /// </summary>
        /// <returns>��ǰ����û�з����仯</returns>
        public bool CheckTreeChanged()
        {
            bool changed = false;

            DataTable newTable = dataBaseManager.GetDiagramInformation(chartMode);
            DataTable treeTable = SaveTree();

            for (int i = 0; i < newTable.Rows.Count; i++)
            {
                DataRow newRow = newTable.Rows[i];                
                string id = newRow["ID"].ToString();                
                DataRow[] rows = treeTable.Select(string.Format("ID = '{0}'", id));

                if (rows.Length > 0) // �ҵ�ƥ���������
                {
                    foreach (DataRow oldRow in rows)
                    {
                        changed = false;

                        if (newRow["FullPath"].ToString() != oldRow["FullPath"].ToString())
                        {
                            changed = true;
                        }

                        if ((bool)newRow["IsFolder"] != (bool)oldRow["IsFolder"])
                        {
                            changed = true;
                        }

                        if (!changed) // ����û�иı�
                        {
                            break;
                        }                    
                    }
                }
                else
                {
                    changed = true;
                }
                
                if (changed) // ���ݸı���
                {
                    break;
                }                               
            }                

            return changed;
        }

        /// <summary>
        /// �л�����
        /// </summary>
        public void SwitchClass()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;            
            chartMode = tree.Tag as string;
            treeMode = EditMode.Normal;
            copyTable = null;
        }

        /// <summary>
        /// ���������
        /// </summary>
        /// <param name="tree">������</param>
        /// <param name="chartClass">����</param>
        public void ConstructTree(TreeView tree, string chartClass)
        {
            DataTable treeTable = dataBaseManager.GetDiagramInformation(chartClass);
            TreeNodeCollection currentNodes;
            TreeNode newNode;

            tree.Nodes.Clear(); // ��������
            treeMode = EditMode.Normal; // ��������״̬

            TreeNode classNode = tree.Nodes.Add("��ͼ����");
            classNode.ImageIndex = 0;
            classNode.SelectedImageIndex = 0;
            classNode.Tag = chartClass;
            diagramInfoTable[chartClass] = new DiagramInfo(chartClass, DateTime.Now, "0", 0);

            foreach (DataRow row in treeTable.Rows)
            {
                string fullPath = row["FullPath"] as string;
                string id = row["ID"].ToString();
                bool isFolder = (bool)row["IsFolder"];
                DateTime editTime = (DateTime)row["EditTime"];
                string hostName = row["Host"].ToString();
                string currentVersion = row["Version"].ToString();

                if (currentVersion == "")
                {
                    currentVersion = "0";
                }

                string[] data = fullPath.Split(new char[] { '\\' });
                currentNodes = classNode.Nodes;

                for (int i = 0; i < data.Length; i++)
                {
                    string classification = data[i];

                    if ((i == data.Length - 1) && !isFolder) // ��ͼ���
                    {
                        newNode = currentNodes.Add(classification);
                        newNode.Name = classification;
                        newNode.Tag = id;
                        newNode.ImageIndex = 1;
                        newNode.SelectedImageIndex = 1;
                        diagramInfoTable[id] = new DiagramInfo(hostName, editTime, currentVersion, 0);
                    }
                    else // ��ͼ������
                    {
                        bool find = false;

                        foreach (TreeNode node in currentNodes)
                        {
                            if (node.Text == classification)
                            {
                                find = true;
                                currentNodes = node.Nodes;
                                break;
                            }
                        }

                        if (!find) // ��ǰ���಻����
                        {
                            StringBuilder path = new StringBuilder();

                            for (int j = 0; j <= i; j++)
                            {
                                path.Append(string.Format("{0}\\", data[j]));                                
                            }

                            path.Remove(path.Length - 1, 1);                            
                            DataRow[] rows = treeTable.Select(string.Format("FullPath = '{0}'", path.ToString()));

                            if (rows.Length > 0)
                            {
                                newNode = currentNodes.Add(classification);
                                newNode.Name = classification;
                                newNode.Tag = rows[0]["ID"];
                                newNode.ImageIndex = 0;
                                newNode.SelectedImageIndex = 0;
                                diagramInfoTable[newNode.Tag.ToString()] = new DiagramInfo(hostName, editTime, currentVersion, 0);

                                currentNodes = newNode.Nodes;
                                diagramInfoTable[id] = new DiagramInfo(hostName, editTime, currentVersion, 0);
                            }
                            else
                            {
                                MessageBox.Show(string.Format("��ͼ������Ϣ��������û���ҵ�·��Ϊ{0}�Ļ�ͼ��", path),
                                                "����������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            }
                        }
                    }
                }
                               
            }

            // ˢ�����ڵ��ӽ�������Ϣ
            RefreshNodeCount(tree);            
        }

        /// <summary>
        /// ˢ��������ӽ�������Ϣ
        /// </summary>
        /// <param name="tree">������</param>
        private void RefreshNodeCount(TreeView tree)
        {
            foreach(TreeNode node in tree.Nodes)
            {
                int childCount = 0;
                RefreshNodeCount(node, ref childCount);
            }
        }

        /// <summary>
        /// ˢ��������ӽ�������Ϣ
        /// </summary>
        /// <param name="node">�����</param>
        /// <param name="childCount">�ӽ�����</param>
        private void RefreshNodeCount(TreeNode node, ref int childCount)
        {
            if (node.ImageIndex == 0) // ���������Ҫˢ���ӽ�����
            {
                int nodeCount = 0;

                foreach (TreeNode childNode in node.Nodes)
                {
                    nodeCount++;
                    RefreshNodeCount(childNode, ref nodeCount);                    
                }                

                DiagramInfo diagramInfo = diagramInfoTable[node.Tag.ToString()] as DiagramInfo;                
                diagramInfo.ReadOnly = false;
                diagramInfo.ChildCount = nodeCount;
                diagramInfo.ReadOnly = true;                
                
                childCount += nodeCount;
            }            
        }

        /// <summary>
        /// ���������
        /// </summary>
        /// <returns>��¼��������Ϣ�����ݱ�</returns>
        private DataTable SaveTree()
        {
            DataTable table = new DataTable("");
            Type stringType = Type.GetType("System.String");
            Type boolType = Type.GetType("System.Boolean");

            table.Columns.Add("ID", stringType);
            table.Columns.Add("FullPath", stringType);
            table.Columns.Add("IsFolder", boolType);
            List<TreeNode> flowChartNodeList = new List<TreeNode>();

            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;

            // �����ҳ����еĻ�ͼ���
            foreach(TreeNode node in tree.Nodes)
            {
                SeekChildNode(node, flowChartNodeList);
            }

            // �������ݱ�
            string id;
            string fullPath;  
            DataRow newRow;

            foreach(TreeNode node in flowChartNodeList)
            {
                id = node.Tag.ToString();
                fullPath = GetNodePath(node);                
                newRow = table.NewRow();
                newRow["FullPath"] = fullPath;
                newRow["ID"] = id;
                newRow["IsFolder"] = (node.ImageIndex == 0);
                table.Rows.Add(newRow);
            }

            // ������е����������������
            if(treeMode == EditMode.Cut)
            {
                id = cutNode.Tag.ToString();
                newRow = table.NewRow();
                newRow["FullPath"] = cutTreeNodePath;
                newRow["ID"] = id;
                newRow["IsFolder"] = (cutNode.ImageIndex == 0);
                table.Rows.Add(newRow);
            }

            return table;
        }

        /// <summary>
        /// �����ӽ��
        /// </summary>
        /// <param name="node">��ǰ�����</param>
        /// <param name="list">�����������</param>
        private void SeekChildNode(TreeNode node, List<TreeNode> list)
        {
            list.Add(node);

            foreach(TreeNode childNode in node.Nodes)
            {
                SeekChildNode(childNode, list);
            }
        }        

        /// <summary>
        /// ����Ǩ��
        /// </summary>
        public void TransferData()
        {
            TransferDataForm transferDataForm = new TransferDataForm();
            transferDataForm.ShowDialog();
        }

        /// <summary>
        /// �ָ���ͼ
        /// </summary>
        public void RestoreDiagram()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;
            TreeNode node = tree.SelectedNode;

            string originID = node.Tag.ToString();
            string sqlString = string.Format("SELECT ID, EditTime, Host, Version FROM DiagramBackup WHERE OriginID = '{0}'", originID);
            DataTable restoreTable = dataBaseManager.GetDataTable(sqlString);

            if (restoreTable.Rows.Count > 0) // �ҵ�������Ϣ
            {
                RestoreForm rForm = new RestoreForm(originID, restoreTable);
                DialogResult result = rForm.ShowDialog();

                if (result == DialogResult.OK)
                {
                    // д��־                    
                    string nodePath = GetNodePath(node);
                    logRecorder.SaveLog("�ָ���ͼ", string.Format("{0}�������ָ���ͼ[{1}]", chartMode, nodePath), true);
                }
            }
            else
            {
                MessageBox.Show("��ǰ��ͼû���ҵ���Ӧ�ı�����Ϣ��", "�ָ���ͼ", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �����µķ���
        /// </summary>
        public void AddClass()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;

            if (CheckTreeChanged()) // ��ǰ���ṹ�������仯
            {
                MessageBox.Show("��ǰĿ¼�ṹ�Ѿ����޸Ĺ�����Ҫ���¼��ظ��£�", "�޸�Ŀ¼",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);

                // ���¼��ط�����
                ConstructTree(tree, chartMode);
            }
            else
            {                
                TreeAddClassCommand cmd = new TreeAddClassCommand(tree, "�½�����");

                if(cmd.Execute(mapList)) // ������������
                {
                    // д��־
                    TreeNode node = tree.SelectedNode;
                    string nodePath = GetNodePath(node);
                    logRecorder.SaveLog("�½�����", string.Format("{0}�������½�����[{1}]", chartMode, nodePath), true);
                }
            }                                             
        }

        /// <summary>
        /// �鿴��ͼ����
        /// </summary>
        public void ViewChartProperty()
        {
            if (flowChartManager != null)
            {
                PropertyForm pForm = new PropertyForm(flowChartManager.CurrentGraphManager.CurrentCanvas.GraphElementList);
                pForm.ShowDialog();
            }
            else
            {
                MessageBox.Show("���ȴ�Ҫ�鿴���ԵĻ�ͼ��", "�鿴��ͼ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ���ó������
        /// </summary>
        public void SetArgument()
        {
            SettingForm sForm = new SettingForm();
            sForm.ShowDialog();
        }

        /// <summary>
        /// �����־�ı�
        /// </summary>
        /// <param name="text">�ı�����</param>
        public void PrintText(string text)
        {
            helper.PrintText(text);            
        }

        /// <summary>
        /// ���ı����浽�ļ���
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <param name="content">�ı�����</param>
        public void SaveTextToFile(string fileName, string content)
        {
            fileTable[fileName] = content;
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        public void CutGraphElement()
        {
            if (flowChartManager != null)
            {
                flowChartManager.CurrentGraphManager.CutGraphElement();
            }
            else
            {
                MessageBox.Show("��ǰû�����ڱ༭�Ļ�ͼ��", "����ͼԪ", MessageBoxButtons.OK,
                                MessageBoxIcon.Information);
            }
            
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        public void CopyGraphElement()
        {
            if(flowChartManager != null)
            {
                flowChartManager.CurrentGraphManager.CopyGraphElement();
            }
            else
            {
                MessageBox.Show("��ǰû�����ڱ༭�Ļ�ͼ��", "����ͼԪ", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }            
        }

        /// <summary>
        /// ճ��ͼԪ
        /// </summary>
        public void PasteGraphElement()
        {
            if(flowChartManager != null)
            {
                flowChartManager.CurrentGraphManager.PasteGraphElement();
            }
            else
            {
                MessageBox.Show("��ǰû�����ڱ༭�Ļ�ͼ��", "ճ��ͼԪ", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �鿴��־
        /// </summary>
        public void ViewLog()
        {
            logRecorder.ViewLog();
        }

        /// <summary>
        /// ˢ��ͼԪ�����ű�����ʾ
        /// </summary>
        public void RefreshCanvasScaleDisplay()
        {
            scaleComboBox.SelectedIndex = -1;
            scaleComboBox.WatermarkText = string.Format("{0}%", (int)(GraphSetting.GetGraphSetting().CanvasScale * 100));
        }

        /// <summary>
        /// ���ɵ�ǰ��ͼ�Ĵ���
        /// </summary>
        public void GenerateCode()
        {
            string outputDir = helper.OutputDir; // ����ļ�Ŀ¼

            if (outputDir != "") // �����ļ�Ŀ¼��Ϊ��
            {
                if(flowChartManager != null) // ��ǰ��ͼ��������Ϊ��
                {
                    GraphManager graphManager = flowChartManager.CurrentGraphManager;
                    DataManager dataManager = flowChartManager.CurrentDataManager;
                    logBox.Text = "";
                    logBox.Refresh(); // ǿ��ˢ��һ��

                    PrintText(string.Format("------ ��ʼ���ػ�ͼ {0} ------", flowChartManager.CurrentPath));
                    PrintText("��ʼ����ͼ�ɹ���");
                    PrintText("------ ��ʼ����߼���Ч�� ------");

                    // ִ���߼�����
                    LogicBaseManager logicManager = helper.GetLogicManager(flowChartManager.CurrentChartMode);
                    bool avail = logicManager.DoLogicOperation(flowChartManager, LogicType.CheckLogic, null);
                    bool success = false;

                    if (avail) // ����ͼ��Ч��
                    {
                        PrintText("�߼���Ч�Լ��ɹ���");

                        // �ȱ��浱ǰ��ͼ
                        if (flowChartManager.ContentChanged)
                        {
                            SaveFlowChart(flowChartManager);
                        }

                        // ִ���߼�����
                        success = logicManager.DoLogicOperation(flowChartManager, LogicType.CompileCode, null);                      
                    }
                    else
                    {
                        PrintText("�߼���Ч�Լ��ʧ�ܣ�");
                    }

                    logRecorder.SaveLog("���ɴ���", string.Format("{0}���������ɻ�ͼ[{1}]�Ĵ���", chartMode, flowChartManager.CurrentPath), success);
                }
                else
                {
                    MessageBox.Show("��ǰû�����ڴ򿪵Ļ�ͼ��", "���ɴ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("����ļ�Ŀ¼����Ϊ�գ�", "���ɴ���", MessageBoxButtons.OK, MessageBoxIcon.Information);
                SettingForm sForm = new SettingForm();
                sForm.ShowDialog();
            } 
        }

        /// <summary>
        /// �༭ע��
        /// </summary>
        public void EditRemark()
        {
            flowChartManager.CurrentGraphManager.EditDataElement();
        }

        /// <summary>
        /// �����ͼ
        /// </summary>
        /// <param name="flowChartManager">Ҫ����Ļ�ͼ</param>
        private void SaveFlowChart(FlowChartManager flowChartManager)
        {
            GraphManager graphManager = flowChartManager.CurrentGraphManager;
            DataManager dataManager = flowChartManager.CurrentDataManager;

            // �Զ�������ͼ��Ĵ�С
            graphManager.AdjustCanvasSize(); 

            // ��֤��ͼ��Ч��
            graphManager.ValidateBeforeSave();
            dataManager.ValidateBeforeSave();

            // �����ͼ
            dataBaseManager.SaveDiagramData(flowChartManager.ID, flowChartManager.GetArguments());
            dataBaseManager.Diagram_Unlock(int.Parse(flowChartManager.ID)); // ����
            flowChartManager.EditTime = dataBaseManager.GetDiagramEditTime(flowChartManager.ID);
            flowChartManager.Version = dataBaseManager.GetDiagramVersion(flowChartManager.ID);
            flowChartManager.ContentChanged = false;            

            // д��־
            logRecorder.SaveLog("�����ͼ", string.Format("{0}�����������ͼ[{1}]", chartMode, flowChartManager.CurrentPath), true);
        }

        /// <summary>
        /// ���ػ�ͼ
        /// </summary>
        /// <param name="id">��ͼID</param>
        /// <param name="data">��ͼ</param>
        private void LoadFlowChart(string id, object data)
        {
            Hashtable args = null; 
            
            if (data == null)
            {
                args = dataBaseManager.GetDiagramData(id) as Hashtable;
            }
            else 
            {
                args = data as Hashtable;
            }

            if (args != null) // ��ͼ���ݲ�Ϊ��
            {
                flowChartManager.SetArguments(args as Hashtable);
            }

            DateTime editTime = dataBaseManager.GetDiagramEditTime(id);
            int version = dataBaseManager.GetDiagramVersion(id);
            flowChartManager.EditTime = editTime;
            flowChartManager.Version = version;

            // ִ���߼�����
            LogicBaseManager logicManager = helper.GetLogicManager(chartMode);
            logicManager.DoLogicOperation(flowChartManager, LogicType.AfterLoad, null);

            // д��־
            logRecorder.SaveLog("���ػ�ͼ", string.Format("{0}���������ػ�ͼ[{1}]", chartMode, flowChartManager.CurrentPath), true);
        }

        /// <summary>
        /// ���ر��ݵĻ�ͼ
        /// </summary>
        /// <param name="backupID">����ID</param>
        private void LoadBackupFlowChart(string backupID)
        {
            int originID = int.Parse(backupID) - backupIDOffset;

            Hashtable args = dataBaseManager.GetBackupDiagramData(originID.ToString()) as Hashtable;

            if (args != null) // ��ͼ���ݲ�Ϊ��
            {
                flowChartManager.SetArguments(args as Hashtable);
            }

            DateTime editTime = dataBaseManager.GetBackupDiagramEditTime(originID.ToString());
            int version = dataBaseManager.GetBackupDiagramVersion(originID.ToString());
            flowChartManager.EditTime = editTime;
            flowChartManager.Version = version;
            flowChartManager.EnableSave = false; // ���ݵĻ�ͼ��������

            // ִ���߼�����
            LogicBaseManager logicManager = helper.GetLogicManager(chartMode);
            logicManager.DoLogicOperation(flowChartManager, LogicType.AfterLoad, null);

            // д��־
            logRecorder.SaveLog("���ػ�ͼ", string.Format("{0}���������ػ�ͼ[{1}]", chartMode, flowChartManager.CurrentPath), true);
        }

        /// <summary>
        /// ��鲢�����ͼ
        /// </summary>
        public void CheckAndSaveFlowChart()
        {
            TabItem currentTab = tabControl.SelectedTab;
            bool cancelSave = false;

            if (currentTab != null)
            {
                FlowChartManager currentFlowChartManager = flowChartManagerTable[currentTab.Tag] as FlowChartManager;

                if (currentFlowChartManager != null && currentFlowChartManager.ContentChanged) // ��ǰ��ͼ���޸Ĺ�����δ����
                {
                    DialogResult result = MessageBox.Show(string.Format("��ͼ{0}��δ���棬�Ƿ����ڱ��棿", currentFlowChartManager.Name), "���ݱ���",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Information);

                    switch (result)
                    {
                        case DialogResult.Yes:
                            {
                                SaveFlowChart(currentFlowChartManager);                                
                                break;
                            }
                        case DialogResult.No:
                            {
                                // ����
                                dataBaseManager.Diagram_Unlock(int.Parse(currentFlowChartManager.ID));
                                break;
                            }
                        case DialogResult.Cancel:
                            {
                                cancelSave = true;
                                break;
                            }
                    }
                }

                if (!cancelSave) // �Ƴ���ǩҳ
                {
                    currentFlowChartManager.CurrentGraphManager.Release(); // �ͷŻ�ͼ�Ļ�ͼ��Դ                    
                    flowChartManagerTable.Remove(currentTab.Tag);
                    tabControl.Tabs.Remove(currentTab);                    
                }
            }
        }

        /// <summary>
        /// ���浱ǰ��ͼ
        /// </summary>
        public void SaveCurrentFlowChart()
        {
            if (flowChartManager != null && flowChartManager.ContentChanged) // ��ǰ��ͼ�Ѿ����޸Ĺ�
            {                
                // ִ���߼�����
                LogicBaseManager logicManager = helper.GetLogicManager(chartMode);
                logicManager.DoLogicOperation(flowChartManager, LogicType.Save, null);

                SaveFlowChart(flowChartManager);                                
            } 
        }

        /// <summary>
        /// ��鲢�����ͼ
        /// </summary>
        public bool CheckAndSaveFlowCharts()
        {
            bool cancelSave = false;

            foreach (object o in flowChartManagerTable.Values)
            {
                if (cancelSave) // ȡ��������ֱ���˳�
                {
                    break;
                }

                FlowChartManager currentFlowChartManager = o as FlowChartManager;

                if (currentFlowChartManager != null && currentFlowChartManager.ContentChanged) // ��ǰ��ͼ���޸Ĺ�����δ����
                {
                    DialogResult result = MessageBox.Show(string.Format("��ͼ{0}��δ���棬�Ƿ����ڱ��棿", currentFlowChartManager.Name), "���ݱ���",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Information);

                    switch (result)
                    {
                        case DialogResult.Yes:
                            {
                                SaveFlowChart(currentFlowChartManager);
                                break;
                            }
                        case DialogResult.No:
                            {
                                // ����
                                dataBaseManager.Diagram_Unlock(int.Parse(currentFlowChartManager.ID));
                                break;
                            }
                        case DialogResult.Cancel:
                            {
                                cancelSave = true;
                                break;
                            }
                    }
                }
            }

            return cancelSave;
        }

        /// <summary>
        /// �����µĻ�ͼ
        /// </summary>
        public void AddFlowChart()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;

            if (CheckTreeChanged()) // ��ǰ���ṹ�������仯
            {
                MessageBox.Show("��ǰĿ¼�ṹ�Ѿ����޸Ĺ�����Ҫ���¼��ظ��£�", "�޸�Ŀ¼",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);

                // ���¼��ط�����               
                ConstructTree(tree, chartMode);
            }
            else
            {                
                TreeAddFlowChartCommand cmd = new TreeAddFlowChartCommand(tree, "�½���ͼ");
                
                if(cmd.Execute(tree)) // ������������
                {
                    AddNewPanel(); // �򿪱༭�û�ͼ

                    // д��־
                    TreeNode node = tree.SelectedNode;
                    string nodePath = GetNodePath(node);
                    logRecorder.SaveLog("�½���ͼ", string.Format("{0}�������½���ͼ[{1}]", chartMode, nodePath), true);
                }
            }            
        }

        /// <summary>
        /// ���������
        /// </summary>
        public void CutNode()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;

            if (CheckTreeCanOperate(tree))
            {                
                TreeNode node = tree.SelectedNode;
                string nodePath = GetNodePath(node);
                
                TreeCutNodeCommand cmd = new TreeCutNodeCommand(tree, "���������");
                
                if (cmd.Execute(tree)) // ������������
                {
                    // д��־
                    logRecorder.SaveLog("���з���/��ͼ", string.Format("{0}���������з���/��ͼ[{1}]", chartMode, nodePath), true);
                }
            }                        
        }

        /// <summary>
        /// ���������
        /// </summary>
        public void CopyNode()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;
            TreeCopyCommand cmd = new TreeCopyCommand(tree, "���������");
            
            if (cmd.Execute(tree)) // ������������
            {
                // д��־
                TreeNode node = tree.SelectedNode;
                string nodePath = GetNodePath(node);
                logRecorder.SaveLog("���Ʒ���/��ͼ", string.Format("{0}���������Ʒ���/��ͼ[{1}]", chartMode, nodePath), true);
            }
        }

        /// <summary>
        /// ճ�������
        /// </summary>
        public void PasteNode()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;

            if (CheckTreeChanged()) // ��ǰ���ṹ�������仯
            {
                MessageBox.Show("��ǰĿ¼�ṹ�Ѿ����޸Ĺ�����Ҫ���¼��ظ��£�", "�޸�Ŀ¼",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);

                // ���¼��ط�����
                ConstructTree(tree, chartMode);
            }
            else
            {                
                TreePasteNodeCommand cmd = new TreePasteNodeCommand(tree, "ճ�������");
                
                if (cmd.Execute(tree)) // ������������
                {
                    // д��־
                    TreeNode node = tree.SelectedNode;
                    string nodePath = GetNodePath(node);
                    logRecorder.SaveLog("ճ������/��ͼ", string.Format("{0}������ճ������/��ͼ[{1}]", chartMode, nodePath), true);
                }
            }            
        }

        /// <summary>
        /// ɾ�������
        /// </summary>
        public void DeleteNode()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;

            if (CheckTreeCanOperate(tree))
            {                
                TreeNode node = tree.SelectedNode;
                string nodePath = GetNodePath(node);
                TreeDeleteNodeCommand cmd = new TreeDeleteNodeCommand(tree, "ɾ�������");
                
                if (cmd.Execute(tree)) // ����ִ�гɹ�
                {
                    List<TreeNode> list = new List<TreeNode>();
                    GetAllChildNodes(node, list);

                    foreach (TreeNode treeNode in list)
                    {
                        TabItem currentTab = null;

                        // ����Ӧid�����û�б���
                        foreach (TabItem t in tabControl.Tabs)
                        {
                            if (t.Tag.ToString() == treeNode.Tag.ToString()) // �ҵ�ɾ��������Ӧ�Ļ�ͼ���
                            {
                                currentTab = t;
                                break;
                            }
                        }

                        if (currentTab != null) // �ҵ�������Ӧ�Ļ�ͼ���
                        {
                            if (tabControl.SelectedTab == currentTab) // �ÿյ�ǰ��ͼ������
                            {
                                flowChartManager = null;
                            }

                            flowChartManagerTable.Remove(currentTab.Tag);
                            tabControl.Tabs.Remove(currentTab);
                        }
                    }

                    // д��־
                    logRecorder.SaveLog("ɾ������/��ͼ", string.Format("{0}������ɾ������/��ͼ[{1}]", chartMode, nodePath), true);
                }
            }
        }

        /// <summary>
        /// ��ȡ��ǰ�����������ӽ��
        /// </summary>
        /// <param name="node">��ȡ��ǰ�����������ӽ��</param>
        /// <param name="list">�����������</param>
        private void GetAllChildNodes(TreeNode node, List<TreeNode> list)
        {
            list.Add(node);

            foreach(TreeNode childNode in node.Nodes)
            {
                GetAllChildNodes(childNode, list);
            }
        }

        /// <summary>
        /// ����Ƿ���ԶԷ��������в���
        /// </summary>
        /// <param name="tree">������</param>
        /// <returns>�Ƿ���Խ��в���</returns>
        private bool CheckTreeCanOperate(TreeView tree)
        {
            bool canOperate = true;

            if (CheckTreeChanged()) // ��ǰ���ṹ�������仯
            {
                MessageBox.Show("��ǰĿ¼�ṹ�Ѿ����޸Ĺ�����Ҫ���¼��ظ��£�", "�޸�Ŀ¼",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);

                // ���¼��ط�����
                ConstructTree(tree, chartMode);

                canOperate = false;
            }
            else if (CheckFlowChartOpened(tree.SelectedNode)) // ����Ӱ�쵱ǰ�򿪵Ļ�ͼ
            {
                MessageBox.Show("���Ѿ��򿪵Ļ�ͼ���ܵ�ǰ������Ӱ�죬���Ƚ���رգ�", "����һ���Լ��",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);

                canOperate = false;
            }

            return canOperate;
        }

        /// <summary>
        /// �����������
        /// </summary>
        public void RenameNode()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;

            if (CheckTreeCanOperate(tree))
            {                
                TreeRenameCommand cmd = new TreeRenameCommand(tree, "�����������");

                if (cmd.Execute(tree))
                {
                    // д��־
                    TreeNode node = tree.SelectedNode;
                    string nodePath = GetNodePath(node);
                    logRecorder.SaveLog("����������/��ͼ", string.Format("{0}����������������/��ͼ[{1}]", chartMode, nodePath), true);
                }
            }                                
        }

        /// <summary>
        /// ���������Ƿ�ѡ��
        /// </summary>
        /// <param name="currentNode">��ǰ�����</param>
        /// <param name="selectedNode">ѡ�е������</param>
        /// <param name="p">��ǰ��λ��</param>
        private void CheckNodeSelected(TreeNode currentNode, Point p)
        {
            if (currentNode.Bounds.Contains(p))
            {
                selectedNode = currentNode;
            }
            else
            {
                foreach(TreeNode node in currentNode.Nodes)
                {
                    CheckNodeSelected(node, p);
                }
            }
        }

        /// <summary>
        /// ��Ŀ¼
        /// </summary>
        public void OpenFolder()
        {
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;

            if (CheckTreeChanged()) // ��ǰ���ṹ�������仯
            {
                MessageBox.Show("��ǰĿ¼�ṹ�Ѿ����޸Ĺ�����Ҫ���¼��ظ��£�", "�޸�Ŀ¼",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);

                // ���¼��ط�����
                ConstructTree(tree, chartMode);
            }
            else
            {
                TreeNode currentNode = tree.SelectedNode;
                if(currentNode != null)
                {
                    // ִ���߼�����
                    LogicBaseManager logicManager = helper.GetLogicManager(chartMode);
                    logicManager.DoLogicOperation(null, LogicType.ViewFolder, currentNode);                    
                }
            }            
        }

        /// <summary>
        /// �鿴��ͼ�Ĵ���
        /// </summary>
        public void ViewCode()
        {
            // ִ���߼�����
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;
            string path = GetNodePath(tree.SelectedNode);
            LogicBaseManager logicManager = helper.GetLogicManager(chartMode);
            logicManager.DoLogicOperation(null, LogicType.ViewCode, path);
        }

        /// <summary>
        /// �����������Ҽ��˵�
        /// </summary>
        /// <param name="point">������ڵ�</param>
        public void PopTreeNodeMenu(Point point)
        {
            selectedNode = null;

            // ��ȡ��ǰ������
            TreeView tree = treeTabControl.SelectedTab.Tag as TreeView;
            Point p = point + new Size(tree.RectangleToScreen(tree.ClientRectangle).Location) 
                    - new Size(tree.AutoScrollOffset); // ����ת����������������������                   
           
            // �ȼ�鵱ǰ�Ƿ�������㱻ѡ��
            foreach(TreeNode node in tree.Nodes)
            {
                CheckNodeSelected(node, point);
            }

            if (selectedNode != null) // ��ѡ�е������
            {
                treeMenu.SubItems["bNewNode"].SubItems["bNewClass"].Enabled = false;
                treeMenu.SubItems["bNewNode"].SubItems["bNewFlowChart"].Enabled = false;
                treeMenu.SubItems["bCutNode"].Enabled = false;
                treeMenu.SubItems["bCopyNode"].Enabled = false;
                treeMenu.SubItems["bPasteNode"].Enabled = false;
                treeMenu.SubItems["bDeleteNode"].Enabled = false;
                treeMenu.SubItems["bRenameNode"].Enabled = false;
                treeMenu.SubItems["bOpenFolder"].Enabled = false;
                treeMenu.SubItems["bViewCode"].Enabled = false;
                treeMenu.SubItems["bRestoreNode"].Enabled = false;
                treeMenu.SubItems["bSaveToFile"].Enabled = false;
                treeMenu.SubItems["bLoadFromFile"].Enabled = false;

                tree.SelectedNode = selectedNode;

                if (selectedNode.ImageIndex == 0) // ��ǰѡ��������Ƿ�����
                {
                    treeMenu.SubItems["bNewNode"].SubItems["bNewClass"].Enabled = true;                                                                               

                    if(selectedNode.Level > 0) // �Ե�ͼ������û�в���
                    {
                        treeMenu.SubItems["bNewNode"].SubItems["bNewFlowChart"].Enabled = true;
                        treeMenu.SubItems["bDeleteNode"].Enabled = true;
                        treeMenu.SubItems["bOpenFolder"].Enabled = true;
                    }

                    if (selectedNode.Level > 0) // ��������������ͼ������
                    {
                        treeMenu.SubItems["bRenameNode"].Enabled = true;
                    }                    
                    
                    if (treeMode == EditMode.Copy || treeMode == EditMode.Cut) // ֻ�м��к͸���ģʽ����ճ��
                    {
                        treeMenu.SubItems["bPasteNode"].Enabled = true;
                    }
                }
                else // ��ǰѡ��������ǻ�ͼ���
                {
                    treeMenu.SubItems["bCutNode"].Enabled = true;
                    treeMenu.SubItems["bCopyNode"].Enabled = true;
                    treeMenu.SubItems["bPasteNode"].Enabled = true;
                    treeMenu.SubItems["bDeleteNode"].Enabled = true;
                    treeMenu.SubItems["bRenameNode"].Enabled = true;
                    treeMenu.SubItems["bViewCode"].Enabled = true;
                    treeMenu.SubItems["bRestoreNode"].Enabled = true;
                    treeMenu.SubItems["bSaveToFile"].Enabled = true;
                    treeMenu.SubItems["bLoadFromFile"].Enabled = true;

                    if (treeMode == EditMode.Copy || treeMode == EditMode.Cut) // ֻ�м��к͸���ģʽ����ճ��
                    {
                        treeMenu.SubItems["bPasteNode"].Enabled = true;
                    }
                }
                
                // �������������˵�
                treeMenu.Popup(p);
            }                        
        }

        /// <summary>
        /// ������������
        /// </summary>
        /// <param name="commandIndex">��ǰ������</param>
        /// <param name="description">���������</param>
        /// <param name="commandCount">��������</param>
        public void AdjustCommandList(int commandIndex, string description, int commandCount)
        {            
            List<ButtonItem> deleteButtonList = new List<ButtonItem>(); // Ҫɾ���İ�ť������

            for (int i = commandIndex + 1; i < commandCount; i++)
            {                
                int buttonIndex = commandCount - 1 - i;

                if (buttonIndex >= 0 && buttonIndex < undoButton.SubItems.Count)
                {
                    deleteButtonList.Add((ButtonItem)undoButton.SubItems[buttonIndex]);
                }                
            }            

            // ɾ�����ť
            foreach (ButtonItem buttonItem in deleteButtonList)
            {
                undoButton.SubItems.Remove(buttonItem);
            }

            commandIndex++;
            commandCount++;

            // ����°�ť            
            ButtonItem newButton = new ButtonItem("undoButton" + commandIndex, description);
            newButton.Tag = commandIndex.ToString();
            flowChartManager.CurrentGraphManager.CurrentEventManager.RegistCommandButtonEvent(newButton);
            undoButton.SubItems.Add(newButton, 0);

            if (commandIndex == commandCount - 1)
            {
                SetUndoState(true);
            }

            SetRedoState(false);  
        }

        /// <summary>
        /// ����������ť��״̬
        /// </summary>
        /// <param name="enable">�Ƿ���Ч</param>
        public void SetRedoState(bool enable)
        {
            redoButton.Enabled = enable;
        }

        /// <summary>
        /// ���ó�����ť��״̬
        /// </summary>
        /// <param name="enable">�Ƿ���Ч</param>
        public void SetUndoState(bool enable)
        {
            undoButton.Enabled = enable;
        }

        /// <summary>
        /// ��������
        /// </summary>
        public void Undo()
        {
            if (flowChartManager != null) // ִ�л�ͼ�ĳ�������
            {
                flowChartManager.CurrentGraphManager.Undo();
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        public void Redo()
        {
            if (flowChartManager != null) // ִ�л�ͼ����������
            {
                flowChartManager.CurrentGraphManager.Redo();
            }
        }

        /// <summary>
        /// ��ͼ��Ϣ��
        /// </summary>
        class DiagramInfo
        {
            private string hostName; // �༭��
            private DateTime editTime; // �༭ʱ��  
            private string currentVersion; // �汾��
            private int childCount = 0; // �ӻ�ͼ�ĸ���
            private bool readOnly = true; // ��Ա�����Ƿ���ֻ��
            
            /// <summary>
            /// ���캯��
            /// </summary>
            /// <param name="hostName">�༭��</param>
            /// <param name="editTime">�༭ʱ��</param>
            /// <param name="currentVersion">�汾��</param>
            /// <param name="childCount">�ӻ�ͼ�ĸ���</param>
            public DiagramInfo(string hostName, DateTime editTime, string currentVersion, int childCount)
            {
                this.hostName = hostName;
                this.editTime = editTime;
                this.currentVersion = currentVersion;
                this.childCount = childCount;
            }

            /// <summary>
            /// ��Ա�����Ƿ���ֻ��
            /// </summary>
            [Browsable(false)]
            public bool ReadOnly
            {
                set
                {
                    readOnly = value;
                }

                get
                {
                    return readOnly;
                }
            }

            /// <summary>
            /// �༭��
            /// </summary>
            [Browsable(true), DisplayName("�༭��"), Description("��ͼ�ϴα༭���û���"), Category("��ͼ����")]
            public string HostName
            {
                set
                {
                    if (readOnly)
                    {
                        MessageBox.Show("��ͼ��Ϣ���ܱ��ⲿ�༭��", "�༭��ͼ��Ϣ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        hostName = value;
                    }
                }

                get
                {
                    return hostName;
                }
            }

            /// <summary>
            /// �汾��
            /// </summary>
            [Browsable(true), DisplayName("�汾��"), Description("��ͼ��ǰ�İ汾��"), Category("��ͼ����")]
            public string CurrentVersion
            {
                set
                {
                    if (readOnly)
                    {
                        MessageBox.Show("��ͼ��Ϣ���ܱ��ⲿ�༭��", "�༭��ͼ��Ϣ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        currentVersion = value;
                    }                    
                }

                get
                {
                    return currentVersion;
                }
            }

            /// <summary>
            /// �༭ʱ��
            /// </summary>
            [Browsable(true), DisplayName("�༭ʱ��"), Description("��ͼ�ϴα༭��ʱ��"), Category("��ͼ����")]
            public DateTime EditTime
            {
                set
                {
                    if (readOnly)
                    {
                        MessageBox.Show("��ͼ��Ϣ���ܱ��ⲿ�༭��", "�༭��ͼ��Ϣ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        editTime = value;
                    }
                }

                get
                {
                    return editTime;
                }
            }

            /// <summary>
            /// �ӻ�ͼ����
            /// </summary>
            [Browsable(true), DisplayName("�ӻ�ͼ����"), Description("�ӻ�ͼ�ĸ���"), Category("��ͼ����")]
            public int ChildCount
            {
                set
                {
                    if (readOnly)
                    {
                        MessageBox.Show("��ͼ��Ϣ���ܱ��ⲿ�༭��", "�༭��ͼ��Ϣ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        childCount = value;
                    }                    
                }

                get
                {
                    return childCount;
                }
            }
        }
    }
}
