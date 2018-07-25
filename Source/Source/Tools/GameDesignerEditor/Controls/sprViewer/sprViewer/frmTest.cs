using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.IO;
namespace sprViewer
{
    public partial class frmTest : Form
    {
        public SPRItem sio;
        public frmTest()
        {
            InitializeComponent();
        }

        private void frmTest_Load(object sender, EventArgs e)
        {
            pb1.Image = new Bitmap(100, 100);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {            
            try
            {
                pb1.Image = sio.getNextImage();
            }
            catch
            {
                pb1.Image = new Bitmap(100, 100);
                timer1.Enabled = false;
            }   
        }

        private void panel1_SizeChanged(object sender, EventArgs e)
        {

        }

        public void fitContoner()
        {
            if (pb1.Image == null)
                return;
            if(pb1.Image.Height <= panel1.Height && pb1.Image.Width <= panel1.Width)
            {
                //��������ͼƬ
                int lx = -(pb1.Image.Width - panel1.Width) / 2;
                int ly = -(pb1.Image.Height - panel1.Height) / 2;
                pb1.Location = new System.Drawing.Point(lx, ly);
                pb1.Width = pb1.Image.Width;
                pb1.Height = pb1.Image.Height;
            }
            else
            {
                //����װ����ͼƬ����Ҫ����
                if ((float)pb1.Image.Height / (float)pb1.Image.Width > (float)panel1.Height / (float)panel1.Width) 
                {
                    //��panel1�ĸ�
                    pb1.Width = pb1.Image.Width * panel1.Height / pb1.Image.Height;
                    pb1.Height = panel1.Height;
                    int lx = (panel1.Width - pb1.Width) / 2;
                    pb1.Location = new System.Drawing.Point(lx, 0);
                }
                else
                {
                    //��panel1�Ŀ�
                    pb1.Height = pb1.Image.Height * panel1.Width / pb1.Image.Width;
                    pb1.Width = panel1.Width;
                    int ly = (panel1.Height - pb1.Height) / 2;
                    pb1.Location = new System.Drawing.Point(0, ly);
                }
            }
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            fitContoner();
        }


        //the begin of sunyong's
       
       
        string m_skillName;
        List<string> m_listDesc = new List<string>();
        List<string> m_listShortDesc = new List<string>();       
        string m_skillSpecialDesc;
        string m_strMaxLevel;

       /* int nSkillID, SqlConnection conn, string RootDir,
           string skillName, string skillDesc, string skillSpecialDesc, string skillShortDesc, string skilllevel*/
        public void SetSkillInfo(int nSkillID, SqlConnection conn, string RootDir,
           string skillName, List<string> skillDescs, string skillSpecialDesc, List<string> skillShortDescs,
           string skilllevel, int count, string strMaxLevel
           )
        {
           
            
            m_skillName = skillName;
           
            
            m_skillSpecialDesc = skillSpecialDesc;
            m_listDesc = skillDescs;
            m_listShortDesc = skillShortDescs;
            m_strMaxLevel = strMaxLevel;

            int nMaxLevel = -1; 
            nMaxLevel = Convert.ToInt32(strMaxLevel);
            if (count == 0)
            {
                return;
            }
            else if (count == 1)
            {
                string strDesc = m_listDesc[0];
                string strShortDesc = m_listShortDesc[0];
                string strDescTemp = string.Empty;
                string strShortDescTemp = string.Empty;
                strDesc = strDesc.Replace("\\n", "\n");
                strShortDesc = strShortDesc.Replace("\\n", "\n");
                m_listShortDesc.Clear();
                m_listDesc.Clear();                
                for (int i = 1; i <= nMaxLevel; i++)
                {
                    strDescTemp = strDesc;
                    strShortDescTemp = strShortDesc;
                    ReplaceTag(ref strDescTemp, nSkillID, conn, RootDir, i);
                    ReplaceTag(ref strShortDescTemp, nSkillID, conn, RootDir, i + 1);
                    m_listDesc.Add(strDescTemp);
                    m_listShortDesc.Add(strShortDescTemp);
                }
            }
            else if (count > 1)
            {                
                if (count != nMaxLevel)
                {
                   // MessageBox.Show("���ȼ��ͼ�¼������һ��", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                string strDesc = string.Empty;
                string strShortDesc = string.Empty;
                for (int i = 0; i < nMaxLevel; i++)
                {
                    strDesc = m_listDesc[i];
                    strShortDesc = m_listShortDesc[i];
                    strDesc = strDesc.Replace("\\n", "\n");
                    strShortDesc = strDesc.Replace("\\n", "\n");
                    ReplaceTag(ref strDesc, nSkillID, conn, RootDir, i);
                    ReplaceTag(ref strShortDesc, nSkillID, conn, RootDir, i + 1);
                    m_listDesc[i] = strDesc;
                    m_listShortDesc[i] = strShortDesc;
                }
            }
            InitPanel1ForSkill(nMaxLevel, skilllevel);     
        }

        private void ReplaceTag(ref string strWithTag, int nSkillID, SqlConnection conn, string RootDir, int nLevel)
        {
            int nBegin = -1;
            int nNextBegin = -1;
            int nEnd = -1;
            string strTag = string.Empty;
            string strReplace = string.Empty;

            nBegin = strWithTag.IndexOf('<');
            while (nBegin != -1)
            {
                nEnd = strWithTag.IndexOf('>', nBegin);
                nNextBegin = strWithTag.IndexOf('<', nBegin + 1);
                if (nEnd > nNextBegin && nNextBegin != -1)
                {
                    nBegin = nNextBegin;
                    continue;
                }
                if (nEnd == -1)
                {
                    break;
                }
                strTag = strWithTag.Substring(nBegin, nEnd - nBegin + 1);
                strReplace = Tag2Replace(strTag, nSkillID, conn, RootDir, nLevel);
                strWithTag = strWithTag.Replace(strTag, strReplace);
                nBegin = strWithTag.IndexOf('<');
            }
        }

        private string Tag2Replace(string strTag, int nSkillID, SqlConnection conn, string RootDir, int nLevel)
        {
            string strReplace = string.Empty;
            if (strTag.Contains("SKILL"))
            {
                strReplace = GetSKILLReplace(strTag, nSkillID, conn, RootDir, nLevel);
            }
            else if (strTag.Contains("BUFF"))
            {
                strReplace = GetBUFFReplace(strTag, conn, nLevel);
            }
            else if (strTag.Contains("SUB"))
            {
                strReplace = GetSUBReplace(strTag, conn, nLevel);
            }
            else
            {
                strReplace = "[δ֪��ǩ]";
            }
            return strReplace;
        }

        /// <summary>
        /// ��ǩ SUB 409 0 ����滻
        /// </summary>
        /// <param name="strTag"></param>
        /// <param name="conn"></param>
        /// <returns></returns>
        private string GetSUBReplace(string strTag, SqlConnection conn, int nLevel)
        {
            strTag = strTag.Replace('\t', ' ');

            string strSperator = " \t<>";
            string[] strSUBTag = strTag.Split(strSperator.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
            
            if(strSUBTag.Length != 3)
            {
                return "[δ֪SUB]";
            }
            int theLevel = -1;
            theLevel = Convert.ToInt32(strSUBTag[2]);
            if (theLevel == 0)
            {
                theLevel = nLevel;
            }
            string strReplace = string.Empty;
            string cmdText = string.Empty;
            cmdText = string.Format("select [Desc] from ui_skill where skillID = '{0}' and [Level] = {1}", strSUBTag[1], theLevel);
            if (conn.State == System.Data.ConnectionState.Closed)
            {
                conn.Open();
            }
            SqlCommand cmd = new SqlCommand(cmdText, conn);
            SqlDataReader dr = cmd.ExecuteReader();
            if (dr.Read())
            {
                strReplace = dr[0].ToString();
            }
            else
            {
                int nBegin = cmdText.IndexOf("and");

                cmdText = cmdText.Remove(nBegin);
                cmd.CommandText = cmdText;
                dr.Close();
                dr = cmd.ExecuteReader();
                if (dr.Read())
                {
                    strReplace = dr[0].ToString();
                }
            }
            dr.Close();
            conn.Close();
            return strReplace;

        }

        /// <summary>
        /// ��ȡ��ǩ��buff .. . .. .�����滻ֵ
        /// </summary>
        /// <param name="strTag"></param>
        /// <param name="conn"></param>
        /// <returns></returns>
        private string GetBUFFReplace(string strTag, SqlConnection conn, int nLevel)
        {
            strTag = strTag.Replace('\t', ' ');

            int theLevel = -1;
            string strReplace = string.Empty;
            string strBuffID = string.Empty;
            string strSperator = " \t<>";
            string[] strBuffTag = strTag.Split(strSperator.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);

            if (strBuffTag.Length == 4)
            {
                theLevel = Convert.ToInt32(strBuffTag[2]);
                if (theLevel == 0)
                {
                    theLevel = nLevel;
                }
                strReplace = SelectFromui_buff(strBuffTag, conn, theLevel);

                string strSpecialBuff = string.Empty;
                string strSelectFromNewBuff = string.Empty;
                strSpecialBuff = HasSpecialBuff(strReplace);
                string strSpecialBuffID = strBuffTag[1];
                while (strSpecialBuff != string.Empty)
                {
                    string[] strBuffTag2 = strSpecialBuff.Split(strSperator.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                    strSelectFromNewBuff = SelectFromNewBuff(strBuffTag2[1], conn, nLevel, strSpecialBuffID);
                    strReplace = strReplace.Replace(strSpecialBuff, strSelectFromNewBuff);
                    strSpecialBuff = HasSpecialBuff(strReplace);
                }               
            }         
            else
            {
                strReplace = "[δ֪BUFF]";
            }
            return strReplace;
        }

        /// <summary>
        /// �ж��Ƿ��������BUFF ��ǩ��������򷵻ظñ�ǩ�����򷵻ؿ��ַ���
        /// </summary>
        /// <param name="strTest"></param>
        /// <returns></returns>
        private string HasSpecialBuff(string strTest)
        {
            int nBegin = strTest.IndexOf("BUFF");
            int nEnd = -1;
            string strReturn = string.Empty;
            
            if (nBegin != -1)
            {
                nBegin = strTest.LastIndexOf('<', nBegin);
                if (nBegin != -1)
                {
                    nEnd = strTest.IndexOf('>', nBegin);
                    strReturn = strTest.Substring(nBegin, nEnd - nBegin + 1);
                    string strSperator = " \t<>";
                    string[] strBuffTag = strReturn.Split(strSperator.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                    if (strBuffTag.Length != 2)
                    {
                        strReturn = string.Empty;
                    }
                }
                
            }
            return strReturn;
        }
        /// <summary>
        /// ��ǩ��buff 513 1 name��֮��Ĵӱ�ui_skill��ѡȡ�滻ֵ
        /// ��֪�����������name,desc,time
        /// </summary>
        /// <param name="cmdText"></param>
        /// <param name="conn"></param>
        /// <returns></returns>
        private string SelectFromui_buff(string[] strBuffTag, SqlConnection conn, int nLevel)
        {
            string strReplace = string.Empty;
            string cmdText = string.Empty;

            if (conn.State == System.Data.ConnectionState.Closed)
            {
                conn.Open();
            }
            SqlCommand cmd = conn.CreateCommand();
            switch (strBuffTag[3])
            {
                case "name":
                    cmdText = string.Format("select Name from ui_buff where BuffID = '{0}' and [Level] = {1}", strBuffTag[1], nLevel);
                    break;
                case "desc":
                    cmdText = string.Format("select [Desc] from ui_buff where BuffID = '{0}' and [Level] = {1}", strBuffTag[1], nLevel);
                    break;
                case "time":
                    cmdText = string.Format("select [count] * [interval] as Time from tbl_NewBuff where ID = '{0}' and [Level] = {1}", strBuffTag[1], nLevel);
                    break;
                default:
                    return "[δ֪buff����]";
            }
            cmd.CommandText = cmdText;
            SqlDataReader dr = cmd.ExecuteReader();
            if (dr.Read())
            {
                strReplace = dr[0].ToString();
            }
            else
            {
                int nBegin = cmdText.IndexOf("and");
                
                cmdText = cmdText.Remove(nBegin);
                cmd.CommandText = cmdText;
                dr.Close();
                dr = cmd.ExecuteReader();
                if (dr.Read())
                {
                    strReplace = dr[0].ToString();
                }                
            }
            
            dr.Close();
            conn.Close();
            if (strBuffTag[3] == "time")
            {
                strReplace = FormatTime(strReplace);
            }

            return strReplace;
        }

        /// <summary>
        /// ת��֡����ʾ��ʱ��ΪСʱ�������ʽʱ��
        /// </summary>
        /// <param name="strTime">֡����1�� = 16֡��</param>
        /// <returns></returns>
        private string FormatTime(string strTime)
        {
            string strResult = string.Empty;
            int nTime = Convert.ToInt32(strTime);
            int nRemain = 0;
            nTime /= 16;
            
            nRemain = nTime % 60;
            if (nRemain > 0)
            {
                strResult = string.Format("{0}{1}��", strResult, nRemain);
            }
            nTime /= 60;
            nRemain = nTime % 60;
            if (nRemain > 0)
            {
                strResult = string.Format("{0}{1}����", strResult, nRemain);
            }
            nTime /= 60;            
            if (nTime > 0)
            {
                strResult = string.Format("{0}{1}Сʱ", strResult, nTime);
            }
            return strResult;
            
        }
        /// <summary>
        /// ��ǩΪ��BUFF atCallPhysicsDamage������ʱ��tbl_NewBuff��ѡȡ��ֵ
        /// </summary>
        /// <param name="strConst">���磺atCallPhysicsDamage</param>
        /// <param name="conn"></param>
        /// <returns></returns>
        private string SelectFromNewBuff(string strConst, SqlConnection conn, int nLevel, string strBuffID)
        {
            if (nLevel > Convert.ToInt32(m_strMaxLevel))
            {
                return "";
            }
            string strTheColumn = string.Empty;
            string strGoalColumn = string.Empty;
            string cmdText = string.Empty;
            string strReplace = "[δ֪����Ϊ2��buff]";
            string[] strPrefixs = { "Begin", "Active", "EndTime" };
            int n = 1;
            bool bGetIt = false;

            if (conn.State == System.Data.ConnectionState.Closed)
            {
                conn.Open();
            }
            SqlCommand cmd = conn.CreateCommand();
            while (!bGetIt)
            {
                for (int i = 0; i < strPrefixs.Length; i++)
                {
                    strTheColumn = string.Format("{0}Attrib{1}", strPrefixs[i], n);                 
                    strGoalColumn = string.Format("{0}Value{1}A", strPrefixs[i], n);
                    cmdText = string.Format("select {0} from tbl_NewBuff where {1} = '{2}' and [Level] = {3} and ID = {4}", strGoalColumn, strTheColumn, strConst, nLevel, strBuffID);

                    cmd.CommandText = cmdText;
                    SqlDataReader dr = cmd.ExecuteReader();
                    if (dr.Read())
                    {
                        strReplace = dr[0].ToString();
                        bGetIt = true;
                        dr.Close();
                        break;
                    }
                    dr.Close();
                }
                n++;
            }
            conn.Close();
            if (strReplace.Length > 1 && strReplace[0] == '-')//����Ǹ���
            {
                strReplace = AbsoluteIt(strReplace);
            }
            return strReplace;
        }

        /// <summary>
        /// ��ȡ��ǩ��skill .. .. ..>���滻ֵ
        /// </summary>
        /// <param name="strTag"></param>
        /// <param name="nSkillID"></param>
        /// <param name="conn"></param>
        /// <param name="RootDir"></param>
        /// <param name="nLevel"></param>
        /// <returns></returns>
        private string GetSKILLReplace(string strTag, int nSkillID, SqlConnection conn, string RootDir, int nLevel)
        {
            string strScriptFile = string.Empty;
            string strReplace = string.Empty;
            if (conn.State == System.Data.ConnectionState.Closed)
            {
                conn.Open();
            }
            SqlCommand cmd = conn.CreateCommand();
            string cmdText = String.Format("select ScriptFile from tbl_skills where skillID = '{0}'", nSkillID);
            cmd.CommandText = cmdText;
            SqlDataReader dr = cmd.ExecuteReader();
            if (dr.Read())
            {
                strScriptFile = dr[0].ToString();
            }
            dr.Close();
            conn.Close();
            string strScriptFullPath = RootDir + "scripts\\skill\\" + strScriptFile;

            FileStream fsLuaScript = new FileStream(strScriptFullPath, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fsLuaScript);

            string strFileCotent = sr.ReadToEnd();

            string strSperator = " \t<>";
            string[] strSKILLTag = strTag.Split(strSperator.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
            System.Diagnostics.Debug.Assert(strSKILLTag.Length == 2);

            strReplace = GetSkillValue(nLevel, strFileCotent, strSKILLTag[1]);

            return strReplace;
        }
        /// <summary>
        /// ��ȡָ���ȼ���skill ... ��ǩ��Ӧ��ֵ
        /// </summary>
        /// <param name="nLevel">���ܵȼ�</param>
        /// <param name="strFileCotent">�ű��ļ�����</param>
        /// <param name="strAttriType">������������ �磺PhysicsDamage</param>
        /// <returns></returns>
        private string GetSkillValue(int nLevel, string strFileCotent, string strAttriType)
        {
            string strResult = string.Empty;
            string strType = string.Empty;
            string strVariable = string.Empty;
            switch (strAttriType)
            {
                case "PhysicsDamage":
                    strType = "SKILL_PHYSICS_DAMAGE";
                    break;
                case "SolarDamage":
                    strType = "SKILL_SOLAR_DAMAGE";
                    break;
                case "NeutralDamage":
                    strType = "SKILL_NEUTRAL_DAMAGE";
                    break;
                case "LunarDamage":
                    strType = "SKILL_LUNAR_DAMAGE";
                    break;
                case "PoisonDamage":
                    strType = "SKILL_POISON_DAMAGE";
                    break;
                case "Therapy":
                    strType = "SKILL_THERAPY";
                    break;

                default:
                    return "[δ֪skill]";
            }

            int nBegin = -1;
            int nEnd = -1;
            bool bIsOneWordEnd = false;

            do
            {
                nBegin = strFileCotent.IndexOf(strType);
                if (nBegin == -1)
                {
                    break;
                }
                nEnd = nBegin + strType.Length;
                bIsOneWordEnd = IsOneWordEnd(strFileCotent[nEnd]);
            } while (nBegin != -1 && !bIsOneWordEnd);
            if (nBegin != -1)
            {
                string strTrim = " \t\r\n=";
                nBegin = strFileCotent.IndexOf('.', nEnd + 1);
                nEnd = strFileCotent.IndexOf(',', nEnd + 1);
                if (nEnd > nBegin && nBegin != -1)
                {
                    strVariable = strFileCotent.Substring(nBegin + 1, nEnd - nBegin - 1);
                    strVariable = strVariable.Trim(strTrim.ToCharArray());
                    int i = 1;
                    nBegin = strFileCotent.IndexOf(strVariable, 0);
                    while (i < nLevel && nBegin != -1)
                    {
                        nBegin = strFileCotent.IndexOf(strVariable, nBegin + 1);
                        bIsOneWordEnd = IsOneWordEnd(strFileCotent[nBegin + strVariable.Length]);
                        if (bIsOneWordEnd)
                        {
                            i++;
                        }
                    }
                    if (nBegin != -1)
                    {
                        nBegin = nBegin + strVariable.Length;
                        nEnd = strFileCotent.IndexOf(',', nBegin);
                        strResult = strFileCotent.Substring(nBegin, nEnd - nBegin);
                        strResult = strResult.Trim(strTrim.ToCharArray());
                    }

                }
            }
            if (strResult.Length > 1 && strResult[0] == '-')//����Ǹ���
            {
                strResult = AbsoluteIt(strResult);
            }
            
            return strResult;
        }

        /// <summary>
        /// �ѱ�ʾ�������ַ���ת��Ϊ����
        /// </summary>
        /// <param name="strNumber"></param>
        /// <returns></returns>
        private string AbsoluteIt(string strNumber)
        {
            string strReturn = strNumber;
            
            if ( strNumber.Length < 1 || strNumber[0] != '-')
            {
                return strReturn;
            }
            strNumber = strNumber.Substring(1, strNumber.Length - 1);
            foreach (char c in strNumber)
            {
                if (c < '0' || c > '9')
                {
                    return strReturn;

                }
            }             
            return strNumber;
        }
        protected bool IsOneWordEnd(char chAfterWord)
        {
            switch (chAfterWord)
            {
                case ' ':
                case '\t':
                case '(':
                case '\r':
                case '\n':
                case ',':
                    return true;
                default:
                    return false;
            }
        }


        /// <summary>
        /// Ϊ���ܱ༭��Ԥ����ʼ��
        /// </summary>
        /// <param name="nMaxLevel">����ܵȼ�</param>
        private void InitPanel1ForSkill(int nMaxLevel, string skilllevel)
        {

            cbxSkillLevel.Visible = true;
            cbxSkillLevel.Items.Clear();
            for (int i = 1; i <= nMaxLevel; i++)
            {
                cbxSkillLevel.Items.Add(string.Format("{0}", i));
            }
            int nLevel = Convert.ToInt32(skilllevel);
            if (nLevel != 0)
            {
                cbxSkillLevel.SelectedIndex = nLevel - 1;
            }
            else
            {
                cbxSkillLevel.SelectedIndex = 0;
            }          

            panel1.BackColor = Color.DarkBlue;
        }
        const int DRAWWIDTH = 400;
        private void cbxSkillLevel_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            Brush[] myBrushs = { Brushes.Yellow, Brushes.Wheat };

            Image img1 = new Bitmap(DRAWWIDTH, 230);
            Graphics g = Graphics.FromImage(img1);
            Font cFont = new Font("SimHei", 12F);//, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Pixel, ((byte)(134)));           

            ComboBox cbx = sender as ComboBox;
            if (cbx.SelectedIndex == -1)
            {
                cbx.SelectedIndex = 0;
            }
            int nLevel = cbx.SelectedIndex + 1;

            string strDesc = m_listDesc[nLevel - 1];
            string strShortDesc = m_listShortDesc[nLevel - 1];
            //Draw skillName
            RectangleF rectDraw = new RectangleF(10, 40, DRAWWIDTH, 20);
            float fHeight = 0f;
            fHeight = GetStringDrawHeight(m_skillName, cFont, DRAWWIDTH);
            rectDraw.Height = fHeight;
            g.DrawString(m_skillName, cFont, myBrushs[0], rectDraw);  
           //Draw skill specialDesc
            rectDraw.Y += fHeight;
            fHeight = GetStringDrawHeight(m_skillSpecialDesc, cFont, DRAWWIDTH);
            rectDraw.Height = fHeight;
            g.DrawString(m_skillSpecialDesc, cFont, myBrushs[1], rectDraw);
            
            rectDraw.Y += fHeight;
            fHeight = GetStringDrawHeight(strDesc, cFont, DRAWWIDTH);
            rectDraw.Height = fHeight;
            g.DrawString(strDesc, cFont, myBrushs[0], rectDraw);
            
            rectDraw.Y += fHeight;
            fHeight = GetStringDrawHeight(strShortDesc, cFont, DRAWWIDTH);
            rectDraw.Height = fHeight;
            g.DrawString(strShortDesc, cFont, myBrushs[0], rectDraw);

            pb1.Image = img1;
            fitContoner();

        }
        
        private float GetStringDrawHeight(string strDraw, Font myFont, int nRowWidth)
        {
            Graphics g = this.CreateGraphics();            
            SizeF stringSize = new SizeF();
            stringSize = g.MeasureString(strDraw, myFont);
            float fTotalWidth = stringSize.Width;
            float fFontHeight = stringSize.Height;

            int nRowCnt = (((int)fTotalWidth + nRowWidth) / nRowWidth);
            float fDrawHeight = nRowCnt * fFontHeight;

            if (g != null)
            {
                g.Dispose();
            }
            return fDrawHeight;
        }
        //the end of sunyong's
    }
}