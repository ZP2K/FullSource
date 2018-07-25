#ifndef UI_DRIVER_K_IMAGE_INFO_H
#define UI_DRIVER_K_IMAGE_INFO_H

#include "./kstexfileinfo.h"
#include "./kimagealpha.h"
#include "./kimageframe.h"
#include "./kimageframegroup.h"
#include "../common/tblock.h"

namespace UI
{

	class KImageInfo
	{
		friend class KImageInfoMgr;

	public:
		typedef TBlock<KImageFrame>			KImageFrameArray;
		typedef TBlock<KImageFrameGroup>	KImageFrameGroupArray;

        struct UITEXFILEHEADER           //UiTex�ļ�ͷ
        {
            DWORD   dwUiTexFlag;         //UiTex�ļ���־
            int     nWidth;              //��
            int     nHeight;             //��
            int     nFrameCount;         //֡��
            int     nAnimateCount;       //������
            DWORD   dwFileLen;           //�ļ�����
            char    szTgaFileName[32];   //STex�ļ���
            char    szBuffer[32];        //����,Ҳ������ΪszTextFile���ӳ�����
        };

        struct UITEXFRAMEDATASTRUCTURE   //Frame���ݿ�
        {
            DWORD   dwInfo;              //Frame��Ϣ
            int     nLeft;               //������
            int     nTop;                //������
            int     nWidth;              //���
            int     nHeight;             //�߶�
        };

	public:
		KImageInfo();
		virtual ~KImageInfo();

		bool operator==(KImageInfo const& rhs) const;

		SIZE	GetImageSize() const;
		int		GetFrameCount() const;
		SIZE	GetFrameSize(int nFrame) const;
		DWORD	GetImageID(int nFrame) const;
		RECT	GetImageRect(int nFrame) const;
		int		GetImageFrame(int nFrame) const;
		bool	IsImagePixelAlpha(int nFrame, int nX, int nY) const;

		int		GetFrameGroupCount() const;
		int		GetFrameCount(int nGroup) const;
		SIZE	GetFrameSize(int nGroup, int nGroupFrame) const;
		DWORD	GetImageID(int nGroup, int nGroupFrame) const;
		RECT	GetImageRect(int nGroup, int nGroupFrame) const;
		int		GetImageFrame(int nGroup, int nGroupFrame) const;
		bool	IsImagePixelAlpha(int nGroup, int nGroupFrame, int nX, int nY) const;
		int		GetFrameIndex(int nGroup, int nGroupFrame) const;

	private:
		bool	LoadFromFile(LPCSTR szFileName);
		bool	LoadUITexFile(LPCSTR szFileName);
		bool	LoadUIAlphaFile(LPCSTR szFileName);

		void	Unload();

	private:
		SIZE					m_sizeImage;

		KImageFrameArray		m_aFrame;
		KImageFrameGroupArray	m_aFrameGroup;

		KSTexFileInfo			m_STexFileInfo;
		KImageAlpha				m_Alpha;
	};

}

#endif // UI_DRIVER_K_IMAGE_INFO_H

