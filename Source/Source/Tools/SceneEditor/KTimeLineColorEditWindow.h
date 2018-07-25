#pragma once
//#include "KTimeLine.h"
//
//#define GETRED(SRC)   ((SRC&0x00ff0000)>>16)
//#define D3DCOLOR_GETGREEN(SRC) ((SRC&0x0000ff00)>> 8)
//#define D3DCOLOR_GETBLUE(SRC)        (SRC&0x000000ff)
//
//class KTimeLineColorEditBar: public CWnd
//{
//	DECLARE_DYNAMIC(KTimeLineColorEditBar)
//	enum enumDrawStyle
//	{
//		ENUM_DRAW_STYLE_BEGIN = 0,
//		ENUM_DRAW_STYLE_VERTICAL,
//		ENUM_DRAW_STYLE_HORIZONTAL,
//		ENUM_DRAW_STYLE_COUNT,
//		ENUM_DRAW_STYLE_FOCE_DWORD = 0xffffffff,
//	};
//
//	enum enumChannel
//	{
//		ENUM_CHANNEL_BEGIN = 0,
//		ENUM_CHANNEL_BLUE = ENUM_CHANNEL_BEGIN,
//		ENUM_CHANNEL_GREEN,
//		ENUM_CHANNEL_RED,
//		ENUM_CHANNEL_ALPHA,
//		ENUM_CHANNEL_COUNT,
//		ENUM_CHANNEL_FORCE_DWORD = 0xffffffff,
//	};
//public:
//	KTimeLineColorEditBar(KTimeLine<float, DWORD>* pTimeLine, CWnd* pParent, const RECT& rectWindow, const RECT& rectBar, const RECT* prectDrop = NULL);
//	~KTimeLineColorEditBar();
//	void DrawColorBar(CDC* pdc);
//
//	void SetWindowRect(const RECT& rectWindow) { m_rectWindow = rectWindow; }
//	RECT GetWindowRect() { return m_rectWindow;}
//
//	inline void SetColorBarRect(const RECT& rectBarRegion);
//	RECT GetColorBarRect() const { return m_rectBarRegion; }
//
//	void SetDropRegion(const RECT& rectDropRegion) { m_rectDropRegion.left = 0; 
//	m_rectDropRegion.top = 0;
//	m_rectDropRegion.bottom = abs(rectDropRegion.bottom - rectDropRegion.top);
//	m_rectDropRegion.right = abs(rectDropRegion.right - rectDropRegion.left);}
//	RECT GetDropRegion() { return m_rectDropRegion; }
//
//private:
//
//	BOOL Create(CWnd* pParent, const RECT& rectWindow, const RECT& rectBar, const RECT* prectDrop /*= NULL*/);//��������
//
//	//////////////////////////////////////////////////////////////////////////
//	//��������
//	float CoordinateToTime(int pt);//��Ļλ�õ�ʱ���ת��
//	int TimeToCoordinate(float fTime);//ʱ�䵽��Ļλ�õ�ת��
//	int IsBounceRegion(POINT& pt);//����Ƿ�ѡ�����α�
//	BOOL OnInit();//��ʼ���ؼ�
//	//////////////////////////////////////////////////////////////////////////
//
//	//////////////////////////////////////////////////////////////////////////
//	//��ͼ���
//	virtual void DrawButton(CDC* pdc, POINT& pt, DWORD crColor);//��Ⱦ�α�
//	void GetSystemColorFormat() { };
//	void RenderColorBar(CDC* pdc, enumDrawStyle DrawStyle);//��Ⱦ��ɫ��
//	void ClearRegion(CDC* pDc, RECT& rect);//�������
//	//////////////////////////////////////////////////////////////////////////
//
//	//////////////////////////////////////////////////////////////////////////
//	//���ݲ���
//	int AddKeyColor(float fTime, DWORD dwColor);//��fTime�ƶ�λ�����֡����ɫΪdwColor
//	int RemoveSelectedKeyColor();//ɾ����ǰѡ��֡
//	void ChangeColor();//�ı䵱ǰ֡����ɫ
//	//////////////////////////////////////////////////////////////////////////
//
//
//	//////////////////////////////////////////////////////////////////////////
//	//�ؼ�
//	CEdit m_ctlEditChannel[4];
//	CSliderCtrl m_ctlSliderChannel[4];
//	CStatic m_ctlStaticChannel[4];
//	//////////////////////////////////////////////////////////////////////////
//
//	KTimeLine<float, DWORD>* m_pTimeLine;//TimeLine������
//	vector<KTimeLine<float, DWORD>::FrameData> m_vecKeys;//����ԣ���̣����Ӧ������
//
//
//
//	//////////////////////////////////////////////////////////////////////////
//	//���ֲ���
//	const static int m_nGridSize;//�����ĸ��Ӵ�С
//	const static RECT m_rectButtonRgn;//��ť����Ӧ��Χ
//	const static int m_nSpan;//��ť�Ͳ�ɫ����ľ���
//	RECT m_rectClearRgn;//�α������
//	RECT m_rectWindow;//���ڵ�����
//	RECT m_rectDropRegion;//�α��ɾ������
//	RECT m_rectBarRegion;//��ɫ��������
//	RECT m_rectInsertRgn;//���Լ����¹ؼ��������
//	enumDrawStyle m_DrawStyle;//��ɫ������ʽ
//	//////////////////////////////////////////////////////////////////////////
//
//	//////////////////////////////////////////////////////////////////////////
//	//��������
//	CDC* m_pBackBuffer;//Ϊ�˷�ֹ��˸�������Ȼ������DC�ϣ� Ȼ��bitblt���ãУ�����ģ�
//	CBitmap* m_pBmpBar;//��ɫ����λͼBuffer
//	DWORD* m_pColorBarBuffer;//��ɫ���ģ£������
//	CBitmap* m_bmpBackBuffer;
//
//	BOOL m_bDirty;//��ʾ�Ƿ��н��иĶ�
//	BOOL m_bVerneirsMove;//��ʾ�Ƿ����α��ƶ���Ҫ�ػ�
//	int m_nSelectButton;//��ǰѡ�е��α꣬������ʾû��ѡ�е�
//	//////////////////////////////////////////////////////////////////////////
//
//public:
//	DECLARE_MESSAGE_MAP()
//	afx_msg void OnPaint();
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
//	virtual void PostNcDestroy();
//};
//
//class KColorBarDialog:public CDialog
//{
//public:
//	KColorBarDialog(KTimeLine<float, DWORD>* pTimeLine, CWnd* pParent = NULL);
//	~KColorBarDialog();
//	virtual BOOL OnInitDialog();
//	DECLARE_MESSAGE_MAP()
//protected:
//	virtual void OnCancel();
//	virtual void OnOK();
//private:
//	KTimeLine<float, DWORD>* m_pTimeLine;
//	KTimeLineColorEditBar* m_pColorBar;
//protected:
//	virtual void PostNcDestroy();
//};
