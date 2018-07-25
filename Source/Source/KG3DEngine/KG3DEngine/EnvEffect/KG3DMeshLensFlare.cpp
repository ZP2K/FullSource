#include "stdafx.h"
#include "kg3dmeshlensflare.h"
//#include "kg3dgraphicstool.h"
#include "KG3DTextureTable.h"
//#include "kg3denginemanager.h"
#include "kg3drenderstate.h"
#include "IEKG3DMeshLensFlare.h"
#include "IEKG3DTexture.h"
#include "KG3DCamera.h"
#include "KG3DScene.h"
#include "KGINIFileHelpers.h"
#include "KGHelperClasses.h"
//#include "MathTool.h"
#include "KG3DGraphicsStructEx.h"
//#include "KG3DEnvEffLight.h"
#include "KG3DBaseHelpers.h"
#include "KG3DDefaultShaderGroup.h"
#include "shaders/ShaderDefinitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////
namespace KG3D_ENVIRONMENT_EFFECT
{
	KG3DMeshLensFlare::KG3DMeshLensFlare(int nID, KG3DENVEFF_TYPE emType)
		:KG3DEnvEffect(nID, emType)
		,m_pTextureFlare(NULL)
		,m_bEnableBlending(FALSE)
		,m_fBlendPercent(0.0f)
		,m_pQuery(NULL)
		,m_vSunPosPostProj(0, 0, 0, 0)
		,m_fAlpha(1)
		,m_fAlphaWithoutFirst(1)
		,m_pVertexBuffer(NULL)
		,m_VertexCountInBuffer(0)
		,m_pOCVertexBuffer(NULL)
		,m_fLastOcclusionRet(0)
	{
		D3DXMatrixIdentity(&m_matRotationOfFlares);
	}	

	KG3DMeshLensFlare::~KG3DMeshLensFlare(void)
	{
		UnInit();
	}
	////////////////////////////////////////////////////////////////////////////////
	ULONG KG3DMeshLensFlare::GetLensFlareCount()
	{
		return (ULONG)m_vecLensFlare.size();
	}
	HRESULT KG3DMeshLensFlare::GetLensFlareItem(size_t uPosition, LensFlareInfo *pFlareInfo)
	{
		HRESULT hResult = E_FAIL;
		KG_PROCESS_ERROR(pFlareInfo);
		KG_PROCESS_ERROR(uPosition < m_vecLensFlare.size());
		*pFlareInfo = m_vecLensFlare[uPosition];
		hResult = S_OK;
Exit0:
		return hResult;
	}
	HRESULT KG3DMeshLensFlare::SetLensFlareItem( size_t uPosition, const LensFlareInfo* pFlareInfo
												, size_t* pNewPos )
	{
		HRESULT hResult = E_FAIL;

		KG_PROCESS_ERROR(pFlareInfo);
		KG_PROCESS_ERROR(uPosition < m_vecLensFlare.size());
		
		{
			LensFlareInfo TempLI = *pFlareInfo;
			bool bRet = ValidateLensFlareInfo(TempLI);
			KG_PROCESS_ERROR(bRet);
			m_vecLensFlare[uPosition] = TempLI;
		}

		//���Height������˳��Ҫ�󣬻�����������������֮��ԭ�����Ǹ�˳����ˣ�Ҫ֪ͨ�ⲿ
		bool bNeedResort = false;
		do
		{
			if(m_vecLensFlare.size() <= 1)
				break;
			
			if(uPosition > 0)
			{
				if (m_vecLensFlare[uPosition-1].fHeight < m_vecLensFlare[uPosition].fHeight)
				{
					bNeedResort = true;
				}
			}

			if (uPosition + 1 < m_vecLensFlare.size())
			{
				if (m_vecLensFlare[uPosition+1].fHeight > m_vecLensFlare[uPosition].fHeight)
				{
					bNeedResort = true;
				}
			}
		}while(false);	//end of do 	

		if (bNeedResort)
		{
			LensFlareInfo Temp = m_vecLensFlare[uPosition];
			this->SortLensInfo();
			if (NULL != pNewPos)
			{
				for (size_t i = 0; i < m_vecLensFlare.size(); ++i)
				{
					if (m_vecLensFlare[i] == Temp)
					{
						*pNewPos = i;
						break;
					}
				}
			}
		}
		else
		{
			if (NULL != pNewPos)
			{
				*pNewPos = uPosition;
			}
		}

		FillVertexBuffer();		

		hResult = S_OK;
Exit0:
		return hResult;
	}

	HRESULT KG3DMeshLensFlare::GetTexID( DWORD* pID )
	{
		KG_PROCESS_ERROR(pID && m_pTextureFlare);
		*pID = m_pTextureFlare->GetID();
		return S_OK;
Exit0:
		return E_FAIL;
	}

	////////////////////////////////////////////////////////////////////////////////

	/*static*/bool KG3DMeshLensFlare::SortComp(LensFlareInfo pElem1, LensFlareInfo pElem2)
	{
		return pElem1.fHeight > pElem2.fHeight;
	}
	HRESULT KG3DMeshLensFlare::SortLensInfo()
	{
		std::sort(m_vecLensFlare.begin(), m_vecLensFlare.end(), KG3DMeshLensFlare::SortComp);
		return S_OK;
	}
	HRESULT KG3DMeshLensFlare::AddLensFlare(const LensFlareInfo& lensFlare)
	{
		return AddLensFlare(lensFlare, TRUE);
	}

	HRESULT KG3DMeshLensFlare::AddLensFlare( LensFlareInfo lensFlare, BOOL bReflashResource )
	{
		HRESULT hr = E_FAIL;

		KG_PROCESS_ERROR(m_vecLensFlare.size() < EM_MAX_LENSFLARE);

		{
			bool bRet = ValidateLensFlareInfo(lensFlare);
			KG_PROCESS_ERROR(bRet);
		}

		m_vecLensFlare.push_back(lensFlare);

		this->SortLensInfo();

		if (bReflashResource)
		{
			FillVertexBuffer();
		}

		hr = S_OK;
Exit0:
		return hr;
	}
	HRESULT KG3DMeshLensFlare::DelLensFlare(size_t uIndex)
	{
		if (1 >= m_vecLensFlare.size())
		{
			return E_FAIL;
		}
		vector<LensFlareInfo>::iterator iter = m_vecLensFlare.begin();
		m_vecLensFlare.erase(iter + uIndex);
		this->SortLensInfo();

		FillVertexBuffer();

		return S_OK;
	}

