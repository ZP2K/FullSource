using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;


//����ʱ�䣺2009-1-21


namespace UpdateMiddleMapNPCxyz
{
    //��ȡ�ͻ��˵ĵ�ͼlogical�ļ�
    //���±�middlemap_npc����ֶ�middlemap,x,y,z 
    public class TableUpdater
    {
        public string Update(string rootdir, System.Data.SqlClient.SqlConnection conn)
        {
            UpdateMiddleMapNpc updater = UpdateMiddleMapNpc.GetUpdateMiddleMapNpcInstance();

            if ( updater.Init(rootdir, conn))
            {
                updater.UpdateAllTable();
            }
            return updater.CheckError();
        }
        public string UpdateOneNode(string rootdir, System.Data.SqlClient.SqlConnection conn, TreeNode node)
        {
            UpdateMiddleMapNpc updater = UpdateMiddleMapNpc.GetUpdateMiddleMapNpcInstance();

            if (updater.Init(rootdir, conn))
            {
                updater.UpdateTable4OneNode(node);
            }
            return updater.CheckError();

        }
    }
    
    //�����ݱ�middlemap_npc������npc.tab�ļ�(���ж��)
    public class DB2TabExportor
    {
        public bool ExportNpcTab(System.Data.SqlClient.SqlConnection conn, string rootdir)
        {
            MiddleMapNpcExportor npcExportor = new MiddleMapNpcExportor(rootdir, conn);
            npcExportor.Export();

            return true;
        }
    };
}
