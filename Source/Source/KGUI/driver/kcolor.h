#ifndef UI_DRIVER_K_COLOR_H
#define UI_DRIVER_K_COLOR_H

namespace UI
{
	DWORD StringToColor(LPCSTR szString);	//���ַ�����ʾ����ɫ��ϢתΪ��ֵ��ʾ
	LPCSTR ColorToString(DWORD crColor);	//����ֵ��ʾ����ɫ��ϢתΪ�ַ�����ʾ
    DWORD AddAlpha(DWORD dwColor, int nAlpha);

}

#endif // UI_DRIVER_K_COLOR_H

