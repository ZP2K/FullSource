#ifndef UI_DRIVER_K_ANIMATE_IMAGE_H
#define UI_DRIVER_K_ANIMATE_IMAGE_H

namespace UI
{

	class KAnimateImage
	{
		friend class KAnimateImageMgr;

	public:
		KAnimateImage();

		bool operator==(KAnimateImage const& rhs) const;
		
		void SetImageInfoIndex(int nValue);
		void SetGroup(int nValue);
		void SetFrame(int nValue);
		void SetTotalFrame(int nValue);
		void SetInterval(int nValue);
		void SetLoopCount(int nValue);

		int GetImageInfoIndex() const;
		int GetGroup() const;
		int GetFrame() const;

		int IsFinished() const;

	private:
		bool Init();
		bool Activate(DWORD dwElapsed);

	private:
		// ��̬����
		int					m_nImageInfoIndex;	// KImageInfoMgr������
		int					m_nGroup;			// �������
		int					m_nTotalFrame;		// ����������֡��
		int					m_nLoopCount;		// ����ѭ�����Ŵ�������Ϊ-1��ʾһֱ����
		DWORD				m_dwInterval;		// �������ʱ�䣬��λms��

		// ��̬����
		DWORD				m_dwFlipTime;		// ��ǰ֡�л�������ʱ��
		int					m_nFrame;			// ��������֡����Ϊ-1��ʾ����
        int                 m_nCurrentCount;    // ��ǰ�����˶��ٴζ���
		int					m_nFinished;		// �Ƿ����
	};


	//--------------------------------------------------------------------------------------------------------------------
	inline void KAnimateImage::SetImageInfoIndex(int nValue)
	{
		m_nImageInfoIndex = nValue;
	}


	inline void KAnimateImage::SetGroup(int nValue)
	{
		m_nGroup = nValue;
        m_nCurrentCount = 0;
		m_nFinished = false;
	}


	inline void KAnimateImage::SetFrame(int nValue)
	{
		m_nFrame = nValue;
	}


	inline void KAnimateImage::SetTotalFrame(int nValue)
	{
		m_nTotalFrame = nValue;
	}


	inline void KAnimateImage::SetInterval(int nValue)
	{
		if (nValue < 0)
			m_dwInterval = UINT_MAX;
		else
			m_dwInterval = static_cast<DWORD>(nValue);
	}


	inline void KAnimateImage::SetLoopCount(int nValue)
	{
		m_nLoopCount = nValue;
	}


	inline int KAnimateImage::GetImageInfoIndex() const
	{
		return m_nImageInfoIndex;
	}


	inline int KAnimateImage::GetGroup() const
	{
		return m_nGroup;
	}


	inline int KAnimateImage::GetFrame() const
	{
		return m_nFrame;
	}


	inline int KAnimateImage::IsFinished() const
	{
		return m_nFinished;
	}

}

#endif // UI_DRIVER_K_ANIMATE_IMAGE_H

