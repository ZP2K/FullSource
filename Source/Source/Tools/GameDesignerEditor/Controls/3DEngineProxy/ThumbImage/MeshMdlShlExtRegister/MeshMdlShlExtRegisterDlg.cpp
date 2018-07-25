// MeshMdlShlExtRegisterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MeshMdlShlExtRegister.h"
#include "MeshMdlShlExtRegisterDlg.h"
#include <comutil.h>
#include <string.h>
#include <Psapi.h>
#include "Init3dEngine.h"

#define USE_DIRECTLY

#define KG_PROCESS_ERROR(Condition) do { if (!(Condition)) goto Exit0;}while(false)

#define REG_DOT_MESH ".mesh"

#define REG_MAX_PAGE_SIZE "MaxPageSize"
#define REG_MAX_WORKSET_SIZE "MaxWorkSetSize"

#define TIMER_REFRESH 1
#define TIMER_AFTER_START_MINIMIZE 2

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL QueryRegKeyValue(HKEY hkeyMain, LPCSTR lpSubKeyName, LPCSTR lpValueName, LPSTR lpReturn);

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMeshMdlShlExtRegisterDlg dialog




CMeshMdlShlExtRegisterDlg::CMeshMdlShlExtRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeshMdlShlExtRegisterDlg::IDD, pParent)
	, DllPath(_T(""))
	, EngineRootPath(_T(""))
	, strMaxWorkSetSize(_T("100M"))
	, strMaxPageSize(_T("100M"))
	, EngineAppPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	char path[256] = "\0";
	GetModuleFileName(NULL, path,  256);
	
	CString strModulePath = _T(path);
	int nPos = strModulePath.ReverseFind('\\');
	strModulePath = strModulePath.Left(nPos + 1);

	
	
	DllPath = strModulePath + _T("MeshMdlExt.dll");
	EngineAppPath = strModulePath + _T("3dengine.exe");

	EngineRootPath = strModulePath;

	char szMaxWorkSetSize[256] = {0};
	if (QueryRegKeyValue(HKEY_CLASSES_ROOT, ".mesh", "MaxWorkSetSize", szMaxWorkSetSize))
	{
		strMaxWorkSetSize = _T(szMaxWorkSetSize);
	}
	char szPageSize[256] = {0};
	if (QueryRegKeyValue(HKEY_CLASSES_ROOT, ".mesh", "MaxPageSize", szPageSize))
	{
		strMaxPageSize = _T(szPageSize);
	}

}

void CMeshMdlShlExtRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_LIST1, EngineMemoryInfoList);
	DDX_Text(pDX, IDC_EDIT3, strMaxWorkSetSize);
	DDX_Text(pDX, IDC_EDIT4, strMaxPageSize);
	DDX_Control(pDX, IDC_SLIDER1, sliderMaxMemory);
	DDX_Control(pDX, IDC_SLIDER2, sliderMaxPageSize);
}

BEGIN_MESSAGE_MAP(CMeshMdlShlExtRegisterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL(IDC_SLIDER_MaxMemory, &CMeshMdlShlExtRegisterDlg::OnHScroll)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CMeshMdlShlExtRegisterDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_SET_SIZE, &CMeshMdlShlExtRegisterDlg::OnBnClickedButtonSetSize)
	
	ON_MESSAGE(WM_SHOWTASK,onShowTask)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE, &CMeshMdlShlExtRegisterDlg::OnBnClickedButtonEnable)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE, &CMeshMdlShlExtRegisterDlg::OnBnClickedButtonDisable)
	ON_COMMAND(ID_QUIT, &CMeshMdlShlExtRegisterDlg::OnQuit)
	ON_COMMAND(ID_RETURN_BACK, &CMeshMdlShlExtRegisterDlg::OnReturnBack)
END_MESSAGE_MAP()


// CMeshMdlShlExtRegisterDlg message handlers

BOOL CMeshMdlShlExtRegisterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	EngineMemoryInfoList.InsertColumn(0, "����", LVCFMT_RIGHT, 100);
	EngineMemoryInfoList.InsertColumn(1, "ʹ��������λKB��", LVCFMT_LEFT, 110);

	char TypeNames[][128] = {
		"�ڴ�ʹ��:",
		"�����ڴ��С:",
		"�߷��ڴ�ʹ��:",
		"ҳ�滺���:",
		"��ҳ�滺���:"

	};
	for (int i = 4; i >= 0; i--)
	{
		EngineMemoryInfoList.InsertItem(0, TypeNames[i]);
		
	}

	SetTimer(1, 2000, NULL);
	SetTimer(TIMER_AFTER_START_MINIMIZE, 1000, NULL);


	//����ڴ���������
	sliderMaxMemory.SetRange(50, 200, TRUE);
	sliderMaxPageSize.SetRange(50, 200, TRUE);


	int nOldMaxMemory = atoi(strMaxWorkSetSize.GetBuffer());
	int nOldMaxPageSize = atoi(strMaxPageSize.GetBuffer());

	sliderMaxMemory.SetPos(nOldMaxMemory);
	sliderMaxPageSize.SetPos(nOldMaxPageSize);

	

	//��ʼ����Ա: NOTIFYICONDATA m_notifyIconData;
	InitNID();
	
	bool bRun = TryRun();
	OnEngineStarted(bRun);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMeshMdlShlExtRegisterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == SC_CLOSE)//�رնԻ���ʱ�����Ԥ�����ܴ��ˣ��ر�֮
		{
			Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData);
			if (m_bEngineIsRunning)
			{
				OnBnClickedButtonDisable();
			}			
		}
		
		
		if (nID == SC_MINIMIZE)//��С��������
		{
			MinimizeToTray();
		}
		else
		{
			CDialog::OnSysCommand(nID, lParam);

		}
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMeshMdlShlExtRegisterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMeshMdlShlExtRegisterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CMeshMdlShlExtRegisterDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		OnBnClickedButtonRefresh();
		break;
	case TIMER_AFTER_START_MINIMIZE:
		if (m_bEngineIsRunning)
		{
			MinimizeToTray();
		}
		
		KillTimer(TIMER_AFTER_START_MINIMIZE);
		break;
	}
	CDialog::OnTimer(nIDEvent);
}




void CMeshMdlShlExtRegisterDlg::OnBnClickedButtonSetSize()
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	char szEngineRootPathKeyName[] = ".mesh";
	TCHAR szWorkSetSizeValueName[] = "MaxWorkSetSize";
	TCHAR szPageSizeValueName[] = "MaxPageSize";

	LPBYTE pbData = (LPBYTE)strMaxWorkSetSize.GetBuffer();
	DWORD dwDataLen = strMaxWorkSetSize.GetLength();

	LPBYTE pbPageSizeData = (LPBYTE)strMaxPageSize.GetBuffer();
	DWORD dwPageSizeDataLen = strMaxPageSize.GetLength();

	HKEY hKeyReturn;	

	CString strResult;


	LRESULT lresult = RegOpenKeyEx(HKEY_CLASSES_ROOT,LPCSTR(szEngineRootPathKeyName), 0, KEY_ALL_ACCESS,  &hKeyReturn);

	if (lresult == ERROR_SUCCESS)
	{
		lresult = RegSetValueEx(hKeyReturn, LPCTSTR(szWorkSetSizeValueName), NULL, REG_SZ , pbData, dwDataLen);
		if (lresult == ERROR_SUCCESS)
		{
			strResult = _T("�����ڴ����޳ɹ�\r\n");
		}
		else
		{
			strResult = _T("�����ڴ�����ʧ��\r\n");
		}
		lresult = RegSetValueEx(hKeyReturn, LPCTSTR(szPageSizeValueName), NULL, REG_SZ , pbPageSizeData, dwPageSizeDataLen);
		if (lresult == ERROR_SUCCESS)
		{
			strResult = strResult + _T("���������ڴ����޳ɹ�");
		}		
		else
		{
			strResult = strResult + _T("���������ڴ�����ʧ��");
		} 
		RegCloseKey(hKeyReturn);

	}
	else
	{
		strResult = _T("open .mesh failed!");
	}

	MessageBox(LPCTSTR(strResult));
}

BOOL QueryRegKeyValue(HKEY hkeyMain, LPCSTR lpSubKeyName, LPCSTR lpValueName, LPSTR lpReturn)
{
	BOOL bRetCode = false;
	BOOL bResult = false;

	HKEY hKeyReturn;
	DWORD dwLenReturn;
	DWORD dwTypeReturn;

	BYTE  byData[256];
	DWORD dwDataLen = 256;

	BOOL bOpenKeySucess = true;

	LRESULT lresult = RegOpenKeyEx(hkeyMain,lpSubKeyName,  0, KEY_READ,  &hKeyReturn);
	if (lresult == ERROR_SUCCESS)
	{
		lresult = RegQueryValueEx(hKeyReturn, lpValueName, NULL, &dwTypeReturn , byData, &dwDataLen);
		if (lresult == ERROR_SUCCESS)
		{
			sprintf(lpReturn, "%s", byData);
			bResult = TRUE;
		}
		RegCloseKey(hKeyReturn);
	}

	return bResult;
}

