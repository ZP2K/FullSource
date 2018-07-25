#pragma once
#include "resource.h"
#include <d3d9.h>
#include <bitset>

// KColorPickerDialog dialog
#define GETRED(SRC)   ((SRC&0x00ff0000)>>16)
#define GETGREEN(SRC) ((SRC&0x0000ff00)>> 8)
#define GETBLUE(SRC)  (SRC&0x000000ff)
#define GETALPHA(SRC) (SRC>>24)

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

private:
	//////////////////////////////////////////////////////////////////////////
	//����ؼ�
	CButton m_ctlChannel[3];
	CEdit m_ctlEditChannel[4];
	CStatic m_ctlAlphaStatic;
	BOOL m_bAutoUpdate;
	//////////////////////////////////////////////////////////////////////////
	bool m_bUseColorValue;
	virtual BOOL OnInit();//��ʼ���ؼ�
	void DrawColor1(CDC* pDc,BYTE byRed, BYTE byGreen, BYTE byBlue, const RECT& rect, BOOL bUpdateBmp = TRUE);//��ʾ��ߵ���ɫ��, RGBģʽ
	void DrawColor2(CDC* pDc, BYTE byRed, BYTE byGreen , BYTE byBlue, BOOL bUpdateBmp = TRUE);//��ʾ�ұߵ���ɫ�� RGBģʽ
	void DrawColor1HSV(CDC* pDc, float h, float s, float v, BOOL bUpdateBmp = TRUE);
	void DrawColor2HSV(CDC* pDc, float h, float s, float v, BOOL bUpdateBmp = TRUE);
	void DrawAlpha(CDC* pDc);
	void DrawSelectColor(CDC* pDc);//��ʾ��ǰѡ����ɫ
	void DrawOriginColor(CDC* pDc);//��ʾ��ʼ��ɫ
	virtual void ClearVernier(CDC* pDc);//����α�
	virtual void DrawVernier(CDC* pDc);//���ұߵ��α�

	BOOL m_bVernierSelect;//�Ƿ�ѡ�����ұߵ���ɫ����α�
	BOOL m_bAlphaEnable;//�Ƿ�Ҫ�޸�Alphaֵ
	BOOL m_bColor1Select;//�Ƿ��������ɫ���а��������
	BOOL m_bAlphaSelect;//�Ƿ�ѡ���ˣ��������α�

	//////////////////////////////////////////////////////////////////////////
	//���ֲ���
	static int m_nWidth2;//�ұ���ɫ��Ŀ��
	RECT m_rectClearRgn[3];//������λ��
	RECT m_rectOriginColor;//��ʼ��ɫ��λ��
	RECT m_rectSelectColor;//��ǰѡ����ɫ��λ��
	RECT m_rectAlpha;//Alpha����λ��
	RECT m_rectAlphaRgn;//Alpha������Ӧ����
	static const POINT m_ptVernier;//�α�Ĵ�С
	static const RECT m_rectVernierRgn;//�α����Ӧ����
	static const int m_nSpan;//�α굽��ɫ��ľ���
	static const RECT m_rectColor1;//�����ɫ���λ��
	static const RECT m_rectColor2;//�ұ���ɫ���λ��
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//��������
	CBitmap* m_pbmpColor1;//�����ɫ���Ӧ��λͼ
	CBitmap* m_pbmpColor2;//�ұ���ɫ���Ӧ��λͼ
	CBitmap* m_pBmpAlpha;//Alpha��Ӧ��λͼ

	DWORD m_dwColor;//������ת����ɫ
	D3DCOLORVALUE* m_pColorValue;

	DWORD* m_pColorBuffer1;
	DWORD* m_pColorBuffer2;
	DWORD* m_pAlphaBuffer;

	BOOL m_bAlphaMoved;
	BOOL m_bVernierMoved;

	RECT m_rectSelAndOrg;
	int m_nGridSize;	
	BYTE m_byChannel[4];
	float m_HSV[3];
	BOOL m_bHSVModel;
	enumChannel m_enumLockChannel;//��ǰ������ͨ��(���ұ���ɫ������޸ĵ�ͨ��)
	//////////////////////////////////////////////////////////////////////////
public:
	DWORD* m_pColor;//�������ɫ�ĵ�ַ
	DWORD m_dwColorOrg;//��ʼ��ɫ��ֵ
	CWnd* m_pNotify;
