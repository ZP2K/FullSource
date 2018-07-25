#ifndef _KINVENTORY_H_
#define	_KINVENTORY_H_

class KItem;

#define		MAX_ITEM_PACKAGE_SIZE		32
#define		FIRST_PACKAGE_SIZE			20
#define		MAX_SOLDLIST_PACKAGE_SIZE	10

class KInventory
{
	friend class KItemList;
private:
	BOOL	m_bTakeWith;							// �Ƿ�������Я���İ�
	int		m_nBoxType;								// �ռ�����
	DWORD	m_dwSize;
	DWORD	m_dwUsed;
	KItem*	m_pItemArray[MAX_ITEM_PACKAGE_SIZE];
	union	// �ñ������ڲ�ͬ�ı������ͣ����Ա�ʾ��ͬ�ĺ���
	{
		int		m_nSoldListHead;						// �����ر��е�ǰ��һ����������Ʒ��λ��
		int		m_nContainItemType;						// �������ɵ���Ʒ����
	};
public:
	KInventory();
	~KInventory();
	BOOL	Init(DWORD dwSize, int nBoxType, BOOL bTakeWith);
	KItem*	GetItem(DWORD dwX);
	BOOL	PlaceItem(KItem* pItem, DWORD dwX);
	KItem*	PickUpItem(DWORD dwX);
	int		FindFreePlace();
	int		FindFreeSoldList(int& pDelPos);		// �ҵ������´�����Ʒ��λ�ã�����ж���˵�����ˣ������ԭ����Ʒ
	BOOL	IsOpened();
	void	Open();
	void	Close();
    int     GetSoldListHead();
	int		GetFreeCount();
	int		GetUsedCount() { return m_dwUsed; }
};
#endif