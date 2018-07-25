// PSD2TGA.h: interface for the CPSD2TGA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSD2TGA_H__B4DDF54D_903D_409C_B335_7D319466F191__INCLUDED_)
#define AFX_PSD2TGA_H__B4DDF54D_903D_409C_B335_7D319466F191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPSD2TGA  
{
public:
	CPSD2TGA();
	virtual ~CPSD2TGA();
	HRESULT PSD2TGA(LPSTR pPSDFileNane,LPSTR pTGAFileName);
private:
	
#pragma pack(push,1)
	struct _TGAHeader 
	{
		unsigned char   Id ;
		unsigned char   ColourMapType ;
		unsigned char   ImageType ;
		unsigned short  ColourMapOrigin ;
		unsigned short  ColourMapLength ;
		unsigned char   ColourMapDepth ;
		unsigned short  x_origin ;
		unsigned short  y_origin ;
		unsigned short  width ;
		unsigned short  height ;
		unsigned char   bitsperpixel ;
		unsigned char   imagedescriptor ;
	};
#pragma pack(pop)

	void WriteChannel(BYTE Data, int Count, BYTE *Start, int Step);
	void Reverse(BYTE *data, int, int);
	HRESULT Save2Tga(LPSTR strFilename,
						   DWORD dwWidth,     // dwWidth��dwHeightΪ��ߣ�
						   DWORD dwHeight,
						   BYTE* pixels      // pixelsΪ���Ŀ��ָ�룬
						   );

	DWORD Read32(FILE* pFile);
	WORD Read16(FILE* pFile);
	HRESULT UnPackPSD(FILE *fp,          // fpΪPSD�ļ�ָ�룬
					  DWORD dwWidth,     // dwWidth��dwHeightΪ��ߣ�
					  DWORD dwHeight,
					  BYTE* pixels,      // pixelsΪ���Ŀ��ָ�룬
					  WORD ChannelCnt,   // ChannelCntΪͨ����
					  WORD Compression );

};

#endif // !defined(AFX_PSD2TGA_H__B4DDF54D_903D_409C_B335_7D319466F191__INCLUDED_)
