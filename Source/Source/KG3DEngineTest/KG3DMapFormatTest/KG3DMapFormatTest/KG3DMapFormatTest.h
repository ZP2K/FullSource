//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KG3DMapFormatTest.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-9-14 10:00
//  Comment     :   ��ͼ�����¸�ʽ�Ĳ��ԡ�
//					��Ҫʵ�������������������ء������¾ɸ�ʽ��ͼ�����Ĺ��ܡ�
//					ͨ�����Խӿڻ�ȡ�ĸ��ֵ�ͼ�������ݲ��������ݱȶ���֤����֤�¾ɵ�ͼ�ڴ��������ء�����Ľ����ȷ
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KG3DMapFormatTest_H__
#define _KG3DMapFormatTest_H__

#include "KGFileManager.h"
#include "KG3DEngineHelper.h"

#include "..\KGOldEngineController\IKG3DEngineController.h"

const char TEST_PROJECT_NAME[] = "KG3DMapFormatTest";

class KG3DMapFormatTest
{
public:
	KG3DMapFormatTest(void);
	~KG3DMapFormatTest(void);

	int  Init(const char cszClientPath[]);
	int  Run();
	void UnInit();

private:
	//��������/////////////////////////////////////////////////////////////////
	//��֤�¾�������ؾɸ�ʽ��ͼ����������
	int  ValidateLoadOldMapByDifferEngine(const char cszOldMapName[]);
	//��֤�����½��ĵ�ͼ����(�����¾ɸ�ʽ)
	int  ValidateSaveCreatedMap(int nPackFileSign);
	//��֤�ش��Ѵ��ڵĵ�ͼ����(�����¾ɸ�ʽ)
	int  ValidateSaveExistMap(int nPackFileSign, const char cszMapName[]);
	//��֤�ɸ�ʽ��ͼת��Ϊ�¸�ʽ��ͼ(����ת�벻ת)
	int  ValidateOldFormatTransformNew(const char cszOldMapName[], bool bTransformNew);
	//////////////////////////////////////////////////////////////////////////

	
	//��ͼ������Դ���ݱȽ�(�¾�������ص���Դ�Ƚ�)
	int  CompareDifferEngineMapData(IKG3DScene* pNewEngineScene, IKG3DEngineController* pOldEngineController);
	int  CompareDifferEngineEntityInfo(IEKG3DSceneOutDoorSpaceMgr* pNewEngineOutDoorMgr, IKG3DEngineController* pOldEngineController);
	int  CompareDifferEngineTerrainInfo(IEKG3DSceneOutDoorSpaceMgr* pNewEngineOutDoorMgr, IKG3DEngineController* pOldEngineController);

	//��ͼ������Դ���ݱȽ�
	int  CompareMapData(IKG3DScene* pScene_1, IKG3DScene* pScene_2);
	int  CompareEntityInfo(IEKG3DSceneOutDoorSpaceMgr* pOutDoorMgr_1, IEKG3DSceneOutDoorSpaceMgr* pOutDoorMgr_2);
	int  CompareTerrainInfo(IEKG3DSceneOutDoorSpaceMgr* pOutDoorMgr_1, IEKG3DSceneOutDoorSpaceMgr* pOutDoorMgr_2);
	int  CompareSceneBlockInfo(IEKG3DSceneOutDoorSpaceMgr* pOutDoorMgr_1, IEKG3DSceneOutDoorSpaceMgr* pOutDoorMgr_2);
	int  CompareEntityData(IEKG3DSceneOutDoorSpaceMgr* pOutDoorMgr_1, IEKG3DSceneOutDoorSpaceMgr* pOutDoorMgr_2);
	int  CompareDataBuffer(PVOID pData_1, PVOID pData_2, DWORD dwLenth);
	int  CompareMapFiles(const char cszMapPath_1[], const char cszMapPath_2[]);

	//�¸�ʽ�ļ�����֤(��֤�Ƿ񱣴�Ϊmap.TerrainInfo��map.EntityInfo)
	int  CheckNewFormatFile(const char cszMapName[]);
	int  CheckMapVersion(const char cszMapName[], DWORD dwCheckVersion);
	int  LoadMapList();
	int  LoadCaseSetting();
	int  OutputResult();
	int  CleanSaveMap(const char cszMapName[]);

	//�����̲߳���MessageBox���ڷ�����Ϣģ�ⰴ��Yes��ť
	//����������ValidateOldFormatTransformNew�н��ɸ�ʽ��ͼ��������Ϊ�¸�ʽʱ������MessageBox����ʾ��Ҫȷ��
	int  CreateWindowFinderThread();
	static void FinderRun(void *pvArg);
	int DestroyThread();

private:
	struct CaseSetting 
	{
		int nTestingAllMap;				//1���������е�ͼ 0�������ȡĳ����ͼ����
		int nMaxModelToCreate;			//��ӵ������е����ģ������
		int nMinModelToCreate;			//��ӵ������е���Сģ������
		int nMaxOldMapID;				//MapList.tab�оɵ�ͼ���ƶ�Ӧ�����ID��
		char szResourcePath[MAX_PATH];	//��ӵ������е�ģ����Դ
	};
	CaseSetting m_CaseSetting;
	struct CaseInfo
	{
		string strCaseName;
		string strCaseInfo;
		string strCaseResult;
	};
	
	char m_szConfigFullName[MAX_PATH];
	char m_szClientPath[MAX_PATH];
	char m_szTestMapFullName[MAX_PATH];
	vector<string> m_vecOldMapList;				//�ɳ��������б�
	vector<string> m_vecNewMapList;				//�³��������б�
	vector<CaseInfo> m_vecTestResult;				//�����������н��
	bool m_bKMemoryInited;						//�ڴ�����ʼ��״̬
	bool m_bNeedToDelKey;						//���Խ����Ƿ���Ҫɾ��config.ini���nPackFile��
	KG3DEngineHelper m_EngineHelper;			//3D����ӿڲ�����װ
	KThread* m_pWorkThread;
	static bool m_bTransformNew;				//�Ƿ�ת�����¸�ʽ��ͼ����
};

#endif