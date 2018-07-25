using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Collections;

namespace UpdateMiddleMapNPCxyz
{
    struct PointXY
    {
        public int nX;
        public int nY;
    };

    //Ϊָ����ͼ����npc����
    //key��nTempleteID,value��xy��������� ht[123] = PointXY
    //�����ж��nTempleteID��ͬ�ģ�ֻȡ�����ȶ����ĵ�һ��
    class NpcIndex
    {
        private string strMapName;
        private string strRootDir;

        private string strErrorMsg;
        private bool bHasError = false;

        const int MaxNpcCapcity = 199;
        private Hashtable htIndex = new Hashtable(MaxNpcCapcity);//key:nTemepleteId,value:[NPC**]
        
        public string RootDir
        {
            set
            {
                if (Directory.Exists(value))
                    strRootDir = value;
                else
                {
                    strRootDir = "";
                    strErrorMsg = string.Format("Dictinory [{0}] is not exists!\r\n", value);
                    bHasError = true;
                }
            }
        }

        public string MapName
        {
            set
            {
                string strMapLogicalFileFullName = string.Format("{0}\\data\\source\\maps\\{1}\\{1}.Map.Logical", strRootDir, value);
                if (File.Exists(strMapLogicalFileFullName))
                    strMapName = value;
                else
                {
                    strErrorMsg = string.Format("[{0}]'s map.logical is not exists!\r\n", value);
                    bHasError = true;
                    strMapName = "";
                }
            }
            get
            {
                return strMapName;
            }
        }

        public string ErrorMsg
        {
            get
            {
                return strErrorMsg;
            }
        }
        //���캯��
        public NpcIndex(string rootdir)
        {           
            RootDir = rootdir;            
        }

        //��ȡstrMapName��Logical.map�ļ������htIndex
        private bool ReadLogicalFile()
        {
            string strMapLogicalFileFullName = string.Format("{0}\\data\\source\\maps\\{1}\\{1}.Map.Logical", strRootDir, strMapName);
            if (!File.Exists(strMapLogicalFileFullName))
            {
                strErrorMsg = string.Format("file {0} is not exists!\r\n", strMapLogicalFileFullName);
                return false;
            }

            // ���ļ�
            string content = Helper.FileToString(strMapLogicalFileFullName, Encoding.Default);
            byte[] bytes = Encoding.Default.GetBytes(content);
            bytes = Encoding.Convert(Encoding.Default, Encoding.Unicode, bytes);
            char[] chars = Encoding.Unicode.GetChars(bytes);
            content = new String(chars);
            Helper.OpenIniFile(strMapLogicalFileFullName, content);
            if (Helper.m_inis == null)
            {
                strErrorMsg = string.Format("{0} ������ȷ��ini��ʽ��\r\n���ܰ����� \"<<<<<<< .mine\" �� SVN �ϲ������ַ�����\r\n����֮��", strMapLogicalFileFullName);                
                return false;
            }
            
            int NpcCnt = Convert.ToInt32(Helper.GetIniString("MAIN", "NumNPC"));
            string strSection = string.Empty;
            int nTempleteID = 0;

            for (int i = 0; i < NpcCnt; ++i)
            {
                strSection = string.Format("NPC{0}", i);
                nTempleteID = Convert.ToInt32(Helper.GetIniString(strSection, "nTempleteID"));
                if (!htIndex.ContainsKey(nTempleteID))
                {
                    PointXY point = new PointXY();
                    point.nX = Convert.ToInt32(Helper.GetIniString(strSection, "nX"));
                    point.nY = Convert.ToInt32(Helper.GetIniString(strSection, "nY"));
                    htIndex[nTempleteID] = point;
                }                
            }
            Helper.CloseIniFile();

            return true;
        }

        public bool Init()
        {
            bool bRet = false;
            if (!bHasError)
            {
                bRet = ReadLogicalFile();
            }            
            return bRet;
        }

        //����nTempleteID�������
        //id�����ڷ���0��0
        public PointXY GetPoint(int templateid)
        {
            PointXY pointRet;
            if (!htIndex.ContainsKey(templateid))
            {
                strErrorMsg = string.Format("mapname = {0},nTempleteID = {1} is not exists.\r\n", strMapName, templateid);                
                pointRet = new PointXY();
                pointRet.nX = 0;
                pointRet.nY = 0;
            }
            else
            {
                pointRet = (PointXY)htIndex[templateid];
            }
            return pointRet;
        }
    }
}
