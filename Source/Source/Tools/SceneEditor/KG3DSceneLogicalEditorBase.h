#pragma once
#include "IEEditor.h" //by dengzhihui Refactor 2006��11��16��

//#include "KG3DStructs.h"
class KScene;
class KRegion;
class KCell;
interface IEKG3DTerrain;
class CInputProxy;

#define MAXCELLSRENDEREACHTIME 512 //ÿ����໭���ٸ�����

class KG3DSceneLogicalEditorBase : public IEKG3DSceneLogicalEditorBase
{

private:
	LPDIRECT3DVERTEXBUFFER9 m_pCellVB;
	LPDIRECT3DINDEXBUFFER9  m_pCellIB;
	int m_nCurRegionX,m_nCurRegionZ;//��ǰ�༭����REGION
protected:
	struct CellInfo
	{
		D3DXVECTOR3 vPosition;//���½ǵ�����
		float      fTop;
		KCell*     pCell;
		BOOL       bSelected;
		AABBOX     vBBox;
		KRegion*   pRegion;
		BYTE	   byX, byY;
		float      H[4];//���涥��߶ȣ���Ӧ��ͼ����б�ȣ�����۲�
		CellInfo();
		CellInfo(bool abEditGroundCells, IEKG3DSceneSceneEditor* pEditor,int nX,int nZ,KCell* pCellInput, KRegion* argpRegion, BYTE argbyX, BYTE argbyY,float fLogicalStartX,float fLogicalStartZ);
		BOOL IsRayIntersect(float& fDistance,D3DXVECTOR3& CameraPos,D3DXVECTOR3& CameraDirct);

		//(by dengzhihui 2006��7��26�� 16:22:36
		BOOL operator==(const CellInfo& comp) const
		{
			//return vPosition == comp.vPosition; //ֻ�ж����½��Ƿ����
			return pCell == comp.pCell;
		}
		CellInfo(const CellInfo& src);
			
		//)
	};

	//BYTE m_byVBBuf[64*64*9*32];//64*64*9*24
	//(by dengzhihui 2006��7��26�� 15:51:51
	typedef list<CellInfo*> defShowListCellInfo;
	defShowListCellInfo m_listLogicalCellCurrMarked;	//���ڵ�ǰ�༭״̬�����Լ��ϵ��߼�����
	defShowListCellInfo m_listLogicalCellDiffMarked;	//�Ǳ༭״̬�����Լ��ϵ��߼�����
	//)

	list<CellInfo>m_listLogicalCellSelectableGround;//����ѡ��/�༭���߼�����
	list<CellInfo>m_listLogicalCellSelectableOverGround;//����ѡ��/�༭���߼�����
	defShowListCellInfo m_listLogicalCellSelected;//ѡ�е��߼�����
	list<KRegion*>m_listLogicalRegionEditing;//���ڱ༭���߼�����
	KScene* m_pLogicalScene;//���ڱ༭���߼�����

	
	//By Li Kan
	//��m_listLogicalCellSelectableGround�Ż����Ż�������洢�����������񵽵ĵ���cell�����ǲ���һ��region�����еĵ���cell
	list<CellInfo>m_listLogicalCellGroundSelected;



	BOOL m_bShowRest;
	BOOL m_bEditGroundCells;
	BOOL m_bEditOverGroundCells;
	CInputProxy* m_pProxy;
public:
	/*enum eCellModifyState
	{
		MODIFY_NONE          = 0,
		MODIFY_BLOCKCHARACTER = 1,
		MODIFY_PUTOBJECT     = 2,
		MODIFY_SAFE         = 4,
		MODIFY_STALL        = 8,
		MODIFY_CELLTYPE      = 16,
		MODIFY_PASSWIDTH     = 32,
		MODIFY_SLIPDIRECTION = 64,
		MODIFY_SLIPVELOCITY  =128,
//(by dengzhihui 2006��8��3�� 10:35:48
		MODIFY_INDOOR		 =256,	//��������
		MODIFY_REST			 =512,	//��Ϣ����
		MODIFY_RIDEHORSE	 =1024,	//��������
		MODIFY_SCRIPTFILE	 =2048,	//�����ļ�ID
//)
		DWORD_FORCE = 0xFFFFFFFF,
	};*///by dengzhihui Refactor 2006��11��16��
	DWORD m_dwLogicalCellModifyState;//�༭״̬
	DWORD m_dwLogicalCellShowState;  //��ʾ״̬
	DWORD m_dwCurrentScriptID;		 //��ǰѡ�еĽű�ID,�������洫����ļ�������ó�,0��ʾû��ѡ��ű�

	BOOL m_bAddCellInfo;//��ӻ���ɾ���ϰ��ϵ���Ϣ
// 	enum eCellBrushShape
// 	{
// 		cbCircle = 0, cbRectangle = 1,
// 	};//by dengzhihui Refactor 2006��11��16��

private:
	HRESULT GetShowCellList(defShowListCellInfo* pListCurrShow,defShowListCellInfo* pListDiffShow, const list<CellInfo>& listInput);

	HRESULT RenderLogicalCellList(defShowListCellInfo* pList,DWORD dwColor);

	CellInfo* GetFirstLogicalCellIntersectRay(D3DXVECTOR3& CameraPos,D3DXVECTOR3& CameraDirct);

	HRESULT CreateBuffersForRenderCell();
	//(by dengzhihui 2006��7��27�� 9:08:31
	//inline void ModifyLogicCellInfo(KCell* pCell);
	DWORD m_d3dclrCurrColor; //��ǰ���ڱ༭��CELLӦ����ʾ����ɫ
	void GetCurrentModifyMask(DWORD dwCurrentState, DWORD* pDes);
	int m_nBrushWidth, m_nDisplayWidth, m_nCellHorzHeight;
	int m_nXStart, m_nXEnd, m_nZStart, m_nZEnd; //��ǰ����ʾ���߼�����Χ
	inline void GetMapSize(int* pMapWidth, int* pMapHeight);
	eCellBrushShape m_eBrushType;
	//)

	list<D3DXVECTOR3>m_listInterPos;
protected:
	virtual HRESULT PaintSelectedLogicalCell(BOOL bAddCellInfo, BOOL bForceDel);
	virtual HRESULT RenderLogicalCell();
	virtual HRESULT SelectLogicalCells(int nXStart,int nZStart,IEKG3DSceneSceneEditor* pEditor,D3DXVECTOR3* pCameraPos,D3DXVECTOR3* pCameraDirct,D3DXVECTOR3 vGroundInter);
	virtual HRESULT UpdateEditingCells(int nXStart,int nZStart,IEKG3DSceneSceneEditor* pEditor,D3DXVECTOR3& InterPos);
public:
	virtual HRESULT Init();
	virtual HRESULT UnInit();

	HRESULT SetModifyState(eCellModifyState eState,BOOL bModify);
	BOOL GetModifyState(eCellModifyState eState);
	HRESULT SetShowState(eCellModifyState eState,BOOL bModify);
	BOOL GetShowState(eCellModifyState eState);

	HRESULT SetLogicalScene(KScene* pLogicalScene);
	KScene* GetLogicalScene();

	void SetInputProxy(CInputProxy* pProxy);

	KG3DSceneLogicalEditorBase(void);
	virtual ~KG3DSceneLogicalEditorBase(void);
	//(by dengzhihui 2006��8��3�� 10:47:49
	void RecalcShowCell();
	void SetCurrentEditColor(COLORREF clr);
	void ModifyCurrentCellInfo(long lOldInfo, long lNewInfo, LPCTSTR OldScriptFileName, LPCTSTR NewScriptFileName);
	void DeleteCurrentCellInfo();//long lLogicInfo, LPCTSTR lpstrScriptFileName);
	void SetCurrentScriptFileName(LPCTSTR lpstrFileName);
	void SetBrushInfo(int nWidth, eCellBrushShape eBrushType);
	void SetDisplayParam(int nDisplayWidth, int nCellHorzHeight);
	void SetShowRest(BOOL bShowRest);
	//)
	//(by dengzhihui Refactor 2006��11��16��
	virtual HRESULT SetShowLogicalCell(BOOL bValue);
	virtual HRESULT SetShowLogicalTerrenCell(BOOL bValue);
	virtual HRESULT SetShowLogicalItemCell(BOOL bValue);
	virtual HRESULT SetShowLogicalSelectedCell(BOOL bValue);
	virtual HRESULT GetLogicScene(LPVOID* ppLogicScene);
	virtual HRESULT SetLogicScene(LPVOID pLogicScene);
protected:
	BOOL m_bShowLogicalCell;
	BOOL m_bShowLogicalTerrenCell;
	BOOL m_bShowLogicalItemCell;
	BOOL m_bShowLogicalSelectedCell;

	virtual BOOL GetShowLogicalCell(){return m_bShowLogicalCell;}
	virtual BOOL GetShowLogicalTerrenCell(){return m_bShowLogicalTerrenCell;}
	virtual BOOL GetShowLogicalItemCell(){return m_bShowLogicalItemCell;}
	virtual BOOL GetShowLogicalSelectedCell(){return m_bShowLogicalSelectedCell;}
	//)
	virtual HRESULT GetCellModifyState(DWORD* eState);
};
