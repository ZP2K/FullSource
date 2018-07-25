/*******************************************************************************
File        : UiEquipmentRepairAlert.h
Creator     : Fyt(Fan Zhanpeng)
create data : 10-15-2003(mm-dd-yyyy)
Description : ��Ʒ���������
********************************************************************************/


#if !defined(AFX_EQUIPMENTREPAIRALERT_H__DAAACD58_7AB6_4DCA_8DCA_09765EDB7CA3__INCLUDED_)
#define AFX_EQUIPMENTREPAIRALERT_H__DAAACD58_7AB6_4DCA_8DCA_09765EDB7CA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndimage.h"

class KUiEquipmentRepairAlert : public KWndImage
{
public:
	KUiEquipmentRepairAlert();
	virtual ~KUiEquipmentRepairAlert();

	static        KUiEquipmentRepairAlert* OpenWindow();  //�򿪴���
	static        KUiEquipmentRepairAlert* GetIfVisible();//�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE);      //�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);        //������淽��
	static        KUiEquipmentRepairAlert* GetIfCreate(); //��������Ѿ����������򷵻ض���ʵ�������򷵻�NULL

	virtual void  Show(int nDur, char* szName); //��ʾ����

	//����Ҫ������Ǹ�װ����Tip
	int			  SetTargetItemTip(char *szName, int nbIsAlmostCrash = 0);

private:
	static        KUiEquipmentRepairAlert *ms_pSelf;

private:
	void          Initialize();
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

private:
	int           m_nYellowAlertSignal;
	int           m_nRedAlertSignal;

	char		  m_szTip[64];
	int			  m_nItemIndex;
};

#endif // !defined(AFX_EQUIPMENTREPAIRALERT_H__DAAACD58_7AB6_4DCA_8DCA_09765EDB7CA3__INCLUDED_)
