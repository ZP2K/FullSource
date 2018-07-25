#include "stdafx.h"
#include ".\sceneswordtest.h"
#include "KSceneSwordTestWizard.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////

KSceneSwordTest::KSceneSwordTest(void)
{
	FPS = 0;
	LastTime = timeGetTime()*1.0f;

	m_bTesting = FALSE;
	m_dwTestTimeStart = 0;
	m_nFrameCount = 0;

	m_nFacesCount = 0;

	m_nTestType = 0;
	m_nNowTestingMeshIndex = 0;
}

KSceneSwordTest::~KSceneSwordTest(void)
{
	m_RigidTable.CleanUp();
}

HRESULT KSceneSwordTest::FrameMove()
{
	K3DScene::FrameMove();

	m_Sim.FrameMove();

	if (m_bTesting)
	{
		m_nFrameCount++;
		int Time = timeGetTime() - m_dwTestTimeStart;
		if(Time>=15000)
		{
			m_bTesting = FALSE;

			if(m_nTestType==3)
			{
				int Index = m_nNowTestingMeshIndex;
				m_nNowTestingMeshIndex++;
				if(m_nNowTestingMeshIndex<=(int)m_vecTestModelID.size())
				{
					_TestInfo* pInfo = &m_vecTestModelID[Index];
					pInfo->FPS = m_nFrameCount / 15.0f;

					m_bTesting = TRUE;
					m_nFrameCount = 0;
					m_dwTestTimeStart = timeGetTime();


					ENTITYLIST::iterator i = m_RenderEntityList.begin();
					while(i!=m_RenderEntityList.end())
					{
						KSceneEntity Entity = *i;
						if(Entity.Type==SCENEENTITY_OBJECT)
						{
							LPOBJECT pObject = (LPOBJECT) Entity.Pointer;
							pObject->m_vecMeshKeeper[0].m_dMeshID = pInfo->MeshID;
						}
						i++;
					}
				}
				else
				{
					TCHAR Name[256];
					wsprintf(Name,"%s\\���ѹ�����Ա���.txt",g_Def_AppDirectory);
					FILE* pFile = fopen(Name,"aw");

					fprintf(pFile,"======================================================\n");
					for(DWORD i=0;i<m_vecTestModelID.size();i++)
					{
						_TestInfo* pInfo = &m_vecTestModelID[i];

						TCHAR Name[256];
						wsprintf(Name,"%s",pInfo->Name.c_str());
						fprintf(pFile,"%-15s.Mesh %-6d  %-6.1f %-6.1f\n",Name,
							pInfo->NumFaces,
							pInfo->FPS,
							pInfo->FPS*pInfo->NumFaces/100);
					}
					fprintf(pFile,"======================================================\n\n");

					fclose(pFile);

					m_nNowTestingMeshIndex = 0;
				}
			}
		}
		else
		{
			float t = Time * 6.283f / 15000 ;

			//m_lpGirlNo1->m_Position.x = sinf(t)*3000 + m_Center.x;
			//m_lpGirlNo1->m_Position.z = cosf(t)*3000 + m_Center.z;

			GraphicsStruct::KCamera* pCamera = &g_GraphiceEngine.m_cCamera;

			pCamera->Position.x = sinf(t)*2500;
			pCamera->Position.z = cosf(t)*2500;
			pCamera->Position.y = 700;

			pCamera->ViewPoint = D3DXVECTOR3(0,0,0);
		}
	}

	return S_OK;
}

