<%@ Page Language="C#" MasterPageFile="~/Common/Main.master" AutoEventWireup="true"
	CodeFile="ServerList.aspx.cs" Inherits="GameServer_ServerList" Title="<%$ Resources:StringDef, GameServerList %>" %>
<%@ Register TagPrefix="Blaze" TagName="ServerGroupDropDownList" Src="~/Common/ServerGroupDropDownList.ascx" %>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolderTopMessage" runat="server">
	<div style="width: 13px; height: 10px; background-color: #00FF00; border: solid 1px #FFFFFF;">
	</div>
</asp:Content>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolderMain" runat="Server">

<script type="text/javascript">

var updateGameServerListAjax = new sack("GameServerHandler.ashx");
var updateGameServerSummaryAjax = new sack("GameServerHandler.ashx");
var updatingGameServerList = false;
var updatingGameSummary = false;
var updateGameServerSummaryId = 0;
var startGameServerAjax = new sack("GameServerHandler.ashx");
var stopGameServerAjax = new sack("GameServerHandler.ashx");

var serverGroupId = "";

var bShortShow="FALSE";

//my code//
//++

var op = ""     //��Server�����Ĳ���������function buttonClick()�����û���DropDownList�е�ѡ��������
var intervalID = "";    //�洢setInterval()�����ص�ID���ڵ���clearInterval()ʱ���õ�

//function init()��ҳ��Load��ʱ�򱻵���
function init() {
    document.all.serverTypeDropDownList.style.visibility = "hidden";
    document.all.ctl00_ContentPlaceHolderMain_serverGroupDropDownList_DropDownListServerGroup.style.visibility = "hidden";	
    
    serverGroupId = getQueryString( 'serverGroupId' );
    
    if (serverGroupId != "")
    {
         document.all.filterDropDownList.value = "filterByServerGroup";
         document.all.ctl00_ContentPlaceHolderMain_serverGroupDropDownList_DropDownListServerGroup.value = serverGroupId;
         document.all.ctl00_ContentPlaceHolderMain_serverGroupDropDownList_DropDownListServerGroup.style.visibility = "visible";	
         buttonClick();
    }
    else
    {
	    updateGameServerList();
	}
}

//����������ǰ����function init()����updateGameServerList();֮�����䣬
//��������init�е�setInterval()�޷���clearInterval()�����
//���Խ��������init���
if (serverGroupId == "")
{
    intervalID = setInterval(updateGameServerList, 3000);
}
setInterval(updateGameServerSummary, 1000);	

//++
//my code//

function updateGameServerList() {
	if (!updatingGameServerList)
	{
		updatingGameServerList = true;
		updateGameServerListAjax.reset();
		updateGameServerListAjax.element = "DivGameServerList";
		updateGameServerListAjax.runAJAX("op=GetGameServerList");		
		updateGameServerListAjax.onCompletion = new Function("updatingGameServerList=false;hideTopMessage(5);");
		showTopMessage(10);
	}
}

function updateGameServerSummary() {
	if (updateGameServerSummaryId && !updatingGameSummary)
	{
		updatingGameSummary = true;
		updateGameServerSummaryAjax.reset();
		updateGameServerSummaryAjax.element = "tooltip";
		updateGameServerSummaryAjax.runAJAX("op=GetGameServerSummary&serverId=" + updateGameServerSummaryId + "&short=" + bShortShow);
		updateGameServerSummaryAjax.onCompletion = new Function("updatingGameSummary=false");
	}
}

function setNavigatingGameServer(serverId) {
	updateGameServerSummaryId = serverId;
	bShortShow="FALSE";
}

function setNavigatingGameServerWithShorShow(serverId)
{
    updateGameServerSummaryId = serverId;
    bShortShow= "TRUE";
}


//my code//
//++

//�����Ѿ����ɵ�op��Server
function sendOp() {
	if (!updatingGameServerList)
	{	
	    updatingGameServerList = true;
	    updateGameServerListAjax.reset();
	    updateGameServerListAjax.element = "DivGameServerList";
	    updateGameServerListAjax.runAJAX(op);		
	    updateGameServerListAjax.onCompletion = new Function("updatingGameServerList=false;hideTopMessage(5);");
	    showTopMessage(10);
    }
} 

