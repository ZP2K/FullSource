//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2007
//
//      Created_datetime : 2007-4-12
//      File_base        : Strategy
//      File_ext         : cs
//      Author           : ������
//      Description      : ����
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Data;
using FSEye.DAL;
using FSEye.Security;

namespace FSEye
{
	/// <summary>
	/// FSEye�¼�
	/// </summary>
	public enum FSEyeEvent
	{
		TestEvent = 0,//�����¼�
		ServerConnected = 1000,//����������
		ServerDisconnected,//�������Ͽ�
		GameStart,//��Ϸ����
		GameStop,//��Ϸ�ر�
		UpdateGameBegin,//������Ϸ��ʼ
		UpdateGameSucceed,//������Ϸ�ɹ�
		UpdateGameFail,//������Ϸʧ��
		StartGameFail,//������Ϸʧ��
		GameStopByAccident,//��Ϸ����ر�
		GameStartByAccident,//��Ϸ��������
		ModulePaysysError,//��ϷPaysysģ�鷢������
		ModuleDatabaseError,//��Ϸ���ݿ�ģ�鷢������
	}

	/// <summary>
	/// ���Թ�����
	/// </summary>
	public class StrategyManager : ISecurity, IDisposable
	{
        const string SecurityObjectStrategyManager = "/FSEye/System/StrategyManager";

		FSEyeObject _securityObject;

		IList _strategyList;

		IList<FSEyeEvent> _eventList;

		bool _on;

		public FSEyeObject SecurityObject
		{
			get { return _securityObject; }
			set { _securityObject = value; }
		}

		public bool On
		{
			get { return _on; }
			set { _on = value; }
		}

		public IList StrategyList
		{
			get { return _strategyList; }
		}

		public IList<FSEyeEvent> EventList
		{
			get { return _eventList; }
		}

		public StrategyManager()
		{
			_strategyList = ArrayList.Synchronized(new ArrayList());
			_eventList = new List<FSEyeEvent>();
		}

		//�����¼�
		public void OnEvent(FSEyeEvent fsEyeEvent, AutomationContext context)
		{
			if (context == null)
			{
				throw new ArgumentNullException("context");
			}

			if (_on)
			{
				foreach (Strategy strategy in _strategyList)
				{
					if (strategy.Enabled && strategy.Event == fsEyeEvent)
					{
						AdminServer.TheInstance.AutomationManager.Do(strategy.Automation, context);
					}
				}
			}
		}

		/// <summary>
		/// �õ�����
		/// </summary>
		public Strategy Get(int strategyId)
		{
			foreach (Strategy strategy in _strategyList)
			{
				if (strategy.SecurityObject.Id == strategyId)
					return strategy;
			}

			return null;
		}

		/// <summary>
		/// ��Ӳ���
		/// </summary>		
		public bool Add(Strategy strategy, string name, string comment)
		{
			using (IBlazeDatabase db = DbFactory.GetDatabase())
			{
				try
				{
					db.BeginTrans();

					SecurityManager sm = AdminServer.TheInstance.SecurityManager;
					FSEyeObject newObject = sm.Get(SecurityObject.FullPath + SecurityManager.ObjectPathDelimiter + name, db);
					byte[] strategyBytes = AdminServer.TheInstance.AutomationManager.Save(strategy.Automation);

                    if (newObject != null && strategyBytes != null)
                    {
                        newObject.Comment = comment;
                        sm.Set(newObject);
                        strategy.SecurityObject = newObject;

                        IBlazeTable table = db.GetTable(TableString.StrategyTableName);
                        DataSet strategyData = new DataSet();
                        table.Get(strategyData);
                        DataTable strategyTable = strategyData.Tables[TableString.StrategyTableName];

                        DataRow newRow = strategyTable.NewRow();
                        newRow[TableString.StrategyFieldId] = newObject.Id;
                        newRow[TableString.StrategyFieldEvent] = (int)strategy.Event;
                        newRow[TableString.StrategyFieldAutomation] = strategyBytes;
                        newRow[TableString.StrategyFieldEnabled] = (strategy.Enabled ? 1 : 0);
                        strategyTable.Rows.Add(newRow);
                        table.Set(strategyTable);

                        //����Ȩ��
                        AdminServer.TheInstance.SecurityManager.CopyAce(newObject.Parent, newObject, true);

                        _strategyList.Add(strategy);

                        db.CommitTrans();

                        return true;
                    }
                    else
                    {
                        return false;
                    }
				}
				catch (Exception ex)
				{
					if (db != null)
						db.RollbackTrans();

                    throw ex;
				}
			}
		}

