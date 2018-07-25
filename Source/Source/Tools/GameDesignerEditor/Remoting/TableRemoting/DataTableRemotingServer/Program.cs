using System;
using System.Collections.Generic;
using System.Text;
using DataTableRemoting;

namespace DataTableRemotingServer
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.ForegroundColor = ConsoleColor.DarkYellow;
            string title = "DataTableServer Runing";
            bool success = true;
            try
            {
                DataTableServer.Run("DataTableRemotingServer.exe.config");
            }
            catch (Exception ex)
            {
                Console.WriteLine("����������ʧ�ܣ���鿴�����ļ���\r\n" + ex.Message);
                success = false;
            }
            if (success)
            {

                Console.BufferHeight = 8000;
                Console.BufferWidth = 300;
                Console.WriteLine("Server start success.");

                string[] classes = DataTableServer.GetRemotingEntries();
                Console.WriteLine(classes[0]);

                while (DataTableServer.RunServer)
                {
                    NetClientManager.CheckConnect();
                    Console.Title = title;
                    title += ".";
                    if (title.Length > 30)
                    {
                        title = "DataTableServer Runing";
                    }
                    System.Threading.Thread.Sleep(1000);
                }
                DataTableServer.Release();
            }
        }
    }
}