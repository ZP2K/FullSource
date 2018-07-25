#include "StdAfx.h"
#include "KG3DSceneLogicalEditorBase.h"
#include "Klist.h"
#include "../include/SO3World/KCell.h"
#include "../include/SO3World/KRegion.h"
#include "../Include/SO3World/KScene.h"

#include "IEKG3DTerrain.h"
#include "SceneEditor.h"

#include "InputProxy.h"
#include "..\..\KG3DEngine\KG3DEngine\MathTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TODWORD(baseinfo) (*(DWORD*)baseinfo)
const static short ScriptIDCount = 0xf;

KG3DSceneLogicalEditorBase::CellInfo::CellInfo(const CellInfo& src)
{
	//ʹ��Ĭ�ϵ�COPY���캯��
	*this = src;
}

KG3DSceneLogicalEditorBase::CellInfo::CellInfo()
{
	vPosition = D3DXVECTOR3(0,0,0);
	fTop = 0;
	pCell = NULL;
	bSelected = FALSE;
	pRegion = NULL;
	byX = 0;
	byY = 0;
}

#define GETBBOX(pTerrain)\
	AABBOX* pBox = NULL;\
	pTerrain->GetBBox((LPVOID*)&pBox);\
	_ASSERTE(pBox);

KG3DSceneLogicalEditorBase::CellInfo::CellInfo(bool abEditGroundCells,IEKG3DSceneSceneEditor* pEditor,int nX,int nZ,KCell* pCellInput, KRegion* argpRegion, BYTE argbyX, BYTE argbyY,float fLogicalStartX,float fLogicalStartZ)
{
	pCell = pCellInput;
	//vPostion �����߼���ʽ�����½ǵĵ�����
	//GETBBOX(pTerrain);

	vPosition.x = fLogicalStartX + nX * LOGICAL_CELL_CM_LENGTH; //nX ������ͼ��X�����ϵ��߼����, vPoition.x = �ӵ�ͼ���½�����������ֵ
	vPosition.z = fLogicalStartZ + nZ * LOGICAL_CELL_CM_LENGTH;

	pEditor->GetGroundHeight(H[0], vPosition.x, vPosition.z);
	pEditor->GetGroundHeight(H[1], vPosition.x + LOGICAL_CELL_CM_LENGTH, vPosition.z);
	pEditor->GetGroundHeight(H[2], vPosition.x + LOGICAL_CELL_CM_LENGTH, vPosition.z + LOGICAL_CELL_CM_LENGTH);
	pEditor->GetGroundHeight(H[3], vPosition.x, vPosition.z + LOGICAL_CELL_CM_LENGTH);

	vPosition.y = max(H[0] - 50, pCell->m_wLowLayer  * ALTITUDE_UNIT + TERRAIN_MIN_HEIGHT);

	fTop       = pCell->m_wHighLayer * ALTITUDE_UNIT + TERRAIN_MIN_HEIGHT; 
	bSelected = FALSE;
	BOOL bGourndCell = abEditGroundCells;
	if (abEditGroundCells)
	{
		//�����logic cell�߶� <= ��ǰ���θ���ߵ㣬����Ϊû��������ϲ�������Ҫ����б�����
		bGourndCell = fTop <= max(H[0], max(H[1], max(H[2], H[3])));
	}
	if (!bGourndCell)
	{
		H[0] = H[1] = H[2] = H[3] = fTop; //Ϊ����ǵر������ʾ�����߼���������
	}

	vBBox.Clear();
	vBBox.AddPosition(vPosition);
	D3DXVECTOR3 Top = vPosition + D3DXVECTOR3(LOGICAL_CELL_CM_LENGTH,0,LOGICAL_CELL_CM_LENGTH);
	Top.y = fTop;
	vBBox.AddPosition(Top);
	pRegion = argpRegion;
	byX = argbyX;
	byY = argbyY;
}

BOOL KG3DSceneLogicalEditorBase::CellInfo::IsRayIntersect(float& fDistance,D3DXVECTOR3& CameraPos,D3DXVECTOR3& CameraDirct)
{
	D3DXVECTOR3 BoxPos[8];
	vBBox.Extract(BoxPos);

	D3DXVECTOR3 Inter(0,0,0);

	if(IsLineIntersectBox(&Inter,&CameraPos,&(CameraPos+CameraDirct*10000),BoxPos))
	{
		D3DXVECTOR3 V = Inter - CameraPos;
		float Len = D3DXVec3Length(&V);

		fDistance = Len;
		return TRUE;
	}
	return FALSE;
}

KG3DSceneLogicalEditorBase::KG3DSceneLogicalEditorBase(void) 
: m_d3dclrCurrColor(0x80FF8080), m_dwCurrentScriptID(0), m_nBrushWidth(5), m_nDisplayWidth(1), m_nCellHorzHeight(1), m_eBrushType(cbRectangle)
{
	m_pLogicalScene = NULL;
	m_pCellVB = NULL;
	m_pCellIB = NULL;
	m_bEditGroundCells = TRUE;
	m_bEditOverGroundCells = TRUE;
	m_nCurRegionX = -10000;
	m_nCurRegionZ = -10000;
	m_dwLogicalCellModifyState = MODIFY_NONE;
	m_dwLogicalCellShowState = MODIFY_NONE;
	m_bAddCellInfo = TRUE;

	m_bShowLogicalCell = FALSE;
	m_bShowLogicalTerrenCell = FALSE;
	m_bShowLogicalSelectedCell = FALSE;
	m_bShowLogicalItemCell = FALSE;
	m_pProxy = NULL;
	m_bShowRest = FALSE;
}

void KG3DSceneLogicalEditorBase::SetInputProxy(CInputProxy* pProxy)
{
	m_pProxy = pProxy;
}


KG3DSceneLogicalEditorBase::~KG3DSceneLogicalEditorBase(void)
{
	UnInit();
}

HRESULT KG3DSceneLogicalEditorBase::Init()
{
	SAFE_RELEASE(m_pCellVB);
	SAFE_RELEASE(m_pCellIB);
	m_pLogicalScene = NULL;

	m_listLogicalCellSelectableGround.clear();
	m_listLogicalCellSelectableOverGround.clear();
	m_listLogicalCellSelected.clear();
	m_listLogicalRegionEditing.clear();

	CreateBuffersForRenderCell();
	return S_OK;
}

HRESULT KG3DSceneLogicalEditorBase::UnInit()
{
	SAFE_RELEASE(m_pCellVB);
	SAFE_RELEASE(m_pCellIB);

	m_pLogicalScene = NULL;
	m_listLogicalCellSelectableGround.clear();
	m_listLogicalCellSelectableOverGround.clear();
	m_listLogicalCellSelected.clear();
	m_listLogicalRegionEditing.clear();
	m_nCurRegionX = -10000;
	m_nCurRegionZ = -10000;
	return S_OK;
}

HRESULT KG3DSceneLogicalEditorBase::SetLogicalScene(KScene* pLogicalScene)
{
	m_pLogicalScene = pLogicalScene;
	return S_OK;
}

KScene* KG3DSceneLogicalEditorBase::GetLogicalScene()
{
	return m_pLogicalScene;
}

