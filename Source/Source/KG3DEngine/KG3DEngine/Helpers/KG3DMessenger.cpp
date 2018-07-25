////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DMessenger.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-3-11 15:42:02
//  Comment     : 
/*
����һ����һ���ṹ�����࣬����KG3DMessage����Ϣ�����壬Ȼ����Դ����Դ��ӵ���߷ֱ�
����IKG3DMessageSender��IKG3DMessageReceiver�����ӿ��Բο�KG3DTexture��KG3DMeshFarMountain֮��Ľ�����ʽ��
FarMountain��Textureע�����Լ�������OnReceiveMessage��Ȼ��Ϳ�������ͼ�Զ�֪ͨ��ͼ��Reload�¼��ˡ�
ע��������ļ��в�Ҫ�Ѷ���ǿת��Messenger�ĸ����࣬��ΪMessenger��Ҫ�����������õģ����ܱ�֤�ǵ�һ������
*/

//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "KG3DMessenger.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/************************************************************************/
// KG3DMessageSingleReceiverSender���忪ʼ
/************************************************************************/
KG3DMessageSingleReceiverSender::KG3DMessageSingleReceiverSender()
:m_pReceiver(NULL)
{

}
KG3DMessageSingleReceiverSender::~KG3DMessageSingleReceiverSender()
{
	if(NULL ==  m_pReceiver)
		return;
	PostMessage( KG3DMessage( KM_MSG_SENDER_DESTRUCTED, NULL, NULL ) );
	m_pReceiver = NULL;
}
void KG3DMessageSingleReceiverSender::PostMessage( const KG3DMessage& Message )
{
	if (NULL != m_pReceiver)
	{
		KG3DMessageSenderBase::LetReceiverPickMessage(*m_pReceiver, Message);
	}
}
bool __stdcall KG3DMessageSingleReceiverSender::RegisterReceiver( IKG3DMessageReceiverBase* pReceiver )
{
	KG_PROCESS_ERROR(NULL == m_pReceiver && NULL != pReceiver);
	{
		KG3DMessageReceiverBase* pReceiverBase = dynamic_cast<KG3DMessageReceiverBase*>(pReceiver);
		_ASSERTE(NULL != pReceiverBase);
		bool bRet = KG3DMessageSenderBase::LetReceiverPickMessage(*pReceiverBase
			, KG3DMessage(KM_MSG_SENDER_REGISTERED));///�����鷵��ֵ
		KG_PROCESS_ERROR(bRet);
		m_pReceiver = pReceiverBase;
		return true;
	}
Exit0:
	return false;
}

bool __stdcall KG3DMessageSingleReceiverSender::UnRegisterReceiver( IKG3DMessageReceiverBase* pReceiver )
{
	KG_PROCESS_ERROR(NULL != pReceiver && pReceiver == m_pReceiver);///ע�����а�����m_pReceiver != NULL
	{
		bool bRet = KG3DMessageSenderBase::LetReceiverPickMessage(*m_pReceiver
			, KG3DMessage(KM_MSG_SENDER_UNREGISTERED));
		KG_PROCESS_ERROR(bRet);
		m_pReceiver = NULL;
		return true;
	}
Exit0:
	return false;
}
/************************************************************************/
// KG3DMessageSingleSenderReceiver
/************************************************************************/
KG3DMessageSingleSenderReceiver::KG3DMessageSingleSenderReceiver()
:m_pSender(NULL)
{

}
KG3DMessageSingleSenderReceiver::~KG3DMessageSingleSenderReceiver()
{
	if (NULL != m_pSender)
	{
#if defined(DEBUG) | defined(_DEBUG)
		bool bRet = 
#endif
			m_pSender->UnRegisterReceiver(this);
		_ASSERTE(bRet && m_pSender == NULL);
	}
}

bool KG3DMessageSingleSenderReceiver::PickMessage( KG3DMessage Message , KG3DMessageSenderBase* pSender )
{
	switch(Message.m_uMessage)
	{
	case KM_MSG_SENDER_DESTRUCTED:
	case KM_MSG_SENDER_UNREGISTERED:
		{
			KG_PROCESS_ERROR(pSender == m_pSender);
			m_pSender = NULL;
		}
		break;
	case KM_MSG_SENDER_REGISTERED:
		{
			KG_PROCESS_ERROR(pSender != NULL && m_pSender == NULL);
			m_pSender = pSender;
		}
		break;
	default:
		break;
	}
	OnReceiveMessage( Message, pSender );
	return true;
Exit0:
	return false;
}
void __stdcall KG3DMessageSingleSenderReceiver::OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender )
{
	NULL;
}
/************************************************************************/
//Sender���忪ʼ                                                                   
/************************************************************************/
KG3DMessageSender::KG3DMessageSender()
{
	NULL;
}

