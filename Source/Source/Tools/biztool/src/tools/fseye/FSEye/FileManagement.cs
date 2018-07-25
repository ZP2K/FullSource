//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2006-8
//      File_base        : FileManagement
//      File_ext         : cs
//      Author           : ������
//      Description      : �ļ�����
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
using FSEye.Security;
using FSEye.Net;

namespace FSEye
{
	#region �ļ������������
	/// <summary>
	/// �ļ������������
	/// </summary>
	/// <author>������</author>
	public abstract class FileManageTask : IDisposable
	{
		public enum TaskState
		{
			Created,
			Queuing,
			Processing,
			Paused,
			Aborting,
			Stopped,
			Completed
		}

		protected bool _disposed = false;

		protected TaskState _taskState;

		protected DateTime _startTime;

		protected float _completePercentage;

		public FileManageTask()
		{
			_taskState = TaskState.Created;
		}

		public TaskState State
		{
			get { return _taskState; }
			set { _taskState = value; }
		}

		public DateTime StartTime
		{
			get { return _startTime; }
			set { _startTime = value; }
		}

		public float CompletePercentage
		{
			get { return _completePercentage; }
			set { _completePercentage = value; }
		}

		public abstract bool Start(GameServer gameServer);

		public abstract bool Pause(GameServer gameServer);

		public abstract bool Continue(GameServer gameServer);

		public abstract bool Stop(GameServer gameServer);

		public abstract bool DoTask(GameServer gameServer, IProtocol message);

		public abstract bool Complete(GameServer gameServer);

		public abstract void Dispose();
	}
	#endregion

    #region �ļ���������
	/// <summary>
	/// �ļ���������
	/// </summary>
	/// <author>������</author>
	public class FileTranTask : FileManageTask
	{
		string _sourceFileName;

		string _targetFileName;

		Stream _fileStream;

		uint _offset;

		uint _fileTotalLength;

		UInt16 _sendLength;

        FileTransferAutomation _fileTranAutomation;

		public FileTranTask(string sourceFile, string targetFile)
		{
			if (sourceFile == null || sourceFile.Length == 0)
			{
				throw new ArgumentException("SourceFileName can not be null or empty.");
			}
			if (targetFile == null || targetFile.Length == 0)
			{
				throw new ArgumentException("TargetFileName can not be null or empty.");
			}

			_sourceFileName = sourceFile;
			_targetFileName = targetFile;
			_offset = 0;
		}

        public FileTranTask(FileTransferAutomation fileTranAutomation, string sourceFile, string targetFile)
        {
            _fileTranAutomation = fileTranAutomation;
            //FileTranTask(sourceFile, targetFile);
            if (sourceFile == null || sourceFile.Length == 0)
            {
                throw new ArgumentException("SourceFileName can not be null or empty.");
            }
            if (targetFile == null || targetFile.Length == 0)
            {
                throw new ArgumentException("TargetFileName can not be null or empty.");
            }

            _sourceFileName = sourceFile;
            _targetFileName = targetFile;
            _offset = 0;
        }

		/// <summary>
		/// Դ�ļ���
		/// </summary>
		public string SourceFileName
		{
			get { return _sourceFileName; }
		}

		/// <summary>
		/// Ŀ���ļ���
		/// </summary>
		public string TargetFileName
		{
			get { return _targetFileName; }
		}

		public override bool Start(GameServer gameServer)
		{
			if (_taskState == TaskState.Created || _taskState == TaskState.Queuing)
			{
				e2g_openfile protocol = new e2g_openfile();
				protocol.szFileName = _targetFileName;
				protocol.bFlag = (byte)1;//0-��,1-д,2-����
				protocol.bFullPath = (byte)0;//0-���,1-����
				if (gameServer.SendMessage(protocol))
				{
					_startTime = DateTime.Now;
					_taskState = TaskState.Processing;
					return true;
				}
			}
            
            if (_fileTranAutomation != null)
                _fileTranAutomation.FinishUnit(gameServer,false);

			return false;
		}

		public override bool Pause(GameServer gameServer)
		{
			if (_taskState == TaskState.Processing)
			{
				_taskState = TaskState.Paused;
				return true;
			}

            if (_fileTranAutomation != null)
                _fileTranAutomation.FinishUnit(gameServer, false);
			return false;
		}

