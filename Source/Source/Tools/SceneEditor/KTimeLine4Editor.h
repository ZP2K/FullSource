#pragma once
//
//#include "ktimeline.h"
//#include <set>
//using namespace std;
//
//enum enumScale
//{
//	ENUM_SCALE_BEGIN,
//	ENUM_SCALE_HORIZONTAL,
//	ENUM_SCALE_VERTICAL,
//	ENUM_SCALE_COUNT,
//	ENUM_SCALE_FORCE_DWORD,
//};
//
////////////////////////////////////////////////////////////////////////////
////Ϊ�˱༭TimeLine����
//
//template<typename TimeType, typename FrameType>
//class KTimeLine4Editor : public KTimeLine<TimeType, FrameType>
//{
//public:
//	KTimeLine4Editor(void);
//	~KTimeLine4Editor(void);
//	//void DrawTimeLineCurve(CDC* pDC, COLORREF crColor, POINT& ptOrg, float fHorizontalScale, float fVerticalScale);
//	/*
//	*@brief �趨ԭ��
//	*@param ptOrg ԭ��
//	*/
//	void SetOrg(POINT& ptOrg){ memcpy(&m_ptOrg, &ptOrg, sizeof(POINT)); }
//	
//	/*
//	*@brief �õ���ǰԭ��
//	*@return ԭ��
//	*/
//	POINT GetOrg() { return m_ptOrg; }
//
//	/*
//	*@brief �趨���ű���
//	*@param fScale ����
//	*@param ScaleType ��������
//	*/
//	void SetScale( float fScale, enumScale ScaleType ){ ScaleType == ENUM_SCALE_HORIZONTAL ? m_fHorizontalScale = fScale : m_fVerticalScale = fScale; }
//	
//	/*
//	*@brief �õ���ǰ����
//	*@param ScaleType ��������
//	*@return ����
//	*/
//	float GetScale( enumScale ScaleType ) { return ScaleType == ENUM_SCALE_HORIZONTAL ? m_fHorizontalScale : m_fVerticalScale; }
//	
//	BOOL IsTimePointSelected(TimeType Time)
//	{
//		BOOL bReturn = FALSE;
//		if ( m_SelectedTimePoints.count(Time))
//			bReturn = TRUE;
//		return bReturn;
//	}
//
//	BOOL IsNeighbor(TimeType TimeSrc, TimeType TimeDest)
//	{
//		BOOL bReturn = false;
//		map<TimeType, FrameType>::iterator it = m_Frames.find(Time);
//		if ( it != m_Frames.end() )
//		{
//			
//		}
//	}
//
//	inline void ClearSelection();
//	void DrawTimeLineCurve(CDC* pDC, COLORREF crColor);
//	int SelectPointInRegion(RECT& rect, TimeType& First, BOOL bAddToList = TRUE);
//	BOOL SelectSinglePoint(POINT& pt, TimeType& Time, BOOL bAddToList = TRUE);
//	TimeType ClickSelection(POINT& pt);
//	BOOL IsActive(){ return m_bLineActived;}
//	void SetOperateKey(TimeType Time) { m_OperateKey = Time; }
//	TimeType GetOperateKey() { return m_OperateKey;}
//	void MoveTo(POINT& pt);
//	void Remove();
//	void Add(POINT& pt);
//	void Insert(POINT& pt);
//	void DrawInsertPoint(POINT& pt);
//	int GetNumSelectedKeys();
//private:
//	void DrawTimePoint(CDC* pDC, COLORREF crColor, POINT& ptPos);
//	
//	pair<TimeType, FrameType> m_InsertPoint;
//	BOOL m_bInsertPoint;
//	
//	POINT KeyToCoordinate(const TimeType& Time, const FrameType& Frame);
//	pair<TimeType, FrameType> CoordinateToKey(POINT& ptPos);
//	POINT m_ptOrg;//ԭ���λ��
//	float m_fHorizontalScale;//ˮƽ��������ű���
//	float m_fVerticalScale;//��ֱ��������ű���
//	static COLORREF m_crSelected;//��ѡ�е����߲��ֵ���ɫ
//	int m_nTimePointSize;//�ؼ���Ĵ�С
//	set<TimeType> m_SelectedTimePoints;
//	BOOL m_bLineActived;//�Ƿ񼤻�,��������һ���㱻ѡ�е�ʱ��ΪTRUE
//	TimeType m_OperateKey;//��Ӧ�����Ĺؼ���
//};
//
//
//template<typename TimeType, typename FrameType>
//COLORREF KTimeLine4Editor<TimeType, FrameType>::m_crSelected = 0x00ff00ff;
//
//template<typename TimeType, typename FrameType>
//int KTimeLine4Editor<TimeType, FrameType>::GetNumSelectedKeys()
//{
//	return static_cast<int>(m_SelectedTimePoints.size());
//}
///*
//*@brief �������λ���ƶ��ؼ���
//*@param pt ����λ��
//*/
//template<typename TimeType, typename FrameType>
//void KTimeLine4Editor<TimeType, FrameType>::MoveTo(POINT& pt)
//{
//	pair<TimeType, FrameType> KeyAndValue = CoordinateToKey(pt);
//	while ( ValidateKey(KeyAndValue.first) )
//	{
//		KeyAndValue.first += 0.0001f;
//	}
//	RemoveFrame(m_OperateKey);
//	m_SelectedTimePoints.insert(KeyAndValue.first);
//	m_SelectedTimePoints.erase(m_OperateKey);
//	AddFrame(KeyAndValue.first, KeyAndValue.second);
//	m_OperateKey = KeyAndValue.first;
//}
//
//template<typename TimeType, typename FrameType>
//void KTimeLine4Editor<TimeType, FrameType>::DrawInsertPoint(POINT& pt)
//{
//	m_InsertPoint = CoordinateToKey(pt);
//	GetCurrentFrame(m_InsertPoint.first, m_InsertPoint.second);
//	m_bInsertPoint = TRUE;
//}
//
//template<typename TimeType, typename FrameType>
//void KTimeLine4Editor<TimeType, FrameType>::Add(POINT& pt)
//{
//	pair<TimeType, FrameType> KeyAndValue = CoordinateToKey(pt);
//	AddFrame(KeyAndValue.first, KeyAndValue.second);
//	m_OperateKey = KeyAndValue.first;
//}
//
//template<typename TimeType, typename FrameType>
//void KTimeLine4Editor<TimeType, FrameType>::Insert(POINT& pt)
//{
//	m_bInsertPoint = FALSE;
//	pair<TimeType, FrameType> KeyAndValue = CoordinateToKey(pt);
//	GetCurrentFrame(KeyAndValue.first, KeyAndValue.second);
//	AddFrame(KeyAndValue.first, KeyAndValue.second);
//	m_OperateKey = KeyAndValue.first;
//}
//
///*
//*@brief ɾ������ѡ�еĹؼ���
//*/
//template<typename TimeType, typename FrameType>
//void KTimeLine4Editor<TimeType, FrameType>::Remove()
//{
//	set<TimeType>::iterator it  = m_SelectedTimePoints.begin();
//	while ( it != m_SelectedTimePoints.end() )
//	{
//		TimeType i = *it;
//		RemoveFrame(*it);
//		i = *it;
//		it++;
//	}
//	m_OperateKey = -9999999.9f;
//	ClearSelection();
//}
//
//
///*
//*@brief �Ƿ�ѡ���˹ؼ���
//*@param pt    ����ѡ��λ��
//*@return �Ƿ�ѡ��, TRUE(ѡ��)
//*/
//template<typename TimeType, typename FrameType>
//BOOL KTimeLine4Editor<TimeType, FrameType>::SelectSinglePoint(POINT& pt, TimeType& Time, BOOL bAddToList /*= TRUE */)
//{
//	BOOL bReturn = FALSE;
//	int nXBounce = 4;//4�����ص� / ˮƽ����ϵ��
//	int nYBounce = 4;
//	RECT rect = { max(pt.x - nXBounce, 0), max(0, pt.y - nYBounce), pt.x + nXBounce, pt.y + nYBounce };
//	bReturn = SelectPointInRegion(rect, Time, bAddToList);
//	return bReturn;
//}
//
//template<typename TimeType, typename FrameType>
//KTimeLine4Editor<TimeType, FrameType>::KTimeLine4Editor(void): m_fVerticalScale(2.0f), m_fHorizontalScale(3.0f)
//{
//	m_ptOrg.x = 10;
//	m_ptOrg.y = 200;
//	m_fVerticalScale = 1.0f;
//	m_fHorizontalScale = 1.0f;
//	m_nTimePointSize = 2;
//	m_bInsertPoint = FALSE;
//	m_bLineActived = FALSE;
//	m_OperateKey = -9999999.9f;
//}
//
///*
//*@brief ��RECT�����ڵĹؼ������ѡ���б�
//*@param rect ������Ļ���꣩
//*@return ��ѡ�еĵ�ĸ���
//*/
//template<typename TimeType, typename FrameType>
//int KTimeLine4Editor<TimeType, FrameType>::SelectPointInRegion(RECT& rect, TimeType& First, BOOL bAddToList /*= TRUE*/)
//{
//	m_bLineActived = FALSE;
//	int nReturn = 0;
//	POINT pt1 = { min(rect.left, rect.right), max(rect.top, rect.bottom) };
//	POINT pt2 = { max(rect.right, rect.left), min(rect.bottom, rect.top) };
//	pair<TimeType, FrameType> KeyAndValue1 = CoordinateToKey(pt1);
//	pair<TimeType, FrameType> KeyAndValue2 = CoordinateToKey(pt2);
//	map<TimeType, FrameType>::iterator it = m_Frames.lower_bound(KeyAndValue1.first);
//	map<TimeType, FrameType>::iterator itEnd = m_Frames.lower_bound(KeyAndValue2.first);
//	for ( ; it != itEnd; it++ ) 
//	{
//		if ( it->second >= KeyAndValue1.second && it->second <= KeyAndValue2.second )
//		{
//			if ( !nReturn )
//			{
//				First = it->first;
//				m_OperateKey = First;
//			}
//			nReturn++;
//			if ( bAddToList )
//				m_SelectedTimePoints.insert(it->first);
//			m_bLineActived = TRUE;
//		}
//	}
//	return nReturn;
//}
//
///*
//*@brief ���ѡ�йؼ����б�
//*/
//template<typename TimeType, typename FrameType>
//inline void KTimeLine4Editor<TimeType, FrameType>::ClearSelection()
//{
//	m_SelectedTimePoints.clear();
//}
//
///*
//*@brief �����ѡ�ؼ��㣬���û�е㱻ѡ�У������������ѡ�еĵ�
//*@param rect ������Ļ���꣩
//*/
//template<typename TimeType, typename FrameType>
//TimeType KTimeLine4Editor<TimeType, FrameType>::ClickSelection(POINT& pt)
//{
//	TimeType ReturnTime;
//	ClearSelection();
//	SelectSinglePoint(pt, ReturnTime);
//	return ReturnTime;
//}
//
//
//template<typename TimeType, typename FrameType>
//KTimeLine4Editor<TimeType, FrameType>::~KTimeLine4Editor(void)
//{
//	
//}
//
//
///*
//*@brief ��ָ���ģģ�����ʾ�ԣ���̣���
//*@param pDC ���ڵ�DC
//*@param crColor ��ɫ
//*/
//template<typename TimeType, typename FrameType>
//void KTimeLine4Editor<TimeType, FrameType>::DrawTimeLineCurve(CDC* pDC, COLORREF crColor)
//{
//	if ( m_Frames.size() )//If there is key frame exist
//	{
//		CPen Pen(PS_SOLID, 1, crColor);//For normal keys
//		static CPen PenSelected(PS_SOLID, 1, m_crSelected);//For selected keys
//		CPen* pOldPen = pDC->SelectObject(&PenSelected);
//		map<TimeType, FrameType>::iterator it = m_Frames.begin();
//		POINT ptPos = KeyToCoordinate(it->first, it->second);
//		BOOL bSelected = IsTimePointSelected(it->first);
//		COLORREF crTimePointColor = bSelected ? m_crSelected : crColor;
//		if ( GetNumSelectedKeys() && GetOperateKey() == it->first )//Operation key
//			crTimePointColor = RGB(255, 0, 255);
//		DrawTimePoint(pDC, crTimePointColor, ptPos);
//		it++;
//		BOOL bLastSelected = bSelected;
//		while( it != m_Frames.end() )
//		{
//			bSelected = IsTimePointSelected(it->first);
//			( bSelected | bLastSelected) ? pDC->SelectObject(PenSelected) : pDC->SelectObject(Pen);
//			POINT ptPos1 = KeyToCoordinate(it->first, it->second);
//			
//			if ( m_InterpolateType != ENUM_INTERPOLATE_NONE )//Only linear currently
//			{
//				pDC->MoveTo(ptPos);
//				pDC->LineTo(ptPos1);
//			}
//			else//No Interpolating
//			{
//				POINT ptTemp = { ptPos1.x, ptPos.y };
//				pDC->MoveTo(ptPos);
//				pDC->LineTo(ptTemp);
//				pDC->LineTo(ptPos1);
//			}
//			
//			crTimePointColor = bSelected ? m_crSelected : crColor;
//			if (GetNumSelectedKeys() && GetOperateKey() == it->first )
//			{
//				crTimePointColor = RGB(255, 0, 255);
//			}
//			DrawTimePoint(pDC, crTimePointColor, ptPos1);
//			bLastSelected = bSelected;
//			ptPos = ptPos1;
//			it++;
//		}
//		//Draw Insert Point here
//		if ( m_bInsertPoint )
//		{
//			HGDIOBJ pOldBrush = pDC->SelectObject(GetStockObject(HOLLOW_BRUSH));
//			POINT pt = KeyToCoordinate(m_InsertPoint.first, m_InsertPoint.second);
//			RECT rectInsertPoint = { pt.x - 4, pt.y - 4, pt.x + 4, pt.y + 4 };
//			CPen Pen(PS_SOLID, 1, RGB(0, 0, 0));
//			pDC->SelectObject(&Pen);
//			pDC->Rectangle(&rectInsertPoint);
//			pDC->SelectObject(pOldBrush);
//		}
//		pDC->SelectObject(pOldPen);
//		
//	}
//}
//
///*
//*@brief ��ʾ�ؼ���
//*@param pDC ���ڵ�DC
//*@param crColor ��ɫ
//*@param ptPos λ��
//*/
//template<typename TimeType, typename FrameType>
//void KTimeLine4Editor<TimeType, FrameType>::DrawTimePoint(CDC* pDC, COLORREF crColor, POINT& ptPos)
//{
//	CBrush Brush(crColor);
//	CPen Pen(PS_SOLID, 1, crColor);
//	CBrush* pOldBrush = pDC->SelectObject(&Brush);
//	CPen* pOldPen = pDC->SelectObject(&Pen);
//	RECT rectTimePoint = { ptPos.x - m_nTimePointSize, ptPos.y - m_nTimePointSize, ptPos.x + m_nTimePointSize, ptPos.y + m_nTimePointSize };
//	pDC->Rectangle(&rectTimePoint);
//	pDC->SelectObject(pOldPen);
//	pDC->SelectObject(pOldBrush);
//}
//
///*
//*@brief ��ֵת����Ϊλ����Ϣ
//*@param KeyAndValue ������Ӧ��ֵ
//*@return λ����Ϣ
//*/
//template<typename TimeType, typename FrameType>
//POINT KTimeLine4Editor<TimeType, FrameType>::KeyToCoordinate(const TimeType& Time, const FrameType& Frame)
//{
//	POINT ptPos = { static_cast<LONG>(Time * m_fHorizontalScale + m_ptOrg.x), static_cast<LONG>(m_ptOrg.y - Frame * m_fVerticalScale) };
//    return ptPos;
//}
//
///*
//*@brief ��λ����Ϣת����Ϊֵ
//*@param ptPos λ�� 
//*@return ������Ӧ��ֵ
//*/
//template<typename TimeType, typename FrameType>
//pair<TimeType, FrameType> KTimeLine4Editor<TimeType, FrameType>::CoordinateToKey(POINT& ptPos)
//{
//	pair<TimeType, FrameType> KeyAndValue;
//	KeyAndValue.first = (ptPos.x - m_ptOrg.x) / m_fHorizontalScale;
//	KeyAndValue.second = (m_ptOrg.y - ptPos.y) / m_fVerticalScale;
//	return KeyAndValue;
//}
//
