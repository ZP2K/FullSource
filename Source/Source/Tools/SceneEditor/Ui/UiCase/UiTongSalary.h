/*******************************************************************************
File        : UiTongSalary.h
Creator     : Fyt(Fan Zhanpeng)
create data : 02-24-2004(mm-dd-yyyy)
Description : ��ᷢǮ����
********************************************************************************/

#pragma once
#include "..\elem\wndlabeledbutton.h"
//#include "..\elem\wndimage.h"
#include "..\elem\wndtext.h"
#include "..\elem\wndedit.h"

class KUiTongSalary : public KWndImage
{
public:
	KUiTongSalary(void);
	~KUiTongSalary(void);

public: //�������������������
	enum enumSALARY_TYPE
	{
		enumST_WHOLE,
		enumST_WHOLE_DIRECTOR,
		enumST_WHOLE_MANAGER,
		enumST_WHOLE_MEMBER,
		enumST_PER_DIRECTOR,
		enumST_PER_MANAGER,
		enumST_PER_MEMBER,
		enumST_NUM,
	};

	enum enumINPUT_SALARY
	{
		enumIS_DIRECTOR,
		enumIS_MANAGER,
		enumIS_MEMBER,
		enumIS_NUM,
	};

public: //�����׼�Ľӿ�
	static			KUiTongSalary* OpenWindow();		//�򿪴���
	static			KUiTongSalary* GetIfVisible();		//�������������ʾ���򷵻�ʵ��ָ��
	static void		CloseWindow(bool bDestory = TRUE);	//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void		LoadScheme(const char* pScheme);	//������淽��

private://�����׼���ڲ�������
	void			Initialize();						//��ʼ������

	//���������Ϣ�Ĵ�����
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

private: //��׼�������ݶ���
	static KUiTongSalary*	ms_pSelf;

public:  //�ӿ�
	//��������
	void			ClearCount();

	//�ⲿ���ݽ����Ĵ����򵥵�ͳ������������ok
	void			InjectData(KTongMemberItem *pItem, int nCount, int nType);

	/**
	 *@brief Ϊнˮ�����������д�����İ�����(���Ǹ������ж����ʽ�ȵ���)
	 */
	void			ArrangeDecree(KTongInfo sInfo);

	/**
	 *@brief ���½�����ʾ
	 */
	void			UpdateView();

private: //�ڲ�������
	void			Reset();							///���ü�����ֵ

	/**
	 *@brief ���벿��������ɵ�����
	 */
	void			InjectNotify();

	/**
	 *@brief ���������������нˮ����
	 */
	void			WorkoutSalary();

	/**
	 *@brief ��Ӧ�ַ��༭��ĸĶ�
	 */
	void			OnEditChange(KWndWindow *pWnd);

	/**
	 *@brief ��Ӧ��������/�����л���ť�ĵ��
	 */
	void			OnIsTotalButtonCheck();

	/**
	 *@brief ��Ӧȷ����ť����
	 */
	void			OnConfirm();

private: //������Դ��
	//������������
	int						m_aryCount[enumIS_NUM];

	//��¼���ֽ�������
	int						m_arySalary[enumST_NUM];

	//��¼����������ֵ�����
	int						m_aryInputSalary[enumIS_NUM];

	//����Ľ����������
	BOOL					m_arybIsTotal[enumIS_NUM];

	//Ҫ���в����İ�������
	KTongInfo				m_TongInfo;

	//�ַ������ݣ�����ʽ���
	char					m_szNotEnoughMoney[128];
	//�ַ������ݣ������������ʾ�ַ���
	char					m_szDescript[128];

private: //�ռ���Դ��
	KWndEdit32				m_editSalary[enumIS_NUM];
	KWndButton				m_btnIsTotal[enumIS_NUM];
	KWndText256				m_textDescription;

	KWndButton				m_btnConfirm;
	KWndButton				m_btnCancel;
};
