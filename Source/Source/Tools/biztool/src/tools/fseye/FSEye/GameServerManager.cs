//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2006-8
//      File_base        : GameServerManager
//      File_ext         : cs
//      Author           : ������
//      Description      : ��Ϸ������������
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Data;
using System.IO;
using System.Net;
using System.Net.Sockets;
using FSEye.Security;
using FSEye.Net;
using FSEye.DAL;
using FSEye.PlugIn;
using FSEye.Cryptography;
using FSEye.DllWrap;
using System.Resources;
using System.Security.Cryptography;
using System.Diagnostics;

namespace FSEye
{
	#region GameServerManager
	/// <summary>
	/// ��Ϸ��������������
	/// </summary>
	/// <author>������</author>
	public class GameServerManager : ISecurity, IMessageHandler, IDisposable
	{
		static Random s_random = new Random();

		public IList _gameServerList;

		IList _gameServerGroupList;

        IList _serverRegionList;

		IList _topServerGroupList;

        IList _cabinetList;

		IList _authenticationList;

		System.Timers.Timer _systemTickTimer;

        System.Timers.Timer _maxPlayerTimer;

        System.Timers.Timer _GroupPlayerCountTimer;

		private const int SystemTickPeriod = 1000;

        private const int PlayerTickPeriod = 60000;

		bool _dispose = false;

		bool _running = false;

		bool _isConfigingGuard = false;

		IList _configGuardServerList;

		FSEyeObject _securityObject;

        private ServerMap _serverMap;

        public ServerRegion RootRegion;

        public ServerRegion DirectUnderRegion;

        DateTime _systemStartTime;

		public FSEyeObject SecurityObject
		{
			get { return _securityObject; }
			set { _securityObject = value; }
		}        

		/// <summary>
		/// �������б�
		/// </summary>
		public IList Servers
		{
			get
			{
				//User user = Thread.GetData(Thread.GetNamedDataSlot(SystemConfig.User)) as User;
				//AdminServer.TheInstance().SecurityManager.Get("/FSEye/System/GameServerManager/GameServerList").CheckPrivilege(user, OpType.READ);				
				return _gameServerList;
			}
		}

		/// <summary>
		/// ���з�������
		/// </summary>
		public IList ServerGroups
		{
			get { return _gameServerGroupList; }
		}

		/// <summary>
		/// �����������
		/// </summary>
		public IList TopServerGroups
		{
			get { return _topServerGroupList; }
		}

        public IList CabinetList
        {
            get { return _cabinetList; } 
        }

		/// <summary>
		/// ��֤�б�
		/// </summary>
		public IList AuthenticationList
		{
			get { return _authenticationList; }
		}

		/// <summary>
		/// �Ƿ�����Guard
		/// </summary>
		public bool IsConfigGuard
		{
			get { return _isConfigingGuard; }
			set
			{
				_isConfigingGuard = value;
				//if (_isConfigingGuard != value)
				//{
				//    _isConfigingGuard = value;

				//    if (_isConfigingGuard)
				//    {
				//    //    Thread listenThread = new Thread(new ThreadStart(ListenForConfigGuardReturn));
				//    //    listenThread.Priority = ThreadPriority.Lowest;
				//    //    listenThread.Start();

				//        //Thread configThread = new Thread(new ThreadStart(ProcessConfigGuard));
				//        //configThread.Priority = ThreadPriority.Lowest;
				//        //configThread.Start();
				//    }
				//}
			}
		}

        public ServerMap ServerMap
        {
            get { return _serverMap; }
        }

		public GameServerManager()
		{
			_gameServerList = ArrayList.Synchronized(new ArrayList());
			_gameServerGroupList = ArrayList.Synchronized(new ArrayList());
            _serverRegionList = ArrayList.Synchronized(new ArrayList());
			_topServerGroupList = ArrayList.Synchronized(new ArrayList());
			_authenticationList = ArrayList.Synchronized(new ArrayList());
			_configGuardServerList = ArrayList.Synchronized(new ArrayList());
            _serverMap = new ServerMap();
			_systemTickTimer = new System.Timers.Timer();
			_systemTickTimer.Interval = SystemTickPeriod;
			Thread configThread = new Thread(new ThreadStart(ProcessConfigGuard));
			configThread.Priority = ThreadPriority.Lowest;
			configThread.Start();

            //��ʱͳ�Ƶ����������
            _maxPlayerTimer = new System.Timers.Timer();
            _maxPlayerTimer.Interval = SystemConfig.Current.GroupPlayerCountKickInterval;
            _maxPlayerTimer.Elapsed += new System.Timers.ElapsedEventHandler(_maxPlayerTimer_Tick);
            
            //��ʱͳ��ÿ������������������������浽�ļ�
            _GroupPlayerCountTimer = new System.Timers.Timer();
            _GroupPlayerCountTimer.Interval = SystemConfig.Current.GroupPlayerCountKickInterval;
            _GroupPlayerCountTimer.Elapsed += new System.Timers.ElapsedEventHandler(GroupPlayerCount2File);

            _systemStartTime = DateTime.Now;
            _cabinetList = ArrayList.Synchronized(new ArrayList());
		}

        protected void _maxPlayerTimer_Tick(object sender, EventArgs e)
        {
            try
            {
                //��ȡ�����������
                int rootPlayersCount = 0;
                int directGroupPlayersCount = 0;
                int rootOfflineCount = 0;
                int directGroupOfflineCount = 0;

                if (_systemStartTime.Date != DateTime.Now.Date)
                {
                    //ÿ�����¼����������
                    RootRegion.MaxPlayer = 0;
                    DirectUnderRegion.MaxPlayer = 0;

                    foreach (ServerGroup group in _topServerGroupList)
                    {
                        InitMaxPlayer(group);
                    }

                    _systemStartTime = DateTime.Now;
                }

                for (int i = 0; i < _gameServerList.Count; i++)
                {
                    GameServer server = _gameServerList[i] as GameServer;

                    //��gc����һ����ĺ���������
                    if (server.Type == GameServer.ServerType.gamecenter)
                    {
                        int groupOfflineCount = 0;
                        int groupNowPlayer = 0;

                        if (server.GameServiceState == GameServer.ServiceState.Running)
                        {
                            groupNowPlayer = (int)server.GetPlugInData(AdminServer.TheInstance.AutomationManager.SecurityObject.Id, LordControl.PlugInGuid, LordControl.DataKeyCurrentPlayerCount);
                        }

                        //���±���Ŀǰ����
                        server.Group.NowPlayers = groupNowPlayer;
                        if (groupNowPlayer > server.Group.MaxPlayer)
                        {
                            server.Group.SetGroupMaxPlays(groupNowPlayer);
                        }

                        //�����й�����
                        foreach (GameServer svr in server.Group.List)
                        {
                            if (svr.Type == GameServer.ServerType.gameserver && svr.GameServiceState == GameServer.ServiceState.Running)
                            {
                                //�����й�����������ѯgs
                                groupOfflineCount += (int)svr.GetPlugInData(AdminServer.TheInstance.AutomationManager.SecurityObject.Id, LordControl.PlugInGuid, LordControl.DataKeyCurrentPlayerOfflineCount);
                            }
                        }

                        server.Group.OfflinePlayer = groupOfflineCount;
                        rootPlayersCount += groupNowPlayer;
                        rootOfflineCount += groupOfflineCount;
                    }
                }

                foreach (ServerGroup group in _topServerGroupList)
                {
                    if (group.GroupType == ServerGroup.Type.Group)
                    {
                        //�м�����
                        ChildGroupPlayCount(group);
                        ChildGroupOfflineCount(group);
                    }
                    else
                    {
                        //directUnder
                        directGroupPlayersCount += group.NowPlayers;
                        directGroupOfflineCount += group.OfflinePlayer;
                    }

                }


                //root
                RootRegion.NowPlayers = rootPlayersCount;
                RootRegion.OfflinePlayer = rootOfflineCount;
                if (rootPlayersCount > RootRegion.MaxPlayer)
                {
                    RootRegion.SetRegionMaxPlays(rootPlayersCount);
                }

                //ֱ����
                DirectUnderRegion.NowPlayers = directGroupPlayersCount;
                DirectUnderRegion.OfflinePlayer = directGroupOfflineCount;

                if (directGroupPlayersCount > DirectUnderRegion.MaxPlayer)
                {
                    DirectUnderRegion.SetRegionMaxPlays(directGroupPlayersCount);
                }

                //TODO:����?

            }
            catch (Exception ex)
            {
                Util.DebugLog("_maxPlayerTimer_Tick: " + ex.Message.ToString());
            }
        }


        private void InitMaxPlayer(ServerGroup group)
        {
            if (group == null) return;

            group.MaxPlayer = 0;

            if (group.List.Count == 0 || group.GroupType == ServerGroup.Type.Server) return;

            foreach (ServerGroup childGroup in group.List)
            {
                if (childGroup.GroupType == ServerGroup.Type.Server)
                {
                    childGroup.MaxPlayer = 0;
                }
                else
                {
                    InitMaxPlayer(childGroup);
                }
            }
        }

        private int ChildGroupPlayCount(ServerGroup group)
        {
            if (group==null) return 0;

            int midGroupPlayCount = 0;            

            foreach (ServerGroup childGroup in group.List)
            {
                if (childGroup.GroupType == ServerGroup.Type.Server)
                {
                    midGroupPlayCount += childGroup.NowPlayers;
                }
                else
                {
                    midGroupPlayCount  += ChildGroupPlayCount(childGroup);
                }
            }

            if (midGroupPlayCount > group.MaxPlayer)
            {
                group.SetGroupMaxPlays(midGroupPlayCount);
            }

            group.NowPlayers = midGroupPlayCount;
            return midGroupPlayCount;
        }

        private int ChildGroupOfflineCount(ServerGroup group)
        {
            if (group == null) return 0;

            int midGroupOfflineCount = 0;

            foreach (ServerGroup childGroup in group.List)
            {
                if (childGroup.GroupType == ServerGroup.Type.Server)
                {
                    midGroupOfflineCount += childGroup.OfflinePlayer;
                }
                else
                {
                    midGroupOfflineCount += ChildGroupPlayCount(childGroup);
                }
            }

           
            group.OfflinePlayer = midGroupOfflineCount;
            return midGroupOfflineCount;
        }


		public void Startup()
		{
			lock (this)
			{
				if (!_running)
				{
					_securityObject = AdminServer.TheInstance.SecurityManager.Get("/FSEye/System/GameServerManager");

					_configGuardServerList.Clear();

					_authenticationList.Clear();

					//������Ϸ������
					LoadGameServers();

					//�����������
					LoadServerGroups();

                    //����
                    SortServerGroupByGroupId();

					_systemTickTimer.Enabled = true;

					_isConfigingGuard = true;

                    //����ServerMap
                    _serverMap.LoadServerMap();

					_running = true;

                    _maxPlayerTimer.Enabled = true;
                    _GroupPlayerCountTimer.Enabled = true;

                    //������������
                    RootRegion = new ServerRegion();
                    DirectUnderRegion = new ServerRegion();
                    LoadServerRegions();
                    LoadCabinet();
				}
			}
		}

        public IList GetGroupInCabinet(string cabinetName)
        {
            IList groupInCabinetList = ArrayList.Synchronized(new ArrayList());
            foreach (ServerGroup group in _topServerGroupList)
            {
                GroupInCabinet(group, cabinetName, groupInCabinetList);
            }
            return groupInCabinetList;
        }

        private void GroupInCabinet(ServerGroup group, string cabinetName, IList groupInCabinetList)
        {
            if (group == null) return;

            if (group.GroupType == ServerGroup.Type.Server)
            {
                if (group.Cabinet.Trim().ToLower() == cabinetName.Trim().ToLower())
                    groupInCabinetList.Add(group);
            }
            else
            {
                foreach (ServerGroup childGroup in group.List)
                {
                    if (childGroup.GroupType == ServerGroup.Type.Server)
                    {
                        if (childGroup.Cabinet.Trim().ToLower() == cabinetName.Trim().ToLower())
                            groupInCabinetList.Add(childGroup);
                    }
                    else
                    {
                        GroupInCabinet(childGroup,cabinetName, groupInCabinetList);
                    }
                }
            }
        }


        void LoadCabinet()
        {
            foreach (ServerGroup group in _topServerGroupList)
            {
                GetDistinctCabinet(group);
            }
        }

        private void GetDistinctCabinet(ServerGroup group)
        {
            if (group == null) return;

            if (group.GroupType == ServerGroup.Type.Server)
            {
                if (group.Cabinet != string.Empty && !_cabinetList.Contains(group.Cabinet)) _cabinetList.Add(group.Cabinet);
            }
            else
            {
                foreach (ServerGroup childGroup in group.List)
                {
                    if (childGroup.GroupType == ServerGroup.Type.Server)
                    {
                        if (childGroup.Cabinet != string.Empty && !_cabinetList.Contains(childGroup.Cabinet)) _cabinetList.Add(childGroup.Cabinet);
                    }
                    else
                    {
                        GetDistinctCabinet(childGroup);
                    }
                }
            }
        }

		public void Shutdown()
		{
			lock (this)
			{
				if (_running)
				{
					_isConfigingGuard = false;

                    _maxPlayerTimer.Enabled = false;
                    _GroupPlayerCountTimer.Enabled = false;

					_systemTickTimer.Enabled = false;

					foreach (GameServer server in _gameServerList)
					{
						_systemTickTimer.Elapsed -= new System.Timers.ElapsedEventHandler(server.OnTick);
						server.Dispose();
					}

					_gameServerList.Clear();

					_gameServerGroupList.Clear();

					_topServerGroupList.Clear();

					_authenticationList.Clear();

					_configGuardServerList.Clear();

					_running = false;
				}
			}
		}

        /// <summary>
        /// �����������
        /// </summary>
        void LoadServerRegions()
        {
            bool bFindRoot = false;
            bool bFinDirectUnder = false;

            _serverRegionList.Clear();

            using (IBlazeDatabase db = DbFactory.GetDatabase())
            {
                IBlazeTable serverRegionTable = db.GetTable(TableString.ServerRegionTableName);
                DataSet dataSet = new DataSet();
                serverRegionTable.Get(dataSet);

                DataRowCollection rows = dataSet.Tables[TableString.ServerRegionTableName].Rows;
                
                foreach (DataRow row in rows)
                {
                    int regionId = (int)row[TableString.ServerRegionFieldId];
   
                    ServerRegion Region = new ServerRegion();
                    Region.Id = (int)row[TableString.ServerRegionFieldId];
                    Region.RegionType = (ServerRegion.Type)Enum.Parse(typeof(ServerRegion.Type),row[TableString.ServerRegionFieldRegionType].ToString());
                    Region.MaxPlayer = (int)row[TableString.ServerRegionFieldMaxPlayers];
                    Region.MaxPlayerLogTime = DateTime.Now;
                    Region.RegionName = row[TableString.ServerRegionFieldRegionName].ToString();
                    
                    if (Region.MaxPlayer > 0)
                    {
                        string maxPlayerTime = row[TableString.ServerRegionFieldMaxPlayersLogTime].ToString();
                        if (maxPlayerTime != string.Empty)
                        {
                            Region.MaxPlayerLogTime = DateTime.Parse(maxPlayerTime);
                        }
                    }

                    if (Region.RegionType == ServerRegion.Type.Root)
                    {
                        bFindRoot = true;
                        RootRegion = Region;
                    }
                    else if (Region.RegionType == ServerRegion.Type.DirectUnder)
                    {
                        bFinDirectUnder = true;
                        DirectUnderRegion = Region;
                    }

                    _serverRegionList.Add(Region);
                }
            }

            if (bFindRoot == false)
            {
                AddRegion("root", (int)ServerRegion.Type.Root);
            }

            if (bFinDirectUnder == false)
            {
                AddRegion("directUnder", (int)ServerRegion.Type.DirectUnder);
            }
        }

