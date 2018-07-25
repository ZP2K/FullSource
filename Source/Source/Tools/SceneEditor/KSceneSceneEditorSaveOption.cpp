// KSceneSceneEditorSaveOption.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "KSceneSceneEditorSaveOption.h"
#include "IEEditorTypes.h"
#include <math.h>
#include "KSceneHelperFuncs.h"


// KSceneSceneEditorSaveOption dialog

IMPLEMENT_DYNAMIC(KSceneSceneEditorSaveOption, CDialog)

KSceneSceneEditorSaveOption::KSceneSceneEditorSaveOption(CWnd* pParent /*=NULL*/)
	: CDialog(KSceneSceneEditorSaveOption::IDD, pParent)
	, m_bComputeCells(FALSE)
	, m_bLogicalExchange(FALSE)
	, m_b3DScene(FALSE)
	, m_bClearCell(false)
	, m_bOnlyAppointCell(FALSE)
	, m_dwSceneSaveOp(0)
	, m_bTerrainMap(FALSE)
	, m_fScale(1.0f)
{

}

KSceneSceneEditorSaveOption::~KSceneSceneEditorSaveOption()
{
	KGCH::TContainerDelete(m_vecSaveOptionCheckBoxes);
}




BEGIN_MESSAGE_MAP(KSceneSceneEditorSaveOption, CDialog)
	ON_BN_CLICKED(IDC_CHECK_CELL, &KSceneSceneEditorSaveOption::OnBnClickedCheckCell)
	ON_BN_CLICKED(IDC_CHECK_LOGICALEXCHANGE, &KSceneSceneEditorSaveOption::OnBnClickedCheckLogicalexchange)
	ON_BN_CLICKED(IDC_CHECK_3DSCENE, &KSceneSceneEditorSaveOption::OnBnClickedCheck3dscene)
	ON_BN_CLICKED(IDCANCEL, &KSceneSceneEditorSaveOption::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CLEARCELL, &KSceneSceneEditorSaveOption::OnBnClickedClearcell)
	ON_BN_CLICKED(IDC_CHECK_COMPUTEBBOX, &KSceneSceneEditorSaveOption::OnBnClickedCheckComputebbox)
	ON_BN_CLICKED(IDC_CHECK_ONLYAPPOINTCELL, &KSceneSceneEditorSaveOption::OnBnClickedCheckOnlyappointcell)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_ALL_REST, &KSceneSceneEditorSaveOption::OnBnClickedCheckUpdateAllRest)
	ON_BN_CLICKED(IDC_RADIO_REST, &KSceneSceneEditorSaveOption::OnBnClickedRadioRest)
	ON_BN_CLICKED(IDC_RADIO_NOT_REST, &KSceneSceneEditorSaveOption::OnBnClickedRadioNotRest)
	ON_BN_CLICKED(IDC_CHECK_TERRAINMAP, &KSceneSceneEditorSaveOption::OnBnClickedCheckTerrainmap)
	ON_EN_CHANGE(IDC_EDIT_SCALE, &KSceneSceneEditorSaveOption::OnEnChangeEditScale)
	ON_BN_CLICKED(IDOK, &KSceneSceneEditorSaveOption::OnBnClickedOk)
END_MESSAGE_MAP()


// KSceneSceneEditorSaveOption message handlers



void KSceneSceneEditorSaveOption::OnBnClickedCheckCell()
{
	UpdateData(TRUE);

	if (m_bComputeCells)
	{
		if (IDNO == AfxMessageBox("��ѡ������ᵼ�³������������е�TRAPȫ����ʧ��Ҫ������", MB_YESNO, MB_ICONQUESTION|MB_DEFBUTTON2))
		{
			m_bComputeCells = FALSE;
			UpdateData(FALSE);			
		}
	}
}

void KSceneSceneEditorSaveOption::OnBnClickedCheckLogicalexchange()
{
	UpdateData(TRUE);
}

void KSceneSceneEditorSaveOption::OnBnClickedCheck3dscene()
{
	UpdateData(TRUE);
	CheckSaveOptionCheckBoxes();
}

void KSceneSceneEditorSaveOption::OnBnClickedCheckUpdateAllRest()
{
	UpdateData(TRUE);
	BOOL bEnable = (BST_CHECKED == this->IsDlgButtonChecked(IDC_CHECK_UPDATE_ALL_REST));
	CWnd* p = this->GetDlgItem(IDC_RADIO_REST);
	p->EnableWindow(bEnable);
	p = this->GetDlgItem(IDC_RADIO_NOT_REST);
	p->EnableWindow(bEnable);
}
void KSceneSceneEditorSaveOption::OnBnClickedRadioRest()
{
	int id = this->GetCheckedRadioButton(IDC_RADIO_REST, IDC_RADIO_NOT_REST);
	m_bAllRest = id == IDC_RADIO_REST;
}
void KSceneSceneEditorSaveOption::OnBnClickedRadioNotRest()
{
	int id = this->GetCheckedRadioButton(IDC_RADIO_REST, IDC_RADIO_NOT_REST);
	m_bAllRest = id == IDC_RADIO_REST;
}

