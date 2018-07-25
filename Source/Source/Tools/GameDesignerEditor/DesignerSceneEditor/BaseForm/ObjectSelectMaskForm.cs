using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DesignerSceneEditor
{
	public partial class ObjectSelectMaskForm: Form
	{
        private int selectObjectTypeMask = 0; // ѡ���������
        private bool confirmSelection = false; // �Ƿ�ȷ��ѡ��

        /// <summary>
        /// ���캯��s
        /// </summary>
		public ObjectSelectMaskForm()
		{
			InitializeComponent();
		}        

        /// <summary>
        /// �Ƿ�ȷ��ѡ��
        /// </summary>
        public bool ConfirmSelection
        {
            get
            {
                return confirmSelection;
            }
        }

        /// <summary>
        /// ѡ���������
        /// </summary>
        public int SelectObjectTypeMask
        {
            get
            {
                return selectObjectTypeMask;
            }
        }

        /// <summary>
        /// ˢ������
        /// </summary>
        public void RefreshData()
        {
            confirmSelection = false;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            selectObjectTypeMask = 0;

            if (cNpc.Checked)
            {
                selectObjectTypeMask += 2;
            }
            if (cDoodad.Checked)
            {
                selectObjectTypeMask += (int)Math.Pow(2, 8);
            }
            if (cWayPoint.Checked)
            {
                selectObjectTypeMask += (int)Math.Pow(2, 9);
            }
            if (cPoly.Checked)
            {
                selectObjectTypeMask += (int)Math.Pow(2, 11);
            }
            if (cRefreshPoint.Checked)
            {
                selectObjectTypeMask += (int)Math.Pow(2, 10);
            }
            if (cTrafficPoint.Checked)
            {
                selectObjectTypeMask += (int)Math.Pow(2, 22);
            }

            if (checkboxAll.Checked)
            {
                selectObjectTypeMask = -1;
            }

            confirmSelection = true;
            this.Close();
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            confirmSelection = false;
            this.Close();
        }

        /// <summary>
        /// ����ر��¼�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void ObjectSelectMaskForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Visible = false;
        }
	}
}