#ifndef _KNPC_ORDER_LIST_H_
#define _KNPC_ORDER_LIST_H_

// -------------------------------------------------------------------------------
// ----------------------- NPC ���ͱ� ---------------------------

#include <vector>
#include <map>

// �����������������(������)�������г�Ա�����"��"������,��λ: (��, 360/256��)
struct KORDER_NODE 
{
    int nRadius;
    int nAngel;
};

typedef std::vector<KORDER_NODE> KORDER;

class KNpcOrderManager
{
public:
    BOOL Init();
    void UnInit();

    const KORDER* GetOrder(DWORD dwID);

private:
    typedef std::map<DWORD, KORDER> KORDER_LIST;

    KORDER_LIST m_OrderList;

    BOOL LoadOrder(KORDER& rOrder, const char cszFileName[], float fZoom);
};

#endif
