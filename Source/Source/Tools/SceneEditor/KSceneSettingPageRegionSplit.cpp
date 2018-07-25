// KSceneSettingPageRegionSplit.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "KSceneSettingPageRegionSplit.h"
#include "IEKG3DRegionInfoManager.h"
#include "IEKG3DTerrain.h"
//#include "KSceneInterfaceGetters.h"
#include "KSceneCommonParamDialog.h"
#include "KSceneSceneHelpers.h"
#include "IEEditor.h"
//#include "IEOther.h"
#include "KGStringFunctionsBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//KSceneSettingPageRegionSplit dialog

//using namespace KSH::COMMON_LIST;
//typedef TKGCommonListTrait<IEKG3DRegionInfoManager, DWORD, LOAD_STRATAGE_LOADALL, GET_NAME_BY_VALUE> RI_LIST_TRAIT;

static IEKG3DRegionInfoManager* FuncRIMGetter(DWORD_PTR WParam, DWORD_PTR LParam)
{
	IEKG3DRegionInfoManager* pRIM = NULL;
	IEKG3DRegionInfoManagerTable* pRIMTable 
		= KSH::GetEngineCombinativeInterface<KG3DTYPE_RESOURCE_TABLE_TYPE, KG3DTYPE_REGION_INFO_MANAGER>();
	if (pRIMTable)
	{
		HRESULT hr  = pRIMTable->GetRIM(_kg_to_smaller_size_cast<DWORD>(WParam), &pRIM, NULL);
		if (SUCCEEDED(hr))
		{
			return pRIM;
		}
	}
	return NULL;
}

/*
static RI_LIST_TRAIT stTempTrait(&FuncRIMGetter
								 , NULL
								 , &IEKG3DRegionInfoManager::GetDiffArrayCount
								 , &IEKG3DRegionInfoManager::GetDiffArray
								 , NULL
								 , NULL
								 );
typedef TKGCommonListHelper<RI_LIST_TRAIT> RI_LIST_HELPER;
static RI_LIST_HELPER stTempHelper(stTempTrait);*/



IMPLEMENT_DYNAMIC(KSceneSettingPageRegionSplit, CPropertyPage)

KSceneSettingPageRegionSplit::KSceneSettingPageRegionSplit()
	: KSceneSettingPageBase(KSceneSettingPageRegionSplit::IDD)
	, m_nTexID(-1)
	, m_nOutputWndID(-1)
	, m_RegionList(TypeContainerGetter()
					, TypeElemGetter(&IEKG3DRegionInfoManager::GetDiffArrayCount
						, &IEKG3DRegionInfoManager::GetDiffArray)
					, TypeNameGetter()
					)
	, m_bShowRegionMap(FALSE)
	, m_bNeedShowRegionMap(FALSE)
	, m_uRegionCount(0)
	, m_uWidth(0)
	, m_uHeight(0)
	, m_uPixelPerRegion(0)
	, m_uMemorySize(0)
	//, m_bEnableOutputRegionIndex(FALSE)
	, m_dwTimeHandle(0)
	//, m_fLastTimerTime(0)
{
#if defined(DEBUG) | defined(_DEBUG)
	m_nFateCode = 0xFA7EC0DE;
#endif
}

KSceneSettingPageRegionSplit::~KSceneSettingPageRegionSplit()
{
}

void KSceneSettingPageRegionSplit::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SS_TARGET_TREE, m_Tree);
	DDX_Control(pDX, IDC_ST_OUTPUT_WND, m_stOutputWnd);
	DDX_Control(pDX, IDC_LT_SS_ELEMENT, m_RegionList);
	DDX_Control(pDX, IDC_CK_SHOW_REGION_SPLIT_MAP, m_ckShowRegionMap);
	DDX_Control(pDX, IDC_SS_ED_TEXPATH, m_edTexPath);
	DDX_Text(pDX, IDC_ED_COUNT, m_uRegionCount);
	DDX_Text(pDX, IDC_ED_WIDTH, m_uWidth);
	DDX_Text(pDX, IDC_ED_HEIGHT, m_uHeight);
	DDX_Text(pDX, IDC_ED_PIXELS_COUNT, m_uPixelPerRegion);
	DDX_Text(pDX, IDC_ED_MEMORY, m_uMemorySize);
}


BEGIN_MESSAGE_MAP(KSceneSettingPageRegionSplit, CPropertyPage)
ON_BN_CLICKED(IDC_SS_LIST_BN_ADD, &KSceneSettingPageRegionSplit::OnBnClickedSsListBnAdd)
	//ON_BN_CLICKED(IDC_SS_BN_READ_REGION_SPLIT_MAP, &KSceneSettingPageRegionSplit::OnBnClickedSsBnReadRegionSplitMap)
	ON_BN_CLICKED(IDC_CK_SHOW_REGION_SPLIT_MAP, &KSceneSettingPageRegionSplit::OnBnClickedCkShowRegionSplitMap)
	ON_BN_CLICKED(IDC_SS_BN_READ_REGION_SPLIT_MAP_AGAIN, &KSceneSettingPageRegionSplit::OnBnClickedSsBnReadRegionSplitMapAgain)
	ON_BN_CLICKED(IDC_SS_LIST_BN_DEL, &KSceneSettingPageRegionSplit::OnBnClickedSsListBnDel)
	ON_BN_CLICKED(IDC_SS_LIST_BN_SAVE, &KSceneSettingPageRegionSplit::OnBnClickedSsListBnSave)
	ON_BN_CLICKED(IDC_SS_LIST_BN_RENAME, &KSceneSettingPageRegionSplit::OnBnClickedSsListBnRename)
	ON_BN_CLICKED(IDC_CK_SHOW_REGION_INDEX, &KSceneSettingPageRegionSplit::OnBnClickedCkShowRegionIndex)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CK_SHOW2, &KSceneSettingPageRegionSplit::OnBnClickedCkShowMask)
