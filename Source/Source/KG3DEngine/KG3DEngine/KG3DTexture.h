////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DTexture.h
//  Version     : 1.0
//  Creator     : huyi
//  Create Date : 2006-11-8 16:06:58
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DTEXTURE_H_
#define _INCLUDE_KG3DTEXTURE_H_

////////////////////////////////////////////////////////////////////////////////

#include "kg3dinterface.h"
#include "KG3DTypes.h"
#include "KG3DTemplates.h"
#include "IEKG3DTexture.h"
#include "KG3DMessenger.h"

enum
{
	TEXOP_COMMON = 0,
	TEXOP_ANIMATION = 1,
	TEXOP_MOVIE = 2,
	TEXOP_FLASH = 3,
	TEXOP_RENDERTARGET = 0x100,
	TEXOP_DYNAMIC = 0x200,	// ��̬����,��Ҫ���ڽ���
	TEXOP_RELOAD = 0x400,   // ����ʹ���Զ���������
	TEXOP_DISABLEMIPMAP = 0x1000,  //��ֹʹ��Mipmap
	TEXOP_DISABLEPOW2 = 0x2000,  //��ֹʹ�� pow2 load
	TEXOP_DISABLEDTX = 0x4000,  //��ֹʹ��dtx��ʽ
	TEXOP_DISABLESCALE = 0x8000,  //��ֹʹ����������
};

struct KG3DVideoMemTexture
{
	PVOID pUser;
	LPDIRECT3DTEXTURE9 pTexture;
	UINT  uID;
	DWORD dwLastTime;
	PVOID pMgr;

	HRESULT Init()
	{
		ZeroMemory(this,sizeof(KG3DVideoMemTexture));
		return S_OK;
	}

	HRESULT OnResourceUse();
	HRESULT OnFreeResource();

	~KG3DVideoMemTexture();
};

class KG3DTexture : public IEKG3DTexture
{
	friend class KG3DTextureTable;
	friend class KG3DVideoMemTextureTable;
public:
	D3DXIMAGE_FILEFORMAT m_eImageType;

    virtual HRESULT GetName(LPCTSTR *ppszName);
    virtual HRESULT GetID(DWORD *pdwID);
    
	virtual HRESULT OnLostDevice();
	virtual HRESULT OnResetDevice();

	virtual BOOL IsResourceReady();

    virtual HRESULT GetIEKG3DTexture(IEKG3DTexture **ppIEKG3DTexture);
    
	unsigned m_uOption;

protected:
	DWORD m_dwType;//���ͣ�SetType
	DWORD m_dwID;
	DWORD m_dwTextureType;
	BOOL m_bAutoGenMipmap;

	DWORD m_dwTimes;
	BOOL  m_bCanReload;		// �ܷ�����װ��
    
	enum DynamicState
	{
		STATE_DELAYLOAD,//�ӳټ���
		STATE_NONE,//û�м���
		STATE_SYSTEMMEM,//���ص��ڴ�����
		STATE_VIDEOMEM,//���ص��ִ�����
		STATE_LOADFAILED
    }m_eDynamicLoadState;
	
	KG3DTEXTURE_DESC	m_Desc;
	enum { MAX_DIVISOR_OF_BACKBUFFER = 4 };//��С�Ǻ󻺳��1/4

	DWORD m_dwRenderTarger;
	int m_nDim;

	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DTEXTURE9 m_pTextureSmall;
	LPDIRECT3DTEXTURE9 m_pTextureSys;
	LPDIRECT3DTEXTURE9 m_pTextureSysSmall;
	LPDIRECT3DTEXTURE9 m_pDiscard;

	KG3DVideoMemTexture* m_pVideoMemShare;

	LPDIRECT3DSURFACE9 m_pDepthStencilSurface;

	HRESULT _CheckOption(unsigned uOption,BOOL bInMultiThread);
	
	KG3DMessageSingleReceiverSender m_KG3DSRMessageSender;///���ڴ����Texture��ӵ���ߵĽ���,��ʱ��Ҫ����֪ͨӵ����Reload����Ϣ��Reload�����У�// Last modify by Chen Tianhong, 2006-11-12 17:39:58
	static bool _GetFileTime(LPCSTR cszFileName, FILETIME& FileTimeRet);	//���ڴ����ļ�ʱ��m_FileTime�ĳ�ʼ��,���ʧ�ܣ�����дFileTimeRet

public:
	FILETIME m_FileTime;
	ULONG    m_ulRefCount;//��Ʒʹ�ü���
	std::string m_scName;
	std::string m_scNameReal;//����

	KG3DTexture(void);
	virtual ~KG3DTexture(void);

