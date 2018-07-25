/*******************************************************************************
File        : UiCityManager.h
Creator     : Fyt(Fan Zhanpeng)
create data : 02-11-2004(mm-dd-yyyy)
Description : �����й������
********************************************************************************/

#include "KWin32.h"
#include "KIniFile.h"
#include "CoreShell.h"
#include "GameDataDef.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"

#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"

#include "../UiBase.h"
#include "../UiSoundSetting.h"

#include "UiCityManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////



#define UI_CITY_MANAGE_SCHEME  "���й������.ini"


KUiCityManager* KUiCityManager::ms_pSelf = NULL;

extern iCoreShell* g_pCoreShell;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KUiCityManager::KUiCityManager()
{

}

KUiCityManager::~KUiCityManager()
{

}


/*********************************************************************
* ���ܣ��򿪴���
**********************************************************************/
KUiCityManager* KUiCityManager::OpenWindow()
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiCityManager;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		ms_pSelf->BringToTop();
		ms_pSelf->PrepareData();
		ms_pSelf->Show();
	}
	return ms_pSelf;
}


/*********************************************************************
* ���ܣ��������������ʾ���򷵻�ʵ��ָ��
**********************************************************************/
KUiCityManager* KUiCityManager::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
	{
		return ms_pSelf;
	}
	return NULL;
}


/*********************************************************************
* ���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
**********************************************************************/
void KUiCityManager::CloseWindow(bool bDestory)
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
* ���ܣ�������淽��
**********************************************************************/
void KUiCityManager::LoadScheme(const char* pScheme)
{
	if(ms_pSelf)
	{
		char		Buff[128];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, UI_CITY_MANAGE_SCHEME);

    	if (Ini.Load(Buff))
    	{
			ms_pSelf->Init(&Ini, "Main");

			ms_pSelf->m_textMasterName.Init(&Ini, "textMasterName");
			ms_pSelf->m_textTongName.Init(&Ini, "textTongName");
			ms_pSelf->m_BtnMaxPrice.Init(&Ini, "BtnMaxPrice");
			ms_pSelf->m_editTax.Init(&Ini, "textTax");
			ms_pSelf->m_editPrice.Init(&Ini, "textPrice");
			ms_pSelf->m_BtnMinTax.Init(&Ini, "BtnMinTax");
			ms_pSelf->m_BtnMaxTax.Init(&Ini, "BtnMaxTax");
			ms_pSelf->m_BtnMinPrice.Init(&Ini, "BtnMinPrice");

			ms_pSelf->m_BtnIncTax.Init(&Ini, "BtnIncreaseTax");
			ms_pSelf->m_BtnDecTax.Init(&Ini, "BtnDecreaseTax");
			ms_pSelf->m_BtnIncPrice.Init(&Ini, "BtnIncreasePrice");
			ms_pSelf->m_BtnDecPrice.Init(&Ini, "BtnDecreasePrice");

			ms_pSelf->m_BtnClose.Init(&Ini, "BtnClose");
			ms_pSelf->m_BtnCancel.Init(&Ini, "BtnCancel");
			ms_pSelf->m_BtnConfirm.Init(&Ini, "BtnConfirm");
			ms_pSelf->m_BtnHistory.Init(&Ini, "BtnHistory");
		}
	}
}


/*********************************************************************
* ���ܣ���ʼ��
**********************************************************************/
void KUiCityManager::Initialize()
{
	AddChild(&m_textMasterName);
	AddChild(&m_textTongName);
	AddChild(&m_BtnMaxPrice);
	AddChild(&m_editTax);
	AddChild(&m_editPrice);
	AddChild(&m_BtnMinTax);
	AddChild(&m_BtnMaxTax);
	AddChild(&m_BtnMinPrice);

	AddChild(&m_BtnIncTax);
	AddChild(&m_BtnDecTax);
	AddChild(&m_BtnIncPrice);
	AddChild(&m_BtnDecPrice);

	AddChild(&m_BtnClose);
	AddChild(&m_BtnCancel);
	AddChild(&m_BtnConfirm);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}


