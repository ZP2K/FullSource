#pragma once
#include "..\..\..\include\kg3dengine\kg3dinterface.h"
#include "Scene/KG3DSceneEntity.h"
#include "KG3DCamera.h"
#include "KG3DSceneWndManager.h"
#include "KG3DEnvEffLight.h"
#include "KG3DSceneEntityMgr.h"
#include "IEKG3DScene.h"
//#include "KG3DSceneCameraMovement.h"
#include "KG3DEnvEffTimeInformer.h"
#include "KG3DGPS.h"
#include "KG3DSceneEntity.h"
#include "KG3DCommonObjectNode.h"
#include "KG3DCommonObjectPool.h"

class KG3DSceneSelectionTool;

class KG3DPhysiscScene;

class KG3DEnvironment;
class KG3DSceneOutDoorSpaceMgr;

class KG3DRegionInfoManager;
class KG3DSceneCameraMovement;
class KG3DSceneSpaceNode;
class KG3DSceneSceneEditor;
class KG3DSceneEntity;
class KG3DCommonObjectBuilder;
class KG3DSceneObjectPlacementTool;

struct KG3DMessageBroadcasterGroup;
struct KG3DTransformTool;

class KG3DScene : public IEKG3DScene
{
public:// for IEKG3DScene
	virtual HRESULT RefreshPhysisc();
	virtual HRESULT GetSceneSFXEditor(IEKG3DSceneSFXEditor **ppiRetSFXEditor);
    virtual IEKG3DScenePvsEditor* GetScenePvsEditor() { return NULL; }
    virtual IEKG3DSceneSelectBase* GetSelectBase() { return NULL; }

	// if no more entity, function return S_OK, and set *ppiRetEntity to null
	virtual HRESULT GetFirstRenderEntity(IEKG3DSceneEntity **ppiRetEntity);
	virtual HRESULT GetNextRenderEntity(IEKG3DSceneEntity **ppiRetEntity);
	virtual HRESULT FindRenderEntity(
		SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, IEKG3DSceneEntity **ppiRetEntity
		);

	//virtual HRESULT GetLightCount(DWORD *pdwRetLightCount);

	//virtual D3DCOLOR* GetAmbient();
	D3DCOLOR	GetAmbient();
	virtual HRESULT GetAmbient(DWORD *pdwRetAmbient);
	virtual HRESULT SetAmbient(DWORD dwAmbient);

	//(by dengzhihui Refactor 2006��11��23��
	virtual HRESULT GetSceneModelEditor(IEKG3DSceneModelEditor** ppModelEditor);
	virtual HRESULT GetSceneSceneEditor(IEKG3DSceneSceneEditor** ppSceneEditor);
	virtual HRESULT GetSceneObjectEditor(IEKG3DSceneObjectEditor** ppObjectEditor);
	virtual HRESULT GetSceneDataFlowEditor(IEKG3DSceneDataFlowEditor** ppDataFlowEditor);
	virtual KG3DSceneSceneEditor* GetSceneSceneEditor();

    virtual BOOL InUnderWater() const { return FALSE; }

	//)
	virtual HRESULT TraverseEntityMeshForGenerateLogicObstacle(
		float fWorldStartX, float fWorldStartY, int nRegionPosX, int nRegionPosY,
		KTraverseEntityMeshForGenerateLogicObstacleCallback* pCallback    
		);
	
	virtual HRESULT Traverse(KG3DRepresentObjectVisitor& visitor);

	virtual HRESULT ClearAllHelpLine();
	virtual HRESULT SetHelpLineSize(size_t nLine, size_t nSize);
	virtual HRESULT SetHelpLineStart(size_t nLine, size_t nStart);
	virtual HRESULT SetHelpLine(size_t nLine, int nSetID, size_t nBegin, size_t nCount, D3DCOLOR color, D3DXVECTOR3* pPos);
	virtual HRESULT GetHelpLine(size_t nLine, size_t* pPoint, D3DXVECTOR3** ppPos);

