using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Collections;

namespace QuestGuide
{
    public enum TemplateType
    {
        NPC_TEMPLATE,
        DOODAD_TEMPLATE,
        NONE
    }
  
    public class MapConfig
    {
        private int nWidth = 0;
        private int nHeight = 0;
        private float fScale = 0.5f;
        private float fStartx = 0.0f;
        private float fStarty = 0.0f;
        private int nMapId = 0;
        private string strMapName = "";
        private Image image = null;
        private float fTotalWidth = 0.0f;
        private float fTotalHeight = 0.0f;

       
        public Image TagImage
        {
            get
            {
                return image;
            }
            set
            {
                image = value;
            }
        }
        public string MapName
        {
            get
            {
                return strMapName;
            }
        }
        public MapConfig(int width, int height, float scale, float startx, float starty, int mapid, string mapname)
        {
            nWidth = width;
            nHeight = height;
            fScale = scale;
            fStarty = starty;
            fStartx = startx;
            nMapId = mapid;
            strMapName = mapname;
            fTotalWidth = nWidth / fScale;
            fTotalHeight = nHeight / fScale;
        }
        public void GetConfig(ref int width, ref int height, ref float startx, ref float starty, ref float scale)
        {
            width = nWidth;
            height = nHeight;
            startx = fStartx;
            starty = fStarty;
            scale = fScale;
        }
        public float StartX
        {
            get { return fStartx; }
        }
        public float StartY
        {
            get { return fStarty; }
        }
        public float MapTotalWidth
        {
            get { return fTotalWidth; }
        }
        public float MapTotalHeight
        {
            get { return fTotalHeight; }
        }
        public int MapId
        {
            get
            {
                return nMapId;
            }
        }
    };
   
   
    public class CircleType
    {
        public static int MaxType = 5;
        public static int MinType = 2;
        public static int DefaultType = 3;
        public static float ClientWidth = 1024.0f;
        public static float ClientHeight = 896.0f;
        public static float WidthHeigthScale = 1024.0f / 896.0f;
        public static float[] Radius = new float[6] {0f, 0f, 54f, 68.4f, 82.8f, 97.2f};
    };

    public enum NodeType{POINT, REGION}
    public class OneNode
    {
        public int MapId;
        private int old_MapId;
        public Image imageMap;
        public bool CanEdit;
        private string strColumunNameKey = "";
       
        private ArrayList listGrayId;
        private string old_strGrayIds;

        public int nX;
        public int nY;
        public float fScaleX;
        public float fScaleY;
        public Brush PointColor;

        public NodeType nodeType;
        public TemplateType templateType;
        public ArrayList arrBrothers = null;

        ~OneNode()
        {
            if (arrBrothers != null)
            {
                arrBrothers.Clear();
                arrBrothers = null;
            }
            if (listGrayId != null)
            {
                listGrayId.Clear();
                listGrayId = null;
            }
        }

        public void Init(int mapid, int x, int y, Brush color)
        {
            nX = x;
            nY = y;
            fScaleX = 0.5f;
            fScaleY = 0.5f;
            PointColor = color;            
            Init(mapid, NodeType.POINT);

        }
        public void Init(int mapid, string areas, TemplateType ttype, string columnKey)
        {
            Init(mapid, NodeType.REGION);            
            templateType = ttype;
            old_strGrayIds = areas;
            listGrayId = new ArrayList();
            ColumnNameKey = columnKey;
            foreach (string area in areas.Split(new char[]{','}, StringSplitOptions.RemoveEmptyEntries))
            {
                int grayId;
                if (int.TryParse(area, out grayId))
                {
                    listGrayId.Add(grayId);
                }
            }
        }
        private void Init(int mapid, NodeType type)
        {
            MapId = mapid;
            old_MapId = mapid;
            nodeType = type;
            imageMap = null;
            CanEdit = false;
        } 
        public bool ContainGrayId(int grayId)
        {
            return listGrayId.Contains(grayId);
        }
        public void AddGrayId(int grayId)
        {
            listGrayId.Add(grayId);
        }
        public void RemoveGrayId(int grayId)
        {
            listGrayId.Remove(grayId);
        }
        public Array GrayIdArray
        {
            get { return listGrayId.ToArray(); }
        }
        public void ClearGrayId()
        {
            listGrayId.Clear();
        }
        public string StringGrayIds
        {
            get 
            {
                string strRet = string.Empty;
                foreach (int grayid in this.GrayIdArray)
                {
                    strRet +=string.Format("{0},", grayid);
                }
                if (strRet != string.Empty)
                {
                    strRet = strRet.Substring(0, strRet.Length - 1);
                }
                return strRet;
            }
            
        }
        public bool IsChanged
        {
            get
            {
                if (old_MapId != MapId)
                    return true;
                string[] grayids = old_strGrayIds.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                if (grayids.Length != listGrayId.Count)
                    return true;
                int grayid;
                foreach (string strGray in grayids)
                {
                    grayid = Convert.ToInt32(strGray);

                    if (!listGrayId.Contains(grayid))
                        return true;                    
                }

                return false;
            }
        }
        public string ColumnNameKey
        {
            get { return strColumunNameKey; }
            set { strColumunNameKey = value; }
        }
        public void BackUp()
        {
            old_strGrayIds = this.StringGrayIds;
            old_MapId = MapId;
        }

    };

