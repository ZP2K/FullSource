#include "TGALoader.h"

static IDirect3D9* m_pIDirect3D;//IDirect3D9�ӿ�
static IDirect3DDevice9* m_pIDirect3DDevice;   //Direct3D�豸
static IDirect3DVertexBuffer9* m_pVertexBuffer;  //���㻺����
static IDirect3DTexture9* m_pTexture;//�������
static LPD3DXLINE m_pLine;
static UITEXFILEHEADER m_UiTexFileHeader;
static std::vector<UITEXFRAMEDATASTRUCTURE> m_aFrameVector;

static int m_IconID, g_nFrame;
static RECT m_IconRect;
static RECT g_IconOldRect;
static RECT g_IconOldDrawRect;

void SetOldIconRect(RECT rect)
{
	g_IconOldRect = rect;
}

int GetCurrenIconID()
{
	return m_IconID;
}
//���������ֵ�������Ⱦָ���Ĳ�������
void SetRenderFrame(int nFrame)
{
	g_nFrame = nFrame;
}

RECT GetCurrenIconRect()
{
	return m_IconRect;
}

SIZE GetCurrentImageSize()
{
	SIZE sz;
	sz.cx = m_UiTexFileHeader.nWidth;
	sz.cy = m_UiTexFileHeader.nHeight;
	return sz;
}

SIZE GetCurrentFrameSize()
{
	SIZE sz;
	if (g_nFrame >= 0 && (int)m_aFrameVector.size() > g_nFrame)
	{
		UITEXFRAMEDATASTRUCTURE frame = m_aFrameVector[g_nFrame];
		sz.cx = frame.nWidth;
		sz.cy = frame.nHeight;
	}
	else
	{
		sz.cx = 0;
		sz.cy = 0;
	}
	return sz;
}

bool CreateD3Device(HWND hWnd, bool bFullScreen)
{
	//����IDirect3D9����
	m_pIDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pIDirect3D == NULL )
	{
		return false;
	}
	D3DCAPS9 d3dcaps;
	m_pIDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&d3dcaps);
	bool bHarwareProcess;
	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		bHarwareProcess = true;
	}
	else
	{
		bHarwareProcess = false;
	}
	//ȡ����ʾģʽ
	D3DDISPLAYMODE d3dDisplayMode;
	if (FAILED(m_pIDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3dDisplayMode)))
	{
		return false;
	}
	//����D3D�豸����Ⱦ����
	D3DPRESENT_PARAMETERS d3dPresent_Param; //��Ⱦ�����ṹ��
	ZeroMemory(&d3dPresent_Param,sizeof(d3dPresent_Param));  //�ṹ������
	d3dPresent_Param.BackBufferWidth = d3dDisplayMode.Width;
	d3dPresent_Param.BackBufferHeight = d3dDisplayMode.Height;
	d3dPresent_Param.BackBufferFormat = d3dDisplayMode.Format;
	d3dPresent_Param.BackBufferCount = 1; //һ�����ͼ����
	d3dPresent_Param.hDeviceWindow = hWnd;
	d3dPresent_Param.Windowed = !bFullScreen; //falseΪȫ��Ļ��ʾ��ʽ
	d3dPresent_Param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dPresent_Param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//����D3D�豸
	if (bHarwareProcess)
	{
		if (FAILED(m_pIDirect3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&d3dPresent_Param,&m_pIDirect3DDevice)))
		{
			return false;
		}
	}
	else
	{
		if (FAILED(m_pIDirect3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dPresent_Param,&m_pIDirect3DDevice)))
		{
			return false;
		}
	}
	return true;
}

//��ʼ����������
bool InitVertexBuffer()
{
	CUSTOMVERTEX customvertex[]={
		{0.0f,0.0f,0.0f,1.0f,0.0f,0.0f},
		{(float)m_UiTexFileHeader.nWidth,0.0f,0.0f,1.0f,1.0f,0.0f},
		{0.0f,(float)m_UiTexFileHeader.nHeight,0.0f,1.0f,0.0f,1.0f},
		{(float)m_UiTexFileHeader.nWidth,(float)m_UiTexFileHeader.nHeight,0.0f,1.0f,1.0f,1.0f}
	};

	BYTE* pVertexData;
	//�������㻺������ָ��
	if(FAILED(m_pIDirect3DDevice->CreateVertexBuffer(4*sizeof(CUSTOMVERTEX),0,CUSTOMVERTEX_FVF,D3DPOOL_MANAGED,&m_pVertexBuffer,NULL)))
	{
		return false;
	}	
	//ȡ�ö��㻺������ָ��
	if (FAILED(m_pVertexBuffer->Lock(0,0,(void**)&pVertexData,0)))
	{
		return false;
	}
	//д�붥�����ݵ����㻺����
	memcpy(pVertexData,customvertex,sizeof(customvertex));
	//����
	m_pVertexBuffer->Unlock();

	return true;
}