HRESULT KSceneSwordTest::Render(void)
{
	GraphicsStruct::KCamera* pCamera = &g_GraphiceEngine.m_cCamera;

	/*float s = timeGetTime()*0.0001f;
	float Lens = 400;
	pCamera->Position = D3DXVECTOR3(Lens*sinf(s) , Lens*0.577f+20 , Lens*cosf(s));
	pCamera->ViewPoint = D3DXVECTOR3(0,100,0);
	pCamera->SetCamera();*/

	g_cGraphicsTool.DrawGroundGrid(10);

	K3DScene::Render();

	TCHAR str[200];
	wsprintf(str,"F %d",m_nFacesCount);
	g_cGraphicsTool.DrawText(100,100,1,1,0xFFFFFFFF,16,"����",str,0);

	float NowTime = (float)(timeGetTime());
	float F = 1000.0f/ (NowTime - LastTime);
	FPS = (FPS + F)/2;
	DWORD DFPS = (DWORD)FPS;


	g_cGraphicsTool.DrawRect( &D3DXVECTOR3(0,0,0),  &D3DXVECTOR3(1000,0,0),  &D3DXVECTOR3(0,1000,-1000), 0xaaaaaaaa, 0 );

	//LastTime = timeGetTime()*1.0f;

	m_Sim.Render();

	/*int k =0;
	ENTITYLIST::iterator i = m_RenderEntityList.begin();
	while(i!=m_RenderEntityList.end())
	{
		KSceneEntity Entity = *i;
		switch(Entity.Type)
		{
			case SCENEENTITY_OBJECT:
				{
					LPOBJECT pObject = (LPOBJECT) Entity.Pointer;
					pObject->m_Position.z -= 10.0f ;
					if(pObject->m_Position.z <-5000)
						pObject->m_Position.z = 5000;
					break;
				}
		}
	i++;
	k++;
	}*/


	if (m_nFrameCount>0)
	{
		if(m_bTesting)
		{
			wsprintf(str,"Frame Count %d ",m_nFrameCount);
			g_cGraphicsTool.DrawText(100,500,1,1,0xFFFF00FF,20,"����",str,0);
		}
		else
		{
			sprintf(str,"Frames: %d FPS: %.1f ",m_nFrameCount,m_nFrameCount/15);
			g_cGraphicsTool.DrawText(100,500,1,1,0xFFFF00FF,20,"����",str,0);
		}
	}
	return E_NOTIMPL;
}

