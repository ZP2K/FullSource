#pragma once
//#include "KTimeLine4Editor.h"
//#pragma warning(disable:4244)
//class KTimeLineGrid
//{
//public:
//	KTimeLineGrid(const POINT& WindowSize) { m_ptSize = WindowSize; m_ptOrg.x = 10;	m_ptOrg.y = 200; m_fHScale = 1.0f; m_fVScale = 1.0f; }
//	KTimeLineGrid(int nWidth, int nHeight) { m_ptSize.x = nWidth; m_ptSize.y = nHeight; m_ptOrg.x = 10;	m_ptOrg.y = 200; m_fHScale = 1.0f; m_fVScale = 1.0f; }
//	~KTimeLineGrid(void);
//	
//	/*
//	*@brief ��ʾ����
//	*@param ʹ�õ�DC
//	*@param ��Ҫ��������, NULLΪȫ���ػ�
//	*/
//	void DrawGrid(CDC* pDC, RECT* pRect = NULL);
//		
//	/*
//	*@brief �趨��ǰ�����ű���
//	*@param nScale ����
//	*/
//	void SetScale(float fScale, enumScale Type) { Type == ENUM_SCALE_HORIZONTAL ? m_fHScale = fScale : m_fVScale = fScale; }
//
//	/*
//	*@brief �õ���ǰ�����ű���
//	*/
//	float GetScale(enumScale Type) { return (Type == ENUM_SCALE_HORIZONTAL ? m_fHScale : m_fVScale); }
//
//	/*
//	*@brief �õ���ǰ��ԭ��
//	*/
//	POINT GetOrigin() { return m_ptOrg;	}
//
//	/*
//	*@brief �趨ԭ��
//	*@param ptOrg�µ�ԭ��
//	*/
//	void SetOrigin(POINT& ptOrg){ m_ptOrg = ptOrg; }
//
//private:
//	//////////////////////////////////////////////////////////////////////////
//	//˽������
//	POINT m_ptOrg;//�������½����߼��ռ��е�λ��
//	float m_fHSpan;//ˮƽ�����ϵļ��
//	float m_fVSpan;//��ֱ�����ϵļ�࣬ʵ����ʾʱ���Ϊm_nXSpan * m_nScale
//	float m_fHScale;//ˮƽ�����ϵ�����ϵ��
//	float m_fVScale;//��ֱ�����ϵ�����ϵ��
//	bool m_bRedraw;//��ʾ�Ƿ���Ҫ�ػ�
//	POINT m_ptSize;//���ڵĳߴ�
//};
