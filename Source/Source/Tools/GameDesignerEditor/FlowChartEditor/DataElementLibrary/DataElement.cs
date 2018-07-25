using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace DataElementLibrary
{
    [Serializable]
    public abstract class DataElement
    {
        protected string name; // ����Ԫ����
        protected string description; // ����Ԫ������
        protected string dataType; // ����Ԫ������
        protected object data; // ����Ԫ������
        protected string text = ""; // ����Ԫ�ı�
        protected string tooltipText = ""; // ����Ԫ����ʾ�ı�
        protected int id; // ����Ԫ��id

        public delegate void PrintInfo(string text); // ���������Ϣ
        [NonSerialized]
        protected PrintInfo printInfo; // ���������Ϣ�Ļص�ʵ��

        /// <summary>
        /// ����Ԫ����ʾ�ı�
        /// </summary>
        public virtual string TooltipText
        {
            set
            {
                tooltipText = value;
            }

            get
            {
                return tooltipText;
            }
        }

        /// <summary>
        /// ����Ԫ�ı�����
        /// </summary>
        public virtual string Text
        {
            set
            {
                text = value;
            }

            get
            {
                return text;
            }
        }

        /// <summary>
        /// ����Ԫ����������
        /// </summary>
        public virtual string Name
        {
            set
            {
                name = value;
            }

            get
            {
                return name;
            }
        }

        /// <summary>
        /// ����Ԫ����������
        /// </summary>
        public virtual string Description
        {
            set
            {
                description = value;
            }

            get
            {
                return description;
            }
        }

        /// <summary>
        /// ����Ԫ����������
        /// </summary>
        public virtual object Data
        {
            set
            {
                data = value;
            }

            get
            {
                return data;
            }
        }

        /// <summary>
        /// ����Ԫ��id����
        /// </summary>
        public virtual int ID
        {
            set
            {
                id = value;
            }

            get
            {
                return id;
            }
        }

        /// <summary>
        /// ���������Ϣ�Ļص�
        /// </summary>
        public virtual PrintInfo PrintInformation
        {
            set
            {
                printInfo = value;
            }

            get
            {
                return printInfo;
            }
        }

        /// <summary>
        /// ����Ԫ����������
        /// </summary>
        public virtual string DataType
        {
            set
            {
                dataType = value;
            }

            get
            {
                return dataType;
            }
        }

        /// <summary>
        /// �༭����
        /// </summary>
        /// <param name="o">����Ԫ������</param>
        /// <return>�Ƿ�༭�ɹ�</return>
        public abstract bool EditData(Hashtable table);

        /// <summary>
        /// �ض�����
        /// </summary>
        /// <param name="table">��ǰ����</param>
        /// <return>�Ƿ��ض��ɹ�</return>
        public abstract bool ReloadData(Hashtable table);

        /// <summary>
        /// ��ȡ����Ĳ���
        /// </summary>
        /// <returns>����Ĳ���</returns>
        public abstract Hashtable GetOutputArgs();
    }
}