	virtual LPDIRECT3DCUBETEXTURE9 GetEnvMap(LPTSTR pNameBuffer, size_t uBufferCount);
	virtual HRESULT SetEnvMap(LPCTSTR szPath){return E_FAIL;}
	//virtual std::string GetEvnMapName() { return m_szEvnMapName; }
    
    virtual HRESULT ShockWaveRender() { return E_NOTIMPL; }

	virtual HRESULT RefreshOption(unsigned uOption);
	virtual HRESULT OnLostDevice();
	virtual HRESULT OnResetDevice();

    virtual DWORD GetDataInterfaceVersion();

	virtual ULONG GetEditState();

    virtual int GetModelOverlapBox(KG3DModel* pModels[], int maxNum, D3DXVECTOR3 box[8]);

	virtual HRESULT GetSelectionToolInterface(IEKG3DSceneSelectionTool** ppTool);
	virtual HRESULT GetPickRayPos(D3DXVECTOR3* pOutPos);

public:
    virtual HRESULT LoadCameraAnimation(LPSTR pszCameraAniFile);
    virtual HRESULT StartPlayCamAni();
    virtual HRESULT StopPlayCamAni();
    virtual HRESULT PausePlayCamAni();

private:
	HRESULT TraverseModelsForGenerateLogicObstacle(vector<KG3DModel*>& vModels,int nDynamicObstacleGroupIndex,
				int nRegionPosX, int nRegionPosY, D3DXVECTOR2& vRegionLeftBottom,D3DXVECTOR2& vRegionRightTop,
				BBox& RegionBox,KTraverseEntityMeshForGenerateLogicObstacleCallback* pCallback);
private:
	KG3DSceneEntityList::iterator m_itRenderEntity;
public:
	KG3DPhysiscScene* m_lpPhysicsScene;
    KG3DSceneOutputWnd* m_pCurRenderWnd;

	//LPDIRECT3DCUBETEXTURE9 m_lpEvnMap;//<EnvMap Mov>
	//std::string            m_szEvnMapName;

	UINT m_uOption;

	int m_nLogicalSceneX_Start;//����Ϊ��λ����Ϸ�߼������ڳ����е�λ��
	int m_nLogicalSceneZ_Start;
	int m_nLogicalSceneX_Width;
	int m_nLogicalSceneZ_Width;
	
	int m_nLogicalRegionX_Start;
	int m_nLogicalRegionZ_Start;
	int m_nCountLogicalRegionX;
	int m_nCountLogicalRegionZ;
public:
	KG3DSceneEntityMgr m_EntityMgr;
	KG3DSceneOutDoorSpaceMgr* m_lpSceneOutDoorSpaceMgr;

    KG3DScenePointLightRender* GetPointLightRender() { return m_EntityMgr.m_lpPointLightRender; }

    /* in editor, edit node will point to the locked node
       in client, edit node ever point to the root node  
       */
    KG3DSceneSpaceNode* m_pEditSpaceNode;

    KG3DSceneSpaceNode* GetEditSpaceNode() { return m_pEditSpaceNode; }

	KG3DSceneOutDoorSpaceMgr* GetOutDoorSpaceMgr(){return m_lpSceneOutDoorSpaceMgr;}
	//////////////////////////////////////////////////////////////////////////
	//�����ռ�ָ�ڵ�����
	HRESULT ProcessSpaceNodeByPosition(UINT& uCurrentNode,UINT nStartNode,D3DXVECTOR3& vPosition);
	HRESULT ProcessSpaceNodeByCamera(KG3DCamera* pCamera);
    HRESULT CalcFrustum(KG3DCamera* pCamera);
	UINT m_uCurrentSpaceNodeHandle;
	map<UINT,KG3DSceneSpaceNode*>m_mapSceneSpaceNodeHandle;
	KG3DSceneSpaceNode* GetSpaceNodeByHandle(UINT hHandle);
	HRESULT RegisterSpaceNode(KG3DSceneSpaceNode* pNode);
	HRESULT UnRegisterSpaceNode(KG3DSceneSpaceNode* pNode);
	UINT GetOneNewSpaceNodeHandle();
	KG3DSceneSpaceNode* m_lpCurrentSpaceNode;//��ǰ�����Ŀռ�ڵ�

