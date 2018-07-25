//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2006-8
//      File_base        : TableString
//      File_ext         : cs
//      Author           : ������
//      Description      : ���ַ���
//
//////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using System.Text;

/// <summary>
/// ���ַ����������˱����͸����������
/// </summary>
/// <author>������</author>
public class TableString
{
	//FSEYE_OBJECT
	public const string ObjectTableName = "fseye_object";
	public const string ObjectFieldId = "id";
	public const string ObjectFieldOwner = "owner";
	public const string ObjectFieldParentId = "parent_id";
	public const string ObjectFieldName = "name";
	public const string ObjectFieldComment = "comment";

	//USER
	public const string UserTableName = "user";
	public const string UserFieldId = "id";
	public const string UserFieldUserName = "user_name";
	public const string UserFieldRealName = "real_name";
	public const string UserFieldPassword = "password";

	//USER_GROUP
	public const string UserGroupTableName = "user_group";
	public const string UserGroupFieldId = "id";
	public const string UserGroupFieldGroupName = "group_name";

	//USER_IN_GROUP
	public const string UserInGroupTableName = "user_in_group";
	public const string UserInGroupFieldUserId = "user_id";
	public const string UserInGroupFieldGroupId = "group_id";
	public const string UserInGroupFieldEstablishDatetime = "establish_datetime";

	//ACE
	public const string AceTableName = "ace";
	public const string AceFieldAceId = "ace_id";
	public const string AceFieldObjectId = "object_id";
	public const string AceFieldOperatorId = "operator_id";
	public const string AceFieldAceType = "ace_type";
	public const string AceFieldOperation = "operation";

	//GAME_SERVER
	public const string GameServerTableName = "game_server";
	public const string GameServerFieldId = "id";
	public const string GameServerFieldNetAddress = "net_address";
	public const string GameServerFieldGameDir = "game_dir";
	public const string GameServerFieldLordPort = "lord_port";
	public const string GameServerFieldMaxPlayer = "max_player";
	public const string GameServerFieldIntranetMask = "intranet_mask";
	public const string GameServerFieldInternetMask = "internet_mask";
	public const string GameServerFieldPaysysIp = "paysys_ip";
	public const string GameServerFieldPaysysPort = "paysys_port";
	public const string GameServerFieldPaysysLoginName = "paysys_login_name";
	public const string GameServerFieldPaysysLoginPwd = "paysys_login_pwd";	
	public const string GameServerFieldDbHost = "db_host";
	public const string GameServerFieldDbName = "db_name";
	public const string GameServerFieldDbLoginName = "db_login_name";
	public const string GameServerFieldDbLoginPwd = "db_login_pwd";
	public const string GameServerFieldDbBackupDir = "db_backup_dir";
	public const string GameServerFieldFSEyeIp = "fseye_ip";
	public const string GameServerFieldFSEyePort = "fseye_port";
	public const string GameServerFieldHasBeenConfigGuard = "has_been_config_guard";
	public const string GameServerFieldServerType = "server_type";
    public const string GameServerFieldCfgString = "ConfigString";
    public const string GameServerFieldFtpDownloadServer = "FtpServer";
    public const string GameServerFieldGmcIp = "gmc_ip";
    public const string GameServerFieldGmcAccount = "gmc_account";
	
	//SERVER_GROUP
	public const string ServerGroupTableName = "server_group";
	public const string ServerGroupFieldId = "id";
	public const string ServerGroupFieldType = "TYPE";
    public const string ServerGroupFieldGroupEnable = "GroupEnable";
    public const string ServerGroupFieldMaxPlayers = "MaxPlayers";
    public const string ServerGroupFieldMaxPlayersLogTime = "MaxPlayersLogTime";
    public const string ServerGroupFieldCabinet = "cabinet";
    public const string ServerGroupFieldGroupVersion = "GroupVersion";


    //SERVER_REGION
    public const string ServerRegionTableName = "server_region";
    public const string ServerRegionFieldId = "id";
    public const string ServerRegionFieldMaxPlayers = "MaxPlayers";
    public const string ServerRegionFieldMaxPlayersLogTime = "MaxPlayersLogTime";
    public const string ServerRegionFieldRegionName = "RegionName";
    public const string ServerRegionFieldRegionType = "RegionType";

	//GAME_LOG
	public const string GameLogTableName = "game_log";
	public const string GameLogFieldId = "id";
	public const string GameLogFieldGameServerId = "game_server_id";
	public const string GameLogFieldDate = "log_date";
	public const string GameLogFieldTotalMoney = "total_money";
	public const string GameLogFieldTotalMoneyInBox = "total_money_in_box";

    //SERVER_PLUGIN
    public const string ServerPlugInTableName = "server_plugin";
    public const string ServerPlugInFieldId = "id";
    public const string ServerPlugInFieldGameServerId = "game_server_id";
    public const string ServerPlugInFieldPlugInGuid = "plugin_guid";
    public const string ServerPlugInFieldParameter1 = "parameter_1";
    public const string ServerPlugInFieldParameter2 = "parameter_2";
    public const string ServerPlugInFieldParameter3 = "parameter_3";
    public const string ServerPlugInFieldPlugInName = "plugIn_name";

	//Strategy
	public const string StrategyTableName = "strategy";
	public const string StrategyFieldId = "id";
	public const string StrategyFieldEvent = "event";
	public const string StrategyFieldAutomation = "automation";
	public const string StrategyFieldEnabled = "enabled";

	//ScheduledTask
	public const string ScheduledTaskTableName = "scheduled_task";
	public const string ScheduledTaskFieldId = "id";
	public const string ScheduledTaskFieldTaskData = "taskdata";

