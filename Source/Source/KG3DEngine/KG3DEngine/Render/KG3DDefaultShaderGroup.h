////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DDefaultShaderGroup.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-9-5 9:40:39
//  Comment     : 
/*
ʹ�÷�ʽ��
1.��KG3DTypes.h�����һ��DEFST_XX������
2.��KG3DCollocator���水˳����һ��KG3DDefaultShaderRegistrationArg
3.OK���㶨�����κεط�,g_GetDefaultShaderGroup().GetDefaultShader(DEFST_XX)�����ͺ���
4.�õ���KG3DShaderHolder�ο���h�ļ���˵��������GetEffect��Ȼ��Effect���÷��þͺ��ˡ�
�Զ��豸��ʧ��ָ������ù�������

5.�и����������⣬Effect���SetTexture��ÿ��Texture��Reference������OnLost��ʱ����ʵ
û����ȫ��ա�����ShaderManager�ṩ��KG3DShaderEventHandler���Զ���Lost��ʱ����ͷš�
�������Ӻ��鷳�����ڶ���һ�������Ĺ涨
texture������������

int gTexCount = 2;
texture gTex0;
texture gTex1;

��ôShaderGroup���Զ���������ͷ�����¡�
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DDEFAULTSHADERGROUP_H_
#define _INCLUDE_KG3DDEFAULTSHADERGROUP_H_

#include "KG3DShaderHolder.h"

class KG3DShaderEventHandler;
////////////////////////////////////////////////////////////////////////////////

enum
{
	EM_MIN_SHADER_MODEL_SUPPORTED = 1,
	EM_MAX_SHADER_MODEL_SUPPORTED = 3,
	EM_SHADER_MODEL_SUPPORTED_COUNT = EM_MAX_SHADER_MODEL_SUPPORTED - EM_MIN_SHADER_MODEL_SUPPORTED + 1,
};

struct KG3DDefaultShaderRegistrationArg 
{
	DWORD dwType;

	//���������ַ������������Դ����ʹ��MAKEINTRESOURCE������ID����ַ���ָ��
	std::tstring	tcsPathVersion1_1;	//������static�ַ�ָ��
	std::tstring	tcsPathVersion2_0; //������static�ַ�ָ��
	std::tstring	tcsPathVersion3_0;	//ͬ��

    unsigned uShaderParamHandleSize;
	
	D3DXMACRO_ARRAY	macroArray;	//Ĭ����D3DXMACRO_ARRAY::GetDefault���뱣֤Macro������ַ�ָ�����Ч����ֱ��д���ַ�����û������
	//����Ķ�����ΪNULL
	LPCTSTR		IncludeFilePath;
	DWORD		Flags;
};

struct KG3D_MaterialDetailInfo_ParamHandle
{
    D3DXHANDLE  tChannel;
    D3DXHANDLE  tDetail0;
    D3DXHANDLE  tDetail1;
    D3DXHANDLE  vEnvSpecular;
    D3DXHANDLE  vBodyScales;
    D3DXHANDLE  vDetailScale;
    D3DXHANDLE  vDetailMtl;
    D3DXHANDLE  vBodyColor;
    D3DXHANDLE  vBodyColor1; 
	D3DXHANDLE  vDetail0Color;
    D3DXHANDLE  vDetail1Color;
};

struct KG3D_LightInfo_ParamHandle
{
    D3DXHANDLE  tEnv;
    D3DXHANDLE  sunLight;
    D3DXHANDLE  playerLight;
    D3DXHANDLE  tSkinTable;
    D3DXHANDLE  eyes;
    D3DXHANDLE  Time;
    D3DXHANDLE  bShadowMask;
    D3DXHANDLE  ShadowTexture;
    D3DXHANDLE  fBiasU;
    D3DXHANDLE  fBiasV;
};

struct KG3D_DEFST_SKINMESH_ParamHandle
{
    KG3D_MaterialDetailInfo_ParamHandle   MaterialDetailInfo;
    KG3D_LightInfo_ParamHandle            LightInfo;

    D3DXHANDLE  nNumBlend;
    D3DXHANDLE  WorldViewProj;
    D3DXHANDLE  matWorld;
    D3DXHANDLE  eyes;
    D3DXHANDLE  bAlphaUseFactor;
   /* D3DXHANDLE  m_emissive;
    D3DXHANDLE  m_diffuse;
    D3DXHANDLE  m_ambient;
    D3DXHANDLE  m_specular;
	D3DXHANDLE  spe_power;*/
	D3DXHANDLE  materialCur;
    D3DXHANDLE  spe_exp;
    D3DXHANDLE  color_cast;    
    D3DXHANDLE  emssive_power;
    D3DXHANDLE  bhightlight;
};


