using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Text;

namespace IniFileIO
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
                StreamReader r = File.OpenText(fileName);
                fileContents = r.ReadToEnd();
                r.Close();
            }   
        }

        /// <summary>
        /// ������������
        /// </summary>
        public string[] Fields
        {
            get
            {
                return fields;
            }
        }

        /// <summary>
        /// ��������������
        /// </summary>
        public string[] Values
        {
            get
            {
                return values;
            }
        }

        /// <summary>
        /// ��������·�����ļ���ת��Ϊ����·�����ļ���
        /// </summary>
        /// <param name="fileName">������·�����ļ���</param>
        /// <returns>����·�����ļ���</returns>
        private string ConvertFileName(string fileName)
        {
            int index = fileName.LastIndexOf('\\');
            return fileName.Substring(index + 1);
        }

        /// <summary>
        /// �������
        /// </summary>
        public void Fill()
        {
            try
            {
                FillFields();
                FillValues();
            }
            catch(Exception ex)
            {
                MessageBox.Show("ini�ı�ת��ʱ�����쳣: " + ex.ToString());
            }
        }

        /// <summary>
        /// �����������
        /// </summary>
        private void FillFields()
        {
            sectionNames = SectionNames;
            values = new string[sectionNames.Length];
            sectionKeys1 = GetKeys(sectionNames[0]);
            sectionKeys2 = GetKeys(sectionNames[1]);
            fields = new string[2 + sectionKeys1.Length + sectionKeys2.Length];
            fields[0] = ("filename");
            fields[1] = ("label");
            int index = 2;
            foreach(string s in sectionKeys1)
            {
                fields[index] = s;
                index++;
            }
            foreach(string s in sectionKeys2)
            {
                fields[index] = s;
                index++;
            }
        }

        /// <summary>
        /// �����ֵ����
        /// </summary>
        private void FillValues()
        {
            string value = ConvertFileName(fileName) + "\t-1\t";
            foreach(string s in sectionKeys1)
            {
                value = value + GetKeyString(sectionNames[0] , s) + "\t";
            }
            foreach(string s in sectionKeys2)
            {
                value = value + "-1\t";
            }
            values[0] = value;

            for(int i = 1; i < values.Length; i++)
            {
                value = ConvertFileName(fileName) + "\t";
                value = value + sectionNames[i] + "\t";
                foreach (string s in sectionKeys1)
                {
                    value = value + "\t";
                }
                foreach (string s in sectionKeys2)
                {
                    value = value + GetKeyString(sectionNames[i], s) + "\t";
                }
                values[i] = value;
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
        private string GetKeyString(string sectionName, string keyName)
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
