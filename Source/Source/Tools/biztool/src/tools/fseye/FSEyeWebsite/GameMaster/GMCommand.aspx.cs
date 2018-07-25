//////////////////////////////////////////////////////////////////////
//
//��ҳ����������Ԥ��д�õ�xmlģ����ִ���ض���GMָ��
//ģ����\FSEyeWebsite\GMCommandTemplate.xml����
//���Ը������������չ��ֻ���޸ĸ�xml�ļ�
//��xml�Ķ��������ϸ��Լ��
//2008.8.18
//
//////////////////////////////////////////////////////////////////////


using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Collections.Generic;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Text;
using Resources;
using FSEye;
using FSEye.Security;
using FSEye.PlugIn;
using FSEye.FS2Lib;
using System.Xml;
using System.Xml.XPath;

public partial class GameMaster_GMCommand : BasePage
{
    public class GMCommandTemplate
    {
        public string cmd;
        public TextBox PlayerNameTextBox;
        public bool isAccountName = false;
        public Panel TemplatePanel;
        public IList<Control> ControlList = new List<Control>();
    }

    IList<GMCommandTemplate> templateList = new List<GMCommandTemplate>();
    string SecurityObjectGameServerGroups = StringDef.AllServers;
    
	protected void Page_Load(object sender, EventArgs e)
	{
        if (!IsPostBack)
        {
            if (!WebUtil.CheckPrivilege(WebConfig.FunctionGameMasterGMCommand, OpType.READ, Session))
            {
                Response.Redirect(WebConfig.PageNotEnoughPrivilege, true);
            }
            
            CreateServerGroupTree();

            //����javascript��ˢ�¹�ѡcheckbox
            TreeViewServerGroup.Attributes.Add("onclick", "OnTreeClick(event)");
        }

        //��GMCommandTemplate.xml����GMָ��ģ�壬���ɶ�Ӧҳ��Ԫ��
        try
        {
            LoadGMCommandTemplate();
        }
        catch (Exception ex)
        {
            LabelOpMsg.Text = ex.Message;
            TableCell templateTableCell = this.TemplateTable.FindControl("TemplateTableCell") as TableCell;
            TemplateTableCell.Controls.Clear();
            return;
        }
	}

