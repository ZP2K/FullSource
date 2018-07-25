using System;
using System.Collections.Generic;
using System.Text;

namespace MakeIDL
{
    enum NoteType
    {
        NONE,
        OLD,
        CANNOT
    }
    class KTransferOneFunction
    {
        private int nSameFuncNameCnt = 1;
        private string strOldParas = "";
        private NoteType noteType = NoteType.NONE;
        private string strParasResult;
        private string m_strFuncName;
        private string m_strReturn;
        private string m_strParas;
        private string m_strAllfuc;
        public KTransferOneFunction(string strAll)
        {
            m_strAllfuc = strAll;
            m_strFuncName = "";
            m_strParas = "";
            m_strReturn = "";
            Init();

        }
        public KTransferOneFunction()
        {
            m_strAllfuc = "";
            m_strFuncName = "";
            m_strParas = "";
            m_strReturn = "";

        }
        public void ReInit(string strAll)
        {
            noteType = NoteType.NONE;
            m_strAllfuc = strAll;
            m_strFuncName = "";
            m_strParas = "";
            m_strReturn = "";
            Init();            
        }
        public string GetFuncName()
        {
            return m_strFuncName;
        }
        public string GetParas()
        {
            return m_strParas;
        }
        protected void Init()
        {
            m_strAllfuc = m_strAllfuc.Replace('\t', ' ');//
            SetStrParas();
            SetStrReturn();
            SetFuncName();
        }

        protected void SetStrReturn()
        {
            int nBegin = m_strAllfuc.IndexOf(' ', 0);
            int nEnd = m_strAllfuc.IndexOf(' ', nBegin + 1);
            string str = m_strAllfuc.Substring(nBegin, nEnd - nBegin);
            m_strReturn = str.Trim();
        }

        protected void SetStrParas()
        {
            int nBegin = -1;
            int nEnd = -1;
            nBegin = m_strAllfuc.IndexOf('(', 0) + 1;
            nEnd = m_strAllfuc.LastIndexOf(')', m_strAllfuc.Length - 1);
            m_strParas = m_strAllfuc.Substring(nBegin, nEnd - nBegin);
            string strTrim = " \t\r\n";
            m_strParas = m_strParas.Trim(strTrim.ToCharArray());
        }

        protected void SetFuncName()
        {
            int nBegin = -1;
            int nEnd = -1;
            nEnd = m_strAllfuc.IndexOf('(', 0);
            nBegin = m_strAllfuc.LastIndexOf(' ', nEnd, nEnd);
            m_strFuncName = m_strAllfuc.Substring(nBegin + 1, nEnd - nBegin - 1);
            string strTrim = " \t\r\n";
            m_strFuncName = m_strFuncName.Trim(strTrim.ToCharArray());
            
        }

        public void SetOldParas(string strIdlParas)
        {
            string strTrim = " \t\r\n";
            strOldParas = strIdlParas.Trim(strTrim.ToCharArray());
        }

        /// <summary>
        /// �Ƚ������ɵĲ�����idl��ɲ���������ע�ͺ����ղ���
        /// </summary>
        protected void ModifyParasResult()
        {
           strParasResult = AnalyseParameters();
           if (strParasResult == "void" && strOldParas != "" && strOldParas != "void")
            {
               strParasResult = strOldParas;
               noteType = NoteType.OLD;                        
            }                       
        }
        /// <summary>
        /// ����ע�����ͻ�ȡע��
        /// </summary>
        /// <returns></returns>
        protected string GetNote()
        {
            switch (noteType)
            {
                case NoteType.CANNOT:
                    return "//��δ֪���������ͣ�û��ת��";
                case NoteType.NONE:
                    return "";
                case NoteType.OLD:
                    return "//����ԭ��idl��ɵĲ���";
                default:
                    return "";
            }
        }

        /// <summary>
        /// �������ͬ������Ӧ���޸ĺ�����
        /// </summary>
        /// <param name="nSameNameCnt"></param>
        public void ModifyFuncName(int nSameNameCnt)
        {
            if (nSameNameCnt >= 2)
            {
                m_strFuncName = string.Format("{0}{1}", m_strFuncName, nSameNameCnt);
            }
        }

