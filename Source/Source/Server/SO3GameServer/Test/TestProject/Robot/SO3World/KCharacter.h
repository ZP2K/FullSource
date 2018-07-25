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
#include "Global.h"
#include "SO3Result.h"
#include "KAttribute.h"
#include "KSceneObject.h"
#include "KThreatList.h"
//#include "KBuffList.h"
#include "KItem.h"
#include "KTarget.h"

#define MAX_POSITION_RECORD	(GAME_FPS * 4)

class KCharacter;
class KPatrolPath;
//struct KSKILL_BULLET;
//class KSkill;
class KLootList;
class KItem;
class KAIControl;

//��ɫ״̬����ת��ָ��
enum CHARACTER_COMMAND
{
	ccInvalid = 0,

	ccTotal
};

struct KCharaterCommand
{
	CHARACTER_COMMAND	CmdKind;		// ����C
	int					Param_X;		// ����X
	int					Param_Y;		// ����Y
	int					Param_Z;		// ����Z
};

// ��ɫ״̬����״̬
enum CHARACTER_MOVE_STATE
{
	cmsInvalid = 0,				// ��Ч״̬

	// �ܿ�״̬
	cmsOnStand,					// վ��״̬
	cmsOnWalk,					// ��·״̬
	cmsOnRun,					// �ܲ�״̬
    cmsOnJump,                  // ��Ծ״̬
    cmsOnSwim,                  // ��Ӿ״̬
	cmsOnSit,					// ��״̬
    cmsOnMeditate,              // ����״̬

	// ���ܿ�״̬
	cmsOnKnockedDown,			// ������״̬
	cmsOnKnockedBack,			// ������״̬
	cmsOnKnockedOff,			// ������״̬ 
    cmsOnStun,                  // ������״̬(ѣ��)       
    cmsOnHalt,                  // ����ס�������

	cmsOnDeath,					// ����״̬
	cmsOnDash,					// ���״̬

	cmsTotal
};



// �ýṹ���ڽ��ռ�������Ľ��
//struct KSKILL_CALCULATE_RESULT
//{
//    BOOL bHitTarget;
//    BOOL bSkillDodge;
//    BOOL bCriticalStrike;
//
//    int  nSkillEffectResult[serTotal];   
//    BOOL bPhysicsBlock;
//    BOOL bSolarMagicBlock;
//    BOOL bNeutralMagicBlock;
//    BOOL bLunarMagicBlock;
//    BOOL bPoisonBlock;
//    BOOL bDebuffDodge;
//    BOOL bDebuffImmunity;
//    int  nDebuffID;
//    int  nDebuffLevel;
//    BOOL bDotDodge;
//    int  nDotID;
//    int  nDotLevel;
//    
//    struct
//    {
//        KSKILL_EFFECT_TYPE  eType;
//        DWORD               dwID;
//        int                 nLevel;        
//    } EffectSrc;
//};

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
    // ... ...
};

//struct KProfessionSkill
//{
//	KTarget Target;
//	DWORD dwCraftID;
//	DWORD dwRecipeID;
//};

//struct KOT_ACTION_PARAM 
//{
//	KOT_ACTION_TYPE	    eActionType;
//	union 
//	{
//		KSkill*         pSkill;
//		KDoodad*        pDoodad;
//		KItem*			pItem;
//	} Data;
//    KTarget		        Target;
//	KProfessionSkill	ProfessionSkill;
//	int			        nEndFrame;
//    union
//    {
//        int             nNextActFrame;  // ͨ�������������ʾ��һ������ʱ��
//        int             nPrepareFrame;  // ���������������ʾ�ü��ܱ�������ʱ�� 
//    };
//
//    int                 nBeatBackCount; // ����/ͨ�������˵ļ���
//	DWORD				dwScriptID;		// CustomOTAction���һ���ű�
//	BOOL				bIsBreakable;	// �Ƿ�ᱻ�������
//};

//struct KAUTO_CAST_SKILL_PARAM
//{
//    KSkill* pSkill;
//    BOOL    bMelee;
//    int*    pnCastCycle;
//    int     nPreCastFrame;
//};

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


class KCharacter : public KSceneObject
{
//����ֵ
public:
	//����
	char						m_szName[_NAME_LEN];		// ��ɫ��
	char						m_szTitle[TITLE_LEN];		// �ƺ�
	CHARACTER_GENDER			m_eGender;					// �Ա�
	CHARACTER_KIND				m_eKind;					// ��ɫ����
	DWORD						m_dwClassID;				// ����ID
	CHARACTER_RACE				m_eRace;					// ��ɫ����
	DWORD						m_dwForceID;			    // ����
	int							m_nLevel;					// ��ɫ�ȼ�
	int							m_nExperience;				// ��ɫ����
	int							m_nMoral;					// ��ɫ�ĵ���ֵ
    DWORD                       m_dwPortraitID;             // ͷ��ID
	DWORD						m_dwCharacterAction;		// ����״̬

