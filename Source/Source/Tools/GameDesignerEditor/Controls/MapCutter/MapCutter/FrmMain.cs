using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;

namespace MapCutter
{
    public partial class FrmMain : Form
    {
        [DllImport("PsdRead.dll")]
        public static extern void ReadPsdInfo(string fileName, ref int layerCount, int[] layBuffer);
        [DllImport("kernel32.dll")]
        static extern void ExitProcess(uint exitCode);

        private string rootPath;
        private string strMapPath = "";
        private atlaxLib.Test3DEngine m_3DEngie = null;

        public FrmMain()
        {
            try
            {
                InitializeComponent();
                ImageCutter.OnCutting += new ImageCutter.OnCuttingHandler(ImageCutter_OnCutting);
                string strIniFileFullName = Application.StartupPath + "\\GameDesingerTools.ini";
                strMapPath = IniIO.IniReadValue("General", "RootDir", strIniFileFullName);
                if(strMapPath == null || strMapPath == "")
                {
                    MessageBox.Show("��ͼĿ¼·��δ����!��ѳ���ŵ������ͬһĿ¼��!", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                strMapPath = strMapPath.TrimEnd(new char[] { '\\' });
                rootPath = strMapPath;
                Panel p = new Panel();

                try
                {
                    Helper.RegComDll("atlax.dll");
                    m_3DEngie = new atlaxLib.Test3DEngine();
                    m_3DEngie.Init((int)p.Handle, strMapPath + "\\", Application.StartupPath);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("atlax��ʼ��ʧ��: " + ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                strMapPath += @"\data\source\maps\";
            }
            catch (Exception ex)
            {
                MessageBox.Show("frmMain��ʼ��ʧ��: " + ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
        }

        private void FrmMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                ExitProcess(0);
            }
            catch (Exception)
            {
                ExitProcess(0);           	
            }
        }

        void ImageCutter_OnCutting(object sender, ImageCutter.CutterEventArgs e)
        {
            this.proc.Maximum = e.nAllFrame;
            this.proc.Value = e.nCurFrame;
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void btnSmallOpen_Click(object sender, EventArgs e)
        {
            CD1.FileName = "";
            CD1.Filter = "*.bmp|*.bmp";
            CD1.ShowDialog();
            if(CD1.FileName != "")
            {
                txtSmallPath.Text = CD1.FileName;
            }
        }

        private void btnMiddleOpen_Click(object sender, EventArgs e)
        {
            CD1.FileName = "";
            CD1.Filter = "*.psd|*.psd";
            CD1.ShowDialog();
            if (CD1.FileName != "")
            {
                txtMiddlePath.Text = CD1.FileName;
            }
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if(strMapPath == "")
            {
                MessageBox.Show("��ͼĿ¼·��δ����!", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if(tabControl1.SelectedTabIndex == 0)
            {
                //����С��ͼ��˵
                if (txtSmallPath.Text == "" || txtSmallName.Text == "")
                {
                    return;
                }
                btnStart.Enabled = false;
                
                //�ƶ�ͼƬ��ָ��Ŀ¼
                string strPreBmp = "";
                {
                    if (radioButton1.Checked)
                        strPreBmp = "0";
                    if (radioButton2.Checked)
                        strPreBmp = "1";
                }
                float fscale = 1.0f;
                {
                    if (checkBox4.Checked)
                        fscale = 0.6f;
                }
                string strCreatePath = "";
                {
                    strCreatePath = Path.Combine(this.strMapPath, this.txtSmallName.Text + "minimap");
                    if (!Directory.Exists(strCreatePath))
                        Directory.CreateDirectory(strCreatePath);
                }
                if(!File.Exists(txtSmallPath.Text))
                {
                    MessageBox.Show("�Ҳ���Դ�ļ�" + txtSmallPath.Text, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                //��ʼ�и����ͼ����Ƭ
                proc.Value = 0;
                proc.Visible = true;
                Application.DoEvents();
                Image imgScale = ImageCutter.ImageCutAndSave(txtSmallPath.Text, 128, 128, fscale, strCreatePath, strPreBmp + "_{1}_{0}.jpg");
                
                //����������ͼ
                // imgScale.Save(Path.Combine(strCreatePath, strPreBmp + ".bmp"), System.Drawing.Imaging.ImageFormat.Bmp);

                //дINI
                string strIniFileName = Path.Combine(strCreatePath, "config.ini");
                IniIO.IniWriteValue("config", "width", "128", strIniFileName);
                if (checkBox4.Checked)
                    IniIO.IniWriteValue("config", "scale", "0.012", strIniFileName);
                else
                    IniIO.IniWriteValue("config", "scale", "0.02", strIniFileName);

                proc.Value = 0;
                proc.Visible = false;
                
                //��ʾ�ɹ�
                MessageBox.Show("������ϣ�" + strMapPath, "���", MessageBoxButtons.OK, MessageBoxIcon.Information);
                btnStart.Enabled = true;
            }
            else
            {
                //�����е�ͼ��˵
                if (txtMiddlePath.Text == "" || txtMiddleName.Text == "")
                {
                    return;
                }
                btnStart.Enabled = false;

                //��ȡPSD�ļ���2��ͼ��
                int layCount = 0;
                int[] layBuffer =  new int[8];
                ReadPsdInfo(txtMiddlePath.Text, ref layCount, layBuffer);

                if (layBuffer == null) // û�л�����������Ϣ��ֱ�ӱ�����
                //test if(layCount != 2 || layBuffer == null)
                {
                    MessageBox.Show("��ȡpsdʧ��!", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    btnStart.Enabled = true;
                    return;
                }
                Rectangle rectBig = new Rectangle(layBuffer[1], layBuffer[0], layBuffer[3] - layBuffer[1], layBuffer[2] - layBuffer[0]);
                Rectangle rectSmall = Rectangle.Empty;
                if (layCount == 1) // ֻ��һ��ͼ�㣬rectBig��rectSmall�غ�
                {
                    rectSmall = new Rectangle(layBuffer[1], layBuffer[0], layBuffer[3] - layBuffer[1], layBuffer[2] - layBuffer[0]);
                }
                else if (layCount == 2)
                {
                    rectSmall = new Rectangle(layBuffer[1 + 4], layBuffer[0 + 4], layBuffer[3 + 4] - layBuffer[1 + 4], layBuffer[2 + 4] - layBuffer[0 + 4]);
                }
                else
                {
                    MessageBox.Show("����psd�ļ�ͼ��̫�࣬���飡\r\n\r\n����㲻֪������ʲô��˼������RTX�� kuangsihao ���� zhouhengda");
                    btnStart.Enabled = true;
                    return;
                }

                //����Ŀ¼
                string strCreatePath = "";
                {
                    strCreatePath = Path.Combine(this.strMapPath, this.txtMiddleName.Text + "minimap");
                    if (!Directory.Exists(strCreatePath))
                        Directory.CreateDirectory(strCreatePath);
                }

                //����
                float nScale = 0;
                float nStartX = 0;
                float nStartY = 0;

                int nScaleWidth = 0;   //Wʵ��Region��
                int nScaleHeight = 0;
                try
                {
                    // ֱ�Ӵ�.Map�ļ���ȡregion��Ŀ��
//                     string strMap = string.Format("{0}/data/source/maps/{1}/{1}.Map", rootPath, this.txtMiddleName.Text);
//                     FileStream fs = new FileStream(strMap, FileMode.Open, FileAccess.Read);
//                     BinaryReader r = new BinaryReader(fs);
//                     r.ReadBytes(32);
//                     nScaleWidth = r.ReadInt32();
//                     nScaleHeight = r.ReadInt32();
//                     fs.Close();



                    int ptr3DScene = 0;
                    string strMap = this.strMapPath + this.txtMiddleName.Text + "\\" + this.txtMiddleName.Text + ".Map";
                    m_3DEngie.LoadOneMap_NoOutputWindow(strMap, out ptr3DScene);
                    m_3DEngie.ReadMapTerrainInfo(ptr3DScene, out nScaleWidth, out nScaleHeight);
                    nScaleWidth /= 16;
                    nScaleHeight /= 16;
                }
                catch(Exception ex)
                {
                    MessageBox.Show("��ȡMapʧ��:" + ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    btnStart.Enabled = true;
                    return;
                }

                if (checkBoxFormat.Checked)
                {
                    string configfile = textBoxOld.Text;
                    if (configfile.Length == 0)
                    {
                        MessageBox.Show("��ѡ��ɵ�config.ini�ļ���");
                        btnStart.Enabled = true;
                        return;
                    }

                    string strScale = IniIO.IniReadValue("middlemap0", "scale", configfile);
                    float fScale = (float)Convert.ToDouble(strScale);
                    string strWidth = IniIO.IniReadValue("middlemap0", "width", configfile);
                    float fWidth = (float)Convert.ToDouble(strWidth);
                    nScaleWidth = Convert.ToInt32(fWidth * 2 / 64 / 32 / fScale);
                }


                nScale = ((float)rectSmall.Width) / ((float)nScaleWidth / 2 * 64 * 32);
                int nX0 = rectSmall.Left;
                int nY0 = rectBig.Height - rectSmall.Height - rectSmall.Top;
                int nY0_____1 = rectBig.Bottom - rectSmall.Bottom;
                nStartX = (-nX0) / nScale;
                nStartY = (-nY0) / nScale;

                //дINI
                string strIniFileName = Path.Combine(strCreatePath, "config.ini");
                IniIO.IniWriteValue("middlemap0", "name", this.txtMiddleName.Text, strIniFileName);
                IniIO.IniWriteValue("middlemap0", "image", "middlemap.tga", strIniFileName);
                IniIO.IniWriteValue("middlemap0", "width", rectBig.Width.ToString(), strIniFileName);
                IniIO.IniWriteValue("middlemap0", "height", rectBig.Height.ToString(), strIniFileName);
                IniIO.IniWriteValue("middlemap0", "scale", nScale.ToString(), strIniFileName);
                IniIO.IniWriteValue("middlemap0", "startx", nStartX.ToString(), strIniFileName);
                IniIO.IniWriteValue("middlemap0", "starty", nStartY.ToString(), strIniFileName);
                IniIO.IniWriteValue("middlemap0", "copy", checkBox1.Checked ? "1" : "0", strIniFileName);
                IniIO.IniWriteValue("middlemap0", "fresherroom", checkBox2.Checked ? "1" : "0", strIniFileName);
                IniIO.IniWriteValue("middlemap0", "battlefield", checkBox3.Checked ? "1" : "0", strIniFileName);

                //��ʾ�ɹ�
                MessageBox.Show("����Ini�ļ���ϣ�\r\n\r\n���ֹ�����middlemap.tga��\r\n\r\n" + strMapPath, "���", 
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                btnStart.Enabled = true;
            }
        }

        private void btnMiddleOpen2_Click(object sender, EventArgs e)
        {
            CD1.FileName = "";
            CD1.Filter = "*.ini|*.ini";
            CD1.ShowDialog();
            if (CD1.FileName != "")
            {
                textBoxOld.Text = CD1.FileName;
            }
        }

        private void btnTransfer_Click(object sender, EventArgs e)
        {
            AreaTabFiller tabFileTransfer = new AreaTabFiller();
            string assistFilePath = tbxArtFile.Text.ToString();
            if (!File.Exists(assistFilePath))
            {
                MessageBox.Show("�����ļ�" + assistFilePath + "������");
                return;
            }
            try
            {
                tabFileTransfer.Init(this.rootPath, tbxMapName.Text.ToString(), assistFilePath);
                tabFileTransfer.Transfer();
                tabFileTransfer.Save();
                MessageBox.Show("��ϲ��\r\nת�����", "�ɹ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception myex)
            {              
                MessageBox.Show(myex.ToString(), "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            
        }

        private void btnBrower_Click(object sender, EventArgs e)
        {
            CD1.FileName = "";
            CD1.Filter = "tab�ļ�(*.tab)|*.tab|�����ļ�(*.*)|*.*";
            CD1.ShowDialog();            
            if (CD1.FileName != "")
            {
                tbxArtFile.Text = CD1.FileName;              
            }
        }


    }
}