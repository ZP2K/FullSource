using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace QueryContextData
{
    public class IniIO
    {
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

        public static void IniWriteValue(string Section, string Key, string Value, string filepath)    //��ini�ļ�����д�����ĺ���
        {
            long i = 0;
            i = WritePrivateProfileString(Section, Key, Value, filepath);
        }

        public static string IniReadValue(string Section, string Key, string filepath)                 //��ini�ļ����ж������ĺ���
        {
            StringBuilder temp = new StringBuilder(255);
            int i = GetPrivateProfileString(Section, Key, "", temp, 255, filepath);
            return temp.ToString();
        }
    }
}
