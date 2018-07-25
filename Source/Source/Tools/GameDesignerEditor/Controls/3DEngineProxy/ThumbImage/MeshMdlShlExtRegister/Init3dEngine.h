//����Ĵ��ھ��
HWND g_hwnd_Engine = NULL;

//���������Ϣ
PROCESS_INFORMATION g_Engine_Process_Information = {0};

//��ʼ��3dengine
void Init3Dengine(LPSTR lpEngineAppName);

//�����������
BOOL StartEngineProcess(LPSTR lpEngineAppName);

//ֹͣ�������
BOOL StopEngineProcess();

//�ж���������Ƿ����
BOOL IsEngineProcessExist(HANDLE);

//��ȡ�������Ĵ��ھ��
HWND GetEngineWnd();

//ö�ٴ��ڵĻص�����
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

//���洰������
#define EngineWindowName "_EngineWindowName_For_Mesh_"

#define WM_REINIT_ENGINE		WM_USER + 104

void Init3Dengine(LPSTR lpEngineAppName)
{
	g_hwnd_Engine = GetEngineWnd();
	
	if (g_hwnd_Engine == NULL)
	{	
		if (StartEngineProcess(lpEngineAppName))
		{
			//�ȴ������ʼ�����(g_hwnd_Engine != NULL)
			while (IsEngineProcessExist(g_Engine_Process_Information.hProcess) && g_hwnd_Engine == NULL)
			{
				g_hwnd_Engine = GetEngineWnd();

				::SwitchToThread();
			}
			::CloseHandle(g_Engine_Process_Information.hProcess);
			::CloseHandle(g_Engine_Process_Information.hThread);
		}
	}
}


BOOL StartEngineProcess(LPSTR lpEngineAppName)
{
	BOOL bResult = TRUE;

	STARTUPINFO si = {sizeof(si)};

	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW;

	ZeroMemory( &g_Engine_Process_Information, sizeof(g_Engine_Process_Information) );



	// Start the child process. 
	if( !CreateProcess(NULL,   // No module name (use command line)
		lpEngineAppName,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&g_Engine_Process_Information )           // Pointer to PROCESS_INFORMATION structure
		) 
	{	
		bResult = FALSE;
	}

	return bResult;
}


BOOL IsEngineProcessExist(HANDLE hProcess)
{
	DWORD dwExitCode = 0;

	::GetExitCodeProcess(hProcess, &dwExitCode);

	return dwExitCode == STILL_ACTIVE;


}
BOOL CALLBACK EnumWindowsProc(HWND hwnd,
							  LPARAM lParam
							  )
{
	BOOL bRet = TRUE;
	HWND* hWndOut = (HWND*)lParam;


	char szTempWndText[128];

	::GetWindowText(hwnd, szTempWndText, 128);

	if (strcmp(szTempWndText, EngineWindowName) == 0)
	{
		*hWndOut = hwnd;
		bRet = FALSE;

	}

	return bRet;
}
HWND GetEngineWnd()
{
	HWND hWnd_Ret = NULL;

	::EnumWindows((WNDENUMPROC)(EnumWindowsProc),(LPARAM) &hWnd_Ret);

	return hWnd_Ret;
}
BOOL StopEngineProcess()
{
	g_hwnd_Engine = GetEngineWnd();

	if (g_hwnd_Engine)
	{
		::SendMessage(g_hwnd_Engine, WM_DESTROY, 0, NULL);
	}

	return TRUE;
}