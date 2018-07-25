#ifndef _KAUCTION_DEF_H_
#define _KAUCTION_DEF_H_

#define SALE_NAME_LEN          64
#define SYNC_PAGE_MAX_COUNT    50

enum KAUCTION_SALE_STATE
{
    assIgnore,
    assSomeoneBid,  // ���˳���
    assNoOneBid,    // ���˳���
};

enum KAUCTION_ORDER_TYPE
{
    aotQuality,
    aotLevel,
    aotLeftTime,
    aotPrice,
    aotBuyItNowPrice,

    aotTotal
};

enum KAUCTION_RESULT_CODE
{
    arcSucceed,
    arcItemNotExist,    // ��Ʒ������
    arcPriceTooLow,     // ����̫��
    arcCannotCancel,    // ���˳��ۣ�����ȡ��
    arcServerBusy,      // ������æ���ܾ���ѯ
    arcMailBoxFull,     // �ʼ�̫�࣬�ܾ�����

    arcUnknownError
};

enum KAUCTION_MESSAGE_CODE
{
    amcAcquireItem,
    amcBidLost,
    amcSomeoneBid,
    amcItemSold,
    amcTimeOver,

    amcTotalMessageCode
};

#pragma pack(1)
struct KAUCTION_LOOKUP_PARAM
{
    char    szSaleName[SALE_NAME_LEN];
    int     nAucGenre;
    int     nAucSub;
    int     nLevelMin;
    int     nLevelMax;
    int     nQuality;
    char    szSellerName[ROLE_NAME_LEN];
    DWORD   dwSellerID;
    DWORD   dwBidderID;
    int     nTopPrice;
    BYTE    byAuction;
    int     nStartIndex;
    BYTE    byOrderType;
    BYTE    byDesc;
};

struct KAUCTION_PAGE_NODE
{
    DWORD   dwSaleID;
    DWORD   dwSellerID;
    DWORD   dwBidderID;
    char    szSellerName[ROLE_NAME_LEN];
    char    szBidderName[ROLE_NAME_LEN];
    int     nPrice;
    int     nBuyItNowPrice;
    int     nLeftTime;
    size_t  uItemSize;
    BYTE    byItemData[0];
};

struct KAUCTION_PAGE_HEAD 
{
    int     nTotalCount;
    size_t  uSize;
    BYTE    bySaleList[0];
};
#pragma pack()

#endif // _KAUCTION_DEF_H_
