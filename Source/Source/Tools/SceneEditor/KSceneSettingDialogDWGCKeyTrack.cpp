////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KSceneSettingDialogDWGCKeyTrack.cpp
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-2-9 10:22:47
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
// KSceneSettingDialogDWGCKeyTrack.cpp : implementation file
//

#include "stdafx.h"
#include "KSceneSettingDialogDWGCKeyTrack.h"
#include "SceneEditor.h"
#include "IEKG3DEnvEffDynamicWeather.h"
#include "IEKG3DEnvironment.h"
#include "KSceneSceneGridCtrl.h"
#include "KGStringFunctionsEx.h"
#include "KSceneCommonParamDialog.h"
#include "KSceneSettingPropertySheet.h"
#include "IEKG3DRegionInfoManager.h"
#include "KSceneSceneHelpers.h"
#include "KSceneCustomGridCtrl.h"
#include "KSceneCustomTreeCtrl.h"
#include "KSceneSettingPageDW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// KSceneSettingDialogDWGCKeyTrack dialog

IMPLEMENT_DYNAMIC(KDummyPropertyPage, CDialog)

BEGIN_MESSAGE_MAP(KDummyPropertyPage, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

typedef KSceneCustomGridCtrl<KG3DENVEFF_DWGCKEY_HANDLE> TypeGridCtrl;
typedef KSceneCustomTreeCtrl<KG3DENVEFF_HANDLE> TypeTreeCtrl;

void KDummyPropertyPage::OnShowWindow( BOOL bShow, UINT nStatus )
{
	CDialog::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		this->OnSetActive();
	}
	else
	{
		this->OnKillActive();
	}
}

IMPLEMENT_DYNAMIC(KSceneSettingDialogDWGCKeyTrack, KSceneSettingDialogDWGCKeyTrack::TypeBase)


#define  CM_REFLESH_GRID (WM_USER + 301)
#define  CM_CHECK_WEATHER_TREE_GRID_MATCH (WM_USER + 302)

enum
{
	DWGC_KT_EditMode_Normal = 0,
	DWGC_KT_EditMode_SetKey = 1,
	DWGC_KT_EditMode_EditKey = 2,
	DWGC_KT_EditMode_EditKeyDirect = 3,
};

enum
{
	DWGC_KT_CopyMode_ByValue = 0,
	DWGC_KT_CopyMode_ByRef = 1,
};

KSceneSettingDialogDWGCKeyTrack::KSceneSettingDialogDWGCKeyTrack()
: TypeBase(KSceneSettingDialogDWGCKeyTrack::IDD)
, m_bIsDWPlaying(false)
, m_nCopyMode(DWGC_KT_CopyMode_ByValue)
, m_pGridCtrl(NULL)
, m_pTree(NULL)
, m_hWndToPropertySheet(0)
{
	m_nFlag = 0;

	try
	{
		if(NULL == m_pGridCtrl)
		{
			m_pGridCtrl = new TypeGridCtrl();
			_ASSERTE(NULL != m_pGridCtrl);
		}
		if (NULL == m_pTree)
		{
			m_pTree = new TypeTreeCtrl();
			_ASSERTE(NULL != m_pTree);
		}
	}
	catch (...)
	{
		
	}
}

KSceneSettingDialogDWGCKeyTrack::~KSceneSettingDialogDWGCKeyTrack()
{
	if (m_bIsDWPlaying)
	{
		IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
		if (pDW)
		{
			pDW->StopQuickPlay(NULL);
		}
	}
	SAFE_DELETE(m_pGridCtrl);	
	SAFE_DELETE(m_pTree);
}

void KSceneSettingDialogDWGCKeyTrack::DoDataExchange(CDataExchange* pDX)
{
	TypeBase::DoDataExchange(pDX);
	if (NULL != m_pTree)
	{
		DDX_Control(pDX, IDC_DWGC_TREE, *m_pTree);
	}
	if(NULL != m_pGridCtrl)
	{
		DDX_Control(pDX, IDC_GRID, *m_pGridCtrl);
	}
	DDX_Control(pDX, IDC_ED_DWGC_REGION, m_edRegion);
	DDX_Control(pDX, IDC_DWGC_HOUR, m_edHour);
	DDX_Control(pDX, IDC_BN_DWGC_PLAY, m_cbPlay);
	DDX_Radio(pDX, IDC_RADIO1, m_nCopyMode);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
}

int KSceneSettingDialogDWGCKeyTrack::Update()
{
	return 1;
}
BEGIN_MESSAGE_MAP(KSceneSettingDialogDWGCKeyTrack, KSceneSettingDialogDWGCKeyTrack::TypeBase)
	ON_BN_CLICKED(IDC_BN_DWGC_ROW_ADD, &KSceneSettingDialogDWGCKeyTrack::OnBnClickedBnDwgcRowAdd)
	ON_BN_CLICKED(IDC_BN_DWGC_ROW_DEL, &KSceneSettingDialogDWGCKeyTrack::OnBnClickedBnDwgcRowDel)
	ON_BN_CLICKED(IDC_BN_DWGC_ROW_MOD, &KSceneSettingDialogDWGCKeyTrack::OnBnClickedBnDwgcRowMod)
	ON_BN_CLICKED(IDC_BN_DWGC_ROW_DOWN, &KSceneSettingDialogDWGCKeyTrack::OnBnClickedBnDwgcRowDown)
	ON_BN_CLICKED(IDC_SS_BN_SETKEY, &KSceneSettingDialogDWGCKeyTrack::OnBnClickedSsBnSetKey)
	ON_BN_CLICKED(IDC_BN_DWGC_PLAY, &KSceneSettingDialogDWGCKeyTrack::OnBnClickedBnDwgcPlay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SS_BN_HELP, &KSceneSettingDialogDWGCKeyTrack::OnBnClickedSsBnHelp)
	ON_BN_CLICKED(IDC_SS_BN_SETALLKEY, &KSceneSettingDialogDWGCKeyTrack::OnBnClickedSsBnSetAllKey)
	ON_BN_CLICKED(IDC_SS_BN_COPY_PRE_KEY, &KSceneSettingDialogDWGCKeyTrack::OnBnClickedSsBnCopyPrevKey)
	ON_BN_CLICKED(IDC_SS_BN_DEL_ALL_KEY, &KSceneSettingDialogDWGCKeyTrack::OnBnClickedSsBnDelAllKey)
	ON_WM_HSCROLL()
	
END_MESSAGE_MAP()