        public bool AddRegion(string regionName, int regionType)
        {
             IBlazeDatabase db = null;
             bool ret = false;
             int regionID = 0;
             try
             {
                 db = DbFactory.GetDatabase();
                 db.BeginTrans();

                 //�������ݵ���������
                 string[] fields = new string[] { 
					TableString.ServerRegionFieldRegionName,
					TableString.ServerRegionFieldRegionType
				};

                 object[] values = new object[] {
					regionName,
					regionType
				};
                 IBlazeTable regionTable = db.GetTable(TableString.ServerRegionTableName);
                 regionID = (int)regionTable.Add(fields, values);

                 db.CommitTrans();
                 ret = true;
             }
             catch (Exception ex)
             {
                 db.RollbackTrans();
                 Util.DebugLog(ex.Message.ToString());
             }
             finally
             {
                 db = null;
             }
             if (ret)
             {
                 ServerRegion Region = new ServerRegion();
                 Region.Id = regionID;
                 Region.RegionType = (ServerRegion.Type)Enum.Parse(typeof(ServerRegion.Type), regionType.ToString());
                 Region.MaxPlayer = 0;
                 Region.MaxPlayerLogTime = DateTime.Now;
                 Region.RegionName = regionName;
                 _serverRegionList.Add(Region);
             }

            return ret;
        }

        public ServerRegion GetRegion(int regionID)
        {
            lock (_serverRegionList.SyncRoot)
            {
                foreach (ServerRegion region in _serverRegionList)
                {
                    if (region.Id == regionID)
                    {
                        return region;
                    }
                }
            }

            return null;
        }

        public ServerRegion GetRootRegion()
        {
            lock (_serverRegionList.SyncRoot)
            {
                foreach (ServerRegion region in _serverRegionList)
                {
                    if (region.RegionType == ServerRegion.Type.Root)
                    {
                        return region;
                    }
                }
            }

            return null;
        }

        public ServerRegion GetDirectRegion()
        {
            lock (_serverRegionList.SyncRoot)
            {
                foreach (ServerRegion region in _serverRegionList)
                {
                    if (region.RegionType == ServerRegion.Type.DirectUnder)
                    {
                        return region;
                    }
                }
            }

            return null;
        }


		public GameServer GetGameServer(int serverId)
		{
			lock (_gameServerList.SyncRoot)
			{
				foreach (GameServer server in _gameServerList)
				{
					if (server.Id == serverId)
					{
						return server;
					}
				}				
			}

			return null;
		}

		public ServerGroup GetGameServerGroup(int groupId)
		{
			lock (_gameServerGroupList.SyncRoot)
			{
				foreach (ServerGroup group in _gameServerGroupList)
				{
					if (group.Id == groupId)
					{
						return group;
					}
				}
			}

			return null;
		}

		/// <summary>
		/// �����������
		/// </summary>
		void LoadServerGroups()
		{
			SecurityManager sm = AdminServer.TheInstance.SecurityManager;
			_gameServerGroupList.Clear();

			using (IBlazeDatabase db = DbFactory.GetDatabase())
			{
				IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
				DataSet dataSet = new DataSet();
				serverGroupTable.Get(dataSet);			

				DataRowCollection rows = dataSet.Tables[TableString.ServerGroupTableName].Rows;
				foreach (DataRow row in rows)
				{
					int groupId = (int)row[TableString.ServerGroupFieldId];
					ServerGroup.Type groupType = (ServerGroup.Type)row[TableString.ServerGroupFieldType];

					ServerGroup group = new ServerGroup();
					group.SecurityObject = sm.Get(groupId);
					group.GroupType = groupType;
                    group.group_enable = (ServerGroup.GroupEnable)Enum.Parse(typeof(ServerGroup.GroupEnable), row[TableString.ServerGroupFieldGroupEnable].ToString());
                    group.MaxPlayer = (int)row[TableString.ServerGroupFieldMaxPlayers];
                    group.MaxPlayerLogTime = DateTime.Now;
                    group.Cabinet = row[TableString.ServerGroupFieldCabinet].ToString();
                    group.GroupVer = row[TableString.ServerGroupFieldGroupVersion].ToString();

                    if (group.MaxPlayer > 0)
                    {
                        string maxPlayerTime = row[TableString.ServerGroupFieldMaxPlayersLogTime].ToString();
                        if (maxPlayerTime != string.Empty)
                        {
                            group.MaxPlayerLogTime = DateTime.Parse(maxPlayerTime);
                        }
                    }
					_gameServerGroupList.Add(group);
				}
			}

			OrganizeServerGroups();
		}

		/// <summary>
		/// ��֯��������
		/// </summary>
		void OrganizeServerGroups()
		{
			_topServerGroupList.Clear();
			FSEyeObject gameServerListObject = AdminServer.TheInstance.SecurityManager.Get(SecurityObject.FullPath + SecurityManager.ObjectPathDelimiter + "GameServerList");			
			if (gameServerListObject != null)
			{	
				//��֯�����������
				FSEyeObject[] topGroupObjects = gameServerListObject.Children;
				foreach (FSEyeObject obj in topGroupObjects)				
				{
					ServerGroup topGroup = GetGameServerGroup(obj.Id);
					if (topGroup != null)
					{
						_topServerGroupList.Add(topGroup);
					}
				}

				//��֯��������ṹ
				lock (_gameServerGroupList.SyncRoot)
				{
					foreach (ServerGroup group in _gameServerGroupList)
					{
						group.List.Clear();
						FSEyeObject[] children = group.SecurityObject.Children;
						if (children != null && children.Length > 0)
						{
							if (group.GroupType == ServerGroup.Type.Server)
							{
								foreach (FSEyeObject child in children)
								{
									GameServer server = GetGameServer(child.Id);
									if (server != null)
									{
										server.Group = group;
										group.List.Add(server);
									}
								}
							}
							else if (group.GroupType == ServerGroup.Type.Group)
							{
								foreach (FSEyeObject child in children)
								{
									ServerGroup childGroup = GetGameServerGroup(child.Id);
									group.List.Add(childGroup);
								}
							}
						}
					}
				}	
			}					
		}

        /// <summary>
        /// ������.GroupType == ServerGroup.Type.Group��ServerGroup��.List��������
        /// ����.Id������������
        /// ��������.Listʱ���ǰ���ServerGroup������˳����
        /// </summary>
        private void SortServerGroupByGroupId()
        {
            IList netGroupList = AdminServer.TheInstance.GameServerManager.TopServerGroups;

            foreach (ServerGroup netGroup in netGroupList)
            {
                if (netGroup.GroupType == ServerGroup.Type.Group)
                {
                    SortActualServerGroup(netGroup);
                }
            }
        }

        private void SortActualServerGroup(ServerGroup netGroup)
        {
            if (netGroup.List.Count > 0)
            {
                //��netGroup.List����������.Id���Խ�������                          
                IComparer myComparer = new ByServerGroupIdComparer();
                ArrayList tempArrayList = ArrayList.Adapter(netGroup.List);
                tempArrayList.Sort(myComparer);
                netGroup.List = (IList)tempArrayList;

                foreach (ServerGroup group in netGroup.List)
                {
                    if (group.GroupType == ServerGroup.Type.Group)
                    {
                        SortActualServerGroup(group);
                    }
                }
            }
        }

        //�����������շ��������id���򣬹�����SortActualServerGroupʹ��
        public class ByServerGroupIdComparer : IComparer
        {
            int IComparer.Compare(Object x, Object y)
            {
                int xId = ((ServerGroup)x).Id;
                int yId = ((ServerGroup)y).Id;
                return ((new CaseInsensitiveComparer()).Compare(xId, yId));
            }
        }

		/// <summary>
		/// ��Ϸ�����������¼�����
        /// ������ӣ���˵����guard���������ӵ�WEB�������ˣ�֮����ͨ��Э���ж����ĸ�����server������
		/// </summary>
		public void GameServerConnected(object sender, ClientConnectEventArgs e)
		{
			string address = e.RemoteAddress;
			int index = address.LastIndexOf(":");
			string ipAddress = address.Substring(0, index);
			int port = int.Parse(address.Substring(index + 1));
			long connectionId = e.ConnectionId;
            UInt16 GuardServers = 0;

			//��Ϸ��������֤����
			if (SystemConfig.Current.NeedGuardAuthorization)
			{
				GameServerAuthentication auth = new GameServerAuthentication(connectionId, ipAddress, port, SystemConfig.Current.GuardAuthorizationTimeout);
				lock (_authenticationList.SyncRoot)
				{
					_authenticationList.Add(auth);
				}
				auth.Begin();
			}
			else
			{
                GameServer svr = null;
                string[] para = new string[16];
                for(int i=0;i<16;i++)
                {
                    para[i] = "0";
                }

				lock (_gameServerList.SyncRoot)
				{
					foreach (GameServer server in _gameServerList)
					{
						lock (server)
						{
							if (server.IpAddress  == ipAddress)
							{
                                if (svr == null) svr = server;
                                GuardServers++;
								if (!server.IsConnected)
								{
									server.OnConnected(connectionId, new byte[] { 1 }, port);
									//return;
								}

                                switch (server.Type)
                                {
                                    case GameServer.ServerType.goddess:
                                        para[0] = server.Id.ToString();
                                        break;
                                    case GameServer.ServerType.bishop:
                                        para[1] = server.Id.ToString();
                                        break;
                                    case GameServer.ServerType.gamecenter:
                                        para[2] = server.Id.ToString();
                                        break;
                                    case GameServer.ServerType.logserver:
                                        para[3] = server.Id.ToString();
                                        break;
                                    case GameServer.ServerType.gameserver:
                                        if (server.Name.IndexOf("01") > 0) para[4] = server.Id.ToString();
                                        else if (server.Name.IndexOf("02") > 0) para[5] = server.Id.ToString();
                                        else if (server.Name.IndexOf("03") > 0) para[6] = server.Id.ToString();
                                        else if (server.Name.IndexOf("04") > 0) para[7] = server.Id.ToString();
                                        else if (server.Name.IndexOf("05") > 0) para[8] = server.Id.ToString();
                                        else if (server.Name.IndexOf("06") > 0) para[9] = server.Id.ToString();
                                        else if (server.Name.IndexOf("07") > 0) para[10] = server.Id.ToString();
                                        else if (server.Name.IndexOf("08") > 0) para[11] = server.Id.ToString();
                                        else if (server.Name.IndexOf("09") > 0) para[12] = server.Id.ToString();
                                        else if (server.Name.IndexOf("10") > 0) para[13] = server.Id.ToString();
                                        else if (server.Name.IndexOf("11") > 0) para[14] = server.Id.ToString();
                                        else if (server.Name.IndexOf("12") > 0) para[15] = server.Id.ToString();
                                        break;
                                }
							}
						}
					}
				}

                if (svr != null)   svr.DoPlugInAction(SecurityManager.SystemAutomationId, E2gServerid.PlugInGuid, E2gServerid.actionKey, para);

				//�Ͽ�û�м�¼������
                if (GuardServers == 0) 
				    AdminServer.TheInstance.ConnectionManager.Disconnect(connectionId);                
			}
		}

		/// <summary>
		/// ��Ϸ�������Ͽ��¼�����
		/// </summary>
		public void GameServerDisconnected(object sender, ClientDisconnectEventArgs e)
		{
			lock (_gameServerList.SyncRoot)
			{
				foreach (GameServer server in _gameServerList)
				{
					lock (server)
					{
						if (server.ConnectionId == e.ConnectionId)
						{
							if (server.IsConnected)
							{
								server.OnDisconnected();
								//return;
                            }
						}
					}
				}
			}

			//TODO ��Ҫ��¼δ֪�ĶϿ�
		}

		/// <summary>
		/// ������Ϸ������
		/// </summary>
		private void LoadGameServers()
		{
			lock (this)
			{
				IBlazeDatabase db = null;
				try
				{
					db = DbFactory.GetDatabase();
					IBlazeTable table = db.GetTable(TableString.GameServerTableName);					

					DataSet data = new DataSet();
					table.Get(data);
					DataRowCollection rows = data.Tables[TableString.GameServerTableName].Rows;
					for (int i = 0; i < rows.Count; i++)
					{
						DataRow row = rows[i];
						int serverId = (int)row[TableString.GameServerFieldId];
						string serverNetAddress = row[TableString.GameServerFieldNetAddress] as string;
						string gameDir = row[TableString.GameServerFieldGameDir] as string;
                        string configcontent = row[TableString.GameServerFieldCfgString] as string;
                        int ftpServer = (int)row[TableString.GameServerFieldFtpDownloadServer];
						GameServer.ConfigGuardState configState = (GameServer.ConfigGuardState)row[TableString.GameServerFieldHasBeenConfigGuard];
						GameServer.ServerType serverType = (GameServer.ServerType)row[TableString.GameServerFieldServerType];
												
						FSEyeObject serverObj = AdminServer.TheInstance.SecurityManager.Get(serverId);
						if (serverObj != null)
						{
							GameServer server = new GameServer();
							server.SecurityObject = serverObj;
							server.IpAddress = serverNetAddress;
							server.GameDir = gameDir;
							server.ConfigState = configState;
							server.Type = serverType;
                            server.ConfigContent = configcontent;
                            server.FtpServer = ftpServer;

                            //ȡ��gmc_ip
                            string GMCIp = row[TableString.GameServerFieldGmcIp] as string;
                            if ((GMCIp != null) && (GMCIp.Length != 0))
                            {
                                server.LordConfig.GMCIp = GMCIp;
                            }
                            
                            //ȡ��gmc_account
                            string GMCAccount = row[TableString.GameServerFieldGmcAccount] as string;
                            if ((GMCAccount != null) && (GMCAccount.Length != 0))
                            {
                                server.LordConfig.GMCAccount = GMCAccount;
                            }

                            try
                            {
                                server.LordConfig.FSEyeIp = (string)row[TableString.GameServerFieldFSEyeIp];
                                server.LordConfig.FSEyePort = (int)row[TableString.GameServerFieldFSEyePort];
                            }
                            catch (Exception)
                            { }

                            try
                            {
                                server.LordConfig.PaysysIp = (string)row[TableString.GameServerFieldPaysysIp];
                                server.LordConfig.PaysysPort = (int)row[TableString.GameServerFieldPaysysPort];
                                server.LordConfig.PaysysLoginName = (string)row[TableString.GameServerFieldPaysysLoginName];
                                server.LordConfig.PaysysLoginPwd = (string)row[TableString.GameServerFieldPaysysLoginPwd];
                            }
                            catch (Exception)
                            { }

                            try
                            {
								server.LordConfig.DbHost = (string)row[TableString.GameServerFieldDbHost];
								server.LordConfig.DbName = (string)row[TableString.GameServerFieldDbName];
								server.LordConfig.DbLoginName = (string)row[TableString.GameServerFieldDbLoginName];
								server.LordConfig.DbLoginPwd = (string)row[TableString.GameServerFieldDbLoginPwd];
								server.LordConfig.DbBackupPath = (string)row[TableString.GameServerFieldDbBackupDir];
							}
							catch (Exception)
							{
							}
                            server.LordConfig.BindIp = server.IpAddress;
							
							server.Initialize();
							server.GameStateChanged += new GameStateChangeEventHandler(GameStateChanged);
							_systemTickTimer.Elapsed += new System.Timers.ElapsedEventHandler(server.OnTick);
							_gameServerList.Add(server);
						}
					}					
				}
				catch (Exception ex)
				{
					throw ex;
				}
				finally
				{
					if (db != null)
					{
						db.Close();
					}
				}
			}
		}