	//KG3DWaterEffect* m_lpWaterEffect; //this object is very aspect-specific, and it is more proper to move it to KG3DSceneSceneEditor

public:
	KG3DScene(void);
	virtual ~KG3DScene(void);

public:
	DWORD m_dwType;
	DWORD m_dwID;
	DWORD m_dwRecordID;

	KG3DSceneEntityList m_listRenderEntity;

    PostRenderParams m_EffectParams;

	//DWORD m_dwAmbient;

public:
	ULONG   m_ulRefCount;
	std::string m_scName;

public:
	virtual HRESULT AutoReleadModifiedResource();
	HRESULT SetPhysiscScene(KG3DPhysiscScene* pPhyScene)
	{
		m_lpPhysicsScene = pPhyScene;
		return S_OK;
	}
	KG3DPhysiscScene* GetPhysiscScene()
	{
		return m_lpPhysicsScene;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef( void);
	virtual ULONG STDMETHODCALLTYPE Release(void);
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		REFIID riid, void **ppvObject
		);

	virtual HRESULT Init();
	virtual HRESULT UnInit();

	virtual HRESULT LoadFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption);
	virtual HRESULT SaveToFile(const char cszFileName[],   unsigned uOption);

	virtual HRESULT OnSetFocus();	//// Last modify by Chen Tianhong: 2007-7-23 11:20:03///����Ҫ��GraphicsTool��SetCurScene

	virtual DWORD GetID();
	virtual unsigned GetType();
	void SetID(DWORD dwID) { m_dwID = dwID;}
	void SetType(DWORD dwType){m_dwType = dwType;}

	virtual HRESULT PrepareRender(unsigned uOption);
	virtual HRESULT PostRender();
	virtual HRESULT GetDateInformation(int* pnMem,int* pnVedioMem,int* pnConMem,int* pnConVMem);//�õ�������Ϣ����������Ϊռ���ڴ��С���ִ��С��ѹ�����ڴ��С��ѹ�����ڴ��С
	virtual unsigned   GetLastUseTime(); 
	virtual unsigned   GetUseFrequency();

	virtual HRESULT FrameMove();
	virtual HRESULT Render(unsigned uOption);		//��Ⱦ
	virtual HRESULT RenderToScreen(D3DXVECTOR2 const* pA, D3DXVECTOR2 const* pC, D3DXVECTOR2 const* pUVA, D3DXVECTOR2 const* pUVC, float fZ, DWORD dwFlags);

	virtual HRESULT AddOutputWindow(LPCTSTR szName, HWND hWnd, DWORD dwType, DWORD_PTR dwOption, int* pOutputWindowID);
	virtual HRESULT RemoveOutputWindow(int nOutputWindowID);
	virtual HRESULT EnablePostRenderEffect(int nOutputWindowID, BOOL bEnable);
	virtual HRESULT EnablePostRenderEffect(int nOutputWindowID, PostRenderEffectType nType, BOOL bEnable);
	virtual HRESULT AddPostRenderEntity(IKG3DResourceBase *pEntity, unsigned uClass, BOOL bSelectable = TRUE);
	
	//<Add Remove>//Add Removeһ�������֣�Model(IKG3DResourceBase*),Entity,Object