	DECLARE_LUA_STRING(Name, _NAME_LEN);
	DECLARE_LUA_STRING(Title, TITLE_LEN);
	DECLARE_LUA_ENUM(Gender);
	DECLARE_LUA_ENUM(Kind);
	DECLARE_LUA_DWORD(ClassID);
	DECLARE_LUA_ENUM(Race);
	DECLARE_LUA_DWORD(ForceID);
	DECLARE_LUA_INTEGER(Level);
	DECLARE_LUA_INTEGER(Experience);
	DECLARE_LUA_INTEGER(Moral);
	DECLARE_LUA_DWORD(CharacterAction);
    // -------------- �ƶ���� -------------------------------------------------->
	CHARACTER_MOVE_STATE		m_eMoveState;				// �ƶ�״̬

    int                         m_nVelocityXY;  // ˮƽ�ٶ�
    int                         m_nDirectionXY; // ˮƽ�ٶȷ���
    int                         m_nVelocityZ;   // ��ֱ�ٶ�

	int							m_nDestX;					
	int							m_nDestY;		

    // �����ٶ�,������Runʱ,˲ʱ�ٶȻ��ܵ���,������Ӱ����仯,�����ٶ����������ֵ
    int                         m_nConvergenceSpeed;

    int                         m_nWalkSpeed;
    int                         m_nRunSpeed;
    int                         m_nSwimSpeed;           

    int                         m_nWalkSpeedBase;           
    int                         m_nRunSpeedBase;           
    int                         m_nSwimSpeedBase;

    int                         m_nMoveSpeedPercent;    

    int			                m_nCurrentGravity;		
	int			                m_nGravity;			       
    int			                m_nGravityBase;
    int			                m_nGravityPercent;

	int							m_nJumpCount;				// ��Ծ������
	int							m_nMaxJumpCount;		    // �����Ծ����
    int                         m_nDropDefence;             // ��ˤ��ϵ��, [0 - 1024]

    int                         m_nJumpSpeedXY;             // ��Ծ��ˮƽ���ٶ�
    int                         m_nJumpSpeedXYBase;  
    int                         m_nJumpSpeedXYPercent;

    int                         m_nJumpSpeedZ;              // ��Ծ�Ĵ�ֱ���ٶ�
    int                         m_nJumpSpeedZBase;
    int                         m_nJumpSpeedZPercent;

    int                         m_nDivingCount;
    int                         m_nDivingFrame;             // ���Ǳˮʱ��
    int                         m_nDivingFrameBase;
    int                         m_nDivingFramePercent; 

    int                         m_nSwimAbility;             // �Ƿ����Ӿ
    int                         m_nWaterFlyAbility;         // �Ƿ����ˮ��Ư

    BOOL                        m_bTryPathState;            // �Ƿ�������·����֮��
    int                         m_nTryPathSide;             // ����������������·
    int                         m_nTryPathDestX;            // ��·��ʼ��ʱ����ƶ�Ŀ���X
    int                         m_nTryPathDestY;            // ��·��ʼ��ʱ����ƶ�Ŀ���Y

    int                         m_nSubjectiveMoveDirection; // �����ƶ���Ը�ķ���,���ڱ����߼��������л���

	DECLARE_LUA_ENUM(MoveState);

    DECLARE_LUA_INTEGER(VelocityXY);
    DECLARE_LUA_INTEGER(DirectionXY);
    DECLARE_LUA_INTEGER(VelocityZ);

	DECLARE_LUA_INTEGER(DestX);
	DECLARE_LUA_INTEGER(DestY);

    DECLARE_LUA_INTEGER(WalkSpeed);
    DECLARE_LUA_INTEGER(RunSpeed);
    DECLARE_LUA_INTEGER(SwimSpeed);

    DECLARE_LUA_INTEGER(WalkSpeedBase);
	DECLARE_LUA_INTEGER(RunSpeedBase);
    DECLARE_LUA_INTEGER(SwimSpeedBase);

	DECLARE_LUA_INTEGER(MoveSpeedPercent);
	
    DECLARE_LUA_INTEGER(MaxJumpCount);
    DECLARE_LUA_INTEGER(Gravity);
    DECLARE_LUA_INTEGER(GravityBase);
    DECLARE_LUA_INTEGER(GravityPercent);

    DECLARE_LUA_INTEGER(JumpSpeedXY);
    DECLARE_LUA_INTEGER(JumpSpeedXYBase);
    DECLARE_LUA_INTEGER(JumpSpeedXYPercent);

