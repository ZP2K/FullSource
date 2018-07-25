//////////////////////////////////////////////////////////////////////////
//�����ˣ�        hu changyin(������)
//����ʱ��:       2005.6.13
//���ڶ�ȡ�Ͳ���32λtga�ļ���
//
//////////////////////////////////////////////////////////////////////////

#ifndef _K_TGA_H_
#define _K_TGA_H_

struct TGAFILEHEADER
{
    BYTE		IDLength;					// ID length
	BYTE		ColorMapType;				// Color map type
	BYTE		ImageType;					// Image type
	BYTE		ColorMapSpec[5];			// Color map specification
	WORD		X,Y;						// Image specification
	WORD		Width,Height;
	BYTE		PixelDep;
	BYTE		Desc;
};

class KTga32
{
private:
    TGAFILEHEADER m_TgaFileHeader;
    char          m_szTgaFireName[MAX_PATH];
    void          *m_pDataBuffer;
public:
    int         CreateFromFile(char *pTgaFileName, BOOL bCreateDataBuffer = FALSE);
    int         Create(int nWidth, int nHeight, int nDepth = 32); //nDepth,only 32 support now.
    int         Release();
    int         ReadARowToMemory(void * pTarget, int nRow); //nRow start from zero.
    int         ReadAllDataToMemory(void * pTarget, int x, int y, int nWidth); //x,y start from zero.
    int         ReadAllDataToSelfMemory();
    int         SaveDataBufferToTgaFile(char *pTgaFileName);
    int         SaveDataBufferToBmpFile(char *pBmpFileName);
	int			SaveDataBufferToCurFile(char *pBmpFileName);
    int         ProduceAlphaCodeToFile(char *pAlphaDataFileName, char *pAlphaCodeFileName);
    int         CopyRectBufferTo(BYTE *pDest, int nDestBufferSize, int x, int y, int nWidth, int nHeight);

    const char  *GetTgaFileName(){ return m_szTgaFireName;     };
    void        *GetDataBufferPointer(){ return m_pDataBuffer; };
	long		GetWidth()	{ return m_TgaFileHeader.Width;    };
	long		GetHeight() { return m_TgaFileHeader.Height;   };
    long        GetDepth()  { return m_TgaFileHeader.PixelDep; };
    KTga32();
    ~KTga32();
};

#endif


