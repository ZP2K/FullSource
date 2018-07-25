#pragma once
//#include "kg3dkeyframedatabase.h"

#include "IEKG3DSceneCamera.h"
#include "KG3DMessenger.h"

class KG3DCamera;
class KG3DSceneEntity;
class IEKG3DScene;
class KG3DRepresentObjectDummy;
/************************************************************************/
/* @brief ����������࣬���ڼ�¼�Ϳ�����������Ŷ���
/************************************************************************/
class KG3DSceneCameraAnimation : public IEKG3DSceneCameraAnimation, public KG3DMessageReceiver
{
public:
/************************************************************************/
/* @brief �ṹ�壬���ڼ�¼�ؼ�֡��һ���ؼ�֡����1.�����λ�����ߣ�2.������������ߣ�3.�����λ�ã�4.���������λ�á�������ComputeTangent�������㡣
/************************************************************************/
	enum
	{
		emCameraKeyFrameForSelection_PositionTan = 0,
		emCameraKeyFrameForSelection_LookAtTan = 1,
		emCameraKeyFrameForSelection_Position = 2,
		emCameraKeyFrameForSelection_LookAt = 3,
		emCameraKeyFrameForSelection_Center = 4,
	};
	struct CameraKeyFrame : public IKG3DSceneCameraKeyFrame
	{
		CameraKeyFrame()
		{
			memset(this, 0, sizeof(IKG3DSceneCameraKeyFrame));
		}
		CameraKeyFrame(const D3DXVECTOR3& vPosTemp, const D3DXVECTOR3& vLookAtTemp, const D3DXVECTOR3& vPosTanTemp, const D3DXVECTOR3& vLookAtTangentTemp)
		{
			vPosition = vPosTemp;
			vLookAt = vLookAtTemp;
			vPositionTangent = vPosTanTemp;
			vLookAtTangent = vLookAtTangentTemp;
		}
		enum{vec_count = 4,};
		size_t GetVecCount(){return vec_count;}
		DWORD  GetVecDesc(size_t uSize)
		{
			static DWORD DescArray[] = 
			{
				emCameraKeyFrameForSelection_PositionTan,
				emCameraKeyFrameForSelection_LookAtTan,
				emCameraKeyFrameForSelection_Position,
				emCameraKeyFrameForSelection_LookAt,
			};
			_ASSERTE(uSize < vec_count);
			return DescArray[uSize];
		}
		D3DXVECTOR3&	GetVec(size_t uSize)
		{
			switch(uSize)
			{
			case 0:
				return vPositionTangent;
				break;
			case 1:
				return vLookAtTangent;
				break;
			case 2:
				return vPosition;
			    break;
			case 3:
				return vLookAt;
			    break;
			default:
				_ASSERTE(NULL);
			    break;
			}
			static D3DXVECTOR3 vecError;
			return vecError;
		}
	};
	typedef map<DWORD, CameraKeyFrame> TypeKeyContainer;
	enum{max_name_length = 64,};
	enum{cur_version_main = 1, cur_version_sub = 10000,};
    static VOID CopyKeyFrame(IKG3DSceneCameraKeyFrame& From, DWORD dwTime, IKG3DSceneCameraKeyFrameEx& To)
	{
		memcpy_s(&To, sizeof(IKG3DSceneCameraKeyFrame), &From, sizeof(IKG3DSceneCameraKeyFrame));
		To.dwTime = dwTime;
	}
private:
	struct TinyLine//�˶������ϵ�һС��
	{
		D3DXVECTOR3 vLookAtA,vLookAtB,vPositionA,vPositionB;
		float fDistance;
		float fDistanceStart;
		float fDistanceEnd;

		HRESULT GetDataByTinyLine(CameraKeyFrame* pValue, float fDistance);
	};
	vector<TinyLine>m_vecTineLine;
    float   m_fDistanceTotal;

	HRESULT GetDataByTinyLine(CameraKeyFrame* pValue, DWORD dwTime);

	HRESULT ComputeTinyLine();

	std::string	m_szName;		///���������֣�
	float	m_fPlaySpeed;	///���ŵ�������ӣ�û��ʲô��	
	DWORD	m_dwPlayMethod;	///���ŵķ�����û��ʲô��

	float m_fAngle; //��ͷ��ת�Ƕ�¼���� 

    DWORD m_dwLastKeyFrameTime;	///���ڼ�¼�������Ⱥͼ���֡��ʱ���֡��Key�ؼ��֣�ʵ����û��������¼���ȣ�ֻ��ÿ�μӸ�100����
    
