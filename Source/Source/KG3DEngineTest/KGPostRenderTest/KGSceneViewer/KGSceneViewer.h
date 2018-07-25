//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KGSceneViewer.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-7-28 10:00
//  Comment     :   �����������ʵ���˳����ļ��غ���Ⱦ����Ӧ����������������ڳ��������ߣ��ͶԳ���������Ļ��Ч�Ĺ���
//
//////////////////////////////////////////////////////////////////////////////////////

#include "KGTestCommon.h"
#include "KG3DInterface.h"
#include "KG3DSceneDataInterface.h"
#include "IEEditor.h"

const TCHAR TEST_PROJECT_NAME[] = "KGSceneViewer";

class KGSceneViewer
{
public:
	KGSceneViewer(void);
	~KGSceneViewer(void);

	int  Init(HWND hWindow);
	int  LoadScene(const TCHAR cszMapPathName[]);
	int  Run();
	void UnInit();
	int  KeyListener(UINT_PTR uKeyValue);
	int  OnMouseMove(WPARAM wParam, LPARAM lParam);
	int  SetMouseState(UINT uMsg, LPARAM lParam);

private:
	IKG3DEngineManager*	m_pEngineMgr;
	HMODULE m_hEngineDLL;
	HWND m_hWindow;
	IKG3DScene* m_pScene;
	IEKG3DSceneOutputWnd* m_pCurOutputWnd;

	bool m_bEnableEffect;			//�����Ч״̬
	bool m_bRButton;				//����Ҽ�״̬(true:down false:up)
	POINT m_ptLastMousePos;
	POINT m_ptCurrentMousePos;
	TCHAR m_szSceneInfo[1024];		//������Ϣ(Ŀǰֻ�ռ��������λ�úͳ�����Ϣ)
	TCHAR m_szResultPath[MAX_PATH];	//��ͼ���Ŀ¼
	int m_nPictureIndex;			//��ͼ�������������ɲ��ظ���ͼƬ�ļ���
	bool m_bKMemoryInited;			//�ڴ�����ʼ�����
};
