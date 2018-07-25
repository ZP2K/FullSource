using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace AnimationAutoFill
{
    public partial class UpdateResultForm : Form
    {
        private int currentRowIndex = 0; // ��ǰ�к�

        /// <summary>
        /// ���캯��
        /// </summary>
        public UpdateResultForm()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ���ݳ�ʼ��
        /// </summary>
        private void Init()
        {
            DataGridViewTextBoxColumn rowIndexColumn = new DataGridViewTextBoxColumn();
            rowIndexColumn.Name = "RowIndex";
            rowIndexColumn.HeaderText = "�к�";

            DataGridViewTextBoxColumn fieldNameColumn = new DataGridViewTextBoxColumn();
            fieldNameColumn.Name = "FieldName";
            fieldNameColumn.HeaderText = "�ֶ���";

            DataGridViewTextBoxColumn animationNameColumn = new DataGridViewTextBoxColumn();
            animationNameColumn.Name = "AnimationName";
            animationNameColumn.HeaderText = "������";

            dataGridViewX1.Columns.Add(rowIndexColumn);
            dataGridViewX1.Columns.Add(fieldNameColumn);
            dataGridViewX1.Columns.Add(animationNameColumn);
        }

        /// <summary>
        /// ��յ�ǰ����
        /// </summary>
        public void Reset()
        {
            dataGridViewX1.Rows.Clear();
            currentRowIndex = 0;
        }

        /// <summary>
        /// ������־
        /// </summary>
        /// <param name="rowIndex">�к�</param>
        /// <param name="fieldName">�ֶ���</param>
        /// <param name="animationName">������</param>
        public void AppendLog(string rowIndex, string fieldName, string animationName)
        {
            dataGridViewX1.Rows.Add(1);
            DataGridViewRow newRow = dataGridViewX1.Rows[currentRowIndex];
            newRow.Cells["RowIndex"].Value = rowIndex;
            newRow.Cells["FieldName"].Value = fieldName;
            newRow.Cells["AnimationName"].Value = animationName;

            currentRowIndex++;
        }
    }
}