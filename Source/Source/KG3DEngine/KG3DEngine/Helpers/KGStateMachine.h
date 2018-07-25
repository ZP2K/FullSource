////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGStateMachine.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-10-20 10:50:29
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGSTATEMACHINE_H_
#define _INCLUDE_KGSTATEMACHINE_H_

#include "KG3DCommonObject.h"
////////////////////////////////////////////////////////////////////////////////
enum
{
	KG_STATE_MACHINE_SELF = 0,	//��ʶ��״̬ʱ�����Լ�״̬�����Ᵽ���֣��κ�״̬������SELF��ͷ
};
interface KGStateMachineState;

struct KGStateMachineState : public KG3DCommonObjectSimple
{
	STDMETHOD_(ULONG, GetType)();
	virtual DWORD	GetStateID() = 0;
	virtual KGStateMachineState* Clone(DWORD_PTR param) = 0;	//�Լ�ʵ���ڴ���������static�͵�Singleton������this�ͺ��ˡ��ͷű���Release
	virtual DWORD	CheckInput(DWORD_PTR Input) = 0;//����1�������Mealy�ͣ��Լ�ת��������ṹ�������Moore�ͣ����Ե��ͺ��ˡ���������һ��״̬��ID
};

class KGStateMachine : public KG3DCommonObjectSimple
{
public:
	enum
	{
		em_max_state_count = 64,
	};

	struct BuildStruct 
	{
		KGStateMachineState**	pStates;
		size_t					uStateCount;
		DWORD					dwInitStateID;
	};

	static KGStateMachine* Build(LPCTSTR, DWORD_PTR pBuildStruct, DWORD_PTR);
	
	STDMETHOD_(ULONG, GetType)();
	STDMETHOD(Init)(KGStateMachineState** states, size_t uStateCount, DWORD InitID);//ID������������ʼState������State������һ��
	VOID CheckInput(DWORD_PTR Input);
	KGStateMachineState&	GetCurState();
private:
	KGStateMachine();
	STDMETHOD(UnInit)();
	std::vector<KGStateMachineState*>	m_States;
	size_t						m_uStateCount;
	DWORD						m_dwFirstID;
	DWORD						m_dwCurStateID;
};

#endif //_INCLUDE_KGSTATEMACHINE_H_
