// KSceneSceneEditorDialogNPC.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "KSceneSceneEditorDialogNPC.h"
#include ".\kscenesceneeditordialognpc.h"
//#include ".\kg3drepresentobjecttable.h"
//#include ".\kg3denginemanager.h"
//#include "KG3DRepresentNPC.h"
//#include "kg3dSceneSceneEditor.h"
#include "KNpcTemplateList.h"
#include "KSO3World.h"
#include "SO3Represent/SO3RepresentInterface.h"
#include "IEEditor.h"
#include "IEKG3DSceneObjectPlacementTool.h"
#include "KSceneEditorDocLogical.h"
#include "KSceneEditorReviveDialog.h"
#include "KSceneEditorRandomGroupDialog.h"
#include "KSceneEditorAISetDialog.h"
#include "KSceneHelperFuncs.h"

#include "..\..\KG3DEngine\EditInterface\IEKG3DSceneSelectionTool.h"
#include "KSceneEditorDocLogical.h"


//KG3DEngine\KG3DEngine\KG3DRepresentObject.h

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern IKSO3RepresentHandler* g_pRepresentHandler;

////////////////////////////////////////////////////////////////////////////////


// KSceneSceneEditorDialogNPC dialog

IMPLEMENT_DYNAMIC(KSceneSceneEditorDialogNPC, CDialog)
KSceneSceneEditorDialogNPC::KSceneSceneEditorDialogNPC(CWnd* pParent /*=NULL*/)
	: CDialog(KSceneSceneEditorDialogNPC::IDD, pParent)
	, m_SubStr(_T(""))
{
	m_lpSceneSceneEditor = NULL;
	m_nOutputWindowID = -1;
	m_FindNode = NULL;
	m_dwDefaultReliveID = 0;
	m_dwDefaultRandomID = 0;
	m_dwDefaultAISet = 0;
	m_lpDocLogical = NULL;
	m_dwNpcTempleteID = 0;
}

KSceneSceneEditorDialogNPC::~KSceneSceneEditorDialogNPC()
{
}

void KSceneSceneEditorDialogNPC::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_TREE_NPC, m_Tree);
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_Static_PreView);
	DDX_Control(pDX, IDC_BUTTON_RELIVE, m_Button_ReliveGroup);
	DDX_Control(pDX, IDC_BUTTON_RANDOM, m_Button_RandomGroup);
}


BEGIN_MESSAGE_MAP(KSceneSceneEditorDialogNPC, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_EDITMOVESPLINE, OnBnClickedButtonEditmovespline)
	ON_BN_CLICKED(IDC_BUTTON_EDITNPC, OnBnClickedButtonEditnpc)
	ON_BN_CLICKED(IDC_BUTTON_NEWNPC, OnBnClickedButtonNewnpc)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_NPC, OnTvnSelchangedTreeNpc)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_LOOKUP, &KSceneSceneEditorDialogNPC::OnBnClickedButtonLookup)
	ON_EN_CHANGE(IDC_EDIT1, &KSceneSceneEditorDialogNPC::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_RELIVE, &KSceneSceneEditorDialogNPC::OnBnClickedButtonRelive)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &KSceneSceneEditorDialogNPC::OnBnClickedButtonRandom)
	ON_BN_CLICKED(IDC_BUTTON_AISET, &KSceneSceneEditorDialogNPC::OnBnClickedButtonAiset)
	ON_BN_CLICKED(IDC_NPC_REPLACE_ONE, &KSceneSceneEditorDialogNPC::OnBnClickedNpcReplaceOne)
	ON_BN_CLICKED(IDC_NPC_REPLACE_ALL, &KSceneSceneEditorDialogNPC::OnBnClickedNpcReplaceAll)
END_MESSAGE_MAP()


// KSceneSceneEditorDialogNPC message handlers

void KSceneSceneEditorDialogNPC::OnBnClickedButtonEditmovespline()
{
	// TODO: Add your control notification handler code here
}

void KSceneSceneEditorDialogNPC::OnBnClickedButtonEditnpc()
{
	// TODO: Add your control notification handler code here
}

