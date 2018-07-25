/*******************************************************************************
File        : UiTongAssignTitle.h
Creator     : Fyt(Fan Zhanpeng)
create data : 10-31-2003(mm-dd-yyyy)
Description : ������Ľ���
********************************************************************************/

#if !defined(AFX_UITONGASSIGNTITLE_H__2F4CCA33_DF93_4DED_AFFE_CB9535A861A6__INCLUDED_)
#define AFX_UITONGASSIGNTITLE_H__2F4CCA33_DF93_4DED_AFFE_CB9535A861A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndpuretextbtn.h"
#include "../elem/WndWindow.h"
#include "../elem/wndbutton.h"
#include "../elem/wndimage.h"
#include "../elem/wndedit.h"
#include "GameDataDef.h"

class KUiTongAssignTitle : public KWndImage
{
public:
	KUiTongAssignTitle();
	virtual ~KUiTongAssignTitle();

	static        KUiTongAssignTitle* OpenWindow();  //�򿪴���
	static        KUiTongAssignTitle* GetIfVisible();//�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE); //�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);   //������淽��

public:
	//���ں�������������������Ϣ
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

	//������ע�뵽���������
	void          InjectData(KTongMemberItem *pData, int nDataCount, int nFigure, int nSelect = 0);
	
	//���½�����ʾ����
	void          UpdateView();

private:
	void          Initialize();                      //��ʼ������
	void          PopupSelectMenu(int nX, int nY);   //����Ŀ�����ѡ���

private:
	enum RESULT
	{
		RESULT_SELECT_TARGET_MENU,
	};

private:
	static KUiTongAssignTitle* ms_pSelf;             //ָ���Լ���������ָ��

private:
	KWndPureTextBtn            m_BtnTarget;          //д�������ֵ��򣬿��Ե��ѡ��������
	KWndEdit32                 m_EditTitle;          //�ĳƺŵ�ʱ��д�ƺŵĵط�

	KWndButton                                       //ȷ����ȡ����ť
		                       m_BtnConfirm, m_BtnCancel;

private:
	KTongMemberItem*           m_pData;              //�������ݵ��ڴ��ָ�룬�ڴ���Ҫ����
	int                        m_nDataCount;         //���ݵ�����
	KTongMemberItem            m_SelectData;         //��ǰѡ��Ķ��������
	int                        m_nSelected;          //��ǰѡ������λ�ÿ�

	int                        m_nFigure;            //������һ��ְλ�ĳƺ�Ⱥ
};

#endif // !defined(AFX_UITONGASSIGNTITLE_H__2F4CCA33_DF93_4DED_AFFE_CB9535A861A6__INCLUDED_)