END_MESSAGE_MAP()


// KSceneSettingPageRegionSplit message handlers

void KSceneSettingPageRegionSplit::OnBnClickedSsListBnAdd()
{
	// TODO: Add your control notification handler code here
	
	{
		CString csLoadPath = this->GetLoadPath();
		KG_PROCESS_ERROR(csLoadPath != _T(""));
		//////////////////////////////////////////////////////////////////////////

		//�õ�Ĭ���������
		IEKG3DRegionInfoManager* pRM = this->GetCurRIM();
		KG3DREGION_INFORMER_LOADFILE_PARAM* pLoadParam;
		pLoadParam = this->GetLoadParam(pRM ? pRM : NULL);
		KG_PROCESS_ERROR(pLoadParam);		

		//////////////////////////////////////////////////////////////////////////
		HRESULT hr = AddRIM(csLoadPath, pLoadParam, NULL, NULL);
		KG_COM_PROCESS_ERROR(hr);
	}
	return;
Exit0:
	return;
}
void KSceneSettingPageRegionSplit::OnBnClickedSsListBnDel()
{
	// TODO: Add your control notification handler code here
	int nRet = MessageBox(_T("ȷ��ɾ���𣿣�ɾ����ֻ���ڴ��е����ݣ��ѱ���ķֿ�ͼ�����ᱻɾ�����´λ����Լ���")
		, _T("��ʾ"), MB_OKCANCEL|MB_ICONINFORMATION);
	KG_PROCESS_SUCCESS(nRet != IDOK);
	{	
		HRESULT hr = DelRIM();
		KG_COM_PROCESS_ERROR(hr);
	}
Exit1:
	return;
Exit0:
	MessageBox(_T("�ֿ�ͼ��û�б�ɾ����������û�������ѡ��һ���ֿ�ͼ\
��Ҳ�����������л����ڶ�����ֿ�ͼ�����ã��㲻��ɾ����"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
	return;
}
BOOL KSceneSettingPageRegionSplit::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	m_Tree.DeleteAllItems();
	///m_vecSavedTex����Ҫ���
	IEKG3DRegionInfoManagerTable* pTable = KSH::GetEngineCombinativeInterface<KG3DTYPE_RESOURCE_TABLE_TYPE, KG3DTYPE_REGION_INFO_MANAGER>();
	if (pTable)
	{
		DWORD dwCurHandle = 0;
		DWORD dwNextHandle = 0;
		INT nRIMCount = 0;
		HRESULT hr = pTable->GetCount(&nRIMCount);
		_ASSERTE(SUCCEEDED(hr));
		if(SUCCEEDED(hr) && nRIMCount > 0)
		{
			IEKG3DRegionInfoManager* pRIM = NULL;
			hr = pTable->GetFirstRIM(&pRIM, &dwCurHandle, &dwNextHandle);
			while (SUCCEEDED(hr)  && dwCurHandle)
			{	
				CHAR cPathBuffer[MAX_PATH] = "";
				HRESULT hr = pRIM->GetPath(cPathBuffer, _countof(cPathBuffer));
				_ASSERTE(SUCCEEDED(hr));
				cPathBuffer[_countof(cPathBuffer) - 1] = '\0';///������ζ����Ͻ�������
				KG_CUSTOM_HELPERS::UnicodeConvertor Cvt;

				//�ñ�������֣����ڲ���Path���������������
				this->AddAnRIMToTree(dwCurHandle, NULL, Cvt.ToT(cPathBuffer), NULL);
				
				//////////////////////////////////////////////////////////////////////////
				dwCurHandle = dwNextHandle;
				dwNextHandle = 0;
				hr = pTable->GetRIM(dwCurHandle, &pRIM, &dwNextHandle);
			}
			_ASSERTE(m_Tree.GetCount() == (UINT)nRIMCount);
		}
	}

	BindCallbacks(TRUE);

	//////////////////////////////////////////////////////////////////////////
	if (m_Tree.GetCount() > 0)///����SelChange�ص���ˢ��ҳ��
	{
		HTREEITEM hFirstItem = m_Tree.GetChildItem(TVI_ROOT);
		if (m_Tree)
		{
			m_Tree.SelectItem(hFirstItem);
		}
	}
	if (KSH::SynACheckWndWithABool(this->GetDlgItem(IDC_CK_SHOW_REGION_INDEX), FALSE, FALSE))
	{
		this->EnableOuputRegionIndex(TRUE);
	}
	if (KSH::SynACheckWndWithABool(this->GetDlgItem(IDC_CK_SHOW2), FALSE, FALSE))
	{
		this->EnableShowMask(TRUE);
	}

	//�������ؼ��
	this->SetTimer(1, 500, NULL);

	//SynFromInnerToInterface();
	return CPropertyPage::OnSetActive();
}

BOOL KSceneSettingPageRegionSplit::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class
	BindCallbacks(FALSE);
	RemoveOutputTexWnd();
	
	this->EnableOuputRegionIndex(FALSE);
	this->EnableShowMask(FALSE);

	//ȥ�����ؼ��
	this->KillTimer(1);
	return CPropertyPage::OnKillActive();
}

BOOL KSceneSettingPageRegionSplit::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

