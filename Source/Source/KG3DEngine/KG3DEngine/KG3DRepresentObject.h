////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DRepresentObject.h
//  Version     : 1.0
//  Author      : ZhaoXinyu (Chiyer)
//  CopyRight   : Kingsoft Corporation All rights Reserved
//  History     : 2005-9-27 14:46:19
//  Comment     : Object Class
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DREPRESENTOBJECT_H_
#define _INCLUDE_KG3DREPRESENTOBJECT_H_

#include "../../../Include/kg3dengine/kg3dinterface.h"
#include "KG3DTransformation.h"
#include "KG3DModel.h"
#include "KG3DRepresetObjectPropertyModifior.h"
#include "KG3DRepresentObjectTemplate.h"
#include "IEKG3DRepresentObject.h"
#include "KG3DToolsImpl.h"
#include "KG3DFrustum.h"
#include "KG3DMemoryFile.h"


class KG3DSceneSpaceNode;
struct KG3DIntersectionFilter;
class KG3DCullData;
class KG3DScenePointLightRender;


#define OBJLOAD_NONE       0
#define OBJLOAD_MULTITHEAD 1

enum
{
	TOBF_LUMP_HEADER	=	0x46424F54,		// 'MOBF', map object flag
	TOBF_LUMP_SECT		=	0x46434F4C,		// 'MOBL', map object list
};

/*

TObfHeader_T
TObfSect_T * dwMapObjs;
TObfRegion_T * dwMapObjRegions;

*/

#pragma pack(2)
struct TObfHeader_T
{
	DWORD  dwMask;
	DWORD  dwVersion;

	DWORD  dwMapObjs;			// map object nums, �ܼ� 64k ������Ӧ�ù��˰� (���� ��")
	DWORD  dwMapObjRegions;

};

struct TObfSect_T
{
	UINT   nOffset;
	UINT   nSize;
};

struct TObfRegion_T
{
	SHORT  nIndexX, nIndexZ;			// ��ʾ�ҽӵ��ĸ� Region
	USHORT nMapObjs;					// region map object nums
	USHORT nMapObjRefs[127];			// region map object ref nums, �ܼ� 127 Ӧ�ù��˰� (���� ��")

};

#pragma pack()

class KG3DScene;
class KG3DBsp;

class KG3DSceneEntityContainer;
class KG3DRepresentObjectPVS;
class KG3DSceneRegion;
class KG3DSceneSceneEditor;

class KG3DRepresentObject;

struct KG3DRepresentObjectVisitor 
{
	virtual HRESULT Accept(KG3DRepresentObject& Obj) = 0;
	virtual ~KG3DRepresentObjectVisitor() = 0{}
};

