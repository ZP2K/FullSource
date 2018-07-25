////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KTradingBox.h
//  Version     : 1.0
//  Creator     : Ye Bixuan
//  Create Date : 2006-12-18 
//  Comment     : 

////////////////////////////////////////////////////////////////////////////////

#ifndef _KTRADING_BOX_H_
#define _KTRADING_BOX_H_

#include "Global.h"

class KPlayer;
class KItem;
enum  TRADING_RESULT_CODE;

class KTradingBox
{
public:
    KTradingBox();
    ~KTradingBox();

    BOOL Init(KPlayer* pSelf, KPlayer* pOther);
    void UnInit();

    // dwGridIndex: �Ž����������ĸ�����
    // dwBoxIndex:  ��Ʒ����ҵ��ĸ���������
    // dwPosIndex:  ��Ʒ�ڱ��������ʲô�ط�
#ifdef _SERVER
    TRADING_RESULT_CODE AddItem(DWORD dwGridIndex, DWORD dwBoxIndex, DWORD dwPosIndex);
#endif

#ifdef _CLIENT
    BOOL        AddItem(DWORD dwGridIndex, KItem* pItem);
#endif

    BOOL        DelItem(DWORD dwGridIndex);
    KItem*      GetItem(DWORD dwGridIndex);

    BOOL        SetMoney(int nMoney);
    int         GetMoney();
    
    void        Confirm();
    void        Cancel();

    void        SetKey(int nKey);
    int         GetKey();

    KPlayer*    GetOtherSide();
    
#ifdef _SERVER
    BOOL        FinishTradingCheck();
    void        FinishTrading();
#endif

    void        ClearTradingInfo();
private:
    void        SetConfirmFlag(BOOL bConfirm);
    BOOL        GetConfirmFLag();
#ifdef _SERVER
    BOOL        CheckItemExistLimit(DWORD dwTabType, DWORD dwItemIndex, int nItemAmount);
#endif

    struct KITEM_GRID
    {
        DWORD   dwItemID;    
        KItem*  pItem;

        int     nStackNum;
        DWORD   dwBoxIndex;
        DWORD   dwPosIndex;
    };

    int             m_nMoney;
    KITEM_GRID      m_ItemGrids[MAX_TRADING_BOX_SIZE];
    int             m_nTradingItemCount;

    KPlayer*        m_pSelf;
    KPlayer*        m_pOtherSide;

    BOOL            m_bConfirm;
    int             m_nKey;  
};

#endif // _KTRADING_BOX_H_


// ����TradingBox�����ʱ��: 
// �ڽ���˫����ͬ�⽻�׵�ʱ, �ö���Ż��������

// �ͷ�TradingBox�����ʱ��:
// 1. �ڽ��׳ɹ�
// 2. �ڽ��ױ�ȡ��
// 3. �ڿͻ���: ����һ���ܳ��˿ͻ��������Χ��9��Region, �ڵ���Player����UnInit()ʱ
// 4. �����: ������˳���Ϸ��ʱ��, �ڵ���Player����UnInit()ʱ
