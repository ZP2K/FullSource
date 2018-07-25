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
        private bool Fill_Trinket()
        {
            // ����ϱ�����
            Helper.ClearDbTable("web_trinket", Conn);

            // ����
            DataRow defaultrow = TblTrinket.Rows[0];
            foreach (DataRow row in TblTrinket.Rows)
            {
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
                int level = Convert.ToInt32(row["Level"] == DBNull.Value ? defaultrow["Level"] : row["Level"]);

                // ����ȼ�
                int requirelevel = Convert.ToInt32(row["Require1Value"] == DBNull.Value ? defaultrow["Require1Value"] : row["Require1Value"]);

                // ������
                int b = Convert.ToInt32(row["BindType"] == DBNull.Value ? defaultrow["BindType"] : row["BindType"]);
                string bindtype = GetBindType(b);

                // ����
                int st = Convert.ToInt32(row["SubType"] == DBNull.Value ? defaultrow["SubType"] : row["SubType"]);
                string detailtype = GetTrinketType(st);

                // ��Ʒ��Դ
                //string source = row["GetType"].ToString().Trim();
                string magicKind = row["MagicKind"].ToString().Trim();
                string magicType = row["GetType"].ToString().Trim();
                string belongSchool = row["BelongSchool"].ToString().Trim();
                string getType = row["GetType"].ToString().Trim();
                string belongMap = row["BelongMap"].ToString().Trim();

                // ����۸�
                string price = IsLogicallyNull(row["Price"]) ? "0" : MoneyToString(Convert.ToInt32(row["Price"]));

                // ����;ö�
                int maxDurability = Convert.ToInt32(row["MaxDurability"] == DBNull.Value ? defaultrow["MaxDurability"] : row["MaxDurability"]);

                // ��Ʒ����
                string attrib = GetEquipDesc(row);

                // ����һ��quest������
                string sql = string.Format("insert into web_trinket values('{0}', {1}, '{2}', '{3}', {4}, {5}, '{6}', '{7}', '{8}', '{9}', '{10}', '{11}', '{12}', '{13}', {14}, '{15}')",
                    icon, id, name, quality, level, requirelevel, bindtype, detailtype, magicKind, magicType, belongSchool, getType, belongMap, price, maxDurability, attrib);
                Helper.ExecSqlCmd(sql, Conn);
            }


            return true;
        }

        private string GetTrinketType(int subtype)
        {
            string[] subtypes = {
                "��������", "Զ������", "����", "ͷ��",
                "����", "��ָ", "����", "��׺",
                "����", "Ь��", "����", "������չ",
                "����", "������ҩ", "������չ", "����",
                "�½�"
            };
            return subtypes[subtype];
        }
    }
}