	int		m_nRenderCount;			///��¼������Ⱦ�Ĵ���
	bool	m_bIsPlaying;			///��ʾ������ʼ��
	bool	m_bIsPaused;			///��ʾ�����Ƿ�����ͣ״̬
	bool	m_bLoopWhilePlaying;	///�Ƿ�ѭ������
    DWORD	m_dwAnimationStartTime;	///��¼������ʼ��ʱ��
    DWORD	m_dwAnimationPauseTime;
    DWORD	m_dwAnimationPauseStartTime;
	TypeKeyContainer m_mapCameraKeyFrame;	///���ڷ��ùؼ�֡��map

	bool	m_bIsTangentComputed;	///��ʾ�Ƿ�Ҫ�ټ���һ������

	bool	m_bIsKeyEditable;		///��ʾ�Ƿ���Ա༭�ؼ�֡��������ԵĻ��������RenderEntity�б���
	
	/*KG3DSceneEntityList* m_plistRender;	///�ⲿ��Entity�б��ָ��
	KG3DSceneEntityList* m_plistSelectable;	///�ⲿ��Entityѡ���б��ָ��
	KG3DSceneEntityList* m_plistSelected;*/
	//<�ؼ�֡ѡ��>
public:
	struct CameraKeyFrameForSelection;
public:
	//typedef KG3DSceneEntityDummyExecutor<KG3DSceneEntityDummy>	TypeCamKeyForSelBase;	///���ĵ��¼���Dummy������
	struct CameraKeyFrameForSelection// : public TypeCamKeyForSelBase	//���ǵ������е�ĳЩ����
	{
		DWORD m_dwType;
		//const size_t m_uIndex;	//vec�е�����������ѡ��
		DWORD m_dwTime;
		D3DXVECTOR3 m_vVecPos;
		KG3DSceneCameraAnimation* m_pAnimation;

		//////////////////////////////////////////////////////////////////////////
		
		/*CameraKeyFrameForSelection(const CameraKeyFrameForSelection& Other)
			:m_dwType(Other.m_dwType)
		{
			memcpy_s(this, sizeof(CameraKeyFrameForSelection), &Other, sizeof(CameraKeyFrameForSelection));			
		}*/
		/*CameraKeyFrameForSelection& operator=(const CameraKeyFrameForSelection& Other)
		{
			_ASSERTE(m_dwType == Other.m_dwType);
			memcpy_s(this, sizeof(CameraKeyFrameForSelection), &Other, sizeof(CameraKeyFrameForSelection));	
			return *this;
		};*/
		/*HRESULT GetCenter(LPVOID, D3DXVECTOR3* pvCenter)
		{
			if (pvCenter)
			{
				*pvCenter = m_vVecPos;
				return S_OK;
			}
			return E_FAIL;
		}
		HRESULT GetBBox(LPVOID, AABBOX* pOutBox)
		{
			if (pOutBox)
			{
				static D3DXVECTOR3	stSize(5, 5, 5);
				*pOutBox = AABBOX(m_vVecPos+stSize, m_vVecPos-stSize);
				return S_OK;
			}
			return E_FAIL;
		}
		HRESULT RenderOption(LPVOID, DWORD_PTR dwOption){return E_FAIL;}
		HRESULT RenderSelectMark(LPVOID, DWORD dwMask);
		HRESULT OnSelect(LPVOID){_ASSERTE(m_pAnimation); m_pAnimation->OnKeyFrameSelected(this); return S_OK;}
		HRESULT SetTranslation(LPVOID, D3DXVECTOR3* pVecPos){_ASSERTE(m_pAnimation); m_pAnimation->OnKeyFrameTranslate(this, pVecPos);return S_OK;}
		HRESULT SetRotation(LPVOID, D3DXQUATERNION* pqRot){_ASSERTE(m_pAnimation); if(m_dwType == emCameraKeyFrameForSelection_Center)m_pAnimation->OnKeyFrameRotation(this, pqRot);return S_OK;}
		HRESULT GetTranslation(LPVOID lpObj, D3DXVECTOR3* pvPos){_ASSERTE(pvPos); *pvPos = m_vVecPos;return S_OK;}*/
	};
private:
	//static KG3DSceneEntityDummyExecutor<KG3DSceneEntityDummy> m_stKG3DSceneEntityDummyExecutor;
	friend CameraKeyFrameForSelection;
	//size_t m_uSelectedKeyIndex;
	DWORD m_dwSelectedKeyTime;
	typedef vector<CameraKeyFrameForSelection> TypeVecKeyForSelection;
	TypeVecKeyForSelection m_vecKeyForSelection;
	KG3DSCAniSelKeyChangeCallback	m_CallbackKeySelChange;

