#include "stdafx.h"

///�����ѡ��
#include "KG3DEnvEffCommonDef.h"

#include "KG3DMeshFarMountain.h"
//#include "KG3DGraphicsStructEx.h"//GraphicsStruct::RenderStateAlphaWithoutSettingAlphaTest
#include "KG3DEngineManager.h"//g_Def_WorkDirectory
#include "KG3DTextureTable.h"
#include "KG3DStructs.h"
#include "MathTool.h"
#include "KGINIFileHelpers.h"
#include "KG3DMeshTable.h"
#include "KG3DGraphicsTool.h"
#include "shaders/ShaderDefinitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace 
{
	const TCHAR* KG3DMESHFARMOUNTAIN_MESH_PATH = _T("Data\\public\\FarMountain\\FarMountain.Mesh");
	const TCHAR* KG3DMESHFARMOUNTAIN_TEXTURE_PATH = _T("Data\\public\\FarMountain\\Texture\\FarMountain01.tga");
	const FLOAT FARMOUNT_PARAMETER_MAX_FLOAT = FLT_MAX /2;
}

namespace KG3D_ENVIRONMENT_EFFECT
{
	KG3DMeshFarMountain::KG3DMeshFarMountain()
		: KG3DEnvEffect(0, KG3DTYPE_ENVEFF_SKYSYSTEM_FARMOUNTAIN)
		, m_pVB(NULL)
	{
		m_tczName[0] = '\0';

//		m_pD3DXMesh = NULL;

		m_pColorMap = NULL;
		//m_uTextureHeight = m_uTextureWidth = 0;
		//m_bIsTextureLoaded = false;
		
		//ZeroMemory( m_tczTexturePathBuffer, sizeof(m_tczTexturePathBuffer) );
		m_bIsInitialized = false;
		
		ZeroMemory(m_tczName, sizeof(m_tczName));

		//m_d3dc_FogBlendedColor = D3DCOLOR_XRGB( 0, 0, 0 );
		
		//m_bBlockReloadMessage = false;

		m_bEnableBlending = false;
		//m_bIsSubStateInitialized = false;
		m_fBlendPercent = 0.0f;
	}
//
	KG3DMeshFarMountain::~KG3DMeshFarMountain()
	{
		//SAFE_RELEASE(m_pD3DXMesh);
		SAFE_RELEASE(m_pVB);
		SAFE_RELEASE( m_pColorMap );
	}

    HRESULT KG3DMeshFarMountain::OnLostDevice()
    {
        HRESULT hr = E_FAIL;
        HRESULT hrResult = E_FAIL;

        hr = KG3DEnvEffect::OnLostDevice();
        KGLOG_COM_PROCESS_ERROR(hr);

        hrResult = S_OK;
Exit0:
        return hrResult;
    }

    HRESULT KG3DMeshFarMountain::OnResetDevice()
    {
        HRESULT hr = E_FAIL;
        HRESULT hrResult = E_FAIL;

        hr = KG3DEnvEffect::OnResetDevice();
        KGLOG_COM_PROCESS_ERROR(hr);

        hrResult = S_OK;
Exit0:
        return hrResult;
    }

	HRESULT KG3DMeshFarMountain::CustomInit()
	{
		HRESULT hr  = E_FAIL;
        HRESULT hrResult = E_FAIL;
		KG3DMesh* pMesh = NULL;
//        LPD3DXMESH lpD3DXMesh = NULL;

		ASSERT(!m_bIsInitialized);

		//����Ĭ�ϵ�Mesh�����Ǳض�Ҫ��

        //_ASSERTE(m_pD3DXMesh == NULL);
		
		/*hr = g_cMeshTable.LoadResourceFromFile(KG3DMESHFARMOUNTAIN_MESH_PATH, 0, 0, (IKG3DResourceBase**)&pMesh);
		KGLOG_COM_PROCESS_ERROR(hr);		

        lpD3DXMesh = pMesh->GetMesh(KG3DMesh::SMBT_NORMAL);
        KGLOG_PROCESS_ERROR(lpD3DXMesh);

        m_BBox = pMesh->m_BBox;

        hr = ProcessMesh(lpD3DXMesh);
        KGLOG_COM_PROCESS_ERROR(hr);*/
		
		_ASSERTE(NULL == m_pVB);
		FLOAT fAngleStep = (- 2 * D3DX_PI / (FLOAT)em_slice);
		TypeVertex	arrayVerices[(em_slice + 1) * 2];//ÿ����4�����㣬32��Slice�����������ٸ�������
		for (size_t i = 0; i < em_slice; ++i)
		{
			FLOAT fAngle =  D3DXToRadian(-50)+ fAngleStep * (FLOAT)i;	//ԭ��û���ö�̬����֮ǰ��ʵ��ģ�ͣ�û�п�����UV�ģ�Ϊ�˺�ԭ����һ��ƥ�䣬�ý�ͼ�Ա�֮������������Ҫ���ϵĶ���������������ȫƥ��
			FLOAT fX = cos(fAngle);
			FLOAT fZ = sin(fAngle);

			TypeVertex* pCurVertex = arrayVerices + i * 2;
			pCurVertex[0].x = fX;
			pCurVertex[0].y = 0;
			pCurVertex[0].z = fZ;
			pCurVertex[0].uPercent = 1 - (FLOAT)i / (FLOAT)em_slice;
			pCurVertex[1].x = fX;
			pCurVertex[1].y = 1;
			pCurVertex[1].z = fZ;
			pCurVertex[1].uPercent = 1 - (FLOAT)i / (FLOAT)em_slice;
			_ASSERTE(pCurVertex + 1 < ((TypeVertex*)arrayVerices + _countof(arrayVerices)));
		}
		//�����������㸴�Ƴ���ǰ��������
		errno_t errt = memcpy_s(arrayVerices + em_slice * 2
						, 2 * sizeof(TypeVertex), arrayVerices, 2 * sizeof(TypeVertex));
		errt;
		_ASSERTE(0 == errt);

		arrayVerices[em_slice * 2].uPercent = 0;
		arrayVerices[em_slice * 2 + 1].uPercent = 0;

		hr = g_pd3dDevice->CreateVertexBuffer(sizeof(arrayVerices), 0, em_fvf, D3DPOOL_MANAGED, &m_pVB, NULL);
		KG_COM_PROCESS_ERROR(hr);

		LPVOID pData;
		hr = m_pVB->Lock(0, sizeof(arrayVerices), &pData, 0);
		KG_COM_PROCESS_ERROR(hr);
		memcpy_s(pData, sizeof(arrayVerices), arrayVerices, sizeof(arrayVerices));
		m_pVB->Unlock();


		m_DescStruct.SetDefaultValue();

		m_bIsInitialized = true;
		
		hrResult = S_OK;
Exit0:
		SAFE_RELEASE(pMesh);
        if (FAILED(hrResult))
        {
            m_bIsInitialized = false;
          //  SAFE_RELEASE(m_pD3DXMesh);	//���ģ������ʧ�ܣ�������з���
			SAFE_RELEASE(m_pVB);
        }
		return hrResult;	
	}