HRESULT KSceneSwordTest::InitPhysicsTest(int choice)
{
	Physics::LPRIGIDBODY pRigid = NULL;
	Physics::LPRIGIDBODY pRigid0 = NULL;
	D3DXPLANE  plane;
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//˵��
	//1. �����ʼ��������,һ��Ҫ�ȵ���SetSP����,�����Initialise����
	//	pRigid->SetSP() (����ȷ������ļ�������)
	//	pRigid->Initialise() (����ȷ���������������)
	//
	//2.������г�ʼ��������������õ�����
	//	SetMass(float mass);  ��������
	//	SetInertia(float inertia); ����ת������,Ŀǰת��������һ������������ʾ,���Կ���ʹ��һ���������洢��άת��������Ϣ
	//	m_LineVel;		���ٶ�
	//	m_AngVel;		���ٶ�
	//	m_Force;        ����		//ע������,���������о�����Ҫֱ�����ú����������,����ȥ�������ٶȻ���ٶ�
	//	m_Torque;       ������
	//	m_matOrient;    ����ֲ�����ϵ�� ��Ӧ������   //ע��,ͬ��������Ҫ����ֱ�������������
	//	m_fStaticFriction;  ��Ħ��ϵ��
	//	m_fDynamicFriction; ��Ħ��ϵ��
	//
	//  ���������ṩ��һЩ��������,��
	//  m_bImmovable;  �����ƶ�������		//����Ҫ��KPhysicsSimulator�������޸�,��������,���������޸Ĳ�����,����ע�͵���
	//										//������һ���Ե�˼·��,����������嶼�ǲ����ƶ�����,������ײ���
	//										//�������֮һ�ǲ����ƶ�����,������ײ���,����ײ����ʱ,�������Ƹ���ײƽ��Ĵ��벿������ɳ����ļ���
	//
	//  m_bNonePenetration					//���������������Ϊ����ֹ����֮�以��Ƕ�������,�ɲο�KPhysicsSimulator���е��ⲿ�ִ���(��ע�͵�)
	//										//��ΪЧ������,����Ҳע�͵���,ԭ���Ǹ��ݸ�����ײ������õ�Ƕ�����,��������Ԥ�ڵ��ٶ�
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//3.���������õĲ���
	//	m_bAddingGravity;					//�Ƿ�������������
	//	m_bUsingFriction;					//�Ƿ���Ħ��������(Ŀǰֻ�����˸����е�Ħ��ϵ��,��ʵ�Ͽ��Կ���Ϊƽ���������Ӧ��Ħ��ϵ��,����KPhysicsSimulator���������޸ļ���)
	//	m_fEpsilon;							//��ײ�ָ�ϵ��,0~1֮��,1Ϊ��ȫ������ײ,0Ϊ��ȫ�ǵ�����ײ
	//
	//  DisableCollision()						//��������������������֮�����ײ
	///////////////////////////////////////////////////////////////////////////////////////////////////
	switch(choice)
	{
	case 1:		//������������������ײ�����Ч��,��������,Ħ����,��ײ�ָ�ϵ��0.65f
		m_Sim.m_bAddingGravity = TRUE;
		m_Sim.m_bUsingFriction = TRUE;
		m_Sim.m_fEpsilon = 0.65f;

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(0,300,0), D3DXVECTOR3(0,0.7f,0.7f), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(0,300,0), 1.0f, 833.3f );
		m_Sim.AddRigidBody(pRigid);

		D3DXPlaneFromPointNormal( &plane, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0) );
		m_Sim.listPlane.push_back( plane );

		break;

	case 2:		//��������ƽ�У�������,��Ħ����,����,��ȫ������ײ

		m_Sim.m_bAddingGravity = FALSE;
		m_Sim.m_bUsingFriction = FALSE;
		m_Sim.m_fEpsilon = 1.0f;

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(1,100,0), D3DXVECTOR3(0,1,0), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(1,100,0), 1.0f, 833.3f );
		m_Sim.AddRigidBody(pRigid);

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(1000,100,0), D3DXVECTOR3(0,1,0), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(1000,100,0), 1.0f, 833.3f );
		pRigid->m_LineVel = D3DXVECTOR3(-100,0,0);
		m_Sim.AddRigidBody(pRigid);

		break;

	case 3:		//�������洹ֱ,������,��Ħ����,��ȫ������ײ
		m_Sim.m_bAddingGravity = FALSE;
		m_Sim.m_bUsingFriction = FALSE;
		m_Sim.m_fEpsilon = 1.0f;

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(1,100,0), D3DXVECTOR3(0,1,0), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(1,100,0), 1.0f, 833.3f );
		m_Sim.AddRigidBody(pRigid);

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(100,100,0), D3DXVECTOR3(0,0,1), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(100,100,0), 1.0f, 833.3f );
		pRigid->m_LineVel = D3DXVECTOR3( -10, 0, 0 );
		m_Sim.AddRigidBody(pRigid);

		break;

	case 4:		//����ƽ�У����Ĳ��ȸ�,������,��Ħ����,��ȫ������ײ
		m_Sim.m_bAddingGravity = FALSE;
		m_Sim.m_bUsingFriction = FALSE;
		m_Sim.m_fEpsilon = 1.0f;

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(1,100,0), D3DXVECTOR3(0,1,0), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(1,100,0), 1.0f, 833.3f );
		m_Sim.AddRigidBody(pRigid);

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(100,180,0), D3DXVECTOR3(0,1,0), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(100,180,0), 1.0f, 833.3f );
		pRigid->m_LineVel = D3DXVECTOR3( -10, 0, 0 );
		m_Sim.AddRigidBody(pRigid);

		break;

	case 5:		//�������治��ֱ,������,��Ħ����,��ȫ������ײ
		m_Sim.m_bAddingGravity = FALSE;
		m_Sim.m_bUsingFriction = FALSE;
		m_Sim.m_fEpsilon = 1.0f;

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(1,100,0), D3DXVECTOR3(0,1,0), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(1,100,0), 1.0f, 833.3f );
		m_Sim.AddRigidBody(pRigid);

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(150,110,0), D3DXVECTOR3(0,0.7f,0.7f), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(150,110,0), 1.0f, 833.3f );
		pRigid->m_LineVel = D3DXVECTOR3( -10, 0, 0 );
		m_Sim.AddRigidBody(pRigid);

		break;

	case 6:		//�������棬 �������岿�����������岿�֣�������,��Ħ����,��ȫ������ײ
		m_Sim.m_bAddingGravity = FALSE;
		m_Sim.m_bUsingFriction = FALSE;
		m_Sim.m_fEpsilon = 1.0f;

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(1,100,0), D3DXVECTOR3(0,1,0), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(1,100,0), 1.0f, 833.3f );
		m_Sim.AddRigidBody(pRigid);

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(100,100,50), D3DXVECTOR3(0,0.7f,0.7f), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(100,100,50), 1.0f, 833.3f );
		pRigid->m_LineVel = D3DXVECTOR3( -10, 0, 0 );
		m_Sim.AddRigidBody(pRigid);

		break;

	case 7:		//�ӱ����ɿ���,������ܻ���ֻ���Ƕ�������,����Ҫ����
		m_Sim.m_bAddingGravity = TRUE;
		m_Sim.m_bUsingFriction = TRUE;
		m_Sim.m_fEpsilon = 0.65f;

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(0,100,0), D3DXVECTOR3(0,1,0), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(0,100,0), 100.0f, 83333.3f );
		m_Sim.AddRigidBody(pRigid);

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(0,230,0), D3DXVECTOR3(0,1,0), 0, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(0,230,0), 20.0f, 7200 );
		m_Sim.AddRigidBody(pRigid);

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(300,210,0), D3DXVECTOR3(0,1,0), 0.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(300,210,0), 10.0f, 10*360 );
		pRigid->m_LineVel = D3DXVECTOR3(-200,0,0);
		m_Sim.AddRigidBody(pRigid);

		D3DXPlaneFromPointNormal( &plane, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0) );
		m_Sim.listPlane.push_back( plane );

		break;

	case 8:		//������ƽ����ײ��Ч��,������,Ħ����,��ײϵ��0.65f
		m_Sim.m_bAddingGravity = TRUE;
		m_Sim.m_bUsingFriction = TRUE;
		m_Sim.m_fEpsilon = 0.65f;

		m_RigidTable.Get1NewRigidBody(&pRigid);
		pRigid->SetSP( D3DXVECTOR3(110,300,-110), D3DXVECTOR3(0,0.7f,0.7f), 70.0f, 30.0f );
		pRigid->Initialise( D3DXVECTOR3(110,300,-110), 1.0f, 833.3f );
		m_Sim.AddRigidBody(pRigid);

		D3DXPlaneFromPointNormal( &plane, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0) );
		m_Sim.listPlane.push_back( plane );

		D3DXPlaneFromPointNormal( &plane, &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,0.7f,0.7f) );
		m_Sim.listPlane.push_back( plane );

		break;
	}
	return S_OK;
}