	D3DXVECTOR3 m_vecPosCenter;

	std::vector<KG3DRepresentObjectDummy*>	m_vecObjectsInTheScene;
public:
	VOID OnKeyFrameSelected(CameraKeyFrameForSelection* pKeyForSelection)
	{
		_ASSERTE(pKeyForSelection);
		//m_uSelectedKeyIndex = pKeyForSelection->m_uIndex;
		m_dwSelectedKeyTime = pKeyForSelection->m_dwTime;
		if (m_CallbackKeySelChange)
		{
			(*m_CallbackKeySelChange)(m_dwSelectedKeyTime);
		}
	}
	VOID OnKeyFrameTranslate(CameraKeyFrameForSelection* pKeyForSelection, const D3DXVECTOR3* pVecPos)
	{
		_ASSERTE(pKeyForSelection && pVecPos);
		if (pKeyForSelection->m_dwType == emCameraKeyFrameForSelection_Center)///�����ƶ�
		{
			D3DXVECTOR3 vBios = *pVecPos - m_vecPosCenter;
			for (TypeKeyContainer::iterator it = m_mapCameraKeyFrame.begin(); it != m_mapCameraKeyFrame.end(); it++)
			{
				CameraKeyFrame& Key = it->second;
				Key.vLookAt += vBios;
				Key.vLookAtTangent += vBios;
				Key.vPosition += vBios;
				Key.vPositionTangent += vBios;
			}
			for (size_t i = 0; i < m_vecKeyForSelection.size(); i++)
			{
				CameraKeyFrameForSelection& Key = m_vecKeyForSelection[i];
				Key.m_vVecPos += vBios;
			}
			m_vecPosCenter = *pVecPos;
			
		}
		else
		{
			pKeyForSelection->m_vVecPos = *pVecPos;
			TypeKeyContainer::iterator it = m_mapCameraKeyFrame.find(pKeyForSelection->m_dwTime);
			if (it != m_mapCameraKeyFrame.end())
			{
				CameraKeyFrame& Key = it->second;
				pKeyForSelection->m_vVecPos = *pVecPos;
				switch(pKeyForSelection->m_dwType)
				{
				case emCameraKeyFrameForSelection_PositionTan:
					Key.vPositionTangent = *pVecPos;
					break;
				case emCameraKeyFrameForSelection_LookAtTan:
					Key.vLookAtTangent = *pVecPos;
					break;
				case emCameraKeyFrameForSelection_Position:
					Key.vPosition = *pVecPos;
					break;
				case emCameraKeyFrameForSelection_LookAt:
					Key.vLookAt = *pVecPos;
					break;
				default:
					_ASSERTE(NULL);
					break;
				}
			}
			if (m_CallbackKeySelChange)
			{
				(*m_CallbackKeySelChange)(m_dwSelectedKeyTime);
			}
		}
		return;
	}
	virtual void __stdcall OnReceiveMessage(const KG3DMessage& Message, IKG3DMessageSenderBase* pSender );
	//</�ؼ�֡ѡ��>
	

public:
	int GetRenderCount() const { return m_nRenderCount; }
	void SetRenderCount(int val) { m_nRenderCount = val; }
    DWORD GetAnimationStartTime() const { return m_dwAnimationStartTime; }
    void SetAnimationStartTime(DWORD dwStartTime) { m_dwAnimationStartTime = dwStartTime; }	
    DWORD   GetAnimationTotalTime();
    HRESULT PausePlay(IKG3DSceneCameraKeyFrame* pPauseKey, BOOL bBenchMark = FALSE);
protected:	///�������������������һ��EnableEditCamAniKeyFrame����
    HRESULT Sync(DWORD dwNowTime, BOOL bBenchMark = FALSE);		///ͬ����fTimePassed����UpdateCamera��ʱ������������
	HRESULT AddDataToSceneEditing();	///�ѹؼ�֡���뵽������KG3DRenderEntity�б��С�
	HRESULT RemoveDataFromSceneEditing();	///�Ƴ������м����KG3DRenderEntity�ؼ�֡��
public:
	virtual float GetPlaySpeed()
	{
		return m_fPlaySpeed;
	}
	virtual HRESULT SetPlaySpeed(float fSpeed)
	{
		m_fPlaySpeed = fSpeed;
		return S_OK;
	}

