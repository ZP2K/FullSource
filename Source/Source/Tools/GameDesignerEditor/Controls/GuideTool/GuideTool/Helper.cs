using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Xml;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Windows.Forms;

namespace GuideTool
{
    class Helper
    {
        private static IFormatter formatter = new BinaryFormatter();

        /// <summary>
        /// ����쳣��Ϣ
        /// </summary>
        /// <param name="ex">��ǰ�쳣</param>
        public static void PrintExceptionInfo(Exception ex)
        {
            MessageBox.Show(string.Format("��ǰ���������쳣��{0}", ex.Message, "����",
                            MessageBoxButtons.OK, MessageBoxIcon.Error));
        }

        /// <summary>
        /// ��ȡ�ļ�����
        /// </summary>
        /// <param name="fileName">�ļ�·��</param>
        /// <returns>�ļ�����</returns>
        public static byte[] ReadFile(string fileName)
        {            
            byte[] buffer = null;

            if (File.Exists(fileName))
            {
                try
                {
                    buffer = File.ReadAllBytes(fileName);
                }
                catch (IOException ex)
                {
                    PrintExceptionInfo(ex);
                }
            }

            return buffer;
        }

        /// <summary>
        /// ���ı����浽�ļ�
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="content">�ļ�����</param>
        public static bool SaveFile(string fileName, byte[] content)
        {
            bool success = true;

            try
            {
                FileInfo fi = new FileInfo(fileName);

                if (!fi.Exists) // �ļ�������
                {
                    string folderName = fi.DirectoryName; // ��ȡĿ¼

                    if (!string.IsNullOrEmpty(folderName) && !Directory.Exists(folderName)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(folderName); // �ȴ���Ŀ¼���ٴ����ļ�
                    }

                    fi.Create().Close();
                }

                File.WriteAllBytes(fileName, content);
            }
            catch (IOException ex)
            {                
                success = false;
                PrintExceptionInfo(ex);
            }

            return success;
        }        

        /// <summary>
        /// �����л�����
        /// </summary>
        /// <param name="buffer">��ǰ����</param>
        /// <returns>�����л��������</returns>
        public static object UnseralizeData(byte[] buffer)
        {
            MemoryStream memoryStream = new MemoryStream(buffer);
            object data = formatter.Deserialize(memoryStream);
            memoryStream.Close();

            return data;
        }

        /// <summary>
        /// ���л�����
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <returns>���л��������</returns>
        public static byte[] SeralizeData(object data)
        {
            MemoryStream memoryStream = new MemoryStream();
            formatter.Serialize(memoryStream, data);
            byte[] buffer = new byte[memoryStream.Length];
            memoryStream.Seek(0, SeekOrigin.Begin);
            memoryStream.Read(buffer, 0, (int)memoryStream.Length);
            memoryStream.Close();

            return buffer;
        }

        /// <summary>
        /// ��ȡ������ѡ����Ϣ
        /// </summary>
        /// <param name="infoString">ѡ���ַ�������</param>
        /// <returns>ѡ����Ϣ</returns>
        public static string[] GetListItemInfo(string infoString)
        {
            int index = infoString.LastIndexOf("(");
            string itemName = infoString.Substring(0, index - 1);
            string itemValue = infoString.Substring(index + 1, infoString.Length - index - 2);

            return new string[] { itemName, itemValue };
        }

        /// <summary>
        /// ��ǰֵ�Ƿ��ǿ�ֵ�������ݿ��ֵ
        /// </summary>
        /// <param name="value">��ǰֵ</param>
        /// <returns>�Ƿ��ǿ�ֵ�������ݿ��ֵ</returns>
        public static bool IsNullOrDBNull(object value)
        {
            bool isNullOrDBNull = ((value == null) || (value is DBNull));
            return isNullOrDBNull;
        }

        /// <summary>
        /// ��ȡ���ֵ�����
        /// </summary>
        /// <param name="typeData">��������</param>
        /// <returns>���ֵ�����</returns>
        public static string GetNumberType(string typeData)
        {
            XmlDocument xmlDocument = new XmlDocument();
            xmlDocument.LoadXml(typeData);
            XmlNode typeNode = xmlDocument["typeData"];
            string numberType = typeNode.Attributes["numberType"].Value;

            return numberType;
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <param name="conn">sql����</param>
        /// <returns>���ݱ�</returns>
        public static DataTable GetDataTable(string sqlString, SqlConnection conn)
        {
            DataTable table = null;

            if (conn.State == ConnectionState.Closed)
            {
                conn.Open();
            }

            SqlDataAdapter adapter = new SqlDataAdapter(sqlString, conn);
            adapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet dataSet = new DataSet();
            adapter.Fill(dataSet);
            table = dataSet.Tables[0];

            if (conn.State == ConnectionState.Open)
            {
                conn.Close();
            }

            return table;
        }
    }
}
