using System;
using System.Collections.Generic;
using System.Text;

namespace DataInteractor
{
    [Serializable]
    public class Interactor
    {
        private static Interactor interactor; // ���ݽ���������ʵ��
        private object customData; // �Զ�������
        private FlowChartInteractor flowChartInteractor; // ����ͼ���ݽ���������ʵ��
        private string rootDir; // Ӧ�ó����Ŀ¼

        /// <summary>
        /// ����ģʽ���캯��
        /// </summary>
        private Interactor()
        {

        }

        /// <summary>
        /// Ӧ�ó����Ŀ¼
        /// </summary>
        public string RootDir
        {
            set
            {
                rootDir = value;
            }

            get
            {
                return rootDir;
            }
        }

        /// <summary>
        /// �Զ�������
        /// </summary>
        public object CustomData
        {
            set
            {
                customData = value;
            }

            get
            {
                return customData;
            }
        }

        /// <summary>
        /// ����ͼ���ݽ���������ʵ��
        /// </summary>
        public FlowChartInteractor CurrentFlowChartInteractor
        {
            set
            {
                flowChartInteractor = value;
            }

            get
            {
                return flowChartInteractor;
            }
        }

        /// <summary>
        /// ��ȡ���ݽ���������ʵ��
        /// </summary>
        /// <returns>���ݽ���������ʵ��</returns>
        public static Interactor GetInteractor()
        {
            if(interactor == null)
            {
                interactor = new Interactor(); 
            }

            return interactor;
        }


    }
}
