//////////////////////////////////////////////////////////////////////
//
//      Kingsoft 
//
//      Created_datetime : 2008
//      File_base        : GameServer
//      File_ext         : cs
//      Author           : huangbiao
//      Description      : servergroup�б������ؼ�;��ʾ����gamecenter����
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

public partial class Common_DropDownListGameCenter : System.Web.UI.UserControl
{
    /// <summary>
	/// ѡ�з�������ı��¼�
	/// </summary>
    public event EventHandler SelectedServerGroupChanged;

    public bool AutoPostBack
    {
        get { return GameCenterList.AutoPostBack; }
        set { GameCenterList.AutoPostBack = value; }
    }


    protected void Page_Load(object sender, EventArgs e)
    {

    }

    public bool Enabled
    {
        get { return GameCenterList.Enabled; }
        set { GameCenterList.Enabled = value; }
    }

    public string SelectedValue
    {
        get
        {
            return GameCenterList.SelectedValue;
        }
        set
        {
            GameCenterList.SelectedValue = value;
        }
    }

    /// <summary>
    /// ѡ�еķ�������
    /// </summary>
    public GameServer SelectedServer
    {
        get
        {
            try
            {
                int serverID = int.Parse(GameCenterList.SelectedValue);
                return AdminServer.TheInstance.GameServerManager.GetGameServer(serverID);
            }
            catch (Exception)
            {
                return null;
            }
        }
    }

    /// <summary>
    /// ˢ��
    /// </summary>
    public void Refresh()
    {
        ListServerGroups();
    }

    protected void Page_Init(object sender, EventArgs e)
    {
        if (!IsPostBack)
            ListServerGroups();
    }

    void ListServerGroups()
    {
        GameCenterList.Items.Clear();
        //IList serverGroups = AdminServer.TheInstance.GameServerManager.ServerGroups;
        //foreach (ServerGroup group in serverGroups)
        //{
        //    if (group.GroupType == ServerGroup.Type.Server || (group.List.Count == 0 && group.GroupType == ServerGroup.Type.Group))
        //    {
        //        DropDownListServerGroup.Items.Add(new ListItem(group.Name, group.Id.ToString()));
        //    }
        //}

        IList topGroupList = AdminServer.TheInstance.GameServerManager.TopServerGroups;
        foreach (ServerGroup group in topGroupList)
        {
            LastGroup(group);
        }



        if (GameCenterList.Items.Count != 0) GameCenterList.SelectedIndex = 0;
    }

    protected void GameCenterList_SelectedIndexChanged(object sender, EventArgs e)
    {
        if (this.SelectedServerGroupChanged != null)
        {
            this.SelectedServerGroupChanged(this, null);
        }
    }

    //ѡ�����һ���麬��gamecenter����
    private void LastGroup(ServerGroup group)
    {
        if (group != null)
        {
            if (group.GroupType == ServerGroup.Type.Group)
            {
                if (group.List.Count == 0)
                {
                    //GameCenterList.Items.Add(new ListItem(group.Name, group.Id.ToString()));
                }
                else
                {
                    foreach (ServerGroup childGroup in group.List)
                    {
                        LastGroup(childGroup);
                    }
                }
            }
            else if (group.GroupType == ServerGroup.Type.Server)
            {
                foreach (GameServer childServer in group.List)
                {
                    //GameServer server = AdminServer.TheInstance.GameServerManager.GetGameServer(childServer.Id);
                    if (childServer.Type == GameServer.ServerType.gamecenter)
                    {
                        //��ʾ����,valueΪ������gamecenter.id
                        GameCenterList.Items.Add(new ListItem(group.Name, childServer.Id.ToString()));
                        break;
                    }
                }
                
            }
        }
    }
}
