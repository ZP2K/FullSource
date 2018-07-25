using System;
using System.Collections.Generic;
using System.Text;

namespace FSEye
{
	/// <summary>
	/// CPUʹ����Ԥ������
	/// </summary>
	public class CpuUsageAlertCondition : AlertCondition
	{
		public const int DefaultAlertValue = 100;

		int _alertValue = DefaultAlertValue;

		public CpuUsageAlertCondition(int alertValue)
		{
			_alertValue = alertValue;
		}

		public override bool Check(object checkObject)
		{
			GameServer server = checkObject as GameServer;

			if (server.CurrentCpuLoad > _alertValue)
				return false;

			return true;
		}

		public override string ToString()
		{
			return "CPU Usage > " + _alertValue + "%";
		}
	};

	/// <summary>
	/// ����ʹ����Ԥ������
	/// </summary>
	public class DiskUsageAlertCondition : AlertCondition
	{
		public enum Mode
		{
			FreeDiskPercentage,
			FreeDiskSize
		}

		long _alertValue;
		Mode _alertMode;

		/// <summary>
		/// Ԥ��ֵ
		/// </summary>
		public long AlertValue
		{
			get { return _alertValue; }
			set { _alertValue = value; }
		}

		/// <summary>
		/// Ԥ��ģʽ
		/// </summary>
		public Mode AlertMode
		{
			get { return _alertMode; }
			set { _alertMode = value; }
		}

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="alertMode">Ԥ��ģʽ</param>
		/// <param name="alertValue">Ԥ��ֵ</param>		
		public DiskUsageAlertCondition(Mode alertMode, long alertValue)
		{
			_alertMode = alertMode;
			_alertValue = alertValue;			
		}

		public override bool Check(object checkObject)
		{
			GameServer server = checkObject as GameServer;

			foreach (DiskInfo info in server.DiskInfos)
			{
				switch (_alertMode)
				{
					case Mode.FreeDiskSize:
						if (info.FreeSpace <= _alertValue)
							return false;
						break;
					case Mode.FreeDiskPercentage:
						if ((info.FreeSpace * 100 / info.TotalSpace) <= _alertValue)
							return false;
						break;
					default:
						break;
				}
			}		

			return true;
		}

		public override string ToString()
		{
			switch(_alertMode)
			{
				case Mode.FreeDiskSize:
					return "Free Disk Size <= " + _alertValue + "M";
				case Mode.FreeDiskPercentage:
					return "Free Disk Percentage <= " + _alertValue + "%";
				default:
					return base.ToString();
			}
		}
	};
}