class KG3DRenderSorterStratege;
class KG3DRepresentObject : 
	public CKG3DConnectionPoint<IEKG3DRepresentObject>,
	public KG3DTransformation ,
	public IEKG3DSimpleSink
{
	friend class KG3DSceneObjectEditor;
	friend class KG3DSceneSceneEditor;

private:
	//KG3DScene* m_lpScene;//<SetScene Refactor>//������ϣ������Ա�ǲ���Ҫ��
	//list<KG3DSceneRegion*>m_listSceneRegion;//���Լ�������Region,���ڼ�¼���������Ļ���
	set<KG3DSceneEntityContainer*>m_setRefrenceContainer;//��ҪPublic

	float m_fDistance;//����������ľ���

	DWORD m_dwType;   //����Ǻܻ����ĳ�Ա�����ڱ�־���ͣ�Ҫprivate��������ĵĻ���һЩǿת�������ˡ�
	DWORD m_dwID;
	KG3DOBJProperty* m_pProperty;

	DWORD m_dwVisibleCount;	//���Ӽ���	��GetSetVisibleCount����
	KG3DRepresentObject* m_pParentObj;	//��GetSetParentObj����

	ULONG   m_ulRefCount;      //���ü���ǧ���public�ĺ�
public:
    virtual size_t GetSubModels(vector<KG3DModel*>& vModels);

	struct ModelInformation//ģ����չ��Ϣ,��Ҫ�������ڱ���ϲ�������ģ���Ƿ���Դ��MODELSET,��֤�ֽ�ʱ�̿��Ի�ԭΪMODELSET
	{
		int  nTypeSource;//��Դ���� 0,Ĭ�� 1��modelset
		std::string szSource;//��Դ�ļ���
		int  nIndex;//
		ModelInformation()
		{
			nTypeSource = 0;
			szSource = "";
			nIndex = 0;
		}
	};
	enum OBJECT_STATE
	{
		REPRESENTOBJECTSTATE_NONE,
		REPRESENTOBJECTSTATE_RESOURSENOTREADY,
		REPRESENTOBJECTSTATE_ZOOMIN,
		REPRESENTOBJECTSTATE_ZOOMOUT,
	};
protected:

    // use for editor (for group obj)
    D3DXVECTOR3     m_vGroupTrans;
    D3DXVECTOR3     m_vGroupScal;
    D3DXQUATERNION  m_qGroupRota;
    int             m_bGroupPoseValidate;

	vector<ModelInformation*>m_vecModelInformation;	//���ںϲ���
	vector<KG3DModel*>m_vecModel;
	DWORD m_dwSceneLayerID;//�����в�����

	const D3DXMATRIX*  m_pMatParent;
	D3DXMATRIX  m_matWorld;	//��GetWorldMatrix�õ�

	float m_fAlpha;

	float m_fAlphaWhenStateChanged;//m_fAlphaChangeTime����״̬�ı�ʱ���Alpha

	OBJECT_STATE m_eState;//���ڶ��̵߳��뵭��	//��GetState�õ�
public:

    KG3DRepresentObjectPVS* m_pPvsOn;
	float m_fVisibleAngle;//��������µĽǶȣ���λ�ǻ���
	std::string  m_scName;
	AABBOX  m_BBox;
    DWORD m_dwLastCullCounter;
    BOOL    m_bBBoxChanged;
	//BOOL m_bSelected;
public:
	//���ü���
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		REFIID riid, void **ppvObject
		);

	virtual HRESULT Init();
	virtual HRESULT UnInit();

	//KG3DRepresentObjectBuilder������Load�����ڲ�����ˣ�������ģ�ͻ��Ǳ�ģ�Ҳ������ini���Ǳ�Ķ���
	virtual HRESULT Load(LPCTSTR lpFileName, DWORD_PTR wParam, DWORD_PTR lParam);//������ڼ��ظ�����ģ�ͻ��߱�Ķ���

	//Load��LoadFromFile�ǲ�һ���ģ�LoadFromFile�Ǳ��汾����Template���ã�Load�Ǽ������е�
	virtual HRESULT LoadFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption);

	//���ڼ�¼���������Ļ��ƣ���
	HRESULT OneInputContainer(KG3DSceneEntityContainer* pContainer);
	HRESULT OnRemoveFromContainer(KG3DSceneEntityContainer* pContainer);
	HRESULT RemoveFromContainer();


    HRESULT OnEnterPvs(KG3DRepresentObjectPVS* pvs);
    HRESULT LeavePvs();

    virtual HRESULT ResetGroupPose();
    virtual void GetBspModel(vector<KG3DModel*>* pvModel);


	//���ڼ�¼���������Ļ��ƣ���
	//HRESULT AddSceneRegion(KG3DSceneRegion* pRegion);
	//HRESULT RemoveFromRegions();
	//HRESULT ClearSceneRegionList();

	virtual void SetPhysicsScene(KG3DPhysiscScene *pScene);
	//HRESULT SetScene(KG3DScene* pScene);//<SetScene Refactor>

	// ��ȡ��̬�ϰ�����������, INVALID_OBSTACLE_GROUP��ʾ�Ƕ�̬�ϰ�
	int GetDynamicObstacleGroupIndex();
	virtual HRESULT SetDynamicObstacleGroupIndex(int nindex);

	FLOAT	GetDistance()				{return m_fDistance;}
	VOID	SetDistance(FLOAT fDistance){m_fDistance = fDistance;}

	const D3DXMATRIX& GetMatrixWorld()				{return m_matWorld;}
	const D3DXVECTOR3& GetWorldPos();
	VOID	SetMatrixWorld(const D3DXMATRIX& mat)	{m_matWorld = mat;}

	FLOAT	GetAlpha()					{return m_fAlpha;}
	OBJECT_STATE	GetState()			{return m_eState;}

	virtual BOOL IsLoaded();
    const D3DXMATRIX* GetParentMatrix() { return m_pMatParent; }

	virtual HRESULT OnLostDevice(){return S_OK;};
	virtual HRESULT OnResetDevice(){return S_OK;};

	// use for npc
	virtual HRESULT SetModelFace(LPCTSTR pMeshFile,LPCTSTR pMtlFile,LPCTSTR pAniFile,LPCTSTR pSocketName) {return E_NOTIMPL;};
	virtual HRESULT PlayAnimation(LPCSTR szAniPath) {return E_NOTIMPL;};

	virtual HRESULT ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);//<PROCESS_MESSAGE>

	virtual HRESULT ChangeModel(INT nChangeCode){return E_FAIL;}	//��OnCommand�ĵط���������

	DWORD GetModelCount();

	KG3DModel* GetModel(DWORD dwIndex);
	KG3DModel* GetModel(DWORD dwIndex,INT nLODLevel);
	virtual HRESULT GetModel(KG3DModel*&  pModel, DWORD dwIndex,BOOL bLod);
	virtual HRESULT GetModel(IKG3DModel*& pModel, DWORD dwIndex);
	virtual HRESULT SetModel(DWORD dwIndex,
		const char pcszMeshFileName[],
		const char pcszMtlFileName[],
		const char pcszAniFileName[],BOOL bForceLoad);

	virtual HRESULT BindModel(LPCSTR strBindTo, 
		KG3DModel* pModel);

	HRESULT GetModelFileName(int nIndex,
		LPSTR pMeshFile,LPSTR pMtlFile,LPSTR pAniName);

	virtual HRESULT GetName(LPTSTR pName, size_t BufferSize);
	virtual const AABBOX& GetBBox();

	virtual HRESULT Reload();

	virtual void UpdateTransformation();
	int  IsSockWave();

	virtual void SinkFunc(INT nSinkID);
	virtual void SinkFunc2(INT nSinkID,INT nsubSinkID,DWORD dwValue,PVOID pPoint);

	// virtual DWORD GetObjType()   { return REPRESENT_TYPE_NORMAL; }//��GetType���жϾͿ�����
	virtual BOOL  IsVisible(const vector<KG3DFrustum>& vFrustum);

	KG3DRepresentObject*	GetParentObject()						{return m_pParentObj;}
	VOID					SetParentObject(KG3DRepresentObject* p)	{m_pParentObj = p;}

	DWORD GetVisibleCount(){return m_dwVisibleCount;};
	void  SetVisibleCount(DWORD dwCount){m_dwVisibleCount = dwCount;};

    virtual void UpdateVisiableSubset(KG3DCullData* pCull) { };

	virtual HRESULT RefreshOption(unsigned uOption);
	virtual HRESULT SaveToFile(const char cszFileName[],   unsigned uOption);
	virtual HRESULT LoadFromFileEx(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, DWORD_PTR WParam, DWORD_PTR LParam)
	{
		return LoadFromFile(cszFileName, uFileNameHash, uOption);
	}
	virtual DWORD GetID();
	void	SetID(DWORD dwID)		{ m_dwID = dwID;}
	virtual unsigned GetType();
	void	SetType(DWORD dwType)	{m_dwType = dwType;}

    virtual KG3DSceneSpaceNode* Conver2SpaceNode() { return NULL; }

	virtual HRESULT PrepareRender(unsigned dwOption);  
	virtual HRESULT PostRender();                          
	virtual HRESULT GetDateInformation(        
		int* pnMem,int* pnVedioMem,            
		int* pnConMem,int* pnConVMem           
		);                                                    
	virtual unsigned   GetLastUseTime();       
	virtual unsigned   GetUseFrequency();      

	virtual HRESULT Copy(KG3DRepresentObject* pSrcObject);//�����ֻ࣬��Ҫ��������ͺ��ˣ�����ֻ��Ҫ�������ݣ������һ��Object����ObjectTable�����������ڵ�

	virtual HRESULT GetAnotherIEInterface(DWORD dwType, VOID** ppAnotherInterface){return E_FAIL;}
