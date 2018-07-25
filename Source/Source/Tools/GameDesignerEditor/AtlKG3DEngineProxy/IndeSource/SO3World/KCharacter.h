/************************************************************************/
/* ��ɫ			                                                        */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2004.12.30	Create												*/
/* Comment   :															*/
/*		��ɫ����Һ�NPC�Ĺ�ͬ����										*/
/************************************************************************/

#ifndef _KCHARACTER_H_
#define	_KCHARACTER_H_

#include <list>
#include <bitset>
#include "Global.h"
#include "SO3Result.h"
#include "KAttribute.h"
#include "KSceneObject.h"
#include "KThreatList.h"
#include "KItem.h"
#include "KTarget.h"
#include "KSkill.h"
#include "KSkillDebug.h"
#include "KAIVM.h"
#include "KBuffList.h"

class KPatrolPath;
class KLootList;
class KItem;

// ����ID�ж��Ƿ�����ң����ID�ĸ�4λΪ0
#define IS_NPC(ID)    ((ID) & 0xf0000000)
#define IS_PLAYER(ID) (!IS_NPC(ID))

#define MAX_HIT_VALUE               10000
#define MAX_PARTY_EFFECT_RANGE      (40 * 64)

#define GET_SWIM_WATER_LINE(nHeight) ((nHeight) * 7 / 10)
#define IS_UNDER_WATER(nWaterLine, nCharacterHeight) ((nWaterLine) >= (nCharacterHeight) * 9 / 10)

// �ýṹ���ڽ��ռ�������Ľ��
struct KSKILL_CALCULATE_RESULT
{
    BOOL bShield;
    BOOL bHitTarget;
    BOOL bSkillDodge;
    BOOL bCriticalStrike;
    BOOL bImmunity;
    BOOL bParry;
    BOOL bInsight;

    int  nSkillEffectResult[serTotal];   
    
    BOOL bPhysicsBlock;
    BOOL bSolarMagicBlock;
    BOOL bNeutralMagicBlock;
    BOOL bLunarMagicBlock;
    BOOL bPoisonBlock;

    BOOL bBuffResist;
    BOOL bBuffImmunity;
    int  nBuffID;
    int  nBuffLevel;

    struct
    {
        KSKILL_EFFECT_TYPE  eType;
        DWORD               dwID;
        int                 nLevel;        
    } EffectSrc;
};

// -------------- Over time action ---------------------->
// ����һ��ʱ�����Ϊ

enum KOT_ACTION_TYPE
{
    otActionIdle,
    otActionSkillPrepare,
    otActionSkillChannel,
    otActionRecipePrepare,
    otActionPickPrepare,
	otActionPicking,
	otActionItemSkill,
	otActionCustomPrepare,
	otActionCustomChannel,
    otActionUseItemPrepare
};

struct KProfessionSkill
{
	KTarget Target;
	DWORD dwCraftID;
	DWORD dwRecipeID;
	DWORD dwSourceItemID;	//��������ܵĵ���ID,����п������䷽��,Ҳ�п�����һ�����ĵ���
};

struct KOT_ACTION_PARAM 
{
	KOT_ACTION_TYPE	    eActionType;
	union 
	{
        KSkillRecipePointer SkillRecipePointer;
		DWORD				dwDoodadID;
		DWORD			    dwItemID;
	} Data;
    KTarget		        Target;
	KProfessionSkill	ProfessionSkill;
	int			        nEndFrame;
    union
    {
        int             nNextActFrame;      // ͨ�������������ʾ��һ������ʱ��
        int             nPrepareFrame;      // ���������������ʾ�ü��ܱ�������ʱ�� 
    };

    int                 nBeatBackCount; // ����/ͨ�������˵ļ���
	DWORD				dwScriptID;		// CustomOTAction���һ���ű�
	BOOL				bIsBreakable;	// �Ƿ�ᱻ�������

#ifdef _CLIENT 
    DWORD               dwEndAniID;     // ʹ�õ���  
#endif
};

struct KAUTO_CAST_SKILL_PARAM
{
    KSkillRecipePointer SkillRecipePointer;
    BOOL                bMelee;
    int*                pnCastCycle;
    int                 nPreCastFrame;
};

// <-------------- Over time event ----------------------

// ��ɫ�߶�,Ѱ·�Ľ��
enum KMOVE_RESULT
{
    mrInvalid,
    mrSucceed,      // �����ƶ���
    mrOutOfMap,     // ����ͼ��Ե��,��Ч������
    mrObstacle,     // ���ϰ�,�޷�ǰ��
    mrDeleteSelf,   // �ڿͻ���,����ͬ����Χ֮��,���Լ�ɾ����
    mrReachTarget,  // �Ѿ�����Ŀ��
    mrTotal
};

enum KSKILL_HIT_RESULT
{
    shrInvalid = 0,

    shrHit,
    shrMiss,
    shrDodge,
    shrParry,
    shrInsight,
    shrCriticalStrike,

    shrTotal
};


class KCharacter : public KSceneObject
{
public:
	KCharacter();
	virtual ~KCharacter();

	BOOL Init();
	void UnInit();

	void Activate(void);

public:
	// ����
	char						m_szName[_NAME_LEN];		// ��ɫ��
	char						m_szTitle[TITLE_LEN];		// �ƺ�
	CHARACTER_GENDER	        m_eGender;
	CHARACTER_KIND				m_eKind;					// ��ɫ����
	DWORD						m_dwClassID;				// ����ID
	CHARACTER_RACE				m_eRace;					// ��ɫ����
	DWORD						m_dwForceID;			    // ����
	int							m_nLevel;					// ��ɫ�ȼ�
	DWORD						m_dwCharacterAction;		// ����״̬
    DWORD			            m_dwModelID;                // ģ��ID

#ifdef _SERVER
    // �е�NPC�����ר�е�,ֻ����Ownerͬ��
    // ֮���Էŵ�Character����,��Ϊ�˴����,ʡ�õ����ж�IS_NPC
    DWORD                       m_dwOwner;
#endif

	DECLARE_LUA_STRING(Name, _NAME_LEN);
	DECLARE_LUA_STRING(Title, TITLE_LEN);
	DECLARE_LUA_ENUM(Gender);
	DECLARE_LUA_ENUM(Kind);
	DECLARE_LUA_DWORD(ClassID);
	DECLARE_LUA_ENUM(Race);
	DECLARE_LUA_DWORD(ForceID);
	DECLARE_LUA_INTEGER(Level);
	DECLARE_LUA_DWORD(CharacterAction);
    DECLARE_LUA_DWORD(ModelID);

#ifdef _SERVER
	DECLARE_LUA_DWORD(Owner);
#endif

    // -------------- �ƶ���� -------------------------------------------------->
	CHARACTER_MOVE_STATE		m_eMoveState;				// �ƶ�״̬

    BOOL                        m_bSlip;
    int                         m_nJumpLimitFrame;

    int                         m_nVelocityXY;  // ˮƽ�ٶ�
    int                         m_nDirectionXY; // ˮƽ�ٶȷ���
    int                         m_nVelocityZ;   // ��ֱ�ٶ�

	int							m_nDestX;					
	int							m_nDestY;		

    // �����ٶ�,������Runʱ,˲ʱ�ٶȻ��ܵ���,������Ӱ����仯,�����ٶ����������ֵ
    int                         m_nConvergenceSpeed;

    int                         m_nRunSpeed;
    int                         m_nRunSpeedBase;           
    int                         m_nMoveSpeedPercent;    

    int			                m_nCurrentGravity;		
	int			                m_nGravity;			       
    int			                m_nGravityBase;
    int			                m_nGravityPercent;

	int							m_nJumpCount;				// ��Ծ������
	int							m_nMaxJumpCount;		    // �����Ծ����
    int                         m_nDropDefence;             // ��ˤ��ϵ��, [0 - 1024]

