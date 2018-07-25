using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

using GraphElementLibrary;

using FlowChartEditor.BaseGraphElement;

namespace FlowChartEditor
{
    class RegionManager
    {
        private List<RegionData> regionDataList = new List<RegionData>(); // �������������
        private RegionData backupRegionData; // ��Ե��������
        private bool[,] canvasGridBoard; // ��ͼ���������� 
        private Size canvasSize; // ��ͼ���С
        private int gridWidth; // ������
        private int gridBoardWidth; // �������
        private int gridBoardHeight; // �����߶�

        // ��������
        class RegionData
        {
            private Rectangle regionRectangle = Rectangle.Empty;
            private List<ConnectorContainer> connectorContainers = new List<ConnectorContainer>();
            private List<SlotContainer> slotContainers = new List<SlotContainer>();

            /// <summary>
            /// ���캯��
            /// </summary>
            /// <param name="regionRectangle">�������ڵľ���</param>
            public RegionData(Rectangle regionRectangle)
            {
                this.regionRectangle = regionRectangle;
            }

            /// <summary>
            /// ���������е�������
            /// </summary>
            public List<ConnectorContainer> ConnectorContainers
            {
                get
                {
                    return connectorContainers;
                }
            }

            /// <summary>
            /// ���������еĲ������
            /// </summary>
            public List<SlotContainer> SlotContainers
            {
                get
                {
                    return slotContainers;
                }
            }

            /// <summary>
            /// ɾ�������еĲ������
            /// </summary>
            /// <param name="slotContainer">��ǰ�������</param>
            public void Remove(SlotContainer slotContainer)
            {
                slotContainers.Remove(slotContainer);
            }

            /// <summary>
            /// ɾ�������е�������
            /// </summary>
            /// <param name="line">��ǰ������</param>
            public void Remove(ConnectorContainer line)
            {
                connectorContainers.Remove(line);
            }

            /// <summary>
            /// ����������Ƿ������ǰ�������
            /// </summary>
            /// <param name="slotContainer">��ǰ�������</param>
            /// <returns>�����Ƿ������ǰ�������</returns>
            public bool Contains(SlotContainer slotContainer)
            {
                bool result = slotContainers.Contains(slotContainer);

                return result;
            }

            /// <summary>
            /// ����������Ƿ������ǰ������
            /// </summary>
            /// <param name="line">��ǰ������</param>
            /// <returns>�������Ƿ������ǰ������</returns>
            public bool Contains(ConnectorContainer line)
            {
                bool result = connectorContainers.Contains(line);

                return result;
            }

            /// <summary>
            /// �����Ƿ���������
            /// </summary>
            /// <param name="p">��ǰ��</param>
            /// <returns>���Ƿ���������</returns>
            public bool IsInRegion(Point p)
            {
                bool result = regionRectangle.Contains(p);

                return result;
            }

            /// <summary>
            /// �������Ƿ��ں������н���
            /// </summary>
            /// <param name="rectangle">��ǰ����</param>
            /// <returns>�����Ƿ�������н���</returns>
            public bool IsInRegion(Rectangle rectangle)
            {
                bool result = regionRectangle.Contains(rectangle) || regionRectangle.IntersectsWith(rectangle);

                return result;
            }

            /// <summary>
            /// ��Ӳ������������
            /// </summary>
            /// <param name="slotContainer">�������</param>
            public void Add(SlotContainer slotContainer)
            {
                if (!slotContainers.Contains(slotContainer))
                {
                    slotContainers.Add(slotContainer);
                }
            }

            /// <summary>
            /// ��������ߵ�����
            /// </summary>
            /// <param name="line">��������ߵ�����</param>
            public void Add(ConnectorContainer line)
            {
                if (!connectorContainers.Contains(line))
                {
                    connectorContainers.Add(line);
                }
            }
        }

        /// <summary>
        /// ��ͼ������
        /// </summary>
        public bool[,] CanvasGridBoard
        {
            get
            {
                return canvasGridBoard;
            }
        }