void KSceneSceneEditorDialogNPC::OnOK()
{

}

void KSceneSceneEditorDialogNPC::OnBnClickedButtonNewnpc()
{
	HRESULT hRetCode = S_OK;
	if(!m_lpSceneSceneEditor)
		return ;

    if (g_pSO3World == NULL)
        return;

	m_Tree.DeleteAllItems();
	m_mapType.clear();

	TCHAR Name[MAX_PATH];

	HTREEITEM hClass = NULL;
	int nCount = g_pSO3World->m_Settings.m_NpcTemplateList.GetNpcTemplateCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		const KNpcTemplate* pcNpcTemplate = g_pSO3World->m_Settings.m_NpcTemplateList.GetTemplateByIndex(nIndex);

		if (pcNpcTemplate && pcNpcTemplate->dwTemplateID != 0)
		{
			map<CString,HTREEITEM>::iterator s = m_mapType.find(pcNpcTemplate->szMapName);
			if(s==m_mapType.end())
			{
				//wsprintf(Name,"NPC����%s",pcNpcTemplate->szMapName);
				hClass = m_Tree.InsertItem(pcNpcTemplate->szMapName,NULL,TVI_LAST);
				m_Tree.SetItemData(hClass,0);
				m_mapType[pcNpcTemplate->szMapName] = hClass;
			}
			else
			{
				hClass = s->second;
			}
			HTREEITEM hItem = m_Tree.InsertItem(pcNpcTemplate->szName,hClass,TVI_LAST);
			m_Tree.SetItemData(hItem, pcNpcTemplate->dwTemplateID);

			//////////////////////////////////////////////////////////////////////////
			DWORD dwCount =0;
			dwCount = _kg_to_smaller_size_cast<DWORD>(m_Tree.GetItemData(hClass));
			dwCount++;
			m_Tree.SetItemData(hClass,dwCount);
			wsprintf(Name,"%s ��%d��",pcNpcTemplate->szMapName,dwCount);
			m_Tree.SetItemText(hClass,Name);
		}
		
		////������
		/*if (pcNpcTemplate && pcNpcTemplate->dwTemplateID != 0)
		{
			HTREEITEM hItem = m_Tree.InsertItem(pcNpcTemplate->szName);
			m_Tree.SetItemData(hItem, pcNpcTemplate->dwTemplateID);
		}*/
	}
	m_Tree.SortTree(NULL);


	
	
}

HRESULT KSceneSceneEditorDialogNPC::CreateNpcInHandle()
{
	HRESULT hr = E_FAIL;
	KGLOG_PROCESS_ERROR(g_pSO3World);

	const KNpcTemplate* pcNpcTempate = g_pSO3World->m_Settings.m_NpcTemplateList.GetTemplate(m_dwNpcTempleteID);
	if (pcNpcTempate == NULL)
		return E_FAIL;

	// const KAIParamTemplate* pcAITempate = g_pSO3World->m_Settings.m_AIParamTemplateList.GetTemplate(pcNpcTempate->nAIParamTemplateID);
	if (pcNpcTempate && pcNpcTempate->dwTemplateID != 0)
	{

		DWORD dwRepresentID = pcNpcTempate->dwRepresentID[0];

		KRepresentNPCEquipResource NPCRes;
		KRepresentNPCAnimationResource AniRes;

		g_pRepresentHandler->GetNPCEquipResource(dwRepresentID,&NPCRes);
		g_pRepresentHandler->GetNPCAnimationResource(dwRepresentID,30,&AniRes);

		LPCTSTR szMDLName = NPCRes.szMDLFilePath;
		LPCTSTR szAniName = AniRes.szAniFilePath;

		LPCTSTR szFaceMesh = NPCRes.Parts[RL_NPC_SOCKET_FACE].szMeshFilePath;
		LPCTSTR szFaceMtl = NPCRes.Parts[RL_NPC_SOCKET_FACE].szMtlFilePath;

		// float fAlertRange  = (float)(pcAITempate->nAlertRange) * LOGICAL_CELL_CM_LENGTH / CELL_LENGTH;
		// float fAttackRange = (float)(pcAITempate->nAttackRange) * LOGICAL_CELL_CM_LENGTH / CELL_LENGTH;
        float fAlertRange = 0.0f;
        float fAttackRange = 0.0f;

		TCHAR szMDLName_S[MAX_PATH];
		TCHAR szAniName_S[MAX_PATH];
		wsprintf(szMDLName_S,"%s",szMDLName);
		wsprintf(szAniName_S,"%s",szAniName);

		IEKG3DRepresentObject* pObj = BuildRepresentNPC(g_GetEngineIEInterface(), m_dwNpcTempleteID, m_szNpcName, szMDLName_S
			, szAniName_S, szFaceMesh, szFaceMtl, pcNpcTempate->nLevel, pcNpcTempate->nKind/*, pcNpcTempate->nAIType*/
			, NPCRes.fModelScale, pcNpcTempate->dwRepresentID[0], fAlertRange, fAttackRange,m_dwDefaultReliveID,m_dwDefaultRandomID,m_dwDefaultAISet);
		if (pObj)
		{
			hr = m_lpSceneSceneEditor->SetObjectInHandle(pObj, TRUE);
			if (FAILED(hr))
			{
				SAFE_RELEASE(pObj);
			}
		}
	}
	
	return S_OK;
Exit0:
	return E_FAIL;
}

