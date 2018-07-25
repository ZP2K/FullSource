// SceneSkinEdit.h: interface for the KSceneSceneEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENESKINEDIT_H__16CCC5FF_63EA_4D45_ADD6_C238124856B5__INCLUDED_)
#define AFX_SCENESKINEDIT_H__16CCC5FF_63EA_4D45_ADD6_C238124856B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scene.h"
#include "SceneEditBase.h"
#include "KSceneSceneEditorPanelDialog.h"
#include "KSceneSceneEditorDialogLargeTerrian.h"

#include "KSceneSettingDialog.h"
#include "meshskybox.h"

#include "KSO3World.h"
#include <map>

class KSceneSceneEditorDialogGroundTextureMgr;
class KSceneSceneEditorDialogObjectMgr;
class KModelSceneSFX;

class KSceneSceneEdit : public KSceneEditBase  
{
public:

#pragma pack(push,1)
	struct _TGAHeader 
	{
		unsigned char   Id ;
		unsigned char   ColourMapType ;
		unsigned char   ImageType ;
		unsigned short  ColourMapOrigin ;
		unsigned short  ColourMapLength ;
		unsigned char   ColourMapDepth ;
		unsigned short  x_origin ;
		unsigned short  y_origin ;
		unsigned short  width ;
		unsigned short  height ;
		unsigned char   bitsperpixel ;
		unsigned char   imagedescriptor ;
	};
#pragma pack(pop)

	KScene* m_pScene;	//��Ӧ����Ϸ�߼��еĳ�����ָ��

	BOOL        m_bEnablePlaceArray;
	BOOL        m_bEnableRandomRotation;
	BOOL        m_bEnableRandomScale;
	float       m_fRotationDN;
	float       m_fRotationUP;
	float       m_fScaleDN;
	float       m_fScaleUP;
	float       m_fPlaceArrayDest;


	LPTEXTURE   m_lpTextureSky;

	KSceneSettingDialog*                 m_lpDialogSceneSetting;//�������öԻ���
	KSceneSceneEditorPanelDialog*        m_lpDialogPanel;       //�����������
	KSceneSceneEditorDialogLargeTerrian* m_lpDialogLargeTerrian;//��ߴ���α༭�Ի���
	KSceneSceneEditorDialogGroundTextureMgr* m_lpDialogGroundTextureMgr;//�ر���ͼ�������Ի���
	KSceneSceneEditorDialogObjectMgr*        m_lpDialogObjectMgr;


	int  m_nDestMapWidth;//���ڵ�����ͼ��С
	int  m_nDestMapHeigth;
	BOOL m_bNeedReSizeMap;

	struct _SceneHead
	{
		DWORD  FileMask;
		DWORD  BlockLength;

		DWORD  MapsBlock;
		DWORD  ObjectBlock;
		DWORD  Extend[256];

		_SceneHead()
		{
			FileMask = 0xFF00FF00;
			BlockLength = 0;
			MapsBlock = 0;
			ObjectBlock = 0;
			ZeroMemory(Extend,sizeof(DWORD)*256);
		}
	};

	struct _SceneOperatorPlaceObject : public _SceneOperatorBase
	{
		HRESULT Undo(KSceneEditBase* pScene);
		HRESULT AddPlacedObject(LPOBJECT pObject);

		_SceneOperatorPlaceObject();
		virtual ~_SceneOperatorPlaceObject();
	};
	ModelSFX::KModelSFXSkyBox* m_lpSkyBox;

	KModelSceneSFX *m_lpSSFX;

	KModelTerrain* m_lpMapMesh;
	//KModelTerrainLOD*  m_lpTerrainLOD;

	LPMODEL m_lpLightFlare;

	//ModelTools::KModelToolTerrain*        m_lpMainTool;
	//ModelTools::KModelToolObjectTable* m_lpObjectTool;

	//////////////////////////////////////////////////////////////////////////
	//�������
	KComponentObject*   m_lpInHandObject;
	vector<string>      m_vecInHandObjectName;
	int                 m_nCurInHandObjectIndex;
	//////////////////////////////////////////////////////////////////////////
	//����ģʽ
	KRepGirlNo1*        m_lpGirlNo1;
	
	
	//////////////////////////////////////////////////////////////////////////
	//ˮ�棬��Ӱ
	//Water::KModelWater*        m_lpWater;

	//////////////////////////////////////////////////////////////////////////
	//�ӵ����
	class MissileMgr
	{
	public:
		typedef struct _MissileInfo
		{
			DWORD    dwRepMissileID;
			KMissile* lpLogicMissile; 
		}*LPMISSILEINFO,MISSILEINFO;

		list<MISSILEINFO>m_listMissileInfo;

	public:
		MissileMgr();
		virtual ~MissileMgr();
	};

	//////////////////////////////////////////////////////////////////////////
	//�������
	class ObjectGroupLeaf
	{
	public:
		DWORD   m_dwObjectIndex;
		TCHAR   m_szName[256];
		vector<LPOBJECT>m_vecObject;
	public:
		ObjectGroupLeaf();
		virtual ~ObjectGroupLeaf();

		HRESULT Split2SubGroupByMeshRef();//�������ʹ��ģ�͵�����з�
	};

	class ObjectGroupTree
	{
	public:
		TCHAR   m_szName[256];
		list<ObjectGroupTree*>m_listSubGroupTree;
		list<ObjectGroupLeaf*>m_listGroupLeaf;
		ObjectGroupTree*  m_lpParentTree;
	public:
		ObjectGroupTree();
		virtual ~ObjectGroupTree();
	};

