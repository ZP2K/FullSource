using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Collections;
using System.Text;
using Resources;
using FSEye.FS2Lib;
using FSEye.PlugIn;
using FSEye.Security;
using FSEye;
using System.Threading;

/// <summary>
/// GM�����࣬��Ҫ��GM�����Ծ�̬�����ķ�ʽ������������С�
/// </summary>
public class GMUtil
{
    public GMUtil()
    {
    }

    /// <summary>
    /// ��ҽ��Լ������
    /// </summary>
    /// <param name="userId">FSEye�û�</param>
    /// <param name="server">������</param>
    /// <param name="roleName">��ɫ��</param>
    /// <param name="chatEnable">���Ա�־λ trueΪ����� falseΪ����</param>
    /// <returns></returns>
    public static bool SetRoleChatEnable(int userId, GameServer server, string roleName, bool chatEnable)
    {
        if (server == null || !server.IsConnected)
            return false;

        //�ȸ������ݿ�Ȼ��GMָ��
        ArrayList paramList = new ArrayList();
        string sqlCmdText = string.Format("UPDATE {0} SET {1}='?' WHERE {2}='?';",
            FS2TableString.RolesfirstTableName,
            FS2TableString.RolesfirstFieldNoChatIn, 
            FS2TableString.RolesfirstFieldRoleName);
        paramList.Add(chatEnable ? "0" : "1");
        paramList.Add(roleName);

        SqlCommand sqlCmd = new SqlCommand(sqlCmdText, paramList.ToArray());
        SqlResult result = WebUtil.QueryGameServerDb(userId, server, sqlCmd);
        if (result != null && result.Success)
        {
            string gmCmd = string.Format("?gm ds SetChatFlag({0})", chatEnable ? 0 : 1);
            server.DoPlugInAction(userId, LordControl.PlugInGuid, LordControl.ActionKeyExecuteGMCommand, roleName, gmCmd);

            FSEye.Security.LogSystem.TheInstance.WriteGMOperationLog(userId,
                chatEnable ? (int)GMOperation.ChatEnable : (int)GMOperation.ChatDisable,
                roleName,
                server.Id,
                string.Format(StringDef.GMMessageLog,
                              AdminServer.TheInstance.SecurityManager.GetUser(userId).RealName,
                              server.Name,
                              roleName == null || roleName.Length == 0 ? string.Empty : string.Format("[{0}]", roleName),
                              chatEnable ? GMOperation.ChatEnable.ToString() : GMOperation.ChatDisable.ToString()),
                (int)GMOpTargetType.Role);
            return true;
        }
        else
        {
            return false;
        }        
    }

    public static bool SetRoleFrozen(int userId, GameServer server, string roleName, bool freeze)
    {
        if (server == null || !server.IsConnected)
            return false;

        //�ȸ������ݿ�Ȼ��GMָ��
        ArrayList paramList = new ArrayList();
        string sqlCmdText = string.Format("UPDATE {0} SET {1}='?' WHERE {2}='?';",
            FS2TableString.RolesfirstTableName,
            FS2TableString.RolesfirstFieldNoLoginIn,
            FS2TableString.RolesfirstFieldRoleName);
        paramList.Add(freeze ? "1" : "0");
        paramList.Add(roleName);

        SqlCommand sqlCmd = new SqlCommand(sqlCmdText, paramList.ToArray());
        SqlResult result = WebUtil.QueryGameServerDb(userId, server, sqlCmd);
        if (result != null && result.Success)
        {
            //��GMָ������Login��־λ����ǿ���������
            string gmCmd = string.Format("?gm ds SetLoginFlag({0})", freeze ? 1 : 0);
            server.DoPlugInAction(userId, LordControl.PlugInGuid, LordControl.ActionKeyExecuteGMCommand, roleName, gmCmd);

            if (freeze)
                server.DoPlugInAction(userId, LordControl.PlugInGuid, LordControl.ActionKeyExecuteGMCommand, roleName, WebConfig.GMCommandKickPlayer);
            
            FSEye.Security.LogSystem.TheInstance.WriteGMOperationLog(userId,
                freeze ? (int)GMOperation.FreezeRole : (int)GMOperation.UnfreezeRole,
                roleName,
                server.Id,
                string.Format(StringDef.GMMessageLog,
                              AdminServer.TheInstance.SecurityManager.GetUser(userId).RealName,
                              server.Name,
                              roleName == null || roleName.Length == 0 ? string.Empty : string.Format("[{0}]", roleName),
                              freeze ? StringDef.Freeze + StringDef.Role : StringDef.Unfreeze + StringDef.Role),
                (int)GMOpTargetType.Role);
            return true;
        }
        else
        {
            return false;
        }
    }

