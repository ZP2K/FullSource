using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace ArgumentsEditor
{
    public partial class EditForm : Form
    {
        private SqlConnection conn; // ���ݿ�����
        private string oldValue; // ��ֵ
        private string argumentsValue; // ����ֵ�ַ���
        private bool beginEdit = false; // �Ƿ��Ѿ���ʼ�༭

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">���ݿ�����</param>
        /// <param name="oldValue">��ֵ</param>
        public EditForm(SqlConnection conn, string oldValue)
        {
            this.conn = conn;
            this.oldValue = oldValue;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ����ֵ�ַ���
        /// </summary>
        public string ArgumentsValue
        {
            get
            {
                return argumentsValue;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ����ѡ��            
            List<string> list = new List<string>();
            list.Add("��");
            list.Add("����");
            list.Add("���ֶ�");

            // ��ȡ��ֵ
            string[] data = new string[]{"��", "��", "��"};
            if (oldValue != null)
            {
                string[] tempData = oldValue.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                for(int i = 0; i < tempData.Length && i < 3; i++)
                {
                    data[i] = tempData[i];
                }
            }

            comboBoxEx1.Text = data[0];
            comboBoxEx2.Text = data[1];
            comboBoxEx3.Text = data[2];
            beginEdit = true;
        }        

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if(comboBoxEx1.Text == "" || comboBoxEx2.Text == "" || comboBoxEx3.Text == "")
            {
                MessageBox.Show("�в���û����ȷ���������ͣ�", "��������",
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                bool previousNone = false;
                bool valid = true;
                argumentsValue = "";
                List<string> list = new List<string>();
                list.Add(comboBoxEx1.Text);
                list.Add(comboBoxEx2.Text);
                list.Add(comboBoxEx3.Text);

                for (int i = 0; i < 3; i++)
                {
                    string value = list[i];
                    if (value == "��")
                    {
                        previousNone = true;
                    }
                    else
                    {
                        if (previousNone)
                        {
                            MessageBox.Show("�в���û����ȷ���������ͣ�", "��������",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
                            valid = false;
                        }
                        else
                        {
                            previousNone = false;
                        }
                    }
                    argumentsValue += string.Format("{0};", value);
                }
                argumentsValue = argumentsValue.TrimEnd(new char[] { ';' });

                if (valid)
                {
                    DialogResult = DialogResult.OK;
                    this.Close();
                }
            }                        
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// �������͸ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxEx1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (beginEdit)
            {
                if (comboBoxEx1.SelectedIndex == 2)
                {
                    FieldSelectForm sForm = new FieldSelectForm(conn);
                    DialogResult result = sForm.ShowDialog();
                    if (result == DialogResult.OK)
                    {
                        beginEdit = false;
                        comboBoxEx1.Items[2] = sForm.Value;
                        beginEdit = true;
                    }
                }
            }
        }

        /// <summary>
        /// �������͸ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxEx2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (beginEdit)
            {
                if (comboBoxEx2.SelectedIndex == 2)
                {
                    FieldSelectForm sForm = new FieldSelectForm(conn);
                    DialogResult result = sForm.ShowDialog();
                    if (result == DialogResult.OK)
                    {
                        beginEdit = false;
                        comboBoxEx2.Items[2] = sForm.Value;
                        beginEdit = true;
                    }
                }
            }
        }

        /// <summary>
        /// �������͸ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxEx3_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (beginEdit)
            {
                if (comboBoxEx3.SelectedIndex == 2)
                {
                    FieldSelectForm sForm = new FieldSelectForm(conn);
                    DialogResult result = sForm.ShowDialog();
                    if (result == DialogResult.OK)
                    {
                        beginEdit = false;
                        comboBoxEx3.Items[2] = sForm.Value;
                        beginEdit = true;
                    }
                }
            }
        }     
    }
}