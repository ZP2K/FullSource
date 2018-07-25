using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace GameDesingerTools
{
    public partial class AttachAttribute : Form
    {
        private ImageList imageList; // ͼƬ����
        private object dragItem; // �϶���Item
        private enum DragView { view1, view2, none } // �϶���Դ����ö��
        private enum State { unpair, pair } // ����״̬ö��
        private DragView view = DragView.none; // �϶���Դ����
        private State state = State.pair; // ����״̬
        private List<string[]> dataList = new List<string[]>(); // ������������
        private List<string> attributeList1 = new List<string>(); // ��������1
        private List<string> attributeList2 = new List<string>(); // ��������2 
        private List<string> existAttributes1 = new List<string>(); // ���ڵ���������1
        private List<string> existAttributes2 = new List<string>(); // ���ڵ���������2

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="imageList">ͼƬ����</param>
        public AttachAttribute(ImageList imageList)
        {
            this.imageList = imageList;
            InitializeComponent();
            listView1.SmallImageList = imageList;
            listView1.LargeImageList = imageList;
            listView1.StateImageList = imageList;
            listView2.SmallImageList = imageList;
            listView2.LargeImageList = imageList;
            listView2.StateImageList = imageList;
        }

        /// <summary>
        /// ����������������
        /// </summary>
        public List<string[]> DataList
        {
            get
            {
                return dataList;
            }
        }

        /// <summary>
        /// ��������1����
        /// </summary>
        public List<string> AttributeList1
        {
            set
            {
                attributeList1 = value;
            }
        }

        /// <summary>
        /// ��������2����
        /// </summary>
        public List<string> AttributeList2
        {
            set
            {
                attributeList2 = value;
            }
        }

        /// <summary>
        /// ������������1
        /// </summary>
        public List<string> ExistAttributes1
        {
            set
            {
                existAttributes1 = value;
            }
        }

        /// <summary>
        /// ������������2
        /// </summary>
        public List<string> ExistAttributes2
        {
            set
            {
                existAttributes2 = value;
            }
        }

        /// <summary>
        /// �������
        /// </summary>
        public void FillContent()
        {
            foreach (string s in attributeList1) // ������������б�
            {
                ListViewItem item = listView1.Items.Add(s);
                item.ImageIndex = 2;
            }

            foreach (string s in attributeList2) // ��������ֶα������б�
            {
                ListViewItem item = listView2.Items.Add(s);
                item.ImageIndex = 2;
            }

            for(int i = 0; i < existAttributes1.Count; i++) // ��������ϵ�б�
            {
                listBox3.Items.Add(existAttributes1[i] + "  ����  " + existAttributes2[i]);
                dataList.Add(new string[] { existAttributes1[i], existAttributes2[i] });
            }
        }

        /// <summary>
        /// �϶�����ؼ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void listBox3_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.Text))
            {
                e.Effect = DragDropEffects.Move;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        /// <summary>
        /// �϶����´���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void listBox3_DragDrop(object sender, DragEventArgs e)
        {
            string dummy = "temp";
            string text = "";
            //��ý���"Drag"�������϶����ַ���
            string info = e.Data.GetData(dummy.GetType()) as string;
            string regexPattern = @"ListViewItem: \{(?<value>.*)\}";
            Regex r = new Regex(regexPattern);

            if (r.IsMatch(info))
            {
                text = r.Match(info).Result("${value}");
            }
            else
            {
                return;
            }

            if (state == State.pair && view == DragView.view2)
            {
                MessageBox.Show("���������������е��ֶ�!");
                return;
            }

            if (state == State.unpair && view == DragView.view1)
            {
                MessageBox.Show("�����е��ֶ��Ѿ����ù�!");
                return;
            }
            
            int count = listBox3.Items.Count;

            switch (view)
            {
                case DragView.none:
                    {
                        break;
                    }
                case DragView.view1:
                    {
                        dataList.Add(new string[] { "", "" });
                        listView1.Items.Remove(dragItem as ListViewItem);
                        state = State.unpair;
                        dataList[count][0] = text;
                        listBox3.Items.Add(text);
                        break;
                    }
                case DragView.view2:
                    {
                        listView2.Items.Remove(dragItem as ListViewItem);
                        state = State.pair;
                        dataList[count - 1][1] = text;
                        text = dataList[count - 1][0] + " - " + text;
                        listBox3.Items[count - 1] = text;
                        break;
                    }
            }

            listBox3.SelectedItem = text;
        }

        /// <summary>
        /// �û��϶����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void listView1_ItemDrag(object sender, ItemDragEventArgs e)
        {
            dragItem = e.Item;
            view = DragView.view1;
            string strItem = e.Item.ToString();
            //��ʼ����"Drag"����
            DoDragDrop(strItem, DragDropEffects.Move);
        }

        /// <summary>
        /// �û��϶����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void listView2_ItemDrag(object sender, ItemDragEventArgs e)
        {
            dragItem = e.Item;
            view = DragView.view2;
            string strItem = e.Item.ToString();
            //��ʼ����"Drag"����
            DoDragDrop(strItem, DragDropEffects.Move);
        }

        /// <summary>
        /// �û�ѡ��ɾ������
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (listBox3.SelectedItem != null)
            {
                object o = listBox3.SelectedItem;
                int index = listBox3.SelectedIndex;
                if (dataList[index][0] != "")
                {
                    ListViewItem item = listView1.Items.Add(dataList[index][0]);
                    item.ImageIndex = 2;
                }
                if (dataList[index][1] != "")
                {
                    ListViewItem item = listView2.Items.Add(dataList[index][1]);
                    item.ImageIndex = 2;
                }
                if(index == listBox3.Items.Count -1)
                {
                    state = State.pair;
                }
                dataList.RemoveAt(index);
                listBox3.Items.Remove(o);
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            int count = listBox3.Items.Count;
            if (count > 0 && dataList[count - 1][1] == "")
            {
                MessageBox.Show("������ϵ������!");
                return;
            }
            DialogResult = DialogResult.OK;
            this.Close();
        }

        /// <summary>
        /// �û�ѡ��ȡ��
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }
    }
}