	/*HRESULT KG3DMeshFarMountain::ProcessMesh(LPD3DXMESH pMeshIn)
	{
		HRESULT hr  = E_FAIL;
        HRESULT hrResult = E_FAIL;
		const DWORD tempFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
		LPD3DXMESH pMesh = NULL;
        BOOL bLockVertexBuffer = FALSE;
		struct _Ver 
		{
			D3DXVECTOR3 Pos;
			D3DXVECTOR3 Nor;
			D3DXVECTOR3 Tex;
		}* pVers = NULL;


		KGLOG_PROCESS_ERROR(pMeshIn);

		hr = pMeshIn->CloneMeshFVF(D3DXMESH_MANAGED, tempFVF, g_pd3dDevice, &pMesh);
		KGLOG_COM_PROCESS_ERROR(hr);
		
		hr = D3DXComputeNormals(pMesh, NULL);
        KGLOG_COM_PROCESS_ERROR(hr);

        ASSERT(m_pD3DXMesh == NULL);

        m_pD3DXMesh = pMesh;
		pMesh = NULL;

		hr = m_pD3DXMesh->LockVertexBuffer(0, (void**)&pVers);
		KGLOG_COM_PROCESS_ERROR(hr);

        bLockVertexBuffer = TRUE;

		{
			D3DXVECTOR3 PO;
			float fXYScaleFactor = m_DescStruct.m_fRadius;
			float fZMin = 100000.0f;
            float fZMax = -10000000.0f;
			float fUMax = -100.0f;
            float fUMin = 100.0f;
			D3DXVECTOR3 Center = m_BBox.GetCenter();
			int nNumVertices = m_pD3DXMesh->GetNumVertices();
			for (int i = 0; i < nNumVertices; i++)
			{
				PO = pVers[i].Pos - Center;		///��SizeΪ�����Ŵ�Mesh
				D3DXVec3Normalize( &PO, &PO );
				pVers[i].Pos = Center + ( PO )*fXYScaleFactor;
				if ( pVers[i].Pos.y < fZMin )///������Z��ʵ��Y
				{
					fZMin = pVers[i].Pos.y;
				}
				if ( pVers[i].Pos.y > fZMax )
				{
					fZMax = pVers[i].Pos.y;
				}
				if ( pVers[i].Tex.x < fUMin )
				{
					fUMin = pVers[i].Tex.x;
				}
				if ( pVers[i].Tex.x > fUMax )
				{
					fUMax = pVers[i].Tex.x; 
				}		
			}
			float fTempHeight = fZMax - fZMin;

			m_DescStruct.m_fHeight = fTempHeight;
			KGLOG_PROCESS_ERROR(m_DescStruct.m_fHeight > FLT_EPSILON);

            m_DescStruct.m_fTexCordUTiles = fUMax - fUMin;

			///Z�ײ������ƶ���0
			for (int i = 0; i < nNumVertices; i++)
			{
				if ( pVers[i].Pos.y < fZMin + 0.1f )
				{
					pVers[i].Pos.y = 0;
				}
				else
				{
					pVers[i].Pos.y = fTempHeight;
				}
			}
			m_DescStruct.m_fBotmZPos = 0;
		}

		hrResult = S_OK;
Exit0:
        if (bLockVertexBuffer)
        {
			m_pD3DXMesh->UnlockVertexBuffer();
        }

        if (FAILED(hrResult))
        {
            SAFE_RELEASE(m_pD3DXMesh);
        }

		SAFE_RELEASE(pMesh);
		return hrResult;
	}*/

//	HRESULT KG3DMeshFarMountain::ModifyMesh(const float* pRadius, const float* pHeight, 
//        const float* pBotmZPos, const float *pfZRotation, const float *pfTextureCordUTiles)
//	{
//        HRESULT hr = E_FAIL;
//        HRESULT hrResult = E_FAIL;
//        BOOL bLockVertexBuffer = FALSE;
//		struct _Ver 
//		{
//			D3DXVECTOR3 Pos;
//			D3DXVECTOR3 Nor;
//			D3DXVECTOR3 Tex;
//		}* pVers = NULL;
//
//        ASSERT(m_bIsInitialized);
//
//		KG_PROCESS_SUCCESS(pRadius == NULL && pHeight == NULL && pBotmZPos == NULL && pfZRotation == NULL && pfTextureCordUTiles == NULL);
//
//		KG_PROCESS_ERROR(m_pD3DXMesh);
//
//        hr = m_pD3DXMesh->LockVertexBuffer(0, (void**)&pVers);
//        KGLOG_COM_PROCESS_ERROR(hr);
//
//        bLockVertexBuffer = TRUE;
//
//        {
//			D3DXVECTOR3 tempVec( 0.0f, 0.0f, 0.0f );
//			D3DXVECTOR3 tempCenterVec( 0.0f, 0.0f, 0.0f );
//			int nNumVer = (int)m_pD3DXMesh->GetNumVertices();
//			D3DXVECTOR3 Center = m_BBox.GetCenter();	
//			_ASSERTE(nNumVer>=0);
//			//��Radius
//			if( pRadius )
//			{
//				if ( 
//					(*pRadius != m_DescStruct.m_fRadius)
//					&& *pRadius > 0.0f && *pRadius < FARMOUNT_PARAMETER_MAX_FLOAT
//					)
//				{
//					float fXYScaleFactor = (float)*pRadius;
//					for (int i = 0; i < nNumVer; i++)
//					{
//						tempCenterVec.x = Center.x;
//						tempCenterVec.z = Center.z;
//						tempCenterVec.y = pVers[i].Pos.y;
//						tempVec = pVers[i].Pos - tempCenterVec;		///��RadiusΪ�����Ŵ�Mesh
//						tempVec.y = 0; ///ֻ��xyƽ������
//						D3DXVec3Normalize( &tempVec, &tempVec );
//						tempVec = tempCenterVec + fXYScaleFactor*tempVec; 
//						pVers[i].Pos.x = tempVec.x;
//						pVers[i].Pos.z = tempVec.z;///ע����ʵ��Z����ʵ��D3D��Y��
//					}
//					m_DescStruct.m_fRadius = *pRadius;
//				}
//			}
//
//			//�ĸ߶Ⱥ�BotmZPos
//			if (pHeight || pBotmZPos)
//			{
//				bool bModifyHeight = false;
//                bool bModifyBotmZPos = false;
//				float fHeight = m_DescStruct.m_fHeight;
//				float fBotmZPos = m_DescStruct.m_fBotmZPos;
//
//				if (pHeight)
//				{
//					if (MT_FLOAT_UNEQUAL(*pHeight, m_DescStruct.m_fHeight))
//					{
//						bModifyHeight = true;
//						fHeight = *pHeight;
//					}
//				}
//				if( pBotmZPos )
//				{
//					if (
//						(*pBotmZPos != m_DescStruct.m_fBotmZPos)///BotmZPos����Ϊ�κ�ֵ
//						&& *pBotmZPos < FARMOUNT_PARAMETER_MAX_FLOAT 
//						)
//						///������
//					{
//						bModifyBotmZPos = true;
//						fBotmZPos = *pBotmZPos;
//					}
//				}
//				if (bModifyHeight || bModifyBotmZPos)
//				{
//					float fZMin = 1000000.0f;
//					for (int i = 0; i < nNumVer; i++)///�ȵõ���С
//					{
//						if ( pVers[i].Pos.y < fZMin - 0.1f )///����==�ıȽ�
//						{
//							fZMin = pVers[i].Pos.y;
//						}
//					}
//					for (int i = 0; i < nNumVer; i++)///�����ļӵ��ߵ�λ����
//					{
//						if ( pVers[i].Pos.y > fZMin + 0.1f )///����==�ıȽ�
//						{
//							pVers[i].Pos.y = fBotmZPos + fHeight;
//						}
//						else
//						{
//							pVers[i].Pos.y = fBotmZPos;
//						}
//					}///End of for
//					///�޸�BBox������,ԭ����λ��Ӧ����fZMin+m_nHeight/2�����ڵ�λ����fBotmZPos+fHeight/2������͵õ��˲�ֵ
//					float fBBoxCenterBias = (fBotmZPos-fZMin)+(fHeight-m_DescStruct.m_fHeight)/2; 
//					m_BBox.A.y += fBBoxCenterBias;
//					m_BBox.B.y += fBBoxCenterBias;
//					Center = m_BBox.GetCenter();///���¼�������
//					///��Height�Ļ�ҲӦ��˳��ĵ����ںϸ߶�
//					float fNewBotmBlendHeight = fHeight * m_DescStruct.m_fBotmBlendHeight / m_DescStruct.m_fHeight; 
//					m_DescStruct.m_fHeight = fHeight;
//					m_DescStruct.m_fBotmZPos = fBotmZPos;
//
//					ModifyTexture(NULL, &fNewBotmBlendHeight);
//				}///End of if
//			}///�ĸ߶Ȳ�����
//
//			//����ת
//			if (pfZRotation)
//			{
//                D3DXMATRIX matTrans;
//                D3DXQUATERNION quatRotation;
//                D3DXQuaternionRotationAxis ( &quatRotation, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ), D3DXToRadian((*pfZRotation - m_DescStruct.m_fZRotation) ) );
//                D3DXMatrixTransformation( &matTrans, NULL, NULL, NULL, &Center, &quatRotation, NULL );
//                for (int i = 0; i < nNumVer; i++)///�ȵõ���С
//                {
//                    D3DXVec3TransformCoord( &pVers[i].Pos, &pVers[i].Pos, &matTrans );
//                }
//                m_DescStruct.m_fZRotation = (*pfZRotation);
//			}
//
//			//��U�ظ�����
//			if ( pfTextureCordUTiles )
//			{
//				if ( 
//					( (*pfTextureCordUTiles) != m_DescStruct.m_fTexCordUTiles)
//					&& (*pfTextureCordUTiles)>0
//					)
//				{
//					float fUMin = 1000.0f, fUMax = -1000.0f, fUTilesOld = m_DescStruct.m_fTexCordUTiles; 
//					for (int i = 0; i < nNumVer; i++)///�ȵõ���СU�����U
//					{
//						if ( pVers[i].Tex.x < fUMin )
//						{
//							fUMin = pVers[i].Tex.x;
//						}
//						if ( pVers[i].Tex.x > fUMax )
//						{
//							fUMax = pVers[i].Tex.x; 
//						}				
//					}
//					fUTilesOld = fUMax - fUMin;
//					float UScaleFactor = (*pfTextureCordUTiles) / fUTilesOld;
//					for (int i = 0; i < nNumVer; i++)///�ȵõ���СU�����U
//					{
//						pVers[i].Tex.x = ( pVers[i].Tex.x - fUMin )*UScaleFactor + fUMin;///һ���㷨	
//					}
//
//					///pVers[nNumVer-1].Tex.x = pVers[0].Tex.x+(*pfTextureCordUTiles);///��֤�����з�
//					m_DescStruct.m_fTexCordUTiles = (*pfTextureCordUTiles);
//				}
//			}
//		}///End of else 
//
//Exit1:
//		hrResult = S_OK;
//Exit0:
//        if (bLockVertexBuffer)
//        {
//            m_pD3DXMesh->UnlockVertexBuffer();
//        }
//
//		return hrResult;
//	}

//	HRESULT KG3DMeshFarMountain::ModifyTexture(
//        const D3DCOLOR* pColor, const float* pfBotmBlendHeight, const D3DCOLOR* pFogColor, const float* pfFogBlendFactor)
//	{
//        HRESULT hr = E_FAIL;
//        HRESULT hrResult = E_FAIL;
//
//        ASSERT(m_bIsInitialized);
//
//        KG_PROCESS_ERROR(m_bIsTextureLoaded && m_pColorMap);
//		KG_PROCESS_SUCCESS(!pColor && !pfBotmBlendHeight && !pFogColor && !pfFogBlendFactor);///ʲôҲ����
//		
//		{
//            LPDIRECT3DTEXTURE9 pD3DTexture = NULL;
//			D3DLOCKED_RECT rect;
//			PDWORD pPixels = NULL;
//            PDWORD pRectStart = NULL;
//			bool bNeedRecaculateLevels = false;///�����Ƿ���Ҫ���¼������MipmapLevel��ͨ��
//			DWORD dwResAlpha = 0; ///������ͼ�ϵ�����
//			int nTexWidth = 0;///int �� unsigned�Ĺ���Ч�ʸ�
//			int nTexHeight = 0;
//
//			///����������ͼ��ʱ������Ķ�m_nBotmBlendHeight�Ļ�����ı�Alphaͨ������Ҫ����ȡ��Alphaͨ����
//			if (pfBotmBlendHeight)
//			{
//				_ASSERTE(!m_bBlockReloadMessage );
//				m_bBlockReloadMessage = true;
//				m_pColorMap->ForceReload();
//				m_bBlockReloadMessage = false;
//				///���ʱ����Ҫ���������ͼ������Ժ�Ҫ�Ķ���ͼ����ɫ�ı�ķ�ʽ��ʱ��Ҫ�������
//			}
//
//			pD3DTexture =  m_pColorMap->GetTexture();
//			KGLOG_PROCESS_ERROR( pD3DTexture );
//
//			nTexWidth = (int)m_uTextureWidth;///int �� unsigned�Ĺ���Ч�ʸ�
//			nTexHeight = (int)m_uTextureHeight;
//
//			///��ɫӳ��
//			if (pColor)
//			{
//				m_DescStruct.m_d3dc_Color = *pColor;
//			}
//
//			while (pfBotmBlendHeight)///���Կٿյײ�
//			{
//				///�õ���Ҫ�޸ĵ����صķ�Χ
//
//				_ASSERTE( 0 < m_DescStruct.m_fHeight );
//
//
//				float fTempHeight = *pfBotmBlendHeight;
//				if(fTempHeight < FLT_EPSILON)
//				{
//					m_DescStruct.m_fBotmBlendHeight = 0;
//					break;
//				}
//
//				int nHeightInTexture = static_cast<int>(static_cast<float>(nTexHeight) * fTempHeight/m_DescStruct.m_fHeight);	//����������	
//				_ASSERTE( nHeightInTexture <= (nTexHeight) );///���ֻ�ܿٿ�һ��
//
//
//				int nBlendUpperBound = nTexHeight - nHeightInTexture;///�趨�߽�
//				int nBlendLowerBound = nTexHeight;
//
//				if ( nBlendLowerBound <= nBlendUpperBound )
//					break;
//				const float fAlphaDelta = (float)( ( 0.0f - 255.0f ) / ( (float)nBlendLowerBound - (float)nBlendUpperBound ) );
//
//				float fTempAlpha = 255.0f;///һ��ʼ�ǲ�͸����
//
//
//				hr = pD3DTexture->LockRect( 0, &rect, NULL, 0 );
//				KGLOG_COM_PROCESS_ERROR(hr);
//
//				pPixels = pRectStart = (PDWORD)rect.pBits;
//
//				int nY = 0, nX = 0;
//
//				//���Կٿյײ�
//				//����ǰ�������
//				nY = nBlendUpperBound;
//				pPixels = pRectStart;
//				pPixels += nTexWidth*nBlendUpperBound;
//
//				for ( ; nY < nBlendLowerBound; nY++ )
//				{
//					fTempAlpha += fAlphaDelta;
//					dwResAlpha = (DWORD)fTempAlpha;
//					dwResAlpha <<= 24;
//
//					for ( nX = 0; nX < nTexWidth; nX++, pPixels++ )
//					{
//						D3DCOLOR TempColor = *pPixels;
//						DWORD dwAlpha = TempColor & 0xff000000;
//						if(dwAlpha <= dwResAlpha)	//��������͸��������
//							continue;
//
//						(*pPixels) &= 0x00ffffff;//������λ						
//						(*pPixels) |= dwResAlpha;
//					}
//				}//End of For
//
//				bNeedRecaculateLevels = true;
//
//				pD3DTexture->UnlockRect(0);
//
//				m_DescStruct.m_fBotmBlendHeight = fTempHeight;
//				break;
//			}//while (pnBotmBlendHeight)
//			///�����Fog��ɫ���
//			if (pFogColor)
//			{
//				m_DescStruct.m_d3dc_FogColor = *pFogColor;
//			}
//			if(pfFogBlendFactor)
//			{
//				m_DescStruct.m_fFogBlendFactor = *pfFogBlendFactor;
//			}
//			///����m_d3dc_FogBlendedColor///�������ɫ�����Render�еĹ̶�����ȷ��
//			ModifyFinalBlendedColor(m_DescStruct.m_d3dc_Color, m_DescStruct.m_d3dc_FogColor, m_DescStruct.m_fFogBlendFactor);///���ջ��
//
//			///���¼���MipMap
//			if (bNeedRecaculateLevels)
//			{
//                hr = D3DXFilterTexture(pD3DTexture, NULL, D3DX_DEFAULT, D3DX_DEFAULT);
//				KGLOG_COM_PROCESS_ERROR(hr);
//			}
//		}
//Exit1:
//		hrResult = S_OK;
//Exit0:
//		return hrResult;
//	}

