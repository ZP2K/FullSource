////////////////////////////////////////////////////////////
///����˵��:
///
///
///������:  κΰ�� (WilsonWei)
///����ʱ��: 2003-6-14 11:21:0
///��Ȩ����: Copyright Wilson
///
////////////////////////////////////////////////////////////
///

using System;
using System.Data;
using System.Data.SqlClient;
using System.Data.SqlTypes;
using System.Data.OleDb ;
using XSJ.Sword3.Editor.Studio;

namespace XSJ.Sword3.Editor.Studio.Data
{
	#region OleDb Implement
	/*
	public class DataBaseForOleDb : IDisposable
	{
		#region IDisposable Members

		public void Dispose()
		{
			// TODO:  Add DataBaseForOleDb.Dispose implementation
		}

		#endregion
		private string m_SqlConnectionString = String.Empty;

		public string SqlConnectionString
		{
			get{ return m_SqlConnectionString; }
			set{ m_SqlConnectionString = value;}
		}
		public int GetRecordCount ( string sql )
		{
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand sqlcmd = new OleDbCommand( sql,con );
			int iret = Convert.ToInt32 ( sqlcmd.ExecuteScalar ());
			con.Close ();
			return iret;
		}
		public DataTable GetTable( string sql)
		{
			DataTable retTable = new DataTable();
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbDataAdapter sda = new OleDbDataAdapter( sql, con );
			sda.Fill ( retTable );
			con.Close ();
			return retTable;
		}
		public DataTable GetTable( string sql ,string tablename )
		{
			DataTable retTable = new DataTable();
			retTable.TableName = tablename;
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbDataAdapter sda = new OleDbDataAdapter( sql, con );
			sda.Fill ( retTable );
			con.Close ();
			return retTable;
		}
		public DataSet FindAll(string sql,string tablename)
		{
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			DataSet ds = new DataSet();
			OleDbDataAdapter sap = new OleDbDataAdapter(sql,con);
			//ds��ӳ��ı���,����ǵ�������Ҫtablename
			sap.Fill(ds,tablename);
			con.Close ();
			return ds;
		}
		/// <summary>
		/// Run stored procedure.
		/// </summary>
		/// <param name="procName">Name of stored procedure.</param>
		/// <param name="UseTransaction">use sqltransaction?</param>
		/// <returns>iret = cmd.ExecuteNonQuery();</returns>
		public int RunProc(string procName) 
		{
			int iret = -1;
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand cmd = CreateCommand(ref con,procName, null);
			string name = Functions.CreateTemporaryPassword ( 8 );
			iret = cmd.ExecuteNonQuery();
			con.Close ();
			return  iret>0?0:-1;
		}
		

		/// <summary>
		/// Run stored procedure.
		/// </summary>
		/// <param name="procName">Name of stored procedure.</param>
		/// <param name="prams">Stored procedure params.</param>
		/// <returns>iret = cmd.ExecuteNonQuery();</returns>
		public int RunProc(string procName, OleDbParameter[] prams) 
		{
			int iret = -1;
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand cmd = CreateCommand(ref con,procName, prams);
			iret = cmd.ExecuteNonQuery();
			con.Close ();
			return  iret>0?0:-1;
		}
		/// <summary>
		/// ִ��һ���洢���̡�(ExecuteScalar)
		/// </summary>
		/// <param name="procName">�洢������</param>
		/// <param name="prams">����</param>
		/// <returns>���ص�һ�е�һ�е�ֵ</returns>
		public int RunProc2(string procName, OleDbParameter[] prams)
		{
			int iret = -1;
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand cmd = CreateCommand(ref con,procName, prams);
			object obj = cmd.ExecuteScalar ();
			iret = Convert.ToInt32 ( obj );
			con.Close ();
			return  iret ;
		}
		/// <summary>
		/// Run stored procedure(For query only,do not use "insert, update").
		/// </summary>
		/// <param name="procName">Name of stored procedure.</param>
		/// <param name="dataReader">Return result of procedure.</param>
		/// <remarks>cmd.ExecuteReader()</remarks>
		public void RunProc(string procName, out OleDbDataReader dataReader) 
		{
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand cmd = CreateCommand(ref con,procName, null);
			dataReader = cmd.ExecuteReader(System.Data.CommandBehavior.CloseConnection);
		}

		/// <summary>
		/// Run stored procedure.(For query only ,do not use "insert ,update").
		/// </summary>
		/// <param name="procName">Name of stored procedure.</param>
		/// <param name="prams">Stored procedure params.</param>
		/// <param name="dataReader">Return result of procedure.</param>
		/// <remarks>cmd.ExecuteReader()</remarks>
		public void RunProc(string procName, OleDbParameter[] prams, out OleDbDataReader dataReader) 
		{
			OleDbConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand cmd = CreateCommand(ref con,procName, prams);
			dataReader = cmd.ExecuteReader(System.Data.CommandBehavior.CloseConnection);
		}
		/// <summary>
		/// ���� command ����,�����ô洢����.
		/// </summary>
		/// <param name="procName">�洢��������.</param>
		/// <param name="prams">�洢���̲���.</param>
		/// <returns>Command object.</returns>
		private OleDbCommand CreateCommand(ref OleDbConnection con,string procName, OleDbParameter[] prams) 
		{
			OleDbCommand cmd = new OleDbCommand(procName, con);
			cmd.CommandType = CommandType.StoredProcedure;
			// add proc parameters
			if (prams != null) 
			{
				foreach (OleDbParameter parameter in prams)
					cmd.Parameters.Add(parameter);
			}
			// return param
			cmd.Parameters.Add(
				new OleDbParameter("ReturnValue", OleDbType.Integer , 4,
				ParameterDirection.ReturnValue, false, 0, 0,
				string.Empty, DataRowVersion.Default, null));
			return cmd;
		}

		/// <summary>
		/// Make input param.
		/// </summary>
		/// <param name="ParamName">Name of param.</param>
		/// <param name="DbType">Param type.</param>
		/// <param name="Size">Param size.</param>
		/// <param name="Value">Param value.</param>
		/// <returns>New parameter.</returns>
		public OleDbParameter MakeInParam(string ParamName, OleDbDbType DbType, int Size, object Value) 
		{
			return MakeParam(ParamName, DbType, Size, ParameterDirection.Input, Value);
		} 

		/// <summary>
		/// Make output param.
		/// </summary>
		/// <param name="ParamName">Name of param.</param>
		/// <param name="DbType">Param type.</param>
		/// <param name="Size">Param size.</param>
		/// <returns>New parameter.</returns>
		public OleDbParameter MakeOutParam(string ParamName, OleDbDbType DbType, int Size) 
		{
			return MakeParam(ParamName, DbType, Size, ParameterDirection.Output, null);
		} 
		#endregion
		/// <summary>
		/// ����StoreProcedure ����
		/// </summary>
		/// <param name="ParamName">Parameter's name</param>
		/// <param name="DbType">Sql data type</param>
		/// <param name="Size">size of the field</param>
		/// <param name="Direction">direction of the parameter.</param>
		/// <param name="Value">initial value of the parameter.</param>
		/// <returns>return a new parameter object.</returns>
		public OleDbParameter MakeParam(string ParamName,OleDbDbType DbType, Int32 Size, ParameterDirection Direction,object Value)
		{
			OleDbParameter param = null;
			if( !ParamName.StartsWith ("@"))
			{
				ParamName = "@" +ParamName;
			}
			if( Size > 0)
			{
				param = new OleDbParameter(ParamName, DbType, Size );
			}
			else
			{
				param = new OleDbParameter(ParamName, DbType);
			}
			param.Direction = Direction;
			if( !(Direction == ParameterDirection.Output && Value == null))
				param.Value = Value;
			param.SourceColumn = ParamName.Substring (1);
			return param;
		}
	
		/// <summary>
		/// //����command����,������SQL���ִ��
		/// </summary>
		private OleDbCommand CreateSqlCommand(ref OleDbConnection con,string strSqlName, OleDbParameter[] prams)
		{
			OleDbCommand cmd = new OleDbCommand(strSqlName,con);
			cmd.CommandType=CommandType.Text;
			cmd.Connection=con;
			// add proc parameters
			if (prams != null) 
			{
				foreach (OleDbParameter parameter in prams)
					cmd.Parameters.Add(parameter);
			}

			// return param
			cmd.Parameters.Add(
				new OleDbParameter("ReturnValue", OleDbType.Integer , 4,
				ParameterDirection.ReturnValue, false, 0, 0,
				string.Empty, DataRowVersion.Default, null));

			return cmd;
		}

		/// <summary>
		/// Run Transact-SQL ��� 
		/// </summary>
		/// <param name="strSqlName">Sql string</param>
		/// <returns>iret = cmd.ExecuteNonQuery();</returns>
		public int RunSql(string strSqlName)
		{
			int iret = -1;
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand cmd = CreateSqlCommand(ref con ,strSqlName, null);
			iret = cmd.ExecuteNonQuery();
			con.Close ();
			return iret>0?0:-1;
		}
		/// <summary>
		/// Run Transact-SQL language
		/// </summary>
		/// <param name="strSqlName">Name of Transact-SQL language.</param>
		/// <returns>iret = cmd.ExecuteNonQuery();</returns>
		public int RunSql(string strSqlName, OleDbParameter[] prams)
		{
			int iret = -1;
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand cmd = CreateSqlCommand(ref con,strSqlName, prams);
			iret = cmd.ExecuteNonQuery();
			con.Close ();
			return iret>0?0:-1;
		}
		/// <summary>
		/// Run Transact-SQL language(Query only)
		/// </summary>
		/// <param name="strSqlName">Name of Transact-SQL language.</param>
		/// <param name="dataReader">Return result of Sql.</param>
		public void RunSql(string strSqlName, out OleDbDataReader dataReader)
		{
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand cmd = CreateSqlCommand(ref con,strSqlName, null);
			dataReader = cmd.ExecuteReader(System.Data.CommandBehavior.CloseConnection);
		}
		/// <summary>
		/// Run Transact-SQL language(Query Only)
		/// </summary>
		/// <param name="strSqlName">Name of Transact-SQL language.</param>
		/// <param name="dataReader">Return result of Sql.</param>
		public void RunSql(string strSqlName, OleDbParameter[] prams,out OleDbDataReader dataReader)
		{
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand cmd = CreateSqlCommand(ref con,strSqlName, prams);
			dataReader = cmd.ExecuteReader(System.Data.CommandBehavior.CloseConnection);
		}
		/// <summary>
		/// ���Է��أ۹ؼ��ֶε�ֵ��
		/// </summary>
		/// <param name="strSqlName"></param>
		/// <param name="prams"></param>
		/// <returns>���أ۹ؼ��ֶε�ֵ��</returns>
		/// <remarks>cmd.ExecuteScalar();</remarks>
		public int RunSql2(string strSqlName, OleDbParameter[] prams)
		{
			int iret = -1;
			OleDbConnection con = new OleDbConnection( m_SqlConnectionString );
			con.Open ();
			OleDbCommand cmd = CreateSqlCommand(ref con,strSqlName, prams);
			object obj = cmd.ExecuteScalar ();
			con.Close ();
			return iret;
		}
	}
*/
	#endregion
	/// <summary>
	///  ADO.NET data access using the SQL Server Managed Provider.
	/// </summary>
	public class Database: IDisposable
	{
		private string m_SqlConnectionString = String.Empty;

