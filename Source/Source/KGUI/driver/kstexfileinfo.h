#ifndef UI_DRIVER_GRAPHIC_K_STEX_FILE_INFO_H
#define UI_DRIVER_GRAPHIC_K_STEX_FILE_INFO_H


namespace UI
{

	// ά����Ϸ��Ⱦ�����õ���TextureID�Լ�STex�ļ����Ĺ�ϵ
	class KSTexFileInfo
	{
	public:
		KSTexFileInfo();
		KSTexFileInfo(DWORD dwImageID, int nFilePathID);

		bool operator==(KSTexFileInfo const& rhs) const;

		void	SetImageID(DWORD dwImageID);
		void	SetFilePathID(int nFilePathID);

		DWORD	GetImageID() const;
		int		GetFilePathID() const;

	private:
		DWORD	m_dwImageID;
		int		m_nFilePathID;		// stex�ļ�·��
	};


	//--------------------------------------------------------------------------------------------------------------------
	inline void KSTexFileInfo::SetImageID(DWORD dwImageID)
	{
		m_dwImageID = dwImageID;
	}

	inline void KSTexFileInfo::SetFilePathID(int nFilePathID)
	{
		m_nFilePathID = nFilePathID;
	}

	inline DWORD KSTexFileInfo::GetImageID() const
	{
		return m_dwImageID;
	}

	inline int KSTexFileInfo::GetFilePathID() const
	{
		return m_nFilePathID;
	}

	inline bool KSTexFileInfo::operator==(KSTexFileInfo const& rhs) const
	{
		ASSERT(m_nFilePathID == rhs.m_nFilePathID ? m_dwImageID == rhs.m_dwImageID : true);
		return m_nFilePathID == rhs.m_nFilePathID;
	}

}


#endif // UI_DRIVER_GRAPHIC_K_STEX_FILE_INFO_H

