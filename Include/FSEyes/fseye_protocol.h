#ifndef _FSEYE_PROTOCOL_H
#define _FSEYE_PROTOCOL_H
#include "ProtocolBasic.h"

#pragma pack(push, 1)

enum
{
	emKEYE_NAMELEN = 32,
};

struct KProtocolHead
{
	unsigned short wProtocol;
	unsigned short wServer;
};

struct KSubProtocolHead : public KProtocolHead
{
	unsigned short wSubProtocol;
};

/*
struct g2e_ping {
	KProtocolHead sHead;
};

struct e2g_ping {
	KProtocolHead sHead;
};
*/

struct e2g_openfile : public KProtocolHead
{
	unsigned char bFlag;
	unsigned char bFullPath;
	char szFileName[256];
};

struct g2e_openfile : public KProtocolHead
{
	unsigned int nFileLen;
	int nRetCode;
};

struct e2g_readfile : public KProtocolHead
{
	unsigned short nDataLen;
};

struct g2e_readfile : public KProtocolHead
{
	int nRetCode;
	unsigned int nReadLen;
};

struct e2g_writefile : public KProtocolHead
{
	unsigned short nDataLen;
};

struct g2e_writefile : public KProtocolHead
{
	unsigned int nWritedLen;
	int nRetCode;
};

struct e2g_seekfile : public KProtocolHead
{
	unsigned char bKeep;
	unsigned int nOffset;
};

struct g2e_seekfile : public KProtocolHead
{
	int nRetCode;
};

/*
struct e2g_closefile {
	KProtocolHead sHead;
};
*/

struct g2e_closefile : public KProtocolHead
{
	int nRetCode;
};

/*
struct e2g_loadplug {
	KProtocolHead sHead;
};
*/

struct g2e_loadplug : public KProtocolHead
{
	int nRetCode;
};

/*
struct e2g_unloadplug {
	KProtocolHead sHead;
};
*/

struct g2e_unloadplug : public KProtocolHead
{
	int nRetCode;
};

/*
struct e2g_getcpubaseinfo {
	KProtocolHead sHead;
};
*/

struct cpubaseinfo {
	char szCPUName[100];
	char szCPUVendor[100];
	char szCPUDesc[100];
	int nCPULoad;
};

struct g2e_getcpubaseinfo : public KProtocolHead
{
	unsigned short nCPUCount;
	cpubaseinfo Processor[10];
};

struct g2e_getcpuload : public KProtocolHead
{
	unsigned short nCPUCount;
	unsigned short nLoad[10];
};

struct g2e_getmeminfo : public KProtocolHead
{
	unsigned int nTotalMem;
	unsigned int nFreeMem;
};

struct diskbaseinfo {
	char szDiskDesc[100];
	unsigned int nTotalSize;
	unsigned int nFreeSize;
};

struct g2e_getdiskinfo : public KProtocolHead
{
	unsigned short nDiskCount;
	diskbaseinfo Disk[10];
};

struct netcardinfo {
	char szCardDesc[100];
	char szIP[20];
	char szMask[20];
	char szMac[20];
};

struct g2e_getnetinfo : public KProtocolHead
{
	unsigned short	nCardCount;
	netcardinfo		Card[10];
	char			szSystemName[100];
	char			szHostName[100];
};

struct netcardload {
	unsigned int nTXSize;
	unsigned int nRXSize;
	unsigned int nTXRate;
	unsigned int nRXRate;
};

struct g2e_getcardload : public KProtocolHead
{
	unsigned short nCardCount;
	netcardload Card[10];
};

struct procinfo {
	char szProcName[50];
	unsigned int nPID;
	unsigned int nMemUse;
	unsigned int nVMSize;
	unsigned int nCPUTime;
	unsigned int nThreadCount;
};

struct g2e_getprocinfo : public KProtocolHead
{
	unsigned short nProcCount;
	procinfo Proc[80];
};

struct pluginfo {
	char szPath[50];
	char szModHAndModE[50];
	char szGUID[50];
	char szAuthor[50];
	char szVersion[50];
	char szDesc[100];
};

struct g2e_getpluginfo : public KProtocolHead
{
	int PlugCount;
	pluginfo PlugInfo[5];
};

struct e2g_exesql  : public KProtocolHead
{
	unsigned int	nSessionID;
	char			szDatabase[48];
	unsigned int	nSqlLen;
};

