#ifndef UI_DRIVER_GRAPHIC_K_IMAGE_GROUP_H
#define UI_DRIVER_GRAPHIC_K_IMAGE_GROUP_H

#include "../common/tblock.h"

namespace UI
{

	// ���KImageFrame��ɵļ��ϣ����ڶ������ţ������ͼ��
	class KImageFrameGroup
	{
	private:
		typedef TBlock<int>	KImageFrameIndexArray;

	public:
		void SetFrameCount(int nFrameCount);
		void SetFrameIndex(int nGroupFrame, int nFrameIndex);

		int	GetFrameCount() const;
		int	GetFrameIndex(int nGroupFrame) const;

	private:
		KImageFrameIndexArray m_aFrameIndex;
	};


	//--------------------------------------------------------------------------------------------------------------------
	inline void KImageFrameGroup::SetFrameCount(int nFrameCount)
	{
		ASSERT(nFrameCount > 0);
		m_aFrameIndex.Alloc(nFrameCount);
	}


	inline void KImageFrameGroup::SetFrameIndex(int nGroupFrame, int nFrameIndex)
	{
		ASSERT(nGroupFrame >= 0 && nGroupFrame < GetFrameCount());
		ASSERT(nFrameIndex >= 0);
		m_aFrameIndex[nGroupFrame] = nFrameIndex;
	}


	inline int KImageFrameGroup::GetFrameCount() const
	{
		return m_aFrameIndex.GetSize();
	}


	inline int KImageFrameGroup::GetFrameIndex(int nGroupFrame) const
	{
		ASSERT(nGroupFrame >= 0 && nGroupFrame < GetFrameCount());
		return m_aFrameIndex[nGroupFrame];
	}

}


#endif // UI_DRIVER_GRAPHIC_K_IMAGE_GROUP_H

