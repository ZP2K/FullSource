using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Collections;
using FlowChartEditor.BaseGraphElement;
using DevComponents.DotNetBar;
using DevComponents.DotNetBar.Controls;
using DataElementLibrary;
using GraphElementLibrary;
using DBManager;
using DataInteractor;

namespace FlowChartEditor
{
    public class FlowChartManager
    {
        private TabItem tabItem; // ����ͼ��Ӧ�ı�ǩҳ
        private string id; // ����ͼ��id
        private string name; // ����ͼ������
        private string mapName; // ����ͼ���ڵ�ͼ������
        private string path; // ����ͼ��·��
        private int version; // ����ͼ�ı༭�汾 
        private DateTime editTime; // ����ͼ�ı༭ʱ��
        private GraphManager graphManager; // ͼԪ������
        private DataManager dataManager; // ���ݹ�����
        private DataBaseManager dataBaseManager; // ���ݿ������
        private bool locked = false; // �Ƿ��Ѿ�������ǰ����ͼ
        private bool contentChanged = false; // ����ͼ�����Ƿ�ı�����
        private Hashtable pasteTable = new Hashtable(); // ճ������ͼԪ�Ĺ�ϣ��
        private FlowChartInteractor flowChartInteractor; // ����ͼ����������ʵ��
        private bool supportCommand = false; // �Ƿ��ṩ����֧��
        private bool enableSave = true; // �Ƿ�������
        private Hashtable customData = new Hashtable(); // �Զ�������
        private readonly string chartMode; // ��ͼ����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="chartMode">��ͼ�ı༭ģʽ</param>
        public FlowChartManager(string chartMode)
        {
            this.chartMode = chartMode;
        }

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="chartMode">��ͼ�ı༭ģʽ</param>
        /// <param name="tabItem">��ǩҳ</param>
        /// <param name="id">��ͼ��id</param>
        /// <param name="name">��ͼ������</param>
        /// <param name="path">��ͼ��·��</param>
        public FlowChartManager(string chartMode, TabItem tabItem, string id, string name, string path)
        {
            this.chartMode = chartMode;
            this.tabItem = tabItem;
            this.id = id;
            this.name = name;
            this.path = path;            
        }
        
        /// <summary>
        /// ���캯��
        /// </summary>
        public FlowChartManager()
        {

        }

        /// <summary>
        /// ��ͼ���Զ�������
        /// </summary>
        public Hashtable CustomData
        {
            get
            {
                return customData;
            }
        }

        /// <summary>
        /// �Ƿ�������
        /// </summary>
        public bool EnableSave
        {
            set
            {
                enableSave = value;
            }

            get
            {
                return enableSave;
            }
        }

        /// <summary>
        /// ����֧��
        /// </summary>
        public bool SupportCommand
        {
            set
            {
                supportCommand = value;
            }

            get
            {
                return supportCommand;
            }
        }

        /// <summary>
        /// ����ͼ����������ʵ��
        /// </summary>
        public FlowChartInteractor CurrentFlowChartInteractor
        {
            get
            {
                return flowChartInteractor;
            }
        }

        /// <summary>
        /// ��ͼ�ı༭ģʽ
        /// </summary>
        public string CurrentChartMode
        {
            get
            {
                return chartMode;
            }
        }

        /// <summary>
        /// ճ������ͼԪ�Ĺ�ϣ��
        /// </summary>
        public Hashtable PasteTable
        {
            set
            {
                pasteTable = value;
            }

            get
            {
                return pasteTable;
            }
        }

        /// <summary>
        /// ����ͼ�������Ƿ�ı�����
        /// </summary>
        public bool ContentChanged
        {
            set
            {
                if (enableSave) // ��������ݾͲ��ش���
                {
                    MarkTabContent(value);
                }                
            }

            get
            {
                return contentChanged;
            }
        }

        /// <summary>
        /// ����ͼ�ı༭�汾
        /// </summary>
        public int Version
        {
            set
            {
                version = value;
            }

            get
            {
                return version;
            }
        }

        /// <summary>
        /// ����ͼ�ı༭ʱ��
        /// </summary>
        public DateTime EditTime
        {
            set
            {
                editTime = value;
            }

            get
            {
                return editTime;
            }
        }

        /// <summary>
        /// ����ͼ��·������
        /// </summary>
        public string CurrentPath
        {
            set
            {
                path = value;
            }

            get
            {
                return path;
            }
        }

        /// <summary>
        /// ����ͼ��ID
        /// </summary>
        public string ID
        {
            get
            {
                return id;
            }
        }

        /// <summary>
        /// ����ͼ������
        /// </summary>
        public string Name
        {
            get
            {
                return name;
            }
        }

        /// <summary>
        /// ����ͼ���ڵ�ͼ������
        /// </summary>
        public string MapName
        {
            get
            {
                return mapName;
            }
        }

        /// <summary>
        /// ͼԪ����������
        /// </summary>
        public GraphManager CurrentGraphManager
        {
            get
            {
                return graphManager;
            }
        }

        /// <summary>
        /// ���ݹ���������
        /// </summary>
        public DataManager CurrentDataManager
        {
            get
            {
                return dataManager;
            }
        }        