	virtual float GetPlayAngle()
	{
		return m_fAngle;
	}
	virtual HRESULT SetPlayAngle(float fAngle)
	{
		m_fAngle = fAngle;
		return S_OK;
	}

	const std::string& GetName();
	virtual HRESULT ComputeTangent();		///�������ߣ��洢���ؼ�֡��

	HRESULT AddKeyFrame( CameraKeyFrame* pKey, DWORD dwKeyFrameTime = 0 );	///��dwKeyFrameTimeΪʱ������Ѵ��ڵĹؼ�֡�����fKeyFrameTime��Ĭ��ֵ�Ļ����Զ�����ʱ��
	
    HRESULT AddKeyFrame( KG3DCamera* pCamera,DWORD dwKeyFrameTime = 0, CameraKeyFrame* pKeyOut = NULL );		///��dwKeyFrameTimeΪʱ�䣬��ȡpCamera��λ�úͽ���λ�ô����ؼ�֡�����ѽ���������pKeyOut�����fKeyFrameTime��NULLֵ�Ļ����Զ�����ʱ��
	
	virtual HRESULT AddKeyFrame();	///�Զ��õ�ǰʱ���Camera���KeyFrame	

	virtual HRESULT AddKeyFrameToHeadFramePosition(); ///��һ���ؼ�֡���Ժ͵�һ���ؼ�֡����
	
	virtual HRESULT RemoveKeyFrame( DWORD dwKeyFrameTime);	///�Ƴ�dwKeyFrameTimeʱ��Ĺؼ�֡�����ȡ0�Ļ���ɾ�������һ֡

	HRESULT RenderCameraKeyFrame(DWORD dwOption);	///��Ⱦ�ؼ�֡������
	
    HRESULT UpdateCamera( KG3DCamera* pCamera);	//ˢ��Camera��λ�á�ʹ��֮ǰ���뱣֤ʹ����StartPlay������������Ч��Ȼ�󲥷Ž�����ʱ��������StopPlay������
	HRESULT GetData(CameraKeyFrame* pValue, DWORD dwTime);	///��dwTimeΪʱ��ĵ�һ���ؼ�֡�ṹ�壬ʵ�����������ؼ�֮֡��Ĳ�ֵ��
    BOOL    IsFinished(DWORD dwNowTime);
    HRESULT UpdateCameraByTime(KG3DCamera* pCamera);

	HRESULT SaveToFile(FILE* pFile);	///����CameraAni�ļ�
	HRESULT LoadFromFile(IFile* pFile);	///��ȡCameraAni�ļ�

	virtual HRESULT Clear();	///���map�е����йؼ�֡

	virtual HRESULT StartPlay(BOOL bLoop);
	
	virtual HRESULT StopPlay();		///�趨��ʼ���ŵı��
	virtual HRESULT PausePlay(IKG3DSceneCameraKeyFrame* pPauseKey, BOOL* bIsPaused);
	

    virtual HRESULT GetName(LPCTSTR *ppcszRetName);	///�õ�����
	virtual HRESULT GetName(LPTSTR ppcszRetName, size_t uBufferSize);//�����Ǹ��ɵ�û������
    virtual HRESULT SetName(LPCTSTR pcszName);

	virtual HRESULT GetPlayState(BOOL *pbRetIsPlaying);	///����Ƿ��ڲ��Ź�����

	
	virtual HRESULT IsKeyFrameEditable(BOOL *pbRetIsKeyFreeEditable);	///����m_bIsEditable

	virtual HRESULT GetKeyFrameCount(DWORD *pdwKeyFrameCount);		///�õ��ؼ�֡����

	virtual HRESULT GetCurSelectedKeyTime(DWORD *pdwRetSelectedKeyTime, DWORD *pdwRetPreKeyTime, DWORD *pdwRetNextKeyTime);
	virtual HRESULT GetCurSelectedKey(IKG3DSceneCameraKeyFrameEx* pKey);
	virtual HRESULT SetCurSelectedKey(const IKG3DSceneCameraKeyFrameEx* pKey);
	virtual HRESULT SetKeySelChangeCallback(KG3DSCAniSelKeyChangeCallback pNewCallback, KG3DSCAniSelKeyChangeCallback* ppOldCallback);
	virtual HRESULT CreateSpecialTrack(DWORD dwType, DWORD_PTR dwParam);