public:

    virtual HRESULT OnTransformationChange(KG3DScene* pScene);
    virtual void OnRemoveFromEntityMgr(KG3DCullData* pCull) {  }
	virtual HRESULT OnInformationChange(KG3DScene* pScene);
	//<�¼�>
	virtual HRESULT OnDeleteFromScene(KG3DSceneSceneEditor* pEditor){return E_NOTIMPL;}
	//virtual HRESULT OnAddToScene(KG3DSceneSceneEditor* pEditor){return E_NOTIMPL;}//û�м�鵽�κε���
	virtual HRESULT OnSelect(){return S_OK;}
	virtual HRESULT OnUnSelect() {return S_OK;}
	//</�¼�>

    virtual BOOL HasSfx();

	virtual void SetParentMatrix(const D3DXMATRIX* pMat);

	virtual BOOL    IsResourceReady();
	HRESULT ChangeState(OBJECT_STATE eState);

	ModelInformation* GetModelInformation(int nIndex);
	HRESULT SetModelInformation(int nIndex,ModelInformation* pInfo);

	virtual HRESULT RenderReflect(float fWaterHeight,D3DXPLANE& WaterPlane,D3DXPLANE& WaterPlaneFX);

	virtual void GetProfile(DWORD& dwNumFacesTotal,DWORD& dwNumVertices,DWORD& dwNumTextures,DWORD& dwNumID);

	virtual HRESULT RenderPlaneShadow(D3DXPLANE& plane,const D3DXVECTOR4& LightDir);
	virtual HRESULT RenderShadowMap(LPD3DXEFFECT &lpEffect,const D3DXMATRIX &matVP,const D3DXMATRIX &matWVP);
	virtual HRESULT RenderSelfShadow(LPD3DXEFFECT &lpEffect,const D3DXMATRIX &matVP,
		const D3DXMATRIX &matWVP,LPDIRECT3DTEXTURE9 lpDepthMap);
	virtual HRESULT ComputeBBox();
	virtual HRESULT ComputeDynamicBBox();
    virtual HRESULT RenderHierarchy(BOOL bCull = TRUE, KG3DScenePointLightRender* pRender = NULL);
	virtual HRESULT FrameMove();
	virtual HRESULT Render(DWORD dwOption);
	virtual HRESULT RenderSelectMark(DWORD dwColor = 0xFFFF0000);

	//<��ײ���>
	virtual BOOL IsRayIntersect(D3DXVECTOR3&Inter,D3DXVECTOR3 Origin,D3DXVECTOR3 Direction);
	// bFiltrate �Ƿ�������������õĲ�����־ģ��
	virtual BOOL GetRayIntersect(DWORD& dwCount,D3DXVECTOR3* pInterPoint,DWORD dwMaxNum,D3DXVECTOR3 Origin,D3DXVECTOR3 Direction, BOOL bFiltrate = FALSE);//�õ����е���ײ��
	virtual BOOL IsVertexInside(D3DXVECTOR3& vVertex, D3DXVECTOR3& vDir);
	BOOL GetCameraNearRayIntersect(
		IN CONST D3DXVECTOR3 vSrc[],
		IN CONST D3DXVECTOR3 vDst[],
		IN CONST INT         nNum, 
		OUT      FLOAT*      pDis 
		);//�õ���Des����Ľ��㣬���ڲ���Mesh��SSE�Ĵ��롣ע����ʹ�ã���Ҫ�Ǹ�������õ�
	virtual BOOL RayIntersection(const D3DXVECTOR3& vSrc
		, const D3DXVECTOR3& vNormalizedDir
		, FLOAT* pfRet
		, BOOL bIntersectBoxOnly
		, KG3DIntersectionFilter* pFilter = NULL);//ֻ��Ҫ������ײ������㣬�������ԣ���vSrc + vNormalizedDir * fRet�Ϳ��Եõ���������
	//GetNearRayIntersect�Ĺ�����RayIntersection����
	/*virtual BOOL GetNearRayIntersect(const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vNormalizedDir
		, FLOAT* pfRet);*/
	//BOOL GetRayIntersectPointAndDir(DWORD& dwCount,
	//	D3DXVECTOR3* pInterPoint,
	//	float* pAngle,
	//	const D3DXVECTOR3& vec3Org,
	//	const D3DXVECTOR3& vec3Dir);//�õ���ײ�����ײ���Angle
	//</��ײ���>

	//<Trans>
	virtual HRESULT SetTranslation(const D3DXVECTOR3* pTrans);
	virtual HRESULT SetRotation(const D3DXQUATERNION* pRotaion);
	virtual HRESULT SetScaling(const D3DXVECTOR3* pScale);
	//</Trans>

	

	KG3DRepresentObject(void);
	virtual ~KG3DRepresentObject(void);

	HRESULT SetLOD(float fLod);

	BOOL IsSecondUVExist();
	
	void SetAlpha(float fAlpha);

	DWORD GetSceneLayerID();
	void  SetSceneLayerID(DWORD dwID);

	HRESULT BindSFXSuedeFromFile(LPCSTR pstrBindFileName, IKG3DScene* pActiveScene = NULL);

	HRESULT SaveToIniFile(LPCTSTR pFileName,DWORD dwOption);
	HRESULT LoadFromIniFile(LPCTSTR pFileName,DWORD dwOption);
	virtual HRESULT GetModelCount(INT* pValue);
	//virtual void SetScaling(D3DXVECTOR3 *pValue);//�����Ѿ���һ��SetScaling��

	/*
	�����ڴ��������ݱ��浽�ڴ��У�Ȼ��ʱ����LoadFromInformationBuffer��������
	dwType�����Լ����ͣ�dwLength����д�Ķ��٣�pFile��дBuffer��

	�ο�Load������ͨ����ΪBuild�Ļ��ƣ�IE�Ǳ߻���һ��IEKG3DRepresentObject_BuildStruct֮��Ľṹ��
	�����Ǹ��ṹ��ͺ��ˣ�LoadFromInformationBuffer��ʱ����Load���������ؾͺ���
	*/
	virtual HRESULT SaveToInformationBuffer(/*DWORD& dwType,DWORD& dwLength,*/KG3DMemoryFile* pFile);
	virtual HRESULT LoadFromInformationBuffer(/*DWORD dwType,DWORD dwLength,*/const KG3DMemoryFile* pFile,DWORD dwOption);

	virtual BOOL	IsSpecial(){return FALSE;}//���⣬ָ������Dummy����û�а�ģ�ͻ���������ģ�͵�������������Բ��ᵥ����һ������ά���ģ���Ϊ�麯����������һ��������������֧�ֵģ�ר�Ž�һ������ά��������������ص�Policyʵ����ֻ���ø�����	
    virtual HRESULT LoadPostProcess();
