#pragma once
#ifndef _INCLUDE_KSCENELOGICCELLBUF_H_
#define _INCLUDE_KSCENELOGICCELLBUF_H_

#include "kg3dinterface.h"
#include "../EditInterface/IEEditor.h"

//region���߼�cell����ʾ���ݻ���
class KG3DLogicRegionCellBuf
{
private:

   BOOL m_bAvailable;

   //���buf��Ӧ��LogicRegion
   int m_nLogicX;
   int m_nLogicZ;

   LPDIRECT3DVERTEXBUFFER9 m_lpLogicalTerrian;
   LPDIRECT3DVERTEXBUFFER9 m_lpLogicalTerrianFace;
   LPDIRECT3DVERTEXBUFFER9 m_lpLogicalItem;
   LPDIRECT3DVERTEXBUFFER9 m_lpLogicalItemFace;

   int m_nCount1;
   int m_nCount2;

public:
   
   KG3DLogicRegionCellBuf();
   
   //�Ƿ���Ч��CellBuf��ʧЧ����������ץȡ�ķ�Χ���ڵ�ͼ֮��
   BOOL IsAvailable();

   //��LogicRegion��ȡ����
   HRESULT FetchCellDataFromLogicRegion(IELogicalSceneConnection* pLogicalScene, int nRegionX, int nRegionZ);

   //�����߼�cell
   void DrawLogicCell(BOOL bTerrian, BOOL bItem, BOOL bSelected);

   virtual ~KG3DLogicRegionCellBuf();
};

#endif