    DECLARE_LUA_INTEGER(JumpSpeedZ);
    DECLARE_LUA_INTEGER(JumpSpeedZBase);
    DECLARE_LUA_INTEGER(JumpSpeedZPercent);

    DECLARE_LUA_INTEGER(DropDefence);

    DECLARE_LUA_INTEGER(DivingCount);         
    DECLARE_LUA_INTEGER(DivingFrame);         
    DECLARE_LUA_INTEGER(DivingFrameBase);
    DECLARE_LUA_INTEGER(DivingFramePercent); 

    DECLARE_LUA_INTEGER(SwimAbility);
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

    int							m_nCurrentLife;				// ��ǰ����
	int							m_nMaxLife;					// �������ֵ    
    int                         m_nMaxLifeBase;             // ��ȡ����ְҵ,�ȼ�
	int							m_nMaxLifePercentAdd;	    // �������ֵ���Ӱٷֱ�
	int							m_nMaxLifeAdditional;		// ׷�ӵ��������ֵ
	int							m_nLifeReplenish;			// ���������ָ��ٶ� = m_nSpunkLifeReplenish + ����(װ����BUFF�ӳɵ�)
    int                         m_nSpunkLifeReplenish;      // ����Ӱ��������ָ�����
	int							m_nLifeReplenishPercent;	// �����ٷֱȻָ��ٶ�, ��λ1/1024

	DECLARE_LUA_INTEGER(CurrentLife);
	DECLARE_LUA_INTEGER(MaxLife);
    DECLARE_LUA_INTEGER(MaxLifeBase);
	DECLARE_LUA_INTEGER(MaxLifePercentAdd);
	DECLARE_LUA_INTEGER(MaxLifeAdditional);
	DECLARE_LUA_INTEGER(LifeReplenish);
	DECLARE_LUA_INTEGER(LifeReplenishPercent);
	

    // ---------------- ���� ----------------------------------------------------->

	int							m_nCurrentMana;				
	int							m_nMaxMana;					
    int                         m_nMaxManaBase;             
	int							m_nMaxManaPercentAdd;		
	int							m_nMaxManaAdditional;		
	int							m_nManaReplenish;        // �������� = m_nSpunkManareplenish + ����(װ����BUFF�ӳɵ�)
    int                         m_nSpunkManareplenish;   // ����Ӱ��Ļ�������
	int							m_nManaReplenishPercent;	

	DECLARE_LUA_INTEGER(CurrentMana);
	DECLARE_LUA_INTEGER(MaxMana);
    DECLARE_LUA_INTEGER(MaxManaBase);
	DECLARE_LUA_INTEGER(MaxManaPercentAdd);
	DECLARE_LUA_INTEGER(MaxManaAdditional);
	DECLARE_LUA_INTEGER(ManaReplenish);
	DECLARE_LUA_INTEGER(ManaReplenishPercent);

    // ---------------- ŭ�� ----------------------------------------------------->

	int							m_nCurrentRage;				
	int							m_nMaxRage;					
    int                         m_nMaxRageBase;
	int							m_nMaxRagePercentAdd;		
	int							m_nMaxRageAdditional;		
	int							m_nRageReplenish;			
    int							m_nRageReplenishPercent;	

	DECLARE_LUA_INTEGER(CurrentRage);
	DECLARE_LUA_INTEGER(MaxRage);
    DECLARE_LUA_INTEGER(MaxRageBase);
	DECLARE_LUA_INTEGER(MaxRagePercentAdd);
	DECLARE_LUA_INTEGER(MaxRageAdditional);
	DECLARE_LUA_INTEGER(RageReplenish);
    DECLARE_LUA_INTEGER(RageReplenishPercent);

    // --------------- ս������ --------------------------------------------------->
	int							m_nDodge;			        // ���ܵ��� = m_nAgilityDodege + ����(װ����BUFF�ӳɵ�)
    int                         m_nAgilityDodege;           // ��Ӱ������ܵ���
    BOOL                        m_bFightState;
    BOOL                        m_bSheathFlag;
    int                         m_nPositiveShield;          // ����Ч������,������������Ч��(���Ѫ,Buff)����
    int                         m_nNegativeShield;          // ����Ч������,�������и���Ч��(���˺�, Debuff)����
    int                         m_nGlobalBlock;             // �����͸�
    int                         m_nGlobalDamageAbsorb;      // ���������Ƶ������˺�
	int							m_nGlobalDamageManaShield;  // ���������Ƶ����������˺�
	int							m_nStealLifePercent;	    // ͵ȡ�з������ٷֱ�	
	int							m_nStealManaPercent;        // ͵ȡ�з������ٷֱ�
    int                         m_nBeatBackRate;
    int							m_nStunStrikeRate;		    // ������
    int                         m_nKnockedBackRate;         // ��������
    int							m_nKnockedOffRate;		    // ��������

    // ͵ȡ�з������ٷֱ�: ָ�Եз�����˺����˺��еĶ��ٱ���ת��Ϊ�Լ�������,��λ(1/1024)
    // ͵ȡ�з������ٷֱ�: �������A��B����,A�ܵ������˺�1000��,��ʱ,A����͵ȡB������(1000 * m_nStealManaPercent / 1024)

	DECLARE_LUA_INTEGER(Dodge);
    DECLARE_LUA_BOOL(FightState);
    DECLARE_LUA_BOOL(SheathFlag);
    DECLARE_LUA_INTEGER(PositiveShield);
    DECLARE_LUA_INTEGER(NegativeShield);
    DECLARE_LUA_INTEGER(GlobalBlock);
    DECLARE_LUA_INTEGER(GlobalDamageAbsorb);
	DECLARE_LUA_INTEGER(GlobalDamageManaShield);
    DECLARE_LUA_INTEGER(StealLifePercent);
    DECLARE_LUA_INTEGER(StealManaPercent);
    DECLARE_LUA_INTEGER(BeatBackRate);
    DECLARE_LUA_INTEGER(StunStrikeRate);
    DECLARE_LUA_INTEGER(KnockedBackRate);
    DECLARE_LUA_INTEGER(KnockedOffRate);

    int m_nPhysicsAttackPower[wdtTotal];        // ������ѧ���⹦�˺�������
    int m_nMagicAttackPower[wdtTotal];          // ������ѧ���ڹ��˺�������


    // -------------- �⹦ ------------------------------------------------------>

    int m_nMeleeWeaponDamageBase;
    int m_nMeleeWeaponDamageRand;

    int m_nRangeWeaponDamageBase;
    int m_nRangeWeaponDamageRand;

    int m_nSkillPhysicsDamage;
    int m_nSkillPhysicsDamageRand;
    int m_nSkillPhysicsDamagePercent;
    int m_nPhysicsAttackPowerBase;
    int m_nPhysicsDamageCoefficient;     // �⹦�˺��ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)

    int m_nPhysicsAttackHit;      
    int m_nAgilityCriticalStrike;        // ��Ӱ����⹦����һ��ֵ
    int m_nPhysicsCriticalStrike;        // �⹦����һ��ֵ = m_nAgilityCriticalStrike + ����(װ����BUFF�ӳɵ�)

    int	m_nPhysicsResistPercent;	
             
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

	DECLARE_LUA_INTEGER(PhysicsAttackPowerBase);    
    DECLARE_LUA_INTEGER(PhysicsAttackHit);
    DECLARE_LUA_INTEGER(PhysicsCriticalStrike);
    DECLARE_LUA_INTEGER(PhysicsResistPercent);
    DECLARE_LUA_INTEGER(PhysicsShield);
    DECLARE_LUA_INTEGER(PhysicsReflection);		
    DECLARE_LUA_INTEGER(PhysicsReflectionPercent);
    DECLARE_LUA_INTEGER(PhysicsBlock);
    DECLARE_LUA_INTEGER(PhysicsDamageAbsorb);    
   	DECLARE_LUA_INTEGER(PhysicsDamageManaShield); 

    // -------------- �ڹ� ----------------------------------------------------------
    int m_nSpunkAttackPower;
    // m_nMeleeSpunkAttackPowerPercent = m_nMeleeSpunkAttackPower / (m_nMeleeSpunkAttackPower + 5 * m_nLevel + 5)
    int m_nMeleeSpunkAttackPowerPercent;
    int m_nMeleeSpunkAttackPower;
    int m_nRangeSpunkAttackPowerPercent;
    int m_nRangeSpunkAttackPower;
    // ����Ӱ����ڹ�����һ��ֵ; �����ڹ�������һ��ֵ = m_nSpiritCriticalStrike + ����(װ����BUFF�ӳɵ�)
    int m_nSpiritCriticalStrike;            

    DECLARE_LUA_INTEGER(SpunkAttackPower);

    // -------------- �����ڹ� ------------------------------------------------------>

    int m_nSkillSolarDamage;
    int m_nSkillSolarDamageRand;
    int m_nSkillSolarDamagePercent;	
    int m_nSolarAttackPowerBase;
    int m_nSolarDamageCoefficient;      // �����ڹ��˺��ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)
	
    int m_nSolarMagicHit;               
    int m_nSolarCriticalStrike;         

	int	m_nSolarMagicResistPercent;	    // �ֿ��ٷֱ�

	int m_nSolarMagicShield;			// ������,�����ֵ���ջ���������÷���ϵ��

	int	m_nSolarMagicReflection;		
	int	m_nSolarMagicReflectionPercent;	

