////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DEnvEffList.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-5-11 10:03:16
//  Comment     : ֻ����ָ���ȡ����������Ĵ����ͷ�
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DENVEFFLIST_H_
#define _INCLUDE_KG3DENVEFFLIST_H_
#include "KG3DEnvEffDW_KeyBase.h"
////////////////////////////////////////////////////////////////////////////////
namespace KG3D_ENVIRONMENT_EFFECT
{
	class KG3DEnvEffect;
	class KG3DEnvEff_List : public KG3DEnvEffListBase
	{
	public:
		ULONG GetValidElementCount();
		KG3DEnvEffect* Find(DWORD dwType);
		VOID Clear();
		ULONG Empty();
		KG3DEnvEffect* Erase(DWORD dwType);
		HRESULT Insert(DWORD dwType, KG3DEnvEffect* pEffect);

		//BOOL	IsValidHandle(UINT Handle);
		//UINT	GetTopEnvEffHandle(DWORD dwType);
		//KG3DEnvEffect* GetTopEnvEffect(UINT nHandle);//ֻ�м��ٵ�ʱ��������
		
		////������ʱ��������������
		//virtual KG3DEnvEffect* ResetIterator();//�ͻ�����������
		//virtual KG3DEnvEffect* NextIterator();

		virtual KG3DEnvEffect* GetFirstTopEffect(KGCH::IteratorHolder& holderFirst);
		virtual KG3DEnvEffect* GetNextTopEffect(KGCH::IteratorHolder& holderPrev);


		//������ʱ��������������
		//virtual KG3DEnvEffect* GetFirstTopEnvEffect(UINT& Handle);
		//virtual KG3DEnvEffect* GetNextTopEnvEffect(UINT Handle, UINT& NextHandle);

		//Ҳ���������ֱ���
		virtual KG3DEnvEffect* GetEnvEffect(DWORD dwType, int nFinded = 0);///���Դ����õ����е�EnvEffect
		virtual INT GetEnvEffectCount(DWORD dwType);
		virtual VOID GetExistEnvEffectTypes(std::vector<DWORD>& dwTyps);

		//�����ֱ���
		//virtual KG3DEnvEffect* GetFirstTopEnvEffect(KG3DENVEFF_HANDLE& EnvHandle);
		//virtual KG3DEnvEffect* GetNextTopEnvEffect(KG3DENVEFF_HANDLE PrevHandle, KG3DENVEFF_HANDLE& NextHandle);

		virtual HRESULT Traverse(KG3DEnvEffectVisitor& Visitor);
		///////////////////////////////////////////////////////////
		
		KG3DEnvEff_List();
		~KG3DEnvEff_List();

		static VOID TDDTest();

	private:
		enum{MAX_SIZE = KG3DENVEFF_MAX,};
		//KG3DEnvEffect*	m_InnerArray[MAX_SIZE];
		//int	m_nValidElementCount;	//���ڽo�ⲿ����ռ��ʱ����Ϊ�ο�����Ϊ��һ�����е�EnvEff������ʼ����
		/*inline KG3DEnvEffect*&	GetElement(int nIndex)
		{
			_ASSERTE(nIndex >= 0 && nIndex < _countof(m_InnerArray));
			return m_InnerArray[nIndex];
		}*/
		typedef std::map<DWORD, KG3DEnvEffect*> TypeCt;
		TypeCt m_Ct;
		TypeCt::iterator m_CtIt;
	};
};


#endif //_INCLUDE_KG3DENVEFFLIST_H_
