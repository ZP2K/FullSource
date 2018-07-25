using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Text;

namespace PatrolIO
{
    class ImportData
    {
        private string fileName; // �ļ���
        private string[] sectionNames; // ������
        private string[] sectionKeys1; // ��������1
        private string[] sectionKeys2; // ��������2
        private string fileContents = null; // �ı�����
        private string[] fields; // ��������
        private string[] values; // ����������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        public ImportData(string fileName)
        {
            this.fileName = fileName;
            if (File.Exists(fileName))
            {
                //StreamReader r = File.OpenText(fileName);
                StreamReader r = new StreamReader(fileName, Encoding.GetEncoding("gb2312"));
                fileContents = r.ReadToEnd();
                r.Close();
            }   
        }     

        /// <summary>   
        /// Get section names in an array, Read only   
        /// </summary>   
        public string[] SectionNames
        {
            get
            {
                // Using regular expression to get all section names.   
                string regexPattern = @"\[(?<SectionName>\w*(\w|\s)*)\]";

                Regex r = new Regex(regexPattern); // Match "[anywords]"   
                MatchCollection matches = r.Matches(fileContents);

                // Writing all section names to a string array.   
                string[] results = new string[matches.Count];

                for (int i = 0; i < matches.Count; i++)
                {
                    results[i] = matches[i].Result("${SectionName}");
                }
                return results;
            }
        }

        /// <summary>   
        /// Get the whole string in one section   
        /// </summary>   
        /// <param name="sectionName">��������</param>   
        /// <returns>��������</returns>   
        private string GetSectionString(string sectionName)
        {
            //string regexPattern = @"(\[" + sectionName + @"\]"
              //+ @"(?<SectionString>.*)\[)";
            string regexPattern = @"(\[" + sectionName + @"\]"
              + @"(?<SectionString>.*?)\r\n\r\n)";

            Regex r = new Regex(regexPattern, RegexOptions.Singleline);
            //Regex r = new Regex(regexPattern);
            
            if (r.IsMatch(fileContents))
            {
                return r.Match(fileContents).Result("${SectionString}");
            }
            return string.Empty;
        }

        /// <summary>   
        /// Get the string value of certain key in certain section   
        /// </summary>   
        /// <param name="sectionName">��������</param>   
        /// <param name="keyName">��������</param>   
        /// <returns>����ֵ</returns>   
        public string GetKeyString(string sectionName, string keyName)
        {
            string sectionString = this.GetSectionString(sectionName) + "\r\n";
            string regexPattern = @"(" + keyName + @"=(?<value>.*)\r\n)";

            Regex r = new Regex(regexPattern);
            if (r.IsMatch(sectionString))
            {
                return r.Match(sectionString).Result("${value}");
            }

            return string.Empty;
        } 
  
        /// <summary>
        /// ��ȡ�ض������µ�����������
        /// </summary>
        /// <param name="sectionName">��������</param>
        /// <returns>����������</returns>
        private string[] GetKeys(string sectionName)
        {
            string[] keys;
            string sectionString = this.GetSectionString(sectionName) + "\r\n";
            string regexPattern = @"(" + "(?<value>.*)" + @"=(\w+)\r\n)";
            Regex r = new Regex(regexPattern);
            MatchCollection matches = r.Matches(sectionString);

            keys = new string[matches.Count];
            for (int i = 0; i < matches.Count; i++)
            {
                keys[i] = matches[i].Result("${value}");
            }
            return keys;
        }
    }
}