		public override bool Continue(GameServer gameServer)
		{
			if (_taskState == TaskState.Paused)
			{				
				e2g_openfile protocol = new e2g_openfile();
				protocol.szFileName = _targetFileName;
				protocol.bFlag = (byte)1;
				if (gameServer.SendMessage(protocol))
				{
					_taskState = TaskState.Processing;
					return true;
				}
			}
            if (_fileTranAutomation != null)
                _fileTranAutomation.FinishUnit(gameServer, false);
			return false;
		}

		public override bool Stop(GameServer gameServer)
		{
			if (_taskState == TaskState.Processing || _taskState == TaskState.Paused)
			{
				if (_fileStream != null)
				{
					_fileStream.Close();
				}

                KProtocolHead protocol = new KProtocolHead();
                protocol.ProtocolId = (ushort)ProtocolDef.e2g_closefile_def;
				gameServer.SendMessage(protocol);
				_taskState = TaskState.Aborting;

				return true;
			}
			else
			{
                if (_fileTranAutomation != null)
                    _fileTranAutomation.FinishUnit(gameServer, false);
				return false;
			}
		}

		public override bool DoTask(GameServer gameServer, IProtocol message)
		{
			if (gameServer == null || message == null)
			{
				throw new ArgumentException("gameServer & message can not be null.");
			}

			if (_taskState == TaskState.Processing)
			{
				FSEyeResult result;
				switch (message.ProtocolId)
				{
					case (UInt16)ProtocolDef.g2e_openfile_def:
						result = (FSEyeResult)((g2e_openfile)message).nRetCode;
						if (result == FSEyeResult.fseye_success)
						{
							if (_fileStream == null)
							{
								_fileStream = new FileStream(SystemConfig.Current.AdminServerUploadFileRootPath + SourceFileName, FileMode.Open, FileAccess.Read, FileShare.Read);
							}
							_fileTotalLength = (uint)_fileStream.Length;
							e2g_seekfile protocol = new e2g_seekfile();
							protocol.nOffset = _offset;

							//TODO û�д�����ʧ�ܵ����
							gameServer.SendMessage(protocol);
							return true;
						}
						else if (result == FSEyeResult.filetran_app_err)//�����ļ�������
						{
							//TODO �������¿�ʼ
							e2g_openfile protocol = new e2g_openfile();
							protocol.szFileName = _targetFileName;
							protocol.bFlag = 1;

							//TODO û�д�����ʧ�ܵ����
							gameServer.SendMessage(protocol);
							return true;
						}
						else
						{
							//FSEyeResult.filetran_opening_err//�ļ��Ѿ���
							//FSEyeResult.filetran_cre_err//�ļ�����ʧ��
							//��������δ֪�Ĵ���
							//��Щ����޷�����ֻ����ֹ����
                            if (_fileTranAutomation != null)
                                _fileTranAutomation.FinishUnit(gameServer, false);

							Stop(gameServer);
							return false;
						}
					case (UInt16)ProtocolDef.g2e_seekfile_def:
						result = (FSEyeResult)((g2e_seekfile)message).nRetCode;
						if (result == FSEyeResult.fseye_success)
						{
                            e2g_writefile protocol = new e2g_writefile((int)_fileStream.Length);
							_fileStream.Seek(_offset, SeekOrigin.Begin);
                            _sendLength = (UInt16)_fileStream.Read(protocol.szBuf, 0, (int)_fileStream.Length);
                            Array.Resize(ref protocol.szBuf, _sendLength);

							//TODO û�д�����ʧ�ܵ����
							gameServer.SendMessage(protocol);
							return true;
						}
						else
						{
							//FSEyeResult.filetran_seek_err
							//��������δ֪�Ĵ���
							//��Щ����޷�����ֻ����ֹ����

                            if (_fileTranAutomation != null)
                                _fileTranAutomation.FinishUnit(gameServer, false);

							Stop(gameServer);
							return false;
						}
					case (UInt16)ProtocolDef.g2e_writefile_def:
						result = (FSEyeResult)((g2e_writefile)message).nRetCode;
						if (result == FSEyeResult.fseye_success)
						{
							uint guardWriteLength = ((g2e_writefile)message).nWritedLen;
							_offset += guardWriteLength;

							CompletePercentage = (float)_offset / (float)_fileTotalLength;

							if (CompletePercentage < 1)
							{
								if (guardWriteLength < _sendLength)
								{
									_fileStream.Seek(guardWriteLength - _sendLength, SeekOrigin.Current);
								}
                                e2g_writefile protocol = new e2g_writefile((int)_fileStream.Length);
                                _sendLength = (UInt16)_fileStream.Read(protocol.szBuf, 0, (int)_fileStream.Length);
                                Array.Resize(ref protocol.szBuf, _sendLength);

								//TODO û�д�����ʧ�ܵ����
								gameServer.SendMessage(protocol);
							}
							else
							{
                                KProtocolHead protocol = new KProtocolHead();
                                protocol.ProtocolId = (ushort)ProtocolDef.e2g_closefile_def;

								//TODO û�д�����ʧ�ܵ����
								gameServer.SendMessage(protocol);
							}

							return true;
						}
						else
						{
							//��������δ֪�Ĵ���
							//��Щ����޷�����ֻ����ֹ����

                            if (_fileTranAutomation != null)
                                _fileTranAutomation.FinishUnit(gameServer, false);

							Stop(gameServer);
							return false;
						}
					case (UInt16)ProtocolDef.g2e_closefile_def:
						result = (FSEyeResult)((g2e_closefile)message).nRetCode;
						if (result == FSEyeResult.fseye_success)
						{
                            //֪ͨ�󶨵�Automation���ļ��������
                            if (_fileTranAutomation != null)
                                _fileTranAutomation.FinishUnit(gameServer, true);

							//TODO ��ӹرճɹ�����
							Complete(gameServer);
							return true;
						}
						else
						{
                            if (_fileTranAutomation != null)
                                _fileTranAutomation.FinishUnit(gameServer, false);
							return false;
						}						
					default:
						break;
				}
			}
			else if (_taskState == TaskState.Aborting)
			{
				if (message.ProtocolId == (UInt16)ProtocolDef.g2e_closefile_def)
				{
					_taskState = TaskState.Stopped;					
				}

				return true;
			}

			return false;
		}

