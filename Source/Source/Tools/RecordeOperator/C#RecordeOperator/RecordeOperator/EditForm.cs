using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using AtlKGRecordeOperatorLib;

namespace RecordeOperator
{
    public partial class EditForm : Form
    {
        string m_strType = null;

        private int[] nCount = new int[18];

        Label[] aLabel = new Label[6];
        TextBox[] aTextBox = new TextBox[6];
        Button Button1 = new Button();

        string m_strValue = null;
        string m_strName  = null;
        int    m_nNum     = -1;
        
        public EditForm()
        {
            InitializeComponent();
        }

        ~EditForm()
        {

        }

        public void Init(string strType)
        {
            m_strType = strType;

            if (strType == "Model" || strType == "ModelST" || strType == "SFX")
            {
                ListBox.Items.Add("�����������");
                ListBox.Items.Add("����λ������");
                ListBox.Items.Add("������ת����");
                ListBox.Items.Add("������ת��������");
                ListBox.Items.Add("������������");
                ListBox.Items.Add("����������������");
                ListBox.Items.Add("����������ת����");
                ListBox.Items.Add("����λ��������ת");
                ListBox.Items.Add("�󶨹ؽ�����");
                ListBox.Items.Add("�󶨹�������");
                ListBox.Items.Add("�󶨶�������");
                ListBox.Items.Add("�����������");
                ListBox.Items.Add("���Ŷ�������");
                ListBox.Items.Add("����SPLIT��������");
                ListBox.Items.Add("���ز���ѡ��");
                ListBox.Items.Add("���ö���͸������");
                ListBox.Items.Add("���ö���ص�����");
                ListBox.Items.Add("��������");

                InitCount();
            }
            else if (strType == "Camera")
            {
                ListBox.Items.Add("���þ�ͷλ��");
                ListBox.Items.Add("���þ�ͷĿ��λ��");
                ListBox.Items.Add("���þ�ͷ�Ϸ���");
                //FT_KG3DCAMERA_SETTRACKINFO
                //FT_KG3DCAMERA_UPDATECAMERA
                //FT_KG3DCAMERA_SETGLOBALPERSPECTIVE
                //FT_KG3DCAMERA_SETGLOBALORTHOGONAL

                InitCount();
            }
            else if (strType == "SceneEditor")
            {
                ListBox.Items.Add("������������");
                ListBox.Items.Add("��������");
                ListBox.Items.Add("������Ⱦ����");
                ListBox.Items.Add("ɾ����Ⱦ����");
                //FT_KG3DSCENEEDITOR_ADDOUTPUTWINDOW

                InitCount();
            }

            aLabel[0] = new Label();
            aLabel[1] = new Label();
            aLabel[2] = new Label();
            aLabel[3] = new Label();
            aLabel[4] = new Label();
            aLabel[5] = new Label();

            aTextBox[0] = new TextBox();
            aTextBox[1] = new TextBox();
            aTextBox[2] = new TextBox();
            aTextBox[3] = new TextBox();
            aTextBox[4] = new TextBox();
            aTextBox[5] = new TextBox();
        }

        private void InitCount()
        {
            if (m_strType == "Model" || m_strType == "ModelST" || m_strType == "SFX")
            {
                nCount[7] = nCount[16] = nCount[17] = 0;
                nCount[10] = nCount[11] = 1;
                nCount[8] = nCount[9] = nCount[14] = nCount[15] = 2;
                nCount[0] = nCount[1] = nCount[3] = nCount[4] = nCount[5] = 3;
                nCount[2] = nCount[6] = nCount[12] = 4;
                nCount[13] = 6;
            }
            else if (m_strType == "Camera")
            {
                nCount[0] = nCount[1] = nCount[2] = 3;
            }
            else if (m_strType == "SceneEditor")
            {
                nCount[0] = nCount[1] = nCount[2] = 3;
            }
            return;
        }

        private void ListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            int nIndex = ListBox.SelectedIndex;
            PanelShow(nIndex);

