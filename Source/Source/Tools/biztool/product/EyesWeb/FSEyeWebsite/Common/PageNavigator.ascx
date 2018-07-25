<%@ control language="C#" autoeventwireup="true" inherits="Common_PageNavigator, App_Web_1ceayx6e" %>

<asp:Panel ID="PanelMain" runat="server">
	<table>
		<tr>
			<td>
				<ul>
					<li>
						<asp:Button ID="ButtonFirstPage" runat="server" Text="��һҳ" OnClick="ButtonFirstPage_Click" /></li>
					<li>
						<asp:Button ID="ButtonPerviousPage" runat="server" Text="��һҳ" OnClick="ButtonPreviousPage_Click" /></li>
					<li>
						<asp:DropDownList ID="DropDownListPage" runat="server" OnSelectedIndexChanged="DropDownListPage_SelectedIndexChanged" AutoPostBack="true" />
					<li>
						<asp:Button ID="ButtonNextPage" runat="server" Text="��һҳ" OnClick="ButtonNextPage_Click" /></li>
					<li>
						<asp:Button ID="ButtonLastPage" runat="server" Text="���ҳ" OnClick="ButtonLastPage_Click" /></li>
				</ul>
			</td>
		</tr>
	</table>	
</asp:Panel>
