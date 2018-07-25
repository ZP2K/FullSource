#pragma once
#include "kg3dtexture.h"

class KG3DTextureAnimation : public KG3DTexture
{
public:
	KG3DTextureAnimation();
	virtual ~KG3DTextureAnimation();

	virtual HRESULT Init();
	virtual HRESULT UnInit();

    virtual HRESULT OnLostDevice();
	virtual HRESULT OnResetDevice();

	virtual HRESULT CheckLoadState();

    virtual HRESULT Update(unsigned uMethod, DWORD dwTime);//������ͼ,�Զ�����ͼ������
	virtual HRESULT Update(int nFrame);//������ͼ

	virtual HRESULT LoadFromFile(const char cszFileName[], unsigned uFileNameHash, unsigned uOption);

private:
	HRESULT Clean();

private:
	LPDIRECT3DTEXTURE9* m_lpBackTextures;
	DWORD m_dwNumFrame;
	float m_fFrameLength;
};
