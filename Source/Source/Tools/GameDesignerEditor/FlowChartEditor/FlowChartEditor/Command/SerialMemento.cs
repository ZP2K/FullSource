using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization.Formatters.Soap;

namespace FlowChartEditor.Command
{
    class SerialMemento:Memento
    {
        private static IFormatter formatter = new BinaryFormatter();
        private byte[] m_bindata;

        /// <summary>
        /// ���ض�������
        /// </summary>
        /// <returns>������</returns>
        public override object LoadData()
        {           
            object data = null;

            if (m_bindata == null || m_bindata.Length == 0)
            {
                
            }
            else
            {
                // ��λ
                MemoryStream memoryStream = new MemoryStream(m_bindata);                
                data = formatter.Deserialize(memoryStream);
                memoryStream.Close();
            }
            
            return data;
        }

        /// <summary>
        /// �����������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        public override void SaveData(object o)
        {            
            MemoryStream memoryStream = new MemoryStream();
            formatter.Serialize(memoryStream, o);
            m_bindata = new byte[memoryStream.Length];
            memoryStream.Seek(0, SeekOrigin.Begin);
            memoryStream.Read(m_bindata, 0, (int)memoryStream.Length);
            memoryStream.Close();            
        }
    }
}
