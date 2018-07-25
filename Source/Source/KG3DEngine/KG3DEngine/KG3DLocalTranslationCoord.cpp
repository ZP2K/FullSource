////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DLocalTranslationCoord.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-11-17 15:06:02
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG3DLocalTranslationCoord.h"

#include "KG3DBaseCoordImp.h"
#include "KG3DFrameDrawer.h"
#include "KG3DMeshHelpers.h"

#include "IEEditorTypes.h"

class KG3DTranslationCoordinateSolid : public KG3DBaseCoordImp
{
	friend KG3DBaseCoord* BuildKG3DTranslationCoordinateSolid( LPCTSTR, DWORD_PTR pScene, DWORD_PTR );

	STDMETHOD_(ULONG, GetType)(){return KG3DTYPE_COORD_TRANSLATION;}
	STDMETHOD(Init)();
	virtual	ULONG GetActivationState(){return SCENESTATE_MOVE;}
	STDMETHOD(Render)();
	virtual D3DXMATRIX GetTrans();
	KG3DTranslationCoordinateSolid(KG3DMessageBroadcasterGroup& Sc):KG3DBaseCoordImp(Sc){}
};
HRESULT STDMETHODCALLTYPE KG3DTranslationCoordinateSolid::Init()
{
	RepresentData	rpData;

	_ASSERTE(NULL == m_RepresentData.MeshForAxis[0] && _T("�ظ��ĳ�ʼ��"));

	HRESULT hr = E_FAIL;
	//������һ����Mesh����X�ᣬλ����ԭ��
	{
		hr = D3DXCreateCylinder(g_pd3dDevice, em_mesh_inner_radius, em_mesh_inner_radius
			, em_mesh_range, em_mesh_sides, 1, &rpData.MeshForAxis[0], NULL);
		KG_COM_PROCESS_ERROR(hr);

		D3DXMATRIX matTransform;
		{
			D3DXMATRIX matTranslation;
			D3DXMatrixTranslation(&matTranslation, em_mesh_range/2, 0, 0);	//��ΪD3DXĬ�ϴ����������Ǹ�Բ����Z����ģ��е���ԭ��ģ�����Ҫת��Y���򣬲�����ȥ���Ƶ��ײ���ԭ��
			D3DXMATRIX matRotation;
			D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0,1,0), D3DX_PI / 2);
			matTransform = matRotation * matTranslation;
		}

		hr = D3DXMeshTransformation(rpData.MeshForAxis[0], &matTransform, NULL);
		KG_COM_PROCESS_ERROR(hr);
	}

	//������һ����Mesh������ΪX��
	{
		float fBoxRange = (FLOAT)em_mesh_plane_range;
		hr = D3DXCreateBox(g_pd3dDevice, em_mesh_inner_radius, fBoxRange, fBoxRange, &rpData.MeshForPlane[0], NULL);;
		KG_COM_PROCESS_ERROR(hr);

		D3DXMATRIX matTranslation;
		D3DXMatrixTranslation(&matTranslation, 0, fBoxRange/2, fBoxRange/2);

		hr = D3DXMeshTransformation(rpData.MeshForPlane[0], &matTranslation, NULL);
		KG_COM_PROCESS_ERROR(hr);
	}
	//���Ƴ������������������棬���Ƴ�����������Mesh
	{
		_ASSERTE(NULL != rpData.MeshForAxis[0] && NULL != rpData.MeshForPlane[0]);
		for (size_t i = 1; i < _countof(rpData.MeshForAxis); ++i)
		{
			hr = D3DXMeshClone(rpData.MeshForAxis[0], &rpData.MeshForAxis[i]);
			KG_COM_PROCESS_ERROR(hr);

			const D3DXMATRIX& matRotation = D3DXMatrixGetRotationMatrixOfAxis(i);
			hr = D3DXMeshTransformation(rpData.MeshForAxis[i], &matRotation, NULL);
			KG_COM_PROCESS_ERROR(hr);
		}

		{
			hr = D3DXMeshClone(rpData.MeshForPlane[0], &rpData.MeshForPlane[1]);
			KG_COM_PROCESS_ERROR(hr);

			D3DXMATRIX matRotation;
			D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0,0,1), -D3DX_PI / 2);

			hr = D3DXMeshTransformation(rpData.MeshForPlane[1], &matRotation, NULL);
			KG_COM_PROCESS_ERROR(hr);

			hr = D3DXMeshClone(rpData.MeshForPlane[0], &rpData.MeshForPlane[2]);
			KG_COM_PROCESS_ERROR(hr);

			D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(0,1,0), D3DX_PI / 2);
			hr = D3DXMeshTransformation(rpData.MeshForPlane[2], &matRotation, NULL);
			KG_COM_PROCESS_ERROR(hr);
		}
	}
	
	m_RepresentData = rpData;
	ZeroMemory(&rpData, sizeof(rpData));

	hr = KG3DBaseCoordImp::Init();
	KG_COM_PROCESS_ERROR(hr);

	//��Ϊ������ʱ���ǰ���Y�ᣬZ�ᣬX���˳�򴴽��ģ�
	return S_OK;
