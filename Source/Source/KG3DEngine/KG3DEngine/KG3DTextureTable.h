////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DTextureTable.h
//  Version     : 1.0
//  Creator     : huyi
//  Create Date : 2006-11-9 9:14:04
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DTEXTURETABLE_H_
#define _INCLUDE_KG3DTEXTURETABLE_H_

////////////////////////////////////////////////////////////////////////////////

#include "kg3dinterface.h"
#include "KG3DTemplates.h"
#include "IEKG3DTextureTable.h"
#include "KG3DTexture.h"

struct USIZE
{
    UINT x;
    UINT y;

    USIZE() : x(0), y(0)
    {
    }

    USIZE(UINT _x, UINT _y) : x(_x), y(_y)
    {
    }
};

struct KG3DDetail_Define
{
	D3DCOLORVALUE vBodyColor;
	D3DCOLORVALUE vBodyColor1;
	float         fMax;
	float         fMin;
	float         fScale;
	float         fEnv;
	float         fSpecular;
	string        szName;

	D3DCOLORVALUE vColor0     ;
	string        szFileName0 ;
	float         fColorScale0;
	float         fUVScale0   ;
	float         fEnv0       ;
	float         fSpecular0  ;

	D3DCOLORVALUE vColor1     ;
	string        szFileName1 ;
	float         fColorScale1;
	float         fUVScale1   ;
	float         fEnv1       ;
	float         fSpecular1  ;

	D3DCOLORVALUE Flow_Color;
	float         Flow_ColorScale;//����Ч������
	float         Flow_UVScale;
	D3DXVECTOR2   Flow_Speed;
	string        Flow_Texture;

	KG3DDetail_Define()
	{
		UnInit();
	}

	HRESULT UnInit()
	{
		vBodyColor.r = vBodyColor.g = vBodyColor.b = vBodyColor.a = 1;
		vBodyColor1 = vBodyColor;
		fMax = 1;
		fMin = 0;
		fScale = 1;
		fEnv = 1;
		fSpecular = 1;

		vColor0.r    = vColor0.g = vColor0.b = vColor0.a = 1;
		fColorScale0 = 1;
		fUVScale0    = 1;
		fEnv0        = 0;
		fSpecular0   = 0;
		vColor1.r    = vColor1.g = vColor1.b = vColor1.a = 1;
		fColorScale1 = 1;
		fUVScale1    = 1;
		fEnv1        = 0;
		fSpecular1   = 0;

		Flow_Color = vBodyColor;
		Flow_ColorScale = 0;
		Flow_UVScale =1;
		Flow_Speed = D3DXVECTOR2(1,1);
		Flow_Texture = "";
		return S_OK;
	}
};



class KG3DVideoMemTextureTable
{
protected:
	typedef TKG3DResourceLimitSharePool<KG3DVideoMemTexture>VIDEOMEMPOOL;
	map<KG3DTEXTURE_DESC,VIDEOMEMPOOL*>m_mapTextures;

public:
	KG3DVideoMemTextureTable();
	~KG3DVideoMemTextureTable();

	HRESULT Init();
	HRESULT UnInit();

	KG3DVideoMemTexture* RequestResource(KG3DTexture* pUser);
	HRESULT  FreeResource(KG3DTexture* pUser,KG3DVideoMemTexture* Resource);

};

