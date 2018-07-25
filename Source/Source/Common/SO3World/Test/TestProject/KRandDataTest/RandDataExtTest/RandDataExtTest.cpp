// RandDataExtTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "KProbability.h"
#include "KRandDataExtTest.h"
#include <math.h>

int KRandDataExtTest::Init()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;

    nRetCode = m_RandExtTest.Init();
    KG_PROCESS_ERROR(nRetCode);
    m_bInitExtFlag = true;

    nResult = true;
Exit0:
    return nResult;
}

void KRandDataExtTest::UnInit()
{
    m_RandExtTest.UnInit();
    m_bInitExtFlag = false;
}

BOOL KRandDataExtTest::TestSaveNothing()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    BYTE byBuffer[MAX_BUFFERLENTGH];

    if (m_bInitExtFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);
    //������ɾ�������´�������֤���ݵĸɾ�

    m_RandExtTest.Save(byBuffer, MAX_BUFFERLENTGH);

    UnInit();
    //�����֮��������ɾ��������´���һ��

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);

    m_RandExtTest.Load(byBuffer, MAX_BUFFERLENTGH);

    KG_PROCESS_ERROR(m_RandExtTest.m_RandCache.Size() == 0);

    nResult = true;
Exit0:
    return nResult;

}

BOOL KRandDataExtTest::TestSaveOne()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    BYTE byBuffer[MAX_BUFFERLENTGH];

    if (m_bInitExtFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);
    //������ɾ�������´�������֤���ݵĸɾ�
    
    m_RandExtTest.RandomResult(1, 500, MAX_PROBABILITY - 1);

    m_RandExtTest.Save(byBuffer, MAX_BUFFERLENTGH);

    UnInit();
    //�����֮��������ɾ��������´���һ��

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);

    m_RandExtTest.Load(byBuffer, MAX_BUFFERLENTGH);

    KG_PROCESS_ERROR(m_RandExtTest.m_RandCache.Size() == 1);

    nResult = true;
Exit0:
    return nResult;

}

BOOL KRandDataExtTest::TestSaveAll()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    BYTE byBuffer[MAX_BUFFERLENTGH];

    if (m_bInitExtFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);
    //������ɾ�������´�������֤���ݵĸɾ�

    for (int i = 1; i <= 6; i++)
    {
        m_RandExtTest.RandomResult(i, 500, MAX_PROBABILITY - 1);
    }

    m_RandExtTest.Save(byBuffer, MAX_BUFFERLENTGH);

    UnInit();
    //�����֮��������ɾ��������´���һ��

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);

    m_RandExtTest.Load(byBuffer, MAX_BUFFERLENTGH);

    KG_PROCESS_ERROR(m_RandExtTest.m_RandCache.Size() == 5);

    nResult = true;
Exit0:
    return nResult;
}

BOOL KRandDataExtTest::TestRandom(DWORD nProbabilityValue)
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    
    int nAccount    =   0;
    
    int nWantResult = (int)ceil(1000 * 2.5 / (float)nProbabilityValue);

    // ������Ϊ100%ʱ��Ԥ�ڽ��Ӧ����1��
    if (nProbabilityValue == 1000)
        nWantResult = 1;
    

    //������ɾ�������´�������֤���ݵĸɾ�
    if (m_bInitExtFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);


    while(nAccount <= 65535)    //�������ô��λ�û�ɹ����Ǳ�����ǳ�����
    {
        nAccount++;
        // 999��g_Random(1000)�����ֵ��Ҳ�������ʧ�ܵ�ֵ
        nRetCode = m_RandExtTest.RandomResult(1, nProbabilityValue, MAX_PROBABILITY - 1);
        if (nRetCode)
            break;
    }
    
    KG_PROCESS_ERROR(nAccount == nWantResult);

    nResult = true;
Exit0:
    return nResult;
}



BOOL KRandDataExtTest::Test2Key01()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    int nAccount    =   0;

    //������ɾ�������´�������֤���ݵĸɾ�
    if (m_bInitExtFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);

    nRetCode = m_RandExtTest.RandomResult(1, 500, MAX_PROBABILITY - 1);
    KG_PROCESS_ERROR(!nRetCode);        //�ȶ�ʧ��

    while(nAccount <= 65535)    //��ô����ˣ�һ������ʲô����
    {
        nAccount++;
        nRetCode = m_RandExtTest.RandomResult(2, 500, MAX_PROBABILITY - 1);
        if (nRetCode)
            break;
        
    }

    KG_PROCESS_ERROR(nAccount == 5);

    nResult = true;
Exit0:
    return nResult;
}

