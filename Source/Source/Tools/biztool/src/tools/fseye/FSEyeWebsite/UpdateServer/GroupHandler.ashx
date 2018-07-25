<%@ WebHandler Language="C#" Class="GroupHandler" %>

//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2008-5-21
//      File_base        : copy from GameServerHandler
//      File_ext         : ashx
//      Author           : 
//      Description      : ר�����������رա����ء�����
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Web.SessionState;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using Resources;
using ZedGraph;
using FSEye;
using FSEye.Security;
using FSEye.WebControls;
using FSEye.PlugIn;

/// <summary>
/// ��Ϸ������AJAX��������
/// </summary>
/// <author>������</author>
public class GroupHandler : IHttpHandler, IReadOnlySessionState 
{
	/* AJAX����Ĳ��� */

    //��������(ServerOverviewAndMonitor.aspx)
    private const string OperationGetServerGroupOverviewMonitor = "GetServerGroupOverviewMonitor";

    //��������(ServerOverviewAndMonitor.aspx) tooltip(������)
    private const string OperationGetServerGroupSummary = "GetServerGroupSummary";

	public void ProcessRequest(HttpContext context)
	{        
		try
		{
			if (!AdminServer.TheInstance.IsRunning)
			{
				context.Response.ContentType = "text/plain";
				context.Response.Output.Write(StringDef.ServiceUnavaiable);
				return;
			}

            //����Ȩ�޼��
            if (context.Session[WebConfig.SessionCurrentUser] == null)
            {
                //Response.Redirect(WebConfig.PageLogin, true);
                return;
            }
            UserInfo userInfo = context.Session[WebConfig.SessionCurrentUser] as UserInfo;
            if (userInfo == null) return;
            if (!AdminServer.TheInstance.SecurityManager.ValidateLoginUser(userInfo.Id, userInfo.LoginId))
            {
                context.Session.Remove(WebConfig.SessionCurrentUser);
                return;
            }
            
			if (context.Request.Params[WebConfig.ParamOperation] != null)
			{
				context.Response.ContentType = "text/plain";
  
                //��Request���зָ�
                //Request����ʽΪ�� op / selectGroupId / filter
                
                string fullOp = context.Request.Params[WebConfig.ParamOperation];
                String[] tempString = fullOp.Split('_');
                string op = tempString[0];
                string selectGroupId = string.Empty;
                string filter = string.Empty;
                if (tempString.Length == 3)
                {
                    selectGroupId = tempString[1];
                    filter = tempString[2];
                }
                
                
				switch (op)
				{
					
                    //��������(ServerOverviewAndMonitor.aspx)                            
                    case OperationGetServerGroupOverviewMonitor:
                        ProcessGetServerGroupOverviewMonitor(context, selectGroupId, filter);
                        break;

                    //��������(ServerOverviewAndMonitor.aspx)
                    //tooltip(������)
                    case OperationGetServerGroupSummary:
                        ProcessGetServerGroupSummary(context);
                        break;
                                                                  
					default:
						//TODO ��¼���󣬲���ʱ�׳��쳣
						throw new Exception("�յ�δ֪��GameServerHandler����");
				}
			}
			else
			{
				//TODO ��¼����
			}
		}
		catch (Exception ex)
		{
			Util.DebugLog(ex.ToString());
		}
	}

	public bool IsReusable
	{
		get
		{
			return true;
		}
	}






    private void ProcessGetServerGroupOverviewMonitor(HttpContext context, string selectGroup, string filter)
    {
        IList netGroupList = AdminServer.TheInstance.GameServerManager.TopServerGroups;

        ServerGroup tempServerGroup = new ServerGroup();
        tempServerGroup.GroupType = ServerGroup.Type.Group;
        
        foreach (ServerGroup netGroup in netGroupList)
        {
            if (netGroup.GroupType == ServerGroup.Type.Server)
            {
                if (filter == "noFilter")
                {
                    tempServerGroup.List.Add(netGroup);
                }
                else if (filter == "filterOpenGroup" && netGroup.group_enable == ServerGroup.GroupEnable.Open)
                {
                    tempServerGroup.List.Add(netGroup);
                }
                else if (filter == "filterCloseGroup" && netGroup.group_enable == ServerGroup.GroupEnable.Close)
                {
                    tempServerGroup.List.Add(netGroup);
                }               
            }
            else if (netGroup.GroupType == ServerGroup.Type.Group)
            {
                GetActualServerGroup(context, netGroup, netGroup.Name, selectGroup, filter);
            }
        }

        if (tempServerGroup.List.Count > 0)
        {
            CreateServerGroupOverviewMonitor(context, tempServerGroup, "/", selectGroup);
        }
    }

