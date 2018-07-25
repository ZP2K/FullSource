////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DSkySystem.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-10-18 17:01:07
//  Comment     : ��ԭ����պеĹ��ֳܷ��������֣�SkyBox��Զɽ���ơ����������������ͳһ����
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DSKYSYSTEM_H_
#define _INCLUDE_KG3DSKYSYSTEM_H_

////////////////////////////////////////////////////////////////////////////////


#include "KG3DMesh.h"
#include "KG3DCloud.h"
#include "KG3DMeshTable.h"
#include "KG3DMeshFarMountain.h"
#include "IEKG3DSkySystem.h"
#include "KG3DEnvEffect.h"
#include "KG3DMeshSkyBox.h"
//#include "KG3DEnvEffDW_HelperClasses.h"
#include "KG3DEnvEffDW_KeyBase.h"

//#include "KG3DMessenger.h"
//#include "TemplateLibs/LOKI_APP/KG3DLokiStructInterface.h"
//using namespace std;

class KG3DTexture;
namespace KG3D_ENVIRONMENT_EFFECT
{	
//	struct KG3DDWGC_Key_SkySystem : public KG3DDWGC_KeyBase///DW����DynamicWeather��̬������GC����GraduallyChange���� 
//	{	
//	public:		
//		enum
//		{
//			MAX_KEYNAME_SIZE = 64,
//			MAX_KEYVALUE_SIZE = 64,
//		};
//		enum
//		{
//			MAX_SKYBOX = 1,
//			MAX_FARMOUNT = 10,
//			MAX_CLOUD = 10, 
//		};
//		typedef vector<INT> FARMOUNT_INDEX_ARRAY;
//
//		enum{ DWGCKEY_TYPE = KG3DTYPE_ENVEFF_SKYSYSTEM_SYSTEM,};
//		virtual DWORD GetType()const{return DWGCKEY_TYPE;}
//		//////////////////////////////////////////////////////////////////////////
//		DWORD	m_dwModificationOption;
//		INT m_nSkyBoxIndex;
//		FARMOUNT_INDEX_ARRAY m_FarMountIndexArray;
//		//////////////////////////////////////////////////////////////////////////
//
//		/*
//		typedef LOKI_APP::StructInterface::TElementConvertor<
//					TYPELIST_2(INT, FARMOUNT_INDEX_ARRAY)> ConverterType;
//				static ConverterType& GetConverter()
//				{
//					static MAKE_DESC_ARRAY_2(pDescArray, m_nSkyBoxIndex, m_FarMountIndexArray);
//					static ConverterType Convertor(pDescArray, _countof(pDescArray));
//					return Convertor;
//				}*/
//		
//	public:
//		KG3DDWGC_Key_SkySystem();
//		KG3DDWGC_Key_SkySystem(const KG3DDWGC_Key_SkySystem& OtherStruct);
//		KG3DDWGC_Key_SkySystem& operator=(const KG3DDWGC_Key_SkySystem& OtherStruct); 
//		
//		virtual bool CalculateInterp(const KG3DDWGC_KeyBase& BaseKeyBefore
//									, const KG3DDWGC_KeyBase& BaseKeyAfter
//									, float fPercent
//									, KG3DDWGC_KeyTableManagerBase* pTableManager = NULL);
//		bool SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
//		bool LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
//		//void ReleaseContainedKey(KG3DDWGC_KeyTableManagerBase* pTableManager, bool OnlyInterp);
//		//virtual KG3DDWGC_KeyBase* GetContainedKey(KG3DDWGC_KeyTableManagerBase& RefTableMgr, DWORD dwType, int nFinded) const;//(ע��ǿת�Ŀ�����)
//		//virtual INT GetContainedKeyIndex(KG3DDWGC_KeyTableManagerBase& RefTableMgr, DWORD dwType, int nFinded = 0) const;
//		//virtual bool SetContainedKeyIndex(KG3DDWGC_KeyTableManagerBase& RefTableMgr, DWORD dwType, int nFinded, INT nNewIndex);
//		//virtual int GetContainedKeyCount()const;
//		//virtual std::pair<DWORD, int> GetContainedKeyIndexByOrder(int nOrder)const;
//		//virtual bool SetContainedKeyIndexByOrder(KG3DDWGC_KeyTableManagerBase& RefTableMgr, int nOrder, INT nNewIndex);
//		//virtual bool IsLegal(KG3DDWGC_KeyTableManagerBase* pTableManager)const;
//		//virtual void CopyLegalPartsOfAnotherKey(const KG3DDWGC_KeyBase& OtherKeyBase, KG3DDWGC_KeyTableManagerBase& TableManager );
//#if defined(DEBUG) | defined(_DEBUG)
//		void DebugPrint(std::ostringstream& OSS);
//#endif
//
//	private://<DW_SUB_Frame>
//		inline void CalculateSkyBox(KG3DDWGC_KeyTableManagerBase& RefTableMgr, KG3DDWGC_Key_SkySystem &KeyBefore, KG3DDWGC_Key_SkySystem &KeyAfter, float fPercent );
//		inline void CalculateFarmount(KG3DDWGC_KeyTableManagerBase& RefTableMgr, KG3DDWGC_Key_SkySystem &KeyBefore, KG3DDWGC_Key_SkySystem& KeyAfter, float fPercent);
//	};
	
