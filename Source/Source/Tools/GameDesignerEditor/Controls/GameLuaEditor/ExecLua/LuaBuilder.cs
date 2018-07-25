using System;
using System.Collections.Generic;
using System.Text;
using LuaInterface;
using System.Windows.Forms;
namespace ExecLua
{
    class LuaBuilder
    {
        public LuaBuilder(string strTempFileName)
        {
            m_strTempFile = strTempFileName;
            m_strDir = Application.StartupPath;
            m_strIniFile = m_strDir + "\\API.temp.lua";
            m_strExecFile = m_strDir + "\\ExecLua.lua";
            m_strHeaderFile = "LuaHeader.lh";
            m_strFixedCode = "local bReturnCheckerA = false; local bReturnCheckerB = false;  dofile(\"Config.lh\"); dofile(\"" + m_strHeaderFile + "\");";
            m_strCode = "";
            this.Init();
        }

        private void Init()
        {
            string str = "";
            str = FileFolderHelper.FileToString(m_strTempFile);

            str = m_strFixedCode + str + "\r\n";

            //ȥ��ע��  �����ַ������� ��--�� ������
            // �ļ��ַ�������  �ᵼ��ʧ�� �������η�Χ
            for (int nIndexR, nIndexN; str.Length != 0; )
            {
                nIndexR = str.IndexOf("--");
                nIndexN = str.IndexOf("\r\n");
                if (nIndexR < nIndexN && nIndexR >= 0)
                {
                    m_strCode += str.Substring(0, nIndexR) + "\r\n";
                    str = str.Substring(nIndexN + 2, str.Length - nIndexN - 2);
                }
                else if (nIndexR > nIndexN && nIndexN >= 0)
                {
                    m_strCode += str.Substring(0, nIndexN + 2);
                    str = str.Substring(nIndexN + 2, str.Length - nIndexN - 2);
                }
                else if (nIndexR == -1 && nIndexN != -1)
                {
                    m_strCode += str;
                    break;
                }
                else if (nIndexN == -1)
                {
                    break;
                }
                else
                {
                    MessageBox.Show("�쳣���ַ��������쳣��");
                }
           }

           // �ٴα��� ���Check����  ʹ�ű������з�֧���Ա�����

           for (int nIndexP = 0, nIndexE = 0; nIndexE < m_strCode.Length; )
           {

               nIndexP = nIndexE;
               nIndexE = m_strCode.IndexOf("\r\n", nIndexP) + 2;

               str = m_strCode.Substring(nIndexP, nIndexE - nIndexP);

               // �и�ֵ���� ���С�=��        ��һ���ų�         ������==��                        ����"~="              ���� "<"               ����">"                             ����"}"                     ����"{"
               if (str.IndexOf("=") != -1 && nIndexP != 0 && ((str.IndexOf("==") == -1) && (str.IndexOf("~=") == -1) && (str.IndexOf("<") == -1) && (str.IndexOf(">") == -1) && (str.IndexOf("}") == -1) && str.IndexOf("{") == -1))
               {
                   str = str.Substring(0, str.Length - 2);
                   //���еġ�"���ĳɡ�\"�� Դ�������С�\"�����������
                   for (int nIndexF = 0; nIndexF <= str.Length; )
                   {
                       nIndexF = str.IndexOf("\"", nIndexF);
                       if (nIndexF == -1)
                       {
                           break;
                       }
                       str = str.Insert(nIndexF, "\\");
                       nIndexF += 2;
                   }
                   str = str.Trim();
                   str = "CheckVars(\"" + str + "\"); ";
                   m_strCode = m_strCode.Insert(nIndexP, str);
                   nIndexE += str.Length;
               }

               str = m_strCode.Substring(nIndexP, nIndexE - nIndexP);

               // �� elseif XX then �ṹ ��Ϊ end; if XX then
               int n = str.LastIndexOf("elseif");
               if (n != -1)
               {
                   m_strCode = m_strCode.Substring(0, n + nIndexP) + "end;" + m_strCode.Substring(n + nIndexP + 4, m_strCode.Length - n - nIndexP - 4);
               }

               str = m_strCode.Substring(nIndexP, nIndexE - nIndexP);

               //��else ��Ϊend; if true then
               n = str.LastIndexOf("else");
               if (n != -1)
               {
                   m_strCode = m_strCode.Substring(0, n + nIndexP) + "end;if true then" + m_strCode.Substring(n + nIndexP + 4, m_strCode.Length - n - nIndexP - 4);
                   nIndexE += "end;if true then".Length - 4;
               }

               str = m_strCode.Substring(nIndexP, nIndexE - nIndexP);
               // ��if XX then �ṹ ��Ϊ if xx or true then
               int nIndexL = str.LastIndexOf("then");
               if (nIndexL != -1)
               {
                   m_strCode = m_strCode.Insert(nIndexL + nIndexP, "or true ");
                   nIndexE += "or true ".Length;
               }

               str = m_strCode.Substring(nIndexP, nIndexE - nIndexP);

               //��return ��ӡ�--��ע�͵�
               if (str.LastIndexOf("return") != -1)
               {
                   m_strCode = m_strCode.Insert(nIndexP + str.LastIndexOf("return"), "--");
                   nIndexE += 2;
               }
           }


            // д��ExecLua�ļ�
            FileFolderHelper.StringToFile(m_strCode, m_strExecFile);
        }


