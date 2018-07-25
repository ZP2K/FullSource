#include "Stdafx.h"
#include "UiWindowManage.h"
#include "resource.h"

namespace UIWINDOWMANAGE
{
    static KUiWindowManage WindowManager;

    KUiWindowManage::KUiWindowManage()
    {   
        m_nCurrentItemNum = -1;
        m_hWnd = NULL;
        m_hListWnd = NULL;
        m_szWndFileName[0] = 0;
        m_szSelWndName[0] = 0;
		m_szParentName[0] = 0;
        m_pWndIni = NULL;
        m_pOutIni = NULL;
        m_bSel = FALSE;
        m_bManage = FALSE;
		m_pWndIni = NULL;


    }

	void KUiWindowManage::InitMgr()
	{
		if (m_pWndIni) 
		{
			m_pWndIni->Release();
			m_pWndIni = NULL;
		}

		char szPath[MAX_PATH] = "";
		g_pfnUIBehavior(PCB_UI_GET_ROOT_PATH, (WPARAM)szPath, 0);

		strcat(szPath, "\\plugin\\UiEditor_Save.ini");
		strcpy(m_szWndFileName, szPath);
		if (g_IsFileExist(m_szWndFileName))
		{
			m_pWndIni = g_OpenIniFile(m_szWndFileName);
		}
	}

    KUiWindowManage::~KUiWindowManage()
    {
        Save();

        if (m_pWndIni) 
        {
            m_pWndIni->Release();
            m_pWndIni = NULL;
        }
	    if (m_hWnd)
	    {
		    ::DestroyWindow(m_hWnd);
		    m_hWnd = NULL;
	    }
    }

    int KUiWindowManage::Init(HWND hWnd)
    {
        int nResult  = false;
        int nRetCode = false;

        KG_PROCESS_ERROR(hWnd);
        KG_PROCESS_ERROR(m_szWndFileName[0]);
        m_hWnd = hWnd;

        RECT rcWindow;
        ::GetWindowRect(m_hWnd, &rcWindow);

        ::SetWindowPos(m_hWnd, HWND_TOP, 400, 200,
		    rcWindow.right -rcWindow.left, rcWindow.bottom - rcWindow.top,
		    SWP_SHOWWINDOW);

	    m_hListWnd = ::GetDlgItem(hWnd, IDC_WND_WND_MANAGER_LIST);
        KG_PROCESS_ERROR(m_hListWnd);
	    //�����ļ��б��������������Ϣ
	    ListView_SetExtendedListViewStyle(m_hListWnd,
		    ListView_GetExtendedListViewStyle(m_hListWnd) |	LVS_EX_FULLROWSELECT);
	    RECT	rc;
	    ::GetClientRect(m_hListWnd, &rc);
	    LVCOLUMN	lvc;
	    char		szHeaderText[128];
	    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	    lvc.cx = (rc.right - rc.left) / 2;
	    lvc.pszText = szHeaderText;
	    lvc.cchTextMax = 128;
        strcpy(szHeaderText, "������:");
	    ListView_InsertColumn(m_hListWnd, 0, &lvc);

        lvc.cx = (rc.right - rc.left)  / 2;
	    lvc.mask |= LVCF_SUBITEM;
	    lvc.iSubItem = 1;
        strcpy(szHeaderText, "��������:");
	    ListView_InsertColumn(m_hListWnd, 1, &lvc);


        //����Font�����ļ�
        if (m_pWndIni == NULL) 
        {
            m_pWndIni = g_OpenIniFile(m_szWndFileName);
        }
        KG_PROCESS_ERROR(m_pWndIni);


        char szSectionName[128] = "";
        char szNextSectionName[128] = "";
        char szParent[128] = "";
        char szType[128] = "";
        while (m_pWndIni->GetNextSection(szSectionName, szNextSectionName)) 
        {
            m_pWndIni->GetString(szNextSectionName, "._Parent", "", szParent, sizeof(szParent));
            if (!strcmp(szParent, "TopParent")) 
            {
                LV_ITEM			lvi;
		        memset(&lvi, 0, sizeof(LV_ITEM));
		        lvi.mask = LVIF_TEXT;
		        lvi.pszText		= (char*)szNextSectionName;
		        lvi.cchTextMax  = 128;
		        lvi.iItem       = ListView_GetItemCount(m_hListWnd);
		        lvi.iItem		= ListView_InsertItem(m_hListWnd, &lvi);

                m_pWndIni->GetString(szNextSectionName, "._WndType", "", szType, sizeof(szType));
                ListView_SetItemText(m_hListWnd, lvi.iItem, 1, szType);
            }

            strcpy(szSectionName, szNextSectionName);
        }

        nResult = true;
    Exit0:

	    return nResult;
    }