// KSceneSettingDialogDWGCKeyTrack message handlers
BOOL KSceneSettingDialogDWGCKeyTrack::OnInitDialog()
{
	TypeBase::OnInitDialog();

	// TODO:  Add extra initialization here
#if defined(_DEBUG) | defined(DEBUG)
#if defined(KG_ENABLE_TDD)
	KSceneCustomGridCtrlTest	TestGridCtrl;
	KG_USE_ARGUMENT(TestGridCtrl);
	KSceneCustomTreeCtrlTester  TestTreeCtrl;
	KG_USE_ARGUMENT(TestTreeCtrl);
#endif
#endif

	_ASSERTE(NULL != m_pTree);
	m_pTree->SetItemHeight(16);
	int nTreeItemHeight = (int)m_pTree->GetItemHeight();
	{///���������ƶ�һ��㣬����Grid��һ��
		CRect rect;
		m_pTree->GetWindowRect(rect);
		this->ScreenToClient(rect);
		if(rect.Width() > nTreeItemHeight)
			rect.top += nTreeItemHeight;
		m_pTree->MoveWindow(rect, TRUE);
	}
 
	///��ʼ��Grid
	_ASSERTE(NULL != m_pGridCtrl);
	{
		KSceneSceneGridCtrl::InitAppearanceStruct InitStruct;
		InitStruct.m_bEditEnable = true;
		InitStruct.m_bInitBasicTitles = true;
		InitStruct.m_bEnableHightLighFixed = false;
		InitStruct.m_csCornerDescription = _T("R/T");
		InitStruct.m_nColumnCount = KG3DENVEFFTI_HOUR_IN_A_DAY + 1;
		InitStruct.m_nDefRowHeight = nTreeItemHeight;
		InitStruct.m_nDefColumnWidth = 25;
		InitStruct.m_nFixedColumnCount = 1;
		InitStruct.m_nFixedColumnWidth = 100;
		InitStruct.m_nFixedRowCount = 1;
		InitStruct.m_nRowCount = 1;
		InitStruct.m_bSingleRowSelection = 1;
		InitStruct.m_bSingleColomnSelection = 1;
		InitStruct.m_bEnableColResize = false;
		InitStruct.m_bEnableRowResize = false;
		m_pGridCtrl->InitAppearance(&InitStruct);

		HWND hWndGrid = NULL;
		this->GetDlgItem(IDC_GRID, &hWndGrid);
		if (NULL != hWndGrid)
		{
			CRect rect;
			::GetWindowRect(hWndGrid, &rect);
			this->ScreenToClient(&rect);

			m_pGridCtrl->MoveWindow(&rect, TRUE);
		}
	}

	//Slider�ؼ�
	m_Slider.SetRange(KG3DENVEFFTI_HOUR_MIN, KG3DENVEFFTI_HOUR_MAX);
	m_Slider.SetPos(KG3DENVEFFTI_HOUR_MIN);
	
	for (int i = KG3DENVEFFTI_HOUR_MIN + 1; i < KG3DENVEFFTI_HOUR_MAX; ++i)
	{
		m_Slider.SetTic(i);
	}
	m_Slider.SetTicFreq(1);

	//Tree����ѡ��ͼ��
	m_ImageList.Create(IDB_BITMAP_ARROW, 16, 1, RGB(255,255,255));
	m_pTree->SetImageList(&m_ImageList, TVSIL_NORMAL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL KSceneSettingDialogDWGCKeyTrack::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	
	HRESULT hr = E_FAIL;

	//Ϊ������չ�����Tree���ұ�Grid��Ҫ�ȸ���һ��״̬����Ȼһ���أ�״̬������
	KeyTrackStateStruct TempStateStruct = m_KeyTrackStateStruct;

	IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
	KG_PROCESS_ERROR(pDW);

	//������ߵ�Tree���ұߵ�Grid
	{
		//ж�ػص�����
		BindCallBacks(FALSE);
		ClearContents();///ע��������������˳��

		ReloadAllEnvEff();//��Reload��ʱ��ᴥ��OnSeletect���ͻ�����ұߵ�Grid
	}
	
	//һЩ��������ĵİ�ť
	static INT UnModifiableIDs[] = {IDC_SS_BN_SETKEY, IDC_ED_DWGC_REGION, IDC_DWGC_HOUR};
	{
		KSCENEEDITOR_HELPERCLASSES::LockChildWindows(this->GetSafeHwnd(), FALSE, UnModifiableIDs, _countof(UnModifiableIDs));
		UpdateData(FALSE);
	}

	{
		m_cbPlay.SetWindowText(_T("����"));
		m_cbPlay.SetCheck(BST_UNCHECKED);
		_ASSERTE(false == m_bIsDWPlaying);
	}

	//�ڱ༭�ؼ�֡״̬���棬��ֹ�ؼ�֡�Ĳ���
	pDW->EnablePlay(FALSE);

	BindCallBacks(TRUE);

	//���ݱ�����State��չ����ߵ�Tree���ұߵ�Grid
	if(TempStateStruct.m_bIsValid)
	{
		_ASSERTE(NULL != m_pTree);
		HTREEITEM hItem = static_cast<TypeTreeCtrl*>(m_pTree)->GetHandleOfBigData(TempStateStruct.m_Handle);
		if(hItem)
		{
			m_pTree->EnsureVisible(hItem);
			m_pTree->SelectItem(hItem);	//�ᴥ��SelChange�Ӷ�ˢ��֡��
		}
		INT nRowToSelectInGrid = -1;
		for (int i = 1; i < m_pGridCtrl->GetRowCount(); ++i)
		{
			KG3DENVEFF_DWGCKEY_HANDLE* p = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(i, 0);
			if (p)
			{
				if(p->m_Region == TempStateStruct.m_KeyPos.m_Region)
				{
					nRowToSelectInGrid = i;
					break;
				}
			}
		}
		INT nHourToSelect = TempStateStruct.m_KeyPos.m_Hour;
		if (nRowToSelectInGrid > 0)
		{
			m_pGridCtrl->SetSelectedRange(nRowToSelectInGrid, nHourToSelect
				, nRowToSelectInGrid + 1, nHourToSelect + 1, TRUE);
		}

		{
			CString TempStr;
			KG_CUSTOM_HELPERS::TValueToString(nRowToSelectInGrid, TempStr);
			m_edRegion.SetWindowText(TempStr);
			KG_CUSTOM_HELPERS::TValueToString(nHourToSelect, TempStr);
			m_edHour.SetWindowText(TempStr);
		}
	}
	
	m_pGridCtrl->ShowWindow(SW_SHOW);
	
	this->SetTimer(0, 500, NULL);	//��Timmer��ͬ��Slider��DW
	this->SetTimer(1, 500, NULL);

	return TypeBase::OnSetActive();
Exit0:
	BindCallBacks(TRUE);
	BOOL bResult = TypeBase::OnSetActive();
	KSCENEEDITOR_HELPERCLASSES::LockChildWindows(this->GetSafeHwnd(), TRUE, UnModifiableIDs, _countof(UnModifiableIDs));///���������Ӵ���
	MessageBox("Dynamic Weather������");
	return bResult;
}

BOOL KSceneSettingDialogDWGCKeyTrack::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class
	BindCallBacks(FALSE);
	ClearContents();

	IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
	if (pDW)
	{
		pDW->StopQuickPlay(NULL);
		pDW->EnablePlay(TRUE);
	}
	m_bIsDWPlaying = false;

	this->KillTimer(0);
	this->KillTimer(1);

	m_KeyTrackStateStruct.m_bIsValid = TRUE;

	return TypeBase::OnKillActive();
}

VOID KSceneSettingDialogDWGCKeyTrack::BindCallBacks( BOOL bBind )
{
	BindGridCallbacks(bBind);
	BindTreeCallbacks(bBind);
}
VOID KSceneSettingDialogDWGCKeyTrack::BindTreeCallbacks( BOOL bBind )
{
	if (bBind)
	{
		_ASSERTE(NULL != m_pTree);
		m_pTree->SetEventHandler(this);
	}
	else
	{
		_ASSERTE(NULL != m_pTree);
		m_pTree->SetEventHandler(NULL);
	}
}
VOID KSceneSettingDialogDWGCKeyTrack::BindGridCallbacks( BOOL bBind )
{
	if (bBind)
	{
		m_pGridCtrl->SetEventHandler(this);
	}
	else
	{
		m_pGridCtrl->SetEventHandler(NULL);
	}
}
void KSceneSettingDialogDWGCKeyTrack::GridReloadKey( INT nRow, INT nColumn)
{
	_ASSERTE(nRow >= 1 && nRow < m_pGridCtrl->GetRowCount());

	KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(nRow, 0);
	KG_PROCESS_ERROR(pKeyHandle);
	{
		_ASSERTE(NULL != m_pTree);
		HTREEITEM hItemSelected = static_cast<TypeTreeCtrl*>(m_pTree)->GetSelectedItem();
		KG3DENVEFF_HANDLE* pEnvEffHandle = static_cast<TypeTreeCtrl*>(m_pTree)->GetItemBigData(hItemSelected);
		KG_PROCESS_ERROR(pEnvEffHandle);

		m_pGridCtrl->ClearRow(nRow, FALSE);

		IEKG3DEnvEffDynamicWeather* pDW = GetDynamicWeather();
		KG_PROCESS_ERROR(pDW);

		INT Region = pKeyHandle->m_Region;
		
		KG3DENVEFF_DWGCKEY_HANDLE KeyHandle;
		size_t KeyInfo = 0;
		HRESULT hr = pDW->TL_GetFirstKeyHandleOfRegion(*pEnvEffHandle, Region, &KeyHandle, reinterpret_cast<LPVOID>(&KeyInfo));
		while (SUCCEEDED(hr))
		{
			_ASSERTE(KeyHandle.m_Region == Region);
			this->SetGridKey(nRow, KeyHandle.m_Hour, KeyInfo);

			hr = pDW->TL_GetNextKeyHandleOfRegion(*pEnvEffHandle, Region, KeyHandle, &KeyHandle, reinterpret_cast<LPVOID>(&KeyInfo));
		}
	}

Exit0:
	m_pGridCtrl->RedrawRow(nRow);
	return;	
}

