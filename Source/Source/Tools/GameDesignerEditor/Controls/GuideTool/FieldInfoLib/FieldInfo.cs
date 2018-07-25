using System;
using System.Collections.Generic;
using System.Text;

namespace FieldInfoLib
{
    [Serializable]
    public class FieldInfo
    {        
        private readonly string fieldID; // ���ݿ�ID        
        private string fieldName; // ����        
        private string fieldType; // ����        
        private string typeData; // ��������        
        private string defaultValue; // Ĭ��ֵ        
        private string fieldValue; // ֵ        
        private bool allowNullValue; // �Ƿ������ֵ                

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="fieldID">���ݿ�ID</param>
        /// <param name="fieldName">����</param>
        /// <param name="fieldType">����</param>
        /// <param name="typeData">��������</param>
        /// <param name="defaultValue">Ĭ��ֵ</param>
        /// <param name="allowNullValue">�Ƿ������ֵ</param>
        public FieldInfo(string fieldID, string fieldName, string fieldType, string typeData, string defaultValue, bool allowNullValue)
        {
            this.fieldID = fieldID;
            this.fieldName = fieldName;
            this.fieldType = fieldType;
            this.typeData = typeData;
            this.defaultValue = defaultValue;
            this.allowNullValue = allowNullValue;
        }

        /// <summary>
        /// ���ݿ�ID
        /// </summary>
        public string FieldID
        {
            get
            {
                return fieldID;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string FieldName
        {
            get
            {
                return fieldName;
            }
        }

        /// <summary>
        /// ����
        /// </summary>
        public string FieldType
        {
            get
            {
                return fieldType;
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        public string TypeData
        {
            get
            {
                return typeData;
            }
        }

        /// <summary>
        /// ֵ
        /// </summary>
        public string FieldValue
        {
            set
            {
                fieldValue = value;
            }

            get
            {
                return fieldValue;
            }
        }

        /// <summary>
        /// �Ƿ������ֵ
        /// </summary>
        public bool AllowNullValue
        {
            get
            {
                return allowNullValue;
            }
        }
    }
}