        /// <summary>
        /// ��ʼ�������ǰ�
        /// </summary>
        public void InitCanvasGridBoard(List<SlotContainer> slotContainerList, Size canvasSize, int gridWidth)
        {
            this.canvasSize = canvasSize;
            this.gridWidth = gridWidth;

            gridBoardWidth = (int)Math.Ceiling((float)canvasSize.Width / gridWidth);
            gridBoardHeight = (int)Math.Ceiling((float)canvasSize.Height / gridWidth);
            canvasGridBoard = new bool[gridBoardWidth, gridBoardHeight];

            foreach (SlotContainer slotContainer in slotContainerList)
            {
                Rectangle rectangle = slotContainer.InvalidRectangle;
                List<Point> gridPointList = GetGridPointList(slotContainer, gridWidth);

                foreach (Point point in gridPointList)
                {
                    int gridX = (int)(point.X / gridWidth);
                    int gridY = (int)(point.Y / gridWidth);

                    if (gridX >= 0 && gridX < gridBoardWidth && point.Y >= 0 && point.Y < gridBoardHeight)
                    {
                        canvasGridBoard[gridX, gridY] = true;
                    }
                }
            }
        }

        /// <summary>
        /// ���²��������Ӧ������
        /// </summary>
        /// <param name="slotContainer">�������</param>
        private void UpdateSlotContainerGrid(SlotContainer slotContainer)
        {
            Rectangle rectangle = slotContainer.InvalidRectangle;
            List<Point> gridPointList = GetGridPointList(slotContainer, gridWidth);

            foreach (Point point in gridPointList)
            {
                int gridX = (int)(point.X / gridWidth);
                int gridY = (int)(point.Y / gridWidth);

                if (gridX >= 0 && gridX < gridBoardWidth && point.Y >= 0 && point.Y < gridBoardHeight)
                {
                    canvasGridBoard[gridX, gridY] = true;
                }
            }
        }

        /// <summary>
        /// ������������Ӧ������
        /// </summary>
        /// <param name="slotContainer">�������</param>
        private void ClearSlotContainerGrid(SlotContainer slotContainer)
        {
            Rectangle rectangle = slotContainer.InvalidRectangle;
            List<Point> gridPointList = GetGridPointList(slotContainer, gridWidth);

            foreach (Point point in gridPointList)
            {
                int gridX = (int)(point.X / gridWidth);
                int gridY = (int)(point.Y / gridWidth);

                if (gridX >= 0 && gridX < gridBoardWidth && point.Y >= 0 && point.Y < gridBoardHeight)
                {
                    canvasGridBoard[gridX, gridY] = false;
                }
            }
        }

        /// <summary>
        /// ��ȡ����������������ļ���
        /// </summary>
        /// <param name="slotContainer">�������</param>
        /// <param name="gridWidth">������</param>
        /// <returns>����������������ļ���</returns>
        private List<Point> GetGridPointList(SlotContainer slotContainer, int gridWidth)
        {
            List<Point> list = new List<Point>();
            Rectangle rectangle = slotContainer.TextRectangle;
            int leftX = (int)Math.Ceiling((float)rectangle.Left / gridWidth) * gridWidth;
            int rightX = (int)Math.Floor((float)rectangle.Right / gridWidth) * gridWidth;
            int topY = (int)Math.Ceiling((float)rectangle.Top / gridWidth) * gridWidth;
            int bottomY = (int)Math.Floor((float)rectangle.Bottom / gridWidth) * gridWidth;
            Point point = Point.Empty;

            for (int i = leftX; i <= rightX; i += gridWidth)
            {
                for (int j = topY; j <= bottomY; j += gridWidth)
                {
                    point.X = i;
                    point.Y = j;
                    list.Add(point);
                }
            }

            return list;
        }

