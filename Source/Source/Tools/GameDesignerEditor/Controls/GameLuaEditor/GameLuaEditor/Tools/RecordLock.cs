using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Data;
using System.Windows.Forms;

namespace GameLuaEditor
{
    /*
     *���ӣ�
            RecordLock l = new RecordLock(m_conn, MainTableName, DBPrimaKeys, KeyValues);
            l.Lock();
            l.UnLock();
            l.Clear();
     */
    public class RecordLock
    {
        protected RecordLock(SqlConnection conn, string strTableName, object[] DBPrimaryKeys, int nModID)
        {
            m_conn = conn;
            m_strTableName = strTableName;

            if (m_conn == null || m_strTableName == null || m_strTableName.Length == 0)
                throw new Exception("�޷����������� RecordLock �Ƿ�����ȷ�Ĳ�����");

            m_strIP = GetIPAddress();
            m_strHostName = GetHostName();

            foreach (string strKey in DBPrimaryKeys)
            {
                m_strKeys += strKey + ",";
            }
            m_nModID = nModID;
        }

        public void Clear()
        {
            m_conn = null;
            if (m_adp != null)
                m_adp.Dispose();
            if (m_ds != null)
                m_ds.Dispose();
            if (m_cmdBuilder != null)
                m_cmdBuilder.Dispose();
        }

        private bool InitDBParams(object[] KeyValues)
        {
            if (m_Values == null)
            {
                m_Values = new object[3];
                //}
                //{
                string strsql = string.Format("select * from sys_lock_record where tablename='{0}' and fieldnames='{1}'", m_strTableName, m_strKeys);
                m_adp = new SqlDataAdapter(strsql, m_conn);
                m_adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                m_ds = new DataSet();
                m_cmdBuilder = new SqlCommandBuilder(m_adp);
            }
            int nCount = 1;
            m_Values[0] = m_strTableName;
            m_Values[1] = m_strKeys;
            m_strValues = "";
            foreach (object obj in KeyValues)
            {
                m_strValues += obj.ToString() + ",";
                nCount++;
            }
            m_Values[2] = m_strValues;
            return true;
        }

        virtual public bool Lock(object[] KeyValues)
        {
            if (KeyValues == null)
                return false;

            InitDBParams(KeyValues);

            DataTable tbl = GetTable();
            bool bRet = false;
            try
            {
                DataRow row = GetLockRow(tbl);

                int nModId = row["modid"] == DBNull.Value ? -1 : Convert.ToInt32(row["modid"]);
                if (nModId != m_nModID && nModId != -1) //����ͬģ��������
                    return false;

                string strlockip = row["lockip"] == DBNull.Value ? "" : row["lockip"].ToString();
                if (strlockip == m_strIP || strlockip.Trim().Length == 0) //����Ǳ�����������û��������
                {
                    bRet = UpdateRow(tbl, row, Convert.ToInt32(row["lastversion"]), DateTime.Now, DateTime.MinValue, m_nModID, true);
                    m_bLocked = bRet;
                }
                else
                    return false;
            }
            finally
            {
                tbl.Dispose();
            }
            return bRet;
        }

        //���ǲ��Ǳ����ڱ�ģ���Ѿ�������,��Ҫ����ģ����������,����Ǳ�����ģ���Ѿ���������,���ټ���
        public bool IsMyLock(object[] KeyValues)
        {
            if (KeyValues == null)
                return false;

            InitDBParams(KeyValues);

            DataTable tbl = GetTable();
            //bool bRet = false;
            try
            {
                DataRow row = GetLockRow(tbl);

                int nModId = row["modid"] == DBNull.Value ? -1 : Convert.ToInt32(row["modid"]);
                if (nModId != m_nModID && nModId != -1) //����ͬģ��������
                    return false;

                string strlockip = row["lockip"] == DBNull.Value ? "" : row["lockip"].ToString();
                if (strlockip == m_strIP)//����Ǳ�������
                    return true;
                else
                    return false;
            }
            finally
            {
                tbl.Dispose();
            }
            //return bRet;
        }

