/*****************************************************************************************
//	����--��Ϣ�������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-12-13
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndMessageListBox.h"

#pragma warning( disable : 4786 4800 4355 4146 4273 4503 )
#include <map>
#include <string>
using namespace std;
#include "../FilterTextLib.h"

int VerticalSplitTwoWindow(KWndWindow* pLeft, KWndWindow* pRight, int nAt);

struct KUiMsgParam;

#define MAX_CHANNELRESOURCE 10

struct KChannelResourceInfo
{
	char cTitle[32];
	char cShortTitle[MAX_CHANNELRESOURCE][32];
	KRColor uTextColor;
	KRColor uTextBorderColor;
	WORD nMenuPicIndex;
	WORD nMenuPicHeight;
	char cMenuText[32];
	WORD nMenuDeactivatePicIndex;
	WORD nMenuDeactivatePicHeight;
	char cMenuDeactivateText[32];
	KRColor uMenuBkColor;
	WORD nTextPicIndex;
	char cFormatName[32];	//����Ƶ������
	int nNeverClose;
	char szSoundFileName[80];
	unsigned int uSendMsgInterval;	//���ڷ�����Ϣ��ʱ����(����)
	unsigned int uSendMsgNum;	//���ڻ����������Ϣ�����Ŀ,���10��
};

struct KChannelActivateInfo
{
	DWORD nChannelID;
	BYTE cost;
	int ResourceIndex;	//-1��ʾ����Դ
	unsigned int uLastSendMsgTime;	//�ϴη���Ϣ��ʱ��
	unsigned int uLeftSendMsgNum;	//�Ѿ�����Ĵ�����Ϣ
	unsigned int uBufferOffset;	//���ÿռ����ʼλ��
	char Buffer[1024 * 10];
};

struct KMSN_ChannelInfo
{
	KRColor uTextColorSelf;
	KRColor uTextBorderColorSelf;
	WORD nSelfTextPicIndex;
	KRColor uTextBKColorSelf;
	
	KRColor uTextFriendColor;
	KRColor uTextBorderFriendColor;
	WORD nFriendMenuPicIndex;
	WORD nFriendMenuPicHeight;
	KRColor uFriendMenuBkColor;
	WORD nFriendTextPicIndex;
	char szFriendSoundFileName[80];

	KRColor uTextColorUnknown;
	KRColor uTextBorderColorUnknown;
	WORD nStrangerMenuPicIndex;
	WORD nStrangerMenuPicHeight;
	KRColor uStrangerMenuBkColor;
	WORD nStrangerTextPicIndex;
};

class KSysMsgCentrePad : public KWndWindow
{
public:
	KWndButton				m_OpenSysButton;
	KWndButton				m_UpButton;
	KWndButton				m_DownButton;
	KWndMessageListBox		m_SysRoom;
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	void ScrollBottom();
};

class KUiMsgCentrePad : public KWndWindow
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiMsgCentrePad* OpenWindow();					//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static void				CloseWindow(bool bDestroy);		//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void				Clear();
	static void				LoadScheme(const char* pScheme);//������淽��
	static void				DefaultScheme(const char* pScheme);//���³�ʼ������
	static void				HideAllMessage();//�����������
	static void				ShowAllMessage();//��ʾ�������
	static void				SetPrivateMsgMode(bool bPrivateMsgMode);//�����Ƿ�ֻ��ʾ˽����Ϣ
	static bool				IsPrivateMsgMode();						//�Ƿ���ֻ��ʾ˽����Ϣ״̬
	
	static void				SystemMessageArrival(const char* pMsgBuff, unsigned short nMsgLength);

	//����������Э�鵽��Ĵ���
	static int				NewChannelMessageArrival(DWORD nChannelID, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength);
	static void				NewMSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength);
	static void				OpenChannel(const char* channelName, DWORD nChannelID, BYTE cost);	//ͬʱ����
	static void				NewChannel(const char* pszChannel, const char* pszName);
	static void				FreeChannel(const char* pszChannel);
	//������ҵ�����
	static void				ShowSomeoneMessage(char* szSourceName, const char* pMsgBuff, unsigned short nMsgLength);
	enum  SelfChannel
	{
		ch_Team = 0,
		ch_Faction,
		ch_Tong,
		ch_Screen,
		ch_GM,
		ch_Union,
	};
	static void				CloseSelfChannel(SelfChannel type);	//ͬʱ�˶�

	static void				QueryAllChannel();
	static void				ReplaceChannelName(char* szDest, size_t nDestSize, char* szSrc);
	static bool				GetChannelMenuinfo(int nChannelIndex, WORD* pnPicIndex = NULL, WORD* pPicHei = NULL, KRColor* puTextColor = NULL, KRColor* puBkColor = NULL, char* pszMenuText = NULL, short* pnCheckPicIndex = NULL);
	static bool				GetPeopleMenuinfo(char* szDest, WORD* pnPicIndex = NULL, WORD* pPicHei = NULL, KRColor* puTextColor = NULL, KRColor* puBkColor = NULL);
	static bool				ReplaceSpecialField(char* szDest, char* szSrc);
	static DWORD			GetChannelID(int nChannelIndex);
	static char*			GetChannelTitle(int nChannelIndex);
	static DWORD			GetChannelCount();
	static int				GetChannelIndex(char* channelName);
	static int				GetChannelIndex(DWORD dwID);
	static int				ReleaseActivateChannelAll();	//����ԭ�����ܸ���
	static bool				GetChannelSubscribe(int nChannelIndex);
	static bool				IsChannelType(int nChannelIndex, SelfChannel type);
	static int				CheckChannel(int nChannelIndex, bool b);
	static int				CheckChannel(char* channelName, bool b);
	static KUiMsgCentrePad* GetSelf()	{return m_pSelf;}
	static void SetFontSize(int nFontSize);
	static int GetFontSize();
	static int SetChannelTextColor(char* cTitle, DWORD uTextColor, DWORD uTextBorderColor);
	static char* GetChannelTextColor(int nIndex, DWORD& uTextColor, DWORD& uTextBorderColor);
	static void SetMSNTextColor(int nType, DWORD uTextColor, DWORD uTextBorderColor);	//nType 0 is me, 1 is friend, 2 is stranger
	static void GetMSNTextColor(int nType, DWORD& uTextColor, DWORD& uTextBorderColor);
	static BYTE	GetChannelCost(DWORD dwID);
	static int	PushChannelData(DWORD dwID, const char* Buffer, int nLen);
	static int	GetChannelData(DWORD& dwID, BYTE& cost, char*& Buffer, int& nLen);
	static int	PopChannelData(DWORD dwID);

	//�Ƿ�Ҫ��������ѡ��
	static int  IsPopupFilterItem();

	virtual void SetSize(int nWidth, int nHeight);

	void	Breathe();								//�����

	//��һ�����˱��ʽд�뱾ģ����ַ�����ģ��
	void	AddFilterExpression(char *szKeyWord);

	//����ַ�����ģ��������ı��ʽ
	void	CleanFilterModule();

	//���ù��˹���
	void	SetFilter(int nFilterLevel, int nChannelIndex = 0, char* szKeyWord = NULL, char* szPlayerName = NULL);

public://˽�����ݶ���

	//���˹���ṹ����
	struct MSG_CENTER_FILTER_RULE
	{
		MSG_CENTER_FILTER_RULE()
		{
			nChannelIndex = -1;
			nFilterLevel  = enumFL_APPEARANCE;
			szKeyWord[0]  = szPlayerName[0] = 0;
		};
		int  nFilterLevel;		//�˹��˹���Ĺ��˵ȼ����ο�enumFILTER_LEVEL
		int  nChannelIndex;		//�˹��˹��������ڵ�Ƶ����Index
		char szKeyWord[256];	//���˹ؼ���
		char szPlayerName[32];	//�Ƿ����Ϊ��һ������ҵ���Ϣ
	};
	//     MSG_CENTER_FILTER_RULE

	//���˵ȼ�
	enum enumFILTER_LEVEL
	{
		enumFL_INCOMING,
		enumFL_APPEARANCE,
	};

	enum enumRESULT_TYPE
	{
		enumRT_WORLD_CHANNEL_KEYWORD,
	};

private:
	KUiMsgCentrePad();
	~KUiMsgCentrePad();
	void	Initialize();							//��ʼ��
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);//���ں���
	int		ChanageHeight(int nOffsetY, bool bAtTop);
	void	LoadScheme(KIniFile* pIni);
	int		PtInWindow(int x, int y);				//�ж�һ�����Ƿ����ڴ�����,������Ǿ�������
	void	SetAutoDelMsgInterval(unsigned int uInterval = 0);	//�����Զ�ɾ����Ϣ��ʱ����
				//����Ĭ��ֵΪ0����ʾ������ʱ�����Զ�ɾ����ʱ�䵥λ����
	void	DragWndSize(int nMoveOffset);
	void	DragWndPos(int nMoveOffset);
	void	PaintWindow();							//���ƴ���

	int		FindActivateChannelResourceIndex(char* cTitle);
	int		FindActivateChannelIndex(char* cTitle);
	int		FindActivateChannelIndex(DWORD nChannelID);
	BYTE	FindActivateChannelCost(DWORD nChannelID);
	int		FindChannelResourceIndex(char* cTitle);
	int		FindActivateChannelIndexByKey(const char* cKey);
	int		IsNeverCloseActivateChannel(int nChannelIndex);
	bool	ChannelMessageArrival(int nChannelIndex, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, KWndMessageListBox* pM, bool bName);
	void	MSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength);
	void	ShowMSNMessage(char* szName, const char* pMsgBuff, unsigned short nMsgLength, KRColor uColor, KRColor uBKColor, KRColor uBorderColor, WORD nPic);
	void	SendQueryChannelID(int nChannelResourceIndex);
	void	SendChannelSubscribe(int nChannelIndex, bool b);
	int		AddActivateChannel(const KChannelActivateInfo& Item);	//��������Item��Index
	int		ReleaseActivateChannel(int nChannelIndex);	//����ʣ����ܸ���
	void	CloseActivateChannel(int nChannelIndex);	//ͬʱ�˶�

	void	PopupChannelMenu(int x, int y);
	
	int		FilterTextColor(char* pMsgBuff, unsigned short nMsgLength, const KRColor& uColor);
	
	int		GetMessageSendName(KWndWindow* pWnd, int nIndex, char* szCurText);
	int		GetMessageSendName(KWndWindow* pWnd, int x, int y, char* szCurText);

	//��Ϣ���˷���������һ���Ĺ�����˽��յ��ĺ�Ҫ��ʾ����Ϣ
	int		IsMsgPass(int nChannelIndex, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, enumFILTER_LEVEL eLevel);

private:
	static KUiMsgCentrePad*	m_pSelf;

	bool				m_bPrivateMsgMode;	//�Ƿ���ֻ��ʾ˽����Ϣ״̬
	unsigned int		m_hMsgData;			//������Ϣ����(��m_bPrivateMsgMode==trueʱ��Ϊ�����������ݣ�)
											//	(��m_bPrivateMsgMode==falseʱ��Ϊ˽����������)

	KWndImage			m_BorderImg;
	KWndImage			m_SizeBtn;
	KWndImage			m_MoveImg;
	KWndButton          m_BgShadowBtn;
	
	int                 m_nSizeBtnX;
	int                 m_nSizeBtnY;
	int                 m_nMoveImgX;
	int                 m_nMoveImgY;

	KSysMsgCentrePad	m_Sys;

	int					m_nMinTopPos, m_nMaxBottomPos;
	bool				m_bSizingWnd;
	bool				m_bDockLeftSide;
	bool                m_bShowShadow;

	short				m_nSizeBtnDragOffsetY;
	
	bool				m_bSizeUp;

#define SECOND_AUTODELMSG 20000
	unsigned int	m_uAutoDelMsgInterval;	//�Զ�ɾ����Ϣ�ļ��ʱ�䣬��λ����
	unsigned int	m_uLastDelMsgTime;		//�ϴ�ɾ����Ϣʱ��ʱ��


	KScrollMessageListBox m_ChatRoom;

	KChannelResourceInfo m_ChannelsResource[MAX_CHANNELRESOURCE];
	int m_nChannelsResource;
	char m_DefaultChannelSendName[32];
	int m_nDefaultChannelResource;
	KChannelActivateInfo* m_pActivateChannel;
	int m_nActivateChannels;
	typedef std::map<std::string, int> SUBSCRIBELIST;
	SUBSCRIBELIST m_CloseList;	//�ڱ��е�Ƶ��Ϊ�رյ�Ƶ��
	KMSN_ChannelInfo m_MSNInfo;
	short m_nCheckOnPicIndex;
	short m_nCheckOffPicIndex;

	KRColor m_NameColor;
	KRColor m_BorderNameColor;

	static int ms_DefaultHeight, ms_DefaultWidth;

	MSG_CENTER_FILTER_RULE		m_FilterRule;
	ITextFilter*				m_pTextFilter;
	int							m_nbFilterItem;
};