BOOL KSceneSceneEditorSaveOption::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_b3DScene = TRUE;
	m_bComputeCells = FALSE;
	m_bLogicalExchange = TRUE;
	m_bClearCell = FALSE;
	m_bComputeBBox = FALSE;
	m_bOnlyAppointCell = FALSE;
	m_bTerrainMap = FALSE;
	ShowSaveOption();

	UpdateData(FALSE);
	m_dwSceneSaveOp = EM_SAVE_OP_ALL;
	UpdateOptionState(FALSE);

	CheckSaveOptionCheckBoxes();

	// ��ȫ�����
	m_bUpdateAllRest = FALSE;
	m_bAllRest = FALSE;
	this->CheckRadioButton(IDC_RADIO_REST, IDC_RADIO_NOT_REST, IDC_RADIO_NOT_REST);
	CButton* p = (CButton*)this->GetDlgItem(IDC_CHECK_UPDATE_ALL_REST);
	p->SetCheck(FALSE);

	CWnd* pWnd =  this->GetDlgItem(IDC_EDIT_SCALE);
	if (NULL != pWnd)
	{
		BOOL bEnable = (BST_CHECKED == this->IsDlgButtonChecked(IDC_CHECK_TERRAINMAP));
		pWnd->EnableWindow(bEnable);
		((CWnd*)(GetDlgItem(IDC_STATIC_SCALE)))->EnableWindow(bEnable);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void KSceneSceneEditorSaveOption::OnBnClickedCancel()
{
	OnCancel();
}

void KSceneSceneEditorSaveOption::OnBnClickedClearcell()
{
	UpdateData(TRUE);
}

void KSceneSceneEditorSaveOption::OnBnClickedCheckComputebbox()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void KSceneSceneEditorSaveOption::OnBnClickedCheckOnlyappointcell()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

namespace
{
	static UINT GetIDofOptionDesc(size_t i)
	{
		return IDC_BN_OPTION_BEGIN + static_cast<UINT>(i);
	}
	static UINT GetIndexOfOptionID(UINT id)
	{
		_ASSERTE(id >= IDC_BN_OPTION_BEGIN && id <= IDC_BN_OPTION_END);
		return id - IDC_BN_OPTION_BEGIN;
	}
};
void KSceneSceneEditorSaveOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_CELL, m_bComputeCells);
	DDX_Check(pDX, IDC_CHECK_LOGICALEXCHANGE, m_bLogicalExchange);
	DDX_Check(pDX, IDC_CHECK_3DSCENE, m_b3DScene);
	DDX_Check(pDX, IDC_CLEARCELL, m_bClearCell);
	DDX_Check(pDX, IDC_CHECK_COMPUTEBBOX, m_bComputeBBox);
	DDX_Check(pDX, IDC_CHECK_ONLYAPPOINTCELL, m_bOnlyAppointCell);
	DDX_Check(pDX, IDC_CHECK_UPDATE_ALL_REST, m_bUpdateAllRest);
	DDX_Check(pDX, IDC_CHECK_TERRAINMAP, m_bTerrainMap);
	DDX_Text(pDX, IDC_EDIT_SCALE,m_fScale);
}
namespace
{
	VOID UpdateOptionStatePrivate(BOOL bToDataOrToInterface, std::vector<CButton*>& vecButtons, IButtonOperator& buttonOp
									, DWORD& dwSceneSaveOpInput)	//�Ѻ����㷨��������������ӾͿ��Խ���TDD������
	{
		TypeSaveOptionDescArrayRef saveOpDesc = g_GetSaveOptionDesc();
		if (bToDataOrToInterface)	//�ӽ��浽�ڲ�
		{
			DWORD dwOp = EM_SAVE_OP_ALL;


			//�����ȡ����ѡ���� "û�й���ȥ��"�ķ�ʽ���㣬������"�й��ͼ��ϵķ�ʽ����"��������ȫһ�㣬ʡ���е�ѡ��û����������������û�б���
			for (size_t i = 0; i < vecButtons.size(); ++i)
			{
				BOOL bCheck = (BST_CHECKED == /*m_vecSaveOptionCheckBoxes[i]->GetCheck()*/buttonOp.GetCheck(*vecButtons[i]));
				if (! bCheck)
				{
					dwOp &= (~(saveOpDesc[i].dwOp));
				}
			}
#if defined(DEBUG) | defined(_DEBUG)
			{
				//����������Ƿ�ÿ��ѡ����ж�Ӧ��ѡ���
				DWORD dwOpCheck = dwOp;
				for (UINT i = 0; i < EM_SAVE_OP_COUNT; ++i)
				{
					if (dwOpCheck & saveOpDesc[i].dwOp)
					{
						_ASSERTE(BST_CHECKED == /*m_vecSaveOptionCheckBoxes[i]->GetCheck()*/buttonOp.GetCheck(*vecButtons[i]));
					}
				}
			}
#endif
			dwSceneSaveOpInput = dwOp;
		}
		else
		{
			for (size_t i = 0; i < vecButtons.size(); ++i)
			{
				BOOL bCheck = saveOpDesc[i].dwOp & dwSceneSaveOpInput;
				//m_vecSaveOptionCheckBoxes[i]->SetCheck(bCheck ? BST_CHECKED : BST_UNCHECKED);
				buttonOp.SetCheck(*vecButtons[i], bCheck ? BST_CHECKED : BST_UNCHECKED);
			}
		}
	}
};
void KSceneSceneEditorSaveOption::UpdateOptionState(BOOL bToDataOrToInterface)
{
	struct TypeButtonOp : IButtonOperator 
	{
		virtual VOID SetCheck(CButton& bt, INT nCheckState)
		{
			bt.SetCheck(nCheckState);
		}
		virtual INT GetCheck(CButton& bt)
		{
			return bt.GetCheck();
		}
	};
	TypeButtonOp tempOp;
	return UpdateOptionStatePrivate(bToDataOrToInterface, m_vecSaveOptionCheckBoxes, tempOp, m_dwSceneSaveOp);
}
VOID KSceneSceneEditorSaveOption::ShowSaveOption()
{
	_ASSERTE(IDC_BN_OPTION_BEGIN < IDC_BN_OPTION_END && _T("��Ҫ����������ID�Ĵ�С����֤һϵ��Button�Ķ�̬\
														   �������������������Ծ�Ҫ���ID"));
	_ASSERTE(IDC_BN_OPTION_END - IDC_BN_OPTION_BEGIN > EM_SAVE_OP_COUNT);

	const UINT uCheckBoxHeight = 20;
	const UINT uCheckBoxHeightCap = 2;
	const UINT uCheckBoxWidth = 150;
	const UINT uCheckBoxWidthGap = 2;

	//�Ӳο����ڵõ�һ���ο���С
	CRect rectStandard;
	{
		HWND hWndRect = 0;
		this->GetDlgItem(IDC_STATIC_TEXT, &hWndRect);
		if (0 == hWndRect)	//����ؼ���������λ��
			return;

		CRect rectTemp;
		::GetWindowRect(hWndRect, &rectTemp);
		this->ScreenToClient(&rectTemp);
		rectStandard = rectTemp;
		rectStandard.bottom = rectStandard.top + uCheckBoxHeight;
		rectStandard.right = rectStandard.left + uCheckBoxWidth;
		rectStandard.MoveToXY(rectStandard.left + uCheckBoxWidthGap, rectStandard.top + uCheckBoxHeight);
	}

	

	TypeSaveOptionDescArrayRef saveOpDesc = g_GetSaveOptionDesc();
	_ASSERTE(_countof(saveOpDesc) == m_vecSaveOptionCheckBoxes.size() || m_vecSaveOptionCheckBoxes.empty());
	
	if (m_vecSaveOptionCheckBoxes.empty())	//��һ��Init��empty�ģ�Ȼ��Ͳ����ˣ�
	{
		for (UINT i = 0; i < _countof(saveOpDesc); ++i)
		{
			CButton* p = new CButton;
			KG_PROCESS_ERROR(NULL != p);

			m_vecSaveOptionCheckBoxes.push_back(p);
		}
	}
	
	if(_countof(saveOpDesc) == m_vecSaveOptionCheckBoxes.size())//�ڶ���Init��ʱ��Ҫ���°Ѵ��ڸ���������
	{
		for (UINT i = 0; i < _countof(saveOpDesc); ++i)
		{
			CButton* p = m_vecSaveOptionCheckBoxes[i];
			KEditorOptionDesc& descTemp = saveOpDesc[i];

			CRect rectTemp;
			rectTemp.top = rectStandard.top + (uCheckBoxHeight + uCheckBoxHeightCap) * (i / 2);
			rectTemp.bottom = rectTemp.top + uCheckBoxHeight;
			rectTemp.left = rectStandard.left + (i % 2) * (uCheckBoxWidth + uCheckBoxWidthGap);
			rectTemp.right = rectTemp.left + uCheckBoxWidth;

			BOOL bRet = p->Create(descTemp.strDesc, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, rectTemp, this, GetIDofOptionDesc(i));
			KG_PROCESS_ERROR(bRet);

			p->SetCheck(BST_CHECKED);
		}
	}
	
	return;
Exit0:
	KGCH::TContainerDelete(m_vecSaveOptionCheckBoxes);
	return;
}

