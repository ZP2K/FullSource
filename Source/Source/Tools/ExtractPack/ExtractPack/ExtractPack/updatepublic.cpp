//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   UpdatePublic.cpp
//  Version     :   1.0
//  Creater     :   Cheng Bitao
//  Date        :   2002-1-17 11:15:44
//  Comment     :   Inplement the comment interface of update system
//
//////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "windows.h"
#include <stdio.h>
#include "updatepublic.h"
#include "KGPublic.h"
#include <stdio.h>
#include <tchar.h>
#include <io.h>
#include <map>
using std::map;

///////////////////////////////////////////////////////////////////////////////
//���°��ϲ� Add By Fellow 2003.08.11w

///////////////////////////////////////////////////////////////////////////////
//���°��ϲ�bug�޸� �Լ� ������ Add By zhongguangze

#ifndef MAX
	#define MAX(x, y)					((x) > (y) ? (x) : (y))
	#define MIN(x, y)					((x) < (y) ? (x) : (y))
#endif



// INT getIndex(t_index_info* IndexList,INT Count, DWORD id)
// {//���ַ��ҵ�ָ��������
// 	INT nBegin, nEnd, nMid;
// 	nBegin = 0;
// 	nEnd = Count;
// 	while (nBegin <= nEnd) {
// 		nMid = (nBegin + nEnd) / 2;
// 		if (id == IndexList[nMid].id) break;
// 		if (id < IndexList[nMid].id) nEnd = nMid - 1;
// 		else nBegin = nMid + 1;
// 	}
// 	if(id != IndexList[nMid].id) return -1;								//�����ļ�����Ҳû��
// 	return nMid;
// 	
//}
VOID GetHeader(t_pack_header* aHeader, FILE* aFile)
{
	_fseeki64(aFile,0,SEEK_SET);
	fread(aHeader,1,sizeof(t_pack_header),aFile);
}

VOID GetIndexInfo(t_index_info* aIndexInfo, CONST t_pack_header* aHeader, FILE* aFile)
{
	_fseeki64(aFile,aHeader->index_offset,SEEK_SET);
	fread(aIndexInfo, sizeof(t_index_info), aHeader->count, aFile);
}

