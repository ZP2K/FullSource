/*****************************************************************************************
//	����--С��ͼ
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-21
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "LinkStruct.h"
#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShadow.h"
#include "../Elem/PopupMenu.h"
#include "../elem/wndpuretextbtn.h"
#include "../../Core/Src/GameDataDef.h"

//С��ͼ����ʾ����ģʽ
enum MINIMAP_MODE
{
	MINIMAP_M_BRIEF_PIC = 0,			//����ͼ
	MINIMAP_M_BRIEF_PIC_BROWSE,			//����ͼ���/�Ŵ�ģʽ
	MINIMAP_M_WORLD_MAP,				//�����ͼ

	//MINIMAP_M_OBSTACLE,				//�ϰ�ͼģʽ
	MINIMAP_M_NONE,					    //����ʾ��ͼ

	MINIMAP_M_COUNT,
};

struct KUiSceneTimeInfo;

struct KMinimapTraffic
{
	POINT	TrafficPoint;
	POINT	TargetPoint;
	int		TargetIndex;
	KMinimapTraffic *previous;
	KMinimapTraffic *next;
};

class KUiMiniMap : public KWndImage
{
public:
	//----�������ͳһ�Ľӿں���----
	static KUiMiniMap*	OpenWindow();					//�򿪴��ڣ�����Ψһ��һ�������ʵ��
	static KUiMiniMap*	GetIfVisible();					//�������������ʾ���򷵻�ʵ��ָ��
	static void			CloseWindow();					//�رմ���

	static void			SetMode(MINIMAP_MODE eMode);	//������ʾ����ģʽ
	static void			LoadScheme(const char* pScheme);//������淽��
	static void			UpdateSceneTimeInfo(KUiSceneTimeInfo* pInfo);

	static void			HandleMapInfo(int nIndex);		//���õ�ǰ��ɫ����������
														//�ĵ���Ĵ������ƶ�ֵ(���)

	//���øô��ͼ�Բ���ʾ��ϸС��ͼ
	static void         SetIsShowMapDetail(int nbIsDetail);

	//��С��ͼ�����ȷ�ĵ�ͼ������
	static void			RestoreMapPos();

	static KUiMiniMap*	GetSelf() {return ms_pSelf;}

	void	Show();										//��ʾ����
	void	Hide();										//���ش���

private:
	KUiMiniMap();
	~KUiMiniMap() {}
	void	Initialize();
	void	PaintWindow();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	LoadScheme(KIniFile* pIni);		//������淽��
	void	SetStatus(bool bSmall);			//�л�״̬
	void    MapScroll(int nbScrollScene);	//��ͼ��
	void	StopScrollMap();				//ֹͣ�϶���ͼ
	void	MapMoveBack();					//��ͼ��ԭԭʼƫ��
	virtual void	Breathe();

	//���ܣ�С��ͼ�����Ӵ������Ͻ�Ϊ0�㣬�ѵ�ǰ�����������ת��Ϊ������С��ͼ�е���������
	//		����Ϊin out
	int     PosToPhotoPos(int *pX, int *pY);

	int		DoTransfer();					//��С��ͼʩչ���λ�Ӱ�󷨣������Ϊ���
											//�ڴ����ϵ�λ�ã�Ҫת��Ϊ��С��ͼ������
											//��λ��

	int		HandleCursor(int nX, int nY);	//��������ƶ���HOVER�������Ϊ���
											//�ڴ����ϵ�λ�ã�Ҫת��Ϊ��С��ͼ������
											//��λ��

	//ͨ��С��ͼ�������������С��ͼ�����Ӵ������Ͻ�Ϊ0�������
	//ֵΪ���أ�����Ϊin out
	int		GetCursorPosOnMap(int *pX, int *pY);

	void	LoadTrafficData();				//���ƹ���������ƶ�ֵ�������ϱ�
											//�õ���־���ڵ���������ƶ�ֵ
											//(���뽻ͨ����)

	void	ReleaseTrafficData();			//���ý�ͨ����

	void	SetMapFocus(int nX, int nY);	//����С��ͼ�Ľ���

	void	SetFlag(int nX, int nY);		//����С��

private:
	static KUiMiniMap*	ms_pSelf;
	static BOOL         m_bIsShowMapDetail; // �Ƿ���ʾ��ϸС��ͼ�����
	static int			m_nRealMapIndex;	// ����ɫ��С��ͼʹ�������λ�Ӱ
											// �󷨣������������ڵĵ����
											// �������ƶ���ֵ(Index���)

	static int			m_nCurrentMapIndex; // ��ǰ��־��������Ĵ�������ֵ(Index)
	static int			m_nbInTrafficMode;	// �Ƿ������С��ͼ�϶�ģʽ

public:
	enum enumMINIMAP_CTRL_MODE
	{
		enumMCM_SCROLL = 1,
		enumMCM_FLAG = 2,
		enumMCM_WAITING_TO_SET_BACK = 4,
	};

private:
	unsigned int	m_uShowElem;
	POINT			m_MapPos;
	SIZE			m_MapSize;
	KWndButton		m_SwitchBtn;
	KWndButton		m_BtnFlag;
	KWndText32		m_SceneName;
	KWndText32		m_SceneNameBG;
	KWndText32		m_ScenePos;
	KWndShadow		m_Shadow;
	KWndButton		m_WorldMapBtn;

	POINT           m_OldPos;
	POINT			m_FlagPos;
	unsigned int	m_uLastScrollTime;             // �����һ�ι�����ʱ��(��)
	int             m_nOffsetX;                    // ÿ��Loop����⵽���ϴ����λ�õ�ƫ��X
	int             m_nOffsetY;                    // ÿ��Loop����⵽���ϴ����λ�õ�ƫ��Y

	KLinkStruct<KMinimapTraffic>
					m_Traffic;					   // �������е�������������
												   // (С��ͼ��ͨ����)

	KMinimapTraffic
				   *m_pTrafficNow;				   // ָ��ǰ�����ָ�Ľ�ͨ���ݵ�ָ��

	KUiImageRef		m_FlagImage;				   // ���ӵ�ͼ������

	int				m_nScaleH, m_nScaleV;		   // ������������
	
	int				m_nDefaultCursor;			   // ������С��ͼǰ�����״̬

	int				m_nCtrlMode;				   // С��ͼ����һ��ʲô���Ĳ���ģʽ
												   // λ����
};

void	MapToggleStatus();
void	MapSetMode(MINIMAP_MODE eMode);	//������ʾ����ģʽ