using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace biscuit
{
    public class Class1
    {
        private MainForm mainform;

        /// <summary>
        /// ��ʼ���ؼ�
        /// </summary>
        /// <param name="conn">��������ݿ�����</param>
        /// <param name="keyword">Ψһ�Ĺؼ��֣����ñ���+����+����1��ֵ+����+...����tablename,2,4</param>
        /// <returns>����һ��Control�����ã���ʵҲ���Ƿ�����һ��tabControl</returns>
        public Control Init(SqlConnection conn, string rootDir, string catname)
        {
            mainform = new MainForm(conn, rootDir, catname);
            return mainform.OutterCtrl;
        }

        /// <summary>
        /// �Ƿ��û��ı�
        /// </summary>
        /// <returns>����һ���Ƿ��޸ĵı�־</returns>
        public Boolean hasChanged()
        {
            return mainform.hasChanged();
        }


        /// <summary>
        /// ��������ҳ
        /// </summary>
        public void saveAll()
        {
            mainform.saveAll();
        }

        /// <summary>
        /// ˢ������ҳ��
        /// </summary>
        public void reloadAll()
        {
            mainform.reloadAll();
        }

        /// <summary>
        /// �����¹ؼ�����������
        /// </summary>
        /// <param name="keyword">�¹ؼ���</param>
        public void reloadAll(string keyword)
        {
            mainform.reloadAll(keyword);
        }
    }
}
