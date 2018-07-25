// -------------------------------------------------------------------------
//	�ļ���		��	UiGetString.cpp
//	������		��	Wooy(Wu yue)
//	����ʱ��	��	2003-3-18
//	��������	��	����ַ�������
//	modified	��	�޸�Ϊ�����ַ��������ֵ�ͨ�ý���[wxb 2004-3-29]
//					���� minLen/maxLen ����������ʱ��ʾ������ֵ��Χ
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiGetString.h"
#include "UiInformation.h"
#include "../UiSoundSetting.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////

#define	SCHEME_INI 	"�����ִ�����.ini"


KUiGetString* KUiGetString::m_pSelf = NULL;

KUiGetString::KUiGetString()
{
	m_pRequester = NULL;
	m_bForbidMove = FALSE;
}

//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiGetString* KUiGetString::OpenWindow(const char* pszTitle,
						const char* pszInitString,
						KWndWindow* pRequester, unsigned int uParam,
 						int nMinLen, int nMaxLen,
						BOOL bForbidMove, BOOL bInputNumber)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiGetString;
		if (m_pSelf)
			m_pSelf->Initialize(bInputNumber);
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_bForbidMove = bForbidMove;
		m_pSelf->m_bIsNumberInput = bInputNumber;
		m_pSelf->m_nMinLen = nMinLen;
		m_pSelf->m_nMaxLen = nMaxLen;
		m_pSelf->m_pRequester = pRequester;
		m_pSelf->m_uRequesterParam = uParam;
		if (pszTitle)
			m_pSelf->m_Title.SetText(pszTitle);
		else
			m_pSelf->m_Title.SetText("");

		if (!bInputNumber)
		{
			m_pSelf->m_StringEdit.SetLimitLen(nMaxLen);
			if (pszInitString)
				m_pSelf->m_StringEdit.SetText(pszInitString);
			else
				m_pSelf->m_StringEdit.ClearText();
		}
		else
			m_pSelf->m_StringEdit.SetIntText(nMinLen);

		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

void KUiGetString::OnCheckInput()
{
	if (!m_bIsNumberInput)
		return;

	int nNumber = m_StringEdit.GetIntNumber();
	/*if (nNumber < m_nMinLen)
		nNumber = m_nMinLen;
	else */if (nNumber > m_nMaxLen)
		nNumber = m_nMaxLen;
	char	szBuff1[16], szBuff2[16];
	itoa(nNumber, szBuff1, 10);
	m_StringEdit.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_StringEdit.SetIntText(nNumber);
}

//--------------------------------------------------------------------------
//	���ܣ��ر����ٴ���
//--------------------------------------------------------------------------
void KUiGetString::CloseWindow(bool bDestroy)
{
	bDestroy = TRUE;		//ǿ���ͷ� [wxb 2004-3-29]
	if (m_pSelf)
	{
		m_pSelf->m_pRequester = NULL;
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
	}
}

KUiGetString*	KUiGetString::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//��ʾ����
void KUiGetString::Show()
{
	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetFocusWnd(&m_StringEdit);
	Wnd_SetExclusive((KWndWindow*)this);
}

//���ش���
void KUiGetString::Hide()
{
	Wnd_SetFocusWnd(NULL);
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
void KUiGetString::Initialize(BOOL bNumber)
{
	AddChild(&m_Title);
	AddChild(&m_StringEdit);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	if (bNumber)
	{
		m_StringEdit.SetLimitLen(9);
		m_StringEdit.SetType(0);
	}

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiGetString::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_Title.Init(&Ini, "Title");
			m_pSelf->m_StringEdit.Init(&Ini, "StringInput");
			m_pSelf->m_OkBtn.Init(&Ini, "OkBtn");
			m_pSelf->m_CancelBtn.Init(&Ini, "CancelBtn");
		}
	}
}

//--------------------------------------------------------------------------
//	���ܣ�������Ϣ����
//--------------------------------------------------------------------------
int KUiGetString::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
			OnOk();
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			OnOk();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN &&
			uParam == (unsigned int)(KWndWindow*)&m_StringEdit)
		{
			OnOk();
			return 1;
		}
		break;
	case WND_N_EDIT_CHANGE:
		OnCheckInput();
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ���ȷ�ϰ�ť
//--------------------------------------------------------------------------
void KUiGetString::OnOk()
{
	if (m_bIsNumberInput)
	{
		//�������� [wxb 2004-3-29]
		int nNumber = m_StringEdit.GetIntNumber();
		if (nNumber < m_nMinLen)
		{
			UIMessageBox("���������̫С��");
		}
		else
		{
			if (m_pRequester)
				m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, WND_OPER_RESULT(nNumber));
			CloseWindow(true);
		}
		return;
	}

	char	szString[32];

	int nLen = m_StringEdit.GetText(szString, sizeof(szString), true);
	if (nLen >= m_nMinLen && nLen <= m_nMaxLen)
	{
		if (m_pRequester)
			m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, (int)&szString);
		CloseWindow(true);
	}
	else
	{
		UIMessageBox(((nLen < m_nMinLen) ?
			"������ִ�̫�٣�" : "��������������������󳤶ȣ�"));
	}
}

void KUiGetString::OnCancel()
{
	if (m_pRequester && !m_bIsNumberInput)
		m_pRequester->WndProc(WND_M_OTHER_WORK_RESULT, m_uRequesterParam, 0);
	CloseWindow(true);
}
