#ifndef _SO3_RESULT_H_
#define _SO3_RESULT_H_

//���󷵻�ֵ����
enum ERROR_CODE_TYPE
{
	ectInvalid = 0,

	ectSkillErrorCode,
	ectLoginErrorCode,
	ectQuestErrorCode,
	ectItemErrorCode,
    ectTradingErrorCode,
    ectTalkErrorCode,
	ectCraftErrorCode,

	ectTotal
};

//������صĴ��󷵻�ֵ
enum SKILL_RESULT_CODE
{
	srcInvalid = 0,

	srcSuccess,             // �ɹ�
	srcFailed,              // ʧ�ܣ�δ����ԭ��

    srcInvalidCastMode,     // �ͷŷ�ʽ����
	srcNotEnoughLife,		// Ѫ������
	srcNotEnoughMana,		// ��������
	srcNotEnoughStamina,	// ��������
	srcNotEnoughRage,		// ŭ������
	srcNotEnoughItem,		// ���߲���
    srcNotEnoughAmmo,       // ��ҩ����
	srcSkillNotReady,		// ����CDʱ��δ��
    srcInvalidSkill,        // ��Ч�ļ���
    srcInvalidTarget,       // ��Ч��Ŀ��
    srcNoTarget,            // û��Ŀ��
    srcNeedTarget,          // ��Ҫָ��һ��Ŀ��
    srcTooCloseTarget,      // Ŀ��̫��
	srcTooFarTarget,		// Ŀ��̫Զ
    srcOutOfAngle,          // Ŀ��ǶȲ���
    srcTargetInvisible,     // Ŀ�겻�����߷�Χ��
    srcWeaponError,         // ������ƥ��
    srcAmmoError,           // ��ҩ��������ƥ��
    srcNotEquitAmmo,        // û��װ����ҩ
    srcMountError,          // û��װ����Ӧ���书
	srcInventoryIsFull,		// ��������

	srcLootFailed,			// ʰȡʧ��
	srcNotExistLootItem,	// Ҫʰȡ����Ʒ������
	srcAddLootItemFailed,	// ʰȡ����Ʒ�޷����뱳��
	srcNoLootTarget,		// ��ʰȡĿ��Doodad
	srcTooFarToLoot,		// ̫Զ������ʰȡ

    srcInOTAction,          // ����������OTAction��Ϊ
	srcTotal
};

//��½��صĴ��󷵻�ֵ
enum LOGIN_RESULT_CODE
{
	lrcInvalid = 0,

	lrcSuccess,
	lrcFailed,

	//��GS���صĴ������
	lrcCannotFindPlayerInGC,			//�ܿ����Ҳ��������
	lrcErrorDestGS,						//�����Ŀ��GameServer
	lrcErrorRoleStateInGC,				//�ܿ��ϵ����״̬����
	lrcCannotFindPlayerInLoginQueue,	//��½�������Ҳ������
	lrcErrorGameStatus,					//����Ľ�ɫ״̬
	lrcErrorGUID,						//�����Guid
	lrcErrorRoleData,					//����Ľ�ɫ����
	lrcAddPlayerFailed,					//����Ϸ��������ӽ�ɫʧ��

	lrcTotal
};

enum QUEST_RESULT_CODE
{
	qrcInvalid = 0,

	qrcSuccess,
	qrcFailed,

