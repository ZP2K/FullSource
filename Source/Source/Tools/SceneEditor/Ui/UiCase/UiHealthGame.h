/*******************************************************************************
File        : UiHealthGame.h
Creator     : Fyt(Fan Zhanpeng)
create data : 11-21-2003(mm-dd-yyyy)
Description : ������Ϸ�Ҹ�
********************************************************************************/

#if !defined(AFX_UIHEALTHGAME_H__A667B514_B34E_4F6B_8E06_74065B61B5CB__INCLUDED_)
#define AFX_UIHEALTHGAME_H__A667B514_B34E_4F6B_8E06_74065B61B5CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndtext.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../Elem/WndShowAnimate.h"

class KUiHealthGame : public KWndShowAnimate
{
public:  //�����׼�Ķ���ӿ�
	KUiHealthGame();
	virtual ~KUiHealthGame();

	static        KUiHealthGame* OpenWindow();		//�򿪴���
	static        KUiHealthGame* GetIfVisible();	//�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE);//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);	//������淽��

private: //�����׼���ڲ�������
	void          Initialize();						//��ʼ������

													//���������Ϣ�Ĵ�����
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

private: //������Ҫ�ľ�̬����
	static KUiHealthGame*	ms_pSelf;

private:	//�ؼ������ݳ�Ա
	KWndButton				m_BtnClose;
	KWndText256				m_Text;
};

#endif // !defined(AFX_UIHEALTHGAME_H__A667B514_B34E_4F6B_8E06_74065B61B5CB__INCLUDED_)
