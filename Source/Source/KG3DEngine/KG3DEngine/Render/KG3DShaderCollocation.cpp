////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DShaderCollocation.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-7-17 16:08:22
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#include "stdAfx.h"
#include "KG3DShaderCollocation.h"
#include "KG3DDefaultShaderGroup.h"
#include "KG3DSceneCheckers.h"
#include "KG3DShaderRCBridge.h"
#include "KG3DShaderManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////
//#define MY_MAKESHADERIDSTRING(ID)	KG3DShaderRCBridge::GetStringFromID(ID, strBuffer)

#define USE_RESOURCE_SHADER	0	//�µ�����ʹ����Դdll����shader��������ļ��ڸ�����֧��ȫ�ύ�����֮ǰ����ʱ�������ļ���֮�����һ���Ӿ��л���ȥ

#if	USE_RESOURCE_SHADER == 1
#define MAKESHADERSTRING(path)	KG3DShaderManager::GetResourceString(path)
#else
#define MAKESHADERSTRING(path)	path
#endif

VOID KG3DShaderCollocation::Collocate()
{
	//int* p = new int;
#if USE_RESOURCE_SHADER == 1
	KG3DShaderRCBridge::InitLibrary();
#endif

	//����Ĭ�ϵ�Shader,�밴����ö��ֵ��˳����д���������
	DWORD dwDefaultShaderCompileFlag = 0;

	//ע��D3DXMACRO_ARRAY�����Ǹ�ָ�룬D3DXMACRO_ARRAYֱ�Ӽ�¼��DefaultShaderGroup���档�����Ǹ�ָ��Ҫô��NULL��Ҫô
	//��֤��ָ����ȷ���ڴ֡�����ȻDefaultShaderGroup��static�ģ��������ָ���Ҳ�Ǿ�̬���ڴ棬����Ĵ��룬֧��һ��Buffer
	D3DXMACRO_ARRAY macroDefault = D3DXMACRO_ARRAY::GetEmptyMacroArray();//���Macro��ȫ��shader�����õģ����Կ�����һЩȫ�ֵĶ��壬������Ϊ����������ˣ������������shader��ʱ�����׼����ȷ�ԣ�������û��ǲ�Ҫ��ô��

#ifdef DEBUG_VS
	dwDefaultShaderCompileFlag |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
	dwDefaultShaderCompileFlag |= D3DXSHADER_DEBUG ;//| D3DXSHADER_SKIPOPTIMIZATION;
#endif
#ifdef DEBUG_PS
	dwDefaultShaderCompileFlag |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
	dwDefaultShaderCompileFlag |= D3DXSHADER_DEBUG ;//| D3DXSHADER_SKIPOPTIMIZATION;
#endif

	//2009-8-5 ���������ԴID��Ϊ�ļ�·����shaderManager�������ļ�·��
	KG3DDefaultShaderRegistrationArg Args[] = 
	{
		{
			DEFST_LIBRARY,
			MAKESHADERSTRING(_T("Data\\public\\shader\\Library.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\Library.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\Library.fx")),
			0,
			macroDefault, NULL, dwDefaultShaderCompileFlag, 
		},//<Shared Pool>
		{
			DEFST_SKINMESH,
			MAKESHADERSTRING(_T("Data\\public\\shader\\shaderskin_2.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\shaderskin_2.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\shaderskin_3.fx")),
			sizeof(KG3D_DEFST_SKINMESH_ParamHandle),
			macroDefault, NULL, dwDefaultShaderCompileFlag,
		},
		{
			DEFST_POST_RENDER,
			MAKESHADERSTRING(_T("Data\\public\\shader\\PostRenderFinalPass.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\PostRenderFinalPass.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\PostRenderFinalPass.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag, 
		},//<PostRenderShadowMove>
		{
			DEFST_NEWLIGHT,
			MAKESHADERSTRING(_T("Data\\public\\shader\\new light mode.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\new light mode.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\new light mode v3.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_NORMAL_NOSKIN,
			MAKESHADERSTRING(_T("Data\\public\\shader\\Normal map without skin.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\Normal map without skin.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\Normal map without skin.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_MODEL_SHADOW,
			MAKESHADERSTRING(_T("Data\\public\\shader\\ModelShadow.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\ModelShadow.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\ModelShadow.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_LIGHT_MAP,
			MAKESHADERSTRING(_T("Data\\public\\shader\\lightmap.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\lightmap.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\lightmap.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_CLOUD,
			MAKESHADERSTRING(_T("Data\\public\\shader\\cloud.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\cloud.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\cloud.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_NEWLIGHT_DETAIL,
			MAKESHADERSTRING(_T("Data\\public\\shader\\new light mode_detail_2_0.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\new light mode_detail_2_0.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\new light mode_detail.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_TERRAIN,
			MAKESHADERSTRING(_T("Data\\public\\shader\\TerrainRender_2_0.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\TerrainRender_2_0.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\TerrainRender.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_TERRAIN_NORMALMAP,
			MAKESHADERSTRING(_T("Data\\public\\shader\\TerrainRender_normalmap.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\TerrainRender_normalmap.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\TerrainRender_normalmap.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_GRASS,
			MAKESHADERSTRING(_T("Data\\public\\shader\\TerrainGrass.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\TerrainGrass.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\TerrainGrass.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_LENSFLARE,
			MAKESHADERSTRING(_T("Data\\public\\shader\\Lensflare.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\Lensflare.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\Lensflare.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_TERRAIN_ROAD,
			MAKESHADERSTRING(_T("Data\\public\\shader\\Road_2_0.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\Road_2_0.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\Road.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_SFX_BOX_PROJ,
			MAKESHADERSTRING(_T("Data\\public\\shader\\box_proj_effect.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\box_proj_effect.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\box_proj_effect.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_BILLBOARD,
			MAKESHADERSTRING(_T("Data\\public\\shader\\shaderbillboard.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\shaderbillboard.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\shaderbillboard.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_HIGH_GRASS,
			MAKESHADERSTRING(_T("Data\\public\\shader\\Grass.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\Grass.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\Grass.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_SPEED_TREE,
			MAKESHADERSTRING(_T("Data\\public\\shader\\SpeedTree.fx")),	
			MAKESHADERSTRING(_T("Data\\public\\shader\\SpeedTree.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\SpeedTree v3.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_WATER,
			MAKESHADERSTRING(_T("Data\\public\\shader\\water.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\water.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\water.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_SKYBOX,
			MAKESHADERSTRING(_T("Data\\public\\shader\\SkyBox.fx")),	
			MAKESHADERSTRING(_T("Data\\public\\shader\\SkyBox.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\SkyBox.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_FARMOUNTAIN,
			MAKESHADERSTRING(_T("Data\\public\\shader\\FarMountain.fx")),	
			MAKESHADERSTRING(_T("Data\\public\\shader\\FarMountain.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\FarMountain.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_LIGHT_MAP_BAKE_DIRECTIONLIGHT,
			MAKESHADERSTRING(_T("Data\\public\\shader\\lightmapbakedirection.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\lightmapbakedirection.fx")),
			MAKESHADERSTRING(_T("Data\\public\\shader\\lightmapbakedirection.fx")),
			0,
			macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_SCALEOUTPUT,
				MAKESHADERSTRING(_T("Data\\public\\shader\\scaleoutput.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\scaleoutput.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\scaleoutput.fx")),
				0,
				macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_NORMAL_OUTPUT,
				MAKESHADERSTRING(_T("Data\\public\\shader\\NormalOutput.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\NormalOutput.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\NormalOutput.fx")),
				0,
				macroDefault, NULL,dwDefaultShaderCompileFlag,
		},	
		{
			DEFST_BILLBOARDCLOUD,
				MAKESHADERSTRING(_T("Data\\public\\shader\\BillboardCloud.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\BillboardCloud.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\BillboardCloud.fx")),
				0,
				macroDefault, NULL,dwDefaultShaderCompileFlag,
		},	
		{
			DEFST_LIGHT_MAP_BAKE_POINTLIGHT,
				MAKESHADERSTRING(_T("Data\\public\\shader\\lightmapbakepoint.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\lightmapbakepoint.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\lightmapbakepoint.fx")),
				0,
				macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
		{
			DEFST_LINEARDEPTHOUTPUT,
				MAKESHADERSTRING(_T("Data\\public\\shader\\LinearDepthoutput.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\LinearDepthoutput.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\LinearDepthoutput.fx")),
				0,
				macroDefault, NULL,dwDefaultShaderCompileFlag,
		},		
		{
			DEFST_LIGHT_MAP_BAKE_ENVLIGHT,
				MAKESHADERSTRING(_T("Data\\public\\shader\\lightmapbakeEnv.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\lightmapbakeEnv.fx")),
				MAKESHADERSTRING(_T("Data\\public\\shader\\lightmapbakeEnv.fx")),
				0,
				macroDefault, NULL,dwDefaultShaderCompileFlag,
		},
			//�µ�Shader������������ܼ򵥣���һ��DEFST_XX��ֵ��Ȼ��������ṹ����ˣ�ע�ⰴenum��˳�����Ȼ��Assert
	};

	KG3DDefaultShaderGroup& ShaderGroup = g_GetDefaultShaderGroup();
	HRESULT hr = ShaderGroup.Init(Args, _countof(Args));
	_ASSERTE(SUCCEEDED(hr));

	/*���NormalChecker����Щ�ļ�û��ʵ����������ʹ�ã���������Լ�����Ҫ�Ĵ�NormalChecker���ܵ�shader���궨������˼�飬��
	NormalChecker������������Ҫ��֤
	*/
	KG3DNormalChekerShaderArg NormalCheckerArg[] = 
	{
		{
			DEFST_NEWLIGHT
			, MAKESHADERSTRING(_T("Data\\public\\shader\\new light mode_checker.fx")),
		},
		{
			DEFST_SKINMESH
			,MAKESHADERSTRING(_T("Data\\public\\shader\\shaderskin_2_checker.fx")),
		},
		{
			DEFST_NEWLIGHT_DETAIL
			, MAKESHADERSTRING(_T("Data\\public\\shader\\new light mode_detail_checker.fx")),
		},
		{
			DEFST_TERRAIN
			, MAKESHADERSTRING(_T("Data\\public\\shader\\TerrainRender_Checker.fx")),
		},	
	};

	hr = g_GetSceneCheckers().InitNormalCheckerShaders(NormalCheckerArg, _countof(NormalCheckerArg));

	(void)hr;
}

VOID KG3DShaderCollocation::CheckShaders()
{
	_ASSERTE(NULL != g_pd3dDevice);

	KG3DShaderHolder typeShaderHolder = g_GetDefaultShaderGroup().GetDefaultShader(DEFST_LIBRARY);
	//���NormalChecker��Shader���Ϳ⡣�������û������Cases����ָ���ı������ߴ�С��һ�£����Կ϶��κε���Ⱦ���ⶼ�п��ܳ����ˡ�
	{
		KG_PROCESS_ERROR(Private::IsShaderHolderInited(typeShaderHolder));

		struct CheckCase 
		{
			LPCTSTR strParamName;
			size_t	uSize;
		};

		//������Shader�еĲ�����ʱ������Ӧ���struct��С��һһ��Ӧ
		CheckCase checkCases[] = 
		{
			{_T("sunLight"), sizeof(KG3DLightParam)},
			{_T("playerLight"), sizeof(KG3DLightParam)},
			{_T("fogParam"), sizeof(KG3DFogParamShader)},
			{_T("materialCur"), sizeof(D3DMATERIAL9)},
			{_T("materialEx"), sizeof(KG3DShaderParamMaterialEx)},
			{_T("shadowMapParam"), sizeof(KG3DShaderParamShadowMap)},
			{_T("detailParam"), sizeof(KG3DShaderParamDetail)},
		};

		LPD3DXEFFECT pEffect = typeShaderHolder.GetEffect();
		_ASSERTE(NULL != pEffect);		

		for(size_t i = 0; i < _countof(checkCases); ++i) 
		{
			D3DXPARAMETER_DESC Desc;
			HRESULT hr = pEffect->GetParameterDesc(checkCases[i].strParamName, &Desc);
			if(FAILED(hr))
			{
				_ASSERTE("shader���Ϳ���ʧ��");
				KGLogPrintf(KGLOG_WARNING, _T("Shader���Ϳ����±�������򲻴��ڣ�����������shader����ʧ�ܣ�%s"), checkCases[i].strParamName);
			}
			KG_COM_PROCESS_ERROR(hr);

			BOOL bSizeMatch = (Desc.Bytes == checkCases[i].uSize);
			if(! bSizeMatch)
			{
				_ASSERTE("shader���Ϳ���ʧ��");
				KGLogPrintf(KGLOG_WARNING, _T("Shader���Ϳ����±�����С�������е�struct��С����Ӧ��%s"), checkCases[i].strParamName);
			}

			KG_PROCESS_ERROR(bSizeMatch);

		}while(false);


	}
	{
		_ASSERTE(! KG3DShaderRCBridge::GetShaderLibraryInterface()->IsLibraryBuildedInUnicode() && _T("shaderLibraryӦ�ò�����Unicode����ĲŶԣ������ź������CodeSetƥ��"));
	}
	return;
Exit0:
	{
		KG3DShaderBuildParams buildParams;
		g_GetShaderManager().GetShaderBuildParams(typeShaderHolder, &buildParams);
		KGLogPrintf(KGLOG_WARNING, _T("Shader���Ϳⲻ��ȷ�������ܵ�������Shader����ʧ�ܣ�%s"), buildParams.strFilePathOrResourceIDString);
	}
}

VOID KG3DShaderCollocation::UnCollocate()
{
	KG3DShaderRCBridge::UninitLibrary();//��ʹû��InitҲҪ���ã���ֹ�о�̬��Դ������
}