	HRESULT KG3DMeshLensFlare::ReBuildVertexBuffer(size_t uLensFlareCount)
	{
		HRESULT hr = E_FAIL;
		_ASSERTE(uLensFlareCount <= EM_MAX_LENSFLARE);
		if(NULL == m_pVertexBuffer || m_VertexCountInBuffer < uLensFlareCount * EM_VERTEX_PER_FACE)
		{
			_ASSERTE(0 != uLensFlareCount);
			{
				SAFE_RELEASE(m_pVertexBuffer);
				size_t VertexCount = uLensFlareCount * EM_VERTEX_PER_FACE;
				size_t VertexBufferSize = VertexCount * sizeof(NewVertexStruct);
				hr = g_pd3dDevice->CreateVertexBuffer((UINT)VertexBufferSize, 0
					, NEW_FVF, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

				KG_COM_PROCESS_ERROR(hr);
				m_VertexCountInBuffer =  VertexCount;
			}
		}
		return S_OK;
Exit0:
		return E_FAIL;
	}
	HRESULT KG3DMeshLensFlare::ReBuildOcclusionQuery()
	{
		///���Դ���Query
		HRESULT hr = E_FAIL;
		SAFE_RELEASE(m_pQuery);
		static bool bDeviceSupportQuery = true;
		if(bDeviceSupportQuery)
		{
			hr = g_pd3dDevice->CreateQuery(D3DQUERYTYPE_OCCLUSION, &m_pQuery);
			if (FAILED(hr))///ʧ�ܵĻ��������豸��֧��
			{
				bDeviceSupportQuery = false;
				_ASSERTE(! m_pQuery);
			}
		}
		return hr;
	}
	HRESULT STDMETHODCALLTYPE KG3DMeshLensFlare::Init()
	{
		HRESULT hr = E_FAIL;

		_ASSERTE(NULL == m_pQuery && NULL == m_pVertexBuffer);

		//����Effect
		m_shaderHolder = g_GetDefaultShaderGroup().GetDefaultShader(DEFST_LENSFLARE);

		//hr = RecordEffectParamHandles();
		//KG_COM_PROCESS_ERROR(hr);

		hr = ReBuildOcclusionQuery();
		//���ж�OcclusionQuery�ĳɹ����е�Ӳ���ǲ�֧�ֵ�

		_ASSERTE(NULL == m_pOCVertexBuffer);
		//����OcclussionCulling�Ĳ���
		if(NULL != m_pQuery)
		{
			HRESULT hr = g_pd3dDevice->CreateVertexBuffer(EM_OC_POINT_COUNT * EM_OC_POINT_SIZE, 0
				, EM_OC_FVF, D3DPOOL_MANAGED, &m_pOCVertexBuffer, NULL);
			KG_COM_PROCESS_ERROR(hr);
		}

		FillOCVertexBuffer();

		return S_OK;
Exit0:
		return E_FAIL;
		
	}
	HRESULT STDMETHODCALLTYPE KG3DMeshLensFlare::UnInit()
	{
		SAFE_RELEASE(m_pTextureFlare);
		SAFE_RELEASE(m_pQuery);
		SAFE_RELEASE(m_pVertexBuffer);
		SAFE_RELEASE(m_pOCVertexBuffer);
		return S_OK;
	}

	D3DXVECTOR4 KG3DMeshLensFlare::CalculateScreenPos(const D3DXVECTOR3& vPosIn, const D3DXMATRIX& matView
														, const D3DXMATRIX& matProj)
	{
		D3DXVECTOR4	vRet(0, 0, 0, 0);

		D3DXMATRIX	matConcate = matView * matProj;
		//matConcate *= matProj;		
		
		D3DXVec3Transform(&vRet, &vPosIn, &matConcate);

        if (MT_FLOAT_UNEQUAL(vRet.w, 0.f))
        {
		    vRet.x /= vRet.w;
		    vRet.y /= vRet.w;
		    vRet.z /= vRet.w;
        }

		return vRet;///������w
	}
	FLOAT KG3DMeshLensFlare::CalculateVisibility()
	{
		if (m_bEnableBlending)
		{
			KG3DDWGC_Key_LensFlare TempKey, MainKey;
			bool bRet = m_DescStruct.ToKey(MainKey);
			_ASSERTE(bRet);
			bRet = TempKey.CalculateInterp(MainKey, m_SubKey, m_fBlendPercent);
			_ASSERTE(bRet);
			_ASSERTE(TempKey.m_fVisibility > 0-FLT_EPSILON && TempKey.m_fVisibility < 1+FLT_EPSILON);
			return TempKey.m_fVisibility;
		}
		return m_DescStruct.m_fVisibility;
	}

    HRESULT KG3DMeshLensFlare::OnLostDevice()
    {
        SAFE_RELEASE(m_pQuery);
		return S_OK;
    }

    HRESULT KG3DMeshLensFlare::OnResetDevice()
    {
        HRESULT hr = E_FAIL;
        HRESULT hrResult = E_FAIL;

		//hr = RecordEffectParamHandles();
		//KG_COM_PROCESS_ERROR(hr);

        hr = ReBuildOcclusionQuery();
		//OcclusionQuery�Ĵ��������ɹ�

        hrResult = S_OK;
        return hrResult;
    }

	HRESULT KG3DMeshLensFlare::FrameMove()
	{
		KG_PROCESS_SUCCESS(m_vecLensFlare.empty());

		m_fAlpha = this->CalculateVisibility();						//Ȩ�أ����ڶ�̬�����仯
		m_fAlphaWithoutFirst = m_fAlpha;
		MT_LIMIT_TO_RANGE(m_fAlpha, (1+FLT_EPSILON), (0-FLT_EPSILON));
		KG_PROCESS_SUCCESS(m_fAlpha < FLT_EPSILON);	

		{
			//������˵�һ������֮��������Ļ���С�Ĳ��ֵ�͸����
			m_fAlphaWithoutFirst = m_fAlpha;
			m_fAlphaWithoutFirst *= this->CalculateOffScreenFactor(m_vSunPosPostProj);		//����˥��	

			if(m_fAlphaWithoutFirst > FLT_EPSILON)
			{					
				m_fAlphaWithoutFirst *= RetrieveOcclusionCullingData();
			}

			{
				D3DXMATRIX matTemp;
				D3DXVECTOR2 vSunPos2D(m_vSunPosPostProj.x, m_vSunPosPostProj.y);
				D3DXVec2Normalize(&vSunPos2D, &vSunPos2D);
				FLOAT fAngleToRotate = MT_FLOAT_EQUAL(vSunPos2D.x, 0) ? 
					(vSunPos2D.y > 0 ? D3DX_PI/2 : -D3DX_PI/2)
					: atan(vSunPos2D.x / vSunPos2D.y);
				//��ΪZ������Ļ�ڵģ����Ƕ��ǰ���Ļ����ģ������и���
				D3DXMatrixRotationZ(&matTemp, -fAngleToRotate);

				m_matRotationOfFlares = matTemp;
			}			
			
			return S_OK;
		}
Exit1:
		m_fAlpha = 0;
		m_fAlphaWithoutFirst = 0;
		return S_OK;
	}
	
	FLOAT KG3DMeshLensFlare::RetrieveOcclusionCullingData()
	{
		if(NULL == m_pQuery)
		{
			return 0;
		}
		
		DWORD dwVerticesWrited = 0;
		//�õ���һ֡�Ľ��
		///����S_FALSE��E_FAILED���������ο�d3d�ĵ���GetDataֻ�з���S_OK�����ݲ�����Ч��
		if(S_OK != m_pQuery->GetData((void*)&dwVerticesWrited, sizeof(DWORD), D3DGETDATA_FLUSH))
		{
			return m_fLastOcclusionRet;//����޷�ȡ��OC�����ô�����ϴεĽ���ȼۺã���Ȼ��������˸�������ӾͲ�������ͨ�˲�ʲô�ĸ߿Ƽ�
		}
		FLOAT fRet = (float)dwVerticesWrited/EM_OC_POINT_COUNT;
		//_ASSERTE(fRet > -FLT_EPSILON && fRet < 1.0f + FLT_EPSILON);

		fRet = sqrt(fRet);//���������źŴ������½�������1�����仺Щ������Ч����Щ��

		m_fLastOcclusionRet = fRet;
		return m_fLastOcclusionRet;
	}
	void KG3DMeshLensFlare::SetShaderParams(LPD3DXEFFECT pEffect, D3DXVECTOR2 vSunPostProj, FLOAT fAlpha)
	{
		D3DXVECTOR2 vTexSplit(1/(FLOAT)m_DescStruct.m_nTexSplitNumX
			, 1/(FLOAT)(m_DescStruct.m_nTexSplitNumY));

		FLOAT fOCFactor = m_DescStruct.m_dwState & KG3DMeshLensFlare_State_DrawDummyLen ? 1.0f : 1.0f/255.0f;  
		/*pEffect->SetFloat(m_ShaderHandles[EM_FLOAT_OC_FACTOR], fOCFactor);*/

		ShaderConstsLensflare	shaderConsts;
		shaderConsts.matRotation = m_matRotationOfFlares;
		shaderConsts.vSunPosPostProj = vSunPostProj;
		shaderConsts.vTexSplit = vTexSplit;
		shaderConsts.fScreenAspect = m_vScreenSize.x/m_vScreenSize.y;
		shaderConsts.fBlendFactor = fAlpha;
		shaderConsts.fOCFactor = fOCFactor;

		HRESULT hr = E_FAIL;
		HrV(pEffect->SetValue("g_shaderConsts", &shaderConsts, sizeof(shaderConsts)));
	}
	//���������Skysystem��RenderSpecial����ϵģ�����Ϊ����ɽ��ס̫������Ҳû�а취
	HRESULT KG3DMeshLensFlare::RenderFirstFlare(FLOAT fDecay /* = 1 */)
	{
        HRESULT hr = E_FAIL;
		
		
		//�õ�̫��λ�ã�̫��λ������̫����ķ��������Ļλ�õõ���
		{
			KG3DScene* pScene = g_cGraphicsTool.GetCurScene();
			KG_PROCESS_ERROR(pScene);

			D3DXVECTOR3	CurSunDirection = g_cGraphicsTool.GetSunLightParam(*pScene).dir;//g_cGraphicsTool.GetSunLight(*pScene).GetDirection();
			D3DXVec3Normalize(&CurSunDirection, &CurSunDirection);

			KG3DCamera* pCamera = g_cGraphicsTool.GetSceneMainCamera();//�������Ҳ������CurScene��
			KG_PROCESS_ERROR(pCamera);

			enum { sun_distance_infinite = 1000000,};	//10��������˰�	
			static FLOAT stfSunDistance = sun_distance_infinite;	//�Ӹ���ʱ������Ϊ�˵��Է���

			D3DXVECTOR3 vSunPosWorld = pCamera->GetPosition() - CurSunDirection * stfSunDistance;
			m_vScreenSize = this->GetScreenSize();

			//�õ����ǲü��ռ��x,y,z, ��Ļ�ڵ�ȫ����-1��1
			m_vSunPosPostProj = CalculateScreenPos(vSunPosWorld,pCamera->GetViewMatrix()
						, pCamera->GetProjectionMatrix());

			//̫������������򣬲�����Ⱦ��
			if(IsSunOnTheBackSideOfCamera(m_vSunPosPostProj))	
			{
				m_fAlphaWithoutFirst = 0;
				goto Exit1;
			}
		}

		LPD3DXEFFECT pEffect = m_shaderHolder.GetEffect();
		_ASSERTE(NULL != pEffect);

		KG_PROCESS_SUCCESS(m_vecLensFlare.empty());
		KG_PROCESS_ERROR(NULL != m_pVertexBuffer && NULL != m_pTextureFlare);
		{
			g_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(NewVertexStruct));
			g_pd3dDevice->SetFVF(NEW_FVF);

			g_pd3dDevice->SetTexture(0, m_pTextureFlare->GetTexture());

			/*{
				pEffect->SetFloatArray(m_ShaderHandles[EM_FLOAT2_SUN_POS_POSTPROJ]
					, (FLOAT*)&m_vSunPosPostProj, 2);

				D3DXVECTOR2 vTexSplit(1/(FLOAT)m_DescStruct.m_nTexSplitNumX
					, 1/(FLOAT)(m_DescStruct.m_nTexSplitNumY));
				pEffect->SetFloatArray(m_ShaderHandles[EM_FLOAT2_TEXTURE_SPLIT]
				, (FLOAT*)&vTexSplit, 2);

				pEffect->SetMatrix(m_ShaderHandles[EM_MATRIX_ROTATION], &m_matRotationOfFlares);
			}

			MT_LIMIT_TO_RANGE(fDecay, 1, 0); 
			pEffect->SetFloat(m_ShaderHandles[EM_FLOAT_BLEND_FACTOR], m_fAlpha * fDecay);
			pEffect->SetFloat(m_ShaderHandles[EM_FLOAT_SCREEN_ASPECT], m_vScreenSize.x/m_vScreenSize.y);*/

			SetShaderParams(pEffect, D3DXVECTOR2(m_vSunPosPostProj.x, m_vSunPosPostProj.y), m_fAlpha * fDecay);

			UINT PassCount = 0;
			hr = pEffect->Begin(&PassCount, 0);
			if (SUCCEEDED(hr))
			{
				if (PassCount > 0)
				{
					_ASSERTE(m_VertexCountInBuffer >= EM_VERTEX_PER_FACE);
					hr = pEffect->BeginPass(0);
					if (SUCCEEDED(hr))
					{
						g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

						pEffect->EndPass();
					}	
				}				
				
				pEffect->End();
			}
			
		}
Exit1:
		return S_OK;
Exit0:
		return E_FAIL;
	}