		public override bool Complete(GameServer gameServer)
		{
			if (_taskState == TaskState.Processing)
			{
				_taskState = TaskState.Completed;
				if (_fileStream != null)
				{
					_fileStream.Close();
				}
				return true;
			}

			return false;
		}

		~FileTranTask()
		{
			Dispose();
		}

		public override void Dispose()
		{
			if (!_disposed)
			{
				_disposed = true;
				if (_fileStream != null)
				{
					_fileStream.Close();
				}
			}
		}

		public override string ToString()
		{
			return "TransferFile(" + _sourceFileName + ")";
		}
	}
	#endregion

    #region �ļ���������
    /// <summary>
    /// �ļ���������
    /// </summary>
    public class FileDownloadTask : FileManageTask
    {
        string _sourceFileName;

        string _targetFileName;

        Stream _fileStream;

        //������Guard���ļ���ȡ��ƫ��
        uint _offset;

        uint _fileTotalLength;

        //UInt16 _sendLength;

        bool _overWrite = false;

        FileDownloadAutomation _fileDownloadAutomation;

        public FileDownloadTask(string sourceFile, string targetFile, bool overWrite)
        {
            if (sourceFile == null || sourceFile.Length == 0)
            {
                throw new ArgumentException("SourceFileName can not be null or empty.");
            }
            if (targetFile == null || targetFile.Length == 0)
            {
                throw new ArgumentException("TargetFileName can not be null or empty.");
            }

            _sourceFileName = sourceFile;
            _targetFileName = targetFile;
            _offset = 0;
            _overWrite = overWrite;
        }

        public FileDownloadTask(FileDownloadAutomation fileDownloadAutomation, string sourceFile, string targetFile, bool overWrite)
        {
            _fileDownloadAutomation = fileDownloadAutomation;
            if (sourceFile == null || sourceFile.Length == 0)
            {
                throw new ArgumentException("SourceFileName can not be null or empty.");
            }
            if (targetFile == null || targetFile.Length == 0)
            {
                throw new ArgumentException("TargetFileName can not be null or empty.");
            }

            _sourceFileName = sourceFile;
            _targetFileName = targetFile;
            _offset = 0;
            _overWrite = overWrite;
        }

        /// <summary>
        /// Դ�ļ���
        /// </summary>
        public string SourceFileName
        {
            get { return _sourceFileName; }
        }

        /// <summary>
        /// Ŀ���ļ���
        /// </summary>
        public string TargetFileName
        {
            get { return _targetFileName; }
        }