	ObjectGroupTree m_EntityGroupTree;
	//ObjectGroupLeaf m_GroupSingleMesh;
	vector<ObjectGroupLeaf*>m_vecEntityGroupLeaf;
	vector<ObjectGroupLeaf*>m_vecEntityGroupLeafSelected;
	//////////////////////////////////////////////////////////////////////////
	//�������Ϣ
	D3DXVECTOR3 m_CameraPosition;
	D3DXVECTOR3 m_CameraLookAt;
	D3DXVECTOR3 m_CameraAngle;
	float       m_CameraDistance;

	list<D3DXVECTOR3>m_listMoveDestPos;

	BOOL   m_bLOD;
	
	DWORD  m_dwTestTimeStart;
	BOOL   m_bTesting;
	int    m_nFrameCount;
	D3DXVECTOR3 m_Center;

	vector<KSceneEntityGroup>m_vecEntityGroup;

	HWND    m_hWndSmallMap;
	HWND    m_hWndObjectPreView;

	BOOL    m_bNeedRefreshSmallMap;
	DWORD   m_dwSmallMapOption;
	float   m_fSmallMapScaling;
	D3DXVECTOR3   m_SmallMapCenter;
	BOOL    m_bOptimizeSmallEntity;

	float   m_fSunHeight;

	DWORD   m_dwWaterReflectionMapID;//ˮ�淴����ͼ���к�

	//DWORD   m_dGUIID;
	DWORD   m_dwShowObscale;
public:
	HRESULT PT_Scene2Logical(int* pX,int* pY,int* pZ,D3DXVECTOR3 Pos);
	HRESULT PT_Logical2Scene(D3DXVECTOR3* pPos,int X,int Y,int Z);

	HRESULT  GetEntityGroupIndex(DWORD* pIndex,KSceneEntity Entity);//��ȡ�����Ӧ�����������.
	HRESULT  GroupObjects();//�������
	HRESULT  GroupObjectsSelected();

	HRESULT  RestoreDeviceObjects(void);
	HRESULT  InvalidateDeviceObjects(void);

	HRESULT WaterMapCreate();
	HRESULT WaterMapRelease();
	HRESULT RenderWaterReflection();//����ˮ������Ҫ�ķ�����ͼ

	HRESULT ToggleEditing();//�л��༭/����ģʽ
	HRESULT ShowDialogSceneSetting(BOOL bShow);//��ʾ���߹رճ������öԻ���
	HRESULT ShowDialogLargeTerrian(BOOL bShow);//��ʾ���߹رմ�ߴ���α༭��
	HRESULT ShowDialogGroundTextureMgr(BOOL bShow);//��ʾ���߹رյر���ͼ������
	HRESULT ShowDialogObjectMgr(BOOL bShow);//��ʾ���߹ر����������

	HRESULT OutputSmallMap(LPSTR pFileName);
	HRESULT MoveCamera2SmallMapPos(float X,float Y);

	BOOL    IsAbleToUnGroup();
	HRESULT RenderSmallMap();
	HRESULT RenderObjectPreview();
	HRESULT Render4LargeTerrianDialog(HWND hOutputWnd);

	HRESULT Edit_GroupSelectedEntity();
	HRESULT Edit_UnGroupSelectedEntity();

	HRESULT OnNewScence();
	HRESULT SetInHandObjectFileDirectory(LPCSTR pDir,LPCSTR pCurFileName);//���÷�������滻��Ŀ¼
	HRESULT ScrollInHandObject(BOOL m_bUp);

	HRESULT CheckUpMapSize(int& Size);

	void PutEntitytoRegion();
	void PutEntitytoTerrianRegion(KSceneEntity& Entity,BOOL bOptimize);
	
	HRESULT FrameMove();
	HRESULT ComputeCamera();

	HRESULT Edit_PlaceObject();
	HRESULT Edit_Undo();
	HRESULT Edit_Copy();
	HRESULT Edit_Paste();
	HRESULT Edit_Cut();
	HRESULT Edit_Select();

	HRESULT Edit_DropSelectedObject();
	
	HRESULT LoadObjects(FILE *pFile);
	HRESULT SaveObjects(FILE* pFile);

	void ONKEYDOWN    (WPARAM wParam, LPARAM lParam);
	void ONLBUTTONDOWN(WPARAM wParam, LPARAM lParam);
	void ONLBUTTONUP  (WPARAM wParam, LPARAM lParam);
	void ONRBUTTONDOWN(WPARAM wParam, LPARAM lParam);
	void ONMOUSEWHEEL (WPARAM wParam, LPARAM lParam);

	HRESULT Initialize(PVOID pSceneTable);
	HRESULT Render();
	DWORD  m_CurObjectID;
	DWORD  m_CurXMeshID;//��ǰ������XMESH�����ڵ�ǰOBJECT�е�λ��
	POINT m_LastMousePos;
	HRESULT ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	KSceneSceneEdit();
	virtual ~KSceneSceneEdit();

private:
	DWORD	m_dwLogicalMapID;

	BOOL SetEditorData2LogicalData(void);

	void ONMOUSEMOVE(WPARAM wParam, LPARAM lParam);
public:

	HRESULT RenderScene();

	HRESULT Acquire();
	HRESULT Edit_EditMesh(void);
	HRESULT SaveScene(LPSTR pFileName);
	HRESULT LoadScene(LPSTR pFileName);

	virtual HRESULT  GetHeight(D3DXVECTOR3* pOut,D3DXVECTOR2* pIN,DWORD Type);

protected:
	
public:
	HRESULT OnShowSelectedEntityProperties(void);
};

#endif // !defined(AFX_SCENESKINEDIT_H__16CCC5FF_63EA_4D45_ADD6_C238124856B5__INCLUDED_)
