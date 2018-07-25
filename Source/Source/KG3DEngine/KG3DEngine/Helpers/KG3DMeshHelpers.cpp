////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DMeshHelpers.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-11-19 16:49:45
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG3DMeshHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
////////////////////////////////////////////////////////////////////////////////
BOOL IsMeshValidToBeCreated(DWORD dwMeshCreationOption, DWORD dwNumVertices, DWORD dwNumIndex)
{
	enum
	{
		em_max_vectices = 30000,
		em_max_index = em_max_vectices * 3,
	};
	if (dwNumVertices > em_max_vectices)
	{
		return FALSE;
	}

	if (dwNumIndex > em_max_index)
	{
		return FALSE;
	}

	if (dwNumVertices > std::numeric_limits<WORD>::max())
	{
		return (dwMeshCreationOption & D3DXMESH_32BIT) && _T("Ҫ��32bitIndexBuffer�������޷���ȷ��ӦVertex");
	}

	return TRUE;
}

HRESULT D3DXMeshTransformation( LPD3DXMESH pMesh, const D3DXMATRIX* pMatrix
							   , VisitorForD3DXVECTOR3* pPointFilter)
{
	KG_PROCESS_ERROR(NULL != pMesh && NULL != pMatrix);

	{
		DWORD fvf = pMesh->GetFVF();
		KG_PROCESS_ERROR(fvf | D3DFVF_XYZ);

		DWORD dwNumBytePerVertex = pMesh->GetNumBytesPerVertex();
		_ASSERTE(dwNumBytePerVertex >= sizeof(D3DXVECTOR3));

		DWORD dwNumVertex = pMesh->GetNumVertices();

		BYTE* pBufferStart = NULL;
		_ASSERTE(sizeof(BYTE) == 1);
		

		if(NULL == pPointFilter)
		{
			HRESULT hr = pMesh->LockVertexBuffer(0, (LPVOID*)&pBufferStart);
			KG_COM_PROCESS_ERROR(hr);
			D3DXVec3TransformCoordArray((D3DXVECTOR3*)(pBufferStart), dwNumBytePerVertex
				, (D3DXVECTOR3*)pBufferStart, dwNumBytePerVertex, pMatrix, dwNumVertex);
			pMesh->UnlockVertexBuffer();
		}
		else
		{
			//����Filter֮�������½��ǵ�Ȼ�ģ������ⲻ�Ǹ�ʵʱ�õģ�����û�й�ϵ������
			//���ô����͵���ʱ���ͺ���
			D3DXMeshVertexEnumer vertexEnumer;
			HRESULT hr = D3DXMeshCreateVertexEnumer(pMesh, vertexEnumer);
			KG_COM_PROCESS_ERROR(hr);
			_ASSERTE(vertexEnumer.IsValid());
			HRESULT hrForVisitor = E_FAIL;
			for (UINT i = 0; i < vertexEnumer.GetVertexCount(); ++i)
			{
				const D3DXVECTOR3& vTemp = vertexEnumer.GetPos(i);
				hrForVisitor = pPointFilter->Accept(vTemp);
				if (FAILED(hr))
				{
					continue;
				}

				D3DXVECTOR3 vTransformed;
				D3DXVec3TransformCoord(&vTransformed, &vTemp, pMatrix);
				vertexEnumer.SetPos(i, vTransformed);
			}
		}

		
	}		

	return S_OK;
Exit0:
	return E_FAIL;
}

HRESULT D3DXMeshClone(LPD3DXMESH pSrc, LPD3DXMESH* pCloned)
{
	LPD3DXMESH pMeshCloned = NULL;
	DWORD dwOption = pSrc->GetOptions();
	DWORD fvf = pSrc->GetFVF();
	LPDIRECT3DDEVICE9 pDevice = NULL;
	KG_PROCESS_ERROR(NULL != pSrc && NULL != pCloned);
	{
		HRESULT hr = pSrc->GetDevice(&pDevice);
		KG_COM_PROCESS_ERROR(hr);

		hr = pSrc->CloneMeshFVF(dwOption, fvf, pDevice, &pMeshCloned);
		SAFE_RELEASE(pDevice);

		*pCloned = pMeshCloned;
		
		SAFE_RELEASE(pDevice);
		return S_OK;
	}	
Exit0:
	SAFE_RELEASE(pMeshCloned);
	SAFE_RELEASE(pDevice);
	return E_FAIL;
}

