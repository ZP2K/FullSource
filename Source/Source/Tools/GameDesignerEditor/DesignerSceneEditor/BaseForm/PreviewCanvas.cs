using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.IO;
using System.Windows.Forms;
using System.Data.SqlClient;
using AtlKG3DEngineProxyLib;

namespace DesignerSceneEditor
{
	public class PreviewCanvas : Panel
	{
        private bool beginInit = false; // �Ƿ�ʼ��ʼ�� 
        private string previewType = "NPC"; // Ԥ������

        /// <summary>
        /// ���캯��
        /// </summary>
        public PreviewCanvas()
        {

        }

        /// <summary>
        /// Ԥ������
        /// </summary>
        public string PreviewType
        {
            set
            {
                previewType = value;
            }
        }

        /// <summary>
        /// �Ƿ�ʼ��ʼ��
        /// </summary>
        public bool BeginInit
        {
            set
            {
                beginInit = value;
            }
        }

        /// <summary>
        /// ��������С�ı��¼�
        /// </summary>
        /// <param name="eventargs">�¼�����</param>
        protected override void OnResize(EventArgs eventargs)
        {
            base.OnResize(eventargs);
            if(EngineLayer.ATLBase != null)
            {
                EngineLayer.ATLBase.OnResizeNpcWindow();
                EngineLayer.ATLBase.OnResizeDoodadWindow();
            }
        }

        /// <summary>
        /// ����ϵͳ��Ϣ
        /// </summary>
        /// <param name="msg">ϵͳ��Ϣ</param>
        protected override void WndProc(ref Message msg)
        {
            if (beginInit && EngineLayer.ATLBase != null)
            {
                int ret = 0;
                tagMSG tmsg = new tagMSG();
                tmsg.hwnd = (IntPtr)msg.HWnd;
                tmsg.lParam = (int)(msg.LParam);
                tmsg.message = Convert.ToUInt32(msg.Msg);
                tmsg.wParam = (uint)(msg.WParam);
                
                EngineLayer.ATLBase.OnWndMsg(ref tmsg, ref ret, previewType);
            }
            base.WndProc(ref msg);            
        }
	}
}
