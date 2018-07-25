//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KGTestEffect.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-7-28 10:00
//  Comment     :   ��װ������Ļ��Ч�������������������Ŀ������ݽ���������ִ�У��ȼ��ز��Գ�����
//					�ռ���Ⱦǰ����Ⱦ��Ĳ������ݣ����������
//					
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KGTestEffect_H_
#define _KGTestEffect_H_

#include "KGTestCommon.h"
#include "KG3DInterface.h"
#include "IEKG3DSceneOutputWnd.h"

class KGTestEffect
{
public:
	KGTestEffect();
	~KGTestEffect();

	int  Init(IKG3DEngineManager*	cpEngineMgr,
			  const HWND			chRenderWindow,
			  const TCHAR			cszResultFile[],
			  const TCHAR			cszMapName[],
			  const DWORD			cdwEffectType,
			  const int				cnTotalFrame);
	int  Run();
	void UnInit();

	int  AddCaseData(int nTestFrame,
		D3DXVECTOR3 vCameraPos,
		D3DXVECTOR3 vLookAtPos);
	void SetDefaultCameraPos(D3DXVECTOR3 vCameraPos) {m_CaseData.vDefaultCameraPos = vCameraPos;}
	void SetDefaultLookAtPos(D3DXVECTOR3 vLookAtPos) {m_CaseData.vDefaultLookAtPos = vLookAtPos;}
 	bool HasEffect(const DWORD dwEffectType) {return !!(m_dwEffectType & dwEffectType);}
	string GetCaseName() {return m_strCaseName;}

	struct FrameData						//֡���ݽṹ
	{
		int nFrameCount;					//��ǰ֡��
		float nAverageFPS;					//ƽ��֡��
		float nFPS;							//��֡FPS
		DWORD dwBeginTime;					//��֡��ʼʱ��
		DWORD dwLastTime;					//��Ⱦ����ʱ��
		DWORD dwEndTime;					//��֡����ʱ��
	};

	struct CaseData							//�������ݽṹ
	{
		int nDefaultTotalTestFrame;		//Ĭ�ϲ�����֡��
		D3DXVECTOR3 vDefaultCameraPos;	//Ĭ�������λ��
		D3DXVECTOR3 vDefaultLookAtPos;	//Ĭ�����������
		vector<int> vecTestFrame;			//��Ҫ�ռ�֡���ݵ�֡��
		vector<D3DXVECTOR3> vecCameraPos;	//�����λ��
		vector<D3DXVECTOR3> vecLookAtPos;	//���������
	};
private:
	//��Ⱦ֡����
	int BeforeFrame();
	int OnFrameMove();
	int AfterFrame();
	//////////////////////////////////////////////////////////////////////////

	//���������Ļ��Ч�����
	int TestSetEffectProcess();

	//������Ļ��Ч
	int DisableAllEffect();
	int EnableEffect(bool bEnable);
	//////////////////////////////////////////////////////////////////////////

	//�����������
	int MakeupCaseName();

	//����֡����
	int AnalyseFrameData();

	//���ÿ������֡����Ϣ
	int OutputResult();

	//��������
	int LoadScene();
	int SetupScene();
	int RemoveScene();
	//////////////////////////////////////////////////////////////////////////

private:
	string m_strCaseName;							//������������
	TCHAR m_szTestSceneName[MAX_PATH];				//���Գ�������
	TCHAR m_szResultFile[MAX_PATH];					//���ݽ������ļ�
	TCHAR m_szResultPath[MAX_PATH];					//������Ŀ¼

	int m_nCurrentFrame;							//��ǰ��֡��
	DWORD m_dwTestingBeginTime;						//���ԵĿ�ʼʱ��
	FrameData m_CurrentFrameData;					//��ǰ֡����
	CaseData m_CaseData;							//����������������
	DWORD m_dwEffectType;							//���Ե���Ч����
	int m_nTestFrameIndex;							//���Ե�֡����

	vector<string> m_vecTestProcessResult;			//������Ч������֤���

	IKG3DEngineManager* m_pEngineMgr;
	IKG3DScene* m_pScene;
	IEKG3DSceneOutputWnd* m_pCurOutputWnd;
	HWND m_hRenderWindow;
	ITabFile* m_pFile;								//������TAB�ļ�ָ��
	static int m_nResultLine;						//������������

};

#endif