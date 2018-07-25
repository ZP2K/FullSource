// File: RWFile.h
// Desc:

#ifndef RWFILE_H
#define RWFILE_H

#include"File.h"

namespace Common
{

class DLLAPI RWFile: public File  // һ��ʼ����public�̳�ROFile��WOFile������һ��Open�������������γ�����ʯ�ε���̳�����Effective C++һ�鲻�ᳫ����������Ϊʲô��
{
public:
	virtual void Open();

	virtual void ReadBuffer(void* pBuffer, unsigned int uiBufferSize);
	virtual void WriteBuffer(const void* pBuffer, unsigned int uiBufferSize);

	virtual unsigned int ReadLine(char* pBuffer, unsigned int uiBufferSize = 256);
	virtual String ReadLine();
	virtual void WriteLine(const String& sLine);

	void SwitchRW() const { fflush(m_File); }
};

}

#endif