using System;
using System.Collections.Generic;
using System.Text;

using FlowChartDataLibrary;

namespace CodeCompiler
{
    public abstract class Compiler
    {
        public delegate void PrintDebugInfo(string text); // ���������Ϣ
        protected PrintDebugInfo printDebugInfo; // ���������Ϣ�Ļص�ʵ��

        public delegate void SaveFile(string fileName, string content); // ���ı����浽�ļ���
        protected SaveFile saveFile; // ���ı����浽�ļ��еĻص�ʵ��

        protected string rootDirectory; // ��Ŀ¼
        protected string scriptPath; // �ű�·��

        /// <summary>
        /// ��Ŀ¼
        /// </summary>
        public string RootDirectory
        {
            set
            {
                rootDirectory = value;
            }

            get
            {
                return rootDirectory;
            }
        }

        /// <summary>
        /// �ű�·��
        /// </summary>
        public string ScriptPath
        {
            set
            {
                scriptPath = value;
            }

            get
            {
                return scriptPath;
            }
        }

        /// <summary>
        /// ���������Ϣ�Ļص�
        /// </summary>
        public virtual PrintDebugInfo PrintDebugInformation
        {
            set
            {
                printDebugInfo = value;
            }

            get
            {
                return printDebugInfo;
            }
        }

        /// <summary>
        /// ���ı����浽�ļ���
        /// </summary>
        public virtual SaveFile SaveTheFile
        {
            set
            {
                saveFile = value;
            }

            get
            {
                return saveFile;
            }
        }

        /// <summary>
        /// ���ɽű�
        /// </summary>
        /// <param name="chartID">��ͼID</param>
        /// <param name="data">��ͼ����</param>
        /// <param name="manager">��ͼ���ݹ�����</param>
        /// <returns>�Ƿ����ɳɹ�</returns>
        public abstract bool GenerateCode(int chartID, List<FlowChartMetaData> data, FlowChartMetaDataManager manager);
    }
}
