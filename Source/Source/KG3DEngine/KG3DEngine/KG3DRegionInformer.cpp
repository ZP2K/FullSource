////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DRegionInformer.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-4-13 15:13:01
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "KG3DRegionInformer.h"
#include "KGINIFileHelpers.h"
#include "KG3DGraphicsTool.h"
#include "KGHelperClasses.h"
#include "KG3DEnvEffDW_KeyBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////
KG3DRegionInformer::KG3DRegionInformer()
: m_nGetTimeTick(0)
, m_dwLastGainTime(0)
, m_nLastRegionIndex(0)
, m_nEnterRegionCount(0)
{

}

KG3DRegionInformer::~KG3DRegionInformer()
{
}

HRESULT KG3DRegionInformer::FrameMove()
{
	if (m_DescStruct.m_bIgnoreRegionChange)//�����Region�ĸı�
		return S_OK;

	m_nGetTimeTick++;
	KG_PROCESS_SUCCESS(m_nGetTimeTick < m_DescStruct.m_nGetTimeGap);//��һ��ʱ���ȡʱ��
	
	{
		m_nGetTimeTick = 0;
        DWORD dwNowTime = g_cGraphicsTool.GetNowTime();
		KG_PROCESS_SUCCESS(dwNowTime - m_dwLastGainTime < m_DescStruct.m_fGetRegionInfoTimeGap);	//��һ��ʱ���ȡRegion
		m_dwLastGainTime = dwNowTime;
	
		//�ӳ�����Region�Ĵ������߼��ļ�������Ա����������л���̬������Ч��
		KG_PROCESS_SUCCESS(IsRegionCheckingStopped());

		INT nIndex = GetRegionTraitByGPS();
		if (nIndex >= 0 )	//ȡRegion�Ĳ���
		{
			_KG3D_DEBUG_SELF_ASSERTE(nIndex != 0 && "Ӧ�ò�Ϊ0�Ŷ�");
			if (m_nLastRegionIndex != nIndex)
			{
				m_nLastRegionIndex = nIndex;
				m_nEnterRegionCount = 1;
			}
			else///������ͬһ��Region���棬�Ӻ�����Ĵ���
			{
				if (m_nEnterRegionCount > 0)///�����ǳ�����һ��Region�е�״̬
				{
					m_nEnterRegionCount++;
					if (m_nEnterRegionCount >= m_DescStruct.m_nBroadcastWhenEncounterSameRegionCount)
					{
						//��μ�鵽���µ�Region�У�����仯��̬����
						if(KG3D_ENVIRONMENT_EFFECT::CheckIsRegionIndexValid(nIndex))
						{
							m_DescStruct.m_nCurRegion = nIndex;
							PostMessage(KG3DMessage(KM_REGION_CHANGED, nIndex));
							m_nEnterRegionCount = 0;
						}
					}
				}
			}
		}
	}
Exit1:
	return S_OK;
}

HRESULT KG3DRegionInformer::SaveMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
{
	KG_PROCESS_ERROR(pIniFile && pSection);
	m_DescStruct.SaveMapSetting(pIniFile, pSection);
	return S_OK;
Exit0:
	return E_FAIL;
}

HRESULT KG3DRegionInformer::LoadMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
{
	if (! pIniFile)
	{
		return CustomInit();
	}
	KG_PROCESS_ERROR(pIniFile && pSection);
	bool bRet = m_DescStruct.LoadMapSetting(pIniFile, pSection);
	KG_PROCESS_ERROR(bRet);
	this->CustomInit();
	_KG3D_DEBUG_SELF_OUTPUT1(_STRINGER(KG3DRegionInformer)_T("����ĵ�ǰRegion��%d\n"), m_DescStruct.m_nCurRegion);
	return S_OK;
Exit0:
	_KG3D_DEBUG_SELF_REPORT(" KG3DRegionInformer::LoadMapSetting����\n");
	m_DescStruct = KG3DRegionInformerDescStruct();///����Ĭ�ϵ�
	this->CustomInit();
	_KG3D_DEBUG_SELF_OUTPUT1(_STRINGER(KG3DRegionInformer)_T("����ĵ�ǰRegion��%d\n"), m_DescStruct.m_nCurRegion);
	return E_FAIL;
}
HRESULT KG3DRegionInformer::GetRegionIndexByGPS( INT& nXIndex, INT& nZIndex )
{
	D3DXVECTOR3 vPos;
	KG_PROCESS_ERROR(m_tpGPS);
	{
		HRESULT hr = m_tpGPS->GetRegionIndexByPos(nXIndex, nZIndex);
		KG_COM_PROCESS_ERROR(hr);
		return S_OK;
	}
Exit0:
	return E_FAIL;
}