	HRESULT KG3DMeshLensFlare::Render(const KG3DEnvEffRenderContext& renderContext)
	{			
		return S_OK;
	}
	
	HRESULT KG3DMeshLensFlare::PostRender()
	{
		//�����֧��Ӳ��Occlusion֧�ֵĻ����ǾͲ���ȾLensflare�ĺ��沿���ˣ����������ײ�Ļ���̫��ʱ��
		KG_PROCESS_SUCCESS(NULL == m_pQuery);
		RenderOcclusionCullingElement(m_vSunPosPostProj);
		
		KG_PROCESS_SUCCESS(m_fAlphaWithoutFirst < 2 * FLT_EPSILON);	
		{
			if (m_fAlphaWithoutFirst > 1)
				m_fAlphaWithoutFirst = 1;
			
			LPD3DXEFFECT pEffect = m_shaderHolder.GetEffect();
			_ASSERTE(NULL != pEffect);
			KG_PROCESS_ERROR(NULL != m_pVertexBuffer && NULL != m_pTextureFlare);
			
			g_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(NewVertexStruct));
			g_pd3dDevice->SetFVF(NEW_FVF);

			g_pd3dDevice->SetTexture(0, m_pTextureFlare->GetTexture());

			/*{		
				pEffect->SetFloat(m_ShaderHandles[EM_FLOAT_BLEND_FACTOR], m_fAlphaWithoutFirst);
				pEffect->SetFloatArray(m_ShaderHandles[EM_FLOAT2_SUN_POS_POSTPROJ]
								, (FLOAT*)&m_vSunPosPostProj, 2);
				
				pEffect->SetFloatArray(m_ShaderHandles[EM_FLOAT2_TEXTURE_SPLIT]
									, (FLOAT*)&vTexSplit, 2);
				pEffect->SetMatrix(m_ShaderHandles[EM_MATRIX_ROTATION], &m_matRotationOfFlares);
			}*/

			SetShaderParams(pEffect, D3DXVECTOR2(m_vSunPosPostProj.x, m_vSunPosPostProj.y), m_fAlphaWithoutFirst);

			UINT PassCount = 0;
			HRESULT hr = pEffect->Begin(&PassCount, 0);
			if (SUCCEEDED(hr))
			{
				if (PassCount > 1)
				{
					hr = pEffect->BeginPass(1);
					if (SUCCEEDED(hr))
					{
						_ASSERTE(m_VertexCountInBuffer >= m_vecLensFlare.size() * EM_VERTEX_PER_FACE);
						UINT uFaceCount = (UINT)(m_vecLensFlare.size() - 1) * 2;
						if(uFaceCount > 0)
							g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, EM_VERTEX_PER_FACE * 1, uFaceCount);//����Ҫ������һ��Flare

						pEffect->EndPass();
					}	
				}				

				pEffect->End();
			}	
		}
