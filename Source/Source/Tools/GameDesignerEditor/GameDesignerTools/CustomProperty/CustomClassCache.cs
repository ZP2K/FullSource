using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Collections;
using System.Data.SqlClient;
using System.Windows.Forms;
using DataTableRemotingClient;


namespace GameDesingerTools
{
    public static class CustomClassCache
    {
        private static SqlConnection m_conn = MainForm.conn;
        
        private static Hashtable m_htDefVal = new Hashtable();
        private static Hashtable m_htDbTable = new Hashtable();
        private static Hashtable m_htDbMeta = new Hashtable();
        private static Hashtable m_htComboCaches = new Hashtable();
        private static Hashtable m_htModTabDef = new Hashtable();
        private static Hashtable m_htScriptStore = new Hashtable();
        private static Hashtable m_hasDefaultRow; // ���ݱ��Ƿ���Ĭ���л��棬��������
        private static Hashtable m_primaryKey = new Hashtable(); // ���ݱ���������棬��������
        private static Hashtable m_checkSum = new Hashtable(); // ���ݱ�У��ֵ���棬��������
        private static Hashtable m_sqlDataAdapter = new Hashtable(); // sqlDataAdapter����        

        // �޶�����Ĵ�С������Ƶ���л���¼ʱռ�ô����ڴ�
        private static int defValCacheSize = 100;
        private static int dbTableCacheSize = 100;
        private static int dbMetaCacheSize = 100;
        private static int comboCacheSize = 100;
        private static int modTabDefCacheSize = 100;
        private static int scriptStoreCacheSize = 100;
        private static int sqlDataAdapterCacheSize = 100;

        private static DataTableProxy mDataTableProxy;

        static CustomClassCache()
        {
            if (Program.EnableRemoting)
            {
                mDataTableProxy = DataTableServerProxy.GetDataTableProxy();
            }
        }

        public static SqlConnection Conn 
        {
            get { return m_conn; } 
            set{m_conn = value;}
        }

        public static void DelAllCaches()
        {
            m_htDefVal = new Hashtable();
            m_htDbTable = new Hashtable();
            m_htDbMeta = new Hashtable();
            m_htComboCaches = new Hashtable();
            m_htModTabDef = new Hashtable();
            m_htScriptStore = new Hashtable();
        }

        public static void DelComboCache()
        {
            m_htComboCaches = new Hashtable();
        }        

        /// <summary>
        /// ��ȡ���ݱ��У��ֵ
        /// </summary>
        /// <param name="tableName">���ݱ���</param>
        /// <return>���ݱ��У��ֵ</return>
        private static int GetTableCheckSum(string tableName)
        {
            int tableCheckSum = 0;

            if (m_conn.State == ConnectionState.Closed)
            {
                m_conn.Open();
            }

            SqlCommand cmd = m_conn.CreateCommand();
            string sqlString = string.Format("SELECT CHECKSUM_AGG(BINARY_CHECKSUM(*)) FROM {0}", tableName);
            cmd.CommandText = sqlString;
            object executeResult = cmd.ExecuteScalar();

            if (m_conn.State == ConnectionState.Open)
            {
                m_conn.Close();
            }

            if (executeResult != null && executeResult != DBNull.Value)
            {
                tableCheckSum = (int)executeResult;
            }

            return tableCheckSum;
        }

        /// <summary>
        /// ��ȡ���ݱ���
        /// </summary>
        /// <param name="sqlString">sql���</param>
        /// <returns>���ݱ���</returns>
        private static string GetTableName(string sqlString)
        {
            string tableName = null;

            string[] data = sqlString.ToLower().Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

            for (int i = 0; i < data.Length; i++)
            {
                if (data[i] == "from")
                {
                    tableName = data[i + 1];
                    break;
                }
            }

            return tableName;
        }