BOOL KSceneSettingDialogDWGCKeyTrack::IsEditStringValid( KSceneSceneGridCtrl& ctrl, CPoint point, LPCTSTR pStrEdit )
{
	INT nRow = point.x;
	INT nColumn = point.y;

	size_t uStringLen = _tcslen(pStrEdit);
	HRESULT hr = E_FAIL;
	KG_PROCESS_ERROR(uStringLen <= 20);
	{
		KG_PROCESS_ERROR(0 == _tcscmp(pStrEdit, _T("")));//Edit�����ֻ����Del
		this->GridDelKey(nRow, nColumn);	
		return TRUE;
	}
Exit0:
	return FALSE;
}

BOOL KSceneSettingDialogDWGCKeyTrack::IsEditCharValid( KSceneSceneGridCtrl& ctrl, CPoint point, UINT uChar )
{
	return FALSE;
}

BOOL KSceneSettingDialogDWGCKeyTrack::GridDelKey( INT Row, INT Column )
{
	HRESULT hr = E_FAIL;
	{
		IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
		KG_PROCESS_ERROR(pDW);

		KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(Row, 0);
		KG_PROCESS_ERROR(pKeyHandle);

		_ASSERTE(NULL != m_pTree);
		HTREEITEM hItemSelected = static_cast<TypeTreeCtrl*>(m_pTree)->GetSelectedItem();
		KG3DENVEFF_HANDLE* pEnvEffHandle = static_cast<TypeTreeCtrl*>(m_pTree)->GetItemBigData(hItemSelected);
		KG_PROCESS_ERROR(pEnvEffHandle);

		hr = pDW->TL_DelKey(*pEnvEffHandle, KG3DENVEFF_DWGCKEY_HANDLE(pKeyHandle->m_Region, Column));
		KG_COM_PROCESS_ERROR(hr);

		GridReloadKey(Row, Column);

		return TRUE;
	}
Exit0:
	return FALSE;
}

BOOL KSceneSettingDialogDWGCKeyTrack::IsLButtomDoubleClickValid( KSceneSceneGridCtrl& ctrl, CPoint CellPos )
{
	INT nRow = CellPos.x;
	INT nColumn = CellPos.y;
	_KG3D_DEBUG_SELF_OUTPUT2("���������Cell: %d, %d", CellPos.x, CellPos.y);
	
	{
		IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
		KG_PROCESS_ERROR(pDW);

		KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl&>(ctrl).GetItemBigData(nRow, 0);
		KG_PROCESS_ERROR(pKeyHandle);
		INT nHour = nColumn;

		_ASSERTE(NULL != m_pTree);
		HTREEITEM hItemSelected = static_cast<TypeTreeCtrl*>(m_pTree)->GetSelectedItem();
		KG3DENVEFF_HANDLE* pEnvEffHandle = static_cast<TypeTreeCtrl*>(m_pTree)->GetItemBigData(hItemSelected);
		KG_PROCESS_ERROR(pEnvEffHandle);

		HRESULT hr = pDW->KeyToState(KG3DENVEFF_DWGCKEY_HANDLE(pKeyHandle->m_Region, nHour), *pEnvEffHandle);
		KG_COM_PROCESS_ERROR(hr);

		_ASSERTE(this->m_pGridCtrl->GetItemText(nRow, nColumn) != _T(""));
	}
Exit0:
	return FALSE;
}

void KSceneSettingDialogDWGCKeyTrack::OnGridEndEdit( NMHDR *pNotifyStruct,  LRESULT* pResult )
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	CString tempString = m_pGridCtrl->GetItemText(pItem->iRow, pItem->iColumn);
	static int nInValidStringLength = std::numeric_limits<INT>::digits10;
	int nStringLength = tempString.GetLength();
	if (nStringLength > nInValidStringLength)
	{
		*pResult = -1;
		return;
	}
	int nRet = _ttoi((LPCTSTR)tempString);

	*pResult = 0;
}

BOOL KSceneSettingDialogDWGCKeyTrack::OnItemExpanded( KSceneSceneTreeCtrl& tree, HTREEITEM hItemExpander )
{
	return TRUE;
}

BOOL KSceneSettingDialogDWGCKeyTrack::OnItemDelete( KSceneSceneTreeCtrl& tree, HTREEITEM hItemDelete )
{
	return TRUE;
}

BOOL KSceneSettingDialogDWGCKeyTrack::OnItemInserted( KSceneSceneTreeCtrl& tree, HTREEITEM hItemInserted )
{
	HTREEITEM hParentItem = tree.GetParentItem(hItemInserted);
	if (!hParentItem)
		hParentItem = TVI_ROOT;
	KG_PROCESS_SUCCESS(hParentItem != TVI_ROOT && !tree.IsItemExpanded(hParentItem));
	{
		INT nPos = tree.GetItemExpandedPos(hItemInserted);
		KG_PROCESS_SUCCESS(nPos < 0);

		KSceneSettingDialogDWGCKeyTrack* pDialog = this;
		INT nRowCount = pDialog->m_pGridCtrl->GetRowCount();
		_ASSERTE(nPos <= nRowCount && pDialog);
		if (nPos == nRowCount)///ע��Grid�ĵ�һ���Ǳ���
		{	
			pDialog->m_pGridCtrl->AddRow(-1);		
		}
		else
		{
			pDialog->m_pGridCtrl->AddRow(nPos);
		}
		_ASSERTE(tree.GetExpandedTreeItemCount()+1 == pDialog->m_pGridCtrl->GetRowCount());

		struct TempVisitor : TreeItemVisitor 
		{
			KSceneSettingDialogDWGCKeyTrack* m_pTrack;
			virtual HRESULT Accept(HTREEITEM hItem)
			{
				m_pTrack->OnEnumTree(hItem);
				return S_OK;
			}
		};
		TempVisitor visitor;
		visitor.m_pTrack = this;

		tree.TraverseItemsExpanded(hParentItem, visitor);
	}

Exit1:
	return TRUE;
}

VOID KSceneSettingDialogDWGCKeyTrack::OnEnumTree( HTREEITEM hItem )
{
	return;
}

BOOL KSceneSettingDialogDWGCKeyTrack::OnItemSelectChange( KSceneSceneTreeCtrl& tree, HTREEITEM hItemSeleted, HTREEITEM hItemLastSelect )
{
	_ASSERTE(NULL != m_pGridCtrl);
	//�õ���Ҫ�Ŀ�����
	TypeTreeCtrl* pTree = static_cast<TypeTreeCtrl*>(&tree);
	_ASSERTE(pTree);
	KG3DENVEFF_HANDLE* pCurTreeItemEnvHandle = pTree->GetItemBigData(hItemSeleted);
	KG_PROCESS_ERROR(pCurTreeItemEnvHandle);

	{

		//��ѡ���¼����
		{
			m_KeyTrackStateStruct.m_Handle = *pCurTreeItemEnvHandle;
		}


		IEKG3DEnvEffDynamicWeather* pDW = GetDynamicWeather();
		KG_PROCESS_ERROR(pDW);

		m_pGridCtrl->SetRowCount(1);//���������������������

		KG_PROCESS_ERROR(pDW->IsEffectDWGCAvailable(*pCurTreeItemEnvHandle));

		std::map<INT, INT> MapRegionRow;	//����������Key��ʱ�������ҵ���Ӧ����
		//��Grid����ߵ�һ�У�����Region�����кţ�����Data������־	
		{
			ULONG uRegionsCount = pDW->TL_GetRegionIndicesCount();
			_ASSERTE(uRegionsCount < 256);
			KG_PROCESS_ERROR(m_pGridCtrl->SetRowCount((int)uRegionsCount + 1));

			INT nRegionIndex = 0;
			ULONG AddCount = 0;
			TCHAR	tczItemText[MAX_PATH];
			ZeroMemory(tczItemText, sizeof(tczItemText));
			HRESULT hr = pDW->TL_GetFirstRegionIndex(&nRegionIndex);
			while(SUCCEEDED(hr))
			{
				++AddCount;
				_ASSERTE(AddCount <= uRegionsCount);

				_stprintf_s(tczItemText, "R_%u", nRegionIndex);

				m_pGridCtrl->SetItemText(AddCount, 0, tczItemText);
				static_cast<TypeGridCtrl*>(m_pGridCtrl)->SetItemBigData(AddCount, 0, KG3DENVEFF_DWGCKEY_HANDLE(nRegionIndex, KG3DENVEFFTI_HOUR_INVALID));

				MapRegionRow[nRegionIndex] = AddCount;

				hr = pDW->TL_GetNextRegionIndex(nRegionIndex, &nRegionIndex);
			}
		}

		//����Key
		{
			KG3DENVEFF_DWGCKEY_HANDLE	KeyHandle;
			size_t KeyInfo = 0;//ʵ����KeyTable�ڲ���������к�

			HRESULT hr = pDW->TL_GetFirstKeyHandle(*pCurTreeItemEnvHandle, &KeyHandle, (LPVOID)(&KeyInfo));
			while (SUCCEEDED(hr))
			{
				//�ҵ�Ҫ�����Row
				INT nRowToInsert = MapRegionRow[KeyHandle.m_Region];
				_ASSERTE(nRowToInsert > 0);

#if defined(DEBUG) | defined(_DEBUG)
				{
					KG3DENVEFF_DWGCKEY_HANDLE* pKeyRowHead = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(nRowToInsert, 0);
					_ASSERTE(pKeyRowHead);
					_ASSERTE(pKeyRowHead->m_Region == KeyHandle.m_Region);
				}
#endif
				SetGridKey(nRowToInsert,  KeyHandle.m_Hour, KeyInfo);

				hr = pDW->TL_GetNextKeyHandle(*pCurTreeItemEnvHandle, KeyHandle, &KeyHandle, (LPVOID)(&KeyInfo));
			}
			m_pGridCtrl->RedrawWindow();
		}
		//һЩ����Ľ������
		_ASSERTE(NULL != m_pTree);
		KG3DENVEFF_HANDLE* p = static_cast<TypeTreeCtrl*>(m_pTree)->GetItemBigData(hItemSeleted);
		if (p)
		{			
			CString Cs = _T("R/T ");
			Cs += KSH::GetRTTITypeName(*p);

			m_pGridCtrl->SetItemText(0,0,Cs);
			m_pGridCtrl->RedrawCell(0,0);
		}

	}

Exit0:
	return TRUE;
}

