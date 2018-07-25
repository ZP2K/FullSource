#include "stdafx.h"
#include "KAttribute.h"
#include "StrMap.h"

// �ַ���ӳ����������ñ��м�����ֱ����д�ַ�������������ö�ٵ�����
DECLARE_STRING_MAP_BEGIN(ATTRIBUTE_TYPE)

	REGISTER_MAP_VALUE(atInvalid)

    // ------------- �ƶ���� ------------------------------------------>
	REGISTER_MAP_VALUE(atWalkSpeedBase)
	REGISTER_MAP_VALUE(atRunSpeedBase)
    REGISTER_MAP_VALUE(atSwimSpeedBase)
	REGISTER_MAP_VALUE(atMoveSpeedPercent)
	REGISTER_MAP_VALUE(atMaxJumpCount)
    REGISTER_MAP_VALUE(atGravityBase)
    REGISTER_MAP_VALUE(atGravityPercent)
    REGISTER_MAP_VALUE(atJumpSpeedXYBase)
    REGISTER_MAP_VALUE(atJumpSpeedXYPercent)
    REGISTER_MAP_VALUE(atJumpSpeedZBase)
    REGISTER_MAP_VALUE(atJumpSpeedZPercent)
	REGISTER_MAP_VALUE(atDropDefence)
    REGISTER_MAP_VALUE(atDivingFrameBase)
    REGISTER_MAP_VALUE(atDivingFramePercent) 
    REGISTER_MAP_VALUE(atSwimAbility)
    REGISTER_MAP_VALUE(atWaterFlyAbility)


    // ------------- Ǳ�� ---------------------------------------------->

	REGISTER_MAP_VALUE(atStrengthBase)
    REGISTER_MAP_VALUE(atStrengthBasePercentAdd)
	REGISTER_MAP_VALUE(atAgilityBase)
    REGISTER_MAP_VALUE(atAgilityBasePercentAdd)
	REGISTER_MAP_VALUE(atVitalityBase)
    REGISTER_MAP_VALUE(atVitalityBasePercentAdd)
	REGISTER_MAP_VALUE(atSpiritBase)
    REGISTER_MAP_VALUE(atSpiritBasePercentAdd)
    REGISTER_MAP_VALUE(atSpunkBase)
    REGISTER_MAP_VALUE(atSpunkBasePercentAdd)

    // ------------- ���� ---------------------------------------------->

	REGISTER_MAP_VALUE(atCurrentLife)				

    REGISTER_MAP_VALUE(atMaxLifeBase)              
	REGISTER_MAP_VALUE(atMaxLifePercentAdd)		
    REGISTER_MAP_VALUE(atMaxLifeAdditional)      

    REGISTER_MAP_VALUE(atLifeReplenish)
    REGISTER_MAP_VALUE(atLifeReplenishPercent)		

    // ------------- ���� ---------------------------------------------->

	REGISTER_MAP_VALUE(atCurrentMana)				

	REGISTER_MAP_VALUE(atMaxManaBase)			
    REGISTER_MAP_VALUE(atMaxManaPercentAdd)			
    REGISTER_MAP_VALUE(atMaxManaAdditional)

    REGISTER_MAP_VALUE(atManaReplenish)
    REGISTER_MAP_VALUE(atManaReplenishPercent)

    // ------------- ���� ---------------------------------------------->

	REGISTER_MAP_VALUE(atCurrentStamina)				

    REGISTER_MAP_VALUE(atMaxStaminaBase)              
	REGISTER_MAP_VALUE(atMaxStaminaPercentAdd)		
    REGISTER_MAP_VALUE(atMaxStaminaAdditional)      

    REGISTER_MAP_VALUE(atStaminaReplenish)
    REGISTER_MAP_VALUE(atStaminaReplenishPercent)	

    REGISTER_MAP_VALUE(atQiToSkillExpPercentAdd)

    // ------------- ŭ�� ---------------------------------------------->

	REGISTER_MAP_VALUE(atCurrentRage)				

    REGISTER_MAP_VALUE(atMaxRageBase)              
	REGISTER_MAP_VALUE(atMaxRagePercentAdd)		
    REGISTER_MAP_VALUE(atMaxRageAdditional)      

    REGISTER_MAP_VALUE(atRageReplenish)
    REGISTER_MAP_VALUE(atRageReplenishPercent)		

    // ------------- ��� ---------------------------------------------->
    REGISTER_MAP_VALUE(atGainPositiveThreatCoefficient)
    REGISTER_MAP_VALUE(atGainNegativeThreatCoefficient)
    REGISTER_MAP_VALUE(atImpairPositiveThreatCoefficient)
    REGISTER_MAP_VALUE(atImpairNegativeThreatCoefficient)

    // ------------- ս������ ------------------------------------------>
    REGISTER_MAP_VALUE(atDodge)
    REGISTER_MAP_VALUE(atPositiveShield)
    REGISTER_MAP_VALUE(atNegativeShield)
    REGISTER_MAP_VALUE(atGlobalBlock)
    REGISTER_MAP_VALUE(atGlobalDamageAbsorb)
    REGISTER_MAP_VALUE(atGlobalDamageManaShield)
    REGISTER_MAP_VALUE(atStealLifePercent)
    REGISTER_MAP_VALUE(atStealManaPercent)
    REGISTER_MAP_VALUE(atBeatBackRate)
    REGISTER_MAP_VALUE(atKnockedBackRate)
    REGISTER_MAP_VALUE(atKnockedOffRate)

    REGISTER_MAP_VALUE(atPhysicsAttackPower)
    REGISTER_MAP_VALUE(atMagicAttackPower)

    REGISTER_MAP_VALUE(atMeleeWeaponDamageBase)
    REGISTER_MAP_VALUE(atMeleeWeaponDamageRand)

    REGISTER_MAP_VALUE(atRangeWeaponDamageBase)
    REGISTER_MAP_VALUE(atRangeWeaponDamageRand)

    // ------------- �⹦ ---------------------------------------------->

    REGISTER_MAP_VALUE(atSkillPhysicsDamage)
    REGISTER_MAP_VALUE(atSkillPhysicsDamageRand)
    REGISTER_MAP_VALUE(atSkillPhysicsDamagePercent)
    REGISTER_MAP_VALUE(atPhysicsAttackPowerBase)
    REGISTER_MAP_VALUE(atPhysicsDamageCoefficient)


    REGISTER_MAP_VALUE(atPhysicsAttackHit)
    REGISTER_MAP_VALUE(atPhysicsCriticalStrike)

    REGISTER_MAP_VALUE(atPhysicsResistPercent)

    REGISTER_MAP_VALUE(atPhysicsDefenceAdd)
    REGISTER_MAP_VALUE(atPhysicsDefenceMax)

    REGISTER_MAP_VALUE(atPhysicsShieldBase)
    REGISTER_MAP_VALUE(atPhysicsShieldPercent)
    REGISTER_MAP_VALUE(atPhysicsShieldAdditional)

    REGISTER_MAP_VALUE(atPhysicsReflection)
    REGISTER_MAP_VALUE(atPhysicsReflectionPercent)

    REGISTER_MAP_VALUE(atPhysicsBlock)  

    REGISTER_MAP_VALUE(atPhysicsDamageAbsorb)
    REGISTER_MAP_VALUE(atPhysicsDamageManaShield)

    // ------------- �ڹ� ------------------------------------------------->
    REGISTER_MAP_VALUE(atSpunkAttackPower)
    REGISTER_MAP_VALUE(atMeleeSpunkAttackPower)
    REGISTER_MAP_VALUE(atRangeSpunkAttackPower)

    // ------------- �����ڹ� ---------------------------------------------->
    REGISTER_MAP_VALUE(atSkillSolarDamage)
    REGISTER_MAP_VALUE(atSkillSolarDamageRand)
    REGISTER_MAP_VALUE(atSkillSolarDamagePercent)
    REGISTER_MAP_VALUE(atSolarAttackPowerBase)
    REGISTER_MAP_VALUE(atSolarDamageCoefficient)

    REGISTER_MAP_VALUE(atSolarMagicHit)
    REGISTER_MAP_VALUE(atSolarCriticalStrike)
    REGISTER_MAP_VALUE(atSolarMagicResistPercent)
    REGISTER_MAP_VALUE(atSolarMagicShield)
    REGISTER_MAP_VALUE(atSolarMagicReflection)
    REGISTER_MAP_VALUE(atSolarMagicReflectionPercent)
    REGISTER_MAP_VALUE(atSolarMagicBlock)
    REGISTER_MAP_VALUE(atSolarDamageAbsorb)
    REGISTER_MAP_VALUE(atSolarDamageManaShield)

    // ------------- �����ڹ� ---------------------------------------------->
    REGISTER_MAP_VALUE(atSkillNeutralDamage)
    REGISTER_MAP_VALUE(atSkillNeutralDamageRand)
    REGISTER_MAP_VALUE(atSkillNeutralDamagePercent)
    REGISTER_MAP_VALUE(atNeutralAttackPowerBase)
    REGISTER_MAP_VALUE(atNeutralDamageCoefficient)

    REGISTER_MAP_VALUE(atNeutralMagicHit)
    REGISTER_MAP_VALUE(atNeutralCriticalStrike)
    REGISTER_MAP_VALUE(atNeutralMagicResistPercent)
    REGISTER_MAP_VALUE(atNeutralMagicShield)
    REGISTER_MAP_VALUE(atNeutralMagicReflection)
    REGISTER_MAP_VALUE(atNeutralMagicReflectionPercent)
    REGISTER_MAP_VALUE(atNeutralMagicBlock)
    REGISTER_MAP_VALUE(atNeutralDamageAbsorb)
    REGISTER_MAP_VALUE(atNeutralDamageManaShield)

    // ------------- �����ڹ� ---------------------------------------------->
    REGISTER_MAP_VALUE(atSkillLunarDamage)
    REGISTER_MAP_VALUE(atSkillLunarDamageRand)
    REGISTER_MAP_VALUE(atSkillLunarDamagePercent)
    REGISTER_MAP_VALUE(atLunarAttackPowerBase)
    REGISTER_MAP_VALUE(atLunarDamageCoefficient)

    REGISTER_MAP_VALUE(atLunarMagicHit)
    REGISTER_MAP_VALUE(atLunarCriticalStrike)
    REGISTER_MAP_VALUE(atLunarMagicResistPercent)
    REGISTER_MAP_VALUE(atLunarMagicShield)
    REGISTER_MAP_VALUE(atLunarMagicReflection)
    REGISTER_MAP_VALUE(atLunarMagicReflectionPercent)
    REGISTER_MAP_VALUE(atLunarMagicBlock)
    REGISTER_MAP_VALUE(atLunarDamageAbsorb)
    REGISTER_MAP_VALUE(atLunarDamageManaShield)

    // ------------- ���� -------------------------------------------------->
    REGISTER_MAP_VALUE(atSkillPoisonDamage)
    REGISTER_MAP_VALUE(atSkillPoisonDamageRand)
    REGISTER_MAP_VALUE(atSkillPoisonDamagePercent)
    REGISTER_MAP_VALUE(atPoisonAttackPowerBase)
    REGISTER_MAP_VALUE(atPoisonDamageCoefficient)

    REGISTER_MAP_VALUE(atPoisonMagicHit)
    REGISTER_MAP_VALUE(atPoisonCriticalStrike)
    REGISTER_MAP_VALUE(atPoisonMagicResistPercent)
    REGISTER_MAP_VALUE(atPoisonMagicShield)
    REGISTER_MAP_VALUE(atPoisonMagicReflection)
    REGISTER_MAP_VALUE(atPoisonMagicReflectionPercent)
    REGISTER_MAP_VALUE(atPoisonMagicBlock)
    REGISTER_MAP_VALUE(atPoisonDamageAbsorb)
    REGISTER_MAP_VALUE(atPoisonDamageManaShield)

    // ------------- ���� -------------------------------------------------->
    REGISTER_MAP_VALUE(atTherapyPowerBase)    
    REGISTER_MAP_VALUE(atSkillTherapy)
    REGISTER_MAP_VALUE(atSkillTherapyRand)
    REGISTER_MAP_VALUE(atSkillTherapyPercent) 
    REGISTER_MAP_VALUE(atTherapyCoefficient)

    // ------------- ���ּ����й� ------------------------------------------->
    REGISTER_MAP_VALUE(atNoneWeaponAttackSpeedBase)
    REGISTER_MAP_VALUE(atMeleeWeaponAttackSpeedBase)
    REGISTER_MAP_VALUE(atMeleeWeaponAttackSpeedPercentAdd)
    REGISTER_MAP_VALUE(atMeleeWeaponAttackSpeedAdditional)

    REGISTER_MAP_VALUE(atRangeWeaponAttackSpeedBase)
    REGISTER_MAP_VALUE(atRangeWeaponAttackSpeedPercentAdd)
    REGISTER_MAP_VALUE(atRangeWeaponAttackSpeedAdditional)

    // ----------------------------------------------------------------------
    REGISTER_MAP_VALUE(atIgnoreOtherAlarmRange)
    REGISTER_MAP_VALUE(atSelfAlarmRangeRevise)
    REGISTER_MAP_VALUE(atOtherAlarmRangeRevise)

    // ---------------- ����״̬��ص����� --------------------------------->
    REGISTER_MAP_VALUE(atMeditate)
    REGISTER_MAP_VALUE(atDash)      // ���
    REGISTER_MAP_VALUE(atStun)      // ����, ѣ��
    REGISTER_MAP_VALUE(atHalt)      // �����������ﲻ���ƶ�������������������ʹ�õ��ߵ�  
    REGISTER_MAP_VALUE(atRevive)
    REGISTER_MAP_VALUE(atJump)
    REGISTER_MAP_VALUE(atSit)

    // ------------- ���� -------------------------------------------------->
    REGISTER_MAP_VALUE(atCallKnockedBackExhale)
    REGISTER_MAP_VALUE(atCallKnockedBackConvergence) 
    REGISTER_MAP_VALUE(atCallKnockedBackRandom)
    REGISTER_MAP_VALUE(atCallKnockedBackSrcFace)

    // ------------- ���� -------------------------------------------------->
    REGISTER_MAP_VALUE(atCallKnockedOffParabola)

    // ------------- �������� ------------------------------------------------>
    REGISTER_MAP_VALUE(atCallTherapy) 
    REGISTER_MAP_VALUE(atCallPhysicsDamage)
    REGISTER_MAP_VALUE(atCallSolarDamage)
    REGISTER_MAP_VALUE(atCallNeutralDamage)
    REGISTER_MAP_VALUE(atCallLunarDamage)
    REGISTER_MAP_VALUE(atCallPoisonDamage)
    REGISTER_MAP_VALUE(atCastSkillTargetSrc)
    REGISTER_MAP_VALUE(atCastSkillTargetDst)
    REGISTER_MAP_VALUE(atSkillEventHandler)

    // ------------ ����ļ���ħ������ --------------------------------------->
    REGISTER_MAP_VALUE(atPracticeKungfu)

    // ------------ Buff �����й� -------------------------------------------->
    REGISTER_MAP_VALUE(atCallBuff)
    REGISTER_MAP_VALUE(atCallDebuff)
    REGISTER_MAP_VALUE(atCallDot)
    REGISTER_MAP_VALUE(atCallHot)

    // ------------- Buff ��ɢ ---------------------------------------------->
    REGISTER_MAP_VALUE(atDetachSingleBuff)
    REGISTER_MAP_VALUE(atDetachSingleDebuff)
    REGISTER_MAP_VALUE(atDetachSingleGroupDot)
    REGISTER_MAP_VALUE(atDetachSingleGroupHot)

    REGISTER_MAP_VALUE(atDetachMultiGroupBuff)
    REGISTER_MAP_VALUE(atDetachMultiGroupDebuff)
    REGISTER_MAP_VALUE(atDetachMultiGroupDot)
    REGISTER_MAP_VALUE(atDetachMultiGroupHot)

    // ------------- ��� --------------------------------------------------->
    REGISTER_MAP_VALUE(atModifyThreat)

    // ------------- ִ�нű� ----------------------------------------------->
    REGISTER_MAP_VALUE(atExecuteScript)

