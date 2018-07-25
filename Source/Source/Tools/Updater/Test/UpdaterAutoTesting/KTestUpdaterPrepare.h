//////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) Kingsoft
//
//  FileName    :   KTestUpdaterPrepare.h    
//  Version     :   1.0
//  Creater     :   dinglong
//  Date        :   2009-2-18 10:00
//  Comment     :   ����ǰ������׼���Ĳ��ԣ���Ҫ��֤����ǰ��ȡ�����ݵ���ȷ��
//					����LoadConfigֱ�Ӷ�ȡ����Ŀ¼�µ������ļ������Դ˹��ܽӿ�ֻ��������Ŀ¼�²��ԣ��޷��ڲ��Կռ���в���
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KTestUpdaterPrepare_H_
#define _KTestUpdaterPrepare_H_

#include "cppunit\extensions\HelperMacros.h"
#include "KTestCommon.h"

class KTestUpdaterPrepare : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE(KTestUpdaterPrepare);
	CPPUNIT_TEST(TestPrepareSuccess);
	CPPUNIT_TEST(TestLostVersionFile);
 	CPPUNIT_TEST(TestLostConfigFile);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	//��ȡ��������ȷ����֤
	void TestPrepareSuccess();
	//�汾��Ϣ�ļ�ȱʧ�Ĵ���
	void TestLostVersionFile();
	//�����ļ�ȱʧ�Ĵ���
	void TestLostConfigFile();

private:
	//׼�������õİ汾��Ϣ�ļ�
	int PrepareVersionFile(const TCHAR cszVersion[]);
	//׼�������õ������ļ�
	int PrepareGUconfigFile(const TCHAR cszRemotePath[]);
	
private:
	TCHAR m_szGUconfig[MAX_PATH];					//�����ļ�
	TCHAR m_szVersionFile[MAX_PATH];				//�汾��Ϣ�ļ�
	TCHAR m_szUpdateTempDir[MAX_PATH];				//��ʱ����Ŀ¼
	TCHAR m_szAppDir[MAX_PATH];						//�����ó�������Ŀ¼
	BOOL m_bUpdateDirCreated;						//��ʱ�����ļ��д������
};

#endif ///_KTestUpdaterPrepare_H_