HRESULT KG3DSceneLogicalEditorBase::GetLogicScene(LPVOID* ppLogicScene)
{
	_ASSERTE(ppLogicScene);
	if (ppLogicScene)
	{
		*ppLogicScene = m_pLogicalScene;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT KG3DSceneLogicalEditorBase::SetModifyState(eCellModifyState eState,BOOL bModify)
{
	if(bModify)
	{
		m_dwLogicalCellModifyState |= eState;
	}
	else
	{
		m_dwLogicalCellModifyState &= (~eState);
	}
	return S_OK;
}

BOOL KG3DSceneLogicalEditorBase::GetModifyState(eCellModifyState eState)
{
	return(m_dwLogicalCellModifyState&eState);
}

DWORD GetIDFromFileName(LPCTSTR lpstrFileName,BOOL bFullPath)
{
	DWORD dwRet = 0;
	TCHAR szFileName[MAX_PATH + 1];
	if (!lpstrFileName ||strlen(lpstrFileName) == 0)
	{
		dwRet = 0;
	}
	else
	{
		if(bFullPath)
		{
			g_GetFilePathFromFullPath(szFileName, lpstrFileName);///����ֱ�Ӵ������·��
			if(_strcmpi(szFileName,"")==0)
				dwRet = 0;
			else
				dwRet = g_FileNameHash(szFileName);
		}
		else
		{
			if(_strcmpi(lpstrFileName,"")==0)
				dwRet = 0;
			else
				dwRet = g_FileNameHash(lpstrFileName);
		}
	}
	//else
	//{
	//	dwRet = 0;
	//	CString smsg;
	//	smsg.Format("ע�⣬��ѡ��Ľű��ļ�[%s]����Ч�ģ���ѡ������ִ���ļ���ͬ��Ŀ¼�µ��ļ���", lpstrFileName);
	//	AfxMessageBox(smsg, MB_OK|MB_ICONERROR);
	//	KGLOG_PROCESS_ERROR(0); //����ʧ�ܣ�����ű���Ϊ��
	//}
//Exit0:
	return dwRet;
}

//���ļ���תΪID
void KG3DSceneLogicalEditorBase::SetCurrentScriptFileName(LPCTSTR lpstrFileName)
{
	m_dwCurrentScriptID = GetIDFromFileName(lpstrFileName,FALSE);
}

HRESULT KG3DSceneLogicalEditorBase::SetShowState(eCellModifyState eState,BOOL bModify)
{
	if(bModify)
	{
		m_dwLogicalCellShowState |= eState;
	}
	else
	{
		m_dwLogicalCellShowState &= (~eState);
	}
	//(by dengzhihui 2006��7��26�� 15:55:31
	//RecalcShowCell();
	//)
	return S_OK;
}

BOOL KG3DSceneLogicalEditorBase::GetShowState(eCellModifyState eState)
{
	return(m_dwLogicalCellShowState&eState);
}

void KG3DSceneLogicalEditorBase::GetMapSize(int* pMapWidth, int* pMapHeight)
{
	int& nWidth = *pMapWidth;
	for(; nWidth < MAX_REGION_WIDTH; nWidth ++)
	{
		KRegion* pRegion = m_pLogicalScene->GetRegion(nWidth, 0);
		if (!pRegion) break;
	}
	int& nHeight = *pMapHeight;
	for (; nHeight < MAX_REGION_HEIGHT; nHeight ++)
	{
		KRegion* pRegion = m_pLogicalScene->GetRegion(0, nHeight);
		if (!pRegion) break;
	}	
}





HRESULT KG3DSceneLogicalEditorBase::UpdateEditingCells(int nXStart,int nZStart,IEKG3DSceneSceneEditor* pEditor,D3DXVECTOR3& InterPos)
{
	KGLOG_PROCESS_ERROR(m_pLogicalScene);

	m_listInterPos.push_back(InterPos);
	if (m_listInterPos.size()>200)
	{
		m_listInterPos.pop_front();
	}

	{
		int nXcm = (int)(InterPos.x - nXStart*100);
		int nZcm = (int)(InterPos.z - nZStart*100);
		int nRegionX = nXcm/LOGICAL_CELL_CM_LENGTH/REGION_GRID_WIDTH;
		int nRegionZ = nZcm/LOGICAL_CELL_CM_LENGTH/REGION_GRID_HEIGHT;

        static int lastDistplayWidth = -1;
		static int lastInRegionRectX = -1;
		static int lastInRegionRectZ = -1;


		//��ǰ��ʾ��Ȳ��䣬��ǰ���༭��region����Ҳ�������Χ֮��
		if(lastDistplayWidth == m_nDisplayWidth && nRegionX / lastDistplayWidth == lastInRegionRectX && nRegionZ / lastDistplayWidth == lastInRegionRectZ)
		{
		  return S_OK;
		}
		
		//OutputDebugString("���¼���.............\r\n");
        
		lastDistplayWidth = m_nDisplayWidth;
		lastInRegionRectX = nRegionX / lastDistplayWidth;
		lastInRegionRectZ = nRegionZ / lastDistplayWidth;

		m_nCurRegionX = nRegionX;
		m_nCurRegionZ = nRegionZ;
		
		int nRegionXStart = max(m_nCurRegionX - (m_nDisplayWidth - 1), 0);
		int nRegionXEnd   = m_nCurRegionX + (m_nDisplayWidth - 1);
		int nRegionZStart = max(m_nCurRegionZ - (m_nDisplayWidth - 1), 0);
		int nRegionZEnd   = m_nCurRegionZ + (m_nDisplayWidth - 1);


		// ��������ŵ�ͼ��������Ҫ������ʾ����
		//���õ����ŵ�ͼ�Ĵ�С
		int nMapWidth = 0;
		int nMapHeight = 0;
		GetMapSize(&nMapWidth, &nMapHeight);
		if (m_nXStart <= 0 && m_nXEnd >= nMapWidth 
			&& m_nZStart <= 0 && m_nZEnd >= nMapHeight)
			return S_OK;

		
		//int nRegionXStart = max(nRegionX - (m_nDisplayWidth - 1), 0);
		//int nRegionXEnd   = nRegionX + (m_nDisplayWidth - 1);
		//int nRegionZStart = max(nRegionZ - (m_nDisplayWidth - 1), 0);
		//int nRegionZEnd   = nRegionZ + (m_nDisplayWidth - 1);

		/*
		//����� 1 �� Region �Ĺ��ܣ������Ż�
		int nRegionXStart = nRegionX;
		int nRegionXEnd   = nRegionX;
		int nRegionZStart = nRegionZ;
		int nRegionZEnd   = nRegionZ;
		*/

		m_nXStart = nRegionXStart;
		m_nXEnd   = nRegionXEnd;
		m_nZStart = nRegionZStart;
		m_nZEnd   = nRegionZEnd; 

		m_listLogicalRegionEditing.clear();
		m_listLogicalCellSelectableGround.clear();
		m_listLogicalCellSelectableOverGround.clear();
		m_listLogicalCellSelected.clear();


		for(int z=nRegionZStart;z<=nRegionZEnd;z++)
		{
			for (int x=nRegionXStart;x<=nRegionXEnd;x++)
			{
				KRegion* pRegion = m_pLogicalScene->GetRegion(x,z);
				if(pRegion)
				{
					m_listLogicalRegionEditing.push_back(pRegion);
				}
			}
		}

	}
	//////////////////////////////////////////////////////////////////////////
	//Get Selectable Cell
	{
		list<KRegion*>::iterator i = m_listLogicalRegionEditing.begin();
		list<KRegion*>::iterator iend = m_listLogicalRegionEditing.end();
		while (i!=iend)
		{
			KRegion* pRegion = *i;
			for(BYTE Z=0;Z<REGION_GRID_HEIGHT;Z++)
			{
				for(BYTE X=0;X<REGION_GRID_WIDTH;X++)
				{
					KCell* pCell = &pRegion->m_pCells[REGION_GRID_WIDTH * Z + X];

					if(pCell->m_BaseInfo.dwCellType != ctWater )
					{
						if(m_bEditGroundCells)
						{
							CellInfo cellinfo(true, pEditor,
								X + pRegion->m_nRegionX*REGION_GRID_WIDTH,    //m_dwRegionX �ڵ�ǰ�߼�Region�е�x���
								Z + pRegion->m_nRegionY*REGION_GRID_HEIGHT,
								pCell, pRegion, X, Z,
								nXStart * 100.0F,nZStart * 100.0F);


							m_listLogicalCellSelectableGround.push_back(cellinfo);
						}
						if(m_bEditOverGroundCells)
						{
							pCell = pCell->m_pNext;
							while (pCell)
							{
								CellInfo cellinfo(false, pEditor,
									X+pRegion->m_nRegionX*REGION_GRID_WIDTH,
									Z+pRegion->m_nRegionY*REGION_GRID_HEIGHT,
									pCell, pRegion, X, Z,
									nXStart * 100.0F,nZStart * 100.0F);


								//if(pCell->m_BaseInfo.dwCellType != ctWater )
									m_listLogicalCellSelectableOverGround.push_back(cellinfo);

								pCell = pCell->m_pNext;
							}
						}
					}					
				}
			}
			i++;
		}
	}
	//(by dengzhihui 2006��7��26�� 15:55:31
	RecalcShowCell();
	//)

	return S_OK;
Exit0:
	return E_FAIL;
}

//(by dengzhihui 2006��7��27�� 9:08:57
void KG3DSceneLogicalEditorBase::RecalcShowCell()
{
	m_listLogicalCellCurrMarked.clear();
	m_listLogicalCellDiffMarked.clear();
	GetShowCellList(&m_listLogicalCellCurrMarked, &m_listLogicalCellDiffMarked, m_listLogicalCellSelectableGround);
	GetShowCellList(&m_listLogicalCellCurrMarked, &m_listLogicalCellDiffMarked, m_listLogicalCellSelectableOverGround);
}

void KG3DSceneLogicalEditorBase::GetCurrentModifyMask(DWORD dwCurrentState, DWORD* pDes)
{
	KCell::KBaseInfo stCellNullMask;
	KCell::KBaseInfo& stCellCurrModifyMask = *(KCell::KBaseInfo*)pDes;
	memset(&stCellNullMask, 0x0, sizeof(KCell::KBaseInfo));

	CopyMemory(&stCellCurrModifyMask, &stCellNullMask, sizeof(KCell::KBaseInfo));
	if(dwCurrentState&MODIFY_BLOCKCHARACTER)
		stCellCurrModifyMask.dwBlockCharacter = m_bAddCellInfo;
	if(dwCurrentState&MODIFY_PUTOBJECT)
		stCellCurrModifyMask.dwPutObj = m_bAddCellInfo;
	if(dwCurrentState&MODIFY_SAFE)
		stCellCurrModifyMask.dwRest = m_bAddCellInfo;
	if(dwCurrentState&MODIFY_STALL)
		stCellCurrModifyMask.dwStall = m_bAddCellInfo;

	if(dwCurrentState&MODIFY_INDOOR)
		stCellCurrModifyMask.dwIndoor = m_bAddCellInfo;
	if(dwCurrentState&MODIFY_REST)
		stCellCurrModifyMask.dwRest = m_bAddCellInfo;
	if(dwCurrentState&MODIFY_RIDEHORSE)
		stCellCurrModifyMask.dwRideHorse = m_bAddCellInfo;
	//@
	if(dwCurrentState&MODIFY_SCRIPTFILE)
		stCellCurrModifyMask.dwScriptIndex = 0;//dwScriptID;
}

//scriptidλ��0������λ�� 1
inline DWORD GetScriptIDMask()
{
	DWORD dwRet = 0xffffffff;
	KCell::KBaseInfo& stCellReservedMask = *(KCell::KBaseInfo*)&dwRet;
	stCellReservedMask.dwScriptIndex = 0;
	return dwRet;
}
//dwRestλ��0������λ��1
inline DWORD GetRestMask()
{
	DWORD dwRet = 0xffffffff;
	KCell::KBaseInfo& stCellReserverMask = *(KCell::KBaseInfo*)&dwRet;
	stCellReserverMask.dwRest = 0;
	return dwRet;
}

//����λ��0������λ�� 1
inline DWORD GetReserveMask()
{
	DWORD dwRet = 0xffffffff;
	KCell::KBaseInfo& stCellReservedMask = *(KCell::KBaseInfo*)&dwRet;
	stCellReservedMask.dwNoObstacleRange = 0;
	stCellReservedMask.dwFaceUp	= 0;
	stCellReservedMask.dwDynamic	= 0;
	stCellReservedMask.dwGradientDirection = 0;
	stCellReservedMask.dwPassWidth = 0;
	stCellReservedMask.dwGradientDegree = 0;	
	stCellReservedMask.dwBarrierDirection = 0;
	stCellReservedMask.dwCellType = 0;
	return dwRet;
}

inline void FindScriptIDbyRegion(KRegion* pRegion, DWORD dwOldScriptID, DWORD dwNewScriptID, 
								 DWORD* pdwFindOldIndex, DWORD* pdwFindNewIndex)
{
	if (dwOldScriptID == 0 && pdwFindOldIndex)
		*pdwFindOldIndex = 0;
	if (dwNewScriptID == 0 && pdwFindNewIndex)
		*pdwFindNewIndex = 0;
	if (dwNewScriptID == 0 && dwOldScriptID == 0)
		return;
	for(int nSciprtIndex = 0; nSciprtIndex <= ScriptIDCount; nSciprtIndex++)
	{
		//����ID��ÿ��REGION���ǲ�ͬ��, ��Ҫ���·Ž�mask��һ��
		//����ǰregion��Ӧ�� oldmask �� newmask ��������
		if (dwOldScriptID != 0 && pdwFindOldIndex && pRegion->GetScriptIDByIndex(nSciprtIndex) == dwOldScriptID)					
			*pdwFindOldIndex = nSciprtIndex;
		else if (dwNewScriptID != 0 && pdwFindNewIndex && pRegion->GetScriptIDByIndex(nSciprtIndex) == dwNewScriptID )
			*pdwFindNewIndex = nSciprtIndex;
	}
}

void KG3DSceneLogicalEditorBase::DeleteCurrentCellInfo()//long lLogicInfo, LPCTSTR lpstrScriptFileName)
{
	//if (lLogicInfo == 0)
	//	return;
	DWORD dwCurrentMask = 0;
	GetCurrentModifyMask(m_dwLogicalCellShowState, &dwCurrentMask);
	KCell::KBaseInfo& stCellCurrModifyMask = *(KCell::KBaseInfo*)&dwCurrentMask;
	DWORD dwReservedMask = GetReserveMask();
	DWORD dwSetInReservedMask = ~dwReservedMask;
	//����ȫ��ͼ���е�cell������lLogicInfo��ͬ�õ�cell����ȫ�����
	DWORD* pBaseInfo;
	for(int i = 0; i < MAX_REGION_WIDTH; i ++)
		for (int j = 0; j < MAX_REGION_HEIGHT; j ++)
		{
			KRegion* pRegion = m_pLogicalScene->GetRegion(i, j);
			if (!pRegion) continue;
			DWORD dwCurrRegionScriptIndex = 0;
			FindScriptIDbyRegion(pRegion, m_dwCurrentScriptID, 0, &dwCurrRegionScriptIndex, NULL);
			if (dwCurrRegionScriptIndex == 0 && m_dwCurrentScriptID != 0) //��region��û��Ҫ�ҵ�script����
				continue;
			((KCell::KBaseInfo*)&dwCurrentMask)->dwScriptIndex = dwCurrRegionScriptIndex;
			if (pRegion)
			{
				for(int Z=0;Z<REGION_GRID_HEIGHT;Z++)
                {
					for(int X=0;X<REGION_GRID_WIDTH;X++)
					{
						KCell* pCell = &pRegion->m_pCells[REGION_GRID_WIDTH * Z + X];
						while (pCell)
						{	//�����Ȱ�baseinfoתΪ32�ֽڵ�dword,��ֱ����λ�����ȽϿ�
							pBaseInfo = (DWORD*)&(pCell->m_BaseInfo);
							if((*pBaseInfo & dwReservedMask) == dwCurrentMask)
							{
								*pBaseInfo &= dwSetInReservedMask; //������λ������λ��0
							}
							pCell = pCell->m_pNext;
						}
					}
                }
			}
			else
				continue;
		}
		//����ǰ�༭��־���
		m_dwCurrentScriptID = 0;
		m_dwLogicalCellShowState = 0;
		m_dwLogicalCellModifyState = 0;
}

inline bool SetScriptID(KRegion* pRegion, KCell* pCell, DWORD dwNewScriptID)
{
	if (!pRegion->SetScriptID(pCell, dwNewScriptID)) //�ű��ռ����
	{
		if (!pRegion->RecycleScriptIndex())
		{
			KGLOG_PROCESS_ERROR(0); //�ű��ռ����
		}
		else if(!pRegion->SetScriptID(pCell, dwNewScriptID))//�ٴγ���ִ��
		{
			KGLOG_PROCESS_ERROR(0); //�ű��ռ����
		}
	}
	return true;
Exit0:
	//MessageBox("����ͬһ��Region�������˳���16���ű������½ű��ռ�������˲������ܾ�ִ�С�");
	return false;
}


void KG3DSceneLogicalEditorBase::ModifyCurrentCellInfo(long lOldInfo, long lNewInfo, LPCTSTR OldScriptFileName, LPCTSTR NewScriptFileName)
{
	KGLOG_PROCESS_ERROR(m_pLogicalScene);

	if (lOldInfo == 0 && (!OldScriptFileName || strlen(OldScriptFileName) == 0))
		return;
	//����ȫ��ͼ���е�cell������oldinfo��ͬ��ͬ���õ�cell, תΪNewInfo
	DWORD dwOldMask = 0;
	DWORD dwNewMask = 0;
	GetCurrentModifyMask(lOldInfo, &dwOldMask);
	GetCurrentModifyMask(lNewInfo, &dwNewMask);
	DWORD dwOldScriptIndex = 0;
	DWORD dwNewScriptIndex = 0;

	DWORD dwReservedMask = GetReserveMask();
	//dwNewMask |= (~dwReservedMask);  //������λ����Ϊ1
	DWORD dwInvReservedMask = ~dwReservedMask;
	//����ȫ��ͼ���е�cell
	//short nRegionIndexCache[ScriptIDCount];
	DWORD dwOldScriptID = GetIDFromFileName(OldScriptFileName,FALSE);
	DWORD dwNewScriptID = GetIDFromFileName(NewScriptFileName,FALSE);
	//DWORD dwScriptIDMask = GetScriptIDMask();
	DWORD* pBaseInfo;
	for(int i = 0; i < MAX_REGION_WIDTH; i ++)
		for (int j = 0; j < MAX_REGION_HEIGHT; j ++)
		{
			KRegion* pRegion = m_pLogicalScene->GetRegion(i, j);			
			if (pRegion)
			{	//���ҵ�ǰREGION�ж�Ӧ newinf �� oldinfo �� index
				dwOldScriptIndex = 0;
				dwNewScriptIndex = 0;
				FindScriptIDbyRegion(pRegion, dwOldScriptID, dwNewScriptID, &dwOldScriptIndex, &dwNewScriptIndex);
				if (dwOldScriptIndex == 0 && dwOldScriptID != 0) //��region��û��Ҫ�ҵ�script����
					continue;
				((KCell::KBaseInfo*)&dwOldMask)->dwScriptIndex = dwOldScriptIndex;
				((KCell::KBaseInfo*)&dwNewMask)->dwScriptIndex = dwNewScriptIndex;
				for(int Z=0;Z<REGION_GRID_HEIGHT;Z++)
					for(int X=0;X<REGION_GRID_WIDTH;X++)
					{
						KCell* pCell = &pRegion->m_pCells[REGION_GRID_WIDTH * Z + X];
						while (pCell)
						{	
							pBaseInfo = (DWORD*)&(pCell->m_BaseInfo);

							if((*pBaseInfo & dwReservedMask) == dwOldMask //������λ���ű�λ��������ͬ
								//&& (dwOldScriptID == 0 || dwOldScriptIndex == pCell->m_BaseInfo.m_ScriptIndex) //���������trap��û�нű� ���� ��ǰCELL���� ������trap�еĽű�index��ͬ
								)  
							{								
								//����λ����ԭ�����ݣ�����λ��ΪNewInfo������								
								//*pBaseInfo &= dwNewMask;  //�������newinfo��ͬ�����ݣ�����λ����
								//*pBaseInfo |= dwNewMask;  //�ȼӻ�newinfo���У���pBaseInfo��û�е�����
								*pBaseInfo &= dwInvReservedMask;
								*pBaseInfo |= dwNewMask;

								if (!SetScriptID(pRegion, pCell, dwNewScriptID))
								{
									AfxMessageBox("����ͬһ��Region�������˳���16���ű������½ű��ռ�������˲������ܾ�ִ�С�");
									KGLOG_PROCESS_ERROR(0); //�ű��ռ����
								}
							}
							pCell = pCell->m_pNext;
						}
					}
			}
			else
				continue; //��ͼ��һ���Ƿ��ε�
		}
Exit0:
		;
}

//)
//�޸ĵ�ǰ��괦������
HRESULT KG3DSceneLogicalEditorBase::PaintSelectedLogicalCell(BOOL bAddCellInfo, BOOL bForceDel)
{
	if (!m_pProxy || !m_pProxy->IsCaptured())
		return S_OK;

	defShowListCellInfo::iterator i = m_listLogicalCellSelected.begin();

	bAddCellInfo = bForceDel ? FALSE : bAddCellInfo;

	DWORD dwOldScriptIndex = 0;
	DWORD dwOldScriptID = 0;
	DWORD dwOldMask = 0;
	DWORD dwReservedMask = GetReserveMask();
	GetCurrentModifyMask(m_dwLogicalCellModifyState, &dwOldMask);
	while (i!=m_listLogicalCellSelected.end())
	{
		CellInfo*& pcellInfo = *i;
		KCell::KBaseInfo& BaseCell = pcellInfo->pCell->m_BaseInfo;

		if (!bForceDel && !bAddCellInfo) //����û�ǿ��ɾ��bForceDel=true���򲻹��Ƿ�ǰѡ�е�TRAP��ȫ�����
		{
			FindScriptIDbyRegion(pcellInfo->pRegion, dwOldScriptID, 0, &dwOldScriptIndex, NULL);
			if (dwOldScriptIndex == 0 && dwOldScriptID != 0) //��region��û��Ҫ�ҵ�script����
				continue;
			((KCell::KBaseInfo*)&dwOldMask)->dwScriptIndex = dwOldScriptIndex;

			DWORD dwBaseInfo = *(DWORD*)(&pcellInfo->pCell->m_BaseInfo);
			if((dwBaseInfo & dwReservedMask) != dwOldMask) //���ǻ��Trap
			{
				i++;
				continue;
			}
		}

		BaseCell.dwBlockCharacter = m_dwLogicalCellModifyState&MODIFY_BLOCKCHARACTER ? bAddCellInfo : 0;
		BaseCell.dwPutObj = m_dwLogicalCellModifyState&MODIFY_PUTOBJECT ? bAddCellInfo : 0;
		BaseCell.dwRest = m_dwLogicalCellModifyState&MODIFY_SAFE ? bAddCellInfo : 0;
		BaseCell.dwStall = m_dwLogicalCellModifyState&MODIFY_STALL ? bAddCellInfo : 0;

		BaseCell.dwIndoor = m_dwLogicalCellModifyState&MODIFY_INDOOR ? bAddCellInfo : 0;
		BaseCell.dwRest = m_dwLogicalCellModifyState&MODIFY_REST ? bAddCellInfo : 0;
		BaseCell.dwRideHorse = m_dwLogicalCellModifyState&MODIFY_RIDEHORSE ? bAddCellInfo : 0;

		//@
		//BaseCell.m_ScriptIndex = m_dwLogicalCellModifyState&MODIFY_SCRIPTFILE ? m_dwCurrentScriptID : 0;
		bool bScriptOverflow = false;
		if (bAddCellInfo)
			bScriptOverflow = !SetScriptID(pcellInfo->pRegion, pcellInfo->pCell, m_dwCurrentScriptID);
		else
			bScriptOverflow = !SetScriptID(pcellInfo->pRegion, pcellInfo->pCell, 0);

		if (bScriptOverflow)
		{
			AfxMessageBox("����ͬһ��Region�������˳���16���ű������½ű��ռ�������˲������ܾ�ִ�С�");
			break;
		}

		if (m_dwLogicalCellModifyState != MODIFY_NONE || m_dwCurrentScriptID)
		{
			if (bForceDel)
			{
				m_listLogicalCellCurrMarked.remove(pcellInfo);
				m_listLogicalCellDiffMarked.remove(pcellInfo);
			}
			else
			{
				if(bAddCellInfo)
				{
					//(by dengzhihui 2006��7��26�� 16:10:36
					m_listLogicalCellCurrMarked.remove(pcellInfo);//�����ظ�����cellinfo����
					m_listLogicalCellDiffMarked.remove(pcellInfo);//ͬʱ����������ɫ����
					m_listLogicalCellCurrMarked.push_back(pcellInfo);
					//)
				}
				else
				{
					//(by dengzhihui 2006��7��26�� 16:10:36
					m_listLogicalCellCurrMarked.remove(pcellInfo);
					//)
				}
			}
		}
		i++;
	}
	return S_OK;
}

HRESULT KG3DSceneLogicalEditorBase::RenderLogicalCellList(defShowListCellInfo* pList,DWORD dwColorInput)
{
	VFormat::_Faces_Diffuse_Texture1* pV = NULL;
	defShowListCellInfo::iterator i;
	int nCount = 0;
	HRESULT hr = S_OK;
	D3DXMATRIX mat;

	KGLOG_PROCESS_ERROR(m_pCellVB);

	hr = m_pCellVB->Lock(0,0,(void**)&pV,0);
	KGLOG_COM_PROCESS_ERROR(hr);

	D3DXMatrixIdentity(&mat);

	g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat );
	g_pd3dDevice->SetTexture(0,NULL);

	i = pList->begin();
	while (i!=pList->end())
	{
		CellInfo*& pcellinfo = *i;

		float HOff = -pcellinfo->vPosition.y + (10 * m_nCellHorzHeight - 1) + 1;
		//float H = pcellinfo->fTop + HOff;

		DWORD dwColor = dwColorInput;
		int nCount8 = nCount * 8;
		//λ���������
		pV[nCount8+BOX_A1].diffuse = dwColor;
		pV[nCount8+BOX_A1].p = pcellinfo->vPosition;
		pV[nCount8+BOX_A1].tu1 = 0;
		pV[nCount8+BOX_A1].tv1 = 0;

		pV[nCount8+BOX_B1].diffuse = dwColor;
		pV[nCount8+BOX_B1].p = pcellinfo->vPosition + D3DXVECTOR3(LOGICAL_CELL_CM_LENGTH,0,0);
		pV[nCount8+BOX_B1].tu1 = 1;
		pV[nCount8+BOX_B1].tv1 = 0;

		pV[nCount8+BOX_C1].diffuse = dwColor;
		pV[nCount8+BOX_C1].p = pcellinfo->vPosition + D3DXVECTOR3(LOGICAL_CELL_CM_LENGTH,0,LOGICAL_CELL_CM_LENGTH);
		pV[nCount8+BOX_C1].tu1 = 1;
		pV[nCount8+BOX_C1].tv1 = 1;

		pV[nCount8+BOX_D1].diffuse = dwColor;
		pV[nCount8+BOX_D1].p = pcellinfo->vPosition + D3DXVECTOR3(0,0,LOGICAL_CELL_CM_LENGTH);
		pV[nCount8+BOX_D1].tu1 = 0;
		pV[nCount8+BOX_D1].tv1 = 1;

		//λ���������
		//���½ǵ�
		pV[nCount8+BOX_A2].diffuse = dwColor;
		pV[nCount8+BOX_A2].p = pcellinfo->vPosition + D3DXVECTOR3(0,pcellinfo->H[0] + HOff,0);
		pV[nCount8+BOX_A2].tu1 = 0;
		pV[nCount8+BOX_A2].tv1 = 0;
		//���½�
		pV[nCount8+BOX_B2].diffuse = dwColor;
		pV[nCount8+BOX_B2].p = pcellinfo->vPosition + D3DXVECTOR3(LOGICAL_CELL_CM_LENGTH,
			pcellinfo->H[1] + HOff, 0);
		pV[nCount8+BOX_B2].tu1 = 1;
		pV[nCount8+BOX_B2].tv1 = 0;
		//���Ͻ�
		pV[nCount8+BOX_C2].diffuse = dwColor;
		pV[nCount8+BOX_C2].p = pcellinfo->vPosition + D3DXVECTOR3(LOGICAL_CELL_CM_LENGTH,
			pcellinfo->H[2] + HOff, LOGICAL_CELL_CM_LENGTH);
		pV[nCount8+BOX_C2].tu1 = 1;
		pV[nCount8+BOX_C2].tv1 = 1;
		//���Ͻ�
		pV[nCount8+BOX_D2].diffuse = dwColor;
		pV[nCount8+BOX_D2].p = pcellinfo->vPosition + D3DXVECTOR3(0,
			pcellinfo->H[3] + HOff,LOGICAL_CELL_CM_LENGTH);
		pV[nCount8+BOX_D2].tu1 = 0;
		pV[nCount8+BOX_D2].tv1 = 1;

		i++;
		nCount++;

		if(nCount>=MAXCELLSRENDEREACHTIME)
		{
			hr = m_pCellVB->Unlock();
			KGLOG_COM_PROCESS_ERROR(hr);

			g_pd3dDevice->SetIndices(m_pCellIB);
			g_pd3dDevice->SetStreamSource(0,m_pCellVB,0,sizeof(VFormat::_Faces_Diffuse_Texture1));
			g_pd3dDevice->SetFVF(D3DFVF_Faces_Diffuse_Texture1);

			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,MAXCELLSRENDEREACHTIME*8,	0,nCount*12);

			nCount = 0;

			hr = m_pCellVB->Lock(0,0,(void**)&pV,0);
			KGLOG_COM_PROCESS_ERROR(hr);

		}
	}

	hr = m_pCellVB->Unlock();
	KGLOG_COM_PROCESS_ERROR(hr);
	if(nCount)
	{
		g_pd3dDevice->SetIndices(m_pCellIB);
		g_pd3dDevice->SetStreamSource(0,m_pCellVB,0,sizeof(VFormat::_Faces_Diffuse_Texture1));
		g_pd3dDevice->SetFVF(D3DFVF_Faces_Diffuse_Texture1);

		g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,MAXCELLSRENDEREACHTIME*8,	0,nCount*12);
	}
	return S_OK;
