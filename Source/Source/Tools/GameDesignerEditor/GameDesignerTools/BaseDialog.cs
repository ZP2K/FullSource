using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace GameDesingerTools
{
    public partial class BaseDialog : Form
    {
        private string modelName; // ģ������
        private Hashtable propertyTable; // ����hash�����
        BaseForm bForm; // BaseForm����
        public bool useHideMode = false;

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="modelName">ģ������</param>
        public BaseDialog(string modelName)
        {
            this.modelName = modelName;
            InitializeComponent();
            InitBaseForm();
            this.Text = modelName;
        }

        /// <summary>
        /// ����hash�����
        /// </summary>
        public Hashtable PropertyTable
        {
            get
            {
                return propertyTable;
            }
        }

        /// <summary>
        /// ��ʼ��BaseForm�Ӵ���
        /// </summary>
        private void InitBaseForm()
        {
            try
            {
                bForm = new BaseForm(modelName);
                bForm.MdiParent = this;
                bForm.WindowState = FormWindowState.Maximized;      
                bForm.Show();
                bForm.Update();     
            }
            catch (Exception ex)
            {
                if (bForm != null)
                    bForm.Close();
                MessageBox.Show(ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Stop);
            }
        }

        /// <summary>
        /// ��ȡBaseForm���ض�������ֵ
        /// </summary>
        /// <param name="propertyName">���Ե�����</param>
        /// <returns>���Ե�ֵ</returns>
        public object GetPropertyValue(string propertyName)
        {
            return propertyTable[propertyName];
        }

        private void CloseOrHideForm()
        {
            // ���������ش��棬����򿪶�Σ����Ч�ʡ�
            if (this.useHideMode)
            {
                this.Hide();
            }
            else
            {
                foreach (Form f in this.MdiChildren) // �ر��Ӵ���
                {
                    f.Close();
                }
                this.Close();
            }
        }

        /// <summary>
        /// �رնԻ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            CloseOrHideForm();
        }

        /// <summary>
        /// �û�ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (this.useHideMode || propertyTable == null)
            {
                propertyTable = bForm.GetPropertyGridData();
            }

            if(propertyTable.Count != 0)
            {
                DialogResult = DialogResult.OK;
            }

            CloseOrHideForm();
        }

        /// <summary>
        /// �ؼ��ֹ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void bFind_Click(object sender, EventArgs e)
        {
            FiltrateForm filtrateForm = new FiltrateForm(bForm.BaseTree);
            DialogResult result = filtrateForm.ShowDialog();
        }
    }
}