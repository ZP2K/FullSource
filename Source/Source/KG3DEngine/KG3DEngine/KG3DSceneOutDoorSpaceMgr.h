#pragma once
#include "KG3DSceneEntity.h"
#include "KG3DSceneDataInterface.h"
#include "KG3DSceneBlock.h"
#include "IEKG3DTerrain.h"
#include "IEEditor.h"
#include "Terrain/KG3DSceneSpaceNode.h"

class KG3DSceneEntityContainer;
class KG3DSceneZone;
class KG3DSceneRegion;
class KG3DSceneSection;
class KG3DTerrainBlock;
class KG3DTerrainDetailMaterialMgr;
class KG3DHeightMap;
class KG3DFrustum;
class KG3DEntityObjectShell;
class KG3DSceneBlockLoader;
class KG3DTerrainGrassSet;
class KG3DRepresentObjectSoundBall;
class KG3DFrustum;
class KG3DTerrainRiver;
class KG3DTerrainRoad;
struct KG3DTerrainRoadPart;
class KG3DTerrainRoadMgr;
class KG3DTerrainConverMap;
class KG3DTerrainRiverMgr;
class KG3DSceneShadowMap;
class KG3DSceneSceneEditor;
class KG3DSceneEntityMgr;
typedef enum _KM_MESSEGE KM_MESSEGE;

////////////////////////////////////////////////////////////////////////////////


class KG3DScene;

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ����ṹ���Ǿɵ��εģ�ֻ����Ϊ�浽�ļ������ˣ�Ϊ�˼����ϸ�ʽ�ļ��ݲ�ɾ��
#define GROUND_TEXTURE_LAYERS 4
struct KG3DTerrainRegionTextureInfo_Old
{
    SHORT   TextureCount;
    SHORT   TextureIndex[GROUND_TEXTURE_LAYERS];

    KG3DTerrainRegionTextureInfo_Old()
    {
        TextureCount = 1;
        TextureIndex[0] = 1;
        TextureIndex[1] = TextureIndex[2] = TextureIndex[3] = -1;
    }
};




////////////////////////////////////////////////////////////////////////////////
typedef map<UINT,KG3DEntityObjectShell*> TypeShellMap;

struct MultiThreadShellMap
{
private:
	TypeShellMap m_map;

public:
	KMutex m_vlock;

	HRESULT UnInit();
	BOOL IsExist(UINT uHandle);
	HRESULT OutPutEntityShell(KG3DEntityObjectShell* pShellArray[],DWORD& dwCount,DWORD dwMaxCount);
	HRESULT GetEntityShell(KG3DEntityObjectShell* pShellArray[],DWORD& dwCount,DWORD dwMaxCount);
	HRESULT InputShell(UINT uHandle,KG3DEntityObjectShell* pShell);

	KG3DEntityObjectShell* Find(UINT uHandle);
	HRESULT STDMETHODCALLTYPE Release(TKG3DResourcePool<KG3DEntityObjectShell>* pObjectShellPool,UINT uHandle);
	size_t GetSize(){return m_map.size();};

	HRESULT CheckMemoryUse_EntityShell(BOOL* pbStop,int nDestCount,
		set<KG3DRepresentObjectSoundBall*>*psetObjectSoundBall,
		vector<KG3DRepresentObject*>*pvecObjectToRelease);

};

