/*******************************************************************************
File        : UiTrembleItem.cpp
Creator     : Fyt(Fan Zhanpeng)
create data : 09-08-2003(mm-dd-yyyy)
Description : ���������Ľ���
********************************************************************************/

#include "KWin32.h"
#include "KIniFile.h"
#include "CoreShell.h"
#include "GameDataDef.h"

#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"

#include "../UiBase.h"
#include "../UiSoundSetting.h"

#include "UiTrembleItem.h"
#include "UiItem.h"
#include "UiSysMsgCentre.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////

#define TREMBLE_ITEM_INI "������Ʒ.ini"

extern iCoreShell*		g_pCoreShell;

KUiTrembleItem* KUiTrembleItem::ms_pSelf = NULL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KUiTrembleItem::KUiTrembleItem()
{
	m_nStatusHandle = 0;
	m_nStatus = STATUS_WAITING_MATERIALS;
}

KUiTrembleItem::~KUiTrembleItem()
{

}


/*********************************************************************
* ���ܣ��򿪴���
**********************************************************************/
KUiTrembleItem* KUiTrembleItem::OpenWindow()
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiTrembleItem;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		g_UiBase.SetStatus(UIS_S_BESET, ms_pSelf->m_nStatusHandle);
		ms_pSelf->m_nStatus = STATUS_WAITING_MATERIALS;
		ms_pSelf->BringToTop();
		ms_pSelf->Show();
		if (KUiItem::GetIfVisible() == NULL)
			KUiItem::OpenWindow();
		else
			UiSoundPlay(UI_SI_WND_OPENCLOSE);
		if(g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_IS_GO_SERVER_PICK_DROP, 1, 0);
		}
	}
	return ms_pSelf;
}


/*********************************************************************
* ���ܣ��������������ʾ���򷵻�ʵ��ָ��
**********************************************************************/
KUiTrembleItem* KUiTrembleItem::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}


/*********************************************************************
* ���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
**********************************************************************/
void KUiTrembleItem::CloseWindow(bool bDestory)
{
	if (ms_pSelf)
	{
		ms_pSelf->Hide();
		if(g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_IS_GO_SERVER_PICK_DROP, 0, 0);
			g_pCoreShell->OperationRequest(GOI_BESET_FINISH, 0, 0);
		}
		g_UiBase.ReleaseStatus(ms_pSelf->m_nStatusHandle);
		if (bDestory)
		{
			ms_pSelf->Destroy();
			ms_pSelf = NULL;
		}
	}
}


/*********************************************************************
* ���ܣ���ʼ��
**********************************************************************/
void KUiTrembleItem::Initialize()
{
	for(int i = 0;i < UITP_NUM;i++)
	{
		AddChild(m_Item + i);
	}
	AddChild(&m_Confirm);
	AddChild(&m_Cancel);
	AddChild(&m_TrembleEffect);
	m_TrembleEffect.Hide();

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}


/*********************************************************************
* ���ܣ�������淽��
**********************************************************************/
void KUiTrembleItem::LoadScheme(const char* pScheme)
{
	if(ms_pSelf)
	{
		char		Buff[128];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, TREMBLE_ITEM_INI);
    	if (Ini.Load(Buff))
    	{
			ms_pSelf->Init(&Ini, "Main");

			ms_pSelf->m_Item[UITP_ITEM].Init(&Ini, "Item");
			ms_pSelf->m_Item[UITP_GOLD].Init(&Ini, "GemGold");
			ms_pSelf->m_Item[UITP_WOOD].Init(&Ini, "GemWood");
			ms_pSelf->m_Item[UITP_WATER].Init(&Ini, "GemWater");
			ms_pSelf->m_Item[UITP_FIRE].Init(&Ini, "GemFire");
			ms_pSelf->m_Item[UITP_EARTH].Init(&Ini, "GemEarth");
			ms_pSelf->m_Item[UITP_LEVEL].Init(&Ini, "GemLevel");
			ms_pSelf->m_Item[UITP_SPIRIT].Init(&Ini, "GemSpirit");

			ms_pSelf->m_TrembleEffect.Init(&Ini, "Effect");

			Ini.GetString("Error", "NoGem", "���������һ�ű�ʯ��", ms_pSelf->m_szErrorNoGem, sizeof(ms_pSelf->m_szErrorNoGem));
			Ini.GetString("Error", "NoItem", "���������Ƕ��ʯ����Ʒ��", ms_pSelf->m_szErrorNoItem, sizeof(ms_pSelf->m_szErrorNoItem));
			Ini.GetString("Error", "OnlyOne", "���б�ʯֻ���Է�һ�š�", ms_pSelf->m_szErrorOnlyOne, sizeof(ms_pSelf->m_szErrorOnlyOne));

			ms_pSelf->m_Confirm.Init(&Ini, "Assemble");
			ms_pSelf->m_Cancel.Init(&Ini, "Close");
		}
	}
}


