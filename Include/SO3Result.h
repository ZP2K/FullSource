#ifndef _SO3_RESULT_H_
#define _SO3_RESULT_H_

// ���󷵻�ֵ����
enum ERROR_CODE_TYPE
{
	ectInvalid = 0,

	ectSkillErrorCode,
	ectLoginErrorCode,
	ectQuestErrorCode,
    ectFellowshipErrorCode,
    ectFellowshipAttractionErrorCode,
	ectItemErrorCode,
    ectTradingErrorCode,
    ectTalkErrorCode,
	ectCraftErrorCode,
	ectLootItemErrorCode,
    ectMailErrorCode,
    ectTeamNotifyCode,
	ectAddItemErrorCode,
    ectPeekOtherPlayerErrorCode,
    ectShopEventNotifyCode,
	ectPKErrorCode,
    ectFellowshipOnline,
    ectFoeOnline,
    ectBanishNotifyCode,
    ectRoadTrackErrorCode,
    ectSwitchMapErrorCode,
    ectUseItemErrorCode,
    ectOpenVenationRetCode,
    ectShareQuestErrorCode,
    ectSetQuestAssistDailyCount,
    ectFinishAssistQuest,
    ectAddSkillRecipe,
    ectFellowshipAttractionFallOff,
    ectCampResultCode,
    ectChargeLimitCode,

	ectTotal
};

// ��ӵ��ߵĴ�����ʾ
enum ADD_ITEM_RESULT_CODE
{
	aircInvalid = 0,

	aircSuccess,
	aircFailed,

	aircNotEnoughFreeRoom,	// û���㹻�ı����ռ�			
	aircItemAmountLimited,	// ���޷���ø���Ĵ�����Ʒ
    aircCannotPutThatPlace, // ���ܷ��ڸ�λ��

	aircTotal
};

// ������صĴ��󷵻�ֵ
enum SKILL_RESULT_CODE
{
	srcInvalid = 0,

	srcSuccess,             // �ɹ�
	srcFailed,              // ʧ�ܣ�δ����ԭ��

    srcInvalidCastMode,     // �ͷŷ�ʽ����
	srcNotEnoughLife,		// Ѫ������
	srcNotEnoughMana,		// ��������
    srcNotEnoughRage,       // ŭ������
	srcNotEnoughStamina,	// ��������
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
    srcWeaponDestroy,       // ��������(�;ö�Ϊ0)
    srcAmmoError,           // ��ҩ��������ƥ��
    srcNotEquitAmmo,        // û��װ����ҩ
    srcMountError,          // û��װ����Ӧ���书
    srcInOTAction,          // ����������OTAction��Ϊ
    srcOnSilence,           // �����ڽ���״̬
    srcNotFormationLeader,  // �������ۣ������ͷ���
    srcNotEnoughMember,     // ����󷨵ĳ�Ա����
    srcNotStartAccumulate,  // û�п�ʼ����
    srcBuffError,           // û������Ҫ��Buff
    srcNotInFight,          // ������ս����
    srcMoveStateError,      // �ƶ�״̬����
    srcErrorByHorse,        // �������״̬����
    srcBuffInvalid,         // �����õ�BUFF������
    srcForceEffect,         // ��ǿ���Ч����������
    srcBuffImmunity,        // ���BUFF��������
    srcTargetLifeError,     // Ŀ��Ѫ��������
    srcSelfLifeError,     // ����Ѫ��������
    srcDstMoveStateError,
    srcNotTeam,            // û�����
    srcMapBan,

	srcTotal
};

enum LOOT_ITEM_RESULT_CODE
{
	lircInvalid = 0,

	lircSuccess,            // �ɹ�
	lircFailed,             // ʧ�ܣ�δ����ԭ��

	lircInventoryIsFull,	// ��������
	lircNotExistLootItem,	// Ҫʰȡ����Ʒ������
	lircAddLootItemFailed,	// ʰȡ����Ʒ�޷����뱳��
	lircNoLootTarget,		// ��ʰȡĿ��Doodad
	lircTooFarToLoot,		// ̫Զ������ʰȡ
	lircOverItemLimit,		// ����Я������

	lircTotal
};

enum QUEST_RESULT_CODE
{
	qrcInvalid = 0,

	qrcSuccess,
	qrcFailed,

