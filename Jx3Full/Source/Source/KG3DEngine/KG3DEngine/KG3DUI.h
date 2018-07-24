#ifndef KG3D_UI_H
#define KG3D_UI_H

#include "../../../Include/KG3DEngine/KG3DInterface.h"
#include "./KG3DUIVBuffer.h"
#include "./KG3DUITexture.h"
#include "./KG3DTexture.h"
#include "./KG3DTextureTable.h"

//#define ENABLE_KG3DUI_EFFECT
#define ENABLE_VIDEO_MEMORY_INFO

//--------------------------------------------------------------------------------------------------------------------------

struct KG3DUIVertex2
{
	D3DCOLOR crDiffuse;
	D3DXVECTOR2 v2Vertex;
};
struct KG3DUIText2
{
	DWORD dwFontID;
	FLOAT fScale, fSpacing;
	LPCWSTR wszText;
	std::size_t nTextLength;
	D3DXVECTOR2 v2Min, v2Max;
	BYTE byShadow, byBorder, byAlpha;
	D3DCOLOR crShadow, crBorder, crFont;
};
struct KG3DUILine2
{
	D3DCOLOR crMin, crMax;
	D3DXVECTOR2 v2Min, v2Max;
};
struct KG3DUIRect2
{
	D3DCOLOR crDiffuse;
	D3DXVECTOR2 v2Min, v2Max;
};
struct KG3DUIImage2
{
	D3DXCOLOR crDiffuse;
	D3DXVECTOR2 v2Min, v2Max;
	D3DXVECTOR2 v2UVMin, v2UVMax;
};
struct KG3DUIScene2;
struct KG3DUIRectPart2;
struct KG3DUIImagePart2
{
	FLOAT fRatio;
	DWORD dwType;
	D3DXVECTOR2 v2Min, v2Max;
	D3DXVECTOR2 v2UVMin, v2UVMax;
};
struct KG3DUIImageRotate2;

class KG3DUIRenderStateGuard
{
public:
	KG3DUIRenderStateGuard();
	KG3DUIRenderStateGuard(D3DRENDERSTATETYPE type, DWORD dwValue);
	~KG3DUIRenderStateGuard();

	void Save(D3DRENDERSTATETYPE type);
	void Save(D3DRENDERSTATETYPE type, DWORD dwValue);
	void Restore();

private:
	DWORD				m_dwValue;
	D3DRENDERSTATETYPE	m_type;
    BOOL                m_bSaved;
};


class KG3DUITextureStageStateGuard
{
public:
	KG3DUITextureStageStateGuard();
	KG3DUITextureStageStateGuard(DWORD dwStage, D3DTEXTURESTAGESTATETYPE type, DWORD dwValue);
	~KG3DUITextureStageStateGuard();

	void Save(DWORD dwStage, D3DTEXTURESTAGESTATETYPE type);
	void Save(DWORD dwStage, D3DTEXTURESTAGESTATETYPE type, DWORD dwValue);
	void Restore();

private:
	DWORD						m_dwStage;
	DWORD						m_dwValue;
	D3DTEXTURESTAGESTATETYPE 	m_type;
    BOOL                        m_bSaved;
};

class KG3DUISamplerStateGuard
{
public:
	KG3DUISamplerStateGuard();
	KG3DUISamplerStateGuard(DWORD dwSampler, D3DSAMPLERSTATETYPE  type, DWORD dwValue);
	~KG3DUISamplerStateGuard();

	void Save(DWORD dwSampler, D3DSAMPLERSTATETYPE type);
	void Save(DWORD dwSampler, D3DSAMPLERSTATETYPE type, DWORD dwValue);
	void Restore();

private:
	DWORD m_dwSampler;
	DWORD m_dwValue;
	D3DSAMPLERSTATETYPE m_type;
    BOOL m_bSaved;
};


class KG3DUIViewportGuard
{
public:
	KG3DUIViewportGuard();
	KG3DUIViewportGuard(D3DVIEWPORT9 const& Viewport);
	~KG3DUIViewportGuard();

