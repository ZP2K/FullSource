/* -----------------------------------------------------------------------*\
 * �ļ���     :  Functions.cs
 * ������     :  κΰ��
 * ����ʱ��   :  2002-1-22 13:49:21
 * ��������   :  ���ܺ������������ߺ�����
 * ע������   :  
 * -----------------------------------------------------------------------*/

using System;
using System.Data ;
using System.Data.SqlClient;
using System.IO;

namespace XSJ.Sword3.Editor.Studio
{
    /// <summary>
    /// XMLת�庯����
    /// </summary>
    public class XMLFunctionLib
    {
        /// <summary>
        /// ��ΪFullDescription�ֶδ洢�ĸ�ʽ��һ��������XML�ĵ�,����Ҫ��"<>"ת��洢.
        /// </summary>
        /// <param name="oristring">Ҫת���Դ�ַ���</param>
        /// <returns>����ת�����ַ���</returns>
        static public string TransformXMLCharacters(string oristring)
        {
            string retstring = string.Empty;
            retstring = oristring.Replace("&", "&amp;");
            retstring = retstring.Replace("<", @"&lt;");
            retstring = retstring.Replace(">", @"&gt;");
            return retstring;
        }
        /// <summary>
        /// ���Ѿ���ת�����XML��Ƿ���ԭ
        /// </summary>
        /// <param name="transformedString">��ת������ַ���</param>
        /// <returns>���ػ�ԭ���ַ���</returns>
        static public string UnTransformXMLCharacters(string transformedString)
        {
            string retstring = string.Empty;
            retstring = transformedString.Replace(@"&amp;", "&");
            retstring = retstring.Replace(@"&lt;", "<");
            retstring = retstring.Replace(@"&gt;", ">");
            return retstring;
        }
    }
    public class HtmlFunctionLib
    {
        /// <summary>
        /// �����л��з�ת��Ϊ"<BR>"�����
        /// </summary>
        /// <param name="inStr">������ַ���</param>
        /// <returns>���ؽ��</returns>
        static public string OutputHTML(string inStr)
        {
            string tmpstr;
            tmpstr = inStr.Replace("\n", "<BR>");
            tmpstr = tmpstr.Replace(" ", "&nbsp;");
            return tmpstr;
        }
        /// <summary>
        /// ��IE���ڵ���ʽ
        /// </summary>
        public class IEWinStyle
        {
            public const string noToolbar = "'toolbar=no,location=no,directories=no,status=no,scrollbars=yes,resizable=yes,copyhistory=no,width=600,height=400'";
            public const string noToolbar2 = "'toolbar=no,location=no,directories=no,menubar=yes,titlebar=no,status=yes,scrollbars=yes,resizable=yes,copyhistory=no'";
        }
        /// <summary>
        /// ��ȡ�ַ���
        /// </summary>
        /// <param name="inStr">Դ�ַ���</param>
        /// <param name="length">��ȡ����</param>
        /// <returns>���ؽ�ȡ����ַ���</returns>
        static public string SubString(string inStr, int length)
        {
            string _retStr = "";
            if (inStr.Length > 0)
            {
                try
                {
                    _retStr = inStr.Substring(0, length);
                    _retStr += "...";
                    _retStr = "<ACRONYM title='" + inStr + "'>" + _retStr + "</ACRONYM>";
                }
                catch
                {
                    _retStr = inStr;
                }
            }
            return _retStr;
        }
        /// <summary>
        /// ��ָ�����ı����Ƚ�������(��ʧ��!ʧ����Ϊ1%,���20���ַ���ʧ)
        /// </summary>
        /// <param name="inStr">Դ�ַ���</param>
        /// <param name="length">ÿ���ַ�����</param>
        /// <returns>���ش������ַ���</returns>
        static public string WrapText(string inStr, int length)
        {
            string _OrginalStr = inStr;
            string _retStr = "";///ת����������ַ���
            string[] _tmpStr;
            int _inStrLen = inStr.Length;
            int _fragment = 0;
            int _basePtr = 0;
            if (inStr.Length < length) { return inStr; }
            if (inStr.Length > 0)
            {
                try
                {
                    _fragment = _inStrLen / length;
                    _tmpStr = new String[_fragment];
                    ///��������ɶ�!
                    for (int i = 0; i < _fragment; i++)
                    {
                        _tmpStr[i] = inStr.Substring(_basePtr, length);
                        ///��ÿ�κ��"<BR>",��HTML���з�
                        _tmpStr[i] += "<BR>";
                        _retStr += _tmpStr[i];
                        _basePtr += length + 1;
                    }
                }
                catch
                {
                    //_retStr = _OrginalStr;
                }
            }
            return _retStr;
        }
    }
    //=============================����Ϊһ�㺯����=============================================
    /// <summary>
    /// һ��ĺ�����.
    /// </summary>
    public class Functions
    {
        /// <summary>
        /// ���ܺ���
        /// </summary>
        public Functions()
        {
            //
            // TODO: Add constructor logic here
            //
        }
        /// <summary>
        /// Creates a temporary password of a specified length.
        /// </summary>
        /// <param name="length">The maximum length of the temporary password to create.</param>
        /// <returns>A temporary password less than or equal to the length specified.</returns>
        public static String CreateTemporaryPassword(int length)
        {
            string strTempPassword = Guid.NewGuid().ToString("N");
            for (int i = 0; i < (length / 32); i++)
            {
                strTempPassword += Guid.NewGuid().ToString("N");
            }
            return strTempPassword.Substring(0, length);
        }