	qrcErrorQuestIndex,					//�������������
	qrcErrorQuestID,					//���������ID
	qrcQuestListFull,					//�����б�����
	qrcErrorQuestState,					//���������״̬
	qrcAlreadyAcceptQuest,				//�Ѿ�����������
	qrcAlreadyFinishedQuest,			//�Ѿ����������
	qrcCannotFindQuest,					//�������ñ����Ҳ���ָ��������
	qrcTooLowLevel,						//��ɫ�ȼ�̫��
	qrcTooHighLevel,					//��ɫ�ȼ�̫��
	qrcErrorGender,						//��ɫ�Ա𲻷���Ҫ��
	qrcErrorRoleType,					//��ɫ���Ͳ���
	qrcErrorForceID,					//��ɫ���ɲ���
	qrcCooldown,						//Cooldown��ʱ��δ��ʱ��
	qrcPrequestUnfinished,				//ǰ������û���
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
	qrcAddReputationFailed,				//�������ʧ��
	qrcNotEnoughStartItem,				//��������߲���
	qrcNotEnoughFreeRoom,				//�����ռ䲻��
	qrcErrorMap,						//����ĵ�ͼ
	qrcUnfinishedQuest,					//δ������񣬲���Э��
	qrcCannotFindQuestInDestPlayer,		//�޷���Ŀ����������ҵ���ҪЭ��������
	qrcNeedAccept,						//�����Ƚ�������ܽ�
	qrcNoNeedAccept,					//���ýӾ��ܽ�������
	qrcQuestFailed,						//��������ʧ��
	qrcAddSkillFailed,					//��Ӽ���ʧ��
	qrcNotEnoughMoney,					//��Ǯ����
    qrcMoneyLimit,                      //Ǯ�Ѿ��ﵽ����
    qrcErrorRepute,                     //����������
    qrcAddPresentItemFailed,            //�޷�����������ɵ���
    qrcDailyQuestFull,                  //ÿ����������
    qrcEscortQuestIsRunning,            //�����������������������
    qrcErrorCamp,                       //��Ӫ����
    qrcQuestEndBookNotMemorized,        //�����Ķ�������
    qrcChargeLimit,
    qrcRequireApprentice,               //������ͽ��
    qrcTooManyQuestOfferItem,           //���񷢷ŵ��߳���ӵ������

	qrcTotal
};

enum ITEM_RESULT_CODE
{
	ircInvalid = 0,

	ircSuccess,
	ircFailed,

	ircItemNotExist,					// ��Ʒ������
	ircNotEnoughDurability,				// �;öȲ���������������
	ircErrorEquipPlace,					// װ����Ʒ��λ�ô���
    ircForceError,                      // ���ɲ���
	ircTooLowAgility,					// ����̫��,����װ��
	ircTooLowStrength,					// ����̫��,����װ��
	ircTooLowSpirit,					// ����̫��,����װ��
	ircTooLowVitality,					// ����̫��,����װ��
	ircTooLowLevel,						// �ȼ�̫��,����װ��
	ircTooLowDurability,				// �;ö�Ϊ��,����װ��
	ircCannotEquip,						// ��Ʒ���ɱ�װ��
	ircCannotPutToThatPlace,			// ��Ʒ�޷����õ��Ǹ�λ��	
	ircCannotDisablePackage,			// �������ж����޷�ж��
	ircPackageNotEnoughSize,			// ������С����
	ircPackageErrorType,				// �������Ͳ���
	ircPlayerIsDead,					// ����״̬���ܽ����������
	ircBankPackageDisabled,				// ���б���û�м���,�������Ϸű���
    ircEquipedPackageCannotDestroy,     // �Ѿ�װ���ϵı������ܴݻ�
    ircBinded,                          // ��Ʒ�Ѿ���
    ircCannotDestroy,                   // ����Ʒ���ܱ��ݻ�
    ircGenderError,                     // �Ա𲻷���
    ircPlayerInFight,                   // ս���в����л�
    ircOnHorse,                         // �����ʱ����������
    ircCampCannotEquip,                 // ��ǰ������Ӫ����װ������Ʒ
    ircRequireProfession,               // ��Ҫѧϰĳ�����
    ircRequireProfessionBranch,         // ��Ҫѧϰĳ����ܷ�֧
    ircProfessionLevelTooLow,           // ����ܵȼ�̫��

	ircTotal
};

enum USE_ITEM_RESULT_CODE
{
	uircInvalid = 0,

