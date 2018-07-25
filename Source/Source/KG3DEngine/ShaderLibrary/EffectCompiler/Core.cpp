////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : Core.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-6-2 20:48:41
/*  Comment     : 

2009-06-02 �ļ����ֲܷ�ͬĿ¼��飬ͷ�ļ��ֲܷ�ͬĿ¼���������·���õ�ȫ·����ͷ�ļ���鲻�ܵݹ�
ͷ�ļ����ļ�������ͬ��
*/
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Core.h"

#include "shlwapi.h"//PathRemoveSpec

#include <map>
#include <string>
#include <iostream>

#include "HeaderAnalysor.h"
#include "Helpers.h"

  
#if defined(UNICODE) | defined(_UNICODE)
#define  tstring wstring
#define  tcout	wcout
#else
#define  tstring string
#define  tcout  cout
#endif
 
AppCmdParams g_AppCmdParams;

struct HeadFileData
{
	CString strName;
	CString strFullPath;
	BOOL bUnChanged;
	FILETIME ftLastWrite;
};

BOOL ProcessHeaders(HeaderAnalysor* pHeaderAnalysor, CString strFullPath, ICCompileResultOutput &outDevice, std::vector<HeadFileData> &vecHeadFileDatas, ICCompileFileController &fileController, BOOL &bAllHeadFilesUnChanged)
{
	CString csError;
	std::vector<CString> vecHeaders;
	if (! pHeaderAnalysor->GetHeaderFileNameList(strFullPath, &vecHeaders, &csError))
	{
		COutputItem outItem;
		COutputItem::CreateAppError(&outItem, csError);
		outDevice.Output(outItem);
		return FALSE;
	}

	//���ͷ�ļ��Ƿ�����������
	bAllHeadFilesUnChanged = TRUE;
	CString csDriverAndDir;

	//�õ�ȫ·���е�Driver��Dir���֣�׼���ϲ�·��
	{
		TCHAR strDrive[MAX_PATH], strDir[MAX_PATH];
		errno_t err = _tsplitpath_s(strFullPath, strDrive, _countof(strDrive)
			, strDir, _countof(strDir), NULL, 0, NULL, 0);
		csDriverAndDir = strDrive;
		csDriverAndDir += strDir;

		if (0 != err)
		{
			COutputItem outItem;
			CString csTemp; csTemp.Format(_T("File path split error: %s"), strFullPath);
			COutputItem::CreateAppError(&outItem, csTemp);
			outDevice.Output(outItem);
			return FALSE;
		}
	}

	vecHeadFileDatas.resize(vecHeaders.size());
	_ASSERTE(vecHeadFileDatas.size() == vecHeaders.size());
	for (size_t i = 0; i < vecHeaders.size(); ++i)
	{
		vecHeadFileDatas[i].strName = vecHeaders[i];
		vecHeadFileDatas[i].strFullPath = csDriverAndDir + vecHeadFileDatas[i].strName;	//ֻ�ܴ���û��..�����������ͬһ��Ŀ¼��������

		FILETIME fileTimeTemp;
		if(::GetSingleFileLastWriteTime(vecHeadFileDatas[i].strFullPath, &fileTimeTemp))
		{
			vecHeadFileDatas[i].ftLastWrite = fileTimeTemp;
			BOOL bUnChanged = fileController.IsFileUnChanged(vecHeadFileDatas[i].strName, fileTimeTemp);
			vecHeadFileDatas[i].bUnChanged = bUnChanged;
			if (! bUnChanged)
			{
				bAllHeadFilesUnChanged = FALSE;
			}
		}
		else
		{
			COutputItem outItem;
			CString csTemp; csTemp.Format(_T("Can not find this header file: %s"), (LPCTSTR)vecHeadFileDatas[i].strFullPath);
			COutputItem::CreateAppError(&outItem, csTemp);
			outDevice.Output(outItem);
			return FALSE;
		}
	}

	return TRUE;
}
void Core::CompileShaders(ICFileEnumer& fileEnumer, ICCompileResultOutput& outDevice, ICCompileFileController& fileController
						  , LPCTSTR strOutputDir, LPCTSTR strCopyToDir, BOOL bForceRebuild, BOOL bShowOnlyError)
{
	LPDIRECT3D9 pDeviceCore = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;

	HeaderAnalysor* pHeaderAnalysor = NULL;
	HRESULT hr = E_FAIL;
	
	
	{
		//�����豸
		HWND hWnd = ::GetDesktopWindow();
		hr = CreateDevice(pDeviceCore, hWnd, pDevice);
		COM_PROCESS_ERROR(hr);
	
	}

	{
		//����ͷ�ļ�������
		pHeaderAnalysor = ::CreateDefHeaderAnalysor();
		_ASSERTE(NULL != pHeaderAnalysor);
		/*std::vector<CString> vecTemp;
		CString csError;
		BOOL bRet = pHeaderAnalysor->GetHeaderFileNameList(_T("XXX.txt"), &vecTemp, &csError);
		_ASSERTE(vecTemp.size() == 4);*/
	}
	{
		BOOL bNeedCopyFile = FALSE;
		
		{
			//��ʼ��������Э����
			outDevice.SetOnlyOutputError(bShowOnlyError);
			fileController.SetForceRebuild(bForceRebuild);
			bNeedCopyFile = (NULL != strOutputDir && 0 != _tcslen(strOutputDir));
			fileController.SetPaths(strOutputDir, strCopyToDir);
		}

		{
			//�����ʼ��Ϣ
			if (bForceRebuild)
			{
				COutputItem outItem;
				COutputItem::CreateInfo(&outItem, COutputItem::em_out_notice, _T("Force Rebuild..."));
				outDevice.Output(outItem);
			}

			if (bNeedCopyFile)
			{
				COutputItem outItem;
				CString csTemp;
				_ASSERTE(NULL != strCopyToDir);
				csTemp.Format(_T("Files compiled successfully will be copied to this directory: %s"), strCopyToDir);
				
				COutputItem::CreateInfo(&outItem, COutputItem::em_out_notice, csTemp);
				outDevice.Output(outItem);
			}
		}

		//��ʼ����
		WIN32_FIND_DATA FindFileData;
		ICFileEnumer::em_get_file_ret emGetFileRet = fileEnumer.GetFirstFile(&FindFileData);
		
		FLOAT fFileCount = (FLOAT)fileEnumer.GetFileCount();

		if (fFileCount < 1)
		{
			COutputItem outItem;
			COutputItem::CreateAppError(&outItem, _T("Can not find any files"));
			outDevice.Output(outItem);
			return;
		}

		//׼������õ�ͳ�� 
		UINT uErrorCount = 0;
		UINT uSkippedCount = 0;
		UINT uSucceedCount = 0;
		UINT uValidFxFileCount = 0;
		UINT uCopiedFileCount = 0;
		UINT uFailedCopiedFileCount = 0;
		UINT uIncludeFileCopied = 0;
		UINT uIncludeFileCopiedFailed = 0;

		for(UINT uFileIndex = 0
			; emGetFileRet != ICFileEnumer::em_no_next_file
			; ++uFileIndex, emGetFileRet = fileEnumer.GetNextFile(&FindFileData))
		{
			outDevice.OutputPercent((FLOAT)uFileIndex / fFileCount);

			if (emGetFileRet == ICFileEnumer::em_file_name_not_correct)	//ָ�����ļ�ȴ�Ҳ����ļ����������
			{
				++uValidFxFileCount;
				++uErrorCount;
				COutputItem outItem;
				CString csTomuchFile;
				csTomuchFile.Format(_T("File io incorrect: %s"), FindFileData.cFileName);
				COutputItem::CreateAppError(&outItem, csTomuchFile);
				outDevice.Output(outItem);
				continue;
			}

			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	//����Ŀ¼
			{
				continue;
			}

			//������fx�ļ�
			{
				LPCTSTR pExt = _tcsrchr(FindFileData.cFileName, _T('.'));
				if(NULL == pExt)
					continue;

				if (0 != _tcsicmp(pExt, _T(".fx")))
					continue;
			}

			//_ASSERTE(0 == _tcsstr(FindFileData.cFileName, "shaderskin"));
			++uValidFxFileCount;
			if (uValidFxFileCount > em_file_limit)	//����̫����ļ��Ļ�����
			{
				COutputItem outItem;
				CString csTomuchFile;
				csTomuchFile.Format(_T("File Limit: %d, To much Files, compile process stopped, ..."), (int)em_file_limit);
				COutputItem::CreateAppError(&outItem, csTomuchFile);
				outDevice.Output(outItem);
				break;
			}

			//׼���ļ��͵ķ�����Ϣ
			COutputItem outItemFileResult;
			outItemFileResult.dwOutputCategory = COutputItem::em_out_file_result;
			outItemFileResult.GetFileName() = FindFileData.cFileName;
			outItemFileResult.bCompileSuccess = TRUE;

			//�õ�ȫ·��
			CString strFullPath;
			{
				LPCTSTR pfullPath = NULL;
				pfullPath = fileEnumer.FindFileAndGetFullPath();
				if (NULL == pfullPath)
				{
					outItemFileResult.bCompileSuccess = FALSE;
					outItemFileResult.GetCompileResult() = _T("Can not find the file");
					outDevice.Output(outItemFileResult);
					++uErrorCount;
					continue;
				}
				else
				{
					outItemFileResult.GetFullPath() = pfullPath;
					strFullPath = pfullPath;
				}
			}

			//����ͷ�ļ�
			
			BOOL bAllIncludeFilesUnChanged = TRUE;

			std::vector<HeadFileData> vecIncludeFiles;			

			if(! ProcessHeaders(pHeaderAnalysor, strFullPath, outDevice, vecIncludeFiles, fileController
				, bAllIncludeFilesUnChanged))
			{
				++uErrorCount;
				continue;
			}
			
			//�������ǿ�Ʊ��룬��ÿ��ͷ�ļ���û�ģ������Ѿ�������û�и��ĵ��ļ�
			if(! bForceRebuild)
			{
				if (bAllIncludeFilesUnChanged && fileController.IsFileUnChanged(FindFileData.cFileName, FindFileData.ftLastWriteTime))
				{
					BOOL bAllHeadFileUnChange = FALSE;
					outItemFileResult.bCompileSuccess = TRUE;
					outItemFileResult.GetCompileResult() = _T("Not changed,Skipped");
					outDevice.Output(outItemFileResult);

					++uSkippedCount;
					continue;
				}
			}
			
			//�õ�������Ϣ��fxo�ļ�
			LPD3DXBUFFER pBufferError = NULL;
			LPD3DXEFFECT pEffect = NULL;
			HRESULT hr = D3DXCreateEffectFromFile(pDevice, strFullPath, NULL, NULL, 0, NULL, &pEffect, &pBufferError);
			if (SUCCEEDED(hr))
			{
				outItemFileResult.bCompileSuccess = TRUE;	
				outItemFileResult.GetCompileResult() = _T("Compile Succeeded");
				
				fileController.SaveFileOutput(FindFileData.cFileName, FindFileData.ftLastWriteTime, pEffect, FALSE);

				//ͷ�ļ�Ҳ����
				for (size_t i = 0; i < vecIncludeFiles.size(); ++i)
				{
					fileController.SaveFileOutput(vecIncludeFiles[i].strName, vecIncludeFiles[i].ftLastWrite, NULL, TRUE);
				}

				if (bNeedCopyFile)	//ֻ��������������Ӧ��ͷ�ļ�
				{
					BOOL bCopyFileOK = TRUE;
					BOOL bCopiedSkipped = FALSE;
					CString csFileNowCopying;

					for (size_t i = 0; i < vecIncludeFiles.size(); ++i)
					{
						csFileNowCopying = vecIncludeFiles[i].strFullPath;
						bCopiedSkipped = FALSE;
						bCopyFileOK = fileController.CopyFileToDes(csFileNowCopying, &bCopiedSkipped);
						if (bCopyFileOK)
						{
							if (! bCopiedSkipped)
							{
								COutputItem outItem;
								CString csTemp; csTemp.Format(_T("%s, Included file copied"), vecIncludeFiles[i].strFullPath);
								COutputItem::CreateInfo(&outItem, COutputItem::em_out_notice, csTemp);
								outDevice.Output(outItem);
								++uIncludeFileCopied;
							}
						}
						else
						{
							COutputItem outItem;
							CString csTemp; csTemp.Format(_T("%s, Included file copied failed"), vecIncludeFiles[i].strFullPath);
							COutputItem::CreateAppError(&outItem, csTemp);
							outDevice.Output(outItem);
							++uIncludeFileCopiedFailed;
							continue;
						}
					}

					if (bCopyFileOK)
					{
						csFileNowCopying= strFullPath;
						bCopyFileOK = fileController.CopyFileToDes(csFileNowCopying, &bCopiedSkipped);
					}

					if (! bCopyFileOK)
					{
						COutputItem outItem;
						CString csTomuchFile;
						_ASSERTE(NULL != strCopyToDir);
						csTomuchFile.Format(_T("File Copy Error: From: %s, To %s"), csFileNowCopying, strCopyToDir);
						COutputItem::CreateAppError(&outItem, csTomuchFile);
						outDevice.Output(outItem);
						
						++uFailedCopiedFileCount;
					}
					else
					{
						++uCopiedFileCount;
						if (bCopiedSkipped)
						{
							outItemFileResult.GetCompileResult() += _T("; Copy skipped because last write time of files identical");
						}
						else
						{
							outItemFileResult.GetCompileResult() += _T("; Copy OK");
						}
					}
				}
				++uSucceedCount;
			}
			else
			{
				++uErrorCount;
				outItemFileResult.bCompileSuccess = FALSE;	

				LPCTSTR pError = NULL;

				if (NULL != pBufferError)
				{
					pError = static_cast<LPCTSTR>(pBufferError->GetBufferPointer());
				}

				if (NULL != pError)
				{
					outItemFileResult.GetCompileResult() = pError;
				}
				else
				{
					outItemFileResult.GetCompileResult() = strFullPath;
					outItemFileResult.GetCompileResult() += _T(" : File path incorrect or application failed");
				}
			}
			SAFE_RELEASE(pBufferError);
			SAFE_RELEASE(pEffect);

			outDevice.Output(outItemFileResult);
			
		}//end of For

#if defined(DEBUG) | defined(_DEBUG)
		if (uValidFxFileCount <= em_file_limit)
		{
			_ASSERTE(_T("�����Ϣ���Ժ���") && (uValidFxFileCount == uSucceedCount + uSkippedCount + uErrorCount));
		}
		if (bNeedCopyFile)
		{
			_ASSERTE(uCopiedFileCount + uFailedCopiedFileCount == uSucceedCount);
		}
#endif

		outDevice.OutputPercent(1);
		
		{
			CString outString;
			if (0 == uErrorCount&& 0 == uFailedCopiedFileCount && 0 == uIncludeFileCopiedFailed)
			{
				outString = _T("All succeeded;");
			}
			else
			{
				outString = _T("There are errors;");
			}

			{
				CString csTemp;
				csTemp.Format(_T("%u Compiled, %u Success, %u Skipped, %u Error, %u Copied, %u Copy failed, %u Include file copied, \
%u Include file copy failed")
					, uValidFxFileCount, uSucceedCount, uSkippedCount, uErrorCount, uCopiedFileCount, uFailedCopiedFileCount
					, uIncludeFileCopied, uIncludeFileCopiedFailed);
				outString += csTemp;
			}

			COutputItem outItem;
			COutputItem::CreateInfo(&outItem, COutputItem::em_out_statistic, outString);

			outDevice.Output(outItem);
		}
	}
Exit0:
	SAFE_DELETE(pHeaderAnalysor);
	SAFE_RELEASE(pDeviceCore);
	SAFE_RELEASE(pDevice);
	return;
}

