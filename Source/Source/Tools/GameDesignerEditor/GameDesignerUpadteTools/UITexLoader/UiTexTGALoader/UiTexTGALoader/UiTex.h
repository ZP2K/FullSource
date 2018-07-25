////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : UiTex.h
//  Version     : 1.0
//  Creator     : Hu Changyin
//  Create Date : 2005-12-17 13:45:25
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_UITEX_H_
#define _INCLUDE_UITEX_H_

////////////////////////////////////////////////////////////////////////////////

#include <vector>

////////////////////////////////////////////////////////////////////////////////

/*
UiTex�ļ�˵��:
����ͼ����Ϣ�ļ�
UiTex�ļ�ͷ
Frame������ ����Ϊ  nFrameCount * sizeof(UITEXFRAMEDATASTRUCTURE)
Animate������ ���Ȳ���  nFrameCount FrameIndex...
*/

struct UITEXFILEHEADER           //UiTex�ļ�ͷ
{
    DWORD   dwUiTexFlag;         //UiTex�ļ���־
    int     nWidth;              //��
    int     nHeight;             //��
    int     nFrameCount;         //֡��
    int     nAnimateCount;       //������
    DWORD   dwFileLen;           //�ļ�����
    char    szTgaFileName[32];   //Tga�ļ���
    char    szBuffer[32];        //����,Ҳ������ΪTga���ӳ�����
};

struct UITEXFRAMEDATASTRUCTURE   //Frame���ݿ�
{
    DWORD   dwInfo;              //Frame��Ϣ
    int     nLeft;               //������
    int     nTop;                //������
    int     nWidth;              //���
    int     nHeight;             //�߶�
};

struct TXTFILEDATA
{
	DWORD   dwInfo;					//Frame��Ϣ
	int		nFrameNum;				//UITex�ļ��ڶ������
	int		nTextNum;				//Icon�ļ��ڶ������
	int     nLeft;					//��
	int     nTop;					//��
	int     nWidth;					//��
	int     nHeight;				//��
	int		nNewLeft;				//icon�ļ����ź����
	int		nNewTop;				//icon�ļ����ź����
	char	szTxtFile[MAX_PATH];	//·��
};

#define MAX_TGA_BLOCK_NAME_LEN      32
#define KEEP_ORIGINAL_INDEX         0x00000001
#define HAS_AREADY_ANIMATE_GROUP    0x00000002
#define KEEP_POS_WHEN_AUTO_MAKE_UP  0x00000004  
#define HASE_BEEEN_SET_POS          0x00000008


#endif //_INCLUDE_UITEX_H_
