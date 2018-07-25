////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEKG3DScene.h
//  Version     : 1.0
//  Creator     : Wu Caizhong
//  Create Date : 2006-11-13 15:24:29
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IEKG3DSCENE_H_
#define _INCLUDE_IEKG3DSCENE_H_

#include "KG3DInterface.h"


#define OUTPUTWND_SCENEEDITOR_MAIN					0	//���ܸģ��ͻ���ʹ�õ���0����û��ʹ�������
//#define OUTPUTWND_SCENEEDITOR_SMALLMAP				1
#define OUTPUTWND_SCENEEDITOR_OBJECTPREVEIW			2
#define OUTPUTWND_SCENEEDITOR_TOPVIEW				3
//#define OUTPUTWND_SCENEEDITOR_RENDERTOOL			4   
#define OUTPUTWND_SCENEEDITOR_PREVIEW				5   // �߻�����Ԥ��

#define OUTPUTWND_RESOURCE_MINIVIEW					8
#define OUTPUTWND_SCENEEDITOR_TEXTUREVIEW			16

#define OUTPUTWND_MODELEDITOR_MAIN					0
#define OUTPUTWND_MODELEDITOR_PREVIEW				1   // �߻�����Ԥ��
#define OUTPUTWND_MODELEDITOR_PREVIEW_NPC_PORTRAIT  2   // �߻�����Ԥ��

#define OUTPUTWND_OBJECTEDITOR_MAIN					0
#define OUTPUTWND_SFXEDITOR_MAIN					0
#define OUTPUTWND_PVSEDITOR_MAIN                    0
#define MAX_LIGHT_IN_SCENE							8

#define OUTPUTWND_DATAFLOWEDITOR_MAIN               0
#define OUTPUTWND_DATAFLOWEDITOR_PREVIEW            1
#define OUTPUTWND_DATAFLOWEDITOR_RUN                2
#define OUTPUTWND_DATAFLOWEDITOR_PROCESSOR          3
#define OUTPUTWND_DATAFLOWEDITOR_EVENT              4

typedef struct IEKG3DEnvEffLight IEKG3DLight;
interface IEKG3DSkySystem;
interface IEKG3DSceneEntity;
class IKG3DScene;
interface IEKG3DSceneOutputWnd;
interface IEKG3DSceneModelEditor;
interface IEKG3DSceneSFXEditor;
interface IEKG3DSceneSceneEditor;
interface IEKG3DSceneObjectEditor;
interface IEKG3DSceneEditorBase;
interface IEKG3DSceneCameraMovement;
interface IEKG3DEnvironment;
interface IEKG3DSceneDataFlowEditor;
interface IEKG3DCommonContainer;
class KG3DSceneEntity;
interface IEKG3DRegionInfoManager;
class IKG3DMessageReceiverBase;
class IEKG3DSceneSelectionTool;

typedef BOOL (KTraverseEntityMeshForGenerateLogicObstacleCallback)(
    float fXStart, float fYStart, float fZStart, 
    LPD3DXMESH piMesh,int nNumSubset,BOOL bSubsetNoCell[],
    const D3DXMATRIX& crTransformationMatrix,
    int nDynamicObstacleGroupIndex,LPSTR pMeshName
);

////////////////////////////////////////////////////////////////////////////////
typedef struct _OUTPUTWND_TEXTURE_PARAM
{
	DWORD dwTextureID;
}OUTPUTWND_TEXTURE_PARAM;
//////////////////////////////////////////////////////////////////////////
enum
{
	//scene_setting_how_many_pixels_per_region = 0,
	scene_setting_enable_render_terrain_mask = 0,//Data��0��ֹ��ͼ��Ⱦ
	scene_setting_count,
	scene_setting_max = 64,
};
#define TERRAINREPRESENTSFXNUM 4
struct TerrainRepresentInfo
{
	DWORD dwType;//�ر�����
	TCHAR strFileName[MAX_PATH];//��ͼ����
	TCHAR strAnnotate[MAX_PATH];//ע��
	TCHAR strSound[MAX_PATH];//�������������
	TCHAR strSfx[TERRAINREPRESENTSFXNUM][MAX_PATH];//�����������Ч
    TCHAR strSfxTerrain[MAX_PATH];//�ر�ϸ����Ч���罦��Ļ���
	float fSfxPlayRate;//��Ч���ŵļ���
};

class IEKG3DRepresentObject;
class IEKG3DScenePvsEditor;
class IEKG3DSceneSelectBase;

interface IEKG3DScene : public IKG3DScene
{
	virtual HRESULT SaveToFile(const char cszFileName[],   unsigned uOption) = 0;	//ע��uOption�����õģ�����ָ�����������

    virtual HRESULT RefreshPhysisc() = 0;
    virtual HRESULT GetSceneSFXEditor(IEKG3DSceneSFXEditor **ppiRetSFXEditor) = 0;
    virtual IEKG3DScenePvsEditor* GetScenePvsEditor() = 0;
    virtual IEKG3DSceneSelectBase* GetSelectBase() = 0;
   
    virtual HRESULT RenderWindowBegin(IEKG3DSceneOutputWnd* pWnd,BOOL bSetViewPort) = 0;
    virtual HRESULT RenderWindowEnd(IEKG3DSceneOutputWnd* pWnd) = 0;

    // if no more entity, function return S_OK, and set *ppiRetEntity to null
    virtual HRESULT GetFirstRenderEntity(IEKG3DSceneEntity **ppiRetEntity) = 0;
    virtual HRESULT GetNextRenderEntity(IEKG3DSceneEntity **ppiRetEntity) = 0;
    virtual HRESULT FindRenderEntity(
        SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer, IEKG3DSceneEntity **ppiRetEntity
    ) = 0;

