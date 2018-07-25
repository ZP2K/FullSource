////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEClassMemberDescription.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-7-30 14:20:59
/*  Comment     : �淶���Ľṹ���Ա��������IEClassMemberDescription����һ����̬
���飬Ȼ������������ʼ��һ��IEClassMemberDescriptionPack��Ȼ�󴫵����棬�磺
IEClassMemberDescriptionPack	GetMemberDescrption()const
{
	typedef KG3DBillboardCloudCreaterThresholdStruct MyType;
	static IEClassMemberDescription Desc[] = 
	{
		{KG3DTYPE_FLOAT, sizeof(FLOAT), offsetof(MyType, m_fMaxNormalBias), NULL, _T("���Normalƫ��/����")},
		{KG3DTYPE_FLOAT, sizeof(FLOAT), offsetof(MyType, m_fMaxNormalDistanceBiasPercent), NULL, _T("Normal����������ƫ�ưٷֱ�")},
		{KG3DTYPE_FLOAT, sizeof(FLOAT), offsetof(MyType, m_fMaxDistanceToPoint), NULL, _T("�㵽��������ƫ�ưٷֱ�")},
		{KG3DTYPE_FLOAT, sizeof(FLOAT), offsetof(MyType, m_fMinAreaInfluenceAtNormalCorrection), NULL, _T("��С���Ȩֵ")},
		{KG3DTYPE_FLOAT, sizeof(FLOAT), offsetof(MyType, m_fMinAreaAccepted), NULL, _T("������С���")},
	};
	static IEClassMemberDescriptionPack Pack(Desc, _countof(Desc), this);
	return Pack;
}
��������Ϳ�������Щ���ݳ�ʼ�������ˡ����ṹ��Ķ���ʱ��ֻҪ�Ķ���Щ������UI�Ĵ��벻�����κθĶ���
�ܹ��Զ����ɽ���
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IECLASSMEMBERDESCRIPTION_H_
#define _INCLUDE_IECLASSMEMBERDESCRIPTION_H_

////////////////////////////////////////////////////////////////////////////////
struct IEClassMemberDescription 
{
	DWORD	m_Type;
	size_t	m_Size;
	size_t	m_Offset;
	LPCTSTR m_MemberName;	//��Ա��ԭ����������NULL
	LPCTSTR m_MemberTitle;	//��Ա�����ֵ���˼�����ڽ��棬Ӧ�ü�࣬��������ģ�����ΪNULL
};
//��ʼ������������� static IEClassMemberDescriptionPack stPack(stDesc, _countof(stDesc), this);
//��ʼ���������������Ҫ�ļ��
struct IEClassMemberDescriptionPack 
{
	enum{max_num = 1024,};
	const IEClassMemberDescription* m_pDesc;
	size_t	m_NumberOfMembers;

	IEClassMemberDescriptionPack():m_pDesc(NULL), m_NumberOfMembers(0){}

	template<class _Type>
	IEClassMemberDescriptionPack(const IEClassMemberDescription* pDesc, size_t num, const _Type* pType)//num���ܳ���max_num
		:m_pDesc(pDesc)
		,m_NumberOfMembers(num)
	{
		_ASSERTE(NULL != pDesc && num <= max_num);
		//������Ϸ���
		if(num > 1)
		{
			for(size_t i = 1; i < num; ++i)
			{
				_ASSERTE(pDesc[i-1].m_Offset < pDesc[i].m_Offset);
			}
		}
#if defined(DEBUG) | defined(_DEBUG)
		size_t Bound = pDesc[num-1].m_Offset + pDesc[num-1].m_Size;
		_ASSERTE(Bound <= sizeof(_Type));
#endif		
	}

	friend bool operator==(const IEClassMemberDescriptionPack& A1
		, const IEClassMemberDescriptionPack& A2)
	{
		return 0 == memcmp(&A1, &A2, sizeof(IEClassMemberDescriptionPack));
	}
	friend bool operator!=(const IEClassMemberDescriptionPack& A1
		, const IEClassMemberDescriptionPack& A2)
	{
		return ! (A1 == A2);
	}
};

#endif //_INCLUDE_IECLASSMEMBERDESCRIPTION_H_