	qrcErrorQuestID,					//���������ID
	qrcQuestListFull,					//�����б�����
	qrcErrorQuestState,					//���������״̬
	qrcAlreadyAcceptQuest,				//�Ѿ������˸�����
	qrcCannotFindQuest,					//�������ñ����Ҳ���ָ��������
	qrcErrorQuestIndex,					//�������������
	qrcTooLowLevel,						//��ɫ�ȼ�̫��
	qrcTooHighLevel,					//��ɫ�ȼ�̫��
	qrcTooLowMoral,						//��ɫ����ֵ̫��
	qrcTooHighMoral,					//��ɫ����ֵ̫��
	qrcErrorGender,						//��ɫ�Ա𲻷���Ҫ��
	qrcErrorRoleType,					//��ɫ���Ͳ���
	qrcCooldown,						//Cooldown��ʱ��δ��ʱ��
	qrcPrequestUnfinished,				//ǰ������û���
	qrcErrorQuestValue,					//������������״̬
	qrcErrorItemCount,					//�����������Ʒ����
	qrcErrorKillNpcCount,				//�����ɱ������
	qrcTooLongTime,						//������ʱ�䳬������
	qrcErrorTaskValue,					//��������������ֵ
	qrcAssistQuestListFull,				//Э�������б����
	qrcErrorAssistQuestIndex,			//�����Э���������
	qrcErrorStartNpcTarget,				//Ŀ��Npc���ϲ��ܽӸ�����
	qrcErrorStartDoodadTarget,			//Ŀ��Doodad���ϲ��ܽӸ�����
	qrcErrorEndNpcTarget,				//Ŀ��Npc���ϲ��ܽ�������
	qrcErrorEndDoodadTarget,			//Ŀ��Doodad���ϲ��ܽ�������
	qrcNoStartQuestItem,				//û��������ʼ����
	qrcNoEndQuestItem,					//û�н�����ʼ����
	qrcTooEarlyTime,					//������ʱ��̫��
	qrcTooLateTime,						//������ʱ��̫��
	qrcTooLowSkillLevel,				//�书���ܵȼ�̫��
	qrcTooLowLifeLevel,					//����ܵȼ�̫��
	qrcErrorGenerateItem,				//������������Ʒʧ��
	qrcAddItemFailed,					//�����������Ʒʧ��
	qrcAddMoneyFailed,					//��ӽ�Ǯʧ��
	qrcNotEnoughStartItem,				//��������߲���
	qrcNotEnoughFreeRoom,				//�����ռ䲻��
	qrcErrorMap,						//����ĵ�ͼ
	qrcUnfinishedQuest,					//δ������񣬲���Э��
	qrcCannotFindQuestInDestPlayer,		//�޷���Ŀ����������ҵ���ҪЭ��������
	qrcNeedAccept,						//�����Ƚ�������ܽ�

	qrcTotal
};

enum ITEM_RESULT_CODE
{
	ircInvalid = 0,

	ircSuccess,
	ircFailed,

	ircItemNotExist,					//��Ʒ������
	ircCannotUseItem,					//��Ʒ���ɱ�ʹ��
	ircUseInPackage,					//�ڱ����в���ʹ��
	ircUseWhenEquipped,					//װ���ϲ���ʹ�ø���Ʒ
	ircNotEnoughDurability,				//�;öȲ���������������
	ircCannotCastSkill,					//�����ͷ���Ʒ�ϵļ���
	ircCastSkillFailed,					//�ͷż���ʧ��
	ircCostItemDurabilityFailed,		//�����;ö�ʧ��
	ircErrorEquipPlace,					//װ����Ʒ��λ�ô���
	ircTooLowAgility,					//����̫��,����װ��
	ircTooLowStrength,					//����̫��,����װ��
	ircTooLowSpirit,					//����̫��,����װ��
	ircTooLowVitality,					//����̫��,����װ��
	ircTooLowLevel,						//�ȼ�̫��,����װ��
	ircTooLowDurability,				//�;ö�Ϊ��,����װ��
	ircCannotEquip,						//��Ʒ���ɱ�װ��
	ircCannotDestory,					//��Ʒ�޷����ݻ�
	ircCannotPutToThatPlace,			//��Ʒ�޷����õ��Ǹ�λ��	
	ircCannotDisablePackage,			//�������ж����޷�ж��
	ircPackageNotEnoughSize,			//������С����
	ircPackageErrorType,				//�������Ͳ���
	ircTotal
};

enum PARTY_RESULT_CODE
{
    ePartyRetInvalid = 0,
    ePartyDstNotExist,                  
    ePartyInvitedPlayerAlreadInParty,
    ePlayerAlreadyBeingInInvitedState,
    ePartyApplyDstFull,
    ePartyDstDenied,                    
    ePartyDstNotOnline,
    ePartyRetTotal
};

enum SHOP_SYSTEM_RESPOND_CODE
{
    ssrcInvalid= 0,
    ssrcSellSuccess,                    // ���۳ɹ�    
    ssrcBuySuccess,                     // ����ɹ�
    ssrcRepairSuccess,                  // ����ɹ�
    ssrcSellFailed,                     // ����ʧ��
    ssrcBuyFailed,                      // ����ʧ��
    ssrcRepairFailed,                   // ����ɹ�
    
