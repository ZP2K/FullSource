using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace StandardNpcCreate
{
    class AnimationHelper
    {
        private static AnimationHelper animationHelper; // �����ļ����������
        private string rootPath; //�ͻ��� ��Ŀ¼

        /// <summary>
        /// ˽�й��캯��
        /// </summary>
        private AnimationHelper()
        {

        }

        /// <summary>
        /// �ͻ��˸�Ŀ¼
        /// </summary>
        public string RootPath
        {
            set
            {
                rootPath = value;
            }
        }

        /// <summary>
        /// ��ȡ�����ļ����������
        /// </summary>
        /// <returns>�����ļ����������</returns>
        public static AnimationHelper GetAnimationHelper()
        {
            if (animationHelper == null)
            {
                animationHelper = new AnimationHelper();
            }

            return animationHelper;
        }

        /// <summary>
        /// ��ȡ�����ļ�֡��
        /// </summary>
        /// <param name="animationFileName">�����ļ���</param>
        /// <returns>�����ļ�֡��</returns>
        public int GetAniFrameCount(string animationFileName)
        {
            int aniFrameCount = 0;

            if (animationFileName.EndsWith(".tani"))
            {
                string fileName = Path.Combine(rootPath, animationFileName);
                FileInfo fi = new FileInfo(fileName);

                if (fi.Exists)
                {
                    FileStream fs = fi.OpenRead();                

                    // ����ǰ��8���ֽ�
                    for (int i = 0; i < 8; i++)
                    {
                        fs.ReadByte();                        
                    }

                    List<byte> byteList = new List<byte>();

                    while (true)
                    {
                        int charValue = fs.ReadByte();
                        byte b = (byte)charValue;
                        char c = Convert.ToChar(b);    

                        if (c == '\0')
                        {
                            break;
                        }
                        else
                        {
                            byteList.Add(b);
                        }
                    }

                    fs.Close();
                    
                    string aniFileName = Encoding.GetEncoding("gb2312").GetString(byteList.ToArray());
                    aniFrameCount = GetAnimationFramCount(aniFileName);
                }                
            }
            else if (animationFileName.EndsWith(".ani"))
            {
                aniFrameCount = GetAnimationFramCount(animationFileName);
            }

            return aniFrameCount;
        }

        /// <summary>
        /// ��ȡ�����ļ�֡��
        /// </summary>
        /// <param name="animationFileName">�����ļ���</param>
        /// <returns>�����ļ�֡��</returns>
        private int GetAnimationFramCount(string animationFileName)
        {
            int aniFrameCount = 0;

            string fileName = Path.Combine(rootPath, animationFileName);
            FileInfo fi = new FileInfo(fileName);

            if (fi.Exists)
            {
                FileStream fs = fi.OpenRead();

                // ����ǰ��12���ֽ�
                for (int i = 0; i < 12; i++)
                {
                    fs.ReadByte();
                }

                StringBuilder aniFileName = new StringBuilder();
                byte[] buffer = new byte[4];

                for (int i = 0; i < 4; i++)
                {
                    buffer[i] = (byte)fs.ReadByte();                    
                }

                int animationType = (int)BitConverter.ToUInt32(buffer, 0);
                int frameCount = 0;
                float frameLength = 0;

                switch (animationType)
                {
                    case 1: // ANIMATION_VERTICES
                        {
                            // ����38���ֽ�
                            for (int i = 0; i < 38; i++)
                            {
                                fs.ReadByte();
                            }

                            buffer = new byte[4];

                            for (int i = 0; i < 4; i++)
                            {
                                buffer[i] = (byte)fs.ReadByte();
                            }
                            frameCount = (int)BitConverter.ToUInt32(buffer, 0);

                            buffer = new byte[4];

                            for (int i = 0; i < 4; i++)
                            {
                                buffer[i] = (byte)fs.ReadByte();
                            }
                            frameLength = (float)BitConverter.ToSingle(buffer, 0);

                            break;
                        }
                    case 2: // ANIMATION_VERTICES_RELATIVE
                        {
                            for (int i = 0; i < 38; i++)
                            {
                                fs.ReadByte();
                            }

                            buffer = new byte[4];

                            for (int i = 0; i < 4; i++)
                            {
                                buffer[i] = (byte)fs.ReadByte();
                            }
                            frameCount = (int)BitConverter.ToUInt32(buffer, 0);

                            buffer = new byte[4];

                            for (int i = 0; i < 4; i++)
                            {
                                buffer[i] = (byte)fs.ReadByte();
                            }
                            frameLength = (float)BitConverter.ToSingle(buffer, 0);

                            break;
                        }
                    case 10: // ANIMATION_BONE
                        {
                            for (int i = 0; i < 34; i++)
                            {
                                fs.ReadByte();
                            }

                            buffer = new byte[4];

                            for (int i = 0; i < 4; i++)
                            {
                                buffer[i] = (byte)fs.ReadByte();
                            }
                            frameCount = (int)BitConverter.ToUInt32(buffer, 0);

                            buffer = new byte[4];

                            for (int i = 0; i < 4; i++)
                            {
                                buffer[i] = (byte)fs.ReadByte();
                            }
                            frameLength = (float)BitConverter.ToSingle(buffer, 0);

                            break;
                        }
                    case 11: // ANIMATION_BONE_RELATIVE
                        {
                            for (int i = 0; i < 34; i++)
                            {
                                fs.ReadByte();
                            }

                            buffer = new byte[4];

                            for (int i = 0; i < 4; i++)
                            {
                                buffer[i] = (byte)fs.ReadByte();
                            }
                            frameCount = (int)BitConverter.ToUInt32(buffer, 0);

                            buffer = new byte[4];

                            for (int i = 0; i < 4; i++)
                            {
                                buffer[i] = (byte)fs.ReadByte();
                            }
                            frameLength = (float)BitConverter.ToSingle(buffer, 0);

                            break;
                        }
                    case 13: // ANIMATION_BONE_RTS
                        {
                            for (int i = 0; i < 34; i++)
                            {
                                fs.ReadByte();
                            }

                            buffer = new byte[4];

                            for (int i = 0; i < 4; i++)
                            {
                                buffer[i] = (byte)fs.ReadByte();
                            }
                            frameCount = (int)BitConverter.ToUInt32(buffer, 0);

                            buffer = new byte[4];

                            for (int i = 0; i < 4; i++)
                            {
                                buffer[i] = (byte)fs.ReadByte();
                            }
                            frameLength = (float)BitConverter.ToSingle(buffer, 0);

                            break;
                        }                        
                }

                if (frameCount != 0 && frameLength != 0) // ��ֵ��Ч
                {
                    aniFrameCount = (int)((frameCount - 1) * frameLength * 16f / 1000) + 1;
                }

                fs.Close();                
            }

            return aniFrameCount;
        }
    }
}
