////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DTextureHelpers.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-6-22 16:38:46
//  Comment     : 
//
/*
���ڴ�����ͼ�����ֹ��Ĳ�����
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DTEXTUREHELPERS_H_
#define _INCLUDE_KG3DTEXTUREHELPERS_H_

////////////////////////////////////////////////////////////////////////////////
class KG3DTexture;

namespace KG3D_CUSTOM_HELPERS
{
	class TextureSampler	//�Զ������ͼ�����������ԷŴ���С��ͼ
	{
	private:
		typedef std::pair<int, int> PixelPos;
		typedef std::pair<int, int> TexSize;
		typedef std::pair<float, float> ScaleGroup;
		BOOL m_bResult;
	public:
		enum{TEX_MAX_SIZE = 4096,};
		TextureSampler( KG3DTexture* pTex	//Դ
			, DWORD* pdwResultBuffer		//Ŀ��
			, INT nResultBufferSize			//Ŀ��Buffer��С�������ܷ���nTargetWidth*nTargetHeight������
			, INT nSampleTimes				//��������
			, INT nTargetWidth				//Ŀ���
			, INT nTargetHeight				//Ŀ���
			, BOOL bSampleAlpha = FALSE);	//��KG3DTexture������pdwResultBuffer��ȥ
		BOOL SampleDWORDArrayEx(DWORD* pTexPixels	//Դ
			, INT nTexWidth					//Դ��
			, INT nTexHeight				//Դ��
			, DWORD* pdwResultBuffer		//Ŀ��
			, INT nResultBufferSize			//Ŀ��Buffer��С�������ܷ���nTargetWidth*nTargetHeight������
			, INT nSampleTimes
			, INT nTargetWidth
			, INT nTargetHeight
			, BOOL bSampleAlpha = FALSE);
		operator BOOL() const{return m_bResult;}
	private:
		/*
		inline PixelPos CalculateSamplePixelPos(PixelPos TargetPos, PixelPos SampleGroupPointPos, ScaleGroup SrcToDesXY, INT nSampleTimes)///fSampleGap
		{
		INT nBeginX = TargetPos.first * SrcToDesXY.first;
		INT nBeginY = TargetPos.second * SrcToDesXY.second;
		INT nPointX = nBeginX + 1*SrcToDesXY.first/nSampleTimes*SampleGroupPointPos.second;
		INT nPointY = nBeginY + 1*SrcToDesXY.second/nSampleTimes*SampleGroupPointPos.second;
		return PixelPos(nPointX, nPointY);
		}
		*/
		///�����������汾�������Ǽ��ٰ汾
		inline PixelPos CalculateSamplePixelPos(PixelPos SampleGroupTopLeftPos
			, PixelPos SampleGroupPointPos
			, ScaleGroup SampleGap)///SampleGap = SrcToDes/nSampleTimes
		{
			return PixelPos(SampleGroupTopLeftPos.first + (int)(1 * SampleGap.first * (float)SampleGroupPointPos.first)
				, SampleGroupTopLeftPos.second + (int)(1 * SampleGap.second * (float)SampleGroupPointPos.second));
		}
		inline PixelPos CalculateFirstPixelPosInSampleGroup(PixelPos TargetPos, ScaleGroup SrcToDesXY)
		{
			return PixelPos((int)((float)TargetPos.first*SrcToDesXY.first), (int)((float)TargetPos.second*SrcToDesXY.second));
		}
		inline D3DCOLOR GetColorAtPos(D3DCOLOR* pPixels, TexSize TSize, PixelPos Pos)
		{
			_ASSERTE(Pos.first >=0 && Pos.second >= 0 
				&&Pos.first < TSize.first && Pos.second < TSize.second);
			return pPixels[TSize.first * Pos.second + Pos.first];
		}
		inline void SetColorAtPos(D3DCOLOR* pDes, INT nDesSize, INT nPos, D3DCOLOR color)
		{
			_ASSERTE(nPos >= 0 && nPos < nDesSize);
			pDes[nPos] = color ;
		}
		
	};
	class TextureToGrayArray	//��RGBA��ɫ����䵽�Ҷ�����
	{
	private:
		BOOL m_bResult;
	public:
		TextureToGrayArray(DWORD* pdwSource, INT nArraySize, UCHAR* pResult, INT nResultArraySize);///�����������UCHAR��
		TextureToGrayArray(DWORD* pdwSource, INT nArraySize, DWORD* pResult, INT nResultArraySize, BOOL bAlphaTo255 = TRUE);///�����������ȫDWORD��
		operator BOOL() const{return m_bResult;}
	};

	HRESULT TextureTexToDWORDBuffer(KG3DTexture* pTexture, DWORD* pdwArray, INT nInputArraySize);	//����ͼ�е����ؿ�����DWORD������
	HRESULT TextureDWORDBufferToTex(KG3DTexture* pTexture, DWORD* pdwArray, INT nInputArraySize);	//��һ��DWORDѹ��Texture
	BOOL TextureGrayBufferToDWORDBuffer(UCHAR* pGray, INT nGrayArraySize, DWORD* pColor, INT nColorArraySize);	//�ѻҶ�������XRGB����
};

#endif //_INCLUDE_KG3DTEXTUREHELPERS_H_
