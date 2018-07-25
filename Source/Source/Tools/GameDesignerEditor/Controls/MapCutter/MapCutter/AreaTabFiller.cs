using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.IO;

namespace MapCutter
{
    class AreaTabFiller
    {
        private Hashtable htAreaTabRows;//key:�Ҷ� value:��Ӧ�е��ַ�������
        private Hashtable htAssistAreaTabRows;//�����ṩ������AreaTab���ļ���Ϊ���ǻ�ȡx,y
        private Hashtable htConfigs;//key:middlemap value:MiddleMapConfig(config.ini�ļ����Ӧmiddlemap������)
        private Hashtable htHeader;//key������ values��������

        private string RootDir;
        private string MapName;
        private string[] TabFileHead = {"id","name", "middlemap", "backgroundmusic", "type", "x", "y", "z"};
        private int ColCnt;
      

        public void Init(string rootdir, string mapName, string assistFilePath)
        {
            htAreaTabRows = new Hashtable();
            htAssistAreaTabRows = new Hashtable();
            htConfigs = new Hashtable();
            htHeader = new Hashtable();

            RootDir = rootdir;
            MapName = mapName;
            InitHeader();

            ReadTabFile(htAssistAreaTabRows, assistFilePath);
            
            string AreaFilePath = string.Format("{0}\\data\\source\\maps\\{1}minimap\\area.tab", RootDir, MapName);
            ReadTabFile(htAreaTabRows, AreaFilePath);

            InitConfig();


        }

        private void ReadTabFile(Hashtable htRows, string tabFilePath)
        {
            if (!File.Exists(tabFilePath))
            {
                throw new Exception(tabFilePath + "������");
            }
           
            string fileContent;
            fileContent = Helper.FileToString(tabFilePath).Replace("\r", "");
            int id;
            string[] row;
            
            foreach (string str in fileContent.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries))
            {
                if (str.IndexOfAny("0123456789".ToCharArray()) == -1)
                {
                    continue;
                }
                row = str.Split(new char[] { '\t' }, StringSplitOptions.RemoveEmptyEntries);
                if (row.Length < 6)
                {
                    throw new Exception(string.Format("\r\n�ļ�{0}\r\n��\"{1}\"��ʽ����", tabFilePath, str));
                }
               
                if (row[Convert.ToInt32(htHeader["id"])] == "id")//��һ����ͷ�Ļ�������
                {
                    continue;
                }
                id = Convert.ToInt32(row[0]);

                if (!Helper.CanConverToNumber(row[5]) ||
                    !Helper.CanConverToNumber(row[5]))
                {
                    throw new Exception(string.Format("\r\n�ļ�{0}\r\n��\"{1}\"��ʽ����", tabFilePath, str));
                }

                string[] rowFull = NewFullRow(row);
                htRows[id] = rowFull; 

            }
        }
        private string[] NewFullRow(string[] row)
        {
            int colCnt = ColCnt;
            if (row.Length > ColCnt)
            {
                colCnt = row.Length;
            }
            string[] newRow = new string[colCnt];

            int i = 0;
            for (i = 0; i < row.Length; ++i)
            {
                newRow[i] = row[i];
            }
            for (; i < ColCnt; ++i)
            {
                newRow[i] = "0";
            }
            return newRow;
        }
        private void InitHeader()
        {
            string tabFilePath = string.Format("{0}\\data\\source\\maps\\{1}minimap\\area.tab", RootDir, MapName);
            if (!File.Exists(tabFilePath))
            {
                throw new Exception(tabFilePath + "������");
            }
            string fileContent;
            fileContent = Helper.FileToString(tabFilePath).Replace("\r", "");

            string headRow = fileContent.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries)[0];

            TabFileHead = headRow.Split(new char[] { '\t' }, StringSplitOptions.RemoveEmptyEntries);

            
            for (int i = 0; i < TabFileHead.Length; ++i )
            {
                htHeader[TabFileHead[i]] = i;
            }

