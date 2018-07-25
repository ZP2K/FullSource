////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : UiTex.h
//  Version     : 1.0
//  Creator     : Hu Changyin
//  Create Date : 2005-12-17 13:45:25
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_UITEX_H_
#define _INCLUDE_UITEX_H_

////////////////////////////////////////////////////////////////////////////////

#include <vector>

////////////////////////////////////////////////////////////////////////////////

/*
UiTex�ļ�˵��:
����ͼ����Ϣ�ļ�
UiTex�ļ�ͷ
Frame������ ����Ϊ  nFrameCount * sizeof(UITEXFRAMEDATASTRUCTURE)
Animate������ ���Ȳ���  nFrameCount FrameIndex...
*/

struct UITEXFILEHEADER           //UiTex�ļ�ͷ
{
    DWORD   dwUiTexFlag;         //UiTex�ļ���־
    int     nWidth;              //��
    int     nHeight;             //��
    int     nFrameCount;         //֡��
    int     nAnimateCount;       //������
    DWORD   dwFileLen;           //�ļ�����
    char    szTgaFileName[32];   //Tga�ļ���
    char    szBuffer[32];        //����,Ҳ������ΪTga���ӳ�����
};

struct UITEXFRAMEDATASTRUCTURE   //Frame���ݿ�
{
    DWORD   dwInfo;              //Frame��Ϣ
    int     nLeft;               //������
    int     nTop;                //������
    int     nWidth;              //���
    int     nHeight;             //�߶�
};

struct TXTFILEDATA
{
	DWORD   dwInfo;					//Frame��Ϣ
	int		nFrameNum;				//UITex�ļ��ڶ������
	int		nTextNum;				//Icon�ļ��ڶ������
	int     nLeft;					//��
	int     nTop;					//��
	int     nWidth;					//��
	int     nHeight;				//��
	int		nNewLeft;				//icon�ļ����ź����
	int		nNewTop;				//icon�ļ����ź����
	char	szTxtFile[MAX_PATH];	//·��
};

#define MAX_TGA_BLOCK_NAME_LEN      32
#define KEEP_ORIGINAL_INDEX         0x00000001
#define HAS_AREADY_ANIMATE_GROUP    0x00000002
#define KEEP_POS_WHEN_AUTO_MAKE_UP  0x00000004  
#define HASE_BEEEN_SET_POS          0x00000008

class KTgaBlock
{
protected:
    KTgaBlock();
    ~KTgaBlock();

public:
    static KTgaBlock *CreateFromFile(LPCTSTR pcszTgaFileName);
    static KTgaBlock *CreateFromBuffer(
        BYTE *pbyBuffer, int nBufferSize, int nWidth, int nHeight, LPCTSTR pcszName
    );
    static KTgaBlock *CreateFromBuffer(
        BYTE *pbyBuffer, int nBufferSize, int nWidth, int nHeight, 
        int nOriginalLeft, int nOriginalTop, int nIsIndexChangeable, int nOriginalIndex, 
        int nPosSeted, LPCTSTR pcszName
    );
    virtual void Release();

    int SetName(LPCTSTR pcszName);
    int EnableChangeIndex();
    int DisableChangeIndex();
    int SetPosHasBeenSeted();
    int ClearPosHasBeenSeted();
    int SetLeft(int nLeft);
    int SetTop(int nTop);
    int SetWidth(int nWidth);
    int SetHeight(int nHeight);
    int SetOriginalIndex(int nIndex);
    int SetBuffer(BYTE *pbyBuffer, int nBufferSize);
    int SetInAnimateGroup();
    int ClearInAnimateGroup();
    int SetByRect(RECT &rcSet);

    int GetByRect(RECT &rcGet);
    int GetName(LPTSTR pszNameBuffer, int nNameBufferSize);
    LPCTSTR GetName();
    int IsIndexChangeable();
    int IsPosHasBeenSeted();
    int GetLeft();
    int GetTop();
    int GetWidth();
    int GetHeight();
    int GetOriginalIndex();
    BYTE *GetBufferPtr();
    int GetBufferSize();
    int IsAlreadyOnAnimateGroup();

