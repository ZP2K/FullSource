using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;
using FSEye.Cryptography;

namespace FSEye
{
	public class SystemConfig
	{
		const string XmlElementSystemConfig = "SystemConfig";
		
		const string XmlElementConfig = "Config";

		const string XmlAttribute = "attribute";

		const string XmlValue = "value";

		ushort _maxConnectionCount;

        string _adminServerListenIP;

		int _adminServerListenPort;

		int _secondaryAdminServerListenPort;

		string _adminServerUploadFileRootPath;

        string _serverListFileDir;

        string _serverListFileName;

        string _pakFileName;

        string _serverListCRCFileName;

        int _serverListServerGroupMaxPlayer;

		string _dbConnectionString;

        string dbDefaultIP;

        string defaultGameDbName;

		string _defaultDbName;

		Encoding _defaultEncoding = Encoding.Default;

		string _defaultNewLine = Environment.NewLine;

		string _logDir = string.Empty;

		bool _logEnabled = true;

		int _listenForConfigGuardReturnPort;

		int _configGuardPort;

		bool _needGuardAuthorization = true;

		string _gameSettingDir;

		long _guardAuthorizationTimeout = 5000;

		string _paySysDbConnectionString;

		bool _enablePaysys = false;

		bool _preloadGameData = false;

        string _smsUrl = "";

        string _smsCommand = "";

        string websiteVersion = "0-0-0-0";
        string _groupPlayerCountDir;
        string _groupPlayerCountFileName;
        int _groupPlayerCountKickInterval;

		static SystemConfig g_currentConfig = new SystemConfig();

		public static SystemConfig Current
		{
			get	{ return g_currentConfig; }
			set { g_currentConfig = value; }
		}

