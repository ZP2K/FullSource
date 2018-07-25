#pragma once
#include "stdafx.h"
#include "IEOther.h"
#include "IEEditor.h"

class EngineProxy
{
public:
	EngineProxy(void);
	~EngineProxy(void);

public:
	//��ʼ������ϵͳ
	BOOL InitLog();	

	//��ʼ��3d����
	BOOL Init3DEngine(unsigned uMode, unsigned int uFlag, HWND hBaseWindow, HWND hRenderWindow,const char szEngineRootPath[]);

	//����ʼ��3d����
	void UnInit3DEngine();

	//��ʼ������ʱ������
	BOOL InitContext();

	//��ʼ������
	BOOL InitScene(HWND hwndOutput, LONG iOption);	

	//��ʼ��ģ�ͱ༭��
	BOOL InitModeEditor();

	//���ݵ�ǰ�ļ�·�����������·��
	BOOL UpdateEngineRootPath(LPCSTR lpFileName);


public:
	
	//��ʼ����
	BOOL Start(HWND hwd);

	//����ʼ��
	BOOL UnInit();

	//ֹͣ����
	BOOL Stop();

	//�����ļ�������ͼ���������Ƿ�ɹ�
	BOOL CreateThumbnail(const SIZE bmSize, LPCTSTR lpFileName);

	//BOOL CreateGifFile(const SIZE bmSize, LPCTSTR lpFileName);

	BOOL CreateAnimationFile(const SIZE bmSize, LPCTSTR lpFileName, LPCTSTR lpAniFileName);
//
//private:
//	//�ж��ļ��Ƿ���mesh| mdl �ļ�����
//	BOOL	IsMeshMdlFile(LPCTSTR lpFileName);


	//������
	IEKG3DSceneModelEditor* m_pModelEditor ;
	HMODULE m_hEngineMod;
	IEKG3DEngineManager* m_pEngineManager;
	IEKG3DScene* m_pScene;
	IKG3DCamera* m_pCamera ;
	D3DXVECTOR3 m_CameraPos;
	INT m_nOutputWindowID;
	IEKG3DModel* m_pLastModel;
	HANDLE m_hThreadEngine;
	HANDLE m_hEngineInitFinishEvent;


	
};