INT KG3DRegionInformer::GetRegionTraitByGPS()
{
	if(m_DescStruct.m_bIgnoreRegionChange)
		return m_DescStruct.m_nCurRegion;

	HRESULT hr = E_FAIL;
	INT nX = 0, nZ = 0;
	hr = GetRegionIndexByGPS(nX, nZ);
	KG_COM_PROCESS_ERROR(hr);
	{
		KG_PROCESS_ERROR(m_tpRegionInfoManager);
		INT nRet = 0;
		BOOL bRet = m_tpRegionInfoManager->GetData(nX, nZ, nRet);
		KG_PROCESS_ERROR(bRet);
		_KG3D_DEBUG_SELF_ASSERTE(nRet > 0);
		return nRet;		
	}
Exit0:
	//_KG3D_DEBUG_SELF_REPORT("���ܵõ���ǰCameraRegionλ��");
	return -1;
}

BOOL KG3DRegionInformer::BindManager( KG3DRegionInfoManager& InfoManager )
{
	if (!m_tpRegionInfoManager)
	{
		m_tpRegionInfoManager = &InfoManager;
		if (m_tpRegionInfoManager)
		{
			return TRUE;
		}
	}
	return FALSE;
}

HRESULT KG3DRegionInformer::CustomInit()
{
	if (this->IsInit())
	{
		return S_OK;
	}
	this->RequestModification(m_DescStruct, DWORD(KG3DRI_MDO_ALL));
	{
		INT nTemp = GetRegionTraitByGPS();
		if (nTemp >= 0)
		{
			m_nLastRegionIndex = nTemp;
		}
	}
	this->IsInit(true);
	return S_OK;
}

HRESULT KG3DRegionInformer::AcceptGPS( KG3DGPSBase& RefGPS )
{
	if (!m_tpGPS)
	{
		m_tpGPS = &RefGPS;
		if (m_tpGPS)
		{
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT KG3DRegionInformer::RequestModification(const KG3DRegionInformerDescStruct& DescStruct, DWORD dwModificationOption )
{
	HRESULT hr = S_OK;
	KG3DRegionInformerDescStruct TempStruct  = m_DescStruct;
	if (dwModificationOption & KG3DRI_MDO_GET_TIME_GAP)
	{
		TempStruct.m_nGetTimeGap = DescStruct.m_nGetTimeGap;
		if (TempStruct.m_nGetTimeGap < 1)
			TempStruct.m_nGetTimeGap = 1;
	}
	if (dwModificationOption & KG3DRI_MDO_GET_REGIONINFOR_TIME_GAP)
	{
		TempStruct.m_fGetRegionInfoTimeGap = DescStruct.m_fGetRegionInfoTimeGap;
		if(DescStruct.m_fGetRegionInfoTimeGap < 900)///���ٲ��һ��
			TempStruct.m_fGetRegionInfoTimeGap = 900;

	}
	if (dwModificationOption  & KG3DRI_MDO_CHANGE_WHEN_SAME_REGION_TIMES)
	{
		TempStruct.m_nBroadcastWhenEncounterSameRegionCount = DescStruct.m_nBroadcastWhenEncounterSameRegionCount;
		if (TempStruct.m_nBroadcastWhenEncounterSameRegionCount < 1)
			TempStruct.m_nBroadcastWhenEncounterSameRegionCount = 1;
	}
	if (dwModificationOption & KG3DRI_MDO_CURRENT_REGION)
	{
		TempStruct.m_nCurRegion = DescStruct.m_nCurRegion;
		m_nLastRegionIndex = GetRegionTraitByGPS();
	}
	if (SUCCEEDED(hr))
	{
		m_DescStruct = TempStruct;

		if (dwModificationOption & KG3DRI_MDO_CURRENT_REGION)
		{
			this->PostMessage(KG3DMessage(KM_REGION_CHANGED, m_DescStruct.m_nCurRegion));
		}
		
	}
	return hr;
}
HRESULT KG3DRegionInformer::ReStartRegionChecking()
{
	if (this->IsRegionCheckingStopped())
	{
		this->IsRegionCheckingStopped(false);
	}
	return S_OK;		
}
HRESULT KG3DRegionInformer::StopRegionChecking( INT StoppedRegion )
{
	this->IsRegionCheckingStopped(true);
	return S_OK;
}
HRESULT KG3DRegionInformer::RequestModification( const KG3DREGION_INFORMER_DESCSTRUCT* pDescStruct, DWORD dwModificationOption )
{
	if (pDescStruct)
	{
		KG3DRegionInformerDescStruct TempStruct = *pDescStruct;
		return RequestModification(TempStruct, dwModificationOption);
	}
	return E_FAIL;
}
HRESULT KG3DRegionInformer::GetStruct( KG3DREGION_INFORMER_DESCSTRUCT* pDescStruct )
{
	if (pDescStruct)
	{
		m_DescStruct.ToBaseStruct(pDescStruct);
		return S_OK;
	}
	return E_FAIL;
}


bool KG3DRegionInformerDescStruct::SaveMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
{
	///��Щû�м�Template��������INT�汾
	bool bRet = false;
	KG_PROCESS_ERROR(pIniFile && pSection);
	bRet = KG_CUSTOM_HELPERS::SaveINIFileData(pIniFile, pSection, _STRINGER(m_nGetTimeGap), m_nGetTimeGap);
	KG_PROCESS_ERROR(bRet);
	bRet = KG_CUSTOM_HELPERS::SaveINIFileData<FLOAT>(pIniFile, pSection, _STRINGER(m_fGetRegionInfoTimeGap), m_fGetRegionInfoTimeGap);
	KG_PROCESS_ERROR(bRet);
	bRet = KG_CUSTOM_HELPERS::SaveINIFileData(pIniFile, pSection, "BroadcastWhenEncounterSameRegionCount", m_nBroadcastWhenEncounterSameRegionCount);
	KG_PROCESS_ERROR(bRet);
	bRet = KG_CUSTOM_HELPERS::SaveINIFileData(pIniFile, pSection, "CurRegion", m_nCurRegion);
	KG_PROCESS_ERROR(bRet);
	return true;
Exit0:
	return false;
}

bool KG3DRegionInformerDescStruct::LoadMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
{
	bool bRet = false;
	KG_PROCESS_ERROR(pIniFile && pSection);
	bRet = KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, _STRINGER(m_nGetTimeGap), 0, &m_nGetTimeGap);
	KG_PROCESS_ERROR(bRet);
	bRet = KG_CUSTOM_HELPERS::LoadINIFileData<FLOAT>(pIniFile, pSection, _STRINGER(m_fGetRegionInfoTimeGap), 0, &m_fGetRegionInfoTimeGap);
	KG_PROCESS_ERROR(bRet);
	bRet = KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, "BroadcastWhenEncounterSameRegionCount", 0, &m_nBroadcastWhenEncounterSameRegionCount);
	KG_PROCESS_ERROR(bRet);
	bRet = KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, "CurRegion", 0, &m_nCurRegion);
	KG_PROCESS_ERROR(bRet);
	bRet = KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, "IgnoreRegionChange", 0, &m_bIgnoreRegionChange);//�����ɹ�
	return true;