BOOL KSceneSettingDialogDWGCKeyTrack::IsLButtonDownValid( KSceneSceneGridCtrl& ctrl, CPoint CellPos )
{
	ctrl.HighLightRow(static_cast<INT>(CellPos.x));

	//��¼ѡ���λ��
	KG3DENVEFF_DWGCKEY_HANDLE* p = static_cast<TypeGridCtrl&>(ctrl).GetItemBigData(CellPos.x, 0);

	if (NULL != p)
	{
		this->m_KeyTrackStateStruct.m_KeyPos = KG3DENVEFF_DWGCKEY_HANDLE(p->m_Region, CellPos.y);
	}


	if (CellPos.y == 0)//ѡ��ǰ��Ļ���SetRegion
	{
		if (NULL != p)
		{
			IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
			pDW->SetCurRegion(p->m_Region, ! this->IsBlendingNeededToBeShown());
		}
	}

	return TRUE;
}

BOOL KSceneSettingDialogDWGCKeyTrack::OnItemExpanding( KSceneSceneTreeCtrl& tree, HTREEITEM hItemExpanding )
{
	tree.SelectItem(hItemExpanding);
	return TRUE;
}

BOOL KSceneSettingDialogDWGCKeyTrack::IsCopyValid( KSceneSceneGridCtrl& ctrl, CPoint CellPos )
{
	KG_PROCESS_ERROR(ctrl.GetItemText(CellPos.x, CellPos.y) != _T(""));
	{
		KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl&>(ctrl).GetItemBigData(CellPos.x, 0);
		KG_PROCESS_ERROR(pKeyHandle);

		HTREEITEM hItemSelected = static_cast<TypeTreeCtrl*>(m_pTree)->GetSelectedItem();
		KG3DENVEFF_HANDLE* pEnvEffHandle = static_cast<TypeTreeCtrl*>(m_pTree)->GetItemBigData(hItemSelected);
		KG_PROCESS_ERROR(pEnvEffHandle);

		KeyTrackCopyStruct& CopyStruct = this->GetCopyStruct();
		CopyStruct.m_HandleSrc = *pEnvEffHandle;
		CopyStruct.m_KeySrc = KG3DENVEFF_DWGCKEY_HANDLE(pKeyHandle->m_Region, CellPos.y);
		CopyStruct.m_bIsValid = true;
		CopyStruct.m_bCopyOrCut = true;
		return FALSE;
	}
Exit0:
	this->GetCopyStruct().m_bIsValid = false;
	return FALSE;//����Ĭ�ϵ�OnEditCopy���У�������ͬ
}

BOOL KSceneSettingDialogDWGCKeyTrack::IsCutValid( KSceneSceneGridCtrl& ctrl, CPoint CellPos )
{
	KG_PROCESS_ERROR(ctrl.GetItemText(CellPos.x, CellPos.y) != _T(""));
	{
		KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl&>(ctrl).GetItemBigData(CellPos.x, 0);
		KG_PROCESS_ERROR(pKeyHandle);

		_ASSERTE(NULL != m_pTree);
		HTREEITEM hItemSelected = static_cast<TypeTreeCtrl*>(m_pTree)->GetSelectedItem();
		KG3DENVEFF_HANDLE* pEnvEffHandle = static_cast<TypeTreeCtrl*>(m_pTree)->GetItemBigData(hItemSelected);
		KG_PROCESS_ERROR(pEnvEffHandle);

		KeyTrackCopyStruct& CopyStruct = this->GetCopyStruct();
		CopyStruct.m_HandleSrc = *pEnvEffHandle;
		CopyStruct.m_KeySrc = KG3DENVEFF_DWGCKEY_HANDLE(pKeyHandle->m_Region, CellPos.y);
		CopyStruct.m_bIsValid = true;
		CopyStruct.m_bCopyOrCut = false;

		return FALSE;
	}
Exit0:
	this->GetCopyStruct().m_bIsValid = false;
	return FALSE;//����Ĭ�ϵ�OnEditCopy����
}

BOOL KSceneSettingDialogDWGCKeyTrack::IsPasteValid( KSceneSceneGridCtrl& ctrl, CPoint CellPos )
{
	IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
	KG_PROCESS_ERROR(pDW);
	{
		KeyTrackCopyStruct& CopyStruct = this->GetCopyStruct();
		KG_PROCESS_ERROR(CopyStruct.m_bIsValid);

		_ASSERTE(NULL != m_pTree);

		_ASSERTE(NULL != m_pTree);
		HTREEITEM hItemSelected = static_cast<TypeTreeCtrl*>(m_pTree)->GetSelectedItem();
		KG3DENVEFF_HANDLE* pHandleTo = static_cast<TypeTreeCtrl*>(m_pTree)->GetItemBigData(hItemSelected);
		KG_PROCESS_ERROR(pHandleTo);

		KG3DENVEFF_DWGCKEY_HANDLE* pRegionHandle = static_cast<TypeGridCtrl&>(ctrl).GetItemBigData(CellPos.x, 0);
		KG_PROCESS_ERROR(pRegionHandle);

		this->UpdateData();
		INT CopyByValueOrByRef = this->m_nCopyMode == DWGC_KT_CopyMode_ByValue ? TRUE : FALSE;
		INT Hour = CellPos.y;
		HRESULT hr = pDW->TL_CopyKeyTo(CopyStruct.m_HandleSrc
			, CopyStruct.m_KeySrc
			, *pHandleTo
			, KG3DENVEFF_DWGCKEY_HANDLE(pRegionHandle->m_Region, Hour)
			, CopyByValueOrByRef);
		if(FAILED(hr))
		{
			this->MessageBox(_T("�ؼ�֡ճ��ʧ�ܣ���������\n\
								   1.λ�����Ѿ����ڹؼ�֡��Ҫ��ɾ������Ϊ�����ÿ����Ĵ��ڣ����򲻻��Զ�����ԭ�йؼ�֡\n\
								   2.���ܿ������͵Ĺؼ�֡����ͬ���͵Ŀ����ڲ�ͬ�Ĺؼ�֡�����໥����\n\
								   3.λ�ò��Ϸ������Region��Hour"));
		}
		else
		{
			if (! CopyStruct.m_bCopyOrCut)//�����Cut�Ļ���ɾ��ԭ����
			{
				pDW->TL_DelKey(CopyStruct.m_HandleSrc, CopyStruct.m_KeySrc);

			}
		}


		this->GridReloadKey(CellPos.x, CellPos.y);
		return FALSE;
	}
Exit0:
	this->GetCopyStruct().m_bIsValid = false;
	this->GridReloadKey(CellPos.x, CellPos.y);
	return FALSE;
}

LRESULT KSceneSettingDialogDWGCKeyTrack::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{	
	return TypeBase::WindowProc(message, wParam, lParam);
}