        public override bool Start(GameServer gameServer)
        {
            if (_taskState == TaskState.Created || _taskState == TaskState.Queuing)
            {
                e2g_openfile protocol = new e2g_openfile();
                protocol.szFileName = _sourceFileName;
                protocol.bFlag = (byte)0;                                  //0-��,1-д,2-����
                protocol.bFullPath = (byte)1;                              //0-���,1-����
                if (gameServer.SendMessage(protocol))
                {
                    _startTime = DateTime.Now;
                    _taskState = TaskState.Processing;
                    return true;
                }
            }

            if (_fileDownloadAutomation != null)
                _fileDownloadAutomation.FinishUnit(gameServer,false);
            return false;
        }

        public override bool Pause(GameServer gameServer)
        {
            if (_taskState == TaskState.Processing)
            {
                _taskState = TaskState.Paused;
                return true;
            }

            return false;
        }

        public override bool Continue(GameServer gameServer)
        {
            if (_taskState == TaskState.Paused)
            {
                _overWrite = false;
                e2g_openfile protocol = new e2g_openfile();
                protocol.szFileName = _sourceFileName;
                protocol.bFlag = (byte)0;                                   //0-��,1-д,2-����
                protocol.bFullPath = (byte)1;                               //0-���,1-����
                if (gameServer.SendMessage(protocol))
                {
                    _taskState = TaskState.Processing;
                    return true;
                }
            }

            return false;
        }

        public override bool Stop(GameServer gameServer)
        {
            if (_taskState == TaskState.Processing || _taskState == TaskState.Paused)
            {
                if (_fileStream != null)
                {
                    _fileStream.Close();
                }

                KProtocolHead protocol = new KProtocolHead();
                protocol.ProtocolId = (ushort)ProtocolDef.e2g_closefile_def;
                gameServer.SendMessage(protocol);
                _taskState = TaskState.Aborting;

                return true;
            }
            else
            {
                if (_fileDownloadAutomation != null)
                    _fileDownloadAutomation.FinishUnit(gameServer,false);
                return false;
            }
        }