    /// <summary>
    /// �ܹ��ʺ����õ���ɫ��(��ʱ��û��ȫ������)
    /// </summary>
    /// <param name="userId">FSEye�û�ID</param>
    /// <param name="server">������</param>
    /// <param name="account">�ʺ�</param>
    /// <param name="selectOnline">�Ƿ�����</param>
    /// <returns></returns>
    public static IList GetRoleNameByAccount(int userId, GameServer server, string account, bool selectOnline)
    {
        if (server == null || !server.IsConnected)
            return null;
        ArrayList roleList = new ArrayList();
        StringBuilder cmdText = new StringBuilder();
        cmdText.AppendFormat("SELECT {0},{1} FROM {2} WHERE {3}='?'", 
            FS2TableString.RolesfirstFieldId,
            FS2TableString.RolesfirstFieldRoleName, 
            FS2TableString.RolesfirstTableName,
            FS2TableString.RolesfirstFieldAccountName);
        if (selectOnline)
            cmdText.AppendFormat(" AND {0}='1'", FS2TableString.RolesfirstFieldOnline);

        SqlCommand cmd = new SqlCommand(cmdText.ToString(), account);
        SqlResult result = WebUtil.QueryGameServerDb(userId, server, cmd);
        if (result != null && result.Success)
        {
            result.SetFieldType(new SqlDataType[]{
                            SqlDataType.Int32,
                            SqlDataType.String
                            });
            object[] record = null;
            while ((record = result.ReadRecord()) != null)
            {
                roleList.Add(record[1] as string);
            }
        }
        return roleList;
    }

    /// <summary>
    /// ͨ�������ʺŵ�������Ϣ��ȡ�����ڵ�¼��GameServer(δ���)
    /// </summary>
    /// <param name="account"></param>
    /// <returns></returns>
    public static GameServer GetAccountOnlineServer(int userId,string account)
    {
        //return AdminServer.TheInstance.GameServerManager.GetGameServer(91);
        return null;
    }

    /// <summary>
    /// �����ʺ�
    /// </summary>
    /// <param name="userId">FSEye�û�Id</param>
    /// <param name="account">�ʺ�</param>
    /// <param name="freeze">�����־λ trueΪ���� falseΪ�ⶳ��</param>
    /// <returns>FSEye.PaySysLib.OperationResult</returns>
    public static FSEye.PaySysLib.OperationResult SetAccountFrozen(int userId,string account,bool freeze)
    {
        FSEye.PaySysLib.OperationResult result;
        if(freeze)
            result =  AdminServer.TheInstance.PaySysAgent.FreezeAccount(account);
        else
            result = AdminServer.TheInstance.PaySysAgent.UnfreezeAccount(account);

        FSEye.Security.LogSystem.TheInstance.WriteGMOperationLog(userId,
            freeze ? (int)GMOperation.FreezeAccount : (int)GMOperation.UnfreezeAccount,
            account,
            -1,
            string.Format(StringDef.GMMessageAccountLog,
                          AdminServer.TheInstance.SecurityManager.GetUser(userId).RealName,
                          account,
                          freeze ? GMOperation.FreezeAccount.ToString() : GMOperation.UnfreezeAccount.ToString(),
                          result.ToString()),
            (int)GMOpTargetType.Account);
        return result;
    }

    /// <summary>
    /// �õ��ʺ�������Ϣ
    /// </summary>
    /// <param name="userId">FSEye�û�Id</param>
    /// <param name="account">�ʺ�</param>
    /// <returns>������Ϣ</returns>
    public static string GetGatewayByAccount(int userId,string account)
    {
        return AdminServer.TheInstance.PaySysAgent.GetGatewayByAccount(account);
    }

    /// <summary>
    /// [GMָ��]���ʼ�
    /// </summary>
    /// <param name="userId">FSEye�û�Id</param>
    /// <param name="serverId">������Id</param>
    /// <param name="parameters">���ʼ�GMָ�����</param>
    public static bool SendMail(int userId,int serverId,object[] parameters)
    {
        GameServer server = AdminServer.TheInstance.GameServerManager.GetGameServer(serverId);
        if (server != null && server.IsConnected)
        {
            StringBuilder mailParam = new StringBuilder();
            string roleName = string.Empty;
            if (parameters.Length == 5 || parameters.Length == 10)
            {
                roleName = parameters[0] as string;
                foreach (object param in parameters)
                {
                    mailParam.Append(param);
                    mailParam.Append(',');
                }
            }
            else
            {
                return false;
                //throw new Exception("Parameters' number not correct.");
            }

            string gmCmd = string.Format(WebConfig.GMCommandSendMail, mailParam.ToString(0, mailParam.Length - 1));
            bool gmCmdResult = server.DoPlugInAction(userId, LordControl.PlugInGuid, LordControl.ActionKeyExecuteGMCommand, roleName, gmCmd);

            FSEye.Security.LogSystem.TheInstance.WriteGMOperationLog(userId,
                (int)GMOperation.SendMail,
                roleName,
                server.Id,
                string.Format(StringDef.GMMessageLog,
                              roleName,
                              server.Name,
                              roleName.Length == 0 ? string.Empty : string.Format("[{0}]", roleName),
                              StringDef.SendMail + string.Format("[{0}]", gmCmdResult ? StringDef.Success : StringDef.Failure)),
                (int)GMOpTargetType.Role);

            return gmCmdResult;
        }
        return false;
    }