Exit0:
	return E_FAIL;
}

HRESULT KG3DSceneLogicalEditorBase::GetShowCellList(defShowListCellInfo* pListCurrShow, defShowListCellInfo* pListDiffShow, const list<CellInfo>& listInput)
{
	KCell::KBaseInfo stCellNullMask, stCellCurrModifyMask;
	memset(&stCellNullMask, 0x0, sizeof(KCell::KBaseInfo));

	//���ڱ༭״̬�ļ���ת��Ϊ�ɱȽϵ� Mask;
	GetCurrentModifyMask(m_dwLogicalCellShowState, (DWORD*)&stCellCurrModifyMask);

	BOOL bModifyNull = TODWORD(&stCellNullMask) == TODWORD(&stCellCurrModifyMask) && (m_dwCurrentScriptID == 0);
	DWORD dwReservedMask = GetReserveMask();

	list<CellInfo>::const_iterator i = listInput.begin();
	KRegion* preRegion = NULL;
	DWORD dwPreScriptIndex = 0;
	DWORD dwCurrRegionScriptIndex = 0;
	DWORD dwScriptIDMask = GetScriptIDMask();
	DWORD dwRestMask;
	if (m_bShowRest)
	{
		dwRestMask = 0xffffffff;
	}
	else
	{
		dwRestMask = GetRestMask();
	}
	while (i!=listInput.end() && !bModifyNull)
	{
		const CellInfo& cellinfo = *i;
		KCell::KBaseInfo* pBaseInfo = &cellinfo.pCell->m_BaseInfo;

		/*if(pBaseInfo->dwCellType == ctWater )
		{

		}*/
		//�����ǰCELL�ǿ�
		if ((TODWORD(pBaseInfo) & dwReservedMask & dwRestMask))
		{
			if( ((TODWORD(&stCellCurrModifyMask ) & dwRestMask) == (TODWORD(pBaseInfo) & dwReservedMask & dwScriptIDMask & dwRestMask))) //������λ���ű�λ����Ϣ��λ��������ͬ
			{
				//��Ҫ�ж��Ƿ�ScriptIndex�Ƿ���ͬ
				if (m_dwCurrentScriptID == 0 && pBaseInfo->dwScriptIndex == 0) 
					pListCurrShow->push_back((CellInfo*)&cellinfo); //û������scriptindex����ֱ���ж�Ϊ��ǰ����
				else
				{
					//��ת�����Ƚϵ� sciprtindex Ϊ��regionʵ�ʵ� scriptidx
					if (preRegion == cellinfo.pRegion) //cache
						dwCurrRegionScriptIndex = dwPreScriptIndex;
					else
					{
						DWORD dwFindID = 0;
						for (int i = 0; i < ScriptIDCount; i++)
						{
							dwFindID = cellinfo.pRegion->GetScriptIDByIndex(i);
							if (m_dwCurrentScriptID == dwFindID)
								dwCurrRegionScriptIndex = i;
						}
					}
					//dwCurrRegionScriptIndex��ת��Ϊ��ǰregion��scriptindex, �Ƿ��뵱ǰCELL��scriptID��ͬ
					if (dwCurrRegionScriptIndex == pBaseInfo->dwScriptIndex)
						pListCurrShow->push_back((CellInfo*)&cellinfo);
					else
						pListDiffShow->push_back((CellInfo*)&cellinfo);
				}
			}
			else
				pListDiffShow->push_back((CellInfo*)&cellinfo);
		}
		i++;
	}
	return S_OK;
}

