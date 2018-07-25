using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using FieldInfoLib;

namespace GuideTool
{
    class FieldControl
    {
        private Control controlInstance;
        private FieldInfo fieldInfoInstance;

        /// <summary>
        /// ��װ��ѡ����
        /// </summary>
        /// <param name="controlInstance">�ؼ�ʵ��</param>
        /// <param name="fieldInfoInstance">ѡ��������ʵ��</param>
        public FieldControl(Control controlInstance, FieldInfo fieldInfoInstance)
        {
            this.controlInstance = controlInstance;
            this.fieldInfoInstance = fieldInfoInstance;
        }

        /// <summary>
        /// �ؼ��Ƿ���Ч
        /// </summary>
        public bool Enable
        {
            set
            {
                controlInstance.Enabled = value;
            }

            get
            {
                return controlInstance.Enabled;
            }
        }

        /// <summary>
        /// ѡ��ֵ
        /// </summary>
        public string Value
        {
            set
            {
                controlInstance.Text = value;
            }

            get
            {
                return controlInstance.Text;
            }
        }    
        
        /// <summary>
        /// ѡ������
        /// </summary>
        public string FieldType
        {
            get
            {
                return fieldInfoInstance.FieldType;
            }
        }

        /// <summary>
        /// ѡ������
        /// </summary>
        public string Name
        {
            get
            {
                return fieldInfoInstance.FieldName;                
            }
        }        
    }
}
