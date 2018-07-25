using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.IO;
using System.Drawing.Imaging;

namespace GameDesingerTools.DBTools
{
    public partial class AddImageForm : Form
    {
        private string fileName; // �ļ�����
        private Image image; // ͼ��
        Image previewImage; // Ԥ����ͼ��
        private TreeNode node; // �����
        private ModelEdit modelEditor; // ��Ԫ�༭������
        private bool readSuccess = false; // �Ƿ��ȡͼ��ɹ�
        private List<string> encodableTypeList = new List<string>(); // ���Խ��б������õ���������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="image">Ҫ�����ͼ��</param>
        public AddImageForm(string fileName, TreeNode node, ModelEdit modelEditor)
        {
            this.fileName = fileName;
            this.node = node;
            this.modelEditor = modelEditor;
            InitializeComponent();
            comboBoxEx1.SelectedIndex = 2; // Ĭ��Ϊ��ͼ������
            comboBoxEx2.SelectedIndex = 1; // Ĭ��Ϊ16λɫ��
            FillEncodableList();
            ShowImage();
        }

        /// <summary>
        /// �����Խ��б������õ���������
        /// </summary>
        private void FillEncodableList()
        {
            /*
             * GDI+���õı�����֧�ֵ��ļ������У� 
             * 1 *.bmp, *.dib, *.rle
             * 2 *.jpg, *.jpeg, *.jpe, *.jfif
             * 3 *.gif
             * 4 *.tif, *.tiff
             * 5 *.png
             */
            encodableTypeList.Add(".bmp");
            encodableTypeList.Add(".dib");
            encodableTypeList.Add(".rle");
            encodableTypeList.Add(".jpg");
            encodableTypeList.Add(".jpeg");
            encodableTypeList.Add(".jpe");
            encodableTypeList.Add(".jfif");
            encodableTypeList.Add(".gif");
            encodableTypeList.Add(".tif");
            encodableTypeList.Add(".tiff");
            encodableTypeList.Add(".png");
        }

        /// <summary>
        /// ��ʾͼ��
        /// </summary>
        private void ShowImage()
        {
            try
            {
                image = Image.FromFile(fileName);
                readSuccess = true;
            }
            catch(Exception ex)
            {
                MessageBox.Show("�޷�ʶ���ͼ���ļ�! \n\n" + ex.ToString());
                readSuccess = false;
            }

            if(readSuccess && image != null)
            {
                if(!encodableTypeList.Contains(Path.GetExtension(fileName).ToLower())) // ��֧�ֽ��б������õ�����
                {
                    checkBoxX1.Enabled = false;
                }

                Image.GetThumbnailImageAbort callb = new Image.GetThumbnailImageAbort(ThumbnailCallback); 
                Image newImage = image.GetThumbnailImage(24, 24, callb, new IntPtr());
                label1.Image = newImage;
                image.Dispose(); // �ͷ�image��ռ��Դ
                image = newImage;
            }
            else
            {
                DialogResult = DialogResult.No;
                this.Close();
            }
        }

        /// <summary>
        /// �ص�����
        /// </summary>
        /// <returns>����ֵ</returns>
        private bool ThumbnailCallback()
        {
            return false;
        }

        /// <summary>
        /// ��ȡͼƬ������
        /// </summary>
        /// <param name="extention">�ļ���׺</param>
        /// <returns>ͼƬ����</returns>
        private ImageFormat GetImageFormat(string extention)
        {
            ImageFormat format = ImageFormat.Bmp;

            switch(extention)
            {
                case ".jpg":
                    {
                        format = ImageFormat.Jpeg;
                        break;
                    }
                case ".jpeg":
                    {
                        format = ImageFormat.Jpeg;
                        break;
                    }
                case ".bmp":
                    {
                        format = ImageFormat.Bmp;
                        break;
                    }
                case ".emf":
                    {
                        format = ImageFormat.Emf;
                        break;
                    }
                case ".exif":
                    {
                        format = ImageFormat.Exif;
                        break;
                    }
                case ".gif":
                    {
                        format = ImageFormat.Gif;
                        break;
                    }
                case ".ico":
                    {
                        format = ImageFormat.Icon;
                        break;
                    }
                case ".icon":
                    {
                        format = ImageFormat.Icon;
                        break;
                    }
                case ".png":
                    {
                        format = ImageFormat.Png;
                        break;
                    }
                case ".tiff":
                    {
                        format = ImageFormat.Tiff;
                        break;
                    }
                case ".wmf":
                    {
                        format = ImageFormat.Wmf;
                        break;
                    }
            }

            return format;
        }