#define KG_SHADER_SET(pShader, Function, Key, Value, pParamHandle, hrRetCode)   \
    if (!pParamHandle->Key)         \
    { \
        pParamHandle->Key = pShader->GetParameterByName(NULL, #Key);    \
        if (!pParamHandle->Key)     \
            hrRetCode = E_FAIL;     \
    }   \
    if (pParamHandle->Key)          \
    {   \
        hrRetCode = pShader->Function(pParamHandle->Key, Value); \
    }

#define KG_SHADER_SET_VALUE(pShader, Function, Key, Value, ValueSize, pParamHandle, hrRetCode) \
    if (!pParamHandle->Key) \
    {   \
        pParamHandle->Key = pShader->GetParameterByName(NULL, #Key);        \
        if (!pParamHandle->Key)     \
            hrRetCode = E_FAIL;      \
    }   \
    if (pParamHandle->Key)          \
    {   \
        hrRetCode = pShader->Function(pParamHandle->Key, Value, ValueSize);   \
    }

#define KG_SHADER_SET2(pShader, Function, Key, Value, pParamHandle, hrRetCode) \
    hrRetCode = pShader->Function(#Key, Value);


#define KG_SHADER_SET_VALUE2(pShader, Function, Key, Value, ValueSize, pParamHandle, hrRetCode) \
    hrRetCode = pShader->Function(#Key, Value, ValueSize);

//�����KG3DShaderParam��ͷ��Struct����Shader��Struct��һһ��Ӧ��
//<KG3DShaderParam>
struct KG3DShaderParamMaterialEx 
{
	INT   bhightlight;
	D3DXCOLOR color_cast;
	FLOAT  emssive_power;
	FLOAT  spe_exp;
};

struct KG3DShaderParamShadowMap 
{
	INT	bShadowMask;
	D3DXVECTOR2 vBiasUV;
};

struct KG3DShaderParamDetail
{
	D3DXVECTOR4 vDetailScale;
	D3DXVECTOR4 vDetailMtl;

	D3DCOLORVALUE vDetail0Color;
	D3DCOLORVALUE vDetail1Color;
	D3DCOLORVALUE vBodyColor;
	D3DCOLORVALUE vBodyColor1;

	D3DXVECTOR4 vBodyScales;
	D3DXVECTOR4 vEnvSpecular;
};

//</KG3DShaderParam>

struct KG3DDefaultShaderGroupElem
{
	KG3DShaderHolder	m_ShaderBottles[EM_SHADER_MODEL_SUPPORTED_COUNT];
};
struct KG3DDefaultShaderGroup 
{
	virtual KG3DShaderHolder& GetDefaultShader(DWORD dwType) = 0;
	virtual LPD3DXEFFECT	GetDefaultEffect(DWORD dwType) = 0;
	//virtual LPD3DXEFFECT	GetDefaultEffectAndHandles(DWORD dwType, LPCTSTR handleStrings[], size_t uStringCount, LPVOID* ppStruct, size_t uStructSize) = 0;
	virtual HRESULT ReloadShaders() = 0;
	
	virtual HRESULT	Init(KG3DDefaultShaderRegistrationArg* pArgs, size_t Count) = 0;//����Ҫ��һ����Χ��
	virtual HRESULT ReplaceDefaultShader(DWORD dwType, const KG3DDefaultShaderGroupElem& newShader, KG3DDefaultShaderGroupElem& oldShader) = 0;	//�滻Shader,��Ҫ����ɵģ���Ȼ��һ����ܻᱻ�Զ�������
	virtual HRESULT ModifyGlobalMacro(D3DXMACRO_ARRAY macroModificationArray) = 0;//������ҪNULL,NULL��β��ע�����ʱ�������ܻ�����һ����Shader�ر��롣���û�б���ʼ����Shader����ֻ�޸Ĵ����������ȴ���ʼ����

	virtual HRESULT CheckShaderModel() = 0;
	virtual ULONG GetShaderModelToUse() = 0;

	virtual HRESULT LoadAllShaders() = 0;

	virtual ~KG3DDefaultShaderGroup() = 0{}
};

extern KG3DDefaultShaderGroup& g_GetDefaultShaderGroup();

#endif //_INCLUDE_KG3DDEFAULTSHADERGROUP_H_