// ִ��SQL��䣺�����һ��SQL���ִ�в��ɹ���ִ�еڶ���SQL���
struct e2g_exesql2 : public KProtocolHead
{
	unsigned int	nSessionID;
	char			szDatabase[48];
	unsigned int	nSql1Len;			// ��һ��SQL��䳤��
	unsigned int	nSql2Len;			// �ڶ���SQL��䳤��
};

struct g2e_exesql : public KProtocolHead
{
	unsigned int nSessionID;
	unsigned int nRetCode;
	unsigned int nLen;
};

struct e2g_exesyscmd : public KProtocolHead
{
	unsigned short	bNeedOutput;
	unsigned short	nCmdLen;
	unsigned short	nBufLen;
};

struct g2e_exesyscmd  : public KProtocolHead
{
	int ReturnCode;
	char OutputBuff[256];
};

struct e2l_SayToWorld : public KSubProtocolHead
{
	char Message[256];
};

struct l2e_GetBasicInfo : public KSubProtocolHead
{
	unsigned short PlayerCount;
	unsigned short UpTime;
};
struct e2l_ExeGMCmd : public KProtocolHead
{
	unsigned int  nSessionID;
	unsigned short uIsRole;
	unsigned short uGSorGC;
	char PlayerName[32];
	char Command[1024];
	unsigned int uServerCount;
};

struct l2e_ExeGMCmd : public KSubProtocolHead
{
	unsigned int nSessionID;
	int ReturnCode;
	int nResultLen;
	char szResult[1024];
};

struct e2g_Authentication : public KProtocolHead
{
	int X;
};

struct g2e_Authentication : public KProtocolHead
{
	char Y[64];
};

enum KE_GUARD_PLAYERCOUNT
{
	emKGUARD_PLAYERCOUNT_TOTOAL = 0,	// �ܵ��������
	emKGUARD_PLAYERCOUNT_OFFLIVE,		// �����й�����
	emKGUARD_PLAYERCOUNT_COUNT
};

// �䳤Э�飬�����һ��INT���飬���������ĿΪemKPLAYERCOUNT_COUNT���������Ϊ-1��ʾδ֪
struct l2e_PlayerCount : public KSubProtocolHead
{
	unsigned short uCount;
};

struct g2e_GetGuardDir : public KProtocolHead
{
	char GuardDir[256];
};

struct e2l_Who : public KSubProtocolHead
{
	unsigned short Offset;
};

struct l2e_Who_PlayerInfo {
	char Name[32];
};

struct l2e_Who : public KSubProtocolHead
{
	unsigned short PlayerCount;
	l2e_Who_PlayerInfo PlayerList[20];
};

struct e2l_GetGlobalVariable : public KSubProtocolHead
{
	unsigned short VariableIndex;
};

struct l2e_GetGlobalVariable : public KSubProtocolHead
{
	unsigned short VariableIndex;
	int VariableValue;
};

struct e2l_SetGlobalVariable : public KSubProtocolHead
{
	unsigned short VariableIndex;
	int VariableValue;
};

struct e2g_Key {
	unsigned char Data[128];
};

struct e2g_ConfigInfo {
	int X;
	char Y[64];
	unsigned short ConfigFileDataLength;
	char ConfigFileData[4096];
};

struct e2g_config : public KProtocolHead
{
	e2g_Key Key;
	unsigned char ConfigInfo[4166];
};

struct e2g_file : public KProtocolHead
{
	unsigned short wFileLen;
	char szPath[256];
	char szData[4096];
};

struct e2g_switchmode : public KProtocolHead 
{
	unsigned short Mode;
};

struct l2e_GetGameStartTime : public KSubProtocolHead
{
	char GameStartTime[32];
};

struct l2e_ReportError : public KSubProtocolHead
{
	int Module;
	int ErrorCode;
};

struct e2g_DeliverKey : public KProtocolHead
{
	e2g_Key Key;
};

struct e2g_exesyscmd_large : public KProtocolHead
{
	unsigned short bNeedOutput;
	char Command[6144];
	char InputBuff[256];
};

struct l2e_info : public KSubProtocolHead
{
	char Info[1024];
};

struct l2e_info_large : public KSubProtocolHead
{
	char InfoLarge[4096];
};

