using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace VarManager
{
    public partial class SpaceForm : Form
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="table">�������ݱ�</param>
        public SpaceForm(DataTable table)
        {
            InitializeComponent();
            canvas.Init(table);
        }
    }
}