HRESULT Core::CreateDevice( LPDIRECT3D9 &pDeviceCore, HWND hWndOut, LPDIRECT3DDEVICE9& pDevice )
{
	HRESULT hr = E_FAIL;
	{
		pDeviceCore = Direct3DCreate9(D3D_SDK_VERSION);
		PROCESS_ERROR(NULL != pDeviceCore);

		D3DPRESENT_PARAMETERS params;
		params.BackBufferWidth = 256;
		params.BackBufferHeight = 256;
		params.BackBufferFormat = D3DFMT_A8R8G8B8;
		params.BackBufferCount = 1;

		params.MultiSampleType = D3DMULTISAMPLE_NONE;
		params.MultiSampleQuality = 0;

		params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		params.hDeviceWindow = hWndOut;	//�������CreateDevice����ָ��Wnd����Ҫ����ָ��
		params.Windowed = TRUE;
		params.EnableAutoDepthStencil = TRUE;
		params.AutoDepthStencilFormat = D3DFMT_D24S8;
		params.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

		/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
		params.FullScreen_RefreshRateInHz = 0;
		params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		hr = pDeviceCore->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL
			, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &pDevice);
		COM_PROCESS_ERROR(hr);

	}
Exit0:
	return hr;
}

ICCompileResultOutput& Core::GetDefaultOutput()
{
	struct CStdOut : public  ICCompileResultOutput
	{
		virtual VOID SetOnlyOutputError(BOOL bOnlyOutputError)
		{
			m_bOnlyOutputError = bOnlyOutputError;
			std::tcout << _T("\n");
		}
		virtual VOID Output(const COutputItem& outItem)
		{
			switch(outItem.dwOutputCategory)
			{
			case COutputItem::em_out_file_result:
				{
					if (outItem.bCompileSuccess)
					{
						if (! m_bOnlyOutputError)
						{
							std::tcout << _T("\t\t") << outItem.GetFileName() << _T(" ") << outItem.GetCompileResult(); 
						}
					}
					else
					{
						std::tcout << outItem.GetCompileResult();
					}
				}
				break;
			case COutputItem::em_out_notice:
			case COutputItem::em_out_statistic:
				{
					std::tcout << outItem.GetCompileResult();
				}
				break;
			}
			std::tcout << _T("\n") << std::flush;//���һ�����˼�ǵ���flush��ǿ�������������������壬�ڱ����ʱ�������Ļ���������֮ǰ�����������
		}
		virtual VOID OutputPercent(FLOAT fPercent)
		{
			return;
		}

		BOOL m_bOnlyOutputError;
		CStdOut():m_bOnlyOutputError(FALSE){}
	};
	static CStdOut s_Instance;
	return s_Instance;
}