    int                         m_nJumpSpeed;               // ��Ծ�Ĵ�ֱ���ٶ�
    int                         m_nJumpSpeedBase;
    int                         m_nJumpSpeedPercent;

    int                         m_nDivingCount;
    int                         m_nDivingFrame;             // ���Ǳˮʱ��
    int                         m_nDivingFrameBase;
    int                         m_nDivingFramePercent; 

    int                         m_nWaterFlyAbility;         // �Ƿ����ˮ��Ư

    BOOL                        m_bTryPathState;            // �Ƿ�������·����֮��
    int                         m_nTryPathSide;             // ����������������·
    int                         m_nTryPathDestX;            // ��·��ʼ��ʱ����ƶ�Ŀ���X
    int                         m_nTryPathDestY;            // ��·��ʼ��ʱ����ƶ�Ŀ���Y

#ifdef _SERVER
    // ��һ�NPC�ͷ�ĳЩҪ���Զ�����ļ���ʱ���˱����Ϊtrue��
    // ����������ı䳯��ʱ���˱����Ϊfalse��ȡ���Զ�����
    BOOL                        m_bAutoTurnFlag;
#endif

    // �Զ��������
    int m_nCurrentTrack;    // ��ǰ�ƶ��켣��
    int m_nFromNode;        // ��ʼ���
    int m_nTargetCity;      // Ŀ�ĳ���

	DECLARE_LUA_ENUM(MoveState);

    DECLARE_LUA_INTEGER(VelocityXY);
    DECLARE_LUA_INTEGER(DirectionXY);
    DECLARE_LUA_INTEGER(VelocityZ);

	DECLARE_LUA_INTEGER(DestX);
	DECLARE_LUA_INTEGER(DestY);

    DECLARE_LUA_INTEGER(RunSpeed);
	DECLARE_LUA_INTEGER(RunSpeedBase);
	DECLARE_LUA_INTEGER(MoveSpeedPercent);
	
    DECLARE_LUA_INTEGER(MaxJumpCount);
    DECLARE_LUA_INTEGER(Gravity);
    DECLARE_LUA_INTEGER(GravityBase);
    DECLARE_LUA_INTEGER(GravityPercent);

    DECLARE_LUA_INTEGER(JumpSpeed);
    DECLARE_LUA_INTEGER(JumpSpeedBase);
    DECLARE_LUA_INTEGER(JumpSpeedPercent);

    DECLARE_LUA_INTEGER(DropDefence);

    DECLARE_LUA_INTEGER(DivingCount);         
    DECLARE_LUA_INTEGER(DivingFrame);         
    DECLARE_LUA_INTEGER(DivingFrameBase);
    DECLARE_LUA_INTEGER(DivingFramePercent); 

    DECLARE_LUA_INTEGER(WaterFlyAbility);

	// -------------- Ǳ�� ------------------------------------------------------>

	int							m_nCurrentStrength;			    // ��������ҪӰ��������⹦�˺�
	int                         m_nStrengthBase;
    int                         m_nStrengthBasePercentAdd;
    
    int							m_nCurrentAgility;				// ������ҪӰ��������ж��ٶȣ������ٶȣ������ʣ������ʡ�
	int                         m_nAgilityBase;
    int                         m_nAgilityBasePercentAdd;
	
    int							m_nCurrentVitality;				// ���ʣ���ҪӰ�����������ֵ��
	int                         m_nVitalityBase;
    int                         m_nVitalityBasePercentAdd;

    int							m_nCurrentSpirit;				// ���ǣ���ҪӰ�����������ֵ���ڹ�ϵ�˺���
	int                         m_nSpiritBase;
    int                         m_nSpiritBasePercentAdd;
    
    int                         m_nCurrentSpunk;                // ����: ��ҪӰ������������ָ��ٶȺ������ָ��ٶ�.
    int                         m_nSpunkBase;
    int                         m_nSpunkBasePercentAdd;
    
	DECLARE_LUA_INTEGER(CurrentStrength);
    DECLARE_LUA_INTEGER(StrengthBase);
    DECLARE_LUA_INTEGER(StrengthBasePercentAdd);

	DECLARE_LUA_INTEGER(CurrentAgility);
    DECLARE_LUA_INTEGER(AgilityBase);
    DECLARE_LUA_INTEGER(AgilityBasePercentAdd);

    DECLARE_LUA_INTEGER(CurrentVitality);
    DECLARE_LUA_INTEGER(VitalityBase);
    DECLARE_LUA_INTEGER(VitalityBasePercentAdd);
	
    DECLARE_LUA_INTEGER(CurrentSpirit);
    DECLARE_LUA_INTEGER(SpiritBase);
    DECLARE_LUA_INTEGER(SpiritBasePercentAdd);

    DECLARE_LUA_INTEGER(CurrentSpunk);
    DECLARE_LUA_INTEGER(SpunkBase);
    DECLARE_LUA_INTEGER(SpunkBasePercentAdd);

	// --------------- ���� ------------------------------------------------------>

    // m_nMaxLife = m_nMaxLifeBase * (1024 + m_nMaxLifePercentAdd) / 1024 + m_nMaxLiftAdditional;

    // ��ս��״̬��Ѫ���㹫ʽ��
    // m_nLifeReplenish + (m_nLifeReplenish * m_nLifeReplenishCoefficient) + (m_nMaxLife * m_nLifeReplenishPercent) + m_nLifeReplenishExt
    //
    // ս��״̬��Ѫ���㹫ʽ��
    // (m_nLifeReplenish * m_nLifeReplenishCoefficient) + (m_nMaxLife * m_nLifeReplenishPercent) + m_nLifeReplenishExt

    int							m_nCurrentLife;				// ��ǰ����
	int							m_nMaxLife;					// �������ֵ    
    int                         m_nMaxLifeBase;             // ��ȡ����ְҵ,�ȼ�
	int							m_nMaxLifePercentAdd;	    // �������ֵ���Ӱٷֱ�
	int							m_nMaxLifeAdditional;		// ׷�ӵ��������ֵ
	int							m_nLifeReplenish;			// ���������ָ��ٶ� = m_nSpunkLifeReplenish + ����(װ����BUFF�ӳɵ�)
	int							m_nLifeReplenishPercent;	// �����ٷֱȻָ��ٶ�, ��λ1/1024
    int                         m_nLifeReplenishCoefficient;// ���������ָ��ٶȵ�ϵ������λ1/1024
    int                         m_nLifeReplenishExt;        // ���������ָ�����

	DECLARE_LUA_INTEGER(CurrentLife);
	DECLARE_LUA_INTEGER(MaxLife);
    DECLARE_LUA_INTEGER(MaxLifeBase);
	DECLARE_LUA_INTEGER(MaxLifePercentAdd);
	DECLARE_LUA_INTEGER(MaxLifeAdditional);
	DECLARE_LUA_INTEGER(LifeReplenish);
	DECLARE_LUA_INTEGER(LifeReplenishPercent);
    DECLARE_LUA_INTEGER(LifeReplenishCoefficient);
    DECLARE_LUA_INTEGER(LifeReplenishExt);
	

    // ---------------- ���� ----------------------------------------------------->

	int							m_nCurrentMana;				
	int							m_nMaxMana;					
    int                         m_nMaxManaBase;             
	int							m_nMaxManaPercentAdd;		
	int							m_nMaxManaAdditional;		
	int							m_nManaReplenish;        // �������� = m_nSpunkManareplenish + ����(װ����BUFF�ӳɵ�)
	int							m_nManaReplenishPercent;	
    int                         m_nManaReplenishCoefficient;
    int                         m_nManaReplenishExt;