    public class Canvas : Panel
    {
        private Bitmap bmp; // ����ͼƬ
        public Image image; // Ҫ���Ƶ�ͼƬ

        public int canvasWidth = 600; // �����Ŀ��
        public int canvasHeight = 480; // �����ĸ߶�

        private ArrayList ArryQuestNode = new ArrayList(2);
        public int CurrentMapId = 0;
        public OneNode CurrentNode;
        
        const float Diameter = 15.0f;
        const float RegionPointDiameter = 4.5f;

        private float[,] RegionPoints = new float[8000, 2];
        public MapConfig CurrentConfig;
        public QuestDirector2.RegionInfo CurrentRegion;

        private static Random rand = new Random();
        private ArrayList listColor = new ArrayList();
        private int colorIndex;


        /// <summary>
        /// ���캯��
        /// </summary>
        public Canvas()
        {
            // ����˫����
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer |
                    ControlStyles.ResizeRedraw |
                    ControlStyles.AllPaintingInWmPaint, true);            
        }

        /// <summary>
        /// �����Ŀ������
        /// </summary>
        public int CanvasWidth
        {
            get
            {
                return canvasWidth;
            }
        }

        /// <summary>
        /// �����ĸ߶�����
        /// </summary>
        public int CanvasHeight
        {
            get
            {
                return canvasHeight;
            }
        }


        /// <summary>
        /// ��д�Ļ�ͼ����
        /// </summary>
        /// <param name="e">��ͼ�¼�����</param>
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics dc = e.Graphics;
            if (canvasWidth == 0 || canvasHeight == 0)
                return;

            Bitmap bufferBMP = new Bitmap(canvasWidth, canvasHeight); // ��ͼ�Ļ���
            Graphics g = Graphics.FromImage(bufferBMP);
            g.Clear(this.BackColor);
            g.SmoothingMode =  SmoothingMode.HighQuality; //������
            g.PixelOffsetMode = PixelOffsetMode.HighQuality; //������ƫ������

            PaintMap(g);            
           // PaintRegions(g);
            PaintNodeBrothers(CurrentNode, g);
            PaintPoints(g);

