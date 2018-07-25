#include "stdafx.h"
#include "KPlayerServer.h"
#include "KItemList.h"
#include "KRoleDBDataDef.h"
#include "KSO3World.h"
#include "KPlayerServer.h"
#include "KItem.h"
#include "KPlayer.h"
#include "KItemLib.h"
#ifdef _CLIENT
#include "SO3Represent/SO3GameWorldRepresentHandler.h"
#include "SO3UI/SO3GameWorldUIHandler.h"
#endif

#define TEMP_ENCHANT_TIME_CHECK_FREQUENCY  (GAME_FPS * 60)

// װ���������
EQUIPMENT_SUB_TYPE g_CanEquipList[eitTotal] =
{
    estMeleeWeapon, estRangeWeapon, estChest,   estHelm,       estAmulet,  estRing,        estRing,    
    estWaist,       estPendant,     estPants,   estBoots,      estBangle,  estWaistExtend, estPackage, 
    estPackage,     estPackage,     estPackage, estPackage,    estPackage, estPackage,     estPackage,
	estPackage,     estArrow,       estBackExtend, estHorse
};

DWORD g_GetBoxIndexFromSlot(DWORD dwSlot)
{
	DWORD nResult = 0;
	switch(dwSlot)
	{
	case eitPackage1:
		nResult = ibPackage1;
		break;
	case eitPackage2:
		nResult = ibPackage2;
		break;
	case eitPackage3:
		nResult = ibPackage3;
		break;
	case eitPackage4:
		nResult = ibPackage4;
		break;
	case eitBankPackage1:
		nResult = ibBankPackage1;
		break;
	case eitBankPackage2:
		nResult = ibBankPackage2;
		break;
	case eitBankPackage3:
		nResult = ibBankPackage3;
		break;
	case eitBankPackage4:
		nResult = ibBankPackage4;
		break;
	case eitBankPackage5:
		nResult = ibBankPackage5;
		break;
	default:
		break;
	}

	return nResult;
}

BOOL KItemList::Init(KPlayer* pPlayer)
{
    BOOL bResult    = false;
    BOOL bRetCode   = false;

    assert(pPlayer);

    m_nMoney            = 0;
    m_nMoneyLimit       = MONEY_LIMIT;

    m_bBankOpened       = false;

#ifdef _SERVER
    m_bSoldListOpened   = false;
#endif

    m_nEnabledBankPackageCount = 0;

    m_pPlayer = pPlayer;

    bRetCode = m_Box[ibEquip].Init(eitTotal, ivtEquipment);
    KGLOG_PROCESS_ERROR(bRetCode);

    bRetCode = m_Box[ibPackage].Init(FIRST_PACKAGE_SIZE, ivtPackage);
    KGLOG_PROCESS_ERROR(bRetCode);

    bRetCode = m_Box[ibPackage1].Init(0, ivtPackage);
    KGLOG_PROCESS_ERROR(bRetCode);

    bRetCode = m_Box[ibPackage2].Init(0, ivtPackage);
    KGLOG_PROCESS_ERROR(bRetCode);

    bRetCode = m_Box[ibPackage3].Init(0, ivtPackage);
    KGLOG_PROCESS_ERROR(bRetCode);

	bRetCode = m_Box[ibPackage4].Init(0, ivtPackage);
	KGLOG_PROCESS_ERROR(bRetCode);

    bRetCode = m_Box[ibBank].Init(INIT_BANK_SIZE, ivtBank);
    KGLOG_PROCESS_ERROR(bRetCode);

	bRetCode = m_Box[ibBankPackage1].Init(0, ivtBank);
	KGLOG_PROCESS_ERROR(bRetCode);

	bRetCode = m_Box[ibBankPackage2].Init(0, ivtBank);
	KGLOG_PROCESS_ERROR(bRetCode);

	bRetCode = m_Box[ibBankPackage3].Init(0, ivtBank);
	KGLOG_PROCESS_ERROR(bRetCode);

	bRetCode = m_Box[ibBankPackage4].Init(0, ivtBank);
	KGLOG_PROCESS_ERROR(bRetCode);

	bRetCode = m_Box[ibBankPackage5].Init(0, ivtBank);
	KGLOG_PROCESS_ERROR(bRetCode);

    bRetCode = m_Box[ibSoldList].Init(MAX_SOLDLIST_PACKAGE_SIZE, ivtSoldlist);
    KGLOG_PROCESS_ERROR(bRetCode);

    bResult = true;
Exit0:
    return bResult;
}

void KItemList::UnInit()
{
    KItem* pItem = NULL;

    for (int i = 0; i < ibTotal; i++)
    {
        for (DWORD j = 0; j < m_Box[i].m_dwSize; j++)
        {
            pItem = m_Box[i].GetItem(j);
            if (pItem)
            {
                g_pSO3World->m_ItemManager.FreeItem(pItem);
                pItem = NULL;
            }
        }
    }
}

#ifdef _SERVER
BOOL KItemList::Activate()
{    
    if ((g_pSO3World->m_nGameLoop - m_pPlayer->m_dwID) % TEMP_ENCHANT_TIME_CHECK_FREQUENCY == 0)
    {
        for (DWORD dwBoxIndex = 0; dwBoxIndex < ibTotal; dwBoxIndex++)
        {
            if (dwBoxIndex == ibSoldList)
            {
                continue;
            }

            for (DWORD dwX = 0; dwX < m_Box[dwBoxIndex].m_dwSize; dwX++)
            {
                KItem* pItem = m_Box[dwBoxIndex].GetItem(dwX);
                
                if (pItem == NULL)
                {
                    continue;
                }

                if (pItem->m_nEnchantTime > 0 && pItem->m_nEnchantTime <= g_pSO3World->m_nCurrentTime)
                {
                    RemoveEnchant(dwBoxIndex, dwX, false);
                }
            }
        }
    }

    return true;
}
#endif

ADD_ITEM_RESULT_CODE KItemList::CanAddItem(KItem* pItem)
{
    ADD_ITEM_RESULT_CODE    nResult         = aircFailed;
    ADD_ITEM_RESULT_CODE    eRetCode        = aircFailed;
    BOOL                    bRetCode        = false;
    int                     nAddAmount      = 0;
    int                     nExistAmount    = 0;
    int                     nMaxExistAmount = 0;
    int                     nCanPushAmount  = 0;
    DWORD                   dwX             = 0;
    KItem*                  pDestItem       = NULL;

    assert(pItem);

    if (pItem->IsStackable())
    {
        nAddAmount = pItem->GetStackNum();
    
        nExistAmount = GetItemTotalAmount(pItem->m_GenParam.dwTabType, pItem->m_GenParam.dwIndex);
        nMaxExistAmount = pItem->GetMaxExistAmount();

        KG_PROCESS_ERROR_RET_CODE(nMaxExistAmount == 0 || (nExistAmount + nAddAmount) <= nMaxExistAmount, aircItemAmountLimited);

        for (DWORD dwBox = ibPackage; dwBox <= ibPackage4; ++dwBox)
        {
            for (DWORD dwX = 0; dwX < m_Box[dwBox].m_dwSize; ++dwX)
            {
                pDestItem = m_Box[dwBox].m_pItemArray[dwX];
                if (pDestItem && pDestItem->CanStack(pItem))
                {
                    nCanPushAmount += pDestItem->GetMaxStackNum() - pDestItem->GetStackNum();
                    KG_PROCESS_SUCCESS(nCanPushAmount >= nAddAmount);
                }
            }
        }
    }
    
    for (DWORD dwIndex = ibPackage; dwIndex <= ibPackage4; ++dwIndex)
    {
        bRetCode = FindFreeRoom(dwIndex, dwX);
        if (!bRetCode)
        {
            continue;
        }
        
        eRetCode = CanAddItemToPos(pItem, dwIndex, dwX);
        KG_PROCESS_ERROR_RET_CODE(eRetCode != aircItemAmountLimited, eRetCode);

        if (eRetCode != aircSuccess)
        {
            continue;
        }
        goto Exit1;
    }
    
    KG_PROCESS_ERROR_RET_CODE(false, aircNotEnoughFreeRoom);

Exit1:
    nResult = aircSuccess;
Exit0:
    return nResult;
}

ADD_ITEM_RESULT_CODE KItemList::CanAddItemToPos(KItem* pItem, DWORD dwDestBox, DWORD dwDestX)
{
    ADD_ITEM_RESULT_CODE	nResult             = aircFailed;
    ITEM_RESULT_CODE        eRetCode            = ircFailed;
    int                     nAddAmount          = 0;
    int                     nExistAmount        = 0;
    int                     nMaxExistAmount     = 0;
    KItem*                  pDestItem           = NULL;

    assert(pItem);
    
    pDestItem = GetItem(dwDestBox, dwDestX);
    KG_PROCESS_ERROR_RET_CODE(pDestItem == NULL, aircCannotPutThatPlace);

    nAddAmount = pItem->GetStackNum();
    nExistAmount = GetItemTotalAmount(pItem->m_GenParam.dwTabType, pItem->m_GenParam.dwIndex);
    nMaxExistAmount = pItem->GetMaxExistAmount();

    KG_PROCESS_ERROR_RET_CODE(nMaxExistAmount == 0 || (nExistAmount + nAddAmount) <= nMaxExistAmount, aircItemAmountLimited);

    eRetCode = CanExchangeItemToPos(pItem, dwDestBox, dwDestX);
    KG_PROCESS_ERROR(eRetCode == ircSuccess);

    nResult = aircSuccess;
Exit0:
    return nResult;
}

ADD_ITEM_RESULT_CODE KItemList::AddItem(KItem* pItem, BOOL bNotifyClient)
{
    ADD_ITEM_RESULT_CODE     nResult        = aircFailed;
    ADD_ITEM_RESULT_CODE     eRetCode       = aircFailed;
    BOOL                     bRetCode       = false;
    int	                     nPlace         = 0;
    int                      nItemAmount    = 0;
    int                      nCanPushAmount = 0;

    assert(m_pPlayer);
    assert(pItem);
    
    eRetCode = CanAddItem(pItem);
    KG_PROCESS_ERROR_RET_CODE(eRetCode == aircSuccess, eRetCode);
    
    if (pItem->IsStackable())
    {
        int nAddAmount = pItem->GetStackNum();

        for (DWORD dwBox = ibPackage; dwBox <= ibPackage4; ++dwBox)
        {
            for (DWORD dwX = 0; dwX < m_Box[dwBox].m_dwSize; ++dwX)
            {
                if (m_Box[dwBox].m_pItemArray[dwX] && m_Box[dwBox].m_pItemArray[dwX]->CanStack(pItem))
                {
                    nCanPushAmount += pItem->GetMaxStackNum() - m_Box[dwBox].m_pItemArray[dwX]->GetStackNum();
                    if (nCanPushAmount >= nAddAmount)
                    {
                        break;
                    }
                }
            }
        }
        
        if (nCanPushAmount >= nAddAmount)
        {
            for (int i = ibPackage; i <= ibPackage4; ++i)
            {
                bRetCode = PushItem(pItem, i, bNotifyClient);
                assert(bRetCode);

                nItemAmount = pItem->GetStackNum();
                if (nItemAmount == 0)
                {
                    break;
                }
            }
            assert(nItemAmount == 0);

            g_pSO3World->m_ItemManager.FreeItem(pItem);
            pItem = NULL;

            goto Exit1;
        }
    }

    nItemAmount = pItem->GetStackNum(); // Ҫ��ӵĵ�����

    for (int i = ibPackage; i <= ibPackage4; ++i)
    {
        DWORD dwPlace = 0;
        
        bRetCode = FindFreeRoom(i, dwPlace);
        if (!bRetCode)
        {
            continue;
        }

        eRetCode = CanAddItemToPos(pItem, i, dwPlace);
        if (eRetCode != aircSuccess)
        {
            continue;
        }

        bRetCode = m_Box[i].PlaceItem(pItem, dwPlace);
        _ASSERT(bRetCode);
#ifdef _SERVER
        g_PlayerServer.DoSyncItemData(m_pPlayer->m_nConnIndex, m_pPlayer->m_dwID, pItem, i, dwPlace);
        if (bNotifyClient)
        {
            g_PlayerServer.DoAddItemNotify(m_pPlayer, pItem->m_dwID, nItemAmount);
        }
#endif
        goto Exit1;
    }
    
    goto Exit0;

Exit1:
    nResult = aircSuccess;
Exit0:
    return nResult;
}

ADD_ITEM_RESULT_CODE KItemList::AddItemToPos(KItem* pItem, DWORD dwBoxIndex, DWORD dwX)
{
    ADD_ITEM_RESULT_CODE    nResult    = aircFailed;
    ADD_ITEM_RESULT_CODE    eRetCode   = aircFailed;
    BOOL                    bRetCode   = false;
    KCustomEquipInfo*       pEquipInfo = NULL;

    assert(pItem);
    KGLOG_PROCESS_ERROR(dwBoxIndex < ibTotal);

#ifdef _SERVER
    eRetCode = CanAddItemToPos(pItem, dwBoxIndex, dwX);
    KG_PROCESS_ERROR_RET_CODE(eRetCode == aircSuccess, eRetCode);
#endif

    bRetCode = m_Box[dwBoxIndex].PlaceItem(pItem, dwX);
    KGLOG_PROCESS_ERROR(bRetCode);

#ifdef _SERVER
    g_PlayerServer.DoSyncItemData(m_pPlayer->m_nConnIndex, m_pPlayer->m_dwID, pItem, dwBoxIndex, dwX);
#endif

    if (dwBoxIndex == ibEquip)
    {
        DWORD dwSlotBox = 0;

        bRetCode = Equip(pItem, dwX);
        KGLOG_CHECK_ERROR(bRetCode);

		dwSlotBox = g_GetBoxIndexFromSlot(dwX);
		if (dwSlotBox)
		{
            KGLOG_PROCESS_ERROR(
                pItem->m_GenParam.dwTabType == ittCustWeapon    || 
                pItem->m_GenParam.dwTabType == ittCustArmor     ||
                pItem->m_GenParam.dwTabType == ittCustTrinket 
            );

            pEquipInfo = (KCustomEquipInfo*)pItem->GetItemInfo();
			KGLOG_PROCESS_ERROR(pEquipInfo);
            KGLOG_PROCESS_ERROR(pEquipInfo->nSub == estPackage);
            EnablePackage(dwSlotBox, pItem->m_nPackageSize, pEquipInfo->nPackageGenerType, pEquipInfo->nPackageSubType);
		}
    }

    nResult = aircSuccess;
Exit0:
    return nResult;
}

KItem* KItemList::PickUpItem(DWORD dwBoxIndex, DWORD dwX, BOOL bSync)
{
    BOOL    bRetCode    = false;
    KItem*  pItem       = NULL;
Exit0:
    return pItem;
}

//���������е���������Ʒ
KItem* KItemList::GetItem(DWORD dwBoxIndex, DWORD dwX)
{
    KItem* pItem = NULL;

    KGLOG_PROCESS_ERROR(dwBoxIndex < ibTotal);

    pItem = m_Box[dwBoxIndex].GetItem(dwX);

Exit0:
    return pItem;
}

DWORD KItemList::GetItemAmountInEquipAndPackage(DWORD dwTabType, DWORD dwIndex, int nBookRecipeID)
{
    DWORD dwResult      = 0;
    DWORD dwItemAmount  = 0;

    for (int i = ibEquip; i <= ibPackage4; ++i)
    {
        dwItemAmount = GetItemAmountInBox(i, dwTabType, dwIndex, nBookRecipeID);
        dwResult += dwItemAmount;
    }

    return dwResult;
}

DWORD KItemList::GetItemTotalAmount(DWORD dwTabType, DWORD dwIndex, int nBookRecipeID)
{
    DWORD dwResult      = 0;
    DWORD dwItemAmount  = 0;

    for (int i = 0; i < ibTotal; ++i)
    {
        if (m_Box[i].m_nBoxType == ivtSoldlist)
        {
            continue;
        }

        dwItemAmount = GetItemAmountInBox(i, dwTabType, dwIndex, nBookRecipeID);
        dwResult += dwItemAmount;
    }

    return dwResult;
}

DWORD KItemList::GetItemAmountInBox(
    int nBoxIndex, DWORD dwTabType, DWORD dwIndex, int nBookRecipeID
)
{
    DWORD dwResult = 0;
    
    KGLOG_PROCESS_ERROR(nBoxIndex >= ibEquip && nBoxIndex < ibTotal);

    for (DWORD dwX = 0; dwX < m_Box[nBoxIndex].m_dwSize; dwX++)
    {
        KItem* pItem = m_Box[nBoxIndex].GetItem(dwX);
        
        if (pItem == NULL)
            continue;
        
        if (pItem->m_GenParam.dwTabType != dwTabType || pItem->m_GenParam.dwIndex != dwIndex)
            continue;
        
        if (nBookRecipeID != -1)
        {
            if (pItem->m_Common.nGenre != igBook || pItem->m_nCurrentDurability != nBookRecipeID)
                continue;
        }
            
        dwResult += pItem->GetStackNum();
    }

Exit0:
    return dwResult;
}

