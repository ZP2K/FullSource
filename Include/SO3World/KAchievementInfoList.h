#ifndef _KACHIEVEMENT_INFO_LIST_H_
#define _KACHIEVEMENT_INFO_LIST_H_

enum KACHIEVEMENT_SHIFT_TYPE
{
    astNeverShift,          // ��ת��
    astComplete,            // ��ȫת��
    astShiftTriggerValOnce, // ֻת��TriggerVal����һ��

    astTotal
};

struct KACHIEVEMENT_INFO 
{
    int     nTriggerVal;
    int     nShiftID;
    BYTE    byShiftType;
    int     nPoint;
    int     nExp;
    BYTE    byPrefix;
    BYTE    byPostfix;
};

class KAchievementInfoList
{
public:
    KAchievementInfoList();
    ~KAchievementInfoList();

    BOOL Init();
    void UnInit();

    KACHIEVEMENT_INFO* GetInfo(int nAchievementID);

private:
    typedef std::map<int, KACHIEVEMENT_INFO> KINFO_LIST;
    KINFO_LIST m_InfoList;
};
#endif
