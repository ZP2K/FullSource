using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

namespace FSEye.PlugIn
{
    public class ServerPlugInFactory
    {
        public static int PlugCount = 0;
        public static ArrayList TotalPlugIn = new ArrayList();
        public enum PlugInList:int
        {
            GameServerDb = 0,
            LordControl,
            UpdateNetworkCard,
            UpdateCpu,
            UpdateMemory,
            UpdateProcess,
            UpdatePlugIn,
            UpdateDisk,
            Ping,
            FileManage,
            GameServerControl,
            MessageLog,
            GetFileList
        }

        /// <summary>
        /// ��ʼ�����������,˳���ϸ���PlugInList
        /// </summary>
        public static void Initialize()
        {
            //��ֹ�ظ�����
            if (PlugCount != 0) return;

            TotalPlugIn.Add(new GameServerDb());
            TotalPlugIn.Add(new LordControl());
            TotalPlugIn.Add(new UpdateNetworkCard());
            TotalPlugIn.Add(new UpdateCpu());
            TotalPlugIn.Add(new UpdateMemory());
            TotalPlugIn.Add(new UpdateProcess());
            TotalPlugIn.Add(new UpdatePlugIn());
            TotalPlugIn.Add(new UpdateDisk());
            TotalPlugIn.Add(new Ping());
            TotalPlugIn.Add(new FileManage());
            TotalPlugIn.Add(new GameServerControl());
            TotalPlugIn.Add(new MessageLog());            
            PlugCount = TotalPlugIn.Count;            
        }


        /// <summary>
        /// ͨ�����ݿ��е�server_plugin��������ӦGameServer�Ĳ��
        /// </summary>
        /// <param name="PlugIn_Guid">�����ID����Guid</param>
        /// <param name="parameter1">Ԥ������1,int</param>
        /// <param name="parameter2">Ԥ������2,int</param>
        /// <param name="parameter3">Ԥ������3,string</param>
        /// <returns></returns>
        public static IPlugIn GetPlugIn(string PlugIn_Guid, int parameter1, int parameter2, string parameter3)
        {
            switch (PlugIn_Guid)
            {
                case "9609275e-205e-47a5-9065-58e7c52e3cbf":
                    return new GameServerDb();
                case "3b2635c2-64ed-48c3-8ae8-25eed4ddbc88":
                    return new LordControl(parameter1, parameter2);
                case "895235f4-47d8-4ddb-af7c-fdb3591d437f":
                    return new UpdateNetworkCard(parameter1);
                case "7008f6d4-73ea-419f-8f08-5fc7f81740ea":
                    return new UpdateCpu(parameter1);
                case "63b1a0d7-11cc-4680-9b4a-04740dd4f2ea":
                    return new UpdateMemory(parameter1);                    
                case "7025123c-4eee-4461-a183-a81535903a59":
                    return new UpdateProcess(parameter1);
                case "91e63a4f-9ead-4127-bb5d-19f67622ab9d":
                    return new UpdatePlugIn(parameter1);
                case "c29ed7be-8d7f-4765-83fe-d2a8b581f446":
                    return new UpdateDisk(parameter1);
                case "02959648-ed95-4220-bef7-a8983da81f18":
                    return new Ping(parameter1,parameter2);
                case "f6038d00-c898-4f98-b143-bb6edc51ae17":
                    return new FileManage();                 
                case "a90a90e3-57d3-4711-9e70-30c53c615d70":
                    return new GameServerControl();
                case "ccdc26d6-9ca5-4f3f-84f1-4e5fa0493956":
                    return new MessageLog(parameter3);
                default:
                    return null;
            }
        }

