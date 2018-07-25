<%@ page language="C#" masterpagefile="~/Common/Main.master" autoeventwireup="true" inherits="GameInfo_GameDetail, App_Web_d99p0fdh" title="<%$ Resources:StringDef, GameInfo %>" theme="BlazeSky" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolderMain" Runat="Server">

<script type="text/javascript" src="../Script/layout.js"></script>
<script type="text/javascript" src="../Script/control.js"></script>

<script type="text/javascript">
function init() {
	//���ò���Ԫ��
	buildLayout();
	
	//������Ϸ���������
	setGameServerId();
	
	//�տ�ʼ��Ҫ�����Զ�����
	fastLayoutDoTimer = setInterval(fastLayoutDo, 200);
	
	//��һ�θ���������Ϣ
	updateGameBasicInfo();
	updateGamePlayerCount();
	
	//�����Զ�������Ϣ
	setInterval(updateGameBasicInfo, 3000);
	setInterval(updateGamePlayerCount, 3000);	
	
	//�Զ�����
	setInterval(layoutDo, 3000);
}

var fastLayoutDoTimer;
var redoLayoutCount = 0;

function fastLayoutDo() {
	layoutDo();
	redoLayoutCount++;
	if (redoLayoutCount > 20) {
		clearInterval(fastLayoutDoTimer);
	}
}

function buildLayout() {
	layoutSetBase("ParentPanel", 165, 135);
	layoutAdd("DivGameBasicInfo", LAYOUT_MODE_VERTICAL, 0);
	layoutAdd("DivGamePlayerCountFront", LAYOUT_MODE_VERTICAL, 10);
	layoutAdd("DivGamePlayerCountBack", LAYOUT_MODE_OVERLAP, 0);
}

var serverId;

function setGameServerId() {
	serverId = parseInt(document.getElementById("GameServerId").getAttribute("value"), 10);
}

function updateGameBasicInfo() {
	ajax = new sack("GameInfoHandler.ashx");
	ajax.element = "DivGameBasicInfo";
	ajax.runAJAX("op=GetGameBasicInfo&serverId=" + serverId);
}

/* ����������� */
var updateGamePlayerCountFlipDelay = 1000;
var updateGamePlayerCountGraphFlipped = false;

// ����
function updateGamePlayerCount()
{
	ajax = new sack("GameInfoHandler.ashx");
	if (updateGamePlayerCountGraphFlipped === false) {
		ajax.element = 'DivGamePlayerCountBack';
	}
	else {
		ajax.element = 'DivGamePlayerCountFront';
	}
	ajax.runAJAX("op=GetGamePlayerCount&serverId=" + serverId);
	
	setTimeout(flipGamePlayerCountGraph, updateGamePlayerCountFlipDelay);
}

// ��ת
function flipGamePlayerCountGraph() {
	if (updateGamePlayerCountGraphFlipped === false) {
		updateGamePlayerCountGraphFlipped = true;		
		document.all.DivGamePlayerCountFront.style.visibility = 'hidden';
		document.all.DivGamePlayerCountBack.style.visibility = 'visible';
	}
	else {
		updateGamePlayerCountGraphFlipped = false;
		document.all.DivGamePlayerCountFront.style.visibility = 'visible';
		document.all.DivGamePlayerCountBack.style.visibility = 'hidden';		
	}
}
</script>

<input id="GameServerId" type="hidden" value="<%=GetGameServerId() %>" />
<table width="100%">
	<tr>
		<td class="TitleNormal"><asp:Literal ID="literal1" runat="server" Text="<%$ Resources:StringDef, GameInfo %>" /></td>
	</tr>
	<tr>
		<td id="ParentPanel">
			<div id="DivGameBasicInfo"></div>
			<div id="DivGamePlayerCountFront"></div>
			<div id="DivGamePlayerCountBack"></div>
		</td>
	</tr>
</table>
</asp:Content>