class KG3DMessageSender;
class KG3DSceneOutDoorSpaceMgr : public KG3DSceneSpaceNode , public IEKG3DSceneOutDoorSpaceMgr, 
	public KG3DSceneDataEventConnection//����ռ������
{
	friend class KG3DSceneBlockLoader;
	friend class KG3DRepresentTerrainBlock;
	friend class KG3DTerrainBlockModifier;
    friend class KG3DSceneEntityContainerData;

	KG3DSceneRegion m_vErrorRegion;//�ݴ��Region����GetErrorRegion�õ�

	ULONG m_ulRefCount;
public:
	HRESULT InputNewZone(KG3DSceneZone* pZone);
	HRESULT DoDynamicLoadOneEntity(UINT uEntityHandle,DWORD dwOption);
	//HRESULT DoDynamicLoadOneEntity(UINT uEntityHandle,KG3DSceneEntityContainer* pcontainer,DWORD dwOption);
	HRESULT DoDynamicLoadEntityContainer(KG3DSceneEntityContainer* pCon,BOOL bLoadAll,DWORD dwOption);

    bool CheckNeedLoad(UINT uHandle);

	HRESULT CheckMemoryUse(KG3DSceneSceneEditor* pEditor);//��鲢ж�س��ڲ��õ�BLOCK
protected:
    KG3DScene& m_Scene;//�������޺ð����úܶ�ָ���ж�
	int m_nTotalLoadStep;
	int m_nCurrentLoadStep;

	BOOL m_bImportOldScene;//�Ƿ��ڵ���ɸ�ʽ
	int m_nZoneStart_X;
	int m_nZoneStart_Z;
	int m_nNumZone_X;
	int m_nNumZone_Z;

	// add by zhangzhixiong(2009.11.17)
	BOOL m_bCreateTerrainLess128x128;
	int m_nXRegionNum;
	int m_nYRegionNum;
	//end
	KG3DSceneDataInterface* m_lpDataInterface;
	KG3DSceneBlockLoader* m_lpSceneBlockLoader;

	set<KG3DSceneZone*>m_setZone;
	KG3DSceneZone** m_lpZones;
	list<POINT>m_listDeletedZone;//ɾ����ZONE

	MultiThreadShellMap m_mapObjectShell;
	//MultiThreadShellMap m_mapObjectShellMultiThreadLoaded;

	list<KG3DEntityObjectShell*>m_listObjectShellNoHandle;//û��HANDLE��OBJECT����

	std::set<KG3DRepresentObjectSoundBall*>m_setObjectSoundBall;//��ǰλ������������Ч��

	//KG3DOutDoorAssistantFuncs m_Funcs;//����Щ�ܳ��ĺ�����װ�����������

	//////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 m_vCameraFront;
	D3DXVECTOR3 m_vCameraLeft;
	D3DXVECTOR3 m_vCameraPosition;
	D3DXMATRIX  m_vCameraView;

	HRESULT InputZoneToArray(KG3DSceneZone* pZone);
	HRESULT CleanUp();

	HRESULT InputObjectShell(KG3DEntityObjectShell* pShell, BOOL bCheck2Pvs);
	HRESULT ClearSoundBallSet();
	KG3DMessageSender& GetSceneMessageSender(const KM_MESSEGE& dwType);
public:

	HRESULT DoDynamicUnloadEntityShells();

    KG3DEntityObjectShell* GetObjShellByHandle(UINT uHandle);

	HRESULT FrameMove(DWORD dwLoadLimit);

	HRESULT CheckMemoryUseMultiThread_TerrainBlock(KG3DSceneSceneEditor* pEditor);

	HRESULT CheckMemoryUse_EntityShell(BOOL* pbStop,int nDestCount);

    HRESULT OnLostDevice();
    HRESULT OnResetDevice();

    HRESULT StopMultiThreadLoad();

	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

    virtual BOOL IsPositionInSpace(D3DXVECTOR3& vPos) { return TRUE; }
    virtual DWORD GetDataInterfaceVersion();
    virtual HRESULT SetEntityParent(UINT uHandle, UINT uParent);
    virtual UINT GetEntityParent(UINT uHandle);

    KG3DScene& GetScene() { return m_Scene; }
   // void InputObjectShell(KG3DSceneEntityContainer* pContainer, KG3DEntityObjectShell* objShell);

	HRESULT GetSceneRect(int& nX_Start,int& nZ_Start,int& nX_Width,int& nZ_Width);//��λ����
	HRESULT GetSceneRect(D3DXVECTOR2* pLowerLeft, D3DXVECTOR2* pUpperRight);	//��λ��cm

	HRESULT ProcessSoundBall(KG3DCamera* pCamera);
	HRESULT ProcessSoundBallMultiThread();

	HRESULT FindSoundBall(D3DXVECTOR2 A,D3DXVECTOR2 C);

    virtual HRESULT CalcFrustum(const TypeFrustums& vInputFrustum, DWORD dwLookMode);	//�̳���KG3DSceneSpaceNode
    virtual DWORD GetSceneNodeType() { return SceneNodeType_OutDoor; }

    HRESULT CheckInput2Pvs(KG3DModel* pModel);
    HRESULT CheckRemove2Pvs(KG3DModel* pModel);

	HRESULT RemoveModel(KG3DModel* pModel);
	HRESULT InputModel(KG3DModel* pModel);

	HRESULT ProcessDynamicLoad(BOOL bForceSetRect,D3DXVECTOR2 vPosition,D3DXVECTOR2 vDir,float fRZone,float fRSection,float fRRegion,
		KG3DSceneSceneEditor* pEditor);

	virtual HRESULT LoadFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption);
	virtual HRESULT SaveToFile(const char cszFileName[], unsigned uOption);

    virtual LPCTSTR GetNodeName()                            { return TEXT("Out Door Mgr"); }


	/*
	���bToDataInterface����ô�����ڲ����´�����һ��Object��������ppCreatedObject���õ�����´�����Object��
	ppCreatedObject����ΪNULL�����ص�Object�������ü�����
	*/
	HRESULT InputRepresentObject(KG3DRepresentObject* pObject, int bForceLoad ,BOOL bToDataInterface, KG3DRepresentObject** ppCreatedObject);
	HRESULT DelRepresentObject(UINT Handle);
	HRESULT RemoveRepresentObject(UINT Handle){return DelRepresentObject(Handle);}//�����������ռ�ţ�ʡ�ĺ���������������ֺ�DelXX���÷���һ��
	HRESULT RemoveRepresentObject(KG3DRepresentObject* pObject);
	//////////////////////////////////////////////////////////////////////////

	KG3DTerrainDetailMaterialMgr* m_lpDetailMtls;
 
	KG3DTerrainGrassSet*          m_lpSetPointer[3];
	UINT						  m_uSetHandle[3]; //�ֱ�����GrassSet,StoneSet,FrutexSet��Handle

	virtual HRESULT FindPattern(LPCSTR strFileName, INT *pnRetIndex,DWORD dwType);
	virtual HRESULT SetGrassSetTexture(LPCSTR strFileName,DWORD dwType);
	virtual HRESULT SetCurGrassSet(DWORD dwType);
	virtual std::string GetGrassSetTextureName(DWORD dwType);;

	HRESULT GetTerrainRect(D3DXVECTOR2& vA,D3DXVECTOR2& vB);

	float GetGroundHeight(float fX,float fZ);//����ڵ�ǰ�����ڵĵ�������û�������ʱ��ȡ�����Ľ���Ǵ���ġ�Ч�ʸߣ�һ������²��������⡣
	DWORD GetGroundType(const D3DXVECTOR2 &vec2Pos);
	float GetGroundHeightEx(float fX,float fZ);//�������ȡ�߶�ǰ��鵱ǰȡ�߶ȵĵ��μ������û�����û���أ���ѵ�ǰ��TerrainBlock���ؽ�����huangjinshou-2008-11-7
	BYTE GetDetailTextureIndex(float fX,float fZ); 
	HRESULT GetWaterHeight(float& fHeight,float x,float z);
	ULONG	GetObjectCount(DWORD dwTypeFilter);//dwType=REPRESENTOBJECT_NONE��ʱ��ֱ��ͳ�ƣ�Ч�ʽϸߣ�����͹��˶�Ӧ����

	HRESULT SelectZone(vector<POINT>*pvecZoneIndex,D3DXVECTOR2 A,D3DXVECTOR2 C,BOOL bUnDeleted);
	HRESULT SelectZone(vector<KG3DSceneZone*>*pvecZone,D3DXVECTOR2 A,D3DXVECTOR2 C);

	HRESULT SelectSection(vector<POINT>*pvecSectionIndex,D3DXVECTOR2 A,D3DXVECTOR2 C,BOOL bUnDeleted);
	HRESULT SelectSection(vector<KG3DSceneSection*>*pvecSection,D3DXVECTOR2 A,D3DXVECTOR2 C);

	HRESULT SelectRegion(vector<POINT>*pvecRegionIndex,D3DXVECTOR2 A,D3DXVECTOR2 C,BOOL bUnDeleted);
	HRESULT SelectRegion(vector<KG3DSceneRegion*>*pvecRegion,D3DXVECTOR2 A,D3DXVECTOR2 C);
	
	KG3DTerrainBlock* GetTerrainBlock(int nLevel,int nIndexX,int nIndexZ);
	/*HRESULT SetZoneConverTexture(LPSTR pFileName,D3DXVECTOR2 A,D3DXVECTOR2 C);
	HRESULT SetSectionConverTexture(LPSTR pFileName,D3DXVECTOR2 A,D3DXVECTOR2 C);
	HRESULT SetRegionConverTexture(LPSTR pFileName,D3DXVECTOR2 A,D3DXVECTOR2 C);*/
	HRESULT SetTerrainConverTexture(LPSTR pFileName,D3DXVECTOR2 A,D3DXVECTOR2 C,BOOL bZone,BOOL bSection,BOOL bRegion);

	HRESULT SetZoneHeight(KG3DHeightMap* pHeight,D3DXVECTOR2 A,D3DXVECTOR2 C);
	HRESULT SetSectionHeight(KG3DHeightMap* pHeight,D3DXVECTOR2 A,D3DXVECTOR2 C);
	HRESULT SetRegionHeight(KG3DHeightMap* pHeight,D3DXVECTOR2 A,D3DXVECTOR2 C);
	HRESULT SetZoneWaterHeight(KG3DHeightMap* pHeight,D3DXVECTOR2 A,D3DXVECTOR2 C);
	HRESULT SetSectionWaterHeight(KG3DHeightMap* pHeight,D3DXVECTOR2 A,D3DXVECTOR2 C);
	HRESULT SetRegionWaterHeight(KG3DHeightMap* pHeight,D3DXVECTOR2 A,D3DXVECTOR2 C);


	HRESULT SetZoneHeight(LPSTR pFileName,float fLowest,float fTotal,D3DXVECTOR2 A,D3DXVECTOR2 C);
	HRESULT SetZoneWaterHeight(LPSTR pFileName,float fLowest,float fTotal,D3DXVECTOR2 A,D3DXVECTOR2 C);

	HRESULT SetSectionHeight(LPSTR pFileName,float fLowest,float fTotal,D3DXVECTOR2 A,D3DXVECTOR2 C);
	HRESULT SetSectionWaterHeight(LPSTR pFileName,float fLowest,float fTotal,D3DXVECTOR2 A,D3DXVECTOR2 C);

	HRESULT SetRegionHeight(LPSTR pFileName,float fLowest,float fTotal,D3DXVECTOR2 A,D3DXVECTOR2 C);

	KG3DSceneRegion* GetRegion(int nX,int nZ);
	HRESULT NewOneRegion(KG3DSceneRegion** pNewRegion,int nRegion,int nRegionZ);
	HRESULT DeleteRegion(KG3DSceneRegion* pRegion);
	HRESULT GetRegionNormal(D3DXVECTOR3& vec3Normal, float fX, float fZ);
	//////////////////////////////////////////////////////////////////////////
	KG3DSceneSection* GetSection(int nX,int nZ);
	HRESULT NewOneSection(KG3DSceneSection** ppNewSection,int nSectionX,int nSectionZ);
	HRESULT DeleteSection(KG3DSceneSection* pSection);

	HRESULT CreateTerrainBlock(int nLevel,int nX,int nZ);
	HRESULT DeleteTerrainBlock(int nLevel,int nX,int nZ);

	HRESULT CreateSubDetailForZone(int nX,int nZ,int nXSectionNum,int nZSectionNum,DWORD dwOption);
	HRESULT CreateSubDetailForZone(int nX,int nZ,DWORD dwOption);
	HRESULT CreateSubDetailForSection(int nX,int nZ,DWORD dwOption);


	BOOL CheckTerrainBlockExist(int nLevel,POINT& Index);
	//////////////////////////////////////////////////////////////////////////
	HRESULT RenderAllZoneBox();

	KG3DSceneZone* GetZone(int nX,int nZ);

	HRESULT GetVisibleSceneZone(
		vector<KG3DSceneEntityContainer *> *pvecContainer,
		KG3DTerrainRender *pTerrainRender,
		map<int,KG3DTerrainRoad *> *pmapRoad,
		set<KG3DTerrainRiver *> *psetRiver,
		const KG3DFrustum *pFrustum, 
        const D3DXVECTOR3 &vPosCamera,
		int nLevelDeep);	//�ü�����ȣ�0-ZONE,1-SECTION,2-REGION

	HRESULT NewOneZone(KG3DSceneZone** ppZone,int nX,int nZ);
	HRESULT DeleteZone(KG3DSceneZone* pZone);

	HRESULT InitOutDoor(int nXStart,int nZStart,int nXCount,int nZCount,LPCTSTR pFileName);

	HRESULT Resize(int nXStart,int nZStart,int nXCount,int nZCount);

	KG3DSceneOutDoorSpaceMgr(KG3DScene& Scene);
	virtual ~KG3DSceneOutDoorSpaceMgr(void);


	HRESULT CreateDataInterface(LPCTSTR pFileName,BOOL bNewScene);
	HRESULT DeleteDataInterface();

	HRESULT LoadOneObjectShell(KG3DEntityObjectShell* pShell,DWORD dwOption);
	//HRESULT UnLoadOneObjectShell(KG3DEntityObjectShell* pShell);
	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT OnEntityCreate(UINT uHandle, int bForceLoad);
	virtual HRESULT OnEntityDelete(UINT uHandle);

    virtual HRESULT OnEntityTransformModified(UINT uHandle, BOOL bCheckInput2Pvs);
	virtual HRESULT OnEntityInformationModified(UINT uHandle);
	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT OnSceneBlockCreate(int nLevel,POINT Index);
	virtual HRESULT OnSceneBlockDelete(int nLevel,POINT Index);
	virtual HRESULT OnTerrainBlockCreate(int nLevel,POINT Index);
	virtual HRESULT OnTerrainBlockDelete(int nLevel,POINT Index);
	virtual HRESULT OnTerrainBlockModified(int nLevel,POINT index,DWORD dwDataType);

	virtual HRESULT OnTerrainInformationModified(UINT uInfoHandle,DWORD dwDataType);


    virtual HRESULT LoadDefaultTerrain(KG3DTERRAIN_LOAD_PARAM* dwExtraParam);

	//<��ײ���>
	//BOOL RayIntersection(D3DXVECTOR3 vSrc, D3DXVECTOR3 vNormalizedDir, D3DXVECTOR3& vRet);
	HRESULT GetObjectShellIntersectPoint2D(list<KG3DEntityObjectShell*>*plistShell,D3DXVECTOR2& vPos);
	virtual void GetObjectShellIntersectLine2D(set<KG3DEntityObjectShell*>* plistShell
								, const D3DXVECTOR2& A, const D3DXVECTOR2& C);
	//</��ײ���>


	//<����>	//����һ��VISITOR����KG3DEntityCtVisitorForObj�����Ϳ���ֱ�ӱ���Object�������Ǳ���Container�ˡ���KG3DEntityCtVisitorForObjVisitor�Ļ�����ֱ��ת���ⲿ��ObjectVisitor
	HRESULT Traverse(KG3DEntityCtVisitor& visitor, D3DXVECTOR2 vSrc, D3DXVECTOR2 vDes);//�����߶�������,ֻ����2D,�������,ֻ������vDesΪֹ
	HRESULT Traverse(KG3DEntityCtVisitor& visitor, const KG3DFrustum& Frustum);	//�����׶�帲�ǵ�Container
	HRESULT Traverse(KG3DEntityCtVisitor& visitor, FLOAT fLeft, FLOAT fBottom, FLOAT fRight, FLOAT fUp);	//����һ��������������GetSceneRect�õ��ķ�Χ�Ϳ��Ա������У���fRight==fLeft,fBottom == fUp���ڱ���һ���㡣
	//<����>

	

	float GetLoadingProcess();
	KG3DSceneDataInterface* GetSceneDataInterface()
	{
		return m_lpDataInterface;
	}
	
	HRESULT RenderBrush();


    void CheckReleaseFromPvs(KG3DRepresentObject* obj);

	HRESULT RequestObjectTransformationChange(KG3DRepresentObject* pObject);
    HRESULT RequestObjectInformationChange(KG3DRepresentObject* pObject);

	HRESULT RequestCreateSceneBlock(int nLevel,list<POINT>*plistBlock,BOOL bCreateTerrain);
	HRESULT RequestDeleteSceneBlock(int nLevel,list<POINT>*plistBlock,BOOL bDeleteTerrainOnly);

	KG3DSceneRegion& GetErrorRegion(){return m_vErrorRegion;}
