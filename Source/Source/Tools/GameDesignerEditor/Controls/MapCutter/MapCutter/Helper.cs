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

namespace MapCutter
{
    class Helper
    {
        [DllImport("user32.dll")]//, CharSet = CharSet.Unicode)]
        public static extern IntPtr PostMessage(IntPtr hwnd, int wMsg, IntPtr wParam, string lParam);
        [DllImport("user32.dll")]
        static extern void LockWindowUpdate(IntPtr handle);
        [DllImport("Kernel32")]
        public static extern int GetProcAddress(int handle, String funcname);
        [DllImport("Kernel32")]
        public static extern int LoadLibrary(String funcname);
        [DllImport("Kernel32")]
        public static extern int FreeLibrary(int handle);
        public delegate void DllEntryPoint();
        [DllImport("user32.dll", EntryPoint = "FindWindow", CharSet = CharSet.Auto)]
        public extern static IntPtr FindWindow(string lpClassName, string lpWindowName);

        public static void LockWndUpdate(Control ctrl)
        {
            if (ctrl != null)
                LockWindowUpdate(ctrl.Handle);
            else
                LockWindowUpdate(IntPtr.Zero);
        }

        /// <summary>
        /// �رմ��弰�����е������ж���Ĵ���
        /// </summary>
        /// <param name="form">����</param>
        /// <returns>�����ж���Ĵ����Ƿ��Ѿ�ȫ���ر�</returns>
        public static bool CloseAllWindows(Form form)
        {
            bool result = true;

            try // ��ȡ���ԵĹ��̿��ܻ�����쳣
            {
                Type t = form.GetType();
                foreach (PropertyInfo pi in t.GetProperties()) // �÷����ȡ������������ԣ��������Դ���ر�
                {
                    object o = t.InvokeMember(pi.Name, BindingFlags.GetProperty, null, form, null);
                    if (o is Form) // ��������
                    {
                        Form newForm = o as Form;
                        if (newForm != null)
                        {
                            newForm.Close();
                            if (!newForm.IsDisposed && newForm.Visible) // ���ִ������Ԫ�༭����ֻ����û�رգ���Ҫ�������ж�
                            {
                                result = false;
                                break;
                            }
                        }
                    }
                }
            }
            catch { } // �����쳣

            if(result) // ���д��������Ѿ��ɹ��ر�
            {
                IntPtr ptr = FindWindow(null, form.Text);
                PostMessage(ptr, 0x10, IntPtr.Zero, null);
            }

            return result;
        }

        public static string GetVersionInfo()
        {
            string strPath = Path.GetDirectoryName(Process.GetCurrentProcess().MainModule.FileName) + @"\";
            //string strFilename = Process.GetCurrentProcess().MainModule.FileName.Split(new string[]{".vshost.exe"}, StringSplitOptions.RemoveEmptyEntries)[0] + ".exe";
            string strFilename = strPath + "GameDesingerTools.exe";
            Assembly assembly = Assembly.LoadFile(strFilename);
            string fileVersion = null;
            object[] fileVersionAttributes = assembly.GetCustomAttributes(typeof(AssemblyFileVersionAttribute), true);
            if (fileVersionAttributes.Length != 0)
            {
                fileVersion = (fileVersionAttributes[0] as AssemblyFileVersionAttribute).Version.ToString();
            }
            return fileVersion;
        }
        //��HASHTABLE��KEYֵתΪȫ��д����ȫСд
        public static Hashtable TransHashTableCase(Hashtable htSrc, bool bConverToLower)
        {
            Hashtable ht = new Hashtable();
            foreach (DictionaryEntry entry in htSrc)
            {
                if (bConverToLower)
                    ht[entry.Key.ToString().ToLower()] = entry.Value;
                else
                    ht[entry.Key.ToString().ToUpper()] = entry.Value;
            }
            return ht;
        }
        public static Hashtable TransToHashTable(object[] objarr, bool bForceToLower)
        {
            Hashtable ht = new Hashtable();
            foreach(object obj in objarr)
            {
                if (bForceToLower)
                    ht[obj.ToString().ToLower()] = obj;
                else
                    ht[obj.ToString()] = obj;
            }
            return ht;
        }
        private static Delegate GetAddress(int dllModule, string functionname, Type t)
        {
            int addr = GetProcAddress(dllModule, functionname);
            if (addr == 0)
                return null;
            else
                return Marshal.GetDelegateForFunctionPointer(new IntPtr(addr), t);
        }

