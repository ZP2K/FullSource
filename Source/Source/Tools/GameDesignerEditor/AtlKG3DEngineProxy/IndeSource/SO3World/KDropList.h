/************************************************************************/
/* ���ߵ����							                                */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.02.28	Create												*/
/*		2005.12.14	Corrected item drop algorithm						*/
/************************************************************************/
#ifndef _KDROP_LIST_H_
#define _KDROP_LIST_H_

#include "KIndividualDropList.h"

class KItem;

class KDropList
{
public:
	KDropList(void);
	~KDropList(void);

	//��ʼ�������ñ�Ĵ�С
	BOOL Init(DWORD dwSize);

	//�趨���ߵ������������ţ��������ͣ����߱�ţ������ʣ�
	BOOL AddItem(DROP_DATA& DropData);

	//�������������
	BOOL PreProcess(void);

	//�������һ������
	KItem* GetRandomItem(DROP_DATA& DropData);

private:
	DROP_DATA*		m_ItemList;
	DWORD			m_dwSize;
	DWORD			m_dwIndex;
};

class KMoneyDropList
{
public:
    KMoneyDropList(void){};
    ~KMoneyDropList(void){};

	BOOL Init();

	BOOL AddMoney(DWORD dwMinMoney, DWORD dwMaxMoney);
	BOOL SetMoneyDropRate(DWORD dwMoneyDropRate);
	DWORD GetRandomMoney();
	BOOL IsDropInRandom();

private:
	DWORD			m_dwMinMoney;
	DWORD			m_dwMaxMoney;
	DWORD			m_dwMoneyDropRate;
};
#endif	//_KDROP_LIST_H_
