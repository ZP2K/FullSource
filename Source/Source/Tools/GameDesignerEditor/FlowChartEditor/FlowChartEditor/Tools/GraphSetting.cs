using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace FlowChartEditor
{
    public class GraphSetting
    {
        private static GraphSetting graphSetting; // ��ͼ���ö���        

        private Color backgroundColor = Color.White; // ������ɫ
        private Brush backgroundBrush; // ������ˢ

        private Font graphElementTextFont; // ͼԪ���ı�����
        private Font graphElementTextBoldFont; // ͼԪ���ı���������
        private Font graphElementTextItalicFont; // ͼԪ���ı�б������
        private Font graphElementTextUnderLineFont; // ͼԪ���ı��»�������
        private Color graphElementTextColor = Color.Yellow; // ͼԪ���ı���ɫ
        private Brush graphElementTextBrush; // ͼԪ���ı���ˢ
        
        private Color graphElementTextSelectedColor = Color.Red; // ͼԪ���ı���ѡ��״̬����ɫ
        private Brush graphElementTextSelectedBrush; // ͼԪ���ı���ѡ��״̬�ı�ˢ

        private int lineWidth = 2; // �����ߵĿ��
        private Color lineTextColor = Color.Black; // �����ߵ��ı���ɫ
        private Color lineMarkedColor = Color.Goldenrod; // �����߱����״̬����ɫ

        private Brush lineTextBrush; // �����ߵ��ı���ˢ
        private Pen lineNormalPen; // �����ߵĻ���
        private Pen lineActivatedPen; // �����߱�����״̬�Ļ���
        private Pen lineSelectedPen; // �����߱�ѡ��״̬�Ļ���        
        private Pen lineMarkdPen; // �����߱������״̬�Ļ���
 
        private Pen lineNormalArrawPen; // ����ͷ�����ߵĻ���
        private Pen lineActivatedArrawPen; // ����ͷ�����߱�����״̬�Ļ���
        private Pen lineSelectedArrawPen; // ����ͷ�����߱�ѡ��״̬�Ļ���
        private Pen lineMarkedArrawPen; // ����ͷ�����߱����״̬�Ļ���

        private int graphElementBorderWidth = 1; // ͼԪ�߿���

        private Color graphElementNormalColor = Color.Black; // ͼԪ��ͨ״̬��ɫ
        private Color graphElementActivatedColor = Color.DarkOrange; // ͼԪ������״̬��ɫ
        private Color graphElementSelectedColor = Color.Red; // ͼԪ��ѡ��״̬��ɫ
        private Color graphElementBindedColor = Color.Green; // ͼԪ����״̬��ɫ
        private Color graphElementBorderColor = Color.RoyalBlue; // ͼԪ�ı߿���ɫ

        private Brush graphElementNormalBrush; // ͼԪ��ͨ״̬�Ļ�ˢ
        private Brush graphElementActivatedBrush; // ͼԪ������״̬�Ļ�ˢ
        private Brush graphElementSelectedBrush; // ͼԪ��ѡ��״̬�Ļ�ˢ
        private Brush graphElementBindedBrush; // ͼԪ����״̬�Ļ�ˢ

        private Pen graphElementNormalPen; // ͼԪ��ͨ״̬�Ļ���
        private Pen graphElementActivatePen; // ͼԪ������״̬�Ļ���
        private Pen graphElementSelectedPen; // ͼԪ��ѡ��״̬�Ļ���        
        private Pen graphElementBorderPen; // ͼԪ�ı߿򻭱�

        private Color remarkBackgroundColor = Color.LightSteelBlue; // ע�͵ı���ɫ
        private Brush remarkBackgroundBrush; // ע�͵ı�����ˢ

        private Color rodmanBackgroundColor = Color.Gray; // �α�ı�����ɫ
        private Color rodmanActivatedColor = Color.DarkOrange; // �α걻����״̬����ɫ
        private Color rodmanSelectedColor = Color.Red; // �α걻ѡ��״̬����ɫ
        private int rodmanWidth = 25; // �α�Ŀ��

        private Brush rodmanBackgroundBrush; // �α�ı�����ˢ
        private Brush rodmanActivatedBrush; // �α걻����״̬�Ļ�ˢ
        private Brush rodmanSelectedBrush; // �α걻ѡ��״̬�Ļ�ˢ        

        private Color tooltipBackgroundColor = Color.LightYellow; // ��ʾ�򱳾���ɫ
        private Brush tooltipBackgroundBrush; // ��ʾ�򱳾���ˢ

        private int multiSelectMarkWith = 2; // ��ѡ��ǵı߿���
        private Color multiSelectMarkBackgroundColor = Color.FromArgb(127, Color.LightSteelBlue); // ��ѡ��ǵı�����ɫ
        private Color multiSelectMarkColor = Color.RoyalBlue; // ��ѡ��ǵ���ɫ

        private Brush multiSelectMarkBackgroundBrush; // ��ѡ��ǵı�����ˢ
        private Pen multiSelectMarkPen; // ��ѡ��ǵĻ���

        private int adjustLineWidth = 2; // �����ߵĿ��
        private Color adjustLineColor = Color.Purple; // �����ߵ���ɫ
        private Pen adjustLinePen; // �����ߵĻ���

        private Color gridLineColor = Color.Silver; // �߿��ߵ���ɫ
        private Pen gridLinePen; // �߿��ߵĻ���

        private Image actionNodeNormalImage; // ���������ͨ״̬�µ�ͼƬ
        private Image actionNodeSelectedImage; // ������㱻ѡ��״̬�µ�ͼƬ
        private Image actionNodeActivatedImage; // ������㱻����״̬�µ�ͼƬ
        private Image actionNodeShadowImage; // ����������ӰͼƬ
        private Image actionNodeAbbreviateImage; // ������������ͼƬ

        private Image conditionNodeNormalImage; // ���������ͨ״̬�µ�ͼƬ
        private Image conditionNodeSelectedImage; // ������㱻ѡ��״̬�µ�ͼƬ
        private Image conditionNodeActivatedImage; // ������㱻����״̬�µ�ͼƬ
        private Image conditionNodeShadowImage; // ����������ӰͼƬ
        private Image conditionNodeAbbreviateImage; // ������������ͼƬ

        private Image eventNodeNormalImage; // �¼������ͨ״̬�µ�ͼƬ
        private Image eventNodeSelectedImage; // �¼���㱻ѡ��״̬�µ�ͼƬ
        private Image eventNodeActivatedImage; // �¼���㱻����״̬�µ�ͼƬ
        private Image eventNodeShadowImage; // �¼�������ӰͼƬ
        private Image eventNodeAbbreviateImage; // �¼���������ͼƬ

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

        private Image interfaceNodeNormalImage; // �ӿڽ����ͨ״̬�µ�ͼƬ
        private Image interfaceNodeSelectedImage; // �ӿڽ�㱻ѡ��״̬�µ�ͼƬ
        private Image interfaceNodeActivatedImage; // �ӿڽ�㱻����״̬�µ�ͼƬ
        private Image interfaceNodeShadowImage; // �ӿڽ����ӰͼƬ
        private Image interfaceNodeAbbreviateImage; // �ӿڽ������ͼƬ

        private Image connectButtonNormalImage; // ���Ӱ�ť��ͨ״̬�µ�ͼƬ
        private Image connectButtonActivatedImage; // ���Ӱ�ť������״̬�µ�ͼƬ        

        private Image addButtonNormalImage; // ��Ӱ�ť��ͨ״̬�µ�ͼƬ
        private Image addButtonActivatedImage; // ��Ӱ�ť������״̬�µ�ͼƬ   

        private Dictionary<string, Image> imageDictionary = new Dictionary<string, Image>(); // ͼƬ�ֵ�

        private float canvasScale = 1.0f; // ��ͼ������Ų���

        private Size eventNodeElementSize = new Size(90, 90); // �¼�����ͼԪ��С
        private Size eventNodeMoveOffset = new Size(45, 45); // �¼������ƶ�ƫ��
        private Size conditionNodeElementSize = new Size(150, 90); // ��������ͼԪ��С
        private Size conditionNodeMoveOffset = new Size(75, 45); // ���������ƶ�ƫ��
        private Size actionNodeElementSize = new Size(150, 90); // ��������ͼԪ��С
        private Size actionNodeMoveOffset = new Size(75, 45); // ���������ƶ�ƫ��
        private Size aiStateNodeElementSize = new Size(100, 25); // AI״̬����ͼԪ��С
        private Size aiStateNodeMovOffset = new Size(50, 12); // AI״̬�����ƶ�ƫ��
        private Size aiActionNodeElementSize = new Size(100, 25); // AI��������ͼԪ��С
        private Size aiActionNodeMoveOffset = new Size(50, 12); // AI���������ƶ�ƫ��
        private Size aiActionsNodeElementSize = new Size(100, 25); // AI���������ͼԪ��С
        private Size aiActionsNodeMoveOffset = new Size(50, 12); // AI����������ƶ�ƫ��
        private Size innerChartElementSize = new Size(100, 25); // �ӻ�ͼ����ͼԪ��С
        private Size innerChartMoveOffset = new Size(50, 12); // �ӻ�ͼ�����ƶ�ƫ��
        private Size interfaceNodeElementSize = new Size(100, 25); // �ӿڽ���ͼԪ��С
        private Size interfaceNodeMoveOffset = new Size(50, 12); // �ӿڽ����ƶ�ƫ��

        private Helper helper = Helper.GetHelper(); // ������

        /// <summary>
        /// ˽�й��캯��
        /// </summary>
        private GraphSetting()
        {

        }

        /// <summary>
        /// ��ȡ��ͼ���ö���
        /// </summary>
        /// <returns>��ͼ���ö���</returns>
        public static GraphSetting GetGraphSetting()
        {
            if (graphSetting == null)
            {
                graphSetting = new GraphSetting();
            }

            return graphSetting;
        }        

        /// <summary>
        /// �¼�����ͼԪ��С
        /// </summary>
        public Size EventNodeElementSize
        {
            set
            {
                eventNodeElementSize = value;
            }

            get
            {
                return eventNodeElementSize;
            }
        }

        /// <summary>
        /// �¼������ƶ�ƫ��
        /// </summary>
        public Size EventNodeMoveOffset
        {
            set
            {
                eventNodeMoveOffset = value;
            }

            get
            {
                return eventNodeMoveOffset;
            }
        }

        /// <summary>
        /// ��������ͼԪ��С
        /// </summary>
        public Size ConditionNodeElementSize
        {
            set
            {
                conditionNodeElementSize = value;
            }

            get
            {
                return conditionNodeElementSize;
            }
        }

        /// <summary>
        /// ���������ƶ�ƫ��
        /// </summary>
        public Size ConditionNodeMoveOffset
        {
            set
            {
                conditionNodeMoveOffset = value;
            }

            get
            {
                return conditionNodeMoveOffset;
            }
        }

        /// <summary>
        /// �������ͼԪ��С
        /// </summary>
        public Size ActionNodeElementSize
        {
            set
            {
                actionNodeElementSize = value;
            }

            get
            {
                return actionNodeElementSize;
            }
        }

        /// <summary>
        /// ��������ƶ�ƫ��
        /// </summary>
        public Size ActionNodeMoveOffset
        {
            set
            {
                actionNodeMoveOffset = value;
            }

            get
            {
                return actionNodeMoveOffset;
            }
        }

        /// <summary>
        /// AI״̬����ͼԪ��С
        /// </summary>
        public Size AIStateNodeElementSize
        {
            set
            {
                aiStateNodeElementSize = value;
            }

            get
            {
                return aiStateNodeElementSize;
            }
        }

        /// <summary>
        /// AI״̬�����ƶ�ƫ��
        /// </summary>
        public Size AIStateNodeMoveOffset
        {
            set
            {
                aiStateNodeMovOffset = value;
            }

            get
            {
                return aiStateNodeMovOffset;
            }
        }

        /// <summary>
        /// AI��������ͼԪ��С
        /// </summary>
        public Size AIActionNodeElementSize
        {
            set
            {
                aiActionNodeElementSize = value;
            }

            get
            {
                return aiActionNodeElementSize;
            }
        }

        /// <summary>
        /// AI���������ƶ�ƫ��
        /// </summary>
        public Size AIActionNodeMoveOffset
        {
            set
            {
                aiActionNodeMoveOffset = value;
            }

            get
            {
                return aiActionNodeMoveOffset;
            }
        }

        /// <summary>
        /// AI���������ͼԪ��С
        /// </summary>
        public Size AIActionsNodeElementSize
        {
            set
            {
                aiActionsNodeElementSize = value;
            }

            get
            {
                return aiActionsNodeElementSize;
            }
        }

        /// <summary>
        /// AI����������ƶ�ƫ��
        /// </summary>
        public Size AIActionsNodeMoveOffset
        {
            set
            {
                aiActionsNodeMoveOffset = value;
            }

            get
            {
                return aiActionsNodeMoveOffset;
            }
        }

        /// <summary>
        /// �ӻ�ͼ����ͼԪ��С
        /// </summary>
        public Size InnerChartElementSize
        {
            set
            {
                innerChartElementSize = value;
            }

            get
            {
                return innerChartElementSize;
            }
        }

        /// <summary>
        /// �ӻ�ͼ�����ƶ�ƫ��
        /// </summary>
        public Size InnerChartMoveOffset
        {
            set
            {
                innerChartMoveOffset = value;
            }

            get
            {
                return innerChartMoveOffset;
            }
        }

        /// <summary>
        /// �ӿڽ���ͼԪ��С
        /// </summary>
        public Size InterfaceNodeElementSize
        {
            set
            {
                interfaceNodeElementSize = value;
            }

            get
            {
                return interfaceNodeElementSize;
            }
        }

        /// <summary>
        /// �ӿڽ����ƶ�ƫ��
        /// </summary>
        public Size InterfaceNodeMoveOffset
        {
            set
            {
                interfaceNodeMoveOffset = value;
            }

            get
            {
                return interfaceNodeMoveOffset;
            }
        }

        /// <summary>
        /// ͼԪ���ı���ѡ��״̬����ɫ
        /// </summary>
        public Color GraphElementTextSelectedColor
        {
            set
            {
                graphElementTextSelectedColor = value;
            }

            get
            {
                return graphElementTextSelectedColor;
            }
        }

        /// <summary>
        /// ͼԪ���ı���ѡ��״̬�ı�ˢ
        /// </summary>
        public Brush GraphElementTextSelectedBrush
        {
            get
            {
                if (graphElementTextSelectedBrush == null)
                {
                    graphElementTextSelectedBrush = new SolidBrush(graphElementTextSelectedColor);
                }

                return graphElementTextSelectedBrush;
            }
        }

        /// <summary>
        /// ������ɫ
        /// </summary>
        public Color BackgroundColor
        {
            set
            {
                backgroundColor = value;
            }

            get
            {
                return backgroundColor;
            }
        }

        /// <summary>
        /// �߿��ߵĻ���
        /// </summary>
        public Pen GridLinePen
        {
            get
            {
                if (gridLinePen == null)
                {
                    gridLinePen = new Pen(gridLineColor);
                }

                return gridLinePen;
            }
        }

        /// <summary>
        /// ��ʾ�򱳾���ˢ
        /// </summary>
        public Brush TooltipBackgroundBrush
        {
            get
            {
                if (tooltipBackgroundBrush == null)
                {
                    tooltipBackgroundBrush = new SolidBrush(tooltipBackgroundColor);
                }

                return tooltipBackgroundBrush;
            }
        }

        /// <summary>
        /// ��ͼ������Ų���
        /// </summary>
        public float CanvasScale
        {
            set
            {
                canvasScale = value;
            }

            get
            {
                return canvasScale;
            }
        }

        /// <summary>
        /// ��ѡ��ǵı�����ˢ
        /// </summary>
        public Brush MultiSelectBackgroudBrush
        {
            get
            {
                if (multiSelectMarkBackgroundBrush == null)
                {
                    multiSelectMarkBackgroundBrush = new SolidBrush(multiSelectMarkBackgroundColor);
                }

                return multiSelectMarkBackgroundBrush;
            }
        }

        /// <summary>
        /// ��ѡ��ǵĻ���
        /// </summary>
        public Pen MultiSelectMarkPen
        {
            get
            {
                if (multiSelectMarkPen == null)
                {
                    multiSelectMarkPen = new Pen(multiSelectMarkColor, multiSelectMarkWith);
                }

                return multiSelectMarkPen;
            }
        }

        /// <summary>
        /// �����ߵĻ���
        /// </summary>
        public Pen AdjustLinePen
        {
            get
            {
                if (adjustLinePen == null)
                {
                    adjustLinePen = new Pen(adjustLineColor, adjustLineWidth);
                    adjustLinePen.DashStyle = DashStyle.Dash;
                }

                return adjustLinePen;
            }
        }

        /// <summary>
        /// �α걳����ˢ
        /// </summary>
        public Brush RodmanBackgroundBrush
        {
            get
            {
                if (rodmanBackgroundBrush == null)
                {
                    rodmanBackgroundBrush = new SolidBrush(rodmanBackgroundColor);
                }

                return rodmanBackgroundBrush;
            }
        }

        /// <summary>
        /// �α걻����״̬�Ļ�ˢ
        /// </summary>
        public Brush RodmanActivatedBrush
        {
            get
            {
                if (rodmanActivatedBrush == null)
                {
                    rodmanActivatedBrush = new SolidBrush(rodmanActivatedColor);
                }

                return rodmanActivatedBrush;
            }
        }

        /// <summary>
        /// �α걻ѡ��״̬�Ļ�ˢ
        /// </summary>
        public Brush RodmanSelectedBrush
        {
            get
            {
                if (rodmanSelectedBrush == null)
                {
                    rodmanSelectedBrush = new SolidBrush(rodmanSelectedColor);
                }

                return rodmanSelectedBrush;
            }
        }        

        /// <summary>
        /// ���������ͨ״̬��ͼƬ����
        /// </summary>
        public Image ActionNodeNormalImage
        {
            get
            {
                if (actionNodeNormalImage == null)
                {
                    Image image = helper.LoadImageFromResource("actionNode_normal");
                    actionNodeNormalImage = ResizeImage(image, actionNodeElementSize);
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
                if (actionNodeSelectedImage == null)
                {
                    Image image = helper.LoadImageFromResource("actionNode_selected");
                    actionNodeSelectedImage = ResizeImage(image, actionNodeElementSize);
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
                if (actionNodeActivatedImage == null)
                {
                    Image image = helper.LoadImageFromResource("actionNode_activated");
                    actionNodeActivatedImage = ResizeImage(image, actionNodeElementSize);
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
                if (actionNodeShadowImage == null)
                {
                    Image image = helper.LoadImageFromResource("actionNode_shadow");
                    actionNodeShadowImage = ResizeImage(image, actionNodeElementSize);
                }

                return actionNodeShadowImage;
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
                    Image image = helper.LoadImageFromResource("actionNode_normal");
                    actionNodeAbbreviateImage = ResizeImage(image, actionNodeElementSize);
                }

                return actionNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ���������ͨ״̬��ͼƬ����
        /// </summary>
        public Image ConditionNodeNormalImage
        {
            get
            {
                if (conditionNodeNormalImage == null)
                {
                    Image image = helper.LoadImageFromResource("conditionNode_normal");
                    conditionNodeNormalImage = ResizeImage(image, conditionNodeElementSize);
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
                if (conditionNodeSelectedImage == null)
                {
                    Image image = helper.LoadImageFromResource("conditionNode_selected");
                    conditionNodeSelectedImage = ResizeImage(image, conditionNodeElementSize);
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
                if (conditionNodeActivatedImage == null)
                {
                    Image image = helper.LoadImageFromResource("conditionNode_activated");
                    conditionNodeActivatedImage = ResizeImage(image, conditionNodeElementSize);
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
                if (conditionNodeShadowImage == null)
                {
                    Image image = helper.LoadImageFromResource("conditionNode_shadow");
                    conditionNodeShadowImage = ResizeImage(image, conditionNodeElementSize);
                }

                return conditionNodeShadowImage;
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
                    Image image = helper.LoadImageFromResource("conditionNode_normal");
                    conditionNodeAbbreviateImage = ResizeImage(image, conditionNodeElementSize);
                }

                return conditionNodeAbbreviateImage;
            }
        }       

        /// <summary>
        /// �¼������ͨ״̬��ͼƬ����
        /// </summary>
        public Image EventNodeNormalImage
        {
            get
            {
                if (eventNodeNormalImage == null)
                {
                    Image image = helper.LoadImageFromResource("eventNode_normal");
                    eventNodeNormalImage = ResizeImage(image, eventNodeElementSize);
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
                if (eventNodeSelectedImage == null)
                {
                    Image image = helper.LoadImageFromResource("eventNode_selected");
                    eventNodeSelectedImage = ResizeImage(image, eventNodeElementSize);
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
                if (eventNodeActivatedImage == null)
                {
                    Image image = helper.LoadImageFromResource("eventNode_activated");
                    eventNodeActivatedImage = ResizeImage(image, eventNodeElementSize);
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
                if (eventNodeShadowImage == null)
                {
                    Image image = helper.LoadImageFromResource("eventNode_shadow");
                    eventNodeShadowImage = ResizeImage(image, eventNodeElementSize);
                }

                return eventNodeShadowImage;
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
                    Image image = helper.LoadImageFromResource("eventNode_normal");
                    eventNodeAbbreviateImage = ResizeImage(image, eventNodeElementSize);
                }

                return eventNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ��Ӱ�ť��ͨ״̬��ͼƬ����
        /// </summary>
        public Image AddButtonNormalImage
        {
            get
            {
                if (addButtonNormalImage == null)
                {
                    addButtonNormalImage = helper.LoadImageFromResource("addButton_normal");
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
                if (addButtonActivatedImage == null)
                {
                    addButtonActivatedImage = helper.LoadImageFromResource("addButton_activated");
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
                if (connectButtonNormalImage == null)
                {
                    connectButtonNormalImage = helper.LoadImageFromResource("connectButton_normal");
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
                if (connectButtonActivatedImage == null)
                {
                    connectButtonActivatedImage = helper.LoadImageFromResource("connectButton_activated");
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
                if (aiActionNodeNormalImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiActionNode_normal");
                    aiActionNodeNormalImage = ResizeImage(image, aiActionNodeElementSize);
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
                if (aiActionNodeSelectedImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiActionNode_selected");
                    aiActionNodeSelectedImage = ResizeImage(image, aiActionNodeElementSize);
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
                if (aiActionNodeActivatedImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiActionNode_activated");
                    aiActionNodeActivatedImage = ResizeImage(image, aiActionNodeElementSize);
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
                if (aiActionNodeShadowImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiActionNode_shadow");
                    aiActionNodeShadowImage = ResizeImage(image, aiActionNodeElementSize);
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
                    Image image = helper.LoadImageFromResource("aiActionNode_normal");
                    aiActionNodeAbbreviateImage = ResizeImage(image, aiActionNodeElementSize);
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
                if (aiActionsNodeNormalImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiActionsNode_normal");
                    aiActionsNodeNormalImage = ResizeImage(image, aiActionsNodeElementSize);
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
                if (aiActionsNodeSelectedImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiActionsNode_selected");
                    aiActionsNodeSelectedImage = ResizeImage(image, aiActionsNodeElementSize);
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
                if (aiActionsNodeActivatedImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiActionsNode_activated");
                    aiActionsNodeActivatedImage = ResizeImage(image, aiActionsNodeElementSize);
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
                if (aiActionsNodeShadowImage == null)
                {
                    Image image = helper.LoadImageFromResource("actionNode_shadow");
                    aiActionsNodeShadowImage = ResizeImage(image, aiActionsNodeElementSize);
                }

                return aiActionsNodeShadowImage;
            }
        }

        /// <summary>
        /// ai������������ͼƬ
        /// </summary>
        public Image AIActionsNodeAbbreviateImage
        {
            get
            {
                if (aiActionsNodeAbbreviateImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiActionsNode_normal");
                    aiActionsNodeAbbreviateImage = ResizeImage(image, aiActionsNodeElementSize);
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
                if (aiStateNodeNormalImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiStateNode_normal");
                    aiStateNodeNormalImage = ResizeImage(image, aiStateNodeElementSize);
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
                if (aiStateNodeSelectedImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiStateNode_selected");
                    aiStateNodeSelectedImage = ResizeImage(image, aiStateNodeElementSize);
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
                if (aiStateNodeActivatedImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiStateNode_activated");
                    aiStateNodeActivatedImage = ResizeImage(image, aiStateNodeElementSize);
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
                if (aiStateNodeShadowImage == null)
                {
                    Image image = helper.LoadImageFromResource("aiStateNode_shadow");
                    aiStateNodeShadowImage = ResizeImage(image, aiStateNodeElementSize);
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
                    Image image = helper.LoadImageFromResource("aiStateNode_normal");
                    aiStateNodeAbbreviateImage = ResizeImage(image, aiStateNodeElementSize);
                }

                return aiStateNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// �ӿڽ����ͨ״̬ͼƬ
        /// </summary>
        public Image InterfaceNodeNormalImage
        {
            get
            {
                if (interfaceNodeNormalImage == null)
                {
                    interfaceNodeNormalImage = helper.LoadImageFromResource("interfaceNode_normal");
                }

                return interfaceNodeNormalImage;
            }
        }

        /// <summary>
        /// �ӿڽ�㱻����״̬ͼƬ
        /// </summary>
        public Image InterfaceNodeActivatedImage
        {
            get
            {
                if (interfaceNodeActivatedImage == null)
                {
                    interfaceNodeActivatedImage = helper.LoadImageFromResource("interfaceNode_activated");
                }

                return interfaceNodeActivatedImage;
            }
        }

        /// <summary>
        /// �ӿڽ�㱻ѡ��״̬ͼƬ
        /// </summary>
        public Image InterfaceNodeSelectedImage
        {
            get
            {
                if (interfaceNodeSelectedImage == null)
                {
                    interfaceNodeSelectedImage = helper.LoadImageFromResource("interfaceNode_selected");
                }

                return interfaceNodeSelectedImage;
            }
        }

        /// <summary>
        /// �ӿڽ����ӰͼƬ
        /// </summary>
        public Image InterfaceNodeShadowImage
        {
            get
            {
                if (interfaceNodeShadowImage == null)
                {
                    interfaceNodeShadowImage = helper.LoadImageFromResource("interfaceNode_shadow");
                }

                return interfaceNodeShadowImage;
            }
        }

        /// <summary>
        /// �ӿڽ������ͼƬ
        /// </summary>
        public Image InterfaceNodeAbbreviateImage
        {
            get
            {
                if (interfaceNodeAbbreviateImage == null)
                {                    
                    interfaceNodeAbbreviateImage = helper.LoadImageFromResource("interfaceNode_normal");
                }

                return interfaceNodeAbbreviateImage;
            }
        }

        /// <summary>
        /// ע�͵ı�����ˢ
        /// </summary>
        public Brush RemarkBackgroundBrush
        {
            get
            {
                if (remarkBackgroundBrush == null)
                {
                    remarkBackgroundBrush = new SolidBrush(remarkBackgroundColor);
                }

                return remarkBackgroundBrush;
            }
        }

        /// <summary>
        /// ͼԪ�Ļ���
        /// </summary>
        public Pen GraphElementNormalPen
        {
            get
            {
                if (graphElementNormalPen == null)
                {
                    graphElementNormalPen = new Pen(graphElementNormalColor, graphElementBorderWidth);
                }

                return graphElementNormalPen;
            }
        }

        /// <summary>
        /// ͼԪ������״̬�Ļ���
        /// </summary>
        public Pen GraphElementActivatedPen
        {
            get
            {
                if (graphElementActivatePen == null)
                {
                    graphElementActivatePen = new Pen(graphElementActivatedColor, graphElementBorderWidth);
                }

                return graphElementActivatePen;
            }
        }

        /// <summary>
        /// ͼԪ��ѡ��״̬�Ļ���
        /// </summary>
        public Pen GraphElementSelectedPen
        {
            get
            {
                if (graphElementSelectedPen == null)
                {
                    graphElementSelectedPen = new Pen(graphElementSelectedColor, graphElementBorderWidth);
                }

                return graphElementSelectedPen;
            }
        }

        /// <summary>
        /// ͼԪ�ı߿򻭱�
        /// </summary>
        public Pen GraphElementBorderPen
        {
            get
            {
                if (graphElementBorderPen == null)
                {
                    graphElementBorderPen = new Pen(graphElementBorderColor, graphElementBorderWidth);
                }

                return graphElementBorderPen;
            }
        }

        /// <summary>
        /// ͼԪ����״̬�Ļ�ˢ
        /// </summary>
        public Brush GraphElementBindedBrush
        {
            get
            {
                if (graphElementBindedBrush == null)
                {
                    graphElementBindedBrush = new SolidBrush(graphElementBindedColor);
                }

                return graphElementBindedBrush;
            }
        }

        /// <summary>
        /// ������ˢ
        /// </summary>
        public Brush BackgroundBrush
        {
            get
            {
                if (backgroundBrush == null)
                {
                    backgroundBrush = new SolidBrush(backgroundColor);
                }

                return backgroundBrush;
            }
        }

        /// <summary>
        /// ����ͷ�������ߵĻ���
        /// </summary>
        public Pen LineNormalArrawPen
        {
            get
            {
                if (lineNormalArrawPen == null)
                {
                    lineNormalArrawPen = new Pen(graphElementNormalColor, lineWidth);
                    lineNormalArrawPen.CustomEndCap = new AdjustableArrowCap(lineWidth * 2, lineWidth * 3, true);
                }

                return lineNormalArrawPen;
            }
        }

        /// <summary>
        /// ����ͷ�������߱�����״̬�Ļ���
        /// </summary>
        public Pen LineActivatedArrawPen
        {
            get
            {
                if (lineActivatedArrawPen == null)
                {
                    lineActivatedArrawPen = new Pen(graphElementActivatedColor, lineWidth);
                    lineActivatedArrawPen.CustomEndCap = new AdjustableArrowCap(lineWidth * 2, lineWidth * 3, true);
                }

                return lineActivatedArrawPen;
            }
        }

        /// <summary>
        /// ����ͷ�����߱�ѡ��״̬�Ļ���
        /// </summary>
        public Pen LineSelectedArrawPen
        {
            get
            {
                if (lineSelectedArrawPen == null)
                {
                    lineSelectedArrawPen = new Pen(graphElementSelectedColor, lineWidth);
                    lineSelectedArrawPen.CustomEndCap = new AdjustableArrowCap(lineWidth * 2, lineWidth * 3, true);
                }

                return lineSelectedArrawPen;
            }
        }

        /// <summary>
        /// ����ͷ�����߱����״̬�Ļ���
        /// </summary>
        public Pen LineMarkedArrawPen
        {
            get
            {
                if (lineMarkedArrawPen == null)
                {
                    lineMarkedArrawPen = new Pen(lineMarkedColor, lineWidth);
                    lineMarkedArrawPen.CustomEndCap = new AdjustableArrowCap(lineWidth * 2, lineWidth * 3, true);
                }

                return lineMarkedArrawPen;
            }
        }

        /// <summary>
        /// �����߱����״̬�Ļ���
        /// </summary>
        public Pen LineMarkedPen
        {
            get
            {
                if (lineMarkdPen == null)
                {
                    lineMarkdPen = new Pen(lineMarkedColor, lineWidth);
                }

                return lineMarkdPen;
            }
        }

        /// <summary>
        /// �����ߵĻ���
        /// </summary>
        public Pen LineNormalPen
        {
            get
            {
                if (lineNormalPen == null)
                {
                    lineNormalPen = new Pen(graphElementNormalColor, lineWidth);
                }

                return lineNormalPen;
            }
        }

        /// <summary>
        /// �����߱�����״̬�Ļ���
        /// </summary>
        public Pen LineActivatedPen
        {
            get
            {
                if (lineActivatedPen == null)
                {
                    lineActivatedPen = new Pen(graphElementActivatedColor, lineWidth);
                }

                return lineActivatedPen;
            }
        }

        /// <summary>
        /// �����߱�ѡ��״̬�Ļ���
        /// </summary>
        public Pen LineSelectedPen
        {
            get
            {
                if (lineSelectedPen == null)
                {
                    lineSelectedPen = new Pen(graphElementSelectedColor, lineWidth);
                }

                return lineSelectedPen;
            }
        }

        /// <summary>
        /// �����ߵ��ı���ˢ
        /// </summary>
        public Brush LineTextBrush
        {
            get
            {
                if (lineTextBrush == null)
                {
                    lineTextBrush = new SolidBrush(lineTextColor);
                }

                return lineTextBrush;
            }
        }

        /// <summary>
        /// �α�Ŀ��
        /// </summary>
        public int RodmanWidth
        {
            set
            {
                rodmanWidth = value;
            }

            get
            {
                return rodmanWidth;
            }
        }

        /// <summary>
        /// �α걻ѡ�е���ɫ
        /// </summary>
        public Color RodmanselectedColor
        {
            set
            {
                rodmanSelectedColor = value;
            }

            get
            {
                return rodmanSelectedColor;
            }
        }

        /// <summary>
        /// �α걻�������ɫ
        /// </summary>
        public Color RodmanActivatedColor
        {
            set
            {
                rodmanActivatedColor = value;
            }

            get
            {
                return rodmanActivatedColor;
            }
        }

        /// <summary>
        /// �α����ɫ
        /// </summary>
        public Color RodmanBackgroundColor
        {
            set
            {
                rodmanBackgroundColor = value;
            }

            get
            {
                return rodmanBackgroundColor;
            }
        }

        /// <summary>
        /// ͼԪ���ı���ɫ
        /// </summary>
        public Color GraphElementTextColor
        {
            set
            {
                graphElementTextColor = value;
            }

            get
            {
                return graphElementTextColor;
            }
        }

        /// <summary>
        /// ͼԪ���ı���ˢ
        /// </summary>
        public Brush GraphElementTextBrush
        {
            get
            {
                if (graphElementTextBrush == null)
                {
                    graphElementTextBrush = new SolidBrush(graphElementTextColor);
                }

                return graphElementTextBrush;
            }
        }

        /// <summary>
        /// ͼԪ���ı�����
        /// </summary>
        public Font GraphElementTextFont
        {
            get
            {
                if (graphElementTextFont == null)
                {
                    graphElementTextFont = new Font("Sim Sun", 8, FontStyle.Regular);
                }

                return graphElementTextFont;
            }
        }

        /// <summary>
        /// ͼԪ���ı���������
        /// </summary>
        public Font GraphElementTextBoldFont
        {
            get
            {
                if (graphElementTextBoldFont == null)
                {
                    graphElementTextBoldFont = new Font("Sim Sun", 8, FontStyle.Bold);
                }

                return graphElementTextBoldFont;
            }            
        }

        /// <summary>
        /// ͼԪ���ı�б������
        /// </summary>
        public Font GraphElementTextItalicFont
        {
            get
            {
                if (graphElementTextItalicFont == null)
                {
                    graphElementTextItalicFont = new Font("Sim Sun", 8, FontStyle.Italic);
                }

                return graphElementTextItalicFont;
            }
        }

        /// <summary>
        /// ͼԪ���ı��»�������
        /// </summary>
        public Font GraphElementTextUnderLineFont
        {
            get
            {
                if (graphElementTextUnderLineFont == null)
                {
                    graphElementTextUnderLineFont = new Font("Sim Sun", 8, FontStyle.Underline);                    
                }

                return graphElementTextUnderLineFont;
            }
        }

        /// <summary>
        /// ͼԪ��ͨ״̬�Ļ�ˢ
        /// </summary>
        public Brush GraphElementNormalBrush
        {
            get
            {
                if (graphElementNormalBrush == null)
                {
                    graphElementNormalBrush = new SolidBrush(graphElementNormalColor);
                }

                return graphElementNormalBrush;
            }
        }

        /// <summary>
        /// ͼԪ��ѡ��״̬�Ļ�ˢ
        /// </summary>
        public Brush GraphElementSelectedBrush
        {
            get
            {
                if (graphElementSelectedBrush == null)
                {
                    graphElementSelectedBrush = new SolidBrush(graphElementSelectedColor);
                }

                return graphElementSelectedBrush;
            }
        }

        /// <summary>
        /// ͼԪ������״̬�Ļ�ˢ
        /// </summary>
        public Brush GraphElementActivatedBrush
        {
            get
            {
                if (graphElementActivatedBrush == null)
                {
                    graphElementActivatedBrush = new SolidBrush(graphElementActivatedColor);
                }

                return graphElementActivatedBrush;
            }
        }

        /// <summary>
        /// ����ͼƬ
        /// </summary>
        /// <param name="image">ԭʼͼƬ</param>
        /// <param name="size">ͼƬ��С</param>
        /// <returns>��ͼƬ</returns>
        private Image ResizeImage(Image image, Size size)
        {
            Bitmap bmp = new Bitmap(size.Width, size.Height);
            Graphics g = Graphics.FromImage(bmp);
            Rectangle rectangle = new Rectangle(Point.Empty, size);
            g.DrawImage(image, rectangle);
            g.Dispose();

            return bmp;
        }
    }
}
