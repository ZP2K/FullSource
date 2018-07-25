#include "stdafx.h"

#ifdef _UNIT_TEST
#include "KItemNull_Test.h"
#include "../ui/elem/wndstation.h"

CPPUNIT_TEST_SUITE_REGISTRATION(KItemNull_Test);

void KItemNull_Test::setUp()
{
	int nRetCode = false;
	m_item = NULL;


	m_item = UI::KItemNull::Create(m_itemdata);
	CPPUNIT_ASSERT(m_item);	

	nRetCode = UI::KWndStation::Init();
	CPPUNIT_ASSERT(nRetCode);
	
}

void KItemNull_Test::tearDown()
{
	if (m_item)
	{
		KG_COM_RELEASE(m_item);
	}

	UI::KItemEventMgr::Exit();
}

//////////////////////////////////////////////////////////////////////////
//@������� :1
//@����Ŀ�� :��֤int KItemNull::Init(KItemNullData & ItemNullData) ����ȷ��
//@ǰ�ó��� :
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :����ն���
//@OP2      :δ��ʼ���ڴ��KItemNullData
//@OP3      :KItemNullData��һ���ַ�Ϊ0
//@OP4      :�ڴ�Գ�ʼ��Ϊ1
//@OP5      :��ֵ������KItemNullData����
//@OP6      :Init����
//@���ó��� :
//@CP1      :Initʧ��
//@CP2      :Initʧ��
//@CP3      :Initʧ��
//@CP4      :Initʧ��
//@CP5      :Init�ɹ�
//@CP6      :Init�ɹ�
//////////////////////////////////////////////////////////////////////////
void KItemNull_Test::InitItemTest_1()
{
	int nRetCode = false;
	UI::KItemNullData *pTempItemData = NULL;

	nRetCode = m_item->Init(*pTempItemData);
	CPPUNIT_ASSERT(!nRetCode);
}

void KItemNull_Test::InitItemTest_2()
{
	int nRetCode = false;
	UI::KItemNullData *pTempItemData = NULL;

	pTempItemData = (UI::KItemNullData *)malloc(sizeof(UI::KItemNullData));
	nRetCode  = m_item->Init(*pTempItemData);
	if (pTempItemData)
	{
		free(pTempItemData);
		pTempItemData = NULL;
	}
	CPPUNIT_ASSERT(!nRetCode);
}

void KItemNull_Test::InitItemTest_3()
{
	int nRetCode = false;
	UI::KItemNullData *pTempItemData = NULL;
	char *pstr = NULL;


	pTempItemData = (UI::KItemNullData *)malloc(sizeof(UI::KItemNullData));
	pstr = (char *)pTempItemData;
	pstr[0] = '\0';
	nRetCode = m_item->Init(*pTempItemData);
	if (pTempItemData)
	{
		free(pTempItemData);
		pTempItemData = NULL;
	}
	CPPUNIT_ASSERT(!nRetCode);

}

void KItemNull_Test::InitItemTest_4()
{
	int nRetCode = false;
	UI::KItemNullData *pTempItemData = NULL;
	char *pstr = NULL;

	pTempItemData = (UI::KItemNullData *)malloc(sizeof(UI::KItemNullData));
	memset(pTempItemData, 1, sizeof(UI::KItemNullData));
	nRetCode = m_item->Init(*pTempItemData);
	if (pTempItemData)
	{
		free(pTempItemData);
		pTempItemData = NULL;
	}
	CPPUNIT_ASSERT(!nRetCode);
}

void KItemNull_Test::InitItemTest_5()
{
	int nRetCode = false;
	UI::KItemNullData *pTempItemData = NULL;
	char *pstr = NULL;

	strcpy(m_itemdata.szName, "testitem1");
	m_itemdata.nAutoSize = 0;
	m_itemdata.nPosType = 1;
	m_itemdata.fHeight = 0.1f;
	m_itemdata.fWidth = 0.2f;
	pTempItemData = &m_itemdata;
	nRetCode = m_item->Init(*pTempItemData);
	CPPUNIT_ASSERT(nRetCode);
}

