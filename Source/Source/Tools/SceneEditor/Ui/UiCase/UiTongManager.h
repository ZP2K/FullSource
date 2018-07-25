/*******************************************************************************
File        : UiTongManager.h
Creator     : Fyt(Fan Zhanpeng)
create data : 08-29-2003(mm-dd-yyyy)
Description : ������Ľ���
********************************************************************************/

#if !defined(AFX_UITONGMANAGER_H__13BA213D_11EC_4F24_BF98_F51C3F414D6D__INCLUDED_)
#define AFX_UITONGMANAGER_H__13BA213D_11EC_4F24_BF98_F51C3F414D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndlabeledbutton.h"
#include "../elem/wndpuretextbtn.h"
#include "../elem/wndscrollbar.h"
#include "../elem/wndimage.h"
#include "../elem/wndlist2.h"

#include "../../Engine/Src/LinkStruct.h"
#include "../../Core/Src/GameDataDef.h"

#define TONG_MANAGER_STATUS_SHOW_ALL_MEMBER 0x01

struct KPilgarlicItem : public KTongMemberItem
{
	int nHashID;
	KPilgarlicItem *next;
    KPilgarlicItem *previous;
};


class KUiTongManager : KWndImage
{
public:
	KUiTongManager();
	virtual ~KUiTongManager();

	static        KUiTongManager* OpenWindow();      //�򿪴���
	static        KUiTongManager* OpenWindow(        //�򿪴��ڣ�ͬʱ���Ž��沼��
		                                     char* pszPlayerName);
	static        KUiTongManager* GetIfVisible();    //�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE); //�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);   //������淽��

	                                                 //�����µ�����
	static void   NewDataArrive(KUiGameObjectWithName *ParamInfo, KTongMemberItem *pIncome);

	                                                 //���ܰ����Ϣ
	static void   TongInfoArrive(KUiPlayerRelationWithOther* Relation, KTongInfo *pTongInfo);
	static int    RemoveTempFile();                  //ɾ����ʱ�ļ�

	static void   ResponseResult(                    //�Խ������Ӧ���
		                         KUiGameObjectWithName *pResult, int nbIsSucceed);


public:
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void          ArrangeComposition(                //���ݴ����������֣����Ž��沼��
		                             char* pszPlayerName);


private:
	void          Initialize();

	void          PopupConfirmWindow(                //����ȷ�ϴ���
		                        const char* pszInfo, unsigned int uHandleID, int nbSelf = 0, int nbLeague = 0);

	static void   RequestData(int nType, int nIndex);//��Core������������
	static void   SaveNewData(                       //���µ����б���Ϣ�浽��ʱ�ļ�
		                        KUiGameObjectWithName *ParamInfo, KTongMemberItem *pIncome);

	static void   SaveWholeData(int nType);          //��ָ�����͵��б����������洢����

	static int    IsPlayerExist(                     //��ѯ�ض����ֵ�����Ƿ��Ѿ�����
		                        KPilgarlicItem *MeToCmp, KLinkStruct<KPilgarlicItem> *pData);

	KLinkStruct<KPilgarlicItem>*                     //��ѯ�ض����ֵ���ҵ�����λ��
		          FindPlayer(char *pszName);

	KLinkStruct<KPilgarlicItem>*                     //������Ҵ�����
		          HandleFind(KLinkStruct<KPilgarlicItem> *pData, char *pszName);

	void          LoadData(int nType);               //��������
	int           LoadDataHandler(                   //�������ϵĴ�����
		                        KLinkStruct<KPilgarlicItem> *pData,
							    const char* szSection);

	void          UpdateBtnTheme(                    //���ݴ��˵���ݸ��°�ť���÷���
		                        int nType, BOOL IsDissable = FALSE);

	void		  UpdateThemeForList(int nType);	 //����ѡ����б��Ű�ť����

	KLinkStruct<KPilgarlicItem>*                     //���ص�ǰ��ѡ����б���������Ӧ�����ݼ�
		          GetCurrentSelectedData();

	KPilgarlicItem*                                  //������ѡ��Ļ�Ա
		          GetSelectedPlayer();

	void          UpdateTongInfoView();              //���°��ѶϢ����ʾ

	//�����б���
	void          UpdateTongListView(BOOL bAll = FALSE);

	void          ClearTongData();                   //�Ѱ����Ϣ�����

	void          UpdateListCheckButton(int nType);  //�����б�CheckBox�Ĺ���

	//���ְ������Ĵ�����
	void          OnAction(int nType, int nbForceBondingName = FALSE);

	//���в����İ���������
	void		  OnParamAction(int nType, unsigned int uParam, int nParam);