Exit1:
Exit0:
		return S_OK;
	}

	HRESULT KG3DMeshLensFlare::DefaultBuild()
	{
		return DefaultBuildPrivate();
	}

	HRESULT KG3DMeshLensFlare::DefaultBuildPrivate()
	{
		{
			KG3DMeshLensFlareDescStruct TempStruct;
			this->GetDefaultDesc(&TempStruct);
			TempStruct.m_dwModificationOption = KG3DMeshLensFlare_MDO_All;
			this->RequestModification(&TempStruct);

			m_vecLensFlare.clear();

			int nNum = 0;
			for (int i = 0; i < TempStruct.m_nTexSplitNumX; i++)
			{
				for (int j = 0; j < TempStruct.m_nTexSplitNumY; j++)
				{
					LensFlareInfo lensInfo;

					lensInfo.fHeight = 1.0f - (float)(nNum) / 7.0f;

					++nNum;

					lensInfo.nIndexX = i;
					lensInfo.nIndexY = j;
					lensInfo.fRate   = 2;

					if (nNum == 1)
						lensInfo.fSize = 0.1f;
					else if (nNum == 2)
						lensInfo.fSize = 0.05f;
					else if (nNum == 3)
						lensInfo.fSize = 0.01f;
					else
						lensInfo.fSize = 0.05f;
					AddLensFlare(lensInfo, FALSE);
				}
			}

			FillVertexBuffer();
		}

		return S_OK;
	}


	HRESULT KG3DMeshLensFlare::SetFlareTexture(LPCTSTR tczPath)
	{
		HRESULT hr = E_FAIL;
		KG3DTexture* pTempTexture = NULL;
		
		TCHAR pathRelativePath[MAX_PATH] = _T("\0");
		KG_PROCESS_ERROR(NULL != tczPath);

		{
			BOOL bRet = KG3DBASE::GetFilePathFromFullPath(pathRelativePath, _countof(pathRelativePath), tczPath);
			KG_PROCESS_ERROR(bRet);

			hr = g_cTextureTable.LoadResourceFromFile(pathRelativePath, 0, TEXOP_DISABLEDTX
				, (IKG3DResourceBase **)&pTempTexture);
			KG_COM_PROCESS_ERROR(hr);

			_ASSERTE(NULL != pTempTexture);

			SAFE_RELEASE(m_pTextureFlare);	//ע�⣬��ʹ�ǵõ���ָ�����ԭ���ģ�����������û��ġ�
			m_pTextureFlare = pTempTexture;
			errno_t errTemp = _tcscpy_s(m_DescStruct.m_tczTexName, pathRelativePath);
			_ASSERTE(0 == errTemp);
			KG_USE_ARGUMENT(errTemp);
		}

		return S_OK;
Exit0:
		return E_FAIL;
	}

	HRESULT KG3DMeshLensFlare::SaveMapSetting( IIniFile* pIniFile, LPCTSTR tczSectionIn)
	{
		KG_PROCESS_ERROR(pIniFile);
		KG_PROCESS_ERROR(m_pTextureFlare);
		{
			TCHAR tSection[64] = _T("LensFlare");
			if (tczSectionIn)
			{
				_tcscpy_s(tSection, tczSectionIn);
			}
			KG_CUSTOM_HELPERS::EraseSection(pIniFile, tSection);
			{
				bool bRet = m_DescStruct.SaveMapSetting(pIniFile, tSection);
				KG_PROCESS_ERROR(bRet);
			}

			pIniFile->WriteInteger(tSection, "Count", (int)m_vecLensFlare.size());///���ڼ��ص�ʱ��Ԥ�ȷ����ڴ�

				int k = 1;
			char szKeyName[64];
			char szKeyValue[512];

			vector<LensFlareInfo>::iterator i = m_vecLensFlare.begin();
			while (i != m_vecLensFlare.end())
			{
				sprintf_s(szKeyName, "%d", k);
				sprintf_s(szKeyValue, "%s%d", tSection, k);
				pIniFile->WriteString(tSection, szKeyName, szKeyValue);

				pIniFile->WriteInteger(szKeyValue, "X", i->nIndexX);
				pIniFile->WriteInteger(szKeyValue, "Y", i->nIndexY);
				pIniFile->WriteFloat(szKeyValue, "Height", i->fHeight);
				pIniFile->WriteFloat(szKeyValue, "Size", i->fSize);
				pIniFile->WriteFloat(szKeyValue, "Rate", i->fRate);
				k++;
				i++;
			}
			return S_OK;
		}
Exit0:
		return E_FAIL;
	}

	HRESULT KG3DMeshLensFlare::LoadMapSetting( IIniFile* pIniFile, LPCTSTR tczSectionIn)
	{
		TCHAR tSection[64] = _T("LensFlare");//Ĭ�ϵ�����
		HRESULT hr = E_FAIL;
		if (! pIniFile)
		{
			hr = Init();			
			if (SUCCEEDED(hr))
			{
				hr = DefaultBuildPrivate();
			}
			return hr;
		}

		hr = Init();
		KG_COM_PROCESS_ERROR(hr);	

		if (tczSectionIn)
		{
			_tcscpy_s(tSection, tczSectionIn);
		}

		TCHAR tszKeyValue[512];
		TCHAR tszKeyName[64];
		*tszKeyValue = '\0';
		*tszKeyName  = '\0';

		{
			KG3DMeshLensFlareDescStruct TempStruct;
			KG_PROCESS_ERROR(TempStruct.LoadMapSetting(pIniFile, tSection));
			TempStruct.m_dwModificationOption = KG3DMeshLensFlare_MDO_All;
			TempStruct.m_dwModificationOption &= ~KG3DMeshLensFlare_MDO_State;///State�ǲ������
			KG_COM_PROCESS_ERROR(this->RequestModification(&TempStruct));
		}

		int nLensFrameCount = 9;
		{
			pIniFile->GetInteger(tSection, "Count", 9, &nLensFrameCount);
			KG_PROCESS_ERROR(nLensFrameCount <= EM_MAX_LENSFLARE && nLensFrameCount > 0);
		}
		///��"1"��Ϊ�ؼ��ֿ�ʼȡLensFlareInfo
		BOOL bRet = KG_CUSTOM_HELPERS::TValueToString((INT)1, tszKeyName);
		_ASSERTE(bRet);
		for(size_t uLimit = 0; bRet && uLimit < EM_MAX_LENSFLARE && uLimit < (size_t)nLensFrameCount
			; bRet = KG_CUSTOM_HELPERS::GetNextKey(pIniFile, tSection, tszKeyName, tszKeyName), ++uLimit) 
		{
			if(! KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, tSection, tszKeyName, _T(""), tszKeyValue))
			{
				continue;
			}
			if (*tszKeyValue == '\0' || (! KG_CUSTOM_HELPERS::IsSectionExist(pIniFile, tszKeyValue)))
			{
				continue;
			}
			LensFlareInfo lenInfo;

			int nIndexX = 0;
			int nIndexY = 0;
			float fHeight = 0;
			float fSize   = 0;
			float fRate = 0;

			pIniFile->GetInteger(tszKeyValue, "X", nIndexX, &nIndexX);
			pIniFile->GetInteger(tszKeyValue, "Y", nIndexY, &nIndexY);
			pIniFile->GetFloat(tszKeyValue, "Height", fHeight, &fHeight);
			pIniFile->GetFloat(tszKeyValue, "Size", fSize, &fSize);
			pIniFile->GetFloat(tszKeyValue, "Rate", fRate, &fRate);

			lenInfo.nIndexX = nIndexX;
			lenInfo.nIndexY = nIndexY;
			lenInfo.fHeight = fHeight;
			lenInfo.fSize   = fSize;
			lenInfo.fRate   = fRate;

			hr = AddLensFlare(lenInfo, FALSE);
			KG_COM_PROCESS_ERROR(hr);
		}
		
		FillVertexBuffer();

		return S_OK;