//��OK��ť����ʱִ�У�����op�ַ����������ǰ��ˢ�£�
//����sendOp()����ˢ��
function buttonClick() {
      
    //��ʲô��ʽ����?
    var selectedFilter = document.all.filterDropDownList.value;
    
    //��ʲô����״̬���ˣ�
    var selectedConnectionStatus = document.all.showByConnectionDropDownList.value;
    
    //Ϊ"�޹���"
    if (selectedFilter == "noFilter")
    {        
        if (selectedConnectionStatus == "showAll")
        {
	        clearInterval(intervalID);
            updateGameServerList();
            intervalID = setInterval(updateGameServerList, 3000);
        }
        else if (selectedConnectionStatus == "showDisconnectedOnly")
        {
	        clearInterval(intervalID);
	        op = "op=GetGameServerListShowDisconnectedOnly"; 
        	sendOp();	                   
            intervalID = setInterval(sendOp, 3000);            
        }
        else if (selectedConnectionStatus == "showConnectedOnly")
        {
	        clearInterval(intervalID);
	        op = "op=GetGameServerListShowConnectedOnly"; 
        	sendOp();	                   
            intervalID = setInterval(sendOp, 3000);          
        }        
    }
    //Ϊ"�����������͹���"
    else if (selectedFilter == "filterByServerType")
    {
    
        var selectedServerType = document.all.serverTypeDropDownList.value;
        if (selectedConnectionStatus == "showAll")
        {
            op = "op" + "=" + "GetGameServerListByType_" + selectedServerType;
        }
        else if (selectedConnectionStatus == "showDisconnectedOnly")
        {
            op = "op" + "=" + "GetGameServerListByTypeShowDisconnectedOnly_" + selectedServerType;            
        }
        else if (selectedConnectionStatus == "showConnectedOnly")
        {
            op = "op" + "=" + "GetGameServerListByTypeShowConnectedOnly_" + selectedServerType;
        }
	    clearInterval(intervalID);
	    sendOp();
	    intervalID = setInterval(sendOp, 3000); 
	    
    }
    //Ϊ"�������������"
    else if (selectedFilter == "filterByServerGroup")
    {
        var selectedGroupId = document.all.ctl00_ContentPlaceHolderMain_serverGroupDropDownList_DropDownListServerGroup.value;
 
        if (selectedConnectionStatus == "showAll")
        {
            op = "op" + "=" + "GetGameServerListByGroup_" + selectedGroupId;
        }
        else if (selectedConnectionStatus == "showDisconnectedOnly")
        {
            op = "op" + "=" + "GetGameServerListByGroupShowDisconnectedOnly_" + selectedGroupId;            
        }
        else if (selectedConnectionStatus == "showConnectedOnly")
        {
            op = "op" + "=" + "GetGameServerListByGroupShowConnectedOnly_" + selectedGroupId;
        }
	    clearInterval(intervalID);
	    sendOp();
	    intervalID = setInterval(sendOp, 3000);         
        
    }           
}
function showResult()
{
    var msg = document.getElementById('divMsg').innerHTML;
    if(msg && msg.length > 0)
    {
        window.alert(msg);
    }
}

function startGameServer(serverId){
    if(window.confirm("<%= Resources.StringDef.MsgOperationConfirm %>"))
    {
        document.getElementById('divMsg').innerHTML = "";
        startGameServerAjax.reset();
        startGameServerAjax.element = "divMsg";
        startGameServerAjax.runAJAX("op=StartGameServer&serverId=" + serverId);
        startGameServerAjax.onCompletion = showResult;
    }
}

function stopGameServer(serverId){
    if(window.confirm("<%= Resources.StringDef.MsgOperationConfirm %>"))
    {
        document.getElementById('divMsg').innerHTML = "";
        stopGameServerAjax.reset();
        stopGameServerAjax.element = "divMsg";
        stopGameServerAjax.runAJAX("op=StopGameServer&serverId=" + serverId);
        stopGameServerAjax.onCompletion = showResult;
    }
}

//��filterDropDownList��ѡ�е�����ı�ʱ������
function filterDropDownListChanged() {
    
    var selectedFilter = document.all.filterDropDownList.value;
        
    if (selectedFilter == "noFilter")
    {
        document.all.serverTypeDropDownList.style.visibility = "hidden";
        document.all.ctl00_ContentPlaceHolderMain_serverGroupDropDownList_DropDownListServerGroup.style.visibility = "hidden";
    }
    else if (selectedFilter == "filterByServerType")
    {
        document.all.serverTypeDropDownList.style.visibility = "visible";
        document.all.ctl00_ContentPlaceHolderMain_serverGroupDropDownList_DropDownListServerGroup.style.visibility = "hidden";               
    }
    else if (selectedFilter == "filterByServerGroup")
    {
        document.all.ctl00_ContentPlaceHolderMain_serverGroupDropDownList_DropDownListServerGroup.style.visibility = "visible";
        document.all.serverTypeDropDownList.style.visibility = "hidden";        
    }
}

