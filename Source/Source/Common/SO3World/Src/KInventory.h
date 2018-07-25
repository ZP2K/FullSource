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
    BOOL	Init(DWORD dwSize);
    void    UnInit();
    
    DWORD   GetSize() { return m_dwSize; }
	KItem*	GetItem(DWORD dwX);
	BOOL	PlaceItem(KItem* pItem, DWORD dwX);
	KItem*	PickUpItem(DWORD dwX);

    void    SetContainItemType(int nContainItemGenerType, int nContainItemSubType);
    void    GetContainItemType(int* pnContainItemGenerType, int* pnContainItemSubType);
    BOOL    CheckContainItemType(int nItemGenerType, int nItemSubType);
    BOOL    CheckContainItemType(KItem* pItem);

private:
	DWORD	m_dwSize;
	KItem*	m_pItemArray[MAX_ITEM_PACKAGE_SIZE];
	int		m_nContainItemGenerType;				// �������ɵ���Ʒ����
    int		m_nContainItemSubType;                  // �������ɵ���ƷС��
};
#endif
