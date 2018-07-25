//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2006-10
//      File_base        : TempFileManager
//      File_ext         : cs
//      Author           : ������
//      Description      : ��ʱ�ļ�������
//
//////////////////////////////////////////////////////////////////////

using System;
using System.IO;
using System.Collections;

/// <summary>
/// ��ʱ�ļ���������
/// </summary>
public class TempFileManager
{
	/// <summary>
	/// ��ʱ�ļ���
	/// </summary>
	private class TempFile
	{
		private string _path;

		private long _timeToLive;

		public TempFile(string filePath, long timeToLive)
		{
			_path = filePath;
			_timeToLive = timeToLive;
		}

		public void TimeEsacpe(long time)
		{
			_timeToLive -= time;
		}

		public string Path
		{
			get { return _path; }
		}

		public long TimeToLive
		{
			get { return _timeToLive; }
		}
	}

	private static TempFileManager g_theInstance = new TempFileManager();

	private ArrayList _fileList;

	private long _checkInterval;

	private System.Threading.Timer _checkTempFileTimer;

	public static TempFileManager TheInstance
	{
		get { return g_theInstance; }
	}

	/// <summary>
	/// �����
	/// </summary>
	/// <remarks>Ĭ��ֵ�ǣ�5000����</remarks>
	public long CheckInterval
	{
		get { return _checkInterval; }
		set
		{
			_checkInterval = value;
			//�ı���������Ҫ�ı䶨ʱ���ļ��
			_checkTempFileTimer.Change(0, _checkInterval);
		}
	}

	/// <summary>
	/// ʹ�õ���ģʽ
	/// </summary>
	private TempFileManager()
	{
		_checkInterval = 5000;
		_fileList = ArrayList.Synchronized(new ArrayList());
		_checkTempFileTimer = new System.Threading.Timer(new System.Threading.TimerCallback(CheckFiles), null, 0, _checkInterval);
	}

	/// <summary>
	/// ���������ʱ�ļ�
	/// </summary>
	public void ClearAllTempFiles()
	{
		lock (_fileList.SyncRoot)
		{
			foreach (TempFile file in _fileList)
			{
				FileInfo info = new FileInfo(file.Path);
				if (info.Exists)
				{
					info.Delete();
				}
			}

			_fileList.Clear();
		}
	}

	~TempFileManager()
	{
		ClearAllTempFiles();
	}

	/// <summary>
	/// �����ʱ�ļ�
	/// </summary>
	/// <param name="filePath">�ļ�·��</param>
	/// <param name="timeToLive">�ļ�����ʱ�䣬��λ������</param>
	public void AddTempFile(string filePath, long timeToLive)
	{
		_fileList.Add(new TempFile(filePath, timeToLive));
	}

	/// <summary>
	/// ���һ����ʱ�ļ�
	/// </summary>
	private void CheckFiles(object state)
	{
		lock (_fileList.SyncRoot)
		{
			if (_fileList.Count > 0)
			{
				ArrayList toBeRemoved = new ArrayList();//��Ҫɾ������ʱ�ļ��б�

				int count = _fileList.Count;
				TempFile file = null;
				for (int i = 0; i < count; i++)
				{
					file = _fileList[i] as TempFile;

					//����ʱ�ļ�������ʱ�����һ��
					file.TimeEsacpe(CheckInterval);

					//�����ʱ�ļ����������յ�
					if (file.TimeToLive <= 0)
					{
						toBeRemoved.Add(file);

						//�Ӵ�����ɾ�������ʱ�ļ�
						FileInfo info = new FileInfo(file.Path);
						if (info.Exists)
						{
							info.Delete();
						}
					}
				}

				//����ʱ�ļ��б���ɾ�������ʱ�ļ�
				foreach (TempFile tempfile in toBeRemoved)
				{
					_fileList.Remove(tempfile);
				}
				toBeRemoved.Clear();
			}
		}
	}
}