        public static bool RegComDll(string strFullFileName)
        {
            int hdll = 0;
            hdll = LoadLibrary(strFullFileName);
            try
            {
                DllEntryPoint regdll = (DllEntryPoint)GetAddress(hdll, "DllRegisterServer", typeof(DllEntryPoint));
                if (regdll == null)
                    return false;
                regdll();
            }
            finally
            {
                FreeLibrary(hdll);
            }
            return true;
        }

        public static bool CompareStringArray(string[] src, string[] des, bool bIgoreCase)
        {
            if (des.Length != src.Length)
                return false;
            
            Hashtable htsrc = new Hashtable();
            for(int i = 0; i < src.Length; i ++)
            {
                string strkey = bIgoreCase ? src[i].ToLower() : src[i];
                htsrc.Add(strkey, i + 1);
            }
            for (int i = 0; i < des.Length; i++)
            {
                string strkey = bIgoreCase ? des[i].ToLower() : des[i];
                if (htsrc[strkey] == null)
                    return false;
            }
            return true;
        }

        public static void FillDBNullWithDefault(ref System.Data.DataTable tbl)
        {
            if (tbl.PrimaryKey.Length == 0)
                return;

            object[] keys = new object[tbl.PrimaryKey.Length];
            for (int i = 0; i < keys.Length; i++)
            {
                keys[i] = 0;
            }
            DataRow default_row = tbl.Rows.Find(keys);
            if (default_row != null)
            {
                foreach (DataRow row in tbl.Rows)
                {
                    foreach (DataColumn col in tbl.Columns)
                    {
                        if (row[col.ColumnName] == DBNull.Value)
                            row[col.ColumnName] = default_row[col.ColumnName];
                    }
                }
            }
        }

        public static bool IsColumnInTable(string colname, System.Data.DataTable tbl)
        {
            foreach (DataColumn col in tbl.Columns)
            {
                if (colname.Trim().ToLower() == col.ColumnName.Trim().ToLower())
                {
                    return true;
                }
            }
            return false;
        }

        public static bool FieldIsTableKey(string fieldname, System.Data.DataTable tbl)
        {
            foreach (DataColumn col in tbl.PrimaryKey)
            {
                if (col.ColumnName.Trim() == fieldname)
                {
                    return true;
                }
            }
            return false;
        }

        public static bool FieldIsTableKey(DataColumn col, System.Data.DataTable tbl)
        {
            foreach (DataColumn c in tbl.PrimaryKey)
            {
                if (col.ColumnName.Trim() == c.ColumnName.Trim())
                {
                    return true;
                }
            }
            return false;
        }

        public static bool RenameDbTable(string oldTblName, string newTblName, SqlConnection Conn)
        {
            SqlCommand cmd = Conn.CreateCommand();
            string sql = string.Format("sp_rename '{0}', '{1}', 'OBJECT'", oldTblName, newTblName);
            cmd.CommandText = sql;
            cmd.ExecuteNonQuery();
            return true;
        }

        public static bool RenameDbField(string tblName, string oldFieldName, string newFieldName, SqlConnection Conn)
        {
            SqlCommand cmd = Conn.CreateCommand();
            string _Old = tblName + "." + oldFieldName;
            string sql = string.Format("sp_rename '{0}', '{1}', 'COLUMN'", _Old, newFieldName);
            cmd.CommandText = sql;
            cmd.ExecuteNonQuery();
            return true;
        }

        public static bool RemoveDbTable(string tblName, SqlConnection Conn)
        {
            bool bSuc = false;
            Conn.Open();
            try
            {
                SqlCommand cmd = Conn.CreateCommand();
                cmd.CommandText = "DROP TABLE " + tblName;
                cmd.ExecuteNonQuery();
                bSuc = true;
            }
            catch (Exception ex)
            {
                string err = string.Format("��ɾ���� {0} ʱ��������\r\n\r\n������Ϣ��{1}", tblName, ex.Message);
                MessageBox.Show(err);
                bSuc = false;
            }
            Conn.Close();
            return bSuc;
        }



        public static System.Data.DataTable GetDataTable(string sql, SqlConnection conn)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            System.Data.DataTable tbl = ds.Tables[0];
            return tbl;
        }

        public static byte[] GetByteArrayFromString(string str)
        {
            System.Text.Encoding encoding = System.Text.Encoding.UTF8;
            return encoding.GetBytes(str);
        }
        public static string GetStringFromByteArray(object value)
        {
            if (value == null || value == DBNull.Value)
                return "";
            byte[] byArray = (byte[])value;
            System.Text.Encoding encoding = System.Text.Encoding.UTF8;
            return encoding.GetString(byArray);
        }

