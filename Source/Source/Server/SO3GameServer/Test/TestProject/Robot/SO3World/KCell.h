/************************************************************************/
/* ��ͼ�ϵĵ�Ԫ��								                        */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.01.12	Create												*/
/* Comment	 :															*/
/*		KCell��ʾ�ռ���һ������Ϊ0.5M*0.5M,�߶ȿ��趨�ĳ�����			*/
/*		��ӵ��������������,�ϱ����վ��,��˰������ϱ�����ϰ������Ϣ	*/
/************************************************************************/
#ifndef _KCELL_H_
#define _KCELL_H_
#include "Luna.h"
#include "Global.h"

#define MAX_CHARACTER_COUNT_ON_CELL	255
#define MAX_DOODAD_COUNT_ON_CELL	3

class KPlayer;
class KNpc;
class KDoodad;
class KMissile;

enum CELL_TYPE
{
	ctPlain = 0,
	ctWater,
    ctIce,
	ctTotal
};

#pragma pack(1)
struct KCell
{
	struct KBaseInfo
	{
		DWORD		dwScriptIndex		:	4;		// �ű�����
		DWORD		dwBlockCharacter	:	1;		// �ϰ���Ϣ
		DWORD		dwPutObj			:	1;		// �ɰڷ�		
		DWORD		dwRest				:	1;		// �Ƿ�����Ϣ��
		DWORD		dwStall			    :	1;		// �Ƿ�ɰ�̯
		DWORD		dwRideHorse		    :	1;		// �Ƿ�����
		DWORD		dwIndoor			:	1;		// �Ƿ�����
		DWORD		dwCellType			:	2;		// �ر�����		
		DWORD		dwPassWidth			:	3;		// ͨ��������
		DWORD		dwNoObstacleRange	:	6;		// ���ϰ���Χ
		DWORD		dwGradientDirection :   3;		// ��������,8������,�ϲ�ʱȡ���
		DWORD		dwGradientDegree    :	3;		// �¶�,��λ: 90��Բ����1/8,�ϲ�ʱȡ��ϵ�,����90��ȡ����
		DWORD		dwBarrierDirection  :	3;		// �ϰ�����,��λ: 180��Բ����1/8,�ϲ�ʱȡ��б����
		DWORD		dwFaceUp			:	1;		// �Ƿ������ϣ����ڱ༭ʱ�ϲ�CELL
		DWORD		dwReserve			:	1;		// ����
	} m_BaseInfo;
	WORD				m_wLowLayer;					// �²�߶�(�Ժ�ƽ��Ϊ��׼)
	WORD				m_wHighLayer;					// �ϲ�߶�(�Ժ�ƽ��Ϊ��׼)
	KCell*				m_pNext;						// ��Ƭ����

	BOOL AddNpc(KNpc* pNpc);
	BOOL AddDoodad(KDoodad* pDoodad);
	BOOL AddPlayer(KPlayer* pPlayer);

	BOOL RemoveNpc(KNpc* pNpc);
	BOOL RemoveDoodad(KDoodad* pDoodad);
	BOOL RemovePlayer(KPlayer* pPlayer);

    void SetGradientDegree(int nDegree)
    { 
        ASSERT(nDegree >= 0 && nDegree < DIRECTION_COUNT / 4);
        m_BaseInfo.dwGradientDegree = nDegree / (DIRECTION_COUNT / 4 / 8);
    }

    int GetGradientDegree()
    { 
        return m_BaseInfo.dwGradientDegree * DIRECTION_COUNT / 4 / 8; 
    }

    void SetGradientDirection(int nDirection)
    { 
        ASSERT(nDirection >= 0 && nDirection < DIRECTION_COUNT);
        m_BaseInfo.dwGradientDirection = nDirection / (DIRECTION_COUNT / 8);
    }

    int GetGradientDirection()
    { 
        return m_BaseInfo.dwGradientDirection * DIRECTION_COUNT / 8; 
    }

    void SetBarrierDirection(int nDirection)
    { 
        ASSERT(nDirection >= 0 && nDirection < DIRECTION_COUNT);
        if (nDirection >= DIRECTION_COUNT / 2)
        {
            nDirection -= DIRECTION_COUNT / 2;
        }
        m_BaseInfo.dwBarrierDirection = nDirection / (DIRECTION_COUNT / 2 / 8);
    }

    int GetBarrierDirection()
    { 
        return m_BaseInfo.dwBarrierDirection * DIRECTION_COUNT / 2 / 8; 
    }

public:
	//Lua�ű��ӿ�
	DECLARE_LUA_CLASS(KCell);

	DECLARE_LUA_STRUCT_INTEGER(IsBlockCharacter, m_BaseInfo.dwBlockCharacter);
	DECLARE_LUA_STRUCT_INTEGER(CanPutObj, m_BaseInfo.dwPutObj);
	DECLARE_LUA_STRUCT_INTEGER(IsSafe, m_BaseInfo.dwRest);
	DECLARE_LUA_STRUCT_INTEGER(CanStall, m_BaseInfo.dwStall);
	DECLARE_LUA_STRUCT_INTEGER(Type, m_BaseInfo.dwCellType);
};
#pragma pack()


#endif	//_KCELL_H_