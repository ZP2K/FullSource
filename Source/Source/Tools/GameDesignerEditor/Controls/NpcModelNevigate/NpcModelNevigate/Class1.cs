using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace NpcModelNevigate
{
    public class NpcModelNev
    {
        private string filePath; // �ļ�·��
        private string modelName; // npcģ������

        public NpcModelNev()
        {

        }

        /// <summary>
        /// �ļ�·������
        /// </summary>
        public string FilePath
        {
            get
            {
                return filePath;
            }
        }

        /// <summary>
        /// npcģ������
        /// </summary>
        public string ModelName
        {
            get
            {
                return modelName;
            }
        }

        public int Show(SqlConnection Conn, object oldValue, string tblname, string catfieldname, string mapname)
        {
            Form1 frm = new Form1(Conn, tblname, catfieldname, mapname);
            DialogResult result = frm.ShowDialog();
            if (result == DialogResult.OK)
            {
                filePath = frm.FilePath;
                modelName = frm.NpcName;
                return frm.ret;
            }
            else // cancel
            {
                return Convert.ToInt32(oldValue);
            }
        }

    }
}
