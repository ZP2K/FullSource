#pragma once

#include "..\..\..\include\KG3DEngine\KG3DInterface.h"
#include "kg3dfilefinder.h"
#include "KG3DRepresentNPCTemplate.h"
#include "Scene/KG3DSceneWndManager.h"

#include "../EditInterface/IEOther.h" //by dengzhihui Refactor 2006��11��2��

#include "KG3DEditMoveGrid.h"
#include "KG3DEditRotationGrid.h"
#include "KG3DEditScaleGrid.h"
#include "KG3DEngineOptionCaps.h"

using namespace std;

#define MULTITRHEAD_NONE      0
#define MULTITRHEAD_ANIFINISH 1
#define MULTITRHEAD_TEXTURELOADFINISH 2
#define MULTITRHEAD_MESHLOADFINISH 3

class KG3DScene;
class IKG3DUI;
class IIPPCodec;
class KG3DPhysiscManager;
struct IKG3DSoundShell;
class KG3DTimeClock;

class TimeControl
{
protected:
    float m_fSpeedBack;
    float m_fSpeedDest;
    float m_fSpeedIncrease;
private:
    UINT m_FrameCount;//ע�����ͣ�UINT_MAX��32λϵͳ���棬��һ��60֡�㣬Ҳ������19884Сʱ�������������
public:
    DWORD m_dwNowTime;
    DWORD m_dwLastTime;

    DWORD Now;
    DWORD Lst;
    float m_fSpeed;
    DWORD m_dwTimeGetTimeLast;
    DWORD m_dwInterval;

    DWORD m_dwLastAppTime;
    float m_fScaling;

    TimeControl();
    void FrameMove(BOOL bSaveToFile);

    void SetScaling(float fScal)
    {
        m_fScaling = fScal;
    }

    DWORD GetNowTime()
    {
        return m_dwNowTime;
    }

    float GetTinyTime()
    {
        return (float)(m_dwNowTime - m_dwLastTime);
    }

    DWORD GetTinyTimeOriginal()
    {
        return m_dwNowTime - m_dwLastTime;
    }

    void Pause()
    {
        m_fSpeedBack = m_fSpeed;
        m_fSpeed = 0;
    }
    void Restore()
    {
        m_fSpeed = m_fSpeedBack;
    }
    void SetDestSpeed(float Value,float Increate = 0.1f)
    {
        m_fSpeedIncrease = Increate;
        m_fSpeedDest = Value;
    }
    UINT GetFrameCount()const {return m_FrameCount;}
};

class KG3DEngineManager : public IEKG3DEngineManager //by dengzhihui Refactor 2006��11��2��
{
private:

	HINSTANCE	m_hSoundDll;//����ģ����
	BOOL		m_MovieReady;
	BOOL        m_bRecordReady;

	list<KG3DScene*> m_listScene;
	//------------------------------------------------------
	// Ϊ����������������������
	typedef list<KG3DScene*>::iterator IterListScene;	// Add by FengBo
	IterListScene			m_iterListScene;
	//------------------------------------------------------
	KG3DScene*         m_lpFocusScene;
    //----------------------------------------------------

public:
	BOOL m_bRenderFocusSceneOnly;
	KG3DFileFinder m_FileFinder;

	KG3DEngineManager();
	virtual ~KG3DEngineManager();

	KG3DPhysiscManager* m_pPhysicsManager;

	virtual IRecPlayer* GetNewRecPlayer();
	virtual RecordOption* GetRecordOption();
	virtual void SetRecordOption(const RecordOption *pOption);

public:
	virtual IKG3DEnventCenter* GetEventCenter();

	virtual IKG3DFaceMotionSettings* GetFaceMotionSettings();
	virtual BOOL GetImportOldScene(); //��ȡ�Ƿ���ؾɵ��εı�־

	virtual HRESULT Init(unsigned uMode, unsigned int uFlag, HWND hBaseWindow, HWND hRenderWindow); // uMode = 0 : windowed, uMode = 1: fullscreen
	virtual HRESULT UnInit();

