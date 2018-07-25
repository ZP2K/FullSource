//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestFullRun.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-3-4 10:00
//  Comment     :   ��֤�������и��³�����и��º������
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestFullRun_H__
#define _KTestFullRun_H__

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"

class KTestFullRun : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestFullRun);
	CPPUNIT_TEST(TestUpdateOneVersionAndCombinePack);
	CPPUNIT_TEST(TestUpdateOneVersionAndNewPack);
 	CPPUNIT_TEST(TestUpdateSeveralVersionAndCombinePack);
	CPPUNIT_TEST(TestUpdateSeveralVersionAndNewPack);
	
	//CPPUNIT_TEST(TestUpdateSeveralVersionAndNewPack_OneDay);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown(); 

	//����һ������������֤��ͬʱ��֤�ϰ�����������
	void TestUpdateOneVersionAndCombinePack();
	//����һ������������֤��ͬʱ��֤�ְ��͸��Ǹ���
	void TestUpdateOneVersionAndNewPack();
	//���¶������������֤��ͬʱ��֤�ϰ�����������
	void TestUpdateSeveralVersionAndCombinePack();
	//���¶������������֤��ͬʱ��֤�ְ��͸��Ǹ���
	void TestUpdateSeveralVersionAndNewPack();

	////��ʱ��������һ�졣���¶������������֤��ͬʱ��֤�ְ��͸��Ǹ���
	//int  SetUp_OneDay();
	//int  TearDown_OneDay();
	//void TestUpdateSeveralVersionAndNewPack_OneDay();

private:
	//׼�����º���ļ���������֤���Ǹ���
	int PrepareExistFiles();

	//׼���������µĲ��Ի���
	int PrepareTestSpace(const TCHAR cszTestVersion[]);

	//��֤���ݸ��º���ļ�
	int ValidateFileData();

	//��֤���º��ע���
	int ValidateRegData(const TCHAR cszVersion[]);

	int StartProc();
	int WaitAndCloseProc(const TCHAR szLatestVersion[], UINT uMaxTimeOut);

	//�����ﵽĬ�Ϸְ�������Ŀ��ϲ���
	int CreatePakForNewPack();

private:
	STARTUPINFO			m_ServerStartupinfo;		//http������������Ϣ
	STARTUPINFO			m_GameUpdaterStartupinfo;	//GameUpdater����������Ϣ
	PROCESS_INFORMATION m_ServerProcessInfo;		//http������������Ϣ�ṹ��
	PROCESS_INFORMATION m_GameUpdaterProcessInfo;	//GameUpdater������Ϣ�ṹ��
	BOOL				m_bRegSaved;				//��¼ԭʼ��ע���ı�־
};

#endif ///KTestFullRun