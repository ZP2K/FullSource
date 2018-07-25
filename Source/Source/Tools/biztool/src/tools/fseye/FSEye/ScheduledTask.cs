//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2007
//
//      Created_datetime : 2007-5
//      File_base        : ScheduledTask
//      File_ext         : cs
//      Author           : ������
//      Description      : �ƻ�����
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Collections;
using System.Threading;
using System.Data;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using FSEye.Security;
using FSEye.DAL;

namespace FSEye
{
	/// <summary>
	/// �ƻ���������
	/// </summary>
	public enum ScheduledTaskType
	{
		Once,
		Repeat,
		DayOfWeek,
		DayOfMonth
	}

	/// <summary>
	/// �ƻ�����ӿ�
	/// </summary>
	public interface IScheduledTask : ISecurity, ISerializable 
	{		
		/// <summary>
		/// �ƻ���������
		/// </summary>
		ScheduledTaskType Type { get; }

		/// <summary>
		/// ��һ��ִ��ʱ��
		/// </summary>
		DateTime NextExecuteTime { get; }

		/// <summary>
		/// �Զ�������
		/// </summary>
		IAutomation Automation { get; set; }

		/// <summary>
		/// �Ƿ�����
		/// </summary>
		bool Enabled { get; set; }

		/// <summary>
		/// ִ��
		/// </summary>
		void Execute();
	}

	/// <summary>
	/// �ƻ��������Ԫ
	/// </summary>
	public class ScheduledTaskUnit
	{
		public IScheduledTask Task = null;
		public long AvoidCheckTimeout = 0;
	}

	/// <summary>
	/// �ƻ����������
	/// </summary>
	public class ScheduledTaskManager : ISecurity, IDisposable
	{
        const string SecurityObjectScheduledTaskManager = "/FSEye/System/ScheduledTaskManager";

		const long MaxAvoidCheckTimeout = 120;//�����쳬ʱ

		const long CheckTaskPeriodTimeSpanTicks = 15 * 1000 * 1000 * 10;//�������tick��

		const int CheckTaskPeriod = 1000 * 15;//����������룩

		FSEyeObject _securityObject;
		
		Timer _checkTaskTimer;//��鶨ʱ��

		IList _taskList;//�������Ԫ�б�

		bool _on;//�Ƿ��

		bool _disposed = false;

		public FSEyeObject SecurityObject
		{
			get { return _securityObject; }
			set { _securityObject = value; }
		}

		/// <summary>
		/// �Ƿ��
		/// </summary>
		public bool On
		{
			get { return _on; }
			set { _on = value; }
		}

		/// <summary>
		/// �ƻ��������Ԫ�б�
		/// </summary>
		public IList ScheduledTaskList
		{
			get { return _taskList; }
		}
				
		public ScheduledTaskManager()
        {
            _taskList = ArrayList.Synchronized(new ArrayList());
			_on = false;
			_checkTaskTimer = new Timer(new TimerCallback(ProcessCheckTasks), null, 5000, CheckTaskPeriod);
        }

		/// <summary>
		/// ����ƻ�����
		/// </summary>
		public void Load()
		{
            this.SecurityObject = AdminServer.TheInstance.SecurityManager.Get(SecurityObjectScheduledTaskManager);
			_taskList.Clear();

			using (IBlazeDatabase db = DbFactory.GetDatabase())
			{
				IBlazeTable scheduledTaskTable = db.GetTable(TableString.ScheduledTaskTableName);
				DataSet ds = new DataSet();
				scheduledTaskTable.Get(ds);
				DataTable table = ds.Tables[TableString.ScheduledTaskTableName];
				foreach (DataRow row in table.Rows)
				{
                    try
                    {
                        int id = (int)row[TableString.ScheduledTaskFieldId];
                        byte[] taskData = (byte[])row[TableString.ScheduledTaskFieldTaskData];

                        MemoryStream taskDataStream = new MemoryStream(taskData);
                        IFormatter formatter = new BinaryFormatter();
                        IScheduledTask task = (IScheduledTask)formatter.Deserialize(taskDataStream);
                        if (task != null)
                        {
                            FSEyeObject secObj = AdminServer.TheInstance.SecurityManager.Get(id);
                            if (secObj != null)
                            {
                                task.SecurityObject = secObj;

                                ScheduledTaskUnit unit = new ScheduledTaskUnit();
                                unit.Task = task;
                                unit.AvoidCheckTimeout = 0;
                                _taskList.Add(unit);
                            }
                        }
                    }
                    catch (Exception e)
                    {
                        Util.DebugLog(e.StackTrace);
                        continue;
                    }
				}
			}
		}