		/// <summary>
		/// ��ȡ/�������ݿ������ַ���
		/// </summary>
		public string SqlConnectionString
		{
			get{ return m_SqlConnectionString; }
			set{ m_SqlConnectionString = value;}
		}
        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="msg">������Ϣ</param>
        /// <param name="stackinfo">��ջ��Ϣ</param>
        /// <returns>����������ӳɹ�,�򷵻�true,���򷵻�false.</returns>
        public bool TryConnect(out string msg,out string stackinfo)
        {
            bool bret = false;
            msg = string.Empty;
            stackinfo = string.Empty;
            SqlConnection con = new SqlConnection(m_SqlConnectionString);
            try
            {
                con.Open();
                bret = true;
                con.Close();
            }
            catch(SqlException ex)
            {
                msg = ex.Message ;
                stackinfo = ex.StackTrace;
                bret = false;
            }
            return bret;
        }
		/// <summary>
		/// ����T-SQL����ȡ���������ļ�¼��
		/// ����T-SQL����м���"count([field])"�ؼ���,��:"select count(*) from xxx"
		/// ���û�м�,��"select * from xxx",����᷵���޷�Ԥ�ڵĽ��!
		/// </summary>
		/// <param name="sql">����T-SQL����м���"count([field])"�ؼ���,��:"select count(*) from xxx"</param>
		/// <returns>���ؼ������ļ�¼��</returns>
		public int GetRecordCount ( string sql )
		{
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand sqlcmd = new SqlCommand( sql,con );
			int iret = Convert.ToInt32 (sqlcmd.ExecuteScalar());
			con.Close ();
			return iret;
		}
		/// <summary>
		/// ����SQL����ȡ���ݱ�
		/// </summary>
		/// <param name="sql">SQL���</param>
		/// <returns>�����ѻ�ȡ�ı����</returns>
		public DataTable GetTable( string sql)
		{
			DataTable retTable = new DataTable();
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlDataAdapter sda = new SqlDataAdapter( sql, con );
			sda.Fill ( retTable );
			con.Close ();
			return retTable;
		}
		/// <summary>
		/// ����SQL����ȡ���ݱ�
		/// </summary>
		/// <param name="sql">SQL���</param>
		/// <param name="tablename">ָ��������(�����ѻ�ȡ�ı�ȡ������)</param>
		/// <returns>�����ѻ�ȡ�ı����</returns>
		public DataTable GetTable( string sql ,string tablename )
		{
			DataTable retTable = new DataTable();
			retTable.TableName = tablename;
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlDataAdapter sda = new SqlDataAdapter( sql, con );
			sda.Fill ( retTable );
			con.Close ();
			return retTable;
		}
		public DataSet FindAll(string sql,string tablename)
		{
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			DataSet ds = new DataSet();
			SqlDataAdapter sap = new SqlDataAdapter(sql,con);
			//ds��ӳ��ı���,����ǵ�������Ҫtablename
			sap.Fill(ds,tablename);
			con.Close ();
			return ds;
		}
		/// <summary>
		/// Run stored procedure.
		/// </summary>
		/// <param name="procName">Name of stored procedure.</param>
		/// <param name="UseTransaction">use sqltransaction?</param>
		/// <returns>iret = cmd.ExecuteNonQuery();</returns>
		public int RunProc(string procName, bool UseTransaction) 
		{
			int iret = -1;
			SqlTransaction trans = null ;
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand cmd = CreateCommand(ref con,procName, null);
			cmd.Transaction = trans;
			string name = Functions.CreateTemporaryPassword ( 8 );
			if( UseTransaction)
			{
				trans = con.BeginTransaction ( name );
				cmd.Transaction = trans;
			}
			try
			{
				iret = cmd.ExecuteNonQuery();
				if( UseTransaction) trans.Commit ();
				//iret = (int)cmd.Parameters["ReturnValue"].Value;
			}
			catch
			{
				if( UseTransaction)
					trans.Rollback ( name );
			}
            cmd.Parameters.Clear();
			con.Close ();
			return  iret>0?0:-1;
		}
		