VOID KSceneSettingPageRegionSplit::BindCallbacks( BOOL bBind )
{
	if (bBind)
	{
		/*m_Tree.CallbackSetFuncAndParam<KSceneSceneTreeCtrl::CALLBACK_ITEM_SELCHANGED>(&FuncTreeSelChangeCallback
			, (DWORD_PTR)this);*/
		m_Tree.SetEventHandler(this);
	}
	else
	{
		//m_Tree.CallbackUnBindAll();
		m_Tree.SetEventHandler(NULL);
	}
}
BOOL KSceneSettingPageRegionSplit::OnItemSelectChange( KSceneSceneTreeCtrl& tree, HTREEITEM hItemInserted, HTREEITEM hItemLastSelect )
{
	KSceneSettingPageRegionSplit* pDialog = this;
	_ASSERTE(pDialog->IsValidInMemory());

	pDialog->SynFromInnerToInterface();

	if(BST_CHECKED == pDialog->IsDlgButtonChecked(IDC_CK_SHOW2))
		pDialog->EnableShowMask(TRUE);
	return TRUE;
}
//DWORD_PTR CALLBACK KSceneSettingPageRegionSplit::FuncTreeSelChangeCallback( KSceneSceneTreeCtrl* pTree, HTREEITEM hItem, DWORD_PTR dwTreeParam, DWORD_PTR dwCustomParam )
//{
//	KSceneSettingPageRegionSplit* pDialog = (KSceneSettingPageRegionSplit*)dwCustomParam;
//	_ASSERTE(pDialog->IsValidInMemory());
//	
//	pDialog->SynFromInnerToInterface();
//
//	if(BST_CHECKED == pDialog->IsDlgButtonChecked(IDC_CK_SHOW2))
//		pDialog->EnableShowMask(TRUE);
//	return TRUE;
//}
//////////////////////////////////////////////////////////////////////////
int KSceneSettingPageRegionSplit::Update()
{
	return 1;
}

HRESULT KSceneSettingPageRegionSplit::ShowRegionMap(BOOL bShow)
{
	IEKG3DRegionInfoManager* pRI = this->GetCurRIM();
	KG_PROCESS_ERROR(pRI);
	{
		if (bShow)
		{
			BOOL bRet = FALSE;
			BOOL bExist = FALSE;
			INT  nID = -1;
			HRESULT hr = pRI->GetRegionTexState(&bExist, &nID, NULL, 0);
			if (SUCCEEDED(hr) && bExist)
			{
				
				_KG3D_DEBUG_FAIL_REPORT(nID != -1);
				bRet = AddOutputTexWnd(nID);
			}
			else
			{
				DWORD dwTexID = 0;
				HRESULT hr = pRI->GenerateRegionTex(0, &dwTexID);
				if (SUCCEEDED(hr))
				{
					bRet= AddOutputTexWnd(dwTexID);
				}
			}
			if (!bRet)
			{
				MessageBox("�����ͼ���ʧ��");
			}
			KSH::SynACButtonWithABool(m_ckShowRegionMap, TRUE, TRUE);
			m_bNeedShowRegionMap = TRUE;
		}
		else
		{
			this->RemoveOutputTexWnd();
			KSH::SynACButtonWithABool(m_ckShowRegionMap, FALSE, FALSE);
			m_bNeedShowRegionMap = FALSE;
		}
	}
	return S_OK;
Exit0:
	return E_FAIL;
}

// KSceneSettingPageRegionSplit message handlers

