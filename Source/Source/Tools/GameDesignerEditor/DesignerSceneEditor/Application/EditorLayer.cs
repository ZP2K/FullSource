using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Data.SqlClient;
using System.IO;
using AtlKG3DEngineProxyLib;

namespace DesignerSceneEditor
{
	class EditorLayer
	{
        [DllImport("kernel32.dll")]
        static extern void ExitProcess(uint exitCode);


        static public MainForm MainFrm = null;
        static public bool UseSO3Logic = true;
        static public string EngineDir = string.Empty;
        static public int MainFrmHwnd = 0;
        static public Encoding Encode936 = Encoding.GetEncoding(936);

        private const int MAX_PATH = 260;
        private const string configFile = "GameDesingerTools.ini";

        static public void Init()
        {
            MainFrm = new MainForm();
            MainFrmHwnd = (int)MainFrm.Handle;

            Helper.SetAdditionalEnvPath(Application.StartupPath);


            string iniFile = Application.StartupPath + "/" + configFile;
            FileInfo file = new FileInfo(iniFile);
            if (!file.Exists)
            {
                MessageBox.Show("�����ļ� " + configFile + " �����ڡ�\r\n���򼴽��˳���");
                Application.Exit();
            }
            StringBuilder sbEngineDir = new StringBuilder(MAX_PATH);
            FileFolderHelper.ReadIniFile("General", "RootDir", "", sbEngineDir, MAX_PATH, iniFile);
            EngineDir = sbEngineDir.ToString().Trim();
            Helper.RootDir = EngineDir.TrimEnd(new char[] { '\\' });

            DirectoryInfo dir = new DirectoryInfo(EngineDir);
            if (!dir.Exists)
            {
                MessageBox.Show("����Ŀ¼�����ڡ��������趨" + configFile + "\r\n���򼴽��˳���");
                Application.Exit();
            }
            else // ����Ƿ�����ȷ�Ŀͻ���·��
            {
                string engineResourcePath = Path.Combine(EngineDir, "KG3DEngine.dll");
                if (!File.Exists(engineResourcePath))
                {
                    MessageBox.Show("����Ŀ¼����ȷ���������趨" + configFile + "\r\n���򼴽��˳���");
                    Application.Exit();
                }
                else // ���uselogic��û�д�
                {
                    StringBuilder configString = new StringBuilder(MAX_PATH);
                    iniFile = Path.Combine(EngineDir, "config.ini");
                    FileFolderHelper.ReadIniFile("ENGINEOPTION", "UseLogical", "", configString, MAX_PATH, iniFile);
                    string useLogic = configString.ToString().Trim();

                    if (useLogic != "1")
                    {
                        MessageBox.Show("config.iniû�����ô��߼����������趨" + configFile + "\r\n���򼴽��˳���");
                        Application.Exit();
                    }
                }
            }
        }

        static public void Run()
        {
            Application.Idle += new EventHandler(OnApplicationIdle);
            Application.Run(MainFrm);
        }

        static void OnApplicationIdle(object sender, EventArgs e)
        {
            //EngineLayer.ATLBase.UpdateKeyState();
            //EngineLayer.ATLBase.FrameMove();
            //EngineLayer.ATLBase.Render();
        }

        static public void Uninit()
        {
            try
            {
                ExitProcess(0);
            }
            catch
            {
                ExitProcess(0);
            }
        }
	}
}
