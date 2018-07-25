
/**
* @file     SystemInfo.h
* @brief    ��ȡϵͳ��Ϣ��
*
* @details  �����ȡϵͳ������Ϣ
* @author   xiaojian <xiaojian@kingsoft.com>
* @date     2008-09-09
* @version	1.0.0.1
*/

#ifndef _SYSTEM_INFO_H_
#define _SYSTEM_INFO_H_

#pragma warning(disable: 4819)

class CSystemInfo
{
public:
    /** @brief	���캯�� */
    CSystemInfo(void);
    /** @brief	�������� */
    ~CSystemInfo(void);

public:
    /**
    * @brief		    ��ȡ����ϵͳ�汾��Ϣ
    *
    * @details
    * 
    * @param[out]       ���ز���ϵͳ�汾�ַ���
    * @return[S_OK]     �ɹ�
    * @return[E_FAILE]  ʧ��
    * @remarks	       
    */
    HRESULT GetSystemVersion(CString& strVersion);

	/**
	* @brief		    ��ȡ����ϵͳ�汾��Ϣ(���ַ�ʽ)
	*
	* @details
	* 
	* @param[out]       ���ز���ϵͳ���
	* @return[S_OK]     �ɹ�
	* @return[E_FAILE]  ʧ��
	* @remarks	        �汾����ϵͳ��Ӧ���£�
	*                   5.0 �� Windows 2000
	*                   5.1 = Windows XP
	*                   5.2 = Windows Server 2003
	*					6.0 = Windows Vista
	*/
	HRESULT GetSystemId(CString& SystemId);

    /**
    * @brief		    ��ȡϵͳ�ڴ��С
    *
    * @details
    * 
    * @param[out]       �����ڴ��С�ַ���
    * @return[S_OK]     �ɹ�
    * @return[E_FAILE]  ʧ��
    * @remarks	        ��λMb
    */
    HRESULT GetSystemMemInfo(CString& strMem);

    /**
    * @brief		    ��ȡϵͳCPU��Ϣ
    *
    * @details
    * 
    * @param[out]       ����CPU��Ϣ�ַ���
    * @return[S_OK]     �ɹ�
    * @return[E_FAILE]  ʧ��
    * @remarks	        ��CPUʱ,��CPU��Ϣ�� '&' ���ŷָ�
    */
    HRESULT GetSystemCpuInfo(CString& strCpu);

	/*
	* @brief			��ȡϵͳ�Կ���Ϣ
	* 
	* @details
	* @param[out]		�����Կ���Ϣ
	* @return[S_OK]		�ɹ�
	* @return[E_FAILE]	ʧ��
	* @remarks			���Կ�ʱ�����Կ���Ϣ�� '&' ���ŷָ�
	*/
	HRESULT GetDisplayInfo(CString& strDisplay);

	/*
	* @brief			��ȡϵͳ�Կ��Դ��С
	* 
	* @details
	* @param[out]		�����Կ��Դ��С
	* @return[S_OK]		�ɹ�
	* @return[E_FAILE]	ʧ��
	* @remarks
	*/
	HRESULT GetDisplayMem(CString& strMem);

	/*
	* @brief			��ȡӲ��ΨһID
	*
	* @details
	* @param[out]		Ӳ��id
	* @return[S_OK]		�ɹ�
	* @return[E_FAILE]	ʧ��
	* @remarks			MAC��ַ
	*/
	HRESULT GetHardDiskId(CString& strHardId);

	/*
	* @brief			md5����
	*
	* @details			ֱ������Ҫ�����ַ�����׷����Կ�ַ���,Ȼ�����md5ֵ
	* @param[in]		��Ҫ�����ַ���
	* @param[out]		���ܺ��ַ���
	* @return[S_OK]		�ɹ�
	* @return[E_FAILE]	ʧ��
	* @remarks			������Կ : 92A73EB6-FA72-40f9-AFD2-BD0376F40FD0
	*/
	HRESULT EncryptString(const CString strSrc, CString& strOut);


private:
	const CString m_strKey;
};

#endif // _SYSTEM_INFO_H_