	void Save();
	void Save(D3DVIEWPORT9 const& Viewport);
	void Set(D3DVIEWPORT9 const& Viewport);
	void Restore();

private:
	D3DVIEWPORT9 m_Viewport;
    BOOL m_bSaved;
};

class KG3DUITextureRenderTargetGuard
{
public:
	KG3DUITextureRenderTargetGuard();
	~KG3DUITextureRenderTargetGuard();

	void Save(DWORD dwRenderTargetIndex, LPDIRECT3DTEXTURE9 pTexture, D3DXVECTOR2* pv2Min, D3DXVECTOR2* pv2Max, UINT uMipmapLevel);
	void Restore();

	HRESULT OnLostDevice();
	HRESULT OnResetDevice();

private:
	DWORD m_dwRenderTargetIndex;

	KG3DUIViewportGuard m_ViewportGuard;

    LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DSURFACE9 m_pDepthStencilSurface;

    LPDIRECT3DSURFACE9 m_pSurface;
	LPDIRECT3DSURFACE9 m_pSurfaceOld;
	LPDIRECT3DSURFACE9 m_pDepthStencilSurfaceOld;

    D3DXMATRIX m_mProjection;
	D3DXMATRIX m_mWorld;
	D3DXMATRIX m_mView;

    BOOL m_bSaved;
};


class KG3DUIRenderTargetStateGuard
{
private:
	enum { RENDER_STATE_GUARD_COUNT = 12 };
	enum { TEXTURE_STAGE_0_STATE_GUARD_COUNT = 7 };
	enum { TEXTURE_STAGE_1_STATE_GUARD_COUNT = 2 };
	enum { SAMPLER_0_STATE_GUARD_COUNT = 3 };

public:
	KG3DUIRenderTargetStateGuard();
	~KG3DUIRenderTargetStateGuard();

	void Save(LPDIRECT3DTEXTURE9 pTexture, D3DXVECTOR2* pvMin, D3DXVECTOR2* pvMax, UINT uMipmapLevel);
	void Save(DWORD dwTextureID, D3DXVECTOR2* pvMin, D3DXVECTOR2* pvMax, UINT uMipmapLevel);
	void Restore();

	HRESULT OnLostDevice();
	HRESULT OnResetDevice();

private:
	KG3DUIRenderStateGuard m_aRenderStateGuard[RENDER_STATE_GUARD_COUNT];
	KG3DUITextureStageStateGuard m_aTextureStage0StateGuard[TEXTURE_STAGE_0_STATE_GUARD_COUNT];
	KG3DUITextureStageStateGuard m_aTextureStage1StateGuard[TEXTURE_STAGE_1_STATE_GUARD_COUNT];
	KG3DUISamplerStateGuard m_aSampler0StateGuard[SAMPLER_0_STATE_GUARD_COUNT];
	KG3DUITextureRenderTargetGuard m_TextureRenderTargetGuard0;

    KG3DTexture* m_p3DTexture;
};


//-----------------------------------------------------------------------------------------------------------------------
class KG3DUIMiniScene;
class IKG3DeviceCallback;

typedef std::list<KG3DUIMiniScene*> KG3DUIMiniScenePtrList;

class KG3DUI : public IKG3DUI
{
public:
	KG3DUI();
	virtual ~KG3DUI();

    virtual HRESULT SetHWnd(HWND hWnd);

    virtual HRESULT SetDeviceCallback(IKG3DeviceCallback* pCallback);

    virtual HRESULT LoadFont(DWORD dwFontID, LPCSTR szFilePath, float fPixel, DWORD dwStyle);
	virtual HRESULT UnloadFont(DWORD dwFontID);
	virtual HRESULT UnloadAllFont();

	virtual HRESULT CreateImage(UINT uWidth, UINT uHeight, LPDWORD pdwImageID, DWORD dwFlags);
	virtual HRESULT DuplicateImage(DWORD dwImageID, DWORD dwFlags, LPDWORD pdwNewImageID);
	virtual HRESULT GrayImage(DWORD dwImageID, LPDWORD pdwNewImageID);
	virtual HRESULT MaskImage(DWORD dwImageID, DWORD dwMaskImageID);
	virtual HRESULT LoadImage(LPCTSTR szFilePath, LPDWORD pdwImageID);
	virtual HRESULT UnloadImage(DWORD dwImageID);
	virtual HRESULT UnloadAllImage();
    virtual HRESULT SaveImage(LPCTSTR szFilePath, DWORD dwImageID);
	virtual HRESULT RegisterSliceImageSize(UINT uWidth, UINT uHeight);

