//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KG3DEngineHelper.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-10-10 10:00
//  Comment     :   3D����ӿڵ��õĲ�����
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KG3DEngineHelper_H__
#define _KG3DEngineHelper_H__

#include "KGTestCommon.h"
#include "KGFileManager.h"

#include "KG3DInterface.h"
#include "KG3DSceneDataInterface.h"
#include "IEKG3DTerrain.h"

class KG3DEngineHelper
{
public:
	KG3DEngineHelper(void);
	~KG3DEngineHelper(void);

	int  Init(const char cszClientPath[]);
	int  UnInit();

	//����Ĭ�ϵĵ�ͼ����
	int  CreateMap(IKG3DScene** pOutScene, const char cszResourcePath[], int nModelToCreate);
	//���ص�ͼ������Ϣ��������ģ������
	int  LoadMapInfo(const char cszMapName[], IKG3DScene** pOutScene);
	//����������ͼ�������ݣ�����ģ������
	int  LoadMapData(const char cszMapName[], IKG3DScene** pOutScene);
	//�����ͼ����
	int  SaveMap(const char cszMapName[], IKG3DScene* pOutScene);
	//�ͷų�����Դ
	int  RemoveScene(IKG3DScene* pScene);

	//����ļ��غ�ж��
	int  LoadEngineDLL();
	int  UnLoadEngineDLL();

private:
	HMODULE m_hEngineDLL;
	IKG3DEngineManager* m_pEngineMgr;
	char m_szClientPath[MAX_PATH];
	vector<string> m_vecResNameList;			//�����½�������ģ�������б�
};

#endif