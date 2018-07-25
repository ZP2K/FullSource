using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using LuaInterface;
using System.Collections;

namespace GameLuaEditor
{
    public partial class ImportForm : Form
    {
        private string[] as_maps = new string[] {"��","����", "�����", "���", "����", "����" };
        //private string[] as_maps = new string[] {"�����"};

        public ImportForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            button1.Enabled = false;
            foreach(string strMap in as_maps)
            {
                Export(strMap);
            }
            MessageBox.Show("�ɹ�");
            button1.Enabled = true;
        }
        private void Export(string strMap)
        {
            string strPath = @"D:\Kingsoft\sword3-products\trunk\client\scripts\Map\" + strMap;
            Lua mLua = new Lua();
            mLua.DoFile(strPath + @"\include\�Ի�.ls");

            string[] as_head = new string[]
            { 
                "COMMON_NPC_STRING",
                "COMMON_DOODAD_STRING",
                "COMMON_ITEM_STRING",
                "COMMON_TRAP_STRING",
                "COMMON_OTHER_STRING",

                "WANHUA_DOODAD_STRING",
                "WANHUA_ITEM_STRING",
                "WANHUA_COMMON_NPC_STRING"
            };           

            //�����ļ�
            foreach(FileInfo fi in new DirectoryInfo(strPath).GetFiles("*.*", SearchOption.AllDirectories))
            {
                string strCode = Helper.FileToString(fi.FullName);
                if(fi.Name.ToLower().EndsWith(".lua"))
                {
                    //��������
                    foreach(string strHead in as_head)
                    {
                        LuaTable lt_head = mLua.GetTable(strHead);
                        if (lt_head == null) continue;
                        //����ĳ����
                        foreach (string strKey in lt_head.Keys)
                        {
                            if (lt_head[strKey] is LuaTable)
                            {
                                string strPreDim = "";
                                LuaTable lt = lt_head[strKey] as LuaTable;
                                strPreDim = "LS_" + strKey + " = {";
                                foreach(object substring in lt.Values)
                                {
                                    strPreDim += "\r\n\t\"" + substring.ToString().Replace("\n", "\\n") + "\",";
                                }
                                if(strCode.IndexOf(strHead + "[\"" + strKey + "\"]") != -1)
                                    strCode = strPreDim.TrimEnd(new char[] { ',' }) + "\r\n}\r\n" + strCode;
                                strCode = strCode.Replace("math.random(", "Random(");
                                strCode = strCode.Replace(strHead + "[\"" + strKey + "\"]", "LS_" + strKey);                         
                            }
                            else if(lt_head[strKey] is string)
                            {
                                strCode = strCode.Replace(strHead + "[\"" + strKey + "\"]", "\"" + lt_head[strKey].ToString().Replace("\n", "\\n") + "\"");
                            }
                        }
                    }
                    strCode = strCode.Replace("Include(\"scripts/Map/" + strMap + "/include/�Ի�.ls\");", "");
                }
                else if (fi.Name == "�Ի�.ls")
                {
                    //donothing
                    continue;
                }
                else if (fi.Name.ToLower().EndsWith(".ls"))
                {
                    //�����û��Զ���ls��������
                }
                else
                {
                    //SVN�ļ�������
                    continue;
                }
                
                //ת����Ķ�������ļ�
                string strOutPath = fi.FullName.Replace(strPath, @"D:\xxx\" + strMap);
                Helper.WriteStringToFile(strCode, strOutPath);
            }


        }

        private void button2_Click(object sender, EventArgs e)
        {
            button2.Enabled = false;
            string strR = "";
            DataBaseIO.DataBaseManager dbm = DataBaseIO.DataBaseManager.GetDataBaseManager();
            DataTable dt = dbm.GetScriptInformation();
            foreach (FileInfo fi in new DirectoryInfo(@"d:\xxx").GetFiles("*.*", SearchOption.AllDirectories))
            {
                string strName = fi.FullName.Replace(@"d:\xxx", @"scripts\Map");
                string strCode = Helper.FileToString(fi.FullName);
                if (dt.Select("path='" + strName.ToLower() + "'").Length > 0 ||
                    dt.Select("path='" + strName.ToUpper() + "'").Length > 0 )
                {
                    strR += strName + "\r\n";
                    continue;
                }
                //�ؼ���λ����ȥ��ע�;Ϳ��Ե�����
                //dbm.CreateScriptData(strName);
                //string strView = "":
                //bool bret = dbm.SaveScriptData(strName, strCode, out strView);
                //if(bret)
                //    File.Delete(fi.FullName);
            }
            MessageBox.Show("�ɹ�");
            button2.Enabled = true;
        }
    }
}