LPCTSTR Core::FindFileAndGetFullPath( LPCTSTR strFileName, CString* pRetString)
{
	_ASSERTE(NULL != pRetString && NULL != strFileName);

	static bool bModulePath;
	static CString	strModulePath;
	if (! bModulePath)
	{
		bModulePath = true;

		TCHAR strTemp[MAX_PATH * 2] = _T("");
		::GetModuleFileName(0, strTemp, _countof(strTemp));

		::PathRemoveFileSpec(strTemp);

		strModulePath = strTemp;
	}
	if (! pRetString->Find(_T('\\')) && ! pRetString->Find(_T('/')))
	{
		*pRetString = strModulePath;
		*pRetString += _T("\\");
		*pRetString += strFileName;
	}
	else
	{
		*pRetString = strFileName;
	}
	
	return *pRetString;
}
VOID COutputItem::GetElementDesc( LPCTSTR** pItemDescArray, size_t* pDescCount ) /*�����ָ���ַ����������ָ�����˼����ָ���ָ����������ָ������壬��Ȼ���ѿ��� */
{
	static LPCTSTR s_DescArray[] = 
	{
		_T("File Name"),
		_T("Compile Result"),
		_T("Compile Path"),
	};

	if (NULL != pItemDescArray)
	{
		*pItemDescArray = s_DescArray;
	}

	if (NULL != pDescCount)
	{
		*pDescCount = _countof(s_DescArray);
	}
}

