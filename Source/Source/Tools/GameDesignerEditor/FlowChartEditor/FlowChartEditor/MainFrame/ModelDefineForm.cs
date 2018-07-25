using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using DevComponents.DotNetBar;

using DBManager;

namespace FlowChartEditor
{
    public partial class ModelDefineForm : Office2007Form
    {
        DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager(); // ���ݿ������
        private string chartClass; // ����ͼ����
        private string nodeType; // �������
        private string modelName; // ģ������
        private string modelDescription; // ģ������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="chartClass">����ͼ����</param>
        /// <param name="nodeType">�������</param>
        public ModelDefineForm(string chartClass, string nodeType)
        {
            this.chartClass = chartClass;
            this.nodeType = nodeType;
            InitializeComponent();
            textBoxX3.Text = chartClass;
            textBoxX4.Text = nodeType;
        }

        /// <summary>
        /// ģ������
        /// </summary>
        public string ModelName
        {
            get
            {
                return modelName;
            }
        }

        /// <summary>
        /// ģ������
        /// </summary>
        public string ModelDescription
        {
            get
            {
                return modelDescription;
            }
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            modelName = textBoxX1.Text;
            modelDescription = textBoxX2.Text;

            if(modelName != "" && modelDescription != "")
            {
                if(!dataBaseManager.ModelNameExist(modelName, chartClass, nodeType))
                {
                    DialogResult = DialogResult.OK;
                    this.Close();
                }
                else
                {
                    MessageBox.Show("��ǰ�����ģ�������Ѿ����ڣ�", "�Զ���ģ��",
                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                }                
            }
            else
            {
                MessageBox.Show("�����������Ĳ�����Ϣ��", "�Զ���ģ��",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
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
    }
}