#ifndef _KWEAPONTYPELIST_H_
#define _KWEAPONTYPELIST_H_
/*
�����ķ���,����������ʹ�÷��ű���:
�������Ԫ�ز����Ǿ����ĳ������,����WEAPON_DETAIL_TYPE.
WEAPON_DETAIL_TYPE ����˵��ĳ�������ǵ�ǹ����...�е���һ��.
��������ּ���������ܶ������ķ�������.
����:
ĳ������Ҫ��1������,1����������: ǹ,��,��
ĳ������Ҫ��2������,2����������: ��,��
*/

#include <map>
#include <vector>

class KWeaponTypeList
{
public:
    BOOL Init();
    void UnInit();

    BOOL CheckType(DWORD dwTypeID, int eDetailType);

    int  GetWeaponGenre(DWORD dwTypeID);

private:
    typedef std::vector<int> KDETAIL_TYPE_TABLE;

    struct KWEAPON_REQUEST_INFO
    {
        int                 eWeaponGenre;     // ��ʾ�ǽ��̻���Զ�� 
        KDETAIL_TYPE_TABLE  DetailTypeTable;  // ��ʾ��,��,ǹ����ϸ����
    };

    typedef std::map<DWORD, KWEAPON_REQUEST_INFO> KTYPE_LIST;
    KTYPE_LIST m_TypeList;

    BOOL LoadLine(ITabFile* piTabFile, int nLine);
};

#endif

