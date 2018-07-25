// File: ConsoleInput.h
// Desc:

#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H

#include"DLLAPI.h"

namespace Common
{

DLLAPI bool WaitInputND(unsigned int WaitMS, char ExpectChar = 0, bool bCase = false);  // ������
DLLAPI bool WaitInputD(unsigned int WaitMS, char ExpectChar = 0, bool bCase = false);  // ����

}  // namespace

#endif