    int m_nSolarMagicBlock;             

    int m_nSolarDamageAbsorb;     
	int	m_nSolarDamageManaShield; 

    DECLARE_LUA_INTEGER(SolarMagicHit);
    DECLARE_LUA_INTEGER(SolarCriticalStrike);
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
    int m_nNeutralAttackPowerBase;
    int m_nNeutralDamageCoefficient;        // �����ڹ��˺��ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)        

    int m_nNeutralMagicHit;               
    int m_nNeutralCriticalStrike;         

    int	m_nNeutralMagicResistPercent;	    // �ֿ��ٷֱ�

    int m_nNeutralMagicShield;			    // ������,�����ֵ���ջ���������÷���ϵ��

    int	m_nNeutralMagicReflection;		
    int	m_nNeutralMagicReflectionPercent;	

    int m_nNeutralMagicBlock;             

    int m_nNeutralDamageAbsorb;     
    int	m_nNeutralDamageManaShield; 

    DECLARE_LUA_INTEGER(NeutralMagicHit);
    DECLARE_LUA_INTEGER(NeutralCriticalStrike);
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
    int m_nLunarAttackPowerBase;
    int m_nLunarDamageCoefficient;          // �����ڹ��˺��ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)

    int m_nLunarMagicHit;               
    int m_nLunarCriticalStrike;         

    int	m_nLunarMagicResistPercent;	        // �ֿ��ٷֱ�

    int m_nLunarMagicShield;			    // ������,�����ֵ���ջ���������÷���ϵ��

    int	m_nLunarMagicReflection;		
    int	m_nLunarMagicReflectionPercent;	

    int m_nLunarMagicBlock;             

    int m_nLunarDamageAbsorb;     
    int	m_nLunarDamageManaShield; 

    DECLARE_LUA_INTEGER(LunarMagicHit);
    DECLARE_LUA_INTEGER(LunarCriticalStrike);
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
    int m_nPoisonAttackPowerBase;
    int m_nPoisonDamageCoefficient;         // �����ڹ��˺��ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)

    int m_nPoisonMagicHit;               
    int m_nPoisonCriticalStrike;         

    int	m_nPoisonMagicResistPercent;	    // �ֿ��ٷֱ�

    int m_nPoisonMagicShield;			    // ������,�����ֵ���ջ���������÷���ϵ��

    int	m_nPoisonMagicReflection;		
    int	m_nPoisonMagicReflectionPercent;	

    int m_nPoisonMagicBlock;             

    int m_nPoisonDamageAbsorb;     
    int	m_nPoisonDamageManaShield; 

    DECLARE_LUA_INTEGER(PoisonMagicHit);
    DECLARE_LUA_INTEGER(PoisonCriticalStrike);
    DECLARE_LUA_INTEGER(PoisonMagicShield);
    DECLARE_LUA_INTEGER(PoisonMagicResistPercent);
    DECLARE_LUA_INTEGER(PoisonMagicReflection);		
    DECLARE_LUA_INTEGER(PoisonMagicReflectionPercent);	
    DECLARE_LUA_INTEGER(PoisonMagicBlock);
    DECLARE_LUA_INTEGER(PoisonDamageAbsorb);     
    DECLARE_LUA_INTEGER(PoisonDamageManaShield); 

    // ---------------------------- ����(�����ظ�) -------------------------------------------------
    int m_nTherapyPowerBase;            // ������������
    int m_nSkillTherapy; 		        // ������ص�����Ч��
    int m_nSkillTherapyRand;            
    int m_nSkillTherapyPercent; 
    int m_nTherapyCoefficient;          // ����Ч���ӳ�ϵ��, ���˺��������̵�����������ϵ��(��ȡ����ֵ�Ǳ��˺������ϵ�)

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

    int                         m_nIgnoreOtherAlarmRange;             // ��������(NPC)���䷶Χ
    int                         m_nSelfAlarmRangeRevise;              // �������䷶Χ������; һ������NPC
    int                         m_nOtherAlarmRangeRevise;             // �����˾��䷶Χ������ 

public:
    // m_dwRepresentState�����˽�ɫ���ϵ�ħ����Ч״̬(��ʵ��������ЧID�����), һ������������Ч��:
    // 1. �ŵ��²ȵ�(���ҵ���0-4λ).
    // 2. ���ϲ��Ƶ�(���ҵ���5-9λ)
    // 3. ͷ�϶��ŵ�(���ҵ���10-14λ)
	DWORD   					m_dwRepresentState;

