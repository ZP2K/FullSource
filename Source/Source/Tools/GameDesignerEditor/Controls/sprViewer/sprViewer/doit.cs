using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Xml;
using System.Collections;
using System.IO;
using System.Drawing;
using System.Reflection;
using System.Data.SqlClient;
using System.Data;
using System.Drawing.Imaging;

using atlaxLib;

namespace sprViewer
{
    public class doit
    {
        private string strFileName = "";        
        private frmTest ft;

        /// <summary>
        /// ��ʼ��
        /// </summary>
        /// <returns>Control������</returns>
        public Control Init()
        {
            ft = new frmTest();
            return ft.panel1;
        }

        // �ִ���û�г�ʼ�������
        public void InitCacae(SqlConnection conn)
        {
            Helper.GetHelper().CurrentSqlConnection = conn;
            Helper.GetHelper().InitCacheTable();
        }

        public void UnInit()
        {            
            ft.timer1.Enabled = false;
            ft.Close();            
        }

        /// <summary>
        /// ����spr�ļ��������Զ�����Ԥ������
        /// </summary>
        /// <param name="strFileName">SPR�ļ���������ȫ·��</param>
        public void SetFileName(string strFileName, Test3DEngine engine)
        {
            if (strFileName.LastIndexOf('.') == -1)
            {
                throw (new Exception("�����ļ������Ϸ�"));
            }

            string strExt = strFileName.Substring(strFileName.LastIndexOf('.') + 1).ToLower();
            ft.timer1.Enabled = false;
            ft.labError.Visible = false;
            this.strFileName = strFileName;

            if(!File.Exists(strFileName))
            {
                ft.labError.Visible = true;
                ft.pb1.Image = new Bitmap(10, 10);
                return;
            }

            SPRItem si;
            if(strExt == "mdl")
            {
                si = new SPRItem(strFileName, SPRItem.PIC_TYPE.MDL, engine);
            }
            else if(strExt == "mtl")
            {
                si = new SPRItem(strFileName, SPRItem.PIC_TYPE.MTL, engine);
            }
            else if (strExt == "mesh")
            {
                si = new SPRItem(strFileName, SPRItem.PIC_TYPE.MESH, engine);
            }
            else if (strExt == "ani")
            {
                si = new SPRItem(strFileName, SPRItem.PIC_TYPE.ANI, engine);
            }
            else if (strExt == "tani")
            {
                si = new SPRItem(strFileName, SPRItem.PIC_TYPE.ANI, engine);
            }
            else //spr
            {
                si = new SPRItem(strFileName);
            }

            ft.sio = si;
            ft.timer1.Enabled = true;
        }

        /// <summary>
        /// ����UITex�ļ���
        /// </summary>
        /// <param name="strFileName"></param>
        /// <param name="nFrame">֡��</param>
        public void SetUtexFileName(string strFileName, int nFrame)
        {
            ft.timer1.Enabled = false;
            ft.labError.Visible = false;
            this.strFileName = strFileName;
            SPRItem si;

            if (!File.Exists(strFileName))
            {
                ft.labError.Visible = true;
                ft.pb1.Image = new Bitmap(10, 10);
                return;
            }

            si = new SPRItem(strFileName, nFrame);
            ft.sio = si;
            ft.timer1.Enabled = true;
        }
    