		/// <summary>
		/// Run stored procedure.
		/// </summary>
		/// <param name="procName">Name of stored procedure.</param>
		/// <param name="prams">Stored procedure params.</param>
		/// <returns>iret = cmd.ExecuteNonQuery();</returns>
		public int RunProc(string procName, SqlParameter[] prams,bool UseTransaction) 
		{
			int iret = -1;
			SqlTransaction trans = null ;
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand cmd = CreateCommand(ref con,procName, prams);
			string name = Functions.CreateTemporaryPassword ( 8 );
			if( UseTransaction)
			{
				trans = con.BeginTransaction ( name );
				cmd.Transaction = trans;
			}
			try
			{
				iret = cmd.ExecuteNonQuery();
				if( UseTransaction )
					trans.Commit ();
			}
			catch(Exception ex)
			{
				if( UseTransaction )
					trans.Rollback ( name );
                Console.WriteLine(ex.Message +ex.StackTrace);
			}
            cmd.Parameters.Clear();
			con.Close ();
			return  iret>0?0:-1;
		}
		/// <summary>
		/// ִ��һ���洢���̡�(ExecuteScalar)
		/// </summary>
		/// <param name="procName">�洢������</param>
		/// <param name="prams">����</param>
		/// <param name="UseTransaction">�Ƿ�ʹ������</param>
		/// <returns>���ص�һ�е�һ�е�ֵ</returns>
		public int RunProc2(string procName, SqlParameter[] prams, bool UseTransaction)
		{
			int iret = -1;
			SqlTransaction trans = null ;
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand cmd = CreateCommand(ref con,procName, prams);
			cmd.Transaction = trans;
			string name = Functions.CreateTemporaryPassword ( 8 );
			if( UseTransaction )
			{
				trans = con.BeginTransaction ( name );
				cmd.Transaction = trans;
			}
			try
			{
				object obj = cmd.ExecuteScalar ();
				if( UseTransaction)
					trans.Commit ();
				iret = Convert.ToInt32 ( obj );
			}
			catch( Exception ex )
			{
				if( UseTransaction)
					trans.Rollback ( name );
                Console.WriteLine(ex.Message + ex.StackTrace);
				iret = -1;
			}
            cmd.Parameters.Clear();
			con.Close();
			return  iret ;
		}
		/// <summary>
		/// Run stored procedure(For query only,do not use "insert, update").
		/// </summary>
		/// <param name="procName">Name of stored procedure.</param>
		/// <param name="dataReader">Return result of procedure.</param>
		/// <remarks>cmd.ExecuteReader()</remarks>
		public void RunProc(string procName, out SqlDataReader dataReader) 
		{
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand cmd = CreateCommand(ref con,procName, null);
			dataReader = cmd.ExecuteReader(System.Data.CommandBehavior.CloseConnection);
		}