HRESULT D3DXMeshCreateByVerticesPos( DWORD dwFVF, D3DVECTOR* pPosArray, DWORD dwVertexCount
									, DWORD* pIndexArray, DWORD dwNumIndexCount
									, LPDIRECT3DDEVICE9 pDevice, DWORD dwOption, LPD3DXMESH* pRet )
{
	HRESULT hr = E_FAIL;
	LPD3DXMESH pMesh = NULL;
	KG_PROCESS_ERROR((dwFVF & D3DFVF_XYZ));
	KG_PROCESS_ERROR(NULL != pPosArray && NULL != pIndexArray && NULL != pDevice && NULL != pRet);

	_ASSERTE(IsMeshValidToBeCreated(dwOption, dwVertexCount, dwNumIndexCount));

	{
		DWORD dwNumFaces = dwNumIndexCount / 3;
		
		hr = D3DXCreateMeshFVF(dwNumFaces, dwVertexCount, dwOption, dwFVF, pDevice, &pMesh);
		KG_COM_PROCESS_ERROR(hr);
		
#if defined(DEBUG) | defined(_DEBUG)
		ULONG uRefSave = KGGetRef(pMesh);
#endif

		{
			BOOL bValidity = pMesh->GetNumVertices() == dwVertexCount && pMesh->GetNumFaces() == dwNumFaces;
			KG_PROCESS_ERROR(bValidity);
		}
		
		_ASSERTE(sizeof(BYTE) == 1);
		{
			D3DXMeshVertexEnumer vertexEnumer;
			hr = D3DXMeshCreateVertexEnumer(pMesh, vertexEnumer);
			KG_COM_PROCESS_ERROR(hr);
			_ASSERTE(vertexEnumer.IsValid() && vertexEnumer.GetVertexCount() == dwVertexCount);
			for (UINT i = 0; i < vertexEnumer.GetVertexCount(); ++i)
			{
				vertexEnumer.SetPos(i, pPosArray[i]);
			}
		}

		{
			D3DXMeshIndexEnumer indexEnumer;
			hr = D3DXMeshCreateIndexEnumer(pMesh, indexEnumer);
			KG_COM_PROCESS_ERROR(hr);
			_ASSERTE(indexEnumer.IsValid() && indexEnumer.GetIndexCount() <= dwNumIndexCount);
			
			for (UINT i = 0; i < indexEnumer.GetIndexCount(); ++i)
			{
				indexEnumer.SetIndex(i, pIndexArray[i]);
			}
		}

		D3DXMeshZeroMeshAttributes(pMesh);

		_ASSERTE(uRefSave == KGGetRef(pMesh));
	}
	
	*pRet = pMesh;
	return S_OK;
Exit0:
	SAFE_RELEASE(pMesh);
	return E_FAIL;
}

