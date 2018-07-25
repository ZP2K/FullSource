#ifndef _INCLUDE_KG3DTERRAINROAD_H_
#define _INCLUDE_KG3DTERRAINROAD_H_

#include "IEEditor.h"
#include "Terrain\KG3DSerpentine.h"

class KG3DTexture;

class KG3DTerrainRoad;

struct KG3D_PointLight_Info;

struct KG3DTerrainRoadPassage : public KG3DPassage 
{
	KG3DTexture* m_lpTexture;//��·��ͼ
	LPDIRECT3DTEXTURE9 GetTexture();
		
	virtual HRESULT UpdateVertex(KG3DSerpentine* pSerpentine);

	HRESULT UpdateRoadHeight(KG3DSceneOutDoorSpaceMgr* pOutDoorSpaceMgr);// ���ر�ĸ߶ȸ���·��
	HRESULT UpdateTerrainHeight(KG3DSceneOutDoorSpaceMgr* pOutDoorSpaceMgr); // ��·��ĸ߶ȸ����ر�

public:
	KG3DTerrainRoadPassage();
	~KG3DTerrainRoadPassage();
	
};

class KG3DTerrainRoad :public KG3DSerpentine
{
public:
	struct _RoadHead
	{
		DWORD        dwNumOfNode;//�ڵ���Ŀ 
		DWORD        dwNodePosBlock;  //
		DWORD        dwNumOfPassage;//
		DWORD        dwPassageBlock;
		TCHAR		 scName[32];//��·��
		TCHAR        scTextureName[MAX_PATH];//��·��ͼ��		
		float        fWidth;//��·���
		float		 fTexDensity;      //��ͼ�ܶ�
		float		 fBlendLength;      //�˵��͸�����
		float	     fEdgeModulus;     //��Ե����ϵ��
		float        fBendModulus;     //Hermite�����߷��̵Ĳ���
		int			 nID;              //����˳���
		float   	 fNodeSize;//��·�ڵ�Ĵ�С
		DWORD		 dwSegmentLength;   //�ֶγ���
		DWORD        dwNumParentBlock;//
		DWORD        dwParentTerrainBlock;
		DWORD        dwReserved[5];
		_RoadHead()
		{
			dwNumOfNode = 0;
			dwNodePosBlock = 0;
			dwNumOfPassage =0;
			dwPassageBlock = 0;
			ZeroMemory(scName,sizeof(TCHAR)*32);
			ZeroMemory(scTextureName,sizeof(TCHAR)*MAX_PATH);
			fWidth = 0;
			fTexDensity = 0;
			fBlendLength = 0;
			fEdgeModulus = 0;
			fBendModulus = 0;
			nID = 0;
			fNodeSize = 0;
			dwSegmentLength=0; 
			dwNumParentBlock= 0;//
			dwParentTerrainBlock =0;
			ZeroMemory(dwReserved,sizeof(DWORD)*5);;
		}
		~_RoadHead()
		{
		}
	};
	
public:
	std::string  m_scTextureName;
	KG3DTexture* m_lpTexture;//��·��ͼ
	float m_fTexDensity;      //��ͼ�ܶ�
	float m_fBlendLength;      //�˵��͸�����
	float m_fEdgeModulus;     //��Ե����ϵ��
	int   m_nID;              //����˳���

	KG3D_PointLight_Info* m_lpPointLightInfo;
public:
	HRESULT SetPointLightInfo(KG3D_PointLight_Info& vInfo);
	HRESULT RefreshHeight(KG3DSceneOutDoorSpaceMgr* pOutDoorSpaceMgr,BOOL bToTerrain);

	HRESULT LoadRoadTexture(LPCTSTR pFileName);

	virtual HRESULT AddOnePassage(KG3DPassage** ppPassage,int nPos);
	virtual HRESULT AddOnePassage(KG3DPassage** ppPassage);
	virtual HRESULT SaveToFile(const char cszFileName[]);
	virtual HRESULT LoadFromFile(const char cszFileName[]);
	virtual HRESULT LoadNode(DWORD dwNumOfNode,D3DXVECTOR3* pNodePos);
	
	KG3DTexture* GetTexture(){return m_lpTexture;}
	//ADD Code by huangjingshou
	BOOL CheckIncurvate(KG3DTerrainRoadPassage::TinyLine& L1,KG3DTerrainRoadPassage::TinyLine& L2);
	
	//float GetNodeSize();
	//HRESULT SetNodeSize(float nSize);
	
	virtual void UnInit();
public:
	KG3DTerrainRoad(void);
	virtual ~KG3DTerrainRoad(void);
};


