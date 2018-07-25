/************************************************************************
* @file      : igameptcallback.h
* @author    : ChenZhiQiang <chenzhiqiang@kingsoft.com>
* @date      : 2009/11/19 9:46:17
* @brief     : 
*
* $Id: igameptcallback.h,v 1.1 2009/12/07 10:18:40 kuangsihao Exp $
/************************************************************************/
#ifndef __IGAMEPTCALLBACK_H__
#define __IGAMEPTCALLBACK_H__

// -------------------------------------------------------------------------

/**
* @class        IGamePt
* @brief        �ܱ�֪ͨ��һص��ӿڶ���
*
* @details      
*/
[
    uuid("5B835A22-BB1F-42ba-9EE1-1E88E8917B29")
]
interface IGamePtCallback : public IUnknown
{
    /**
    * @brief        ����֪ͨ��Ϣ
    * @param        DWORD dwGameID             ��Ϸid
    * @param        DWORD dwRabotID            ���id
    * @param        const WCHAR* pszGamePath   ��Ϸ·��        
    * @param        const WCHAR* pszRabotPath  ���·��   
    * @remark       
    */
    virtual STDMETHODIMP NotifyInfo(
                DWORD dwGameID, 
                DWORD dwRabotID,
                const WCHAR* pszGamePath,
                const WCHAR* pszRabotPath) = 0;
};

// -------------------------------------------------------------------------
// $Log: igameptcallback.h,v $
// Revision 1.1  2009/12/07 10:18:40  kuangsihao
// ����duba��dll
//

#endif /* __IGAMEPTCALLBACK_H__ */