	/*void KG3DMeshFarMountain::ModifyFinalBlendedColor( D3DCOLOR SurfaceColor, D3DCOLOR FogColor, float fPercent )
	{
		TwoColorBlend(SurfaceColor, FogColor, m_d3dc_FogBlendedColor, fPercent);
	}*/

//	HRESULT KG3DMeshFarMountain::SetTexture( LPCTSTR tczTexturePath )
//	{
//		HRESULT hr = E_FAIL;
//		KG_PROCESS_ERROR( tczTexturePath && MAX_PATH > _tcslen(tczTexturePath));
//		///��Ⱦ���������������
//		if ( NULL != m_DescStruct.m_tczTextureFullPath )
//		{
//			if( 0 == _tcscmp( tczTexturePath, m_DescStruct.m_tczTextureFullPath ) )
//				return S_FALSE;///�ɹ�����������
//		}	
//		if ( NULL == m_pColorMap )
//		{
//			hr = g_cTextureTable.Get1NewResource(&m_pColorMap, 0, 0);
//			KG_COM_PROCESS_ERROR( hr );
//			_ASSERTE( m_pColorMap );
//			m_pColorMap->GetMessageSender()->RegisterReceiver(this);
//		}
//		_ASSERTE( m_pColorMap );
//
//		hr = m_pColorMap->LoadFromFile(tczTexturePath, 0, TEXOP_DISABLEDTX | TEXOP_COMMON | TEXOP_RELOAD);
//		KG_COM_PROCESS_ERROR( hr );
//
//		_ASSERTE( m_pColorMap->GetFormat() == D3DFMT_A8R8G8B8 );
//		hr = m_pColorMap->GetSize( &m_uTextureWidth, &m_uTextureHeight );
//		///�������·��
//		_tcscpy_s( m_tczTexturePathBuffer, MAX_PATH, tczTexturePath );
//		m_DescStruct.m_tczTextureFullPath = m_tczTexturePathBuffer;///����Ψһ�ܸı�·��ָ��ķ���
//
//		//Exit1:
//		//m_bIsTextureLoaded = true;
//		return S_OK;
//Exit0:
//		return E_FAIL;
//
//	}
	
