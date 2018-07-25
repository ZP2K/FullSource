using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Collections;

namespace LogicalEditor
{
    public class Canvas : Panel
    {
        private Brush blueBrush = Brushes.Blue; // ��ɫ��ˢ��
        private Brush yellowBrush = Brushes.Yellow; // ��ɫ��ˢ��
        private Pen redPen = new Pen(Color.Red, 4); // ��ɫ�ı�
        private Pen greenPen = new Pen(Color.Green, 1); // ��ɫ�ı�
        private Font timesFont = new Font("Times New Roman", 8, FontStyle.Bold); // ��ʾ���ֵ�����
        private List<Point> markPointList = new List<Point>(); // ��ǵĵ�����
        private List<Point> multiPointList = new List<Point>(); // �������ǵĵ�����
        private Bitmap bmp; // ����ͼƬ
        Size littileSize = new Size(1, 1); // 1�����صĿ��
        public Image image; // Ҫ���Ƶ�ͼƬ
        private string text; // Ҫ��ʾ������
        private float textX; // Ҫ��ʾ�����ֵĺ�����
        private float textY; // Ҫ��ʾ�����ֵ�������
        private bool blueString = true; // ����ɫ��ˢ����ʾ����
        public int canvasWidth = 600; // �����Ŀ��
        public int canvasHeight = 480; // �����ĸ߶�
        public int m_lenDirEnd = 24; // ����ĳ���

        public float scale = 1.0f; // ����
        public float start_adjust_x = 0, start_adjust_y = 0; // �����м��ƶ�����ʼ��

        public Hashtable m_ODIs = new Hashtable(); // ObjDrawInfo ��hash

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

        // ѡ�е�� ODI
        public void AddODI(string key, ObjDrawInfo odi)
        {
            if (!m_ODIs.ContainsKey(key))
                m_ODIs.Add(key, odi);
        }
        public void RemoveODI(string key)
        {
            if (m_ODIs.ContainsKey(key))
                m_ODIs.Remove(key);
        }

        public PointF GetDirEndAdjustment(int nDir)
        {
            double angel = (double)nDir / 256 * Math.PI * 2;
            return new PointF((float)(m_lenDirEnd * Math.Cos(angel)), (float)(-m_lenDirEnd * Math.Sin(angel)));
        }

        public PointF GetODIClientPos(ObjDrawInfo odi)
        {
            float startscalex = ((1.0f - scale) * image.Width / 2 + start_adjust_x) / image.Width;
            float startscaley = ((1.0f - scale) * image.Height / 2 + start_adjust_y) / image.Height;
            float x = (odi.pfPos.X - startscalex) * canvasWidth / scale;
            float y = (odi.pfPos.Y - startscaley) * canvasHeight / scale;
            return new PointF(x, y);
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

            if (image != null) // ����ͼƬ
            {
                RectangleF destRect = new RectangleF(0, 0, canvasWidth, canvasHeight);
                RectangleF srcRect = new RectangleF((1.0f - scale) * image.Width / 2 + start_adjust_x, (1.0f - scale) * image.Height / 2 + start_adjust_y, image.Width * scale, image.Height * scale);
                g.DrawImage(image, destRect, srcRect, GraphicsUnit.Pixel);

                foreach (DictionaryEntry entry in m_ODIs)
                {
                    ObjDrawInfo odi = entry.Value as ObjDrawInfo;
                    PointF pointf = GetODIClientPos(odi);
                    g.DrawEllipse(redPen, pointf.X - 3, pointf.Y - 3, 7, 7);
                    int nDir = (int)(odi.nDir);
                    if ((this.Parent as Preview).m_ShowDir && nDir != -1)
                    {
                        PointF adjustment = GetDirEndAdjustment(nDir);
                        g.DrawLine(redPen, pointf.X, pointf.Y, pointf.X + adjustment.X, pointf.Y + adjustment.Y);
                    }
                    if ((this.Parent as Preview).m_ShowObjName)
                    {
                        LogicObj curObj = odi.lObj;
                        string strODI = string.Format("{0}[{1}]", curObj.tyPe.ToLower().EndsWith("point") ? "Point" : (curObj.tyPe.ToLower().EndsWith("vertex") ? "Vertex" : curObj.values[0]), curObj.inDex);
                        g.DrawString(strODI, timesFont, yellowBrush, pointf.X + 3, pointf.Y + 7);
                    }
                }
            }

            // �ӻ����л�������ͼ��
            dc.DrawImage(bufferBMP, 0, 0); // g.DrawLine(greenPen, p, p + littileSize);
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

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="text">��������</param>
        /// <param name="x">���ֵĺ�����</param>
        /// <param name="y">���ֵ�������</param>
        /// <param name="blueString">�����Ƿ�����ɫ��ʾ</param>
        public void PaintString(string text, int x, int y, bool blueString)
        {
            this.text = text;
            this.blueString = blueString;

            if (x > canvasWidth - 100)
            {
                textX = canvasWidth - 100;
            }
            else
            {
                textX = x;
            }

            if (y > canvasHeight - 50)
            {
                textY = canvasHeight - 50;
            }
            else
            {
                textY = y;
            }

            this.Refresh();
        }

        /// <summary>
        /// ���ѡ�е�����
        /// </summary>
        /// <param name="value">�ҶȲ���</param>
        public void MarkArea(int value)
        {
            markPointList.Clear();
            Color color;
            int x;
            int y;

            for (int i = 1; i < canvasWidth; i += 5)
            {
                for (int j = 1; j < canvasHeight; j += 5)
                {
                    x = (int)((double)i * image.Width / canvasWidth);
                    y = (int)((double)j * image.Height / canvasHeight);
                    if ((x < bmp.Width) && (y < bmp.Height))
                    {
                        color = bmp.GetPixel(x, y);
                        if (color.B == value)
                        {
                            markPointList.Add(new Point(i, j));
                        }
                    }
                }
            }

            this.Refresh();
        }

        /// <summary>
        /// ���ƶ�������
        /// </summary>
        /// <param name="values">����ĻҶ�����</param>
        public void MultiMarkArea(List<int> values)
        {
            multiPointList.Clear();
            Color color;
            int x;
            int y;

            for (int i = 1; i < canvasWidth; i += 5)
            {
                for (int j = 1; j < canvasHeight; j += 5)
                {
                    x = (int)((double)i * image.Width / canvasWidth);
                    y = (int)((double)j * image.Height / canvasHeight);
                    if ((x < bmp.Width) && (y < bmp.Height))
                    {
                        color = bmp.GetPixel(x, y);
                        if (values.Contains(color.B))
                        {
                            multiPointList.Add(new Point(i, j));
                        }
                    }
                }
            }

            this.Refresh();
        }

        /// <summary>
        /// �����ͼ���
        /// </summary>
        public void ClearPaint()
        {
            markPointList.Clear();
            multiPointList.Clear();
            text = null;
            this.Refresh();
        }
    }
}
