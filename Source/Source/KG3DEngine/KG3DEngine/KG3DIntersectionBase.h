////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DIntersectionBase.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-3-9 10:48:46
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DINTERSECTIONBASE_H_
#define _INCLUDE_KG3DINTERSECTIONBASE_H_

////////////////////////////////////////////////////////////////////////////////
class KG3DRepresentObject;
class KG3DModel;
class KG3DModelST;
class KG3DMesh;
class KG3DSceneEntity;
/*
RayIntersection����������һ��Filter��������ֵ�ų�ĳЩ��������ײ
��Filter������KG3DIntersectionFilter������
��Accept������һ�������������ص���E_FAILED��������ĳ������
��ô�Ǹ������RayIntersection����FALSE��
�磬����������ModelST��ײ����ô������

struct CameraInterFil : public KG3DIntersectionFilter
{
virtual HRESULT Accept(KG3DModelST& ModelST){return E_FAILED;}
}

�������Ǹ������Ǹ������з���E_FAILED�Ϳ����ˣ�Ҫ����ϸ�ļ��Ļ�
�����ں�����д������ϸ�ļ�鷽�������һ��Mesh�������پ����Ƿ񷵻�E_FAILED��

RayIntersection�ᱣ֤
����飬����ֻ��Ҫ�����ĵط�дAccept�ͺ��ˡ�
��Mesh������ų���������Ե�
����Mesh���������ײ��ʱ��Ҫ�ų�����ô����Ҫ����Model�ǲ㣬Ȼ���Model�ж�Mesh
����ֱ�Ӹ���Mesh�ǲ�ͺ��ˡ�

��Filter���д��۵ģ����FilterΪNULL��ô���ۼ���Ϊ0������������飬��Ȼ
RayIntersection���ڲ�汾���ܱ����ã�Ȼ��û�б�Filter����������û�б�Ҫ��
�𴫸�Filter������ȥ��
*/

struct KG3DIntersectionFilter
{
	virtual HRESULT Accept(KG3DRepresentObject& Object){return S_OK;}
	virtual HRESULT Accept(KG3DSceneEntity& Entity){return S_OK;}
	virtual HRESULT Accept(KG3DModel& Model){return S_OK;}
	virtual HRESULT Accept(KG3DModelST& ModelST){return S_OK;}
	virtual HRESULT Accept(KG3DMesh& Mesh){return S_OK;}
};

#endif //_INCLUDE_KG3DINTERSECTIONBASE_H_