	HRESULT KG3DMeshFarMountain::RequestModification( const KG3DMeshFarMountDescStruct* pStruct )
	{
		HRESULT hr = E_FAIL;
        HRESULT hrResult = E_FAIL;

		if (!m_bIsInitialized)///�ӳٵĳ�ʼ��
		{
            hr = CustomInit();
			KG_COM_PROCESS_ERROR(hr);
		}

		if (! pStruct)//����Ĭ��
		{
			///����Ĭ�ϲ���
			m_DescStruct.SetDefaultValue();//��Ȼ��Ĭ�ϵĹ��캯����Ҳ����Ĭ�ϲ������������Ժ���ܻ��������������Ĭ�ϲ������룬�������������

			SAFE_RELEASE(m_pColorMap);
			hr = g_cTextureTable.LoadResourceFromFile(KG3DMESHFARMOUNTAIN_TEXTURE_PATH, 0, TEXOP_DISABLEDTX | TEXOP_COMMON | TEXOP_RELOAD | TEXOP_DISABLEMIPMAP, (IKG3DResourceBase**)&m_pColorMap);
			KG_COM_PROCESS_ERROR(hr);

			//		KG_COM_PROCESS_ERROR( hr );
			//
			//		_ASSERTE( m_pColorMap->GetFormat() == D3DFMT_A8R8G8B8 );



		/*	hr = ModifyMesh(&TempStruct.m_fRadius, &TempStruct.m_fHeight, &TempStruct.m_fBotmZPos, &TempStruct.m_fZRotation, &TempStruct.m_fTexCordUTiles);
			KGLOG_COM_PROCESS_ERROR(hr);*/

			/*hr = SetTexture(KG3DMESHFARMOUNTAIN_TEXTURE_PATH);
			KGLOG_COM_PROCESS_ERROR(hr);*/

			/*hr = ModifyTexture(&TempStruct.m_d3dc_Color, &TempStruct.m_fBotmBlendHeight, &TempStruct.m_d3dc_FogColor, &TempStruct.m_fFogBlendFactor);
			KGLOG_COM_PROCESS_ERROR(hr);*/
		}
        else
        {
            DWORD dwMDOption = pStruct->m_dwModificationOption ;
			KG3DMeshFarMountDescStruct tmpDesc = m_DescStruct;
            if( dwMDOption & KG3DMESHFARMOUNTAIN_MDO_MESH )
            {
				KG_PROCESS_ERROR(pStruct->m_fRadius > 0);
				tmpDesc.m_fRadius = pStruct->m_fRadius;
				KG_PROCESS_ERROR(pStruct->m_fHeight > 0);
				tmpDesc.m_fHeight = pStruct->m_fHeight;
				
				tmpDesc.m_fBotmZPos = pStruct->m_fBotmZPos;

				tmpDesc.m_fZRotation = fmod(pStruct->m_fZRotation, 360);

				tmpDesc.m_fTexCordUTiles = pStruct->m_fTexCordUTiles;
                /*FLOAT fRadius = pStruct->m_fRadius;
                FLOAT* pRadius = NULL;
                if (MT_FLOAT_UNEQUAL(fRadius, m_DescStruct.m_fRadius) && fRadius > 0)
                    pRadius = &fRadius;

                FLOAT fHeight = pStruct->m_fHeight;
                FLOAT* pHeight = NULL;
                if(MT_FLOAT_UNEQUAL(fHeight,m_DescStruct.m_fHeight) && fHeight > 0)
                    pHeight = &fHeight;

                FLOAT fBotmZPos = pStruct->m_fBotmZPos;
                FLOAT* pBotmZPos = NULL;
                if(MT_FLOAT_UNEQUAL(fBotmZPos, m_DescStruct.m_fBotmZPos))
                    pBotmZPos = &fBotmZPos;

                float fZRotation = pStruct->m_fZRotation;
                float* pfZRotation = NULL;
                if(MT_FLOAT_UNEQUAL(fZRotation, m_DescStruct.m_fZRotation))
                    pfZRotation = &fZRotation;

                float fTexCoordUTiles = pStruct->m_fTexCordUTiles;
                float* pfTexCoordUTiles = NULL;
                if(MT_FLOAT_UNEQUAL(fTexCoordUTiles, m_DescStruct.m_fTexCordUTiles))
                    pfTexCoordUTiles = &fTexCoordUTiles;*/

               /* hr = ModifyMesh(pRadius, pHeight, pBotmZPos, pfZRotation, pfTexCoordUTiles);
                KGLOG_COM_PROCESS_ERROR(hr);*/
            }

            if ( (dwMDOption & KG3DMESHFARMOUNTAIN_MDO_TEXPATH) && 0 != _tcslen(pStruct->m_strTextureFullPath) )
            {
                //hr = SetTexture( pStruct->m_tczTextureFullPath );
				SAFE_RELEASE(m_pColorMap);
				hr = g_cTextureTable.LoadResourceFromFile(pStruct->m_strTextureFullPath, 0, TEXOP_DISABLEDTX | TEXOP_COMMON | TEXOP_RELOAD | TEXOP_DISABLEMIPMAP, (IKG3DResourceBase**)&m_pColorMap);
                KGLOG_COM_PROCESS_ERROR(hr);
				_tcscpy_s(tmpDesc.m_strTextureFullPath, pStruct->m_strTextureFullPath);

                //��ͼ��������,����ͼ���������ز���ҲҪ������һ��
                //{
                //    if ( pStruct->m_dwModificationOption & KG3DMESHFARMOUNTAIN_MDO_TEX )
                //    {
                //        hr = ModifyTexture(&pStruct->m_d3dc_Color, &pStruct->m_fBotmBlendHeight, 
                //            &pStruct->m_d3dc_FogColor, &pStruct->m_fFogBlendFactor);//ǿ���ػ�
                //        KGLOG_COM_PROCESS_ERROR(hr);
                //    }
                //    else
                //    {
                //        hr = ModifyTexture(&m_DescStruct.m_d3dc_Color, &m_DescStruct.m_fBotmBlendHeight, 
                //            &m_DescStruct.m_d3dc_FogColor, &m_DescStruct.m_fFogBlendFactor);//ǿ���ػ�
                //        KGLOG_COM_PROCESS_ERROR(hr);
                //    }
                //}
            }//�������ͼ·���Ļ���������ͼ��Ȼ�ػ���ͼ������Ҫelse
            
			if (dwMDOption & KG3DMESHFARMOUNTAIN_MDO_TEX )//�����͹�ı���ͼ
            {
				KG_PROCESS_ERROR(pStruct->m_fBotmBlendHeight >= 0);
				tmpDesc.m_fBotmBlendHeight = pStruct->m_fBotmBlendHeight;

				tmpDesc.m_d3dc_FogColor = pStruct->m_d3dc_FogColor;

				KG_PROCESS_ERROR(pStruct->m_fFogBlendFactor >= 0 && pStruct->m_fFogBlendFactor <= 1);
				tmpDesc.m_fFogBlendFactor = pStruct->m_fFogBlendFactor;

               /* FLOAT fBotmBlendHeight = pStruct->m_fBotmBlendHeight;
                FLOAT* pBBH = NULL;
                if (MT_FLOAT_UNEQUAL(fBotmBlendHeight, m_DescStruct.m_fBotmBlendHeight) 
                    && fBotmBlendHeight > -FLT_EPSILON && fBotmBlendHeight <= m_DescStruct.m_fHeight)///�����ƻ�����ɽ������ͨ��
                {
                    pBBH = &fBotmBlendHeight;
                }

                DWORD FogColor = pStruct->m_d3dc_FogColor;
                DWORD* pFogColor = NULL;
                if (FogColor != m_DescStruct.m_d3dc_FogColor)
                {
                    pFogColor = &FogColor;
                }
                float FogBlendFactor = pStruct->m_fFogBlendFactor;
                float* pFogBlendFactor = NULL;
                if (FogBlendFactor != m_DescStruct.m_fFogBlendFactor
                    && FogBlendFactor >=0 && FogBlendFactor <= 1)
                {
                    pFogBlendFactor = &FogBlendFactor;
                }*/

                /*hr = ModifyTexture(pStruct->m_d3dc_Color != m_DescStruct.m_d3dc_Color ? &pStruct->m_d3dc_Color : NULL, 
                    pBBH, pFogColor, pFogBlendFactor);		
                KGLOG_COM_PROCESS_ERROR(hr);*/
            }
            /*
            if (dwMDOption & KG3DMESHFARMOUNTAIN_MDO_SYNWITHFOG )
            {
            BOOL bEnable = (pStruct->m_bEnableSynWithFogColor == true)?TRUE:FALSE;
            BOOL bResult = FALSE;
            bResult = EnableSynWithFogColor( bEnable, &(pStruct->m_d3dc_Color) );
            KG_PROCESS_ERROR(bResult == bEnable);
            }*/

            if (dwMDOption & KG3DMESHFARMOUNTAIN_MDO_ENABLETEXCOLOR)
            {
                tmpDesc.m_bEnableTexColor = pStruct->m_bEnableTexColor;
            }
            if (dwMDOption & KG3DMESHFARMOUNTAIN_MDO_SPEED)
            {
                KG_PROCESS_ERROR( pStruct->m_fSpeed > -FLT_EPSILON);
                tmpDesc.m_fSpeed = pStruct->m_fSpeed;
            }

            if (dwMDOption & KG3DMESHFARMOUNTAIN_MDO_TEXFACTOR)
            {
                tmpDesc.m_fTexFactor = pStruct->m_fTexFactor;
            }

			m_DescStruct = tmpDesc;
        }

		hrResult = S_OK;
Exit0:
		return hrResult;
	}

