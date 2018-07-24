#include "StdAfx.h"
#include "kg3dsceneeditorbase.h"
#include "KG3DEngineManager.h"
#include "KG3DRepresentObject.h"
#include "kg3dgraphicstool.h"
#include "afxres.h"
#include "KG3DSceneLayer.h"
#include "KG3DSceneCameraMovement.h"
#include "KG3DSceneFunctions.h"
#include "KG3DCollocator.h"
#include "KG3DSceneObjectTransformTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW_3DENGINE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

float CameraSpeed = 100.0;
bool    g_bIsTurnToMouseL = false;


KG3DSceneEditorBase::KG3DSceneEditorBase(void)
:KG3DSceneSelectBase(static_cast<KG3DScene&>(*this))
,m_pCustomReceiver(NULL)
,m_pSelectionBoxGetter(NULL)
{
	m_bEditing = TRUE;
	m_dwEditState = SCENESTATE_SELECT;
	m_pRootLayer = NULL;
    m_RtsActive = FALSE;

	m_fWheel = 1.f;

    m_bEnableMoveCam = TRUE;

}

KG3DSceneEditorBase::~KG3DSceneEditorBase(void)
{
	SAFE_DELETE(m_pCustomReceiver);
	SAFE_DELETE(m_pSelectionBoxGetter);
	UnInit();
}

struct KG3DSceneEditorBaseCustomReceiver : public KG3DMessageReceiver 
{
	KG3DSceneEditorBase* m_pSceneEditorBase;

#if defined(DEBUG) | defined(_DEBUG)
	KG3DMessageReceiverBase* m_pTesterReceiver;	//���ڸ����Գ����ȡ��Ϣ
#endif

	virtual void __stdcall OnReceiveMessage(const KG3DMessage& Message , IKG3DMessageSenderBase* pSender)
	{
#if defined(DEBUG) | defined(_DEBUG)
		//what the fuck
		//if(NULL != m_pTesterReceiver)
		//	m_pTesterReceiver->OnReceiveMessage(Message, pSender);
#endif
		return m_pSceneEditorBase->OnReceiveMessage(Message, pSender);
	}

	KG3DSceneEditorBaseCustomReceiver()
		:m_pSceneEditorBase(NULL)
#if defined(DEBUG) | defined(_DEBUG)
		,m_pTesterReceiver(NULL)
#endif
	{
	}
};

HRESULT KG3DSceneEditorBase::Init()
{
	//InitLayers();
	KG3DSceneSelectBase::Init();

	HRESULT hr = KG3DScene::Init();
	KG_COM_PROCESS_ERROR(hr);

	m_pSelectionBoxGetter = KSF::BuildDefaultSelectionBoxGetter();
	KG_PROCESS_ERROR(NULL != m_pSelectionBoxGetter);

	{
		//�þۺϵķ�ʽ�������Ǽ̳еķ�ʽ��������Ϣ
		m_pCustomReceiver = new KG3DSceneEditorBaseCustomReceiver;
		if(NULL != m_pCustomReceiver)
		{
			(static_cast<KG3DSceneEditorBaseCustomReceiver*>(m_pCustomReceiver))->m_pSceneEditorBase = this;

			//�¾ɵ�ѡ������ע���¼�������������ȫ�ֿ����л����ߵĹ���
			HRESULT hrAnother = KG3DSceneSelectBase::RegisterEventListener(KM_SELECT, *m_pCustomReceiver);
			_ASSERTE(SUCCEEDED(hrAnother) && _T("���ע�᲻�ɹ��Ļ������ܽ��յ�һЩ����ѡ����¼�����Ϣ"));	

			KG3DSceneSelectionTool& selToolNew = KG3DScene::GetSelectionTool();
			_ASSERTE(selToolNew.IsNewSelectionTool());

			hrAnother = selToolNew.RegisterEventListener(KM_SELECT, *m_pCustomReceiver);
			_ASSERTE(SUCCEEDED(hrAnother) && _T("���ע�᲻�ɹ��Ļ������ܽ��յ�һЩ����ѡ����¼�����Ϣ"));


			for (DWORD dwType = KG3DTYPE_TRANSFORM_TOOL_BEGIN; dwType <= KG3DTYPE_TRANSFORM_TOOL_END; ++dwType)
			{
				DWORD dwMsgType = KG3DTransformTool::GetTypeCorrespondingMessage(dwType);

				//�¾ɵ�TransformTool��ע���¼�
				hrAnother = KG3DScene::GetTransformTool(dwType).RegisterEventListener(dwMsgType, *m_pCustomReceiver);
				_ASSERTE(SUCCEEDED(hrAnother));

				hrAnother = KG3DSceneSelectBase::GetTransformToolInterface(dwType).RegisterEventListener(dwMsgType, *m_pCustomReceiver);
				_ASSERTE(SUCCEEDED(hrAnother));
			}
		}

		//����State�仯�ļ�������Ч�ģ�����ѡ�񹤾߻᲻������������û�кܺõ��ⲿ����㣬��ʱ����д������
#if defined(DEBUG) | defined(_DEBUG)
		DWORD dwOldState = m_dwEditState;
		LPVOID pTestData = reinterpret_cast<LPVOID>(static_cast<KG3DSceneEditorBase*>(this));
		TDD::RunTest(KG3DSceneEditorBase::TDDCheck, pTestData, _T("KG3DSceneEditorBase::TDDCheck"), TFILE, __LINE__);
		_ASSERTE(dwOldState == m_dwEditState);
#endif
		return S_OK;
	}
Exit0:
	return E_FAIL;
}

VOID	KG3DSceneEditorBase::TDDCheck(LPVOID pExtraData)
{
#if defined(DEBUG) | defined(_DEBUG)

	KG3DSceneEditorBase* pSceneEditorBase = reinterpret_cast<KG3DSceneEditorBase*>(pExtraData);
	_ASSERTE(NULL != pSceneEditorBase);
	KG3DSceneEditorBase& mySelf = *pSceneEditorBase;

	//<OnToggleStateMove Refactor>//������״̬�ĸı����ܱ��ػ��
	{
		DWORD dwOldState = mySelf.m_dwEditState;

		struct CustomListener : public KG3DMessageSingleSenderReceiver
		{
			BOOL m_bMoving;
			BOOL  m_bMoveEndCalled;

			virtual void __stdcall OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender )
			{
				if (Message.m_uMessage == KM_STATE_CHANGE)
				{
					DWORD_PTR uNewState = Message.m_dwWParam;
					DWORD_PTR uOldState = Message.m_dwLParam;
					if (uNewState != uOldState)
					{
						if (uNewState == SCENESTATE_MOVE)
						{
							m_bMoving = FALSE;
						}

						if(uOldState == SCENESTATE_MOVE)
						{
							m_bMoveEndCalled = TRUE;
						}
					}
				}
			}
		};

		CustomListener listener;
		HRESULT hr = mySelf.RegisterListener(KM_STATE_CHANGE, &listener);
		_ASSERTE(SUCCEEDED(hr) && _T("ע��Ӧ�óɹ�"));

		//Case1 �л���SCENESTATE_MOVE
		{
			listener.m_bMoving = TRUE;
			mySelf.m_dwEditState = SCENESTATE_SELECT;
			mySelf.SetEditState(SCENESTATE_MOVE);
			_ASSERTE(! listener.m_bMoving);
		}

		//Case2 ��SCENESTATE_MOVE�л�����,Ҫ����MoveEnd
		{
			listener.m_bMoveEndCalled = FALSE;
			mySelf.m_dwEditState = SCENESTATE_MOVE;
			mySelf.SetEditState(SCENESTATE_SELECT);
			_ASSERTE(listener.m_bMoveEndCalled);
		}

		mySelf.m_dwEditState = dwOldState;
	}
#endif
}

