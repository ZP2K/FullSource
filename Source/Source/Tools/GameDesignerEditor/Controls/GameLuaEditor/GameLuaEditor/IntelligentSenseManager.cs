using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Collections;
using System.Data.SqlClient;
using System.IO;
using System.Windows.Forms;
using System.Diagnostics;

namespace GameLuaEditor
{
    class IntelligentSenseManager
    {
        private static IntelligentSenseManager intelligentSenseManager;
        private Hashtable apiTable = new Hashtable();
        private DataTable classTable = null;
        private DataTable codeTable = null;
        private DataTable returnTable = null;
        private DataTable argumentTable = null;

        private IntelligentSenseManager()
        {

        }

        /// <summary>
        /// ��ȡ���ܸ�֪���ݹ������
        /// </summary>
        /// <returns>���ܸ�֪���ݹ������</returns>
        public static IntelligentSenseManager GetIntelligentSenseManager()
        {
            if (intelligentSenseManager == null)
            {
                intelligentSenseManager = new IntelligentSenseManager();
                intelligentSenseManager.Init();
            }

            return intelligentSenseManager;
        }

        /// <summary>
        /// API����
        /// </summary>
        public Hashtable APITable
        {
            get
            {
                return apiTable;
            }
        }

        /// <summary>
        /// ����API�ļ�
        /// </summary>
        private void UpdateAPIFile()
        {
            bool requireUpdate = true;
            string fileName = Path.Combine(Application.StartupPath, @"Plugins\LuaCheck\API.lua");

            if (File.Exists(fileName))
            {
                DateTime fileWriteTime = File.GetLastWriteTime(fileName);

                if (fileWriteTime.Date == DateTime.Now.Date)
                {
                    requireUpdate = false;
                }
            }

            // 1��ֻ����һ��API�����ļ�
            if (requireUpdate)
            {
                string connectString = "Server = jx3web; DataBase = SIMDB; Uid = sim; Password = 1234;";
                SqlConnection connection = new SqlConnection(connectString);

                connection.Open();

                string sqlString = "SELECT * FROM SIM_ClassTable";
                classTable = Helper.GetDataTable(sqlString, connection);

                sqlString = "SELECT * FROM SIM_CodeList";
                codeTable = Helper.GetDataTable(sqlString, connection);

                sqlString = "SELECT * FROM SIM_ReturnList";
                returnTable = Helper.GetDataTable(sqlString, connection);

                sqlString = "SELECT * FROM SIM_ArgList";
                argumentTable = Helper.GetDataTable(sqlString, connection);

                connection.Close();

                // ��������
                foreach (DataRow dataRow in classTable.Rows)
                {
                    string classID = dataRow["ClassID"].ToString().Trim();
                    string className = dataRow["ClassName"].ToString().Trim();
                    string classProperty = dataRow["ClassProperty"].ToString();
                    string classFunction = dataRow["ClassFunction"].ToString();

                    Hashtable data = new Hashtable();
                    Hashtable propertyTable = new Hashtable();
                    Hashtable functionTable = new Hashtable();
                    // data["_Info"] = GetInfoTable(classTable, dataRow);

                    if (classProperty == "True")
                    {
                        data["'Property'"] = propertyTable;
                    }

                    if (classFunction == "True")
                    {
                        data["'Function'"] = functionTable;
                    }

                    data["'Name'"] = TranslateNilValue(className, false);
                    data["'IsConst'"] = dataRow["ClassType"].ToString().ToLower();

                    apiTable[string.Format("'{0}'", className)] = data;

                    // ��������ķ���������
                    DataRow[] rows = codeTable.Select(string.Format("ParentClass = '{0}'", classID));
                    bool constType = (bool)dataRow["ClassType"]; // ����������Ҫ���⴦��
                    Hashtable constTable = new Hashtable();

                    for (int i = 0; i < rows.Length; i++)
                    {
                        DataRow row = rows[i];
                        int index = i + 1;
                        string codeID = row["CodeID"].ToString().Trim();
                        string codeName = row["SenseWord"].ToString().Trim();
                        string category = row["Category"].ToString().Trim();
                        string field = row["Field"].ToString().Trim();

                        Hashtable codeData = new Hashtable();
                        // codeData["_Info"] = GetInfoTable(codeTable, row);
                        codeData["'Name'"] = TranslateNilValue(codeName, false);
                        codeData["'Abstract'"] = TranslateNilValue(row["Summary"].ToString(), false);
                        codeData["'Type'"] = "'function'";
                        codeData["'Class'"] = TranslateNilValue(className, false);
                        codeData["'CanServerUse'"] = field.Contains("SERVER").ToString().ToLower();
                        codeData["'CanClientUse'"] = field.Contains("CLIENT").ToString().ToLower();
                        codeData["'Detail'"] = TranslateNilValue(TranslateHtmlCode(row["Info"].ToString()), true);
                        codeData["'Index'"] = index.ToString();

                        if (category == "0") // �Ƿ���
                        {
                            Hashtable paramInfo = GetParameterInfo(codeID);

                            if (paramInfo != null)
                            {
                                codeData["'Param'"] = paramInfo;
                            }
                            else
                            {
                                codeData["'Param'"] = "nil";
                            }

                            Hashtable returnInfo = GetReturnInfo(codeID);

                            if (returnInfo != null)
                            {
                                codeData["'Return'"] = returnInfo;
                            }
                            else
                            {
                                codeData["'Return'"] = "nil";
                            }

                            functionTable[string.Format("'{0}'", codeName)] = codeData;
                        }
                        else // ������
                        {
                            propertyTable[string.Format("'{0}'", codeName)] = codeData;
                        }
                    }
                }

                string apiContent = ConstructLuaTable(apiTable);            
                Helper.WriteStringToFile(apiContent, fileName);
            }            
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ʼ�����ܸ�֪����
            UpdateAPIFile();
                        
            Process p = new Process();
            p.StartInfo.FileName = Application.StartupPath + "\\LuaEditLogicHeader.exe";
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;
            p.Start();
        }

