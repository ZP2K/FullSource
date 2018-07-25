////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DRegionInfoManager.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-3-27 19:00:53
/*  Comment     : ��������Ҫ��;�����ڱ��ǰ����Region�Լ�ȡ��Region��
�����Ϣ��ͬʱҲ�������趨Region�������Ϣ��
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DRegionInfoManager_H_
#define _INCLUDE_KG3DRegionInfoManager_H_
#include "IEKG3DRegionInfoManager.h"
#include "KG3DTemplates.h"


////////////////////////////////////////////////////////////////////////////////

class KG3DTexture;
namespace KG_CUSTOM_HELPERS
{
	class ArrayCompressor;
};
class KG3DRegionInfoManager : public IKG3DRegionInfoManager, public IEKG3DRegionInfoManager, public KG3D_CUSTOM_HELPERS::KG3DLifeTimeDependedClassBase<1>
{
public:
	friend class KG3DRegionInfoManagerTable;
	enum
	{
		TEX_MAX_SIZE = 4096,
	};

	//<ResourceBase>
public:
	KG3DRegionInfoManager(void);
	virtual ~KG3DRegionInfoManager(void);
public:///ģ��Ҫ�õ���
	ULONG m_ulRefCount;
	DWORD m_dwID;
	std::string m_scName;
	//</ResourceBase>
public:
	
	//���ü���
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void **ppvObject);
	//ID,type
	virtual DWORD GetID(){return m_dwID;}
	virtual unsigned GetType(){return KG3DTYPE_REGION_INFO_MANAGER;}
	void SetID(DWORD dwID) { m_dwID = dwID;}
	void SetType(DWORD dwType){NULL;}
	//��ʼ��,�������
	virtual HRESULT Init();
	virtual HRESULT UnInit();
	//////////////////////////////////////////////////////////////////////////
	//����,����
	virtual HRESULT LoadFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption);
	virtual HRESULT SaveToFile(const char cszFileName[],   unsigned uOption);
	virtual HRESULT LoadFromFileEx(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, DWORD_PTR WParam, DWORD_PTR LParam);
	virtual HRESULT RefreshOption(unsigned uOption);
	//////////////////////////////////////////////////////////////////////////
	//�ڴ����
	virtual HRESULT PrepareRender(unsigned uOption);//ѹ������
	virtual HRESULT PostRender();//�ָ����ݣ��������������ʹ��
	virtual HRESULT GetDateInformation(int* pnMem,int* pnVedioMem,int* pnConMem,int* pnConVMem);//�õ�������Ϣ����������Ϊռ���ڴ��С���ִ��С��ѹ�����ڴ��С��ѹ�����ڴ��С
	virtual unsigned   GetLastUseTime();//��ѯ�ϴ�ʹ�õ�ʱ��
	virtual unsigned   GetUseFrequency();//��ѯʹ��Ƶ�ʣ�һ��ʱ�����ʹ�ô���
	//////////////////////////////////////////////////////////////////////////
	static KG3DREGION_INFORMER_LOADFILE_PARAM GetDefaultLoadFileStruct();
	INT GetHeight();
	INT GetWidth();
	BOOL GetData(INT nX, INT nZ, INT& nRet);///��ͼ����Ϊ0����й���ͼ���ϵ����µ�ת����
	//HRESULT LoadImageWithDefaultParam(const TCHAR cszFileName[], DWORD_PTR uFileNameHash);
	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT IELoadFromFile(const TCHAR cszFileName[], DWORD_PTR uFileNameHash, DWORD_PTR uOption);
	virtual HRESULT GetRegionTexState(BOOL* pbIsExist, INT* pnID, LPSTR czPathBuffer, INT nBufferSize);
	virtual HRESULT GetDiffArrayCount(INT* pnCount);
	virtual HRESULT GetDiffArray(DWORD* pBuffer, INT nBufferSize);
	virtual HRESULT GetDefaultParam(KG3DREGION_INFORMER_LOADFILE_PARAM* pParam);
	virtual HRESULT GenerateRegionTex(DWORD_PTR dwExtraParam, DWORD* pdwTexID);
	virtual HRESULT GetPath(LPSTR pPathBuffer, INT nBufferSize);
	virtual HRESULT Save(const TCHAR cszFileName[], DWORD_PTR uOption);
	virtual HRESULT GetState(KG3DREGION_INFORMER_STATE* pState);//�õ�һЩ���Ա���
	virtual HRESULT IEGetState(KG3DREGION_INFORMER_STATE* pState){return GetState(pState);}//IE�İ汾
	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT GetRegionInfoData(D3DXVECTOR3 vecPos, DWORD* pdwData);
public:
	HRESULT SetTexture(UINT uStage);
#if defined(DEBUG) | defined(_DEBUG)
	void DebugPrintCompresedData();
#endif
private:
	HRESULT LoadImage(const TCHAR cszFileName[], DWORD_PTR uFileNameHash, DWORD_PTR uOption);
	BOOL AnalizeTexture(const TCHAR cszFileName[], KG3DTexture* pTex, INT nTargetWidth
						, INT nTargetHeight, KG3DREGION_INFORMER_LOADFILE_PARAM* pParam);
	BOOL GenerateTex(LPCTSTR lpPath  = NULL);
private:
	KG_CUSTOM_HELPERS::ArrayCompressor* m_pArrayCompressor;
	KG3DTexture* m_pTexture;
	BOOL m_bHasSuccessfullyLoaded;
	std::tstring*	m_pstTexPath;					//���ڹ���һ����ͼ·��������һ��Ԥ���õ���ͼ
	UINT			m_uHowManyPixelsInOneRegion;	//�������󾫶ȣ�����Ĭ����1��
	INT				m_nRegionXCount;
	INT				m_nRegionZCount;			//���ڴ洢Region�Ĵ�С�����ٴ�graphicTool��ȡScene

};
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*����趨��Ϊ�˴��������ܳ��ֵĶ�Manager����� 
*/
/************************************************************************/
class KG3DRegionDWInfoManager : public KG3DRegionInfoManager
{
public:
	static const char* GetDefaultMapName();
};
//////////////////////////////////////////////////////////////////////////
class KG3DRegionInfoManagerTable : public TKG3DResourceManagerBase<KG3DRegionInfoManager>, public IEKG3DRegionInfoManagerTable
{
private:
	enum
	{
		max_element_count = 100, 
	};
	typedef std::map<DWORD, IKG3DRegionInfoManager*> TypeExtraMap;///�����ResourceMap������һ�����Դ�Iָ��
	TypeExtraMap* m_pMapExtraRecord;
public:
	KG3DRegionInfoManagerTable();
	~KG3DRegionInfoManagerTable();
public:
	virtual HRESULT Get1NewResourcePointer(KG3DRegionInfoManager** ppT,DWORD Type,DWORD_PTR Option);
	virtual HRESULT GetTypeInfoByFileName(TypeInfo** ppTypeInfo,const char cszFileName[]);
	virtual HRESULT Init();
	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT GetCount(INT* pnRet);
	virtual HRESULT GetFirstRIM(IEKG3DRegionInfoManager** ppRIM, DWORD* dwFirstHandle, DWORD* pdwNextHandle);///ʹ��Handle���ǽӿ��ǿ�ѡ��
	virtual HRESULT GetRIM(DWORD dwHandle, IEKG3DRegionInfoManager** ppRIM, DWORD* pdwNextHandle);
	virtual HRESULT Add(const TCHAR cszFileName[]
						, unsigned uFileNameHash
						, unsigned uOption
						, DWORD_PTR WParam
						, DWORD_PTR LParam
						, IEKG3DRegionInfoManager** ppRIM
						, DWORD* pdwHandle);///uOption��KG3DREGION_INFORMER_LOADFILE_PARAM*
	virtual HRESULT Del(DWORD dwHandle);

	// Inherited via TKG3DResourceManagerBase
	virtual HRESULT LoadResourceFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, IKG3DResourceBase ** ppRetResource, bool bSyncLoad = true, ResNodeState ** res_node_state = NULL) override;
	virtual HRESULT LoadAnimationFromFile(const char cszFileName[], void ** ppRet) override;
	virtual HRESULT UnloadAnimation(void * pResource) override;
	virtual HRESULT LoadTagAnimationFromFile(const char cszFileName[], void ** ppRet) override;
	virtual HRESULT UnloadTagAnimation(void * pResource) override;
	virtual HRESULT LoadResourceFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, HANDLE * pRetHandle) override;
	virtual MODEL_LOAD_STATE GetResourceLoadState(HANDLE hResourceHandle) override;
	virtual HRESULT GetResourceByHandle(HANDLE hResourceHandle, IKG3DResourceBase ** ppRetResource) override;
	virtual void CloseResourceHandle(HANDLE hResourceHandle) override;
};
extern KG3DRegionInfoManagerTable g_cRegionInfoManagerTable;
//////////////////////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG)
	void g_DebugRegionInformerPrintCompressedData(KG3DRegionInfoManager* pRI);
#endif



#endif //_INCLUDE_KG3DRegionInfoManager_H_