	HRESULT KG3DMeshFarMountain::SaveMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
	{
		_ASSERTE(pIniFile);
		_ASSERTE(pSection);
		pIniFile->WriteString( pSection, "Name", m_tczName );
		m_DescStruct.SaveMapSetting(pIniFile, pSection);

		return S_OK;
	}

	HRESULT KG3DMeshFarMountain::LoadMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
	{
		HRESULT hr = E_FAIL;
        HRESULT hrResult = E_FAIL;
        int nRetCode = false;
		KG3DMeshFarMountDescStruct tempStruct;

        if (pIniFile)
        {
            _ASSERTE(pIniFile);
            _ASSERTE(pSection);

            nRetCode = KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, _T("Name"), _T(""), m_tczName);
			//KGLOG_PROCESS_ERROR(nRetCode);

            nRetCode = tempStruct.LoadMapSetting(pIniFile, pSection);
            KGLOG_PROCESS_ERROR(nRetCode);

            tempStruct.m_dwModificationOption = KG3DMESHFARMOUNTAIN_MDO_ALL;  

            hr = RequestModification(&tempStruct);
            KGLOG_COM_PROCESS_ERROR(hr);
        }
        else
		{
			hr = RequestModification((const KG3DMeshFarMountDescStruct*)NULL);
            KGLOG_COM_PROCESS_ERROR(hr);
		}

		hrResult = S_OK;
Exit0:
        if (FAILED(hrResult))
        {
		    _KG3D_DEBUG_REPORT("KG3DMeshFarmount��ȡʧ��");
        }
		return hrResult;
	}

	HRESULT KG3DMeshFarMountain::FrameMove()
	{
		return S_OK;
	}
	/*	
	KG3DMeshFarMountain::RenderState::RenderState()
		:m_AlphaState()
		,m_AddressVClamp(0)
	{
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR );
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	}*/

	//KG3DMeshFarMountain::RenderState::~RenderState()
	//{
	//	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);//��ԭĬ��
	//}
	HRESULT KG3DMeshFarMountain::Render(const KG3DEnvEffRenderContext& renderContext)
	{

		HRESULT hr = E_FAIL;
		KG_PROCESS_ERROR(NULL != m_pColorMap /*&& NULL != m_pD3DXMesh*/ && NULL != m_pVB);

#if 0
        m_pColorMap->SetTexture(g_pd3dDevice, 0);

        if (! m_DescStruct.m_bEnableTexColor)
        {
            ///�������Ӧ�ñ�֤A��DescStruct��Ӧ��Key��B��SubKey��Ȼ����Percent��ʱ��ֵ
            ///������ʲô�������٣���Ӧ�ñ�֤�������
            ///ʹ�õ�ɫ��ColorOp
            if (m_bEnableBlending && m_bIsSubStateInitialized)
            {
                KG3DDWGC_Key_FarMount TempKey;
                KG3DDWGC_Key_FarMount MainKey;
                m_DescStruct.ToKey(MainKey);
                TempKey.CalculateInterp(MainKey, m_SubKey, m_fBlendPercent);
                ModifyFinalBlendedColor(TempKey.m_d3dc_Color, m_DescStruct.m_d3dc_FogColor, TempKey.m_fFogBlendFactor);
                _KG3D_DEBUG_SELF_ASSERTE(TempKey.m_fFogBlendFactor >= 0.0f&&TempKey.m_fFogBlendFactor <= 1.0f);
            }
            g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR );

            g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, (DWORD)m_d3dc_FogBlendedColor );

        }
        else	//���������ͼ��ɫ��ֱ��ʹ����ͼ����Ⱦ�ͺ��ˣ������������ʹ��TextureFactor������
        {
            g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        }

        if (m_DescStruct.m_fSpeed > 0)///��һ����ת
        {
            float fAngle = g_cGraphicsTool.GetNowTime()/1000.0f * m_DescStruct.m_fSpeed;
            fAngle = fmod(fAngle, 360.0f);

            D3DXMATRIX matRot;
            D3DXMatrixRotationY(&matRot, D3DXToRadian(fAngle));

            D3DXMATRIX matSave;
            g_pd3dDevice->GetTransform(D3DTS_WORLD, &matSave);

            D3DXMATRIX matTemp = matRot * matSave; 
            g_pd3dDevice->SetTransform(D3DTS_WORLD, &matTemp);

            m_pD3DXMesh->DrawSubset(0);

            g_pd3dDevice->SetTransform(D3DTS_WORLD, &matSave);
        }
        else
        {
            m_pD3DXMesh->DrawSubset(0);
        }
