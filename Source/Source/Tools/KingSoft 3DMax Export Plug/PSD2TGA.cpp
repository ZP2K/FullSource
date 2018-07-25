#include "header.h"
#include "asciiexp.h"
#include "PSD2TGA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

#define  MAX_PSD_CHANNELS 24
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPSD2TGA::CPSD2TGA()
{

}

CPSD2TGA::~CPSD2TGA()
{

} 

//DEL HRESULT CPSD2TGA::LoadPSD(LPSTR strFilename)
//DEL {
//DEL 	DWORD dwWidth, dwHeight; // ���
//DEL 	//long lSurfWidth = m_Rect.right - m_Rect.left;
//DEL 	//long lSurfHeight = m_Rect.bottom - m_Rect.top;
//DEL 	WORD CompressionType; // ѹ������
//DEL 	//HDC hDC;
//DEL 	FILE *fpPSD;
//DEL 	WORD ChannelCount; // ͨ����
//DEL 	BYTE* m_pbAlphaMask = NULL;
//DEL 	
//DEL 	// ��PSD�ļ�
//DEL 	if ( ( fpPSD = fopen ( strFilename, "rb" ) ) == NULL )
//DEL 	{
//DEL 		MessageBox(NULL, "����", "�ļ�������", MB_OK);
//DEL 		return E_FAIL;
//DEL 	}
//DEL 	// ͷ�ĸ��ֽ�Ϊ"8BPS"
//DEL 	char signature[5];
//DEL 	signature[0] = fgetc( fpPSD );
//DEL 	signature[1] = fgetc( fpPSD );
//DEL 	signature[2] = fgetc( fpPSD );
//DEL 	signature[3] = fgetc( fpPSD );
//DEL 	signature[4] = '\0';
//DEL 	if ( strcmp( signature,"8BPS" ) != 0 )
//DEL 	{
//DEL 		return E_FAIL;
//DEL 	}
//DEL 	
//DEL 	// �汾����Ϊ1
//DEL 	if ( Read16( fpPSD ) != 1 ) 
//DEL 	{
//DEL 		return E_FAIL;
//DEL 	}
//DEL 	
//DEL 	// ����һЩ���� (����0)
//DEL 	Read32( fpPSD );
//DEL 	Read16( fpPSD );
//DEL 	
//DEL 	// ��ȡͨ����
//DEL 	ChannelCount = Read16( fpPSD );
//DEL 	
//DEL 	// ȷ��������һ��ͨ��
//DEL 	if ( ( ChannelCount < 0 ) || ( ChannelCount > MAX_PSD_CHANNELS ) ) 
//DEL 	{
//DEL 		return E_FAIL;
//DEL 	}
//DEL 	
//DEL 	// �����͸�
//DEL 	dwHeight = Read32( fpPSD );
//DEL 	dwWidth = Read32( fpPSD );
//DEL 	
//DEL 	// ֻ����8λͨ��
//DEL 	if ( Read16( fpPSD ) != 8 ) 
//DEL 	{
//DEL 		return E_FAIL;
//DEL 	}
//DEL 	
//DEL 	// ȷ��ģʽΪRGB.
//DEL 	// ����ֵ��
//DEL 	// 0: λͼ
//DEL 	// 1: �ҽ�
//DEL 	// 2: ����
//DEL 	// 3: RGB
//DEL 	// 4: CMYK
//DEL 	// 7: Multichannel
//DEL 	// 8: Duotone
//DEL 	// 9: Lab
//DEL 	if ( Read16( fpPSD ) != 3 )
//DEL 	{
//DEL 		return E_FAIL;
//DEL 	}
//DEL 
//DEL 		
//DEL 	// �������ݣ����ɫ�壩
//DEL 	int ModeDataCount = Read32( fpPSD );
//DEL 	if ( ModeDataCount )
//DEL 		fseek( fpPSD, ModeDataCount, SEEK_CUR );
//DEL 	
//DEL 	// �������ݣ��磺pen tool paths, etc��
//DEL 	int ResourceDataCount = Read32( fpPSD );
//DEL 	if ( ResourceDataCount )
//DEL 		fseek( fpPSD, ResourceDataCount, SEEK_CUR );
//DEL 	
//DEL 	// ����layer and mask����
//DEL 	int ReservedDataCount = Read32( fpPSD );
//DEL 	if ( ReservedDataCount )
//DEL 		fseek( fpPSD, ReservedDataCount, SEEK_CUR );
//DEL 	
//DEL 	// 0: ��ѹ��
//DEL 	// 1: RLEѹ��
//DEL 	CompressionType = Read16( fpPSD );
//DEL 	if ( CompressionType > 1 )
//DEL 	{
//DEL 		return E_FAIL;
//DEL 	}
//DEL 	int length ;
//DEL 	length = ftell(fpPSD);
//DEL 	fseek(fpPSD, 0, SEEK_END);
//DEL 	length = ftell(fpPSD) - length;
//DEL 	fseek(fpPSD, -length, SEEK_END);
//DEL 	
//DEL 	
//DEL 	BYTE* PSDPixels = new BYTE[ ( dwHeight * dwWidth ) * 4 ];
//DEL 	
//DEL 	// �������
//DEL 	UnPackPSD( fpPSD, dwWidth, dwHeight, PSDPixels, ChannelCount, CompressionType );
//DEL 	fclose( fpPSD );
//DEL 	
//DEL 	Reverse(PSDPixels,  dwWidth,dwHeight);
//DEL 	Save2Tga("Kill.Tga",dwWidth,dwHeight,PSDPixels);
//DEL 	SAFE_DELETE_ARRAY( PSDPixels );
//DEL 	
//DEL 	return S_OK;
//DEL }