        public void SetItemInfo(
            string iName, int iColor, bool iCanStack, int iMax,             //��������
            int tGenre, int tBindType,                                      //������
            int maxExist,                                                   //���ӵ��
            string desc,                                                    //˵��
            bool canTrade, int price,                                       //�۸�
            string icoFileName, int icoFrame,                                //ͼ��
            int maxexisttime
            )
        {
            ft.timer1.Enabled = false;

            //ӳ��� for iColor
            Brush[] colorTable = { 
                new SolidBrush(Color.FromArgb(159, 159, 159)),    //��
                Brushes.White,                                  //��
                new SolidBrush(Color.FromArgb(20, 255, 20)),    //��
                new SolidBrush(Color.FromArgb(32, 160, 255)),   //��
                new SolidBrush(Color.FromArgb(150, 0, 255)),  //��
                new SolidBrush(Color.FromArgb(250, 150, 0)),  //��
                new SolidBrush(Color.FromArgb(0xB8, 0x86, 0x0B)), //��
                };
            //ӳ��� for tGenre ��������
            string[] genreTable = { "װ��", "ҩƷ", "�������", "����" ,"�Ķ�", "�ƺ�", "��Ƕ"};
            Font[] fontTable = { new Font("����", 11, FontStyle.Bold), new Font("����", 9) };

            //�о�
            int nRowLen = 18;

            //����desc��
            string strDesc1 = "";
            string strDesc2 = "";
            {
                string[] asp = { "<Text>text=\"", "\" font=", "</text>", @"\\\n" };
                string[] sr = desc.Replace("<text>","<Text>").Split(asp, StringSplitOptions.RemoveEmptyEntries);
                if (sr.Length == 4)
                {
                    strDesc1 = sr[0];
                    strDesc2 = sr[2];
                }
                else if (sr.Length == 2)
                {
                    if (sr[0].StartsWith("ʹ�ã�")
                        || sr[0].StartsWith("װ����")
                        || sr[0].StartsWith("Я����")
                        )
                    {
                        strDesc1 = sr[0];
                    }
                    else
                    {
                        strDesc2 = sr[0];
                    }
                }
            }

            //���
            int nWidth = 0;
            {
                Image img1 = new Bitmap(500, 500);
                Graphics g1 = Graphics.FromImage(img1);
                nWidth = 220;
                int i = 0;
                i = (int)g1.MeasureString(strDesc1, fontTable[1]).Width;
                nWidth = nWidth < i ? i : nWidth;
                i = (int)g1.MeasureString(strDesc2, fontTable[1]).Width;
                nWidth = nWidth < i ? i : nWidth;
                nWidth += 30;
                img1 = null;
                g1 = null;
            }

            //�߶�
            int nHeight = 0;
            {
                nHeight = nRowLen * 2 + 5;
                if (!(tGenre != 2 && tBindType < 2)) nHeight += nRowLen;
                if (maxExist >= 1) nHeight += nRowLen;
                if (canTrade) nHeight += nRowLen;
                nHeight += ((desc.Length - desc.ToLower().Replace("<text>", "").Length) / 6) * nRowLen;
                if (maxexisttime != 0) nHeight += nRowLen;
                string[] descs = desc.Split(new string[] {"\\n"}, StringSplitOptions.RemoveEmptyEntries);
                nHeight += (descs.Length - 1) * nRowLen;
            }

            //��ͼ��
            Image imgico;
            try
            {
                FileInfo fi = new FileInfo(icoFileName);
                Image imgico1 = tga.uitex2img(fi.DirectoryName, icoFileName, icoFrame);
                Bitmap b = new Bitmap(imgico1.GetThumbnailImage(48, 48, null, IntPtr.Zero));
                b.SetResolution(96, 96);
                imgico = b;
                nWidth += imgico.Width;
            }
            catch
            {
                imgico = null;
            }

            //���߶�
            int nPosy = 5;
            Image img = new Bitmap(nWidth, nHeight);
            Graphics g = Graphics.FromImage(img);
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
            //��ʼ��
            {
                //������
                g.FillRectangle(new SolidBrush(Color.FromArgb(0x29, 0x29, 0x29)), new Rectangle(0, 0, nWidth, nHeight));
                g.DrawRectangle(Pens.White, new Rectangle(0, 0, nWidth - 1, nHeight - 1));

                //��ͼ��
                if (imgico != null)
                    g.DrawImage(imgico, new Point(nWidth - imgico.Width - 5, 5));
                else
                    g.DrawString("�޷���ʾ", new Font("����", 9), Brushes.White, new Point(nWidth - 65, 5));


                //������
                {
                    //������
                    string strPaint = iName;
                    g.DrawString(strPaint, fontTable[0], colorTable[iColor], 5, nPosy);
                    int w = (int)g.MeasureString(strPaint, fontTable[0]).Width;
                    strPaint = "";
                    if (iCanStack && iMax > 1)
                    {
                        strPaint = "(" + iMax.ToString() + ")";
                    }
                    if (!iCanStack)
                    {
                        strPaint = "(" + iMax.ToString() + "��)";
                    }
                    g.DrawString(strPaint, fontTable[1], Brushes.White, w + 5, nPosy + 2);
                    nPosy += nRowLen;
                    strPaint = "";

                    //��������
                    if (tGenre == 2)
                    {
                        strPaint = "ʰȡ���";
                    }
                    else
                    {
                        if (tBindType == 2) strPaint = "ʹ�ú��";
                        if (tBindType == 3) strPaint = "ʰȡ���";
                    }
                    if (strPaint != "")
                    {
                        g.DrawString(strPaint, fontTable[1], Brushes.White, 12, nPosy);
                        nPosy += nRowLen;
                    }
                    strPaint = "";

                    //��Ψһ����
                    if (maxExist == 1) strPaint = "Ψһ";
                    if (maxExist > 1) strPaint = "Ψһ(" + maxExist + ")";
                    if (strPaint != "")
                    {
                        g.DrawString(strPaint, fontTable[1], Brushes.White, 12, nPosy);
                        nPosy += nRowLen;
                    }

                    //����������
                    strPaint = genreTable[tGenre];
                    {
                        g.DrawString(strPaint, fontTable[1], Brushes.White, 12, nPosy);
                        nPosy += nRowLen;
                    }
                    strPaint = "";

                    //������˵��1
                    strPaint = strDesc1;
                    if (strPaint != "")
                    {
                        g.DrawString(strPaint, fontTable[1], new SolidBrush(Color.FromArgb(20, 255, 20)), 12, nPosy);
                        nPosy += nRowLen;
                    }
                    strPaint = "";

                    //������˵��2
                    strPaint = strDesc2;
                    if (strPaint != "")
                    {
                        string[] strPaints = strPaint.Split(new string[] { "\\n" }, StringSplitOptions.RemoveEmptyEntries);
                        foreach (string _s in strPaints)
                        {
                            g.DrawString(_s, fontTable[1], Brushes.Yellow, 12, nPosy);
                            nPosy += nRowLen;
                        }
                    }
                    strPaint = "";

                    //������ɾ��ʱ��
                    strPaint = "����ɾ��ʱ�䣺";
                    if (maxexisttime != 0)
                    {
                        strPaint += maxexisttime.ToString();
                    }
                    else
                    {
                        strPaint = "";
                    }
                    if (strPaint != "")
                    {
                        g.DrawString(strPaint, fontTable[1], Brushes.White, 12, nPosy);
                        nPosy += nRowLen;
                    }
                    strPaint = "";

                    //���۸�
                    if (canTrade)
                    {
                        Stream s = Assembly.GetExecutingAssembly().GetManifestResourceStream("sprViewer.bg.png");
                        Image imgbg = Image.FromStream(s);
                        g.DrawImage(imgbg, nWidth - 85, nPosy);
                        string sm1 = string.Format("{0}", price / 10000);
                        string sm2 = string.Format("{0}", ((price - (price / 10000) * 10000) / 100));
                        string sm3 = string.Format("{0}", price - (price / 100) * 100);
                        float sm1w = g.MeasureString(sm1, fontTable[1]).Width;
                        float sm2w = g.MeasureString(sm2, fontTable[1]).Width;
                        float sm3w = g.MeasureString(sm3, fontTable[1]).Width;
                        g.DrawString(sm1, fontTable[1], Brushes.White, nWidth - 82 - sm1w, nPosy + 2);
                        g.DrawString(sm2, fontTable[1], Brushes.White, nWidth - 50 - sm2w, nPosy + 2);
                        g.DrawString(sm3, fontTable[1], Brushes.White, nWidth - 18 - sm3w, nPosy + 2);
                    }
                }
            }

            ft.pb1.Image = img;
            ft.fitContoner();
        }

