////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DRepresentObjectBuilder.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-3 15:42:17
/*  Comment     : ��ObjectTable��ͬ���������ֱ�Ӵ������󶨺��ڲ������RepresentObject��
��KG3DRepresentObjectTable�������ǿյ�Object���������֮����Ҫ�ֿ�����Ϊ��ͬRepresentObject
��ҽӲ�ͬ���࣬���ֱ����KG3DRepresentObjectTable���������ģ����Model����RepresentObject��
KG3DRepresentObjectTable����Ҫ�ͱ�Ĵ������ҽ������ˣ��Ӷ��γ�����ϡ�

KG3DRepresentObjectBuilder�ܹ�ֱ�Ӵ���������RepresentObject���������ģ�ͻ���SoundBall��

ʹ�÷�ʽ��
KG3DRepresentObjectBuilder::Build(pFile, REPRESENTOBJECT_DEFAULT, 0, 0, &pObj);
����ͨ��g_cEngineManager.BuildKG3DRepresentObject,�������ð������ͷ�ļ���

�������pFile����ô������ֱ��ͨ��pFileȷ�ϡ�Ҳ���Բ���������ƣ�ֻҪд�Եڶ��������Ϳ�����
���������pFile,ֱ�Ӵ��������͡�������ЩObj���ǲ���Ҫ�ļ��ġ�


Ҫʹ��������ƣ���Ҫ
1.KG3DRepresentObjectTable��GetTypeInfoByFileName��ͨ���ļ������������������͵�Object��
��Ϊ�Ѿ����ֳɵĻ����ˣ�ֻ��Ҫ��KG3DRepresentObjectTable��Init��������дstTypeInfo�Ǹ���

2.ȷ��KG3DRepresentObjectTable��Get1NewResourcePointer�ܹ��������Ǹ����͵��ࡣֱ��д
switch caseҲ�У����õ���ͨ��KG3DCollocator����KG3DRepresentObjectTable::RegisterType��ע��һ������
�����Ͳ���KG3DRepresentObjectTable�ͱ���ļ����������KG3DCollocator��ô��϶�����ν

3.����KG3DRepresentObjectTable��Load����������wParam,lParam��DWORD_PTR���͵ģ����Դ���
ָ�룬���߱���ڽ����ͣ��㹻�Ź�����Ϣ�ˡ�

*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DREPRESENTOBJECTBUILDER_H_
#define _INCLUDE_KG3DREPRESENTOBJECTBUILDER_H_
class KG3DRepresentObject;
class IEKG3DRepresentObject;
////////////////////////////////////////////////////////////////////////////////
class KG3DRepresentObjectBuilder
{
public:
	/*
	Build�����ú��������lpFileName��ΪNULL����ôType���Ǳ���ģ������FileName���ж����ͣ�
	��Щ�����ǲ���Ҫ�ļ��ģ��������Type������ҪFileName
	*/
	static HRESULT Build(LPCTSTR lpFileName, DWORD dwType, DWORD_PTR wParam, DWORD_PTR lParam, KG3DRepresentObject** ppRet);
	static HRESULT Build(LPCTSTR lpFileName, DWORD dwType, DWORD_PTR wParam, DWORD_PTR lParam, IEKG3DRepresentObject** ppRet);
	static HRESULT New(DWORD dwType, DWORD_PTR wParam, DWORD_PTR lParam, KG3DRepresentObject** ppRet);
};

#endif //_INCLUDE_KG3DREPRESENTOBJECTBUILDER_H_
