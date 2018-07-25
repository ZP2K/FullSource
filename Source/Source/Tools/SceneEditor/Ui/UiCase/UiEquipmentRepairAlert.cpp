/*******************************************************************************
File        : UiEquipmentRepairAlert.cpp
Creator     : Fyt(Fan Zhanpeng)
create data : 10-15-2003(mm-dd-yyyy)
Description : ��Ʒ������
********************************************************************************/

#include "KWin32.h"
#include "KIniFile.h"
#include "CoreShell.h"
#include "GameDataDef.h"

#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"
#include "../elem/mousehover.h"

#include "../UiBase.h"
#include "../UiSoundSetting.h"

#include "UiEquipmentRepairAlert.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////

#define REPAIR_ITEM_ALERT_INI "������.ini"

KUiEquipmentRepairAlert* KUiEquipmentRepairAlert::ms_pSelf = NULL;

extern iCoreShell*		g_pCoreShell;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KUiEquipmentRepairAlert::KUiEquipmentRepairAlert()
{

}

KUiEquipmentRepairAlert::~KUiEquipmentRepairAlert()
{

}


/*********************************************************************
* ���ܣ��򿪴���
**********************************************************************/
KUiEquipmentRepairAlert* KUiEquipmentRepairAlert::OpenWindow()
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiEquipmentRepairAlert;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		ms_pSelf->Hide();
		ms_pSelf->BringToTop();
	}
	return ms_pSelf;
}


/*********************************************************************
* ���ܣ��������������ʾ���򷵻�ʵ��ָ��
**********************************************************************/
KUiEquipmentRepairAlert* KUiEquipmentRepairAlert::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}


/*********************************************************************
* ���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
**********************************************************************/
void KUiEquipmentRepairAlert::CloseWindow(bool bDestory)
{
	if (ms_pSelf)
	{
		ms_pSelf->Hide();
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
void KUiEquipmentRepairAlert::Initialize()
{
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this, WL_TOPMOST);
}


/*********************************************************************
* ���ܣ����ں���
**********************************************************************/
int KUiEquipmentRepairAlert::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSEHOVER:
		g_MouseOver.SetMouseHoverInfo((void*)(KWndWindow*)this, 0,
				GET_X_LPARAM(nParam), GET_Y_LPARAM(nParam), false, FALSE);
		g_MouseOver.SetMouseHoverTitle(m_szTip, strlen(m_szTip), 0xffffffff);
		nRet = 1;
		break;

	default:
		Wnd_TransmitInputToGameSpace(uMsg, uParam, nParam);
	}
	return nRet;
}


/*********************************************************************
* ���ܣ�������淽��
**********************************************************************/
void KUiEquipmentRepairAlert::LoadScheme(const char* pScheme)
{
	if(ms_pSelf)
	{
		char		Buff[128];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, REPAIR_ITEM_ALERT_INI);
    	if (Ini.Load(Buff))
    	{
			ms_pSelf->Init(&Ini, "Main");
			Ini.GetInteger("Main", "Yellow", 3, &ms_pSelf->m_nYellowAlertSignal);
			Ini.GetInteger("Main", "Red", 1, &ms_pSelf->m_nRedAlertSignal);
		}
	}
}


/*********************************************************************
* ���ܣ���ʾ����
**********************************************************************/
void KUiEquipmentRepairAlert::Show(int nDur, char *szName)
{
    if(nDur > m_nYellowAlertSignal || nDur == -1)
	    Hide();
	else
	{
		KWndImage::Show();
		if(nDur > m_nRedAlertSignal)
		{
			SetTargetItemTip(szName);
    	    SetFrame(0);
		}
		else
		{
			SetTargetItemTip(szName, TRUE);
    		SetFrame(1);
		}
	}
}


/*********************************************************************
* ���ܣ���������Ѿ����������򷵻ض���ʵ�������򷵻�NULL
**********************************************************************/
KUiEquipmentRepairAlert* KUiEquipmentRepairAlert::GetIfCreate()
{
	if(ms_pSelf)
		return ms_pSelf;
	else
		return NULL;
}


/*********************************************************************
* ���ܣ�����Ҫ������Ǹ�װ����index
**********************************************************************/
int	KUiEquipmentRepairAlert::SetTargetItemTip(char *szName, int nbIsAlmostCrash)
{
	if(g_pCoreShell)
	{
		if(nbIsAlmostCrash)
		{
			sprintf(m_szTip, "��� %s ���������Ͼ�Ҫ���ˡ�", szName);
		}
		else
		{
			sprintf(m_szTip, "��е���� %s ����һЩ�Ѻۡ�", szName);
		}
		return 1;
	}
	return 0;
}