        /// <summary>
        /// ��ʼ�����������
        /// </summary>
        /// <param name="slotContainerList">�����������</param>
        /// <param name="connectorContainerList">����������</param>
        /// <param name="canvasSize">��ͼ��Ĵ�С</param>
        public void Init(List<SlotContainer> slotContainerList, List<ConnectorContainer> connectorContainerList, Size canvasSize, int gridWidth)
        {
            regionDataList.Clear();

            for (int i = 0; i < canvasSize.Width; i += 600)
            {
                for (int j = 0; j < canvasSize.Height; j += 600)
                {
                    Rectangle regionRectangle = Rectangle.Empty;
                    regionRectangle.Location = new Point(i, j);
                    int regionWidth = 600;
                    int regionHeight = 600;

                    if (i + 600 > canvasSize.Width)
                    {
                        regionWidth = canvasSize.Width - i;
                    }

                    if (j + 600 > canvasSize.Height)
                    {
                        regionHeight = canvasSize.Height - j;
                    }

                    regionRectangle.Size = new Size(regionWidth, regionHeight);

                    RegionData regionData = new RegionData(regionRectangle);
                    regionDataList.Add(regionData);
                }
            }

            foreach (SlotContainer slotContainer in slotContainerList)
            {
                foreach (RegionData regionData in regionDataList)
                {
                    if (regionData.IsInRegion(slotContainer.InvalidRectangle))
                    {
                        regionData.Add(slotContainer);
                    }
                }
            }

            foreach (ConnectorContainer line in connectorContainerList)
            {
                foreach (RegionData regionData in regionDataList)
                {
                    if (regionData.IsInRegion(line.InvalidRectangle))
                    {
                        regionData.Add(line);
                    }
                }
            }

            InitCanvasGridBoard(slotContainerList, canvasSize, gridWidth);

            backupRegionData = new RegionData(Rectangle.Empty);
        }

        /// <summary>
        /// ��ͼԪ���뵽������
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        public void AddToRegion(GraphElement graphElement)
        {
            if (graphElement is ConnectorContainer)
            {
                AddToRegion(graphElement as ConnectorContainer);
            }
            else if (graphElement is SlotContainer)
            {
                AddToRegion(graphElement as SlotContainer);
            }
        }

        /// <summary>
        /// �������������������
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        private void AddToRegion(SlotContainer slotContainer)
        {
            bool added = false;

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.IsInRegion(slotContainer.InvalidRectangle))
                {
                    regionData.Add(slotContainer);
                    added = true;
                }
            }

            if (!added)
            {
                backupRegionData.Add(slotContainer);
            }

