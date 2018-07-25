/*****************************************************************************************
//	�[�򴰿ڵĽӿ�
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-18
------------------------------------------------------------------------------------------
	������ȴ��9-18���ܣ����ǹ�ȥ��ζ�ű��ѣ�
*****************************************************************************************/
#pragma once

#include "../Elem/WndGameSpace.h"
#include "../Elem/WndWindow.h"
#include "GameDataDef.h"

enum enumGAME_SPACE_RESULT
{
	enumGSR_GET_MONEY_FOR_GIVE,
	enumGSR_GET_ROLENAME_FOR_WISEMAN,
	enumGSR_GET_STRING_COMMON,
	enumGSR_GET_NUMBER_COMMON,
};

class KUiGameSpace : public KWndWindow
{
public:
	enum enumTRIGGER_TYPE
	{
		enumTT_SWITCH_SCENE_FINISH,
		enumTT_GAME_START,
	};

	enum enumGAME_RUNNING_SETTING
	{
		enumGRS_INVISIBLE_FRIEND_LIST,
		enumGRS_NUM,
	};

public:
	KUiGameSpace();
	//���봦����Ϣ�Լ�һЩ�ض�������Ϣ����Ӧ
	int	WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	//������Ϸ����
	void PaintWindow();

	void OnMouseMoveCursor(int x,int y, BOOL bCtrl);

	KUiPlayerItem m_SelectPlayer;

	void SetGiveMoneyTargetID(unsigned int uID);	//�������ͽ�Ǯ�Ķ����ID

	void Trigger(enumTRIGGER_TYPE eType, unsigned int uParam, int nParam);

	void Setting(enumGAME_RUNNING_SETTING eType, int nValue);

	int	 GetSetting(enumGAME_RUNNING_SETTING eType);

private:
	unsigned int	m_uGiveMoneyTargetID;			//���ͽ�Ǯ�Ķ����ID
	int				m_nGameSetting[enumGRS_NUM];
};

extern KUiGameSpace	g_WndGameSpace;

enum
{
	ACTION_CHAT = 0,					//Ҫ��Name
	ACTION_MAKEFRIEND,					//Ҫ��Name
	ACTION_TRADE,						//pDest->nData == PLAYER_MENU_STATE_TRADEOPEN && pDest->nIndex != -1
	ACTION_JOINTEAM,					//pDest->nData == PLAYER_MENU_STATE_TEAMOPEN && pDest->nIndex != -1
	ACTION_INVITETEAM,					//pDest->uId != 0
	ACTION_FOLLOW,						//pDest->nIndex != -1
	ACTION_VIEWITEM,					//pDest->uId != 0
	ACTION_TONG,                        //Ҫ��Name
	ACTION_BLACKLIST,					//Ҫ��Name
	ACTION_REVENGE,						//pDest->uId != 0
	ACTION_GIVE_MONEY,					//��Ǯ��Ҫ pDest->uId != 0
	ACTION_FILTER_WORLD_CHANNEL,		//��Ƶ�ؼ��ֹ��ˣ���Ҫ�ж�KUiMsgCenterPad::IsPopupFilterItem()
};

//pDest��Name������, Ȼ���������ݾ��������д
void ProcessPeople(KUiPlayerItem* pDest, int nAction);
void ProcessEmote(char* szDest, char *szDestChannel = "", int nEmoteIndex = 0);

void PopUpContextPeopleMenu(const KUiPlayerItem& SelectPlayer, int x, int y);
void PopUpContextEmoteMenu(char* szDest, int x, int y);