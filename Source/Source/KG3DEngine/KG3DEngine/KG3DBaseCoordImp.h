////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DBaseCoordImp.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-11-27 14:19:56
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DBASECOORDIMP_H_
#define _INCLUDE_KG3DBASECOORDIMP_H_

////////////////////////////////////////////////////////////////////////////////
#include "KG3DBaseCoord.h"
#include "KG3DMessenger.h"
#include "KG3DMessageBroadcasterGroup.h"
#include "KGStateMachine.h"
#include "IEEditorTypes.h"


class KG3DBaseCoordImp : public KG3DBaseCoord, public KG3DMessageReceiver
{
public:
	STDMETHOD_(ULONG, GetType)() = 0;
	STDMETHOD(Init)();	//����Ҫת���������
	virtual	ULONG GetActivationState() = 0;	//ʲô״̬�¼���
	virtual D3DXMATRIX GetTrans() = 0;
	STDMETHOD(Render)() = 0;

	STDMETHOD(FrameMove)();
	

	virtual ULONG	GetCurStateID();
	virtual HRESULT SetPosition(const D3DXVECTOR3& Pos);
	virtual HRESULT	Zoom(FLOAT zoomFactor);//zoomFactor��ӵ�Scaleϵ�����棬�����ǳ�

	virtual void __stdcall OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender );

#if defined(DEBUG) | defined(_DEBUG)
	virtual KG3DMessageReceiverBase* TestGetReceiverInterface(){return this;}
#endif

	virtual ~KG3DBaseCoordImp();
protected:
	KG3DBaseCoordImp(KG3DMessageBroadcasterGroup& Sc);

	
	STDMETHOD(UnInit)();

	struct RepresentData;
	struct ResultData;
	struct InputData;

	static FLOAT GetSelectorRingRadius(const RepresentData& data);

	static VOID RenderIntersectMesh(RepresentData& RpData, const D3DXMATRIX& matView, const D3DXMATRIX& matProj);

	static BOOL IntersectMeshAndFindPoint(const RepresentData& RpData, const InputData& IpData, D3DXVECTOR3& vInter, KG3DCOORD& IntersectedCoord);

	static BOOL IntersectPlaneAndFindPoint(KG3DCOORD WhichCoord, const D3DXVECTOR3& Center, const D3DXVECTOR3& vSrc
		, const D3DXVECTOR3& vDir, D3DXVECTOR3& vInter);

	static D3DXVECTOR3 RefineInterPoint(const RepresentData& rpData, const D3DXVECTOR3& vInter, const ResultData& retData);

protected:
	DWORD       m_dwMode;			//�������껹�Ǿֲ�����

	struct RepresentData			//��Դȫ��Manager�͵Ĳ���Ҫ���豸��ʧ
	{
		//ȫ��Managed�͵�Mesh
		LPD3DXMESH MeshForAxis[3];
		LPD3DXMESH MeshForPlane[3];
		LPD3DXMESH MeshForIntegration;	//һ�·���֮���õ���

		FLOAT	zoomFactor;//������������ȦȦ�ı���
		D3DXVECTOR3	vecPos;	
		D3DXVECTOR3 CamPos;
		
		INT			bFilterInter;

		RepresentData();
		~RepresentData();
		VOID ReleaseMeshes();
	};

	RepresentData m_RepresentData;

	struct InputData
	{
		BOOL		IsActivated;
		D3DXVECTOR3 vMouseRaySrc;
		D3DXVECTOR3 vMouseRayNormalizedDir;
		InputData(){ZeroMemory(this,sizeof(*this));}
	};

	InputData m_InputData;

	struct ResultData 
	{
		KG3DCOORD	IntersectedCoord;
		D3DXVECTOR3	StartDir;
		D3DXVECTOR3 LastDir;
		FLOAT		Angle;

		D3DXVECTOR3 StartPos;
		D3DXVECTOR3 EndPos;

		ResultData()
			:StartDir(1,0,0)
			,LastDir(0,0,1)
			,IntersectedCoord(KG3DCOORD_INVALID)
			,Angle(0)
		{

		}
	};

	ResultData m_ResultData;

	enum
	{
		em_mesh_inner_radius = 10,
		em_mesh_range = 200,
		em_mesh_plane_range = 60,	//��Լ��em_mesh_range��1/3
		em_mesh_sides = 4,
		em_mesh_rings = 16,

		em_selplane_inner_edge = 100,
		em_selplane_outter_edge = 125,

		em_state_check_gap = 40,//����

		em_filter_none = 0,	//������ײ����
		em_filter_front = 1,
		em_filter_back = 2,
	};

	KG3DMessage m_messageCur;

	//״̬������,ԭ���״̬��ͼ��ȫһ��,����һ������,�ı䵱ǰ״̬���߱���״̬
protected:
	struct CustomStateMachineState : public KGStateMachineState
	{
		virtual ULONG STDMETHODCALLTYPE Release(){return 1;}//��̬�࣬����ҪRelease
		virtual KGStateMachineState* Clone(DWORD_PTR){return this;}
		virtual DWORD CheckInput(KG3DBaseCoordImp& instance) = 0;
		virtual DWORD CheckInput(DWORD_PTR Input)
		{
			_ASSERTE(NULL != Input);
			return CheckInput(*reinterpret_cast<KG3DBaseCoordImp*>(Input));
		}
	};

	KGStateMachine*	m_pStateMachine;

	struct StateNotActivated : public CustomStateMachineState
	{
		virtual DWORD	GetStateID();
		virtual DWORD	CheckInput(KG3DBaseCoordImp& instance);
	};

	struct StateActivated : public CustomStateMachineState
	{
		virtual DWORD	GetStateID();
		virtual DWORD	CheckInput(KG3DBaseCoordImp& instance);
	};

	struct StateTransforming : public CustomStateMachineState
	{
		virtual DWORD	GetStateID();
		virtual DWORD	CheckInput(KG3DBaseCoordImp& instance);
	};

	static KGStateMachine::BuildStruct GetStateMachineStateDesc();
	static const D3DXPLANE& GetPlaneIntergration();//�õ�����ͳһ�������Ǹ�NormalΪ1,1,1��ƽ��
	
};


#endif //_INCLUDE_KG3DBASECOORDIMP_H_
