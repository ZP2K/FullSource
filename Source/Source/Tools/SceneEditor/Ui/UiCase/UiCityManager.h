/*******************************************************************************
File        : UiCityManager.h
Creator     : Fyt(Fan Zhanpeng)
create data : 02-11-2004(mm-dd-yyyy)
Description : �����й������
            : |��ʾʵ��|-|��ʾ�߼�|-|�����߼�|-|���ݹ���|    �ṹ��������
********************************************************************************/

#if !defined(AFX_UICITYMANAGER_H__571581A9_0256_4AF3_97BC_CF4F18DEFA47__INCLUDED_)
#define AFX_UICITYMANAGER_H__571581A9_0256_4AF3_97BC_CF4F18DEFA47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndscrollbar.h"
#include "../elem/wndbutton.h"
#include "../elem/wndtext.h"
#include "../elem/wndedit.h"

class KUiCityManager : public KWndImage
{
public:
	KUiCityManager();
	virtual ~KUiCityManager();

	static        KUiCityManager* OpenWindow();		//�򿪴���
	static        KUiCityManager* GetIfVisible();	//�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE);//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);	//������淽��


private: //�����׼���ڲ�������
	void          Initialize();						//��ʼ������

	//���������Ϣ�Ĵ�����
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);


private: //��׼�������ݶ���
	static KUiCityManager* ms_pSelf;


private: //���ݹ���
	char			m_szTongName[32];
	char			m_szMasterName[32];
	int				m_nTaxRate;				//ԭʼ˰��
	int				m_nPriceRate;			//ԭʼ���ָ��
	int				m_nCurrentTaxRate;		//��ǰ�趨��˰��
	int				m_nCurrentPriceRate;	//��ǰ�趨�����ָ��

	void			PrepareData();			//׼����������

public:  //���ݹ��ɲ��ֵĽӿ�
	HRESULT UpdateCurrentCityData(char *pszTongName, char *pszMasterName, int nTaxRate, int nPriceRate);


private: //�����߼�
	int				m_nMaxTax, m_nMinTax;
	int				m_nMaxPrice, m_nMinPrice;

	void			MinTax();
	void			MaxTax();
	void			MinPrice();
	void			MaxPrice();

	void			AdjustPrice(int nCount);
	void			AdjustTax(int nCount);


public:  //�����߼����ֵĽӿ�

	//���³������õĲ���
	HRESULT UpdateCurrentCityParam(int nMaxTax, int nMinTax, int nMaxPrice, int nMinPrice);


private: //��ʾ�߼�

	//���ܹ��������õĽ�������ģʽ
	void			TheCANNOTCHANGE_UI();
	//���Ը������õĽ�������ģʽ
	void			TheCANCHANGE_UI();
	///������ʾ
	void			UpdateView();

	//�Ƿ���ܸ��ĵĴ򿪷�ʽ
	bool			m_bIsCanChange;


private: //��ʾʵ��(D�ؼ��Լ�������)

private: //�����ҽ�
	KWndText32		m_textTongName;
	KWndText32		m_textMasterName;

	KWndEdit32		m_editTax;
	KWndEdit32		m_editPrice;

	KWndButton		m_BtnMinTax, m_BtnMaxTax;
	KWndButton		m_BtnMinPrice, m_BtnMaxPrice;

	KWndButton		m_BtnIncTax, m_BtnDecTax;
	KWndButton		m_BtnIncPrice, m_BtnDecPrice;

	KWndButton		m_BtnClose;
	KWndButton		m_BtnConfirm;
	KWndButton		m_BtnCancel;
	KWndButton		m_BtnHistory;
};

#endif // !defined(AFX_UICITYMANAGER_H__571581A9_0256_4AF3_97BC_CF4F18DEFA47__INCLUDED_)
