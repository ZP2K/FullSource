//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2006-8
//      File_base        : MessageEngine
//      File_ext         : cs
//      Author           : ������
//      Description      : ��Ϣ����
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.IO;
using FSEye.Net;
using FSEye.DllWrap;

namespace FSEye
{
	/// <summary>
	/// ��Ϣ����
	/// </summary>
	/// <author>������</author>
	class MessageEngine : IDisposable
	{
		public enum EngineState
		{
			Stopped,
			Running,
			Quit
		}
		
		Thread _processThread;

		EngineState _state;

		bool _disposed = false;

		public MessageEngine()
		{
			_state = EngineState.Stopped;
			_processThread = new Thread(new ThreadStart(MessageProcess));
			_processThread.Start();
		}

		public void Startup()
		{
			if (_disposed)
			{
				throw new ObjectDisposedException(this.ToString());
			}
			
			_state = EngineState.Running;
		}

		public void Shutdown()
		{
			if (_disposed)
			{
				throw new ObjectDisposedException(this.ToString());
			}

			_state = EngineState.Stopped;
		}

		public void Dispose()
		{
			lock (this)
			{
				if (!_disposed)
				{
					_state = EngineState.Quit;
					_processThread = null;

					GC.SuppressFinalize(this);

					_disposed = true;
				}
			}
		}

		/// <summary>
		/// ��Ϣ����
		/// </summary>
		private void MessageProcess()
		{
			while (_state != EngineState.Quit)
			{
				if (_state == EngineState.Running)
				{
					IList servers = AdminServer.TheInstance.GameServerManager.Servers;
					lock (servers.SyncRoot)
					{
						for (int i = 0; i < servers.Count; i++)
						{
                            
							GameServer server = servers[i] as GameServer;
							if (server.IsConnected)
                            {
                               
									byte[] msg = AdminServer.TheInstance.ConnectionManager.ReciveFrom(server.ConnectionId);
                                    try
                                    {
									if (msg != null && msg.Length >= 2)
									{
                                        KProtocolHead prohead = new KProtocolHead();
                                        prohead.FromBytes(msg);
                                        if (prohead.ServerID == 0)
                                        {
                                            //Dispatch(server, msg);//romandouUtilDllWrap.RC4Encrypt(msg, server.CommunicationKey));

                                            for (int j = 0; j < servers.Count; j++)
                                            {
                                                GameServer tmpserver = servers[j] as GameServer;
                                                if (tmpserver.ConnectionId == server.ConnectionId)
                                                {
                                                    Dispatch(tmpserver, msg);
                                                }
                                            }
                                        }
                                        else
                                        {
                                            for (int j = 0; j < servers.Count; j++)
                                            {
                                                GameServer tmpserver = servers[j] as GameServer;
                                                if (tmpserver.Id == prohead.ServerID)
                                                {
                                                    Dispatch(tmpserver, msg);
                                                    break;
                                                }
                                            }
                                        }
									}
								}
                                catch (Exception ex)
                                {
                                    Util.DebugLog("Normal protocol parse exception: " + ex.StackTrace);
                                    //AdminServer.TheInstance.ConnectionManager.Disconnect(server.ConnectionId);
                                }
							}
						}
					}

					IList authenticationList = AdminServer.TheInstance.GameServerManager.AuthenticationList;
					lock (authenticationList.SyncRoot)
					{
						if (authenticationList.Count > 0)
						{
							ArrayList invalidList = new ArrayList();
							foreach (GameServerAuthentication authentication in authenticationList)
							{
								authentication.Tick();
								if (authentication.Valid)
								{
									try
									{
										byte[] msg = AdminServer.TheInstance.ConnectionManager.ReciveFrom(authentication.ConnectionId);
										if (msg != null && msg.Length >= 2)
										{
											DispatchAuthentication(authentication, msg);//romandou UtilDllWrap.RC4Encrypt(msg, authentication.CommunicationKey));
										}
									}
									catch (Exception ex)
									{
										Util.DebugLog("Authentication protocol parse exception: " + ex.StackTrace);
										AdminServer.TheInstance.ConnectionManager.Disconnect(authentication.ConnectionId);
									}
								}
								else
								{
									invalidList.Add(authentication);
								}
							}

							if (invalidList.Count > 0)
							{
								foreach (GameServerAuthentication authentication in invalidList)
								{
									authentication.Dispose();
									authenticationList.Remove(authentication);
								}
							}
						}
					}
				}
				Thread.Sleep(1);
			}
		}