public:
    HRESULT CheckConnectNode(KG3DEntityObjectShell* objShell);
	HRESULT ProcessMultiThread();
	HRESULT FindUnLoadedObjectShellInRect(set<KG3DEntityObjectShell*>*psetShells,D3DXVECTOR2 A,D3DXVECTOR2 C);

	virtual HRESULT AddDetailTextureToMaterialMgr(BYTE& byIndex,LPCTSTR pFileName);
	virtual HRESULT DelDetailTextureFromMaterialMgr(BYTE byIndex);
	virtual HRESULT GetAllDetailTextureFromMaterialMgr(map<BYTE,std::string>* pmapDetailTex);
	virtual HRESULT ChangeDetailTexture(BYTE byIndex,LPCTSTR pFileName);
	virtual	HRESULT SetDetailTextureCoordIndex(INT nIndex,BYTE byCoord);
	virtual HRESULT GetDetailTextureCoordIndex(INT nIndex,BYTE& byCoord);
	virtual HRESULT GetDetailTextureUV(INT nIndex,FLOAT* pfScaleU,FLOAT* pfScaleV,FLOAT* pfEnv,FLOAT* pfSpecular,FLOAT* pfEmissive);
	virtual HRESULT SetDetailTextureUV(INT nIndex,FLOAT m_fScaleU,FLOAT m_fScaleV,FLOAT   fEnv,FLOAT   fSpecular,FLOAT   fEmissive);
	virtual HRESULT GetDetailTextureGrassData(BYTE byIndex,BOOL* pbGrass,BYTE* pbyGrassTexIndex,BYTE* pbyPatternIndex);
	virtual HRESULT SetDetailTextureGrassData(BYTE byIndex,BOOL* pbGrass,BYTE* pbyGrassTexIndex,BYTE* pbyPatternIndex,BOOL bSave = FALSE);

	HRESULT GetEntityIntersectRect(KG3DSceneEntityList*plistEntity,D3DXVECTOR2& A,D3DXVECTOR2& C,BOOL bShellMustLoaded);