        //return <= 0 ��ʾʧ��
        virtual public int UnLock(object[] KeyValues) //todo:��ģ�鼶����¼����TABҳ�Լ������ļ�¼����Ҫÿ��ȥ�����ݿ⣬̫����
        {
            if (KeyValues == null)
                return -1;

            InitDBParams(KeyValues);

            DataTable tbl = GetTable();
            int nRet = -1;
            try
            {
                DataRow row = GetLockRow(tbl);
                string strlockip = row["lockip"] == null ? "" : row["lockip"].ToString();
                //strlockipΪ�գ���֮ǰ�Ѿ�������ˣ������ٽ�ԭ�����£�
                //LOCKIPΪ�գ��ᵼ���ظ�����������ӱ�����ָ��ͬһ��¼ʱ���������Ⱥ������ᵼ�°汾��һ�£�����������������Ѿ����¡���BUG
                int nOldVer = Convert.ToInt32(row["lastversion"]);
                if (strlockip.Trim().Length == 0)
                {
                    nRet = nOldVer;
                    return nRet;
                }

                if (strlockip == m_strIP)// || strlockip.Trim().Length == 0) 
                {
                    nRet = UpdateRow(tbl, row, nOldVer + 1, DateTime.MinValue, DateTime.Now, -1, false) ? nOldVer + 1 : -1;
                    //m_bLocked = false;
                }
                else
                    return -1;
            }
            finally
            {
                m_bLocked = nRet > 0 ? false : m_bLocked;
                tbl.Dispose();
            }
            return nRet;
        }
        //��ʾ����κ��˼ӵ���������ֻ�ڹ���Ա�ʺ���ʹ�á�
        public bool ForceClearLock(object[] KeyValues)
        {
            InitDBParams(KeyValues);
            DataTable tbl = GetTable();
            bool bRet = false;
            try
            {
                DataRow row = GetLockRow(tbl);
                bRet = UpdateRow(tbl, row, Convert.ToInt32(row["lastversion"]), DateTime.MinValue, DateTime.MinValue, -1, false);
            }
            finally
            {
                tbl.Dispose();
            }
            return bRet;
        }

        //������ָʾ���Ƿ��ɱ�ʵ��ӹ��������ܴ���������ϵı���¼�Ƿ�ӹ���
        public bool Locked
        {
            get { return m_bLocked; }
        }

        private object GetFieldData(object[] KeyValues, string fieldname)
        {
            InitDBParams(KeyValues);
            DataTable tbl = GetTable();
            try
            {
                //DataRow row = GetLockRow(tbl);
                DataRow row = tbl.Rows.Find(m_Values);
                if (row == null)
                {
                    return null;
                }
                else
                {
                    return row[fieldname];
                }
            }
            finally
            {
                tbl.Dispose();
            }
            //return null;
        }


        //����0��ʾû��ȡ���汾��
        public int GetServerLastVersion(object[] KeyValues)
        {
            object o = GetFieldData(KeyValues, "lastversion");
            if (o == null)
                return 0;
            else
                return Convert.ToInt32(o);
        }

        public string GetServerLastLockUser(object[] KeyValues)
        {
            object o = GetFieldData(KeyValues, "lockusername");
            if (o == null)
                m_strLastLockUser = "";
            else
                m_strLastLockUser = o.ToString();
            return m_strLastLockUser;
        }
        public int GetServerLastLockModID(object[] KeyValues)
        {
            object o = GetFieldData(KeyValues, "modid");
            if (o == null || o == DBNull.Value)
                return -1;
            else
                return Convert.ToInt32(o);
        }
        //Private:

        private DataTable GetTable()
        {
            m_adp.Fill(m_ds);
            return m_ds.Tables[0];
        }
        private DataRow GetLockRow(DataTable tbl)
        {
            DataRow row = tbl.Rows.Find(m_Values);
            if (row == null)
            {
                try
                {
                    DataRow newrow = tbl.NewRow();
                    newrow.BeginEdit();

                    newrow["tablename"] = m_strTableName;
                    newrow["fieldnames"] = m_strKeys;
                    newrow["fieldvalues"] = m_strValues;
                    newrow["lastversion"] = 0;
                    newrow["modid"] = DBNull.Value;

                    newrow["lockip"] = DBNull.Value;
                    newrow["lockhostname"] = DBNull.Value;
                    newrow["lockusername"] = DBNull.Value;

                    newrow["locktime"] = DBNull.Value;// DateTime.MinValue;
                    newrow["unlocktime"] = DBNull.Value;//DateTime.MinValue;

                    newrow.EndEdit();
                    tbl.Rows.Add(newrow);

                    SqlCommandBuilder cmdBuilder = m_cmdBuilder;// new SqlCommandBuilder(m_adp);
                    m_adp.InsertCommand = cmdBuilder.GetInsertCommand();
                    int val = m_adp.Update(tbl);
                    tbl.AcceptChanges();
                    row = newrow;
                }
                catch (Exception ex)
                {
                    tbl.RejectChanges();
                    throw ex;
                }
            }
            return row;
        }

        private bool UpdateRow(DataTable tbl, DataRow row, int nLastVer, DateTime dtLock, DateTime dtUnLock, int nModId, bool bLock)
        {
            int val = 0;

            try
            {
                row.BeginEdit();

                row["tablename"] = m_strTableName;
                row["fieldnames"] = m_strKeys;
                row["fieldvalues"] = m_strValues;
                row["lastversion"] = nLastVer;

                if (bLock)
                {
                    row["lockip"] = m_strIP;
                    row["lockhostname"] = m_strHostName;
                    row["lockusername"] = m_strHostName;

                    if (dtLock == DateTime.MinValue)
                        row["locktime"] = DBNull.Value;
                    else
                        row["locktime"] = dtLock;

                    if (dtUnLock == DateTime.MinValue)
                        row["unlocktime"] = DBNull.Value;
                    else
                        row["unlocktime"] = dtUnLock;

                    if (nModId == -1)
                        row["modid"] = DBNull.Value;
                    else
                        row["modid"] = nModId;
                }
                else
                {
                    row["lockip"] = DBNull.Value;
                    row["lockhostname"] = DBNull.Value;
                    row["lockusername"] = DBNull.Value;

                    row["locktime"] = DBNull.Value;
                    if (dtUnLock == DateTime.MinValue)
                        row["unlocktime"] = DBNull.Value;
                    else
                        row["unlocktime"] = dtUnLock;

                    row["modid"] = DBNull.Value;
                }

                row.EndEdit();

                SqlCommandBuilder cmdBuilder = m_cmdBuilder;// new SqlCommandBuilder(m_adp);
                m_adp.UpdateCommand = cmdBuilder.GetUpdateCommand();
                val = m_adp.Update(tbl);

                tbl.AcceptChanges();
                return val > 0;
            }
            catch (Exception ex)
            {
                if (val == 0)
                {
                    string[] keynames = new string[3];
                    keynames[0] = "tablename";
                    keynames[1] = "fieldnames";
                    keynames[2] = "fieldvalues";
                    string[] keys = new string[3];
                    keys[0] = m_strTableName;
                    keys[1] = m_strKeys;
                    keys[2] = m_strValues;
                    string[] fields = new string[7];
                    fields[0] = "lastversion";
                    fields[1] = "lockip";
                    fields[2] = "lockhostname";
                    fields[3] = "lockusername";
                    fields[4] = "locktime";
                    fields[5] = "unlocktime";
                    fields[6] = "modid";
                    string[] values = new string[7];
                    values[0] = nLastVer.ToString();
                    values[1] = m_strIP;
                    values[2] = m_strHostName;
                    values[3] = m_strHostName;
                    values[4] = row["locktime"] == DBNull.Value ? "<NULL>" : row["locktime"].ToString().Trim();
                    values[5] = row["unlocktime"] == DBNull.Value ? "<NULL>" : row["unlocktime"].ToString().Trim();
                    values[6] = row["modid"].ToString().Trim();
                    return SqlUpdateRecord("sys_lock_record", keynames, keys, fields, values, m_conn);
                }
                else
                {
                    tbl.RejectChanges();
                    throw ex;
                }
            }
        }

        private string GetHostName()
        {
            return System.Net.Dns.GetHostName();
        }

        private string GetIPAddress()
        {
            string ip = System.Net.Dns.GetHostEntry(System.Net.Dns.GetHostName()).AddressList[0].ToString();
            return ip;
        }