    //Task
    public const string TaskTableName = "task";
    public const string TaskFieldId = "id";
    public const string TaskFieldName = "name";
    public const string TaskFieldAutomation = "automation";
    public const string TaskFieldPeriodUnit = "period_unit";
    public const string TaskFieldInterval = "interval";
    public const string TaskFieldStart = "start";
    public const string TaskFieldEnd = "end";
    public const string TaskFieldSelectedDay = "selected_day";
    public const string TaskFieldSelectedMonth = "selected_month";
    public const string TaskFieldRepeatPeriod = "repeat_period";
    public const string TaskFieldRepeatPersistenceTime = "repeat_persistence_time";
    public const string TaskFieldEnabled = "enabled";

    //UpdateServer
    public const string UpdateServerTableName = "update_server";
    public const string UpdateServerFieldId = "id";
    public const string UpdateServerFieldAddress = "ftp_address";
    public const string UpdateServerFieldPort = "port";
    public const string UpdateServerFieldUsername = "username";
    public const string UpdateServerFieldPassword = "password";
    public const string UpdateServerFieldLiteral = "literal";
    public const string UpdateServerFieldUpdateServerType = "server_type";

	//SystemLog
	public const string SystemLogTableName = "system_log";
	public const string SystemLogFieldId = "id";
	public const string SystemLogFieldOperator = "operator";
	public const string SystemLogFieldOperation = "operation";
	public const string SystemLogFieldTarget = "target";
	public const string SystemLogFieldDescription = "description";
	public const string SystemLogFieldLogTime = "logtime";

	//PlayerCount
	public const string PlayerCountTableName = "player_count";
	public const string PlayerCountFieldServerId = "server_id";
	public const string PlayerCountFieldRecordTime = "record_time";
	public const string PlayerCountFieldMaxPlayerCount = "max_player_count";
	public const string PlayerCountFieldMinPlayerCount = "min_player_count";
	public const string PlayerCountFieldAveragePlayerCount = "average_player_count";

    //GMOpLog
    public const string GMOpLogTableName = "gm_op_log";
    public const string GMOpLogFieldId = "id";
    public const string GMOpLogFieldUserId = "user_id";
    public const string GMOpLogFieldOperation = "operation";
    public const string GMOpLogFieldTarget = "target";
    public const string GMOpLogFieldServerId = "server_id";
    public const string GMOpLogFieldDescription = "description";
    public const string GMOpLogFieldLogTime = "log_time";
    public const string GMOpLogFieldTargetType = "target_type";

    //account_info
    public const string AccountInfoTableName = "account_info";
    public const string AccountInfoFieldId = "id";
    public const string AccountInfoFieldName = "name";
    public const string AccountInfoFieldPassword = "password";

    //ibshop
    public const string IbShopTableName = "ibshop";
    public const string IbShopFieldId = "id";
    public const string IbShopFieldTimeStamp = "time_stamp";
    public const string IbShopFieldScript = "script";

    //BatchTask
    public const string BatchTaskTableName = "batch_task";
    public const string BatchTaskId = "id";
    public const string BatchTaskAutomation = "automation";
    public const string BatchTaskStep = "step";
    public const string BatchTaskServerIds  = "server_ids";

    //ServerEvent
    public const string ServerEventTableName = "server_event";
    public const string ServerEventFieldServerId = "server_id";
    public const string ServerEventFieldEventTime = "event_time";
    //EventType
    //1��ʾ��������(GameStart)
    //2��ʾ����ʧ��(GameStartFail)
    //3��ʾ�����ر�(GameStop)
    //4��ʾ��������(GameStartByAccident)
    //5��ʾ����(GameStopByAccident)
    //�μ�FSEye\GameServerManager.cs,����GameStateChanged �� FSEye\GameServerManager.cs,Enum ServerEventType
    public const string ServerEventFieldEventType = "event_type";


    //ServerOperationLog
    //��server�Ĳ�����¼��
    public const string ServerOperationLogTableName = "server_operation_log";
    public const string ServerOperationLogFieldId = "id";                               //���м�¼��id��Ϊ�����ֶΣ�������
    public const string ServerOperationLogFieldOperationId = "operation_id";            //������ˮ�ţ��߼���ͬһ�ζ�����server���еĲ���������ͬ����ˮ��
    public const string ServerOperationLogFieldUserId = "user_id";                      //��վ�û���ID
    public const string ServerOperationLogFieldServerOrGroup = "server_or_group";       //����������server����servergroup.0Ϊserver,1Ϊservergroup
    public const string ServerOperationLogFieldTargetId = "target_id";                  //���������ID
    public const string ServerOperationLogFieldOperationType = "operation_type";        //�������͡���������ʾ
    public const string ServerOperationLogFieldOperationResult = "operation_result";    //�����Ƿ�ɹ���0Ϊʧ�ܣ�1Ϊ�ɹ�
    public const string ServerOperationLogFieldDescription = "description";             //�Բ���������������
    public const string ServerOperationLogFieldLogTime = "log_time";                    //дlog��ʱ��
    public const string ServerOperationLogFieldFatherRegion = "fatherRegion";           //�ݹ������������ID,�ö��ŷָ�

    //server_map
    //�洢�������б�(���ͻ���ʹ��),��GameServer/ServerListConfig.aspx��ʹ��
    public const string ServerMapTableName = "server_map";
    public const string ServerMapFieldId = "ID";
    public const string ServerMapFieldParentId = "parent_id";
    public const string ServerMapFieldName = "name";
    public const string ServerMapFieldMapType = "map_type";
    public const string ServerMapFieldIndex = "map_index";
    public const string ServerMapFieldFlag = "flag";
    public const string ServerMapFieldMapServerId = "map_server_id";
}
