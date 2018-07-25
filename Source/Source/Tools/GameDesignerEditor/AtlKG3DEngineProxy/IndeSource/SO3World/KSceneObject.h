/************************************************************************/
/* ��������			                                                    */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.30	Create												*/
/* Comment	 :															*/
/*		�����ڵ�ͼ����ʾ�����嶼�ǳ�������								*/
/************************************************************************/
#ifndef _KSCENEOBJECT_H_
#define	_KSCENEOBJECT_H_

#include "Global.h"
#include "KBaseObject.h"
#include "KSceneObjNode.h"
#include "KCell.h"
#include "Luna.h"
#include "KMath.h"

class KRegion;
class KScene;

class KSceneObject : public KBaseObject
{
public:
	KSceneObject();
	virtual ~KSceneObject();
	
	BOOL	Init();
	void	UnInit();

	BOOL	CheckGameLoop();

	BOOL    GetAbsoluteCoordinate(int* pnX, int* pnY, int* pnZ);

    virtual void SetCell(KCell* pDstCell);

public:
	int			m_nX;
	int			m_nY;
	int			m_nZ;

    int         m_nTouchRange;      // �ܻ���Χ��������Χ������뾶

	// ��ǰRegion�еĸ������꣬Region���½�Ϊԭ��
	int			m_nXCell;			// Region�еĸ���X����		��λ������
	int			m_nYCell;			// Region�еĸ���Y����		��λ������

	int			m_nMass;			// ����						��λ��g
	
	int			m_nWidth;			// ��ȣ�X���ϵ�				��λ����
	int			m_nLength;			// ���ȣ�Y���ϵ�				��λ����
	int			m_nHeight;			// ��ȣ�Z���ϵ�				��λ����
	
	int			m_nFaceDirection;	// ��������ĳ���,���ٶ��޹�	��λ: (360/256)��
    int         m_nPitchDirection;  // ������,��λ(360/256)��

public:
	int						    m_nGameLoop;		// ��Ϸѭ��������Ҫ�����ж��ظ�����

#ifdef _CLIENT
	void*                       m_pRepresentObject;	// �����߼�����
#endif

	KCell*						m_pCell;			// ������Cell
	KRegion*					m_pRegion;			// ������Region
	KScene*						m_pScene;			// �����ĵ�ͼ
	KSceneObjNode				m_RegionObjNode;	// ����ά��KRegion�еĳ�����������

public:
	DECLARE_LUA_INTEGER(X);
	DECLARE_LUA_INTEGER(Y);
	DECLARE_LUA_INTEGER(Z);
    DECLARE_LUA_INTEGER(TouchRange);
	DECLARE_LUA_INTEGER(XCell);
	DECLARE_LUA_INTEGER(YCell);
	DECLARE_LUA_INTEGER(Mass);
	DECLARE_LUA_INTEGER(Width);
	DECLARE_LUA_INTEGER(Length);
	DECLARE_LUA_INTEGER(Height);
	DECLARE_LUA_INTEGER(FaceDirection);
    DECLARE_LUA_INTEGER(PitchDirection);

	int LuaGetCell(Lua_State* L);
	int LuaGetScene(Lua_State* L);
    int LuaGetAbsoluteCoordinate(Lua_State* L);
};

inline BOOL g_InRange(KSceneObject* pObjA, KSceneObject* pObjB, int nRange)
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;
    int     nXa         = 0;
    int     nYa         = 0;
    int     nZa         = 0;
    int     nXb         = 0;
    int     nYb         = 0;
    int     nZb         = 0;

    assert(pObjA);
    assert(pObjB);

    KG_PROCESS_ERROR(pObjA->m_pScene);
    KG_PROCESS_ERROR(pObjB->m_pScene);

    KG_PROCESS_ERROR(pObjA->m_pScene == pObjB->m_pScene);

    bRetCode = pObjA->GetAbsoluteCoordinate(&nXa, &nYa, &nZa);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = pObjB->GetAbsoluteCoordinate(&nXb, &nYb, &nZb);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = g_InRange(nXa, nYa, ZPOINT_TO_XYPOINT(nZa), nXb, nYb, ZPOINT_TO_XYPOINT(nZb), nRange);
    KG_PROCESS_ERROR(bRetCode);

    bResult = true;
Exit0:
    return bResult;
}

inline IN_RANGE_RESULT g_InRange(KSceneObject* pObjA, KSceneObject* pObjB, int nMinRange, int nMaxRange)
{
    IN_RANGE_RESULT nResult     = irrInvalid;
    BOOL            bRetCode    = false;
    int             nXa         = 0;
    int             nYa         = 0;
    int             nZa         = 0;
    int             nXb         = 0;
    int             nYb         = 0;
    int             nZb         = 0;

    assert(pObjA);
    assert(pObjB);

    KG_PROCESS_ERROR(pObjA->m_pScene);
    KG_PROCESS_ERROR(pObjB->m_pScene);

    KG_PROCESS_ERROR(pObjA->m_pScene == pObjB->m_pScene);

    bRetCode = pObjA->GetAbsoluteCoordinate(&nXa, &nYa, &nZa);
    KG_PROCESS_ERROR(bRetCode);

    bRetCode = pObjB->GetAbsoluteCoordinate(&nXb, &nYb, &nZb);
    KG_PROCESS_ERROR(bRetCode);

    nResult = g_InRange(nXa, nYa, ZPOINT_TO_XYPOINT(nZa), nXb, nYb, ZPOINT_TO_XYPOINT(nZb), nMinRange, nMaxRange);

Exit0:
    return nResult;
}

#endif	// _KSCENEOBJECT_H_
