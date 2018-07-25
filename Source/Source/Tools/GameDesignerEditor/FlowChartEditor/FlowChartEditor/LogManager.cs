using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Net;

using DBManager;

namespace FlowChartEditor
{
    class LogManager
    {
        private static LogManager logManager; // ��־����������
        private LogForm logForm; // ��־�鿴����
        private string logTableName = "DiagramLog"; // ��־���ݱ� 


        /// <summary>
        /// ˽�й��캯��
        /// </summary>
        private LogManager()
        {

        }

        /// <summary>
        /// ��ȡ��־����������
        /// </summary>
        /// <returns>��־����������</returns>
        public static LogManager GetLogManager()
        {
            if (logManager == null)
            {
                logManager = new LogManager();
            }

            return logManager;
        }

        /// <summary>
        /// ������־
        /// </summary>
        /// <param name="operation">��ǰ����</param>
        /// <param name="description">��������</param>
        /// <param name="success">�Ƿ�����ɹ�</param>
        public void SaveLog(string operation, string description, bool success)
        {
            string hostName = Dns.GetHostName();
            string strSuccess = "0";

            if (success)
            {
                strSuccess = "1";
            }

            string sqlString = string.Format("INSERT INTO {0} ([HostName], [Operation], [OperateTime], [Success], [Description]) VALUES ('{1}', '{2}', GetDate(), {3}, '{4}')",
                                             logTableName, hostName, operation, strSuccess, description);
            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            dataBaseManager.ExecuteSqlCommand(sqlString);          
        }

        /// <summary>
        /// �鿴��־
        /// </summary>
        public void ViewLog()
        {
            if (logForm == null)
            {
                logForm = new LogForm();
            }

            logForm.Reload();
            logForm.ShowDialog();
        }
    }
}
