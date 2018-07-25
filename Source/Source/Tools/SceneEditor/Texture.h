// Texture.h: interface for the KTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE_H__DB78064D_653D_4625_AEF2_74551A3343B1__INCLUDED_)
#define AFX_TEXTURE_H__DB78064D_653D_4625_AEF2_74551A3343B1__INCLUDED_

#include "Engine.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PAL_SIZE 256 
#define MAX_BACK_TEXTURE_NUM	121		//����Cache��Texture
//����Represent��������Ͷ���
#define REPRESENT_OK				0
#define REPRESENT_ERR				1
#define REPRESENT_ERR_FILE_OPEN		2				//�ļ���ȡ����
#define REPRESENT_ERR_OUTMEMORY		3				//�ڴ治��
#define REPRESENT_ERR_OUTVIEDOMEMORY	4			//�Դ治��
#define REPRESENT_ERR_FILENAME		5				//�ļ�������
#define REPRESENT_ERR_FILE_CONTENT	6				//�ļ����ݴ���

typedef int					int32;
typedef unsigned int        uint32;

/*

typedef struct {
	BYTE		Red;		// ��ɫ����
	BYTE		Green;		// ��ɫ����
	BYTE		Blue;		// ��ɫ����
} KPAL24;					// 24λɫ��
*/


struct TextureInfo
{
	uint32	nWidth;						// ��ͼ���
	uint32	nHeight;					// ��ͼ�߶�
	LPDIRECT3DTEXTURE9 pTexture;		// ��ͼָ��
	uint32	nFrameX;					// ����һ֡ͼ���ϵ����Ͻ�λ��
	uint32	nFrameY;					// ����һ֡ͼ���ϵ����Ͻ�λ��
	int32	nFrameWidth;				// ��ͼ���������ݿ��
	int32	nFrameHeight;				// ��ͼ���������ݸ߶�
};

struct FrameToTexture
{
	int32	nTexNum;					// ��һ֡���Ϊ������ͼ
	int32	nWidth;						// ��һ֡�Ŀ��
	int32	nHeight;					// ��һ֡�ĸ߶�
	int32	nOffX;						// ���������spr��X����ƫ��
	int32	nOffY;						// ���������spr��Y����ƫ��
	TextureInfo texInfo[4];				// ��ͼ��Ϣ���飬һ����֣����4����ͼ
	int32	nRawDataLen;				// ԭʼ���ݳ���
	BYTE	*pRawData;					// ��һ֡��ԭʼ����
	void	*pFrame;					// ����SprGetFrame���ص�ָ�룬��SprReleaseFrameʹ��
};

typedef class KTexture  
{
public:
	FILETIME m_FileTime;

	HRESULT ReloadFromFile();
	HRESULT Exchange2Bump(LPDIRECT3DTEXTURE9& lpTexture);
	virtual HRESULT LoadBumpMap(LPSTR pFileName);

	HRESULT CreateFromText(TCHAR* strFontName,
						   TCHAR* Texts,
						   WORD   FontHeight,
						   DWORD*   pTextureWidth,
						   DWORD*   pTextureHeight,
						   DWORD  nFlag);
	HRESULT CreateFromText(TCHAR* strFontName,	
						   TCHAR* Texts,
						   WORD   FontHeight,
						   DWORD* pTextureWidth,
						   DWORD* pTextureHeight,
						   DWORD  nFlag,
						   DWORD  nChar,
						   DWORD& nCharPos);

	HRESULT LoadCubeMap(LPSTR pFileName);
	HRESULT LoadVolumeMap(LPSTR pFileName);
	HRESULT ComputeNormal();
	virtual HRESULT Update(DWORD Method,float Time);
	virtual HRESULT LoadFromFile(LPSTR pFileName);

	virtual HRESULT CleanUp();
	LPSTR m_lpFileName;
	DWORD m_dOption;
	virtual void Initialize(PVOID pTextureTable);
	DWORD m_dType;
	DWORD ID;

	LPDIRECT3DTEXTURE9  m_lpTexture;

	KTexture();
	virtual ~KTexture();

	HRESULT SetAlpha(D3DCOLOR Color,BYTE Alpha);
	virtual HRESULT InvalidateDeviceObjects(void);
	virtual HRESULT RestoreDeviceObjects(void);
}* LPTEXTURE;

class KAnimateTexture :public KTexture
{
public:

    virtual HRESULT CleanUp();
	HRESULT Update(DWORD Method,float Time);

	HRESULT LoadBumpMap(LPSTR pFileName);
	HRESULT LoadFromFile(LPSTR pFileName);
	LPDIRECT3DTEXTURE9*  m_lpBackTextures;
	DWORD  m_dwNumFrame;
	float  m_fFrameLength;
	KAnimateTexture()
	{
		m_fFrameLength = 100;
		m_dwNumFrame = NULL;
		m_lpBackTextures = NULL;
	};
	virtual ~KAnimateTexture()
	{
	};

};

