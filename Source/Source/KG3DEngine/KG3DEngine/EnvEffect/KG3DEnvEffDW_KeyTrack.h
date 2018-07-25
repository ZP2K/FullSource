////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEnvEffDW_KeyTrack.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-4-28 14:22:09
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DENVEFFDW_KEYTRACK_H_
#define _INCLUDE_KG3DENVEFFDW_KEYTRACK_H_

////////////////////////////////////////////////////////////////////////////////
#include "IEKG3DEnvironment.h"
#include "IEKG3DEnvEffDynamicWeather.h"
#include "KG3DMessenger.h"
#include "KG3DEnvEffDW_BaseClasses.h"

#include "KG3DEnvEffDW_HelperClasses.h"

////////////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*
������Ҫ�ı���඼Ҫ��KG3DEnvEffect�������࣬����SkyBox��FarMountain��
Cloud���Ѿ�����ˣ����������ǵĹ�����SkySystem��ͳһ��Ϊһ��EnvEffect����

ÿ���඼��һ����ص� DWGC_Key
DW����DynamicWeather��̬������GC����GraduallyChange����

���Key���ڽ��䣬����ṹ�������ڲ�����Desc_Struct�����ṹ��ȽϴӶ��õ�
�������ݡ���ʵ����ֱ��ʹ�������ṹ��ģ����Ƕ�һ������ṹ��Ļ���
���Լ��ٴ���ռ䣬���ҿ������ƿ��Խ���Ĳ�������Զɽ���֣�����������
�ĸ߶Ȼὥ�䣬���Խ��������Ӧ�����Ƶġ�

ÿһ������ṹ���е�DWORD����Ϊһ�������֣���Ϊ�����������Ժ��������
�����е�Struct��ָ������Struct��Index��������Invalid�ģ����ΪInvalid�����ʾû
�йؼ�֡��
*/
/************************************************************************/

/************************************************************************/
//ʱ�������ʽ���� 
/************************************************************************/

///����Ҫͳһ�����namespace
namespace KG3D_ENVIRONMENT_EFFECT
{
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	class KG3DDWGC_KeyTrackTester;
	class KG3DDWGC_KeyTrack
	{
		friend class KG3DDWGC_KeyTrackTester;
	public:
		typedef std::vector<KG3DDWGC_KeyTableGeneratorBase*> TypeTableGens;
		typedef std::set<INT>	TypeRegionIndexSet;
		enum{all_enveff_count = KG3DTYPE_ENVEFF_END - KG3DTYPE_ENVEFF_BEGIN,};
		static size_t GetVecPos(DWORD dwType){return dwType - KG3DTYPE_ENVEFF_BEGIN;}
	private:
		TypeTableGens	m_vecTableGens;	//�����Table���������������������N��Table
		TypeRegionIndexSet	m_setRegionIndex;
		KG3DEnvEffListBase&	m_EnvEffList;
	protected:
		VOID AddKeyTables();
		KG3DDWGC_KeyTrack();//EnvEffList��Ҫ�����õ�KeyTable�������RunTimeTest�õĻ������ڲ�����Table
		KG3DDWGC_KeyTableBase* GetTable(KG3DENVEFF_HANDLE Handle);
		KG3DDWGC_KeyTableGeneratorBase* GetTableGen(DWORD dwType);
		std::tstring&	GetSaveSectionName();
		size_t GetAllTableCount();
	public:
		KG3DDWGC_KeyTrack(KG3DEnvEffListBase& List);
		~KG3DDWGC_KeyTrack();
		
		VOID	GetTypesOfAllExistTables(std::vector<DWORD>& vecResult);
		IEKG3DEnvEffDWGC_KeyTable* GetTableInterface(KG3DENVEFF_HANDLE Handle){return GetTable(Handle);}//IEKG3DEnvEffDWGC_KeyTable��һ���˻����棬ֻ��
		bool	IsRegionAllowed(INT Region){return m_setRegionIndex.end() != m_setRegionIndex.find(Region);}
		//Region����
		HRESULT AddRegionIndex(INT RegionIndex);
		HRESULT DelRegionIndex(INT RegionIndex);
		HRESULT ClearRegionIndices();
		ULONG	GetRegionIndicesCount(){return static_cast<ULONG>(m_setRegionIndex.size());}
		HRESULT ModifyRegionIndex(INT RegionIndex, INT NewIndex);

		HRESULT GetFirstRegionIndex(INT* pRegionIndex);
		HRESULT GetNextRegionIndex(INT PrevRegionIndex, INT* pNextRegionIndex);
		

		//Key����
		KG3DDWGC_KeyBase* NewKey(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE KeyHandle);
		KG3DDWGC_KeyBase* GetKey(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE KeyHandle);
		VOID			DelKey(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE KeyHandle);
		ULONG			GetKeyCount(KG3DENVEFF_HANDLE Handle);
		HRESULT			GetFirstKeyHandle(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE* pHandle, LPVOID pExtraInfo);
		HRESULT			GetNextKeyHandle(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE PrevHandle, KG3DENVEFF_DWGCKEY_HANDLE* pNextHandle, LPVOID pExtraInfo);
		HRESULT			GetFirstKeyHandleOfRegion(KG3DENVEFF_HANDLE Handle, INT Region, KG3DENVEFF_DWGCKEY_HANDLE* pHandle, LPVOID pExtraInfo);
		HRESULT			GetNextKeyHandleOfRegion(KG3DENVEFF_HANDLE Handle, INT Region, KG3DENVEFF_DWGCKEY_HANDLE PrevHandle, KG3DENVEFF_DWGCKEY_HANDLE* pNextHandle, LPVOID pExtraInfo);
		HRESULT			GetPrevKeyHandle(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE KeyHandle, KG3DENVEFF_DWGCKEY_HANDLE* pPrevHandle);

		VOID			ClearKey(KG3DENVEFF_HANDLE Handle);

		//Key��ֵ
		std::auto_ptr<KG3DDWGC_KeyBase>		GetInterpKey(KG3DENVEFF_HANDLE Handle, KG3DENVEFF_DWGCKEY_HANDLE KeyHandle);//ע��AutoPtr���÷�
		std::auto_ptr<KG3DDWGC_KeyBase>		NewAKey(KG3DENVEFF_HANDLE Handle);//��������

		//Copy
		HRESULT			CopyTo(KG3DENVEFF_HANDLE SrcHandle, KG3DENVEFF_DWGCKEY_HANDLE KeySrc, KG3DENVEFF_HANDLE DescHandle, KG3DENVEFF_DWGCKEY_HANDLE KeyDesc, BOOL bByValueOrRef);

		//Load Save
		HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR);
		HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR);
	};
	
	
};


#endif //_INCLUDE_KG3DENVEFFDW_KEYTRACK_H_