HRESULT KSceneSettingPageRegionSplit::SynFromInterfaceToInner(DWORD_PTR dwOption/* = 0*/ )
{
	return S_OK;
}
HRESULT KSceneSettingPageRegionSplit::SynFromInnerToInterface(DWORD_PTR dwOption /*= 0*/ )
{
	HTREEITEM hItem = m_Tree.GetSelectedItem();
	if (hItem)
	{
		DWORD_PTR dwRIHandle = m_Tree.GetItemData(hItem);
		
		//�ı�List�еĿ��ƶ��󣬼�Tree��ѡ����RIM
		m_RegionList.GetContainerGetter().m_WParam = dwRIHandle;
		m_RegionList.Reload();
		
		ShowRegionMap(m_bNeedShowRegionMap);
		IEKG3DRegionInfoManager* pRIM = FuncRIMGetter(dwRIHandle, 0);

		{
			this->m_uHeight = 0;
			this->m_uWidth = 0;
			this->m_uPixelPerRegion = 0;
			this->m_uRegionCount = 0;
			this->m_uMemorySize = 0;
		}
		if (pRIM)
		{
			CHAR cPathBuffer[MAX_PATH];
			ZeroMemory(cPathBuffer, sizeof(CHAR)*_countof(cPathBuffer));
			HRESULT hr = pRIM->GetPath(cPathBuffer, _countof(cPathBuffer));
			_ASSERTE(SUCCEEDED(hr));
			cPathBuffer[_countof(cPathBuffer) - 1] = '\0';///������ζ����Ͻ�������
			this->m_edTexPath.SetWindowText(cPathBuffer);

			INT nDiffArrayCount = 0;
			if (SUCCEEDED(pRIM->GetDiffArrayCount(&nDiffArrayCount)))
			{
				this->m_uRegionCount = (UINT)nDiffArrayCount;
			}
			KG3DREGION_INFORMER_STATE State;
			if (SUCCEEDED(pRIM->IEGetState(&State)))
			{
				this->m_uHeight = State.uInnerTexHeight;
				this->m_uWidth = State.uInnerTexWidth;
				this->m_uPixelPerRegion = State.uHowManyPixelsInOneRegion;
				this->m_uMemorySize = State.uSizeInMemory;
			}
			
		}
		UpdateData(FALSE);
		
	}
	
	return S_OK;
}
KG3DREGION_INFORMER_LOADFILE_PARAM* KSceneSettingPageRegionSplit::GetLoadParam(IEKG3DRegionInfoManager* pRIM)
{
	KG3DREGION_INFORMER_LOADFILE_PARAM	TempParam;
	KSceneCommonParamDialog TempParamDlg;
	HRESULT hr = E_FAIL;
	if (pRIM)
	{
		hr = pRIM->GetDefaultParam(&TempParam);
		_ASSERTE(SUCCEEDED(hr));
	}	
	INT nIndex1 = TempParamDlg.AddAnParam(_T("�����ݲ���ֵ"), TempParam.nPixelTolerance,TRUE);
	INT nIndex2 = TempParamDlg.AddAnParam(_T("��������(1-3)"), TempParam.nSampleTimes,TRUE);
	INT nIndex3 = TempParamDlg.AddAnParam(_T("��ʳ������Ƭ"), TempParam.nEliminateIsoDataCount,TRUE);
	INT nIndex4 = TempParamDlg.AddAnParam(_T("ɫ������"), TempParam.nTotalValue,TRUE);
	INT nIndex5 = TempParamDlg.AddAnParam(_T("����ÿRegion"), TempParam.uHowManyPixelsInOneRegion,TRUE);
	KG_PROCESS_ERROR(nIndex1 >= 0 && nIndex2 >= 0);
	if (TempParamDlg.DoModal() == IDOK)
	{
		BOOL bRet = TRUE;
		bRet &= TempParamDlg.GetData(nIndex1, TempParam.nPixelTolerance);
		bRet &= TempParamDlg.GetData(nIndex2, TempParam.nSampleTimes);
		bRet &= TempParamDlg.GetData(nIndex3, TempParam.nEliminateIsoDataCount);
		bRet &= TempParamDlg.GetData(nIndex4, TempParam.nTotalValue);
		bRet &= TempParamDlg.GetData(nIndex5, TempParam.uHowManyPixelsInOneRegion);
		if (! bRet)
		{
			MessageBox(_T("��������ȷ������Ĭ�ϲ�������"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
			if (pRIM)
			{
				hr = pRIM->GetDefaultParam(&TempParam);
				_ASSERTE(SUCCEEDED(hr));
			}
		}
		TempParam.dwRetTextureID = 0;
		TempParam.m_bNeedPreviewTex = TRUE;///����������Ժ�Ҫ��Ҫ������������ͼ��
		*m_pLoadParam = TempParam;
		m_pLoadParam->strError = _T("");//��մ������
		return KGCommonBufferGetPtr(m_pLoadParam);//����������ģ��Ĳ���ƥ�亯�����ң�����дnamespace�ģ�Ҳû��д
	}
Exit0:
	return NULL;
}
CString KSceneSettingPageRegionSplit::GetLoadPath()
{
	char szFilter[] ="Texture Files(*.bmp;*.jpg;*.png;*.tga;*.Spr;*.DDS)|*.bmp;*.jpg;*.png;*.tga;*.Spr;*.DDS||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY , szFilter);
	if (dlg.DoModal() == IDOK)
	{
		return dlg.GetPathName();
	}
	return _T("");
}
/*
void KSceneSettingPageRegionSplit::OnBnClickedSsBnReadRegionSplitMap()
{
	// TODO: Add your control notification handler code here
	
	HRESULT hr = E_FAIL;
	
	IEKG3DRegionInfoManager* pRM = this->GetCurRIM();
	KG_PROCESS_ERROR(pRM);
	{
		CString csLoadPath = this->GetLoadPath();
		KG_PROCESS_ERROR(csLoadPath != _T(""));
		KG3DREGION_INFORMER_LOADFILE_PARAM* pLoadParam = this->GetLoadParam(pRM);
		KG_PROCESS_ERROR(pLoadParam);
		BOOL bRet = ReloadRegionMap(csLoadPath, pLoadParam);
		KG_PROCESS_ERROR(bRet);
		return;
	}
Exit0:
	MessageBox(_T("��ͼ��ȡʧ��"),_T("��ʾ"), MB_ICONINFORMATION | MB_OK);
	return;
}*/


void KSceneSettingPageRegionSplit::OnBnClickedCkShowRegionSplitMap()
{
	// TODO: Add your control notification handler code here
	BOOL bTemp = FALSE;
	KSH::SynACButtonWithABool(m_ckShowRegionMap, bTemp, FALSE);
	if ((bTemp&& !m_bNeedShowRegionMap) || (!bTemp && m_bNeedShowRegionMap ))
	{
		ShowRegionMap(bTemp);
	}
}

VOID KSceneSettingPageRegionSplit::RemoveOutputTexWnd()
{
	IEKG3DScene* pScene = KSH::GetEngineInterface<KG3DTYPE_SCENE>();
	if (pScene)
	{
		HRESULT hr = pScene->RemoveOutputWindow(m_nOutputWndID);
		if (SUCCEEDED(hr))
		{
			m_nOutputWndID = -1;
			m_bShowRegionMap = FALSE;
			KSH::SynACButtonWithABool(m_ckShowRegionMap, m_bShowRegionMap, TRUE);
			this->Invalidate();
		}
	}
}

BOOL KSceneSettingPageRegionSplit::AddOutputTexWnd( INT nTexID )
{
	if (m_nOutputWndID >= 0)
	{
		RemoveOutputTexWnd();
	}
	KG_PROCESS_ERROR(nTexID != KG3DREGION_INFORMER_LOADFILE_PARAM::INVALID_MAX_TEXTURE_ID);
	{
		IEKG3DScene* pScene = KSH::GetEngineInterface<KG3DTYPE_SCENE>();
		//CWnd* pWnd = GetDlgItem(IDC_ST_OUTPUT_WND);
		KG_PROCESS_ERROR(pScene);
		{
			INT nTempRetWinID = -1;
			OUTPUTWND_TEXTURE_PARAM TempParam;
			TempParam.dwTextureID = nTexID;
			HRESULT hr = pScene->AddOutputWindow(_T("Region�ֿ���ͼ"), m_stOutputWnd.GetSafeHwnd(), OUTPUTWND_SCENEEDITOR_TEXTUREVIEW, (DWORD_PTR)(&TempParam), &nTempRetWinID);
			KG_PROCESS_ERROR(SUCCEEDED(hr) && nTempRetWinID >= 0);
			m_nOutputWndID = nTempRetWinID;
			m_nTexID = nTexID;
			m_bShowRegionMap = TRUE;
		}
	}
	return TRUE;
Exit0:
	MessageBox("�����ͼ�������ʧ��");
	return FALSE;
}

IEKG3DRegionInfoManager* KSceneSettingPageRegionSplit::GetCurRIM( DWORD_PTR* pdwHandle, HTREEITEM* pTreeItem)
{
	HTREEITEM hItem = m_Tree.GetSelectedItem();
	if(!hItem)
		return NULL;
	if (pTreeItem)
	{
		*pTreeItem = hItem;
	}
	DWORD_PTR dwHandle = m_Tree.GetItemData(hItem);
	IEKG3DRegionInfoManager* pRet = FuncRIMGetter(dwHandle, 0);
	if (pdwHandle)
	{
		*pdwHandle = dwHandle;
	}
	return pRet;
}



void KSceneSettingPageRegionSplit::OnBnClickedSsBnReadRegionSplitMapAgain()
{
	// TODO: Add your control notification handler code here
	if (! ReloadRegionMap(NULL, NULL))
	{
		MessageBox(_T("����ʧ��"));
	}
}

BOOL KSceneSettingPageRegionSplit::ReloadRegionMap(LPCTSTR lpPath, KG3DREGION_INFORMER_LOADFILE_PARAM* pParam)
{
	DWORD dwHandle = NULL;
	HRESULT hr = E_FAIL;
	IEKG3DRegionInfoManager* pRIM = this->GetCurRIM(&dwHandle);
	KG_PROCESS_ERROR(pRIM);
	
	{
		TypeRIMExtraParamContainer::iterator it = m_ParamContainer.find(dwHandle);
		KG_PROCESS_ERROR(it != m_ParamContainer.end());
		TypeRIMExtraParams& ExtraParam = it->second;

		KG3DREGION_INFORMER_LOADFILE_PARAM* pLoadParam = NULL;
		KG3DREGION_INFORMER_LOADFILE_PARAM TempLoadParam;
		if (pParam) 
		{
			pLoadParam = pParam;
		}
		else if (ExtraParam.m_bLoadParamValid)
		{
			pLoadParam = KGCommonBufferGetPtr(ExtraParam.m_pLoadParam);
		}
		else
		{
			pRIM->GetDefaultParam(&TempLoadParam);
			KG3DREGION_INFORMER_STATE  TempState;
			hr = pRIM->IEGetState(&TempState);
			_ASSERTE(SUCCEEDED(hr));
			TempLoadParam.nRegionXCount = (INT)(TempState.uInnerTexWidth / TempState.uHowManyPixelsInOneRegion);
			TempLoadParam.nRegionZCount = (INT)(TempState.uInnerTexHeight / TempState.uHowManyPixelsInOneRegion);

			pLoadParam = &TempLoadParam;
		}
		pLoadParam->m_bNeedPreviewTex = TRUE;//��Ȼ����ˢ����ͼ

		LPCTSTR lpLoadPathUse = NULL;
		TCHAR	lpTempBuffer[MAX_PATH];
		if (lpPath)
		{
			lpLoadPathUse = lpPath;
		}
		/*
		else if (ExtraParam.m_Path != _T(""))
				{
					lpLoadPathUse = ExtraParam.m_Path.c_str();
				}*/
		
		else
		{
			hr = pRIM->GetPath(lpTempBuffer, _countof(lpTempBuffer));			
			_ASSERTE(SUCCEEDED(hr));
			lpLoadPathUse = lpTempBuffer;
		}
		_ASSERTE(lpLoadPathUse);

		HRESULT hr = pRIM->IELoadFromFile(lpLoadPathUse, 0, (DWORD_PTR)pLoadParam);
		KG_COM_PROCESS_ERROR(hr);

		if (pLoadParam == pParam)
		{
			*ExtraParam.m_pLoadParam = *pLoadParam;
		}
		/*
		if (lpLoadPathUse == lpPath)
				{
					ExtraParam.m_Path = lpPath;
				}*/
		

		//������֮��Ҫˢ��ʱ��
		FILETIME fTime;
		if(this->GetFileTime(lpLoadPathUse, &fTime))
		{
			ExtraParam.m_LastWriteTime = fTime;
		}

		SynFromInnerToInterface();
	}	

	return TRUE;
Exit0:
	_KG3D_DEBUG_OUTPUT0("��������RIMʧ��\n");
	return FALSE;
}


void KSceneSettingPageRegionSplit::OnBnClickedSsListBnSave()
{
	// TODO: Add your control notification handler code here
	DWORD dwHandle = 0;
	IEKG3DRegionInfoManager* pRIM = this->GetCurRIM(&dwHandle);
	if(!pRIM)
		return;
	_ASSERTE(dwHandle != 0);
	char szFilter[] ="Texture Files(*.bmp;*.jpg;*.png;*.tga;*.Spr;*.DDS)|*.bmp;*.jpg;*.png;*.tga;*.Spr;*.DDS||";
	CFileDialog FileDlg(FALSE, "bmp", NULL, OFN_OVERWRITEPROMPT, szFilter);
	if (IDOK != FileDlg.DoModal())
		return;
	CString csPath = FileDlg.GetPathName();
	if (csPath == _T("")) 
		return;
	HRESULT hr = pRIM->Save((LPCTSTR)csPath, 0);
	if (FAILED(hr))
	{
		MessageBox(_T("δ֪ԭ�򣬱��治�ɹ�"));
	}

	//�����Ѿ�����ı��
	TypeRIMExtraParamContainer::iterator it = m_ParamContainer.find(dwHandle);
	_ASSERTE(it != m_ParamContainer.end());
	TypeRIMExtraParams ExtraParam = it->second;
	ExtraParam.m_bHasSaved = true;
	//ExtraParam.m_Path = (LPCTSTR)csPath;

	//������֮�󣬰�Pathָ�򱣴�ĵط�����������
	hr = this->DelRIM();
	hr = this->AddRIM(csPath/*ExtraParam.m_Path.c_str()*/, KGCommonBufferGetPtr(ExtraParam.m_pLoadParam)
		, ExtraParam.m_NameForRepresent.c_str(), &dwHandle);
	_KG3D_DEBUG_FAIL_REPORT_MSG(SUCCEEDED(hr), ����֮����������벻�ɹ�);
	if (SUCCEEDED(hr))
	{
		_ASSERTE(m_ParamContainer.find(dwHandle) != m_ParamContainer.end());
		m_ParamContainer[dwHandle] = ExtraParam;//Del��ʱ���Erase����Ҫ���»ָ�
	}
}

void KSceneSettingPageRegionSplit::OnBnClickedSsListBnRename()
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_Tree.GetSelectedItem();
	if(!hItem)
		return;
	KSceneCommonParamDialog Dlg;
	std::tstring csName = (LPCTSTR)(m_Tree.GetItemText(hItem));
	int nIndex = Dlg.AddAnParam<std::tstring>(_T("�ֿ�ͼ����"), csName,TRUE);
	if (Dlg.DoModal() != IDOK)
		return;
	if(!Dlg.GetData<std::tstring>(nIndex, csName))
		return;
	DWORD_PTR dwData = m_Tree.GetItemData(hItem);
	TypeRIMExtraParamContainer::iterator it = m_ParamContainer.find(_kg_to_smaller_size_cast<DWORD>(dwData));
	if (it != m_ParamContainer.end())
	{
		TypeRIMExtraParams& ExtraParam = it->second;
		ExtraParam.m_NameForRepresent = csName.c_str();
		m_Tree.SetItemText(hItem, csName.c_str());
	}
}
bool KSceneSettingPageRegionSplit::PromoteSave()
{
	int nRet = MessageBox(_T("ע�⣬�����еķֿ�ͼ��û�б����档\
��ֻ��һ�������������ض�ӦRegion�ķֿ�ͼ�Ĺ��ߣ�����Ψһ��DWSplit.bmp�Ƕ�̬�����ķֿ�ͼ\
��ϵͳ�Զ�����֮�⣬��Ҫ�Լ����������ķֿ�ͼ��")
		, _T("��ʾ"), MB_OKCANCEL | MB_ICONINFORMATION);
	return nRet == IDOK;
}

bool KSceneSettingPageRegionSplit::IsRIMSaved( DWORD dwHandle )
{
	if(m_ParamContainer.empty())
		return false;
	TypeRIMExtraParamContainer::iterator it = m_ParamContainer.find(dwHandle);
	if(it == m_ParamContainer.end())
		return false;
	return it->second.m_bHasSaved;
}
void KSceneSettingPageRegionSplit::OnBnClickedCkShowRegionIndex()
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = this->GetDlgItem(IDC_CK_SHOW_REGION_INDEX);
	KG_PROCESS_ERROR(pWnd);
	{
		BOOL bCheck = KSH::SynACheckWndWithABool(pWnd, FALSE, FALSE);
		if (bCheck)
		{
			EnableOuputRegionIndex(TRUE);
		}
		else
		{
			EnableOuputRegionIndex(FALSE);
		}
	}
Exit0:
	return;
}