bool ReSetTGAPos(float Width,float Height)
{
	if (!m_pVertexBuffer) return false;

	BYTE* pVertexData;
	//CUSTOMVERTEX* pCustomV;
	
	float X, Y;

	float fleft, ftop, fright, fbottom, fwidth, fheight;
	if (g_nFrame >= 0 && (int)m_aFrameVector.size() > g_nFrame)
	{
		UITEXFRAMEDATASTRUCTURE frame = m_aFrameVector[g_nFrame];
		fleft = (float)frame.nLeft / (float)m_UiTexFileHeader.nWidth;
		fright = (float)(frame.nLeft + frame.nWidth) / (float)m_UiTexFileHeader.nWidth;
		ftop = (float)frame.nTop / (float)m_UiTexFileHeader.nWidth;
		fbottom = (float)(frame.nTop + frame.nHeight) / (float)m_UiTexFileHeader.nHeight;
		fwidth = (float)frame.nWidth;
		fheight = (float)frame.nHeight;
		X = (Width - ((float)frame.nWidth))/2.0f;
		Y = (Height - ((float)frame.nHeight))/2.0f;
	}
	else
	{
		fleft = 0.0f;
		ftop = 0.0f;
		fright = 1.0f;
		fbottom = 1.0f;
		fwidth = (float)m_UiTexFileHeader.nWidth;
		fheight = (float)m_UiTexFileHeader.nHeight;
		X = (Width - ((float)m_UiTexFileHeader.nWidth))/2.0f;
		Y = (Height - ((float)m_UiTexFileHeader.nHeight))/2.0f;
	}

	CUSTOMVERTEX customvertex[]={
		//{X,Y,0.0f,1.0f,0.0f,0.0f},
		//{X+m_UiTexFileHeader.nWidth,Y,0.0f,1.0f,1.0f,0.0f},
		//{X,Y+m_UiTexFileHeader.nHeight,0.0f,1.0f,0.0f,1.0f},
		//{X+m_UiTexFileHeader.nWidth,Y+m_UiTexFileHeader.nHeight,0.0f,1.0f,1.0f,1.0f}
		{X,Y,0.0f,1.0f,						fleft, ftop},
		{X+fwidth,Y,0.0f,1.0f,				fright,ftop},
		{X,Y+fheight,0.0f,1.0f,				fleft, fbottom},
		{X+fwidth,Y+fheight,0.0f,1.0f,		fright,fbottom}

	};
	//ȡ�ö��㻺������ָ��
	if (FAILED(m_pVertexBuffer->Lock(0,0,(void**)&pVertexData,0)))
	{
		return false;
	}
	memcpy(pVertexData,customvertex,sizeof(customvertex));

	//����
	m_pVertexBuffer->Unlock();
	return true;
}


bool CreateTexture(LPSTR szfilename){

	if(FAILED(D3DXCreateTextureFromFile(m_pIDirect3DDevice,szfilename,
		&m_pTexture))){
			MessageBox(NULL,"��������ӿڶ���ʧ��.","����",MB_OK);
			return false;
	}

	return true;
}
//��Ⱦ����
bool Render(HWND hWnd)
{
	if (m_pIDirect3DDevice==NULL)
	{
		return false;
	}
	RECT WndRect;
	::GetClientRect(hWnd,&WndRect);
	float Width = (float)(WndRect.right - WndRect.left);
	float Height = (float)(WndRect.bottom - WndRect.top);
	
	if (!ReSetTGAPos(Width,Height)) 
		return false;
	m_pIDirect3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(192,192,192),1.0f,0);
	//��ʼ��Ⱦ
	m_pIDirect3DDevice->BeginScene();
	//��Ⱦ��������
	m_pIDirect3DDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(CUSTOMVERTEX));
	m_pIDirect3DDevice->SetFVF(CUSTOMVERTEX_FVF);
	m_pIDirect3DDevice->SetTexture(0,m_pTexture);
	m_pIDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	RECT IconRect;
	m_IconID=LookupIconIndex(hWnd,&IconRect);

	//�����ɵ�λ������
	if(!IsRectEmpty(&g_IconOldRect))
	{
		D3DXVECTOR2 p1((float)g_IconOldDrawRect.left,(float)g_IconOldDrawRect.bottom);
		D3DXVECTOR2 p2((float)g_IconOldDrawRect.right,(float)g_IconOldDrawRect.bottom);
		D3DXVECTOR2 p3((float)g_IconOldDrawRect.right,(float)g_IconOldDrawRect.top);
		D3DXVECTOR2 p4((float)g_IconOldDrawRect.left,(float)g_IconOldDrawRect.top);
		DrawRectangle(&p1,&p2,&p3,&p4,D3DCOLOR_XRGB(255,0,128),D3DCOLOR_XRGB(0,128,255));
	}

	if(m_IconID != -1)
	{
		D3DXVECTOR2 p1((float)IconRect.left,(float)IconRect.bottom);
		D3DXVECTOR2 p2((float)IconRect.right,(float)IconRect.bottom);
		D3DXVECTOR2 p3((float)IconRect.right,(float)IconRect.top);
		D3DXVECTOR2 p4((float)IconRect.left,(float)IconRect.top);
		DrawRectangle(&p1,&p2,&p3,&p4,D3DCOLOR_XRGB(255,0,0),D3DCOLOR_XRGB(0,255,0));
	}
	HRESULT hr;
	//������Ⱦ
	m_pIDirect3DDevice->EndScene();
	//�û�ǰ���ͼ���滺��������ʾ��ǰ֡
	hr = m_pIDirect3DDevice->Present(&WndRect,NULL,hWnd,NULL);
	return true;
}