bool CMeshMdlShlExtRegisterDlg::RegisterMeshMdlDll()
{
	bool bResult = false;
	bool bRetCode = false;

	bRetCode = CallDllFunction(DllPath, "DllRegisterServer");
	KG_PROCESS_ERROR(bRetCode);
	
	bResult = true;
Exit0:
	return bResult;
}
bool CMeshMdlShlExtRegisterDlg::CallDllFunction(CString dllFilePath, LPCTSTR lpszFunctionName)
{
	bool bResult = false;
	bool bRetCode = false;

	HMODULE hMod = NULL;

	hMod = ::LoadLibrary(dllFilePath);
	KG_PROCESS_ERROR(hMod);

	typedef HRESULT (*pfnFunction)(void);
	pfnFunction theFuncion = (pfnFunction)GetProcAddress(hMod, lpszFunctionName);
	KG_PROCESS_ERROR(theFuncion);

	HRESULT hr = theFuncion();
	bRetCode = SUCCEEDED(hr);
	KG_PROCESS_ERROR(bRetCode);

	

	bResult = true;
Exit0:

	if (hMod)
	{
		FreeLibrary(hMod);
	}
	
	return bResult;

}
bool CMeshMdlShlExtRegisterDlg::UnRegisterMeshMdlDll()
{
	bool bResult = false;
	bool bRetCode = false;

	bRetCode = CallDllFunction(DllPath, "DllUnregisterServer");
	KG_PROCESS_ERROR(bRetCode);

	
	bResult = true;
Exit0:
	return bResult;

}

void CMeshMdlShlExtRegisterDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	int nMaxMemory   = sliderMaxMemory.GetPos();
	int nMaxPageSize = sliderMaxPageSize.GetPos();

	strMaxWorkSetSize.Format(_T("%dM"), nMaxMemory);
	strMaxPageSize.Format(_T("%dM"), nMaxPageSize);

	this->UpdateData(false);

}


void CMeshMdlShlExtRegisterDlg::MinimizeToTray()
{ 
	if (m_bEngineIsRunning)
	{
		strcpy(m_notifyIconData.szInfo, "��~\r\n��������");
	}
	else
	{
		strcpy(m_notifyIconData.szInfo, "Ŷ��\r\n�ر�����");
	}
	Shell_NotifyIcon(NIM_ADD,&m_notifyIconData);//�����������ͼ��   
	ShowWindow(SW_HIDE);//���������� 

}
void CMeshMdlShlExtRegisterDlg::InitNID()
{
	m_notifyIconData.cbSize=(DWORD)sizeof(NOTIFYICONDATA);   
	m_notifyIconData.hWnd=this->m_hWnd;   
	m_notifyIconData.uID=IDR_MAINFRAME;   
	m_notifyIconData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO   ;   
	m_notifyIconData.uCallbackMessage=WM_SHOWTASK;//�Զ������Ϣ����   
	m_notifyIconData.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));   
	strcpy(m_notifyIconData.szTip,"Mesh/Mdl����ͼԤ��");//��Ϣ��ʾ��Ϊ���ƻ��������ѡ�
	m_notifyIconData.uTimeout = 2000;
	strcpy(m_notifyIconData.szInfoTitle, "Mesh/MdlԤ������״̬");
	
	m_notifyIconData.dwInfoFlags = 1;

	

}

LRESULT CMeshMdlShlExtRegisterDlg::onShowTask(WPARAM wParam,LPARAM lParam)
{
	if(wParam!=IDR_MAINFRAME)   
		return   1;   
	switch(lParam)   
	{   
	case   WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�   
		{
			LPPOINT   lpoint=new   tagPOINT;   
			::GetCursorPos(lpoint);//�õ����λ��   
			CMenu   menu;   
			menu.CreatePopupMenu();//����һ������ʽ�˵�   
			//���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ���   
			//���أ��������������   
			menu.AppendMenu(MF_STRING,ID_RETURN_BACK,"��ԭ");
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING,ID_QUIT,"�˳�");

			//ȷ������ʽ�˵���λ��   
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this);   
			//��Դ����   
			HMENU   hmenu=menu.Detach();   
			menu.DestroyMenu();  
			

			delete   lpoint;   
		}   
		break;   
	case   WM_LBUTTONDBLCLK://˫������Ĵ���   
		{   
			this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶�   
			Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData);
		}   
		break;   
	}   

	
	return   0;   


}