	//////////////////////////////////////////////////////////////////////////
	//���ü���
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void **ppvObject);
	ULONG STDMETHODCALLTYPE ReleaseWithoutDelay();///����TextureTable��RELEASE_ATONCE������������ü���Ϊ0������ɾ����
	//////////////////////////////////////////////////////////////////////////
	//��ʼ��,�������
	virtual HRESULT Init();
	virtual HRESULT UnInit();
	//////////////////////////////////////////////////////////////////////////
	//����,����
	virtual HRESULT _LoadFromMemory( LPCVOID pSrcData, UINT SrcSize, unsigned uOption ,BOOL bRetryIfFailed);
    int _ProcessDelayLoad();
	virtual HRESULT LoadFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption);

	virtual HRESULT SaveToFile(const char cszFileName[],   unsigned uOption);
    virtual HRESULT SaveSysToFile(const char cszFileName[],   unsigned uOption);
	virtual HRESULT LoadFromFileEx(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, DWORD_PTR WParam, DWORD_PTR LParam)
	{
		return LoadFromFile(cszFileName, uFileNameHash, uOption);
	}
	virtual HRESULT RefreshOption(unsigned uOption);
	//////////////////////////////////////////////////////////////////////////
	//ID,type
	virtual DWORD GetID();
	virtual unsigned GetType();
	void SetID(DWORD dwID) { m_dwID = dwID;}
	void SetType(DWORD dwType){m_dwType = dwType;}
	//////////////////////////////////////////////////////////////////////////
	//�ڴ����
	virtual HRESULT PrepareRender(unsigned uOption);//ѹ������
	virtual HRESULT PostRender();//�ָ����ݣ��������������ʹ��
	virtual HRESULT GetDateInformation(int* pnMem,int* pnVedioMem,int* pnConMem,int* pnConVMem);//�õ�������Ϣ����������Ϊռ���ڴ��С���ִ��С��ѹ�����ڴ��С��ѹ�����ڴ��С
	virtual unsigned   GetLastUseTime();//��ѯ�ϴ�ʹ�õ�ʱ��
	virtual unsigned   GetUseFrequency();//��ѯʹ��Ƶ�ʣ�һ��ʱ�����ʹ�ô���

	virtual HRESULT Update(unsigned uMethod, DWORD dwTime);//������ͼ,�Զ�����ͼ������
	virtual HRESULT Update(int nFrame);//������ͼ

	virtual HRESULT GetSize(unsigned *puWidth, unsigned *puHeight);
	virtual std::pair<UINT, UINT>	GetSize();
	virtual LPDIRECT3DTEXTURE9 GetTexture();	//û��AddRef�������������ǧ����AddRef�����еط����ǰ�û��AddRef����ʽд��

	virtual HRESULT CheckLoadState();

	VOID	PreLoad();//ֻ��Managed���͵Ĳ���PreLoad
	HRESULT ForceReload();
	HRESULT UpdateTexture();
	HRESULT SetTexture(LPDIRECT3DDEVICE9 pDevice, int i);


	bool  EvictResources( );
public:
	BOOL IsFileChanged();
	D3DFORMAT GetFormat();
	IKG3DMessageSenderBase* GetMessageSender(){ return &m_KG3DSRMessageSender; };///����������Ǳ��뱩¶�ڽӿ��е�
	HRESULT GetSurfaceDesc(UINT dwLevel, D3DSURFACE_DESC* pDesc);

	HRESULT ColorFill(CONST RECT* pRect, D3DCOLOR color);//��չ��DX��ColorFill��Rect����ΪNULL��ֻ֧��RenderTarget����D3DFMT_A8R8G8B8��D3DFMT_X8R8G8B8
	HRESULT SetAsRenderTarget(LPDIRECT3DDEVICE9 pDevice, DWORD RTIndex);	//��RT�İ����࣬�����GetSurface
	HRESULT SetAsDelphStencil(LPDIRECT3DDEVICE9 pDevice);
	HRESULT StretchRect(KG3DTexture& DestTex, CONST RECT* pSrcRect, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE FilterType);//��D3D��һ�£��ѵ�ǰ��ͼ����������һ����ͼ��ֻ����0��
protected:
	virtual HRESULT _OnReferenceZero(DWORD dwReleaseOpion = RELEASE_INQUEUE);
	//���ָ��uBackBufferSizeDividing�������Backbuffer����,1���Ǻ󻺳��С��
	//�󻺳�ƥ������Ҫ�����������Ӳ��Width��Height�ģ���Ϊ�󻺳��С��ı�
	HRESULT CreateTexture(unsigned int uWidth,
		unsigned int uHeight,
		D3DFORMAT format,
		TEXTYPE	texType = TEXTYPE_COMMON,	//0��Ĭ�ϣ������ο�ö��
		UINT uBackBufferSizeDividing = 0);
private:
	D3DXIMAGE_FILEFORMAT _GetFileTypeFromName( const char* strFileName );
	HRESULT _CreateTexturePrivate(BOOL bUseDevice, UINT Width,UINT Height,UINT MipLevels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool, UINT uBackBufferSizeDividing);

    HANDLE m_hFileLoader;
};

class KG3DClip;
extern int g_nNumTextureDynamicLoadedFrame;
extern int g_nNumAllowTextureDynamicLoadFrame;
//extern std::set<KG3DClip*> g_setMultiThreadReadAni;
//extern KG3DClip* g_pCurrentAni;

void g_MultiThreadLoadProgress(void * pInput);
void g_MultiThreadLoadProgressTerrain(void * pInput);

#ifdef KG3DENGINE_DISABLE_MULTITHREAD
void UpdateMultiThreadModelProgress(void* pInput);
#endif

#endif //_INCLUDE_KG3DTEXTURE_H_