protected:
	HRESULT OnReferenceZero();
private:
	UINT m_HandleHeldBySpaceMgr;//����KG3DSceneOutDoorSpaceMgr�з����Entity��Handle,�Է����ͷ�
public:
	UINT GetHandleHeldBySpaceMgr(){return m_HandleHeldBySpaceMgr;}
	VOID SetHandleHeldBySpaceMgr(UINT Handle){m_HandleHeldBySpaceMgr = Handle;}

	//KG3DRenderSorterStratege& GetRenderSorterStratege();
//<LOD>
private:
	enum{emCheckLODFrameGap = 10,};
	DWORD		m_dwLODFrameCount;   //���ڿ���ÿ������������һ��CheckLODLevel
	INT			m_nLODLevel;       // ģ��LOD����  
	enum
	{
		EM_LOD_EXIST_NONE   = 0, //û���κε�LODģ������Ҫ��LOD���
		EM_LOD_EXIST_MIDDLE = 1,//0x00000001������ģ��
		EM_LOD_EXIST_LOW = 2 //0x00000010���е�ģ��
	};
	DWORD       m_dwLODExist;		//LOD�ĵ�ģ����ģ�ļ��Ƿ���ڵı�־�����û�У�CheckLODLevelʲô���������� 
protected:
	vector<KG3DModel*> m_vecModelM;
	vector<KG3DModel*> m_vecModelL;