	//ǰ��������Model�õģ�����������Editor�õ�
	virtual HRESULT AddRenderEntity(IKG3DResourceBase *pEntity,    unsigned uClass, BOOL bSelectable,BOOL bLight) = 0;
	virtual HRESULT RemoveRenderEntity(IKG3DResourceBase *pEntity, unsigned uClass, BOOL bSelectable) = 0;

    virtual HRESULT AddRenderEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer) = 0;
    virtual HRESULT RemoveRenderEntity(SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer) = 0;

	//����SceneSceneEditor��˵��
	STDMETHOD(AddRepresentObject)(IEKG3DRepresentObject* pObject, ULONG uOption, IEKG3DRepresentObject** ppCreatedObject) PURE;
	STDMETHOD(RemoveRepresentObject)(IEKG3DRepresentObject* pObject) = 0;

    virtual HRESULT GetAmbient(DWORD *pdwRetAmbient) = 0;
    virtual HRESULT SetAmbient(DWORD dwAmbient) = 0;

    virtual HRESULT AddOutputWindow(LPCTSTR szName, HWND hWnd, DWORD dwType, DWORD_PTR dwOption, int* pOutputWindowID) = 0;
    virtual HRESULT GetOutputWindow(int nID, IEKG3DSceneOutputWnd **ppiRetSceneOutputWnd) = 0;
    virtual HRESULT SetCurrentOutputWindow(int nID) = 0;
    virtual HRESULT GetCurOutputWnd(IEKG3DSceneOutputWnd **ppiRetCurOutputWnd) = 0;
    virtual HRESULT GetCurRenderWnd(IEKG3DSceneOutputWnd **ppiRetCurOutputWnd) = 0;

	virtual HRESULT GetSceneModelEditor(IEKG3DSceneModelEditor** ppModelEditor) = 0;
	virtual HRESULT GetSceneSceneEditor(IEKG3DSceneSceneEditor** ppSceneEditor) = 0;
	virtual HRESULT GetSceneObjectEditor(IEKG3DSceneObjectEditor** ppObjectEditor) = 0;
	virtual HRESULT GetSceneEditorBase(IEKG3DSceneEditorBase** pBase) = 0;
	virtual HRESULT GetSceneDataFlowEditor(IEKG3DSceneDataFlowEditor** ppDataFlowEditor) = 0;

	//////////////////////////////////////////////////////////////////////////
	//Other Control Object
	virtual HRESULT GetCameraMovement(IEKG3DSceneCameraMovement** ppiCameraMovement) = 0;
	virtual HRESULT GetEnvironment(IEKG3DEnvironment** ppiEnvironment) = 0;

    virtual HRESULT TraverseEntityMeshForGenerateLogicObstacle(
        float fWorldStartX, float fWorldStartY, int nRegionPosX, int nRegionPosY,
        KTraverseEntityMeshForGenerateLogicObstacleCallback* pCallback
    ) = 0;

	virtual HRESULT GetEngineIEInterface(DWORD dwType, DWORD_PTR wParam, DWORD_PTR lPram, VOID** ppiInterface) = 0;//�õ��Ľӿڣ������Release�����ģ�����������Release
	virtual HRESULT GetEngineIEAbstructInterface(DWORD dwAbstructType, DWORD dwType,  DWORD_PTR wParam, DWORD_PTR lParam, VOID** ppiInterface) = 0;//�õ��Ľӿڣ������Release�����ģ�����������Release

	virtual LPDIRECT3DCUBETEXTURE9 GetEnvMap(LPTSTR pNameBuffer, size_t uBufferCount) = 0;	//����˳��õ�����
    virtual HRESULT SetEnvMap(LPCTSTR szPath)  = 0;

	virtual HRESULT SetSceneSettingData(DWORD dwEnum, DWORD_PTR dwData) = 0;
	virtual HRESULT GetSceneSettingData(DWORD dwEnum, DWORD_PTR* pdwData) = 0;

	virtual HRESULT GetLogicalSceneSize(int* pXStart,int* pZStart,int* pWidthX,int* pWidthZ) = 0;
	virtual HRESULT SetLogicalSceneSize(int  nXStart,int  nZStart,int  nWidthX,int  nWidthZ) = 0;
	
	virtual HRESULT GetAppointLogicalRegion(int& nRegionXStart,int& nRegionZStart,int& nCountRegionX,int& nCountRegionZ) = 0;
	virtual HRESULT SetAppointLogicalRegion(int nRegionXStart,int nRegionZStart,int nCountRegionX,int nCountRegionZ) = 0;
	//�¼��������������������̷�ע�ᣬ��Ϊ���е�Listener����ҪCPUʱ�䴦���
	virtual HRESULT RegisterListener(DWORD ListenerType, IKG3DMessageReceiverBase* pReceiver) = 0;
	virtual HRESULT UnRegisterListener(DWORD ListenerType, IKG3DMessageReceiverBase* pReceiver) = 0;

	virtual HRESULT GetName(LPTSTR pNameBuffer, size_t uBufferSize) = 0;

	virtual HRESULT GetSelectionToolInterface(IEKG3DSceneSelectionTool** ppTool) = 0;
	
	virtual HRESULT GetPickRayPos(D3DXVECTOR3* pOutPos) = 0;

    virtual HRESULT LoadCameraAnimation(LPSTR pszCameraAniFile) = 0;
    virtual HRESULT StartPlayCamAni() = 0;
    virtual HRESULT StopPlayCamAni()  = 0;
    virtual HRESULT PausePlayCamAni() = 0;
};

#endif //_INCLUDE_IEKG3DSCENE_H_