HRESULT CPSD2TGA::UnPackPSD(FILE *fp,          // fpΪPSD�ļ�ָ�룬
							DWORD dwWidth,     // dwWidth��dwHeightΪ��ߣ�
							DWORD dwHeight,
							BYTE* pixels,      // pixelsΪ���Ŀ��ָ�룬
							WORD ChannelCnt,   // ChannelCntΪͨ����
							WORD Compression ) // Compressionλѹ�����͡� ��������������������������������
{
	int Default[4] = { 0, 0, 0, 255 };
	int chn[4] = { 2, 1, 0, 3};
	int PixelCount = dwWidth * dwHeight;
	BYTE * Channels[4];
	char Header;
	BYTE Pixel;
	unsigned int Count;
	int Length = 0;
	Channels[0] = pixels + 2;
	Channels[1] = pixels + 1;
	Channels[2] = pixels ;
	Channels[3] = pixels + 3;
	if( ChannelCnt > 4)
	{
		ChannelCnt = 4;
		MessageBox(NULL, "��ͼͨ������4��", "Warning", MB_OK);
	}
	if ( Compression ) 
	{
		fseek( fp, dwHeight * ChannelCnt * 2, SEEK_CUR );

		for (int cn = 0; cn < ChannelCnt; cn ++)
		{
			unsigned int i = 0 ;
			while ( i < dwHeight * dwWidth)
			{
				Header = fgetc(fp);
				Pixel = fgetc(fp);
				if ( Header == -128)
					continue;
				else if ( Header < 0 )
				{
					for (Count = -Header + 1, i += Count; Count >0; Count --)
					{
						*(Channels[cn]) = Pixel;
						Channels[cn] += 4;
					}
				}
				else
				{
					*(Channels[cn]) = Pixel;
					Channels[cn] += 4;
					for (Count = Header , i += Count + 1; Count > 0 ; Count --)
					{
						Pixel = fgetc(fp);
						*(Channels[cn]) = Pixel;
						Channels[cn] += 4;
					}
				}
			}
			
		}
		if(ChannelCnt<=3)
		{
			int i =0;
			while ( i < dwHeight * dwWidth)
			{
				(*Channels[3]) = 255;
				Channels[3] += 4;
				i++;
			}
		}
	}	
	else//û��ѹ���Ĵ���
	{
		for ( int c=0; c < 4; c++ )
		{
			int channel = chn[c];
			if ( channel > ChannelCnt ) 
			{
				for( int pn = 0; pn < PixelCount; pn++ ) 
				{
					pixels[ ( pn * 4 ) + channel ] = Default[ channel ];
				}
			}
			else 
			{
				for( int n = 0; n < PixelCount; n++ ) 
				{
					pixels[ ( n * 4 ) + channel ] = fgetc( fp );
				}
			}
		}
	}
	return S_OK;
}

WORD CPSD2TGA::Read16(FILE *pFile)
{
	WORD A = 0;
	WORD B = 0;
	A = fgetc(pFile);
	B = fgetc(pFile);
	WORD S = (A<<8)|B;
	return S;
}

DWORD CPSD2TGA::Read32(FILE *pFile)
{
	DWORD A,B;
	A = Read16(pFile);
	B = Read16(pFile);
	DWORD S =(A<<16)|B;
	return S;
}

HRESULT CPSD2TGA::Save2Tga(LPSTR strFilename,
						   DWORD dwWidth,     // dwWidth��dwHeightΪ��ߣ�
						   DWORD dwHeight,
						   BYTE* pixels      // pixelsΪ���Ŀ��ָ�룬
						   )
{
	_TGAHeader Head;
	ZeroMemory(&Head,sizeof(_TGAHeader));
	Head.Id =0;
	Head.ColourMapType = 0;
	Head.ImageType = 2;
	Head.ColourMapOrigin = 0;
	Head.ColourMapLength = 0;
	Head.ColourMapType   = 0;
	Head.x_origin = 0;
	Head.y_origin = 0;
	Head.width  = (unsigned short) dwWidth;
	Head.height = (unsigned short) dwHeight;
	Head.bitsperpixel = 32;
	Head.imagedescriptor = 8;
	FILE* pFile = fopen(strFilename,"wb");
	if(!pFile)
		return E_FAIL;
	fwrite(&Head,sizeof(_TGAHeader),1,pFile);
	fwrite(pixels,1,dwWidth*dwHeight*4,pFile);
	fclose(pFile);

	return S_OK;

}



