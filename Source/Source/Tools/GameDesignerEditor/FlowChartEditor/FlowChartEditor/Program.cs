using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;

namespace FlowChartEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            // ��ʼ����������
            EV ev = new EV();
            ev.AddToEnvironmentPath(Path.GetDirectoryName(Application.ExecutablePath));

            // ����û���Ȩ
            DBManager.DataBaseManager dataBaseManager = DBManager.DataBaseManager.GetDataBaseManager();
            Helper helper = Helper.GetHelper();
            string hostName = helper.GetHostName();
            List<string> addressList = helper.GetHostAddresses();
            bool valid = false;

            foreach (string s in addressList)
            {
                if (dataBaseManager.CheckAuthority(hostName, s))
                {
                    valid = true;
                    break;
                }
            }

            if (valid)
            {                
                Application.Run(new MainForm());
            }
            else
            {
                MessageBox.Show(string.Format("Ȩ�����ʧ�ܣ�\r\n\r\n�û�����{0}\r\nIP��{1}\r\n", hostName, addressList[0]), "��Ȩ���",
                                              MessageBoxButtons.OK, MessageBoxIcon.Information);
            }            
        }

        class EV
        {
            /// <summary>
            /// ��ĳ��Ŀ¼�µ�������Ŀ¼׷�ӵ�path����������
            /// </summary>
            /// <param name="path">Ŀ¼��</param>
            public void AddToEnvironmentPath(string path)
            {
                string spath = Environment.GetEnvironmentVariable("path");
                AddToEnvironmentPath(path, ref spath);
                Environment.SetEnvironmentVariable("path", spath);
            }

            /// <summary>
            /// �ݹ��Ŀ¼��������Ŀ¼׷�ӵ�path����������
            /// </summary>
            /// <param name="path">��ǰ·��</param>
            /// <param name="spath">��·��</param>
            private void AddToEnvironmentPath(string path, ref string spath)
            {
                DirectoryInfo di = new DirectoryInfo(path);

                foreach (DirectoryInfo d in di.GetDirectories())
                {
                    if (!d.Name.StartsWith("_"))
                    {
                        spath += ";" + d.FullName;
                    }
                }

                foreach (DirectoryInfo d in di.GetDirectories())
                {
                    if (!d.Name.StartsWith("_"))
                    {
                        AddToEnvironmentPath(d.FullName, ref spath);
                    }
                }
            }
        };
    }
}