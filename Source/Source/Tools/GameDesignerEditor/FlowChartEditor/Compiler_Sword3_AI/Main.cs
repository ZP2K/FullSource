using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Collections;
using System.Data;

using FlowChartDataLibrary;
using DBManager;
using DataInteractor;
using CodeCompiler;

namespace Compiler_Sword3_AI
{
    public class Main : Compiler
    {
        private DataTable actionTable; // �������ݱ�
        private List<string> customFunctionList = new List<string>(); // �Զ��嶯���ű�����

        /// <summary>
        /// ���ɴ�������
        /// </summary>
        /// <param name="code">����</param>
        /// <param name="metaData">����ͼԪ����</param>
        /// <param name="list">����ͼԪ��������</param>
        private void GenerateCode(StringBuilder code, FlowChartMetaData metaData, List<FlowChartMetaData> list)
        {
            if(!list.Contains(metaData))
            {
                list.Add(metaData);

                switch(metaData.DataType)
                {
                    case "AIStateNode": // AI״̬���
                        {
                            code.AppendLine(string.Format("    -- {0}", metaData.DisplayText));
                            code.AppendLine(string.Format("    state = ai.NewState({0})", 
                                            metaData.Code));

                            List<FlowChartMetaData> nextMetaDataList = new List<FlowChartMetaData>();

                            foreach (FlowChartMetaData aiLine in metaData.NextMetaDataList)
                            {
                                code.AppendLine(string.Format("    state.HandleEvent({0}, {1})",
                                                CodeProviderClass.GetEventString(aiLine.Data), aiLine.NextMetaDataList[0].ID));
                                nextMetaDataList.Add(aiLine.NextMetaDataList[0]);
                            }

                            code.AppendLine();

                            foreach (FlowChartMetaData nextMetaData in nextMetaDataList)
                            {
                                GenerateCode(code, nextMetaData, list);
                            }                            

                            break;
                        }
                    case "AIActionNode": // AI�������
                        {
                            if(metaData.DisplayText != "")
                            {
                                code.AppendLine(string.Format("    -- {0}", metaData.DisplayText));
                            }
                            
                            string[] information = CodeProviderClass.GetActionInformation(metaData.Data);
                            int actionID = int.Parse(information[2]);

                            if(actionID > 1000) // �Զ��嶯��
                            {
                                code.AppendLine(string.Format("    action = ai.NewAction({0}, {1})",
                                                              metaData.ID, information[2]));

                                if (actionID > 2000) // ID����2000��ActionΪ���ļ����Զ��嶯��
                                {
                                    customFunctionList.Add(information[3]);
                                }
                            }                            
                            else // ϵͳ����
                            {
                                code.AppendLine(string.Format("    action = ai.NewAction({0}, AI_ACTION.{1})",
                                    metaData.ID, information[0]));
                            }                            

                            if(information[1] != "")
                            {                                
                                code.AppendLine(string.Format("    action.SetParam({0})", information[1]));
                            }                            
                            
                            List<FlowChartMetaData> nextMetaDataList = new List<FlowChartMetaData>();
                            SortedList<string, string> sortedList = new SortedList<string, string>();

                            foreach(FlowChartMetaData aiLine in metaData.NextMetaDataList)
                            {
                                sortedList.Add(aiLine.Data as string, aiLine.NextMetaDataList[0].ID.ToString());
                                nextMetaDataList.Add(aiLine.NextMetaDataList[0]);
                            }
                            
                            string branchArgs = "";

                            for(int i = 0; i < sortedList.Count; i++)
                            {
                                branchArgs += string.Format("{0}, ", sortedList.Values[i]);
                                
                            }

                            branchArgs = branchArgs.TrimEnd(new char[] { ',', ' ' });                                                        

                            if(branchArgs != "")
                            {
                                code.AppendLine(string.Format("    action.SetBranch({0})", branchArgs));
                            }

                            // ע��������
                            code.AppendLine(string.Format("    g_AIDebugInfo[{0}] = '[{0}] {1}({2})'", metaData.ID.ToString(),
                                                          information[0], information[1].Replace("'", "")));
                            code.AppendLine();
                            
                            foreach(FlowChartMetaData nextMetaData in nextMetaDataList)
                            {
                                GenerateCode(code, nextMetaData, list);
                            }

                            break;
                        }
                    case "AISetStateNode": // AI״̬ת�����
                        {
                            code.AppendLine(string.Format("    -- {0}", metaData.DisplayText));
                            code.AppendLine(string.Format("    action = ai.NewAction({0}, AI_ACTION.SET_STATE)", metaData.ID));
                            code.AppendLine(string.Format("    action.SetParam({0})", metaData.NextMetaDataList[0].NextMetaDataList[0].Code));

                            // ע��������
                            code.AppendLine(string.Format("    g_AIDebugInfo[{0}] = '[{0}] SetParam({1})'", metaData.ID.ToString(),
                                                          metaData.NextMetaDataList[0].NextMetaDataList[0].Code));
                            code.AppendLine();

                            GenerateCode(code, metaData.NextMetaDataList[0].NextMetaDataList[0], list);

                            break;
                        }
                }
            }
        }

