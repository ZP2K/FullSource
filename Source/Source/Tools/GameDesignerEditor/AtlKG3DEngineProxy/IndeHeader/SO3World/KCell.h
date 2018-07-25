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

#define  MAX_PARTITION_INDEX_BIT    12

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

struct KCellBaseInfo
{
	DWORD		dwCellType			:	2;		// �ر�����		
	DWORD		dwGradientDirection :   3;		// ��������,8������,�ϲ�ʱȡ���
    DWORD		dwGradientDegree    :	3;		// �¶�,��λ: 90��Բ����1/8,�ϲ�ʱȡ��ϵ�,����90��ȡ����
	DWORD		dwBarrierDirection  :	3;		// �ϰ�����,��λ: 180��Բ����1/8,�ϲ�ʱȡ��б����
    DWORD		dwDynamic			:	1;		// �Ƿ�̬Cell
	DWORD		dwFaceUp			:	1;		// �Ƿ������ϣ����ڱ༭ʱ�ϲ�CELL
    DWORD       dwNavigationIndex   :   7;      // ��������
    DWORD       dwPartitionIndex    :   MAX_PARTITION_INDEX_BIT;     // ������������, 4096������
};

struct KCell
{
    struct KBaseInfo
	{
		DWORD		dwCellType			:	2;		// �ر�����		
		DWORD		dwIndoor			:	1;		// �Ƿ�����
		DWORD		dwPassWidth			:	3;		// ͨ��������
		DWORD		dwGradientDirection :   3;		// ��������,8������,�ϲ�ʱȡ���
        DWORD		dwGradientDegree    :	3;		// �¶�,��λ: 90��Բ����1/8,�ϲ�ʱȡ��ϵ�,����90��ȡ����
		DWORD		dwBarrierDirection  :	3;		// �ϰ�����,��λ: 180��Բ����1/8,�ϲ�ʱȡ��б����
		DWORD		dwFaceUp			:	1;		// �Ƿ������ϣ����ڱ༭ʱ�ϲ�CELL
        DWORD		dwDynamic			:	1;		// �Ƿ�̬Cell

        DWORD		dwNoObstacleRange	:	6;		// ���ϰ���Χ
        DWORD		dwScriptIndex		:	4;		// �ű�����
		DWORD		dwPutObj			:	1;		// �ɰڷ�	
		DWORD		dwRest				:	1;		// �Ƿ�����Ϣ��
		DWORD		dwStall			    :	1;		// �Ƿ�ɰ�̯
        DWORD		dwRideHorse		    :	1;		// �Ƿ�����
		DWORD		dwBlockCharacter	:	1;		// �ϰ���Ϣ
    } m_BaseInfo;

	WORD    m_wLowLayer;					// �²�߶�(�Ժ�ƽ��Ϊ��׼)
	WORD    m_wHighLayer;					// �ϲ�߶�(�Ժ�ƽ��Ϊ��׼)
	KCell*  m_pNext;						// ��Ƭ����

    inline void SetGradientDegree(int nDegree)
    { 
        assert(nDegree >= 0 && nDegree < DIRECTION_COUNT / 4);
        m_BaseInfo.dwGradientDegree = nDegree / (DIRECTION_COUNT / 4 / 8);
    }

    inline int GetGradientDegree()
    { 
        return m_BaseInfo.dwGradientDegree * DIRECTION_COUNT / 4 / 8; 
    }

    inline void SetGradientDirection(int nDirection)
    { 
        assert(nDirection >= 0 && nDirection < DIRECTION_COUNT);
        m_BaseInfo.dwGradientDirection = nDirection / (DIRECTION_COUNT / 8);
    }

    inline int GetGradientDirection()
    { 
        return m_BaseInfo.dwGradientDirection * DIRECTION_COUNT / 8; 
    }

    inline void SetBarrierDirection(int nDirection)
    { 
        assert(nDirection >= 0 && nDirection < DIRECTION_COUNT);
        if (nDirection >= DIRECTION_COUNT / 2)
        {
            nDirection -= DIRECTION_COUNT / 2;
        }
        m_BaseInfo.dwBarrierDirection = nDirection / (DIRECTION_COUNT / 2 / 8);
    }

    inline int GetBarrierDirection()
    { 
        return m_BaseInfo.dwBarrierDirection * DIRECTION_COUNT / 2 / 8; 
    }

    inline void SetWaterFlowSpeed(int nSpeed)
    {
    }

    inline int GetWaterFlowSpeed()
    {
        return 0;
    }

    inline void SetWaterFlowDirection(int nDirection)
    {
    }

    inline int GetWaterFlowDirection()
    {
        return 0;
    }

    inline int GetStaticFrictionCoefficient()
    {
        assert(false);
        return 0;
    }

    inline int GetDynamicFrictionCoefficient()
    {
        const int nDynamicFrictionCoefficients[ctTotal] = {768, 0, 128};
        return nDynamicFrictionCoefficients[m_BaseInfo.dwCellType];
    }

    inline int GetCriticalSlipSpeed()
    {
        const int nCriticalSlipSpeed[ctTotal] = {24, 0, 10};
        return nCriticalSlipSpeed[m_BaseInfo.dwCellType];
    }  

    inline BOOL CheckSlip()
    {
        const int nCriticalGradientDegree[ctTotal]  = {4, 8, 1};
        int       nCellType                         = m_BaseInfo.dwCellType;
        int       nGradientDegree                   = m_BaseInfo.dwGradientDegree;

        // ½�ع�45�ȴ�,ˮ�治�����жϴ�(Ҫ�ж�ˮ�׵ر�Ĵ�),������Զ��
        if (nCellType == ctWater)
        {
            return false;
        }

        if (nCellType == ctIce)
        {
            return true;
        }

        return nGradientDegree >= nCriticalGradientDegree[nCellType];
    }


	DECLARE_LUA_CLASS(KCell);
	DECLARE_LUA_STRUCT_INTEGER(IsBlockCharacter, m_BaseInfo.dwBlockCharacter);
	DECLARE_LUA_STRUCT_INTEGER(CanPutObj, m_BaseInfo.dwPutObj);
	DECLARE_LUA_STRUCT_INTEGER(IsSafe, m_BaseInfo.dwRest);
	DECLARE_LUA_STRUCT_INTEGER(CanStall, m_BaseInfo.dwStall);
	DECLARE_LUA_STRUCT_INTEGER(Type, m_BaseInfo.dwCellType);
};

struct KDynamicCell : KCell 
{
    WORD m_wGroupIndex;
};

#pragma pack()


#endif	//_KCELL_H_
