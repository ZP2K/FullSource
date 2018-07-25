using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Drawing;
using System.Windows.Forms;
using System.Net;
using System.Reflection;
using System.Collections;
using DevComponents.DotNetBar.Controls;

namespace FlowChartEditor
{
    public class Helper
    {
        private static Helper helper; // Ψһ��˽��ʵ��

        private Image actionNodeNormalImage; // ���������ͨ״̬�µ�ͼƬ
        private Image actionNodeSelectedImage; // ������㱻ѡ��״̬�µ�ͼƬ
        private Image actionNodeActivatedImage; // ������㱻����״̬�µ�ͼƬ
        private Image actionNodeShadowImage; // ����������ӰͼƬ
        private Image actionNodeDisableNormalImage; // ���������Ч��ͨ״̬�µ�ͼƬ
        private Image actionNodeDisableSelectedImage; // ���������Ч��ѡ��״̬�µ�ͼƬ
        private Image actionNodeDisableActivatedImage; // ���������Ч������״̬�µ�ͼƬ
        private Image actionNodeDisableShadowImage; // ���������Ч״̬�µ���ӰͼƬ
        private Image actionNodeAbbreviateImage; // ������������ͼƬ
        private Image actionNodeDisableAbbreviateImage; // ���������Ч״̬�µ�����ͼƬ

        private Image conditionNodeNormalImage; // ���������ͨ״̬�µ�ͼƬ
        private Image conditionNodeSelectedImage; // ������㱻ѡ��״̬�µ�ͼƬ
        private Image conditionNodeActivatedImage; // ������㱻����״̬�µ�ͼƬ
        private Image conditionNodeShadowImage; // ����������ӰͼƬ
        private Image conditionNodeDisableNormalImage; // ���������Ч��ͨ״̬�µ�ͼƬ
        private Image conditionNodeDisableSelectedImage; // ���������Ч��ѡ��״̬�µ�ͼƬ
        private Image conditionNodeDisableActivatedImage; // ���������Ч������״̬�µ�ͼƬ
        private Image conditionNodeDisableShadowImage; // ���������Ч״̬�µ���ӰͼƬ
        private Image conditionNodeAbbreviateImage; // ������������ͼƬ
        private Image conditionNodeDisableAbbreviateImage; // ���������Ч״̬������ͼƬ

        private Image eventNodeNormalImage; // �¼������ͨ״̬�µ�ͼƬ
        private Image eventNodeSelectedImage; // �¼���㱻ѡ��״̬�µ�ͼƬ
        private Image eventNodeActivatedImage; // �¼���㱻����״̬�µ�ͼƬ
        private Image eventNodeShadowImage; // �¼�������ӰͼƬ
        private Image eventNodeDisableNormalImage; // �¼������Ч��ͨ״̬�µ�ͼƬ
        private Image eventNodeDisableSelectedImage; // �¼������Ч��ѡ��״̬�µ�ͼƬ
        private Image eventNodeDisableActivatedImage; // �¼������Ч������״̬�µ�ͼƬ
        private Image eventNodeDisableShadowImage; // �¼������Ч״̬�µ���ӰͼƬ
        private Image eventNodeAbbreviateImage; // �¼���������ͼƬ
        private Image eventNodeDisableAbbreviateImage; // �¼��������״̬��ͼƬ

        private Image aiActionNodeNormalImage; // ai���������ͨ״̬�µ�ͼƬ
        private Image aiActionNodeSelectedImage; // ai������㱻ѡ��״̬�µ�ͼƬ
        private Image aiActionNodeActivatedImage; // ai������㱻����״̬�µ�ͼƬ
        private Image aiActionNodeShadowImage; // ai���������ӰͼƬ
        private Image aiActionNodeAbbreviateImage; // ai�����������ͼƬ

        private Image aiActionsNodeNormalImage; // ai����������ͨ״̬�µ�ͼƬ
        private Image aiActionsNodeSelectedImage; // ai�������㱻ѡ��״̬�µ�ͼƬ
        private Image aiActionsNodeActivatedImage; // ai�������㱻����״̬�µ�ͼƬ
        private Image aiActionsNodeShadowImage; // ai����������ӰͼƬ
        private Image aiActionsNodeAbbreviateImage; // ai������������ͼƬ