VOID KSceneSettingDialogDWGCKeyTrack::ClearContents()
{
	_ASSERTE(NULL != m_pGridCtrl);
	m_pGridCtrl->SetRowCount(1);
	_ASSERTE(NULL != m_pTree);
	m_pTree->DeleteAllItems();
	m_vecEnvEffHandle.clear();
	{
		KG3DENVEFF_HANDLE BlankHandle;
		BlankHandle.m_dwType = 0;
		BlankHandle.m_nIndex = 0;
		m_vecEnvEffHandle.push_back(BlankHandle);///��Ϊ��Ч����
	}
}
IEKG3DEnvEffDynamicWeather* KSceneSettingDialogDWGCKeyTrack::GetDynamicWeather()
{
	return KSH::GetEngineInterface<KG3DTYPE_ENVEFF_DYNAMIC_WEATHER>();
}

//////////////////////////////////////////////////////////////////////////


VOID KSceneSettingDialogDWGCKeyTrack::SetGridKey( int nRow, int nColumn, INT nValue, LPCTSTR pText/* = NULL*/ )
{
	_ASSERTE(NULL != m_pGridCtrl);
	_ASSERTE(nRow >= 0 && nRow < m_pGridCtrl->GetRowCount() && nColumn >= 0 && nColumn < m_pGridCtrl->GetColumnCount());
	if (pText)
	{
		m_pGridCtrl->SetItemText(nRow, nColumn, pText);
	}
	else
	{
		TCHAR StringBuffer[64];
		if (KG_CUSTOM_HELPERS::TValueToString(nValue, StringBuffer, _countof(StringBuffer)))
		{
			m_pGridCtrl->SetItemText(nRow, nColumn, StringBuffer);
		}
		
	}	
	m_pGridCtrl->SetItemData(nRow, nColumn, (LPARAM)nValue);
	m_pGridCtrl->MarkCell(nRow, nColumn);
}

VOID KSceneSettingDialogDWGCKeyTrack::SetGridKey( INT Row, INT Column, size_t Info )
{
	_ASSERTE(NULL != m_pGridCtrl);
	TCHAR tczItemText[MAX_PATH];
	KG_CUSTOM_HELPERS::TValueToString((UINT)Info, tczItemText);
	m_pGridCtrl->SetItemText(Row, Column, tczItemText);
	m_pGridCtrl->MarkCell(Row, Column);
}

void KSceneSettingDialogDWGCKeyTrack::OnBnClickedBnDwgcRowAdd()
{
	// TODO: Add your control notification handler code here
	KSceneCommonParamDialog CTDialog;
	INT nRegionIndex = 0;
	INT nParamIndex = CTDialog.AddAnParam(_T("���(1-255)��"), nRegionIndex,TRUE);
	if (CTDialog.DoModal() != IDOK)
		return;  
	if (! CTDialog.GetData(nParamIndex, nRegionIndex))
		return;
	IEKG3DEnvEffDynamicWeather* pDW = GetDynamicWeather();
	if(! pDW)
		return;
	pDW->TL_AddRegionIndex(nRegionIndex);
	//FuncTreeItemSelectedChangeCallback(&m_Tree, m_Tree.GetSelectedItem(), NULL, reinterpret_cast<DWORD_PTR>(this));//ǿ��Ҫ���ض�
	_ASSERTE(NULL != m_pTree);
	this->OnItemSelectChange(*m_pTree, m_pTree->GetSelectedItem(), NULL);
}

void KSceneSettingDialogDWGCKeyTrack::OnBnClickedBnDwgcRowDel()
{
	_ASSERTE(NULL != m_pGridCtrl);
	INT Row = m_pGridCtrl->GetSelectedMinRow();
	KG_PROCESS_ERROR(Row > 0 && Row < m_pGridCtrl->GetRowCount());//��һ���ǲ���ɾ����
	{
		KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(Row, 0);
		KG_PROCESS_ERROR(pKeyHandle);

		IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
		KG_PROCESS_ERROR(pDW);

		HRESULT hr = pDW->TL_DelRegionIndex(pKeyHandle->m_Region);
		KG_COM_PROCESS_ERROR(hr);

		_ASSERTE(NULL != m_pTree);
		this->OnItemSelectChange(*m_pTree, m_pTree->GetSelectedItem(), NULL);
	}
Exit0:
	return;
}

void KSceneSettingDialogDWGCKeyTrack::OnBnClickedBnDwgcRowMod()
{
	// TODO: Add your control notification handler code here
	_ASSERTE(NULL != m_pGridCtrl);
	INT Row = m_pGridCtrl->GetSelectedMinRow();
	KG_PROCESS_ERROR(Row > 0 && Row < m_pGridCtrl->GetRowCount());//��һ���ǲ���ɾ����
	{
		KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(Row, 0);
		KG_PROCESS_ERROR(pKeyHandle);

		IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
		KG_PROCESS_ERROR(pDW);

		INT nRegionIndexNew = 0;
		{
			KSceneCommonParamDialog CTDialog;
			nRegionIndexNew = pKeyHandle->m_Region;
			INT nParamIndex = CTDialog.AddAnParam(_T("��Region��Index��"), nRegionIndexNew,TRUE);
			KG_PROCESS_SUCCESS(CTDialog.DoModal() != IDOK);
			KG_PROCESS_ERROR(CTDialog.GetData(nParamIndex, nRegionIndexNew));
			KG_PROCESS_ERROR(nRegionIndexNew != 0);
			KG_PROCESS_ERROR(nRegionIndexNew != pKeyHandle->m_Region);
		}

		HRESULT hr = pDW->TL_ModifyRegionIndex(pKeyHandle->m_Region, nRegionIndexNew);
		KG_COM_PROCESS_ERROR(hr);

		//FuncTreeItemSelectedChangeCallback(&m_Tree, m_Tree.GetSelectedItem(), NULL, reinterpret_cast<DWORD_PTR>(this));
		_ASSERTE(NULL != m_pTree);
		this->OnItemSelectChange(*m_pTree, m_pTree->GetSelectedItem(), NULL);
	}
Exit1:
Exit0:
	return;
}

void KSceneSettingDialogDWGCKeyTrack::OnBnClickedBnDwgcRowDown()//�ӷֿ���ȡ
{
	// TODO: Add your control notification handler code here
	IEKG3DRegionInfoManager* pRIM = KSH::GetEngineInterface<KG3DTYPE_REGION_INFO_MANAGER>();
	IEKG3DEnvEffDynamicWeather* pDW =  this->GetDynamicWeather();
	if (!pRIM || !pDW)
		return;
	INT nDiffArrayCount = 0;
	DWORD* pDiffBuffer = NULL;
	HRESULT hr = pRIM->GetDiffArrayCount(&nDiffArrayCount);
	KG_COM_PROCESS_ERROR(hr);
	{
		_ASSERTE(nDiffArrayCount >= 0 && nDiffArrayCount < 1024);
		pDiffBuffer = new DWORD[nDiffArrayCount];
		HRESULT hr = pRIM->GetDiffArray(pDiffBuffer, nDiffArrayCount);
		KG_COM_PROCESS_ERROR(hr);
		
		
		INT nRegionCount = pDW->TL_GetRegionIndicesCount();
		if (nDiffArrayCount+nRegionCount > KG3DENVEFFRI_REGION_MAX)
		{
			MessageBox("�ֿ�ͼ�еķֿ���ȫ�����֮���������������ֿ�����Ҫ�࣬����ֻ�ܲ�����ӣ�\
�뿼���޸ķֿ�ͼ���ٷֿ���������ɾ����ǰ�ķֿ�");
		}
		for (int i = 0; i < nDiffArrayCount; i++)
		{
			pDW->TL_AddRegionIndex(pDiffBuffer[i]);
		}
		_ASSERTE(NULL != m_pTree);
		this->OnItemSelectChange(*m_pTree, m_pTree->GetSelectedItem(), NULL);//ǿ��Ҫ���ض�
		SAFE_DELETE_ARRAY(pDiffBuffer);
		return;
	}
Exit0:
	SAFE_DELETE_ARRAY(pDiffBuffer);
	return;
}

