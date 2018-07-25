////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DMeshHelpers.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-11-19 16:49:50
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DMESHHELPERS_H_
#define _INCLUDE_KG3DMESHHELPERS_H_
#include "KGMathFunctions.h"
////////////////////////////////////////////////////////////////////////////////

HRESULT D3DXMeshTransformation(LPD3DXMESH pMesh, const D3DXMATRIX* pMatrix, VisitorForD3DXVECTOR3* pPointFilter);//ֻ��FVF������D3DFVF_XYZ��Mesh�����ã����Ҷ���Ҫ����ǰ��, pointFilter���ڹ���ĳЩ����Ҫ�任�ĵ�,���Դ�NULL������pointFilterҪһ�������飬�Ƚ���
HRESULT D3DXMeshClone(LPD3DXMESH pSrc, LPD3DXMESH* pCloned);
HRESULT D3DXMeshCreateByVerticesPos(DWORD dwFVF, D3DVECTOR* pPosArray, DWORD dwVertexCount
									, DWORD* pIndexArray, DWORD dwIndexCount
									, LPDIRECT3DDEVICE9 pDevice, DWORD dwOption, LPD3DXMESH* pRet);//3��Indexһ����
HRESULT D3DXMeshCreatePlane(LPDIRECT3DDEVICE9 pDevice, DWORD dwFVF, DWORD dwOption, UINT uXSlice, UINT uZSlice
						, FLOAT fXLength, FLOAT fZLength, LPD3DXMESH* pRet);	//һ����XZƽ���ϵ�Mesh

HRESULT D3DXMeshCreateDiamond(LPDIRECT3DDEVICE9 pDevice, DWORD dwFVF, DWORD dwOption, UINT uSlice
							, FLOAT fRadius, FLOAT fUpperConeHeight, FLOAT fLowerConeHeight, LPD3DXMESH* pRet);//��������Բ׶��ɵ���ʯ��

//////////////////////////////////////////////////////////////////////////

class D3DXMeshVertexEnumer;

HRESULT D3DXMeshCreateVertexEnumer(LPD3DXMESH pMesh, D3DXMeshVertexEnumer& enumer);

class D3DXMeshVertexEnumer	
{
	friend HRESULT D3DXMeshCreateVertexEnumer(LPD3DXMESH pMesh, D3DXMeshVertexEnumer& enumer);
public:
	BOOL	IsValid();//��D3DXMeshCreateEnumer����������Enumer�����������Ƿ���Ч
	VOID		SetPos(UINT uIndex, const D3DXVECTOR3& vPos);
	const D3DXVECTOR3& GetPos(UINT uIndex);
	UINT		GetVertexCount();
	BYTE*		GetVertexByteHead(UINT uIndex);
	LPD3DXMESH	GetMesh();

	D3DXMeshVertexEnumer();
	~D3DXMeshVertexEnumer();//������ʱ���Զ�UnLock
private:	//һ��Ҫ��private��
	LPD3DXMESH m_pMesh;
	DWORD m_dwNumBytePerVertex;
	DWORD m_dwNumVertexCount;
	BYTE* m_pBuffer;

	D3DXMeshVertexEnumer(const D3DXMeshVertexEnumer&);	//���ܸ���
	D3DXMeshVertexEnumer& operator=(const D3DXMeshVertexEnumer&);//���ܸ�ֵ
};

//////////////////////////////////////////////////////////////////////////
class D3DXMeshVertexTexCoordEnumer;

HRESULT D3DXMeshCreateVertexTexCoordEnumer(D3DXMeshVertexEnumer& vEnumer, D3DXMeshVertexTexCoordEnumer& texCoordEnumer);//vEnumer�����Ѿ�Valid��

class D3DXMeshVertexTexCoordEnumer	//��Ҫ��VertexEnumer������������ʹ��
{
	friend HRESULT D3DXMeshCreateVertexTexCoordEnumer(D3DXMeshVertexEnumer& vEnumer, D3DXMeshVertexTexCoordEnumer& texCoordEnumer);
public:
	FLOAT&	GetTexCoord1(UINT uIndex);
	D3DXVECTOR2&	GetTexCoord2(UINT uIndex);
	D3DXVECTOR3&	GetTexCoord3(UINT uIndex);
	D3DXVECTOR4&	GetTexCoord4(UINT uIndex);
	BOOL	IsValid();
	UINT	GetTexCoordSize();//���ö��ٸ�����
	BYTE*	GetTexCoordHead(UINT uIndex);
	
	D3DXMeshVertexTexCoordEnumer();
private:
	D3DXMeshVertexEnumer* m_pVertexEnumer;
	DWORD	m_dwByteOffsetOfTexture;
	DWORD	m_dwTexCoordSize;

	D3DXMeshVertexTexCoordEnumer(const D3DXMeshVertexTexCoordEnumer&);	//���ܸ���
	D3DXMeshVertexTexCoordEnumer& operator=(const D3DXMeshVertexTexCoordEnumer&);//���ܸ�ֵ
};
//////////////////////////////////////////////////////////////////////////
class D3DXMeshIndexEnumer;

HRESULT D3DXMeshCreateIndexEnumer(LPD3DXMESH pMesh, D3DXMeshIndexEnumer& enumer);

class D3DXMeshIndexEnumer
{
	friend HRESULT D3DXMeshCreateIndexEnumer(LPD3DXMESH pMesh, D3DXMeshIndexEnumer& enumer);
public:
	BOOL	IsValid();//�����������Ƿ���Ч
	VOID	SetIndex(UINT uWhichIndex, DWORD dwIndex);//16λIndexBuffer��ʱ���ڲ���ǿ�ƽ���WORD��ֻҪ��ֵ��ʱ��֤��ԽWORD_MAX��û����
	DWORD	GetIndex(UINT uWhichIndex);
	BOOL	Is32Bit();
	UINT	GetIndexCount();
	D3DXMeshIndexEnumer();
	~D3DXMeshIndexEnumer();//������ʱ���Զ�UnLock
private:	//һ��Ҫ��private��
	LPD3DXMESH m_pMesh;
	DWORD m_dwNumBytePerIndex;
	DWORD m_dwNumIndexCount;
	BYTE* m_pBuffer;

	D3DXMeshIndexEnumer(const D3DXMeshIndexEnumer&);	//���ܸ���
	D3DXMeshIndexEnumer& operator=(const D3DXMeshIndexEnumer&);//���ܸ�ֵ
};
//////////////////////////////////////////////////////////////////////////
class D3DXMeshAttributeEnumer;

HRESULT D3DXMeshCreateAttributeEnumer(LPD3DXMESH pMesh, D3DXMeshAttributeEnumer& enumer);
HRESULT D3DXMeshZeroMeshAttributes(LPD3DXMESH pMesh );

class D3DXMeshAttributeEnumer
{
	friend HRESULT D3DXMeshCreateAttributeEnumer(LPD3DXMESH pMesh, D3DXMeshAttributeEnumer& enumer);
public:
	BOOL	IsValid();//�����������Ƿ���Ч
	
	VOID	SetAttribute(UINT uWhichIndex, DWORD dwAttribute);
	DWORD	GetAttribute(UINT uWhichIndex);

	UINT	GetAttributeCount();
	VOID	SetAllAttributeToValue(DWORD dwValue);

	D3DXMeshAttributeEnumer();
	~D3DXMeshAttributeEnumer();//������ʱ���Զ�UnLock
private:	//һ��Ҫ��private��
	LPD3DXMESH m_pMesh;
	DWORD m_dwNumFaceCount;
	BYTE* m_pBuffer;

	D3DXMeshAttributeEnumer(const D3DXMeshAttributeEnumer&);	//���ܸ���
	D3DXMeshAttributeEnumer& operator=(const D3DXMeshAttributeEnumer&);//���ܸ�ֵ
};


#endif //_INCLUDE_KG3DMESHHELPERS_H_
