using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml;

namespace autoUpdateXML
{
    public partial class frmMain : Form
    {
        public frmMain()
        {
            InitializeComponent();
        }

        private void btnGet_Click(object sender, EventArgs e)
        {
            try
            {
                XmlDocument xd = new XmlDocument();
                xd.LoadXml(txtInfo.Text);
                XmlNodeList xnl = xd.GetElementsByTagName("filelist");
                xnl[0].InnerXml = codeMake();
                txtInfo.Text = FormatXml(xd.InnerXml);
                label3.Text = "�������~~";
            }
            catch
            {
                MessageBox.Show("�ȶ�ȡ�������ϵĴ�����");
                label3.Text = "����ʧ��!";
            }
        }

        private string codeMake()
        {
            string strOut = "";
            strOut += "<item name=\"";
            strOut += @"\\changan\newtools\UpdateProgram.exe";
            strOut += "\" size=\"\"><value /></item>";
            string sp = "";

            foreach(string strFileName in Directory.GetFiles(Application.StartupPath,"*",SearchOption.AllDirectories))
            {
                if(strFileName.IndexOf("\\_") != -1)
                {
                    continue;
                }

                //�ų��ļ��б�
                if (strFileName.ToLower() == Path.Combine(Application.StartupPath,"UpdateProgram.exe").ToLower()
                 || strFileName.ToLower() == Path.Combine(Application.StartupPath, "autoUpdateXML.exe").ToLower()
                 || strFileName.ToLower() == Path.Combine(Application.StartupPath, "update.xml").ToLower()
                 || strFileName.ToLower() == Path.Combine(Application.StartupPath, "local.xml").ToLower()
                 || strFileName.ToLower() == Path.Combine(Application.StartupPath, "GameDesingerTools.exe.config").ToLower()
                 || strFileName.ToLower() == Path.Combine(Application.StartupPath, "GameDesingerTools.ini").ToLower()
                    )
                {
                    sp += strFileName + "\n";
                    continue;
                }
                strOut += "<item name=\"";
                strOut += strFileName.Replace(Application.StartupPath, @"\\changan\newtools");
                strOut += "\" size=\"\"><value /></item>";
            }
            MessageBox.Show("�ų��ļ�(��������_����ͷ�ļ�)��\n\n" + sp);
            return strOut;
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            label3.Text = "���ڱ���...";
            if (txtInfo.Text == "")
            {
                MessageBox.Show("�ļ���~");
                return;
            }
            if (File.Exists("update.xml"))
            {
                File.Delete("update.xml");
            }
            FileStream fs = File.OpenWrite("update.xml");
            StreamWriter sw = new StreamWriter(fs);
            sw.Write(txtInfo.Text);
            sw.Close();
            fs.Close();
            label3.Text = "";
            MessageBox.Show("update.xml�ѱ��棡");
        }

        private void btnDownload_Click(object sender, EventArgs e)
        {
            label3.Text = "��ȡ��...";
            if (File.Exists("update.xml"))
            {
                File.Delete("update.xml");
            }
            File.Copy(@"\\changan\newtools\update.xml", "update.xml");
            string stext = File.ReadAllText("update.xml");
            txtInfo.Text = stext;
            label3.Text = "��ȡ���";
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private string FormatXml(string sUnformattedXml)
        {
            XmlDocument xd = new XmlDocument();
            xd.LoadXml(sUnformattedXml);
            StringBuilder sb = new StringBuilder();
            StringWriter sw = new StringWriter(sb);
            XmlTextWriter xtw = null;
            try
            {
                xtw = new XmlTextWriter(sw);
                xtw.Formatting = Formatting.Indented;
                xtw.Indentation = 4;
                xtw.IndentChar = ' ';

                xd.WriteTo(xtw);
            }
            finally
            {
                if (xtw != null)
                    xtw.Close();
            }
            return sb.ToString();
        }

        private void btnHelp_Click(object sender, EventArgs e)
        {
            string info = "\t\t\t����\n";
            info += "ʹ�÷�����\n";
            info += "1. ��[��ȡ����������]���ӷ������ϻ�ȡupdate.xml�ļ�\n";
            info += "2. ��[����filelist]������filelist����\n";
            info += "3. ��[����XML]������update.xml�ڵ�ǰĿ¼��\n";
            info += "\nע�����\n";
            info += "1. ��filelist�Ǹ��ݵ�ǰĿ¼�õ��ģ�������Ҫ����ȷĿ¼������\n";
            info += "2. ֻ�����filelist, �������򲻻ᶯ\n";
            MessageBox.Show(info);
        }

    }
}