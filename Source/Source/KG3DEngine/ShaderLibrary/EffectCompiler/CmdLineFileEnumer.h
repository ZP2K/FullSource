////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : CmdLineFileEnumer.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-5-7 15:15:56
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_CMDLINEFILEENUMER_H_
#define _INCLUDE_CMDLINEFILEENUMER_H_

////////////////////////////////////////////////////////////////////////////////
struct ICFileEnumer;

ICFileEnumer* CreateCmdLineFileEnumer(LPCTSTR strCmdLine, LPCTSTR strDirectory);//����/F:"xx" "xx"�����������е��ļ����ļ������������������ġ��ļ������û��·������Ϊ�ǵ�ǰ·��


#endif //_INCLUDE_CMDLINEFILEENUMER_H_
