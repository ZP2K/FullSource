using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace NotesManager
{
    public class doit
    {
        private frmMain mainFm;

#if false
        public int show(SqlConnection conn, string keyword)
        {
            frmMain fm = new frmMain(conn,keyword);
            fm.Activate();
            fm.ShowDialog();
            return 1;
        }
#endif


        /// <summary>
        /// ��ʼ���ؼ�
        /// </summary>
        /// <param name="conn">��������ݿ�����</param>
        /// <param name="keyword">Ψһ�Ĺؼ��֣����ñ���+����+����1��ֵ+����+...����tablename,2,4</param>
        /// <returns>����һ��Control�����ã���ʵҲ���Ƿ�����һ��tabControl</returns>
        public Control Init(SqlConnection conn)
        {
            mainFm = new frmMain(conn);
            return mainFm.tabControl1;
        }

        /// <summary>
        /// �Ƿ��û��ı�
        /// </summary>
        /// <returns>����һ���Ƿ��޸ĵı�־</returns>
        public Boolean hasChanged()
        {
            return mainFm.hasChanged();
        }


        /// <summary>
        /// ��������ҳ
        /// </summary>
        public void saveAll()
        {
            mainFm.saveAll();
        }

        /// <summary>
        /// ˢ������ҳ��
        /// </summary>
        public void reloadAll()
        {
            mainFm.reloadAll();
        }

        /// <summary>
        /// �����¹ؼ�����������
        /// </summary>
        /// <param name="keyword">�¹ؼ���</param>
        public void reloadAll(string keyword)
        {
            mainFm.reloadAll(keyword);
        }
    }

}
