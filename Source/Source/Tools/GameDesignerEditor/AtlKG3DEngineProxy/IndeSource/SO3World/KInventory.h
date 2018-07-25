#ifndef _KINVENTORY_H_
#define	_KINVENTORY_H_

class KItem;

#define		MAX_ITEM_PACKAGE_SIZE		32
#define		FIRST_PACKAGE_SIZE			20
#define		MAX_SOLDLIST_PACKAGE_SIZE	10
#define     INVALID_CONTAIN_ITEM_TYPE   -1

class KInventory
{
	friend class KItemList;
public:
    BOOL	Init(DWORD dwSize, int nBoxType);
	KItem*	GetItem(DWORD dwX);
    DWORD   GetItemPosByItemID(DWORD dwItemID);
	BOOL	PlaceItem(KItem* pItem, DWORD dwX);
	KItem*	PickUpItem(DWORD dwX);
	int		FindFreePlace();
	int		FindFreeSoldList(int& rnDelPos);		// �ҵ������´�����Ʒ��λ�ã�����ж���˵�����ˣ������ԭ����Ʒ
	
    int     GetSoldListHead();
	int		GetUsedCount() { return m_dwUsed; }
    int     GetBoxType() { return m_nBoxType; }
    DWORD   GetSize() { return m_dwSize; }
    void    SetContainItemType(int nContainItemGenerType, int nContainItemSubType);
    void    GetContainItemType(int* pnContainItemGenerType, int* pnContainItemSubType);
    BOOL    CheckContainItemType(int nItemGenerType, int nItemSubType);
    BOOL    CheckContainItemType(KItem* pItem);

private:
	int		m_nBoxType;								// �ռ�����
	DWORD	m_dwSize;
	DWORD	m_dwUsed;
	KItem*	m_pItemArray[MAX_ITEM_PACKAGE_SIZE];
	int		m_nSoldListHead;						// �����ر��е�ǰ��һ����������Ʒ��λ��
	int		m_nContainItemGenerType;				// �������ɵ���Ʒ����
    int		m_nContainItemSubType;                  // �������ɵ���ƷС��
};
#endif
