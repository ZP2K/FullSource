/*
UiTex�ļ�˵��:
����ͼ����Ϣ�ļ�
UiTex�ļ�ͷ
Frame������ ����Ϊ  nFrameCount * sizeof(UITEXFRAMEDATASTRUCTURE)
Animate������ ���Ȳ���  nFrameCount FrameIndex...
*/


struct UITEXFILEHEADER           //UiTex�ļ�ͷ
{
    DWORD   dwUiTexFlag;          //UiTex�ļ���־
    int     nWidth;              //��
    int     nHeight;             //��
    int     nFrameCount;         //֡��
    int     nAnimateCount;       //������
    DWORD   dwFileLen;           //�ļ�����
    char    szTgaFileName[32];   //STex�ļ���
    char    szBuffer[32];         //����,Ҳ������ΪszTextFile���ӳ�����
};

struct UITEXFRAMEDATASTRUCTURE   //Frame���ݿ�
{
    DWORD   dwInfo;              //Frame��Ϣ
    int     nLeft;               //������
    int     nTop;                //������
    int     nWidth;              //���
    int     nHeight;             //�߶�
};