        /// <summary>
        /// ����LS��
        /// </summary>
        /// <returns>LS������</returns>
        private string ConstructLSFile()
        {
            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            string sqlString = "SELECT * FROM AI_LS";
            DataTable lsTable = dataBaseManager.GetDataTable(sqlString, dataBaseManager.Connection_Jx3web);
            StringBuilder code = new StringBuilder();

            code.AppendLine("ARGUMENT_STRING_TABLE = {");

            for (int i = 0; i < lsTable.Rows.Count; i++)
            {
                DataRow row = lsTable.Rows[i];
                string id = row["ID"].ToString();
                string word = row["Content"].ToString();

                if(i == lsTable.Rows.Count - 1) // ���һ������
                {                    
                    if ((word.StartsWith("'") && word.EndsWith("'")) ||
                        (word.StartsWith("\"") && word.EndsWith("\"") ||
                        (word.StartsWith("[[") && word.EndsWith("]]")))) // ����Ƿ��Ѿ����ַ������
                    {
                        code.AppendLine(string.Format("    [{0}] = {1}", id, word));
                    }
                    else
                    {
                        code.AppendLine(string.Format("    [{0}] = '{1}'", id, word));
                    }                    
                }
                else
                {
                    if ((word.StartsWith("'") && word.EndsWith("'")) ||
                        (word.StartsWith("\"") && word.EndsWith("\"") ||
                        (word.StartsWith("[[") && word.EndsWith("]]")))) // ����Ƿ��Ѿ����ַ������
                    {
                        code.AppendLine(string.Format("    [{0}] = {1},", id, word));
                    }
                    else
                    {
                        code.AppendLine(string.Format("    [{0}] = '{1}',", id, word));
                    } 
                }
            }

            code.Append("}");

            return code.ToString();
        }

        /// <summary>
        /// ��鶯�������Ƿ����ַ���
        /// </summary>
        /// <param name="actionID">����ID</param>
        /// <param name="paramIndex">����ID</param>
        /// <returns>���������Ƿ����ַ����ַ���</returns>
        private bool IsParamString(string actionID, int paramIndex, DataTable actionTable)
        {
            bool result = false;

            DataRow[] rows = actionTable.Select(string.Format("actionid = '{0}'", actionID));
            if(rows.Length > 0)
            {               
                if(!(rows[0]["parms"] is DBNull) && rows[0]["parms"].ToString() != "")
                {
                    string[] data = rows[0]["parms"].ToString().Split(new char[] { '\n' });
                    string[] value = data[paramIndex].TrimEnd(new char[] { '\r' }).Split(new char[] { ',' });
                    result = (value[1] == "-2"); 
                }                
            }

            return result;
        }

