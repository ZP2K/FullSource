////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEKG3DSkySystem.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-11-15 14:18:22
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IEKG3DSKYSYSTEM_H_
#define _INCLUDE_IEKG3DSKYSYSTEM_H_

#include "IEKG3DMeshFarMountain.h"
#include "IEKG3DCloud.h"

////////////////////////////////////////////////////////////////////////////////
interface IEKG3DMeshSkyBox;

struct IEKG3DSkySystem
{
	///<��յĹ�����>
	virtual HRESULT GetSkyBox(IEKG3DMeshSkyBox** ppiMeshSkyBox) = 0;
	virtual HRESULT AddSkyBox(IEKG3DMeshSkyBox** ppiMeshSkyBox) = 0;
	virtual HRESULT DelSkyBox() = 0;
	///</��յĹ�����>
    ///<Զɽ�Ĺ�����>
    virtual HRESULT AddFarMount( const KG3DMESHFARMOUNTAIN_DESCSTRUCT* pDescStruct, IEKG3DMeshFarMountain** ppAddedFarMount ) = 0;///�������NULL�û�����Ĭ�ϲ�������
    virtual HRESULT DelFarMount( int nIndex ) = 0;
    virtual HRESULT DelFarMount( IEKG3DMeshFarMountain* pFarMount ) = 0;
    virtual HRESULT UpperFarMount( int nIndex ) = 0;
    virtual HRESULT LowerFarMount( int nIndex ) = 0;
    virtual HRESULT ClearFarMountVec() = 0;
    virtual HRESULT GetFarMount( int nIndex, IEKG3DMeshFarMountain** ppFarMount ) = 0;
    virtual HRESULT GetFarMountIndexByPointer( IEKG3DMeshFarMountain* pFarMout, int* pIndex ) = 0;///�õ�Vector�е�Index�����û�еĻ�����-1
    virtual HRESULT IsFarMountPointerValid( IEKG3DMeshFarMountain* pFarMout, BOOL* pBool ) = 0;///�ӷ���ֵ��pBool������֪���Ƿ���Ч
    virtual HRESULT GetFarMountVecSize( int* pSize ) = 0;
    ///</Զɽ�Ĺ�����>
    ///<�ƵĹ�����>
    virtual HRESULT AddCloud() = 0;
    virtual HRESULT DelCloud( int nIndex ) = 0;
    virtual HRESULT DelCloud( IEKG3DCloud* pCloud ) = 0;
    virtual HRESULT ClearCloudVec() = 0;
    virtual HRESULT GetCloud( int nIndex, IEKG3DCloud** ppCloud ) = 0;
    virtual HRESULT GetCloudIndexByPointer( IEKG3DCloud* pCloud, int* pIndex ) = 0;///�õ�Vector�е�Index�����û�еĻ�����-1
    virtual HRESULT IsCloudPointerValid( IEKG3DCloud* pCloud, BOOL* pBool ) = 0;///�ӷ���ֵ��pBool������֪���Ƿ���Ч
    virtual HRESULT GetCloudVecSize( int* pSize ) = 0;
    //virtual	HRESULT	SortCloudVec() = 0; ///ǿ�����������Ƶ�Vector
    ///</�ƵĹ�����>
	///<��ͬ��>
	//virtual HRESULT EnableSynWithFogColor(BOOL bEnable) = 0;
	//virtual HRESULT IsEnableSynWithFogColor(BOOL* pbEnable) = 0;
};

#endif //_INCLUDE_IEKG3DSKYSYSTEM_H_