    BOOL    GetRepresentState(KBUFF_REPRESENT_POS nPos, DWORD&   rdwRepresentID);

public:
	KThreatList				m_ThreatList;				    // ��ޱ�

//״̬��
public:
    //BOOL PrepareSkill(DWORD dwSkillID, DWORD dwSkillLevel, KTarget& rTarget, int nFrame);
    //SKILL_RESULT_CODE CastSkill(DWORD dwSkillID, DWORD dwSkillLevel, KTarget& rTarget, DWORD dwBulletID);
    //SKILL_RESULT_CODE CastSkillSub(DWORD dwSkillID, DWORD dwSkillLevel, KTarget& rTarget, DWORD dwBulletID);

	BOOL Stop(void);			
    // �ú��������������귶Χ
    BOOL EmendateDestCoordinate(int& nDestX, int& nDestY); 

    BOOL Stand(BOOL bForceMove = false);
	BOOL WalkTo(int nDestX, int nDestY, BOOL bForceMove = false);
	BOOL RunTo(int nDestX, int nDestY, BOOL bForceMove = false);			
    BOOL SwimTo(int nDestX, int nDestY, int nDestZ, BOOL bForceMove = false);			
	BOOL Jump(BOOL bStandJump, int nJumpDirection, BOOL bForceMove = false);
	BOOL SitDown(BOOL bForceMove = false);

	BOOL Dash(int nDirection, int nDistance, int nVelocityXY);
	BOOL KnockedDown(int nFrame);
	BOOL KnockedBack(int nDirection, int nDistance, int nVelocityXY);
	BOOL KnockedOff(int nDirection, int nDistance, int nVelocityXY);
    BOOL Stun();                                                // �����λ�ѣ��      
    BOOL Halt();                                                // ��ס������ȡ���

    BOOL Meditate();                                            // ����
	BOOL Die(KCharacter* pKiller);								// ����
    BOOL Revive();

	BOOL Say(char* pszText);									// ˵��(��Ļ����)
	BOOL Action(DWORD dwCharacterID, BYTE byActionType);		// ����
	BOOL Turn(int nNewDirection);
    BOOL Pitch(int nNewDirection);
	//BOOL Open(KDoodad* pDoodad);								// ����
    BOOL MoveTo(int nX, int nY, int nZ);

protected:
	BOOL						m_bOnTalk;					// �Ƿ����ں�NPC�Ի�
   
public:
 //   int                         m_nStunCounter;             // ���μ�����(ѣ��)
 //   int                         m_nHaltCounter;             // ���������
	  int						  m_nMoveFrameCounter;

public:
    KTarget                     m_SelectTarget;
    KTarget                     m_SkillTarget;

public:

    // ״̬������
    BOOL OnStand(){ return true; }
    BOOL OnWalk(){ return true; }
    BOOL OnRun(){ return true; }
    BOOL OnJump(){ return true; }
    BOOL OnSit(){ return true; }
    BOOL OnMeditate(){ return true; }
    BOOL OnPassiveMove();
    BOOL OnStun(){ return true; }
    BOOL OnHalt(){ return true; }
    BOOL OnDeath();

	// ״̬ת��ָ��
	void DoStand();
	void DoWalk();
	void DoRun();
    void DoSwim();
    void DoJump();
	void DoSit();
    void DoMeditate();

	void DoDash();              // ���
	void DoKnockedDown();       // ����
	void DoKnockedBack();		// ����
	void DoKnockedOff();		// ����
    void DoStun();
    void DoHalt();
	void DoDeath(int nDelayFrame);

public:

 //   // ------------- ����һ��ʱ�����Ϊ ---------------------->
	//void DoOTAction(KOT_ACTION_PARAM& rParam);

	//void DoActionIdle(void);

 //   void DoAutoCastSkill(KAUTO_CAST_SKILL_PARAM& param);

 //   void DoAutoCastSkillIdle(void);

 //   void DoSkillPrepare(KSkill* pSkill, int nPrepareFrame);

	//void DoSkillChannel(KSkill* pSkill, int nChannelFrame);

 //   void DoRecipePrepare(KProfessionSkill* pProfessionSkill, int nFrame);

	//void DoPickPrepare(KDoodad* pDoodad, int nFrame);

	//void DoPicking(KDoodad* pDoodad);

	//void DoCustomOTActionPrepare(BOOL bIsBreakable, int nFrame, KTarget& crTarget, DWORD dwScriptID);

	//void DoCustomOTActionChannel(BOOL bIsBreakable, int nFrame, KTarget& crTarget, DWORD dwScriptID);

 //   KOT_ACTION_PARAM            m_OTActionParam;
 //   KAUTO_CAST_SKILL_PARAM      m_AutoCastSkillParam;

 //   void ProcessOTAction();
 //   void ProcessAutoCastSkill();

 //   BOOL OnSkillPrepare();
 //   BOOL OnSkillChannel();
 //   BOOL OnRecipePrepare();
 //   BOOL OnPickPrepare();
	//BOOL OnPicking();
	//BOOL OnItemSkill();
	//BOOL OnCustomOTActionPrepare();
	//BOOL OnCustomOTActionChannel();