        private bool SqlUpdateRecord(string tblname, string[] keynames, string[] keys, string[] fields, string[] values, SqlConnection Conn)
        {
            bool bSuc = true;
            Conn.Open();
            try
            {
                SqlCommand cmd = Conn.CreateCommand();
                string str_key_rest = " WHERE ";
                for (int i = 0; i < keynames.Length; i++)
                {
                    str_key_rest += string.Format("{0}='{1}'", keynames[i], keys[i]);
                    if (i < keynames.Length - 1)
                    {
                        str_key_rest += " AND ";
                    }
                }

                for (int i = 0; i < fields.Length; i++)
                {
                    cmd.CommandText = string.Format("UPDATE {0} SET {1} = {2} {3}", tblname, fields[i], (values[i] == "<NULL>" ? "NULL" : "'" + values[i] + "'"), str_key_rest);
                    cmd.ExecuteNonQuery();
                }
            }
            catch (Exception ex)
            {
                bSuc = false;
                string err = string.Format("�ڸ��±� {0} ʱ��������\r\n\r\n������Ϣ��{1}", tblname, ex.Message);
                MessageBox.Show(err);
            }
            Conn.Close();
            return bSuc;
        }

        private SqlConnection m_conn;
        private DataSet m_ds;
        protected string m_strTableName, m_strIP, m_strHostName;
        SqlCommandBuilder m_cmdBuilder;
        private SqlDataAdapter m_adp;
        protected string m_strKeys, m_strValues;
        private object[] m_Values;
        private bool m_bLocked;
        private string m_strLastLockUser;
        protected int m_nModID;//���������ڵ� modid
    };


    class CLocalVersion
    {
        private int m_nVersion = -1;
        public int Version
        {
            get { return m_nVersion; }
            set { m_nVersion = value; }
        }
    };

    public class RecordLockEx : RecordLock
    {
        private static Hashtable m_htModLocks = new Hashtable(); //���ڹ���ͬģ���еİ汾�ţ��Խ��ͬһ��ͬһ��¼�ڶ����ͬ�ط�����������
        //private int m_nLocalVersion = 0;
        private CLocalVersion m_verobj = null;

        public int LocalVersion
        {
            get
            {
                if (m_verobj == null)
                    return -1;
                else
                    return m_verobj.Version;
            }
        }

        public RecordLockEx(SqlConnection conn, string strTableName, object[] DBPrimaryKeys, int nModID)
            : base(conn, strTableName, DBPrimaryKeys, nModID)
        {
        }

        public void UpdateLocalVersion(object[] KeyValues)
        {
            m_verobj = GetVersionObject(KeyValues);
            m_verobj.Version = GetServerLastVersion(KeyValues);
        }

        private CLocalVersion GetVersionObject(object[] KeyValues)
        {
            //��������ȡ�汾�Ŷ���
            Hashtable htMod = null;
            if (!m_htModLocks.ContainsKey(m_nModID))
            {
                htMod = new Hashtable();
                m_htModLocks[m_nModID] = new Hashtable();
            }
            htMod = m_htModLocks[m_nModID] as Hashtable;

            CLocalVersion verobj = null;
            string strkey = GetKey(KeyValues);
            if (!htMod.ContainsKey(strkey))
            {
                htMod.Add(strkey, new CLocalVersion());
            }
            verobj = htMod[strkey] as CLocalVersion;
            return verobj;
        }

        public override bool Lock(object[] KeyValues)
        {
            m_verobj = GetVersionObject(KeyValues);
            return base.Lock(KeyValues);
        }

        private string TransArrayToString(object[] array)
        {
            StringBuilder strbuf = new StringBuilder();
            foreach (object obj in array)
            {
                strbuf.Append(obj.ToString());
                strbuf.Append(",");
            }
            return strbuf.ToString();
        }
        private string GetKey(object[] KeyValues)
        {
            return string.Format("{0}#{1}#{2}", m_strTableName, m_strKeys, TransArrayToString(KeyValues));
        }
        public override int UnLock(object[] KeyValues)
        {
            int nVer = base.UnLock(KeyValues);
            m_verobj.Version = nVer;
            return nVer;
        }
    };
}
