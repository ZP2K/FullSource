////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGDummyIFile.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-9-2 14:23:49
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGDUMMYIFILE_H_
#define _INCLUDE_KGDUMMYIFILE_H_

////////////////////////////////////////////////////////////////////////////////
class KGDummyIFile : public IFile
{
public:
	// ��ȡ�ļ�����
	virtual unsigned long	Read(void* Buffer, unsigned long ReadBytes) {return ReadBytes;}
	// д���ļ�����
	virtual unsigned long	Write(const void* Buffer, unsigned long WriteBytes) {return WriteBytes;}
	// ����ļ�����Buffer,��iFile�ӿڸ����ͷ�������������ⲿӦ�ò��������ͷ�����
	virtual void*			GetBuffer() {return 0;}
	// �ƶ��ļ�ָ��λ�ã�Origin	-> Initial position: SEEK_CUR SEEK_END SEEK_SET
	virtual long	Seek(long Offset, int Origin) {return 0;}
	// ȡ���ļ�ָ��λ��,ʧ�ܷ���-1��
	virtual long	Tell() {return 0;}
	// ȡ���ļ�����,ʧ�ܷ���0��
	virtual unsigned long	Size() {return 0;}
	// �жϴ򿪵��Ƿ��ǰ��е��ļ�,���ز���ֵ
	virtual int		IsFileInPak() {return 0;}

	//�ж��ļ��Ƿ�ֿ�ѹ����
	virtual int		IsPackedByFragment() {return 0;}
	//��ȡ�ļ��ֿ����Ŀ
	virtual int		GetFragmentCount() {return 0;}
	//��ȡ�ֿ�Ĵ�С
	virtual unsigned int	GetFragmentSize(int nFragmentIndex) {return 0;}
	//��ȡһ���ļ��ֿ飬�����pBufferΪĿ�껺�������������Ļ�����ָ��Ϊ�գ����ڲ�������µĻ�������������ָ�룬�ⲿ��Ҫ�������١�
	//�������Ĵ�С��Ҫ�������·ֿ�����ݣ�����ͨ��GetFragmentCount֪��ÿ���ֿ�����ݴ�С����������������Ҫ����ô��
	//����ֵ��ʾʵ�ʶ�ȡ����������ֿ�����ݴ�С,�������ʧ��(�����ֿ鲻����,���ļ����ǰ��ֿ鷽ʽ�洢�ȵ�)����0��
	virtual unsigned long	ReadFragment(int nFragmentIndex, void*& pBuffer) {return 0;}

	// �رմ򿪵��ļ�
	virtual void	Close() {}
	// �ӿڶ�������
	virtual void	Release() {}

	virtual ~KGDummyIFile() {};
};

#endif //_INCLUDE_KGDUMMYIFILE_H_