		/// <summary>
		/// ɾ����Ϸ������
		/// </summary>
		/// <param name="id">���������</param>
		public bool DeleteGameServer(GameServer server)
		{
			if (server == null)
				throw new ArgumentNullException("server");

			IBlazeDatabase db = null;
			try
			{
				db = DbFactory.GetDatabase();
				db.BeginTrans();

				ServerGroup group = server.Group;
				if (group != null)
				{
                    group.DeleteGameServer(server, db);
                    //�����server���ڵ�groupû��server�˾Ͱ�����ΪGroup�͵�
                    if (group.SecurityObject.Children.Length == 0)
                    {
                        group.GroupType = ServerGroup.Type.Group;
                        db.GetTable(TableString.ServerGroupTableName).Set(TableString.ServerGroupFieldType, (int)group.GroupType, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, group.Id));
                    }					
				}                

				IBlazeTable serverTable = db.GetTable(TableString.GameServerTableName);
				serverTable.Delete(FilterFactory.CreateEqualFilter(TableString.GameServerFieldId, server.Id));
				
                //����ɾ��GaveServer�Ĳ��
                serverTable = db.GetTable(TableString.ServerPlugInTableName);
                serverTable.Delete(FilterFactory.CreateEqualFilter(TableString.ServerPlugInFieldGameServerId, server.Id));

				SecurityManager sm = AdminServer.TheInstance.SecurityManager;
				sm.Delete(server.Id, db);

				db.CommitTrans();

				_gameServerList.Remove(server);
				_systemTickTimer.Elapsed -= new System.Timers.ElapsedEventHandler(server.OnTick);
				server.Dispose();

				return true;
			}
			catch (Exception ex)
			{
				if (db != null)
					db.RollbackTrans();
				throw ex;
			}
			finally
			{
				if (db != null)
					db.Close();
			}
		}

		/// <summary>
		/// �����Ϸ������
		/// </summary>
		/// <param name="name">��������</param>
		/// <param name="comment">����������</param>
		/// <param name="ipAddress">IP��ַ</param>
		/// <param name="macAddress">MAC��ַ</param>
		/// <param name="gameDir">��ϷĿ¼</param>
		/// <param name="group">��������</param>
		public bool AddGameServer(
			string name,
			string comment,
			string ipAddress,
			string gameDir,
			ServerGroup group,
			GameServer.ConfigGuardState configState,
			//bool hasBeenConfigGuard,
            string cfgContent,
			GameServer.ServerType serverType,
            int ftpServer
		)
		{
			if (name == null)
				throw new ArgumentNullException("name");
			if (comment == null)
				throw new ArgumentNullException("comment");
			if (ipAddress == null)
				throw new ArgumentNullException("ipAddress");
			if (gameDir == null)
				throw new ArgumentNullException("gameDir");
			if (group == null)
				throw new ArgumentNullException("group");
            if (cfgContent == null)
                throw new ArgumentNullException("cfgContent");
			IBlazeDatabase db = null;
			try
			{
				db = DbFactory.GetDatabase();
				db.BeginTrans();

				SecurityManager sm = AdminServer.TheInstance.SecurityManager;

				//������ذ�ȫ����
				string newGameServerObjPath = group.SecurityObject.FullPath + SecurityManager.ObjectPathDelimiter + name;
				FSEyeObject newGameServerObj = sm.Get(newGameServerObjPath, db);
				newGameServerObj.Comment = comment;
				sm.Set(newGameServerObj);
                
                //2008-07-16 Ϊ�˸������ܺͼ�Ȩ�����ã�����������Щobject��
                //sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.Console, db);
                //sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo, db);				
                //sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.PlugIn, db);
                //sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.Process, db);
                //sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.NetCard, db);
                //sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.BasicInfo, db);
                //sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.Cpu, db);
                //sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.Disk, db);
                //sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.FMTask, db);
                //sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.Memory, db);
				
				//�������ݵ���������
				string[] fields = new string[] { 
					TableString.GameServerFieldId,
					TableString.GameServerFieldNetAddress,
					TableString.GameServerFieldGameDir,
					TableString.GameServerFieldHasBeenConfigGuard,
					TableString.GameServerFieldServerType,
                    TableString.GameServerFieldCfgString,
                    TableString.GameServerFieldFtpDownloadServer
				};
                
				object[] values = new object[] {
					newGameServerObj.Id,
					ipAddress,
					gameDir,
					(int)configState,
					(int)serverType,
                    cfgContent,
                    ftpServer
				};
				IBlazeTable serverTable = db.GetTable(TableString.GameServerTableName);
				serverTable.Add(fields, values);

				GameServer server = new GameServer();
				server.SecurityObject = newGameServerObj;
				server.IpAddress = ipAddress;
				server.GameDir = gameDir;
				server.ConfigState = configState;
				server.Type = serverType;
                server.ConfigContent = cfgContent;
                server.FtpServer = ftpServer;
                server.GameServiceState = GameServer.ServiceState.OKStopped;
				group.AddGameServer(server, db);

                //���group��Type��Group���뽫���ΪServer�͵�
                if (group.GroupType == ServerGroup.Type.Group)
                {
                    group.GroupType = ServerGroup.Type.Server;
                    IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
                    serverGroupTable.Set(TableString.ServerGroupFieldType, (int)group.GroupType, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, group.Id));
                }

                /*2008-07-16 GameServer��Ȩ�޲��ٵ������ƣ�����ͳһ��GameServerManager���ƣ����ԾͲ��ٸ��Ƹ�һ����Ȩ�޹����ˡ�
                 * 
                 * 
                //����Ĭ��Ȩ��,����ֱ�Ӹ��׵�Ȩ��
                AdminServer.TheInstance.SecurityManager.CopyAce(newGameServerObj.Parent, newGameServerObj, true);
                 */

				db.CommitTrans();

				_gameServerList.Add(server);
				server.Initialize();
				_systemTickTimer.Elapsed += new System.Timers.ElapsedEventHandler(server.OnTick);

				return true;
			}
			catch (Exception)
			{
				if (db != null)
					db.RollbackTrans();
			}
			finally
			{
				if (db != null)
					db.Close();
			}