    /*  ��ǰ�İ汾�����ڲ�ֳ���������
     *  ExeGMCommand������Э��
     *  GetGMCommandResult������server��userIdȡִ�н��
     *  ������������ǰ�İ汾
    public static string ExeGMCommand(int userId, int serverId, string roleName, string gmCmd, bool needLog)
    {
        object outArg = null;

        GameServer server = AdminServer.TheInstance.GameServerManager.GetGameServer(serverId);
        if ((server != null && server.IsConnected) == false)
            return null;

        bool result = server.DoPlugInAction(userId, LordControl.PlugInGuid, LordControl.ActionKeyExecuteGMCommand, ref outArg, roleName, gmCmd);

        if (result)
        {
            if (server != null && needLog)
            {
                User user = AdminServer.TheInstance.SecurityManager.GetUser(userId);
                FSEye.Security.LogSystem.TheInstance.WriteGMOperationLog(
                    userId,
                    (int)GMOperation.ExeSysCmd,
                    roleName,
                    server.Id,
                    string.Format(StringDef.GMMessageLog,
                            user == null ? userId + "(ID)" : user.RealName,
                            server.Name,
                            roleName.Length == 0 ? string.Empty : string.Format("[{0}]", roleName),
                            string.Format("{0}[{1}][{2}]", StringDef.ExecuteGMCommand, gmCmd, result ? StringDef.Success : StringDef.Failure)),
                    roleName.Length == 0 ? (int)GMOpTargetType.System : (int)GMOpTargetType.Role);
            }

            uint sessionId = (uint)outArg;
            for (int i = 0; i < WebConfig.GetQueryResultRetryTimes; i++)
            {
                Thread.Sleep(WebConfig.GetQueryResultDelay);
                string GMResult = server.GetPlugInData(userId, LordControl.PlugInGuid, LordControl.DataKeyGMResult, sessionId) as string;
                return GMResult;
            }
        }

        return null;
    }
    */

    public static bool ExeGMCommand(int userId, int serverId, string roleName, bool isAccountName, string gmCmd, bool isExecutedByGameServer, ArrayList gameServerArrayList, bool needLog)
    {
        object outArg = null;

        GameServer server = AdminServer.TheInstance.GameServerManager.GetGameServer(serverId);
        if ((server != null && server.IsConnected) == false)
            return false;

        bool result = server.DoPlugInAction(userId, LordControl.PlugInGuid, LordControl.ActionKeyExecuteGMCommand, ref outArg, roleName, isAccountName, gmCmd, isExecutedByGameServer, gameServerArrayList);

        if (server != null && needLog)
        {
            User user = AdminServer.TheInstance.SecurityManager.GetUser(userId);
            FSEye.Security.LogSystem.TheInstance.WriteGMOperationLog(
                userId,
                (int)GMOperation.ExeSysCmd,
                roleName,
                server.Id,
                string.Format(StringDef.GMMessageLog,
                        user == null ? userId + "(ID)" : user.RealName,
                        server.Name,
                        roleName.Length == 0 ? string.Empty : string.Format("[{0}]", roleName),
                        string.Format("{0}[{1}][{2}]", StringDef.ExecuteGMCommand, gmCmd, result ? StringDef.Success : StringDef.Failure)),
                roleName.Length == 0 ? (int)GMOpTargetType.System : (int)GMOpTargetType.Role);
        }

        return result;
    }

    public static string GetGMCommandResult(GameServer server, int userId)
    {
        string GMResult = server.GetPlugInData(userId, LordControl.PlugInGuid, LordControl.DataKeyUserGMResult, userId) as string;
        
        return GMResult;
    }










    public enum GMOperation
    {
        FreezeRole = 0,
        UnfreezeRole,
        FreezeAccount,
        UnfreezeAccount,
        ChatEnable,
        ChatDisable,
        SendMail,
        SendMessage,
        ExeSysCmd,
        CreateIBShopScript,
        DeleteIBShopScript
    }

    public enum GMOpTargetType
    {
        Account = 0,
        Role,
        System,
        Unknow = 10
    }
}