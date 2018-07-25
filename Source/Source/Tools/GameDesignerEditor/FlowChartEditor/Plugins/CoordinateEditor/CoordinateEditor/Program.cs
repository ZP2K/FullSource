using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace CoordinateEditor
{
    public class Program
    {
        CoordinateForm cForm;

        //public static void Main()
        //{
        //    CoordinateForm cForm = new CoordinateForm("1,2,3");
        //    cForm.ShowDialog();
        //}

        /// <summary>
        /// ���캯��
        /// </summary>
        public Program()
        {

        }

        /// <summary>
        /// ѡ�������
        /// </summary>
        public string Cordinate
        {
            get
            {
                return cForm.Cordinate;
            }
        }

        /// <summary>
        /// �༭���
        /// </summary>
        /// <param name="cordinate">��ǰ����</param>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public int Show(string cordinate)
        {
            int success = 0;
            cForm = new CoordinateForm(cordinate);
            DialogResult result = cForm.ShowDialog();
            if(result == DialogResult.OK)
            {
                success = 1;
            }

            return success;
        }
    }
}