HRESULT KSceneSwordTest::Initialize(PVOID pSceneTable)
{
	KSceneEditBase::Initialize(pSceneTable);

	//TCHAR Name[256];
	//TCHAR Str[256];

	//if(IDOK==Dlg.DoModal())
	//{
	//	m_nTestType = Dlg.m_mTestType;
	//}
	//else
	//{
	//	m_nTestType = 0;
	//}


	//if(m_nTestType==0)
	//{
	//	LPCSTR SecName = "Test1";

	//	KIniFile Ini;
	//	if(FAILED(g_cFileFinder.FindFile(Name,"����ѹ������.ini")))
	//	{
	//		return E_FAIL;
	//	}
	//	Ini.Load(Name);

	//	if(!Ini.IsSectionExist(SecName))
	//		return E_FAIL;

	//	int K =0;
	//	vector<KComponentObject*>vecScrObject;

	//	for(int i=1;i<=100;i++)
	//	{
	//		wsprintf(Str,"Mesh%d ",i);
	//		Ini.GetString(SecName,Str,"",Name,256);
	//		::TrimString(Name);
	//		if(_strcmpi(Name,"")!=0)
	//		{
	//			LPMODEL pMesh = NULL;
	//			DWORD NewID = 0;
	//			TCHAR MeshName[256];

	//			if(SUCCEEDED(g_cFileFinder.FindFile(MeshName,Name)))
	//			{
	//				if (FAILED(g_cMeshTable.LoadFromFile(&NewID,MeshName)))
	//					continue ;
	//				if (FAILED(g_cMeshTable.GetXMesh(&pMesh,NewID)))
	//					continue ;
	//				m_nFacesCount += pMesh->m_pMeshSys->GetNumFaces();

	//				wsprintf(Str,"Mtl%d ",i);
	//				Ini.GetString(SecName,Str,"",Name,256);
	//				::TrimString(Name);
	//				if(SUCCEEDED(g_cFileFinder.FindFile(MeshName,Name)))
	//				{
	//					pMesh->LoadMaterial(MeshName);
	//				}

	//				KComponentObject* pObject = NULL;
	//				g_cObjectTable.Get1NewObject(&pObject,OBJECT_DEFAULT);
	//				pObject->AddMesh(NewID);
	//				D3DXVECTOR3 Pos = GetObjectPosition(i);
	//				//D3DXVECTOR3 Pos(0,0,i*200);
	//				pObject->Edit_Move(&Pos);

	//				AddRenderObject(pObject);
	//				vecScrObject.push_back(pObject);
	//			}
	//		}
	//		else
	//		{
	//			K = i;
	//			break;
	//		}
	//	}

	//	for(int j=0;j<9;j++)
	//	{
	//		for(int i=0;i<(int)vecScrObject.size();i++)
	//		{
	//			KComponentObject* pNewObject = NULL;
	//			g_cObjectTable.CopyObject(&pNewObject,vecScrObject[i]);

	//			//D3DXVECTOR3 Pos = vecScrObject[i]->m_Position + D3DXVECTOR3(j*100,0,0);
	//			D3DXVECTOR3 Pos = GetObjectPosition(K);
	//			pNewObject->Edit_Move(&Pos);

	//			AddRenderObject(pNewObject);
	//			K++;
	//		}
	//	}

	//	m_nFacesCount *= 10;
	//}
	//else if(m_nTestType==1)
	//{
	//	LPCSTR SecName = "Test2";

	//	KIniFile Ini;
	//	if(FAILED(g_cFileFinder.FindFile(Name,"����ѹ������.ini")))
	//	{
	//		return E_FAIL;
	//	}
	//	Ini.Load(Name);

	//	if(!Ini.IsSectionExist(SecName))
	//		return E_FAIL;

	//	int K =0;
	//	vector<KComponentObject*>vecScrObject;

	//	for(int i=1;i<=100;i++)
	//	{
	//		wsprintf(Str,"Mesh%d ",i);
	//		Ini.GetString("Test2",Str,"",Name,256);
	//		::TrimString(Name);
	//		if(_strcmpi(Name,"")!=0)
	//		{
	//			LPMODEL pMesh = NULL;
	//			DWORD NewID = 0;
	//			TCHAR MeshName[256];

	//			if(SUCCEEDED(g_cFileFinder.FindFile(MeshName,Name)))
	//			{
	//				if (FAILED(g_cMeshTable.LoadFromFile(&NewID,MeshName)))
	//					continue ;
	//				if (FAILED(g_cMeshTable.GetXMesh(&pMesh,NewID)))
	//					continue ;

	//				wsprintf(Str,"Mtl%d ",i);
	//				Ini.GetString("Test2",Str,"",Name,256);
	//				::TrimString(Name);
	//				if(SUCCEEDED(g_cFileFinder.FindFile(MeshName,Name)))
	//				{
	//					pMesh->LoadMaterial(MeshName);
	//				}

	//				KComponentObject* pObject = NULL;
	//				g_cObjectTable.Get1NewObject(&pObject,OBJECT_DEFAULT);
	//				pObject->AddMesh(NewID);
	//				D3DXVECTOR3 Pos = GetObjectPosition(i+K);
	//				pObject->Edit_Move(&Pos);

	//				AddRenderObject(pObject);
	//			}
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}
	//}
	//else if(m_nTestType==2)//����ѹ������
	//{
	//	for(DWORD i=0;i<Dlg.m_vecFileName.size();i++)
	//	{
	//		wsprintf(Name,"%s",Dlg.m_vecFileName[i].c_str());

	//		LPMODEL pMesh = NULL;
	//		DWORD NewID = 0;
	//		TCHAR MeshName[256];

	//		if(SUCCEEDED(g_cFileFinder.FindFile(MeshName,Name)))
	//		{
	//			if (FAILED(g_cMeshTable.LoadFromFile(&NewID,MeshName)))
	//				continue ;
	//			if (FAILED(g_cMeshTable.GetXMesh(&pMesh,NewID)))
	//				continue ;
	//			m_nFacesCount += pMesh->m_pMeshSys->GetNumFaces();

	//			//////////////////////////////////////////////////////////////////////////
	//			TCHAR path_buffer[256];
	//			TCHAR drive[256];
	//			TCHAR dir[256];
	//			TCHAR fname[256];
	//			TCHAR ext[256];

	//			wsprintf(path_buffer,"%s",Name);
	//			_splitpath( path_buffer, drive, dir, fname, ext );


	//			wsprintf(Name,"%s%s%s.Mtl",drive,dir,fname);
	//			if(SUCCEEDED(g_cFileFinder.FindFile(MeshName,Name)))
	//			{
	//				pMesh->LoadMaterial(MeshName);
	//			}

	//			KComponentObject* pObject = NULL;
	//			g_cObjectTable.Get1NewObject(&pObject,OBJECT_DEFAULT);
	//
	//			pObject->AddMesh(NewID);
	//			D3DXVECTOR3 Pos = GetObjectPosition(i);
	//			//D3DXVECTOR3 Pos(0,0,i*200);
	//			pObject->Edit_Move(&Pos);

	//			AddRenderObject(pObject);
	//		}
	//	}
	//}
	//else if(m_nTestType==3)
	//{
	//	for(DWORD i=0;i<Dlg.m_vecFileName.size();i++)
	//	{
	//		wsprintf(Name,"%s",Dlg.m_vecFileName[i].c_str());

	//		LPMODEL pMesh = NULL;
	//		DWORD NewID = 0;
	//		TCHAR MeshName[256];

	//		if(SUCCEEDED(g_cFileFinder.FindFile(MeshName,Name)))
	//		{
	//			if (FAILED(g_cMeshTable.LoadFromFile(&NewID,MeshName)))
	//				continue ;
	//			if (FAILED(g_cMeshTable.GetXMesh(&pMesh,NewID)))
	//				continue ;
	//			m_nFacesCount += pMesh->m_pMeshSys->GetNumFaces();

	//			//////////////////////////////////////////////////////////////////////////
	//			TCHAR path_buffer[256];
	//			TCHAR drive[256];
	//			TCHAR dir[256];
	//			TCHAR fname[256];
	//			TCHAR ext[256];

	//			wsprintf(path_buffer,"%s",Name);
	//			_splitpath( path_buffer, drive, dir, fname, ext );


	//			wsprintf(Name,"%s%s%s.Mtl",drive,dir,fname);
	//			if(SUCCEEDED(g_cFileFinder.FindFile(MeshName,Name)))
	//			{
	//				pMesh->LoadMaterial(MeshName);
	//			}

	//			_TestInfo Info;
	//			Info.Name = fname;
	//			Info.MeshID = NewID;
	//			Info.NumFaces = pMesh->m_pMeshSys->GetNumFaces();

	//			m_vecTestModelID.push_back(Info);
	//		}
	//	}

	//	for(int X = -5;X<5;X++)
	//	{
	//		for(int Y= -5;Y<5;Y++)
	//		{
	//			KComponentObject* pObject = NULL;
	//			g_cObjectTable.Get1NewObject(&pObject,OBJECT_DEFAULT);

	//			pObject->AddMesh(m_vecTestModelID[0].MeshID);
	//			D3DXVECTOR3 Pos(X*1000.0f,0,Y*1000.0f);
	//			pObject->Edit_Move(&Pos);

	//			AddRenderObject(pObject);

	//		}
	//	}
	//}

	//return S_OK;

	return S_OK;
}

