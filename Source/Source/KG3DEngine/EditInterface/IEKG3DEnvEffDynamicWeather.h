////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEKG3DEnvEffDynamicWeather.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-12-13 11:36:11
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IEKG3DENVEFFDYNAMICWEATHER_H_
#define _INCLUDE_IEKG3DENVEFFDYNAMICWEATHER_H_
#include "IEKG3DEnvironment.h"


typedef enum _KG3DENVEFFTI_MDO
{
	KG3DENVEFFTI_MDO_BOOST_FACTOR = 0x01,
	KG3DENVEFFTI_MDO_CUR_TIME = 0x02,
	KG3DENVEFFTI_MDO_ALL = 0x0000ffff,
}KG3DENVEFFTI_MDO;
typedef struct _KG3DENVEFFTI_DESCSTRUCT
{
	float m_fBoostFactor;
	DWORD m_dwCurHour;///����ֱ����ʱ����1��ʼ��24, �����0�Ļ�����ѭȫ�ֵ�ϵͳʱ��
	_KG3DENVEFFTI_DESCSTRUCT()
	{
		ZeroMemory(this, sizeof(_KG3DENVEFFTI_DESCSTRUCT));
	}
}KG3DENVEFFTI_DESCSTRUCT, *LPKG3DENVEFFTI_DESCSTRUCT;
//////////////////////////////////////////////////////////////////////////
typedef enum _KG3DENVEFFTI_ENUM
{
	KG3DENVEFFTI_MILLISECOND_IN_A_SECOND = 1000,
	KG3DENVEFFTI_SECOND_IN_A_MINUTE = 60,
	KG3DENVEFFTI_MINUTE_IN_AN_HOUR = 60,
	KG3DENVEFFTI_SECOND_IN_AN_HOUR = 3600,
	KG3DENVEFFTI_HOUR_IN_A_DAY = 24,
	KG3DENVEFFTI_HOUR_MAX = KG3DENVEFFTI_HOUR_IN_A_DAY,
	KG3DENVEFFTI_HOUR_MIN = 1,
	KG3DENVEFFTI_HOUR_INVALID = 0,
	KG3DENVEFFTI_TIME_RANGE = 86400, ///һ���������
	KG3DENVEFFTI_TIME_MAX = KG3DENVEFFTI_TIME_RANGE,
	KG3DENVEFFTI_TIME_MIN = 1,
	KG3DENVEFFTI_TIME_TOLERANCE = 60, ///����ݲ�/��
	KG3DENVEFFTI_MAX_BOOST_FACTOR = 1000,///86������һ��ѭ��
	KG3DENVEFFTI_GET_TIME_GAP = 60,///��Լ2��ŵ���һ��GetTime
}KG3DENVEFFTI_ENUM;
typedef enum _KG3DENVEFFRI_ENUM
{
	KG3DENVEFFRI_REGION_INVALID = 0,
	KG3DENVEFFRI_REGION_MIN = 1,
	KG3DENVEFFRI_REGION_COUNT_MAX = 255,
	KG3DENVEFFRI_REGION_MAX = 255,
};
typedef enum	_KG3DRI_MDO
{
	KG3DRI_MDO_GET_TIME_GAP = 0x01,
	KG3DRI_MDO_GET_REGIONINFOR_TIME_GAP  = 0x02,
	KG3DRI_MDO_CHANGE_WHEN_SAME_REGION_TIMES = 0x04,
	KG3DRI_MDO_CURRENT_REGION = 0x08,
	KG3DRI_MDO_ALL = 0xffffffff,
}KG3DRI_MDO;
struct KG3DREGION_INFORMER_DESCSTRUCT
{
	INT m_nGetTimeGap;///֡Ϊ��λ
	FLOAT m_fGetRegionInfoTimeGap;///����Ϊ��λ
	INT m_nBroadcastWhenEncounterSameRegionCount;
	INT m_nCurRegion;
	BOOL m_bIgnoreRegionChange;//�����Region�ı����Ϣ���㶨ʹ��m_nCurRegion
};
/************************************************************************/
/*IEKG3DEnvEffTimeInformer 
*/
/************************************************************************/
struct IEKG3DEnvEffTimeInformer 
{
	virtual HRESULT RequestModification(const KG3DENVEFFTI_DESCSTRUCT* pDescStruct, DWORD dwModificationOption) = 0;
	virtual HRESULT GetStruct(KG3DENVEFFTI_DESCSTRUCT* pDescStruct) = 0;
};
struct IEKG3DRegionInformer
{
	virtual HRESULT RequestModification(const KG3DREGION_INFORMER_DESCSTRUCT* pDescStruct, DWORD dwModificationOption) = 0;
	virtual HRESULT GetStruct(KG3DREGION_INFORMER_DESCSTRUCT* pDescStruct) = 0;
};
/************************************************************************/
/*IEKG3DEnvEffDynamicWeather 
*/
/************************************************************************/
typedef	enum _KG3DENVEFF_DYNWEATHER_MDO
{
	KG3DENVEFF_DYNWEATHER_MDO_TIME,
}KG3DENVEFF_DYNWEATHER_MDO;

