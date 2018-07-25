#ifndef _KPLAYER_PK_H_
#define	_KPLAYER_PK_H_

#include "Global.h"
#include "SO3Result.h"

class KPlayer;

#define PK_SCRIPT SCRIPT_DIR"/player/PVPScript.lua"

enum PK_STATE
{
	pksInvalid,
	pksIdle,

	pksWaitForDuelRespond,	// �ȴ��д�ظ�
	pksConfirmDuel,		    // ���������д裬�ڿ����Ƿ�ͬ��
	pksPrepareDuel,	        // �д�ͬ���ˣ��ȴ���ʼ����ʱ
	pksDueling, 			// �д���
	pksDuelOutOfRange,	    // �д��У����ǳ�����Χ��

	pksStartSlay,			// ������ɱ����ʱ
    pksSlaying,             // ��ɱ�� 
	pksExitSlay,			// ����ر���ɱ������ʱ��

	pksTotal
};

// PK ������ԭ��
enum PK_END_CODE
{
    pkecInvalid,

    pkecBeat,               // ������
    pkecOffline,            // ����
    pkecUnexpectedDie,      // ��������
    pkecSwitchMap,          // �е�ͼ
    pkecOutofRangeTimeUp,   // ������Χ��ʱ

    pkecTotal
};

enum PK_OPERATE
{
	pkoInvalid,

	pkoApplyDuel,
	pkoRefuseDuel,
	pkoAcceptDuel,
	pkoLossDuel,
	pkoApplySlay,
	pkoCloseSlay,

	pkoTotal
};

class KPKController
{
public:
	KPKController();
	virtual ~KPKController();

	BOOL			Init(KPlayer* pPlayer);
	void			UnInit();
    
    DWORD   		GetTargetID(){ return m_dwTargetID; }
    KPlayer*   		GetTargetPlayer();
    DWORD           GetLoserID(){ return m_dwDuelLoser; }
    void            SetLoserID(DWORD dwID) { m_dwDuelLoser = dwID; }

    PK_RESULT_CODE	CanApplyDuel(KPlayer* pTarget);
    PK_RESULT_CODE	CanApplySlay();

	void			SetState(
        PK_STATE    eState, 
        int         nEndFrame   = 0, 
        DWORD       dwTarget    = ERROR_ID, 
        BOOL        bRefuseDuel = false
    );
	PK_STATE		GetState()	{ return m_eState; }
    
    BOOL            IsDuel();
    BOOL            IsOnSlay();
    PK_STATE        GetPKState();

#ifdef _SERVER
    int             GetCloseSlayLeftTime();
    void			Activate();
	void			SetDuelPostion(DWORD dwMapID, int nX, int nY, int nZ);

    PK_RESULT_CODE	ApplyDuel(DWORD dwTargetID);	// �����д�
    PK_RESULT_CODE	RefuseDuel();                   // �ܾ��д�
    PK_RESULT_CODE	AcceptDuel();                   // �����д�
    PK_RESULT_CODE  LossDuel();        	            // �д�ʧ��
	PK_RESULT_CODE	ApplySlay();                    // ������ɱ
	PK_RESULT_CODE	CloseSlay();                    // �ر���ɱ
#endif

private:

#ifdef _SERVER
	BOOL			CheckDuelDistance(int nDistance);
	void			CancelDuel(BOOL bRefuseDuelFlag = false);

	BOOL			CreateDuelFlag(int nX, int nY, int nZ);
	void			DeleteDuelFlag();

    void			OnWaitConfirmDuel();
	void			OnPrepareDuel();
	void			OnDueling();
	void			OnDuelOutOfRange();
    void            OnStartSlay();
	void			OnSlaying();
	void			OnExitSlay();

public:
    BOOL            CallDuelAcceptScript();
    BOOL            CallDuelBeginScript();
    BOOL            CallDuelEndScript(int nCode);
#endif

private:
    KPlayer*		m_pPlayer;
    PK_STATE		m_eState;
    DWORD			m_dwTargetID;
    DWORD           m_dwDuelLoser;

#ifdef _SERVER
    int				m_nDuelCenterX;
    int				m_nDuelCenterY;
    int				m_nDuelCenterZ;
    DWORD			m_dwMapID;
    int				m_nTimer;
    DWORD			m_dwFlagDoodadID;
#endif
};
#endif