HRESULT D3DXMeshZeroMeshAttributes(LPD3DXMESH pMesh )
{
	_ASSERTE(NULL != pMesh);
	HRESULT hr = E_FAIL;
	D3DXMeshAttributeEnumer attrEnumer;
	KG_PROCESS_ERROR(NULL != pMesh);
	hr = D3DXMeshCreateAttributeEnumer(pMesh, attrEnumer);
	KG_COM_PROCESS_ERROR(hr);
	attrEnumer.SetAllAttributeToValue(0);
Exit0:
	return hr;
}
HRESULT D3DXMeshCreatePlane( LPDIRECT3DDEVICE9 pDevice, DWORD dwFVF, DWORD dwOption, UINT uXSlice, UINT uZSlice
						, FLOAT fXLength, FLOAT fZLength, LPD3DXMESH* pRet )
{
	HRESULT hr = E_FAIL;
	LPD3DXMESH pMesh = NULL;

	KG_PROCESS_ERROR(D3DFVF_XYZ & dwFVF);//����XYZ�͵�Mesh�޷��Զ�����
	KG_PROCESS_ERROR(NULL != pRet);
	KG_PROCESS_ERROR(uXSlice < 2048 && uZSlice < 2048 && _T("���ܴ���̫���Plane"));

	{
		DWORD dwVertexCountInXAxis = (uXSlice + 1);
		DWORD dwVertexCountInZAxis = (uZSlice + 1);
		DWORD dwVertexCount = dwVertexCountInXAxis * dwVertexCountInZAxis; 
		DWORD dwNumFaces = uXSlice * uZSlice;

		_ASSERTE(IsMeshValidToBeCreated(dwOption, dwVertexCount, dwNumFaces * 3));

		hr = D3DXCreateMeshFVF(dwNumFaces, dwVertexCount, dwOption, dwFVF, pDevice, &pMesh);
		KG_COM_PROCESS_ERROR(hr);

		_ASSERTE(sizeof(BYTE) == 1);

		////���,D3DXMeshVertexEnumer������
		{
			D3DXMeshVertexEnumer vertexEnumer;
			hr = D3DXMeshCreateVertexEnumer(pMesh, vertexEnumer);
			KG_COM_PROCESS_ERROR(hr);

			FLOAT XSliceGap = fXLength / static_cast<FLOAT>(uXSlice);
			FLOAT ZSliceGap = fZLength / static_cast<FLOAT>(uZSlice);

			_ASSERTE(vertexEnumer.GetVertexCount() == dwVertexCount);

			for (UINT i = 0; i < dwVertexCountInXAxis; ++i)
			{
				for (UINT j = 0; j < dwVertexCountInZAxis; ++j)
				{
					FLOAT xPos = i * XSliceGap;
					FLOAT zPos = i * ZSliceGap;

					UINT uIndex = i * dwVertexCountInXAxis + j;
					vertexEnumer.SetPos(uIndex, D3DXVECTOR3(xPos, 0, zPos));
				}
			}

			//����������
			if (D3DFVF_TEX1 & dwFVF)	//���Ĭ�Ͼ���UV��������ġ��������D3DFVF_TEXCOORDn�Ļ���λ���
			{
				D3DXMeshVertexTexCoordEnumer texEnumer;
				hr = D3DXMeshCreateVertexTexCoordEnumer(vertexEnumer, texEnumer);

				FLOAT xUGap = 1.f / static_cast<FLOAT>(uXSlice);
				FLOAT zVGap = 1.f / static_cast<FLOAT>(uZSlice);

				if (SUCCEEDED(hr))
				{
					_ASSERTE(2 == texEnumer.GetTexCoordSize());
					for (UINT i = 0; i < dwVertexCountInXAxis; ++i)
					{
						for (UINT j = 0; j < dwVertexCountInZAxis; ++j)
						{
							UINT uIndex = i * dwVertexCountInXAxis + j;
							texEnumer.GetTexCoord2(uIndex) = D3DXVECTOR2(xUGap * i, zVGap * j);
						}
					}
				}
			}
		}////���,D3DXMeshVertexEnumer������

		//��Index
		{
			D3DXMeshIndexEnumer indexEnumer;
			hr = D3DXMeshCreateIndexEnumer(pMesh, indexEnumer);
			KG_COM_PROCESS_ERROR(hr);
			if (! indexEnumer.Is32Bit())
			{
				KG_PROCESS_ERROR((pMesh->GetNumVertices() < SHRT_MAX) && _T("����������IndexBuffer�������"));
			}
			
			for (UINT i = 0; i < uXSlice; ++i)
			{
				for (UINT j = 0; j < uZSlice; ++j)
				{
					DWORD uLowerLeftVertexIndex = i * (uXSlice + 1) + j;
					DWORD uLowerRightVertexIndex = uLowerLeftVertexIndex + 1;
					DWORD uUpperLeftVertexIndex = uLowerLeftVertexIndex + (uXSlice + 1);
					DWORD uUpperRightVertexIndex = uUpperLeftVertexIndex + 1;

					UINT uRegionIndexBegin = (i * uXSlice + j) * 6;

					indexEnumer.SetIndex(uRegionIndexBegin, uLowerLeftVertexIndex);
					indexEnumer.SetIndex(uRegionIndexBegin + 1, uUpperLeftVertexIndex);
					indexEnumer.SetIndex(uRegionIndexBegin + 2, uLowerRightVertexIndex);
					indexEnumer.SetIndex(uRegionIndexBegin + 3, uLowerLeftVertexIndex);
					indexEnumer.SetIndex(uRegionIndexBegin + 4, uUpperLeftVertexIndex);
					indexEnumer.SetIndex(uRegionIndexBegin + 5, uUpperRightVertexIndex);
					indexEnumer.SetIndex(uRegionIndexBegin + 6, uLowerRightVertexIndex);
				}
			}
		}
		
		//��Attribute
		hr = D3DXMeshZeroMeshAttributes(pMesh);
		
		_ASSERTE(NULL != pRet);
		*pRet = pMesh;
		return S_OK;
	}
Exit0:
	SAFE_RELEASE(pMesh);
	return NULL;
	return E_FAIL;
}