void CPSD2TGA::Reverse(BYTE *data, int dwWidth, int dwHeight)
{
	BYTE temp ;
	int iHeight = dwHeight >> 1;
	int iWidth = dwWidth << 2;
	for (int i = 0 ; i < iHeight ; i ++)
		for ( int j = 0 ; j < iWidth; j++)
		{
			temp = *(data + i * iWidth  + j);
			*(data + i * iWidth  + j) = *(data + (dwHeight -i - 1) * iWidth + j);
			*(data + (dwHeight -i - 1) * iWidth  + j) = temp;
			
		}
}

void CPSD2TGA::WriteChannel(BYTE Data, int Count, BYTE *Start, int Step)
{
	for ( int i =0; i < Count; i ++)
	{
		*Start = Data;
		Start += Step;
	}

}

HRESULT CPSD2TGA::PSD2TGA(LPSTR pPSDFileNane, LPSTR pTGAFileName)
{
	DWORD dwWidth, dwHeight; // ���
	//long lSurfWidth = m_Rect.right - m_Rect.left;
	//long lSurfHeight = m_Rect.bottom - m_Rect.top;
	WORD CompressionType; // ѹ������
	//HDC hDC;
	FILE *fpPSD;
	WORD ChannelCount; // ͨ����
	BYTE* m_pbAlphaMask = NULL;
	
	// ��PSD�ļ�
	if ( ( fpPSD = fopen ( pPSDFileNane, "rb" ) ) == NULL )
	{
		TCHAR ErrorInfo[1024];
		sprintf(ErrorInfo, "�ļ�%s������", pPSDFileNane );
		MessageBox(NULL, ErrorInfo, "����", MB_OK);
		return E_FAIL;
	}
	// ͷ�ĸ��ֽ�Ϊ"8BPS"
	char signature[5];
	signature[0] = fgetc( fpPSD );
	signature[1] = fgetc( fpPSD );
	signature[2] = fgetc( fpPSD );
	signature[3] = fgetc( fpPSD );
	signature[4] = '\0';
	if ( strcmp( signature,"8BPS" ) != 0 )
	{
		return E_FAIL;
	}
	
	// �汾����Ϊ1
	if ( Read16( fpPSD ) != 1 ) 
	{
		return E_FAIL;
	}
	
	// ����һЩ���� (����0)
	Read32( fpPSD );
	Read16( fpPSD );
	
	// ��ȡͨ����
	ChannelCount = Read16( fpPSD );
	
	// ȷ��������һ��ͨ��
	if ( ( ChannelCount < 0 ) || ( ChannelCount > MAX_PSD_CHANNELS ) ) 
	{
		return E_FAIL;
	}
	
	// �����͸�
	dwHeight = Read32( fpPSD );
	dwWidth = Read32( fpPSD );
	
	// ֻ����8λͨ��
	if ( Read16( fpPSD ) != 8 ) 
	{
		return E_FAIL;
	}
	
	// ȷ��ģʽΪRGB.
	// ����ֵ��
	// 0: λͼ
	// 1: �ҽ�
	// 2: ����
	// 3: RGB
	// 4: CMYK
	// 7: Multichannel
	// 8: Duotone
	// 9: Lab
	if ( Read16( fpPSD ) != 3 )
	{
		return E_FAIL;
	}

		
	// �������ݣ����ɫ�壩
	int ModeDataCount = Read32( fpPSD );
	if ( ModeDataCount )
		fseek( fpPSD, ModeDataCount, SEEK_CUR );
	
	// �������ݣ��磺pen tool paths, etc��
	int ResourceDataCount = Read32( fpPSD );
	if ( ResourceDataCount )
		fseek( fpPSD, ResourceDataCount, SEEK_CUR );
	
	// ����layer and mask����
	int ReservedDataCount = Read32( fpPSD );
	if ( ReservedDataCount )
		fseek( fpPSD, ReservedDataCount, SEEK_CUR );
	
	// 0: ��ѹ��
	// 1: RLEѹ��
	CompressionType = Read16( fpPSD );
	if ( CompressionType > 1 )
	{
		return E_FAIL;
	}
	int length ;
	length = ftell(fpPSD);
	fseek(fpPSD, 0, SEEK_END);
	length = ftell(fpPSD) - length;
	fseek(fpPSD, -length, SEEK_END);
	
	
	BYTE* PSDPixels = new BYTE[ ( dwHeight * dwWidth ) * 4 ];
	
	// �������
	UnPackPSD( fpPSD, dwWidth, dwHeight, PSDPixels, ChannelCount, CompressionType );
	fclose( fpPSD );
	
	Reverse(PSDPixels,  dwWidth,dwHeight);
	Save2Tga(pTGAFileName,dwWidth,dwHeight,PSDPixels);
	SAFE_DELETE_ARRAY(PSDPixels);
	
	return S_OK;
}