/*********************************************************************
* ���ܣ�������淽��
**********************************************************************/
int KUiTrembleItem::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_ITEM_PICKDROP:
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM))
			OnPickDropItem((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;

	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_Confirm)
		{
			if(CheckValid(NULL))
			{
				Lock(TRUE);
				m_nStatus = STATUS_BEGIN_TREMBLE;
			}
		}
		else if(uParam == (unsigned int)&m_Cancel)
		{
			CloseWindow();
		}
		break;

	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 1;
}


/*********************************************************************
* ���ܣ������
**********************************************************************/
void KUiTrembleItem::Breathe()
{
	if(m_nStatus == STATUS_BEGIN_TREMBLE)
	{
		m_TrembleEffect.Show();
		m_TrembleEffect.SetFrame(0);
		m_nStatus = STATUS_TREMBLING;
	}
	else if(m_nStatus == STATUS_TREMBLING)
	{
		if(!PlayEffect())
		{
			m_nStatus = STATUS_CHANGING_ITEM;
			m_TrembleEffect.Hide();
			OnConfirm();
		}
	}
	else if(m_nStatus == STATUS_CHANGING_ITEM)
	{
		if(UpdateResult() == 1)
		{
		    m_nStatus = STATUS_FINISH;
		}
	}
}


/*********************************************************************
* ���ܣ�������Ч
**********************************************************************/
int KUiTrembleItem::PlayEffect()
{
	if(m_TrembleEffect.GetMaxFrame() == 0 ||
	   m_TrembleEffect.GetMaxFrame() <= m_TrembleEffect.GetCurrentFrame() + 1)
	{
		m_TrembleEffect.SetFrame(0);
		return 0;
	}
	else
	{
		m_TrembleEffect.NextFrame();
		return 1;
	}
}


/*********************************************************************
* ���ܣ��ѽ�����µ�������
**********************************************************************/
int KUiTrembleItem::UpdateResult()
{
	return 0;
}


/*********************************************************************
* ���ܣ���Ӧ��ʯ���������
**********************************************************************/
void KUiTrembleItem::OnPickDropItem(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Drop, Pick;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	if(pDropPos && CheckIsOtherGemAlreadyPut(pDropPos->pWnd))
	{
		KSystemMessage	sMsg;
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		strcpy(sMsg.szMessage, m_szErrorOnlyOne);
		KUiSysMsgCentre::AMessageArrival(&sMsg, NULL);
		return;
	}
	if (pPickPos)
	{
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = 0;
		Pick.eContainer = UOC_SMITH;
		pWnd = pPickPos->pWnd;
	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;

	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = 0;
		Drop.eContainer = UOC_SMITH;
	}

	for(int i = 0;i < UITP_NUM;i++)
	{
		if(pWnd == (KWndWindow *)(m_Item + i))
			Drop.Region.v = Pick.Region.v = i;
	}

	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}


int KCanGetNumImage::GetMaxFrame()
{
	return m_Image.nNumFrames;
}


int KCanGetNumImage::GetCurrentFrame()
{
	return m_Image.nFrame;
}