void Release()
{
	m_aFrameVector.clear();
	SafeRelease(m_pTexture);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIDirect3DDevice);
	SafeRelease(m_pIDirect3D);
	SafeRelease(m_pLine);
}

HRESULT DrawLine(D3DXVECTOR2* pA,D3DXVECTOR2* pB,D3DCOLOR cl)
{

	D3DXVECTOR2 v[2];
	v[0] = *pA;
	v[1] = *pB;
	
	if (m_pLine == NULL )
	{
		D3DXCreateLine(m_pIDirect3DDevice,&m_pLine);
	}
	
	m_pLine->SetWidth(2);
	m_pLine->Begin();
	m_pLine->Draw(v,2,cl);
	m_pLine->End();


	return S_OK;

}
void DrawRectangle(D3DXVECTOR2* p1,D3DXVECTOR2* p2,D3DXVECTOR2* p3,D3DXVECTOR2* p4, D3DCOLOR cl1, D3DCOLOR cl2)
{
	DrawLine(p1,p2,cl1);
	DrawLine(p1,p4,cl1);
	DrawLine(p2,p3,cl1);
	DrawLine(p3,p4,cl1);

	D3DXVECTOR2 point1(p1->x+2,p1->y-2);
	D3DXVECTOR2 point2(p2->x-2,p2->y-2);
	D3DXVECTOR2 point3(p3->x-2,p3->y+2);
	D3DXVECTOR2 point4(p4->x+2,p4->y+2);
	DrawLine(&point1,&point2,cl2);
	DrawLine(&point1,&point4,cl2);
	DrawLine(&point2,&point3,cl2);
	DrawLine(&point3,&point4,cl2);
}

int ReadUITex(LPCTSTR pcszUITex, int nLoadStyle)
{
	int nResult = false;
	int nRetCode = false;
	int nPosed = 0;
	int nIndexCanchangeable = 0;

	FILE *fpUiTex = NULL;
	size_t uReadIn;

	UITEXFRAMEDATASTRUCTURE FrameInfo;

	int i = 0;
	int j = 0;
	int k = 0;

	switch(nLoadStyle) 
	{
	case LOAD_AS_APPEND:
		nPosed = false;
		nIndexCanchangeable = true;
		break;
	case LOAD_AS_REPLACE:
		nPosed = true;
		nIndexCanchangeable = false;
		break;
	default:
		ASSERT(0);
	}

	fpUiTex = fopen(pcszUITex, "rb");
	KG_PROCESS_ERROR(fpUiTex);

	//MessageBox(NULL,"before uReadIn.","����",MB_OK);
	uReadIn = fread(&m_UiTexFileHeader, sizeof(m_UiTexFileHeader), 1, fpUiTex); //���ܻ�ʧ��
	KG_PROCESS_ERROR(uReadIn == 1);

	//MessageBox(NULL,"before m_aFrameVector.","����",MB_OK);
	m_aFrameVector.reserve(m_UiTexFileHeader.nFrameCount);
	//MessageBox(NULL,"m_aFrameVector.reserve.","����",MB_OK);
	for (i = 0; i < m_UiTexFileHeader.nFrameCount; ++i) 
	{
		uReadIn = fread(&FrameInfo, sizeof(FrameInfo), 1, fpUiTex);
		FrameInfo.dwInfo = 0;
		KG_PROCESS_ERROR(uReadIn == 1);
		m_aFrameVector.push_back(FrameInfo);
	}

	nResult = true;
Exit0:
	if (fpUiTex)
	{
		fclose(fpUiTex);
		fpUiTex = NULL;
	}
	return nResult;
}

