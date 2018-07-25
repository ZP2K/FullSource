using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Reflection;
using System.Drawing;
using System.IO;

namespace BaseExp.SearchComboBox
{
    public class SearchComboBoxEx : DevComponents.DotNetBar.Controls.ComboBoxEx
    {
        private ArrayList m_arrlist_All = new ArrayList();
        ArrayList itemObjectList = new ArrayList(); // ���������ݶ����б�
        static Random rnd = new Random(); // ��������� 

        [DllImport("user32.dll")]
        private static extern IntPtr FindWindowExA(IntPtr hWnd1, IntPtr hWnd2, string lpsz1, string lpsz2);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern IntPtr SendMessage(IntPtr hwnd, int wMsg, int length,
            [MarshalAs(UnmanagedType.LPTStr)]StringBuilder lParam);

        [DllImport("user32.dll", EntryPoint = "SendMessageA", CharSet = CharSet.Auto)]
        private static extern int SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);

        private const int WM_GETTEXT = 0xd;
        private const int WM_KEYDOWN = 0x100;

        /// <summary>
        /// ���캯��
        /// </summary>
        public SearchComboBoxEx()
        {
            this.DrawMode = DrawMode.OwnerDrawFixed;
            this.DisplayMember = "Text";
            this.Images = StandardImageProvider.ImgList;
        }

        /// <summary>
        /// Ϊ�˽��Window��bug,��Items��ĿΪ0�����������˵���������״̬����ȡText������⡣
        /// ����ֻ���ֹ��������������bug��FindWindow���ٴ�comboBox������Ǹ�Edit��ȡ��Text
        /// </summary>
        public override string Text
        {
            get
            {
                if (base.Items.Count > 0)
                {
                    return base.Text;
                }
                else
                {
                    IntPtr ip = FindWindowExA(this.Handle, IntPtr.Zero, null, null);

                    if (ip != IntPtr.Zero)
                    {
                        StringBuilder sb = new StringBuilder(256);
                        SendMessage(ip, WM_GETTEXT, 255, sb);

                        return sb.ToString();
                    }

                    return "";
                }
            }

            set 
            { 
                base.Text = value; 
            }
        }


        protected override void OnEnter(EventArgs e)
        {
            foreach (object i in base.Items)
            {
                string strPy = "";
                strPy = Chinese2Spell.GetHeadOfChs(i.ToString());
                strPy = strPy.Replace(" ", "").ToLower();
                m_arrlist_All.Add(new object[] { i, strPy });
            }

            base.OnEnter(e);
        }

        protected override void OnLeave(EventArgs e)
        {
            foreach (object[] i in m_arrlist_All)
            {
                object i1 = i[0];
                if (!base.Items.Contains(i1)) base.Items.Add(i1);
            }
            this.m_arrlist_All.Clear();

            base.OnLeave(e);
        }

        protected override void OnTextUpdate(EventArgs e)
        {
            string oldText = this.Text;

            List<object> list = new List<object>();
            foreach (object[] i in m_arrlist_All)
            {
                object i1 = i[0];   //���ݶ���
                object i2 = i[1];   //ƴ���ַ���
                bool bShow = true;
                if (this.Text.Length > 0)
                {
                    string[] strKeys = this.Text.Split(new string[] { " " }, StringSplitOptions.RemoveEmptyEntries);
                    foreach (string strKey in strKeys)
                    {
                        if (i2.ToString().IndexOf(strKey) == -1)
                        {
                            bShow = false;
                            break;
                        }
                    }
                }
                if (bShow)
                {
                    if (!base.Items.Contains(i1)) list.Add(i1); //Items.Add(i1);
                }
                else
                {
                    if (base.Items.Contains(i1)) base.Items.Remove(i1);
             
                }
            }
            base.Items.AddRange(list.ToArray());
            if (base.Items.Count > 0)
            {
                this.DroppedDown = true;
            }
            this.Cursor = System.Windows.Forms.Cursors.Arrow;
            base.OnTextUpdate(e);
            
            // ̫����
            if (this.Text != oldText)
            {
                string report = string.Format("old text: {0}\r\nnew text:{1}", oldText, this.Text);
                System.Diagnostics.Debug.WriteLine(report);
                
                this.Text = oldText;
                IntPtr hwnd = FindWindowExA(this.Handle, IntPtr.Zero, null, null);
                if (hwnd != IntPtr.Zero)
                {
                    int vk_end = 0x23;
                    SendMessage(hwnd, WM_KEYDOWN, vk_end, 0);
                }
            }
        }

        /// <summary>
        /// �����������������
        /// </summary>
        /// <param name="objs">��Ӷ��󼯺�</param>
        public void Items_AddRange(object[] objs)
        {
            foreach(object o in objs)
            {
                Items_Add(o);
            }
        }        

        /// <summary>
        /// �������������
        /// </summary>
        /// <param name="obj">��Ӷ���</param>
        public void Items_Add(object obj)
        {
            DevComponents.Editors.ComboItem item = new DevComponents.Editors.ComboItem();
 
            item.ImageIndex = rnd.Next(400);
            item.Tag = obj;
            item.Text = obj.ToString();
            base.Items.Add(item);
            itemObjectList.Add(obj);
        }

        /// <summary>
        /// �������������
        /// </summary>
        /// <param name="obj">��Ӷ���</param>
        /// <param name="imageIndex">ͼ����</param>
        public void Items_Add(object obj, int imageIndex)
        {
            DevComponents.Editors.ComboItem item = new DevComponents.Editors.ComboItem();

            item.ImageIndex = imageIndex;
            item.Tag = obj;
            item.Text = obj.ToString();
            base.Items.Add(item);
            itemObjectList.Add(obj);
        }

        /// <summary>
        /// ��������Ŀ�б�
        /// </summary>
        public ArrayList Items_All
        {
            get
            {
                return itemObjectList;
            }
        }

        /// <summary>
        /// ����Items����
        /// </summary>
        public new ComboBox.ObjectCollection Items
        {
            set 
            { 
                ; 
            }
        }

        /// <summary>
        /// �������ǩ����
        /// </summary>
        public int Items_Count
        {
            get
            {
                return base.Items.Count;
            }
        }

        /// <summary>
        /// ѡ�е��������ǩ
        /// </summary>
        public object Items_SelectedItem 
        { 
            get
            {
                if (base.SelectedItem is DevComponents.Editors.ComboItem)
                {
                    return (base.SelectedItem as DevComponents.Editors.ComboItem).Tag;
                }
                else
                {
                    return null;
                }
            }

            set
            {
                foreach (DevComponents.Editors.ComboItem item in base.Items)
                {
                    if (item.Tag == value)
                    {
                        this.SelectedItem = item;
                        break;
                    }
                }
            }
        }
    }

    /// <summary>
    /// �ṩͼ��Դ
    /// </summary>
    public class StandardImageProvider
    {
        private static ImageList imglist = null;

        public static ImageList ImgList
        {
            get
            {
                if (imglist == null)
                {
                    imglist = new ImageList();
                    Assembly asm = Assembly.GetExecutingAssembly();

                    foreach (string resName in asm.GetManifestResourceNames())
                    {
                        if (resName.EndsWith(".png"))
                        {
                            imglist.Images.Add(Image.FromStream(asm.GetManifestResourceStream(resName)));
                        }
                    }

                }

                return imglist;
            }
        }

        private StandardImageProvider()
        {
 
        }
    }
}