void KSceneSceneEditorDialogNPC::OnTvnSelchangedTreeNpc(NMHDR *pNMHDR, LRESULT *pResult)
{
	HRESULT hRetCode = S_OK;

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	if(!m_lpSceneSceneEditor)
		return ;

	HTREEITEM hItem = m_Tree.GetSelectedItem();
	if(!hItem)
		return ;

	m_dwNpcTempleteID = (DWORD)m_Tree.GetItemData(hItem);
	m_szNpcName = m_Tree.GetItemText(hItem);

	CreateNpcInHandle();

}

void KSceneSceneEditorDialogNPC::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		m_nOutputWindowID = 0;
		m_lpSceneSceneEditor->AddOutputWindow("ScenePreView",m_Static_PreView.GetSafeHwnd(),
			OUTPUTWND_SCENEEDITOR_OBJECTPREVEIW,0,&m_nOutputWindowID);
		m_lpSceneSceneEditor->IEOnResizeWindow(NULL);
	}
	else
	{
		m_lpSceneSceneEditor->RemoveOutputWindow(m_nOutputWindowID);
	}
}

BOOL KSceneSceneEditorDialogNPC::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	OnBnClickedButtonNewnpc();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void KSceneSceneEditorDialogNPC::OnBnClickedButtonLookup()
{
	UpdateData(TRUE);
	if(m_SubStr=="")
	{
		MessageBox("��������Ϊ�գ�������!");
		return ;
	}
	m_FindNode = m_Tree.FindNextItembySubString(m_SubStr,m_FindNode);
	KG_PROCESS_ERROR(m_FindNode);
	
	m_Tree.SelectItem(m_FindNode);
	((CButton*)(GetDlgItem(IDC_BUTTON_LOOKUP)))->SetWindowText("������һ��");
	return ; 
Exit0:
	m_Tree.SelectItem(NULL);
	MessageBox("���ҽ�����û���ҵ��κ�ѡ�ϵ�NPC��!");
	((CButton*)(GetDlgItem(IDC_BUTTON_LOOKUP)))->SetWindowText("��ʼ����");

		
}

void KSceneSceneEditorDialogNPC::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT1,m_SubStr);
	((CButton*)(GetDlgItem(IDC_BUTTON_LOOKUP)))->SetWindowText("��ʼ����");
	m_FindNode = NULL;
}

void KSceneSceneEditorDialogNPC::OnBnClickedButtonRelive()
{
	KSceneEditorReviveDialog dlg;
	dlg.SetDocLogical(m_lpDocLogical,TRUE);

	dlg.DoModal();

	m_dwDefaultReliveID = dlg.m_dwCurrentSelectedID;

	TCHAR szName[MAX_PATH];
	wsprintf(szName,"������ID %d",m_dwDefaultReliveID);
	m_Button_ReliveGroup.SetWindowText(szName);

	CreateNpcInHandle();

}

