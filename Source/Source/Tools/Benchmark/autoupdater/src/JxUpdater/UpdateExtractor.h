#ifndef UPDATE_EXTRACTOR_H
#define UPDATE_EXTRACTOR_H

#include <vector>
using std::vector;

#ifdef UPDATEEXTRACTOR_EXPORTS
	#define DLLDECL __declspec(dllexport)
#else
	#define DLLDECL __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* HExtract;

//��ʼ��,���ؽ�ѹ����һ��HANDLE
//packagePath:	ѹ����·��
//outputPath:	��ѹ��·��
DLLDECL HExtract UpdateExtractor_InitW(wchar_t const *packagePath, wchar_t const *outputPath);
DLLDECL HExtract UpdateExtractor_InitA(char const *packagePath, char const *outputPath);

#if defined(UNICODE) || defined(_UNICODE)
#define UpdateExtractor_Init		UpdateExtractor_InitW
#else
#define UpdateExtractor_Init		UpdateExtractor_InitA
#endif

//��ѹ����һ���ļ�
DLLDECL int UpdateExtractor_ExtractNext(HExtract he);

//��ѹ����һ���ļ�
DLLDECL int UpdateExtractor_ExtractSingleFile(HExtract he, const TCHAR* szFileName);
//�ͷ���Դ
//���InitExtractInfoʧ�ܣ����ص���UninitExtractInfo
DLLDECL void UpdateExtractor_Uninit(HExtract he);
//��ȡѹ�������ļ���������һ�����ֵ��ʵ��Ӧ��ExtractNext�ķ���ֵ�ж��Ƿ��ѹ��ɡ�
DLLDECL int UpdateExtractor_GetFileCount(HExtract he);
//��ȡ��ǰ���ڽ�ѹ�����ļ���
DLLDECL char const *UpdateExtractor_GetCurrentFileA(HExtract he);
DLLDECL wchar_t const *UpdateExtractor_GetCurrentFileW(HExtract he);

#if defined(UNICODE) || defined(_UNICODE)
#define UpdateExtractor_GetCurrentFile	UpdateExtractor_GetCurrentFileW
#else
#define UpdateExtractor_GetCurrentFile	UpdateExtractor_GetCurrentFileA
#endif

#ifdef __cplusplus
}
#endif

#endif