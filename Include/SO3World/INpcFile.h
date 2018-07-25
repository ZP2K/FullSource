//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   INpcFile.h
//  Version     :   1.0
//  Creater     :   Zhao Chunfeng
//  Date        :   2008-05-28 15:11:19
//  Comment     :   NPC file interface
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _INPCFILE_H_
#define _INPCFILE_H_

#include "KSUnknown.h"

#pragma	pack(1)
struct KNPC_DATA                        // 96 bytes in total
{
    char    szNickName[_NAME_LEN];
    DWORD   dwTemplateID;
    int     nX;
    int     nY;
    int     nZ;
    BYTE    byFaceDirection;
    BYTE    byKind;						// Npc���ͣ���CHARACTER_KIND
    DWORD   dwScriptID;
    DWORD   dwReliveID;
    DWORD   dwRandomID;                 // �������ID�������0����ʾ�����
    DWORD   dwAIType;
    DWORD   dwThreatLinkID;             // ������ӵķ���ID
    int     nPatrolPathID;              // Ѱ··��ID��ͬID��NPC�Զ���Ϊһ��
    int     nOrderIndex;                // NPC�ڶ����е�λ��
    DWORD   dwRepresentID;
    int     nProgressID;
    BYTE    byReserved[10];
};
#pragma pack()

struct INpcFile : IUnknown
{
    virtual BOOL Push(const KNPC_DATA& crNpcInfo) = 0;
    virtual BOOL Save(const char cszName[]) = 0;

    virtual BOOL Load(const char cszName[]) = 0;
    virtual BOOL Pop(KNPC_DATA* pNpcInfo) = 0;

    virtual int  GetCount() = 0;
};

INpcFile* CreateNpcFileInterface();

#endif