        /// <summary>
        /// ת����ֵ�ַ���
        /// </summary>
        /// <param name="value">���ַ���</param>
        /// <param name="multiLine">�Ƿ��Ƕ����ı�</param>
        /// <returns>���ַ���</returns>
        private string TranslateNilValue(string value, bool multiLine)
        {
            string newValue = value.Trim();

            if (newValue == "")
            {
                newValue = "nil";
            }
            else
            {                
                if (multiLine)
                {
                    newValue = string.Format("[[{0}]]", newValue.Replace("[", "").Replace("]", ""));
                }
                else
                {
                    newValue = string.Format("'{0}'", newValue.Replace("'", ""));
                }                
            }

            return newValue;
        }        

        /// <summary>
        /// ����lua���ݱ�
        /// </summary>
        /// <param name="infoTable">��ϣ���ݱ�</param>
        /// <returns>�ı�����</returns>
        private string ConstructLuaTable(Hashtable infoTable)
        {
            StringBuilder content = new StringBuilder();
            int level = 0;
            content.AppendLine("APIInfo = {");
            ConstructLuaTable(infoTable, content, ref level);
            content.AppendLine("}");

            return content.ToString();
        }

        /// <summary>
        /// ����lua���ݱ�
        /// </summary>
        /// <param name="infoTable">��ϣ���ݱ�</param>
        /// <param name="content">�ı�����</param>
        /// <param name="level">��ǰ�㼶</param>
        private void ConstructLuaTable(Hashtable infoTable, StringBuilder content, ref int level)
        {
            level++;

            // �Զ�����
            StringBuilder spaceBuilder = new StringBuilder();
            for (int i = 0; i < level; i++)
            {
                spaceBuilder.Append("    ");
            }            

            foreach (string key in infoTable.Keys)
            {
                Hashtable childTable = infoTable[key] as Hashtable;

                if (childTable != null)
                {
                    content.AppendLine(string.Format("{0}[{1}] = {{", spaceBuilder.ToString(), key));
                    ConstructLuaTable(childTable, content, ref level);
                    content.AppendLine(string.Format("{0}}},", spaceBuilder.ToString()));
                }
                else
                {
                    content.AppendLine(string.Format("{0}[{1}] = {2},", spaceBuilder.ToString(), key, infoTable[key] as string));
                }
            }

            level--;
        }

        /// <summary>
        /// ת��html����
        /// </summary>
        /// <param name="detail">Դ�ַ���</param>
        /// <returns>ת������ַ���</returns>
        private string TranslateHtmlCode(string detail)
        {
            string newDetail = detail;

            // ���⴦���б��
            newDetail = newDetail.Replace("<P>", "").Replace("<\\P>", "\r\n");
            newDetail = newDetail.Replace("<BR>", "\r\n");           

            // �����ҳ��ǩȫ������
            StringBuilder conent = new StringBuilder();
            int ignoreLevel = 0;

            foreach (char c in newDetail.ToCharArray())
            {
                switch(c)
                {
                    case '<':
                        {
                            ignoreLevel++;
                            break;
                        }
                    case '>':
                        {
                            ignoreLevel--;
                            break;
                        }
                    default:
                        {
                            if (ignoreLevel == 0)
                            {
                                conent.Append(c);
                            }

                            break;
                        }
                }
            }

            return conent.ToString();
        }