std::pair<DWORD, std::tstring> KSceneSceneEditorSaveOption::GetSceneSaveOption()
{
	if (m_vecSaveOptionCheckBoxes.empty())
	{
		return std::pair<DWORD, std::tstring>(EM_SAVE_OP_ALL, std::tstring(_T("ȫ��")));
	}

	TypeSaveOptionDescArrayRef saveOpDesc = g_GetSaveOptionDesc();
	_ASSERTE(_countof(saveOpDesc) == m_vecSaveOptionCheckBoxes.size());

	DWORD dwOpRet = EM_SAVE_OP_ALL;
	std::tstring strRet = _T("");

	//�����ȡ����ѡ���� "û�й���ȥ��"�ķ�ʽ���㣬������"�й��ͼ��ϵķ�ʽ����"��������ȫһ�㣬ʡ���е�ѡ��û����������������û�б���
	for (size_t i = 0; i < _countof(saveOpDesc); ++i)
	{
		if(m_dwSceneSaveOp & saveOpDesc[i].dwOp)
		{
			strRet += _T("Yes:");
		}
		else
		{	
			strRet += _T("No:");
		}
		strRet += saveOpDesc[i].strDesc;
		strRet += _T("\r\n");
	}


	return std::pair<DWORD, std::tstring>(m_dwSceneSaveOp, strRet);
}