void KSceneSettingDialogDWGCKeyTrack::OnBnClickedSsBnSetKey()
{
	// TODO: Add your control notification handler code here	
	IEKG3DEnvEffDynamicWeather* pDW = GetDynamicWeather();
	KG_PROCESS_ERROR(pDW);

	{
		_ASSERTE(NULL != m_pGridCtrl);
		CCellID cellID = m_pGridCtrl->GetFocusCell();

		INT nRow = cellID.row;
		INT nColumn = cellID.col;
		KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(nRow, 0);
		KG_PROCESS_ERROR(pKeyHandle);
		INT nHour = nColumn;
		INT nRegion = pKeyHandle->m_Region;

		_ASSERTE(NULL != m_pTree);
		HTREEITEM hItemSelected = static_cast<TypeTreeCtrl*>(m_pTree)->GetSelectedItem();
		KG3DENVEFF_HANDLE* pEnvEffHandle = static_cast<TypeTreeCtrl*>(m_pTree)->GetItemBigData(hItemSelected);
		KG_PROCESS_ERROR(pEnvEffHandle);

		HRESULT hr = pDW->StateToKey(*pEnvEffHandle, KG3DENVEFF_DWGCKEY_HANDLE(nRegion, nHour));
		this->GridReloadKey(nRow,  nColumn);//����������Ӧ��Reload

		KG_COM_PROCESS_ERROR(hr);
	}
Exit0:
	return;

}

void KSceneSettingDialogDWGCKeyTrack::OnBnClickedSsBnSetAllKey()
{
	IEKG3DEnvEffDynamicWeather* pDW = GetDynamicWeather();
	{
		_ASSERTE(NULL != m_pGridCtrl);
		CCellID cellID = m_pGridCtrl->GetFocusCell();

		INT nRow = cellID.row;
		INT nColumn = cellID.col;
		KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(nRow, 0);
		KG_PROCESS_ERROR(pKeyHandle);
		INT nHour = nColumn;
		INT nRegion = pKeyHandle->m_Region;

		struct Visitor : public KSH::CTreeCtrlVisitor 
		{
			IEKG3DEnvEffDynamicWeather* m_pDW;
			KG3DENVEFF_DWGCKEY_HANDLE KeyHandle;
			std::vector<KG3DENVEFF_HANDLE> m_vecFailedEffect;
#if defined(DEBUG) | defined(_DEBUG)
			UINT m_VisitedItemCount;
#endif
			
			void Accept(CTreeCtrl& Tree, HTREEITEM hItem)
			{
				_ASSERTE(m_pDW && hItem != TVI_ROOT);
#if defined(DEBUG) | defined(_DEBUG)
				++m_VisitedItemCount;
#endif
				KG3DENVEFF_HANDLE* pEnvEffHandle = (dynamic_cast<TypeTreeCtrl&>(Tree)).GetItemBigData(hItem);
				HRESULT hr = E_FAIL;
				KG_PROCESS_ERROR(pEnvEffHandle);
				KG_PROCESS_SUCCESS(! m_pDW->IsEffectDWGCAvailable(*pEnvEffHandle));
				hr = m_pDW->StateToKey(*pEnvEffHandle, KeyHandle);
				KG_COM_PROCESS_ERROR(hr);
Exit1:
				return;
Exit0:
				m_vecFailedEffect.push_back(*pEnvEffHandle);
			}
		};

		Visitor visitorTemp;
		visitorTemp.KeyHandle = KG3DENVEFF_DWGCKEY_HANDLE(nRegion, nHour);
		visitorTemp.m_pDW = this->GetDynamicWeather();
#if defined(DEBUG) | defined(_DEBUG)
		visitorTemp.m_VisitedItemCount = 0;
#endif
		KG_PROCESS_ERROR(visitorTemp.m_pDW);

		_ASSERTE(NULL != m_pTree);
		KSH::CTreeCtrlTraverse(*m_pTree, visitorTemp, KSH::emDepthFirst);
		_ASSERTE(visitorTemp.m_VisitedItemCount == m_pTree->GetCount());

		this->GridReloadKey(nRow, nColumn);

		if (! visitorTemp.m_vecFailedEffect.empty())
		{
			CString TempStr = _T("����Effect����SetAllKey������ʧ�ܣ�ԭ��δ֪��\n");
			for (size_t i = 0; i < visitorTemp.m_vecFailedEffect.size(); ++i)
			{
				TempStr += KSH::GetRTTITypeName(visitorTemp.m_vecFailedEffect[i]);
				TempStr += _T(";\n");
			}
			
			MessageBox(TempStr);
		}
	}
Exit0:
	return;
}

void KSceneSettingDialogDWGCKeyTrack::OnBnClickedSsBnCopyPrevKey()
{
	IEKG3DEnvEffDynamicWeather* pDW = GetDynamicWeather();
	KG_PROCESS_ERROR(pDW);
	{
		_ASSERTE(NULL != m_pGridCtrl);
		CCellID cellID = m_pGridCtrl->GetFocusCell();

		INT nRow = cellID.row;
		INT nColumn = cellID.col;
		KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(nRow, 0);
		KG_PROCESS_ERROR(pKeyHandle);
		INT nHour = nColumn;
		INT nRegion = pKeyHandle->m_Region;
		KG3DENVEFF_DWGCKEY_HANDLE SrcHandle;
		{
			bool bIsAnyPrevKeyExist = false;
			for (int i = nColumn; i > 0; --i)
			{
				if (m_pGridCtrl->GetItemText(nRow, i) != _T(""))
				{
					SrcHandle = KG3DENVEFF_DWGCKEY_HANDLE(nRegion, i);
					bIsAnyPrevKeyExist = true;
					break;
				}
			}
			if (! bIsAnyPrevKeyExist)
			{
				MessageBox(_T("��ǰѡ�е�Keyǰ��û��Key������������ɹ�"));
				goto Exit0;
			}
		}
		KG3DENVEFF_DWGCKEY_HANDLE DescHandle(nRegion, nHour);

		//���ȼ��ǰһ��Key��λ���Ƿ���һ����
		{
			struct Visitor : public  KSH::CTreeCtrlVisitor
			{
				KG3DENVEFF_DWGCKEY_HANDLE	m_KeyHandleSrc;
				KG3DENVEFF_DWGCKEY_HANDLE	m_KeyHandleDesc;
				IEKG3DEnvEffDynamicWeather* m_pDW;
				std::vector<KG3DENVEFF_HANDLE>	m_vecPrevKeyUnmatchEffects;
				std::vector<KG3DENVEFF_HANDLE>	m_vecPrevKeyDonotExistEffect;
				void Accept(CTreeCtrl& Tree, HTREEITEM hItem)
				{
					_ASSERTE(m_pDW);
					KG3DENVEFF_HANDLE* pEnvEffHandle = (dynamic_cast<TypeTreeCtrl&>(Tree)).GetItemBigData(hItem);
					_ASSERTE(pEnvEffHandle);
					
					if (! m_pDW->IsEffectDWGCAvailable(*pEnvEffHandle))
					{
						return;
					}

					KG3DENVEFF_DWGCKEY_HANDLE HandleRet;
					
					HRESULT hr = m_pDW->TL_GetPrevKeyHandle(*pEnvEffHandle, m_KeyHandleDesc, &HandleRet);
					if (SUCCEEDED(hr))
					{
						if (HandleRet != m_KeyHandleSrc)
						{
							m_vecPrevKeyUnmatchEffects.push_back(*pEnvEffHandle);
						}												
					}
					else
					{
						m_vecPrevKeyDonotExistEffect.push_back(*pEnvEffHandle);
					}
				}
			};

			Visitor visitorTemp;
			visitorTemp.m_pDW = pDW;
			visitorTemp.m_KeyHandleSrc = SrcHandle;
			visitorTemp.m_KeyHandleDesc = DescHandle;
			
			_ASSERTE(NULL != m_pTree);
			KSH::CTreeCtrlTraverse(*m_pTree, visitorTemp, KSH::emDepthFirst);

			if (! visitorTemp.m_vecPrevKeyDonotExistEffect.empty())
			{
				CString TempStr = _T("�����µ�Effect��Key���У���ǰѡ�е�Keyǰ��û��Key���������ɹ�:\n");
				for (size_t i = 0; i < visitorTemp.m_vecPrevKeyDonotExistEffect.size(); ++i)
				{
					TempStr += KSH::GetRTTITypeName(visitorTemp.m_vecPrevKeyDonotExistEffect[i]);
					TempStr += _T(";\n");
				}
				MessageBox(TempStr);
				goto Exit0;
			}

			if (! visitorTemp.m_vecPrevKeyUnmatchEffects.empty())
			{
				CString TempStr = _T("�����µ�Effect��Key���У�ǰ���Key��λ�ò�һ�£��������ɹ�:\n");
				for (size_t i = 0; i < visitorTemp.m_vecPrevKeyUnmatchEffects.size(); ++i)
				{
					TempStr += KSH::GetRTTITypeName(visitorTemp.m_vecPrevKeyUnmatchEffects[i]);
					TempStr += _T(";\n");
				}
				MessageBox(TempStr);
				goto Exit0;
			}
		}

		//��ʼ��ʽ����
		{
			struct Visiter: public KSH::CTreeCtrlVisitor
			{
				KG3DENVEFF_DWGCKEY_HANDLE	m_KeyHandleSrc;
				KG3DENVEFF_DWGCKEY_HANDLE	m_KeyHandleDesc;
				IEKG3DEnvEffDynamicWeather* m_pDW;
				void Accept(CTreeCtrl& Tree, HTREEITEM hItem)
				{
					_ASSERTE(m_pDW);
					KG3DENVEFF_HANDLE* pEnvEffHandle = (dynamic_cast<TypeTreeCtrl&>(Tree)).GetItemBigData(hItem);
					_ASSERTE(pEnvEffHandle);
					if (! m_pDW->IsEffectDWGCAvailable(*pEnvEffHandle))
					{
						return;
					}
					m_pDW->TL_CopyKeyTo(*pEnvEffHandle, m_KeyHandleSrc
						, *pEnvEffHandle, m_KeyHandleDesc, TRUE);//���ֿ���ֻ��ByValue

				}
			};

			Visiter visiterTemp;
			visiterTemp.m_pDW = pDW;
			visiterTemp.m_KeyHandleDesc = DescHandle;
			visiterTemp.m_KeyHandleSrc = SrcHandle;
			
			_ASSERTE(NULL != m_pTree);
			KSH::CTreeCtrlTraverse(*m_pTree, visiterTemp, KSH::emDepthFirst);

			//ǿ�����¼��ص�ǰ��Key��
			
			_ASSERTE(NULL != m_pTree);
			this->OnItemSelectChange(*m_pTree, m_pTree->GetSelectedItem(), NULL);
		}

	}
Exit0:
	return;
}