    virtual KG3DScene* GetFocusScene() { return m_lpFocusScene; };

    virtual IKG3DResourceManager* Get3DModelTable();
	virtual IKG3DSoundBallTable* GetSoundBallTable();
    virtual IKG3DUI* Get3DUI();
	virtual LPDIRECT3DDEVICE9 GetDevice();
	virtual LPDIRECT3D9	GetD3D();

    virtual void GetHDFilePath(char szHD1Path[], char szHD2Path[], const char szRetPath[]);

	//////////////////////////////////////////////////////////////////////////
	//��Ϣ����
	virtual HRESULT ProcessMessage(unsigned message, WPARAM uParam, LPARAM ulParam);//<PROCESS_MESSAGE>
	//��Ⱦ
	virtual HRESULT FrameMove();
	virtual HRESULT Render();

    virtual KG3DSceneSceneEditor* GetMainScene();

    virtual BOOL	GetRenderOption(RENDER_OPTION opt);
    virtual HRESULT SetRenderOption(RENDER_OPTION opt, BOOL bEnable);

    virtual BOOL    GetFrameMoveOption(FRAMEMOVE_OPTION opt);
    virtual HRESULT SetFrameMoveOption(FRAMEMOVE_OPTION opt, BOOL bEnable);

    virtual DWORD   GetRenderParam(RENDER_PARAM Param);
    virtual HRESULT SetRenderParam(RENDER_PARAM Param, DWORD Value);

    virtual PostRenderParams* LockPostRenderParams(BOOL bReadOnly);
    virtual void UnlockPostRenderParams();

	virtual	HRESULT GetEngineCaps(KG3DEngineCaps* pCaps);
	//virtual HRESULT GetCaps(D3DCAPS9 **ppRetCaps9);

	//////////////////////////////////////////////////////////////////////////
	//��������
	virtual HRESULT NewOneScene(unsigned uType, IKG3DScene **ppRetScene);
	virtual HRESULT RemoveScene(IKG3DScene *pScene);
	
	//(by dengzhihui Refactor 2006��11��2��
	virtual HRESULT Reset();//�ͷ�������Դ,�ָ�Ϊ������״̬ 
	virtual HRESULT SetFocusScene(IKG3DScene* pScene);		
	virtual HRESULT SetMaxViewScene(IKG3DScene* pScene);	
	//)
	HRESULT SetFocusScene(KG3DScene* pScene);
	HRESULT SetMaxViewScene(KG3DScene* pScene);	

	//////////////////////////////////////////////////////////////////////////
	// Add Option Func . By FengBo 2007-9-5
	virtual HRESULT SetEngineOption(const KG3DEngineOption* pEngineOption);
	virtual HRESULT GetEngineOption(KG3DEngineOption* pEngineOption);

    virtual int GetColorCurveStypeNum();
    virtual void GetColorCurveStypeName(char szName[], int nIndex);


    virtual KG3DEngineOption* GetEngineOptionPtr();
	virtual HRESULT SetEngineEditorOption(KG3DEngineEditorOption* pEngineEditorOption);
	virtual HRESULT GetEngineEditorOption(KG3DEngineEditorOption* pEngineEditorOption);

	virtual HRESULT ExecuteCommand(LPCTSTR szCommand);

    virtual void SetOnPresentCallBackFunc(pfOnPresent pfCallback);
	virtual void SetOnRenderEndCallBackFunc(pfOnRenderEnd pfCallback);

    // ��Ƶ¼��
    BOOL InitIPPVideo();
    void UnInitIPPVideo();

    virtual HRESULT SetIPPVideo(void* pfnCreateIPPCodec);
	virtual HRESULT MovieRecord(LPCTSTR szFolder, int nSizeScale, UINT uFilter, UINT uQuality, int nEncoder, int nFps);
	virtual HRESULT MovieStop();
	virtual BOOL    MovieReady() const;
	virtual VOID    MoviePresent();
	virtual VOID    MovieViewport(RECT const& Viewport);

