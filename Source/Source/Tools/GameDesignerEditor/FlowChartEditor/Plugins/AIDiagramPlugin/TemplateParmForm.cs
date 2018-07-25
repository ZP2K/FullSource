using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using DevComponents.DotNetBar;
using DevComponents.AdvTree;

using DataInteractor;

namespace AIDiagramPlugin
{
    public partial class TemplateParmForm : Office2007Form
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        public TemplateParmForm()
        {
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            codeEditBox.ForWho = "GameLuaEditor";
            FlowChartInteractor it = Interactor.GetInteractor().CurrentFlowChartInteractor;
            codeEditBox.Text = it.CustomText;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (tabControl1.SelectedTabIndex == 0) // ��ǰ�Ƿ��ڱ༭�ű�
            {
                if (!AIDiagramHelper.CheckScript(codeEditBox, codeEditBox.Text)) // ���ű�
                {
                    if (MessageBox.Show("û��ͨ���ű���飬�Ƿ����±༭�ű���", "�ű����",
                                        MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
                    {
                        return;
                    }
                }
            }

            // ��������
            FlowChartInteractor it = Interactor.GetInteractor().CurrentFlowChartInteractor;           

            if (codeEditBox.Text != "")
            {
                it.CustomText = codeEditBox.Text;
            }            

            this.DialogResult = DialogResult.OK;
            this.Close();
        }        

        /// <summary>
        /// ���ܸ�֪
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void codeEditBox_queryObjects(object sender, luaEditor.QueryEventsArgs e)
        {
            AIDiagramHelper.QueryObjects(sender, e);            
        }
    }
}