#endif

		{
			LPD3DXEFFECT	pShader = g_GetDefaultShaderGroup().GetDefaultEffect(DEFST_FARMOUNTAIN);
			_ASSERTE(NULL != pShader);

			D3DXCOLOR	surfaceColor;
			{
				if(! m_DescStruct.m_bEnableTexColor)
				{
					///�������Ӧ�ñ�֤A��DescStruct��Ӧ��Key��B��SubKey��Ȼ����Percent��ʱ��ֵ
					///������ʲô�������٣���Ӧ�ñ�֤�������
					///ʹ�õ�ɫ��ColorOp
					FLOAT fBlendPercent = 0;
					if (m_bEnableBlending)
					{
						KG3DDWGC_Key_FarMount TempKey;
						KG3DDWGC_Key_FarMount MainKey;
						m_DescStruct.ToKey(MainKey);
						TempKey.CalculateInterp(MainKey, m_SubKey, m_fBlendPercent);
						_KG3D_DEBUG_SELF_ASSERTE(TempKey.m_fFogBlendFactor >= 0.0f && TempKey.m_fFogBlendFactor <= 1.0f);
						fBlendPercent = TempKey.m_fFogBlendFactor;
					}
					else
					{
						fBlendPercent = m_DescStruct.m_fFogBlendFactor;
					}
					surfaceColor = renderContext.vFogColor * (1 - fBlendPercent) + D3DXCOLOR(m_DescStruct.m_d3dc_FogColor) * fBlendPercent;
				}
				else
				{
					surfaceColor = D3DXCOLOR(0,0,0,0);
				}
			}
			

			//float4x4	matViewProj;
			//float3		vColor;
			//float4x4	matAnimation;
			//float3		vCamPos;

			//float		fRadius;
			//float		fZMin;
			//float		fHeight;
			//float		fUTiles;
			//float		fEnableTex;
			//float2x2	matZRotation;

			//float3		vBlendColor;
			//float		fBlendFactor;

			ShaderVariantsFarMountain	shaderVariants;
            shaderVariants.fTexFactor  = m_DescStruct.m_fTexFactor;
			shaderVariants.matViewProj = renderContext.matViewProj;
			shaderVariants.vColor = D3DXVECTOR3(surfaceColor.r, surfaceColor.g, surfaceColor.b);
			if (m_DescStruct.m_fSpeed != 0)
			{
				float fAngle = g_cGraphicsTool.GetNowTime()/1000.0f * m_DescStruct.m_fSpeed;
				fAngle = fmod(fAngle, 360.0f);

				D3DXMatrixRotationY(&shaderVariants.matAnimation, D3DXToRadian(fAngle));
			}
			else
			{
				shaderVariants.matAnimation = D3DXMatrixGetIdentity();
			}
			shaderVariants.vCamPos = renderContext.vCamPos;

			shaderVariants.fRadius = m_DescStruct.m_fRadius;
			shaderVariants.fZMin = m_DescStruct.m_fBotmZPos;
			shaderVariants.fHeight = m_DescStruct.m_fHeight;
			shaderVariants.fUTiles = m_DescStruct.m_fTexCordUTiles;
			shaderVariants.fEnableTex = m_DescStruct.m_bEnableTexColor ? 1.f : 0.f;
			D3DXMatrixRotationY(&shaderVariants.matZRotation, D3DXToRadian(m_DescStruct.m_fZRotation));
			shaderVariants.fBotmFadePercent = m_DescStruct.m_fBotmBlendHeight / m_DescStruct.m_fHeight;

			D3DXCOLOR vBlendColor = m_DescStruct.m_d3dc_FogColor;
			shaderVariants.vBlendColor = D3DXVECTOR3(vBlendColor.r, vBlendColor.g, vBlendColor.b);
			shaderVariants.fBlendFactor = m_DescStruct.m_fFogBlendFactor;
			
			HrV(pShader->SetValue("g_shaderVariants", &shaderVariants, sizeof(shaderVariants)));
			HrV(pShader->SetTexture("g_texMain", m_pColorMap->GetTexture()));

			g_pd3dDevice->SetFVF(em_fvf);
			g_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(TypeVertex));
		
			if(SUCCEEDED(pShader->Begin(NULL, 0)))
			{
				if(SUCCEEDED(pShader->BeginPass(0)))
				{
					g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, em_slice * 2);
					pShader->EndPass();
				}
				pShader->End();
			}
		}

		return S_OK;
Exit0:
		return E_FAIL;
	}