    ssrcTooFarAway,                     // ����̫Զ��
    ssrcNotEnoughMoney,                 // ��Ǯ����
    ssrcNotEnoughRepate,                // ��������
    ssrcNotEnoughRank,                  // �ƺŲ���Ҫ��
    ssrcItemSoldOut,                    // �̵����Ʒ������
    ssrcBagFull,                        // ��������
    ssrcItemExistLimit,                 // ��Ʒӵ������
    ssrcItemNotNeedRepair,              // ��Ʒ��������
    ssrcNoneItemNeedRepair,             // û����Ʒ��������
    ssrcCanNotSell,                     // ���ܳ��۸ĸ���Ʒ
    ssrcItemHadLimit,                   // ��Ʒӵ�дﵽ����
    ssrcItemModifiedWhileBuy,           // ���㷢��������������ʱ���ڣ�������Ʒ�����˱仯

    ssrcTotal
};

enum TRADING_RESULT_CODE
{
    trcInvalid,
    trcSuccess,
    trcFailed,                  

    trcInvalidTarget,
    trcInvalidInvite,           // ���������ѹ���
    trcRefuseInvite,            // Ŀ��ܾ�����
    trcTargetNotInGame,         // �����Ŀ�겻����Ϸ��
    trcTargetBusy,              // Ŀ������æ
    trcYouBusy,                 // ������æ�������
    trcTooFar,                  // ����̫Զ��
    trcTradingCancel,           // ����ȡ��
    trcTradingCanceled,         // ���ױ�ȡ��
    trcItemBeBound,             // ��Ʒ�Ѱ�
    trcNotEnoughMoney,          // û���㹻�Ľ�Ǯ
    trcNotEnoughFreeRoom,       // û���㹻�Ŀռ�    
    trcItemExistAmountLimit,    // ��Ʒӵ�г�������

    trcTotal
};

enum CRAFT_RESULT_CODE
{
	crcInvalid = 0,

	crcSuccess,             // �ɹ�
	crcFailed,              // ʧ�ܣ�δ����ԭ��

	crcInvalidCastMode,     // �ͷŷ�ʽ����
	crcNotEnoughStamina,	// ��������
	crcNotEnoughItem,		// ���߲���
	crcSkillNotReady,		// ����CDʱ��δ��
	crcInvalidProfession,   // ��Ч�ļ���
	crcInvalidCraft,		// ��Ч�ļ���
	crcInvalidRecipe,		// ��Ч���䷽
	crcInvalidTarget,       // ��Ч��Ŀ��
	crcInvalidCaster,		// ��Ч���ͷ���
	crcInvalidItem,			// ��Ч�ĵ���
	crcLootFailed,			// ���ɵ����ʧ��
	crcNoTarget,            // û��Ŀ��
	crcNeedTarget,          // ��Ҫָ��һ��Ŀ��
	crcWeaponError,         // ������ƥ��
	crcProfessionNotLearned,  //����ܻ�ûѧ��
	crcRecipeNotLearned,		//�䷽û��ѧ��
	crcTooLowProfessionLevel, //����ܵȼ�̫��
	crcAddItemFaild,			//��ӵ���ʧ��

	crcInventoryIsFull,		// ��������
	crcAddItemFailed,		// ʰȡ����Ʒ�޷����뱳��

	crcInOTAction,          // ����������OTAction��Ϊ

	crcTotal
};

enum PLAYER_RELATION_RESPOND_CODE
{
    rrcInvalid = 0,

    rrcSucess,              // �ɹ���

    rrcSucessAdd,
    rrcSucessDel,

    rrcFailed,              // ʧ�ܣ�δ���塣

    rrcInvalidName,         // ��Ч��������֡�
    rrcRelationListFull,    // �����б�������
    rrcRelationExists,      // ָ���ĺ����Ѿ����ڡ�
    rrcRelationNotFound,    // ָ���ĺ��Ѳ����ڡ�

    rrcTotal
};

//�ַ�����
extern char* g_QuestRMList[];

#endif	//_SO3_RESULT_H_