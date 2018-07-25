////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DRepresentObjectTrans.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-10-27 11:19:45
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG3DRepresentObjectTrans.h"

struct KG3DRepresentObjectTransformVisitorSolid : public KG3DRepresentObjectTransformVisitor 
{
	D3DXMATRIX m_matTrans;
	KG3DRepresentObjectTransformVisitorSolid()
	{
		D3DXMatrixIdentity(&m_matTrans);
	}
	virtual HRESULT Accept(KG3DRepresentObject& Obj) = 0;
	virtual	HRESULT SetMatrix(const D3DXMATRIX& matTrans)
	{
		m_matTrans = matTrans;
		return S_OK;
	}
};

struct KG3DRepresentObjectRotationVisitorSolid : public KG3DRepresentObjectTransformVisitorSolid
{
	virtual HRESULT Accept(KG3DRepresentObject& Obj)
	{
		const D3DXMATRIX& LocalMatrix = Obj.GetMatrix();
		D3DXMATRIX LocalMatScaleRotation = LocalMatrix;
		
		/*
		ֻҪ��Local����Ϳ����ˣ�World�������ÿ֡���µġ�Parent�Ĳ��������Լ��˾ͺ���
		��Local����ֻ��Ҫ��ȡ��Translation�Ĳ��֣�Ȼ����ת��Ȼ���ٰ�Translation�Ĳ��ַŻ�ȥ���ο�
		��3D��ѧ����:ͼ������Ϸ��������P124

		�൱�� LocalMat * (T^-1) * R * (T)
		*/

		//���ƶ��Ĳ���ȥ��������¼����
		D3DXVECTOR3& vecTransPart = D3DXMatrixGetTranslationPart(LocalMatScaleRotation);
		D3DXVECTOR3 vecTrans = vecTransPart;
		vecTransPart = D3DXVECTOR3(0,0,0);

		/*����LocalMatScaleRotationֻʣ��ԭ����Scale��Rotation�Ĳ�����
		�൱��LoalMatScaleRotation * (T^-1)*/

		LocalMatScaleRotation *= m_matTrans;	//��ת
		vecTransPart = vecTrans;				//Ȼ���ٴΰѱ任�Ĳ��ָ��ǻ�ȥ�������ӱ������������

		Obj.SetMatrix(LocalMatScaleRotation);

		Obj.m_bSleep = FALSE;
		Obj.FrameMove();//<SLEEP_REFRACTOR>

		return S_OK;
	}
};

KG3DRepresentObjectTransformVisitor& g_GetRepresentObjectRotationVisitor()
{
	static KG3DRepresentObjectRotationVisitorSolid stInstance;
	return stInstance;
}
////////////////////////////////////////////////////////////////////////////////

struct KG3DRepresentObjectTranslationVisitorSolid : public KG3DRepresentObjectTransformVisitorSolid
{
	virtual HRESULT Accept(KG3DRepresentObject& Obj)
	{
		const D3DXMATRIX& LocalMatrix = Obj.GetMatrix();
		D3DXMATRIX LocalMatScaleRotation = LocalMatrix * m_matTrans;		

		Obj.SetMatrix(LocalMatScaleRotation);

		Obj.m_bSleep = FALSE;
		Obj.FrameMove();//<SLEEP_REFRACTOR>

		return S_OK;
	}
};

KG3DRepresentObjectTransformVisitor& g_GetRepresentObjectTranslationVisitor()
{
	static 	KG3DRepresentObjectTranslationVisitorSolid stInstance;
	return stInstance;
}
//////////////////////////////////////////////////////////////////////////
struct KG3DRepresentObjectScalingVisitorSolid : public KG3DRepresentObjectTransformVisitorSolid
{
	virtual HRESULT Accept(KG3DRepresentObject& Obj)
	{
		const D3DXMATRIX& LocalMatrix = Obj.GetMatrix();
		D3DXMATRIX LocalMatScaleRotation = LocalMatrix;

		//���ƶ��Ĳ���ȥ��������¼����
		D3DXVECTOR3& vecTransPart = D3DXMatrixGetTranslationPart(LocalMatScaleRotation);
		D3DXVECTOR3 vecTrans = vecTransPart;
		vecTransPart = D3DXVECTOR3(0,0,0);

		LocalMatScaleRotation = m_matTrans * LocalMatScaleRotation;	//��ת
		D3DXMatrixGetTranslationPart(LocalMatScaleRotation) = vecTrans;		//Ȼ���ٴΰѱ任�Ĳ��ָ��ǻ�ȥ�������ӱ������������

		Obj.SetMatrix(LocalMatScaleRotation);

		Obj.m_bSleep = FALSE;
		Obj.FrameMove();//<SLEEP_REFRACTOR>
		return S_OK;
	}
};

KG3DRepresentObjectTransformVisitor& g_GetRepresentObjectScalingVisitor()
{
	static 	KG3DRepresentObjectScalingVisitorSolid stInstance;
	return stInstance;
}

struct KG3DRepresentObjectTransVisitorSolid : public KG3DRepresentObjectTransformVisitorSolid
{
	virtual HRESULT Accept(KG3DRepresentObject& Obj)
	{
		const D3DXMATRIX& matLocal = Obj.GetMatrix();
		
		D3DXMATRIX matTrans = matLocal * m_matTrans;

		Obj.SetMatrix(matTrans);

		Obj.m_bSleep = FALSE;
		Obj.FrameMove();//<SLEEP_REFRACTOR>

		return S_OK;
	}
};

KG3DRepresentObjectTransformVisitor& g_GetRepresentObjectTransVisitor()
{
	static KG3DRepresentObjectTransVisitorSolid s_Instance;
	return s_Instance;
}