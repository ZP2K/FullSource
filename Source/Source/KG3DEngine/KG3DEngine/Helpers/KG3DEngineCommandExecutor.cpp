////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEngineCommandExecutor.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-12-10 17:13:03
/*  Comment     : �ڿͻ���ʹ�ñ���GMָ�Ȼ����������ExecuteCommand(LPCTSTR),Ȼ��ͻ�
������g_ExecuteEngineCommand��GMָ��ĸ�ʽ�� KG3DEngine.ExecuteCommand("�ַ���")
*/
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KG3DEngineCommandExecutor.h"
#include "KG3DShaderManager.h"	
#include "KG3DModelShadowRenderer.h"	//ModelShadowSync
#include "KG3DEngineManager.h"
#include "KG3DCollocator.h"				//ToggleFilter
#include "KG3DTypes.h"

////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
namespace
{
	BOOL GetFirstCommand(LPCTSTR strFullCommand, size_t uCommandLen, LPTSTR pBuffer, size_t uBufferSize)
	{
		errno_t errCode = 0;
		size_t i = 0;
		for (; i < uCommandLen; ++i)
		{
			TCHAR curChar = strFullCommand[i];
			if(curChar == _T(' ') || curChar == _T('('))
			{
				errCode = _tcsncpy_s(pBuffer, uBufferSize, strFullCommand, i);
				break;				
			}
		}
		KG_PROCESS_ERROR(errCode == 0);
		pBuffer[i] = _T('\0');

		if (i == uCommandLen)
		{
			errCode = _tcscpy_s(pBuffer, uBufferSize, strFullCommand);	//û�в��������
			KG_PROCESS_ERROR(errCode == 0);
			pBuffer[uBufferSize - 1] = _T('\0');
		}
		return TRUE;
Exit0:
		return FALSE;
	}

	static TCHAR strArgSep[] = _T(" ,)\t\n");

	LPTSTR GetFirstArg(LPTSTR strFullCommand, LPTSTR* pNextArg)
	{
		_ASSERTE(NULL != pNextArg);
		LPTSTR strRet = _tcschr(strFullCommand, _T('('));
		++strRet;

		LPTSTR strNext = NULL;
		KG_PROCESS_ERROR(NULL != strRet);
		strRet = _tcstok_s(strRet, strArgSep, &strNext);
		*pNextArg = strNext;
		return strRet;
Exit0:
		return NULL;
	}

	LPTSTR GetNextArg(LPTSTR* pNextArg)
	{
		return _tcstok_s(NULL, strArgSep, pNextArg);
	}
	struct ErrorOutputer 
	{
		virtual VOID Out(LPCTSTR pError) = 0;
	};

	struct ErrorOutputerSolid : public ErrorOutputer 
	{
		virtual VOID Out(LPCTSTR pError)
		{
			_ASSERTE(NULL != pError);
			::OutputDebugString(pError);
		}
	};

	ErrorOutputerSolid s_ErrorOutSolid;
	ErrorOutputer* s_pErrorOut = &s_ErrorOutSolid;


	VOID OutputError(LPCTSTR pError)
	{
		_ASSERTE(NULL != s_pErrorOut);
		s_pErrorOut->Out(pError);
	}

	struct ErrorOuterDummy : ErrorOutputer 
	{
		virtual VOID Out(LPCTSTR pError){NULL;}
	};
}

void ParseAndExecCommand(LPCTSTR strFirstCommand,  LPTSTR strCommand, size_t uCommandBufferSize)//strCommand �ǿɱ��
{
	ULONG uRet = g_cEngineManager.IsInitFinished();
	_ASSERTE(uRet && _T("����û�г�ʼ���������Ӳ��ܵ��ú���"));
	KG_PROCESS_ERROR(uRet);

	LPTSTR pNextArg = NULL;
	if(0 == _tcsicmp(strFirstCommand, _T("ReloadShader")))	//��Ӧ����KG3DEngine.ExecuteCommand("ReloadShader(xx.fx)")	
	{

		LPTSTR pShaderName = GetFirstArg(strCommand, &pNextArg);
		if(NULL != pShaderName)
		{
			KG3DShaderHolder tempHolder;
			HRESULT hr = g_GetShaderManager().Find(pShaderName, tempHolder);
			if (SUCCEEDED(hr))
			{
				tempHolder.Reload();
			}
			else
			{
				OutputError(_T("��Shader��û�б���ȷ����"));
				goto Exit0;
			}
		}
		else
		{
			OutputError(_T("�������Ϸ�"));
			goto Exit0;
		}
	}
	else if (0 == _tcsicmp(strFirstCommand, _T("ReloadAllShaders")))
	{
		g_GetShaderManager().ReloadAll();
	}
	else if (0 == _tcsicmp(strFirstCommand, _T("ToggleSwitch")))	//�����ʽ: ToggleSwitch(bXX, 0)
	{
		LPTSTR strFilterName = GetFirstArg(strCommand, &pNextArg);
		KG_PROCESS_ERROR(NULL != strFilterName);

		LPTSTR strBool = GetNextArg(&pNextArg);
		KG_PROCESS_ERROR(NULL != strBool);

		BOOL bEnable = FALSE;
		BOOL bRet = KGCH::TStringToValue(strBool, bEnable);
		KG_PROCESS_ERROR(bRet);

		KG3DCollocator::ToggleGlobalSwitch(KG3DCollocator::GetSwitchByName(strFilterName, NULL), bEnable);
	}
	else if (0 == _tcsicmp(strFirstCommand, _T("ModelShadowSync")))	//�����ʽ: ModelShadowSync(1)
	{
		LPTSTR pValue  = GetFirstArg(strCommand, &pNextArg);
		BOOL bSync = TRUE;
		if(KG_CUSTOM_HELPERS::TStringToValue(pValue, (INT)_tcslen(pValue) + 1, bSync))
		{
			g_GetModelShadowRenderer().SynchronizedStrenchRect(bSync);
		}
	}
	else if (0 == _tcsicmp(strFirstCommand, _T("ModelShadowType")))//�����ʽ: ModelShadowType(1)
	{
		LPTSTR pValue  = GetFirstArg(strCommand, &pNextArg);
		INT nType = 0;
		if(KG_CUSTOM_HELPERS::TStringToValue(pValue, (INT)_tcslen(pValue) + 1, nType))
		{
			KG3DEngineOption op;
			g_cEngineManager.GetEngineOption(&op);
			op.ModelShadowType = (ULONG)nType;
			g_cEngineManager.SetEngineOption(&op);
		}
	}
	else if(0 == _tcsicmp(strFirstCommand, _T("ForceShaderModel"))) //�����ʽ: ForceShaderModel(2)
	{
		LPTSTR pValue  = GetFirstArg(strCommand, &pNextArg);
		INT nModel = 0;
		if(KG_CUSTOM_HELPERS::TStringToValue(pValue, (INT)_tcslen(pValue) + 1, nModel))
		{
			if (nModel >= 0 && nModel <= KG3DEngineCaps::EM_MAX_SHADER_MODEL_SUPPORTED)//0��ʾ�Զ�ƥ��ShaderModel
			{
				g_cEngineOption.nForceShaderModel = nModel;
			}
		}
	}
    else if (!_tcsicmp(strFirstCommand, TEXT("EnableBsp")))
    {
        LPTSTR pValue  = GetFirstArg(strCommand, &pNextArg);
        INT nType = 0;
        if(KG_CUSTOM_HELPERS::TStringToValue(pValue, (INT)_tcslen(pValue) + 1, nType))
        {
            KG3DEngineOption op;
            g_cEngineManager.GetEngineOption(&op);
            op.bUseModelBsp = (BOOL)nType;
            g_cEngineManager.SetEngineOption(&op);
        }
    }
	else if(0==_tcsicmp(strFirstCommand,"ReportResourceUse"))
	{
		g_ReportResourceUse();
	}
Exit0:
	return;
}