        /// <summary>
        /// ��鲢��յ�ǰsql�����ص�cache
        /// </summary>
        /// <param name="sqlString">sql���</param>
        /// <param name="cacheTable">cache��ϣ��</param>
        private static void ClearRelevateSqlKey(string sqlString, Hashtable cacheTable)
        {
            string tableName = GetTableName(sqlString);

            if (tableName != null && !tableName.StartsWith("sys_"))
            {
                if (RequireClearCache(tableName)) // ������ݱ��У��ֵ�Ƿ�ı䣬��ı�Ļ���Ҫ��ȡ������Cache
                {
                    List<string> keyList = new List<string>();

                    foreach (string key in cacheTable.Keys)
                    {
                        if (key.ToLower().Contains(string.Format("from {0}", tableName)))
                        {
                            keyList.Add(key);

                        }
                    }

                    foreach (string s in keyList)
                    {
                        cacheTable[s] = null;
                    }
                }
            }
        }

        /// <summary>
        /// �Ƿ���Ҫ����cache
        /// </summary>
        /// <param name="tableName">���ݱ���</param>
        /// <returns>�Ƿ���Ҫ����cache</returns>
        private static bool RequireClearCache(string tableName)
        {
            bool requireClear = false;
           
            string currentCheckSum = GetTableCheckSum(tableName).ToString();
            string cacheCheckSum = m_checkSum[tableName] as string;

            if (cacheCheckSum != currentCheckSum)
            {
                requireClear = true;
                m_checkSum[tableName] = currentCheckSum;
            }
            
            return requireClear;
        }

        /// <summary>
        /// ��ȡ���ݱ������
        /// </summary>
        /// <param name="tableName">���ݱ���</param>
        /// <returns>���ݱ������</returns>
        public static List<string> GetPrimaryKeys(string tableName)
        {            
            List<string> primaryKeyList = m_primaryKey[tableName] as List<string>;

            if (primaryKeyList == null)
            {
                primaryKeyList = new List<string>();
                string sqlString = string.Format("sp_pkeys '{0}'", tableName);

                if (m_conn.State == ConnectionState.Closed)
                {
                    m_conn.Open();
                }

                SqlCommand cmd = m_conn.CreateCommand();
                cmd.CommandText = sqlString;
                SqlDataReader dataReader = cmd.ExecuteReader();

                while (dataReader.Read())
                {
                    primaryKeyList.Add(dataReader["COLUMN_NAME"].ToString());
                }

                dataReader.Close();

                if (m_conn.State == ConnectionState.Open)
                {
                    m_conn.Close();
                }
                
                m_primaryKey[tableName] = primaryKeyList;                                
            }
            
            if (primaryKeyList == null)
            {
                primaryKeyList = new List<string>();
            }

            return primaryKeyList;
        }

        public static DataTable GetScriptStore(string sql)
        {
            if (m_htScriptStore[sql] == null)
            {
                SqlDataAdapter adp = new SqlDataAdapter(sql, Conn);
                DataSet ds = new DataSet();
                adp.Fill(ds);

                if (m_htScriptStore.Keys.Count > scriptStoreCacheSize)
                {
                    m_htScriptStore.Clear();
                }

                m_htScriptStore[sql] = ds.Tables[0];
            }

            return (DataTable)(m_htScriptStore[sql]);
        }

        public static DataTable GetScriptStoreProxy(string[] names, string sql)
        {
            DataTable table = null;

            if (m_htScriptStore[sql] == null && Program.EnableRemoting)
            {
                table = mDataTableProxy.GetDataTableWithSql(names, sql);

                if (m_htScriptStore.Keys.Count > scriptStoreCacheSize)
                {
                    m_htScriptStore.Clear();
                }

                m_htScriptStore[sql] = table;
            }
            else
            {
                table = GetScriptStore(sql);
            }

            return table;
        }

        public static void DelAllScriptStore()
        {
            m_htScriptStore = new Hashtable();
        }

        public static DataTable GetModTabDef(string sql)
        {
            if (m_htModTabDef[sql] == null)
            {
                SqlDataAdapter adp = new SqlDataAdapter(sql, Conn);
                DataSet ds = new DataSet();
                adp.Fill(ds);

                if (m_htModTabDef.Keys.Count > modTabDefCacheSize)
                {
                    m_htModTabDef.Clear();
                }

                m_htModTabDef[sql] = ds.Tables[0];
            }

            return (DataTable)(m_htModTabDef[sql]);
        }

