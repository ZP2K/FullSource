using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Data;
using DataTableRemotingCommon;

namespace DataTableRemoting
{
    class TableUnit
    {
        private string mTableName;

        private DataTableEx mTable;

        private SqlDataAdapter mAdapter;

        private bool mTableChanged;

        private uint mVersion;

        private string mSqlCmd;

        private SqlConnection mConn;

        /// <summary>
        /// ����TableUnit
        /// </summary>
        /// <param name="conn">SQL����</param>
        /// <param name="tblname">���ݱ���</param>
        public TableUnit(SqlConnection conn, string tblname)
        {
            mTableName = tblname;
            string sql = "select * from " + tblname;
            mSqlCmd = sql;
            mConn = conn;
            LoadDataTable(conn, sql);

        }

        /// <summary>
        /// ����TableUnit
        /// </summary>
        /// <param name="conn">SQL����</param>
        /// <param name="sql">SQL���</param>
        /// <param name="obj">δʹ��</param>
        public TableUnit(SqlConnection conn, string sql, object obj)
        {
            mTableName = sql;
            mSqlCmd = sql;
            mConn = conn;
            LoadDataTable(conn, sql);
        }

        /// <summary>
        /// ȡDataTable
        /// </summary>
        /// <returns></returns>
        public DataTable GetTable()
        {
            return mTable;
        }

        /// <summary>
        /// ������޸ĵ����ݿ�
        /// </summary>
        /// <returns>���������</returns>
        public int SaveTable()
        {
            int nRet = 0;
            if (mTableChanged || true)
            {
                try
                {
                    mAdapter = new SqlDataAdapter(mSqlCmd, mConn);
                    SqlCommandBuilder cmd = new SqlCommandBuilder(mAdapter);
                    mAdapter.UpdateCommand = cmd.GetUpdateCommand();
                    DataTable tbl = mTable.GetChanges();
                    if (tbl != null && tbl.Rows.Count > 0)
                    {
                        nRet = mAdapter.Update(tbl);
                    }
                    mTable.AcceptChanges();
                    mTableChanged = false;
                }
                catch(Exception ex)
                {
                    throw ex;
                }
            }
            return nRet;
        }


        /// <summary>
        /// �ͷ���Դ
        /// </summary>
        public void ReleaseTable()
        {
            mTable.Dispose();
            mAdapter.Dispose();
        }

        /// <summary>
        /// �������ݱ�
        /// </summary>
        /// <param name="conn">SQL����</param>
        /// <param name="sql">SQL��ѯ���</param>
        private void LoadDataTable(SqlConnection conn, string sql)
        {
            mAdapter = new SqlDataAdapter(sql, conn);
            mAdapter.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            mTable = new DataTableEx();
            mAdapter.Fill(mTable);
            mTable.TableName = mTableName;
            mTableChanged = false;
            foreach (DataColumn column in mTable.Columns)
            {
                column.ReadOnly = false;
            }
        }

        /// <summary>
        /// ���û��ȡ�Ƿ����޸�
        /// </summary>
        public bool TableChanged { get { return mTableChanged; } set { mTableChanged = value; } }

    }
}