	DECLARE_LUA_INTEGER(CurrentMana);
	DECLARE_LUA_INTEGER(MaxMana);
    DECLARE_LUA_INTEGER(MaxManaBase);
	DECLARE_LUA_INTEGER(MaxManaPercentAdd);
	DECLARE_LUA_INTEGER(MaxManaAdditional);
	DECLARE_LUA_INTEGER(ManaReplenish);
	DECLARE_LUA_INTEGER(ManaReplenishPercent);
    DECLARE_LUA_INTEGER(ManaReplenishCoefficient);
    DECLARE_LUA_INTEGER(ManaReplenishExt);

    // --------------- ս������ --------------------------------------------------->
	int							m_nDodge;			        // ���ܵ��� = m_Dodege
    int                         m_nDodgeBaseRate;
    BOOL                        m_bFightState;
    BOOL                        m_bSheathFlag;              // ����λ�ñ��,true��ʾ��������,false��ʾ������
    BOOL                        m_bSystemShield;            // ϵͳ�޵ж�
    int                         m_nPositiveShield;          // ����Ч������,������������Ч��(���Ѫ,Buff)����
    int                         m_nNegativeShield;          // ����Ч������,�������и���Ч��(���˺�, Debuff)����
    int                         m_nGlobalBlock;             // �����͸�
    int                         m_nGlobalDamageAbsorb;      // ���������Ƶ������˺�
	int							m_nGlobalDamageManaShield;  // ���������Ƶ����������˺�
    int                         m_nDamageToLifeForSelf;     // ���˺�תΪ�Լ�������
    int                         m_nDamageToManaForSelf;     // ���˺�תΪ�Լ�������
    int                         m_nBeatBackRate;
    int							m_nStunStrikeRate;		    // ������
    int                         m_nKnockedBackRate;         // ��������
    int							m_nKnockedOffRate;		    // ��������

    BOOL                        m_bImmortal;                // ������ȥ

    int                         m_nMountingRate;            // ��������ĵֿ�����

    int                         m_nAccumulateValue;         // ����ֵ

    int                         m_nParryBaseRate;
    int                         m_nParry;                   // �мܵ���
    int                         m_nParryBase;
    int                         m_nParryPercent;
    int                         m_nParryValue;              // �м�ֵ�������мܼ�����˺�
    int                         m_nParryValueBase;
    int                         m_nParryValuePercent;
    int                         m_nSense;                   // ����ֵ ��NPC��ʶ���й�
    int                         m_nStrain;                  // Ӧ��ֵ ��Player�ֿ�Npc��ʶ���й�
    int                         m_nStrainBase;
    int                         m_nStrainPercent;
    int                         m_nToughnessBaseRate;
    int                         m_nToughness;               // ����ֵ �������ٱ����ĵļ���
    int                         m_nToughnessBase;
    int                         m_nToughnessPercent;

    int                         m_nCriticalDamagePowerBaseKiloNumRate;
    int                         m_nCriticalDamagePower;     // ���ӻ����˺�
    int                         m_nCriticalDamagePowerBase;
    int                         m_nCriticalDamagePowerPercent;
    int                         m_nDecriticalDamagePowerBaseKiloNumRate;
    int                         m_nDecriticalDamagePower;   // ���ٻ����˺�
    int                         m_nDecriticalDamagePowerBase;
    int                         m_nDecriticalDamagePowerPercent;

    int                         m_nTransferDamageToManaPercent; //�˺�תΪ����

    int                         m_nModifyCostManaPercent;   // ���＼�ܵ���������

    DWORD                       m_dwAddDamageByDstMoveStateMask;    // ����Ŀ����ƶ�״̬�����˺�������
    int                         m_nAddDamagePercentByDstMoveState;  // ����Ŀ����ƶ�״̬�����˺��İٷֱ�

    DWORD                       m_dwTransferDamageToCharacter;
    int                         m_nTransferDamagePercent;

    DWORD                       m_dwTransferTherapyToCharacter;
    int                         m_nTransferTherapyPercent;

    // -->[����]
#ifdef _SERVER
    int                         m_nUpperWeak;               // �϶�����
    int                         m_nMiddleWeak;              // �ж�����
    int                         m_nLowerWeak;               // �¶�����
#endif

    int                         m_nUpperWeakShield;         // �϶�������
    int                         m_nMiddleWeakShield;        // �ж�������
    int                         m_nLowerWeakShield;         // �¶�������

    int                         m_nUpperWeakLevel;         // ��������0���ͣ��У��ߣ�
    int                         m_nMiddleWeakLevel;
    int                         m_nLowerWeakLevel;

    DECLARE_LUA_INTEGER(UpperWeakLevel);
    DECLARE_LUA_INTEGER(MiddleWeakLevel);
    DECLARE_LUA_INTEGER(LowerWeakLevel);

    // [����]<--

    // ͵ȡ�з������ٷֱ�: ָ�Եз�����˺����˺��еĶ��ٱ���ת��Ϊ�Լ�������,��λ(1/1024)
    // ͵ȡ�з������ٷֱ�: �������A��B����,A�ܵ������˺�1000��,��ʱ,A����͵ȡB������(1000 * m_nStealManaPercent / 1024)

	DECLARE_LUA_INTEGER(Dodge);
    DECLARE_LUA_INTEGER(DodgeBaseRate);
    DECLARE_LUA_BOOL(FightState);
    DECLARE_LUA_BOOL(SheathFlag);
    DECLARE_LUA_INTEGER(PositiveShield);
    DECLARE_LUA_INTEGER(NegativeShield);
    DECLARE_LUA_INTEGER(GlobalBlock);
    DECLARE_LUA_INTEGER(GlobalDamageAbsorb);
	DECLARE_LUA_INTEGER(GlobalDamageManaShield);
    DECLARE_LUA_INTEGER(DamageToLifeForSelf);
    DECLARE_LUA_INTEGER(DamageToManaForSelf);
    DECLARE_LUA_INTEGER(BeatBackRate);
    DECLARE_LUA_INTEGER(StunStrikeRate);
    DECLARE_LUA_INTEGER(KnockedBackRate);
    DECLARE_LUA_INTEGER(KnockedOffRate);
    
    DECLARE_LUA_INTEGER(ParryBaseRate);
    DECLARE_LUA_INTEGER(Parry);
    DECLARE_LUA_INTEGER(Sense);
    DECLARE_LUA_INTEGER(Strain);
    DECLARE_LUA_INTEGER(ToughnessBaseRate);
    DECLARE_LUA_INTEGER(Toughness);
    DECLARE_LUA_INTEGER(CriticalDamagePowerBaseKiloNumRate);
    DECLARE_LUA_INTEGER(CriticalDamagePower);
    DECLARE_LUA_INTEGER(DecriticalDamagePowerBaseKiloNumRate);
    DECLARE_LUA_INTEGER(DecriticalDamagePower);
    DECLARE_LUA_INTEGER(ParryValue);

    DECLARE_LUA_INTEGER(MountingRate);
    DECLARE_LUA_INTEGER(AccumulateValue);

    // -------------- �⹦ ------------------------------------------------------>
	int m_nMeleeWeaponDamageBase;
	int m_nMeleeWeaponDamageRand;

	DECLARE_LUA_INTEGER(MeleeWeaponDamageBase);
	DECLARE_LUA_INTEGER(MeleeWeaponDamageRand);

    int m_nRangeWeaponDamageBase;
    int m_nRangeWeaponDamageRand;

	DECLARE_LUA_INTEGER(RangeWeaponDamageBase);
	DECLARE_LUA_INTEGER(RangeWeaponDamageRand);

    int m_nSkillPhysicsDamage;
    int m_nSkillPhysicsDamageRand;
    int m_nSkillPhysicsDamagePercent;
    int m_nPhysicsDamageCoefficient;     // �⹦�˺��ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)

