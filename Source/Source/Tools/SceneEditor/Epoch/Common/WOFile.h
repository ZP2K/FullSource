// File: WOFile.h
// Desc: ֻд�ļ�

#ifndef WOFILE_H
#define WOFILE_H

#include"File.h"

namespace Common
{

class DLLAPI WOFile: public File
{
public:
	virtual void Open();

	virtual void WriteBuffer(const void* pBuffer, unsigned int uiBufferSize);
	virtual void WriteLine(const String& sLine);
};

}  // namespace

#endif