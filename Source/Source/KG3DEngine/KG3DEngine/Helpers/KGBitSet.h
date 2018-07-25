////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KGBitSet.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-7-5 20:47:50
//  Comment     : std::bitset������λʲô�ģ���test������Ҫ���Խ�磬�Ƚ�һ��bool
/*��if��˵������̫�ߣ������׳��쳣���ѵ���if��ʱ��try����������������뷨��
KGBitSetֻ���ڴ���ܶ��bool�����Ҳ���Ҫ�Լ���ʼ�����ڹ��캯���л��Զ�ȫ���ÿա�
test��ʱ����Ҫ��ʽ��ģ��ָ������test�ĸ�,setҲһ��
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KGBITSET_H_
#define _INCLUDE_KGBITSET_H_
#include "KGCommonWidgets.h"
////////////////////////////////////////////////////////////////////////////////
namespace KG_CUSTOM_HELPERS
{
	template<size_t _Bits>
	class KGBitSet
	{
	private:
		typedef INT _BitContainer;	//No matter how many bits are needed, we always use 32bit for bit storage, for the sack of bit alignment.
		enum
		{
			_emChatBit = 8,
			_emBitsNeeded = ((_Bits / _emChatBit) + ((_Bits % _emChatBit) == 0 ? 0 : 1)) * _emChatBit, 
			_emBitsMax = sizeof(_BitContainer) * (_emChatBit),
		};
		
	public:
		KGBitSet()
			:m_Bits(0)
		{
			
			_KG3D_DEBUG_STATIC_CHECK_T(_emBitsNeeded >= _Bits, BitsNotEnough);
			_KG3D_DEBUG_STATIC_CHECK_T(_emBitsNeeded <= _emBitsMax, BitsTooMany);
		}
		KGBitSet(_BitContainer InitFlags)
			:m_Bits(InitFlags)
		{
			_KG3D_DEBUG_STATIC_CHECK_T(_emBitsNeeded >= _Bits, BitsNotEnough);
			_KG3D_DEBUG_STATIC_CHECK_T(_emBitsNeeded <= _emBitsMax, BitsTooMany);
		}

		template<size_t _Pos>
		inline BOOL Test() const{ _KG3D_DEBUG_STATIC_CHECK_T(_Pos < _emBitsNeeded, OutOfRange); return m_Bits & (1 << _Pos);}

		template<size_t _Pos>
		inline BOOL At() const { return this->Test<_Pos>();}
		template<size_t _Pos>
		inline BOOL Is() const { return this->Test<_Pos>();}
		inline BOOL GetBOOL(size_t Pos) {_ASSERTE(Pos < _emBitsNeeded); return m_Bits & (1 << Pos);}

		template<size_t _Pos>
		inline VOID Set() { _KG3D_DEBUG_STATIC_CHECK_T(_Pos < _emBitsNeeded, OutOfRange); m_Bits |= (1 << _Pos);}
		template<size_t _Pos>
		inline VOID Reset() { _KG3D_DEBUG_STATIC_CHECK_T(_Pos < _emBitsNeeded, OutOfRange); m_Bits &= (~(1 << _Pos));}
		template<size_t _Pos>
		inline VOID SetTrue() { return this->Set<_Pos>();}
		template<size_t _Pos>
		inline VOID SetFalse() { return this->Reset<_Pos>();}
		template<size_t _Pos>
		inline VOID Set(BOOL bEnable){ _KG3D_DEBUG_STATIC_CHECK_T(_Pos < _emBitsNeeded, OutOfRange); bEnable ? (m_Bits |= (1 << _Pos)) : m_Bits &= (~(1 << _Pos));}

		inline BOOL Any() const { return m_Bits == 0? FALSE : TRUE;}
		inline BOOL None() const { return !this->Any();}

		inline INT	ToInt()const { return m_Bits;}
	private:
		_BitContainer	m_Bits;
	};
	
};
namespace KGCH = KG_CUSTOM_HELPERS;
#endif //_INCLUDE_KGBITSET_H_
