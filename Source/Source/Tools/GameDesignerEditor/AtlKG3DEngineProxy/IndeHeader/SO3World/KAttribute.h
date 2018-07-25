/************************************************************************/
/* Ӱ���������������					                            */
/* Copyright : Kingsoft 2005										    */
/* Author	 : Zhu Jianqiu												*/
/* History	 :															*/
/*		2005.01.18	Create												*/
/************************************************************************/
#ifndef _KATTRIBUTE_H_
#define _KATTRIBUTE_H_
#include "Engine/KMemory.h"

enum ATTRIBUTE_TYPE
{
	atInvalid = 0,

    // ------------- �ƶ���� ------------------------------------------>
    atRunSpeedBase,
    atMoveSpeedPercent,
    atMaxJumpCount,
    atGravityBase,
    atGravityPercent,
    atJumpSpeedBase,
    atJumpSpeedPercent,
    atDropDefence,
    atDivingFrameBase,
    atDivingFramePercent, 
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
    atTransmitTrain,
    atMaxTrainValue,
    atBaseVenationCof,
    atPhysicsVenationCof,
    atMagicVenationCof,
    atAssistVenationCof,
    atBasePotentialAdd,

    // ------------- ���� ---------------------------------------------->
	atCurrentLife,

    atMaxLifeBase,
	atMaxLifePercentAdd,
    atMaxLifeAdditional,

    atLifeReplenish,
    atLifeReplenishPercent,
    atLifeReplenishCoefficient,
    atLifeReplenishExt,

    // ------------- ���� ---------------------------------------------->
	atCurrentMana,				

	atMaxManaBase,			
    atMaxManaPercentAdd,			
    atMaxManaAdditional,

    atManaReplenish,
    atManaReplenishPercent,
    atManaReplenishCoefficient,
    atManaReplenishExt,

    // ------------- ���� ---------------------------------------------->
	atCurrentStamina,				
    
    // ------------- ���� ---------------------------------------------->
    atCurrentThew,				

    // -------------- ��� --------------------------------------------->
    atActiveThreatCoefficient,
    atPassiveThreatCoefficient,

    // ------------- ս������ ------------------------------------------>
    atDodge,
    atDodgeBaseRate,
    atParryBaseRate,
    atParryBase,
    atParryPercent,
    atParryValueBase,
    atParryValuePercent,
    atSense,
    atStrainBase,
    atStrainPercent,
    atToughnessBaseRate,
    atToughnessBase,
    atToughnessPercent,
    atCriticalDamagePowerBaseKiloNumRate,
    atCriticalDamagePowerBase,
    atCriticalDamagePowerPercent,
    atDecriticalDamagePowerBaseKiloNumRate,
    atDecriticalDamagePowerBase,
    atDecriticalDamagePowerPercent,
    atPositiveShield,
    atNegativeShield,
    atGlobalResistPercent,
    atGlobalBlock,
    atGlobalDamageAbsorb,
    atGlobalDamageManaShield,
    atDamageToLifeForSelf,
    atDamageToManaForSelf,
    atBeatBackRate,
    atKnockedBackRate,
    atKnockedOffRate,

    atImmortal,
    atMountingRate,
    
    atMeleeWeaponDamageBase,
    atMeleeWeaponDamageRand,

    atRangeWeaponDamageBase,
    atRangeWeaponDamageRand,

    // ------------- �⹦ ---------------------------------------------->
    atSkillPhysicsDamage,
    atSkillPhysicsDamageRand,
    atSkillPhysicsDamagePercent,
    atPhysicsDamageCoefficient,

    atPhysicsAttackPowerBase,
    atPhysicsAttackPowerPercent,
    
    atPhysicsHitBaseRate,
    atPhysicsHitValue,
    atPhysicsCriticalStrikeBaseRate,
    atPhysicsCriticalStrike,

    atPhysicsOvercomeBase,
    atPhysicsOvercomePercent,

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

    // ------------- �����ڹ� ---------------------------------------------->
    atSkillMagicDamage,
    atMagicAttackPowerBase,
    atMagicAttackPowerPercent,
    atMagicCriticalStrike,
    atMagicShield,
    atMagicOvercome,

    // ------------- �����ڹ� ---------------------------------------------->
    atSkillSolarDamage,
    atSkillSolarDamageRand,
    atSkillSolarDamagePercent,
    atSolarDamageCoefficient,

    atSolarAttackPowerBase,
    atSolarAttackPowerPercent,

