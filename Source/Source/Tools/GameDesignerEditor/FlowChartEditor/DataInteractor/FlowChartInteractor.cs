using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using DataElementLibrary;

namespace DataInteractor
{
    [Serializable]
    public class FlowChartInteractor
    {
        private string customText; // �Զ����ı�
        private Hashtable customTable = new Hashtable(); // �Զ����ϣ��

        /// <summary>
        /// ���캯��
        /// </summary>
        public FlowChartInteractor()
        {

        }

        /// <summary>
        /// �Զ����ı�
        /// </summary>
        public string CustomText
        {
            set
            {
                customText = value;
            }

            get
            {
                return customText;
            }
        }

        /// <summary>
        /// �Զ����ϣ��
        /// </summary>
        public Hashtable CustomTable
        {
            get
            {
                if(customTable == null)
                {
                    customTable = new Hashtable();
                }

                return customTable;
            }
        }

        /// <summary>
        /// �������ݲ���
        /// </summary>
        /// <param name="args">���ݲ�����ϣ��</param>
        public void SetArguments(Hashtable args)
        {
            customText = args["customText"] as string;
            customTable = args["customTable"] as Hashtable;
        }

        /// <summary>
        /// ��ȡ���ݲ���
        /// </summary>
        /// <returns></returns>
        public Hashtable GetArguments()
        {
            Hashtable args = new Hashtable();
            args["customText"] = customText;
            args["customTable"] = customTable;

            return args;
        }
    }
}