void KSceneSceneEditorSaveOption::CheckSaveOptionCheckBoxes()
{
	BOOL bEnable = (BST_CHECKED == this->IsDlgButtonChecked(IDC_CHECK_3DSCENE));
	CWnd* p = this->GetDlgItem(IDC_CHECK_COMPUTEBBOX);
	if (NULL != p)
	{
		p->EnableWindow(bEnable);
	}
#if 1	//<DISABLE_SAVE_OPTIONS>
	for (size_t i = 0; i < m_vecSaveOptionCheckBoxes.size(); ++i)
	{
		m_vecSaveOptionCheckBoxes[i]->EnableWindow(bEnable);
	}
#else
	for (size_t i = 0; i < m_vecSaveOptionCheckBoxes.size(); ++i)
	{
		m_vecSaveOptionCheckBoxes[i]->EnableWindow(FALSE);
	}
#endif
}
BOOL KSceneSceneEditorSaveOption::OnCommand( WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = CDialog::OnCommand(wParam, lParam);


	int nCode = HIWORD(wParam);
	if (nCode != BN_CLICKED )
		return bRet;

	UINT uID = LOWORD(wParam);
	if (uID < IDC_BN_OPTION_BEGIN || uID > IDC_BN_OPTION_END)
	{
		UpdateOptionState(TRUE);
	}
//
//	CButton* pBn = NULL;
//	{
//		UINT uOrder = GetIndexOfOptionID(uID);
//		if(uOrder < m_vecSaveOptionCheckBoxes.size())
//		{
//			pBn = m_vecSaveOptionCheckBoxes[uOrder];
//			_ASSERTE(NULL != pBn);
//		}
//	}
//	if(NULL == pBn)
//		return;
//	
//#if defined(DEBUG) | defined(_DEBUG)
//	{
//		HWND hWnd = (HWND)lParam;
//		if (0 != hWnd)
//		{
//			_ASSERTE(hWnd == pBn->GetSafeHwnd());
//		}
//	}
//#endif
//	
//	if(! bRet)
//	{
//		//������Ӧ��̬�����İ�ť�ĵ���Ļ���ֻ���Լ���Ӧ��
//		pBn->SetCheck(pBn->GetCheck() == BST_CHECKED ? BST_UNCHECKED : BST_CHECKED);
//	}
//
//	BOOL bChecked = pBn->GetCheck() == BST_CHECKED ? TRUE : FALSE;
//	
//	if (bChecked)
//	{
//		uID
//	}

	return bRet;

}