public:
	void Init(DWORD* pColor, LPCSTR strCaption, CWnd* pNotify = NULL ,BOOL bAlphaEnable = TRUE);
	void Init(D3DCOLORVALUE* pColor , LPCSTR strCaption, CWnd* pNotify = NULL, BOOL bAlphaEnable = TRUE);
	KColorPickerDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual void PostNcDestroy();
	void RGB2HSV(DWORD rgb, float& h, float& s, float& v);
	DWORD HSV2RGB(float h, float s, float v);
	virtual void OnOK();
public:
	afx_msg void OnBnClickedRadioH();
	afx_msg void OnBnClickedRadioS();
	afx_msg void OnBnClickedRadioB();
	afx_msg void OnBnClickedRadioRed();
	afx_msg void OnBnClickedRadioGreen();
	afx_msg void OnBnClickedRadioBlue();
	//void SetColorPointer(D3DCOLORVALUE* pNewValue, LPCSTR strCaption, CWnd* pNotify = NULL, BOOL bAlpha = TRUE);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	VOID UnInit(void);///�����ⲿǿ�Ƹ�λ���������ڷ���һ��Close��Ϣ
	BOOL m_bCreated;
	BOOL m_bUpdateBmp1;
	BOOL m_bUpdateBmp2;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DWORD GetCurrentColor() { return *m_pColor; }
};

extern KColorPickerDialog g_ColorPicker;
extern KColorPickerDialog* GetColorPickerDialog();

//<CTH Added><@remark ��װ�ܹ��Զ�����KColorPickerDialog�����ݸ��İ�ť��ɫ�İ�ť>
//ֻ��Ҫ��h�ļ��Ŀؼ�����������ĳ����ťΪ�����࣬Ȼ��򿪰�ť��Owner-Draw����
//�Ϳ��Եõ��Ի��Ƶ�����
//Ҫʹ���ⲿ��ɫ��ʼ����������Dialog��OnInitDialog��ʹ��SetBKColor������Ȼ��Ҫȡ��ʱ����GetBKColorȡ�ã�Ҳ������BindColor��UnBindColor����һ����ɫ

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
	};
	D3DCOLOR m_d3dc_bkColor;	//���ڰ�ť����
	//����BindColorʱ��ʵʱ��ʾ��ɫ
	D3DCOLOR* m_pd3dc_BindedColor;
	D3DCOLORVALUE* m_pd3dc_BindedColor_v;
protected:
	std::string	m_tczColorPickerDialogCaption;//��ɫѡ�����ı�ǩ
	std::bitset<8> m_bitset;	//״̬�Ĵ�
public:
	void SetBKColor( D3DCOLOR CR_Color );
	D3DCOLOR GetBkColor( void ){return m_d3dc_bkColor;}
	//����ɫ֮��ÿ��������ɫ�ı��ʱ��Ҳ��ȥ�ı��޸ĵ���ɫ
	HRESULT BindColor( D3DCOLOR* ColorToBind )
	{
		UnBindColor();

		m_pd3dc_BindedColor =  ColorToBind;
		m_d3dc_bkColor      = *ColorToBind;
		return S_OK;
	}
	HRESULT BindColor(D3DCOLORVALUE* pColor)
	{
		UnBindColor();

		m_pd3dc_BindedColor_v = pColor;
		m_d3dc_bkColor = D3DCOLOR_COLORVALUE(pColor->r, pColor->g, pColor->b, pColor->a);
		return S_OK;
	}

	HRESULT UnBindColor( void )
	{
		m_pd3dc_BindedColor = NULL;
		m_pd3dc_BindedColor_v = NULL;
		return S_OK;
	}

	void	SetColorPickerDialogCaption( LPCSTR szCaption ){m_tczColorPickerDialogCaption = szCaption ? szCaption :_T("");}
	void	SetAlphaEnable( BOOL bIsEnable ){m_bitset.set(EM_IS_ALPHA_ENABLE, bIsEnable?true : false);}
	BOOL	IsAlphaEnable(){return m_bitset.test(EM_IS_ALPHA_ENABLE)?TRUE:FALSE; }
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS/*lpDrawItemStruct*/);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
};
//</CTH Added>