/************************************************************************/
/* ��Ϸ�����������						                                */
/* Copyright : Kingsoft 2004										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.01.01	Create												*/
/* Comment	 :															*/
/*		������Ϸ��������Ҫ��ID������Ҫ�����ڴ����Ķ��󣬶�����������	*/
/************************************************************************/
#ifndef _KBASEOBJECT_H_
#define	_KBASEOBJECT_H_

#include "Luna.h"

class KBaseObject
{
public:
	DWORD			m_dwID;

	DECLARE_LUA_DWORD(ID);
};
#endif