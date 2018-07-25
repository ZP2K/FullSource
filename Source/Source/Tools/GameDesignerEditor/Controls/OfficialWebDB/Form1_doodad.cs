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
        private bool Fill_Doodad()
        {
            // ����ϱ�����
            Helper.ClearDbTable("web_doodad", Conn);

            // ����
            DataRow defaultrow = TblDoodadTemplate.Rows[0];
            foreach (DataRow row in TblDoodadTemplate.Rows)
            {
                // id
                int id = Convert.ToInt32(row["ID"]);

                // ����
                string name = row["Name"].ToString().Trim();

                // ���ڳ���
                string mapname = row["MapName"].ToString().Trim();

                if (mapname != "�Ķ�")
                    continue;

                // ����ȼ�
                int requireLevel = 0;

                // ����һ��quest������
                string sql = string.Format("insert into web_doodad values({0}, '{1}', '{2}', {3})",
                    id, name, mapname, requireLevel);
                Helper.ExecSqlCmd(sql, Conn);

            }
            return true;
        }
    }
}
