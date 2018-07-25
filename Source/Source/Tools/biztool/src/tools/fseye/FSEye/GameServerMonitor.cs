//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2007-3
//      File_base        : GameServerMonitor
//      File_ext         : cs
//      Author           : ������
//      Description      : ��Ϸ������������
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using System.Text;

namespace FSEye
{
	/// <summary>
	/// Ԥ���ȼ�
	/// </summary>
	public enum AlertLevel
	{
		Attention,
		Important,
		Serious
	}

	/// <summary>
	/// Ԥ������
	/// </summary>
	public abstract class AlertCondition
	{
		AlertLevel _level;

		/// <summary>
		/// Ԥ���ȼ�
		/// </summary>
		public AlertLevel Level
		{
			get { return _level; }
			set { _level = value; }
		}

		public AlertCondition()
		{
			_level = AlertLevel.Attention;
		}

		public AlertCondition(AlertLevel level)
		{
			_level = level;
		}

		/// <summary>
		/// ��������Ƿ�����
		/// </summary>
		/// <param name="checkObject">������</param>
		public abstract bool Check(object checkObject);
	}

	/// <summary>
	/// �����
	/// </summary>
	public class CheckResult
	{
		IList _alertConditions;

		/// <summary>
		/// δ�������Ԥ������
		/// </summary>
		public IList AlertConditions
		{
			get { return _alertConditions; }			
		}

		/// <summary>
		/// û������
		/// </summary>
		/// <remarks>�жϼ�����Ƿ�һ��������û���κ�Ԥ��������������</remarks>
		public bool NoProblem
		{
			get { return (_alertConditions.Count == 0); }
		}

		public CheckResult()
		{
			_alertConditions = new ArrayList();
		}

		public override string ToString()
		{
			StringBuilder str = new StringBuilder();
			foreach(AlertCondition condition in _alertConditions)
			{
				str.Append(condition.ToString());
				str.Append(", ");
			}

			return str.ToString();
		}
	}

	/// <summary>
	/// ��Ϸ������������
	/// </summary>
	public class GameServerMonitor
	{
		public const long DefaultCheckPeriod = 2000;//Ĭ�ϼ����
		public const long DefaultDueTime = 2000;//Ĭ���ӳ�

		IList _alertConditions;

		IList _lastCheckResults;

		Timer _checkGameServerTimer;

		long _checkPeriod = DefaultCheckPeriod;

		long _dueTime = DefaultDueTime;

		public IList AlertConditions
		{
			get { return _alertConditions; }
		}

		public IList LastCheckResults
		{
			get { return _lastCheckResults; }
		}

		public long CheckPeriod
		{
			get { return _checkPeriod; }
			set
			{
				_checkPeriod = value;
				_checkGameServerTimer.Change(_dueTime, _checkPeriod);
			}
		}

		public GameServerMonitor()
		{
            _alertConditions = ArrayList.Synchronized(new ArrayList());
            _lastCheckResults = ArrayList.Synchronized(new ArrayList());
			_checkGameServerTimer = new Timer(new TimerCallback(CheckAllGameServers), null, Timeout.Infinite, Timeout.Infinite);

			//TODO For Debug
			AlertCondition condition = new CpuUsageAlertCondition(50);
			condition.Level = AlertLevel.Attention;
			_alertConditions.Add(condition);
			condition = new DiskUsageAlertCondition(DiskUsageAlertCondition.Mode.FreeDiskSize, 3000);
			_alertConditions.Add(condition);
		}

		public void Start()
		{
			_checkGameServerTimer.Change(_dueTime, _checkPeriod);
		}

		public void Stop()
		{
			_checkGameServerTimer.Change(Timeout.Infinite, Timeout.Infinite);
		}

		public CheckResult Check(GameServer server)
		{
			CheckResult result = new CheckResult();

			foreach(AlertCondition condition in _alertConditions)
			{
				if (!condition.Check(server))
				{
					result.AlertConditions.Add(condition);
				}
			}

			return result;
		}

		private void CheckAllGameServers(object arg)
		{
			_lastCheckResults.Clear();
			foreach (GameServer server in AdminServer.TheInstance.GameServerManager.Servers)
			{
				CheckResult result = Check(server);
				_lastCheckResults.Add(result);
                server.LastCheckResult = result;
			}
		}
	}
}