Exit0:
		KGLogPrintf(KGLOG_WARNING, "%s%s", "KG3DMeshLensFlare::LoadMapSettingʧ��,Section:", tSection);
		return E_FAIL;
	}

	HRESULT KG3DMeshLensFlare::ToSolidInterface( LPVOID* pEffectInterface )
	{
		KG_PROCESS_ERROR(pEffectInterface);
		{
			IEKG3DMeshLensFlare* pIE = this;
			*pEffectInterface = (LPVOID)pIE;
			return S_OK;
		}
Exit0:
		return E_FAIL;
	}

	HRESULT KG3DMeshLensFlare::DWGCRequestPopFrontState(float fCurBlendPercent/* = 0.0f*/)
	{
		if (fCurBlendPercent > 0 && fCurBlendPercent < 1.0f)
		{
			KG3DDWGC_Key_LensFlare MidKey, BeforeKey;
			m_DescStruct.ToKey(BeforeKey);
			MidKey.CalculateInterp(BeforeKey, m_SubKey, fCurBlendPercent);
			m_DescStruct.SynWithKey(MidKey);
		}
		else
		{
#if defined(_DEBUG) | defined(DEBUG)
			bool bRet = 
#endif
				m_DescStruct.SynWithKey(m_SubKey);
			_ASSERTE(bRet);
		}

		return S_OK;
	}

	HRESULT KG3DMeshLensFlare::DWGCRequestSubStateChange( const KG3DDWGC_KeyBase* pRetKey)
	{
		const KG3DDWGC_Key_LensFlare* pSolidKey =  dynamic_cast<const KG3DDWGC_Key_LensFlare*>(pRetKey);
		KG_PROCESS_ERROR(pSolidKey);
		m_SubKey = *pSolidKey;
		return S_OK;
Exit0:
		return E_FAIL;
	}

	HRESULT KG3DMeshLensFlare::DWGCGetCurStateToKey( KG3DDWGC_KeyBase* pRetKey)
	{
		KG3DDWGC_Key_LensFlare* pSolidKey =  dynamic_cast<KG3DDWGC_Key_LensFlare*>(pRetKey);
		KG_PROCESS_ERROR(pSolidKey);
		m_DescStruct.ToKey(*pSolidKey);
		return S_OK;
Exit0:
		return E_FAIL;
	}

	HRESULT KG3DMeshLensFlare::DWGCEnableSubState( BOOL bEnable )
	{
		m_bEnableBlending = bEnable;
		return S_OK;
	}

	HRESULT KG3DMeshLensFlare::DWGCSetPercent( float fPercent )
	{
		m_fBlendPercent = fPercent;
		return S_OK;
	}

	HRESULT KG3DMeshLensFlare::DWGCRequestModification( const KG3DDWGC_KeyBase* pKey)
	{
		const KG3DDWGC_Key_LensFlare* pSolidKey =  dynamic_cast<const KG3DDWGC_Key_LensFlare*>(pKey);
		KG_PROCESS_ERROR(pSolidKey);
		KG_PROCESS_ERROR(m_DescStruct.SynWithKey(*pSolidKey));
		return S_OK;
Exit0:
		return E_FAIL;
	}

	HRESULT KG3DMeshLensFlare::RequestModification(const IEKG3DMeshLensFlareDescStruct* pIn ) /*= 0*/
	{
		HRESULT hr = E_FAIL;
		KG_PROCESS_ERROR(pIn);
		if (pIn->m_dwModificationOption & KG3DMeshLensFlare_MDO_TexSplitNum)
		{
			KG_PROCESS_ERROR(pIn->m_nTexSplitNumX > 0 && pIn->m_nTexSplitNumY > 0);
			if (pIn != &m_DescStruct)
			{
				//����зֱ�խ�����ܸ���
				for (size_t i = 0; i < m_vecLensFlare.size(); ++i)
				{
					LensFlareInfo& LensInfo = m_vecLensFlare[i];
					KG_PROCESS_ERROR(LensInfo.nIndexX < pIn->m_nTexSplitNumX);
					KG_PROCESS_ERROR(LensInfo.nIndexY < pIn->m_nTexSplitNumY);
				}
				m_DescStruct.m_nTexSplitNumX = pIn->m_nTexSplitNumX;
				m_DescStruct.m_nTexSplitNumY = pIn->m_nTexSplitNumY;
			}
		}
		if (pIn->m_dwModificationOption & KG3DMeshLensFlare_MDO_Visibility)
		{
			KG_PROCESS_ERROR(pIn->m_fVisibility > 0-FLT_EPSILON);
			if (pIn != &m_DescStruct)
			{
				m_DescStruct.m_fVisibility = pIn->m_fVisibility;
			}
		}
		if (pIn->m_dwModificationOption & KG3DMeshLensFlare_MDO_TexPath)
		{
			hr = this->SetFlareTexture(pIn->m_tczTexName);
			KG_COM_PROCESS_ERROR(hr);
		}
		if (pIn->m_dwModificationOption & KG3DMeshLensFlare_MDO_BoundAspect)
		{
			KG_PROCESS_ERROR(pIn->m_fVisibleOutterBoundAspect > 0-FLT_EPSILON && pIn->m_fVisibleOutterBoundAspect < 3+FLT_EPSILON);
			if (pIn != &m_DescStruct)
			{
				m_DescStruct.m_fVisibleOutterBoundAspect = pIn->m_fVisibleOutterBoundAspect;
			}
		}
		if (pIn->m_dwModificationOption & KG3DMeshLensFlare_MDO_DummyLenSize)
		{
			KG_PROCESS_ERROR(pIn->m_fDummyLenSize >= 0 && pIn->m_fDummyLenSize < 0.41);//��Ļ��40%����ôҲ�����ˣ�����0��ʱ�򣬽�ֹ�������
			if (m_DescStruct.m_fDummyLenSize != pIn->m_fDummyLenSize)
			{
				m_DescStruct.m_fDummyLenSize = pIn->m_fDummyLenSize;
				FillOCVertexBuffer();
			}
		}
		if (pIn->m_dwModificationOption & KG3DMeshLensFlare_MDO_State)
		{
			if (pIn != &m_DescStruct)
			{
				m_DescStruct.m_dwState = pIn->m_dwState;
			}
		}
		//Exit1:
		return S_OK;
Exit0:
		return E_FAIL;
	}

	HRESULT KG3DMeshLensFlare::GetDescStruct( IEKG3DMeshLensFlareDescStruct* pRetStruct ) /*= 0*/
	{
		KG_PROCESS_ERROR(pRetStruct);
		{
			if (pRetStruct != &m_DescStruct)
			{
				*pRetStruct = m_DescStruct;
			}
			return S_OK;
		}
Exit0:
		return E_FAIL;
	}

	void KG3DMeshLensFlare::GetDefaultDesc( IEKG3DMeshLensFlareDescStruct* pDescStruct )
	{
		_ASSERTE(NULL != pDescStruct);
		_ASSERTE(NULL != pDescStruct->m_tczTexName);
		_tcscpy_s(pDescStruct->m_tczTexName, _T("Data\\public\\defaultscene\\Flare.tga"));
		pDescStruct->m_nTexSplitNumX = em_default_x_split;
		pDescStruct->m_nTexSplitNumY = em_default_y_split;
		pDescStruct->m_fVisibility = 1.0f;
		pDescStruct->m_fVisibleOutterBoundAspect = 0.3f;
		pDescStruct->m_fDummyLenSize = 0.01f;
		pDescStruct->m_dwModificationOption = 0;
	}

	D3DXVECTOR2 KG3DMeshLensFlare::GetScreenSize()
	{
        D3DVIEWPORT9 vp;
        g_pd3dDevice->GetViewport(&vp);
        return D3DXVECTOR2((FLOAT)vp.Width, (FLOAT)vp.Height);
    }
	
	VOID	KG3DMeshLensFlare::RenderOcclusionCullingElement(const D3DXVECTOR4 vSunPosPostProj)
	{
		LPD3DXEFFECT pEffect = m_shaderHolder.GetEffect();
		KG_PROCESS_ERROR(NULL != pEffect && NULL != m_pOCVertexBuffer && NULL != m_pQuery);
		{
			BOOL bEnableOcclussion = m_DescStruct.m_fDummyLenSize > 0;
			KG_PROCESS_ERROR(bEnableOcclussion);
			
			/*FLOAT fOCFactor = m_DescStruct.m_dwState & KG3DMeshLensFlare_State_DrawDummyLen ? 1.0f : 1.0f/255.0f;  
			pEffect->SetFloat(m_ShaderHandles[EM_FLOAT_OC_FACTOR], fOCFactor);*/
			
			D3DXVECTOR2 vSunPosProj(m_vSunPosPostProj.x, m_vSunPosPostProj.y);
			{
				//OC�����������Ļ��û�������ˣ�����Ҫ��������Ļ�ڲ�
				float fMaxSun = max(abs(m_vSunPosPostProj.x) , abs(m_vSunPosPostProj.y));

				float fMaxScale = (fMaxSun + m_DescStruct.m_fDummyLenSize) * 1.005f;
				if(fMaxScale > 1)
					vSunPosProj /= fMaxScale;
			}
			//pEffect->SetFloatArray(m_ShaderHandles[EM_FLOAT2_SUN_POS_POSTPROJ], (FLOAT*)&vSunPosProj, 2);
			SetShaderParams(pEffect, vSunPosProj, 0);
			
			g_pd3dDevice->SetFVF(EM_OC_FVF);
			g_pd3dDevice->SetStreamSource(0, m_pOCVertexBuffer, 0, EM_OC_POINT_SIZE);

			HRESULT hr = E_FAIL;
			hr = m_pQuery->Issue(D3DISSUE_BEGIN);
			if (SUCCEEDED(hr))
			{
				UINT uPasses = 0;
				hr = pEffect->Begin(&uPasses, 0);
				if(SUCCEEDED(hr))
				{
					hr = pEffect->BeginPass(2);
					if (SUCCEEDED(hr))
					{
						if (uPasses >= 3)
						{
							g_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, EM_OC_POINT_COUNT);
						}
						
						pEffect->EndPass();
					}
					pEffect->End();
				}
				m_pQuery->Issue(D3DISSUE_END);
			}
		}
