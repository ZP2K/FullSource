using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

namespace FlowChartDataLibrary
{
    public class FlowChartMetaDataManager
    {
        private int flowChartID; // ����ͼ��ID
        private List<FlowChartMetaData> entranceNodeList; // ��ڽ��Ԫ��������
        private Hashtable eventTable = new Hashtable(); // ��ڽ��Ĺ�ϣ�� 
        private string mapName; // ��������
        private int version; // �汾��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="flowChartID">����ͼ��ID</param>
        /// <param name="entranceNodeList">��ڽ��Ԫ��������</param>
        public FlowChartMetaDataManager(int flowChartID, List<FlowChartMetaData> entranceNodeList)
        {
            this.flowChartID = flowChartID;
            this.entranceNodeList = entranceNodeList;
            Init();
        }        

        /// <summary>
        /// ��������
        /// </summary>
        public string MapName
        {
            set
            {
                mapName = value;
            }

            get
            {
                return mapName;
            }
        }

        /// <summary>
        /// �汾��
        /// </summary>
        public int Version
        {
            set
            {
                version = value;
            }

            get
            {
                return version;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            foreach (FlowChartMetaData eventNode in entranceNodeList)
            {
                List<FlowChartMetaData> list = new List<FlowChartMetaData>();
                foreach (FlowChartMetaData metaData in eventNode.NextMetaDataList)
                {
                    GetNodeNames(metaData, list);
                }
                eventTable[eventNode] = list;
            }
        }

        /// <summary>
        /// ��ȡ������ͼ��һ���ж��ٸ���ڽ��
        /// </summary>
        /// <returns>��ڽ�����</returns>
        public int GetEventCount()
        {
            return entranceNodeList.Count;
        }

        /// <summary>
        /// ��ȡ�¼�����������
        /// </summary>
        /// <param name="i">�¼����������</param>
        /// <returns>�¼�����������</returns>
        public string GetEventFunctionName(int i)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            string functionName = eventNode.DataType + "_" + flowChartID + "_" + eventNode.ID;
            return functionName;
        }

        /// <summary>
        /// �����к���Ԫ���ݼ�������
        /// </summary>
        /// <param name="metaData">��ǰԪ����</param>
        /// <param name="list">����Ԫ���ݵ�����</param>
        private void GetNodeNames(FlowChartMetaData metaData, List<FlowChartMetaData> list)
        {
            if(!list.Contains(metaData))
            {
                if(metaData.DataType != "Line" && metaData.DataType != "AILine")
                {
                    list.Add(metaData);
                }
                
                foreach(FlowChartMetaData d in metaData.NextMetaDataList)
                {
                    GetNodeNames(d, list);
                }
            }
        }

        /// <summary>
        /// ��ȡ�¼������еĽڵ������
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <returns>���н�������</returns>
        public string GetNodeNames(int i)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;

            string nodeNames = "";
            foreach(FlowChartMetaData metaData in list)
            {
                nodeNames += metaData.DataType + "_" + flowChartID + "_" + metaData.ID + ", ";
            }
            nodeNames = nodeNames.TrimEnd(new char[] { ',', ' ' });

            return nodeNames;
        }

        /// <summary>
        /// ��ȡ�¼��Ľڵ�����
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <returns>���н�������</returns>
        public int GetNodeCount(int i)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;