            dc.DrawImage(bufferBMP, 0, 0);
        }

        /// <summary>
        /// ���軭���ĳߴ�
        /// </summary>
        /// <param name="width">���</param>
        /// <param name="height">�߶�</param>
        public void ResetSize(int width, int height)
        {
            this.canvasWidth = width;
            this.canvasHeight = height;
            this.Refresh();
        }

        /// <summary>
        /// ����ͼƬ
        /// </summary>
        /// <param name="image">ͼƬ</param>
        public void PaintPicture(Image img)
        {
            this.image = img;
            bmp = new Bitmap(this.image);
            this.Refresh();
        }

        public void ClearPaint()
        {
            ColorClear();
            this.ArryQuestNode.Clear();
            this.CurrentNode = null;            
            this.Refresh();
        }
        public void AddNode(OneNode node)
        {
            if (node != null)
            {
                ArryQuestNode.Add(node);
            }
        }       
        
        public void ClearNodes()
        {
            ArryQuestNode.Clear();
            CurrentNode = null;
        }
        private void PaintMap(Graphics g)
        {
            if (CurrentNode == null || CurrentNode.imageMap == null)
            {
                return;
            }
            image = CurrentNode.imageMap;
            RectangleF destRect = new RectangleF(0, 0, canvasWidth, canvasHeight);
            RectangleF srcRect = new RectangleF(0, 0, image.Width, image.Height);
            g.DrawImage(image, destRect, srcRect, GraphicsUnit.Pixel);
        }

        private void PaintPoints(Graphics g)
        {
            if (CurrentNode ==null)
            {
                return;
            }           
            const int PointCntCapcity = 31;        
            Hashtable htOffset = new Hashtable(PointCntCapcity);

            int CurrentMapId = CurrentNode.MapId;
            foreach (OneNode node in ArryQuestNode)
            {  
                if (node.MapId == CurrentMapId)
                {
                    float fx = node.fScaleX * CanvasWidth ;
                    float fy = node.fScaleY * canvasHeight;                
                    float fxOffset = 0.0f;
                    float fyOffset = 0.0f;
                    GetOffset(htOffset, fx, fy, ref fxOffset, ref fyOffset);
                    float centerx = fx + fxOffset;
                    float centery = fy + fyOffset;
                    fx = centerx - Diameter / 2;
                    fy = centery - Diameter / 2;
                    g.FillEllipse(node.PointColor, fx, fy, Diameter, Diameter);
                    System.Drawing.Font font = new Font("SimSun", 10.0f);
                }                      
            }          
        }
        private void PaintRegions(Graphics g)
        {
            if (CurrentNode == null || 
                CurrentNode.nodeType == NodeType.POINT)
            {
                return;
            }            
            float fScaleX;
            float fScaleY;
            SolidBrush brush;       
            int nPointNumReturn;
            ColorReWind();
            foreach (int GrayId in CurrentNode.GrayIdArray)
            {
                brush = new SolidBrush(NextColor());
                if (!GetCurrenDrawData(GrayId, 8, this.RegionPoints, out nPointNumReturn))
                {
                    g.DrawString("���鲻��������ĵ���Щû�д����!", new Font("SimSun", 20.0f), Brushes.White, 0.0f, 0.0f);
                }
                for (int i =0; i < nPointNumReturn; i++)
                {
                    fScaleX = RegionPoints[i, 0];
                    fScaleY = RegionPoints[i, 1];                    
                    g.FillEllipse(brush, fScaleX * CanvasWidth, fScaleY * CanvasHeight, RegionPointDiameter, RegionPointDiameter);  

                }
              
            }
            PaintNodeBrothers(CurrentNode, g);
            
        }
        private void PaintNodeBrothers(OneNode node, Graphics g)
        {
            if (node == null ||
                node.nodeType == NodeType.POINT)
            {
                return;
            }
            ArrayList arrBrothers = node.arrBrothers;
            if (arrBrothers == null)
            {
                return;
            }
            float mydiameter = Diameter * 0.5f;

            foreach (object ob in arrBrothers)
            {
                MapLogicalFileReader.fPoint2d fpoint = (MapLogicalFileReader.fPoint2d)ob;
                float fx = fpoint.fX * CanvasWidth;
                float fy = fpoint.fY * canvasHeight;
                g.FillEllipse(Brushes.Purple, fx - mydiameter / 2, fy - mydiameter / 2, mydiameter, mydiameter);               
            }

        }
        private void GetOffset(Hashtable ht, float fx, float fy, ref float fxoff, ref float fyoff)
        {
            float x = fx / 4;
            float y = fy / 4;
            string strKey = string.Format("x:{0}y:{1}", Convert.ToInt32(x), Convert.ToInt32(y));
            int cntRepeat = 0;
            
            if (ht.ContainsKey(strKey))
            {
                cntRepeat = Convert.ToInt32(ht[strKey]);
                switch (cntRepeat)
                {                   
                    case 1:
                        fxoff = Diameter / 2;
                        fyoff = 0.0f;
                        break;
                    case 2:
                        fxoff = 0.0f;
                        fyoff = Diameter / 2;
                        break;
                    case 3:
                        break;
                    default:
                        break;
                }               
            }
            else
            {
                fxoff = 0.0f;
                fyoff = 0.0f;                
            }
            ht[strKey] = ++cntRepeat;

        }
        private bool GetCurrenDrawData(int GrayId, int BmpStep, float[,] dataRet, out int dataLen)
        {
            bool bResult = false;
            dataLen = 0;
            if (CurrentNode.nodeType == NodeType.POINT ||
                CurrentConfig == null ||
                CurrentRegion == null)
            {
                return false;
            }

            bResult = CurrentRegion.GetGreyPoints(GrayId, CurrentConfig.StartX, CurrentConfig.StartY, CurrentConfig.MapTotalWidth, CurrentConfig.MapTotalHeight, BmpStep, dataRet, out dataLen);

            return bResult;
        }
        private Color RandomColor()
        {
            return Color.FromArgb(rand.Next(255), rand.Next(255), rand.Next(255));
        }
        private Color NextColor()
        {            
            Color color;
            if (colorIndex > listColor.Count)
            {
                //error:
                color = RandomColor();
            }
            else if (colorIndex == listColor.Count)
            {
                color = RandomColor();
                listColor.Add(color);
            }
            else
            {
                color = (Color)listColor[colorIndex];
            }

            colorIndex++;
            return color;
        }
        private void ColorReWind()
        {
            colorIndex = 0;
        }
        private void ColorClear()
        {
            colorIndex = 0;
            listColor.Clear();
        }
    }

}