void KSceneSettingDialogDWGCKeyTrack::OnBnClickedSsBnDelAllKey()
{
	int nRet = MessageBox(_T("����ɾ�����йؼ�֡���ϵĵ�ǰѡ�е�Region/Hour��Ӧ�Ĺؼ�֡���Ƿ����?"), NULL, MB_YESNO);
	KG_PROCESS_SUCCESS(nRet != IDYES);
	{
		_ASSERTE(NULL != m_pGridCtrl);
		IEKG3DEnvEffDynamicWeather* pDW = GetDynamicWeather();
		KG_PROCESS_ERROR(pDW);
		
		CCellID cellID = m_pGridCtrl->GetFocusCell();

		INT nRow = cellID.row;
		INT nColumn = cellID.col;
		KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(nRow, 0);
		KG_PROCESS_ERROR(pKeyHandle);
		INT nHour = nColumn;
		INT nRegion = pKeyHandle->m_Region;
		KG3DENVEFF_DWGCKEY_HANDLE SrcHandle(nRegion, nHour);

		struct Visiter : public KSH::CTreeCtrlVisitor 
		{
			IEKG3DEnvEffDynamicWeather* m_pDW;
			KG3DENVEFF_DWGCKEY_HANDLE	m_KeyHandle;
			void Accept(CTreeCtrl& TreeBase, HTREEITEM hItem)
			{
				TypeTreeCtrl& Tree = dynamic_cast<TypeTreeCtrl&>(TreeBase);
				KG3DENVEFF_HANDLE* pEffHandle = Tree.GetItemBigData(hItem);
				KG_PROCESS_ERROR(pEffHandle);
				_ASSERTE(m_pDW);
				if (m_pDW->IsEffectDWGCAvailable(*pEffHandle))
				{
					m_pDW->TL_DelKey(*pEffHandle, m_KeyHandle);

				}
			Exit0:
				return;

			}
		};

		Visiter visitorTemp;
		visitorTemp.m_pDW = pDW;
		visitorTemp.m_KeyHandle = SrcHandle;

		_ASSERTE(NULL != m_pTree);
		KSH::CTreeCtrlTraverse(*m_pTree, visitorTemp, KSH::emDepthFirst);

		this->GridReloadKey(nRow, nColumn);
	}

Exit1:
Exit0:
	return;
}

void KSceneSettingDialogDWGCKeyTrack::OnBnClickedBnDwgcPlay()
{
	// TODO: Add your control notification handler code here

	if (m_bIsDWPlaying)///������
	{
		IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
		if (pDW)
		{
			pDW->StopQuickPlay(NULL);
			HRESULT hr = pDW->EnablePlay(FALSE);
			_ASSERTE(SUCCEEDED(hr));
		}
		m_cbPlay.SetWindowText(_T("����"));
		m_bIsDWPlaying = false;
		goto Exit1;
	}
	{
		CString csRegion, csHour;//, csBoostFactor;
		m_edRegion.GetWindowText(csRegion);
		m_edHour.GetWindowText(csHour);
		BOOL bRet = FALSE;
		INT nRegion  = 0;
		INT nHour = 0;
		
		HRESULT hr = E_FAIL;
		IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
		if(!pDW)
		{
			MessageBox(_T("��̬����������?"));
			goto Exit0;
		}
#if defined(_DEBUG) | defined(DEBUG)
		{
			CString csZero = _T("000.0000");
			FLOAT fValue = 1;
			BOOL dbBRet = KG_CUSTOM_HELPERS::TStringToValue(csZero, fValue);
			_ASSERTE(dbBRet && fValue == 0.0f);
		}
#endif
		bRet = KG_CUSTOM_HELPERS::TStringToValue(csRegion, nRegion);
		if (!bRet)
		{
			_KG3D_DEBUG_SELF_REPORT(_T("Regionת��ʧ�ܣ�Ϊʲô?\n"));
			goto Exit0;
		}
		bRet = KG_CUSTOM_HELPERS::TStringToValue(csHour, nHour);
		if (!bRet || nHour <1 || nHour > 24)
		{
			MessageBox(_T("Hour���Ϸ�"));
			goto Exit0;
		}		
		hr = pDW->EnablePlay(TRUE);
		KG_COM_PROCESS_ERROR(hr);
		hr = pDW->StartQuickPlay(nRegion, nHour, NULL);
		KG_COM_PROCESS_ERROR(hr);
		
		//_ASSERTE(SUCCEEDED(hr)); 

#if defined(_DEBUG) | defined(DEBUG)
		CString csTemp2;
		m_cbPlay.GetWindowText(csTemp2);
		_ASSERTE(csTemp2 == _T("����"));
#endif
		
		m_cbPlay.SetWindowText(_T("ֹͣ"));
		m_bIsDWPlaying = true;
	}
	
Exit1:
	return;
Exit0:
	return;
}

void KSceneSettingDialogDWGCKeyTrack::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (0 == nIDEvent)
	{
		IEKG3DEnvEffDynamicWeather* pDW = this->GetDynamicWeather();
		if (pDW)
		{
			CString TempStr = _T("");
			int nHour = static_cast<int>(pDW->GetCurHour());
			
			if (nHour != m_Slider.GetPos())
			{
				m_Slider.SetPos(nHour);
			
				KG_CUSTOM_HELPERS::TValueToString<INT>(nHour, TempStr);
				m_edHour.SetWindowText(TempStr);
			}

			int nRegion = static_cast<int>(pDW->GetCurRegion());
			KG_CUSTOM_HELPERS::TValueToString(nRegion, TempStr);
			m_edRegion.SetWindowText(TempStr);
		}
	}
	else if (1 == nIDEvent)
	{
		if (::GetActiveWindow() == this->GetSafeHwnd())
		{
			HTREEITEM hItemSelected = static_cast<TypeTreeCtrl*>(m_pTree)->GetSelectedItem();
			KG3DENVEFF_HANDLE* pEnvEffHandle = static_cast<TypeTreeCtrl*>(m_pTree)->GetItemBigData(hItemSelected);
			if(NULL != pEnvEffHandle)
			{
				this->UpdatePage(pEnvEffHandle->m_dwType);
			}
		}
	}
	TypeBase::OnTimer(nIDEvent);
}