    atSolarHitBaseRate,
    atSolarHitValue,
    atSolarCriticalStrikeBaseRate,
    atSolarCriticalStrike,
    atSolarOvercomeBase,
    atSolarOvercomePercent,
    atSolarMagicResistPercent,
    atSolarMagicShieldBase,
    atSolarMagicShieldPercent,
    atSolarMagicReflection,
    atSolarMagicReflectionPercent,
    atSolarMagicBlock,
    atSolarDamageAbsorb,
    atSolarDamageManaShield,

    // ------------- �����ڹ� ---------------------------------------------->
    atSkillNeutralDamage,
    atSkillNeutralDamageRand,
    atSkillNeutralDamagePercent,
    atNeutralDamageCoefficient,

    atNeutralAttackPowerBase,
    atNeutralAttackPowerPercent,

    atNeutralHitBaseRate,
    atNeutralHitValue,
    atNeutralCriticalStrikeBaseRate,
    atNeutralCriticalStrike,
    atNeutralOvercomeBase,
    atNeutralOvercomePercent,
    atNeutralMagicResistPercent,
    atNeutralMagicShieldBase,
    atNeutralMagicShieldPercent,
    atNeutralMagicReflection,
    atNeutralMagicReflectionPercent,
    atNeutralMagicBlock,
    atNeutralDamageAbsorb,
    atNeutralDamageManaShield,

    // ------------- �����ڹ� ---------------------------------------------->
    atSkillLunarDamage,
    atSkillLunarDamageRand,
    atSkillLunarDamagePercent,
    atLunarDamageCoefficient,

    atLunarAttackPowerBase,
    atLunarAttackPowerPercent,

    atLunarHitBaseRate,
    atLunarHitValue,
    atLunarCriticalStrikeBaseRate,
    atLunarCriticalStrike,
    atLunarOvercomeBase,
    atLunarOvercomePercent,
    atLunarMagicResistPercent,
    atLunarMagicShieldBase,
    atLunarMagicShieldPercent,
    atLunarMagicReflection,
    atLunarMagicReflectionPercent,
    atLunarMagicBlock,
    atLunarDamageAbsorb,
    atLunarDamageManaShield,

    // ------------- ���� -------------------------------------------------->
    atSkillPoisonDamage,
    atSkillPoisonDamageRand,
    atSkillPoisonDamagePercent,
    atPoisonDamageCoefficient,

    atPoisonAttackPowerBase,
    atPoisonAttackPowerPercent,

    atPoisonHitBaseRate,
    atPoisonHitValue,
    atPoisonCriticalStrikeBaseRate,
    atPoisonCriticalStrike,
    atPoisonOvercomeBase,
    atPoisonOvercomePercent,
    atPoisonMagicResistPercent,
    atPoisonMagicShieldBase,
    atPoisonMagicShieldPercent,
    atPoisonMagicReflection,
    atPoisonMagicReflectionPercent,
    atPoisonMagicBlock,
    atPoisonDamageAbsorb,
    atPoisonDamageManaShield,

    // ------------- ���� -------------------------------------------------->
    atTherapyPowerBase,    
    atTherapyPowerPercent,
    atSkillTherapy,
    atSkillTherapyRand,
    atSkillTherapyPercent, 
    atTherapyCoefficient,
    atBeTherapyCoefficient,

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
    atUnarm,                    // ��е

    // -------------- ����״̬��ص����� -------------------------------->
    atDash,         // ���
    atDashForward,
    atDashBackward,
    atDashLeft,
    atDashRight,
    atHalt,         // ����
    atFreeze,       // ����
    atEntrap,       // �����ƶ�
    atFear,         // �־�, ע�⣺��ħ������ֻ������DEBUFF��BUFF����Ϊ�����ϵ�Ҫ�ع���ħ������
    atRevive,       // ����
    atJump,         // �ɼ��ܷ������Ծ(�������ֿո������������Ծ)
    atSit,          // �ɼ��ܷ��������

    // ------------ ���� ---------------------------------------------------->
    atCallKnockedBackExhale,
    atCallKnockedBackConvergence,  
    atCallKnockedBackRandom,
    atCallKnockedBackSrcFace,
    
    // ------------- ���� --------------------------------------------------->
    atCallKnockedOffParabola,
    atCallKnockedDown,

    // ------------- ���� ---------------------------------------------------->
    atCallRepulsed,

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
    atAccumulate,           // ����״̬

    // ------------ Buff �����й� -------------------------------------------->
    atCallBuff,
    atCallBuffAccordingToAttraction,

    // ------------ Buff ��ɢ ------------------------------------------------>
    atDetachSingleBuff,

    atDetachMultiGroupBuff,

