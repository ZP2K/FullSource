#pragma once

interface IEKG3DSceneEditorBase;

#define WM_ACTIONEXECUTED WM_USER + 2000
typedef struct tagKeyStateSnap
{
	unsigned MainKey : 8;		//��Ҫ���������������е�������ĸ���Լ�CTRL�ȿ��Ƽ�
	unsigned CtrlState:1;		//�Ƿ���Ҫ���� ctrl ������Ч��1 ��ʾ��Ҫ����
	unsigned ShiftState:1;
	unsigned AltState:1;
	unsigned LBTState:1;		//������
	unsigned MBTState:1;		//����м�	
	unsigned RBTState:1;		//����Ҽ�
	unsigned Reverse:2;			//����λ
}KeyStateSnap;

struct PROXYKEYMAP
{
	KeyStateSnap keystate;
	DWORD TriggerMsgID;				//��������ϢId, �磬��WM_MOUSEMOVEʱ����Ҫ�ж���ת��ͷ�ļ��Ƿ���
	WORD wActionID;					//ActionID
	BOOL bPrevState;				//��¼���һ���Ƿ��Ѿ�����Щ��ϼ�
	WORD wIgnoreState;				//���ԱȽϵ�״̬��0�����ԣ�1����shift,ctrl,alt�Ƚϣ�2����lmr�����Ƚ�, 3ȫ�����ԣ�ֻ�Ƚ�MainKey
	BOOL bExclusive;				//�Ƿ��ռ����ռʱ�������ж���������ϼ�λ
};

interface IEKG3DSceneSceneEditor;
class CInputProxy
{
public:
	CInputProxy();
	virtual ~CInputProxy(void);

public:
	void Init(IEKG3DSceneEditorBase* pSceneEditor, CWnd* pWnd, PROXYKEYMAP* pKeyMapArray);
	INT CheckInputEvent(MSG* pMsg);
	BOOL IsCaptured();
private:
	IEKG3DSceneEditorBase* m_pSceneEditor;
	CWnd* m_pWnd;
	PROXYKEYMAP* m_pKeyMapArray;

	static WORD m_wIgnoreKeyBoardMask, m_wIgnoreMouseMask, m_wIgnoreAllMask;
	static BOOL m_bIgnoreInit;

	//����δʵ�ֵĶ���
	CInputProxy& operator=(const CInputProxy& rhs);
	CInputProxy(const CInputProxy& src);

	BOOL m_bInCapture;
	INT m_nMouseX, m_nMouseY;
};