        private Image aiStateNodeNormalImage; // ai״̬�����ͨ״̬�µ�ͼƬ
        private Image aiStateNodeSelectedImage; // ai״̬��㱻ѡ��״̬�µ�ͼƬ
        private Image aiStateNodeActivatedImage; // ai״̬��㱻����״̬�µ�ͼƬ
        private Image aiStateNodeShadowImage; // ai״̬�����ӰͼƬ
        private Image aiStateNodeAbbreviateImage; // ai״̬�������ͼƬ

        private Image connectButtonNormalImage; // ���Ӱ�ť��ͨ״̬�µ�ͼƬ
        private Image connectButtonActivatedImage; // ���Ӱ�ť������״̬�µ�ͼƬ        

        private Image addButtonNormalImage; // ��Ӱ�ť��ͨ״̬�µ�ͼƬ
        private Image addButtonActivatedImage; // ��Ӱ�ť������״̬�µ�ͼƬ   

        private Image scrollUpArraw; // ���Ϲ����ļ�ͷ
        private Image scrollLeftArraw; // ��������ļ�ͷ
        private Image scrollDownArraw; // ���¹����ļ�ͷ
        private Image scrollRightArraw; // ���ҹ����ļ�ͷ

        private Font textFont; // ͼԪ�ı�������
        private Font titleFont;  // ͼԪ��������� 
        private Font tooltipFont;  // ͼԪ��ʾ�ı�������

        private Graphics graphicsObject; // ��ͼ����
        private TextBoxX logBox; // ��־����ı���

        private Point abbreviatePanelLocation = Point.Empty; // ��ͼ������ͼ�ؼ���λ��

        private string outputDir = ""; // ����ͼ�����·��
        private bool showRemark = true; // �Ƿ���ʾͼԪע��
        private bool autoAdjustLine = true; // �Ƿ��Զ�����������
        private bool autoAdjustGrid = false; // �Ƿ��Զ����뵽���� 
        private List<int> latestDiagramList = new List<int>(); // ����򿪵Ļ�ͼ
        private int latestDiagramIndex = 0; // ����򿪵Ļ�ͼ���������

        private LogicBaseManager flowChartLogicManager; // ����ͼ�߼�������ʵ��
        private LogicBaseManager aiChartLogicManager; // AIͼ�߼�������ʵ��

        /// <summary>
        /// ���캯��
        /// </summary>
        private Helper()
        {

        }

        /// <summary>
        /// ��ȡ�������ʵ��
        /// </summary>
        public static Helper GetHelper()
        {
            if(helper == null)
            {
                helper = new Helper();
            }

            return helper;
        }

        /// <summary>
        /// ��������ͼ����
        /// </summary>
        /// <param name="args">������ϣ��</param>
        public void SetArguments(Hashtable args)
        {           
            outputDir = args["OutPutDir"] as string;
            showRemark = (bool)args["ShowRemark"];
            autoAdjustLine = (bool)args["AutoAdjustLine"];
            autoAdjustGrid = (bool)args["AutoAdjustGrid"];
            latestDiagramList = args["LatestDiagramList"] as List<int>;

            if (latestDiagramList == null)
            {
                latestDiagramList = new List<int>();
            }

            if (args["LatestDiagramIndex"] == null)
            {
                latestDiagramIndex = 0;
            }
            else
            {
                latestDiagramIndex = int.Parse(args["LatestDiagramIndex"].ToString());
            }
        }

        /// <summary>
        /// ��ȡ����ͼ����
        /// </summary>
        /// <returns>������ϣ��</returns>
        public Hashtable GetArguments()
        {
            Hashtable args = new Hashtable();
            args["OutPutDir"] = outputDir;
            args["ShowRemark"] = showRemark;
            args["AutoAdjustLine"] = autoAdjustLine;
            args["AutoAdjustGrid"] = autoAdjustGrid;
            args["LatestDiagramList"] = latestDiagramList;
            args["LatestDiagramIndex"] = latestDiagramIndex.ToString();

            return args;
        }

        /// <summary>
        /// ����򿪵Ļ�ͼ����s
        /// </summary>
        public List<int> LatestDiagramList
        {
            get
            {
                return latestDiagramList;
            }
        }