bool PackComb(LPCSTR stdFilePath, LPCSTR addFilePath)
{//�ϲ��ļ�
	int           nResult     = false;
	t_pack_header stdHeader;
	t_pack_header addHeader;
	DWORD         offset      = 0;
	t_index_info* stdIndex    = NULL;
	t_index_info* addIndex    = NULL;
	//����ԴԴ�ļ�
	FILE*         stdFile     = NULL;
	FILE*         addFile     = NULL;
	char*         temp_buffer = NULL;
	LPSTR         DataBuffer  = NULL;

	temp_buffer = new CHAR[MAX_FILE * sizeof(t_index_info)];	
	KGLOG_PROCESS_ERROR(temp_buffer);
	ZeroMemory(temp_buffer, MAX_FILE * sizeof(t_index_info));

	KGLOG_PROCESS_ERROR(stdFilePath);
	KGLOG_PROCESS_ERROR(addFilePath);

	stdFile = fopen(stdFilePath, "r+b");
	KGLOG_PROCESS_ERROR(stdFile);
	
	GetHeader(&stdHeader, stdFile); //�õ�Ŀ��PAK���ļ�����

	//����Դ�������ļ�	
	addFile = fopen(addFilePath, "rb");
	KGLOG_PROCESS_ERROR(addFile);
	GetHeader(&addHeader, addFile);

	//���addFile����û����Ҫ���ӵ����ݾ�ֱ�����
	KG_PROCESS_SUCCESS(addHeader.count == 0);

	KGLOG_PROCESS_ERROR((stdHeader.count + addHeader.count) < MAX_FILE);

	stdIndex = new t_index_info[stdHeader.count + addHeader.count];
	KGLOG_PROCESS_ERROR(stdIndex);
	memset(stdIndex, 0, sizeof(t_index_info) * (stdHeader.count + addHeader.count));
	//�õ�Ŀ���ļ���������Ϣ
	GetIndexInfo(stdIndex, &stdHeader, stdFile);

	addIndex = new t_index_info[addHeader.count];
	KG_PROCESS_ERROR(addIndex);
	memset(addIndex, 0, sizeof(t_index_info) * addHeader.count);
	//�õ�Ҫ����ļ���������Ϣ
	GetIndexInfo(addIndex, &addHeader, addFile);
	
	//copy���ӵ����ݵ�Դ�ļ���
	_fseeki64(stdFile, 0, SEEK_END);
	offset = (DWORD)_ftelli64(stdFile);
	INT DataSize = addHeader.index_offset - addHeader.data_offset; //�õ��������ݿ�����Ĵ�С
	_fseeki64(addFile, addHeader.data_offset, SEEK_SET);  //���ļ�ָ���ƶ������ݿ�

	while (DataSize > 0)
	{
		// �������1M�ڴ棬��������ļ����ʹ��
		UINT uCurDataSize = (UINT)MIN(1024*1024, DataSize);

		if (!DataBuffer)
			DataBuffer = new CHAR[uCurDataSize];

		UINT uReadSize = fread(DataBuffer, 1, uCurDataSize, addFile);

		LPSTR pDataBufferWrite = DataBuffer;
		while (DataBuffer + uReadSize - pDataBufferWrite > 0)
			pDataBufferWrite += fwrite(pDataBufferWrite, 1, DataBuffer + uReadSize - pDataBufferWrite, stdFile);

		DataSize -= uReadSize;
	}

	//���½���index
	UINT i = 0;
	UINT j = 0;
	for(i = 0; i < addHeader.count; i++)
	{
		t_index_info* index = &addIndex[i];
		for(j = 0; j < stdHeader.count; j++) {
			if(index->id < stdIndex[j].id)
			{	//��������
				memcpy(temp_buffer, (CHAR *)&stdIndex[j], (stdHeader.count - j) * sizeof(t_index_info));
				memcpy((CHAR *)&stdIndex[j + 1], temp_buffer, (stdHeader.count - j) * sizeof(t_index_info));
				//memmove((CHAR *)&stdIndex[j + 1], (CHAR *)&stdIndex[j], (stdHeader.count - j) * sizeof(t_index_info));

				//���������
				memcpy(&stdIndex[j], index, sizeof(t_index_info));
				stdIndex[j].offset = offset + index->offset - sizeof(t_pack_header);
				++stdHeader.count;
				break;
			}
			else if(index->id == stdIndex[j].id)
			{	//�������滻������
				stdIndex[j].offset = offset + index->offset - sizeof(t_pack_header);
				stdIndex[j].size = index->size;
				stdIndex[j].compress_size = index->compress_size;
				break;
			}
		}
		if(j == stdHeader.count) {
			memcpy(&stdIndex[j], index, sizeof(t_index_info));
			stdIndex[j].offset = offset + index->offset - sizeof(t_pack_header);//Add By Fellow 09.04
			stdHeader.count++;
		}
	}

	//д���µ�index
	_fseeki64(stdFile,0,SEEK_END);
	offset = (DWORD)_ftelli64(stdFile);
	fwrite(stdIndex,sizeof(t_index_info),stdHeader.count,stdFile);
	//д��header
	stdHeader.index_offset = offset;
	_fseeki64(stdFile,0,SEEK_SET);
	fwrite(&stdHeader,sizeof(t_pack_header),1,stdFile);

Exit1:
	nResult = true;
	//�ر��ļ�
Exit0:
	if (stdFile)
	{
		fclose(stdFile);
		stdFile = NULL;
	}
	if (addFile)
	{
		fclose(addFile);
		addFile = NULL;
	}

	KG_DELETE_ARRAY(DataBuffer);
	KG_DELETE_ARRAY(stdIndex);
	KG_DELETE_ARRAY(addIndex);
	KG_DELETE_ARRAY(temp_buffer);

	return nResult;
}