/*********************************************************************
* ���ܣ�������Ϣ������
**********************************************************************/
int KUiCityManager::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		{
			KWndWindow *pWnd = (KWndWindow *)uParam;
			if(pWnd == &m_BtnCancel || pWnd == &m_BtnClose)
			{
				CloseWindow();
			}
			else if(pWnd == &m_BtnMaxTax)
			{
				MaxTax();
			}
			else if(pWnd == &m_BtnMinTax)
			{
				MinTax();
			}
			else if(pWnd == &m_BtnMaxPrice)
			{
				MaxPrice();
			}
			else if(pWnd == &m_BtnMinPrice)
			{
				MaxPrice();
			}
			else if(pWnd == &m_BtnIncTax)
			{
				AdjustTax(1);
			}
			else if(pWnd == &m_BtnDecTax)
			{
				AdjustTax(-1);
			}
			else if(pWnd == &m_BtnIncPrice)
			{
				AdjustPrice(1);
			}
			else if(pWnd == &m_BtnDecPrice)
			{
				AdjustPrice(-1);
			}
			else if(pWnd == &m_BtnConfirm)
			{
				if(m_nCurrentTaxRate == m_nTaxRate && m_nCurrentPriceRate == m_nPriceRate)
				{
					CloseWindow();
				}
				else
				{
					int nFlag = 1;
					if(m_nCurrentTaxRate < m_nMinTax)
					{
						MinTax();
						nFlag = 0;
					}
					if(m_nCurrentPriceRate < m_nMinPrice)
					{
						MinPrice();
						nFlag = 0;
					}
					//������������
					if(nFlag)
					{
						if(g_pCoreShell)
						{
							g_pCoreShell->OperationRequest(GOI_SET_CITY_TAX, m_nCurrentTaxRate, m_nCurrentPriceRate);
						}
					}
					CloseWindow();
				}
			}
			UpdateView();
		}
		break;

	case WND_N_BUTTON_HOLD:
		{
			KWndWindow *pWnd = (KWndWindow *)uParam;
			if(pWnd == &m_BtnIncTax)
			{
				AdjustTax(1);
			}
			else if(pWnd == &m_BtnDecTax)
			{
				AdjustTax(-1);
			}
			else if(pWnd == &m_BtnIncPrice)
			{
				AdjustPrice(1);
			}
			else if(pWnd == &m_BtnDecPrice)
			{
				AdjustPrice(-1);
			}
			UpdateView();
		}
		break;

	case WND_N_EDIT_CHANGE:
		{
			KWndWindow *pWnd = (KWndWindow *)uParam;
			if(pWnd == &m_editTax)
			{
				m_nCurrentTaxRate = m_editTax.GetIntNumber();
				if(m_nCurrentTaxRate > m_nMaxTax)
				{
					MaxTax();
					UpdateView();
				}
			}
			else if(pWnd == &m_editPrice)
			{
				m_nCurrentPriceRate = m_editPrice.GetIntNumber();
				if(m_nCurrentPriceRate > m_nMaxPrice)
				{
					MaxPrice();
					UpdateView();
				}
			}
		}
		break;

	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
		break;
	}

	return nRet;
}


/*********************************************************************
* ���ܣ�׼����������
**********************************************************************/
void KUiCityManager::PrepareData()
{
	m_nMinTax = 0;
	m_nMinPrice = 0;
	m_szTongName[0] = 0;
	m_szMasterName[0] = 0;
	m_nTaxRate = 0;
	m_nPriceRate = 0;
	m_nCurrentTaxRate = 0;
	m_nCurrentPriceRate = 0;
	if(g_pCoreShell)
	{
		KTongOperationParam sParam;
		g_pCoreShell->GetGameData(GDI_CITY_MANAGER_PARAM, (unsigned int)&sParam, 0);
		UpdateCurrentCityParam(sParam.nData[0], sParam.nData[1], sParam.nData[2], sParam.nData[3]);
	}
}


/*********************************************************************
* ���ܣ�������ʾ
**********************************************************************/
void KUiCityManager::UpdateView()
{
	m_textTongName.SetText(m_szTongName);
	m_textMasterName.SetText(m_szMasterName);
	m_editTax.SetIntText(m_nCurrentTaxRate);
	m_editPrice.SetIntText(m_nCurrentPriceRate);
}


/*********************************************************************
* ���ܣ���˰����Ϊ��Сֵ
**********************************************************************/
void KUiCityManager::MinTax()
{
	m_nCurrentTaxRate = m_nMinTax;
}