HRESULT g_ExecuteEngineCommand( LPCTSTR strIC )
{
	TCHAR strFirstCommand[MAX_PATH];	//�ѵ�һ������ͳ�����������ʱ����Щ
	TCHAR* strCommand = NULL;	//Ҫ�ȸ��Ƴ�Сд����ȻScanf��ʧ��
	strFirstCommand[_countof(strFirstCommand) - 1]  = _T('\0');
	

	KG_PROCESS_ERROR(NULL != strIC);
	{
		size_t uCommandBufferSize = _tcslen(strIC) + 1;
		strCommand = new TCHAR[uCommandBufferSize];
		_tcscpy_s(strCommand, uCommandBufferSize, strIC);
		strCommand[uCommandBufferSize - 1] = _T('\0');

		//��������һ������
		BOOL bRet = GetFirstCommand(strCommand, _tcslen(strCommand), strFirstCommand, _countof(strFirstCommand));
		KG_PROCESS_ERROR(bRet);

		if (0 == _tcsicmp(strFirstCommand, _T("CommandSample")))	//�������������
		{
			int n1;
			float f2;
			TCHAR s3[MAX_PATH];
			unsigned u4;

			//_stscanf_s ��ȫ�ķ�deprecated�Ҽ���Unicode��sscanf�汾����sscanf���������ÿ���ַ���������Ҫ�����ַ�����С
			//sscanfϵ�����ɨ���ַ�������Ҫ������ʽд�����ο�google������ɨ���ַ�����ʱ��ܶ����ơ�����%[^,]����˼��
			//ɨ��һ���������ŵ��ַ���
			int nParsed = _stscanf_s(strCommand, _T("CommandSample(%d,%f,%[^,],%u)"), &n1, &f2, s3, _countof(s3), &u4);
			if(nParsed != 4)
			{
				OutputError(_T("������������ȫ"));
				goto Exit0;
			}

			//sscanf�ǱȽϼ���Ч�ķ���������һ��Ҫ�󲻶�Ļ��������ͺ���
			//�����ReloadShader�õ���GetFirstArg��GetNextArg�ķ����������ܼ��ݿո�ȴ���
			//�п�������Ҫ��ʱ���������TStringToValue��KG3DTYPE_INT��ֵд�������Զ�ƥ������б�İ汾
		}
		else
		{
			ParseAndExecCommand(strFirstCommand, strCommand, uCommandBufferSize);

		}
	}
	
	SAFE_DELETE_ARRAY(strCommand);
	return S_OK;
Exit0:
	SAFE_DELETE_ARRAY(strCommand);
	return	E_FAIL;
}

#if defined(KG_ENABLE_TDD)
KG_TEST_BEGIN(CommandExecutor)
{
	
	ErrorOuterDummy myErrorOutter;
	s_pErrorOut = &myErrorOutter;

	HRESULT hr = g_ExecuteEngineCommand(_T("CommandSample(-10,10.0,Oh_Yeah,20)"));
	_ASSERTE(SUCCEEDED(hr));
	hr = g_ExecuteEngineCommand(_T("CommandSample(-10,10.0,Oh_Yeah)"));
	_ASSERTE(FAILED(hr));

	s_pErrorOut = &s_ErrorOutSolid;
	_ASSERTE(NULL != s_pErrorOut);
}
KG_TEST_END(CommandExecutor)
#endif
