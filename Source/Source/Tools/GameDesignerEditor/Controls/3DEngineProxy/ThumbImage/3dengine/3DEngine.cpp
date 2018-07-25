/*------------------------------------------------------------
3d engine for creat bitmap for meshfile
------------------------------------------------------------*/


#include "EngineProxy.h"

//�����������ڣ�һ�����������ļ����ƣ���һ�������ļ����ƣ�������õ��ļ�������ͼ��Ȼ��ش�

#define IDTIFY_SHARE_FILE_MAPPING_NAME "my_shared_memory_with_3dengine"
#define MAX_SHARE_SIZE 4096

//�����ļ����ƴ�������
#define SendFileNameWindowName "SendFileNameWindowName"
//�����ļ����ƴ�����������
#define SendFileNameWindowClassName "SendFileNameWindowClassName"

//���洰������
#define EngineWindowName "_EngineWindowName_For_Mesh_"
//���洰����������
#define EngineWindowClassName "EngineWindowClassName"

#define StartEngineProcessOK "start_3dengine_for_mesh_ok"

//���ڻص�����Ϣ��������
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

//�ļ�ӳ����
HANDLE g_hMapFile;
//3d���洰�ھ��
HWND g_hWnd_3dEngine = NULL;

//�����ļ����ƴ��ھ��
HWND g_hWnd_SendFile = NULL;

//��һͨ�ŵ���Ϣ����
//���߷����ļ����ƴ������洰�ڵľ��
#define WM_TELL_HWND			WM_USER + 100
#define WM_SEND_MESH_FILE_NAME	WM_USER + 101
#define WM_CREATE_BMP_OK		WM_USER + 102
#define WM_ASK_HWND				WM_USER + 103
#define WM_REINIT_ENGINE		WM_USER + 104
#define WM_SEND_ANI_FILE_NAME	WM_USER + 105
#define WM_ANIMATION_FILE		WM_USER + 106

extern BOOL g_bAnimationReady;

void* g_lpMapAddr = NULL;

char g_szMeshFileName[256];
char g_szAniFileName[MAX_PATH];

void MyExit();

//��ʼ�������ڴ�
BOOL InitShareMemory()
{
	BOOL bResult = TRUE;

	g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _T(IDTIFY_SHARE_FILE_MAPPING_NAME));


	if (g_hMapFile == NULL)
	{
		g_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MAX_SHARE_SIZE, _T(IDTIFY_SHARE_FILE_MAPPING_NAME));
	}

	if (g_hMapFile == NULL )
	{
		bResult = FALSE;
	}
	else
	{
		if (g_lpMapAddr)
		{
			::UnmapViewOfFile(g_lpMapAddr);
		}
		LPVOID lpMapAddress = MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		g_lpMapAddr = lpMapAddress;
	}

	
	return bResult;
}
//��ȡ�����ڴ�
BOOL GetShareAddr()
{
	BOOL bResult = FALSE;
	g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _T(IDTIFY_SHARE_FILE_MAPPING_NAME));
	if (g_hMapFile != NULL)
	{
		LPVOID lpMapAddress = MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		g_lpMapAddr = lpMapAddress;
		bResult = TRUE;
	}

	return bResult;
}

//д��MapFile
BOOL WriteToMapFile(void* pVoid, size_t uLen)
{
	BOOL bResult = TRUE;
	ASSERT(g_hMapFile != NULL);	
	if (pVoid == NULL)
	{
		return FALSE;
	}

	memset(g_lpMapAddr, 0, MAX_SHARE_SIZE);	 
	memcpy(g_lpMapAddr, pVoid, uLen);
	FlushViewOfFile(g_lpMapAddr, uLen + 1);

	return bResult;

}

