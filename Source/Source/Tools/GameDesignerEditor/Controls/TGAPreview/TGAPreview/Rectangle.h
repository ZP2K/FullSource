#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "KG3DTextureAnimation.h"

#define CUSTOMVERTEX_FVF (D3DFVF_XYZRHW|D3DFVF_TEX1)  //����ɱ䶥���ʽ�ĺ�
struct CUSTOMVERTEX //����ṹ��
{
	float x,y,z,rhw;  //��������
	float u,v;  //�������������
};

class CRectangle
{
public:
	CRectangle(IDirect3DDevice9* pDevice);
	~CRectangle();

	void FrameMove(LPRECT pRect);
	void Render();
	KG3DTextureAnimation* GetTextureManager() {return &m_textani;}
private:
	bool InitVertexBuffer();
	bool ResetPos(LPRECT pRect);
	KG3DTextureAnimation m_textani;

	IDirect3DVertexBuffer9* m_pVertexBuffer;  //���㻺����
	IDirect3DDevice9* m_pDevice;
};