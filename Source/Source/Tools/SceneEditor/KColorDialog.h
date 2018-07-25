#pragma once
#include "resource.h"
#include <d3d9.h>
#include <bitset>

// KColorPickerDialog dialog


/************************************************************************/
/*
WM_COLORPOINTER_CHANGE ��Ϣ��WPARAM = COLOR�� LPARAM = ������Ϣ��CWnd�����hWnd
*/
/************************************************************************/
#define WM_COLORPOINTER_CHANGE WM_USER + 1
#define WM_COLORDIALOG_CLOSE WM_USER + 2
class KColorPickerButton;
class KColorPickerDialog : public CDialog
{
	DECLARE_DYNAMIC(KColorPickerDialog)
	friend KColorPickerButton;
public:
	//KColorPickerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~KColorPickerDialog();

// Dialog Data
	enum { IDD = IDD_COLOR_PICKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	enum enumChannel
	{
		ENUM_CHANNEL_BEGIN,
		ENUM_CHANNEL_RED   = 0x00ff0000,
		ENUM_CHANNEL_H = ENUM_CHANNEL_RED,
		ENUM_CHANNEL_GREEN = 0x0000ff00,
		ENUM_CHANNEL_S = ENUM_CHANNEL_GREEN,
		ENUM_CHANNEL_BLUE  = 0x000000ff,
		ENUM_CHANNEL_B = ENUM_CHANNEL_BLUE,
		ENUM_CHANNEL_END,
		ENUM_CHANNEL_FORCE_DWORD = 0xffffffff,
	};
	enum
	{
		IDC_CHANNEL_BEGIN = 0,
		IDC_CHANNEL_RED = 16,
		IDC_CHANNEL_GREEN = 8,
		IDC_CHANNEL_BLUE = 0,
		IDC_CHANNEL_ALPHA = 24,
	};
	enum
	{
		em_part_right_vernier = 1 << 0, //�ұ��α�
		em_part_center_color_pallete = 1 << 1,
		em_part_alpha_vernier = 1 << 3,
	};

	enum
	{
		em_color_pallete_size = 256,
		em_vernier_pallete_width = 1,
	};
private:
	//���ֲ���
	static const POINT s_ptVernier;//�α�Ĵ�С
	static const INT s_nSpan;//�α굽��ɫ��ľ���
	static const RECT s_rectColor1;//�����ɫ���λ��
	static const RECT s_rectColor2;//�ұ���ɫ���λ��
	//static const INT s_nWidth2;//�ұ���ɫ��Ŀ��
	static const RECT s_rectVernierRgn;//�α����Ӧ����
	//static const RECT m_rectAlpha;//Alpha����λ��
	//static const RECT m_rectAlphaRgn;//Alpha������Ӧ����
	static const RECT m_rectClearRgn[3];//������λ��,�ֱ����ұ߻�������ߣ��ұߺ�Alpha�������
	static const RECT m_rectSelectColor;//��ǰѡ����ɫ��λ��
	static const RECT m_rectOriginColor;//��ʼ��ɫ��λ��
	static const RECT m_rectSelAndOrg;	//ѡ��ͳ�ʼ��ɫ�������
	//////////////////////////////////////////////////////////////////////////
	//����ؼ�
	//CButton m_ctlChannel[3];
	//CEdit m_ctlEditChannel[4];
	//CStatic m_ctlAlphaStatic;
	//////////////////////////////////////////////////////////////////////////
	//bool m_bUseColorValue;

	DWORD m_dwWhichPartIsCursorIn;
	//BOOL m_bVernierSelect;//�Ƿ�ѡ�����ұߵ���ɫ����α�
	BOOL m_bAlphaEnable;//�Ƿ�Ҫ�޸�Alphaֵ
	//BOOL m_bColor1Select;//�Ƿ��������ɫ���а��������
	//BOOL m_bAlphaSelect;//�Ƿ�ѡ����Alpha���α�

	//////////////////////////////////////////////////////////////////////////
	//��������
	//CBitmap* m_pbmpColor1;//�����ɫ���Ӧ��λͼ
	CBitmap	m_bmpLeft;	//��ߴ���ɫ��������õ�λͼ

	//CBitmap* m_pbmpColor2;//�ұ���ɫ���Ӧ��λͼ
	CBitmap m_bmpRight;	//�ұ���ɫ���õ�λͼ
	