        /// <summary>
        /// �ӱ��ֶζ�ȡ�������ַ������ܺ���(˫����),��Ҫ����ת���C#�ĸ�ʽ.
        /// </summary>
        /// <param name="inStr">��Ҫת����ַ���</param>
        /// <returns>����ת�����ַ���</returns>
        static public string ConvertStringForCSharp(string inStr)
        {
            string strtmp = inStr;
            if (inStr.Length > 0)
            {
                strtmp = inStr.Replace("'", "\'");
                strtmp = strtmp.Replace((char)0x3D, (char)0xA3A2);
                strtmp = strtmp.Replace("\"", "\"");
            }
            return strtmp;
        }
        /// <summary>
        /// Ϊ�˱����û�����"'"(������),��(˫����),��ΪSQL SERVER�����,�����轫��ת��.
        /// </summary>
        /// <param name="inStr">��Ҫת����ַ���</param>
        /// <returns>����ת�����ַ���</returns>
        static public string ConvertStringForDB(string inStr)
        {
            string strtmp = inStr;
            if (inStr.Length > -1)
            {
                strtmp = strtmp.Replace("'", "''");
                strtmp = strtmp.Replace("\"", "\"");
            }
            return strtmp;
        }
        /// <summary>
        /// ���˷Ƿ��ַ�,��Ҫ�����û���¼
        /// </summary>
        /// <param name="inStr"></param>
        /// <returns></returns>
        static public string VerifyString2(string inStr)
        {
            string strtmp = "";
            strtmp = inStr.Replace("'", "");
            strtmp = strtmp.Replace(" ", "");
            strtmp = strtmp.Replace("@", "");
            strtmp = strtmp.Replace("#", "");
            strtmp = strtmp.Replace("!", "");
            strtmp = strtmp.Replace("&", "");
            strtmp = strtmp.Replace("^", "");
            strtmp = strtmp.Replace("$", "");
            strtmp = strtmp.Replace("+", "");
            strtmp = strtmp.Replace("?", "");
            strtmp = strtmp.Replace("/", "");
            strtmp = strtmp.Replace("\\", "");
            strtmp = strtmp.Replace((char)0x22, (char)'��');
            return strtmp;
        }
    }


    /// <summary>
    /// ��ҳ����
    /// </summary>
    public enum PageAction
    {
        NextPage = 1, PreviousPage, FirstPage, LastPage
    }
    /// <summary>
    /// ��������
    /// </summary>
    public class Operater
    {
        /// <summary>
        /// ������Ľṹ
        /// </summary>
        public struct TOperater
        {
            public string Name;
            public string Value;
        }
        /// <summary>
        /// �߼������
        /// </summary>
        public struct TLogic
        {
            public string Name;
            public string Value;
        }
        /// <summary>
        /// ��ȡ�����ֶ�����Ĳ�����
        /// </summary>
        /// <returns>�������в�����</returns>
        public TOperater[] GetOperaters()
        {
            TOperater[] _ops = new TOperater[8];
            _ops[0].Name = "����";
            _ops[0].Value = "=";
            _ops[1].Name = "����";
            _ops[1].Value = ">";
            _ops[2].Name = "С��";
            _ops[2].Value = "<";
            _ops[3].Name = "������";
            _ops[3].Value = "<>";
            _ops[4].Name = "���ڵ���";
            _ops[4].Value = ">=";
            _ops[5].Name = "С�ڵ���";
            _ops[5].Value = "<=";
            _ops[6].Name = "������";
            _ops[6].Value = "like";
            _ops[7].Name = "��������";
            _ops[7].Value = "not like";
            return _ops;
        }
        public TLogic[] GetLogicOperaters()
        {
            TLogic[] _lgs = new TLogic[2];
            _lgs[0].Name = "����";
            _lgs[0].Value = " and ";
            _lgs[1].Name = "����";
            _lgs[1].Value = " or ";
            return _lgs;
        }
    }
    /// <summary>
    /// Summary description for TypeConverter.
    /// </summary>
    public class TypeConverter
    {
        public TypeConverter()
        {
            //
            // TODO: Add constructor logic here
            //
        }
        static public bool IsNumberic(char chr)
        {
            switch (chr.ToString())
            {
                case "1":
                case "2":
                case "3":
                case "4":
                case "5":
                case "6":
                case "7":
                case "8":
                case "9":
                case "0":
                    return true;

            }
            return false;
        }
        static public int HexToInteger(string HexString)
        {
            return System.Convert.ToInt32(HexString, 16);
        }
        static public string IntegerToHex(int IntValue, int length)
        {
            return IntValue.ToString("X" + length.ToString());
        }
    }
}