    virtual HRESULT SetupD3DDeviceCursor();
    virtual HRESULT SetD3D9DeviceCursor(HCURSOR hCursor);
    virtual HRESULT SetD3D9DeviceCursorPosition(INT nX, INT nY, DWORD dwFlags);
    virtual HRESULT ShowD3D9DeviceCursor(BOOL bShow);
    virtual HRESULT ClipD3DDeviceCursor(BOOL bClip);
    virtual BOOL IsShowD3D9DeviceCursor() const;
    virtual BOOL IsClipD3D9DeviceCursor() const;

    virtual HRESULT CreateScene(IKG3DScene** pp3DScene);
	virtual HRESULT UnloadScene(IKG3DScene* p3DScene);

	virtual HRESULT GetImageData(DWORD dwImageID, UINT uWidth, UINT uHeight, BYTE* pData, UINT uLength, BOOL bUpToDown) const;
	virtual HRESULT GetImageSize(DWORD dwImageID, UINT* puWidth, UINT* puHeight) const;

	virtual HRESULT FillTriangleFan(KG3DUIVertex2 const* pBuffer, size_t nCount);

    virtual HRESULT ScreenShot(const char szFilePath[], D3DXIMAGE_FILEFORMAT eType, int nnQuality);
	virtual HRESULT LoadText(KG3DUIText2 const& text);

	virtual HRESULT DrawMultiLine(KG3DUILine2 const* pBuffer, size_t nCount);
	virtual HRESULT DrawMultiRect(KG3DUIRect2 const* pBuffer, size_t nCount);
	virtual HRESULT DrawMultiText(KG3DUIText2 const* pBuffer, size_t nCount);
	virtual HRESULT FillMultiRect(KG3DUIRect2 const* pBuffer, size_t nCount);
	virtual HRESULT DrawMultiImage(KG3DUIImage2 const* pBuffer, size_t nCount);
	virtual HRESULT DrawMultiScene(KG3DUIScene2 const* pBuffer, size_t nCount);
	virtual HRESULT FillMultiRectPart(KG3DUIRectPart2 const* pBuffer, size_t nCount);
	virtual HRESULT DrawMultiImagePart(KG3DUIImagePart2 const* pBuffer, size_t nCount);
	virtual HRESULT DrawMultiImageRotate(KG3DUIImageRotate2 const* pBuffer, size_t nCount);

	virtual HRESULT DrawMultiImageToBuffer(KG3DUIImage2 const* pBuffer, size_t nCount, UINT uWidth, UINT uHeight, BYTE *pData, size_t uLength, BOOL bUpToDown);
	
	virtual HRESULT DrawUserdataToImage(DWORD dwImageID, KG3DUIDrawUserdata DrawUserdata,void* pUserData);

	virtual HRESULT DrawOleToImage(LPUNKNOWN pUnknown, DWORD dwImageID);

	virtual HRESULT BeginScene();
	virtual HRESULT EndScene();

    virtual HRESULT BeginRenderToScreen();
	virtual HRESULT EndRenderToScreen();

    virtual HRESULT Present();

	virtual HRESULT BeginRenderToTexture(DWORD dwTextureID, UINT uMipmapLevel);
	virtual HRESULT EndRenderToTexture();

	virtual HRESULT BeginRenderToImage(DWORD dwImageID);
	virtual HRESULT EndRenderToImage();

	virtual HRESULT BeginStencil();
	virtual HRESULT SetStencilFunc();
	virtual HRESULT EndStencil();