//	void __stdcall KG3DMeshFarMountain::OnReceiveMessage( const KG3DMessage& Message ,IKG3DMessageSenderBase* pSender )
//	{
//        HRESULT hr = E_FAIL;
//        HRESULT hrResult = E_FAIL;
//
//		if ( Message.m_uMessage == KM_TEXTURE_RELOADED && m_bBlockReloadMessage != true )
//		{
//            if (!m_bIsInitialized)///�ӳٵĳ�ʼ��
//            {
//                hr = CustomInit();
//                KGLOG_COM_PROCESS_ERROR(hr);
//            }	
//
//			//m_pColorMap->GetSize( &m_uTextureWidth, &m_uTextureHeight );
//
//			//hr = ModifyTexture(&m_DescStruct.m_d3dc_Color, &m_DescStruct.m_fBotmBlendHeight, &m_DescStruct.m_d3dc_FogColor, &m_DescStruct.m_fFogBlendFactor); //��ͼ��Reload�����������ˣ�ǿ���ػ�
//   //         KGLOG_COM_PROCESS_ERROR(hr);
//		}
//		//KG3DMessageSingleSenderReceiver::OnReceiveMessage(Message, pSender);
//
//        hrResult = S_OK;
//Exit0:
//        if (FAILED(hrResult))
//        {
//        }
//	}

	//���ڶ�̬�����Ĳ���

	HRESULT KG3DMeshFarMountain::ToSolidInterface( LPVOID* pEffectInterface )
	{
		if(!pEffectInterface)
		{
			return E_INVALIDARG;
		}
		IEKG3DMeshFarMountain* pInterface = this;
		*pEffectInterface = static_cast<LPVOID>(pInterface);
		return S_OK;
	}

	///<DW_Controller>
	HRESULT KG3DMeshFarMountain::DWGCRequestPopFrontState(float fCurBlendPercent/* = 0.0f*/)
	{	
		if (fCurBlendPercent > 0 && fCurBlendPercent < 1.0f)
		{
			KG3DDWGC_Key_FarMount MidKey, BeforeKey;
			m_DescStruct.ToKey(BeforeKey);
			MidKey.CalculateInterp(BeforeKey, m_SubKey, fCurBlendPercent);
			m_DescStruct.SynWithKey(MidKey);
		}
		else
		{
			m_DescStruct.SynWithKey(m_SubKey);	
		}
			
		_KG3D_DEBUG_SELF_OUTPUT0(_STRINGER(KG3DMeshFarMountain)":"_STRINGER(DWGCRequestPopFrontState)"\n");
		return S_OK;
	}

	HRESULT KG3DMeshFarMountain::DWGCGetCurStateToKey( KG3DDWGC_KeyBase* pRetKey)
	{
		TypeKey* pSolidKey = dynamic_cast<TypeKey*>(pRetKey);
		KG_PROCESS_ERROR(pSolidKey);
		{
			m_DescStruct.ToKey(*pSolidKey);
			return S_OK;
		}
Exit0:
		return E_FAIL;
	}

	HRESULT KG3DMeshFarMountain::DWGCRequestSubStateChange(const KG3DDWGC_KeyBase* pRetKey)
	{
		_KG3D_DEBUG_SELF_OUTPUT0(_STRINGER(KG3DMeshFarMountain)":"_STRINGER(DWGCRequestSubStateChange)"\n");
		const TypeKey* pSolidKey = dynamic_cast<const TypeKey*>(pRetKey);
		KG_PROCESS_ERROR(pSolidKey);
		m_SubKey = *pSolidKey;
		//m_bIsSubStateInitialized = true;
		return S_OK;
Exit0:
		return E_FAIL;
	}

	HRESULT KG3DMeshFarMountain::DWGCEnableSubState( BOOL bEnable )
	{
		m_bEnableBlending = bEnable?true:false;
		//if (m_bEnableBlending)
		//{
		//	//m_DescStruct.ToKey(&m_MainKey);			
		//}
		//else
		//{
		//	ModifyFinalBlendedColor(m_DescStruct.m_d3dc_Color, m_DescStruct.m_d3dc_FogColor, m_DescStruct.m_fFogBlendFactor);
		//}
		
		return S_OK;
	}

	HRESULT KG3DMeshFarMountain::DWGCSetPercent( float fPercent )
	{
		m_fBlendPercent = fPercent;
		return S_OK;
	}

	HRESULT KG3DMeshFarMountain::DWGCRequestModification( const KG3DDWGC_KeyBase* pKey)
	{
		const KG3DDWGC_Key_FarMount* pKeyFarMount = dynamic_cast<const KG3DDWGC_Key_FarMount*>(pKey);
		if(!pKeyFarMount)
			return E_FAIL;
		KG3DMeshFarMountDescStruct TempStruct = m_DescStruct;
		TempStruct.SynWithKey(*pKeyFarMount);
		TempStruct.m_dwModificationOption = KG3DMESHFARMOUNTAIN_MDO_TEX;
		return RequestModification((KG3DMeshFarMountDescStruct*)&TempStruct);
	}
	///</DW_Controller>
	
	/************************************************************************/
	//�ӿڶ��忪ʼ                                                                      
	/************************************************************************/

	HRESULT KG3DMeshFarMountain::RequestModification( const KG3DMESHFARMOUNTAIN_DESCSTRUCT* pStruct )
	{
		if( NULL == pStruct)
			return E_FAIL;
		return RequestModification( (KG3DMeshFarMountDescStruct*)pStruct );
	}

	
	HRESULT KG3DMeshFarMountain::GetStruct( KG3DMESHFARMOUNTAIN_DESCSTRUCT* pResultStruct )
	{
		if (NULL == pResultStruct)
		{
			return E_FAIL;
		}
		m_DescStruct.ToBaseStruct( *pResultStruct );
		return S_OK;
	}

	HRESULT KG3DMeshFarMountain::GetName( LPTSTR szName, UINT uBufferCount )
	{
		if (szName == NULL || uBufferCount < _tcslen(m_tczName))
		{
			return E_FAIL;
		}
		_tcscpy_s(szName, uBufferCount, m_tczName);
		return S_OK;
	}

	HRESULT KG3DMeshFarMountain::SetName( LPTSTR szName )
	{
		if ( NULL == szName && countof(m_tczName) < _tcslen(szName) )
		{
			return E_FAIL;
		}
		_tcscpy_s(m_tczName, countof(m_tczName), szName);
		return S_OK;
	}
	
	/************************************************************************/
	//KG3DMeshFarMountDescStruct���忪ʼ
	/************************************************************************/
	KG3DMeshFarMountDescStruct::KG3DMeshFarMountDescStruct()
	{
		this->SetDefaultValue();	

		//m_tczTextureFullPath = NULL;
	}

	KG3DMeshFarMountDescStruct::KG3DMeshFarMountDescStruct( const KG3DMeshFarMountDescStruct &OtherStruct )
	{
		//m_tczTextureFullPath = NULL;///��������Ȱ����ָ���һ��Buffer�Ļ������´���ַ�
		operator=( OtherStruct );
	}

	KG3DMeshFarMountDescStruct::KG3DMeshFarMountDescStruct( const KG3DMESHFARMOUNTAIN_DESCSTRUCT &OtherStruct )
	{
		//m_tczTextureFullPath = NULL;
		operator = ( OtherStruct );
	}

	void KG3DMeshFarMountDescStruct::operator=( const KG3DMeshFarMountDescStruct& OtherStruct )
	{
		if ( &OtherStruct == this )
		{
			return;
		}
		//LPTSTR pTemp = m_tczTextureFullPath;///������ָ��
		memcpy_s( this, sizeof(KG3DMeshFarMountDescStruct), &OtherStruct, sizeof(KG3DMeshFarMountDescStruct) );
		//m_tczTextureFullPath = pTemp;
		//if( NULL != m_tczTextureFullPath && NULL != OtherStruct.m_tczTextureFullPath )
		//	_tcscpy_s( m_tczTextureFullPath, MAX_PATH, OtherStruct.m_tczTextureFullPath );
	}

	void KG3DMeshFarMountDescStruct::operator=( const KG3DMESHFARMOUNTAIN_DESCSTRUCT& OtherStruct )
	{
		if ( &OtherStruct == this )
		{
			return;
		}
		//LPTSTR pTemp = m_tczTextureFullPath;///������ָ��
		memcpy_s( this, sizeof(KG3DMeshFarMountDescStruct), &OtherStruct, sizeof(KG3DMESHFARMOUNTAIN_DESCSTRUCT) );
		/*m_tczTextureFullPath = pTemp;
		if( NULL != m_tczTextureFullPath && NULL != OtherStruct.m_tczTextureFullPath )
			_tcscpy_s( m_tczTextureFullPath, MAX_PATH, OtherStruct.m_tczTextureFullPath );*/
	}

	void KG3DMeshFarMountDescStruct::ToBaseStruct( KG3DMESHFARMOUNTAIN_DESCSTRUCT &OtherStruct )
	{
		if ( &OtherStruct == this )
		{
			return;
		}
		//LPTSTR pTemp = OtherStruct.m_tczTextureFullPath;///������ָ��
		memcpy_s( &OtherStruct, sizeof(KG3DMESHFARMOUNTAIN_DESCSTRUCT), this, sizeof(KG3DMESHFARMOUNTAIN_DESCSTRUCT) );
		/*OtherStruct.m_tczTextureFullPath = pTemp;
		if( NULL != m_tczTextureFullPath && NULL != OtherStruct.m_tczTextureFullPath )
			_tcscpy_s( OtherStruct.m_tczTextureFullPath, MAX_PATH, m_tczTextureFullPath );*/
	}

	bool KG3DMeshFarMountDescStruct::SetDefaultValue()
	{
		m_dwModificationOption = KG3DMESHFARMOUNTAIN_MDO_DEFAULT;
		m_fRadius = 300000.0f;
		m_fHeight = 50000.0f;
		m_fBotmZPos = 0;
		m_fZRotation = 0.0f;
		m_fTexCordUTiles = 15.0f;

		//m_d3dc_Color = D3DCOLOR_XRGB(127, 194, 105);
		m_d3dc_FogColor = D3DCOLOR_XRGB( 255, 255, 255 );
		m_bEnableTexColor = false;
		m_fFogBlendFactor = 0.0f;
		m_fBotmBlendHeight = 0;

		m_fSpeed = 0;
        m_fTexFactor = 1.0f;
		_tcscpy_s(m_strTextureFullPath, KG3DMESHFARMOUNTAIN_TEXTURE_PATH);
		return true;
	}

	bool KG3DMeshFarMountDescStruct::SaveMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
	{
		_ASSERTE(pIniFile&&pSection);
		pIniFile->WriteFloat(pSection, "Radius", m_fRadius);
		pIniFile->WriteFloat(pSection, "Height", m_fHeight);
		pIniFile->WriteFloat(pSection, "BotmZPos", m_fBotmZPos);
		pIniFile->WriteFloat(pSection, "ZRotation", m_fZRotation);
		pIniFile->WriteFloat(pSection, "TexCordUTiles", m_fTexCordUTiles);
		//KG_CUSTOM_HELPERS::SaveINIFileData<D3DCOLOR>(pIniFile, pSection, "Color", m_d3dc_Color);
		//pIniFile->WriteInteger(pSection, "Color", (int)m_d3dc_Color);
		//pIniFile->WriteInteger(pSection, "EnableSynWithFogColor", (int)m_bEnableSynWithFogColor);
		pIniFile->WriteInteger(pSection, "EnableTexColor", (int)m_bEnableTexColor);
		KG_CUSTOM_HELPERS::SaveINIFileData<D3DCOLOR>(pIniFile, pSection, "FogColor", m_d3dc_FogColor);
		//pIniFile->WriteInteger(pSection, "FogColor", (int)m_d3dc_FogColor);
		pIniFile->WriteFloat(pSection, "FogBlendFactor", m_fFogBlendFactor);
		pIniFile->WriteFloat(pSection, "BotmBlendHeight", m_fBotmBlendHeight);

		pIniFile->WriteFloat(pSection, "Speed", m_fSpeed);
        pIniFile->WriteFloat(pSection, "TexFactor", m_fTexFactor);

		//if (NULL != m_tczTextureFullPath)
		{
			pIniFile->WriteString( pSection, "TexturePath", m_strTextureFullPath );
		}

		return true;
	}

	bool KG3DMeshFarMountDescStruct::LoadMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
	{
		_ASSERTE(pIniFile&&pSection);
		pIniFile->GetFloat(pSection, "Radius", 0, &m_fRadius);
		pIniFile->GetFloat(pSection, "Height", 0, &m_fHeight);
		pIniFile->GetFloat(pSection, "BotmZPos", 0, &m_fBotmZPos);
		pIniFile->GetFloat(pSection, "ZRotation", 0.0f, &m_fZRotation);
		pIniFile->GetFloat(pSection, "TexCordUTiles", 0.0f, &m_fTexCordUTiles);
		//KG_CUSTOM_HELPERS::LoadINIFileData<D3DCOLOR>(pIniFile, pSection, "Color", 0x00000000, &m_d3dc_Color);
		//KG_CUSTOM_HELPERS::LoadINIFileData<bool>(pIniFile, pSection, "EnableSynWithFogColor", 0, &m_bEnableSynWithFogColor);
		KG_CUSTOM_HELPERS::LoadINIFileData<bool>(pIniFile, pSection, "EnableTexColor", 0, &m_bEnableTexColor);
		KG_CUSTOM_HELPERS::LoadINIFileData<D3DCOLOR>(pIniFile, pSection, "FogColor", 0x00000000, &m_d3dc_FogColor);
		pIniFile->GetFloat(pSection, "FogBlendFactor", -1.0f, &m_fFogBlendFactor);
		pIniFile->GetFloat(pSection, "BotmBlendHeight", 0, &m_fBotmBlendHeight);

		pIniFile->GetFloat(pSection, "Speed", 0, &m_fSpeed);
        pIniFile->GetFloat(pSection, "TexFactor", 1.0, &m_fTexFactor);

		if (m_fFogBlendFactor < 0)
		{
			return false;
		}
		//if (NULL != m_tczTextureFullPath)
		{
			pIniFile->GetString( pSection, "TexturePath", "", m_strTextureFullPath, MAX_PATH );
		}
		///��ʹ�����Ǵ��Ҳ��Ҫ����RequestModification��ʱ���������Ϸ��ԣ����Բ���ÿ����������ȡ��
		
		return true;
	}

	void KG3DMeshFarMountDescStruct::SynWithKey(const KG3DDWGC_Key_FarMount& Key )
	{
		//m_d3dc_Color = Key.m_d3dc_Color;
		m_fFogBlendFactor = Key.m_fFogBlendFactor;
	}

	void KG3DMeshFarMountDescStruct::ToKey( KG3DDWGC_Key_FarMount& OtherStruct )
	{
		//OtherStruct.m_d3dc_Color = m_d3dc_Color;
		OtherStruct.m_fFogBlendFactor = m_fFogBlendFactor;
	}

	
	/************************************************************************/
	//�������� 
	/************************************************************************/
	bool KG3DDWGC_Key_FarMount::CalculateInterp(const KG3DDWGC_KeyBase& BaseKeyBefore
		, const KG3DDWGC_KeyBase& BaseKeyAfter
		//, const KG3DEnvEff_DWGC_Key_Struct_Base& BaseKeyResult
		, float fPercent)
	{
		KG3DDWGC_Key_FarMount& KeyBefore = (KG3DDWGC_Key_FarMount&)BaseKeyBefore;
		KG3DDWGC_Key_FarMount& KeyAfter = (KG3DDWGC_Key_FarMount&)BaseKeyAfter;
		//DWGC_KG3DMeshFarMount_Key_Struct& KeyResult = (DWGC_KG3DMeshFarMount_Key_Struct&)BaseKeyResult;
		if (fPercent <= 0.0 )
		{
			*this = KeyBefore;
			return true;
		}
		else if (fPercent >= 1.0)
		{
			*this = KeyAfter;
			return true;
		}
		//TwoColorBlend(KeyBefore.m_d3dc_Color, KeyAfter.m_d3dc_Color, this->m_d3dc_Color, fPercent);
		MT_FLOAT_BLEND(KeyBefore.m_fFogBlendFactor, KeyAfter.m_fFogBlendFactor, this->m_fFogBlendFactor, fPercent);
		return true;
	}

	bool KG3DDWGC_Key_FarMount::SaveMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
	{
		_ASSERTE(pIniFile&&pSection);
		//KG_CUSTOM_HELPERS::SaveINIFileData<D3DCOLOR>(pIniFile, pSection, "Color", m_d3dc_Color);
		//pIniFile->WriteInteger(pSection, "Color", (int)m_d3dc_FogColor);
		pIniFile->WriteFloat(pSection, "FogBlendFactor", m_fFogBlendFactor);
		return true;
	}

	bool KG3DDWGC_Key_FarMount::LoadMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
	{
		_ASSERTE(pIniFile&&pSection);
		//KG_CUSTOM_HELPERS::LoadINIFileData<D3DCOLOR>(pIniFile, pSection, "Color", 0x00000000, &m_d3dc_Color);
		pIniFile->GetFloat(pSection, "FogBlendFactor", -1.0f, &m_fFogBlendFactor);
		if (m_fFogBlendFactor < 0)
		{
			return false;///�÷Ƿ�ֵ�����������ȷ�ԣ��������ȼ��IsSectionExist��
		}
		return true;
	}

#if defined(DEBUG) | defined(_DEBUG)
	void KG3DDWGC_Key_FarMount::DebugPrint(std::ostringstream& oss)
	{
		/*
		_ASSERTE(pDebugStream);
				char czTemp[32];
				sprintf_s(czTemp, 32, "%x; ", m_d3dc_Color);
				_tcscat_s(pDebugStream, nSize, czTemp);
				nSize-=(int)strlen(czTemp);*/
		//oss << m_d3dc_Color <<"; ";	
	}
#endif

	KG3DDWGC_Key_FarMount::KG3DDWGC_Key_FarMount()
	{
		///ȡ��DescStruct��Ĭ��ֵ��
		m_dwModificationOption = 0;
		KG3DMeshFarMountDescStruct tempStruct;
		m_fFogBlendFactor = tempStruct.m_fFogBlendFactor;
	}

};