        public static DataTable GetModTabDefProxy(string name, string sql)
        {
            DataTable table = null;
            if (m_htModTabDef[sql] == null && Program.EnableRemoting)
            {
                table = mDataTableProxy.GetDataTableWithSql(name, sql);

                if (m_htModTabDef.Keys.Count > modTabDefCacheSize)
                {
                    m_htModTabDef.Clear();
                }

                m_htModTabDef[sql] = table;
            }
            else
            {
                table = GetModTabDef(sql);
            }
            return table;
        }

        public static Hashtable[] GetComboCaches(string key, DBCustomClass dbClass)
        {
            if (m_htComboCaches[key] == null)
            {
                Hashtable[] htReturn = new Hashtable[3];

                string[] keys = key.Split(new Char[] {','});
                string strlisttable = keys[0];
                string strliscondition = keys[1];
                string strlistf = keys[2];
                string strlistk = keys[3];
                string strlistfex = keys[4];

                string sql = "SELECT * FROM " + strlisttable;
                if (strliscondition.Length > 0)
                    sql += " WHERE " + strliscondition;
                /*DataTable tbl = Helper.GetDataTable(sql, Conn);*/
                DataTable tbl = GetDataTableWithKeyProxy(strlisttable, strliscondition, null);

                Hashtable comhashkeys = new Hashtable();
                Hashtable comhashvalues = new Hashtable();
                Hashtable comhashfieldsex = dbClass.GetListFieldExArray(strlistfex); // ��������Ի�ȡһЩ������ֶΣ��Ա��ڽű����ʴӱ��ֶ�����
                Hashtable modihash = comhashfieldsex == null ? null : (Hashtable)comhashfieldsex.Clone();
                Hashtable comfieldsexContainer = new Hashtable();
                foreach (DataRow comborow in tbl.Rows)
                {
                    string strkey = dbClass.GetFieldStrNoLower(comborow, strlistk);
                    string newStrListF = dbClass.GetFieldStr(comborow, strlistf) + "[" + strkey + "]";

                    comhashkeys[newStrListF] = strkey;
                    comhashvalues[strkey] = newStrListF;
                    if (comhashfieldsex != null)
                    {
                        foreach (string eachkey in comhashfieldsex.Keys)
                        {
                            if (comborow[eachkey] != null)
                            {
                                modihash[eachkey] = comborow[eachkey].ToString();
                            }
                            else
                                modihash[eachkey] = "";
                        }
                        if (comhashfieldsex.Count > 0 && !comfieldsexContainer.ContainsKey(strkey))
                            comfieldsexContainer[strkey] = modihash.Clone(); // ÿһ���γ�һ��HASHTABLE
                    }
                }

                htReturn[0] = comhashkeys;
                htReturn[1] = comhashvalues;
                htReturn[2] = comfieldsexContainer; // ÿһ�м�¼�γ�һ��HASHTABLE,������KEYֵ��listfieldIDֵ

                if (m_htComboCaches.Keys.Count > comboCacheSize)
                {
                    m_htComboCaches.Clear();
                }

                m_htComboCaches[key] = htReturn;
            }

            return (Hashtable[])(m_htComboCaches[key]);
        }
        
        public static DataTable GetDataTableWithKey(string sql)
        {                        
            ClearRelevateSqlKey(sql, m_htDbTable); // �����ص�sql�����Ƿ���Ҫ���¶�ȡ����

            if (m_htDbTable[sql] == null)
            {
                SqlDataAdapter adp = new SqlDataAdapter(sql, Conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);

                if (m_htDbTable.Keys.Count > dbTableCacheSize)
                {
                    m_htDbTable.Clear();
                }

                m_htDbTable[sql] = ds.Tables[0];
            }

            return (DataTable)(m_htDbTable[sql]);
        }