        /// <summary>
        /// ��������Ƿ���Ч
        /// </summary>
        public void CheckDataValid()
        {
            string requestReloadID = dataManager.RequestReloadID;

            if (requestReloadID != "")
            {
                DialogResult result = MessageBox.Show("���ֻ�ͼ���������ڳ����ⷢ���ı䣬��Ҫ���±༭���£�", "����һ���Լ��", 
                                                      MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                if (result == DialogResult.OK)
                {
                    string[] idArray = requestReloadID.Split(new char[] { ',' });
                    Hashtable argTable = new Hashtable();
                    argTable["globe_args"] = dataManager.GlobeArgs;
                    argTable["requestReloadID"] = "";
                    argTable["forceReload"] = "1";

                    foreach(string id in idArray)
                    {
                        GraphElement graphElement = dataManager.FindGraphElementByID(int.Parse(id));

                        if (graphElement != null)
                        {
                            DataElement dataElement = dataManager.FindDataElementByID(int.Parse(id));
                            dataElement.ReloadData(argTable);
                            graphManager.SelectGraphElement(graphElement, true);
                            graphManager.EditDataElement(graphElement);
                        }
                    }                    
                }                
            }
        }

        /// <summary>
        /// ��������ͼ����
        /// </summary>
        /// <param name="args">��������</param>
        public void SetArguments(Hashtable args)
        {
            try
            {                
                flowChartInteractor.SetArguments(args["flowChartInteractData"] as Hashtable);

                Hashtable graphData = args["graphData"] as Hashtable;
                Hashtable logicData = args["logicData"] as Hashtable;                

                graphManager.SetArguments(graphData);                
                dataManager.SetArguments(logicData);
                customData = args["customData"] as Hashtable;

                if (customData == null)
                {
                    customData = new Hashtable();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("��ʼ����ͼʧ�ܣ�" + ex.Message, "���ػ�ͼ", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }            
        }

        /// <summary>
        /// ��ȡ��ͼ����
        /// </summary>
        /// <returns>��������</returns>
        public Hashtable GetArguments()
        {
            Hashtable args = new Hashtable();            
            args["graphData"] = graphManager.GetArguments();
            args["logicData"] = dataManager.GetArguments();
            args["flowChartInteractData"] = flowChartInteractor.GetArguments();
            args["customData"] = customData;           

            return args;
        }

        /// <summary>
        /// ��Ǳ�ǩҳ������
        /// </summary>
        /// <param name="mark">�Ƿ���</param>
        public void MarkTabContent(bool mark)
        {
            if (mark)
            {                
                if (!locked) // ��û������ǰ����ͼ
                {
                    DocumentManager documentManager = DocumentManager.GetDocumentManager();
                    DateTime latestEditTime = dataBaseManager.GetDiagramEditTime(id);
                    string editor = dataBaseManager.GetDiagramEditor(id);
                    int latestVersion = dataBaseManager.GetDiagramVersion(id);

                    if (version != latestVersion) // ����Ƿ�����������
                    {
                        MessageBox.Show(string.Format("��ǰ��ͼ�Ѿ���{0}��{1}�޸Ĺ�����Ҫ���¼����Ի�ȡ�������ݣ�", editor, latestEditTime.ToString()),
                                        "��ͼ����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        ReLoadFlowChart();
                        graphManager.ClearCommandList();
                        documentManager.ClearCommandButton();
                        contentChanged = false;
                    }
                    else // ���Լ���
                    {
                        try
                        {                        
                            if(dataBaseManager.Diagram_Lock(int.Parse(id))) // �����ɹ�
                            {
                                locked = true;
                                tabItem.Text = name + " *";
                                contentChanged = true;
                            }
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show(string.Format("��ǰ��ͼ�Ѿ���{0}�����༭����ȴ��޸���ɺ��ٳ��Դ򿪣�", ex.Message), "��ͼ����",
                                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                            ReLoadFlowChart();
                            graphManager.ClearCommandList();
                            documentManager.ClearCommandButton();
                            contentChanged = false;
                        } 
                    }                                                           
                }
                else // �Ѿ�����
                {
                    tabItem.Text = name + " *";
                    contentChanged = true;                 
                }
            }
            else
            {
                tabItem.Text = name;                
                locked = false;
                contentChanged = false;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        /// <param name="canvas">��ͼ��ʵ��</param>
        /// <param name="propertyGrid">�������ʵ��</param>
        public void Init(Canvas canvas)
        {
            graphManager = new GraphManager(this, canvas);
            dataManager = new DataManager(this);
            dataBaseManager = DataBaseManager.GetDataBaseManager();
            int i = path.IndexOf('\\');
            mapName = path.Substring(0, i);
            flowChartInteractor = new FlowChartInteractor();
        }                

        /// <summary>
        /// �༭����Ԫ
        /// </summary>
        /// <param name="graphElement">����������Ԫ</param>
        /// <return>�Ƿ�༭�ɹ�</return>
        public bool EditDataElement(GraphElement graphElement)
        {            
            bool result = dataManager.EditData(graphElement);
            return result;
        }

        /// <summary>
        /// ���¼�������ͼ
        /// </summary>
        private void ReLoadFlowChart()
        {
            Hashtable args = dataBaseManager.GetDiagramData(id) as Hashtable;
            if (args != null) // ����ͼ���ݲ�Ϊ��
            {
                graphManager.SetArguments(args["graphData"] as Hashtable);
                dataManager.SetArguments(args["logicData"] as Hashtable);
                flowChartInteractor.SetArguments(args["flowChartInteractData"] as Hashtable);
            }

            editTime = dataBaseManager.GetDiagramEditTime(id);
            version = dataBaseManager.GetDiagramVersion(id);
        }
    }
}