struct l2e_getpid : public KProtocolHead
{
	int	nPid;	// ����ID
};

struct e2g_ftpdownload : public KProtocolHead
{
	char	szFtpHost[128];
	char	szFtpUser[32];
	char	szFtpPass[32];
	char	szFtpPath[256];
};

struct g2e_ftpdownload : public KProtocolHead
{
	unsigned char	byProgress;	// ��ǰ�����ذٷֱ�(0-100)
	unsigned char	byResult;	// ��������1Ϊ�ɹ���0Ϊʧ��
	unsigned short	wLen;		// ������Ϣ����
};

struct e2g_updateserver : public KProtocolHead
{
	char			szFile[128];		// ���°����ļ���������·����
	char			szCode[33];			// �ļ�MD5��
	unsigned char	byMode;				// 0=���£�1����MD5У��
};

struct g2e_updateserver : public KProtocolHead
{
	unsigned short	nCurrentStep;	// ��ǰ���裨��1��ʼ��
	unsigned short	nStepCount;		// �ܲ�����
	unsigned short	nResult;		// ��������1Ϊ�ɹ���0Ϊʧ��
	unsigned short	nMsgLen;		// ������Ϣ���ȣ�����Ϊ0��
};

struct c2r_gmcmd : public KProtocolHead
{
	unsigned short nCmdLen;			// �����
};

struct r2c_gmcmd : public KProtocolHead
{
	char Command[6144];
};

enum KE_GMCMD_ASKER
{
	emKE_eyes,
	emKE_gmc,
	emKE_game,
	emKE_none
};

struct e2l_gmcmd_gc : public KSubProtocolHead 
{
    char            szGmName[emKEYE_NAMELEN];
    char            szCommand[0];
};

struct e2l_remote_lua_call : public KSubProtocolHead 
{
    char   szFunction[32];
    size_t uParamLen;
    BYTE   byParam[0];
};

struct e2l_gmcmd_gs : public KSubProtocolHead
{
    char            szGmName[emKEYE_NAMELEN];
	char			szRoleName[emKEYE_NAMELEN];
    char            szCommand[0];
};

struct l2e_send_gm_message : public KProtocolHead
{
    char            szGmName[emKEYE_NAMELEN];
    char            szMessage[0];
};

struct e2l_queryrolelist : public KSubProtocolHead
{
	int				nSession;					// �Ự
	char			szAccount[emKEYE_NAMELEN];	// �ʺ���
};

struct l2e_rolelist : public KProtocolHead
{
	int				nSession;					// �Ự
	char			szAccount[emKEYE_NAMELEN];	// �ʺ���
	unsigned char	byRoleCount;				// ��ɫ��Ŀ��ÿ����ɫ��ռ{emKEYE_NAMELEN}���ֽ�
};

enum KE_GMCMD_RESULT
{
	emKGMCMD_RESULT_SUCCESS,			// �ɹ�
	emKGMCMD_RESULT_FAILURE,			// ʧ��
	emKGMCMD_RESULT_DISPFAIL,			// �����޷������ͣ����ܵ�ԭ������Ŀ������������/δ����
	emKGMCMD_RESULT_TIMEOUT,			// ��ʱ
	emKGMCMD_RESULT_NONE,				// ����ѯ�ģ�Ŀ�겻����
	emKGMCMD_RESULT_NOTALLOW,			// �����������
	emKGMCMD_RESULT_WRONGCMD,			// �������
	emKGMCMD_RESULT_COUNT
};

struct l2e_gmcmd : public KProtocolHead
{
	int				nSession;		// �ỰID
	unsigned char	byAction;		// 0��ʾ������1��ʾ����
	unsigned char	byResult;		// �����������KE_GMCMD_RESULT
	unsigned char	byAsker;		// 0��ʾfseye 1��ʾgmc
	int				nLen;			// �����ַ�������
};

// GMƵ����Ϣ
struct l2e_gmchn : public KSubProtocolHead
{
	char	szAccount[32];
	char	szRole[32];
	int		nMsgLen;
};

struct l2e_report : public KSubProtocolHead 
{
    int     nType;
    char    szKey[32];
    int     nValueLen;
    char    szValue[0];
};

// python�ű���guard���͵���־Э�飨�䳤Э�飩
struct p2g_log : public KProtocolHead
{
	unsigned short	wLen;	// ��Ϣ����
};

