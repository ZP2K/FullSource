using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace OfficialWebDB
{
    public partial class Form1
    {
        private bool Fill_Other()
        {
            // ����ϱ�����
            Helper.ClearDbTable("web_other", Conn);

            // ����
            DataRow defaultrow = TblOther.Rows[0];
            foreach (DataRow row in TblOther.Rows)
            {
                // ����
                string detailtype = row["_CATEGORY"].ToString().Trim();
                if (!CategoryRequired(detailtype))
                    continue;

                // id
                int id = Convert.ToInt32(row["ID"]);

                // ͼ��
                int uiid = Convert.ToInt32(row["UiID"] == DBNull.Value ? defaultrow["UiID"] : row["UiID"]);
                string icon = GetItemIconName(uiid);

                // ����
                string name = row["Name"].ToString().Trim();

                // Ʒ��
                int q = Convert.ToInt32(row["Quality"] == DBNull.Value ? defaultrow["Quality"] : row["Quality"]);
                string quality = GetQuality(q);

                // �ȼ�
                int level = Convert.ToInt32(row["UiID"] == DBNull.Value ? defaultrow["UiID"] : row["UiID"]);

                // ����ȼ�
                int requirelevel = row["RequireLevel"] != DBNull.Value ? Convert.ToInt32(row["RequireLevel"]) : 0;

                // ������
                int b = Convert.ToInt32(row["BindType"] == DBNull.Value ? defaultrow["BindType"] : row["BindType"]);
                string bindtype = GetBindType(b);

                // ������Դ
                string source = string.Empty;

                // ����۸�
                string price = row["Price"].ToString().Trim();

                // ����;ö�
                int maxDurability = Convert.ToInt32(row["MaxDurability"] == DBNull.Value ? defaultrow["MaxDurability"] : row["MaxDurability"]);

                // ����һ��quest������
                string sql = string.Format("insert into web_other values('{0}', {1}, '{2}', '{3}', {4}, {5}, '{6}', '{7}', '{8}', {9}, {10})",
                    icon, id, name, quality, level, requirelevel, bindtype, detailtype, source, price, maxDurability);
                Helper.ExecSqlCmd(sql, Conn);
            }


            return true;
        }

        public bool CategoryRequired(string cat)
        {
            if (cat == "���Ҳ���" || cat == "�����䷽" || cat == "���Ҳ���" ||
                cat == "��⿲���" || cat == "����䷽" || cat == "��⿲���" ||
                cat == "ҽ������" || cat == "ҽ���䷽" || cat == "ҽ������" ||
                cat == "�������" || cat == "�����䷽" || cat == "�������")
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