		/// <summary>
		/// �õ��ƻ�����
		/// </summary>
        public ScheduledTaskUnit Get(int taskId)
		{
			for (int i = 0; i < _taskList.Count; i++)
			{
				ScheduledTaskUnit unit = _taskList[i] as ScheduledTaskUnit;
				if (unit.Task.SecurityObject.Id == taskId)
				{
					return unit;
				}
			}

			return null;
		}

		/// <summary>
		/// ���üƻ�����
		/// </summary>
		public bool Enable(int taskId, bool enabled)
		{
            ScheduledTaskUnit taskUnit = Get(taskId);
            if (taskUnit != null)
			{
                if (taskUnit.Task.Enabled != enabled)
				{
					taskUnit.Task.Enabled = enabled;
					Update(taskId, taskUnit.Task, taskUnit.Task.SecurityObject.Name, taskUnit.Task.SecurityObject.Comment);
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// ��Ӽƻ�����
		/// </summary>
		public void Add(IScheduledTask task, string name, string comment)
		{
			if (task == null)
				throw new ArgumentNullException("task");
			if (name == null)
				throw new ArgumentNullException("name");
			if (comment == null)
				throw new ArgumentNullException("comment");

			using (IBlazeDatabase db = DbFactory.GetDatabase())
			{
				try
				{
					db.BeginTrans();

					SecurityManager sm = AdminServer.TheInstance.SecurityManager;
					FSEyeObject newObject = sm.Get(SecurityObject.FullPath + SecurityManager.ObjectPathDelimiter + name, db);
					byte[] automationData = AdminServer.TheInstance.AutomationManager.Save(task.Automation);

					MemoryStream taskDataStream = new MemoryStream();
					IFormatter formatter = new BinaryFormatter();
					formatter.Serialize(taskDataStream, task);
					byte[] taskData = taskDataStream.ToArray();
					if (newObject != null && automationData != null && taskData != null)
					{
						newObject.Comment = comment;
						sm.Set(newObject);
						task.SecurityObject = newObject;

						IBlazeTable scheduledTaskTable = db.GetTable(TableString.ScheduledTaskTableName);
						DataSet ds = new DataSet();
						scheduledTaskTable.Get(ds);
						DataTable table = ds.Tables[TableString.ScheduledTaskTableName];
						DataRow row = table.NewRow();
						row[TableString.ScheduledTaskFieldId] = newObject.Id;
						row[TableString.ScheduledTaskFieldTaskData] = taskData;
						table.Rows.Add(row);
						scheduledTaskTable.Set(table);

						ScheduledTaskUnit unit = new ScheduledTaskUnit();
						unit.Task = task;
						unit.AvoidCheckTimeout = 0;
						_taskList.Add(unit);

                        //����Ȩ��
                        AdminServer.TheInstance.SecurityManager.CopyAce(newObject.Parent, newObject, true);

						db.CommitTrans();
					}
				}
				catch (Exception)
				{
					db.RollbackTrans();
				}
			}
		}

		/// <summary>
		/// ɾ���ƻ�����
		/// </summary>
		/// <param name="taskId">�ƻ�����ID</param>		
		public bool Delete(int taskId)
		{
			for (int i = 0; i < _taskList.Count; i++)
			{
				ScheduledTaskUnit unit = _taskList[i] as ScheduledTaskUnit;
				if (unit.Task.SecurityObject.Id == taskId)
				{
					using (IBlazeDatabase db = DbFactory.GetDatabase())
					{
						try
						{
							db.BeginTrans();

							AdminServer.TheInstance.SecurityManager.Delete(taskId, db);

							IBlazeTable scheduledTaskTable = db.GetTable(TableString.ScheduledTaskTableName);
							scheduledTaskTable.Delete(FilterFactory.CreateEqualFilter(TableString.ScheduledTaskFieldId, taskId));

                            AdminServer.TheInstance.SecurityManager.Delete(taskId, db);

							_taskList.RemoveAt(i);

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
			}

			return false;
		}

		/// <summary>
		/// ���¼ƻ�����
		/// </summary>
		public void Update(int taskId, IScheduledTask newTask, string name, string comment)
		{
			using (IBlazeDatabase db = DbFactory.GetDatabase())
			{
				try
				{
					db.BeginTrans();

					SecurityManager sm = AdminServer.TheInstance.SecurityManager;
					FSEyeObject taskObject = sm.Get(taskId);
					taskObject.Name = name;
					taskObject.Comment = comment;
					sm.Set(taskObject, db);
					newTask.SecurityObject = taskObject;

					using (MemoryStream taskDataStream = new MemoryStream())
					{
						IFormatter formatter = new BinaryFormatter();
						formatter.Serialize(taskDataStream, newTask);
						byte[] taskData = taskDataStream.ToArray();
						DataSet ds = new DataSet();
						IBlazeTable scheduledTaskTable = db.GetTable(TableString.ScheduledTaskTableName);
						scheduledTaskTable.Get(ds);
						DataTable table = ds.Tables[TableString.ScheduledTaskTableName];
						DataRow[] rows = table.Select(TableString.ScheduledTaskFieldId + "=" + newTask.SecurityObject.Id);
						if (rows.Length > 0)
						{
							DataRow row = rows[0];
							row[TableString.ScheduledTaskFieldTaskData] = taskData;
							scheduledTaskTable.Set(table);
						}
					}

					foreach (ScheduledTaskUnit unit in _taskList)
					{
						if (unit.Task.SecurityObject.Id == taskId)
						{
							unit.AvoidCheckTimeout = 0;
							unit.Task = newTask;
							break;
						}
					}

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

		/// <summary>
		/// ���ƻ�����
		/// </summary>
		void ProcessCheckTasks(object state)
		{
			if (!_on)
				return;

			DateTime now = DateTime.Now;
			foreach (ScheduledTaskUnit unit in _taskList)
			{
				if (unit.Task.Enabled)
				{
					unit.AvoidCheckTimeout--;
					if (unit.AvoidCheckTimeout <= 0)
					{
						DateTime nextExeTime = unit.Task.NextExecuteTime;
						if (nextExeTime != DateTime.MinValue)
						{
							long spanTicks = nextExeTime.Subtract(now).Ticks;
							//ʱ�䵽�ˣ�ִ������
							if (spanTicks >= 0)
							{
								if (spanTicks < CheckTaskPeriodTimeSpanTicks)
								{
									unit.Task.Execute();
								}
								else if (spanTicks < (CheckTaskPeriodTimeSpanTicks * 5))
								{
									unit.AvoidCheckTimeout = 0;
								}
								else if (spanTicks < (CheckTaskPeriodTimeSpanTicks * 10))
								{
									unit.AvoidCheckTimeout = 5;
								}
								else
								{
									unit.AvoidCheckTimeout = 10;
								}
							}
						}
						else
						{
							unit.AvoidCheckTimeout = MaxAvoidCheckTimeout;
						}
					}
				}
			}
		}

		public void Dispose()
		{
			if (!_disposed)
			{
				if (_checkTaskTimer != null)
					_checkTaskTimer.Dispose();
				if (_taskList != null)
					_taskList.Clear();
			}
		}
	}

	/// <summary>
	/// �ƻ��������
	/// </summary>
	public abstract class BaseScheduledTask : IScheduledTask
	{
		FSEyeObject _securityObject;

		byte[] _automationData;

		bool _enabled;

		public FSEyeObject SecurityObject
		{
			get { return _securityObject; }
			set { _securityObject = value; }
		}

		public bool Enabled
		{
			get { return _enabled; }
			set { _enabled = value; }
		}

		public IAutomation Automation
		{
			get
			{
				return AdminServer.TheInstance.AutomationManager.Load(_automationData);
			}

			set
			{
				_automationData = AdminServer.TheInstance.AutomationManager.Save(value);
			}
		}

		public void Execute()
		{
			AdminServer.TheInstance.AutomationManager.Do(this.Automation);
		}

		public abstract ScheduledTaskType Type { get; }

		public abstract DateTime NextExecuteTime { get; }

		public BaseScheduledTask()
		{
		}

		protected BaseScheduledTask(SerializationInfo info, StreamingContext context)
		{
			_enabled = info.GetBoolean("enabled");
			_automationData = (byte[])info.GetValue("automation", typeof(byte[]));
		}

		public virtual void GetObjectData(SerializationInfo info, StreamingContext context)
		{
			info.AddValue("enabled", _enabled);
			info.AddValue("automation", _automationData, typeof(byte[]));
		}
	}

	/// <summary>
	/// ����ִ�еļƻ�����
	/// </summary>
	/// <remarks>���磺2007-5-12 15:00:00ִ��</remarks>
	[Serializable]
	public class OnceScheduledTask : BaseScheduledTask
	{
		DateTime _executeTime;

		/// <summary>
		/// ִ��ʱ��
		/// </summary>
		public DateTime ExecuteTime
		{
			get { return _executeTime; }
			set { _executeTime = value; }
		}

		public override ScheduledTaskType Type
		{
			get { return ScheduledTaskType.Once; }
		}

		public override DateTime NextExecuteTime
		{
			get
			{
				if (_executeTime >= DateTime.Now)
					return _executeTime;
				else
					return DateTime.MinValue;
			}
		}

		public OnceScheduledTask()
		{
		}

		public override string ToString()
		{
			return string.Format("{0} [{1}] [Once: {2}]", SecurityObject.Name, SecurityObject.Comment, _executeTime);
		}

		protected OnceScheduledTask(SerializationInfo info, StreamingContext context)
			: base(info, context)
		{
			_executeTime = info.GetDateTime("ExecuteTime");
		}

		public override void GetObjectData(SerializationInfo info, StreamingContext context)
		{
			base.GetObjectData(info, context);
			info.AddValue("ExecuteTime", _executeTime);
		}
	}

	/// <summary>
	/// һ��ʱ���ڼ��һ��ʱ��ִ�е�����
	/// </summary>
	/// <remarks>���磺��5:00:00��12:00:00��ÿ��15����ִ��һ��</remarks>
	[Serializable]
	public class RepeatScheduledTask : BaseScheduledTask
	{
		TimeSpan _startTime;

		TimeSpan _endTime;

		TimeSpan _interval;

		/// <summary>
		/// ��ʼʱ��
		/// </summary>
		public TimeSpan StartTime
		{
			get { return _startTime; }
			set { _startTime = value; }
		}

		/// <summary>
		/// ����ʱ��
		/// </summary>
		public TimeSpan EndTime
		{
			get { return _endTime; }
			set { _endTime = value; }
		}

		/// <summary>
		/// ���ʱ��
		/// </summary>
		public TimeSpan Interval
		{
			get { return _interval; }
			set { _interval = value; }
		}

		public override ScheduledTaskType Type
		{
			get { return ScheduledTaskType.Repeat; }
		}

		public override DateTime NextExecuteTime
		{
			get
			{
				DateTime now = DateTime.Now;
				TimeSpan currentTime = now.TimeOfDay;
				if (_startTime >= currentTime)
				{
					return now.Date.Add(_startTime);
				}
				else if (_endTime >= currentTime)
				{
					//TODO �����д�����
					TimeSpan span = currentTime.Subtract(_startTime);
					long period = span.Ticks / _interval.Ticks;
					return now.Date.Add(_startTime.Add(new TimeSpan((period + 1) * _interval.Ticks)));
				}
				else
				{
					return DateTime.MinValue;
				}
			}
		}

		public RepeatScheduledTask()
		{
		}

		public override string ToString()
		{
			return string.Format("{0} [{1}] [Repeat: From {2} to {3}, interval={4}]", SecurityObject.Name, SecurityObject.Comment, _startTime, _endTime, _interval);
		}

		protected RepeatScheduledTask(SerializationInfo info, StreamingContext context)
			: base(info, context)
		{
			_startTime = new TimeSpan((long)info.GetInt64("StartTime"));
			_endTime = new TimeSpan((long)info.GetInt64("EndTime"));
			_interval = new TimeSpan((long)info.GetInt64("Interval"));
		}

		public override void GetObjectData(SerializationInfo info, StreamingContext context)
		{
			base.GetObjectData(info, context);
			info.AddValue("StartTime", _startTime.Ticks);
			info.AddValue("EndTime", _endTime.Ticks);
			info.AddValue("Interval", _interval.Ticks);
		}
	}

	/// <summary>
	/// ÿ�ܵ�ĳ��ļƻ�����
	/// </summary>
	/// <remarks>���磺ÿ�ܶ�2:00:00ִ��</remarks>
    [Serializable]
	public class DayOfWeekScheduledTask : BaseScheduledTask
	{
		DayOfWeek[] _dayOfWeekArray;

		TimeSpan _timeOfDay;

		public DayOfWeek[] DayOfWeek
		{
			get { return _dayOfWeekArray; }
			set { _dayOfWeekArray = value; }
		}

		public TimeSpan TimeOfDay
		{
			get { return _timeOfDay; }
			set { _timeOfDay = value; }
		}

		public override ScheduledTaskType Type
		{
			get { return ScheduledTaskType.DayOfWeek; }
		}

		public override DateTime NextExecuteTime
		{
			get
			{
				DateTime now = DateTime.Now;
				DayOfWeek currentDayOfWeek = now.DayOfWeek;
				TimeSpan currentTimeOfDay = now.TimeOfDay;

				//TODO ������Ҫ����߼�
				
				foreach (DayOfWeek day in _dayOfWeekArray)
				{
					if (day == currentDayOfWeek)
					{
						if (_timeOfDay >= currentTimeOfDay)
						{
							return DateTime.Today.Add(_timeOfDay);
						}
					}
                    if (day > currentDayOfWeek)
                    {
                        return DateTime.Today.AddDays((double)(day - currentDayOfWeek)).Add(_timeOfDay);
                    }
				}
                return DateTime.Today.AddDays((double)(7 + _dayOfWeekArray[0] - currentDayOfWeek)).Add(_timeOfDay);
			}
		}

		public DayOfWeekScheduledTask()
		{
		}

        public override string ToString()
        {
            StringBuilder days = new StringBuilder();
            if (_dayOfWeekArray != null)
            {
                foreach (DayOfWeek day in _dayOfWeekArray)
                {
                    days.Append(day.ToString());
                    days.Append(',');
                }
            }
            return string.Format("{0} [{1}] [DayOfWeek: {2} {3} ]", SecurityObject.Name, SecurityObject.Comment, days.ToString(), _timeOfDay);
        }

        public DayOfWeekScheduledTask(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
            _dayOfWeekArray = (DayOfWeek[])info.GetValue("DayOfWeekArray", typeof(DayOfWeek[]));
            _timeOfDay = new TimeSpan((long)info.GetInt64("TimeOfDay"));
        }

        public override void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            base.GetObjectData(info, context);
            info.AddValue("DayOfWeekArray", _dayOfWeekArray, typeof(DayOfWeek[]));
            info.AddValue("TimeOfDay", _timeOfDay.Ticks);
        }
	}
}