	//BOOL OnBreakPickPrepare(KOT_ACTION_PARAM& rNewAction);
	//BOOL OnBreakPicking(KOT_ACTION_PARAM& rNewAction);

    // <------------- ����һ��ʱ�����Ϊ ----------------------

public:

	BOOL ForwardPosition(int nForwardFrame);

    BOOL UpdateMoveParameter();

private:

	void ProcessMove();

    void ProcessFriction();          // Ħ����������
    void ProcessGradientSlip();      // б�´򻬼���
    void ProcessAcceleration();      // ������ٶȼ���  

    void ProcessHorizontalMove();    // ˮƽ�ƶ�
    void ProcessVerticalMove();      // ��ֱ�ƶ�

	BOOL CellMove(int nStepLength, int nDirection);
	int  TryMove(int nStepLength, int nDirection, int* pnObstacleDirection);

public:
    // ��ȡ��ˮ���, ��ˮ����,����һ������0��ֵ��ʾ��ˮ���,���򷵻�0
    // ��ˮ���ָˮ����������µĸ߶Ȳ�
    int GetWaterline();          

    // ȡ�ýŵ��µر�ľ��Ը߶�,��Ȼ,ˮ����"��"
    int GetEarthPos();

    // ��ȡ�������۵��ƶ���Ը����
    int GetSubjectiveDirection();

private:

public:
	KCharacter();
	virtual ~KCharacter();

	BOOL Init(void);
	BOOL UnInit(void);
	BOOL Activate(void);

public:
	BOOL ApplyAttribute(KAttribute* pAttr);
	BOOL UnApplyAttribute(KAttribute* pAttr);

private:
    int  m_nNextReplenishFrame;
    void AttribReplenish();
    void ReviseAttribValue();
    void CheckFightFlag();

	virtual BOOL ChangeRegion(KRegion* pRegion) = 0;
	virtual BOOL ChangeCell(KCell* pCell) = 0;

	// ����ѭ����ָ����������˵��ָ���б�����NPC��˵�ǵ���NPCAI
	virtual BOOL GetCommand() = 0;
	// �����ɫ��״̬��
	void RunStateMachine();

	friend class KAIController;
	friend class KAIBase;
	//friend class KSkill;
	friend class KBuffList;
	friend class KItemList;

public:
    // �������������������̵Ĳ���
    //struct  KSKILL_ATTRIBUTE_PARAM
    //{
    //    KSkill*         pSkill;
    //    DWORD           dwSkillSrcID;
    //    int             nSkillSrcLevel;
    //    KCharacter*     pSkillSrc;
    //    KCharacter*     pSkillDst;
    //    BOOL            bCriticalStrike;       // �Ƿ񱩻�
    //    // �������ߵĲ���,�˺�����ʱҪ��,�ر���Dot
    //    KSKILL_BULLET*  pBullet;
    //    int             nPhysicsDamage;         // �⹦�˺�����
    //    int             nSolarDamage;           // �����ڹ��˺�
    //    int             nNeutralDamage;         // �����ڹ��˺�
    //    int             nLunarDamage;           // �����ڹ��˺�
    //    int             nPoisonDamage;          // �����˺�����
    //    int             nStealLifePercent;      // ͵ȡ�Է���������
    //    int             nStealManaPercent;      // ͵ȡ�Է���������
    //    int             nTherapy;               // ��������
    //    int             nGainThreatCoefficient;
    //    int             nImpairThreatCoefficient;
    //    BOOL            bDot;
    //};

    //KSKILL_ATTRIBUTE_PARAM*  m_pSkillAttributeParam;


    //KSKILL_CALCULATE_RESULT* m_pSrcSkillCalculateResult;
    //KSKILL_CALCULATE_RESULT* m_pDstSkillCalculateResult;