void KSceneSceneEditorDialogNPC::OnBnClickedButtonRandom()
{
	KSceneEditorRandomGroupDialog Dlg;
	Dlg.SetSceneEditor(m_lpSceneSceneEditor,m_lpDocLogical);
	Dlg.DoModal();

	m_dwDefaultRandomID = Dlg.m_dwSelectedGroupID;

	TCHAR szName[MAX_PATH];
	wsprintf(szName,"�����ID %d",m_dwDefaultRandomID );
	m_Button_RandomGroup.SetWindowText(szName);

	CreateNpcInHandle();
}

void KSceneSceneEditorDialogNPC::OnBnClickedButtonAiset()
{
	/*KSceneEditorAISetDialog dlg;
	dlg.SetSceneEditor(m_lpSceneSceneEditor,m_lpDocLogical,m_dwNpcTempleteID,NULL);
	dlg.DoModal();

	m_dwDefaultAISet = dlg.m_dwSelectedID;
	CreateNpcInHandle();*/

}

bool KSceneSceneEditorDialogNPC::ReplaceSelectedNpc(OUT vector<DWORD> &templateids)
{
	//�õ����ʰȡ������
	IEKG3DRepresentObject * obj = 0;
	m_lpSceneSceneEditor->GetObjectInHandle(&obj);

	if(!obj)
	{
		MessageBox("����б���ѡȡNPC", "����", 0);
		return false;
	}

	IEKG3DRepresentNPC *handlenpc = m_lpSceneSceneEditor->GetIEKG3DRepresentNPC(obj);

	DWORD templateId = 0;
	handlenpc->GetTemplateID(&templateId);

	DWORD representID = 0;
	handlenpc->GetRepresentID(&representID);

	IEKG3DSceneObjectPlacementTool* pPlacementTool = KSH::GetEngineInterfaceEx<IEKG3DSceneObjectPlacementTool>();
	IEKG3DSceneSelectionTool* pSelTool = NULL;
	if(FAILED(m_lpSceneSceneEditor->GetSelectionToolInterface(&pSelTool)))
		return false;

	IEKG3DSceneSelectionToolAccessProxy* pProxy = NULL;
	if(FAILED(pSelTool->GetAccessProxy(&pProxy)))
		return false;



    int selectedNum = 0;
	for (IEKG3DSceneEntity* pEntity = pProxy->Reset(); NULL != pEntity; pEntity = pProxy->Next())
	{
		if(!pEntity)
			continue;

		void *pointer;
		pEntity->GetPointer(&pointer);

		IEKG3DRepresentObject* pObject = (IEKG3DRepresentObject*)pointer;
		if(!pObject)
			continue;

		if(pObject->GetType() != REPRESENTOBJECT_NPC)
		{
			char msg[256];
			pEntity->GetName(msg, 256);
			strcat(msg, "���ܱ��滻");
			MessageBox(msg, "����", 0);
			return false;
		}
		selectedNum ++;
	}

	if(!selectedNum)
	{
		::MessageBox(0, "��δѡ����Ҫ���滻��NPC", "����", 0);
		return false;
	}


	int nOK = ::MessageBox(0, "ȷ��Ҫ�滻��","��ʾ��Ϣ",MB_OKCANCEL   |   MB_ICONQUESTION);
	if(nOK != IDOK)
	{
		return false;
	}

     //////////////////////////////������ͨ���ˣ�����ִ���滻

	//��Ҫ���Ƴ���
	vector<IEKG3DRepresentObject *> removeObjs;

	//�滻����Ҫ��ӵ�ѡ���б��
	vector<IEKG3DRepresentObject *> selObjs;

	for (IEKG3DSceneEntity* pEntity = pProxy->Reset(); NULL != pEntity; pEntity = pProxy->Next())
	{
		if(!pEntity)
			continue;

		void *pointer;
		pEntity->GetPointer(&pointer);

		IEKG3DRepresentObject* pObject = (IEKG3DRepresentObject*)pointer;
		if(!pObject)
			continue;


		IEKG3DRepresentObject * pOutObject = NULL;
		pPlacementTool->DoPlaceObjectByOther(pObject, &pOutObject);


		//�������ˣ������·���һ��npcinfo, npcinfo�б����¾ɲ��棬��Ҫ�Ѿɵ�npcinfo��������Ϣ��������
		KSceneEditorDocLogical::NPCInfo *infoOld = 0;
		KSceneEditorDocLogical::NPCInfo *infoNew = 0;

		//��npc�б��зֱ�����¾�����npcinfo
		for(std::list<KSceneEditorDocLogical::NPCInfo*>::iterator it = m_lpDocLogical->m_listNPCInfo.begin(); it!=  m_lpDocLogical->m_listNPCInfo.end(); it ++)
		{
			KSceneEditorDocLogical::NPCInfo *info = *it;

			if(info->pPointer == pObject)
				infoOld = info;

			if(info->pPointer == pOutObject)
				infoNew = info;

			if(infoOld && infoNew)
				break;
		}

		string newName = infoNew->szName;
		DWORD newTemplateId = infoNew->nTempleteID;
		DWORD newRepresentID = infoNew->dwRepresentID;

		assert(infoNew && infoOld);

		//���ݿ���
		*infoNew = *infoOld;

		//�ָ��Լ�������
		infoNew->szName = newName;
		infoNew->nTempleteID = newTemplateId;
		infoNew->dwRepresentID = newRepresentID;
		infoNew->pPointer = pOutObject;

        //�������滻��templateid�б����˵��ظ���
		if(find(templateids.begin(), templateids.end(), infoOld->nTempleteID) == templateids.end())
		{
			templateids.push_back(infoOld->nTempleteID);
		}

		//��npc��Ϣ�б���ɾ��
		m_lpDocLogical->DeleteNPC(infoOld);
		removeObjs.push_back(pObject);
		selObjs.push_back(pOutObject);
	}




	for(vector<IEKG3DRepresentObject *>::iterator it = removeObjs.begin(); it != removeObjs.end(); it ++)
	{
		IEKG3DRepresentObject * obj = *it;
		//��ѡ���б���ɾ��
		pSelTool->SetSelected(*obj, false);
		//�ӳ�����ɾ��
		m_lpSceneSceneEditor->RemoveRepresentObject(obj);
	}

	//�����Ժ����ӵ�ѡ���б�
	for(vector<IEKG3DRepresentObject *>::iterator it = selObjs.begin(); it != selObjs.end(); it ++)
	{
		IEKG3DRepresentObject * obj = *it;
		pSelTool->SetSelected(*obj, true);
	}

	

	return true;
}

