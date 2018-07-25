///////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
// 
// FileName :   KMailBoxInfoData.h
// Creator  :   liuzhibiao
// Date     :   2009-7-10
// Comment  :   v1.0        �����ݿ��в�ѯ��ɫ�ʼ���Ϣ
///////////////////////////////////////////////////////////////

#ifndef KMAILBOXINFODATA_H
#define KMAILBOXINFODATA_H

#include "SO3GlobalDef.h"
#include "KGPublic.h"
#include "KMailDef.h"
#include "KItemDef.h"
#include <vector>
using namespace std;

#pragma	pack(1)

// ���ʼ���ʽ��Ҫ
#define MAIL_TITLE_LEN      64

struct KMAIL_CONTENT_OLD 
{
    int             nMoney;
    unsigned short  wTextSize;
    unsigned short  wItemSize;
    unsigned char   byTextAndItemData[0];
};
// ���ʼ���ʽ��Ҫend

struct KMAILBOX_DB_HEAD
{
    unsigned short  wVersion;
    unsigned long   dwNextMailID;
    unsigned long   dwLastGlobalMailIndex;
};

struct KMAIL_DB_OLD
{
    unsigned long       dwMailID;
    char                szSenderName[ROLE_NAME_LEN];
    char                szTitle[MAIL_TITLE_LEN];
    time_t              nReceiveTime;
    time_t              nGetAttachmentLock;
    unsigned char       byFlags;
    KMAIL_CONTENT_OLD   Content;
};

#define KMAIL_MAX_ITEM_COUNT 8
#define _NAME_LEN            32
// ����Ľṹ����Ķ�����Ϊ����ɰ汾���ݣ��°汾�����õ�

#define KMAIL_BIT_FLAG_UNREAD    0x01
#define KMAIL_BIT_FLAG_MONEY     0x02
#define KMAIL_BIT_FLAG_TEXT      0x04
#define KMAIL_BIT_FLAG_ITEM      0x08
#define KMAIL_BIT_FLAG_SYSTEM    0x10
struct KMAILDB_ITEM
{
    bool           bAcquired;   // true ��ʾ�Ѿ��ַ�������� 
    unsigned char  ucDataLen;
    int            nPrice;      // ��������ĵ��߼۸�
    int            nIsEquip;
    union
    {
        KCOMMON_ITEM_DATA commonEquipData;
        KCUSTOM_EQUI_DATA customEquipData;
        //            KTRAIN_EQUIP_DATA trainEquipData;
    };
};

struct KMail_DB_ContentOld
{
    int             nMoney;
    WORD            wTextLen;
    WORD            wItemLen;
    BYTE            byData[0];
};

struct KMail_DB_V3
{
    DWORD               dwMailID;
    char                szSenderName[ROLE_NAME_LEN];
    char                szTitle[MAIL_TITLE_LEN];
    time_t              nReceiveTime;
    time_t              nGetAttachmentLock;
    BYTE                byFlags;
    KMail_DB_ContentOld Content;
};

struct KMail_DB_V4      //  ��ʱ�����ݿ�����ת��֮��Ϳ���ɾ����
{
    DWORD           dwMailID;
    BYTE            byType;
    bool            bRead;
    char            szSenderName[ROLE_NAME_LEN];
    char            szTitle[MAIL_TITLE_LEN];
    time_t          nSendTime;
    time_t          nRecvTime;
    time_t          nLockTime;
    int             nMoney;
    WORD            wTextLen;
    KMailItemDesc   ItemDesc[KMAIL_MAX_ITEM_COUNT];
    BYTE            byData[0];
};

struct KMail_DB_V5
{
    DWORD           dwMailID;
    BYTE            byType;
    bool            bRead;
    DWORD           dwSenderID;
    char            szSenderName[ROLE_NAME_LEN];
    char            szTitle[MAIL_TITLE_LEN];
    time_t          nSendTime;
    time_t          nRecvTime;
    time_t          nLockTime;
    int             nMoney;
    WORD            wTextLen;
    KMailItemDesc   ItemDesc[KMAIL_MAX_ITEM_COUNT];
    BYTE            byData[0];
};

//struct KMAIL_DB_NEW
//{
//    unsigned long   ulMailID;
//    unsigned char   ucType;
//    bool            bRead;
//    char            szSenderName[_NAME_LEN];
//    char            szTitle[MAIL_TITLE_LEN];
//    time_t          nSendTime;
//    time_t          nRecvTime;
//    time_t          nLockTime;
//    int             nMoney;
//    unsigned short  uTextLen;
//    char            szText[512];
//    int             nItemCount;
//    KMAILDB_ITEM    MailItem[KMAIL_MAX_ITEM_COUNT];
//    //KMailItemDesc   ItemDesc[KMAIL_MAX_ITEM_COUNT];
//    //unsigned char   ucData[0];
//};

struct KMAIL_DB_NOW_USE
{
    unsigned long   ulMailID;
    unsigned char   ucType;
    bool            bRead;
    DWORD           dwSenderID;
    char            szSenderName[_NAME_LEN];
    char            szTitle[MAIL_TITLE_LEN];
    time_t          nSendTime;
    time_t          nRecvTime;
    time_t          nLockTime;
    int             nMoney;
    unsigned short  uTextLen;
    char            szText[512];
    int             nItemCount;
    KMAILDB_ITEM    MailItem[KMAIL_MAX_ITEM_COUNT];
    //KMailItemDesc   ItemDesc[KMAIL_MAX_ITEM_COUNT];
    //unsigned char   ucData[0];
};

#pragma	pack()

class KMailBoxInfoData
{
public:
    KMailBoxInfoData(void);
    ~KMailBoxInfoData(void);

public:
    int PaserMailBoxInfo(unsigned char* pbyData, size_t uDataLen);
    vector<KMAIL_DB_NOW_USE*> m_MailList;
    
    unsigned long m_ulNextMailID;
    unsigned long m_ulLastGlobalMailIndex;
    unsigned short m_usVersion;

private:
    int LoadVersion4(unsigned char* pbyData, size_t uDataLen);
    int LoadVersion3(unsigned char* pbyData, size_t uDataLen);
    int LoadVersion5(unsigned char* pbyData, size_t uDataLen);
};
#endif //KMAILBOXINFODATA_H