        public static string AddSlash(string sPath)
        {
            if (sPath.Length > 0 && sPath.Substring(sPath.Length - 1) != "\\")
            {
                sPath += "\\";
                return sPath;
            }
            return sPath;
        }
        private static string PreProcessString(ref string s1, ref string s2)
        {
            bool b2 = s2.Trim().Length > 0;
            string strfullpath = s2.Trim().Length > 0 ? s2 : s1;
            int nFindFileName = strfullpath.LastIndexOf('\\');
            string filename = "";
            if (nFindFileName >= 0)
            {
                filename = strfullpath.Substring(nFindFileName + 1);
                if (b2)
                    s2 = s2.Substring(0, nFindFileName + 1);
                else
                    s1 = s1.Substring(0, nFindFileName + 1);
            }
            return filename;

        }

        public static string[] split(string strinput, string sp)
        {
            string tmp = "";
            int strlen = 0, splen = 0;
            int found = 0;
            string[] rt = null;//��������

            try
            {
                //�����ж�
                if (strinput == null || sp == null || strinput.Length == 0 || sp.Length == 0) return null;

                //��ʼ��һ�������б�������̬���飩
                ArrayList tmp3 = new ArrayList();

                strlen = strinput.Length;//��������ַ����ĳ���
                splen = sp.Length;//�ָ����ĳ���
                for (int i = 0; i < strlen; i++)
                {
                    //���ҷָ���
                    found = strinput.IndexOf(sp, i);
                    if (found >= 0)
                    {
                        tmp = "";
                        //ȡ�ָ���ǰ���ַ���
                        tmp = strinput.Substring(i, found - i);
                        //��ӵ������б�
                        tmp3.Add(tmp);
                        i = found + splen - 1;
                    }
                    else
                    {
                        string tmp2 = "";
                        //ȡ�����ַ���
                        tmp2 = strinput.Substring(i);
                        if (tmp2 != "")
                            tmp3.Add(tmp2);
                        break;
                    }
                }

                //����̬�����ά�����ó�ʵ�ʴ��ڵ�Ԫ�ظ�������Ϊ�����б�����16�ı�������ά����
                tmp3.TrimToSize();
                //ת�������б�Ϊ�ַ������飬�����ء�
                rt = (string[])tmp3.ToArray(typeof(String));
                tmp3.Clear();
            }
            catch (Exception e)
            {
                //Console.WriteLine("{0}", e.Message);
                throw e;
            }

            return rt;
        }

        public static string GetFieldDataType(string tblname, string field, SqlConnection Conn)
        {
            System.Data.DataTable tbl = GetDataTable("SELECT * FROM " + tblname, Conn);
            DataColumn col = tbl.Columns[field];
            return col.DataType.ToString();
        }

        public static string GetHostName()
        {
            return System.Net.Dns.GetHostName();
        }
        public static string GetIPAddress()
        {
            //System.Net.IPAddress addr;            
            //addr = new System.Net.IPAddress(System.Net.Dns.GetHostByName(System.Net.Dns.GetHostName()).AddressList[0].Address);
            //addr = new System.Net.IPAddress(ip);
            //return addr.ToString();
            string ip = System.Net.Dns.GetHostEntry(System.Net.Dns.GetHostName()).AddressList[0].ToString();
            return ip;
        }
        public static System.Net.IPAddress[] GetIPAddresses()
        {
            //System.Net.IPAddress addr;            
            //addr = new System.Net.IPAddress(System.Net.Dns.GetHostByName(System.Net.Dns.GetHostName()).AddressList[0].Address);
            //addr = new System.Net.IPAddress(ip);
            //return addr.ToString();
            return System.Net.Dns.GetHostEntry(System.Net.Dns.GetHostName()).AddressList;
        }

        public static string StringArrayToString(object[] strarray)
        {
            string str = "";
            if (strarray != null)
                foreach (object obj in strarray)
                {
                    str += obj + ";";
                }
            return str;
        }


        public static bool SqlUpdateRecord(string tblname, string[] keynames, string[] keys, string[] fields, string[] values, SqlConnection Conn)
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