        /// <summary>
        /// ��ȡͼ��ı�����Ϣ
        /// </summary>
        /// <param name="mimeType">��������</param>
        /// <returns>������Ϣ</returns>
        private ImageCodecInfo GetImageCodecInfo(string extention)
        {
            ImageCodecInfo imageCodecInfo = null;
            ImageCodecInfo[] encoders = ImageCodecInfo.GetImageEncoders();
            /*
             * GDI+���õı�����֧�ֵ��ļ������У� 
             * 1 *.bmp, *.dib, *.rle
             * 2 *.jpg, *.jpeg, *.jpe, *.jfif
             * 3 *.gif
             * 4 *.tif, *.tiff
             * 5 *.png
             */
            foreach(ImageCodecInfo i in encoders)
            {
                if(i.FilenameExtension.Contains(extention))
                {
                    imageCodecInfo = i;
                }
            }
            return imageCodecInfo;
        }

        /// <summary>
        /// ������ʱ�ļ�����ͼ����Ϣ
        /// </summary>
        /// <returns>�ַ�����</returns>
        private byte[] SaveImageInFile()
        {
            int index = 0;
            string tempFileName = index.ToString() + Path.GetExtension(fileName);
            byte[] buffer = null;
            try
            {
                while(File.Exists(tempFileName)) // �ļ��Ѿ�����
                {
                    index++;
                    tempFileName = index.ToString() + Path.GetExtension(fileName);
                }
                image.Save(tempFileName);
                FileInfo fi = new FileInfo(tempFileName);
                Stream s = fi.OpenRead();
                buffer = new byte[(int)fi.Length];
                s.Read(buffer, 0, buffer.Length);
                s.Close();
                fi.Delete();
            }
            catch(Exception ex)
            {
                MessageBox.Show("�ڱ����޸ĺ��ͼ����ʱ�ļ�ʱ�����쳣: " + ex.ToString());
            }
            return buffer;
        }

        /// <summary>
        /// ��ȡ�Զ���ı������
        /// </summary>
        /// <returns>�Զ���ı������</returns>
        private EncoderParameters GetConfigEncoderParameters()
        {
            long imageQuality = 20L; // ͼ������
            switch (comboBoxEx1.SelectedIndex) // ��ȡͼ������
            {
                case 0: // ������
                    {
                        imageQuality = 100L;
                        break;
                    }
                case 1: // ������
                    {
                        imageQuality = 60L;
                        break;
                    }
                case 2: // ������
                    {
                        imageQuality = 20L;
                        break;
                    }
            }

            long imageColorDepth = 16L; // ͼ��ɫ��
            switch (comboBoxEx2.SelectedIndex) // ��ȡͼ��ɫ��
            {
                case 0: // 8λɫ��
                    {
                        imageColorDepth = 8L;
                        break;
                    }
                case 1: // 16λɫ��
                    {
                        imageColorDepth = 16L;
                        break;
                    }
                case 2: // 24λɫ��
                    {
                        imageColorDepth = 24L;
                        break;
                    }
                case 3: // 32λɫ��
                    {
                        imageColorDepth = 32L;
                        break;
                    }
            }

            ImageCodecInfo myImageCodecInfo = GetImageCodecInfo(Path.GetExtension(fileName).ToUpper());
            System.Drawing.Imaging.Encoder myEncoder1 = System.Drawing.Imaging.Encoder.Quality; 
            System.Drawing.Imaging.Encoder myEncoder2 = System.Drawing.Imaging.Encoder.ColorDepth;
            EncoderParameters myEncoderParameters = new EncoderParameters(2);
            EncoderParameter myEncoderParameter1 = new EncoderParameter(myEncoder1, imageQuality);
            EncoderParameter myEncoderParameter2 = new EncoderParameter(myEncoder2, imageColorDepth);
            myEncoderParameters.Param[0] = myEncoderParameter1;
            myEncoderParameters.Param[1] = myEncoderParameter2;
            return myEncoderParameters;
        }