	virtual HRESULT GetCharWidth(DWORD dwFontID, FLOAT fScale, FLOAT& fWidth) const;
	virtual HRESULT GetCharHeight(DWORD dwFontID, FLOAT fScale, FLOAT& fHeight) const;
	virtual HRESULT GetTextExtent(DWORD dwFontID, LPCWSTR wszText, int nTextLength, FLOAT &fWidth, FLOAT &fHeight, FLOAT fSpace, FLOAT fScale, BOOL bBorder) const;
	virtual HRESULT GetTextPosExtent(DWORD dwFontID, LPCWSTR wszText, int nTextLength, FLOAT fCursorPos, LPINT lpCharPos, FLOAT fSpace, FLOAT fScale, BOOL bBorder) const;

    virtual HRESULT OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice, D3DSURFACE_DESC const* pBackBufferSurfaceDesc, D3DPRESENT_PARAMETERS const* pPresentParameters);
    virtual HRESULT OnDestroyDevice();
	virtual HRESULT OnLostDevice();
	virtual HRESULT OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice, D3DSURFACE_DESC const* pBackBufferSurfaceDesc, D3DPRESENT_PARAMETERS const* pPresentParameters);

    virtual HRESULT GetDeviceSettings(KG3DDeviceSettings* pSettings);
    virtual HRESULT SetDeviceSettings(KG3DDeviceSettings* pSettings);
	virtual HRESULT SetViewportSize(UINT uWidth, UINT uHeight);

	virtual HRESULT UpdateMultiSceneViewport(KG3DUIScene2 const* pBuffer, size_t nCount);

	virtual VideoMemoryInfo GetVideoMemeryInfo() const;
	virtual UINT GetAvailableTextureMem() const;
	virtual void GetUITextureUsage(KG3DUITextureUsage& Usage) const;

	virtual UINT GetAllowableAdapterModeCount() const;
    virtual HRESULT GetAllowableAdapterMode(D3DDISPLAYMODE* pAdapterMode, UINT* puCount) const;

#if defined(DEBUG) | defined(_DEBUG)
	virtual LPDIRECT3DDEVICE9 GetDevice();	//���ڸ�UI�Ǳߵ�D3D�����ã�ֻ��Debug�汾��������á�����ʽ�������治�ܱ�¶��Device
#endif

private:
	HRESULT InvalidateFont();
	HRESULT RestoreDeviceFont();

	HRESULT DrawMultiImageImpl(KG3DUIImage2 const* pBuffer, size_t nCount, size_t nOffset, DWORD dwImageID);
	HRESULT DrawMultiImagePartImpl(KG3DUIImagePart2 const* pBuffer, size_t nCount, size_t nOffset, DWORD dwImageID);
	HRESULT DrawMultiImageRotateImpl(KG3DUIImageRotate2 const* pBuffer, size_t nCount, size_t nOffset, DWORD dwImageID);

	HRESULT FillMultiRectPartTypeTimer(D3DXVECTOR2 const& v2Min, D3DXVECTOR2 const& v2Max, D3DCOLOR diffuse, float fRatio, K2DDiffuseVertex* pVertex, int& nTriCount);
	HRESULT FillMultiRectPartTypeXMinToMax(D3DXVECTOR2 const& v2Min, D3DXVECTOR2 const& v2Max, D3DCOLOR diffuse, float fRatio, K2DDiffuseVertex* pVertex, int& nTriCount);
	HRESULT FillMultiRectPartTypeXMaxToMin(D3DXVECTOR2 const& v2Min, D3DXVECTOR2 const& v2Max, D3DCOLOR diffuse, float fRatio, K2DDiffuseVertex* pVertex, int& nTriCount);
	HRESULT FillMultiRectPartTypeYMinToMax(D3DXVECTOR2 const& v2Min, D3DXVECTOR2 const& v2Max, D3DCOLOR diffuse, float fRatio, K2DDiffuseVertex* pVertex, int& nTriCount);
	HRESULT FillMultiRectPartTypeYMaxToMin(D3DXVECTOR2 const& v2Min, D3DXVECTOR2 const& v2Max, D3DCOLOR diffuse, float fRatio, K2DDiffuseVertex* pVertex, int& nTriCount);

	HRESULT FillMultiImagePartTypeTimer(D3DXVECTOR2 const& v2Min, D3DXVECTOR2 const& v2Max, D3DXVECTOR2 const& v2UVMin, D3DXVECTOR2 const& v2UVMax, D3DXCOLOR crDiffuse, float fRatio, K2DTex1Vertex* pVertex, int& nTriCount);
	HRESULT FillMultiImagePartTypeXMinToMax(D3DXVECTOR2 const& v2Min, D3DXVECTOR2 const& v2Max, D3DXVECTOR2 const& v2UVMin, D3DXVECTOR2 const& v2UVMax, D3DXCOLOR crDiffuse, float fRatio, K2DTex1Vertex* pVertex, int& nTriCount);
	HRESULT FillMultiImagePartTypeXMaxToMin(D3DXVECTOR2 const& v2Min, D3DXVECTOR2 const& v2Max, D3DXVECTOR2 const& v2UVMin, D3DXVECTOR2 const& v2UVMax, D3DXCOLOR crDiffuse, float fRatio, K2DTex1Vertex* pVertex, int& nTriCount);
	HRESULT FillMultiImagePartTypeYMinToMax(D3DXVECTOR2 const& v2Min, D3DXVECTOR2 const& v2Max, D3DXVECTOR2 const& v2UVMin, D3DXVECTOR2 const& v2UVMax, D3DXCOLOR crDiffuse, float fRatio, K2DTex1Vertex* pVertex, int& nTriCount);
	HRESULT FillMultiImagePartTypeYMaxToMin(D3DXVECTOR2 const& v2Min, D3DXVECTOR2 const& v2Max, D3DXVECTOR2 const& v2UVMin, D3DXVECTOR2 const& v2UVMax, D3DXCOLOR crDiffuse, float fRatio, K2DTex1Vertex* pVertex, int& nTriCount);

	HRESULT BeginRenderToTexture(LPDIRECT3DTEXTURE9 pTexture, UINT uMipmapLevel);

	//void ChangeDrawState(KDrawState DrawState);