#if defined(KG_ENABLE_TDD)
KG_TEST_BEGIN(KSceneSceneEditorSaveOption)
{
	struct TypeButtonOp : public IButtonOperator 
	{
		std::vector<CButton*>	m_vecButtons;
		BOOL m_nButtonState[EM_SAVE_OP_COUNT];


		TypeButtonOp()
		{
			ZeroMemory(m_nButtonState, sizeof(m_nButtonState));
			m_vecButtons.reserve(EM_SAVE_OP_COUNT);
			for (size_t i = 0; i < EM_SAVE_OP_COUNT; ++i)
			{
				m_vecButtons.push_back(reinterpret_cast<CButton*>(i));
			}
		}
		virtual VOID SetCheck(CButton& bt, INT nCheckState)
		{
			_ASSERTE(0 != m_vecButtons.size());			
			size_t iIndex = reinterpret_cast<size_t>(&bt);
			_ASSERTE(iIndex < _countof(m_nButtonState));
			m_nButtonState[iIndex] = nCheckState;
		}

		virtual INT GetCheck(CButton& bt)
		{
			_ASSERTE(0 != m_vecButtons.size());
			size_t iIndex = reinterpret_cast<size_t>(&bt);
			_ASSERTE(iIndex < _countof(m_nButtonState));
			return m_nButtonState[iIndex];
		}
	};

	TypeButtonOp tempOp;
	DWORD dwSaveOp = 0;

	{
		dwSaveOp = 0;
		UpdateOptionStatePrivate(FALSE, tempOp.m_vecButtons, tempOp, dwSaveOp);
		for(size_t i = 0; i < _countof(tempOp.m_nButtonState); ++i)
		{
			_ASSERTE(BST_UNCHECKED == tempOp.m_nButtonState[i]);
		}
		dwSaveOp = 12345;
		UpdateOptionStatePrivate(TRUE, tempOp.m_vecButtons, tempOp, dwSaveOp);
		_ASSERTE(0 == dwSaveOp);
	}

	{
		dwSaveOp = EM_SAVE_OP_ALL;
		UpdateOptionStatePrivate(FALSE, tempOp.m_vecButtons, tempOp, dwSaveOp);
		for(size_t i = 0; i < _countof(tempOp.m_nButtonState); ++i)
		{
			_ASSERTE(BST_CHECKED == tempOp.m_nButtonState[i]);
		}
		dwSaveOp = 0;
		UpdateOptionStatePrivate(TRUE, tempOp.m_vecButtons, tempOp, dwSaveOp);
		_ASSERTE(EM_SAVE_OP_ALL == dwSaveOp);
	}

	{
		dwSaveOp = EM_SAVE_OP_TERRAIN_AND_ENTITY;
		UpdateOptionStatePrivate(FALSE, tempOp.m_vecButtons, tempOp, dwSaveOp);
		
		INT nIndexOfTerrainOP = (INT)(log((float)EM_SAVE_OP_TERRAIN_AND_ENTITY) / log(2.f));	//��0x1000�����ӣ���1�ǵڼ�λ����ʵ�Ͼ���2Ϊ�׵�log
		_ASSERTE(BST_CHECKED == tempOp.m_nButtonState[nIndexOfTerrainOP]);
		
		dwSaveOp = 0;
		UpdateOptionStatePrivate(TRUE, tempOp.m_vecButtons, tempOp, dwSaveOp);
		_ASSERTE(EM_SAVE_OP_TERRAIN_AND_ENTITY == dwSaveOp);
	}
}
KG_TEST_END(KSceneSceneEditorSaveOption)
#endif

void KSceneSceneEditorSaveOption::OnBnClickedCheckTerrainmap()
{
	UpdateData();
	CWnd* pWnd =  this->GetDlgItem(IDC_EDIT_SCALE);
	if (NULL != pWnd)
	{
		BOOL bEnable = (BST_CHECKED == this->IsDlgButtonChecked(IDC_CHECK_TERRAINMAP));
		pWnd->EnableWindow(bEnable);
		((CWnd*)(GetDlgItem(IDC_STATIC_SCALE)))->EnableWindow(bEnable);
	}
	 
}

void KSceneSceneEditorSaveOption::OnEnChangeEditScale()
{
	UpdateData();
	if(m_fScale<1.0f)
	{
		m_fScale = 1.0f;
		UpdateData(FALSE);
	}
}

void KSceneSceneEditorSaveOption::OnBnClickedOk()
{
	if (m_bUpdateAllRest && !m_bAllRest)
		AfxMessageBox("ȫ��ͼ��ȫ��Ϣ�ı������ȥ����ˢ���������Ƶİ�ȫ��Ϣ��", MB_OK, MB_ICONINFORMATION|MB_DEFBUTTON1);
	
	OnOK();
}