public:	
	virtual HRESULT AddRenderEntity(IKG3DResourceBase *pEntity,    unsigned uClass, BOOL bSelectable,BOOL bLight);
	virtual HRESULT RemoveRenderEntity(IKG3DResourceBase *pEntity, unsigned uClass, BOOL bSelectable = TRUE);
	virtual HRESULT RemoveSelectableModel(IKG3DResourceBase *pEntity);

	//STDMETHOD(AddRepresentObject)(IEKG3DRepresentObject* pObject, BOOL bSelectable, BOOL bSeleted){return E_NOTIMPL;}	
	STDMETHOD(AddRepresentObject)(IEKG3DRepresentObject* pObject, ULONG uOption, IEKG3DRepresentObject** ppCreatedObject){return E_NOTIMPL;}
	STDMETHOD(RemoveRepresentObject)(IEKG3DRepresentObject* pObject){return E_NOTIMPL;}	

	virtual HRESULT AddRenderEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer);
	virtual HRESULT RemoveRenderEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer);


	//���ڲ�ʹ����Щ����
	virtual HRESULT AddRenderEntity(KG3DSceneEntity Entity, BOOL bIsSelectable = TRUE, BOOL bIsSelected = FALSE);
	virtual HRESULT DelRenderEntity(KG3DSceneEntity Entity, BOOL bEitherRemoveInSelectedList = TRUE);//ռס������֣�ʡ�ĺ�DelXX�����岻һ��
	virtual HRESULT RemoveRenderEntity(KG3DSceneEntity Entity, BOOL bEitherRemoveInSelectedList = TRUE);//�Ƿ���SeletedList��Ҳɾ��Entity

	virtual ULONG	GetObjectCount(DWORD dwTypeFilter);//dwTypeFilter��REPRESENTOBJECT_NONE��ʱ�򣬼����������������ֻ�����Ӧ���͵����
	//</Add Remove>
	

	virtual HRESULT ProcessMessage( UINT message, WPARAM wParam, LPARAM lParam);//<PROCESS_MESSAGE>

	virtual IKG3DCamera* GetCurrentCamera(); 
	virtual HRESULT      SetCurrentCamera(IKG3DCamera* pCamera); 

	virtual HRESULT LoadScene(LPSTR pFileName,DWORD Option);

	virtual HRESULT GetFloor(/*D3DXVECTOR3* pVecOut, const D3DXVECTOR3* pVecIn*/const D3DXVECTOR3* pvInput, FLOAT* pfRet);//����3D���꣬���·�������ߣ���˵㳡���ĸ߶�
	virtual DWORD   GetGroundType(const D3DXVECTOR3 &vec3Pos);//���صر�����

	virtual HRESULT GetWaterHeight(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn);
	virtual HRESULT GetHeight(D3DXVECTOR3* pOut,D3DXVECTOR3* pIn);
	virtual HRESULT GetForce (D3DXVECTOR4* pOut,D3DXVECTOR3* pIn);
	virtual float   GetHeight(float x, float z) {return 0;}


	virtual HRESULT LogicalToScene(D3DXVECTOR3* pPos, float fX, float fY, float fZ, BOOL bOnFloor);
	virtual HRESULT SceneToLogical(int* pX, int* pY, int* pZ, D3DXVECTOR3 Pos);
	virtual HRESULT ScenePosToScreenPos(D3DXVECTOR3 const& vScenePos, float& fScreenX, float& fScreenY);

	HRESULT RenderToTexture(DWORD dwTextureID, D3DXVECTOR2 const* pA, D3DXVECTOR2 const* pC, D3DXVECTOR2 const* pUVA,
		D3DXVECTOR2 const* pUVC, float fZ, DWORD dwFlags)
	{
		return S_OK;
	}

	virtual HRESULT OnResizeWindow(PRECT const pRect);

	//virtual HRESULT GetStandHeight(DWORD& dwCount,D3DXVECTOR3* pInter,D3DXVECTOR3& vIn,DWORD dwMaxCount);

	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT GetMinimapLayer(IN const D3DVECTOR &vPos, OUT int &nLayer);
	virtual HRESULT SetFocus(D3DXVECTOR3& vecPos);
	virtual HRESULT GetFocus(D3DXVECTOR3* pvecPos);

	//protected:
public:
	virtual HRESULT OnReferenceZero();

	virtual HRESULT RenderForEachWindow(KG3DSceneOutputWnd& wndCur);

	//virtual HRESULT GetSelectModelListForClient(const D3DXVECTOR2 &fScale,std::vector<IKG3DModel*> &pList){return E_NOTIMPL;}
	//virtual HRESULT GetSelectModelList(const D3DXVECTOR2 &fScale,std::vector<IKG3DModel*> &pList){return E_NOTIMPL;}
	virtual HRESULT GetSelectModelList(const D3DXVECTOR2& vPosScreen, std::vector<IKG3DModel*>& pList, BOOL bGetNearestOrAll){return E_NOTIMPL;}
	//virtual HRESULT GetSelectModelList(const D3DXVECTOR2 &fScaleLeftUp,const D3DXVECTOR2 &fScale1RightBottom,
	//	std::vector<IKG3DModel*> &pList){return E_NOTIMPL;}
	virtual HRESULT GetSelectPosition(D3DXVECTOR2 const& vScale, D3DXVECTOR3& vPosition);

    virtual HRESULT GetTerrainCross(D3DXVECTOR3* vInter, const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vDst) { return E_NOTIMPL; }

protected:
	KG_CUSTOM_HELPERS::TCommonParamBuffer<KG3DSceneCameraMovement, 36> m_pCameraMovement;	//���������
	KG3DRegionInfoManager*	m_lpRegionDWInfoManager;	//������Ӧ�ĵ�����Ϣ
	
	KG3DGPS_Dummy	m_GameGPS; //Ϊ�˵õ������λ�ö�����
private:
	//����������CommonBuffer�������Ͳ�����new�ˣ�Ҫ��֤��㶨���ڣ�ȫ������Light��Fog��Ҫ��㶨����
	KG_CUSTOM_HELPERS::TCommonParamBuffer<KG3DEnvironment, 1026> m_lpEnvironment;	//���ﲻҪ�Ӵ��ø�ָ�룬Ȼ��new����������һ����	
public:
	KG3DEnvironment& GetEnvironment();
public:
	virtual HRESULT RenderWindowBegin(IEKG3DSceneOutputWnd* pWnd,BOOL bSetViewPort);
	virtual HRESULT RenderWindowEnd(IEKG3DSceneOutputWnd* pWnd);
	virtual HRESULT GetCameraMovement(IEKG3DSceneCameraMovement** ppiCameraMovement);
	virtual HRESULT GetCameraMovement(IKG3DSceneCameraMovement** ppiCameraMovement);
	virtual HRESULT GetEnvironment(IEKG3DEnvironment** ppiEnvironment);

    virtual HANDLE  AddSceneSigns(const D3DXVECTOR3& vPos) { return 0; }
    virtual void    DelSceneSings(HANDLE Handle)           {}

	virtual HRESULT LoadFromFileEx(const char cszFileName[], unsigned uFileNameHash, unsigned uOption, DWORD_PTR WOption, DWORD_PTR LOption);

	KG3DRegionInfoManager* GetRegionDWInfoManager(){return m_lpRegionDWInfoManager;}
	KG3DGPSBase*	GetCurGameGPS(){return &m_GameGPS;}
	virtual HRESULT GetEngineIEInterface(DWORD dwType,  DWORD_PTR wParam, DWORD_PTR lParam, VOID** ppiInterface);
	virtual HRESULT GetEngineIEAbstructInterface(DWORD dwAbstructType, DWORD dwType,  DWORD_PTR wParam, DWORD_PTR lParam, VOID** ppiInterface);

	//</CTH>
protected:
	KG3DSceneWndManager* m_SceneWndManager;