BOOL KRandDataExtTest::Test2Key02()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    int nAccount    =   0;


    //������ɾ�������´�������֤���ݵĸɾ�
    if (m_bInitExtFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);


    while(nAccount <= 65535)    //��ô����ˣ�һ������ʲô����
    {
        nAccount++;

        nRetCode = m_RandExtTest.RandomResult(1, 200, MAX_PROBABILITY - 1);
        if (nRetCode)
        {
            break;
        }

        nRetCode = m_RandExtTest.RandomResult(2, 500, MAX_PROBABILITY - 1);
        if (nRetCode)
        {
            KG_PROCESS_ERROR(nAccount == 5 || nAccount == 10);
        }

    }

    KG_PROCESS_ERROR(nAccount == 13);

    nResult = true;
Exit0:
    return nResult;
}

BOOL KRandDataExtTest::TestGetItem01()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    int nAccount    =   0;


    //������ɾ�������´�������֤���ݵĸɾ�
    if (m_bInitExtFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);

    nRetCode = m_RandExtTest.RandomResult(1, 500, MAX_PROBABILITY - 1);
    KG_PROCESS_ERROR(!nRetCode);    // һ��ʧ��

    nRetCode = m_RandExtTest.RandomResult(1, 500, 0);
    KG_PROCESS_ERROR(nRetCode);     //һ���ɹ�

    while (nAccount < 65535)
    {
        nAccount++;
        nRetCode = m_RandExtTest.RandomResult(1, 500, MAX_PROBABILITY - 1);
        if (nRetCode)
            break;
    }

    KG_PROCESS_ERROR(nAccount == 5);

    nResult = true;
Exit0:
    return nResult;
}

BOOL KRandDataExtTest::TestGetItem02()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    int nAccount    =   0;


    //������ɾ�������´�������֤���ݵĸɾ�
    if (m_bInitExtFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);

    nRetCode = m_RandExtTest.RandomResult(1, 500, 0);
    KG_PROCESS_ERROR(nRetCode);     //һ���ɹ�

    while (nAccount < 65535)
    {
        nAccount++;
        nRetCode = m_RandExtTest.RandomResult(1, 500, MAX_PROBABILITY - 1);
        if (nRetCode)
            break;
    }

    KG_PROCESS_ERROR(nAccount == 5);

    nResult = true;
Exit0:
    return nResult;
}

BOOL KRandDataExtTest::TestOverKey01()
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    int nAccount    =   0;


    //������ɾ�������´�������֤���ݵĸɾ�
    if (m_bInitExtFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);

    for (int nIndex = 0; nIndex < 4; nIndex++)
    {
        nRetCode = m_RandExtTest.RandomResult(1, 500, MAX_PROBABILITY - 1);
        KG_PROCESS_ERROR(!nRetCode);
    }

    for (int nIndex = 2; nIndex <= 6; nIndex++)
    {
        nRetCode = m_RandExtTest.RandomResult(nIndex, 500, MAX_PROBABILITY - 1);
        KG_PROCESS_ERROR(!nRetCode);
    }

    while (nAccount < 65535) //��ô����ˣ��϶��������ʲô����
    {
        nAccount++;
        nRetCode = m_RandExtTest.RandomResult(1, 500, MAX_PROBABILITY - 1);
        if (nRetCode)
            break;
    }

    KG_PROCESS_ERROR(nAccount == 5);

    nResult = true;
Exit0:
    return nResult;
}

BOOL KRandDataExtTest::TestOverKey02()
{

    BOOL nRetCode    =   false;
    int nResult     =   false;
    int nAccount    =   0;


    //������ɾ�������´�������֤���ݵĸɾ�
    if (m_bInitExtFlag)
    {
        UnInit();
    }

    nRetCode = Init();
    KG_PROCESS_ERROR(nRetCode);

    for (int nIndex = 0; nIndex < 4; nIndex++)
    {
        nRetCode = m_RandExtTest.RandomResult(1, 500, MAX_PROBABILITY - 1);
        KG_PROCESS_ERROR(!nRetCode);
    }

    for (int nIndex = 2; nIndex <= 6; nIndex++)
    {
        nRetCode = m_RandExtTest.RandomResult(nIndex, 500, MAX_PROBABILITY - 1);
        KG_PROCESS_ERROR(!nRetCode);
    }

    while (nAccount < 65535) //��ô����ˣ��϶��������ʲô����
    {
        nAccount++;
        nRetCode = m_RandExtTest.RandomResult(1, 500, MAX_PROBABILITY - 1);
        if (nRetCode)
            break;
    }

    KG_PROCESS_ERROR(nAccount == 5);

    nAccount = 0; // ��0������һ��
    
    while (nAccount < 65535)
    {
        nAccount++;
        nRetCode = m_RandExtTest.RandomResult(3, 500, MAX_PROBABILITY - 1);
        if (nRetCode)
            break;
    }

    KG_PROCESS_ERROR(nAccount == 4);

    nResult = true;
Exit0:
    return nResult;
}

