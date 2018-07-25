namespace DesignerSceneEditor
{
    partial class TopViewForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.bTabContext = new DevComponents.DotNetBar.ButtonItem();
            this.cmdContextSave = new DevComponents.DotNetBar.ButtonItem();
            this.cmdContextClose = new DevComponents.DotNetBar.ButtonItem();
            this.bTabColor = new DevComponents.DotNetBar.ButtonItem();
            this.cmNodeRightClick = new DevComponents.DotNetBar.ButtonItem();
            this.buttonAddSameLevel = new DevComponents.DotNetBar.ButtonItem();
            this.buttonAddSub = new DevComponents.DotNetBar.ButtonItem();
            this.buttonRename = new DevComponents.DotNetBar.ButtonItem();
            this.buttonCopy = new DevComponents.DotNetBar.ButtonItem();
            this.buttonPaste = new DevComponents.DotNetBar.ButtonItem();
            this.buttonDelete = new DevComponents.DotNetBar.ButtonItem();
            this.cmScriptOp = new DevComponents.DotNetBar.ButtonItem();
            this.bShowLuaCode = new DevComponents.DotNetBar.ButtonItem();
            this.bShowMetainfo = new DevComponents.DotNetBar.ButtonItem();
            this.bEditFile = new DevComponents.DotNetBar.ButtonItem();
            this.bDeleteRecord = new DevComponents.DotNetBar.ButtonItem();
            this.bClearContent = new DevComponents.DotNetBar.ButtonItem();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.miView = new System.Windows.Forms.ToolStripMenuItem();
            this.miTerrain = new System.Windows.Forms.ToolStripMenuItem();
            this.miTerrainLight = new System.Windows.Forms.ToolStripMenuItem();
            this.miTerrainZoneEdge = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.miWater = new System.Windows.Forms.ToolStripMenuItem();
            this.miGrass = new System.Windows.Forms.ToolStripMenuItem();
            this.miFrutex = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.miObjectFar = new System.Windows.Forms.ToolStripMenuItem();
            this.miObjectMedium = new System.Windows.Forms.ToolStripMenuItem();
            this.miObjectNear = new System.Windows.Forms.ToolStripMenuItem();
            this.miObjectPostRender = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.miCamera = new System.Windows.Forms.ToolStripMenuItem();
            this.miRole = new System.Windows.Forms.ToolStripMenuItem();
            this.miCameraAniPoint = new System.Windows.Forms.ToolStripMenuItem();
            this.miQuadTree = new System.Windows.Forms.ToolStripMenuItem();
            this.miOperation = new System.Windows.Forms.ToolStripMenuItem();
            this.miRectSelect = new System.Windows.Forms.ToolStripMenuItem();
            this.miPolygonSelect = new System.Windows.Forms.ToolStripMenuItem();
            this.miBlockPolygon = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.miSelectAllObject = new System.Windows.Forms.ToolStripMenuItem();
            this.miMapManage = new System.Windows.Forms.ToolStripMenuItem();
            this.miLoadNewMap = new System.Windows.Forms.ToolStripMenuItem();
            this.miLoadDefault = new System.Windows.Forms.ToolStripMenuItem();
            this.miObjectManage = new System.Windows.Forms.ToolStripMenuItem();
            this.miTodo = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.panelEx1 = new DevComponents.DotNetBar.PanelEx();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textZCount = new DevComponents.DotNetBar.Controls.TextBoxX();
            this.textZStart = new DevComponents.DotNetBar.Controls.TextBoxX();
            this.textXCount = new DevComponents.DotNetBar.Controls.TextBoxX();
            this.labelX3 = new DevComponents.DotNetBar.LabelX();
            this.labelX2 = new DevComponents.DotNetBar.LabelX();
            this.textXStart = new DevComponents.DotNetBar.Controls.TextBoxX();
            this.labelX1 = new DevComponents.DotNetBar.LabelX();
            this.checkShowAppoint = new DevComponents.DotNetBar.Controls.CheckBoxX();
            this.buttonX1 = new DevComponents.DotNetBar.ButtonX();
            this.panel1 = new DesignerSceneEditor.MyPanel();
            this.menuStrip1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.panelEx1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // bTabContext
            // 
            this.bTabContext.AutoExpandOnClick = true;
            this.bTabContext.GlobalName = "bTabContext";
            this.bTabContext.ImagePaddingHorizontal = 8;
            this.bTabContext.Name = "bTabContext";
            this.bTabContext.SubItems.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.cmdContextSave,
            this.cmdContextClose,
            this.bTabColor});
            this.bTabContext.Text = "bTabContext";
            // 
            // cmdContextSave
            // 
            this.cmdContextSave.GlobalItem = false;
            this.cmdContextSave.ImageIndex = 17;
            this.cmdContextSave.ImagePaddingHorizontal = 8;
            this.cmdContextSave.Name = "cmdContextSave";
            this.cmdContextSave.Text = "����";
            this.cmdContextSave.Tooltip = "�����ĵ�";
            // 
            // cmdContextClose
            // 
            this.cmdContextClose.Enabled = false;
            this.cmdContextClose.GlobalItem = false;
            this.cmdContextClose.ImagePaddingHorizontal = 8;
            this.cmdContextClose.Name = "cmdContextClose";
            this.cmdContextClose.Text = "�ر�";
            this.cmdContextClose.Tooltip = "�رջ�ĵ�";
            // 
            // bTabColor
            // 
            this.bTabColor.BeginGroup = true;
            this.bTabColor.GlobalName = "bTabColor";
            this.bTabColor.ImagePaddingHorizontal = 8;
            this.bTabColor.Name = "bTabColor";
            this.bTabColor.PopupType = DevComponents.DotNetBar.ePopupType.Container;
            this.bTabColor.Text = "���ı�ǩ����ɫ";
            // 
            // cmNodeRightClick
            // 
            this.cmNodeRightClick.AutoExpandOnClick = true;
            this.cmNodeRightClick.ImagePaddingHorizontal = 8;
            this.cmNodeRightClick.Name = "cmNodeRightClick";
            this.cmNodeRightClick.SubItems.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.buttonAddSameLevel,
            this.buttonAddSub,
            this.buttonRename,
            this.buttonCopy,
            this.buttonPaste,
            this.buttonDelete});
            this.cmNodeRightClick.Text = "cmNodeRightClick";
            // 
            // buttonAddSameLevel
            // 
            this.buttonAddSameLevel.ImagePaddingHorizontal = 8;
            this.buttonAddSameLevel.Name = "buttonAddSameLevel";
            this.buttonAddSameLevel.Text = "��ӱ���";
            // 
            // buttonAddSub
            // 
            this.buttonAddSub.ImagePaddingHorizontal = 8;
            this.buttonAddSub.Name = "buttonAddSub";
            this.buttonAddSub.Text = "����Ӳ�";
            // 
            // buttonRename
            // 
            this.buttonRename.BeginGroup = true;
            this.buttonRename.ImagePaddingHorizontal = 8;
            this.buttonRename.Name = "buttonRename";
            this.buttonRename.Text = "������";
            // 
            // buttonCopy
            // 
            this.buttonCopy.ImagePaddingHorizontal = 8;
            this.buttonCopy.Name = "buttonCopy";
            this.buttonCopy.Text = "����";
            // 
            // buttonPaste
            // 
            this.buttonPaste.ImagePaddingHorizontal = 8;
            this.buttonPaste.Name = "buttonPaste";
            this.buttonPaste.Text = "ճ��";
            // 
            // buttonDelete
            // 
            this.buttonDelete.BeginGroup = true;
            this.buttonDelete.ImagePaddingHorizontal = 8;
            this.buttonDelete.Name = "buttonDelete";
            this.buttonDelete.Text = "ɾ��";
            // 
            // cmScriptOp
            // 
            this.cmScriptOp.AutoExpandOnClick = true;
            this.cmScriptOp.ImagePaddingHorizontal = 8;
            this.cmScriptOp.Name = "cmScriptOp";
            this.cmScriptOp.SubItems.AddRange(new DevComponents.DotNetBar.BaseItem[] {
            this.bShowLuaCode,
            this.bShowMetainfo,
            this.bEditFile,
            this.bDeleteRecord,
            this.bClearContent});
            this.cmScriptOp.Text = "cmScriptOp";
            // 
            // bShowLuaCode
            // 
            this.bShowLuaCode.ImagePaddingHorizontal = 8;
            this.bShowLuaCode.Name = "bShowLuaCode";
            this.bShowLuaCode.Text = "��ʾ����(&S)";
            // 
            // bShowMetainfo
            // 
            this.bShowMetainfo.BeginGroup = true;
            this.bShowMetainfo.ImagePaddingHorizontal = 8;
            this.bShowMetainfo.Name = "bShowMetainfo";
            this.bShowMetainfo.Text = "�༭Ԫ��Ϣ(&X)";
            // 
            // bEditFile
            // 
            this.bEditFile.ImagePaddingHorizontal = 8;
            this.bEditFile.Name = "bEditFile";
            this.bEditFile.Text = "�༭�ļ�(&E)";
            // 
            // bDeleteRecord
            // 
            this.bDeleteRecord.BeginGroup = true;
            this.bDeleteRecord.ImagePaddingHorizontal = 8;
            this.bDeleteRecord.Name = "bDeleteRecord";
            this.bDeleteRecord.Text = "ɾ���ӱ��¼(&D)";
            // 
            // bClearContent
            // 
            this.bClearContent.ImagePaddingHorizontal = 8;
            this.bClearContent.Name = "bClearContent";
            this.bClearContent.Text = "���";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miView,
            this.miOperation,
            this.miMapManage,
            this.miObjectManage});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(529, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // miView
            // 
            this.miView.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miTerrain,
            this.miTerrainLight,
            this.miTerrainZoneEdge,
            this.toolStripSeparator2,
            this.miWater,
            this.miGrass,
            this.miFrutex,
            this.toolStripSeparator3,
            this.miObjectFar,
            this.miObjectMedium,
            this.miObjectNear,
            this.miObjectPostRender,
            this.toolStripSeparator4,
            this.miCamera,
            this.miRole,
            this.miCameraAniPoint,
            this.miQuadTree});
            this.miView.Name = "miView";
            this.miView.Size = new System.Drawing.Size(41, 20);
            this.miView.Text = "�鿴";
            // 
            // miTerrain
            // 
            this.miTerrain.Name = "miTerrain";
            this.miTerrain.Size = new System.Drawing.Size(160, 22);
            this.miTerrain.Text = "�ر�";
            // 
            // miTerrainLight
            // 
            this.miTerrainLight.Name = "miTerrainLight";
            this.miTerrainLight.Size = new System.Drawing.Size(160, 22);
            this.miTerrainLight.Text = "�ر����";
            // 
            // miTerrainZoneEdge
            // 
            this.miTerrainZoneEdge.Name = "miTerrainZoneEdge";
            this.miTerrainZoneEdge.Size = new System.Drawing.Size(160, 22);
            this.miTerrainZoneEdge.Text = "�ر�����߽�";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(157, 6);
            // 
            // miWater
            // 
            this.miWater.Name = "miWater";
            this.miWater.Size = new System.Drawing.Size(160, 22);
            this.miWater.Text = "ˮ��";
            // 
            // miGrass
            // 
            this.miGrass.Name = "miGrass";
            this.miGrass.Size = new System.Drawing.Size(160, 22);
            this.miGrass.Text = "��";
            // 
            // miFrutex
            // 
            this.miFrutex.Name = "miFrutex";
            this.miFrutex.Size = new System.Drawing.Size(160, 22);
            this.miFrutex.Text = "��ľ";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(157, 6);
            // 
            // miObjectFar
            // 
            this.miObjectFar.Name = "miObjectFar";
            this.miObjectFar.Size = new System.Drawing.Size(160, 22);
            this.miObjectFar.Text = "��� - Զ����";
            // 
            // miObjectMedium
            // 
            this.miObjectMedium.Name = "miObjectMedium";
            this.miObjectMedium.Size = new System.Drawing.Size(160, 22);
            this.miObjectMedium.Text = "��� - �о���";
            // 
            // miObjectNear
            // 
            this.miObjectNear.Name = "miObjectNear";
            this.miObjectNear.Size = new System.Drawing.Size(160, 22);
            this.miObjectNear.Text = "��� - ������";
            // 
            // miObjectPostRender
            // 
            this.miObjectPostRender.Name = "miObjectPostRender";
            this.miObjectPostRender.Size = new System.Drawing.Size(160, 22);
            this.miObjectPostRender.Text = "��� - �����Ⱦ";
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(157, 6);
            // 
            // miCamera
            // 
            this.miCamera.Name = "miCamera";
            this.miCamera.Size = new System.Drawing.Size(160, 22);
            this.miCamera.Text = "�����������";
            // 
            // miRole
            // 
            this.miRole.Name = "miRole";
            this.miRole.Size = new System.Drawing.Size(160, 22);
            this.miRole.Text = "ģ������";
            // 
            // miCameraAniPoint
            // 
            this.miCameraAniPoint.Name = "miCameraAniPoint";
            this.miCameraAniPoint.Size = new System.Drawing.Size(160, 22);
            this.miCameraAniPoint.Text = "�����������";
            // 
            // miQuadTree
            // 
            this.miQuadTree.Name = "miQuadTree";
            this.miQuadTree.Size = new System.Drawing.Size(160, 22);
            this.miQuadTree.Text = "�����Ĳ���";
            // 
            // miOperation
            // 
            this.miOperation.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miRectSelect,
            this.miPolygonSelect,
            this.miBlockPolygon,
            this.toolStripSeparator1,
            this.miSelectAllObject});
            this.miOperation.Name = "miOperation";
            this.miOperation.Size = new System.Drawing.Size(41, 20);
            this.miOperation.Text = "����";
            // 
            // miRectSelect
            // 
            this.miRectSelect.Name = "miRectSelect";
            this.miRectSelect.Size = new System.Drawing.Size(190, 22);
            this.miRectSelect.Text = "����ѡ������";
            // 
            // miPolygonSelect
            // 
            this.miPolygonSelect.Name = "miPolygonSelect";
            this.miPolygonSelect.Size = new System.Drawing.Size(190, 22);
            this.miPolygonSelect.Text = "���������ѡ��";
            // 
            // miBlockPolygon
            // 
            this.miBlockPolygon.Name = "miBlockPolygon";
            this.miBlockPolygon.Size = new System.Drawing.Size(190, 22);
            this.miBlockPolygon.Text = "�ϰ�����������ѡ��";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(187, 6);
            // 
            // miSelectAllObject
            // 
            this.miSelectAllObject.Name = "miSelectAllObject";
            this.miSelectAllObject.Size = new System.Drawing.Size(190, 22);
            this.miSelectAllObject.Text = "�����������";
            // 
            // miMapManage
            // 
            this.miMapManage.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miLoadNewMap,
            this.miLoadDefault});
            this.miMapManage.Name = "miMapManage";
            this.miMapManage.Size = new System.Drawing.Size(65, 20);
            this.miMapManage.Text = "��ͼ����";
            // 
            // miLoadNewMap
            // 
            this.miLoadNewMap.Name = "miLoadNewMap";
            this.miLoadNewMap.Size = new System.Drawing.Size(202, 22);
            this.miLoadNewMap.Text = "����һ���µ�ͼ���ó���";
            // 
            // miLoadDefault
            // 
            this.miLoadDefault.Name = "miLoadDefault";
            this.miLoadDefault.Size = new System.Drawing.Size(202, 22);
            this.miLoadDefault.Text = "����Ĭ�ϵ���";
            // 
            // miObjectManage
            // 
            this.miObjectManage.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miTodo});
            this.miObjectManage.Name = "miObjectManage";
            this.miObjectManage.Size = new System.Drawing.Size(65, 20);
            this.miObjectManage.Text = "�������";
            // 
            // miTodo
            // 
            this.miTodo.Name = "miTodo";
            this.miTodo.Size = new System.Drawing.Size(118, 22);
            this.miTodo.Text = "ɶ��û��";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.panelEx1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.panel1);
            this.splitContainer1.Size = new System.Drawing.Size(769, 500);
            this.splitContainer1.SplitterDistance = 219;
            this.splitContainer1.TabIndex = 0;
            // 
            // panelEx1
            // 
            this.panelEx1.CanvasColor = System.Drawing.SystemColors.Control;
            this.panelEx1.Controls.Add(this.groupBox1);
            this.panelEx1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelEx1.Location = new System.Drawing.Point(0, 0);
            this.panelEx1.Name = "panelEx1";
            this.panelEx1.Size = new System.Drawing.Size(219, 500);
            this.panelEx1.Style.Alignment = System.Drawing.StringAlignment.Center;
            this.panelEx1.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(227)))), ((int)(((byte)(239)))), ((int)(((byte)(255)))));
            this.panelEx1.Style.BackColor2.Color = System.Drawing.Color.FromArgb(((int)(((byte)(169)))), ((int)(((byte)(206)))), ((int)(((byte)(254)))));
            this.panelEx1.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
            this.panelEx1.Style.BorderColor.ColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelBorder;
            this.panelEx1.Style.ForeColor.ColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelText;
            this.panelEx1.Style.GradientAngle = 90;
            this.panelEx1.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textZCount);
            this.groupBox1.Controls.Add(this.textZStart);
            this.groupBox1.Controls.Add(this.textXCount);
            this.groupBox1.Controls.Add(this.labelX3);
            this.groupBox1.Controls.Add(this.labelX2);
            this.groupBox1.Controls.Add(this.textXStart);
            this.groupBox1.Controls.Add(this.labelX1);
            this.groupBox1.Controls.Add(this.checkShowAppoint);
            this.groupBox1.Controls.Add(this.buttonX1);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(216, 178);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "�߼�����";
            // 
            // textZCount
            // 
            // 
            // 
            // 
            this.textZCount.Border.Class = "TextBoxBorder";
            this.textZCount.Location = new System.Drawing.Point(155, 102);
            this.textZCount.Name = "textZCount";
            this.textZCount.Size = new System.Drawing.Size(41, 21);
            this.textZCount.TabIndex = 3;
            this.textZCount.TextChanged += new System.EventHandler(this.AppointParamsChanged);
            // 
            // textZStart
            // 
            // 
            // 
            // 
            this.textZStart.Border.Class = "TextBoxBorder";
            this.textZStart.Location = new System.Drawing.Point(155, 75);
            this.textZStart.Name = "textZStart";
            this.textZStart.Size = new System.Drawing.Size(41, 21);
            this.textZStart.TabIndex = 3;
            this.textZStart.TextChanged += new System.EventHandler(this.AppointParamsChanged);
            // 
            // textXCount
            // 
            // 
            // 
            // 
            this.textXCount.Border.Class = "TextBoxBorder";
            this.textXCount.Location = new System.Drawing.Point(99, 102);
            this.textXCount.Name = "textXCount";
            this.textXCount.Size = new System.Drawing.Size(41, 21);
            this.textXCount.TabIndex = 3;
            this.textXCount.TextChanged += new System.EventHandler(this.AppointParamsChanged);
            // 
            // labelX3
            // 
            this.labelX3.Location = new System.Drawing.Point(18, 140);
            this.labelX3.Name = "labelX3";
            this.labelX3.Size = new System.Drawing.Size(178, 23);
            this.labelX3.TabIndex = 2;
            this.labelX3.Text = "(����ѡ�����ָ������)";
            // 
            // labelX2
            // 
            this.labelX2.Location = new System.Drawing.Point(18, 102);
            this.labelX2.Name = "labelX2";
            this.labelX2.Size = new System.Drawing.Size(75, 23);
            this.labelX2.TabIndex = 2;
            this.labelX2.Text = "����";
            // 
            // textXStart
            // 
            // 
            // 
            // 
            this.textXStart.Border.Class = "TextBoxBorder";
            this.textXStart.Location = new System.Drawing.Point(99, 75);
            this.textXStart.Name = "textXStart";
            this.textXStart.Size = new System.Drawing.Size(41, 21);
            this.textXStart.TabIndex = 3;
            this.textXStart.TextChanged += new System.EventHandler(this.AppointParamsChanged);
            // 
            // labelX1
            // 
            this.labelX1.Location = new System.Drawing.Point(18, 75);
            this.labelX1.Name = "labelX1";
            this.labelX1.Size = new System.Drawing.Size(75, 23);
            this.labelX1.TabIndex = 2;
            this.labelX1.Text = "���(Index)";
            // 
            // checkShowAppoint
            // 
            this.checkShowAppoint.Location = new System.Drawing.Point(112, 33);
            this.checkShowAppoint.Name = "checkShowAppoint";
            this.checkShowAppoint.Size = new System.Drawing.Size(98, 23);
            this.checkShowAppoint.TabIndex = 1;
            this.checkShowAppoint.Text = "��ʾָ������";
            this.checkShowAppoint.CheckedChanged += new System.EventHandler(this.checkShowAppoint_CheckedChanged);
            // 
            // buttonX1
            // 
            this.buttonX1.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.buttonX1.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
            this.buttonX1.Location = new System.Drawing.Point(18, 33);
            this.buttonX1.Name = "buttonX1";
            this.buttonX1.Size = new System.Drawing.Size(88, 23);
            this.buttonX1.TabIndex = 0;
            this.buttonX1.Text = "ѡ���߼�����";
            this.buttonX1.Click += new System.EventHandler(this.buttonX1_Click);
            // 
            // panel1
            // 
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(546, 500);
            this.panel1.TabIndex = 0;
            this.panel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseMove);
            this.panel1.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseDoubleClick);
            this.panel1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseDown);
            this.panel1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.panel1_MouseUp);
            this.panel1.SizeChanged += new System.EventHandler(this.panel1_Resize);
            // 
            // TopViewForm
            // 
            this.ClientSize = new System.Drawing.Size(769, 500);
            this.Controls.Add(this.splitContainer1);
            this.KeyPreview = true;
            this.Name = "TopViewForm";
            this.Text = "����ͼ";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.TopViewForm_FormClosing);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.TopViewForm_KeyDown);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.panelEx1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private DevComponents.DotNetBar.ButtonItem bTabContext;
        private DevComponents.DotNetBar.ButtonItem cmdContextSave;
        private DevComponents.DotNetBar.ButtonItem cmdContextClose;
        private DevComponents.DotNetBar.ButtonItem bTabColor;
        private DevComponents.DotNetBar.ButtonItem cmNodeRightClick;
        private DevComponents.DotNetBar.ButtonItem buttonAddSameLevel;
        private DevComponents.DotNetBar.ButtonItem buttonAddSub;
        private DevComponents.DotNetBar.ButtonItem buttonRename;
        private DevComponents.DotNetBar.ButtonItem buttonCopy;
        private DevComponents.DotNetBar.ButtonItem buttonPaste;
        private DevComponents.DotNetBar.ButtonItem buttonDelete;
        private DevComponents.DotNetBar.ButtonItem cmScriptOp;
        private DevComponents.DotNetBar.ButtonItem bShowLuaCode;
        private DevComponents.DotNetBar.ButtonItem bShowMetainfo;
        private DevComponents.DotNetBar.ButtonItem bEditFile;
        private DevComponents.DotNetBar.ButtonItem bDeleteRecord;
        private DevComponents.DotNetBar.ButtonItem bClearContent;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem miView;
        private System.Windows.Forms.ToolStripMenuItem miTerrain;
        private System.Windows.Forms.ToolStripMenuItem miTerrainLight;
        private System.Windows.Forms.ToolStripMenuItem miTerrainZoneEdge;
        private System.Windows.Forms.ToolStripMenuItem miOperation;
        private System.Windows.Forms.ToolStripMenuItem miMapManage;
        private System.Windows.Forms.ToolStripMenuItem miObjectManage;
        private System.Windows.Forms.ToolStripMenuItem miWater;
        private System.Windows.Forms.ToolStripMenuItem miGrass;
        private System.Windows.Forms.ToolStripMenuItem miFrutex;
        private System.Windows.Forms.ToolStripMenuItem miObjectFar;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem miObjectMedium;
        private System.Windows.Forms.ToolStripMenuItem miObjectNear;
        private System.Windows.Forms.ToolStripMenuItem miObjectPostRender;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem miCamera;
        private System.Windows.Forms.ToolStripMenuItem miRole;
        private System.Windows.Forms.ToolStripMenuItem miCameraAniPoint;
        private System.Windows.Forms.ToolStripMenuItem miQuadTree;
        private System.Windows.Forms.ToolStripMenuItem miRectSelect;
        private System.Windows.Forms.ToolStripMenuItem miPolygonSelect;
        private System.Windows.Forms.ToolStripMenuItem miBlockPolygon;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem miSelectAllObject;
        private System.Windows.Forms.ToolStripMenuItem miLoadNewMap;
        private System.Windows.Forms.ToolStripMenuItem miLoadDefault;
        private System.Windows.Forms.ToolStripMenuItem miTodo;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private MyPanel panel1;
        private DevComponents.DotNetBar.PanelEx panelEx1;
        private System.Windows.Forms.GroupBox groupBox1;
        private DevComponents.DotNetBar.Controls.TextBoxX textZCount;
        private DevComponents.DotNetBar.Controls.TextBoxX textZStart;
        private DevComponents.DotNetBar.Controls.TextBoxX textXCount;
        private DevComponents.DotNetBar.LabelX labelX2;
        private DevComponents.DotNetBar.Controls.TextBoxX textXStart;
        private DevComponents.DotNetBar.LabelX labelX1;
        private DevComponents.DotNetBar.Controls.CheckBoxX checkShowAppoint;
        private DevComponents.DotNetBar.ButtonX buttonX1;
        private DevComponents.DotNetBar.LabelX labelX3;
    }
}