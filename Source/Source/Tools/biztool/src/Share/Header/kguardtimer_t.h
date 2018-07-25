/* -------------------------------------------------------------------------
//	�ļ���		��	kguardtimer_t.h
//	������		��	wangbin
//	����ʱ��	��	2008-1-9 10:38:00
//	��������	��	���̼�ͨ�Ŷ�ʱ��ģ��
// -----------------------------------------------------------------------*/
#ifndef __KGUARDTIMER_T_H__
#define __KGUARDTIMER_T_H__

#include "ksdef.h"
#include <list>

template <class KType>
class KGuardTimer_T
{
public:
	typedef VOID (KType::*fnTimer)();
	struct KTIMER
	{
		fnTimer		pfnTimer;
		KType*		pTimer;
		time_t		tLastTime;
		INT			nInterval;
	};
public:
	VOID Activate(time_t tNow)
	{
		ASSERT(tNow > 0);
		typename std::list<KTIMER>::iterator it = m_listTimer.begin();
		for (; it != m_listTimer.end(); ++it)
		{
			KTIMER& sTimer = *it;
			if (tNow - sTimer.tLastTime >= sTimer.nInterval)
			{
				(sTimer.pTimer->*sTimer.pfnTimer)();
				sTimer.tLastTime = tNow;
			}
		}
	}
	// ע�ᶨʱ����ʱ����Ϊ��
	VOID Register(KType* pTimer, fnTimer pfnTimer, INT nInterval)
	{
		ASSERT(pTimer && pfnTimer && nInterval > 0);
		KTIMER sTimer;
		sTimer.pTimer	 = pTimer;
		sTimer.pfnTimer	 = pfnTimer;
		sTimer.tLastTime = 0;
		sTimer.nInterval = nInterval;
		m_listTimer.push_back(sTimer);
	}
	VOID Clear()
	{
		m_listTimer.clear();
	};
private:
	std::list<KTIMER> m_listTimer;
};

#endif //__KGUARDTIMER_T_H__
