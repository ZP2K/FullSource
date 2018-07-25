using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace ArgumentsEditor
{
    public class Program
    {
        private EditForm eForm; // �����༭����
        private OperationEditForm oForm; // ���ʽ�༭���� 

        /// <summary>
        /// �����ַ���
        /// </summary>
        public string Value
        {
            get
            {
                return eForm.ArgumentsValue;
            }
        }

        /// <summary>
        /// ���ʽ
        /// </summary>
        public string Expression
        {
            get
            {
                return oForm.Value;
            }
        }

        /// <summary>
        /// ���������������
        /// </summary>
        //[STAThread]
        //public static void Main()
        //{
        //    SqlConnection conn = new SqlConnection("Server = 192.168.8.54; DataBase = FS2Settings; Uid = sa; Password = 1023;");
        //    OperationEditForm oForm = new OperationEditForm(conn, "");
        //    oForm.ShowDialog();
        //}

        /// <summary>
        /// ��ʾ�༭����
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="oldValue">��ֵ</param>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public bool Show(SqlConnection conn, string oldValue)
        {            
            eForm = new EditForm(conn, oldValue);
            bool editSuccess = (eForm.ShowDialog() == DialogResult.OK);

            return editSuccess;
        }

        /// <summary>
        /// �༭���ʽ
        /// </summary>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public bool EditExpression(SqlConnection conn, string oldValue)
        {
            oForm = new OperationEditForm(conn, oldValue);
            bool editSuccess = (oForm.ShowDialog() == DialogResult.OK);

            return editSuccess;
        }
    }
}
