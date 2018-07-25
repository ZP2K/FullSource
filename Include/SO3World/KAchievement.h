#ifndef _KACHIEVEMENT_H_
#define _KACHIEVEMENT_H_

#include "KCustomData.h"

#define ACHIEVEMENT_SCRIPT SCRIPT_DIR"/Achievement/Achievement.lua"
#define SCRIPT_ON_FINISH_ACHIEVEMENT "OnFinishAchievement"

struct KACHIEVEMENT_INFO;
class KPlayer;

class KAchievement
{
public:
    KAchievement();
    ~KAchievement();

    BOOL Init(KPlayer* pPlayer);

    BOOL Save(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize);
    BOOL Load(BYTE* pbyData, size_t uDataLen);

    BOOL AddCount(int nID, int nDeltaCount);
    int GetCount(int nID);
    BOOL IsAchieve(int nID);
    BOOL AddPoint(int nDeltaPoint);
    BOOL Acquire(int nID);

private:
    BOOL OnAchieve(int nID, KACHIEVEMENT_INFO* pInfo);
#ifdef _SERVER
    BOOL CallFinishScript(int nID);
#endif

public:
    KPlayer*                                    m_pPlayer;
    int                                         m_nPoint;       // �ɾͻ���
    int                                         m_nRecord;      // ��Ϸ��ɶ�

private:
    KCustomData<MAX_ACHIEVEMENT_ID / CHAR_BIT>  m_BitSet;       // �ɾ�bitset
    typedef std::map<int, int> KCOUNTER_TABLE;
    KCOUNTER_TABLE                              m_CounterTable; // ��������
};
#endif