class KG3DTerrainRoadMgr : public KG3DSerpentineMgr,
						   public IEKG3DTerrainRoadMgr //��·������
{
public:	

	HRESULT SaveToFile(LPCSTR strFileName); //�����·��Ϣ
	//HRESULT LoadRoadFromFile(LPCSTR strFileName); //��ȡ��·��Ϣ
	HRESULT LoadByHandle(UINT uHandle,KG3DTerrainRoad** ppOutRoad);
public:
	KG3DTerrainRoadMgr();
	KG3DTerrainRoadMgr(KG3DSceneOutDoorSpaceMgr* pOutDoorMgr);
	~KG3DTerrainRoadMgr();
	//////////////////////////////////////////////////////////////////////////
	//HRESULT GetVisibleRoad(list<KG3DTerrainRoad*>* plistRoad,const KG3DFrustum* pFrustum);
	HRESULT AddNodeToCurSerpentine(KG3DRepresentObjectNode* pNode,D3DXVECTOR3& vPos);
	//��·

	virtual HRESULT DeleteOne(int nIndex);
	HRESULT NewOne(KG3DSerpentine** ppRoad);


	int  GetID();
	KG3DTerrainRoad* GetRoadByID(int& nID);
	virtual	BOOL FindRoadByID(int& nID);

	virtual HRESULT SetCurrentRoadBlendLength(float fBlendLength);
	virtual HRESULT GetCurrentRoadBlendLength(float& fBlendLength);
	virtual HRESULT SetCurrentRoadTexDensity(float fTexDensity);
	virtual HRESULT GetCurrentRoadTexDensity(float& fTexDensity);
	virtual HRESULT SetCurrentRoadEdgeModulus(float fEdgeModulus);
	virtual HRESULT GetCurrentRoadEdgeModulus(float& fEdgeModulus);

	virtual HRESULT ChangeCurrentRoadTexture(const TCHAR* strName);
	virtual HRESULT GetCurrentRoadID(int& nID);
	virtual HRESULT SetCurrentRoadID(int& nID);

	/////////////////////////////////////////////////////////////
	virtual DWORD GetCount(){return KG3DSerpentineMgr::GetCount();}
	HRESULT AddNewOne(){return KG3DSerpentineMgr::AddNewOne();}
	virtual HRESULT SetCurSerpentine(int nIndex){return KG3DSerpentineMgr::SetCurSerpentine(nIndex);}
	virtual BOOL	FindSerpentineByName(const TCHAR* strName){return KG3DSerpentineMgr::FindSerpentineByName(strName);}
	virtual HRESULT GetAllName(vector<std::string>& vecName){return KG3DSerpentineMgr::GetAllName(vecName);}
	virtual HRESULT RefreshCurSerpentine(){return KG3DSerpentineMgr::RefreshCurSerpentine();}
	virtual HRESULT RefreshHeight(BOOL bToTerrain){return KG3DSerpentineMgr::RefreshHeight(bToTerrain);}
	virtual float GetCurSerWidth(){return KG3DSerpentineMgr::GetCurSerWidth();}
	virtual void SetCurSerWidth(float fWidth){return KG3DSerpentineMgr::SetCurSerWidth(fWidth);}
	virtual void SetCurSerDrawSelectMark(BOOL bVal){return KG3DSerpentineMgr::SetCurSerDrawSelectMark(bVal);}
	virtual BOOL GetCurSerDrawSelectMark(){return KG3DSerpentineMgr::GetCurSerDrawSelectMark();}
	virtual void SetCurSerDrawExtendLine(BOOL bVal){return KG3DSerpentineMgr::SetCurSerDrawExtendLine(bVal);}
	virtual BOOL GetCurSerDrawExtendLine(){return KG3DSerpentineMgr::GetCurSerDrawExtendLine();}
	virtual HRESULT SetCurSerModulusK(float fK){return KG3DSerpentineMgr::SetCurSerModulusK(fK);}
	virtual HRESULT GetCurSerModulusK(float& fK){return KG3DSerpentineMgr::GetCurSerModulusK(fK);}
	virtual HRESULT SetCurSerName(const TCHAR* strName);
	virtual HRESULT GetCurSerName(std::string& strName){return KG3DSerpentineMgr::GetCurSerName(strName);}
	virtual HRESULT SetCurSerSegmentLength(int nLength){return KG3DSerpentineMgr::SetCurSerSegmentLength(nLength);}
	virtual HRESULT GetCurSerSegmentLength(int& nLength){return KG3DSerpentineMgr::GetCurSerSegmentLength(nLength);}
	virtual HRESULT MoveCurSerpentinePre(){return KG3DSerpentineMgr::MoveCurSerpentinePre();}
	virtual HRESULT MoveCurSerpentineNext(){return KG3DSerpentineMgr::MoveCurSerpentineNext();}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

};
#endif