        public static DataTable GetDataTableWithKeyProxy(string tableName)
        {
            DataTable table = null;
            string sql = string.Format("select * from {0}", tableName);

            if (Program.EnableRemoting)
            {
                ClearRelevateSqlKey(sql, m_htDbTable); // �����ص�sql�����Ƿ���Ҫ���¶�ȡ����

                if (m_htDbTable[sql] == null)
                {
                    table = mDataTableProxy.GetDataTable(tableName);

                    if (m_htDbTable.Keys.Count > dbTableCacheSize)
                    {
                        m_htDbTable.Clear();
                    }

                    m_htDbTable[sql] = table;
                }
                else
                {
                    table = m_htDbTable[sql] as DataTable;
                }
            }
            else
            {
                table = GetDataTableWithKey(sql);
            }
            return table;
        }

        public static DataTable GetDataTableWithKeyProxy(string tableName, string filter, string order)
        { 
            string sort = string.Empty;
            if (order != null && order != string.Empty)
            {
                sort = string.Format(" order by {0}", order);
            }
            string where = string.Empty;
            if (filter != null && filter != string.Empty)
            {
                where = string.Format(" where {0}", filter);
            }

            if (sort == string.Empty && where == string.Empty)
            {
                return GetDataTableWithKeyProxy(tableName);
            }

            string sql = string.Format("Select * from {0}", tableName);
            sql += where + sort;

            DataTable table = null;
            if (Program.EnableRemoting)
            {
                ClearRelevateSqlKey(sql, m_htDbTable); // �����ص�sql�����Ƿ���Ҫ���¶�ȡ����

                if (m_htDbTable[sql] == null)
                {
                    table = mDataTableProxy.GetDataTable(tableName, filter, order);

                    if (m_htDbTable.Keys.Count > dbTableCacheSize)
                    {
                        m_htDbTable.Clear();
                    }

                    m_htDbTable[sql] = table;
                }
                else 
                {
                    table = m_htDbTable[sql] as DataTable;
                }
            }
            else
            {
                table = GetDataTableWithKey(sql);
            }

            return table;

        }

        public static DataTable GetDataTableWithKeyTopRowProxy(string tableName)
        {
            string sql = string.Format("select top 1 * from {0}", tableName);
            DataTable table = null;
            if (Program.EnableRemoting)
            {
                ClearRelevateSqlKey(sql, m_htDbTable); // �����ص�sql�����Ƿ���Ҫ���¶�ȡ����

                if (m_htDbTable[sql] == null)
                {
                    table = mDataTableProxy.GetDataTableTopRow(tableName);

                    if (m_htDbTable.Keys.Count > dbTableCacheSize)
                    {
                        m_htDbTable.Clear();
                    }

                    m_htDbTable[sql] = table;
                }
                else
                {
                    table = m_htDbTable[sql] as DataTable;
                }
            }
            else
            {
                table = GetDataTableWithKey(sql);
            }

            return table;
        }

        


        
        public static DataTable GetDataTableWithoutKey(string sql, ref SqlDataAdapter sqlDataAdapter)
        {                        
            DataTable dataTable = m_htDbTable[sql] as DataTable;
            SqlDataAdapter adapter = m_sqlDataAdapter[sql] as SqlDataAdapter;

            ClearRelevateSqlKey(sql, m_htDbTable); // �����ص�sql�����Ƿ���Ҫ���¶�ȡ����

            if (dataTable == null || adapter == null)
            {
                adapter = new SqlDataAdapter(sql, Conn);
                DataSet ds = new DataSet();
                adapter.Fill(ds);
                
                dataTable = ds.Tables[0];
                sqlDataAdapter = adapter; // ����ֱ�ӷ���SqlDataAdapter�������ں������ú��ٴ���һ��  
                
                if (m_htDbTable.Keys.Count > dbTableCacheSize)
                {
                    m_htDbTable.Clear();
                }

                if (m_sqlDataAdapter.Keys.Count > sqlDataAdapterCacheSize)
                {
                    m_sqlDataAdapter.Clear();
                }

                m_htDbTable[sql] = dataTable;
                m_sqlDataAdapter[sql] = adapter;
            }

            sqlDataAdapter = adapter;
            return dataTable;
        }

