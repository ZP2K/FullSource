/***************************************************
Filename   :   KCommon.h
CreatedBy  :   Monkey(Hou Rui)
CreatedOn  :   2008��2��12��
Description:   ��ɫ����DLL�ṩ���ⲿ�Ľӿ�
**************************************************/

#ifndef KCOMMON_H
#define KCOMMON_H

#ifdef JX3ROLEANALYSER_EXPORTS
#ifdef WIN32
#define DLLDECL __declspec(dllexport)
#else
#define DLLDECL
#endif

#else

#ifdef WIN32
#define DLLDECL __declspec(dllexport)
#else
#define DLLDECL
#endif

#endif

#ifdef WIN32
#include <windows.h>
#endif

typedef void* PARSEHANDLE;

#ifdef __cplusplus
extern "C" {
#endif

//��Դ����������ɫ��Ϣ�ӿ�ʹ���������ñ�����
DLLDECL void EndRoleParse(PARSEHANDLE& hHandle);
//��Դ���������ʼ���Ϣ�ӿ�ʹ���������ñ�����
DLLDECL void EndMailBoxParse(PARSEHANDLE& hHandle);
//��Դ���������ʼ���Ϣ�ӿ�ʹ���������ñ�����
DLLDECL void EndFellowshipParse(PARSEHANDLE& hHandle);
//��Դ����������������Ϣ�ӿ�ʹ���������ñ�����
DLLDECL void EndAuctionParse(PARSEHANDLE& hHandle);
//��Դ�������������Ϣ�ӿ�ʹ���������ñ�����
DLLDECL void EndTongParse(PARSEHANDLE& hHandle);
//��Դ������������ͳ�ƽӿ�ʹ���������ñ�����
DLLDECL void EndStatDataParse(PARSEHANDLE& hHandle);
//��Դ������,QP��ӿ�ʹ���������ñ�����
DLLDECL void EndPQDataParse(PARSEHANDLE& hHandle);
//��Դ������,Mentor��ӿ�ʹ���������ñ�����
DLLDECL void EndMentorPares(PARSEHANDLE& hHandle);
//******************************************************************************
// ����Ϊ��ɫ������Ϣ��ȡ�ӿ�
//******************************************************************************
//������ɫ�������ݵĻ������������µ�Get�ӿ�ʹ��ǰ������ñ�����
DLLDECL PARSEHANDLE ParseRoleBaseData(unsigned char* pbyData, size_t uDataLen);
//��ȡ��ɫ����
DLLDECL int GetRoleType(PARSEHANDLE hParser, char& cRoleType);
//��ȡ��ɫ���ڵ�ͼ����ǰ����
DLLDECL int GetRolePosition(
                            PARSEHANDLE hParser, unsigned long& dwMapID, int& nMapCopyIndex,
                            int& nX, int& nY, int& nZ, unsigned char& byFaceDirection
);
//��ȡ��ɫ��һ�������
DLLDECL int GetLastEntry(
                         PARSEHANDLE hParser, unsigned long& dwMapID, int& nMapCopyIndex,
                         int& nX, int& nY, int& nZ, unsigned char& byFaceDirection
);
//��ȡ��ɫ�ȼ�
DLLDECL int GetRoleLevel(PARSEHANDLE hParser, int& nRoleLevel);
//��ȡ��ɫ����ID����
//����˵��:unsigned short pdwRepresentId[] ҪΪָ���СΪ27��unsigned short����, int& nSize �����õ�����Ĵ�С;
DLLDECL int GetRepresentID(PARSEHANDLE hParser, unsigned short RepresentIDArray[], int& nSize);
//��ȡ��ɫ�ϴδ���ʱ��
DLLDECL int GetLastSaveTime(PARSEHANDLE hParser, time_t& nLastSaveTime);
//��ȡ��ɫ�ϴε�¼ʱ��
DLLDECL int GetLastLoginTime(PARSEHANDLE hParser, time_t& nLastLoginTime);
//��ȡ��ɫ�ܵ���Ϸʱ��
DLLDECL int GetTotalGameTime(PARSEHANDLE hParser, time_t& nTotalGameTime);
//��ȡ��ɫ����Ӫ
DLLDECL int GetCamp(PARSEHANDLE hParser, int& nCamp);
//��ȡ��ɫ������
DLLDECL int GetForceID(PARSEHANDLE hParser, unsigned long& dwForceID);
//��ȡ��ɫ����ʱ��
DLLDECL int GetCreateTime(PARSEHANDLE hParser, time_t& nCreateTime);

//������ɫ��չ���ݵĻ������������µ�Get�ӿ�ʹ��ǰ������ñ�����
DLLDECL PARSEHANDLE ParseRoleExtData(unsigned char* pbyData, size_t uDataLen);
//******************************************************************************
// ����Ϊ������Ϣ��ȡ�ӿ�
//******************************************************************************
// �ж�ĳ�����ݿ��Ƿ����;
DLLDECL int IsDataBlockExist(PARSEHANDLE hParser, int nDataBlockIndex, bool& bIsDataBlockExist);
//��ȡ��ɫװ���ڹ���ID
DLLDECL int GetMountKungfuID(PARSEHANDLE hParser, unsigned long& dwMountKungfuID);
//��ȡ��ɫװ���ڹ���Level
DLLDECL int GetMountKungfuLevel(PARSEHANDLE hParser, unsigned long& dwMountKungfuLevel);
//��ȡ��ɫ�ļ�������
DLLDECL int GetSkillCount(PARSEHANDLE hParser, int& nSkillCount);
//��ȡ��ɫ�ļ�����Ϣ(�����Ƿ��ص���������Ϣ����Ҫ��ȡ��ɫ�����б�Ļ�����Ҫѭ�����ñ�����)
//����˵��: unsigned long dwSkillIndex  ����������, unsigned short& wSkillID ����ID, unsigned char& byMaxLevel ������ߵȼ�, unsigned long& dwExp ���ܵ�ǰ����ֵ
DLLDECL int GetSkillItem(PARSEHANDLE hParser, unsigned long dwSkillIndex, unsigned short& wSkillID, unsigned char& byMaxLevel, unsigned long& dwExp);


//******************************************************************************
// ����Ϊ������Ϣ��ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫ����������
DLLDECL int GetSkillRecipeCount(PARSEHANDLE hParser, int& nkillRecipeCount);
//��ȡ��ɫ��������Ϣ(�����Ƿ��ص���������Ϣ����Ҫ��ȡ��ɫ�����б�Ļ�����Ҫѭ�����ñ�����)
//����˵��: unsigned long dwSkillRecipeIndex ����������, unsigned long& dwRecipeKey ���ű��, unsigned char& byActive �����Ƿ񼤻�
DLLDECL int GetSkillRecipe(PARSEHANDLE hParser, unsigned long dwSkillRecipeIndex, unsigned long& dwRecipeKey, unsigned char& byActive);


//******************************************************************************
// ����Ϊ��Ʒ��Ϣ��ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫ�Ľ�Ǯ����
DLLDECL int GetRoleMoney(PARSEHANDLE hParser, unsigned long& dwMoney);
//��ȡ��ɫ�Ŀ��ñ�������
DLLDECL int GetRoleEnabledBankPackageCount(PARSEHANDLE hParser, unsigned short& wEnabledBankPackageCount);
//��ȡ��ɫ����Ʒ����
DLLDECL int GetRoleItemCount(PARSEHANDLE hParser, unsigned short& wItemCount);
//��ȡ��ɫ����Ʒ��Ϣ(�����Ƿ��ص�����Ʒ��Ϣ����Ҫ��ȡ��ɫ������Ʒ�б�Ļ�����Ҫѭ�����ñ�����)
//����˵��:unsigned long dwItemIndex ��Ʒ������, unsigned char& byBox ���ڱ���,
//  unsigned char& byPos ����λ��, unsigned char& byVersion ��Ʒ�汾��, unsigned char& byTabType ��Ʒ����,
//  unsigned short& wTabIndex ��Ʒ�����������е�����, unsigned char& byBind �Ƿ��, unsigned short& wDurability �;ö�,
//  time_t& nGenTime ��Ʒ����ʱ�� int& nIsEqui �Ƿ�Ϊװ����Ʒ, unsigned long& dwRandSeed װ���������, unsigned short& wLeftEnchantTime ʣ�฽ħʱ��, unsigned char& byColorID ��ɫ,
//  unsigned short& wPermanentEnchant ���ø�ħID, unsigned short& wTemporaryEnchant ��ʱ��ħID,
//  unsigned short& wMount1 װ����ǶID, unsigned short& wMount2 װ����ǶID, unsigned short& wMount3 װ����ǶID, unsigned short& wMount4 װ����ǶID
//  ��nIsEquiΪtrueʱ,��nIsEqui����Ĳ�����������.
DLLDECL int GetRoleItemInfo(
                            PARSEHANDLE hParser, unsigned long dwItemIndex, unsigned char& byBox, 
                            unsigned char& byPos, unsigned char& byVersion, unsigned char& byTabType, 
                            unsigned short& wTabIndex, unsigned char& byBind, unsigned short& wDurability, time_t& nGenTime,
                            int& nIsEqui, unsigned long& dwRandSeed, unsigned short& wLeftEnchantTime, unsigned char& byColorID,
                            unsigned short& wPermanentEnchant, unsigned short& wTemporaryEnchant,
                            unsigned short& wMount1, unsigned short& wMount2, unsigned short& wMount3, unsigned short& wMount4
);
// ��ȡָ�������ϵ���Ʒ��Ŀ
//����˵��: unsigned char ucBoxIndex ����������, unsigned short& wItemCount ��Ʒ��Ŀ
DLLDECL int GetRoleBagItemCount(PARSEHANDLE hParser, unsigned char ucBoxIndex, unsigned short& wItemCount);

//��ȡ��ɫ��ָ������λ�õ���Ʒ��Ϣ(�����Ƿ��ص�����Ʒ��Ϣ)
// ����˵��:
//  unsigned char ucBoxIndex ��������(0 Ϊװ�������ϵ�, 1~5 Ϊ���ϵ���չ����, 6~11Ϊ���б���, 12Ϊ�ѳ�����Ʒ����), 
//  unsigned char ucPos, ����λ��, (����������Ϊ0ʱ, posȡֵ��Χ:
//                                                    0Ϊ��ս����, 1 Զ������, 2 ����, 3 ͷ��, 4 ����, 5 ��ָ, 6 ����, 7 ��׺
//                                                    8 ����, 9 Ь��, 10 ����, 11 ������չ��װ�����ƺ�֮���, 12 ����, 13 ����
//                                                    14 ������չ, 15 ����)
//  bool bNext, ���ڱ�������Ϊ0ʱ,���� ucPos Ϊ 5 ��ָʱ,��������,��ȡ��һ����ָλ����Ʒ.
//  int& nHadFind, �Ƿ������Ʒ
//  unsigned char& byPos ����λ��, unsigned char& byVersion ��Ʒ�汾��, unsigned char& byTabType ��Ʒ����,
//  unsigned short& wTabIndex ��Ʒ�����������е�����, unsigned char& byBind �Ƿ��, unsigned short& wDurability �;ö�,
//  time_t& nGenTime ��Ʒ����ʱ�� int& nIsEqui �Ƿ�Ϊװ����Ʒ, unsigned long& dwRandSeed װ���������, unsigned short& wLeftEnchantTime ʣ�฽ħʱ��, unsigned char& byColorID ��ɫ,
//  unsigned short& wPermanentEnchant ���ø�ħID, unsigned short& wTemporaryEnchant ��ʱ��ħID,
//  unsigned short& wMount1 װ����ǶID, unsigned short& wMount2 װ����ǶID, unsigned short& wMount3 װ����ǶID, unsigned short& wMount4 װ����ǶID
//  ��nIsEquiΪtrueʱ,��nIsEqui����Ĳ�����������.
DLLDECL int GetRoleItemInfoInBag(
                            PARSEHANDLE hParser, unsigned char ucBagIndex, unsigned char ucPos, bool bNext,
                            int& nHadFind, unsigned char& byVersion, unsigned char& byTabType, 
                            unsigned short& wTabIndex, unsigned char& byBind, unsigned short& wDurability, time_t& nGenTime,
                            int& nIsEqui, unsigned long& dwRandSeed, unsigned short& wLeftEnchantTime, unsigned char& byColorID,
                            unsigned short& wPermanentEnchant, unsigned short& wTemporaryEnchant,
                            unsigned short& wMount1, unsigned short& wMount2, unsigned short& wMount3, unsigned short& wMount4
);

//******************************************************************************
// ����Ϊ������Ϣ��ȡ�ӿ�
//******************************************************************************
//��ȡ��Ϸ������������
DLLDECL int GetGameQuestCount(PARSEHANDLE hParser, int& nCount);
//��ȡ��ɫ������״̬(�����ǽ�ɫ���������״̬����Ҫ��ȡ��ɫ��������״̬�Ļ�����Ҫѭ�����ñ�����)
//����˵��:unsigned long dwQuestIndex��������(��1��ʼ,С������ӿڵ��÷��ص���������), unsigned char& dwQuestState ����״̬(1Ϊ���,0Ϊδ���)
DLLDECL int GetRoleQuestState(PARSEHANDLE hParser, unsigned long dwQuestIndex, unsigned char& dwQuestState);
//��ȡ��ɫ��ǰ�������������
DLLDECL int GetRoleAcceptQuestCount(PARSEHANDLE hParser, unsigned char& dwAcceptQuestCount);
//��ȡ��ɫ��ǰ�����������Ϣ(�����Ƿ��ص���������Ϣ����Ҫ��ȡ��ɫ����������Ϣ�Ļ�����Ҫѭ�����ñ�����)
//����˵��:unsigned long dwQuestIndex ����������, unsigned short& wQuestID ����ID,
//  unsigned char& byFailed �Ƿ�ʧ��, time_t& nLimitTime �޶�ʱ��,
//  unsigned char byKillNpcCount[] ɱnpc��Ŀ(���������ڵ���4��unsigned char�Ŀռ�), int& nKillNPCMaxKind ɱnpc��Ŀ�����С,
//  int nQuestValue[] �������(���������ڵ���8��int�Ŀռ�), int& nnQuestValueMaxKind ������������С
DLLDECL int GetRoleQuestInfo(
                             PARSEHANDLE hParser, unsigned long dwQuestIndex, unsigned short& wQuestID,
                             unsigned char& byFailed, time_t& nLimitTime,
                             unsigned char KillNpcCountArray[], int& nKillNPCMaxKind,
                             int QuestValueArray[], int& nnQuestValueMaxKind
);

//��ȡ��ɫ��ǰЭ�����������
DLLDECL int GetRoleAssistQuestCount(PARSEHANDLE hParser, unsigned char& dwAssistQuestCount);
//��ȡ��ɫÿ���ܽ��ܵ�Э�����������
DLLDECL int GetRoleAssistDailyCount(PARSEHANDLE hParser, unsigned char& dwAssistDailyCount);
//��ȡ��ɫЭ���������Ϣ(�����Ƿ��ص���Э��������Ϣ����Ҫ��ȡ��ɫ����Э��������Ϣ�Ļ�����Ҫѭ�����ñ�����)
//����˵��:unsigned long dwAssistQuestIndex Э����������, unsigned long& dwPlayerID Э�����ID, unsigned short& dwQuestID ����ID
DLLDECL int GetRoleAssistQuestInfo(PARSEHANDLE hParser, unsigned long dwAssistQuestIndex, unsigned long& dwPlayerID, unsigned short& dwQuestID);
//��ȡ��ɫ��ǰÿ�����������
DLLDECL int GetRoleDailyQuestCount(PARSEHANDLE hParser, unsigned char& byDailyQuestCount);
//��ȡ��ɫ��ǰÿ���������Ϣ(�����Ƿ��ص���������Ϣ����Ҫ��ȡ��ɫ����������Ϣ�Ļ�����Ҫѭ�����ñ�����)
//����˵��:unsigned long dwQuestIndex ������, unsigned short& wQuestID ����ID, int& nNextAcceptTime �´ο��Խ�ÿ�������ʱ��
DLLDECL int GetRoleDailyQuestInfo(PARSEHANDLE hParser, unsigned long dwQuestIndex, unsigned short& wQuestID, int& nNextAcceptTime);


//******************************************************************************
// ����Ϊ�������Ϣ��ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫ��ǰѧϰ������ܵ�����
DLLDECL int GetRoleProfessionCount(PARSEHANDLE hParser, unsigned char& byCount);
//��ȡ��ɫ����ܵ���Ϣ(�����Ƿ��ص����������Ϣ����Ҫ��ȡ��ɫ�����������Ϣ�Ļ�����Ҫѭ�����ñ�����)
//����˵��:unsigned long dwProfessionIndex, ���������
//  unsigned char& byProfessionID �����ID, unsigned char& byMaxLevel �������ߵȼ�,
//  unsigned char& byCurrentLevel ��ǰ�ȼ�, unsigned short& wProficiency ������,
//  unsigned char& byBranchID ����ܵķ�֧ID
DLLDECL int GetRoleProfessionInfo(
                                  PARSEHANDLE hParser, unsigned long dwProfessionIndex,
                                  unsigned char& byProfessionID, unsigned char& byMaxLevel,
                                  unsigned char& byCurrentLevel, unsigned short& wProficiency,
                                  unsigned char& byBranchID
);


//******************************************************************************
// ����Ϊ��ɫ������ؼ���Ϣ��ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫ�Ƿ�ѧϰĳһ����ܵ�ĳһ�ؼ�(ע������������ID��dwCraftID��1����)
//����˵��:unsigned long dwRecipeIndex ����������, int& bIsLearned �Ƿ��Ѿ�ѧϰ
DLLDECL int GetRoleRecipeListOfCraft(PARSEHANDLE hParser, unsigned long dwCraftID, unsigned long dwRecipeIndex, int& bIsLearned);


//******************************************************************************
// ����Ϊ��ɫ״̬��Ϣ��ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫ��ǰ�ƶ�״̬
DLLDECL int GetRoleMoveState(PARSEHANDLE hParser, unsigned char& byMoveState);
//��ȡ��ɫ��ǰ���ڽ�ͨ·��
DLLDECL int GetRoleCurrentTrack(PARSEHANDLE hParser, unsigned short& wCurrentTrack);
//��ȡ��ɫ��ǰ�ڽ�ͨϵͳ�е��ƶ�֡��
DLLDECL int GetRoleMoveFrameCounter(PARSEHANDLE hParser, int& nMoveFrameCounter);
//��ȡ��ɫ���ԵĽ�ͨ�����
DLLDECL int GetRoleFromFlyNode(PARSEHANDLE hParser, unsigned short& wFromFlyNode);
//��ȡ��ɫĿ�ĳ��б��
DLLDECL int GetRoleTargetCityID(PARSEHANDLE hParser, unsigned short& wTargetCityID);
//��ȡ��ɫ��ǰ����ֵ
DLLDECL int GetRoleExperience(PARSEHANDLE hParser, int& nExperience);
//��ȡ��ɫ��ǰ����ֵ
DLLDECL int GetRoleCurrentLife(PARSEHANDLE hParser, int& nCurrentLife);
//��ȡ��ɫ��ǰ����ֵ
DLLDECL int GetRoleCurrentMana(PARSEHANDLE hParser, int& nCurrentMana);
//��ȡ��ɫ��ǰ����ֵ
DLLDECL int GetRoleCurrentStamina(PARSEHANDLE hParser, int& nCurrentStamina);
//��ȡ��ɫ��ǰ����ֵ
DLLDECL int GetRoleCurrentThew(PARSEHANDLE hParser, int& nCurrentThew);
//��ȡ��ɫ��ǰǱ�ܵ�
DLLDECL int GetRoleCurrentTrainValue(PARSEHANDLE hParser, int& nCurrentTrainValue);
//��ȡ��ɫ�Ѿ��õ���Ǳ�ܵ�
DLLDECL int GetRoleUsedTrainValue(PARSEHANDLE hParser, int& nUsedTrainValue);
//��ȡ��ɫ�����Ѿ����ӵ�Ǳ�ܵ�
DLLDECL int GetRoleAddTrainTimeInToday(PARSEHANDLE hParser, int& nAddTrainTimeInToday);
//��ȡ��ɫ��PK״̬
DLLDECL int GetRolePKState(PARSEHANDLE hParser, unsigned char& byPKState);
//��ȡ��ɫ��ɱ������ʣ��ʱ��(��λ����)
DLLDECL int GetRoleCloseSlayLeftTime(PARSEHANDLE hParser, unsigned short& wCloseSlayLeftTime);
//��ȡ��ɫʣ��ĸ���֡��
DLLDECL int GetRoleLeftReviveFrame(PARSEHANDLE hParser, unsigned short& wLeftReviveFrame);
//��ȡ��ɫ�ϴ�ԭ�ظ���ʱ��
DLLDECL int GetRoleLastSituReviveTime(PARSEHANDLE hParser, time_t& nLastSituReviveTime);
//��ȡ��ɫһ��ʱ����ԭ�ظ������
DLLDECL int GetRoleSituReviveCount(PARSEHANDLE hParser, unsigned char& bySituReviveCount);
//��ȡɱ����ɫ�Ľ�ɫID
DLLDECL int GetRoleKillerID(PARSEHANDLE hParser, unsigned long& dwKillerID);
//��ȡ��ɫ��ǰ��ɱ��ֵ
DLLDECL int GetRoleCurrentKillPoint(PARSEHANDLE hParser, unsigned short& wCurrentKillPoint);
//��ȡ��ɫ��ǰ������ֵ
DLLDECL int GetRoleCurrentPrestige(PARSEHANDLE hParser, int& nCurrentPrestige);
//��ȡ��ɫ���׶�
DLLDECL int GetContribution(PARSEHANDLE hParser, int& nContribution);
//��ȡ��ɫ���Ƶȼ�
DLLDECL int GetMaxLevel(PARSEHANDLE hParser, int& nMaxLevel);
//��ȡ��ɫ����ñ��״̬
DLLDECL int GetHideHat(PARSEHANDLE hParser, int& nHideHat);
//��ȡ��ɫ�Ľ���ʱ��
DLLDECL int GetBanTime(PARSEHANDLE hParser, time_t& nBanTime);

//******************************************************************************
// ����Ϊ��ɫBuff��Ϣ��ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫBuff����
DLLDECL int GetRoleBuffCount(PARSEHANDLE hParser, unsigned char& byCount);

//��ȡ��ɫBuff��Ϣ(�����Ƿ��ص���Buff��Ϣ����Ҫ��ȡ��ɫ����Buff��Ϣ�Ļ�����Ҫѭ�����ñ�����)
//����˵��:unsigned long BuffIndex buff��������, unsigned long& dwID buff��ID��, 
//int& nLevel �ȼ�, unsigned long& dwRecipeKey ���ű��, unsigned long& dwSkillSrcID �����ͷ��ߵĽ�ɫID,
//unsigned char& nSkillSrcLevel �����ͷ��ߵĵȼ�, unsigned char& byStackNum buff��������, int& nLeftFrame buff��ʣ����Ч֡��
DLLDECL int GetRoleBuffInfo(
                            PARSEHANDLE hParser, unsigned long BuffIndex, unsigned long& dwID,
                            int& nLevel, unsigned long& dwRecipeKey, unsigned long& dwSkillSrcID,
                            unsigned char& nSkillSrcLevel, unsigned char& byStackNum, int& nLeftFrame
);

//******************************************************************************
// ����Ϊ��ɫ������Ϣ��ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫ���ڵ�����������
//����˵��:unsigned short& usCount ����������
DLLDECL int GetRoleReputeCount(PARSEHANDLE hParser, unsigned short& usCount);
// ��ȡ��ɫ������Ϣ
//����˵��:int nIndex ��������(����0,С�������ȡ������������), unsigned char& ucFightFlag ս�����, unsigned char& ucForce ����,
//unsigned char& ucLevel �ȼ�, unsigned char& ucShow ��ʾ, short& shRepute ����
DLLDECL int GetRoleReputeInfo(
                            PARSEHANDLE hParser, int nIndex, unsigned char& ucFightFlag, unsigned char& ucForce,
                            unsigned char& ucLevel, unsigned char& ucShow, short& shRepute
);

//******************************************************************************
// ����Ϊ��ɫPQ��Ϣ��ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫPQ����
DLLDECL int GetRolePQCount(PARSEHANDLE hParser, unsigned short& wCount);
//��ȡ��ɫPQ��Ϣ(�����Ƿ��ص���PQ��Ϣ����Ҫ��ȡ��ɫ����PQ��Ϣ�Ļ�����Ҫѭ�����ñ�����)
DLLDECL int GetRolePQInfo(PARSEHANDLE hParser, unsigned long PQIndex, unsigned long& dwID);

//******************************************************************************
// ����Ϊ��ɫӢ����Ϣ��ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫӢ�۸������
DLLDECL int GetRoleHeroFlag(PARSEHANDLE hParser, int& bHeroFlag);
//��ȡ��ɫ�Ƿ��н���ĳ��Ӣ�۸�����Ȩ��
//����˵��:unsigned long dwMapID ����ID, int& bOpenFlag �������
DLLDECL int GetRoleHeroMapCopyOpenFlag(PARSEHANDLE hParser, unsigned long dwMapID, int& bOpenFlag);


//******************************************************************************
// ����Ϊ��ɫ�ɾ����ݻ�ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫ�ɾͻ���
//����˵��:int& nPoint  �ɾͻ���
DLLDECL int GetRoleAchievementPoint(PARSEHANDLE hParser, int& nPoint);
//��ȡ��ɫ��Ϸ��ɶ�
//����˵��:int& nRecord  ��Ϸ��ɶ�
DLLDECL int GetRoleAchievementRecord(PARSEHANDLE hParser, int& nRecord);
//��ȡ��ɫָ���ɾ���ļ�����Ŀ
//����˵��:int nID �ɾ���ID, int& nCount ��Ŀ
DLLDECL int GetRoleAchievementCounterCount(PARSEHANDLE hParser, int nID, int& nCount);
//��ȡ��ɫָ����ɾ��Ƿ����
//����˵��:int nID �ɾ���ID, int& nIsAchieve ��ɱ��
DLLDECL int GetRoleAchievementAchieveStateFormID(PARSEHANDLE hParser, int nID, int& nIsAchieve);
//��ȡ��ɫ�ɾ�������
//����˵��:int& nCount ������ϳɾ�������
DLLDECL int GetRoleAchievementCount(PARSEHANDLE hParser, int& nCount);
//��ȡ��ɫ�ɾ�����Ϣ,��Ҫ��ȡ���Գɾ�����Ϣ,��Ҫѭ����������
//����˵��:unsigned long ulIndex, int& nID, int& nCount
DLLDECL int GetRoleAchievementInfo(PARSEHANDLE hParser, unsigned long ulIndex, int& nID, int& nCount, int& nIsAchieve);

//******************************************************************************
// ����Ϊ��ɫ�ƺ����ݻ�ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫ��ǰǰ׺
//����˵��:int& nCurrentPrefix ��ǰǰ׺
DLLDECL int GetRoleDesignationCurrentPrefix(PARSEHANDLE hParser, int& nCurrentPrefix);
//��ȡ��ɫ��ǰ��׺
//����˵��:int& nCurrentPostfix ��ǰ��׺
DLLDECL int GetRoleDesignationCurrentPostfix(PARSEHANDLE hParser, int& nCurrentPostfix);
//��ȡ��ɫ���ݱ��
//����˵��:int& nGenerationIndex ���ݱ��
DLLDECL int GetRoleDesignationGenerationIndex(PARSEHANDLE hParser, int& nGenerationIndex);
//��ȡ��ɫ�����е����ֱ��
//����˵��:int& nNameInForceIndex �����е����ֱ��
DLLDECL int GetRoleDesignationNameInForceIndex(PARSEHANDLE hParser, int& nNameInForceIndex);
//����˵��:int& nDisplayFlag�Ƿ���ʾ
DLLDECL int GetRoleDesignationDisplayFlag(PARSEHANDLE hParser, int& nDisplayFlag);
//��ȡ����Ƿ���ĳ��ǰ׺
//����˵��:int nPrefix ���, int& nIsPrefixAcquired �Ƿ���
DLLDECL int GetRoleDesignationIsPrefixAcquiredFormPrefix(PARSEHANDLE hParser, int nPrefix, int& nIsPrefixAcquired);
//��ȡ����Ƿ���ĳ����׺
//����˵��:int nPostfix ���, int& nIsPostfixAcquired�Ƿ���
DLLDECL int GetRoleDesignationIsPostfixAcquiredFormPostfix(PARSEHANDLE hParser, int nPostfix, int& nIsPostfixAcquired);

//******************************************************************************
// ����Ϊ��ɫ��г��ᷢ������ȡ�ӿ�
//******************************************************************************
//��ȡ��ɫ��ȡ������Ʒ���������Ϣ
//����˵��:unsigned long dwID ����id, int nItemPosIndex ��Ʒλ��, int& nHadFind �Ƿ��ҵ�, unsigned long& dwFrustration ����ֵ, int& nCount ������Ŀ
DLLDECL int GetRoleQuestDropInfo(PARSEHANDLE hParser, unsigned long dwID, int nItemPosIndex, int& nHadFind, unsigned long& dwFrustration, int& nCount);
//��ȡ��ɫ��ͨ���������Ϣ
//����˵��:unsigned long dwSkillID  skillID��, unsigned long dwSkillLevel skill�ȼ�, int& nHadFind �Ƿ��ҵ�, unsigned long& dwFrustration ����ֵ, int& nCount ������Ŀ
DLLDECL int GetVenationInfo(PARSEHANDLE hParser, unsigned long dwSkillID, unsigned long dwSkillLevel, int& nHadFind, unsigned long& dwFrustration, int& nCount);

//******************************************************************************
// ����Ϊ��ɫ����ʦͽ��Ϣ��ȡ�ӿ�
//******************************************************************************
DLLDECL int GetRoleMentorBaseInfo(PARSEHANDLE hParser, unsigned char& ucMaxApprenticeCount, int& nAcquiredMentorValue,
                                  time_t& nLastEvokeMentorTime, unsigned char& ucEvokeMentorCount
);

//******************************************************************************
// ����Ϊ��ɫMailBox��Ϣ��ȡ�ӿ�
//******************************************************************************
//������ɫ�ʼ����ݵĻ������������µ�Get�ӿ�ʹ��ǰ������ñ�����
DLLDECL PARSEHANDLE ParseMailBoxInfoData(unsigned char* pbyData, size_t uDataLen);

// ��ȡ��ɫ�ʼ�������Ϣ
//  
DLLDECL int GetMailBaseInfo(PARSEHANDLE hParser, unsigned long& ulNextMailID, 
                            unsigned long& ulLastGlobalMailIndex,
                            unsigned short& usVersion
);

//��ȡ��ɫ�ʼ�����
DLLDECL int GetMailCount(PARSEHANDLE hParser, int& nMailCount);

//��ȡһ���ʼ�������Ϣ(�����Ƿ��ص����ʼ���Ϣ����Ҫ��ȡ��ɫ�����ʼ�������Ϣ�Ļ�����Ҫѭ�����ñ�����)
// ����˵��: unsigned long dwMailIndex �ʼ�����, 
// unsigned long& dwMailID �ʼ�ID, unsigned char& ucType �ʼ�����( 0 Ϊ����ʼ�, 1Ϊϵͳ�ʼ�, 2Ϊ�������ʼ�,3Ϊgm�ʼ�, 4Ϊ�����Ϣ),
// bool&   bRead �Ƿ��Ѷ�,
// char szSenderName[] ����������(����Ϊ32��char��С), int nInSenderNameSize �����szSenderName�����С, 
// char szTitle[] ����(����Ϊ64��char��С), int nInTitleSize �����szTitle�����С,
// char szText[] �ʼ�������Ϣ(����Ϊ512��char��С), int nInTextSize �����szText�����С,
// time_t& nSendTime ����ʱ��,
// time_t& nReceiveTime ����ʱ��,
// time_t& nLockTime ��סʱ��, 
// int& nMoney ��Ǯ,
// int& nItemCount ��Ʒ��Ŀ
DLLDECL int GetMailInfo(PARSEHANDLE hParser, unsigned long dwMailIndex,     
                            unsigned long& dwMailID, unsigned char& ucType,
                            bool&   bRead,
                            char szSenderName[], int nInSenderNameSize, 
                            unsigned long& ulSenderID,
                            char szTitle[], int nInTitleSize,
                            char szText[], int nInTextSize,
                            time_t& nSendTime,
                            time_t& nReceiveTime,
                            time_t& nLockTime, 
                            int& nMoney,
                            int& nItemCount
);

////��ȡ�ʼ�����Ʒ����Ϣ(��������Ʒ,��Ҫ��ȡ��ɫ�����ʼ���Ʒ��Ϣ�Ļ�����Ҫѭ�����ñ�����)
// ����˵��: unsigned long dwMailIndex, �ʼ�����
//  int nItemIndex ��Ʒ����, bool& bAcquired ��ʾ�Ƿ�ַ�������� (true ��ʾ�ѷַ�), int& nPrice ��������ļ۸�, 
//  unsigned char& byVersion ��Ʒ�汾, unsigned char& byTabType ��Ʒ����,
//  unsigned short& wTabIndex ��Ʒ���������е�����, unsigned char& byBind �Ƿ��,
//  unsigned short& wDurability ��Ʒ�;ö�, time_t& nGenTime ��Ʒ����ʱ��
//  int& nIsEqui �Ƿ�Ϊװ����Ʒ, 
//  unsigned long& dwRandSeed װ���������, unsigned short& wLeftEnchantTime ʣ�฽ħʱ��, unsigned char& byColorID ��ɫ,
//  unsigned short& wPermanentEnchant ���ø�ħID, unsigned short& wTemporaryEnchant ��ʱ��ħID,
//  unsigned short& wMount1 װ����ǶID, unsigned short& wMount2 װ����ǶID, unsigned short& wMount3 װ����ǶID,
//  unsigned short& wMount4 װ����ǶID
DLLDECL int GetInMailItemInfo(PARSEHANDLE hParser, unsigned long dwMailIndex, 
                              int nItemIndex, bool& bAcquired, int& nPrice, 
                              unsigned char& byVersion, unsigned char& byTabType, 
                              unsigned short& wTabIndex, unsigned char& byBind,
                              unsigned short& wDurability, time_t& nGenTime, 
                              int& nIsEqui, unsigned long& dwRandSeed, 
                              unsigned short& wLeftEnchantTime, unsigned char& byColorID,
                              unsigned short& wPermanentEnchant, unsigned short& wTemporaryEnchant,
                              unsigned short& wMount1, unsigned short& wMount2, unsigned short& wMount3, unsigned short& wMount4
);

//******************************************************************************
// ����Ϊ��ɫ������Ϣ��ȡ�ӿ�
//******************************************************************************
//������ɫ�������ݵĻ������������µ�Get�ӿ�ʹ��ǰ������ñ�����
DLLDECL PARSEHANDLE ParseFellowShipData(unsigned char* pbyData, size_t uDataLen);
//��ȡ��ɫ��������
DLLDECL int GetFellowShipCount(PARSEHANDLE hParser, int& nFellowShipCount);
//��ȡһ�����ѻ�����Ϣ(�����Ƿ��ص���������Ϣ����Ҫ��ȡ��ɫ���к��ѻ�����Ϣ�Ļ�����Ҫѭ�����ñ�����)
//����˵��:int nFellowShipIndex,  ����������
//  unsigned long& dwAlliedPlayerID ����ID, int& nAttraction �øж�, 
//  int& nGroupID ����ID, int& nMarried �����, int& nBrother �ֵܱ��, int& nWhisperDailyCount ÿ�����Ĵ���,
//  int& nDuelDailyCount ÿ���������, char szRemark[] ��������ڱ��汸ע�ַ�����ָ��, int nRemarkSize ��������ڱ��汸ע�ַ����Ĵ�С,Ҫ����32
DLLDECL int GetFellowshipInfo(PARSEHANDLE hParser, int nFellowShipIndex, 
                              unsigned long& dwAlliedPlayerID, int& nAttraction,
                              int& nGroupID, int& nMarried, int& nBrother, int& nWhisperDailyCount,
                              int& nDuelDailyCount, char szRemark[], int nRemarkSize
);

//��ȡ��ɫ��������
DLLDECL int GetFoeCount(PARSEHANDLE hParser, int& nFoeCount);
//��ȡһ������ID��Ϣ(�����Ƿ��ص�������ID����Ҫ��ȡ��ɫ���г���ID�Ļ�����Ҫѭ�����ñ�����)
//����˵��: int nFoeIndex ����������, unsigned long& dwAlliedPlayerID ���˽�ɫID
DLLDECL int GetFoeID(PARSEHANDLE hParser, int nFoeIndex, unsigned long& dwAlliedPlayerID);
//��ȡ��ɫ����������
DLLDECL int GetBlackListCount(PARSEHANDLE hParser, int& nBlackListCount);//new+
//��ȡһ��������ID��Ϣ(�����Ƿ��ص���������ID����Ҫ��ȡ��ɫ���к�����ID�Ļ�����Ҫѭ�����ñ�����)
//����˵��: int nBlackListIndex ������������, unsigned long& dwAlliedPlayerID ��������ɫID
DLLDECL int GetBlackListID(PARSEHANDLE hParser, int nBlackListIndex, unsigned long& dwAlliedPlayerID);


//******************************************************************************
// ����Ϊ��������Ϣ��ȡ�ӿ�
//******************************************************************************
//������������Ʒ���ݵĻ������������µ�Get�ӿ�ʹ��ǰ������ñ�����
DLLDECL PARSEHANDLE PaserAuctionItemData(unsigned char* pbyData, size_t uDataLen);
//��ȡ�����м�����Ʒ��Ϣ,ֻ������ƷKCOMMON_ITEM_DATA����
// ����˵��: unsigned char& byVersion ��Ʒ�汾, unsigned char& byTabType ��Ʒ����,
//  unsigned short& wTabIndex ��Ʒ���������е�����, unsigned char& byBind �Ƿ��, unsigned short& wDurability ��Ʒ�;ö�,
//  time_t& nGenTime ��Ʒ����ʱ�� int& nIsEqui �Ƿ�Ϊװ��, unsigned long& dwRandSeed װ���������, 
// unsigned short& wLeftEnchantTime ʣ�฽ħʱ��, unsigned char& byColorID ��ɫ,
// unsigned short& wPermanentEnchant ���ø�ħID, unsigned short& wTemporaryEnchant ��ʱ��ħID,
// unsigned short& wMount1 װ����ǶID, unsigned short& wMount2 װ����ǶID, unsigned short& wMount3 װ����ǶID, unsigned short& wMount4 װ����ǶID
// ��nIsEquiΪtrueʱ,��nIsEqui����Ĳ�����������.
DLLDECL int GetAuctionItemInfo(
                               PARSEHANDLE hParser, unsigned char& byVersion, unsigned char& byTabType, 
                               unsigned short& wTabIndex, unsigned char& byBind, unsigned short& wDurability, 
                               time_t& nGenTime, int& nIsEqui, unsigned long& dwRandSeed, 
                               unsigned long& dwLeftEnchantTime, unsigned char& byColorID,
                               unsigned short& wPermanentEnchant, unsigned short& wTemporaryEnchant,
                               unsigned short& wMount1, unsigned short& wMount2, unsigned short& wMount3, unsigned short& wMount4
);


//******************************************************************************
// ����Ϊ�����Ϣ��ȡ�ӿ�
//******************************************************************************
//����������ݵĻ������������µ�Get�ӿ�ʹ��ǰ������ñ�����
DLLDECL PARSEHANDLE PaserTongData(unsigned char* pbyData, size_t uDataLen);

//��ȡ���Ļ�����Ϣ
//����˵��:char szName[] ����ı����������ַ���ָ��, int nNameSize ����ı����������ַ�����С, Ҫ����32
//unsigned long& dwMaster ������ɫID, int& nFund �ʽ�, int& nLevel �ȼ� , int& nDevelopmentPoint ��չ��,
//unsigned short& wMaxMemberCount ��Ա����, unsigned char& byTongState ���״̬( 1Ϊ���ڳﱸ��, 2Ϊ����״̬, 3Ϊ������̭), time_t& nStateTimer ״̬����ʱ���(�����������״̬,�˲�����Ч),
//unsigned char& byCamp ��Ӫ
DLLDECL int GetTongBaseInfo(PARSEHANDLE hParser, char szName[], int nNameSize, 
                            unsigned long& dwMaster, int& nFund, int& nLevel, int& nDevelopmentPoint,
                            unsigned short& wMaxMemberCount, unsigned char& byTongState, time_t& nStateTimer,
                            unsigned char& byCamp
);

//��ȡ������չ��Ϣ
//����˵��:char szAnnouncement[] �������ڱ����ṫ����ַ���ָ��, int nAnnouncementSize �������ڱ����ṫ����ַ����Ĵ�С,Ҫ����128,
//  char szOnlineMessage[] �������ڱ�����������ʾ���ַ���ָ��, int nOnlineMessage �������ڱ�����������ʾ���ַ����Ĵ�С,Ҫ����128,                     
//  char szIntroduction[] �������ڱ�������ܵ��ַ���ָ��, int nIntroduction �������ڱ�������ܵ��ַ����Ĵ�С,Ҫ����128, 
//  char szRules[] �������ڱ���������ַ���ָ��, int nRules �������ڱ���������ַ����Ĵ�С,Ҫ����1024,                                   
DLLDECL int GetTongExInfo(PARSEHANDLE hParser, char szAnnouncement[], int nAnnouncementSize, 
                            char szOnlineMessage[], int nOnlineMessage, 
                            char szIntroduction[], int nIntroduction, 
                            char szRules[], int nRules
);

//��ȡ����Ա��Ŀ
DLLDECL int GetTongMemberCount(PARSEHANDLE hParser, int& nCount);

//��ȡһ������Ա��Ϣ(�����Ƿ��ص�������Ա����Ҫ��ȡ���а���Ա�Ļ�����Ҫѭ�����ñ�����)
//����˵��:int nMemberIndex ��Ա������, unsigned long& dwID ��Ա��ɫID, 
//  int& nGroupID ������Ա��, time_t& nJoinTime ����ʱ��, time_t& nLastOfflineTime �������ʱ��,
//  char szRemark[] �������ڱ��汸ע���ַ���ָ��, int nRemarkSize �������ڱ��汸ע���ַ�����С,Ҫ����32
//  int& nSalary // ���ʴ���(����)
DLLDECL int GetTongMemberInfo(PARSEHANDLE hParser, int nMemberIndex, unsigned long& dwID, 
                              int& nGroupID, time_t& nJoinTime, time_t& nLastOfflineTime,
                              char szRemark[], int nRemarkSize, int& nSalary 
);

//******************************************************************************
// ����Ϊ����ͳ����Ϣ��ȡ�ӿ�
//******************************************************************************
//��������ͳ�����ݵĻ������������µ�Get�ӿ�ʹ��ǰ������ñ�����
DLLDECL PARSEHANDLE PaserStatData(unsigned char* pbyData, size_t uDataLen);

//���ʹ�õ���ID���ֵ
//����˵��: nMaxUsedID ʹ�õ���ID���ֵ
DLLDECL int GetStatDataMaxUsedID(PARSEHANDLE hParser, int& nMaxUsedID);

//���ָ��ID�е���ֵ
//����˵��: int nID ID��, long long& nValue��ֵ
DLLDECL int GetStatDataValueFromID(PARSEHANDLE hParser, int nID, long long& nValue);

//******************************************************************************
// ����ΪPQ����Ϣ��ȡ�ӿ�
//******************************************************************************
//����PQ�����ݵĻ������������µ�Get�ӿ�ʹ��ǰ������ñ�����
DLLDECL PARSEHANDLE PaserPQTableData(unsigned char* pbyData, size_t uDataLen);
//��ȡPQ����Ϣ
//����˵��:unsigned char& byVersion �汾��, unsigned long& dwPQID , unsigned long& dwPQTemplateID,
//time_t& nBeginTime ��ʼʱ��, int pnPQValue[] ָ��int��������(���������С��8��Ԫ��), int nPQValueSize ����Ĵ�С, int& nNeedPQValueSize ����᷵��ʵ��pnPQValue��Ҫ�Ĵ�С,
//unsigned char& bFinished �Ƿ���ɱ��, unsigned char& byOnwerType ����������,
//unsigned long& dwOnwerID1 ��һ��������ID, unsigned long& dwOnwerID2 �ڶ���������ID, unsigned long& dwOnwerID3������������ID
DLLDECL int GetPQTabeDataInfo(PARSEHANDLE hParser, unsigned char& byVersion, unsigned long& dwPQID, unsigned long& dwPQTemplateID,
                              time_t& nBeginTime, int pnPQValue[], int nPQValueSize, int& nNeedPQValueSize,
                              unsigned char& bFinished, unsigned char& byOnwerType,
                              unsigned long& dwOnwerID1, unsigned long& dwOnwerID2, unsigned long& dwOnwerID3
);

//******************************************************************************
// ����ΪMentor����Ϣ��ȡ�ӿ�
//******************************************************************************
//����Mentor�����ݵĻ������������µ�Get�ӿ�ʹ��ǰ������ñ�����
DLLDECL PARSEHANDLE PaserMentorTableData(unsigned char* pbyData, size_t uDataLen);

//��ȡMentor��Ϣ
// ����˵��:
//time_t  nCreateTrefnCreateTimeime;        // ��ʦʱ��
//int     refnMentorValue;                  // ʦͽֵ
//time_t  refnEndTime;                      // ״̬�Ľ�ֹʱ��
//BYTE    refucState;                       // ��ϵ�������
//BYTE    byOriginalALevel;                 // ��ʦʱͽ�ܵĵȼ�    
DLLDECL int GetMentorInfo(PARSEHANDLE hParser, time_t& refnCreateTime, 
                          int& refnMentorValue, time_t& refnEndTime,
                          unsigned char& refucState, unsigned char& byOriginalALevel
);

#ifdef __cplusplus
}
#endif


#endif