        List<string> skillDescs = new List<string>();
        public void AddOneDescItem(string strItem)
        {
            int n = skillDescs.Count;
            skillDescs.Insert(n, strItem);
        }
        List<string> skillShortDescs = new List<string>();
        public void AddOneShortDescItem(string strItem)
        {
            int n = skillShortDescs.Count;
            skillShortDescs.Insert(n, strItem);
        }
        public void ClearList()
        {
            skillShortDescs.Clear();
            skillDescs.Clear();
        }
        public void SetSkillInfo(
            int nSkillID, SqlConnection conn, string RootDir,            
            string skillName,string skillSpecialDesc,
            string skilllevel, int count, string strMaxLevel
            )
        {
            ft.SetSkillInfo(nSkillID, conn, RootDir, skillName, skillDescs, skillSpecialDesc, skillShortDescs, skilllevel, count, strMaxLevel);
        }
    }



    public class SPRItem 
    {
        public string tgaFileName = ""; // ���ͼƬ�ļ�����·����������չ����
        public string tgaFolderName;
        public string sprFileName = ""; // ����ͼƬ�ļ�����·��������չ����
        public Hashtable TGAList = new Hashtable(); // ��Ԥ�����ļ��б�

        private DateTime time;
        private string cachePath = ""; // ����·��
        private int ipos = 1;
        private int utex_flag = 0;

        public bool m_forceRefresh = false;
        private static Hashtable saveImageTable = new Hashtable(); // �Ѿ�����ͼƬ�ļ������ݱ�
        private static string iconFolder; // ͼ���ļ���
        private Image iconImage;

        /// <summary>
        /// ͼ���ļ���
        /// </summary>
        public static string IconFolder
        {
            set
            {
                iconFolder = value;
            }
        }

