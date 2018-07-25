using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace VarManager
{
    public partial class EditForm : Form
    {
        DataRow dataRow; // ��ǰ������
        private Hashtable editData = new Hashtable(); // �༭������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="dataRow"></param>
        public EditForm(DataRow dataRow)
        {
            this.dataRow = dataRow;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// �༭������
        /// </summary>
        public Hashtable EditData
        {
            get
            {
                return editData;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            textBoxX1.Text = dataRow["ID"].ToString();
            textBoxX2.Text = dataRow["BindType"] as string;
            textBoxX3.Text = dataRow["BindObject"] as string;
            textBoxX4.Text = dataRow["BindType"] as string;
            textBoxX5.Text = dataRow["Name"] as string;
            textBoxX6.Text = dataRow["Description"] as string;
            integerInput1.Value = (int)dataRow["StartIndex"];
            integerInput2.Value = (int)dataRow["Length"];
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string id = dataRow["ID"].ToString();
            string newName = textBoxX5.Text;
            string newDescription = textBoxX6.Text;
            string newIndex = integerInput1.Value.ToString();
            string newLength = integerInput2.Value.ToString();

            if(newName != "" && newDescription != "") // ���벻��Ϊ��
            {
                if(newName != dataRow["Name"].ToString() ||
                    newDescription != dataRow["Description"].ToString() ||
                    newIndex != dataRow["StartIndex"].ToString() ||
                    newLength != dataRow["Length"].ToString()) // ��������Ƿ�Ϊ��ֵ
                {
                    editData["id"] = id;
                    editData["name"] = newName;
                    editData["description"] = newDescription;
                    editData["startIndex"] = newIndex;
                    editData["length"] = newLength;
                    DialogResult = DialogResult.OK;
                }
                else
                {
                    DialogResult = DialogResult.Cancel;
                }

                this.Close();
            }
            else
            {
                MessageBox.Show("���벻��Ϊ�գ�", "�޸ı���", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// �û�ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}