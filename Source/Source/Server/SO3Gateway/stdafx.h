// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif		

#include "KGStdAfx.h"

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#include <WinCon.h>
#include <mbstring.h>
#endif

#ifdef __GNUC__
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <limits.h>
#endif

#ifdef WIN32
#include <conio.h>
#endif

#include "Ksdef.h"

#include <stdio.h>
#include <map>
#include <list>
#include <string>
#include <string.h>

#include "KGPublic.h"
#include "SO3GlobalDef.h"
#include "ProtocolBasic.h"
#include "Common/KG_Socket.h"
#include "Engine/KG_Time.h"
#include "Engine/KThread.h"
#include "Engine/KGLog.h"
#include "Engine/FileType.h"
#include "Engine/File.h"
#include "Engine/KSG_MD5_String.h"
#include "Engine/inoutmac.h"
#include "Engine/Random.h"


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