void KSceneSettingPageRegionSplit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0)
	{
		IEKG3DRegionInfoManager* pRIM = this->GetCurRIM();
		KG_PROCESS_ERROR(pRIM && g_pGraphicsTool);

		IEKG3DCamera* pCam = NULL;
		HRESULT hr = g_pGraphicsTool->GetSceneMainCamera(&pCam);//���CameraӦ������Զ��Ч�ģ����ܳ���һ������ָ�룬ȴ����һ����Ч��Camera
		KG_COM_PROCESS_ERROR(hr);
		_ASSERTE(pCam);
		D3DXVECTOR3	vecPos;
		KG_COM_PROCESS_ERROR(pCam->GetLookAtPosition(&vecPos));
		DWORD dwData;
		KG_COM_PROCESS_ERROR(pRIM->GetRegionInfoData(vecPos, &dwData));
		TCHAR tszValue[64];
		KG_PROCESS_ERROR(KG_CUSTOM_HELPERS::TValueToString((INT)dwData, tszValue));
		TCHAR tszString[MAX_PATH];
		_stprintf_s(tszString, _T("CurRegionIndex: %s"), tszValue);

		g_pGraphicsTool->IEDrawDebugText(tszString, 1000, &m_dwTimeHandle);
	}
	else if (nIDEvent == 1)
	{
		//�������
		DWORD_PTR dwParam = NULL;
		IEKG3DRegionInfoManager* pRIM = this->GetCurRIM(&dwParam);
		if (pRIM)
		{
			BOOL bChanged = FALSE;
			{
				
				/*
								
																if (it != m_ParamContainer.end())
																{
																	TypeRIMExtraParams& Param = it->second;
																	bChanged = this->IsFileChanged(Param.m_Path.c_str(), Param.m_LastWriteTime);
																}*/
								
				TCHAR tcsTemp[MAX_PATH] = _T("");
				if (SUCCEEDED(pRIM->GetPath(tcsTemp, _countof(tcsTemp))))
				{
					TypeRIMExtraParamContainer::iterator it = m_ParamContainer.find(_kg_to_smaller_size_cast<DWORD>(dwParam));
					if (it != m_ParamContainer.end())
					{
						bChanged = this->IsFileChanged(tcsTemp, it->second.m_LastWriteTime);
					}
					
				}
			}
			if(bChanged)
			{
				this->ReloadRegionMap(NULL, NULL);
			}
		}		
	}