Exit0:
	rpData.ReleaseMeshes();
	return E_FAIL;
}

HRESULT KG3DTranslationCoordinateSolid::Render()
{
	HRESULT hr = E_FAIL;
	_ASSERTE(NULL != m_pStateMachine);
	if(this->GetCurStateID() == EM_EDITOR_TOOL_STATE_NOTATIVATED)
		return S_OK;

	const D3DCOLORVALUE selectedColor = Color2Value(0xfff2d900);
	const D3DCOLORVALUE axisColor[] = {Color2Value(0xffc80000), Color2Value(0xff0000c3), Color2Value(0xff00cc00)};
	const D3DCOLORVALUE selectedColorAlpha = Color2Value(0x50f2d900);

	D3DXMATRIX matView; 
	D3DXMATRIX matProj;
	g_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	//ÿ֡�õ�һ��Cameraλ��
	{
		D3DXVECTOR3 camPos;
		if(NULL != D3DXMatrixGetPosOfView(&matView, &camPos))
		{
			m_RepresentData.CamPos = camPos;
		}
	}

	FLOAT RadiusOfSelector = GetSelectorRingRadius(m_RepresentData);

	//��Ͷ������ZNear��ZFarѹ��ѡ��Ȧ���ߣ�����ѡ��Ȧ�����ӽǱ䶯��ʱ�򱣳ִ�С�Ҳ��ᱻ�ڵ�
	D3DXMATRIX matCustomProj = matProj;
	{
		const FLOAT fProjZScale = 0.0001f;
		matCustomProj._33 *= fProjZScale;
		matCustomProj._43 *= fProjZScale;
	}

#if 0//��Ⱦ��ײȦ�����ڼ���Ƿ���ȷ��ײ
	RenderIntersectMesh(m_RepresentData, matView, matProj);
#endif

#if 1
	D3DXMATRIX matWorld;
	{
		D3DXMatrixScaling(&matWorld, RadiusOfSelector,RadiusOfSelector,RadiusOfSelector);
		D3DXMatrixGetTranslationPart(matWorld) = m_RepresentData.vecPos;
	}
	KG3DFrameDrawerBasic& Drawer = g_GetFrameDrawerBasic();
	hr = Drawer.Begin(&matWorld, &matView, &matCustomProj);
	if (SUCCEEDED(hr))
	{
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		GraphicsStruct::RenderState rsZFunc(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		_ASSERTE(KG3DCOORD_AXIS_COUNT <= 3 && KG3DCOORD_PLANE_COUNT <= 3 && _T("������ڴ�Խ��"));
		_ASSERTE(KG3DFrameDrawer::em_fvf == D3DFVF_XYZ);
		
		
		struct AxisLine 
		{
			D3DXVECTOR3 Start;
			D3DXVECTOR3 End;
		};
		const FLOAT fAxisBegin = 1.0f/4.0f;
		
		static AxisLine axisLineBuffer[KG3DCOORD_AXIS_COUNT] = 
		{
			{D3DXVECTOR3(fAxisBegin, 0, 0), D3DXVec3GetNormalOfPlane(0)},
			{D3DXVECTOR3(0, fAxisBegin, 0), D3DXVec3GetNormalOfPlane(1)},
			{D3DXVECTOR3(0, 0, fAxisBegin), D3DXVec3GetNormalOfPlane(2)},
		};

		//��3����
		for(int i = 0; i < KG3DCOORD_AXIS_COUNT; ++i)
		{
			INT iAxisIndex = m_ResultData.IntersectedCoord - KG3DCOORD_FIRST_AXIS;
			(iAxisIndex == i) ? Drawer.SetColor(selectedColor) : Drawer.SetColor(axisColor[i]);

			AxisLine& lineBuffer = axisLineBuffer[i];
			g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &lineBuffer, sizeof(D3DXVECTOR3));
		}

		const FLOAT fFaceRange = 1.0f/ 3.0f;
		//����������
		{
			struct PlaneEdge 
			{
				D3DXVECTOR3 OneEnd;
				D3DXVECTOR3 Mid;
				D3DXVECTOR3 AnotherEnd;
			};

			
			static PlaneEdge PlaneEdgeBuffer[KG3DCOORD_AXIS_COUNT] = 
			{
				{D3DXVECTOR3(fFaceRange, 0, fFaceRange), D3DXVECTOR3(fFaceRange, 0, 0), D3DXVECTOR3(fFaceRange,fFaceRange,0)},
				{D3DXVECTOR3(fFaceRange,fFaceRange,0),D3DXVECTOR3(0,fFaceRange,0),D3DXVECTOR3(0,fFaceRange,fFaceRange)},
				{D3DXVECTOR3(0,fFaceRange,fFaceRange),D3DXVECTOR3(0,0,fFaceRange),D3DXVECTOR3(fFaceRange,0,fFaceRange)},
			};
			
			for (int i = 0; i < KG3DCOORD_AXIS_COUNT; ++i)
			{
				Drawer.SetColor(axisColor[i]);
				g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 2, &PlaneEdgeBuffer[i], sizeof(D3DXVECTOR3));
			}

			
		}
		//��ѡ�������ѡ�б���
		{
			struct SelPlane 
			{
				D3DXVECTOR3 v1,v2,v3,v4;
			};

			static SelPlane selectedRectBuffer[] = {
				{D3DXVECTOR3(0,0,fFaceRange), D3DXVECTOR3(0,fFaceRange,fFaceRange), D3DXVECTOR3(0,fFaceRange,0), D3DXVECTOR3(0,0,0)},
				{D3DXVECTOR3(0,0,fFaceRange), D3DXVECTOR3(fFaceRange, 0,fFaceRange), D3DXVECTOR3(fFaceRange,0,0), D3DXVECTOR3(0,0,0)},
				{D3DXVECTOR3(0,fFaceRange,0), D3DXVECTOR3(fFaceRange,fFaceRange,0), D3DXVECTOR3(fFaceRange,0,0), D3DXVECTOR3(0,0,0)},
			};
			if (m_ResultData.IntersectedCoord >= KG3DCOORD_FIRST_PLANE && m_ResultData.IntersectedCoord < KG3DCOORD_FIRST_PLANE + KG3DCOORD_PLANE_COUNT)
			{
				UINT iIndexOfPlane = m_ResultData.IntersectedCoord - KG3DCOORD_FIRST_PLANE;
				
				Drawer.SetColor(selectedColorAlpha);

				g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &selectedRectBuffer[iIndexOfPlane], sizeof(D3DXVECTOR3));

				Drawer.SetColor(selectedColor);

				g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 2, &selectedRectBuffer[iIndexOfPlane], sizeof(D3DXVECTOR3));

				UINT iPreIndex = (iIndexOfPlane - 1 + KG3DCOORD_PLANE_COUNT) % KG3DCOORD_PLANE_COUNT;
				UINT iNextIndex = (iIndexOfPlane + 1) % KG3DCOORD_PLANE_COUNT;
				_ASSERTE(iPreIndex < _countof(axisLineBuffer) && iNextIndex < _countof(axisLineBuffer));
				AxisLine& preAxisLine = axisLineBuffer[iPreIndex];
				AxisLine& nextAxisLine = axisLineBuffer[iNextIndex];

				
				g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &preAxisLine, sizeof(D3DXVECTOR3));
				g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &nextAxisLine, sizeof(D3DXVECTOR3));
			}
		}

		Drawer.End();
	}

	//���������ϵļ���Բ׶
	{
		KG3DFrameDrawerCone& DrawerCone = g_GetFrameDrawerCone();
		DrawerCone.SetParameters(TRUE, FALSE, 0xffffffff, 0xffffffff);
		hr = DrawerCone.Begin(&matWorld, &matView, &matCustomProj);
		if (SUCCEEDED(hr))
		{
			const FLOAT fConeHeight = 0.3f;
			const FLOAT fConeRadius = 0.03f;
			const FLOAT fGapCenterToConeCenter = 1.0f - fConeHeight + 0.1f;
			for (size_t i = 0; i < KG3DCOORD_AXIS_COUNT; ++i)
			{
				const D3DXVECTOR3& vNormal = D3DXVec3GetNormalOfPlane(i);

				D3DXVECTOR3 vConeCenter = vNormal * fGapCenterToConeCenter;

				DrawerCone.SetColor(axisColor[i]);

				DrawerCone.DrawCone(vConeCenter, vNormal, fConeRadius, fConeHeight);
			}
			

			DrawerCone.End();
		}
	}
