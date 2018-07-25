using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Drawing;

using sprViewer;

namespace IconSelector
{

    public class Implement
    {
        private FormSelector m_formselector;
        private int m_id;
        private bool m_bEdit = false;
        private bool inited = false; // �Ƿ��Ѿ���ʼ����
        private SqlConnection connection; // sql����
        private string iconDirectory; // ͼ������Ŀ¼
        private Helper helper = Helper.GetHelper();

        public int IconID
        {
            get { return m_id; }
            set { m_id = value; }
        }

        /// <summary>
        /// �Ƿ��Ѿ���ʼ����
        /// </summary>
        public bool Inited
        {
            get
            {
                return inited;
            }
        }

        public string IconDirectory
        {
            set
            {
                iconDirectory = value;
            }
        }

        public SqlConnection Connection
        {
            set
            {
                connection = value;
            }
        }

        public bool Init()
        {
            return Init(connection, iconDirectory);
        }

        public void EnterEditMode()
        {
            m_bEdit = true;
        }

        public bool Init(SqlConnection Conn, string strIconDir)
        {
            // ��ʼ�����ݿ����Ӽ����ݱ�
            helper.CurrentSqlConnection = Conn;
            helper.InitCacheTable();

            tgaIO.sqlConn = Conn;
            m_id = -1;
            tgaIO.StrIconFolder = strIconDir;
            m_formselector = new FormSelector();
            m_formselector.Tag = this;
            if (m_bEdit)
                m_formselector.EnterEditMode();
            FormScan form = new FormScan();
            object[] objs = form.Init(Conn, strIconDir, m_formselector.imageIcons);
            m_formselector.IconsInfo = objs;
            inited = true;

            if (objs.Length > 0)
            {
                form.ShowDialog(); //��ʾͼ�񲢴���ImageList
            }
            else
            {
                return false;
            }

            return true;
        }

        public int Show()
        {
            m_formselector.SelectedID = m_id;
            DialogResult r = m_formselector.ShowDialog();
            int nRet = m_formselector.SelectedID;
            if (r == DialogResult.OK)
                return nRet;
            else
                return -1;
        }
    }
   

    public static class tgaIO
    {
        public static string tga2rtf(int id, System.Data.SqlClient.SqlConnection conn, string strIconDir)
        {
            FormScan form = new FormScan();
            string filename = "";
            int frame = 0;

            //��ѯ���ݿ����ļ���
            DataTable tbl = GetDataTable("select * from icon where id=" + id.ToString(), conn);
            if (tbl.Rows.Count == 0)
            {
                return ("");
            }
            DataRow row = tbl.Rows[0];
            filename = strIconDir + "\\" + row["filename"].ToString();
            frame = Convert.ToInt32(row["frame"]);
            if (!System.IO.File.Exists(filename))
            {
                return ("");
            }

            //�����ļ�������ͼ��
            Image timg = sprViewer.tga.uitex2img(strIconDir, filename, frame);
            if(timg == null)
            {
                return("");
            }

            //���ݼ��а�
            //Object obj = new Object();
            //obj = Clipboard.GetDataObject();

            //�ָ����а�
            //Clipboard.SetDataObject(obj);


            //ת��ͼ��
            Clipboard.SetImage(timg);
            form.txt.Clear();
            form.txt.Paste();

            //���ؽ��
            form.txt.SelectAll();
            return form.txt.SelectedRtf;
        }


        public static SqlConnection sqlConn = null;
        public static string StrIconFolder = "";
        public static DataTable GetDataTable(string sql, SqlConnection conn)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            DataTable tbl = ds.Tables[0];
            return tbl;
        }
        
        //�����޸�ĳ��ͼ�������ݿ��еķ���
        public static void ResetAllKind(string id, string newKind, string newSubKind)
        {
            if ( sqlConn == null ) return;
            sqlConn.Open();

            string sql = string.Format("UPDATE icon SET Kind = '{0}', SubKind = '{1}' WHERE ID = {2}",
                newKind, newSubKind, id);
            SqlCommand sqlCmd = new SqlCommand(sql, sqlConn);
            sqlCmd.ExecuteNonQuery();
            sqlConn.Close();
        }
        public static void ResetKind(string id, string newKind)
        {
            if ( sqlConn == null ) return;
            sqlConn.Open();

            string sql = string.Format("UPDATE icon SET Kind = '{0}' WHERE ID = {1}", newKind, id);
            SqlCommand sqlCmd = new SqlCommand(sql, sqlConn);
            sqlCmd.ExecuteNonQuery();
            sqlConn.Close();
        }
        public static void ResetSubKind(string id, string newSubKind)
        {
            if ( sqlConn == null ) return;
            sqlConn.Open();

            string sql = string.Format("UPDATE icon SET SubKind = '{0}' WHERE ID = {1}", newSubKind, id);
            SqlCommand sqlCmd = new SqlCommand(sql, sqlConn);
            sqlCmd.ExecuteNonQuery();
            sqlConn.Close();
        }

        //���ڷ���������
        public static void RnameType(string oldType, string newType)
        {
            if ( sqlConn == null ) return;
            sqlConn.Open();
            SqlCommand sqlCmd = new SqlCommand("update icon set kind='" + newType + "' where Kind='" + oldType + "'", sqlConn);
            sqlCmd.ExecuteNonQuery();
            sqlConn.Close();
        }

        //���ڸ���tag1��tag2
        public static void UpdateTag12(int[] ids, string fieldname, string strvalue)
        {
            if (sqlConn == null) return;
            sqlConn.Open();

            string sqlNewValue = strvalue.Length == 0 ? "NULL" : string.Format("'{0}'", strvalue);
            string sql = string.Format("update icon set {0}={1} where ", fieldname, sqlNewValue);
            foreach (int oneid in ids)
            {
                sql += "id=" + oneid.ToString() + " or ";
            }
            sql += "0=1";
            SqlCommand sqlCmd = new SqlCommand(sql, sqlConn);
            sqlCmd.ExecuteNonQuery();
            sqlConn.Close();
        }

        //����¼�¼,����ID
        public static int InsertNewRecord(string FileName, string tag1, string tag2)
        {
            return InsertNewRecord(FileName, tag1, tag2, sqlConn);
        }

        // ����¼�¼,����ID
        public static int InsertNewRecord(string FileName, string tag1, string tag2, SqlConnection Conn)
        {
            if (Conn == null) return 0;

            string sql = string.Format("SELECT FileName FROM icon WHERE FileName = '{0}'", FileName);
            DataTable tbl = GetDataTable(sql, Conn);
            if (tbl.Rows.Count > 0) return 0;

            Conn.Open();
            string strSQL = @"
begin transaction;
declare @newid int;
set @newid = (select max(id) from icon);
insert into icon(id, filename, frame, kind, subkind, tag1, tag2) values (
    @newid+1,'{0}','0','����Դ','','{1}','{2}'
);
select max(id) from icon;
commit transaction;
";

            SqlCommand sqlCmd = new SqlCommand(string.Format(strSQL, FileName, tag1, tag2), Conn);
            int nRet = Convert.ToInt32(sqlCmd.ExecuteScalar().ToString());
            Conn.Close();
            return nRet;
        }
   };
}