/*********************************************************************
* ���ܣ���˰����Ϊ���ֵ
**********************************************************************/
void KUiCityManager::MaxTax()
{
	m_nCurrentTaxRate = m_nMaxTax;
}


/*********************************************************************
* ���ܣ������ָ����Ϊ��Сֵ
**********************************************************************/
void KUiCityManager::MinPrice()
{
	m_nCurrentPriceRate = m_nMinPrice;
}


/*********************************************************************
* ���ܣ������ָ����Ϊ���ֵ
**********************************************************************/
void KUiCityManager::MaxPrice()
{
	m_nCurrentPriceRate = m_nMaxPrice;
}


/*********************************************************************
* ���ܣ����³��е���������
**********************************************************************/
HRESULT KUiCityManager::UpdateCurrentCityData(char *pszTongName, char *pszMasterName, int nTaxRate, int nPriceRate)
{
	if(pszTongName && pszTongName[0])
	{
		strncpy(m_szTongName, pszTongName, sizeof(m_szTongName));
	}
	else
	{
		m_szTongName[0] = 0;
	}
	if(pszMasterName && pszMasterName[0])
	{
		strncpy(m_szMasterName, pszMasterName, sizeof(m_szMasterName));
	}
	else
	{
		m_szMasterName[0] = 0;
	}
	m_nTaxRate = m_nCurrentTaxRate = nTaxRate;
	m_nPriceRate = m_nCurrentPriceRate = nPriceRate;

	UpdateView();

	return 1;
}


/*********************************************************************
* ���ܣ����³��е����ò���
**********************************************************************/
HRESULT KUiCityManager::UpdateCurrentCityParam(int nMaxTax, int nMinTax, int nMaxPrice, int nMinPrice)
{
	m_nMaxTax = nMaxTax;
	m_nMinTax = nMinTax;
	m_nMaxPrice = nMaxPrice;
	m_nMinPrice = nMinPrice;

	return 1;
}


/*********************************************************************
* ���ܣ��������ָ����ֵ
**********************************************************************/
void KUiCityManager::AdjustPrice(int nCount)
{
	int nTotal = m_nCurrentPriceRate + nCount;

	if(nTotal > m_nMaxPrice)
	{
		nTotal = m_nMaxPrice;
	}
	else if(nTotal < m_nMinPrice)
	{
		nTotal = m_nMinPrice;
	}
	m_nCurrentPriceRate = nTotal;
}


/*********************************************************************
* ���ܣ�����˰�ʵ�ֵ
**********************************************************************/
void KUiCityManager::AdjustTax(int nCount)
{
	int nTotal = m_nCurrentTaxRate + nCount;

	if(nTotal > m_nMaxTax)
	{
		nTotal = m_nMaxTax;
	}
	else if(nTotal < m_nMinTax)
	{
		nTotal = m_nMinTax;
	}
	m_nCurrentTaxRate = nTotal;
}


/*********************************************************************
* ���ܣ����ܽ������õ�UI��������
**********************************************************************/
void KUiCityManager::TheCANNOTCHANGE_UI()
{
	m_BtnMaxTax.Enable(FALSE);
	m_BtnMinTax.Enable(FALSE);
	m_BtnMaxPrice.Enable(FALSE);
	m_BtnMinPrice.Enable(FALSE);
	m_BtnIncPrice.Enable(FALSE);
	m_BtnDecPrice.Enable(FALSE);
	m_BtnIncTax.Enable(FALSE);
	m_BtnDecTax.Enable(FALSE);
	m_BtnClose.Show();
	m_BtnCancel.Hide();
	m_BtnConfirm.Hide();
}


/*********************************************************************
* ���ܣ����Խ������õ�UI��������
**********************************************************************/
void KUiCityManager::TheCANCHANGE_UI()
{
	m_BtnMaxTax.Enable(TRUE);
	m_BtnMinTax.Enable(TRUE);
	m_BtnMaxPrice.Enable(TRUE);
	m_BtnMinPrice.Enable(TRUE);
	m_BtnIncPrice.Enable(TRUE);
	m_BtnDecPrice.Enable(TRUE);
	m_BtnIncTax.Enable(TRUE);
	m_BtnDecTax.Enable(TRUE);

	m_BtnClose.Hide();
	m_BtnCancel.Show();
	m_BtnConfirm.Show();
}
