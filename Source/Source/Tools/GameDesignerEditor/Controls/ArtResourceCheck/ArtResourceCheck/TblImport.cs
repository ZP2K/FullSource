using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Reflection;
using System.Data.OleDb;

namespace ArtResourceCheck
{
    public partial class Helper
    {
        public static bool ImportTab(string tblName, string strFile, SqlConnection Conn)
        {
            //////////////////////////////////////////////////////////////////////////
            // ׼������
            //////////////////////////////////////////////////////////////////////////
            string[] strLines = null;
            string[] strColNames = null;

            DataTable tbl = GetDataTable("SELECT * FROM " + tblName, Conn);
            List<string> newNodeList = new List<string>(); // �����ӵ��������������
            List<string> newColumnList = new List<string>(); // �����ӵ��е���������
            List<DataRow> overwriteRowList = new List<DataRow>(); // ����д�������е�����
            int rowNumber = tbl.Rows.Count; // ���ݱ������
            int iOverwriteCount = 0; // ���ǵ�����
            int iAddCount = 0; // ����������

            // split
            string strContent = FileFolderHelper.FileToString(strFile);
            if (strContent == string.Empty)
            {
                MessageBox.Show("���ܶ�ȡ�ļ����ݣ������ļ����ڱ���������򿪡�", "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }

            strLines = strContent.Split(new char[2] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

            bool bHasTitle = true;
            if (bHasTitle && strLines.Length < 2)
                return false;

            if (bHasTitle)
            {
                strColNames = strLines[0].Split(new char[1] { '\t' }, StringSplitOptions.RemoveEmptyEntries);
                for (int i = 0; i < strColNames.Length; i++)
                {
                    strColNames[i] = strColNames[i].Trim();
                }
            }

            //////////////////////////////////////////////////////////////////////////
            // ѯ���Ƿ�ȫ������
            //////////////////////////////////////////////////////////////////////////
            try // ��ʼ����ʱ����в�������ʵ��д�����ݿ⣬���Ժ������в������쳣
            {
                // ����ֶ�
                foreach (string colname in strColNames)
                {
                    if (!tbl.Columns.Contains(colname)) // ����δ�ڱ���
                    {
                        DataColumn c = tbl.Columns.Add(colname);
                        c.MaxLength = 3000; // ������󳤶�
                        newColumnList.Add(colname);
                    }
                }


                // ��ȡ����, �����Ƿ����±�
                bool bFirstLine = true;
                bool bTblHasPrimKey = (tbl.PrimaryKey.Length != 0);

                foreach (string eachLine in strLines)
                {
                    // ��һ�����ֶ���; ���
                    if (bHasTitle)
                    {
                        if (bFirstLine)
                        {
                            bFirstLine = false;
                            continue;
                        }
                    }
                    string[] strFields = eachLine.Split(new char[1] { '\t' });
                    if (eachLine.Split(new char[1] { '\t' }, StringSplitOptions.RemoveEmptyEntries).Length == 0)
                    {
                        MessageBox.Show("�����ļ����п��У����顣");
                        return false;
                    }

                    // ��ʼ����
                    if (!bTblHasPrimKey) // û����key�����±������ȫ����
                    {
                        DataRow newRow = tbl.NewRow();
                        newRow.BeginEdit();

                        if (!WriteRow(ref tbl, ref newRow, ref strColNames, ref strFields))
                        {
                            MessageBox.Show("����ʧ�ܣ������������ݽ���������!",
                                "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return false;
                        }

                        tbl.Rows.Add(newRow);
                        newRow.EndEdit();

                        iAddCount++;
                    }

                    else // �Ǿɱ���Ҫ����key�ж�ԭ���Ƿ���ڸ���
                    {
                        string strAsking = "�Ѿ����� ";

                        // format key values to find
                        object[] objKeyValues = new object[tbl.PrimaryKey.Length];
                        int iKeyIndex = 0;
                        foreach (DataColumn col in tbl.PrimaryKey)
                        {
                            string primKeyToFind = col.ColumnName;
                            for (int i = 0; i < strColNames.Length; i++)
                            {
                                if (strColNames[i].Trim().ToLower() == primKeyToFind.Trim().ToLower())
                                {
                                    if (strFields[i] == string.Empty)
                                    {
                                        string strError = string.Format("�ڵ���� {0} ʱ����ĳЩ����Ϊ�յ��С�\r\n����Դ�ļ� {1}��", tblName, strFile);
                                        MessageBox.Show(strError, "ʧ��", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                        return false;
                                    }

                                    objKeyValues[iKeyIndex++] = strFields[i];
                                    strAsking += primKeyToFind;
                                    strAsking += "Ϊ";
                                    strAsking += strFields[i];
                                    strAsking += " ";
                                }
                            }
                        }
                        strAsking += "���С�";

                        // find it
                        DataRow row = tbl.Rows.Find(objKeyValues);
                        if (row != null) // ����ͬkey���У�ѯ��
                        {
                            if (!WriteRow(ref tbl, ref row, ref strColNames, ref strFields))
                            {
                                MessageBox.Show("����ʧ�ܣ������������ݽ���������!",
                                    "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return false;
                            }

                            overwriteRowList.Add(row); // �������д������
                            iOverwriteCount++;
                        }

                        // ��������
                        else
                        {
                            DataRow newRow = tbl.NewRow();
                            newRow.BeginEdit();

                            if (!WriteRow(ref tbl, ref newRow, ref strColNames, ref strFields))
                            {
                                MessageBox.Show("����ʧ�ܣ������������ݽ���������!",
                                    "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return false;
                            }

                            tbl.Rows.Add(newRow);
                            newRow.EndEdit();

                            iAddCount++;
                        }
                    }
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show("��ȡ������ʱ�����쳣: " + ex.ToString() + "\r\n\r\n�������ݽ���������",
                                    "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            // �ύ 
            if (Conn.State == ConnectionState.Closed) // ��sql����
            {
                Conn.Open();
            }

            // �������ݿ�ϵͳ�����
            SqlCommand cfg_cmd = Conn.CreateCommand();
            cfg_cmd.CommandText = "sp_configure 'allow updates', '1'";
            cfg_cmd.ExecuteNonQuery();
            cfg_cmd.CommandText = "reconfigure with override";
            cfg_cmd.ExecuteNonQuery();

            SqlTransaction transaction = Conn.BeginTransaction(); // ��������
            SqlCommand cmd = new SqlCommand(); // �½�sql����
            cmd.Connection = Conn;
            cmd.Transaction = transaction; // ���������

            int col_index = 0; // �����
            int index = 0; // �м����α�

            List<string> strColNameList = new List<string>(); // ��¼��������������
            foreach (string s in strColNames)
            {
                strColNameList.Add(s);
            }

            try
            {
                // ����ֶ�
                foreach (string colname in newColumnList)
                {
                    string columnName = colname.Replace("]", "]]"); // �滻�����ַ�
                    cmd.CommandText = string.Format("ALTER TABLE {0} ADD [{1}] [varchar] (3000) NULL", tblName, columnName);
                    cmd.ExecuteNonQuery();
                    newNodeList.Add(colname);
                }

                // ����Ƿ����е�˳�����˱仯
                bool sameOrder = true; // ��˳���Ƿ�仯�ı��
                for (int i = 0; i < strColNames.Length; i++)
                {
                    if (strColNames[i] != tbl.Columns[i].ColumnName)
                    {
                        sameOrder = false;
                        break;
                    }
                }

                if (!sameOrder) // �����е�˳�����˱仯����Ҫ������
                {
                    // ��Ϊϵͳ��������������ܻ�����������⣬������������������ݣ������������¼���
                    cmd.CommandText = string.Format("DELETE FROM {0}", tblName);
                    cmd.ExecuteNonQuery();

                    foreach (DataColumn c in tbl.Columns) // ������colid��1000��Ϊ��������׼��
                    {
                        col_index++;
                        cmd.CommandText = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", col_index + 1000, c.ColumnName, tblName);
                        cmd.ExecuteNonQuery();
                    }

                    col_index = 0;

                    foreach (string colname in strColNames) // ����������
                    {
                        col_index++;
                        cmd.CommandText = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", col_index, colname, tblName);
                        cmd.ExecuteNonQuery();
                    }

                    foreach (DataColumn c in tbl.Columns) // ����������
                    {
                        string colname = c.ColumnName;
                        if (!strColNameList.Contains(colname))
                        {
                            col_index++;
                            cmd.CommandText = string.Format("UPDATE syscolumns SET colid = '{0}' WHERE name = '{1}' AND id = object_id('{2}')", col_index, colname, tblName);
                            cmd.ExecuteNonQuery();
                        }
                    }

                    // �ָ�����Լ�� ����������ܸı���ԭ�ȵ�����Լ��
                    DataColumn[] primaryKeys = tbl.PrimaryKey;
                    if (primaryKeys.Length > 0) // ��������Լ��
                    {
                        string keyString = string.Empty;
                        foreach (DataColumn c in primaryKeys)
                        {
                            keyString = keyString + c.ColumnName + ",";
                        }

                        // ��ȡ����Լ��
                        // cmd.CommandText = string.Format("SELECT constrname FROM sysconstraints WHERE tabid IN (SELECT tabid FROM systables WHERE tabname = '{0}')", tblName);
                        cmd.CommandText = string.Format("SELECT name FROM sysindexes WHERE id = OBJECT_ID('{0}') AND name LIKE 'PK%'", tblName);
                        object constraint = cmd.ExecuteScalar();

                        // ɾ������Լ��
                        cmd.CommandText = string.Format("ALTER TABLE {0} DROP CONSTRAINT {1}", tblName, constraint);
                        cmd.ExecuteNonQuery();

                        // �����µ�����Լ��
                        cmd.CommandText = string.Format("ALTER TABLE {0} ADD CONSTRAINT PK_{1} PRIMARY KEY({2})", tblName, tblName, keyString.TrimEnd(','));
                        cmd.ExecuteNonQuery();
                    }

                    cmd.CommandText = string.Format("SELECT OBJECTPROPERTY(OBJECT_ID('{0}'), 'TableHasIdentity')", tblName);
                    object executeResult = cmd.ExecuteScalar();

                    if (executeResult != null && executeResult.ToString() == "1") // ������ݱ��Ƿ����Զ������
                    {
                        // ����ֵ���뵽�Զ��������
                        cmd.CommandText = string.Format("SET IDENTITY_INSERT {0} ON", tblName);
                        cmd.ExecuteNonQuery();
                    }

                    // ������ӱ�����
                    foreach (DataRow r in tbl.Rows)
                    {
                        string newValue = "";
                        string columns = "";

                        //for (int i = 0; i < strColNames.Length; i++)
                        for (int i = 0; i < tbl.Columns.Count; i++)
                        {
                            string raw_colname = tbl.Columns[i].ColumnName;

                            string value = r[raw_colname].ToString().Replace("'", "''"); // �滻�����ַ�
                            string columnName = raw_colname.Replace("]", "]]"); // �滻�����ַ�
                            if (r[raw_colname] is Boolean) // �ж��Ƿ��ǲ���ֵ
                            {
                                value = (bool)r[raw_colname] ? "1" : "0";
                            }

                            if (value == string.Empty) // �Զ����Կ�ֵ��������ֵĿ�ֵת��������
                            {
                                continue;
                            }
                            else
                            {
                                columns += string.Format("[{0}], ", columnName);
                                newValue += string.Format("'{0}', ", value);
                            }
                        }

                        newValue = newValue.Trim().TrimEnd(',');
                        columns = columns.Trim().TrimEnd(',');
                        cmd.CommandText = string.Format("INSERT INTO {0} ({1}) VALUES ({2})", tblName, columns, newValue);
                        try
                        {
                            cmd.ExecuteNonQuery();
                        }
                        catch (SqlException ex)
                        {
                            MessageBox.Show(ex.Message + "\r\n\r\n" + cmd.CommandText);
                            throw ex;
                        }
                    }
                }
                else // ��˳�򲻱䣬ֻ����������ݼ���
                {
                    // ���Ǿɵ�������
                    if (overwriteRowList.Count > 0)
                    {
                        DataColumn[] keys = tbl.PrimaryKey;
                        foreach (DataRow r in overwriteRowList)
                        {
                            string setValue = "";
                            string keyValue = "";

                            foreach (string s in strColNames)
                            {
                                string columnName = s.Replace("]", "]]"); // �滻�����ַ�
                                string value = r[s].ToString().Replace("'", "''"); // �滻�����ַ�
                                if (r[s] is Boolean) // �ж��Ƿ��ǲ���ֵ
                                {
                                    value = (bool)r[s] ? "1" : "0";
                                }

                                if (value == string.Empty) // ��ֵ��������������Ҫ���⴦��
                                {
                                    setValue += string.Format("[{0}] = NULL, ", columnName);
                                }
                                else
                                {
                                    setValue += string.Format("[{0}] = \'{1}\', ", columnName, value);
                                }
                            }
                            setValue = setValue.Trim().TrimEnd(',');

                            foreach (DataColumn c in keys)
                            {
                                string columnName = c.ColumnName.Replace("]", "]]"); // �滻�����ַ�
                                string value = r[c.ColumnName].ToString().Replace("'", "''"); // �滻�����ַ�
                                keyValue += string.Format("[{0}] = \'{1}\' AND ", columnName, value);
                            }
                            keyValue = keyValue.Substring(0, keyValue.Length - 5);

                            cmd.CommandText = string.Format("UPDATE {0} SET {1} WHERE {2}", tblName, setValue, keyValue);
                            try
                            {
                                cmd.ExecuteNonQuery();
                            }
                            catch (SqlException ex)
                            {
                                MessageBox.Show(ex.Message + "\r\n\r\n" + cmd.CommandText);
                                throw ex;
                            }
                        }
                    }

                    // ������������
                    foreach (DataRow r in tbl.Rows)
                    {
                        if (index < rowNumber) // ���Ծɵ�������
                        {
                            index++;
                            continue;
                        }

                        string newValue = "";
                        string columns = "";

                        for (int i = 0; i < strColNames.Length; i++)
                        {
                            string value = r[strColNames[i]].ToString().Replace("'", "''"); // �滻�����ַ�
                            string columnName = strColNames[i].Replace("]", "]]"); // �滻�����ַ�
                            if (r[strColNames[i]] is Boolean) // �ж��Ƿ��ǲ���ֵ
                            {
                                value = (bool)r[strColNames[i]] ? "1" : "0";
                            }

                            if (value == string.Empty) // �Զ����Կ�ֵ��������ֵĿ�ֵת��������
                            {
                                continue;
                            }
                            else
                            {
                                columns += string.Format("[{0}], ", columnName);
                                newValue += string.Format("'{0}', ", value);
                            }
                        }

                        newValue = newValue.Trim().TrimEnd(',');
                        columns = columns.Trim().TrimEnd(',');
                        cmd.CommandText = string.Format("INSERT INTO {0} ({1}) VALUES ({2})", tblName, columns, newValue);
                        try
                        {
                            cmd.ExecuteNonQuery();
                        }
                        catch (SqlException ex)
                        {
                            MessageBox.Show(ex.Message + "\r\n\r\n" + cmd.CommandText);
                            throw ex;
                        }
                    }
                }

                tbl.AcceptChanges();
                transaction.Commit(); // �ύ����
            }
            catch (SqlException ex)
            {
                MessageBox.Show("���ݸ���ʱ�����쳣: " + ex.ToString() + "\r\n\r\n�������ݽ���������",
                     "�������ݱ�", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                transaction.Rollback(); // ����ع�
                return false;
            }
            finally
            {
                // ��ֹ���ݿ�ϵͳ�����
                cfg_cmd.CommandText = "sp_configure 'allow updates', '0'";
                cfg_cmd.ExecuteNonQuery();
                cfg_cmd.CommandText = "reconfigure with override";
                cfg_cmd.ExecuteNonQuery();

                if (Conn.State == ConnectionState.Open) // �ر�sql����
                {
                    Conn.Close();
                }
            }

            //////////////////////////////////////////////////////////////////////////
            // �����ڴ� ����
            //////////////////////////////////////////////////////////////////////////
            string rpt = string.Format("�ɹ���������,�����У�{0}  �����У�{1}", iOverwriteCount, iAddCount);
            //MessageBox.Show(rpt);
            return true;
        }

        private static bool WriteRow(ref DataTable tbl, ref DataRow row, ref string[] strColNames, ref string[] strFields)
        {
            try
            {
                for (int i = 0; i < strColNames.Length; i++)
                {
                    if (i < strFields.Length)
                    {
                        if (strFields[i] == "")
                            row[strColNames[i]] = DBNull.Value;
                        else
                        {
                            if (tbl.Columns[strColNames[i]].DataType.Name == "Boolean")
                            {
                                if (strFields[i].Trim() == "0")
                                    row[strColNames[i]] = false;
                                else
                                    row[strColNames[i]] = true;
                            }
                            else
                            {
                                try
                                {
                                    row[strColNames[i]] = strFields[i];
                                }
                                catch (Exception ex)
                                {
                                    string err = string.Format("���޸ĵ� {0} �������� (�ֶ���: {1}; �ֶ�ֵ: {2}) ʱ��������, ���޸����ݱ�����µ��롣\r\n\r\n������Ϣ��{3}", i, strColNames[i], strFields[i], ex.Message);
                                    MessageBox.Show(err);
                                    return false;
                                }
                            }
                        }
                    }
                    else
                    {
                        row[strColNames[i]] = DBNull.Value;
                    }
                }
            }
            catch//Exception ex)
            {
                return false;
            }

            return true;
        }
    }
}
