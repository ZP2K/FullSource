#pragma once
/************************************************************************/
/*
GetLength�ǵõ��ܳ���GetPos�ǵõ���ǰд��λ�á��������g_cMemoryFileд�Ļ�
����GetLength�����ܵõ���ȷ��д���ֽڴ�С�������û���ù�SetPos�Ļ���GetPos�Ľ��Ӧ��һ�¡�
�����g_cMemoryFile���Ļ���GetLength�����Ѿ�д�Ĵ�С��
��Reset����LengthҲ��գ�����ҪSetPos(0)��read�꣬����GetPos()�������ܶ��Ĵ�С
*/
/************************************************************************/
#include "KG3DTemplates.h"

class KG3DMemoryFile
{
private:
	mutable size_t m_Position;	//�൱��ָ��
	size_t m_Length;	//�Ѿ�д�ĳ���

	enum
	{
		EM_BUFFER_SIZE = 1024*1024*4,
	};
	BYTE  m_byteData[EM_BUFFER_SIZE];

public:
	size_t GetCapability()const{return EM_BUFFER_SIZE;}
	void* GetBuffer();

	HRESULT Reset();

	int write(const void * _Str,size_t _Size);
	int read(void* _Str,size_t _Size)const;

	DWORD GetLength();
	DWORD GetMaxLength()	{return static_cast<DWORD>(GetCapability());}
	size_t GetPos()const	{return m_Position;}//��������������ƶ�дλ��
	VOID SetPos(size_t Pos);

	KG3DMemoryFile(void);
	~KG3DMemoryFile(void);

	HRESULT UnInit();
};

extern TKG3DResourcePool<KG3DMemoryFile>g_cMemoryFilePool;