	class KG3DMeshLensFlare;
	class KG3DSkySystem : 
        public KG3DEnvEffect,
        public IEKG3DSkySystem
	{
	public:	
		enum { MAX_FARMOUNT = 10 };

    private:
        typedef std::vector<KG3DMeshFarMountain*> KG3DMeshFarMountainPtrVector;
        typedef std::vector<KG3DCloud*> KG3DCloudPtrVector;

	private:
		KG3DMeshFarMountainPtrVector    m_vecFarMounts;
		KG3DCloudPtrVector              m_vecClouds;
		vector<unsigned>				m_vecCloudRenderOrder;//Cloud��Ⱦ��ʱ����Ҫ���߶�����

		KG3DMeshSkyBox* m_lpSkyBox;

		///<��ͬ��>
		D3DCOLOR m_d3dc_FogColor;
		BOOL m_bEnableSynWithFogColor;
		///<�ӿ�>
	public:
        virtual HRESULT OnLostDevice();
        virtual HRESULT OnResetDevice();
		///<��պв���>
		virtual HRESULT GetSkyBox(IEKG3DMeshSkyBox** ppiMeshSkyBox);
		virtual HRESULT AddSkyBox(IEKG3DMeshSkyBox** ppiMeshSkyBox);
		virtual HRESULT DelSkyBox();
		///</��պв���>
		///<Զɽ�Ľӿڲ���>
		virtual HRESULT AddFarMount( const KG3DMESHFARMOUNTAIN_DESCSTRUCT* pDescStruct, IEKG3DMeshFarMountain** ppAddedFarMount);///�������NULL�û�����Ĭ�ϲ�������
		virtual HRESULT DelFarMount( int nIndex );
		virtual HRESULT DelFarMount( IEKG3DMeshFarMountain* pFarMount );
		virtual HRESULT UpperFarMount( int nIndex );
		virtual HRESULT LowerFarMount( int nIndex );
		virtual HRESULT ClearFarMountVec();
		virtual HRESULT GetFarMount( int nIndex, IEKG3DMeshFarMountain** ppFarMount );
		virtual HRESULT GetFarMountIndexByPointer( IEKG3DMeshFarMountain* pFarMout, int* pIndex );
		virtual HRESULT IsFarMountPointerValid( IEKG3DMeshFarMountain* pFarMout, BOOL* pBool );
		virtual HRESULT GetFarMountVecSize( int* pSize );
		///</Զɽ�Ľӿڲ���>
		///<�ƵĽӿڲ���>
		virtual HRESULT AddCloud();
		virtual HRESULT DelCloud( int nIndex );
		virtual HRESULT DelCloud( IEKG3DCloud* pCloud );
		virtual HRESULT ClearCloudVec();
		virtual HRESULT GetCloud( int nIndex, IEKG3DCloud** ppCloud );
		virtual HRESULT GetCloudIndexByPointer( IEKG3DCloud* pCloud, int* pIndex );///�õ�Vector�е�Index�����û�еĻ�����-1
		virtual HRESULT IsCloudPointerValid( IEKG3DCloud* pCloud, BOOL* pBool );///�ӷ���ֵ��pBool������֪���Ƿ���Ч
		virtual HRESULT GetCloudVecSize( int* pSize );
		///</�ƵĽӿڲ���>
		///</�ӿ�>
	public :
		///<Զɽ�Ĺ�����>//������ӿ��ص�
		HRESULT	AddFarMount( const KG3DMeshFarMountDescStruct* pDescStruct, KG3DMeshFarMountain** ppAddedFarMount);///�������NULL�û�����Ĭ�ϲ�������
		HRESULT	DelFarMount( KG3DMeshFarMountain* pFarMount );
		inline KG3DMeshFarMountain*	GetFarMount( int nIndex );
		int	GetFarMountIndexByPointer( KG3DMeshFarMountain* pFarMout );///�õ�Vector�е�Index�����û�еĻ�����-1
		BOOL	IsFarMountPointerValid( KG3DMeshFarMountain* pFarMout );
		inline int	GetFarMountVecSize();
		///</Զɽ�Ĺ�����>
		///<�ƵĹ�����>
		HRESULT	DelCloud(KG3DCloud* pCound);
		KG3DCloud*	GetCloud(int nIndex);
		inline	int	GetCloudVecSize();
		inline BOOL	IsCloudPointerValid(KG3DCloud* pCloud);
		inline	int	GetCloudIndexByPointer(KG3DCloud* pCloud);
		VOID	SortCloudRenderOrder();
		///</�ƵĹ�����>	

		///<KG3DEnvEffect����>
		virtual HRESULT FrameMove();
		virtual HRESULT Render(const KG3DEnvEffRenderContext& renderContext);
		virtual HRESULT SaveMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		virtual HRESULT LoadMapSetting(IIniFile* pIniFile, LPCTSTR pSection);
		virtual HRESULT ToSolidInterface(LPVOID* pEffectInterface);
		virtual KG3DEnvEffect* GetSubEffect(DWORD dwType, INT nIndex);
		virtual KG3DEnvEffect* GetSubEffect(INT nIndex, KG3DENVEFF_HANDLE* pHandle = NULL);
		virtual INT GetSubEffectCount();
		virtual INT GetSubEffectCount(DWORD dwSubType);
		virtual VOID GetSubEffectTypeList(std::vector<DWORD>& Types);
		virtual VOID GetExistSubEffectTypeList(std::vector<DWORD>& Types);

		///</KG3DEnvEffect����>	
	public:
		///<��������>
		HRESULT AddEnvEffect(DWORD dwType, KG3DEnvEffect** ppEnvEffect);
		///</��������>
	public:
		KG3DMeshSkyBox* AddSkyBox(IIniFile* pFile, LPCTSTR strSection);
		KG3DMeshSkyBox* GetSkyBox()
		{
			return m_lpSkyBox;
		}
		HRESULT ClearSkyBox()
		{
			return DelSkyBox();
		}
		///</CTH Added>
	private:///��������
		//static bool SortComp(KG3DCloud* pElem1, KG3DCloud* pElem2);
		//HRESULT CheckIsEnableSynWithFog(BOOL* pbEnable);
		//HRESULT EnableSynWithFogColor(BOOL bEnable, D3DCOLOR FogColor);
		HRESULT AddFarMountToVec(KG3DMeshFarMountain* pFarmount);
		/*
		template<class T>
				HRESULT	ForEach(T EffectOperator)
				{
					HRESULT hr = E_FAIL;
					bool bHasOneFail = false;
					KG3DMeshSkyBox* pSkyBox = this->GetSkyBox();
					if(pSkyBox)
					{
						hr = EffectOperator.Accept(*pSkyBox);
						if (FAILED(hr))
							bHasOneFail = true;
					}
					int nFarmountVecSize = this->GetFarMountVecSize();
					for (int i = 0; i < nFarmountVecSize; i++)
					{
						KG3DMeshFarMountain* pFarmount = this->GetFarMount(i);
						_ASSERTE(pFarmount);
						hr = EffectOperator.Accept(*pFarmount);
						if (FAILED(hr))
							bHasOneFail = true;
					}
					///<DW_SUBFrame>;
					KG_PROCESS_ERROR(! bHasOneFail);
					return S_OK;
		Exit0:
					return E_FAIL;
				}
				template<class T>
				inline HRESULT ForEachEx(T EffectOperator)
				{
					HRESULT hr = E_FAIL;
					bool bHasOneFail = false;
					KG3DMeshSkyBox* pSkyBox = this->GetSkyBox();
					if(pSkyBox)
					{
						hr = EffectOperator.Accept(*pSkyBox, KG3DENVEFF_HANDLE(KG3DTYPE_ENVEFF_SKYSYSTEM_SKYBOX, 0));
						if(FAILED(hr))
							bHasOneFail = true;
					}
					int nFarmountVecSize = this->GetFarMountVecSize();
					for (int i = 0; i < nFarmountVecSize; i++)
					{
						KG3DMeshFarMountain* pFarmount = this->GetFarMount(i);
						_ASSERTE(pFarmount);
						hr = EffectOperator.Accept(*pFarmount, KG3DENVEFF_HANDLE(KG3DTYPE_ENVEFF_SKYSYSTEM_FARMOUNTAIN, i));
						if(FAILED(hr))
							bHasOneFail = true;
					}
					///<DW_SUBFrame>;
					KG_PROCESS_ERROR(! bHasOneFail);
					return S_OK;
		Exit0:
					return E_FAIL;
				}
				template<class T>
				struct ForEachExHelper : public T
				{
					TypeKey& m_KeyRef;
					KG3DDWGC_KeyTableManagerBase& KeyTableRef;
					ForEachExHelper(const ForEachExHelper& Other):m_KeyRef(Other.m_KeyRef), KeyTableRef(Other.KeyTableRef){}
					ForEachExHelper(TypeKey& Key, KG3DDWGC_KeyTableManagerBase& KeyTable):m_KeyRef(Key), KeyTableRef(KeyTable){}
					ForEachExHelper(const TypeKey& Key, KG3DDWGC_KeyTableManagerBase& KeyTable):m_KeyRef(const_cast<TypeKey&>(Key)), KeyTableRef(KeyTable){}
					inline HRESULT Accept(KG3DEnvEffect& RefEnvEff, KG3DENVEFF_HANDLE EnvEffHandle)
					{
						return T::Accept(RefEnvEff, EnvEffHandle, m_KeyRef, KeyTableRef);
					}
				};*/
		
	public:	
		KG3DSkySystem();
		virtual ~KG3DSkySystem(void);
	};

};
#endif //_INCLUDE_KG3DSKYSYSTEM_H_