//(by dengzhihui 2006��8��3�� 10:47:49
void KG3DSceneLogicalEditorBase::SetCurrentEditColor(COLORREF clr)
{	
	m_d3dclrCurrColor = ColorValue(Colorref2value(clr, 0x88));
}
//)

HRESULT KG3DSceneLogicalEditorBase::RenderLogicalCell()
{
	GraphicsStruct::RenderStateCullMode cullmode(D3DCULL_NONE);
	GraphicsStruct::RenderState Light(D3DRS_LIGHTING,FALSE);
	GraphicsStruct::RenderStateAlpha Alpha(0x20,TRUE,TRUE,D3DBLEND_SRCALPHA,D3DBLEND_INVSRCALPHA);

	g_pd3dDevice->SetTexture(0,NULL);
	g_pd3dDevice->SetTexture(1,NULL);

	//(by dengzhihui 2006��7��26�� 16:02:15 --�˴������Ѿ�ת�Ƶ�RecalcShowCell������Ҫÿ�ζ�����
	//list<CellInfo>listLogicalCellMarked;
	//m_listLogicalCellCurrMarked.clear();
	//GetShowCellList(&m_listLogicalCellCurrMarked,&m_listLogicalCellSelectableGround);
	//GetShowCellList(&m_listLogicalCellCurrMarked,&m_listLogicalCellSelectableOverGround);
	//)

	RenderLogicalCellList(&m_listLogicalCellDiffMarked,0x80203040); //�ǵ�ǰ���ϵ���ɫ
	RenderLogicalCellList(&m_listLogicalCellCurrMarked,m_d3dclrCurrColor); //��ǰѡ�м��ϵ���ɫ
	RenderLogicalCellList(&m_listLogicalCellSelected,0x88880099);   //�������λ�õĸ���ɫ

	/*if(1)
	{
		list<D3DXVECTOR3>::iterator i = m_listInterPos.begin();
		list<D3DXVECTOR3>::iterator iend = m_listInterPos.end();

		while (i!=iend)
		{
			D3DXVECTOR3 pos = *i;
			g_cGraphicsTool.DrawPoint(pos,0xFF9040FF);
			i++;
		}
	}*/
	return S_OK;
}