int KRandDataExtTest::DoTest(int nTestCase)
{
    BOOL nRetCode    =   false;
    int nResult     =   false;
    FILE *fpResult  =   NULL;

    fpResult = fopen("ExtTestResult.log", "a");
    KG_PROCESS_ERROR(fpResult);

    switch(nTestCase)
    {
    case rdetcSaveNothing:
        {
            nRetCode = TestSaveNothing();
            if (nRetCode)
            {
                fputs("�洢0����Key����(24443)����ͨ��\n", fpResult);
            }
            else 
            {
                fputs("�洢0����Key����(24443)����ʧ��\n", fpResult);
            }
            break;
        }
    case rdetcSaveOne:
        {
            nRetCode = TestSaveOne();
            if (nRetCode)
            {
                fputs("�洢1����Key����(24444)����ͨ��\n", fpResult);
            }
            else 
            {
                fputs("�洢1����Key����(24444)����ʧ��\n", fpResult);
            }
            break;
        }
    case rdetcSaveAll:
        {
            nRetCode = TestSaveAll();
            if (nRetCode)
            {
                fputs("�洢������Key����(24445)����ͨ��\n", fpResult);
            }
            else 
            {
                fputs("�洢������Key����(24445)����ʧ��\n", fpResult);
            }
            break;
        }
    case rdetcRandom02:
        {
            nRetCode = TestRandom(2);
            if (nRetCode)
            {
                fputs("������Ϊ0.002����������(24453)����ͨ��\n", fpResult);
            }
            else 
            {
                fputs("������Ϊ0.002����������(24453)����ʧ��\n", fpResult);
            }
            break;
        }
    case rdetcRandom10:
        {
            nRetCode = TestRandom(100);
            if (nRetCode)
            {
                fputs("������Ϊ0.1����������(24454)����ͨ��\n", fpResult);
            }
            else 
            {
                fputs("������Ϊ0.1����������(24454)����ʧ��\n", fpResult);
            }
            break;
        }
    case rdetcRandom20:
        {
            {
                nRetCode = TestRandom(200);
                if (nRetCode)
                {
                    fputs("������Ϊ0.2����������(24455)����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("������Ϊ0.2����������(24455)����ʧ��\n", fpResult);
                }
                break;
            }
        }
    case rdetcRandom50:
        {
            {
                nRetCode = TestRandom(500);
                if (nRetCode)
                {
                    fputs("������Ϊ0.5����������(24456)����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("������Ϊ0.5����������(24456)����ʧ��\n", fpResult);
                }
                break;
            }
        }
    case rdetcRandom100:
        {
            {
                nRetCode = TestRandom(1000);
                if (nRetCode)
                {
                    fputs("������Ϊ1����������(24457)����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("������Ϊ1����������(24457)����ʧ��\n", fpResult);
                }
                break;
            }
        }
    case rdetcGetItem01:
        {
            {
                nRetCode = TestGetItem01();
                if (nRetCode)
                {
                    fputs("�ɹ�ʱ������ղ�������01(24449)����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("�ɹ�ʱ������ղ�������01(24449)����ʧ��\n", fpResult);
                }
                break;
            }
        }
    case rdetcGetItem02:
        {
            {
                nRetCode = TestGetItem02();
                if (nRetCode)
                {
                    fputs("�ɹ�ʱ������ղ�������02(24449)����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("�ɹ�ʱ������ղ�������02(24449)����ʧ��\n", fpResult);
                }
                break;
            }
        }
    case rdetc2Key01:
        {
            {
                nRetCode = Test2Key01();
                if (nRetCode)
                {
                    fputs("��Key�������Ų�������01(24449)����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("��Key�������Ų�������01(24449)����ʧ��\n", fpResult);
                }
                break;
            }
        }
    case rdetc2Key02:
        {
            {
                nRetCode = Test2Key02();
                if (nRetCode)
                {
                    fputs("��Key�������Ų�������02(24449)����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("��Key�������Ų�������02(24449)����ʧ��\n", fpResult);
                }
                break;
            }
        }
    case rdetcOverKey01:
        {
            {
                nRetCode = TestOverKey01();
                if (nRetCode)
                {
                    fputs("��Key���ǲ�������01(24449)����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("��Key���ǲ�������01(24449)����ʧ��\n", fpResult);
                }
                break;
            }
        }
    case rdetcOverKey02:
        {
            {
                nRetCode = TestOverKey02();
                if (nRetCode)
                {
                    fputs("��Key���ǲ�������02(24449)����ͨ��\n", fpResult);
                }
                else 
                {
                    fputs("��Key���ǲ�������02(24449)����ʧ��\n", fpResult);
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
    BOOL                 nRetCode    =   false;
    int                 nResult     =   false;
    BOOL                bInitFlag   =   false;

    KRandDataExtTest kRandTest;

    nRetCode = kRandTest.Init();
    KG_PROCESS_ERROR(nRetCode);
    bInitFlag = true;

    for (int nIndex = rdetcBegin + 1; nIndex < rdetcTotal; nIndex++)
    {
        nRetCode = kRandTest.DoTest(nIndex);
        KG_PROCESS_ERROR(nRetCode);
    }


    nResult = true;
Exit0:
    if (bInitFlag)
    {
        kRandTest.UnInit();
    }
    return nResult;
}

