#ifndef UI_DRIVER_K_POINTER_H
#define UI_DRIVER_K_POINTER_H

#include "KG3DEngine/KG3DInterface.h"

namespace UI
{

#define HAND_OBJECT_CURSOR_WIDTH	32
#define HAND_OBJECT_CURSOR_HEIGHT	32

	enum CURSOR_STATE
	{
		CURSOR_BEGIN,

		CURSOR_NORMAL				= CURSOR_BEGIN,		//Ĭ��
		CURSOR_CAST,				//ʩ�ż���
		CURSOR_UNABLECAST,			//����ʩ�ż���
		CURSOR_TRAVEL,				//��վ
		CURSOR_UNABLETRAVEL,		//������վ
		CURSOR_SELL,				//����
		CURSOR_UNABLESELL,			//���ܳ���
		CURSOR_BUYBACK,				//�ع�
		CURSOR_UNABLEBUYBACK,		//���ܻع�
		CURSOR_REPAIRE,				//����
		CURSOR_UNABLEREPAIRE,		//��������
		CURSOR_ATTACK,				//����
		CURSOR_UNABLEATTACK,		//���ܹ���
		CURSOR_SPEAK,				//�Ի�
		CURSOR_UNABLESPEAK,			//���ܶԻ�
		CURSOR_LOOT,				//ʰȡ
		CURSOR_UNABLELOOT,			//����ʰȡ
		CURSOR_LOCK,				//��
		CURSOR_UNABLELOCK,			//������
		CURSOR_INSPECT,				//�쿴
		CURSOR_UNABLEINSPECT,		//���ܲ쿴
		CURSOR_SPLIT,				//���
		CURSOR_UNABLESPLIT,			//���ܲ��
		CURSOR_FLOWER,				//�ɻ�
		CURSOR_UNABLEFLOWER,		//���ܲɻ�
		CURSOR_MINE,				//�ɿ�
		CURSOR_UNABLEMINE,			//���ܲɿ�
		CURSOR_SEARCH,				//����
		CURSOR_UNABLESEARCH,		//��������
		CURSOR_QUEST,				//�������
		CURSOR_UNABLEQUEST,			//���ܼ��������
		CURSOR_READ,				//�Ķ�
		CURSOR_UNABLEREAD,			//�����Ķ�

		CURSOR_MARKPRICE,			//���

		CURSOR_TOP_BOTTOM,			//���Ҹı��С
		CURSOR_LEFT_RIGHT,			//���¸ı��С
		CURSOR_LEFTTOP_RIGHTBOTTOM,	//�������¸ı��С
		CURSOR_RIGHTTOP_LEFTBOTTOM, //�������¸ı��С
		CURSOR_MOVE,				//�ƶ�

        // 18��npc������ع��
        CURSOR_NPC_BAG,             
        CURSOR_NPC_BANK,
        CURSOR_NPC_BOOK,
        CURSOR_NPC_CRAFT,
        CURSOR_NPC_DRESS,
        CURSOR_NPC_FOOD,
        CURSOR_NPC_GROCERIES,
        CURSOR_NPC_GUARDER,
        CURSOR_NPC_HORSE,
        CURSOR_NPC_MAIL,
        CURSOR_NPC_MATERIAL,
        CURSOR_NPC_MEDICINE,
        CURSOR_NPC_REPUTATION,
        CURSOR_NPC_RING,
        CURSOR_NPC_SKILL,
        CURSOR_NPC_SPECIAL,
        CURSOR_NPC_STAGE,
        CURSOR_NPC_WEAPON,

		CURSOR_HAND_OBJECT,			//�����е�ͼ��ת��Ϊ�Ĺ��

		CURSOR_END,
		CURSOR_WINDOWS_DEAULT = CURSOR_END, //ϵͳ
	};

	class KPointer
	{
#pragma pack(push, 2)
		struct KUSERSINGLEFRAMECURFILEHEADER
		{
			WORD cdreserved;
			WORD cdtype;
			WORD cdcount;

			byte bwidth;
			byte bheight;
			byte bcolorcount;
			byte breserved;

			WORD wxhotspot;
			WORD wyhotspot;

			DWORD lbytesinres;
			DWORD dwimageoffset;

			DWORD      biSize;
			LONG       biWidth;
			LONG       biHeight;
			WORD       biPlanes;
			WORD       biBitCount;
			DWORD      biCompression;
			DWORD      biSizeImage;
			LONG       biXPelsPerMeter;
			LONG       biYPelsPerMeter;
			DWORD      biClrUsed;
			DWORD      biClrImportant;

			KUSERSINGLEFRAMECURFILEHEADER()
			{
				cdreserved = 0;
				cdtype = 2;
				cdcount = 1;

				bwidth = HAND_OBJECT_CURSOR_WIDTH;
				bheight = HAND_OBJECT_CURSOR_HEIGHT;
				bcolorcount = 0;
				breserved = 0;

				wxhotspot = 0;
				wyhotspot = 0;

				lbytesinres = HAND_OBJECT_CURSOR_WIDTH * HAND_OBJECT_CURSOR_HEIGHT * sizeof(DWORD) + 
					(HAND_OBJECT_CURSOR_WIDTH * HAND_OBJECT_CURSOR_HEIGHT) / 8;
				dwimageoffset = 22;

				biSize = sizeof(BITMAPINFOHEADER);
				biWidth = HAND_OBJECT_CURSOR_WIDTH;
				biHeight = HAND_OBJECT_CURSOR_HEIGHT * 2;
				biPlanes = 1;
				biBitCount = 32;
				biCompression = 0;
				biSizeImage = HAND_OBJECT_CURSOR_WIDTH * HAND_OBJECT_CURSOR_HEIGHT * sizeof(DWORD);
				biXPelsPerMeter = 0;
				biYPelsPerMeter = 0;
				biClrUsed = 0;
				biClrImportant = 0;
			}
		};
#pragma pack(pop)
	public:
		KPointer();
		~KPointer();

        bool Init();
		void Exit();

		bool ShowCursor(bool bShow);						//�������ָ�����ʾ״̬
		bool IsCursorVisible() const;

		bool SetCursorPos(int nX, int nY);					//�������ָ��λ��
		bool GetCursorPos(int& nX, int& nY) const;			//��ȡ���ָ��λ��

		int	SwitchCursor(int nIndex, int nIconID = -1);		//�л���ǰ���ָ��ͼ��
		int	GetCurrentCursorIndex() const;					//�õ���ǰ���ͼ�ε�����
		void RestoreCursor();	        					//���¼������
		void RestoreToWindowDefaultCursor();

		bool IsExistCursor(int nIndex) const;
		bool LoadCursor(int nIndex, LPCSTR szCursorFile);
		bool UnloadCursor(int nIndex); 
		int LoadAllCursor();
		bool UnloadAllCursor();

		bool LoadCursor(int nIndex, int nIconID);	        //��IconIDת�������ָ��

	private:
		int KUiLoadCursor(const char* pszImgFile, HCURSOR &rhRetCursor);
		int KUiLoadCursor(BYTE *pbyData, size_t uDataLen, BYTE *pbyXOR, size_t uXORLen, HCURSOR &rhRetCursor);

	private:
		HCURSOR m_aCursor[CURSOR_END];
		int	m_nCurrentCursor;
		
		KG3DUIImage2 m_ImageArrow;
		char m_szArrowPath[MAX_PATH];

		KUSERSINGLEFRAMECURFILEHEADER m_CurFileHeader;
		bool m_bVisible;
	};
}

#endif // UI_DRIVER_K_POINTER_H