    private void LoadGMCommandTemplate()
    {
        int textBoxCount = 100;
        int validatorCount = 100;
        
        TableCell templateTableCell = this.TemplateTable.FindControl("TemplateTableCell") as TableCell;

        XPathNavigator nav;
        XPathDocument docNav;

        docNav = new XPathDocument(WebConfig.WebsiteRootPath + "GMCommandTemplate.xml");
        nav = docNav.CreateNavigator();
        nav.MoveToRoot();
        nav.MoveToFirstChild();

        while (nav.NodeType != XPathNodeType.Element)
            nav.MoveToNext();

        if (nav.LocalName != "GMCommandTemplates")
            throw new Exception("��������Ϊ&lt;GMCommandTemplates&gt;");

        if (nav.HasChildren)
            nav.MoveToFirstChild();
        else
            throw new Exception("&lt;GMCommandTemplates&gt;�±������ӽ��");

        do
        {
            if (nav.NodeType == XPathNodeType.Element)
            {
                if (nav.LocalName != "Template")
                    throw new Exception("&lt;GMCommandTemplates&gt;�µ��ӽ��ֻ����Ϊ&lt;Template&gt;");

                if (nav.HasChildren)
                {
                    nav.MoveToFirstChild();

                    GMCommandTemplate template = new GMCommandTemplate();

                    Panel panel = new Panel();
                    Table table = new Table();

                    string templateName = String.Empty;
                    string cmd = String.Empty;
                    
                    Label desLabel = new Label();                    
                    panel.Controls.Add(desLabel);

                    do
                    {
                        if (nav.LocalName == "TemplateName")
                        {
                            templateName = nav.Value;
                            if (IsPostBack == false)
                                ListBoxOperation.Items.Add(new ListItem(templateName));
                        }
                        else if (nav.LocalName == "Executer")
                        {
                            if ((nav.Value == "Role") || (nav.Value == "Account"))
                            {
                                TableRow row = new TableRow();
                                row.HorizontalAlign = HorizontalAlign.Center;
                                TableCell[] cell = new TableCell[2];
                                for (int i = 0; i < 2; i++)
                                    cell[i] = new TableCell();

                                cell[0].Width = new Unit(20f, UnitType.Percentage);
                                cell[0].Style.Value = "text-align: center;font-weight: bold;color: #FFFFFF;background-color: #5099B3;height: 20px;border-bottom: solid 1px #808080;border-right: solid 1px #808080;";

                                //Role��AccountΨһ��ͬ�ĵط�
                                if (nav.Value == "Role")
                                {
                                    cell[0].Text = "��ɫ��";
                                    template.isAccountName = false;
                                }
                                else
                                {
                                    cell[0].Text = "�˺���";
                                    template.isAccountName = true;
                                }

                                cell[1].Width = new Unit(80f, UnitType.Percentage);

                                TextBox textBox = new TextBox();
                                textBox.ID = "textBox" + textBoxCount.ToString();
                                textBoxCount++;
                                cell[1].Controls.Add(textBox);
                                template.PlayerNameTextBox = textBox;

                                //����������,RequiredFieldValidator
                                RequiredFieldValidator validator = new RequiredFieldValidator();
                                validator.ID = "validator" + validatorCount.ToString();
                                validatorCount++;
                                validator.ControlToValidate = textBox.ID;
                                validator.Display = ValidatorDisplay.None;
                                validator.ErrorMessage = StringDef.RequiredFieldValidatorErrorMsg;
                                validator.SetFocusOnError = true;
                                cell[1].Controls.Add(validator);
                                AjaxControlToolkit.ValidatorCalloutExtender validatorExtender = new AjaxControlToolkit.ValidatorCalloutExtender();
                                validatorExtender.TargetControlID = validator.ID;
                                cell[1].Controls.Add(validatorExtender);

                                row.Cells.AddRange(cell);
                                table.Rows.Add(row);
                            }
                            else if (nav.Value == "GameCenter")
                            {
                                //�������κοؼ���ʲôҲ����,ֻ�Ǳ�֤"GameCenter"Ϊ�Ϸ���ֵ
                            }
                            else
                                throw new Exception("&lt;Executer&gt;��ֵ" + nav.Value + "���Ϸ�,�Ϸ���ֵΪRole,Account��GameCenter");
                        }
                        else if (nav.LocalName == "TemplateCMD")
                        {
                            cmd = nav.Value;
                        }
                        else if (nav.LocalName == "Description")
                        {
                            desLabel.Text = nav.Value + "<br />&nbsp;";
                        }
                        else if (nav.LocalName == "Parameter")
                        {
                            if (nav.HasChildren)
                            {
                                nav.MoveToFirstChild();

                                if (nav.LocalName == "Name")
                                {
                                    TableRow row = new TableRow();
                                    row.HorizontalAlign = HorizontalAlign.Center;
                                    TableCell[] cell = new TableCell[2];
                                    for (int i = 0; i < 2; i++)
                                        cell[i] = new TableCell();

                                    cell[0].Width = new Unit(20f, UnitType.Percentage);
                                    cell[0].Style.Value = "text-align: center;font-weight: bold;color: #FFFFFF;background-color: #5099B3;height: 20px;border-bottom: solid 1px #808080;border-right: solid 1px #808080;";
                                    cell[0].Text = nav.Value;
                                    cell[1].Width = new Unit(80f, UnitType.Percentage);

                                    if (nav.MoveToNext())
                                    {
                                        if (nav.LocalName == "Control")
                                        {
                                            if (nav.HasChildren)
                                            {
                                                nav.MoveToFirstChild();

                                                if (nav.LocalName == "Type")
                                                {
                                                    switch (nav.Value)
                                                    {
                                                        case "TextBox":
                                                            TextBox textBox = new TextBox();
                                                            textBox.ID = "textBox" + textBoxCount.ToString();
                                                            textBoxCount++;
                                                            cell[1].Controls.Add(textBox);
                                                            template.ControlList.Add(textBox);

                                                            //����������,RequiredFieldValidator
                                                            RequiredFieldValidator validator = new RequiredFieldValidator();
                                                            validator.ID = "validator" + validatorCount.ToString();
                                                            validatorCount++;
                                                            validator.ControlToValidate = textBox.ID;
                                                            validator.Display = ValidatorDisplay.None;
                                                            validator.ErrorMessage = StringDef.RequiredFieldValidatorErrorMsg;
                                                            validator.SetFocusOnError = true;
                                                            cell[1].Controls.Add(validator);
                                                            AjaxControlToolkit.ValidatorCalloutExtender validatorExtender = new AjaxControlToolkit.ValidatorCalloutExtender();
                                                            validatorExtender.TargetControlID = validator.ID;
                                                            cell[1].Controls.Add(validatorExtender);

                                                            //TextBox���Ϳ����е��������Զ���
                                                            while (nav.MoveToNext())
                                                            {
                                                                if (nav.LocalName == "MultiLine")
                                                                {
                                                                    if (nav.Value == "True")
                                                                    {
                                                                        textBox.TextMode = TextBoxMode.MultiLine;
                                                                        textBox.Wrap = true;
                                                                    }
                                                                }
                                                                else if (nav.LocalName == "MaxCharCount")
                                                                {
                                                                    if (textBox.TextMode == TextBoxMode.MultiLine)
                                                                    {
                                                                        textBox.Attributes.Add("onkeypress", "return validateMaxLength(this, " + nav.Value + ");");
                                                                        textBox.Attributes.Add("onbeforepaste", "doBeforePaste(this, " + nav.Value + ");");
                                                                        textBox.Attributes.Add("onpaste", "doPaste(this, " + nav.Value + ");");
                                                                    }
                                                                    else
                                                                        textBox.MaxLength = int.Parse(nav.Value);
                                                                }
                                                                else if (nav.LocalName == "Style")
                                                                    textBox.Style.Value = nav.Value;
                                                            }
                                                            break;

                                                        case "IntegerTextBox":
                                                            TextBox integerTextBox = new TextBox();
                                                            integerTextBox.ID = "textBox" + textBoxCount.ToString();
                                                            textBoxCount++;
                                                            cell[1].Controls.Add(integerTextBox);
                                                            template.ControlList.Add(integerTextBox);

                                                            //����������,RequiredFieldValidator
                                                            RequiredFieldValidator integerValidator = new RequiredFieldValidator();
                                                            integerValidator.ID = "validator" + validatorCount.ToString();
                                                            validatorCount++;
                                                            integerValidator.ControlToValidate = integerTextBox.ID;
                                                            integerValidator.Display = ValidatorDisplay.None;
                                                            integerValidator.ErrorMessage = StringDef.RequiredFieldValidatorErrorMsg;
                                                            integerValidator.SetFocusOnError = true;
                                                            cell[1].Controls.Add(integerValidator);
                                                            AjaxControlToolkit.ValidatorCalloutExtender integerValidatorExtender = new AjaxControlToolkit.ValidatorCalloutExtender();
                                                            integerValidatorExtender.TargetControlID = integerValidator.ID;
                                                            cell[1].Controls.Add(integerValidatorExtender);

                                                            //IntegerTextBox���ͣ���CompareValidator�޶�ֻ����������,
                                                            CompareValidator compareValidator = new CompareValidator();
                                                            compareValidator.ID = "validator" + validatorCount.ToString();
                                                            validatorCount++;
                                                            compareValidator.ControlToValidate = integerTextBox.ID;
                                                            compareValidator.Display = ValidatorDisplay.None;
                                                            compareValidator.ErrorMessage = "������д����";
                                                            compareValidator.SetFocusOnError = true;
                                                            compareValidator.Operator = ValidationCompareOperator.DataTypeCheck;
                                                            compareValidator.Type = ValidationDataType.Integer;
                                                            cell[1].Controls.Add(compareValidator);
                                                            AjaxControlToolkit.ValidatorCalloutExtender compareValidatorExtender = new AjaxControlToolkit.ValidatorCalloutExtender();
                                                            compareValidatorExtender.TargetControlID = compareValidator.ID;
                                                            cell[1].Controls.Add(compareValidatorExtender);

                                                            //IntegerTextBox���Ϳ����е��������Զ���
                                                            while (nav.MoveToNext())
                                                            {
                                                                //��CompareValidator�޶�ֵ������ڵ���MinValue
                                                                if (nav.LocalName == "MinValue")
                                                                {
                                                                    string minValue = nav.Value;

                                                                    CompareValidator minValidator = new CompareValidator();
                                                                    minValidator.ID = "validator" + validatorCount.ToString();
                                                                    validatorCount++;
                                                                    minValidator.ControlToValidate = integerTextBox.ID;
                                                                    minValidator.Display = ValidatorDisplay.None;
                                                                    minValidator.ErrorMessage = "�����ֵ������ڵ���" + minValue;
                                                                    minValidator.SetFocusOnError = true;
                                                                    minValidator.Operator = ValidationCompareOperator.GreaterThanEqual;
                                                                    minValidator.Type = ValidationDataType.Integer;
                                                                    minValidator.ValueToCompare = minValue;
                                                                    cell[1].Controls.Add(minValidator);
                                                                    AjaxControlToolkit.ValidatorCalloutExtender minValidatorExtender = new AjaxControlToolkit.ValidatorCalloutExtender();
                                                                    minValidatorExtender.TargetControlID = minValidator.ID;
                                                                    cell[1].Controls.Add(minValidatorExtender);
                                                                }

                                                                //��CompareValidator�޶�ֵ����С�ڵ���MaxValue
                                                                else if (nav.LocalName == "MaxValue")
                                                                {
                                                                    string maxValue = nav.Value;

                                                                    CompareValidator maxValidator = new CompareValidator();
                                                                    maxValidator.ID = "validator" + validatorCount.ToString();
                                                                    validatorCount++;
                                                                    maxValidator.ControlToValidate = integerTextBox.ID;
                                                                    maxValidator.Display = ValidatorDisplay.None;
                                                                    maxValidator.ErrorMessage = "�����ֵ����С�ڵ���" + maxValue;
                                                                    maxValidator.SetFocusOnError = true;
                                                                    maxValidator.Operator = ValidationCompareOperator.LessThanEqual;
                                                                    maxValidator.Type = ValidationDataType.Integer;
                                                                    maxValidator.ValueToCompare = maxValue;
                                                                    cell[1].Controls.Add(maxValidator);
                                                                    AjaxControlToolkit.ValidatorCalloutExtender maxValidatorExtender = new AjaxControlToolkit.ValidatorCalloutExtender();
                                                                    maxValidatorExtender.TargetControlID = maxValidator.ID;
                                                                    cell[1].Controls.Add(maxValidatorExtender);
                                                                }
                                                                else if (nav.LocalName == "Style")
                                                                    integerTextBox.Style.Value = nav.Value;
                                                            }
                                                            break;

                                                        case "DropDownList":
                                                            DropDownList dropDownList = new DropDownList();
                                                            cell[1].Controls.Add(dropDownList);
                                                            template.ControlList.Add(dropDownList);

                                                            while (nav.MoveToNext())
                                                            {
                                                                if (nav.LocalName == "Style")
                                                                    dropDownList.Style.Value = nav.Value;

                                                                //���dropDownList���е�Item
                                                                else if (nav.LocalName == "Item")
                                                                {
                                                                    if (nav.HasChildren)
                                                                    {
                                                                        nav.MoveToFirstChild();

                                                                        ListItem item = new ListItem();

                                                                        do
                                                                        {
                                                                            if (nav.LocalName == "Text")
                                                                                item.Text = nav.Value;
                                                                            else if (nav.LocalName == "Value")
                                                                                item.Value = nav.Value;
                                                                        }
                                                                        while (nav.MoveToNext());

                                                                        if ((item.Text != String.Empty) && (item.Value != String.Empty))
                                                                            dropDownList.Items.Add(item);

                                                                        nav.MoveToParent();
                                                                    }
                                                                }
                                                            }
                                                            break;

                                                        default:
                                                            throw new Exception("&lt;Type&gt;��ֵ" + nav.Value + "���Ϸ�,�Ϸ���ֵΪTextBox,IntegerTextBox��DropDownList");
                                                    }
                                                }

                                                nav.MoveToParent();
                                            }
                                            else
                                                throw new Exception("&lt;Control&gt;�±������ӽ��");
                                        }
                                        else
                                            throw new Exception("&lt;Name&gt;��Ľ��ֻ��Ϊ&lt;Control&gt;");
                                    }
                                    else
                                        throw new Exception("&lt;Parameter&gt;�²���ֻ��&lt;Name&gt;���");

                                    row.Cells.AddRange(cell);
                                    table.Rows.Add(row);
                                }
                                else
                                    throw new Exception("&lt;Parameter&gt;�µĵ�һ��������Ϊ&lt;Name&gt;");

                                nav.MoveToParent();
                            }
                            else
                                throw new Exception("&lt;Parameter&gt;�±������ӽ��");
                        }
                        else
                            throw new Exception("���&lt;" + nav.LocalName + "&gt;���Ϸ�");
                    }
                    while (nav.MoveToNext());

                    panel.Controls.Add(table);

                    Button button = new Button();
                    button.Text = templateName;
                    button.CommandName = templateList.Count.ToString();
                    button.Click += new EventHandler(Button_Click);
                    button.OnClientClick = "if (Page_ClientValidate()){return window.confirm('ȷ��ִ��GMָ����?');}";
                    panel.Controls.Add(button);

                    templateTableCell.Controls.Add(panel);

                    template.TemplatePanel = panel;
                    template.cmd = cmd;

                    templateList.Add(template);

                    nav.MoveToParent();
                }
                else
                    throw new Exception("&lt;Template&gt;�±������ӽ��");                
            }            
        }
        while (nav.MoveToNext());

        if (IsPostBack == false)
            ListBoxOperation.SelectedIndex = 0;

        SetPanelVisible();    
    }