void KSceneSettingDialogDWGCKeyTrack::OnBnClickedSsBnHelp()
{
	// TODO: Add your control notification handler code here
	CString csTemp = _T("1.������Environment���������ϸ�������Ч����������պк�ԶɽҪ�ڸ��Ե���������\
���Ժ�ῼ��ȫ����Environment�������ӡ��������Ӷ�̬�������������Ч��\n\
2.��Ӻ�Region����Ӧ���������Զ���Region�����г���\n\
3.ʹ������ģʽ֮һ���ұߵĹؼ�֡DopeSheet�б༭�ؼ�֡����ͨ��˫��Ԥ����\
SetKey��˫���������ĵ�ǰ״̬��ӣ���������ڣ��ؼ�֡�����޸Ĺؼ�֡��\
�༭��˫���򿪶�Ӧ����壬�����༭������ᱻ����ֱ���༭��ɣ�\
ֱ�ӱ༭��˫��ֱ�ӱ༭Key��Index������ͬ��Key��Index����ֱ��д���������棬\
�����ؼ�֡Ҳ����������ɡ�ĳЩ���ܱ༭�Ĺؼ�֡����SkySystem�Ĺؼ�֡����Ϲؼ�֡���ؼ�֡������\
����������SkyBox��Զɽ�ȵ�Key��Indexֵ��ֱ�ӱ༭�Ϳ����ˡ�ɾ��Ҳ����ֱ�ӱ༭���ѹؼ�֡��ɿվ�\
��ɾ����\n\
����ʹ�÷�ʽ�ο�Max�Ĺؼ�֡�༭��TrackGraph��DopeSheet\n\
4.������Ҫ�ṩ�ĸ�Region�����ĸ�ʱ�俪ʼ���Լ����ٶ��ٱ���һ�㽨��1000��Ϊ���ޣ�����10000�Ļ�\
��Ϊ���Զ�ƽ����ƣ��������л��Ĺؼ�֡��ֱ���ӵ������Է���������Ч������ʵ����ͨģʽ��˫��\
��Ԥ��Ҳ������ġ�\
5.�༭��Ķ�������ڳ����ļ�ͬĿ¼�����SeneSetting.ini�У���ʽ�������ȶ��ģ����Խ������\
����Ӱ��༭�õĶ�����\n\
6.�ܶ�С����û�����ļ���ɣ��Ժ��һֱ��������ʲôBug֮����������ϵRTX��chentianhong��");
	MessageBox(csTemp, _T("��ʾ"), MB_OK|MB_ICONWARNING);
}

VOID KSceneSettingDialogDWGCKeyTrack::ReloadAllEnvEff()
{
	BindTreeCallbacks(FALSE);
	{
		_ASSERTE(NULL != m_pTree);
		m_pTree->DeleteAllItems();
		IEKG3DEnvironment* pEnv = KSH::GetEngineInterfaceEx<IEKG3DEnvironment>();
		KG_PROCESS_ERROR(pEnv);

		KG3DENVEFF_HANDLE Handle;
		IEKG3DEnvEffect*	pTopEff = NULL;
		HRESULT hr = E_FAIL;
		
		KGCH::IteratorHolder itHolder;
		for(hr = pEnv->GetFirstTopEnvEffect(itHolder, &pTopEff)
			; SUCCEEDED(hr)
			; hr = pEnv->GetNextTopEnvEffect(itHolder, &pTopEff))
		{
			Handle = KG3DENVEFF_HANDLE(pTopEff->GetType(), 0);
			_ASSERTE(pTopEff);
			HTREEITEM hTopItem = static_cast<TypeTreeCtrl*>(m_pTree)->InsertItemEx2(KSH::GetRTTITypeName(Handle), TVI_ROOT
				, TVI_LAST, &Handle, 0,1);
			
			if(hTopItem)
			{
				INT nSubCount = 0;
				KG3DENVEFF_HANDLE SubHandle;
				HRESULT hrSub = pTopEff->GetSubEffectCount(&nSubCount);
				if (SUCCEEDED(hrSub))
				{
					for (INT i = 0; i < nSubCount; ++i)
					{
						hrSub = pTopEff->GetSubEffect(i, NULL, &SubHandle);
						if (SUCCEEDED(hrSub))
						{
							static_cast<TypeTreeCtrl*>(m_pTree)->InsertItemEx2(KSH::GetRTTITypeName(SubHandle), hTopItem, TVI_LAST, &SubHandle,0,1);
						}
					}
				}
			}
		}
	}	
Exit0:
	BindTreeCallbacks(TRUE);
	HTREEITEM h = static_cast<TypeTreeCtrl*>(m_pTree)->GetChildItem(TVI_ROOT);
	if (h)
	{
		static_cast<TypeTreeCtrl*>(m_pTree)->SelectItem(h);
	}
	
}

void KSceneSettingDialogDWGCKeyTrack::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	_ASSERTE(NULL != m_pGridCtrl);
	//����ʱ�们����
	if ((LPVOID)pScrollBar == (LPVOID)&m_Slider)
	{
		int nHour = m_Slider.GetPos();
		IEKG3DEnvEffDynamicWeather* pDW = GetDynamicWeather();
		if (pDW)
		{
			INT nRegion = static_cast<INT>(pDW->GetCurRegion());
			if(nRegion < KG3DENVEFFRI_REGION_MIN || nRegion > KG3DENVEFFRI_REGION_MAX)
			{
				INT nRet = MessageBox(_T("��̬�����ĵ�ǰ���򲻺Ϸ���������Region�б�ս��������\
���Ƿ��Զ���Region�б��еĵ�һ����Ϊ��̬�����ĵ�ǰ����?"), NULL, MB_YESNO);
				if(nRet == IDYES)
				{
					KG3DENVEFF_DWGCKEY_HANDLE* pKeyHandle = static_cast<TypeGridCtrl*>(m_pGridCtrl)->GetItemBigData(1,0);
					if(pKeyHandle)
					{
						pDW->SetCurRegion(pKeyHandle->m_Region, ! this->IsBlendingNeededToBeShown());
					}
				}
			}
			HRESULT hr = pDW->SetCurHour(nHour, ! this->IsBlendingNeededToBeShown());
			if (SUCCEEDED(hr))
			{
				INT InnerHour = static_cast<INT>(pDW->GetCurHour());
				_ASSERTE(InnerHour == nHour);
				CString TempStr;
				KG_CUSTOM_HELPERS::TValueToString<INT>(InnerHour, TempStr);
				m_edHour.SetWindowText(TempStr);
			}
		}
	}
	TypeBase::OnHScroll(nSBCode, nPos, pScrollBar);
}

VOID KSceneSettingDialogDWGCKeyTrack::UpdatePage( DWORD dwType )
{
	if(0 == m_hWndToPropertySheet)
		return;
	
	CWnd* p = CWnd::FromHandle(m_hWndToPropertySheet);

	try
	{
		KSceneSettingPropertySheet* pSheet = dynamic_cast<KSceneSettingPropertySheet*>(p);
		if(NULL == pSheet)
			return;

		CPropertyPage* pPropertyPage = pSheet->GetActivePage();
		KSceneSettingPageBase* pSettingPage = dynamic_cast<KSceneSettingPageBase*>(pPropertyPage);
		if(NULL == pSettingPage)
			return;

		pSettingPage->SynFromInnerToInterface(0);
	}
	catch (...)
	{

	}
}

VOID KSceneSettingDialogDWGCKeyTrack::SetAssociateHWnd( HWND hWnd )
{
	m_hWndToPropertySheet = hWnd;
}

#if 0
LRESULT KSceneSettingDialogDWGCKeyTrack::TypeTreeCtrl::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	if(message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK)
	{
		if (wParam == MK_LBUTTON)
		{
			INT x = LOWORD(lParam);
			INT y = HIWORD(lParam);
			UINT uFlags; 
			HTREEITEM hItem = this->HitTest(CPoint(x, y), &uFlags);
			if (hItem != NULL && TVHT_ONITEM & uFlags || TVHT_ONITEMBUTTON & uFlags)
			{
				//_KG3D_DEBUG_OUTPUT0("Selected\n");
				KG3DENVEFF_HANDLE* p = this->GetItemBigData(hItem);
				if (p && m_pDW)
				{
					if (! m_pDW->IsEffectDWGCAvailable(*p))
					{
						return FALSE;//�ػ���ʹ�ö�̬�������࣬���ø�ѡ��
					}
				}	
			}
		}	
	}
	else if (message == TVM_SELECTITEM)
	{
		HTREEITEM hItem = reinterpret_cast<HTREEITEM>(lParam);
		KG3DENVEFF_HANDLE* p = this->GetItemBigData(hItem);
		if (p && m_pDW)
		{
			if (! m_pDW->IsEffectDWGCAvailable(*p))
			{
				return FALSE;
			}
		}
	}
	return TypeTreeCtrlBase::WindowProc(message, wParam, lParam);
}
#endif