void KSceneSceneEditorDialogNPC::OnBnClickedNpcReplaceOne()
{
	vector<DWORD> replacedTemplateids;
    ReplaceSelectedNpc(replacedTemplateids);

	//�滻��ɣ�����Ϊ����ѡ��״̬��������ȡ����ģ���б��ѡ��
	m_lpSceneSceneEditor->SetEditState(SCENESTATE_SELECT);

}

void KSceneSceneEditorDialogNPC::OnBnClickedNpcReplaceAll()
{
	//���滻����ѡ�е�
	vector<DWORD> replacedTemplateids;
	if(!ReplaceSelectedNpc(replacedTemplateids))
	{
		m_lpSceneSceneEditor->SetEditState(SCENESTATE_SELECT);
		return;
	}

	//�õ����ʰȡ������
	IEKG3DRepresentObject * obj;
	m_lpSceneSceneEditor->GetObjectInHandle(&obj);
	IEKG3DRepresentNPC *handlenpc = m_lpSceneSceneEditor->GetIEKG3DRepresentNPC(obj);

	DWORD templateId = 0;
	handlenpc->GetTemplateID(&templateId);

	DWORD representID = 0;
	handlenpc->GetRepresentID(&representID);

	char npcName[256];
	handlenpc->GetName(npcName, 256);

	IEKG3DSceneObjectPlacementTool* pPlacementTool = KSH::GetEngineInterfaceEx<IEKG3DSceneObjectPlacementTool>();
	IEKG3DSceneSelectionTool* pSelTool = NULL;
	if(FAILED(m_lpSceneSceneEditor->GetSelectionToolInterface(&pSelTool)))
		return;

	IEKG3DSceneSelectionToolAccessProxy* pProxy = NULL;
	if(FAILED(pSelTool->GetAccessProxy(&pProxy)))
		return;



	//�ڳ������������ң�ѡ�е��Ѿ����滻�ˣ���������ĵ������������Ѿ����ص�δ��ѡ�У�������Ҫ���滻�ľͳ�����
	vector<IEKG3DRepresentObject*> UnSelectedNeedReplaceNpcs;
	for(vector<DWORD>::iterator it  = replacedTemplateids.begin(); it!= replacedTemplateids.end(); it ++)
	{
	   DWORD &tid = *it;
       m_lpSceneSceneEditor->FindLoadedNpc(UnSelectedNeedReplaceNpcs, tid);
	}

	for(vector<IEKG3DRepresentObject*>::iterator it = UnSelectedNeedReplaceNpcs.begin(); it!= UnSelectedNeedReplaceNpcs.end(); it ++)
	{
       IEKG3DRepresentObject *pObject = *it;

	   IEKG3DRepresentObject * pOutObject = NULL;
	   pPlacementTool->DoPlaceObjectByOther(pObject, &pOutObject);


	   //�������ˣ������·���һ��npcinfo, npcinfo�б����¾ɲ��棬��Ҫ�Ѿɵ�npcinfo��������Ϣ��������
	   KSceneEditorDocLogical::NPCInfo *infoOld = 0;
	   KSceneEditorDocLogical::NPCInfo *infoNew = 0;

	   //��npc�б��зֱ�����¾�����npcinfo
	   for(std::list<KSceneEditorDocLogical::NPCInfo*>::iterator it2 = m_lpDocLogical->m_listNPCInfo.begin(); it2!=  m_lpDocLogical->m_listNPCInfo.end(); it2 ++)
	   {
		   KSceneEditorDocLogical::NPCInfo *info = *it2;

		   if(info->pPointer == pObject)
			   infoOld = info;

		   if(info->pPointer == pOutObject)
			   infoNew = info;

		   if(infoOld && infoNew)
			   break;
	   }

	   string newName = infoNew->szName;
	   DWORD newTemplateId = infoNew->nTempleteID;
	   DWORD newRepresentID = infoNew->dwRepresentID;

	   assert(infoNew && infoOld);

	   //���ݿ���
	   *infoNew = *infoOld;

	   //�ָ��Լ�������
	   infoNew->szName = newName;
	   infoNew->nTempleteID = newTemplateId;
	   infoNew->dwRepresentID = newRepresentID;
	   infoNew->pPointer = pOutObject;

	   //�ӿ�ѡ���б���ɾ��
	   pSelTool->SetSelected(*pObject, false);

	   //�ӳ�����ɾ��
	   m_lpSceneSceneEditor->RemoveRepresentObject(pObject);

	   //��npc��Ϣ�б���ɾ��
	   m_lpDocLogical->DeleteNPC(infoOld);

	}


	//ʣ�µ�һ����δ�����ص������У���ҲҪ���滻�ģ�ֱ�Ӹ���templateId  representID ��npcname��������Ϣ�ͺ���
	for(vector<DWORD>::iterator it  = replacedTemplateids.begin(); it!= replacedTemplateids.end(); it ++)
	{
		DWORD &tid =  *it;
		for(std::list<KSceneEditorDocLogical::NPCInfo*>::iterator it2 = m_lpDocLogical->m_listNPCInfo.begin(); it2!=  m_lpDocLogical->m_listNPCInfo.end(); it2 ++)
		{
			KSceneEditorDocLogical::NPCInfo *info = *it2;
			if(info->nTempleteID == tid)
			{
				info->szName = npcName;
				info->nTempleteID = templateId;
				info->dwRepresentID = representID;
			}
		}
	}


	//�滻��ɣ�����Ϊ����ѡ��״̬��������ȡ����ģ���б��ѡ��
	m_lpSceneSceneEditor->SetEditState(SCENESTATE_SELECT);
}
