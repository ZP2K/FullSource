// RandDataAutoTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "KProbability.h"
#include "KRandDataTest.h"
#include <map>

int KRandDataAutoTest::Init()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    
    nRetCode = m_RandTest.Init();
    KG_PROCESS_ERROR(nRetCode);
    m_bInitFlag = true;

    nRetCode = m_RandTestSmall.Init();
    KG_PROCESS_ERROR(nRetCode);
    m_bInitSmallFlag = true;

    nResult = true;
Exit0:
    if (!nResult)
    {
        if (m_bInitSmallFlag)
        {
            m_RandTestSmall.UnInit();
            m_bInitSmallFlag = false;
        }
        if (m_bInitFlag)
        {
            m_RandTest.UnInit();
            m_bInitFlag = false;
        }
    }
    return nResult;
}


void KRandDataAutoTest::UnInit()
{
    m_RandTest.UnInit();
    m_bInitFlag = false;

    m_RandTestSmall.UnInit();
    m_bInitSmallFlag = false;
}

BOOL KRandDataAutoTest::TestWrongZero()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    int nAccount    =   0;

    if (m_bInitFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);
    //������ɾ�������´�������֤���ݵĸɾ�
    
    while (nAccount < 65535)        //�ܾ�����Ĵ����������������ĳ��֡�
    {
        nAccount++;
        nRetCode = m_RandTest.RandomResult(1, 0);
        KG_PROCESS_ERROR(!nRetCode);
    }

    nResult = true;
Exit0:
    return nResult;
}

BOOL KRandDataAutoTest::TestWrongHundred()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    int nAccount    =   0;

    if (m_bInitFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);
    //������ɾ�������´�������֤���ݵĸɾ�

    while (nAccount < 65535)        //�ܾ�����Ĵ����������������ĳ��֡�
    {
        nAccount++;
        nRetCode = m_RandTest.RandomResult(1, MILLION_NUM);
        KG_PROCESS_ERROR(nRetCode);
    }

    nResult = true;
Exit0:
    return nResult;
}
BOOL KRandDataAutoTest::TestSmall()
{
    BOOL     nRetCode    =   false;
    int     nResult     =   false;
    int     nAccount    =   0;
    int     nGetItemNum =   10;
    FILE    *pResult   =   NULL;
    char    szFileName[MAX_PATH];

    map< int, int > mapGetItemIndex;
    map< int, int >::iterator iter;


    if (m_bInitFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);

    //ʹ��һ��ѭ�������и�������Ĳ��ԡ������ĵ����ʴ�1��99����1%��99%)����1024Ϊ��������¼���ݡ�

    for (int nIndex = 1; nIndex < 100; nIndex++)
    {

        mapGetItemIndex.clear();

        snprintf(szFileName, sizeof(szFileName), "DropSmallDistribute%d.tab", nIndex);
        szFileName[sizeof(szFileName) -1] = '\0';

        pResult = fopen(szFileName, "a");
        KG_PROCESS_ERROR(pResult);

        for (int nNum = 0; nNum < TEST_TIME; nNum++)
        {
            nAccount = 0;
            nGetItemNum = 1;
            while(true)
            {
                nAccount++;
                nRetCode = m_RandTestSmall.RandomResult(nIndex, nIndex);
                if (nRetCode)
                {
                    nGetItemNum--;
                    if (nGetItemNum == 0)
                    {
                        break;
                    }
                }
            }
            iter = mapGetItemIndex.find(nAccount);
            if (iter!= mapGetItemIndex.end())
            {
                iter->second = iter->second + 1;
            }
            else
            {
                mapGetItemIndex.insert(pair< int, int >(nAccount, 1));
            }

            if (m_bInitFlag)
            {
                UnInit();
            }

            nRetCode = Init();
            KG_PROCESS_ERROR(nRetCode);
        }

        //������
        for (iter = mapGetItemIndex.begin(); iter != mapGetItemIndex.end(); iter++)
        {
            fprintf(pResult, "%d\t%d\n", iter->first, iter->second);
        }
        fclose(pResult);
        pResult = NULL;
    }

    nResult = true;
Exit0:

    if (pResult)
    {
        fclose(pResult);
        pResult = NULL;
    }
    return nResult;
}

