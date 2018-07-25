#pragma once
#include "KG3DInterface.h"
struct FaceMotionInfo
{
	TCHAR strName[MAX_PATH];//���������
	TCHAR strSuffix[MAX_PATH];//����Ķ����ļ��ĺ�׺
};

struct DefaultFaceMotion
{
	TCHAR strSuffix[MAX_PATH];//��׺����
	float fPlayRate;//���ż���
	int nSpan;//����Ƿ�Ҫ���ŵļ��ʱ��
};

class KG3DModel;
struct DefaultFaceMotionMgrItem
{
	KG3DModel *pModel;
	DWORD dwLastCheck;
	DefaultFaceMotionMgrItem()
	{
		pModel = NULL;
		dwLastCheck = 0;
	}
};

class KG3DFaceMotionSettings : public IKG3DFaceMotionSettings
{
public:
	KG3DFaceMotionSettings();
	~KG3DFaceMotionSettings();

	HRESULT Init();
	HRESULT ApplyDefaultMotion(DefaultFaceMotionMgrItem& FaceMotionItem);
	HRESULT GetFaceMotion(DWORD dwType,
		LPCSTR strFaceMeshName,
		LPSTR strFaceMotionAnimationName);
	virtual LPCSTR GetFaceMotionTypeName(DWORD dwIndex);
	virtual LPCSTR GetFaceMotionSuffixName(DWORD dwIndex);
	virtual LPCSTR GetFaceMotionModifyPath();
	virtual void GetFaceMotionPathFromFacePath(LPSTR strFacePath);
	virtual DWORD GetNumFaceMotion();
	void RefreshIgnoreFile();
private:
	DWORD GetFaceMotionType(LPCSTR strTypeName);
	const static DWORD s_dwInvalidateType = 0xffffffff;
	std::vector<FaceMotionInfo> m_FaceMotion;
	std::vector<DefaultFaceMotion> m_DefaultMotion;
	//�����Ǹ�������mesh�ļ����Զ��Ҷ����ļ���Ҫ�Ĳ���
	TCHAR m_strFaceMotionModifyPath[MAX_PATH];//��Ҫ��ӵ�Ŀ¼
	int m_nPathDepth;//��Ҫ���ݶ��ټ�Ŀ¼
	std::set<DWORD> m_IgnoreFile;
};

class KG3DDefaultFaceMotionMgr
{
public:
	KG3DDefaultFaceMotionMgr();
	~KG3DDefaultFaceMotionMgr();
	HRESULT UnInit();
	void SetDefaultFaceMotion(KG3DModel *pModel);
	void FrameMove();
private:
	std::vector<DefaultFaceMotionMgrItem> m_FaceModels;
	std::vector<DefaultFaceMotionMgrItem> m_FaceModelsBack;
	std::vector<DefaultFaceMotionMgrItem>* m_pCurrentFaceModelList;
	std::vector<DefaultFaceMotionMgrItem>* m_pBackFaceModelList;
};

extern KG3DFaceMotionSettings g_FaceMotionSettings;
extern KG3DDefaultFaceMotionMgr g_DefaultFaceMotionMgr;