		/// <summary>
		/// Run stored procedure.(For query only ,do not use "insert ,update").
		/// </summary>
		/// <param name="procName">Name of stored procedure.</param>
		/// <param name="prams">Stored procedure params.</param>
		/// <param name="dataReader">Return result of procedure.</param>
		/// <remarks>cmd.ExecuteReader()</remarks>
		public void RunProc(string procName, SqlParameter[] prams, out SqlDataReader dataReader) 
		{
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand cmd = CreateCommand(ref con,procName, prams);
			dataReader = cmd.ExecuteReader(System.Data.CommandBehavior.CloseConnection);
		}
		/// <summary>
		/// ���� command ����,�����ô洢����.
		/// </summary>
		/// <param name="procName">�洢��������.</param>
		/// <param name="prams">�洢���̲���.</param>
		/// <returns>Command object.</returns>
		private SqlCommand CreateCommand(ref SqlConnection con,string procName, SqlParameter[] prams) 
		{
			//command = new SqlCommand( sprocName, new SqlConnection( ConfigManager.DALConnectionString ) );
			SqlCommand cmd = new SqlCommand(procName, con);
			cmd.CommandType = CommandType.StoredProcedure;

			// add proc parameters
			if (prams != null) 
			{
                for (int i = 0; i < prams.Length; i++)
                {
                    SqlParameter parameter = (SqlParameter)prams[i];
                    cmd.Parameters.Add(parameter);
                }
			}

			// return param
			cmd.Parameters.Add(
				new SqlParameter("ReturnValue", SqlDbType.Int, 4,
				ParameterDirection.ReturnValue, false, 0, 0,
				string.Empty, DataRowVersion.Default, null));
			return cmd;
		}
		/// <summary>
		/// Release resources.
		/// </summary>
		public void Dispose() 
		{
			// make sure connection is closed
		}

