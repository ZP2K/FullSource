#ifndef TGALoader_Include
#define TGALoader_Include

#ifdef UITEXTGALOADER_EXPORTS
#define UITEXTGALOADER_API __declspec(dllexport)
#else
#define UITEXTGALOADER_API __declspec(dllimport)
#endif
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <tchar.h>
#include "KGPublic.h"
#include "UiTex.h"



#define CUSTOMVERTEX_FVF (D3DFVF_XYZRHW|D3DFVF_TEX1)  //����ɱ䶥���ʽ�ĺ�
#define SafeRelease(pObject) if (pObject != NULL){pObject->Release();pObject=NULL;}

enum 
{
	LOAD_AS_APPEND = 0,
	LOAD_AS_REPLACE = 1,
	READ_TEXT = 1,
	NOT_READ_TEXT = 0,
	FIND_ICON_FILE = 1,
	NOT_FIND_ICON_FILE = 0,
};

struct CUSTOMVERTEX //����ṹ��
{
	float x,y,z,rhw;  //��������
	float u,v;  //�������������
};

bool CreateD3Device(HWND,bool bFullScreen = true);//����Direct3D�豸
bool CreateTexture(LPSTR szfilename);//�����������
bool InitVertexBuffer(); //��ʼ�����㻺����
bool ReSetTGAPos(float Width,float Height);
HRESULT DrawLine(D3DXVECTOR2* pA,D3DXVECTOR2* pB,D3DCOLOR cl);
void DrawRectangle(D3DXVECTOR2* p1,D3DXVECTOR2* p2,D3DXVECTOR2* p3,D3DXVECTOR2* p4, D3DCOLOR cl1,D3DCOLOR cl2);
int ReadUITex(LPCTSTR pcszUITex, int nLoadStyle);
extern "C" UITEXTGALOADER_API void Release(); //�ͷ�Direct3D����
extern "C" UITEXTGALOADER_API int LookupIconIndex(HWND hWnd,RECT* rect);
extern "C" UITEXTGALOADER_API bool Setup(HWND hWnd,LPCTSTR lpFileName);
extern "C" UITEXTGALOADER_API bool Render(HWND hWnd);//��Ⱦ����
extern "C" UITEXTGALOADER_API int GetCurrenIconID();
extern "C" UITEXTGALOADER_API RECT GetCurrenIconRect();
extern "C" UITEXTGALOADER_API void SetOldIconRect(RECT rect);
extern "C" UITEXTGALOADER_API SIZE GetCurrentImageSize();
extern "C" UITEXTGALOADER_API void SetRenderFrame(int nFrame);
extern "C" UITEXTGALOADER_API SIZE GetCurrentFrameSize();
#endif