			return false;
		}


        /// <summary>
        /// ��Ӷ��������
        /// </summary>
        /// <param name="name">��������</param>
        /// <param name="comment">����������</param>
        /// <param name="ipAddress">IP��ַ</param>
        /// <param name="gameDir">��ϷĿ¼</param>
        /// <param name="group">��������</param>
        public bool AddGameServers(
            IList<object[]> para
        )
        {
            IList<GameServer> gameServers = new List<GameServer>();
            IBlazeDatabase db = null;
            try
            {
                db = DbFactory.GetDatabase();
                db.BeginTrans();

                foreach (object[] strPara in para)
                {
                    string name = ((string)strPara[0]).ToString();
                    string comment = ((string)strPara[1]).ToString();
                    string ipAddress = ((string)strPara[2]).ToString();
                    string gameDir = ((string)strPara[3]).ToString();
                    ServerGroup group = (ServerGroup)strPara[4];
                    GameServer.ConfigGuardState configState = (GameServer.ConfigGuardState)strPara[5];
                    string cfgContent = ((string)strPara[6]).ToString();
                    GameServer.ServerType serverType = (GameServer.ServerType)strPara[7];

                    SecurityManager sm = AdminServer.TheInstance.SecurityManager;

                    //������ذ�ȫ����
                    string newGameServerObjPath = group.SecurityObject.FullPath + SecurityManager.ObjectPathDelimiter + name;
                    FSEyeObject newGameServerObj = sm.Get(newGameServerObjPath, db);
                    newGameServerObj.Comment = comment;
                    sm.Set(newGameServerObj);
                    sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.Console, db);
                    sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo, db);
                    sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.PlugIn, db);
                    sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.Process, db);
                    sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.NetCard, db);
                    sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.BasicInfo, db);
                    sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.Cpu, db);
                    sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.Disk, db);
                    sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.FMTask, db);
                    sm.Get(newGameServerObjPath + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.ServerInfo + SecurityManager.ObjectPathDelimiter + PredefinedSecurityObjects.Memory, db);

                    //�������ݵ���������
                    string[] fields = new string[] { 
					TableString.GameServerFieldId,
					TableString.GameServerFieldNetAddress,
					TableString.GameServerFieldGameDir,
					TableString.GameServerFieldHasBeenConfigGuard,
					TableString.GameServerFieldServerType,
                    TableString.GameServerFieldCfgString
				};

                    object[] values = new object[] {
					newGameServerObj.Id,
					ipAddress,
					gameDir,
					(int)configState,
					(int)serverType,
                    cfgContent
				};
                    IBlazeTable serverTable = db.GetTable(TableString.GameServerTableName);
                    serverTable.Add(fields, values);

                    GameServer server = new GameServer();
                    server.SecurityObject = newGameServerObj;
                    server.IpAddress = ipAddress;
                    server.GameDir = gameDir;
                    server.ConfigState = configState;
                    server.Type = serverType;
                    server.ConfigContent = cfgContent;                    
                    group.AddGameServer(server, db);

                    //���group��Type��Group���뽫���ΪServer�͵�
                    if (group.GroupType == ServerGroup.Type.Group)
                    {
                        group.GroupType = ServerGroup.Type.Server;
                        IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
                        serverGroupTable.Set(TableString.ServerGroupFieldType, (int)group.GroupType, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, group.Id));
                    }

                    //����Ĭ��Ȩ��,����ֱ�Ӹ��׵�Ȩ��
                    AdminServer.TheInstance.SecurityManager.CopyAce(newGameServerObj.Parent, newGameServerObj, true);
                    
                    gameServers.Add(server);                                       
                }
                db.CommitTrans();
                foreach(GameServer server in gameServers)
                {
                _gameServerList.Add(server); 
                server.Initialize();
                _systemTickTimer.Elapsed += new System.Timers.ElapsedEventHandler(server.OnTick);
                }
                

                return true;
            }
            catch (Exception)
            {
                if (db != null)
                    db.RollbackTrans();
            }
            finally
            {
                if (db != null)
                    db.Close();
            }
            return false;
        }

		/// <summary>
		/// �޸���Ϸ������
		/// </summary>
		/// <param name="server">������</param>
		/// <param name="name">��������</param>
		/// <param name="comment">����������</param>
		/// <param name="ipAddress">IP��ַ</param>
		/// <param name="macAddress">MAC��ַ</param>
		/// <param name="gameDir">��ϷĿ¼</param>
		/// <param name="newGroup">��������</param>
		/// <param name="lordConfig">Lord����</param>
		public bool ModifyGameServer(
			GameServer server,
			string name,
			string comment,
			string ipAddress,
			string gameDir,
			ServerGroup newGroup,
			LordConfig lordConfig,
            GameServer.ConfigGuardState configState,
            string cfgContent,
			//bool hasBeenConfigGuard,
			GameServer.ServerType serverType,
            int ftpServer
		)
		{
			if (server == null)
				throw new ArgumentNullException("server");
			if (name == null)
				throw new ArgumentNullException("name");
			if (comment == null)
				throw new ArgumentNullException("comment");
			if (ipAddress == null)
				throw new ArgumentNullException("ipAddress");
			if (gameDir == null)
				throw new ArgumentNullException("gameDir");
			if (newGroup == null)
				throw new ArgumentNullException("newGroup");
            if (cfgContent == null)
                throw new ArgumentNullException("cfgContent");
			IBlazeDatabase db = null;
			try
			{
				SecurityManager sm = AdminServer.TheInstance.SecurityManager;

				db = DbFactory.GetDatabase();
				db.BeginTrans();

				FSEyeObject serverObject = server.SecurityObject;
				serverObject.Name = name;
				serverObject.Comment = comment;
				sm.Set(serverObject, db);
                //��Ϸ����������ֵ
				string[] gameServerFields = null;
				object[] gameServerValues = null;

				if (lordConfig == null)
				{
					gameServerFields = new string[] { 
						TableString.GameServerFieldNetAddress,
						TableString.GameServerFieldGameDir,
						TableString.GameServerFieldHasBeenConfigGuard,
						TableString.GameServerFieldServerType,
                        TableString.GameServerFieldCfgString,
                        TableString.GameServerFieldFtpDownloadServer
					};

					gameServerValues = new object[] {
						ipAddress,
						gameDir,
						(int)configState,
						(int)serverType,
                        cfgContent,
                        ftpServer
					};
				}
				else
				{
                    ArrayList gameServerFieldList = new ArrayList();
                    ArrayList gameServerValueList = new ArrayList();

                    //TODO: HUANGBIAO 2008-1-20 ����������û��
                    //gameServerFieldList.Add(TableString.GameServerFieldNetAddress);
                    //gameServerFieldList.Add(TableString.GameServerFieldGameDir);
                    //gameServerFieldList.Add(TableString.GameServerFieldLordPort);
                    //gameServerFieldList.Add(TableString.GameServerFieldMaxPlayer);
                    //gameServerFieldList.Add(TableString.GameServerFieldIntranetMask);
                    //gameServerFieldList.Add(TableString.GameServerFieldInternetMask);

                    //gameServerValueList.Add(ipAddress);
                    //gameServerValueList.Add(gameDir);
                    //gameServerValueList.Add(lordConfig.ServerCount);
                    //gameServerValueList.Add(lordConfig.MaxPlayer);
                    //gameServerValueList.Add(lordConfig.IntranetMask);
                    //gameServerValueList.Add(lordConfig.InternetMask);

                    //Payasys�Ƿ�����
                    if (lordConfig.PaysysIp != null && lordConfig.PaysysIp.Length != 0)
                    {
                        gameServerFieldList.Add(TableString.GameServerFieldPaysysIp);
                        gameServerFieldList.Add(TableString.GameServerFieldPaysysPort);
                        gameServerFieldList.Add(TableString.GameServerFieldPaysysLoginName);
                        gameServerFieldList.Add(TableString.GameServerFieldPaysysLoginPwd);

                        gameServerValueList.Add(lordConfig.PaysysIp);
                        gameServerValueList.Add(lordConfig.PaysysPort);
                        gameServerValueList.Add(lordConfig.PaysysLoginName);
                        gameServerValueList.Add(lordConfig.PaysysLoginPwd);
                    }

                    if (cfgContent.Length != 0)
                    {
                        gameServerFieldList.Add(TableString.GameServerFieldCfgString);
                        gameServerValueList.Add(cfgContent);
                    }

                    //Db�Ƿ�����
                    if (lordConfig.DbHost != null && lordConfig.DbHost.Length != 0)
                    {
                        gameServerFieldList.Add(TableString.GameServerFieldDbHost);
                        gameServerFieldList.Add(TableString.GameServerFieldDbName);
                        gameServerFieldList.Add(TableString.GameServerFieldDbLoginName);
                        gameServerFieldList.Add(TableString.GameServerFieldDbLoginPwd);
                        gameServerFieldList.Add(TableString.GameServerFieldDbBackupDir);

                        gameServerValueList.Add(lordConfig.DbHost);
                        gameServerValueList.Add(lordConfig.DbName);
                        gameServerValueList.Add(lordConfig.DbLoginName);
                        gameServerValueList.Add(lordConfig.DbLoginPwd);
                        gameServerValueList.Add(lordConfig.DbBackupPath);
                    }

                    //�Ƿ�����GMCIp��GMCAccount�������ֶ�
                    if (lordConfig.GMCIp != null && lordConfig.GMCIp.Length != 0)
                    {
                        gameServerFieldList.Add(TableString.GameServerFieldGmcIp);
                        gameServerValueList.Add(lordConfig.GMCIp);
                    }
                    if (lordConfig.GMCAccount != null && lordConfig.GMCAccount.Length != 0)
                    {
                        gameServerFieldList.Add(TableString.GameServerFieldGmcAccount);
                        gameServerValueList.Add(lordConfig.GMCAccount);
                    }

                    gameServerFieldList.Add(TableString.GameServerFieldFSEyeIp);
                    gameServerFieldList.Add(TableString.GameServerFieldFSEyePort);
                    gameServerFieldList.Add(TableString.GameServerFieldHasBeenConfigGuard);
                    gameServerFieldList.Add(TableString.GameServerFieldServerType);

                    gameServerValueList.Add(lordConfig.FSEyeIp);
                    gameServerValueList.Add(lordConfig.FSEyePort);
                    gameServerValueList.Add((int)configState);
                    gameServerValueList.Add((int)serverType);

                    gameServerFields = (string[])gameServerFieldList.ToArray(typeof(string));
                    gameServerValues = (object[])gameServerValueList.ToArray();
				}

				IBlazeTable gameServerTable = db.GetTable(TableString.GameServerTableName);
				gameServerTable.Set(gameServerFields, gameServerValues, FilterFactory.CreateEqualFilter(TableString.GameServerFieldId, server.Id));

				server.SecurityObject.Name = name;
				server.SecurityObject.Comment = comment;

				server.IpAddress = ipAddress;
				server.GameDir = gameDir;
				server.ConfigState = configState;
				server.Type = serverType;
                server.ConfigContent = cfgContent;
                server.FtpServer = ftpServer;

				if (lordConfig != null)
				{
					server.LordConfig.Copy(lordConfig);					
				}

				ServerGroup oldGroup = server.Group;
				if (oldGroup != null && newGroup != null)
				{					
					oldGroup.List.Remove(server);
					oldGroup.SecurityObject.RemoveChild(server.SecurityObject.Name);
					sm.Set(oldGroup.SecurityObject, db);

					newGroup.List.Add(server);
					newGroup.SecurityObject.AddChild(server.SecurityObject);
					sm.Set(newGroup.SecurityObject, db);

					server.Group = newGroup;
				}

				sm.Set(serverObject, db);

				db.CommitTrans();

				return true;
			}
			catch (Exception)
			{
				if (db != null)
					db.RollbackTrans();
			}
			finally
			{
				if (db != null)
					db.Close();
			}
			
			return false;
		}

        public void ReBuildLordConfig(GameServer server)
        {
            server.LordConfig.ServerIDs = server.SameGuardServerIDs;
            server.LordConfig.ServerCount = server.SameGuardServerCount;
            server.LordConfig.BindIp = server.IpAddress;
        }

		/// <summary>
		/// ��ӷ�������
		/// </summary>
		/// <param name="groupName">����</param>
		/// <param name="groupComment">����</param>
		/// <param name="parentGroup">������������</param>
		/// <returns></returns>
		public ServerGroup AddServerGroup(string groupName, string groupComment, ServerGroup.Type grouptype, ServerGroup parentGroup, string cabinet)
		{
			if (groupName == null)
				throw new ArgumentNullException("groupName");
			if (groupComment == null)
				throw new ArgumentNullException("groupComment");
			if (parentGroup != null && parentGroup.GroupType != ServerGroup.Type.Group && parentGroup.List.Count > 0)
				throw new Exception(string.Format("Server group {0} contains game servers.", parentGroup.Name));

			using (IBlazeDatabase db = DbFactory.GetDatabase())
			{
				try
				{
					db.BeginTrans();
					SecurityManager sm = AdminServer.TheInstance.SecurityManager;

					IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
					ServerGroup newGroup = new ServerGroup();
					newGroup.GroupType = grouptype;
                    newGroup.Cabinet = cabinet;

					FSEyeObject parentObject = null;
					FSEyeObject newServerGroupObject = null;
					if (parentGroup == null)
					{
						parentObject = sm.Get(SecurityObject.FullPath + SecurityManager.ObjectPathDelimiter + "GameServerList");

                        //���������������֮�䲻��������
                        foreach (ServerGroup group in _topServerGroupList)
                        {
                            if (group.Name == groupName)
                                throw new Exception(String.Format("Group with name {0} already exist.", groupName));
                        }

						//��ӵ�������������б���
						_topServerGroupList.Add(newGroup);
					}
					else
					{
						parentObject = parentGroup.SecurityObject;

						//ͬһ�������������µ��ӷ������鲻������
						foreach (ServerGroup group in parentGroup.List)
						{
							if (group.Name == groupName)
								throw new Exception(String.Format("Group with name {0} already exist.", groupName));
						}
												
						//���������������Group���͵ģ������ΪGroup���͵�
						if (parentGroup.GroupType != ServerGroup.Type.Group)
						{
							parentGroup.GroupType = ServerGroup.Type.Group;
							serverGroupTable.Set(TableString.ServerGroupFieldType, (int)parentGroup.GroupType, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, parentGroup.Id));
						}
						//��ӵ�����������ķ��������б���
						parentGroup.List.Add(newGroup);						
					}

					if (parentObject != null)
					{
						//�������洢��ȫ����
						newServerGroupObject = sm.Get(parentObject.FullPath + SecurityManager.ObjectPathDelimiter + groupName, db);
						newServerGroupObject.Comment = groupComment;
						sm.Set(newServerGroupObject);

						newGroup.SecurityObject = newServerGroupObject;

						//����µķ���������󵽷��������б���
						_gameServerGroupList.Add(newGroup);

						//��ӷ��������¼�����������						
						string[] fields = new string[] {
							TableString.ServerGroupFieldId,
							TableString.ServerGroupFieldType,
                            TableString.ServerGroupFieldCabinet                            
						};
						object[] values = new object[] {
							newServerGroupObject.Id,
							(int)grouptype,
                            cabinet                            
						};
						serverGroupTable.Add(fields, values);

                        /*2008-07-16 GameServer��Ȩ�޲��ٵ������ƣ�����ͳһ��GameServerManager���ƣ����ԾͲ��ٸ��Ƹ�һ����Ȩ�޹����ˡ�
                         * 
                        //����Ȩ��
                        AdminServer.TheInstance.SecurityManager.CopyAce(newServerGroupObject.Parent, newServerGroupObject, true);
                         *
                         */

						db.CommitTrans();
						return newGroup;
					}
				}
				catch (Exception ex)
				{
					if (db != null)
						db.RollbackTrans();

					throw ex;
				}
			}

			return null;
		}

		/// <summary>
		/// ɾ����������
		/// </summary>
		/// <param name="groupId">���������</param>		
		public bool DeleteServerGroup(int groupId)
		{
			ServerGroup group = GetGameServerGroup(groupId);
			if (group == null)
				return false;

			using(IBlazeDatabase db = DbFactory.GetDatabase())
			{
				try
				{					
					db.BeginTrans();
					SecurityManager sm = AdminServer.TheInstance.SecurityManager;

					//����Ƕ���������飬��_topServerGroupList��ɾ�������򣬴Ӹ���������ɾ��
					FSEyeObject serverGroupListObject = sm.Get(SecurityObject.FullPath + SecurityManager.ObjectPathDelimiter + "GameServerList");
					FSEyeObject groupParent = group.SecurityObject.Parent;					
					if (groupParent == serverGroupListObject)
					{
						_topServerGroupList.Remove(group);
					}
					else
					{
						ServerGroup parentGroup = GetGameServerGroup(groupParent.Id);
						parentGroup.List.Remove(group);
					}

					//�ӷ��������б���ɾ��
					_gameServerGroupList.Remove(group);

					//ɾ����ȫ����
					sm.Delete(groupId, db);

					//�ӷ����������ɾ��
					IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
					serverGroupTable.Delete(FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, groupId));

					db.CommitTrans();
					return true;
				}
				catch (Exception ex)
				{
					if (db != null)
						db.RollbackTrans();

					throw ex;
				}
			}
		}

		/// <summary>
		/// �޸ķ�������
		/// </summary>
		/// <param name="modifyGroup">�޸ĵķ�������</param>
		/// <param name="newParentGroup">�µ�������������</param>
		/// <returns></returns>
		public bool ModifyServerGroup(ServerGroup modifyGroup, ServerGroup newParentGroup)
		{
			if (modifyGroup == null)
				throw new ArgumentNullException("modifyGroup");

			using (IBlazeDatabase db = DbFactory.GetDatabase())
			{
				try
				{
					db.BeginTrans();

					SecurityManager sm = AdminServer.TheInstance.SecurityManager;
					FSEyeObject serverGroupListObject = sm.Get(SecurityObject.FullPath + SecurityManager.ObjectPathDelimiter + "GameServerList");
					FSEyeObject originalParentObject = modifyGroup.SecurityObject.Parent;
					FSEyeObject modifyGroupObject = modifyGroup.SecurityObject;

					//û�и��������飬���޸�Ϊ����������飻��������ԭ�и���������������µĸ���������
					if (newParentGroup == null)
					{
						//�������Ƕ���������飻���򣬲���Ҫ�ı�
						if (originalParentObject != serverGroupListObject)
						{
							//������ȫ����
							originalParentObject.RemoveChild(modifyGroupObject.Name);
							serverGroupListObject.AddChild(modifyGroupObject);

							//��ԭ���ķ���������ɾ��
							ServerGroup originalParent = GetGameServerGroup(originalParentObject.Id);
							originalParent.List.Remove(modifyGroup);

							//��ӵ�������������б���
							_topServerGroupList.Add(modifyGroup);
						}
					}
					else
					{						
						FSEyeObject newParentObject = newParentGroup.SecurityObject;
						//���������鷢���˱仯�����򣬲���Ҫ�ı�
						if (originalParentObject != newParentObject)
						{
							originalParentObject.RemoveChild(modifyGroupObject.Name);
							newParentObject.AddChild(modifyGroupObject);

							//ԭ�����Ƕ���������飬��ԭ���ĸ�����������ɾ�������򣬴Ӷ�����������б���ɾ��
							if (originalParentObject != serverGroupListObject)
							{
								ServerGroup originalParent = GetGameServerGroup(originalParentObject.Id);
								originalParent.List.Remove(modifyGroup);
							}
							else
							{
								_topServerGroupList.Remove(modifyGroup);
							}

							//��ӵ��µĸ�����������
							newParentGroup.List.Add(modifyGroup);

							//���游��������İ�ȫ����
							sm.Set(newParentObject, db);
						}
					}

					//����������ȫ����
					sm.Set(serverGroupListObject, db);
					sm.Set(originalParentObject, db);
					sm.Set(modifyGroupObject, db);

					db.CommitTrans();
					return true;
				}
				catch (Exception ex)
				{
					if (db != null)
						db.RollbackTrans();

					throw ex;
				}
			}
		}

		/// <summary>
		/// �ƶ�����������
		/// </summary>
		/// <param name="group"></param>
		public void MoveToServerGroup(GameServer server, ServerGroup newGroup)
		{
			if (server == null)
				throw new ArgumentNullException("server");
			if (newGroup == null)
				throw new ArgumentNullException("newGroup");
			if (newGroup.GroupType == ServerGroup.Type.Group && newGroup.List.Count > 0)
				throw new Exception(string.Format("ServerGroup {0} contains server groups.", newGroup.Name));

			ServerGroup oldGroup = server.Group;
			if (oldGroup != null)
			{
				using (IBlazeDatabase db = DbFactory.GetDatabase())
				{
					try
					{
						db.BeginTrans();
						SecurityManager sm = AdminServer.TheInstance.SecurityManager;						

						oldGroup.List.Remove(server);
						oldGroup.SecurityObject.RemoveChild(server.SecurityObject.Name);
						sm.Set(oldGroup.SecurityObject, db);

                        //�����server���ڵ�groupû��server�˾Ͱ�����ΪGroup�͵�
                        if (oldGroup.SecurityObject.Children.Length == 0)
                        {
                            oldGroup.GroupType = ServerGroup.Type.Group;                            
                            db.GetTable(TableString.ServerGroupTableName).Set(TableString.ServerGroupFieldType, (int)oldGroup.GroupType, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, oldGroup.Id));
                        }

						if (newGroup.GroupType == ServerGroup.Type.Group)
						{
							newGroup.GroupType = ServerGroup.Type.Server;
						}

						IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
						serverGroupTable.Set(TableString.ServerGroupFieldType, (int)newGroup.GroupType, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, newGroup.Id));

                        newGroup.List.Add(server);
						newGroup.SecurityObject.AddChild(server.SecurityObject);
						sm.Set(newGroup.SecurityObject, db);

						server.Group = newGroup;

						sm.Set(server.SecurityObject, db);
						db.CommitTrans();
					}
					catch (Exception ex)
					{
						if (db != null)
							db.RollbackTrans();

                        throw ex;
					}
				}
			}
		}

		/// <summary>
		/// ʵ����IMessageHandler�ӿ�
		/// </summary>
		public void ProcessMessage(GameServer server, IProtocol message)
		{
			if (server != null)
			{
				server.OnMessageReceived(message);
			}
		}

		public void Dispose()
		{
			lock (this)
			{
				if (!_dispose)
				{
					_dispose = true;
					GC.SuppressFinalize(this);

					Shutdown();
					_systemTickTimer.Close();
				}
			}
		}

        /// <summary>
        /// GameServer�Ĳ���Ĳ������
        /// </summary>
        /// <param name="plugIns"></param>
        public bool InsertPlugIn(GameServer server, IList plugIns)
        {
            IBlazeDatabase db = null;
            //�������ݵ���������
            string[] fields = new string[] {
                    TableString.ServerPlugInFieldGameServerId,
                    TableString.ServerPlugInFieldPlugInGuid,
                    TableString.ServerPlugInFieldParameter1,
                    TableString.ServerPlugInFieldParameter2,
                    TableString.ServerPlugInFieldParameter3,
                    TableString.ServerPlugInFieldPlugInName
                };
            try
            {
                db = DbFactory.GetDatabase();
                db.BeginTrans();
                foreach (IPlugIn plugIn in plugIns)
                {                    
                    IBlazeTable plugInTable = db.GetTable(TableString.ServerPlugInTableName);
                    plugInTable.Add(fields, ServerPlugInFactory.GetData(server.Id, plugIn));
                    server.InstallPlugIn(plugIn);
                }
                db.CommitTrans();
                return true;
            }
            catch (Exception)
            {
                if (db != null)
                    db.RollbackTrans();
                return false;                
            }
            finally
            {
                if (db != null)
                    db.Close();
            }            
        }

        /// <summary>
        /// GameServer�Ĳ�����޸Ĳ���
        /// </summary>
        /// <param name="plugIns"></param>
        public bool EditPlugIn(GameServer server, IList plugIns)
        {
            IBlazeDatabase db = null;
            //�������ݵ���������
            string[] fields = new string[] {
                    TableString.ServerPlugInFieldGameServerId,
                    TableString.ServerPlugInFieldPlugInGuid,
                    TableString.ServerPlugInFieldParameter1,
                    TableString.ServerPlugInFieldParameter2,
                    TableString.ServerPlugInFieldParameter3,
                    TableString.ServerPlugInFieldPlugInName
                };
            try
            {
                db = DbFactory.GetDatabase();
                db.BeginTrans();
                foreach (IPlugIn plugIn in plugIns)
                {                    
                    IBlazeTable plugInTable = db.GetTable(TableString.ServerPlugInTableName);
                    plugInTable.Set(fields, ServerPlugInFactory.GetData(server.Id, plugIn),
                        FilterFactory.CreateAndFilter(
                        FilterFactory.CreateEqualFilter(TableString.ServerPlugInFieldGameServerId, server.Id),
                        FilterFactory.CreateEqualFilter(TableString.ServerPlugInFieldPlugInGuid, plugIn.Guid)));
                    //server.UnInstallPlugIn(plugIn);
                    //server.InstallPlugIn(plugIn);
                }
                db.CommitTrans();
                return true;
            }
            catch (Exception)
            {
                if (db != null)
                    db.RollbackTrans();
                return false;
            }
            finally
            {
                if (db != null)
                    db.Close();
            }
        }

        /// <summary>
        /// GameServer�Ĳ����ɾ������
        /// </summary>
        /// <param name="plugIns"></param>
        public bool DeletePlugIn(GameServer server,IList plugIns)
        {
            IBlazeDatabase db = null;
            try
            {                
                db = DbFactory.GetDatabase();
                db.BeginTrans();
                foreach (IPlugIn plugIn in plugIns)
                {
                    
                    IBlazeTable plugInTable = db.GetTable(TableString.ServerPlugInTableName);
                    plugInTable.Delete(FilterFactory.CreateAndFilter(
                        FilterFactory.CreateEqualFilter(TableString.ServerPlugInFieldGameServerId,server.Id),
                        FilterFactory.CreateEqualFilter(TableString.ServerPlugInFieldPlugInGuid,plugIn.Guid)));                                        
                    server.UnInstallPlugIn(plugIn);
                }
                db.CommitTrans();
                return true;
            }
            catch (Exception)
            {
                if (db != null)
                    db.RollbackTrans();
                return false;
            }
            finally
            {
                if (db != null)
                    db.Close();
            }
        }
        
        /// <summary>
        /// �Է��������Ƿ�������Ĳ���д��־�����ش˴β�������ˮ�ţ��ڲ��Զ����ɵģ�
        /// targetId = -2��ʾ�Զ���鿪ʼ���в����������������description�ֶ���
        /// </summary>
        public Int64 WriteServerOperationLog(int userId, ServerGroup.Type targetType, int targetId, GameServer.ServerOperationType operationType, bool operationSuccessful, string description,string region)
        {
            System.Random random = new Random(new RNGCryptoServiceProvider().GetHashCode());
            Int64 OperationID = Int64.Parse(DateTime.Now.Year.ToString() + DateTime.Now.Month.ToString() + DateTime.Now.Day.ToString() + DateTime.Now.Hour.ToString() + DateTime.Now.Minute.ToString() + DateTime.Now.Millisecond.ToString() + random.Next(1, 100));
            random = null;

            using (IBlazeDatabase db = DbFactory.GetDatabase())
            {
                IBlazeTable table = db.GetTable(TableString.ServerOperationLogTableName);

                string[] fieldNames = {
					        TableString.ServerOperationLogFieldOperationId,
					        TableString.ServerOperationLogFieldUserId,
					        TableString.ServerOperationLogFieldServerOrGroup,
                            TableString.ServerOperationLogFieldTargetId,
                            TableString.ServerOperationLogFieldOperationType,
                            TableString.ServerOperationLogFieldOperationResult,
                            TableString.ServerOperationLogFieldDescription,
                            TableString.ServerOperationLogFieldLogTime,
                            TableString.ServerOperationLogFieldFatherRegion
				    };

                object[] fieldValues = {
				        OperationID,
				        userId,
				        (Int16)targetType,
                        targetId,
                        (Int16)operationType,
                        Convert.ToInt16(operationSuccessful),
                        description,
                        DateTime.Now,
                        region
			        };

                //��Ӵ˴εĲ�����־
                table.Add(fieldNames, fieldValues);                

                return OperationID;
            }
        }
        
        /// <summary>
        /// �Է��������Ƿ�������Ĳ���д��־��������Ҫָ��������ˮ��
        /// </summary>
        public void WriteServerOperationLog(Int64 operationId, int userId, ServerGroup.Type targetType, int targetId, GameServer.ServerOperationType operationType, bool operationSuccessful, string description, string region)
        {
            using (IBlazeDatabase db = DbFactory.GetDatabase())
            {
                IBlazeTable table = db.GetTable(TableString.ServerOperationLogTableName);

                string[] fieldNames = {
					        TableString.ServerOperationLogFieldOperationId,
					        TableString.ServerOperationLogFieldUserId,
					        TableString.ServerOperationLogFieldServerOrGroup,
                            TableString.ServerOperationLogFieldTargetId,
                            TableString.ServerOperationLogFieldOperationType,
                            TableString.ServerOperationLogFieldOperationResult,
                            TableString.ServerOperationLogFieldDescription,
                            TableString.ServerOperationLogFieldLogTime,
                            TableString.ServerOperationLogFieldFatherRegion
				    };

                object[] fieldValues = {
                        operationId,
                        userId,
				        (Int16)targetType,
                        targetId,
                        (Int16)operationType,
                        Convert.ToInt16(operationSuccessful),
                        description,
                        DateTime.Now,
                        region
			        };

                //��Ӵ˴εĲ�����־
                table.Add(fieldNames, fieldValues);
            }
        }

		public void GameStateChanged(GameServer server, GameServer.ServiceState originalState, GameServer.ServiceState newState)
		{
            GameServer.ServerEventType eventType = GameServer.ServerEventType.UnknownEvent;

            switch (originalState)
                {
                    case GameServer.ServiceState.Starting:
                        {
                            if (newState == GameServer.ServiceState.Running)
                            {
                                //��Ϸ��������
                                server.AppendMessage("Start", GameServer.MessageInfo.Type.Success);

                                AutomationContext context = new AutomationContext();
                                context.Message = string.Format("{0} Start", server.ToString()); ;
                                context.Server = server;
                                AdminServer.TheInstance.StrategyManager.OnEvent(FSEyeEvent.GameStart, context);

                                eventType = GameServer.ServerEventType.GameStart;
                            }
                            else if (newState == GameServer.ServiceState.ErrorStopped)
                            {
                                //��Ϸ����ʧ��
                                server.AppendMessage("StartFail", GameServer.MessageInfo.Type.Failure);

                                AutomationContext context = new AutomationContext();
                                context.Message = string.Format("{0} StartFail", server.ToString()); ;
                                context.Server = server;
                                AdminServer.TheInstance.StrategyManager.OnEvent(FSEyeEvent.StartGameFail, context);

                                eventType = GameServer.ServerEventType.GameStartFail;
                            }
                        } break;
                    case GameServer.ServiceState.Stopping:
                        {
                            if (newState == GameServer.ServiceState.OKStopped || newState == GameServer.ServiceState.ErrorStopped)
                            {
                                //ֹͣ��ָweb�����رշ���������
                                //��Ϸ�����ر�
                                server.AppendMessage("Stop", GameServer.MessageInfo.Type.Alert);

                                AutomationContext context = new AutomationContext();
                                context.Message = string.Format("{0} Stop", server.ToString()); ;
                                context.Server = server;
                                AdminServer.TheInstance.StrategyManager.OnEvent(FSEyeEvent.GameStop, context);

                                eventType = GameServer.ServerEventType.GameStop;
                            }
                        } break;
                    case GameServer.ServiceState.ErrorStopped:
                    case GameServer.ServiceState.OKStopped :
                        {
                            if (newState == GameServer.ServiceState.Running)
                            {
                                //��Ϸ��������
                                server.AppendMessage("StartByAccident", GameServer.MessageInfo.Type.Failure);

                                AutomationContext context = new AutomationContext();
                                context.Message = string.Format("{0} StartByAccident", server.ToString()); ;
                                context.Server = server;
                                AdminServer.TheInstance.StrategyManager.OnEvent(FSEyeEvent.GameStartByAccident, context);

                                eventType = GameServer.ServerEventType.GameStartByAccident;
                            }
                        } break;
                    case GameServer.ServiceState.Running:
                        {
                            if (newState == GameServer.ServiceState.ErrorStopped)
                            {
                                //��Ϸ����
                                server.AppendMessage("StopByAccident", GameServer.MessageInfo.Type.Failure);

                                AutomationContext context = new AutomationContext();
                                context.Message = string.Format("{0} StopByAccident", server.ToString()); ;
                                context.Server = server;
                                AdminServer.TheInstance.StrategyManager.OnEvent(FSEyeEvent.GameStopByAccident, context);

                                eventType = GameServer.ServerEventType.GameStopByAccident;
                            }
                        } break;
                }

                if (eventType != GameServer.ServerEventType.UnknownEvent)
                {
                    DateTime eventTime = DateTime.Now;

                    using (IBlazeDatabase db = DbFactory.GetDatabase())
                    {
                        IBlazeTable table = db.GetTable(TableString.ServerEventTableName);

                        string[] fieldNames = {
					TableString.ServerEventFieldServerId,
					TableString.ServerEventFieldEventTime,
					TableString.ServerEventFieldEventType
				    };

                        object[] fieldValues = {
					server.Id,
					eventTime,
					(Int16)eventType
				    };

                        table.Add(fieldNames, fieldValues);
                    }
                }
            
		}

		/// <summary>
		/// �����������б�����ļ�
		/// </summary>
		public void CreateServerListFiles(GameServer[] servers)
		{
			CreateServerListFiles(servers, SystemConfig.Current.AdminServerUploadFileRootPath, SystemConfig.Current.DefaultEncoding);
		}

		/// <summary>
		/// �����������б�����ļ�
		/// </summary>
		/// <param name="fileDir">�����ļ���Ŀ¼</param>
		/// <param name="encoding">����</param>
		/// <remarks>�����б��ļ����б��ļ�У���ļ�</remarks>
		public void CreateServerListFiles(GameServer[] servers, string fileDir, Encoding encoding)
		{
			if (fileDir == null)
				throw new ArgumentNullException("fileDir");

			Encoding usingEncoding = (encoding != null ? encoding : SystemConfig.Current.DefaultEncoding);

			string filePath = fileDir + SystemConfig.Current.ServerListFileName;
			using (FileStream fileStream = new FileStream(filePath, FileMode.Create))
			{
				CreateServerListFile(servers, fileStream, usingEncoding);
			}

			using (FileStream fileStream = new FileStream(filePath, FileMode.Open))
			{
				string crcFilePath = fileDir + SystemConfig.Current.ServerListCRCFileName;
				using (TextWriter crcWriter = new StreamWriter(crcFilePath, false, usingEncoding))
				{
					long crc32 = CryptographyManager.TheInstance.CRC32(fileStream);
					crcWriter.Write(crc32);
				}
			}
		}

        /// <summary>
        /// �����������б�����ļ�
        /// </summary>
        /// <param name="fileDir">�����ļ���Ŀ¼</param>
        /// <param name="encoding">����</param>
        /// <remarks>�����б��ļ����б��ļ�У���ļ�</remarks>
        public void CreateServerListFiles(List<MapNode> netList, string fileDir, Encoding encoding)
        {
            if (fileDir == null)
                throw new ArgumentNullException("fileDir");

            Encoding usingEncoding = (encoding != null ? encoding : SystemConfig.Current.DefaultEncoding);

            string filePath = fileDir + SystemConfig.Current.ServerListFileName;
            using (FileStream fileStream = new FileStream(filePath, FileMode.Create))
            {
                CreateServerListFile(netList, fileStream, usingEncoding);
            }

            if (SystemConfig.Current.PakFileName.ToLower() != SystemConfig.Current.ServerListFileName)
            {
                CreatePakFile();
            }

            using (FileStream fileStream = new FileStream(filePath, FileMode.Open))
            {
                string crcFilePath = fileDir + SystemConfig.Current.ServerListCRCFileName;
                using (TextWriter crcWriter = new StreamWriter(crcFilePath, false, usingEncoding))
                {
                    long crc32 = CryptographyManager.TheInstance.CRC32(fileStream);
                    crcWriter.Write(crc32);
                }
            }
        }

        /// <summary>
        /// ����pak��
        /// </summary>
        public void CreatePakFile()
        {
            Process p = new Process();
            p.StartInfo.FileName = "cmd.exe";
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardInput = true;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.CreateNoWindow = true;
            p.Start();

            string strOutput = null;
            string cmdText = ShellScriptManager.TheInstance.ScriptRootPath + "windows" + GameServer.FilePathWinDelimiter + "pack.cmd";
            p.StandardInput.WriteLine(cmdText + " " + SystemConfig.Current.ServerListFileDir);

            p.StandardInput.WriteLine("exit");
            strOutput = p.StandardOutput.ReadToEnd();
            Console.WriteLine(strOutput);
            p.WaitForExit();
            p.Close();
        }

		/// <summary>
		/// �����������б��ļ�
		/// </summary>
		public void CreateServerListFile(GameServer[] servers, Stream outputStream)
		{
			CreateServerListFile(servers, outputStream, null);
		}

		/// <summary>
		/// �����������б��ļ�
		/// </summary>
		public void CreateServerListFile(GameServer[] servers, Stream outputStream, Encoding encoding)
		{
			if (outputStream == null)
				throw new ArgumentNullException("outputStream");

			IList<GameServer> displayServerList = new List<GameServer>(servers);

			try
			{
				using (TextWriter writer = new StreamWriter(outputStream, (encoding != null ? encoding : SystemConfig.Current.DefaultEncoding)))
				{
					writer.NewLine = SystemConfig.Current.DefaultNewLine;

					IList netGroupList = TopServerGroups;
										
					writer.WriteLine("[List]");
					writer.WriteLine("Version=1");
					writer.WriteLine("NetCount={0}", netGroupList.Count);
					writer.WriteLine();

					for (int netIndex = 0; netIndex < netGroupList.Count; netIndex++)
					{
						ServerGroup netGroup = netGroupList[netIndex] as ServerGroup;

						if (netGroup.GroupType == ServerGroup.Type.Group)
						{
							IList regionGroupList = netGroup.List;
							IList<ServerGroup> displayRegionList = new List<ServerGroup>();

							foreach (ServerGroup regionGroup in regionGroupList)
							{
								if (ExistDisplayServer(regionGroup, displayServerList))
								{
									displayRegionList.Add(regionGroup);
								}
							}

							writer.WriteLine("[Net_{0}]", netIndex);
							writer.WriteLine("NetName={0}", netGroup.Name);
							writer.WriteLine("RegionNum={0}", displayRegionList.Count);
							writer.WriteLine();

							for (int regionIndex = 0; regionIndex < displayRegionList.Count; regionIndex++)
							{
								ServerGroup regionGroup = displayRegionList[regionIndex] as ServerGroup;

								if (regionGroup.GroupType == ServerGroup.Type.Server)
								{
									IList serverList = regionGroup.List;
									IList<GameServer> displayerServerListInThisRegion = new List<GameServer>();

									foreach (GameServer server in serverList)
									{
										if (displayServerList.Contains(server))
										{
											displayerServerListInThisRegion.Add(server);
										}
									}

									writer.WriteLine("[Net_{0}_Region_{1}]", netIndex, regionIndex);
									writer.WriteLine("RegionName={0}", regionGroup.Name);
									writer.WriteLine("ServerNum={0}", displayerServerListInThisRegion.Count);
									writer.WriteLine();

									for (int serverIndex = 0; serverIndex < displayerServerListInThisRegion.Count; serverIndex++)
									{
										GameServer server = displayerServerListInThisRegion[serverIndex] as GameServer;

                                        writer.WriteLine("{0}_Server_Name={1}", serverIndex, server.Name);
										writer.WriteLine("{0}_Server_State={1}", serverIndex, "Good");
										writer.WriteLine("{0}_Server_IP={1}", serverIndex, server.IpAddress);
										writer.WriteLine();
									}
								}
								else
								{
									throw new Exception("Invalid GameServerGroup structure: RegionGroup must be consist of Servers.");
								}
							}
						}
						else
						{
							throw new Exception("Invalid GameServerGroup structure: NetGroup must be consist of RegionGroups.");
						}
					}
				}
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}

		bool ExistDisplayServer(ServerGroup regionGroup, IList<GameServer> displayServerList)
		{
			if (regionGroup.GroupType == ServerGroup.Type.Server)
			{
				IList serverList = regionGroup.List;

				foreach (GameServer server in serverList)
				{
					if (displayServerList.Contains(server))
					{
						return true;
					}
				}

				return false;
			}

			return false;
		}


        /// <summary>
        /// �����������б��ļ�
        /// </summary>
        public void CreateServerListFile(List<MapNode> netList, Stream outputStream, Encoding encoding)
        {
            if (outputStream == null)
                throw new ArgumentNullException("outputStream");

            try
            {
                using (TextWriter writer = new StreamWriter(outputStream, (encoding != null ? encoding : SystemConfig.Current.DefaultEncoding)))
                {
                    writer.NewLine = SystemConfig.Current.DefaultNewLine;

                    writer.WriteLine("[List]");
                    writer.WriteLine("Version=1");
                    writer.WriteLine("AdviceRegion=1");
                    writer.WriteLine("RegionCount={0}", netList.Count);
                    for (int netIndex = 0; netIndex < netList.Count; netIndex++)
                    {
                        MapNode net = netList[netIndex];
                        if (net.MapType == ServerMap.ServerMapType.Net)
                        {
                            MapNode[] displayRegionList = net.Children;// SplitHide(net.Children);
                            writer.WriteLine("Region_{0}={1}", netIndex + 1, net.Name);
                        }
                    }
                    writer.WriteLine();

                    for (int netIndex = 0; netIndex < netList.Count; netIndex++)
                    {
                        MapNode net = netList[netIndex];
                        if (net.MapType == ServerMap.ServerMapType.Net)
                        {
                            MapNode[] displayRegionList = net.Children;// SplitHide(net.Children);
                            writer.WriteLine("[Region_{0}]", netIndex + 1);
                         //   writer.WriteLine("Count={0}", displayRegionList.Length);

                            int nServerCount = 0;
                            for (int regionIndex = 0; regionIndex < displayRegionList.Length; ++regionIndex)
                            {
                                MapNode region = displayRegionList[regionIndex];

                                if (region.MapType == ServerMap.ServerMapType.Region)
                                {
                                    MapNode[] displayServerList = region.Children;    //
                                    
                                    for (int serverIndex = 0; serverIndex < displayServerList.Length; serverIndex++)
                                    {
                                        MapNode serverNode = displayServerList[serverIndex];

                                        if (serverNode.MapType == ServerMap.ServerMapType.Server)
                                        {
                                            if (serverNode.Hide == false)
                                            {
                                                GameServer server = GetGameServer(serverNode.MapServerId);

                                                if (server != null)
                                                {
                                                    nServerCount++;
                                                    writer.WriteLine("{0}_Title={1}", nServerCount, GetServerNameDesc(serverNode));
                                                    writer.WriteLine("{0}_State={1}", nServerCount, GetServerStateDesc(server.IsConnected ? server.ServerReadableState : GameServer.ReadableState.Stop));     //ʹ��Ĭ��״̬
                                                    writer.WriteLine("{0}_Address={1}", nServerCount, server.IpAddress);
                                                    writer.WriteLine("{0}_GatewayName={1}", nServerCount, server.Group.GatewayName);
                                                }
                                                else
                                                {
                                                    //throw new Exception(string.Format("Invalid GameServer : GameServer with id {0} doesn't exist", serverNode.MapServerId));
                                                    
                                                    //�����server�����ڣ���ɾ�����ˣ�����serverlist���û������Ӧ��ɾ��
                                                    //���Զ�ɾ�������serverNode�����µ��ڴ�����ݿ�
                                                    //��Ӱ��serverlist�ļ������ɣ�������ִ����ȥ
                                                    MapNode parentNode = serverNode.Parent;
                                                    parentNode.RemoveChild(serverNode);
                                                    //���浽�ڴ�����ݿ�
                                                    _serverMap.SaveServerMapList(netList);
                                                }
                                            }
                                        }
                                        else
                                        {
                                            throw new Exception("Invalid RegionGroup structure: RegionGroup must consist of GameServers");
                                        }
                                    }
                                }
                                else
                                {
                                    throw new Exception("Invalid RegionGroup structure: NetGroup must consist of RegionGroups");
                                }
                            }
                            writer.WriteLine("Count={0}", nServerCount);
                            writer.WriteLine();

                        }
                        else
                        {
                            throw new Exception("Invalid NetGroup structure: NetGroup must be on top");
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //public void CreateServerStateFile(List<MapNode> netList, string fileDir, Encoding encoding)
        //{
        //    if (fileDir == null)
        //        throw new ArgumentNullException("fileDir");

        //    Encoding usingEncoding = (encoding != null ? encoding : SystemConfig.Current.DefaultEncoding);

        //    string stateFilePath = fileDir + "state.ini";
        //    using (FileStream fileStream = new FileStream(stateFilePath, FileMode.Create))
        //    {
        //        CreateServerStateFile(netList, fileStream, usingEncoding);
        //    }
        //}

        public void CreateServerStateFile(List<MapNode> netList, Stream outputStream, Encoding encoding)
        {
            using (TextWriter writer = new StreamWriter(outputStream, (encoding != null ? encoding : SystemConfig.Current.DefaultEncoding)))
            {
                for (int netIndex = 0; netIndex < netList.Count; netIndex++)
                {
                    MapNode net = netList[netIndex];
                    if (net.MapType == ServerMap.ServerMapType.Net)
                    {
                        MapNode[] displayRegionList = net.Children;// SplitHide(net.Children);
                        for (int regionIndex = 0; regionIndex < displayRegionList.Length; ++regionIndex)
                        {
                            MapNode region = displayRegionList[regionIndex];

                            if (region.MapType == ServerMap.ServerMapType.Region)
                            {
                                MapNode[] displayServerList = region.Children;    //SplitHide

                                for (int serverIndex = 0; serverIndex < displayServerList.Length; serverIndex++)
                                {
                                    MapNode serverNode = displayServerList[serverIndex];

                                    if (serverNode.MapType == ServerMap.ServerMapType.Server)
                                    {
                                        GameServer server = GetGameServer(serverNode.MapServerId);
                                        if (server != null)
                                        {
                                            writer.WriteLine("[{0}]", GetServerNameDesc(serverNode));
                                            writer.WriteLine("State={0}", GetServerStateDesc(server.IsConnected ? server.ServerReadableState : GameServer.ReadableState.Stop));
                                            
                                            writer.WriteLine();
                                        }
                                        else
                                        {
                                            throw new Exception(string.Format("Invalid GameServer : GameServer with id {0} doesn't exist", serverNode.MapServerId));
                                        }
                                    }
                                    else
                                    {
                                        throw new Exception("Invalid RegionGroup structure: RegionGroup must consist of GameServers");
                                    }
                                }
                            }
                            else
                            {
                                throw new Exception("Invalid RegionGroup structure: NetGroup must consist of RegionGroups");
                            }
                        }
                    }
                    else
                    {
                        throw new Exception("Invalid NetGroup structure: NetGroup must be on top");
                    }
                }
            }
        }

        /// <summary>
        /// �õ�������״̬����
        /// </summary>
        /// <param name="server">������</param>
        public string GetServerStateDesc(GameServer.ReadableState serverState)
        {
            string stateDesc = string.Empty;
            switch (serverState)
            {
                case GameServer.ReadableState.Good:
                    stateDesc = StringDef.StateGood;
                    break;
                case GameServer.ReadableState.Busy:
                    stateDesc = StringDef.StateBusy;
                    break;
                case GameServer.ReadableState.Full:
                    stateDesc = StringDef.StateFull;
                    break;
                case GameServer.ReadableState.Stop:
                    stateDesc = StringDef.StateStoped;
                    break;
            }
            return stateDesc;
        }

        /// <summary>
        /// �õ���������������
        /// </summary>
        /// <param name="serverNode">������ӳ����</param>
        public string GetServerNameDesc(MapNode serverNode)
        {
            StringBuilder serverNameBuilder = new StringBuilder();
            serverNameBuilder.Append("��" + serverNode.Name + "��");
            if (serverNode.New)
            {
                serverNameBuilder.Append(StringDef.ServerFlagNew);
            }
            if (serverNode.Recommend)
            {
                //ֻ����1��flag����Ϊ�ͻ��˻���ʾ����
                //����New�����ȼ�����Recommand
                //���������flag���У���ֻ����New
                //if (serverNode.New == false)
                //{
                    serverNameBuilder.Append(StringDef.ServerFlagRecommend);
                //}
            }
            return serverNameBuilder.ToString();
        }

		void SendConfigPackage(GameServer server)
		{
			string fileContentString = null;
			using (MemoryStream configStream = new MemoryStream())
			{
				server.LordConfig.WriteGuardConfig(configStream);
                fileContentString = SystemConfig.Current.DefaultEncoding.GetString(configStream.ToArray()); //Encoding.ASCII.GetString(configStream.ToArray());
			}

			int x = s_random.Next(1000);
			int y = Util.CaculateY(x);

			string encryptFileContent = fileContentString;
			//string encryptFileContent = string.Empty;
			//util.DBEncrypt(fileContentString, ref encryptFileContent);

			string md5 = UtilDllWrap.MD5(y.ToString(), SystemConfig.Current.DefaultEncoding);

			e2g_ConfigInfo configInfo = new e2g_ConfigInfo();
			configInfo.X = x;
			configInfo.Y = md5;
			configInfo.ConfigFileDataLength = (ushort)encryptFileContent.Length;
			configInfo.ConfigFileData = encryptFileContent;
			//configProtocol.X = x;
			//configProtocol.Y = md5;
			//configProtocol.ConfigFileDataLength = (ushort)encryptFileContent.Length;
			//configProtocol.ConfigFileData = encryptFileContent;

			server.CommunicationKey = UtilDllWrap.CreateRandomKey(8);

            byte[] encryptedKey = server.CommunicationKey;//romandou UtilDllWrap.RSAEncrypt(server.CommunicationKey);
            byte[] encryptedConfigInfo = configInfo.ToBytes();//romandou UtilDllWrap.RC4Encrypt(configInfo.ToBytes(), server.CommunicationKey);

			e2g_config configProtocol = new e2g_config();
			Array.Copy(encryptedKey, configProtocol.Key.Data, encryptedKey.Length);
			Array.Copy(encryptedConfigInfo, configProtocol.ConfigInfo, encryptedConfigInfo.Length);

			IPEndPoint target = new IPEndPoint(IPAddress.Parse(server.IpAddress), SystemConfig.Current.ConfigGuardPort);
			if (AdminServer.TheInstance.ConnectionManager.SendTo(target, configProtocol.ToBytes()))
				server.AppendMessage(StringDef.AttemptToConfigGuard, GameServer.MessageInfo.Type.Normal);
		}

        public void SendServerConfigFile(GameServer svr)
        {
            //serve����״̬
            //if (server.ConfigState == GameServer.ConfigGuardState.Configuring)
            foreach(GameServer server in _gameServerList)
            {
                if (server.IpAddress != svr.IpAddress)
                    continue;

                string[] para = new string[3];
                para[0] = server.Id.ToString();
                para[1] = server.ConfigContent;
                para[2] = server.GameDir;

                if (server.Type == GameServer.ServerType.bishop) para[2] += JX2Lib.JXConfigFileName.Gateway;
                if (server.Type == GameServer.ServerType.gamecenter) para[2] += JX2Lib.JXConfigFileName.GameCenter;
                if (server.Type == GameServer.ServerType.gameserver) para[2] += JX2Lib.JXConfigFileName.GameServer;
                if (server.Type == GameServer.ServerType.goddess) para[2] += JX2Lib.JXConfigFileName.RoleServer;
                if (server.Type == GameServer.ServerType.unidentified) para[2] += JX2Lib.JXConfigFileName.Unidentified;
                if (server.Type == GameServer.ServerType.logserver) para[2] += JX2Lib.JXConfigFileName.LogServer;

                //���÷�������cgf�ļ�
                if (server.DoPlugInAction(0, E2gSendfile.PlugInGuid, E2gSendfile.actionKey, para))
                {
                    server.AppendMessage("Transfer Server Config File:" + para[2], GameServer.MessageInfo.Type.Normal);
                    //para[1] = "[Server]" + SystemConfig.Current.DefaultNewLine + "Server=" + server.Id.ToString() + SystemConfig.Current.DefaultNewLine;
                    //para[2] = server.GameDir + "shm.ini";
                    //server.AppendMessage("Transfer Server ShareMemery File:" + para[2], GameServer.MessageInfo.Type.Normal);
                    //���÷�������shm.ini
                    //if (server.DoPlugInAction(0, E2gSendfile.PlugInGuid, E2gSendfile.actionKey, para))
                    //{
                    //    server.ConfigState = GameServer.ConfigGuardState.Configured;
                    //    server.AppendMessage(StringDef.StopConfigGuard, GameServer.MessageInfo.Type.Normal);
                    //}
                }
            }

        }

		void ProcessConfigGuard()
		{
			while (!_dispose)
			{
				if (_isConfigingGuard)
				{
					try
					{
						int serverCount = _gameServerList.Count;
						for (int serverIndex = 0; serverIndex < serverCount; serverIndex++)
						{
							GameServer server = _gameServerList[serverIndex] as GameServer;
							if (server.ConfigState == GameServer.ConfigGuardState.Configuring)
							{
								try
								{
                                    //���¼���GUARD������ķ�����
                                    int iServerCount = 0;
                                    string sServerIDs = "";
                                    int nFirstConfigServerID = 0; 
                                    foreach (GameServer svr in AdminServer.TheInstance.GameServerManager.Servers)
                                    {
                                        if (server.IpAddress == svr.IpAddress)
                                        {
                                            if (svr.ConfigState == GameServer.ConfigGuardState.Configuring && nFirstConfigServerID == 0)
                                            {
                                                nFirstConfigServerID = svr.Id;
                                            }
                                            
                                            if (iServerCount == 0)
                                                sServerIDs = svr.Id.ToString();
                                            else
                                                sServerIDs = sServerIDs + "," + svr.Id.ToString();

                                            iServerCount++;
                                        }
                                    }

                                    server.LordConfig.ServerCount = iServerCount;
                                    server.LordConfig.ServerIDs = sServerIDs;
                                    server.LordConfig.BindIp = server.IpAddress;
                                    //ͬipserver��˳������
                                    if (server.Id != nFirstConfigServerID)
                                    {
                                        server.AppendMessage("waiting config " + nFirstConfigServerID.ToString() , GameServer.MessageInfo.Type.Success);
                                        continue;
                                    }

                                    //��UDP
                                    e2g_switchmode switchMode = new e2g_switchmode();
                                    switchMode.Mode = 1;
                                    switchMode.wServer = (UInt16)server.Id;
                                    server.SendMessage(switchMode);
                                    server.AppendMessage(StringDef.SwitchGuardMode, GameServer.MessageInfo.Type.Success);
                                    
                                    if (server.LordConfig.FSEyeIp != null && server.LordConfig.FSEyePort >= 0 && server.ConfigContent!=null)
                                    {
                                        SendConfigPackage(server);
                                        //Thread.Sleep(2000);
                                        //SendServerConfigFile(server);
                                    }
                                    else
                                    {
                                        server.ConfigState = GameServer.ConfigGuardState.Configured;
                                    }
								}
								catch (Exception exinner)
								{
									Util.DebugLog("ProcessConfigGuard.SendConfigPackage: " + exinner.Message);
								}
							}

							if (!_isConfigingGuard)
								break;

							Thread.Sleep(100);
						}
					}
					catch (Exception ex)
					{
						Util.DebugLog("ProcessConfigGuard: " + ex.Message);
					}

					Thread.Sleep(15000);
				}

				Thread.Sleep(1000);
			}
		}

        protected void GroupPlayerCount2File(object sender, EventArgs e)
        {
            string playerCountLog = string.Empty;
            
            for (int i = 0; i < _gameServerList.Count; i++)
            {
                GameServer server = _gameServerList[i] as GameServer;

                //��gc����һ����ĺ���������
                if (server.Type == GameServer.ServerType.gamecenter)
                {                  
                    int groupNowPlayer = 0;

                    if (server.GameServiceState == GameServer.ServiceState.Running)
                    {
                        groupNowPlayer = (int)server.GetPlugInData(AdminServer.TheInstance.AutomationManager.SecurityObject.Id, LordControl.PlugInGuid, LordControl.DataKeyCurrentPlayerCount);
                        playerCountLog += string.Format("{0}\t{1}\t{2}\n", server.Group.SecurityObject.Parent.Name, server.Group.Name, groupNowPlayer);
                    }
                }
            }
            if (playerCountLog != string.Empty)
            {
                string playerCountLogDir = SystemConfig.Current.GroupPlayerCountDir;
                string playerCountFileName = SystemConfig.Current.GroupPlayerCountFileName;

                if (playerCountLogDir != null && playerCountFileName != null)
                {
                    using (StreamWriter writer = new StreamWriter(playerCountLogDir + playerCountFileName, false, Encoding.GetEncoding("gb2312")))
                    {
                        writer.Write(playerCountLog);
                        writer.Flush();
                    }
                }
            }

        }
	}
	#endregion

	#region ServerGroup
	/// <summary>
	/// ��������
	/// </summary>
	/// <author>������</author>
	public class ServerGroup : ISecurity
	{
		public enum Type
		{
			Server=0,//����������
			Group=1//����������
		}

        /// <summary>
        /// ��״̬,���ȼ��ӵ͵���
        /// </summary>
        public enum GroupState
        {
            Nothing = 0,
            Downloading,
            Updating,
            Stopping,
            Starting
        }

        /// <summary>
        /// ���Ƿ񿪷�
        /// </summary>
        public enum GroupEnable
        {
            Close=0,  //δ����
            Open=1,   //����
            Merge=2   //����ϲ�����һ����,�����鱻ɾ��
        }

        public static GroupEnable ParseGroupEnable(string state)
        {
            if (state.Equals("Close", StringComparison.OrdinalIgnoreCase))
                return GroupEnable.Close;
            else if (state.Equals("Open", StringComparison.OrdinalIgnoreCase))
                return GroupEnable.Open;
            else if (state.Equals("Merge", StringComparison.OrdinalIgnoreCase))
                return GroupEnable.Merge;
            else throw new Exception("Parse Group State error.");
        }

        public static GroupState ParseGroupState(string state)
        {
            if (state.Equals("Nothing", StringComparison.OrdinalIgnoreCase))
                return GroupState.Nothing;
            else if (state.Equals("Downloading", StringComparison.OrdinalIgnoreCase))
                return GroupState.Downloading;
            else if (state.Equals("Updating", StringComparison.OrdinalIgnoreCase))
                return GroupState.Updating;
            else if (state.Equals("Stopping", StringComparison.OrdinalIgnoreCase))
                return GroupState.Stopping;
            else if (state.Equals("Starting", StringComparison.OrdinalIgnoreCase))
                return GroupState.Starting;
            else throw new Exception("Parse Group State error.");
        }

		FSEyeObject _securityObject;

		Type _type;

        GroupState _groupState;

        GroupEnable _groupEnable;

        int _maxPlayers=0;

        int _nowPlayers = 0;

        int _offlinePlayers = 0;

        string _groupVersion = "";

        string _cabinet;

        DateTime _maxPlayerLogTime;

		IList _list;

		public FSEyeObject SecurityObject
		{
			get { return _securityObject; }
			set { _securityObject = value; }
		}

		public int Id
		{
			get { return _securityObject.Id; }
		}

        public int OfflinePlayer
        {
            get { return _offlinePlayers; }
            set { _offlinePlayers = value; }
        }

		public string Name
		{
			get
			{
				if (_securityObject != null)
					return _securityObject.Name;
				else
					return string.Empty;
			}
		}

        public string GroupVer
        {
            get {return _groupVersion;}
            set { _groupVersion = value; }
        }

        //������GMCAccount������serverlist.ini��GateWay�ֶ�
        public string GatewayName
        {
            get
            {
                foreach (GameServer server in _list)
                {
                    if (server.Type == GameServer.ServerType.gamecenter)
                        return server.LordConfig.GMCAccount;
                }

                return String.Empty;
            }
        }

        public GroupState group_state
        {
            get { return _groupState; }
            set { _groupState = value; }
        }

        public GroupEnable group_enable
        {
            get { return _groupEnable; }
            set { _groupEnable = value; }
        }

        public int MaxPlayer
        {
            get { return _maxPlayers; }
            set { _maxPlayers = value; }
        }

        public int NowPlayers
        {
            get { return _nowPlayers; }
            set { _nowPlayers = value; }
        }

        public string Cabinet
        {
            get { return _cabinet; }
            set { _cabinet = value; }
        }

        public DateTime MaxPlayerLogTime
        {
            get { return _maxPlayerLogTime; }
            set { _maxPlayerLogTime = value; }
        }
        
		public string Comment
		{
			get
			{
				if (_securityObject != null)
					return _securityObject.Comment;
				else
					return string.Empty;
			}
		}

		public Type GroupType
		{
			get { return _type; }
			set { _type = value; }
		}

		public IList List
		{
			get { return _list; }
			set { _list = value; }
		}

		public ServerGroup()
		{
			_list = ArrayList.Synchronized(new ArrayList());
		}

        public bool SetGroupEnable(GroupEnable groupEnable)
        {
            try
            {
                IBlazeDatabase db = DbFactory.GetDatabase();
                IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
                serverGroupTable.Set(TableString.ServerGroupFieldGroupEnable, (int)groupEnable, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, this.Id));
                this.group_enable = groupEnable;
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }


        public bool SetGroupMaxPlays(int maxPlayers)
        {
            try
            {
                string[] fieldName = new string[2] { TableString.ServerGroupFieldMaxPlayers, TableString.ServerGroupFieldMaxPlayersLogTime };
                object[] dbValue = new object[2] { maxPlayers, DateTime.Now };
                IBlazeDatabase db = DbFactory.GetDatabase();
                IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
                serverGroupTable.Set(fieldName, dbValue, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, this.Id));
                this._maxPlayers = maxPlayers;
                
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// ���÷�����������Ϣ
        /// </summary>
        /// <param name="label"></param>
        /// <param name="cabinet"></param>
        public void SetGroupCabinet(string cabinet)
        {
            try
            {
                string[] fieldName = new string[1] { TableString.ServerGroupFieldCabinet };
                object[] dbValue = new object[1] { cabinet };
                IBlazeDatabase db = DbFactory.GetDatabase();
                IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
                serverGroupTable.Set(fieldName, dbValue, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, this.Id));                
                this.Cabinet = cabinet;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        /// <summary>
        /// ������汾��
        /// </summary>
        /// <param name="ver"></param>
        public void SetGroupVersion(string ver)
        {
            try
            {
                string[] fieldName = new string[1] { TableString.ServerGroupFieldGroupVersion };
                object[] dbValue = new object[1] { ver };
                IBlazeDatabase db = DbFactory.GetDatabase();
                IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
                serverGroupTable.Set(fieldName, dbValue, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, this.Id));
                this.GroupVer = ver;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

		/// <summary>
		/// �����Ϸ������
		/// </summary>
		public void AddGameServer(GameServer server)
		{
			IBlazeDatabase db = DbFactory.GetDatabase();
			AddGameServer(server, db);
		}

		/// <summary>
		/// �����Ϸ������
		/// </summary>
		public void AddGameServer(GameServer server, IBlazeDatabase db)
		{
			if (server == null)
				throw new ArgumentNullException("server");
			if (db == null)
				throw new ArgumentNullException("db");
			if (_type != Type.Server && _list.Count > 0)
				throw new Exception("GameServerGroup width Type=" + _type + " does not support this operation.");

			try
			{
				db.BeginTrans();

				this.SecurityObject.AddChild(server.SecurityObject);
				AdminServer.TheInstance.SecurityManager.Set(this.SecurityObject, db);
				AdminServer.TheInstance.SecurityManager.Set(server.SecurityObject, db);

				db.CommitTrans();

				if (_type != Type.Server)
				{
					_type = Type.Group;
				}
				IBlazeTable serverGroupTable = db.GetTable(TableString.ServerGroupTableName);
				serverGroupTable.Set(TableString.ServerGroupFieldType, (int)_type, FilterFactory.CreateEqualFilter(TableString.ServerGroupFieldId, this.Id));

				_list.Add(server);
				server.Group = this;
			}
			catch (Exception ex)
			{
				db.RollbackTrans();
				throw ex;
			}
		}

		/// <summary>
		/// ɾ����Ϸ������
		/// </summary>
		public void DeleteGameServer(GameServer server)
		{
			IBlazeDatabase db = DbFactory.GetDatabase();
			DeleteGameServer(server, db);
		}

		/// <summary>
		/// ɾ����Ϸ������
		/// </summary>
		public void DeleteGameServer(GameServer server, IBlazeDatabase db)
		{
			if (server == null)
				throw new ArgumentNullException("server");
			if (db == null)
				throw new ArgumentNullException("db");

			try
			{
				db.BeginTrans();

				this.SecurityObject.RemoveChild(server.SecurityObject.Name);
				AdminServer.TheInstance.SecurityManager.Set(this.SecurityObject, db);
				AdminServer.TheInstance.SecurityManager.Set(server.SecurityObject, db);

				db.CommitTrans();

				this.List.Remove(server);
				server.Group = null;
			}
			catch (Exception ex)
			{
				db.RollbackTrans();
				throw ex;
			}
		}
	}
	#endregion

	#region GameServerAuthentication
	/// <summary>
	/// ��Ϸ��������֤
	/// </summary>
	public class GameServerAuthentication : IDisposable
	{
		static Random s_random = new Random();

		long _connectionId;

		string _address;

		int _port;

		bool _valid;

		int _x;

		bool _success;

		byte[] _communicationKey;

		DateTime _timeout;

		/// <summary>
		/// �Ƿ�ɹ�
		/// </summary>
		public bool Success
		{
			get { return _success; }
		}

		/// <summary>
		/// ����ID
		/// </summary>
		public long ConnectionId
		{
			get { return _connectionId; }
		}

		/// <summary>
		/// ͨѶ��Կ
		/// </summary>
		public byte[] CommunicationKey
		{
			get { return _communicationKey; }
		}

		/// <summary>
		/// �Ƿ�Ϸ�
		/// </summary>
		public bool Valid
		{
			get { return _valid; }
		}

		public GameServerAuthentication(long connectionId, string address, int port, long timeout)
		{
			_connectionId = connectionId;
			_address = address;
			_port = port;
			_valid = true;
			_success = false;
			_timeout = DateTime.Now.AddMilliseconds(timeout);
		}

		/// <summary>
		/// ��ʼ
		/// </summary>
		public void Begin()
		{
			CreateKey();
			DeliverKey();
			Challenge();
		}

		/// <summary>
		/// ������Կ
		/// </summary>
		void CreateKey()
		{
			_communicationKey = UtilDllWrap.CreateRandomKey(8);
		}

		/// <summary>
		/// ������Կ
		/// </summary>
		void DeliverKey()
		{
            byte[] encryptedKey = _communicationKey;//romandou UtilDllWrap.RSAEncrypt(_communicationKey);
			
			e2g_DeliverKey protocol = new e2g_DeliverKey();
			if (encryptedKey == null || encryptedKey.Length != protocol.Key.Data.Length)
				throw new Exception("Protocol is not fit for encrypted key.");
			Array.Copy(encryptedKey, protocol.Key.Data, encryptedKey.Length);
			AdminServer.TheInstance.ConnectionManager.SendTo(_connectionId, protocol.ToBytes());
		}

		/// <summary>
		/// ������ս��
		/// </summary>
		void Challenge()
		{
			_x = s_random.Next(1000);
			e2g_Authentication protocol = new e2g_Authentication();
			protocol.X = _x;
            byte[] sendBytes = protocol.ToBytes();//romandou UtilDllWrap.RC4Encrypt(protocol.ToBytes(), _communicationKey);
			AdminServer.TheInstance.ConnectionManager.SendTo(_connectionId, sendBytes);
		}

		public void Tick()
		{
			if (DateTime.Now > _timeout)
				_valid = false;
		}

		public void Authentication(IProtocol msg)
		{
			_success = false;
			if ((ProtocolDef)msg.ProtocolId == ProtocolDef.g2e_Authentication_def)
			{
				g2e_Authentication protocol = msg as g2e_Authentication;
				int y = Util.CaculateY(_x);
				string md5y = UtilDllWrap.MD5(y.ToString(), SystemConfig.Current.DefaultEncoding);

				if (md5y == protocol.Y)
				{
					IList gameServerList = AdminServer.TheInstance.GameServerManager.Servers;
					lock (gameServerList.SyncRoot)
					{
						foreach (GameServer server in gameServerList)
						{
							if (server.IpAddress == _address && !server.IsConnected)
							{
								Confirm(server);
								break;
							}
						}
					}
				}
			}

			Dispose();
		}

		public void Confirm(GameServer server)
		{
			_success = true;
			server.OnConnected(_connectionId, _communicationKey, _port);

			Dispose();
		}

		public void Dispose()
		{
			if (!_success)
			{
				AdminServer.TheInstance.ConnectionManager.Disconnect(_connectionId);
			}

			_valid = false;
		}
	}
	#endregion

    #region ServerMap
    public class ServerMap
    {
        public const int RootNodeId = 1;
        private List<MapNode> _netList = new List<MapNode>();

        /// <summary>
        /// ����
        /// </summary>
        public enum ServerMapType
        {
            Net = 0,
            Region = 1,
            Server
        }

        /// <summary>
        /// ���λ
        /// </summary>
        public enum ServerMapFlag
        {
            Hide = 1,           //����
            New = 2,            //�¿�
            Recommend = 4       //�Ƽ�
        }

        public List<MapNode> NetList
        {
            get { return _netList; }
        }

        /// <summary>
        /// ʹ��Ĭ�ϵ�MapNode�б�����ݿ�����������
        /// </summary>
        public bool LoadServerMap()
        {
            return LoadServerMap(ref _netList);
        }

        /// <summary>
        /// �����ݿ�����������
        /// </summary>
        /// <param name="netList">���ã�ref��</param>
        public bool LoadServerMap(ref List<MapNode> netList)
        {
            IBlazeDatabase db = null;
            try
            {
                db = DbFactory.GetDatabase();
                db.BeginTrans();

                List<MapNode> resultList = new List<MapNode>();

                IBlazeTable serverTable = db.GetTable(TableString.ServerMapTableName);
                DataSet data = new DataSet();
                serverTable.Get(data);

                DataTable mapServerTable = data.Tables[TableString.ServerMapTableName];
                string condition = string.Format("{0}={1}", TableString.ServerMapFieldMapType, (int)ServerMapType.Net);
                string order = string.Format("{0} ASC", TableString.ServerMapFieldIndex);
                DataRow[] netData = mapServerTable.Select(condition, order);
                if (netData.Length > 0)
                {
                    foreach (DataRow netRow in netData)
                    {
                        int nodeId = (int)netRow[TableString.ServerMapFieldId];
                        int parentId = (int)netRow[TableString.ServerMapFieldParentId];
                        string name = netRow[TableString.ServerMapFieldName] as string;
                        int nodeIndex = (int)netRow[TableString.ServerMapFieldIndex];
                        ServerMapType type = (ServerMap.ServerMapType)netRow[TableString.ServerMapFieldMapType];
                        int flag = (int)netRow[TableString.ServerMapFieldFlag];
                        int mapServerId = (int)netRow[TableString.ServerMapFieldMapServerId];

                        MapNode node = new MapNode(nodeId, type, flag, name, mapServerId, nodeIndex);

                        BuildMapNode(node, data.Tables[TableString.ServerMapTableName]);

                        resultList.Insert(nodeIndex, node);
                    }
                }

                netList = resultList;
                return true;
            }
            catch (Exception ex)
            {
                FSEye.Util.DebugLog(ex.ToString());

                return false;
            }
            finally
            {
                if (db != null)
                {
                    db.Close();
                }
            }
        }

        /// <summary>
        /// �������
        /// </summary>
        /// <param name="rootNode">�������Ľ��</param>
        /// <param name="dataTable">���ݱ�</param>
        public void BuildMapNode(MapNode rootNode, DataTable dataTable)
        {
            try
            {
                if (rootNode != null)
                {
                    DataRow[] childData = dataTable.Select(TableString.ServerMapFieldParentId + "=" + rootNode.ID, TableString.ServerMapFieldIndex + " ASC");
                    foreach (DataRow childRow in childData)
                    {
                        int nodeId = (int)childRow[TableString.ServerMapFieldId];
                        int parentId = (int)childRow[TableString.ServerMapFieldParentId];
                        string name = childRow[TableString.ServerMapFieldName] as string;
                        int nodeIndex = (int)childRow[TableString.ServerMapFieldIndex];
                        ServerMapType type = (ServerMap.ServerMapType)childRow[TableString.ServerMapFieldMapType];
                        int flag = (int)childRow[TableString.ServerMapFieldFlag];
                        int mapServerId = (int)childRow[TableString.ServerMapFieldMapServerId];

                        MapNode node = new MapNode(nodeId, type, flag, name, mapServerId);
                        BuildMapNode(node, dataTable);
                        rootNode.InsertChild(nodeIndex, node);
                    }
                }
            }
            catch (Exception ex)
            {
                FSEye.Util.DebugLog(ex.ToString());
            }
        }

        /// <summary>
        /// ������������
        /// </summary>
        /// <param name="netList">����</param>
        public bool SaveServerMapList(List<MapNode> netList)
        {
            IBlazeDatabase db = null;
            try
            {
                db = DbFactory.GetDatabase();
                db.BeginTrans();

                IBlazeTable serverTable = db.GetTable(TableString.ServerMapTableName);

                DataSet dataSet = new DataSet();
                serverTable.Get(dataSet);
                DataTable serverMapTable = dataSet.Tables[TableString.ServerMapTableName];

                //ɾ��������    //Ϊʲô��serverMapTable.Rows.Clear()��ɾ������������
                foreach (DataRow row in serverMapTable.Rows)
                {
                    row.Delete();
                }
                for (int netIndex = 0; netIndex < netList.Count; ++netIndex)
                {
                    MapNode netNode = netList[netIndex];

                    DataRow netRow = serverMapTable.NewRow();

                    netRow[TableString.ServerMapFieldId] = netNode.ID;
                    netRow[TableString.ServerMapFieldParentId] = 0;
                    netRow[TableString.ServerMapFieldName] = netNode.Name;
                    netRow[TableString.ServerMapFieldMapType] = (int)netNode.MapType;
                    netRow[TableString.ServerMapFieldIndex] = netIndex;
                    netRow[TableString.ServerMapFieldFlag] = netNode.Flag;
                    netRow[TableString.ServerMapFieldMapServerId] = netNode.MapServerId;

                    serverMapTable.Rows.Add(netRow);

                    for (int regionIndex = 0; regionIndex < netNode.Children.Length; ++regionIndex)
                    {
                        MapNode regionNode = netNode.Children[regionIndex];

                        DataRow regionRow = serverMapTable.NewRow();
                        regionRow[TableString.ServerMapFieldId] = regionNode.ID;
                        regionRow[TableString.ServerMapFieldParentId] = regionNode.Parent.ID;
                        regionRow[TableString.ServerMapFieldName] = regionNode.Name;
                        regionRow[TableString.ServerMapFieldMapType] = (int)regionNode.MapType;
                        regionRow[TableString.ServerMapFieldIndex] = regionIndex;
                        regionRow[TableString.ServerMapFieldFlag] = regionNode.Flag;
                        regionRow[TableString.ServerMapFieldMapServerId] = regionNode.MapServerId;

                        serverMapTable.Rows.Add(regionRow);

                        for (int serverIndex = 0; serverIndex < regionNode.Children.Length; ++serverIndex)
                        {
                            MapNode serverNode = regionNode.Children[serverIndex];

                            DataRow serverRow = serverMapTable.NewRow();
                            serverRow[TableString.ServerMapFieldId] = serverNode.ID;
                            serverRow[TableString.ServerMapFieldParentId] = serverNode.Parent.ID;
                            serverRow[TableString.ServerMapFieldName] = serverNode.Name;
                            serverRow[TableString.ServerMapFieldMapType] = (int)serverNode.MapType;
                            serverRow[TableString.ServerMapFieldIndex] = serverIndex;
                            serverRow[TableString.ServerMapFieldFlag] = serverNode.Flag;
                            serverRow[TableString.ServerMapFieldMapServerId] = serverNode.MapServerId;

                            serverMapTable.Rows.Add(serverRow);
                        }
                    }
                }

                serverTable.Set(serverMapTable);
                db.CommitTrans();

                //����һ��Ϊ֮ǰ�Ĵ��룬�������������ǣ�
                //ҳ���ڵ�netList�������_netList��ָ����ͬ�Ķ���
                //������ͬһ��ҳ��ķ����ڣ�ֻҪ����������������һ���޸ģ�
                //֮��ͬһ�ε�ҳ���ڵ��޸Ľ�ֱ�������������_netList��
                //�⽫ʹ��"�쿴ϵͳ����"��"�쿴��ǰ����"�Ľ��û������
                //_netList = netList;

                //����Ĵ�����Ϊ�˽��֮ǰΪǳ���������⣬���´����ݿ������µ�_netList
                //��������ͬһ�δ򿪵�ServerListConfig.aspx�ڣ�δ������޸Ĳ���Ӱ�������_netList
                //��Ϊ����ȫ��ͬ������List<MapNode>��
                List<MapNode> resultList = new List<MapNode>();
                IBlazeTable serverTable1 = db.GetTable(TableString.ServerMapTableName);
                DataSet data = new DataSet();
                serverTable1.Get(data);
                DataTable mapServerTable = data.Tables[TableString.ServerMapTableName];
                string condition = string.Format("{0}={1}", TableString.ServerMapFieldMapType, (int)ServerMapType.Net);
                string order = string.Format("{0} ASC", TableString.ServerMapFieldIndex);
                DataRow[] netData = mapServerTable.Select(condition, order);
                if (netData.Length > 0)
                {
                    foreach (DataRow netRow in netData)
                    {
                        int nodeId = (int)netRow[TableString.ServerMapFieldId];
                        int parentId = (int)netRow[TableString.ServerMapFieldParentId];
                        string name = netRow[TableString.ServerMapFieldName] as string;
                        int nodeIndex = (int)netRow[TableString.ServerMapFieldIndex];
                        ServerMapType type = (ServerMap.ServerMapType)netRow[TableString.ServerMapFieldMapType];
                        int flag = (int)netRow[TableString.ServerMapFieldFlag];
                        int mapServerId = (int)netRow[TableString.ServerMapFieldMapServerId];

                        MapNode node = new MapNode(nodeId, type, flag, name, mapServerId, nodeIndex);

                        BuildMapNode(node, data.Tables[TableString.ServerMapTableName]);

                        resultList.Insert(nodeIndex, node);
                    }
                }
                _netList = resultList;


                return true;
            }
            catch (Exception ex)
            {
                if (db != null)
                {
                    db.RollbackTrans();
                }

                FSEye.Util.DebugLog(ex.ToString());
                return false;
            }
            finally
            {
                if (db != null)
                {
                    db.Close();
                }
            }
        }

        /// <summary>
        /// ���б����ҳ���Id��Ӧ��MapNode
        /// </summary>
        /// <param name="mapNodeId">MapNodeId</param>
        /// <param name="netList">NetList�б�</param>
        public MapNode GetMapNode(int mapNodeId, List<MapNode> netList)
        {
            try
            {
                if (netList != null)
                {
                    foreach (MapNode net in netList)
                    {
                        if (net.ID == mapNodeId)
                        {
                            return net;
                        }
                        foreach (MapNode region in net.Children)
                        {
                            if (region.ID == mapNodeId)
                            {
                                return region;
                            }
                            foreach (MapNode mapServer in region.Children)
                            {
                                if (mapServer.ID == mapNodeId)
                                {
                                    return mapServer;
                                }
                            }
                        }
                    }
                }
                return null;
            }
            catch (Exception ex)
            {
                FSEye.Util.DebugLog(ex.ToString());
                return null;
            }
        }

        /// <summary>
        /// ���б����ҳ���һ����serverIdƥ���MapNode
        /// </summary>
        /// <param name="serverId">ʵ�ʵķ�����Id</param>
        public MapNode GetServerMapNode(int serverId, List<MapNode> netList)
        {
            try
            {
                if (netList != null)
                {
                    foreach (MapNode net in netList)
                    {
                        foreach (MapNode region in net.Children)
                        {
                            foreach (MapNode mapServer in region.Children)
                            {
                                if (mapServer.MapServerId == serverId)
                                {
                                    return mapServer;
                                }
                            }
                        }
                    }
                }
                return null;
            }
            catch (Exception ex)
            {
                FSEye.Util.DebugLog(ex.ToString());
                return null;
            }
        }

        /// <summary>
        /// ���б���������serverIdƥ���MapNode
        /// </summary>
        /// <param name="serverId">ʵ�ʵķ�����Id</param>
        public MapNode[] GetServerMapNodes(int serverId, List<MapNode> netList)
        {
            if (netList != null && netList.Count > 0)
            {
                List<MapNode> resultList = new List<MapNode>();
                foreach (MapNode net in netList)
                {
                    foreach (MapNode region in net.Children)
                    {
                        foreach (MapNode mapServer in region.Children)
                        {
                            if (mapServer.MapServerId == serverId)
                            {
                                resultList.Add(mapServer);
                            }
                        }
                    }
                }
                return resultList.ToArray();
            }
            return null;
        }

        /// <summary>
        /// ����MapNode��ȡ���Ӧ��GameServers�������ظ��ģ�
        /// </summary>
        /// <param name="selNodes">MapNodes</param>
        public GameServer[] GetGameServerFromMapNodes(MapNode[] selNodes)
        {
            try
            {
                if (selNodes != null)
                {
                    List<GameServer> serverList = new List<GameServer>();
                    for (int index = 0; index < selNodes.Length; ++index)
                    {
                        MapNode node = selNodes[index];
                        if (node != null)
                        {
                            switch (node.MapType)
                            {
                                case ServerMapType.Net:
                                    {
                                        foreach (MapNode regionNode in node.Children)
                                        {
                                            foreach (MapNode serverNode in regionNode.Children)
                                            {
                                                GameServer server = AdminServer.TheInstance.GameServerManager.GetGameServer(serverNode.MapServerId);
                                                if (server != null && !serverList.Contains(server))
                                                {
                                                    serverList.Add(server);
                                                }
                                            }
                                        }
                                    }
                                    break;
                                case ServerMapType.Region:
                                    {
                                        foreach (MapNode serverNode in node.Children)
                                        {
                                            GameServer server = AdminServer.TheInstance.GameServerManager.GetGameServer(serverNode.MapServerId);
                                            if (server != null && !serverList.Contains(server))
                                            {
                                                serverList.Add(server);
                                            }
                                        }
                                    }
                                    break;
                                case ServerMapType.Server:
                                    {
                                        GameServer server = AdminServer.TheInstance.GameServerManager.GetGameServer(node.MapServerId);
                                        if (server != null && !serverList.Contains(server))
                                        {
                                            serverList.Add(server);
                                        }
                                    }
                                    break;
                            }
                        }
                    }
                    return serverList.ToArray();
                }
                else
                {
                    return null;
                }
            }
            catch (Exception ex)
            {
                FSEye.Util.DebugLog(ex.ToString());
                return null;
            }
        }
    }

    [Serializable]
    public class MapNode
    {
        private int _id;

        private List<MapNode> _childrenList = new List<MapNode>();

        private int _flag = 0;

        private string _name = string.Empty;

        private ServerMap.ServerMapType _type;

        private int _mapServerId;

        private MapNode _parent = null;

        private int _index;

        public int ID
        {
            get { return _id; }
        }

        public MapNode[] Children
        {
            get
            {
                return _childrenList.ToArray();
            }
        }

        public int Flag
        {
            get { return _flag; }
            set { _flag = value; }
        }

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        public ServerMap.ServerMapType MapType
        {
            get { return _type; }
            set { _type = value; }
        }

        public int MapServerId
        {
            get { return _mapServerId; }
            set { _mapServerId = value; }
        }

        public MapNode Parent
        {
            get { return _parent; }
            set { _parent = value; }
        }

        public int Index
        {
            get { return _index; }
            set { _index = value; }
        }

        public bool Hide
        {
            get
            {
                return (_flag & (int)ServerMap.ServerMapFlag.Hide) > 0;
            }
        }

        public bool New
        {
            get
            {
                return (_flag & (int)ServerMap.ServerMapFlag.New) > 0;
            }
        }

        public bool Recommend
        {
            get
            {
                return (_flag & (int)ServerMap.ServerMapFlag.Recommend) > 0;
            }
        }

        public MapNode FirstChild
        {
            get
            {
                if (_childrenList.Count > 0)
                {
                    return _childrenList[0];
                }
                return null;
            }
        }

        public MapNode LastChild
        {
            get
            {
                if (_childrenList.Count > 0)
                {
                    return _childrenList[_childrenList.Count - 1];
                }
                return null;
            }
        }

        public MapNode(int id, ServerMap.ServerMapType type, int flag, string name, int mapServerId, int index, MapNode parent)
        {
            _id = id;
            _type = type;
            _flag = flag;
            _name = name;
            _mapServerId = mapServerId;
            _index = index;
            _parent = parent;
        }

        public MapNode(int id, ServerMap.ServerMapType type, int flag, string name)
            : this(id, type, flag, name, -1, 0, null)
        {
        }

        public MapNode(int id, ServerMap.ServerMapType type, int flag, string name, int mapServerId)
            : this(id, type, flag, name, mapServerId, 0, null)
        {
        }

        public MapNode(int id, ServerMap.ServerMapType type, int flag, string name, int mapServerId, int index)
            : this(id, type, flag, name, mapServerId, index, null)
        {
        }

        /// <summary>
        /// �ǵò�׽�쳣
        /// </summary>
        /// <param name="index"></param>
        /// <param name="node"></param>
        public void InsertChild(int index, MapNode node)
        {
            lock (_childrenList)
            {
                _childrenList.Insert(index, node);
                node.Parent = this;
            }
        }

        public void AddChild(MapNode node)
        {
            lock (_childrenList)
            {
                _childrenList.Add(node);
                node.Index = _childrenList.Count - 1;
                node.Parent = this;
            }
        }

        public void RemoveChild(MapNode node)
        {
            lock (_childrenList)
            {
                _childrenList.Remove(node);
                node.Parent = null;
            }
        }

        public void MoveChildForward(MapNode node)
        {
            lock (_childrenList)
            {
                int index = _childrenList.IndexOf(node);
                if (index != -1 && index != 0)
                {
                    _childrenList.Reverse(index - 1, 2);
                }
            }
        }

        public void MoveChildBackward(MapNode node)
        {
            lock (_childrenList)
            {
                int index = _childrenList.IndexOf(node);
                if (index != -1 && index < _childrenList.Count - 1)
                {
                    _childrenList.Reverse(index, 2);
                }
            }
        }

        public void MoveChildToTop(MapNode node)
        {
            lock (_childrenList)
            {
                int index = _childrenList.IndexOf(node);
                if (index != -1 && index != 0)
                {
                    while (index != 0)
                    {
                        _childrenList.Reverse(index - 1, 2);
                        index--;
                    }
                }
            }
        }

        public void MoveChildToBottom(MapNode node)
        {
            lock (_childrenList)
            {
                int index = _childrenList.IndexOf(node);
                if (index != -1 && index < _childrenList.Count - 1)
                {
                    while (index != _childrenList.Count - 1)
                    {
                        _childrenList.Reverse(index, 2);
                        index++;
                    }
                }
            }
        }
    }
    #endregion


    #region ServerRegion
    /// <summary>
    /// ��������
    /// </summary>
    /// <author>������</author>
    public class ServerRegion
    {
        public enum Type
        {
            Root = 0,//ȫ��
            DirectUnder = 1,// ����ֱ������Ϊserver����
            Cabinet //�����߼���
        }
     
        int _maxPlayers = 0;

        Type _type;

        string _name = string.Empty;

        int _id;

        int _nowPlayers=0;

        int _offlinePlayers=0;
     
        DateTime _maxPlayerLogTime=DateTime.Now;

        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }

        public int NowPlayers
        {
            get { return _nowPlayers; }
            set { _nowPlayers = value; }
        }

        public int OfflinePlayer
        {
            get { return _offlinePlayers; }
            set { _offlinePlayers = value; }
        }

        public int MaxPlayer
        {
            get { return _maxPlayers; }
            set { _maxPlayers = value; }
        }

        public DateTime MaxPlayerLogTime
        {
            get { return _maxPlayerLogTime; }
            set { _maxPlayerLogTime = value; }
        }

        public Type RegionType
        {
            get { return _type; }
            set { _type = value; }
        }

        public string RegionName
        {
            get { return _name; }
            set { _name = value; }
        }

        public ServerRegion()
        {
        }       


        public bool SetRegionMaxPlays(int maxPlayers)
        {
            try
            {
                string[] fieldName = new string[2] { TableString.ServerRegionFieldMaxPlayers, TableString.ServerRegionFieldMaxPlayersLogTime };
                object[] dbValue = new object[2] { maxPlayers, DateTime.Now };
                IBlazeDatabase db = DbFactory.GetDatabase();
                IBlazeTable serverRegionTable = db.GetTable(TableString.ServerRegionTableName);
                serverRegionTable.Set(fieldName, dbValue, FilterFactory.CreateEqualFilter(TableString.ServerRegionFieldId, this.Id));
                this._maxPlayers = maxPlayers;

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        public bool SetRegionName(string regionName)
        {
            try
            {
                IBlazeDatabase db = DbFactory.GetDatabase();
                IBlazeTable serverRegionTable = db.GetTable(TableString.ServerRegionTableName);
                serverRegionTable.Set(TableString.ServerRegionFieldRegionName, regionName, FilterFactory.CreateEqualFilter(TableString.ServerRegionFieldId, this.Id));

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
    }
    #endregion

 









}