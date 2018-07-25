#pragma once

class KG3DModel;
class KG3DSelector
{
public:
	KG3DSelector();
	~KG3DSelector();

	HRESULT Init();
	HRESULT OnRender();
	void UnInit();
	void OnLostDevice();
	void OnResetDevice();
	void Push(KG3DModel *pModel);
	void ResetSelection();
	KG3DModel* GetSelected();
	POINT m_MousePoint;
private:
	void RenderModelList();
	LPDIRECT3DTEXTURE9 m_pStencilRT;//����ģ���õ�ģ��
	LPDIRECT3DSURFACE9 m_pStencilDepth;//����ģ���õ�ģ�����
	LPDIRECT3DSURFACE9 m_pRT;//��СΪ1���׵����λ��ģ����ɫ
	LPDIRECT3DTEXTURE9 m_pResult;//����ȡ����ɫ��Surface
	LPDIRECT3DSURFACE9 m_pDepthSave;//�����Depth
	LPDIRECT3DSURFACE9 m_pRTSave;//�����RT
	D3DVIEWPORT9       m_vpSave;//�����VP
	int m_nWidth;//Stencil Target�Ŀ��
	int m_nHeight;//Stencil Target�ĸ߶�
	int m_nScale;//Stencil Target������ϵ��
	DWORD m_dwIndex;//��ȾStencilʱʹ�õ�index
	std::vector<KG3DModel*> m_vecModels;//��Ҫ����ȷ�жϵ�ģ���б�
	BOOL m_bSelected;
	KG3DModel *m_pSelectedModel;
};

extern KG3DSelector g_Selector;