DECLARE_STRING_MAP_END(ATTRIBUTE_TYPE)

DECLARE_STRING_MAP_BEGIN(REQUIRE_TYPE)

	REGISTER_MAP_VALUE(rqtInvalid)

	REGISTER_MAP_VALUE(rqtVitality)
	REGISTER_MAP_VALUE(rqtStrength)
	REGISTER_MAP_VALUE(rqtSpirit)
	REGISTER_MAP_VALUE(rqtAgility)
	REGISTER_MAP_VALUE(rqtLevel)
	REGISTER_MAP_VALUE(rqtFaction)
	REGISTER_MAP_VALUE(rqtGender)
	REGISTER_MAP_VALUE(rqtBodytype)

	REGISTER_MAP_VALUE(rqtTotal)

DECLARE_STRING_MAP_END(REQUIRE_TYPE)



BOOL AttributeStringToID(LPCTSTR szAttribute, int& nID)
{
	MAP_STRING_EXTERN(ATTRIBUTE_TYPE, szAttribute, nID);

	return TRUE;
Exit0:
	nID = 0;
	return FALSE;
}

BOOL RequireStringToID(LPCTSTR Require, int& nID)
{
	MAP_STRING_EXTERN(REQUIRE_TYPE, Require, nID);

	return TRUE;
Exit0:
	nID = 0;
	return FALSE;
}

