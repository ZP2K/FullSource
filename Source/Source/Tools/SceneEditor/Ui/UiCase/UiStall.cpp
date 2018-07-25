/*
 * File:     UiStall.h
 * Desc:     �໰�̯ing
 * Author:   flying
 * Creation: 2003/10/23
 */
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "uitrade.h"
#include "UiStall.h"
#include "UiStallMarkPrice.h"
#include "UiStoreBox.h"
#include "UiTradeConfirmWnd.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiBase.h"
#include "../ShortcutKey.h"
#include "../UiSoundSetting.h"
#include "KPlayerStall.h"
#include "UiItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI_ITEM		"��̯��Ʒ.ini"

KUiStall* KUiStall::m_pSelf = NULL;

enum WAIT_OTHER_WND_OPER_PARAM
{
	UIITEM_WAIT_GETMONEY,
};

KUiStall::KUiStall()
{
	m_nStatusHandle = 0;
}

//--------------------------------------------------------------------------
//	���ܣ��������������ʾ���򷵻�ʵ��ָ��
//--------------------------------------------------------------------------
KUiStall* KUiStall::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiStall* KUiStall::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiStall;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	g_UiBase.SetStatus(UIS_S_STALL_BUY, m_pSelf->m_nStatusHandle);
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
//--------------------------------------------------------------------------
void KUiStall::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		//�˳���̯״̬
		g_UiBase.ReleaseStatus(m_pSelf->m_nStatusHandle);

		if (g_pCoreShell->GetGameData(GDI_IS_STALL, -1, -1) == 2)
		{
			g_pCoreShell->OperationRequest(GOI_SWITCH_STALL_BUY, 0, 0);

		}
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		if (KUiItem::GetIfVisible())
			KUiItem::CloseWindow(false);
	}
}

// -------------------------------------------------------------------------
// ����	: ��ʼ��
// -------------------------------------------------------------------------
void KUiStall::Initialize()
{
	AddChild(&m_Owner);
	AddChild(&m_CloseBtn);
	AddChild(&m_ItemBox);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	m_ItemBox.SetContainerId((int)UOC_STALL_OTHER);
	Wnd_AddWindow(this);
}

//�����
void KUiStall::Breathe()
{
}

//--------------------------------------------------------------------------
//	���ܣ����캯��
//--------------------------------------------------------------------------
void KUiStall::UpdateData()
{
	char szStallOwnerName[32];
	g_pCoreShell->GetGameData(GDI_STALL_OWNER_NAME, (unsigned int)szStallOwnerName, 0);
	m_Owner.SetText(szStallOwnerName);
	m_ItemBox.Clear();
	StallViewItemInfo *pSellerStallItem = NULL;
	//ȡ������̯����Ʒ����
	int nCount = g_pCoreShell->GetGameData(GDI_ITEM_IN_STALL_OTHER, 0, 0);
	//�����Ʒ����Ϊ0, �˳�����״̬,����
	if (nCount == 0)
	{
		//�رմ���,����
		CloseWindow(true);
		return;
	}

	if (pSellerStallItem = (StallViewItemInfo*)malloc(sizeof(StallViewItemInfo) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_IN_STALL_OTHER, (unsigned int)pSellerStallItem, nCount);//���߳�ִ�У�nCountֵ����
		for (int i = 0; i < nCount; i++)
		{
			KUiDraggedObject no;
			no.uGenre = CGOG_ITEM;//pSellerStallItem[i].m_btGenre;
			no.uId = pSellerStallItem[i].m_nID;
			no.DataX = pSellerStallItem[i].nX;
			no.DataY = pSellerStallItem[i].nY;
			no.DataW = pSellerStallItem[i].nWidth;
			no.DataH = pSellerStallItem[i].nHeight;
			m_ItemBox.AddObject(&no, 1);
		}
		free(pSellerStallItem);
		pSellerStallItem = NULL;
	}
}

// -------------------------------------------------------------------------
// ����	: ��Ʒ�仯����
// -------------------------------------------------------------------------
void KUiStall::RemoveItem(KUiObjAtRegion* pItem)
{
	if (pItem)
	{
		KUiDraggedObject Obj;
		Obj.uGenre = pItem->Obj.uGenre;
		Obj.uId = pItem->Obj.uId;
		Obj.DataX = pItem->Region.h;
		Obj.DataY = pItem->Region.v;
		Obj.DataW = pItem->Region.Width;
		Obj.DataH = pItem->Region.Height;
		m_ItemBox.RemoveObject(&Obj);
		UiSoundPlay(UI_SI_PICKPUT_ITEM);
	}
	else
		UpdateData();
}

// -------------------------------------------------------------------------
// ����	: ������淽��
// -------------------------------------------------------------------------
void KUiStall::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_ITEM);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");
		m_pSelf->m_ItemBox.Init(&Ini, "ItemBox");
		m_pSelf->m_Owner.Init(&Ini, "Owner");

		m_pSelf->m_ItemBox.EnableTracePutPos(false);
		m_pSelf->m_ItemBox.EnablePickPut(false);
	}
}

void KUiStall::OnClickItem(KUiDraggedObject* pItem)
{
	if (pItem == NULL || g_pCoreShell == NULL)
		return;
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre = pItem->uGenre;
	Obj.Obj.uId = pItem->uId;
	Obj.Region.h = pItem->DataX;
	Obj.Region.v = pItem->DataY;
	Obj.Region.Width  = pItem->DataW;
	Obj.Region.Height = pItem->DataH;
	Obj.eContainer = UOC_STALL_OTHER;

	StallViewItemInfo *pSellerStallItem = NULL;
	//ȡ������̯����Ʒ����
	int nCount = g_pCoreShell->GetGameData(GDI_ITEM_IN_STALL_OTHER, 0, 0);
	//�����Ʒ����Ϊ0, �˳�����״̬,����
	if (pSellerStallItem = (StallViewItemInfo*)malloc(sizeof(StallViewItemInfo) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_IN_STALL_OTHER, (unsigned int)pSellerStallItem, nCount);//���߳�ִ�У�nCountֵ����
		for (int i = 0; i < nCount; i++)
		{
			if (Obj.Obj.uId == pSellerStallItem[i].m_nID)
			{
				KUiItemBuySelInfo info;
				g_pCoreShell->GetGameData(GDI_TRADE_ITEM_PRICE, (unsigned int)&Obj, (int)&info);
				info.nPrice = pSellerStallItem[i].nPrice;
				KUiTradeConfirm::OpenWindow(&Obj, &info, TCA_BUY);
			}
		}
		free(pSellerStallItem);
		pSellerStallItem = NULL;
	}
}

// -------------------------------------------------------------------------
// ����	: ���ں���
// -------------------------------------------------------------------------
int KUiStall::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_LEFT_CLICK_ITEM:
			OnClickItem((KUiDraggedObject*)uParam);
		break;
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
			CloseWindow(true);
		break;
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

