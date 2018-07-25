/* 
 * File:     UiStall.h
 * Desc:     �໰�̯ing
 * Author:   flying
 * Creation: 2003/10/23
 */
#if !defined _UISTALL_HEADER
#define _UISTALL_HEADER

#pragma once

#include "../elem/WndButton.h"
#include "../elem/WndText.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"

struct KUiObjAtRegion;

class KUiStall : public KWndShowAnimate
{
public:
	static KUiStall* OpenWindow();				//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiStall* GetIfVisible();				//�������������ʾ���򷵻�ʵ��ָ��
	static void		CloseWindow(bool bDestroy);	//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void		LoadScheme(const char* pScheme);//������淽��
	void			RemoveItem(KUiObjAtRegion* pItem);//��Ʒ�仯����
	void			Breathe();									//�����

private:
	KUiStall();
	virtual ~KUiStall() {}
	void	Initialize();							//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void	UpdateData();
	void	OnClickItem(KUiDraggedObject* pItem);
private:
	static	KUiStall*	m_pSelf;
private:
	KWndButton			m_CloseBtn;
	KWndObjectMatrix	m_ItemBox;
	KWndText256			m_Owner;

	//����״̬���
    int					m_nStatusHandle;
};

#endif