        /// <summary>
        /// ����ͼ�߼�������ʵ��
        /// </summary>
        public LogicBaseManager FlowChartLogicManager
        {
            get
            {
                if(flowChartLogicManager == null)
                {
                    try
                    {
                        Assembly assembly = Assembly.LoadFrom(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\FlowChartLogicManager.dll"));
                        flowChartLogicManager = assembly.CreateInstance("FlowChartLogicManager.LogicManager") as LogicBaseManager;
                    }
                    catch (Exception ex)
                    {
                        logBox.AppendText(string.Format("\r\n��������ͼ�߼�dllʱ�����쳣��{0}\r\n", ex.Message));
                        flowChartLogicManager = new LogicBaseManager();
                    }
                }

                return flowChartLogicManager;
            }
        }

        /// <summary>
        /// AI�߼�������ʵ��
        /// </summary>
        public LogicBaseManager AIChartLogicManager
        {
            get
            {
                if (aiChartLogicManager == null)
                {
                    try
                    {
                        Assembly assembly = Assembly.LoadFrom(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\AIChartLogicManager.dll"));
                        aiChartLogicManager = assembly.CreateInstance("AIChartLogicManager.LogicManager") as LogicBaseManager;
                    }
                    catch (Exception ex)
                    {
                        logBox.AppendText(string.Format("\r\n����AIͼ�߼�dllʱ�����쳣��{0}\r\n", ex.Message));
                        aiChartLogicManager = new LogicBaseManager();
                    }
                }

                return aiChartLogicManager;
            }
        }

        /// <summary>
        /// ��־����ı���
        /// </summary>
        public TextBoxX LogBox
        {
            set
            {
                logBox = value;
            }
        }

        /// <summary>
        /// ����ͼ������·������
        /// </summary>
        public string OutputDir
        {
            set
            {
                outputDir = value;
            }

            get
            {
                return outputDir;
            }
        }

        /// <summary>
        /// ��ͼ����
        /// </summary>
        public Graphics GraphicsObject
        {
            get
            {
                if(graphicsObject == null)
                {
                    Bitmap bmp = new Bitmap(1, 1);
                    graphicsObject = Graphics.FromImage(bmp);
                }

                return graphicsObject;
            }
        }

        /// <summary>
        /// �Ƿ��Զ����뵽��������
        /// </summary>
        public bool AutoAdjustGrid
        {
            set
            {
                autoAdjustGrid = value;
            }

            get
            {
                return autoAdjustGrid;
            }
        }

        /// <summary>
        /// �Ƿ��Զ�����������
        /// </summary>
        public bool AutoAdjustLine
        {
            set
            {
                autoAdjustLine = value;
            }

            get
            {
                return autoAdjustLine;
            }
        }

        /// <summary>
        /// �Ƿ���ʾͼԪ��ע������
        /// </summary>
        public bool ShowRemark
        {
            set
            {
                showRemark = value;                
            }

            get
            {
                return showRemark;
            }
        }

        /// <summary>
        /// ���Ϲ����ļ�ͷͼƬ����
        /// </summary>
        public Image ScrollUpArraw
        {
            get
            {
                if (scrollUpArraw == null)
                {
                    scrollUpArraw = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\scrollArraw-up.wmf"));
                }

                return scrollUpArraw;
            }
        }

        /// <summary>
        /// ���¹����ļ�ͷͼƬ����
        /// </summary>
        public Image ScrollDownArraw
        {
            get
            {
                if(scrollDownArraw == null)
                {
                    scrollDownArraw = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\scrollArraw-down.wmf"));
                }

                return scrollDownArraw;
            }
        }

        /// <summary>
        /// ��������ļ�ͷͼƬ����
        /// </summary>
        public Image ScrollLeftArraw
        {
            get
            {
                if (scrollLeftArraw == null)
                {
                    scrollLeftArraw = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\scrollArraw-left.wmf"));
                }

                return scrollLeftArraw;
            }
        }

        /// <summary>
        /// ���ҹ����ļ�ͷͼƬ����
        /// </summary>
        public Image ScrollRightArraw
        {
            get
            {
                if (scrollRightArraw == null)
                {
                    scrollRightArraw = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\scrollArraw-right.wmf"));
                }

                return scrollRightArraw;
            }
        }

        /// <summary>
        /// ��ͼ������ͼ�ؼ���λ������
        /// </summary>
        public Point AbbreviatePanelLocation
        {
            set
            {
                abbreviatePanelLocation = value;
            }

            get
            {
                return abbreviatePanelLocation;
            }
        }

        /// <summary>
        /// ͼԪ��ʾ�ı�����������
        /// </summary>
        public Font TooltipFont
        {
            get
            {
                if(tooltipFont == null)
                {
                    tooltipFont = new Font("Sim Sun", 8, FontStyle.Regular);
                }

                return tooltipFont;
            }
        }

        /// <summary>
        /// ͼԪ�������������
        /// </summary>
        public Font TitleFont
        {
            get
            {
                if(titleFont == null)
                {
                    titleFont = new Font("Sim Sun", 8, FontStyle.Bold);
                }

                return titleFont;
            }
        }

        /// <summary>
        /// ͼԪ�ı�����������
        /// </summary>
        public Font TextFont
        {
            get
            {
                if(textFont == null)
                {
                    textFont = new Font("Sim Sun", 8, FontStyle.Regular);
                }

                return textFont;
            }
        }

        /// <summary>
        /// ���������ͨ״̬��ͼƬ����
        /// </summary>
        public Image ActionNodeNormalImage
        {
            get
            {
                if(actionNodeNormalImage == null)
                {
                    actionNodeNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-normal.wmf"));
                }

                return actionNodeNormalImage;
            }
        }

        /// <summary>
        /// ������㱻ѡ��״̬��ͼƬ����
        /// </summary>
        public Image ActionNodeSelectedImage
        {
            get
            {
                if(actionNodeSelectedImage == null)
                {
                    actionNodeSelectedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-selected.wmf"));
                }

                return actionNodeSelectedImage;
            }
        }

        /// <summary>
        /// ������㱻����״̬��ͼƬ����
        /// </summary>
        public Image ActionNodeActivatedImage
        {
            get
            {
                if(actionNodeActivatedImage == null)
                {
                    actionNodeActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-activated.wmf"));
                }

                return actionNodeActivatedImage;
            }
        }

        /// <summary>
        /// ���������Ӱ��ͼƬ����
        /// </summary>
        public Image ActionNodeShadowImage
        {
            get
            {
                if(actionNodeShadowImage == null)
                {
                    actionNodeShadowImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-shadow.wmf"));
                }

                return actionNodeShadowImage;
            }
        }

        /// <summary>
        /// ���������Ч��ͨ״̬��ͼƬ����
        /// </summary>
        public Image ActionNodeDisableNormalImage
        {
            get
            {
                if(actionNodeDisableNormalImage == null)
                {
                    actionNodeDisableNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-disable-normal.wmf"));
                }

                return actionNodeDisableNormalImage;
            }
        }

        /// <summary>
        /// ���������Ч��ѡ��״̬��ͼƬ����
        /// </summary>
        public Image ActionNodeDisableSelectedImage
        {
            get
            {
                if (actionNodeDisableSelectedImage == null)
                {
                    actionNodeDisableSelectedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-disable-selected.wmf"));
                }

                return actionNodeDisableSelectedImage;
            }
        }

        /// <summary>
        /// ���������Ч������״̬��ͼƬ����
        /// </summary>
        public Image ActionNodeDisableActivatedImage
        {
            get
            {
                if (actionNodeDisableActivatedImage == null)
                {
                    actionNodeDisableActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-disable-activated.wmf"));
                }

                return actionNodeDisableActivatedImage;
            }
        }

        /// <summary>
        /// ���������Ч״̬����ӰͼƬ����
        /// </summary>
        public Image ActionNodeDisableShadowImage
        {
            get
            {
                if (actionNodeDisableShadowImage == null)
                {
                    actionNodeDisableShadowImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-disable-shadow.wmf"));
                }

                return actionNodeDisableShadowImage;
            }
        }

        /// <summary>
        /// ������������ͼƬ����
        /// </summary>
        public Image ActionNodeAbbreviateImage
        {
            get
            {
                if (actionNodeAbbreviateImage == null)
                {
                    actionNodeAbbreviateImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-normal.wmf"));
                }

                return actionNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ���������Ч״̬������ͼƬ����
        /// </summary>
        public Image ActionNodeDisableAbbreviateImage
        {
            get
            {
                if (actionNodeDisableAbbreviateImage == null)
                {
                    actionNodeDisableAbbreviateImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-disable-normal.wmf"));
                }

                return actionNodeDisableAbbreviateImage;
            }
        }

        /// <summary>
        /// ���������ͨ״̬��ͼƬ����
        /// </summary>
        public Image ConditionNodeNormalImage
        {
            get
            {
                if(conditionNodeNormalImage == null)
                {
                    conditionNodeNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\conditionNode-normal.wmf"));
                }

                return conditionNodeNormalImage;
            }
        }

        /// <summary>
        /// ������㱻ѡ��״̬��ͼƬ����
        /// </summary>
        public Image ConditionNodeSelectedImage
        {
            get
            {
                if(conditionNodeSelectedImage == null)
                {
                    conditionNodeSelectedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\conditionNode-selected.wmf"));
                }

                return conditionNodeSelectedImage;
            }
        }

        /// <summary>
        /// ������㱻����״̬��ͼƬ����
        /// </summary>
        public Image ConditionNodeActivatedImage
        {
            get
            {
                if(conditionNodeActivatedImage == null)
                {
                    conditionNodeActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\conditionNode-activated.wmf"));
                }

                return conditionNodeActivatedImage;
            }
        }

        /// <summary>
        /// ���������Ӱ��ͼƬ����
        /// </summary>
        public Image ConditionNodeShadowImage
        {
            get
            {
                if(conditionNodeShadowImage == null)
                {
                    conditionNodeShadowImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\conditionNode-shadow.wmf"));
                }

                return conditionNodeShadowImage;
            }
        }

        /// <summary>
        /// ���������Ч��ͨ״̬��ͼƬ����
        /// </summary>
        public Image ConditionNodeDisableNormalImage
        {
            get
            {
                if (conditionNodeDisableNormalImage == null)
                {
                    conditionNodeDisableNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\conditionNode-disable-normal.wmf"));
                }

                return conditionNodeDisableNormalImage;
            }
        }

        /// <summary>
        /// ���������Ч��ѡ��״̬��ͼƬ����
        /// </summary>
        public Image ConditionNodeDisableSelectedImage
        {
            get
            {
                if (conditionNodeDisableSelectedImage == null)
                {
                    conditionNodeDisableSelectedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\conditionNode-disable-selected.wmf"));
                }

                return conditionNodeDisableSelectedImage;
            }
        }

        /// <summary>
        /// ���������Ч������״̬��ͼƬ����
        /// </summary>
        public Image ConditionNodeDisableActivatedImage
        {
            get
            {
                if (conditionNodeDisableActivatedImage == null)
                {
                    conditionNodeDisableActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\conditionNode-disable-activated.wmf"));
                }

                return conditionNodeDisableActivatedImage;
            }
        }

        /// <summary>
        /// ���������Ч״̬����ӰͼƬ����
        /// </summary>
        public Image ConditionNodeDisableShadowImage
        {
            get
            {
                if (conditionNodeDisableShadowImage == null)
                {
                    conditionNodeDisableShadowImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\conditionNode-disable-shadow.wmf"));
                }

                return conditionNodeDisableShadowImage;
            }
        }

        /// <summary>
        /// ������������ͼƬ����
        /// </summary>
        public Image ConditionNodeAbbreviateImage
        {
            get
            {
                if (conditionNodeAbbreviateImage == null)
                {
                    conditionNodeAbbreviateImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\conditionNode-normal.wmf"));
                }

                return conditionNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ���������Ч״̬������ͼƬ����
        /// </summary>
        public Image ConditionNodeDisableAbbreviateImage
        {
            get
            {
                if (conditionNodeDisableAbbreviateImage == null)
                {
                    conditionNodeDisableAbbreviateImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\conditionNode-disable-normal.wmf"));
                }

                return conditionNodeDisableAbbreviateImage;
            }
        }

        /// <summary>
        /// �¼������ͨ״̬��ͼƬ����
        /// </summary>
        public Image EventNodeNormalImage
        {
            get
            {
                if(eventNodeNormalImage == null)
                {
                    eventNodeNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\eventNode-normal.wmf"));
                }

                return eventNodeNormalImage;
            }
        }

        /// <summary>
        /// �¼���㱻ѡ��״̬��ͼƬ����
        /// </summary>
        public Image EventNodeSelectedImage
        {
            get
            {
                if(eventNodeSelectedImage == null)
                {
                    eventNodeSelectedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\eventNode-selected.wmf"));
                }

                return eventNodeSelectedImage;
            }
        }

        /// <summary>
        /// �¼���㱻����״̬��ͼƬ����
        /// </summary>
        public Image EventNodeActivatedImage
        {
            get
            {
                if(eventNodeActivatedImage == null)
                {
                    eventNodeActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\eventNode-activated.wmf"));
                }

                return eventNodeActivatedImage;
            }
        }

        /// <summary>
        /// �¼������Ӱ��ͼƬ����
        /// </summary>
        public Image EventNodeShadowImage
        {
            get
            {
                if(eventNodeShadowImage == null)
                {
                    eventNodeShadowImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\eventNode-shadow.wmf"));
                }

                return eventNodeShadowImage;
            }
        }

        /// <summary>
        /// �¼������Ч��ͨ״̬��ͼƬ����
        /// </summary>
        public Image EventNodeDisableNormalImage
        {
            get
            {
                if (eventNodeDisableNormalImage == null)
                {
                    eventNodeDisableNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\eventNode-disable-normal.wmf"));
                }

                return eventNodeDisableNormalImage;
            }
        }

        /// <summary>
        /// �¼������Ч��ѡ��״̬��ͼƬ����
        /// </summary>
        public Image EventNodeDisableSelectedImage
        {
            get
            {
                if (eventNodeDisableSelectedImage == null)
                {
                    eventNodeDisableSelectedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\eventNode-disable-selected.wmf"));
                }

                return eventNodeDisableSelectedImage;
            }
        }

        /// <summary>
        /// �¼������Ч������״̬��ͼƬ����
        /// </summary>
        public Image EventNodeDisableActivatedImage
        {
            get
            {
                if (eventNodeDisableActivatedImage == null)
                {
                    eventNodeDisableActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\eventNode-disable-activated.wmf"));
                }

                return eventNodeDisableActivatedImage;
            }
        }

        /// <summary>
        /// �¼������Ч״̬����ӰͼƬ����
        /// </summary>
        public Image EventNodeDisableShadowImage
        {
            get
            {
                if (eventNodeDisableShadowImage == null)
                {
                    eventNodeDisableShadowImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\eventNode-disable-shadow.wmf"));
                }

                return eventNodeDisableShadowImage;
            }
        }

        /// <summary>
        /// �¼���������ͼƬ����
        /// </summary>
        public Image EventNodeAbbreviateImage
        {
            get
            {
                if (eventNodeAbbreviateImage == null)
                {
                    eventNodeAbbreviateImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\eventNode-normal.wmf"));
                }

                return eventNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// �¼������Ч״̬������ͼƬ����
        /// </summary>
        public Image EventNodeDisableAbbreviateImage
        {
            get
            {
                if (eventNodeDisableAbbreviateImage == null)
                {
                    eventNodeDisableAbbreviateImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\eventNode-disable-normal.wmf"));
                }

                return eventNodeDisableAbbreviateImage;
            }
        }

        /// <summary>
        /// ��Ӱ�ť��ͨ״̬��ͼƬ����
        /// </summary>
        public Image AddButtonNormalImage
        {
            get
            {
                if(addButtonNormalImage == null)
                {
                    addButtonNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\addButton-normal.wmf"));
                }

                return addButtonNormalImage;
            }
        }

        /// <summary>
        /// ��Ӱ�ť������״̬��ͼƬ����
        /// </summary>
        public Image AddButtonActivatedImage
        {
            get
            {
                if(addButtonActivatedImage == null)
                {
                    addButtonActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\addButton-activated.wmf"));
                }

                return addButtonActivatedImage;
            }
        }

        /// <summary>
        /// ���Ӱ�ť��ͨ״̬��ͼƬ����
        /// </summary>
        public Image ConnectButtonNormalImage
        {
            get
            {
                if(connectButtonNormalImage == null)
                {
                    connectButtonNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\connectButton-normal.wmf"));
                }

                return connectButtonNormalImage;
            }
        }

        /// <summary>
        /// ���Ӱ�ť������״̬��ͼƬ����
        /// </summary>
        public Image ConnectButtonActivatedImage
        {
            get
            {
                if(connectButtonActivatedImage == null)
                {
                    connectButtonActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\connectButton-activated.wmf"));
                }

                return connectButtonActivatedImage;
            }
        }

        /// <summary>
        /// ai���������ͨ״̬��ͼƬ
        /// </summary>
        public Image AIActionNodeNormalImage
        {
            get
            {
                if(aiActionNodeNormalImage == null)
                {
                    aiActionNodeNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiActionNode-normal.wmf"));
                }

                return aiActionNodeNormalImage;
            }
        }

        /// <summary>
        /// ai������㱻ѡ��״̬��ͼƬ
        /// </summary>
        public Image AIActionNodeSelectedImage
        {
            get
            {
                if(aiActionNodeSelectedImage == null)
                {
                    aiActionNodeSelectedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiActionNode-selected.wmf"));
                }

                return aiActionNodeSelectedImage;
            }
        }

        /// <summary>
        /// ai������㱻����״̬��ͼƬ
        /// </summary>
        public Image AIActionNodeActivatedImage
        {
            get
            {
                if(aiActionNodeActivatedImage == null)
                {
                    aiActionNodeActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiActionNode-activated.wmf"));
                }

                return aiActionNodeActivatedImage;
            }
        }

        /// <summary>
        /// ai���������Ӱ״̬��ͼƬ
        /// </summary>
        public Image AIActionNodeShadowImage
        {
            get
            {
                if(aiActionNodeShadowImage == null)
                {
                    aiActionNodeShadowImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiActionNode-shadow.wmf"));
                }

                return aiActionNodeShadowImage;
            }
        }

        /// <summary>
        /// ai����������Ե�ͼƬ
        /// </summary>
        public Image AIActionNodeAbbreviateImage
        {
            get
            {
                if (aiActionNodeAbbreviateImage == null)
                {
                    aiActionNodeAbbreviateImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiActionNode-normal.wmf"));
                }

                return aiActionNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ai����������ͨ״̬�µ�ͼƬ
        /// </summary>
        public Image AIActionsNodeNormalImage
        {
            get
            {
                if(aiActionsNodeNormalImage == null)
                {
                    aiActionsNodeNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiActionsNode-normal.wmf"));
                }

                return aiActionsNodeNormalImage;
            }
        }

        /// <summary>
        /// ai�������㱻ѡ��״̬�µ�ͼƬ
        /// </summary>
        public Image AIActionsNodeSelectedImage
        {
            get
            {
                if(aiActionsNodeSelectedImage == null)
                {
                    aiActionsNodeSelectedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiActionsNode-selected.wmf"));
                }

                return aiActionsNodeSelectedImage;
            }
        }

        /// <summary>
        /// ai�������㱻����״̬�µ�ͼƬ
        /// </summary>
        public Image AIActionsNodeActivatedImage
        {
            get
            {
                if(aiActionsNodeActivatedImage == null)
                {
                    aiActionsNodeActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiActionsNode-activated.wmf"));
                }

                return aiActionsNodeActivatedImage;
            }
        }

        /// <summary>
        /// ai״̬�����ӰͼƬ
        /// </summary>
        public Image AIActionsNodeShadowImage
        {
            get
            {
                if(aiActionsNodeShadowImage == null)
                {
                    aiActionsNodeShadowImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\actionNode-shadow.wmf"));
                }

                return aiActionsNodeShadowImage;
            }
        }

        /// <summary>
        /// ai״̬�������ͼƬ
        /// </summary>
        public Image AIActionsNodeAbbreviateImage
        {
            get
            {
                if (aiActionsNodeAbbreviateImage == null)
                {
                    aiActionsNodeAbbreviateImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiActionsNode-normal.wmf"));
                }

                return aiActionsNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ai״̬�����ͨ״̬��ͼƬ
        /// </summary>
        public Image AIStateNodeNormalImage
        {
            get
            {
                if(aiStateNodeNormalImage == null)
                {
                    aiStateNodeNormalImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiStateNode-normal.wmf"));
                }

                return aiStateNodeNormalImage;
            }
        }

        /// <summary>
        /// ai״̬��㱻ѡ��״̬��ͼƬ
        /// </summary>
        public Image AIStateNodeSelectedImage
        {
            get
            {
                if(aiStateNodeSelectedImage == null)
                {
                    aiStateNodeSelectedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiStateNode-selected.wmf"));
                }

                return aiStateNodeSelectedImage;
            }
        }

        /// <summary>
        /// ai״̬��㱻����״̬��ͼƬ
        /// </summary>
        public Image AIStateNodeActivatedImage
        {
            get
            {
                if(aiStateNodeActivatedImage == null)
                {
                    aiStateNodeActivatedImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiStateNode-activated.wmf"));
                }

                return aiStateNodeActivatedImage;
            }
        }

        /// <summary>
        /// ai״̬�����ӰͼƬ
        /// </summary>
        public Image AIStateNodeShadowImage
        {
            get
            {
                if(aiStateNodeShadowImage == null)
                {
                    aiStateNodeShadowImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiStateNode-shadow.wmf"));
                }

                return aiStateNodeShadowImage;
            }
        }

        /// <summary>
        /// ai״̬�������ͼƬ
        /// </summary>
        public Image AIStateNodeAbbreviateImage
        {
            get
            {
                if (aiStateNodeAbbreviateImage == null)
                {
                    aiStateNodeAbbreviateImage = LoadImageFromFile(Path.Combine(Application.StartupPath, "Plugins\\FlowChart\\Images\\aiStateNode-normal.wmf"));
                }

                return aiStateNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ��ȡͼƬ�ļ�
        /// </summary>
        /// <param name="filePath">�ļ�·��</param>
        /// <returns>ͼƬ</returns>
        private Image LoadImageFromFile(string filePath)
        {
            Image image = null;

            try
            {
                image = Image.FromFile(filePath);
            }
            catch (IOException ex)
            {
                MessageBox.Show(string.Format("��ȡ�ļ�{0}ʱ�����쳣:" + ex.ToString(), filePath), "ͼƬ�ļ���ȡ",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            return image;
        }

        /// <summary>
        /// ��ȡ��ǰ�û���
        /// </summary>
        /// <returns>��ǰ�û���</returns>
        public string GetHostName()
        {
            string hostName = Dns.GetHostName();

            return hostName;
        }

        /// <summary>
        /// ��ȡ��ǰ�û���ַ
        /// </summary>
        /// <returns>��ǰ�û���ַ</returns>
        public string GetHostAddress()
        {            
            string ip = Dns.GetHostEntry(Dns.GetHostName()).AddressList[0].ToString();;

            return ip;
        }

        /// <summary>
        /// ���ı����浽�ļ�
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="content">�ı�����</param>
        public bool SaveTextToFile(string fileName, string content)
        {
            bool success = true;

            try
            {
                FileInfo fi = new FileInfo(fileName);
                StreamWriter sw;

                if (!fi.Exists) // �ļ�������
                {
                    string path = Path.GetDirectoryName(fileName); // ��ȡĿ¼
                    if (!Directory.Exists(path)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(path); // �ȴ���Ŀ¼���ٴ����ļ�
                    }
                    FileStream fs = fi.Create();
                    fs.Close();
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }
                else
                {
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));                        
                }

                sw.Write(content);
                sw.Close();
            }
            catch (IOException ex)
            {
                MessageBox.Show(string.Format("���ļ�{0}����IO����ʱ�����쳣: " + ex.ToString(), fileName));
                success = false;
            }

            return success;
        }

        /// <summary>
        /// ��������ı�
        /// </summary>
        /// <param name="text">�����ı�</param>
        public void PrintText(string text)
        {
            logBox.AppendText(string.Format("������Ϣ�� {0}\r\n", text));
        }

        /// <summary>
        /// ��¼����򿪵Ļ�ͼID
        /// </summary>
        /// <param name="id">����򿪵Ļ�ͼID</param>
        public void RecordLatestDiagram(int id)
        {
            if (latestDiagramList.Count < 5)
            {
                if (!latestDiagramList.Contains(id))
                {
                    latestDiagramList.Add(id);
                }
            }
            else
            {
                if (!latestDiagramList.Contains(id))
                {
                    latestDiagramList[latestDiagramIndex] = id;
                    latestDiagramIndex++;

                    if (latestDiagramIndex > 4)
                    {
                        latestDiagramIndex = 0;
                    }
                }                                
            }
        }

        /// <summary>
        /// ��ȡ�߼�������ʵ��
        /// </summary>
        /// <param name="chartMode">��ǰ�༭ģʽ</param>
        /// <returns>�߼�������ʵ��</returns>
        public LogicBaseManager GetLogicManager(ChartMode chartMode)
        {
            LogicBaseManager logicManager = null;

            switch(chartMode)
            {
                case ChartMode.FlowChart: // �༭����ͼģʽ
                    {
                        logicManager = FlowChartLogicManager;
                        break;
                    }

                case ChartMode.AIChart: // �༭AIͼģʽ
                    {
                        logicManager = AIChartLogicManager;
                        break;
                    }
            }

            return logicManager;
        }
    }
}