BOOL ReadFromMapFile(void* pVoid, size_t uLen)
{
	BOOL bResult = TRUE;
	ASSERT(g_hMapFile != NULL);	
	if (pVoid == NULL)
	{
		return FALSE;
	}
	
	memset(pVoid, 0, uLen);
	memcpy(pVoid, g_lpMapAddr, uLen);
	

	return bResult;
}
EngineProxy* g_pEngineProxy = NULL;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("3d_engine") ;
	MSG          msg ;
	WNDCLASS     wndclass ;

	wndclass.style         = CS_VREDRAW | CS_HREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
			szAppName, MB_ICONERROR) ;
		return 0 ;
	}

	g_hWnd_3dEngine = CreateWindow (szAppName,                  // window class name
		TEXT (""), // window caption
		WS_OVERLAPPEDWINDOW,        // window style
		CW_USEDEFAULT,              // initial x position
		CW_USEDEFAULT,              // initial y position
		220,                         // initial x size
		220,                         // initial y size
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL) ;                     // creation parameters

	
	ShowWindow (g_hWnd_3dEngine, iCmdShow) ;
	UpdateWindow (g_hWnd_3dEngine) ;

	

	g_pEngineProxy = new EngineProxy();
	BOOL bInitProxy = FALSE;
	bInitProxy = g_pEngineProxy->Start(g_hWnd_3dEngine);
	if (!bInitProxy)
	{
#ifdef DEBUG
		MessageBox(g_hWnd_3dEngine, "Init EngineProxy failed", "quit", MB_ICONERROR);
#endif		
		MyExit();

		return 1;
	}

	if (!InitShareMemory())
	{
		MyExit();
		return 1;
	}
	
	::SetWindowText(g_hWnd_3dEngine, _T(EngineWindowName));

	BOOL bRet = false;
	while (bRet = GetMessage (&msg, NULL, 0, 0))
	{
		if (bRet == -1)
		{


		}
		else
		{
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		}
		
	}
	return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SEND_MESH_FILE_NAME:
		{
			ReadFromMapFile(g_szMeshFileName, sizeof(g_szMeshFileName) * sizeof(char));
		
	
			SIZE ssss;
			ssss.cx = 96;
			ssss.cy = 96;
			BOOL bCreateBmpOK = g_pEngineProxy->CreateThumbnail(ssss, g_szMeshFileName);
			LRESULT lRet = S_FALSE;
			if (bCreateBmpOK)
			{
				lRet = S_OK;
			}
			else
			{
				lRet = S_FALSE;
			}
			return lRet;
		}
	case WM_SEND_ANI_FILE_NAME:
		{
			if (g_bAnimationReady && strcmp((char*)g_lpMapAddr, g_szAniFileName) != 0)
			{
				ReadFromMapFile(g_szAniFileName, sizeof(g_szAniFileName) * sizeof(char));
				return S_OK;
			}
			else 
			{
				return S_FALSE;
			}
		}
	case WM_ANIMATION_FILE:
		{
			ReadFromMapFile(g_szMeshFileName, sizeof(g_szMeshFileName) * sizeof(char));

			SIZE ssss;
			ssss.cx = 300;
			ssss.cy = 300;
			BOOL bCreateBmpOK = g_pEngineProxy->CreateAnimationFile(ssss, g_szMeshFileName, g_szAniFileName);
			LRESULT lRet = S_FALSE;
			if (bCreateBmpOK)
			{
				lRet = S_OK;
			}
			else
			{
				lRet = S_FALSE;
			}
			return lRet;
		}
	case WM_DESTROY:
		MyExit();
		PostQuitMessage (0) ;
		return 0 ;
	case WM_REINIT_ENGINE:
		if (g_pEngineProxy)
		{
			g_pEngineProxy->Stop();
			BOOL bInitProxy = FALSE;
			bInitProxy = g_pEngineProxy->Start(g_hWnd_3dEngine);
			if (!bInitProxy)
			{
				MessageBox(g_hWnd_3dEngine, "Init EngineProxy failed", "quit", MB_ICONERROR);
				MyExit();

				return 1;
			}
		}
		
		return 0;

	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

void MyExit()
{
	if (g_pEngineProxy)
	{
		g_pEngineProxy->Stop();
		delete g_pEngineProxy;
		g_pEngineProxy = NULL;
	}
	if (g_lpMapAddr)
	{
		UnmapViewOfFile(g_lpMapAddr);
		g_lpMapAddr = NULL;
	}
	if (g_hMapFile)
	{
		CloseHandle(g_hMapFile);
		g_hMapFile = NULL;
	}

}