    int m_nPhysicsAttackPower;
    int m_nPhysicsAttackPowerBase;
    int m_nPhysicsAttackPowerPercent;

    int m_nPhysicsHitBaseRate;
    int m_nPhysicsHitValue;
    //int m_nAgilityCriticalStrike;        // ��Ӱ����⹦����һ��ֵ
    int m_nPhysicsCriticalStrikeBaseRate;
    int m_nPhysicsCriticalStrike;        // �⹦����һ��ֵ = m_nAgilityCriticalStrike + ����(װ����BUFF�ӳɵ�)
    int m_nPhysicsCriticalStrikeBase;

    int	m_nPhysicsResistPercent;	

    // �⹦����,�������Ŀ����⹦����
    int m_nPhysicsOvercome;
    int m_nPhysicsOvercomeBase;
    int m_nPhysicsOvercomePercent;
             
    // �⹦����
    // int m_nPhysicsDefence;
    int	m_nPhysicsDefenceAdd;       
	int	m_nPhysicsDefenceMax;	

	// �⹦������, ���ڼ���"m_nPhysicsDefence"
	int	m_nPhysicsShield; 
    int	m_nPhysicsShieldBase;      
    int	m_nPhysicsShieldPercent;
    int	m_nPhysicsShieldAdditional;	

    // �⹦����,��ֵ��ֱ�ӷ����������ߵ��˺�����,���ǲ���������Լ��ܵ����˺���ֵ
    // �⹦�����ٷֱ�,��ֵ������Լ������˺��������˺��������Է�, ��λ(1/1024),��ֵ [0, 1024]

	int	m_nPhysicsReflection;		
	int	m_nPhysicsReflectionPercent;

    int m_nPhysicsBlock;

    int m_nPhysicsDamageAbsorb;    
	int	m_nPhysicsDamageManaShield; 

	DECLARE_LUA_INTEGER(PhysicsAttackPower);
    DECLARE_LUA_INTEGER(SkillPhysicsDamageRand);
    DECLARE_LUA_INTEGER(PhysicsHitValue);
    DECLARE_LUA_INTEGER(PhysicsHitBaseRate);
    DECLARE_LUA_INTEGER(PhysicsCriticalStrike);
    DECLARE_LUA_INTEGER(PhysicsCriticalStrikeBaseRate);
    DECLARE_LUA_INTEGER(PhysicsResistPercent);
    DECLARE_LUA_INTEGER(PhysicsOvercome);
    DECLARE_LUA_INTEGER(PhysicsShield);
    DECLARE_LUA_INTEGER(PhysicsReflection);
    DECLARE_LUA_INTEGER(PhysicsReflectionPercent);
    DECLARE_LUA_INTEGER(PhysicsBlock);
    DECLARE_LUA_INTEGER(PhysicsDamageAbsorb);
   	DECLARE_LUA_INTEGER(PhysicsDamageManaShield);

    // -------------- �����ڹ� ------------------------------------------------------>

    int m_nSkillSolarDamage;
    int m_nSkillSolarDamageRand;
    int m_nSkillSolarDamagePercent;
    int m_nSolarDamageCoefficient;      // �����ڹ��˺��ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)

    int m_nSolarAttackPower;
    int m_nSolarAttackPowerBase;
    int m_nSolarAttackPowerPercent;

    int m_nSolarHitBaseRate;
    int m_nSolarHitValue;
    int m_nSolarCriticalStrikeBaseRate;
    int m_nSolarCriticalStrike;
    int m_nSolarCriticalStrikeBase;

    int m_nSolarOvercome;
    int m_nSolarOvercomeBase;
    int m_nSolarOvercomePercent;

	int	m_nSolarMagicResistPercent;	    // �ֿ��ٷֱ�

	int m_nSolarMagicShield;			// ������,�����ֵ���ջ���������÷���ϵ��
    int m_nSolarMagicShieldBase;
    int m_nSolarMagicShieldPercent;

	int	m_nSolarMagicReflection;
	int	m_nSolarMagicReflectionPercent;

    int m_nSolarMagicBlock;

    int m_nSolarDamageAbsorb;
	int	m_nSolarDamageManaShield;

    DECLARE_LUA_INTEGER(SolarAttackPower);
    DECLARE_LUA_INTEGER(SolarHitValue);
    DECLARE_LUA_INTEGER(SolarHitBaseRate);
    DECLARE_LUA_INTEGER(SolarCriticalStrike);
    DECLARE_LUA_INTEGER(SolarCriticalStrikeBaseRate);
    DECLARE_LUA_INTEGER(SolarOvercome);
    DECLARE_LUA_INTEGER(SolarMagicShield);
    DECLARE_LUA_INTEGER(SolarMagicResistPercent);
    DECLARE_LUA_INTEGER(SolarMagicReflection);		
    DECLARE_LUA_INTEGER(SolarMagicReflectionPercent);
    DECLARE_LUA_INTEGER(SolarMagicBlock);
    DECLARE_LUA_INTEGER(SolarDamageAbsorb);     
    DECLARE_LUA_INTEGER(SolarDamageManaShield); 

    // -------------- �����ڹ� ------------------------------------------------------>
    int m_nSkillNeutralDamage;
    int m_nSkillNeutralDamageRand;
    int m_nSkillNeutralDamagePercent;	
    int m_nNeutralDamageCoefficient;        // �����ڹ��˺��ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)        


    int m_nNeutralAttackPower;
    int m_nNeutralAttackPowerBase;
    int m_nNeutralAttackPowerPercent;

    int m_nNeutralHitBaseRate;
    int m_nNeutralHitValue;               
    int m_nNeutralCriticalStrikeBaseRate;
    int m_nNeutralCriticalStrike;
    int m_nNeutralCriticalStrikeBase;

    int m_nNeutralOvercome;
    int m_nNeutralOvercomeBase;
    int m_nNeutralOvercomePercent;

    int	m_nNeutralMagicResistPercent;	    // �ֿ��ٷֱ�

    int m_nNeutralMagicShield;			    // ������,�����ֵ���ջ���������÷���ϵ��
    int m_nNeutralMagicShieldBase;
    int m_nNeutralMagicShieldPercent;

    int	m_nNeutralMagicReflection;		
    int	m_nNeutralMagicReflectionPercent;	

    int m_nNeutralMagicBlock;             

    int m_nNeutralDamageAbsorb;     
    int	m_nNeutralDamageManaShield; 

    DECLARE_LUA_INTEGER(NeutralAttackPower);
    DECLARE_LUA_INTEGER(NeutralHitValue);
    DECLARE_LUA_INTEGER(NeutralHitBaseRate);
    DECLARE_LUA_INTEGER(NeutralCriticalStrike);
    DECLARE_LUA_INTEGER(NeutralCriticalStrikeBaseRate);
    DECLARE_LUA_INTEGER(NeutralOvercome);
    DECLARE_LUA_INTEGER(NeutralMagicShield);
    DECLARE_LUA_INTEGER(NeutralMagicResistPercent);
    DECLARE_LUA_INTEGER(NeutralMagicReflection);		
    DECLARE_LUA_INTEGER(NeutralMagicReflectionPercent);	
    DECLARE_LUA_INTEGER(NeutralMagicBlock);
    DECLARE_LUA_INTEGER(NeutralDamageAbsorb);     
    DECLARE_LUA_INTEGER(NeutralDamageManaShield); 

    // -------------- �����ڹ� ------------------------------------------------------>
    int m_nSkillLunarDamage;
    int m_nSkillLunarDamageRand;
    int m_nSkillLunarDamagePercent;	
    int m_nLunarDamageCoefficient;          // �����ڹ��˺��ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)

    int m_nLunarAttackPower;
    int m_nLunarAttackPowerBase;
    int m_nLunarAttackPowerPercent;