        public int IncreaseSameNameCnt()
        {
            nSameFuncNameCnt++;
            return nSameFuncNameCnt;
        }
        public int GetSameNameCnt()
        {
            return nSameFuncNameCnt;
        }
        public string GetIdlResult()
        {
            ModifyParasResult();
            string strIdlResult = "";           
            string strHelp = "";
            string strNote = "";
            strNote = GetNote();
            strHelp = string.Format("[helpstring(\"method {0}\")] HRESULT ", m_strFuncName);
            strIdlResult = string.Format("{0}{1}({2});{3}", strHelp, m_strFuncName, strParasResult,strNote);
            return strIdlResult;
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <returns>idl�Ĳ���</returns>
        protected string AnalyseParameters()
        {
            string strRtnPara = "";
            string strRtnAll = "";
            strRtnPara = AnalyseReturn();            
            if (m_strParas == "")
            {
                if (strRtnPara == "")
                {
                    return "void";
                }
                return strRtnPara;
            }

            int nBegin = 0;
            int nEnd = -1;
            string strOnePara = "";
            string strOneParaResult = "";
            nEnd = m_strParas.IndexOf(',', 0);
            while (nEnd != -1)
            {
                strOnePara = m_strParas.Substring(nBegin, nEnd - nBegin);
                strOneParaResult = AnalyseOneParameter(strOnePara);
                if (strOneParaResult == "void")
                {
                    noteType = NoteType.CANNOT;
                    return "void";
                }
                strRtnAll += strOneParaResult + ", ";
                nBegin = nEnd + 1;
                nEnd = m_strParas.IndexOf(',', nBegin);
            }
            strOnePara = m_strParas.Substring(nBegin, m_strParas.Length - nBegin);
            strOneParaResult = AnalyseOneParameter(strOnePara);
            if (strOneParaResult == "void")
            {
                noteType = NoteType.CANNOT;
                return "void";
            }
            strRtnAll += strOneParaResult;
            if (strRtnAll == "")
            {
                strRtnAll = "void";
            }
            return strRtnAll;

        }

        /// <summary>
        /// ��������ֵ
        /// </summary>
        /// <returns>������ʾΪvoid�� ��void����ʾ����������Ϊvoid</returns>
        protected string AnalyseReturn()
        {
            int nStarCnt = GetCharCnt('*', m_strReturn);
            string strTrim = "  *";
            string strTemp = m_strReturn.Trim(strTrim.ToCharArray());

            int nTypeType = 0;
            
            string strResult = GetTransferType(strTemp, ref nTypeType);
            if (nTypeType == -1)
            {
                return "[out,retval] LONG* Result";
            }
            else if (nTypeType == 4)//void����
            {
                if (nStarCnt == 0)//voidʱ���ؿ�
                {
                    return "";
                }
                else   //void* void**
                {
                    nStarCnt--;
                }
            }
            else if (nTypeType == 3 && strTemp == "BYTE")
            {
                if (nStarCnt > 0)
                {
                    nStarCnt--;
                }
            }            
            
            if (strResult != "")// ����HRESULT����
            {                
                nStarCnt++;
                string strStar = new string('*', nStarCnt);
                strResult = "[out,retval] " + strResult + strStar + " Result";
            }
            return strResult;

        }
        /// <summary>
        /// ����һ������
        /// </summary>
        /// <param name="strOnePara"></param>
        /// <returns></returns>
        protected string AnalyseOneParameter(string strOnePara)
        {
            const string strIn = "[in] ";
            const string strOut = "[out] ";
            string strInOut = "";
            int nStarCnt = -1;
            string strParaName = "";
            string strTypeName = "";
            string strTypeResult = "";
            int nTypeType = 0;//1:�Զ������� 2:�������� 3:��������

            int nIndex = -1;
            nIndex = strOnePara.IndexOf("const", 0);
            if (nIndex != -1)
            {
                strInOut = strIn;
                strOnePara = strOnePara.Replace("const", "");//ɾ��const
            }
            if ((nIndex = strOnePara.IndexOf('&', 0)) != -1)
            {               
                strOnePara = strOnePara.Replace('&', '*');                
            }
            
            nIndex = strOnePara.LastIndexOf(' ');
            strTypeName = strOnePara.Substring(0, nIndex);
            strParaName = strOnePara.Substring(nIndex);
            string strTrim = " *    ";
            strTypeName = strTypeName.Trim(strTrim.ToCharArray());//��ȡ��������
            strParaName = strParaName.Trim(strTrim.ToCharArray());//��ȡ��������


            strTypeResult = GetTransferType(strTypeName, ref nTypeType); 
            
            nStarCnt = GetCharCnt('*', strOnePara);            
            if (nTypeType == 1)
            {
                if (strInOut == "" && nStarCnt == 1)
                {
                    strInOut = strIn;
                }
                else if (strInOut == "" && nStarCnt == 2)
                {
                    strInOut = strOut;
                }
               
            }
            else if (nTypeType == 2)
            {
                if (strInOut == "" && nStarCnt == 0)
                {
                    strInOut = strIn;
                }
                else if (strInOut == "" && nStarCnt == 1)
                {
                    strInOut = strOut;
                }
                
            }
            else if (nTypeType == 3)
            {
                if (strTypeResult == "UCHAR")//
                {
                    strInOut = strIn;
                    nStarCnt = 0;
                    strParaName.Replace("[]", "");
                    strParaName += "[MAX_PATH]";
                }
                else if (strTypeName == "BYTE")
                {
                    if (nStarCnt > 0) 
                    {
                        nStarCnt--;
                    }

                    if (strInOut == "" && nStarCnt == 0)
                    {
                        strInOut = strIn;
                    }
                    else if (strInOut == "" && nStarCnt == 1)
                    {
                        strInOut = strOut;
                    }
                } 
            }
            else if (nTypeType == 4)//void
            {
                if (nStarCnt > 0)
                {
                    nStarCnt--;
                }
            }
            else
            {
                return "void";
            }
            string strSatr = new string('*', nStarCnt);
            strTypeResult = strTypeResult + strSatr;

            string strAllResult = strTypeResult + " " + strParaName;

            return strAllResult;

        }

        /// <summary>
        /// ��ȡ�ַ�����ĳ���ַ��ĸ���
        /// </summary>
        /// <param name="c"></param>
        /// <param name="strCnt"></param>
        /// <returns></returns>
        protected int GetCharCnt(char c, string strCnt)
        {
            int n = 0;
            int nCnt = 0;
            for (n = 0; n < strCnt.Length; n++)
            {
                if (strCnt[n] == c)
                {
                    nCnt++;
                }
            }
            return nCnt;
        }


        protected string GetTransferType(string strParaType, ref int nTypeType)
        {
            //nTypeType���� 1;��ʾ�Զ������� 2:�������� 3:�������� 4; void����
            if (IsSelfType(strParaType))
            {
                nTypeType = 1;
                return "IAtl" + strParaType;
            }
            switch (strParaType)
            {
                case "IEKG3DSceneEditorBase":
                case "IKG3DSceneEditorBase":
                    nTypeType = 1;
                    return "IAtlIEKG3DSceneEditorBase";
                    
                case "IEKG3DScene":
                case "IKG3DScene":
                    nTypeType = 1;
                    return "IAtlIEKG3DScene";
                    
                case "IEKG3DSceneSceneEditor":
                case "IKG3DSceneSceneEditor":
                    nTypeType = 1;
                    return "IAtlIEKG3DSceneSceneEditor";
                    
                case "IELogicalSceneConnection":
                case "ILogicalSceneConnection":
                    nTypeType = 1;
                    return "IAtlIELogicalSceneConnection";
                   
                case "IKG3DResourceBase":
                case "IG3DResourceBase":
                    nTypeType = 1;
                    return "IAtlIKG3DResourceBase"; 
                    
                case "IEKG3DTexture":
                case "IKG3DTexture":
                    nTypeType = 1;
                    return "IAtlIEKG3DTexture";                   

                case "BOOL":
                case "HWND":
                case "WPARAM":
                case "LPARAM":
                case "int":
                case "INT":
                case "PVOID":
                    nTypeType = 2;
                    return "LONG";
                    
                case "unsigned":
                case "unsigned int":
                case "UINT":
                case "DWORD_PTR":
                case "DWORD":
                    nTypeType = 2;
                    return "ULONG";                   

                case "float":
                case "FLOAT":
                    nTypeType = 2;
                    return "FLOAT";
                    
                case "LPCSTR":
                case "LPCTSTR":
                case "char":
                    nTypeType = 3;
                    return "UCHAR";

                case "BYTE":                
                    nTypeType = 3;
                    return "LONG";

                case "VOID":
                case "void":
                    nTypeType = 4;
                    return "LONG";

                case "HRESULT":
                    nTypeType = 3;
                    return "";

                default:
                    nTypeType = -1;
                    return "";
            }
        }
        /// <summary>
        /// �Զ������͵�ת��
        /// </summary>
        /// <param name="strType"></param>
        /// <returns></returns>
        protected bool IsSelfType(string strType)
        {
            return strType.Contains("IEKG3D") || strType.Contains("IKG3D");           
        }

       
    }
}