HRESULT D3DXMeshCreateDiamond( LPDIRECT3DDEVICE9 pDevice, DWORD dwFVF, DWORD dwOption, UINT uSlice
							, FLOAT fRadius, FLOAT fUpperConeHeight, FLOAT fLowerConeHeight, LPD3DXMESH* pRet )
{
	//�������¸�һ��׶����ʯ��
	_ASSERTE(fRadius > 0 && fUpperConeHeight >= 0 && fLowerConeHeight >= 0 && uSlice >= 3);

	HRESULT hr = E_FAIL;
	LPD3DXMESH pMesh = NULL;
	KG_PROCESS_ERROR(NULL != pRet);
	KG_PROCESS_ERROR(fRadius > 0 && fUpperConeHeight >= 0 && fLowerConeHeight >= 0 && uSlice >= 3);

	{
		UINT uNumVertices = uSlice + 2; //2������������
		UINT uNumFaces = uSlice * 2;

		_ASSERTE(IsMeshValidToBeCreated(dwOption, uNumVertices, uNumFaces * 3));
		
		hr = D3DXCreateMeshFVF(uNumFaces, uNumVertices, dwOption, dwFVF, pDevice, &pMesh);
		KG_COM_CHECK_ERROR(hr);

		//��ķֲ�������Բ�ܵ�һȦ��Ȼ��������������
		{
			D3DXMeshVertexEnumer vertexEnumer;
			hr = D3DXMeshCreateVertexEnumer(pMesh, vertexEnumer);
			KG_COM_PROCESS_ERROR(hr);
			_ASSERTE(vertexEnumer.IsValid() && vertexEnumer.GetVertexCount() == uNumVertices);

			float fAnglePerVertex = 2 * D3DX_PI /static_cast<FLOAT>(uSlice);
			float fAngleRotate = D3DX_PI / 4.f;	//�����ж�����ת45�ȣ������Ӻ�Щ

			//ע��Angle�ǲ����ܳ���cos��sin��ֵ��ģ����ü����
			for (UINT i = 0; i < vertexEnumer.GetVertexCount() - 2; ++i)
			{
				FLOAT fAngle = fAnglePerVertex * i;
				D3DXVECTOR3 vTemp;
				vTemp.x = fRadius * cosf(fAngle + fAngleRotate);	//��λԲ׶������ٷ���������������1
				vTemp.y = 0;
				vTemp.z = fRadius * sinf(fAngle + fAngleRotate);
				vertexEnumer.SetPos(i, vTemp);
			}
			_ASSERTE(vertexEnumer.GetVertexCount() > 2);
			//��������������
			UINT uTopPoint = vertexEnumer.GetVertexCount() - 2;
			vertexEnumer.SetPos(uTopPoint, D3DXVECTOR3(0, fUpperConeHeight, 0));
			UINT uBottomPoint = vertexEnumer.GetVertexCount() - 1;
			vertexEnumer.SetPos(uBottomPoint, D3DXVECTOR3(0, -fLowerConeHeight, 0));
		}

		{
			D3DXMeshIndexEnumer indexEnumer;
			hr = D3DXMeshCreateIndexEnumer(pMesh, indexEnumer);
			KG_COM_PROCESS_ERROR(hr);

			_ASSERTE(indexEnumer.IsValid()&& indexEnumer.GetIndexCount() == 2 * uSlice * 3);

			UINT uVertexCountInCircle = pMesh->GetNumVertices() - 2;
			{
				UINT uUpperFaceCount = uVertexCountInCircle;
				DWORD uTopPointIndex = pMesh->GetNumVertices() - 2;

				for (UINT uIt = 0, uIndexIndex = 0; uIt < uUpperFaceCount; ++uIt, uIndexIndex += 3)
				{
					indexEnumer.SetIndex(uIndexIndex, uIt);
					indexEnumer.SetIndex(uIndexIndex + 1, uTopPointIndex);
					indexEnumer.SetIndex(uIndexIndex + 2, (uIt + 1) % uVertexCountInCircle);
				}
			}

			{
				UINT uBottomFaceCount = uVertexCountInCircle;
				UINT uUpperFaceCount = uVertexCountInCircle;
				DWORD uBottomPointIndex = pMesh->GetNumVertices() - 1;

				for (UINT uIt = 0, uIndexIndex = uUpperFaceCount * 3
					; uIt < uBottomFaceCount; ++uIt, uIndexIndex += 3)
				{
					indexEnumer.SetIndex(uIndexIndex,  uIt);
					indexEnumer.SetIndex(uIndexIndex + 1, (uIt + 1) % uVertexCountInCircle);
					indexEnumer.SetIndex(uIndexIndex + 2, uBottomPointIndex);
				}
			}
		}
		hr = D3DXMeshZeroMeshAttributes(pMesh);
	}

	_ASSERTE(NULL != pRet);
	*pRet = pMesh;
	return S_OK;
Exit0:
	SAFE_RELEASE(pMesh);
	return E_FAIL;
}

