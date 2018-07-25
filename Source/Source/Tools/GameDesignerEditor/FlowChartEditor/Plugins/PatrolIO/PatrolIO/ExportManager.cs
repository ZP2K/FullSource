using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using DBManager;

namespace PatrolIO
{
    public class ExportManager
    {
        SqlConnection conn; // sql����
        string sqlStr; // ��Ҫִ�е�sql���
        Hashtable fileTable = new Hashtable(); // �����ļ��Ĺ�ϣ��
        string rootPath; // �ͻ��˸�Ŀ¼

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="rootPath">�ͻ���Ŀ¼</param>
        public ExportManager(string rootPath)
        {
            this.conn = DataBaseManager.GetDataBaseManager().Connection_Jx3web;
            this.rootPath = rootPath;
        }

        /// <summary>
        /// ��Ҫִ�е�sql���
        /// </summary>
        public string SqlString
        {
            get
            {
                return sqlStr;
            }
        }

        /// <summary>
        /// �����ļ��Ĺ�ϣ��
        /// </summary>
        public Hashtable FileTable
        {
            get
            {
                return fileTable;
            }
        }

        /// <summary>
        /// �����������ļ�
        /// </summary>
        /// <param name="pathInfo">���·��</param>
        /// <returns>�ļ������ı�</returns>
        public bool ConstructFile(string pathInfo)
        {
            bool success = true;
            string text = "X\tY\tRestFrame\tFaceTo\tAnimationID\tAniLoop\tIsRun\tScriptName\r\n";
            string fileName = "";

            // ��ֳ�·����Ϣ
            string[] data = pathInfo.Split(new char[]{','});
            string mapName = data[0];
            string patrolID = data[2];
            string patrolPointID = data[3];
            string relevateName = string.Format("scripts\\flowlib\\catcher\\patrol\\{0}_{1}_{2}.lua", data[0], data[2], data[3]);

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM PatrolInfo WHERE MapName = '{0}' AND PatrolID = '{1}' ORDER BY PatrolPointID",
                    mapName, patrolID);
                DataTable table = GetDataTable(sqlString);                
                foreach(DataRow row in table.Rows)
                {
                    StringBuilder line = new StringBuilder();                  
                    line.Append(string.Format("{0}\t", row["X"]));
                    line.Append(string.Format("{0}\t", row["Y"]));
                    line.Append(string.Format("{0}\t", row["RestFrame"]));
                    line.Append(string.Format("{0}\t", row["FaceTo"]));
                    line.Append(string.Format("{0}\t", row["AnimationID"]));
                    line.Append(string.Format("{0}\t", row["AniLoop"]));
                    line.Append(string.Format("{0}\t", row["IsRun"]));
                    if(row["PatrolPointID"].ToString() == patrolPointID) // ֱ�Ӹ���·��
                    {
                        line.Append(string.Format("{0}", relevateName));
                    }
                    else
                    {
                        line.Append(string.Format("{0}", row["ScriptName"]));
                    }                    
                    line.Append("\r\n");
                    text += line.ToString();

                    if(fileName == "")
                    {
                        fileName = Path.Combine(rootPath, row["FileName"].ToString());                        
                    }
                }                 
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ·����Ϣʱ����sql�쳣��" + ex.ToString());
                success = false;
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }                
            }
                                    
            // д�ļ�
            fileTable[fileName] = text;            
            fileName = Path.Combine(rootPath, relevateName);
            string scriptText = string.Format(@"Include('scripts/flowlib/api.lua')
using('comgr')

function OnPatrol(npc)
	print('enter patrol point {0}')
	local co = comgr.findNpc(npc)
	if co then
		print('find co!')
		coroutine.resume(co, '{0}')
	else
		print('not find co!')
	end
end;", pathInfo);
            fileTable[fileName] = scriptText;

            // ִ��sql���
            sqlStr = string.Format("UPDATE PatrolInfo SET ScriptName = '{0}' WHERE MapName = '{1}' AND PatrolID = '{2}' AND PatrolPointID = '{3}'", 
                relevateName, data[0], data[2], data[3]);

            return success;
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sql">sql���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sql)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            System.Data.DataTable tbl = ds.Tables[0];
            return tbl;
        }
    }
}