        public static DataTable GetDataTableWithoutKey(string sql)
        {
            ClearRelevateSqlKey(sql, m_htDbTable); // �����ص�sql�����Ƿ���Ҫ���¶�ȡ����

            if (m_htDbTable[sql] == null)
            {
                SqlDataAdapter adp = new SqlDataAdapter(sql, Conn);
                DataSet ds = new DataSet();
                adp.Fill(ds);

                if (m_htDbTable.Keys.Count > dbTableCacheSize)
                {
                    m_htDbTable.Clear();
                }

                m_htDbTable[sql] = ds.Tables[0];
            }

            return (DataTable)(m_htDbTable[sql]);
        }


        public static DataTable GetDataTableWithoutKeyProxy(string tableName)
        {
            string sql = string.Format("select * from {0}", tableName);

            DataTable table = null;
            if (Program.EnableRemoting)
            {
                ClearRelevateSqlKey(sql, m_htDbTable); // �����ص�sql�����Ƿ���Ҫ���¶�ȡ����

                if (m_htDbTable[sql] == null)
                {
                    table = mDataTableProxy.GetDataTable(tableName);

                    if (m_htDbTable.Keys.Count > dbTableCacheSize)
                    {
                        m_htDbTable.Clear();
                    }

                    m_htDbTable[sql] = table;
                }
                else
                {
                    table = m_htDbTable[sql] as DataTable;
                }
            }
            else
            {
                table = GetDataTableWithoutKey(sql);
            }
            return table;
        }

        public static DataTable GetDataTableWithoutKeyProxy(string tableName, string filter, string order)
        {
            string sort = string.Empty;
            if (order != null && order != string.Empty)
            {
                sort = string.Format(" order by {0}", order);
            }
            string where = string.Empty;
            if (filter != null && filter != string.Empty)
            {
                where = string.Format(" where {0}", filter);
            }

            if (sort == string.Empty && where == string.Empty)
            {
                return GetDataTableWithKeyProxy(tableName);
            }

            string sql = string.Format("Select * from {0}", tableName);
            sql += where + sort;

            DataTable table = null;

            if (Program.EnableRemoting)
            {
                ClearRelevateSqlKey(sql, m_htDbTable);

                if (m_htDbTable[sql] == null)
                {
                    table = mDataTableProxy.GetDataTable(tableName, filter, order);

                    if (m_htDbTable.Keys.Count > dbTableCacheSize)
                    {
                        m_htDbTable.Clear();
                    }

                    m_htDbTable[sql] = table;
                }
                else
                {
                    table = m_htDbTable[sql] as DataTable;
                }
            }
            else
            {
                table = GetDataTableWithoutKey(sql);
            }

            return (DataTable)(m_htDbTable[sql]);

        }

        public static DataTable GetDataTableWithoutKeyProxy(string tableName, ref SqlDataAdapter sqlDataAdapter)
        {
            string sql = string.Format("select * from {0}", tableName);

            DataTable table = null;
            if (Program.EnableRemoting)
            {
                table = GetDataTableWithoutKeyProxy(tableName);
            }
            else
            {
                table = GetDataTableWithoutKey(sql, ref sqlDataAdapter);
            }
            return table;
        }


        public static DataTable GetDataTableWithoutKeyProxy(string tableName, string filter, string order, ref SqlDataAdapter sqlDataAdapter)
        {
            DataTable table = null;
            string sort = string.Empty;
            if (order != null && order != string.Empty)
            {
                sort = string.Format(" order by {0}", order);
            }
            string where = string.Empty;
            if (filter != null && filter != string.Empty)
            {
                where = string.Format(" where {0}", filter);
            }

            if (sort == string.Empty && where == string.Empty)
            {
                table = GetDataTableWithoutKeyProxy(tableName, ref sqlDataAdapter);
            }

            string sql = string.Format("Select * from {0}", tableName);
            sql += where + sort;

            if (Program.EnableRemoting)
            {
                table = GetDataTableWithoutKeyProxy(tableName, filter, order);
            }
            else
            {
                table = GetDataTableWithoutKey(sql, ref sqlDataAdapter);
            }
            return table;

        }

        // �������ص�Cache
        public static void DelDataTableCache(string strsql)
        {
            if (m_htDbTable.Contains(strsql))
            {
                if (m_htDbTable[strsql] == null)
                {
                    m_htDbTable.Remove(strsql);
                }
                else
                {
                    ((DataTable)m_htDbTable[strsql]).Dispose();
                    m_htDbTable.Remove(strsql);
                }
            }
        }     
   