HRESULT D3DXMeshCreateVertexEnumer( LPD3DXMESH pMesh, D3DXMeshVertexEnumer& enumer )
{
	_ASSERTE(! enumer.IsValid());
	ZeroMemory(&enumer, sizeof(D3DXMeshVertexEnumer));//��Ϊ�ⲿ�����enumer�ǿ������õģ��������Ҫ�������һ��

	_ASSERTE(NULL != pMesh);
	KG_PROCESS_ERROR(NULL != pMesh);
	{
		BOOL bIsAcceptableMesh = pMesh->GetFVF() & D3DFVF_XYZ;

		KG_PROCESS_ERROR(bIsAcceptableMesh && _T("��֧��û��XYZ��־��Mesh���������ܱ�֤ÿ���ڵ㿪ͷ�Ƕ���"));

		HRESULT hr = pMesh->LockVertexBuffer(0, reinterpret_cast<LPVOID*>(&enumer.m_pBuffer));
		KG_COM_PROCESS_ERROR(hr);
		_ASSERTE(1 == sizeof(BYTE));
		enumer.m_pMesh = pMesh;
		enumer.m_pMesh->AddRef();
		enumer.m_dwNumBytePerVertex = pMesh->GetNumBytesPerVertex();
		enumer.m_dwNumVertexCount = pMesh->GetNumVertices();
		return S_OK;
	}
Exit0:
	return E_FAIL;
}

BOOL D3DXMeshVertexEnumer::IsValid()
{
	return NULL != m_pBuffer;
}

D3DXMeshVertexEnumer::~D3DXMeshVertexEnumer()
{
	if (IsValid())
	{
		_ASSERTE(NULL != m_pMesh);
		m_pMesh->UnlockVertexBuffer();
	}
	SAFE_RELEASE(m_pMesh);
}

UINT D3DXMeshVertexEnumer::GetVertexCount()
{
	_ASSERTE(IsValid());
	return m_dwNumVertexCount;
}

D3DXMeshVertexEnumer::D3DXMeshVertexEnumer()
{
	ZeroMemory(this, sizeof(D3DXMeshVertexEnumer));
}

