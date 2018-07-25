/*******************************************************************************
File        : UiTrembleItem.h
Creator     : Fyt(Fan Zhanpeng)
create data : 09-08-2003(mm-dd-yyyy)
Description : ���������Ľ���
********************************************************************************/

#if !defined(AFX_UITREMBLEITEM_H__69079BE2_10C3_4AA0_A3C2_190285964E36__INCLUDED_)
#define AFX_UITREMBLEITEM_H__69079BE2_10C3_4AA0_A3C2_190285964E36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndobjcontainer.h"
#include "../elem/wndvalueimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndimage.h"

class KCanGetNumImage : public KWndValueImage
{
public:
	int GetMaxFrame();
	int GetCurrentFrame();
};

class KUiTrembleItem : public KWndImage
{
public:
	KUiTrembleItem();
	virtual ~KUiTrembleItem();

	static        KUiTrembleItem* OpenWindow();      //�򿪴���
	static        KUiTrembleItem* GetIfVisible();    //�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE); //�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);   //������淽��

	//���½�����ʾ
	void		  UpdateView(KUiObjAtRegion *pItem, int nbAdd);
	void		  Clear();

private:
	static        KUiTrembleItem *ms_pSelf;

private:
	void          Initialize();
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void  Breathe();

private:
	void          ApplyAssemble();                   //��Core���ʹ������������
	void          ResultReturn();                    //����������
	int           PlayEffect();                      //������Ч
	int           UpdateResult();                    //�ѽ�����µ�������
	void		  OnConfirm();						 //��Ӧȷ���Ĳ���
	void          OnPickDropItem(                    //��Ӧ��ʯ���������
		                         ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);

	//�������λ���Ƿ��Ѿ����ñ�ʯ
	int			  CheckIsOtherGemAlreadyPut(KWndWindow *pWhere);
	
	//�������ݺϷ��Լ��
	int			  CheckValid(KUiBesetOperationParam *pParam);
	void		  Lock(BOOL bLock);					 //�Ƿ���ס����(����Ӧ�κβ���)

private:
	enum THIS_INTERFACE_STATUS
	{
		STATUS_WAITING_MATERIALS,
		STATUS_BEGIN_TREMBLE,
		STATUS_PLAYING_EFFECT,
		STATUS_TREMBLING,
		STATUS_CHANGING_ITEM,
		STATUS_FINISH,
	};

private:
	KWndObjectBox m_Item[UITP_NUM];					 //��ʯ����Ʒ

	KWndButton    m_Confirm;                         //�ϳɰ�ť
	KWndButton    m_Cancel;                          //�رհ�ť

	KCanGetNumImage
		          m_TrembleEffect;                   //�ϳ��е���Ч

private:
	int           m_nStatus;                         //��ǰ����������״̬

	//����״̬���(���ָUI״̬������ָ��������״̬)
	int			  m_nStatusHandle;

private:
	char		  m_szErrorNoItem[64];				 //�����װ�� ����
	char		  m_szErrorNoGem[64];				 //���������һ�ű�ʯ ����
	char		  m_szErrorOnlyOne[64];				 //���б�ʯֻ�ܹ���һ�� ����
};

#endif // !defined(AFX_UITREMBLEITEM_H__69079BE2_10C3_4AA0_A3C2_190285964E36__INCLUDED_)