// ������������Ӧ��serverid�����û�ж�Ӧserverid��Ϊ0
struct e2g_serverid : public KProtocolHead
{
	int	aryServerId[16];	// ��KE_GUARDSERVER@kipcstrea.h
};

struct g2e_serverpid : public KProtocolHead
{
	int nPid;				// ����ID��0��ʾû�г�ʼ����-1��ʾ���̽�����>0��ʾ����
};

struct KPROTOCOL_L2G_PIDRET : public KSubProtocolHead
{
	int	nPid;		// ����ID
};

struct e2l_update_custom_info : public KSubProtocolHead
{
};

struct l2e_update_custom_info : public KSubProtocolHead
{
    size_t uDataLen;
    BYTE   byData[0];
};

struct l2e_update_map_list : public KSubProtocolHead
{
    int nCount;
    struct KMapDesc
    {
        DWORD   dwMapID;
        char    szName[16];
        int     nCopyIndex;
        int     nPlayerCount;
        int     nNpcCount;
        int     nTotalRegion;
        int     nRegionActive;
    } MapDesc[0];
};

struct l2e_prize_info : public KSubProtocolHead
{
    char szAccount[_NAME_LEN];
    char szRoleName[_NAME_LEN];
    DWORD dwRoleID;
    char szType[128];
    int nValue;
};

enum ProtocolDef {
	e2l_header_def = 0,
	l2e_header_def,
	g2e_ping_def,
	e2g_ping_def,
	e2g_openfile_def,
	g2e_openfile_def,
	e2g_readfile_def,
	g2e_readfile_def,
	e2g_writefile_def,
	g2e_writefile_def,
	e2g_seekfile_def,
	g2e_seekfile_def,
	e2g_closefile_def,
	g2e_closefile_def,
	e2g_getcpubaseinfo_def,
	g2e_getcpubaseinfo_def,
	e2g_getcpuload_def,
	g2e_getcpuload_def,
	e2g_getmeminfo_def,
	g2e_getmeminfo_def,
	e2g_getdiskinfo_def,
	g2e_getdiskinfo_def,
	e2g_getnetbaseinfo_def,
	g2e_getnetinfo_def,
	e2g_getcardload_def,
	g2e_getcardload_def,
	e2g_getprocinfo_def,
	g2e_getprocinfo_def,
	e2g_getpluginfo_def,
	g2e_getpluginfo_def,
	e2g_exesql_def,
	g2e_exesql_def,
	e2g_exesyscmd_def,
	g2e_exesyscmd_def,
	e2l_SayToWorld_def,
	e2l_GetBasicInfo_def,
	l2e_GetBasicInfo_def,
	e2l_ExeGMCmd_def,
	l2e_ExeGMCmd_def,
	e2g_Authentication_def,
	g2e_Authentication_def,
	e2l_PlayerCount_def,
	l2e_PlayerCount_def,
	e2g_GetGuardDir_def,
	g2e_GetGuardDir_def,
	e2g_UpdateGuard_def,
	e2l_Who_def,
	l2e_Who_def,
	e2l_GetGlobalVariable_def,
	l2e_GetGlobalVariable_def,
	e2l_SetGlobalVariable_def,
	e2g_config_def,
	e2g_file_def,
	g2e_config_def,
	e2g_switchmode_def,
	g2e_switchmode_def,
	e2l_GetGameStartTime_def,
	l2e_GetGameStartTime_def,
	l2e_ReportError_def,
	e2g_DeliverKey_def,
	e2g_exesyscmd_large_def,
	l2e_info_def,
	l2e_info_large_def,
	e2l_exit_def,
	e2l_getpid_def,					// ��ȡ����ID��struct: KProtocolHead
	l2e_getpid_def,					// ��ȡ����ID��struct: l2e_getpid
	l2e_ping_def,					// server����web��ping
	e2g_exesql2_def,				// ִ��SQL��䣺�����һ��SQL���ִ�в��ɹ���ִ�еڶ���SQL���
	e2g_updateserver_def,			// ���·���������
	g2e_updateserver_def,			// ���·��������
	c2r_gmcmd_def,					// ��gmc��gamecenter(relay)��������Э��
	r2c_gmcmd_def,
	e2l_gmcmd_def,					// ִ��GM����
	l2e_gmcmd_def,					// ����GM������
	l2e_gmchn_def,					// GMƵ����Ϣ
	e2l_queryrolelist_def,			// ��ѯ��ɫ�б�
	l2e_rolelist_def,				// ��ɫ�б�
	e2g_ftpdownload_def,			// ���ظ��°�����
	g2e_ftpdownload_def,			// ���ظ��°����
	e2g_updateself_def,				// ����guard
	p2g_log_def,					// python�ű���guard���͵���־Э��
	e2g_serverid_def,				// ������������Ӧ��servrerid��Э��Ϊe2g_serverid
	g2l_header_def,					// guard��server���͵�Э��
	l2g_header_def,					// server��guard���͵�Э��
	e2g_serverpid_def,				// ����server�Ľ���ID��Э��ΪKProtocolHead
	g2e_serverpid_def,				// Ӧ��server�Ľ���ID��Э��Ϊg2e_serverpid
	g2e_groupVer_def,               // Ӧ��ĳ�����İ汾��
	e2g_groupVer_def,               // ��ȡĳ�����İ汾��

	//�����Զ����Э�鿪ʼ
	e2l_gmcmd_gs_def = 200,			// ��GameServer��ִ��GM����
	e2l_gmcmd_gc_def,				// ��GameCenter��ִ��GM����
    e2l_remote_lua_call_def,        // Զ�̵���Game cener�ű�
	l2e_send_gm_message_def,        // ��GM����һ���ַ�����Ϣ
	e2l_update_custom_info_def,     // Web��ͻ�������������Զ�����ʾ��Ϣ
	l2e_update_custom_info_def,     // �ͻ�������Web�����Զ�����ʾ��Ϣ
    l2e_update_map_list_def,		// ��ͼ��ϵ�б�
	l2e_artificial_help_def,        // �˹�����
	l2e_report_trick_def,			// �ٱ�������Ϣ
    l2e_report_cheat_def,           // �ٱ����
	l2e_prize_info_def,             // ����Ϣ
    l2e_report_def,					// GameCenter��kingeye����һЩ��Ϸ�ڵ�����

	fseye_protocol_count,
	fseye_protocol_maxcount = 255	// ��ֹguard�Ͳ��Դ��������
};

// server��guard���͵���Э��
enum KE_PROTOCOL_L2G
{
	emKPROTOCOL_L2G_PIDRET,			// ���ؽ���ID
};

// guard��server���͵���Э��
enum KE_PROTOCOL_G2L
{
	emKPROTOCOL_G2L_PIDREQ,			// ѯ�ʽ���ID
};

enum FSEyeResult {
	fseye_success = 0,
	guard_err,
	guard_create_client_err,
	guard_startup_client_err,
	guard_client_invalidhadle,
	guard_client_send_err,
	plug_opendll_err,
	plug_getproc_err,
	plug_creat_err,
	filetran_opening_err,
	filetran_app_err,
	filetran_cre_err,
	filetran_seek_err,
	filetran_close_err,
	mydb_err_opendb,
	mydb_err_query,
	mydb_err_dbuncon,
	servicestate_stopped,
	servicestate_starting,
	servicestate_running,
	servicestate_stopping,
	servicestate_restarting,
	db_err,
	db_transaction_started_err,
	db_transaction_not_started_err,
	db_rebuild_table_err,
	db_delete_table_err,
	db_table_not_exist_err,
	db_table_exist_err,
	db_begin_transaction_err,
	db_commit_transaction_err,
	db_rollback_transaction_err,
	db_get_table_data_err,
	db_adapter_not_init_err,
	db_add_table_data_err,
	db_update_table_data_err,
	as_err,
	as_bad_argument_err,
	as_user_not_exist_err,
	as_user_already_login_err,
	as_fm_task_complete,
	sec_err,
	sec_allow,
	sec_deny,
	sec_unknown,
	sec_not_found_in_cache,
	sec_not_enough_privilege_err,
	sec_ace_already_exist_err,
	sec_ace_not_exist_err,
	sec_user_already_login_err,
	sec_user_not_exist_err,
	sec_user_not_login_err,
	l2e_ExeGMCmd_err,
	l2e_ExeGMCmd_player_not_found_err,
	g2e_ExeSysCmd_done,
	g2e_ExeSysCmd_busy,
	g2e_ExeSysCmd_result,
	mydb_more_result,
};

#pragma pack(pop)
#endif