#define  ILLEGAL_PTR ((LPVOID)0x00000003) //SceneEntity�Ĺ��캯���������ָ�룬����ƭ��һ��
#define  ILLEGAL_PTR2 ((LPVOID)0x00000004)
VOID KG3DSceneEditorBase::TDDCheckAfterInitialized(LPVOID pExtraData)
{
	return;
#if defined(DEBUG) | defined(_DEBUG)

	KG3DSceneEditorBase* pSceneEditorBase = reinterpret_cast<KG3DSceneEditorBase*>(pExtraData);
	_ASSERTE(NULL != pSceneEditorBase);
	KG3DSceneEditorBase& mySelf = *pSceneEditorBase;

	static bool s_bHasCheck = false;
	if (s_bHasCheck)
	{
		return;
	}
	else
	{
		s_bHasCheck = true;
	}

	
	{
		struct CustomListener : public KG3DMessageSingleSenderReceiver
		{
			std::vector<KG3DMessage> m_messages;
			virtual void __stdcall OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender )
			{
				_ASSERTE(m_messages.size() < 4);
				m_messages.push_back(Message);
			}
			BOOL	HasSuchEvent(DWORD uMsgID, DWORD_PTR lParamEventState)
			{
				for (size_t i = 0; i < m_messages.size(); ++i)
				{
					KG3DMessage& msg = m_messages[i];
					if(msg.m_uMessage == uMsgID && msg.m_dwLParam == lParamEventState)
					{
						return TRUE;
					}
				}
				return FALSE;
			}
		};
		CustomListener tempLst;

		//��EditorBase�Ľ������еĲ��Բ�۽���һ����ʱ�Ľ��������ػ����е���Ϣ
		KG3DSceneEditorBaseCustomReceiver* pCustomReceiver = NULL; 
		try
		{
			pCustomReceiver = dynamic_cast<KG3DSceneEditorBaseCustomReceiver*>(mySelf.m_pCustomReceiver);	
		}
		catch (...)
		{
			_ASSERTE(NULL && _T("���಻��ȷ"));
		}
		_ASSERTE(NULL != pCustomReceiver);

		//��ʼ�����������
		
		pCustomReceiver->m_pTesterReceiver = &tempLst;
		DWORD dwOldState = mySelf.m_dwEditState;		
		BOOL bNewSelectionSave = g_Switch.bNewSelection;


		//����ѡ���꣬��û�з���ѡ����ɵ���Ϣ
		{
			KG3DSceneSelectionTool* pSelTool = static_cast<KG3DSceneSelectBase*>(&mySelf);
			KG3DCollocator::ToggleGlobalSwitch(g_Switch.bNewSelection, FALSE);

			size_t iSelToolCount = 2;//�¾�����
			for(size_t whichTool = 0; whichTool < iSelToolCount; ++whichTool)
			{
				tempLst.m_messages.clear();

				//�л���ѡ��״̬
				KG3DMessageSender* pSenderForTest = pSelTool->TestBegin();
				_ASSERTE(NULL != pSenderForTest);

				pSenderForTest->PostMessage(KG3DMessage(KM_STATE_CHANGE, SCENESTATE_SELECT, SCENESTATE_NONE));

				typedef KG3DMessageActionExecutionStruct ExecStruct;

				//ǿ�Ʒ���һ���������Ϣ
				ExecStruct execStruct;		
				execStruct = ExecStruct(EXEACTION_LEFE_KEY_DOWN, 1, 0, 0);
				pSenderForTest->PostMessage(KG3DMessage(KM_EXECUTE_ACTION, reinterpret_cast<DWORD_PTR>(&execStruct), 0));
				execStruct = ExecStruct(EXEACTION_LEFE_KEY_DOWN, 0, 0, 0);
				pSenderForTest->PostMessage(KG3DMessage(KM_EXECUTE_ACTION, reinterpret_cast<DWORD_PTR>(&execStruct), 0));

				//ȷ�ϵ���ᴥ��ѡ����ɵ���Ϣ
				//BOOL bReceived = tempLst.HasSuchEvent(KM_SELECT, EM_EVENT_STATE_END);
				//_ASSERTE(bReceived);

				pSelTool->TestEnd();

				KG3DCollocator::ToggleGlobalSwitch(g_Switch.bNewSelection, TRUE);
				pSelTool = &mySelf.KG3DScene::GetSelectionTool();//��һ�β�����ϣ��л����µ�ѡ�񹤾�
			}
		}
		//����Proxy
		{
			KG3DSceneSelectionTool* pSelTool = static_cast<KG3DSceneSelectBase*>(&mySelf);
			KG3DCollocator::ToggleGlobalSwitch(g_Switch.bNewSelection, FALSE);

			size_t iSelToolCount = 2;
			for(size_t whichTool = 0; whichTool < iSelToolCount; ++whichTool)
			{
				pSelTool->TestBegin();

				//���һ��ѡ��
				{
					//pSelTool->SetSelectable(KG3DSceneEntity(SCENEENTITY_TEST, ILLEGAL_PTR), TRUE);//<Selectable Refactor>
					mySelf.IEInsertEntitySelectable(SCENEENTITY_TEST, ILLEGAL_PTR, TRUE);
					pSelTool->SetSelected(KG3DSceneEntity(SCENEENTITY_TEST, ILLEGAL_PTR), TRUE);

					_ASSERTE(1 == pSelTool->GetSelectionCount());

					IEKG3DSceneSelectionToolAccessProxy* pAccessProxy = NULL;
					pSelTool->GetAccessProxy(&pAccessProxy);
					_ASSERTE(NULL != pAccessProxy);

					IEKG3DSceneEntity* pEntity = pAccessProxy->Reset();
					_ASSERTE(NULL != pEntity);

					SCENEENTITYTYPE tempType = SCENEENTITY_NONE;
					pEntity->GetType(&tempType);
					_ASSERTE(tempType == SCENEENTITY_TEST);

					LPVOID pTemp = NULL;
					pEntity->GetPointer(&pTemp);
					_ASSERTE(pTemp == ILLEGAL_PTR);

					pEntity = pAccessProxy->Next();
					_ASSERTE(NULL == pEntity);

					_ASSERTE(NULL == pAccessProxy->Next());
				}

				//�������ѡ��
				{
					pSelTool->ClearSelection();
					//pSelTool->SetSelectable(KG3DSceneEntity(SCENEENTITY_TEST, ILLEGAL_PTR), TRUE);//<Selectable Refactor>
					mySelf.IEInsertEntitySelectable(SCENEENTITY_TEST, ILLEGAL_PTR, TRUE);
					pSelTool->SetSelected(KG3DSceneEntity(SCENEENTITY_TEST, ILLEGAL_PTR), TRUE);
					//pSelTool->SetSelectable(KG3DSceneEntity(SCENEENTITY_TEST, ILLEGAL_PTR2), TRUE);
					mySelf.IEInsertEntitySelectable(SCENEENTITY_TEST, ILLEGAL_PTR2, TRUE);//<Selectable Refactor>
					pSelTool->SetSelected(KG3DSceneEntity(SCENEENTITY_TEST, ILLEGAL_PTR2), TRUE);

					IEKG3DSceneSelectionToolAccessProxy* pAccessProxy = NULL;
					pSelTool->GetAccessProxy(&pAccessProxy);
					_ASSERTE(NULL != pAccessProxy);

					_ASSERTE(2 == pSelTool->GetSelectionCount());
					KG3DSceneEntity* pEntity = static_cast<KG3DSceneEntity*>(pAccessProxy->Reset());
					_ASSERTE(NULL != pEntity && pEntity->m_pPointer == ILLEGAL_PTR);
					pEntity = static_cast<KG3DSceneEntity*>(pAccessProxy->Next());
					_ASSERTE(NULL != pEntity && pEntity->m_pPointer == ILLEGAL_PTR2);

					pEntity = static_cast<KG3DSceneEntity*>(pAccessProxy->Next());
					_ASSERTE(NULL == pEntity);
				}

				
				pSelTool->TestEnd();

				KG3DCollocator::ToggleGlobalSwitch(g_Switch.bNewSelection, TRUE);
				pSelTool = &mySelf.KG3DScene::GetSelectionTool();//��һ�β�����ϣ��л����µ�ѡ�񹤾�
			}
		}
		//�����ƶ�����Ƿ��յ���Ϣ
		{
			struct TransToolTest 
			{
				DWORD dwToolType;
				DWORD dwToolEditState;
				DWORD dwToolMsg;
			};

			TransToolTest transToolTests[] = 
			{
				{KG3DTYPE_TRANSLATION_TOOL, SCENESTATE_MOVE, KM_TRANSLATION},
				{KG3DTYPE_ROTATION_TOOL, SCENESTATE_ROTATE, KM_ROTATION},
				{KG3DTYPE_SCALING_TOOL, SCENESTATE_SCALE, KM_SCALING},
			};

			for (size_t iToolTest = 0; iToolTest < _countof(transToolTests); ++iToolTest)
			{
				TransToolTest& curTestData = transToolTests[iToolTest];

				KG3DTransformTool* pTransTool = &mySelf.KG3DSceneSelectBase::GetTransformToolInterface(curTestData.dwToolType);
				KG3DSceneSelectionTool* pSelTool = static_cast<KG3DSceneSelectBase*>(&mySelf);
				KG3DCollocator::ToggleGlobalSwitch(g_Switch.bNewSelection, FALSE);

				size_t iSelToolCount = 2;//�¾�����
				for(size_t whichTool = 0; whichTool < iSelToolCount; ++whichTool)
				{
					tempLst.m_messages.clear();

					pSelTool->TestBegin();

					//���һ��ѡ�񣬲�Ȼû��Transform��SCENEENTITY_TEST��BBox����ԭ���
					//pSelTool->SetSelectable(KG3DSceneEntity(SCENEENTITY_TEST, ILLEGAL_PTR), TRUE);//<Selectable Refactor>
					mySelf.IEInsertEntitySelectable(SCENEENTITY_TEST, ILLEGAL_PTR, TRUE);
					pSelTool->SetSelected(KG3DSceneEntity(SCENEENTITY_TEST, ILLEGAL_PTR), TRUE);

					KG3DMessageSender* pSenderForTest = pTransTool->TestBegin();
					_ASSERTE(NULL != pSenderForTest);

					//�������������Ȼ��Ŀ��̫���ǵò�����Ϣ��
					{
						D3DXVECTOR3 tempCamPos(0,0,-1010);
						pSenderForTest->PostMessage(KG3DMessage(KM_CAM_POS, reinterpret_cast<DWORD_PTR>(&tempCamPos), NULL));
					}

					//�ѳ����༭״̬�ĳɶ�Ӧ�ı༭״̬
					pSenderForTest->PostMessage(KG3DMessage(KM_STATE_CHANGE, curTestData.dwToolEditState, SCENESTATE_NONE));

					mySelf.m_dwEditState = curTestData.dwToolEditState;

					pTransTool->FrameMove();

					typedef KG3DMessageActionExecutionStruct ExecStruct;

					//���������OutputWnd
					struct TestOutputWnd : public IEKG3DSceneOutputWndDummy 
					{
						virtual HRESULT GetPickRay(D3DXVECTOR3* pRaySrc,D3DXVECTOR3* pNormalizedDir, D3DXVECTOR2* pRaySrc2)
						{
							_ASSERTE(NULL != pRaySrc && NULL != pNormalizedDir);
							*pRaySrc = D3DXVECTOR3(0,0,-1000);
							*pNormalizedDir = D3DXVECTOR3(0,0,1);
							D3DXVec3Normalize(pNormalizedDir, pNormalizedDir);

							if(NULL != pRaySrc2)
								*pRaySrc2 = D3DXVECTOR2(0,0);
							return S_OK;
						}
					};
					TestOutputWnd testOutputWnd;

					//ģ�ⷢ��һϵ������ƶ�����Ϣ
					ExecStruct execStruct;		
					execStruct = ExecStruct(EXEACTION_LEFE_KEY_DOWN, 1, 0, 0);
					KG3DMessage msgPress(KM_EXECUTE_ACTION, reinterpret_cast<DWORD_PTR>(&execStruct)
						, reinterpret_cast<DWORD_PTR>(&testOutputWnd));
					pSenderForTest->PostMessage(msgPress);

					pTransTool->FrameMove();

					execStruct = ExecStruct(EXEACTION_MOUSE_MOVE, 1, 0, 0);
					KG3DMessage msgMove(KM_EXECUTE_ACTION, reinterpret_cast<DWORD_PTR>(&execStruct)
						, reinterpret_cast<DWORD_PTR>(&testOutputWnd));
					pSenderForTest->PostMessage(msgMove);

					pTransTool->FrameMove();

					execStruct = ExecStruct(EXEACTION_LEFE_KEY_DOWN, 0, 0, 0);
					KG3DMessage msgUp(KM_EXECUTE_ACTION, reinterpret_cast<DWORD_PTR>(&execStruct)
						, reinterpret_cast<DWORD_PTR>(&testOutputWnd));
					pSenderForTest->PostMessage(msgUp);

					pTransTool->FrameMove();

					//ȷ���ƶ���ɵ���Ϣ
					BOOL bReceived = tempLst.HasSuchEvent(curTestData.dwToolMsg, EM_EVENT_STATE_END);
					
					//_ASSERTE(bReceived && _T("û���յ���ȷ���¼�"));

					//�����Ϣ������ֱ�ӽ����ƶ��������Ȳ����ܷ��յ���ȷ����Ϣ
					tempLst.m_messages.clear();

					pTransTool->TransformDirect(D3DXVECTOR3(1,2,3));
					bReceived = tempLst.HasSuchEvent(curTestData.dwToolMsg, EM_EVENT_STATE_END);
					//_ASSERTE(bReceived);
					

					pTransTool->TestEnd();
					pSelTool->TestEnd();

					KG3DCollocator::ToggleGlobalSwitch(g_Switch.bNewSelection, TRUE);
					pTransTool = &mySelf.KG3DScene::GetTransformTool(curTestData.dwToolType);
					pSelTool = &mySelf.KG3DScene::GetSelectionTool();//��һ�β�����ϣ��л����µ�ѡ�񹤾�

				}//for(size_t whichTool = 0;
			}//for (size_t iToolTest;
		}
		
		//�Ӳ����˳�����ԭ״̬
		pCustomReceiver->m_pTesterReceiver = NULL;
		mySelf.m_dwEditState = dwOldState;

		KG3DCollocator::ToggleGlobalSwitch(g_Switch.bNewSelection, bNewSelectionSave);
	}