VOID COutputItem::CreateInfo(COutputItem* pOutItem, DWORD dwCategory, LPCTSTR strString )
{
	_ASSERTE(NULL != pOutItem && dwCategory != em_out_file_result);
	_ASSERTE(dwCategory < em_out_count);

	pOutItem->dwOutputCategory = dwCategory;
	pOutItem->GetCompileResult() = strString;
}

VOID COutputItem::CreateAppError( COutputItem* pOutItem, LPCTSTR strErrorResult )
{
	_ASSERTE(NULL != pOutItem);

	pOutItem->dwOutputCategory = em_out_file_result;
	pOutItem->GetFileName() = _T("Application");
	pOutItem->GetCompileResult() = strErrorResult;
	pOutItem->bCompileSuccess = FALSE;
}

BOOL ICCompileFileController::CopyFileToDesDefaultImp( LPCTSTR fileFullPath, LPCTSTR copyToFullPath )
{
	_ASSERTE(NULL != fileFullPath && NULL != copyToFullPath);
	const BOOL bFailIfExists= FALSE;
	BOOL bRet = ::CopyFile(fileFullPath, copyToFullPath, bFailIfExists);
	/*TCHAR tcsDir[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, tcsDir);
	BOOL bRet = ::CopyFile(_T("C:/a.fx"), ("C:/b.fx"), bFailIfExists);*/
	return bRet;
}