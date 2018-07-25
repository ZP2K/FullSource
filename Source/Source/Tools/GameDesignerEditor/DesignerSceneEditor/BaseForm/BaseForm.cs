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
    unsafe public partial class BaseForm : Form, IKDocument
    {
        private BaseDoc m_doc;
        public BaseDoc DOC { get { return m_doc; } set { m_doc = value; } }

        private BaseView m_view;
        public BaseView VIEW { get { return m_view; } set { m_view = value; } }

        public MyPanel PANEL { get { return this.PanelView; } set { this.PanelView = value; } } 

        string m_filename = string.Empty;
        public string FileName { get { return m_filename; } }

        private SceneEditor m_scenesceneeditor = new SceneEditor();
        public SceneEditor SceneSceneEditor { get { return m_scenesceneeditor; } set { m_scenesceneeditor = value; } }

        private TopViewForm m_topView = null;

        private SaveOptionForm m_SaveOptionForm = null;

        //private bool m_bEditing = true;
        public bool m_bTopView = false;
        public bool m_bFullScreen = false;

        const int WM_KEYDOWN = 0x0100;
        const int WM_KEYUP = 0x0101;
        const int WM_CHAR = 0x0102;
        const int MAX_PATH = 260;

        private LuaEx m_lua = null;

        private string m_curMapName = string.Empty;
        private Preview previewform = null;

        public int curMapWidth = 0, curMapHeight = 0;

        private string[] m_types;
        public Hashtable htObjScenePosIndices = null;
        public Hashtable htObjLogicNXIndices = null;
        public Hashtable htObjLogicDirIndices = null;
        public Hashtable htObjLogicRotationIndices = null;

        private bool m_shiftPress = false;
        private bool m_ctrlPress = false;

        const int CAMERA_STORAGE = 4;
        private bool[] m_bPosSaved = new bool[CAMERA_STORAGE];
        private _AtlVector3[] m_SavedCameraPos = new _AtlVector3[CAMERA_STORAGE];
        private _AtlVector3[] m_SavedCameraLookat = new _AtlVector3[CAMERA_STORAGE];

        // ahpho suntao
        private enum EditMode { None, AddNpc, AddDoodad, AddWayPoint, AddTrafficPoint }; // �༭ģʽ
        private EditMode editMode = EditMode.None; // ��ǰ�༭ģʽ
        private Font normalTextFont = new Font("SimSun", 9, FontStyle.Regular); // ��ͨ�ı�����
        private Font boldTextFont = new Font("SimSun", 9, FontStyle.Bold); // �����ı�����
        private bool beginEdit = false; // �Ƿ��Ѿ���ʼ�༭
        public SqlConnection Conn; // sql����
        private string rootDir; // �ͻ��˸�Ŀ¼
        public static DataTable npcTable; // npc���ݱ�
        private DataTable npcRepresentTable; // npc���ֱ�
        private Hashtable npcInfoTable = new Hashtable(); // npc��Ϣ���ݱ�
        private Hashtable npcAnimationTable = new Hashtable(); // npc�����ļ����ݱ�
        private DataTable doodadTable; // doodad���ݱ�
        private DataTable doodadRepresentTable; // doodad���ֱ�
        private Hashtable doodadInfoTable = new Hashtable(); // doodad��Ϣ���ݱ�        
        private List<string> npcAnimationList; // npc�����ļ�����
        private int npcAnimationIndex = 0; // npc�������
        private int doodadModelIndex = 0; // Doodadģ�ͱ��
        private TextBox cameraLookAtBox; // ���������ؼ�
        public TextBox CameraLookAtBox { set { cameraLookAtBox = value; } } // ���������ؼ�
        private SliderItem cameraSpeedBar; // ����ٶȿ�����
        private MainForm m_parent;

        public static DataTable patrolPathTable = new DataTable(); // npcѲ��·���ݱ�
        public static DataTable npcOrderTable = new DataTable(); // npc�������ݱ�

        // �����߼���С�������
        public float m_fLogicTotalWidth = 0.0f;
        public float m_fLogicTotalHeight = 0.0f;
        public float m_fMiddleMapStartX = 0.0f;
        public float m_fMiddleMapStartY = 0.0f;
        public float m_fLogicStartX = 0.0f;
        public float m_fLogicStartZ = 0.0f;

        // ��GetObjDisplayInfo�õ���ʱ������
        int logicObj = 0;
        int representObj = 0;
        int hasScript = 0;
        string name = string.Empty;
        string nickName = string.Empty;
        int templateID = -1;

        // �����߼����Ϊ�������
        private bool m_bShowAllNpcDoodad = true;

        private CheckRelive m_CheckRelive;

        public RegionSplitFrom m_RegionSplitFrom = null;

        public BaseForm(string filename, MainForm parent)
        {
            InitializeComponent();

            InitVars();

            m_parent = parent;
            m_doc = new BaseDoc(this, filename);
            m_view = new BaseView(this, this.PANEL.Handle);
            this.PANEL.m_baseForm = this;
            m_doc.DocLogical.SetMainViewHwnd((int)this.PANEL.Handle);
            this.MouseWheel += new MouseEventHandler(this.BaseForm_MouseWheel);
            this.PanelView.GotFocus += new EventHandler(this.PanelView_GotFocus);
            this.PanelView.LostFocus += new EventHandler(this.PanelView_LostFocus);
            this.PanelView.MouseDown += new MouseEventHandler(this.PanelView_MouseDown);
            this.PanelView.Resize += new EventHandler(this.PanelView_Resize);
            this.KeyPreview = true;

            // ����ͼ
            InitTopView_Tga(filename);
            InitTopView_Old();

            // lua�ű�
            InitLua();

            // ahpho suntao
            // ��ʼ��·�����
            InitPatrol();

            // ��ʼ����ͨ���
            InitTraffic();

            //
            InitPolyComponents();
            InitPoly();

            // ��ʼ��sql����
            InitSqlConnection();

            // ��ʼ��Npc���
            InitNpc();
            EngineLayer.ATLBase.OnResizeNpcWindow();

            // ��ʼ��Doodad���
            InitDoodad();
            EngineLayer.ATLBase.OnResizeDoodadWindow();

            // ��ʼ��Ԥ�����
            InitPreviewCanvas();
            // end ahpho

            // loadtree
            DateTime t_begin = DateTime.Now;
            LogicHelper.LoadTree(m_doc.m_AllLogicObj, this.treeView1, m_bShowAllNpcDoodad);
            TimeSpan t_delta = DateTime.Now - t_begin;
            Debug.WriteLine(t_delta.ToString());
            //MessageBox.Show(t_delta.ToString());

            // init timer
            InitRenderTimer();

            // ��ȡ·����������Ϣ
            DataColumn orderIDColumn = new DataColumn("ID");
            DataColumn orderNameColumn = new DataColumn("Name");
            DataColumn orderCountColumn = new DataColumn("Count");

            npcOrderTable.Columns.Clear();
            npcOrderTable.Columns.Add(orderIDColumn);
            npcOrderTable.Columns.Add(orderNameColumn);
            npcOrderTable.Columns.Add(orderCountColumn);

            InitNpcOrderTable();

            DataColumn patrolPathIDColumn = new DataColumn("ID");
            DataColumn patrolPathNameColumn = new DataColumn("Name");

            patrolPathTable.Columns.Clear();
            patrolPathTable.Columns.Add(patrolPathIDColumn);
            patrolPathTable.Columns.Add(patrolPathNameColumn);

            InitPatrolPathTable();

            InitCameraUI();

            m_CheckRelive = new CheckRelive();
            m_CheckRelive.Init(filename, m_doc.DocLogical, SceneSceneEditor);

        }

        private void InitCameraUI()
        {
            m_parent.UpdateCameraSpeedToUI();
        }

        private void checkShowAll_CheckedChanged(object sender, EventArgs e)
        {
            m_bShowAllNpcDoodad = this.checkShowAll.Checked;
            LogicHelper.LoadTree(m_doc.m_AllLogicObj, this.treeView1, m_bShowAllNpcDoodad);
        }

        /// <summary>
        /// ����ٶȿ�����
        /// </summary>
        public SliderItem CamerSpeedBar
        {
            set
            {
                cameraSpeedBar = value;
            }
        }

        private void InitLua()
        {
            m_lua = new LuaEx();
            string luafile = Application.StartupPath + "\\lua.lua";
            if (!File.Exists(luafile))
                return;

            m_lua.DoFile(luafile);
            LuaFunction fun = m_lua.GetFunction("transform");
            if (fun != null)
            {
                object[] retobjs = fun.Call(luafile);
            }
        }

        /// <summary>
        /// ���ö���ѡ������
        /// </summary>
        /// <param name="mask">����</param>
        public void SetObjectSelectMask(int nMask)
        {
            SceneSceneEditor.SetObjectSelectMask((uint)nMask);
        }

        public bool Save()
        {
            m_SaveOptionForm = new SaveOptionForm();
            DialogResult dr = m_SaveOptionForm.ShowDialog();
            if (dr != DialogResult.OK)
                return false;

            int nRet = m_CheckRelive.Recheck();
            if (nRet == 0)
            {
                return true;
            }

            EngineLayer.ATLBase.SetCurDir();
            EngineLayer.ATLBase.ShowProgressForm(1);
            m_doc.SaveEditorIni();
            if (m_SaveOptionForm.m_ComputeCell)
            {
                m_doc.ComputeCells(m_SaveOptionForm.m_AppointCell, m_SaveOptionForm.m_ClearCell);
            }
            if (m_SaveOptionForm.m_UpdateAllRest)
            {
                m_doc.UpdateAllRest(m_SaveOptionForm.m_AllRest);
            }
            if (!m_SaveOptionForm.m_SaveIniOnly && m_SaveOptionForm.m_SaveLogics)
            {
                m_doc.ExchangeLogics();
            }
            EngineLayer.ATLBase.ShowProgressForm(0);

            m_CheckRelive.SaveSetting();

            return true;
        }

        private void PanelView_LostFocus(object sender, System.EventArgs e)
        {

        }

        private void PanelView_GotFocus(object sender, System.EventArgs e)
        {
            SceneSceneEditor.SetFocus();
            SceneSceneEditor.SetCurrentOutputWindow(VIEW.MAINWINDOWID);
            
//             if (m_Scene != null && m_nOutputWindowID != -1)
//             {
//                 EngineLayer.EngineMgr.SetFocusScene(m_Scene);
//                 m_Scene.SetCurrentOutputWindow(m_nOutputWindowID);
// 
//                 if (EngineLayer.EngineMgr.GetAutoReloadTexture() != 0)
//                 {
//                     AtlIEKG3DTextureTable textureTable = null;
//                     EngineLayer.EngineMgr.GetIEKG3DTextureTable(ref textureTable);
//                     textureTable.AutoReload();
//                 }
//             }
        }        

        private void InitRenderTimer()
        {
            tmrRender.Enabled = true;
        }

        private void tmrRender_Tick(object sender, EventArgs e)
        {
            EngineLayer.ATLBase.UpdateKeyState();
            EngineLayer.ATLBase.FrameMove();
            EngineLayer.ATLBase.Render();
            previewform.m_panel1.Refresh();
        }

        private void BaseForm_MouseWheel(object sender, MouseEventArgs e)
        {
            // EngineLayer.ATLBase.MsgOnMouseWheel((short)e.Delta);
        }

        private void PanelView_MouseDown(object sender, MouseEventArgs e)
        {
            // ahpho suntao
            switch(e.Button)
            {
                case MouseButtons.Right:
                    {
                        if (m_shiftPress && CheckObjectCanRotate()) // �ȼ�鵱ǰѡ�еĶ����Ƿ��ܹ���ת
                        {
                            RotateSelectedObject(); // ��תѡ�еĲ���
                        }
                        
                        break;
                    }
            }           
            // end ahpho

            this.ActiveControl = this.PanelView;
        }

        private void PanelView_MouseUp(object sender, MouseEventArgs e)
        {
            switch(e.Button)
            {
                case MouseButtons.Left:
                    {
                        switch (editMode)
                        {
                            case EditMode.AddWayPoint:
                                {
                                    RefreshPatrol();
                                    break;
                                }
                            case EditMode.AddTrafficPoint:
                                {
                                    RefreshTraffic();
                                    break;
                                }
                        }

                        break;
                    }
                case MouseButtons.Right:
                    {
                        cameraLookAtBox.Text = GetCameraLookAt();

                        break;
                    }
            }
        }

        private void BaseForm_KeyUp(object sender, KeyEventArgs e)
        {
            if (!e.Shift)
            {
                m_shiftPress = false;                
            }

            if (!e.Control)
            {
                m_ctrlPress = false;
            }

            EngineLayer.ATLBase.ProcessMessage((uint)WM_KEYUP, e.KeyValue, e.KeyValue);
        }   

        private void BaseForm_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Shift)
            {
                m_shiftPress = true;                
            }
            if (e.Control)
            {
                m_ctrlPress = true;
            }

            if (!(this.ActiveControl is PanelDockContainer) && this.ActiveControl != this.PanelView)
            {
                return;
            }
            if (!e.Alt && !e.Shift && e.Control && e.KeyCode == Keys.Enter)
            {
                ToggleFullScreen();
                return;
            }
            if (!e.Alt && !e.Shift && !e.Control && e.KeyCode == Keys.P)
            {
                TogglePMode();
                return;
            }
            if (!e.Alt && !e.Shift && !e.Control && e.KeyCode == Keys.T)
            {
                ToggleTopView();
                return;
            }
            if (!e.Alt && !e.Shift && !e.Control && e.KeyCode == Keys.X)
            {
                SceneSceneEditor.DropSelectedEntity();
            }
            if (!e.Alt && !e.Shift && !e.Control && e.KeyCode == Keys.Z)
            {
                SceneSceneEditor.ZoomToObj();
            }
            if (e.Shift && e.KeyCode >= Keys.F1 && e.KeyCode <= Keys.F1 + CAMERA_STORAGE - 1)
            {
                int saveindex = e.KeyCode - Keys.F1;
                m_bPosSaved[saveindex] = true;
                SceneSceneEditor.GetCameraPosLookat(ref m_SavedCameraPos[saveindex], ref m_SavedCameraLookat[saveindex]);
                return;
            }
            if (!e.Shift && e.KeyCode >= Keys.F1 && e.KeyCode <= Keys.F1 + CAMERA_STORAGE - 1)
            {
                int saveindex = e.KeyCode - Keys.F1;
                if (m_bPosSaved[saveindex])
                    SceneSceneEditor.SetCameraPosLookat(ref m_SavedCameraPos[saveindex], ref m_SavedCameraLookat[saveindex]);
                return;
            }
            if (!e.Shift && !e.Control && e.KeyCode == Keys.Enter)
            {
                ShowProperties();
            }

            switch(e.KeyCode)
            {
                case Keys.Oemcomma:
                    {
                        NpcPlayNextAction();
                        return;
                    }
                case Keys.OemPeriod:
                    {
                        NpcPlayPreviousAction();
                        return;
                    }
                case Keys.G:
                    {
                        EditGroup();
                        return;
                    }
                case Keys.Oem1:
                    {
                        editMode = EditMode.None;
                        break;
                    }
                case Keys.W:
                    {
                        cameraLookAtBox.Text = GetCameraLookAt();
                        break;
                    }
                case Keys.S:
                    {
                        cameraLookAtBox.Text = GetCameraLookAt();
                        break;
                    }
                case Keys.A:
                    {
                        cameraLookAtBox.Text = GetCameraLookAt();
                        break;
                    }
                case Keys.D:
                    {
                        cameraLookAtBox.Text = GetCameraLookAt();
                        break;
                    }
                case Keys.Delete:
                    {
                        DeleteSelectedObject();
                        break;
                    }
                case Keys.Add:
                    goto case Keys.Oemplus;
                case Keys.Oemplus:
                    {
                        m_parent.m_sliderCameraSpeed.Value++;
                        m_parent.UpdateCameraSpeedToUI();
                        break;
                    }
                case Keys.Subtract:
                    goto case Keys.OemMinus;
                case Keys.OemMinus:
                    {
                        m_parent.m_sliderCameraSpeed.Value--;
                        m_parent.UpdateCameraSpeedToUI();
                        break;
                    }
            }

            EngineLayer.ATLBase.ProcessMessage((uint)WM_KEYDOWN, e.KeyValue, e.KeyValue);
        }

        /// <summary>
        /// ģ�⴦������Ϣ
        /// </summary>
        /// <param name="e">������Ϣ����</param>
        public void DoKeyMessage(KeyEventArgs e)
        {
            BaseForm_KeyDown(this, e);
            BaseForm_KeyUp(this, e);
        }

        private void ShowProperties()
        {
            int entityCount = -1;
            SceneSceneEditor.GetSelectedEntityCount(ref entityCount);

            _AtlObjInfo[] objinfos = new _AtlObjInfo[entityCount];
            for (int i = 0; i < entityCount; i++)
            {
                int iLogicObjPtr = 0;
                string objTypeName = string.Empty;
                m_doc.DocLogical.GetSelectedLogicObjPtr(i, ref iLogicObjPtr, ref objTypeName);

                if (iLogicObjPtr != 0)
                {
                    _AtlObjInfo atlinfo = new _AtlObjInfo();
                    atlinfo.bChanged = 1;
                    atlinfo.iLogicObjPtr = iLogicObjPtr;
                    //_AtlObjInfo��Ҫ����iLogicObjPtr entityPtr������C#�Ի��򱣴�ʱ ֱ��exchangeȥdocLogical��
                    //����iLogicObjPtr֮�߼�����entityPtrָ��ı��֡�
                    m_doc.DocLogical.GetSetObjInfo(objTypeName, ref atlinfo, iLogicObjPtr, 1);
                    objinfos[i] = atlinfo;
                }
                else
                {
                    //objinfos[i] = _AtlObjInfo; �Զ���valueType��ζ���empty
                }
            }

            if (EditSelectedObjectsProperty(objinfos))
            {
                for (int i = 0; i < objinfos.Length; i++)
                {
                    _AtlObjInfo objinfo = objinfos[i];
                    //if (objinfo.bChanged != 0)
                    //{
                    m_doc.DocLogical.GetSetObjInfo(objinfo.strType, ref objinfo, objinfo.iLogicObjPtr, 0);
                    m_doc.DocLogical.UpdateObjRepresentByLogicObj(ref objinfo, objinfo.iLogicObjPtr);
                    //}
                }
            }
        }

        /// <summary>
        /// ɾ��ѡ�еĶ���
        /// </summary>
        private void DeleteSelectedObject()
        {
            int entityCount = -1;
            SceneSceneEditor.GetSelectedEntityCount(ref entityCount);
            if (entityCount > 0)
            {
                DialogResult dialogResult = MessageBox.Show("ȷ��Ҫɾ��ѡ�еĶ�����", "ɾ������",
                                                            MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                if (dialogResult == DialogResult.OK)
                {
                    //m_doc.DocLogical.DeleteSelectedObject();
                    SceneSceneEditor.DeleteSelectedEntity();
                    RefreshPatrol();
                    RefreshTraffic();
                }
            }
        }

        public void ToggleTopView()
        {
            m_bTopView = !m_bTopView;
            if (m_bTopView)
            {
                m_topView.Init();
                m_topView.Show();
                m_topView.SetMyCurrentWindow();
            }
            else
            {
                m_topView.Hide();
                m_topView.UnInit();
            }
        }

        public void ToggleFullScreen()
        {
            m_bFullScreen = !m_bFullScreen;
            m_parent.SetFullScreen(m_bFullScreen);
            SetFullScreen(m_bFullScreen);
            SceneSceneEditor.SetFullScreen(m_bFullScreen ? 1 : 0);
        }

        public void TogglePMode()
        {
            SceneSceneEditor.TogglePMode();
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
        }

        private void PanelView_Resize(object sender, EventArgs e)
        {
            SceneSceneEditor.OnResizeWindow();
        }

        public void ExecCommand(string strCmd, object data)
        {
            switch (strCmd.Trim().ToLower())
            {
            case "savedocument":
                break;
            case "findrecord":
                break;
            case "cmdexport":
                break;
            case "cmdimport":
                break;
            default:
                break;
            }
        }

        /// <summary>
        /// ��鵱ǰѡ�еĶ����Ƿ��ܹ���ת ahpho suntao
        /// </summary>
        /// <returns>�Ƿ��ܹ���ת</returns>
        private bool CheckObjectCanRotate()
        {
            bool canRotate = true;

            int entityCount = 0;
            SceneSceneEditor.GetSelectedEntityCount(ref entityCount);

            _AtlObjInfo[] objInfoArray = new _AtlObjInfo[entityCount];
            for (int i = 0; i < entityCount; i++)
            {
                string objectType = objInfoArray[i].strType;
                if (objectType == "NPC" || objectType == "Doodad")
                {
                    ;
                }
                else
                {
                    canRotate = false;
                    break;
                }
            }

            return canRotate;
        }

        /// <summary>
        /// ��תѡ�еĲ���
        /// </summary>
        private void RotateSelectedObject()
        {
            string newRotation = null;
            SceneSceneEditor.RotateSelectedObject(ref newRotation);
            if(newRotation != null) // �Ѿ��ı䳯��
            {
                string[] rotations = newRotation.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);

                int entityCount = -1;
                SceneSceneEditor.GetSelectedEntityCount(ref entityCount);

                _AtlObjInfo[] objinfos = new _AtlObjInfo[entityCount];
                for (int i = 0; i < entityCount; i++)
                {
                    int iLogicObjPtr = 0;
                    string objTypeName = string.Empty;
                    m_doc.DocLogical.GetSelectedLogicObjPtr(i, ref iLogicObjPtr, ref objTypeName);
                    _AtlObjInfo atlinfo = new _AtlObjInfo();
                    atlinfo.bChanged = 1;
                    atlinfo.iLogicObjPtr = iLogicObjPtr;
                    //_AtlObjInfo��Ҫ����iLogicObjPtr entityPtr������C#�Ի��򱣴�ʱ ֱ��exchangeȥdocLogical��
                    //����iLogicObjPtr֮�߼�����entityPtrָ��ı��֡�
                    m_doc.DocLogical.GetSetObjInfo(objTypeName, ref atlinfo, iLogicObjPtr, 1);
                    objinfos[i] = atlinfo;

                    // д���µ���ת�Ƕ���Ϣ
                    for(int j = 0; j < atlinfo.iFieldsCount; j++)
                    {
                        if(atlinfo.strFields[j] == "vRotation")
                        {
                            atlinfo.strValues[j] = newRotation;                            
                        }
                        else if(atlinfo.strFields[j] == "nFaceDirection")
                        {
                            atlinfo.strValues[j] = ConvertRotationToDirection(rotations[i]);
                        }
                    }

                    // ���¶�������
                    m_doc.DocLogical.GetSetObjInfo(atlinfo.strType, ref atlinfo, atlinfo.iLogicObjPtr, 0);
                    m_doc.DocLogical.UpdateObjRepresentByLogicObj(ref atlinfo, atlinfo.iLogicObjPtr);
                }
            }
        }

        /// <summary>
        /// ѡ�нǶ�ת��Ϊ����
        /// </summary>
        /// <returns>�Ƕ�</returns>
        private string ConvertRotationToDirection(string rotation)
        {
            string[] sites = rotation.Split(new char[] { ',' });            
            float x = float.Parse(sites[0]);
            float y = float.Parse(sites[1]);
            float z = float.Parse(sites[2]);
            float w = float.Parse(sites[3]);
            int direction = 0;
            EngineLayer.ATLBase.QuaternionNDirByRotation(x, y, z, w, ref direction);
            
            return direction.ToString();
        }

        /// <summary>
        /// ����������ƶ��ٶ� ahpho suntao
        /// </summary>
        /// <param name="value">����ֵ</param>
        public void SetCameraSpeed(int value)
        {
            //SceneSceneEditor.SetCameraSpeed(100.0f * value);
        }

        public void SetRenderRegionEdge(bool bRenderEdge)
        {
            SceneSceneEditor.SetRenderBlockEdge(bRenderEdge?1:0);
        }

        public void SetCameraDis(float fDis)
        {
            SceneSceneEditor.SetCameraDis(fDis);
        }

        /// <summary>
        /// ��ʾ�����������
        /// </summary>
        /// <param name="textBox">��ʾ�õ��ı���</param>
        public void ShowCameraLookAt(TextBox textBox)
        {
            
        }


        /// <summary>
        /// ��ʼ��Ԥ����� ahpho suntao
        /// </summary>
        private void InitPreviewCanvas()
        {
            
        }
        
        /// <summary>
        /// ��ʼ�����ݿ�����
        /// </summary>
        private void InitSqlConnection()
        {
            IniStructure m_inis;
            string content;

            string fileName = Path.Combine(Application.StartupPath, "GameDesingerTools_Public.ini");
            FileInfo fi = new FileInfo(fileName);
            if (fi.Exists)
            {
                content = FileFolderHelper.FileToString(fileName);
                SymmetricCode sm = new SymmetricCode();
                content = sm.Decrypto(content);
                
                m_inis = IniStructure.ReadIniWithContent(content);
                string connectString = m_inis.GetValue("General", "ConnString");
                Conn = new SqlConnection(connectString);                
            }
            else
            {
                MessageBox.Show("��ȡ������Ϣʧ�ܣ���ȷ��GameDesingerTools_Public.ini�ļ��ڳ���Ŀ¼�У�", "��ʼ��",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            fileName = Path.Combine(Application.StartupPath, "GameDesingerTools.ini");
            fi = new FileInfo(fileName);
            if(fi.Exists)
            {
                content = FileFolderHelper.FileToString(fileName);
                m_inis = IniStructure.ReadIniWithContent(content);
                rootDir = m_inis.GetValue("General", "RootDir");
            }
        }                     

        /// <summary>
        /// ��ȡ�ֶζ�Ӧ����� ahpho suntao
        /// </summary>
        /// <param name="objectInfo">������Ϣ</param>
        /// <param name="field">�ֶ�</param>
        /// <returns>�ֶζ�Ӧ�����</returns>
        private int GetFieldIndex(_AtlObjInfo objectInfo, string field)
        {
            int index = -1;

            for(int i = 0; i < objectInfo.iFieldsCount; i++)
            {
                if(objectInfo.strFields[i] == field)
                {
                    index = i;
                    break;
                }
            }

            return index;
        }

        /// <summary>
        /// �༭������ѡ�ж��������
        /// </summary>
        public bool EditSelectedObjectsProperty(_AtlObjInfo[] objectInfoArray)
        {
            ObjectsEditForm editFrm = new ObjectsEditForm(m_doc, objectInfoArray);
            DialogResult dr = editFrm.ShowDialog();

            if (dr == DialogResult.OK)
            {
                return true;
            }
            else
            {
                return false;
            }
        }


        private _AtlVector3 GetAtlObjPos(_AtlObjInfo obj)
        {
            _AtlVector3 pos;
            pos.x = pos.y = pos.z = 0.0f;
            if (obj.strType == "NPC")
            {
                string[] _str = obj.strValues[16].Split(new char[] { ',' });
                pos.x = (float)(Convert.ToDouble(_str[0]));
                pos.y = (float)(Convert.ToDouble(_str[1]));
                pos.z = (float)(Convert.ToDouble(_str[2]));
            }
            return pos;
        }

        private void MoveCameraToPosition(_AtlVector3 newPos)
        {
            _AtlVector3 oldpos = new _AtlVector3(), oldlookat = new _AtlVector3();
            SceneSceneEditor.GetCameraPosLookat(ref oldpos, ref oldlookat);
            _AtlVector3 delta = new _AtlVector3();
            delta.x = oldlookat.x - oldpos.x; delta.y = oldlookat.y - oldpos.y; delta.z = oldlookat.z - oldpos.z;
            _AtlVector3 newpos = new _AtlVector3();
            newpos.x = newPos.x - delta.x; newpos.y = newPos.y - delta.y; newpos.z = newPos.z - delta.z;
            SceneSceneEditor.SetCameraPosLookat(ref newpos, ref newPos);
        }

        private void MoveCameraToRepresentObj(int representObjPtr)
        {
            _AtlVector3 objPos = new _AtlVector3();
            EngineLayer.ATLBase.GetRepresentObjTranslation(representObjPtr, ref objPos);

            MoveCameraToPosition(objPos);

            //_AtlVector3 oldpos = new _AtlVector3(), oldlookat = new _AtlVector3();
            //SceneSceneEditor.GetCameraPosLookat(ref oldpos, ref oldlookat);
            //_AtlVector3 delta = new _AtlVector3();
            //delta.x = oldlookat.x - oldpos.x; delta.y = oldlookat.y - oldpos.y; delta.z = oldlookat.z - oldpos.z;
            //_AtlVector3 newpos = new _AtlVector3();
            //newpos.x = objPos.x - delta.x; newpos.y = objPos.y - delta.y; newpos.z = objPos.z - delta.z;
            //SceneSceneEditor.SetCameraPosLookat(ref newpos, ref objPos);

            SceneSceneEditor.UpdateSelectableEntityList(5);
        }

        private void SelectOnlyRepresentObj(int representObjPtr)
        {
            SceneSceneEditor.ClearSelectedEntityList();
            SceneSceneEditor.AddSelectedRepresentObject(representObjPtr, 1);
            SceneSceneEditor.SetFirstSelectedKing();
        }

        /// <summary>
        /// ��������ƶ����������ڵ�λ��
        /// </summary>
        /// <param name="objectInfo">������Ϣ</param>
        private void MoveCameraToObject(_AtlObjInfo objectInfo)
        {
            // �ƶ���ͷ
            Hashtable infoTable = Helper.GetInfoTable(objectInfo);
            string positionStr = infoTable["vPosition"] as string;

            if (positionStr != null) // ��Ч������
            {
                string[] positionData = positionStr.Split(new char[] { ',' });
                float positionX = float.Parse(positionData[0]);
                float positionY = float.Parse(positionData[1]);
                float positionZ = float.Parse(positionData[2]);

                _AtlVector3 objPos = new _AtlVector3();
                objPos.x = positionX;
                objPos.y = positionY;
                objPos.z = positionZ;

                _AtlVector3 oldpos = new _AtlVector3(), oldlookat = new _AtlVector3();
                SceneSceneEditor.GetCameraPosLookat(ref oldpos, ref oldlookat);
                _AtlVector3 delta = new _AtlVector3();
                delta.x = oldlookat.x - oldpos.x; delta.y = oldlookat.y - oldpos.y; delta.z = oldlookat.z - oldpos.z;
                _AtlVector3 newpos = new _AtlVector3();
                newpos.x = objPos.x - delta.x; newpos.y = objPos.y - delta.y; newpos.z = objPos.z - delta.z;
                SceneSceneEditor.SetCameraLocation(newpos.x, newpos.y, newpos.z, objPos.x, objPos.y, objPos.z);
            }            
        }

        /// <summary>
        /// ����������� ahpho suntao
        /// </summary>
        /// <param name="x">x����</param>
        /// <param name="y">y����</param>
        /// <param name="z">z����</param>
        public void SetCameraFocus(int x, int y, int z)
        {
            _AtlVector3 cameraLocation = new _AtlVector3();
            _AtlVector3 cameraLookAt = new _AtlVector3();
            _AtlVector3 delta = new _AtlVector3();
            
            SceneSceneEditor.GetCameraPosLookat(ref cameraLocation, ref cameraLookAt);

            delta.x = cameraLookAt.x - cameraLocation.x; 
            delta.y = cameraLookAt.y - cameraLocation.y; 
            delta.z = cameraLookAt.z - cameraLocation.z;

            cameraLookAt.x = x;
            cameraLookAt.y = y;
            cameraLookAt.z = z;

            cameraLocation.x = x - delta.x;
            cameraLocation.y = y - delta.y;
            cameraLocation.z = z - delta.z;

            SceneSceneEditor.SetCameraPosLookat(ref cameraLocation, ref cameraLookAt);
        }

        private void SetFullScreen(bool bFullScreen)
        {
            this.SideCtrlBar.Visible = !bFullScreen;
            if (bFullScreen)
            {
                this.SideCtrlBar.Hide();
                this.SideCtrlBar.Refresh();
            }
            else
            {
                this.SideCtrlBar.Show();
                this.SideCtrlBar.Refresh();
            }
        }

        /// <summary>
        /// ��ȡ��������� ahpho suntao
        /// </summary>
        /// <returns>����������ַ���</returns>
        private string GetCameraLookAt()
        {
            _AtlVector3 cameraLocation = new _AtlVector3();
            _AtlVector3 cameraLookAt = new _AtlVector3();
            SceneSceneEditor.GetCameraPosLookat(ref cameraLocation, ref cameraLookAt);
            string cameraLookAtString = string.Format("{0},{1},{2}", (int)cameraLookAt.x, (int)cameraLookAt.y, (int)cameraLookAt.z);

            return cameraLookAtString;
        }

        /// <summary>
        /// ���¶�������
        /// </summary>
        /// <param name="objectInfo">������Ϣ</param>
        /// <param name="fieldName">��Ҫ���µ��ֶ�</param>
        /// <param name="fieldValue">��Ҫ���µ�ֵ</param>
        /// <param name="index">������</param>
        /// <param name="parentIndex">��������</param>
        private void UpdateObjectInfo(_AtlObjInfo objectInfo, string fieldName, string fieldValue, int index, int parentIndex)
        {
            int fieldIndex = GetFieldIndex(objectInfo, fieldName);

            if (fieldIndex >= 0)
            {
                string oldValue = objectInfo.strValues[fieldIndex];

                if (oldValue != fieldValue)
                {
                    string objectType = objectInfo.strType;
                    objectInfo.bChanged = 1;
                    m_doc.DocLogical.GetObjDisplayInfo(objectType, index, parentIndex, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
                    objectInfo.strValues[fieldIndex] = fieldValue;
                    m_doc.DocLogical.GetSetObjInfo(objectType, ref objectInfo, logicObj, 0);
                    m_doc.DocLogical.UpdateObjRepresentByLogicObj(ref objectInfo, logicObj);       
                }
            }
        }

        /// <summary>
        /// �����������
        /// </summary>
        /// <param name="currentCell">��ǰ�༭�ĵ�Ԫ��</param>
        /// <param name="objectInfo">������Ϣ</param>
        /// <param name="index">������</param>
        /// <param name="parentIndex">��������</param>
        private void SaveObjectData(DataGridViewCell currentCell, _AtlObjInfo objectInfo, int index, int parentIndex)
        {                            
            string newValue = currentCell.EditedFormattedValue as string;
            string oldValue = currentCell.Tag as string;
            string objectType = objectInfo.strType;                
            int fieldIndex = GetFieldIndex(objectInfo, currentCell.OwningRow.Tag as string);

            if (newValue == oldValue)
            {
                currentCell.Style.Font = normalTextFont;
            }
            else
            {
                currentCell.Style.Font = boldTextFont;
            }

            objectInfo.bChanged = 1;
            m_doc.DocLogical.GetObjDisplayInfo(objectType, index, parentIndex, ref name, ref nickName, ref hasScript, ref representObj, ref logicObj, ref templateID);
            objectInfo.strValues[fieldIndex] = newValue;
            m_doc.DocLogical.GetSetObjInfo(objectType, ref objectInfo, logicObj, 0);
            m_doc.DocLogical.UpdateObjRepresentByLogicObj(ref objectInfo, logicObj);            
        }

        private void BaseForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            bool cancelSave = false;
            DialogResult dr = MessageBox.Show("�ر�ǰ����Ҫ���浱ǰ������", "������ʾ", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
            if (dr == DialogResult.Cancel)
            {
                cancelSave = true;
            }
            else if (dr == DialogResult.Yes)
            {
                if (!Save())
                    cancelSave = true;
            }

            e.Cancel = cancelSave;
            if (!cancelSave)
                m_parent.ActiveFrm = null;
        }

        private void PanelView_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right && e.Clicks == 1 && m_ctrlPress)
            {
                cmRightClick.Popup(Control.MousePosition);
            }
        }

        private void buttonJmpToTree_Click(object sender, EventArgs e)
        {
            int entityCount = -1;
            SceneSceneEditor.GetSelectedEntityCount(ref entityCount);
            if (entityCount != 1)
            {
                MessageBox.Show("����ѡ��2�������ϵĶ�����ʹ�ô˹��ܡ�");
                return;
            }

            int iLogicObjPtr = 0;
            string objTypeName = string.Empty;
            m_doc.DocLogical.GetSelectedLogicObjPtr(0, ref iLogicObjPtr, ref objTypeName);

            switch (objTypeName)
            {
                case "LogicalPoly":
                    ToSelectPoly(iLogicObjPtr);
                    break;
                default:
                    break;
            }
        }

        private void buttonProp_Click(object sender, EventArgs e)
        {
            ShowProperties();
        }

        private void buttonDelSelObj_Click(object sender, EventArgs e)
        {
            DeleteSelectedObject();
        }

        /// <summary>
        /// �������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void ManageGroup(object sender, EventArgs e)
        {
            GroupManageForm gForm = new GroupManageForm();
            gForm.CurrentBaseDoc = m_doc;
            gForm.Init();
            gForm.ShowDialog();
        }

        private void buttonX27_Click(object sender, EventArgs e)
        {
            m_CheckRelive.Show();
        }
    }
}