    private void GetActualServerGroup(HttpContext context, ServerGroup netGroup, String groupName, String selectGroup, String filter)
    {
        if (context == null)
            throw new ArgumentNullException("context");
        if (netGroup == null)
            return;

        //ʵ���ϣ���������netGroupһ��������Group����Ϊ�Ѿ�����һ�������������
        if (netGroup.GroupType == ServerGroup.Type.Group)
        {
            IList childGroupIList = netGroup.List;
            ServerGroup tempServerGroup = new ServerGroup();
            tempServerGroup.GroupType = ServerGroup.Type.Group;
            
            if ((childGroupIList != null) && (childGroupIList.Count > 0))
            {                
                foreach (ServerGroup childGroup in childGroupIList)                
                {                    
                    if (childGroup.GroupType == ServerGroup.Type.Group)
                    {
                        string childGroupName = groupName + "/" + childGroup.Name;
                        GetActualServerGroup(context, childGroup, childGroupName, selectGroup, filter);
                    }
                    else if (childGroup.GroupType == ServerGroup.Type.Server)
                    {                        
                        if (filter == "noFilter")
                        {
                            tempServerGroup.List.Add(childGroup);
                        }
                        else if (filter == "filterOpenGroup" && childGroup.group_enable == ServerGroup.GroupEnable.Open)
                        {
                            tempServerGroup.List.Add(childGroup);
                        }
                        else if (filter == "filterCloseGroup" && childGroup.group_enable == ServerGroup.GroupEnable.Close)
                        {
                            tempServerGroup.List.Add(childGroup);
                        }   
                    }
                }
            }
            else
            {
                return;   
            }
            
            if (tempServerGroup.List.Count > 0)
            {
                IComparer myComparer = new ByServerGroupIdComparer();
                ArrayList tempArrayList = ArrayList.Adapter(tempServerGroup.List);
                tempArrayList.Sort(myComparer);
                tempServerGroup.List = (IList)tempArrayList;
                
                CreateServerGroupOverviewMonitor(context, tempServerGroup, groupName, selectGroup);
            }
        }
    }