        /// <summary>
        /// ��������
        /// </summary>
        private void SaveData()
        {
            // �������ݿ�
            try
            {                
                if(MainForm.conn.State == ConnectionState.Closed) // ��sql����
                {
                    MainForm.conn.Open();
                }

                EncoderParameters myEncoderParameters; // �������
                if(checkBoxX1.Checked == true) // �û��Զ���������
                {
                    myEncoderParameters = GetConfigEncoderParameters();
                }
                else // ����Ĭ�ϵı������
                {
                    myEncoderParameters = new EncoderParameters(1);
                    System.Drawing.Imaging.Encoder myEncoder = System.Drawing.Imaging.Encoder.Quality;
                    EncoderParameter myEncoderParameter = new EncoderParameter(myEncoder, 100L);
                    myEncoderParameters.Param[0] = myEncoderParameter;
                }

                MemoryStream ms = new MemoryStream();
                ImageCodecInfo myImageCodecInfo = GetImageCodecInfo(Path.GetExtension(fileName).ToUpper());

                byte[] imageData = null; // ͼ�����Ϣ
                if(checkBoxX1.Checked && myImageCodecInfo != null) // ��Ч�ı�����Ϣ
                {
                    image.Save(ms, myImageCodecInfo, myEncoderParameters);
                    imageData = ms.GetBuffer();
                    ms.Read(imageData, 0, imageData.Length);
                }
                else // �����ļ������¶�ȡ 
                {
                    imageData = SaveImageInFile();
                }
                

                // ����Ԥ��Ч��
                // Image previewImage = Image.FromStream(ms);
                // label1.Image = previewImage;
                /* remoting ???
                SqlCommand cmd = MainForm.conn.CreateCommand();
                cmd.CommandText = string.Format("UPDATE sys_modl_def SET iconid = {0}, ImageData = @imageData WHERE id = {1}", 
                    -1, (int)node.Tag);
                SqlParameter paramData = new SqlParameter("@imageData", SqlDbType.Image);
                paramData.Value = imageData;
                cmd.Parameters.Add(paramData);
                cmd.ExecuteNonQuery();
                 */

                string filter = string.Format("id = {0}", (int)node.Tag);
                string _sql = "select * from sys_modl_def where " + filter;

                DataTable tbl = Helper.GetDataTableProxy("sys_modl_def", filter, null, MainForm.conn);
                foreach (DataRow row in tbl.Rows)
                {
                    row["iconid"] = -1;
                    row["ImageData"] = imageData;
                }
                Helper.SaveTable(tbl, _sql, MainForm.conn);


                modelEditor.imglstIcons.Images.Add(fileName, image);
                node.ImageIndex = modelEditor.imglstIcons.Images.Count - 1;
                node.SelectedImageIndex = modelEditor.imglstIcons.Images.Count - 1;
                modelEditor.LoadModelDefTable();
                Program.MainForm.RefreshIcons(node.Tag.ToString(), image); // ˢ��ģ��id��Ӧ��Buttonͼ��
            }
            catch(Exception ex)
            {
                MessageBox.Show("�ڸ���ͼ���ļ����ݿ�ʱ�����쳣: " + ex.ToString());
            }
            finally
            {
                if(MainForm.conn.State == ConnectionState.Open) // �ر�sql����
                {
                    MainForm.conn.Close();
                }
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��string sql = "select * from sys_modl_def where ";
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            SaveData();
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// �û�ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }

        /// <summary>
        /// �û�ѡ���Զ���������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void checkBoxX1_CheckedChanged(object sender, EventArgs e)
        {
            if(checkBoxX1.Checked == true) // �Ѿ�ѡ��
            {
                groupPanel2.Enabled = true;
                if(previewImage == null) // û������Ԥ��ͼ��
                {
                    EncoderParameters myEncoderParameters = GetConfigEncoderParameters(); // �������
                    MemoryStream ms = new MemoryStream(); // �ڴ���
                    ImageCodecInfo myImageCodecInfo = GetImageCodecInfo(Path.GetExtension(fileName).ToUpper());
                    image.Save(ms, myImageCodecInfo, myEncoderParameters);
                    byte[] imageData = ms.GetBuffer(); // ͼ�����Ϣ
                    ms.Read(imageData, 0, imageData.Length);
                    previewImage = Image.FromStream(ms); // Ԥ����ͼ��
                    label1.Image = previewImage;
                }
                else
                {
                    label1.Image = previewImage;
                }       
            }
            else // ȡ��ѡ��
            {
                groupPanel2.Enabled = false;
                label1.Image = image;
            }
        }

        /// <summary>
        /// �û��ı�ͼ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void comboBoxEx1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(image != null && checkBoxX1.Checked)
            {
                EncoderParameters myEncoderParameters = GetConfigEncoderParameters(); // �������
                MemoryStream ms = new MemoryStream(); // �ڴ���
                ImageCodecInfo myImageCodecInfo = GetImageCodecInfo(Path.GetExtension(fileName).ToUpper());
                image.Save(ms, myImageCodecInfo, myEncoderParameters);
                byte[] imageData = ms.GetBuffer(); // ͼ�����Ϣ
                ms.Read(imageData, 0, imageData.Length);
                previewImage = Image.FromStream(ms); // Ԥ����ͼ��
                label1.Image = previewImage;
            }
        }

        /// <summary>
        /// �û��ı�ͼ��ɫ��
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void comboBoxEx2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (image != null && checkBoxX1.Checked)
            {
                EncoderParameters myEncoderParameters = GetConfigEncoderParameters(); // �������
                MemoryStream ms = new MemoryStream(); // �ڴ���
                ImageCodecInfo myImageCodecInfo = GetImageCodecInfo(Path.GetExtension(fileName).ToUpper());
                image.Save(ms, myImageCodecInfo, myEncoderParameters);
                byte[] imageData = ms.GetBuffer(); // ͼ�����Ϣ
                ms.Read(imageData, 0, imageData.Length);
                previewImage = Image.FromStream(ms); // Ԥ����ͼ��
                label1.Image = previewImage;
            }         
        }
    }
}