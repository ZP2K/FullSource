using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace StandardNpcCreate
{
    public class ConfigClass
    {
        private ConfigForm cForm; // ѡ����
        private StringBuilder logText = new StringBuilder(); // ��־�ı�

        /// <summary>
        /// ���캯��
        /// </summary>
        public ConfigClass()
        {

        }

        /// <summary>
        /// �Ƿ���²��
        /// </summary>
        public bool UpdateSlot
        {
            get
            {
                return cForm.UpdateSlot;
            }
        }

        /// <summary>
        /// �Ƿ���¶���
        /// </summary>
        public bool UpdateAnimation
        {
            get
            {
                return cForm.UpdateAnimation;
            }
        }

        /// <summary>
        /// �Ƿ���¼���
        /// </summary>
        public bool UpdateSkill
        {
            get
            {
                return cForm.UpdateSkill;
            }
        }

        /// <summary>
        /// ��ʾ�༭����
        /// </summary>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public bool ShowEditForm()
        {
            bool editSuccess = false;

            cForm = new ConfigForm();
            if (cForm.ShowDialog() == DialogResult.OK)
            {
                editSuccess = true;
            }

            return editSuccess;
        }

        /// <summary>
        /// �����־�ı�
        /// </summary>
        /// <param name="appendText">�ı�����</param>
        public void AppendLogText(string appendText)
        {
            logText.AppendLine(appendText);
        }

        /// <summary>
        /// ��ʾ��־�ı�
        /// </summary>
        public void ShowLogText()
        {
            MessageBox.Show(logText.ToString(), "ˢ�½��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            logText = new StringBuilder();
        }
    }
}
