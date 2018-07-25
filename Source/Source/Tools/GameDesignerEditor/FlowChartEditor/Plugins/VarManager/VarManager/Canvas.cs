using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Collections;
using System.Data;
using System.Drawing.Drawing2D;

namespace VarManager
{
    class Canvas:Panel
    {
        private DataTable table; // �������ݱ�
        private Font textFont = new Font("Sim Sun", 10, FontStyle.Bold);
        private Pen borderPen = new Pen(Color.Blue);
        private Brush textBrush = Brushes.Black;
        private Brush backGroundBrush = Brushes.Green;
        private Brush varBrush = Brushes.Red;
        private Hashtable varTable = new Hashtable();
        private List<object[]> dataList = new List<object[]>();
        private bool initFinished = false;
        private int maxStringLength = 0;

        /// <summary>
        /// ���캯��
        /// </summary>
        public Canvas()
        {
            // ����˫����Ȼ��Ʋ���
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.ResizeRedraw, true);
        }

        /// <summary>
        /// �����ַ����ĳ���
        /// </summary>
        /// <param name="text">��ǰ�ַ���</param>
        /// <param name="font">�ı�����</param>
        /// <returns>�ַ����ĳ���</returns>
        private int MeasureStringLength(string text, Font font)
        {
            Bitmap bmp = new Bitmap(1, 1);
            Graphics g = Graphics.FromImage(bmp);
            SizeF textSize = g.MeasureString(text, font);
            float textWidth = textSize.Width;
            int stringLength = (int)textWidth;
            bmp.Dispose();

            return stringLength;
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        /// <param name="table">���ݱ�</param>
        public void Init(DataTable table)
        {
            this.table = table;
            List<string> sceneList = new List<string>();
            string varName;
            int stringLength;
            object[] args;

            foreach(DataRow row in table.Rows)
            {
                string bindType = row["BindType"].ToString();
                switch(bindType)
                {
                    case "����":
                        {
                            string sceneName = row["BindObject"].ToString();
                            if(!sceneList.Contains(sceneName))
                            {
                                sceneList.Add(sceneName);
                            }
                            
                            break;
                        }
                    case "���":
                        {
                            break;
                        }
                }
            }

            int index = 0;
            Point point;
            Size size;

            foreach(string s in sceneList)
            {
                varName = string.Format("�������� [{0}]", s);
                stringLength = MeasureStringLength(varName, textFont);
                if (stringLength > maxStringLength)
                {
                    maxStringLength = stringLength;
                } 
                point = new Point(5, index * 50 + 5);
                size = new Size(5120, 30);
                args = new object[3];
                args[0] = varName;
                args[1] = point;
                args[2] = size;
                dataList.Add(args);
                varTable[s] = point;
                index++;
            }

            varName = "��ұ���";
            stringLength = MeasureStringLength(varName, textFont);
            if (stringLength > maxStringLength)
            {
                maxStringLength = stringLength;
            }
            point = new Point(5, index * 50 + 5);
            size = new Size(640, 30);
            args = new object[3];
            args[0] = varName;
            args[1] = point;
            args[2] = size;
            dataList.Add(args);
            varTable[varName] = point;
            index++;            

            this.AutoScrollMinSize = new Size(10 + maxStringLength + 5120 + 50, index * 50);

            initFinished = true;
        }

        /// <summary>
        /// ���ƿռ�����
        /// </summary>
        private void PaintSpaceData(Graphics g)
        {
            Point location = Point.Empty;
            Size elementSize = Size.Empty;
            Rectangle rectangle = Rectangle.Empty;

            foreach(DataRow row in table.Rows)
            {
                string bindType = row["BindType"].ToString();
                int startIndex = int.Parse(row["StartIndex"].ToString());
                int length = int.Parse(row["Length"].ToString());

                switch(bindType)
                {
                    case "����":
                        {
                            string bindObject = row["BindObject"].ToString();                            
                            Point point = (Point)varTable[bindObject];
                            location.X = 10 + maxStringLength + startIndex * 5;
                            location.Y = point.Y;
                            elementSize.Width = length * 5;
                            elementSize.Height = 30;
                            rectangle.Location = location;
                            rectangle.Size = elementSize;
                            g.FillRectangle(varBrush, rectangle);

                            break;
                        }
                    case "���":
                        {
                            Point point = (Point)varTable["��ұ���"];
                            location.X = 10 + maxStringLength + startIndex * 5;
                            location.Y = point.Y;
                            elementSize.Width = length * 5;
                            elementSize.Height = 30;
                            rectangle.Location = location;
                            rectangle.Size = elementSize;
                            g.FillRectangle(varBrush, rectangle);

                            break;
                        }
                }
            }
        }

        /// <summary>
        /// ���ƻ�ͼ��
        /// </summary>
        /// <param name="e">�����¼�����</param>
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            // ��ȡ��ͼ����
            Graphics g = e.Graphics;

            // ���û�ͼ����
            g.SmoothingMode = SmoothingMode.HighQuality;
            g.PixelOffsetMode = PixelOffsetMode.HighSpeed;
            g.TranslateTransform(this.AutoScrollPosition.X, this.AutoScrollPosition.Y);   
            
            if(initFinished) // ���Ʊ����ռ�����
            {
                PaintSpaceData(g);
            }
        }

        /// <summary>
        /// ���Ʊ���
        /// </summary>
        /// <param name="e">�����¼�����</param>
        protected override void OnPaintBackground(PaintEventArgs e)
        {
            base.OnPaintBackground(e);

            // ��ȡ��ͼ����
            Graphics g = e.Graphics;

            // ���û�ͼ����
            g.SmoothingMode = SmoothingMode.HighQuality;
            g.PixelOffsetMode = PixelOffsetMode.HighSpeed;
            g.TranslateTransform(this.AutoScrollPosition.X, this.AutoScrollPosition.Y);
            
            if(initFinished)
            {
                g.Clear(Color.White);
                foreach(object[] args in dataList)
                {
                    string varName = args[0] as string;
                    Point point = (Point)args[1];
                    Size size = (Size)args[2];
                    Rectangle rectangle = new Rectangle(new Point(10 + maxStringLength, point.Y), size);
                    g.DrawString(varName, textFont, textBrush, 5, point.Y);
                    g.FillRectangle(backGroundBrush, rectangle);                    
                }
            }
        }
    }
}