public:
	enum
	{
		EM_LOD_LEVEL_HIGH = 0,	//0����߾��ȣ���Ϊ����ģ�Ͷ��Ǵ���߾�������ģ��߾������ޣ�����ģ�������޵;���
		EM_LOD_LEVEL_MIDDLE = 1,
		EM_LOD_LEVEL_LOW = 2,
	};
	HRESULT		CheckLODLevel();
	INT         GetLODLevel(){return m_nLODLevel;}	
	HRESULT SetLowModel(DWORD dwIndex,const char pcszMeshFileName[],const char scMtlName[],DWORD dwOption);
	HRESULT SetMiddleModel(DWORD dwIndex,const char pcszMeshFileName[],const char scMtlName[],DWORD dwOption);

	HRESULT SetModelTranslation(DWORD dwIndex,D3DXVECTOR3 &vTrans);
	HRESULT SetModelScaling(DWORD dwIndex,D3DXVECTOR3 &vScale);
	HRESULT SetModelRotation(DWORD dwIndex,D3DXQUATERNION &qRot);
	HRESULT UpdateModelTransformation(DWORD dwIndex);
	HRESULT ComputeModelBBox(DWORD dwIndex);

//<LOD>
	HRESULT	SetSelectable(BOOL bSelectable);
	ULONG	IsSelectable();
private:
	enum
	{
		em_state_selectable= 1 << 1,
	};
	DWORD	m_dwState;	//Selected��ӦҪObject�Լ��ܣ�һ��ObjΪʲôҪ֪���Լ���ѡ�еģ����Ҫ֪�����Ƿ��Ǳ�ѡ�еģ���SelectionTool��Selected����Obj������
	int     m_nDynamicObstacleGroupIndex;
};

#endif //_INCLUDE_KG3DREPRESENTOBJECT_H_