Exit0:
	_KG3D_DEBUG_REPORT(_STRINGER(KG3DRegionInformerDescStruct::LoadMapSetting)_T("ʧ��"));
	*this = KG3DRegionInformerDescStruct();///�ظ�Ĭ��ֵ
	return false;
}

KG3DRegionInformerDescStruct::KG3DRegionInformerDescStruct()
{
	m_nGetTimeGap = 24;
	m_fGetRegionInfoTimeGap = 1800;///�������,��λ�Ǻ���
	m_nBroadcastWhenEncounterSameRegionCount = 2;
	m_nCurRegion = 1;
}

KG3DRegionInformerDescStruct::KG3DRegionInformerDescStruct( const KG3DRegionInformerDescStruct& OtherStruct )
{
	KG_CUSTOM_HELPERS::TTypeCopy<KG3DREGION_INFORMER_DESCSTRUCT>(this, &OtherStruct);		
}

KG3DRegionInformerDescStruct::KG3DRegionInformerDescStruct( const KG3DREGION_INFORMER_DESCSTRUCT& OtherStruct )
{
	KG_CUSTOM_HELPERS::TTypeCopy<KG3DREGION_INFORMER_DESCSTRUCT>(this, &OtherStruct);
}
void KG3DRegionInformerDescStruct::operator=( const KG3DRegionInformerDescStruct& OtherStruct )
{
	KG_CUSTOM_HELPERS::TTypeCopy<KG3DREGION_INFORMER_DESCSTRUCT>(this, &OtherStruct);
}

void KG3DRegionInformerDescStruct::ToBaseStruct( KG3DREGION_INFORMER_DESCSTRUCT* pBaseStruct )
{
	if (!pBaseStruct)
		return;
	KG_CUSTOM_HELPERS::TTypeCopy<KG3DREGION_INFORMER_DESCSTRUCT>(pBaseStruct, this);
}
