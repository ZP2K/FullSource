// ����ѡ�����

class KNpc;
class KSkill;

// �������͵ļ���ѡ�����ķ���ֵ����������������
#define SKILL_SELECTOR_IGNORE       0   // ���ͷ�
#define SKILL_SELECTOR_CAST         1   // �ͷ�
#define SKILL_SELECTOR_READY        2   // �����ݸ��ʾ��� 

int StandardTargetPeriodEvaluator(KNpc* pNpc, KTarget* pTarget);
int StandardTargetPeriodRangeEvaluator(KNpc* pNpc, KTarget* pTarget);
int StandardSelfPeriodEvaluator(KNpc* pNpc, KTarget* pTarget);
int StandardPassiveRespond(KNpc* pNpc, KTarget* pTarget);
int StandardSelfHPEvaluator(KNpc* pNpc, KTarget* pTarget, int nLifeRate);
int StandardTargetCountEvaluator(KNpc* pNpc, KTarget* pTarget, int nSkillRate);
int StandardChannelingBreak(KNpc* pNpc, KTarget* pTarget, int nSkillRate);