/*********************************************************************
* ���ܣ����½�����ʾ
**********************************************************************/
void KUiTrembleItem::UpdateView(KUiObjAtRegion *pItem, int nbAdd)
{
	if(pItem->Region.v >= 0 && pItem->Region.v < UITP_NUM)
	{
		if(nbAdd)
		{
			m_Item[pItem->Region.v].HoldObject(pItem->Obj.uGenre, pItem->Obj.uId,
							pItem->Region.Width, pItem->Region.Height);
		}
		else
		{
			m_Item[pItem->Region.v].HoldObject(CGOG_NOTHING, 0, 0, 0);
		}
	}
}


/*********************************************************************
* ���ܣ���Ӧȷ���Ĳ���
**********************************************************************/
void KUiTrembleItem::OnConfirm()
{
	KUiBesetOperationParam Param = {-1, -1, -1, -1, -1};

	if(CheckValid(&Param))
	{
		g_pCoreShell->OperationRequest(GOI_BESET_BEGIN, (unsigned int)&Param, 0);
	}
	return;
}


/*********************************************************************
* ���ܣ��������ݺϷ��Լ��
**********************************************************************/
int KUiTrembleItem::CheckValid(KUiBesetOperationParam *pParam)
{
	KUiDraggedObject Obj;
	KUiBesetOperationParam Param = {-1, -1, -1, -1, -1};

	for(int i = UITP_GOLD;i <= UITP_EARTH;i++)
	{
		if(m_Item[i].GetObject(Obj))
		{
			Param.nWuxingGemType = i;
			Param.nWuxingGemIndex   = Obj.uId;
		}
	}
	if(m_Item[UITP_ITEM].GetObject(Obj))
	{
		Param.nItemIndex = Obj.uId;
	}
	if(m_Item[UITP_LEVEL].GetObject(Obj))
	{
		Param.nLevelGemIndex = Obj.uId;
	}
	if(m_Item[UITP_SPIRIT].GetObject(Obj))
	{
		Param.nSpiritGemIndex = Obj.uId;
	}
	if(Param.nItemIndex != -1 && (Param.nLevelGemIndex != -1 || Param.nSpiritGemIndex != -1 ||
		                          Param.nWuxingGemIndex != -1))
	{
		if(pParam)
		{
			*pParam = Param;
		}
		return 1;
	}
	KSystemMessage	sMsg;
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	if(Param.nItemIndex == -1)
	{
		strcpy(sMsg.szMessage, m_szErrorNoItem);
	}
	else
	{
		strcpy(sMsg.szMessage, m_szErrorNoGem);
	}
	KUiSysMsgCentre::AMessageArrival(&sMsg, NULL);
	return 0;
}


/*********************************************************************
* ���ܣ��������λ���Ƿ��Ѿ����ñ�ʯ
**********************************************************************/
int	KUiTrembleItem::CheckIsOtherGemAlreadyPut(KWndWindow *pWhere)
{
	KWndWindow *pGem = NULL;
	KUiDraggedObject Obj;

	if(pWhere >= &m_Item[UITP_GOLD] && pWhere <= &m_Item[UITP_EARTH])
	{
		for(int i = UITP_GOLD;i <= UITP_EARTH;i++)
		{
			if(m_Item[i].GetObject(Obj) && pWhere != (KWndWindow *)(m_Item + i))
			{
				return 1;
			}
		}
	}
	return 0;
}

/*********************************************************************
* ���ܣ��������λ���Ƿ��Ѿ����ñ�ʯ
**********************************************************************/
void KUiTrembleItem::Lock(BOOL bLock)
{
	for(int i = 0;i < UITP_NUM;i++)
	{
		m_Item[i].EnablePickPut(!bLock);
	}
	m_Confirm.Enable(!bLock);
	m_Cancel.Enable(!bLock);
}


void KUiTrembleItem::Clear()
{
	for(int i = 0;i < UITP_NUM;i++)
	{
		m_Item[i].Celar();
	}
	m_Confirm.Enable(TRUE);
	m_Cancel.Enable(TRUE);
	Lock(FALSE);
}
