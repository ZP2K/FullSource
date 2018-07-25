////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEKG3DTypes.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-4-2 15:28:42
//  Comment     : ʹ��IEģ�巽ʽ�ӿڵĲ��裺
/*
1.��KG3DTypes�м�һ��Type
2.������ļ���ADDһ��ģ��
3.��KG3DEngineManager����KG3DScene��д��Get
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IEKG3DTYPES_H_
#define _INCLUDE_IEKG3DTYPES_H_
#include "KG3DTypes.h"


//���������ڱ༭���Ľӿ�ϵͳ��
interface IEKG3DEnvEffect;

interface IEKG3DMeshSkyBox;
interface IEKG3DMeshFarMountain;
interface IEKG3DCloud;
interface IEKG3DSkySystem;
interface IEKG3DEnvEffFog;
interface IEKG3DMeshLensFlare;
interface IEKG3DEnvEffLight;//��IEKG3DLight
interface IEKG3DEnvEffLightSet;//��IEKG3DLightSet
interface IEKG3DEnvEffDWGC_KeyTable0;
interface IEKG3DEnvEffDynamicWeather;
interface IEKG3DEnvEffStarryNight;

interface IEKG3DEnvEffWind;
interface IEKG3DEnvironment;

interface IEKG3DRegionInfoManager;
interface IEKG3DRegionInfoManagerTable;

interface IEKG3DSceneSceneEditor;

interface IEKG3DSceneCameraMovement;
interface IEKG3DSceneCameraAnimation;

interface IEKG3DCommonContainer;
interface IEKG3DCommonObject;
interface IEKG3DSceneObjectPlacementTool;
interface IEKG3DSceneCheckers;

interface IEKG3DCollocator;

namespace IEKG3DTYPE
{
	///��������Dummy��
	struct KG3DNullTypeAbstruct 
	{
		virtual void DoSomeThing() = 0;
	};
	struct KG3DNullType : public KG3DNullTypeAbstruct
	{
		virtual void DoSomeThing(){}
	};

	///�ػ�ģ��Ļ���ģ��
	template<DWORD dwTypeAbstruct, DWORD dwTypeSolid>//ǰ����ǻ��࣬�������������ʵ����
	struct TKG3DDWORD_TO_IEINTERFACE	//������DWORD������ָ���õ�Interface
	{
		enum{bIsAbstract = false};
		typedef KG3DNullType Result;//���ػ����޷�ʹ��
	};

	//��һ������ΪNULL���ػ�
	template<DWORD dwTypeSolid>
	struct TKG3DDWORD_TO_IEINTERFACE<NULL, dwTypeSolid>
	{
		enum{bIsAbstract = false};
		typedef KG3DNullType Result;//���ػ����޷�ʹ��
	};

	///��Interface���������õ�����DWORD
	template<typename _Interface>
	struct TKG3DIEINTERFACE_TO_DWORD;

	///ȫ�ػ�ģ��
#define _KG3D_ADD_AN_IETYPE(dwType, IEInterface, IsAbstract)	\
	template<>	\
	struct TKG3DDWORD_TO_IEINTERFACE<NULL, dwType>	\
	{	\
		enum	\
		{	\
			bIsAbstract = IsAbstract,	\
		};	\
		typedef IEInterface Result;	\
	};	\
	template<>	\
	struct TKG3DIEINTERFACE_TO_DWORD<IEInterface>	\
	{	\
		enum	\
		{	\
			bIsAbstract = IsAbstract,	\
		};	\
		enum{_type_dword1 = NULL, _type_dword2 = dwType,};	\
	}

//�е�DWORD��Ӧ������ͬ�Ľӿڣ������ظ����TKG3DIEINTERFACE_TO_DWORD
#define _KG3D_ADD_AN_IETYPE_NO_REVERSE(dwType, IEInterface, IsAbstract)	\
	template<>	\
	struct TKG3DDWORD_TO_IEINTERFACE<NULL, dwType>	\
	{	\
		enum	\
		{	\
			bIsAbstract = IsAbstract,	\
		};	\
		typedef IEInterface Result;	\
	}

#define  _KG3D_ADD_AN_IETYPE2(dwTypeTypeBase, dwType, IEInterface)	\
	template<>	\
	struct TKG3DDWORD_TO_IEINTERFACE<dwTypeTypeBase, dwType>	\
	{	\
		enum	\
		{	\
			bIsAbstract = false,	\
		};	\
		typedef IEInterface Result;	\
	};	\
	template<>	\
	struct TKG3DIEINTERFACE_TO_DWORD<IEInterface>	\
	{	\
		enum	\
		{	\
			bIsAbstract = false,	\
		};	\
		enum{_type_dword1 = dwTypeTypeBase, _type_dword2 = dwType,};	\
	}

	_KG3D_ADD_AN_IETYPE(KG3DTYPE_NONE_ABSTRUCT, KG3DNullTypeAbstruct, true);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_NONE, KG3DNullType, false);

	_KG3D_ADD_AN_IETYPE(KG3DTYPE_SCENE, IEKG3DScene, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_SCENE_SCENE_EDITOR, IEKG3DSceneSceneEditor, false);

	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVIRONMENT, IEKG3DEnvironment, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENV_EFFECT, IEKG3DEnvEffect, true);///������
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_KEY_TABLE, IEKG3DEnvEffDWGC_KeyTable0, true);///������
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_SKYSYSTEM_SYSTEM, IEKG3DSkySystem, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_SKYSYSTEM_SKYBOX, IEKG3DMeshSkyBox, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_SKYSYSTEM_FARMOUNTAIN, IEKG3DMeshFarMountain, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_SKYSYSTEM_CLOUD, IEKG3DCloud, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_FOG, IEKG3DEnvEffFog, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_LENSFLARE_SUN, IEKG3DMeshLensFlare, false);//ע�������Moon��ͬһ���ӿڣ�������TKG3DINTERFACE_TO_DWORD
	_KG3D_ADD_AN_IETYPE_NO_REVERSE(KG3DTYPE_ENVEFF_LENSFLARE_MOON, IEKG3DMeshLensFlare, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_LIGHTSET, IEKG3DEnvEffLightSet, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_LIGHT, IEKG3DEnvEffLight, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_DYNAMIC_WEATHER, IEKG3DEnvEffDynamicWeather, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_STARRY_NIGHT, IEKG3DEnvEffStarryNight, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_ENVEFF_WIND, IEKG3DEnvEffWind, false);

	_KG3D_ADD_AN_IETYPE(KG3DTYPE_REGION_INFO_MANAGER, IEKG3DRegionInfoManager, false);//������õ����Ƕ�̬�����Ǹ�RIM
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_CAM_MOVEMENT, IEKG3DSceneCameraMovement, false);
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_CAM_ANIMATION, IEKG3DSceneCameraAnimation, false);

	_KG3D_ADD_AN_IETYPE(KG3DTYPE_SCENE_OBJECT_PLACEMENT_TOOL, IEKG3DSceneObjectPlacementTool, false);

	_KG3D_ADD_AN_IETYPE2(KG3DTYPE_RESOURCE_TABLE_TYPE, KG3DTYPE_REGION_INFO_MANAGER, IEKG3DRegionInfoManagerTable);

	_KG3D_ADD_AN_IETYPE(KG3DTYPE_COMMON_OBJECT_CONTAINER, IEKG3DCommonContainer, true);	//������
	_KG3D_ADD_AN_IETYPE(KG3DTYPE_COMMON_OBJECT, IEKG3DCommonObject, true);	//������

	_KG3D_ADD_AN_IETYPE(KG3DTYPE_SCENE_CHECKERS, IEKG3DSceneCheckers, false);

	_KG3D_ADD_AN_IETYPE(KG3DTYPE_COLLOCATOR, IEKG3DCollocator, false);
	
};


#endif //_INCLUDE_IEKG3DTYPES_H_
