//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2007-4
//      File_base        : UpdateManagement
//      File_ext         : cs
//      Author           : Ҧң
//      Description      : ���¹���--��Ӹ��·�����ҳ��
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using FSEye;
using Resources;
using FSEye.Security;

public partial class UpDateManagement_AddUpdateServer : BasePage
{
    protected void Page_Load(object sender, EventArgs e)
    {        
		if (!WebUtil.CheckPrivilege(WebConfig.FunctionUpdateServerAddUpdateServer, OpType.READ, Session))
		{
			Response.Redirect(WebConfig.PageNotEnoughPrivilege, true);
		}

        if (IsPostBack == false)
        {
            CreateUpdateServerTypeDropDownList();
        }

        ClearMessage();
        LabelOpMsg.Visible = false;
    }

    private void CreateUpdateServerTypeDropDownList()
    {
        string text = String.Empty;

        foreach (FTPServer.FTPServerType ftpservertype in Enum.GetValues(typeof(FTPServer.FTPServerType)))
        {
            switch (ftpservertype)
            {
                case FTPServer.FTPServerType.ServerUpdate:
                    text = "����˸���FTP";
                    break;
                case FTPServer.FTPServerType.ClientUpdate:
                    text = "�ͻ��˸���FTP";
                    break;
                case FTPServer.FTPServerType.ClientServerListFile:
                    text = "�ͻ���ServerList����FTP";
                    break;

                default:
                    break;
            }

            ListItem item = new ListItem(text, ((int)ftpservertype).ToString());            
            UpdateServerTypeDropDownList.Items.Add(item);
        }
    }

    protected void Add_Click(object sender, EventArgs e)
    {
        //��Ӹ��·�������Ҫ�ڴ�ҳ������Write��Ȩ��
        if (!WebUtil.CheckPrivilege(WebConfig.FunctionUpdateServerAddUpdateServer, OpType.WRITE, Session))
        {
            LabelOpMsg.Text = "ɾ��������ʧ�ܣ� " + StringDef.NotEnoughPrivilege;
            LabelOpMsg.Visible = true;
            return;
        }
        try
        {
            FTPServer server = new FTPServer(TextBoxServerIpAddress.Text,
                                             int.Parse(TextBoxServerPort.Text),
                                             TextBoxServerUserName.Text,
                                             TextBoxServerPassword.Text,
                                             TextBoxServerComment.Text,
                                             (FTPServer.FTPServerType)Enum.Parse(typeof(FTPServer.FTPServerType), UpdateServerTypeDropDownList.SelectedValue));
            if (TheAdminServer.FTPClient.AddUpdateServer(server))
            {
                ShowMessage(StringDef.OperationSucceed, MessageType.Success);
                Response.Redirect("UpdateServerConfig.aspx");
            }
            else
            {
                throw new Exception();
            }
        }
        catch (FormatException)
        {
            ShowMessage(StringDef.OperationFail+StringDef.Colon, MessageType.Failure);
        }
        catch (Exception)
        {
            ShowMessage(StringDef.OperationFail, MessageType.Failure);
        }

    }
}