Exit0:
		return;
	}

	FLOAT KG3DMeshLensFlare::CalculateOffScreenFactor( D3DXVECTOR4 vSunPosScreen )
	{
		KG_PROCESS_SUCCESS(vSunPosScreen.w <= 0); //FLT_EPSILON);///Ͷ�䵽�����ˣ��϶�û�й���
		_KG3D_DEBUG_SELF_ASSERTE(vSunPosScreen.z >= 0);

		{
			_ASSERTE(m_DescStruct.m_fVisibleOutterBoundAspect > 0-FLT_EPSILON && m_DescStruct.m_fVisibleOutterBoundAspect < 1+FLT_EPSILON);
			FLOAT fMaxGapToScreenCenter = 0;
			{
				FLOAT fABS_X = vSunPosScreen.x > 0 ? vSunPosScreen.x : -vSunPosScreen.x;
				FLOAT fABS_Y = vSunPosScreen.y > 0 ? vSunPosScreen.y : -vSunPosScreen.y;
				fMaxGapToScreenCenter = fABS_X > fABS_Y ? fABS_X : fABS_Y;
			}

			KG_PROCESS_SUCCESS(fMaxGapToScreenCenter >= 1 + m_DescStruct.m_fVisibleOutterBoundAspect);

			if (fMaxGapToScreenCenter >= 1 && fMaxGapToScreenCenter < 1 + m_DescStruct.m_fVisibleOutterBoundAspect)
			{
				return (1 - 0) * (1 + m_DescStruct.m_fVisibleOutterBoundAspect - fMaxGapToScreenCenter)/m_DescStruct.m_fVisibleOutterBoundAspect;
			}
			//����Ļ���棬ʲôҲ����,ά��Alpha
			return 1;
		}
Exit1:
		return 0;
	}

	VOID KG3DMeshLensFlare::FillVertexBuffer()
	{
		HRESULT hr = E_FAIL;
		_ASSERTE(0 != m_vecLensFlare.size());
		//����VertexBuffer�����Buffer�Ƚϴ󣬲���������С
		hr = ReBuildVertexBuffer(m_vecLensFlare.size());
		KG_COM_PROCESS_ERROR(hr);

		KG_PROCESS_ERROR(NULL != m_pVertexBuffer);
		size_t uVertexNeedToLockCount = m_vecLensFlare.size() * EM_VERTEX_PER_FACE;
		_ASSERTE(m_VertexCountInBuffer >= uVertexNeedToLockCount);
		{
			NewVertexStruct* pVertexStruct = NULL;
			NewVertexStruct* pBuffer = NULL;
			HRESULT hr = m_pVertexBuffer->Lock(0, (UINT)(uVertexNeedToLockCount * sizeof(NewVertexStruct))
										, (VOID**)&pBuffer, 0);
			KG_COM_PROCESS_ERROR(hr);

			_ASSERTE(NULL != pBuffer);
			pVertexStruct = pBuffer;
			for (size_t i = 0; i < m_vecLensFlare.size(); ++i)
			{
				//��������˳��Ϊ���ϣ����£����£����ϣ�Ȼ����ǰ����������
				//0,1,2,3,1,0
				const LensFlareInfo& Info = m_vecLensFlare[i];
				FLOAT fVertexScale = Info.fSize;	
				_ASSERTE(Info.fSize > -FLT_EPSILON);
				//0
				pVertexStruct[0].xPostProj = -fVertexScale;
				pVertexStruct[0].yPostProj = fVertexScale;
				pVertexStruct[0].Height = Info.fHeight;

				pVertexStruct[0].xIndex = (float)Info.nIndexX;
				pVertexStruct[0].yIndex = (float)Info.nIndexY;
				
				//1
				pVertexStruct[1].xPostProj = fVertexScale;
				pVertexStruct[1].yPostProj = -fVertexScale;
				pVertexStruct[1].Height = Info.fHeight;

				pVertexStruct[1].xIndex = (float)Info.nIndexX+1.0f;
				pVertexStruct[1].yIndex = (float)Info.nIndexY+1.0f;

				//2
				pVertexStruct[2].xPostProj = -fVertexScale;
				pVertexStruct[2].yPostProj = -fVertexScale;
				pVertexStruct[2].Height = Info.fHeight;

				pVertexStruct[2].xIndex = (float)Info.nIndexX;
				pVertexStruct[2].yIndex = (float)Info.nIndexY+1.0f;

				//3
				pVertexStruct[3].xPostProj = fVertexScale;
				pVertexStruct[3].yPostProj = fVertexScale;
				pVertexStruct[3].Height = Info.fHeight;

				pVertexStruct[3].xIndex = (float)Info.nIndexX+1.0f;
				pVertexStruct[3].yIndex = (float)Info.nIndexY;

				pVertexStruct[4] = pVertexStruct[1];
				pVertexStruct[5] = pVertexStruct[0];

				pVertexStruct += EM_VERTEX_PER_FACE;
				_ASSERTE(pVertexStruct <= pBuffer + uVertexNeedToLockCount);
			}

			m_pVertexBuffer->Unlock();
		}
Exit0:
		return;
	}

	bool KG3DMeshLensFlare::ValidateLensFlareInfo( LensFlareInfo &TempLI )
	{
		KG_PROCESS_ERROR(TempLI.fHeight >= -2.0f && TempLI.fHeight <= 2.f);
		KG_PROCESS_ERROR(TempLI.fSize <= 2.0f && TempLI.fSize > -FLT_EPSILON);
		KG_PROCESS_ERROR(TempLI.nIndexX < m_DescStruct.m_nTexSplitNumX);
		KG_PROCESS_ERROR(TempLI.nIndexY < m_DescStruct.m_nTexSplitNumY);
		return true;
Exit0:
		return false;
	}

	VOID KG3DMeshLensFlare::FillOCVertexBuffer()
	{
		KG_PROCESS_ERROR(NULL != m_pOCVertexBuffer);
		{
			D3DVECTOR* pBuffer = NULL;
			HRESULT hr = m_pOCVertexBuffer->Lock(0, EM_OC_POINT_COUNT * EM_OC_POINT_SIZE, (VOID**)&pBuffer, 0);
			KG_COM_PROCESS_ERROR(hr);

			_ASSERTE(EM_OC_POINT_COUNT > 0 && EM_OC_POINT_SIZE > 0);

			UINT uPointCountInAxis = EM_OC_POINT_ON_EDGE;

			_ASSERTE(m_DescStruct.m_fDummyLenSize > -FLT_EPSILON);
			FLOAT fGap = m_DescStruct.m_fDummyLenSize * 2 / (uPointCountInAxis - 1);
			FLOAT fStartPos = -m_DescStruct.m_fDummyLenSize;
			
			for (UINT x = 0; x < uPointCountInAxis; x++)
			{
				for (UINT y = 0; y < uPointCountInAxis; y++)
				{
					D3DVECTOR& Vertex = pBuffer[x*uPointCountInAxis + y];
					_ASSERTE(&Vertex < pBuffer +  EM_OC_POINT_COUNT);
					Vertex.x = fStartPos + x * fGap;
					Vertex.y = fStartPos + y * fGap;
					Vertex.z = 1;//��ʵ������XY�͹��ˣ���FVF����û��FVF_XY��дDeclaration��̫�鷳��
				}
			}

			m_pOCVertexBuffer->Unlock();
		}
Exit0:
		return;
	}

	HRESULT KG3DMeshLensFlare::ReloadShader()
	{
		return m_shaderHolder.Reload();
	}

	bool KG3DMeshLensFlare::IsSunOnTheBackSideOfCamera( D3DXVECTOR4& vSunPosPostProj )
	{
		return vSunPosPostProj.w < 0;
	}

	/*HRESULT KG3DMeshLensFlare::RecordEffectParamHandles()
	{
		LPD3DXEFFECT pEffect = m_shaderHolder.GetEffect();
		ZeroMemory(m_ShaderHandles, _countof(m_ShaderHandles)*sizeof(D3DXHANDLE));

		m_ShaderHandles[EM_MATRIX_ROTATION] 
			= pEffect->GetParameterByName(NULL, _T("g_matRotation"));
		m_ShaderHandles[EM_FLOAT2_SUN_POS_POSTPROJ] 
			= pEffect->GetParameterByName(NULL, _T("g_vSunPosPostProj"));
		m_ShaderHandles[EM_FLOAT2_TEXTURE_SPLIT] 
			= pEffect->GetParameterByName(NULL, _T("g_vTexSplit"));
		m_ShaderHandles[EM_FLOAT_BLEND_FACTOR] 
			= pEffect->GetParameterByName(NULL, _T("g_fBlendFactor"));
		m_ShaderHandles[EM_FLOAT_OC_FACTOR]
			= pEffect->GetParameterByName(NULL, _T("g_fOCFactor"));
		m_ShaderHandles[EM_FLOAT_SCREEN_ASPECT]
			= pEffect->GetParameterByName(NULL, _T("g_fScreenAspect"));

		for (size_t i = 0; i < _countof(m_ShaderHandles); ++i)
		{
			KG_PROCESS_ERROR(0 != m_ShaderHandles[i]);
		}
		return S_OK;
Exit0:
		return E_FAIL;
	}*/


	/************************************************************************/
	/* KG3DEnvEffLensFlareSun
	*/
	/************************************************************************/
	
	KG3DEnvEffLensFlareSun::KG3DEnvEffLensFlareSun()
		:KG3DMeshLensFlare(0, KG3DTYPE_ENVEFF_LENSFLARE_SUN)
	{
	}

	KG3DEnvEffLensFlareSun::~KG3DEnvEffLensFlareSun()
	{

	}

	/************************************************************************/
	/* KG3DEnvEffLensFlareMoon 
	*/
	/************************************************************************/

	

	KG3DEnvEffLensFlareMoon::KG3DEnvEffLensFlareMoon()
		:KG3DMeshLensFlare(0, KG3DTYPE_ENVEFF_LENSFLARE_MOON)
	{
	}

	KG3DEnvEffLensFlareMoon::~KG3DEnvEffLensFlareMoon()
	{

	}

	HRESULT KG3DEnvEffLensFlareMoon::SaveMapSetting( IIniFile* pIniFile, LPCTSTR  )
	{
		return KG3DMeshLensFlare::SaveMapSetting(pIniFile, _T("LensFlareMoon"));
	}

	HRESULT KG3DEnvEffLensFlareMoon::LoadMapSetting( IIniFile* pIniFile, LPCTSTR  )
	{
		return KG3DMeshLensFlare::LoadMapSetting(pIniFile, _T("LensFlareMoon"));
	}

	void KG3DEnvEffLensFlareMoon::GetDefaultDesc( IEKG3DMeshLensFlareDescStruct* pDescStruct )
	{
		KG3DMeshLensFlare::GetDefaultDesc(pDescStruct);
		pDescStruct->m_nTexSplitNumX = 1;
		pDescStruct->m_nTexSplitNumY = 1;
		_tcscpy_s(pDescStruct->m_tczTexName, _T("data\\public\\defaultscene\\MoonFlare.tga"));
	}
	/************************************************************************/
	/* KG3DDWGC_Key_LensFlare
	*/
	/************************************************************************/
	KG3DDWGC_Key_LensFlare::KG3DDWGC_Key_LensFlare()
	{
		m_fVisibility = 1.0;
	}
	bool KG3DDWGC_Key_LensFlare::CalculateInterp( const KG3DDWGC_KeyBase& BaseKeyBefore, const KG3DDWGC_KeyBase& BaseKeyAfter,  float fPercent)
	{
		///ע��ǿת�Ŀ�����
		_KG3D_DEBUG_STATIC_ASSERTE((void*)dynamic_cast<const KG3DDWGC_Key_LensFlare*>(&BaseKeyBefore) == (void*)(const KG3DDWGC_Key_LensFlare*)(&BaseKeyBefore));
		KG3DDWGC_Key_LensFlare& KeyBefore = (KG3DDWGC_Key_LensFlare&)BaseKeyBefore;
		KG3DDWGC_Key_LensFlare& KeyAfter = (KG3DDWGC_Key_LensFlare&)BaseKeyAfter;

		MT_FLOAT_BLEND(KeyBefore.m_fVisibility, KeyAfter.m_fVisibility, m_fVisibility, fPercent);

		return true;
	}

	bool KG3DDWGC_Key_LensFlare::SaveMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
	{
		_ASSERTE(pIniFile && pSection);
		KG_CUSTOM_HELPERS::SaveINIFileData(pIniFile, pSection, _T("fVisibility"), m_fVisibility);
		return true;
	}

	bool KG3DDWGC_Key_LensFlare::LoadMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
	{
		_ASSERTE(pIniFile && pSection);
		KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, _T("fVisibility"), -1.0f, &m_fVisibility);
		if (m_fVisibility < 0)
		{
			return false;
		}
		return true;
	}