    atDelSingleBuffByFunctionType,

    atDelMultiGroupBuffByFunctionType,

    atDelSingleBuffByIDAndLevel,
    atDelSingleGroupBuffByIDAndLevel,

    // ------------ ��� ----------------------------------------------------->
    atModifyThreat,
    atSpoof,
    
    // ------------ ��ħ ----------------------------------------------------->
    atSilence,
    atSilenceAll,

    // ------------ ���� ----------------------------------------------------->
    atImmunity,

    // ------------ ���� ----------------------------------------------------->
    atAddWeak,
    atShiftWeak,
    atAddWeakShield,

    // ------------ ����������־ --------------------------------------------->
    atModifySkillCastFlag,

    // ------------ ִ�нű� ------------------------------------------------->
    atExecuteScript,
    
    // ------------ ��ױ ----------------------------------------------------->
    atPrinkRepresentID,
    atPrinkModel,    
    atPrinkForce,
    
    // ------------ ���� ----------------------------------------------------->
    atRideHorse,

    // ------------ ���� ----------------------------------------------------->
    atDoAction,

    atAddExpPercent,
    atAddReputationPercent,

    atEmployerCastSkillTargetSelf,

    atClearCoolDown,
    atResetCoolDown,

    atTransferDamageToManaPercent,

    atModifyCostManaPercent,

    atDirectCastMask,

    atPull,
    atTurnToSrc,
    atConsumeBuff,
    atTopThreat,

    atFormationEffect,

    atAddDamageByDstMoveState,

    atTransferDamage,
    atTransferTherapy,

    atAddFellowshipAttraction,

    atCallHelp,
    atCallRequest,
    atCatchCharacter,
    atTransmission,

    // ----------------------------
    atVitalityToParryValueCof,
    atVitalityToMaxLifeCof,
    atVitalityToPhysicsAttackPowerCof,
    atVitalityToSolarAttackPowerCof,
    atVitalityToLunarAttackPowerCof,
    atVitalityToNeutralAttackPowerCof,
    atVitalityToPoisonAttackPowerCof,
    atVitalityToTherapyPowerCof,
    atVitalityToPhysicsShieldCof,
    atSpiritToMaxManaCof,
    atSpiritToSolarCriticalStrikeCof,
    atSpiritToLunarCriticalStrikeCof,
    atSpiritToNeutralCriticalStrikeCof,
    atSpiritToPoisonCriticalStrikeCof,
    atSpiritToPhysicsAttackPowerCof,
    atSpiritToPhysicsCriticalStrikeCof,
    atSpiritToManaReplenishCof,
    atSpiritToSolarMagicShieldCof,
    atSpiritToLunarMagicShieldCof,
    atSpiritToNeutralMagicShieldCof,
    atSpiritToPoisonMagicShieldCof,
    atSpunkToManaReplenishCof,
    atSpunkToSolarAttackPowerCof,
    atSpunkToLunarAttackPowerCof,
    atSpunkToNeutralAttackPowerCof,
    atSpunkToPoisonAttackPowerCof,
    atStrengthToPhysicsCriticalStrikeCof,
    atStrengthToPhysicsAttackPowerCof,
    atAgilityToPhysicsCriticalStrikeCof,
    // ----------------------------------

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

	rqtTotal
};

enum ATTRIBUTE_EFFECT_MODE
{
    aemInvalid,
    aemEffectToSelfAndRollback,
    aemEffectToSelfNotRollback,
    aemEffectToDestAndRollback,
    aemEffectToDestNotRollback,
    aemEffectToDestOnMinorWeak,
    aemEffectToDestOnMajorWeak,
    aemEffectToDestOnMaxWeak,
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
        if (pList == NULL)                  \
        {                                   \
            pList = pNode;                  \
        }                                   \
        else                                \
        {                                   \
            KAttribute *_pNode = pList;     \
            while (_pNode->pNext)           \
            {                               \
                _pNode = _pNode->pNext;     \
            }                               \
            _pNode->pNext = pNode;          \
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
            KMemory::Delete(pTmp);                      \
            pTmp = NULL;                                    \
        }                                                   \
    } while (false)


BOOL AttributeStringToID(LPCTSTR szAttribute, int& nID);
BOOL RequireStringToID(LPCTSTR Require, int& nID);

KAttribute* CloneAttributeList(const KAttribute* pHead);
BOOL CloneAttribListToEnd(KAttribute** ppDestAttribList, KAttribute* pCloneAttribList);

#define KATTRIBUTE_STRING_LEN                   64

#endif	//_KATTRIBUTE_H_