        // �������ص�Cache
        public static void DelAllDataTableCache()
        {
            m_htDbTable = new Hashtable();            
        }

        public static DataTable GetDbMetaInfo(string sql)
        {
            if (m_htDbMeta[sql] == null)
            {
                SqlDataAdapter adp = new SqlDataAdapter(sql, Conn);
                DataSet ds = new DataSet();
                adp.Fill(ds);

                if (m_htDbMeta.Keys.Count > dbMetaCacheSize)
                {
                    m_htDbMeta.Clear();
                }

                m_htDbMeta[sql] = ds.Tables[0];
            }

            return (DataTable)(m_htDbMeta[sql]);
        }

        public static DataTable GetDbMetaInfoProxy(string name, string sql)
        {
            DataTable table = null;
            if (Program.EnableRemoting)
            {
                if (m_htDbMeta[sql] == null)
                {
                    table = mDataTableProxy.GetDataTableWithSql(name, sql);

                    if (m_htDbMeta.Keys.Count > dbMetaCacheSize)
                    {
                        m_htDbMeta.Clear();
                    }

                    m_htDbMeta[sql] = table;
                }
                else
                {
                    table = m_htDbMeta[sql] as DataTable;
                }
            }
            else
            {
                table = GetDbMetaInfo(sql);
            }

            return table;
        }

        public static void DelMetaCache()
        {
            m_htDbMeta = new Hashtable();
        }

        public static DataRow GetPropDefaultValue(string tblname)
        {
            if (m_hasDefaultRow == null) // ֻ��ʼ��һ�μ���
            {
                m_hasDefaultRow = new Hashtable();

                /*DataTable tempTable = Helper.GetDataTable(string.Format("SELECT tablename, hasdefaultrow FROM sys_table_cfg"), m_conn);*/

                DataTable tempTable = GetDataTableWithKeyProxy("sys_table_cfg");
                
                foreach (DataRow dataRow in tempTable.Rows)
                {
                    m_hasDefaultRow[dataRow["tablename"].ToString()] = dataRow["hasdefaultrow"].ToString();
                }
            }

            if (m_hasDefaultRow[tblname] == null || m_hasDefaultRow[tblname].ToString() != "True")
            {
                return null;
            }

            if (m_htDefVal[tblname] == null)
            {
                // Ĭ����һ��������ݱ�ĵ�һ�У��ټ��һ�������Ƿ�Ϊ0����ȷ��
                /*DataTable tbl = CustomClassCache.GetDataTableWithKey("SELECT top 1 * FROM " + tblname);*/
                DataTable tbl = CustomClassCache.GetDataTableWithKeyTopRowProxy(tblname);
                string[] def_keys = new string[tbl.PrimaryKey.Length];
                DataRow defaultRow = tbl.Rows[0];
                bool valid = true;

                for (int i = 0; i < def_keys.Length; i++)
                {
                    if (defaultRow[tbl.PrimaryKey[i]].ToString() != "0")
                    {
                        valid = false;
                        break;
                    }
                }

                if (valid)
                {
                    if (m_htDefVal.Keys.Count > defValCacheSize)
                    {
                        m_htDefVal.Clear();
                    }

                    m_htDefVal[tblname] = defaultRow;
                }
                else
                {
                    defaultRow = null;
                }

                return defaultRow;
            }
            else
            {
                return (DataRow)(m_htDefVal[tblname]);
            }
        }

        public static int SaveTable(DataTable table, ref SqlDataAdapter sqlDataAdapter)
        {
            int nRet = 1;
            if (Program.EnableRemoting)
            {
                mDataTableProxy.SaveTable(table, sqlDataAdapter.SelectCommand.CommandText);
            }
            else
            { 
                SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(sqlDataAdapter);
                sqlDataAdapter.UpdateCommand = cmdBuilder.GetUpdateCommand();
                nRet = sqlDataAdapter.Update(table);               
            }
            table.AcceptChanges();
            return nRet;
        }

    }
}