            ColCnt = htHeader.Count;
        }
        private void InitConfig()
        {
            string middlemap = null;
            string[] row = null;
            foreach (DictionaryEntry de in htAreaTabRows)
            {
                row = de.Value as string[];
                middlemap = row[Convert.ToInt32(htHeader["middlemap"])];

                if (!htConfigs.ContainsKey(middlemap))
                {
                    htConfigs[middlemap] = GetConfig(middlemap);
                }
                
            }
        }
        public void Transfer()
        {
            int id;
            string[] row = null;
            string[] assistrow = null;
            int psX;
            int psY;
            int logicalX = 0;
            int logicalY = 0;
            MiddleMapConfig config;
            int colIndex;
            string middlemap;

            foreach (DictionaryEntry de in htAreaTabRows)
            {
                id = Convert.ToInt32(de.Key);
                row = de.Value as string[];            
                
                assistrow = htAssistAreaTabRows[id] as string[];
                if (assistrow == null)
                {
                    continue;
                }

                colIndex = Convert.ToInt32(htHeader["x"]);
                psX = Convert.ToInt32(assistrow[colIndex]);
                colIndex = Convert.ToInt32(htHeader["y"]);
                psY = Convert.ToInt32(assistrow[colIndex]);

                colIndex = Convert.ToInt32(htHeader["middlemap"]);
                middlemap = row[colIndex];
                config = (htConfigs[middlemap]) as MiddleMapConfig;
                if (config == null)
                {
                    continue;
                }

                psXY2Logical(psX, psY, config, ref logicalX, ref logicalY);

                colIndex = Convert.ToInt32(htHeader["x"]);
                row[colIndex] = logicalX.ToString();
                colIndex = Convert.ToInt32(htHeader["y"]);
                row[colIndex] = logicalY.ToString();                
                
            }

        }
        public void Save()
        {
            StringBuilder sb = new StringBuilder();

            for (int i = 0; i < TabFileHead.Length; ++i)
            {                
                sb.Append(TabFileHead[i]);
                sb.Append('\t');
            }
            sb.Replace('\t', '\n', sb.Length - 1, 1);
            
            string[] row;
            ArrayList arrlist = new ArrayList(htAreaTabRows.Keys);
            arrlist.Sort();
            for (int j = 0; j < arrlist.Count; ++j)
            {                
                row = htAreaTabRows[arrlist[j]] as string[];
                for (int i = 0; i < row.Length; ++i)
                {
                    sb.Append(row[i]);
                    sb.Append('\t');
                }
                sb.Replace('\t', '\n', sb.Length - 1, 1);
            }

            string newFileContent = sb.ToString();

            string AreaFilePath = string.Format("{0}\\data\\source\\maps\\{1}minimap\\area.tab", RootDir, MapName);
            Helper.StringToFile(newFileContent, AreaFilePath, Encoding.GetEncoding("gb2312"));
        }
        private void psXY2Logical(int psX, int psY, MiddleMapConfig config, ref int logicalX, ref int logicalY)
        {
            int height = config.cHeight;
            double scale = config.cScale;
            double startx = config.cStartx;
            double starty = config.cStarty;

            logicalX = Convert.ToInt32(startx + psX / scale);
            logicalY = Convert.ToInt32(starty + (height - psY) / scale);
        }
        private MiddleMapConfig GetConfig(string middlemap)
        {
            string configFilePath = string.Format("{0}\\data\\source\\maps\\{1}minimap\\config.ini", RootDir, MapName);
            string Section = string.Format("middlemap{0}", middlemap);

            string value;
            string key;
            int height;
            double scale;
            double startx;
            double starty;

            //height
            key = "height";
            value = IniIO.IniReadValue(Section, key, configFilePath);
            if (value == "" || !Helper.CanConverToNumber(value))
            {
                throw new Exception(string.Format("{0}��{1}��{2}���ò���", configFilePath, Section, key));
            }
            height = Convert.ToInt32(value);
            //scale
            key = "scale";
            value = IniIO.IniReadValue(Section, key, configFilePath);
            if (value == "" || !Helper.CanConverToNumber(value))
            {
                throw new Exception(string.Format("{0}��{1}��{2}���ò���", configFilePath, Section, key));
            }
            scale = Convert.ToDouble(value);

            //startx
            key = "startx";
            value = IniIO.IniReadValue(Section, key, configFilePath);
            if (value == "" || !Helper.CanConverToNumber(value))
            {
                throw new Exception(string.Format("{0}��{1}��{2}���ò���", configFilePath, Section, key));
            }
            startx = Convert.ToDouble(value);

            //starty
            key = "starty";
            value = IniIO.IniReadValue(Section, key, configFilePath);
            if (value == "" || !Helper.CanConverToNumber(value))
            {
                throw new Exception(string.Format("{0}��{1}��{2}���ò���", configFilePath, Section, key));
            }
            starty = Convert.ToDouble(value);

            MiddleMapConfig config = new MiddleMapConfig();
            config.Init(height, scale, startx, starty);

            return config;
            
        }        
        
    }

    class MiddleMapConfig
    {
        public int cHeight;
        public double cScale;
        public double cStartx;
        public double cStarty;

        public void Init(int height, double scale, double startx, double starty)
        {
            cHeight = height;
            cScale = scale;
            cStartx = startx;
            cStarty = starty;
        }
    };
}
