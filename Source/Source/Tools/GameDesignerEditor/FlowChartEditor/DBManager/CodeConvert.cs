using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Data;

namespace DBManager
{
    public class CodeConvert
    {
        private static CodeConvert codeConvert; // �ı�ת����ʵ��
        private string convertTableName; // ��Ҫת���ı���

        /// <summary>
        /// ˽�й��캯��
        /// </summary>
        private CodeConvert()
        {

        }

        /// <summary>
        /// ��Ҫת���ı���
        /// </summary>
        public string ConvertTableName
        {
            set
            {
                convertTableName = value;
            }

            get
            {
                return convertTableName;
            }
        }

        /// <summary>
        /// ��ȡ�ı�ת����ʵ��
        /// </summary>
        public static CodeConvert GetCodeConvert()
        {
            if (codeConvert == null)
            {
                codeConvert = new CodeConvert();
            }

            return codeConvert;
        }

        /// <summary>
        /// ���룺��SCENE_STRING_TABLE['123']��ɺ���
        /// </summary>
        /// <param name="strCode">Դ����</param>
        /// <param name="ls_ht">ls�� KeyΪ�����ַ�����ValueΪ�����ַ���</param>
        /// <returns>�����ĵĴ���</returns>
        public string Decode(string strCode, Hashtable ls_ht)
        {
            string tableString = convertTableName + "[";
            string[] asp = { tableString };
            string[] as_str = strCode.Split(asp, StringSplitOptions.None);

            for (int i = 1; i < as_str.Length; i++)
            {
                if ((as_str[i].IndexOf("]")) == -1) continue;
                string strKey = as_str[i].Substring(0, as_str[i].IndexOf("]"));
                if (!ls_ht.ContainsKey(strKey))
                {
                    continue;
                }
                strCode = strCode.Replace(tableString + strKey.Trim() + "]", ls_ht[strKey].ToString());
            }

            return strCode;
        }

        /// <summary>
        /// ��ȡ���������ַ�����Ϣ��hashtable
        /// </summary>
        /// <param name="strCode">ԭ�ַ���</param>
        /// <returns>���������ַ�����Ϣ��hashtable</returns>
        private Hashtable GetSubstringHashtable(string strCode)
        {
            DFATable dfa = new DFATable();

            return dfa.getStringList(strCode);
        }

        /// <summary>
        /// �жϽű��Ƿ���Ҫls�ļ�
        /// </summary>
        /// <param name="content">�ű�����</param>
        /// <returns>�Ƿ���Ҫls�ļ�</returns>
        public static bool RequireLsFile(string content)
        {
            return content.Contains("SCENE_STRING_TABLE[");
        }

        /// <summary>
        /// ��ls������ת��Ϊls�ļ��е��ַ���
        /// </summary>
        /// <param name="lsRows"></param>
        /// <returns></returns>
        public static string ConvertLsFileContent(DataRow[] lsRows)
        {
            string data = "Include('scripts/Include/String.lh')\r\nSCENE_STRING_TABLE = {\r\n";
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
        /// ���룺�Ѵ����ĵĸ���ɲ������ĵ�
        /// </summary>
        /// <param name="strcode">�����ĵ�Դ����</param>
        /// <param name="ht_value">������ls�е�key�Ķ�Ӧ��Key: �����ַ��� Value: �����ַ���</param>
        /// <returns>�������ġ��������ĵ�Դ����</returns>
        private string EncodeString(string strcode, Hashtable ht_value)
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
                result = tempStr.Insert(index, string.Format("{0}[{1}]", convertTableName, ht_value[content]));
            }

            return result;
        }

        /// <summary>
        /// ���Ҵ��������������ַ���
        /// </summary>
        /// <param name="strCode">���������ַ����Ĵ���</param>
        /// <returns>�����ַ����б�</returns>
        public List<string> GetStringList(string strCode)
        {
            List<string> list = new List<string>();
            DFATable dfa = new DFATable();
            Hashtable table = dfa.getStringList(strCode);
            foreach (string s in table.Values)
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
        /// <returns>�������ĵı���</returns>
        public string Encode(string strcode, Hashtable ht_value)
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
                result = tempStr.Insert(index, string.Format("{0}[{1}]", convertTableName, ht_value[content]));
            }

            return result;
        }
    }
}
