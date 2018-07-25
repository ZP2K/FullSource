/*******************************************************************************
File        : UiPortrait.h
Creator     : Fyt(Fan Zhanpeng)
create data : 11-18-2003(mm-dd-yyyy)
Description : ͷ��ѡ������Լ���ش���
********************************************************************************/

#if !defined(AFX_UIPORTRAIT_H__A0ED4640_37B6_4731_991A_0E65248B991C__INCLUDED_)
#define AFX_UIPORTRAIT_H__A0ED4640_37B6_4731_991A_0E65248B991C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../elem/wndscrollbar.h"
#include "../elem/wndbutton.h"
#include "../elem/wndimage.h"
#include "../elem/uiimage.h"

class KUiPortrait : public KWndImage
{
public:  //�����׼�Ķ���ӿ�
	KUiPortrait();
	virtual ~KUiPortrait();

	static        KUiPortrait* OpenWindow();		//�򿪴���
	static        KUiPortrait* GetIfVisible();		//�������������ʾ���򷵻�ʵ��ָ��
	static void   CloseWindow(bool bDestory = TRUE);//�رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
	static void   LoadScheme(const char* pScheme);	//������淽��

private: //�����׼���ڲ�������
	void          Initialize();						//��ʼ������

													//���������Ϣ�Ĵ�����
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);

	void		  PaintWindow();					//�滭����ķ���

private: //������Ҫ�ľ�̬����
	static KUiPortrait*		ms_pSelf;

public:  //�����еĽṹ��ý�ٶ���
	typedef struct KPortraitParam
	{
		int nPortraitCount;
		int nPortraitWidth;
		int nPortraitHeight;
	} PORTRAIT_PARAM;

public:  //�����ľ�̬�ӿ�
	/*�����������ģ��λð�����Ӱ�������Ҫ�ҵ����ǵ����ڣ������Ƕ�װ�����Ժ���*/
	static int		InitializeProtrait();

	/*�Σ�������ʱ��������ĺܿ�ϧ�����е�һ�У�Ҳֻ�������˰ɡ�����*/
	static int		ReleasePortrait();

	/*�滭����һ��ɫ�ʵ�ͷ��:D,�����������Screen����,nbDarkness�����Ƿ�䰵*/
	static int		DrawPortrait(int nIndex, int nX, int nY, int nbDarkness = FALSE);

	/*��ȡ�λ�һ���Ф�����Դ֮��*/
	static char*	GetPortraitImagePath(int nPortraitIndex, char *szImage);
	
	/*��������ϼУ���¼������ĸ����λ�ģ����������Ե�֪������*/
	static int		GetPortraitCount(){return m_PortraitParam.nPortraitCount;};

private:	//������ľ�̬���ݳ�Ա�������ؼ���ͱ�����
	static PORTRAIT_PARAM	m_PortraitParam;
	static KUiImageRef	   *m_pImage;

private: //������ķ���
	int	   GetIndexAtPos(int nX, int nY);		//ͨ������ȡ����ѡ���ͷ�������
	int	   UpdateInterface();					//������ҵ�ͷ����½������ʾ

private:	//�ؼ������ݳ�Ա
	KWndButton		m_Confirm;	/*ȷ�ϵİ�ť*/
	KWndButton		m_Cancel;	/*ȡ���İ�ť*/
	KWndScrollBar	m_ScrollBar;/*������*/

private:	//�������ݳ�Ա
	int			m_nColumnCount;	/*ͷ���б�����*/
	int			m_nLineCount;	/*ͷ���б�����*/
	int			m_nSpacingH;	/*ͷ���м�ļ��*/
	int			m_nSpacingV;	/*ͷ���м�ļ��*/
	int			m_nFirstLine;	/*������ʾ�ĵ�һ������һ��*/
	int			m_nSelectIndex; /*ѡ���˵�ͷ�������*/
	POINT		m_LTPosition;	/*ͷ���б�����Ͻǣ�����ڽ��洰����˵*/
};

#endif // !defined(AFX_UIPORTRAIT_H__A0ED4640_37B6_4731_991A_0E65248B991C__INCLUDED_)