VOID D3DXMeshVertexEnumer::SetPos( UINT uIndex, const D3DXVECTOR3& vPos )
{
	BYTE* pVertex = GetVertexByteHead(uIndex);
	*reinterpret_cast<D3DXVECTOR3*>(pVertex) = vPos;
}

const D3DXVECTOR3& D3DXMeshVertexEnumer::GetPos( UINT uIndex )
{
	BYTE* pVertex = GetVertexByteHead(uIndex);
	return *reinterpret_cast<const D3DXVECTOR3*>(pVertex);
}

BYTE* D3DXMeshVertexEnumer::GetVertexByteHead( UINT uIndex )
{
	_ASSERTE(IsValid());
	_ASSERTE(uIndex < GetVertexCount());
	return m_pBuffer + m_dwNumBytePerVertex * uIndex;
}

LPD3DXMESH D3DXMeshVertexEnumer::GetMesh()
{
	return m_pMesh;
}
//////////////////////////////////////////////////////////////////////////
HRESULT D3DXMeshCreateVertexTexCoordEnumer( D3DXMeshVertexEnumer& vEnumer, D3DXMeshVertexTexCoordEnumer& texCoordEnumer )
{
	_ASSERTE(vEnumer.IsValid() && !texCoordEnumer.IsValid());
	KG_PROCESS_ERROR(vEnumer.IsValid());
	{
		ZeroMemory(&texCoordEnumer, sizeof(D3DXMeshVertexTexCoordEnumer));
		
		_ASSERTE(! texCoordEnumer.IsValid());

		texCoordEnumer.m_pVertexEnumer = &vEnumer;
		DWORD dwFVF = vEnumer.GetMesh()->GetFVF();
		D3DVERTEXELEMENT9 declarationTemp[MAX_FVF_DECL_SIZE];
		
		HRESULT hr = D3DXDeclaratorFromFVF(dwFVF, declarationTemp);
		KG_COM_PROCESS_ERROR(hr);
		
		for (size_t i = 0; i < _countof(declarationTemp); ++i)
		{
			D3DVERTEXELEMENT9& elemTemp = declarationTemp[i];
			if (0xFF == elemTemp.Stream)//���һ��D3DDECL_END
			{
				break;
			}

			if (D3DDECLUSAGE_TEXCOORD == elemTemp.Usage)
			{
				_ASSERTE(0 == elemTemp.Stream);
				texCoordEnumer.m_dwByteOffsetOfTexture = elemTemp.Offset;
				switch(elemTemp.Type)
				{
				case D3DDECLTYPE_FLOAT1:
					texCoordEnumer.m_dwTexCoordSize = 1;
					break;
				case D3DDECLTYPE_FLOAT2:
					texCoordEnumer.m_dwTexCoordSize = 2;
					break;
				case D3DDECLTYPE_FLOAT3:
					texCoordEnumer.m_dwTexCoordSize = 3;
					break;
				case D3DDECLTYPE_FLOAT4:
					texCoordEnumer.m_dwTexCoordSize = 4;
					break;
				default:
					goto Exit0;
				    break;
				}
				_ASSERTE(D3DDECLMETHOD_DEFAULT == elemTemp.Method);
				_ASSERTE(0 == elemTemp.UsageIndex);//�����ǵڼ���TexCoord
			}
		}
		
		KG_PROCESS_ERROR(texCoordEnumer.IsValid());
	}
	return S_OK;
Exit0:
	return E_FAIL;
}
D3DXMeshVertexTexCoordEnumer::D3DXMeshVertexTexCoordEnumer()
{
	ZeroMemory(this, sizeof(D3DXMeshVertexTexCoordEnumer));
}
BYTE* D3DXMeshVertexTexCoordEnumer::GetTexCoordHead( UINT uIndex )
{
	_ASSERTE(IsValid());
	BYTE* p = m_pVertexEnumer->GetVertexByteHead(uIndex);
	return p + m_dwByteOffsetOfTexture;
}
FLOAT& D3DXMeshVertexTexCoordEnumer::GetTexCoord1( UINT uIndex )
{
	_ASSERTE(1 == m_dwTexCoordSize);
	BYTE* p = GetTexCoordHead(uIndex);
	return *reinterpret_cast<FLOAT*>(p);
}
D3DXVECTOR2& D3DXMeshVertexTexCoordEnumer::GetTexCoord2( UINT uIndex )
{
	_ASSERTE(2 == m_dwTexCoordSize && IsValid());
	BYTE* p = GetTexCoordHead(uIndex);
	return *reinterpret_cast<D3DXVECTOR2*>(p);
}
D3DXVECTOR3& D3DXMeshVertexTexCoordEnumer::GetTexCoord3( UINT uIndex )
{
	_ASSERTE(3 == m_dwTexCoordSize && IsValid());
	BYTE* p = GetTexCoordHead(uIndex);
	return *reinterpret_cast<D3DXVECTOR3*>(p);
}
D3DXVECTOR4& D3DXMeshVertexTexCoordEnumer::GetTexCoord4( UINT uIndex )
{
	_ASSERTE(4 == m_dwTexCoordSize && IsValid());
	BYTE* p = GetTexCoordHead(uIndex);
	return *reinterpret_cast<D3DXVECTOR4*>(p);
}

