/**
* @file     toolhelp.h
* @brief    �ṩһЩ����������
*
* @details  
* @author   xiaojian <xiaojian@kingsoft.com>
* @date     2008-09-09
* @version	1.0.0.1
*/

#ifndef __TOOL_HELP_H__
#define __TOOL_HELP_H__
#include "stdafx.h"

TCHAR* trim(TCHAR* str);
bool getmac_mac(int lana_num, CString& strMac);
int getnet_mac(CString& strMac);

#endif  /* __TOOL_HELP_H__ */