void KItemNull_Test::InitItemTest_6()
{
	int nRetCode = false;
	UI::KItemNullData *pTempItemData = NULL;
	char *pstr = NULL;

	strcpy(m_itemdata.szName, "testitem2");
	m_itemdata.nAutoSize = 2;
	m_itemdata.nPosType = 2;
	m_itemdata.fHeight = 0.5f;
	m_itemdata.fWidth = 0.2f;
	pTempItemData = &m_itemdata;
	nRetCode = m_item->Init(*pTempItemData);
	CPPUNIT_ASSERT(nRetCode);
	nRetCode = m_item->Init(*pTempItemData);
	CPPUNIT_ASSERT(nRetCode);
}
//////////////////////////////////////////////////////////////////////////
//@������� :2
//@����Ŀ�� :��֤virtual void Release(); ����ȷ��
//@ǰ�ó��� :
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1      :��������Release
//@���ó��� :
//@CP1      :Release�ɹ�
//////////////////////////////////////////////////////////////////////////
void KItemNull_Test::DeleteItemTest()
{
	int nRetCode = false;

	m_item->Release();
	m_item->Release();
}
//////////////////////////////////////////////////////////////////////////
//@������� :3
//@����Ŀ�� :��֤virtual int IsItemCanChangeTo(DWORD dwType); ����ȷ��
//@ǰ�ó��� :
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1       :����ΪITEM_BEGINE
//@OP2       :����ΪITEM_NULL
//@OP3       :����ΪITEM_ANIAMTE
//@OP4       :����ΪITEM_COUNT
//@OP5       :����Ϊ100
//@OP6       :����Ϊ-1 
//@���ó��� :
//@CP1      :���سɹ�
//@CP2      :���سɹ�
//@CP3      :����ʧ��
//@CP4      :����ʧ��
//@CP5      :����ʧ��
//@CP6      :����ʧ��
//////////////////////////////////////////////////////////////////////////
void KItemNull_Test::ChgItemTypeTest_1()
{
	int nRetCode = false;
	DWORD dwItemType = UI::ITEM_BEGINE;

	nRetCode = m_item->IsItemCanChangeTo(dwItemType);
	CPPUNIT_ASSERT(nRetCode);
}

void KItemNull_Test::ChgItemTypeTest_2()
{
	int nRetCode = false;
	DWORD DWItemType = UI::ITEM_NULL;

	nRetCode = m_item->IsItemCanChangeTo(DWItemType);
	CPPUNIT_ASSERT(nRetCode);
}

void KItemNull_Test::ChgItemTypeTest_3()
{
	int nRetCode = false;
	DWORD DWItemType = UI::ITEM_ANIAMTE;

	nRetCode = m_item->IsItemCanChangeTo(DWItemType);
	CPPUNIT_ASSERT(nRetCode);
}

void KItemNull_Test::ChgItemTypeTest_4()
{
	int nRetCode = false;
	DWORD DWItemType = UI::ITEM_COUNT;

	nRetCode = m_item->IsItemCanChangeTo(DWItemType);
	CPPUNIT_ASSERT(nRetCode);
}

void KItemNull_Test::ChgItemTypeTest_5()
{
	int nRetCode = false;
	DWORD DWItemType = 100;

	nRetCode = m_item->IsItemCanChangeTo(DWItemType);
	CPPUNIT_ASSERT(nRetCode);
}

void KItemNull_Test::ChgItemTypeTest_6()
{
	int nRetCode = false;
	DWORD DWItemType = -1;

	nRetCode = m_item->IsItemCanChangeTo(DWItemType);
	CPPUNIT_ASSERT(nRetCode);
}
//////////////////////////////////////////////////////////////////////////
//@������� :4
//@����Ŀ�� :��֤int KItemNull::PtInItem(float fAbsX, float fAbsY)����ȷ��
//@ǰ�ó��� :
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1       :X<m_fAbsX,Y<m_fAbsY
//@OP2       :X<m_fAbsX,Y=m_fAbsY
//@OP3       :X=m_fAbsX,Y<m_fAbsY
//@OP4       :X=m_fAbsX,Y=m_fAbsY
//@OP5       :X=m_fAbsX + m_fWidth,y=m_fAbsY +m_fHeight
//@OP6       :X>m_fAbsX + m_fWidth,y=m_fAbsY +m_fHeight
//@OP7       :X=m_fAbsX + m_fWidth,y>m_fAbsY +m_fHeight
//@OP8       :X>m_fAbsX + m_fWidth,y>m_fAbsY +m_fHeight
//@OP9       :Item����Ϊ��ʾ
//@OP10      :Item����Ϊ����
//@���ó��� :
//@CP1      :����Item��
//@CP2      :����Item��
//@CP3      :����Item��
//@CP4      :��Item��
//@CP5      :��Item��
//@CP6      :����Item��
//@CP7      :����Item��
//@CP8      :����Item��
//@CP9      :����x=m_fAbsX+1,y=m_fAbsY+1,������Item��
//@CP10     :����x=m_fAbsX+1,y=m_fAbsY+1,���겻��Item��
//////////////////////////////////////////////////////////////////////////
void KItemNull_Test::PtINItemTest_1()
{
	int nResult = false;
	int nRetCode = false;
	float fx = 0;
	float fy = 0;

	nRetCode = m_item->SetAbsPos(1, 1);
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->SetSize(10, 10);
	KG_PROCESS_ERROR(nRetCode);
	fx = 0;
	fy = 0;
	nRetCode = m_item->PtInItem(fx,fy);
	KG_PROCESS_ERROR(!nRetCode);
	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
	
}