KG3DMessageSender::~KG3DMessageSender()
{
	if ( m_ListReceiver.empty() )
		return;
	PostMessage( KG3DMessage( KM_MSG_SENDER_DESTRUCTED, 0, 0 ) );
	m_ListReceiver.clear();
}

bool __stdcall KG3DMessageSender::RegisterReceiver(  IKG3DMessageReceiverBase* pReceiver  )
{
	KG_PROCESS_ERROR(NULL != pReceiver);
	{
		bool bReceiverNotRegisted = m_ListReceiver.empty() 
			|| m_ListReceiver.end() == std::find( m_ListReceiver.begin(), m_ListReceiver.end(), pReceiver );
		KG_PROCESS_ERROR(bReceiverNotRegisted);	///�ظ�ע���ǲ������

		KG3DMessageReceiverBase* pReceiverBase = dynamic_cast<KG3DMessageReceiverBase*>(pReceiver);
		bool bRet = KG3DMessageSenderBase::LetReceiverPickMessage(*pReceiverBase
			, KG3DMessage(KM_MSG_SENDER_REGISTERED));
		KG_PROCESS_ERROR(bRet);
		m_ListReceiver.push_back( pReceiverBase );

		return true;
	}
Exit0:
	return false;
}

bool __stdcall KG3DMessageSender::UnRegisterReceiver(  IKG3DMessageReceiverBase* pReceiver  )
{
	KG_PROCESS_ERROR(pReceiver);
	{	
		bool bRet = Remove( pReceiver );///ע�⣬������������������Receiver����˼
		KG_PROCESS_ERROR(bRet);

		KG3DMessageReceiverBase* pReceiverBase = dynamic_cast<KG3DMessageReceiverBase*>(pReceiver);
		bRet = KG3DMessageSenderBase::LetReceiverPickMessage(*pReceiverBase
			, KG3DMessage(KM_MSG_SENDER_UNREGISTERED));
		_ASSERTE(bRet);

		return true;		
	}
Exit0:
	return false;
}
void KG3DMessageSender::PostMessage( const KG3DMessage& Message )
{
	KG_PROCESS_SUCCESS(m_ListReceiver.empty());
	{
		KG3DMessage tempMessage( Message );			///����һ����������ֵ����������߳�����
		for (size_t i = 0; i < m_ListReceiver.size(); ++i)
		{
			KG3DMessageSenderBase::LetReceiverPickMessage(*(m_ListReceiver[i]), tempMessage);
		}
	}
Exit1:
	return;
}

bool KG3DMessageSender::Remove( IKG3DMessageReceiverBase* pReceiver )
{
	_ASSERTE(pReceiver);
	RECEIVER_LIST::iterator it = find(m_ListReceiver.begin(), m_ListReceiver.end(), pReceiver);
	if (it != m_ListReceiver.end())
	{
		m_ListReceiver.erase(it);
		return true;
	}
	return false;
}

KG3DMessageReceiverBase* KG3DMessageSender::GetReceiver( DWORD_PTR i)
{
	size_t Index = _kg_to_smaller_size_cast<size_t>(i);
	if(Index < m_ListReceiver.size())
		return m_ListReceiver[Index];
	return NULL;
}
/************************************************************************/
//    KG3DMessageReceiver���忪ʼ                                                                  
/************************************************************************/
KG3DMessageReceiver::KG3DMessageReceiver()
{
	NULL;
}

KG3DMessageReceiver::~KG3DMessageReceiver()
{
	if(m_ListSender.empty())
		return;
	KG3DMessageSenderBase* pSender = NULL;
	for (SENDER_LIST::iterator it = m_ListSender.begin(); it != m_ListSender.end();)
	{
		pSender = *it;
		size_t OldSize = m_ListSender.size();
		pSender->UnRegisterReceiver( this );	//������Ϣ��ʱ���ı�List		
		size_t NewSize = m_ListSender.size();
		if (NewSize != OldSize)
		{
			it = m_ListSender.begin();
		}
		else
		{
			++it;	
		}
	}
	m_ListSender.clear();
}

