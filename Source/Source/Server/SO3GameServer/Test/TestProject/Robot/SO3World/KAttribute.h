/************************************************************************/
/* Ӱ���������������					                                */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.01.18	Create												*/
/************************************************************************/
#ifndef _KATTRIBUTE_H_
#define _KATTRIBUTE_H_


enum ATTRIBUTE_TYPE
{
	atInvalid = 0,

    // ------------- �ƶ���� ------------------------------------------>
    atWalkSpeedBase,
    atRunSpeedBase,
    atSwimSpeedBase,
    atMoveSpeedPercent,
    atMaxJumpCount,
    atGravityBase,
    atGravityPercent,
    atJumpSpeedXYBase,
    atJumpSpeedXYPercent,
    atJumpSpeedZBase,
    atJumpSpeedZPercent,
    atDropDefence,
    atDivingFrameBase,
    atDivingFramePercent, 
    atSwimAbility,      
    atWaterFlyAbility,


    // ------------- Ǳ�� ---------------------------------------------->

	atStrengthBase,
    atStrengthBasePercentAdd,
	atAgilityBase,				
    atAgilityBasePercentAdd,
	atVitalityBase,
    atVitalityBasePercentAdd,
	atSpiritBase,
    atSpiritBasePercentAdd,
    atSpunkBase,
    atSpunkBasePercentAdd,

    // ------------- ���� ---------------------------------------------->

	atCurrentLife,				

    atMaxLifeBase,              
	atMaxLifePercentAdd,		
    atMaxLifeAdditional,      

    atLifeReplenish,
    atLifeReplenishPercent,		

    // ------------- ���� ---------------------------------------------->

	atCurrentMana,				

	atMaxManaBase,			
    atMaxManaPercentAdd,			
    atMaxManaAdditional,

    atManaReplenish,
    atManaReplenishPercent,

    // ------------- ���� ---------------------------------------------->

	atCurrentStamina,				

    atMaxStaminaBase,              
	atMaxStaminaPercentAdd,		
    atMaxStaminaAdditional,      

    atStaminaReplenish,
    atStaminaReplenishPercent,	

    atQiToSkillExpPercentAdd,

    // ------------- ŭ�� ---------------------------------------------->

	atCurrentRage,				

    atMaxRageBase,              
	atMaxRagePercentAdd,		
    atMaxRageAdditional,      

    atRageReplenish,
    atRageReplenishPercent,		

    // -------------- ��� --------------------------------------------->
    atGainPositiveThreatCoefficient,
    atGainNegativeThreatCoefficient,
    atImpairPositiveThreatCoefficient,
    atImpairNegativeThreatCoefficient,

    // ------------- ս������ ------------------------------------------>
    atDodge,
    atPositiveShield,
    atNegativeShield,
    atGlobalBlock,
    atGlobalDamageAbsorb,
    atGlobalDamageManaShield,
    atStealLifePercent,
    atStealManaPercent,
    atBeatBackRate,
    atKnockedBackRate,
    atKnockedOffRate,
    
    atPhysicsAttackPower,
    atMagicAttackPower,

    atMeleeWeaponDamageBase,
    atMeleeWeaponDamageRand,

    atRangeWeaponDamageBase,
    atRangeWeaponDamageRand,

    // ------------- �⹦ ---------------------------------------------->
    
    atSkillPhysicsDamage,
    atSkillPhysicsDamageRand,
    atSkillPhysicsDamagePercent,
    atPhysicsAttackPowerBase,
    atPhysicsDamageCoefficient,

    
    atPhysicsAttackHit,
    atPhysicsCriticalStrike,

    atPhysicsResistPercent,

    atPhysicsDefenceAdd,
    atPhysicsDefenceMax,

    atPhysicsShieldBase,
    atPhysicsShieldPercent,
    atPhysicsShieldAdditional,

    atPhysicsReflection,
    atPhysicsReflectionPercent,

    atPhysicsBlock,  

    atPhysicsDamageAbsorb,
    atPhysicsDamageManaShield,
    // ------------- �ڹ� -------------------------------------------------->
    atSpunkAttackPower,
    atMeleeSpunkAttackPower,
    atRangeSpunkAttackPower,

    // ------------- �����ڹ� ---------------------------------------------->
    atSkillSolarDamage,
    atSkillSolarDamageRand,
    atSkillSolarDamagePercent,
    atSolarAttackPowerBase,
    atSolarDamageCoefficient,

    atSolarMagicHit,
    atSolarCriticalStrike,
    atSolarMagicResistPercent,
    atSolarMagicShield,
    atSolarMagicReflection,
    atSolarMagicReflectionPercent,
    atSolarMagicBlock,
    atSolarDamageAbsorb,
    atSolarDamageManaShield,

    // ------------- �����ڹ� ---------------------------------------------->
    atSkillNeutralDamage,
    atSkillNeutralDamageRand,
    atSkillNeutralDamagePercent,
    atNeutralAttackPowerBase,
    atNeutralDamageCoefficient,

    atNeutralMagicHit,
    atNeutralCriticalStrike,
    atNeutralMagicResistPercent,
    atNeutralMagicShield,
    atNeutralMagicReflection,
    atNeutralMagicReflectionPercent,
    atNeutralMagicBlock,
    atNeutralDamageAbsorb,
    atNeutralDamageManaShield,

    // ------------- �����ڹ� ---------------------------------------------->
    atSkillLunarDamage,
    atSkillLunarDamageRand,
    atSkillLunarDamagePercent,
    atLunarAttackPowerBase,
    atLunarDamageCoefficient,

