using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using Resources;
using FSEye;

/// <summary>
/// ��Ϸ������������
/// </summary>
public interface IGameServerFilter
{
	string Name
	{
		get;
	}

	bool IsMatch(GameServer server);
}

/// <summary>
/// �չ�����
/// </summary>
public class NullFilter : IGameServerFilter
{
	public string Name
	{
		get { return "�޹���"; }
	}

	public NullFilter()
	{
	}

	public bool IsMatch(GameServer server)
	{
		return true;
	}
}

/// <summary>
/// ����״̬������
/// </summary>
public class ConnectionStatusFilter : IGameServerFilter
{
	bool _isConnected;

	public string Name
	{
		get { return "������״̬����"; }
	}

	public bool IsConnected
	{
		get { return _isConnected; }
		set { _isConnected = value; }
	}

	public ConnectionStatusFilter()
	{
		_isConnected = true;
	}

	public bool IsMatch(GameServer server)
	{
		if (server.IsConnected == _isConnected)
			return true;
		else
			return false;
	}
}

/// <summary>
/// �������������
/// </summary>
public class GameServerGroupFilter : IGameServerFilter
{
	ServerGroup _group;

	public string Name
	{
		get { return "�������������"; }
	}

	public ServerGroup Group
	{
		get { return _group; }
		set { _group = value; }
	}

	public GameServerGroupFilter(ServerGroup group)
	{
		_group = group;
	}

	public bool IsMatch(GameServer server)
	{
		if (_group != null)
		{
			return _group.List.Contains(server);
		}

		return false;
	}
}

/// <summary>
/// �ؼ��ֹ�����
/// </summary>
public class KeywordFilter : IGameServerFilter
{
	string _keyword;

	public string Name
	{
		get { return "���ؼ��ֹ���"; }
	}

	public string Keyword
	{
		get { return _keyword; }
		set { _keyword = value; }
	}

	public KeywordFilter() : this(string.Empty)
	{
	}

	public KeywordFilter(string keyword)
	{
		_keyword = keyword;
	}

	public bool IsMatch(GameServer server)
	{
		if (_keyword != null)
		{
			string lowerCaseKeyword = _keyword.ToLower();
			if (server.Name.ToLower().Contains(lowerCaseKeyword) ||
				server.Comment.ToLower().Contains(lowerCaseKeyword) ||
				(server.Group != null &&
					(server.Group.Name.ToLower().Contains(lowerCaseKeyword) || server.Group.Comment.ToLower().Contains(lowerCaseKeyword))
				))
			{
				return true;
			}
		}

		return false;
	}
}

/// <summary>
/// ���������͹�����
/// </summary>
public class ServerTypeFilter : IGameServerFilter
{
	GameServer.ServerType _serverType;

	public string Name
	{
		get { return "�����������͹���"; }
	}

	public GameServer.ServerType ServerType
	{
		get { return _serverType; }
		set { _serverType = value; }
	}

	public ServerTypeFilter()
	{
	}

	public bool IsMatch(GameServer server)
	{
		return (server.Type == _serverType);
	}
}