KG3DSceneLogicalEditorBase::CellInfo* KG3DSceneLogicalEditorBase::GetFirstLogicalCellIntersectRay(D3DXVECTOR3& CameraPos,D3DXVECTOR3& CameraDirct)
{
	CellInfo* pcellinfo = NULL;

	float fDistance = 1000000;
	list<CellInfo>::iterator i = m_listLogicalCellSelectableOverGround.begin();
	while (i!=m_listLogicalCellSelectableOverGround.end())
	{
		CellInfo& cell = *i;
		cell.bSelected = FALSE;
		float Len = 1000000;
		if(cell.IsRayIntersect(Len,CameraPos,CameraDirct))
		{
			if(Len<fDistance)
			{
				fDistance = Len;
				pcellinfo = &cell;
			}
		}
		i++;
	}
	return pcellinfo;
}

HRESULT KG3DSceneLogicalEditorBase::SelectLogicalCells(int nXStart,int nZStart,IEKG3DSceneSceneEditor* pEditor,D3DXVECTOR3* pCameraPos,D3DXVECTOR3* pCameraDirct,D3DXVECTOR3 vGroundInter)
{
	

	KGLOG_PROCESS_ERROR(m_pLogicalScene);

	m_listLogicalCellSelected.clear();

	CellInfo* pCellInfo = GetFirstLogicalCellIntersectRay(*pCameraPos,*pCameraDirct);

	float Width = (m_eBrushType == cbCircle) ? m_nBrushWidth * float(2000) : m_nBrushWidth * 25;//10000;

	if(pCellInfo)//ѡ���˿��еĸ���
	{
		//if (m_nBrushWidth == 1) //���⴦��ֻ��һ������
		{
			//m_listLogicalCellSelected.push_back(pCellInfo);
		}
		//else
		{		
			list<CellInfo>::iterator i = m_listLogicalCellSelectableOverGround.begin();
			while (i!=m_listLogicalCellSelectableOverGround.end()) //�������еĿ��и���
			{
				CellInfo& cellinfo = *i;

				float x = cellinfo.vPosition.x - pCellInfo->vPosition.x;
				float z = cellinfo.vPosition.z - pCellInfo->vPosition.z;
				int y = cellinfo.pCell->m_wLowLayer - pCellInfo->pCell->m_wLowLayer;

				{
					if( (m_eBrushType == cbCircle && (x*x+z*z<=Width)&&(abs(y)<=2)) ||
						(m_eBrushType == cbRectangle && abs(x) <= Width && abs(z) <= Width && (abs(y)<=2))
						)
					{
						m_listLogicalCellSelected.push_back(&cellinfo);
					}	
				}
				i++;
			}
		}
		//m_listLogicalCellSelected.push_back(*pCellInfo);
	}
	else//�������ϵĸ���
	{
		D3DXVECTOR3 Inter = vGroundInter;

        /*
		list<CellInfo>::iterator i = m_listLogicalCellSelectableGround.begin();
		while (i!=m_listLogicalCellSelectableGround.end())
		{
			CellInfo& cellinfo = *i;

			float x = cellinfo.vPosition.x - Inter.x;
			float z = cellinfo.vPosition.z - Inter.z;
			if( (m_eBrushType == cbCircle && (x*x+z*z<=Width)) ||
				(m_eBrushType == cbRectangle && abs(x) <= Width && abs(z) <= Width)
				)
			{
				m_listLogicalCellSelected.push_back(&cellinfo);
			}
			i++;
		}
		*/
        /////////////////////////////////////////////////////////////////

	    //clear�ǲ��������ͷſռ�ģ�Ч�ʲ����
		m_listLogicalCellGroundSelected.clear();


		list<KRegion*>::iterator i = m_listLogicalRegionEditing.begin();
		list<KRegion*>::iterator iend = m_listLogicalRegionEditing.end();
		while (i!=iend)
		{
			KRegion* pRegion = *i;

			//��С����ͽ���ľ���
			float mincell_x = pRegion->m_nRegionX * 3200;
			float mincell_z = pRegion->m_nRegionY * 3200;

			float maxcell_x = mincell_x + 3200;
			float maxcell_z = mincell_z + 3200;


			if(Inter.x > mincell_x && Inter.x < maxcell_x &&
				Inter.z > mincell_z && Inter.z < maxcell_z)
			{			
				for(BYTE Z=0;Z<REGION_GRID_HEIGHT;Z++)
				{
					for(BYTE X=0;X<REGION_GRID_WIDTH;X++)
					{
						KCell* pCell = &pRegion->m_pCells[REGION_GRID_WIDTH * Z + X];

						if(pCell->m_BaseInfo.dwCellType != ctWater )
						{
							if(m_bEditGroundCells)
							{

								CellInfo cellinfo(true, pEditor,
									X + pRegion->m_nRegionX*REGION_GRID_WIDTH,    //m_dwRegionX �ڵ�ǰ�߼�Region�е�x���
									Z + pRegion->m_nRegionY*REGION_GRID_HEIGHT,
									pCell, pRegion, X, Z,
									nXStart * 100.0F, nZStart * 100.0F);

								//���ݿ���
								m_listLogicalCellGroundSelected.push_back(cellinfo);
								//��ȡ���������ĵ�ַ
                                CellInfo *cellinfo_ref = &(m_listLogicalCellGroundSelected.back());

								float x = cellinfo_ref->vPosition.x - Inter.x;
								float z = cellinfo_ref->vPosition.z - Inter.z;

								if( (m_eBrushType == cbCircle && (x*x+z*z<=Width)) ||
									(m_eBrushType == cbRectangle && abs(x) <= Width && abs(z) <= Width)
									)
								{
									m_listLogicalCellSelected.push_back(cellinfo_ref);
								}

							}
						}
					}
				}
			}
			i ++;
		}
	
		/////////////////////////////////////////////////////////////////
        
	}
	return S_OK;
Exit0:
	return E_FAIL;
}

