using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using MapLogicalFileReader;
using System.Collections;

namespace OfficialWebDB
{
    public partial class Form1
    {
        private bool Fill_Npc()
        {
            // ����ϱ�����
            Helper.ClearDbTable("web_npcs", Conn);

            const int MapCount = 41;
            Hashtable htMapLogicaNpcReader = new Hashtable(MapCount);//key:mapname value:TempleteIdXYProcessor
            Hashtable htMapNames = new Hashtable(61); //key:mapname in npctemplate value: map directory name

            FillhtMapNames(htMapNames);

            // ����
            DataRow defaultrow = TblNpcTemplate.Rows[0];
            foreach (DataRow row in TblNpcTemplate.Rows)
            {
                // npc�ȼ�
                int npclevel = Convert.ToInt32(row["Level"] == DBNull.Value ? defaultrow["Level"] : row["Level"]);
                if (npclevel > 100)
                    continue;

                // npc_id
                int npcid = Convert.ToInt32(row["ID"]);
                // npc����
                string npcmap = row["MapName"].ToString().Trim();
                // npc����
                string npcname = row["Name"].ToString().Trim();
                // npc�ƺ�
                string npctitle = row["Title"].ToString().Trim();
                // npcǿ��
                int ns = Convert.ToInt32(row["Intensity"] == DBNull.Value ? defaultrow["Intensity"] : row["Intensity"]);
                string npcintensity = GetNpcIntensity(ns);
                // ��Ӫ
                int forceid = Convert.ToInt32(row["ForceID"] == DBNull.Value ? defaultrow["ForceID"] : row["ForceID"]);
                string forcename = GetForceNameByID(forceid);
                // ����
                int race = Convert.ToInt32(row["RaceClass"] == DBNull.Value ? defaultrow["RaceClass"] : row["RaceClass"]);
                string racename = GetNpcRaceName(race);

                float npc_x = 0.0f;
                float npc_y = 0.0f;
                string strx = "null";
                string stry = "null";

                if (htMapNames.ContainsKey(npcmap))
                {
                    string logicalmap = htMapNames[npcmap].ToString();
                    if (logicalmap != "" &&
                        GetNPC_XY(logicalmap, npcid, ref npc_x, ref npc_y, htMapLogicaNpcReader)
                        )
                    {                    
                        strx = npc_x.ToString();
                        stry = npc_y.ToString();
                    }
                }

                
              
                // ����һ��quest������
                string sql = string.Format("insert into web_npcs values({0}, '{1}', '{2}', '{3}', '{4}', '{5}', {6}, '{7}', {8}, {9})",
                    npcid, npcmap, npcname, npctitle, npcintensity, forcename, npclevel, racename, strx, stry);
                Helper.ExecSqlCmd(sql, Conn);
            }

            htMapLogicaNpcReader.Clear();
            htMapLogicaNpcReader = null;
            htMapNames.Clear();
            htMapNames = null;

            return true;
        }
        private bool GetNPC_XY(string mapName, int nTemplateId, ref float x, ref float y, Hashtable htProcessors)
        {
            TempleteIdXYProcessor processor;
            if (htProcessors.ContainsKey(mapName))
            {
                processor = htProcessors[mapName] as TempleteIdXYProcessor;
            }
            else 
            {
                string maplogicaFilePath = string.Format("{0}/data/source/maps/{1}/{1}.Map.Logical", RootDir, mapName);
                string configFilePath = string.Format("{0}/data/source/maps/{1}minimap/config.ini", RootDir, mapName);
                processor = new TempleteIdXYProcessor(maplogicaFilePath, configFilePath, "NPC");
                processor.Init();
              
                htProcessors[mapName] = processor;
            }

            if (!processor.Available)
            {
                return false;
            }
            return  processor.GetScaleXY(nTemplateId, ref x, ref y);
        }
       
        //
        private void FillhtMapNames(Hashtable htMaps)
        { 
            DataTable dt = Helper.GetDataTable("select * from web_npc_maps", Conn);

            string WebMapName;
            string LogicalMapName;
            foreach (DataRow row in dt.Rows)
            {
                WebMapName = row["web_npc_map"].ToString();

                if (row.IsNull("logical_map"))
                {
                    continue;
                }
                LogicalMapName = row["logical_map"].ToString();
                if (LogicalMapName == "")
                {
                    continue;
                }

                htMaps[WebMapName] = LogicalMapName;
            }

            dt.Dispose();

        }
       
        private string GetNpcRaceName(int race)
        {
            string[] strRaces = {
                "��", "����", "������", "������", "��ʬ��", "������"
            };
            return strRaces[race];
        }

        private string GetNpcIntensity(int intensity)
        {
            if (intensity == 2)
                return "����";
            else if (intensity == 4 || intensity == 5 || intensity == 7)
                return "ͷĿ";
            else if (intensity == 6)
                return "����";
            else if (intensity == 1 || intensity == 3)
                return "��ͨ";

            return "δ֪ǿ��";
        }
    }
}
