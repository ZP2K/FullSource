////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DLocalRotationCoord.h
//  Version     : 1.0
//  Creator     : Chiyer (Zhao XinYu)
//  Create Date : 2006-11-9 14:14:48
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DLOCALROTATIONCOORD_H_
#define _INCLUDE_KG3DLOCALROTATIONCOORD_H_

#include "KG3DSceneEntity.h"

#define ROTATION_COORD_MESH_PATH TEXT("\\Data\\editor\\��ת\\local_rotation_coord.x")
class KG3DScene;
class KG3DRotationCoordinateOld
{
//private :
//    KG3DRotationCoordinateOld();

public :
	KG3DRotationCoordinateOld(KG3DScene& scene);
    virtual ~KG3DRotationCoordinateOld();

    //static KG3DRotationCoordinateOld* GetCoordinate();

    STDMETHOD(FrameMove)();
    STDMETHOD(Render)();

    //HRESULT AttachEntity(KG3DScene* pScene, std::list<KG3DSceneEntity> listEntitys);
    //HRESULT AttachEntity(KG3DScene* pScene, KG3DSceneEntity Entity);
    //HRESULT ReleaseAttach();
    HRESULT RotateBegin();
    HRESULT RotateEnd();
    HRESULT SetMode(DWORD dwMode);
    HRESULT Zoom(FLOAT fScanl);
    
	FLOAT	GetRotationAngleX(){return m_fAngelX;}
	FLOAT	GetRotationAngleY(){return m_fAngelY;}
	FLOAT	GetRotationAngleZ(){return m_fAngelZ;}

    //const EntityList& GetEntityList() { return m_EntityList; }

#if defined(DEBUG) | defined(_DEBUG)
	VOID	TestSelectCoord(DWORD dwCoord){m_dwCurrSelCoord = dwCoord;}
#endif

private :
	float   GetScalingFactor();
	DWORD   GetSelCoord(const D3DXVECTOR3& vSrc, const D3DXVECTOR3& vDir);

    KG3DScene*  m_pAttachScene;
    //EntityList  m_EntityList;
    LPD3DXMESH  m_pCoordMesh;
    DWORD       m_dwNumMatrials;
    DWORD       m_dwMode;			//�������껹�Ǿֲ�����

    DWORD       m_dwCurrSelCoord;	//ѡ������ĸ�ȦȦ

    D3DXVECTOR3 m_currSelNormal;	//��ǰѡ���ȦȦ��Ӧ���Ǹ�Normal
    D3DXPLANE   m_currSelPane;		//��ǰѡ�е�ȦȦ��Ӧ���Ǹ���
    D3DXVECTOR3 m_vPrevCross;		//ǰһ������
    D3DXVECTOR3 m_vBeginCross;		//��ʼ����
    D3DXMATRIX  m_matCoord;			//��������
    //D3DXMATRIX  m_matCoordPane;
    D3DXMATRIX  m_matEntityWorld;	//EntityList�ĺ�������

    FLOAT       m_fZoom;			//������������ȦȦ�ı���




    INT         m_nMoveFlag;		//�Ƿ���ת��Begin��
    // editer
    FLOAT       m_fAngelX;	
    FLOAT       m_fAngelY;
    FLOAT       m_fAngelZ;
};

#include "KG3DBaseCoord.h"

//struct KG3DRotationCoordinate : public KG3DBaseCoord
//{
//	virtual	ULONG	GetCurState() = 0;
//	virtual HRESULT SetPosition(const D3DXVECTOR3& Pos) = 0;
//	virtual HRESULT	Zoom(FLOAT zoomFactor) = 0;//zoomFactor��ӵ�Scaleϵ�����棬�����ǳ�
//	virtual D3DXVECTOR3 GetRotationAngle() = 0;//����XYZ��Ӧ�����ת
//	virtual ~KG3DRotationCoordinate() = 0{}
//};
KG3DBaseCoord* BuildKG3DRotationCoordinateSolid(LPCTSTR, DWORD_PTR pScene, DWORD_PTR);


#endif //_INCLUDE_KG3DLOCALROTATIONCOORD_H_