		public static SystemConfig LoadFromXml(Stream stream, bool isEncrypted)
		{
			if (stream == null)
			{
				throw new ArgumentNullException("stream");
			}

			using (XmlTextReader reader = new XmlTextReader(stream))
			{
				if (reader != null)
				{
					SystemConfig config = new SystemConfig();
					while (reader.Read())
					{
						if (reader.NodeType == XmlNodeType.Element && reader.Name == XmlElementConfig)
						{
							reader.MoveToAttribute(XmlAttribute);
							string attribute = reader.ReadContentAsString();
							switch (attribute)
							{
								case "MaxConnectionCount":
									reader.MoveToAttribute(XmlValue);
									config._maxConnectionCount = (ushort)reader.ReadContentAsInt();
									break;
                                case "AdminServerListenIP":
                                    reader.MoveToAttribute(XmlValue);
                                    config._adminServerListenIP = reader.ReadContentAsString();
                                    break;
								case "AdminServerListenPort":
									reader.MoveToAttribute(XmlValue);
									config._adminServerListenPort = reader.ReadContentAsInt();
									break;                                
								case "SecondaryAdminServerListenPort":
									reader.MoveToAttribute(XmlValue);
									config._secondaryAdminServerListenPort = reader.ReadContentAsInt();
									break;
								case "AdminServerUploadFileRootPath":
									reader.MoveToAttribute(XmlValue);
									config._adminServerUploadFileRootPath = reader.ReadContentAsString();

                                    if (!config._adminServerUploadFileRootPath.EndsWith(@"\")) config._adminServerUploadFileRootPath += @"\";

                                    //�����ļ��в����ڣ��򴴽���
                                    if (Directory.Exists(config._adminServerUploadFileRootPath) == false)
                                    {
                                        Directory.CreateDirectory(config._adminServerUploadFileRootPath);
                                    }

									break;
								case "DbConnectionString":
									{
										reader.MoveToAttribute(XmlValue);
										string readConnectionString = reader.ReadContentAsString();
										if (isEncrypted)
										{
											try
											{
												config._dbConnectionString = CryptographyManager.TheInstance.PredefinedDecrypt(readConnectionString, Encoding.Default);
											}
											catch (Exception ex)
											{
												throw new Exception("Decrypt SystemConfig Entry \"DbConnectionString\" Failed.", ex);
											}
										}
										else
										{
											config._dbConnectionString = readConnectionString;
										}
									}
									break;
								case "DefaultDbName":
									reader.MoveToAttribute(XmlValue);
									config._defaultDbName = reader.ReadContentAsString();
									break;
								case "DefaultEncoding":
									reader.MoveToAttribute(XmlValue);
									config._defaultEncoding = Encoding.GetEncoding(reader.ReadContentAsString());
									break;
								case "DefaultNewLine":
									{
										reader.MoveToAttribute(XmlValue);
										string newLine = reader.ReadContentAsString();
										if (newLine == @"\n")
										{
											config._defaultNewLine = "\n";
										}
										else if (newLine == @"\r\n")
										{
											config._defaultNewLine = "\r\n";
										}
									}
									break;
								case "LogDir":
									reader.MoveToAttribute(XmlValue);
									config._logDir = reader.ReadContentAsString();

                                    if (!config._logDir.EndsWith(@"\")) config._logDir+=@"\";

                                    //�����ļ��в����ڣ��򴴽���
                                    if (Directory.Exists(config._logDir) == false)
                                    {
                                        Directory.CreateDirectory(config._logDir);
                                    }

									break;
								case "LogEnabled":
									reader.MoveToAttribute(XmlValue);
									config._logEnabled = reader.ReadContentAsBoolean();
									break;
								case "ListenForConfigGuardReturnPort":
									reader.MoveToAttribute(XmlValue);
									config._listenForConfigGuardReturnPort = reader.ReadContentAsInt();
									break;
								case "ConfigGuardPort":
									reader.MoveToAttribute(XmlValue);
									config._configGuardPort = reader.ReadContentAsInt();
									break;
								case "NeedGuardAuthorization":
									reader.MoveToAttribute(XmlValue);
									config._needGuardAuthorization = reader.ReadContentAsBoolean();
									break;
								case "GameSettingDir":
									reader.MoveToAttribute(XmlValue);
                                    config._gameSettingDir = reader.ReadContentAsString();

                                    if (!config._gameSettingDir.EndsWith(@"\")) config._gameSettingDir += @"\";

                                    //�����ļ��в����ڣ��򴴽���
                                    if (Directory.Exists(config._gameSettingDir) == false)
                                    {
                                        Directory.CreateDirectory(config._gameSettingDir);
                                    }
									
									break;
								case "GuardAuthorizationTimeout":
									reader.MoveToAttribute(XmlValue);
									config._guardAuthorizationTimeout = reader.ReadContentAsLong();
									break;
                                case "EnablePaysys":
                                    reader.MoveToAttribute(XmlValue);
                                    config._enablePaysys = reader.ReadContentAsBoolean();
                                    break;
                                case "DBDefaultIP":
                                    reader.MoveToAttribute(XmlValue);
                                    config.dbDefaultIP = reader.ReadContentAsString();
                                    break;
                                case "DefaultGameDbName":
                                    reader.MoveToAttribute(XmlValue);
                                    config.defaultGameDbName = reader.ReadContentAsString();
                                    break;
								case "PaySysDbConnectionString":
									{
										reader.MoveToAttribute(XmlValue);
										string readConnectionString = reader.ReadContentAsString();
                                        if (config._enablePaysys)
                                        {
                                            if (isEncrypted)
                                            {
                                                try
                                                {
                                                    config._paySysDbConnectionString = CryptographyManager.TheInstance.PredefinedDecrypt(readConnectionString, Encoding.Default);
                                                }
                                                catch (Exception ex)
                                                {
                                                    throw new Exception("Decrypt SystemConfig Entry \"PaySysDbConnectionString\" Failed.", ex);
                                                }
                                            }
                                            else
                                            {
                                                config._paySysDbConnectionString = readConnectionString;
                                            }
                                        }										
									}
									break;
								case "PreloadGameData":
									reader.MoveToAttribute(XmlValue);
									config._preloadGameData = reader.ReadContentAsBoolean();
									break;
                                case "ServerListFileDir":
                                    reader.MoveToAttribute(XmlValue);
                                    config._serverListFileDir = reader.ReadContentAsString();
                                    
                                    if (!config._serverListFileDir.EndsWith(@"\")) config._serverListFileDir += @"\";

                                    //�����ļ��в����ڣ��򴴽���
                                    if (Directory.Exists(config._serverListFileDir) == false)
                                    {
                                        Directory.CreateDirectory(config._serverListFileDir);
                                    }

                                    break;
                                case "ServerListFileName":
                                    reader.MoveToAttribute(XmlValue);
                                    config._serverListFileName = reader.ReadContentAsString();
                                    break;
                                case "PakFileName":
                                    reader.MoveToAttribute(XmlValue);
                                    config._pakFileName = reader.ReadContentAsString();
                                    break;
                                case "ServerListCRCFileName":
                                    reader.MoveToAttribute(XmlValue);
                                    config._serverListCRCFileName = reader.ReadContentAsString();
                                    break;
                                case "ServerListServerGroupMaxPlayer":
                                    reader.MoveToAttribute(XmlValue);
                                    config._serverListServerGroupMaxPlayer = reader.ReadContentAsInt();
                                    break;
                                case "SMSUrl":
                                    reader.MoveToAttribute(XmlValue);
                                    config._smsUrl = reader.ReadContentAsString();
                                    break;
                                case "SMSCommand":
                                    reader.MoveToAttribute(XmlValue);
                                    config._smsCommand = reader.ReadContentAsString();
                                    break;
                                case "WebSiteVersion":
                                    reader.MoveToAttribute(XmlValue);
                                    config.websiteVersion = reader.ReadContentAsString();
                                    break;
                                case "GroupPlayerCountDir":
                                    reader.MoveToAttribute(XmlValue);
                                    config._groupPlayerCountDir = reader.ReadContentAsString();
                                    if (!config._groupPlayerCountDir.EndsWith(@"\")) config._groupPlayerCountDir += @"\";

                                    //�����ļ��в����ڣ��򴴽���
                                    if (Directory.Exists(config._groupPlayerCountDir) == false)
                                    {
                                        Directory.CreateDirectory(config._groupPlayerCountDir);
                                    }
                                    break;
                                case "GroupPlayerCountFileName":
                                    reader.MoveToAttribute(XmlValue);
                                    config._groupPlayerCountFileName = reader.ReadContentAsString();
                                    break;
                                case "GroupPlayerCountKickInterval":
                                    reader.MoveToAttribute(XmlValue);
                                    config._groupPlayerCountKickInterval = reader.ReadContentAsInt();
                                    break;
								default:
									throw new Exception("Unknown attribute '" + attribute + "' found in xml stream.");
							}
						}
					}
					return config;
				}
			}

			return null;
		}

		public SystemConfig()
		{
		}

		/// <summary>
		/// �����������
		/// </summary>
		/// <remarks>0��ʾ������</remarks>
		public ushort MaxConnectionCount
		{
			get { return _maxConnectionCount; }
		}

        public string AdminServerListenIP
        {
            get { return _adminServerListenIP; }
        }

		/// <summary>
		/// ��������������˿ڣ���ѡ��
		/// </summary>
		public int AdminServerListenPort
		{
			get { return _adminServerListenPort; }
		}

        /// <summary>
        /// ��Ӫƽ̨IP����ѡ��
        /// </summary>
        public string DBDefaultIP
        {
            get { return dbDefaultIP; }
        }

        /// <summary>
        /// ��������(WEB)
        /// </summary>
        public string SMSUrl
        {
            get { return _smsUrl; }
        }

        /// <summary>
        /// ����������֤
        /// </summary>
        public string SMSCommand
        {
            get { return _smsCommand; }
        }

		/// <summary>
		/// ��������������˿ڣ���ѡ��
		/// </summary>
		public int SecondaryAdminServerListenPort
		{
			get { return _secondaryAdminServerListenPort; }
		}

		/// <summary>
		/// ��������Guardȷ�϶˿�
		/// </summary>
		public int ListenForConfigGuardReturnPort
		{
			get { return _listenForConfigGuardReturnPort; }
		}

		/// <summary>
		/// ����Guard�˿�
		/// </summary>
		public int ConfigGuardPort
		{
			get { return _configGuardPort; }
		}

		/// <summary>
		/// ����������ϴ��ļ���·��
		/// </summary>
		public string AdminServerUploadFileRootPath
		{
			get { return _adminServerUploadFileRootPath; }
		}

		/// <summary>
		/// ���ݿ������ַ���
		/// </summary>
		public string DbConnectionString
		{
			get { return _dbConnectionString; }
		}

		/// <summary>
		/// Ĭ�����ݿ���
		/// </summary>
		public string DefaultDbName
		{
			get { return _defaultDbName; }
		}

        public string DefaultGameDbName
		{
			get { return defaultGameDbName; }
		}

       

		/// <summary>
		/// Ĭ�ϱ���
		/// </summary>
		public Encoding DefaultEncoding
		{
			get { return _defaultEncoding; }
		}

		/// <summary>
		/// Ĭ�ϻ��з���
		/// </summary>
		public string DefaultNewLine
		{
			get { return _defaultNewLine; }
		}

		/// <summary>
		/// ��־Ŀ¼
		/// </summary>
		public string LogDir
		{
			get { return _logDir; }
		}

		/// <summary>
		/// ������־
		/// </summary>
		public bool LogEnabled
		{
			get { return _logEnabled; }
		}

		/// <summary>
		/// ��ҪGuard��֤
		/// </summary>
		public bool NeedGuardAuthorization
		{
			get { return _needGuardAuthorization; }
		}

		/// <summary>
		/// ��Ϸ�趨Ŀ¼
		/// </summary>
		public string GameSettingDir
		{
			get { return _gameSettingDir; }
		}

		/// <summary>
		/// Guard��֤��ʱ
		/// </summary>
		public long GuardAuthorizationTimeout
		{
			get { return _guardAuthorizationTimeout; }
		}

		/// <summary>
		/// PaySys���ݿ������ַ���
		/// </summary>
		public string PaySysDbConnectionString
		{
			get { return _paySysDbConnectionString; }
		}

		/// <summary>
		/// ����Paysys
		/// </summary>
		public bool EnablePaysys
		{
			get { return _enablePaysys; }
		}

		/// <summary>
		/// Ԥ��������Ϸ����
		/// </summary>
		public bool PreloadGameData
		{
			get { return _preloadGameData; }
		}

        /// <summary>
        /// �������б��ļ�����Ŀ¼
        /// </summary>
        public string ServerListFileDir
        {
            get { return _serverListFileDir; }
        }

        /// <summary>
        /// �������б��ļ�����
        /// </summary>
        public string ServerListFileName
        {
            get { return _serverListFileName; }
        }

        /// <summary>
        /// pak������
        /// </summary>
        public string PakFileName
        {
            get { return _pakFileName; }
        }

        /// <summary>
        /// �������б�CRC�ļ�����(�洢�Է������б��ļ���CRCУ����)
        /// </summary>
        public string ServerListCRCFileName
        {
            get { return _serverListCRCFileName; }
        }

        /// <summary>
        /// �������б����������������
        /// </summary>
        public int ServerListServerGroupMaxPlayer
        {
            get { return _serverListServerGroupMaxPlayer; }
        }
        public string WebSiteVersion
        {
            get { return websiteVersion; }
        }
        public string GroupPlayerCountFileName
        {
            get { return _groupPlayerCountFileName; }
        }
        public string GroupPlayerCountDir
        {
            get { return _groupPlayerCountDir; }
        }
        public int GroupPlayerCountKickInterval
        {
            get { return _groupPlayerCountKickInterval; }
        }
        

	}

	/// <summary>
	/// Ԥ����İ�ȫ����
	/// </summary>
	public class PredefinedSecurityObjects
	{
		public const string GameServerList = "GameServerList";//��Ϸ�������б�
		public const string ServerInfo = "ServerInfo";//��������Ϣ
			public const string BasicInfo = "BasicInfo";//������Ϣ
			public const string Cpu = "Cpu";//CPU
			public const string Memory = "Memory";//�ڴ�
			public const string NetCard = "NetCard";//����
			public const string Disk = "Disk";//����
			public const string Process = "Process";//����
			public const string PlugIn = "PlugIn";//���
			public const string FMTask = "FMTask";//�ļ���������		
		public const string Console = "Console";//����̨

		public const string Users = "Users";//�û�
		public const string Groups = "Groups";//��
	}
}