D3DXVECTOR3 KSceneSwordTest::GetObjectPosition(int Index)
{
	float R_Base = 150;
	int n = 1;
	int S = 1;

	while(Index>S)
	{
		n += 2;
		S = n * n;
	}
	if(Index>1)
	{
		int L = (n-2)*(n-2);
		float K = D3DX_PI * 2 / ( S - L );
		float R = R_Base * n/2;

		int I = Index - L;
		float X = R*sinf( K*I );
		float Z = R*cosf( K*I );

		return D3DXVECTOR3(X,0,Z);
	}
	return D3DXVECTOR3(0,0,0);
}

void KSceneSwordTest::ONKEYDOWN(WPARAM wParam, LPARAM lParam)
{
	if(!m_bTesting)
	{
		if(wParam=='T')
		{
			m_bTesting = TRUE;
			m_nFrameCount = 0;
			m_dwTestTimeStart = timeGetTime();
		}
	}

	if('I' == wParam)
	{
		m_SunLight.Specular.r = 0;
		m_SunLight.Specular.g = 0;
		m_SunLight.Specular.b = 0;
	}

	switch(wParam)
	{
	case VK_F1:
		InitPhysicsTest(8);
		break;
	case VK_F2:
		InitPhysicsTest(1);
		break;
	case VK_F3:
		InitPhysicsTest(2);
		break;
	case VK_F4:
		InitPhysicsTest(3);
		break;
	case VK_F5:
		InitPhysicsTest(4);
		break;
	case VK_F6:
		InitPhysicsTest(5);
		break;
	case VK_F7:
		InitPhysicsTest(6);
		break;
	case VK_F8:
		InitPhysicsTest(7);
		break;
	}
	KSceneEditBase::ONKEYDOWN(wParam,lParam);
}