typedef struct _KG3DENVEFF_DYNWEATHER_DESCSTRUCT 
{
	DWORD	m_dwModificationOption;
}KG3DENVEFF_DYNWEATHER_DESCSTRUCT, *LPKG3DENVEFF_DYNWEATHER_DESCSTRUCT;
//////////////////////////////////////////////////////////////////////////
//�ؼ�֡�ӿ�ר��
typedef struct _KG3DENVEFF_DWGCKEY_HANDLE 
{
	INT	m_Region;
	INT m_Hour;
#ifdef __cplusplus
	_KG3DENVEFF_DWGCKEY_HANDLE():m_Region(KG3DENVEFFRI_REGION_INVALID),m_Hour(KG3DENVEFFTI_HOUR_INVALID){}
	_KG3DENVEFF_DWGCKEY_HANDLE(INT Region, INT Hour):m_Region(Region),m_Hour(Hour){}
	inline bool operator==(const _KG3DENVEFF_DWGCKEY_HANDLE& Other){return 0 == memcmp(this, &Other, sizeof(_KG3DENVEFF_DWGCKEY_HANDLE));}
	inline bool operator!=(const _KG3DENVEFF_DWGCKEY_HANDLE& Other){return !operator==(Other);}
#endif	
}KG3DENVEFF_DWGCKEY_HANDLE;

//////////////////////////////////////////////////////////////////////////
struct IEKG3DEnvEffDWGC_KeyTable0
{
	virtual ULONG GetSize() = 0;
	virtual HRESULT GetType(DWORD* pdwType) = 0;
	virtual HRESULT GetElementIndices(INT* pnIndex, INT nArraySize, INT* pnRealWritedIndexCount) = 0;///����һ�����飬һ��д�����е�Index
	virtual HRESULT RemoveKey(INT nIndex) = 0;
	///�ؼ�֡����
	virtual HRESULT GetDescription(INT nIndex, LPTSTR pDesc, INT nBufferSize) = 0;
	virtual HRESULT KeyElementCount(INT* pSize) = 0;
	virtual HRESULT ConvertValueString(INT nWhichKey, INT nWhichPart, LPTSTR tczString, INT nBufferSize, BOOL bGetOrSet) = 0;
};

struct IEKG3DEnvEffDWGC_KeyTable1	//����ӿ�ֻ�ܶ�����д��Ҫд�Ļ���ͨ��DynamicWeather����
{
	virtual ULONG	GetSize() = 0;
	virtual HRESULT GetType(DWORD* pdwType) = 0;
	virtual HRESULT GetFirstKeyHandle(KG3DENVEFF_DWGCKEY_HANDLE* pHandle, LPVOID pExtraInfo) = 0;
	virtual HRESULT GetNextKeyHandle(KG3DENVEFF_DWGCKEY_HANDLE PrevHandle, KG3DENVEFF_DWGCKEY_HANDLE* pNextHandle, LPVOID pExtraInfo) = 0;
};
typedef struct IEKG3DEnvEffDWGC_KeyTable1 IEKG3DEnvEffDWGC_KeyTable;

//////////////////////////////////////////////////////////////////////////
typedef enum _KG3DENVEFF_DYNWEATHER_KEYLINE_MDO
{
	KG3DENVEFF_DYNWEATHER_KEYLINE_MDO_ADD_ROW,
	KG3DENVEFF_DYNWEATHER_KEYLINE_MDO_DEL_ROW,
	KG3DENVEFF_DYNWEATHER_KEYLINE_MDO_UP_ROW,
	KG3DENVEFF_DYNWEATHER_KEYLINE_MDO_DOWN_ROW,
	KG3DENVEFF_DYNWEATHER_KEYLINE_MDO_MODIFY_ROW,
}KG3DENVEFF_DYNWEATHER_KEYLINE_MDO;
//////////////////////////////////////////////////////////////////////////
struct IEKG3DEnvEffDynamicWeather 
{
	virtual HRESULT	RequestModification( const KG3DENVEFF_DYNWEATHER_DESCSTRUCT* pStruct ) = 0;///������pStruct�е�ModificationOption�趨��ʲô����
	virtual HRESULT	GetStruct( KG3DENVEFF_DYNWEATHER_DESCSTRUCT* pResultStruct ) = 0;