//class KTextureResSpr : public KTexture
//{
//public:
//	KTextureResSpr();
//	virtual ~KTextureResSpr();
//
//	// ����Ա������Ϊ��ʼֵ
//	virtual void ResetVar();
//
//	// �����ڴ���Դ
//	virtual bool CreateImage(const char* szImage, int nWidth, int nHeight, uint32 nType);
//	// ���ļ�������Դ
//	virtual int LoadImage(char* szImage, uint32 nType);
//
//	// �豸��ʧʱ�ͷ�d3d����
//	virtual HRESULT InvalidateDeviceObjects();
//
//	// �豸�ָ�ʱ�ؽ�d3d����
//	virtual HRESULT RestoreDeviceObjects();
//
//	// �ͷ��ڴ�
//	void Clear();
//
//	// �ͷ�һ֡��ͼ����
//	// �������ʾ�ͷ���һЩ���ݣ����ؼٱ�ʾ���е���ͼ���ݶ����ͷ����
//	virtual bool ReleaseAFrameData();
//
//	// Ϊ����׼�����ݣ�
//	// szImage���ļ���
//	// nFrame���ڼ�֡����
//	int PrepareFrameData(const char* szImage, int32 nFrame, bool bPrepareTex);
//
//	// ȡ��nFrame֡�ĵ�nIdx����ͼ�����ݲ�ͬ���nIdxȡֵΪ1-4
//	LPDIRECT3DTEXTURE9 GetTexture(int32 nFrame, int nIdx);
//
//	uint32 GetCenterX(){ return m_nCenterX; }
//	uint32 GetCenterY(){ return m_nCenterY; }
//	uint32 GetFrameNum(){ return m_nFrameNum; }
//	uint32 GetDirections(){ return m_nDirections; };
//	uint32 GetInterval(){ return m_nInterval; };
//
//	// ȡ�õ�nFrame֡ͼ��x��y�����ϵ����ص�alphaֵ
//	int32 GetPixelAlpha(const char* szImage, int32 nFrame, int32 x, int32 y);
//
//	//���˺�����Ϊpublic���ƻ��˷�װ������Ϊ������memoy��飬ֻ������
//	int CreateTexture16Bit(int32 nFrame,LPDIRECT3DDEVICE9 pD3DDevice);
//	//private:
//public:
//	uint32		m_nCenterX;
//	uint32		m_nCenterY;
//	uint32		m_nColors;
//	uint32		m_nDirections;
//	uint32		m_nInterval;
//
//	// ��������ͳ��
//	unsigned long		m_nTexMemUsed;			// ��ͼ�ڴ�ռ������,��λ�ֽ�
//	unsigned long		m_nSprMemUsed;			// ��ͼ�ڴ�ռ������,��λ�ֽ�
//
//
//	KPAL24		m_pPal24[MAX_PAL_SIZE];		// ��ɫ��
//	WORD		m_pPal16[MAX_PAL_SIZE];		// 4444��ɫ��
//	int32		m_nFrameNum;				// ��Դ֡��
//
//	FrameToTexture*		m_pFrameInfo;		// ֡����ͼӳ����Ϣ����ָ��
//	char*		m_pHeader;					// sprͷ����Ϣ�������Ϊ����ζ�ŷ�֡ѹ��������Ϊ����ѹ��
//
//private:
//	int LoadSprFile(char* szImage);
//
//	void CreateTexture32Bit(uint32 nFrame);
//
//	// �����ͼ������ͼ���Ϊ1��4���Գ��������ͼ�ռ�
//	int SplitTexture(uint32 nFrame);
//	bool m_bPacked;
//	bool m_bCacheable;
//	bool m_bNeedReDraw;
//
//	int m_nWidth		;
//	int m_nHeight		;
//	uint32		m_nType;
//	char m_szFileName[80];
//	///spr�ļ����ڴ��е�ӡ��
//	//SPRHEAD*		m_psprHead;			
//};

//class KMovieTexture : public KTexture
//{
//public:
//	//-----------------------------------------------------------------------------
//	// Define GUID for Texture Renderer
//	// {71771540-2017-11cf-AE26-0020AFD79767}
//	//-----------------------------------------------------------------------------
//	struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;
//
//	class CTextureRenderer : public CBaseVideoRenderer
//	{
//	public:
//		CTextureRenderer(LPUNKNOWN pUnk,HRESULT *phr);
//		~CTextureRenderer();
//	public:
//		HRESULT SetDevice(LPDIRECT3DDEVICE9 pDevice);
//		D3DFORMAT g_TextureFormat;
//		LPDIRECT3DTEXTURE9 g_pTexture;
//
//		HRESULT CheckMediaType(const CMediaType *pmt );     // Format acceptable?
//		HRESULT SetMediaType(const CMediaType *pmt );       // Video format notification
//		HRESULT DoRenderSample(IMediaSample *pMediaSample); // New video sample
//
//		LONG m_lVidWidth;	// Video width
//		LONG m_lVidHeight;	// Video Height
//		LONG m_lVidPitch;	// Video Pitch
//	};
//
//	/*CTextureRenderer        *pCTR;          // DShow Texture renderer
//
//	CComPtr<IGraphBuilder>  g_pGB;          // GraphBuilder
//	CComPtr<IMediaControl>  g_pMC;          // Media Control
//	CComPtr<IMediaPosition> g_pMP;          // Media Postion
//	CComPtr<IMediaEvent>    g_pME;          // Media Event
//
//	CComPtr<IVideoWindow>   g_pVW ;
//	CComPtr<IBasicAudio>    g_pBA ;
//	CComPtr<IBasicVideo>    g_pBV ;
//	CComPtr<IMediaSeeking>  g_pMS ;*/
//
//public:
//	KMovieTexture();
//	~KMovieTexture();
//
//	HRESULT  LoadFormMoveFile(LPCSTR pScrFile);
//	void     Cleanup();
//	void     CleanupDShow();
//	void     CheckMovieStatus();
//
//	HRESULT  InitDShowTextureRenderer(LPDIRECT3DTEXTURE9* ppTexture,LPCSTR pScrFile);
//
//};

#endif // !defined(AFX_TEXTURE_H__DB78064D_653D_4625_AEF2_74551A3343B1__INCLUDED_)