        private string GetStringFromFile(string strTableName, string strFieldName, string strDefReturn)
        {
            Lua lua = new Lua();
            lua.DoFile(m_strIniFile);
            LuaTable tbl = lua.GetTable(strTableName);
            string strReturn = "";
            try
            {
                strReturn = (string)tbl[strFieldName];
            }
            catch (Exception e)
            {
                return strDefReturn;
            }
            return strReturn == null ? strDefReturn : strReturn;
        }


        public void Run()
        {
            Lua lua = new Lua();
            try
            {
                lua.DoFile(m_strExecFile);
            }
            catch (Exception Ex)
            {
                Console.WriteLine(Ex.Message);
                return;
            }
            LuaTable luaTable = lua.GetTable("EVENT_LIST");
            foreach (object Key in luaTable.Keys)
            {
                string strEventFunction = (string)luaTable[Key];
                //���������ʽΪ   ��param1, param2, param3, ��
                int nIndexP = strEventFunction.IndexOf("(") + 1;
                int nIndexE = strEventFunction.IndexOf(")");
                string strFunctionName = strEventFunction.Substring(0, nIndexP - 1);
                string strParam = strEventFunction.Substring(nIndexP, nIndexE - nIndexP) + ", ";
                nIndexP = 0;
                nIndexE = 0;
                strFunctionName = strFunctionName + "(";
                while (true)
                {
                    nIndexP = nIndexE;
                    nIndexE = strParam.IndexOf(", ", nIndexP) + 2;
                    if (nIndexE == 1)
                    {
                        break;
                    }
                    string strParamName = strParam.Substring(nIndexP, nIndexE - 2 - nIndexP);
                    string sb = GetStringFromFile("GetParam", strParamName, "nil");
                    if (sb == "nil")
                    {
                        MessageBox.Show("�����ļ����޷��ҵ��߼������������" + strParamName);
                    }
                    strFunctionName = strFunctionName + sb + ", ";
                }
                if (strEventFunction.IndexOf("(") + 1 != strEventFunction.IndexOf(")"))
                {
                    strFunctionName = strFunctionName.Substring(0, strFunctionName.Length - 2);
                }
                strFunctionName = strFunctionName + ")";
                try
                {
                     lua.DoString(strFunctionName);
                }
                catch (Exception Ex)
                {   
                    //������Ϊ�����˲����ڵ�Event����Ĵ���
                    string strFunc = strFunctionName.Substring(0, strFunctionName.IndexOf("("));
                    int nIndexEx = Ex.Message.IndexOf(strFunc);
                    if (nIndexEx == -1)
                    {
                        Console.WriteLine(Ex.Message);
                    }
                }
            }
        }
        string m_strTempFile;
        string m_strExecFile;
        string m_strDir;
        string m_strFixedCode;
        string m_strCode;
        string m_strHeaderFile;
        string m_strIniFile;
    }
}
