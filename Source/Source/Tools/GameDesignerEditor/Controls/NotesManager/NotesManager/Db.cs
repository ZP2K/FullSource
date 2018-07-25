using System;
using System.IO;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;

namespace NotesManager
{
    public class SqlDb
    {

        //���캯��
        public SqlDb()
        {   
            m_sqlCnn = null;
        }


        /// <summary>
        /// Connection����
        /// </summary>
        private string m_strCnn;   // = System.Configuration.ConfigurationSettings.AppSettings["ConnectionString"];
        private SqlConnection m_sqlCnn = null;    //new SqlConnection(m_strCnn);

        /// ִ��delete��insert��update�Ȳ���������ֵΪӰ�������
        public int ExeNoQuery(string strSql)
        {
            int nRet = -1;
            m_strError = "";
            SqlCommand sqlCmd = new SqlCommand(strSql, m_sqlCnn);
            try
            {
                OpenCnn();
                nRet = sqlCmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                nRet = -1;
                m_strError = ex.ToString();
            }
            finally
            {
                if (m_sqlCnn.State == ConnectionState.Open)
                {
                    sqlCmd.Dispose();
                    m_sqlCnn.Close();
                }
            }
            return nRet;
        }

        /// ִ��delete��update,insert����������ֵΪ��Ӱ�������
        public int ExeNoQuery(SqlCommand sqlCmd)
        {
            int nRet = -1;
            m_strError = "";
            sqlCmd.Connection = m_sqlCnn;
            try
            {
                OpenCnn();
                nRet = sqlCmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                nRet = -1;
                m_strError = ex.ToString();
            }
            finally
            {
                if (m_sqlCnn.State == ConnectionState.Open)
                {
                    m_sqlCnn.Close();
                }
            }
            return nRet;
        }

        //�ж��Ƿ����
        public int ExeIsExist(string strSql)
        {
            int nExist = -1;
            m_strError = "";
            SqlCommand sqlCmd = new SqlCommand(strSql, m_sqlCnn);
            try
            {
                OpenCnn();
                SqlDataReader sqlReader = sqlCmd.ExecuteReader();
                if (sqlReader.Read())
                {
                    nExist = 1;
                }
                else
                {
                    nExist = 0;
                }
                sqlReader.Close();

            }
            catch (SqlException ex)
            {
                nExist = -1;
                m_strError = ex.ToString();
            }
            finally
            {
                if (m_sqlCnn.State == ConnectionState.Open)
                {
                    sqlCmd.Dispose();
                    m_sqlCnn.Close();
                }
            }
            return nExist;
        }

        //�ж��Ƿ����
        public int ExeIsExist(SqlCommand sqlCmd)
        {
            int nExist = -1;
            m_strError = "";
            sqlCmd.Connection = m_sqlCnn;
            try
            {
                OpenCnn();
                SqlDataReader sqlReader = sqlCmd.ExecuteReader();
                if (sqlReader.Read())
                {
                    nExist = 1;
                }
                else
                {
                    nExist = 0;
                }
                sqlReader.Close();
            }
            catch (SqlException ex)
            {
                nExist = -1;
                m_strError = ex.ToString();
            }
            finally
            {
                if (m_sqlCnn.State == ConnectionState.Open)
                {
                    m_sqlCnn.Close();
                }
            }
            return nExist;
        }


        //����DataSet
        public DataSet ExeSqlDs(string strSql, string strTable)
        {
            m_strError = "";
            SqlDataAdapter sqlAdapter = new SqlDataAdapter(strSql, m_sqlCnn);
            DataSet dbSet = new DataSet();
            try
            {
                sqlAdapter.Fill(dbSet, strTable);
            }
            catch (SqlException ex)
            {
                dbSet = null;
                m_strError = ex.ToString();
            }

            return dbSet;
        }


        //����DataSet
        public DataSet ExeSqlDs(string strSql, int nStart, int nCount, string strTable)
        {
            m_strError = "";
            SqlDataAdapter sqlAdapter = new SqlDataAdapter(strSql, m_sqlCnn);
            DataSet dbSet = new DataSet();
            try
            {
                sqlAdapter.Fill(dbSet, nStart, nCount, strTable);
            }
            catch (SqlException ex)
            {
                dbSet = null;
                m_strError = ex.ToString();
            }
            return dbSet;
        }


        //����DataReader
        public SqlDataReader ExeSqlDr(string strSql)
        {
            m_strError = "";
            SqlDataReader dr = null;
            SqlCommand sqlCmd = new SqlCommand(strSql, m_sqlCnn);
            try
            {
                OpenCnn();
                dr = sqlCmd.ExecuteReader();
            }
            catch (SqlException ex)
            {
                dr = null;
                m_strError = ex.ToString();
            }
            finally
            {
                if (m_sqlCnn.State == ConnectionState.Open)
                {
                    sqlCmd.Dispose();
                }
            }
            return dr;
        }

        //����DataTable
        public DataTable GetDataTable(string sql)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, m_sqlCnn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            DataTable tbl = ds.Tables[0];
            return tbl;
        }

        //ִ��ExecuteScalar
        public int ExeScalar(string strSql)
        {
            int nCount = -1;
            m_strError = "";
            SqlCommand sqlCmd = new SqlCommand(strSql, m_sqlCnn);
            try
            {
                OpenCnn();
                nCount = (int)sqlCmd.ExecuteScalar();
            }
            catch (SqlException ex)
            {
                nCount = -1;
                m_strError = ex.ToString();
            }
            finally
            {
                if (m_sqlCnn.State == ConnectionState.Open)
                {
                    sqlCmd.Dispose();
                    m_sqlCnn.Close();
                }
            }
            return nCount;
        }

        //��Connection
        private void OpenCnn()
        {
            if (m_sqlCnn.State == ConnectionState.Open)
            {
                m_sqlCnn.Close();
            }
            m_sqlCnn.Open();
        }

        //�ر�Connection
        public void CloseCnn()
        {
            if (m_sqlCnn.State == ConnectionState.Open)
            {
                m_sqlCnn.Close();
            }
        }


        //����DataSet
        public DataSet ExeSqlDs(SqlCommand sqlCmd, string strTable)
        {
            m_strError = "";
            DataSet ds = new DataSet();
            sqlCmd.Connection = m_sqlCnn;
            SqlDataAdapter sqlAdapter = new SqlDataAdapter();
            sqlAdapter.SelectCommand = sqlCmd;
            try
            {
                sqlAdapter.Fill(ds, strTable);
            }
            catch (SqlException ex)
            {
                ds = null;
                m_strError = ex.ToString();
            }
            return ds;
        }


        //�趨Connection �ַ�����new Connection
        public void SetConnectionString(string strSource)
        {
            m_strCnn = strSource;
            m_sqlCnn = null;
            m_sqlCnn = new SqlConnection(m_strCnn);
        }

        //�趨SqlConnection
        public void SetConnection(SqlConnection conn)
        {
            m_sqlCnn = conn;
        }

        /// ������Ϣ
        private string m_strError;


        /// ������Ϣ
        public string StrErrorInfo
        {
            get
            {
                return m_strError;
            }
        }

    }
}