private:
	void CheckEntityFroceLoad();
public:
	HRESULT ForceLoadAllEntity();
	HRESULT ForceLoadEntity(D3DXVECTOR2 A, D3DXVECTOR2 C);
	HRESULT ForceLoadAllTerrainBlock();
	HRESULT ForceLoadTerrainBlock(D3DXVECTOR2 A, D3DXVECTOR2 C);
	HRESULT ComputeAllEntityBBox();//���㳡�������������BBOX��С�����浽���ݲ㡣
	HRESULT LoadTerrainBlockByPos(float fx, float fz);

	DWORD   GetLoadedObjectShellCount();
public:
	//��·////////////////////////////////////////////////////////////
	KG3DTerrainRoadMgr*  m_lpRoadMgr;
	HRESULT GetVisibleRoad(list<KG3DTerrainRoad*>* plistRoad,const KG3DFrustum* pFrustum);
	virtual HRESULT GetTerrainRoadMgr(IEKG3DTerrainRoadMgr** ppRoadMgr);
	
	KG3DTerrainConverMap* m_lpConverMap;
	UINT m_uConvermapHandle;
	HRESULT InputRoad(KG3DTerrainRoad* pRoad);
	HRESULT EraseRoad(KG3DTerrainRoad* pRoad);
	HRESULT RefreshRoadRiver(BOOL bRefreshRoad,BOOL bRefreshRiver);
	//����
	KG3DTerrainRiverMgr* m_lpRiverMgr;
	virtual HRESULT GetTerrainRiverMgr(IEKG3DTerrainRiverMgr** ppRiverMgr);
	HRESULT InputRiver(KG3DTerrainRiver* pRiver);
	HRESULT EraseRiver(KG3DTerrainRiver* pRiver);

	//////////////////////////////////////////////////////////////////////////
	//���θ߶�ͼ�����������
	HRESULT ExprotTerrainHeightMap(LPCSTR pFileName,DWORD dwOption);
	HRESULT ImportTerrainHeightMap(LPCSTR pFileName,DWORD dwOption);
	HRESULT ApplyBaseMapToSelectedSceneBlock(LPTSTR pBaseMap);
	HRESULT GetSceneHeightInfo(int* pLowest,int* pHeightTotal);
	HRESULT SetSceneHeightInfo(int  nLowest,int  nHeightTotal);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//HRESULT EnableRealTimeShadowMap(BOOL bEnable,KG3DSceneEntityMgr* pEntityMgr);
	//HRESULT ProcessShadowMap(KG3DCamera* pCamera,D3DXVECTOR4& vLight,KG3DSceneSceneEditor* pEditor);
	//HRESULT ShowShadowMapScreen();
	
	///////////////////////////////////////////////////////////////////////////////
