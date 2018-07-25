////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DSceneFunctions.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2009-1-4 16:37:11
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DSCENEFUNCTIONS_H_
#define _INCLUDE_KG3DSCENEFUNCTIONS_H_

////////////////////////////////////////////////////////////////////////////////
class KG3DSceneSelectionTool;
class KG3DScene;
class KG3DMessageSenderBase;
class KG3DRepresentNPC;

struct KG3DSelectionBBoxGetter 
{
	virtual HRESULT GetBBox(KG3DSceneSelectionTool& selTool, AABBOX& boxRet) = 0;
	virtual ~KG3DSelectionBBoxGetter() = 0{}
};
struct IELogicalSceneConnection;
namespace KG3D_SCENE_FUNCTIONS
{
	HRESULT GetSelectionCenter(KG3DSceneSelectionTool& selTool, D3DXVECTOR3& vCenterRet);	//��û��ѡ����Ŀ��ʱ���ʧ��,���ʹ��Box���õ����ĵ�

	KG3DSelectionBBoxGetter* BuildDefaultSelectionBoxGetter();

	HRESULT SetSelectionSFXScaleSize(KG3DSceneSelectionTool& selTool, FLOAT fSize);

	HRESULT FrameMoveSceneRunner(KG3DRepresentNPC& npcToMove, KG3DScene& sceneCur);

	//����������������ֱ��ʹ��
	HRESULT SelectionTranslationEntity(KG3DSceneSelectionTool& selTool, const D3DXVECTOR3& Move);
	HRESULT SelectionRotationEntity(KG3DSceneSelectionTool& selTool, const D3DXMATRIX& matRotation);
	HRESULT SelectionScaleEntity(KG3DSceneSelectionTool& selTool, const D3DXVECTOR3& vScaling, KG3DCOORD dwMode);

	VOID SceneToLogicalDirect(const D3DXVECTOR3& vSceneCoord,INT *pX, INT *pY, INT *pZ );//�������굽�߼����ֱ꣬��ת��
	VOID LogicalToSceneDirect(const D3DXVECTOR3& vLogic, D3DXVECTOR3& vScene);	//�߼����굽�������ֱ꣬��ת��
	POINT  LogicalToLogicalRegionIndex(FLOAT fLogical);	//�߼����굽�߼�RegionIndex������

	FLOAT GetLogicalHeight(IELogicalSceneConnection& logicalCnt, const D3DXVECTOR3& vLogicalCoord, size_t* pOrderOfCell);	//���߼�����õ��߼��߶ȣ�ʧ�ܷ���-FLT_MAX�������������pOrderOfCell������֪����ǰ���ĸ�Cell������CellҪ���ȵ�ǰ�߶ȵͣ�Ҫ��������ǰ�߶�
	
};

namespace KSF = KG3D_SCENE_FUNCTIONS;



#endif //_INCLUDE_KG3DSCENEFUNCTIONS_H_