    protected void serverGroupSelectionOptionRadioButtonList_OnSelectedIndexChanged(object sender, EventArgs e)
    {
        if (serverGroupSelectionOptionRadioButtonList.SelectedValue == "TreeView")
        {
            TreeViewServerGroup.Visible = true;
            serverGroupDropDownList.Visible = false;
        }
        else
        {
            TreeViewServerGroup.Visible = false;
            serverGroupDropDownList.Visible = true;
        }
            
    }

    protected void ListBoxOperation_IndexChanged(object sender, EventArgs e)
    {
        SetPanelVisible();
    }

    private void SetPanelVisible()
    {
        int index = ListBoxOperation.SelectedIndex;
        if (index >= 0)
        {
            foreach (GMCommandTemplate template in templateList)
                template.TemplatePanel.Visible = false;

            templateList[index].TemplatePanel.Visible = true;
        }
    }


    void CreateServerGroupTree()
    {
        IList topGroupList = TheAdminServer.GameServerManager.TopServerGroups;

        TreeViewServerGroup.Nodes.Clear();

        TreeNode rootNode = new TreeNode();
        rootNode.Text = SecurityObjectGameServerGroups;
        rootNode.Value = "0";



        foreach (ServerGroup group in topGroupList)
        {
            TreeNode node = CreateServerGroupNode(group);
            if (node != null)
            {
                rootNode.ChildNodes.Add(node);
            }
        }

        TreeViewServerGroup.Nodes.Add(rootNode);
        TreeViewServerGroup.ExpandDepth = 0;
    }