	//�¼�¼��
	virtual BOOL    CanRecord();
	virtual BOOL    IsRecording();
	virtual HRESULT StartRecord(LPCTSTR szFolder,unsigned uMode);
	virtual HRESULT StopRecord();


	//�����趨<CTH>///////////////////////////////////////////////////////////////////////
	virtual HRESULT GetRegionInfoManager(const char cszFileName[]
									, unsigned uFileNameHash
									, DWORD_PTR dwOption
									, IKG3DRegionInfoManager** ppRIM);
	virtual HRESULT GetIEKG3DRegionInfoManagerTable(IEKG3DRegionInfoManagerTable** ppTable);
	virtual HRESULT GetIEInterface(DWORD dwType1, DWORD dwType2, DWORD_PTR wParam, DWORD_PTR lParam, LPVOID* ppInterface);//��һ��Type�ǳ������ͣ��ڶ����Ǿ������ͣ����������ǻ�ȡ����
	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT GetUseLogicObject(BOOL* pUseLogicObject);
	//by dengzhihui Refactor 2006��11��6��
	virtual HRESULT GetUseSO3Logical(BOOL* pUseLogic);
	HRESULT GetGraphicsTool(IEKG3DGraphicsTool** pTool);
	
	HRESULT GetDefWorkDirectory(LPTSTR pszBuf, DWORD dwBufSizeInCharCount);
	HRESULT GetD3DDevice(LPDIRECT3DDEVICE9* ppDevice);
	HRESULT SetRenderTerrain(BOOL bRenderTerrain);
	BOOL GetRenderTerrain();
	HRESULT GetUI(IKG3DUI** pUI);

	IEKG3DMesh* FindMeshBase(DWORD dwID);
    
	HRESULT OnLostDevice();
	HRESULT OnResetDevice();

    TimeControl& GetTimeControl()  { return m_TimeControl; }

    //by xiayong
    virtual HRESULT GetIEKG3DTextureTable(IEKG3DTextureTable **ppTexture);
    virtual HRESULT GetIEKG3DModelTable(IEKG3DModelTable **ppModelTable);
	virtual HRESULT GetIEKG3DClipTable(IEKG3DClipTable **ppClipTable);
    virtual HRESULT GetNewIEKG3DMaterial(IEKG3DMaterial **ppMat);
    virtual HRESULT DestroyIEKG3DMaterial(IEKG3DMaterial *pMat);
    virtual HRESULT ExchangeModelSubsetID(IEKG3DModel* pIModel, int nSubset1, int nSubset2);
    virtual HRESULT GetNewIEKG3DLight(IEKG3DEnvEffLight **ppiLight);
    virtual HRESULT GetBaseWnd(HWND *phBaseWnd); //ȡȫ�ֱ���g_hBaseWnd

    virtual HRESULT GetSpeedTreeLeafScale(FLOAT *pfRetScale);
    virtual HRESULT SetSpeedTreeLeafScale(FLOAT fScale);

    virtual HRESULT GetRenderTreeFlag(BOOL *pbRetRenderTree);
    virtual HRESULT SetRenderTreeFlag(BOOL bRenderTree);

    virtual HRESULT ChangeMovingState();
    
    virtual HRESULT GetAutoReloadTexture(BOOL *pbAutoReloadTexture);
    virtual HRESULT SetAutoReloadTexture(BOOL bAutoReloadTexture);

    virtual HRESULT SetFullScreen(BOOL bFullScreen);
    
