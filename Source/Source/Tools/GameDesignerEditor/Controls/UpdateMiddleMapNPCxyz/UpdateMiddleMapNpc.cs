using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Data.SqlClient;
using System.Data;
using System.IO;
using System.Windows.Forms;

namespace UpdateMiddleMapNPCxyz
{
    class UpdateMiddleMapNpc
    {
        const int MaxMapCntCapcity = 29;
        private Hashtable htNpcIndex = new Hashtable(MaxMapCntCapcity);//key : mapname,value: NpcIndex
        private Hashtable htRegionInfo = new Hashtable(MaxMapCntCapcity);//key : mapname,value: RegionInfo

        private string strRootDir = "";
        private SqlConnection Conn;
        const string TableName = "middlemap_npc";

        private StringBuilder sbErrorMsg = new StringBuilder();

        static UpdateMiddleMapNpc g_UpdateMiddleMapNpcInstance;

        public static UpdateMiddleMapNpc GetUpdateMiddleMapNpcInstance()
        {
            if (g_UpdateMiddleMapNpcInstance == null)
            {
                g_UpdateMiddleMapNpcInstance = new UpdateMiddleMapNpc();
            }
            return g_UpdateMiddleMapNpcInstance;
        }

        public bool Init(string rootdir, SqlConnection conn)
        {
            sbErrorMsg.Remove(0, sbErrorMsg.Length);
            if (!Directory.Exists(rootdir))
            {
                return false;
            }
            strRootDir = rootdir;
            Conn = conn;

            return true;
        }
        /// <summary>
        /// Ϊmiddlemap_npc�����еĵ�ͼlogical�ļ���������
        /// </summary>
        /// <returns></returns>
        private bool InithtNpcIndexAndRegionInfo()
        {
            string sql = string.Format("select DISTINCT {0} from {1}", "map", TableName);
            DataTable dt = Helper.GetDataTable(sql, Conn);

            string mapname = string.Empty;
           
            foreach (DataRow dr in dt.Rows)
            {
                mapname = dr[0].ToString();
                InithtNpcIndexAndRegionInfo4OneMap(mapname);
            }
            return true;
        }
        /// <summary>
        /// Ϊһ����ͼ�� logical�ļ��������� �� ��ȡ������Ϣ
        /// </summary>
        /// <param name="mapname"></param>
        /// <returns></returns>
         private bool InithtNpcIndexAndRegionInfo4OneMap(string mapname)
         {
             NpcIndex npcindex = null;
             RegionInfo regionInfo = null;

             if (!htNpcIndex.ContainsKey(mapname))
             {
                 npcindex = new NpcIndex(strRootDir);
                 npcindex.MapName = mapname;
                 if (npcindex.Init())
                 {
                     htNpcIndex[mapname] = npcindex;
                 }
                 else
                 {
                     sbErrorMsg.Append(npcindex.ErrorMsg);
                 }
             }
             if (!htRegionInfo.ContainsKey(mapname))
             {
                 regionInfo = new RegionInfo();
                 if (regionInfo.Init(strRootDir, mapname))
                 {
                     htRegionInfo[mapname] = regionInfo;
                 }
                 else
                 {
                     sbErrorMsg.Append(regionInfo.strErrorMsg);
                 }
             }
             return true;

         }
        /// <summary>
        /// ���±�middlemap_npcȫ��
        /// </summary>
        public void UpdateAllTable()
        {
            InithtNpcIndexAndRegionInfo();
            string sql = "select * from " + TableName;
            DataTable dt = Helper.GetDataTable(sql, Conn);
            Update4Table(dt);
        }
        /// <summary>
        /// ����һ���ڵ�
        /// </summary>
        /// <param name="node"></param>
        public void UpdateTable4OneNode(TreeNode node)
        {            
            string sql = string.Empty;
            string mapname = string.Empty;

            if (node.Parent == null && node.Nodes.Count != 0)
            {
                mapname = node.Text;
                sql = string.Format("select * from {0} where map = '{1}'", TableName, mapname);
            }
            else if (node.Parent != null && node.Nodes.Count == 0)
            {
                mapname = node.Parent.Text;
                sql = string.Format("select * from {0} where map = '{1}' and name = '{2}'", TableName, mapname, node.Text);
            }
            InithtNpcIndexAndRegionInfo4OneMap(mapname);

            DataTable dt = Helper.GetDataTable(sql, Conn);
            Update4Table(dt);
        }
        /// <summary>
        /// ����dt�����е���
        /// </summary>
        /// <param name="dt"></param>
        private void Update4Table(DataTable dt)
        {  
            string mapname = string.Empty;
            int templateid = 0;
            int middlemapid = 0;
            PointXY point;
            NpcIndex npcindex = null;
            RegionInfo regionInfo = null;
            string sqlupdate = string.Empty;

            foreach (DataRow dr in dt.Rows)
            {
                mapname = dr["map"].ToString();
                templateid = Convert.ToInt32(dr["id"]);

                if (templateid == 0)
                    continue;

                npcindex = htNpcIndex[mapname] as NpcIndex;
                regionInfo = htRegionInfo[mapname] as RegionInfo;
                
                if (npcindex != null)
                {
                    point = npcindex.GetPoint(templateid);    
                    
                    if (point.nX == 0 && point.nY == 0)
                    {
                        string str = string.Format("�����ļ�.Map.Logical���Ҳ�����npc������ɾ��֮��\r\n��������{0}  \r\nnpcģ��id��{1}", mapname, templateid);
                        //MessageBox.Show(str);
                        Console.WriteLine(str);
                        continue;
                    }

                    if (regionInfo != null)
                    {
                        middlemapid = regionInfo.GetMiddleMapId(point.nX, point.nY);
                    }

                    sqlupdate = string.Format("update {0} set x = {1}, y = {2}, middlemap = {3} where id = {4}", TableName, point.nX, point.nY, middlemapid, templateid);//middlemap�ĸ�����ʱû��
                    Helper.ExecuteSqlCmd(sqlupdate, Conn);
                }                

            }

        }
        //���س��ֵĴ���
        public string CheckError()
        {
            return sbErrorMsg.ToString();
        }
    }
}
