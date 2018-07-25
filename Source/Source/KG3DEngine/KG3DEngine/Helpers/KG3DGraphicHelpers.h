////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DGraphicHelpers.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-7-24 14:39:53
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DGRAPHICHELPERS_H_
#define _INCLUDE_KG3DGRAPHICHELPERS_H_

////////////////////////////////////////////////////////////////////////////////
namespace KG3D_CUSTOM_HELPERS
{
	HRESULT FillTexture(LPDIRECT3DSURFACE9 pSuface, CONST RECT* pRect, D3DCOLOR color);//D3D��ColorFill��ʹ�����ƣ��������ͨ�ã�pRect��Fill�ķ�Χ�����Դ�NULL��Fill����Texture����RenderTargetֻ֧��D3DFMT_A8R8G8B8��D3DFMT_X8R8G8B8
	HRESULT FillTexture(LPDIRECT3DTEXTURE9 pTex, CONST RECT* pRect, D3DCOLOR color);//ע��ʹ�÷�ʽ��Tex�����㣬Fill��Ҫ����GenerateMipSubLevels�������������GenerateMipSubLevels��ֱ�ӵõ�Surface��Ȼ���������İ汾
	HRESULT GenerateMipSubLevels(LPDIRECT3DTEXTURE9 pTex);	//���ĳЩ��ͼ����D3DUSAGE_AUTOGENMIPMAP���ܵ���GenerateMipSubLevels������
};


#endif //_INCLUDE_KG3DGRAPHICHELPERS_H_
