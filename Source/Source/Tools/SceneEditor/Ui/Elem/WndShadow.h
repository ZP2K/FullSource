/*****************************************************************************************
//	��Ӱ����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-5
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "WndWindow.h"

class KWndShadow : public KWndWindow
{
public:
	virtual int	Init(KIniFile* pIniFile, const char* pSection);//��ʼ��
	virtual int		PtInWindow(int x, int y);	//�ж�һ�����Ƿ��ڴ��ڷ�Χ��,������Ǿ�������
	void	SetColor(unsigned int uColor);
	void	Clone(KWndShadow* pCopy);
	//�����Ƿ���ƽ������һ���жϵ��Ƿ��ڴ�����
	void	SetExactWnd(BOOL bExactWnd) { m_bExactWnd = bExactWnd; }
	KWndShadow() { m_uColor = 0; m_bExactWnd = FALSE; }
protected:
	void	PaintWindow();
private:
	unsigned int	m_uColor;
	BOOL			m_bExactWnd;	//�Ƿ���ƽ������һ���жϵ��Ƿ��ڴ�����,
									//������ǣ���˴�����ȫ��͸
};
