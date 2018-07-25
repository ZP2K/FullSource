using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartDataLibrary
{
    /// <summary>
    /// ����ͼԪ������
    /// </summary>
    public class FlowChartMetaData
    {
        private readonly int id; // ID
        private readonly object data; // ����
        private readonly string dataType; // ��������
        private string code = ""; // ���ɴ���
        private string displayText = ""; // ��ʾ���ı�
        private List<FlowChartMetaData> nextMetaDataList = new List<FlowChartMetaData>(); // ���������ͼԪ��������
        private List<FlowChartMetaData> previousMetaDataList = new List<FlowChartMetaData>(); // ����������ͼԪ��������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="id">id</param>
        /// <param name="data">����</param>
        /// <param name="dataType">��������</param>
        public FlowChartMetaData(int id, object data, string dataType)
        {
            this.id = id;
            this.data = data;
            this.dataType = dataType;
        }

        /// <summary>
        /// ��ʾ���ı�����
        /// </summary>
        public string DisplayText
        {
            set
            {
                displayText = value;
            }

            get
            {
                return displayText;
            }
        }

        /// <summary>
        /// ���ɴ�������
        /// </summary>
        public string Code
        {
            set
            {
                code = value;
            }

            get
            {
                return code;
            }
        }

        /// <summary>
        /// ID����
        /// </summary>
        public int ID
        {
            get
            {
                return id;
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        public object Data
        {
            get
            {
                return data;
            }
        }

        /// <summary>
        /// ������������
        /// </summary>
        public string DataType
        {
            get
            {
                return dataType;
            }
        }

        /// <summary>
        /// ���������ͼԪ��������
        /// </summary>
        public List<FlowChartMetaData> NextMetaDataList
        {
            get
            {
                return nextMetaDataList;
            }
        }

        /// <summary>
        /// ����������ͼԪ��������
        /// </summary>
        public List<FlowChartMetaData> PreviousMetaDataList
        {
            get
            {                
                return previousMetaDataList;
            }
        }

        /// <summary>
        /// �����������ͼ��Ԫ����
        /// </summary>
        /// <param name="metaData">Ԫ����</param>
        public void AddPreviousMetaData(FlowChartMetaData metaData)
        {
            if(!previousMetaDataList.Contains(metaData))
            {
                previousMetaDataList.Add(metaData);
            }
        }

        /// <summary>
        /// �����������ͼ��Ԫ����
        /// </summary>
        /// <param name="metaData">Ԫ����</param>
        public void AddNextMetaData(FlowChartMetaData metaData)
        {
            if(!nextMetaDataList.Contains(metaData))
            {
                nextMetaDataList.Add(metaData);
            }
        }

        /// <summary>
        /// ������������ͼ��Ԫ��������
        /// </summary>
        public void ClearPreviousMetaDataList()
        {
            previousMetaDataList.Clear();
        }

        /// <summary>
        /// ������������ͼ��Ԫ��������
        /// </summary>
        public void ClearNextMetaDataList()
        {
            nextMetaDataList.Clear();
        }
    }
}