HRESULT KG3DSceneLogicalEditorBase::CreateBuffersForRenderCell()
{
	HRESULT hr = S_OK;
	DWORD dwNumVertex,dwNumIndex;
	WORD* pIndex = NULL;

	SAFE_RELEASE(m_pCellVB);
	SAFE_RELEASE(m_pCellIB);

	dwNumVertex = MAXCELLSRENDEREACHTIME * 8;
	dwNumIndex  = MAXCELLSRENDEREACHTIME * 6 * 2 * 3;

	hr = g_pd3dDevice->CreateVertexBuffer(dwNumVertex * sizeof(VFormat::_Faces_Diffuse_Texture1),
		0,D3DFVF_Faces_Diffuse_Texture1,
		D3DPOOL_MANAGED, 
		&m_pCellVB, NULL);
	KGLOG_COM_PROCESS_ERROR(hr);

	hr = g_pd3dDevice->CreateIndexBuffer(dwNumIndex*sizeof(DWORD),
		0,D3DFMT_INDEX16,D3DPOOL_MANAGED,
		&m_pCellIB,NULL);
	KGLOG_COM_PROCESS_ERROR(hr);

	//////////////////////////////////////////////////////////////////////////
	//FillIndexBuffer
	hr = m_pCellIB->Lock(0,0,(void**)&pIndex,0);
	KGLOG_COM_PROCESS_ERROR(hr);

	for(int i=0;i<MAXCELLSRENDEREACHTIME;i++)
	{
		pIndex[i*36   ] = (WORD)(i*8+BOX_A2);//A2B2C2D2
		pIndex[i*36+1 ] = (WORD)(i*8+BOX_B2);
		pIndex[i*36+2 ] = (WORD)(i*8+BOX_C2);
		pIndex[i*36+3 ] = (WORD)(i*8+BOX_A2);
		pIndex[i*36+4 ] = (WORD)(i*8+BOX_C2);
		pIndex[i*36+5 ] = (WORD)(i*8+BOX_D2);

		pIndex[i*36+6]  = (WORD)(i*8+BOX_A1);//A1D1D2A2
		pIndex[i*36+7 ] = (WORD)(i*8+BOX_D1);
		pIndex[i*36+8 ] = (WORD)(i*8+BOX_D2);
		pIndex[i*36+9 ] = (WORD)(i*8+BOX_A1);
		pIndex[i*36+10] = (WORD)(i*8+BOX_D2);
		pIndex[i*36+11] = (WORD)(i*8+BOX_A2);

		pIndex[i*36+12] = (WORD)(i*8+BOX_D1);//D1C1C2D2
		pIndex[i*36+13] = (WORD)(i*8+BOX_C1);
		pIndex[i*36+14] = (WORD)(i*8+BOX_C2);
		pIndex[i*36+15] = (WORD)(i*8+BOX_D1);
		pIndex[i*36+16] = (WORD)(i*8+BOX_C2);
		pIndex[i*36+17] = (WORD)(i*8+BOX_D2);

		pIndex[i*36+18] = (WORD)(i*8+BOX_B1);//B1C1C2B2
		pIndex[i*36+19] = (WORD)(i*8+BOX_C1);
		pIndex[i*36+20] = (WORD)(i*8+BOX_C2);
		pIndex[i*36+21] = (WORD)(i*8+BOX_B1);
		pIndex[i*36+22] = (WORD)(i*8+BOX_C2);
		pIndex[i*36+23] = (WORD)(i*8+BOX_B2);

		pIndex[i*36+24] = (WORD)(i*8+BOX_A1);//A1B1B2A2
		pIndex[i*36+25] = (WORD)(i*8+BOX_B1);
		pIndex[i*36+26] = (WORD)(i*8+BOX_B2);
		pIndex[i*36+27] = (WORD)(i*8+BOX_A1);
		pIndex[i*36+28] = (WORD)(i*8+BOX_B2);
		pIndex[i*36+29] = (WORD)(i*8+BOX_A2);

		pIndex[i*36+30] = (WORD)(i*8+BOX_A1);//A1B1C1D1
		pIndex[i*36+31] = (WORD)(i*8+BOX_B1);
		pIndex[i*36+32] = (WORD)(i*8+BOX_C1);
		pIndex[i*36+33] = (WORD)(i*8+BOX_A1);
		pIndex[i*36+34] = (WORD)(i*8+BOX_C1);
		pIndex[i*36+35] = (WORD)(i*8+BOX_D1);
	}
	hr = m_pCellIB->Unlock();
	KGLOG_COM_PROCESS_ERROR(hr);

	return S_OK;
Exit0:
	SAFE_RELEASE(m_pCellVB);
	SAFE_RELEASE(m_pCellIB);
	return E_FAIL;
}
//(by dengzhihui 2006��8��9�� 12:06:45
void KG3DSceneLogicalEditorBase::SetBrushInfo(int nWidth, eCellBrushShape eBrushType)
{
	if (nWidth > 0 && nWidth < 100)
		m_nBrushWidth = nWidth;
	switch(eBrushType)
	{
	case cbRectangle:
	case cbCircle:
		m_eBrushType = eBrushType;
		break;
	default:
		m_eBrushType = cbRectangle;
	}
}

