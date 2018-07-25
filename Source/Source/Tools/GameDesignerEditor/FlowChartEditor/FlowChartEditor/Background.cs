using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace FlowChartEditor
{
    [Serializable]
    public class Background
    {
        private Color backgroundColor = Color.White; // ������ɫ
        private Size gridSize = new Size(25, 25); // ��Ǹ��С
        private Size canvasSize = new Size(1200, 1600); // ������С                  
        private Size baseSize = new Size(1200, 1600); // ��ͼ��Ļ�����С
        private Size minSize = new Size(1200, 1600); // ��ͼ�����С��С        

        [NonSerialized]
        private bool debugMode = false; // �Ƿ��ڵ���ģʽ                

        /// <summary>
        /// �Ƿ��ڵ���ģʽ
        /// </summary>
        [Browsable(false)]
        public bool DebugMode
        {
            set
            {
                debugMode = value;
            }

            get
            {
                return debugMode;
            }
        }

        /// <summary>
        /// ��ͼ�����С��С
        /// </summary>
        [Browsable(false)]
        public Size MinSize
        {
            set
            {
                minSize = value;
            }

            get
            {
                return minSize;
            }
        }
       
        /// <summary>
        /// ��ͼ��Ļ�����С
        /// </summary>
        [Browsable(false)]
        public Size BaseSize
        {
            set
            {
                baseSize = value;
            }

            get
            {
                return baseSize;
            }
        }        

        /// <summary>
        /// ������ɫ����
        /// </summary>
        [Browsable(false)]
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
        /// ��Ǹ��С����
        /// </summary>
        [Browsable(false)]
        public Size GridSize
        {
            set
            {
                gridSize = value;
            }

            get
            {
                return gridSize;
            }
        }

        /// <summary>
        /// ������С����
        /// </summary>
        [Browsable(false)]
        public Size CanvasSize
        {
            set
            {
                canvasSize = value;
            }

            get
            {
                return canvasSize;
            }
        }

        /// <summary>
        /// ˢ������
        /// </summary>
        public void Refresh()
        {
            minSize = new Size(1200, 1600);
        }
    }
}