        public override bool DoTask(GameServer gameServer, IProtocol message)
        {
            if (gameServer == null || message == null)
            {
                throw new ArgumentException("gameServer & message can not be null.");
            }

            if (_taskState == TaskState.Processing)
            {
                FSEyeResult result;
                switch (message.ProtocolId)
                {
                    case (UInt16)ProtocolDef.g2e_openfile_def:
                        result = (FSEyeResult)((g2e_openfile)message).nRetCode;
                        if (result == FSEyeResult.fseye_success)
                        {
                            if (_fileStream == null)
                            {
                                try
                                {
                                    if (_fileDownloadAutomation != null)
                                    {
                                        //�����Automation�����Ļ�����������ļ������ּ�ǰ׺��
                                        string fileName = SystemConfig.Current.AdminServerUploadFileRootPath + gameServer.Id + "_" + gameServer.Name + "_" + _targetFileName;
                                        _fileStream = new FileStream(fileName, FileMode.OpenOrCreate, FileAccess.Write, FileShare.Read);                                        
                                    }
                                    else
                                    {
                                        _fileStream = new FileStream(SystemConfig.Current.AdminServerUploadFileRootPath + _targetFileName, FileMode.OpenOrCreate, FileAccess.Write, FileShare.Read);
                                    }
                                }
                                catch (Exception)
                                {
                                    if (_fileDownloadAutomation != null)
                                        _fileDownloadAutomation.FinishUnit(gameServer,false);
                                    return false;
                                }
                            }
                            //�õ��ļ�����
                            _fileTotalLength = ((g2e_openfile)message).nFileLen;

                            if ((uint)_fileStream.Length > _fileTotalLength)
                            {
                                //���� �����ļ����ȴ���Դ�ļ� ���������ļ�
                                _fileStream.SetLength(0);
                                _offset = 0;
                            }
                            if (_overWrite)
                            {
                                //����
                                _fileStream.SetLength(0);
                                _offset = 0;
                            }
                            else
                            {
                                //����
                                if (_fileStream.Length == _fileTotalLength)
                                {
                                    //�ļ��Ѿ����������
                                    KProtocolHead protocol = new KProtocolHead();
                                    protocol.ProtocolId = (ushort)ProtocolDef.e2g_closefile_def;
                                    gameServer.SendMessage(protocol);
                                    return true;
                                }
                                else
                                {
                                    _offset = (uint)_fileStream.Length;
                                    _fileStream.Position = _fileStream.Length;
                                }                                
                            }
                            
                            e2g_seekfile seekfile = new e2g_seekfile();
                            seekfile.bKeep = 1;
                            seekfile.nOffset = _offset;

                            //TODO û�д�����ʧ�ܵ����
                            gameServer.SendMessage(seekfile);
                            return true;
                        }
                        else 
                        {
                            //�ļ�������?!
                            Stop(gameServer);
                            if (_fileDownloadAutomation != null)
                                _fileDownloadAutomation.FinishUnit(gameServer,false);
                            return false;
                        }
                    case (UInt16)ProtocolDef.g2e_seekfile_def:
                        result = (FSEyeResult)((g2e_seekfile)message).nRetCode;
                        if (result == FSEyeResult.fseye_success)
                        {
                            e2g_readfile protocol = new e2g_readfile();
                            protocol.nDataLen = 4000;
                            gameServer.SendMessage(protocol);
                            return true;
                        }
                        else
                        {
                            //FSEyeResult.filetran_seek_err
                            //��������δ֪�Ĵ���
                            Stop(gameServer);
                            if (_fileDownloadAutomation != null)
                                _fileDownloadAutomation.FinishUnit(gameServer,false);
                            return false;
                        }
                    case (UInt16)ProtocolDef.g2e_readfile_def:
                        result = (FSEyeResult)((g2e_readfile)message).nRetCode;
                        if (result == FSEyeResult.fseye_success)
                        {
                            g2e_readfile receivedProtocol = message as g2e_readfile;

                            _fileStream.Write(receivedProtocol.szBuf, 0, (int)receivedProtocol.szBuf.Length);
                            _offset += (uint)receivedProtocol.szBuf.Length;
                            
                            CompletePercentage = (float)_fileStream.Length / (float)_fileTotalLength;

                            if (CompletePercentage < 1)
                            {
                                if (_offset != _fileStream.Length)
                                {
                                    //Guard�˶��ļ�ƫ�ƺͱ���д�ļ���ƫ�Ʋ�ͬ
                                    e2g_seekfile seekProtocol = new e2g_seekfile();
                                    seekProtocol.nOffset = (uint)_fileStream.Length;
                                    gameServer.SendMessage(seekProtocol);
                                }
                                else
                                {
                                    e2g_readfile readProtocol = new e2g_readfile();
                                    readProtocol.nDataLen = 4000;
                                    gameServer.SendMessage(readProtocol);
                                }
                            }
                            else
                            {
                                KProtocolHead protocol = new KProtocolHead();
                                protocol.ProtocolId = (ushort)ProtocolDef.e2g_closefile_def;
                                //TODO û�д�����ʧ�ܵ����
                                gameServer.SendMessage(protocol);
                            }

                            return true;
                        }
                        else
                        {
                            Stop(gameServer);
                            if (_fileDownloadAutomation != null)
                                _fileDownloadAutomation.FinishUnit(gameServer,false);
                            return false;
                        }
                    case (UInt16)ProtocolDef.g2e_closefile_def:
                        result = (FSEyeResult)((g2e_closefile)message).nRetCode;
                        if (result == FSEyeResult.fseye_success)
                        {
                            //TODO ��ӹرճɹ�����
                            Complete(gameServer);
                            return true;
                        }
                        else
                        {
                            if (_fileDownloadAutomation != null)
                                _fileDownloadAutomation.FinishUnit(gameServer,false);
                            return false;
                        }
                    default:
                        break;
                }
            }
            else if (_taskState == TaskState.Aborting)
            {
                if (message.ProtocolId == (UInt16)ProtocolDef.g2e_closefile_def)
                {
                    _taskState = TaskState.Stopped;
                }

                return true;
            }

            return false;
        }

        public override bool Complete(GameServer gameServer)
        {
            if (_taskState == TaskState.Processing)
            {
                _taskState = TaskState.Completed;
                if (_fileStream != null)
                {
                    _fileStream.Close();
                }
                if (_fileDownloadAutomation != null)
                    _fileDownloadAutomation.FinishUnit(gameServer,true);
                return true;
            }

            if (_fileDownloadAutomation != null)
                _fileDownloadAutomation.FinishUnit(gameServer,false);
            return false;
        }

        public override void Dispose()
        {
            if (!_disposed)
            {
                _disposed = true;
                if (_fileStream != null)
                {
                    _fileStream.Close();
                }
            }
        }

        public override string ToString()
        {
            return "DownloadFile(" + _sourceFileName + ")";
        }
    }
    #endregion
}