        private string GetReturnType(string returnString)
        {
            string returnType = "Object";

            switch (returnString)
            {
                case "t":
                    {
                        returnType = "Table";
                        break;
                    }
                case "dw":
                    {
                        returnType = "DWord";
                        break;
                    }
                case "nil":
                    {
                        returnType = "Null";
                        break;
                    }
                case "n":
                    {
                        returnType = "Integer";
                        break;
                    }
                case "b":
                    {
                        returnType = "Boolean";
                        break;
                    }
                case "player":
                    {
                        returnType = "Player";
                        break;
                    }
                case "sz":
                    {
                        returnType = "String";
                        break;
                    }
                case "npc":
                    {
                        returnType = "Npc";
                        break;
                    }
                case "doodad":
                    {
                        returnType = "Doodad";
                        break;
                    }
                case "shop":
                    {
                        returnType = "Shop";
                        break;
                    }
                case "scene":
                    {
                        returnType = "Scene";
                        break;
                    }
                case "item":
                    {
                        returnType = "Item";
                        break;
                    }
                case "char":
                    {
                        returnType = "Character";
                        break;
                    }
                case "buff":
                    {
                        returnType = "Buff";
                        break;
                    }
                /* ���ݿ�������ظ�
                case "n":
                    {
                        returnType = "Const";
                        break;
                    }
                */
                case "skill":
                    {
                        returnType = "Skill";
                        break;
                    }
                case "e":
                    {
                        returnType = "Enum";
                        break;
                    }
            }

            return returnType;
        }

        /// <summary>
        /// ��ȡ�����ķ���ֵ��Ϣ
        /// </summary>
        /// <param name="codeID">������ID</param>
        /// <returns>������Ϣ</returns>
        private Hashtable GetReturnInfo(string codeID)
        {
            Hashtable infoTable = null;

            DataRow[] rows = returnTable.Select(string.Format("CodeID = '{0}'", codeID), "RECID");

            if (rows.Length > 0)
            {
                infoTable = new Hashtable();
            }

            for (int i = 0; i < rows.Length; i++)
            {
                DataRow row = rows[i];
                int index = i + 1;
                Hashtable data = new Hashtable();

                data["'Info'"] = TranslateNilValue(TranslateHtmlCode(row["ReturnInfo"].ToString()), true);
                data["'ReturnType'"] = TranslateNilValue(GetReturnType(row["ReturnClass"].ToString()), false);
                infoTable[index.ToString()] = data;
            }

            return infoTable;
        }

        /// <summary>
        /// ��ȡ�����Ĳ�����Ϣ
        /// </summary>
        /// <param name="codeID">������ID</param>
        /// <returns>������Ϣ</returns>
        private Hashtable GetParameterInfo(string codeID)
        {
            Hashtable infoTable = null;

            DataRow[] rows = argumentTable.Select(string.Format("CodeID = '{0}'", codeID, "RECID"));
            
            if (rows.Length > 0)
            {
                infoTable = new Hashtable();
            }

            for (int i = 0; i < rows.Length; i++)
            {
                DataRow row = rows[i];
                int index = i + 1;                
                Hashtable data = new Hashtable();

                data["'Name'"] = TranslateNilValue(row["ArgValue"].ToString(), false);
                data["'Info'"] = TranslateNilValue(row["ArgInfo"].ToString(), true);
                data["'Optional'"] = row["Optional"].ToString().ToLower();
                // data["_Info"] = GetInfoTable(argumentTable, row);
                infoTable[index.ToString()] = data;
            }

            return infoTable;
        }

        /// <summary>
        /// ��ȡ��Ϣ���ݱ�
        /// </summary>
        /// <param name="dataTable">���ݱ�</param>
        /// <param name="dataRow">������</param>
        /// <returns>��Ϣ��</returns>
        private Hashtable GetInfoTable(DataTable dataTable, DataRow dataRow)
        {
            Hashtable infoTable = new Hashtable();

            foreach (DataColumn dataColumn in dataTable.Columns)
            {
                infoTable[dataColumn.ColumnName] = dataRow[dataColumn].ToString();
            }

            return infoTable;
        }
    }
}