	//<Object���>
	virtual HRESULT GetRepresentObjectTable(IKG3DResourceManager** ppObjectTable);
	/*
	Build�����ú��������lpFileName��ΪNULL����ôType���Ǳ���ģ������FileName���ж����ͣ�
	��Щ�����ǲ���Ҫ�ļ��ģ��������Type������ҪFileName
	*/
	virtual HRESULT BuildKG3DRepresentObject(LPCTSTR lpFileName, DWORD dwType, DWORD_PTR wParam
		, DWORD_PTR lParam, IEKG3DRepresentObject **ppObject);
	virtual HRESULT NewKG3DRepresentObject(DWORD dwType, DWORD_PTR wParam, DWORD_PTR lParam, IEKG3DRepresentObject **ppObject);
	//</Object���>

    //MathTool 
   // virtual void LPCTSTR2IntInRange(LPCTSTR czSource, INT* pnDes, INT nLowerBound, INT nUpperBound);
    //virtual D3DCOLOR ColorValue(D3DCOLORVALUE co);
    //virtual D3DCOLORVALUE Color2Value(D3DCOLOR Color);
    //virtual void Color2Ref( const D3DCOLOR &D3DColor, COLORREF &CRColor);

    // GrassPattern
    virtual HRESULT LoadPattern(LPCSTR strFileName, int& nIndex);
    virtual HRESULT FindPattern(LPCSTR strFileName, INT *pnRetIndex);

    //  Represent object template
    virtual HRESULT GetRepresentObjectPropertyCount(DWORD *pdwRetCount);
    virtual HRESULT GetRepresentObjectProperty(DWORD dwIndex, OBJECT_PROPERTY **ppRetObjProperty);

    // Represent NPC template
    virtual HRESULT GetRepresentNPCPropertyCount(DWORD *pdwRetCount);
    virtual HRESULT GetRepresentNPCProperty(DWORD dwIndex, NPC_PROPERTY **ppRetNPCProperty);
    
    virtual HRESULT GetRelativePath(const TCHAR* strAbsPath11, const TCHAR* strAbsPath21, TCHAR* strRelativePath);
    virtual HRESULT GetEnableAddEntity(BOOL **ppbEnableAddEntity);

	virtual HRESULT GetKeyStateBuffer(BYTE** ppbyKeyStateBuf);
	//CTH <RTTI_INTERFACE>
	virtual HRESULT GetKG3DRTTITypeManager(IKG3DRTTITypeManager** pRTTI);

    virtual void RegistPushAlertMessageFunction(void (*)(const char[]));

	virtual BOOL GetAmibentShaderOnly();
	virtual HRESULT SetAmibentShaderOnly(BOOL bValue);
	virtual void ReloadAnimation();

	// �߻��༭��
	virtual void ManipulateSceneList(IEKG3DScene* pTargetScene, BOOL bAdd);

	void OnPresent(double fValue);
	void OnRenderEnd(double fValue);

	virtual HRESULT InitGameContext(KG3D_GAME_INIT_CONTEXT* pContext);
	virtual HRESULT GetInterface(DWORD dwType, DWORD_PTR p1, DWORD_PTR p2, LPVOID* ppInterface);
    
	IKG3DSoundShell* GetSound();
	void SetSoundFocusScene(IKG3DScene *pScene);
	IKG3DScene* GetSoundFocusScene();

    virtual IKG3DEngineOptionCaps* CreateIKG3DEngineOptionCaps();
    virtual void RemoveKG3DEngineOptionCaps(IKG3DEngineOptionCaps* pIEngineOptionCaps);
    void UnintEngineOptionCaps();
    list<KG3DEngineOptionCaps*> m_listEngineOptionCaps;

	// �ƶ��������  by liumingod
	KG3DEditMoveGrid        m_EditMoveGrid;
	KG3DEditRotationGrid    m_EditRotationGrid;
	KG3DEditScaleGrid       m_EditScaleGrid;

