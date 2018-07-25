#ifndef _KBISHOP_CLIENT_H_
#define _KBISHOP_CLIENT_H_

#include <string>
#include <vector>
#include "Common/KG_Socket.h"
#include "SO3GlobalDef.h"
#include "SO3ProtocolBasic.h"

class KMonster;

enum LOGIN_RESULT
{
    Login_Invalid,
    Login_Sucess,
    Login_ConnectBishop,
    Login_VerifyAccount,
    Login_GetRolelist,
    Login_CreateRole,
    Login_DeleteRole,
    Login_RequestLoginGameserver,
    Login_Total
};

struct UI_ROLE_LIST_ITEM
{
    char    szAccountName[_NAME_LEN];
    char    szRoleName[_NAME_LEN];
    int     nRoleType;
    int     nLevel;
    WORD    wRepresentID[REPRESENT_COUNT];
};

struct UI_ROLE_CREATE_PARAM
{
    char    szRoleName[_NAME_LEN];
    int     nRoleType; 
    DWORD   dwMapID;
    DWORD   dwMapCopyIndex;
    WORD    wRepresentID[REPRESENT_COUNT];
};

#define MAX_RETRY_COUNT (10)

class KBishopClient
{
public:
	KBishopClient(void);
	~KBishopClient(void);

    int Init(KMonster *pMonster);
    int UnInit();
    LOGIN_RESULT GetLoginResult();
    const char *GetResultDescription();

public:
    // �û��ֶ��ĵ�һ�ε�¼����
    int BeginLogin(
        const char cszLoginIP[], unsigned short usLoginPort,
        const char cszAccountName[], const char cszPasswordMD5[]
    );

    // ��������ʱ����(����Ҫ�ٵ��������������)
    int AutoLogin();

    // ��״̬����λ, ���ڵ�½�������е�ʱ��, ���Ҫȡ��, �͵���
    void Cancel();

    // ��ȡ��ɫ����
    int GetRoleCount();
    // ȡ�ý�ɫ��, nRoleIndex��0��ʼ,��֤BufferSize�㹻��,�������ֻᱻ�ض�
    int GetRoleListItem(UI_ROLE_LIST_ITEM *pRoleListItem, int nRoleIndex);
    // �ڲ����סѡ��Ľ�ɫ
    int RoleLogin(const char cszRoleName[]);
    // �ú���������Ҫɾ���Ľ�ɫ��
    int DeleteRole(const char cszRoleName[], const char cszSecPasswordMD5[]);
    // ���ô�����ɫ����ز���
    int CreateRole(const UI_ROLE_CREATE_PARAM &crRoleCreateParam);

private:
    int Login();
    int ProcessLogin();
    int Activate();

    KMonster *m_pOwner;
    int m_nRetryCount;
    LOGIN_RESULT m_nLoginResult;
    static char ms_szResultDescription[Login_Total][256];

private:
    char                    m_szLoginIP[32];
    unsigned short          m_usLoginPort;
    IKG_SocketStream       *m_piSocketStream;
    int                     m_nAutoLoginFlag;

    char                    m_szAccountName[_NAME_LEN];
    KSG_PASSWORD            m_Password;    
    char                    m_szActivateCode[_NAME_LEN];

    UI_ROLE_CREATE_PARAM    m_RoleCreateParam;
    char                    m_szLoginRole[_NAME_LEN];
    char                    m_szRoleToDelete[_NAME_LEN];

	typedef std::vector<UI_ROLE_LIST_ITEM>  ROLE_LIST;
    ROLE_LIST               m_RoleList;    

private:
    typedef enum
    {
        LOGIN_STATE_NONE,
        LOGIN_STATE_START,
        LOGIN_STATE_REQUEST_LOGIN_VERIFY,
        LOGIN_STATE_WAIT_LOGIN_VERIFY_RESPOND,
        LOGIN_STATE_WAIT_PLAYER_INPUT_PSW_AGAIN,
        LOGIN_STATE_WAIT_PLAYER_INPUT_ACTIVATE_CODE,
        LOGIN_STATE_WAIT_ROLE_LIST,
        LOGIN_STATE_WAIT_PLAYER_OPERATE,
        LOGIN_STATE_REQUEST_CREATE_ROLE,
        LOGIN_STATE_WAIT_CREATE_RESPOND,
        LOGIN_STATE_REQUEST_DELETE_ROLE,
        LOGIN_STATE_WAIT_DELETE_RESPOND,
        LOGIN_STATE_REQUEST_LOGIN_GAME,
        LOGIN_STATE_WAIT_GAME_LOGIN_INFO
    } LOGIN_STATE;

    LOGIN_STATE m_CurrentState;

    int login_state_start();
    int login_state_request_login_verify();
    int login_state_wait_login_verify_respond();
    int login_state_wait_role_list();
    int login_state_wait_player_operate();
    int login_state_request_create_role();
    int login_state_wait_create_respond();
    int login_state_request_delete_role();
    int login_state_wait_delete_respond();
    int login_state_request_login_game();
    int login_state_wait_game_login_info();
};

#endif	//_KBISHOP_CLIENT_H_
