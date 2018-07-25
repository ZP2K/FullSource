using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Remoting;

namespace DataTableRemoting
{
    public static class DataTableServer
    {

        public static bool RunServer;
        /// <summary>
        /// ����Զ�̷���
        /// </summary>
        /// <param name="config"></param>
        public static void Run(string config)
        {
            RunServer = true;
            RemotingConfiguration.Configure(config, false);
        }

        /// <summary>
        /// ȡ��Զ������Ϣ
        /// </summary>
        /// <returns></returns>
        public static string[] GetRemotingEntries()
        {
            WellKnownServiceTypeEntry[] entries =
                RemotingConfiguration.GetRegisteredWellKnownServiceTypes();
            string[] types = new string[entries.Length];
            for (int index = 0; index < entries.Length; ++index)
            {
                types[index] = entries[index].TypeName;
            }
            return types;
        }

        public static void ServerLog(string msg)
        {
            LogHelper.LogServerMessage(msg, DataTableRemotingCommon.LogType.Program);
        }

        public static void Release()
        {
            TableHelper.Release();
        }
    }
}
