#include "StdAfx.h"
#include "KG3DTextureAnimation.h"
#include "kg3denginemanager.h"
#include "KG3DGraphicsTool.h"


#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////

KG3DTextureAnimation::KG3DTextureAnimation(void)
{
	m_lpBackTextures = NULL;
	m_dwNumFrame = 0;
	m_fFrameLength =100;
}

KG3DTextureAnimation::~KG3DTextureAnimation(void)
{
	Clean();
}

HRESULT KG3DTextureAnimation::Init()
{
    HRESULT hr = E_FAIL;
    HRESULT hrResult = E_FAIL;
    int bInitKG3DTexture = false;

    hr = KG3DTexture::Init();
    KGLOG_COM_PROCESS_ERROR(hr);

    bInitKG3DTexture = true;

    hrResult = S_OK;
Exit0:
    if (FAILED(hrResult))
    {
        if (bInitKG3DTexture)
        {
            hr = KG3DTexture::UnInit();
            KGLOG_COM_CHECK_ERROR(hr);
        }
    }
    return hrResult;
}

HRESULT KG3DTextureAnimation::UnInit()
{
    HRESULT hr = E_FAIL;
    HRESULT hrResult = E_FAIL;

	hr = Clean();
    KGLOG_COM_PROCESS_ERROR(hr);

	hr = KG3DTexture::UnInit();
    KGLOG_COM_PROCESS_ERROR(hr);

    hrResult = S_OK;
Exit0:
    return hrResult;
}

HRESULT KG3DTextureAnimation::OnLostDevice()
{
    // TODO: �˴�ʵ��ֻ��֤��ȷ�ԣ�û��������

    HRESULT hr = E_FAIL;
    HRESULT hrResult = E_FAIL;

    hr = Clean();
    KGLOG_COM_PROCESS_ERROR(hr);

#if 0
    // TODO: KG3DTexture ���ڸ��ӣ��ü̳й�ϵ��ʹ�ò����ʣ������KG3DTexture����Ҫ��̫�๦�ܷŵ����棬�������ͼ��
    hr = KG3DTexture::OnLostDevice();
    KGLOG_COM_PROCESS_ERROR(hr);
#endif

    hrResult = S_OK;
Exit0:
    return hrResult;
}

HRESULT KG3DTextureAnimation::OnResetDevice()
{
    HRESULT hr = E_FAIL;
    HRESULT hrResult = E_FAIL;

#if 0
    hr = KG3DTexture::OnResetDevice();
    KGLOG_COM_PROCESS_ERROR(hr);
#endif

    hr = CheckLoadState();
    KGLOG_COM_PROCESS_ERROR(hr);

    hrResult = S_OK;
Exit0:
    return hrResult;
}

HRESULT KG3DTextureAnimation::Clean()
{
	m_pTexture = NULL;

	if (m_lpBackTextures)
	{
		for(DWORD i=0;i<m_dwNumFrame;i++)
		{
			SAFE_RELEASE(m_lpBackTextures[i]);
		}
		SAFE_DELETE_ARRAY(m_lpBackTextures);
	}
	m_dwNumFrame = 0;
	return S_OK;
}

HRESULT KG3DTextureAnimation::CheckLoadState()
{
	HRESULT hr = E_FAIL;
    HRESULT hrResult = E_FAIL;

	m_pTexture = NULL;

	hr = LoadFromFile(m_scName.c_str(), 0, m_dwType);
    KGLOG_COM_PROCESS_ERROR(hr);

    hrResult = S_OK;
Exit0:
	return hrResult;
}

HRESULT KG3DTextureAnimation::LoadFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption)
{
    HRESULT hr = E_FAIL;
    HRESULT hrResult = E_FAIL;
    IIniFile* pIniFile = NULL;
	int nCount = 0;

	Clean();
	
	if (g_eEngineOption.bEnableAutoReLoadTexture)
		_GetFileTime(cszFileName,m_FileTime);

	pIniFile = g_OpenIniFile(cszFileName);
	KGLOG_PROCESS_ERROR(pIniFile);

	pIniFile->GetInteger("Main", "NumFrame", 0, &nCount);
	pIniFile->GetFloat("Main", "FrameTime", 100, &m_fFrameLength);

	m_dwNumFrame = (DWORD)nCount;

    _ASSERTE(m_lpBackTextures == NULL);

	m_lpBackTextures = new LPDIRECT3DTEXTURE9[m_dwNumFrame];
	KGLOG_PROCESS_ERROR(m_lpBackTextures);
	ZeroMemory(m_lpBackTextures, sizeof(LPDIRECT3DTEXTURE9) * m_dwNumFrame);

	for (int i = 0;i < nCount; ++i)
	{
		TCHAR str[64];
		TCHAR FileName[256];

        wsprintf(str,"Texture%d",i);

		pIniFile->GetString("Main", str,"",FileName,256);

		hr = KG3DD3DXCreateTextureFromFile(g_pd3dDevice, FileName, &m_lpBackTextures[i]);
        KGLOG_COM_CHECK_ERROR(hr);
	}

	m_uOption = uOption;
	m_scName = cszFileName;

    hrResult = S_OK;
Exit0:
	if(FAILED(hrResult))
	{
		if (m_lpBackTextures)
		{
			for(DWORD i=0;i<m_dwNumFrame;i++)
			{
				SAFE_RELEASE(m_lpBackTextures[i]);
			}
			SAFE_DELETE_ARRAY(m_lpBackTextures);
		}
	}
	KG_COM_RELEASE(pIniFile);

	return hrResult;
}

HRESULT KG3DTextureAnimation::Update(unsigned uMethod, DWORD dwTime)//������ͼ,�Զ�����ͼ������
{
	if(!m_lpBackTextures)
		return E_FAIL;

	DWORD dwCurTime = dwTime % (DWORD)(m_dwNumFrame * m_fFrameLength);
	DWORD dwCurFrame = (DWORD)(dwCurTime / m_fFrameLength);
	if(dwCurFrame>=m_dwNumFrame)
		return E_FAIL;
	m_pTexture = m_lpBackTextures[dwCurFrame];
	return S_OK;
}

HRESULT KG3DTextureAnimation::Update(int nFrame)//������ͼ
{
	if(nFrame>=(int)m_dwNumFrame)
		return E_FAIL;
	if(nFrame<0)
		return E_FAIL;

	m_pTexture = m_lpBackTextures[nFrame];

	return S_OK;
}