	uircSuccess,
	uircFailed,
    
    uircItemNotExist,                    // ��Ʒ������   
    uircCannotUseItem,                   // ��Ʒ����ʹ��
    uircNotEnoughDurability,             // �;öȲ���
    uircNotReady,                        // ��Ʒ��û׼����
    uircUseInPackage,					 // �ڱ����в���ʹ��
	uircUseWhenEquipped,				 // װ���ϲ���ʹ�ø���Ʒ
    uircTooLowLevel,                     // �ȼ�̫��
    uircRequireProfession,               // ��Ҫѧϰĳ�����
    uircRequireProfessionBranch,         // ��Ҫѧϰĳ����ܷ�֧
    uircProfessionLevelTooLow,           // ����ܵȼ�̫��
    uircOnHorse,                         // ��������
    uircInFight,                         // ����ս��
    uircRequireMale,                     // ���˲���ʹ��
    uircRequireFeMale,                   // Ů�˲���ʹ��
    uircCampCannotUse,                   // ��ɫ������Ӫ����ʹ��
    uircCannotCastSkill,                 // �����ͷ���Ʒ�ϵļ���
    uircCastSkillFailed,                 // �ͷ�ʧ��
    uircCostItemDurabilityFailed,        // �����;ö�ʧ��
    uircForceError,                      // ���ɲ���
    uircAleardHaveDesignation,           // �Ѿ���øóƺ�

	uircTotal
};

enum PARTY_NOTIFY_CODE
{
    epncInvalid = 0,
    epncPlayerInviteNotExist,
    epncPlayerApplyNotExist,
    epncPlayerAlreadInYourTeam,
    epncPlayerAlreadInOtherTeam,
    epncYouAlreadInTeamState,
    epncPlayerIsBusy,
    epncInvitationDenied,   
    epncApplicationDenied,
    epncPlayerNotOnline,
    epncDestTeamIsFull,
    epncYourTeamIsFull,
    epncInvitationOutOfDate,  // �����Ѿ�����
    epncApplicationOutOfDate, // �����Ѿ�����
    epncTeamCreated,
    epncTeamJoined,
    epncInvitationDone,       // ����XXX��������
    epncApplicationDone,      // ����XXX��������
    epncInvitationReject,     // ��ܾ���XXX������
    epncApplicationReject,    // ��ܾ���XXX������
    epncCampError,
    epncTotal
};

enum SHOP_SYSTEM_RESPOND_CODE
{
    ssrcInvalid = 0,
    ssrcSellSuccess,                    // ���۳ɹ�
    ssrcBuySuccess,                     // ����ɹ�
    ssrcRepairSuccess,                  // ����ɹ�
    ssrcSellFailed,                     // ����ʧ��
    ssrcBuyFailed,                      // ����ʧ��
    ssrcRepairFailed,                   // ����ɹ�

    ssrcTooFarAway,                     // ����̫Զ��
    ssrcNotEnoughMoney,                 // ��Ǯ����
    ssrcNotEnoughPrestige,              // ������������
    ssrcNotEnoughContribution,          // ���׵�������
    ssrcAchievementRecordError,         // ��Ϸ��ɶȲ���
    ssrcNotEnoughAchievementPoint,      // �ɾ͵�������
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
    ssrcYouDeath,                       // �����
    ssrcItemBroken,                     // ����Ʒ�;ö�̫�ͣ��޷����ۡ�
    ssrcItemNotInPackage,               // ��Ʒ���ڱ����С�
    ssrcNotEnoughItem,                  // ��Ҫ�ĵ��߲���

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
    trcYouDead,                 // ����Ѿ�����
    trcItemNotInPackage,        // ֻ�б����ڵ���Ʒ���Խ���
    trcTargetDead,              // Ŀ���Ѿ�����

    trcTotal
};

enum CRAFT_RESULT_CODE
{
	crcInvalid = 0,

	crcSuccess,                 // �ɹ�
	crcFailed,                  // ʧ�ܣ�δ����ԭ��

	crcNotEnoughStamina,	    // ��������
	crcNotEnoughThew,	        // ��������
    crcNotEnoughItem,		    // ���߲���
	crcSkillNotReady,		    // ����CDʱ��δ��
	crcWeaponError,             // ������ƥ��
	crcTooLowProfessionLevel,   // ����ܵȼ�̫��
    crcTooLowExtProfessionLevel,// ��չ����ܵȼ�̫��
	crcAddItemFailed,			// ��ӵ���ʧ��
	crcInventoryIsFull,		    // ��������
	crcProfessionNotLearned,    // ����δѧϰ
    crcExtProfessionNotLearned, // ��չ����δѧϰ
	crcErrorTool,			    // û����ع���
	crcBookIsAlreadyMemorized,	// �鼮�Ѿ���������
	crcRequireDoodad,			// ������Χ�й����õ�Doodad
    crcItemTypeError,           // Ŀ����Ʒ���Ͳ���
    crcBookCannotBeCopy,        // ���鲻�ܱ���¼
    crcDoingOTAction,           // �����������Ķ���
    crcMoveStateError,          // ��ǰ״̬�޷����������
    crcTooLowLevel,             // ��ҵȼ�̫��

	crcTotal
};

enum PLAYER_FELLOWSHIP_RESPOND_CODE
{
    rrcInvalid = 0,

    rrcSuccess,                 // �ɹ���

    rrcSuccessAdd,              // ��ӹ�ϵ�ɹ�
    rrcSuccessAddFoe, 
    rrcSuccessAddBlackList, 

    rrcFailed,                  // ʧ�ܣ�δ���塣

    rrcInvalidName,             // ��Ч��������֡�
    rrcAddSelf,                 // ����Լ�Ϊ����
    rrcFellowshipListFull,      // �����б�������
    rrcFoeListFull,             // �����б�������
    rrcBlackListFull,           // �������б�������
    rrcFellowshipExists,        // ָ���ĺ����Ѿ����ڡ�
    rrcFoeExists, 
    rrcBlackListExists, 
    rrcFellowshipNotFound,      // ָ���ĺ��Ѳ����ڡ�
    rrcFoeNotFound, 
    rrcBlackListNotFound,

    rrcInvalidGroupName,        // ��Ч�ķ������֡�
    rrcInvalidRemark,           // ��Ч�ı�ע��
    rrcFellowshipGroupFull,     // ���ѷ���������
    rrcFellowshipGroupNotFound, // ָ�����鲻���ڡ�

    rrcInFight,                 // ս��״̬���ܲ������ˡ�

    rrcBeAddFriend,             // ���������
    rrcBeAddFoe,                // ���������
    rrcBeAddBlackList,          // �����������

    rrcTotal
};

enum PEEK_OTHER_PLAYER_RESPOND_CODE
{
    prcInvalid = 0,

    prcSuccess,                 // �ɹ�
    prcFailed,                  // ʧ�ܣ�δ�������
    prcCanNotFindPlayer,        // ʧ�ܣ��Ҳ���ָ�����
    prcTooFar,                  // ����̫Զ

    prcTotal
};

enum PK_RESULT_CODE
{
	pkrcInvalid = 0,

	pkrcSuccess,				// �ɹ�
	pkrcFailed,					// δ֪����

	pkrcApplyDuelFailed,
    pkrcTargetIsDueling,
    pkrcInSameTeam,            // ��ͬһ���������治��PK
    pkrcOutofRange,             // ���ھ��뷶Χ��
	pkrcApplySlayFailed,
    pkrcLevelTooLow,            // ����̫��
    pkrcPKNotReady,             // CD��

	pkrcTotal
};

enum ROAD_TRACK_RESULT_CODE
{
    rtrcInvalid = 0,

    rtrcSuccess,
    rtrcFailed,

    rtrcNotOpen,
    rtrcCostError,
    rtrcNotEnoughMoney,

    rtrcTotal
};

enum SWITCH_MAP_RESULT_CODE
{
    smrcInvalid = 0,

    smrcSuccess,
    smrcFailed,
    
    smrcMapCreating,
    smrcEnterNewCopyTooManyTimes,
    smrcGameMaintenance,
    smrcGameOverLoaded,
    smrcCreateMapFinished,
    smrcMapCopyCountLimit,

    smrcTotal
};

enum SHARE_QUEST_RESULT_CODE
{
	sqrcInvalid = 0,

	sqrcSuccess,
	sqrcFailed,

	sqrcTooFar,
    sqrcQuestCannotShare,
    sqrcNotInTeam,
    sqrcAlreadyAcceptQuest,
    sqrcAlreadyFinishedQuest,
    sqrcQuestListFull,
    sqrcAcceptQuest,
    sqrcRefuseQuest,