int CompressPack(const TCHAR* cpszPackFileName)
{
	int nResult  = false;
	t_pack_header PakHeader;
	t_index_info* pPakIndexInfo = NULL;
	DWORD dwOffset = 0;
	FILE* pPakFile = NULL;
	FILE* pTargetFile = NULL;
	LPSTR pszDataBuffer = NULL;
	LPSTR pszFileBuffer = NULL;
	int   nTotalComprassSize = 0;
	map<DWORD, t_PakNode_Info> mapIndex;
	map<DWORD, t_PakNode_Info>::iterator itorStart;
	map<DWORD, t_PakNode_Info>::iterator itorEnd;
	TCHAR szTargetFile[MAX_PATH];

	KGLOG_PROCESS_ERROR(cpszPackFileName);

	//��PAK�ļ�
	pPakFile = fopen(cpszPackFileName, "rb");
	KGLOG_PROCESS_ERROR(pPakFile);

	//��ȡPAK�ļ�ͷ��Ϣ
	GetHeader(&PakHeader, pPakFile);

	//ȡ��PAK���ڵ�����index��Ϣ
	pPakIndexInfo = new t_index_info[PakHeader.count];
	memset(pPakIndexInfo, 0, sizeof(t_index_info) * PakHeader.count);

	GetIndexInfo(pPakIndexInfo, &PakHeader, pPakFile);


	//��������Ϣ������MAP��
	for (int k = 0; k < PakHeader.count; k++)
	{
		//t_PakNode_Info pNode;
		unsigned int nRealComprassSize = pPakIndexInfo[k].compress_size & 0x07ffffff;

		nTotalComprassSize += nRealComprassSize;
		mapIndex[pPakIndexInfo[k].offset].pIndexInfo = &pPakIndexInfo[k];
		mapIndex[pPakIndexInfo[k].offset].nComprassSize = nRealComprassSize;
	}
	
	//������ʱ�ļ�
	_snprintf(szTargetFile, MAX_PATH, "%s.tmp", cpszPackFileName);
	pTargetFile = fopen(szTargetFile, "wb+");
	KGLOG_PROCESS_ERROR(pTargetFile);
	
	//�����ļ���д��PAKͷ��Ϣ
	_fseeki64(pTargetFile, 0, SEEK_SET);	
	fwrite(&PakHeader, sizeof(t_pack_header), 1, pTargetFile);

	//ȡ��д��PAKͷ��Ϣ���ļ�ָ��λ��,����������ļ���data_offset
	dwOffset = _ftelli64(pTargetFile);
	PakHeader.data_offset = dwOffset; //����ͷ��Ϣ�е����ݿ��ļ�ƫ��

	pszDataBuffer = new char[BUFFER_MAX]; //���뻺����
	pszFileBuffer = new char[BUFFER_MAX];

	memset(pszDataBuffer, 0xee, BUFFER_MAX);
	memset(pszFileBuffer, 0xcc, BUFFER_MAX);

	//Ѱ�Ҳ�д�����ݿ�
	itorStart = mapIndex.begin();
	itorEnd   = itorStart;
	
	int nTotalWriteSize = sizeof(t_pack_header);  //�Ѿ�д���˶��ٸ��ֽ� �����Ѿ�д���PAKͷ��Ϣ
	while (true)
	{
		int nRealWriteSize = 0;  //���μ������Ҫд������ݴ�С
		int nBeasOffset    = 0;  //����ƫ�ƵĻ���ַ
		int nFileReadSize  = 0;  //׼����ȡ���ڴ������

		if (itorStart == mapIndex.end())
			break;
		
		nBeasOffset = itorStart->first;

		//����׼����ȡ���ڴ�����䣬�����ǰ���ǣ�ƫ�Ƶ�ַ-����ַ ������64MB
		while (true)
		{
			if (itorEnd == mapIndex.end())
				break;
			if ((itorEnd->first + itorEnd->second.nComprassSize - nBeasOffset) >= BUFFER_MAX)
				break;

			nFileReadSize = itorEnd->first + itorEnd->second.nComprassSize - nBeasOffset;
			++itorEnd;
		}	

		//��ԴPAK�ж�������
		_fseeki64(pPakFile, nBeasOffset, SEEK_SET);
		int nTempSize = nFileReadSize;
		int nRealSize = 0;
		while (nTempSize > 0)
		{
			nRealSize += fread(pszFileBuffer + nRealSize, 1, nTempSize, pPakFile);
			nTempSize -= nRealSize;
		}

		int nDataOffset = 0; //�ڻ������е�ƫ��ָ��
		nTempSize = nFileReadSize;
		//������Ҫ�����ݵ���������
		while (itorStart != itorEnd)
		{	
			memcpy(pszDataBuffer + nDataOffset, pszFileBuffer + (itorStart->first - nBeasOffset), itorStart->second.nComprassSize);
			itorStart->second.pIndexInfo->offset = nTotalWriteSize;
			nTotalWriteSize += itorStart->second.nComprassSize;

			nDataOffset += itorStart->second.nComprassSize;
			nTempSize -= itorStart->second.nComprassSize;
			++itorStart;
		}

		int nWriteSize = 0;
		nRealWriteSize = nDataOffset;

		while (nRealWriteSize > 0)
		{			
			_fseeki64(pTargetFile, 0, SEEK_END);
			nWriteSize += fwrite(pszDataBuffer + nWriteSize, 1, nRealWriteSize, pTargetFile);
			nRealWriteSize -= nWriteSize;
		}
		nRealWriteSize = 0;
		nDataOffset = 0;
 
	}

	//д�����
	_fseeki64(pTargetFile, 0, SEEK_END);	

	//��������ƫ��
	dwOffset = (DWORD)_ftelli64(pTargetFile);	
	//��index_infoд�뵽�ļ�ĩ
	fwrite(pPakIndexInfo, sizeof(t_index_info), PakHeader.count, pTargetFile);

	//����PAKͷ��Ϣ
	PakHeader.index_offset = dwOffset;
	_fseeki64(pTargetFile, 0, SEEK_SET);
	fwrite(&PakHeader, sizeof(t_pack_header), 1, pTargetFile);

	nResult = true;
Exit0:

	if (pTargetFile)
	{
		fclose(pTargetFile);
		pTargetFile = NULL;
	}

	if (pPakFile)
	{
		fclose(pPakFile);
		pPakFile = NULL;
	}
	
	if (nResult)
	{
		TCHAR szRename[MAX_PATH];
		_snprintf(szRename, MAX_PATH, "%s.rename", cpszPackFileName);
		_trename(cpszPackFileName, szRename);
		_trename(szTargetFile, cpszPackFileName);
		_tremove(szRename);
	}

	KG_DELETE_ARRAY(pPakIndexInfo);
	KG_DELETE_ARRAY(pszDataBuffer);
	KG_DELETE_ARRAY(pszFileBuffer);

	return nResult;
}

////////////////////////////////////////////////////////////////////////////////
