//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   UpdatePublic.h
//  Version     :   1.0
//  Creater     :   Cheng Bitao
//  Date        :   2002-1-17 11:14:17
//  Comment     :   Define the comment interface of update system
//
//////////////////////////////////////////////////////////////////////////////////////


#ifndef __UPDATE_PUBLIC_H__
#define __UPDATE_PUBLIC_H__ 

//------------------------------------------------------------------------------------

#define MAX_FILE					2004800			//���40����ļ�
///////////////////////////////////////////////////////////////////////////////
//���°��ϲ� Add By Fellow 2003.08.11
//һ��Pack�ļ���������Ľṹ:
//�������ĸ��ֽڵ��ļ���ͷ��־:�ַ���'PACK',Ȼ���������ĿȻ����������ʼ��ƫ����\���ݿ�ʼ��ƫ����,Ȼ����У���,Ȼ���Ǳ������ֽ�:
typedef struct {
	UCHAR signature[4];			//"PACK"
	DWORD count;				//���ݵ���Ŀ��
	DWORD index_offset;			//������ƫ����
	DWORD data_offset;			//���ݵ�ƫ����
	DWORD crc32;
	UCHAR reserved[12];
} t_pack_header;



typedef struct {		//������Ϣ
	DWORD id;
	DWORD offset;
	INT size;
	INT compress_size;
} t_index_info;

typedef struct 
{
	t_index_info* pIndexInfo;
	int nComprassSize;
	//int nBlackNum;

} t_PakNode_Info;

#define MAX_FILE					2004800			//���20����ļ�
#define BUFFER_MAX                  1024 * 1024 * 64  //128MB�Ļ���ռ�

#ifndef MAX
	#define MAX(x, y)					((x) > (y) ? (x) : (y))
	#define MIN(x, y)					((x) < (y) ? (x) : (y))
#endif

VOID GetHeader(t_pack_header* aHeader, FILE* aFile);
VOID GetIndexInfo(t_index_info* aIndexInfo, CONST t_pack_header* aHeader, FILE* aFile);
bool PackComb(LPCSTR stdFilePath,LPCSTR addFilePath);
int CompressPack(const TCHAR* cpszPackFileName);
///////////////////////////////////////////////////////////////////////////

#endif  //__UPDATE_PUBLIC_H__