	sqrcTotal
};

enum BATTLE_FIELD_RESULT_CODE
{
    bfrcInvalid = 0,

    bfrcSuccess,
    bfrcFailed,

    bfrcInBlackList,
    bfrcLevelError,
    bfrcForceError,
    bfrcTeamMemberError,
    bfrcTeamSizeError,
    bfrcTooManyJoin,
    bfrcNotInSameMap,
    bfrcCampError,
    bfrcTimeError,

    bfrcTotal
};

enum TONG_EVENT_NOTIFY_CODE
{
    tenInvalid = 0,

    tenInviteSuccess,
    tenInviteNameNotFoundError,
    tenInviteTargetAlreadyJoinTongError,
    tenInviteTargetBusyError,
    tenInviteTargetOfflineError,
    tenInviteNoPermissionError, 

    tenInviteInvaildError,
    tenInviteTimeOutError,
    tenInviteTargetRefuse,
    tenInviteTargetChargeLimit,

    tenInviteMemberNumberLimitError,

    tenKickOutSuccess,
    tenKickOutTargetNotFoundError,
    tenKickOutTargetNoPermissionError,

    tenModifyTongNameSuccess,
    tenModifyTongNameIllegalError,
    tenModifyTongNameNoPermissionError,

    tenModifyAnnouncementSuccess,
    tenModifyAnnouncementNoPermissionError,

    tenModifyOnlineMessageSuccess,
    tenModifyOnlineMessageNoPermissionError,

    tenModifyIntroductionSuccess,
    tenModifyIntroductionNoPermissionError,

    tenModifyRulesSuccess,
    tenModifyRulesNoPermissionError,

    tenModifyGroupNameSuccess,
    tenModifyGroupNameNoPermissionError,

    tenModifyBaseOperationMaskSuccess,
    tenModifyBaseOperationMaskNoPermissionError,

    tenModifyAdvanceOperationMaskSuccess,
    tenModifyAdvanceOperationMaskNoPermissionError,

    tenModifyGroupWageSuccess,
    tenModifyGroupWageNoPermissionError,

    tenModifyMemorabiliaSuccess,
    tenModifyMemorabiliaError,

    tenQuitNotInTongError,
    tenQuitIsMasterError,

    tenChangeMemberGroupSuccess,
    tenChangeMemberGroupError,

    tenChangeMasterSuccess,
    tenChangeMasterError,

    tenChangeMemberRemarkSuccess,
    tenChangeMemberRemarkError,

    tenInviteTargetInRivalCampError,
    tenTargetTongIsRivalCampError,

    tenChangeCampPermissionError,
    tenChangeCampLimitError,

    tenCreateTongNameIllegalError,
    tenCreateTongAlreadyInTongError,
    tenCreateTongNameConflictError,

    tenRepertoryGridFilledError,
    tenRepertoryPageFullError,
    tenPutItemInRepertorySuccess,
    tenItemNotInRepertoryError,
    tenTakeItemFromRepertorySuccess,
    tenExchangeRepertoryItemSuccess,

    tenTakeRepertoryItemPermissionDenyError,
    tenPutItemToRepertoryPermissionDenyError,
    tenStackItemToRepertoryFailError,

    tenSaveMoneySuccess,
    tenSaveMoneyTooMushError,
    tenPaySalarySuccess,
    tenPaySalaryFailedError,

    tenRenameSuccess,
    tenRenameNoRightError,
    tenRenameUnnecessaryError,
    tenRenameConflictError,
    tenRenameIllegalError,

    tenTotal
};

enum CAMP_RESULT_CODE
{
    crtInvalid,

    crtFaild,
    crtSuccess,

    crtTongConflict,
    crtInTeam,

    crtTotal
};

enum CHARGE_LIMIT_CODE
{
    cltInvalid,

    cltTalkWorld,
    cltTalkCamp,
    cltYouTrading,
    cltTargetTrading,
    cltMail,
    cltAuctionBid,
    cltAuctionSell,
    cltYouJoinTong,
    cltTargetJoinTong,
    cltLoginMessage,

    cltTotal
};

enum ENCHANT_RESULT_CODE
{
    ercInvalid,
    
    ercSuccess,
    ercPackageIsFull,
    ercCannotUnMount,
    ercFailed,

    ercTotal
};
#endif	//_SO3_RESULT_H_
