using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.IO;

namespace AddNpcActionNode
{
    public class Program
    {
        private ConfigForm cForm; // ��ʾ�Ĵ���
        private MultiAddForm mForm; // ��ѡ�Ĵ���
        private string fileName; // �ļ���

        /// <summary>
        /// ģ����������
        /// </summary>
        public string ModelName
        {
            get
            {
                return cForm.ModelName;
            }
        }

        /// <summary>
        /// ����id����
        /// </summary>
        public String AnimationID
        {
            get
            {
                return cForm.AnimationID;
            }
        }

//         /// <summary>
//         /// ���ȼ�����
//         /// </summary>
//         public String Priority
//         {
//             get
//             {
//                 return cForm.Priority;
//             }
//         }

        /// <summary>
        /// ������������
        /// </summary>
        public String KindID
        {
            get
            {
                return cForm.KindID;
            }
        }

        /// <summary>
        /// ���������������
        /// </summary>
        public string[] ActionIndexArray
        {
            get
            {
                return mForm.ActionIndexArray;
            }
        }

        /// <summary>
        /// ������������
        /// </summary>
        public string[] KindIDArray
        {
            get
            {
                return mForm.KindIDArray;
            }
        }

//         /// <summary>
//         /// ���ȼ�����
//         /// </summary>
//         public string[] PropertyArray
//         {
//             get
//             {
//                 return mForm.PropertyArray;
//             }
//         }

        /// <summary>
        /// �Ƿ�ѭ������
        /// </summary>
        public string[] LoopArray
        {
            get
            {
                return mForm.LoopArray;
            }
        }

        /// <summary>
        /// �ļ�������
        /// </summary>
        public string FileName
        {
            get
            {
                return fileName;
            }
        }

        /// <summary>
        /// �Ƿ�ѭ������
        /// </summary>
        public string Loop
        {
            get
            {
                return cForm.Loop;
            }
        }

        /// <summary>
        /// �ļ�·������
        /// </summary>
        public string FilePath
        {
            get
            {
                return cForm.FilePath;
            }
        }

        /// <summary>
        /// ��ʾ���봰��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="representID">����ID</param>
        /// <returns>�û��Ƿ�ȷ��ѡ��</returns>
        public int Show(SqlConnection conn, string representID)
        {
            if (cForm == null)
                cForm = new ConfigForm(conn);

            cForm.InitFileInfo(representID);

            DialogResult result = cForm.ShowDialog();
            if(result == DialogResult.OK)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        /// <summary>
        /// ��ʾ��ѡ�Ĵ���
        /// </summary>
        /// <returns>�û��Ƿ�ȷ��ѡ��</returns>
        public int ShowMultiAddForm(SqlConnection conn)
        {
            mForm = new MultiAddForm(conn);
            DialogResult result = mForm.ShowDialog();
            if(result == DialogResult.OK)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        /// <summary>
        /// ����ļ��Ƿ����
        /// </summary>
        /// <param name="filePath">�ļ�·��</param>
        /// <param name="fileName">�ļ���</param>
        /// <returns>�ļ��Ƿ����</returns>
        public bool CheckFileExist(string filePath, string fileName)
        {
            fileName = fileName.ToLower();
            bool result = false;

            DirectoryInfo di = new DirectoryInfo(filePath);
            if(di.Exists)
            {
                bool newType = false; // �Ƿ����´洢����

                DirectoryInfo[] folders = di.GetDirectories();
                foreach(DirectoryInfo dInfo in folders)
                {
                    if (dInfo.Name == "����")
                    {
                        newType = true;
                        break;
                    }
                }

                if(newType)
                {
                    di = new DirectoryInfo(Path.Combine(di.FullName, "����"));
                }
                FileInfo[] files = di.GetFiles();
                foreach(FileInfo fi in files)
                {
                    string str1 = fi.Name.ToLower();
                    string str2 = fi.Extension.ToLower();
                    if(str1.Contains(fileName) && (str2 == ".ani" || str2 == ".tani"))
                    {
                        this.fileName = str1;
                        result = true;
                        break;
                    }
                }

                if(newType)
                {
                    this.fileName = "����\\" + this.fileName;
                }
            }

            return result;
        }
    }
}