    atLunarMagicHit,
    atLunarCriticalStrike,
    atLunarMagicResistPercent,
    atLunarMagicShield,
    atLunarMagicReflection,
    atLunarMagicReflectionPercent,
    atLunarMagicBlock,
    atLunarDamageAbsorb,
    atLunarDamageManaShield,

    // ------------- ���� -------------------------------------------------->
    atSkillPoisonDamage,
    atSkillPoisonDamageRand,
    atSkillPoisonDamagePercent,
    atPoisonAttackPowerBase,
    atPoisonDamageCoefficient,

    atPoisonMagicHit,
    atPoisonCriticalStrike,
    atPoisonMagicResistPercent,
    atPoisonMagicShield,
    atPoisonMagicReflection,
    atPoisonMagicReflectionPercent,
    atPoisonMagicBlock,
    atPoisonDamageAbsorb,
    atPoisonDamageManaShield,

    // ------------- ���� -------------------------------------------------->
    atTherapyPowerBase,    
    atSkillTherapy,
    atSkillTherapyRand,
    atSkillTherapyPercent, 
    atTherapyCoefficient,

    // ------------- ���ּ����й� ------------------------------------------>
    atNoneWeaponAttackSpeedBase,
    atMeleeWeaponAttackSpeedBase,
    atMeleeWeaponAttackSpeedPercentAdd,
    atMeleeWeaponAttackSpeedAdditional,

    atRangeWeaponAttackSpeedBase,
    atRangeWeaponAttackSpeedPercentAdd,
    atRangeWeaponAttackSpeedAdditional,

    atIgnoreOtherAlarmRange,
    atSelfAlarmRangeRevise,     
    atOtherAlarmRangeRevise,

    // -------------- ����״̬��ص����� -------------------------------->
    atMeditate,     // ����
    atDash,         // ���
    atStun,         // ����, ѣ��
    atHalt,         // �����������ﲻ���ƶ�������������������ʹ�õ��ߵ�  
    atRevive,       // ����
    atJump,         // �ɼ��ܷ������Ծ(�������ֿո������������Ծ)
    atSit,          // �м��ܷ��������
    // ------------ ���� ---------------------------------------------------->
    atCallKnockedBackExhale,
    atCallKnockedBackConvergence,  
    atCallKnockedBackRandom,
    atCallKnockedBackSrcFace,
    
    // ------------- ���� --------------------------------------------------->
    atCallKnockedOffParabola,
    // ------------- �������� ------------------------------------------------>
    atCallTherapy, 
    atCallPhysicsDamage,
    atCallSolarDamage,
    atCallNeutralDamage,
    atCallLunarDamage,
    atCallPoisonDamage,
    atCastSkillTargetSrc,
    atCastSkillTargetDst,

    atSkillEventHandler,
    
    // ------------ ����ļ���ħ������ --------------------------------------->
    atPracticeKungfu,       // �����书

    // ------------ Buff �����й� -------------------------------------------->
    atCallBuff,
    atCallDebuff,
    atCallDot,
    atCallHot,

    // ------------ Buff ��ɢ ------------------------------------------------>
    atDetachSingleBuff,
    atDetachSingleDebuff,
    atDetachSingleGroupDot,
    atDetachSingleGroupHot,
    
    atDetachMultiGroupBuff,
    atDetachMultiGroupDebuff,
    atDetachMultiGroupDot,
    atDetachMultiGroupHot,

    // ------------ ��� ----------------------------------------------------->
    atModifyThreat,
    
    // ------------ ִ�нű� ------------------------------------------------->
    atExecuteScript,

	atTotal
};

enum REQUIRE_TYPE
{
	rqtInvalid = 0,

	rqtVitality,
	rqtStrength,
	rqtSpirit,
	rqtAgility,
	rqtLevel,
	rqtFaction,
	rqtGender,
	rqtBodytype,

	rqtTotal,
};

enum ATTRIBUTE_EFFECT_MODE
{
    aemInvalid,
    aemEffectToSelfAndRollback,
    aemEffectToSelfNotRollback,
    aemEffectToDestAndRollback,
    aemEffectToDestNotRollback,
    aemTotal
};


struct KAttribData
{
	int				nKey;
	int				nValue1;
	int				nValue2;
};

struct KAttribute : KAttribData
{
	KAttribute*		pNext;
};

#define ADD_ATTRIBUTE_TO_END(pList, pNode)  \
    do                                      \
    {                                       \
        if (!pList)                         \
        {                                   \
            pList = pNode;                  \
            pNode->pNext = NULL;            \
        }                                   \
        else                                \
        {                                   \
            KAttribute *_pNode = pList;     \
            while (_pNode->pNext)           \
            {                               \
                _pNode = _pNode->pNext;     \
            }                               \
            _pNode->pNext = pNode;          \
            pNode->pNext = NULL;            \
        }                                   \
    }                                       \
    while (false)

#define FREE_ATTRIB_LIST(pAttrib)                           \
    do                                                      \
    {                                                       \
        while (pAttrib)                                     \
        {                                                   \
            KAttribute *pTmp = pAttrib;                     \
            pAttrib = pAttrib->pNext;                       \
            g_ObjectManager.m_AttributeSet.Delete(pTmp);    \
            pTmp = NULL;                                    \
        }                                                   \
    } while (false)


#define MAKE_IN_RANGE(Value, MinValue, MaxValue)        \
    do                                                  \
    {                                                   \
        ASSERT(MinValue <= MaxValue);                   \
        if ((Value) > (MaxValue)) (Value) = (MaxValue); \
        if ((Value) < (MinValue)) (Value) = (MinValue); \
    }                                                   \
    while (false)



BOOL AttributeStringToID(LPCTSTR szAttribute, int& nID);
BOOL RequireStringToID(LPCTSTR Require, int& nID);

#endif	//_KATTRIBUTE_H_