#endif
	return S_OK;
}

D3DXMATRIX KG3DTranslationCoordinateSolid::GetTrans()
{
	D3DXMATRIX matTranslation;
	D3DXVECTOR3 vTrans = m_ResultData.EndPos - m_ResultData.StartPos;
	D3DXMatrixTranslation(&matTranslation, vTrans.x, vTrans.y, vTrans.z);

	return matTranslation;
}

KG3DBaseCoord* BuildKG3DTranslationCoordinateSolid( LPCTSTR, DWORD_PTR pKG3DMessageBroadcasterGroup, DWORD_PTR )
{
	KG3DBaseCoord* p = NULL;
	HRESULT hr = E_FAIL;
 	KG3DMessageBroadcasterGroup* pBC = reinterpret_cast<KG3DMessageBroadcasterGroup*>(pKG3DMessageBroadcasterGroup);
	_ASSERTE(_tcsstr(typeid(*pBC).name(), _T("KG3DMessageBroadcasterGroup")) && _T("����Ķ������������͵�"));
	_ASSERTE(NULL != pBC);
	p = new KG3DTranslationCoordinateSolid(*pBC);
	KG_PROCESS_ERROR(NULL != p);
	hr = p->Init();
	KG_COM_PROCESS_ERROR(hr);
	return p;
Exit0:
	SAFE_RELEASE(p);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////

