using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Collections;

namespace MapCutter
{
    public partial class FrmMain
    {  
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

        [DllImport("user32.dll")]
        static extern void LockWindowUpdate(IntPtr handle);

        private string currentPath; // ��ǰ·��
        private Image image; // ��ǰ��ʾ��ͼƬ
        private Hashtable dataTable = new Hashtable(); // ����id��Ӧ���ݵĹ�ϣ��
        private List<string> itemList = new List<string>(); // ���汳������������ѡ�������

        /// <summary>
        /// ��ȡ�ļ�
        /// </summary>
        /// <param name="fileName">�ļ�ȫ·��</param>
        private void LoadPicture(string fileName)
        {
            Image image = Image.FromFile(fileName);
            canvas.PaintPicture(image);
            canvas.ClearPaint();
            this.image = image;
            panel1_SizeChanged(null, null);
        }

        /// <summary>
        /// ���ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bOpen_Click(object sender, EventArgs e)
        {
            string fileName; // �ļ���    
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Title = "��ȡ�����ļ�";
            openFileDialog.InitialDirectory = Environment.SpecialFolder.MyComputer.ToString();
            openFileDialog.Filter = "�Ҷ�ͼ�ļ�(*.bmp)|*.bmp"; // �����ļ�����
            openFileDialog.InitialDirectory = rootPath;
            openFileDialog.FilterIndex = 0;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                fileName = openFileDialog.FileName;
                if ((fileName != null) && (fileName != ""))
                {
                    this.currentPath = Path.GetFullPath(fileName);
                    this.Text = "�Ҷ�ͼ����� " + fileName;
                    LoadPicture(fileName);
                }
            }
        }

