////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KTgaBlockLoader.h
//  Version     : 1.0
//  Creator     : Hu Changyin
//  Create Date : 2005-12-19 15:34:53
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include "UiTex.h"

#ifndef _INCLUDE_KTGABLOCKLOADER_H_
#define _INCLUDE_KTGABLOCKLOADER_H_

////////////////////////////////////////////////////////////////////////////////

enum 
{
    LOAD_AS_APPEND = 0,
    LOAD_AS_REPLACE = 1,
	READ_TEXT = 1,
	NOT_READ_TEXT = 0,
	FIND_ICON_FILE = 1,
	NOT_FIND_ICON_FILE = 0,
};

class KTgaBlockLoader
{
protected:
    KTgaBlockLoader();
    ~KTgaBlockLoader();

public:
    static int Init();
    static int Exit();
    static KTgaBlockLoader& GetSelf();

	int SetUITexName(LPCTSTR pcszUITex);
	int SetTgaName(LPCTSTR pcszTga);
	int SetSTexName(LPCTSTR pcszSTex);
	int SetTextName(LPCTSTR pcszText);

	LPCTSTR GetUITexName();
	LPCTSTR GetTgaName();
	LPCTSTR GetSTexName();
	LPCTSTR GetTextName();

    int LoadTga(LPCTSTR pcszTgaFile);//����Tga
	//����UITex�ļ��������ĸ��ļ���<1 = ReadText :: 0 = notReadText> <1 = FindIcon :: 0 = notFindIcon> 
    int LoadUITex(
        LPCTSTR pcszUITex, int nLoadStyle, int nReadKind, 
        int nFindKind, int TgaInd, int nFrameNum, HWND hWnd
        );
	int ReadUITex(
        LPCTSTR pcszUITex, int nLoadStyle, int nFindKind, 
        int TgaInd, int nFrameNum
        );//��ȡUITex�ļ�
	int ReadText(LPCTSTR pcszUITex, int nLoadStyle);//��ȡTxt�ļ�
	int ReadIconText(LPCTSTR pcszText);//��ȡIcon�ļ�
	int ReadTextFromIcon(LPCTSTR pcszText, HWND hWnd);//����icon�ļ��е�UITex�ļ�����

	int LoadTgaToIcon(LPCTSTR pcszUITex, int TgaInd, int nFrameNum, int nLoadStyle);


public:
protected:
private:
    static KTgaBlockLoader *ms_pSelf; 
	std::vector<TXTFILEDATA> m_aNewIconTxtVector;//���Icom����Ϣ
	std::vector<TXTFILEDATA> m_aNewTgaTxtVector;//���UITexͬ��Txt�ļ�����Ϣ

    TCHAR *m_pszFileTitle[6];
	LPCTSTR m_pcszUITex;
	LPCTSTR m_pcszTga;
	LPCTSTR m_pcszSTex;
	LPCTSTR m_pcszText;
    KIconBlock *m_pIcon;
};



#endif //_INCLUDE_KTGABLOCKLOADER_H_