		/// <summary>
		/// Make input param.
		/// </summary>
		/// <param name="ParamName">Name of param.</param>
		/// <param name="DbType">Param type.</param>
		/// <param name="Size">Param size.</param>
		/// <param name="Value">Param value.</param>
		/// <returns>New parameter.</returns>
		public SqlParameter MakeInParam(string ParamName, SqlDbType DbType, int Size, object Value) 
		{
			return MakeParam(ParamName, DbType, Size, ParameterDirection.Input, Value);
		} 

		/// <summary>
		/// Make output param.
		/// </summary>
		/// <param name="ParamName">Name of param.</param>
		/// <param name="DbType">Param type.</param>
		/// <param name="Size">Param size.</param>
		/// <returns>New parameter.</returns>
		public SqlParameter MakeOutParam(string ParamName, SqlDbType DbType, int Size) 
		{
			return MakeParam(ParamName, DbType, Size, ParameterDirection.Output, null);
		} 

		#region old will be delete...
		/*
		/// <summary>
		/// ����Sql����.
		/// </summary>
		/// <param name="ParamName">������.</param>
		/// <param name="DbType">��������.</param>
		/// <param name="Size">������С.</param>
		/// <param name="Direction">��������.</param>
		/// <param name="Value">����ֵ.</param>
		/// <returns>New parameter.</returns>
		public SqlParameter MakeSqlParam(string ParamName, SqlDbType DbType, Int32 Size, ParameterDirection Direction, object Value) 
		{
			SqlParameter param = null;

			if(Size > 0)
				param = new SqlParameter(ParamName, DbType, Size);
			else
				param = new SqlParameter(ParamName, DbType);

			param.Direction = Direction;
			if (!(Direction == ParameterDirection.Output && Value == null))
				param.Value = Value;

			return param;
		}
		*/
		#endregion
		/// <summary>
		/// ����StoreProcedure ����
		/// </summary>
		/// <param name="ParamName">Parameter's name</param>
		/// <param name="DbType">Sql data type</param>
		/// <param name="Size">size of the field</param>
		/// <param name="Direction">direction of the parameter.</param>
		/// <param name="Value">initial value of the parameter.</param>
		/// <returns>return a new parameter object.</returns>
		public SqlParameter MakeParam(string ParamName,SqlDbType DbType, Int32 Size, ParameterDirection Direction,object Value)
		{
			SqlParameter param = null;
			if( !ParamName.StartsWith ("@"))
			{
				ParamName = "@" +ParamName;
			}
			if( Size > 0)
			{
				param = new SqlParameter(ParamName, DbType, Size );
			}
			else
			{
				param = new SqlParameter(ParamName, DbType);
			}
			param.Direction = Direction;
			if( !(Direction == ParameterDirection.Output && Value == null))
				param.Value = Value;
			param.SourceColumn = ParamName.Substring (1);
			return param;
		}
	