        /// <summary>
        /// �ر��ļ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// ��ȡ����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bRead_Click(object sender, EventArgs e)
        {
            string fileName; // �ļ���
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Environment.SpecialFolder.MyComputer.ToString();
            openFileDialog.Filter = "�����ļ�(*.tab)|*.tab"; // �����ļ�����
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 0;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                fileName = openFileDialog.FileName;
                if ((fileName != null) && (fileName != ""))
                {
                    this.currentPath = Path.GetFullPath(fileName);
                    dataTable.Clear(); // ��վ�����
                    itemList.Clear(); // ��վ�����
                    ReadFile(fileName);
                }
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bSave_Click(object sender, EventArgs e)
        {
            string fileName = Path.Combine(currentPath, "area.tab");
            WriteFile(fileName);
        }

        /// <summary>
        /// Ԥ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bView_Click(object sender, EventArgs e)
        {
            PreViewForm pForm = new PreViewForm();
            pForm.ShowData(dataTable);
            pForm.ShowDialog();
        }

        /// <summary>
        /// ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bClear_Click(object sender, EventArgs e)
        {
            canvas.ClearPaint();
        }

        /// <summary>
        /// ��ʾ����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bMark_Click(object sender, EventArgs e)
        {
            List<int> markedIDList = new List<int>();
            foreach (string s in dataTable.Keys)
            {
                markedIDList.Add(int.Parse(s));
            }
            canvas.MultiMarkArea(markedIDList);
        }

        /// <summary>
        /// ���˫��ͼƬ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (image != null)
            {
                int x = (int)((double)e.X * image.Width / canvas.Width);
                int y = (int)((double)e.Y * image.Height / canvas.Height);
                Bitmap bmp = new Bitmap(image);
                if ((x < bmp.Width) && (y < bmp.Height))
                {
                    Color pixelColor = bmp.GetPixel(x, y);
                    string id = GetID(pixelColor);
                    if (id != null)
                    {
                        string[] data = dataTable[id] as string[];
                        SettingForm sForm;

                        if (data != null)
                        {
                            sForm = new SettingForm(data[0], data[1], data[3], itemList);
                        }
                        else
                        {
                            sForm = new SettingForm(id, itemList);
                        }

                        sForm.RootPath = rootPath;
                        DialogResult result = sForm.ShowDialog();
                        if (result == DialogResult.OK)
                        {
                            if (data != null) // �����Ѿ���ʼ����������ֵ����
                            {
                                data[1] = sForm.AreaName;
                                data[2] = "0";
                                data[3] = sForm.BackgroundMusic;
                            }
                            else // ��ʼ������
                            {
                                data = new string[4];
                                data[0] = id;
                                data[1] = sForm.AreaName;
                                data[2] = "0";
                                data[3] = sForm.BackgroundMusic;
                                dataTable[id] = data;
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// ������д���ļ�
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        private void WriteFile(string fileName)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Title = "���������ļ�";
            saveFileDialog.FileName = fileName;
            saveFileDialog.Filter = "�����ļ�(*.tab)|*.tab"; // �����ļ�����
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                fileName = saveFileDialog.FileName;
                if ((fileName != null) && (fileName != ""))
                {
                    string[] data;
                    string value;

                    try
                    {
                        FileInfo fi = new FileInfo(fileName);
                        StreamWriter sw;

                        if (!fi.Exists) // �ļ�������
                        {
                            string path = Path.GetDirectoryName(fileName); // ��ȡĿ¼
                            if (!Directory.Exists(path)) // Ŀ¼������
                            {
                                Directory.CreateDirectory(path); // �ȴ���Ŀ¼���ٴ����ļ�
                            }
                            FileStream fs = fi.Create();
                            fs.Close();
                            sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                        }
                        else
                        {
                            sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                        }

                        // д���ļ�
                        sw.WriteLine("id\tname\tmiddlemap\tbackgroundmusic\t");
                        foreach (string s in dataTable.Keys)
                        {
                            data = dataTable[s] as string[];
                            if (data != null)
                            {
                                value = string.Format("{0}\t{1}\t{2}\t{3}\t", data[0], data[1], data[2], data[3]);
                                sw.WriteLine(value);
                            }
                        }

                        sw.Close();
                        MessageBox.Show(string.Format("�ļ�{0}����ɹ�!", fileName));
                    }
                    catch (IOException ex)
                    {
                        MessageBox.Show("�ڱ���Ҷ�ͼ�����ļ�ʱ����IO�쳣: " + ex.ToString());
                    }
                }
            }
        }

        /// <summary>
        /// ��ȡ��ɫ��Ӧ��id
        /// </summary>
        /// <param name="color">��ɫ����</param>
        /// <returns>��Ӧ��id</returns>
        private string GetID(Color color)
        {
            string id = null;

            id = ((int)color.B).ToString();

            return id;
        }

        /// <summary>
        /// ���ļ���������
        /// </summary>
        /// <param name="fileName"></param>
        private void ReadFile(string fileName)
        {
            try
            {
                StreamReader sr = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                string value = sr.ReadLine();
                string[] data;
                bool firstLine = true;

                while (value != null)
                {
                    if (firstLine) // ������һ��
                    {
                        firstLine = false;
                    }
                    else
                    {
                        data = value.Split(new char[] { '\t' });
                        dataTable[data[0]] = data;
                        if (!itemList.Contains(data[3])) // ��ӱ�����������������
                        {
                            itemList.Add(data[3]);
                        }
                    }

                    value = sr.ReadLine();
                }
                sr.Close();
                MessageBox.Show(string.Format("�ļ�{0}��ȡ�ɹ�!", fileName));
            }
            catch (IOException ex)
            {
                MessageBox.Show("�ڶ�ȡ�Ҷ�ͼ�����ļ�ʱ����IO�쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// ��굥��ͼƬ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void canvas_MouseClick(object sender, MouseEventArgs e)
        {
            LockWindowUpdate(canvas.Handle);

            if (e.Button == MouseButtons.Left) // ������
            {
                if (image != null)
                {
                    int x = (int)((double)e.X * image.Width / canvas.Width);
                    int y = (int)((double)e.Y * image.Height / canvas.Height);
                    Bitmap bmp = new Bitmap(image);
                    bool blueString = true;

                    if ((x < bmp.Width) && (y < bmp.Height))
                    {
                        Color pixelColor = bmp.GetPixel(x, y);
                        string id = GetID(pixelColor);
                        if (id != null) // �������Ӧ��id
                        {
                            if (pixelColor.B < 127) // ��ɫ��������ǳɫ�ı���ʾ
                            {
                                blueString = false;
                            }
                            string[] information = dataTable[id] as string[];
                            if (information != null) // �Ѿ��и�id����Ϣ��ֱ����ʾ
                            {
                                string path = information[3];
                                canvas.PaintString(string.Format("ID: {0}\n��������: {1}\n�����ļ�: {2}", id, information[1], Path.GetFileName(path)), e.X, e.Y, blueString);
                            }
                            else // û�и�id����Ϣ
                            {
                                canvas.PaintString(string.Format("ID: {0}\n��������: ��\n�����ļ�: ��", id), e.X, e.Y, blueString);
                            }
                        }
                        else
                        {
                            canvas.PaintString("ID: ��\n��������: ��\n�����ļ�: ��", e.X, e.Y, blueString);
                        }

                        canvas.MarkArea(pixelColor.B);
                    }
                }
            }
            else // �Ҽ����ļ����
            {
                canvas.ClearPaint();
            }

            LockWindowUpdate(IntPtr.Zero);
        }

        private void buttonItem1_Click(object sender, EventArgs e)
        {
            string strIniFileFullName = Directory.GetCurrentDirectory() + "\\GameDesingerTools.ini";
            string strEnginePath = IniReadValue("General", "3DEnginePath", strIniFileFullName);
            if (strEnginePath == null || strEnginePath == "")
            {
                MessageBox.Show("��ͼĿ¼·��δ����!\r\n��ѳ���ŵ������ͬһĿ¼��!", "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            string fileName; // �ļ���
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Title = "��ȡ��ͼ";
            openFileDialog.InitialDirectory = strEnginePath + "/data/source/maps/";
            openFileDialog.Filter = "��ͼ�ļ�(*.Map)|*.map|ȫ���ļ�(*.*)|*.*"; // �����ļ�����
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 0;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                fileName = openFileDialog.FileName;
                if ((fileName != null) && (fileName != ""))
                {
//                     MapFrm mapform = new MapFrm(strEnginePath, fileName);
//                     mapform.Show();
                }
            }
        }

        private string IniReadValue(string Section, string Key, string filepath) // ��ini�ļ����ж������ĺ���
        {
            StringBuilder temp = new StringBuilder(255);
            int i = GetPrivateProfileString(Section, Key, "", temp, 255, filepath);
            return temp.ToString();
        }

        /// <summary>
        /// �����Ĵ�С�ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void panel1_SizeChanged(object sender, EventArgs e)
        {
            int newWidth = canvas.Width;
            int newHeight = canvas.Height;
            canvas.ClearPaint();
            canvas.ResetSize(newWidth, newHeight);
        }
    }
}
