#ifndef _KAI_PARAM_TEMPLATE_LIST_
#define _KAI_PARAM_TEMPLATE_LIST_

#include <map>
#include "Global.h"

#ifdef _SERVER
#include "KAIEvent.h"
#endif //_SERVER

// AI��ģ������������������ж�̬�޸ĵĲ���
struct KAIParamTemplate
{
	BOOL bIsPositive;			//�Ƿ�������
	KAI_STATE eAIMainState;		//NPCĬ����Ϊ

	int nAlertRange;			//���䷶Χ
	int nAlertOverTime;			//���䳬ʱʱ�䣨��ʱ��תΪս����
	int nAttackRange;			//������Χ
	int nPursuitRange;			//׷������

	int nEscapeTime;			//ÿ�����ܵĳ���ʱ��
	int nEscapeRange;			//����ÿ�����ߵ�������
	int nEscapeInterval;		//���ܼ��

	int nWanderRange;			//�й�ķ�Χ
	int nWanderDistance;		//�й�ÿ�����ߵ�������
	int nWanderInterval;		//�й�ļ��

	//int nPatrolPathID;			//Ѳ��·��ID

#ifdef _SERVER
	//AIģ��ĳ�ʼ���ű�
	char szScriptName[MAX_PATH];
	KAIEvent AIEvent[MAX_AI_EVENT];
#endif //_SERVER
};

class KAIParamTemplateList
{
public:
	KAIParamTemplateList(void);
	~KAIParamTemplateList(void);

	BOOL Init();
	void UnInit();

	BOOL LoadTemplate(int nIndex, ITabFile* pTabFile, KAIParamTemplate* pAITemplate);
	KAIParamTemplate* GetTemplate(DWORD dwID);

private:
	typedef std::map<DWORD, KAIParamTemplate*>  AI_PARAM_TEMPLATE_MAP;
	AI_PARAM_TEMPLATE_MAP m_mapAIParamTemplate;
	KAIParamTemplate* m_pAITemplate;
	KAIParamTemplate m_DefaultTemplate;
};

#endif //_KAI_PARAM_TEMPLATE_LIST_
