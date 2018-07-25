#ifndef _KPROCESS_H_
#define _KPROCESS_H_

class KProcess
{
public:
    KProcess(void);
    ~KProcess(void);

    //************************************
    // Method:    FindProcess
    // FullName:  KProcess::FindProcess
    // Access:    public static 
    // Returns:   int �ɹ� true��ʧ�� false
    // Qualifier:
    // Parameter: const TCHAR cszExeFilePath[] ����exe·��
    // Parameter: int * pnNumRunning �������н������������û�����У�ֵΪ0
    //************************************
    static int FindProcess(const TCHAR cszExeFilePath[], int* pnNumRunning);
};

#endif //_KPROCESS_H_