BOOL KRandDataAutoTest::TestNormal()
{
    BOOL     nRetCode    =   false;
    int     nResult     =   false;
    int     nAccount    =   0;
    int     nGetItemNum =   10;
    FILE    *pResult   =   NULL;
    char    szFileName[MAX_PATH];
    
    map< int, int > mapGetItemIndex;
    map< int, int >::iterator iter;
    

    if (m_bInitFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);

    //ʹ��һ��ѭ�������и�������Ĳ��ԡ������ĵ����ʴ�1024��MILLION_NUM����0.1%��99.9%)����1024Ϊ��������¼���ݡ�

    for (int nIndex = MILLION_STEP; nIndex < MILLION_NUM; nIndex= nIndex + MILLION_STEP)
    {
        
        mapGetItemIndex.clear();

        snprintf(szFileName, sizeof(szFileName), "DropDistribute%d.tab", nIndex);
        szFileName[sizeof(szFileName) -1] = '\0';

        pResult = fopen(szFileName, "a");
        KG_PROCESS_ERROR(pResult);

        for (int nNum = 0; nNum < TEST_TIME; nNum++)
        {
            if (!(nNum % 100000))
            {
                printf("TestTime: %d\n", nNum);
            }
            nAccount = 0;
            nGetItemNum = 1;
            while(true)
            {
                nAccount++;
                nRetCode = m_RandTest.RandomResult(nIndex, nIndex);
                if (nRetCode)
                {
                    nGetItemNum--;
                    if (nGetItemNum == 0)
                    {
                        break;
                    }
                }
            }
            iter = mapGetItemIndex.find(nAccount);
            if (iter!= mapGetItemIndex.end())
            {
                iter->second = iter->second + 1;
            }
            else
            {
                mapGetItemIndex.insert(pair< int, int >(nAccount, 1));
            }

            if (m_bInitFlag)
            {
                UnInit();
            }

            nRetCode = Init();
            KG_PROCESS_ERROR(nRetCode);
        }

        //������
        for (iter = mapGetItemIndex.begin(); iter != mapGetItemIndex.end(); iter++)
        {
            fprintf(pResult, "%d\t%d\n", iter->first, iter->second);
        }

        fclose(pResult);
        pResult = NULL;
    }

    nResult = true;
Exit0:

    if (pResult)
    {
        fclose(pResult);
        pResult = NULL;
    }
    return nResult;
}


int KRandDataAutoTest::DoTest(int nTestCase)
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    FILE *fpResult  =   NULL;
    
    fpResult = fopen("AutoTestResult.log", "a");
    KG_PROCESS_ERROR(fpResult);

    switch(nTestCase)
    {
    case rdtcWrongZero:
        {
            nRetCode = TestWrongZero();
            if (nRetCode)
            {
                fputs("������Ϊ0����(24451)����ͨ��\n", fpResult);
            }
            else 
            {
                fputs("������Ϊ0����(24451)����ʧ��\n", fpResult);
            }
            break;
        }
    case rdtcWrongHundred:
        {
            nRetCode = TestWrongHundred();
            if (nRetCode)
            {
                fputs("�����ʳ����ٷ�֮������(24452)���Գɹ�\n", fpResult);
            }
            else 
            {
                fputs("�����ʳ����ٷ�֮������(24452)����ʧ��\n", fpResult);
            }
            break;
        }
    case rdtcNormal:
        {
            {
                nRetCode = TestNormal();
                if (nRetCode)
                {
                    fputs("��������������(24449)��������100W����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("��������������(24449)��������100W����ʧ��\n", fpResult);
                }
                break;
            }
        }
    case rdtcSmall:
        {
            {
                nRetCode = TestSmall();
                if (nRetCode)
                {
                    fputs("��������������(24449)��������100����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("��������������(24449)��������100����ʧ��\n", fpResult);
                }
                break;
            }
        }
    default:
        {
            fputs("�������", fpResult);
            break;
        }

    }
    
    nResult = true;
Exit0:
    if (fpResult)
    {
        fclose(fpResult);
        fpResult = NULL;
    }
    return nResult;
}

int main(int argc, char* argv[])
{
    BOOL                nRetCode    =   false;
    int                 nResult     =   false;
    BOOL                bInitFlag   =   false;

    KRandDataAutoTest   kTest;
    
    nRetCode = kTest.Init();
    KG_PROCESS_ERROR(nRetCode);
    bInitFlag = true;

    for (int nIndex = rdtcBegin + 1; nIndex < rdtcTotal; nIndex++)
    {
        nRetCode = kTest.DoTest(nIndex);
        KG_PROCESS_ERROR(nRetCode);
    }

    nResult = true;
Exit0:
    if (bInitFlag)
    {
        kTest.UnInit();
    }
	return nResult;
}

