//////////////////////////////////////////////////////////////////////
//
//      Kingsoft 
//
//      Created_datetime : 2008
//      File_base        : GameServer
//      File_ext         : cs
//      Author           : huangbiao
//      Description      : servergroup�б������ؼ�;��ʾΪ�յ���
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

public partial class Common_ServerGroupDropDownList : System.Web.UI.UserControl
{
	/// <summary>
	/// ѡ�з�������ı��¼�
	/// </summary>
	public event EventHandler SelectedServerGroupChanged;

	public bool AutoPostBack
	{
		get { return DropDownListServerGroup.AutoPostBack; }
		set { DropDownListServerGroup.AutoPostBack = value; }
	}

	ServerGroup.Type _groupType = ServerGroup.Type.Server;

	/// <summary>
	/// ������������
	/// </summary>
	public ServerGroup.Type GroupType
	{
		get { return _groupType; }
		set { _groupType = value; }
	}

    public bool Enabled
    {
        get { return DropDownListServerGroup.Enabled; }
        set { DropDownListServerGroup.Enabled = value; }
    }

	/// <summary>
	/// ѡ�еķ�������
	/// </summary>
	public ServerGroup SelectedServerGroup
	{
		get
		{
			try
			{
				int groupId = int.Parse(DropDownListServerGroup.SelectedValue);
				return AdminServer.TheInstance.GameServerManager.GetGameServerGroup(groupId);
			}
			catch (Exception)
			{
				return null;
			}
		}

		set
		{
			if (value != null)
			{
				int groupId = value.Id;
				for (int i = 0; i < DropDownListServerGroup.Items.Count; i++)
				{
					ListItem item = DropDownListServerGroup.Items[i];
					if (item.Value == groupId.ToString())
					{
						DropDownListServerGroup.SelectedIndex = i;
						break;
					}
				}                
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
		DropDownListServerGroup.Items.Clear();
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
            LastGroup(group, "/", false);
        }
		
        if(DropDownListServerGroup.Items.Count!=0)DropDownListServerGroup.SelectedIndex = 0;
	}

    public void ListEmptyServerGroups()
    {
        DropDownListServerGroup.Items.Clear();
        
        IList topGroupList = AdminServer.TheInstance.GameServerManager.TopServerGroups;
        foreach (ServerGroup group in topGroupList)
        {
            LastGroup(group, "/", true);
        }

        if (DropDownListServerGroup.Items.Count != 0) DropDownListServerGroup.SelectedIndex = 0;
    }

	protected void DropDownListServerGroup_SelectedIndexChanged(object sender, EventArgs e)
	{
		if (this.SelectedServerGroupChanged != null)
		{
			this.SelectedServerGroupChanged(this, null);
		}
	}

    //ѡ�����һ����
    private void LastGroup(ServerGroup group, string groupPath, bool onlyShowEmptyGroup)
    {
        if (group != null)
        {
            if (group.GroupType == ServerGroup.Type.Group)
            {
                if (group.List.Count == 0)
                {
                    DropDownListServerGroup.Items.Add(new ListItem(groupPath + group.Name, group.Id.ToString()));
                }
                else
                {
                    foreach (ServerGroup childGroup in group.List)
                    {
                        LastGroup(childGroup, groupPath + group.Name.ToString() + "/", onlyShowEmptyGroup);
                    }
                }
            }
            else if (group.GroupType == ServerGroup.Type.Server)
            {
                if ((onlyShowEmptyGroup) && (group.List.Count != 0))
                    return;                

                DropDownListServerGroup.Items.Add(new ListItem(groupPath + group.Name, group.Id.ToString()));                
            }
        }
    }
}