        /// <summary>
        /// �༭����ͼ
        /// </summary>
        /// <param name="FlowID">����ͼID</param>
        /// <param name="data">Ԫ��������</param>
        /// <param name="manager">Ԫ���ݹ�����</param>
        /// <param name="flowVersion">��ͼ�İ汾��</param>
        /// <returns>�Ƿ����ɹ�</returns>
        public override bool GenerateCode(int FlowID, List<FlowChartMetaData> data, FlowChartMetaDataManager manager)
        {            
            // ��ʼ������            
            string[] dataArray = scriptPath.Split(new char[] { '\\' });
            string shortPath = dataArray[1];

            for (int i = 2; i < dataArray.Length; i++)
            {
                shortPath = shortPath + string.Format("\\{0}", dataArray[i]);
            }

            string fullPath = string.Format(@"{0}\scripts\Map\{1}\ai\{2}", rootDirectory.TrimEnd(new char[] { '\\' }), dataArray[0], shortPath);

            bool success = true;
            DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
            customFunctionList.Clear();

            // ��ʼ������
            StringBuilder code = new StringBuilder();
            string initialStateName = CodeProviderClass.GetStateString(manager.GetEventData(1)); // ��̬�ı�����
            List<FlowChartMetaData> metaDataList = manager.GetAllNodes();
            List<FlowChartMetaData> stateList = new List<FlowChartMetaData>();
            List<FlowChartMetaData> actionList = new List<FlowChartMetaData>();
            Hashtable registActions = new Hashtable();

            string sqlString = "SELECT actionid, parms FROM AI_Action_Define";
            actionTable = dataBaseManager.GetDataTable(sqlString, dataBaseManager.Connection_Jx3web);

            foreach (FlowChartMetaData metaData in metaDataList)
            {
                switch (metaData.DataType)
                {
                    case "AIStateNode":
                        {
                            stateList.Add(metaData);
                            break;
                        }
                    case "AIActionNode":
                        {
                            actionList.Add(metaData);
                            string[] information = CodeProviderClass.GetActionInformation(metaData.Data);
                            int actionID = int.Parse(information[2]);

                            if ((actionID == 0) || (actionID > 1000 && registActions[information[2]] == null)) // ע���Զ��嶯��
                            {
                                registActions[information[2]] = metaData;
                            }

                            break;
                        }
                }
            }

            // ���ע��
            code.AppendLine(string.Format("-- ��д�ߣ�{0}", Dns.GetHostName()));
            code.AppendLine(string.Format("-- �汾�ţ�{0}", manager.Version));
            code.AppendLine();

            // ����ͷ�ļ�
            code.AppendLine("Include('scripts/ai/customFuctions.lua')");
            code.AppendLine("Include('scripts/ai/AIParam.lua')");
            code.AppendLine("Include('scripts/ai/argumentStrings.ls')");
            code.AppendLine();
            code.AppendLine("g_AIDebugInfo = {}");
            code.AppendLine();

            // ��������
            code.AppendLine("-- ����״̬����");
            foreach (FlowChartMetaData stateNode in stateList)
            {
                string stateName = stateNode.Code;
                code.AppendLine(string.Format("local {0} = {1} -- {2}", stateName, stateNode.ID, stateNode.DisplayText));
            }
            code.AppendLine();

            // �����Զ��帽�ӽű�
            FlowChartInteractor it = Interactor.GetInteractor().CurrentFlowChartInteractor;
            string customCode = it.CustomText;
            if(customCode != null && customCode != "")
            {
                code.AppendLine("-- �Զ��帽�ӽű�");
                code.AppendLine(string.Format("{0}", customCode));
                code.AppendLine();
            }

            // д������
            code.AppendLine("function Setup(ai)");
            code.AppendLine("    local state");
            code.AppendLine("    local action");
            code.AppendLine("    local param = g_AIParam[ai.nAIType]");
            code.AppendLine();

            // ע���Զ��嶯��
            code.AppendLine("    -- ע���Զ��嶯��");
            foreach (object o in registActions.Values)
            {
                FlowChartMetaData actionNode = o as FlowChartMetaData;
                string[] information = CodeProviderClass.GetActionInformation(actionNode.Data);
                code.AppendLine(string.Format("    ai.RegisterUserAction({0}, '{1}')", information[2], information[0]));                
            }            
            code.AppendLine();

            List<FlowChartMetaData> list = new List<FlowChartMetaData>();
            GenerateCode(code, manager.GetEventNode(1), list);

            code.AppendLine(string.Format("    ai.SetInitState({0})", initialStateName));
            code.AppendLine("end");
            code.AppendLine();

            // ���Է�������
            code.AppendLine("-- ���Է���");
            code.AppendLine("function DebugOnCallAction(action_id)");
            code.AppendLine("    Log('[AI] Call action: ' .. g_AIDebugInfo[action_id])");
            code.AppendLine("end");

            // д���ļ��Զ���ű���������
            if(customFunctionList.Count > 0)
            {
                code.AppendLine();
                code.AppendLine("-- ��AIͼ���Զ���ű�");

                foreach(string s in customFunctionList)
                {
                    code.AppendLine(string.Format("{0}", s));
                    code.AppendLine();
                }
            }

            // ��ȡת�������ַ���
            string scriptText = code.ToString();
            scriptText = ConvertScriptText(scriptText);

            // ����ļ�
            string fileName = string.Format("{0}.lua", fullPath);
            saveFile(fileName, scriptText);

            string userActionScripts = dataBaseManager.GetUserActionContext();            
            fileName = Path.Combine(rootDirectory, @"scripts\ai\customFuctions.lua");
            saveFile(fileName, userActionScripts);

            fileName = Path.Combine(rootDirectory, @"scripts\ai\argumentStrings.ls");
            string lsContent = ConstructLSFile();
            saveFile(fileName, lsContent);                

            return success;
        }

        /// <summary>
        /// ת���ű�����ַ���
        /// </summary>
        /// <param name="content">�ű�����</param>
        /// <returns>ת������ַ���</returns>
        private string ConvertScriptText(string content)
        {
            CodeConvert codeConvert = CodeConvert.GetCodeConvert();
            codeConvert.ConvertTableName = "ARGUMENT_STRING_TABLE";
            List<string> wordList = codeConvert.GetStringList(content);

            if (wordList.Count > 0) // ��Ҫ�ַ����滻
            {
                DataBaseManager dataBaseManager = DataBaseManager.GetDataBaseManager();
                foreach(string s in wordList)
                {
                    string id = dataBaseManager.InsertIntoLS(s.Replace("'", "''"));
                    content = content.Replace(s, string.Format("{0}[{1}]", codeConvert.ConvertTableName, id));
                }
            }            

            return content;
        }
    }
}
