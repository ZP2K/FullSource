using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
using System.Collections;
using DataElementLibrary;

namespace TextEditor
{
    [Serializable]
    public class TextEditor: DataElement
    {        
        /// <summary>
        /// ���캯��
        /// </summary>
        public TextEditor()
        {
           
        }

        /// <summary>
        /// �༭����
        /// </summary>
        /// <param name="table">��������</param>
        /// <return>�Ƿ�༭�ɹ�</return>
        public override bool EditData(Hashtable table)
        {
            bool editSuccess = false;
            string content = table["data"] as string;
            if(content == null)
            {
                content = "������༭����";
            }
            MainForm mainForm = new MainForm(content);
            DialogResult result = mainForm.ShowDialog();
            if(result == DialogResult.OK)
            {
                text = mainForm.Content;
                data = mainForm.Content;                
                editSuccess = true;
            }

            return editSuccess;
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <return>�Ƿ񱣴�ɹ�</return>
        public override bool SaveData()
        {
            return true;
        }

        /// <summary>
        /// ɾ������
        /// </summary>
        /// <returns>�Ƿ�ɾ���ɹ�</returns>
        public override bool DeleteData()
        {
            return true;
        }

        /// <summary>
        /// �ض�����
        /// </summary>
        /// <param name="table">�ض�����</param>
        /// <return>�Ƿ��ض��ɹ�</return>
        public override bool ReloadData(Hashtable table)
        {
            return false;
        }

        /// <summary>
        /// ��ȡ����Ĳ�������
        /// </summary>
        /// <returns>����Ĳ�������</returns>
        public override Hashtable GetOutputArgs()
        {
            return null;
        }
    }
}