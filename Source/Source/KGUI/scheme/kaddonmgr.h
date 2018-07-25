////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : kaddonmgr.h
//  Version     : 1.0
//  Creator     : Hu Changyin
//  Create Date : 2006-11-9 9:28:23
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KADDONMGR_H_
#define _INCLUDE_KADDONMGR_H_

////////////////////////////////////////////////////////////////////////////////

namespace UI
{

	class KAddOnMgr
	{
	public:
		struct KADDONINFO
		{
			std::string strID;				//�ļ���������Ϊ�����Ψһ����
            std::string strName;				//����
            std::string strDesc;				//����
			std::string strDependence;			//����
			std::string strWeakDependence;		//������
			std::string	strVersion;				//�汾��
			int nDefault;						//Ĭ�ϼ���
			int nDisable;						//������
			int nLoad;							//�Ƿ����
			int nLoaded;						//�Ѿ�����
            std::vector<std::string> aStrFile;
		};

		typedef std::vector<KADDONINFO> KADDONINFOARRAY;

	public:
		int Init();
		int GetCount();
		KADDONINFO * GetAddOnInfo(int nIndex);
		int Enable(int nIndex, int nEnable);
		int Disable(int nIndex, int nDisable);
		int LoadAddOn();

		int EnableOverdueAddOn(int nEnable);
		int IsEnableOverdueAddOn();

		int SetCurrentAddOnVersion(const char * pcszVersion);

		int GetIndexByID(const char * pcszID);

	private:
		int Append(LPCSTR pcszFile, LPCSTR pcszFolder);

		int LoadAddOn(const char * pcszID, BOOL bForce);
		int LoadAddOn(int nIndex, BOOL bForce);

		int CreateEnv();

		static int IndexOfEngine(lua_State* L);
		static int NewIndexOfEngine(lua_State* L);
		static int IndexOfAddOn(lua_State* L);
		static int NewIndexOfAddOn(lua_State* L);

	private:
		KADDONINFOARRAY m_aAddOn;

		int m_nEnableOverdueAddOn;
		std::string m_strCurrentAddOnVersion;
	};

	extern KAddOnMgr g_AddOnMgr;
}



#endif //_INCLUDE_KADDONMGR_H_