		public bool Enable(int strategyId, bool enable)
		{
			Strategy strategy = Get(strategyId);
			if (strategy != null)
			{
				if (strategy.Enabled != enable)
				{
					strategy.Enabled = enable;
					Update(strategyId, strategy, strategy.SecurityObject.Name, strategy.SecurityObject.Comment);
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// ���²���
		/// </summary>
		public void Update(int strategyId, Strategy newStrategy, string name, string comment)
		{
			using (IBlazeDatabase db = DbFactory.GetDatabase())
			{
				try
				{
					db.BeginTrans();

					SecurityManager sm = AdminServer.TheInstance.SecurityManager;
					FSEyeObject strategyObject = sm.Get(strategyId);
					strategyObject.Name = name;
					strategyObject.Comment = comment;
					sm.Set(strategyObject, db);
					newStrategy.SecurityObject = strategyObject;

					IBlazeTable table = db.GetTable(TableString.StrategyTableName);
					DataSet strategyData = new DataSet();
					table.Get(strategyData);
					DataTable strategyTable = strategyData.Tables[TableString.StrategyTableName];
					AutomationManager am = AdminServer.TheInstance.AutomationManager;

					byte[] strategyBytes = am.Save(newStrategy.Automation);
					DataRow[] rows = strategyTable.Select(TableString.StrategyFieldId + "=" + strategyId);
					if (rows != null && rows.Length > 0)
					{
						DataRow row = rows[0];
						row[TableString.StrategyFieldEvent] = (int)newStrategy.Event;
						row[TableString.StrategyFieldEnabled] = (newStrategy.Enabled ? 1 : 0);
						row[TableString.StrategyFieldAutomation] = strategyBytes;

						table.Set(strategyTable);
					}

					for (int i = 0; i < _strategyList.Count; i++)
					{
						Strategy strategy = _strategyList[i] as Strategy;
						if (strategy.SecurityObject.Id == strategyId)
						{
							_strategyList.RemoveAt(i);
							_strategyList.Add(newStrategy);
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
		/// ɾ������
		/// </summary>
		/// <param name="strategyId">����ID</param>
		public bool Delete(int strategyId)
		{
			for (int i = 0; i < _strategyList.Count; i++)
			{
				Strategy strategy = _strategyList[i] as Strategy;
				if (strategy.SecurityObject.Id == strategyId)
				{
					using (IBlazeDatabase db = DbFactory.GetDatabase())
					{
						try
						{
							db.BeginTrans();

							AdminServer.TheInstance.SecurityManager.Delete(strategyId, db);

							IBlazeTable strategyTable = db.GetTable(TableString.StrategyTableName);
							strategyTable.Delete(FilterFactory.CreateEqualFilter(TableString.StrategyFieldId, strategyId));

							_strategyList.RemoveAt(i);

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
		/// �����������
		/// </summary>
		public void Load()
		{
            this.SecurityObject = AdminServer.TheInstance.SecurityManager.Get(SecurityObjectStrategyManager);
			_strategyList.Clear();

			using (IBlazeDatabase db = DbFactory.GetDatabase())
			{
				IBlazeTable table = db.GetTable(TableString.StrategyTableName);
				DataSet strategyData = new DataSet();
				table.Get(strategyData);
				DataTable strategyTable = strategyData.Tables[TableString.StrategyTableName];
				StrategyManager sm = AdminServer.TheInstance.StrategyManager;
				AutomationManager am = AdminServer.TheInstance.AutomationManager;
				foreach (DataRow row in strategyTable.Rows)
				{					
					int strategyId = (int)row[TableString.StrategyFieldId];

                    try
                    {
                        Strategy strategy = new Strategy();
                        strategy.Event = (FSEyeEvent)row[TableString.StrategyFieldEvent];
                        strategy.Automation = am.Load((byte[])row[TableString.StrategyFieldAutomation]);
                        strategy.Enabled = ((int)row[TableString.StrategyFieldEnabled]) == 0 ? false : true;

                        FSEyeObject secObj = AdminServer.TheInstance.SecurityManager.Get(strategyId);
                        if (secObj != null)
                        {
                            strategy.SecurityObject = secObj;
                            _strategyList.Add(strategy);
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

		public void Dispose()
		{
			_strategyList.Clear();
		}
	}

	/// <summary>
	/// ����
	/// </summary>
	public class Strategy : ISecurity
	{
		FSEyeObject _securityObject;

		bool _enabled = false;

		FSEyeEvent _event;

		byte[] _automationData;

		public FSEyeObject SecurityObject
		{
			get { return _securityObject; }
			set { _securityObject = value; }
		}

		/// <summary>
		/// ����
		/// </summary>
		public bool Enabled
		{
			get { return _enabled; }
			set { _enabled = value; }
		}

		/// <summary>
		/// �¼�
		/// </summary>
		public FSEyeEvent Event
		{
			get { return _event; }
			set { _event = value; }
		}

		/// <summary>
		/// �Զ�������
		/// </summary>
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

		public Strategy()
		{
		}

		public override string ToString()
		{
			return string.Format("[{0}] {1} ({2})", this._securityObject.Id, this._securityObject.Name, this._securityObject.Comment);
		}

		public override bool Equals(object obj)
		{
			return (obj != null && obj.GetType() == this.GetType() && this.SecurityObject == (obj as Strategy).SecurityObject);
		}

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
	}
}
