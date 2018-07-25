#ifndef KMENTOR_TABLE_DATA_H
#define KMENTOR_TABLE_DATA_H

#include "KGPublic.h"

#pragma pack(1)
struct KMentorRecordDBHead_Analyser
{
    int nVersion;
};

struct KMentorRecordDB_Analyser : KMentorRecordDBHead_Analyser
{
    time_t  nCreateTime;        // ��ʦʱ��
    int     nMentorValue;       // ʦͽֵ
    time_t  nEndTime;           // ״̬�Ľ�ֹʱ��
    BYTE    byState;            // ��ϵ�������
    BYTE    byOriginalALevel;   // ��ʦʱͽ�ܵĵȼ�
    BYTE    byReserved[18];
};

struct KMentorTableDB_Analyser
{
    int     nVersion;
    time_t  nCreateTime;        // ��ʦʱ��
    int     nMentorValue;       // ʦͽֵ
    time_t  nEndTime;           // ״̬�Ľ�ֹʱ��
    BYTE    byState;            // ��ϵ�������
    BYTE    byOriginalALevel;   // ��ʦʱͽ�ܵĵȼ�
    BYTE    byReserved[18];
};

#pragma pack()

struct KMentorInfo_Analyser
{
    time_t  nCreateTime;        // ��ʦʱ��
    int     nMentorValue;       // ʦͽֵ
    time_t  nEndTime;           // ״̬�Ľ�ֹʱ��
    BYTE    byState;            // ��ϵ�������
    BYTE    byOriginalALevel;   // ��ʦʱͽ�ܵĵȼ�    
    BOOL    bSaved;
};

class KMentorTableData
{
public:
    KMentorTableData(void);
    ~KMentorTableData(void);
public:
    int PaserMentorInfo(unsigned char* pbyData, size_t uDataLen);
public:
    KMentorTableDB_Analyser m_KMentorTableData;
};

#endif //KMENTOR_TABLE_DATA_H