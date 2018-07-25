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

class KRegion;
class KScene;
class KMonster;

enum SCENE_OBJ_TYPE
{
	sotInvalid = 0,
	sotNpc,
	sotPlayer,
	sotDoodad,

	sotTotal
};

class KSceneObject : public KBaseObject
{
public:

	KSceneObject();
    KMonster *m_pOwner;
	virtual ~KSceneObject();
	
	BOOL	Init();
	BOOL	UnInit(void);

	BOOL	CheckGameLoop(void);

public:
	//ԭ��Ϊ����ǰRegion�еĵ����꣬Region���½�Ϊԭ��
	//2005.4.26��Ϊ����������ͼ�ϵ�ȫ�ֵ����꣬��ͼ�����½�Ϊԭ��
	//���������ʹ�ã�
	//m_nX������ȷ������������������ͼ�ϵ�λ�ã������ļ������紫����ʹ�õĶ����������
	//m_nXinCell�������жϰ���ϰ�,2005.07.26ȡ������ϰ�
	//m_nXCell�����ڿ��ٷ���Region�еĸ�������
	//2005.06.14	����ÿ�����ص���Ϊ32*32
	//2006.03.02	����ÿ�����ص���Ϊ16*16
	int			m_nX;				// ��ͼ�е�X����				��λ���㣨100/64cm��
	int			m_nY;				// ��ͼ�е�Y����				��λ����
	int			m_nZ;				// ��ͼ�е�Z���꣬��ر����Ը߶ȸ߶�		��λ����

#if defined(_EDITOR)
	int			m_nEditorZ;			// �༭���õ�Z���꣬��¼���Ը߶�
#endif	//_EDITOR

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
	__int64						m_nGameLoop;		// ��Ϸѭ��������Ҫ�����ж��ظ�����

#if defined(_SERVER) | defined(_EDITOR)
	int							m_nLastBroadcastFrame;
	int							m_nEnterRegionFrame[ALL_DIRECTION + 1];	
	int							m_nChangeDirection; //�ϴο�Region�ķ���
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

	void HostGetAbsoluteCoordinate(int& nX, int& nY, int& nZ) const;

	int LuaGetCell(Lua_State* L);
	int LuaGetScene(Lua_State* L);
    int LuaGetAbsoluteCoordinate(Lua_State* L);
};
#endif	//_SERVER