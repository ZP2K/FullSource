using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Data;
using luaEditor;

namespace DataBaseIO
{
    public static class CodeConvert
    {
        /// <summary>
        /// ���룺�Ѳ������ĵĸ���ɴ����ĵ�
        /// </summary>
        /// <param name="strCode">Դ����</param>
        /// <param name="ls_ht">ls�� KeyΪ�����ַ�����ValueΪ�����ַ���</param>
        /// <param name="tableName">�ַ�������</param>
        /// <returns>�����ĵĴ���</returns>
        public static string Decode(string strCode, Hashtable ls_ht, string tableName)
        {            
            string[] as_str = strCode.Split(new string[] { string.Format("{0}[", tableName) }, StringSplitOptions.None);
            
            for (int i = 1; i < as_str.Length; i++)
            {
                if ((as_str[i].IndexOf("]")) == -1)
                {
                    continue;
                }

                string strKey = as_str[i].Substring(0, as_str[i].IndexOf("]"));

                if (!ls_ht.ContainsKey(strKey))
                {
                    continue;
                }

                strCode = strCode.Replace(string.Format("{0}[{1}]", tableName, strKey.Trim()), ls_ht[strKey].ToString());
            }

            return strCode;
        }

        /// <summary>
        /// ��ȡ���������ַ�����Ϣ��hashtable
        /// </summary>
        /// <param name="strCode">ԭ�ַ���</param>
        /// <returns>���������ַ�����Ϣ��hashtable</returns>
        private static Hashtable GetSubstringHashtable(string strCode)
        {
            GameLuaEditor.DFATable dfa = new GameLuaEditor.DFATable();

            return dfa.getStringList(strCode);
        }

        /// <summary>
        /// ��ls������ת��Ϊls�ļ��е��ַ���
        /// </summary>
        /// <param name="lsRows">ls����</param>
        /// <returns>ls�ı�����</returns>
        public static string ConvertLsFileContent(DataRow[] lsRows, string tableName)
        {
            string data = string.Format("Include('scripts/Include/String.lh')\r\n{0} = {{\r\n", tableName);
            string content;
            string id;

            foreach (DataRow r in lsRows)
            {
                id = r["id"].ToString();
                content = r["content"].ToString();
                data = data + string.Format("    [{0}] = {1},\r\n", id, content);
            }

            data = data.Substring(0, data.Length - 3) + "\r\n";
            data = data + "}";

            return data;
        }

        /// <summary>
        /// ���Ҵ��������������ַ���
        /// </summary>
        /// <param name="strCode">���������ַ����Ĵ���</param>
        /// <returns>�����ַ����б�</returns>
        public static List<string> GetStringList(string strCode)
        {
            List<string> list = new List<string>();
            GameLuaEditor.DFATable dfa = new GameLuaEditor.DFATable();
            Hashtable table = dfa.getStringList(strCode);
            foreach(string s in table.Values)
            {
                list.Add(s);
            }

            return list;
        }

        /// <summary>
        /// ���룺�Ѵ����ĵĸ���ɲ������ĵ�
        /// </summary>
        /// <param name="strcode">�����ĵı���</param>
        /// <param name="ht_value">����hash��</param>
        /// <param name="tableName">�ַ����������</param>
        /// <returns>�������ĵı���</returns>
        public static string Encode(string strcode, Hashtable ht_value, string tableName)
        {
            string result = strcode;

            Hashtable table = GetSubstringHashtable(result);
            List<int> keyList = new List<int>();

            foreach (string s in table.Keys)
            {
                keyList.Add(int.Parse(s));
            }
            keyList.Sort();

            for (int i = keyList.Count - 1; i >= 0; i--)
            {
                int index = keyList[i];
                string content = table[index.ToString()] as string;
                string tempStr = result.Remove(index, content.Length);
                result = tempStr.Insert(index, string.Format("{0}[{1}]", tableName, ht_value[content]));
            }

            return result;
        }
    }
}