//����������ڻ�ô���ҳ��Ĳ���
function getQueryString( name )
{  
    name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");  
    var regexS = "[\\?&]"+name+"=([^&#]*)";  
    var regex = new RegExp( regexS );  
    var results = regex.exec( window.location.href );  
    if( results == null )    
        return "";  
    else    
        return results[1];
}


//++
//my code//

</script>
<div id='divMsg' style="display:none;"></div>
	<table width="100%">
		<tr>
			<td align="center"><asp:Label runat="server" Text="<%$ Resources:StringDef, GameServerList %>" SkinID="Title" /></td>
		</tr>
		
		<tr align="center">
		    <td>
		        <table>
		            <tr>
		                <td>
		                    <select id="filterDropDownList"  onchange="filterDropDownListChanged();">
		                    <option value="noFilter">�޹���</option>
		                    <option value="filterByServerType">�����������͹���</option>
		                    <option value="filterByServerGroup">�������������</option>
		                    </select>		          		                
		                </td>		                
		                
		                <!--
		                <td>
		                    <input type="checkbox" id="showConnectedOnlyCheckBox" />
		                    <label for = "showConnectedOnlyCheckBox">ֻ��ʾ�����ӷ�����</label>
		                </td>   
		                -->
		                
		                <td>
		                    <select id="showByConnectionDropDownList">
		                        <option value="showAll"><asp:Literal ID="Literal8" runat="server" Text="��ʾ���з�����" /></option>
		                        <option value="showDisconnectedOnly"><asp:Literal ID="Literal9" runat="server" Text="ֻ��ʾδ���ӷ�����" /></option>
		                        <option value="showConnectedOnly"><asp:Literal ID="Literal10" runat="server" Text="ֻ��ʾ�����ӷ�����" /></option>
		                    </select>
		                </td>
		            
		                <td>
		                    <select id="serverTypeDropDownList">
		                        <option value="unidentified"><asp:Literal ID="Literal1" runat="server" Text="<%$ Resources:StringDef, ServerTypeUnidentified %>" /></option>
		                        <option value="gameserver"><asp:Literal ID="Literal2" runat="server" Text="<%$ Resources:StringDef, ServerTypeGameServer %>" /></option>
		                        <option value="databaseserver"><asp:Literal ID="Literal3" runat="server" Text="<%$ Resources:StringDef, ServerTypeDatabaseServer %>" /></option>
		                        <option value="bishop"><asp:Literal ID="Literal4" runat="server" Text="<%$ Resources:StringDef, ServerTyperBishop %>" /></option>
		                        <option value="goddess"><asp:Literal ID="Literal5" runat="server" Text="<%$ Resources:StringDef, ServerTyperGoddess %>" /></option>
		                        <option value="gamecenter"><asp:Literal ID="Literal6" runat="server" Text="<%$ Resources:StringDef, ServerTyperGameCenter %>" /></option>
		                        <option value="paysys"><asp:Literal ID="Literal7" runat="server" Text="<%$ Resources:StringDef, ServerTyperPaysys %>" /></option>
		                        <option value="logserver"><asp:Literal ID="Literal11" runat="server" Text="<%$ Resources:StringDef, ServerTypeLogServer %>" /></option>
		                    </ select>
		                </td>
		                
		                <td>
			                <Blaze:ServerGroupDropDownList ID="serverGroupDropDownList" runat="server" />		    
		                </td> 
		   
		                <td>
		                    <input type="button" value="OK" onclick="buttonClick();" />
		                </td>		                
		            </tr>
		        </table>
		    </td>		    		    		   
		</tr>		
		
		<tr>
			<td>
				<div id="DivGameServerList" style="width: 100%" class="DivNormal">
				</div>
			</td>
		</tr>
		<tr class="TableRowSpacing" />
		<tr>
			<td align="center">
				<asp:HyperLink runat="server" NavigateUrl="ControlGame.aspx" Text="<%$ Resources:StringDef, ControlGame %>" />
				<asp:HyperLink runat="server" NavigateUrl="AddAndDelete.aspx" Text="<%$ Resources:StringDef, AddAndDelete %>" />
				<asp:HyperLink runat="server" NavigateUrl="FileManagement.aspx" Text="<%$ Resources:StringDef, FileManagement %>" />
				<asp:HyperLink runat="server" NavigateUrl="AlertConfig.aspx" Text="<%$ Resources:StringDef, AlertConfig %>" />
			</td>
		</tr>
	</table>
</asp:Content>
