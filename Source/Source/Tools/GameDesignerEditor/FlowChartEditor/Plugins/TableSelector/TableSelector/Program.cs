using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Collections;

namespace TableSelector
{
    public class Program
    {
        TreeForm tForm;

        /// <summary>
        /// ���캯��
        /// </summary>
        public Program()
        {

        }

        /// <summary>
        /// ѡ���ֵ
        /// </summary>
        public String SelectedValue
        {
            get
            {
                return tForm.SelectedValue;
            }
        }

        /// <summary>
        /// ѡ���ֵ��
        /// </summary>
        public Hashtable SelectedValues
        {
            get
            {
                return tForm.SelectedValues;
            }
        }

        /// <summary>
        /// ��ʾ�༭����
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="title">�������</param>
        /// <param name="tableName">����</param>
        /// <param name="classifyField">�����ֶ�</param>
        /// <param name="returnField">����ֵ�ֶ�</param>
        /// <returns>�Ƿ�ȷ�ϱ༭</returns>
        public bool Show(SqlConnection conn, string title, string tableName, string classifyField, string returnField, string oldValue)
        {
            bool success = false; // �Ƿ�༭�ɹ�

            tForm = new TreeForm(conn, title, tableName, classifyField, returnField, oldValue);
            DialogResult result = tForm.ShowDialog();
            if(result == DialogResult.OK)
            {
                success = true;
            }

            return success;
        }


        private TreeForm m_form = null; 
        //��ʼ��
        public bool Init(SqlConnection conn, string title, string sql, string classifyField, string returnField)
        {
            m_form = new TreeForm(conn, title, sql, classifyField, returnField, "");
            return false;
        }

        public bool ShowAgain(string oldValue)
        {
            return false;
        }
    }
}