#if defined(_DEBUG) | defined(DEBUG)
	void KG3DDWGC_Key_LensFlare::DebugPrint(std::ostringstream& OSS )
	{
		OSS << "Vib: " << m_fVisibility;
	}
#endif

	/************************************************************************/
	/*KG3DMeshLensFlareDescStruct 
	*/
	/************************************************************************/
	KG3DMeshLensFlareDescStruct::KG3DMeshLensFlareDescStruct()
	{
		_ASSERTE(m_nTexSplitNumX == m_nTexSplitNumY && m_nTexSplitNumX == 1);///ȷ��IE�Ĺ��캯����ִ��
	}

	KG3DMeshLensFlareDescStruct::KG3DMeshLensFlareDescStruct( const KG3DMeshLensFlareDescStruct& Other )
	{	
		KG_CUSTOM_HELPERS::TTypeCopy<IEKG3DMeshLensFlareDescStruct>(this, &Other);
	}
	KG3DMeshLensFlareDescStruct& KG3DMeshLensFlareDescStruct::operator=(const KG3DMeshLensFlareDescStruct& Other)
	{
		KG_CUSTOM_HELPERS::TTypeCopy<IEKG3DMeshLensFlareDescStruct>(this, &Other);
		return *this;
	}
	bool KG3DMeshLensFlareDescStruct::ToKey( KG3DDWGC_Key_LensFlare& Key )
	{
		Key.m_fVisibility = this->m_fVisibility;
		return true;
	}

	bool KG3DMeshLensFlareDescStruct::SaveMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
	{
		if(!pIniFile || !pSection || !m_tczTexName)
			return false;
		KG_CUSTOM_HELPERS::SaveINIFileData(pIniFile, pSection, _T("Texture"), m_tczTexName);
		KG_CUSTOM_HELPERS::SaveINIFileData(pIniFile, pSection, _T("Tex_Num_X"), m_nTexSplitNumX);
		KG_CUSTOM_HELPERS::SaveINIFileData(pIniFile, pSection, _T("Tex_Num_Y"), m_nTexSplitNumY);
		KG_CUSTOM_HELPERS::SaveINIFileData(pIniFile, pSection, _T("fVisibility"), m_fVisibility);
		KG_CUSTOM_HELPERS::SaveINIFileData(pIniFile, pSection, _T("fBound"), m_fVisibleOutterBoundAspect);
		KG_CUSTOM_HELPERS::SaveINIFileData(pIniFile, pSection, _T("DummyLenSize"), m_fDummyLenSize);
		return true;
	}

	bool KG3DMeshLensFlareDescStruct::LoadMapSetting( IIniFile* pIniFile, LPCTSTR pSection )
	{
		if(!pIniFile || !pSection)
			return false;
		bool bRet = true;
		bRet &= KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, _T("Texture")
			, _T(""), m_tczTexName, _countof(m_tczTexName));
		bRet &= KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, _T("Tex_Num_X"), 1, &m_nTexSplitNumX);
		bRet &= KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, _T("Tex_Num_Y"), 1, &m_nTexSplitNumY);
		KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, _T("fVisibility"), 1.0f, &m_fVisibility);///����ʧ��
		KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, _T("fBound"), 1.0f, &m_fVisibleOutterBoundAspect);
		KG_CUSTOM_HELPERS::LoadINIFileData(pIniFile, pSection, _T("DummyLenSize"), 0.04f, &m_fDummyLenSize);
		if (m_fDummyLenSize > 1)
		{
			m_fDummyLenSize /= 1024;	///�ɸ�ʽ
		}
		return bRet;
	}

	bool KG3DMeshLensFlareDescStruct::SynWithKey(const KG3DDWGC_Key_LensFlare& Key )
	{
		m_fVisibility = Key.m_fVisibility;
		return true;
	}

