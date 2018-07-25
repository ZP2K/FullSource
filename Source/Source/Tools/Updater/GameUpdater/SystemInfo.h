
/**
* @file     SystemInfo.h
* @brief    ��ȡϵͳ��Ϣ��
*
* @details  �����ȡϵͳ������Ϣ
* @author   xiaojian <xiaojian@kingsoft.com>
* @date     2008-09-09
* @version	1.0.0.1
*/
#include "stdafx.h"

#pragma once

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
    * @brief		    ��ȡϵͳ�ڴ��С
    *
    * @details
    * 
    * @param[out]       �����ڴ��С�ַ���
    * @return[S_OK]     �ɹ�
    * @return[E_FAILE]  ʧ��
    * @remarks	        ��λkb
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

	int GetCpuCoreNum();

private:
	const CString m_strKey;
	int m_nCpuCoreNum;
};