    TreeNode CreateServerGroupNode(ServerGroup group)
    {
        if (group != null)
        {
            if (group.List.Count == 0)
            {
                return null;
            }
            TreeNode node = new TreeNode();
            node.Text = group.Name;
            node.Value = group.Id.ToString();

            if (group.GroupType == ServerGroup.Type.Group)
            {
                foreach (ServerGroup childGroup in group.List)
                {
                    TreeNode childNode = CreateServerGroupNode(childGroup);
                    if (childNode != null)
                    {
                        node.ChildNodes.Add(childNode);
                    }
                }

            }
            else if (group.GroupType == ServerGroup.Type.Server)
            {
                node.NavigateUrl = "~/GameServer/ServerList.aspx?serverGroupId=" + node.Value;
                node.Value = "g" + group.Id.ToString();

                // ��ʾgroup�������server
                //foreach (GameServer childServer in group.List)
                //{
                //    TreeNode childNode = new TreeNode();
                //    childNode.Text = childServer.Name;
                //    childNode.Value = 's' + childServer.Id.ToString();
                //    if (childNode != null)
                //    {
                //        node.ChildNodes.Add(childNode);
                //    }
                //}
            }

            return node;
        }

        return null;
    }
    
    protected void Button_Click(object sender, EventArgs e)
    {
        if (!WebUtil.CheckPrivilege(WebConfig.FunctionGameMasterGMCommand, OpType.EXECUTE, Session))
        {
            Response.Redirect(WebConfig.PageNotEnoughPrivilege, true);
        }

        Button lb = sender as Button;
        if (lb != null)
        {
            string gmCmd = string.Empty;
            string playerName = string.Empty;
            bool isAccountName = false;
            bool isExecutedByGameServer = false;

            int templateIndex = int.Parse(lb.CommandName);

            GMCommandTemplate template = templateList[templateIndex];
            gmCmd = template.cmd;
            if (template.PlayerNameTextBox != null)
                playerName = template.PlayerNameTextBox.Text.Trim();
            isAccountName = template.isAccountName;
            if (playerName != String.Empty)
                isExecutedByGameServer = true;

            //ֻ�д��ڲ���ʱ����Ҫ���ж�ռλ�����滻
            if (template.ControlList.Count > 0)
            {
                string[] arguments = new string[template.ControlList.Count];
                for (int i = 0; i < template.ControlList.Count; i++)
                {
                    Type t = template.ControlList[i].GetType();
                    if (t == typeof(TextBox))
                    {
                        arguments[i] = ((TextBox)(template.ControlList[i])).Text.Trim();
                        arguments[i] = arguments[i].Replace("\r\n", "\\r\\n");
                    }
                    else if (t == typeof(DropDownList))
                    {
                        DropDownList dropDownList = (DropDownList)template.ControlList[i];
                        if (dropDownList.SelectedIndex != -1)
                            arguments[i] = dropDownList.SelectedValue;
                    }
                }

                gmCmd = string.Format(gmCmd, arguments);
            }

            ArrayList serversToGetResultArrayList = new ArrayList();
            
            try
            {
                if (serverGroupSelectionOptionRadioButtonList.SelectedValue == "DropDownList")
                {
                    int serverId = serverGroupDropDownList.SelectedServer.Id;
                    
                    if (GMUtil.ExeGMCommand(CurrentUser.Id, serverId, playerName, isAccountName, gmCmd, isExecutedByGameServer, null, true))
                    {
                        LabelOpMsg.Text = "GMָ��ͳɹ�";

                        serversToGetResultArrayList.Add(AdminServer.TheInstance.GameServerManager.GetGameServer(serverId));                        
                    }
                    else
                    {
                        LabelOpMsg.Text = "GMָ���ʧ��";                      
                        return;                  
                    }
                }
                else if (serverGroupSelectionOptionRadioButtonList.SelectedValue == "TreeView")
                {
                    LabelOpMsg.Text = String.Empty;

                    int totalTargetServer = 0;
                    int successCount = 0;
                    
                    foreach (TreeNode checkedTreeNode in TreeViewServerGroup.CheckedNodes)
                    {                                               
                        try
                        {
                            if (checkedTreeNode.Value.StartsWith("g"))
                            {
                                ServerGroup serverGroup = TheAdminServer.GameServerManager.GetGameServerGroup(int.Parse(checkedTreeNode.Value.Substring(1)));
                                GameServer gameCenter = null;
                                foreach (GameServer gameServer in serverGroup.List)
                                {
                                    if (gameServer.Type == GameServer.ServerType.gamecenter)
                                    {
                                        gameCenter = gameServer;
                                        break;
                                    }
                                }
                                if (gameCenter != null)
                                {
                                    totalTargetServer++;

                                    if (GMUtil.ExeGMCommand(CurrentUser.Id, gameCenter.Id, playerName, isAccountName, gmCmd, isExecutedByGameServer, null, true))
                                    {
                                        serversToGetResultArrayList.Add(gameCenter);

                                        successCount++;                                        
                                    }
                                }
                            }
                        }
                        catch (Exception exception)
                        {
                            Util.DebugLog(exception.ToString());
                        }
                    }

                    LabelOpMsg.Text = "����ָ��: " + successCount.ToString() + "/" + totalTargetServer.ToString();
                }
            }
            catch (Exception ex)
            {
                Util.DebugLog(ex.ToString());
            }
            finally
            {
                LabelOpMsg.Visible = true;
                LabelResultReceivedMsg.Visible = false;

                if (serversToGetResultArrayList.Count != 0)
                {
                    Session["ServersToGetResult"] = serversToGetResultArrayList;

                    Session["ServersToGetResultTotalCount"] = serversToGetResultArrayList.Count;

                    Session["ResultReceivedCount"] = 0;

                    ((HtmlTextArea)(resultMessagePanel.FindControl("resultMessageTextArea"))).Value = string.Empty;

                    if (ShowGMCommandResultTimer.Enabled == false)
                        ShowGMCommandResultTimer.Enabled = true;
                }
            }
        }
    }

