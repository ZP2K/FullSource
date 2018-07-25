//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KGMapResSearch.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-8-27 10:00
//  Comment     :   ��ͼ������Դ�������߽�����߼�������
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KGMapResSearch_H__
#define _KGMapResSearch_H__

#include "resource.h"
#include "stdafx.h"
#include "KG3DInterface.h"
#include "IEKG3DTerrain.h"
#include <commctrl.h>

class KGMapResSearch
{
public:
	KGMapResSearch(void);
	~KGMapResSearch(void);

	int  Init(HINSTANCE hInstance, IKG3DEngineManager* pEngineMgr, const char cszClientPath[]);
	void UnInit();

	int  InitUI(HWND hDlg);
	int  ListViewUpdate(HWND hWnd, LPARAM lParam);
	//���ö���Դ��ѯ
	int  EnableMultiSearch();
	//���õ���Դ��ѯ
	int  EnableSingleSearch();
	//����Դ�б�������ָ����Դ��Ϣ
	int  Search();
	//��ListView��ѡ�еĵ�ͼ����
	int  OpenMap();
	//����ListView����Ϣ
	int  CatchListViewMessage(WPARAM wParam, LPARAM lParam);
	//����ĳ�����������¼�����а棬Ŀǰֻ������Դ����
	int  CopyResult(WPARAM wParam);

private:
	//��Դ��ϸ��Ϣ���ݽṹ
	struct ResInfo
	{
		vector<string> MapNameList;				//��Դ���ڵĵ�ͼ�����б�
		vector<D3DXVECTOR3> PositionList;		//��Ӧ��ͼ�е���Դλ��
	};
	//�����û�����
	bool MultiSearchAnalyseInput();
	bool SingleSearchAnalyseInput();
	//�������Դ���ҽ��
	int OutputMultiSearchResult();
	//��ȡҪ��������Դ�б�ȫ·������������г���Ŀ¼��
	int  ReadResList(const char cszListFileName[]);
	//������Դ���͵����
	int  AddRelationType();

	//�����û������������Դ����ɸѡ�����е�ͼ������ʹ�õ���Դ
	int  FindResInAllMap();
	int  AnalyseOneResourceInfoInOldMap(const char cszMapName[]);
	int  AnalyseEntityInfoInOldMap(IEKG3DSceneOutDoorSpaceMgr* pIEOutDoorMgr);
	int  InsertUsedResource(const char cszResourceName[]);

	//��*.map�ļ�����������ͼ������ÿ����Դ����λ��
	int  FindResPosInMapFile(const char cszMapName[]);

	//��ɸѡ������Դ�б��в���ĳ����Դ
	int  SearchResInList(const char cszResName[], ResInfo** ppResInfo);

	//�µ�ͼ��ʽ�Ĳ���/////////////////////////////////////////////////////////
	//int  AnalyseOneResourceInfoInNewMap(const char cszMapName[]);
	//int  AnalyseEntityInfoInNewMap(IEKG3DSceneOutDoorSpaceMgr* pIEOutDoorMgr);
	//////////////////////////////////////////////////////////////////////////

	//����������Դ�ļ�
	int  FindResInModelset(const char cszResourceName[]);
	int  FindResInPVS(const char cszResourceName[]);
	int  FindResInSTree(const char cszResourceName[]);
	int  FindResInMDL(const char cszResourceName[]);
	int  FindResInSFX(const char cszResourceName[]);

private:
	typedef map<string, ResInfo*> ResInfoMap;			//string:��Դ����	ResInfo*����Դ��ϸ��Ϣʵ��ָ��
	typedef map<string, ResInfoMap> SourceResMap;		//string:��Դ����	ResInfoMap����Դ��ϸ��Ϣ����
	typedef map<UINT, vector<D3DXVECTOR3>> ResPosMap;	//UINT:��Դ��EntityInfoHandle��vector<D3DXVECTOR3>���꼯��

	ResPosMap m_ResPosList;					//��Դ�����б�
	SourceResMap m_MapResList;				//��ͼ��������������Դ�б�
	set<string> m_ResTypeList;				//������Դ�����б�
	vector<string> m_vecSearchResNameList;	//��������Դ�����б�
	ResInfo* m_pSearchResult;				//�������

	string m_strCurrentMapName;				//��ǰ���������ĵ�ͼ��������
	int m_nCurrentSelection;				//��ǰѡ�е�ListView��¼
	int m_nCurrentID;						//��ǰ������������ԴEntityInfoHandle
	int m_nListViewLine;					//ListView����ʾ����
	bool m_bNeedFindRes;					//����Ƿ���Ҫ����ɸѡ��Դ
	char m_szClientPath[MAX_PATH];			//�ͻ���Ŀ¼
	char m_szSearchResName[MAX_PATH];		//��������Դ����
	char m_szLastSearchType[MAX_PATH];		//��������Դ����

	IKG3DEngineManager* m_pEngineMgr;
	HINSTANCE m_hInstance;
	HWND m_hDlg;
	//�ؼ����
	HWND m_hRadioBtnMultiSearch;
	HWND m_hRadioBtnSingleSearch;
	HWND m_hStaticMultiResText;
	HWND m_hEditMultiResText;
	HWND m_hStaticResName;
	HWND m_hStaticResType;
	HWND m_hStaticResult;
	HWND m_hEditResName;
	HWND m_hEditResType;
	HWND m_hListViewResult;
};

#endif