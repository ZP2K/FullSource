/*****************************************************************************************
//	����--�������������
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-3-23
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#define	MAX_PHRASE_COUNT	10
#define MAX_EMOTE_LENGTH    128
#define MAX_NAME_AND_CMD_LENGTH 16

#define	HAT_MSG_SPECIAL_CONTROL_PREFIX	VK_ESCAPE

//�������ǰ׺��
enum	CHAT_MSG_SPECIAL_CONTROL
{
	CMSC_EMOTE = 1,			//��ɫ����
	CMSC_AUTOREPLY_MSG,		//�Զ��ظ���������Ϣ
	CMSC_OFFLINE_MSG,		//�Լ�������ʱ���˷�������Ϣ
};

class KUiChatPhrase
{
public:
	KUiChatPhrase();
	~KUiChatPhrase();
	static int		Initialize();
	int		GetPhrase(int nIndex, char* pszPhrase);
	int		SetPhrase(int nIndex, char* pszPhrase, int nLen);
	int		GetPhraseCount();
	int		SavePrivateSetting(KIniFile* pFile, LPCSTR lpSection, int nStart);
	int     SavePrivateEmote(KIniFile* pFile, LPCSTR lpSection, int nStart);
	void	ClearAllPhrase();

	enum EMOTE_TYPE
	{
		EMOTE_T_PHRASE = 0,
		EMOTE_T_MENU,
	};
	//�����������춯���ļ�
	int     LoadEntireEmote();
	//�������춯�����и���index�ľ͸�index�ģ�û�о͸���CmdѰ��Ҫ�滻��
	int     SetEmote(char *szCmd, char *szName, char *szStringTarget, char *szStringSelf, int nIndex = -1);
	//
	int     GetEmoteCount(EMOTE_TYPE eType);
	//���������ȡһ����������(pBuff)�������������ȣ������ǰ�����MENU��PHRASE��
	int     GetEmote(const char *szCmd, char *pBuff, int nBuffLen, int nType = 0);
	//����������ȡһ����������(pBuff)�������������ȣ��������ǰ�����MENU��PHRASE��
	int     GetEmote(int nIndex, char *pBuff, int nBuffLen, int nType = 0);
	//��ȡһ�����ڲ˵��ϵĶ�������(pBuff)�������������ȣ�����MENU
	int     GetMenuEmote(const char *szCmd, char *pBuff, int nBuffLen, int nType = 0);
	int     GetMenuEmote(int nIndex, char *pBuff, int nBuffLen, int nType = 0);
	//ȡ��ָ�������Ķ�������
	int     GetMenuEmoteName(int nIndex, char *pBuff, int nBuffLen);
	//�����ַ���
	int     ConvertEmoteString(char *szString, int nStringLen, const char *szMyName, const char *szTarName);

	//�Ѷ�ȡ����Emote��Ϣ�����ȥShortcutϵͳ�еĺ���������
	int     ConstructFunctionAlias();
	int     ConstructAAlias(char* szCmd, int nIndex);

	//�����Զ��ظ����
	static bool	SetAutoReply(const char* pszWords, bool bTakeEffect);
	//�����Զ��ظ����
	static bool SetAutoReply(int nIndex);
	//��ȡ�Զ��ظ����
	static int	GetAutoReply(char* pszBuffer, int nSize, bool bIncludePrefix);

private:
	struct  EMOTE_PHRASE
	{
		char    szName[MAX_NAME_AND_CMD_LENGTH];
		int     nNameLen;
		char    szString[MAX_EMOTE_LENGTH];
		int     nStringLen;
		char    szStringMe[MAX_EMOTE_LENGTH];
		int     nStringMeLen;
		char    szCmd[MAX_NAME_AND_CMD_LENGTH];
	} *m_pEmotePhrase, *m_pMenuEmote;
	
	int     LoadEmotePhrase(KIniFile *pIni);
	int     LoadMenuEmote(KIniFile *pIni);
	int     HandleLoad(KIniFile *pIni, EMOTE_PHRASE *pData, int nCount, const char *szSection);
	int     FindEmote(EMOTE_TYPE eType, const char *szCmd);

private:
	struct	CHAT_PHRASE
	{
		char	szString[64];
		int		nStringLen;
	}		m_PhraseList[MAX_PHRASE_COUNT];

	int m_nEmotePhraseCount;
	int m_nMenuEmoteCount;

	unsigned int m_uMyNameColor;
	unsigned int m_uTargetColor;

private:
	//�Զ��ظ�����
#define			REPLY_COUNT		4
#define			MAX_REPLY_LEN	128
	//��ǰʹ�õ��Զ��ظ���������-1��ʾ��ǰδ�趨ʹ���Զ��ظ�
	static int		ms_nCurrentUseReplyIndex;
	//�Զ��ظ����������ݣ���0��Ϊ������趨��
	static char		ms_szReplyContent[REPLY_COUNT][MAX_REPLY_LEN];
	//�Զ��ظ����ݵĳ���
	static int		ms_nReplyContentLen[REPLY_COUNT];
};

extern KUiChatPhrase	g_UiChatPhrase;