    private void CreateServerGroupOverviewMonitor(HttpContext context, ServerGroup netGroup, String groupName, string selectGroup)
    {
        //=============================
        //    ��������˵�����������ȼ��Ӹߵ��͡�
        //����          ����ķ�������������Ϊ��ĸ
        //�ر�          ����ķ�������������Ϊ��ĸ        
        //Updating      ��ȡÿ��guard�ĸ����������ʾ��С��
        //Downloading   ��ȡÿ��guard�������������ʾ��С��
        //=============================
                    
        using (HtmlTextWriter writer = new HtmlTextWriter(context.Response.Output))
        {
            writer.Write("<table cellspacing='1' class='{0}' style='width:1000;'>", CssConfig.TableNormal);
            writer.Write("<tr><td  align='center' style='font-size:16px;color:#FF9900;weight:2000' class='{0}'>{1}</td></tr>", CssConfig.TableCellNormal, groupName);
            writer.Write("<tr>");
            writer.Write("<td>");
            IList childGroupIList = netGroup.List;

            //����checkbox״̬
            bool bCheckboxEnable;
            if (context.Session["bCheckboxEnable"] == null)
                bCheckboxEnable = true;
            else
                bCheckboxEnable = (bool)context.Session["bCheckboxEnable"];
            
            int count = 0;
            int groupCount = childGroupIList.Count;
            int groupPerRow = 10;
            string columnWidth = "width='" + (100 / groupPerRow) + "%'";
            int rowCount = groupCount / groupPerRow;
            int remainder = groupCount % groupPerRow;            
            if (remainder > 0)
                rowCount++;

            writer.Write("<table width=100% cellspacing=1 cellpadding=0>");

            for (int i = 0; i < rowCount; i++)
            {
                writer.Write("<tr>");

                for (int j = 0; j < groupPerRow; j++)
                {
                    if (count < groupCount)
                    {
                        //ͳ������
                        double groupServerCount = 0;    //�����������������
                        double startedCount = 0;    //������������������������
                        double stopedCount = 0;    //�����ѹرշ�������������

                        bool showProgessBar = false;
                        double progressBarValue = 0;
                        bool isDownloading = false;
                        bool isSelectGroup = false;
                        bool bNoConnected = false; //�жϸ����Ƿ�����������
                       
                        
                        
                        ServerGroup groupToWrite = childGroupIList[i * groupPerRow + j] as ServerGroup;
                        IList serverIList = groupToWrite.List;
                        groupServerCount = groupToWrite.List.Count;

                        int enumLen = Enum.GetValues(typeof(GameServer.ServiceState)).GetLength(0) - 1 ; 
                        GameServer.ServiceState serverGroupState = GameServer.ParseServiceState(Enum.GetName(typeof(GameServer.ServiceState), enumLen));  //���״̬����ɫ��ʾ�ȼ����
                        //bool noWarning = true;
                        IList checkResults = AdminServer.TheInstance.GameServerMonitor.LastCheckResults;

                        Dictionary<string, double> mapIp = new Dictionary<string ,double>();
                        bool bUpdating = false;
                        //foreach (GameServer server in serverIList)
                        for (int iServer = 0; iServer < serverIList.Count; iServer++) 
                        {
                            GameServer server = (GameServer)serverIList[iServer];
                            if (server.GameServerState == GameServer.ServerState.Unknown)
                            {
                                bNoConnected = true;
                                break;
                            }                            
                            
                            if (!mapIp.ContainsKey(server.IpAddress))
                                mapIp.Add(server.IpAddress, 0);
                            
                            if (server.GameServiceState == GameServer.ServiceState.Running)
                            {
                                startedCount++;
                            }
                            else if (server.GameServiceState == GameServer.ServiceState.OKStopped)
                            {
                                stopedCount++;
                            } 
                            else if (server.GameServiceState == GameServer.ServiceState.Starting)
                            {
                               // showProgessBar = true;
                                progressBarValue = startedCount / groupServerCount;
                            }
                            else if (server.GameServiceState == GameServer.ServiceState.Stopping)
                            {
                                //showProgessBar = true;
                                progressBarValue = stopedCount / groupServerCount;
                            }

                            switch (server.UpdateOrDownState)
                            {
                                case GameServer.UpdateState.Downloaded:
                                case GameServer.UpdateState.Updated: 
                                    {
                                        mapIp[server.IpAddress] = 100;
                                        showProgessBar = true;
                                    } break;
                                case GameServer.UpdateState.Updating:
                                    {
                                        showProgessBar = true;
                                        isDownloading = false;
                                        if (server.UpdateMsg.Count > 0)
                                        {
                                            string[] str = server.UpdateMsg[server.UpdateMsg.Count - 1] as string[];
                                            double bar = (double.Parse(str[0]) / double.Parse(str[1]));
                                            mapIp[server.IpAddress] = bar * 100;
                                            if (bar == 0)
                                            {
                                                //ֻ����һ������״̬Ϊ0����������Ϊ0��
                                                progressBarValue = 0;
                                                break;
                                            }
                                            else if (progressBarValue == 0)
                                            {
                                                //�����ѭ������һ��ֵ
                                                progressBarValue = bar;
                                            }
                                            else if (bar < progressBarValue)
                                            {
                                                //��ȡ����server.DownloadProgress����С��ֵ
                                                progressBarValue = bar;
                                            }
                                        }
                                        bUpdating = true;
                                    } break;
                                case GameServer.UpdateState.Downloading:
                                    {

                                        showProgessBar = true;
                                        isDownloading = true;
                                        mapIp[server.IpAddress] = server.DownloadProgress * 100;
                                        if (server.DownloadProgress == 0)
                                        {
                                            //ֻ����һ������״̬Ϊ0����������Ϊ0��
                                            progressBarValue = 0;
                                            break;
                                        }
                                        else if (progressBarValue == 0)
                                        {
                                            //�����ѭ������һ��ֵ
                                            progressBarValue = server.DownloadProgress;
                                        }
                                        else if (server.DownloadProgress < progressBarValue)
                                        {
                                            //��ȡ����server.DownloadProgress����С��ֵ
                                            progressBarValue = server.DownloadProgress;
                                        }                                        
                                    } break;
                            }
                            
                            
                            if (server.GameServiceState < serverGroupState)
                            {
                                serverGroupState = server.GameServiceState;
                            }
                            
                            //if (noWarning == true)
                            //{
                            //    int indexOfServer = AdminServer.TheInstance.GameServerManager.Servers.IndexOf(server);
                            //    if (checkResults != null && indexOfServer < checkResults.Count && indexOfServer >= 0)
                            //    {
                            //        CheckResult result = checkResults[indexOfServer] as CheckResult;
                            //        noWarning = result.NoProblem;
                            //    }
                            //}
                        }

                        string color = String.Empty;
                        ////===================
                        //checkbox
                        
                        if (context.Session["SelectGroup"] != null && selectGroup == "")
                        {
                            selectGroup = context.Session["SelectGroup"].ToString();
                        }
                        if (selectGroup.IndexOf(";" + groupToWrite.Id.ToString() + ";") != -1)
                        {
                            isSelectGroup = true;
                        }
                        if (context.Session["JobType"] != null && isSelectGroup)
                        {
                            string jobtype = context.Session["JobType"].ToString();
                            if (jobtype != null)
                            {
                                switch (jobtype)
                                {
                                    case "2":
                                        serverGroupState = GameServer.ServiceState.Starting;
                                        break;
                                    case "3":
                                        serverGroupState = GameServer.ServiceState.Stopping;
                                        break;
                                }
                            }
                        }

                        if (bNoConnected)
                        {
                            color = "#996633";
                        }
                        else
                        {
                            switch (serverGroupState)
                            {
                                case GameServer.ServiceState.Running:
                                    color = "#00CC00"; //��ɫ
                                    break;
                                case GameServer.ServiceState.OKStopped:
                                    color = "DarkGray";
                                    break;
                                case GameServer.ServiceState.ErrorStopped:
                                    color = "Red";
                                    break;
                                //                            case GameServer.ServiceState.Updating:
                                //                              color = "MediumOrchid";
                                //                            break;
                                case GameServer.ServiceState.Stopping:
                                    color = "DeepSkyBlue";
                                    break;
                                case GameServer.ServiceState.Starting:
                                    color = "Gold";
                                    break;

                                default:
                                    color = "DarkGray";
                                    break;
                            }

                            if (serverGroupState != GameServer.ServiceState.Stopping && serverGroupState != GameServer.ServiceState.Starting && bUpdating)
                            {
                                color = "MediumOrchid";
                            }
                        }
                        
                        writer.Write("<td " + columnWidth + ">");
                        writer.Write("<table width='100%' cellpadding='0' cellspacing='1' class='" + CssConfig.ServerGroupOverviewMonitor + "' style='background-color:" + color + "' onmouseover='setNavigatingServerGroup(" + groupToWrite.Id + ");if (enableToolTip) {{ posTooltip(); showtip(); }}' onmouseout='hidetip();restoreTooltip();setNavigatingServerGroup(0);'>");
                        writer.Write("<tr height='10'>");

                        writer.Write("<td align='center'>");
                        CheckBox chk = new CheckBox();
                        chk.Attributes.Add("onclick", "checkboxClick('" + groupToWrite.Id + "')");
                        if (selectGroup.IndexOf(";"+groupToWrite.Id.ToString()+";") != -1) 
                        {
                            chk.Checked = true;                            
                        }
                        chk.Enabled = bCheckboxEnable;
                        chk.RenderControl(writer);
                        writer.Write("</td>");
                        ////===================
                        writer.Write("<td align='center'>");
                        writer.Write("<a style='color:blue' href='../gameserver/ServerList.aspx?serverGroupId=");
                        writer.Write(groupToWrite.Id);
                        writer.Write("'>");
                        if (groupToWrite.Name.Length <= 10)
                        {
                            writer.Write(groupToWrite.Name);
                        }
                        else
                        {
                            writer.Write(groupToWrite.Name.Substring(0, 8) + "..");
                        }
                        if (groupToWrite.group_enable == ServerGroup.GroupEnable.Close)
                        {
                            writer.Write("(" + StringDef.Disable + ")");
                        }
                        writer.Write("</a></td>");
                        
                        //2008-7-10 huangbiao ��ʱȡ����ȡ�澯��Ϣ
                        writer.Write("<td align='center'>");
                            //ProgressBar warningProgressBar = new ProgressBar();
                            //warningProgressBar.Width = new Unit(10, UnitType.Pixel);
                            //warningProgressBar.Height = new Unit(16, UnitType.Pixel);
                            ////ColorConverter myColorConverter = new ColorConverter();
                            ////warningProgressBar.BackColor = (Color)myColorConverter.ConvertFromString(color);
                            //if (noWarning)
                            //{
                            //    ColorConverter myColorConverter = new ColorConverter();
                            //    warningProgressBar.BackColor = (Color)myColorConverter.ConvertFromString(color);
                            //    warningProgressBar.BorderColor = (Color)myColorConverter.ConvertFromString(color);
                            //}
                            //else
                            //{
                            //    warningProgressBar.BackColor = Color.Black;
                            //    warningProgressBar.BorderColor = Color.Black;
                            //}
                            //warningProgressBar.BorderStyle = BorderStyle.Solid;
                            //warningProgressBar.BorderWidth = new Unit(1, UnitType.Pixel);
                            //warningProgressBar.ShowIndicator = false;                            
                            //warningProgressBar.RenderControl(writer);

                        writer.Write("</td>");             
                        writer.Write("</tr>");
                        writer.Write("<tr><td colspan='3' align='center'>" + groupToWrite.GroupVer + "</td></tr>");                        
                        writer.Write("<tr>");

                       

                        ////===================
                        //������

                        if (showProgessBar)
                        {

                            //ϸ�ڽ�����
                            writer.Write("<td width='102' colspan='3' style='border: outset 1px #FFFFFF;'>");

                            ProgressBar taskProgress1 = new ProgressBar();
                            taskProgress1.Progress = progressBarValue;
                            taskProgress1.Width = new Unit(100, UnitType.Pixel);
                            taskProgress1.Height = new Unit(6, UnitType.Pixel);
                            taskProgress1.CssClass = "LowLoad";
                            taskProgress1.ProcessedCssClass = "ProgressBarProcessed";
                            taskProgress1.IndicatorCssClass = "ProgressBarIndicator";
                            taskProgress1.CompletedCssClass = "ProgressBarCompleted";

                            if (isDownloading == false)
                            {
                                taskProgress1.BackColor = Color.DarkGray;
                            }
                            else
                            {
                                taskProgress1.ForeColor = Color.Cyan;
                            }
                            taskProgress1.RenderControl(writer);
                            writer.Write("</tr>");
                            writer.Write("<tr>");
                            
                            
                            //�ܽ�����
                            writer.Write("<td width='102' colspan='3' style='border: outset 1px #FFFFFF;'>");
                            
                            ProgressBar taskProgress = new ProgressBar();
                            double nCurTotal = 0;
                            foreach(double nCur in mapIp.Values) 
                            {
                                nCurTotal += nCur;
                            }
                            taskProgress.Progress = nCurTotal*1.0 / (mapIp.Count * 100.0);
                            taskProgress.Width = new Unit(100, UnitType.Pixel);
                            taskProgress.Height = new Unit(9, UnitType.Pixel);
                            taskProgress.CssClass = "HighLoad";// "ProgressBarBorder";
                            taskProgress.ProcessedCssClass = "ProgressBarProcessed";
                            taskProgress.IndicatorCssClass = "ProgressBarIndicator";
                            taskProgress.CompletedCssClass = "ProgressBarCompleted";
                            
                            if (isDownloading == false)
                            {
                                taskProgress.BackColor = Color.DarkGray;
                            }
                            else
                            {
                                taskProgress.ForeColor = Color.Cyan;
                            }
                            taskProgress.RenderControl(writer);
                            writer.Write("</td>");
                            writer.Write("</td>");                          
                        }
                        else
                        {
                            writer.Write("<td></td>");
                        }

                        ////===================

                        writer.Write("</tr>");
                        writer.Write("</table>");
                        writer.Write("</td>");
                    }
                    else
                    {
                        writer.Write("<td " + columnWidth + ">");
                        writer.Write("</td>");
                    }

                    count++;
                }

                writer.Write("</tr>");
            }

            writer.Write("</table>");
            
            writer.Write("</td>");
            writer.Write("</tr>");
            writer.Write("</table>");
        }
    }

    
    private void ProcessGetServerGroupSummary(HttpContext context)
    {
        using (HtmlTextWriter writer = new HtmlTextWriter(context.Response.Output))
        {
            if (context.Request.Params[WebConfig.ParamServerGroupId] != null)
            {
                int serverGroupId = int.Parse(context.Request.Params[WebConfig.ParamServerGroupId]);
                ServerGroup serverGroup = AdminServer.TheInstance.GameServerManager.GetGameServerGroup(serverGroupId);

                if (serverGroup != null)
                {
                    writer.Write("<table width='100%'>");
                    writer.Write("<tr><td style='font-weight:bold; text-align: center; color: Yellow;'>{0}</td></tr>", serverGroup.Name);
                    writer.Write("</table>");

                    writer.Write("<br />");
                    writer.Write("<table width='100%'>");
                    writer.Write("<tr><td style='text-align: center; color: White;'>{0}��{1}</td></tr>", "�����й�����", serverGroup.OfflinePlayer.ToString());
                    writer.Write("<tr><td style='text-align: center; color: White;'>{0}��{1}</td></tr>", "��ʷ�������", serverGroup.MaxPlayer.ToString());
                    writer.Write("<tr><td style='text-align: center; color: White;'>{0}��{1}</td></tr>", "�汾��", serverGroup.GroupVer.ToString());
                    writer.Write("</table>");                    
                    
                    writer.Write("<table width='100%'>");
                    writer.Write("<tr style='color:white'><td align='center'>IP</td><td align='center'>��������</td><td align='center'>״̬</td><td align='center'>˵��</td></tr>");

                    writer.Write("<tr class='TableRowSpacing' />");

                    IList serverIList = serverGroup.List;
                    
                    //��servers����������.Name���Խ�������                          
                    IComparer myComparer = new ByServerNameComparer();
                    ArrayList tempArrayList = ArrayList.Adapter(serverIList);
                    tempArrayList.Sort(myComparer);
                    serverIList = (IList)tempArrayList;

                    foreach (GameServer server in serverIList)
                    {
                        string serverState = String.Empty;
                        string color = "Black";

                        if (server.GameServerState == GameServer.ServerState.Unknown)
                        {
                            serverState = "����Ͽ�";
                            color = "#996633";
                        }
                        else
                        {
                            switch (server.GameServiceState)
                            {
                                case GameServer.ServiceState.Running:
                                    serverState = "����";
                                    color = "#00CC00"; //��ɫ
                                    break;
                                case GameServer.ServiceState.OKStopped:
                                    serverState = "�����ر�";
                                    color = "DarkGray";
                                    break;
                                case GameServer.ServiceState.ErrorStopped:
                                    serverState = "����ֹͣ";
                                    color = "Red";
                                    break;
                                //                            case GameServer.ServiceState.Updating:
                                //                              serverState = "������";
                                //                            color = "MediumOrchid";
                                //                          break;
                                case GameServer.ServiceState.Stopping:
                                    serverState = "�ر���";
                                    color = "DeepSkyBlue";
                                    break;
                                case GameServer.ServiceState.Starting:
                                    serverState = "������";
                                    color = "Gold";
                                    break;

                                default:
                                    break;
                            }
                        }

                        writer.Write("<tr style='color:{0}'><td align='center'>{1}</td><td align='center'>{2}</td><td align='center'>{3}</td><td align='center'>{4}</td></tr>", color, server.IpAddress, server.Name, serverState, String.Empty);
                    }
                    
                    writer.Write("</table>");
                }
            }
        }
    }
    
	

    //��NAME���򣬹�����CreateServerDecribeTableʹ��
    public class ByServerNameComparer : IComparer
    {
        int IComparer.Compare(Object x, Object y)
        {
            string xName = ((GameServer)x).Name;
            string yName = ((GameServer)y).Name;
            return ((new CaseInsensitiveComparer()).Compare(xName, yName));
        }
    }

    //��ID����
    public class ByServerGroupIdComparer : IComparer
    {
        int IComparer.Compare(Object x, Object y)
        {
            int xId = ((ServerGroup)x).Id;
            int yId = ((ServerGroup)y).Id;
            return ((new CaseInsensitiveComparer()).Compare(xId, yId));
        }
    }
}
