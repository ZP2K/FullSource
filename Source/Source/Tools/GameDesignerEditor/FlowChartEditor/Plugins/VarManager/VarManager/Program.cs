using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace VarManager
{
    public class Program
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        public Program()
        {

        }

        /// <summary>
        /// �����õ�������
        /// </summary>
        //[STAThread]
        //public static void Main()
        //{
        //    SqlConnection conn = new SqlConnection("Server = jx3web; DataBase = s3design_debug; Uid = s3design; Password = davidbowie");
        //    MainForm mForm = new MainForm(conn);
        //    DialogResult result = mForm.ShowDialog();
        //}

        /// <summary>
        /// ��ʾ�༭����
        /// </summary>
        /// <param name="conn">sql����</param>
        public void Show(SqlConnection conn)
        {
            MainForm mForm = new MainForm(conn);
            DialogResult result = mForm.ShowDialog();
        }

        /// <summary>
        /// ��ʾѡ�񴰿�
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="conditionFields">������</param>
        /// <param name="conditonValues">����ֵ</param>
        public string SelectVar(SqlConnection conn, string conditionFields, string conditionValues)
        {
            string varString = null;
            SelectForm sForm = new SelectForm(conn, conditionFields, conditionValues);
            DialogResult result = sForm.ShowDialog();
            if(result == DialogResult.OK)
            {
                varString = sForm.VarString;
            }

            return varString;
        }
    }
}
