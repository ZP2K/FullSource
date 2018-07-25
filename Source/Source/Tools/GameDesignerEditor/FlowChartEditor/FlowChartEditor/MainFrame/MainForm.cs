using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.IO;
using System.Collections;
using System.Drawing.Imaging;
using System.Diagnostics;
using System.Reflection;

using DevComponents.DotNetBar;

using GraphElementLibrary;
using DBManager;

namespace FlowChartEditor
{
    public partial class MainForm : Office2007Form
    {                
        private DataBaseManager dataBaseManager; // ���ݿ������
        private DocumentManager documentManager; // ���������               

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
            // ��ʼ�����ݿ������
            dataBaseManager = DataBaseManager.GetDataBaseManager();                       

            // ��ʼ�����������
            documentManager = DocumentManager.GetDocumentManager();
            documentManager.TreeMenu = this.bTreeMenu;
            documentManager.UndoButton = this.bUndoCommand;
            documentManager.RedoButton = this.bRedoCommand;
            documentManager.CommandSwitchButton = this.bCommandSwitch;
            documentManager.CurrentTabControl = this.canvasGroupContainer;
            documentManager.TreeTabControl = this.treeContainer;
            documentManager.CurrentAbbreviateCanvas = this.abbreviateCanvas;
            documentManager.CurrentPropertyGrid = this.propertyGrid1;
            documentManager.LogBox = this.LogEditBox;
            documentManager.TreePanel = this.expandablePanel1;
            documentManager.TreeImageList = this.treeNodeImageList;
            documentManager.ScaleComboBox = this.canvasScaleBox;
            documentManager.AddDiagramRecordButton(this.bLatestDiagram1);
            documentManager.AddDiagramRecordButton(this.bLatestDiagram2);
            documentManager.AddDiagramRecordButton(this.bLatestDiagram3);
            documentManager.AddDiagramRecordButton(this.bLatestDiagram4);
            documentManager.AddDiagramRecordButton(this.bLatestDiagram5);
            documentManager.Init();
                       
            // ��ʾ���ݿ�����
            LDBName.Text = dataBaseManager.Connection.Database;

            #if (!DEBUG) // �Զ�������־��
                barLog.AutoHide = true; 
            #endif
        }

        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void MainForm_Load(object sender, EventArgs e)
        {
            try
            {                
                tabStrip1.MdiForm = this;
                
                // ��ȡ��������
                FileInfo fi = new FileInfo(Path.Combine(Application.StartupPath, "_flowchart_layout.xml"));
                if (fi.Exists)
                {
                    dotNetBarManager1.LoadLayout(Path.Combine(Application.StartupPath, "_flowchart_layout.xml")); 
                }

                // ��ʼ��״̬������
                LHostName.Text = Helper.GetHelper().GetHostName();
            }
            catch (IOException ex)
            {
                LogEditBox.AppendText(string.Format("��ʼ������ʱ�����쳣��{0}\r\n", ex.Message));
                this.Close();
            }
        }

        /// <summary>
        /// ����ر��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            dotNetBarManager1.SaveLayout(Path.Combine(Application.StartupPath, "_flowchart_layout.xml")); // ���沼������
            documentManager.SaveUserConfigInfo();