BOOL D3DXMeshVertexTexCoordEnumer::IsValid()
{
	return 0 != m_dwTexCoordSize
#if defined(DEBUG) | defined(_DEBUG)
		&& NULL != m_pVertexEnumer
#endif
		;
}

UINT D3DXMeshVertexTexCoordEnumer::GetTexCoordSize()
{
	return m_dwTexCoordSize;
}


//////////////////////////////////////////////////////////////////////////
HRESULT D3DXMeshCreateIndexEnumer( LPD3DXMESH pMesh, D3DXMeshIndexEnumer& enumer )
{
	_ASSERTE(! enumer.IsValid());
	ZeroMemory(&enumer, sizeof(D3DXMeshIndexEnumer)); //��Ϊ�ⲿ�����enumer�ǿ������õģ��������Ҫ�������һ��

	_ASSERTE(NULL != pMesh);
	KG_PROCESS_ERROR(NULL != pMesh);
	{
		HRESULT hr = pMesh->LockIndexBuffer(0, reinterpret_cast<LPVOID*>(&enumer.m_pBuffer));
		KG_COM_PROCESS_ERROR(hr);
		_ASSERTE(1 == sizeof(BYTE) && 2 == sizeof(WORD) && 4 == sizeof(DWORD));

		enumer.m_pMesh = pMesh;
		enumer.m_pMesh->AddRef();
		enumer.m_dwNumBytePerIndex = (pMesh->GetOptions() & D3DXMESH_32BIT) ? 4 : 2;
		enumer.m_dwNumIndexCount = pMesh->GetNumFaces() * 3;
	
		return S_OK;
	}
Exit0:
	return E_FAIL;
}
D3DXMeshIndexEnumer::D3DXMeshIndexEnumer()
{
	ZeroMemory(this, sizeof(*this));
}

D3DXMeshIndexEnumer::~D3DXMeshIndexEnumer()
{
	if (IsValid())
	{
		_ASSERTE(NULL != m_pMesh);
		m_pMesh->UnlockIndexBuffer();
	}
	SAFE_RELEASE(m_pMesh);
}

BOOL D3DXMeshIndexEnumer::IsValid()
{
	return NULL != m_pBuffer;
}

VOID D3DXMeshIndexEnumer::SetIndex( UINT uWhichIndex, DWORD dwIndex )
{
	_ASSERTE(uWhichIndex < GetIndexCount());
	BYTE* pCurIndex = m_pBuffer + m_dwNumBytePerIndex * uWhichIndex;
	if(Is32Bit())
		*reinterpret_cast<DWORD*>(pCurIndex) = dwIndex;
	else
		*reinterpret_cast<WORD*>(pCurIndex) = (WORD)dwIndex;
}