	//CBitmap* m_pBmpAlpha;//Alpha��Ӧ��λͼ
	//CBitmap m_bmpAlpha;
	
	const DWORD m_dwColor;//������ɫ��ֻ����SetColor����
	DWORD m_dwColorOld;	//û��SetColor֮ǰ����ɫ

	D3DCOLORVALUE* m_pColorValueBinded;	//�ⲿ�󶨵�ColorValue
	DWORD* m_pBindedColor;//�������ɫ�ĵ�ַ
	CSliderCtrl m_SliderAlpha;
	//DWORD* m_pColorBuffer1;
	//DWORD* m_pColorBuffer2;
	//DWORD* m_pAlphaBuffer;

	//BOOL m_bAlphaMoved;
	//BOOL m_bVernierMoved;

	
	//int m_nGridSize;	
	//BYTE m_byChannel[4];
	FLOAT m_HSV[3];//ֵ����360,1,1
	BOOL m_bHSVModel;
	enumChannel m_enumLockChannel;//��ǰ������ͨ��(���ұ���ɫ������޸ĵ�ͨ��)
	//////////////////////////////////////////////////////////////////////////

	
	DWORD m_dwColorOrg;//��ʼ��ɫ��ֵ
	HWND	m_hNotifyHWND;
public:
	void Init(DWORD* pColor, LPCTSTR strCaption, CWnd* pNotify = NULL ,BOOL bAlphaEnable = TRUE);
	void Init(D3DCOLORVALUE* pColor , LPCTSTR strCaption, CWnd* pNotify = NULL, BOOL bAlphaEnable = TRUE);
	DWORD GetCurrentColor() { return m_dwColor; }
	BOOL IsAlphaEnable()	{return m_bAlphaEnable;}
    HWND GetHwnd(){return m_hNotifyHWND;}
	VOID SetAlphaEnable(BOOL bRet);
	VOID UnInit(void);///�����ⲿǿ�Ƹ�λ���������ڷ���һ��Close��Ϣ

	KColorPickerDialog();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedRadioH();
	afx_msg void OnBnClickedRadioS();
	afx_msg void OnBnClickedRadioB();
	afx_msg void OnBnClickedRadioRed();
	afx_msg void OnBnClickedRadioGreen();
	afx_msg void OnBnClickedRadioBlue();
	afx_msg void OnEnChangeEditH();
	afx_msg void OnEnChangeEditS();
	afx_msg void OnEnChangeEditV();
	afx_msg void OnEnChangeEditR();
	afx_msg void OnEnChangeEditG();
	afx_msg void OnEnChangeEditB();
	afx_msg void OnEnChangeEditA();
	afx_msg void OnEnChangeEditARGB();
	afx_msg void OnEnChangeEditARGBHEX();
	afx_msg void OnEnKillfocusEditArgb();
	afx_msg void OnEnKillfocusEditArgbHEX();
	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
private:
	VOID SetColor(D3DCOLOR color, BOOL bModifyAlpha);
	VOID SetColor(FLOAT fH, FLOAT fS, FLOAT fV);

	void SetColorCore( D3DCOLOR color );
	//virtual BOOL OnInit();//��ʼ���ؼ�
	void DrawColor1(CDC* pDc,BYTE byRed, BYTE byGreen, BYTE byBlue, const RECT& rect);//��ʾ��ߵ���ɫ��, RGBģʽ
	void DrawColor2(CDC* pDc, BYTE byRed, BYTE byGreen , BYTE byBlue);//��ʾ�ұߵ���ɫ�� RGBģʽ
	void DrawColor1HSV(CDC* pDc, float h, float s, float v);
	void DrawColor2HSV(CDC* pDc, float h, float s, float v);
	//void DrawAlpha(CDC* pDc);
	void DrawSelectColor(CDC* pDc);//��ʾ��ǰѡ����ɫ
	void DrawOriginColor(CDC* pDc);//��ʾ��ʼ��ɫ
	virtual void ClearVernier(CDC* pDc);//����α�
	virtual void DrawVernier(CDC* pDc);//���ұߵ��α�
	static void RGB2HSV(DWORD rgb, float& h, float& s, float& v);
	static DWORD HSV2RGB(float h, float s, float v);//�����Alphaλһ����0
	//VOID UpdateBindedColor();
    VOID Init(DWORD dwColor, LPCTSTR strCaption, CWnd* pNotify, BOOL bAlphaEnable);
	VOID _Init(DWORD dwColor, LPCTSTR strCaption, CWnd* pNotify, BOOL bAlphaEnable);
	//void SetColorPointer(D3DCOLORVALUE* pNewValue, LPCSTR strCaption, CWnd* pNotify = NULL, BOOL bAlpha = TRUE);

