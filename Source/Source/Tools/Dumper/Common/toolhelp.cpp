#include "stdafx.h"
#include "toolhelp.h"
#include "nb30.h"
#include "KSG_MD5_String.h"
#include <atlstr.h>

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
	ASTAT	Adapter = {0}; 
	NCB		ncb; 
	UCHAR	uRetCode; 

	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBRESET; 
	ncb.ncb_lana_num = lana_num; 
	// ָ�������� 

	// ���ȶ�ѡ������������һ��NCBRESET����Ա���г�ʼ�� 
	uRetCode = Netbios( &ncb ); 
	if (uRetCode != NRC_GOODRET || ncb.ncb_retcode != NRC_GOODRET)
		return false;

	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBASTAT; 
	ncb.ncb_lana_num = lana_num; // ָ�������� 

	strcpy( (char *)ncb.ncb_callname, "*     " );
	ncb.ncb_buffer = (unsigned char *) &Adapter; 

	// ָ�����ص���Ϣ��ŵı��� 
	ncb.ncb_length = sizeof(Adapter); 

	// ���ţ����Է���NCBASTAT�����Ի�ȡ��������Ϣ 
	uRetCode = Netbios( &ncb ); 
	if ( uRetCode == 0 && ncb.ncb_retcode == 0) 
	{ 
		// ������MAC��ַ��ʽ���ɳ��õ�16������ʽ����0010-A4E4-5802 
		if (
			Adapter.adapt.adapter_address[0] == 0 &&
			Adapter.adapt.adapter_address[1] == 0 &&
			Adapter.adapt.adapter_address[2] == 0 &&
			Adapter.adapt.adapter_address[3] == 0 &&
			Adapter.adapt.adapter_address[4] == 0 &&
			Adapter.adapt.adapter_address[5] == 0
			)
		{
			return false; 
		}	

		strMac.Format("%02X-%02X-%02X-%02X-%02X-%02X\n",
			Adapter.adapt.adapter_address[0], 
			Adapter.adapt.adapter_address[1], 
			Adapter.adapt.adapter_address[2], 
			Adapter.adapt.adapter_address[3], 
			Adapter.adapt.adapter_address[4], 
			Adapter.adapt.adapter_address[5]
		);

		return true;
	} 

	return false;
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
	if ( uRetCode == 0 && ncb.ncb_retcode == 0) 
	{ 
		// ��ÿһ�������������������Ϊ�����ţ���ȡ��MAC��ַ 
		for ( int i = 0; i< lana_enum.length; ++i) 
		{
			CString strMacOne;
			if (getmac_mac(lana_enum.lana[i], strMacOne))
			{
				if (strMac.GetLength() > 0)
				{
					strMac.Append("&");
				}
				strMac.Append(strMacOne);

				nNet++;
			}
		}
	}

	return nNet;
}


int GetUUID(char szUUID[64])
{
	CString strHardId;
	int nNet = 0;

	szUUID[0] = '\0';
	 
	nNet = getnet_mac(strHardId);
	if (nNet > 0 && strHardId.GetLength() > 0)
	{
		KG_EDStringToMD5String(szUUID, strHardId.GetString());
		return true;
	}

	return false;
}

int GetGUID(char szGUID[64])
{
    int nResult = false;
    GUID guid = {0};
    HRESULT hr = E_FAIL;
    char szBuf[64] = {0};
    szGUID[0] = '\0';

    CoInitialize(NULL);

    hr = CoCreateGuid(&guid);
    if (hr == E_FAIL)
        goto Exit0;

    _snprintf(
        szBuf, sizeof(szBuf),
        "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
    );

    KG_EDStringToMD5String(szGUID, szBuf);

    nResult = true;
Exit0:
    CoUninitialize();
    return nResult;
}