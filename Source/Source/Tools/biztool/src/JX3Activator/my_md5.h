/**
* @file     my_md5.h
* @brief    ����MD5ֵ
*
* @details  �ַ���MD5ֵ����
* @author   xiaojian <xiaojian@kingsoft.com>
* @date     2008-08-26
* @version	1.0.0.1
*/

#ifndef _MY_MD5_H_
#define _MY_MD5_H_

#include "md5.h"

/**
* @brief		    �����ַ���MD5ֵ
*
* @details 
* @param[in]        ��Ҫ������ַ�����ַ
* @param[out]       ���������
* @param[in]        �������������ڴ��С
* @return[true]     �ɹ�
* @return[false]    ʧ��
* @remarks	
*/
inline bool StringMD5(const CHAR* pInStr, CHAR* pOutStr, UINT uSize)
{
    bool    bRet = false;
    //��������MD5ֵ
    MD5_CTX md5PostString;
    UCHAR   ucFinish[32]    = {0};
    CHAR    szHex[10]       = {0};
    CHAR    szResult[33]    = {0};

    if (NULL == pInStr && uSize > 32)
    {
        return bRet;
    }

    md5PostString.MD5Update((unsigned char*)pInStr, (unsigned)strlen(pInStr));        
    md5PostString.MD5Final((unsigned char*)ucFinish);

    for (int i = 0; i < 16; i++)
    {
        sprintf(szHex, "%02x", ucFinish[i]);
        strcat( szResult, szHex );
    }

    strcpy(pOutStr, szResult);

    bRet = true;

    return bRet;
}

#endif // _MY_MD5_H_