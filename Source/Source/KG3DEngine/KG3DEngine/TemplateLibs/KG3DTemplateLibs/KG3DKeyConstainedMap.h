////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DKeyConstainedMap.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-6-22 9:42:40
//  Comment     : 
/*
map��key�ǲ������Ƶģ�������[]ȡ��Ա��ʱ�������Ա�����ڻ��Զ�����������
���������map����Ҫ��ֹ������Ϊ
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DKEYCONSTAINEDMAP_H_
#define _INCLUDE_KG3DKEYCONSTAINEDMAP_H_

////////////////////////////////////////////////////////////////////////////////
namespace KG3D_CUSTOM_HELPERS
{
	/************************************************************************/
	/*����map������ͨ��[]ȡ��ֻ��ͨ��insert��ӣ�����_Key�ķ�Χ���� 
	*/
	/************************************************************************/
	enum
	{
		KG3DCC_INVALID_KEY = 0,
		KG3DCC_MIN_KEY = 1,
		KG3DCC_MAX_KEY = (INT_MAX/2),
		KG3DCC_INVALID_VALUE = (INT_MAX/2),
	};
	template<class _KeyType
		, class _ValueType
		, int nMaxKeyIndicator = KG3DCC_MAX_KEY
		, int nMinKeyIndicator = KG3DCC_MIN_KEY
		, int nInValidKeyIndicator = KG3DCC_INVALID_KEY
		, int nInValidValueIndicator = KG3DCC_INVALID_VALUE
	>
	class map_trait_maker
	{
	public:
		enum
		{
			TRAIT_KEY_MAX = nMaxKeyIndicator,
			TRAIT_KEY_MIN = nMinKeyIndicator,
			TRAIT_KEY_INVALID = nInValidKeyIndicator,
			TRAIT_VALUE_INVALID = nInValidValueIndicator,
		};		
	};
	//////////////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/*
	Ҫ������Ļ���Key������<����������ұ�����������С����ЧKey
	��ЧValue�� _ValueType������һ������һ��int��DWORD�Ĺ��캯�����ܹ�����
	�����enum����һ����Ч��ֵ����Щ������԰�װ��_TraitMaker���У������
	�����map_trait_maker������_TraitMaker��
	*/
	/************************************************************************/
	template<class _KeyType
		, class _ValueType
		, class _TraitMaker = map_trait_maker<_KeyType, _ValueType>
	>
	class key_constrained_map : public std::map<_KeyType, _ValueType>
	{
	public:
		typedef std::map<_KeyType, _ValueType> TABLE;
		typedef TABLE::iterator iterator;
	private:
		int m_nTrait; 
	public:
		key_constrained_map()
			:m_nTrait(0)
		{

		}
		key_constrained_map(int nTraitIndicator)
			:m_nTrait(nTraitIndicator)
		{

		}
		inline int GetTrait()const{return m_nTrait;}
		_ValueType& operator[](const _KeyType& _Keyval)
		{
			//��ʵ�ϣ��������_InValidKey�Ļ���Ҳ��ȡ��_InValidKey
			//���Ҫ�б𾿾��ǲ��Ƿ�����_ValueType�Ļ���ʹ��ģ�����
			iterator it = TABLE::find(_Keyval);
			if (it == TABLE::end())
			{
				_KG3D_DEBUG_SELF_ASSERTE(NULL&&"key_constrainted_mapȡ��Ч��Ԫ��");
				_KeyType tempInValidKey(_TraitMaker::TRAIT_KEY_INVALID);
				it = TABLE::find(tempInValidKey);
				if (it == TABLE::end())
				{
					_ValueType& RefValue = TABLE::operator [](tempInValidKey);
					RefValue = _ValueType(_TraitMaker::TRAIT_VALUE_INVALID);
					return RefValue;
				}
				return it->second;

			}
			return it->second;		
		}
		void clear()
		{
			return TABLE::clear();
		}
		int get_valid_element_count()
		{
			iterator it = TABLE::find(_TraitMaker::TRAIT_KEY_INVALID);
			if (it != TABLE::end())
			{
				return static_cast<int>(size()-1);
			}
			return static_cast<int>(size());
		}
		_Pairib insert(const value_type& _Val)
		{
			if(_Val.first < _TraitMaker::TRAIT_KEY_MIN || _TraitMaker::TRAIT_KEY_MAX < _Val.first)
			{
				return _Pairib(TABLE::end(), false);
			}
			return TABLE::insert(_Val);
		}
		iterator insert(iterator _Where,
			const value_type& _Val)
		{
			_ASSERTE(NULL&&"��ֹʹ��");
			return TABLE::end();
		}
		template<class _Iter>
		void insert(_Iter _First, _Iter _Last)
		{
			_ASSERTE(NULL&&"��ֹʹ��");
			NULL;
		}
	};

};

#endif //_INCLUDE_KG3DKEYCONSTAINEDMAP_H_
