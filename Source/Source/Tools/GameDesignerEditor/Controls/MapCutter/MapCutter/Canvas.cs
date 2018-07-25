using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace MapCutter
{
    class Canvas : Panel
    {
        private Brush blueBrush = Brushes.Blue; // ��ɫ��ˢ��
        private Brush yellowBrush = Brushes.Yellow; // ��ɫ��ˢ��
        private Pen redPen = new Pen(Color.Red, 1); // ��ɫ�ı�   
        private Pen greenPen = new Pen(Color.Green, 1); // ��ɫ�ı�
        private Font timesFont = new Font("Times New Roman", 8, FontStyle.Bold); // ��ʾ���ֵ�����
        private List<Point> markPointList = new List<Point>(); // ��ǵĵ�����
        private List<Point> multiPointList = new List<Point>(); // �������ǵĵ�����
        private Bitmap bmp; // ����ͼƬ
        Size littileSize = new Size(1, 1); // 1�����صĿ��
        private Image image; // Ҫ���Ƶ�ͼƬ
        private string text; // Ҫ��ʾ������
        private float textX; // Ҫ��ʾ�����ֵĺ�����
        private float textY; // Ҫ��ʾ�����ֵ�������
        private bool blueString = true; // ����ɫ��ˢ����ʾ����
        private int canvasWidth = 600; // �����Ŀ��
        private int canvasHeight = 480; // �����ĸ߶�

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
            Bitmap bufferBMP = new Bitmap(canvasWidth, canvasHeight); // ��ͼ�Ļ���
            Graphics g = Graphics.FromImage(bufferBMP);
            g.Clear(this.BackColor);
            g.SmoothingMode =  SmoothingMode.HighQuality; //������
            g.PixelOffsetMode = PixelOffsetMode.HighQuality; //������ƫ������

            if (image != null) // ����ͼƬ
            {
                g.DrawImage(image, 0, 0, canvasWidth, canvasHeight);
            }

            if (multiPointList.Count != 0) // ���ƶ������ǵĵ�
            {
                foreach (Point p in multiPointList)
                {
                    g.DrawLine(greenPen, p, p + littileSize);
                }
            }

            if (markPointList.Count != 0) // ����ѡ�еĵ�
            {
                foreach (Point p in markPointList)
                {
                    g.DrawLine(redPen, p, p + littileSize);
                }
            }

            if (text != null) // ��������
            {
                if (blueString) // ����ɫ��ʾ����
                {
                    g.DrawString(text, timesFont, blueBrush, textX, textY);
                }
                else // �û�ɫ��ʾ����
                {
                    g.DrawString(text, timesFont, yellowBrush, textX, textY);
                }
            }

            // �ӻ����л�������ͼ��
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
        public void PaintPicture(Image image)
        {
            this.image = image;
            bmp = new Bitmap(image);
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