    protected void ShowGMCommandResult(object sender, EventArgs e)
    {
        if (Session["ServersToGetResult"] == null)
            return;

        int totalTargetServer = (int)Session["ServersToGetResultTotalCount"];

        ArrayList serversToGetResultArrayList = Session["ServersToGetResult"] as ArrayList;

        ArrayList newServersToGetResultArrayList = new ArrayList();

        foreach (GameServer server in serversToGetResultArrayList)
        {
            string serverResult = GMUtil.GetGMCommandResult(server, CurrentUser.Id);

            if (serverResult != null)
            {                
                ((HtmlTextArea)(resultMessagePanel.FindControl("resultMessageTextArea"))).Value += server.Name + ":\t" + serverResult + "\n\n";
            }
            else
            {
                newServersToGetResultArrayList.Add(server);
            }
        }

        if (newServersToGetResultArrayList.Count == 0)
            ShowGMCommandResultTimer.Enabled = false;
        else
            Session["ServersToGetResult"] = newServersToGetResultArrayList;

        int receivedCount = (int)Session["ServersToGetResultTotalCount"] - newServersToGetResultArrayList.Count;
        Session["ResultReceivedCount"] = receivedCount;

        LabelResultReceivedMsg.Text = "�������: " + receivedCount.ToString() + "/" + totalTargetServer.ToString();
        LabelResultReceivedMsg.Visible = true;        
    }
}