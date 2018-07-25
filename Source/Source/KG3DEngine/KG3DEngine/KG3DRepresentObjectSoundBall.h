#pragma once
#include "kg3drepresentobject.h"

struct IKG3DSoundShell;
class KG3DSceneOutDoorSpaceMgr;
class KG3DSoundBallDataTable : public IKG3DSoundBallTable
{
public:
	void UnInit();

	void Release(UINT uID);

	HRESULT LoadSoundBallData(SoundBallData** ppData,
		UINT &uHashID,
		LPCTSTR strFileName);
	
	HRESULT FindSoundBallData(SoundBallData** ppData,
		UINT uHashID);

	virtual HRESULT LoadFromFile(LPCTSTR strFileName,
		SoundBallData *pData);

	virtual HRESULT SaveToFile(LPCTSTR strFileName,
		SoundBallData *pData);

	virtual void Refresh(LPCSTR strFileName);

private:
	const static int s_nVersion = 1;
	map<UINT, SoundBallData*>m_mapSoundBallData;

	// Inherited via IKG3DSoundBallTable
	virtual HRESULT LoadFromFile(LPCTSTR strFileName, SoundDataBase * pData) override;
	virtual HRESULT SaveToFile(LPCTSTR strFileName, SoundDataBase * pData) override;
};

struct SoundBallConfigInfo
{
	float m_f3DSoundMinDistance;
	float m_fSoundBallCheckRange;
	float m_fMaxListenRange;
	float m_fBoundingBoxSize;
	bool m_bRenderSoundBallRange;
	D3DXCOLOR m_crSoundBallColor;
	D3DXCOLOR m_crSoundBallRangeColor;
	SoundBallConfigInfo();
};


class KG3DRepresentObjectSoundBall :
	public KG3DRepresentObject,
	public IEKG3DRepresentObjectSoundBall
{
public:
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	SoundBallData* m_lpSoundBallData;
	string m_szSoundBallDataFileName;
	UINT m_uFileHash;
	int m_nCurrentSound;//��ǰ���ŵ�������ţ������m_lpSoundBallData
	DWORD m_dwSoundID;//��ǰ���ŵ��������
	DWORD m_dwLastCheckTime;//��һ�μ���Ƿ񲥷ŵ�ʱ��
	BOOL m_bHalt;//��֯���̸߳���soundball
	BOOL m_bRenderRange;
	HRESULT LoadFromSoundBallDataFile(LPCTSTR pFileName);
	virtual HRESULT GetName(LPTSTR pName, size_t BufferSize);
	virtual HRESULT SaveToInformationBuffer(KG3DMemoryFile* pFile);
	virtual HRESULT LoadFromInformationBuffer(const KG3DMemoryFile* pFile,DWORD dwOption);

	virtual HRESULT ComputeDynamicBBox();
	virtual HRESULT ComputeBBox();
	virtual HRESULT RenderSelectMark(DWORD dwColor /* = 0xFFFF0000 */);

	BOOL IsRayIntersect(D3DXVECTOR3&Inter,D3DXVECTOR3 Origin,D3DXVECTOR3 Direction);
	virtual HRESULT OnSelect();
	virtual HRESULT OnUnSelect();
	virtual HRESULT FrameMove(const D3DXMATRIX& matView);

	virtual HRESULT Render(DWORD dwOption);
	void Render();
	virtual HRESULT Copy(KG3DRepresentObject* pSrcObject);

	virtual HRESULT Load(LPCTSTR lpFileName, DWORD_PTR wParam, DWORD_PTR lParam);

	virtual HRESULT GetAnotherIEInterface(DWORD dwType, VOID** ppAnotherInterface);

	virtual HRESULT UnInit();

	KG3DRepresentObjectSoundBall(void);
	virtual ~KG3DRepresentObjectSoundBall(void);
	HRESULT SetTranslation(const D3DXVECTOR3* pTrans);
	HRESULT SetScaling(const D3DXVECTOR3* pScale);

private:
	float m_fVolume;
	BOOL  m_bPlayFailed;
	HRESULT StopSound();
public:
	//�༭��ʹ�õ�
	virtual void Stop();
	virtual void Start();
};

class KG3DSoundBallRender
{
public:
	KG3DSoundBallRender();
	~KG3DSoundBallRender();
	void Render();
	void PushBack(KG3DRepresentObjectSoundBall *pSoundBall);
private:
	std::vector<KG3DRepresentObjectSoundBall*> m_SoundBalls;
};

extern KG3DSoundBallDataTable g_cSoundBallTable;
extern KG3DSoundBallRender g_cSoundBallRender;
extern SoundBallConfigInfo g_SoundBallConfig;