void KG3DSceneLogicalEditorBase::SetDisplayParam(int nDisplayWidth, int nCellHorzHeight)
{
	if (nDisplayWidth > 0 && nDisplayWidth < 100 && m_nDisplayWidth != nDisplayWidth)
	{
		m_nDisplayWidth = nDisplayWidth;
		m_nXStart = 0; m_nXEnd = 0;
		m_nZStart = 0; m_nZEnd = 0;
		//defShowListCellInfo::iterator i = m_listLogicalCellSelected.begin();
		//if (i!=m_listLogicalCellSelected.end())
		//	UpdateEditingCells(i->pRegion->gett)
	}

	if (nCellHorzHeight > 0 && nCellHorzHeight < 100 && m_nCellHorzHeight != nCellHorzHeight)
		m_nCellHorzHeight = nCellHorzHeight;
}


void KG3DSceneLogicalEditorBase::SetShowRest(BOOL bShowRest)
{
	m_bShowRest = bShowRest;
}
//)
//(by dengzhihui Refactor 2006��11��16��
HRESULT KG3DSceneLogicalEditorBase::SetShowLogicalCell(BOOL bValue)
{
	m_bShowLogicalCell = bValue;
	return S_OK;
}
HRESULT KG3DSceneLogicalEditorBase::SetShowLogicalTerrenCell(BOOL bValue)
{
	m_bShowLogicalTerrenCell = bValue;
	return S_OK;
}
HRESULT KG3DSceneLogicalEditorBase::SetShowLogicalItemCell(BOOL bValue)
{
	m_bShowLogicalItemCell = bValue;
	return S_OK;
}
HRESULT KG3DSceneLogicalEditorBase::SetShowLogicalSelectedCell(BOOL bValue)
{
	m_bShowLogicalSelectedCell = bValue;
	return S_OK;
}

HRESULT KG3DSceneLogicalEditorBase::SetLogicScene(LPVOID pLogicScene)
{
	m_pLogicalScene = (KScene*)pLogicScene;
	return S_OK;
}
//)
HRESULT KG3DSceneLogicalEditorBase::GetCellModifyState(DWORD* eState)
{
	*eState = m_dwLogicalCellModifyState;
	return S_OK;
}