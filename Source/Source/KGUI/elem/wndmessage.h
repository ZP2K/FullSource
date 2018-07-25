/*****************************************************************************************
//	������Ϣ
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-9
------------------------------------------------------------------------------------------
*****************************************************************************************/
#ifndef UI_ELEM_K_WND_MESSAGE_H
#define UI_ELEM_K_WND_MESSAGE_H

#define	WND_M_BASE				0x500	//WM_USER + 0x100

//------------------------------------------------
//	����һ����Ϣ
#define WND_M_MOUSE_ENTER		WND_M_BASE + 6				//���ָ����봰�ڷ�Χ
#define	WND_M_MOUSE_LEAVE		WND_M_BASE + 7				//���ָ���뿪���ڷ�Χ
#define	WND_M_REFRESH_TIP		WND_M_BASE + 8				//���ָ���µĴ��ڸ���

#define	WND_M_SET_FOCUS			WND_M_BASE + 10				//�������뽹��
//(KWndWindow*)wParam = ԭ��ӵ�����뽹��Ĵ��ڵ�ָ��

#define	WND_M_KILL_FOCUS		WND_M_BASE + 11				//ʧȥ���뽹��
//(KWndWindow*)wParam = ��Ҫӵ�����뽹��Ĵ��ڵ�ָ��

#define	WND_M_CAPTURECHANGED	WND_M_BASE + 12				//ʧȥ�����ָ����Ϣ����

#define	WND_M_CHILD_CAPTURECHANGED	WND_M_BASE + 13			//�ƴ���ʧȥ�����ָ����Ϣ����

//------------------------------------------------
//	������Ϣ
#define WND_M_BUTTON_BASE			WND_M_BASE + 100
#define WND_N_BUTTON_CLICK			WND_M_BUTTON_BASE + 1	//�����ť����
//(KWndWindow*)wParam = ������İ�ť���ڵ�ָ��
//nParam = ����CheckBox���͵İ�ť���˲�����ʾ��ť�����Ƿ��ڰ���״̬

#define	WND_N_BUTTON_DOWN			WND_M_BUTTON_BASE + 2	//��ť�����²���
//(KWndWindow*)wParam = ������İ�ť���ڵ�ָ��

#define	WND_N_BUTTON_HOLD			WND_M_BUTTON_BASE + 4	//��ť��������ס����
//(KWndWindow*)wParam = ����ס�İ�ť���ڵ�ָ��

//------------------------------------------------
// Scroll Bar��Ϣ
#define WND_M_SCORLLBAR_BASE		WND_M_BASE + 200
#define WND_N_SCORLLBAR_POS_CHANGED	WND_M_SCORLLBAR_BASE + 1//����������λ�÷����仯
//(KWndWindow*)wParam = ������Ĺ��������ڵ�ָ��
//nParam = ����������λ�ù�������ֵ

//------------------------------------------------
// �ı��༭����Ϣ
#define	WND_M_EDIT_BASE				WND_M_BASE + 300

#define	WND_N_EDIT_SPECIAL_KEY_DOWN	WND_M_EDIT_BASE + 2		//�ı��༭���ڻ���ض��İ�������
//(KWndWindow*)wParam = �ı��༭�򴰿ڵ�ָ��
//nParam = ������visual key code
//�ض��������� �س�����Esc�����Ʊ��tab�������ϼ�ͷ�������¼�ͷ����ǰ��ҳ������ҳ��

#endif // UI_ELEM_K_WND_MESSAGE_H