	void DrawAll(CDC* pDC);	
	//BOOL m_bCreated;
	//BOOL m_bUpdateBmp1;
	//BOOL m_bUpdateBmp2;
	VOID UpdateBitmaps();

	void UpdateBitmapLeft();
	void UpdateBitmapRight();

	VOID UnBindAllColorPointers();	
};

extern KColorPickerDialog g_ColorPicker;
extern KColorPickerDialog* GetColorPickerDialog();

//<CTH Added>
/*
<@remark ��װ�ܹ��Զ�����KColorPickerDialog�����ݸ��İ�ť��ɫ�İ�ť>
//
1.��resource.h������IDD_COLOR_PICKER
2.���κδ��ڷŸ���ť����Owner-Draw������Ϊtrue
3.���໯�����ťΪKColorPickerButton��������������Ҳ���Ծ�������Dialogͷ�ļ�
�Ŀؼ�����������ĳ����ťΪKColorPickerButton��Ȼ����DDX�ϼ���DDX_Control
4.Ҫʹ���ⲿ��ɫ��ʼ����������Dialog��OnInitDialog��ʹ��SetBKColor������Ȼ��Ҫȡ��ʱ����GetBKColorȡ�ã�Ҳ������BindColor��UnBindColor����һ����ɫ���󶨵���ɫ���ڰ�ť���ʱ���Զ��䡣
//������ON_BN_DOUBLECLICKED����Ӧ�����¼���ON_BN_CLICKED���ڲ���ȡ�ˡ�

*/

class KColorPickerButton :
	public CButton
{
	DECLARE_DYNAMIC(KColorPickerButton)
public:
	KColorPickerButton(void);
	~KColorPickerButton(void);
private:
	//D3DCOLOR m_d3dc_ColorToChange;//���ڸ�Dialog�� // Last modify by Chen Tianhong: 2007-12-10 14:43:18//ȥ������m_d3dc_bkColor����
	enum
	{
		EM_IS_ALPHA_ENABLE = 0,
		//EM_IS_COLOR_CHANGED = 1,
		EM_IS_COLOR_PANEL_POPPING_DISABLE = 1,
	};
	D3DCOLOR m_d3dc_bkColor;	//���ڰ�ť����
	//����BindColorʱ��ʵʱ��ʾ��ɫ
	D3DCOLOR* m_pd3dc_BindedColor;
	D3DCOLORVALUE* m_pd3dc_BindedColor_v;
protected:
	std::tstring	m_tczColorPickerDialogCaption;//��ɫѡ�����ı�ǩ
	std::bitset<8> m_bitset;	//״̬�Ĵ�
public:
	void SetBKColor( D3DCOLOR CR_Color );
	D3DCOLOR GetBkColor( void ){return m_d3dc_bkColor;}
	//����ɫ֮��ÿ��������ɫ�ı��ʱ��Ҳ��ȥ�ı��޸ĵ���ɫ
	HRESULT BindColor( D3DCOLOR* ColorToBind );
    HRESULT BindColor(D3DCOLORVALUE* pColor);

	HRESULT UnBindColor( void );

	void	SetColorPickerDialogCaption( LPCTSTR szCaption );
	void	SetAlphaEnable( BOOL bIsEnable );
	BOOL	IsAlphaEnable(){return m_bitset.test(EM_IS_ALPHA_ENABLE); }
	
	VOID	EnableColorPanelPopping(BOOL bEnable);//���������Զ�Pop��ColorPanel
	BOOL	IsColorPanelPoppingEnabled(){return !m_bitset.test(EM_IS_COLOR_PANEL_POPPING_DISABLE);}
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS/*lpDrawItemStruct*/);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
	//afx_msg BOOL OnBnClicked( NMHDR * pNotifyStruct, LRESULT* result );
};
//</CTH Added>