#endif
}

HRESULT KG3DSceneEditorBase::UnInit()
{
	//UnInitLayers();
	KG3DSceneSelectBase::UnInit();
	return KG3DScene::UnInit();
}


HRESULT KG3DSceneEditorBase::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)//<PROCESS_MESSAGE>
{
	switch(message)
	{
	case WM_LBUTTONDOWN:
		{
			g_bIsTurnToMouseL = true;
            
			g_Mouse_L = TRUE;
			MsgOnLButtonDown(wParam,lParam);
			break;
		}
	case WM_LBUTTONUP:
		{
         //   g_bIsTurnToMouseL = false;
			g_Mouse_L = FALSE;
			MsgOnLButtonUp(wParam,lParam);
			break;
		}
	case WM_MBUTTONDOWN:
		{
			g_Mouse_M = TRUE;
			MsgOnMButtonDown(wParam,lParam);
			break;
		}
	case WM_MBUTTONUP:
		{
			g_Mouse_M = FALSE;
			MsgOnMButtonUp(wParam,lParam);
			break;
		}
	case WM_RBUTTONDOWN:
		{
			g_Mouse_R = TRUE;
			MsgOnRButtonDown(wParam,lParam);
			break;
		}
	case WM_RBUTTONUP:
		{
			g_Mouse_R = FALSE;
			MsgOnRButtonUp(wParam,lParam);
			break;
		}
	case 0x020A://WM_MOUSEWHEEL:
		{
			MsgOnMouseWheel(wParam,lParam);			
			break;
		}
	case WM_MOUSEMOVE:
		{
			MsgOnMouseMove(wParam,lParam);
			break;
		}
	case WM_KEYDOWN:
		{
			g_KeyState[wParam] = 1;
			MsgOnKeyDown(wParam,lParam);
			break;
		}
	case WM_KEYUP:
		{
			g_KeyState[wParam] = 0;
			MsgOnKeyUp(wParam,lParam);
			break;
		}
	case ID_EDIT_COPY:
		{
			//2009-1-12 <Copy Refactor>���Ƹĳ�3dsmax�ķ�ʽ������Ҫ��Ctrl+C��¼ѡ�񣬶���ֱ����Ctrl+V�͸��Ƴ���ǰ��ѡ�񡣼�¼ѡ�����������Ҫ����̫���������Ե����⣬����ҲӦ�þ�����Max�Ĳ�������
			MessageBox(NULL, _T("���Ʋ���ҪCtrl+C��ֱ�Ӱ�Ctrl+V�Ϳ��Ը���ѡ����������ο�3dsmax�Ĳ���"), NULL, MB_OK);
			//CopySelectedEntity();
			break;
		}
	case ID_EDIT_PASTE:
		{
			this->PasteSelection();
			break;
		}
	}

	KG3DWindowsMessage windowMessage(message, wParam, lParam);
	KG3DMessage messageToSend(KM_WINDOWS_MESSAGE, reinterpret_cast<DWORD_PTR>(&windowMessage), 0);
	this->GetMessageSender(KM_WINDOWS_MESSAGE).PostMessage(messageToSend);
	return S_OK;
}

void KG3DSceneEditorBase::MsgOnLButtonDown(WPARAM wParam, LPARAM lParam)//<PROCESS_MESSAGE>
{
	//�ƶ���SceneSelectBase��HandleWindowsMessages
	//if(m_dwEditState==SCENESTATE_SELECT)
	//{
	//	if(m_SceneWndManager->GetCurOutputWnd())
	//	{	
	//		if(!IsSelecting())
	//		{
	//			D3DXVECTOR3 RayOrig,RayDir;
	//			D3DXVECTOR2 RayOrig2D;
	//			m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);
	//			if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	//				SelectBegin(RayOrig, RayDir, RayOrig2D, EM_SELECTION_INCREASE_MODE_ADD);
	//			else if (GetAsyncKeyState(VK_LMENU)  & 0x8000)
	//				SelectBegin(RayOrig, RayDir, RayOrig2D, EM_SELECTION_INCREASE_MODE_SUBTRACT);
	//			else
	//				SelectBegin(RayOrig, RayDir, RayOrig2D, EM_SELECTION_INCREASE_MODE_DEFAULT);
	//		}
	//	}
	//}
	//else if(m_dwEditState==SCENESTATE_MOVE)
	//{
	//	if(m_SceneWndManager->GetCurOutputWnd())
	//	{	
	//		D3DXVECTOR3 RayOrig,RayDir;
	//		D3DXVECTOR2 RayOrig2D;
	//		m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);
	//		MoveBegin(RayOrig,RayDir);
	//		MoveFrameMove(/*&m_SceneWndManager->GetCurOutputWnd()->GetCamera(),RayOrig,RayDir,0,0*/);//<Move Refactor>������Ч
	//	}
	//}
	//else if(m_dwEditState==SCENESTATE_ROTATE)
	//{
	//	if(m_SceneWndManager->GetCurOutputWnd())
	//	{	
	//		D3DXVECTOR3 RayOrig,RayDir;
	//		D3DXVECTOR2 RayOrig2D;
	//		m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);
	//		RotateBegin(RayOrig,RayDir);
	//	}
	//}
	//else if(m_dwEditState==SCENESTATE_SCALE)
	//{
	//	if(m_SceneWndManager->GetCurOutputWnd())
	//	{	
	//		D3DXVECTOR3 RayOrig,RayDir;
	//		D3DXVECTOR2 RayOrig2D;
	//		m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);
	//		ScaleBegin(RayOrig,RayDir);
	//	}
	//}
	//else 
	if (m_dwEditState == SCENESTATE_COPY)
	{
		if(m_SceneWndManager->GetCurOutputWnd())
		{	
			D3DXVECTOR3 RayOrig,RayDir;
			D3DXVECTOR2 RayOrig2D;
			m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);

			float x = RayOrig.x - (RayOrig.y * RayDir.x) / RayDir.y;
			float y = 0;
			float z = RayOrig.z - (RayOrig.y * RayDir.z) / RayDir.y;

			D3DXVECTOR3 vCenter;
			KSF::GetSelectionCenter(this->GetSelectionTool(), vCenter);
			D3DXVECTOR3 vOffset = D3DXVECTOR3(x, y, z) - vCenter;

			TypeEntityAccessProxy& proxy = this->GetSelectionTool().GetSelectionAccessProxy();
			for(proxy.Begin(); !proxy.IsEnd(); proxy.StepForward())
			{
				KG3DSceneEntity& EntitySrc = proxy.GetCur();
				KG3DSceneEntity Entity;
				D3DXVECTOR3 vPos;
				EntitySrc.GetTranslation(&vPos);
				x = vPos.x + vOffset.x;
				y = vPos.y + vOffset.y;
				z = vPos.z + vOffset.z;
				if(SUCCEEDED(EntitySrc.Clone(&Entity, x, y, z)))
				{
					m_listRenderEntity.push_back(Entity);
					
					//this->GetSelectionTool().SetSelectable(Entity, TRUE);//<Selectable Refactor>
					this->InsertEntitySelectable(Entity, TRUE);
				}
			}
		}
	}
}

void KG3DSceneEditorBase::MsgOnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	//����Ĵ����ƶ���SceneSelectBase����
	/*if(m_dwEditState==SCENESTATE_SELECT)	
	{
		if(IsSelecting())
		{
			if(m_SceneWndManager->GetCurOutputWnd())
			{
				D3DXMATRIX Proj,View;
				Proj = m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetProjectionMatrix();
				View = m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetViewMatrix();

				SelectEnd(m_SceneWndManager->GetCurOutputWnd()->m_Viewport,Proj,View);
			}
			
		}
	}
	else if(m_dwEditState==SCENESTATE_MOVE)
	{
		MoveEnd();
	}
	else if(m_dwEditState==SCENESTATE_ROTATE)
	{
		if(m_bRotating)
		{
			RotateEnd();
		}
	}
	else if(m_dwEditState==SCENESTATE_SCALE)
	{
		if(m_bScaling)
		{
			ScaleEnd();
		}
	}*/
}

void KG3DSceneEditorBase::MsgOnRButtonDown(WPARAM wParam, LPARAM lParam)
{
}

void KG3DSceneEditorBase::MsgOnRButtonUp(WPARAM wParam, LPARAM lParam)
{
}

void KG3DSceneEditorBase::MsgOnMButtonDown(WPARAM wParam, LPARAM lParam)
{

}
void KG3DSceneEditorBase::MsgOnMButtonUp(WPARAM wParam, LPARAM lParam)
{
}