Exit0:
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL KSceneSettingPageRegionSplit::EnableOuputRegionIndex( BOOL bEnable )
{
	if (bEnable)
	{
		this->SetTimer(0, 100, NULL);
	}
	else
	{
		g_pGraphicsTool->DeleteDebugText(m_dwTimeHandle);
		this->KillTimer(0);
	}
	return bEnable;
}
void KSceneSettingPageRegionSplit::OnBnClickedCkShowMask()
{
	// TODO: Add your control notification handler code here
	if (BST_CHECKED == this->IsDlgButtonChecked(IDC_CK_SHOW2))
	{
		this->EnableShowMask(TRUE);
	}
	else
	{
		this->EnableShowMask(FALSE);
	}
}

BOOL KSceneSettingPageRegionSplit::EnableShowMask( BOOL bEnable )
{
	
	
	{
#if 0
		IEKG3DSceneSceneEditor* pSE = KSH::GetEngineInterface<KG3DTYPE_SCENE_SCENE_EDITOR>();
		KG_PROCESS_ERROR(pSE);
		IEKG3DTerrain* pTerrain = NULL;
		KG_COM_PROCESS_ERROR(pSE->GetTerrainEx(&pTerrain));
		if (!bEnable)
		{
			pTerrain->SetRenderState(TerrainRS_EnableShowRegionInfo, FALSE);
		}
		else
		{
			IEKG3DRegionInfoManager* pRIM = this->GetCurRIM();
			KG_PROCESS_ERROR(pRIM);
			KG3DREGION_INFORMER_STATE	State;
			KG_COM_PROCESS_ERROR(pRIM->IEGetState(&State));
			
			pTerrain->SetRenderState(TerrainRS_EnableShowRegionInfo, TRUE);
			pTerrain->SetRenderState(TerrainRS_ShowRegionInfoID, State.dwID);
		}
#else
		IEKG3DScene* pScene = KSH::GetEngineInterfaceEx<IEKG3DScene>();
		KG_PROCESS_ERROR(NULL != pScene);
		if (! bEnable)
		{
			pScene->SetSceneSettingData(scene_setting_enable_render_terrain_mask, FALSE);
		}
		else
		{
			IEKG3DRegionInfoManager* pRIM = this->GetCurRIM();
			KG_PROCESS_ERROR(pRIM);
			BOOL bExistTex = FALSE; INT ID = 0;
			HRESULT hr = pRIM->GetRegionTexState(&bExistTex, &ID, NULL, 0);
			KG_COM_PROCESS_ERROR(hr);
			if (! bExistTex)
			{
				pRIM->GenerateRegionTex(0, (DWORD*)&ID);
			}
			pScene->SetSceneSettingData(scene_setting_enable_render_terrain_mask, ID);
			//��ʵ�ϣ���ͼ��IDû��һ����ȫ����ν�Ŀ�ID��0��֪���ܲ���ȷ�����ᱻ�õ���ԭ��
			//��-1����ID������GetRegionTexState���ص���INT������ͼID�õ���DWORD�������Ϊ-1
			//������Ϊ�ǵ�����õģ�������ɱ��������Ծ�����ν��
		}
#endif
	}
	return TRUE;
Exit0:
	_KG3D_DEBUG_OUTPUT0("EnableShowRegionInfoOnTerrainʧ��\n");
	return FALSE;
}

