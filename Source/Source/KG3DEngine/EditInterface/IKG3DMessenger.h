////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DMessenger.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2006-11-12 10:53:25
//  Comment     : 
/*
�����ڸ���ģ��֮�䴫����Ϣ�������ڸ���ģ��֮�佻����Ϣ��һ�ֻ��ơ�
��Ҫ���ڽ��һ����Դ����Դ��ӵ���ߴ�����Ϣ�����⡣һ����Դ��Ӧ��֪������ӵ������ʲô
������ӵ���߿���Ҫ����Դ������������״̬���̳����ǵ�����Ҫд��OnReceiveMessage֮�ࡣ

һ������Observerģʽ�Ļ��ơ�
Sender�Ǳ��۲��ߣ�������Receiver����Ϣ
Receiver�ǹ۲��ߣ�������Ϣ
MessageSender��MessageReceiver�ֱ���Ϊ
��Ϣ�ķ����ߺͽ����ߵĻ��ࡣ��Sender��RegisterReceiverע����Receiver
��ָ��֮��Sender���Ե���PostMessage��������Ϣ��Receiver������PickMessage��
PickMessageԤ������һЩע�ᣬ��ע��֮�����Ϣ֮��֪ͨOnReceiveMessage
Receiver��������ͨ��дOnReceiveMessage��������Ϣ��

Ҫע�����Sender��Receiver�ڲ������˶Է��������¼������������޹أ�
�κ�һ����������֪ͨ����һ��������á����Ը���OnReceiveMessage��ʱ�򣬿��Է���ʹ��
Sender��ָ��

�÷���

1.��Ϣ�����ߣ�����˵�Ǳ��۲��ߣ���KG3DMessageSingleReceiverSender����KG3DMessageSender������
ʲôҲ���ø���
2.дһ����Ϣ�Ĵ��ţ�������KM_��ö��ֵ���벻Ҫ�����Ѿ����ڵ�ö�١�
3.��Ϣ�����ߣ�����˵�ǹ۲��ߣ���KG3DMessageSingleSenderReceiver����KG3DMessageReceiver������
����OnReceiveMessage��������Ϣ�����ܵ�֮�����ж�����Ϣ�����ͺ���δ���
4.���ʵ���ʱ����۲��߻��߱��۲���Init��ʱ�򣬵���Sender.RegisterReceiver(&Receiver);
�ý�����ע�ᵽ�����ߡ���ע����������ʱ����Զ����У�Ҳ�����ֶ���ע�ᡣ˫��������ʱ�򶼻����
�ڶԷ��������ã����ᷢ����Чָ�롣
5.������Ҫ������Ϣ��ʱ�򣬵��÷����ߵ�PostMessage�Ϳ�����
6.��GetReceiver��GetSender���Եõ���Ӧ�Ľ����ߺͷ����ߣ��������ֻ�����Ա�ڲ���������
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IKG3DMESSENGER_H_
#define _INCLUDE_IKG3DMESSENGER_H_

////////////////////////////////////////////////////////////////////////////////
struct KG3DMessageActionExecutionStruct 
{
	INT	nActionID;	//�ο�EXEACTION_XXX
	INT nExecState;//�������������ڰ��£����ǵ��𣬲ο�InputProxy
	WPARAM wParam;
	LPARAM lParam;	//�����WM_KEYFIRST��WM_KEYLAST֮����Message����WM_MOUSEMOVE,����nExecState��EXEACTION_LEFE_KEY_DOWN int xPos = LOWORD(lParam); int yPos = HIWORD(lParam);		
	KG3DMessageActionExecutionStruct(){ZeroMemory(this, sizeof(KG3DMessageActionExecutionStruct));}
	KG3DMessageActionExecutionStruct(INT nActionID, INT State, WPARAM wP, LPARAM lP):nActionID(nActionID), nExecState(State),wParam(wP), lParam(lP){}
	KG3DMessageActionExecutionStruct(const KG3DMessageActionExecutionStruct& Other)	{memcpy_s(this, sizeof(KG3DMessageActionExecutionStruct), &Other, sizeof(KG3DMessageActionExecutionStruct));}
	KG3DMessageActionExecutionStruct& operator=(const KG3DMessageActionExecutionStruct& Other)	{memcpy_s(this, sizeof(KG3DMessageActionExecutionStruct), &Other, sizeof(KG3DMessageActionExecutionStruct)); return *this;}
};

struct KG3DMessageSceneLoadingPercent 
{
	FLOAT	TerrainLoadingPercent;
	FLOAT	ObjectLoadingPercent;
	FLOAT	RestLoadingPercent;
	KG3DMessageSceneLoadingPercent(){ZeroMemory(this, sizeof(KG3DMessageSceneLoadingPercent));}
};

//Message������
typedef enum _KM_MESSEGE
{
	KM_NONE = 0,

	KM_MSG_SENDER_CONSTRUCTED = 10,
	KM_MSG_SENDER_DESTRUCTED = 11,
	KM_MSG_SENDER_REGISTERED = 12,
	KM_MSG_SENDER_UNREGISTERED = 13,
	//KM_MSG_SENDER_REQUEST_DESTRUCTION = 17,
	
	KM_MSG_POSTMAN_DESTRUCTED = 18,
	KM_MSG_POSTMAN_ANOTHER_HOLDER_READY = 19,
	KM_TEXTURE_RELOADED = 20,
	KM_TIME_ON_SOON_REACH_HOUR = 30,///�ڶ���������ʱ��
	KM_TIME_ON_REACH_HOUR = 31,		///ͬ��
	KM_REGION_CHANGED = 35,			///�ڶ�����������������Index
	KM_DATA_SYNCHRONIZE = 40,
	KM_DATA_REQUEST_FOR_CONTROL = 41,


	KM_LISTENER_TYPE_BEGIN = 90,
	KM_D3DDEVICE_BEGIN = 90,
	KM_D3DDEVICE_CREATE = KM_D3DDEVICE_BEGIN,
	KM_D3DDEVICE_RELEASE = KM_D3DDEVICE_BEGIN + 1,
	KM_D3DDEVICE_LOST = KM_D3DDEVICE_BEGIN + 2,
	KM_D3DDEVICE_RESET = KM_D3DDEVICE_BEGIN + 3,
	KM_D3DDEVICE_END = KM_D3DDEVICE_BEGIN + 4,

	KM_WINDOWS_MESSAGE = 100,		//�ڶ���������KG3DWindowsMessage*
	KM_MOUSE_MOUSE = 101,			//������Ϣ��wParam��lParam��WM_MOUSE_MOVE��ͬ
	KM_EXECUTE_ACTION = 103,		//�ڶ���������KG3DMessageActionExecutionStructָ�룬�ο�EXEACTION_MOUSE_MOVE�ȶԲ����Ķ��塣�����������Ƿ�����Ϣ�Ĵ���IEKG3DSceneOutputWnd��ָ�룬����֤�ǿա�
	KM_RENDER = 104,
	KM_FRAMEMOVE = 105,
	KM_STATE_CHANGE = 106,			//��һ���������µ�State���ڶ��������Ǿɵ�State
	KM_COMMON_CONTAINER_UPDATED = 107,//��������Container��ˢ�µ���Ϣ
	KM_LOADING_PERCENT = 108,
	KM_LISTENER_TYPE_END = 110,	//20������KM_LISTENER_TYPE_BEGIN����Щ��Ϣ����������Listener������
	KM_LISTENER_TYPE_COUNT = KM_LISTENER_TYPE_END - KM_LISTENER_TYPE_BEGIN,

	KM_SELECT = 300,		//lParam��ѡ���״̬��EM_EVENT_STATE_BEGIN��
	KM_TRANSFORM_TYPE_BEGIN = 301,
	KM_TRANSLATION = KM_TRANSFORM_TYPE_BEGIN,	//wParam��D3DXVECTOR3*��lParam��EM_EVENT_STATE_BEGIN��
	KM_ROTATION = 302,		//wParam��D3DXQUATERNION*��lParam��EM_EVENT_STATE_BEGIN��
	KM_SCALING	= 303,		//wParam��D3DXVECTOR3*��lParam��EM_EVENT_STATE_BEGIN��
	KM_TRANSFORM_TYPE_END = KM_SCALING,

	KM_CAM_POS = 310,		//�����λ�ñ仯��wParam��D3DXVECTOR3*

	KM_SCENE_LOADING_PERCENT = 400,	//wParam��KG3DMessageSceneLoadingPercent*
}KM_MESSEGE;

//KG3DMessage�ṹ	
struct KG3DMessage
{
	unsigned int m_uMessage;
	DWORD_PTR m_dwWParam;
	DWORD_PTR m_dwLParam;

	KG3DMessage(){ZeroMemory(this,sizeof(KG3DMessage));}
	KG3DMessage( unsigned int uMessage, DWORD_PTR dwWParam = 0, DWORD_PTR dwLParam = 0 ):m_uMessage(uMessage),m_dwWParam(dwWParam),m_dwLParam(dwLParam){}
	inline bool operator==(const KG3DMessage& Other){return 0==memcmp(this, &Other, sizeof(KG3DMessage));}
	inline bool operator!=(const KG3DMessage& Other){return ! operator==(Other);}
	inline KG3DMessage& operator=(const KG3DMessage& Other){memcpy_s(this, sizeof(KG3DMessage), &Other, sizeof(KG3DMessage)); return *this;}
};
struct KG3DWindowsMessage	//ע�⣬�����KG3DMessage��û�й�ϵ��
{
	unsigned int m_uMessage;
	DWORD_PTR m_dwWParam;
	DWORD_PTR m_dwLParam;

	KG3DWindowsMessage(){ZeroMemory(this,sizeof(KG3DWindowsMessage));}
	KG3DWindowsMessage( unsigned int uMessage, DWORD_PTR dwWParam = 0, DWORD_PTR dwLParam = 0 ):m_uMessage(uMessage),m_dwWParam(dwWParam),m_dwLParam(dwLParam){}
	inline bool operator==(const KG3DWindowsMessage& Other){return 0==memcmp(this, &Other, sizeof(KG3DWindowsMessage));}
	inline bool operator!=(const KG3DWindowsMessage& Other){return ! operator==(Other);}
	inline KG3DWindowsMessage& operator=(const KG3DWindowsMessage& Other){memcpy_s(this, sizeof(KG3DWindowsMessage), &Other, sizeof(KG3DWindowsMessage)); return *this;}
};
/************************************************************************/
//�ӿ�����
/************************************************************************/
struct IKG3DMessageSenderBase;
struct IKG3DMessageReceiverBase;