public:
	//<���ڹ���>
	virtual HRESULT GetOutputWindow(int nID, IEKG3DSceneOutputWnd **ppiRetSceneOutputWnd);
	virtual HRESULT SetCurrentOutputWindow(int nID)
	{
		return m_SceneWndManager->SetCurrentOutputWindow(nID);
	}
	HRESULT GetPrimaryWindow(IEKG3DSceneOutputWnd **ppiRetPrimaryWindow)
	{
		return m_SceneWndManager->GetPrimaryWindow(ppiRetPrimaryWindow);
	}
	virtual HRESULT GetCurOutputWnd(IEKG3DSceneOutputWnd **ppiRetCurOutputWnd)
	{
		return m_SceneWndManager->GetCurOutputWnd(ppiRetCurOutputWnd);
	}
    virtual HRESULT GetCurRenderWnd(IEKG3DSceneOutputWnd **ppiRetCurOutputWnd);

	virtual HRESULT GetSceneEditorBase(IEKG3DSceneEditorBase** pBase);//by dengzhihui Refactor 2006��12��11��

	KG3DSceneOutputWnd* GetPrimaryWindow();	//�õ����������
	//</���ڹ���>
	virtual float GetLoadingProcess()
	{
		return 1.0F;
	}
	virtual HRESULT SetSceneSettingData(DWORD dwEnum, DWORD_PTR dwData);
	virtual HRESULT GetSceneSettingData(DWORD dwEnum, DWORD_PTR* pdwData);

	virtual HRESULT GetLogicalSceneSize(int* pXStart,int* pZStart,int* pWidthX,int* pWidthZ);
	virtual HRESULT SetLogicalSceneSize(int  nXStart,int  nZStart,int  nWidthX,int  nWidthZ);

	virtual HRESULT GetName(LPTSTR pNameBuffer, size_t uBufferSize);
	
	virtual HRESULT GetAppointLogicalRegion(int& nRegionXStart,int& nRegionZStart,int& nCountRegionX,int& nCountRegionZ);
	virtual HRESULT SetAppointLogicalRegion(int nRegionXStart,int nRegionZStart,int nCountRegionX,int nCountRegionZ);
//<Container>
public:
	static 	KG3DCommonObjectBuilder& GetCommonObjectBuilder();//IOC ע�����
	KG3DCommonObjectNode&	GetCommonObjectNode();
	
private:
	class KG3DCommonObjectNodeScene : public KG3DCommonObjectNodeSimple 
	{
	public:
		KG3DCommonObjectNodeScene(KG3DCommonObjectBuilder& Builder);//���洴�������ٱ�
		STDMETHOD_(ULONG, GetType)(){return KG3DTYPE_COMMON_OBJECT_NODE;}
		STDMETHOD_(KG3DCommonObjectBuilder*, GetCommonObjectBuilder)();
		STDMETHOD_(KG3DCommonObjectPool&, GetCommonObjectPool)(){return m_Pool;}
	private:
		KG3DCommonObjectBuilder& m_Builder;
		KG3DCommonObjectPool	m_Pool;
	};

	KG3DCommonObjectNodeScene	m_CommonObjNode;

	D3DXVECTOR3 m_vec3FocusPos;
//</Container>

	//<Member>
public:
	virtual KG3DTransformTool& GetTransformTool(DWORD dwType);
	KG3DSceneObjectPlacementTool*	GetObjectPlacementTool();	
	virtual KG3DSceneSelectionTool&	GetSelectionTool();
	
	//</Member>

	//<������Ԫ��>
private:
	//������Щ��ע����֮��Ϳ����ڶ�Ӧ���¼�������ʱ��õ���Ϣ����ôSSE����Ͳ������¼�������ʱ������������Щ�࣬�Ӷ��Ͳ���֪����Щ���ʵ�֣�����������
	KG3DMessageBroadcasterGroup* m_pBroadcaster;