	//<��ǰʱ���Region����>
	virtual HRESULT GetTimeInformer(IEKG3DEnvEffTimeInformer** ppTI) = 0;
	virtual HRESULT GetRegionInformer(IEKG3DRegionInformer** ppRI) = 0;

	virtual HRESULT StartQuickPlay(INT nRegion, INT nStartHour, DWORD_PTR dwExtraParam) = 0;
	virtual HRESULT StopQuickPlay(DWORD_PTR dwExtraParam) = 0;

	virtual HRESULT EnablePlay(BOOL bEnable) = 0;	///����KeyEdit��ʱ��ȫ���������û����ɵ�Key

	virtual ULONG	GetCurHour() = 0;
	virtual HRESULT SetCurHour(INT Hour, BOOL bSetToOrBlendTo) = 0;	///BlendTo�Ļ����ܿ��������Ч��

	virtual ULONG	GetCurRegion() = 0;
	virtual HRESULT SetCurRegion(INT Region, BOOL bSetToOrBlendTo) = 0;	///BlendTo�Ļ����ܿ��������Ч��
	//</��ǰʱ���Region����>

	//<�µ�ʱ���߽ӿ�>
	//Region Index�Ĳ�������
	virtual HRESULT TL_GetKeyTable(KG3DENVEFF_HANDLE Handle, IEKG3DEnvEffDWGC_KeyTable** ppKeyTable) = 0;
	virtual HRESULT TL_AddRegionIndex(INT RegionIndex) = 0;
	virtual HRESULT TL_DelRegionIndex(INT RegionIndex) = 0;
	virtual HRESULT TL_ClearRegionIndices() = 0;
	virtual ULONG	TL_GetRegionIndicesCount() = 0;
	virtual HRESULT TL_ModifyRegionIndex(INT RegionIndex, INT NewIndex) = 0;
	virtual HRESULT TL_GetFirstRegionIndex(INT* pRegionIndex) = 0;
	virtual HRESULT TL_GetNextRegionIndex(INT PrevRegionIndex, INT* pNextRegionIndex) = 0;
	
	//Key�Ĳ�������
	virtual HRESULT	TL_DelKey(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE KeyHandle) = 0;
	virtual ULONG	TL_GetKeyCount(KG3DENVEFF_HANDLE Handle) = 0;
	virtual HRESULT	TL_GetFirstKeyHandle(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE* pHandle, LPVOID pExtraInfo) = 0;
	virtual HRESULT	TL_GetNextKeyHandle(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE PrevHandle, KG3DENVEFF_DWGCKEY_HANDLE* pNextHandle, LPVOID pExtraInfo) = 0;
	virtual HRESULT	TL_ClearKey(KG3DENVEFF_HANDLE Handle) = 0;
	virtual HRESULT	TL_GetFirstKeyHandleOfRegion(KG3DENVEFF_HANDLE Handle, INT Region, KG3DENVEFF_DWGCKEY_HANDLE* pHandle, LPVOID pExtraInfo) = 0;
	virtual HRESULT	TL_GetNextKeyHandleOfRegion(KG3DENVEFF_HANDLE Handle, INT Region, KG3DENVEFF_DWGCKEY_HANDLE PrevHandle, KG3DENVEFF_DWGCKEY_HANDLE* pNextHandle, LPVOID pExtraInfo) = 0;
	virtual HRESULT TL_GetPrevKeyHandle(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE KeyHandle, KG3DENVEFF_DWGCKEY_HANDLE* pPrevHandle) = 0;

	virtual HRESULT TL_CopyKeyTo(KG3DENVEFF_HANDLE SrcHandle, KG3DENVEFF_DWGCKEY_HANDLE KeySrc, KG3DENVEFF_HANDLE DescHandle, KG3DENVEFF_DWGCKEY_HANDLE KeyDesc, BOOL bByValueOrRef) = 0;

	//SetKey
	virtual HRESULT StateToKey(KG3DENVEFF_HANDLE EffHandle,  KG3DENVEFF_DWGCKEY_HANDLE KeyHandle) = 0;
	virtual HRESULT KeyToState(KG3DENVEFF_DWGCKEY_HANDLE KeyHandle, KG3DENVEFF_HANDLE EffHandle) = 0;

	//�򻯲����İ�������
	virtual ULONG	IsEffectDWGCAvailable(KG3DENVEFF_HANDLE EffHandle) = 0;
	//</�µ�ʱ���߽ӿ�>
};

#endif //_INCLUDE_IEKG3DENVEFFDYNAMICWEATHER_H_
