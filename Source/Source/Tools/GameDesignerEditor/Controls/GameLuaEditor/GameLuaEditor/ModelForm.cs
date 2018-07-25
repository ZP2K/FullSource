using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DevComponents.DotNetBar;
using VisualEditor;
using DataBaseIO;


namespace GameLuaEditor
{
    public partial class ModelForm : Form
    {
        private string[] data; // ��ʾ��������
        private List<string> inputList = new List<string>(); // �û���������
        Doit doit = new Doit(); // �ı��༭����

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ʾ��������</param>
        public ModelForm(string[] data)
        {
            this.data = data;
            InitializeComponent();
            InitData();
        }

        /// <summary>
        /// �û�������������
        /// </summary>
        public List<string> InputList
        {
            get
            {
                return inputList;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData()
        {
            // ����label�Ŀ��
            int maxLabelLength = 0;
            int length = 0;
            foreach(string s in data)
            {
                string[] t = s.Split(new char[]{','});
                length = 0;
                foreach(char c in t[0].ToCharArray())
                {
                    if (c == 60 || c > 128) // �����ַ�
                    {
                        length += 15;
                    }
                    else // Ӣ���ַ�
                    {
                        length += 8;
                    }
                }

                if(length > maxLabelLength)
                {
                    maxLabelLength = length;
                }
            }

            for(int i = 0; i < data.Length; i++)
            {
                string[] values = data[i].Split(new char[] {','});

                ItemContainer container = new ItemContainer();
                LabelItem label = new LabelItem("labelItem" + i, values[0]);
                label.Width = maxLabelLength;
                TextBoxItem textBox = new TextBoxItem("textBoxItem" + i, values[2]);
                textBox.Tag = values[1];
                textBox.TextBoxWidth = 200;
               
                textBox.TextBox.BorderStyle = BorderStyle.Fixed3D;
                textBox.TextBox.Text = values[2];
                container.SubItems.Add(label);
                container.SubItems.Add(textBox);
                switch(values[1]) // ������������
                {
                    case "text": // �ı�����
                        {
                            ButtonItem buttonItem = new ButtonItem("buttonItem" + i, "�༭");
                            buttonItem.Click += new EventHandler(editText);
                            buttonItem.Tag = textBox.TextBox; // ���ı���
                            container.SubItems.Add(buttonItem);
                            while (maxLabelLength + 270 + 30 > this.Width) // �Զ��������
                            {
                                this.Width += 30;
                            }
                            break;
                        }
                    case "int": // ��������
                        {
                            textBox.TextBox.TextChanged += new EventHandler(textBoxTextChanged);
                            while (maxLabelLength + 270 > this.Width) // �Զ��������
                            {
                                this.Width += 30;
                            }
                            break;
                        }
                    case "readonly": // ֻ������
                        {
                            textBox.Enabled = false;
                            while (maxLabelLength + 270 > this.Width) // �Զ��������
                            {
                                this.Width += 30;
                            }
                            break;
                        }
                    case "list": // �����˵�����
                        {
                            ComboBoxItem comboBox = new ComboBoxItem("comboBox" + i, "");
                            string s = values[2];
                            string[] sections = s.Split(new char[]{'@'}, StringSplitOptions.RemoveEmptyEntries);
                            switch(sections.Length)
                            {
                                case 1: // ���������б�ֵ
                                    {
                                        if(s.Contains("@")) // ���ı���������
                                        {
                                            textBox.TextBox.Text = ""; // �����ı�����
                                            string[] t = sections[0].Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                                            foreach (string r in t)
                                            {
                                                comboBox.Items.Add(r);
                                            }
                                            comboBox.SelectedIndex = 0;
                                            container.SubItems.Add(comboBox);

                                            while (maxLabelLength + 270 + comboBox.ComboWidth> this.Width) // �Զ��������
                                            {
                                                this.Width += 30;
                                            }
                                        }
                                        else // �����ı���������
                                        {
                                            container.SubItems.Remove(textBox);
                                            string[] t = sections[0].Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                                            foreach(string r in t)
                                            {
                                                comboBox.Items.Add(r);
                                            }
                                            comboBox.SelectedIndex = 0;
                                            comboBox.ComboWidth = 200;
                                            container.SubItems.Add(comboBox);

                                            while (maxLabelLength + 270 > this.Width) // �Զ��������
                                            {
                                                this.Width += 30;
                                            }
                                        }
                                      
                                        break;
                                    }
                                case 2: // �����ݺ��б�ֵ
                                    {
                                        textBox.TextBox.Text = sections[0]; // �����ı�����
                                        string[] t = sections[1].Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                                        foreach(string r in t)
                                        {
                                            comboBox.Items.Add(r);
                                        }
                                        comboBox.SelectedIndex = 0;
                                        container.SubItems.Add(comboBox);

                                        while (maxLabelLength + 270 + comboBox.ComboWidth > this.Width) // �Զ��������
                                        {
                                            this.Width += 30;
                                        }
                                        break;
                                    }
                            }
                            break;
                        }
                }                
                
                itemPanel1.Items.Add(container);
            }
        }

        /// <summary>
        /// ��������
        /// </summary>
        private void SaveData()
        {
            foreach(object o in itemPanel1.Items)
            {
                string text = string.Empty;
                ItemContainer container = o as ItemContainer;
                TextBoxItem textBox = container.SubItems[1] as TextBoxItem;
                if(textBox != null) // �����ı���
                {
                    text += textBox.TextBox.Text;
                    if ((container.SubItems.Count == 3) && (container.SubItems[2] is ComboBoxItem)) // ��������������
                    {
                        text += (container.SubItems[2] as ComboBoxItem).SelectedItem.ToString();
                    }
                }
                else // �������ı��򣬼��������
                {
                    if ((container.SubItems.Count == 2) && (container.SubItems[1] is ComboBoxItem)) // ��������������
                    {
                        text += (container.SubItems[1] as ComboBoxItem).SelectedItem.ToString();
                    }
                }
                
                inputList.Add(text);
            }
        }

        /// <summary>
        /// �༭�ı��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void editText(object sender, EventArgs e)
        {
            ButtonItem buttonItem = sender as ButtonItem;
            TextBox textBox = buttonItem.Tag as TextBox;
            System.Data.SqlClient.SqlConnection conn = DataBaseManager.GetDataBaseManager().Connection;
            string result = doit.show(textBox.Text, ref conn, Helper.ClientPath);
            textBox.Text = result;
        }

        /// <summary>
        /// �ı������ݸı��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void textBoxTextChanged(object sender, EventArgs e)
        {
            TextBox textBox = sender as TextBox;
            string text = textBox.Text;
            int tempInt;
            if (!int.TryParse(text, out tempInt))
            {
                MessageBox.Show("����ֵ����Ϊ����!");
            }
        }

        /// <summary>
        /// �û�ѡ��ȷ��
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
    }
}