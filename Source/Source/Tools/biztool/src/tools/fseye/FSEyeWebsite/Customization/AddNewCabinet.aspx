<%@ Page Language="C#" MasterPageFile="~/Common/Main.master" AutoEventWireup="true" CodeFile="AddNewCabinet.aspx.cs" Inherits="Customization_AddNewCabinet" Title="Untitled Page" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolderTopMessage" Runat="Server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolderMain" Runat="Server">
<asp:UpdatePanel ID="UpdatePanel1" runat="server">
<ContentTemplate>
    <table>
        <tr><td><asp:Literal ID="Literal1" runat="server" Text="���еĻ���" /></td></tr>
        <tr>
            <td>
                <asp:ListBox ID="ListBoxCabinet" runat="server" Rows="20" Width="160px" />
                <asp:LinkButton ID="LinkButtonDeleteCabinet" runat="server" Text="ɾ��ѡ�еĻ���" OnClick="LinkButtonDeleteCabinet_Click" />
            </td>       
        </tr>
        <tr class="TableRowSpacing" />
        <tr>            
            <td>
                <asp:Literal runat="server" ID="Literal2" Text="Ҫ��ӵĻ�������" /><asp:TextBox ID="TextBoxAddCabinet" runat="server" />
                <asp:LinkButton ID="LinkButtonAddCabinet" runat="server" Text="��ӻ���" OnClick="LinkButtonAddCabinet_Click" />
            </td>
        </tr>      
        <tr class="TableRowSpacing" />
        <tr>
            <td>
                <asp:Literal runat="server" ID="Literal3" Text="������������Ϊ��" /><asp:TextBox ID="TextBoxRenameCabinet" runat="server" />
                <asp:LinkButton ID="LinkButtonRenameCabinet" runat="server" Text="������ѡ�еĻ���" OnClick="LinkButtonRenameCabinet_Click" />
            </td>
        </tr>	
    </table>
    
    <br />
    
    <table>
        <tr><td>ע��</td></tr>
        <tr><td>ɾ�����񽫻�Ѹû����·�������Ļ�����Ϣ���</td></tr>
        <tr><td>���������񽫻�ͬʱ���¸û����·�������Ļ�����Ϣ</td></tr>
        <tr class="TableRowSpacing" />
        <tr>
            <td><asp:Label ID="LabelSuccess" runat="server" SkinID="MessageSuccess" /></td>
        </tr>    
        <tr>
            <td><asp:Label ID="LabelOpMsg" runat="server" SkinID="Failure" /></td>
        </tr>
    </table>
</ContentTemplate>
</asp:UpdatePanel>    
</asp:Content>