    int m_nLunarHitBaseRate;               
    int m_nLunarHitValue;               
    int m_nLunarCriticalStrikeBaseRate;         
    int m_nLunarCriticalStrike;         
    int m_nLunarCriticalStrikeBase;

    int m_nLunarOvercome;                   // ����
    int m_nLunarOvercomeBase;
    int m_nLunarOvercomePercent;

    int	m_nLunarMagicResistPercent;	        // �ֿ��ٷֱ�

    int m_nLunarMagicShield;			    // ������,�����ֵ���ջ���������÷���ϵ��
    int m_nLunarMagicShieldBase;
    int m_nLunarMagicShieldPercent;

    int	m_nLunarMagicReflection;		
    int	m_nLunarMagicReflectionPercent;	

    int m_nLunarMagicBlock;             

    int m_nLunarDamageAbsorb;     
    int	m_nLunarDamageManaShield; 

    DECLARE_LUA_INTEGER(LunarAttackPower);
    DECLARE_LUA_INTEGER(LunarHitValue);
    DECLARE_LUA_INTEGER(LunarHitBaseRate);
    DECLARE_LUA_INTEGER(LunarCriticalStrike);
    DECLARE_LUA_INTEGER(LunarCriticalStrikeBaseRate);
    DECLARE_LUA_INTEGER(LunarOvercome);
    DECLARE_LUA_INTEGER(LunarMagicShield);
    DECLARE_LUA_INTEGER(LunarMagicResistPercent);
    DECLARE_LUA_INTEGER(LunarMagicReflection);		
    DECLARE_LUA_INTEGER(LunarMagicReflectionPercent);
    DECLARE_LUA_INTEGER(LunarMagicBlock);
    DECLARE_LUA_INTEGER(LunarDamageAbsorb);     
    DECLARE_LUA_INTEGER(LunarDamageManaShield); 

    // -------------- ���� ---------------------------------------------------------->

    int m_nSkillPoisonDamage;
    int m_nSkillPoisonDamageRand;
    int m_nSkillPoisonDamagePercent;	
    int m_nPoisonDamageCoefficient;         // �����ڹ��˺��ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)

    int m_nPoisonAttackPower;
    int m_nPoisonAttackPowerBase;
    int m_nPoisonAttackPowerPercent;

    int m_nPoisonHitBaseRate;
    int m_nPoisonHitValue;
    int m_nPoisonCriticalStrikeBaseRate;         
    int m_nPoisonCriticalStrike;         
    int m_nPoisonCriticalStrikeBase;         

    int m_nPoisonOvercome;                  // ����
    int m_nPoisonOvercomeBase;
    int m_nPoisonOvercomePercent;

    int	m_nPoisonMagicResistPercent;	    // �ֿ��ٷֱ�

    int m_nPoisonMagicShield;			    // ������,�����ֵ���ջ���������÷���ϵ��
    int m_nPoisonMagicShieldBase;
    int m_nPoisonMagicShieldPercent;

    int	m_nPoisonMagicReflection;		
    int	m_nPoisonMagicReflectionPercent;	

    int m_nPoisonMagicBlock;             

    int m_nPoisonDamageAbsorb;     
    int	m_nPoisonDamageManaShield; 

    DECLARE_LUA_INTEGER(PoisonAttackPower);
    DECLARE_LUA_INTEGER(PoisonHitValue);
    DECLARE_LUA_INTEGER(PoisonHitBaseRate);
    DECLARE_LUA_INTEGER(PoisonCriticalStrike);
    DECLARE_LUA_INTEGER(PoisonCriticalStrikeBaseRate);
    DECLARE_LUA_INTEGER(PoisonOvercome);
    DECLARE_LUA_INTEGER(PoisonMagicShield);
    DECLARE_LUA_INTEGER(PoisonMagicResistPercent);
    DECLARE_LUA_INTEGER(PoisonMagicReflection);		
    DECLARE_LUA_INTEGER(PoisonMagicReflectionPercent);	
    DECLARE_LUA_INTEGER(PoisonMagicBlock);
    DECLARE_LUA_INTEGER(PoisonDamageAbsorb);     
    DECLARE_LUA_INTEGER(PoisonDamageManaShield); 

    // ---------------------------- ����(�����ظ�) -------------------------------------------------
    int m_nTherapyPower;
    int m_nTherapyPowerBase;            // ������������
    int m_nTherapyPowerPercent;
    int m_nSkillTherapy; 		        // ������ص�����Ч��
    int m_nSkillTherapyRand;            
    int m_nSkillTherapyPercent; 
    int m_nTherapyCoefficient;          // ����Ч���ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)

    int m_nBeTherapyCoefficient;        // �����Ƽӳ�ϵ��

    DECLARE_LUA_INTEGER(TherapyPower);
    DECLARE_LUA_INTEGER(SkillTherapy);

    // --------------------------- ���ּ����й� ----------------------------------------------------
    
    int                         m_nNoneWeaponAttackSpeedBase;           // ����ʱȭͷ�Ĺ����ٶ�
    // ����������û��װ����������ʱ,����m_nNoneWeaponAttackSpeedBase���m_nMeleeWeaponAttackSpeedBase������
    // m_nMeleeWeaponAttackSpeed��ֵ.
    int                         m_nMeleeWeaponAttackSpeed;              // ��ս�����ٶ�, ��λ ֡/����
    int                         m_nMeleeWeaponAttackSpeedBase;          // ��ս�����ٶȻ���, ��λ ����/(1024*֡),������ֵΪ512,�����˵ 512/1024 = 0.5��/֡ 
    int                         m_nMeleeWeaponAttackSpeedPercentAdd;
    int                         m_nMeleeWeaponAttackSpeedAdditional;
    
    int                         m_nRangeWeaponAttackSpeed;              // Զ�̹����ٶ�, ��λ ֡/����
    int                         m_nRangeWeaponAttackSpeedBase;          // Զ�̹����ٶȻ���, ��λ ����/֡
    int                         m_nRangeWeaponAttackSpeedPercentAdd;
    int                         m_nRangeWeaponAttackSpeedAdditional;

    DECLARE_LUA_INTEGER(NoneWeaponAttackSpeedBase);
    DECLARE_LUA_INTEGER(MeleeWeaponAttackSpeed);
    DECLARE_LUA_INTEGER(MeleeWeaponAttackSpeedBase);
    DECLARE_LUA_INTEGER(MeleeWeaponAttackSpeedPercentAdd);
    DECLARE_LUA_INTEGER(MeleeWeaponAttackSpeedAdditional);

    DECLARE_LUA_INTEGER(RangeWeaponAttackSpeed);
    DECLARE_LUA_INTEGER(RangeWeaponAttackSpeedBase);
    DECLARE_LUA_INTEGER(RangeWeaponAttackSpeedPercentAdd);
    DECLARE_LUA_INTEGER(RangeWeaponAttackSpeedAdditional);
    // ---------------------------------------------------------------------------------------------

    int                         m_nIgnoreOtherAlarmRange;           // ��������(NPC)���䷶Χ
    int                         m_nSelfAlarmRangeRevise;            // �������䷶Χ������; һ������NPC
    int                         m_nOtherAlarmRangeRevise;           // �����˾��䷶Χ������ 
    int                         m_nResistBehaveConstraintPercent;   // ��ɫ����������Ϊ�ܿ�״̬�ĸ���, ����Ϊ1024
    int                         m_nUnarmCounter;

    // -------------------------------- ��ħ��� ---------------------------------------------------
    char                        m_nSilenceRefCounter[sktTotal - 1]; // ���ݼ��ܵ� KindType ��ħ��

    // -------------------------------- ������� ---------------------------------------------------
    std::bitset<sftTotal>       m_ImmunityFlag;// ����BUFF�� FunctionType ���ߡ�

    // ------------------------------ ����������־ -------------------------------------------------
    DWORD                       m_dwSkillCastFlag;

    // --------------- ��¼Ψһ���õ�BUFF��ʱ���õĽ�ɫID ------------------------------------
    DWORD                       m_dwBuffEffectSoleTargetID;

public:
    // m_dwRepresentState�����˽�ɫ���ϵ�ħ����Ч״̬(��ʵ��������ЧID�����), һ������������Ч��:
    // 1. �ŵ��²ȵ�(���ҵ���0-4λ).
    // 2. ���ϲ��Ƶ�(���ҵ���5-9λ)
    // 3. ͷ�϶��ŵ�(���ҵ���10-14λ)
	DWORD   					m_dwRepresentState;

    BOOL    GetRepresentState(KBUFF_REPRESENT_POS nPos, DWORD&   rdwRepresentID);

public:

#ifdef _CLIENT
    KThreatList				m_ThreatList;				        // ��ޱ�
#endif  //_CLIENT

#ifdef _SERVER
    KSimpThreatList         m_SimpThreatList;

	int						m_nThreatTime;				        // ������˼��

    int                     m_nActiveBaseThreatCoefficient;     // ���������Ļ������ϵ��
    int                     m_nActiveDamageThreatCoefficient;   // �����������˺����ϵ��
    int                     m_nActiveTherapyThreatCoefficient;  // �������������Ƴ��ϵ��
    int                     m_nPassiveBaseThreatCoefficient;    // ���������Ļ������ϵ��
    int                     m_nPassiveDamageThreatCoefficient;  // �����������˺����ϵ��
    int                     m_nPassiveTherapyThreatCoefficient; // �������������Ƴ��ϵ��

	int						m_nOverThreatPercent;
    BOOL                    m_bToDie;
    DWORD                   m_dwKillerID;         
    BOOL ModifyThreat(THREAT_TYPE eType, int nThreatValue);       // ���ڼ���ֱ���޸ĶԷ����ֵ

    int  AdjustThreat(THREAT_TYPE eTheatType, int nThreatValue);                           
#endif	//_SERVER

    // ��AI���
public:
    struct KCharacterAIData 
    {
        int             nAIType;

        int             nOriginPosX;
        int             nOriginPosY;
        int             nOriginPosZ;
        int             nOriginDirection;

        int             nReturnPosX;
        int             nReturnPosY;
        int             nReturnPosZ;

        int             nPatrolPathID;
        KPatrolPath*    pPatrolPath;
        int             nPatrolOrderIndex;

        int             nEscapeCount;

        static DWORD    dwAIEventSrc;
        static int      nAIEventParam;

        int             nCannotAttactCount;
        int             nSkillFailedCount;
        int             nSkillType;
        DWORD           dwRespondCharacterID; // ��¼�����ߵ�ID
    }   m_AIData;
    // DECLARE_LUA_DWORD(AIEventSrc);
    DWORD getAIEventSrc(void)   {return m_AIData.dwAIEventSrc;};
    // DECLARE_LUA_INTEGER(AIEventParam);
    DWORD getAIEventParam(void) {return m_AIData.nAIEventParam;};
    
public:
#ifdef _CLIENT
    BOOL PrepareSkill(DWORD dwSkillID, DWORD dwSkillLevel, KTarget& rTarget, int nPrepareFrame, int nEndFrame);
#endif

#ifdef _SERVER
    SKILL_RESULT_CODE CastSkill(DWORD dwSkillID, DWORD dwSkillLevel, KTarget& rTarget);
    SKILL_RESULT_CODE CastSkillSub(KCharacter* pLogicCaster, DWORD dwSkillID, DWORD dwSkillLevel, KTarget& rTarget);
    SKILL_RESULT_CODE CastSkillAccumulate(DWORD dwSkillID, DWORD dwSkillLevel, KTarget& rTarget);
#else
    SKILL_RESULT_CODE CastSkill(DWORD dwSkillID, DWORD dwSkillLevel, KTarget& rTarget, DWORD dwBulletID);
    SKILL_RESULT_CODE CastSkillSub(KCharacter* pLogicCaster, DWORD dwSkillID, DWORD dwSkillLevel, KTarget& rTarget, DWORD dwBulletID);
#endif
    BOOL ProcessCommonSkillActive(const KSKILL_RECIPE_KEY& crSkillRecipeKey);

    inline int GetWalkSpeed()
    {
        if (IS_PLAYER(m_dwID))
        {
            return m_nRunSpeed * 6 / 20;
        }
        return m_nRunSpeed * 6 / 28;
    }

    // ǿ��ֹͣ״̬(��Stand),����ԭ����ʲô״̬
	BOOL Stop(void);			
    // �ú��������������귶Χ
    BOOL EmendateDestCoordinate(int& nDestX, int& nDestY); 

    BOOL Stand(BOOL bSyncSelf);
	BOOL WalkTo(int nDestX, int nDestY, BOOL bBroadcast, int nWalkSpeed = 0);   // ��WalkSpeed����0ʱ��ȡCharacter���ϵ���·�ٶ�
	BOOL RunTo(int nDestX, int nDestY, BOOL bBroadcast, int nRunSpeed = 0);   // ��RunSpeed����0ʱ��ȡCharacter���ϵ��ܲ��ٶ�
    BOOL SwimTo(int nDestX, int nDestY, BOOL bBroadcast);			
	BOOL Jump(BOOL bStandJump, int nJumpDirection, BOOL bSyncSelf);
	BOOL SitDown(BOOL bSyncSelf);
    BOOL AutoFly(int nFromNode, int nToCity);

	BOOL Dash(int nFrame, int nDirectionXY, int nVelocityXY, int nVelocityZ);
    BOOL Pull(int nFrame, int nDirectionXY, int nVelocityXY, int nVelocityZ);
    BOOL Repulsed(int nFrame, int nDirectionXY, int nVelocityXY);
	BOOL KnockedDown(int nFrame);
	BOOL KnockedBack(int nDirection, int nDistance, int nVelocityXY);
	BOOL KnockedOff(int nDirection, int nDistance, int nVelocityXY);
    BOOL Halt();                                                // ѣ��
    BOOL Freeze();                                              // ����
    BOOL Entrap();

	BOOL CheckDie();								            // ����

	BOOL Action(DWORD dwCharacterID, DWORD dwActionType);		// ����

	BOOL TurnTo(int nNewDirection, BOOL bBroadcast);
    BOOL PitchTo(int nNewDirection, BOOL bBroadcast);

    BOOL MoveTo(int nX, int nY, int nZ, BOOL bIgnoreBlock, BOOL bBroadcast);

    BOOL GoToHell(DWORD dwKiller = ERROR_ID);

	const char* GetName();

protected:
	BOOL						m_bOnTalk;					// �Ƿ����ں�NPC�Ի�
   
public:
    int                         m_nFearCounter;             // �־������
	int							m_nMoveFrameCounter;

public:
    BOOL SelectTarget(TARGET_TYPE eTargetType, DWORD dwTargetID);

    KTarget                     m_SelectTarget;
    KTarget                     m_SkillTarget;

public:
    BOOL CheckFear();

    // ״̬������
    BOOL OnStand(){ return true; }
    BOOL OnWalk(){ return true; }
    BOOL OnRun(){ return true; }
    BOOL OnJump(){ return true; }
    BOOL OnSwimJump(){ return true; }
    BOOL OnSit(){ return true; }
    BOOL OnPassiveMove();
    BOOL OnDash();
    BOOL OnPull();
    BOOL OnRepulsed();
    BOOL OnHalt(){ return true; }
    BOOL OnFreeze(){ return true; }
    BOOL OnEntrap(){ return true; }
    BOOL OnDeath();

    BOOL OnRise();
    BOOL OnSkid();

	void SetMoveState(int nState);

public:

