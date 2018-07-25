//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KGTestMapDisuseResource.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-7-03 10:00
//  Comment     :   1. ͨ������KG3DEngine_AutoTest.dll�����ü���.map�Ľӿڣ�����.EntityInfo��.TerrainInfo�ļ��� 
//					2. ��.EntityInfo��.TerrainInfo�ļ��з��������õ���Դ�ļ�����
//					   ����data/source/map_source����Դ�б����������������û�б����õ���Դ�ļ����� 
//					3. ���û�б����õ���Դ�ļ����б�
//					4. ���ȱʧ����Դ�ļ����б�
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KGTestMapDisuseResource_H__
#define _KGTestMapDisuseResource_H__

#include "KGTestCommon.h"
#include "KG3DInterface.h"
#include "IEKG3DTerrain.h"

const char TEST_PROJECT_NAME[] = "KG3DMapsResourceTest";

class KGTestMapDisuseResource
{
public:
	KGTestMapDisuseResource();
	~KGTestMapDisuseResource();
	
public:
	int  Init();	
	int  RunTest();
	int  OutputResult();
	void UnInit();

private:
	int  LoadEngineDLL();
	int  LoadTestSetting();
	int  AnalyseResType(const char cszResType[]);
	int  FindSourceResource();
	//����ָ����������Դ(.sfx .SoundBall .ModelSet .Mesh .Mtl .pvs��
	int  LoadSourceResourceInfo(const char cszPath[]);
	///////////////////////////////////////////////////////////////////////////
	//���ҳ����ļ�(.map)�����õ�ָ����Դ
	int  FindUsedMapResource();
	int  AnalyseOneMapResourceInfo(const char cszMapName[]);
	int  AnalyseEntityInfo(IEKG3DSceneOutDoorSpaceMgr* pIEOutDoorMgr);
	//int  AnalyseTerrainInfo(IEKG3DSceneOutDoorSpaceMgr* pIEOutDoorMgr);
	//////////////////////////////////////////////////////////////////////////
	//�Ա�������Դ�ͳ�����ʹ�õ���Դ���ҳ�δʹ�õ���Դ
	int  CheckDisuseResource();
	int  CheckLostResource();

	//����������Դ�ļ�
	int  FindResource(const char cszResourceName[], set<string>& setResList);
	int  FindResInModelset(const char cszResourceName[], set<string>& setResList);
	int  FindResInPVS(const char cszResourceName[], set<string>& setResList);
	int  FindResInSTree(const char cszResourceName[], set<string>& setResList);
	int  FindResInMDL(const char cszResourceName[], set<string>& setResList);
	int  FindResInSFX(const char cszResourceName[], set<string>& setResList);

	//���
	int  OutputDisuseRes(const char cszResultPath[]);
	int  OutputLostRes(const char cszResultPath[]);

private:
	HMODULE m_hEngineDLL;
	IKG3DEngineManager* m_pEngineMgr;
	char m_szClientPath[MAX_PATH];
	map<string, vector<string>> m_mapSourceRes;		//������Դ����Ŀ¼��������Դ���� string:��Դ���ͣ���չ���� vector<string>:��Դ�ļ�������
	set<string> m_setUsedRes;						//���г���ʹ�õ���Դ
	vector<string> m_vecLODRes;						//LOD��Դ����*_L.Mesh *_L.Mtl
	vector<string> m_vecMapList;					//���ĵ�ͼ���ƣ���������ļ�û���ã���ΪMapList.tab�ļ��м�¼�ĵ�ͼ����
	vector<string> m_vecResType;					//������Դ����
	vector<string> m_vecResPath;					//������Դ����Ŀ¼
	bool m_bFindDisuseMapRes;						//�Ƿ���ҷ�����ͼ��Դ
};
#endif