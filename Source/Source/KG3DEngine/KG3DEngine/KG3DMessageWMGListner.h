////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DMessageWMGListner.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-1 11:05:29
/*  Comment     : 
KG3DMessage��Sender��Receiverϵ��ʵ����Observerģʽ���������������ڲ���صġ�
˫����������ʱ�򶼻��໥֪ͨ

ʹ�÷�ʽ�ܼ򵥣�

1.���ͷ���Sender��һ������������ӵ��һ��Sender��ʵ��
2.���շ�����OnReceiveMessage
3.����һ��Sender��RegisterReceiver(pReceiver);
4.Sender������PostMessage��ʱ�����е�ע����Ľ��շ������Խ��ܵ���Ϣ
5.���ùܶԷ��Ƿ���ڣ���������Ѿ��Զ��������������ڵ������ˡ�

KG3DMessageMSGBroadcaster��KG3DMessageMSGListner������ģʽ���������ڲ�����Windows��Ϣ
���ࡣֻ�Ǽ򵥵�������Sender��Receiver������֮���Ƿ��ͺͽ��ܵ���Ϣ��KG3DWindowsMessage��
����KG3DMessage��

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DMESSAGEWMGLISTNER_H_
#define _INCLUDE_KG3DMESSAGEWMGLISTNER_H_
#include "KG3DMessenger.h"
/************************************************************************/
/*
��һ������������Ϣ
*/
/************************************************************************/
class KG3DMessageMSGBroadcaster : public KG3DMessageSender
{
public:
	virtual void PostMessage( const KG3DWindowsMessage& Message );

	virtual bool __stdcall RegisterReceiver( IKG3DMessageReceiverBase* pReceiver );
};
/************************************************************************/
/*
���ǵ�һ�������Ϳ��Խ�����Ϣ��
*/
/************************************************************************/
class KG3DMessageMSGListner : public KG3DMessageReceiver
{
public:
	virtual void __stdcall OnReceiveMessage(const KG3DWindowsMessage& Message, IKG3DMessageSenderBase* pSender){NULL;}


public:
	virtual void __stdcall OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender );

};

#endif //_INCLUDE_KG3DMESSAGEWMGLISTNER_H_