class KG3DTextureTable :
	public TKG3DResourceManagerBase<KG3DTexture>, public IEKG3DTextureTable
{
private:
	enum enuSpecialTextureIndex
	{
		STI_INVALIDATE,
		STI_ERROR,
		STI_NULL,
		STI_RENDERTARGET,
		STI_CONVERMAP,
		STI_COUNT,
		STI_FORCE_DWORD = 0xfffffff,
	};
public:
	enum enuSpecialTextureID
	{
		STID_INVALIDATE   = 0xffffffff,
		STID_ERROR        = 0xfffffffe,
		STID_NULL         = 0xfffffffd,
		STID_RENDERTARGET = 0xfffffffc,
	};
    virtual HRESULT UnInit();
	virtual HRESULT Init();
	virtual HRESULT ReloadFromFile();
    virtual HRESULT AutoReload();
    virtual HRESULT GetIEKG3DTexture(IEKG3DTexture** ppTexture, DWORD dwID);
    virtual HRESULT IELoadResourceFromFile(
        const CHAR cszFileName[], 
        DWORD uFileNameHash,
        DWORD uOption, 
        IKG3DResourceBase **ppRetResource
    );
	static LPDIRECT3DCUBETEXTURE9	GetDefaultEnvMap();
	static LPCTSTR	GetDefaultEnvMapName();

	KG3DVideoMemTextureTable m_VideoMemTextureMgr;
public:
	virtual HRESULT ReportResourceUse(LPSTR pTableName);

	KG3DTextureTable(void);
	virtual ~KG3DTextureTable(void);

	HRESULT SetTexture(DWORD Index, DWORD TextureID);

	HRESULT CreateTexture(
        unsigned int uWidth,
		unsigned int uHeight,
		D3DFORMAT format,
		TEXTYPE	texType,	//0��Ĭ�ϣ������ο�ö��
		KG3DTexture** ppTexture,
		LPCTSTR	fileNameForHash = NULL	//Ĭ�ϲ��á���FileNameForHash������ID�����а취��һ��ID���ⲿ��Ϊ��ͼ���
    );//���ʹ��RenderTarget�Ļ�����GetRenderTargetTexture�ȽϺã���֤û���ظ����룬�����ظ�ʹ��
	/*
	1.���Ҫ����ͬ��С����ͬ��ʽ��RT���ͷ�����ͬ�ģ���AddRef�����D3D��������һ�µ�

	2.�����������Ĵ�RT��������ռ�Դ档Ҫ�����ַ�Hash�����Բ�ҪƵ������
	UINT uSizeIsFractionOfBackbufferSize������ˣ���ôWidth��Height��Ч����С�����ֵȷ����ȡBackBuffer�������ֵ
	�Ĵ�С��Ϊ��С��1����BackBuffer�Ĵ�С

	3.���һ���������ڵ�Ŀ�ģ�BackBuffer��С�ǻ��ģ�����ù̶��Ĵ�С���õ�RenderTarget��RT����
	��BackBuffer�ı䲢�豸�ָ���ʱ������򵥵Ļָ�ԭ��С����ͼ����ô�ͻ����RT�����ã�
	����RT�˷ѡ���������������RT�����uSizeIsFractionOfBackbufferSize��1����ô�ͻ��Զ���
	�豸�ָ���ʱ�򣬻��Զ��ָ�һ����BackBufferһ�����RT��
	*/
	HRESULT GetRenderTargetTexture(UINT Width, UINT Height, D3DFORMAT format, KG3DTexture** ppTexture, UINT uSizeIsFractionOfBackbufferSize = 0);


	virtual KG3DTexture* GetRenderTargetTexture(UINT uWidth, 
		UINT uHeight, 
		D3DFORMAT Format);
	KG3DTexture* GetNullTexture();
	KG3DTexture* GetConverTexture();   
	////////////////////////////////////////////////
	//	resource manager
	bool  Resources( DWORD TextureID );
	void  EvictResources( DWORD videoMax );

    LPDIRECT3DTEXTURE9 GetChessboardTexture(UINT uWidth, UINT uHeight);
	//HRESULT FindResource(KG3DTexture** ppT, DWORD dwID);
	HRESULT GetResource(KG3DTexture** ppT, DWORD dwID);

	//////////////////////////////////////////////////////////////////////////
	HRESULT Detail_GetBodyColor(int nTable,int nIndex,D3DCOLORVALUE& vBodyColor,D3DCOLORVALUE& vBodyColor1,float& fMax,float& fMin,float& fScale,float& fEnv,float& fSpecular,LPSTR pName,
		D3DCOLORVALUE& vColor0,float& fColorScale0,float& fUVScale0,float& fDetail0_Env,float& fDetail0_Specular,LPSTR pFileName0,
		D3DCOLORVALUE& vColor1,float& fColorScale1,float& fUVScale1,float& fDetail1_Env,float& fDetail1_Specular,LPSTR pFileName1,
		D3DCOLORVALUE* pFlow_Color,float* pfFlow_ColorScale,float* pfFlow_UVScale,D3DXVECTOR2* pvSpeed,LPSTR pFlowTexture
		);
	HRESULT Detail_SetBodyColor(int nTable,int nIndex,D3DCOLORVALUE  vBodyColor,D3DCOLORVALUE  vBodyColor1,float  fMax,float  fMin,float  fScale,float  fEnv,float  fSpecular,LPSTR pName,
		D3DCOLORVALUE  vColor0,float  fColorScale0,float  fUVScale0,float fDetail0_Env,float fDetail0_Specular,LPSTR pFileName0,
		D3DCOLORVALUE  vColor1,float  fColorScale1,float  fUVScale1,float fDetail1_Env,float fDetail1_Specular,LPSTR pFileName1,
		D3DCOLORVALUE Flow_Color,float fFlow_ColorScale,float fFlow_UVScale,D3DXVECTOR2 vSpeed,LPSTR pFlowTexture);

	HRESULT LoadDetailDefines();
	HRESULT SaveDetailDefines();

	virtual int Detail_GetDefineTableCount();
	virtual HRESULT Detail_GetDefineTableName(int nTable,LPSTR pName);
	virtual HRESULT Detail_SetDefineTableName(int nTable,LPSTR pName);

	virtual HRESULT Detail_AddDefineTable();
	virtual HRESULT Detail_DeleteDefineTable(int nTable);

	virtual int Detail_GetBodyDefineCount(int nTable);

	virtual HRESULT Detail_AddBodyDefine(int nTable,int nIndex);
	virtual HRESULT Detail_DeleteBodyDefine(int nTable,int nIndex);

	virtual HRESULT OnLostDevice();

protected:
	

	struct DetailTable
	{
		string        szName;
		vector<KG3DDetail_Define*>m_vecDetailColorDefine;

		DetailTable()
		{
			m_vecDetailColorDefine.resize(128,NULL);
		}
	};
	vector<DetailTable>m_vecDetailTable;

	HRESULT UnInitDetailTables();
	//////////////////////////////////////////////////////////////////////////
	DetailTable& GetDetailTable(int nTable);

	HRESULT Get1NewResourcePointer(KG3DTexture** ppT,DWORD Type,DWORD_PTR Option);
    std::map<USIZE, LPDIRECT3DTEXTURE9> m_mapChessboardTextures;
	KG3DTexture *m_SpecialTextures[STI_COUNT];

	struct  TypeRenderTargetDesc
	{
		UINT Width;
		UINT Height;
		D3DFORMAT	Format;
	};
	
	std::vector<TypeRenderTargetDesc>	m_vecRenderTargetDesc;//�Ѳ�ͬ��С��RT������������Զ������ͬ��С��Format��RT
	TypeRenderTargetDesc m_CurrentRenderTargetDesc;
	static TCHAR* s_strSpecialTextureFilePath[STI_COUNT];
	static enuSpecialTextureID s_SpecialTextureID[STI_COUNT];

public:
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

extern KG3DTextureTable g_cTextureTable;
extern TKG3DResourcePool<KG3DDetail_Define>g_cDetailDefinePool;
#endif //_INCLUDE_KG3DTEXTURETABLE_H_