            if (m_strType == "Model" || m_strType == "ModelST" || m_strType == "SFX")
            {
                switch (nIndex)
                {
                    case 0:
                        aLabel[0].Text = "�����ļ�·��";
                        aLabel[1].Text = "�����ļ���ϣ";
                        aLabel[2].Text = "�����ļ�ѡ��";
                        Button1.Text = "...";
                        panel1.Controls.Add(Button1);
                        aLabel[0].Size = new System.Drawing.Size(77, 12);
                        aLabel[1].Size = new System.Drawing.Size(77, 12);
                        aLabel[2].Size = new System.Drawing.Size(77, 12);
                        aTextBox[0].Location = new System.Drawing.Point(110, 30);
                        aTextBox[1].Location = new System.Drawing.Point(110, 80);
                        aTextBox[2].Location = new System.Drawing.Point(110, 130);
                        Button1.Location = new System.Drawing.Point(216, 27);
                        Button1.Size = new System.Drawing.Size(40, 25);
                        break;
                    case 1:
                        aLabel[0].Text = "λ��x����";
                        aLabel[1].Text = "λ��y����";
                        aLabel[2].Text = "λ��z����";
                        break;
                    case 2:
                        aLabel[0].Text = "��תx����";
                        aLabel[1].Text = "��תy����";
                        aLabel[2].Text = "��תz����";
                        aLabel[3].Text = "��תw����";
                        break;
                    case 3:
                        aLabel[0].Text = "��ת����x����";
                        aLabel[1].Text = "��ת����y����";
                        aLabel[2].Text = "��ת����z����";
                        break;
                    case 4:
                        aLabel[0].Text = "����x����";
                        aLabel[1].Text = "����y����";
                        aLabel[2].Text = "����z����";
                        break;
                    case 5:
                        aLabel[0].Text = "��������x����";
                        aLabel[1].Text = "��������y����";
                        aLabel[2].Text = "��������z����";
                        break;
                    case 6:
                        aLabel[0].Text = "������תx����";
                        aLabel[1].Text = "������תy����";
                        aLabel[2].Text = "������תz����";
                        aLabel[3].Text = "��ת��תw����";
                        break;
                    case 7:
                        break;
                    case 8:
                        aLabel[0].Text = "�󶨵�����ID";
                        aLabel[1].Text = "�󶨵��ؽ�����";
                        break;
                    case 9:
                        aLabel[0].Text = "�󶨵�����ID";
                        aLabel[1].Text = "�󶨵���������";
                        break;
                    case 10:
                        aLabel[0].Text = "�󶨵�����ID";
                        break;
                    case 11:
                        aLabel[0].Text = "�������ID";
                        break;
                    case 12:
                        aLabel[0].Text = "��������";
                        aLabel[1].Text = "����·��";
                        aLabel[2].Text = "�����ٶ�";
                        aLabel[3].Text = "��ʼʱ��";
                        break;
                    case 13:
                        aLabel[0].Text = "����·��";
                        aLabel[1].Text = "SPLIT����";
                        aLabel[2].Text = "������Ϣ";
                        aLabel[3].Text = "��������";
                        aLabel[4].Text = "�����ٶ�";
                        aLabel[5].Text = "��ʼʱ��";
                        break;
                    case 14:
                        aLabel[0].Text = "����·��";
                        aLabel[1].Text = "����ѡ��";
                        break;
                    case 15:
                        aLabel[0].Text = "����͸����";
                        aLabel[1].Text = "�Ƿ�ʹ������ģʽ";
                        break;
                    case 16:
                    case 17:
                        break;
                    default:
                        break;
                }
            }
            else if (m_strType == "Camera")
            {
                switch (nIndex)
                {
                    case 0:
                        aLabel[0].Text = "��ͷλ��x����";
                        aLabel[1].Text = "��ͷλ��y����";
                        aLabel[2].Text = "��ͷλ��z����";
                        break;
                    case 1:
                        aLabel[0].Text = "��ͷĿ��λ��x����";
                        aLabel[1].Text = "��ͷĿ��λ��y����";
                        aLabel[2].Text = "��ͷĿ��λ��z����";
                        break;
                    case 2:
                        aLabel[0].Text = "��ͷ�Ϸ���x����";
                        aLabel[1].Text = "��ͷ�Ϸ���y����";
                        aLabel[2].Text = "��ͷ�Ϸ���z����";
                        break;
                    default:
                        break;
                }
            }
            else if (m_strType == "SceneEditor")
            {
                switch (nIndex)
                {
                    case 0:
                        aLabel[0].Text = "�����ļ�·��";
                        aLabel[1].Text = "�����ļ���ϣ";
                        aLabel[2].Text = "�����ļ�ѡ��";
                        Button1.Text = "...";
                        panel1.Controls.Add(Button1);
                        aLabel[0].Size = new System.Drawing.Size(77, 12);
                        aLabel[1].Size = new System.Drawing.Size(77, 12);
                        aLabel[2].Size = new System.Drawing.Size(77, 12);
                        aTextBox[0].Location = new System.Drawing.Point(110, 30);
                        aTextBox[1].Location = new System.Drawing.Point(110, 80);
                        aTextBox[2].Location = new System.Drawing.Point(110, 130);
                        Button1.Location = new System.Drawing.Point(216, 27);
                        Button1.Size = new System.Drawing.Size(40, 25);
                        break;
                    case 1:
                        break;
                    case 2:
                    case 3:
                        aLabel[0].Text = "����ID";
                        aLabel[1].Text = "���";
                        aLabel[2].Text = "�Ƿ��ѡ��";
                        break;
                    default:
                        break;
                }
            }
        }