void KSceneSettingPageRegionSplit::GetMapName( std::tstring& csItemName )
{
	KSceneCommonParamDialog ParamDlg;
	INT nTheFirst = ParamDlg.AddAnParam<std::tstring>(_T("�ֿ�ͼ����"), csItemName,TRUE);
	if (ParamDlg.DoModal() == IDOK)
	{
		ParamDlg.GetData<std::tstring>(nTheFirst, csItemName);
	}
}
HTREEITEM KSceneSettingPageRegionSplit::AddAnRIMToTree( DWORD dwHandle, LPCTSTR pName, LPCTSTR pPath, KG3DREGION_INFORMER_LOADFILE_PARAM* pLoadParam )
{
	HTREEITEM hItem = NULL;
	{
		TypeRIMExtraParamContainer::iterator it = m_ParamContainer.find(dwHandle);
		if (it == m_ParamContainer.end())
		{
			KG_PROCESS_ERROR(pPath);//�����Ļ���Path�Ǳ���ģ���Ȼû��Reload

			TypeRIMExtraParams TempParam;
			if (pLoadParam)
			{
				*TempParam.m_pLoadParam = *pLoadParam;
				TempParam.m_bLoadParamValid = true;//��������£�LoadParam���ⲿ���ģ���Ч
			}
			else
			{
				TempParam.m_bLoadParamValid = false;
			}
		

			if(pName)
				TempParam.m_NameForRepresent = pName;
			else
			{
				TCHAR Temp[MAX_PATH];
				_stprintf_s(Temp, "%u", dwHandle);
				TempParam.m_NameForRepresent = Temp;
			}
			//TempParam.m_Path = pPath;
			TempParam.m_bHasSaved = false;

			FILETIME fileTime;
			if (this->GetFileTime(pPath, &fileTime))
			{
				TempParam.m_LastWriteTime = fileTime;
			}
			else
			{
				TempParam.m_LastWriteTime.dwHighDateTime = TempParam.m_LastWriteTime.dwLowDateTime = 0;
			}

			hItem = m_Tree.InsertItemEx(TempParam.m_NameForRepresent.c_str(), TVI_ROOT, TVI_LAST, (LPARAM*)&dwHandle);///ע��LPARAM = LONG_PTR,���Ժ�DWORD_PTR�ǵȳ���
			KG_PROCESS_ERROR(hItem);

			m_ParamContainer[dwHandle] = TempParam;
		}
		else
		{
			TypeRIMExtraParams& ParamInner = it->second;
			if(pName)
				ParamInner.m_NameForRepresent = pName;
			if(pLoadParam)
			{
				*ParamInner.m_pLoadParam = *pLoadParam;
				ParamInner.m_bLoadParamValid = true;
			}
			//ParamInner.m_Path = pPath ? pPath : _T("");

			FILETIME fileTime;
			if (this->GetFileTime(pPath, &fileTime))
			{
				ParamInner.m_LastWriteTime = fileTime;
			}
			else 
			{
				//����
				//ParamInner.m_LastWriteTime.dwHighDateTime = ParamInner.m_LastWriteTime.dwLowDateTime = 0;
			}

			hItem = m_Tree.InsertItemEx(ParamInner.m_NameForRepresent.c_str(), TVI_ROOT, TVI_LAST, (LPARAM*)&dwHandle);
			if (! hItem)
			{
				m_ParamContainer.erase(dwHandle);
				goto Exit0;
			}
		}		

		return hItem;
	}
Exit0:
	_KG3D_DEBUG_OUTPUT0("AddAnRIMToTreeʧ��");
	return NULL;
}