    //// �ýṹ���ڳ���ͬ���͵��˺��������
    //struct KDAMAGE_CALCULATE_PARAM
    //{
    //    KCharacter*  pSkillSrc;
    //    BOOL         bCriticalStrikeFlag;           // �Ƿ񱩻�
    //    int          nDamageValue;                  // ������ֵ
    //    int          nDstResistPercent;             // ���԰ٷֱ�
    //    int          nDstDefenceCoefficient;        // ����ϵ��
    //    int*         pnDstMaxTypedDamageAbsorb;     // �����͵��˺�����
    //    int*         pnDstMaxTypedDamageManaShield; // �����͵�ManaShield
    //    int          nDstPointReflection;           // ��������
    //    int          nDstPercentReflection;         // �ٷֱȷ���
    //    int          nSrcStealLifePercent;
    //    int          nSrcStealManaPercent;
    //    int*         pnSrcRecvTypeDamage;           // ��Դ���������˺���ŵ�����
    //    int*         pnDstRecvTypeDamage;           // ��Ŀ����������˺���ŵ�����
    //};
    //
    //// ����
    //BOOL ProcessKnockedBackExhale(int nDistance, int nVelocityXY);       // ˮƽ�����ϱ�����(�������ż����ͷ���)   
    //BOOL ProcessKnockedBackConvergence(int nDistance, int nVelocityXY);  // ˮƽ�����ϱ�����(�������ż����ͷ���)  
    //BOOL ProcessKnockedBackRandom(int nDistance, int nVelocityXY);       // ˮƽ�����ϱ�����(�������)   
    //BOOL ProcessKnockedBackSrcFace(int nDistance, int nVelocityXY);      // ˮƽ�����ϱ�����(����Ϊ�����ͷ��ߵĳ���)       
    //// ����
    //BOOL ProcessKnockedOffParabola(int nDistance, int nVelocityXY);      // �������߱�����(ˮƽ�ϵķ������ż����ͷ���)  
    //// ���
    //BOOL ProcessDash(int nVelocityXY);
    //// ����
    //BOOL ProcessRevive(int nLife, int nMana);
    //BOOL ProcessTherapy(int nTherapyValue);
    //BOOL ProcessDamage(int nDamageType, int nDamageValue);

    //BOOL ProcessBuff(KSKILL_BUFF_INFO* pBuff);          // �������ϴ���BUFF 
    //BOOL ProcessBuff(int nID, int nLevel);              // ����ħ�����Ե��õ�BUFF
    //BOOL ProcessDebuff(KSKILL_DEBUFF_INFO* pDebuff);
    //BOOL ProcessDebuff(int nID, int nLevel);
    //BOOL ProcessDot(KSKILL_DOT_INFO* pDot);
    //BOOL ProcessDot(int nID, int nLevel);
    //BOOL ProcessHot(KSKILL_HOT_INFO* pHot);
    //BOOL ProcessHot(int nID, int nLevel);
private:
    //BOOL CalculateDamage(const KDAMAGE_CALCULATE_PARAM &crParam);

    //BOOL ConcludeResult(DWORD dwBulletID, BOOL bCriticalStrikeFlag);

public:

    //KBuffList m_BuffList;
    //void ApplyDot(const KUSER_DOT_INFO& crDotInfo);
    //void ApplyHot(const KUSER_HOT_INFO& crHotInfo);
    //BOOL CancelBuff(int nStyle, int nIndex);

public:
 //   void OnOTActionBeatBack(int nFrame);
 //   void OnDamage(KCharacter* pAttackter, int nDamage);

	DWORD GetMapID();

	//BOOL CanDialog(KCharacter* pCharacter);
	//BOOL CanDoAction(KCharacter* pCharacter, BYTE byAction);

public:
	//�ű��ӿں���
    int LuaSetTarget(Lua_State* L);
	int LuaGetTarget(Lua_State* L);
    int LuaCastSkillXYZ(Lua_State* L);
	int LuaCastSkill(Lua_State* L);

	int LuaOpen(Lua_State* L);

	int LuaStop(Lua_State* L);
    int LuaStand(Lua_State* L);
	int LuaRunTo(Lua_State* L);
	int LuaWalkTo(Lua_State* L);
    int LuaSwimTo(Lua_State* L);
	int LuaJump(Lua_State* L);
	int LuaDash(Lua_State* L);

	int LuaPatrol(Lua_State* L);

	int LuaSitDown(Lua_State* L);
	int LuaDie(Lua_State* L);

	int LuaSay(Lua_State* L);
	int LuaTurnTo(Lua_State* L);
    int LuaPitchTo(Lua_State* L);
	int LuaDoAction(Lua_State* L);

	int LuaSetAIType(Lua_State* L);

    int LuaGetBuffList(Lua_State* L);
    int LuaGetDebuffList(Lua_State* L);
    int LuaGetDotList(Lua_State* L);
    int LuaGetHotList(Lua_State* L);

    int LuaCancelBuff(Lua_State* L);

    int LuaSetSheath(Lua_State* L);

	int LuaGetMapID(Lua_State* L);
	int LuaCanDialog(Lua_State* L);
};

//����ID�ж��Ƿ�����ң����ID�ĸ�4λΪ0
inline BOOL g_IsPlayer(DWORD dwCharacterID)
{
	if (dwCharacterID <= 0x0FFFFFFF)
		return TRUE;
	else
		return FALSE;
}

int g_GetRelation(KCharacter* pSrcCharacter, KCharacter* pDstCharacter);

#endif	//_KCHARACTER_H_