//////////////////////////////////////////////////////////////////////////
struct IKG3DMessageReceiverBase 
{
	virtual void __stdcall OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender ) = 0;
	virtual ~IKG3DMessageReceiverBase() = 0{}
};
//////////////////////////////////////////////////////////////////////////
struct IKG3DMessageSenderBase 
{
	virtual bool __stdcall RegisterReceiver( IKG3DMessageReceiverBase* pReceiver ) = 0;
	virtual bool __stdcall UnRegisterReceiver( IKG3DMessageReceiverBase* pReceiver ) = 0;
	virtual ~IKG3DMessageSenderBase() = 0{}
};
	
////////////////////////////////////////////////////////////////////////////////
/************************************************************************/
//Sender��Receive������ࡣI��ͷ�Ľӿ���Ϊ�ӿڹ淶�����ƣ�������protected������
//���Խӿ���˵����Ҳ���ϣ���ҲӦ����Ĭ��ʵ�ֺ�����������Ҫ�����������ฺ����
//һЩ���⣬ע��Ҫ��Messenger�Ļ���һ��Ҫ���������������������Ȼ����
/************************************************************************/
class KG3DMessageSenderBase;

class KG3DMessageReceiverBase : public IKG3DMessageReceiverBase
{
	friend KG3DMessageSenderBase;
public:
	virtual ~KG3DMessageReceiverBase() = 0{}
protected:
	virtual KG3DMessageSenderBase* GetSender(DWORD_PTR) = 0;	//�����ܵ��ã�����������Ƿ�¶
	virtual ULONG	GetCapacity() = 0; //�ܹ����ܶ���Sender
public:
	virtual void __stdcall OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender ) = 0;
