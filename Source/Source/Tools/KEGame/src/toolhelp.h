/**
* @file     toolhelp.h
* @brief    �ṩһЩ����������
*
* @details  
* @author   xiaojian <xiaojian@kingsoft.com>
* @date     2008-09-09
* @version	1.0.0.1
*/

#include "nb30.h"

#pragma comment(lib,"netapi32.lib")

typedef struct _ASTAT_ 
{ 
	ADAPTER_STATUS adapt; 
	NAME_BUFFER NameBuff [30]; 
}ASTAT, * PASTAT; 

/**
* @brief		    ɾ���ַ�����β�ո�
*
* @details
* 
* @param[in]        Դ�ַ���
* @return[TCHAR*]   ����ַ���
* @return[0]        ת��ʧ��
* @remarks	
*/
TCHAR* trim(TCHAR* str)
{
    if( ' ' == *str )
    {
        while( ' '== *str )
        {
            str++;
            if( 0 == *str )
               return 0;
        }
        trim( str );        
        return str;
    }
    else
    {
        while ( ' ' != *str )
        {
            str++;
            if( 0 == *str )
                return 0;
        }
        if( 0 == trim( str ) )
        {
            *str=0;
            return str;
        }
        return str;
    }
}

/**
* @brief		    ��ȡָ������Mac��ַ
*
* @details
* 
* @param[in]		ָ����������
* @param[out]		����Mac��ַ
* @return[true]		�ɹ�
* @return[false]	ʧ��
* @remarks			pszMac���Ȳ���С��18
*/

bool getmac_mac(int lana_num, CString& strMac) 
{
	bool	bRet = false;
	ASTAT	Adapter; 
	NCB		ncb; 
	UCHAR	uRetCode; 

	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBRESET; 
	ncb.ncb_lana_num = lana_num; 
	// ָ�������� 

	// ���ȶ�ѡ������������һ��NCBRESET����Ա���г�ʼ�� 
	uRetCode = Netbios( &ncb ); 

	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBASTAT; 
	ncb.ncb_lana_num = lana_num; // ָ�������� 

	strcpy( (char *)ncb.ncb_callname, 
		"* " ); 
	ncb.ncb_buffer = (unsigned char *) &Adapter; 

	// ָ�����ص���Ϣ��ŵı��� 
	ncb.ncb_length = sizeof(Adapter); 

	// ���ţ����Է���NCBASTAT�����Ի�ȡ��������Ϣ 
	uRetCode = Netbios( &ncb ); 
	if ( uRetCode == 0 ) 
	{ 
		// ������MAC��ַ��ʽ���ɳ��õ�16������ʽ����0010-A4E4-5802 
		strMac.Format(L"%02X-%02X-%02X-%02X-%02X-%02X\n",
			Adapter.adapt.adapter_address[0], 
			Adapter.adapt.adapter_address[1], 
			Adapter.adapt.adapter_address[2], 
			Adapter.adapt.adapter_address[3], 
			Adapter.adapt.adapter_address[4], 
			Adapter.adapt.adapter_address[5]
		); 

		bRet = true;
	} 

	return bRet;
}

/**
* @brief		    ��ȡ��������MAC��ַ
*
* @details
* 
* @param[out]       Mac�ַ���
* @return[int]		������
* @remarks			������С�ڵ���0���ʾʧ��
*/
int getnet_mac(CString& strMac)
{
	int			nNet = 0;
	NCB			ncb; 
	UCHAR		uRetCode; 
	LANA_ENUM	lana_enum; 

	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBENUM; 

	ncb.ncb_buffer = (unsigned char *) &lana_enum; 
	ncb.ncb_length = sizeof(lana_enum); 

	// ����������NCBENUM����Ի�ȡ��ǰ������������Ϣ�����ж��ٸ�������ÿ�������ı�ŵ� 
	uRetCode = Netbios( &ncb ); 
	if ( uRetCode == 0 ) 
	{ 
		// ��ÿһ�������������������Ϊ�����ţ���ȡ��MAC��ַ 
		for ( int i=0; i< lana_enum.length; ++i) 
		{
			CString strMacOne;
			if (getmac_mac(lana_enum.lana[i], strMacOne))
			{
				if (strMac.GetLength() > 0)
				{
					strMac.Append(L"&");
				}
				strMac.Append(strMacOne);

				nNet++;
			}
		}
			
	}

	return nNet;
}