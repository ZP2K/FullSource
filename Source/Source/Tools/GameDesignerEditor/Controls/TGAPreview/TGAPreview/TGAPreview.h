// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TGAPREVIEW_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TGAPREVIEW_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TGAPREVIEW_EXPORTS
#define TGAPREVIEW_API __declspec(dllexport)
#else
#define TGAPREVIEW_API __declspec(dllimport)
#endif

//extern TGAPREVIEW_API int nTGAPreview;
//TGAPREVIEW_API int fnTGAPreview(void);

extern "C" TGAPREVIEW_API BOOL Init(HWND hRenderWnd);
extern "C" TGAPREVIEW_API void UnInit(HWND hRenderWnd); //�����nullΪ�����������еĴ���Ԥ��ȫ������ʼ��
extern "C" TGAPREVIEW_API BOOL SetFileNames(HWND hRenderWnd, char* Filenames, INT nWidth, INT nHeight); //��\r\n�ָ����ļ����б�
extern "C" TGAPREVIEW_API void OnResize(HWND hRenderWnd);
extern "C" TGAPREVIEW_API void FrameMove(HWND hRenderWnd);
extern "C" TGAPREVIEW_API void Render(HWND hRenderWnd);
extern "C" TGAPREVIEW_API BOOL ExportToFile(HWND hRenderWnd, INT nWidth, INT nHeight, char* Filenames);
