using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace CreateAICustomFunction
{
    public class Program
    {
        private CustomFunctionForm cForm; // �Զ��嶯���༭����

        /// <summary>
        /// �Զ��嶯��������
        /// </summary>
        public string FunctionName
        {
            get
            {
                return cForm.FunctionName;
            }
        }

        /// <summary>
        /// �Զ��嶯����ID
        /// </summary>
        public object[] FunctionKey
        {
            get
            {
                return new object[] { cForm.FunctionID };
            }            
        }

        /// <summary>
        /// �����Զ��嶯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public bool CreateFunction(SqlConnection conn)
        {
            bool editSuccess = false;

            cForm = new CustomFunctionForm(conn);
            DialogResult result = cForm.ShowDialog();
            if (result == DialogResult.OK)
            {
                editSuccess = true;
            }

            return editSuccess;
        }

    }
}