		/// <summary>
		/// �ַ���Ϣ
		/// </summary>
		/// <param name="msgBuff">��Ϣ</param>
		private void Dispatch(GameServer server, byte[] msgBuff)
		{
			if (server == null || msgBuff == null)
				return;

            if (msgBuff.Length <= 2)
            {
                return;
            }


			ProtocolDef protocalId = (ProtocolDef)BitConverter.ToUInt16(msgBuff, 0);
			IProtocol message = null;
			switch (protocalId)
			{
				#region �ļ�����
				case ProtocolDef.g2e_openfile_def://���ļ�
                    message = new g2e_openfile();
                    message.FromBytes(msgBuff);
					break;
				case ProtocolDef.g2e_seekfile_def://�ƶ����ļ��е�λ��
					message = new g2e_seekfile();
					break;
				case ProtocolDef.g2e_writefile_def://д���ļ�
					message = new g2e_writefile();
					break;
				case ProtocolDef.g2e_closefile_def://�ر��ļ�
					message = new g2e_closefile();
					break;
                case ProtocolDef.g2e_readfile_def: //���ļ�
                    message = new g2e_readfile();
                    break;
				#endregion

				#region ��Ϸ��������Ϣ
				case ProtocolDef.g2e_getcpubaseinfo_def://ȡ��CPU������Ϣ
					message = new g2e_getcpubaseinfo();
					break;
				case ProtocolDef.g2e_getcpuload_def://ȡ��CPU����
					message = new g2e_getcpuload();
					break;
				case ProtocolDef.g2e_getdiskinfo_def://ȡ�ô�����Ϣ
                    message = new g2e_getdiskinfo();
					break;
				case ProtocolDef.g2e_getmeminfo_def://ȡ���ڴ���Ϣ
                    message = new g2e_getmeminfo();
					break;
				case ProtocolDef.g2e_getprocinfo_def://ȡ�ý�����Ϣ
                    message = new g2e_getprocinfo();
					break;
				case ProtocolDef.g2e_getcardload_def://ȡ����������
                    message = new g2e_getcardload();					
					break;
				case ProtocolDef.g2e_getnetinfo_def://ȡ��������Ϣ
                    message = new g2e_getnetinfo();
					break;
                case ProtocolDef.g2e_getpluginfo_def://ȡ�ò����Ϣ
                    message = new g2e_getpluginfo();
                    break;				
				#endregion

				#region ����
				case ProtocolDef.g2e_ping_def://Ping
                    message = new KProtocolHead();
                    break;
				case ProtocolDef.g2e_exesql_def:
					message = new g2e_exesql();
					break;
				case ProtocolDef.g2e_exesyscmd_def:
					message = new g2e_exesyscmd();
					break;
				case ProtocolDef.g2e_GetGuardDir_def:
					message = new g2e_GetGuardDir();
					break;
				case ProtocolDef.g2e_switchmode_def:
					message = new KProtocolHead();
					break;
				case ProtocolDef.g2e_config_def:
					message = new KProtocolHead();
					break;
                case ProtocolDef.g2e_updateserver_def:
                    message = new g2e_updateserver();
                    break;
                case ProtocolDef.g2e_ftpdownload_def:
                    message = new g2e_ftpdownload();
                    break;
                case ProtocolDef.g2e_serverpid_def:
                    message = new g2e_server_pid();
                    break;                        
				#endregion

				#region ����Ϸ��ͨѶ
			
                case ProtocolDef.l2e_header_def:
                    {
                        if (msgBuff.Length <= 4)
                            return;
                        ProtocolDef subProtocolId = (ProtocolDef)BitConverter.ToUInt16(msgBuff, 4);

                        switch (subProtocolId)
                        {
                            // ahpho
                            case ProtocolDef.l2e_update_custom_info_def:
                                message = new l2e_update_custom_info();
                                break;
                            case ProtocolDef.l2e_PlayerCount_def:
                                message = new l2e_PlayerCount();
                                break;
                            case ProtocolDef.l2e_ExeGMCmd_def:
                                message = new l2e_ExeGMCmd();
                                break;
                            case ProtocolDef.l2e_GetBasicInfo_def:
                                message = new l2e_GetBasicInfo();
                                break;
                            case ProtocolDef.l2e_Who_def:
                                message = new l2e_Who();
                                break;
                            case ProtocolDef.l2e_GetGlobalVariable_def:
                                message = new l2e_GetGlobalVariable();
                                break;
                            case ProtocolDef.l2e_GetGameStartTime_def:
                                message = new l2e_GetGameStartTime();
                                break;
                            case ProtocolDef.l2e_ReportError_def:
                                message = new l2e_ReportError();
                                break;
                            case ProtocolDef.l2e_info_def:
                                message = new l2e_info();
                                break;
                            case ProtocolDef.l2e_info_large_def:
                                message = new l2e_info_large();
                                break;
                            case ProtocolDef.l2e_ping_def:
                                message = new KSubProtocolHead();
                                break;
                            default:
                                throw new Exception("�յ�δ��MessageEngine��ע���Э���");
                        }
                    }
                    break;
				#endregion

				default:
					//TODO ��¼û�г��ֹ���Э����������׶��׳��쳣
					throw new Exception("�յ�δ��MessageEngine��ע���Э���");
			}

			if (server != null && message != null)
			{
                message.FromBytes(msgBuff);
				AdminServer.TheInstance.GameServerManager.ProcessMessage(server, message);
			}
		}

		/// <summary>
		/// �ַ���֤��Ϣ
		/// </summary>
		/// <param name="authentication">��������֤</param>
		/// <param name="msgBuff">��Ϣ</param>
		private void DispatchAuthentication(GameServerAuthentication authentication, byte[] msgBuff)
		{
			if (authentication == null || msgBuff == null)
				return;

			ProtocolDef protocalId = (ProtocolDef)BitConverter.ToUInt16(msgBuff, 0);
			IProtocol message = null;
			switch (protocalId)
			{
				case ProtocolDef.g2e_Authentication_def:
					message = new g2e_Authentication();
                    message.FromBytes(msgBuff);
					break;
				//default:
				//throw new Exception("�յ���Ӧ������֤�׶η��͵�Э���");
			}

			if (message != null)
			{
				authentication.Authentication(message);
			}
		}
	}
}