    int PtInBlock(int nX, int nY);

	int CreateTexture();

	LPDIRECT3DTEXTURE9 GetTexture();
private:
    DWORD m_dwStyle;
    TCHAR m_szName[MAX_PATH];
    int m_nLeft;
    int m_nTop;
    int m_nWidth;
    int m_nHeight;
    int m_nOriginalIndex;

    BYTE *m_pbyBuffer;
    int m_nBufferSize;

	LPDIRECT3DTEXTURE9 m_pTextrue;
};

class KIconBlock
{
protected:
	KIconBlock();
	~KIconBlock();
public:
	static KIconBlock *CreateFromBuffer(
		int nIconID, LPCTSTR pcszFileName, int nFrame
	);
    static KIconBlock *CreateFromBuffer(
        BYTE *pbyBuffer, int nBufferSize, int nId, int nFrame,
        int nWidth, int nHeight, LPCTSTR pcszName, LPCTSTR pcszFileName
    );

	int SetTgaBlock(KTgaBlock *pTgaBlock);
	int SetName(LPCTSTR pcszUITexName);
    int SetFrameName(LPCTSTR pcszFrameName);
	int SetFile(int nFile);
	int SetFrame(int nFrame);
	int SetWidth(int nWidth);
	int SetHeight(int nHeight);
    int SetBitmap(Gdiplus::Bitmap *pBitmap);

	KTgaBlock *GetTgaBlock();
	int GetFrame();
	int GetFile();
	int GetWidth();
	int GetHeight();
    Gdiplus::Bitmap *GetBitmap();
	int GetName(OUT LPTSTR pszUITexName, int nUITexNameBufferSize);
    int GetFrameName(OUT LPTSTR pszFrameName, int nFrameNameBufferSize);

	virtual void Release();
protected:
private:
	KTgaBlock *m_pTgaBlock;
	TCHAR m_szUITexName[MAX_PATH];
    TCHAR m_szFrameName[MAX_PATH];
	int m_nFrame;
    int m_nFile;
	int m_nWidth;
	int m_nHeight;
    Gdiplus::Bitmap *m_pBitmap;
};

class KAnimateBlock
{
    typedef std::vector<_tstring> KAnimateFrameArray; 
protected:
    KAnimateBlock();
    ~KAnimateBlock();
public:
    static KAnimateBlock *Create(
        LPCTSTR pcszName, int nIsIndexChangeable, int nOriginalIndex
    );
    virtual void Release();

    int SetName(LPCTSTR pcszName);
    int SetOriginalIndex(int nIndex);
    int EnableChangeIndex();
    int DisableChangeIndex();

    int AppendFrame(LPCTSTR pcszName);
    int InsertFrame(LPCTSTR pcszName);
    int InsertFrame(int nIndex, LPCTSTR pcszName);
    int RemoveFrame(LPCTSTR pcszName);
    int RemoveFrame(int nIndex);
    int Clear();

    int MoveUp(int nIndex);
    int MoveDown(int nIndex);

    int GetAnimateFramCount();

    int SortByName();

    int GetName(OUT LPTSTR pszName, int nNameBufferSize);
    LPCTSTR GetName();
    int GetOriginalIndex();
    int IsIndexChangeable();

    int LookFromNameShouldFrameMyson(LPCTSTR pcszFrameName);
    int IsFrameMySon(LPCTSTR pcszFrameName);

    int IsMeShouldBeSave();
    int FormateSaveData();

    int ReplaceName(LPCTSTR pcszReplaced, LPCTSTR pcszReplace);

    int UpdataListView();
    
    LPCTSTR Get(int nIndex);
    int GetFrameCount();

private:
    DWORD m_dwStyle;
    TCHAR m_szName[MAX_PATH];
    int m_nOriginalIndex;
    KAnimateFrameArray m_aFrame;

};

#endif //_INCLUDE_UITEX_H_
