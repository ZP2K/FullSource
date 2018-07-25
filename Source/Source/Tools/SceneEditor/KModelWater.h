// KModelWater.h: interface for the KModelWater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KMODELWATER_H__F7683E27_0EAF_44A3_BA10_7848E56260C6__INCLUDED_)
#define AFX_KMODELWATER_H__F7683E27_0EAF_44A3_BA10_7848E56260C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XMesh.h"
#include "Sea.h"
#include "Water.h"

namespace Water
{
class KModelWater : public KModel  
{
public:
	LPD3DXMESH  m_pWaterUp;//ˮ���ϲ�
	LPD3DXMESH  m_pWaterDn;//ˮ���²㣨��ɫ�Ƚ��

	DWORD m_dwNumPoly_X;
	DWORD m_dwNumPoly_Z;
	Sea WSea;         //����
	CWater Water;     //ˮ��
	CRipple Ripple;   //����
	IDirect3DIndexBuffer9  *m_pibIndices;
//	LPD3DXEFFECT           m_pEffect;

	KAnimateTexture*     m_lpBumpSpr;

	LPDIRECT3DTEXTURE9   m_pBumpMapTexture;  //ˮ��bump������
	LPDIRECT3DTEXTURE9     m_pWaterTex;      //ˮ������
	LPDIRECT3DCUBETEXTURE9 m_pSkyCubeTex;    //��պ�����
public:
	HRESULT LoadMesh(LPSTR pFileName);
	HRESULT FrameMove();
	HRESULT Render();
	HRESULT CleanUp();
	HRESULT InitBumpMap();


	KModelWater();
	virtual ~KModelWater();

};
};

#endif // !defined(AFX_KMODELWATER_H__F7683E27_0EAF_44A3_BA10_7848E56260C6__INCLUDED_)