DWORD D3DXMeshIndexEnumer::GetIndex( UINT uWhichIndex )
{
	_ASSERTE(uWhichIndex < GetIndexCount());
	BYTE* pCurIndex = m_pBuffer + m_dwNumBytePerIndex * uWhichIndex;
	if (4 == m_dwNumBytePerIndex)
	{
		return *reinterpret_cast<DWORD*>(pCurIndex);
	}
	else
	{
		return *reinterpret_cast<WORD*>(pCurIndex);
	}
}
UINT D3DXMeshIndexEnumer::GetIndexCount()
{
	return m_dwNumIndexCount;
}

BOOL D3DXMeshIndexEnumer::Is32Bit()
{
	_ASSERTE(NULL != m_pMesh);
	return 4 == m_dwNumBytePerIndex;
}
//////////////////////////////////////////////////////////////////////////
HRESULT D3DXMeshCreateAttributeEnumer(LPD3DXMESH pMesh, D3DXMeshAttributeEnumer& enumer )
{
	_ASSERTE(! enumer.IsValid());
	ZeroMemory(&enumer, sizeof(D3DXMeshAttributeEnumer)); //��Ϊ�ⲿ�����enumer�ǿ������õģ��������Ҫ�������һ��

	_ASSERTE(NULL != pMesh);
	KG_PROCESS_ERROR(NULL != pMesh);
	{
		HRESULT hr = pMesh->LockAttributeBuffer(0, reinterpret_cast<DWORD**>(&enumer.m_pBuffer));
		KG_COM_PROCESS_ERROR(hr);
		_ASSERTE(1 == sizeof(BYTE) && 2 == sizeof(WORD) && 4 == sizeof(DWORD));

		enumer.m_pMesh = pMesh;
		enumer.m_pMesh->AddRef();
		enumer.m_dwNumFaceCount = pMesh->GetNumFaces();

		return S_OK;
	}
Exit0:
	return E_FAIL;
}


D3DXMeshAttributeEnumer::D3DXMeshAttributeEnumer()
{
	ZeroMemory(this, sizeof(*this));
}

D3DXMeshAttributeEnumer::~D3DXMeshAttributeEnumer()
{
	if (IsValid())
	{
		_ASSERTE(NULL != m_pMesh);
		m_pMesh->UnlockAttributeBuffer();
	}
	SAFE_RELEASE(m_pMesh);
}

BOOL D3DXMeshAttributeEnumer::IsValid()
{
	return NULL != m_pBuffer;
}

VOID D3DXMeshAttributeEnumer::SetAttribute( UINT uWhichIndex, DWORD dwAttribute )
{
	_ASSERTE(uWhichIndex < GetAttributeCount());
	*(reinterpret_cast<DWORD*>(m_pBuffer) + uWhichIndex) = dwAttribute;
}

DWORD D3DXMeshAttributeEnumer::GetAttribute( UINT uWhichIndex)
{
	_ASSERTE(uWhichIndex < GetAttributeCount());
	return *(reinterpret_cast<DWORD*>(m_pBuffer) + uWhichIndex);
}

UINT D3DXMeshAttributeEnumer::GetAttributeCount()
{
	return m_dwNumFaceCount;
}

VOID D3DXMeshAttributeEnumer::SetAllAttributeToValue( DWORD dwValue )
{
	_ASSERTE(IsValid());
	if (0 == dwValue)
	{
		memset(m_pBuffer, 0, m_dwNumFaceCount * sizeof(DWORD));
	}
	else
	{
		for (UINT i = 0; i < m_dwNumFaceCount; ++i)
		{
			SetAttribute(i, dwValue);
		}
	}
}
#ifdef KG_ENABLE_TDD
KG_TEST_BEGIN(KG3DMeshHelpers)
{
	_ASSERTE(IsMeshValidToBeCreated(D3DXMESH_32BIT, 10, 10));
	_ASSERTE(! IsMeshValidToBeCreated(0, std::numeric_limits<WORD>::max(), 10));
	_ASSERTE(! IsMeshValidToBeCreated(D3DXMESH_32BIT, std::numeric_limits<DWORD>::max(), 10));
	_ASSERTE(! IsMeshValidToBeCreated(D3DXMESH_32BIT, 10, std::numeric_limits<DWORD>::max()));
}
KG_TEST_END(KG3DMeshHelpers)
#endif