        private void PanelShow(int nIndex)
        {
            panel1.Controls.Clear();
            switch (nCount[nIndex])
            {
                case 0:
                    break;
                case 1:
                    panel1.Controls.Add(aLabel[0]);
                    panel1.Controls.Add(aTextBox[0]);
                    aLabel[0].Location = new System.Drawing.Point(17, 33);
                    aLabel[0].Size = new System.Drawing.Size(107, 12);
                    aTextBox[0].Location = new System.Drawing.Point(140, 30);
                    break;
                case 2:
                    panel1.Controls.Add(aLabel[0]);
                    panel1.Controls.Add(aLabel[1]);
                    panel1.Controls.Add(aTextBox[0]);
                    panel1.Controls.Add(aTextBox[1]);
                    aLabel[0].Location = new System.Drawing.Point(17, 41);
                    aLabel[0].Size = new System.Drawing.Size(107, 12);
                    aLabel[1].Location = new System.Drawing.Point(17, 111);
                    aLabel[1].Size = new System.Drawing.Size(107, 12);
                    aTextBox[0].Location = new System.Drawing.Point(140, 38);
                    aTextBox[1].Location = new System.Drawing.Point(140, 108);
                    break;
                case 3:
                    panel1.Controls.Add(aLabel[0]);
                    panel1.Controls.Add(aLabel[1]);
                    panel1.Controls.Add(aLabel[2]);
                    panel1.Controls.Add(aTextBox[0]);
                    panel1.Controls.Add(aTextBox[1]);
                    panel1.Controls.Add(aTextBox[2]);
                    aLabel[0].Location = new System.Drawing.Point(17, 33);
                    aLabel[0].Size = new System.Drawing.Size(107, 12);
                    aLabel[1].Location = new System.Drawing.Point(17, 83);
                    aLabel[1].Size = new System.Drawing.Size(107, 12);
                    aLabel[2].Location = new System.Drawing.Point(17, 133);
                    aLabel[2].Size = new System.Drawing.Size(107, 12);
                    aTextBox[0].Location = new System.Drawing.Point(140, 30);
                    aTextBox[1].Location = new System.Drawing.Point(140, 80);
                    aTextBox[2].Location = new System.Drawing.Point(140, 130);
                    break;
                case 4:
                    panel1.Controls.Add(aLabel[0]);
                    panel1.Controls.Add(aLabel[1]);
                    panel1.Controls.Add(aLabel[2]);
                    panel1.Controls.Add(aLabel[3]);
                    panel1.Controls.Add(aTextBox[0]);
                    panel1.Controls.Add(aTextBox[1]);
                    panel1.Controls.Add(aTextBox[2]);
                    panel1.Controls.Add(aTextBox[3]);
                    aLabel[0].Location = new System.Drawing.Point(17, 30);
                    aLabel[0].Size = new System.Drawing.Size(107, 12);
                    aLabel[1].Location = new System.Drawing.Point(17, 70);
                    aLabel[1].Size = new System.Drawing.Size(107, 12);
                    aLabel[2].Location = new System.Drawing.Point(17, 110);
                    aLabel[2].Size = new System.Drawing.Size(107, 12);
                    aLabel[3].Location = new System.Drawing.Point(17, 150);
                    aLabel[3].Size = new System.Drawing.Size(107, 12);
                    aTextBox[0].Location = new System.Drawing.Point(140, 27);
                    aTextBox[1].Location = new System.Drawing.Point(140, 67);
                    aTextBox[2].Location = new System.Drawing.Point(140, 107);
                    aTextBox[3].Location = new System.Drawing.Point(140, 147);
                    break;
                case 6:
                    panel1.Controls.Add(aLabel[0]);
                    panel1.Controls.Add(aLabel[1]);
                    panel1.Controls.Add(aLabel[2]);
                    panel1.Controls.Add(aLabel[3]);
                    panel1.Controls.Add(aLabel[4]);
                    panel1.Controls.Add(aLabel[5]);
                    panel1.Controls.Add(aTextBox[0]);
                    panel1.Controls.Add(aTextBox[1]);
                    panel1.Controls.Add(aTextBox[2]);
                    panel1.Controls.Add(aTextBox[3]);
                    panel1.Controls.Add(aTextBox[4]);
                    panel1.Controls.Add(aTextBox[5]);
                    aLabel[0].Location = new System.Drawing.Point(17, 15);
                    aLabel[0].Size = new System.Drawing.Size(107, 12);
                    aLabel[1].Location = new System.Drawing.Point(17, 46);
                    aLabel[1].Size = new System.Drawing.Size(107, 12);
                    aLabel[2].Location = new System.Drawing.Point(17, 77);
                    aLabel[2].Size = new System.Drawing.Size(107, 12);
                    aLabel[3].Location = new System.Drawing.Point(17, 108);
                    aLabel[3].Size = new System.Drawing.Size(107, 12);
                    aLabel[4].Location = new System.Drawing.Point(17, 139);
                    aLabel[4].Size = new System.Drawing.Size(107, 12);
                    aLabel[5].Location = new System.Drawing.Point(17, 170);
                    aLabel[5].Size = new System.Drawing.Size(107, 12);
                    aTextBox[0].Location = new System.Drawing.Point(140, 12);
                    aTextBox[1].Location = new System.Drawing.Point(140, 43);
                    aTextBox[2].Location = new System.Drawing.Point(140, 74);
                    aTextBox[3].Location = new System.Drawing.Point(140, 105);
                    aTextBox[4].Location = new System.Drawing.Point(140, 136);
                    aTextBox[5].Location = new System.Drawing.Point(140, 167);
                    break;
                default:
                    break;
            }
        }

        private void EventOK_Click(object sender, EventArgs e)
        {
            int nIndex = ListBox.SelectedIndex;
            m_strValue = null;
            m_strName  = null;
            m_nNum     = -1;
            if (nIndex < 0)
            {
                this.Close();
                return;
            }

            m_strName = ListBox.Items[nIndex].ToString();
            m_nNum = nCount[nIndex];
            for (int i = 0; i < m_nNum; i++)
            {
                m_strValue += aTextBox[i].Text;
                m_strValue += "\n";
            }
            this.Close();
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        public int GetNum()
        {
            return m_nNum;
        }

        public string GetName()
        {
            return m_strName;
        }

        public string GetValue()
        {
            return m_strValue;
        }
    }
}