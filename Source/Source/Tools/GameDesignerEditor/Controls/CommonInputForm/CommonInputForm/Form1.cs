using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CommonInputForm
{
    public partial class Form1 : Form
    {
        public string strRet = string.Empty;

        public Form1()
        {
            InitializeComponent();
        }

        public Form1(string label, string oldstr)
        {
            InitializeComponent();
            this.label1.Text = label;
            this.textBoxX1.Text = oldstr;
        }

        private void buttonX1_Click(object sender, EventArgs e) // ȷ��
        {
            strRet = this.textBoxX1.Text;
            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void buttonX2_Click(object sender, EventArgs e) // ȡ��
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}