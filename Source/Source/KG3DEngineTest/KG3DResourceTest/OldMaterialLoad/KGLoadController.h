//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KGLoadController.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-6-10 10:00
//  Comment     :   ��װ���ؾɵ�����Dll���ṩ��ü��غ�Ĳ������ݵĽӿڳ������ʵ����
//					���صͰ汾������dll�����þɰ汾�ļ��ؽӿڼ��ز�����Դ
//					�����SVN�汾��ǩ��t_jx3_built_1-0-8-1428
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KGLoadController_H__
#define _KGLoadController_H__

#include "stdafx.h"
#include "IEOther.h"
#include "IEKG3DMaterial.h"
#include "IKGLoadController.h"

class KGLoadController : public IKGLoadController
{
public:
	KGLoadController();
	~KGLoadController();

	virtual int   Init(const char cszClientPath[]);
	virtual void  UnInit();
	virtual int   Reset();
	virtual int   LoadMaterialByOldFunc(const char cszFileName[]);
	virtual DWORD GetNumMaterials();
	virtual int   GetSubset(OldMaterialSubset** ppOldMaterialSubset, const UINT cuIndex);
	virtual int   GetBaseData(BaseMtlData** ppBaseMtlData);

private:
	int  SetMaterialData();
	void CleanVecSubset();
	//��ʽ��Ŀ¼
	int  FormatPath(char* pszPath, const size_t cuSize);

private:
	HMODULE m_hEngineDLL;
	IEKG3DEngineManager* m_pEngineMgr;
	IEKG3DMaterial*		 m_pMaterial;
	DWORD m_dwNumMaterials;
	vector<OldMaterialSubset> m_vecMTLSubset;
	BaseMtlData m_BaseMtlData;
};

extern KGLoadController g_cKGLoadController;

#endif