	KG3DTimeClock& GetTimeClock();
	ULONG	IsInitFinished();//��ΪEngineManager��ȫ�ֱ�����������ȫ�ֺ����е��õ�ʱ�򣬼��϶����Ƿ��Ѿ���ʼ�����жϱȽϰ�ȫ���ⲿǧ��ǧ��Ҫ������״̬����Ӧ�ó�ʼ����������ã�Ҳ��Ҫ�����������¶���ӿڡ������d3d�ĺ����������õ�һ��Deviceָ�룬��û�д����ж���IsInit�����ⲿ��˵����֤�õ��Ķ���״̬���������ȽϺã���Ȼ���Ѵ���

	BOOL NeedDrawTwice();
private:
	HRESULT InitGraphicsEngine(HWND hBaseWindow, HWND hRenderWindow);
    HRESULT UnInitGraphicsEngine();

	void SetSoundShell(IKG3DSoundShell *pShell);

	HRESULT LoadConfig(BOOL* pFileIsNotExit);
	//----------------- Add  by FengBo 2007-9-5	
	HRESULT SaveEngineOptionConfig();			// ����ͻ��˸ı������ѡ��
	HRESULT SaveEngineEditorOptionConfig();	// �������EditorEngineOption��ѡ��
	//---------------------------------
	HRESULT SaveConfig();

	HRESULT LoadPreloadSpeedTree(LPCSTR strFileName);	

#if defined(DEBUG) | defined(_DEBUG)
	VOID	RuntimeTest();
#endif

private:
	IKG3DSoundShell *m_pSound;
	pfOnPresent m_pfOnPresent;
	pfOnRenderEnd m_pfOnRenderEnd;

    DWORD   m_FrameMoveOption;
	DWORD	m_RenderOption;

    HMODULE m_hModuleIPPCodec;
    void* m_pCreateIPPCodec;
    IIPPCodec* m_pIPPCodec;
	KG3DTimeClock* m_pTimeClock;
	IKG3DScene *m_pSoundFocusScene;
    TimeControl m_TimeControl;

    BOOL m_bPostRenderParamsLockReadOnly;

	enum
	{
		em_init_finished = 1,
	};
	std::bitset<sizeof(DWORD)>	m_States;

	// Inherited via IEKG3DEngineManager
	virtual BOOL AttachEventHandle(void * pEventCommon, void * pLogicEventMgr, void * pRepresentEventMgr) override;
	virtual BOOL SetFluencyParam(KFluencyParam * pFluencyParam) override;
	virtual KFluencyParam * GetFluencyParam() override;
	virtual IKG3DModelManager * Get3DModelTableEx() override;
	virtual IKG3DSoundBallTable * GetSoundPolygonTable() override;
	virtual IKG3DSoundBallTable * GetSoundReverbTable() override;
	virtual int GetVideoDll(HMODULE * pVideoDLL) override;
	virtual IKG3DDynamicWeather * GetDynamicWeather() override;
	virtual IKG3DFaceMakerManager * GetFaceMakerManager() override;
	virtual IKG3DMeshMergeManager * GetMeshMergeManager() override;
	virtual IKG3DPSFilterManager * GetPSFilterManager() override;
	virtual IKG3DPerformanceTweaker * GetPerformanceTweaker() override;
	virtual IKG3DSceneResponseManager * GetSceneResponseManager() override;
	virtual HRESULT AddOutputWindow(TCHAR szName[], HWND hWnd, DWORD dwType, DWORD_PTR dwOption) override;
	virtual HRESULT RemoveOutputWindow(int nID) override;
	virtual void SetPaintThreadID(DWORD dwThreadID) override;
	virtual void SetRenderThreadPaint(BOOL bIsRenderThread) override;
	virtual BOOL IsDeviceLost() const override;
	virtual HRESULT ResetDeveice() override;
	virtual HRESULT DoGlobalPreLoad() override;
	virtual HRESULT UnLoadGlobalPreLoad() override;
	virtual HRESULT EnableTextureAutoScale(BOOL bEnable) override;
	virtual HRESULT SetPostRenderDoFDis(FLOAT fDis) override;
	virtual HRESULT GetPostRenderDoFDis(FLOAT * pfDis) override;
	virtual HRESULT SetPostRenderDoFNear(FLOAT fNear) override;
	virtual HRESULT GetPostRenderDoFNear(FLOAT * pfNear) override;
	virtual HRESULT SetPostRenderDoFFar(FLOAT fFar) override;
	virtual HRESULT GetPostRenderDoFFar(FLOAT * pfFar) override;
	virtual HRESULT SetDofUseCameraParam(BOOL bUseCameraParam) override;
	virtual BOOL IsDofUseCameraParam() override;
	virtual HRESULT SetHueScale(IKG3DScene * pScene, FLOAT fHueScale) override;
	virtual HRESULT SetHueBias(IKG3DScene * pScene, FLOAT fHueBias) override;
	virtual HRESULT SetSaturationScale(IKG3DScene * pScene, FLOAT fSaturationScale) override;
	virtual HRESULT SetSaturationBias(IKG3DScene * pScene, FLOAT fSaturationBias) override;
	virtual HRESULT SetIntensityScale(IKG3DScene * pScene, FLOAT fIntensityScale) override;
	virtual HRESULT SetIntensityBias(IKG3DScene * pScene, FLOAT fIntensityBias) override;
	virtual HRESULT GetHueScale(IKG3DScene * pScene, FLOAT & fHueScale) override;
	virtual HRESULT GetHueBias(IKG3DScene * pScene, FLOAT & fHueBias) override;
	virtual HRESULT GetSaturationScale(IKG3DScene * pScene, FLOAT & fSaturationScale) override;
	virtual HRESULT GetSaturationBias(IKG3DScene * pScene, FLOAT & fSaturationBias) override;
	virtual HRESULT GetIntensityScale(IKG3DScene * pScene, FLOAT & fIntensityScale) override;
	virtual HRESULT GetIntensityBias(IKG3DScene * pScene, FLOAT & fIntensityBias) override;
};