BOOL KItemList::GetItemPos(int nVersion, DWORD dwTabType, DWORD dwIndex, DWORD &dwBox, DWORD &dwX)
{
    return false;
}

BOOL KItemList::GetItemPos(DWORD dwTabType, DWORD dwIndex, DWORD& dwBox, DWORD& dwX)
{
	for (int i = 0; i < ibTotal; ++i)
	{
        if ((m_Box[i].m_nBoxType == ivtBank && !m_bBankOpened) || m_Box[i].m_nBoxType == ivtSoldlist)
        {
            continue;
        }

		for (int j = 0; j < (int)m_Box[i].m_dwSize; ++j)
		{
			KItem* pItem = m_Box[i].GetItem(j);
			if (
                pItem && pItem->m_GenParam.dwTabType == dwTabType && 
				pItem->m_GenParam.dwIndex == dwIndex
            )
			{
				dwBox = i;
				dwX = j;
				return true;
			}
		}
	}

	return false;
}

BOOL KItemList::GetItemPos(DWORD dwID, DWORD &dwBox, DWORD &dwX)
{
    for (int i = 0; i < ibTotal; ++i)
    {
        if ((m_Box[i].m_nBoxType == ivtBank && !m_bBankOpened) || m_Box[i].m_nBoxType == ivtSoldlist)
        {
            continue;
        }

        for (int j = 0; j < (int)m_Box[i].m_dwSize; ++j)
        {
            KItem* pItem = m_Box[i].GetItem(j);
            if (pItem && pItem->m_dwID == dwID)
            {
                dwBox = i;
                dwX = j;
                return true;
            }
        }
    }

    return false;
}

int	KItemList::GetBoxType(DWORD dwBox)
{
    int nResult = ivtTotal;

    KGLOG_PROCESS_ERROR(dwBox < ibTotal);

    nResult = m_Box[dwBox].m_nBoxType;
Exit0:
    return nResult;
}