#ifdef KG_ENABLE_TDD
KG_TEST_CLASS_TEST_BEGIN(KG3DMeshLensFlare)
{
	LensFlareInfo	standardInfo;
	standardInfo.nIndexX = 0;
	standardInfo.nIndexY = 0;
	standardInfo.fHeight = 0.5f;
	standardInfo.fSize = 0.1f;
	standardInfo.fRate = 0;

	struct KG3DMeshLensflareTest : public KG3DMeshLensFlare
	{
		KG3DMeshLensflareTest()
			:KG3DMeshLensFlare(0, KG3DTYPE_ENVEFF_LENSFLARE_SUN)
		{

		}
		virtual VOID FillVertexBuffer(){}
		virtual HRESULT PrepareRenderSouce(){return S_OK;}
	};

	//����ValidateLensFlareInfo
	{
	
		KG3DMeshLensflareTest testObj;
		testObj.GetDefaultDesc(&testObj.m_DescStruct);
			
		_ASSERTE(testObj.ValidateLensFlareInfo(standardInfo));
		{
			LensFlareInfo tempInfo = standardInfo;
			tempInfo.nIndexX = testObj.m_DescStruct.m_nTexSplitNumX;
			_ASSERTE(! testObj.ValidateLensFlareInfo(tempInfo));
		}
		{
			LensFlareInfo tempInfo = standardInfo;
			tempInfo.nIndexY = testObj.m_DescStruct.m_nTexSplitNumY;
			_ASSERTE(! testObj.ValidateLensFlareInfo(tempInfo));
		}
		{
			LensFlareInfo tempInfo = standardInfo;
			tempInfo.fHeight = 2.1f;
			_ASSERTE(! testObj.ValidateLensFlareInfo(tempInfo));
			tempInfo.fHeight = -2.1f;
			_ASSERTE(! testObj.ValidateLensFlareInfo(tempInfo));
		}
		{
			LensFlareInfo tempInfo = standardInfo;
			tempInfo.fSize = 2.1f;
			_ASSERTE(! testObj.ValidateLensFlareInfo(tempInfo));
			tempInfo.fSize = -FLT_EPSILON;
			_ASSERTE(! testObj.ValidateLensFlareInfo(tempInfo));
		}
	}
	//����AddLensflare��SetLensflare
	{
		KG3DMeshLensflareTest testObj;
		testObj.GetDefaultDesc(&testObj.m_DescStruct);

		LensFlareInfo tempInfo = standardInfo;
		HRESULT hr = E_FAIL;
		hr = testObj.AddLensFlare(tempInfo, FALSE);
		_ASSERTE(SUCCEEDED(hr) && testObj.GetLensFlareCount() == 1);

		tempInfo.fHeight = 1;
		hr = testObj.AddLensFlare(tempInfo, FALSE);	
		_ASSERTE(SUCCEEDED(hr) && testObj.GetLensFlareCount() == 2);
		_ASSERTE(tempInfo.fHeight == testObj.m_vecLensFlare[0].fHeight && _T("����Ӧ��������"));

		tempInfo.fHeight = 2.1f;
		hr = testObj.AddLensFlare(tempInfo, FALSE);	
		_ASSERTE(FAILED(hr) && testObj.GetLensFlareCount() == 2);

		tempInfo.fHeight = 0;
		size_t uAlterPos = 0;
		hr = testObj.SetLensFlareItem(0, &tempInfo, &uAlterPos);
		_ASSERTE(SUCCEEDED(hr));
		_ASSERTE(tempInfo.fHeight == testObj.m_vecLensFlare[1].fHeight && _T("����Ӧ��������"));

		tempInfo.fHeight = 0;
		tempInfo.fSize = 2.1f;
		hr = testObj.SetLensFlareItem(0, &tempInfo, &uAlterPos);
		_ASSERTE(FAILED(hr));
	}
}
KG_TEST_CLASS_TEST_END(KG3DMeshLensFlare)


#endif 
};