void KG3DSceneEditorBase::MsgOnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	int k = 0;
	k |=wParam;
	float S = 1.0F;
	if (k>0)
	{
		S = -0.9F;
		m_fWheel -= 0.05f;
	}
	else
	{
		S = 1.1F;
		m_fWheel += 0.05f;
	}


	if(m_SceneWndManager->GetCurOutputWnd())
	{
		D3DXVECTOR3 PosMove(0,0,0);
		D3DXVECTOR3 Pos,LookAt;
		m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetPosition(&Pos);
		m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetLookAtPosition(&LookAt);

		D3DXVECTOR3 N = Pos - LookAt;
		float R = D3DXVec3Length(&N);
		if((R>=500)||(k<0))//����һ����С���룬�������С�Ļ���ͬʱ�ƶ��������������
		{
			D3DXVec3Normalize(&N,&N);

			float H = 1;
			if(GetAsyncKeyState(VK_LCONTROL) & 0x8000)
				H = 30;
			Pos = Pos +  N * S * R * 0.1F;
			m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetPosition(Pos);
		}
		else
		{
			D3DXVec3Normalize(&N,&N);

			float H = 1;
			if(GetAsyncKeyState(VK_LCONTROL) & 0x8000)
				H = 30;
			Pos = Pos + N * R * 0.1F * S;
			
			LookAt = LookAt + N * R * 0.1F * S;
			/*m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetPosition(Pos);
			m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetLookAtPosition(LookAt);*/
			KG3DCamera& Cam = m_SceneWndManager->GetCurOutputWnd()->GetCamera();
			
			Cam.SetPositionLookAtUp(&Pos, &LookAt, NULL);
		}
	}
}
//<PROCESS_MESSAGE>
void KG3DSceneEditorBase::MsgOnMouseMove(int fwKeys,int xPos,int yPos,KG3DSceneOutputWnd *pWnd)
{
	if(!pWnd)
		return;


	D3DXVECTOR3 PosMove(0,0,0);
	D3DXVECTOR3 Pos,LookAt;
	pWnd->GetCamera().GetPosition(&Pos);
	pWnd->GetCamera().GetLookAtPosition(&LookAt);

	D3DXVECTOR3 Right = pWnd->GetCamera().GetCameraRight();
	D3DXVECTOR3 Left = pWnd->GetCamera().GetCameraRight();
	D3DXVECTOR3 Front = pWnd->GetCamera().GetCameraFront();
	D3DXVECTOR3 Up = pWnd->GetCamera().GetCameraUp();

	if((g_Mouse_M)&&(GetAsyncKeyState(VK_LCONTROL ) & 0x8000))
	{

	}
	else if ((g_Mouse_M)&&(!(GetAsyncKeyState(VK_LMENU) & 0x8000)))
	{

	}
	else if ((g_Mouse_M)&&(GetAsyncKeyState(VK_LMENU) & 0x8000))
	{

	}
	
	//2009-1-19 �ƶ���SceneSelectBase����
	/*if(m_dwEditState==SCENESTATE_SELECT)
	{
		if(IsSelecting())
		{
			D3DXVECTOR3 RayOrig,RayDir;
			D3DXVECTOR2 RayOrig2D;
			pWnd->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);
			SelectFrameMove(RayOrig, RayDir, RayOrig2D);
		}
	}*/
}

void KG3DSceneEditorBase::MsgOnMouseMove(WPARAM wParam, LPARAM lParam)
{
	int fwKeys = (int)wParam; 
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);


	//(by dengzhihui 2006��8��17�� 15:24:43
	//���޷�����תΪ�з�������
	xPos = xPos > 0x7fff ? (xPos - 0xffff) : xPos;
	yPos = yPos > 0x7fff ? (yPos - 0xffff) : yPos;
	//)
	MsgOnMouseMove(fwKeys,xPos,yPos,m_SceneWndManager->GetCurOutputWnd());

	g_nMouseMoveX = xPos;
	g_nMouseMoveY = yPos;
}