#ifdef KG3D_AUTO_TEST 
	virtual HRESULT GetAllEntityInfoHandle(vector<UINT>* vecHandle);
	virtual HRESULT GetEntityInfoformation(UINT uHandle,DWORD* pdwDataType,DWORD* pdwLength,PVOID pData,DWORD dwMaxLen);

	virtual HRESULT GetAllSceneBlock(vector<SceneBlockIndex>* vecSceneBlockIndex);
	virtual HRESULT GetSceneBlockData(int nLevel,POINT Index,DWORD& dwLenght,PVOID pData);//��ȡ�������ݣ��������Լ���������ݵĴ�����ɾ�����ú�������������ݸ���

	virtual HRESULT GetAllTerrainInfoHandle(vector<UINT>* vecHandle);
	virtual HRESULT GetTerrainInfoformation(UINT uHandle,DWORD* pdwDataType,DWORD* pdwLength,PVOID pData);

	virtual HRESULT GetEntityData(map<UINT,KGEntityData>* mapEntityData);
#endif
private:
	//����������� liumingod
	int m_nTerrainCopyWidth;//���Ƶ��εĿ�ȣ�cell��Ŀ
	int m_nTerrainCopyHeight;
	float* m_lpTerrainCopyHeightData;
	BYTE* m_lpTerrainCopyBlendData;
	KG3DTerrainRegionTextureInfo_Old m_TerrainCopyTextureInfo;
	LPD3DXMESH m_lpTerrainCopyBrush;
	float m_fTerrainCopyHeightMax;
	float m_fTerrainCopyHeightMin;
	float m_fTerrainCopyHeightAve;
	D3DXMATRIX m_matCopyBrush;//���Ƶ����ƶ�����
	D3DXVECTOR3 m_vecCopyPos; //���Ƶ���λ��
	float m_fTerrainCopyRotation;//���Ƶ��ε���ת�Ƕ�
	D3DXVECTOR3 m_vTerrainCopyScale;//���Ƶ��εķŴ���С
	char** m_ppTerrainCopyTexturePath;
	int m_nNumTerrainCopyTexture;
	BYTE* m_pRealTextureIndex;

	KG3DSceneSceneEditor* m_lpSceneEditor;
	DWORD m_dwLastTimeCheckMemoryUse_EntityShell;

