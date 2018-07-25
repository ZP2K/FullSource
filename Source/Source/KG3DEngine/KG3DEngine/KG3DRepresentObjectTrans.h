////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DRepresentObjectTrans.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-10-27 11:19:50
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DREPRESENTOBJECTTRANS_H_
#define _INCLUDE_KG3DREPRESENTOBJECTTRANS_H_

#include "KG3DRepresentObject.h"
////////////////////////////////////////////////////////////////////////////////

struct KG3DRepresentObjectTransformVisitor : public KG3DRepresentObjectVisitor 
{
	virtual HRESULT Accept(KG3DRepresentObject& Obj) = 0;//��ʱֻ���ڱ༭���Ĺ��ߣ�Ч�����Ż��Ŀռ䣬��Ҫ������ͨ��֡��������
	virtual	HRESULT SetMatrix(const D3DXMATRIX& matTrans) = 0;
};

KG3DRepresentObjectTransformVisitor& g_GetRepresentObjectTransVisitor();

KG3DRepresentObjectTransformVisitor& g_GetRepresentObjectRotationVisitor();
KG3DRepresentObjectTransformVisitor& g_GetRepresentObjectTranslationVisitor();
KG3DRepresentObjectTransformVisitor& g_GetRepresentObjectScalingVisitor();

#endif //_INCLUDE_KG3DREPRESENTOBJECTTRANS_H_