	virtual HRESULT EnableEditCamAniKeyFrame(BOOL bEnable);

	KG3DSceneCameraAnimation(IEKG3DScene& Scene);
	virtual ~KG3DSceneCameraAnimation();	///���map�����еĹؼ�֡��

private:
	void operator=( KG3DSceneCameraAnimation &source);	///�Ͻ�ʹ�ÿ��������

private:
	IEKG3DScene& m_SceneRef;

};
/************************************************************************/
/* KG3DSceneCameraMovement������ʼ
/************************************************************************/

class KG3DSceneCameraMovement : public IEKG3DSceneCameraMovement
{	
public:
	KG3DSceneCameraMovement(const IEKG3DScene& Scene);
	virtual ~KG3DSceneCameraMovement(void);
private:
	void operator=( KG3DSceneCameraMovement &source);	///�Ͻ�ʹ�ÿ��������	
public:
	inline KG3DSceneCameraAnimation* GetCurCamAnimation();
	virtual HRESULT GetCurCamAnimation(IKG3DSceneCameraAnimation **ppiRetCameraAnimation);
	virtual HRESULT GetCurCamAnimation(IEKG3DSceneCameraAnimation **ppiRetCameraAnimation);	///���ص�ǰ�����������ָ�룬�������Чָ��Ļ����ᱣ֤���ٷ���һ���µ�Animation����Ϊ����������Ա�֤���������Чָ�룬���Գ����ٶ�Ҫ��ߵ�֡ѭ������֮�⣬���Բ�Ҫֱ�Ӱ���������ķ���ֵ����ʹ�á�ָ��ֵ�Ĵ��ݷ��ز���̫Ӱ���ٶ�
	virtual HRESULT	SetCurCamAnimation( int nIndex );	///�趨��ǰ���������
	virtual HRESULT	NewOneCamAnimation();		///�½�һ����
	HRESULT	RemoveOneCamAnimation( int nIndex = -1 );	///�Ƴ�index��������������ϲ����Ļ����������һ�������������ǰ�Ķ���ָ�����Ҫɾ�����Ǹ��Ļ���ָ���Զ���ǰ�ƶ�����Ϊ��ɾ��֮ǰ����ǰ����ָ������ƶ���λ�ã����Բ�������ʵʱ���Ƶõ���ָ��ʧЧ�����
	virtual HRESULT RemoveCurCamAnimation();		///�Ƴ���ǰ�������������ǰ����ָ����Զ���ǰ�ƶ�����Ϊ��ɾ��֮ǰ����ǰ����ָ������ƶ���λ�ã����Բ�������ʵʱ���Ƶõ���ָ��ʧЧ�����
	int FindCamAniByName( LPCTSTR pstrCamAniName, int nFinded = 1 );	//�������ҵ�vector������,nFinded��ʾ�ڼ����ҵ��ģ�Ĭ����1
	virtual HRESULT FindCamAniByName(LPCTSTR pstrCamAniName, int nFinded, int* nRetIndex);///nFinded��1��ʾҪ��һ���ҵ��ģ�һ�㴫1
	virtual HRESULT SetCurCamAnimationByName(LPCTSTR pstrCamAniName, int nFinded, IKG3DSceneCameraAnimation **ppiRetCameraAnimation);///����ָ��Ͳ�����CamAni
	virtual HRESULT Clear();		///�Ƴ����е�CameraAnimation
	virtual HRESULT GetAnimationCount(DWORD *pdwRetAnimationCount);		///�õ��ڲ������������������0�Ļ���֪���ڲ�Ϊ��
	virtual HRESULT	LetCurCamAniPosMoveUp();	///�ѵ�ǰ�����ڱ�������
	virtual HRESULT LetCurCamAniPosMoveDown();	///�ѵ�ǰ�����ڱ�������
	virtual HRESULT	SetCurCamAniName( LPCTSTR pCamAniName );		///����������
	virtual HRESULT GetCurCamAniIndex(DWORD *pdwRetAnimationIndex);		///�õ���ǰ�������б��е�λ��
	HRESULT SaveToFile(LPSTR pFileName);
	virtual HRESULT LoadFromFile(LPSTR pFileName);

private:
	KG3DSceneCameraAnimation* m_pCurCameraAnimation;
	vector<KG3DSceneCameraAnimation*> m_vecCameraAnimationTable;
	int m_nNewCamAniTimes;	///���ڸ�Animation����
	IEKG3DScene&	m_SceneRef;
};
