#pragma once

class KG3DTerrainRegionInterface
{
public:
	KG3DTerrainRegionInterface(void);
	virtual ~KG3DTerrainRegionInterface(void);
};


class KG3DTerrainRegionDataBase
{
public:
	DWORD m_dwDetailBlendWidth;//�ر�ϸ�ڻ����Ϣ�Ŀ�ȣ�Ĭ��32*32.����16*16,8*8,4*4;
	BYTE* m_lpDetailBlend;
	unsigned long long m_lTextureKey;

	KG3DTerrainRegionDataBase();
	virtual ~KG3DTerrainRegionDataBase();

	HRESULT CreateDetailBlendData(DWORD dwWidth);

	virtual HRESULT Init();
	virtual HRESULT UnInit();
};