bool KG3DMessageReceiver::PickMessage(  KG3DMessage Message , KG3DMessageSenderBase* pSender  )
{
	switch(Message.m_uMessage)
	{
	case KM_MSG_SENDER_DESTRUCTED:
	case KM_MSG_SENDER_UNREGISTERED:
		{
#if defined(DEBUG) | defined(_DEBUG)
			bool bRet = 
#endif

				Remove(pSender);
			_ASSERTE(bRet);
		}
		break;
	case KM_MSG_SENDER_REGISTERED:
		{
			bool bRet = m_ListSender.empty()
				|| m_ListSender.end() == std::find( m_ListSender.begin(), m_ListSender.end(), pSender );
			KG_PROCESS_ERROR(bRet);
			m_ListSender.push_back( pSender );///�ظ�ע���ǲ������
		}
		break;
	default:
		break;
	}
	OnReceiveMessage( Message, pSender );
	return true;
Exit0:
	return false;
}

bool KG3DMessageReceiver::Remove( IKG3DMessageSenderBase* pSender )
{
	SENDER_LIST::iterator it = find(m_ListSender.begin(), m_ListSender.end(), pSender);
	if (it != m_ListSender.end())
	{
		m_ListSender.erase(it);
		return true;
	}
	return false;
}

KG3DMessageSenderBase* KG3DMessageReceiver::GetSender( DWORD_PTR i )
{
	size_t Index = _kg_to_smaller_size_cast<size_t>(i);
	if (Index < m_ListSender.size())
		return m_ListSender[i];
	return NULL;
}