void KItemNull_Test::PtINItemTest_2()
{
	int nResult = false;
	int nRetCode = false;
	float fx = 0;
	float fy = 0;

	nRetCode = m_item->SetAbsPos(1, 1);
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->SetSize(10, 10);
	KG_PROCESS_ERROR(nRetCode);
	fx = 0;
	fy = 1;
	nRetCode = m_item->PtInItem(fx,fy);
	KG_PROCESS_ERROR(!nRetCode);
	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KItemNull_Test::PtINItemTest_3()
{
	int nResult = false;
	int nRetCode = false;
	float fx = 0;
	float fy = 0;

	nRetCode = m_item->SetAbsPos(1, 1);
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->SetSize(10, 10);
	KG_PROCESS_ERROR(nRetCode);
	fx = 1;
	fy = 0;
	nRetCode = m_item->PtInItem(fx,fy);
	KG_PROCESS_ERROR(!nRetCode);
	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KItemNull_Test::PtINItemTest_4()
{
	int nResult = false;
	int nRetCode = false;
	float fx = 0;
	float fy = 0;

	nRetCode = m_item->SetAbsPos(1, 1);
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->SetSize(10, 10);
	KG_PROCESS_ERROR(nRetCode);
	fx = 1;
	fy = 1;
	nRetCode = m_item->PtInItem(fx,fy);
	KG_PROCESS_ERROR(nRetCode);
	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KItemNull_Test::PtINItemTest_5()
{
	int nResult = false;
	int nRetCode = false;
	float fx = 0;
	float fy = 0;

	nRetCode = m_item->SetAbsPos(1, 1);
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->SetSize(10, 10);
	KG_PROCESS_ERROR(nRetCode);
	fx = 11;
	fy = 11;
	nRetCode = m_item->PtInItem(fx,fy);
	KG_PROCESS_ERROR(nRetCode);
	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KItemNull_Test::PtINItemTest_6()
{
	int nResult = false;
	int nRetCode = false;
	float fx = 0;
	float fy = 0;

	nRetCode = m_item->SetAbsPos(1, 1);
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->SetSize(10, 10);
	KG_PROCESS_ERROR(nRetCode);
	fx = 15;
	fy = 11;
	nRetCode = m_item->PtInItem(fx,fy);
	KG_PROCESS_ERROR(!nRetCode);
	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KItemNull_Test::PtINItemTest_7()
{
	int nResult = false;
	int nRetCode = false;
	float fx = 0;
	float fy = 0;

	nRetCode = m_item->SetAbsPos(1, 1);
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->SetSize(10, 10);
	KG_PROCESS_ERROR(nRetCode);
	fx = 11;
	fy = 12;
	nRetCode = m_item->PtInItem(fx,fy);
	KG_PROCESS_ERROR(!nRetCode);
	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KItemNull_Test::PtINItemTest_8()
{
	int nResult = false;
	int nRetCode = false;
	float fx = 0;
	float fy = 0;

	nRetCode = m_item->SetAbsPos(1, 1);
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->SetSize(10, 10);
	KG_PROCESS_ERROR(nRetCode);
	fx = 15;
	fy = 12;
	nRetCode = m_item->PtInItem(fx,fy);
	KG_PROCESS_ERROR(!nRetCode);
	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KItemNull_Test::PtINItemTest_9()
{
	int nResult = false;
	int nRetCode = false;
	float fx = 0;
	float fy = 0;

	nRetCode = m_item->SetAbsPos(1, 1);
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->SetSize(10, 10);
	KG_PROCESS_ERROR(nRetCode);
	fx = 7;
	fy = 7;
	nRetCode = m_item->Show();
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->PtInItem(fx,fy);
	KG_PROCESS_ERROR(nRetCode);
	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}

void KItemNull_Test::PtINItemTest_10()
{
	int nResult = false;
	int nRetCode = false;
	float fx = 0;
	float fy = 0;

	nRetCode = m_item->SetAbsPos(1, 1);
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->SetSize(10, 10);
	KG_PROCESS_ERROR(nRetCode);
	fx = 7;
	fy = 7;
	nRetCode = m_item->Hide();
	KG_PROCESS_ERROR(nRetCode);
	nRetCode = m_item->PtInItem(fx,fy);
	KG_PROCESS_ERROR(!nRetCode);
	nResult = true;
Exit0:
	CPPUNIT_ASSERT(nResult);
}
//////////////////////////////////////////////////////////////////////////
//@������� :5
//@����Ŀ�� :��֤int KItemNull::SetPosType(DWORD dwPosType) ����ȷ��
//@ǰ�ó��� :
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1       :����ΪPOSITION_BEGINE
//@OP2       :����ΪPOSITION_END
//@OP3       :����ΪPOSITION_TOP_CENTER
//@OP4       :����Ϊ-1
//@OP5       :����Ϊ100
//@���ó��� :
//@CP1      :���óɹ�
//@CP2      :����ʧ��
//@CP3      :���óɹ�
//@CP4      :����ʧ��
//@CP5      :����ʧ��
//////////////////////////////////////////////////////////////////////////
void KItemNull_Test::SetPosTypeTest_1()
{
	int nRetCode = false;

	nRetCode = m_item->SetPosType(UI::POSITION_BEGINE);
	CPPUNIT_ASSERT(nRetCode);
}

void KItemNull_Test::SetPosTypeTest_2()
{
	int nRetCode = false;

	nRetCode = m_item->SetPosType(UI::POSITION_END);
	CPPUNIT_ASSERT(!nRetCode);
}

void KItemNull_Test::SetPosTypeTest_3()
{
	int nRetCode = false;

	nRetCode = m_item->SetPosType(UI::POSITION_TOP_CENTER);
	CPPUNIT_ASSERT(nRetCode);
}

void KItemNull_Test::SetPosTypeTest_4()
{
	int nRetCode = false;

	nRetCode = m_item->SetPosType(-1);
	CPPUNIT_ASSERT(!nRetCode);
}

void KItemNull_Test::SetPosTypeTest_5()
{
	int nRetCode = false;

	nRetCode = m_item->SetPosType(100);
	CPPUNIT_ASSERT(!nRetCode);
}
//////////////////////////////////////////////////////////////////////////
//@������� :6
//@����Ŀ�� :��֤int KItemNull::SetName(LPCTSTR pcszItemName) ����ȷ��
//@ǰ�ó��� :
//@C1       :
//@����Դ   :
//@�������� :
//@�������� :
//@OP1       :����Ϊ��ָ��
//@OP2       :����Ϊ���ַ���
//@OP3       :����Ϊ�����ַ������ȳ�����ΧITEM_NAME_MAX_SIZE
//@OP4       :����Ϊ�����ַ���
//@���ó��� :
//@CP1      :����ʧ��
//@CP2      :����ʧ��
//@CP3      :����ʧ��
//@CP4      :���óɹ�
//////////////////////////////////////////////////////////////////////////
void KItemNull_Test::SetNameTest_1()
{
	int nRetCode = false;
	LPCTSTR pcszItemName = NULL;

	nRetCode = m_item->SetName(pcszItemName);
	CPPUNIT_ASSERT(!nRetCode);
}

void KItemNull_Test::SetNameTest_2()
{
	int nRetCode = false;
	LPCTSTR pcszItemName = "";

	//pcszItemName[0] = '\0';
	nRetCode = m_item->SetName(pcszItemName);
	CPPUNIT_ASSERT(!nRetCode);
}
void KItemNull_Test::SetNameTest_3()
{
	int nRetCode = false;
	LPCTSTR pcszItemName = "a123456789b123456789c123456789d12";

	nRetCode = m_item->SetName(pcszItemName);
	CPPUNIT_ASSERT(!nRetCode);
}
void KItemNull_Test::SetNameTest_4()
{
	int nRetCode = false;
	LPCTSTR pcszItemName = "test001";

	nRetCode = m_item->SetName(pcszItemName);
	CPPUNIT_ASSERT(nRetCode);
}
#endif  //#ifdef UNIT_TEST