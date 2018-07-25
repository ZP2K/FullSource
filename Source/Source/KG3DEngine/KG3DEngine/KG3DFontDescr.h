////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DFontDescr.h
//  Version     : 1.0
//  Creator     : dengchao
//  Create Date : 2006-02-13 16:43:32
//  Comment     : font def
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DFONTDESCR_H_
#define _INCLUDE_KG3DFONTDESCR_H_


UINT const KG3DFONT_DEFAULT_POINT = 16;
UINT const KG3DFONT_DEFAULT_PADDING = 2;	// ���֧�� 2x2 �ĳı�

// font glyph
// �ο������� 1998 �����Ʒ֮������ͳ�����Ͽ⣬
// �����Ͽ�ͳ�� 1579771 ���ִʣ����г����ֹ��� 5063 ����
// ��������������ߵ�ǰ 80% ֻ�� 641 ���֣�
// ���ǵ�ǰհ�ԣ���̬��( ��Щ���ҵ����Ա�̬��˵) �ȵȣ�
// ����ʹ�� 1024 ��Ϊ��󻺳��С��

UINT const KG3DFONT_GLYPH_NUMS = 1024;
UINT const KG3DFONT_GLYPH_LIMIT	= 0xFFFF;


struct KG3DFontTLVertex
{
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };

	FLOAT x, y, z;
	D3DCOLOR diffuse;
	float u, v;
};


#endif //_INCLUDE_KG3DFONTDESCR_H_