public:
	virtual KG3DMessageBroadcasterGroup&	GetBroadcasterGroup();
	virtual KG3DMessageSender&				GetMessageSender(KM_MESSEGE emType);
	//�¼�����
	virtual HRESULT RegisterListener(DWORD ListenerType, IKG3DMessageReceiverBase* pReceiver);
	virtual HRESULT UnRegisterListener(DWORD ListenerType, IKG3DMessageReceiverBase* pReceiver);

	void BoardCastAction( INT nActionID, INT nExecState, WPARAM wParam, LPARAM lParam );

	// Inherited via IEKG3DScene
	virtual HRESULT RenderToSingleOutputWnd(IEKG3DSceneOutputWnd * piSceneOutputWnd) override;
	virtual HRESULT ScenePosToScreenXYZ(D3DXVECTOR3 const & vScenePos, float & fScreenX, float & fScreenY, float & fZ) override;
	virtual HRESULT GetSceneParam(int * pLogicStartX, int * pLogicStartZ, float & fCellLength, float & fLogicalCellCmLength, float & fTerrainMinHeight, float & fPointPerAltitude, float & fAltitudeUnit) override;
	virtual HRESULT GetSelectModelList(const D3DXVECTOR2 & vPosScreen, BOOL bGetNearestOrAll, unsigned uMaxCount, IKG3DModel * pRetModelList[], unsigned * puRetRealCount) override;
	virtual HRESULT GetSelectModelList(const D3DXVECTOR3 & vRayOrig, const D3DXVECTOR3 & vRayDir, BOOL bGetNearestOrAll, unsigned uMaxCount, IKG3DModel * pRetModelList[], unsigned * puRetRealCount) override;
	virtual HRESULT GetSelectPosition(D3DXVECTOR2 const & vScale, D3DXVECTOR3 & vPosition, BOOL bNotIntersectTerrain = FALSE, BOOL bNotIntersectTObject = FALSE) override;
	virtual IKG3DResourceBase * AddFullScreenEntity(LPCSTR strName, LPCSTR strAni, BOOL bLoop, BOOL bClearRT, DWORD dwClearColor, BOOL bAutoFit) override;
	virtual HRESULT RemoveFullScreenEntity(IKG3DResourceBase * pEntity) override;
	virtual BOOL RayIntersection(const D3DXVECTOR3 & vSrc, const D3DXVECTOR3 & vNormalizedDir, FLOAT * pfRet, FLOAT fTestRange, BOOL bCheckTerrain) override;
	virtual HRESULT AddOutputWindow(int nOutPutWndID) override;
	virtual HRESULT SetActorOnFoliage(D3DXVECTOR3 & vActorPos, BOOL bIsMainActor) override;
	virtual HRESULT SetBatchProjectionCenter(const D3DXVECTOR3 & vPos) override;
	virtual HRESULT SetParabolaPosition(D3DXVECTOR3 * pVecPos, int nNodeNum) override;
	virtual HRESULT SetParabolaTexture(LPCSTR pTextureName) override;
	virtual HRESULT SetParabolaRenderEnable(BOOL bRenderEnale) override;
	virtual HRESULT EnablePostRenderEffect(PostRenderEffectType nType, BOOL bEnable) override;
	virtual HRESULT PreLoadResource(LPCTSTR cszFileName, BOOL bSync) override;
	virtual HRESULT UnPreLoadResource(LPCTSTR cszFileName) override;
	virtual BOOL IsSREntityCreated(DWORD dwSREntityID) override;
	virtual BOOL BindEntityToSREntity(DWORD dwSREntityID, IKG3DResourceBase * pEntity) override;
	virtual HRESULT LoadSceneTestHeight() override;
	virtual HRESULT SetTrackBlurParam(D3DXVECTOR2 vec2TBCenter, float fTBSampleStrength, float fTBSampleDist) override;
	//</������Ԫ��>
//protected:
//	list<KG3DModel*> m_listKeepModel;  // ���ڼ�¼��Щ����Ч��Ч�ϼ�������������������Ч�Լ��������Ч�͡�
//public:
//	HRESULT PushModel(KG3DModel* pModel);
//	HRESULT EraseModel(KG3DModel* pModel);
//	HRESULT ClearListKeepModel();
};

