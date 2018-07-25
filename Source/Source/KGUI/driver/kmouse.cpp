#include "stdafx.h"
#include "./kmouse.h"
#include "../elem/wnds.h"
//#include "../uibase.h"

namespace UI
{
	KMouse::~KMouse()
	{
	}

	int KMouse::SetCurrentCursorIndex(int nIndex)
	{
		return Wnd_SwitchCursor(nIndex);
	}

	int KMouse::GetCurrentCursorByStatus() const
	{
#if 0
		int	nCursorIndex = CURSOR_NORMAL;
		switch (g_UiBase.GetAppearStatus())
		{
		case UIS_S_TRADE_PLAYER:	//����Player������
		case UIS_S_TRADE_NPC:		//����npc������
			nCursorIndex = CURSOR_NORMAL;
			break;
		case UIS_S_TRADE_SALE:		//����npc������ ��
			nCursorIndex = CURSOR_SELL;
			break;
		case UIS_S_TRADE_BUY:		//����npc������ ��
			nCursorIndex = CURSOR_BUY;
			break;
		case UIS_S_TRADE_REPAIR:	//����npc������ ����
			nCursorIndex = CURSOR_REPAIR;
			break;
		case UIS_S_STALL_SETPRICE:
			nCursorIndex = CURSOR_STALL_MARK_PIRCE;	//��ʱ��������ָ�������[wxb 2003-11-18]
			break;
		case UIS_S_STALL_BUY:
			nCursorIndex = CURSOR_BUY;	//��̯:(��)��������״̬
			break;
		case UIS_S_IDENTIFY:
			nCursorIndex = CURSOR_IDENTIFY; // ������Ʒ
			break;
		case UIS_S_INTERACTIVE:
			nCursorIndex = CURSOR_INTERACTIVE;	//����ҽ���
			break;
		case UIS_S_SEPARATE_ITEM:
			nCursorIndex = CURSOR_SEPARATE_ITEM;	// �����Ʒ
			break;
		}
		return nCursorIndex;
#endif // 0
		return 0;
	}

	int KMouse::HandleMouseInput(unsigned int uKey, int nModifier, int x, int y)
	{
		// TODO : ��KShortcutkeyCenter����
		return 0;
	}

}

