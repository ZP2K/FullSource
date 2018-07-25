////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEngineTypes.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-7-23 15:10:47
//  Comment     : ������Ҫ�������ڲ������ֲ��ܷŵ��ӿڵĶ����ͷ����� 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DENGINETYPES_H_
#define _INCLUDE_KG3DENGINETYPES_H_

////////////////////////////////////////////////////////////////////////////////
enum KG3DDefaultShaderType
{
	DEFST_LIBRARY = 0,	//������Ҫ������֮���˳��
	DEFST_SKINMESH,
	DEFST_POST_RENDER,
	DEFST_NEWLIGHT,
	DEFST_NORMAL_NOSKIN,
	DEFST_MODEL_SHADOW,
	DEFST_LIGHT_MAP,		//���������ͼ�õ�Shader
	DEFST_CLOUD,
	DEFST_NEWLIGHT_DETAIL,
	DEFST_TERRAIN,
	DEFST_TERRAIN_NORMALMAP,
	DEFST_GRASS,
	DEFST_LENSFLARE,
	DEFST_TERRAIN_ROAD,
    DEFST_SFX_BOX_PROJ,
	DEFST_BILLBOARD,
	DEFST_HIGH_GRASS,
	DEFST_SPEED_TREE,
	DEFST_WATER,
	DEFST_SKYBOX,
	DEFST_FARMOUNTAIN,
	DEFST_LIGHT_MAP_BAKE_DIRECTIONLIGHT,//����ͼ�決��SHADER
	DEFST_SCALEOUTPUT,//�������
	DEFST_NORMAL_OUTPUT,//���ģ�͵�NORMAL��ģ�Ϳռ�� 
	DEFST_BILLBOARDCLOUD,//������ƵĻ���SHADER
	DEFST_LIGHT_MAP_BAKE_POINTLIGHT,//����ͼ�決��SHADER
	DEFST_LINEARDEPTHOUTPUT,//�������ת��
	DEFST_LIGHT_MAP_BAKE_ENVLIGHT,//����ͼ�決��SHADER
	DEFST_COUNT,			//���µ�Shader��ʱ�������ǰ�����
	DEFST_FORCE_DWORD = 0xffffffff,
};

#endif //_INCLUDE_KG3DENGINETYPES_H_