void KG3DSceneEditorBase::MsgOnKeyDown(WPARAM wParam, LPARAM lParam)
{
    IEKG3DSceneCameraAnimation *piCameraAnimation = NULL;
	HRESULT hr;
	switch(wParam)
	{
	case '1':
		SetEditState(SCENESTATE_SELECT);
        m_pCameraMovement->GetCurCamAnimation(&piCameraAnimation);
		piCameraAnimation->StopPlay();
		break;

	case '2':
		{
			if(this->GetSelectionTool().GetSelectionCount() == 0)
				break;

			SetEditState(SCENESTATE_MOVE);
			//OnToggleStateMove();//<OnToggleStateMove Refactor>//SceneSelectBase�����SetEditState�����ڲ��ı�״̬

			//if(m_SceneWndManager->GetCurOutputWnd())	//2009-1-22 ȥ�����ڲ����Զ���Ӧ
			//{
			//	D3DXVECTOR3 RayOrig,RayDir;
			//	m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir, NULL);
			//	MoveFrameMove();//<Move Refactor>������Ч
			//}
		}
        break;

	case '3':
        {
			if(this->GetSelectionTool().GetSelectionCount() == 0)
				break;

            SetEditState(SCENESTATE_ROTATE);
            //OnToggleStateRotation();//<OnToggleStateRotation Refactor>//SceneSelectBase�����SetEditState�����ڲ��ı�״̬
        }
		break;
	case '4':
        {
		    SetEditState(SCENESTATE_SCALE);
			//OnToggleStateScaling();//<OnToggleStateScaling Refactor>//SceneSelectBase�����SetEditState�����ڲ��ı�״̬

			if(this->GetSelectionTool().GetSelectionCount() == 0)
				break;
        }
        break;
	case 'F':
		{
			 SetEditState(SCENESTATE_TERRAINSELECTION);
		}
		break;

    case 'C':
    case 'c':
        SetEditState(SCENESTATE_COPY);
		break;
	case 'Z':
		//m_bFocusing = !m_bFocusing;
		{
			//if(/*(m_bFocusing)&&*/(m_SceneWndManager->GetCurOutputWnd()))
			//{
			//	D3DXVECTOR3 Pos;
			//	HRESULT hrTemp = KSF::GetSelectionCenter(this->GetSelectionTool(), Pos);
			//	if(FAILED(hrTemp))
			//		break;
			//	
			//		AABBOX Box;
			//		//GetSelectedEntityBBox(Box);

			//		hrTemp = GetSelectionBBoxGetter().GetBBox(this->GetSelectionTool(), Box);
			//		if(FAILED(hrTemp))
			//			break;

			//		float R =  D3DXVec3Length(&(Box.A - Box.B));
			//		D3DXVECTOR3 Lookat,Eye;
			//		m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetLookAtPosition(&Lookat);
			//		m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetPosition(&Eye);

			//		D3DXVECTOR3 N = Eye - Lookat;
			//		D3DXVec3Normalize(&N,&N);
			//		Lookat = Pos;
			//		Eye = Pos + N * R;
			//		//m_bFocusing = FALSE;

			//		/*m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetLookAtPosition(Lookat);
			//		m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetPosition(Eye);*/
			//		m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetPositionLookAtUp(&Eye,&Lookat,NULL);
			//	
			//}
			ZoomToSelection();
		}
		break;
	case VK_DELETE:
		//DeleteSelectedEntity();
		break;
	case 'M':
		{
            m_pCameraMovement->GetCurCamAnimation(&piCameraAnimation);
			piCameraAnimation->AddKeyFrame();
		}
		break;
	case 'U':
        m_pCameraMovement->GetCurCamAnimation(&piCameraAnimation);
        hr = piCameraAnimation->StartPlay(TRUE);
        if ( SUCCEEDED( hr ) )
        {
             SetEditState(SCENESTATE_CAMERAANIMATION);	
		}			
		break;
    case 'V' :
        if(m_SceneWndManager->GetCurOutputWnd())
        {	
			TypeEntityAccessProxy& proxy = this->GetSelectionTool().GetSelectionAccessProxy();
			for(proxy.Begin(); !proxy.IsEnd(); proxy.StepForward())
            {
                //KG3DSceneEntity &EntitySrc = (*i);
				KG3DSceneEntity& EntitySrc = proxy.GetCur();
                KG3DSceneEntity Entity = EntitySrc;
                D3DXVECTOR3 vPos;
                EntitySrc.GetTranslation(&vPos);
                //EntitySrc.Clone(&Entity, vPos.x, vPos.y, vPos.z);
				Entity.SetTranslation(&vPos);
                m_listRenderEntity.push_back(Entity);
                
				//this->GetSelectionTool().SetSelectable(Entity, TRUE);//<Selectable Refactor>
				this->InsertEntitySelectable(Entity, TRUE);
            }
            SetEditState(SCENESTATE_SELECT);
        }
        break;
	default:
		break;
	}

	D3DXVECTOR3 Pos(0,0,0);
	D3DXQUATERNION Rot(0,1,0,0);

	if(m_dwEditState==SCENESTATE_MOVE)
	{
		switch(wParam)
		{
		case VK_UP:
			Pos = D3DXVECTOR3(0,0,5);
			//MoveDirect(Pos);
			this->GetTransformTool(KG3DTYPE_TRANSLATION_TOOL).TransformDirect(Pos);
			break;	
		case VK_DOWN:
			Pos = D3DXVECTOR3(0,0,-5);
			//MoveDirect(Pos);
			this->GetTransformTool(KG3DTYPE_TRANSLATION_TOOL).TransformDirect(Pos);
			break;	
		case VK_LEFT:
			Pos = D3DXVECTOR3(-5,0,0);
			//MoveDirect(Pos);
			this->GetTransformTool(KG3DTYPE_TRANSLATION_TOOL).TransformDirect(Pos);
			break;	
		case VK_RIGHT:
			Pos = D3DXVECTOR3(5,0,0);
			//MoveDirect(Pos);
			this->GetTransformTool(KG3DTYPE_TRANSLATION_TOOL).TransformDirect(Pos);
			break;
		}
	}
	else if(m_dwEditState==SCENESTATE_ROTATE)
	{
		FLOAT f5Degree = D3DX_PI / 36;
		switch(wParam)
		{
		case VK_UP:
			/*D3DXQuaternionRotationYawPitchRoll(&Rot, 0,           D3DX_PI/36,  0);
			RotateDirect(Rot, &D3DXVECTOR3(0, D3DX_PI / 36, 0));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(0, f5Degree, 0));
			break;	
		case VK_DOWN:
			/*D3DXQuaternionRotationYawPitchRoll(&Rot, 0,           -D3DX_PI/36, 0);
			RotateDirect(Rot, &D3DXVECTOR3(0,-D3DX_PI / 36, 0));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(0, -f5Degree, 0));
			break;	
		case VK_LEFT:
			/*D3DXQuaternionRotationYawPitchRoll(&Rot, D3DX_PI/36,  0,           0);
			RotateDirect(Rot, &D3DXVECTOR3(D3DX_PI / 36, 0, 0));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(f5Degree, 0, 0));
			break;	
		case VK_RIGHT:
			/*D3DXQuaternionRotationYawPitchRoll(&Rot, -D3DX_PI/36, 0,           0);
			RotateDirect(Rot, &D3DXVECTOR3(-D3DX_PI / 36, 0, 0));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(-f5Degree, 0, 0));
			break;	
		case VK_PRIOR:
			/*D3DXQuaternionRotationYawPitchRoll(&Rot,0,0,D3DX_PI/36);
			RotateDirect(Rot, &D3DXVECTOR3(0, 0, D3DX_PI / 36));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(0, 0, f5Degree));
			break;	
		case VK_NEXT:
			/*D3DXQuaternionRotationYawPitchRoll(&Rot,0,0,-D3DX_PI/36);
			RotateDirect(Rot, &D3DXVECTOR3(0, 0, -D3DX_PI / 36));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(0, 0, -f5Degree));
			break;
        case VK_END:
            /*D3DXQuaternionRotationYawPitchRoll(&Rot, 0,           0,           D3DX_PI/36);
            RotateDirect(Rot, &D3DXVECTOR3(0, 0, D3DX_PI / 36));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(0, 0, f5Degree));
            break;
        case VK_HOME:
            /*D3DXQuaternionRotationYawPitchRoll(&Rot, 0,           0,           -D3DX_PI/36);
            RotateDirect(Rot, &D3DXVECTOR3(0, 0, -D3DX_PI / 36));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(0, 0, -f5Degree));
            break;
		}
	}
	else if(m_dwEditState==SCENESTATE_SCALE)
	{
		switch(wParam)
		{
		case VK_UP:
            if (!GetAsyncKeyState(VK_CONTROL))
            {
                D3DXVECTOR3 Scale(1.1f, 1.1f, 1.1f);
                //ScaleDirect(Scale);
				this->GetTransformTool(KG3DTYPE_SCALING_TOOL).TransformDirect(Scale);
                //ScaleDirectSize(1.1f);
				KSF::SetSelectionSFXScaleSize(this->GetSelectionTool(), 1.1f);
            }
            else
            {
                //ScaleDirectSize(1.1f);
				KSF::SetSelectionSFXScaleSize(this->GetSelectionTool(), 1.1f);
            }

			break;

		case VK_DOWN:
            if (!GetAsyncKeyState(VK_CONTROL))
            {
                D3DXVECTOR3 Scale(0.9f, 0.9f, 0.9f);
                //ScaleDirect(Scale);
				this->GetTransformTool(KG3DTYPE_SCALING_TOOL).TransformDirect(Scale);
                //ScaleDirectSize(0.9f);
				KSF::SetSelectionSFXScaleSize(this->GetSelectionTool(), 0.9f);
            }
            else
            {
                //ScaleDirectSize(0.9f);
				KSF::SetSelectionSFXScaleSize(this->GetSelectionTool(), 0.9f);
            }

			break;	
		}
	}
}

void KG3DSceneEditorBase::MsgOnKeyUp(WPARAM wParam, LPARAM lParam)
{
}

HRESULT KG3DSceneEditorBase::FrameMove()
{
		//��δ���û�м�鵽�κ�ʹ��
	if(m_SceneWndManager->GetCurOutputWnd())
	{
		if(m_dwEditState==SCENESTATE_SELECT && m_bEnableMoveCam)
		{
			float Step = 15.0F;
			D3DXVECTOR3 PosMove(0,0,0);
			D3DXVECTOR3 Pos,LookAt;
			m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetPosition(&Pos);
			m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetLookAtPosition(&LookAt);

			D3DXVECTOR3 Left = m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetCameraRight();
			D3DXVECTOR3 Front = m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetCameraFront();
			D3DXVECTOR3 Up = m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetCameraUp();

			Front.y = 0;
			D3DXVec3Normalize(&Front,&Front);

			if(g_KeyState[VK_UP]) 
			{
				PosMove += Front * Step;
			}
			if(g_KeyState[VK_DOWN]) PosMove -= Front * Step;
			if(g_KeyState[VK_LEFT]) PosMove -= Left*Step;
			if(g_KeyState[VK_RIGHT]) PosMove += Left*Step;

			KG3DCamera& Cam = m_SceneWndManager->GetCurOutputWnd()->GetCamera();
			Cam.SetPositionLookAtUp(&(Pos + PosMove), &(LookAt + PosMove), NULL);
		}
	}

	return KG3DScene::FrameMove();
}

HRESULT KG3DSceneEditorBase::DeleteSelectedEntity()
{
	TypeEntityAccessProxy& proxy = this->GetSelectionTool().GetSelectionAccessProxy();
	for(proxy.Begin(); !proxy.IsEnd(); proxy.StepForward())
	{
		KG3DSceneEntity Entity = proxy.GetCur();
		Entity.OnDelete();//����ͨ������Ƿ�ɹ��������Ƿ�Remove����������m_listSelectedEntity�л������˲���
		
		{
			m_listRenderEntity.remove(Entity);
			//selTool.SetSelectable(Entity, FALSE);//<Selectable Refactor>
			this->InsertEntitySelectable(Entity, FALSE);
		}
	}

	//m_listSelectedEntity.clear();
	this->GetSelectionTool().ClearSelection();
	return S_OK;
}

HRESULT KG3DSceneEditorBase::GetFirstSelectedModel(IEKG3DModel*& piModel)
{
    KG3DModel *pModel = NULL;
	/*if(m_listSelectedEntity.size()==0)
		return E_FAIL;*/
	if (0 == this->GetSelectionTool().GetSelectionCount())
	{
		return E_FAIL;
	}

	TypeEntityAccessProxy& proxy = this->GetSelectionTool().GetSelectionAccessProxy();
	for(proxy.Begin(); !proxy.IsEnd(); proxy.StepForward())
	{
		KG3DSceneEntity Entity = proxy.GetCur();
		if(Entity.GetType()==SCENEENTITY_OBJECT)
		{
			KG3DRepresentObject *pObject = (KG3DRepresentObject*) Entity.m_pPointer;
			if(SUCCEEDED(pObject->GetModel(pModel,0,FALSE)))
            {
                piModel = pModel;
				return S_OK;
            }
		}
		else if(Entity.GetType()==SCENEENTITY_MODEL)
		{
			KG3DModel *pModelEntity  = static_cast<KG3DModel*>(Entity.m_pPointer);
			ASSERT(pModelEntity);
            piModel = pModelEntity;
			return S_OK;
		}
		//i++;
	}
	return E_FAIL;
}

//INT nExecState, ��ʾִ��״̬��������ɿ�ĳ����ϼ�(KeyUp)���κ�һ������Ϊ = 0, ����ʱΪ 1
INT KG3DSceneEditorBase::ExecAction(INT nActionID, INT nExecState, WPARAM wParam, LPARAM lParam)
{
	//if (0 == nExecState)
	//	return 0;

	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	D3DXVECTOR3 PosMove(0,0,0);
	D3DXVECTOR3 Pos,LookAt;

	//(by dengzhihui 2006��8��17�� 15:24:43
	//���޷�����תΪ�з�������
	xPos = xPos > 0x7fff ? (xPos - 0xffff) : xPos;
	yPos = yPos > 0x7fff ? (yPos - 0xffff) : yPos;
	//)

	m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetPosition(&Pos);
	m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetLookAtPosition(&LookAt);
	D3DXVECTOR3 Left = m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetCameraRight();
	D3DXVECTOR3 Front = m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetCameraFront();
	D3DXVECTOR3 Up = m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetCameraUp();

	D3DXVECTOR3 N = Pos - LookAt;
	INT nRet = 1;

	switch(nActionID)		
	{
	case EXEACTION_ROTATE_CAMERA: //��ת�������ͷ
		{
			D3DXVECTOR3 N = Pos - LookAt;
			float R = D3DXVec3Length(&N);
			float XX = (xPos - g_nMouseMoveX) * 0.0016F*R;
			float YY = (yPos - g_nMouseMoveY) * 0.0016F*R;

			N +=-XX*Left+YY*Up;
			D3DXVec3Normalize(&N,&N);
			N = N * R;
			Pos = LookAt + N;
			m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetPosition(Pos);
		}
		break;
	case EXEACTION_ZOOM_VIEW: //���������ӽ�
		{
//			float R = D3DXVec3Length(&N);
			float XX = (xPos - g_nMouseMoveX) * 0.0F;
			extern float CameraSpeed;
			float YY = (yPos - g_nMouseMoveY) * CameraSpeed ;

			D3DXVECTOR3 F = Front;
			D3DXVec3Normalize(&F,&F);

			Pos += YY*F;
			LookAt += YY*F;

			D3DXVECTOR3 V = LookAt - Pos;
			D3DXMATRIX mat;
			D3DXMatrixRotationY(&mat,XX);
			D3DXVec3TransformCoord(&V,&V,&mat);
			LookAt = Pos + V;

			//m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetPosition(Pos);
			//m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetLookAtPosition(LookAt);
			KG3DCamera& Cam = m_SceneWndManager->GetCurOutputWnd()->GetCamera();
			Cam.SetPositionLookAtUp(&Pos, &LookAt, NULL);
		}
		break;
	case EXEACTION_PAN_VIEW: // Pan �ƶ��������ͼ
		{
			D3DXVECTOR3 N = Pos - LookAt;
			float R = D3DXVec3Length(&N);
			float XX = (xPos - g_nMouseMoveX) * CameraSpeed * R * 0.000009F;
			float YY = (yPos - g_nMouseMoveY) * CameraSpeed * R * 0.000009F;

			D3DXVECTOR3 F = Up;
			D3DXVec3Normalize(&F,&F);

			Pos += -XX*Left+YY*F;
			LookAt += -XX*Left+YY*F;
			/*m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetPosition(Pos);
			m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetLookAtPosition(LookAt);*/

			KG3DCamera& Cam = m_SceneWndManager->GetCurOutputWnd()->GetCamera();
			Cam.SetPositionLookAtUp(&Pos, &LookAt, NULL);
		}
		break;
	case EXEACTION_LEFE_KEY_DOWN://��ѡ������
	case EXEACTION_SELECTION_ADD://�ڵ�ǰѡ��Χ�У�������ѡ������
	case EXEACTION_SELECTION_SUTRACT://�ڵ�ǰѡ��Χ�У��ټ���ѡ������
		{
			if(nExecState)//<Selecting Refactor>	//2009-1-14 ,ѡ��Ĳ����ᵽSelectBase����,����Copy�Ĳ���
			{
				if (m_dwEditState == SCENESTATE_COPY)
				{
					if(m_SceneWndManager->GetCurOutputWnd())
					{
						D3DXVECTOR3 RayOrig,RayDir;
						D3DXVECTOR2 RayOrig2D;
						m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);

						float x = RayOrig.x - (RayOrig.y * RayDir.x) / RayDir.y;
						float y = 0;
						float z = RayOrig.z - (RayOrig.y * RayDir.z) / RayDir.y;

						D3DXVECTOR3 vCenter(0,0,0);
						//GetSelectedEntityCenter(&vCenter);
						KSF::GetSelectionCenter(*this, vCenter);
						D3DXVECTOR3 vOffset = D3DXVECTOR3(x, y, z) - vCenter;

						TypeEntityAccessProxy& proxy = this->GetSelectionTool().GetSelectionAccessProxy();
						for(proxy.Begin(); !proxy.IsEnd(); proxy.StepForward())
						{
							//KG3DSceneEntity &EntitySrc = (*i);
							KG3DSceneEntity &EntitySrc = proxy.GetCur();
							KG3DSceneEntity Entity;
							D3DXVECTOR3 vPos;
							EntitySrc.GetTranslation(&vPos);
							x = vPos.x + vOffset.x;
							y = vPos.y + vOffset.y;
							z = vPos.z + vOffset.z;
							if(SUCCEEDED(EntitySrc.Clone(&Entity, x, y, z)))
							{
								m_listRenderEntity.push_back(Entity);
								
								//this->GetSelectionTool().SetSelectable(Entity, TRUE);//<Selectable Refactor>
								this->InsertEntitySelectable(Entity, TRUE);
							}
						}
					}
				}	
			}
		}
		break;
	case EXEACTION_ZOOM_TO_OBJECT: //���ž�ͷ�����ʴ�С
		{
			////m_bFocusing = !m_bFocusing;
			//if(/*(m_bFocusing)&&*/(m_SceneWndManager->GetCurOutputWnd()))
			//{
			//	D3DXVECTOR3 Pos;
			//	HRESULT hr = KSF::GetSelectionCenter(*this, Pos);
			//	if(SUCCEEDED(hr))
			//	{
			//		D3DXVECTOR3 Eye,Lookat;

			//		m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetPosition(&Eye);
			//		m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetLookAtPosition(&Lookat);

			//		AABBOX Box;
			//		GetSelectedEntityBBox(Box);
			//		float R =  D3DXVec3Length(&(Box.A - Box.B));

			//		D3DXVECTOR3 N = Eye - Lookat;
			//		D3DXVec3Normalize(&N,&N);
			//		Lookat = Pos;
			//		Eye = Pos + N * R;
			//		//m_bFocusing = FALSE;

			//		/*m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetLookAtPosition(Lookat);
			//		m_SceneWndManager->GetCurOutputWnd()->GetCamera().SetPosition(Eye);*/

			//		KG3DCamera& Cam = m_SceneWndManager->GetCurOutputWnd()->GetCamera();
			//		Cam.SetPositionLookAtUp(&Eye, &Lookat, NULL);
			//	}
			//}
			ZoomToSelection();
		}
		break;
	case EXEACTION_PLAY_CAM_ANI: //�������������
		{
            IEKG3DSceneCameraAnimation *piCameraAnimation = NULL;
            m_pCameraMovement->GetCurCamAnimation(&piCameraAnimation);
            HRESULT hr = piCameraAnimation->StartPlay(TRUE);
			if ( SUCCEEDED( hr ) )
			{
				m_dwEditState = SCENESTATE_CAMERAANIMATION;	
			}			
			break;
		}
		break;
	case EXEACTION_MOUSE_WHEEL:
		{
			this->MsgOnMouseWheel(wParam, lParam);
		}	
		break;
		// 1234
	case EXEACTION_SELECT:
		SetEditState(SCENESTATE_SELECT);
		break;
	case EXEACTION_MOVE:
		SetEditState(SCENESTATE_MOVE);
		//OnToggleStateMove();//<OnToggleStateMove Refactor>//SceneSelectBase�������������ڲ��ı�״̬
		break;
	case EXEACTION_ROTATE:
		SetEditState(SCENESTATE_ROTATE);
		//OnToggleStateRotation();//<OnToggleStateRotation Refactor>//SceneSelectBase�����SetEditState�����ڲ��ı�״̬
		break;
	case EXEACTION_SCALE:
		SetEditState(SCENESTATE_SCALE);
		//OnToggleStateScaling();//<OnToggleStateScaling Refactor>//SceneSelectBase�����SetEditState�����ڲ��ı�״̬
		break;
	case EXEACTION_MOUSE_MOVE:
		this->SyncPickRay();
		break;
	}

	BoardCastAction(nActionID, nExecState, wParam, lParam);

	return nRet;
}

//����ѡ����Ĭ��Ϊ�����������»��ߵ���ʱ�����õĽӿ�
INT KG3DSceneEditorBase::ExecActionSelect(INT nActionID, INT nExecState, INT nXOff, INT nYOff)
{	
	return 1;
	//INT nRet = 0;

	//if(nExecState)	//<Selecting Refactor>	//2009-1-14 ,ѡ��Ĳ����ᵽSelectBase����,����Copy�Ĳ���
	//{
 //       BOOL bSrtFlag = TRUE;
 //       if(m_dwEditState==SCENESTATE_MOVE)
 //       {
 //           if(m_SceneWndManager->GetCurOutputWnd() && !m_bMoving)				
 //           {
 //               D3DXVECTOR3 RayOrig,RayDir;
 //               D3DXVECTOR2 RayOrig2D;
 //               m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);

 //               //if(SUCCEEDED(m_EditMoveGrid.CheckState(RayOrig,RayDir)))
 //               if (!m_bSelecting)
 //               {
 //                   MoveBegin(RayOrig,RayDir);     
 //                   bSrtFlag = m_bMoving;
 //                   MoveFrameMove(/*&m_SceneWndManager->GetCurOutputWnd()->GetCamera(),RayOrig,RayDir,0,0*/);//<Move Refactor>������Ч
 //               }
 //           }
 //       }
 //       else if(m_dwEditState==SCENESTATE_ROTATE)
 //       {
 //           if(m_SceneWndManager->GetCurOutputWnd() && !m_bRotating)
 //           {
 //               D3DXVECTOR3 RayOrig,RayDir;
 //               D3DXVECTOR2 RayOrig2D;
 //               m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);

 //               //if(SUCCEEDED(m_EditRotationGrid.CheckState(RayOrig,RayDir)))
 //               if (!m_bSelecting)
 //               {
 //                   RotateBegin(RayOrig,RayDir);
 //                   bSrtFlag = m_bRotating;
 //               }
 //           }			
 //       }
 //       else if(m_dwEditState==SCENESTATE_SCALE)
 //       {
 //           if(m_SceneWndManager->GetCurOutputWnd() && !m_bScaling)
 //           {	
 //               D3DXVECTOR3 RayOrig,RayDir;
 //               D3DXVECTOR2 RayOrig2D;
 //               m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);
 //               //if(SUCCEEDED(m_EditScaleGrid.CheckState(RayOrig,RayDir)))

 //               if (!m_bSelecting)
 //               {
 //                   ScaleBegin(RayOrig,RayDir);
 //                   bSrtFlag = m_bScaling;
 //               }
 //           }
 //       }

	//	if (m_dwEditState==SCENESTATE_SELECT || !bSrtFlag)
	//	{
	//		if(m_SceneWndManager->GetCurOutputWnd() && !IsSelecting())				
	//		{
	//			D3DXVECTOR3 RayOrig,RayDir;
	//			D3DXVECTOR2 RayOrig2D;
	//			m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);
	//			if (EXEACTION_SELECTION_ADD == nActionID)//GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	//				SelectBegin(RayOrig, RayDir, RayOrig2D, EM_SELECTION_INCREASE_MODE_ADD);
	//			else if (EXEACTION_SELECTION_SUTRACT == nActionID)//GetAsyncKeyState(VK_LMENU)  & 0x8000)
	//				SelectBegin(RayOrig, RayDir, RayOrig2D, EM_SELECTION_INCREASE_MODE_SUBTRACT);
	//			else
	//				SelectBegin(RayOrig, RayDir, RayOrig2D, EM_SELECTION_INCREASE_MODE_DEFAULT);
	//		}
	//	}
	//	else if (m_dwEditState == SCENESTATE_COPY)
	//	{
	//		if(m_SceneWndManager->GetCurOutputWnd())
	//		{
	//			D3DXVECTOR3 RayOrig,RayDir;
	//			D3DXVECTOR2 RayOrig2D;
	//			m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);

	//			float x = RayOrig.x - (RayOrig.y * RayDir.x) / RayDir.y;
	//			float y = 0;
	//			float z = RayOrig.z - (RayOrig.y * RayDir.z) / RayDir.y;

	//			D3DXVECTOR3 vCenter(0,0,0);
	//			//GetSelectedEntityCenter(&vCenter);
	//			::GetSelectionCenter(*this, vCenter);
	//			D3DXVECTOR3 vOffset = D3DXVECTOR3(x, y, z) - vCenter;
	//			
	//			/*KG3DSceneEntityList::iterator i = m_listSelectedEntity.begin();
	//			KG3DSceneEntityList::iterator i_End = m_listSelectedEntity.end();
	//			while (i != i_End)*/
	//			TypeEntityAccessProxy& proxy = this->GetSelectionTool().GetSelectionAccessProxy();
	//			for(proxy.Begin(); !proxy.IsEnd(); proxy.StepForward())
	//			{
	//				//KG3DSceneEntity &EntitySrc = (*i);
	//				KG3DSceneEntity &EntitySrc = proxy.GetCur();
	//				KG3DSceneEntity Entity;
	//				D3DXVECTOR3 vPos;
	//				EntitySrc.GetTranslation(&vPos);
	//				x = vPos.x + vOffset.x;
	//				y = vPos.y + vOffset.y;
	//				z = vPos.z + vOffset.z;
	//				if(SUCCEEDED(EntitySrc.Clone(&Entity, x, y, z)))
	//				{
	//					m_listRenderEntity.push_back(Entity);
	//					AddSelectableEntity(Entity.GetType(), Entity.m_pPointer);
	//				}
	//				//i++;
	//			}
	//		}
	//	}	
	//}
	//else
	//{
 //       BOOL bStrFlag = TRUE;

 //       if(m_dwEditState==SCENESTATE_MOVE)
 //       {
 //           if (m_bMoving)
 //               MoveEnd();
 //           bStrFlag = m_bMoving;
 //       }
 //       else if(m_dwEditState==SCENESTATE_ROTATE)
 //       {
 //           if(m_bRotating)
 //           {
 //               RotateEnd();
 //           }
 //           bStrFlag = m_bRotating;
 //       }
 //       else if(m_dwEditState==SCENESTATE_SCALE)
 //       {
 //           if(m_bScaling)
 //           {
 //               ScaleEnd();
 //           }
 //           bStrFlag = m_bScaling;
 //       }

	//	if(m_dwEditState==SCENESTATE_SELECT || !bStrFlag)
	//	{
	//		if(IsSelecting())
	//		{
	//			if(m_SceneWndManager->GetCurOutputWnd())
	//			{
	//				D3DXMATRIX Proj,View;
	//				Proj = m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetProjectionMatrix();
	//				View = m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetViewMatrix();

	//				SelectEnd(m_SceneWndManager->GetCurOutputWnd()->m_Viewport,Proj,View);

 //                   if (!bStrFlag && /*m_listSelectedEntity.size()*/ 0 != this->GetSelectionTool().GetSelectionCount())
 //                   {
 //                       switch (m_dwEditState)
 //                       {
 //                       case SCENESTATE_MOVE :
 //                           OnToggleStateMove();
 //                           break;
 //                       case SCENESTATE_ROTATE :
 //                           OnToggleStateRotation();
 //                           break;
 //                       case SCENESTATE_SCALE :
 //                           OnToggleStateScaling();
 //                           break;
 //                       default :
 //                           ASSERT(false);
 //                           break;
 //                       }
 //                   }
	//			}

	//		}
	//	}
	//	
	//}
	//return nRet;
}


//void KG3DSceneEditorBase::OnMouseMove(WPARAM wParam, LPARAM lParam)
//{
//	int xPos = LOWORD(lParam); 
//	int yPos = HIWORD(lParam);		
//
//	xPos = xPos > 0x7fff ? (xPos - 0xffff) : xPos;
//	yPos = yPos > 0x7fff ? (yPos - 0xffff) : yPos;
//
//	if(m_bSelecting)
//	{
//		if(m_SceneWndManager->GetCurOutputWnd() && IsSelecting())
//		{
//			D3DXVECTOR3 RayOrig,RayDir;
//			D3DXVECTOR2 RayOrig2D;
//			m_SceneWndManager->GetCurOutputWnd()->GetPickRay(&RayOrig,&RayDir,&RayOrig2D);
//			SelectFrameMove(RayOrig, RayDir, RayOrig2D);
//		}
//	}
//}


//void KG3DSceneEditorBase::ZoomView(short nDelta, POINT pt)
//{
//	//MsgOnMouseWheel(nDelta, 0);
//}

//һЩ�뷽�򼰷�ҳ�ȶ�����صĵ���
void KG3DSceneEditorBase::OnCommand(enumCommandType eCommandType)
{
	D3DXVECTOR3 Pos(0,0,0);
	D3DXQUATERNION Rot(0,1,0,0);

	if(m_dwEditState==SCENESTATE_MOVE)
	{
		switch(eCommandType)
		{
		case ctUp:
			Pos = D3DXVECTOR3(0,0,5);
			//MoveDirect(Pos);
			this->GetTransformTool(KG3DTYPE_TRANSLATION_TOOL).TransformDirect(Pos);
			break;	
		case ctDown:
			Pos = D3DXVECTOR3(0,0,-5);
			//MoveDirect(Pos);
			this->GetTransformTool(KG3DTYPE_TRANSLATION_TOOL).TransformDirect(Pos);
			break;	
		case ctLeft:
			Pos = D3DXVECTOR3(-5,0,0);
			//MoveDirect(Pos);
			this->GetTransformTool(KG3DTYPE_TRANSLATION_TOOL).TransformDirect(Pos);
			break;	
		case ctRight:
			Pos = D3DXVECTOR3(5,0,0);
			//MoveDirect(Pos);
			this->GetTransformTool(KG3DTYPE_TRANSLATION_TOOL).TransformDirect(Pos);
			break;
		case ctPageUp://Y�᷽�����ƶ�
			Pos = D3DXVECTOR3(0,5,0);
			this->GetTransformTool(KG3DTYPE_TRANSLATION_TOOL).TransformDirect(Pos);
			break;	
		case ctPageDown:
			Pos = D3DXVECTOR3(0,-5,0);
			this->GetTransformTool(KG3DTYPE_TRANSLATION_TOOL).TransformDirect(Pos);
			break;
		}
	}
	else if(m_dwEditState==SCENESTATE_ROTATE)
	{
		FLOAT f5Degree = D3DX_PI / 36;
		switch(eCommandType)
		{
		case ctUp://��X��ת
			/*D3DXQuaternionRotationYawPitchRoll(&Rot, 0,           D3DX_PI/36,  0);
			RotateDirect(Rot, &D3DXVECTOR3(0, D3DX_PI / 36, 0));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(f5Degree, 0, 0));	
			break;	
		case ctDown://��X��ת
			/*D3DXQuaternionRotationYawPitchRoll(&Rot, 0,           -D3DX_PI/36, 0);
			RotateDirect(Rot, &D3DXVECTOR3(0,-D3DX_PI / 36, 0));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(-f5Degree, 0, 0));
			break;	
		case ctLeft://��Y��ת
			/*D3DXQuaternionRotationYawPitchRoll(&Rot, D3DX_PI/36,  0,           0);
			RotateDirect(Rot, &D3DXVECTOR3(D3DX_PI / 36, 0, 0));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(0, f5Degree, 0));
			break;	
		case ctRight://��Y��ת
			/*D3DXQuaternionRotationYawPitchRoll(&Rot, -D3DX_PI/36, 0,           0);
			RotateDirect(Rot, &D3DXVECTOR3(-D3DX_PI / 36, 0, 0));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(0, -f5Degree, 0));
			break;	
		case ctPageUp://��Z��ת
			/*D3DXQuaternionRotationYawPitchRoll(&Rot,0,0,D3DX_PI/36);
			RotateDirect(Rot, &D3DXVECTOR3(0, 0, D3DX_PI / 36));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(0, 0, f5Degree));
			break;	
		case ctPageDown://��Z��ת
			/*D3DXQuaternionRotationYawPitchRoll(&Rot,0,0,-D3DX_PI/36);
			RotateDirect(Rot, &D3DXVECTOR3(0, 0, -D3DX_PI / 36));*/
			this->GetTransformTool(KG3DTYPE_ROTATION_TOOL).TransformDirect(D3DXVECTOR3(0, 0, -f5Degree));
			break;
		}
	}
	else if(m_dwEditState==SCENESTATE_SCALE)
	{
		switch(eCommandType)
		{
		case ctUp:
			if (!GetAsyncKeyState(VK_CONTROL))
			{
				D3DXVECTOR3 Scale(1.1f, 1.1f, 1.1f);
				//ScaleDirect(Scale);
				this->GetTransformTool(KG3DTYPE_SCALING_TOOL).TransformDirect(Scale);
				//ScaleDirectSize(1.1f);
				KSF::SetSelectionSFXScaleSize(this->GetSelectionTool(), 1.1f);
			}
			else
			{
				//ScaleDirectSize(1.1f);
				KSF::SetSelectionSFXScaleSize(this->GetSelectionTool(), 1.1f);
			}

			break;

		case ctDown:
			if (!GetAsyncKeyState(VK_CONTROL))
			{
				D3DXVECTOR3 Scale(0.9f, 0.9f, 0.9f);
				//ScaleDirect(Scale);
				this->GetTransformTool(KG3DTYPE_SCALING_TOOL).TransformDirect(Scale);
				//ScaleDirectSize(0.9f);
				KSF::SetSelectionSFXScaleSize(this->GetSelectionTool(), 0.9f);
			}
			else
			{
				//ScaleDirectSize(0.9f);
				KSF::SetSelectionSFXScaleSize(this->GetSelectionTool(), 0.9f);
			}
			break;	
		case ctInc: //�Ŵ���С��ǰ�ı�ˢһ��
			break;
		case ctDec:
			break;
		}
	}
	else
	{
		switch(eCommandType)
		{
		case ctAddKeyFrame:
            {
                IEKG3DSceneCameraAnimation *piCameraAnimation = NULL;
                m_pCameraMovement->GetCurCamAnimation(&piCameraAnimation);
                piCameraAnimation->AddKeyFrame();
            }
            break;
		}
	}
}

//(by dengzhihui Refactor 2006��11��24��
HRESULT KG3DSceneEditorBase::SetEditState(DWORD dwState)
{
	//С��������������������Ҫ�㲥״̬�ĸı䣬������Ҫ������Ϣ����Ṥ���������������ﲻӦ�ü��Ϻ�EditBase�޹صĶ����������Ӧ����RegisterListener�Ļ���������
	//ע��TDDCheckҲ�����������������Ҫ�ı���״̬����ò�Ҫ����Ҫ��TDDCheck��ǰ�����״̬��ԭ��״̬�Աȣ���֤TDDCheckû�иı�������״̬

	//2009-1-16 �����⼸��ĵ�SceneSelectBase����Ϣ��Ӧ������
	/*if ((m_dwEditState == SCENESTATE_MOVE) && (dwState == SCENESTATE_SELECT))
	{
		MoveEnd();
	}*/

	ULONG oldState = m_dwEditState;

	m_dwEditState = dwState;

	ULONG newState = dwState;

	if(newState != oldState)
	{
		GetMessageSender(KM_STATE_CHANGE).PostMessage(KG3DMessage(KM_STATE_CHANGE, newState, oldState));
	}

#if defined(DEBUG) | defined(_DEBUG)
	if (m_SceneWndManager->GetCurOutputWnd() && dwState != SCENESTATE_BENCHMARK)	//��Ҫ�������Ȼ�����޷����У������û��Dummy��ʽ
	{
		LPVOID pTestData = reinterpret_cast<LPVOID>(static_cast<KG3DSceneEditorBase*>(this));
		TDD::RunTest(KG3DSceneEditorBase::TDDCheckAfterInitialized, pTestData, _T("KG3DSceneEditorBase::TDDCheck"), TFILE, __LINE__);
	}
#endif


	return S_OK;
}

HRESULT KG3DSceneEditorBase::GetRootLayer(IEKG3DSceneLayer** pLayer)
{
	_ASSERTE(pLayer);
	if (pLayer)
	{
		*pLayer = m_pRootLayer;
		return S_OK;
	}
	return E_FAIL;
}

//INT KG3DSceneEditorBase::GetSceneLayerSize()
//{
//	return (INT)m_vecSceneLayer.size();
//}

//HRESULT KG3DSceneEditorBase::GetSceneLayer(INT nIndex , IEKG3DSceneLayer** pLayer)
//{
//	_ASSERTE(*pLayer && nIndex >= 0 && nIndex < (INT)m_vecSceneLayer.size());
//
//	if((*pLayer &&nIndex >= 0 && nIndex < (INT)m_vecSceneLayer.size()))
//	{
//		*pLayer = m_vecSceneLayer[nIndex];
//		return S_OK;
//	}
//	return E_FAIL;
//}

//HRESULT KG3DSceneEditorBase::ClearSelectedEntity()
//{
//	//m_listSelectedEntity.clear();
//
//	this->GetSelectionTool().ClearSelection();
//	return S_OK;
//}

HRESULT KG3DSceneEditorBase::GetSceneSelector(IEKG3DSceneSelectBase** ppSelector)
{
	_ASSERTE(ppSelector);
	if (ppSelector)
	{
		*ppSelector = static_cast<IEKG3DSceneSelectBase*>(this);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT KG3DSceneEditorBase::GetEditState(DWORD* pdwState)
{
	_ASSERTE(pdwState);
	if (pdwState)
	{
		*pdwState = m_dwEditState;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT KG3DSceneEditorBase::GetMouseXY(INT* pX, INT* pY)
{
	_ASSERTE(pX && pY);

	if (pX && pY)
	{
		*pX = g_nMouseMoveX;
		*pY = g_nMouseMoveY;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT KG3DSceneEditorBase::SetMouseXY(INT pX, INT pY)
{
	g_nMouseMoveX = pX;	//�����Ψһ�ĸ�ȫ�ֵ�g_nMouseMoveX,Y�ĵط�
	g_nMouseMoveY = pY;
	return S_OK;
}

HRESULT KG3DSceneEditorBase::SetEditing(DWORD dwEditing)
{
	m_bEditing = dwEditing;
	return S_OK;
}

DWORD KG3DSceneEditorBase::GetEditing()
{
	return m_bEditing;
}

KG3DSceneSelectionTool& KG3DSceneEditorBase::GetSelectionTool()
{
	if (g_Switch.bNewSelection)
	{
		//������������Դ�������Ϊ��Init��ʱ��ͱ��봴���ˣ�ǰ�漸������Ҫ��ʱ��Ŵ���������
		return KG3DScene::GetSelectionTool();		
	}
	else
	{
		KG3DSceneSelectionTool& selTool = *this;
		return selTool;
	}
}

KG3DTransformTool& KG3DSceneEditorBase::GetTransformTool( DWORD dwType )
{
	_ASSERTE(dwType >= KG3DTYPE_TRANSFORM_TOOL_BEGIN && dwType <= KG3DTYPE_TRANSFORM_TOOL_END);

	if (g_Switch.bNewSelection)
	{
		return KG3DScene::GetTransformTool(dwType);
	}
	return KG3DSceneSelectBase::GetTransformToolInterface(dwType);
}
HRESULT KG3DSceneEditorBase::GetSelectionToolInterface( IEKG3DSceneSelectionTool** ppTool )
{
	return KG3DScene::GetSelectionToolInterface(ppTool);
}

void __stdcall KG3DSceneEditorBase::OnReceiveMessage( const KG3DMessage& Message , IKG3DMessageSenderBase* pSender )
{
	return;
}

KG3DSelectionBBoxGetter& KG3DSceneEditorBase::GetSelectionBBoxGetter()
{
	_ASSERTE(NULL != m_pSelectionBoxGetter);

	return *m_pSelectionBoxGetter;
}

HRESULT	KG3DSceneEditorBase::SetSelectionBBoxGetter(KG3DSelectionBBoxGetter& newGetter, KG3DSelectionBBoxGetter** ppOld)
{
	if(m_pSelectionBoxGetter != &newGetter && NULL != ppOld)
	{
		*ppOld = m_pSelectionBoxGetter;
		m_pSelectionBoxGetter = &newGetter;
		return S_OK;
	}

	return E_FAIL;
}

VOID KG3DSceneEditorBase::ZoomToSelection()
{
	if(NULL == m_SceneWndManager->GetCurOutputWnd())
		return;

	D3DXVECTOR3 Pos;
	HRESULT hr = KSF::GetSelectionCenter(*this, Pos);
	if(FAILED(hr))
		return;
	D3DXVECTOR3 Eye,Lookat;

	m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetPosition(&Eye);
	m_SceneWndManager->GetCurOutputWnd()->GetCamera().GetLookAtPosition(&Lookat);

	AABBOX Box;
	hr = GetSelectionBBoxGetter().GetBBox(this->GetSelectionTool(), Box);
	if(FAILED(hr))
		return;

	float R =  D3DXVec3Length(&(Box.A - Box.B));

	D3DXVECTOR3 N = Eye - Lookat;
	D3DXVec3Normalize(&N,&N);
	Lookat = Pos;
	Eye = Pos + N * R;

	KG3DCamera& Cam = m_SceneWndManager->GetCurOutputWnd()->GetCamera();
	Cam.SetPositionLookAtUp(&Eye, &Lookat, NULL);
	
}

HRESULT KG3DSceneEditorBase::PasteSelection()
{
	return E_NOTIMPL;
}

void KG3DSceneEditorBase::SyncPickRay()
{
	return;
}

HRESULT KG3DSceneEditorBase::InsertEntitySelectable(KG3DSceneEntity& entity, BOOL bInsertAndSetSelectable)//<Selectable Refactor>
{
	if(! g_Switch.bSelectableRefactorFinished)
	{
		entity.SetSelectable(bInsertAndSetSelectable);
		HRESULT hr  = this->GetSelectionTool().SetSelectable(entity, bInsertAndSetSelectable);		
		return hr;
	}
	else
	{
		if(bInsertAndSetSelectable)
		{
			m_CtEntities.remove(entity);
			m_CtEntities.push_back(entity);

			entity.SetSelectable(TRUE);
		}
		else
		{
			m_CtEntities.remove(entity);

			entity.SetSelectable(FALSE);
		}
		return S_OK;
	}		
}

HRESULT KG3DSceneEditorBase::IEInsertEntitySelectable( SCENEENTITYTYPE dwType, LPVOID pPoint, BOOL bInsertAndSetSelectable )//<Selectable Refactor>
{
	_ASSERTE(NULL != pPoint);
	KG3DSceneEntity entity(dwType, pPoint);
	return InsertEntitySelectable(entity, bInsertAndSetSelectable);
}

HRESULT KG3DSceneEditorBase::AddRenderEntity( KG3DSceneEntity Entity, BOOL bIsSelectable /*= TRUE*/, BOOL bIsSelected /*= FALSE*/ )
{
	if (Entity.GetType() == SCENEENTITY_MODEL)
	{
		KG3DModel* pModel = reinterpret_cast<KG3DModel*>(Entity.m_pPointer);
		pModel->SetPhysicsScene(m_lpPhysicsScene);
	}
	m_listRenderEntity.push_back(Entity);
	if (bIsSelectable)
	{
		KG3DSceneSelectionTool& selTool = GetSelectionTool();
		//selTool.SetSelectable(Entity, bIsSelectable);//<Selectable Refactor>
		this->InsertEntitySelectable(Entity, bIsSelectable);
		if (bIsSelected)
		{
			selTool.SetSelected(Entity, bIsSelected);
		}
	}
	return S_OK;
}

HRESULT KG3DSceneEditorBase::AddRenderEntity( SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer )
{
	if(NULL == pvEntityPointer)//������������û�ж�����Entity������ÿ��������Ҫ�ж�һ�£��ܶ�ط��ᵱ
		return E_FAIL;
	return AddRenderEntity(KG3DSceneEntity(eEntityType, pvEntityPointer));
}
HRESULT KG3DSceneEditorBase::RemoveRenderEntity( IKG3DResourceBase *pPoint, unsigned uClass, BOOL bSelectable /*= TRUE*/ )
{
	if(NULL == pPoint)//������������û�ж�����Entity������ÿ��������Ҫ�ж�һ�£��ܶ�ط��ᵱ
		return E_FAIL;
	SCENEENTITYTYPE dwEntityType = uClass ? SCENEENTITY_OBJECT : SCENEENTITY_MODEL;
	KG3DSceneEntity Entity(dwEntityType, pPoint);
	if (Entity.GetType() == SCENEENTITY_MODEL)
	{
		KG3DModel* pModel = reinterpret_cast<KG3DModel*>(Entity.m_pPointer);
		pModel->SetPhysicsScene(NULL);
	}
	m_listRenderEntity.remove(Entity);

	KG3DSceneSelectionTool& selTool = GetSelectionTool();
	//selTool.SetSelectable(Entity, FALSE);
	this->IEInsertEntitySelectable(dwEntityType, pPoint, FALSE);
	selTool.SetSelected(Entity, FALSE);
	return S_OK;
}

HRESULT KG3DSceneEditorBase::RemoveRenderEntity( KG3DSceneEntity Entity, BOOL bEitherRemoveInSelectedList /*= TRUE*/ )
{
	{
		if (Entity.GetType() == SCENEENTITY_MODEL)
		{
			KG3DModel* pModel = reinterpret_cast<KG3DModel*>(Entity.m_pPointer);
			pModel->SetPhysicsScene(NULL);
		}
		m_listRenderEntity.remove(Entity);

		KG3DSceneSelectionTool& selTool = GetSelectionTool();
		//selTool.SetSelectable(Entity, FALSE);//<Selectable Refactor>
		this->InsertEntitySelectable(Entity, FALSE);
		if(bEitherRemoveInSelectedList)
			selTool.SetSelected(Entity, FALSE);
	}
	return S_OK;
}

HRESULT KG3DSceneEditorBase::RemoveRenderEntity( SCENEENTITYTYPE eEntityType, PVOID pvEntityPointer )
{
	if(NULL == pvEntityPointer)//������������û�ж�����Entity������ÿ��������Ҫ�ж�һ�£��ܶ�ط��ᵱ
		return E_FAIL;
	return RemoveRenderEntity(KG3DSceneEntity(eEntityType, pvEntityPointer), TRUE);//�ⲿ���ã����һ������һ��ΪTrue
}

//)