////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DTransformTool.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
// 
//  Create Date : 2008-10-16 11:32:31
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DSCENEOBJECTTRANSFORMTOOL_H_
#define _INCLUDE_KG3DSCENEOBJECTTRANSFORMTOOL_H_
#include "KG3DCommonObject.h"
////////////////////////////////////////////////////////////////////////////////
typedef enum _KG3DCOORD KG3DCOORD;
class IKG3DMessageReceiverBase;
class KG3DMessageSender;
struct KG3DTransformTool : public KG3DCommonObjectSimple
{
	STDMETHOD_(ULONG, GetType)() = 0;
	STDMETHOD(Render)() = 0;
	STDMETHOD(FrameMove)() = 0;

	virtual HRESULT	SetCoordMode(const KG3DCOORD& coordMode) = 0;//�趨��Local����World����ɶ������ϵ

	virtual HRESULT TransformDirect(const D3DXVECTOR3& vTransform) = 0;	//ֱ�ӱ任�ĺ��������������ƶ���˵����xyz��������ת��xyz����ת�����ڷ������ֱ�������ķ���
	virtual HRESULT TransformDirectCoord(const D3DXMATRIX& matTransform) = 0;	//TransformDirect����������������D3DXQUATERNION����D3DXMatrixRotationQuaternion���D3DXMATRIX�ٴ�����

	virtual HRESULT RegisterEventListener(DWORD messageID, IKG3DMessageReceiverBase& receiver) = 0;

#if defined(DEBUG) | defined(_DEBUG)
	virtual KG3DMessageSender* TestBegin() = 0;
	virtual VOID TestEnd() = 0;
#endif

	virtual ~KG3DTransformTool() = 0{}

	static DWORD GetTypeCorrespondingMessage(DWORD dwToolType);
};

struct KG3DTransformToolDummy : public KG3DTransformTool
{
	STDMETHOD_(ULONG, GetType)(){return KG3DTYPE_NONE;}
	STDMETHOD(Render)(){return E_NOTIMPL;}
	STDMETHOD(FrameMove)(){return E_NOTIMPL;}

	virtual HRESULT	SetCoordMode(const KG3DCOORD& coordMode){return E_NOTIMPL;}//�趨��Local����World����ɶ������ϵ

	virtual HRESULT TransformDirect(const D3DXVECTOR3& vTransform){return E_NOTIMPL;}
	virtual HRESULT TransformDirectCoord(const D3DXMATRIX& matTransform){return E_NOTIMPL;}

	virtual HRESULT RegisterEventListener(DWORD messageID, IKG3DMessageReceiverBase& receiver){return E_NOTIMPL;}

#if defined(DEBUG) | defined(_DEBUG)
	virtual KG3DMessageSender* TestBegin(){return NULL;}
	virtual VOID TestEnd(){return;}
#endif
};

KG3DTransformTool& GetStaticDummyTransformTool();

KG3DCommonObject* BuildKG3DRotationTool(LPCTSTR, DWORD_PTR pScene, DWORD_PTR);
KG3DCommonObject* BuildKG3DTranslationTool(LPCTSTR, DWORD_PTR pScene, DWORD_PTR);
KG3DCommonObject* BuildKG3DScalingTool(LPCTSTR, DWORD_PTR pScene, DWORD_PTR);

#endif //_INCLUDE_KG3DSCENEOBJECTTRANSFORMTOOL_H_