        /// <summary>
        /// �������²�������ݽӿ�
        /// </summary>
        /// <param name="gameServerId"></param>
        /// <param name="plugIn"></param>
        /// <returns></returns>
        public static object[] GetData(int gameServerId,IPlugIn plugIn)
        {
            //Parameter[0]Ϊ��������
            switch (plugIn.Guid)
            {
                case "9609275e-205e-47a5-9065-58e7c52e3cbf":
                    //GameServerDb();
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        0,
                        0,
                        string.Empty,
                        plugIn.Name
                    };
                case "3b2635c2-64ed-48c3-8ae8-25eed4ddbc88":
                    //LordControl 
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        plugIn.Parameters[1],
                        plugIn.Parameters[2],
                        string.Empty,
                        plugIn.Name
                    };
                case "895235f4-47d8-4ddb-af7c-fdb3591d437f":
                    //UpdateNetworkCard
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        plugIn.Parameters[1],
                        0,
                        string.Empty,
                        plugIn.Name
                    };
                case "7008f6d4-73ea-419f-8f08-5fc7f81740ea":
                    //UpdateCpu 
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        plugIn.Parameters[1],
                        0,
                        string.Empty,
                        plugIn.Name
                    };
                case "63b1a0d7-11cc-4680-9b4a-04740dd4f2ea":
                    //UpdateMemory 
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        plugIn.Parameters[1],
                        0,
                        string.Empty,
                        plugIn.Name
                    };
                case "7025123c-4eee-4461-a183-a81535903a59":
                    //UpdateProcess 
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        plugIn.Parameters[1],
                        0,
                        string.Empty,
                        plugIn.Name
                    };
                case "91e63a4f-9ead-4127-bb5d-19f67622ab9d":
                    //UpdatePlugIn
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        plugIn.Parameters[1],
                        0,
                        string.Empty,
                        plugIn.Name
                    };
                case "c29ed7be-8d7f-4765-83fe-d2a8b581f446":
                    //UpdateDisk 
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        plugIn.Parameters[1],
                        0,
                        string.Empty,
                        plugIn.Name
                    };
                case "02959648-ed95-4220-bef7-a8983da81f18":
                    //Ping 
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        plugIn.Parameters[1],
                        plugIn.Parameters[2],
                        string.Empty,
                        plugIn.Name
                    };
                case "f6038d00-c898-4f98-b143-bb6edc51ae17":
                    //FileManage();
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        0,
                        0,
                        string.Empty,
                        plugIn.Name
                    };
                case "a90a90e3-57d3-4711-9e70-30c53c615d70":
                    //GameServerControl();
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        0,
                        0,
                        string.Empty,
                        plugIn.Name
                    };
                case "ccdc26d6-9ca5-4f3f-84f1-4e5fa0493956":
                    //MessageLog
                    return new object[]{
                        gameServerId,
                        plugIn.Guid,
                        0,
                        0,
                        //"D:\\log_"+gameServerId+".txt",
                        plugIn.Parameters[1],
                        plugIn.Name
                    };
                default:
                    return null;
            }
        }


        ///// <summary>
        ///// ��Ĭ�ϲ��������µ�IPlugIn
        ///// </summary>
        ///// <param name="PlugIn_Guid"></param>
        ///// <returns></returns>
        //public static IPlugIn GetDefaultPlugIn(string PlugIn_Guid)
        //{
        //    switch (PlugIn_Guid)
        //    {
        //        case "9609275e-205e-47a5-9065-58e7c52e3cbf":
        //            return new GameServerDb();
        //        case "3b2635c2-64ed-48c3-8ae8-25eed4ddbc88":
        //            return new LordControl();
        //        case "895235f4-47d8-4ddb-af7c-fdb3591d437f":
        //            return new UpdateNetworkCard();
        //        case "7008f6d4-73ea-419f-8f08-5fc7f81740ea":
        //            return new UpdateCpu();
        //        case "63b1a0d7-11cc-4680-9b4a-04740dd4f2ea":
        //            return new UpdateMemory();
        //        case "7025123c-4eee-4461-a183-a81535903a59":
        //            return new UpdateProcess();
        //        case "91e63a4f-9ead-4127-bb5d-19f67622ab9d":
        //            return new UpdatePlugIn();
        //        case "c29ed7be-8d7f-4765-83fe-d2a8b581f446":
        //            return new UpdateDisk();
        //        case "02959648-ed95-4220-bef7-a8983da81f18":
        //            return new Ping();
        //        case "f6038d00-c898-4f98-b143-bb6edc51ae17":
        //            return new FileManage();
        //        case "a90a90e3-57d3-4711-9e70-30c53c615d70":
        //            return new GameServerControl();
        //        case "ccdc26d6-9ca5-4f3f-84f1-4e5fa0493956":
        //            return new MessageLog();
        //        default:
        //            return null;
        //    }
        //}

        /// <summary>
        /// ��Ĭ�ϲ��������µ�IPlugIn
        /// </summary>
        /// <param name="PlugNum">PlugInList�е�enumֵ</param>
        /// <returns></returns>
        public static IPlugIn GetDefaultPlugIn(int PlugNum)
        {
            switch (PlugNum)
            {
                case (int)PlugInList.GameServerDb:
                    return new GameServerDb();
                case (int)PlugInList.LordControl:
                    return new LordControl();
                case (int)PlugInList.UpdateNetworkCard:
                    return new UpdateNetworkCard();
                case (int)PlugInList.UpdateCpu:
                    return new UpdateCpu();
                case (int)PlugInList.UpdateMemory:
                    return new UpdateMemory();
                case (int)PlugInList.UpdateProcess:
                    return new UpdateProcess();
                case (int)PlugInList.UpdatePlugIn:
                    return new UpdatePlugIn();
                case (int)PlugInList.UpdateDisk:
                    return new UpdateDisk();
                case (int)PlugInList.Ping:
                    return new Ping();
                case (int)PlugInList.FileManage:
                    return new FileManage();
                case (int)PlugInList.GameServerControl:
                    return new GameServerControl();
                case (int)PlugInList.MessageLog:
                    return new MessageLog();
                default:
                    return null;
            }
        }


    }
}