            if(documentManager.CheckAndSaveFlowCharts()) // �û�ȡ������
            {
                e.Cancel = true;
            }
        }

        /// <summary>
        /// �����ڹر��¼�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            documentManager.RecordLog();
        }                

        /// <summary>
        /// tabҳ�л��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void tabControl1_SelectedTabChanged(object sender, TabStripTabChangedEventArgs e)
        {
            documentManager.SwitchPanel();
        }     

        /// <summary>
        /// ����뿪Ԥ���ؼ��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void abbreviateCanvas_MouseLeave(object sender, EventArgs e)
        {
            abbreviateCanvas.EndMove();
        }

        /// <summary>
        /// �����Ԥ���ؼ����ƶ��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void abbreviateCanvas_MouseMove(object sender, MouseEventArgs e)
        {
            abbreviateCanvas.MoveRegion(e.Location);
        }

        /// <summary>
        /// �����Ԥ���ؼ��ϰ����¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void abbreviateCanvas_MouseDown(object sender, MouseEventArgs e)
        {
            abbreviateCanvas.SelectRegion(e.Location);
        }

        /// <summary>
        /// �����Ԥ���ؼ��Ϸſ��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void abbreviateCanvas_MouseUp(object sender, MouseEventArgs e)
        {
            abbreviateCanvas.EndMove(e.Location);
        }        

        /// <summary>
        /// �½�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bNewClass_Click(object sender, EventArgs e)
        {
            documentManager.AddClass();
        }

        /// <summary>
        /// �½���ͼ
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void bNewFlowChart_Click(object sender, EventArgs e)
        {
            documentManager.AddFlowChart();
        }

        /// <summary>
        /// ���������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bCutNode_Click(object sender, EventArgs e)
        {
            documentManager.CutNode();
        }

        /// <summary>
        /// ���������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bCopyNode_Click(object sender, EventArgs e)
        {
            documentManager.CopyNode();
        }

        /// <summary>
        /// ճ�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bPasteNode_Click(object sender, EventArgs e)
        {
            documentManager.PasteNode();
        }

        /// <summary>
        /// ɾ�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bDeleteNode_Click(object sender, EventArgs e)
        {
            documentManager.DeleteNode();
        }

        /// <summary>
        /// �����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bRenameNode_Click(object sender, EventArgs e)
        {
            documentManager.RenameNode();
        }

        /// <summary>
        /// �رջ�ͼ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void tabControl1_TabItemClose(object sender, TabStripActionEventArgs e)
        {
            documentManager.CheckAndSaveFlowChart();
        }      

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bSaveFlowChart_Click(object sender, EventArgs e)
        {
            // ���浱ǰ��ͼ
            documentManager.SaveCurrentFlowChart();           
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bUndoCommand_Click(object sender, EventArgs e)
        {
            documentManager.Undo();
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bRedoCommand_Click(object sender, EventArgs e)
        {            
            documentManager.Redo();
        }
        
        /// <summary>
        /// ȫѡ��ǰͼԪ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bSelectAddGraphElement_Click(object sender, EventArgs e)
        {
            documentManager.SelectAllGraphElement();
        }

        /// <summary>
        /// �������Ĳ��������ı�
        /// </summary>
        /// <param name="s">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void propertyGrid1_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            ;
        }

        /// <summary>
        /// ����ͼ����Ч��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCheckCircle_Click(object sender, EventArgs e)
        {
            documentManager.CheckFlowChartAvail();
        }

        /// <summary>
        /// ���ó������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bSetArgument_Click(object sender, EventArgs e)
        {
            documentManager.SetArgument();
        }

        /// <summary>
        /// ���ɵ�ǰ����ͼ�Ĵ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bBuildFlowChart_Click(object sender, EventArgs e)
        {
            documentManager.GenerateCode();
        }

        /// <summary>
        /// ��Ŀ¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bOpenFolder_Click(object sender, EventArgs e)
        {
            documentManager.OpenFolder();
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCutFlowChart_Click(object sender, EventArgs e)
        {
            documentManager.CutGraphElement();
        }

        /// <summary>
        /// �鿴����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bViewCode_Click(object sender, EventArgs e)
        {
            documentManager.ViewCode();
        }

        /// <summary>
        /// �л�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void tabControl3_SelectedTabChanged(object sender, TabStripTabChangedEventArgs e)
        {
            documentManager.SwitchClass();
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bEditVar_Click(object sender, EventArgs e)
        {
            documentManager.EditVar();
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCopyGraphElements_Click(object sender, EventArgs e)
        {
            documentManager.CopyGraphElement();
        }

        /// <summary>
        /// ����ͼԪ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCutGraphElements_Click(object sender, EventArgs e)
        {
            documentManager.CutGraphElement();
        }

        /// <summary>
        /// ճ��ͼԪ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bPasteGraphElements_Click(object sender, EventArgs e)
        {
            documentManager.PasteGraphElement();
        }

        /// <summary>
        /// �ı��ͼ��Ĵ�С
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxItem1_SelectedIndexChanged(object sender, EventArgs e)
        {
            /* ��Ϊ֧�ֵĲ����Ǻ����ƣ���ʱ���ε����Ź���
            float scaleArgument = 1.0f;            
            bool setMaxSize = false;
            
            switch (canvasScaleBox.SelectedIndex)
            {
                case 0: // 25%
                    {
                        scaleArgument = 0.25f;
                        break;
                    }
                case 1: // 50%
                    {
                        scaleArgument = 0.5f;
                        break;
                    }
                case 2: // 75%
                    {
                        scaleArgument = 0.75f;
                        break;
                    }
                case 3: // 100%
                    {
                        scaleArgument = 1.0f;
                        break;
                    }
                case 4: // ���
                    {

                        setMaxSize = true;
                        break;
                    }
            }
            
            if (setMaxSize)
            {
                groupPanel1.Visible = false;
                barLog.Visible = false;
                documentManager.ChangeCanvasScale(1.0f);
                // documentManager.MaximizeCanvas();
            }
            else
            {
                groupPanel1.Visible = true;
                barLog.Visible = true;
                documentManager.ChangeCanvasScale(scaleArgument);
            }
            */

            switch (canvasScaleBox.SelectedIndex)
            {
                case 0: // ������С
                    {
                        groupPanel1.Visible = true;
                        barLog.Visible = true;
                        
                        break;
                    }
                case 1: // ���
                    {
                        groupPanel1.Visible = false;
                        barLog.Visible = false;

                        break;
                    }
            }                        
        }

        /// <summary>
        /// �鿴��־
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bViewLog_Click(object sender, EventArgs e)
        {
            documentManager.ViewLog();
        }

        /// <summary>
        /// �ָ���ͼ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRestoreNode_Click(object sender, EventArgs e)
        {
            documentManager.RestoreDiagram();
        }

        /// <summary>
        /// �鿴������Ϣ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAbout_Click(object sender, EventArgs e)
        {
            MessageBox.Show(string.Format("��ͼ�ű����ɹ��� {0} ��\r\n\r\n����������������", Application.ProductVersion), "������Ϣ", 
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        /// <summary>
        /// �˳�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void ExitApplication_Click(object sender, EventArgs e)
        {
            this.Close();
        }  

        /// <summary>
        /// ������򿪵Ļ�ͼ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void OpenLatestDiagram(object sender, EventArgs e)
        {
            ButtonItem buttonItem = sender as ButtonItem;
            documentManager.OpenDiagram(buttonItem.Tag.ToString());
        }

        /// <summary>
        /// �鿴�����ĵ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bHelpDoc_Click(object sender, EventArgs e)
        {
            documentManager.ViewHelpDoc();
        }

        /// <summary>
        /// �༭����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bEditProperty_Click(object sender, EventArgs e)
        {
            documentManager.ViewChartProperty();
        }

        /// <summary>
        /// ���ұ༭
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void cmdFindRecord_Click(object sender, EventArgs e)
        {
            documentManager.BeginSearch();
        }

        /// <summary>
        /// �л�����֧��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCommandSwitch_Click(object sender, EventArgs e)
        {
            documentManager.SwitchCommandState();
        }

        /// <summary>
        /// ��ͼ���ݱ��浽�����ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bSaveToFile_Click(object sender, EventArgs e)
        {
            documentManager.SaveDiagramDataToFile();
        }

        /// <summary>
        /// �ӱ����ļ����ػ�ͼ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e"></param>
        private void bLoadFromFile_Click(object sender, EventArgs e)
        {
            documentManager.LoadDiagramDataFromFile();
        }

        /// <summary>
        /// �ָ���ͼ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRestoreDiagram_Click(object sender, EventArgs e)
        {
            documentManager.ViewRecycle();
        }

        /// <summary>
        /// �鿴�ӻ�ͼ���ù�ϵ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bViewInnerChartReference_Click(object sender, EventArgs e)
        {
            documentManager.ViewInnerChartReference();
        }

        /// <summary>
        /// ����Ǩ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bTransferData_Click(object sender, EventArgs e)
        {
            documentManager.TransferData();
        }
    }

    /// <summary>
    /// �ĵ�����ӿ�
    /// </summary>
    public interface IKDocument
    {
        void ExecCommand(string strCmd, object data);
    }
}