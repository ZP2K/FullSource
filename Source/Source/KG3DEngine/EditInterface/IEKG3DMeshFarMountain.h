////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEKG3DMeshFarMountain.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-11-15 10:32:27
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IEKG3DMESHFARMOUNTAIN_H_
#define _INCLUDE_IEKG3DMESHFARMOUNTAIN_H_

////////////////////////////////////////////////////////////////////////////////
typedef enum _IEKG3DMESHFARMOUNTAIN_MDO
{
	KG3DMESHFARMOUNTAIN_MDO_DEFAULT = 0x00,		///ʲôҲ����
	KG3DMESHFARMOUNTAIN_MDO_MESH = 0x01,		///��Mesh
	KG3DMESHFARMOUNTAIN_MDO_TEX = 0x02,			///���»����ɳ������ɵ���ͼ
	KG3DMESHFARMOUNTAIN_MDO_TEXPATH = 0x04,		///����������ͼ
	KG3DMESHFARMOUNTAIN_MDO_SYNWITHFOG = 0x08,	//������
	KG3DMESHFARMOUNTAIN_MDO_ENABLETEXCOLOR = 0x10,
	KG3DMESHFARMOUNTAIN_MDO_SPEED = 0x20,		///�ٶ�
    KG3DMESHFARMOUNTAIN_MDO_TEXFACTOR = 0x40,   ///��ͼ���� 
	KG3DMESHFARMOUNTAIN_MDO_ALL = 0xff,			///�޸����е�
} IEKG3DMESHFARMOUNTAIN_MDO;
////////////////////////////////////////////////////////////////////////////////
typedef struct _KG3DMESHFARMOUNTAIN_DESCSTRUCT{
	///�޸�ѡ��
	DWORD	m_dwModificationOption;
	///��״���
	float m_fRadius;
	float m_fHeight;
	float m_fBotmZPos;
	float	m_fZRotation;
	float	m_fTexCordUTiles;
	
	///��������ͼ���		
	//D3DCOLOR m_d3dc_Color;			///��ͼ����ɫ
	//bool m_bEnableSynWithFogColor;	///����ͬ����ɫ��������ֱ��ʹ��
	bool m_bEnableTexColor;			///��ΪԶɽʹ�õ�ʱ�򣬽���ʹ����ɫ�͹��ˣ�����Ҫʹ�õ���ͼ����ɫ
	D3DCOLOR m_d3dc_FogColor;		///�����ɫ
	float	m_fFogBlendFactor;		///����ɫ����ͼɫ�Ļ��ϵ��
	float	m_fBotmBlendHeight;			///ȡʵ�ʸ߶ȣ�Ȼ����ͼ��Blend�ɱ����ó�
	
	float	m_fSpeed;				///��ת�ٶ�

    float   m_fTexFactor;

	///��ͼ��·��ָ�룬�����Ϊ0�Ļ�����Ҫ�޸���ͼ
	TCHAR	m_strTextureFullPath[MAX_PATH];

	_KG3DMESHFARMOUNTAIN_DESCSTRUCT()///Ĭ�Ϲ��캯��
	{
		memset(this, 0, sizeof(_KG3DMESHFARMOUNTAIN_DESCSTRUCT));
	}	

}KG3DMESHFARMOUNTAIN_DESCSTRUCT, *LPKG3DMESHFARMOUNTAIN_DESCSTRUCT;

////////////////////////////////////////////////////////////////////////////////
struct IEKG3DMeshFarMountain{
	virtual HRESULT GetName( LPTSTR szName, UINT uBufferCount ) = 0;
	virtual HRESULT SetName( LPTSTR szName ) = 0;
	virtual HRESULT	RequestModification( const KG3DMESHFARMOUNTAIN_DESCSTRUCT* pStruct ) = 0;///������pStruct�е�ModificationOption�趨��ʲô����
	virtual HRESULT	GetStruct( KG3DMESHFARMOUNTAIN_DESCSTRUCT* pResultStruct ) = 0;
	//virtual HRESULT IsEnableSynWithFogColor( BOOL* pBool ) = 0;	
};


#endif //_INCLUDE_IEKG3DMESHFARMOUNTAIN_H_