#ifdef _SERVER
BOOL KItemList::CostItemInEquipAndPackage(
    DWORD dwTabType, DWORD dwIndex, DWORD dwAmount, int nBookRecipeID
)
{
    BOOL    bResult         = false;
    BOOL    bRetCode        = false;
    DWORD   dwItemAmount    = 0;

    dwItemAmount = GetItemAmountInEquipAndPackage(dwTabType, dwIndex, nBookRecipeID);
    KGLOG_PROCESS_ERROR(dwItemAmount >= dwAmount);

    for (int i = ibEquip; i <= ibPackage4; ++i)
    {
        if (dwAmount == 0)
            break;

        bRetCode = CostItemInBox(i, dwTabType, dwIndex, dwAmount, nBookRecipeID);
        KGLOG_PROCESS_ERROR(bRetCode);
    }

    KGLOG_PROCESS_ERROR(dwAmount == 0);

    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::CostItemInAllBox(
    DWORD dwTabType, DWORD dwIndex, DWORD dwAmount, int nBookRecipeID
)
{
    BOOL    bResult         = false;
    BOOL    bRetCode        = false;
    
    for (int i = 0; i < ibTotal; ++i)
    {
        if (i == ibSoldList)
        {
            continue;
        }

        if (dwAmount == 0)
            break;

        bRetCode = CostItemInBox(i, dwTabType, dwIndex, dwAmount, nBookRecipeID);
        KGLOG_PROCESS_ERROR(bRetCode);
    }

Exit1:
    bResult = true;
Exit0:
    return bResult;
}
#endif

BOOL KItemList::CostSingleItem(DWORD dwBoxIndex, DWORD dwX, int nAmount)
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;
    KItem*  pItem       = NULL;
    
    pItem = GetItem(dwBoxIndex, dwX);
    KGLOG_PROCESS_ERROR(pItem);

    KGLOG_PROCESS_ERROR(nAmount <= pItem->m_nCurrentDurability);

    if (pItem->m_nCurrentDurability == nAmount)
    {
        if (pItem->GetGenre() == igEquipment && !pItem->IsStackable())
        {
            if (dwBoxIndex == ivtEquipment)
            {
				if (pItem->m_bApplyed)
				{
					bRetCode = UnApplyAttrib(pItem);	
					KGLOG_PROCESS_ERROR(bRetCode);
				}

                pItem->m_nCurrentDurability = 0;
            }
        }
        else
        {
            TItemPos ItemPos = {dwBoxIndex, dwX};
            bRetCode = DestroyItem(ItemPos);
            KGLOG_PROCESS_ERROR(bRetCode == ircSuccess);
        }
    }
    else
    {
        pItem->m_nCurrentDurability -= nAmount;
    }

#ifdef _SERVER
    g_PlayerServer.DoUpdateItemAmount(m_pPlayer->m_nConnIndex, dwBoxIndex, dwX, pItem->m_nCurrentDurability);
#endif

    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::FindFreeRoom(DWORD dwBoxIndex, DWORD& dwX)
{
    KGLOG_PROCESS_ERROR(dwBoxIndex < ibTotal);

    for (DWORD dwPos = 0; dwPos < m_Box[dwBoxIndex].m_dwSize; ++dwPos)
    {
        if (m_Box[dwBoxIndex].m_pItemArray[dwPos] == NULL)
        {
            dwX = dwPos;
            return true;
        }
    }

Exit0:
    return false;
}

// ����һ���е��߲��ҿ��Ե���ָ����Ʒ�Ŀո�
BOOL KItemList::FindStackRoom(DWORD dwBoxIndex, DWORD& dwX, KItem* pItem)
{	
	BOOL bResult = false;

	assert(pItem);

	KGLOG_PROCESS_ERROR(dwBoxIndex < ibTotal);

	for (dwX = 0; dwX < m_Box[dwBoxIndex].m_dwSize; dwX++)
	{
		KItem* pTargetItem = m_Box[dwBoxIndex].GetItem(dwX);
		if (pTargetItem && pTargetItem->CanStack(pItem))
		{
			if (pTargetItem->m_nStackNum + pItem->m_nStackNum <= pTargetItem->m_nMaxStackNum)
			{
				goto Exit1;
			}
		}
	}
    
    goto Exit0;

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::FindFreeSoldList(DWORD& dwX)
{
    BOOL        bResult         = false;
    BOOL        bRetCode        = false;
    int         nResult         = -1;
    int         nDelPos         = -1;

    nResult = m_Box[ibSoldList].FindFreeSoldList(nDelPos);
    KGLOG_PROCESS_ERROR(nResult >= 0);

    dwX = nResult;

#ifdef _SERVER
    if (nDelPos != -1)
    {
        g_PlayerServer.DoSyncItemDestroy(m_pPlayer->m_nConnIndex, m_pPlayer->m_dwID, ibSoldList, nDelPos);
    }
#endif

    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::FindFreeRoomInPackage(
    DWORD& dwBoxIndex, DWORD& dwX, int nItemGenerType, int nItemSubType
)
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;
    DWORD   dwPos       = 0;

    for (DWORD dwBox = ibPackage; dwBox <= ibPackage4; ++dwBox)
    {
        KInventory* pInventory = &m_Box[dwBox];
        
        bRetCode = pInventory->CheckContainItemType(nItemGenerType, nItemSubType);
        if (!bRetCode)
        {
            continue;
        }

        bRetCode = FindFreeRoom(dwBox, dwPos);
        if (bRetCode)
        {
            dwBoxIndex = dwBox;
            dwX        = dwPos;

            goto Exit1;
        }
    }

    goto Exit0;

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::FindStackRoomInPackage(DWORD& dwBoxIndex, DWORD& dwX, KItem* pItem)
{
    BOOL    bResult     = false;
	BOOL    bRetCode    = false;
    DWORD   dwBox       = 0;
    DWORD   dwPos       = 0;

	assert(pItem);

	// ��Ѱ�ҿɵ��ӵ�λ��
	for (dwBox = ibPackage; dwBox < ibBank; ++dwBox)
	{
		bRetCode = FindStackRoom(dwBox, dwPos, pItem);
        if (bRetCode)
        {
            dwBoxIndex = dwBox;
            dwX        = dwPos;
            
            goto Exit1;
        }
	}

	// ���ҿո�
	for (dwBox = ibPackage; dwBox < ibBank; ++dwBox)
	{
        bRetCode = m_Box[dwBox].CheckContainItemType(pItem);
        if (!bRetCode)
        {
            continue;
        }
        
		bRetCode = FindFreeRoom(dwBox, dwPos);
        if (bRetCode)
        {
            dwBoxIndex = dwBox;
            dwX        = dwPos;

            goto Exit1;
        }
	}
    
    goto Exit0;

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::FindStackRoomInBank(DWORD& dwBoxIndex, DWORD& dwX, KItem* pItem)
{
    BOOL    bResult     = false;
    BOOL    bRetCode    = false;
    DWORD   dwBox       = 0;
    DWORD   dwPos       = 0;

    assert(pItem);
    
    KGLOG_PROCESS_ERROR(m_bBankOpened);

    // ��Ѱ�ҿɵ��ӵ�λ��
    for (dwBox = ibBank; dwBox <= ibBankPackage5; ++dwBox)
    {
        bRetCode = FindStackRoom(dwBox, dwPos, pItem);
        if (bRetCode)
        {
            dwBoxIndex = dwBox;
            dwX        = dwPos;

            goto Exit1;
        }
    }

    // ���ҿո�
    for (dwBox = ibBank; dwBox <= ibBankPackage5; ++dwBox)
    {
        bRetCode = m_Box[dwBox].CheckContainItemType(pItem);
        if (!bRetCode)
        {
            continue;
        }
        
        bRetCode = FindFreeRoom(dwBox, dwPos);
        if (bRetCode)
        {
            dwBoxIndex = dwBox;
            dwX        = dwPos;

            goto Exit1;
        }
    }

    goto Exit0;

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

int KItemList::GetAllPackageFreeRoomSize(void)
{
    int         nRetSize        = -1;
    int         nFreeRoomSize   = 0;
    KInventory* pInventory      = NULL;

    for (int nBoxIndex = ibPackage; nBoxIndex <= ibPackage4; ++nBoxIndex)
    {
        pInventory = &m_Box[nBoxIndex];
        
        if (m_Box[nBoxIndex].m_nContainItemGenerType != INVALID_CONTAIN_ITEM_TYPE) // ���Ҳ��ϰ��Ŀո�
        {
            continue;
        }

        for (DWORD dwX = 0; dwX < pInventory->m_dwSize; dwX++)
        {
            KItem* pItem = pInventory->GetItem(dwX);
            if (pItem == NULL)
            {
                ++nFreeRoomSize;
            }
        }
    }
    
    nRetSize = nFreeRoomSize;
Exit0:
    return nRetSize;
}

int KItemList::GetBoxFreeRoomSize(int nBoxIndex)
{
    int         nRetSize        = -1;
    int         nFreeRoomSize   = 0;
    KInventory* pInventory      = NULL;

    KGLOG_PROCESS_ERROR(nBoxIndex >= ibEquip && nBoxIndex < ibTotal);
    KG_PROCESS_ERROR(m_Box[nBoxIndex].m_nBoxType != ivtBank || m_bBankOpened);
    KG_PROCESS_ERROR(m_Box[nBoxIndex].m_nBoxType != ivtSoldlist);

    pInventory = &m_Box[nBoxIndex];
    for (DWORD dwX = 0; dwX < pInventory->m_dwSize; dwX++)
    {
        KItem* pItem = pInventory->GetItem(dwX);
        if (pItem == NULL)
        {
            ++nFreeRoomSize;
        }
    }
    
    nRetSize = nFreeRoomSize;
Exit0:
    return nRetSize;
}

BOOL KItemList::SetMoney(int nMoney)
{
    BOOL bResult = false;

    KG_PROCESS_ERROR(nMoney <= m_nMoneyLimit);

    m_nMoney = nMoney;

#ifdef _SERVER
    assert(m_pPlayer);
    g_PlayerServer.DoSyncMoney(m_pPlayer->m_nConnIndex, m_nMoney, false);
#endif

    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::AddMoney(int nMoney, BOOL bShowMsg)
{
    BOOL        bResult     = false;

    assert(m_pPlayer);

    if (nMoney >= 0)
    {
        if (m_nMoney > INT_MAX - nMoney)
        {
            m_nMoney = INT_MAX;
        }
        else
        {
            m_nMoney += nMoney;
        }
    }
    else
    {
        if (m_nMoney < INT_MIN - nMoney)
        {
            m_nMoney = INT_MIN;
        }
        else
        {
            m_nMoney += nMoney;
        }
    }

    if (m_nMoney > m_nMoneyLimit)
    {
        m_nMoney = m_nMoneyLimit;
    }

#ifdef _SERVER
    g_PlayerServer.DoSyncMoney(m_pPlayer->m_nConnIndex, m_nMoney, bShowMsg);
#endif

    bResult = true;
Exit0:
    return bResult;
}

ITEM_RESULT_CODE KItemList::CanExchangeItemToPos(KItem* pItem, DWORD dwDestBox, DWORD dwDestX)
{
    ITEM_RESULT_CODE	nResult             = ircFailed;
    BOOL	            bRetCode            = false;
    ITEM_RESULT_CODE    eRetCode            = ircFailed;
    int                 nCurrentDurability  = 0;
    DWORD               dwNewBox            = 0;

    KG_PROCESS_SUCCESS(pItem == NULL);

    KGLOG_PROCESS_ERROR(dwDestBox < ibTotal);

    assert(m_pPlayer);

    switch(dwDestBox)
    {
    case ibEquip:
        // ������Ƿŵ�װ�����ı����ϣ������Ʒ���ͱ�����װ��
        dwNewBox = g_GetBoxIndexFromSlot(dwDestX);
        KG_PROCESS_ERROR(dwNewBox != 0 || pItem->m_Common.nGenre == igEquipment); 

        if (dwNewBox == 0) // ��װ�������Ϸ�
        {
            eRetCode = CanEquip(pItem, dwDestX);
            KG_PROCESS_ERROR_RET_CODE(eRetCode == ircSuccess, eRetCode);
        }
        else	// �򱳰��Ϸ�
        {
            KItem* pPackage = m_Box[ibEquip].GetItem(dwDestX);
            EQUIPMENT_INVENTORY_TYPE eInventoryType = (EQUIPMENT_INVENTORY_TYPE)dwDestX;

            //������б�������������Ƿ񼤻�
            if (eInventoryType >= eitBankPackage1 && eInventoryType <= eitBankPackage5)
            {
                bRetCode = IsBankPackageEnabled(eInventoryType);
                KG_PROCESS_ERROR(bRetCode);
            }

            if (pPackage)
            {
                if (pItem->m_Common.nSub == estPackage)
                {
                    assert(dwNewBox < ibTotal);

                    int nUseCount = m_Box[dwNewBox].GetUsedCount();

                    // ����������ͬ�����»���ȥ�İ������������ͣ�����Ŀ������޶�������Դ���Ѿ�װ��
                    KG_PROCESS_ERROR_RET_CODE(
                        pPackage->m_Common.nDetail == pItem->m_Common.nDetail || 
                        pItem->m_Common.nDetail == 0 || nUseCount == 0 ||
                        pItem->m_bEquiped, ircCannotPutToThatPlace
                    );

                    KG_PROCESS_ERROR_RET_CODE(pItem->m_nPackageSize >= nUseCount, ircCannotPutToThatPlace);
                }
                else
                {
                    KG_PROCESS_ERROR_RET_CODE(m_Box[dwNewBox].m_dwUsed < m_Box[dwNewBox].m_dwSize, ircPackageNotEnoughSize);
                    
                    bRetCode = m_Box[dwNewBox].CheckContainItemType(pItem);
                    KG_PROCESS_ERROR_RET_CODE(bRetCode, ircCannotPutToThatPlace);
                }
            }
            else
            {
                eRetCode = CanEquip(pItem, dwDestX);
                KG_PROCESS_ERROR_RET_CODE(eRetCode == ircSuccess, eRetCode);
            }
        }
        break;

    case ibPackage:
    case ibPackage1:
    case ibPackage2:
    case ibPackage3:
    case ibPackage4:
    case ibBank:
    case ibBankPackage1:
    case ibBankPackage2:
    case ibBankPackage3:
    case ibBankPackage4:
    case ibBankPackage5:
        bRetCode = m_Box[dwDestBox].CheckContainItemType(pItem);
        KG_PROCESS_ERROR_RET_CODE(bRetCode, ircCannotPutToThatPlace);
        break;

    case ibSoldList:
        break;
    }

Exit1:
    nResult = ircSuccess;
Exit0:
    return nResult;
}

ITEM_RESULT_CODE KItemList::CanExchangeItem(DWORD dwSrcBox, DWORD dwSrcX, DWORD dwDestBox, DWORD dwDestX)
{
    ITEM_RESULT_CODE	nResult         = ircFailed;
    ITEM_RESULT_CODE    eRetCode        = ircFailed;
    KItem*              pSrcItem        = NULL;
    KItem*              pDestItem       = NULL;

    KG_PROCESS_ERROR(dwSrcBox >= ibEquip  && dwSrcBox < ibTotal);
    KG_PROCESS_ERROR(dwDestBox >= ibEquip && dwDestBox < ibTotal);

    KG_PROCESS_ERROR_RET_CODE(m_pPlayer->m_eMoveState != cmsOnDeath, ircPlayerIsDead);
   
#ifdef _SERVER
    if (dwSrcBox == ibSoldList || dwDestBox == ibSoldList)
    {
        KG_PROCESS_ERROR(m_bSoldListOpened); // �����Ѵ�
    }
#endif
    
    if (m_Box[dwSrcBox].m_nBoxType == ivtBank || m_Box[dwDestBox].m_nBoxType == ivtBank)
    {
        KG_PROCESS_ERROR(m_bBankOpened); // �����Ѵ�
    }

    pSrcItem  = GetItem(dwSrcBox, dwSrcX); // Դ��Ʒ����Ϊ��
    KGLOG_PROCESS_ERROR(pSrcItem);
    
    if (dwSrcBox == ibEquip)
    {
        eRetCode = CanUnEquip(dwSrcX);
        KG_PROCESS_ERROR_RET_CODE(eRetCode == ircSuccess, eRetCode);
    }

    pDestItem  = GetItem(dwDestBox, dwDestX);
    
    if (dwDestBox == ibEquip && pDestItem)
    {
        eRetCode = CanUnEquip(dwDestX);
        KG_PROCESS_ERROR_RET_CODE(eRetCode == ircSuccess, eRetCode);
    }

    nResult = CanExchangeItemToPos(pSrcItem, dwDestBox, dwDestX);
    KG_PROCESS_ERROR_RET_CODE(nResult == ircSuccess, nResult);	
    
    if (IsPackageSlot(dwDestBox, dwDestX))
    {
        DWORD dwPackageBox = g_GetBoxIndexFromSlot(dwDestX);
        KG_PROCESS_ERROR_RET_CODE(dwPackageBox != dwSrcBox, ircCannotPutToThatPlace);
    }
    else // ���Ŀ���Ǳ�������������ļ��
    {
        nResult = CanExchangeItemToPos(pDestItem, dwSrcBox, dwSrcX);
        KG_PROCESS_ERROR_RET_CODE(nResult == ircSuccess, nResult);
    }

    if (IsPackageSlot(dwSrcBox, dwSrcX))
    {
        // �ж��Ƿ�ѱ����ŵ����Լ���
        DWORD dwPackageBox = g_GetBoxIndexFromSlot(dwSrcX);
        KG_PROCESS_ERROR_RET_CODE(dwPackageBox != dwDestBox, ircCannotPutToThatPlace);
        
        if (!IsPackageSlot(dwDestBox, dwDestX))
        {
            KG_PROCESS_ERROR_RET_CODE(m_Box[dwPackageBox].m_dwUsed == 0, ircCannotDisablePackage);
        }

        // Ŀ�겻Ϊ���������һ����������
        KItem* pDestItem = GetItem(dwDestBox, dwDestX);
        if (pDestItem)
        {
            KG_PROCESS_ERROR_RET_CODE(
                pDestItem->m_Common.nGenre == igEquipment && pDestItem->m_Common.nSub == estPackage,
                ircCannotPutToThatPlace
            );
        }
    }

    nResult = ircSuccess;
Exit0:
    return nResult;
}

ITEM_RESULT_CODE KItemList::ExchangeItem(const TItemPos& PosSrc, const TItemPos& PosDest, DWORD dwAmount)
{
    ITEM_RESULT_CODE	nResult             = ircFailed;
    BOOL				bRetCode            = false;
    int					nRetCode            = 0;
    KItem*				pSrcItem            = NULL;
    KItem*				pDestItem           = NULL;
    KInventory*			pSrcBox             = NULL;
    KInventory*			pDestBox            = NULL;
	TItemPos            Src                 = PosSrc;
	TItemPos            Dest                = PosDest;
    DWORD               dwSrcItemTabType    = 0;
    DWORD               dwSrcItemIndex      = 0;

    assert(m_pPlayer);

	KG_PROCESS_ERROR_RET_CODE(m_pPlayer->m_eMoveState != cmsOnDeath, ircPlayerIsDead);

    // ���Դ��Ŀ��һ�£�˵�����û�������ʧ�ܡ�
    KGLOG_PROCESS_ERROR(Src.dwBox != Dest.dwBox || Src.dwX != Dest.dwX);

    KGLOG_PROCESS_ERROR(Src.dwBox < ibTotal && Src.dwX < m_Box[Src.dwBox].m_dwSize);
    KGLOG_PROCESS_ERROR(Dest.dwBox < ibTotal && Dest.dwX < m_Box[Dest.dwBox].m_dwSize);

    pSrcItem = GetItem(Src.dwBox, Src.dwX);
    // ��������ʱ���뱣֤Դ���߲�Ϊ��
    KGLOG_PROCESS_ERROR(pSrcItem);
    KGLOG_PROCESS_ERROR_RET_CODE(dwAmount <= (DWORD)pSrcItem->GetStackNum(), ircNotEnoughDurability);
    
    dwSrcItemTabType = pSrcItem->m_GenParam.dwTabType;
    dwSrcItemIndex   = pSrcItem->m_GenParam.dwIndex;

    // Ŀ������п���Ϊ��
    pDestItem = GetItem(Dest.dwBox, Dest.dwX);
    
    pSrcBox = &m_Box[Src.dwBox];
    pDestBox = &m_Box[Dest.dwBox];

#ifdef _SERVER // �ͻ��˲��ü��
    nResult = CanExchangeItem(Src.dwBox, Src.dwX, Dest.dwBox, Dest.dwX);
    KG_PROCESS_ERROR_RET_CODE(nResult == ircSuccess, nResult);
#endif

	// Ŀ��ŵ�������װ�������У�Ҫ�л�Ŀ��
	if (pDestItem && pSrcItem && pSrcItem->m_Common.nSub != estPackage && IsPackageSlot(Dest.dwBox, Dest.dwX))
	{
        DWORD   dwX         = 0;
		int     nBoxIndex   = g_GetBoxIndexFromSlot(Dest.dwX);
		KGLOG_PROCESS_ERROR(nBoxIndex);
        
        bRetCode = FindFreeRoom(nBoxIndex, dwX);
        KG_PROCESS_ERROR_RET_CODE(bRetCode, ircCannotPutToThatPlace);

		Dest.dwBox  = nBoxIndex;
		Dest.dwX    = dwX;
		pDestItem   = NULL;
		pDestBox    = &m_Box[nBoxIndex];
	}

    if (dwAmount == 0)
    {
        dwAmount = pSrcItem->GetStackNum();
    }

    // û����װ�������ƶ�
    if ((pSrcBox->m_nBoxType != ivtEquipment) && (pDestBox->m_nBoxType != ivtEquipment))
    {
        assert(pSrcItem);
        
        if (pDestItem)		// Ŀ���ж���
        {
            if (pDestItem->CanStack(pSrcItem))				// Ŀ����Ե���
            {
                nRetCode = pDestItem->Stack(dwAmount);
                // �Ų��µ�ʣ�ಿ�ֶѻ�ȥ
                nRetCode = pSrcItem->SetStackNum(pSrcItem->GetStackNum() - dwAmount + nRetCode);

                if (pSrcItem->GetStackNum() == 0)
                {
                    nRetCode = DestroyItem(pSrcItem, false);
                    KGLOG_CHECK_ERROR(nRetCode == ircSuccess);	// ����߻�����һ���ɵ��Ӳ��ɴݻٵĶ������ͻ�����LOG
                    pSrcItem = NULL;
                }
            }
            else											// ���ɵ��ӣ�����
            {
                KG_PROCESS_ERROR_RET_CODE(dwAmount == (DWORD)pSrcItem->GetStackNum(), ircCannotPutToThatPlace);

                KItem* pTemp;
                pTemp = pSrcBox->PickUpItem(Src.dwX);
                assert(pTemp == pSrcItem);
                pTemp = pDestBox->PickUpItem(Dest.dwX);
                assert(pTemp == pDestItem);
                bRetCode = pSrcBox->PlaceItem(pDestItem, Src.dwX);
                assert(bRetCode);
                bRetCode = pDestBox->PlaceItem(pSrcItem, Dest.dwX);
                assert(bRetCode);
            }
        }
        else	// Ŀ��û����
        {
            KItem* pTemp = NULL;
            if (dwAmount == (DWORD)pSrcItem->GetStackNum())	// ȫ�ƶ�
            {
                pTemp= pSrcBox->PickUpItem(Src.dwX);
                assert(pTemp == pSrcItem);
                bRetCode = pDestBox->PlaceItem(pSrcItem, Dest.dwX);
                assert(bRetCode);
            }
            else	// ��ֳ��µ���Ʒ
            {
#if defined(_SERVER)		// �����������Ʒ���ͻ��˲�������������Ʒͬ��
                pTemp = g_pSO3World->m_ItemManager.GenerateItem(pSrcItem->m_GenParam.nVersion, 
                    pSrcItem->m_GenParam.dwTabType, pSrcItem->m_GenParam.dwIndex, ERROR_ID,
                    pSrcItem->m_GenParam.dwRandSeed);

                bRetCode = pSrcItem->SetStackNum(pSrcItem->GetStackNum() - dwAmount);
                assert(bRetCode);

                bRetCode = g_PlayerServer.DoUpdateItemAmount(m_pPlayer->m_nConnIndex, Src.dwBox,
                    Src.dwX, pSrcItem->GetStackNum());
                KGLOG_CHECK_ERROR(bRetCode);

                bRetCode = pTemp->SetStackNum(dwAmount);	
                assert(bRetCode);

                bRetCode = m_Box[Dest.dwBox].PlaceItem(pTemp, Dest.dwX);
                assert(bRetCode);

                bRetCode = g_PlayerServer.DoSyncItemData(m_pPlayer->m_nConnIndex, m_pPlayer->m_dwID, pTemp, 
                    Dest.dwBox, Dest.dwX);
                KGLOG_CHECK_ERROR(bRetCode);

                goto Exit1;
#endif
            }
        }
#ifdef _CLIENT
        if (pSrcBox->m_nBoxType != ivtPackage && pDestBox->m_nBoxType == ivtPackage)
        {
            m_pPlayer->m_QuestList.OnUpdateItem(dwSrcItemTabType, dwSrcItemIndex, true);
        }
        else if (pSrcBox->m_nBoxType == ivtPackage && pDestBox->m_nBoxType != ivtPackage)
        {
            m_pPlayer->m_QuestList.OnUpdateItem(dwSrcItemTabType, dwSrcItemIndex, false);
        }
#endif
    }
    else	// װ����װ��
    {
        KGLOG_PROCESS_ERROR(dwAmount == 1 || pSrcItem->m_Common.nSub == estArrow);
		BOOL bSrcPackage = IsPackageSlot(Src.dwBox, Src.dwX);
		BOOL bDestPackage = IsPackageSlot(Dest.dwBox, Dest.dwX);

		if ( bSrcPackage || bDestPackage)
		{
			if (bSrcPackage && bDestPackage)
			{
				DWORD dwSrcBox = g_GetBoxIndexFromSlot(Src.dwX);
				DWORD dwDestBox = g_GetBoxIndexFromSlot(Dest.dwX);
				nResult = ExchangePackageSlot(dwSrcBox, dwDestBox);
			}
			else if (bSrcPackage)
			{
				assert(pSrcItem);

				if (!pDestItem)
				{
					DWORD dwBox = g_GetBoxIndexFromSlot(Src.dwX);
					nResult = DisablePackage(dwBox);
				}
				else
				{
					KCustomEquipInfo* pEquipInfo = NULL;
                    DWORD dwBox = g_GetBoxIndexFromSlot(Src.dwX);

					assert(pDestItem->m_Common.nGenre == igEquipment &&	pDestItem->m_Common.nSub == estPackage);
                    
                    pEquipInfo = (KCustomEquipInfo*)pDestItem->GetItemInfo();
			        KGLOG_PROCESS_ERROR(pEquipInfo);

					nResult = EnablePackage(
                        dwBox, pDestItem->m_nPackageSize, pEquipInfo->nPackageGenerType, pEquipInfo->nPackageSubType
                    );
				}

			}
			else if (bDestPackage)
			{
				assert(pSrcItem);
				assert(pSrcItem->m_Common.nGenre == igEquipment && pSrcItem->m_Common.nSub == estPackage);

				DWORD dwBox = g_GetBoxIndexFromSlot(Dest.dwX);
                KCustomEquipInfo* pEquipInfo = (KCustomEquipInfo*)pSrcItem->GetItemInfo();
                KGLOG_PROCESS_ERROR(pEquipInfo);

				nResult = EnablePackage(
                    dwBox, pSrcItem->m_nPackageSize, pEquipInfo->nPackageGenerType, pEquipInfo->nPackageSubType
                );
			}
			else
			{
				assert(false);
			}
			
			if (nResult == ircSuccess)
			{
				KItem* pTemp = NULL;

				if (Src.dwBox == ibEquip && Dest.dwBox != ibEquip)
				{
					UnEquip(Src.dwX);

					pTemp = pSrcBox->PickUpItem(Src.dwX);
					assert(pTemp == pSrcItem);

					if (pDestItem)
					{
						pTemp = pDestBox->PickUpItem(Dest.dwX);
						assert(pTemp == pDestItem);

						bRetCode = pSrcBox->PlaceItem(pDestItem, Src.dwX);
						assert(bRetCode);
						Equip(pDestItem, Src.dwX);
					}

					bRetCode = pDestBox->PlaceItem(pSrcItem, Dest.dwX);
					assert(bRetCode);
				}
				else if (Dest.dwBox == ibEquip && Src.dwBox != ibEquip)
				{
					if (pDestItem)
					{
						UnEquip(Dest.dwX);
						pTemp = pDestBox->PickUpItem(Dest.dwX);
						assert(pTemp == pDestItem);
					}

					pTemp = pSrcBox->PickUpItem(Src.dwX);
					assert(pTemp == pSrcItem);

					if (pDestItem)
					{
						bRetCode = pSrcBox->PlaceItem(pDestItem, Src.dwX);
						assert(bRetCode);
					}

					bRetCode = pDestBox->PlaceItem(pSrcItem, Dest.dwX);
					assert(bRetCode);

					Equip(pSrcItem, Dest.dwX);
				}
				else
				{
					pTemp = pSrcBox->PickUpItem(Src.dwX);
					assert(pTemp == pSrcItem);
					pTemp = pDestBox->PickUpItem(Dest.dwX);
					assert(pTemp == pDestItem);
					if (pDestItem)
					{
						bRetCode = pSrcBox->PlaceItem(pDestItem, Src.dwX);
						assert(bRetCode);
					}
					bRetCode = pDestBox->PlaceItem(pSrcItem, Dest.dwX);
					assert(bRetCode);
				}
			}
		}
		else if (pSrcBox->m_nBoxType == ivtEquipment && pDestBox->m_nBoxType == ivtEquipment)
		{
			// ��������װ���񣬲��ҿɽ������ã��Ǿ�ֱ�ӽ������ˡ�����Ӧ������
			KItem* pTemp;
			pTemp = pSrcBox->PickUpItem(Src.dwX);
			assert(pTemp == pSrcItem);
            
            if (pDestItem)
            {
                pTemp = pDestBox->PickUpItem(Dest.dwX);
			    assert(pTemp == pDestItem);
			    bRetCode = pSrcBox->PlaceItem(pDestItem, Src.dwX);
			    assert(bRetCode);
            }

			bRetCode = pDestBox->PlaceItem(pSrcItem, Dest.dwX);
			assert(bRetCode);

		}
        else if (pSrcBox->m_nBoxType == ivtEquipment)	// Դװ�����£�Ŀ��װ���еĻ���װ�ϡ�
        {
            if (pDestItem)
            {
                KItem* pTemp = NULL;

//				�Ƿ���װ��ǰ���Ѿ������ˡ�
//              nRetCode = CanEquip(pDestItem, Src.dwX);
//              KGLOG_PROCESS_ERROR(nRetCode == ircSuccess);
                
                bRetCode = UnEquip(Src.dwX);
                KGLOG_PROCESS_ERROR(bRetCode);
                pTemp = m_Box[Src.dwBox].PickUpItem(Src.dwX);
                assert(pTemp == pSrcItem);
                pTemp = m_Box[Dest.dwBox].PickUpItem(Dest.dwX);
                assert(pTemp == pDestItem);
                bRetCode = m_Box[Dest.dwBox].PlaceItem(pSrcItem, Dest.dwX);
                assert(bRetCode);
                bRetCode = m_Box[Src.dwBox].PlaceItem(pDestItem, Src.dwX);
                assert(bRetCode);
                bRetCode = Equip(pDestItem, Src.dwX);
                KGLOG_PROCESS_ERROR(bRetCode);
            }
            else
            {
                KItem* pTemp = NULL;
                
                bRetCode = UnEquip(Src.dwX);
                KGLOG_PROCESS_ERROR(bRetCode);
                pTemp = m_Box[Src.dwBox].PickUpItem(Src.dwX);
                assert(pTemp == pSrcItem);
                bRetCode = m_Box[Dest.dwBox].PlaceItem(pSrcItem, Dest.dwX);
                assert(bRetCode);
            }
        }
        else	// Դװ��װ�ϣ�Ŀ��װ���еĻ��ǲ��¡�
        {
            if (pDestItem)
            {
                KItem* pTemp = NULL;

//				�Ƿ���װ������CanPut�Ѿ������ˡ�
//              nRetCode = CanEquip(pSrcItem, Dest.dwX);
//              KGLOG_PROCESS_ERROR(nRetCode == ircSuccess);

                bRetCode = UnEquip(Dest.dwX);
                KGLOG_PROCESS_ERROR(bRetCode);
                pTemp = m_Box[Src.dwBox].PickUpItem(Src.dwX);
                assert(pTemp == pSrcItem);
                pTemp = m_Box[Dest.dwBox].PickUpItem(Dest.dwX);
                assert(pTemp == pDestItem);
                bRetCode = m_Box[Dest.dwBox].PlaceItem(pSrcItem, Dest.dwX);
                assert(bRetCode);
                bRetCode = m_Box[Src.dwBox].PlaceItem(pDestItem, Src.dwX);
                assert(bRetCode);
                bRetCode = Equip(pSrcItem, Dest.dwX);
                KGLOG_PROCESS_ERROR(bRetCode);
            }
            else
            {
                KItem* pTemp = NULL;

                pTemp = m_Box[Src.dwBox].PickUpItem(Src.dwX);
                assert(pTemp == pSrcItem);
                bRetCode = m_Box[Dest.dwBox].PlaceItem(pSrcItem, Dest.dwX);
                assert(bRetCode);
                bRetCode = Equip(pSrcItem, Dest.dwX);
                KGLOG_PROCESS_ERROR(bRetCode);
            }
        }
    }

#ifdef _SERVER
    g_PlayerServer.DoExchangeItem(
        m_pPlayer->m_nConnIndex, Src.dwBox, Src.dwX, Dest.dwBox, Dest.dwX, dwAmount
    );
#endif

Exit1:
    m_pPlayer->UpdateWeaponAttackSpeed();

    nResult = ircSuccess;
Exit0:
#ifdef _SERVER
    if (nResult != ircSuccess)
    {
        g_PlayerServer.DoMessageNotify(m_pPlayer->m_nConnIndex, ectItemErrorCode, (BYTE)nResult);
    }
#endif
    return nResult;
}

BOOL KItemList::GetEquipPos(int nRepresentIndex, DWORD *pdwEquipPos)
{
    BOOL bResult = false;

    switch (nRepresentIndex)
    {
    case perHelmStyle:
        *pdwEquipPos = eitHelm;
        break;
    
    case perChestStyle:
        *pdwEquipPos = eitChest;
        break;    

    case perWaistStyle:
        *pdwEquipPos = eitWaist;
        break;
    
    case perBangleStyle:
        *pdwEquipPos = eitBangle;
        break;

    case perBootsStyle:
        *pdwEquipPos = eitBoots;
        break;
    
    case perWeaponStyle:
        *pdwEquipPos = eitMeleeWeapon;
        break;
    
    case perBackExtend:
        *pdwEquipPos = eitBackExtend;
        break;

    case perWaistExtend:
        *pdwEquipPos = eitWaistExtend;
        break;
    
    case perHorseStyle:
        *pdwEquipPos = eitHorse;
        break;

	default:
        goto Exit0;
    }

    bResult = true;
Exit0:
    return bResult;
}

EQUIPMENT_INVENTORY_TYPE KItemList::GetEquipPosBySubType(EQUIPMENT_SUB_TYPE eSubType)
{
    EQUIPMENT_INVENTORY_TYPE eRetType = eitTotal;

    assert(eSubType != estPackage);
    assert(eSubType != estRing); // estPackage��estRing���͵�װ���ܷ��ڶ��λ�ã�Ҫ���⴦�����ܵ��������
    
    for (int i = 0; i < eitTotal; ++i)
    {
        if (g_CanEquipList[i] == eSubType)
        {
            eRetType = (EQUIPMENT_INVENTORY_TYPE)i;
            break;
        }
    }
    
    return eRetType;
}

ITEM_RESULT_CODE KItemList::GetCanEquipPos(KItem* pItem, int* pnRetPos)
{
    ITEM_RESULT_CODE         nResult    = ircInvalid;
    ITEM_RESULT_CODE         nRetCode   = ircInvalid;
    EQUIPMENT_INVENTORY_TYPE ePos       = eitTotal;
    KItem*                   pTempItem  = NULL;

    assert(pnRetPos);

    switch(pItem->m_Common.nSub)
    {
    case estPackage:
        for (int i = eitPackage1; i <= eitPackage4; ++i)
        {
            nRetCode = CanEquip(pItem, i);
            if (nRetCode == ircSuccess)
            {
                pTempItem = GetItem(igEquipment, i);
                if (pTempItem == NULL)
                {
                    ePos = (EQUIPMENT_INVENTORY_TYPE)i;
                    goto Exit1;
                }
                else if (ePos == eitTotal)
                {
                    ePos = (EQUIPMENT_INVENTORY_TYPE)i;
                }
            }
        }
        KG_PROCESS_ERROR_RET_CODE(ePos != eitTotal, nRetCode);

        break;
    case estRing:
        nRetCode = CanEquip(pItem, eitLeftRing);
        if (nRetCode == ircSuccess)
        {
            ePos = eitLeftRing;
            pTempItem = GetItem(igEquipment, eitLeftRing);
            KG_PROCESS_SUCCESS(pTempItem == NULL);
        }

        nRetCode = CanEquip(pItem, eitRightRing);
        if (nRetCode == ircSuccess)
        {
            ePos = eitRightRing;
            goto Exit1;
        }
        KG_PROCESS_ERROR_RET_CODE(ePos != eitTotal, nRetCode);

        break;
    default:
        ePos = GetEquipPosBySubType((EQUIPMENT_SUB_TYPE)pItem->m_Common.nSub);
        KGLOG_PROCESS_ERROR(ePos < eitTotal);

        nRetCode = CanEquip(pItem, ePos);
        KG_PROCESS_ERROR_RET_CODE(nRetCode == ircSuccess, nRetCode);
        
        break;
    }
    
Exit1:
    *pnRetPos = ePos;
    nResult = ircSuccess;
Exit0:
    return nResult;
}

ITEM_RESULT_CODE KItemList::CanEquip(KItem* pItem, DWORD dwX)
{
    ITEM_RESULT_CODE    nResult     = ircFailed;
    BOOL                bRetCode    = false;
    KAttribute*         pData       = NULL;
    KItemInfo*          pItemInfo   = NULL;

    assert(pItem);

    KG_PROCESS_ERROR_RET_CODE(pItem->m_Common.nGenre == igEquipment, ircCannotEquip);
    KGLOG_PROCESS_ERROR(dwX < m_Box[ibEquip].m_dwSize);

    KG_PROCESS_ERROR_RET_CODE(g_CanEquipList[dwX] == pItem->m_Common.nSub, ircErrorEquipPlace);

    if (pItem->m_Common.nSub != estMeleeWeapon && pItem->m_Common.nSub != estRangeWeapon && pItem->m_Common.nSub != estHorse && pItem->m_Common.nSub != estArrow)
        KG_PROCESS_ERROR_RET_CODE(!m_pPlayer->m_bFightState, ircPlayerInFight);

    pData = pItem->m_pRequireAttr;
    while (NULL != pData)
    {
        switch(pData->nKey)
        {
        case rqtFaction:
            // �����Ƿ���ͬ
            KGLOG_PROCESS_ERROR_RET_CODE(pData->nValue1 == 0 || m_pPlayer->m_dwForceID == (DWORD)pData->nValue1, ircForceError);
            break;
        case rqtStrength:
            // �����Ƿ���ڻ����װ��Ҫ��
            KGLOG_PROCESS_ERROR_RET_CODE(m_pPlayer->m_nCurrentStrength >= pData->nValue1, ircTooLowStrength);
            break;
        case rqtAgility:
            // �����Ƿ���ڻ����װ��Ҫ��
            KGLOG_PROCESS_ERROR_RET_CODE(m_pPlayer->m_nCurrentAgility >= pData->nValue1, ircTooLowAgility);
            break;
        case rqtSpirit:
            // �����Ƿ���ڻ����װ��Ҫ��
            KGLOG_PROCESS_ERROR_RET_CODE(m_pPlayer->m_nCurrentSpirit >= pData->nValue1, ircTooLowSpirit);
            break;
        case rqtVitality:
            // �����Ƿ���ڻ����װ��Ҫ��
            KGLOG_PROCESS_ERROR_RET_CODE(m_pPlayer->m_nCurrentVitality >= pData->nValue1, ircTooLowVitality);
            break;
        case rqtLevel:
            // �ȼ��Ƿ���ڻ����װ��Ҫ��
            KGLOG_PROCESS_ERROR_RET_CODE(m_pPlayer->m_nLevel >= pData->nValue1, ircTooLowLevel);
            break;
        case rqtGender:
            // �Ա��Ƿ����װ������
            KGLOG_PROCESS_ERROR_RET_CODE(
                pData->nValue1 == 0 || m_pPlayer->m_eGender == pData->nValue1, ircGenderError
            );
            break;
        default:
            break;
        }
        pData = pData->pNext;
    }

    pItemInfo = pItem->GetItemInfo();
    KGLOG_PROCESS_ERROR(pItemInfo);
    
    if (pItemInfo->dwRequireProfessionID != 0)
    {
        bRetCode = m_pPlayer->m_ProfessionList.IsProfessionLearned(pItemInfo->dwRequireProfessionID);
        KG_PROCESS_ERROR_RET_CODE(bRetCode, ircRequireProfession);

        if (pItemInfo->dwRequireProfessionBranch != 0)
        {
            DWORD dwBranchID = m_pPlayer->m_ProfessionList.GetBranchID(pItemInfo->dwRequireProfessionID);
            KG_PROCESS_ERROR_RET_CODE(dwBranchID == pItemInfo->dwRequireProfessionBranch, ircRequireProfessionBranch);
        }

        if (pItemInfo->nRequireProfessionLevel != 0)
        {
            DWORD dwProfessionLevel = m_pPlayer->m_ProfessionList.GetProfessionLevel(pItemInfo->dwRequireProfessionID);
            KG_PROCESS_ERROR_RET_CODE(dwProfessionLevel >= (DWORD)pItemInfo->nRequireProfessionLevel, ircProfessionLevelTooLow);
        }
    }

    KG_PROCESS_ERROR_RET_CODE(pItemInfo->nRequireCamp & (0x01 << m_pPlayer->m_eCamp), ircCampCannotEquip);

    nResult = ircSuccess;
Exit0:
    return nResult;
}

ITEM_RESULT_CODE KItemList::CanUnEquip(DWORD dwX)
{
    ITEM_RESULT_CODE    nResult  = ircFailed;
    KItem*              pItem    = NULL;
    
    assert(m_pPlayer);
    KGLOG_PROCESS_ERROR(dwX < m_Box[ibEquip].m_dwSize);

    pItem = GetItem(ibEquip, dwX);
    KGLOG_PROCESS_ERROR(pItem);
    KGLOG_PROCESS_ERROR(pItem->m_bEquiped);

    if (pItem->m_Common.nSub != estMeleeWeapon && pItem->m_Common.nSub != estRangeWeapon && pItem->m_Common.nSub != estHorse && pItem->m_Common.nSub != estArrow)
        KG_PROCESS_ERROR_RET_CODE(!m_pPlayer->m_bFightState, ircPlayerInFight);
    
    KG_PROCESS_ERROR_RET_CODE(!(dwX == eitHorse && m_pPlayer->m_bOnHorse), ircOnHorse); // �����ʱ����UnEquip��

    nResult = ircSuccess;
Exit0:
    return nResult;
}

BOOL KItemList::Equip(KItem* pItem, DWORD dwX)
{
    BOOL    bResult             = false;
    BOOL    bRetCode            = false;
    int     nRepresentIndex     = 0;
    WORD    wRepresentID        = 0;
    BOOL    bRepresentIsLock    = false;
    int     nCurrentDurability  = 0;

    assert(m_pPlayer);
    KGLOG_PROCESS_ERROR(dwX < m_Box[ibEquip].m_dwSize);
    KGLOG_PROCESS_ERROR(!(pItem->m_bApplyed));
	KGLOG_PROCESS_ERROR(!(pItem->m_bEquiped));
    assert(GetItem(ibEquip, dwX) == pItem);
    
    nCurrentDurability = pItem->GetCurrentDurability();

	if (
        nCurrentDurability != 0 && 
        (m_pPlayer->m_nUnarmCounter == 0 || dwX != eitMeleeWeapon) && 
        (dwX != eitHorse) // װ�����ʱ��Ӧ�����ԣ��������ʱ��Ӧ��
    )
	{
		ApplyAttrib(pItem);
	}

	pItem->m_bEquiped = true;

    if (!pItem->m_bBind && pItem->m_Common.nBindType == ibtBindOnEquipped)
    {
        pItem->m_bBind = true;
    }

#ifdef _SERVER
    if ((pItem->m_dwScriptID != 0 || pItem->m_dwSkillID != 0) && pItem->m_Common.nGenre == igEquipment)
    {
        bRetCode = pItem->ResetCoolDown(m_pPlayer);
        KGLOG_PROCESS_ERROR(bRetCode);
    }
#endif
    
    // ������� ------------- >
    bRetCode = GetEquipRepresentIndex(dwX, &nRepresentIndex);
    if (!bRetCode)
    {
        goto Exit1;
    }

    SetRepresentID(nRepresentIndex, pItem->m_nRepresentID);
    
    if (pItem->m_pPermanentEnchant)
    {
        SetRepresentID(pItem->m_pPermanentEnchant->nRepresentIndex, pItem->m_pPermanentEnchant->nRepresentID);
    }
    
    if (pItem->m_pTemporaryEnchant)
    {
        SetRepresentID(pItem->m_pTemporaryEnchant->nRepresentIndex, pItem->m_pTemporaryEnchant->nRepresentID);
    }
    
    bRetCode = GetEquipColorIndex(dwX, &nRepresentIndex);
    if (!bRetCode)
    {
        goto Exit1;
    }

    SetRepresentID(nRepresentIndex, pItem->m_nColorID);
    
    // <-------------- �������

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::UnEquip(DWORD dwX)
{
    BOOL     bResult            = false;
    BOOL     bRetCode           = false;
    int      nRepresentIndex    = 0;
    BOOL     bRepresentIsLock   = false;
    KItem*   pItem              = NULL;
    
    pItem = GetItem(ibEquip, dwX);
    KGLOG_PROCESS_ERROR(pItem);

    bRetCode = CanUnEquip(dwX);
    KGLOG_PROCESS_ERROR(bRetCode == ircSuccess);

	if (pItem->m_bApplyed)
	{
		UnApplyAttrib(pItem);
	}

	pItem->m_bEquiped = false;
    
    bRetCode = GetEquipRepresentIndex(dwX, &nRepresentIndex);
    if (!bRetCode)
    {
        goto Exit1;
    }

    SetRepresentID(nRepresentIndex, 0);
    
    if (pItem->m_pPermanentEnchant)
    {
        SetRepresentID(pItem->m_pPermanentEnchant->nRepresentIndex, 0);
    }

    if (pItem->m_pTemporaryEnchant)
    {
        SetRepresentID(pItem->m_pTemporaryEnchant->nRepresentIndex, 0);
    }

    bRetCode = GetEquipColorIndex(dwX, &nRepresentIndex);
    if (!bRetCode)
    {
        goto Exit1;
    }

    SetRepresentID(nRepresentIndex, 0);

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

ITEM_RESULT_CODE KItemList::DestroyItem(const TItemPos& Pos, BOOL bSync /*= true*/)
{
    ITEM_RESULT_CODE    nResult     = ircFailed;
    return nResult;
}

ITEM_RESULT_CODE KItemList::DestroyItem(KItem* pItem, BOOL bSync/* = true*/)
{
    ITEM_RESULT_CODE nResult = ircFailed;
    BOOL bRetCode = false;
    KGLOG_PROCESS_ERROR(pItem);

    for (int nBoxIndex = 0; nBoxIndex < ibTotal; nBoxIndex++)
    {
        for (DWORD dwX = 0; dwX < m_Box[nBoxIndex].m_dwSize; dwX++)
        {
            KItem* pTempItem = m_Box[nBoxIndex].GetItem(dwX);
            if (pItem == pTempItem)
            {
                TItemPos ItemPos = {nBoxIndex, dwX};
                nResult = DestroyItem(ItemPos, bSync);
                KGLOG_PROCESS_ERROR(nResult == ircSuccess);

                return ircSuccess;
            }
        }
    }

Exit0:
    return nResult;
}

DWORD KItemList::GetBoxSize(DWORD dwBox)
{
    DWORD	dwResult = 0;

    KGLOG_PROCESS_ERROR(dwBox < ibTotal);

    dwResult = m_Box[dwBox].m_dwSize;
Exit0:
    return dwResult;
}

BOOL KItemList::GetContainType(DWORD dwBox, int* pnContainItemGenerType, int* pnContainItemSubType)
{
	BOOL    bResult = false;

	KGLOG_PROCESS_ERROR(dwBox < ibTotal);
    KGLOG_PROCESS_ERROR(pnContainItemGenerType);
    KGLOG_PROCESS_ERROR(pnContainItemSubType);
	
    m_Box[dwBox].GetContainItemType(pnContainItemGenerType, pnContainItemSubType);

    bResult = true;
Exit0:
	return bResult;
}

DWORD KItemList::GetBoxUsedSize(DWORD dwBox)
{
    DWORD	dwResult = 0;

    KGLOG_PROCESS_ERROR(dwBox < ibTotal);

    dwResult = m_Box[dwBox].m_dwUsed;
Exit0:
    return dwResult;
}

int KItemList::GetMainHandType()
{
	int nResult = -1;

	KItem* pItem = GetItem(ibEquip, eitMeleeWeapon); 
	KG_PROCESS_ERROR(pItem);

	nResult = pItem->m_Common.nDetail;
Exit0:
	return nResult;
}

#ifdef _SERVER
BOOL KItemList::Load(BYTE* pbyData, size_t uDataLen)
{
    BOOL        bResult     = false;
    BOOL        bRetCode    = false;
    size_t      uLeftSize   = uDataLen;
    BYTE*       pbyOffset   = pbyData;
    int         nItemCount  = 0;
    KItem*      pItem       = NULL;
    
    KGLOG_PROCESS_ERROR(uLeftSize >= sizeof(DWORD));
    m_nMoney = *(int*)pbyOffset;
    uLeftSize -= sizeof(int);
    pbyOffset += sizeof(int);
    
    g_PlayerServer.DoSyncMoney(m_pPlayer->m_nConnIndex, m_nMoney, false);

    KGLOG_PROCESS_ERROR(uLeftSize >= sizeof(WORD));
    m_nEnabledBankPackageCount = *(WORD*)pbyOffset;
    uLeftSize -= sizeof(WORD);
    pbyOffset += sizeof(WORD);
    
    g_PlayerServer.DoSyncEnableBankPackage(m_pPlayer->m_nConnIndex, m_nEnabledBankPackageCount);

    KGLOG_PROCESS_ERROR(uLeftSize >= sizeof(WORD));
    nItemCount = *(WORD*)pbyOffset;
    uLeftSize -= sizeof(WORD);
    pbyOffset += sizeof(WORD);

    for (int nIndex = 0; nIndex < nItemCount; nIndex++)
    {
        KITEM_DB_HEADER*    pItemDataHeader     = NULL;
        KItemInfo*          pItemInfo           = NULL;
        DWORD               dwBoxIndex          = 0;
        DWORD               dwX                 = 0;
        time_t              nTotalLogoutTime    = 0;
        KENCHANT*           pEnchant            = NULL;

        KGLOG_PROCESS_ERROR(uLeftSize >= sizeof(KITEM_DB_HEADER));
        pItemDataHeader = (KITEM_DB_HEADER*)pbyOffset;
        uLeftSize -= sizeof(KITEM_DB_HEADER);
        pbyOffset += sizeof(KITEM_DB_HEADER);

        dwBoxIndex          = pItemDataHeader->byBox;
        dwX                 = pItemDataHeader->byPos;

        KGLOG_PROCESS_ERROR(dwBoxIndex < ibTotal);
        
        if (pItem)
        {
            g_pSO3World->m_ItemManager.FreeItem(pItem);
            pItem = NULL;
        }
        
        KGLOG_PROCESS_ERROR(uLeftSize >= pItemDataHeader->byDataLen);
        uLeftSize -= pItemDataHeader->byDataLen;
        pbyOffset += pItemDataHeader->byDataLen;

        pItem = g_pSO3World->m_ItemManager.GenerateItemFromBinaryData(
            ERROR_ID, pItemDataHeader->byData, pItemDataHeader->byDataLen
        );
        if (pItem == NULL)
        {
            KGLogPrintf(KGLOG_ERR, "[ITEM] GenerateItemFromBinaryData failed, Acc: %s, Role: %s<%u>. \n", m_pPlayer->m_szAccount, m_pPlayer->m_szName, m_pPlayer->m_dwID);
            continue;
        }
        
        nTotalLogoutTime = (g_pSO3World->m_nCurrentTime - m_pPlayer->m_nLastSaveTime) / 60;

        pItemInfo = g_pSO3World->m_ItemManager.GetItemInfo(
            pItem->m_GenParam.nVersion, pItem->m_GenParam.dwTabType, pItem->m_GenParam.dwIndex
        );
        KGLOG_PROCESS_ERROR(pItemInfo);

        if (pItemInfo->nMaxExistTime && nTotalLogoutTime > pItemInfo->nMaxExistTime)
        {
            continue;
        }

        bRetCode = m_Box[dwBoxIndex].PlaceItem(pItem, dwX);
        KGLOG_PROCESS_ERROR(bRetCode);

        if (dwBoxIndex == ibEquip)
        {
            DWORD dwSlotBox = 0;

            bRetCode = Equip(pItem, dwX);
            if (!bRetCode)
            {
                m_Box[dwBoxIndex].PickUpItem(dwX);
                goto Exit0;
            }

            dwSlotBox = g_GetBoxIndexFromSlot(dwX);
            if (dwSlotBox)
            {
                KCustomEquipInfo* pItemInfo = NULL;
                
                KGLOG_PROCESS_ERROR(pItem->m_Common.nGenre == igEquipment && pItem->m_Common.nSub == estPackage);
                
                pItemInfo = (KCustomEquipInfo*)pItem->GetItemInfo();
                KGLOG_PROCESS_ERROR(pItemInfo);

                EnablePackage(dwSlotBox, pItem->m_nPackageSize, pItemInfo->nPackageGenerType, pItemInfo->nPackageSubType);
            }
        }

        g_PlayerServer.DoSyncItemData(m_pPlayer->m_nConnIndex, m_pPlayer->m_dwID, pItem, dwBoxIndex, dwX);
        pItem = NULL;
    }

    CheckEquipRequire();

    KGLOG_PROCESS_ERROR(uLeftSize == 0);

    bResult = true;
Exit0:
    if (!bResult)
        KGLogPrintf(KGLOG_ERR, "[ITEM] Load item failed, Acc: %s, Role: %s<%u>. \n", m_pPlayer->m_szAccount, m_pPlayer->m_szName, m_pPlayer->m_dwID);

    if (pItem)
    {
        g_pSO3World->m_ItemManager.FreeItem(pItem);
        pItem = NULL;
    }
    return bResult;
}

BOOL KItemList::Save(size_t* puUsedSize, BYTE* pbyBuffer, size_t uBufferSize)
{
    BOOL    bResult	                    = false;
    BOOL    bRetCode                    = false;
    BYTE*   pbyOffset                   = pbyBuffer;
    size_t  uLeftSize                   = uBufferSize;
    WORD*   pwItemCount                 = NULL;
    int     nItemCount                  = 0;

    KGLOG_PROCESS_ERROR(uLeftSize >= sizeof(int));
    *(int*)pbyOffset = m_nMoney;
    uLeftSize -= sizeof(int);
    pbyOffset += sizeof(int);

    KGLOG_PROCESS_ERROR(uLeftSize >= sizeof(WORD));
    *(WORD*)pbyOffset = (WORD)m_nEnabledBankPackageCount;
    uLeftSize -= sizeof(WORD);
    pbyOffset += sizeof(WORD);

    KGLOG_PROCESS_ERROR(uLeftSize >= sizeof(WORD));
    pwItemCount = (WORD*)pbyOffset;
    uLeftSize -= sizeof(WORD);
    pbyOffset += sizeof(WORD);

    for (int nBoxIndex = 0; nBoxIndex < ibTotal; nBoxIndex++)
    {
        if (nBoxIndex == ibSoldList)
        {
            continue;
        }

        for (int nX = 0; nX < (int)m_Box[nBoxIndex].m_dwSize; nX++)
        {
            KItem*              pItem               = m_Box[nBoxIndex].GetItem(nX);
            KITEM_DB_HEADER*    pItemDataHeader     = NULL;
            size_t              uItemDataSize       = 0;

            if (pItem == NULL)
            {
                continue;
            }

            KGLOG_PROCESS_ERROR(uLeftSize >= sizeof(KITEM_DB_HEADER));
            pItemDataHeader = (KITEM_DB_HEADER*)pbyOffset;
            uLeftSize -= sizeof(KITEM_DB_HEADER);
            pbyOffset += sizeof(KITEM_DB_HEADER);

            bRetCode = pItem->GetBinaryData(&uItemDataSize, pItemDataHeader->byData, uLeftSize);
            KGLOG_PROCESS_ERROR(bRetCode);

            assert(uItemDataSize < UCHAR_MAX);

            pItemDataHeader->byBox      = (BYTE)nBoxIndex;
            pItemDataHeader->byPos      = (BYTE)nX;
            pItemDataHeader->byDataLen  = (BYTE)uItemDataSize;

            uLeftSize -= uItemDataSize;
            pbyOffset += uItemDataSize;

            nItemCount++;
        }
    }

    *pwItemCount = (WORD)nItemCount;

    *puUsedSize = uBufferSize - uLeftSize;

    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::TidyUpSoldList()
{
    int         nRetCode        = 0;
    BOOL        bResult         = false;
    KItem*      pItem           = NULL;
    int         nFreePos     = -1;
    int         nPosIndex       = 0;
    int         nSoldListHead   = 0; 

    nSoldListHead = m_Box[ibSoldList].GetSoldListHead();
    nPosIndex = nSoldListHead;
    while (true)
    {
        pItem = m_Box[ibSoldList].GetItem(nPosIndex);
        if (pItem == NULL)
        {
            nFreePos = nPosIndex;
            break;    
        }

        ++nPosIndex;
        nPosIndex = nPosIndex >= MAX_SOLDLIST_PACKAGE_SIZE ? 0 : nPosIndex;

        if (nPosIndex == nSoldListHead)
        {
            break;
        }
    }

    if (nFreePos != -1)
    {   
        nPosIndex = nFreePos + 1;
        while (true)
        {
            nPosIndex = nPosIndex >= MAX_SOLDLIST_PACKAGE_SIZE ? 0 : nPosIndex;
            if (nPosIndex == nSoldListHead)
            {
                break;
            }
            pItem = m_Box[ibSoldList].GetItem(nPosIndex);
            if (pItem)
            {
                TItemPos Src, Dest;
                Src.dwBox = ibSoldList;
                Src.dwX = nPosIndex;
                Dest.dwBox = ibSoldList;
                Dest.dwX = nFreePos;
                nRetCode = ExchangeItem(Src, Dest);
                KG_PROCESS_ERROR(nRetCode == ircSuccess);

                ++nFreePos;
                nFreePos = (nFreePos >= MAX_SOLDLIST_PACKAGE_SIZE) ? 0 : nFreePos;
            }
            ++nPosIndex;
        }
    }

    bResult = true;
Exit0:
    return bResult;
}

void KItemList::AbradeArmor()
{
    AbradeItemAtEquipBox(eitChest);
    AbradeItemAtEquipBox(eitHelm);
    AbradeItemAtEquipBox(eitWaist);
    AbradeItemAtEquipBox(eitPants);
    AbradeItemAtEquipBox(eitBoots);
    AbradeItemAtEquipBox(eitBangle);
}

void KItemList::AbradeItemAtEquipBox(DWORD dwPos)
{
	BOOL    bRetCode            = false;
    KItem*  pItem               = NULL;
    int     nCurrentDurability  = 0;

    KGLOG_PROCESS_ERROR(dwPos < m_Box[ibEquip].m_dwSize);

    pItem = m_Box[ibEquip].m_pItemArray[dwPos];
    KG_PROCESS_ERROR(pItem);

    bRetCode = pItem->Abrade();
    KG_PROCESS_ERROR(bRetCode);
    
    nCurrentDurability = pItem->GetCurrentDurability();

#ifdef _SERVER
	g_PlayerServer.DoUpdateItemAmount(
        m_pPlayer->m_nConnIndex, ibEquip, dwPos, nCurrentDurability
	);
#endif

    if (nCurrentDurability == 0 && pItem->m_bApplyed)
	{
		UnApplyAttrib(pItem);
	}

Exit0:
    return;
}

BOOL KItemList::AbradeEquipment(int nBoxType, int nAbradePercent)
{
    BOOL bResult = false;
    BOOL bRetCode = false;

    KGLOG_PROCESS_ERROR(nBoxType == ivtEquipment || nBoxType == ivtPackage);
    KGLOG_PROCESS_ERROR(nAbradePercent >= 0 && nAbradePercent <= 100);

    for (int nBoxIndex = ibEquip; nBoxIndex <= ibPackage4; ++nBoxIndex)
    {
        if (m_Box[nBoxIndex].m_nBoxType != nBoxType)
        {
            continue;
        }

        for (DWORD dwX = 0; dwX < m_Box[nBoxIndex].m_dwSize; ++dwX)
        {
            int     nCurrentDurability  = 0;
            int     nMaxDurability      = 0;
            int     nAbradeDurability   = 0;
            KItem*  pItem               = m_Box[nBoxIndex].GetItem(dwX);

            if (pItem == NULL)
                continue;

            if (pItem->GetGenre() != igEquipment)
                continue;
            
            nCurrentDurability  = pItem->GetCurrentDurability();
            nMaxDurability      = pItem->GetMaxDurability();

            if (nCurrentDurability <= 0 || nMaxDurability <= 0)
            {
                continue;
            }

            nAbradeDurability   = max(1, nMaxDurability * nAbradePercent / 100);
            nCurrentDurability  = nCurrentDurability - nAbradeDurability;

            MAKE_IN_RANGE(nCurrentDurability, 0, nMaxDurability);
            bRetCode = pItem->SetCurrentDurability(nCurrentDurability);
            KGLOG_PROCESS_ERROR(bRetCode);
            if (pItem->m_nCurrentDurability == 0 && pItem->m_bApplyed)
			{
				UnApplyAttrib(pItem);
			}
#ifdef _SERVER
            assert(m_pPlayer);
            g_PlayerServer.DoUpdateItemAmount(m_pPlayer->m_nConnIndex, nBoxIndex, dwX, nCurrentDurability);
#endif
        }
    }

    bResult = true;
Exit0:
    return bResult;
}
#endif

BOOL KItemList::ApplyAttrib(KItem* pItem)
{
	BOOL bResult    = false;
	BOOL bRetCode   = false;

	assert(pItem);
	
	m_pPlayer->ApplyAttribute(pItem->m_pBaseAttr);

	m_pPlayer->ApplyAttribute(pItem->m_pMagicAttr);
    
    if (pItem->m_pPermanentEnchant)
    {
        m_pPlayer->ApplyAttribute(pItem->m_pPermanentEnchant->pAttribute);
    }
	
    if (pItem->m_pTemporaryEnchant)
    {
        m_pPlayer->ApplyAttribute(pItem->m_pTemporaryEnchant->pAttribute);
    }
	
	if (pItem->m_Common.dwSetID)
	{
        // +1 ����ΪGetSetNum����������
		// ���ü�鷵��ֵ���п�������û��װ����Ӧ�õġ�
		ApplySetAttrib(pItem->m_Common.dwSetID, GetEquipedSetNum(pItem->m_Common.dwSetID) + 1);
	}

	pItem->m_bApplyed = true;

	bResult = true;
Exit0:
	return bResult;
}

BOOL KItemList::UnApplyAttrib(KItem* pItem)
{
	BOOL bResult    = false;
	BOOL bRetCode   = false;

    assert(pItem->m_bApplyed);
	
    if (pItem->m_pTemporaryEnchant)
    {
        m_pPlayer->UnApplyAttribute(pItem->m_pTemporaryEnchant->pAttribute);
    }
	
    if (pItem->m_pPermanentEnchant)
    {
        m_pPlayer->UnApplyAttribute(pItem->m_pPermanentEnchant->pAttribute);
    }
	
	m_pPlayer->UnApplyAttribute(pItem->m_pMagicAttr);

	m_pPlayer->UnApplyAttribute(pItem->m_pBaseAttr);

	if (pItem->m_Common.dwSetID)
	{
		// ����Ͳ� +1 ��,GetSetNum���������
        // ���ü�鷵��ֵ���п�������û��װ����Ӧ�õġ�
		UnApplySetAttrib(pItem->m_Common.dwSetID, GetEquipedSetNum(pItem->m_Common.dwSetID));
	}

	pItem->m_bApplyed = false;

	bResult = true;
Exit0:
	return bResult;

}

BOOL KItemList::IsPackageSlot(DWORD dwBox, DWORD dwX)
{
	BOOL bResult = false;

	KG_PROCESS_ERROR(dwBox == ibEquip);
	KG_PROCESS_ERROR(
        dwX == eitPackage1 || dwX == eitPackage2 ||	dwX == eitPackage3 || dwX == eitPackage4 ||
		dwX == eitBankPackage1 || dwX == eitBankPackage2 || dwX == eitBankPackage3 || 
        dwX == eitBankPackage4 || dwX == eitBankPackage5
    );

	bResult = true;
Exit0:
	return bResult;
}

#ifdef _SERVER
BOOL KItemList::Repair(DWORD dwBox, DWORD dwX)
{
	BOOL    bResult             = false;
	BOOL    bRetCode            = false;
	KItem*  pItem               = NULL;
    int     nCurrentDurability  = 0;

	KGLOG_PROCESS_ERROR(dwBox < ibTotal);
	KGLOG_PROCESS_ERROR(dwX < m_Box[dwBox].m_dwSize);

	pItem = m_Box[dwBox].GetItem(dwX);
	KGLOG_PROCESS_ERROR(pItem);

    bRetCode = pItem->Repair();
    KGLOG_PROCESS_ERROR(bRetCode);
    
    nCurrentDurability = pItem->GetCurrentDurability();
    KGLOG_PROCESS_ERROR(nCurrentDurability > 0);

    g_PlayerServer.DoUpdateItemAmount(m_pPlayer->m_nConnIndex, dwBox, dwX, nCurrentDurability);

	if (pItem->m_bEquiped && !pItem->m_bApplyed)	// װ���Ų���ûӦ�ù�����
	{
		ApplyAttrib(pItem);
	}

	bResult = true;
Exit0:
	return bResult;
}

BOOL KItemList::EnableBankPackage()
{
	BOOL    bResult     = false;
	int     nCostMoney  = 0;

	KG_PROCESS_ERROR(m_nEnabledBankPackageCount < MAX_BANK_PACKAGE_COUNT);

	nCostMoney = g_pSO3World->m_Settings.m_ConstList.nBankPackagePrice[m_nEnabledBankPackageCount];
	KG_PROCESS_ERROR(m_nMoney >= nCostMoney); // ����ܸ�����Ǯ

	bResult = SetMoney(m_nMoney - nCostMoney);
	KGLOG_PROCESS_ERROR(bResult);

	++m_nEnabledBankPackageCount;

	g_PlayerServer.DoSyncEnableBankPackage(m_pPlayer->m_nConnIndex, m_nEnabledBankPackageCount);

	bResult = true;
Exit0:
	return bResult;
}
#endif

int KItemList::GetBankPackageCount()
{
	return m_nEnabledBankPackageCount;
}

#ifdef _CLIENT
BOOL KItemList::SetEnablePackageCount(int nCount)
{
    BOOL bResult = false;
    
	KGLOG_PROCESS_ERROR(nCount >= 0 && nCount <= MAX_BANK_PACKAGE_COUNT);

	m_nEnabledBankPackageCount = nCount;

	bResult = true;
Exit0:
	return bResult;
}
#endif

BOOL KItemList::IsBankPackageEnabled(EQUIPMENT_INVENTORY_TYPE eInventoryType)
{
	BOOL    bResult         = false;
	int     nPackageIndex   = 0;

	KGLOG_PROCESS_ERROR(eInventoryType >= eitBankPackage1 && eInventoryType <= eitBankPackage5);

	nPackageIndex = (eInventoryType - eitBankPackage1 + 1);
	KG_PROCESS_ERROR(nPackageIndex <= m_nEnabledBankPackageCount);
	
	bResult = true;
Exit0:
	return bResult;
}

#ifdef _SERVER
BOOL KItemList::ConsumeAmmo(DWORD dwAmount)
{
	BOOL	bResult     = false;
	BOOL	bRetCode    = false;
	DWORD	dwTabType   = 0;
	DWORD	dwIndex     = 0;
	KItem*  pItem       = NULL;

    pItem = m_Box[ibEquip].GetItem(eitArrow);
	KG_PROCESS_ERROR(pItem);

	dwTabType   = pItem->m_GenParam.dwTabType;
	dwIndex     = pItem->m_GenParam.dwIndex;

	bRetCode = CostSingleItem(ibEquip, eitArrow, dwAmount);
	KGLOG_PROCESS_ERROR(bRetCode);

	pItem = m_Box[ibEquip].GetItem(eitArrow);
	if (pItem == NULL)
	{
		ReloadAmmo(dwTabType, dwIndex);
	}

	bResult = true;
Exit0:
	return bResult;
}

void KItemList::ReloadAmmo(DWORD dwTabType, DWORD dwIndex)
{
	ITEM_RESULT_CODE	eRetCode    = ircFailed;
	DWORD	            dwBox       = 0;
	DWORD	            dwX         = 0;
	KItem*              pItem       = NULL;
    TItemPos            ItemPos     = {0, 0};
	TItemPos            EquipPos    = {ibEquip, eitArrow};

    pItem = FindItem(dwTabType, dwIndex, dwBox, dwX);
    KG_PROCESS_ERROR(pItem);

	eRetCode = ExchangeItem(ItemPos, EquipPos);
	KGLOG_PROCESS_ERROR(eRetCode == ircSuccess);
	
Exit0:
	return;
}
#endif


BOOL KItemList::AddEnchant(DWORD dwBoxIndex, DWORD dwX, int nEnchantID)
{
    BOOL         bResult        = false;
    BOOL         bRetCode       = false;
    KItem*       pItem          = NULL;
    KENCHANT*    pOldEnchant    = NULL;
    KENCHANT*    pNewEnchant    = NULL;

	pItem = GetItem(dwBoxIndex, dwX);
	KGLOG_PROCESS_ERROR(pItem && pItem->m_Common.nGenre == igEquipment);

    pNewEnchant = g_pSO3World->m_ItemManager.m_EnchantLib.GetEnchantInfo(nEnchantID);
	KGLOG_PROCESS_ERROR(pNewEnchant);
    
	KGLOG_PROCESS_ERROR(pNewEnchant->nDestItemSubType == pItem->m_Common.nSub);

    if (pNewEnchant->nTime <= 0)
    {
        pOldEnchant = pItem->m_pPermanentEnchant;

        pItem->m_pPermanentEnchant              = pNewEnchant;
        pItem->m_GenParam.dwPermanentEnchantID  = nEnchantID;
    }
    else
    {
        pOldEnchant = pItem->m_pTemporaryEnchant;

        pItem->m_pTemporaryEnchant              = pNewEnchant;
        pItem->m_GenParam.dwTemporaryEnchantID  = nEnchantID;
        pItem->m_nEnchantTime                   = g_pSO3World->m_nCurrentTime + pNewEnchant->nTime;
    }

    if (pOldEnchant)
    {
        if (pItem->m_bApplyed)
        {
            m_pPlayer->UnApplyAttribute(pOldEnchant->pAttribute);
        }

        if (pItem->m_bEquiped)
        {
            DWORD dwEquipPos        = 0;
            int   nOldRepresentID   = 0;

            bRetCode = GetEquipPos(pOldEnchant->nRepresentIndex, &dwEquipPos);
            if (bRetCode)
            {
                KItem* pTempItem = GetItem(ibEquip, dwEquipPos);

                if (pTempItem)
                {
                    nOldRepresentID = pTempItem->m_nRepresentID;
                }
            }

            SetRepresentID(pOldEnchant->nRepresentIndex, nOldRepresentID);
        }
    }

    if (pItem->m_bApplyed)
    {
        m_pPlayer->ApplyAttribute(pNewEnchant->pAttribute);
    }

    if (pItem->m_bEquiped)
    {
        SetRepresentID(pNewEnchant->nRepresentIndex, pNewEnchant->nRepresentID);
    }

#ifdef _SERVER
	g_PlayerServer.DoSyncItemData(m_pPlayer->m_nConnIndex, m_pPlayer->m_dwID, pItem, dwBoxIndex, dwX);
#endif

    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::RemoveEnchant(DWORD dwBoxIndex, DWORD dwX, BOOL bPermanentEnchant)
{
    BOOL        bResult         = false;
	BOOL        bRetCode        = false;
	KItem*      pItem           = NULL;
    KENCHANT*   pCurrentEnchant = NULL;

	pItem = GetItem(dwBoxIndex, dwX);
	KGLOG_PROCESS_ERROR(pItem && pItem->m_Common.nGenre == igEquipment);

    if (bPermanentEnchant)
    {
        pCurrentEnchant = pItem->m_pPermanentEnchant;

        pItem->m_pPermanentEnchant              = NULL;
        pItem->m_GenParam.dwPermanentEnchantID  = 0;
    }
    else
    {
        pCurrentEnchant = pItem->m_pTemporaryEnchant;

        pItem->m_pTemporaryEnchant              = NULL;
        pItem->m_GenParam.dwTemporaryEnchantID  = 0;
        pItem->m_nEnchantTime                   = 0;
    }

    KG_PROCESS_ERROR(pCurrentEnchant);

	if (pItem->m_bApplyed)
	{
        m_pPlayer->UnApplyAttribute(pCurrentEnchant->pAttribute);
    }

    if (pItem->m_bEquiped)
    {
        DWORD dwEquipPos        = 0;
        int   nOldRepresentID   = 0;

        bRetCode = GetEquipPos(pCurrentEnchant->nRepresentIndex, &dwEquipPos);
        if (bRetCode)
        {
            KItem* pTempItem = GetItem(ibEquip, dwEquipPos);

            if (pTempItem)
            {
                nOldRepresentID = pTempItem->m_nRepresentID;
            }
        }
        SetRepresentID(pCurrentEnchant->nRepresentIndex, nOldRepresentID);
    }

#ifdef _SERVER
	g_PlayerServer.DoSyncItemData(m_pPlayer->m_nConnIndex, m_pPlayer->m_dwID, pItem, dwBoxIndex, dwX);
#endif

	bResult = true;
Exit0:
	return bResult;
}

BOOL KItemList::SetEquipColor(DWORD dwBoxIndex, DWORD dwPos, int nColorID)
{
    BOOL        bResult             = false;
    BOOL        bRetCode            = false;
    KItem*      pItem               = NULL;
    KItemInfo*  pItemInfo           = NULL;
    int         nRepresentIndex     = 0;

	pItem = GetItem(dwBoxIndex, dwPos);
	KGLOG_PROCESS_ERROR(pItem);
    
    KGLOG_PROCESS_ERROR(pItem->m_Common.nGenre == igEquipment);

    pItemInfo = pItem->GetItemInfo();
    KGLOG_PROCESS_ERROR(pItemInfo);

    switch (pItem->m_GenParam.dwTabType)
    {
    case ittCustWeapon:
    case ittCustArmor:
    case ittCustTrinket:
        KGLOG_PROCESS_ERROR(((KCustomEquipInfo*)pItemInfo)->bCanSetColor);
        break;

    default:
        KGLogPrintf(KGLOG_DEBUG, "Invalid TabType %d\n", pItem->m_GenParam.dwTabType);
        goto Exit0;
        break;
    }

    pItem->m_nColorID = nColorID;

    if (pItem->m_bEquiped)
    {
        bRetCode = GetEquipColorIndex(dwPos, &nRepresentIndex);
        if (bRetCode)
        {
            SetRepresentID(nRepresentIndex, nColorID);
        }
    }

#ifdef _SERVER
	g_PlayerServer.DoSyncItemData(m_pPlayer->m_nConnIndex, m_pPlayer->m_dwID, pItem, dwBoxIndex, dwPos);
#endif

    bResult = true;
Exit0:
    return bResult;
}

KInventory* KItemList::GetAllInventory()
{
    return m_Box;
}

void KItemList::OnChangeCamp()
{
    for (DWORD dwX = 0; dwX < m_Box[ibEquip].m_dwSize; ++dwX)
    {
        KItem*      pItem       = m_Box[ibEquip].GetItem(dwX);
        KItemInfo*  pItemInfo   = NULL;

        if (pItem == NULL)
            continue;

        if (pItem->GetGenre() != igEquipment)
            continue;
        
        if (dwX == eitHorse)
        {
            m_pPlayer->DownHorse();
            continue;
        }

        pItemInfo = pItem->GetItemInfo();
        KGLOG_PROCESS_ERROR(pItemInfo);
        
        // ����װ����Ӧ��������
        if (!(pItemInfo->nRequireCamp & (0x01 << m_pPlayer->m_eCamp)) && pItem->m_bApplyed)
        {
            UnApplyAttrib(pItem);
        }
    }

Exit0:
    return;
}

KItem* KItemList::FindItem(DWORD dwTabType, DWORD dwIndex, DWORD& dwBox, DWORD& dwX)
{
	KItem* pItem = NULL;

	for (int i = ibPackage; i <= ibPackage4; i++)
	{
		for (DWORD j = 0; j < m_Box[i].m_dwSize; j++)
		{
			pItem = m_Box[i].GetItem(j);
			if (pItem && pItem->m_GenParam.dwTabType == dwTabType && pItem->m_GenParam.dwIndex == dwIndex)
			{
				dwBox = i;
				dwX = j;
				return pItem;
			}
		}
	}
	return pItem;
}
#ifdef _SERVER
void KItemList::CallItemScript(KItem* pItem, const TItemPos& crItemPos, KTarget& rTarget, BOOL& rbContinueSkill, BOOL& rbDestroyItem)
{
    BOOL        bRetCode    = false;
    int         nTopIndex   = 0;
    TARGET_TYPE eType;

    assert(pItem);

    KG_PROCESS_ERROR(pItem->m_dwScriptID != 0);

    bRetCode = g_pSO3World->m_ScriptCenter.IsScriptExist(pItem->m_dwScriptID);
    KG_PROCESS_ERROR(bRetCode);
    
    bRetCode = g_pSO3World->m_ScriptCenter.IsFuncExist(pItem->m_dwScriptID, SCRIPT_ON_USE_ITEM);
    KG_PROCESS_ERROR(bRetCode);
        
    g_pSO3World->m_ScriptCenter.SafeCallBegin(&nTopIndex);

	g_pSO3World->m_ScriptCenter.PushValueToStack(m_pPlayer);
	g_pSO3World->m_ScriptCenter.PushValueToStack(pItem);
	g_pSO3World->m_ScriptCenter.PushValueToStack(crItemPos.dwBox);
	g_pSO3World->m_ScriptCenter.PushValueToStack(crItemPos.dwX);
    
    eType = rTarget.GetTargetType();
    
    if (eType == ttItemPos)
    {   
        KPlayer*    pTargetPlayer   = NULL;
		DWORD       dwTargetBox     = 0;
		DWORD       dwTargetX       = 0;

		bRetCode = rTarget.GetTarget(&pTargetPlayer, &dwTargetBox, &dwTargetX);
		KG_PROCESS_ERROR(bRetCode);

		g_pSO3World->m_ScriptCenter.PushValueToStack(dwTargetBox);
		g_pSO3World->m_ScriptCenter.PushValueToStack(dwTargetX);
	}

    g_pSO3World->m_ScriptCenter.CallFunction(pItem->m_dwScriptID, SCRIPT_ON_USE_ITEM, 2);
	g_pSO3World->m_ScriptCenter.GetValuesFromStack("bb", &rbContinueSkill, &rbDestroyItem);
    g_pSO3World->m_ScriptCenter.SafeCallEnd(nTopIndex);
    
Exit0:
    return;
}

USE_ITEM_RESULT_CODE KItemList::CastItemSkill(KItem* pItem, KTarget& rTarget, BOOL* pbCostItemAtOnce)
{
    USE_ITEM_RESULT_CODE    nResult         = uircFailed;
    KSkill*                 pSkill          = NULL;
    SKILL_RESULT_CODE       nSkillResult    = srcInvalid;
    
    assert(pItem);
    assert(pbCostItemAtOnce);

    KG_PROCESS_SUCCESS(pItem->m_dwSkillID == INVALID_SKILL_ID);

    pSkill = g_pSO3World->m_SkillManager.GetSkill_RAW(pItem->m_dwSkillID, pItem->m_dwSkillLevel);
    KGLOG_PROCESS_ERROR(pSkill);

    if (m_pPlayer->m_OTActionParam.eActionType == otActionSkillChannel)
    {
        g_PlayerServer.DoSkillBeatBack(m_pPlayer, 0, true);
        m_pPlayer->DoActionIdle();
    }

    KG_PROCESS_ERROR_RET_CODE(m_pPlayer->m_OTActionParam.eActionType == otActionIdle, uircCannotCastSkill);

    if (pSkill->m_nPrepareFrames > 0)
    {
        KOT_ACTION_PARAM Param;

        nSkillResult = pSkill->CanCast(m_pPlayer, rTarget);
        KG_PROCESS_ERROR_RET_CODE(nSkillResult == srcSuccess, uircCastSkillFailed);

        Param.eActionType       = otActionItemSkill;
        Param.Data.dwItemID     = pItem->m_dwID;
        Param.nEndFrame         = g_pSO3World->m_nGameLoop + pSkill->m_nPrepareFrames;
        Param.nNextActFrame     = 0;
        Param.nBeatBackCount    = 0;

        m_pPlayer->m_SkillTarget = rTarget;
        m_pPlayer->DoOTAction(Param);

        g_PlayerServer.DoSkillPrepare(
            m_pPlayer, pItem->m_dwSkillID, pItem->m_dwSkillLevel,
            pSkill->m_nPrepareFrames, rTarget
        );

        *pbCostItemAtOnce = false;
    }
    else
    {
        nSkillResult = m_pPlayer->CastSkill(pItem->m_dwSkillID, pItem->m_dwSkillLevel, rTarget);
    }

    KG_PROCESS_ERROR_RET_CODE(nSkillResult == srcSuccess, uircCastSkillFailed);
    
    *pbCostItemAtOnce = true;

Exit1:
    nResult = uircSuccess;
Exit0:
    return nResult;
}
#endif

void KItemList::SetRepresentID(int nRepresentIndex, int nRepresentID)
{
    KG_PROCESS_ERROR(nRepresentIndex >= perFaceStyle && nRepresentIndex < perRepresentCount);
    
    KG_PROCESS_ERROR(!(m_pPlayer->m_dwRepresentIdLock & (0x1 << nRepresentIndex)));
    
    if (nRepresentIndex == perHelmStyle)
    {
        KG_PROCESS_ERROR(!m_pPlayer->m_bHideHat);
    }

#ifdef _SERVER
    if (m_pPlayer->m_wRepresentId[nRepresentIndex] != (WORD)nRepresentID)
    {
        g_PlayerServer.DoSyncEquipRepresent(m_pPlayer, nRepresentIndex, nRepresentID);
    }
#endif

    m_pPlayer->m_wRepresentId[nRepresentIndex] = (WORD)nRepresentID;
    
Exit0:
	return;
}

BOOL KItemList::CheckEquipRequire(void)
{
    BOOL bResult  = false;
    BOOL bRetCode = false;

    for (DWORD dwX = eitMeleeWeapon; dwX < eitTotal; dwX++)
    {
        KItem* pItem = m_Box[ibEquip].GetItem(dwX);
        if (pItem == NULL)
        {
            continue;
        }

        bRetCode = CanEquip(pItem, dwX);
        if (bRetCode != ircSuccess)
        {
            if (pItem->m_bApplyed)
            {
                UnApplyAttrib(pItem);
            }
        }
    }

    bResult = true;
//Exit0:
    return bResult;
}

BOOL KItemList::GetEquipRepresentIndex(DWORD dwEquipPos, int* pnIndex)
{
    BOOL bResult = false;

    switch (dwEquipPos)
    {
    case eitMeleeWeapon:
        *pnIndex = perWeaponStyle;
        break;

    case eitHelm:
        *pnIndex = perHelmStyle;
        break;

    case eitChest:
        *pnIndex = perChestStyle;
        break;

    case eitWaist:
        *pnIndex = perWaistStyle;
        break;

    case eitBoots:
        *pnIndex = perBootsStyle;
        break;

    case eitBangle:
        *pnIndex = perBangleStyle;
        break;

    case eitWaistExtend:
        *pnIndex = perWaistExtend;
        break;

    case eitBackExtend:
        *pnIndex = perBackExtend;
        break;

    case eitHorse:
        *pnIndex = perHorseStyle;
        break;

	default:
        goto Exit0;
    }

    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::GetEquipColorIndex(DWORD dwEquipPos, int* pnIndex)
{
    BOOL bResult = false;

    switch (dwEquipPos)
    {
    case eitHelm:
        *pnIndex = perHelmColor;
        break;

    case eitChest:
        *pnIndex = perChestColor;
        break;

    case eitWaist:
        *pnIndex = perWaistColor;
        break;

    case eitBoots:
        *pnIndex = perBootsColor;
        break;

    case eitBangle:
        *pnIndex = perBangleColor;
        break;

	default:
        goto Exit0;
    }

    bResult = true;
Exit0:
    return bResult;
}

BOOL KItemList::PushItem(KItem* pItem, DWORD dwBox, BOOL bNotifyClient)
{
    BOOL    bResult         = false;
    BOOL    bRetCode        = false;
    int     nCount          = 0;

    KGLOG_PROCESS_ERROR(dwBox < ibTotal);
    KGLOG_PROCESS_ERROR(m_Box[dwBox].m_nBoxType == ivtPackage);
    KGLOG_PROCESS_ERROR(pItem);

    assert(m_pPlayer);

    nCount = pItem->GetStackNum();

    for (DWORD i = 0; i < m_Box[dwBox].m_dwSize; i++)
    {
        if (m_Box[dwBox].m_pItemArray[i] && m_Box[dwBox].m_pItemArray[i]->CanStack(pItem))
        {
            int nOldCount = nCount;
            nCount = m_Box[dwBox].m_pItemArray[i]->Stack(nCount);
#ifdef _SERVER
            g_PlayerServer.DoUpdateItemAmount(
                m_pPlayer->m_nConnIndex, dwBox, i, m_Box[dwBox].m_pItemArray[i]->GetStackNum()
            );
            if (bNotifyClient)
            {
			    g_PlayerServer.DoAddItemNotify(
                    m_pPlayer, m_Box[dwBox].m_pItemArray[i]->m_dwID, nOldCount - nCount
                );
            }
#endif
            if (nCount == 0)
                break;
        }
    }

    bRetCode = pItem->SetStackNum(nCount);
    KGLOG_CHECK_ERROR(bRetCode);

    bResult = true;
Exit0:
    return bResult;
}

ITEM_RESULT_CODE KItemList::ExchangePackageSlot(DWORD dwBox1, DWORD dwBox2)
{
	ITEM_RESULT_CODE    nResult     = ircFailed;
    KInventory          ivTemp;

	KGLOG_PROCESS_ERROR(dwBox1 < ibTotal && dwBox2 < ibTotal);
	KGLOG_PROCESS_ERROR(dwBox1 != dwBox2);
    
	ivTemp = m_Box[dwBox1];
	
    m_Box[dwBox1].m_dwSize                  = m_Box[dwBox2].m_dwSize;
    m_Box[dwBox1].m_dwUsed                  = m_Box[dwBox2].m_dwUsed;
    m_Box[dwBox1].m_nContainItemGenerType   = m_Box[dwBox2].m_nContainItemGenerType;
    m_Box[dwBox1].m_nContainItemSubType     = m_Box[dwBox2].m_nContainItemSubType;

    memcpy(m_Box[dwBox1].m_pItemArray, m_Box[dwBox2].m_pItemArray, sizeof(m_Box[dwBox1].m_pItemArray));

    m_Box[dwBox2].m_dwSize                  = ivTemp.m_dwSize;
    m_Box[dwBox2].m_dwUsed                  = ivTemp.m_dwUsed;
    m_Box[dwBox2].m_nContainItemGenerType   = ivTemp.m_nContainItemGenerType;
    m_Box[dwBox2].m_nContainItemSubType     = ivTemp.m_nContainItemSubType;

    memcpy(m_Box[dwBox2].m_pItemArray, ivTemp.m_pItemArray, sizeof(m_Box[dwBox2].m_pItemArray));

	nResult = ircSuccess;
Exit0:
	return nResult;
}

ITEM_RESULT_CODE KItemList::EnablePackage(DWORD dwBox, DWORD dwSize, int nContainGenerType, int nContainSubType)
{
	ITEM_RESULT_CODE    nResult   = ircFailed;
    DWORD               dwOldSize = 0;
    int                 nUsedSize = 0;
    KInventory*         pBox      = NULL;

	KGLOG_PROCESS_ERROR(dwBox < ibTotal);
    
    pBox = &m_Box[dwBox];
    nUsedSize = pBox->GetUsedCount();
    
    if (nContainGenerType != INVALID_CONTAIN_ITEM_TYPE)
    {
        if (nContainGenerType != pBox->m_nContainItemGenerType)
        {
            KG_PROCESS_ERROR_RET_CODE(nUsedSize == 0, ircPackageErrorType);
        }

        if (nContainSubType != INVALID_CONTAIN_ITEM_TYPE && nContainSubType != pBox->m_nContainItemSubType)
        {
            KG_PROCESS_ERROR_RET_CODE(nUsedSize == 0, ircPackageErrorType);
        }
    }
    
    KG_PROCESS_ERROR_RET_CODE(nUsedSize <= (int)dwSize, ircPackageNotEnoughSize);
    
    pBox->SetContainItemType(nContainGenerType, nContainSubType);

	dwOldSize = m_Box[dwBox].m_dwSize;
	// ����
	if (dwOldSize > dwSize)
	{
		DWORD dwFirstFree = 0;
		DWORD dwFirstOver = dwSize;
		while(true)
		{
			DWORD i, j;
			for (j = dwFirstOver; j < dwOldSize; j++)
			{
				if (m_Box[dwBox].GetItem(j))
				{
					dwFirstOver = j;
					break;
				}
			}

			if (j == dwOldSize)
				break;

			for (i = dwFirstFree; i < dwSize; i++)
			{
				if (m_Box[dwBox].GetItem(i) == NULL)
				{
					dwFirstFree = i;
					break;
				}	
			}
			assert(i < dwSize);
			KItem* pItem = m_Box[dwBox].PickUpItem(dwFirstOver);
			m_Box[dwBox].PlaceItem(pItem, dwFirstFree);
		}
	}
	
	m_Box[dwBox].m_dwSize = dwSize;

	nResult = ircSuccess;
Exit0:
	return nResult;
}

ITEM_RESULT_CODE KItemList::DisablePackage(DWORD dwBox)
{
	ITEM_RESULT_CODE nResult = ircFailed;

	KGLOG_PROCESS_ERROR(dwBox < ibTotal);
	KGLOG_PROCESS_ERROR(m_Box[dwBox].m_dwUsed == 0);

	m_Box[dwBox].m_nContainItemGenerType = INVALID_CONTAIN_ITEM_TYPE;
    m_Box[dwBox].m_nContainItemSubType   = INVALID_CONTAIN_ITEM_TYPE;
	m_Box[dwBox].m_dwSize = 0;

	nResult = ircSuccess;
Exit0:
	return nResult;
}

int KItemList::GetEquipedSetNum(DWORD dwSetID)
{
	int nResult = 0;

	for (int i = 0; i < (int)(m_Box[ibEquip].m_dwSize); i++)
	{
		KItem* pItem = GetItem(ibEquip, i);
		if (pItem && pItem->m_bApplyed && pItem->m_Common.dwSetID == dwSetID)
		{
			nResult++;
		}
	}
	return nResult;
}

BOOL KItemList::ApplySetAttrib(DWORD dwSetID, int nSetNum)
{
	BOOL	bResult = false;
	BOOL	bRetCode = false;

	KAttribute	Attrib;
	bRetCode = g_pSO3World->m_ItemManager.GetSetAttrib(Attrib, dwSetID, nSetNum);
	KG_PROCESS_ERROR(bRetCode);

	m_pPlayer->ApplyAttribute(&Attrib);

	bResult = bRetCode;
Exit0:
	return bResult;
}

BOOL KItemList::UnApplySetAttrib(DWORD dwSetID, int nSetNum)
{
	BOOL	bResult = false;
	BOOL	bRetCode = false;

	KAttribute	Attrib;
	bRetCode = g_pSO3World->m_ItemManager.GetSetAttrib(Attrib, dwSetID, nSetNum);
	KG_PROCESS_ERROR(bRetCode);

	m_pPlayer->UnApplyAttribute(&Attrib);

	bResult = bRetCode;
Exit0:
	return bResult;
}

BOOL KItemList::CostItemInBox(int nBoxIndex, DWORD dwTabType, DWORD dwIndex, DWORD& rdwAmount, int nBookRecipeID)
{
    BOOL                bResult         = false;
    ITEM_RESULT_CODE    eRetCode        = ircFailed;
    
    KGLOG_PROCESS_ERROR(nBoxIndex >= 0 && nBoxIndex < ibTotal);

    for (DWORD dwX = 0; dwX < m_Box[nBoxIndex].m_dwSize; ++dwX)
    {
        KItem*      pItem   = m_Box[nBoxIndex].GetItem(dwX);
        TItemPos    ItemPos = {nBoxIndex, dwX};

        if (rdwAmount == 0)
            break;

        if (pItem == NULL)
            continue;

        if (pItem->m_GenParam.dwTabType != dwTabType || pItem->m_GenParam.dwIndex != dwIndex)
            continue;
        
        if (nBookRecipeID != -1)
        {
            if (pItem->m_Common.nGenre != igBook || pItem->m_nCurrentDurability != nBookRecipeID)
            {
                continue;
            }
        }
		if (pItem->IsStackable())
		{
			if ((DWORD)pItem->m_nStackNum > rdwAmount)
			{
				pItem->m_nStackNum -= (int)rdwAmount;
                #ifdef _SERVER
				g_PlayerServer.DoUpdateItemAmount(m_pPlayer->m_nConnIndex, nBoxIndex, dwX, pItem->GetStackNum());
                #endif
                rdwAmount = 0;
				goto Exit1;
			}

			if ((DWORD)pItem->m_nStackNum == rdwAmount)
			{
				eRetCode = DestroyItem(ItemPos);
                KGLOG_PROCESS_ERROR(eRetCode == ircSuccess);
                rdwAmount = 0;
				goto Exit1;
			}

			rdwAmount -= pItem->m_nStackNum;

			eRetCode = DestroyItem(ItemPos);
            KGLOG_PROCESS_ERROR(eRetCode == ircSuccess);
		}
		else
		{
			eRetCode = DestroyItem(ItemPos);
            KGLOG_PROCESS_ERROR(eRetCode == ircSuccess);
			--rdwAmount;
		}
    }

Exit1:
    bResult = true;
Exit0:
    return bResult;
}

USE_ITEM_RESULT_CODE KItemList::CanUseItem(KItem* pItem, DWORD dwBox, KUSE_ITEM* pUseItemInfo)
{
    USE_ITEM_RESULT_CODE nResult        = uircFailed;
    BOOL                 bRetCode       = false;
    DWORD                dwTabType      = 0;
    KOtherItemInfo*      pItemInfo      = NULL;

    assert(pItem);
    assert(pUseItemInfo);

    KGLOG_PROCESS_ERROR(dwBox < ibTotal);

    KG_PROCESS_ERROR(pItem->m_dwScriptID != 0 || pItem->m_dwSkillID != 0);
    
    if (pItem->m_Common.nGenre == igEquipment)
    {
        KGLOG_PROCESS_ERROR(m_Box[dwBox].m_nBoxType == ivtEquipment);
    }
    else
    {
        KGLOG_PROCESS_ERROR(m_Box[dwBox].m_nBoxType == ivtPackage);
    }

    if (pItem->m_Common.bConsumeDurabiltity)
    {
        int nCurrentDurability = pItem->GetCurrentDurability();
        KG_PROCESS_ERROR_RET_CODE(nCurrentDurability != 0, uircNotEnoughDurability);
    }
    
    bRetCode = pItem->CheckCoolDown(m_pPlayer);
    KG_PROCESS_ERROR_RET_CODE(bRetCode, uircNotReady);
        
    dwTabType = pItem->m_GenParam.dwTabType;
    KG_PROCESS_SUCCESS(dwTabType != ittOther);

    pItemInfo = (KOtherItemInfo*)pItem->GetItemInfo();
    KGLOG_PROCESS_ERROR(pItemInfo);

    KG_PROCESS_ERROR_RET_CODE(pItemInfo->nRequireLevel == 0 || m_pPlayer->m_nLevel >= pItemInfo->nRequireLevel, uircTooLowLevel);
    
    if (pItemInfo->dwRequireProfessionID != 0)
    {
        pUseItemInfo->dwProfessionID = pItemInfo->dwRequireProfessionID;

        bRetCode = m_pPlayer->m_ProfessionList.IsProfessionLearned(pItemInfo->dwRequireProfessionID);
        KG_PROCESS_ERROR_RET_CODE(bRetCode, uircRequireProfession);

        if (pItemInfo->dwRequireProfessionBranch != 0)
        {
            DWORD dwBranchID = 0;
            dwBranchID = m_pPlayer->m_ProfessionList.GetBranchID(pItemInfo->dwRequireProfessionID);

            pUseItemInfo->dwProfessionBranchID = pItemInfo->dwRequireProfessionBranch;
            KG_PROCESS_ERROR_RET_CODE(dwBranchID == pItemInfo->dwRequireProfessionBranch, uircRequireProfessionBranch);
        }

        if (pItemInfo->nRequireProfessionLevel != 0)
        {
            DWORD dwProfessionLevel = 0;
            dwProfessionLevel = m_pPlayer->m_ProfessionList.GetProfessionLevel(pItemInfo->dwRequireProfessionID);

            pUseItemInfo->dwProfessionLevel = pItemInfo->nRequireProfessionLevel;

            KG_PROCESS_ERROR_RET_CODE(dwProfessionLevel >= (DWORD)pItemInfo->nRequireProfessionLevel, uircProfessionLevelTooLow);
        }
    }
    
    if (pItemInfo->nRequireGender != 0)
    {
        if (m_pPlayer->m_eGender == cgMale)
        {
            KG_PROCESS_ERROR_RET_CODE(pItemInfo->nRequireGender == m_pPlayer->m_eGender, uircRequireFeMale);
        }
        else if (m_pPlayer->m_eGender == cgFemale)
        {
            KG_PROCESS_ERROR_RET_CODE(pItemInfo->nRequireGender == m_pPlayer->m_eGender, uircRequireMale);
        }
    }
    
    KG_PROCESS_ERROR_RET_CODE(!m_pPlayer->m_bOnHorse || pItemInfo->bCanUseOnHorse != 0, uircOnHorse);
    KG_PROCESS_ERROR_RET_CODE(!m_pPlayer->m_bFightState || pItemInfo->bCanUseInFight != 0, uircInFight);
    KG_PROCESS_ERROR_RET_CODE(pItemInfo->nRequireCamp & (0x01 << m_pPlayer->m_eCamp), uircCampCannotUse);
    KG_PROCESS_ERROR_RET_CODE(
        pItemInfo->dwRequireForceID == 0 || pItemInfo->dwRequireForceID == m_pPlayer->m_dwForceID, 
        uircForceError
    );

Exit1:
    nResult = uircSuccess;
Exit0:
    return nResult;
}

BOOL KItemList::CheckUseItemTargetType(KItem* pItem)
{
    BOOL        bResult     = false;
    TARGET_TYPE eType       = ttInvalid;
    DWORD       dwTargetID  = ERROR_ID;
    KItemInfo*  pItemInfo   = NULL;

    assert(pItem);
    assert(m_pPlayer);
    
    eType = m_pPlayer->m_SelectTarget.GetTargetType();
    KGLOG_PROCESS_ERROR(eType != ttInvalid);
    
    dwTargetID = m_pPlayer->m_SelectTarget.GetTargetID();

    pItemInfo = pItem->GetItemInfo();
    KGLOG_PROCESS_ERROR(pItemInfo);
    
    switch (pItemInfo->nUseTargetType)
    {
    case iuttNoTarget:
        break;
    case iuttSelf:
        KGLOG_PROCESS_ERROR(eType == ttPlayer && dwTargetID == m_pPlayer->m_dwID);
    	break;
    case iuttOtherPlayer:
        KGLOG_PROCESS_ERROR(eType == ttPlayer && dwTargetID != m_pPlayer->m_dwID);
        break;
    case iuttNpc:
        KGLOG_PROCESS_ERROR(eType == ttNpc);
        break;
    case iuttCoordinates:
        KGLOG_PROCESS_ERROR(eType == ttCoordination);
    	break;
    case iuttDoodad:
        KGLOG_PROCESS_ERROR(eType == ttDoodad);
    	break;
    case iuttItem:
        KGLOG_PROCESS_ERROR(eType == ttItemPos);
        break;
    default:
        assert(false);
        break;
    }

    bResult = true;
Exit0:
    return bResult;
}

#ifdef _SERVER
USE_ITEM_RESULT_CODE KItemList::UseItem(TItemPos ItemPos, KTarget& rTarget)
{
    USE_ITEM_RESULT_CODE    nResult         = uircFailed;
    USE_ITEM_RESULT_CODE    eRetCode        = uircFailed;
    BOOL                    bRetCode        = false;
    BOOL                    bCostItemAtOnce = true;
	BOOL                    bDestroyItem    = false;
	BOOL                    bContinueSkill  = true;    
    KItem*                  pItem           = NULL;
    KUSE_ITEM               UseItemInfo;

    assert(m_pPlayer);
    KGLOG_PROCESS_ERROR(ItemPos.dwBox < ibTotal);

    pItem = GetItem(ItemPos.dwBox, ItemPos.dwX);
    KG_PROCESS_ERROR_RET_CODE(pItem, uircItemNotExist);

    eRetCode = CanUseItem(pItem, ItemPos.dwBox, &UseItemInfo);
    KG_PROCESS_ERROR_RET_CODE(eRetCode == uircSuccess, eRetCode);

    CallItemScript(pItem, ItemPos, rTarget, bContinueSkill, bDestroyItem);

	if (bContinueSkill)
    {
        eRetCode = CastItemSkill(pItem, rTarget, &bCostItemAtOnce);
        KG_PROCESS_ERROR_RET_CODE(eRetCode == uircSuccess, eRetCode);
    }

    bRetCode = pItem->ResetCoolDown(m_pPlayer);
    KGLOG_PROCESS_ERROR(bRetCode);
    
    if (bContinueSkill) // �ͷż���Ҫ����
    {
        if (!bCostItemAtOnce) // �������ܲ�������������Ʒ
        {
            goto Exit1;
        }

        if (pItem->m_Common.bConsumeDurabiltity)
        {
            bRetCode = CostSingleItem(ItemPos.dwBox, ItemPos.dwX, 1);
		    KGLOG_PROCESS_ERROR_RET_CODE(bRetCode,  uircCostItemDurabilityFailed);
            
            goto Exit1;
        }
    }

	if (bDestroyItem) // �ű�����Ҫ����
    {
        if (pItem->m_Common.bConsumeDurabiltity)
        {
            bRetCode = CostSingleItem(ItemPos.dwBox, ItemPos.dwX, 1);
		    KGLOG_PROCESS_ERROR_RET_CODE(bRetCode,  uircCostItemDurabilityFailed);

            goto Exit1;
        }

        bRetCode = DestroyItem(ItemPos);
        KGLOG_PROCESS_ERROR(bRetCode == uircSuccess);
    }

Exit1:
    nResult = uircSuccess;
Exit0:
    if (nResult != uircSuccess)
    {
        g_PlayerServer.DoMessageNotify(
            m_pPlayer->m_nConnIndex, ectUseItemErrorCode, nResult, &UseItemInfo, sizeof(UseItemInfo)
        );
    }
    return nResult;
}

USE_ITEM_RESULT_CODE KItemList::UseItem(DWORD dwBox, DWORD dwX, KTarget& rTarget)
{
    USE_ITEM_RESULT_CODE    nResult         = uircFailed;
    USE_ITEM_RESULT_CODE    eRetCode        = uircFailed;
    BOOL                    bRetCode        = false;
    KItem*                  pItem           = NULL;
    KUSE_ITEM               UseItemInfo;

    assert(m_pPlayer);
    KGLOG_PROCESS_ERROR(dwBox < ibTotal);
    
    KGLOG_PROCESS_ERROR(m_pPlayer->m_eMoveState != cmsOnDeath);

    pItem = GetItem(dwBox, dwX);
    KG_PROCESS_ERROR(pItem);

    eRetCode = CanUseItem(pItem, dwBox, &UseItemInfo);
    KG_PROCESS_ERROR_RET_CODE(eRetCode == uircSuccess, eRetCode);
    
    if (pItem->m_dwSkillID == 0 && pItem->m_dwScriptID)
    {
        eRetCode = UseItemOnlyScripts(pItem, rTarget);
        KG_PROCESS_ERROR_RET_CODE(eRetCode == uircSuccess, eRetCode);
    }

    nResult = uircSuccess;
Exit0:
    if (nResult != uircSuccess)
    {
        g_PlayerServer.DoMessageNotify(
            m_pPlayer->m_nConnIndex, ectUseItemErrorCode, nResult, &UseItemInfo, sizeof(UseItemInfo)
        );
    }
    return nResult;
}

USE_ITEM_RESULT_CODE KItemList::UseItemOnlyScripts(KItem* pSrcItem, KTarget& rTarget)
{
    USE_ITEM_RESULT_CODE    nResult     = uircFailed;
    BOOL                    bRetCode    = false;
    int                     nTopIndex   = 0;

    DWORD                   dwOTAniID       = 0;
    DWORD                   dwEndAniID      = 0;
    int                     nFrames         = 0;
    BOOL                    bSetCD          = false;
    BOOL                    bCastSkill      = false;
    char*                   pszText         = NULL;
    
    KOT_ACTION_PARAM        Param;

    bRetCode = g_pSO3World->m_ScriptCenter.IsScriptExist(pSrcItem->m_dwScriptID);
    KGLOG_PROCESS_ERROR(bRetCode);

    bRetCode = g_pSO3World->m_ScriptCenter.IsFuncExist(pSrcItem->m_dwScriptID, START_USE_ITEM);
    KGLOG_PROCESS_ERROR(bRetCode);

    g_pSO3World->m_ScriptCenter.SafeCallBegin(&nTopIndex);

    g_pSO3World->m_ScriptCenter.PushValueToStack(m_pPlayer);
    g_pSO3World->m_ScriptCenter.PushValueToStack(pSrcItem);
    g_pSO3World->m_ScriptCenter.PushValueToStack(&rTarget);

    g_pSO3World->m_ScriptCenter.CallFunction(pSrcItem->m_dwScriptID, START_USE_ITEM, 6);

    g_pSO3World->m_ScriptCenter.GetValuesFromStack("dddsbb", (int*)&dwOTAniID, (int*)&dwEndAniID, &nFrames, &pszText, &bSetCD, &bCastSkill);
    g_pSO3World->m_ScriptCenter.SafeCallEnd(nTopIndex);
    
    KGLOG_PROCESS_ERROR(pszText);
    KGLOG_PROCESS_ERROR(nFrames >= 0);
    
    if (bSetCD)
    {
        bRetCode = pSrcItem->ResetCoolDown(m_pPlayer);
        KGLOG_PROCESS_ERROR(bRetCode);
    }

    Param.eActionType       = otActionUseItemPrepare;
    Param.Data.dwItemID     = pSrcItem->m_dwID;
    Param.nEndFrame         = g_pSO3World->m_nGameLoop + nFrames;
    Param.Target            = rTarget;
    
    bRetCode = m_pPlayer->DoOTAction(Param);
    KGLOG_PROCESS_ERROR(bRetCode);

    g_PlayerServer.DoUseItemPrepare(m_pPlayer, nFrames, dwOTAniID, dwEndAniID, pszText);

    nResult = uircSuccess;
Exit0:
    return nResult;
}

void KItemList::OnBreakUseItem(KItem* pItem)
{   
    BOOL bRetCode = false;
    
    //KGLOG_PROCESS_ERROR(pItem); // pItem could be null
    KGLOG_PROCESS_ERROR(m_pPlayer->m_OTActionParam.eActionType == otActionUseItemPrepare);
    
    g_PlayerServer.DoSkillBeatBack(m_pPlayer, 0, true);
    m_pPlayer->DoActionIdle();
    
    KG_PROCESS_ERROR(pItem);

    if (pItem->m_dwScriptID)
    {
        KSkill* pSkill = g_pSO3World->m_SkillManager.GetSkill_RAW(pItem->m_dwSkillID, pItem->m_dwSkillLevel);
        KGLOG_PROCESS_ERROR(pSkill);

        pSkill->ClearPublicCooldown(m_pPlayer); 
    }

    bRetCode = g_pSO3World->m_ScriptCenter.IsScriptExist(pItem->m_dwScriptID);
    if (bRetCode && g_pSO3World->m_ScriptCenter.IsFuncExist(pItem->m_dwScriptID, USE_ITEM_FAILD))
    {   
        int     nTopIndex = 0;
        BOOL    bCostItem = false;
        BOOL    bSetCD    = false;

        g_pSO3World->m_ScriptCenter.SafeCallBegin(&nTopIndex);

        g_pSO3World->m_ScriptCenter.PushValueToStack(m_pPlayer);
        g_pSO3World->m_ScriptCenter.PushValueToStack(pItem);
        g_pSO3World->m_ScriptCenter.PushValueToStack(&(m_pPlayer->m_OTActionParam.Target));

        g_pSO3World->m_ScriptCenter.CallFunction(pItem->m_dwScriptID, USE_ITEM_FAILD, 2);

        g_pSO3World->m_ScriptCenter.GetValuesFromStack("bb", &bCostItem, &bSetCD);
        g_pSO3World->m_ScriptCenter.SafeCallEnd(nTopIndex);

        if (bSetCD)
        {
            bRetCode = pItem->ResetCoolDown(m_pPlayer);
            KGLOG_PROCESS_ERROR(bRetCode);
        }

        if (bCostItem)
        {
            DWORD dwBox = 0;
            DWORD dwX   = 0;

            bRetCode = GetItemPos(pItem->m_dwID, dwBox, dwX);
            KGLOG_PROCESS_ERROR(bRetCode);

            if (pItem->m_Common.bConsumeDurabiltity)
            {
                bRetCode = CostSingleItem(dwBox, dwX, 1);
		        KGLOG_PROCESS_ERROR(bRetCode);
                
                goto Exit0;
            }
        }
    }
    
Exit0:
    return;
}
#endif

void KItemList::FinishUseItem(KItem* pItem)
{
    BOOL bRetCode = false;
    
#ifdef _SERVER
    assert(pItem);

    if (pItem->m_dwSkillID)
    {
        DWORD	            dwSkillID		= 0;
	    DWORD	            dwSkillLevel	= 0;
        KSkill*	            pSkill			= NULL;
        KSKILL_RECIPE_KEY   RecipeKey;
        SKILL_RESULT_CODE   eRetCode        = srcFailed;

        dwSkillID = pItem->m_dwSkillID;
	    dwSkillLevel = pItem->m_dwSkillLevel;

        bRetCode = g_pSO3World->m_SkillManager.GetSkillRecipeKey(&RecipeKey, dwSkillID, dwSkillLevel, m_pPlayer);
        KGLOG_PROCESS_ERROR(bRetCode);
    	
	    pSkill = g_pSO3World->m_SkillManager.GetSkill(RecipeKey);
	    KGLOG_PROCESS_ERROR(pSkill);

        eRetCode = pSkill->CanCastOnPrepare(m_pPlayer, m_pPlayer->m_OTActionParam.Target);
	    if (eRetCode != srcSuccess)
	    {
            OnBreakUseItem(pItem);
		    g_PlayerServer.DoMessageNotify(m_pPlayer->m_nConnIndex, ectSkillErrorCode, eRetCode);
		    goto Exit0;
	    }

	    pSkill->DoCost(m_pPlayer);
        pSkill->ResetNormalCooldown(m_pPlayer);

	    g_PlayerServer.DoSkillCast(m_pPlayer, pSkill, m_pPlayer->m_OTActionParam.Target, false);

	    eRetCode = pSkill->Cast(m_pPlayer, m_pPlayer, m_pPlayer->m_OTActionParam.Target, RecipeKey);
	    if (eRetCode != srcSuccess)
        {
            OnBreakUseItem(pItem);
        }
    }

    bRetCode = g_pSO3World->m_ScriptCenter.IsScriptExist(pItem->m_dwScriptID);
    if (bRetCode && g_pSO3World->m_ScriptCenter.IsFuncExist(pItem->m_dwScriptID, USE_ITEM_SUCCEED))
    {   
        int     nTopIndex = 0;
        BOOL    bCostItem = false;
        BOOL    bSetCD    = false;

        g_pSO3World->m_ScriptCenter.SafeCallBegin(&nTopIndex);

        g_pSO3World->m_ScriptCenter.PushValueToStack(m_pPlayer);
        g_pSO3World->m_ScriptCenter.PushValueToStack(pItem);
        g_pSO3World->m_ScriptCenter.PushValueToStack(&(m_pPlayer->m_OTActionParam.Target));

        g_pSO3World->m_ScriptCenter.CallFunction(pItem->m_dwScriptID, USE_ITEM_SUCCEED, 2);

        g_pSO3World->m_ScriptCenter.GetValuesFromStack("bb", &bCostItem, &bSetCD);
        g_pSO3World->m_ScriptCenter.SafeCallEnd(nTopIndex);

        if (bSetCD)
        {
            bRetCode = pItem->ResetCoolDown(m_pPlayer);
            KGLOG_PROCESS_ERROR(bRetCode);
        }

        if (bCostItem)
        {
            DWORD dwBox = 0;
            DWORD dwX   = 0;

            bRetCode = GetItemPos(pItem->m_dwID, dwBox, dwX);
            KGLOG_PROCESS_ERROR(bRetCode);

            if (pItem->m_Common.bConsumeDurabiltity)
            {
                bRetCode = CostSingleItem(dwBox, dwX, 1);
		        KGLOG_PROCESS_ERROR(bRetCode);
                
                goto Exit0;
            }
        }
    }
#endif
    
#ifdef _CLIENT
    if (g_pGameWorldRepresentHandler)
    {
        //g_pGameWorldRepresentHandler->OnCharacterPlayAnimation(m_pPlayer, m_pPlayer->m_OTActionParam.dwEndAniID);
    }
#endif

Exit0:
    return;
}