    void KUiWindowManage::Release()
    {
        if (m_hListWnd) 
        {
            ListView_DeleteAllItems(m_hListWnd);
            m_hListWnd = NULL;
        }
        
    }

    BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        int nResult = FALSE;

        switch(message) 
        {
        case WM_INITDIALOG:
            WindowManager.Init(hWnd);
        	break;
        case WM_CLOSE:
            WindowManager.Release();
            EndDialog(hWnd, FALSE);
            nResult = TRUE;
            break;

        case WM_NOTIFY:
            {
		        LPNMHDR pNotify = (LPNMHDR)lParam;
		        if (pNotify->hwndFrom == WindowManager.GetListHandle())
			        WindowManager.OnListNotify(pNotify);
            }
            break;
        case WM_COMMAND:
	        {
		        int nNotify = HIWORD(wParam);
		        switch(nNotify)
		        {
		        case EN_CHANGE:
			        break;
		        case 0:
                    WindowManager.OnButtonClick(LOWORD(wParam));
			        break;
		        }
	        }
            break;
        default:
            break;
        }

	    return nResult;
    }


    void KUiWindowManage::OnListNotify(LPNMHDR pNotify)
    {
	    switch(pNotify->code)
	    {
	    case LVN_ITEMACTIVATE:
	    case NM_CLICK:
		    {
			    LPNMITEMACTIVATE pActive = (LPNMITEMACTIVATE)pNotify;
			    if (pActive->iItem >= 0)
			    {
                    m_nCurrentItemNum = pActive->iItem;

                    char szName[128] = "";
                    ListView_GetItemText(m_hListWnd, m_nCurrentItemNum, 0, szName, sizeof(szName));
                    ::SetDlgItemText(m_hWnd, IDC_WND_WND_MANAGER_CURRENT_WND_NAME, szName);
			    }
		    }
		    break;
        case NM_DBLCLK:
            {
			    LPNMITEMACTIVATE pActive = (LPNMITEMACTIVATE)pNotify;
			    if (pActive->iItem >= 0)
			    {
                    m_nCurrentItemNum = pActive->iItem;

                    char szName[128] = "";
                    ListView_GetItemText(m_hListWnd, m_nCurrentItemNum, 0, szName, sizeof(szName));
                    ::SetDlgItemText(m_hWnd, IDC_WND_WND_MANAGER_CURRENT_WND_NAME, szName);

                    OnSelWnd();
			    }
            }
            break;

	    }
    }

    void KUiWindowManage::OnButtonClick(int nBtnId)
    {

	    switch(nBtnId)
	    {
            case IDC_WND_WND_MANAGER_NAME_CHANGE:
                OnChangeWndName();
                break;
            case IDC_WND_WND_MANAGER_WND_DELETE:
                OnDelWnd();
                break;
            case IDC_WND_WND_MANAGER_WND_SEL:
                OnSelWnd();
                break;

            default:
                break;
	    }

    }

    void KUiWindowManage::OnChangeWndName()
    {
        KG_PROCESS_ERROR(m_pWndIni);
        KG_PROCESS_ERROR(m_hListWnd);
        KG_PROCESS_ERROR(m_nCurrentItemNum >= 0);

        char szBuffer[128] = ""; //������
        char szOringalName[128] = ""; //ԭ��������
        ::GetDlgItemText(m_hWnd, IDC_WND_WND_MANAGER_CURRENT_WND_NAME, szBuffer, sizeof(szBuffer));
        ListView_GetItemText(m_hListWnd, m_nCurrentItemNum, 0, szOringalName, sizeof(szOringalName));

        if (!strcmp(szOringalName, szBuffer)) 
        {
            KG_PROCESS_ERROR(0);
        }

        if(IsWndExist(szBuffer))
        {
            ::MessageBox(m_hWnd, "�ô������Ѵ��ڣ�", "����", MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            char szKey[128] = "";
            char szNextKey[128] = "";
            char szkeyValue[1024] = "";
            while (m_pWndIni->GetNextKey(szOringalName, szKey, szNextKey)) 
            {
                m_pWndIni->GetString(szOringalName, szNextKey, "", szkeyValue, sizeof(szkeyValue));
                m_pWndIni->WriteString(szBuffer, szNextKey, szkeyValue);
                strcpy(szKey, szNextKey);
            }
            
            m_pWndIni->EraseSection(szOringalName);

            char szSection[128] = "";
            char szNextSection[128] = "";
            char szparent[128] = "";
            while (m_pWndIni->GetNextSection(szSection, szNextSection)) 
            {
                m_pWndIni->GetString(szNextSection, "._Parent", "", szparent, sizeof(szparent));
                if (!strcmp(szparent, szOringalName)) 
                {
                    m_pWndIni->WriteString(szNextSection, "._Parent", szBuffer);
                }
                strcpy(szSection, szNextSection);
            }

            ListView_SetItemText(m_hListWnd, m_nCurrentItemNum, 0, szBuffer);
        }

    Exit0:
        return;
    }

    int KUiWindowManage::IsWndExist(const char *pszWndName)
    {
        int nResult = FALSE;
        KG_PROCESS_ERROR(m_pWndIni);
        nResult = m_pWndIni->IsSectionExist(pszWndName);

    Exit0:
        return nResult;
    }

    void KUiWindowManage::OnDelWnd()
    {
        KG_PROCESS_ERROR(m_pWndIni);
        KG_PROCESS_ERROR(m_hListWnd);
        KG_PROCESS_ERROR(m_nCurrentItemNum >= 0);
        KG_PROCESS_ERROR(m_nCurrentItemNum < ListView_GetItemCount(m_hListWnd));

        char szSectionName[128] = "";
        ListView_GetItemText(m_hListWnd, m_nCurrentItemNum, 0, szSectionName, sizeof(szSectionName));

        m_pWndIni->EraseSection(szSectionName);

        KUiEditor::NormalizeIniData(m_pWndIni);

        ListView_DeleteItem(m_hListWnd, m_nCurrentItemNum);

        m_nCurrentItemNum = (m_nCurrentItemNum >= ListView_GetItemCount(m_hListWnd)) ? (ListView_GetItemCount(m_hListWnd) - 1) : m_nCurrentItemNum;
        ListView_SetItemState(m_hListWnd,m_nCurrentItemNum,UINT(LVIS_SELECTED | LVIS_FOCUSED ),LVIS_SELECTED | LVIS_FOCUSED);


        ListView_GetItemText(m_hListWnd, m_nCurrentItemNum, 0, szSectionName, sizeof(szSectionName));
        if (m_nCurrentItemNum < 0) 
        {
            szSectionName[0] = 0;
        }
        ::SetDlgItemText(m_hWnd, IDC_WND_WND_MANAGER_CURRENT_WND_NAME, szSectionName);
        
    Exit0:
        return;
    }

/*
    void KUiWindowManage::OnSelWnd()
    {
        KG_PROCESS_ERROR(m_hListWnd);
        KG_PROCESS_ERROR(m_pWndIni);
        KG_PROCESS_ERROR(m_pOutIni);
        KG_PROCESS_ERROR(!m_bManage);
        KG_PROCESS_ERROR(m_nCurrentItemNum >= 0);
        KG_PROCESS_ERROR(m_nCurrentItemNum < ListView_GetItemCount(m_hListWnd));
        
        char szWndName[128] = "";
        ListView_GetItemText(m_hListWnd, m_nCurrentItemNum, 0, szWndName, sizeof(szWndName));
        KG_PROCESS_ERROR(szWndName[0]);

        char szParent[128] = "";
        char bHaveTopParent = FALSE;
        char szSectonName[128] = "";
        char szNextSectonName[128] = "";
        char szKeyValue[128] = "";
        while (m_pOutIni->GetNextSection(szSectonName, szNextSectonName)) 
        {
            m_pOutIni->GetString(szNextSectonName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
            if (_tcsicmp(szKeyValue, g_GetWndTypeKey(UI_WND_FRAME)) == 0) 
            {
                strcpy(szParent, szNextSectonName);
                bHaveTopParent = TRUE;
                break;
            }   
            strcpy(szSectonName, szNextSectonName);
        }

        if (!bHaveTopParent) 
        {
            strcpy(szParent, g_GetFrameLayerName(LAYER_NORMAL));
        }

        if (bHaveTopParent) 
        {
            m_pWndIni->GetString(szWndName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
            if (_tcsicmp(szKeyValue, g_GetWndTypeKey(UI_WND_FRAME)) == 0) 
            {
                if(MessageBox(m_hWnd, "���ڱ༭���ļ�����Ψһ�Ĵ���(Frame),\n��\"��(Y)\"���������е��Ӵ��ڲ���,��\"��\"(N)��ȡ��������", 
                    "ע�⣺", MB_YESNO | MB_ICONQUESTION) == IDYES)
                {
                    KUiEditor::CopyAllChildWnd(m_pWndIni, szWndName, m_pOutIni, m_szSelWndName, szParent);
                    m_bSel = TRUE;
                }
                else
                {
                    m_bSel = FALSE;
                }
            }
            else
            {
                KUiEditor::CopyWndAndChildWnd(m_pWndIni, szWndName, m_pOutIni, m_szSelWndName, szParent);
                m_bSel = TRUE;
            }

        }
        else
        {
            m_pWndIni->GetString(szWndName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
            if (_tcsicmp(szKeyValue, g_GetWndTypeKey(UI_WND_FRAME)) == 0) 
            {
                KUiEditor::CopyWndAndChildWnd(m_pWndIni, szWndName, m_pOutIni, m_szSelWndName, szParent);
                m_bSel = TRUE;
            }
            else
            {
                MessageBox(m_hWnd, "���ڱ༭���ļ������ڴ���(Frame),\n���ȴ�������(Frame)���ٿ����ô��ڲ�����", 
                    "�������ڲ���ʧ�ܣ�", MB_OK | MB_ICONINFORMATION);
                m_bSel = FALSE;
            }
        }

        PostMessage(m_hWnd, WM_CLOSE, 0, 0);

    Exit0:
        return;

    }
	*/

	void KUiWindowManage::OnSelWnd()
	{
		char szWndName[128] = "";
		ListView_GetItemText(m_hListWnd, m_nCurrentItemNum, 0, szWndName, sizeof(szWndName));
		KG_PROCESS_ERROR(szWndName[0]);

		if (SelWindow(m_hWnd, m_pOutIni, szWndName, m_szParentName, m_szSelWndName))
			PostMessage(m_hWnd, WM_CLOSE, 0, 0);
Exit0:
		return;
	}

    int KUiWindowManage::SelWindow(HWND hWnd, IN IIniFile *pIni, IN const char *pcszName, OUT char *pszNewName)
    {
        int nResult     = false;
        int nRetCode    = false;

        KG_PROCESS_ERROR(pcszName);
        KG_PROCESS_ERROR(pcszName[0]);
        KG_PROCESS_ERROR(pszNewName);
        KG_PROCESS_ERROR(m_pWndIni);
        KG_PROCESS_ERROR(pIni);

        nRetCode = IsWndExist(pcszName);
        KG_PROCESS_ERROR(nRetCode);

        char szParent[128] = "";
        char bHaveTopParent = FALSE;
        char szSectonName[128] = "";
        char szNextSectonName[128] = "";
        char szKeyValue[128] = "";
        while (pIni->GetNextSection(szSectonName, szNextSectonName)) 
        {
            pIni->GetString(szNextSectonName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
            if (_tcsicmp(szKeyValue, g_GetWndTypeKey(UI_WND_FRAME)) == 0) 
            {
                strcpy(szParent, szNextSectonName);
                bHaveTopParent = TRUE;
                break;
            }   
            strcpy(szSectonName, szNextSectonName);
        }

        if (!bHaveTopParent) 
        {
            strcpy(szParent, g_GetFrameLayerName(LAYER_NORMAL));
        }

        if (bHaveTopParent) 
        {
            m_pWndIni->GetString(pcszName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
            if (_tcsicmp(szKeyValue, g_GetWndTypeKey(UI_WND_FRAME)) == 0) 
            {
                if(MessageBox(hWnd, "���ڱ༭���ļ�����Ψһ�Ĵ���(Frame),\n��\"��(Y)\"���������е��Ӵ��ڲ���,��\"��\"(N)��ȡ��������", 
                    "ע�⣺", MB_YESNO | MB_ICONQUESTION) == IDYES)
                {
                    KUiEditor::CopyAllChildWnd(m_pWndIni, pcszName, pIni, pszNewName, szParent);
                    m_bSel = TRUE;
                }
                else
                {
                    m_bSel = FALSE;
                }
            }
            else
            {
                KUiEditor::CopyWndAndChildWnd(m_pWndIni, pcszName, pIni, pszNewName, szParent);
                m_bSel = TRUE;
            }

        }
        else
        {
            m_pWndIni->GetString(pcszName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
            if (_tcsicmp(szKeyValue, g_GetWndTypeKey(UI_WND_FRAME)) == 0) 
            {
                KUiEditor::CopyWndAndChildWnd(m_pWndIni, pcszName, pIni, pszNewName, szParent);
                m_bSel = TRUE;
            }
            else
            {
                MessageBox(hWnd, "���ڱ༭���ļ������ڴ���(Frame),\n���ȴ�������(Frame)���ٿ����ô��ڲ�����", 
                    "�������ڲ���ʧ�ܣ�", MB_OK | MB_ICONINFORMATION);
                m_bSel = FALSE;
            }
        }

        nResult = true;
Exit0:
        return nResult;
    }

	int KUiWindowManage::SelWindow(
		HWND hWnd, IN IIniFile *pIni, IN const char *pcszName, IN const char *pcszFatherName, OUT char *pszNewName
	)
	{
		int nResult = false;
		int nType = 0;
		char szSectonName[128];
		szSectonName[0] = '\0';
		char szNextSectonName[128];
		szNextSectonName[0] = '\0';
		char szKeyValue[128];
		szKeyValue[0] = '\0';
		char szParent[128];
		szParent[0] = '\0';

		int nDestHaveFrame = false;
		int nSrcIsFrame = false;
		
		KG_PROCESS_ERROR(hWnd);
		KG_PROCESS_ERROR(pIni);
		KG_PROCESS_ERROR(pcszName);
		KG_PROCESS_ERROR(pszNewName);
		KG_PROCESS_ERROR(m_pWndIni);

		while (pIni->GetNextSection(szSectonName, szNextSectonName)) 
		{
			pIni->GetString(szNextSectonName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
			nType = g_GetWndType(szKeyValue);
			if (nType == UI_WND_FRAME) 
			{
				nDestHaveFrame = true;
				strncpy(szParent, szNextSectonName, sizeof(szParent));
				break;
			}
			strcpy(szSectonName, szNextSectonName);
		}

		m_pWndIni->GetString(pcszName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
		nType = g_GetWndType(szKeyValue);
		if (nType == UI_WND_FRAME)
			nSrcIsFrame = true;

		if (nSrcIsFrame && nDestHaveFrame)
		{
			MessageBox(hWnd, "���ڱ༭���ļ��Ѿ����ڴ���(Frame),���ܿ����ô���(Frame)��", 
				"�������ڲ���ʧ�ܣ�", MB_OK | MB_ICONINFORMATION);
			m_bSel = FALSE;
			KG_PROCESS_ERROR(false);
		}

		if ((!nSrcIsFrame) && (!nDestHaveFrame))
		{
			MessageBox(hWnd, "���ڱ༭���ļ������ڴ���(Frame),\n���ȴ�������(Frame)���ٿ����ô��ڲ�����", 
				"�������ڲ���ʧ�ܣ�", MB_OK | MB_ICONINFORMATION);
			m_bSel = FALSE;
			KG_PROCESS_ERROR(false);
		}

		if (nSrcIsFrame)
		{
			KUiEditor::CopyWndAndChildWnd(m_pWndIni, pcszName, pIni, pszNewName, "Normal");
			m_bSel = TRUE;
			KG_PROCESS_SUCCESS(true);
		}

		//û��ָ��Parent����Frame��ΪParent
		if ((!pcszFatherName) || (!pcszFatherName[0]))
			pcszFatherName = szParent;

		pIni->GetString(pcszFatherName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
		nType = g_GetWndType(szKeyValue);
		//��Handle
		if (nType == UI_ITEM_HANDLE || nType == UI_ITEM_TREE_LEAFE)
		{
			m_pWndIni->GetString(pcszName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
			nType = g_GetWndType(szKeyValue);
			if (nType >= UI_ITEM_NULL && nType <= UI_ITEM_TREE_LEAFE)
			{
				KUiEditor::CopyWndAndChildWnd(m_pWndIni, pcszName, pIni, pszNewName, pcszFatherName);
				m_bSel = TRUE;
				KG_PROCESS_SUCCESS(true);
			}
			MessageBox(hWnd, "������Handle�п���Wnd", "����ʧ�ܣ�", MB_OK | MB_ICONINFORMATION);
			m_bSel = FALSE;
			KG_PROCESS_ERROR(false);
		}

		//��Wnd
		if (nType < UI_ITEM_NULL)
		{
			m_pWndIni->GetString(pcszName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
			nType = g_GetWndType(szKeyValue);
			if (nType < UI_ITEM_NULL)
			{
				KUiEditor::CopyWndAndChildWnd(m_pWndIni, pcszName, pIni, pszNewName, pcszFatherName);
				m_bSel = TRUE;
				KG_PROCESS_SUCCESS(true);
			}

			if (nType == UI_ITEM_HANDLE)
			{
				szSectonName[0] = '\0';
				szNextSectonName[0] = '\0';
				while (pIni->GetNextSection(szSectonName, szNextSectonName)) 
				{
					pIni->GetString(szNextSectonName, "._WndType", "", szKeyValue, sizeof(szKeyValue));
					if (g_GetWndType(szKeyValue) == UI_ITEM_HANDLE) 
					{
						pIni->GetString(szNextSectonName, "._Parent", "", szKeyValue, sizeof(szKeyValue));
						if (_tcsicmp(szKeyValue, pcszFatherName) == 0)
						{
							MessageBox(hWnd, "Wnd�Ѿ�����ΨһHandle", "����ʧ�ܣ�", MB_OK | MB_ICONINFORMATION);
							m_bSel = FALSE;
							KG_PROCESS_ERROR(false);
						}	
					}
					strcpy(szSectonName, szNextSectonName);
				}
				KUiEditor::CopyWndAndChildWnd(m_pWndIni, pcszName, pIni, pszNewName, pcszFatherName);
				m_bSel = TRUE;
				KG_PROCESS_SUCCESS(true);
			}
		}

		MessageBox(hWnd, "����������ƥ���������δ֪����", 
			"����ʧ�ܣ�", MB_OK | MB_ICONINFORMATION);
		m_bSel = FALSE;
		KG_PROCESS_ERROR(false);

Exit1:
		nResult = true;
Exit0:
		return nResult;

	}
    
    void KUiWindowManage::AddWnd(const char *pszWndName, IIniFile *pIni, OUT char *pszNewName)
    {
        KG_PROCESS_ERROR(pszWndName && pszWndName[0]);
        KG_PROCESS_ERROR(pIni);
        KG_PROCESS_ERROR(m_pWndIni);

        KUiEditor::CopyWndAndChildWnd(pIni, pszWndName, m_pWndIni, pszNewName, "TopParent");

    Exit0:
        return;
    }

    void KUiWindowManage::OnWindowManage(HWND hParent, IIniFile *pOut, const char *pcszparentName, BOOL bManage)
    {
        m_nCurrentItemNum = -1;
        m_hWnd = NULL;
        m_hListWnd = NULL;
        m_szSelWndName[0] = 0;
        m_bSel = FALSE;
        m_bManage = bManage;
        m_pOutIni = pOut;

        KG_PROCESS_ERROR(hParent);

		if (pcszparentName)
			strncpy(m_szParentName, pcszparentName, sizeof(m_szParentName));
		else
			m_szParentName[0] = '\0';
        
        ::DialogBox((HINSTANCE)g_hPluginInstance, MAKEINTRESOURCE(IDD_WndWindowManage), hParent, (DLGPROC)DlgProc);

    Exit0:
        return;
    }

    void KUiWindowManage::Save()
    {
        KG_PROCESS_ERROR(m_pWndIni);
        KG_PROCESS_ERROR(m_szWndFileName && m_szWndFileName[0]);

        KUiEditor::NormalizeIniData(m_pWndIni);

        m_pWndIni->Save(m_szWndFileName);

    Exit0:
        return;

    }

    int AddWindow(const char *pszWndName, IIniFile *pIni, OUT char *pszNewName)
    {
        int nResult = FALSE;

        KG_PROCESS_ERROR(pszWndName);
        KG_PROCESS_ERROR(pIni);
        KG_PROCESS_ERROR(pszNewName);
        WindowManager.AddWnd(pszWndName, pIni, pszNewName);

        nResult = TRUE;
    Exit0:
        return nResult;
    }

    int SelWindow(HWND hWnd, IIniFile *pIni, IN const char *pszFatherName, OUT char *pszName)
    {
        int nResult = FALSE;

        KG_PROCESS_ERROR(hWnd);
        KG_PROCESS_ERROR(pIni);
        KG_PROCESS_ERROR(pszName);
        
        WindowManager.OnWindowManage(hWnd, g_Ui.GetIni(), pszFatherName, FALSE);
        if (WindowManager.IsSeled()) 
        {
            strcpy(pszName, WindowManager.GetSelWnd());
            nResult = TRUE;
        }
        else
        {
            nResult = FALSE;
        }

        
    Exit0:
        return nResult;
    }

    int WindowManage(HWND hWnd)
    {
        int nResult = FALSE;
        KG_PROCESS_ERROR(hWnd);
        
        WindowManager.OnWindowManage(hWnd, g_Ui.GetIni(), NULL, TRUE);

        nResult = TRUE;
    Exit0:
        return nResult;
    }

    void SaveWindowManageData()
    {
        WindowManager.Save();
    }

	int InitWindowManage()
	{
		WindowManager.InitMgr();
		return true;
	}

    int SelWindow(HWND hWnd, IN IIniFile *pIni, IN const char *pcszName, IN const char *pPatherName, OUT char *pszNewName)
    {
        int nResult = false;
        int nRetCode = false;
        nRetCode = WindowManager.SelWindow(hWnd, pIni, pcszName, pPatherName, pszNewName);
        KG_PROCESS_ERROR(nRetCode);

        nResult = true;
Exit0:
        return nResult;

    }

}