            return list.Count;
        }

        /// <summary>
        /// ��ȡ�¼��Ľ�������
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <param name="j">�������</param>
        /// <returns>��������</returns>
        public string GetNodeName(int i, int j)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;
            FlowChartMetaData metaData = list[j - 1];

            string nodeName = metaData.DataType + "_" + flowChartID + "_" + metaData.ID;
            return nodeName;
        }

        /// <summary>
        /// ��ȡ�¼��ĵĽ�������
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <param name="j">�������</param>
        /// <returns>��������</returns>
        public string GetNodeType(int i, int j)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;
            FlowChartMetaData metaData = list[j - 1];

            return metaData.DataType;
        }

        /// <summary>
        /// ��ȡ�¼��ĵĽڵ�����ɴ���
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <param name="j">�������</param>
        /// <returns>�������ɴ���</returns>
        public string GetNodeCode(int i , int j)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;
            FlowChartMetaData metaData = list[j - 1];            

            return metaData.Code;
        }

        /// <summary>
        /// ��ȡ�¼��Ľڵ�ĺ��������ߵ�����
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <param name="j">�������</param>
        /// <returns>�����������ߵ�����</returns>
        public int GetNextConnetionCount(int i, int j)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            int count;

            if(j == 0) // ֱ��ȡ��ڽ��
            {
                count = eventNode.NextMetaDataList.Count;
            }
            else
            {
                List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;
                FlowChartMetaData metaData = list[j - 1];
                count = metaData.NextMetaDataList.Count;
            }
            
            return count;
        }

        /// <summary>
        /// ��ȡ��ǰ�����һ�����ļ���
        /// </summary>
        /// <param name="metaData">��ǰ���</param>
        /// <param name="list">���漯�ϵ�����</param>
        public void GetNextNode(FlowChartMetaData metaData, List<FlowChartMetaData> list)
        {
            foreach(FlowChartMetaData m in metaData.NextMetaDataList)
            {
                foreach(FlowChartMetaData n in m.NextMetaDataList)
                {
                    list.Add(n);
                }
            }
        }

        /// <summary>
        /// ��ȡ�¼��ĵĽڵ�����������ӵĺ����ڵ������Ƿ�����ڵ�
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <param name="j">�������</param>
        /// <param name="k">�����ߵ����</param>
        /// <returns>�Ƿ��ǽ������</returns>
        public bool IsNextNodeFinish(int i, int j, int k)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;
            FlowChartMetaData metaData = list[j - 1];
            FlowChartMetaData line = metaData.NextMetaDataList[k - 1];
            FlowChartMetaData newMetaData = line.NextMetaDataList[0];

            bool result = newMetaData.DataType == "EndNode";

            return result;
        }

        /// <summary>
        /// ��ȡ�¼��Ľڵ�ĺ����ڵ���
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <param name="j">�������</param>
        /// <param name="k">�����������</param>
        /// <returns>�����</returns>
        public string GetNextNodeName(int i, int j, int k)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            string nodeName;

            if(j == 0) // ֱ��ȡ��ǰ��ڽ��
            {
                List<FlowChartMetaData> list = new List<FlowChartMetaData>();
                GetNextNode(eventNode, list);
                FlowChartMetaData nextMetaData = list[k - 1];

                nodeName = nextMetaData.DataType + "_" + flowChartID + "_" + nextMetaData.ID; 
            }
            else
            {
                List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;
                FlowChartMetaData metaData = list[j - 1];
                list = new List<FlowChartMetaData>();
                GetNextNode(metaData, list);
                FlowChartMetaData nextMetaData = list[k - 1];

                nodeName = nextMetaData.DataType + "_" + flowChartID + "_" + nextMetaData.ID;
            }
            
            return nodeName;
        }

        /// <summary>
        /// ��ȡ�¼��Ľڵ�ĺ���ִ���߳�����
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <param name="j">�������</param>
        /// <returns>ִ���߳�����</returns>
        public int GetNextThreadCount(int i, int j)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;
            FlowChartMetaData metaData = list[j - 1];

            return metaData.NextMetaDataList.Count;
        }

        /// <summary>
        /// ��ȡ�¼��Ľڵ�ķ�ִ֧����������
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <param name="j">�������</param>
        /// <returns>��ִ֧����������</returns>
        public int GetNodeSwitchCount(int i, int j)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;
            FlowChartMetaData metaData = list[j - 1];

            return metaData.NextMetaDataList.Count;
        }

        /// <summary>
        /// ��ȡ�¼��Ľڵ���������ϵ���������
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <param name="j">�������</param>
        /// <param name="k">���������</param>
        /// <returns>��������</returns>
        public string GetConditionResult(int i,int j, int k)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;
            FlowChartMetaData metaData = list[j - 1];
            FlowChartMetaData nextMetaData = metaData.NextMetaDataList[k - 1];

            return nextMetaData.Code;
        }

        /// <summary>
        /// ��ȡ�¼��Ľ�����ʾ�ı�
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <param name="j">�������</param>
        /// <returns>��ʾ�ı�</returns>
        public string GetNodeText(int i, int j)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            List<FlowChartMetaData> list = eventTable[eventNode] as List<FlowChartMetaData>;
            FlowChartMetaData metaData = list[j - 1];
            string text = metaData.DisplayText;
            if(text == null)
            {
                text = "";
            }

            return text;
        }

        /// <summary>
        /// ��ȡ�¼�����ʾ�ı�
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <returns>��ʾ�ı�</returns>
        public string GetEventText(int i)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            string text = eventNode.DisplayText;
            if (text == null)
            {
                text = "";
            }

            return text;
        }

        /// <summary>
        /// ��ȡ�¼�������
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <returns>�¼�������</returns>
        public object GetEventData(int i)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            return eventNode.Data;
        }

        /// <summary>
        /// ��ȡ�¼�Ԫ����
        /// </summary>
        /// <param name="i">�¼������</param>
        /// <returns>�¼�Ԫ����</returns>
        public FlowChartMetaData GetEventNode(int i)
        {
            FlowChartMetaData eventNode = entranceNodeList[i - 1];
            return eventNode;
        }

        /// <summary>
        /// ��ȡ���е�Ԫ��������
        /// </summary>
        /// <returns>���е�Ԫ��������</returns>
        public List<FlowChartMetaData> GetAllNodes()
        {
            List<FlowChartMetaData> list = new List<FlowChartMetaData>();

            foreach(FlowChartMetaData eventNode in entranceNodeList)
            {
                GetNodeNames(eventNode, list);
            }

            return list;
        }
    }
}