HRESULT KSceneSettingPageRegionSplit::AddRIM(const CString csLoadPath, KG3DREGION_INFORMER_LOADFILE_PARAM* pLoadParam, LPCTSTR lpName,DWORD* pdwHandle )
{
	IEKG3DRegionInfoManagerTable* pTable = KSH::GetEngineCombinativeInterface<KG3DTYPE_RESOURCE_TABLE_TYPE
		,KG3DTYPE_REGION_INFO_MANAGER>();
	KG_PROCESS_ERROR(pTable);
	DWORD dwHandle = NULL;
	HRESULT hr = pTable->Add(csLoadPath, 0, 0, (DWORD_PTR)pLoadParam, NULL, NULL, &dwHandle);
	KG_COM_PROCESS_ERROR(hr);
	if (pdwHandle)
	{
		*pdwHandle = dwHandle;
	}
	{	
		std::tstring csItemName(_T(""));
		if (! lpName)
		{
			GetMapName(csItemName);
		}
		else
		{
			csItemName = lpName;
		}

		_ASSERTE(dwHandle && pLoadParam);
		HTREEITEM hItem = this->AddAnRIMToTree(dwHandle, csItemName.c_str(), (LPCSTR)csLoadPath, pLoadParam);
		KG_PROCESS_ERROR(hItem);

		m_Tree.SelectItem(hItem);
	}

	//���Tree���ڲ�Table�е�RIM�������
#if defined(_DEBUG) | defined(DEBUG)
	INT nCount = 0;
	hr = pTable->GetCount(&nCount);
	_ASSERTE(SUCCEEDED(hr));
	_ASSERTE(m_Tree.GetCount() == (UINT)nCount);
#endif
	
	return SynFromInnerToInterface();
Exit0:
	MessageBox(pLoadParam->strError.c_str(), _T("���û�гɹ���ԭ������"), MB_OK|MB_ICONWARNING);
	return E_FAIL;
}

HRESULT KSceneSettingPageRegionSplit::DelRIM()
{
	IEKG3DRegionInfoManagerTable* pTable = KSH::GetEngineCombinativeInterface<
		KG3DTYPE_RESOURCE_TABLE_TYPE, KG3DTYPE_REGION_INFO_MANAGER>();
	KG_PROCESS_ERROR(pTable);

	DWORD dwHandle = NULL;
	HTREEITEM hItem = NULL;
	KG_PROCESS_ERROR(this->GetCurRIM(&dwHandle, &hItem));
	if(! IsRIMSaved(dwHandle))
	{
		if (! PromoteSave())
			return S_FALSE;
	}

	HRESULT hr = pTable->Del(dwHandle);
	KG_COM_PROCESS_ERROR(hr);
	_ASSERTE(hItem);
	if (m_Tree.DeleteItem(hItem))
	{
		HTREEITEM hFirstChild = m_Tree.GetChildItem(TVI_ROOT);
		if (hFirstChild)
		{
			m_Tree.SelectItem(hFirstChild);
		}
	}

	m_ParamContainer.erase(dwHandle);
	return S_OK;
Exit0:
	return E_FAIL;
}

BOOL KSceneSettingPageRegionSplit::IsFileChanged( LPCTSTR lpPath, FILETIME OldFileTime )
{
	FILETIME NewFileTime;
	KG_PROCESS_ERROR(GetFileTime(lpPath, &NewFileTime));
	if (NewFileTime.dwHighDateTime != OldFileTime.dwHighDateTime 
		|| NewFileTime.dwLowDateTime != OldFileTime.dwLowDateTime)
	{
		return TRUE;
	}
Exit0:
	return FALSE;
}

BOOL KSceneSettingPageRegionSplit::GetFileTime( LPCTSTR lpPath, LPFILETIME lpFileTime )
{
	_ASSERTE(lpPath && lpFileTime);

	char tcsFullPath[MAX_PATH] = "";
	KG_CUSTOM_HELPERS::UnicodeConvertor Cvt;
	g_GetFullPath(
		tcsFullPath,
#if defined(_UNICODE) | defined(UNICODE)
		Cvt.ToA(lpPath)
#else
		lpPath
#endif
		);

	WIN32_FIND_DATAA	FindedData;

	HANDLE hFind = ::FindFirstFile(lpPath, &FindedData);
	KG_PROCESS_ERROR(hFind);

	*lpFileTime = FindedData.ftLastWriteTime;

	if(hFind)
		::FindClose(hFind);
	return TRUE;
Exit0:
	if(hFind)
		::FindClose(hFind);
	return FALSE;
}

IEKG3DRegionInfoManager* KSceneSettingPageRegionSplit::TypeRIMGetter::GetContainer()
{
	return FuncRIMGetter(m_WParam, NULL);
}