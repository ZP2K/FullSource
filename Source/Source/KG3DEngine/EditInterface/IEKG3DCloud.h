////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEKG3DCloud.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-11-21 10:55:25
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _INCLUDE_IEKG3DCLOUD_H_
#define _INCLUDE_IEKG3DCLOUD_H_

////////////////////////////////////////////////////////////////////////////////
typedef enum _KG3DCLOUD_MDO
{
    KG3DCLOUD_MDO_DEFAULT = 0x00,		//ʲôҲ����
    KG3DCLOUD_MDO_MESH = 1 << 0,		//��Mesh
    KG3DCLOUD_MDO_MOVEMENT = 1 << 1,
    KG3DCLOUD_MDO_COLORTEX = 1 << 2,	//���»����ɳ������ɵ���ͼ
    KG3DCLOUD_MDO_MASKTEX = 1 << 3,		//����������ͼ
	KG3DCLOUD_MDO_VISIBILITY = 1 << 4,	//������͸����
    KG3DCLOUD_MDO_ALPHAREF   = 1 << 5,
	KG3DCLOUD_MDO_COLOR = 1 << 6,		//����ɫ������
	KG3DCLOUD_MDO_ENABLE_CONTRL_COLOR = 1 << 7,
    KG3DCLOUD_MDO_TEXFACTOR = 1 << 8,
    KG3DCLOUD_MDO_ALL = 0xffff,			//�޸����е�
} KG3D3DCLOUD_MDO;
////////////////////////////////////////////////////////////////////////////////
typedef struct _KG3DCLOUD_DESCSTRUCT
{
    DWORD	m_dwMidificationOption;
    ///��MESH����
    FLOAT	m_fHeight;
    FLOAT	m_fSize;
    INT		m_nUVRepeat;
    ///��MOVEMENT����
    FLOAT	m_fSpeed;
    FLOAT	m_fAngel;
    ///��COLORTEX����
    LPTSTR	m_tszColorMapPath;
    ///��MASKTEX����
    LPTSTR	m_tszMaskMapPath;
	
	///��KG3DCLOUD_MDO_VISIBILITY����
	FLOAT	m_fVisibility;

    FLOAT   m_fAlphaRef;
	
	///��COLOR����
	DWORD	m_diffuseColor;
	DWORD	m_ambientColor;

	BOOL	m_bEnableControlColor;

    FLOAT   m_fTexFector;

    _KG3DCLOUD_DESCSTRUCT()
    {
        memset(this, 0, sizeof(_KG3DCLOUD_DESCSTRUCT));
    }
}KG3DCLOUD_DESCSTRUCT, *LPKG3DCLOUD_DESCSTRUCT;

struct IEKG3DCloud 
{
    virtual HRESULT	RequestModification( const KG3DCLOUD_DESCSTRUCT* pStruct ) = 0;///������pStruct�е�ModificationOption�趨��ʲô����
    virtual HRESULT	GetStruct( KG3DCLOUD_DESCSTRUCT* pResultStruct ) = 0;
};

#endif //_INCLUDE_IEKG3DCLOUD_H_