private:
	virtual bool PickMessage( KG3DMessage Message , KG3DMessageSenderBase* pSender ) = 0;	//����Sender��Receiver֮����໥ͨ�ţ���Ҫ��������ӿ�
};

//////////////////////////////////////////////////////////////////////////

class KG3DMessageSenderBase : public IKG3DMessageSenderBase
{
public:
	virtual bool __stdcall RegisterReceiver( IKG3DMessageReceiverBase* pReceiver ) = 0;
	virtual bool __stdcall UnRegisterReceiver( IKG3DMessageReceiverBase* pReceiver ) = 0;
	virtual ~KG3DMessageSenderBase() = 0{}
protected:
	virtual void PostMessage( const KG3DMessage& Message ) = 0;	//ֻ�������ܵ���
	virtual KG3DMessageReceiverBase* GetReceiver(DWORD_PTR) = 0;//�����ܵ��ã�����������Ƿ�¶
	virtual ULONG	GetCapacity() = 0;	//�ܽ��ܶ���Receiverr
	bool LetReceiverPickMessage(KG3DMessageReceiverBase& Receiver, const KG3DMessage& Message)//��ΪPickMessageֻ��KG3DMessageSenderBase�����ã�����Ҫ���������ת�Ƶ���
	{
		return Receiver.PickMessage(Message, this);
	}
};





#endif //_INCLUDE_IKG3DMESSENGER_H_