#if defined(KG_ENABLE_TDD)
KG_TEST_BEGIN(KG3DMessenger)
{
	class TestSingleReceiverSender : public KG3DMessageSingleReceiverSender
	{
	public:
		KG3DMessageReceiverBase* GetReceiver(DWORD_PTR i)
		{
			return KG3DMessageSingleReceiverSender::GetReceiver(i);
		}
		virtual void PostMessage( const KG3DMessage& Message )
		{
			KG3DMessageSingleReceiverSender::PostMessage(Message);
		}
	};

	class TestSingleSenderReceiver : public KG3DMessageSingleSenderReceiver
	{
	public:
		KG3DMessage m_Message;
		KG3DMessageSenderBase* GetSender(DWORD_PTR i)
		{
			return KG3DMessageSingleSenderReceiver::GetSender(i);
		}
		virtual void __stdcall OnReceiveMessage(const KG3DMessage& Message, IKG3DMessageSenderBase* pSender)
		{
			m_Message = Message;
		}
	};

	class TestSender : public KG3DMessageSender
	{
	public:
		using KG3DMessageSender::GetReceiver;
		using KG3DMessageSender::GetReceiverCount;
		using KG3DMessageSender::PostMessage;
	};

	class TestReceiver : public KG3DMessageReceiver
	{	
	public:
		KG3DMessage m_Message;
		using KG3DMessageReceiver::GetSender;
		virtual void __stdcall OnReceiveMessage(const KG3DMessage& Message, IKG3DMessageSenderBase* pSender)
		{
			m_Message = Message;
		}
	};
	//case1, ��Receiver����Sender
	{
		//����ע��
		{
			TestSingleSenderReceiver SSReceiver;
			TestSingleReceiverSender SRSender;



			//Register
			_ASSERTE(NULL == SSReceiver.GetSender(0));
			_ASSERTE(NULL == SRSender.GetReceiver(0));
			SRSender.RegisterReceiver(&SSReceiver);
			_ASSERTE(NULL != SSReceiver.GetSender(0));
			_ASSERTE(NULL != SRSender.GetReceiver(0));

			//Message
			SRSender.PostMessage(KG3DMessage(UINT_MAX));
			_ASSERTE(SSReceiver.m_Message == KG3DMessage(UINT_MAX));

			//UnRegister
			SRSender.UnRegisterReceiver(&SSReceiver);
			_ASSERTE(NULL == SSReceiver.GetSender(0));
			_ASSERTE(NULL == SRSender.GetReceiver(0));


		}
		//�����Զ�����1
		{
			TestSingleSenderReceiver* pSSReceiver = new TestSingleSenderReceiver;
			TestSingleReceiverSender* pSRSender = new TestSingleReceiverSender;

			pSRSender->RegisterReceiver(pSSReceiver);
			_ASSERTE(NULL != pSRSender->GetReceiver(0));


			SAFE_DELETE(pSSReceiver);
			_ASSERTE(NULL == pSRSender->GetReceiver(0));
			SAFE_DELETE(pSRSender);
		}
		//�����Զ�����2
		{
			TestSingleSenderReceiver* pSSReceiver = new TestSingleSenderReceiver;
			TestSingleReceiverSender* pSRSender = new TestSingleReceiverSender;

			pSRSender->RegisterReceiver(pSSReceiver);
			_ASSERTE(NULL != pSSReceiver->GetSender(0));

			SAFE_DELETE(pSRSender);
			_ASSERTE(NULL == pSSReceiver->GetSender(0));
			SAFE_DELETE(pSSReceiver);
		}
	}//case1, ��Receiver����Sender
	//case2, ��Receiver����Sender
	{
		//����ע��
		{
			TestSingleSenderReceiver SSReceiver;
			TestSingleReceiverSender SRSender;
			TestSender Sender;
			TestReceiver Receiver;

			//Register
			_ASSERTE(NULL == Receiver.GetSender(0));
			_ASSERTE(NULL == Sender.GetReceiver(0));
			Sender.RegisterReceiver(&SSReceiver);	//��Զ��ע��
			Sender.RegisterReceiver(&Receiver);
			SRSender.RegisterReceiver(&Receiver);
			_ASSERTE(NULL != Receiver.GetSender(0));
			_ASSERTE(NULL != Receiver.GetSender(1));
			_ASSERTE(NULL != Sender.GetReceiver(0));
			_ASSERTE(NULL != Sender.GetReceiver(1));
			//�ظ�ע��Ӧ��ʧ��
			_ASSERTE(Sender.GetReceiverCount() == 2);
			_ASSERTE(! Sender.RegisterReceiver(&SSReceiver));
			_ASSERTE(! Sender.RegisterReceiver(&Receiver));

			//Message
			Sender.PostMessage(KG3DMessage(UINT_MAX));
			_ASSERTE(SSReceiver.m_Message == KG3DMessage(UINT_MAX));
			_ASSERTE(Receiver.m_Message == KG3DMessage(UINT_MAX));
			SRSender.PostMessage(KG3DMessage(0x12345679));
			_ASSERTE(Receiver.m_Message == KG3DMessage(0x12345679));

			//UnRegister
			Sender.UnRegisterReceiver(&SSReceiver);
			Sender.UnRegisterReceiver(&Receiver);
			SRSender.UnRegisterReceiver(&Receiver);

			_ASSERTE(NULL == Receiver.GetSender(0));
			_ASSERTE(NULL == Sender.GetReceiver(0));				
		}
		//�����Զ�����1,Receiver������
		{
			TestSingleSenderReceiver* pSSReceiver = new TestSingleSenderReceiver;
			TestSingleReceiverSender* pSRSender = new TestSingleReceiverSender;
			TestSender* pSender =new TestSender;
			TestReceiver* pReceiver = new TestReceiver;

			pSender->RegisterReceiver(pSSReceiver);	//��Զ��ע��
			pSender->RegisterReceiver(pReceiver);
			pSRSender->RegisterReceiver(pReceiver);

			_ASSERTE(NULL != pSRSender->GetReceiver(0));
			_ASSERTE(NULL != pSender->GetReceiver(1));
			_ASSERTE(NULL != pSender->GetReceiver(0));
			_ASSERTE(NULL != pReceiver->GetSender(0));
			_ASSERTE(NULL != pReceiver->GetSender(1));


			SAFE_DELETE(pReceiver);
			_ASSERTE(NULL == pSRSender->GetReceiver(0));
			_ASSERTE(NULL == pSender->GetReceiver(1));
			_ASSERTE(NULL != pSender->GetReceiver(0));
			_ASSERTE(NULL != pSSReceiver->GetSender(0));

			SAFE_DELETE(pSRSender);
			SAFE_DELETE(pSender);
			SAFE_DELETE(pSSReceiver);
		}
		//�����Զ�����2,Sender������
		{
			TestSingleSenderReceiver* pSSReceiver = new TestSingleSenderReceiver;
			TestSingleReceiverSender* pSRSender = new TestSingleReceiverSender;
			TestSender* pSender =new TestSender;
			TestReceiver* pReceiver = new TestReceiver;

			pSender->RegisterReceiver(pSSReceiver);	//��Զ��ע��
			pSender->RegisterReceiver(pReceiver);
			pSRSender->RegisterReceiver(pReceiver);


			SAFE_DELETE(pSender);
			_ASSERTE(NULL == pReceiver->GetSender(1));
			_ASSERTE(NULL != pReceiver->GetSender(0));
			_ASSERTE(NULL != pSRSender->GetReceiver(0));
			_ASSERTE(NULL == pSSReceiver->GetSender(0));

			SAFE_DELETE(pSRSender);
			SAFE_DELETE(pSSReceiver);
			SAFE_DELETE(pReceiver);
		}
	}//case2, ��Receiver����Sender
	//_KG3D_DEBUG_REPORT("KG3DMessager Runtime Test Passed\n");
}//Tester()
KG_TEST_END(KG3DMessenger)
#endif