bool CMeshMdlShlExtRegisterDlg::TryStart3dEngine()
{
	bool bResult = true;

	this->UpdateData(true);

	Init3Dengine(LPSTR(EngineAppPath.GetBuffer()));
	if (g_hwnd_Engine == NULL)
	{
		MessageBox("3d�����ʼ��ʧ��" );
		bResult = false;
	}

	return bResult;

}



bool CMeshMdlShlExtRegisterDlg::TryRun()
{
	bool bResult = false;
	
	if (TryStart3dEngine())
	{
		//���������ɹ���
		if (RegisterMeshMdlDll())
		{
			bResult = true;
		}
		else
		{
			MessageBox("ע��Ԥ�����ʧ�ܣ��������Ҳ���MeshMdlExt.dll");
		}
	}
	else//��������ʧ�ܴ���
	{
		MessageBox("��������ʧ�ܣ�\r\n��������Ϊ3dengine����KG3DEngine.dll·�����󣻻���dataĿ¼������");
	}

	
	return bResult;
}

void CMeshMdlShlExtRegisterDlg::OnEngineStarted(bool bStartedResult)
{
	GetDlgItem(IDC_BUTTON_SET_SIZE)->EnableWindow(!bStartedResult);
	GetDlgItem(IDC_BUTTON_ENABLE)->EnableWindow(!bStartedResult);
	GetDlgItem(IDC_BUTTON_DISABLE)->EnableWindow(bStartedResult);
	
	GetDlgItem(IDC_SLIDER_MaxMemory)->EnableWindow(!bStartedResult);
	GetDlgItem(IDC_SLIDER_MaxPageSize)->EnableWindow(!bStartedResult);
	

	m_bEngineIsRunning = bStartedResult;

}
void CMeshMdlShlExtRegisterDlg::OnBnClickedButtonEnable()
{
	// TODO: Add your control notification handler code here
	bool bRunResult = TryRun();
	OnEngineStarted(bRunResult);

}


void CMeshMdlShlExtRegisterDlg::OnBnClickedButtonDisable()
{
	// TODO: Add your control notification handler code here
	//��ע��
	UnRegisterMeshMdlDll();

	//ֹͣ3dengine
	StopEngineProcess();

	//�ؼ�״̬�ָ�
	OnEngineStarted(FALSE);
}


void CMeshMdlShlExtRegisterDlg::OnQuit()
{
	// TODO: Add your command handler code here
	if (m_bEngineIsRunning)
	{
		OnBnClickedButtonDisable();
	}	
	Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData);
	::PostMessage(this->m_hWnd, WM_CLOSE, 0, NULL);
}

void CMeshMdlShlExtRegisterDlg::OnReturnBack()
{
	// TODO: Add your command handler code here

	this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶�   
	Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData);

}
void CMeshMdlShlExtRegisterDlg::OnBnClickedButtonRefresh()
{
	// TODO: Add your control notification handler code here

	HWND hWnd_3dengine = GetEngineWnd();

	PROCESS_MEMORY_COUNTERS pmc = {0};

	if (hWnd_3dengine)
	{
		DWORD dwThreadId, dwProcessId;
		dwThreadId = ::GetWindowThreadProcessId(hWnd_3dengine, &dwProcessId);
		HANDLE hProcess;
		hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION , FALSE, dwProcessId);
		if (hProcess)
		{

			if (::GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
			{

			}
			::CloseHandle(hProcess);
		}

	}
	char szInfo[128];
	sprintf(szInfo, "%lu", pmc.WorkingSetSize / 1024);
	EngineMemoryInfoList.SetItemText(0, 1, szInfo);
	sprintf(szInfo, "%lu", pmc.PagefileUsage / 1024);
	EngineMemoryInfoList.SetItemText(1, 1, szInfo);
	sprintf(szInfo, "%lu", pmc.PeakWorkingSetSize / 1024);
	EngineMemoryInfoList.SetItemText(2, 1, szInfo);
	sprintf(szInfo, "%lu", pmc.QuotaPagedPoolUsage / 1024);
	EngineMemoryInfoList.SetItemText(3, 1, szInfo);
	sprintf(szInfo, "%lu", pmc.QuotaNonPagedPoolUsage / 1024);
	EngineMemoryInfoList.SetItemText(4, 1, szInfo);
}