        public static System.Data.DataTable ExcelToDataTable(string strXLS)
        {
            System.Data.DataTable dtRet = null;
            string strConn = string.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source={0};Extended Properties=Excel 8.0;", strXLS);
            OleDbConnection conn = new OleDbConnection(strConn);
            try
            {
                conn.Open();
                string strExcel = "";
                OleDbDataAdapter myCommand = null;
                strExcel = "select * from [sheet1$]";
                myCommand = new OleDbDataAdapter(strExcel, strConn);
                DataSet ds = new DataSet();
                myCommand.Fill(ds, "table1");
                dtRet = ds.Tables[0];
            }
            catch (Exception ex)
            {
                string err = string.Format("�ڴ�xls�ļ�ʱ��������\r\n\r\n������ϢΪ��{0}", ex.Message);
                MessageBox.Show(err);
            }
            finally
            {
                conn.Close();
            }
            return dtRet;
        }

        // ������Ӽ�¼��Ŀǰֻ֧�ֵ�����
        public static object[] BatchSqlInsertRecord(string tblName, string displayField, string[] newDisplays, SqlConnection Conn)
        {
            // ���DataTable
            SqlDataAdapter adp = new SqlDataAdapter("SELECT * FROM " + tblName, Conn);
            DataSet ds = new DataSet();
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            adp.Fill(ds);
            System.Data.DataTable tbl = ds.Tables[0];

            // ���Ŀǰ���id
            string sql = string.Format("SELECT MAX({0}) FROM {1}", tbl.PrimaryKey[0].ColumnName, tblName);
            System.Data.DataTable dt_maxid = GetDataTable(sql, Conn);
            int iCurMaxId = Convert.ToInt32(dt_maxid.Rows[0][0]);

            // ���Ĭ����
            DataRow defaultRow = tbl.Rows[0];

            // ����������
            object[] IDs_ret = new object[newDisplays.Length];
            for (int i = 0; i < newDisplays.Length; i++)
            {
                DataRow newRow = tbl.NewRow();
                newRow.BeginEdit();
                foreach (DataColumn col in tbl.Columns)
                {
                    newRow[col] = defaultRow[col];
                }
                IDs_ret[i] = ++iCurMaxId;
                newRow[tbl.PrimaryKey[0]] = IDs_ret[i];
                newRow[displayField] = newDisplays[i];
                newRow.EndEdit();
                tbl.Rows.Add(newRow);
            }

            // �ύ
            SqlCommandBuilder cmdBuilder = new SqlCommandBuilder(adp);
            adp.InsertCommand = cmdBuilder.GetInsertCommand();
            int val = adp.Update(tbl);
            tbl.AcceptChanges();

            return IDs_ret;
        }

        public static void CreatePath(string fileName)
        {
            string path = Path.GetDirectoryName(fileName); // ��ȡĿ¼
            try
            {
                if (!Directory.Exists(path)) // Ŀ¼������
                {
                    Directory.CreateDirectory(path);
                }
            }
            catch (IOException ex)
            {
                MessageBox.Show("�ڴ�����Ŀ¼ʱ����IO�쳣: " + ex.ToString());
            }
        }
        public static void StringToFile(string strContent, string strFileName, Encoding encoding)
        {
            if (!File.Exists(strFileName))
            {
                CreatePath(strFileName);
                File.Create(strFileName).Close();
            }
            StreamWriter writer = new StreamWriter(strFileName, false, encoding);
            writer.Write(strContent);
            writer.Close();
        }
        public static string FileToString(string strFileName)
        {
            return FileToString(strFileName, Encoding.GetEncoding("gb2312"));
        }
        public static string FileToString(string strFileName, Encoding encoding)
        {
            StreamReader reader;
            try
            {
                reader = new StreamReader(strFileName, encoding);
            }
            catch //(Exception e)
            {
                //MessageBox.Show(e.Message);
                return "";
            }
            string strContent = reader.ReadToEnd();
            reader.Close();
            return strContent;
        }
        //�ж��ַ����ܷ�ת��Ϊ���֣���������������
        public static bool CanConverToNumber(string ConvertingStr)
        {
            bool bRet = System.Text.RegularExpressions.Regex.IsMatch(ConvertingStr, @"^[+-]?\d+([\.]\d+)?$");
            return bRet;
        }
    }

    class ComboComparer : IComparer
    {
        public int Compare(object o1, object o2)
        {
            return Convert.ToInt32(((DictionaryEntry)o1).Value) - Convert.ToInt32(((DictionaryEntry)o2).Value);
        }
    };

    class StringComparer : IComparer
    {
        public int Compare(object o1, object o2)
        {
            string sK1 = ((DictionaryEntry)o1).Key.ToString().Trim();
            string sK2 = ((DictionaryEntry)o2).Key.ToString().Trim();
            return string.Compare(sK1, sK2);
        }
    };
}