public:
	BOOL m_bTryPauseMultiThreadLoad;
	BOOL m_bPauseMultiThreadLoad;

	HRESULT PauseMultiThreadLoad(BOOL nEnable);

	HRESULT SetSceneEditor(KG3DSceneSceneEditor* pEditor)
	{
		m_lpSceneEditor = pEditor;
		return S_OK;
	}
	HRESULT GetSceneEditor(KG3DSceneSceneEditor** ppEditor)
	{
		if (ppEditor != NULL)
			*ppEditor = m_lpSceneEditor;
		return S_OK;
	}
	int PosInWhichArea(D3DXVECTOR3& vecPos,float fWidth);
	HRESULT RenderTerrainCopyBrush();
	HRESULT InitTerrainCopyBrush();
	HRESULT PasteSelectedTerrain(DWORD dwMixType,bool bIsPasteBlend,bool bIsMax,float fWeight,float fWidth,AABBOX* bBox);
	HRESULT CopySelectedTerrain(D3DXVECTOR3 vA,D3DXVECTOR3 vC);


	HRESULT ReplaceEntity(vector<UINT>& uEntityHandles, KG3DRepresentObject* m_pObjInHandle, bool all, OUT vector<UINT>& allNeedReplaceHandles);//�����滻��ѡ�еĶ��ʵ�壬all��ʾ�Ƿ����������滻, needReplaceHandles�õ�������������Ҫ����ģ�͵�id�б�,���allΪfalse�����᷵�ص� by Likan
	HRESULT ReloadEntiyInShell(UINT uHandle, KG3DRepresentObject** lppObj);	//���¶�̬����ʵ�� by Likan���������¼��ص�object
};


class KG3DLoadPostProcessor
{
public:
    KG3DLoadPostProcessor();
    ~KG3DLoadPostProcessor();
    
    HRESULT Init();
    HRESULT UnInt();
    HRESULT InputModel(KG3DModel *pModel);
    HRESULT InputRepresentObject(KG3DRepresentObject *pRepresentObject);
    HRESULT FrameMove();
private:
    KMutex m_Lock;
    std::vector<KG3DModel *> m_Models;
    std::vector<KG3DModel *> m_ModelsCache;
    std::vector<KG3DRepresentObject *> m_RepresentObjects;
    std::vector<KG3DRepresentObject *> m_RepresentObjectsCache;
};

extern KG3DLoadPostProcessor g_LoadPostProcessor;