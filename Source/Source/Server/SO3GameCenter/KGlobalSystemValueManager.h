//ע�� ����ȫ�ֲ����Ĳ��裺
//1. ����KGlobalSystemValueManager��Ա����KSYSTEM_VALUE m_Name
//2. ʹ�ú�DECLARE_SYSTEM_VALUE(Name)��������, ʹ�ú�REGISTER_LUA_INTEGERע��
//3. ʹ�ú�SAVE_GLOBAL_SYSTEM_VALUE(Name)��Activate()�б���
//4. ��KDBTools::LoadGlobalSystemValue()����GetGlobalSystemValue()��ȡ
//5. ����KGlobalSystemValueCache��Ӧ��Ա����m_nName
//6. �޸�ͬ��Э��R2S_SYNC_GLOBAL_SYSTEM_VALUE �ͷ��ͺͽ���Э����Ĵ���

#ifndef _KGLOBAL_SYSTEM_VALUE_MANAGER_H_
#define _KGLOBAL_SYSTEM_VALUE_MANAGER_H_

#define DECLARE_SYSTEM_VALUE(name)                  \
    int get##name(void) { return m_##name.nValue; }	\
    void set##name(int nValue)          \
    {                                   \
        if (nValue != m_##name.nValue)  \
        {                               \
            m_bNeedSyncToServer = true; \
			m_##name.bUpdate = true;    \
			m_##name.nValue = nValue;   \
		}                               \
	}

class KGlobalSystemValueManager
{
public:
    KGlobalSystemValueManager();
    ~KGlobalSystemValueManager();

    BOOL Load();

    void Activate();

    void SaveAll();

private:
    struct KSYSTEM_VALUE
    {
        KSYSTEM_VALUE() : bUpdate(false), nValue(0) {}

        BOOL    bUpdate;
        int     nValue;
    };

    time_t          m_NextSaveTime;

public:
    BOOL            m_bNeedSyncToServer;

    KSYSTEM_VALUE   m_GameWorldStartTime;
    KSYSTEM_VALUE   m_MaxPlayerLevel;
    KSYSTEM_VALUE   m_GameWorldChargeTime;

	DECLARE_LUA_CLASS(KGlobalSystemValueManager);
	DECLARE_SYSTEM_VALUE(GameWorldStartTime);
	DECLARE_SYSTEM_VALUE(MaxPlayerLevel);
	DECLARE_SYSTEM_VALUE(GameWorldChargeTime);
};

#endif