		/// <summary>
		/// //����command����,������SQL���ִ��
		/// </summary>
		/// <param name="con">���ݿ����Ӷ���</param>
		/// <param name="prams">Ҫ���ɵĲ���</param>
		/// <param name="strSqlName">ָ��������������</param>
		private SqlCommand CreateSqlCommand(ref SqlConnection con,string strSqlName, SqlParameter[] prams)
		{
			SqlCommand cmd = new SqlCommand(strSqlName,con);
			cmd.CommandType=CommandType.Text;
			cmd.Connection=con;
			// add proc parameters
			if (prams != null) 
			{
				foreach (SqlParameter parameter in prams)
					cmd.Parameters.Add(parameter);
			}

			// return param
			cmd.Parameters.Add(
				new SqlParameter("ReturnValue", SqlDbType.Int, 4,
				ParameterDirection.ReturnValue, false, 0, 0,
				string.Empty, DataRowVersion.Default, null));

			return cmd;
		}

		/// <summary>
		/// Run Transact-SQL ��� 
		/// </summary>
		/// <param name="strSqlName">Sql string</param>
		/// <returns>iret = cmd.ExecuteNonQuery();</returns>
		public int RunSql(string strSqlName,bool UseTransaction)
		{
			int iret = -1;
			SqlTransaction trans = null;
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand cmd = CreateSqlCommand(ref con ,strSqlName, null);
			cmd.Transaction = trans;
			string name = Functions.CreateTemporaryPassword ( 8 );
			if( UseTransaction)
			{
				trans = con.BeginTransaction ();
				cmd.Transaction = trans;
			}
			try
			{
				iret = cmd.ExecuteNonQuery();
				if( UseTransaction )
					trans.Commit ();
				//iret = (int)cmd.Parameters["ReturnValue"].Value;
			}
			catch
			{
				if( UseTransaction ) trans.Rollback ( name );
			}
            cmd.Parameters.Clear();
			con.Close ();
			return iret>0?0:-1;
		}
		/// <summary>
		/// Run Transact-SQL language
		/// </summary>
		/// <param name="strSqlName">Name of Transact-SQL language.</param>
		/// <returns>iret = cmd.ExecuteNonQuery();</returns>
		public int RunSql(string strSqlName, SqlParameter[] prams,bool UseTransaction)
		{
			int iret = -1;
			SqlTransaction trans = null;
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand cmd = CreateSqlCommand(ref con,strSqlName, prams);
			cmd.Transaction = trans;
			string name = Functions.CreateTemporaryPassword ( 8 );
			if( UseTransaction )
			{
				trans = con.BeginTransaction ( name );
				cmd.Transaction = trans;
			}
			try
			{
				iret = cmd.ExecuteNonQuery();
				if( UseTransaction )
					trans.Commit ();
				//iret = (int)cmd.Parameters["ReturnValue"].Value;
			}
			catch
			{
				if( UseTransaction )
					trans.Rollback ( name );
			}
            cmd.Parameters.Clear();
			con.Close ();
			return iret>0?0:-1;
		}
		/// <summary>
		/// Run Transact-SQL language(Query only)
		/// </summary>
		/// <param name="strSqlName">Name of Transact-SQL language.</param>
		/// <param name="dataReader">Return result of Sql.</param>
		public void RunSql(string strSqlName, out SqlDataReader dataReader)
		{
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand cmd = CreateSqlCommand(ref con,strSqlName, null);
			dataReader = cmd.ExecuteReader(System.Data.CommandBehavior.CloseConnection);
		}
		/// <summary>
		/// Run Transact-SQL language(Query Only)
		/// </summary>
		/// <param name="strSqlName">Name of Transact-SQL language.</param>
		/// <param name="dataReader">Return result of Sql.</param>
		public void RunSql(string strSqlName, SqlParameter[] prams,out SqlDataReader dataReader)
		{
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand cmd = CreateSqlCommand(ref con,strSqlName, prams);
			dataReader = cmd.ExecuteReader(System.Data.CommandBehavior.CloseConnection);
		}
		/// <summary>
		/// ���Է��أ۹ؼ��ֶε�ֵ��
		/// </summary>
		/// <param name="strSqlName"></param>
		/// <param name="prams"></param>
		/// <returns>���أ۹ؼ��ֶε�ֵ��</returns>
		/// <remarks>cmd.ExecuteScalar();</remarks>
		public int RunSql2(string strSqlName, SqlParameter[] prams,bool UseTransaction)
		{
			int iret = -1;
			SqlTransaction trans = null;
			SqlConnection con = new SqlConnection( m_SqlConnectionString );
			con.Open ();
			SqlCommand cmd = CreateSqlCommand(ref con,strSqlName, prams);
			cmd.Transaction = trans;
			string name = Functions.CreateTemporaryPassword ( 8 );
			if( UseTransaction )
			{
				trans = con.BeginTransaction ( name );
				cmd.Transaction = trans;
			}
			try
			{
				object obj = cmd.ExecuteScalar ();
				if( UseTransaction )
					trans.Commit ();
				iret = Convert.ToInt32 ( obj );
			}
			catch
			{
				if( UseTransaction )
					trans.Rollback ( name );
			}
            cmd.Parameters.Clear();
			con.Close ();
			return iret;
		}
        /// <summary>
        /// ���Է��أ۹ؼ��ֶε�ֵ��
        /// </summary>
        /// <param name="strSql">��ѯ���</param>
        /// <returns>���أ۹ؼ��ֶε�ֵ��</returns>
        /// <remarks>cmd.ExecuteScalar();</remarks>
        public int RunSql2(string strSql)
        {
            int iret = -1;
            SqlConnection con = new SqlConnection(m_SqlConnectionString);
            con.Open();
            SqlCommand cmd = CreateSqlCommand(ref con, strSql, null);
            string name = Functions.CreateTemporaryPassword(8);
            object obj = cmd.ExecuteScalar();
            iret = Convert.ToInt32(obj);
            con.Close();
            return iret;
        }
	}
}