using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
namespace ExecLua
{
    class Program
    {
        static void Main(string[] args)
        {

            LuaBuilder lb1;
            string str = "";
            if (args.Length > 0)
            {
                str = args[0];
                LuaBuilder lb = new LuaBuilder(str);
                lb.Run();
            }
            else
            {
                Console.WriteLine("û�������в�����");
                //lb1 = new LuaBuilder("F:\\�߻�����\\Plugins\\LuaCheck\\���� tmp.lua");
                //lb1.Run();
            }
                return;
        }
    }
}