bool Setup(HWND hWnd,LPCTSTR lpFileName)
{
	Release();

	SetRectEmpty(&g_IconOldRect);

	m_pIDirect3D = NULL;  
	m_pIDirect3DDevice = NULL;
	m_pVertexBuffer = NULL;
	m_pTexture = NULL;
	m_pLine = NULL;
	m_IconRect.bottom=m_IconRect.left=m_IconRect.right=m_IconRect.top = 0;
	m_aFrameVector.clear();
	g_nFrame = -1;

	if (lpFileName == NULL || lstrlen(lpFileName) == 0)
		return false;

	if (!CreateD3Device(hWnd,false))//��ʱ����ȫ��Ļ
	{
		MessageBox(NULL,"CreateD3Deviceʧ��1.","����",MB_OK);
		return false;
	}

	if(!ReadUITex(lpFileName,1))
	{
		MessageBox(NULL,"ReadUITexʧ��1.","����",MB_OK);
		return false;
	}
	
	LPTSTR pszClip = NULL;
	LPTSTR pszTemp = NULL;
	TCHAR szTgaName[MAX_PATH];

	_tcsncpy_s(szTgaName, lpFileName, sizeof(szTgaName) / sizeof(TCHAR));
	szTgaName[sizeof(szTgaName) / sizeof(TCHAR) - 1] = _T('\0');
	pszClip = _tcsrchr(szTgaName, '\\');
	if (pszClip)
		++pszClip;
	else
		pszClip = szTgaName;
	pszTemp = _tcsrchr(pszClip, '/');
	if (pszTemp)
		pszClip = pszTemp + 1;
	_tcsncpy(pszClip, m_UiTexFileHeader.szTgaFileName, sizeof(szTgaName) / sizeof(TCHAR)- (pszClip - szTgaName));
	szTgaName[sizeof(szTgaName) / sizeof(TCHAR) - 1] = _T('\0');
	if(!CreateTexture(szTgaName))
	{
		MessageBox(NULL,"CreateTextureʧ��1.","����",MB_OK);
		return false;
	}
	if (!InitVertexBuffer())
	{
		MessageBox(NULL,"InitVertexBufferʧ��1.","����",MB_OK);
		return false;
	}
	
	return true;

}
//rect�Ǵ�����ʾ�Ŀͻ������λ��, m_iconrect��ͼ���ڲ������λ��
int LookupIconIndex(HWND hWnd,RECT* rect)
{
	RECT ClientRect;
	::GetClientRect(hWnd,&ClientRect);
	float Width =(float)( ClientRect.right - ClientRect.left);
	float Height = (float)(ClientRect.bottom - ClientRect.top);
	float X = (Width - ((float)m_UiTexFileHeader.nWidth))/2.0f;
	float Y = (Height - ((float)m_UiTexFileHeader.nHeight))/2.0f;
	POINT myPoint;
	::GetCursorPos(&myPoint);
	::ScreenToClient(hWnd, &myPoint);
	if(!IsRectEmpty(&g_IconOldRect))
	{
		g_IconOldDrawRect.left =  (long)X + g_IconOldRect.left;
		g_IconOldDrawRect.right =  (long)X + g_IconOldRect.right;
		g_IconOldDrawRect.top =  (long)Y + g_IconOldRect.top;
		g_IconOldDrawRect.bottom =  (long)Y + g_IconOldRect.bottom;
	}

	for (int i=0;i<(int)m_aFrameVector.size();i++)
	{
		float Min_X = X + m_aFrameVector[i].nLeft;
		float Max_X = X + m_aFrameVector[i].nLeft + m_aFrameVector[i].nWidth;
		float Min_Y = Y + m_aFrameVector[i].nTop;
		float Max_Y = Y + m_aFrameVector[i].nTop + m_aFrameVector[i].nHeight;
		if( Min_X <= myPoint.x && myPoint.x<= Max_X &&  Min_Y <= myPoint.y && myPoint.y <= Max_Y)
		{
			rect->left = (int)Min_X;
			rect->right = (int)Max_X;
			rect->top = (int)Min_Y;
			rect->bottom = (int)Max_Y;
			m_IconRect.left = m_aFrameVector[i].nLeft;
			m_IconRect.right = m_aFrameVector[i].nLeft + m_aFrameVector[i].nWidth;
			m_IconRect.top = m_aFrameVector[i].nTop;
			m_IconRect.bottom = m_aFrameVector[i].nTop + m_aFrameVector[i].nHeight;
			return i;
		}
	}
	rect->left = rect->right = rect->bottom = rect->top = 0;
	m_IconRect.left =m_IconRect.right = m_IconRect.bottom = m_IconRect.top = 0;
	return -1;
}