            // ������������
            UpdateSlotContainerGrid(slotContainer);
        }

        /// <summary>
        /// �������߼���������
        /// </summary>
        /// <param name="line">��ǰ������</param>
        private void AddToRegion(ConnectorContainer line)
        {
            bool added = false;

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.IsInRegion(line.InvalidRectangle))
                {
                    regionData.Add(line);
                    added = true;
                }
            }

            if (!added)
            {
                backupRegionData.Add(line);
            }
        }

        /// <summary>
        /// ��ͼԪ��������ɾ��
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        public void DeleteFromRegion(GraphElement graphElement)
        {
            if (graphElement is ConnectorContainer)
            {
                DeleteFromRegion(graphElement as ConnectorContainer);
            }
            else if (graphElement is SlotContainer)
            {
                DeleteFromRegion(graphElement as SlotContainer);
            }
        }

        /// <summary>
        /// �����������������ɾ��
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        private void DeleteFromRegion(SlotContainer slotContainer)
        {
            foreach (RegionData regionData in regionDataList)
            {
                regionData.Remove(slotContainer);
            }

            backupRegionData.Remove(slotContainer);

            // ������������
            ClearSlotContainerGrid(slotContainer);
        }

        /// <summary>
        /// �������ߴ�������ɾ��
        /// </summary>
        /// <param name="line">��ǰ������</param>
        private void DeleteFromRegion(ConnectorContainer line)
        {
            foreach (RegionData regionData in regionDataList)
            {
                regionData.Remove(line);
            }

            backupRegionData.Remove(line);
        }

        /// <summary>
        /// ����ͼԪ���ڵ�����
        /// </summary>
        /// <param name="graphElement">��ǰͼԪ</param>
        public void ChangeRegion(GraphElement graphElement)
        {
            if (graphElement is ConnectorContainer)
            {
                ChangeRegion(graphElement as ConnectorContainer);
            }
            else if (graphElement is SlotContainer)
            {
                ChangeRegion(graphElement as SlotContainer);
            }
        }

        /// <summary>
        /// ���²���������ڵ�����
        /// </summary>
        /// <param name="slotContainer">��ǰ�������</param>
        private void ChangeRegion(SlotContainer slotContainer)
        {
            bool added = false; // �Ƿ��Ѿ���ӵ�������

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.Contains(slotContainer) && !regionData.IsInRegion(slotContainer.InvalidRectangle))
                {
                    regionData.Remove(slotContainer);
                }

                if (!regionData.Contains(slotContainer) && regionData.IsInRegion(slotContainer.InvalidRectangle))
                {
                    regionData.Add(slotContainer);
                    added = true;
                }
            }

            if (!added)
            {
                backupRegionData.Add(slotContainer);
            }

            // ����������������ڵ�����            
            foreach (ConnectorContainer line in slotContainer.GetConnectedLine())
            {
                ChangeRegion(line);
            }
        }

        /// <summary>
        /// �������������ڵ�����
        /// </summary>
        /// <param name="line">��ǰ������</param>
        private void ChangeRegion(ConnectorContainer line)
        {
            bool added = false; // �Ƿ��Ѿ���ӵ�������

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.Contains(line) && !regionData.IsInRegion(line.InvalidRectangle))
                {
                    regionData.Remove(line);
                }

                if (!regionData.Contains(line) && regionData.IsInRegion(line.InvalidRectangle))
                {
                    regionData.Add(line);
                    added = true;
                }
            }

            if (!added)
            {
                backupRegionData.Add(line);
            }
        }

        /// <summary>
        /// ��ȡ�����������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�����������</returns>
        public List<SlotContainer> GetSlotContainerList(Point p)
        {
            List<SlotContainer> list = new List<SlotContainer>();

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.IsInRegion(p))
                {
                    foreach (SlotContainer slotContainer in regionData.SlotContainers)
                    {
                        if (slotContainer.IsInRegion(p) && !list.Contains(slotContainer))
                        {
                            list.Add(slotContainer);
                        }
                    }
                }
            }

            if (list.Count == 0)
            {
                list.AddRange(backupRegionData.SlotContainers);
            }

            return list;
        }

        /// <summary>
        /// ��ȡ�����������
        /// </summary>
        /// <param name="rectangle">��ǰ����</param>
        /// <returns>�����������</returns>
        public List<SlotContainer> GetSlotContainerList(Rectangle rectangle)
        {
            List<SlotContainer> list = new List<SlotContainer>();

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.IsInRegion(rectangle))
                {
                    foreach (SlotContainer slotContainer in regionData.SlotContainers)
                    {
                        Rectangle invalidRectangle = slotContainer.InvalidRectangle;

                        if ((rectangle.IntersectsWith(invalidRectangle) || rectangle.Contains(invalidRectangle) || invalidRectangle.Contains(rectangle))
                            && !list.Contains(slotContainer))
                        {
                            list.Add(slotContainer);
                        }
                    }
                }
            }

            if (list.Count == 0)
            {
                list.AddRange(backupRegionData.SlotContainers);
            }

            return list;
        }

        /// <summary>
        /// ��ȡ�����������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>�����������</returns>
        public List<SlotContainer> GetSlotContainers(Point p)
        {
            List<SlotContainer> list = new List<SlotContainer>();

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.IsInRegion(p))
                {
                    foreach (SlotContainer slotContainer in regionData.SlotContainers)
                    {
                        if (!list.Contains(slotContainer))
                        {
                            list.Add(slotContainer);
                        }
                    }
                }
            }

            if (list.Count == 0)
            {
                list.AddRange(backupRegionData.SlotContainers);
            }

            return list;
        }

        /// <summary>
        /// ��ȡ�����������
        /// </summary>
        /// <param name="rectangle">��ǰ����</param>
        /// <returns>�����������</returns>
        public List<SlotContainer> GetSlotContainers(Rectangle rectangle)
        {
            List<SlotContainer> list = new List<SlotContainer>();

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.IsInRegion(rectangle))
                {
                    foreach (SlotContainer slotContainer in regionData.SlotContainers)
                    {
                        if (!list.Contains(slotContainer))
                        {
                            list.Add(slotContainer);
                        }
                    }
                }
            }

            if (list.Count == 0)
            {
                list.AddRange(backupRegionData.SlotContainers);
            }

            return list;
        }

        /// <summary>
        /// ��ȡ����������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>����������</returns>
        public List<ConnectorContainer> GetConnectorContainerList(Point p)
        {
            List<ConnectorContainer> list = new List<ConnectorContainer>();

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.IsInRegion(p))
                {
                    foreach (ConnectorContainer line in regionData.ConnectorContainers)
                    {
                        if (line.IsInRegion(p) && !list.Contains(line))
                        {
                            list.Add(line);
                        }
                    }
                }
            }

            if (list.Count == 0)
            {
                list.AddRange(backupRegionData.ConnectorContainers);
            }

            return list;
        }

        /// <summary>
        /// ��ȡ����������
        /// </summary>
        /// <param name="rectangle">��ǰ����</param>
        /// <returns>����������</returns>
        public List<ConnectorContainer> GetConnectorContainerList(Rectangle rectangle)
        {
            List<ConnectorContainer> list = new List<ConnectorContainer>();

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.IsInRegion(rectangle))
                {
                    foreach (ConnectorContainer line in regionData.ConnectorContainers)
                    {
                        Rectangle invalidRectangle = line.InvalidRectangle;

                        if ((rectangle.IntersectsWith(invalidRectangle) || rectangle.Contains(invalidRectangle) || invalidRectangle.Contains(rectangle))
                            && !list.Contains(line))
                        {
                            list.Add(line);
                        }
                    }
                }
            }

            if (list.Count == 0)
            {
                list.AddRange(backupRegionData.ConnectorContainers);
            }

            return list;
        }

        /// <summary>
        /// ��ȡ����������
        /// </summary>
        /// <param name="p">��ǰ��</param>
        /// <returns>����������</returns>
        public List<ConnectorContainer> GetAllConnectorContainers(Point p)
        {
            List<ConnectorContainer> list = new List<ConnectorContainer>();

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.IsInRegion(p))
                {
                    foreach (ConnectorContainer line in regionData.ConnectorContainers)
                    {
                        if (!list.Contains(line))
                        {
                            list.Add(line);
                        }
                    }
                }
            }

            if (list.Count == 0)
            {
                list.AddRange(backupRegionData.ConnectorContainers);
            }

            return list;
        }

        /// <summary>
        /// ��ȡ����������
        /// </summary>
        /// <param name="rectangle">��ǰ����</param>
        /// <returns>����������</returns>
        public List<ConnectorContainer> GetConnectorContainers(Rectangle rectangle)
        {
            List<ConnectorContainer> list = new List<ConnectorContainer>();

            foreach (RegionData regionData in regionDataList)
            {
                if (regionData.IsInRegion(rectangle))
                {
                    foreach (ConnectorContainer line in regionData.ConnectorContainers)
                    {
                        if (!list.Contains(line))
                        {
                            list.Add(line);
                        }
                    }
                }
            }

            if (list.Count == 0)
            {
                list.AddRange(backupRegionData.ConnectorContainers);
            }

            return list;
        }
    }
}