	void          OnRefresh();			             //��Ӧˢ�°�ť�Ĳ�������
	
	void		  OnAssign();						 //��Ӧ�����Ĳ���

	void		  OnSalary();						 //��Ӧ��Ǯ����

	void		  OnMemberTitle(int nParam);		 //��Ӧ���ڳƺ��޸ĵĲ���

	void          ResponseLeave(                     //��Ӧ����Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);

	void          ResponseDismiss(                   //��Ӧ���˺�Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);

	void          ResponseDemise(                    //��Ӧ��λ��Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);
	
	void          ResponseAssign(                    //��Ӧ������Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);

	void          ResponseApply(                     //��Ӧ�����Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);

	void          ResponseTitle(                     //��Ӧ�ı�ƺź�Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);

	void          ResponseCamp(                      //��Ӧ�ı���Ӫ��Ĳ���
		                        KUiGameObjectWithName *pResult, int nbIsSucceed);

	void		  ResponseCreateLeague(				 //��Ӧ�������˺�Ĳ���
								KUiGameObjectWithName *pResult, int nbIsSucceed);

	void		  ResponseDismisLeague(				 //��Ӧ��ɢ���˺�Ĳ���
								KUiGameObjectWithName *pResult, int nbIsSucceed);
	
	void		  ResponseDepositMoney(				 //��Ӧ��Ǯ��Ĳ���
								KUiGameObjectWithName *pResult, int nbIsSucceed);

	void		  ResponseTakeMoney(				 //��ӦȡǮ��Ĳ���
								KUiGameObjectWithName *pResult, int nbIsSucceed);
	
	void		  ResponseLeagueAction(				 //���˵�һ���������Ӧ���
								KUiGameObjectWithName *pResult, int nbIsSucceed);

	/**
	 *brief		  ��Ӧ�������Ĳ���
	 */
	void		  ResponseSalary(KUiGameObjectWithName *pResult, int nbIsSucceed);

	int           FormArrayData(                     //������������֯����������
		                        KTongMemberItem **pData, int nType);

	void		  OnPopupSelectSex(int nX, int nY);	 //�������ڳƺŸ��ĵ��Ա�ѡ��˵�

	void		  Refresh(int nType);				 //ˢ�µ�����

	int			  WorkoutIndex(int nSelectIndex,	 //�����б��ѡ��͵�ǰ����״̬�����
							   int nbLeft);			 //�������ݵ�index


	//*ATTENTION*����m_nRelation��ֵ֮��TongInfoArrive()�����֮�󼴿�
	//����m_nRelation��ֵȡ����Ϣ
	void		  PrepareData(BOOL bIsRefresh = FALSE);

	//����������ȻҪ�䵱�������ģ�����Faint
	//��������������ǰ������������ݸ�������Ҫ��Щ���ݵĽ���
	void		  GiveDataToOthers(KTongMemberItem *pItem, int nCount, int nType);

private:
	static        KUiTongManager* ms_pSelf;

	static int    m_nElderDataIndex;
	static int    m_nMemberDataIndex;
	static int    m_nCaptainDataIndex;
	static int	  m_nTongLeagueDataIndex;

private:
	enum RESULT_TYPE
	{
		RESULT_T_DISMISS,
		RESULT_T_ASSIGN,
		RESULT_T_DEMISE,
		RESULT_T_LEAVE,
		RESULT_T_PASSWORD_TO_DEMISE,
		RESULT_T_ASSIGN_TITLE,
		RESULT_T_SELECT_SEX,
		RESULT_T_GET_LEAGUE_NAME,
		RESULT_T_REMOVE_LEAGUE_MEMBER_CONFIRM,
		RESULT_T_LEAVE_LEAGUE_CONFIRM,
		RESULT_T_DISMISS_LEAGUE_CONFIRM,
		RESULT_T_DEPOSIT_MONEY,
		RESULT_T_TAKE_MONEY,
	};

private:
	KWndLabeledButton                                //���ˣ���������λ��ť
		          m_BtnDismiss, m_BtnAssign, m_BtnDemise;
	
	KWndLabeledButton
				  m_BtnApplyLeague;					 //�����������

	KWndLabeledButton
		          m_BtnLeave, m_BtnRecruit;          //�����˰�ť

	KWndLabeledButton
		          m_BtnApply, m_BtnCancel;           //���룬ȡ����ť

	KWndLabeledButton                                //���ϡ��ӳ�����Ա�б�
		          m_BtnElderList, m_BtnCaptainList, m_BtnMemberList;
	
	KWndLabeledButton
				  m_BtnLeagueList;					 //�����˰���б�һ����Ϊ��
													 //���Ե������ϵ�������һ������
													 //���������߰�ʮһ�ᡢ�����ھֵ�
													 //������ǲ쿴��������

	KWndButton                                       //�رա�ˢ�¡��ƺŰ�ť
		          m_BtnClose, m_BtnRefresh, m_BtnTitle;

	KWndButton    m_BtnExtendList;                   //��ʾ������û�������˵�λ�õİ�ť

	KWndButton	  m_BtnAddLeague;					 //�������
	KWndButton	  m_BtnLeaveLeague;					 //�뿪����
	KWndButton	  m_BtnRemoveLeague;				 //�Ƴ�����
	KWndButton	  m_BtnCreateLeague;				 //��������
	KWndButton	  m_BtnDepositMoney;				 //��Ǯ
	KWndButton	  m_BtnDismissLeague;				 //��ɢ����
	KWndButton	  m_BtnTakeMoney;					 //ȡǮ

	KWndText32    m_TongName;                        //�������
	KWndPureTextBtn
				  m_MasterName;						 //�ϴ�����(����)

	KWndPureTextBtn
				  m_Alignment;                       //��Ӫ����

	KWndText32    m_MemberCount, m_Money;            //�������ʽ�

	KWndText32	  m_TongExp;						 //��ᾭ��
	KWndText32	  m_TongLevel;						 //���ȼ�

	KWndList2     m_ListLeft;                        //�м���б�ؼ�
	KWndList2	  m_ListRight;
	KWndScrollBar m_ListScroll;                      //�б�Ĺ�����

	TONG_MEMBER_FIGURE
		          m_Relation;                        //��������������Ŀ����Ĺ�ϵ

	KWndButton	  m_MaleTitle, m_FemaleTitle;		 //�а��ں�Ů���ڵĳƺ�(ͼ�ΰ�)

	KWndButton	  m_btnSalary;						 //��Ǯ������Ҫ����


private:
	KLinkStruct<KPilgarlicItem>
		          m_MemberData, m_CaptainData;       //��Ա�б�Ͷӳ��б������

	KLinkStruct<KPilgarlicItem> m_ElderData;         //�����б�

	KLinkStruct<KPilgarlicItem> m_LeagueData;		 //�����б�

	static KTongInfo            m_TongData;          //������������


private:
	unsigned int  m_uLastRequestTongDataTime;        //��һ�����������ϵ�ʱ��
	unsigned int  m_uLastRequestElderListTime;       //��һ���������б��ʱ��
	unsigned int  m_uLastRequestMemberListTime;      //��һ����������б��ʱ��
	unsigned int  m_uLastRequestCaptainListTime;     //��һ������ӳ��б��ʱ��

	int           m_nRelation;                       //�����Ǹ���������򿪵İ��Ĺ�ϵ
	int           m_nIndex;                          //�������Ǹ���Ӧ����ҵ�Index
	char          m_szPlayerName[32];                //������򿪣�����һ���󶨵���ң�
	                                                 //����ˣ����Լ����������(��)��
	                                                 //���֡���һ���Ƕ�˵��ÿһ�δ򿪽���
	                                                 //����һ����ᣬ�����󶨵���

	int           m_nSelectIndex;                    //���б���ѡ�������±�
	int           m_nCurrentCheckBox;                //����ѡ���CheckBox

	char          m_szDismiss[64], m_szAssign[64];   //���˺�������ȷ�Ͽ���ַ���
	char          m_szDemise[64], m_szLeave[64];     //��λ���뿪��ȷ�Ͽ���ַ���
	char		  m_szRemoveLeagueMember[64];		 //�޳�����ȷ�Ͽ��ַ�
	char		  m_szLeaveLeague[64];				 //�뿪���˵�ȷ�Ͽ��ַ�
	char		  m_szDismissLeague[64];			 //��ɢ���˵�ȷ�Ͽ��ַ�
	char          m_szJustice[16], m_szBalance[16];  //�����е�������
	char          m_szEvil[16];                      //а��������
	char          m_szConfirm[16], m_szCancel[16];   //ȷ�ϴ����ϣ�ȷ�Ϻ�ȡ��������

	char          m_szPassword[32];                  //MD5����֮������봮
	char          m_szTargetPlayerName[32];          //����������ҵ�����

	unsigned int  m_uStatus;                         //������ĸ���״̬(Bit Value)
	                                                 //��Ӧ�� TONG_MEMBER_STATUS_ ϵ��define
};

unsigned long StringToHash(const char *pString, BOOL bIsCaseSensitive = TRUE);

#endif // !defined(AFX_UITONGMANAGER_H__13BA213D_11EC_4F24_BF98_F51C3F414D6D__INCLUDED_)