        // for UIText
        public SPRItem(string utexFileName, int nframe, DateTime time)
        {
            utex_flag = 1;
            this.time = time;
            this.sprFileName = utexFileName;
            this.tgaFolderName = string.Format(@"{0}\_cache\utex\", Application.StartupPath);
            this.tgaFileName = string.Format("{0}{1}{2}", tgaFolderName, ConverFileName(utexFileName), nframe.ToString());                        
            loadUtex(utexFileName, nframe);
        }

        public SPRItem(string utexFileName, int nframe)
        {
            utex_flag = 1;
            this.time = File.GetLastWriteTime(utexFileName);
            this.sprFileName = utexFileName;
            this.tgaFolderName = string.Format(@"{0}\_cache\utex\", Application.StartupPath);
            this.tgaFileName = string.Format("{0}{1}{2}", tgaFolderName, ConverFileName(utexFileName), nframe.ToString());  
            loadUtex(utexFileName, nframe);
        }

        // for SPR 
        public SPRItem(string sprFileName, DateTime time)
        {
            this.time = time;
            this.sprFileName = sprFileName;
            this.tgaFolderName = string.Format(@"{0}\_cache\spr\", Application.StartupPath);
            this.tgaFileName = string.Format("{0}{1}", tgaFolderName, ConverFileName(sprFileName));            
            loadTGA(sprFileName);
        }

        public SPRItem(string sprFileName)
        {
            this.time = File.GetLastWriteTime(sprFileName);
            this.sprFileName = sprFileName;
            this.tgaFolderName = string.Format(@"{0}\_cache\spr\", Application.StartupPath);
            this.tgaFileName = string.Format("{0}{1}", tgaFolderName, ConverFileName(sprFileName));
            loadTGA(sprFileName);
        }

        // ��ҪԤ����ͼƬ����
        public enum PIC_TYPE
        {
            MDL = 0,
            MTL = 1,
            MESH = 2,
            ANI = 3,
            NULL = 4
        }

        public Image IconImage
        {
            get
            {
                return iconImage;
            }
        }

        private bool Init(string strFileName, PIC_TYPE pictype, Test3DEngine engine, bool bForceRefresh)
        {            
            m_forceRefresh = bForceRefresh;

            this.time = File.GetLastWriteTime(strFileName);
            this.sprFileName = strFileName;
            string strExt = Enum.GetName(typeof(PIC_TYPE), pictype).ToLower();
            this.cachePath = string.Format(@"{0}\_cache\{1}\", Application.StartupPath, strExt);
            this.tgaFolderName = this.cachePath;
            this.tgaFileName = string.Format("{0}{1}", this.cachePath, ConverFileName(strFileName));

            if (!Directory.Exists(cachePath))
            {
                Directory.CreateDirectory(cachePath);
            }

            int nFrameCount = 0; //GetFrameCount(strFileName) ˼���Ļ�ȡ֡�����ĺ���
            strFileName = strFileName.ToLower();

            if (strFileName.EndsWith(".mdl") || strFileName.EndsWith(".mesh"))
            {
                nFrameCount = 1;
            }
            else if (strFileName.EndsWith(".ani") || strFileName.EndsWith(".tani"))
            {
                nFrameCount = 20;
            }

            for (int i = 1; i <= nFrameCount; i++)
            {
                string ii = i.ToString();

                while (ii.Length < 3)
                {
                    ii = "0" + ii;
                }
                
                TGAList[i.ToString()] = string.Format("{0}{1}.JPG", this.tgaFileName, ii);
            }

            string imageFileName = string.Format("{0}001.jpg", tgaFileName);

            switch (FileChanged())
            {
                case -1:
                    {
                        Image image = Helper.GetHelper().GetImageFromCache(GetTGAFileName(), "TGA");

                        if (image != null)
                        {
                            if (!File.Exists(imageFileName))
                            {
                                File.Create(imageFileName).Close();

                                if (saveImageTable[imageFileName] == null)
                                {
                                    try
                                    {
                                        image.Save(imageFileName, System.Drawing.Imaging.ImageFormat.Jpeg);
                                    }
                                    catch (ExternalException ex)
                                    {
                                        Helper.GetHelper().RecordLog(string.Format("����ͼƬ�ļ�{0}����{1}", imageFileName, ex.Message));
                                    }
                                    finally
                                    {
                                        saveImageTable[imageFileName] = "1";
                                    }                                                                        
                                }
                            }
                        }

                        iconImage = image;

                        break;
                    }
                case 0:
                    {
                        if (!File.Exists(imageFileName)) // ����ͼƬ�ļ������ڵĻ���Ҫ��������
                        {
                            Image image = Helper.GetHelper().GetImageFromCache(GetTGAFileName(), "TGA");

                            if (image != null)
                            {
                                File.Create(imageFileName).Close();

                                if (saveImageTable[imageFileName] == null)
                                {
                                    try
                                    {
                                        image.Save(imageFileName, System.Drawing.Imaging.ImageFormat.Jpeg);
                                    }
                                    catch (ExternalException ex)
                                    {
                                        ;
                                    }
                                    finally
                                    {
                                        saveImageTable[imageFileName] = "1";
                                    }                                                                        
                                }
                            }

                            iconImage = image;
                        }

                        break;
                    }
                case 1:
                    {
                        try
                        {
                            engine.FileToImage(strFileName, this.tgaFileName);
                        }
                        catch(Exception ex)
                        {
                            Helper.GetHelper().RecordLog(string.Format("����ͼƬ�ļ�{0}����{1}", imageFileName, ex.Message));
                        }
                        finally
                        {
                            saveImageTable[imageFileName] = "1";
                        }

                        Helper.GetHelper().UpdateIcon(GetTGAFileName(), "TGA", imageFileName, GetLastWriteTime());  

                        break;
                    }
            }

            return true;
        }

        //for other
        public SPRItem(string strFileName, PIC_TYPE pictype, Test3DEngine engine)
        {
            Init(strFileName, pictype, engine, false);
        }
        public SPRItem(string strFileName, PIC_TYPE pictype, Test3DEngine engine, bool bForceRefresh)
        {
            Init(strFileName, pictype, engine, bForceRefresh);
        }


        public Image getNextImage()
        {
            if (TGAList.Count + 1 == ipos)
            {
                ipos = 1;
            }

            string spos = ipos.ToString();
            while (spos.Length < 3)
            {
                spos = "0" + spos;
            }

            Image img = null;

            try
            {
                string _strFile;
                if (utex_flag == 1)
                    _strFile = TGAList["1"].ToString();
                else
                    _strFile = string.Format("{0}{1}.JPG", this.tgaFileName, spos);

                if (File.Exists(_strFile))
                    img = Image.FromFile(_strFile);
            }
            finally
            {
                if (img == null)
                {
                    img = new Bitmap(10, 10);
                }                
            }

            ipos++;

            return img ;
        }


        private void loadUtex(string FileName, int nFrame)
        {
            cachePath = string.Format(@"{0}\_cache\utex\", Application.StartupPath);
            tgaFolderName = cachePath;

            if (!Directory.Exists(cachePath))
            {
                Directory.CreateDirectory(cachePath);
            }

            TGAList["1"] = string.Format("{0}.JPG", this.tgaFileName);
            FileInfo fi = new FileInfo(FileName);

            string imageFileName = string.Format("{0}.JPG", tgaFileName);

            switch (FileChanged())
            {
                case -1:
                    {
                        Image image = Helper.GetHelper().GetImageFromCache(GetTGAFileName(), "TGA");

                        if (image != null)
                        {
                            if (!File.Exists(imageFileName))
                            {
                                File.Create(imageFileName).Close();

                                if (saveImageTable[imageFileName] == null)
                                {
                                    try
                                    {
                                        image.Save(imageFileName, System.Drawing.Imaging.ImageFormat.Jpeg);
                                    }
                                    catch (ExternalException ex)
                                    {
                                        Helper.GetHelper().RecordLog(string.Format("����ͼƬ�ļ�{0}����{1}", imageFileName, ex.Message));
                                    }
                                    finally
                                    {
                                        saveImageTable[imageFileName] = "1";
                                    }                                                                       
                                }
                            }
                        }

                        iconImage = image;

                        break;
                    }
                case 0:
                    {
                        if (!File.Exists(imageFileName)) // ����ͼƬ�ļ������ڵĻ���Ҫ��������
                        {
                            Image image = Helper.GetHelper().GetImageFromCache(GetTGAFileName(), "TGA");

                            if (image != null)
                            {
                                File.Create(imageFileName).Close();

                                if (saveImageTable[imageFileName] == null)
                                {
                                    try
                                    {
                                        image.Save(imageFileName, System.Drawing.Imaging.ImageFormat.Jpeg);
                                    }
                                    catch (ExternalException ex)
                                    {
                                        Helper.GetHelper().RecordLog(string.Format("����ͼƬ�ļ�{0}����{1}", imageFileName, ex.Message));
                                    }
                                    finally
                                    {
                                        saveImageTable[imageFileName] = "1";
                                    }                                                                        
                                }
                            }

                            iconImage = image;
                        }

                        break;
                    }
                case 1:
                    {
                        Image img = null;

                        try
                        {
                            img = tga.uitex2img(fi.DirectoryName, FileName, nFrame);

                            if (img != null)
                            {
                                img.Save(imageFileName, System.Drawing.Imaging.ImageFormat.Jpeg);
                            }                            
                        }
                        catch(Exception ex)
                        {
                            Helper.GetHelper().RecordLog(string.Format("����ͼƬ�ļ�{0}����{1}", imageFileName, ex.Message));
                        }
                        finally
                        {                            
                            saveImageTable[imageFileName] = "1";
                        }

                        iconImage = img;

                        if (img != null)
                        {
                            Helper.GetHelper().UpdateIcon(GetTGAFileName(), "TGA", imageFileName, GetLastWriteTime());
                        }                        

                        break;
                    }
            }
        }

        private void loadTGA(string FileName)
        {            
            cachePath = string.Format(@"{0}\_cache\spr\", Application.StartupPath);
            tgaFolderName = cachePath;

            if (!Directory.Exists(cachePath))
            {
                Directory.CreateDirectory(cachePath);
            }

            int nFrameCount = spr2tga.GetSPRFrameCount(FileName);
            for (int i = 1; i <= nFrameCount; i++)
            {
                string ii = i.ToString();
                while (ii.Length < 3)
                {
                    ii = "0" + ii;
                }
                TGAList[i.ToString()] = string.Format("{0}{1}.JPG", this.tgaFileName, ii);
            }

            string imageFileName = string.Format("{0}.JPG", tgaFileName);

            switch (FileChanged())
            {
                case -1:
                    {
                        Image image = Helper.GetHelper().GetImageFromCache(GetTGAFileName(), "TGA");

                        if (image != null)
                        {
                            if (!File.Exists(imageFileName))
                            {
                                File.Create(imageFileName).Close();

                                if (saveImageTable[imageFileName] == null)
                                {
                                    try
                                    {
                                        image.Save(imageFileName, System.Drawing.Imaging.ImageFormat.Jpeg);
                                    }
                                    catch (ExternalException ex)
                                    {
                                        Helper.GetHelper().RecordLog(string.Format("����ͼƬ�ļ�{0}����{1}", imageFileName, ex.Message));
                                    }
                                    finally
                                    {
                                        saveImageTable[imageFileName] = "1";
                                    }                                                                        
                                }
                            }
                        }

                        iconImage = image;

                        break;
                    }
                case 0:
                    {
                        if (!File.Exists(imageFileName)) // ����ͼƬ�ļ������ڵĻ���Ҫ��������
                        {
                            Image image = Helper.GetHelper().GetImageFromCache(GetTGAFileName(), "TGA");

                            if (image != null)
                            {
                                File.Create(imageFileName).Close();

                                if (saveImageTable[imageFileName] == null)
                                {
                                    try
                                    {
                                        image.Save(imageFileName, System.Drawing.Imaging.ImageFormat.Jpeg);
                                    }
                                    catch (ExternalException ex)
                                    {
                                        Helper.GetHelper().RecordLog(string.Format("����ͼƬ�ļ�{0}����{1}", imageFileName, ex.Message));
                                    }
                                    finally
                                    {
                                        saveImageTable[imageFileName] = "1";
                                    }                                                                        
                                }
                            }
                            
                            iconImage = image;
                        }                        

                        break;
                    }
                case 1:
                    {
                        try
                        {
                            spr2tga.SaveSPRToFile(FileName, -1, true, 0, this.tgaFileName, "JPG");
                        }
                        catch (Exception ex)
                        {
                            Helper.GetHelper().RecordLog(string.Format("����ͼƬ�ļ�{0}����{1}", imageFileName, ex.Message));
                        }
                        finally
                        {
                            saveImageTable[imageFileName] = "1";
                        }

                        Helper.GetHelper().UpdateIcon(GetTGAFileName(), "TGA", imageFileName, GetLastWriteTime());

                        break;
                    }
            }
        }

        /// <summary>
        /// ��ȡ�ļ��Ĺ�ϣֵ
        /// </summary>
        /// <returns>�ļ��Ĺ�ϣֵ</returns>
        private string ConverFileName(string fileName)
        {
            string newFileName = fileName;

            if (!string.IsNullOrEmpty(iconFolder)) // ֻȡ���·��
            {
                newFileName = fileName.Replace(iconFolder, "").TrimStart(new char[] { '\\' });
            }
                        
            newFileName = newFileName.Replace("\\", "_");
            newFileName = newFileName.Replace(":", "_"); // Ԥ�����ⲿ����û�и��ļ��и�ֵ������

            return newFileName;
        }

        /// <summary>
        /// ��ȡTGA�ļ���
        /// </summary>
        /// <returns>TGA�ļ���</returns>
        private string GetTGAFileName()
        {
            string[] dataArray = tgaFileName.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
            string fileName = dataArray[dataArray.Length - 1];

            return fileName;
        }

        /// <summary>
        /// ��ȡͼƬ�ļ��ϴ�д���ʱ��
        /// </summary>
        /// <returns>�ϴ�д���ʱ��</returns>
        private DateTime GetLastWriteTime()
        {
            DateTime lastWriteTime = time;

            if (sprFileName.Contains(".UITex")) // UITex�ļ���Ҫ���⴦����Ϊ��������TGA�ļ�
            {
                string referenceFileName = tga.GetTGAFilePath(sprFileName);

                if (referenceFileName != null)
                {
                    string newFileName = "";

                    string[] dataArray = sprFileName.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
                    
                    for (int i = 0; i < dataArray.Length; i++)
                    {
                        if (i != dataArray.Length - 1)
                        {
                            newFileName += string.Format("{0}\\", dataArray[i]);
                        }
                        else
                        {
                            newFileName += referenceFileName;
                        }
                    }

                    DateTime referenceFileTime = File.GetLastWriteTime(newFileName);

                    if (referenceFileTime > time)
                    {
                        lastWriteTime = referenceFileTime;
                    }
                }
            }

            return lastWriteTime;
        }

        /// <summary>
        /// ���TGA�ļ��Ƿ�ı��
        /// ����ֵ-1����ǰͼƬд��ʱ������ݿ��ﱣ���ͼƬ��ʱ��Ҫ�ɣ������ݿ���ȡ�����ļ�
        /// ����ֵ0����ǰͼƬд��ʱ������ݿ��ﱣ���ͼƬʱ����ȣ����ý����κβ���
        /// ����ֵ1����ǰͼƬд��ʱ������ݿ��ﱣ���ͼƬʱ��Ҫ�£����µ����ݿ����
        /// </summary>
        /// <returns>�Ƚ�ֵ</returns>
        private int FileChanged()
        {
            // ǿ��ˢ��
            if (m_forceRefresh)
            {
                return 1;
            }            
           
            return Helper.GetHelper().RequireUpdateCache(GetTGAFileName(), "TGA", GetLastWriteTime());
        }
    };

    public static class spr2tga
    {
        [DllImport("SPR2TGA.dll", EntryPoint = "GetSPRFrameCount")]
        public static extern int GetSPRFrameCount(string strFilename);

        [DllImport("SPR2TGA.dll", EntryPoint = "SaveSPRToTGA")]
        public static extern bool SaveSPRToTGA(string strSprFileName, string strDescFileName, string FileExt);

        [DllImport("SPR2TGA.dll", EntryPoint = "SaveSPRToFile")]
        public static extern bool SaveSPRToFile(string src_path, int frame_index, bool premodulate, int color, string dst_path, string dst_ext);
    };

    public static class tga
    {
        private static Point rectangleLocation = new Point(0, 0); // ����λ�ã����ⴴ�����
        private static Size rectangleSize = new Size(0, 0); // ���δ�С�����ⴴ�����

        /// <summary>
        /// ��ȡTGA�ļ�·��
        /// </summary>
        /// <param name="uitexFileName">UITex�ļ�·��</param>
        /// <returns>TGA�ļ�·��</returns>
        public static string GetTGAFilePath(string uitexFileName)
        {
            FileStream fileStream = File.Open(uitexFileName, FileMode.Open);
            BinaryReader binaryReader = new BinaryReader(fileStream);

            // ���ļ���
            binaryReader.BaseStream.Seek(0x18, System.IO.SeekOrigin.Begin);
            byte[] buffer = binaryReader.ReadBytes(64);
            string tgaFilePath = System.Text.Encoding.Default.GetString(buffer);
            tgaFilePath = tgaFilePath.Substring(0, tgaFilePath.IndexOf('\0'));

            fileStream.Close();

            return tgaFilePath;
        }

        /// <summary>
        /// UITexתImg
        /// </summary>
        /// <param name="strIconDir">UITex��TGA����Ŀ¼��һ���UITexĿ¼��ͬ</param>
        /// <param name="filename">����UITex�ļ�������·����</param>
        /// <param name="frame">�ڼ�֡</param>
        /// <returns></returns>
        public static Image uitex2img(string strIconDir, string filename, int frame)
        {
            //��ȡUITex�ļ�����������ǰ֡����ͳ���
            BinaryReader b = new BinaryReader(File.Open(filename, FileMode.Open));
            /*���ļ���*/
            byte[] tb = new byte[64];
            b.BaseStream.Seek(0x18, System.IO.SeekOrigin.Begin);

            tb = b.ReadBytes(64);

            filename = System.Text.Encoding.Default.GetString(tb);            
            filename = string.Format(@"{0}\{1}", strIconDir, filename.Substring(0, filename.IndexOf('\0')));

            b.BaseStream.Seek(0x0c, System.IO.SeekOrigin.Begin);
            /*֡����*/
            int allframe = b.ReadInt32();
            if (frame > allframe - 1)
            {
                b.Close();
                return null;
            }

            /*��֡���ꡢ�߿�*/
            b.BaseStream.Seek(0x58 + frame * 0x14, SeekOrigin.Begin);
            int dwInfo = b.ReadInt32();
            int nLeft = b.ReadInt32();
            int nTop = b.ReadInt32();
            int nWidth = b.ReadInt32();
            int nHeight = b.ReadInt32();
            b.Close();

            // �ü�ͼƬ
            rectangleLocation.X = nLeft;
            rectangleLocation.Y = nTop;
            rectangleSize.Width = nWidth;
            rectangleSize.Height = nHeight;
            Rectangle rectangle = new Rectangle(rectangleLocation, rectangleSize);
            Bitmap bmp = tga2img(filename) as Bitmap;
            Image cloneImage = null;
            Image image = null;

            if (bmp.Width != nWidth || bmp.Height != nHeight)
            {
                bool cloneSuccess = true;

                try
                {
                    cloneImage = bmp.Clone(rectangle, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
                }
                catch (OutOfMemoryException ex)
                {
                    cloneSuccess = false;
                    cloneImage = bmp; // �������ԭͼ��
                }

                if (cloneSuccess)
                {
                    bmp.Dispose();
                }                
            }
            else
            {
                cloneImage = bmp;
            }
                          
            if (cloneImage != null)
            {
                /*
                // ��¼������Ϣ�����ļ�
                ImageCodecInfo myImageCodecInfo = GetEncoderInfo("image/jpeg");
                Encoder myEncoder = Encoder.Quality;
                EncoderParameters myEncoderParameters = new EncoderParameters(1);
                EncoderParameter myEncoderParameter = new EncoderParameter(myEncoder, 100L);
                myEncoderParameters.Param[0] = myEncoderParameter;

                MemoryStream ms = new MemoryStream();
                cloneImage.Save(ms, myImageCodecInfo, myEncoderParameters);
                cloneImage.Dispose();
                image = Image.FromStream(ms);
                */

                image = cloneImage;
            }

            return image;
        }

        private static ImageCodecInfo GetEncoderInfo(String mimeType)
        {
            int j;
            ImageCodecInfo[] encoders;
            encoders = ImageCodecInfo.GetImageEncoders();

            for (j = 0; j < encoders.Length; ++j)
            {
                if (encoders[j].MimeType == mimeType)
                {
                    return encoders[j];
                }
            }

            return null;
        }

        /// <summary>
        /// TGAתImage
        /// </summary>
        /// <param name="tgaFilename">TGA�����ļ���</param>
        /// <returns>Image</returns>
        public static Image tga2img(string tgaFilename)
        {
            Paloma.TargaImage tgaImage = new Paloma.TargaImage(tgaFilename);
            return tgaImage.Image;

            //// ��ȡTGA�ļ�������������
            //BinaryReader b = new BinaryReader(File.Open(tgaFilename, FileMode.Open));
            //b.BaseStream.Seek(0x0C, System.IO.SeekOrigin.Begin);
            //int nWidth = b.ReadInt16();
            //int nHeight = b.ReadInt16();
            //b.Close();

            //// ��tgaPreview���jpg�ļ�
            //string tempFilePath = Path.GetTempFileName();

            //Panel panel = new Panel();
            //tgaPreview.Init(panel.Handle);
            //tgaPreview.SetFileNames(panel.Handle, tgaFilename + "\r\n", (UInt16)nWidth, (UInt16)nHeight);
            //tgaPreview.Render(panel.Handle);
            //tgaPreview.FrameMove(panel.Handle);
            //tgaPreview.ExportToFile(panel.Handle, (UInt16)nWidth, (UInt16)nHeight, tempFilePath);
            //tgaPreview.UnInit(panel.Handle);

            //// ����jpg�ļ�  
            //FileStream fs = new FileStream(tempFilePath, FileMode.Open, FileAccess.Read);
            //int byteLength = (int)fs.Length;
            //byte[] wf = new byte[byteLength];
            //fs.Read(wf, 0, byteLength);
            //fs.Close();
            //Image img = Image.FromStream(new MemoryStream(wf));
            //File.Delete(tempFilePath);

            //return img;
        }
    }  
    
    /// <summary>
    /// tgaPreview.dll ����
    /// </summary>
    public static class tgaPreview
    {
        [DllImport("TGAPreview.dll")]
        public static extern void Init(IntPtr hRenderWnd);

        [DllImport("TGAPreview.dll")]
        public static extern void UnInit(IntPtr hRenderWnd);

        [DllImport("TGAPreview.dll")]
        public static extern void SetFileNames(IntPtr hRenderWnd, string Filenames, UInt16 nWidth, UInt16 nHeight);

        [DllImport("TGAPreview.dll")]
        public static extern void OnResize(IntPtr hRenderWnd);

        [DllImport("TGAPreview.dll")]
        public static extern void FrameMove(IntPtr hRenderWnd);  //�ɵ�����ÿ��10֡����ÿ�����10��

        [DllImport("TGAPreview.dll")]
        public static extern void Render(IntPtr hRenderWnd);     //ÿ��30�ε���

        [DllImport("TGAPreview.dll")]
        public static extern bool ExportToFile(IntPtr hRenderWnd, UInt16 nWidth, UInt16 nHeight, string Filenames);
    };
}