private:
    IKG3DeviceCallback*                 m_pCallback;

    KG3DUITexture*						m_pUITexture;

	KG3DUIVBuffer<K2DDiffuseVertex>		m_DiffuseVertex;
	UINT								m_nDiffuseVertexLocked;

	KG3DUIVBuffer<K2DTex1Vertex>		m_Tex1Vertex;
	UINT								m_nTex1VertexLocked;

#ifdef ENABLE_VIDEO_MEMORY_INFO
	IDirectDraw7*						m_pDirectDraw;
#endif

	HWND								m_hWnd;
	UINT								m_uViewportWidth;
	UINT								m_uViewportHeight;

	KG3DUIRenderTargetStateGuard		m_RenderToTextureState;
	KG3DUIRenderTargetStateGuard		m_RenderToImageState;

#ifdef ENABLE_KG3DUI_EFFECT
	LPD3DXEFFECT						m_pEffect;
#endif

	KDrawState							m_DrawState;				

	KG3DUITextureStageStateGuard m_aTextureStage0StateGuard[8];
	KG3DUITextureStageStateGuard m_aTextureStage1StateGuard[6];

	KG3DUIRenderStateGuard m_aRenderStateGuard[14];

	KG3DUISamplerStateGuard m_aSampler0StateGuard[4];
	KG3DUISamplerStateGuard m_aSampler1StateGuard[4];
	KG3DUISamplerStateGuard m_aSampler2StateGuard[4];

	KG3DUIViewportGuard	m_ViewportGuard;

	D3DXMATRIX m_mUIProjection;
	D3DXMATRIX m_mUIWorld;
	D3DXMATRIX m_mUIView;

	D3DXMATRIX m_mSceneProjection;
	D3DXMATRIX m_mSceneWorld;
	D3DXMATRIX m_mSceneView;

    KG3DUIMiniScenePtrList m_MiniScenePtrList;

	// Inherited via IKG3DUI
	virtual HRESULT GrayImage(DWORD dwImageID, RECT const & DstRect, LPDWORD pdwNewImageID) override;
	virtual HRESULT FillImage(DWORD dwDstImageID, LPCTSTR szImageFile, RECT const & SrcRect, RECT const & DstRect, int bGray) override;
	virtual HRESULT FillTextGlyph(DWORD dwFontID, const wchar_t * pwszText, int nLen = -1, int nBorder = 0) override;
	virtual HRESULT LoadFont(DWORD dwFontID, LPCSTR szFilePath, long nFaceIndex, float fPixel, DWORD dwStyle, int bAlonemgr = false) override;
	virtual HRESULT CreateScene(IKG3DScene ** pp3DScene, const char * cszEnvFileName = NULL) override;
	virtual HRESULT ScreenShot(const char szFilePath[], D3DXIMAGE_FILEFORMAT eType, int nQuality, RECT const & ShotRect) override;
	virtual HRESULT ScreenShotEx(const char szFilePath[], D3DXIMAGE_FILEFORMAT eType, DWORD nWidth, DWORD nHeight) override;
	virtual HRESULT OverSamplingScreenShot(LPCTSTR tszFilePath, D3DXIMAGE_FILEFORMAT eType, INT nMagnification = 2, BOOL bOuputSourceScreenShot = FALSE) override;
	virtual void DrawScreen3DSfx(IKG3DModel * p3DModel, D3DXVECTOR2 Pos, float fBoxW, float fBoxH, int bClearZ, int bRenderBox) override;
	virtual HRESULT GetModelBoxSize(IKG3DModel * pModel, float & rfWidth, float & rfHeight) override;
	virtual void ResetDrawState() override;
	virtual HRESULT BeginRenderToTextureEx(DWORD dwTextureID, UINT uMipmapLevel) override;
	virtual void TransformSceneCoorSys() override;
	virtual void TransformUICoorSys() override;
	virtual void RestoreCoorSys() override;
	virtual HRESULT OnResetViewport(UINT uWidth, UINT uHeight) override;
	virtual HRESULT SetCodePage(unsigned int uCodePage) override;
	virtual unsigned int GetCodePage() override;
	virtual int LockDiffuseVertexBuff(UINT uVertexCnt, K2DDiffuseVertex** ppVer, UINT* pnRetStartVertex) override;
	virtual int UnLockDiffuseVertexBuff() override;
	virtual size_t GetDiffuseVertexNums() override;
	virtual HRESULT DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT nCount) override;
	virtual int LockTex1VertexBuff(UINT uVertexCnt, K2DTex1Vertex** ppVer, UINT* pnRetStartVertex) override;
	virtual int UnLockTex1VertexBuff() override;
	virtual size_t GetTex1VertexNums() override;
	virtual HRESULT DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, DWORD_PTR dwTextureID, UINT StartVertex, UINT nCount) override;
	virtual HRESULT BeginDrawMultiScene() override;
	virtual HRESULT EndDrawMultiScene() override;
	virtual DWORD_PTR GetImageTexture(DWORD dwImageID, D3DFORMAT* pFormat) override;
	virtual void ChangeDrawState(KDrawState DrawState) override;
	virtual HRESULT GetTextureRenderData(DWORD_PTR dwTextureID, UINT uWidth, UINT uHeight, BYTE * pData, size_t uLength, BOOL bUpToDown) override;
	virtual HRESULT CreateTexture(UINT uWidth, UINT uHeight, DWORD_PTR* pdwTextureID) override;
	virtual void ReleaseTexture(DWORD_PTR dwTextureID) override;
	virtual IKG3DFontTexture * GetFont(DWORD dwFontID) override;
	virtual HRESULT GetTextRange(DWORD dwFontID, LPCWSTR wszText, int nTextLen, LPCWSTR wszEnd, FLOAT fCursorPos, FLOAT fSpace, FLOAT fScale, LPINT lpCharPos, FLOAT & fWidth, FLOAT & fHeight, int nBorder = 0) const override;
	virtual HRESULT ReloadTexture(LPCTSTR szFilePath, DWORD dwImageID, int bGray) override;
	virtual HRESULT LoadImageA(LPCTSTR szFilePath, LPDWORD pdwImageID, int bGray = 0);
};

#endif // KG3D_UI_H
