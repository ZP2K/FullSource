#include "StdAfx.h"
#include "Functions.h"
#include "IEEditor.h"
#include "./3DEngine/InputProxy.h"

void InitKeyState(IEKG3DSceneEditorBase* pBase, HWND hWnd, CInputProxy* pInputProxy)
{
	_ASSERTE(pInputProxy);

	static PROXYKEYMAP KEYMAP[] =
	{	//           C  S  A  L  M  R  ?    Trigger     ID  Prev Ig Exclusive
		{'X'	   , 0, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN , 16,  0, 0, 1},	//ѹ�е���
		{'Z'	   , 0, 1, 0, 1, 0, 0, 0, WM_LBUTTONDOWN , 17,  0, 0, 1},	//����Ȧͬʱ����
		{'Z'	   , 0, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN , 18,  0, 0, 1},	//��Ȧ����

		{VK_MENU   , 0, 0, 1, 0, 1, 0, 0, WM_MOUSEMOVE,   1,  0, 0, 0},	//alt+MBT, ��ת�������ͷ
		{VK_CONTROL, 1, 0, 0, 0, 1, 0, 0, WM_MOUSEMOVE,   2,  0, 0, 0},	//ctrl+MBT, ���������ӽ�
		{VK_MBUTTON, 0, 0, 0, 0, 1, 0, 0, WM_MOUSEMOVE,   3,  0, 0, 0},	//MBT, PAN��ͼ
		{VK_MBUTTON, 0, 1, 0, 0, 1, 0, 0, WM_MOUSEMOVE,   4,  0, 0, 0},	//SHIFT+MBT, ��ѡ����Ϊ������ת��ͼ
		{VK_LBUTTON, 1, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN, 5,  0, 0, 0},	//CTRL��LBT������ǰ����

		{'W'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN    , 6,  0, 2, 0},	//ǰ�� ��������
		{VK_LBUTTON, 0, 0, 0, 1, 0, 1, 0, WM_MOUSEMOVE  , 6,  0, 0, 0},	//��ס���Ҽ����ţ���ֱ��
		{'S'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN    , 7,  0, 2, 0},	//���� ��������
		{'A'	   , 0, 0, 0, 1, 0, 0, 0, WM_KEYDOWN    , 8,  0, 1, 0},	//��ת 
		{'D'	   , 0, 0, 0, 1, 0, 0, 0, WM_KEYDOWN    , 9,  0, 1, 0},	//��ת 
		{'A'	   , 0, 0, 0, 0, 1, 0, 0, WM_KEYDOWN    , 8,  0, 1, 0},	//��ת 
		{'D'	   , 0, 0, 0, 0, 1, 0, 0, WM_KEYDOWN    , 9,  0, 1, 0},	//��ת 
		{'A'	   , 0, 0, 0, 0, 0, 1, 0, WM_KEYDOWN    ,10,  0, 1, 0},	//���� ���Լ���
		{'D'	   , 0, 0, 0, 0, 0, 1, 0, WM_KEYDOWN    ,11,  0, 1, 0},	//���� ���Լ���

		{VK_RBUTTON, 0, 0, 0, 0, 0, 1, 0, WM_MOUSEMOVE , 12,  0, 0, 0},	//��ס�Ҽ����ţ���ת�������
		{' '	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN   , 13,  0, 3, 0},	//������������
		{'A'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN   , 14,  0, 1, 0},	//��ת��ͬʱת�������
		{'D'	   , 0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN   , 15,  0, 1, 0},	//��ת

		{VK_LBUTTON, 0, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN, 19,  0, 0, 0},	//������£�������ѡ�񡢸ı���εȺܶද��
		{VK_SHIFT,   0, 1, 0, 1, 0, 0, 0, WM_LBUTTONDOWN, 20,  0, 0, 0},	//ƽ��

		{VK_ADD,	 0, 0, 0, 0, 0, 0, 0, WM_LBUTTONDOWN, 21,  0, 3, 0},	//����
		{VK_SUBTRACT,0, 0, 0, 0, 0, 0, 0, WM_KEYDOWN,     22,  0, 3, 0},	//����

		{VK_LBUTTON, 1, 0, 0, 1, 0, 0, 0, WM_LBUTTONDOWN, 23,  0, 0, 0},	//�ڵ�ǰѡ��Χ�У�������ѡ������
		{VK_LBUTTON, 0, 0, 1, 1, 0, 0, 0, WM_LBUTTONDOWN, 24,  0, 0, 0},	//�ڵ�ǰѡ��Χ�У��ټ���ѡ������

		{0		   , 0, 0, 0, 0, 0, 0, 0, 0             , 0,  0, 0, 0},	//����
		//           C  S  A  L  M  R  ?    Trigger     ID  Prev Ig Exclusive
	};

	pInputProxy->Init(pBase, CWnd::FromHandle(hWnd), KEYMAP);
}