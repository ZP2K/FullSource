#ifndef UI_DRIVER_HOTKEY_TABLE_H
#define UI_DRIVER_HOTKEY_TABLE_H

namespace UI
{
	class KHotKeyTable
	{
	public:
		enum { VK_LDBUTTON = 0x0100 };
		enum { VK_RDBUTTON = 0x0101 };
		enum { VK_MDBUTTON = 0x0102 };

		enum { HOTKEY_DESC_MAX_LEN = 32 };

	public:

		// @Param : dwHotkey ��Win32 Virtual Key Code 
		// @Return : ת��ʧ�ܣ�����_T("")
		LPCTSTR TranslateHotkeyToText(DWORD dwHotkey) const;

		// @Param : szHotkeyText ��ݼ�������ϣ�����С��HOTKEY_DESC_MAX_LEN����_T("Shift+Space")
		// @Return : ת��ʧ�ܣ����� 0
		DWORD TranslateTextToHotkey(LPCTSTR szHotkeyText) const;

	private:
		mutable TCHAR m_szHotkeyDesc[HOTKEY_DESC_MAX_LEN];
	};

};

#endif // UI_DRIVER_HOTKEY_TABLE_H

