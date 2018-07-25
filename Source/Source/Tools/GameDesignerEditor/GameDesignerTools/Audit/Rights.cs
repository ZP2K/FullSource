using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace GameDesingerTools
{
    static class RightsSettings
    {
        static public bool AllowSave = true;
        static public bool AllowModifyValue = true;
        static public bool AllowModifyTree = true;
        static public bool AllowModifyScript = true;
    };

    class RightManager
    {
        public string _hostname;
        public string _ip;
        public string _username;
        public string _psw;
        public string _userindex;

        public void Init()
        {
            try
            {
                StringBuilder sb = new StringBuilder(255);
                FileFolderHelper.ReadIniFile("Rights", "AllowSave", "1", sb, 255, Application.StartupPath + "/GameDesingerTools.ini");
                RightsSettings.AllowSave = Convert.ToInt32(sb.ToString()) == 1 ? true : false;
                RightsSettings.AllowModifyValue = RightsSettings.AllowSave;
                RightsSettings.AllowModifyTree = RightsSettings.AllowSave;
                RightsSettings.AllowModifyScript = RightsSettings.AllowSave;

                CheckACL();
            }
            catch (Exception ex)
            {
                string strExtMsg = "������Ϣ:\n";
                string strHostName = Helper.GetHostName().ToLower();
                strExtMsg += "������:" + strHostName + "\n";
                strExtMsg += "IP��ַ:";
                System.Net.IPAddress[] ips = Helper.GetIPAddresses();
                foreach (System.Net.IPAddress ip in ips)
                {
                    string strip = ip.ToString();
                    strExtMsg += strip + "\n";
                }

                throw new Exception("Ȩ��ϵͳ��ʼ��ʧ�ܣ�\n" + strExtMsg + "\r\n������ϢΪ��\r\n" + ex.Message);
            }
        }
        private void CheckClientVer()
        {
            //���DUMMY��������ǿ��ʹ�û�����ʹ�þɰ汾�Ŀͻ��˳���
            /*System.Data.DataTable tbldummy = CustomClassCache.GetDataTableWithKey("select * from sys_dummy_1");*/
            System.Data.DataTable tbldummy = CustomClassCache.GetDataTableWithKeyProxy("sys_dummy_1");
            tbldummy.Dispose();
        }

        private void CheckACL()
        {
            //if (MainForm.conn.Database.ToString().ToLower() == "s3design" || MainForm.conn.Database.ToString().ToLower() == "s3design_debug")
            //{
            //    return; //�������Կ��е�����
            //}
            
            CheckClientVer();

            //����Ƿ��û������б��е��û�������ܾ�����
            string strHost = Helper.GetHostName().ToLower();
            string _sql = string.Format("SELECT * FROM sys_useracl WHERE userid = '{0}'", strHost);
            /*System.Data.DataTable tbl = CustomClassCache.GetDataTableWithKey(_sql);*/
            string filter = string.Format("userid = '{0}'", strHost);
            System.Data.DataTable tbl = CustomClassCache.GetDataTableWithKeyProxy("sys_useracl", filter, null);

            //System.Data.DataRow row = null;
            bool bFind = false;
            System.Net.IPAddress[] ips = Helper.GetIPAddresses();
            foreach (System.Data.DataRow r in tbl.Rows)
            {
                foreach (System.Net.IPAddress ip in ips)
                {
                    if (r["ip"].ToString().Trim() == ip.ToString())
                    {
                        bFind = true;

                        // ������Ϣ
                        _hostname = Helper.GetHostName();
                        _ip = Helper.GetIPAddress();
                        _userindex = r["_index"].ToString().Trim();
                        //todo: �Ժ�Ҫ�ӱ���iniȡ
                        _username = string.Empty;
                        _psw = string.Empty;

                        break;
                    }
                }
            }
            if (!bFind)
                throw new Exception("��Ȩ���ʣ�");
        }
        public void Uninit()
        {

        }
    };
}