    // ------------- ����һ��ʱ�����Ϊ ---------------------->
	BOOL DoOTAction(KOT_ACTION_PARAM& rParam);
	void DoActionIdle(void);
    void DoAutoCastSkill(KAUTO_CAST_SKILL_PARAM& param);
    void DoAutoCastSkillIdle(void);
    BOOL DoSkillPrepare(KSkillRecipePointer& rSkillRecipePointer, int nPrepareFrame, int nEndFrame);
	BOOL DoSkillChannel(KSkillRecipePointer& rSkillRecipePointer, int nChannelFrame);
    BOOL DoRecipePrepare(KProfessionSkill* pProfessionSkill, int nFrame);
	BOOL DoPickPrepare(KDoodad* pDoodad, int nFrame);
	BOOL DoPicking(KDoodad* pDoodad);
	BOOL DoCustomOTActionPrepare(BOOL bIsBreakable, int nFrame, KTarget& crTarget, DWORD dwScriptID);
	BOOL DoCustomOTActionChannel(BOOL bIsBreakable, int nFrame, KTarget& crTarget, DWORD dwScriptID);

    KOT_ACTION_PARAM            m_OTActionParam;
    KAUTO_CAST_SKILL_PARAM      m_AutoCastSkillParam;

    // -------------- �������޸ĵ����룬�ͼ������Ӧ -------------->
    DWORD   m_dwDirectCastMask;

    void ProcessOTAction();
    void ProcessAutoCastSkill();

#ifdef _SERVER
    void DoAutoTurn();                      // �����Զ�������룬����Action״̬������ɲ���
	void DoSitDown(KDoodad* pDoodad);		// ����һ��Doodad�ϣ��������ͬ�����ı��ƶ�״̬��Ҳ��Ӱ���Ѫ 
#endif

    BOOL OnSkillPrepare();
    BOOL OnSkillChannel();
    BOOL OnRecipePrepare();
    BOOL OnPickPrepare();
	BOOL OnPicking();

#ifdef _SERVER
	BOOL OnItemSkill();
#endif
    void OnUseItemPrepare();

	BOOL OnCustomOTActionPrepare();
	BOOL OnCustomOTActionChannel();

	BOOL OnBreakPickPrepare(KOT_ACTION_PARAM& rNewAction);
	BOOL OnBreakPicking(KOT_ACTION_PARAM& rNewAction);

    // <------------- ����һ��ʱ�����Ϊ ----------------------

public:
    BOOL UpdateMoveParameter();

    void InvalidateMoveState(int nDelayFrame, BOOL bMoveSyncSelf);

	void ProcessMove();

private:

    void ProcessAutoFly();

    int  GetFrictionDirection();
    BOOL ProcessAcceleration();      // ������ٶȼ���  

    void ProcessHorizontalMove();               // ˮƽ�ƶ�
    void ProcessHorizontalMoveStupid();
    void ProcessHorizontalMoveSmart(); 

    void ProcessVerticalMove();             // ��ֱ�ƶ�
    void ProcessDrowning();

	BOOL CellMoveStupid(int nStepLength);
    BOOL CellMoveSmart(int nStepLength);

	int  TryMove(int nStepLength, int nDirection, int* pnObstacleDirection);
    BOOL IgnoreDynamicObstacle();
    void ProcessDynamicObstacle();

    BOOL ProcessDropDamage();
    BOOL ProcessDropSpeed();

    BOOL CheckSlip();

    int  m_nMoveSyncFrame;
    BOOL m_bMoveSyncSelf;

public:
    // ��ȡ��ˮ���, ��ˮ����,����һ������0��ֵ��ʾ��ˮ���,���򷵻�0
    // ��ˮ���ָˮ����������µĸ߶Ȳ�
    int GetWaterline();          

    // ��ȡ�������۵��ƶ���Ը����
    int GetSubjectiveDirection();

    // ��ȡ�����ƶ��ٶ�
    int GetSubjectiveVelocity();

public:
	void ApplyAttribute(KAttribute* pAttr);
	void UnApplyAttribute(KAttribute* pAttr);

    BOOL CallAttributeFunction(int nKey, BOOL bApply, int nValue1, int nValue2);

    void UpdateWeaponAttackSpeed();
    void UpdateBaseState(); // Life & Mana
    void UpdateAllAttackPowerValue();
    void UpdateAllShieldValue();
    void UpdateAllCriticalStrikeValue();

	BOOL SetForceID(DWORD dwForceID);

    int  m_nNextReplenishFrame;
    void AttribReplenish();
    void ReviseAttribValue();
    void CheckFightFlag();

	virtual BOOL ChangeRegion(KRegion* pRegion) = 0;

	// �����ɫ��״̬��
	void RunStateMachine();

	friend class KAIController;
	friend class KAIBase;
	friend class KSkill;
	friend class KBuffList;
	friend class KItemList;

#ifdef _SERVER
public:
    void AddBullet(KSKILL_BULLET* pBullet);
	BOOL ApplyBullet(KSKILL_BULLET *pBullet);
    void ClearBullet();

    void CheckBullet();
    void ProcessBullet(KSKILL_BULLET *pBullet);
    
    KSKILL_BULLET* m_pBulletListHead;

private:
    KSKILL_HIT_RESULT JudgeSkillHit(KSKILL_BULLET* pBullet);
#endif

public:
    // �������������������̵Ĳ���

    struct KSKILL_ATTRIBUTE_PARAM 
    {
        KSkillRecipePointer SkillRecipePointer;
        DWORD               dwSkillSrcID;
        int                 nSkillSrcLevel;
        KCharacter*         pSkillSrc;
        KCharacter*         pSkillDst;
        BOOL                bCriticalStrike;       // �Ƿ񱩻�
        BOOL                bParray;               // �Ƿ��м�
        BOOL                bInsight;              // �Ƿ�ʶ��
        // �������ߵĲ���,�˺�����ʱҪ��,�ر���Dot
        KSKILL_BULLET*      pBullet;
        int                 nPhysicsDamage;         // �⹦�˺�����
        int                 nSolarDamage;           // �����ڹ��˺�
        int                 nNeutralDamage;         // �����ڹ��˺�
        int                 nLunarDamage;           // �����ڹ��˺�
        int                 nPoisonDamage;          // �����˺�����
        int                 nDamgeToLifeForSelf;    // ͵ȡ�Է���������
        int                 nDamgeToManaForSelf;    // ͵ȡ�Է���������
        int                 nTherapy;               // ��������

        int                 nBaseThreatCoefficient;
        int                 nDamageThreatCoefficient;
        int                 nTherapyThreatCoefficient;

        BOOL                bDot;
        int                 nStackNum;

        int                 nDamageAddPercent;  // �������Ŷ��˺��ļӳɣ��������͡�
    };

    KSKILL_ATTRIBUTE_PARAM* m_pSkillAttributeParam;

    KSKILL_CALCULATE_RESULT* m_pSrcSkillCalculateResult;
    KSKILL_CALCULATE_RESULT* m_pDstSkillCalculateResult;

    int m_nSkillStackDepth;

    // �ýṹ���ڳ���ͬ���͵��˺��������
    struct KDAMAGE_CALCULATE_PARAM
    {
        KCharacter*  pSkillSrc;
        BOOL         bCriticalStrikeFlag;           // �Ƿ񱩻�
        int          nDamageValue;                  // ������ֵ
        int          nDstResistPercent;             // ���԰ٷֱ�
        int          nDstDefenceCoefficient;        // ����ϵ��
        int*         pnDstMaxTypedDamageAbsorb;     // �����͵��˺�����
        int*         pnDstMaxTypedDamageManaShield; // �����͵�ManaShield
        int          nDstPointReflection;           // ��������
        int          nDstPercentReflection;         // �ٷֱȷ���
        int          nDamageToLifeForSrc;           // �˺�ת��Ϊ�ͷ��ߵ�����
        int          nDamageToManaForSrc;           // �˺�ת��Ϊ�ͷ��ߵ�����
        int          nDamageToLifeForSrcParty;      // �˺�ת��Ϊ�ͷ��ߵ�С�����г�Ա������
        int          nDamageToManaForSrcParty;      // �˺�ת��Ϊ�ͷ��ߵ�С�����г�Ա������
        int*         pnDstTypedDamage;              // ��Ŀ����������˺���ŵ�����
        int          nDstTypedDamageCoefficient;
    };

