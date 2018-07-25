#pragma once


class KG3DTexture;

class KG3DModelLightMap
{
public:
	KG3DTexture* m_lpLightMapTexture;//����ͼ
	BOOL         m_bUseShareTexture;//�Ƿ�ʹ�ù���Ĺ���ͼ
	D3DXVECTOR4  m_vUVScale;//�������ͼ��UV����ϵ��

	LPDIRECT3DTEXTURE9 m_lpLightMapForCompute;//���ڼ���Ĺ���ͼ
	LPDIRECT3DTEXTURE9 m_lpLightMapForCompute2;//���ڼ���Ĺ���ͼ

	BOOL         m_bComputing;//�Ƿ��ڼ���ģʽ
	int          m_nLightmapWidth;//����ͼ��� 
	BOOL         m_bUseSecond;

	int          m_nKey;//����ͼ���
public:
	HRESULT EnableUseLightmap2(BOOL bEnable,float ratioA,float ratioB);
	HRESULT Init(int nWidth,BOOL bComputing,LPSTR pFileName);
	HRESULT UnInit();

	LPDIRECT3DTEXTURE9 GetLightMap();

	KG3DModelLightMap(void);
	~KG3DModelLightMap(void);
};