//////////////////////////////////////////////////////////////////////////
//ȫ�ֱ���
extern KG3DEngineManager           g_cEngineManager;

extern HWND                        g_hBaseWnd;//�����Ĵ���
extern HWND                        g_hRenderWnd;//Ĭ�ϵ���Ⱦ����

extern BOOL         g_Mouse_L;//������״̬
extern BOOL         g_Mouse_M;//����м�״̬
extern BOOL         g_Mouse_R;//����Ҽ�״̬
extern BYTE         g_KeyState[256];//����״̬
extern int          g_nMouseMoveX;
extern int          g_nMouseMoveY;
extern int          g_nMouseMoveKey;
extern TCHAR        g_Def_WorkDirectory[256];//ִ���ļ�Ŀ¼
extern KG3DFileFinder* g_pFileFinder;

extern BOOL g_bClient;//�Ƿ��ڿͻ���
extern BOOL g_bRePlay;
extern BOOL g_bAutoReloadTexture;
extern BOOL g_bFullScreen;
// Modify by FengBo 2007-9-5
extern KG3DEngineOption g_cEngineOption;			//�ͻ���ʹ�õ�ѡ��
extern KG3DEngineEditorOption g_eEngineOption;	//�༭����ʹ�õ�ѡ��
extern DWORD              g_dwRenderCount;
const KG3DLogicalConstances& g_GetLogicalConstants();	//�߼��йصı�����������߼���ʼ��
	
extern void (*g_AddAlertMessage)(const char[]);

#define MULTITRHEAD_ANI_BUFFERSIZE 128

extern HRESULT g_ReportResourceUse();
extern BOOL g_bEnablePointLight;

extern TCHAR g_szModuleFilePath[MAX_PATH];
extern TCHAR g_szConfigFilePath[MAX_PATH];

#if defined(_DEBUG) | defined(DEBUG)
HRESULT	g_KG3DEngineInterfaceTest(KG3DEngineManager* pEM);	//��������Ľӿ�//��Debugģʽ������ʱ��õ�ָ�룬Ȼ����Watch�����е���//CTH

#endif