    // ����
    BOOL ProcessKnockedDown(int nFrame);
    // ����
    BOOL ProcessKnockedBackExhale(int nDistance, int nVelocityXY);       // ˮƽ�����ϱ�����(�������ż����ͷ���)   
    BOOL ProcessKnockedBackConvergence(int nDistance, int nVelocityXY);  // ˮƽ�����ϱ�����(�������ż����ͷ���)  
    BOOL ProcessKnockedBackRandom(int nDistance, int nVelocityXY);       // ˮƽ�����ϱ�����(�������)   
    BOOL ProcessKnockedBackSrcFace(int nDistance, int nVelocityXY);      // ˮƽ�����ϱ�����(����Ϊ�����ͷ��ߵĳ���)       
    // ����
    BOOL ProcessKnockedOffParabola(int nDistance, int nVelocityXY);      // �������߱�����(ˮƽ�ϵķ������ż����ͷ���)  
    // ��ץ�����
    BOOL ProcessPull(KCharacter* pCharacter, int nVelocityXY);
    // ��������
    BOOL ProcessRepulsed(int nFrame);
    // �־�
    BOOL ProcessFear(int nDistance);
    // ����
    BOOL ProcessRevive(int nLife, int nMana);
    BOOL ProcessTherapy(int nTherapyValue);
    BOOL ProcessDamage(int nDamageType, int nDamageValue);

    BOOL ConcludeResult(DWORD dwBulletID, BOOL bCriticalStrikeFlag, int* pnRetDamageValue);
private:
    BOOL CalculateDamage(const KDAMAGE_CALCULATE_PARAM &crParam);

public:
    KBuffList m_BuffList;

public:
#ifdef _SERVER
    typedef std::vector<KUSER_SKILL_EVENT> KUSER_SKILL_EVENT_VECTOR;
    KUSER_SKILL_EVENT_VECTOR    m_SkillEventVector;

    // ע��: "KUSER_SKILL_EVENT_VECTOR SkillEvent"�������ô�ֵ�ǹ�������д��
    // ��Ϊ�����¼��������ܿ���ȥ�޸�ԭ����Vector
    void OnSkillEvent(
        KSkill* pSkill, int nEventType, KUSER_SKILL_EVENT_VECTOR SkillEvent, 
        KCharacter* pCaster, KCharacter* pTarget
    );
    void CheckBeatBreak(KSkill* pSkill);    // ���, ����ֻ��ͨ������Ч
    void CheckBeatBack();
#endif
    void OnOTActionBeatBack(int nFrame);
    void OnDamage(KCharacter* pAttackter, int nDamage);

	DWORD GetMapID();

	BOOL CanDialog(KCharacter* pCharacter);
	BOOL CanDoAction(KCharacter* pCharacter, DWORD dwAction);

#ifdef _SERVER
public:
	int m_nLastBroadcastLife;
#endif

    // -------------------------- ���� --------------------------------------------->
    KSkillDebug             m_SkillDebug;

    int LuaAttach(Lua_State* L);
    int LuaDetach(Lua_State* L);
    int LuaDetachAll(Lua_State* L);
    // <-----------------------------------------------------------------------------

    KAIVM   m_AIVM;

public:
	// �ű��ӿں���
    int LuaSetModelID(Lua_State* L);
    int LuaSetTarget(Lua_State* L);
	int LuaGetTarget(Lua_State* L);
    int LuaCastSkillXYZ(Lua_State* L);
	int LuaCastSkill(Lua_State* L);

    void HostStand();
	void HostJump(BOOL bStandJump, int nDirectionXY);
	void HostDash(int nDirection, int nDistance, int nVelocityXY);
	void HostTurnTo(int nNewDirection);
    void HostPitchTo(int nNewDirection);

    int LuaAutoFly(Lua_State* L);
	int LuaStop(Lua_State* L);
    int LuaStand(Lua_State* L);
	int LuaRunTo(Lua_State* L);
	int LuaWalkTo(Lua_State* L);
    int LuaSwimTo(Lua_State* L);
	int LuaJump(Lua_State* L);
	int LuaDash(Lua_State* L);
	int LuaTurnTo(Lua_State* L);
    int LuaPitchTo(Lua_State* L);

	int LuaPatrol(Lua_State* L);

	int LuaSitDown(Lua_State* L);
	int LuaDie(Lua_State* L);

	int LuaDoAction(Lua_State* L);

    int LuaGetAIType(Lua_State* L);
	int LuaSetAIType(Lua_State* L);
    int LuaSetAIDebug(Lua_State* L);

#ifdef _CLIENT
    int LuaGetBuffList(Lua_State* L);
#endif

    int LuaCancelBuff(Lua_State* L);

    int LuaSetSheath(Lua_State* L);

	int LuaGetMapID(Lua_State* L);
	int LuaCanDialog(Lua_State* L);

#ifdef _SERVER
    int LuaAddBuff(Lua_State* L);

    int LuaGetBuff(Lua_State* L);

    int LuaDelBuff(Lua_State* L);
    int LuaDelGroupBuff(Lua_State* L);

    int LuaDelMultiGroupBuffByFunctionType(Lua_State* L);

    int LuaSetBuffNextActiveFrame(Lua_State* L);
    int LuaSetBuffLeftActiveCount(Lua_State* L);

	int LuaClearAllThreat(Lua_State* L);

	int LuaSetForceID(Lua_State* L);

	int LuaSetPosition(Lua_State* L);
#endif

    int LuaFireAIEvent(Lua_State* L);

#ifdef _SERVER
    int LuaSetCustomInteger4(Lua_State* L);
    int LuaGetCustomInteger4(Lua_State* L);
    int LuaSetCustomInteger2(Lua_State* L);
    int LuaGetCustomInteger2(Lua_State* L);
    int LuaSetCustomInteger1(Lua_State* L);
    int LuaGetCustomInteger1(Lua_State* L);

    int LuaSetCustomUnsigned4(Lua_State* L);
    int LuaGetCustomUnsigned4(Lua_State* L);
    int LuaSetCustomUnsigned2(Lua_State* L);
    int LuaGetCustomUnsigned2(Lua_State* L);
    int LuaSetCustomUnsigned1(Lua_State* L);
    int LuaGetCustomUnsigned1(Lua_State* L);
    
    int LuaSetCustomBoolean(Lua_State* L);
    int LuaGetCustomBoolean(Lua_State* L);

    // ƫ����, �ڼ���Bit(������, ��0��ʼ����), ��ֵΪtrue/false
    int LuaSetCustomBitValue(Lua_State* L);
    int LuaGetCustomBitValue(Lua_State* L);

    int LuaSetCustomString(Lua_State* L);
    int LuaGetCustomString(Lua_State* L);
#endif

    int LuaGetSkillPrepareState(Lua_State* L);
#ifdef _SERVER
    int LuaTraceSelectTargetThreat(Lua_State* L);
#endif

    int LuaSetAIOriginPos(Lua_State* L);
    int LuaGetAIOriginPos(Lua_State* L);
};

int g_GetRelation(KCharacter* pSrcCharacter, KCharacter* pDstCharacter);


#endif	//_KCHARACTER_H_
