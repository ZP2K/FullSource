#pragma once

#define IMUSIC_INTERFACE_VERSION "C.Q_Music_System.01" 

#define PlayError -2
#define NoChannelToPlay -1
#define MethodTakePlaceError -3
#define RunOk 1
class MusicApplication
{
public:
	MusicApplication(void) ;
	virtual ~MusicApplication(void);
	// ����һ�����֣������ؾ��,����ܵ��������򷵻�NoChannelToPlay
	virtual int PlayAmusic(char * sFileName) = 0 ;
	// ֹͣ����ĳ������iChannelNum�����־��
	virtual int StopAmusic(int iHandle) = 0 ;
	// ֹͣ�������е�����
	virtual int StopAllMusic(void) = 0 ;
	// ��ʼ��
	virtual void Init(void) = 0 ;

	// ��ֹϵͳ
	virtual void Terminate(void) = 0 ;
};

